include $(SWCFGPATH_DAILEO)/$(PROJ_MODE)/config.mk

ifeq ($(FPU_ENABLE),1)
FLOATING = hardfloat
else
FLOATING = softfloat
endif

SRC_DIR := 2048_PFG
DST_DIR := 2048
BIN_DIR := release/bin
LIB_DIR := lib
RES_DIR := macaron


CP_BINDIR  := $(SRC_DIR)/$(TARGET_CPU)-$(FLOATING)/$(BIN_DIR)
CP_LIBDIR  := $(SRC_DIR)/$(TARGET_CPU)-$(FLOATING)/$(LIB_DIR)


genPKG:
	@echo ">>>Release 2048 $(TARGET_CPU)-$(FLOATING) packages...<<<"
	@mkdir -p $(DST_DIR)/$(BIN_DIR)/
	@mkdir -p $(DST_DIR)/$(LIB_DIR)/
	@cp -rf $(CP_BINDIR)/game2048.bin $(DST_DIR)/$(BIN_DIR)/

	@cp -rf $(CP_LIBDIR)/* $(DST_DIR)/$(LIB_DIR)/
	@rm -rf $(DST_DIR)/$(RES_DIR)/
	@cp -rf $(SRC_DIR)/$(RES_DIR)/ $(DST_DIR)/$(RES_DIR)/

ReGen: clean
	$(MAKE) genPKG

clean:
	@echo ">>>Remove DLNA packages...<<<"
	@rm -rf $(DST_DIR)
