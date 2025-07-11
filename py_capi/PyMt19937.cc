
/// @file PyMt19937.cc
/// @brief PyMt19937 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyMt19937.h"
#include <sstream>
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct Mt19937_Object
{
  PyObject_HEAD
  std::mt19937 mVal;
};

// Python 用のタイプ定義
PyTypeObject Mt19937_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyMt19937::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<Mt19937_Object*>(self);
  // 実は mt19937 はクラス名ではない．
  obj->mVal.~mersenne_twister_engine();
  Py_TYPE(self)->tp_free(self);
}

// generate a random number
PyObject*
eval(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyMt19937::_get_ref(self);
  auto rand_val = val.operator()();
  return PyLong_FromLong(rand_val);
}

// メソッド定義
PyMethodDef methods[] = {
  {"eval",
   eval,
   METH_NOARGS,
   PyDoc_STR("generate a random number")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
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
    "seed",
    nullptr
  };
  int seed_val = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$i",
                                    const_cast<char**>(kwlist),
                                    &seed_val) ) {
    return nullptr;
  }
  try {
    auto obj = type->tp_alloc(type, 0);
    auto obj1 = reinterpret_cast<Mt19937_Object*>(obj);
    new (&obj1->mVal) std::mt19937;
    if ( seed_val != -1 ) {
      obj1->mVal.seed(seed_val);
    }
    return obj;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

END_NONAMESPACE


// @brief Mt19937 オブジェクトを使用可能にする．
bool
PyMt19937::init(
  PyObject* m
)
{
  Mt19937_Type.tp_name = "Mt19937";
  Mt19937_Type.tp_basicsize = sizeof(Mt19937_Object);
  Mt19937_Type.tp_itemsize = 0;
  Mt19937_Type.tp_dealloc = dealloc_func;
  Mt19937_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  Mt19937_Type.tp_doc = PyDoc_STR("Python extended object for std::mt19937");
  Mt19937_Type.tp_methods = methods;
  Mt19937_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "Mt19937", &Mt19937_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief PyObject が std::mt19937 タイプか調べる．
bool
PyMt19937::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &Mt19937_Type);
}

// @brief PyObject から std::mt19937 を取り出す．
std::mt19937&
PyMt19937::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<Mt19937_Object*>(obj);
  return my_obj->mVal;
}

// @brief std::mt19937 を表すオブジェクトの型定義を返す．
PyTypeObject*
PyMt19937::_typeobject()
{
  return &Mt19937_Type;
}

END_NAMESPACE_YM
