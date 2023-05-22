#ifndef YM_FILEINFO_H
#define YM_FILEINFO_H

/// @file ym/FileInfo.h
/// @brief FinleInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

class FileLoc;

//////////////////////////////////////////////////////////////////////
/// @class FileInfo FileInfo.h "ym/FileInfo.h"
/// @brief ファイルの情報を表すクラス
///
/// 具体的には
/// - ファイル名
/// - インクルード元のファイル位置
/// の情報を持つ．
/// 実際には 16 ビットの整数のみを持つというトリッキーな実装
///
/// 本当のデータは FileInfoMgr という静的オブジェクトが持つ．
/// @sa FileInfoMgr
//////////////////////////////////////////////////////////////////////
class FileInfo
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 無効な ID で初期化される．
  FileInfo() = default;

  /// @brief ファイル名を指定したコンストラクタ
  FileInfo(const char* filename); ///< [in] ファイル名

  /// @brief ファイル名を指定したコンストラクタ
  FileInfo(const string& filename); ///< [in] ファイル名

  /// @brief ファイル名とインクルード元の親ファイルの情報
  FileInfo(const char* filename,       ///< [in] ファイル名
	   const FileLoc& parent_loc); ///< [in] インクルード元の親ファイルの情報

  /// @brief ファイル名とインクルード元の親ファイルの情報
  FileInfo(const string& filename,     ///< [in] ファイル名
	   const FileLoc& parent_loc); ///< [in] インクルード元の親ファイルの情報

  /// @brief デストラクタ
  ~FileInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部に公開している関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 有効なデータを持っているときに true を返す．
  bool
  is_valid() const
  {
    return mId != 0xFFFF;
  }

  /// @brief ID番号を得る．
  ///
  /// ほとんど等価比較演算子のための関数
  int
  id() const
  {
    return static_cast<int>(mId);
  }

  /// @brief ファイル名を返す．
  string
  filename() const;

  /// @brief インクルード元のファイル位置を返す．
  ///
  /// インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc() const;

  /// @brief インクルード元のファイル位置の情報のリスト(vector)を返す．
  ///
  /// トップレベルのファイルが先頭になる．
  vector<FileLoc>
  parent_loc_list() const;

  /// @brief 内部の静的なデータをクリアする．
  static
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  std::uint16_t mId{0xFFFFU};

};


//////////////////////////////////////////////////////////////////////
/// @name FileInfo に関係する演算子
/// @{

/// @relates FileInfo
/// @brief 等価比較演算子
inline
bool
operator==(const FileInfo& left,  ///< [in] 左のオペランド
	   const FileInfo& right) ///< [in] 右のオペランド
{
  return left.id() == right.id();
}

/// @relates FileInfo
/// @brief 非等価比較演算子
inline
bool
operator!=(const FileInfo& left,  ///< [in] 左のオペランド
	   const FileInfo& right) ///< [in] 右のオペランド
{
  return !operator==(left, right);
}

/// @relates FileInfo
/// @brief FileInfo 用のストリーム出力演算子
/// @return s をそのまま返す
ostream&
operator<<(ostream& s,                 ///< [in] 出力ストリーム
	   const FileInfo& file_info); ///< [in] ファイル情報

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM

#endif // YM_FILEINFO_H
