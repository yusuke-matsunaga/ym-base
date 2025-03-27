#! /usr/bin/env python3

""" MkJsonValue の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import MkPyCapi, FuncDef
from mk_py_capi import StringArg, BoolArg, ObjArg


        
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
        
        
gen = MkPyCapi(classname='JsonValue',
               pyclassname='PyJsonValue',
               namespace='YM',
               pyname='JsonValue',
               header_include_files=['ym_config.h',
                                     ],
               source_include_files=['pym/PyJsonValue.h',
                                     'pym/PyString.h',
                                     'ym/JsonValue.h'])
        
def gen_preamble(gen):
    gen.gen_CRLF()
    gen.gen_comment('エラーメッセージを表す定数')
    gen._write_line('static const char* EMSG_NOT_OBJ = "not an Object type";')
    gen._write_line('static const char* EMSG_NOT_ARRAY = "not an Array type";')
    gen._write_line('static const char* EMSG_NOT_STR = "not a string type";')
    gen._write_line('static const char* EMSG_NOT_INT = "not an integer type";')
    gen._write_line('static const char* EMSG_NOT_FLOAT = "not a float type";')
    gen._write_line('static const char* EMSG_NOT_BOOL = "not a bool type";')
    gen._write_line('static const char* EMSG_NOT_OBJ_ARRAY = "neither an object nor an array type";')
    gen._write_line('static const char* EMSG_OUT_OF_RANGE = "index is out-of-range";')

gen.add_preamble(gen_preamble)

gen.add_dealloc()

def repr_func(varname):
    return f'{varname}.to_json()'

gen.add_repr(repr_func=repr_func)

def gen_sq_length(gen):
    gen.gen_val_conv('val')
    with gen.gen_if_block('!val.is_object() && !val.is_array()'):
        gen.gen_type_error('EMSG_NOT_OBJ_ARRAY', noexit=True)
        gen.gen_return('-1')
    gen.gen_return('val.size()')

gen.add_sequence(sq_length=FuncDef(func=gen_sq_length))

def gen_null(gen):
    gen.gen_return('PyJsonValue::ToPyObject(JsonValue::null())')

gen.add_method('null',
               doc_str='make null object',
               gen_body=gen_null)


def gen_check_func(gen, check_func):
    gen.gen_val_conv('val')
    gen.gen_auto_assign('ans', f'val.{check_func}()')
    gen.gen_return_py_bool('ans')
    
def gen_is_null(gen):
    gen_check_func(gen, 'is_null')

gen.add_method('is_null',
               gen_body=gen_is_null,
               doc_str='check if null')


def gen_is_string(gen):
    gen_check_func(gen, 'is_string')

gen.add_method('is_string',
               doc_str='check if string-type',
               gen_body=gen_is_string)


def gen_is_number(gen):
    gen_check_func(gen, 'is_number')

gen.add_method('is_number',
               doc_str='check if number-type',
               gen_body=gen_is_number)


def gen_is_int(gen):
    gen_check_func(gen, 'is_int')

gen.add_method('is_int',
               doc_str='check if int-type',
               gen_body=gen_is_int)


def gen_is_float(gen):
    gen_check_func(gen, 'is_float')

gen.add_method('is_float',
               doc_str='check if float-type',
               gen_body=gen_is_float)


def gen_is_bool(gen):
    gen_check_func(gen, 'is_bool')

gen.add_method('is_bool',
               doc_str='check if bool-type',
               gen_body=gen_is_bool)


def gen_is_object(gen):
    gen_check_func(gen, 'is_object')

gen.add_method('is_object',
               doc_str='check if bool-type',
               gen_body=gen_is_object)


def gen_is_array(gen):
    gen_check_func(gen, 'is_array')

gen.add_method('is_array',
               doc_str='check if bool-type',
               gen_body=gen_is_array)


def gen_has_key(gen):
    gen.gen_val_conv('val')
    with gen.gen_if_block('!val.is_object()'):
        gen.gen_type_error('EMSG_NOT_OBJ')
    gen.gen_auto_assign('ans', 'val.has_key(key)')
    gen.gen_return_py_bool('ans')

gen.add_method('has_key',
               arg_list=[StringArg(gen,
                                   name='key',
                                   cvarname='key')],
               gen_body=gen_has_key,
               doc_str='check if having the key')


def gen_get_string(gen):
    gen.gen_val_conv('val')
    with gen.gen_if_block('!val.is_string()'):
        gen.gen_type_error('EMSG_NOT_STR')
    gen.gen_auto_assign('ans', 'val.get_string()')
    gen.gen_return_py_string('ans')

gen.add_method('get_string',
               gen_body=gen_get_string,
               doc_str='get string value')


def gen_get_int(gen):
    gen.gen_val_conv('val')
    with gen.gen_if_block('!val.is_int()'):
        gen.gen_type_error('EMSG_NOT_INT')
    gen.gen_auto_assign('ans', 'val.get_int()')
    gen.gen_return_py_int('ans')

gen.add_method('get_int',
               gen_body=gen_get_int,
               doc_str='get int value')


def gen_get_float(gen):
    gen.gen_val_conv('val')
    with gen.gen_if_block('!val.is_float()'):
        gen.gen_type_error('EMSG_NOT_FLOAT')
    gen.gen_auto_assign('ans', 'val.get_float()')
    gen.gen_return_py_float('ans')

gen.add_method('get_float',
               gen_body=gen_get_float,
               doc_str='get float value')


def gen_get_bool(gen):
    gen.gen_val_conv('val')
    with gen.gen_if_block('!val.is_bool()'):
        gen.gen_type_error('EMSG_NOT_BOOL')
    gen.gen_auto_assign('ans', 'val.get_bool()')
    gen.gen_return_py_bool('ans')

gen.add_method('get_bool',
               gen_body=gen_get_bool,
               doc_str='get bool value')


def gen_read(gen):
    with gen.gen_try_block():
        gen.gen_auto_assign('val', 'JsonValue::read(filename)')
        gen.gen_return('PyJsonValue::ToPyObject(val)')
    with gen.gen_catch_block('std::invalid_argument err'):
        gen.gen_value_error('err.what()')

gen.add_method('read',
               arg_list=[StringArg(gen,
                                   name='filename',
                                   cvarname='filename')],
               is_static=True,
               gen_body=gen_read,
               doc_str='read JSON data from file')


def gen_parse(gen):
    with gen.gen_try_block():
        gen.gen_auto_assign('val', 'JsonValue::parse(json_str)')
        gen.gen_return('PyJsonValue::ToPyObject(val)')
    with gen.gen_catch_block('std::invalid_argument err'):
        gen.gen_value_error('err.what()')

gen.add_method('parse',
               arg_list=[StringArg(gen,
                                   name='json_str',
                                   cvarname='json_str')],
               is_static=True,
               gen_body=gen_parse,
               doc_str='read JSON data from string')



def gen_write(gen):
    gen.gen_declaration('std::ofstream', 's')
    with gen.gen_if_block('!s'):
        gen.gen_declaration('std:ostringstream', 'buff')
        gen._write_line('buff << filename << ": Could not open.";')
        gen.gen_value_error('buff.str().c_str()')
    gen.gen_val_conv('json_value')
    gen._write_line('json_value.write(s, indent);')
    gen.gen_return_py_none()

gen.add_method('write',
               arg_list=[StringArg(gen,
                                   name='filename',
                                   cvarname='filename'),
                         BoolArg(gen,
                                 name='indent',
                                 option=True,
                                 cvarname='indent',
                                 cvardefault='false')],
               gen_body=gen_write,
               doc_str='write JSON data to file')

def key_list_gen(gen):
    with gen.gen_if_block('!val.is_object()'):
        gen.gen_type_error('EMSG_NOT_OBJ')
    with gen.gen_try_block():
        gen.gen_auto_assign('val_list', 'val.key_list()')
        gen.gen_return('PyString::ToPyList(val_list)')
    with gen.gen_catch_block('std::invalid_argument err'):
        gen.gen_value_error('err.what()')

def item_list_gen(gen):
    with gen.gen_if_block('!val.is_object()'):
        gen.gen_type_error('EMSG_NOT_OBJ')
    with gen.gen_try_block():
        gen.gen_auto_assign('item_list', 'val.item_list()')
        gen.gen_auto_assign('n', 'item_list.size()')
        gen.gen_auto_assign('ans', 'PyList_New(n)')
        with gen.gen_for_block('SizeType i = 0',
                                'i < n',
                                '++ i'):
            gen.gen_assign('auto& p', 'item_list[i]')
            gen.gen_auto_assign('key', 'p.first')
            gen.gen_auto_assign('value', 'p.second')
            gen.gen_auto_assign('value_obj', 'PyJsonValue::ToPyObject(value)')
            gen.gen_auto_assign('item_obj', 'Py_BuildValue("(sO)", key.c_str(), value_obj)')
            gen._write_line('PyList_SETITEM(ans, i, item_obj)')
        gen.gen_return('ans')
    with gen.gen_catch_block('std::invalid_argument err'):
        gen.gen_value_error('err.what()')

gen.add_conv()

def deconv_gen(gen):
    with gen.gen_if_block('obj == nullptr'):
        gen.gen_comment('null')
        gen.gen_assign('val', 'JsonValue:null()')
        gen.gen_return('true')

    gen.gen_CRLF()
    gen.gen_comment('ブール型は他の型からの変換が常に成功するので先にチェックする．')
    with gen.gen_if_block('obj == Py_True'):
        gen.gen_comment('true')
        gen.gen_assign('val', 'JsonValue(true)')
        gen.gen_return('true')
    with gen.gen_if_block('obj == Py_False'):
        gen.gen_comment('false')
        gen.gen_assign('val', 'JsonValue(false)')
        gen.gen_return('true')

    gen.gen_CRLF()
    gen.gen_comment('文字列型からの変換')
    with gen.gen_if_block('PyString::Check(obj)'):
        gen.gen_auto_assign('str_val', 'PyString::AsString(obj)')
        gen.gen_assign('val', 'JsonValue(str_val)')
        gen.gen_return('true')

    gen.gen_CRLF()
    gen.gen_comment('整数型からの変換')
    with gen.gen_if_block('PyLong_Check(obj)'):
        gen.gen_auto_assign('int_val', 'PyLong_AsLong(obj)')
        gen.gen_assign('val', 'JsonValue(int_val)')
        gen.gen_return('true')

    gen.gen_CRLF()
    gen.gen_comment('浮動小数点型からの変換')
    with gen.gen_if_block('PyFloat_Check(obj)'):
        gen.gen_auto_assign('double_val', 'PyFloat_AsDouble(obj)')
        gen.gen_assign('val', 'JsonValue(double_val)')

    gen.gen_CRLF()
    gen.gen_comment('シーケンス型からの変換')
    with gen.gen_if_block('PySequence_Check(obj)'):
        gen.gen_auto_assign('n', 'PySequence_Size(obj)')
        gen.gen_declaration('std::vector<JsonValue>', 'elem_list(n)')
        with gen.gen_for_block('SizeType i = 0',
                                'i < n',
                                '++ i'):
            gen.gen_auto_assign('elem_obj', 'PySequence_GetItem(obj, i)')
            gen.gen_declaration('JsonValue', 'elem_val')
            with gen.gen_if_block('!operator()(elem_obj, elem_val)'):
                gen.gen_return('false')
            gen.gen_assign('elem_list[i]', 'elem_val')
        gen.gen_assign('val', 'JsonValue(elem_list)')
        gen.gen_return('true')
            
    gen.gen_CRLF()
    gen.gen_comment('辞書型からの変換')
    with gen.gen_if_block('PyDict_Check(obj)'):
        gen.gen_auto_assign('item_list', 'PyDict_Items(obj)')
        gen.gen_auto_assign('n', 'PyList_Size(item_list)')
        gen.gen_declaration('std::unordered_map<std::string, JsonValue>',
                             'item_dict')
        with gen.gen_for_block('SizeType i = 0',
                                'i < n',
                                '++ i'):
            gen.gen_auto_assign('pair', 'PyList_GetItem(item_list, i)')
            gen.gen_assign('char* key', 'nullptr')
            gen.gen_assign('PyObject* item_obj', 'nullptr')
            with gen.gen_if_block('!PyArg_ParseTuple(pair, "sO", &key, &item_obj)'):
                gen.gen_return('false')
            gen.gen_declaration('JsonValue', 'item_val')
            with gen.gen_if_block('!operator()(item_obj, item_val)'):
                gen.gen_return('false')
            gen._write_line('item_dict.emplace(key, item_val);')
        gen._write_line('Py_DECREF(item_list)')
        gen.gen_assign('val', 'JsonValue(item_list)')
        gen.gen_return('true')

    gen.gen_CRLF()
    gen.gen_comment('PyJsonValue 型からの変換')
    with gen.gen_if_block('PyJsonValue::Check(obj)'):
        gen.gen_assign('val', 'PyJsonValue::_get_ref(obj)')
        gen.gen_return('true')

    gen.gen_CRLF()
    gen.gen_return('false')
    
gen.add_deconv(gen_body=deconv_gen)


def new_gen(gen):
    gen.gen_return('PyJsonValue::ToPyObject(val)')

gen.add_new(arg_list=[JsonValueArg(gen,
                                   name='value',
                                   option=True,
                                   cvarname='val')],
            gen_body=new_gen)

gen.add_getter('get_key_list',
               gen_body=key_list_gen)

gen.add_attr('key_list',
             getter_name='get_key_list',
             doc_str='key list')

gen.add_getter('get_item_list', gen_body=item_list_gen)

gen.add_attr('item_list',
             getter_name='get_item_list',
             doc_str='item list')

gen.make_header()

gen.make_source()
