﻿#ifndef YM_STREAMIDO_H
#define YM_STREAMIDO_H

/// @file ym/StreamIDO.h
/// @brief StreamIDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym/IDO.h"
#include "ym/FileInfo.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StreamIDO StreamIDO.h "ym/StreamIDO.h"
/// @ingroup ym
/// @brief istream を用いた IDO の継承クラス
//////////////////////////////////////////////////////////////////////
class StreamIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 入力ストリーム
  StreamIDO(istream& s);

  /// @brief デストラクタ
  virtual
  ~StreamIDO();


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出し可能なら true を返す．
  virtual
  bool
  is_ready() const;

  /// @brief オープン中のファイル情報を得る．
  virtual
  const FileInfo&
  file_info() const;

  /// @brief 現在のファイル情報を書き換える．
  /// @param[in] file_info 新しいファイル情報
  /// @note プリプロセッサのプラグマなどで用いることを想定している．
  /// @note 通常は使わないこと．
  virtual
  void
  set_file_info(const FileInfo& file_info);

  /// @brief データを読み込む．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @return 実際に読み込んだ量を返す．
  virtual
  int
  read(ymuint8* buff,
       int n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  // このクラスでは意味を持たない．
  FileInfo mFileInfo;

  // 入力ストリーム
  istream& mS;

};

END_NAMESPACE_YM

#endif // YM_STREAMIDO_H
