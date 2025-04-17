
/// @file PyJsonValue.cc
/// @brief PyJsonValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyJsonValue.h"
#include "pym/PyString.h"
#include "pym/PyInt.h"
#include "pym/PyFloat.h"
#include "pym/PyDict.h"
#include "pym/PyList.h"
#include "ym/JsonValue.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct JsonValue_Object
{
  PyObject_HEAD
  JsonValue mVal;
};

// Python 用のタイプ定義
PyTypeObject JsonValue_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyJsonValue::init() 中で初期化する．
};

// エラーメッセージを表す定数
static const char* EMSG_NOT_OBJ = "not an Object type";
static const char* EMSG_NOT_ARRAY = "not an Array type";
static const char* EMSG_NOT_STR = "not a string type";
static const char* EMSG_NOT_INT = "not an integer type";
static const char* EMSG_NOT_FLOAT = "not a float type";
static const char* EMSG_NOT_BOOL = "not a bool type";
static const char* EMSG_NOT_OBJ_ARRAY = "neither an object nor an array type";
static const char* EMSG_OUT_OF_RANGE = "index is out-of-range";

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<JsonValue_Object*>(self);
  obj->mVal.~JsonValue();
  Py_TYPE(self)->tp_free(self);
}

// repr 関数
PyObject*
repr_func(
  PyObject* self
)
{
  auto& val = PyJsonValue::_get_ref(self);
  try {
    auto str_val = val.to_json();
    return PyString::ToPyObject(str_val);
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

Py_ssize_t
sq_length(
  PyObject* self
)
{
  auto& val = PyJsonValue::_get_ref(self);
  try {
    if ( !val.is_object() && !val.is_array() ) {
      PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ_ARRAY);
      return -1;
    }
    auto len_val = val.size();
    return len_val;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return -1;
  }
}

PyObject*
sq_item(
  PyObject* self,
  Py_ssize_t index
)
{
  auto& val = PyJsonValue::_get_ref(self);
  try {
    if ( !val.is_array() ) {
      PyErr_SetString(PyExc_TypeError, EMSG_NOT_ARRAY);
      return nullptr;
    }
    try {
      auto index1 = ( index >= 0 ) ? index : val.size() + index;
      return PyJsonValue::ToPyObject(val.at(index1));
    }
    catch ( std::invalid_argument err ) {
      std::ostringstream buf;
      buf << "invalid argument" << ": " << err.what();
      PyErr_SetString(PyExc_ValueError, buf.str().c_str());
      return nullptr;
    }
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// Sequence オブジェクト構造体
PySequenceMethods sequence = {
  .sq_length = sq_length,
  .sq_item = sq_item
};

PyObject*
mp_subscript(
  PyObject* self,
  PyObject* key
)
{
  auto& val = PyJsonValue::_get_ref(self);
  try {
    if ( PyString::Check(key) ) {
      if ( !val.is_object() ) {
        PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ);
        return nullptr;
      }
      auto key_str = PyString::Get(key);
      try {
        return PyJsonValue::ToPyObject(val.at(key_str));
      }
      catch ( std::invalid_argument err ) {
        std::ostringstream buf;
        buf << "invalid argument" << ": " << err.what();
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
      auto index1 = ( index >= 0 ) ? index : val.size() + index;
      try {
        return PyJsonValue::ToPyObject(val.at(index1));
      }
      catch ( std::out_of_range err ) {
        std::ostringstream buf;
        buf << EMSG_OUT_OF_RANGE << ": " << err.what();
        PyErr_SetString(PyExc_ValueError, buf.str().c_str());
        return nullptr;
      }
    }
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ_ARRAY);
    return nullptr;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// Mapping オブジェクト構造体
PyMappingMethods mapping = {
  .mp_subscript = mp_subscript
};

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyJsonValue::_get_ref(self);
  try {
    if ( PyJsonValue::Check(self) && PyJsonValue::Check(other) ) {
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
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make null object
PyObject*
null(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  return PyJsonValue::ToPyObject(JsonValue::null());
}

// check if null
PyObject*
is_null(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_null();
  return PyBool_FromLong(ans);
}

// check if string-type
PyObject*
is_string(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_string();
  return PyBool_FromLong(ans);
}

// check if number-type
PyObject*
is_number(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_number();
  return PyBool_FromLong(ans);
}

// check if int-type
PyObject*
is_int(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_int();
  return PyBool_FromLong(ans);
}

// check if float-type
PyObject*
is_float(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_float();
  return PyBool_FromLong(ans);
}

// check if bool-type
PyObject*
is_bool(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_bool();
  return PyBool_FromLong(ans);
}

// check if bool-type
PyObject*
is_object(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_object();
  return PyBool_FromLong(ans);
}

// check if bool-type
PyObject*
is_array(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyJsonValue::_get_ref(self);
  auto ans = val.is_array();
  return PyBool_FromLong(ans);
}

// check if having the key
PyObject*
has_key(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "key",
    nullptr
  };
  const char* key_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &key_tmp) ) {
    return nullptr;
  }
  std::string key;
  if ( key_tmp != nullptr ) {
    key = std::string(key_tmp);
  }
  auto& val = PyJsonValue::_get_ref(self);
  if ( !val.is_object() ) {
    PyErr_SetString(PyExc_TypeError, EMSG_NOT_OBJ);
    return nullptr;
  }
  auto ans = val.has_key(key);
  return PyBool_FromLong(ans);
}

// get string value
PyObject*
get_string(
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
  return PyString::ToPyObject(ans);
}

// get int value
PyObject*
get_int(
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
  return PyInt::ToPyObject(ans);
}

// get float value
PyObject*
get_float(
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
  return PyFloat::ToPyObject(ans);
}

// get bool value
PyObject*
get_bool(
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

// write JSON data to file
PyObject*
write(
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
  const char* filename_tmp = nullptr;
  int indent_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|$p",
                                    const_cast<char**>(kwlist),
                                    &filename_tmp,
                                    &indent_tmp) ) {
    return nullptr;
  }
  std::string filename;
  if ( filename_tmp != nullptr ) {
    filename = std::string(filename_tmp);
  }
  bool indent = false;
  if ( indent_tmp != -1 ) {
    indent = static_cast<bool>(indent_tmp);
  }
  auto& val = PyJsonValue::_get_ref(self);
  std::ofstream s;
  if ( !s ) {
    std:ostringstream buff;
    buff << filename << ": Could not open.";
    PyErr_SetString(PyExc_ValueError, buff.str().c_str());
    return nullptr;
    val.write(s, indent);
  }
  Py_RETURN_NONE;
}

// read JSON data from string
PyObject*
parse(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "json_str",
    nullptr
  };
  const char* json_str_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &json_str_tmp) ) {
    return nullptr;
  }
  std::string json_str;
  if ( json_str_tmp != nullptr ) {
    json_str = std::string(json_str_tmp);
  }
  try {
    auto val = JsonValue::parse(json_str);
    return PyJsonValue::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// read JSON data from file
PyObject*
read(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    nullptr
  };
  const char* filename_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &filename_tmp) ) {
    return nullptr;
  }
  std::string filename;
  if ( filename_tmp != nullptr ) {
    filename = std::string(filename_tmp);
  }
  try {
    auto val = JsonValue::read(filename);
    return PyJsonValue::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef methods[] = {
  {"null",
   null,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make null object")},
  {"is_null",
   is_null,
   METH_NOARGS,
   PyDoc_STR("check if null")},
  {"is_string",
   is_string,
   METH_NOARGS,
   PyDoc_STR("check if string-type")},
  {"is_number",
   is_number,
   METH_NOARGS,
   PyDoc_STR("check if number-type")},
  {"is_int",
   is_int,
   METH_NOARGS,
   PyDoc_STR("check if int-type")},
  {"is_float",
   is_float,
   METH_NOARGS,
   PyDoc_STR("check if float-type")},
  {"is_bool",
   is_bool,
   METH_NOARGS,
   PyDoc_STR("check if bool-type")},
  {"is_object",
   is_object,
   METH_NOARGS,
   PyDoc_STR("check if bool-type")},
  {"is_array",
   is_array,
   METH_NOARGS,
   PyDoc_STR("check if bool-type")},
  {"has_key",
   reinterpret_cast<PyCFunction>(has_key),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check if having the key")},
  {"get_string",
   get_string,
   METH_NOARGS,
   PyDoc_STR("get string value")},
  {"get_int",
   get_int,
   METH_NOARGS,
   PyDoc_STR("get int value")},
  {"get_float",
   get_float,
   METH_NOARGS,
   PyDoc_STR("get float value")},
  {"get_bool",
   get_bool,
   METH_NOARGS,
   PyDoc_STR("get bool value")},
  {"write",
   reinterpret_cast<PyCFunction>(write),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("write JSON data to file")},
  {"parse",
   reinterpret_cast<PyCFunction>(parse),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read JSON data from string")},
  {"read",
   reinterpret_cast<PyCFunction>(read),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read JSON data from file")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_key_list(
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
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_item_list(
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
    auto n = item_list.size();
    auto ans = PyList_New(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto& p = item_list[i];
      auto key = p.first;
      auto value = p.second;
      auto value_obj = PyJsonValue::ToPyObject(value);
      auto item_obj = Py_BuildValue("sO", key.c_str(), value_obj);
      PyList_SET_ITEM(ans, i, item_obj);
    }
    return ans;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"key_list", get_key_list, nullptr, PyDoc_STR("key list"), nullptr},
  {"item_list", get_item_list, nullptr, PyDoc_STR("item list"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

// new 関数
PyObject*
new_func(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    nullptr
  };
  PyObject* val_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O",
                                    const_cast<char**>(kwlist),
                                    &val_obj) ) {
    return nullptr;
  }
  JsonValue val;
  if ( val_obj != nullptr ) {
    if ( !PyJsonValue::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to JsonValue");
      return nullptr;
    }
  }
  try {
    return PyJsonValue::ToPyObject(val);
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

END_NONAMESPACE


// @brief JsonValue オブジェクトを使用可能にする．
bool
PyJsonValue::init(
  PyObject* m
)
{
  JsonValue_Type.tp_name = "JsonValue";
  JsonValue_Type.tp_basicsize = sizeof(JsonValue_Object);
  JsonValue_Type.tp_itemsize = 0;
  JsonValue_Type.tp_dealloc = dealloc_func;
  JsonValue_Type.tp_repr = repr_func;
  JsonValue_Type.tp_as_sequence = &sequence;
  JsonValue_Type.tp_as_mapping = &mapping;
  JsonValue_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  JsonValue_Type.tp_doc = PyDoc_STR("Python extended object for JsonValue");
  JsonValue_Type.tp_richcompare = richcompare_func;
  JsonValue_Type.tp_methods = methods;
  JsonValue_Type.tp_getset = getsets;
  JsonValue_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "JsonValue", &JsonValue_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// JsonValue を PyObject に変換する．
PyObject*
PyJsonValue::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyJsonValue::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<JsonValue_Object*>(obj);
  new (&my_obj->mVal) JsonValue(val);
  return obj;
}

// PyObject を JsonValue に変換する．
bool
PyJsonValue::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( obj == nullptr ) {
    // "null オブジェクト"
    val = JsonValue::null();
    return true;
  }
  if ( obj == Py_True ) {
    // "true オブジェクト"
    val = JsonValue(true);
    return true;
  }
  if ( obj == Py_False ) {
    // "false オブジェクト"
    val = JsonValue(false);
    return true;
  }
  if ( PyJsonValue::Check(obj) ) {
    val = PyJsonValue::_get_ref(obj);
    return true;
  }

  {
    std::string val1;
    if ( PyString::FromPyObject(obj, val1) ) {
      // "文字列型"
      val = JsonValue(val1);
      return true;
    }
  }

  {
    int val1;
    if ( PyInt::FromPyObject(obj, val1) ) {
      // "整数型"
      val = JsonValue(val1);
      return true;
    }
  }

  {
    double val1;
    if ( PyFloat::FromPyObject(obj, val1) ) {
      // "浮動小数点型"
      val = JsonValue(val1);
      return true;
    }
  }

  {
    std::unordered_map<std::string, JsonValue> val1;
    if ( PyDict<JsonValue, PyJsonValue>::FromPyObject(obj, val1) ) {
      // "辞書型"
      val = JsonValue(val1);
      return true;
    }
  }

  {
    std::vector<JsonValue> val1;
    if ( PyList<JsonValue, PyJsonValue>::FromPyObject(obj, val1) ) {
      // "シーケンス(リスト)型"
      val = JsonValue(val1);
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
  return Py_IS_TYPE(obj, &JsonValue_Type);
}

// @brief PyObject から JsonValue を取り出す．
JsonValue&
PyJsonValue::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<JsonValue_Object*>(obj);
  return my_obj->mVal;
}

// @brief JsonValue を表すオブジェクトの型定義を返す．
PyTypeObject*
PyJsonValue::_typeobject()
{
  return &JsonValue_Type;
}

END_NAMESPACE_YM
