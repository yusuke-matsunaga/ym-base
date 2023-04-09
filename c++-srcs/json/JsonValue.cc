
/// @file JsonValue.cc
/// @brief JsonValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "JsonValue.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス JsonValue
//////////////////////////////////////////////////////////////////////

// @brief オブジェクト型の時 true を返す．
bool
JsonValue::is_object() const
{
  return false;
}

// @brief 配列型の時 true を返す．
bool
JsonValue::is_array() const
{
  return false;
}

// @brief 文字列型の時 true を返す．
bool
JsonValue::is_string() const
{
  return false;
}

// @brief 整数型の時 true を返す．
bool
JsonValue::is_int() const
{
  return false;
}

// @brief 浮動小数点型の時 true を返す．
bool
JsonValue::is_float() const
{
  return false;
}

// @brief ブール型の時 true を返す．
bool
JsonValue::is_bool() const
{
  return false;
}

// @brief オブジェクトがキーを持つか調べる．
bool
JsonValue::has_key(
  const string& key ///< [in] キー
) const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief キーのリストを返す．
vector<string>
JsonValue::key_list() const
{
  ASSERT_NOT_REACHED;
  return {};
}

// @brief キーと値のリストを返す．
vector<pair<string, Json>>
JsonValue::item_list() const
{
  ASSERT_NOT_REACHED;
  return {};
}

// @brief オブジェクトの要素を得る．
Json
JsonValue::get_value(
  const string& key
) const
{
  ASSERT_NOT_REACHED;
  return Json{};
}

// @brief 配列の要素数を得る．
SizeType
JsonValue::array_size() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 配列の要素を得る．
Json
JsonValue::get_value(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return Json{};
}

// @brief 文字列を得る．
string
JsonValue::get_string() const
{
  ASSERT_NOT_REACHED;
  return string{};
}

// @brief 整数値を得る．
int
JsonValue::get_int() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 浮動小数点値を得る．
double
JsonValue::get_float() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief ブール値を得る．
bool
JsonValue::get_bool() const
{
  ASSERT_NOT_REACHED;
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス JsonObject
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonObject::JsonObject(
  const unordered_map<string, Json>& dict
) : mDict{dict}
{
}

// @brief デストラクタ
JsonObject::~JsonObject()
{
}

// @brief オブジェクト型の時 true を返す．
bool
JsonObject::is_object() const
{
  return true;
}

// @brief オブジェクトがキーを持つか調べる．
bool
JsonObject::has_key(
  const string& key
) const
{
  return mDict.count(key) > 0;
}

// @brief キーのリストを返す．
vector<string>
JsonObject::key_list() const
{
  vector<string> ans_list;
  for ( auto& p: mDict ) {
    ans_list.push_back(p.first);
  }
  sort(ans_list.begin(), ans_list.end());
  return ans_list;
}

// @brief キーと値のリストを返す．
vector<pair<string, Json>>
JsonObject::item_list() const
{
  vector<pair<string, Json>> ans_list;
  for ( auto& p: mDict ) {
    ans_list.push_back(p);
  }
  sort(ans_list.begin(), ans_list.end(),
       [](const pair<string, Json>& a,
	  const pair<string, Json>& b){
	 return a.first < b.first;
       });
  return ans_list;
}

// @brief オブジェクトの要素を得る．
Json
JsonObject::get_value(
  const string& key
) const
{
  if ( mDict.count(key) > 0 ) {
    return mDict.at(key);
  }
  return Json{};
}


//////////////////////////////////////////////////////////////////////
// クラス JsonArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
JsonArray::JsonArray(
  const vector<Json>& array
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
Json
JsonArray::get_value(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < array_size() );
  return mArray[pos];
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

END_NAMESPACE_YM
