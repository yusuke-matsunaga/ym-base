#ifndef CODECENGINE_H
#define CODECENGINE_H

/// @file CodecEngine.h
/// @brief CodecEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class CodecEngine CodecEngine.h "CodecEngine.h"
/// @brief 圧縮/伸張を行なうエンジンのインターフェイスを定義する純粋仮想基底クラス
//////////////////////////////////////////////////////////////////////
class CodecEngine
{
public:

  /// @brief 伸張用のコンストラクタ
  CodecEngine(
    istream* is,              ///< [in] 入力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  );

  /// @brief 圧縮用のコンストラクタ
  CodecEngine(
    ostream* os,              ///< [in] 出力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  );

  /// @brief デストラクタ
  virtual
  ~CodecEngine() = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出しモードの時 true を返す．
  bool
  read_mode() const
  {
    return mInStream != nullptr;
  }

  /// @brief 伸張したデータを読み出す．
  /// @return 読み出したデータサイズを返す．
  ///
  /// エラーの場合には例外が送出される．
  virtual
  SizeType
  read(
    ymuint8* buff, ///< [in] 読み出したデータを格納するバッファ
    SizeType size  ///< [in] 読み出すデータの最大サイズ(<= buff_size)
  ) = 0;

  /// @brief 書き込みモードの時 true を返す．
  bool
  write_mode() const
  {
    return mOutStream != nullptr;
  }

  /// @brief データを圧縮して書き込む．
  ///
  /// エラーの場合には例外が送出される．
  virtual
  void
  write(
    const ymuint8* buff, ///< [in] 入力データのバッファ
    SizeType size        ///< [in] データサイズ(<= buff_size)
  ) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 生のデータを読み出す．
  /// @return 実際に読み出したサイズを返す．
  ///
  /// buff が nullptr の場合は空読みする．
  SizeType
  raw_read(
    ymuint8* buff, ///< [in] 読み出したデータを格納するバッファ
    SizeType size  ///< [in] 読み出すデータの最大サイズ
  );

  /// @brief 生のデータを書き込む．
  void
  raw_write(
    const ymuint8* buff, ///< [in] 入力データのバッファ
    SizeType size        ///< [in] データサイズ(<= buff_size)
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バッファ中の読み出し位置を返す．
  ymuint8*
  read_ptr() const
  {
    return mBuff + mPos;
  }

  /// @brief バッファ中の読み出し可能なサイズを返す．
  SizeType
  read_size() const
  {
    return mDataSize - mPos;
  }

  /// @brief バッファ中の読み出し位置を進める．
  void
  read_seek(
    SizeType n ///< [in] 移動量
  )
  {
    ASSERT_COND( mPos + n <= mDataSize );
    mPos += n;
    if ( mPos == mDataSize ) {
      read_fill();
    }
  }

  /// @brief バッファ中の書き込み位置を返す．
  ymuint8*
  write_ptr() const
  {
    return mBuff + mPos;
  }

  /// @brief バッファ中の書き込み可能なサイズを返す．
  SizeType
  write_size() const
  {
    return mBuffSize - mPos;
  }

  /// @brief バッファ中の書き込み位置を進める．
  void
  write_seek(
    SizeType n ///< [in] 移動量
  )
  {
    ASSERT_COND( mPos + n <= mBuffSize );
    mPos += n;
    if ( mPos == mBuffSize ) {
      write_flush();
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バッファにデータを読み出す．
  void
  read_fill()
  {
    ASSERT_COND( mPos == mDataSize );
    mInStream->read(reinterpret_cast<char*>(mBuff), mBuffSize);
    mDataSize = mInStream->gcount();
    mPos = 0;
  }

  /// @brief バッファのデータを書き込む．
  void
  write_flush()
  {
    if ( mPos > 0 ) {
      mOutStream->write(reinterpret_cast<const char*>(mBuff), mPos);
    }
    mPos = 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream* mInStream{nullptr};

  // 出力ストリーム
  ostream* mOutStream{nullptr};

  // バッファサイズ
  SizeType mBuffSize;

  // バッファ
  ymuint8* mBuff{nullptr};

  // 読み込んだデータサイズ
  SizeType mDataSize{0};

  // 読み込み/書き出し位置
  SizeType mPos{0};

};

END_NAMESPACE_YM

#endif // CODECENGINE_H
