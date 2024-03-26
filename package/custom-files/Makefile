include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=custom-files
PKG_VERSION:=1.0

define Package/$(PKG_NAME)
	CATEGORY:=Custom file
	TITLE:=custom file
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	echo " Build/Prepare mkdir -p $(PKG_BUILD_DIR)"
	$(CP) ./files $(PKG_BUILD_DIR)
	echo " Build/Prepare $(CP) ./files $(PKG_BUILD_DIR)"
endef

define Build/Compile/Default

endef
Build/Compile = $(Build/Compile/Default)

define Package/$(PKG_NAME)/install
	cp -rfp $(PKG_BUILD_DIR)/files/* $(1)/
endef

define Package/$(PKG_NAME)/postinst
#!/bin/sh
if [ -z "$${IPKG_INSTROOT}" ]; then
	/etc/init.d/blade3-boot enable
fi
exit 0
endef

define Package/$(PKG_NAME)/prerm
#!/bin/sh
if [ -z "$${IPKG_INSTROOT}" ]; then
	/etc/init.d/blade3-boot disable
fi
exit 0
endef

$(eval $(call BuildPackage,custom-files))