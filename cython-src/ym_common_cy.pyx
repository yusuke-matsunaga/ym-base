# @file ym_common-cy.pyx
# @brief ym_common の cython ソース
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2015, 2016 Yusuke Matsunaga
# All rights reserved.


cimport CXX_ym_common as cxx


cdef extern from "stdlib.h" :
    void* malloc(size_t size)
    void free(void* ptr)

# @brief RandGen クラスの python バージョン
cdef class RandGen :
    cdef cxx.RandGen _this

    # @brief (再)初期化
    # @param[in] seed 乱数のシード
    def init(RandGen self, unsigned int seed) :
        self._this.init(seed)

    # @brief 配列を用いた(再)初期化
    # @param[in] init_key 初期値の配列
    # @param[in] key_length init_key の要素数
    def init_by_array(RandGen self, list init_key) :
        cdef int n = len(init_key)
        cdef unsigned int* c_init_key = <unsigned int*>malloc(sizeof(unsigned int) * n)
        cdef int i = 0
        cdef unsigned int key
        for key in init_key :
            c_init_key[i] = key
            i += 1
        self._this.init_by_array(c_init_key, n)
        free(c_init_key)

    # @brief 符号なし32ビット整数の乱数を発生する．
    def int32(RandGen self) :
        return self._this.int32()

    # @brief 符号付き31ビット整数の乱数を発生する．
    def int31(RandGen self) :
        return self._this.int31()

    # @brief 符号なし64ビット整数の乱数を発生する．
    def uint64(RandGen self) :
        return self._this.uint64()

    # @brief [0,1]の区間の実数の乱数を発生させる．
    def real1(RandGen self) :
        return self._this.real1()

    # @brief [0,1)の区間の実数の乱数を発生させる．
    def real2(RandGen self) :
        return self._this.real2()

    # @brief (0,1)の区間の実数の乱数を発生させる．
    def real3(RandGen self) :
        return self._this.real3()

    # @brief [0,1)の区間の53ビットの実数の乱数を発生させる．
    def res53(RandGen self) :
        return self._this.res53()


# @brief RandCombiGen クラスの python バージョン
cdef class RandCombiGen :
    cdef cxx.RandCombiGen* _thisptr

    # @brief コンストラクタ
    # @param[in] n 全要素数
    # @param[in] k 組み合わせの要素数
    def __cinit__(RandCombiGen self, unsigned int n, unsigned int k) :
        self._thisptr = new cxx.RandCombiGen(n, k)

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


# @brief RandPermGen クラスの python バージョン
cdef class RandPermGen :
    cdef cxx.RandPermGen* _thisptr

    # @brief コンストラクタ
    # @param[in] n 全要素数
    def __cinit__(RandPermGen self, unsigned int n) :
        self._thisptr = new cxx.RandPermGen(n)

    # @brief デストラクタ
    def __dealloc__(RandPermGen self) :
        if self._thisptr != NULL :
            del self._thisptr

    # @brief 全要素数を返す．
    @property
    def num(RandPermGen self) :
        return self._thisptr.num()

    # @brief ランダムな順列を生成する．
    # @param[in] randgen 乱数生成器
    def generate(RandPermGen self, RandGen randgen) :
        self._thisptr.generate(randgen._this)

    # @brief 順列の要素を取り出す．
    # @param[in] pos 要素の位置番号 ( 0 <= pos < num() )
    def elem(RandPermGen self, pos) :
        return self._thisptr.elem(pos)

    # @brief 順列の要素のリストを返す．
    #
    # 正確にはジェネレータを返す．
    def elem_list(RandPermGen self) :
        cdef int pos
        for pos in range(self.num()) :
            yield self._thisptr.elem(pos)


# @brief RandSampler クラスの python バージョン
cdef class RandSampler :
    cdef cxx.RandSampler* _thisptr

    # @brief コンストラクタ
    # @param[in] weight_list 重みの配列
    def __cinit__(RandSampler self, list weight_list) :
        cdef int n = len(weight_list)
        cdef unsigned int* weight_array = <unsigned int*>malloc(sizeof(unsigned int) * n)
        cdef int i = 0
        cdef unsigned int weight
        for weight in weight_list :
            weight_array[i] = weight
            i += 1
        self._thisptr = new cxx.RandSampler(n, weight_array)
        free(weight_array)

    # @brief デストラクタ
    def __dealloc__(RandSampler self) :
        if self._thisptr != NULL :
            del self._thisptr

    # @brief 要素数を返す．
    def num(RandSampler self) :
        return self._thisptr.num()

    # @brief 要素の重みを返す．
    # @param[in] pos 要素番号 ( 0 <= pos < num() )
    def weight(RandSampler self, int pos) :
        return self._thisptr.weight(pos)

    # @brief 要素のリスト返す．
    def weight_list(RandSampler self) :
        cdef unsigned int n = self._thisptr.num()
        cdef int i
        for i in range(n) :
            yield self._thisptr.weight(i)

    # @brief サンプリングを行う．
    # @param[in] randgen 乱数生成器
    def get_sample(RandSampler self, RandGen randgen) :
        return self._thisptr.get_sample(randgen._this)
