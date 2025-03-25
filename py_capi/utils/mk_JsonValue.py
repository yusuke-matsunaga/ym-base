#! /usr/bin/env python3

""" MkJsonValue の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import MkPyCapi
from mk_py_capi import ReprGen, DeallocGen, MethodGen, NewGen, ConvGen, DeconvGen
from mk_py_capi import ObjArg


class MyReprGen(ReprGen):

    def __init__(self, parent):
        super().__init__(parent)

    def gen_body(self, varname, strname):
        self.gen_auto_assign(f'{strname}', f'{varname}.to_json()')

        
class JsonValueArg(ObjArg):

    def __init__(self, parent, *,
                 name=None,
                 option=False,
                 cvarname):
        super().__init__(parent,
                         name=name,
                         option=option,
                         cvartype='JsonValue',
                         cvarname=cvarname)

    def gen_conv(self):
        self.gen_declaration('JsonValue', f'{self.cvarname}')
        with self.gen_if_block(f'!PyJsonValue::FromPyObject(obj, {self.cvarname})'):
            self.gen_type_error("could not convert to JsonValue")
            self.gen_return('nullptr')
            
        
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
        self.gen_return('PyJsonValue::ToPyObject(val)')
        

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
        self.gen_return('PyJsonValue::ToPyObject(JsonValue::null())')


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
        self.gen_auto_assign('ans', 'val.is_null()')
        self.gen_return('PyBool_FromLong(ans)')

        
class MyDeconvGen(DeconvGen):

    def __init__(self, parent):
        super().__init__(parent)

    def gen_body(self):
        with self.gen_if_block('obj == nullptr'):
            self.gen_comment('null')
            self.gen_assign('val', 'JsonValue:null()')
            self.gen_return('true')

        self.gen_CRLF()
        self.gen_comment('ブール型は他の型からの変換が常に成功するので先にチェックする．')
        with self.gen_if_block('obj == Py_True'):
            self.gen_comment('true')
            self.gen_assign('val', 'JsonValue(true)')
            self.gen_return('true')
        with self.gen_if_block('obj == Py_False'):
            self.gen_comment('false')
            self.gen_assign('val', 'JsonValue(false)')
            self.gen_return('true')

        self.gen_CRLF()
        self.gen_comment('文字列型からの変換')
        with self.gen_if_block('PyString::Check(obj)'):
            self.gen_auto_assign('str_val', 'PyString::AsString(obj)')
            self.gen_assign('val', 'JsonValue(str_val)')
            self.gen_return('true')

        self.gen_CRLF()
        self.gen_comment('整数型からの変換')
        with self.gen_if_block('PyLong_Check(obj)'):
            self.gen_auto_assign('int_val', 'PyLong_AsLong(obj)')
            self.gen_assign('val', 'JsonValue(int_val)')
            self.gen_return('true')

        self.gen_CRLF()
        self.gen_comment('浮動小数点型からの変換')
        with self.gen_if_block('PyFloat_Check(obj)'):
            self.gen_auto_assign('double_val', 'PyFloat_AsDouble(obj)')
            self.gen_assign('val', 'JsonValue(double_val)')

        self.gen_CRLF()
        self.gen_comment('シーケンス型からの変換')
        with self.gen_if_block('PySequence_Check(obj)'):
            self.gen_auto_assign('n', 'PySequence_Size(obj)')
            self.gen_declaration('std::vector<JsonValue>', 'elem_list(n)')
            with self.gen_for_block('SizeType i = 0',
                                    'i < n',
                                    '++ i'):
                self.gen_auto_assign('elem_obj', 'PySequence_GetItem(obj, i)')
                self.gen_declaration('JsonValue', 'elem_val')
                with self.gen_if_block('!operator()(elem_obj, elem_val)'):
                    self.gen_return('false')
                self.gen_assign('elem_list[i]', 'elem_val')
            self.gen_assign('val', 'JsonValue(elem_list)')
            self.gen_return('true')
            
        self.gen_CRLF()
        self.gen_comment('辞書型からの変換')
        with self.gen_if_block('PyDict_Check(obj)'):
            self.gen_auto_assign('item_list', 'PyDict_Items(obj)')
            self.gen_auto_assign('n', 'PyList_Size(item_list)')
            self.gen_declaration('std::unordered_map<std::string, JsonValue>',
                                 'item_dict')
            with self.gen_for_block('SizeType i = 0',
                                    'i < n',
                                    '++ i'):
                self.gen_auto_assign('pair', 'PyList_GetItem(item_list, i)')
                self.gen_assign('char* key', 'nullptr')
                self.gen_assign('PyObject* item_obj', 'nullptr')
                with self.gen_if_block('!PyArg_ParseTuple(pair, "sO", &key, &item_obj)'):
                    self.gen_return('false')
                self.gen_declaration('JsonValue', 'item_val')
                with self.gen_if_block('!operator()(item_obj, item_val)'):
                    self.gen_return('false')
                self._write_line('item_dict.emplace(key, item_val);')
            self._write_line('Py_DECREF(item_list)')
            self.gen_assign('val', 'JsonValue(item_list)')
            self.gen_return('true')

        self.gen_CRLF()
        self.gen_comment('PyJsonValue 型からの変換')
        with self.gen_if_block('PyJsonValue::Check(obj)'):
            self.gen_assign('val', 'PyJsonValue::_get_ref(obj)')
            self.gen_return('true')

        self.gen_CRLF()
        self.gen_return('false')
        

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
gen.conv_gen = ConvGen(gen)
gen.deconv_gen = MyDeconvGen(gen)
gen.add_method(NullGen(gen))
gen.add_method(IsNullGen(gen))

gen.make_header()

gen.make_source()
