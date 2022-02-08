#ifndef ZFILE_H
#define ZFILE_H

/// @file ZFile.h
/// @brief ZFile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BzStream.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ZError ZFile.h "ZFile.h"
/// @brief ZFile のエラーを表すクラス
//////////////////////////////////////////////////////////////////////
class ZError
{
public:

  /// @brief コンストラクタ
  ZError(
    const char* msg ///< [in] メッセージ
  ) : mMsg{msg}
  {
  }

  /// @brief デストラクタ
  ~ZError() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージ
  string mMsg;

};


//////////////////////////////////////////////////////////////////////
/// @class ZFile ZFile.h "ZFile.h"
/// @brief compress(Z) 形式の圧縮/伸張を行なうファイルバッファ
//////////////////////////////////////////////////////////////////////
class ZFile :
  public BzStream
{
public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  using code_int = ymint64;
  using count_int = ymint64;
  using char_type = ymuint8;


public:

  /// @brief コンストラクタ
  ZFile(
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  );

  /// @brief デストラクタ
  ~ZFile();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを圧縮用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  bool
  deflate_open(
    const char* filename, ///< [in] ファイル名
    mode_t mode = 0666,   ///< [in] ファイル作成用のモード
    int level = 0         ///< [in] 圧縮レベル
  );

  /// @brief ファイルを圧縮用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  bool
  deflate_open(
    const string& filename, ///< [in] ファイル名
    mode_t mode = 0666,     ///< [in] ファイル作成用のモード
    int level = 0           ///< [in] 圧縮レベル
  )
  {
    return deflate_open(filename.c_str(), mode, level);
  }

  /// @brief ファイルを伸張用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  bool
  inflate_open(
    const char* filename ///< [in] ファイル名
  );

  /// @brief ファイルを伸張用に開く
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  bool
  inflate_open(
    const string& filename ///< [in] ファイル名
  )
  {
    return inflate_open(filename.c_str());
  }

  /// @brief ファイルを閉じる．
  void
  close();

  /// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
  ///
  /// エラーが起こったら -1 を返す．
  void
  write(
    const ymuint8* wbuff, ///< [in] 圧縮するデータを格納するバッファ
    SizeType num          ///< [in] 書き込むデータ数(バイト)
  );

  /// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
  /// @return 実際に読み出したバイト数を返す．
  ///
  /// エラーが起こったら -1 を返す．
  SizeType
  read(
    ymuint8* rbuff, ///< [in] 展開したデータを格納するバッファ
    SizeType num    ///< [in] 読み出すデータ数(バイト)
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  cl_block();

  void
  cl_hash(
    count_int hsize
  );

  void
  output(
    code_int code
  );

  count_int&
  htabof(
    code_int i
  )
  {
    return m_htab[i];
  }

  code_int&
  codetabof(
    code_int i
  )
  {
    return m_codetab[i];
  }

  code_int
  getcode();

  /// @brief スタックを初期化する．
  void
  init_stack()
  {
    mStackPtr = &mStack[0];
  }

  /// @brief スタックが空の時 true を返す．
  bool
  is_empty()
  {
    return mStackPtr == &mStack[0];
  }

  /// @brief スタックにデータを積む．
  void
  push_stack(
    char_type data
  )
  {
    ASSERT_COND( mStackPtr < &mStack[8000] );

    *(mStackPtr ++) = data;
  }

  /// @brief スタックからデータを取り出す．
  char_type
  pop_stack()
  {
    return *(-- mStackPtr);
  }

  code_int&
  tab_prefixof(
    code_int i
  )
  {
    return mPrefix[i];
  }

  char_type&
  tab_suffixof(
    code_int i
  )
  {
    return mSuffix[i];
  }

  /// @brief num バイトを読み込み buff[] に格納する．
  /// @return 実際に読み込んだバイト数を返す．
  int
  _read(
    ymuint8* buff, ///< [in] データを格納するバッファ
    SizeType num   ///< [in] 読み込むバイト数．
  );

private:
  //////////////////////////////////////////////////////////////////////
  // データ構造の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態
  enum State {
    START_STATE,  ///< 開始
    MIDDLE_STATE, ///< 処理中
    EOF_STATE     ///< 末尾
  };

  /// @brief モード
  enum Mode {
    None,    ///< なし
    Deflate, ///< 圧縮
    Inflate  ///< 伸張
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モード
  Mode mMode{None};

  // Default bits
  static
  const ymuint8 k_BITS = 16;

  // 95 % occupancy
  static
  const ymuint32 k_HSIZE = 69001;

  // コードあたりのビット数
  SizeType m_n_bits;

  // n_bits の最大値
  SizeType m_maxbits;

  // Maximum code
  code_int m_maxcode;

  // Should NEVER generate this code
  code_int m_maxmaxcode;

  State m_state;

  code_int m_free_ent;

  SizeType m_block_compress;

  SizeType m_clear_flg;

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

  char_type mStack[8000];

  char_type* mStackPtr;

  code_int mPrefix[k_HSIZE];

  char_type mSuffix[1 << k_BITS];

  code_int m_oldcode;

  char_type m_finchar;

  code_int m_incode;

  SizeType m_roffset;

  SizeType m_size;

  char_type m_gbuf[k_BITS];

  // ファイルバッファ
  FileBuff mFileBuff;

};

END_NAMESPACE_YM

#endif // ZFILE_H
