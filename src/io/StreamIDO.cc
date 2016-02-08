
/// @file StreamIDO.cc
/// @brief StreamIDO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/StreamIDO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス StreamIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 入力ストリーム
StreamIDO::StreamIDO(istream& s) :
  mS(s)
{
}

// @brief デストラクタ
StreamIDO::~StreamIDO()
{
}

// @brief 読み出し可能なら true を返す．
bool
StreamIDO::is_ready() const
{
  return mS.good();
}

// @brief オープン中のファイル情報を得る．
const FileInfo&
StreamIDO::file_info() const
{
  return mFileInfo;
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
void
StreamIDO::set_file_info(const FileInfo& file_info)
{
  mFileInfo = file_info;
}

// @brief データを読み込む．
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @param[in] n 読み込むデータサイズ
// @return 実際に読み込んだ量を返す．
ymint64
StreamIDO::read(ymuint8* buff,
		ymuint64 n)
{
  mS.read(reinterpret_cast<char*>(buff), n);
  if ( mS ) {
    return n;
  }
  else {
    // かなり乱暴
    return 0;
  }
}

END_NAMESPACE_YM
