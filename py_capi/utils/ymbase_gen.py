#! /usr/bin/env python3

""" ymbase モジュールを生成するスクリプト

:file: ymbase_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import ModuleGen


class YmbaseGen(ModuleGen):

    def __init__(self):
        super().__init__(modulename='ymbase',
                         pyclass_list=['PyMt19937',
                                       'PyJsonValue'],
                         include_files=['pym/PyMt19937.h',
                                        'pym/PyJsonValue.h'],
                         namespace='YM')

if __name__ == '__main__':

    gen = YmbaseGen()
    gen.make_header()
    gen.make_source()
