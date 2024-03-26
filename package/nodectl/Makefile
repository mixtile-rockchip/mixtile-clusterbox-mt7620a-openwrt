include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=nodectl
PKG_VERSION:=1.0

define Package/nodectl
	CATEGORY:=Custom Component
	TITLE:=nodectl
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/nodectl/install
	$(INSTALL_DIR) $(1)/bin  
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nodectl $(1)/bin/
endef

define Package/nodectl/preinst
	#!/bin/sh
	echo 'before install $(PKG_NAME)'
endef

$(eval $(call BuildPackage,nodectl))