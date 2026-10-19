#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import os.path
import sys
import shutil
import hashlib
import json
import zipfile

import xml.etree.ElementTree as ElementTree

from enum import Enum
from contextlib import chdir
from array import array

static_files = [
    "static/metadata.json",
    "static/midim2.resource_order",
    "static/midim2.yyp"
]

library_files = [
    "../out/mingw/libfluidsynth-3.dll",
    "../out/mingw/libmidim2.dll",
    "../out/native/libfluidsynth.so.3",
    "../out/native/libmidim2.so"
]

def extension_func(name: str, returnType: int, 
                   args: array[int]):
    return dict([
        ("$GMExtensionFunction", ""),
        ("%Name", name),
        ("argCount", 0), # always zero?
        ("args", args),
        ("documentation", ""),
        ("externalName", name),
        ("help", ""),
        ("hidden", False),
        ("kind", 1), # unused afaik
        ("name", name),
        ("resourceType", "GMExtensionFunction"),
        ("resourceVersion", "2.0"),
        ("returnType", returnType)
    ])

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
            ("copyToTargets", 192),
            ("filename", "libmidim2.dll"),
            ("init", "mp_init"),
            ("final", "mp_deinit"),
            ("functions", [
                extension_func("mp_set_use_system_patches", 
                               2, [2]),
                extension_func("mp_set_alt_sf_path",
                               2, [1]),
                extension_func("mp_play", 2, [1]),
                extension_func("mp_pause", 2, []),
                extension_func("mp_resume", 2, []),
                extension_func("mp_unload", 2, []),
                extension_func("mp_seek", 2, [2]),
                extension_func("mp_restart", 2, []),
                extension_func("mp_set_bpm", 2, [2]),
                extension_func("mp_get_bpm", 2, []),
                extension_func("mp_get_division", 2, []),
                extension_func("mp_get_tempo", 2, []),
                extension_func("mp_get_length_ticks", 2, []),
                extension_func("mp_get_pos_ticks", 2, []),
                extension_func("mp_set_gain", 2, [2]),
                extension_func("mp_get_gain", 2, []),
                extension_func("mp_set_loop_count", 2, [2]),
                extension_func("mp_wait_until_finished", 2, []),
                extension_func("mp_init", 2, []),
                extension_func("mp_setup", 2, []),
                extension_func("mp_deinit", 2, [])
            ]),
            ("kind", 1),
            ("name", ""),
            ("origname", ""),
            ("ProxyFiles", [
                dict([
                    ("$GMProxyFile", ""),
                    ("%Name", "libmidim2.so"),
                    ("name", "libmidim2.so"),
                    ("resourceType", "GMProxyFile"),
                    ("resourceVersion", "2.0"),
                    ("TargetMask", 7)
                ])
            ]),
            ("resourceType", "GMExtensionFile"),
            ("resourceVersion", "2.0"),
            ("uncompress", False),
            ("usesRunnerInterface", False)
        ]),
        dict([
            ("$GMExtensionFile", "v1"),
            ("%Name", ""),
            ("constants", []),
            ("copyToTargets", 192),
            ("filename", "libfluidsynth-3.dll"),
            ("init", ""),
            ("final", ""),
            ("functions", []),
            ("kind", 1),
            ("name", ""),
            ("origname", ""),
            ("ProxyFiles", [
                dict([
                    ("$GMProxyFile", ""),
                    ("%Name", "libfluidsynth.so.3"),
                    ("name", "libfluidsynth.so.3"),
                    ("resourceType", "GMProxyFile"),
                    ("resourceVersion", "2.0"),
                    ("TargetMask", 7)
                ])
            ]),
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
            xml_declaration=True)
        
        print("yymanifest generation done")

def generate_metadata_file():
    out = json.dumps(metadata)
    outfile = open("out/extensions/ext_midim2/ext_midim2.yy", "w")
    outfile.write(out)
    outfile.close()
    print("manifest (\".yy\" file) generation done")


def copy_static_files():
    for filename in static_files:
        shutil.copy(filename, "out/")
    print("static files done")

def copy_libraries():
    if not os.path.exists("../out/native"):
        print("please run build_release_native.sh!")
        exit(1)
    if not os.path.exists("../out/mingw"):
        print("please run build_release_mingw.sh!")
        exit(1)

    for filename in library_files:
        shutil.copy(filename, "out/extensions/ext_midim2/")

    print("library copying done")

def zip_package():
    shutil.make_archive("ext_midim2", "zip", "out/")
    os.rename("ext_midim2.zip", "ext_midim2.yymps")
    print("zipped up!")


# commands

def clean():
    if os.path.exists("out"):
        shutil.rmtree("out")
    else:
        print("out directory doesn't exist, not cleaning")
    
    if os.path.exists("ext_midim2.yymps"):
        os.remove("ext_midim2.yymps")
    else:
        print("final archive doesn't exist, not cleaning")

def make():
    clean()
    os.mkdir("out")
    copy_static_files()
    
    os.mkdir("out/extensions")
    os.mkdir("out/extensions/ext_midim2")
    copy_libraries()
    generate_metadata_file()

    generate_yymanifest_from_files(
        "metadata.json",
        "midim2.resource_order",
        "midim2.yyp",
        "extensions/ext_midim2/ext_midim2.yy",
        "extensions/ext_midim2/libfluidsynth-3.dll",
        "extensions/ext_midim2/libmidim2.dll",
        "extensions/ext_midim2/libfluidsynth.so.3",
        "extensions/ext_midim2/libmidim2.so"
    )

    zip_package()

if __name__ == "__main__":
    if len(sys.argv) > 1:
        match sys.argv[1]:
            case "clean": clean()
            case "make": make()
            case _:
                print("usage: package.py [make|clean]")
    else: make()
