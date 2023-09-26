
/// @file JsonTest.cc
/// @brief JsonTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM

TEST(JsonTest, int)
{
  string json_str{"{ \"key\" : 123 }"};

  auto value = JsonValue::parse(json_str);
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_int() );
  EXPECT_EQ( 123, value1.get_int() );
}

TEST(JsonTest, float)
{
  string json_str{"{ \"key\" : 123.456 }"};

  auto value = JsonValue::parse(json_str);
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_float() );
  EXPECT_EQ( 123.456, value1.get_float() );
}

TEST(JsonTest, string)
{
  string json_str{"{ \"key\" : \"123\" }"};

  auto value = JsonValue::parse(json_str);
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_string() );
  EXPECT_EQ( "123", value1.get_string() );
}

TEST(JsonTest, true)
{
  string json_str{"{ \"key\" : true }"};

  auto value = JsonValue::parse(json_str);
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_bool() );
  EXPECT_EQ( true, value1.get_bool() );
}

TEST(JsonTest, false)
{
  string json_str{"{ \"key\" : false }"};

  auto value = JsonValue::parse(json_str);
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_bool() );
  EXPECT_EQ( false, value1.get_bool() );
}

TEST(JsonTest, null)
{
  string json_str{"{ \"key\" : null }"};

  auto value = JsonValue::parse(json_str);
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_null() );
}

TEST(JsonTest, read)
{
  string filename{"test.json"};
  auto path = string{TESTDATA_DIR} + "/" + filename;

  auto value = JsonValue::read(path);

  EXPECT_TRUE( value.has_key("str_key") );
  auto value1 = value["str_key"];
  EXPECT_TRUE( value1.is_string() );
  EXPECT_EQ( "abcd", value1.get_string() );

  EXPECT_TRUE( value.has_key("int_key") );
  auto value2 = value["int_key"];
  EXPECT_TRUE( value2.is_int() );
  EXPECT_EQ( 4, value2.get_int() );

  EXPECT_TRUE( value.has_key("float_key") );
  auto value3 = value["float_key"];
  EXPECT_TRUE( value3.is_float() );
  EXPECT_EQ( 0.15, value3.get_float() );

  EXPECT_TRUE( value.has_key("bool_key") );
  auto value4 = value["bool_key"];
  EXPECT_TRUE( value4.is_bool() );
  EXPECT_EQ( true, value4.get_bool() );

  EXPECT_TRUE( value.has_key("object_key") );
  auto value5 = value["object_key"];
  EXPECT_TRUE( value5.is_object() );

  EXPECT_TRUE( value.has_key("array_key") );
  auto value6 = value["array_key"];
  EXPECT_TRUE( value6.is_array() );
}

TEST(JsonTest, parse)
{
  string source_str;
  source_str += "{";
  source_str += "\"str_key\": \"abcd\",";
  source_str += "\"int_key\": 4,";
  source_str += "\"float_key\": 0.15,";
  source_str += "\"bool_key\": true,";
  source_str += "\"object_key\": {";
  source_str += "\"sub_key1\": 0,";
  source_str += "\"sub_key2\": 1";
  source_str += "},";
  source_str += "\"array_key\": [ 0, 1, 2, 3]";
  source_str += "}";

  auto value = JsonValue::parse(source_str);

  EXPECT_TRUE( value.has_key("str_key") );
  auto value1 = value["str_key"];
  EXPECT_TRUE( value1.is_string() );
  EXPECT_EQ( "abcd", value1.get_string() );

  EXPECT_TRUE( value.has_key("int_key") );
  auto value2 = value["int_key"];
  EXPECT_TRUE( value2.is_int() );
  EXPECT_EQ( 4, value2.get_int() );

  EXPECT_TRUE( value.has_key("float_key") );
  auto value3 = value["float_key"];
  EXPECT_TRUE( value3.is_float() );
  EXPECT_EQ( 0.15, value3.get_float() );

  EXPECT_TRUE( value.has_key("bool_key") );
  auto value4 = value["bool_key"];
  EXPECT_TRUE( value4.is_bool() );
  EXPECT_EQ( true, value4.get_bool() );

  EXPECT_TRUE( value.has_key("object_key") );
  auto value5 = value["object_key"];
  EXPECT_TRUE( value5.is_object() );

  EXPECT_TRUE( value.has_key("array_key") );
  auto value6 = value["array_key"];
  EXPECT_TRUE( value6.is_array() );

  string exp_str;
  exp_str += "{\n";
  exp_str += "    \"array_key\": [\n";
  exp_str += "        0,\n";
  exp_str += "        1,\n";
  exp_str += "        2,\n";
  exp_str += "        3\n";
  exp_str += "    ],\n";
  exp_str += "    \"bool_key\": true,\n";
  exp_str += "    \"float_key\": 0.15,\n";
  exp_str += "    \"int_key\": 4,\n";
  exp_str += "    \"object_key\": {\n";
  exp_str += "        \"sub_key1\": 0,\n";
  exp_str += "        \"sub_key2\": 1\n";
  exp_str += "    },\n";
  exp_str += "    \"str_key\": abcd\n";
  exp_str += "}\n";

  ostringstream os;
  value.write(os, true);

  EXPECT_EQ( exp_str, os.str() );
}

END_NAMESPACE_YM
