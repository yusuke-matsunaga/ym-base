
/// @file ymbase_module.cc
/// @brief Python 用の ymbase モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym_config.h"
#include "ym/PyMt19937.h"
#include "ym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義構造体
PyMethodDef ymbase_methods[] = {
  {nullptr, nullptr, 0, nullptr},
};

// モジュール定義構造体
PyModuleDef ymbase_module = {
  PyModuleDef_HEAD_INIT,
  "ymbase",
  PyDoc_STR("ymbase: Extension module for ym-base"),
  -1,
  ymbase_methods,
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

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
