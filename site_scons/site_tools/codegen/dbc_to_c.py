
from argparse import ArgumentParser
import os
import sys

from code_writer import CodeWriter


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
        exit(-1)

    code_writer = CodeWriter(dbc_filepath)
    print(code_writer)


if __name__ == "__main__":
    sys.exit(main())
