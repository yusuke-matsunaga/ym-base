
/// @file JsonValue.cc
/// @brief JsonValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/JsonValue.h"
#include "JsonObj.h"
#include "JsonParser.h"


BEGIN_NAMESPACE_YM_JSON

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
  const std::string& value
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

BEGIN_NONAMESPACE

inline
JsonObj*
new_JsonBool(
  bool value
)
{
  if ( value ) {
    return new JsonTrue;
  }
  else {
    return new JsonFalse;
  }
}

END_NONAMESPACE

// @brief ブール型のコンストラクタ
JsonValue::JsonValue(
  bool value
) : mPtr{new_JsonBool(value)}
{
}

// @brief 配列型のコンストラクタ
JsonValue::JsonValue(
  const std::vector<JsonValue>& value
) : mPtr{new JsonArray{value}}
{
}

// @brief オブジェクト型のコンストラクタ
JsonValue::JsonValue(
  const std::unordered_map<std::string, JsonValue>& value
) : mPtr{new JsonDict{value}}
{
}

// @brief 値を指定したコンストラクタ
JsonValue::JsonValue(
  JsonObj* value
) : mPtr{std::shared_ptr<JsonObj>{value}}
{
}

// @brief デストラクタ
JsonValue::~JsonValue()
{
}

// @brief null型の時 true を返す．
bool
JsonValue::is_null() const
{
  return mPtr.get() == nullptr;
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
  if ( is_null() ) {
    return false;
  }
  return mPtr->is_int() || mPtr->is_float();
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

// @brief 配列の要素数を得る．
SizeType
JsonValue::size() const
{
  _check_object_or_array();
  return mPtr->size();
}

// @brief オブジェクトがキーを持つか調べる．
bool
JsonValue::has_key(
  const std::string& key
) const
{
  _check_object();
  return mPtr->has_key(key);
}

// @brief キーのリストを返す．
std::vector<std::string>
JsonValue::key_list() const
{
  _check_object();
  return mPtr->key_list();
}

// @brief キーと値のリストを返す．
std::vector<std::pair<std::string, JsonValue>>
JsonValue::item_list() const
{
  _check_object();
  return mPtr->item_list();
}

// @brief オブジェクトの要素を得る．
JsonValue
JsonValue::at(
  const std::string& key
) const
{
  _check_object();
  return mPtr->get_value(key);
}

// @brief キーに対応する要素を取り出す．
JsonValue
JsonValue::get(
  const std::string& key
) const
{
  _check_object();
  if ( has_key(key) ) {
    return at(key);
  }
  return JsonValue::null();
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
std::string
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

// @brief 読み込む．
JsonValue
JsonValue::read(
  const std::string& filename
)
{
  std::ifstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": No such file";
    throw std::invalid_argument{buf.str()};
  }

  JsonParser parser{s};
  return parser.read();
}

// @brief JSON文字列をパースする．
JsonValue
JsonValue::parse(
  const std::string& json_str
)
{
  std::istringstream s{json_str};
  JsonParser parser{s};
  return parser.read();
}

// @brief 内容を JSON 文字列に変換する．
std::string
JsonValue::to_json(
  bool indent
) const
{
  if ( is_null() ) {
    return std::string{"null"};
  }
  int indent_val = -1;
  if ( indent ) {
    indent_val = 0;
  }
  auto ans = mPtr->to_json(indent_val);
  if ( indent ) {
    ans += '\n';
  }
  return ans;
}

// @brief 等価比較演算子
bool
JsonValue::operator==(
  const JsonValue& right
) const
{
  if ( mPtr != nullptr ) {
    if (  right.mPtr != nullptr ) {
      return mPtr->is_eq(right.mPtr.get());
    }
    return false;
  }
  if ( right.mPtr == nullptr ) {
    return true;
  }
  return false;
}

// @brief ストリーム入力演算子
std::istream&
operator>>(
  std::istream& s,
  JsonValue& json_obj
)
{
  JsonParser parser{s};
  json_obj = parser.read();
  return s;
}

// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  const JsonValue& json_obj
)
{
  json_obj.write(s);
  return s;
}

END_NAMESPACE_YM_JSON
