
/// @file PyJsonValue.cc
/// @brief Python JsonValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyJsonValue.h"
#include "pym/PyModule.h"
#include "pym/PyString.h"
#include "pym/PyLong.h"
#include "pym/PyFloat.h"
#include "pym/PyDict.h"
#include "pym/PyList.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// エラーメッセージを表す定数
static const char* EMSG_NOT_OBJ = "not an Object type";
static const char* EMSG_NOT_ARRAY = "not an Array type";
static const char* EMSG_NOT_STR = "not a string type";
static const char* EMSG_NOT_INT = "not an integer type";
static const char* EMSG_NOT_FLOAT = "not a float type";
static const char* EMSG_NOT_BOOL = "not a bool type";
static const char* EMSG_NOT_OBJ_ARRAY = "neither an object nor an array type";
static const char* EMSG_OUT_OF_RANGE = "index is out-of-range";

// Python 用のオブジェクト定義
struct JsonValueObject
{
  PyObject_HEAD
  JsonValue mVal;
};

// Python 用のタイプ定義
PyTypeObject JsonValue_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
JsonValue_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "value",
    nullptr
  };

  PyObject* obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O",
				    const_cast<char**>(kwlist),
				    &obj) ) {
    return nullptr;
  }
  JsonValue val;
  if ( !PyJsonValue::FromPyObject(obj, val) ) {
    PyErr_SetString(PyExc_TypeError, "cannot convert to JsonValue");
    return nullptr;
  }

  return PyJsonValue::ToPyObject(val);
}

// repr() 関数
PyObject*
JsonValue_repr(
  PyObject* self
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto tmp_str = val.to_json();
  return Py_BuildValue("s", tmp_str.c_str());
}

// 終了関数
void
JsonValue_dealloc(
  PyObject* self
)
{
  auto node_obj = reinterpret_cast<JsonValueObject*>(self);
  // デストラクタの明示的な呼び出し
  (node_obj->mVal).~JsonValue();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
JsonValue_null(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  return PyJsonValue::ToPyObject(JsonValue::null());
}

PyObject*
JsonValue_is_null(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_null();
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_is_string(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_string();
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_is_number(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_number();
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_is_int(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_int();
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_is_float(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_float();
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_is_bool(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_bool();
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_is_object(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_object();
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_is_array(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_array();
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_has_key(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "key",
    nullptr
  };

  const char* key = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &key) ) {
    return nullptr;
  }
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_object() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ);
    return nullptr;
  }
  auto ans = val.has_key(key);
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_get_string(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_string() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_STR);
    return nullptr;
  }
  auto ans = val.get_string();
  return Py_BuildValue("s", ans.c_str());
}

PyObject*
JsonValue_get_int(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_int() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_INT);
    return nullptr;
  }
  auto ans = val.get_int();
  return Py_BuildValue("i", ans);
}

PyObject*
JsonValue_get_float(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_float() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_FLOAT);
    return nullptr;
  }
  auto ans = val.get_float();
  return Py_BuildValue("d", ans);
}

PyObject*
JsonValue_get_bool(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_bool() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_BOOL);
    return nullptr;
  }
  auto ans = val.get_bool();
  return PyBool_FromLong(ans);
}

PyObject*
JsonValue_read(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    nullptr
  };

  const char* filename = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &filename) ) {
    return nullptr;
  }
  try {
    auto val = JsonValue::read(filename);
    return PyJsonValue::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
JsonValue_parse(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "json_str",
    nullptr
  };

  const char* json_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &json_str) ) {
    return nullptr;
  }
  try {
    auto val = JsonValue::parse(json_str);
    return PyJsonValue::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
JsonValue_write(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    "indent",
    nullptr
  };

  const char* filename = nullptr;
  int indent = false; // これを bool 型にしておくと落ちる．
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|$p",
				    const_cast<char**>(kwlist),
				    &filename, &indent) ) {
    return nullptr;
  }
  ofstream s{filename};
  if ( !s ) {
    ostringstream buff;
    buff << filename << ": Could not open";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
  }
  auto& json_value = PyJsonValue::_get_ref(self);
  json_value.write(s, static_cast<bool>(indent));

  Py_RETURN_NONE;
}

// メソッド定義
PyMethodDef JsonValue_methods[] = {
  {"null", JsonValue_null,
   METH_STATIC | METH_NOARGS,
   PyDoc_STR("make a null object")},
  {"is_null", JsonValue_is_null,
   METH_NOARGS,
   PyDoc_STR("check if null")},
  {"is_string", JsonValue_is_string,
   METH_NOARGS,
   PyDoc_STR("check if a string")},
  {"is_number", JsonValue_is_number,
   METH_NOARGS,
   PyDoc_STR("check if a number")},
  {"is_int", JsonValue_is_int,
   METH_NOARGS,
   PyDoc_STR("check if an integer")},
  {"is_float", JsonValue_is_float,
   METH_NOARGS,
   PyDoc_STR("check if a float number")},
  {"is_bool", JsonValue_is_bool,
   METH_NOARGS,
   PyDoc_STR("check if a Boolean")},
  {"is_object", JsonValue_is_object,
   METH_NOARGS,
   PyDoc_STR("check if a composed object type")},
  {"is_array", JsonValue_is_array,
   METH_NOARGS,
   PyDoc_STR("check if an array")},
  {"has_key", reinterpret_cast<PyCFunction>(JsonValue_has_key),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check if having the specified key")},
  {"get_string", JsonValue_get_string,
   METH_NOARGS,
   PyDoc_STR("return a string value")},
  {"get_int", JsonValue_get_int,
   METH_NOARGS,
   PyDoc_STR("return an integer value")},
  {"get_float", JsonValue_get_float,
   METH_NOARGS,
   PyDoc_STR("return a float value")},
  {"get_bool", JsonValue_get_bool,
   METH_NOARGS,
   PyDoc_STR("return a Boolean value")},
  {"read", reinterpret_cast<PyCFunction>(JsonValue_read),
   METH_VARARGS | METH_STATIC | METH_KEYWORDS,
   PyDoc_STR("read 'json' file")},
  {"parse", reinterpret_cast<PyCFunction>(JsonValue_parse),
   METH_VARARGS | METH_STATIC | METH_KEYWORDS,
   PyDoc_STR("parse 'json' string")},
  {"write", reinterpret_cast<PyCFunction>(JsonValue_write),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("write out to 'json' file")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
JsonValue_key_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_object() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ);
    return nullptr;
  }
  try {
    auto val_list = val.key_list();
    return PyString::ToPyList(val_list);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyObject*
JsonValue_item_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_object() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ);
    return nullptr;
  }
  try {
    auto item_list = val.item_list();
    SizeType n = item_list.size();
    auto ans = PyList_New(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto& p = item_list[i];
      auto key = p.first;
      auto value = p.second;
      auto value_obj = PyJsonValue::ToPyObject(value);
      auto item_obj = Py_BuildValue("(sO)", key.c_str(), value_obj);
      PyList_SetItem(ans, i, item_obj);
    }
    return ans;
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PyGetSetDef JsonValue_getsetters[] = {
  {"key_list", JsonValue_key_list, nullptr, PyDoc_STR("'key' list"), nullptr},
  {"item_list", JsonValue_item_list, nullptr, PyDoc_STR("'item' list"), nullptr},
  {nullptr, nullptr, nullptr, nullptr}
};

Py_ssize_t
JsonValue_length(
  PyObject* self
)
{
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_object() && !val.is_array() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ_ARRAY);
    return -1;
  }
  return val.size();
}

PyObject*
JsonValue_item(
  PyObject* self,
  Py_ssize_t index
)
{
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_array() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_ARRAY);
    return nullptr;
  }
  try {
    int index1 = ( index >= 0 ) ? index : val.size() + index;
    auto ans = val.at(index1);
    return PyJsonValue::ToPyObject(ans);
  }
  catch ( std::invalid_argument err ) {
    PyErr_SetString(PyExc_ValueError, err.what());
    return nullptr;
  }
}

PySequenceMethods JsonValue_sequence = {
  .sq_length = JsonValue_length,
  .sq_item = JsonValue_item
};

PyObject*
JsonValue_subscript(
  PyObject* self,
  PyObject* key
)
{
  auto& val = PyJsonValue::_get_ref(self);
  if ( PyUnicode_Check(key) ) {
    if ( !val.is_object() ) {
      PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ);
      return nullptr;
    }
    auto key2 = PyUnicode_EncodeLocale(key, nullptr);
    string key_str{PyBytes_AsString(key2)};
    try {
      auto ans = val.at(key_str);
      return PyJsonValue::ToPyObject(ans);
    }
    catch ( std::invalid_argument ) {
      ostringstream buf;
      buf << key_str << ": invalid key";
      PyErr_SetString(PyExc_ValueError, buf.str().c_str());
      return nullptr;
    }
  }
  if ( PyLong_Check(key) ) {
    if ( !val.is_array() ) {
      PyErr_SetString(PyExc_TypeError, EMSG_NOT_ARRAY);
      return nullptr;
    }
    auto index = PyLong_AsLong(key);
    int index1 = ( index >= 0 ) ? index : val.size() + index;
    try {
      auto ans = val.at(index1);
      return PyJsonValue::ToPyObject(ans);
    }
    catch ( std::out_of_range err ) {
      PyErr_SetString(PyExc_ValueError, EMSG_OUT_OF_RANGE);
      return nullptr;
    }
  }
  PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ_ARRAY);
  return nullptr;
}

PyMappingMethods JsonValue_mapping = {
  .mp_subscript = JsonValue_subscript
};

// 比較関数
PyObject*
JsonValue_richcompfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyJsonValue::Check(self) &&
       PyJsonValue::Check(other) ) {
    auto& val1 = PyJsonValue::_get_ref(self);
    auto& val2 = PyJsonValue::_get_ref(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(val1 == val2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(val1 != val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

END_NONAMESPACE


// @brief 'JsonValue' オブジェクトを使用可能にする．
bool
PyJsonValue::init(
  PyObject* m
)
{
  JsonValue_Type.tp_name = "JsonValue";
  JsonValue_Type.tp_basicsize = sizeof(JsonValueObject);
  JsonValue_Type.tp_itemsize = 0;
  JsonValue_Type.tp_dealloc = JsonValue_dealloc;
  JsonValue_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  JsonValue_Type.tp_doc = PyDoc_STR("JsonValue object");
  JsonValue_Type.tp_richcompare = JsonValue_richcompfunc;
  JsonValue_Type.tp_methods = JsonValue_methods;
  JsonValue_Type.tp_getset = JsonValue_getsetters;
  JsonValue_Type.tp_new = JsonValue_new;
  JsonValue_Type.tp_repr = JsonValue_repr;
  JsonValue_Type.tp_as_sequence = &JsonValue_sequence;
  JsonValue_Type.tp_as_mapping = &JsonValue_mapping;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "JsonValue", &JsonValue_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief JsonValue を PyObject に変換する．
PyObject*
PyJsonValue::Conv::operator()(
  const JsonValue& val
)
{
  auto obj = JsonValue_Type.tp_alloc(&JsonValue_Type, 0);
  auto node_obj = reinterpret_cast<JsonValueObject*>(obj);
  new (&node_obj->mVal) JsonValue(val);
  return obj;
}

// @brief PyObject を JsonValue に変換する．
bool
PyJsonValue::Deconv::operator()(
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
  if ( PyJsonValue::Check(obj) ) {
    // もともと JsonValue 型だった．
    val = PyJsonValue::_get_ref(obj);
    return true;
  }
  {
    PyString::Deconv str_dec;
    string str_val;
    if ( str_dec(obj, str_val) ) {
      // 文字列型
      val = JsonValue(str_val);
      return true;
    }
  }
  if ( PyLong::Check(obj) ) {
    // 整数型
    int arg_val = PyLong::AsLong(obj);
    val = JsonValue(arg_val);
    return true;
  }
  if ( PyFloat::Check(obj) ) {
    // 浮動小数点型
    auto arg_val = PyFloat::AsDouble(obj);
    val = JsonValue(arg_val);
    return true;
  }
  if ( PyDict<JsonValue, PyJsonValue>::Check(obj) ) {
    // 辞書(オブジェクト)
    std::unordered_map<std::string, JsonValue> item_dict;
    if ( PyDict<JsonValue, PyJsonValue>::FromPyObject(obj, item_dict) ) {
      val = JsonValue(item_dict);
      return true;
    }
  }
  if ( PyList<JsonValue, PyJsonValue>::Check(obj) ) {
    vector<JsonValue> elem_list;
    if ( PyList<JsonValue, PyJsonValue>::FromPyObject(obj, elem_list) ) {
      val = JsonValue(elem_list);
      return true;
    }
  }

  return false;
}

// @brief PyObject が JsonValue タイプか調べる．
bool
PyJsonValue::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief JsonValue を表す PyObject から JsonValue を取り出す．
JsonValue&
PyJsonValue::_get_ref(
  PyObject* obj
)
{
  auto json_obj = reinterpret_cast<JsonValueObject*>(obj);
  return json_obj->mVal;
}

// @brief JsonValue を表すオブジェクトの型定義を返す．
PyTypeObject*
PyJsonValue::_typeobject()
{
  return &JsonValue_Type;
}

END_NAMESPACE_YM
