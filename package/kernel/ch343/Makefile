#
# Copyright (C) 20023-2023 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ch343
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

EXTRA_CFLAGS:=CONFIG_PACKAGE_kmod-ch343=m
EXTRA_CFLAGS:= \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG))))  \

MAKE_OPTS:= \
	ARCH=$(LINUX_KARCH) \
	CROSS_COMPILE=$(TOOLCHAIN_DIR)/bin/$(TARGET_CROSS) \
	M=$(PKG_BUILD_DIR) \
	$(EXTRA_KCONFIG)

define KernelPackage/ch343
	SUBMENU:=Other modules
	TITLE:=Support for CH343 devices
	DEPENDS:=+kmod-usb-core
	FILES:=$(PKG_BUILD_DIR)/ch343.ko
	KCONFIG:=
	AUTOLOAD:=$(call AutoLoad,30,ch343,1)
endef

define KernelPackage/ch343/description
	Kernel support for Winchiphead CH343 USB-to-Serial converters
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -R ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" modules
endef
$(eval $(call KernelPackage,ch343))
