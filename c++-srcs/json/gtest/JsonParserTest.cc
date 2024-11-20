
/// @file JsonParserTest.cc
/// @brief JsonParserTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "JsonParser.h"
#include "JsonObj.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_JSON

TEST(JsonParserTest, int)
{
  istringstream s{"{ \"key\" : 123 }"};

  JsonParser parser{s};

  auto value = parser.read();
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_int() );
  EXPECT_EQ( 123, value1.get_int() );
}

TEST(JsonParserTest, float)
{
  istringstream s{"{ \"key\" : 123.456 }"};

  JsonParser parser{s};

  auto value = parser.read();
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_float() );
  EXPECT_EQ( 123.456, value1.get_float() );
}

TEST(JsonParserTest, string)
{
  istringstream s{"{ \"key\" : \"123\" }"};

  JsonParser parser{s};

  auto value = parser.read();
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_string() );
  EXPECT_EQ( "123", value1.get_string() );
}

TEST(JsonParserTest, true)
{
  istringstream s{"{ \"key\" : true }"};

  JsonParser parser{s};

  auto value = parser.read();
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_bool() );
  EXPECT_EQ( true, value1.get_bool() );
}

TEST(JsonParserTest, false)
{
  istringstream s{"{ \"key\" : false }"};

  JsonParser parser{s};

  auto value = parser.read();
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_bool() );
  EXPECT_EQ( false, value1.get_bool() );
}

TEST(JsonParserTest, null)
{
  istringstream s{"{ \"key\" : null }"};

  JsonParser parser{s};

  auto value = parser.read();
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_null() );
}

TEST(JsonParserTest, object2)
{
  istringstream s{"{ \"key1\" : 1, \"key2\" : 2 }"};

  JsonParser parser{s};

  auto value = parser.read();
  EXPECT_TRUE( value.is_object() );
  EXPECT_EQ( 2, value.size() );
  auto value1 = value["key1"];
  EXPECT_TRUE( value1.is_int() );
  auto value2 = value["key2"];
  EXPECT_TRUE( value2.is_int() );
}

TEST(JsonParserTest, array)
{
  istringstream s{"{ \"key\" : [ 1, 2, 3] }"};

  JsonParser parser{s};

  auto value = parser.read();
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_array() );
  EXPECT_EQ( 3, value1.size() );
  auto value2 = value1[0];
  EXPECT_EQ( 1, value2.get_int() );
  auto value3 = value1[1];
  EXPECT_EQ( 2, value3.get_int() );
  auto value4 = value1[2];
  EXPECT_EQ( 3, value4.get_int() );
}

TEST(JsonParserTest, null_array)
{
  istringstream s{"{ \"key\" : [ ] }"};

  JsonParser parser{s};

  auto value = parser.read();
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_array() );
  EXPECT_EQ( 0, value1.size() );
}

TEST(JsonParserTest, null_object)
{
  // 先頭の '{' が欠けている．
  istringstream s{"\"key\" : null }"};

  JsonParser parser{s};

  EXPECT_THROW(
    auto value = parser.read(),
    std::invalid_argument
  );
}

TEST(JsonParserTest, read_value_bad1)
{
  istringstream s{"{ \"key\" : , }"};

  JsonParser parser{s};

  EXPECT_THROW(
    auto value = parser.read(),
    std::invalid_argument
  );
}

TEST(JsonParserTest, object_bad1)
{
  istringstream s{"{ \"key\" : null "};

  JsonParser parser{s};

  EXPECT_THROW(
    auto value = parser.read(),
    std::invalid_argument
  );
}

TEST(JsonParserTest, object_bad2)
{
  istringstream s{"{ \"key\"  null "};

  JsonParser parser{s};

  EXPECT_THROW(
    auto value = parser.read(),
    std::invalid_argument
  );
}

TEST(JsonParserTest, object_bad3)
{
  istringstream s{"{ \"key1\" : 1 :]"};

  JsonParser parser{s};

  EXPECT_THROW(
    auto value = parser.read(),
    std::invalid_argument
  );
}

TEST(JsonParserTest, object_bad4)
{
  istringstream s{"{ \"key1\" : 1, 2 ]"};

  JsonParser parser{s};

  EXPECT_THROW(
    auto value = parser.read(),
    std::invalid_argument
  );
}

TEST(JsonParserTest, array_bad1)
{
  istringstream s{"{ \"key\" : [ "};

  JsonParser parser{s};

  EXPECT_THROW(
    auto value = parser.read(),
    std::invalid_argument
  );
}

TEST(JsonParserTest, array_bad2)
{
  istringstream s{"{ \"key\" : [ 1 : ]"};

  JsonParser parser{s};

  EXPECT_THROW(
    auto value = parser.read(),
    std::invalid_argument
  );
}

END_NAMESPACE_YM_JSON
