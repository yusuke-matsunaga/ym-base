
/// @file Json.cc
/// @brief Json の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Json.h"
#include "JsonValue.h"
#include "JsonParser.h"


BEGIN_NAMESPACE_YM

// @brief 空のコンストラクタ
Json::Json()
{
}

// @brief 値を指定したコンストラクタ
Json::Json(
  JsonValue* value
) : mPtr{shared_ptr<JsonValue>{value}}
{
}

// @brief デストラクタ
Json::~Json()
{
}

// @brief オブジェクト型の時 true を返す．
bool
Json::is_object() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_object();
}

// @brief 配列型の時 true を返す．
bool
Json::is_array() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_array();
}

// @brief 文字列型の時 true を返す．
bool
Json::is_string() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_string();
}

// @brief 数値型の時 true を返す．
bool
Json::is_number() const
{
  return is_int() || is_float();
}

// @brief 整数型の時 true を返す．
bool
Json::is_int() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_int();
}

// @brief 浮動小数点型の時 true を返す．
bool
Json::is_float() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_float();
}

// @brief ブール型の時 true を返す．
bool
Json::is_bool() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_bool();
}

// @brief null型の時 true を返す．
bool
Json::is_null() const
{
  return mPtr.get() == nullptr;
}

// @brief オブジェクトがキーを持つか調べる．
bool
Json::has_key(
  const string& key
) const
{
  if ( !is_object() ) {
    throw std::invalid_argument{"not an object-type"};
  }
  return mPtr->has_key(key);
}

// @brief キーのリストを返す．
vector<string>
Json::key_list() const
{
  if ( !is_object() ) {
    throw std::invalid_argument{"not an object-type"};
  }
  return mPtr->key_list();
}

// @brief キーと値のリストを返す．
vector<pair<string, Json>>
Json::item_list() const
{
  if ( !is_object() ) {
    throw std::invalid_argument{"not an object-type"};
  }
  return mPtr->item_list();
}

// @brief オブジェクトの要素を得る．
Json
Json::operator[](
  const string& key
) const
{
  if ( !is_object() ) {
    throw std::invalid_argument{"not an object-type"};
  }
  return mPtr->get_value(key);
}

// @brief 配列の要素数を得る．
SizeType
Json::array_size() const
{
  if ( !is_array() ) {
    throw std::invalid_argument{"not an array-type"};
  }
  return mPtr->array_size();
}

// @brief 配列の要素を得る．
Json
Json::operator[](
  SizeType pos
) const
{
  if ( !is_array() ) {
    throw std::invalid_argument{"not an array-type"};
  }
  return mPtr->get_value(pos);
}

// @brief 文字列を得る．
string
Json::get_string() const
{
  if ( !is_string() ) {
    throw std::invalid_argument{"not a string-type"};
  }
  return mPtr->get_string();
}

// @brief 整数値を得る．
int
Json::get_int() const
{
  if ( !is_int() ) {
    throw std::invalid_argument{"not an int-type"};
  }
  return mPtr->get_int();
}

// @brief 浮動小数点値を得る．
double
Json::get_float() const
{
  if ( !is_float() ) {
    throw std::invalid_argument{"not a float-type"};
  }
  return mPtr->get_float();
}

// @brief ブール値を得る．
bool
Json::get_bool() const
{
  if ( !is_bool() ) {
    throw std::invalid_argument{"not a bool-type"};
  }
  return mPtr->get_bool();
}

// @brief 読み込む．
Json
Json::read(
  istream& s,
  const FileInfo& file_info
)
{
  JsonParser parser;
  auto value = parser.read(s, file_info);
  return Json{value};
}

// @brief 内容を書き出す．
void
Json::write(
  ostream& s
) const
{
}

END_NAMESPACE_YM
