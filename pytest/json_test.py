#! /usr/bin/env python3

"""JsonValue のテストプログラム

:file: json_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import pytest
from ymbase import JsonValue

class JsonValueTest:

    @staticmethod
    def test_not_number(js_obj):
        assert not js_obj.is_number()
        JsonValueTest.test_not_integer(js_obj)
        JsonValueTest.test_not_float(js_obj)
        
    @staticmethod
    def test_not_sized(js_obj):
        with pytest.raises(Exception) as e:
            len(js_obj)
        assert e.type == TypeError
        assert str(e.value) == "neither an object nor an array type"
        JsonValueTest.test_not_array(js_obj)
        JsonValueTest.test_not_object(js_obj)

    @staticmethod
    def test_not_string(js_obj):
        """string 型でない場合に予想される結果のテスト"""
        assert not js_obj.is_string()
        with pytest.raises(Exception) as e:
            js_obj.get_string()
        assert e.type == TypeError
        assert str(e.value) == "not a string type"

    @staticmethod
    def test_not_integer(js_obj):
        """integer 型でない場合に予想される結果のテスト"""
        assert not js_obj.is_int()
        with pytest.raises(Exception) as e:
            js_obj.get_int()
        assert e.type == TypeError
        assert str(e.value) == "not an integer type"

    @staticmethod
    def test_not_float(js_obj):
        """float 型でない場合に予想される結果のテスト"""
        assert not js_obj.is_float()
        with pytest.raises(Exception) as e:
            js_obj.get_float()
            assert e.type == TypeError
        assert str(e.value) == "not a float type"

    @staticmethod
    def test_not_bool(js_obj):
        """bool 型でない場合に予想される結果のテスト"""
        assert not js_obj.is_bool()
        with pytest.raises(Exception) as e:
            js_obj.get_bool()
            assert e.type == TypeError
            assert str(e.value) == "not a bool type"
    
    @staticmethod
    def test_not_object(js_obj):
        """object 型でない場合に予想される結果のテスト"""
        assert not js_obj.is_object()

        with pytest.raises(Exception) as e:
            js_obj.has_key("abc")
        assert e.type == TypeError
        assert str(e.value) == "not an Object type"

        with pytest.raises(Exception) as e:
            js_obj.key_list()
        assert e.type == TypeError
        assert str(e.value) == "not an Object type"

        with pytest.raises(Exception) as e:
            js_obj.item_list()
        assert e.type == TypeError
        assert str(e.value) == "not an Object type"

        with pytest.raises(Exception) as e:
            js_obj["abc"]
        assert e.type == TypeError
        assert str(e.value) == "not an Object type"

    @staticmethod
    def test_not_array(js_obj):
        assert not js_obj.is_array()
        with pytest.raises(Exception) as e:
            js_obj[0]
        assert e.type == TypeError
        assert str(e.value) == "not an Array type"
        
        
def test_empty_constr():
    js_obj = JsonValue()

    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_string(js_obj)
    JsonValueTest.test_not_integer(js_obj)
    JsonValueTest.test_not_float(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_null()

    js_str = str(js_obj)
    assert js_str == "null"

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue()
    assert js_obj == js_obj3

def test_null():
    js_obj = JsonValue.null()

    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_string(js_obj)
    JsonValueTest.test_not_integer(js_obj)
    JsonValueTest.test_not_float(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_null()

    js_str = str(js_obj)
    assert js_str == "null"

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue()
    assert js_obj == js_obj3

def test_string1():
    value = "abcde"
    js_obj = JsonValue(value)

    assert not js_obj.is_null()
    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_integer(js_obj)
    JsonValueTest.test_not_float(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_string()
    assert js_obj.get_string() == value

    js_str = str(js_obj)
    assert js_str == '"' + value + '"'

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue(value)
    assert js_obj == js_obj3

def test_string_dq():
    value = '"abcde"'
    js_obj = JsonValue(value)

    assert not js_obj.is_null()
    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_integer(js_obj)
    JsonValueTest.test_not_float(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_string()
    assert js_obj.get_string() == value

    js_str = str(js_obj)
    assert js_str == "'" + value + "'"

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue(value)
    assert js_obj == js_obj3

def test_string_sq():
    value = "'abcde'"
    js_obj = JsonValue(value)

    assert not js_obj.is_null()
    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_integer(js_obj)
    JsonValueTest.test_not_float(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_string()
    assert js_obj.get_string() == value

    js_str = str(js_obj)
    assert js_str == '"' + value + '"'

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue(value)
    assert js_obj == js_obj3

def test_string_dqsq():
    value = "\"'abcde'\""
    js_obj = JsonValue(value)

    assert not js_obj.is_null()
    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_integer(js_obj)
    JsonValueTest.test_not_float(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_string()
    assert js_obj.get_string() == value

    js_str = str(js_obj)
    assert js_str == '"\\\"\'abcde\'\\\""'

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue(value)
    assert js_obj == js_obj3

def test_integer():
    value = 99
    js_obj = JsonValue(value)

    assert not js_obj.is_null()
    JsonValueTest.test_not_string(js_obj)
    JsonValueTest.test_not_float(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_number()
    assert js_obj.is_int()
    assert js_obj.get_int() == value

    js_str = str(js_obj)
    exp_str = f'{value}'
    assert js_str == exp_str

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue(value)
    assert js_obj == js_obj3

def test_float():
    value = 1.2345
    js_obj = JsonValue(value)

    assert not js_obj.is_null()
    JsonValueTest.test_not_string(js_obj)
    JsonValueTest.test_not_integer(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_number()
    assert js_obj.is_float()
    assert js_obj.get_float() == value

    js_str = str(js_obj)
    exp_str = f'{value}'
    assert js_str == exp_str

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue(value)
    assert js_obj == js_obj3

def test_true():
    js_obj = JsonValue(True)

    assert not js_obj.is_null()
    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_string(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_bool()
    
    js_str = str(js_obj)
    assert js_str == "true"

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue(True)
    assert js_obj == js_obj3

def test_false():
    js_obj = JsonValue(False)

    assert not js_obj.is_null()
    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_string(js_obj)
    JsonValueTest.test_not_sized(js_obj)

    assert js_obj.is_bool()
    
    js_str = str(js_obj)
    assert js_str == "false"

    js_obj2 = JsonValue.parse(js_str)
    assert js_obj2 == js_obj

    js_obj3 = JsonValue(False)
    assert js_obj == js_obj3

def test_array1():
    value1 = "xyz"
    js_obj1 = JsonValue(value1)

    value2 = 2
    js_obj2 = JsonValue(value2)

    value3 = 0.99
    js_obj3 = JsonValue(value3)

    js_obj = JsonValue([js_obj1, js_obj2, js_obj3])

    assert not js_obj.is_null()
    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_string(js_obj)
    JsonValueTest.test_not_integer(js_obj)
    JsonValueTest.test_not_float(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_object(js_obj)
    
    assert js_obj.is_array()

    assert len(js_obj) == 3
    assert js_obj[0] == js_obj1
    assert js_obj[1] == js_obj2
    assert js_obj[2] == js_obj3

    with pytest.raises(Exception) as e:
        js_obj[3]
    assert e.type == ValueError
    assert str(e.value) == "index is out-of-range"

def test_object1():
    value1 = "xyz"
    js_obj1 = JsonValue(value1)

    value2 = 2
    js_obj2 = JsonValue(value2)

    value3 = 0.99
    js_obj3 = JsonValue(value3)

    js_obj = JsonValue({"key1": js_obj1,
                        "key2": js_obj2,
                        "key3": js_obj3})

    assert not js_obj.is_null()
    JsonValueTest.test_not_number(js_obj)
    JsonValueTest.test_not_string(js_obj)
    JsonValueTest.test_not_integer(js_obj)
    JsonValueTest.test_not_float(js_obj)
    JsonValueTest.test_not_bool(js_obj)
    JsonValueTest.test_not_array(js_obj)

    assert js_obj.is_object()
    assert len(js_obj) == 3

    assert js_obj["key1"] == js_obj1
    assert js_obj["key2"] == js_obj2
    assert js_obj["key3"] == js_obj3

    with pytest.raises(Exception) as e:
        js_obj["abc"]
    assert e.type == ValueError
    assert str(e.value) == "abc: invalid key"

def test_object2():
    js_obj = JsonValue({"key1": "value1"})
