
/// @file PyMt19937.cc
/// @brief PyMt19937 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyMt19937.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// 通常この構造体は同じサイズのヒープから作られるので
// コンストラクタは起動されない．
// そのためメンバはポインタの形のほうが安全．
struct Mt19937Object
{
  PyObject_HEAD
  std::mt19937* mPtr;
};

// 生成関数
PyObject*
Mt19937_new(
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
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|i",
				    const_cast<char**>(kwlist),
				    &seed_val) ) {
    return nullptr;
  }
  auto obj = type->tp_alloc(type, 0);
  auto mt_obj = reinterpret_cast<Mt19937Object*>(obj);
  auto randgen = new std::mt19937;
  mt_obj->mPtr = randgen;
  if ( seed_val != -1 ) {
    randgen->seed(seed_val);
  }
  return obj;
}

// 終了関数
void
Mt19937_dealloc(
  PyObject* self
)
{
  auto mt_obj = reinterpret_cast<Mt19937Object*>(self);
  delete mt_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

// eval() 関数
PyObject*
Mt19937_eval(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& randgen = PyMt19937::Get(self);
  auto val = randgen.operator()();
  return PyLong_FromLong(val);
}

// メソッド定義
PyMethodDef Mt19937_methods[] = {
  {"eval", Mt19937_eval, METH_NOARGS,
   PyDoc_STR("generate a random number")},
  {nullptr, nullptr, 0, nullptr}
};

// Python 用のタイプ定義
PyTypeObject Mt19937Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyInit_Mt19937 中で初期化する．
};

END_NONAMESPACE


// @brief 'Mt19937' オブジェクトを使用可能にする．
bool
PyMt19937::init(
  PyObject* m
)
{
  Mt19937Type.tp_name = "matoth.Mt19937";
  Mt19937Type.tp_basicsize = sizeof(Mt19937Object);
  Mt19937Type.tp_itemsize = 0;
  Mt19937Type.tp_dealloc = Mt19937_dealloc;
  Mt19937Type.tp_flags = Py_TPFLAGS_DEFAULT;
  Mt19937Type.tp_doc = PyDoc_STR("Mt19937 object");
  Mt19937Type.tp_methods = Mt19937_methods;
  Mt19937Type.tp_new = Mt19937_new;

  if ( !PyModule::reg_type(m, "Mt19937", &Mt19937Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief PyObject が mt19937 タイプか調べる．
bool
PyMt19937::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &Mt19937Type);
}

// @brief mt19937 を表す PyObject から mt19937 を取り出す．
std::mt19937&
PyMt19937::Get(
  PyObject* obj
)
{
  auto mt_obj = reinterpret_cast<Mt19937Object*>(obj);
  return *(mt_obj->mPtr);
}

// @brief mt19937 を表すオブジェクトの型定義を返す．
PyTypeObject*
PyMt19937::_typeobject()
{
  return &Mt19937Type;
}

END_NAMESPACE_YM
