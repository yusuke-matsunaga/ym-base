
/// @file JsonObj.cc
/// @brief JsonObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "JsonObj.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_JSON

BEGIN_NONAMESPACE

string
escaped_string(
  const string& src_string
)
{
  bool has_dq = false;
  bool has_sq = false;
  for ( auto c: src_string ) {
    if ( c == '"' ) {
      has_dq = true;
    }
    if ( c == '\'' ) {
      has_sq = true;
    }
  }
  string ans;
  if ( !has_dq ) {
    ans += '"';
    ans += src_string;
    ans += '"';
  }
  else if ( !has_sq ) {
    ans += "'";
    ans += src_string;
    ans += "'";
  }
  else {
    // 文字列中の " をエスケープする．
    ans += '"';
    for ( auto c: src_string ) {
      if ( c == '"' ) {
	ans += '\\';
      }
      ans += c;
    }
    ans += '"';
  }
  return ans;
}

string
tab(
  SizeType n
)
{
  string ans;
  for ( SizeType i = 0; i < n; ++ i ) {
    ans += "    ";
  }
  return ans;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス JsonObj
//////////////////////////////////////////////////////////////////////

// @brief 文字列型の時 true を返す．
bool
JsonObj::is_string() const
{
  return false;
}

// @brief 整数型の時 true を返す．
bool
JsonObj::is_int() const
{
  return false;
}

// @brief 浮動小数点型の時 true を返す．
bool
JsonObj::is_float() const
{
  return false;
}

// @brief ブール型の時 true を返す．
bool
JsonObj::is_bool() const
{
  return false;
}

// @brief オブジェクト型の時 true を返す．
bool
JsonObj::is_object() const
{
  return false;
}

// @brief 配列型の時 true を返す．
bool
JsonObj::is_array() const
{
  return false;
}

// @brief 配列の要素数を得る．
SizeType
JsonObj::size() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief オブジェクトがキーを持つか調べる．
bool
JsonObj::has_key(
  const string& key ///< [in] キー
) const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief キーのリストを返す．
vector<string>
JsonObj::key_list() const
{
  ASSERT_NOT_REACHED;
  return {};
}

// @brief キーと値のリストを返す．
vector<pair<string, JsonValue>>
JsonObj::item_list() const
{
  ASSERT_NOT_REACHED;
  return {};
}

// @brief オブジェクトの要素を得る．
JsonValue
JsonObj::get_value(
  const string& key
) const
{
  ASSERT_NOT_REACHED;
  return JsonValue{};
}

// @brief 配列の要素を得る．
JsonValue
JsonObj::get_value(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return JsonValue{};
}

// @brief 文字列を得る．
string
JsonObj::get_string() const
{
  ASSERT_NOT_REACHED;
  return string{};
}

// @brief 整数値を得る．
int
JsonObj::get_int() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 浮動小数点値を得る．
double
JsonObj::get_float() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief ブール値を得る．
bool
JsonObj::get_bool() const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief JsonValue の内容を取り出す．
JsonObj*
JsonObj::obj_ptr(
  const JsonValue& value
)
{
  return value.mPtr.get();
}


//////////////////////////////////////////////////////////////////////
// クラス JsonDict
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonDict::JsonDict(
  const unordered_map<string, JsonValue>& dict
) : mDict{dict}
{
}

// @brief デストラクタ
JsonDict::~JsonDict()
{
}

// @brief オブジェクト型の時 true を返す．
bool
JsonDict::is_object() const
{
  return true;
}

// @brief 要素数を得る．
SizeType
JsonDict::size() const
{
  return mDict.size();
}

// @brief オブジェクトがキーを持つか調べる．
bool
JsonDict::has_key(
  const string& key
) const
{
  return mDict.count(key) > 0;
}

// @brief キーのリストを返す．
vector<string>
JsonDict::key_list() const
{
  vector<string> ans_list;
  for ( auto& p: mDict ) {
    ans_list.push_back(p.first);
  }
  // キーでソートしておく
  sort(ans_list.begin(), ans_list.end());
  return ans_list;
}

// @brief キーと値のリストを返す．
vector<pair<string, JsonValue>>
JsonDict::item_list() const
{
  vector<pair<string, JsonValue>> ans_list;
  for ( auto& p: mDict ) {
    ans_list.push_back(p);
  }
  // キーでソートしておく
  sort(ans_list.begin(), ans_list.end(),
       [](const pair<string, JsonValue>& a,
	  const pair<string, JsonValue>& b){
	 return a.first < b.first;
       });
  return ans_list;
}

// @brief オブジェクトの要素を得る．
JsonValue
JsonDict::get_value(
  const string& key
) const
{
  if ( mDict.count(key) == 0 ) {
    ostringstream buf;
    buf << key << ": invalid key";
    throw std::invalid_argument{buf.str()};
  }
  return mDict.at(key);
}

// @brief 内容を JSON 文字列に変換する．
string
JsonDict::to_json(
  int indent
) const
{
  bool first = true;
  int indent1 = indent;
  string ans;
  ans = "{";
  if ( indent >= 0 ) {
    ans += '\n';
    ++ indent1;
  }
  for ( auto& p: item_list() ) {
    if ( first ) {
      first = false;
    }
    else {
      ans += ",";
      if ( indent >= 0 ) {
	ans += '\n';
      }
    }
    auto key = p.first;
    auto value = p.second;
    if ( indent >= 0 ) {
      ans += tab(indent1);
    }
    ans += escaped_string(key);
    ans += ":";
    ans += obj_ptr(value)->to_json(indent1);
  }
  if ( indent >= 0 ) {
    ans += '\n';
    ans += tab(indent);
  }
  ans += "}";
  return ans;
}

// @brief 等価比較
bool
JsonDict::is_eq(
  const JsonObj* right
) const
{
  if ( right->is_object() ) {
    auto obj = reinterpret_cast<const JsonDict*>(right);
    return mDict == obj->mDict;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス JsonArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonArray::JsonArray(
  const vector<JsonValue>& array
) : mArray{array}
{
}

// @brief デストラクタ
JsonArray::~JsonArray()
{
}

// @brief 配列型の時 true を返す．
bool
JsonArray::is_array() const
{
  return true;
}

// @brief 要素数を得る．
SizeType
JsonArray::size() const
{
  return mArray.size();
}

// @brief 配列の要素を得る．
JsonValue
JsonArray::get_value(
  SizeType pos
) const
{
  if ( pos < 0 || size() <= pos ) {
    throw std::out_of_range("pos is out of range");
  }
  return mArray[pos];
}

// @brief 内容を JSON 文字列に変換する．
string
JsonArray::to_json(
  int indent
) const
{
  string ans;
  ans = "[";
  if ( indent >= 0 ) {
    ans += '\n';
  }
  bool first = true;
  int indent1 = indent;
  if ( indent >= 0 ) {
    ++ indent1;
  }
  for ( auto value: mArray ) {
    if ( first ) {
      first = false;
    }
    else {
      ans += ",";
      if ( indent >= 0 ) {
	ans += '\n';
      }
    }
    if ( indent >= 0 ) {
      ans += tab(indent1);
    }
    ans += obj_ptr(value)->to_json(indent1);
  }
  if ( indent >= 0 ) {
    ans += '\n';
    ans += tab(indent);
  }
  ans += "]";
  return ans;
}

// @brief 等価比較
bool
JsonArray::is_eq(
  const JsonObj* right
) const
{
  if ( right->is_array() ) {
    auto obj = reinterpret_cast<const JsonArray*>(right);
    return mArray == obj->mArray;
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス JsonString
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonString::JsonString(
  const string& value
) : mValue{value}
{
}

// @brief デストラクタ
JsonString::~JsonString()
{
}

// @brief 文字列型の時 true を返す．
bool
JsonString::is_string() const
{
  return true;
}

// @brief 文字列を得る．
string
JsonString::get_string() const
{
  return mValue;
}

// @brief 内容を JSON 文字列に変換する．
string
JsonString::to_json(
  int indent
) const
{
  return escaped_string(get_string());
}

// @brief 等価比較
bool
JsonString::is_eq(
  const JsonObj* right
) const
{
  if ( right->is_string() ) {
    return get_string() == right->get_string();
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス JsonInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonInt::JsonInt(
  int value
) : mValue{value}
{
}

// @brief デストラクタ
JsonInt::~JsonInt()
{
}

// @brief 整数型の時 true を返す．
bool
JsonInt::is_int() const
{
  return true;
}

// @brief 整数値を得る．
int
JsonInt::get_int() const
{
  return mValue;
}

// @brief 内容を JSON 文字列に変換する．
string
JsonInt::to_json(
  int indent
) const
{
  ostringstream buf;
  buf << get_int();
  return buf.str();
}

// @brief 等価比較
bool
JsonInt::is_eq(
  const JsonObj* right
) const
{
  if ( right->is_int() ) {
    return get_int() == right->get_int();
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス JsonFloat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonFloat::JsonFloat(
  double value
) : mValue{value}
{
}

/// @brief デストラクタ
JsonFloat::~JsonFloat()
{
}

// @brief 浮動小数点型の時 true を返す．
bool
JsonFloat::is_float() const
{
  return true;
}

// @brief 浮動小数点値を得る．
double
JsonFloat::get_float() const
{
  return mValue;
}

// @brief 内容を JSON 文字列に変換する．
string
JsonFloat::to_json(
  int indent
) const
{
  ostringstream buf;
  buf << get_float();
  return buf.str();
}

// @brief 等価比較
bool
JsonFloat::is_eq(
  const JsonObj* right
) const
{
  if ( right->is_float() ) {
    return get_float() == right->get_float();
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス JsonTrue
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonTrue::JsonTrue()
{
}

// @brief デストラクタ
JsonTrue::~JsonTrue()
{
}

// @brief ブール型の時 true を返す．
bool
JsonTrue::is_bool() const
{
  return true;
}

// @brief ブール値を得る．
bool
JsonTrue::get_bool() const
{
  return true;
}

// @brief 内容を JSON 文字列に変換する．
string
JsonTrue::to_json(
  int indent
) const
{
  return "true";
}

// @brief 等価比較
bool
JsonTrue::is_eq(
  const JsonObj* right
) const
{
  if ( right->is_bool() ) {
    return right->get_bool();
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス JsonFalse
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonFalse::JsonFalse()
{
}

// @brief デストラクタ
JsonFalse::~JsonFalse()
{
}

// @brief ブール型の時 true を返す．
bool
JsonFalse::is_bool() const
{
  return true;
}

// @brief ブール値を得る．
bool
JsonFalse::get_bool() const
{
  return false;
}

// @brief 内容を JSON 文字列に変換する．
string
JsonFalse::to_json(
  int indent
) const
{
  return "false";
}

// @brief 等価比較
bool
JsonFalse::is_eq(
  const JsonObj* right
) const
{
  if ( right->is_bool() ) {
    return !right->get_bool();
  }
  return false;
}

END_NAMESPACE_YM_JSON
