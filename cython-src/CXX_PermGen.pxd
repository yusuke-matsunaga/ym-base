#
# @file CXX_PermGen.pxd
# @brief PermGen 用の pxd ファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

from libcpp cimport bool
ctypedef unsigned int ymuint


cdef extern from "ym/PermGen.h" namespace "nsYm" :

    # PermGen の cython インターフェイス
    cdef cppclass PermGen :
        PermGen(ymuint, ymuint)
        ymuint n()
        ymuitn k()
        void init()
        ymuint operator()(ymuint)
        bool is_end()
        void operator++()
