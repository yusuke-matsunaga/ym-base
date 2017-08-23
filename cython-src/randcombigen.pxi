#
# @file randcombigen.pxi
# @brief randcombigen の cython インターフェイス
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

from CXX_RandCombiGen cimport RandCombiGen as C_RandCombiGen


# @brief RandCombiGen クラスの python バージョン
cdef class RandCombiGen :
    cdef C_RandCombiGen* _thisptr

    # @brief コンストラクタ
    # @param[in] n 全要素数
    # @param[in] k 組み合わせの要素数
    def __cinit__(RandCombiGen self, unsigned int n, unsigned int k) :
        self._thisptr = new C_RandCombiGen(n, k)

    # @brief デストラクタ
    def __dealloc__(RandCombiGen self) :
        if self._thisptr != NULL :
            del self._thisptr

    # @brief 全要素数を返す．
    @property
    def num(RandCombiGen self) :
        return self._thisptr.num()

    # @brief 組み合わせの要素数を返す．
    @property
    def combi_num(RandCombiGen self) :
        return self._thisptr.combi_num()

    # @brief ランダムな組み合わせを生成する．
    # @param[in] randgen 乱数生成器
    def generate(RandCombiGen self, RandGen randgen) :
        self._thisptr.generate(randgen._this)

    # @brief 組み合わせの要素を取り出す．
    # @param[in] pos 要素の位置番号 ( 0 <= pos < combi_num() )
    def elem(RandCombiGen self, pos) :
        return self._thisptr.elem(pos)

    # @brief 組み合わせの要素のリストを返す．
    #
    # 正確にはジェネレータを返す．
    def elem_list(RandCombiGen self) :
        cdef int pos
        for pos in range(self.combi_num()) :
            yield self._thisptr.elem(pos)
