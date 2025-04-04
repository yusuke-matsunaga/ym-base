
/// @file extra_json_conv.cc
/// @brief extra_json_conv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/JsonValue.h"
#include "pym/PyString.h"
#include "pym/PyLong.h"
#include "pym/PyFloat.h"


BEGIN_NAMESPACE_YM

bool
extra_json_conv(
  PyObject* obj,
  JsonValue& val
)
{
  if ( obj == nullptr ) {
    // null
    val = JsonValue::null();
    return true;
  }
  // ブール型は他の型との変換が行えるので先にチェックする．
  if ( obj == Py_True ) {
    // true
    val = JsonValue(true);
    return true;
  }
  if ( obj == Py_False ) {
    // false
    val = JsonValue(false);
    return true;
  }
  if ( PyString::Check(obj) ) {
    // 文字列型
    auto str_val = PyString::AsString(obj);
    val = JsonValue(str_val);
    return true;
  }
  if ( PyLong::Check(obj) ) {
    // 整数型
    auto int_val = PyLong::AsLong(obj);
    val = JsonValue(int_val);
    return true;
  }
  if ( PyFloat::Check(obj) ) {
    // 浮動小数点型
    auto double_val = PyFloat::AsDouble(obj);
    val = JsonValue(double_val);
    return true;
  }
  if ( PyDict_Check(obj) ) {
    // 辞書(オブジェクト)
    auto item_list = PyDict_Items(obj);
    SizeType n = PyList_Size(item_list);
    unordered_map<string, JsonValue> item_dict;
    for ( SizeType i = 0; i < n; ++ i ) {
      auto pair = PyList_GetItem(item_list, i);
      char* key = nullptr;
      PyObject* item_obj = nullptr;
      if ( !PyArg_ParseTuple(pair, "sO", &key, &item_obj) ) {
	return false;
      }
      JsonValue item_val;
      if ( !operator()(item_obj, item_val) ) {
	return false;
      }
      item_dict.emplace(key, item_val);
    }
    Py_DECREF(item_list);
    val = JsonValue(item_dict);
    return true;
  }
  // リスト
  vector<JsonValue> elem_list;
  if ( PyList<JsonValue, PyJsonValue>::FromPyObject(obj, elem_list) ) {
    val = JsonValue(elem_list);
    return true;
  }
  return false;
}

END_NAMESPACE_YM
