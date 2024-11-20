
/// @file JsonObjTest.cc
/// @brief JsonObj のファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "JsonObj.h"


BEGIN_NAMESPACE_YM_JSON

TEST(JsonObjTest, string1)
{
  const char* value = "abcde";
  auto json_obj = new JsonString{value};

  EXPECT_TRUE( json_obj->is_string() );
  EXPECT_FALSE( json_obj->is_int() );
  EXPECT_FALSE( json_obj->is_float() );
  EXPECT_FALSE( json_obj->is_bool() );
  EXPECT_FALSE( json_obj->is_object() );
  EXPECT_FALSE( json_obj->is_array() );

  EXPECT_EQ( value, json_obj->get_string() );
}

TEST(JsonObjTest, string2)
{
  string value = "abcde";
  auto json_obj = new JsonString{value};

  EXPECT_TRUE( json_obj->is_string() );
  EXPECT_FALSE( json_obj->is_int() );
  EXPECT_FALSE( json_obj->is_float() );
  EXPECT_FALSE( json_obj->is_bool() );
  EXPECT_FALSE( json_obj->is_object() );
  EXPECT_FALSE( json_obj->is_array() );

  EXPECT_EQ( value, json_obj->get_string() );
}

TEST(JsonObjTest, int)
{
  int value = 99;
  auto json_obj = new JsonInt{value};

  EXPECT_FALSE( json_obj->is_string() );
  EXPECT_TRUE( json_obj->is_int() );
  EXPECT_FALSE( json_obj->is_float() );
  EXPECT_FALSE( json_obj->is_bool() );
  EXPECT_FALSE( json_obj->is_object() );
  EXPECT_FALSE( json_obj->is_array() );

  EXPECT_EQ( value, json_obj->get_int() );
}

TEST(JsonObjTest, float)
{
  double value = 1.2345;
  auto json_obj = new JsonFloat{value};

  EXPECT_FALSE( json_obj->is_string() );
  EXPECT_FALSE( json_obj->is_int() );
  EXPECT_TRUE( json_obj->is_float() );
  EXPECT_FALSE( json_obj->is_bool() );
  EXPECT_FALSE( json_obj->is_object() );
  EXPECT_FALSE( json_obj->is_array() );

  EXPECT_EQ( value, json_obj->get_float() );
}

TEST(JsonObjTest, bool_true)
{
  auto json_obj = new JsonTrue;

  EXPECT_FALSE( json_obj->is_string() );
  EXPECT_FALSE( json_obj->is_int() );
  EXPECT_FALSE( json_obj->is_float() );
  EXPECT_TRUE( json_obj->is_bool() );
  EXPECT_FALSE( json_obj->is_object() );
  EXPECT_FALSE( json_obj->is_array() );

  EXPECT_EQ( true, json_obj->get_bool() );
}

TEST(JsonObjTest, bool_false)
{
  auto json_obj = new JsonFalse;

  EXPECT_FALSE( json_obj->is_string() );
  EXPECT_FALSE( json_obj->is_int() );
  EXPECT_FALSE( json_obj->is_float() );
  EXPECT_TRUE( json_obj->is_bool() );
  EXPECT_FALSE( json_obj->is_object() );
  EXPECT_FALSE( json_obj->is_array() );

  EXPECT_EQ( false, json_obj->get_bool() );
}

TEST(JsonObjTest, array1)
{
  string value1 = "xyz";
  JsonValue json1{value1};

  int value2 = 2;
  JsonValue json2{value2};

  float value3 = 0.99;
  JsonValue json3{value3};

  vector<JsonValue> value{json1, json2, json3};
  auto json_obj = new JsonArray{value};

  EXPECT_FALSE( json_obj->is_string() );
  EXPECT_FALSE( json_obj->is_int() );
  EXPECT_FALSE( json_obj->is_float() );
  EXPECT_FALSE( json_obj->is_bool() );
  EXPECT_FALSE( json_obj->is_object() );
  EXPECT_TRUE( json_obj->is_array() );

  EXPECT_EQ( value.size(), json_obj->size() );
  EXPECT_EQ( json1, json_obj->get_value(0) );
  EXPECT_EQ( json2, json_obj->get_value(1) );
  EXPECT_EQ( json3, json_obj->get_value(2) );
}

TEST(JsonObjTest, object1)
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
  auto json_obj = new JsonDict{value};

  EXPECT_FALSE( json_obj->is_string() );
  EXPECT_FALSE( json_obj->is_int() );
  EXPECT_FALSE( json_obj->is_float() );
  EXPECT_FALSE( json_obj->is_bool() );
  EXPECT_TRUE( json_obj->is_object() );
  EXPECT_FALSE( json_obj->is_array() );

  //EXPECT_EQ( value.size(), json_obj->size() );
  EXPECT_TRUE( json_obj->has_key("key1") );
  EXPECT_EQ( json1, json_obj->get_value("key1") );
  EXPECT_TRUE( json_obj->has_key("key2") );
  EXPECT_EQ( json2, json_obj->get_value("key2") );
  EXPECT_TRUE( json_obj->has_key("key3") );
  EXPECT_EQ( json3, json_obj->get_value("key3") );
}

END_NAMESPACE_YM_JSON
