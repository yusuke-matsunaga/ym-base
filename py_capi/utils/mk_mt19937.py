#! /usr/bin/env python3

""" MkMt19937 の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import MkPyCapi, IntArg


gen = MkPyCapi(classname='Mt19937',
               pyclassname='PyMt19937',
               namespace='YM',
               pyname='Mt19937',
               header_include_files=['ym_config.h'],
               source_include_files=['pym/PyMt19937.h'])

def dealloc_gen(gen):
    gen.gen_comment('実は mt19937 はクラス名ではない．')
    gen._write_line('obj->mVal.~mersenne_twister_engine();')

gen.add_dealloc(gen_body=dealloc_gen)

seed_arg = IntArg(gen,
                  name="seed",
                  option=True,
                  cvarname='seed_val',
                  cvardefault='-1')

def new_gen(gen):
    gen.gen_auto_assign('obj', 'type->tp_alloc(type, 0)')
    gen.gen_auto_assign('obj1', f'reinterpret_cast<{gen.objectname}*>(obj)')
    gen._write_line(f'new &(obj1->mVal) std::mt19937;')
    with gen.gen_if_block('seed_val != -1'):
        gen._write_line('obj1->mVal.seed(seed_val);')
    gen.gen_return('obj')

gen.add_new(arg_list=[seed_arg], gen_body=new_gen)

def gen_eval(gen):
    gen.gen_val_conv('randgen')
    gen.gen_auto_assign('val', 'randgen.operator()()')
    gen.gen_return('PyLong_FromLong(val)')

gen.add_method('eval',
               gen_body=gen_eval,
               doc_str='generate a random number')


gen.make_header()

gen.make_source()
