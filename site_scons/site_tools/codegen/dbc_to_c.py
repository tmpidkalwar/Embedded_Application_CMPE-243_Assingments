from argparse import ArgumentParser
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


def main():
    # args = get_args()
    # dbc_filepath = args.dbc
    dbc_filepath = os.path.join(os.path.dirname(__file__), "tests/test.dbc")

    if not os.path.isfile(dbc_filepath):
        print("Unable to find DBC file: [{}]".format(dbc_filepath))

    dbc = cantools.database.load_file(dbc_filepath)

    for message in dbc.messages:
        print(message)  # https://cantools.readthedocs.io/en/latest/#cantools.database.can.Message
        for signal in message.signals:
            print(signal)  # https://cantools.readthedocs.io/en/latest/#cantools.database.can.Signal

if __name__ == "__main__":
    sys.exit(main())
