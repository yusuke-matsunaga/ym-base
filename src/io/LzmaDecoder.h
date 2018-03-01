﻿#ifndef LZMADECODER_H
#define LZMADECODER_H

/// @file LzmaDecoder.h
/// @brief LzmaDecoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FileDecoder.h"
#include "LzmaEngine.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LzmaDecoder LzmaDecoder.h "LzmaDecoder.h"
/// @brief bzip2 形式の伸長器
//////////////////////////////////////////////////////////////////////
class LzmaDecoder :
  public FileDecoder
{
public:

  /// @brief コンストラクタ
  LzmaDecoder();

  /// @brief デストラクタ
  virtual
  ~LzmaDecoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  virtual
  bool
  open(const char* filename);

  /// @brief ファイルを閉じる．
  virtual
  void
  close();

  /// @brief 読み出せるデータがある時に true を返す．
  virtual
  bool
  is_ready() const;

  /// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
  /// @param[in] rbuff 展開したデータを格納するバッファ
  /// @param[in] num 読み出すデータ数(バイト)
  /// @return 実際に読み出したバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  virtual
  int
  read(ymuint8* rbuff,
       int num);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ファイル用のバッファ
  FileBuff mBuff;

  // 伸長エンジン
  LzmaEngine mDecompEngine;

};

END_NAMESPACE_YM

#endif // LZMADECODER_H
