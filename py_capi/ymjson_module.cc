
/// @file ymjson_module.cc
/// @brief Python 用の ymjson モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyJsonValue.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義構造体
PyMethodDef ymjson_methods[] = {
  {nullptr, nullptr, 0, nullptr},
};

// モジュール定義構造体
PyModuleDef ymjson_module = {
  PyModuleDef_HEAD_INIT,
  "ymjson",
  PyDoc_STR("ymjson: Extension module for json"),
  -1,
  ymjson_methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_ymjson()
{
  auto m = PyModule::init(&ymjson_module);
  if ( m == nullptr ) {
    return nullptr;
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
