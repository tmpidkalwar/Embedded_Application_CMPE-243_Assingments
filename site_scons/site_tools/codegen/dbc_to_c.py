from argparse import ArgumentParser
from collections import defaultdict

import os
import sys

sys.path.insert(0, "site_packages")

import cantools


def get_args():
    arg_parser = ArgumentParser()
    arg_parser.add_argument(
        "--dbc",
        required=True,
    )

    return arg_parser.parse_args()


class code_writer:
    def __init__(self, stream):
        self._stream = stream

    def file_header(self, dbc_filename):
        self._stream.write('''
// AUTO-GENERATED - DO NOT EDIT
// Generated from {0}
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
'''.format(dbc_filename))

    def common_structs(self):
        self._stream.write('''
/// Missing in Action (MIA) structure
typedef struct {
  uint32_t is_mia : 1;          ///< Missing in action flag
  uint32_t mia_counter_ms : 31; ///< Missing in action counter
} dbc_mia_info_t;

typedef struct {
  uint32_t message_id;
  uint8_t message_dlc;          ///< Data length code of the CAN message
} dbc_message_header_t;
''')

    def message_header_instances(self, messages):
        self._stream.write('\n// Message headers containing CAN message IDs and their DLCs\n')
        for message in messages:
            self._stream.write('static const dbc_message_header_t dbc_header_{0}'.format(message.name).ljust(80))
            self._stream.write(' = {{ {1}, {2} }};\n'.format(
                message.name, str(message.frame_id).rjust(8), message.length))

    def structs(self, messages, generate_layout=False):
        # https://cantools.readthedocs.io/en/latest/#cantools.database.can.Message
        for message in messages:
            message_layout = ("\n"+message.layout_string() if generate_layout else "")
            signal_members = self._gen_struct_signals(message)

            self._stream.write('''
/**
 * {0}: 
 *   Sent by '{1}' {2}
 */
typedef struct {{
{3}}} dbc_{4}_s;
'''.format(message.name, message.senders[0], message_layout, signal_members, message.name))

    def encode_methods(self, messages):
        for message in messages:
            self._stream.write('''
/**
 * {0}:
 *   Sent by '{1}' with message ID {2} composed of {3} bytes
 */
static inline dbc_message_header_t dbc_encode_{4}(uint8_t bytes[8], const dbc_{5}_s *message) {{
{6}
  return dbc_header_{7};
}}
'''.format(message.name, message.senders[0], message.frame_id, message.length,
           message.name, message.name, self._get_encode_signals_code(message), message.name))

    def decode_methods(self, messages):
        for message in messages:
            validation_check = '''
  if (header.message_id != dbc_header_{0}.message_id) {{
    return !success;
  }} else if (header.message_dlc != dbc_header_{1}.message_dlc) {{
    return !success;
  }} else {{
    // DLC and message ID check is good
  }}
'''.format(message.name, message.name)

            self._stream.write('''
/**
 * {0}: Sent by {1}
 */
static inline bool dbc_decode_{2}(dbc_{3}_s *message, const dbc_message_header_t header, const uint8_t bytes[8]) {{
  const bool success = true;
{4}
{5}

  return success;
}}
'''.format(message.name, message.senders[0], message.name, message.name, validation_check, self._get_decode_signals_code(message)))

    def _get_decode_signals_code(self, message):
        code = "  uint64_t raw = 0;\n"

        if message.is_multiplexed():
            code += "  // Multiplexed signals are not handled yet\n"
            code += "  (void)raw;\n"
        else:
            for signal in message.signals:
                code += self._get_decode_signal_code(signal)
                code += '\n'

        # Remove excessive newlines from the end
        return code.rstrip()

    def _get_decode_signal_code(self, signal, raw_sig_name="raw"):
        bit_pos = signal.start
        remaining = signal.length
        byte_num = int(signal.start / 8)
        bit_count = 0
        code = ''

        while remaining > 0:
            bits_in_this_byte = min(8 - (bit_pos % 8), remaining)

            s = ""
            s += "  {0} |= ((uint64_t)((bytes[{1}] >> {2})".format(raw_sig_name, byte_num, (bit_pos % 8))
            s += " & 0x{0})) << {1}".format(format(2 ** bits_in_this_byte - 1, '02x'), bit_count)
            s += "; // {0} bits from B{1}\n".format(bits_in_this_byte, bit_pos)

            # Optimize
            s = s.replace(" >> 0", "")
            s = s.replace(" << 0", "")
            s = s.replace(" & 0xff", "")

            code += s
            if bit_count == 0:
                code = code.replace("|=", " =")

            byte_num += 1
            bit_pos += bits_in_this_byte
            remaining -= bits_in_this_byte
            bit_count += bits_in_this_byte

        # If the signal is not defined as a signed, then we will use this code
        unsigned_code = "message->{0} = (({1} * {2}) + ({3}));\n".format(signal.name, raw_sig_name, signal.scale, signal.offset)

        if signal.is_signed:
            mask = "(1 << {0})".format((signal.length - 1))
            s = ""
            s += "  if ({0} & {1}) {{ // Check signed bit\n".format(raw_sig_name, mask)
            s += "    message->{0} = ".format(signal.name)
            s += "(((((uint64_t)-1 << {0}) | {1}) * {2}) + ({3}));\n".format(str(signal.length - 1), raw_sig_name, str(signal.scale), signal.offset)
            s += "  } else {\n"
            s += "    " + unsigned_code
            s += "  }\n"
        else:
            s = "  " + unsigned_code

        # Optimize
        s = s.replace(" + (0)", "")
        s = s.replace(" * 1)", ")")
        code += s

        return code

    def _get_encode_signals_code(self, message):
        code = '''  uint64_t raw = 0;
  memset(bytes, 0, 8);

'''

        if not message.is_multiplexed():
            for signal in message.signals:
                code += self._get_encode_signal_code(signal, "raw")
                code += '\n'

        # Remove excessive newlines from the end
        return code.rstrip()

    def _get_encode_signal_code(self, signal, raw_sig_name):
        # Compute binary value
        # Encode should subtract offset then divide
        # TODO: Might have to add -0.5 for a negative signal
        raw_sig_code = "  {0} = ((uint64_t)(((message->{1} - ({2})) / {3}) + 0.5))".format(raw_sig_name, signal.name, signal.offset,
                                                                                    signal.scale)
        raw_extract = ""
        if signal.is_signed:
            raw_extract += "  // Stuff a real signed number into the DBC {0}-bit signal\n".format(signal.length)
        raw_extract += raw_sig_code + " & 0x{0};\n".format(format(2 ** signal.length - 1, '02x'))

        # Optimize
        raw_extract = raw_extract.replace(" - (0)", "")
        raw_extract = raw_extract.replace(" / 1)", ")")
        if signal.scale == 1:
            raw_extract = raw_extract.replace(" + 0.5", "")

        code = raw_extract

        # Stuff the raw data into individual bytes
        bit_pos = signal.start
        remaining = signal.length
        byte_num = int(signal.start / 8)

        while remaining > 0:
            bits_in_this_byte = min(8 - (bit_pos % 8), remaining)

            s = ""
            s += "  bytes[{0}] |= (((uint8_t)({1} >> {2})".format(byte_num , raw_sig_name, str(bit_pos - signal.start).rjust(2))
            s += " & 0x{0}) << {1})".format(format(2 ** bits_in_this_byte - 1, '02x'), str(bit_pos % 8))
            s += "; // {0} bits at B{1}\n".format(str(bits_in_this_byte), str(bit_pos))

            # Optimize
            #s = s.replace(" >> 0", "")
            s = s.replace(" << 0", "    ")

            # Cannot optimize by removing 0xff just for code safety
            #s = s.replace(" & 0xff", "")

            code += s
            byte_num += 1

            bit_pos += bits_in_this_byte
            remaining -= bits_in_this_byte

        return code

        '''
        # Min/Max check
        if signal.minimum is not None or signal.maximum is not None:
            # If signal is unsigned, and min value is zero, then do not check for '< 0'
            if not (self.is_unsigned_var() and self.min_val == 0):
                code += ("    if(" + var_name + " < " + self.min_val_str + ") { " + var_name + " = " + self.min_val_str + "; } // Min value: " + self.min_val_str + "\n")
            else:
                code += "    // Not doing min value check since the signal is unsigned already\n"
            code += ("    if(" + var_name + " > " + self.max_val_str + ") { " + var_name + " = " + self.max_val_str + "; } // Max value: " + self.max_val_str + "\n")
'''

    # https://cantools.readthedocs.io/en/latest/#cantools.database.can.Signal
    def _gen_struct_signals(self, message):
        signals_string = ""

        if not message.is_multiplexed():
            for signal in message.signals:
                type_and_name = "{0} {1}".format(self._get_signal_type(signal), signal.name).ljust(40)
                signals_string += "  {0}; // Scale={1}\n".format(type_and_name, signal.scale)
        else:
            # For a muxed message, first generate non-mux symbols (which includes the mux itself)
            for signal in message.signals:
                if signal.multiplexer_ids is None:
                    type_and_name = "{0} {1}".format(self._get_signal_type(signal), signal.name).ljust(40)
                    signals_string += "  {0}; // Non-muxed signal\n".format(type_and_name)

            # Create a dictionary of a list where the key is the mux symbol
            muxed_signals = self._get_muxed_signals(message)

            for mux_id in muxed_signals:
                signals_string += '\n'
                for signal in muxed_signals[mux_id]:
                    type_and_name = "{0} {1}".format(self._get_signal_type(signal), signal.name).ljust(40)
                    signals_string += "  {0}; // M{1}, Scale={2}\n".format(type_and_name, mux_id, signal.scale)


        return signals_string

    def _get_muxed_signals(self, message):
        # Create a dictionary of a list where the key is the mux symbol
        muxed_signals = defaultdict(list)

        for signal in message.signals:
            if signal.multiplexer_ids is not None:
                muxed_signals[signal.multiplexer_ids[0]].append(signal)

        return muxed_signals

    def _get_signal_type(self, signal):
        t = "float";

        if (signal.scale * 1.0).is_integer():
            _max = (2 ** signal.length) * signal.scale
            if signal.is_signed:
                _max *= 2

            t = "uint32_t"
            if _max <= 256:
                t = "uint8_t"
            elif _max <= 65536:
                t = "uint16_t"

            # If the signal is signed, or the offset is negative, remove "u" to use "int" type.
            if signal.is_signed or signal.offset < 0:
                t = t[1:]

        return t


def main():
    # args = get_args()
    # dbc_filepath = args.dbc
    dbc_filepath = os.path.join(os.path.dirname(__file__), "tests/test.dbc")

    if not os.path.isfile(dbc_filepath):
        print("Unable to find DBC file: [{}]".format(dbc_filepath))
        exit(-1)

    dbc = cantools.database.load_file(dbc_filepath)
    cw = code_writer(sys.stdout)
    cw.file_header(dbc_filepath)
    cw.common_structs()
    cw.message_header_instances(dbc.messages)
    cw.structs(dbc.messages)
    cw.encode_methods(dbc.messages)
    cw.decode_methods(dbc.messages)


if __name__ == "__main__":
    sys.exit(main())
