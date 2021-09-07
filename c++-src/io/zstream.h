#ifndef ZSTREAM_H
#define	ZSTREAM_H

/// @file zstream.h
/// @brief zlib を使った圧縮/伸長機能つきストリーム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <zlib.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class zstream zstream.h "zstream.h"
/// @brief z_stream の C++ 的に進化したもの
//////////////////////////////////////////////////////////////////////
class zstream
{
public:
  //////////////////////////////////////////////////////////////////////
  // クラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief zlib のバージョンを表す文字列を返す．
  static
  const char*
  version()
  {
    return zlibVersion();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  zstream(
    alloc_func af = Z_NULL, ///< [in] alloc 関数
    free_func ff = Z_NULL,  ///< [in] free 関数
    voidp op = Z_NULL       ///< [in] opaque オブジェクト
  ) : mMode{0}
  {
    mZ.zalloc = af;
    mZ.zfree = ff;
    mZ.opaque = op;
  }

  /// @brief コピーコンストラクタ
  zstream(
    const zstream& src ///< [in] コピー元のオブジェクト
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
  const zstream&
  operator=(
    const zstream& src ///< [in] コピー元のオブジェクト
  )
  {
    if ( mMode == 1 ) {
      deflateEnd(&mZ);
    }
    else if ( mMode == 2 ) {
      inflateEnd(&mZ);
    }
    mMode = src.mMode;
    if ( mMode == 1 ) {
      deflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
    }
    else if ( mMode == 2 ) {
      inflateCopy(&mZ, const_cast<z_streamp>(&src.mZ));
    }
    return *this;
  }

  /// @brief デストラクタ
  ~zstream()
  {
    end();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 公開インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief deflate 用の初期化を行う．
  /// @retval Z_OK 初期化が成功した．
  /// @retval Z_MEM_ERROR メモリの確保に失敗した．
  /// @retval Z_STREAM_ERROR level の値が不適切だった．
  /// @retval Z_VERSION_ERROR 呼出側とライブラリの間でバージョンの不一致があった．
  /// @note level = 0 は非圧縮
  int
  deflate_init(
    int level = Z_DEFAULT_COMPRESSION ///< [in] 圧縮レベル ( 0 <= level <= 9 )
  )
  {
    mMode = 1;
    int status = deflateInit(&mZ, level);
    // エラーは素通しにする．
    return status;
  }

  /// @brief データを圧縮する．
  /// @retval Z_OK 何らかの処理を行なってエラーが起こらなかった．
  /// @retval Z_STREAM_END 処理が終わった．
  /// @retval Z_STREAM_ERROR 内部状態が異常だった．
  /// @retval Z_BUF_ERROR 入力が読めなかったか出力バッファに余裕がなかった．
  /// @note Z_FINISH を指定した場合，Z_STREAM_END を返すまで繰り返す必要がある．
  int
  deflate(
    int flush = Z_NO_FLUSH ///< [in] フラッシュフラグ
                           ///<  - Z_NO_FLUSH   : フラッシュしない．
                           ///<  - Z_SYNC_FLUSH : deflate() ごとにフラッシュする．
                           ///<  - Z_FULL_FLUSH : Z_SYNC_FLUSH + 内部状態も毎回リセットする．
                           ///<  - Z_FINISH     : 入力を一気に読み込む．
  )
  {
    int status = ::deflate(&mZ, flush);
    // エラーは素通しにする．
    return status;
  }

  /// @brief deflate 用に確保された領域の解放を行う．
  /// @retval Z_OK 処理が成功した．
  /// @retval Z_STREAM_ERROR 内部状態が異常だった．
  /// @retval Z_DATA_ERROR まだデータが残っている状態で呼び出された．
  int
  deflate_end()
  {
    mMode = 0;
    int status = deflateEnd(&mZ);
    // エラーは素通しにする．
    return status;
  }

  /// @brief inflate 用の初期化を行う．
  /// @retval Z_OK 処理が成功した．
  /// @retval Z_MEM_ERROR メモリの確保に失敗した．
  /// @retval Z_VERSION_ERROR ヘッダとライブラリのバージョンが不一致だった．
  int
  inflate_init()
  {
    mMode = 2;
    int status = inflateInit(&mZ);
    // エラーは素通しにする．
    return status;
  }

  /// @brief データを伸長する．
  /// @retval Z_OK 処理が成功した．
  /// @retval Z_NEED_DICT 辞書が必要
  /// @retval Z_STREAM_END 全てのデータを処理し終わった．
  /// @retval Z_DATA_ERROR 入力データが壊れていた．
  /// @retval Z_MEM_ERROR 十分な量のメモリが確保できなかった．
  /// @retval Z_STREAM_ERROR 内部状態が異常だった．
  /// @retval Z_BUF_ERROR 出力バッファに空きがなかった．
  int
  inflate(
    int flush ///< [in] フラッシュフラグ
              ///<  - Z_NO_FLUSH フラッシュしない
              ///<  - Z_SYNC_FLUSH できる限り出力バッファに書き出す．
              ///<  - Z_FINISH 一気に伸長を行なう．
              ///<  - Z_BLOCK ブロック単位で処理を行なう．
  )
  {
    int status = ::inflate(&mZ, flush);
    // エラーは素通しにする．
    return status;
  }

  /// @brief inflate 用に確保された領域の解放を行う．
  /// @retval Z_OK 処理が成功した．
  /// @retval Z_STREAM_ERROR 内部状態が異常だった．
  int
  inflate_end()
  {
    mMode = 0;
    int status = inflateEnd(&mZ);
    // エラーは素通しにする．
    return status;
  }

  /// @brief 内部の状態に応じて inflateEnd か deflateEnd を呼ぶ．
  int
  end()
  {
    if ( mMode == 2 ) {
      return inflate_end();
    }
    else if ( mMode == 1 ) {
      return deflate_end();
    }
    // なにもしない．
    return Z_OK;
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


public:
  //////////////////////////////////////////////////////////////////////
  // 圧縮用の内部のパラメータ調整用の公開インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 詳細な設定を行う deflate 用の初期化
  int
  deflate_init2(
    int level,
    int method,
    int windowBits,
    int memLevel,
    int strategy
  )
  {
    mMode = 1;
    int status = deflateInit2(&mZ, level, method, windowBits, memLevel, strategy);
    return status;
  }

  /// @brief deflate 用の辞書をセットする．
  int
  deflate_set_dictionary(
    const Bytef* dictionary,
    uInt dictLength
  )
  {
    int status = deflateSetDictionary(&mZ, dictionary, dictLength);
    return status;
  }

  /// @brief deflate の状態をリセットする．
  int
  deflate_reset()
  {
    int status = deflateReset(&mZ);
    return status;
  }

  /// @brief deflate 用のパラメータをセットする．
  int
  deflate_params(
    int level,
    int strategy
  )
  {
    int status = deflateParams(&mZ, level, strategy);
    return status;
  }

  /// @brief 圧縮後のサイズの見積りを行う．
  int
  deflate_bound(
    uLong sourceLen
  )
  {
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
    int status = deflatePrime(&mZ, bits, value);
    return status;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 伸長用の内部のパラメータ調整用の公開インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief inflate 用の詳細な設定を行う初期化
  int
  inflate_init2(
    int windowBits
  )
  {
    mMode = 2;
    int status = inflateInit2(&mZ, windowBits);
    return status;
  }

  /// @brief inflate 用の辞書の設定
  int
  inflate_set_dictionary(
    const Bytef* dictionary,
    uInt dictLength
  )
  {
    int status = inflateSetDictionary(&mZ, dictionary, dictLength);
    return status;
  }

  /// @brief 不正なデータをスキップする．
  int
  inflate_sync()
  {
    int status = inflateSync(&mZ);
    return status;
  }

  /// @brief inflate 用の状態の初期化
  int
  inflate_reset()
  {
    int status = inflateReset(&mZ);
    return status;
  }


#if ZLIB_VERNUM >= 0x1230

public:
  //////////////////////////////////////////////////////////////////////
  // Ver 1.23 以降に追加された公開インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief deflate 用の細かなパラメータをセットする．
  int
  deflate_tune(
    int good_length,
    int max_lazy,
    int nice_length,
    int max_chain
  )
  {
    int status = deflateTune(&mZ, good_length, max_lazy, nice_length, max_chain);
    return status;
  }

  /// @brief deflate 用のヘッダのセット
  int
  deflate_set_header(
    gz_headerp head
  )
  {
    int status = deflateSetHeader(&mZ, head);
    return status;
  }

  /// @brief inflate 用の低レベル関数
  int
  inflate_prime(
    int bits,
    int value
  )
  {
    int status = inflatePrime(&mZ, bits, value);
    return status;
  }

  /// @brief inflate 用のヘッダの取得
  int
  inflate_get_header(
    gz_headerp head
  )
  {
    int status = inflateGetHeader(&mZ, head);
    return status;
  }

#endif // ZLIB_VERNUM >= 0x1230


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の z_stream
  z_stream mZ;

  // deflate/inflate の状態
  int mMode;

};


//////////////////////////////////////////////////////////////////////
/// @class zstream_buff zstream.h "zstream.h"
/// @brief バッファ付きの zstream クラス
//////////////////////////////////////////////////////////////////////
class zstream_buff :
  public zstream
{
public:

  /// @brief 入力用のコンストラクタ
  explicit
  zstream_buff(
    istream* s,              ///< [in] 入力ストリーム
    SizeType size = BUFFSIZE ///< [in] バッファサイズ
  );

  /// @brief 出力用のコンストラクタ
  explicit
  zstream_buff(
    ostream* s,              ///< [in] 出力ストリーム
    SizeType size = BUFFSIZE ///< [in] バッファサイズ
  );

  /// @brief デストラクタ
  ~zstream_buff();


public:

  /// @brief データの圧縮を行う．
  ///
  /// 結果は出力ストリームに書き込まれる．
  void
  compress(
    Bytef* buff,   ///< [in] 圧縮するデータを格納しているバッファ
    SizeType size, ///< [in] バッファ中の有効なデータのサイズ
    int flush      ///< [in] フラッシュ制御用のフラグ
  );

  /// @brief データの伸長を行う．
  /// @return バッファに書き出されたサイズ(in bytes)を返す．
  ///
  /// データは入力ストリームから読み込まれる．
  int
  decompress(
    Bytef* buff,  ///< [in] 伸長したデータを格納するバッファ
    SizeType size ///< [in] バッファ中の空きサイズ(in bytes)
  );

  /// @brief 入力ストリームを取り出す．
  istream*
  in()
  {
    return mInStream;
  }

  /// @brief 出力ストリームを取り出す．
  ostream*
  out()
  {
    return mOutStream;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で呼ばれる下請け関数
  //////////////////////////////////////////////////////////////////////

  // バッファの内容を書き出す．
  void
  write()
  {
    mOutStream->write(reinterpret_cast<char*>(mBuff), mSize - avail_out());
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  // バッファサイズ
  static
  const int BUFFSIZE = 4096;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 圧縮データを入力するストリーム
  istream* mInStream;

  // 圧縮データを出力するストリーム
  ostream* mOutStream;

  // 内側の(圧縮されている)バッファ
  Bytef* mBuff;

  // mBuff のサイズ
  int mSize;

  // 伸長モードの時のフラッシュフラグ
  int mInFlush;

};


//////////////////////////////////////////////////////////////////////
/// @class basic_zlib_streambuf zstream.h "zstream.h"
/// @brief zlib を使ったストリームバッファのテンプレート
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_zlib_streambuf :
  public std::basic_streambuf<_CharT, _Traits>
{
public:
  typedef _CharT char_type;
  typedef _Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  typedef std::basic_streambuf<char_type, traits_type> streambuf_type;

public:

  /// @brief 入力用のコンストラクタ
  explicit
  basic_zlib_streambuf(
    istream& s,         ///< [in] 入力ストリーム
    SizeType size       ///< [in] バッファサイズ
    = zstream_buff::BUFFSIZE
  ) : mZbuf(&s, size),
      mBuff(nullptr),
      mSize(size),
      mBuff0(new char_type[size])
  {
    mBuff = mBuff0;
    int wsize = mZbuf.decompress(byte_buff(), byte_size(mSize));
    this->setg(mBuff, mBuff, mBuff + char_size(wsize));
  }

  /// @brief 出力用のコンストラクタ
  explicit
  basic_zlib_streambuf(
    ostream& s,    ///< [in] 出力ストリーム
    SizeType size  ///< [in] バッファサイズ
    = zstream_buf::BUFFSIZE
  ) : mZbuf(&s, size),
      mBuff(nullptr),
      mSize(size),
      mBuff0(new char_type[size])
  {
    mBuff = mBuff0;
    mOutFlush = Z_NO_FLUSH;
    this->setp(mBuff, mBuff + mSize);
  }

  /// @brief デストラクタ
  ~basic_zlib_streambuf()
  {
    mOutFlush = Z_FINISH;

    sync();

    mZbuf.end();

    delete [] mBuff0;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // basic_streambuf<_CharT, _Traits> の仮想関数
  //////////////////////////////////////////////////////////////////////

  streambuf_type*
  setbuf(
    char_type* buf,
    int size
  ) override
  {
    if ( mZbuf.in() ) {
      this->setg(buf, buf, buf + size);
    }
    if ( mZbuf.out() ) {
      this->setp(buf, buf + size);
    }
    mBuff = buf;
    mSize = size;
    return this;
  }

  int_type
  overflow(
    int_type c = traits_type::eof()
  ) override
  {
    // バッファに溜っているデータを圧縮する．
    mZbuf.compress(byte_buff(), byte_size(this->pptr() - mBuff), mOutFlush);
    this->setp(mBuff, mBuff + mSize);

    if ( c == traits_type::eof() ) {
      return c;
    }

    // 一文字書き込む
    *(this->pptr()) = traits_type::to_char_type(c);
    this->pbump(1);

    return traits_type::not_eof(c);
  }

  int_type
  underflow() override
  {
    if ( this->egptr() <= this->gptr() ) {
      // データを伸長してバッファに入れる．
      int wsize = mZbuf.decompress(byte_buff(), byte_size(mSize));
      this->setg(mBuff, mBuff, mBuff + char_size(wsize));
      if ( wsize == 0 ) {
	return traits_type::eof();
      }
    }

    // 一文字読み出す．
    return traits_type::to_int_type(*(this->gptr()));
  }

  int
  sync() override
  {
    if ( mZbuf.out() ) {
      mZbuf.compress(byte_buff(), byte_size(this->pptr() - mBuff), mOutFlush);
      this->setp(mBuff, mBuff + mSize);
    }
    return 0;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  // mBuff を zstream 用にキャストする．
  Bytef*
  byte_buff()
  {
    return reinterpret_cast<Bytef*>(mBuff);
  }

  // char_type 単位のサイズを Bytef 単位のサイズに変換する
  int
  byte_size(
    int size
  )
  {
    return size * sizeof(char_type) / sizeof(Bytef);
  }

  // Bytef 単位のサイズを char_type 単位のサイズに変換する
  int
  char_size(
    int size
  )
  {
    return size * sizeof(Bytef) / sizeof(char_type);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // zlib 圧縮を行なうバッファ
  zstream_buff mZbuf;

  // 外側の(plainな)バッファ
  char_type* mBuff;

  // mBuff のサイズ
  SizeType mSize;

  // デフォルトのバッファ
  char_type* mBuff0;

  // フラッシュフラグ
  int mOutFlush;

};


//////////////////////////////////////////////////////////////////////
/// @class basic_izstream zstream.h "zstream.h"
/// @brief zlib を使った入力ストリームのテンプレート
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_izstream :
  public std::basic_istream<_CharT, _Traits>
{
public:
  using char_type = _CharT;
  using traits_type = _Traits;
  using istream_type = std::basic_istream<char_type, traits_type>;

public:

  /// @brief コンストラクタ
  explicit
  basic_izstream(
    istream& s ///< [in] 伸長元の入力ストリーム
  ) : istream_type(new basic_zlib_streambuf<char_type, traits_type>(s))
  {
  }

  /// @brief デストラクタ
  ~basic_izstream()
  {
    delete this->rdbuf();
  }
};


//////////////////////////////////////////////////////////////////////
/// @class basic_ozstream zstream.h "zstream.h"
/// @brief zlib を使った出力ストリームのテンプレート
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_ozstream :
  public std::basic_ostream<_CharT, _Traits>
{
public:
  using char_type = _CharT;
  using traits_type = _Traits;
  using ostream_type = std::basic_ostream<char_type, traits_type>;

public:

  /// @brief コンストラクタ
  explicit
  basic_ozstream(
    ostream& s ///< [in] 圧縮前の出力ストリーム
  ) : ostream_type(new basic_zlib_streambuf<char_type, traits_type>(s))
  {
  }

  /// @brief デストラクタ
  ~basic_ozstream()
  {
    this->flush();
    delete this->rdbuf();
  }
};

using izstream = basic_izstream<char>;
using ozstream = basic_ozstream<char>;

END_NAMESPACE_YM

#endif // YM_UTILS_ZSTREAM_H
