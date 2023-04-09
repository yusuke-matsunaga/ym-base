#ifndef JSONVALUE_H
#define JSONVALUE_H

/// @file JsonValue.h
/// @brief JsonValue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Json.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class JsonValue JsonValue.h "JsonValue.h"
/// @brief json の値を表す基底クラス
//////////////////////////////////////////////////////////////////////
class JsonValue
{
public:

  /// @brief デストラクタ
  virtual
  ~JsonValue() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オブジェクト型の時 true を返す．
  virtual
  bool
  is_object() const;

  /// @brief 配列型の時 true を返す．
  virtual
  bool
  is_array() const;

  /// @brief 文字列型の時 true を返す．
  virtual
  bool
  is_string() const;

  /// @brief 整数型の時 true を返す．
  virtual
  bool
  is_int() const;

  /// @brief 浮動小数点型の時 true を返す．
  virtual
  bool
  is_float() const;

  /// @brief ブール型の時 true を返す．
  virtual
  bool
  is_bool() const;

  /// @brief オブジェクトがキーを持つか調べる．
  ///
  /// - オブジェクト型でない場合は無効
  virtual
  bool
  has_key(
    const string& key ///< [in] キー
  ) const;

  /// @brief キーのリストを返す．
  ///
  /// - オブジェクト型でない場合は無効
  virtual
  vector<string>
  key_list() const;

  /// @brief キーと値のリストを返す．
  ///
  /// - オブジェクト型でない場合は無効
  virtual
  vector<pair<string, Json>>
  item_list() const;

  /// @brief オブジェクトの要素を得る．
  ///
  /// - オブジェクト型でない場合は無効
  /// - key に対応する値がない場合は null を返す．
  virtual
  Json
  get_value(
    const string& key ///< [in] キー
  ) const;

  /// @brief 配列の要素数を得る．
  ///
  /// - 配列型でない場合は無効
  virtual
  SizeType
  array_size() const;

  /// @brief 配列の要素を得る．
  ///
  /// - 配列型でない場合は無効
  /// - 配列のサイズ外のアクセスはエラーとなる．
  virtual
  Json
  get_value(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < array_size() )
  ) const;

  /// @brief 文字列を得る．
  ///
  /// - 文字列型でない場合は無効
  virtual
  string
  get_string() const;

  /// @brief 整数値を得る．
  ///
  /// - 整数型でない場合は無効
  virtual
  int
  get_int() const;

  /// @brief 浮動小数点値を得る．
  ///
  /// - 浮動小数点型でない場合は無効
  virtual
  double
  get_float() const;

  /// @brief ブール値を得る．
  ///
  /// - ブール型でない場合は無効
  virtual
  bool
  get_bool() const;

};


//////////////////////////////////////////////////////////////////////
/// @class JsonObject JsonValue.h "JsonValue.h"
/// @brief オブジェクト型を表すクラス
//////////////////////////////////////////////////////////////////////
class JsonObject :
  public JsonValue
{
public:

  /// @brief コンストラクタ
  JsonObject(
    const unordered_map<string, Json>& dict ///< [in] 本体の辞書
  );

  /// @brief デストラクタ
  ~JsonObject();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オブジェクト型の時 true を返す．
  bool
  is_object() const override;

  /// @brief オブジェクトがキーを持つか調べる．
  ///
  /// - オブジェクト型でない場合は無効
  bool
  has_key(
    const string& key ///< [in] キー
  ) const override;

  /// @brief キーのリストを返す．
  ///
  /// - オブジェクト型でない場合は無効
  vector<string>
  key_list() const override;

  /// @brief キーと値のリストを返す．
  ///
  /// - オブジェクト型でない場合は無効
  vector<pair<string, Json>>
  item_list() const override;

  /// @brief オブジェクトの要素を得る．
  ///
  /// - オブジェクト型でない場合は無効
  /// - key に対応する値がない場合は null を返す．
  Json
  get_value(
    const string& key ///< [in] キー
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  unordered_map<string, Json> mDict;

};


//////////////////////////////////////////////////////////////////////
/// @class JsonArray JsonArray.h "JsonArray.h"
/// @brief 配列型を表すクラス
//////////////////////////////////////////////////////////////////////
class JsonArray :
  public JsonValue
{
public:

  /// @brief コンストラクタ
  JsonArray(
    const vector<Json>& array ///< [in] 配列の本体
  );

  /// @brief デストラクタ
  ~JsonArray();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列型の時 true を返す．
  bool
  is_array() const override;

  /// @brief 配列の要素数を得る．
  ///
  /// - 配列型でない場合は無効
  SizeType
  array_size() const override;

  /// @brief 配列の要素を得る．
  ///
  /// - 配列型でない場合は無効
  /// - 配列のサイズ外のアクセスはエラーとなる．
  Json
  get_value(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < array_size() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 配列の実体
  vector<Json> mArray;

};


//////////////////////////////////////////////////////////////////////
/// @class JsonString JsonString.h "JsonString.h"
/// @brief 文字列型を表すクラス
//////////////////////////////////////////////////////////////////////
class JsonString :
  public JsonValue
{
public:

  /// @brief コンストラクタ
  JsonString(
    const string& value ///< [in] 文字列の値
  );

  /// @brief デストラクタ
  ~JsonString();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列型の時 true を返す．
  bool
  is_string() const override;

  /// @brief 文字列を得る．
  ///
  /// - 文字列型でない場合は無効
  string
  get_string() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列の本体
  string mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class JsonInt JsonInt.h "JsonInt.h"
/// @brief 整数型を表すクラス
//////////////////////////////////////////////////////////////////////
class JsonInt :
  public JsonValue
{
public:

  /// @brief コンストラクタ
  JsonInt(
    int value ///< [in] 整数の値
  );

  /// @brief デストラクタ
  ~JsonInt();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数型の時 true を返す．
  bool
  is_int() const override;

  /// @brief 整数値を得る．
  ///
  /// - 整数型でない場合は無効
  int
  get_int() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 整数値の本体
  int mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class JsonFloat JsonFloat.h "JsonFloat.h"
/// @brief 浮動小数点型を表すクラス
//////////////////////////////////////////////////////////////////////
class JsonFloat :
  public JsonValue
{
public:

  /// @brief コンストラクタ
  JsonFloat(
    double value ///< [in] 浮動小数点の値
  );

  /// @brief デストラクタ
  ~JsonFloat();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 浮動小数点型の時 true を返す．
  bool
  is_float() const override;

  /// @brief 浮動小数点値を得る．
  ///
  /// - 浮動小数点型でない場合は無効
  double
  get_float() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 浮動小数点値の本体
  double mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class JsonBool JsonBool.h "JsonBool.h"
/// @brief ブール型を表すクラス
//////////////////////////////////////////////////////////////////////
class JsonBool :
  public JsonValue
{
public:

  /// @brief コンストラクタ
  JsonBool(
    bool value ///< [in] ブール値
  );

  /// @brief デストラクタ
  ~JsonBool();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ブール型の時 true を返す．
  bool
  is_bool() const override;

  /// @brief ブール値を得る．
  ///
  /// - ブール型でない場合は無効
  bool
  get_bool() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ブール値の本体
  bool mValue;

};

END_NAMESPACE_YM

#endif // JSONVALUE_H
