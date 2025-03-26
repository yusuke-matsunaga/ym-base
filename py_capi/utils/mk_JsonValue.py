#! /usr/bin/env python3

""" MkJsonValue の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import MkPyCapi
from mk_py_capi import ReprGen, DeallocGen, NewGen, ConvGen, DeconvGen
from mk_py_capi import MethodGen, GetterGen, SetterGen
from mk_py_capi import StringArg, BoolArg, ObjArg
from mk_py_capi import PreambleGen


def repr_str(varname):
    return f'{varname}.to_json()'

        
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

    def gen_obj_conv(self):
        with self.gen_if_block(f'!PyJsonValue::FromPyObject({self.tmpname}, {self.cvarname})'):
            self.gen_type_error('"Could not convert to JsonValue"')
            
        
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
                         is_static=True,
                         doc_str='make null object')

    def gen_body(self):
        self.gen_return('PyJsonValue::ToPyObject(JsonValue::null())')


class IsNullGen(MethodGen):
    """JsonValue.is_null() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='is_null',
                         doc_str='check if null')

    def gen_body(self):
        self.gen_val_conv('val')
        self.gen_auto_assign('ans', 'val.is_null()')
        self.gen_return_py_bool('ans')


class IsStringGen(MethodGen):
    """JsonValue.is_string() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='is_string',
                         doc_str='check if string-type')

    def gen_body(self):
        self.gen_val_conv('val')
        self.gen_auto_assign('ans', 'val.is_string()')
        self.gen_return_py_bool('ans')


class IsNumberGen(MethodGen):
    """JsonValue.is_number() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='is_number',
                         doc_str='check if number-type')

    def gen_body(self):
        self.gen_val_conv('val')
        self.gen_auto_assign('ans', 'val.is_number()')
        self.gen_return_py_bool('ans')


class IsIntGen(MethodGen):
    """JsonValue.is_int() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='is_int',
                         doc_str='check if int-type')

    def gen_body(self):
        self.gen_val_conv('val')
        self.gen_auto_assign('ans', 'val.is_int()')
        self.gen_return_py_bool('ans')


class IsFloatGen(MethodGen):
    """JsonValue.is_float() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='is_float',
                         doc_str='check if float-type')

    def gen_body(self):
        self.gen_val_conv('val')
        self.gen_auto_assign('ans', 'val.is_float()')
        self.gen_return_py_bool('ans')


class IsBoolGen(MethodGen):
    """JsonValue.is_bool() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='is_bool',
                         doc_str='check if bool-type')

    def gen_body(self):
        self.gen_val_conv('val')
        self.gen_auto_assign('ans', 'val.is_bool()')
        self.gen_return_py_bool('ans')


class IsObjectGen(MethodGen):
    """JsonValue.is_object() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='is_object',
                         doc_str='check if object-type')

    def gen_body(self):
        self.gen_val_conv('val')
        self.gen_auto_assign('ans', 'val.is_object()')
        self.gen_return_py_bool('ans')


class IsArrayGen(MethodGen):
    """JsonValue.is_array() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='is_array',
                         doc_str='check if array-type')

    def gen_body(self):
        self.gen_val_conv('val')
        self.gen_auto_assign('ans', 'val.is_array()')
        self.gen_return_py_bool('ans')


class HasKeyGen(MethodGen):
    """JsonValue.has_key() 関数を生成するクラス"""

    def __init__(self, parent):
        arg_list = [StringArg(parent,
                              name='key',
                              cvarname='key')]
        super().__init__(parent,
                         name='has_key',
                         arg_list=arg_list,
                         doc_str='check if having the key')

    def gen_body(self):
        self.gen_val_conv('val')
        with self.gen_if_block('!val.is_object()'):
            self.gen_type_error('EMSG_NOT_OBJ')
        self.gen_auto_assign('ans', 'val.has_key()')
        self.gen_return_py_bool('ans')


class GetStringGen(MethodGen):
    """JsonValue.get_string() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='get_string',
                         doc_str='get string value')

    def gen_body(self):
        self.gen_val_conv('val')
        with self.gen_if_block('!val.is_string()'):
            self.gen_type_error('EMSG_NOT_STR')
        self.gen_auto_assign('ans', 'val.get_string()')
        self.gen_return_py_string('ans')


class GetIntGen(MethodGen):
    """JsonValue.get_int() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='get_int',
                         doc_str='get int value')

    def gen_body(self):
        self.gen_val_conv('val')
        with self.gen_if_block('!val.is_int()'):
            self.gen_type_error('EMSG_NOT_INT')
        self.gen_auto_assign('ans', 'val.get_int()')
        self.gen_return_py_int('ans')


class GetFloatGen(MethodGen):
    """JsonValue.get_float() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='get_float',
                         doc_str='get float value')

    def gen_body(self):
        self.gen_val_conv('val')
        with self.gen_if_block('!val.is_float()'):
            self.gen_type_error('EMSG_NOT_FLOAT')
        self.gen_auto_assign('ans', 'val.get_float()')
        self.gen_return_py_float('ans')


class GetBoolGen(MethodGen):
    """JsonValue.get_bool() 関数を生成するクラス"""

    def __init__(self, parent):
        super().__init__(parent,
                         name='get_bool',
                         doc_str='get bool value')

    def gen_body(self):
        self.gen_val_conv('val')
        with self.gen_if_block('!val.is_bool()'):
            self.gen_type_error('EMSG_NOT_BOOL')
        self.gen_auto_assign('ans', 'val.get_bool()')
        self.gen_return_py_bool('ans')


class ReadGen(MethodGen):
    """JsonValue.read() 関数を生成するクラス"""

    def __init__(self, parent):
        arg_list = [StringArg(parent,
                              name='filename',
                              cvarname='filename')]
        super().__init__(parent,
                         name='read',
                         arg_list=arg_list,
                         is_static=True,
                         doc_str='read JSON data from file')

    def gen_body(self):
        with self.gen_try_block():
            self.gen_auto_assign('val', 'JsonValue::read(filename)')
            self.gen_return('PyJsonValue::ToPyObject(val)')
        with self.gen_catch_block('std::invalid_argument err'):
            self.gen_value_error('err.what()')


class ParseGen(MethodGen):
    """JsonValue.parse() 関数を生成するクラス"""

    def __init__(self, parent):
        arg_list = [StringArg(parent,
                              name='json_str',
                              cvarname='json_str')]
        super().__init__(parent,
                         name='parse',
                         arg_list=arg_list,
                         is_static=True,
                         doc_str='read JSON data from string')

    def gen_body(self):
        with self.gen_try_block():
            self.gen_auto_assign('val', 'JsonValue::parse(json_str)')
            self.gen_return('PyJsonValue::ToPyObject(val)')
        with self.gen_catch_block('std::invalid_argument err'):
            self.gen_value_error('err.what()')


class WriteGen(MethodGen):
    """JsonValue.write() 関数を生成するクラス"""

    def __init__(self, parent):
        arg_list = [StringArg(parent,
                              name='filename',
                              cvarname='filename'),
                    BoolArg(parent,
                            name='indent',
                            option=True,
                            cvarname='indent',
                            cvardefault='false')]
        super().__init__(parent,
                         name='write',
                         arg_list=arg_list,
                         is_static=True,
                         doc_str='write JSON data to file')

    def gen_body(self):
        self.gen_declaration('std::ofstream', 's')
        with self.gen_if_block('!s'):
            self.gen_declaration('std:ostringstream', 'buff')
            self._write_line('buff << filename << ": Could not open.";')
            self.gen_value_error('buff.str().c_str()')
        self.gen_val_conv('json_value')
        self._write_line('json_value.write(s, indent);')
        self.gen_return_py_none()
        
        
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

class MyPreambleGen(PreambleGen):

    def __init__(self, parent):
        super().__init__(parent)

    def __call__(self):
        self.gen_CRLF()
        self.gen_comment('エラーメッセージを表す定数')
        self._write_line('static const char* EMSG_NOT_OBJ = "not an Object type";')
        self._write_line('static const char* EMSG_NOT_ARRAY = "not an Array type";')
        self._write_line('static const char* EMSG_NOT_STR = "not a string type";')
        self._write_line('static const char* EMSG_NOT_INT = "not an integer type";')
        self._write_line('static const char* EMSG_NOT_FLOAT = "not a float type";')
        self._write_line('static const char* EMSG_NOT_BOOL = "not a bool type";')
        self._write_line('static const char* EMSG_NOT_OBJ_ARRAY = "neither an object nor an array type";')
        self._write_line('static const char* EMSG_OUT_OF_RANGE = "index is out-of-range";')


class KeyListGen(GetterGen):

    def __init__(self, parent,
                 func_name='get_key_list'):
        super().__init__(parent, func_name=func_name)

    def gen_body(self):
        with self.gen_if_block('!val.is_object()'):
            self.gen_type_error('EMSG_NOT_OBJ')
        with self.gen_try_block():
            self.gen_auto_assign('val_list', 'val.key_list()')
            self.gen_return('PyString::ToPyList(val_list)')
        with self.gen_catch_block('std::invalid_argument err'):
            self.gen_value_error('err.what()')

            
class ItemListGen(GetterGen):

    def __init__(self, parent,
                 func_name='get_item_list'):
        super().__init__(parent, func_name=func_name)

    def gen_body(self):
        with self.gen_if_block('!val.is_object()'):
            self.gen_type_error('EMSG_NOT_OBJ')
        with self.gen_try_block():
            self.gen_auto_assign('item_list', 'val.item_list()')
            self.gen_auto_assign('n', 'item_list.size()')
            self.gen_auto_assign('ans', 'PyList_New(n)')
            with self.gen_for_block('SizeType i = 0',
                                    'i < n',
                                    '++ i'):
                self.gen_assign('auto& p', 'item_list[i]')
                self.gen_auto_assign('key', 'p.first')
                self.gen_auto_assign('value', 'p.second')
                self.gen_auto_assign('value_obj', 'PyJsonValue::ToPyObject(value)')
                self.gen_auto_assign('item_obj', 'Py_BuildValue("(sO)", key.c_str(), value_obj)')
                self._write_line('PyList_SETITEM(ans, i, item_obj)')
            self.gen_return('ans')
        with self.gen_catch_block('std::invalid_argument err'):
            self.gen_value_error('err.what()')
            
        
gen = MkPyCapi(classname='JsonValue',
               pyclassname='PyJsonValue',
               namespace='YM',
               pyname='JsonValue',
               header_include_files=['ym_config.h',
                                     ],
               source_include_files=['pym/PyJsonValue.h',
                                     'pym/PyString.h',
                                     'ym/JsonValue.h'])
MyPreambleGen(gen)
ReprGen(gen, repr_str)
DeallocGen(gen)
MyNewGen(gen)
ConvGen(gen)
MyDeconvGen(gen)
NullGen(gen)
IsNullGen(gen)
IsStringGen(gen)
IsNumberGen(gen)
IsIntGen(gen)
IsFloatGen(gen)
IsBoolGen(gen)
IsObjectGen(gen)
IsArrayGen(gen)
HasKeyGen(gen)
GetStringGen(gen)
GetIntGen(gen)
GetFloatGen(gen)
GetBoolGen(gen)
ReadGen(gen)
ParseGen(gen)
WriteGen(gen)
get_key_list = KeyListGen(gen)
gen.add_attr('key_list',
             getter_name=get_key_list.func_name,
             doc_str='key list')
get_item_list = ItemListGen(gen)
gen.add_attr('item_list',
             getter_name=get_item_list.func_name,
             doc_str='item list')

gen.make_header()

gen.make_source()
