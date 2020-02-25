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

    def mia_struct(self):
        self._stream.write('''
/// Missing in Action (MIA) structure
typedef struct {
  uint32_t is_mia : 1;          ///< Missing in action flag
  uint32_t mia_counter_ms : 31; ///< Missing in action counter
} dbc_mia_info_t;
''')

    def structs(self, messages, generate_layout=False):
        # https://cantools.readthedocs.io/en/latest/#cantools.database.can.Message
        for message in messages:
            message_layout = ("\n"+message.layout_string() if generate_layout else "")
            signal_members = self._gen_struct_signals(message)

            self._stream.write('''
/**
 * {0}: Sent by {1} {2}
 */
typedef struct {{
{3}}} dbc_{4}_s;
'''.format(message.name, message.senders[0], message_layout, signal_members, message.name))

    # https://cantools.readthedocs.io/en/latest/#cantools.database.can.Signal
    def _gen_struct_signals(self, message):
        signals_string = ""

        if not message.is_multiplexed():
            for signal in message.signals:
                type_and_name = "{0} {1}".format(self._get_signal_type(signal), signal.name).ljust(40)
                signals_string += "  {0}; // Scale={1}\n".format(type_and_name, signal.scale)
        else:
            for signal in message.signals:
                if signal.multiplexer_ids is None:
                    type_and_name = "{0} {1}".format(self._get_signal_type(signal), signal.name).ljust(40)
                    signals_string += "  {0}; // Non-muxed symbol\n".format(type_and_name)

            muxed_signals = defaultdict(list)
            for signal in message.signals:
                if signal.multiplexer_ids is not None:
                    muxed_signals[signal.multiplexer_ids[0]].append(signal)

            for mux_id in muxed_signals:
                signals_string += '\n'
                for signal in muxed_signals[mux_id]:
                    type_and_name = "{0} {1}".format(self._get_signal_type(signal), signal.name).ljust(40)
                    signals_string += "  {0}; // M{1}, Scale={2}\n".format(type_and_name, mux_id, signal.scale)


        return signals_string

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
    #cw.mia_struct()
    cw.structs(dbc.messages)


if __name__ == "__main__":
    sys.exit(main())
