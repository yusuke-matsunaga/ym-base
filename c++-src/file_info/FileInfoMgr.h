#ifndef FILEINFOMGR_H
#define FILEINFOMGR_H

/// @file FileInfoMgr.h
/// @brief FileInfoMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/FileLoc.h"
#include "ym/StrBuff.h"


BEGIN_NAMESPACE_YM

class FileInfo;
class FileRegion;

//////////////////////////////////////////////////////////////////////
/// @class FileInfoMgr FileInfoMgr.h "FileInfoMgr.h"
/// @brief FileInfoMgr の実際の処理を行うクラス
/// @sa FileInfo FileLoc FileRegion
//////////////////////////////////////////////////////////////////////
class FileInfoMgr
{
public:

  /// @brief コンストラクタ
  FileInfoMgr() = default;

  /// @brief デストラクタ
  ///
  /// このクラスで生成したデータはすべて削除される．
  ~FileInfoMgr() = default;


public:

  /// @brief クリアする
  void
  clear()
  {
    mFiArray.clear();
  }

  /// @brief 新しい _FileInfo を生成する．
  /// @return 生成された _FileInfo の ID 番号
  int
  new_file_info(const char* filename); ///< [in] ファイル名

  /// @brief 新しい _FileInfo を生成する．
  /// @return 生成された _FileInfo の ID 番号
  int
  new_file_info(const char* filename,       ///< [in] ファイル名
		const FileLoc& parent_loc); ///< [in] インクルード元の親ファイルの情報


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル名を返す．
  string
  filename(int id); ///< [in] _FileInfo の ID 番号

  /// @brief インクルード元のファイル位置を返す．
  /// @note インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc(int id); ///< [in] _FileInfo の ID 番号


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル情報の実体
  struct _FileInfo
  {
    /// @brief 空のコンストラクタ
    _FileInfo() = default;

    /// @brief 親のない場合のコンストラクタ
    _FileInfo(const char* filename) ///< [in] ファイル名
      : mFileName{filename}
    {
    }

    /// @brief 親のある場合(インクルードされている場合)のコンストラクタ
    _FileInfo(const char* filename,    ///< [in] ファイル名
	      const FileLoc& file_loc) ///< [in] インクルード元のファイル位置
      : mFileName{filename},
	mParentLoc{file_loc}
    {
    }

    /// @brief ファイル名
    StrBuff mFileName;

    /// @brief インクルード元の情報
    /// インクルードされていない場合には無効な FileInfo が入っている．
    FileLoc mParentLoc;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FileInfo の配列
  vector<_FileInfo> mFiArray;

};

END_NAMESPACE_YM

#endif // FILEINFOMGR_H
