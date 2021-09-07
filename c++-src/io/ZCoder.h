#ifndef ZCODER_H
#define ZCODER_H

/// @file ZCoder.h
/// @brief ZCoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "FileCoder.h"
#include "FileBuff.h"
#include "compress_common.h"


BEGIN_NAMESPACE_YM_COMPCOMMON

//////////////////////////////////////////////////////////////////////
/// @class ZCoder ZCoder.h "ZCoder.h"
/// @brief compress(Z) 形式の圧縮器
//////////////////////////////////////////////////////////////////////
class ZCoder :
  public FileCoder
{
public:

  /// @brief コンストラクタ
  ZCoder();

  /// @brief デストラクタ
  ~ZCoder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  bool
  open(
    const char* filename, ///< [in] ファイル名
    mode_t mode = 0666,   ///< [in] ファイル作成用のモード
    int level = 0         ///< [in] 圧縮レベル
  ) override;

  /// @brief ファイルを閉じる．
  void
  close() override;

  /// @brief 書き込み可能の時に true を返す．
  bool
  is_ready() const override;

  /// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
  /// @return 実際に書き込んだバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  SizeType
  write(
    const ymuint8* wbuff, ///< [in] 圧縮するデータを格納するバッファ
    SizeType num          ///< [in] 書き込むデータ数(バイト)
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  int
  cl_block();

  void
  cl_hash(count_int hsize);

  int
  output(code_int code);

  count_int&
  htabof(code_int i)
  {
    return m_htab[i];
  }

  code_int&
  codetabof(code_int i)
  {
    return m_codetab[i];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コードあたりのビット数
  SizeType m_n_bits;

  // n_bits の最大値
  SizeType m_maxbits;

  // Maximum code
  code_int m_maxcode;

  // Should NEVER generate this code
  code_int m_maxmaxcode;

  tState m_state;

  code_int m_free_ent;

  int m_block_compress;

  int m_clear_flg;

  count_int m_htab[k_HSIZE];

  code_int m_codetab[k_HSIZE];

  long m_fcode;

  code_int m_ent;

  code_int m_hsize_reg;

  int m_hshift;

  int m_offset;

  code_int m_hsize;

  count_int m_ratio;

  count_int m_checkpoint;

  count_int m_in_count;

  count_int m_out_count;

  long m_bytes_out;

  char_type m_buf[k_BITS];

  // ファイルバッファ
  FileBuff mBuff;

};

END_NAMESPACE_YM_COMPCOMMON

BEGIN_NAMESPACE_YM

using nsCompCommon::ZCoder;

END_NAMESPACE_YM

#endif // ZCODER_H
