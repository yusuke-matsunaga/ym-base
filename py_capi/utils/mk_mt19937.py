#! /usr/bin/env python3

""" MkMt19937 の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import MkPyCapi, IntArg
from mk_py_capi import FuncGen


gen = MkPyCapi(classname='Mt19937',
               pyclassname='PyMt19937',
               namespace='YM',
               pyname='Mt19937',
               header_include_files=['ym_config.h'],
               source_include_files=['pym/PyMt19937.h'])

def dealloc_func(writer):
    writer.gen_comment('実は mt19937 はクラス名ではない．')
    writer.write_line('obj->mVal.~mersenne_twister_engine();')

gen.add_dealloc(dealloc_func=dealloc_func)

seed_arg = IntArg(name="seed",
                  option=True,
                  cvarname='seed_val',
                  cvardefault='-1')

def new_body(writer):
    writer.gen_auto_assign('obj', 'type->tp_alloc(type, 0)')
    writer.gen_auto_assign('obj1', f'reinterpret_cast<{gen.objectname}*>(obj)')
    writer.write_line(f'new &(obj1->mVal) std::mt19937;')
    with writer.gen_if_block('seed_val != -1'):
        writer.write_line('obj1->mVal.seed(seed_val);')
    writer.gen_return('obj')

gen.add_new(arg_list=[seed_arg], gen_body=new_body)


def eval_body(writer):
    writer.gen_ref_conv(refname='randgen')
    writer.gen_auto_assign('val', 'randgen.operator()()')
    writer.gen_return('PyLong_FromLong(val)')

        
gen.add_method('eval',
               gen_body=eval_body,
               doc_str='generate a random number')

gen.make_header()

gen.make_source()
