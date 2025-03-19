#ifndef PYMT19937_H
#define PYMT19937_H

/// @file PyMt19937.h
/// @brief PyMt19937 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym_config.h"
#include <random>


BEGIN_NAMESPACE_YM

// このクラスは PyMt19937Conv/PyMt19937Deconv を持たない．

//////////////////////////////////////////////////////////////////////
/// @class PyMt19937 PyMt19937.h "PyMt19937.h"
/// @brief mt19937 を Python から使用するための拡張
///
/// 実際には static メンバ関数しか持たないのでクラスではない．
//////////////////////////////////////////////////////////////////////
class PyMt19937
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @return 初期化が成功したら true を返す．
  static
  bool
  init(
    PyObject* m ///< [in] 親のモジュールを表す PyObject
  );

  /// @brief PyObject が mt19937 タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief mt19937 を表す PyObject から mt19937 を取り出す．
  /// @return mt19937 を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  std::mt19937&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief mt19937 を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYMT19937_H
