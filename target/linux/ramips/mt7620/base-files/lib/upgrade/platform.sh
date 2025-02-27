#
# Copyright (C) 2010 OpenWrt.org
#

PART_NAME=firmware
REQUIRE_IMAGE_METADATA=1

RAMFS_COPY_BIN='fw_printenv fw_setenv'
RAMFS_COPY_DATA='/etc/fw_env.config /var/lock/fw_printenv.lock'

platform_check_image() {
	return 0
}

platform_do_upgrade() {
	local board=$(board_name)

	case "$board" in
	alfa-network,ac1200rm|\
	alfa-network,r36m-e4g|\
	alfa-network,tube-e4g)
		[ "$(fw_printenv -n dual_image 2>/dev/null)" = "1" ] &&\
		[ -n "$(find_mtd_part backup)" ] && {
			PART_NAME=backup
			if [ "$(fw_printenv -n bootactive 2>/dev/null)" = "1" ]; then
				fw_setenv bootactive 2 || exit 1
			else
				fw_setenv bootactive 1 || exit 1
			fi
		}
		default_do_upgrade "$1"
		;;
	rostelecom,rt-fl-1|\
	rostelecom,s1010)
		idx="$(find_mtd_index ftd_and_bootflag)"
		[ -n "$idx" ] && \
			printf 0 | dd bs=1 seek=$((0x18007)) count=1 \
				of=/dev/mtdblock$idx
		default_do_upgrade "$1"
		;;
	cluster-box-control|\
	cluster-box-control-V120)
		dd if=$1 of=/dev/mmcblk0 seek=16 conv=fsync && sync
		;;
	*)
		echo "Unsupported devices"
		;;
	esac
}
