### @file ustime.pxi
### @brief ustime の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2017 Yusuke Matsunaga
### All rights reserved.

from CXX_USTime cimport USTime as C_USTime


### @brief USTime クラスの python バージョン
cdef class USTime :

    # C++ の本体
    cdef C_USTime _this

    ### @brief 初期化
    ### @param[in] u_time user 時間(usec)
    ### @param[in] s_time system 時間(usec)
    ### @param[in] r_time real 時間(usec)
    def __init__(self, u_time = None, s_time = None, r_time = None) :
        # 本当は u_time, s_time, r_time が全部有りかなしかの二択
        if u_time != None and s_time != None and r_time != None :
            self.set(u_time, s_time, r_time)

    ### @brief 値をセットする．
    ### @param[in] u_time user 時間(usec)
    ### @param[in] s_time system 時間(usec)
    ### @param[in] r_time real 時間(usec)
    def set(self, u_time, s_time, r_time) :
        cdef double c_u_time = u_time
        cdef double c_s_time = s_time
        cdef double c_r_time = r_time
        self._this.set(c_u_time, c_s_time, c_r_time)

    ### @brief user 時間をマイクロ秒単位で返す．
    @property
    def usr_time_usec(self) :
        return self._this.usr_time_usec()

    ### @brief system 時間をマイクロ秒単位で返す．
    @property
    def sys_time_usec(self) :
        return self._this.sys_time_usec()

    ### @brief real 時間をマイクロ秒単位で返す．
    @property
    def real_time_usec(self) :
        return self._this.real_time_usec()

    ### @brief uesr 時間を秒単位で返す．
    @property
    def usr_time(self) :
        return self._this.usr_time()

    ### @brief system 時間を秒単位で返す．
    @property
    def sys_time(self) :
        return self._this.sys_time()

    ### @brief real 時間を秒単位で返す．
    @property
    def real_time(self) :
        return self._this.real_time()
