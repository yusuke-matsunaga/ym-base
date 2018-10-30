﻿#ifndef LZMACODER_H
#define LZMACODER_H

/// @file LzmaCoder.h
/// @brief LzmaCoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FileCoder.h"
#include "LzmaEngine.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LzmaCoder LzmaCoder.h "LzmaCoder.h"
/// @brief lzma(xz) 形式の圧縮器
//////////////////////////////////////////////////////////////////////
class LzmaCoder :
  public FileCoder
{
public:

  /// @brief コンストラクタ
  LzmaCoder();

  /// @brief デストラクタ
  virtual
  ~LzmaCoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] mode ファイル作成用のモード
  /// @param[in] level 圧縮レベル ( 0 - 9 )
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 圧縮レベルは実際には 1 - 9 の整数
  /// 0 の場合にはデフォルト値(9)が用いられる．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  bool
  open(const char* filename,
       mode_t mode = 0666,
       int level = 0) override;

  /// @brief ファイルを閉じる．
  void
  close() override;

  /// @brief 書き込み可能の時に true を返す．
  bool
  is_ready() const override;

  /// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
  /// @param[in] wbuff 圧縮するデータを格納するバッファ
  /// @param[in] num 書き込むデータ数(バイト)
  /// @return 実際に書き込んだバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  int
  write(const ymuint8* wbuff,
	int num) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力用のバッファ
  FileBuff mBuff;

  // 圧縮エンジン
  LzmaEngine mCompEngine;

  // 次の動作コード
  lzma_action mNextAction;

};

END_NAMESPACE_YM

#endif // LZMACODER_H
