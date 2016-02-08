
/// @file StrListIDO.cc
/// @brief StrListIDO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ym/StrListIDO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス StrListIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] str_list 文字列のリスト
StrListIDO::StrListIDO(const vector<string>& str_list) :
  mStrList(str_list)
{
  mLineNum = 0;
  mColumnNum = 0;
}

// @brief デストラクタ
StrListIDO::~StrListIDO()
{
}

// @brief 1バイトの読み出し
// @return 読み込んだ値を返す．
ymuint8
StrListIDO::read_8()
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 2バイトの読み出し
// @return 読み込んだ値を返す．
ymuint16
StrListIDO::read_16()
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 4バイトの読み出し
// @return 読み込んだ値を返す．
ymuint32
StrListIDO::read_32()
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 8バイトの読み出し
// @return 読み込んだ値を返す．
ymuint64
StrListIDO::read_64()
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 単精度不動週数点数の読み出し
// @return 読み込んだ値を返す．
float
StrListIDO::read_float()
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 倍精度不動週数点数の読み出し
// @return 読み込んだ値を返す．
double
StrListIDO::read_double()
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列の読み出し
// @param[in] s 入力元のストリーム
string
StrListIDO::read_str()
{
  if ( mLineNum < mStrList.size() ) {
    const string& str = mStrList[mLineNum];
    ++ mLineNum;
    return str;
  }
  else {
    return string();
  }
}

// @brief 読み出し可能なら true を返す．
bool
StrListIDO::is_ready() const
{
  if ( mLineNum < mStrList.size() ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief オープン中のファイル情報を得る．
const FileInfo&
StrListIDO::file_info() const
{
  return mFileInfo;
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
void
StrListIDO::set_file_info(const FileInfo& file_info)
{
  mFileInfo = file_info;
}

// @brief データを読み込む．
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @param[in] n 読み込むデータサイズ
// @return 実際に読み込んだ量を返す．
ymint64
StrListIDO::read(ymuint8* buff,
		 ymuint64 n)
{
  ASSERT_NOT_REACHED;
  return 0;
}

END_NAMESPACE_YM
