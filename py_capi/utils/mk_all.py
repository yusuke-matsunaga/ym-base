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


# このファイルのあるディレクトリ
curdir = os.path.dirname(__file__)
rootdir = os.path.join(curdir, '..', '..', '..')
module_path = os.path.join(rootdir, 'ym-common', 'utils')

sys.path.append(module_path)

parser = ArgumentParser(prog='mk_ymbase',
                        description="make 'ymbase' module")

parser.add_argument('-i', '--include_dir', type=str)
parser.add_argument('-s', '--source_dir', type=str)

args = parser.parse_args()

if args.include_dir is None:
    include_dir = os.path.join(curdir, '..', '..', 'include', 'pym')
else:
    include_dir = args.include_dir
if args.source_dir is None:
    source_dir = os.path.join(curdir, '..')
else:
    source_dir = args.source_dir

from ymbase_gen import YmbaseGen
ymbase_gen = YmbaseGen()
filename = os.path.join(include_dir, 'ymbase.h')
with open(filename, 'wt') as fout:
    ymbase_gen.make_header(fout=fout)
filename = os.path.join(source_dir, 'ymbase_module.cc')
with open(filename, 'wt') as fout:
    ymbase_gen.make_source(fout=fout)

from mt19937_gen import Mt19937Gen
mt19937_gen = Mt19937Gen()

from jsonvalue_gen import JsonValueGen
jsonvalue_gen = JsonValueGen()
for gen, name in ((mt19937_gen, 'PyMt19937'),
                  (jsonvalue_gen, 'PyJsonValue')):
    filename = os.path.join(include_dir, f'{name}.h')
    with open(filename, 'wt') as fout:
        gen.make_header(fout=fout)
    filename = os.path.join(source_dir, f'{name}.cc')
    with open(filename, 'wt') as fout:
        gen.make_source(fout=fout)
