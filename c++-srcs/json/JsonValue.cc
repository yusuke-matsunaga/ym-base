
/// @file JsonValue.cc
/// @brief JsonValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/JsonValue.h"
#include "JsonObj.h"
#include "JsonParser.h"


BEGIN_NAMESPACE_YM

// @brief 空のコンストラクタ
JsonValue::JsonValue()
{
}

// @brief 文字列型のコンストラクタ
JsonValue::JsonValue(
  const char* value
) : mPtr{new JsonString{value}}
{
}

// @brief 文字列型のコンストラクタ
JsonValue::JsonValue(
  const string& value
) : mPtr{new JsonString{value}}
{
}

// @brief 整数型のコンストラクタ
JsonValue::JsonValue(
  int value
) : mPtr{new JsonInt{value}}
{
}

// @brief 浮動小数点型のコンストラクタ
JsonValue::JsonValue(
  double value
) : mPtr{new JsonFloat{value}}
{
}

// @brief ブール型のコンストラクタ
JsonValue::JsonValue(
  bool value
) : mPtr{new JsonBool{value}}
{
}

// @brief 配列型のコンストラクタ
JsonValue::JsonValue(
  const vector<JsonValue>& value
) : mPtr{new JsonArray{value}}
{
}

// @brief オブジェクト型のコンストラクタ
JsonValue::JsonValue(
  const unordered_map<string, JsonValue>& value
) : mPtr{new JsonDict{value}}
{
}

// @brief 値を指定したコンストラクタ
JsonValue::JsonValue(
  JsonObj* value
) : mPtr{shared_ptr<JsonObj>{value}}
{
}

// @brief デストラクタ
JsonValue::~JsonValue()
{
}

// @brief オブジェクト型の時 true を返す．
bool
JsonValue::is_object() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_object();
}

// @brief 配列型の時 true を返す．
bool
JsonValue::is_array() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_array();
}

// @brief 文字列型の時 true を返す．
bool
JsonValue::is_string() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_string();
}

// @brief 数値型の時 true を返す．
bool
JsonValue::is_number() const
{
  return is_int() || is_float();
}

// @brief 整数型の時 true を返す．
bool
JsonValue::is_int() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_int();
}

// @brief 浮動小数点型の時 true を返す．
bool
JsonValue::is_float() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_float();
}

// @brief ブール型の時 true を返す．
bool
JsonValue::is_bool() const
{
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_bool();
}

// @brief null型の時 true を返す．
bool
JsonValue::is_null() const
{
  return mPtr.get() == nullptr;
}

// @brief オブジェクトがキーを持つか調べる．
bool
JsonValue::has_key(
  const string& key
) const
{
  _check_object();
  return mPtr->has_key(key);
}

// @brief キーのリストを返す．
vector<string>
JsonValue::key_list() const
{
  _check_object();
  return mPtr->key_list();
}

// @brief キーと値のリストを返す．
vector<pair<string, JsonValue>>
JsonValue::item_list() const
{
  _check_object();
  return mPtr->item_list();
}

// @brief オブジェクトの要素を得る．
JsonValue
JsonValue::at(
  const string& key
) const
{
  _check_object();
  return mPtr->get_value(key);
}

// @brief 配列の要素数を得る．
SizeType
JsonValue::array_size() const
{
  _check_array();
  return mPtr->array_size();
}

// @brief 配列の要素を得る．
JsonValue
JsonValue::at(
  SizeType pos
) const
{
  _check_array();
  return mPtr->get_value(pos);
}

// @brief 文字列を得る．
string
JsonValue::get_string() const
{
  _check_string();
  return mPtr->get_string();
}

// @brief 整数値を得る．
int
JsonValue::get_int() const
{
  _check_int();
  return mPtr->get_int();
}

// @brief 浮動小数点値を得る．
double
JsonValue::get_float() const
{
  _check_float();
  return mPtr->get_float();
}

// @brief ブール値を得る．
bool
JsonValue::get_bool() const
{
  _check_bool();
  return mPtr->get_bool();
}

// @brief オブジェクト型の要素を追加する．
void
JsonValue::emplace(
  const string& key,
  const JsonValue& value
)
{
  _check_object();
  mPtr->emplace(key, value);
}

// @brief 読み込む．
JsonValue
JsonValue::read(
  const string& filename
)
{
  ifstream s{filename};
  if ( !s ) {
    ostringstream buf;
    buf << filename << ": No such file";
    throw std::invalid_argument{buf.str()};
  }

  JsonParser parser;
  auto obj = parser.read(s, FileInfo{filename});
  return JsonValue{obj};
}

// @brief JSON文字列をパースする．
JsonValue
JsonValue::parse(
  const string& json_str
)
{
  istringstream s{json_str};
  JsonParser parser;
  auto obj = parser.read(s, FileInfo{});
  return JsonValue{obj};
}

// @brief 内容を書き出す．
void
JsonValue::write(
  ostream& s,
  bool indent
) const
{
  int indent_val = -1;
  if ( indent ) {
    indent_val = 0;
  }
  mPtr->write(s, indent_val);
  if ( indent ) {
    s << endl;
  }
}

// @brief 等価比較演算子
bool
JsonValue::operator==(
  const JsonValue& right
) const
{
  return mPtr->is_eq(right.mPtr.get());
}

END_NAMESPACE_YM
