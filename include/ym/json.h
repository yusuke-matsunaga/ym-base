#ifndef JSON_H
#define JSON_H

/// @file json.h
/// @brief json の名前空間定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief json 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_JSON \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsJson)

/// @brief json 用の名前空間の終了
#define END_NAMESPACE_YM_JSON \
END_NAMESPACE(nsJson) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_JSON

class JsonValue;

END_NAMESPACE_YM_JSON

BEGIN_NAMESPACE_YM

using nsJson::JsonValue;

END_NAMESPACE_YM

#endif // JSON_H
