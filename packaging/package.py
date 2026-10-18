#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import os.path
import sys
import shutil
import hashlib
import json

import xml.etree.ElementTree as ElementTree

from enum import Enum
from contextlib import chdir

ExtensionReturnType = Enum("ExtensionReturnType", [
    ("STRING", 1),
    ("DOUBLE", 2)
])

static_files = [
    "static/metadata.json",
    "static/midim2.resource_order",
    "static/midim2.yyp"
]

metadata = dict([
    ("$GMExtension", ""),
    ("%Name", "ext_midim2"),
    ("androidactivityinject", None),
    ("androidclassname", ""),
    ("androidcodeinjection", ""),
    ("androidinject", None),
    ("androidmanifestinject", None),
    ("androidPermissions", []),
    ("androidProps", False),
    ("androidsourcedir", ""),
    ("author", ""),
    ("classname", ""),
    ("copyToTargets", 192),
    ("description", ""),
    ("exportToGame", True),
    ("extensionVersion", "1.0.0"),
    ("files", [
        dict([
            ("$GMExtensionFile", "v1"),
            ("%Name", ""),
            ("constants", []),
            ("copyToTargets", 64),
            ("filename", "libfluidsynth-3.dll"),
            ("init", ""),
            ("final", ""),
            ("functions", []),
            ("kind", 1),
            ("name", ""),
            ("origname", ""),
            ("ProxyFiles", []),
            ("resourceType", "GMExtensionFile"),
            ("resourceVersion", "2.0"),
            ("uncompress", False),
            ("usesRunnerInterface", False)
        ])
    ]),
    ("gradleinject", None),
    ("hasConvertedCodeInjection", True),
    ("helpfile", ""),
    ("HTML5CodeInjection", ""),
    ("html5Props", False),
    ("IncludedResources", []),
    ("installdir", ""),
    ("iosCocoaPodDependencies", ""),
    ("iosCocoaPods", ""),
    ("ioscodeinjection", ""),
    ("iosdelegatename", ""),
    ("iosplistinject", None),
    ("iosProps", False),
    ("iosSystemFrameworkEntries", []),
    ("iosThirdPartyFrameworkEntries", []),
    ("license", ""),
    ("maccompilerflags", ""),
    ("maclinkerflags", ""),
    ("macsourcedir", ""),
    ("name", "ext_midim2"),
    ("options", []),
    ("optionsFile", "options.json"),
    ("packageId", ""),
    ("parent", dict([
        ("name", "midim2"),
        ("path", "midim2.yyp")
    ])),
    ("productId", ""),
    ("resourceType", "GMExtension"),
    ("resourceVersion", "2.0"),
    ("sourcedir", ""),
    ("supportedTargets", -1),
    ("tvosclassname", None),
    ("tvosCocoaPodDependencies", ""),
    ("tvosCocoaPods", ""),
    ("tvoscodeinjection", ""),
    ("tvosdelegatename", None),
    ("tvosmaccompilerflags", ""),
    ("tvosmaclinkerflags", ""),
    ("tvosplistinject", None),
    ("tvosProps", False),
    ("tvosSystemFrameworkEntries", []),
    ("tvosThirdPartyFrameworkEntries", [])
])

def md5_hash_from_file(filename: str):
    file = open(filename, "rb")
    hash = hashlib.md5(file.read())
    return hash.hexdigest().upper()

def generate_yymanifest_from_files(*args: str):
    with chdir("out"):
        rootelement = ElementTree.Element("files")
        for filename in args:
            hash = md5_hash_from_file(filename)
            print(f"hash of {filename}: {hash}")
            fileelement = ElementTree.SubElement(
                rootelement, "file", md5=hash
            )
            fileelement.text = filename
        ElementTree.ElementTree(rootelement).write(
            "yymanifest.xml", encoding="utf-8",
            xml_declaration=True
        )
        print("yymanifest generation done")

def generate_metadata_file():
    pass


def copy_static_files():
    for filename in static_files:
        shutil.copy(filename, "out/")
    print("static files done")

# commands

def clean():
    if os.path.exists("out"):
        shutil.rmtree("out")
    else:
        print("nothing to clean!")

def make():
    clean()
    os.mkdir("out")
    copy_static_files()
    generate_yymanifest_from_files(
        "metadata.json",
        "midim2.resource_order",
        "midim2.yyp"
    )

if __name__ == "__main__":
    if len(sys.argv) > 1:
        match sys.argv[1]:
            case "clean": clean()
            case "make": make()
            case _:
                print("usage: package.py [make|clean]")
    else: make()