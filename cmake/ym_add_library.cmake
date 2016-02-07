
# release, relwithdeb, debug ターゲットライブラリの設定
#
# USAGE: ym_add_library ( <target-name>
#                         [SHARED|STATIC|OBJECT]
#                         [DESTINATION <destination-path>]
#                         <source-file> [<source-file>]
#                       )
function (ym_add_library)
  set (_option "SHARED")
  set (_dbg_option "STATIC")
  set (_destination 0)
  set (_read_destination 0)
  foreach ( pos RANGE 0 ${ARGC} )
    if ( ${pos} EQUAL ${ARGC} )
      break()
    endif ()
    list (GET ARGV ${pos} argv)
    if ( ${_read_destination} EQUAL 1 )
      set (_destination ${argv})
      set (_read_destination 0)
    elseif ( ${pos} EQUAL 0 )
      # ターゲット名の設定
      # - 最適化モード
      # - プロファイルモード (_p)
      # - デバッグモード (_d)
      # の3つがある．
      set (_target_name "${argv}")
      set (_target_name_p "${argv}_p")
      set (_target_name_d "${argv}_d")
    elseif ( ${argv} STREQUAL "SHARED" )
      set (_option "SHARED")
    elseif ( ${argv} STREQUAL "STATIC" )
      set (_option "STATIC")
    elseif ( ${argv} STREQUAL "OBJECT" )
      set (_option "OBJECT")
    elseif ( ${argv} STREQUAL "DESTINATION" )
      set (_read_destination 1)
    else ()
      list (APPEND _sources ${argv})
    endif ()
  endforeach ()

  # release モードのターゲット
  add_library (${_target_name}
    ${_option}
    ${_sources}
    )

  target_compile_options (${_target_name}
    PRIVATE "-O3"
    )

  target_compile_definitions (${_target_name}
    PRIVATE "-DNDEBUG"
    )

  # release with debug info モードのターゲット
  add_library (${_target_name_p}
    ${_option}
    ${_sources}
    )

  target_compile_options (${_target_name_p}
    PRIVATE "-O2" "-g"
    )

  target_compile_definitions (${_target_name_p}
    PRIVATE "-DNDEBUG"
    )

  # debug モードのターゲット
  # 常に static にする．
  add_library (${_target_name_d}
    ${_dbg_option}
    ${_sources}
    )

  target_compile_options (${_target_name_d}
    PRIVATE "-g"
    )

  target_compile_definitions (${_target_name_d}
    PRIVATE
    )

  if ( NOT ${_destination} EQUAL 0 )
    # インストールルール
    install ( TARGETS ${_target_name} ${_target_name_p} ${_target_name_d}
      DESTINATION ${_destination}
      )
  endif ()

endfunction ()
