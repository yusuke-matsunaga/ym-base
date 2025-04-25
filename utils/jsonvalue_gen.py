#! /usr/bin/env python3

"""JsonValueGen の定義ファイル

:file: jsonvalue_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import PyObjGen
from mk_py_capi import OptArg, KwdArg, ObjConvArg, StringArg, BoolArg


class JsonValueArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvartype='JsonValue',
                         cvarname=cvarname,
                         cvardefault=None,
                         pyclassname='PyJsonValue')

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

def repr_func(writer):
    writer.gen_return_py_string('val.to_json()')

def gen_null(writer):
    writer.gen_return_pyobject('PyJsonValue', 'JsonValue::null()')

def gen_check_func(writer, check_func):
    writer.gen_auto_assign('ans', f'val.{check_func}()')
    writer.gen_return_py_bool('ans')

def gen_is_null(writer):
    gen_check_func(writer, 'is_null')

def gen_is_string(writer):
    gen_check_func(writer, 'is_string')

def gen_is_number(writer):
    gen_check_func(writer, 'is_number')

def gen_is_int(writer):
    gen_check_func(writer, 'is_int')

def gen_is_float(writer):
    gen_check_func(writer, 'is_float')

def gen_is_bool(writer):
    gen_check_func(writer, 'is_bool')

def gen_is_object(writer):
    gen_check_func(writer, 'is_object')

def gen_is_array(writer):
    gen_check_func(writer, 'is_array')

def gen_has_key(writer):
    with writer.gen_if_block('!val.is_object()'):
        writer.gen_type_error('EMSG_NOT_OBJ')
    writer.gen_auto_assign('ans', 'val.has_key(key)')
    writer.gen_return_py_bool('ans')

def gen_get_string(writer):
    with writer.gen_if_block('!val.is_string()'):
        writer.gen_type_error('EMSG_NOT_STR')
    writer.gen_auto_assign('ans', 'val.get_string()')
    writer.gen_return_py_string('ans')

def gen_get_int(writer):
    with writer.gen_if_block('!val.is_int()'):
        writer.gen_type_error('EMSG_NOT_INT')
    writer.gen_auto_assign('ans', 'val.get_int()')
    writer.gen_return_py_int('ans')

def gen_get_float(writer):
    with writer.gen_if_block('!val.is_float()'):
        writer.gen_type_error('EMSG_NOT_FLOAT')
    writer.gen_auto_assign('ans', 'val.get_float()')
    writer.gen_return_py_float('ans')

def gen_get_bool(writer):
    with writer.gen_if_block('!val.is_bool()'):
        writer.gen_type_error('EMSG_NOT_BOOL')
    writer.gen_auto_assign('ans', 'val.get_bool()')
    writer.gen_return_py_bool('ans')

def gen_write(writer):
    writer.gen_vardecl(typename='std::ofstream',
                       varname='s')
    with writer.gen_if_block('!s'):
        writer.gen_vardecl(typename='std:ostringstream',
                           varname='buff')
        writer.write_line('buff << filename << ": Could not open.";')
        writer.gen_value_error('buff.str().c_str()')
        writer.write_line('val.write(s, indent);')
    writer.gen_return_py_none()

def gen_parse(writer):
    with writer.gen_try_block():
        writer.gen_auto_assign('val', 'JsonValue::parse(json_str)')
        writer.gen_return_pyobject('PyJsonValue', 'val')
    writer.gen_catch_invalid_argument()

def gen_read(writer):
    with writer.gen_try_block():
        writer.gen_auto_assign('val', 'JsonValue::read(filename)')
        writer.gen_return_pyobject('PyJsonValue', 'val')
    writer.gen_catch_invalid_argument()

def gen_sq_length(writer):
    with writer.gen_if_block('!val.is_object() && !val.is_array()'):
        writer.gen_type_error('EMSG_NOT_OBJ_ARRAY', noexit=True)
        writer.gen_return('-1')
    writer.gen_auto_assign('len_val', 'val.size()')

def gen_sq_item(writer):
    with writer.gen_if_block('!val.is_array()'):
        writer.gen_type_error('EMSG_NOT_ARRAY')
    with writer.gen_try_block():
        writer.gen_auto_assign('index1', '( index >= 0 ) ? index : val.size() + index')
        writer.gen_return_pyobject('PyJsonValue', 'val.at(index1)')
    writer.gen_catch_invalid_argument()

def gen_mp_subscript(writer):
    with writer.gen_if_block('PyString::Check(key)'):
        with writer.gen_if_block('!val.is_object()'):
            writer.gen_type_error('EMSG_NOT_OBJ')
        writer.gen_auto_assign('key_str', 'PyString::Get(key)')
        with writer.gen_try_block():
            writer.gen_return_pyobject('PyJsonValue', 'val.at(key_str)')
        writer.gen_catch_invalid_argument()
    with writer.gen_if_block('PyLong_Check(key)'):
        with writer.gen_if_block('!val.is_array()'):
            writer.gen_type_error('EMSG_NOT_ARRAY')
        writer.gen_auto_assign('index', 'PyLong_AsLong(key)')
        writer.gen_auto_assign('index1', '( index >= 0 ) ? index : val.size() + index')
        with writer.gen_try_block():
            writer.gen_return_pyobject('PyJsonValue', 'val.at(index1)')
        writer.gen_catch_out_of_range('EMSG_OUT_OF_RANGE')
    writer.gen_type_error('EMSG_NOT_OBJ_ARRAY')

def key_list_gen(writer):
    with writer.gen_if_block('!val.is_object()'):
        writer.gen_type_error('EMSG_NOT_OBJ')
    with writer.gen_try_block():
        writer.gen_auto_assign('val_list', 'val.key_list()')
        writer.gen_return('PyString::ToPyList(val_list)')
    writer.gen_catch_invalid_argument()

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
            writer.gen_auto_assign('item_obj', 'Py_BuildValue("sO", key.c_str(), value_obj)')
            writer.write_line('PyList_SET_ITEM(ans, i, item_obj);')
        writer.gen_return('ans')
    writer.gen_catch_invalid_argument()

def new_gen(writer):
    writer.gen_return_pyobject('PyJsonValue', 'val')


class JsonValueGen(PyObjGen):

    def __init__(self):
        super().__init__(classname='JsonValue',
                         namespace='YM',
                         pyname='JsonValue',
                         header_include_files=['ym_config.h',
                                               'ym/JsonValue.h'],
                         source_include_files=['pym/PyJsonValue.h',
                                               'pym/PyString.h',
                                               'pym/PyInt.h',
                                               'pym/PyFloat.h',
                                               'pym/PyDict.h',
                                               'pym/PyList.h',
                                               'ym/JsonValue.h'])

        self.add_preamble(gen_preamble)

        self.add_dealloc() # デフォルト実装

        self.add_repr(func_body=repr_func)


        def richcmp_func(writer):
            with writer.gen_if_block('PyJsonValue::Check(self) && PyJsonValue::Check(other)'):
                self.gen_ref_conv(writer, objname='self', refname='val1')
                self.gen_ref_conv(writer, objname='other', refname='val2')
                with writer.gen_if_block('op == Py_EQ'):
                    writer.gen_return_py_bool('val1 == val2')
                with writer.gen_if_block('op == Py_NE'):
                    writer.gen_return_py_bool('val1 != val2')
            writer.gen_return_py_notimplemented()
        self.add_richcompare(func_body=richcmp_func)

        self.add_static_method('null',
                               doc_str='make null object',
                               func_body=gen_null)
        self.add_method('is_null',
                        func_body=gen_is_null,
                        doc_str='check if null')
        self.add_method('is_string',
                        doc_str='check if string-type',
                        func_body=gen_is_string)
        self.add_method('is_number',
                        doc_str='check if number-type',
                        func_body=gen_is_number)
        self.add_method('is_int',
                        doc_str='check if int-type',
                        func_body=gen_is_int)
        self.add_method('is_float',
                        doc_str='check if float-type',
                        func_body=gen_is_float)
        self.add_method('is_bool',
                        doc_str='check if bool-type',
                        func_body=gen_is_bool)
        self.add_method('is_object',
                        doc_str='check if bool-type',
                        func_body=gen_is_object)
        self.add_method('is_array',
                        doc_str='check if bool-type',
                        func_body=gen_is_array)
        self.add_method('has_key',
                        arg_list=[StringArg(name='key',
                                            cvarname='key')],
                        func_body=gen_has_key,
                        doc_str='check if having the key')
        self.add_method('get_string',
                        func_body=gen_get_string,
                        doc_str='get string value')
        self.add_method('get_int',
                        func_body=gen_get_int,
                        doc_str='get int value')
        self.add_method('get_float',
                        func_body=gen_get_float,
                        doc_str='get float value')
        self.add_method('get_bool',
                        func_body=gen_get_bool,
                        doc_str='get bool value')
        self.add_method('write',
                        arg_list=[StringArg(name='filename',
                                            cvarname='filename'),
                                  OptArg(),
                                  KwdArg(),
                                  BoolArg(name='indent',
                                          cvarname='indent',
                                          cvardefault='false')],
                        func_body=gen_write,
                        doc_str='write JSON data to file')
        self.add_static_method('parse',
                               arg_list=[StringArg(name='json_str',
                                                   cvarname='json_str')],
                               func_body=gen_parse,
                               doc_str='read JSON data from string')
        self.add_static_method('read',
                               arg_list=[StringArg(name='filename',
                                                   cvarname='filename')],
                               func_body=gen_read,
               doc_str='read JSON data from file')

        self.add_sequence(sq_length=gen_sq_length,
                          sq_item=gen_sq_item)

        self.add_mapping(mp_subscript=gen_mp_subscript)

        self.add_new(arg_list=[OptArg(),
                               JsonValueArg(name=None,
                                            cvarname='val')],
                     func_body=new_gen)

        self.add_getter('get_key_list',
                        func_body=key_list_gen)
        self.add_attr('key_list',
                      getter_name='get_key_list',
                      doc_str='key list')

        self.add_getter('get_item_list', func_body=item_list_gen)
        self.add_attr('item_list',
                      getter_name='get_item_list',
                      doc_str='item list')

        self.add_conv('default')

        def deconv_gen(writer):
            # PyObject* の特殊な値の場合の処理
            val_map_list = [('nullptr', 'JsonValue::null()', '"null オブジェクト"'),
                            ('Py_True', 'JsonValue(true)', '"true オブジェクト"'),
                            ('Py_False', 'JsonValue(false)', '"false オブジェクト"')]
            for obj_val, val, comment in val_map_list:
                with writer.gen_if_block(f'obj == {obj_val}'):
                    writer.gen_comment(comment)
                    writer.gen_assign('val', val)
                    writer.gen_return('true')
            # PyJsonValue の変換
            self.gen_raw_conv(writer)
            # PyObject* の拡張型に対する処理
            pytype_list = [('PyString', 'std::string', '"文字列型"'),
                           ('PyInt', 'int', '"整数型"'),
                           ('PyFloat', 'double', '"浮動小数点型"'),
                           ('PyDict<JsonValue, PyJsonValue>',
                            'std::unordered_map<std::string, JsonValue>',
                            '"辞書型"'),
                           ('PyList<JsonValue, PyJsonValue>',
                            'std::vector<JsonValue>',
                            '"シーケンス(リスト)型"')]
            for pytype, ctype, comment in pytype_list:
                with writer.gen_block():
                    writer.gen_vardecl(typename=f'{ctype}',
                                       varname='val1')
                    with writer.gen_if_block(f'{pytype}::FromPyObject(obj, val1)'):
                        writer.gen_comment(comment)
                        writer.gen_assign('val', 'JsonValue(val1)')
                        writer.gen_return('true')
            writer.gen_return('false')
        self.add_deconv(deconv_gen)


if __name__ == '__main__':
    gen = JsonValueGen()
    gen.make_header()
    gen.make_source()
