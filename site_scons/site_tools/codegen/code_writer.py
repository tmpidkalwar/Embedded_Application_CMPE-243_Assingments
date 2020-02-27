from collections import defaultdict
try:
    from StringIO import StringIO
except ImportError:
    from io import StringIO
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "site_packages"))

import cantools

"""
References:
    https://cantools.readthedocs.io/en/latest/#cantools.database.can.Message
    https://cantools.readthedocs.io/en/latest/#cantools.database.can.Signal
"""

ENUM_SUFFIX = "_e"


class CodeWriter(object):
    def __init__(self, dbc_filepath):
        self._stream = StringIO()
        self._dbc_filepath = dbc_filepath
        self._dbc = cantools.database.load_file(dbc_filepath)
        self._messages = self._dbc.messages

        self._file_header()
        self._common_structs()
        self._generate_enums()
        self._message_header_instances()
        self._structs()
        self._encode_methods()
        self._decode_methods()
        self._write_footer()

    def __str__(self):
        return self._stream.getvalue()

    def _file_header(self):
        dbc_filename = os.path.basename(self._dbc_filepath)

        self._stream.write((
            "// clang-format off\n"
            "// AUTO-GENERATED - DO NOT EDIT\n"
            "// Generated from {0}\n"
            "#pragma once\n"
            "\n"
            "#include <stdbool.h>\n"
            "#include <stdint.h>\n"
            "#include <string.h>\n"
            "\n"
            "#ifndef MIN_OF(x,y)\n"
            "#define MIN_OF(x,y) (x) < (y) ? (x) : (y)\n"
            "#endif\n"
            "#ifndef MAX_OF(x,y)\n"
            "#define MAX_OF(x,y) (x) > (y) ? (x) : (y)\n"
            "#endif\n"
            "\n"
        ).format(dbc_filename))

    def _write_footer(self):
        self._stream.write(
            "\n"
            "// clang-format off\n"
             "\n"
        )

    def _common_structs(self):
        self._stream.write(
            "/// Missing in Action (MIA) structure\n"
            "typedef struct {\n"
            "  uint32_t is_mia : 1;          ///< Flag indicating that message is MIA\n"
            "  uint32_t mia_counter_ms : 31; ///< Counter used to track MIA\n"
            "} dbc_mia_info_t;\n"
            "\n"
            "typedef struct {\n"
            "  uint32_t message_id;\n"
            "  uint8_t message_dlc;          ///< Data Length Code of the CAN message\n"
            "} dbc_message_header_t;\n"
        )

    def _generate_enums(self):
        code = ""
        choices = {}

        for message in self._messages:
            for signal in message.signals:
                if signal.choices is not None:
                    choices[signal.name] = signal.choices

        # signal_name is the key',
        # enum_list is:
        # OrderedDict([(2, 'DRIVER_HEARTBEAT_cmd_REBOOT'), (1, 'DRIVER_HEARTBEAT_cmd_SYNC'), (0, 'DRIVER_HEARTBEAT_cmd_NOOP')])
        for signal_name, enum_list in choices.items():
            values = ""
            for value, enum_item_name in enum_list.items():
                values += "  {0} = {1},\n".format(enum_item_name, value)

            code += (
                "\n"
                "// Enumeration for {0}\n"
                "typedef enum {{\n"
                "{1}"
                "}} {2}{3};\n"
            ).format(signal_name, values, signal_name, ENUM_SUFFIX)

        self._stream.write(code)

    def _message_header_instances(self):
        self._stream.write('\n// Message headers containing CAN message IDs and their DLCs\n')
        for message in self._messages:
            self._stream.write('static const dbc_message_header_t dbc_header_{0}'.format(message.name).ljust(80))
            self._stream.write(' = {{ {1}U, {2} }};\n'.format(
                message.name, str(message.frame_id).rjust(8), message.length))

    def _structs(self, generate_layout=False):
        for message in self._messages:
            message_layout = ("\n" + message.layout_string() if generate_layout else "")
            signal_members = self._generate_struct_signals(message)

            self._stream.write((
                "\n"
                "/**\n"
                " * {0}: \n"
                " *   Sent by '{1}' {2}\n"
                " */\n"
                "typedef struct {{\n"
                "{3}}} dbc_{4}_s;\n"
            ).format(message.name, message.senders[0], message_layout, signal_members, message.name))

    def _decode_methods(self):
        for message in self._messages:
            validation_check = (
                "\n"
                "  if (header.message_id != dbc_header_{0}.message_id) {{\n"
                "    return !success;\n"
                "  }} else if (header.message_dlc != dbc_header_{1}.message_dlc) {{\n"
                "    return !success;\n"
                "  }} else {{\n"
                "    // DLC and message ID check is good\n"
                "  }}\n"
            ).format(message.name, message.name)

            self._stream.write((
                "\n"
                "/**\n"
                " * {0}: Sent by {1}\n"
                " */\n"
                "static inline bool dbc_decode_{2}(dbc_{3}_s *message, const dbc_message_header_t header, const uint8_t bytes[8]) {{\n"
                "  const bool success = true;\n"
                "{4}\n"
                "{5}\n"
                "\n"
                "  return success;\n"
                "}}\n"
            ).format(message.name, message.senders[0], message.name, message.name, validation_check, self._get_decode_signals_code(message)))

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
        unsigned_code = "message->{0} = (({1} * {2}f) + ({3}));\n".format(signal.name, raw_sig_name, signal.scale,
                                                                         signal.offset)

        if signal.is_signed:
            mask = "(1 << {0})".format((signal.length - 1))
            s = ""
            s += "  if ({0} & {1}) {{ // Check signed bit of the raw DBC signal and sign extend from 'raw'\n".format(raw_sig_name, mask)
            s += "    message->{0} = ".format(signal.name)

            # Create signed extended number by first getting a type similar to '(int16_t)-1'
            # The only corner case is that this will not work for float, but float should not
            # be listed as a signed number
            signed_max = "(({0})-1)".format(self._get_signal_type(signal))
            s += "((((({0} << {1}) | {2}) * {3}f) + ({4}));\n".format(
                signed_max, str(signal.length - 1), raw_sig_name, str(signal.scale), signal.offset
            )

            s += ("  }} else {{\n"
                  "    {0}"
                  "  }}\n").format(unsigned_code)
        else:
            s = "  " + unsigned_code

        # Optimize
        s = s.replace(" + (0)", "")
        s = s.replace(" * 1f)", ")")
        code += s

        return code

    def _encode_methods(self):
        for message in self._messages:
            encode_code = self._get_encode_signals_code(message)

            self._stream.write((
                "\n"
                "/**\n"
                " * {0}:\n"
                " *   Sent by '{1}' with message ID {2} composed of {3} bytes\n"
                " */\n"
                "static inline dbc_message_header_t dbc_encode_{4}(uint8_t bytes[8], const dbc_{5}_s *message) {{\n"
                "{6}\n"
                "\n"
                "  return dbc_header_{7};\n"
                "}}\n"
            ).format(message.name, message.senders[0], message.frame_id, message.length, message.name, message.name, encode_code, message.name))

    def _get_encode_signals_code(self, message):
        code = "  uint64_t raw = 0;\n"\
               "  memset(bytes, 0, 8);\n"\
               "\n"

        if message.is_multiplexed():
            code += "  // Multiplexed signals are not handled yet\n"
            code += "  (void)raw;\n\n"
        else:
            for signal in message.signals:
                code += self._get_encode_signal_code(signal, "raw")
                code += '\n'

        # Remove excessive newlines from the end
        return code.rstrip()

    def _get_encode_signal_code(self, signal, raw_sig_name):
        # Compute binary value
        # Encode should subtract offset then divide
        # TODO: Might have to add -0.5 for a negative signal

        raw_sig_code = ""
        min_max_comment = ""
        if signal.minimum is None and signal.maximum is None:
            raw_sig_code = "  {0} = ((uint64_t)(((message->{1} - ({2})) / {3}f) + 0.5f))".format(
                raw_sig_name, signal.name, signal.offset, signal.scale
            )
        else:
            min_max_comment = " within range of [{0} -> {1}]".format(signal.minimum, signal.maximum)

            # Cast to int64_t or higher type may be required becuase we may have int8_t signal with
            # value of 100, with an offset of -200 which will roll over and produce incorrect result
            if self._get_signal_type(signal) == "float":
                cast = ""
            else:
                cast = "(int64_t)"

            raw_sig_code = "  {0} = ((uint64_t)(((MAX_OF(MIN_OF({1}message->{2},{3}),{4}) - ({5})) / {6}f) + 0.5f))".format(
                raw_sig_name, cast, signal.name, signal.maximum, signal.minimum, signal.offset, signal.scale
            )

        offset_string = ""
        if signal.offset != 0:
            offset_string = " and offset={0}".format(signal.offset)

        signal_comment = ""
        if signal.is_signed:
            signal_comment = "  // Encode to raw {0}-bit SIGNED signal with scale={1}{2}{3}\n".format(signal.length, signal.scale, offset_string, min_max_comment)
        else:
            signal_comment = "  // Encode to raw {0}-bit signal with scale={1}{2}{3}\n".format(signal.length, signal.scale, offset_string, min_max_comment)

        raw_extract = raw_sig_code + " & 0x{0};\n".format(format(2 ** signal.length - 1, '02x'))

        # Optimize
        raw_extract = raw_extract.replace(" - (0)", "")
        raw_extract = raw_extract.replace(" / 1f)", ")")
        if signal.scale == 1:
            raw_extract = raw_extract.replace(" + 0.5f", "")

        code = signal_comment + raw_extract

        # Stuff the raw data into individual bytes
        bit_pos = signal.start
        remaining = signal.length
        byte_num = int(signal.start / 8)

        while remaining > 0:
            bits_in_this_byte = min(8 - (bit_pos % 8), remaining)

            s = ""
            s += "  bytes[{0}] |= (((uint8_t)({1} >> {2})".format(byte_num, raw_sig_name,
                                                                  str(bit_pos - signal.start).rjust(2))
            s += " & 0x{0}) << {1})".format(format(2 ** bits_in_this_byte - 1, '02x'), str(bit_pos % 8))
            s += "; // {0} bits at B{1}\n".format(str(bits_in_this_byte), str(bit_pos))

            # Optimize
            s = s.replace(" >> 0", "")
            s = s.replace(" << 0", "     ")

            # Cannot optimize by removing 0xff just for code safety
            # s = s.replace(" & 0xff", "")

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

    def _generate_struct_signals(self, message):
        signals_string = ""

        if not message.is_multiplexed():
            for signal in message.signals:
                type_and_name = "{0} {1}".format(self._get_signal_type(signal), signal.name)
                comment = "" if signal.comment is None else " // " + signal.comment
                signals_string += "  {0};{1}\n".format(type_and_name, comment)
        else:
            # For a muxed message, first generate non-mux symbols (which includes the mux itself)
            for signal in message.signals:
                if signal.multiplexer_ids is None:
                    type_and_name = "{0} {1}".format(self._get_signal_type(signal), signal.name)
                    signals_string += "  {0}; // Non-muxed signal\n".format(type_and_name)

            # Create a dictionary of a list where the key is the mux symbol
            muxed_signals = self._get_muxed_signals(message)

            for mux_id in muxed_signals:
                signals_string += '\n'
                for signal in muxed_signals[mux_id]:
                    type_and_name = "{0} {1}".format(self._get_signal_type(signal), signal.name)
                    signals_string += "  {0}; // M{1}\n".format(type_and_name, mux_id)

        return signals_string

    def _get_muxed_signals(self, message):
        # Create a dictionary of a list where the key is the mux symbol
        muxed_signals = defaultdict(list)

        for signal in message.signals:
            if signal.multiplexer_ids is not None:
                muxed_signals[signal.multiplexer_ids[0]].append(signal)

        return muxed_signals

    def _get_signal_type(self, signal):
        signal_type = "float";

        if signal.choices is not None:
            signal_type = signal.name + ENUM_SUFFIX
        elif (signal.scale * 1.0).is_integer():
            max_value = (2 ** signal.length) * signal.scale
            if signal.is_signed:
                max_value *= 2

            signal_type = "uint32_t"
            if max_value <= 256:
                signal_type = "uint8_t"
            elif max_value <= 65536:
                signal_type = "uint16_t"

            # If the signal is signed, or the offset is negative, remove "u" to use "int" type.
            if signal.is_signed or signal.offset < 0:
                signal_type = signal_type[1:]
        else:
            signal_type = "float"

        return signal_type
