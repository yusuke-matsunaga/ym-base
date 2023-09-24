
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
  istringstream s{"{ \"key\" : 123 }"};

  auto value = JsonValue::read(s, FileInfo{});
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_int() );
  EXPECT_EQ( 123, value1.get_int() );
}

TEST(JsonTest, float)
{
  istringstream s{"{ \"key\" : 123.456 }"};

  auto value = JsonValue::read(s, FileInfo{});
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_float() );
  EXPECT_EQ( 123.456, value1.get_float() );
}

TEST(JsonTest, string)
{
  istringstream s{"{ \"key\" : \"123\" }"};

  auto value = JsonValue::read(s, FileInfo{});
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_string() );
  EXPECT_EQ( "123", value1.get_string() );
}

TEST(JsonTest, true)
{
  istringstream s{"{ \"key\" : true }"};

  auto value = JsonValue::read(s, FileInfo{});
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_bool() );
  EXPECT_EQ( true, value1.get_bool() );
}

TEST(JsonTest, false)
{
  istringstream s{"{ \"key\" : false }"};

  auto value = JsonValue::read(s, FileInfo{});
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_bool() );
  EXPECT_EQ( false, value1.get_bool() );
}

TEST(JsonTest, null)
{
  istringstream s{"{ \"key\" : null }"};

  auto value = JsonValue::read(s, FileInfo{});
  EXPECT_TRUE( value.is_object() );
  auto value1 = value["key"];
  EXPECT_TRUE( value1.is_null() );
}

END_NAMESPACE_YM
