#ifndef LZMASTREAM_H
#define LZMASTREAM_H

/// @file LzmaStream.h
/// @brief LzmaStream のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <lzma.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LzmaError LzmaStream.h "LzmaStream.h"
/// @brief lzma のエラーを表すクラス
//////////////////////////////////////////////////////////////////////
class LzmaError
{
public:

  /// @brief コンストラクタ
  LzmaError(
    lzma_ret ret,   ///< [in] リターンコード
    const char* msg ///< [in] メッセージ
  ) : mRet{ret},
      mMsg{msg}
  {
  }

  /// @brief デストラクタ
  ~LzmaError() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リターンコード
  lzma_ret mRet;

  // メッセージ
  string mMsg;

};


//////////////////////////////////////////////////////////////////////
/// @class LzmaStream LzmaStream.h "LzmaStream.h"
/// @brief lzma_stream の C++ 用ラッパクラス
//////////////////////////////////////////////////////////////////////
class LzmaStream
{
public:

  /// @brief コンストラクタ
  LzmaStream(
    lzma_allocator* af = nullptr ///< [in] alloc 関数
  )
  {
    mLzmaStream = (lzma_stream)LZMA_STREAM_INIT;
    if ( af != nullptr ) {
      mLzmaStream.allocator = af;
    }
  }

  /// @brief デストラクタ
  ~LzmaStream()
  {
    end();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 初期化/終了処理を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief xz 圧縮用の初期化を行う．
  void
  easy_encoder(
    int preset = 6,                     ///< [in] 圧縮レベル ( 0 - 9: 6 がデフォルト )
    lzma_check check = LZMA_CHECK_CRC64 ///< [in] 検査方法
                                        ///<  - LZMA_CHECK_NONE
                                        ///<  - LZMA_CHECK_CRC32
                                        ///<  - LZMA_CHECK_CRC64
                                        ///<  - LZMA_CHECK_SHA256
  )
  {
    auto ret = lzma_easy_encoder(&mLzmaStream, preset, check);
    if ( ret != LZMA_OK ) {
      throw LzmaError{ret, "lzma_easy_encoder"};
    }
  }

  /// @brief xz 伸長用の初期化を行う．
  void
  stream_decoder(
    SizeType memlimit = 128 * 1024 * 1024, ///< [in] 割り当てるメモリの上限
    ymuint32 flags = 0                     ///< [in] 動作制御用のフラグ
  )
  {
    auto ret = lzma_stream_decoder(&mLzmaStream, memlimit, flags);
    if ( ret != LZMA_OK ) {
      throw LzmaError{ret, "lzma_stream_decoder"};
    }
  }

  /// @brief 終了処理を行う．
  void
  end()
  {
    lzma_end(&mLzmaStream);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 圧縮/伸長を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮/伸長を行う．
  /// @return 終了コードを返す．
  lzma_ret
  code(
    lzma_action action ///< [in] 動作コード
                       ///<  - LZMA_RUN
                       ///<  - LZMA_SYNC_FLUSH
                       ///<  - LZMA_FULL_FLUSH
                       ///<  - LZMA_FINISH
  )
  {
    auto ret = lzma_code(&mLzmaStream, action);
    if ( ret != LZMA_OK && ret != LZMA_STREAM_END ) {
      throw LzmaError{ret, "lzma_code"};
    }
    return ret;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // バッファの操作関数
  //////////////////////////////////////////////////////////////////////

  /// @brief in バッファを設定する．
  void
  set_inbuf(
    const ymuint8* buf, ///< [in] バッファ本体
    SizeType size       ///< [in] バッファのサイズ
  )
  {
    mLzmaStream.next_in = buf;
    mLzmaStream.avail_in = size;
  }

  /// @brief out バッファを設定する．
  void
  set_outbuf(
    ymuint8* buf, ///< [in] バッファ本体
    SizeType size ///< [in] バッファのサイズ
  )
  {
    mLzmaStream.next_out = buf;
    mLzmaStream.avail_out = size;
  }

  /// @brief 読み出せるデータのバイト数を返す．
  SizeType
  avail_in()
  {
    return mLzmaStream.avail_in;
  }

  /// @brief 書き込めるデータのバイト数を返す．
  SizeType
  avail_out()
  {
    return mLzmaStream.avail_out;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の構造体
  lzma_stream mLzmaStream;

};

END_NAMESPACE_YM

#endif // LZMASTREAM_H
