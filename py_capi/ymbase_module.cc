
/// @file ymbase_module.cc
/// @brief Python 用の ymbase モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyMt19937.h"
#include "pym/PyJsonValue.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義
PyMethodDef methods[] = {
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

// モジュール定義構造体
PyModuleDef ymbase_module = {
  PyModuleDef_HEAD_INIT,
  "ymbase",
  PyDoc_STR(""),
  -1,
  methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_ymbase()
{
  auto m = PyModule::init(&ymbase_module);
  if ( m == nullptr ) {
    return nullptr;
  }
  
  if ( !PyMt19937::init(m) ) {
    goto error;
  }
  if ( !PyJsonValue::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
