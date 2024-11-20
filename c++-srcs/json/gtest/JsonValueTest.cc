
/// @file JsonValueTest.cc
/// @brief JsonValueTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM

TEST(JsonValueTest, null)
{
  JsonValue json_obj;

  EXPECT_TRUE( json_obj.is_null() );
  EXPECT_FALSE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );
  // for Object-type
  EXPECT_THROW( {json_obj.has_key("abc");}, std::invalid_argument );
  EXPECT_THROW( {json_obj.key_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj.item_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  // for Array-type
  EXPECT_THROW( {json_obj.size();}, std::invalid_argument );
  EXPECT_THROW( {json_obj[0];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at(0);}, std::invalid_argument );
  // for string-type
  EXPECT_THROW( {json_obj.get_string();}, std::invalid_argument );
  // for integer-type
  EXPECT_THROW( {json_obj.get_int();}, std::invalid_argument );
  // for float-type
  EXPECT_THROW( {json_obj.get_float();}, std::invalid_argument );
  // for Boolean-type
  EXPECT_THROW( {json_obj.get_bool();}, std::invalid_argument );
  // to_json()
  EXPECT_EQ( "null", json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );
  // eq
  JsonValue another_obj;
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue int_obj{0};
  EXPECT_TRUE( int_obj != json_obj );
}

TEST(JsonValueTest, null2)
{
  auto json_obj = JsonValue::null();

  EXPECT_TRUE( json_obj.is_null() );
  EXPECT_FALSE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );
  // for Object-type
  EXPECT_THROW( {json_obj.has_key("abc");}, std::invalid_argument );
  EXPECT_THROW( {json_obj.key_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj.item_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  // for Array-type
  EXPECT_THROW( {json_obj.size();}, std::invalid_argument );
  EXPECT_THROW( {json_obj[0];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at(0);}, std::invalid_argument );
  // for string-type
  EXPECT_THROW( {json_obj.get_string();}, std::invalid_argument );
  // for integer-type
  EXPECT_THROW( {json_obj.get_int();}, std::invalid_argument );
  // for float-type
  EXPECT_THROW( {json_obj.get_float();}, std::invalid_argument );
  // for Boolean-type
  EXPECT_THROW( {json_obj.get_bool();}, std::invalid_argument );
  // to_json()
  EXPECT_EQ( "null", json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );
  // eq
  JsonValue another_obj;
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue int_obj{0};
  EXPECT_TRUE( int_obj != json_obj );
}

TEST(JsonValueTest, string1)
{
  const char* value = "abcde";
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_TRUE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );
  // for Object-type
  EXPECT_THROW( {json_obj.has_key("abc");}, std::invalid_argument );
  EXPECT_THROW( {json_obj.key_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj.item_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  // for Array-type
  EXPECT_THROW( {json_obj.size();}, std::invalid_argument );
  EXPECT_THROW( {json_obj[0];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at(0);}, std::invalid_argument );
  // for string-type
  EXPECT_EQ( value, json_obj.get_string() );
  // for integer-type
  EXPECT_THROW( {json_obj.get_int();}, std::invalid_argument );
  // for float-type
  EXPECT_THROW( {json_obj.get_float();}, std::invalid_argument );
  // for Boolean-type
  EXPECT_THROW( {json_obj.get_bool();}, std::invalid_argument );
  // to_json()
  ostringstream buf;
  buf << '"' << value << '"';
  EXPECT_EQ( buf.str(), json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );
  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue int_obj{0};
  EXPECT_TRUE( int_obj != json_obj );
}

TEST(JsonValueTest, string2)
{
  string value = "abcde";
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_TRUE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );
  // for Object-type
  EXPECT_THROW( {json_obj.has_key("abc");}, std::invalid_argument );
  EXPECT_THROW( {json_obj.key_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj.item_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  // for Array-type
  EXPECT_THROW( {json_obj.size();}, std::invalid_argument );
  EXPECT_THROW( {json_obj[0];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at(0);}, std::invalid_argument );
  // for string-type
  EXPECT_EQ( value, json_obj.get_string() );
  // for integer-type
  EXPECT_THROW( {json_obj.get_int();}, std::invalid_argument );
  // for float-type
  EXPECT_THROW( {json_obj.get_float();}, std::invalid_argument );
  // for Boolean-type
  EXPECT_THROW( {json_obj.get_bool();}, std::invalid_argument );
  // to_json()
  ostringstream buf;
  buf << '"' << value << '"';
  EXPECT_EQ( buf.str(), json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );
  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue int_obj{0};
  EXPECT_TRUE( int_obj != json_obj );
}

TEST(JsonValueTest, string_dq)
{
  string value = "\"abcde\"";
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_TRUE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );

  EXPECT_EQ( value, json_obj.get_string() );
  // double-quote を含む文字列を表す json 文字列は
  // single-quote を用いる．
  EXPECT_EQ( "'\"abcde\"'", json_obj.to_json() );
}

TEST(JsonValueTest, string_sq)
{
  string value = "'abcde'";
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_TRUE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );

  EXPECT_EQ( value, json_obj.get_string() );
  // single-quote を含む文字列を表す json 文字列は
  // double-quote を用いる．
  EXPECT_EQ( "\"'abcde'\"", json_obj.to_json() );
}

TEST(JsonValueTest, string_dqsq)
{
  string value = "\"'abcde'\"";
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_TRUE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );

  EXPECT_EQ( value, json_obj.get_string() );
  // single-quote と double-quote を含む文字列を表す json 文字列は
  // double-quote を用い，double-quote をエスケープする．
  EXPECT_EQ( R"("\"'abcde'\"")", json_obj.to_json() );
}

TEST(JsonValueTest, int)
{
  int value = 99;
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_FALSE( json_obj.is_string() );
  EXPECT_TRUE( json_obj.is_number() );
  EXPECT_TRUE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );
  // for Object-type
  EXPECT_THROW( {json_obj.has_key("abc");}, std::invalid_argument );
  EXPECT_THROW( {json_obj.key_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj.item_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  // for Array-type
  EXPECT_THROW( {json_obj.size();}, std::invalid_argument );
  EXPECT_THROW( {json_obj[0];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at(0);}, std::invalid_argument );
  // for string-type
  EXPECT_THROW( {json_obj.get_string();}, std::invalid_argument );
  // for integer-type
  EXPECT_EQ( value, json_obj.get_int() );
  // for float-type
  EXPECT_THROW( {json_obj.get_float();}, std::invalid_argument );
  // for Boolean-type
  EXPECT_THROW( {json_obj.get_bool();}, std::invalid_argument );
  // to_json()
  ostringstream buf;
  buf << value;
  EXPECT_EQ( buf.str(), json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );
  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST(JsonValueTest, float)
{
  double value = 1.2345;
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_FALSE( json_obj.is_string() );
  EXPECT_TRUE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_TRUE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );
  // for Object-type
  EXPECT_THROW( {json_obj.has_key("abc");}, std::invalid_argument );
  EXPECT_THROW( {json_obj.key_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj.item_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  // for Array-type
  EXPECT_THROW( {json_obj.size();}, std::invalid_argument );
  EXPECT_THROW( {json_obj[0];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at(0);}, std::invalid_argument );
  // for string-type
  EXPECT_THROW( {json_obj.get_string();}, std::invalid_argument );
  // for integer-type
  EXPECT_THROW( {json_obj.get_int();}, std::invalid_argument );
  // for float-type
  EXPECT_EQ( value, json_obj.get_float() );
  // for Boolean-type
  EXPECT_THROW( {json_obj.get_bool();}, std::invalid_argument );
  // to_json()
  ostringstream buf;
  buf << value;
  EXPECT_EQ( buf.str(), json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );
  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST(JsonValueTest, bool_true)
{
  bool value = true;
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_FALSE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_TRUE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );
  // for Object-type
  EXPECT_THROW( {json_obj.has_key("abc");}, std::invalid_argument );
  EXPECT_THROW( {json_obj.key_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj.item_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  // for Array-type
  EXPECT_THROW( {json_obj.size();}, std::invalid_argument );
  EXPECT_THROW( {json_obj[0];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at(0);}, std::invalid_argument );
  // for string-type
  EXPECT_THROW( {json_obj.get_string();}, std::invalid_argument );
  // for integer-type
  EXPECT_THROW( {json_obj.get_int();}, std::invalid_argument );
  // for float-type
  EXPECT_THROW( {json_obj.get_float();}, std::invalid_argument );
  // for Boolean-type
  EXPECT_EQ( value, json_obj.get_bool() );
  // to_json()
  EXPECT_EQ( "true", json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );
  // eq
  JsonValue another_obj{true};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST(JsonValueTest, bool_false)
{
  bool value = false;
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_FALSE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_TRUE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );
  // for Object-type
  EXPECT_THROW( {json_obj.has_key("abc");}, std::invalid_argument );
  EXPECT_THROW( {json_obj.key_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj.item_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  // for Array-type
  EXPECT_THROW( {json_obj.size();}, std::invalid_argument );
  EXPECT_THROW( {json_obj[0];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at(0);}, std::invalid_argument );
  // for string-type
  EXPECT_THROW( {json_obj.get_string();}, std::invalid_argument );
  // for integer-type
  EXPECT_THROW( {json_obj.get_int();}, std::invalid_argument );
  // for float-type
  EXPECT_THROW( {json_obj.get_float();}, std::invalid_argument );
  // for Boolean-type
  EXPECT_EQ( value, json_obj.get_bool() );
  // to_json()
  EXPECT_EQ( "false", json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( copy_obj, json_obj );
  // eq
  JsonValue another_obj{false};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST(JsonValueTest, array1)
{
  string value1 = "xyz";
  JsonValue json1{value1};

  int value2 = 2;
  JsonValue json2{value2};

  float value3 = 0.99;
  JsonValue json3{value3};

  vector<JsonValue> value{json1, json2, json3};
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_FALSE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_FALSE( json_obj.is_object() );
  EXPECT_TRUE( json_obj.is_array() );
  // for Object-type
  EXPECT_THROW( {json_obj.has_key("abc");}, std::invalid_argument );
  EXPECT_THROW( {json_obj.key_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj.item_list();}, std::invalid_argument );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  // for Array-type
  EXPECT_EQ( value.size(), json_obj.size() );
  EXPECT_EQ( json1, json_obj[0] );
  EXPECT_EQ( json2, json_obj[1] );
  EXPECT_EQ( json3, json_obj[2] );
  EXPECT_EQ( json1, json_obj.at(0) );
  EXPECT_EQ( json2, json_obj.at(1) );
  EXPECT_EQ( json3, json_obj.at(2) );
  EXPECT_THROW( {json_obj[3];}, std::out_of_range );
  EXPECT_THROW( {json_obj.at(4);}, std::out_of_range );
  // for string-type
  EXPECT_THROW( {json_obj.get_string();}, std::invalid_argument );
  // for integer-type
  EXPECT_THROW( {json_obj.get_int();}, std::invalid_argument );
  // for float-type
  EXPECT_THROW( {json_obj.get_float();}, std::invalid_argument );
  // for Boolean-type
  EXPECT_THROW( {json_obj.get_bool();}, std::invalid_argument );
  // to_json()
  const char* exp_str = "[\"xyz\",2,0.99]";
  EXPECT_EQ( exp_str, json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( exp_str, copy_obj.to_json() );
  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

TEST(JsonValueTest, object1)
{
  string value1 = "xyz";
  JsonValue json1{value1};

  int value2 = 2;
  JsonValue json2{value2};

  float value3 = 0.99;
  JsonValue json3{value3};

  unordered_map<string, JsonValue> value{
    {"key1", json1},
    {"key2", json2},
    {"key3", json3}
  };
  JsonValue json_obj{value};

  EXPECT_FALSE( json_obj.is_null() );
  EXPECT_FALSE( json_obj.is_string() );
  EXPECT_FALSE( json_obj.is_number() );
  EXPECT_FALSE( json_obj.is_int() );
  EXPECT_FALSE( json_obj.is_float() );
  EXPECT_FALSE( json_obj.is_bool() );
  EXPECT_TRUE( json_obj.is_object() );
  EXPECT_FALSE( json_obj.is_array() );
  // for Object-type
  EXPECT_TRUE( json_obj.has_key("key1") );
  EXPECT_EQ( json1, json_obj["key1"] );
  EXPECT_EQ( json1, json_obj.at("key1") );
  EXPECT_TRUE( json_obj.has_key("key2") );
  EXPECT_EQ( json2, json_obj["key2"] );
  EXPECT_EQ( json2, json_obj.at("key2") );
  EXPECT_TRUE( json_obj.has_key("key3") );
  EXPECT_EQ( json3, json_obj["key3"] );
  EXPECT_EQ( json3, json_obj.at("key3") );
  EXPECT_FALSE( json_obj.has_key("abc") );
  EXPECT_THROW( {json_obj["abc"];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at("abc");}, std::invalid_argument );
  vector<string> exp_key_list{"key1", "key2", "key3"};
  EXPECT_EQ( exp_key_list, json_obj.key_list() );
  vector<pair<string, JsonValue>> exp_item_list{
    {"key1", json1},
    {"key2", json2},
    {"key3", json3}
  };
  EXPECT_EQ( exp_item_list, json_obj.item_list() );
  // for Array-type
  EXPECT_EQ( value.size(), json_obj.size() );
  EXPECT_THROW( {json_obj[0];}, std::invalid_argument );
  EXPECT_THROW( {json_obj.at(0);}, std::invalid_argument );
  // for string-type
  EXPECT_THROW( {json_obj.get_string();}, std::invalid_argument );
  // for integer-type
  EXPECT_THROW( {json_obj.get_int();}, std::invalid_argument );
  // for float-type
  EXPECT_THROW( {json_obj.get_float();}, std::invalid_argument );
  // for Boolean-type
  EXPECT_THROW( {json_obj.get_bool();}, std::invalid_argument );
  // to_json()
  const char* exp_str = "{\"key1\":\"xyz\",\"key2\":2,\"key3\":0.99}";
  EXPECT_EQ( exp_str, json_obj.to_json() );
  // parse()
  auto copy_obj = JsonValue::parse(json_obj.to_json());
  EXPECT_EQ( exp_str, copy_obj.to_json() );
  // eq
  JsonValue another_obj{value};
  EXPECT_TRUE( another_obj == json_obj );
  JsonValue str_obj{"abc"};
  EXPECT_TRUE( str_obj != json_obj );
}

END_NAMESPACE_YM
