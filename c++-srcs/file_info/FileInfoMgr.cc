
/// @file FileInfoMgr.cc
/// @brief FileInfoMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "FileInfoMgr.h"
#include "ym/FileLoc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileInfoMgr
//////////////////////////////////////////////////////////////////////

// @brief ファイル名を返す．
std::string
FileInfoMgr::filename(
  int id
)
{
  ASSERT_COND( id >= 0 && id < mFiArray.size() );

  const _FileInfo& fi = mFiArray[id];
  return fi.mFileName;
}

// @brief インクルード元のファイル位置を返す．
FileLoc
FileInfoMgr::parent_loc(
  int id
)
{
  ASSERT_COND( id >= 0 && id < mFiArray.size() );

  const _FileInfo& fi = mFiArray[id];
  return fi.mParentLoc;
}

// @brief 新しい _FileInfo を生成する．
int
FileInfoMgr::new_file_info(
  const char* filename
)
{
  int id = mFiArray.size();
  mFiArray.push_back(_FileInfo(filename));
  return id;
}

// @brief 新しい _FileInfo を生成する．
int
FileInfoMgr::new_file_info(
  const char* filename,
  const FileLoc& parent_loc
)
{
  int id = mFiArray.size();
  mFiArray.push_back(_FileInfo(filename, parent_loc));
  return id;
}

END_NAMESPACE_YM
