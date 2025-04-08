#! /usr/bin/env python3

""" Mt19937Gen の定義ファイル

:file: mt19937_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen, IntArg


class Mt19937Gen(PyObjGen):
    
    def __init__(self):
        super().__init__(classname='std::mt19937',
                         pyclassname='PyMt19937',
                         namespace='YM',
                         pyname='Mt19937',
                         header_include_files=['ym_config.h',
                                               '<random>'],
                         source_include_files=['pym/PyMt19937.h'])

        seed_arg = IntArg(name="seed",
                          option=True,
                          cvarname='seed_val',
                          cvardefault='-1')

        def dealloc_func(writer):
            writer.gen_comment('実は mt19937 はクラス名ではない．')
            writer.write_line('obj->mVal.~mersenne_twister_engine();')

        self.add_dealloc(func_body=dealloc_func)

        def new_func(writer):
            writer.gen_auto_assign('obj', 'type->tp_alloc(type, 0)')
            writer.gen_auto_assign('obj1', f'reinterpret_cast<{self.objectname}*>(obj)')
            writer.write_line(f'new (&obj1->mVal) std::mt19937;')
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

        
if __name__ == '__main__':
    gen = Mt19937Gen()
    gen.make_header()
    gen.make_source()
