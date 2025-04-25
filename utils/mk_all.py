#! /usr/bin/env python3

""" ファイルを生成するスクリプト

:file: mk_all.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import os
import sys
from argparse import ArgumentParser
from pathlib import Path


# 引数の解釈
parser = ArgumentParser(prog='mk_all',
                        description="make 'ymbase' module")

parser.add_argument('-i', '--include_dir', type=str)
parser.add_argument('-s', '--source_dir', type=str)

args = parser.parse_args()


# このファイルのあるディレクトリ
curdir = os.path.dirname(__file__)
rootdir = os.path.join(curdir, '..', '..')
module_path = os.path.join(rootdir, 'ym-common', 'utils')

if args.include_dir is None:
    include_dir = os.path.join(curdir, '..', 'include', 'pym')
else:
    include_dir = args.include_dir
if args.source_dir is None:
    source_dir = os.path.join(curdir, '..', 'py_capi')
else:
    source_dir = args.source_dir

sys.path.append(module_path)


from mt19937_gen import Mt19937Gen
mt19937_gen = Mt19937Gen()

from jsonvalue_gen import JsonValueGen
jsonvalue_gen = JsonValueGen()

gen_list = [mt19937_gen, jsonvalue_gen]

from mk_py_capi import ModuleGen
module_gen = ModuleGen(modulename='ymbase',
                       pyclass_gen_list=gen_list,
                       namespace='YM')

module_gen.make_all(include_dir=include_dir,
                    source_dir=source_dir)
