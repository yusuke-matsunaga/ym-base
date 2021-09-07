#ifndef FILEBUFF_H
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
  FileBuff(
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  )
  {
    mFd = -1;
    mBuffSize = buff_size;
    ASSERT_COND( mBuffSize > 0 );

    mBuff = new ymuint8[mBuffSize];
    mDataSize = 0;
    mPos = mBuffSize;
    set_first_time();
    clear_need_flush();
  }

  /// @brief デストラクタ
  ~FileBuff()
  {
    close();
    delete [] mBuff;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルをオープンする．
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  ///
  /// open() システムコールのラッパ
  bool
  open(
    const char* filename, ///< [in] ファイル名
    int flags,            ///< [in] フラグ
    mode_t mode = 0       ///< [in] モード
  )
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
    mFlags.set(0);
    clear_need_flush();
    return mFd >= 0;
  }

  /// @brief ファイルをクローズする．
  ///
  /// 基本的には close() システムコールを呼ぶだけだが，
  /// 未処理のデータが残っていたら書き出す．
  void
  close()
  {
    if ( mFd >= 0 ) {
      if ( need_flush() ) {
	flush();
      }
#if defined(YM_WIN32)
      _close(mFd);
#else
      ::close(mFd);
#endif
    }
    mFd = -1;
  }

  /// @brief 適正なファイルディスクリプタを持っているとき true を返す．
  bool
  is_ready() const
  {
    return mFd >= 0;
  }

  /// @brief buff[0] - buff[num - 1] の内容を書き込む．
  /// @return 実際に書き込んだバイト数を返す．
  SizeType
  write(
    const ymuint8* buff, ///< [in] データを格納したバッファ
    SizeType num         ///< [in] 書き込むバイト数
  );

  /// @brief num バイトを読み込み buff[] に格納する．
  /// @return 実際に読み込んだバイト数を返す．
  SizeType
  read(
    ymuint8* buff, ///< [in] データを格納するバッファ
    SizeType num   ///< [in] 読み込むバイト数．
  );

  /// @brief num バイトを読み込む
  /// @return 実際に読み込んだバイト数を返す．
  ///
  /// ただし読み込んだデータは捨てる．
  SizeType
  dummy_read(
    SizeType num ///< [in] 読み込むバイト数．
  );

  /// @brief バッファにデータを読みだす．
  bool
  prepare();

  /// @brief バッファのデータを書き出す．
  bool
  flush()
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
    clear_need_flush();
    return true;
  }

  /// @brief バッファの現在位置を返す．
  ymuint8*
  buff_ptr() const
  {
    return mBuff + mPos;
  }

  /// @brief バッファのサイズを返す．
  /// @note 読み出しモードの場合にはバッファ中のデータ量を返す．
  /// @note 書き込みモードの場合にはバッファの空き容量を返す．
  SizeType
  buff_size() const
  {
    return mDataSize - mPos;
  }

  /// @brief バッファの最終位置を進める．
  void
  seek(
    SizeType num ///< [in] 指定する位置
  )
  {
    ASSERT_COND( mPos + num <= mDataSize );

    mPos += num;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最初の読み出しの時 true を返す．
  bool
  is_first_time() const
  {
    return mFlags[0];
  }

  /// @brief first_time フラグを立てる．
  void
  set_first_time()
  {
    mFlags.set(0);
  }

  /// @brief first_time フラグを降ろす．
  void
  clear_first_time()
  {
    mFlags.reset(0);
  }

  /// @brief flush が必要な時 true を返す．
  bool
  need_flush() const
  {
    return mFlags[1];
  }

  /// @brief flush フラグを立てる．
  void
  set_need_flush()
  {
    mFlags.set(1);
  }

  /// @brief flush フラグを降ろす．
  void
  clear_need_flush()
  {
    mFlags.reset(1);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルディスクリプタ
  int mFd;

  // いくつかのフラグ
  // 0: 最初の読み出しであることを示すフラグ
  // 1: flush が必要なことを示すフラグ
  bitset<2> mFlags;

  // バッファサイズ
  SizeType mBuffSize;

  // バッファ
  ymuint8* mBuff;

  // 有効なデータのサイズ ( <= mBuffSize )
  SizeType mDataSize;

  // 読み出し/書き込み位置
  SizeType mPos;

};

END_NAMESPACE_YM

#endif // FILEBUFF_H
