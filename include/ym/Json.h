#ifndef JSON_H
#define JSON_H

/// @file Json.h
/// @brief Json のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/FileInfo.h"


BEGIN_NAMESPACE_YM

class JsonValue;

//////////////////////////////////////////////////////////////////////
/// @class Json Json.h "ym/Json.h"
/// @brief json オブジェクトを表すクラス
//////////////////////////////////////////////////////////////////////
class Json
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// null 型の値となる．
  Json();

  /// @brief 値を指定したコンストラクタ
  Json(
    JsonValue* value ///< [in] 値
  );

  /// @brief デストラクタ
  ~Json();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オブジェクト型の時 true を返す．
  bool
  is_object() const;

  /// @brief 配列型の時 true を返す．
  bool
  is_array() const;

  /// @brief 文字列型の時 true を返す．
  bool
  is_string() const;

  /// @brief 数値型の時 true を返す．
  bool
  is_number() const;

  /// @brief 整数型の時 true を返す．
  bool
  is_int() const;

  /// @brief 浮動小数点型の時 true を返す．
  bool
  is_float() const;

  /// @brief ブール型の時 true を返す．
  bool
  is_bool() const;

  /// @brief null型の時 true を返す．
  bool
  is_null() const;

  /// @brief オブジェクトがキーを持つか調べる．
  ///
  /// - オブジェクト型でない場合は無効
  bool
  has_key(
    const string& key ///< [in] キー
  ) const;

  /// @brief キーのリストを返す．
  ///
  /// - オブジェクト型でない場合は無効
  vector<string>
  key_list() const;

  /// @brief キーと値のリストを返す．
  ///
  /// - オブジェクト型でない場合は無効
  vector<pair<string, Json>>
  item_list() const;

  /// @brief オブジェクトの要素を得る．
  ///
  /// - オブジェクト型でない場合は無効
  /// - key に対応する値がない場合は null を返す．
  Json
  operator[](
    const string& key ///< [in] キー
  ) const;

  /// @brief 配列の要素数を得る．
  ///
  /// - 配列型でない場合は無効
  SizeType
  array_size() const;

  /// @brief 配列の要素を得る．
  ///
  /// - 配列型でない場合は無効
  /// - 配列のサイズ外のアクセスはエラーとなる．
  Json
  operator[](
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < array_size() )
  ) const;

  /// @brief 文字列を得る．
  ///
  /// - 文字列型でない場合は無効
  string
  get_string() const;

  /// @brief 整数値を得る．
  ///
  /// - 整数型でない場合は無効
  int
  get_int() const;

  /// @brief 浮動小数点値を得る．
  ///
  /// - 浮動小数点型でない場合は無効
  double
  get_float() const;

  /// @brief ブール値を得る．
  ///
  /// - ブール型でない場合は無効
  bool
  get_bool() const;

  /// @brief 読み込む．
  /// @return 結果を格納したオブジェクトを返す．
  static
  Json
  read(
    istream& s,               ///< [in] 入力ストリーム
    const FileInfo& file_info ///< [in] ファイル情報
  );

  /// @brief 内容を書き出す．
  void
  write(
    ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値の実体
  shared_ptr<JsonValue> mPtr;

};

END_NAMESPACE_YM

#endif // JSON_H
