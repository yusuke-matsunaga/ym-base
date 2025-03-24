#! /usr/bin/env python3

""" MkMt19937 の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import MkPyCapi, DeallocGen, MethodGen, NewGen, IfBlock, ArgInfo


class MyNewGen(NewGen):
    """PyMt19937 用の new 関数生成用のクラス"""

    def __init__(self, parent):
        arg_list = []
        arg_list.append(ArgInfo(name="seed",
                                option=True,
                                pchar='i',
                                ptype=None,
                                cvartype='int',
                                cvarname='seed_val',
                                cvardefault='-1'))
        super().__init__(parent, arg_list=arg_list)

    def bodygen(self):
        self._write_line('auto obj = type->tp_alloc(type, 0);')
        self._write_line(f'auto obj1 = reinterpret_cast<{self.objectname}*>(obj);')
        self._write_line(f'new &(obj1->mVal) std::mt19937;')
        with IfBlock(self.parent,
                     condition='seed_val != -1'):
            self._write_line('obj1->mVal.seed(seed_val);')
        self._write_line('return obj')
        
    
class MyDeallocGen(DeallocGen):
    """PyMt19937 用の dealloc 関数生成用クラス"""
    
    def __init__(self, parent):
        super().__init__(parent)

    def bodygen(self):
        self._write_line('// 実は mt19937 はクラス名ではない．')
        self._write_line('obj->mVal.~mersenne_twister_engine()')

class EvalGen(MethodGen):
    """Mt19937.eval() 関数の実装を行うクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='eval',
                         func_name='eval',
                         arg_list=[],
                         is_static=False,
                         doc_str='generate a random number')

    def bodygen(self):
        self._write_line('auto& randgen = PyMt19937::_get_ref(self);')
        self._write_line('auto val = randgen.operator()();')
        self._write_line('return PyLong_FromLong(val);')
        

gen = MkPyCapi(classname='Mt19937',
               pyclassname='PyMt19937',
               namespace='YM',
               pyname='Mt19937',
               header_include_files=['ym_config.h', ])

gen.dealloc_gen = MyDeallocGen(gen)
gen.new_gen = MyNewGen(gen)
gen.add_method(EvalGen(gen))

gen.make_header()

gen.make_source()
