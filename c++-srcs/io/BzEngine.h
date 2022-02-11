#ifndef BZENGINE_H
#define BZENGINE_H

/// @file BzEngine.h
/// @brief BzEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CodecEngine.h"
#include <bzlib.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BzError BzEngine.h "BzEngine.h"
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
/// @class BzEngine BzEngine.h "BzEngine.h"
/// @brief
//////////////////////////////////////////////////////////////////////
class BzEngine :
  public CodecEngine
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

  /// @brief 伸張用のコンストラクタ
  BzEngine(
    istream* is,               ///< [in] 入力ストリーム
    SizeType buff_size = 4096, ///< [in] バッファサイズ
    int verbosity = 0,         ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int small = 0,             ///< [in] 伸張アルゴリズム用のパラメータ ( 0 か 非0 )
    alloc_func af = nullptr,   ///< [in] alloc 関数
    free_func ff = nullptr,    ///< [in] free 関数
    void* op = nullptr         ///< [in] opaque オブジェクト
  );

  /// @brief コンストラクタ
  BzEngine(
    ostream* os,               ///< [in] 出力ストリーム
    SizeType buff_size = 4096, ///< [in] バッファサイズ
    int block_size_100k = 9,   ///< [in] 作業用のメモリサイズ ( 0 - 9 )
    int verbosity = 0,         ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int work_factor = 0,       ///< [in] 圧縮アルゴリズム用のパラメータ ( 0 - 250 )
    alloc_func af = nullptr,   ///< [in] alloc 関数
    free_func ff = nullptr,    ///< [in] free 関数
    void* op = nullptr         ///< [in] opaque オブジェクト
  );

  /// @brief デストラクタ
  ~BzEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief データを伸長して読み出す．
  /// @return 読み出したデータサイズを返す．
  ///
  /// エラーの場合には例外が送出される．
  SizeType
  read(
    ymuint8* buff, ///< [in] 読み出したデータを格納するバッファ
    SizeType size  ///< [in] 読み出すデータの最大サイズ
  ) override;

  /// @brief データを圧縮して書き込む．
  ///
  /// エラーの場合には例外が送出される．
  void
  write(
    const ymuint8* buff, ///< [in] 入力データのバッファ
    SizeType size        ///< [in] データサイズ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮用の初期化を行う．
  void
  deflate_init(
    int block_size_100k = 9, ///< [in] 作業用のメモリサイズ ( 1 - 9 )
    int verbosity       = 0, ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int work_factor     = 0  ///< [in] 圧縮アルゴリズム用のパラメータ ( 0 - 250 )
  );

  /// @brief 圧縮を行う．
  int
  deflate_common(
    int action ///< [in] 動作コード
               ///<  - BZ_RUN
               ///<  - BZ_FLUSH
               ///<  - BZ_FINISH
  );

  /// @brief 圧縮を終了する．
  void
  deflate_end();

  /// @brief 伸張用のの初期化を行う．
  void
  inflate_init(
    int verbosity, ///< [in] デバッグ用の出力レベル ( 0 - 4 )
    int small      ///< [in] 伸張アルゴリズム用のパラメータ ( 0 か 非0 )
  );

  /// @brief 伸張を終了する．
  void
  inflate_end();

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

};

END_NAMESPACE_YM

#endif // BZENGINE_H
