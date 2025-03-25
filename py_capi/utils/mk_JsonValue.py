#! /usr/bin/env python3

""" MkJsonValue の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import MkPyCapi
from mk_py_capi import ReprGen, DeallocGen, MethodGen, NewGen
from mk_py_capi import IfBlock, ArgInfo
from mk_py_capi import CodeGenBase


class MyReprGen(ReprGen):

    def __init__(self, parent):
        super().__init__(parent)

    def gen_body(self, varname, strname):
        self._write_line(f'auto {strname} = {varname}.to_json();')

class JsonValueArg(ArgInfo):

    def __init__(self, parent, *,
                 name,
                 option,
                 cvarname):
        super().__init__(parent,
                         name=name,
                         option=option,
                         pchar='O',
                         ptype=None,
                         cvartype='JsonValue',
                         cvarname=cvarname)

    def gen_conv(self):
        self._write_line(f'JsonValue {self.cvarname};')
        with IfBlock(self.parent,
                     condition=f'!PyJsonValue::FromPyObject(obj, {self.cvarname})'):
            self._write_line('PyErr_SetString(PyExc_TypeError, "cannot convert to JsonValue");')
            self._write_line('return nullptr;')
            
        
class MyNewGen(NewGen):
    """PyJsonValue 用の new 関数生成用のクラス"""

    def __init__(self, parent):
        jsarg = JsonValueArg(parent,
                             name='value',
                             option=True,
                             cvarname='val')
        arg_list = [jsarg]
        super().__init__(parent, arg_list=arg_list)

    def gen_body(self):
        self._write_line('return PyJsonValue::ToPyObject(val)')
        

class NullGen(MethodGen):
    """JsonValue.null() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='null',
                         func_name='null',
                         arg_list=[],
                         is_static=True,
                         doc_str='make null object')

    def gen_body(self):
        self._write_line('return PyJsonValue::ToPyObject(JsonValue::null());')


class IsNullGen(MethodGen):
    """JsonValue.is_null() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='is_null',
                         func_name='is_null',
                         arg_list=[],
                         is_static=False,
                         doc_str='check if null')

    def gen_body(self):
        self.gen_val_conv('val')
        self._write_line('auto ans = val.is_null();')
        self._write_line('return PyBool_FromLong(ans);')
        

gen = MkPyCapi(classname='JsonValue',
               pyclassname='PyJsonValue',
               namespace='YM',
               pyname='JsonValue',
               header_include_files=['ym_config.h',
                                     ],
               source_include_files=['pym/PyJsonValue.h',
                                     'pym/PyString.h',
                                     'ym/JsonValue.h'])

gen.repr_gen = MyReprGen(gen)
gen.dealloc_gen = DeallocGen(gen)
gen.new_gen = MyNewGen(gen)
gen.add_method(NullGen(gen))
gen.add_method(IsNullGen(gen))

gen.make_header()

gen.make_source()
