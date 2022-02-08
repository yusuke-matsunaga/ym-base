
/// @file codec_test.cc
/// @brief codec_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include <stdlib.h>


BEGIN_NAMESPACE_YM

// テストフィクスチャの共通部分
class codec_test :
public ::testing::Test
{
public:

  // 初期化
  void
  SetUp() override;

  // 終了処理
  void
  TearDown() override;

  // ファイル名(SetUp() 〜 TearDown() の間のみ有効)
  string mFileName;

  // サンプル入力データ
  const char* mTestData{"The quick brown fox jumps over the lazy dog.\n"};

  // mTestText のサイズ(SetUp() 以降有効)
  SizeType mTestSize;

  // バッファサイズ
  static const SizeType BUFF_SIZE = 4096;

  // バッファ
  char mBuff[BUFF_SIZE];

};


void
codec_test::SetUp()
{
  // 一時ファイルの生成
  char tempname[] = "/tmp/temp.XXXXXXXX";
  mFileName = mktemp(tempname);

  // テストデータのサイズ
  mTestSize = strlen(mTestData);
}

void
codec_test::TearDown()
{
  // 一時ファイルの削除
  unlink(mFileName.c_str());
}

END_NAMESPACE_YM
