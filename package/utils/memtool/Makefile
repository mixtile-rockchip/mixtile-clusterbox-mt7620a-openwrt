#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=memtool
PKG_RELEASE:=1
PKG_FIXUP:=autoreconf
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/memtool
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=mem tool
  DEPENDS:=
endef

# define Build/Configure
# 	pushd $(PKG_BUILD_DIR)
# 	$(PKG_BUILD_DIR)/configure
# 	sleep 3
# endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -Wall" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/memtool/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/memtool $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,memtool))
