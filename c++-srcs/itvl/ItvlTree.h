#ifndef ITVLTREE_H
#define ITVLTREE_H

/// @file ItvlTree.h
/// @brief ItvlTree のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ItvlTree ItvlTree.h "ItvlTree.h"
/// @brief 区間を表す平衡二分木
///
/// 各節点は [S, E] の区間を持つ．
/// 二分木なので節点 V の左の部分木のすべての節点の E は V の S よりも小さい
/// 右の部分木のすべての節点の S は V の E よりも大きい．
//////////////////////////////////////////////////////////////////////
class ItvlTree
{
public:

  /// @brief コンストラクタ
  ItvlTree();

  /// @brief デストラクタ
  ~ItvlTree();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  void
  clear();

  /// @brief 追加する．
  void
  add(
    int x ///< [in] 追加する値
  );

  /// @brief 削除する．
  void
  del(
    int x ///< [in] 削除する値
  );

  /// @brief 使用されていない値を取り出す．
  int
  get_min() const;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ型
  //////////////////////////////////////////////////////////////////////

  // 二分木のノードを表すデータ型
  struct Node
  {
    ~Node()
    {
      delete L;
      delete R;
    }

    // 開始点
    int S;

    // 終了点
    int E;

    // 平衡度(-1, 0, 1)
    std::int8_t B;

    // 左の子供
    Node* L{nullptr};

    // 右の子供
    Node* R{nullptr};
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief x を含むノードを探す．
  ///
  /// なければ nullptr を返す．
  Node*
  find(
    int x
  ) const;

  /// @brief x よりも小さく最も左にあるノードを探す．
  Node*
  find_left(
    int x
  ) const;

  /// @brief x よりも大きく最も右にあるノードを探す．
  Node*
  find_right(
    int x
  ) const;

  /// @brief ノードを追加する．
  bool
  insert_node(
    Node* node, ///< [in] 追加するノード
    Node*& ptr  ///< [in] 親のノード
  );

  /// @brief ノードを削除する．
  /// @return 部分木の高さが変わった時に true を返す．
  bool
  remove_node(
    Node* node, ///< [in] 削除するノード
    Node*& ptr  ///< [in] 親のノード
  );

  /// @brief 右隣のノードと位置を入れ替えて削除する．
  /// @return 部分木の高さが変わった時に true を返す．
  bool
  remove_right(
    Node* node, ///< [in] 削除するノード
    Node*& ptr  ///< [in] 親のノード
  );

  /// @brief 左の部分木の高さが減少したときの処理
  /// @return 自分自身の高さも減少する時に true を返す．
  bool
  balance_left(
    Node*& ptr  ///< [in] 親のノード
  );

  /// @brief 右の部分木の高さが減少したときの処理
  /// @return 自分自身の高さも減少する時に true を返す．
  bool
  balance_right(
    Node*& ptr  ///< [in] 親のノード
  );

  /// @brief print() の下請け関数
  static
  bool
  print_sub(
    Node* node, ///< [in] 対象のノード
    ostream& s, ///< [in] 出力ストリーム
    bool first  ///< [in] 最初の要素の時 true にするフラグ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のポインタ
  Node* mRoot{nullptr};

};

END_NAMESPACE_YM

#endif // ITVLTREE_H
