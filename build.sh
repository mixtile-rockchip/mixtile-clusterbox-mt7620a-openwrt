#!/bin/bash

export FORCE_UNSAFE_CONFIGURE=1

CMD=`realpath $0`
COMMON_DIR=`dirname $CMD`
BOARD_CONFIG=.config
TARGET_PRODUCT_DIR=$COMMON_DIR/config/BoardConfig
TARGET_BIN=$COMMON_DIR/bin/targets/ramips/mt7620/openwrt-ramips-mt7620-cluster-box-control-V100-squashfs-sysupgrade.bin

echo "TARGET_PRODUCT_DIR: $TARGET_PRODUCT_DIR"

function feeds_update()
{
	echo "============Start feeds update ============"
	echo "=========================================="
	./scripts/feeds update -a
}

function feeds_install()
{
	echo "============Start feeds install ============"
	echo "=========================================="
	./scripts/feeds install -a
}

function choose_target_board()
{
	echo
	echo "You're building on Linux"
	echo "Lunch menu...pick a combo:"
	echo ""

	echo "0. default BoardConfig.mk"
	echo ${TARGET_BOARD_ARRAY[@]} | xargs -n 1 | sed "=" | sed "N;s/\n/. /"

	local INDEX
	read -p "Which would you like? [0]: " INDEX
	INDEX=$((${INDEX:-0} - 1))

	if echo $INDEX | grep -vq [^0-9]; then
		BUILD_TARGET_BOARD="${TARGET_BOARD_ARRAY[$INDEX]}"
	else
		echo "Lunching for Default BoardConfig.mk boards..."
		BUILD_TARGET_BOARD=.config
	fi
}

function build_select_board()
{
	TARGET_BOARD_ARRAY=( $(cd ${TARGET_PRODUCT_DIR}/ && ls *config | sort) )
    echo "$(cd ${TARGET_PRODUCT_DIR}/ && ls *config | sort)"
	TARGET_BOARD_ARRAY_LEN=${#TARGET_BOARD_ARRAY[@]}
	if [ $TARGET_BOARD_ARRAY_LEN -eq 0 ]; then
		echo "No available Board Config"
		return
	fi

	choose_target_board
    if [ "$BUILD_TARGET_BOARD" != ".config" ]; then
	    ln -rfs $TARGET_PRODUCT_DIR/$BUILD_TARGET_BOARD $BOARD_CONFIG
    fi
	echo "switching to board: `realpath $BUILD_TARGET_BOARD`"
}

function usage()
{
	echo "Usage: build.sh [OPTIONS]"
	echo "Available options:"
	echo "*config    -switch to specified board config"
	echo "lunch              -list current SDK boards and switch to specified board config"
	echo "uboot              -build uboot"
	echo "kernel             -build kernel"
	echo "toolchain          -build toolchain"
	echo "all                -build uboot, kernel, rootfs, recovery image"
	echo "cleanall           -clean uboot, kernel, rootfs, recovery"
	echo "save               -save images, patches, commands used to debug"
	echo ""
	echo "Default option is 'allsave'."
}

function build_all()
{
	echo "============Start building all============"
	echo "=========================================="
	make -j1 V=s
	if [ ! -d output ];then
		mkdir output
	fi
	cp $TARGET_BIN $COMMON_DIR/output/image-release-clusterbox-openwrt22-$(date '+%Y%m%d').bin
	# ln -snf $TARGET_BIN output/image-release-clusterbox-openwrt22-$(date '+%Y%m%d').bin
}

function build_kernel()
{
	echo "============Start building kernel============"
	echo "=========================================="

    make target/linux/compile V=s
}

#=========================
# build targets
#=========================

if echo $@|grep -wqE "help|-h"; then
	if [ -n "$2" -a "$(type -t usage$2)" == function ]; then
		echo "###Current SDK Default [ $2 ] Build Command###"
		eval usage$2
	else
		usage
	fi
	exit 0
fi

OPTIONS="${@:-allsave}"

for option in ${OPTIONS}; do
	echo "processing option: $option"
	case $option in
		*config)
			option=config/BoardConfig/$option

			CONF=$(realpath $option)
			echo "switching to board: $CONF"
			if [ ! -f $CONF ]; then
				echo "not exist!"
				exit 1
			fi
			echo $BOARD_CONFIG
			ln -rsf $CONF $BOARD_CONFIG
			;;
		lunch) build_select_board ;;
		all) build_all ;;
		feeds_update) feeds_update ;;
		feeds_install) feeds_install ;;
		allsave) build_allsave ;;
		cleanall) build_cleanall ;;
		firmware) build_firmware ;;
		toolchain) build_toolchain ;;
		uboot) build_uboot ;;
		kernel) build_kernel ;;
		modules) build_modules ;;
		rootfs_inst_mods) build_rootfs_install_modules ;;
		*) usage ;;
	esac
done
