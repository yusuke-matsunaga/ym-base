﻿#ifndef FILEBUFF_H
#define FILEBUFF_H

/// @file FileBuff.h
/// @brief FileBuff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"

#include <fcntl.h>

#if defined(YM_WIN32)
#include <io.h>
typedef int mode_t;
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileBuff FileBuff.h "FileBuff.h"
/// @brief C の標準ライブラリの FILE 構造体を模したクラス
//////////////////////////////////////////////////////////////////////
class FileBuff
{
public:

  /// @brief コンストラクタ
  /// @param[in] buff_size バッファサイズ
  FileBuff(int buff_size = 4096);

  /// @brief デストラクタ
  ~FileBuff();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @param[in] flags フラグ
  /// @param[in] mode モード
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  /// @note open() システムコールのラッパ
  bool
  open(const char* filename,
       int flags,
       mode_t mode = 0);

  /// @brief ファイルをクローズする．
  /// @note 基本的には close() システムコールを呼ぶだけだが，
  /// 未処理のデータが残っていたら書き出す．
  void
  close();

  /// @brief 適正なファイルディスクリプタを持っているとき true を返す．
  bool
  is_ready() const;

  /// @brief buff[0] - buff[num - 1] の内容を書き込む．
  /// @param[in] buff データを格納したバッファ
  /// @param[in] num 書き込むバイト数
  /// @return 実際に書き込んだバイト数を返す．
  int
  write(const ymuint8* buff,
	int num);

  /// @brief num バイトを読み込み buff[] に格納する．
  /// @param[in] buff データを格納するバッファ
  /// @param[in] num 読み込むバイト数．
  /// @return 実際に読み込んだバイト数を返す．
  int
  read(ymuint8* buff,
       int num);

  /// @brief num バイトを読み込む
  /// @note ただし読み込んだデータは捨てる．
  /// @param[in] num 読み込むバイト数．
  /// @return 実際に読み込んだバイト数を返す．
  int
  dummy_read(int num);

  /// @brief バッファにデータを読みだす．
  bool
  prepare();

  /// @brief バッファのデータを書き出す．
  bool
  flush();

  /// @brief バッファの現在位置を返す．
  ymuint8*
  buff_ptr() const;

  /// @brief バッファのサイズを返す．
  /// @note 読み出しモードの場合にはバッファ中のデータ量を返す．
  /// @note 書き込みモードの場合にはバッファの空き容量を返す．
  int
  buff_size() const;

  /// @brief バッファの最終位置を進める．
  void
  seek(int num);


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルディスクリプタ
  int mFd;

  // 最初の読み出しであることを示すフラグ
  bool mFirstTime;

  // flush が必要なことを示すフラグ
  bool mNeedFlush;

  // バッファサイズ
  int mBuffSize;

  // バッファ
  ymuint8* mBuff;

  // 有効なデータのサイズ ( <= mBuffSize )
  int mDataSize;

  // 読み出し/書き込み位置
  int mPos;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] buff_size バッファサイズ
inline
FileBuff::FileBuff(int buff_size)
{
  mFd = -1;
  mBuffSize = buff_size;
  ASSERT_COND( mBuffSize > 0 );

  mBuff = new ymuint8[mBuffSize];
  mDataSize = 0;
  mPos = mBuffSize;
  mFirstTime = true;
  mNeedFlush = false;
}

// @brief デストラクタ
inline
FileBuff::~FileBuff()
{
  close();
  delete [] mBuff;
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @param[in] flags フラグ
// @param[in] mode モード
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
inline
bool
FileBuff::open(const char* filename,
	       int flags,
	       mode_t mode)
{
  close();
#if defined(YM_WIN32)
  errno_t en = _sopen_s(&mFd, filename, flags, _SH_DENYRW, mode);
  if ( en != 0 ) {
    return false;
  }
#else
  mFd = ::open(filename, flags, mode);
#endif
  if ( flags == O_RDONLY ) {
    mPos = mBuffSize;
  }
  else {
    mPos = 0;
  }
  mDataSize = mBuffSize;
  mFirstTime = true;
  mNeedFlush = false;
  return mFd >= 0;
}

// @brief ファイルをクローズする．
inline
void
FileBuff::close()
{
  if ( mFd >= 0 ) {
    if ( mNeedFlush ) {
      flush();
    }
#if defined(YM_WIN32)
    _close(mFd);
#else
    ::close(mFd);
#endif
  }
  mFd = -1;
  mNeedFlush = false;
}

// @brief 適正なファイルディスクリプタを持っているとき true を返す．
inline
bool
FileBuff::is_ready() const
{
  return mFd >= 0;
}

// @brief バッファのデータを書き出す．
inline
bool
FileBuff::flush()
{
#if defined(YM_WIN32)
  int n = _write(mFd, mBuff, static_cast<ymuint>(mPos));
#else
  int n = ::write(mFd, mBuff, mPos);
#endif
  if ( n < mPos ) {
    // 書き込みが失敗した．
    return false;
  }
  mPos = 0;
  mNeedFlush = false;
  return true;
}

// @brief バッファの現在位置を返す．
inline
ymuint8*
FileBuff::buff_ptr() const
{
  return mBuff + mPos;
}

// @brief バッファのサイズを返す．
// @note 読み出しモードの場合にはバッファ中のデータ量を返す．
// @note 書き込みモードの場合にはバッファの空き容量を返す．
inline
int
FileBuff::buff_size() const
{
  return mDataSize - mPos;
}

// @brief バッファの最終位置を進める．
inline
void
FileBuff::seek(int num)
{
  ASSERT_COND( mPos + num <= mDataSize );

  mPos += num;
}

END_NAMESPACE_YM

#endif // FILEBUFF_H
