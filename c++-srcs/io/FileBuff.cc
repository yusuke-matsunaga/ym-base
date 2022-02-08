
/// @file FileBuff.cc
/// @brief FileBuff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FileBuff.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileBuff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FileBuff::FileBuff(
  SizeType buff_size,
  bool ignore_bom
) : mBuffSize{buff_size},
    mBuff{new ymuint8[mBuffSize]}
{
  mDataSize = 0;
  mReadPos = 0;
  mWritePos = 0;
  set_first_time();
  clear_need_flush();
  if ( ignore_bom ) {
    set_ignore_bom();
  }
  else {
    clear_ignore_bom();
  }
}

// @brief デストラクタ
FileBuff::~FileBuff()
{
  close();
  delete [] mBuff;
}

// @brief ファイルをオープンする．
bool
FileBuff::open(
  const char* filename,
  int flags,
  mode_t mode
)
{
  if ( mFd != -1 ) {
    // すでにオープンしている．
    return false;
  }

#if defined(YM_WIN32)
  errno_t en = _sopen_s(&mFd, filename, flags, _SH_DENYRW, mode);
  if ( en != 0 ) {
    return false;
  }
#else
  mFd = ::open(filename, flags, mode);
  if ( mFd < 0 ) {
    return false;
  }
#endif
  if ( flags == O_RDONLY ) {
    mReadPos = 0;
  }
  else {
    mWritePos = 0;
  }
  set_first_time();
  clear_need_flush();
  return true;
}

// @brief ファイルをクローズする．
void
FileBuff::close()
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
    mFd = -1;
  }
}

// @brief buff[0] - buff[num - 1] の内容を書き込む．
void
FileBuff::write(
  const ymuint8* buff,
  SizeType num
)
{
  ASSERT_COND( is_ready() );

  // 実際に書き込んだサイズ
  SizeType count = 0;
  while ( num > 0 ) {
    // 一度に書き込めるサイズを num1 に入れる．
    SizeType num1 = std::min(num, write_size());

    // mBuff に転送する．
    bcopy(reinterpret_cast<const void*>(buff),
	  reinterpret_cast<void*>(write_ptr()),
	  num1);

    // 諸元を更新する．
    count += num1;
    buff += num1;
    num -= num1;
    write_seek(num1);
  }
}

// @brief num バイトを読み込み buff[] に格納する．
SizeType
FileBuff::read(
  ymuint8* buff,
  SizeType num,
  bool dry_run
)
{
  ASSERT_COND( is_ready() );

  // 実際に読み込んだサイズ
  SizeType count = 0;
  while ( num > 0 ) {
    // バッファに読み込む．
    fill();

    // バッファが空なら終わる．
    if ( mDataSize == 0 ) {
      break;
    }

    // 一度に読み出せるサイズを num1 に入れる．
    SizeType num1 = std::min(num, read_size());

    if ( !dry_run ) {
      // buff に転送する．
      bcopy(reinterpret_cast<const void*>(read_ptr()),
	    reinterpret_cast<void*>(buff),
	    num1);
    }

    // 諸元を更新する．
    count += num1;
    buff += num1;
    num -= num1;
    read_seek(num1);
  }

  return count;
}

// @brief バッファにデータを読みだす．
void
FileBuff::fill()
{
  if ( mReadPos == mDataSize ) {
    // バッファが空なら実際に読み込む．
#if defined(YM_WIN32)
    auto n = _read(mFd, reinterpret_cast<void*>(mBuff), mBuffSize);
#else
    auto n = ::read(mFd, reinterpret_cast<void*>(mBuff), mBuffSize);
#endif
    if ( n < 0 ) {
      throw std::system_error{std::error_code{errno, std::system_category()}, "read"};
    }

    // n == 0 の場合もこのままでよい．

    mReadPos = 0;
    mDataSize = n;

    // 先頭の BOM マークを検出し，空読みする．
    if ( is_first_time() && ignore_bom() ) {
      if ( n >= 3 ) {
	if ( mBuff[0] == 0xEF && mBuff[1] == 0xBB && mBuff[2] == 0xBF ) {
	  mReadPos = 3;
	}
      }
      clear_first_time();
    }
  }
}

// @brief バッファのデータを書き出す．
void
FileBuff::flush()
{
  SizeType tmp_size = mWritePos;
  ymuint8* tmp_buff = mBuff;
  while ( tmp_size > 0 ) {
#if defined(YM_WIN32)
    auto n = _write(mFd, reinterpret_cast<void*>(tmp_buff), tmp_size);
#else
    auto n = ::write(mFd, reinterpret_cast<void*>(tmp_buff), tmp_size);
#endif
    if ( n < 0 ) {
      // 書き込みが失敗した．
      throw std::system_error{std::error_code{errno, std::system_category()}, "write"};
    }
    tmp_buff += n;
    tmp_size -= n;
  }

  mWritePos = 0;
  clear_need_flush();
}

END_NAMESPACE_YM
