#ifndef ZSTREAM_H
#define	ZSTREAM_H

/// @file ZlibEngine.h
/// @brief zlib を使った圧縮/伸長器
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <zlib.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ZlibError ZStream.h "ZStream.h"
/// @brief z_stream のエラーを表すクラス
//////////////////////////////////////////////////////////////////////
class ZlibError
{
public:

  /// @brief コンストラクタ
  ZlibError(
    int ret,        ///< [in] リターンコード
    const char* msg ///< [in] メッセージ
  ) : mRet{ret},
      mMsg{msg}
  {
  }

  /// @brief デストラクタ
  ~ZlibError() = default;


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
/// @class ZStream ZStream.h "ZStream.h"
/// @brief z_stream の C++ 的に進化したもの
//////////////////////////////////////////////////////////////////////
class ZStream
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
  /// @name クラスメソッド
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief zlib のバージョンを表す文字列を返す．
  static
  const char*
  version()
  {
    return zlibVersion();
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  ZStream(
    alloc_func af = Z_NULL, ///< [in] alloc 関数
    free_func ff = Z_NULL,  ///< [in] free 関数
    voidp op = Z_NULL       ///< [in] opaque オブジェクト
  )
  {
    mZ.zalloc = af;
    mZ.zfree = ff;
    mZ.opaque = op;
  }

  /// @brief コピーコンストラクタ
  ZStream(
    const ZStream& src ///< [in] コピー元のオブジェクト
  ) : mMode{src.mMode}
  {
    if ( mMode == 1 ) {
      deflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
    }
    else if ( mMode == 2 ) {
      inflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
    }
  }

  /// @brief 代入演算子
  /// @return 自分自身への定数参照を返す．
  ZStream&
  operator=(
    const ZStream& src ///< [in] コピー元のオブジェクト
  )
  {
    end();
    mMode = src.mMode;
    if ( deflate_mode() ) {
      deflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
    }
    else if ( inflate_mode() ) {
      inflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
    }

    return *this;
  }

  /// @brief デストラクタ
  ~ZStream()
  {
    end();
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 公開インターフェイス
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮モードの時 true を返す．
  bool
  deflate_mode() const
  {
    return mMode == Deflate;
  }

  /// @brief deflate 用の初期化を行う．
  ///
  /// 例外のコードは以下の通り
  /// - Z_MEM_ERROR メモリの確保に失敗した．
  /// - Z_STREAM_ERROR level の値が不適切だった．
  /// - Z_VERSION_ERROR 呼出側とライブラリの間でバージョンの不一致があった．
  void
  deflate_init(
    int level = Z_DEFAULT_COMPRESSION ///< [in] 圧縮レベル ( 0 <= level <= 9 )
  )
  {
    ASSERT_COND( mMode == None );
    mMode = Deflate;
    int ret = deflateInit(&mZ, level);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "defaultInit"};
    }
  }

  /// @brief データを圧縮する．
  /// @retval Z_OK 何らかの処理を行なってエラーが起こらなかった．
  /// @retval Z_STREAM_END 処理が終わった．
  ///
  /// 例外のコードは以下の通り
  /// - Z_STREAM_ERROR 内部状態が異常だった．
  /// - Z_BUF_ERROR 入力が読めなかったか出力バッファに余裕がなかった．
  ///
  /// Z_FINISH を指定した場合，Z_STREAM_END を返すまで繰り返す必要がある．
  int
  deflate(
    int flush = Z_NO_FLUSH ///< [in] フラッシュフラグ
                           ///<  - Z_NO_FLUSH   : フラッシュしない．
                           ///<  - Z_SYNC_FLUSH : deflate() ごとにフラッシュする．
                           ///<  - Z_FULL_FLUSH : Z_SYNC_FLUSH + 内部状態も毎回リセットする．
                           ///<  - Z_FINISH     : 入力を一気に読み込む．
  )
  {
    ASSERT_COND( mMode == Deflate );
    int ret = ::deflate(&mZ, flush);
    switch ( ret ) {
    case Z_OK:
    case Z_STREAM_END:
      break;
    default:
      // それ以外はエラー
      throw ZlibError{ret, "deflate"};
    }
    return ret;
  }

  /// @brief deflate 用に確保された領域の解放を行う．
  ///
  /// 例外のコードは以下の通り
  /// - Z_STREAM_ERROR 内部状態が異常だった．
  /// - Z_DATA_ERROR まだデータが残っている状態で呼び出された．
  void
  deflate_end()
  {
    ASSERT_COND( mMode == Deflate );
    mMode = None;
    int ret = deflateEnd(&mZ);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "deflateEnd"};
    }
  }

  /// @brief 進捗モードの時 true を返す．
  bool
  inflate_mode() const
  {
    return mMode == Inflate;
  }

  /// @brief inflate 用の初期化を行う．
  ///
  /// 例外のコードは以下の通り
  /// - Z_MEM_ERROR メモリの確保に失敗した．
  /// - Z_VERSION_ERROR ヘッダとライブラリのバージョンが不一致だった．
  void
  inflate_init()
  {
    ASSERT_COND( mMode == None );
    mMode = Inflate;
    int ret = inflateInit(&mZ);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "inflateInit"};
    }
  }

  /// @brief データを伸長する．
  /// @retval Z_OK 処理が成功した．
  /// @retval Z_STREAM_END 全てのデータを処理し終わった．
  /// @retval Z_BUF_ERROR 入力バッファが空だった．
  ///
  /// 例外のコードは以下の通り
  /// - Z_NEED_DICT 辞書が必要
  /// - Z_DATA_ERROR 入力データが壊れていた．
  /// - Z_MEM_ERROR 十分な量のメモリが確保できなかった．
  /// - Z_STREAM_ERROR 内部状態が異常だった．
  /// - Z_BUF_ERROR 出力バッファに空きがなかった．
  int
  inflate(
    int flush ///< [in] フラッシュフラグ
              ///<  - Z_NO_FLUSH フラッシュしない
              ///<  - Z_SYNC_FLUSH できる限り出力バッファに書き出す．
              ///<  - Z_FINISH 一気に伸長を行なう．
              ///<  - Z_BLOCK ブロック単位で処理を行なう．
  )
  {
    ASSERT_COND( mMode == Inflate );
    auto ret = ::inflate(&mZ, flush);
    switch ( ret ) {
    case Z_OK:
    case Z_STREAM_END:
      break;
    case Z_BUF_ERROR:
      if ( avail_out() > 0 ) {
	break;
      }
      // わざと次に続く．
    default:
      throw ZlibError{ret, "inflate"};
    }
    return ret;
  }

  /// @brief inflate 用に確保された領域の解放を行う．
  ///
  /// 例外のコードは以下の通り
  /// - Z_STREAM_ERROR 内部状態が異常だった．
  void
  inflate_end()
  {
    ASSERT_COND( mMode == Inflate );
    mMode = None;
    int ret = inflateEnd(&mZ);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "inflateEnd"};
    }
  }

  /// @brief 内部の状態に応じて inflateEnd か deflateEnd を呼ぶ．
  void
  end()
  {
    if ( mMode == Inflate ) {
      inflate_end();
    }
    else if ( mMode == Deflate ) {
      deflate_end();
    }
  }

  /// @brief in バッファを設定する．
  void
  set_inbuf(
    const ymuint8* buf, ///< [in] バッファ本体
    SizeType size       ///< [in] バッファのサイズ
  )
  {
    mZ.next_in = const_cast<Bytef*>(buf);
    mZ.avail_in = size;
  }

  /// @brief out バッファを設定する．
  void
  set_outbuf(
    ymuint8* buf, ///< [in] バッファ本体
    SizeType size ///< [in] バッファのサイズ
  )
  {
    mZ.next_out = buf;
    mZ.avail_out = size;
  }

  /// @brief avail_in の取得
  /// @return 読み出せるデータのバイト数を返す．
  uInt
  avail_in() const
  {
    return mZ.avail_in;
  }

  /// @brief avail_out の取得
  /// @return 書き込めるデータのバイト数を返す．
  uInt
  avail_out() const
  {
    return mZ.avail_out;
  }

  /// @brief msg を得る．
  const char*
  msg() const
  {
    return mZ.msg;
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 圧縮用の内部のパラメータ調整用の公開インターフェイス
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 詳細な設定を行う deflate 用の初期化
  void
  deflate_init2(
    int level,
    int method,
    int windowBits,
    int memLevel,
    int strategy
  )
  {
    ASSERT_COND( mMode == None );
    mMode = Deflate;
    int ret = deflateInit2(&mZ, level, method, windowBits, memLevel, strategy);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "deflateInit2"};
    }
  }

  /// @brief deflate 用の辞書をセットする．
  void
  deflate_set_dictionary(
    const Bytef* dictionary,
    uInt dictLength
  )
  {
    ASSERT_COND( mMode == Deflate );
    int ret = deflateSetDictionary(&mZ, dictionary, dictLength);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "deflateSetDictionary"};
    }
  }

  /// @brief deflate の状態をリセットする．
  void
  deflate_reset()
  {
    ASSERT_COND( mMode == Deflate );
    int ret = deflateReset(&mZ);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "deflateReset"};
    }
  }

  /// @brief deflate 用のパラメータをセットする．
  void
  deflate_params(
    int level,
    int strategy
  )
  {
    ASSERT_COND( mMode == Deflate );
    int ret = deflateParams(&mZ, level, strategy);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "deflateParams"};
    }
  }

  /// @brief 圧縮後のサイズの見積りを行う．
  int
  deflate_bound(
    uLong sourceLen
  )
  {
    ASSERT_COND( mMode == Deflate );
    int status = deflateBound(&mZ, sourceLen);
    return status;
  }

  /// @brief deflate 用の低レベル関数
  int
  deflate_prime(
    int bits,
    int value
  )
  {
    ASSERT_COND( mMode == Deflate );
    int status = deflatePrime(&mZ, bits, value);
    return status;
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 伸長用の内部のパラメータ調整用の公開インターフェイス
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief inflate 用の詳細な設定を行う初期化
  void
  inflate_init2(
    int windowBits
  )
  {
    ASSERT_COND( mMode == None );
    mMode = Inflate;
    int ret = inflateInit2(&mZ, windowBits);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "inflateInit2"};
    }
  }

  /// @brief inflate 用の辞書の設定
  void
  inflate_set_dictionary(
    const Bytef* dictionary,
    uInt dictLength
  )
  {
    ASSERT_COND( mMode == Inflate );
    int ret = inflateSetDictionary(&mZ, dictionary, dictLength);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "inflateSetDictionary"};
    }
  }

  /// @brief 不正なデータをスキップする．
  void
  inflate_sync()
  {
    ASSERT_COND( mMode == Inflate );
    int ret = inflateSync(&mZ);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "inflateSync"};
    }
  }

  /// @brief inflate 用の状態の初期化
  void
  inflate_reset()
  {
    ASSERT_COND( mMode == Inflate );
    int ret = inflateReset(&mZ);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "inflateReset"};
    }
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


#if ZLIB_VERNUM >= 0x1230

public:
  //////////////////////////////////////////////////////////////////////
  /// @name Ver 1.23 以降に追加された公開インターフェイス
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief deflate 用の細かなパラメータをセットする．
  void
  deflate_tune(
    int good_length,
    int max_lazy,
    int nice_length,
    int max_chain
  )
  {
    ASSERT_COND( mMode == Deflate );
    int ret = deflateTune(&mZ, good_length, max_lazy, nice_length, max_chain);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "deflateTune"};
    }
  }

  /// @brief deflate 用のヘッダのセット
  void
  deflate_set_header(
    gz_headerp head
  )
  {
    ASSERT_COND( mMode == Deflate );
    int ret = deflateSetHeader(&mZ, head);
    if ( ret != Z_OK ) {
      throw ZlibError{ret, "deflateSetHeader"};
    }
  }

  /// @brief inflate 用の低レベル関数
  int
  inflate_prime(
    int bits,
    int value
  )
  {
    ASSERT_COND( mMode == Inflate );
    int status = inflatePrime(&mZ, bits, value);
    return status;
  }

  /// @brief inflate 用のヘッダの取得
  int
  inflate_get_header(
    gz_headerp head
  )
  {
    ASSERT_COND( mMode == Inflate );
    int status = inflateGetHeader(&mZ, head);
    return status;
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////

#endif // ZLIB_VERNUM >= 0x1230


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の z_stream
  z_stream mZ;

  // deflate/inflate の状態
  Mode mMode{None};

};

END_NAMESPACE_YM

#endif // ZSTREAM_H
