#! /usr/bin/env python3

""" MkJsonValue の定義ファイル

:file: mk_mt19937.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import ObjArg, StringArg, BoolArg

        
class JsonValueArg(ObjArg):

    def __init__(self, *,
                 name=None,
                 option=False,
                 cvarname):
        super().__init__(name=name,
                         option=option,
                         cvartype='JsonValue',
                         cvarname=cvarname,
                         cvardefault=None,
                         pyclassname='PyJsonValue')


class JsonValueGen(PyObjGen):
    
    def __init__(self):
        super().__init__(classname='JsonValue',
                         namespace='YM',
                         pyname='JsonValue',
                         header_include_files=['ymconfig.h'],
                         source_include_files=['pym/PyJsonValue.h',
                                               'pym/PyString.h',
                                               'ym/JsonValue.h'])

        self.add_dealloc() # デフォルト実装

        
gen = JsonValueGen()
        
        
def gen_preamble(writer):
    writer.gen_CRLF()
    writer.gen_comment('エラーメッセージを表す定数')
    writer.write_line('static const char* EMSG_NOT_OBJ = "not an Object type";')
    writer.write_line('static const char* EMSG_NOT_ARRAY = "not an Array type";')
    writer.write_line('static const char* EMSG_NOT_STR = "not a string type";')
    writer.write_line('static const char* EMSG_NOT_INT = "not an integer type";')
    writer.write_line('static const char* EMSG_NOT_FLOAT = "not a float type";')
    writer.write_line('static const char* EMSG_NOT_BOOL = "not a bool type";')
    writer.write_line('static const char* EMSG_NOT_OBJ_ARRAY = "neither an object nor an array type";')
    writer.write_line('static const char* EMSG_OUT_OF_RANGE = "index is out-of-range";')

gen.add_preamble(gen_preamble)

def repr_func(writer):
    writer.gen_auto_assign('str_val', 'val.to_json()')

gen.add_repr(repr_func=repr_func)

def gen_null(writer):
    writer.gen_return('PyJsonValue::ToPyObject(JsonValue::null())')

gen.add_method('null',
               is_static=True,
               doc_str='make null object',
               func_body=gen_null)

def gen_check_func(writer, check_func):
    writer.gen_auto_assign('ans', f'val.{check_func}()')
    writer.gen_return_py_bool('ans')
    
def gen_is_null(writer):
    gen_check_func(writer, 'is_null')

gen.add_method('is_null',
               func_body=gen_is_null,
               doc_str='check if null')

def gen_is_string(writer):
    gen_check_func(writer, 'is_string')

gen.add_method('is_string',
               doc_str='check if string-type',
               func_body=gen_is_string)

def gen_is_number(writer):
    gen_check_func(writer, 'is_number')

gen.add_method('is_number',
               doc_str='check if number-type',
               func_body=gen_is_number)

def gen_is_int(writer):
    gen_check_func(writer, 'is_int')

gen.add_method('is_int',
               doc_str='check if int-type',
               func_body=gen_is_int)

def gen_is_float(writer):
    gen_check_func(writer, 'is_float')

gen.add_method('is_float',
               doc_str='check if float-type',
               func_body=gen_is_float)

def gen_is_bool(writer):
    gen_check_func(writer, 'is_bool')

gen.add_method('is_bool',
               doc_str='check if bool-type',
               func_body=gen_is_bool)

def gen_is_object(writer):
    gen_check_func(writer, 'is_object')

gen.add_method('is_object',
               doc_str='check if bool-type',
               func_body=gen_is_object)

def gen_is_array(writer):
    gen_check_func(writer, 'is_array')

gen.add_method('is_array',
               doc_str='check if bool-type',
               func_body=gen_is_array)

def gen_has_key(writer):
    with writer.gen_if_block('!val.is_object()'):
        writer.gen_type_error('EMSG_NOT_OBJ')
        writer.gen_auto_assign('ans', 'val.has_key(key)')
        writer.gen_return_py_bool('ans')
        
gen.add_method('has_key',
               arg_list=[StringArg(name='key',
                                   cvarname='key')],
               func_body=gen_has_key,
               doc_str='check if having the key')
        
def gen_get_string(writer):
    with writer.gen_if_block('!val.is_string()'):
        writer.gen_type_error('EMSG_NOT_STR')
        writer.gen_auto_assign('ans', 'val.get_string()')
        writer.gen_return_py_string('ans')

gen.add_method('get_string',
               func_body=gen_get_string,
               doc_str='get string value')

def gen_get_int(writer):
    with writer.gen_if_block('!val.is_int()'):
        writer.gen_type_error('EMSG_NOT_INT')
    writer.gen_auto_assign('ans', 'val.get_int()')
    writer.gen_return_py_int('ans')

gen.add_method('get_int',
               func_body=gen_get_int,
               doc_str='get int value')

def gen_get_float(writer):
    with writer.gen_if_block('!val.is_float()'):
        writer.gen_type_error('EMSG_NOT_FLOAT')
    writer.gen_auto_assign('ans', 'val.get_float()')
    writer.gen_return_py_float('ans')

gen.add_method('get_float',
               func_body=gen_get_float,
               doc_str='get float value')

def gen_get_bool(writer):
    with writer.gen_if_block('!val.is_bool()'):
        writer.gen_type_error('EMSG_NOT_BOOL')
    writer.gen_auto_assign('ans', 'val.get_bool()')
    writer.gen_return_py_bool('ans')

gen.add_method('get_bool',
               func_body=gen_get_bool,
               doc_str='get bool value')

def gen_write(writer):
    writer.gen_vardecl(typename='std::ofstream',
                       varname='s')
    with writer.gen_if_block('!s'):
        writer.gen_vardecl(typename='std:ostringstream',
                           varname='buff')
        writer.write_line('buff << filename << ": Could not open.";')
        writer.gen_value_error('buff.str().c_str()')
        writer.write_line('json_value.write(s, indent);')
    writer.gen_return_py_none()

gen.add_method('write',
               arg_list=[StringArg(name='filename',
                                   cvarname='filename'),
                         BoolArg(name='indent',
                                 option=True,
                                 cvarname='indent',
                                 cvardefault='false')],
               func_body=gen_write,
               doc_str='write JSON data to file')

def gen_parse(writer):
    with writer.gen_try_block():
        writer.gen_auto_assign('val', 'JsonValue::parse(json_str)')
        writer.gen_return('PyJsonValue::ToPyObject(val)')
    with writer.gen_catch_block('std::invalid_argument err'):
        writer.gen_value_error('err.what()')

gen.add_method('parse',
               arg_list=[StringArg(name='json_str',
                                   cvarname='json_str')],
               is_static=True,
               func_body=gen_parse,
               doc_str='read JSON data from string')

def gen_read(writer):
    with writer.gen_try_block():
        writer.gen_auto_assign('val', 'JsonValue::read(filename)')
        writer.gen_return('PyJsonValue::ToPyObject(val)')
    with writer.gen_catch_block('std::invalid_argument err'):
        writer.gen_value_error('err.what()')

gen.add_method('read',
               arg_list=[StringArg(name='filename',
                                   cvarname='filename')],
               is_static=True,
               func_body=gen_read,
               doc_str='read JSON data from file')


def gen_sq_length(writer):
    with writer.gen_if_block('!val.is_object() && !val.is_array()'):
        writer.gen_type_error('EMSG_NOT_OBJ_ARRAY', noexit=True)
        writer.gen_return('-1')
    writer.gen_auto_assign('len_val', 'val.size()')

gen.add_sequence(sq_length=gen_sq_length)


def key_list_gen(writer):
    with writer.gen_if_block('!val.is_object()'):
        writer.gen_type_error('EMSG_NOT_OBJ')
    with writer.gen_try_block():
        writer.gen_auto_assign('val_list', 'val.key_list()')
        writer.gen_return('PyString::ToPyList(val_list)')
    with writer.gen_catch_block('std::invalid_argument err'):
        writer.gen_value_error('err.what()')

def item_list_gen(writer):
    with writer.gen_if_block('!val.is_object()'):
        writer.gen_type_error('EMSG_NOT_OBJ')
    with writer.gen_try_block():
        writer.gen_auto_assign('item_list', 'val.item_list()')
        writer.gen_auto_assign('n', 'item_list.size()')
        writer.gen_auto_assign('ans', 'PyList_New(n)')
        with writer.gen_for_block('SizeType i = 0',
                                'i < n',
                                '++ i'):
            writer.gen_assign('auto& p', 'item_list[i]')
            writer.gen_auto_assign('key', 'p.first')
            writer.gen_auto_assign('value', 'p.second')
            writer.gen_auto_assign('value_obj', 'PyJsonValue::ToPyObject(value)')
            writer.gen_auto_assign('item_obj', 'Py_BuildValue("(sO)", key.c_str(), value_obj)')
            writer.write_line('PyList_SETITEM(ans, i, item_obj)')
        writer.gen_return('ans')
    with writer.gen_catch_block('std::invalid_argument err'):
        writer.gen_value_error('err.what()')

gen.add_conv()

def deconv_gen(writer):
    with writer.gen_if_block('obj == nullptr'):
        writer.gen_comment('null')
        writer.gen_assign('val', 'JsonValue:null()')
        writer.gen_return('true')

    writer.gen_CRLF()
    writer.gen_comment('ブール型は他の型からの変換が常に成功するので先にチェックする．')
    with writer.gen_if_block('obj == Py_True'):
        writer.gen_comment('true')
        writer.gen_assign('val', 'JsonValue(true)')
        writer.gen_return('true')
    with writer.gen_if_block('obj == Py_False'):
        writer.gen_comment('false')
        writer.gen_assign('val', 'JsonValue(false)')
        writer.gen_return('true')

    writer.gen_CRLF()
    writer.gen_comment('文字列型からの変換')
    with writer.gen_if_block('PyString::Check(obj)'):
        writer.gen_auto_assign('str_val', 'PyString::AsString(obj)')
        writer.gen_assign('val', 'JsonValue(str_val)')
        writer.gen_return('true')

    writer.gen_CRLF()
    writer.gen_comment('整数型からの変換')
    with writer.gen_if_block('PyLong_Check(obj)'):
        writer.gen_auto_assign('int_val', 'PyLong_AsLong(obj)')
        writer.gen_assign('val', 'JsonValue(int_val)')
        writer.gen_return('true')

    writer.gen_CRLF()
    writer.gen_comment('浮動小数点型からの変換')
    with writer.gen_if_block('PyFloat_Check(obj)'):
        writer.gen_auto_assign('double_val', 'PyFloat_AsDouble(obj)')
        writer.gen_assign('val', 'JsonValue(double_val)')

    writer.gen_CRLF()
    writer.gen_comment('シーケンス型からの変換')
    with writer.gen_if_block('PySequence_Check(obj)'):
        writer.gen_auto_assign('n', 'PySequence_Size(obj)')
        writer.gen_vardecl(typename='std::vector<JsonValue>',
                           varname='elem_list(n)')
        with writer.gen_for_block('SizeType i = 0',
                                'i < n',
                                '++ i'):
            writer.gen_auto_assign('elem_obj', 'PySequence_GetItem(obj, i)')
            writer.gen_vardecl(typename='JsonValue',
                               varname='elem_val')
            with writer.gen_if_block('!operator()(elem_obj, elem_val)'):
                writer.gen_return('false')
            writer.gen_assign('elem_list[i]', 'elem_val')
        writer.gen_assign('val', 'JsonValue(elem_list)')
        writer.gen_return('true')
            
    writer.gen_CRLF()
    writer.gen_comment('辞書型からの変換')
    with writer.gen_if_block('PyDict_Check(obj)'):
        writer.gen_auto_assign('item_list', 'PyDict_Items(obj)')
        writer.gen_auto_assign('n', 'PyList_Size(item_list)')
        writer.gen_vardecl(typename='std::unordered_map<std::string, JsonValue>',
                           varname='item_dict')
        with writer.gen_for_block('SizeType i = 0',
                                'i < n',
                                '++ i'):
            writer.gen_auto_assign('pair', 'PyList_GetItem(item_list, i)')
            writer.gen_assign('char* key', 'nullptr')
            writer.gen_assign('PyObject* item_obj', 'nullptr')
            with writer.gen_if_block('!PyArg_ParseTuple(pair, "sO", &key, &item_obj)'):
                writer.gen_return('false')
            writer.gen_vardecl(typename='JsonValue',
                               varname='item_val')
            with writer.gen_if_block('!operator()(item_obj, item_val)'):
                writer.gen_return('false')
            writer.write_line('item_dict.emplace(key, item_val);')
        writer.write_line('Py_DECREF(item_list)')
        writer.gen_assign('val', 'JsonValue(item_list)')
        writer.gen_return('true')

    writer.gen_CRLF()
    writer.gen_comment('PyJsonValue 型からの変換')
    with writer.gen_if_block('PyJsonValue::Check(obj)'):
        writer.gen_assign('val', 'PyJsonValue::_get_ref(obj)')
        writer.gen_return('true')

    writer.gen_CRLF()
    writer.gen_return('false')
    
gen.add_deconv(func_body=deconv_gen)


def new_gen(writer):
    writer.gen_return('PyJsonValue::ToPyObject(val)')

gen.add_new(arg_list=[JsonValueArg(name='value',
                                   option=True,
                                   cvarname='val')],
            func_body=new_gen)

gen.add_getter('get_key_list',
               func_body=key_list_gen)

gen.add_attr('key_list',
             getter_name='get_key_list',
             doc_str='key list')

gen.add_getter('get_item_list', func_body=item_list_gen)

gen.add_attr('item_list',
             getter_name='get_item_list',
             doc_str='item list')

gen.make_header()

gen.make_source()
