from SCons.Script import *

import os
import subprocess
import sys

SELF_DIR = Dir(os.path.dirname(__file__))
GENERATE_SYMBOL_TABLE_PY = SELF_DIR.File("generate_symbol_table.py")


"""
SCons tools functions
"""


def generate(env):
    env.AddMethod(symbol_table_generator_method, "SymbolTableGenerator")


def exists(env):
    return True


def symbol_table_generator_method(env, source, target):
    target = Dir(target)
    elf_filenode = File(source)

    basename, _ = os.path.splitext(elf_filenode.name)
    output_filenodes = []
    output_filenodes.append(target.File("{}.{}".format(basename, "json")))
    output_filenodes.append(target.File("{}.{}".format(basename,"log")))

    command = [
        "python",
        GENERATE_SYMBOL_TABLE_PY.abspath,
        "--elf=$SOURCE",
        "--output=${TARGETS[0]}",
        "--log-output=${TARGETS[1]}",
    ]

    command = " ".join(command)

    result = env.Command(action=command, source=elf_filenode.abspath, target=output_filenodes)

    return result
