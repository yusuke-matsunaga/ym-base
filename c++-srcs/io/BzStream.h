#ifndef BZSTREAM_H
#define BZSTREAM_H

/// @file BzStream.h
/// @brief BzStream のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <bzlib.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BzError BzStream.h "BzStream.h"
/// @brief bz_stream のエラーを表すクラス
//////////////////////////////////////////////////////////////////////
class BzError
{
public:

  /// @brief コンストラクタ
  BzError(
    int ret,        ///< [in] リターンコード
    const char* msg ///< [in] メッセージ
  ) : mRet{ret},
      mMsg{msg}
  {
  }

  /// @brief デストラクタ
  ~BzError() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リターンコード
  int mRet;

  // メッセージ
  string mMsg;

};


//////////////////////////////////////////////////////////////////////
/// @class BzStream BzStream.h "BzStream.h"
/// @brief bz_stream の C++ 用ラッパクラス
//////////////////////////////////////////////////////////////////////
class BzStream
{
private:

  /// @brief 内部状態
  enum Mode {
    None,    ///< 中立
    Deflate, ///< 圧縮
    Inflate  ///< 伸張
  };


public:
  //////////////////////////////////////////////////////////////////////
  // 関数の型定義
  //////////////////////////////////////////////////////////////////////

  /// @brief alloc 関数の型定義
  using alloc_func = void* (*)(void *, int, int);

  /// @brief free 関数の型定義
  using free_func = void (*)(void *, void *);


public:

  /// @brief コンストラクタ
  BzStream(
    alloc_func af = nullptr, ///< [in] alloc 関数
    free_func ff = nullptr,  ///< [in] free 関数
    void* op = nullptr       ///< [in] opaque オブジェクト
  )
  {
    mBzStream.bzalloc = af;
    mBzStream.bzfree = ff;
    mBzStream.opaque = op;
  }

  /// @brief デストラクタ
  ~BzStream()
  {
    if ( deflate_mode() ) {
      deflate_end();
    }
    else if ( inflate_mode() ) {
      inflate_end();
    }
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 圧縮用の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮モードの時 true を返す．
  bool
  deflate_mode() const
  {
    return mMode == Deflate;
  }

  /// @brief 圧縮用の初期化を行う．
  void
  deflate_init(
    int block_size_100k = 9, ///< [in] 作業用のメモリサイズ ( 1 - 9 )
    int verbosity       = 0, ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int work_factor     = 0  ///< [in] 圧縮アルゴリズム用のパラメータ ( 0 - 250 )
  )
  {
    ASSERT_COND( mMode == None );
    auto ret = BZ2_bzCompressInit(&mBzStream, block_size_100k, verbosity, work_factor);
    if ( ret != BZ_OK ) {
      throw BzError{ret, "BZ2_bzCompressInit"};
    }
    mMode = Deflate;
  }

  /// @brief 圧縮を行う．
  int
  deflate(
    int action ///< [in] 動作コード
               ///<  - BZ_RUN
               ///<  - BZ_FLUSH
               ///<  - BZ_FINISH
  )
  {
    ASSERT_COND( mMode == Deflate );
    auto ret = BZ2_bzCompress(&mBzStream, action);
    switch ( ret ) {
    case BZ_RUN_OK:
    case BZ_FLUSH_OK:
    case BZ_FINISH_OK:
    case BZ_STREAM_END:
      break;
    default:
      // それ以外はエラー
      throw BzError{ret, "BZ2_bzCompress"};
    }
    return ret;
  }

  /// @brief 圧縮を終了する．
  void
  deflate_end()
  {
    ASSERT_COND( mMode == Deflate );
    auto ret = BZ2_bzCompressEnd(&mBzStream);
    if ( ret != BZ_OK ) {
      throw BzError{ret, "BZ2_bzCompressEnd"};
    }
    mMode = None;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 伸張用の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 進捗モードの時 true を返す．
  bool
  inflate_mode() const
  {
    return mMode == Inflate;
  }

  /// @brief 伸張用のの初期化を行う．
  void
  inflate_init(
    int verbosity, ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int small      ///< [in] 伸張アルゴリズム用のパラメータ ( 0 か 非0 )
  )
  {
    ASSERT_COND( mMode == None );
    auto ret = BZ2_bzDecompressInit(&mBzStream, verbosity, small);
    if ( ret != BZ_OK ) {
      throw BzError{ret, "BZ2_bzDecompressInit"};
    }
    mMode = Inflate;
  }

  /// @brief 伸張を行う．
  int
  inflate()
  {
    ASSERT_COND( mMode == Inflate );
    auto ret = BZ2_bzDecompress(&mBzStream);
    switch ( ret ) {
    case BZ_OK:
    case BZ_STREAM_END:
      break;
    default:
      // それ以外はエラー
      throw BzError{ret, "BZ2_bzDecompress"};
    }
    return ret;
  }

  /// @brief 伸張を終了する．
  void
  inflate_end()
  {
    ASSERT_COND( mMode == Inflate );
    auto ret = BZ2_bzDecompressEnd(&mBzStream);
    if ( ret != BZ_OK ) {
      throw BzError{ret, "BZ2_bzDecompressEnd"};
    }
    mMode = None;
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
    auto tmp = reinterpret_cast<const char*>(buf);
    mBzStream.next_in = const_cast<char*>(tmp);
    mBzStream.avail_in = size;
  }

  /// @brief out バッファを設定する．
  void
  set_outbuf(
    ymuint8* buf, ///< [in] バッファ本体
    SizeType size ///< [in] バッファのサイズ
  )
  {
    mBzStream.next_out = reinterpret_cast<char*>(buf);
    mBzStream.avail_out = size;
  }

  /// @brief 読み出せるデータのバイト数を返す．
  SizeType
  avail_in()
  {
    return mBzStream.avail_in;
  }

  /// @brief 書き込めるデータのバイト数を返す．
  SizeType
  avail_out()
  {
    return mBzStream.avail_out;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の構造体
  bz_stream mBzStream;

  // 内部状態
  Mode mMode{None};

};

END_NAMESPACE_YM

#endif // BZSTREAM_H
