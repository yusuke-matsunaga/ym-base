
/// @file StringIDO.cc
/// @brief StringIDO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/StringIDO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス StringIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] strt 文字列
StringIDO::StringIDO(const char* str) :
  mString(str)
{
  mReady = true;
}

// @brief デストラクタ
StringIDO::~StringIDO()
{
}

// @brief 1バイトの読み出し
// @return 読み込んだ値を返す．
ymuint8
StringIDO::read_8()
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 2バイトの読み出し
// @return 読み込んだ値を返す．
ymuint16
StringIDO::read_16()
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 4バイトの読み出し
// @return 読み込んだ値を返す．
ymuint32
StringIDO::read_32()
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 8バイトの読み出し
// @return 読み込んだ値を返す．
ymuint64
StringIDO::read_64()
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 単精度不動週数点数の読み出し
// @return 読み込んだ値を返す．
float
StringIDO::read_float()
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

/// @brief 倍精度不動週数点数の読み出し
// @return 読み込んだ値を返す．
double
StringIDO::read_double()
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列の読み出し
// @return 読み込んだ値を返す．
string
StringIDO::read_str()
{
  if ( is_ready() ) {
    mReady = false;
    return mString;
  }
  return string();
}

// @brief 読み出し可能なら true を返す．
bool
StringIDO::is_ready() const
{
  return mReady;
}

// @brief オープン中のファイル情報を得る．
const FileInfo&
StringIDO::file_info() const
{
  return mFileInfo;
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
void
StringIDO::set_file_info(const FileInfo& file_info)
{
  mFileInfo = file_info;
}

// @brief データを読み込む．
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @param[in] n 読み込むデータサイズ
// @return 実際に読み込んだ量を返す．
ymint64
StringIDO::read(ymuint8* buff,
		ymuint64 n)
{
  ASSERT_NOT_REACHED;
  return 0;
}

END_NAMESPACE_YM
