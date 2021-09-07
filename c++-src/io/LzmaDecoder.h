#ifndef LZMADECODER_H
#define LZMADECODER_H

/// @file LzmaDecoder.h
/// @brief LzmaDecoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2021 Yusuke Matsunaga
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
  ~LzmaDecoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  bool
  open(
    const char* filename ///< [in] ファイル名
  ) override;

  /// @brief ファイルを閉じる．
  void
  close() override;

  /// @brief 読み出せるデータがある時に true を返す．
  bool
  is_ready() const override;

  /// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
  /// @return 実際に読み出したバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  SizeType
  read(
    ymuint8* rbuff, ///< [in] 展開したデータを格納するバッファ
    SizeType num    ///< [in] 読み出すデータ数(バイト)
  ) override;


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
