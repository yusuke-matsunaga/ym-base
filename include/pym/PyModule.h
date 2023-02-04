#ifndef PYMODULE_H
#define PYMODULE_H

/// @file PyModule.h
/// @brief PyModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyModule PyModule.h "PyModule.h"
/// @brief モジュールを表す PyObject に対応するクラス
//////////////////////////////////////////////////////////////////////
class PyModule
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @return 生成したモジュールおbジェクトを返す．
  static
  PyObject*
  init(
    PyModuleDef* def ///< [in] モジュール定義構造体
  )
  {
    auto m = PyModule_Create(def);
    return m;
  }

  /// @brief 要素を登録する．
  static
  bool
  reg_item(
    PyObject* m,      ///< [in] モジュールオブジェクト
    const char* name, ///< [in] 名前
    PyObject* obj     ///< [in] 登録する要素
  )
  {
    if ( obj == nullptr ) {
      return false;
    }
    if ( PyModule_AddObject(m, name, obj) < 0 ) {
      Py_DECREF(obj);
      return false;
    }
    return true;
  }

  /// @brief サブモジュールを登録する．
  static
  bool
  reg_submodule(
    PyObject* m,      ///< [in] モジュールオブジェクト
    const char* name, ///< [in] 名前
    PyObject* sub_m   ///< [in] 登録するサブモジュール
  )
  {
    if ( !reg_item(m, name, sub_m) ) {
      return false;
    }
    auto sys_modules = PyImport_GetModuleDict();
    auto m_name = string{PyModule_GetName(m)};
    string h_name = m_name + "." + name;
    if ( PyDict_SetItemString(sys_modules,
			      h_name.c_str(), sub_m) < 0 ) {
      return false;
    }
    return true;
  }

  /// @brief 型オブジェクトを登録する．
  static
  bool
  reg_type(
    PyObject* m,           ///< [in] モジュールオブジェクト
    const char* name,      ///< [in] 名前
    PyTypeObject* type_obj ///< [in] 型オブジェクト
  )
  {
    if ( PyType_Ready(type_obj) < 0 ) {
      return false;
    }
    auto obj = reinterpret_cast<PyObject*>(type_obj);
    Py_INCREF(obj);
    if ( PyModule_AddObject(m, name, obj) < 0 ) {
      Py_DECREF(obj);
      return false;
    }
    return true;
  }

};

END_NAMESPACE_YM

#endif // PYMODULE_H
