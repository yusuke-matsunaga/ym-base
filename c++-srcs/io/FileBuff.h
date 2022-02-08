#ifndef FILEBUFF_H
#define FILEBUFF_H

/// @file FileBuff.h
/// @brief FileBuff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014, 2022 Yusuke Matsunaga
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
///
/// 具体的にはファイルディスクリプタとバッファを持つ．
/// MicroSoft のアプリは UTF-8 の場合でもファイルの先頭にBOMマークを
/// 付加するのでそれを読み飛ばす機能を持つ．
/// この機能はコンストラクタの引数で無効化することもできる．
//////////////////////////////////////////////////////////////////////
class FileBuff
{
public:

  /// @brief コンストラクタ
  FileBuff(
    SizeType buff_size = 4096, ///< [in] バッファサイズ
    bool ignore_bom = true     ///< [in] 先頭の BOM マークを読み飛ばすフラグ
  );

  /// @brief デストラクタ
  ~FileBuff();


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
    mode_t mode = 0666    ///< [in] モード
  );

  /// @brief ファイルをオープンする．
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  ///
  /// open() システムコールのラッパ
  bool
  open(
    const string& filename, ///< [in] ファイル名
    int flags,              ///< [in] フラグ
    mode_t mode = 0666      ///< [in] モード
  )
  {
    return open(filename.c_str(), flags, mode);
  }

  /// @brief ファイルをクローズする．
  ///
  /// 基本的には close() システムコールを呼ぶだけだが，
  /// 未処理のデータが残っていたら書き出す．
  void
  close();

  /// @brief 適正なファイルディスクリプタを持っているとき true を返す．
  bool
  is_ready() const
  {
    return mFd >= 0;
  }

  /// @brief buff[0] - buff[num - 1] の内容を書き込む．
  ///
  /// 書き込みが失敗した場合は system_error 例外が送出される．
  void
  write(
    const ymuint8* buff, ///< [in] データを格納したバッファ
    SizeType num         ///< [in] 書き込むバイト数
  );

  /// @brief num バイトを読み込み buff[] に格納する．
  /// @return 読み込んだバイト数を返す．
  ///
  /// 読み込みが失敗した場合は system_error 例外が創出される．
  /// 実際に読み込まれるサイズは num よりも小さい場合がある．
  SizeType
  read(
    ymuint8* buff,       ///< [in] データを格納するバッファ
    SizeType num,        ///< [in] 読み込むバイト数．
    bool dry_run = false ///< [in] 空読みする時に true にする．
  );

  /// @brief num バイトを空読みする．
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  ///
  /// きっちり num バイト読み込んだ場合だけ成功する．
  bool
  dummy_read(
    SizeType num ///< [in] 読み込むバイト数．
  )
  {
    auto n = read(nullptr, num, true);
    return n == num;
  }

  /// @brief バッファにデータを読みだす．
  ///
  /// @exception system_error
  /// - バッファにデータがある場合には何もしない．
  /// - buff_ptr() と buff_size() が更新される．
  void
  fill();

  /// @brief バッファのデータを書き出す．
  void
  flush();

  /// @brief バッファの読み出し位置を返す．
  ymuint8*
  read_ptr() const
  {
    return mBuff + mReadPos;
  }

  /// @brief バッファの書き込み位置を返す．
  ymuint8*
  write_ptr() const
  {
    return mBuff + mWritePos;
  }

  /// @brief 読み出し可能なデータサイズを返す．
  SizeType
  read_size() const
  {
    return mDataSize - mReadPos;
  }

  /// @brief 書き込み可能なデータサイズを返す．
  SizeType
  write_size() const
  {
    return mBuffSize - mWritePos;
  }

  /// @brief 読み出し位置を進める．
  void
  read_seek(
    SizeType num ///< [in] 移動量
  )
  {
    ASSERT_COND( mReadPos + num <= mDataSize );

    mReadPos += num;
  }

  /// @brief 書き込み位置を進める．
  void
  write_seek(
    SizeType num ///< [in] 移動量
  )
  {
    ASSERT_COND( mWritePos + num <= mBuffSize );

    mWritePos += num;
    if ( mWritePos == mBuffSize ) {
      flush();
    }
    else {
      set_need_flush();
    }
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

  /// @brief 先頭のBOMマークを読み飛ばす．
  bool
  ignore_bom() const
  {
    return mFlags[2];
  }

  /// @brief ignore_bom フラグを立てる．
  void
  set_ignore_bom()
  {
    mFlags.set(2);
  }

  /// @brief ignore_bom フラグを下ろす．
  void
  clear_ignore_bom()
  {
    mFlags.reset(2);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイルディスクリプタ
  int mFd{-1};

  // いくつかのフラグ
  // 0: 最初の読み出しであることを示すフラグ
  // 1: flush が必要なことを示すフラグ
  // 2: BOM 読み飛ばし
  bitset<3> mFlags{0};

  // バッファサイズ
  SizeType mBuffSize;

  // バッファ
  ymuint8* mBuff;

  // 有効なデータのサイズ ( <= mBuffSize )
  SizeType mDataSize;

  // 読み出し位置
  SizeType mReadPos;

  // 書き込み位置
  SizeType mWritePos;

};

END_NAMESPACE_YM

#endif // FILEBUFF_H
