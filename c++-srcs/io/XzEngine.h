#ifndef XZENGINE_H
#define XZENGINE_H

/// @file XzEngine.h
/// @brief XzEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/CodecEngine.h"
#include "ym/CodecGen.h"
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
/// @class XzEngine XzEngine.h "XzEngine.h"
/// @brief lzma 圧縮/伸張を行なうクラス
//////////////////////////////////////////////////////////////////////
class XzEngine :
  public CodecEngine
{
public:

  /// @brief 伸張用のコンストラクタ(初期化パラメータ付き)
  XzEngine(
    istream& is,                         ///< [in] 入力ストリーム
    SizeType buff_size = 4096,           ///< [in] バッファサイズ
    const XzEngineGen::Param& param = {} ///< [in] 初期化パラメータ
  );

  /// @brief 圧縮用のコンストラクタ(初期化パラメーター付き)
  XzEngine(
    ostream& os,                         ///< [in] 出力ストリーム
    SizeType buff_size = 4096,           ///< [in] バッファサイズ
    const XzEngineGen::Param& param = {} ///< [in] 初期化パラメータ
  );

  /// @brief デストラクタ
  ~XzEngine();


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

  /// @brief 圧縮用の初期化
  void
  deflate_init(
    int preset,      ///< [in] 圧縮レベル ( 0 - 9: 6 がデフォルト )
    lzma_check check ///< [in] 検査方法
                     ///<  - LZMA_CHECK_NONE
                     ///<  - LZMA_CHECK_CRC32
                     ///<  - LZMA_CHECK_CRC64
                     ///<  - LZMA_CHECK_SHA256
  );

  /// @brief deflate の終了処理
  void
  deflate_end();

  /// @brief deflate の共通処理
  lzma_ret
  deflate_common(
    lzma_action flash
  );

  /// @brief 伸張用の初期化
  void
  inflate_init(
    SizeType memlimit, ///< [in] 割り当てるメモリの上限
    ymuint32 flags     ///< [in] 動作制御用のフラグ
  );

  /// @brief 入力バッファを設定する．
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

#endif // XZENGINE_H
