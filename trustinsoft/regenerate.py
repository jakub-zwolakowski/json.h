#! /usr/bin/env python3

# This script regenerates TrustInSoft CI configuration.

# Run from the root of the project:
# $ python3 trustinsoft/regenerate.py

import tis

import re # sub
import json # dumps, load
import os # makedirs
from os import path # path.basename, path.isdir, path.join
import glob # iglob
from itertools import product  # Cartesian product of lists.
import shutil # copyfileobj
import argparse # ArgumentParser, add_argument, parse_args

# --------------------------------------------------------------------------- #
# ----------------------------- PARSE ARGUMENTS ----------------------------- #
# --------------------------------------------------------------------------- #

parser = argparse.ArgumentParser(
    description="Regenerate the TrustInSoft CI files.",
    epilog="Please call this script only after building jansson.")
args = parser.parse_args()

# --------------------------------------------------------------------------- #
# -------------------------------- SETTINGS --------------------------------- #
# --------------------------------------------------------------------------- #

# Directories.
# common_config_path = path.join("trustinsoft", "common.config")
tis_test_config_path = path.join("trustinsoft", "tis_test.config")
include_dir = "trustinsoft"

# Generated files which need to be a part of the repository.
files_to_copy = [
    tis.make_simple_copy_file(include_dir, "generated.h"),
]

# Architectures.
machdeps = [
    {
        "machdep": "gcc_x86_32",
        "pretty_name": "little endian 32-bit (x86)",
        "fields": {
            "address-alignment": 32
        }
    },
    {
        "machdep": "gcc_x86_64",
        "pretty_name": "little endian 64-bit (x86)",
        "fields": {
            "address-alignment": 64
        }
    },
    {
        "machdep": "gcc_ppc_32",
        "pretty_name": "big endian 32-bit (PPC32)",
        "fields": {
            "address-alignment": 32
        },
    },
    {
        "machdep": "gcc_ppc_64",
        "pretty_name": "big endian 64-bit (PPC64)",
        "fields": {
            "address-alignment": 64
        },
    },
]

# --------------------------------------------------------------------------- #
# ---------------------------------- CHECKS --------------------------------- #
# --------------------------------------------------------------------------- #

# Initial check.
print("1. Check if all necessary directories and files exist...")
tis.check_dir("trustinsoft")
for file in files_to_copy:
    tis.check_file(file['src'])

# --------------------------------------------------------------------------- #
# ------------------ GENERATE trustinsoft/tis_test.config ------------------- #
# --------------------------------------------------------------------------- #

def make_tis_test_config():
    # Whole common.config JSON.
    config = {
        "files": [
            "stub.c",
            "tis_test.c",
        ],
        "machdep": "gcc_x86_64",
        "cpp-extra-args": [
            "-I..",
            "-I.",
        ],
        "cxx-cpp-extra-args": [
            "-I..",
            "-I.",
        ],
    }
    # Done.
    return config

tis_test_config = make_tis_test_config()
with open(tis_test_config_path, "w") as file:
    print("3. Generate the '%s' file." % tis_test_config_path)
    file.write(tis.string_of_json(tis_test_config))

# ---------------------------------------------------------------------------- #
# ------------------ GENERATE trustinsoft/<machdep>.config ------------------- #
# ---------------------------------------------------------------------------- #

def make_machdep_config(machdep):
    machdep_config = {
        "machdep": machdep["machdep"]
    }
    fields = machdep["fields"]
    for field in fields:
        machdep_config[field] = fields[field]
    return machdep_config

print("4. Generate 'trustinsoft/<machdep>.config' files...")
machdep_configs = map(make_machdep_config, machdeps)
for machdep_config in machdep_configs:
    file = path.join("trustinsoft", "%s.config" % machdep_config["machdep"])
    with open(file, "w") as f:
        print("   > Generate the '%s' file." % file)
        f.write(tis.string_of_json(machdep_config))

# --------------------------------------------------------------------------- #
# --------------------------- GENERATE tis.config --------------------------- #
# --------------------------------------------------------------------------- #

def make_main_cpp_test(machdep):
    test = (
        {
            "name": "FULL main.cpp, %s" % (machdep["pretty_name"]),
            "files": [
                "trustinsoft/stub.c",
                "test/main.cpp",
                "test/test.c",
                "test/allocator.cpp",
                "test/allow_c_style_comments.cpp",
                "test/allow_equals_in_object.c",
                "test/allow_global_object.c",
                "test/allow_hexadecimal_numbers.c",
                "test/allow_inf_and_nan.c",
                "test/allow_json5.c",
                "test/allow_leading_or_trailing_decimal_point.c",
                "test/allow_leading_plus_sign.c",
                "test/allow_location_information.c",
                "test/allow_multi_line_strings.c",
                "test/allow_no_commas.c",
                "test/allow_simplified_json.c",
                "test/allow_single_quoted_strings.c",
                "test/allow_trailing_comma.cpp",
                "test/allow_unquoted_keys.c",
                "test/write_minified.cpp",
                "test/write_pretty.cpp",
            ],
            "include_": path.join("trustinsoft", "%s.config" % machdep["machdep"]),
            "cpp-extra-args": [
                "-I.",
                "-Itest",
                "-Itrustinsoft",
                "-D__STDC_VERSION__=201112L",
                "-DUTEST_USE_CLOCKGETTIME",
            ],
            "cxx-cpp-extra-args": [
                "-I.",
                "-Itest",
                "-Itrustinsoft",
                "-D__STDC_VERSION__=201112L",
                "-DUTEST_USE_CLOCKGETTIME",
            ]
        }
    )
    return test

# def make_test(test_path, machdep):
#     tis_test = {
#         "name": "%s, %s" % (test_path, machdep["pretty_name"]),
#         "include": common_config_path,
#         "include_": path.join("trustinsoft", "%s.config" % machdep["machdep"]),
#         "files": [ "test/bin/json_process.c" ],
#         "val-args": (" %s" % (test_path)),
#     }
#     return tis_test


def make_tis_config():
    return (
        list(map(make_main_cpp_test, machdeps))
        +
        [
        {
            "name": "tis_test.c test_parsing/y_number.json",
            "include": "trustinsoft/tis_test.config",
            "filesystem": {
                "files": [
                    {
                        "name": "test.json",
                        "from": "trustinsoft/JSONTestSuite/test_parsing/y_number.json",
                    }
                ]
            }
        }
        ]

        # list(map(
        #     lambda t: make_process_test(t[0], t[1]),
        #     tis_process_tests
        # ))
    )

tis_config = make_tis_config()
with open("tis.config", "w") as file:
    print("5. Generate the 'tis.config' file.")
    file.write(tis.string_of_json(tis_config))

# --------------------------------------------------------------------------- #
# ------------------------------ COPY .h FILES ------------------------------ #
# --------------------------------------------------------------------------- #

print("6. Copy generated files.")
for file in files_to_copy:
    with open(file['src'], 'r') as f_src:
        os.makedirs(path.dirname(file['dst']), exist_ok=True)
        with open(file['dst'], 'w') as f_dst:
            print("   > Copy '%s' to '%s'." % (file['src'], file['dst']))
            shutil.copyfileobj(f_src, f_dst)
