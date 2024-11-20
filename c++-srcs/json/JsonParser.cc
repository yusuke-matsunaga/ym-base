
/// @file JsonParser.cc
/// @brief JsonParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "JsonParser.h"
#include "JsonScanner.h"
#include "JsonObj.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_JSON

// @brief コンストラクタ
JsonParser::JsonParser(
  istream& s
) : mScanner{s}
{
}

// @brief デストラクタ
JsonParser::~JsonParser()
{
}

// @brief 読み込む．
JsonValue
JsonParser::read()
{
  auto obj = read_value();
  auto tk = mScanner.read_token();
  if ( tk != JsonToken::End ) {
    error("syntax error");
  }
  return JsonValue{obj};
}

// @brief 値を読み込む．
JsonObj*
JsonParser::read_value()
{
  auto tk = mScanner.read_token();
  switch ( tk ) {
  case JsonToken::String:
    return new JsonString{mScanner.cur_string()};

  case JsonToken::Int:
    return new JsonInt{mScanner.cur_int()};

  case JsonToken::Float:
    return new JsonFloat{mScanner.cur_float()};

  case JsonToken::LCB:
    return read_object();

  case JsonToken::LBK:
    return read_array();

  case JsonToken::True:
    return new JsonTrue;

  case JsonToken::False:
    return new JsonFalse;

  case JsonToken::Null:
    return nullptr;

  default:
    // シンタックスエラー
    {
      ostringstream buf;
      buf << "'" << mScanner.cur_string() << "': unexpected token";
      error(buf.str());
    }
  }

  return nullptr;
}

// @brief オブジェクトを読み込む．
JsonObj*
JsonParser::read_object()
{
  unordered_map<string, JsonValue> dict;
  auto tk = mScanner.read_token();
  if ( tk == JsonToken::RCB ) {
    // 空のオブジェクト
    auto obj = new JsonDict{dict};
    return obj;
  }
  mScanner.unget_token(tk);
  for ( ; ; ) {
    auto tk = mScanner.read_token();
    if ( tk == JsonToken::String ) {
      auto key = mScanner.cur_string();
      tk = mScanner.read_token();
      if ( tk != JsonToken::Colon ) {
	// ':' ではなかった．
	error("':' is expected");
      }
      auto value = read_value();
      dict.emplace(key, JsonValue{value});
    }
    else {
      // シンタックスエラー
      ostringstream buf;
      buf << mScanner.cur_string()
	  << ": illegal token, string is expected";
      error(buf.str());
    }

    tk = mScanner.read_token();
    if ( tk == JsonToken::RCB ) {
      break;
    }

    if ( tk != JsonToken::Comma ) {
      // シンタックスエラー
      ostringstream buf;
      buf << mScanner.cur_string()
	  << ": illegal token, ',' is expected";
      error(buf.str());
    }
  }
  auto obj = new JsonDict{dict};
  return obj;
}

// @brief 配列を読み込む．
JsonObj*
JsonParser::read_array()
{
  auto tk = mScanner.read_token();
  if ( tk == JsonToken::RBK ) {
    // 空の配列
    auto obj = new JsonArray{{}};
    return obj;
  }
  if ( tk == JsonToken::End ) {
    // シンタックスエラー
    error("unexpected EOF");
  }

  mScanner.unget_token(tk);
  vector<JsonValue> array;
  for ( ; ; ) {
    auto value = read_value();
    array.push_back(JsonValue{value});
    tk = mScanner.read_token();
    if ( tk == JsonToken::RBK ) {
      break;
    }
    if ( tk != JsonToken::Comma ) {
      // シンタックスエラー
      ostringstream buf;
      buf << mScanner.cur_string()
	  << ": illegal token, ',' is expected";
      error(buf.str());
    }
  }
  auto obj = new JsonArray{array};
  return obj;
}

// @brief エラーを出力する．
void
JsonParser::error(
  const string& msg
)
{
  ostringstream buf;
  buf << mScanner.cur_loc()
      << ": " << msg;
  throw std::invalid_argument(buf.str());
}

END_NAMESPACE_YM_JSON
