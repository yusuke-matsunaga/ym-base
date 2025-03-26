#! /usr/bin/env python3

""" MkMt19937 の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import MkPyCapi, DeallocGen, MethodGen, NewGen, IntArg


class MyNewGen(NewGen):
    """PyMt19937 用の new 関数生成用のクラス"""

    def __init__(self, parent):
        seed_arg = IntArg(parent,
                          name="seed",
                          option=True,
                          cvarname='seed_val',
                          cvardefault='-1')
        arg_list = [seed_arg]
        super().__init__(parent, arg_list=arg_list)

    def gen_body(self):
        self.gen_auto_assign('obj', 'type->tp_alloc(type, 0)')
        self.gen_auto_assign('obj1', f'reinterpret_cast<{self.objectname}*>(obj)')
        self._write_line(f'new &(obj1->mVal) std::mt19937;')
        with self.gen_if_block('seed_val != -1'):
            self._write_line('obj1->mVal.seed(seed_val);')
        self.gen_return('obj')
        
    
class MyDeallocGen(DeallocGen):
    """PyMt19937 用の dealloc 関数生成用クラス"""
    
    def __init__(self, parent):
        super().__init__(parent)

    def gen_body(self):
        self.gen_comment('実は mt19937 はクラス名ではない．')
        self._write_line('obj->mVal.~mersenne_twister_engine();')

        
class EvalGen(MethodGen):
    """Mt19937.eval() 関数の実装を行うクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='eval',
                         doc_str='generate a random number')

    def gen_body(self):
        self.gen_val_conv('randgen')
        self.gen_auto_assign('val', 'randgen.operator()()')
        self.gen_return('PyLong_FromLong(val)')
        

gen = MkPyCapi(classname='Mt19937',
               pyclassname='PyMt19937',
               namespace='YM',
               pyname='Mt19937',
               header_include_files=['ym_config.h'],
               source_include_files=['pym/PyMt19937.h'])

MyDeallocGen(gen)
MyNewGen(gen)
EvalGen(gen)

gen.make_header()

gen.make_source()
