
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

// @brief 配列の要素数を得る．
SizeType
JsonObj::array_size() const
{
  ASSERT_NOT_REACHED;
  return 0;
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

// @brief 要素を追加する．
void
JsonObj::emplace(
  const string& key,
  const JsonValue& value
)
{
  ASSERT_NOT_REACHED;
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
  if ( mDict.count(key) > 0 ) {
    return mDict.at(key);
  }
  return JsonValue{};
}

// @brief 要素を追加する．
void
JsonDict::emplace(
  const string& key,
  const JsonValue& value
)
{
  mDict.emplace(key, value);
}

// @brief 内容を書き出す．
void
JsonDict::write(
  ostream& s,
  int indent
) const
{
  bool first = true;
  int indent1 = indent;
  s << "{";
  if ( indent >= 0 ) {
    s << endl;
    ++ indent1;
  }
  for ( auto& p: item_list() ) {
    if ( first ) {
      first = false;
    }
    else {
      s << ",";
      if ( indent >= 0 ) {
	s << endl;
      }
      else {
	s << " ";
      }
    }
    auto key = p.first;
    auto value = p.second;
    if ( indent >= 0 ) {
      s << tab(indent1);
    }
    s << "\"" << key << "\": ";
    obj_ptr(value)->write(s, indent1);
  }
  if ( indent >= 0 ) {
    s << endl;
    s << tab(indent);
  }
  s << "}";
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

// @brief 配列の要素数を得る．
SizeType
JsonArray::array_size() const
{
  return mArray.size();
}

// @brief 配列の要素を得る．
JsonValue
JsonArray::get_value(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < array_size() );
  return mArray[pos];
}

// @brief 内容を書き出す．
void
JsonArray::write(
  ostream& s,
  int indent
) const
{
  s << "[";
  if ( indent >= 0 ) {
    s << endl;
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
      s << ",";
      if ( indent >= 0 ) {
	s << endl;
      }
    }
    if ( indent >= 0 ) {
      s << tab(indent1);
    }
    obj_ptr(value)->write(s, indent1);
  }
  if ( indent >= 0 ) {
    s << endl
      << tab(indent);
  }
  s << "]";
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

// @brief 内容を書き出す．
void
JsonString::write(
  ostream& s,
  int indent
) const
{
  s << get_string();
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

// @brief 内容を書き出す．
void
JsonInt::write(
  ostream& s,
  int indent
) const
{
  s << get_int();
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

// @brief 内容を書き出す．
void
JsonFloat::write(
  ostream& s,
  int indent
) const
{
  s << get_float();
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
// クラス JsonBool
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonBool::JsonBool(
  bool value
) : mValue{value}
{
}

// @brief デストラクタ
JsonBool::~JsonBool()
{
}

// @brief ブール型の時 true を返す．
bool
JsonBool::is_bool() const
{
  return true;
}

// @brief ブール値を得る．
bool
JsonBool::get_bool() const
{
  return mValue;
}

// @brief 内容を書き出す．
void
JsonBool::write(
  ostream& s,
  int indent
) const
{
  if ( is_bool() ) {
    s << "true";
  }
  else {
    s << "false";
  }
}

// @brief 等価比較
bool
JsonBool::is_eq(
  const JsonObj* right
) const
{
  if ( right->is_bool() ) {
    return get_bool() == right->get_bool();
  }
  return false;
}

END_NAMESPACE_YM_JSON
