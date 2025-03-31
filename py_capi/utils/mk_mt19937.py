#! /usr/bin/env python3

""" MkMt19937 の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, IntArg


seed_arg = IntArg(name="seed",
                  option=True,
                  cvarname='seed_val',
                  cvardefault='-1')


class Mt19937Gen(PyObjGen):
    
    def __init__(self):
        super().__init__(classname='Mt19937',
                         namespace='YM',
                         pyname='Mt19937',
                         header_include_files=['ymconfig.h'],
                         source_include_files=['pym/PyMt19937.h'])

        def dealloc_func(writer):
            writer.gen_comment('実は mt19937 はクラス名ではない．')
            writer.write_line('obj->mVal.~mersenne_twister_engine();')

        self.add_dealloc(dealloc_func=dealloc_func)

        def new_func(writer):
            writer.gen_auto_assign('obj', 'type->tp_alloc(type, 0)')
            writer.gen_auto_assign('obj1', f'reinterpret_cast<{gen.objectname}*>(obj)')
            writer.write_line(f'new &(obj1->mVal) std::mt19937;')
            with writer.gen_if_block('seed_val != -1'):
                writer.write_line('obj1->mVal.seed(seed_val);')
            writer.gen_return('obj')
                         
        self.add_new(arg_list=[seed_arg], func_body=new_func)

        def eval_body(writer):
            writer.gen_auto_assign('rand_val', 'val.operator()()')
            writer.gen_return('PyLong_FromLong(rand_val)')

        self.add_method('eval',
                        func_body=eval_body,
                        doc_str='generate a random number')

    
gen = Mt19937Gen()

gen.make_header()

gen.make_source()
