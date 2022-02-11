#ifndef GZENGINE_H
#define GZENGINE_H

/// @file GzEngine.h
/// @brief GzEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CodecEngine.h"
#include <zlib.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GzError GzEngine.h "GzEngine.h"
/// @brief z_stream のエラーを表すクラス
//////////////////////////////////////////////////////////////////////
class GzError
{
public:

  /// @brief コンストラクタ
  GzError(
    int ret,        ///< [in] リターンコード
    const char* msg ///< [in] メッセージ
  ) : mRet{ret},
      mMsg{msg}
  {
  }

  /// @brief デストラクタ
  ~GzError() = default;


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
/// @class GzEngine GzEngine.h "GzEngine.h"
/// @brief gzip 形式の圧縮/伸張器
//////////////////////////////////////////////////////////////////////
class GzEngine :
  public CodecEngine
{
public:

  /// @brief 伸張用のコンストラクタ
  GzEngine(
    istream* is,               ///< [in] 入力ストリーム
    SizeType buff_size = 4096, ///< [in] バッファサイズ
    alloc_func af = Z_NULL,    ///< [in] alloc 関数
    free_func ff = Z_NULL,     ///< [in] free 関数
    voidp op = Z_NULL          ///< [in] opaque オブジェクト
  );

  /// @brief 圧縮用のコンストラクタ
  GzEngine(
    ostream* os,               ///< [in] 出力ストリーム
    SizeType buff_size = 4096, ///< [in] バッファサイズ
    alloc_func af = Z_NULL,    ///< [in] alloc 関数
    free_func ff = Z_NULL,     ///< [in] free 関数
    voidp op = Z_NULL          ///< [in] opaque オブジェクト
  );

  /// @brief デストラクタ
  ~GzEngine();


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

  /// @brief deflate の初期化
  void
  deflate_init();

  /// @brief deflate の終了処理
  void
  deflate_end();

  /// @brief deflate の共通処理
  int
  deflate_common(
    int flash
  );

  /// @brief inflate の初期化
  void
  inflate_init();

  /// @brief inflate の終了処理
  void
  inflate_end();

  /// @brief 入力バッファを設定する．
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

  /// @brief 読み出せるデータのバイト数を返す．
  SizeType
  avail_in()
  {
    return mZ.avail_in;
  }

  /// @brief 書き込めるデータのバイト数を返す．
  SizeType
  avail_out()
  {
    return mZ.avail_out;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の z_stream
  z_stream mZ;

  // CRC コード
  ymuint32 mCRC;

  // 出力されたデータサイズ
  SizeType mOutSize;

};

END_NAMESPACE_YM

#endif // GZENGINE_H
