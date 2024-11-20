#ifndef PYJSONVALUE_H
#define PYJSONVALUE_H

/// @file PyJsonValue.h
/// @brief PyJsonValue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym_config.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyJsonValue PyJsonValue.h "PyJsonValue.h"
/// @brief JsonValue を Python から使用するための拡張
///
/// 実際には static メンバ関数しか持たないのでクラスではない．
//////////////////////////////////////////////////////////////////////
class PyJsonValue
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

  /// @brief JsonValue を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const JsonValue& val ///< [in] 値
  );

  /// @brief PyObject が JsonValue タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief JsonValue を表す PyObject から JsonValue を取り出す．
  /// @return JsonValue を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  const JsonValue&
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief PyObject を JsonValue に変換する．
  /// @return 変換に成功した true を返す．
  static
  bool
  ConvToJsonValue(
    PyObject* obj, ///< [in] 対象の PyObject
    JsonValue& val ///< [out] 変換後の値を格納する変数
  );

  /// @brief JsonValue を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYJSONVALUE_H
