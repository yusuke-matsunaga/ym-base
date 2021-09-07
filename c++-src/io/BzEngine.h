#ifndef BZENGINE_H
#define BZENGINE_H

/// @file BzEngine.h
/// @brief BzEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <bzlib.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BzEngine BzEngine.h "BzEngine.h"
/// @brief bz_stream の C++ 用ラッパクラス
//////////////////////////////////////////////////////////////////////
class BzEngine
{
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
  BzEngine(
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
  ~BzEngine() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 圧縮用の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮用の初期化を行う．
  int
  compress_init(
    int block_size_100k, ///< [in] 作業用のメモリサイズ ( 0 - 9 )
    int verbosity,       ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int work_factor      ///< [in] 圧縮アルゴリズム用のパラメータ ( 0 - 250 )
  )
  {
    return BZ2_bzCompressInit(&mBzStream, block_size_100k, verbosity, work_factor);
  }

  /// @brief 圧縮を行う．
  int
  compress(
    int action ///< [in] 動作コード
               ///<  - BZ_RUN
               ///<  - BZ_FLUSH
               ///<  - BZ_FINISH
  )
  {
    return BZ2_bzCompress(&mBzStream, action);
  }

  /// @brief 圧縮を終了する．
  int
  compress_end()
  {
    return BZ2_bzCompressEnd(&mBzStream);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 伸張用の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 伸張用のの初期化を行う．
  int
  decompress_init(
    int verbosity, ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int small      ///< [in] 伸張アルゴリズム用のパラメータ ( 0 か 非0 )
  )
  {
    return BZ2_bzDecompressInit(&mBzStream, verbosity, small);
  }

  /// @brief 伸張を行う．
  int
  decompress()
  {
    return BZ2_bzDecompress(&mBzStream);
  }

  /// @brief 伸張を終了する．
  int
  decompress_end()
  {
    return BZ2_bzDecompressEnd(&mBzStream);
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
    // void* を利用した巧妙なキャスト
    // といっても本当は良くないコード
    void* tmp = const_cast<ymuint8*>(buf);
    mBzStream.next_in = static_cast<char*>(tmp);
    mBzStream.avail_in = size;
  }

  /// @brief out バッファを設定する．
  void
  set_outbuf(
    ymuint8* buf, ///< [in] バッファ本体
    int size      ///< [in] バッファのサイズ
  )
  {
    // void* を利用した巧妙なキャスト
    // といっても本当は良くないコード
    void* tmp = buf;
    mBzStream.next_out = static_cast<char*>(tmp);
    mBzStream.avail_out = size;
  }

  /// @brief 読み出せるデータのバイト数を返す．
  int
  avail_in()
  {
    return mBzStream.avail_in;
  }

  /// @brief 書き込めるデータのバイト数を返す．
  int
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

};

END_NAMESPACE_YM

#endif // BZENGINE_H
