﻿#ifndef YM_CODECTYPE_H
#define YM_CODECTYPE_H

/// @file ym/CodecType.h
/// @brief CodecType の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief FileCoder/FileDecoder の種類
//////////////////////////////////////////////////////////////////////
enum class CodecType {
  /// @brief 無変換
  Through,
  /// @brief compress (Z)
  Z,
  /// @brief gzip
  Gzip,
  /// @brief bzip2
  Bzip2,
  /// @brief lzma (xz)
  Lzma
};

/// @relates CodecType
/// @brief 実際に利用可能な codec type のリストを返す関数
///
/// Through, Z 以外は対象のライブラリが見つからなければ存在しない．
extern
vector<CodecType> get_codec_type_list();

END_NAMESPACE_YM

#endif // YM_CODECTYPE_H
