#ifndef JSONVALUE_H
#define JSONVALUE_H

/// @file JsonValue.h
/// @brief JsonValue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/json.h"


BEGIN_NAMESPACE_YM_JSON

class JsonObj;

//////////////////////////////////////////////////////////////////////
/// @class JsonValue JsonValue.h "ym/JsonValue.h"
/// @brief json オブジェクトを表すクラス
///
/// 以下のタイプを持つ．
/// - null
/// - 文字列 (string)
/// - 整数 (int)
/// - 浮動小数点数 (double)
/// - ブール (bool)
/// - 配列 (vector<JsonValue>)
/// - オブジェクト (unordered_map<string, JsonValue>)
///
/// 実装としては実体は JsonObj (の派生クラス) が表す．
/// JsonValue は JsonObj の shared_ptr<> のみを持つ．
/// JsonValue の公開メソッドはすべて const なので
/// 共有していても問題はない．
//////////////////////////////////////////////////////////////////////
class JsonValue
{
  friend class JsonObj;

public:

  /// @brief 空のコンストラクタ
  ///
  /// null 型の値となる．
  JsonValue();

  /// @brief 明示的に null 型のオブジェクトを作るクラスメソッド
  static
  JsonValue
  null()
  {
    return JsonValue{};
  }

  /// @brief 文字列型のコンストラクタ
  explicit
  JsonValue(
    const char* value ///< [in] 値
  );

  /// @brief 文字列型のコンストラクタ
  explicit
  JsonValue(
    const std::string& value ///< [in] 値
  );

  /// @brief 整数型のコンストラクタ
  explicit
  JsonValue(
    int value ///< [in] 値
  );

  /// @brief 浮動小数点型のコンストラクタ
  explicit
  JsonValue(
    double value ///< [in] 値
  );

  /// @brief ブール型のコンストラクタ
  explicit
  JsonValue(
    bool value ///< [in] 値
  );

  /// @brief 配列型のコンストラクタ
  explicit
  JsonValue(
    const std::vector<JsonValue>& value ///< [in] 値
  );

  /// @brief オブジェクト型のコンストラクタ
  explicit
  JsonValue(
    const std::unordered_map<std::string, JsonValue>& value ///< [in] 値
  );

  /// @brief 値を指定したコンストラクタ
  JsonValue(
    JsonObj* value ///< [in] 値
  );

  /// @brief デストラクタ
  ~JsonValue();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief null 型の時 true を返す．
  bool
  is_null() const;

  /// @brief 文字列型の時 true を返す．
  bool
  is_string() const;

  /// @brief 数値型の時 true を返す．
  ///
  /// is_int() か is_float() の時 true となる．
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

  /// @brief オブジェクト型の時 true を返す．
  bool
  is_object() const;

  /// @brief 配列型の時 true を返す．
  bool
  is_array() const;

  /// @brief 要素数を得る．
  ///
  /// - is_object() == false and is_array() == false
  ///   の時は std::invalid_argument 例外を送出する．
  SizeType
  size() const;

  /// @brief オブジェクトがキーを持つか調べる．
  ///
  /// - is_object() == false の時は std::invalid_argument 例外を送出する．
  bool
  has_key(
    const std::string& key ///< [in] キー
  ) const;

  /// @brief キーのリストを返す．
  ///
  /// - is_object() == false の時は std::invalid_argument 例外を送出する．
  std::vector<std::string>
  key_list() const;

  /// @brief キーと値のリストを返す．
  ///
  /// - is_object() == false の時は std::invalid_argument 例外を送出する．
  std::vector<std::pair<std::string, JsonValue>>
  item_list() const;

  /// @brief オブジェクトの要素を得る．
  ///
  /// - is_object() == false の時は std::invalid_argument 例外を送出する．
  /// - key に対応する値がない場合は std::invalid_argument 例外を送出する．
  JsonValue
  operator[](
    const std::string& key ///< [in] キー
  ) const
  {
    return at(key);
  }

  /// @brief operator[] の別名
  ///
  /// - is_object() == false の時は std::invalid_argument 例外を送出する．
  /// - key に対応する値がない場合は std::invalid_argument 例外を送出する．
  JsonValue
  at(
    const std::string& key ///< [in] キー
  ) const;

  /// @brief キーに対応する要素を取り出す．
  ///
  /// - is_object() == false の時は std::invalid_argument 例外を送出する．
  /// - key に対応する値がない場合には null を返す．
  JsonValue
  get(
    const std::string& key ///< [in] キー
  ) const;

  /// @brief 配列の要素を得る．
  ///
  /// - is_array() == false の時は std::invalid_argument 例外を送出する．
  /// - 配列のサイズ外のアクセスは std::out_of_range 例外を送出する．
  JsonValue
  operator[](
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < size() )
  ) const
  {
    return at(pos);
  }

  /// @brief operator[] の別名
  ///
  /// - is_array() == false の時は std::invalid_argument 例外を送出する．
  /// - 配列のサイズ外のアクセスは std::out_of_range 例外を送出する．
  JsonValue
  at(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < size() )
  ) const;

  /// @brief 文字列を得る．
  ///
  /// - is_string() == false の時は std::invalid_argument 例外を送出する．
  std::string
  get_string() const;

  /// @brief 整数値を得る．
  ///
  /// - is_int() == false の時は std::invalid_argument 例外を送出する．
  int
  get_int() const;

  /// @brief 浮動小数点値を得る．
  ///
  /// - is_float() == false の時は std::invalid_argument 例外を送出する．
  double
  get_float() const;

  /// @brief ブール値を得る．
  ///
  /// - is_bool() == false の時は std::invalid_argument 例外を送出する．
  bool
  get_bool() const;

  /// @brief 読み込む．
  /// @return 結果を格納したオブジェクトを返す．
  static
  JsonValue
  read(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief JSON文字列をパースする．
  /// @return 結果を格納したオブジェクトを返す．
  static
  JsonValue
  parse(
    const std::string& json_str ///< [in] JSON文字列
  );

  /// @brief 内容を JSON 文字列に変換する．
  std::string
  to_json(
    bool indent = false ///< [in] インデントフラグ
  ) const;

  /// @brief 内容を書き出す．
  void
  write(
    std::ostream& s,    ///< [in] 出力ストリーム
    bool indent = false ///< [in] インデントフラグ
  ) const
  {
    s << to_json(indent);
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const JsonValue& right ///< [in] オペランド
  ) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const JsonValue& right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列型かたどうかチェックする．
  void
  _check_string() const
  {
    if ( !is_string() ) {
      throw std::invalid_argument{"not a string-type"};
    }
  }

  /// @brief 整数型かどうかチェックする．
  void
  _check_int() const
  {
    if ( !is_int() ) {
      throw std::invalid_argument{"not an int-type"};
    }
  }

  /// @brief 浮動小数点型かどうかチェックする．
  void
  _check_float() const
  {
    if ( !is_float() ) {
      throw std::invalid_argument{"not an float-type"};
    }
  }

  /// @brief ブール型かどうかチェックする．
  void
  _check_bool() const
  {
    if ( !is_bool() ) {
      throw std::invalid_argument{"not a bool-type"};
    }
  }

  /// @brief オブジェクト型かどうかチェックする．
  void
  _check_object() const
  {
    if ( !is_object() ) {
      throw std::invalid_argument{"not an object-type"};
    }
  }

  /// @brief 配列型かどうかチェックする．
  void
  _check_array() const
  {
    if ( !is_array() ) {
      throw std::invalid_argument{"not an array-type"};
    }
  }

  /// @brief オブジェクト型か配列型かどうかチェックする．
  void
  _check_object_or_array() const
  {
    if ( !is_object() && !is_array() ) {
      throw std::invalid_argument{"not an object|array-type"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値の実体
  std::shared_ptr<JsonObj> mPtr;

};

/// @brief ストリーム入力演算子
std::istream&
operator>>(
  std::istream& s,    ///< [in] 入力ストリーム
  JsonValue& json_obj ///< [out] 結果を格納するオブジェクト
);

/// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,          ///< [in] 出力ストリーム
  const JsonValue& json_obj ///< [in] 体操のオブジェクト
);

END_NAMESPACE_YM_JSON

#endif // JSONVALUE_H
