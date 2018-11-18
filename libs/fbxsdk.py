#!/usr/bin/env python
# -*- coding: utf-8 -*-

from itertools import repeat, chain, filterfalse
import string
import os
from os.path import join, isdir
import shutil
from ctypes import windll


def unique_everseen(iterable, key=None):
	"""List unique elements, preserving order. Remember all elements ever seen"""
	seen = set()
	seen_add = seen.add
	if key is None:
		for element in filterfalse(seen.__contains__, iterable):
			seen_add(element)
			yield element
	else:
		for element in iterable:
			k = key(element)
			if k not in seen:
				seen_add(k)
				yield element


def _get_drives():
	bitmask = windll.kernel32.GetLogicalDrives()
	return [letter for i, letter in enumerate(string.ascii_uppercase) if (bitmask >> i) & 1]


def _iter_search_env_path():
	path = os.environ.get("PATH", "")
	for dirpath in filter(None, map(str.strip, path.split(";"))):
		if any(basename in dirpath.lower() for basename in ("fbx", "autodesk")):
			yield dirpath


def _iter_search_directories():
	dirpaths = [
		"Autodesk/FBX/FBX SDK",
		"Program Files/Autodesk/FBX/FBX SDK",
		"Program Files (x86)/Autodesk/FBX/FBX SDK",
	]
	for letter in _get_drives():
		path = f"{letter}:/"
		if not isdir(path):
			continue
		for path in map(join, repeat(path), dirpaths):
			if isdir(path):
				yield path


def _iter_search():
	yield from unique_everseen(chain(_iter_search_env_path(), _iter_search_directories()))


def get_fbx_sdk_versions():
	versions = list(chain.from_iterable(map(join, repeat(dirpath), os.listdir(dirpath)) for dirpath in _iter_search()))
	versions.sort()
	versions.sort(key=os.path.basename, reverse=True)
	return versions


def find_fbx_sdk():
	# TODO: If no path is found, then prompt user to input it
	return get_fbx_sdk_versions()[0]


def setup_fbx_sdk():
	assert isdir(join("libs"))
	if isdir("libs/fbxsdk"):
		return
	fbx_sdk_path = find_fbx_sdk()
	print("Copying headers...", flush=True)
	shutil.copytree(join(fbx_sdk_path, "include"), "libs/fbxsdk/include")
	print("Copying libraries...", flush=True)
	shutil.copytree(join(fbx_sdk_path, "lib"), "libs/fbxsdk/lib")
	shutil.copyfile(join(fbx_sdk_path, "lib/vs2015/x64/debug/libfbxsdk.dll"), "libfbxsdk.dll")


if __name__ == "__main__":
	assert os.name == "nt"

	os.chdir(join(os.path.dirname(__file__), os.pardir))
	setup_fbx_sdk()
