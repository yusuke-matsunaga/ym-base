
/// @file CodecType.cc
/// @brief CodecType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ym/CodecType.h"
#include "CodecTypeConf.h"


BEGIN_NAMESPACE_YM

// @brief 実際に利用可能な codec type のリストを返す関数
vector<CodecType>
get_codec_type_list()
{
  vector<CodecType> tmp_list;
  tmp_list.reserve(5);

  tmp_list.push_back(kCodecThrough);
  tmp_list.push_back(kCodecZ);

#if defined(ZLIB_FOUND)
  tmp_list.push_back(kCodecGzip);
#endif

#if defined(BZIP2_FOUND)
  tmp_list.push_back(kCodecBzip2);
#endif

#if defined(LIBLZMA_FOUND)
  tmp_list.push_back(kCodecLzma);
#endif

  return tmp_list;
}

END_NAMESPACE_YM
