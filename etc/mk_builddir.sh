#! /bin/sh
#
# mk_builddir.sh
#
# コンパイル用のディレクトリを作るシェルスクリプト
#
# Copyright (C) 2016, 2017 Yusuke Matsunaga (松永 裕介)
# All rights reserved


# function を使えるシェルかどうかのチェック
# GNU Autoconf/Automake/Libtool 21.1.4 のコードを元にしたけど
# 本に載っているコードはそのままでは引数を渡すことができない．
# また SHELL という環境変数がすでに定義済だとうまく動かない．

# zsh の場合
if test -n "$ZSH_VERSION"; then
    emulate sh
    NULLCMD=:
fi

# bash の場合
test -n "$BASH_VERSION" && set -o posix

TMP_SHELL="${TMP_SHELL-/bin/sh}"
if test x"$1" = x--re-executed; then
    shift
elif "$TMP_SHELL" -c 'foo () { exit 0; }; foo' 2>/dev/null; then
    :
else
    for cmd in sh bash ash bsh ksh zsh sh5; do
	args=${1+"$@"}
	set `IFS=":"; X="$PATH:/bin:/usr/bin:/usr/afsws/bin:/usr/ucb"; echo $X`
	for dir
	do
	    shell="$dir/$cmd"
	    if (test -f "$shell" || test -f "$shell.exe") &&
		   "$shell" -c 'foo () { exit 0; }; foo' 2>/dev/null
	    then
		SHELL="$shell" exec "$shell" "$0" --re-executed $args
	    fi
	done
    done
    echo "Unable to locate a shell interpreter with function support" >&2
    exit 1
fi


# メッセージを出力して yes/no の応答を待つ．
# yes/no 以外の入力の場合には再入力を促してループする．
# 結果は confirmation という変数が保持する．
do_confirm() {
    msg="$1 (yes/no): "
    echo -n $msg
    while read confirmation; do
	case ${confirmation} in
	    "yes")
		break;;
	    "no")
		break;;
	    *)
		echo "Please answer 'yes' or 'no'"
		echo -n $msg
		;;
	esac
    done
}

# コマンドの定義
# 環境変数 SED をセットしておけばその値が使われる．
# そうでなければデフォルトの sed が使われる．
echo ${SED:="sed"} > /dev/null

# 実行されたディレクトリを srcdir にセットする．
srcdir=`pwd`

# 引数の数が異なっていたら usage を表示して終わる．
if [ $# -ne 2 ]; then
    echo "USAGE mk_builddir.sh <compiledir> <installdir>"
    exit 1
fi

# ビルド用のディレクトリ名
builddir=$1

# インストール先のディレクトリ名
installdir=$2

# ディレクトリ名を表示して確認を求める．
echo "****"
echo "source  directory: $srcdir"
echo "build   directory: $builddir"
echo "install directory: $installdir"
echo "****"
do_confirm "continue ?"

if [ ${confirmation} = "no" ]; then
    echo "Cancelled"
    exit 0
fi

# ビルドディレクトリはなければ作る．
test -d ${builddir} || mkdir -p ${builddir}

# boot.sh ファイルを作る．
boot="boot.sh"
${SED} -e s!___SRC_DIR___!${srcdir}! \
     -e s!___INSTALL_DIR___!${installdir}! \
    ${srcdir}/ym-common/etc/${boot}.in > ${builddir}/${boot}
chmod +x ${builddir}/${boot}

# boot-opt.sh ファイルをコピーする．
optfile="boot-opt.sh"
if [ -f ${builddir}/${optfile} ]; then
    echo "***"
    echo "Try to copy ${optfile} to ${builddir}"
    echo "${builddir}/${optfile} already exists."
    do_confirm "Would you overwrite it?"
    if [ ${confirmation} == "no" ]; then
	echo "Coping file is cancelled"
    fi
else
    confirmation="yes"
fi

if [ ${confirmation} = "yes" ]; then
    cp ${srcdir}/ym-common/etc/${optfile} ${builddir}/${optfile}
fi

echo "Set up completed"
