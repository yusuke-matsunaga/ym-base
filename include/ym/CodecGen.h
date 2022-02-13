#ifndef CODECGEN_H
#define CODECGEN_H

/// @file CodecEngine.h
/// @brief CodecEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "CodecTypeConf.h"

#if defined(BZIP2_FOUND)
#include <bzlib.h>
#endif // defined(BZIP2_FOUND)
#if defined(ZLIB_FOUND)
#include <zlib.h>
#endif // defined(ZLIB_FOUND)
#if defined(LIBLZMA_FOUND)
#include <lzma.h>
#endif // defined(LIBLZMA_FOUND)


BEGIN_NAMESPACE_YM

// クラス名の前方宣言
class CodecEngine;

//////////////////////////////////////////////////////////////////////
/// @class CodecGen CodecEngine.h "ym/CodecEngine.h"
/// @brief 圧縮/伸張器のファクトリクラス
///
/// このクラスではインターフェイスの定義のみ行なう．
/// 具体的な内容は派生クラスで指定する．
//////////////////////////////////////////////////////////////////////
class CodecGen
{
public:

  /// @brief デストラクタ
  virtual
  ~CodecGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // エンジンを生成するインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 伸張用のエンジンを作る．
  virtual
  CodecEngine*
  new_engine(
    istream& is,              ///< [in] 入力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const = 0;

  /// @brief 圧縮用のエンジンを作る．
  virtual
  CodecEngine*
  new_engine(
    ostream& os,              ///< [in] 出力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ZEngineGen CodecEngine.h "ym/CodecEngine.h"
/// @brief Z(compress) 用の codec encing を生成するクラス
//////////////////////////////////////////////////////////////////////
class ZEngineGen :
  public CodecGen
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @brief Z 用の初期化パラメータ
  //////////////////////////////////////////////////////////////////////
  struct Param
  {
    int level{0}; ///< 圧縮レベル
  };


public:

  /// @brief コンストラクタ
  ZEngineGen(
    const Param& param = Param{0} ///< [in] 初期化パラメータ
  ) : mParam{param}
  {
  }

  /// @brief デストラクタ
  ~ZEngineGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 伸張用のエンジンを作る．
  CodecEngine*
  new_engine(
    istream& is,              ///< [in] 入力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const override;

  /// @brief 圧縮用のエンジンを作る．
  CodecEngine*
  new_engine(
    ostream& os,              ///< [in] 出力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化パラメータ
  Param mParam;

};


#if defined(BZIP2_FOUND)
//////////////////////////////////////////////////////////////////////
/// @class BzEngineGen CodecEngine.h "ym/CodecEngine.h"
/// @brief bzip2 用の codec engine を生成するクラス
//////////////////////////////////////////////////////////////////////
class BzEngineGen :
  public CodecGen
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @brief Bzip2 の初期化パラメータ
  //////////////////////////////////////////////////////////////////////
  struct Param
  {
    int verbosity{0};                              ///< デバッグ用の出力レベル ( 0 - 4 )
    int small{0};                                  ///< 伸張アルゴリズム用のパラメータ ( 0 か 非0 )
    int block_size_100k{9};                        ///< 作業用のメモリサイズ ( 1 - 9 )
    int work_factor{0};                            ///< 圧縮アルゴリズム用のパラメータ ( 0 - 250 )
    void* (*alloc_func)(void*, int, int){nullptr}; ///< alloc 関数
    void (*free_func)(void*, void*){nullptr};      ///< free 関数
    void* opaque{nullptr};                         ///< opaque オブジェクト
  };


public:

  /// @brief コンストラクタ
  BzEngineGen(
    const Param& param = {0, 0, 9, 0, nullptr, nullptr, nullptr} ///< [in] 初期化パラメータ
  ) : mParam{param}
  {
  }

  /// @brief デストラクタ
  ~BzEngineGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 伸張用のエンジンを作る．
  CodecEngine*
  new_engine(
    istream& is,              ///< [in] 入力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const override;

  /// @brief 圧縮用のエンジンを作る．
  CodecEngine*
  new_engine(
    ostream& os,              ///< [in] 出力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化パラメータ
  Param mParam;

};
#endif // defined(BZIP2_FOUND)


#if defined(ZLIB_FOUND)
//////////////////////////////////////////////////////////////////////
/// @class GzEngineGen CodecEngine.h "ym/CodecEngine.h"
/// @brief gzip 用の codec engine を生成するクラス
//////////////////////////////////////////////////////////////////////
class GzEngineGen :
  public CodecGen
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @brief Gzip の初期化パラメータ
  //////////////////////////////////////////////////////////////////////
  struct Param
  {
    int level{Z_BEST_COMPRESSION};                                   ///< 圧縮レベル
    void* (*alloc_func)(void*, unsigned int, unsigned int){nullptr}; ///< alloc 関数
    void (*free_func)(void*, void*){nullptr};                        ///< free 関数
    void* opaque{nullptr};                                           ///< opaque オブジェクト
  };


public:

  /// @brief コンストラクタ
  GzEngineGen(
    const Param& param = {Z_BEST_COMPRESSION, nullptr, nullptr, nullptr} ///< [in] 初期化パラメータ
  ) : mParam{param}
  {
  }

  /// @brief デストラクタ
  ~GzEngineGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 伸張用のエンジンを作る．
  CodecEngine*
  new_engine(
    istream& is,              ///< [in] 入力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const override;

  /// @brief 圧縮用のエンジンを作る．
  CodecEngine*
  new_engine(
    ostream& os,              ///< [in] 出力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化パラメータ
  Param mParam;

};
#endif // defined(ZLIB_FOUND)


#if defined(LIBLZMA_FOUND)
//////////////////////////////////////////////////////////////////////
/// @class XzEngineGen CodecEngine.h "ym/CodecEngine.h"
/// @brief lzma(xz) 用の codec engine を生成するクラス
//////////////////////////////////////////////////////////////////////
class XzEngineGen :
  public CodecGen
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @brief Xz の初期化パラメータ
  //////////////////////////////////////////////////////////////////////
  struct Param
  {
    SizeType memlimit{128 * 1024 * 1024}; ///< 割り当てるメモリの上限
    ymuint32 flags{0};                    ///< 動作制御用のフラグ
    int preset{6};                        ///< 圧縮レベル ( 0 - 9: 6 がデフォルト )
    lzma_check check{LZMA_CHECK_CRC64};   ///< 検査方法
    lzma_allocator* allocator{nullptr};   ///< アロケータ
  };


public:

  /// @brief コンストラクnタ
  XzEngineGen(
    const Param& param = {128 * 1024 * 1024, 0, 6, LZMA_CHECK_CRC64, nullptr} ///< [in] 初期化パラメータ
  ) : mParam{param}
  {
  }

  /// @brief デストラクタ
  ~XzEngineGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 伸張用のエンジンを作る．
  CodecEngine*
  new_engine(
    istream& is,              ///< [in] 入力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const override;

  /// @brief 圧縮用のエンジンを作る．
  CodecEngine*
  new_engine(
    ostream& os,              ///< [in] 出力ストリーム
    SizeType buff_size = 4096 ///< [in] バッファサイズ
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化パラメータ
  Param mParam;

};
#endif // defined(LIBLZMA_FOUND)

END_NAMESPACE_YM

#endif // CODECGEN_H
