###############################################################################
#
# Copyright (c) 2006-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# (MStar Confidential Information) by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
# Copyright (c) 2006-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Makefile used for building application.
#
################################################################################
################################################################################
# Build Options
################################################################################
# Version: debug or retail
export VERSION      ?= debug

# Verbose (0/1)
V           ?= 0

ifeq ($(V),1)
    Q       :=
else
    Q       := @
endif
################################################################################
# Toolchain
################################################################################
include $(SWCFGPATH_DAILEO)/$(PROJ_MODE)/config.mk
include $(SWCFGPATH_DAILEO)/$(PROJ_MODE)/libs.mk
include $(SWCFGPATH_DAILEO)/tool_config.mk

# **********************************************
# CHIP setting
# **********************************************
#BUILD ?= target

include Game2048.mk

# **********************************************
# Directories
# **********************************************
ROOT        = .


OBJDIR     = ./2048.obj


OBJ_0       = $(notdir $(patsubst %.cpp, %.o, $(SRC)))
OBJ         = $(foreach file, $(OBJ_0), $(OBJDIR)/$(file))

DEP_0       = $(notdir $(patsubst %.cpp, %.d, $(SRC)))
DEP         = $(foreach file, $(DEP_0), $(OBJDIR)/$(file))


# **********************************************
# Compiler and linker options
# **********************************************

INCLUDE = $(foreach dir, $(INCDIR), -I$(dir))
INCLUDE += $(SN_INCLUDES)
INCLUDE += $(SN_MUF_INCLUDES)
INCLUDE += -I$(INCLUDE_PATH)/misc

CC_OPTS += $(INCLUDE)
CC_OPTS += -I$(PHOTOSPHERE_ROOT)/projects/msrv/multimedia/dlna/include
CC_OPTS += -I$(PHOTOSPHERE_ROOT)/projects/msrv/multimedia/mminterface/include

ifeq ($(VERSION), debug)
    CC_OPTS    := $(CC_OPTS) -g
else
    CC_OPTS    := $(CC_OPTS) -O2
endif

ifeq ($(TARGET_CPU), arm)
    FROM := -shared
    TO :=
    CC_OPTS_BIN := $(subst $(FROM), $(TO), $(CC_OPTS))
endif

# **********************************************
# Rules
# **********************************************
.PHONY : all clean setup bin depend check_photosphere

#check_gccver is deleted for wider compiler acceptance; add it if needed
all: check_photosphere setup
	$(MAKE) MSTAR_IPC=$(MSTAR_IPC) BUILD=$(BUILD) THUMBNAIL_ENABLED=$(THUMBNAIL_ENABLED) bin

setup:
	@echo "-- Environment Checking -------------------------------------------------------"
	@echo "BINNAME : $(BINNAME)"
ifeq ($(BUILD),msim)
	@echo ">> BUILD : msim (simulator for x86 env.)"
else
ifeq ($(BUILD),target)
	@echo ">> BUILD : target"
else
	@echo ">> UNDEFINED BUILD, please try $>make BUILD=target or BUILD=msim"
	@exit -1
endif
endif
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@rm -f $(BINDIR)/$(BINNAME)
#	@./utility/BuildResProfile $(CUR_PROJ_PATH) $(RC_PATH)

bin: $(OBJ)
	@echo "Linking console executable: $(BINDIR)/$(BINNAME)"
ifeq ($(TARGET_CPU), arm)
	$(Q) $(CXX) $(OBJ) $(CC_OPTS_BIN) $(LD_OPTS) -o $(BINDIR)/$(BINNAME)
else
    ifneq ($(PURESW), 1)
	$(Q) $(CXX) $(OBJ) $(CC_OPTS) $(LD_OPTS) -o $(BINDIR)/$(BINNAME)
    endif
endif
	@echo -e "\033[43;4m Linking Complete! \033[0m"
	@mkdir -p ../2048_PFG/macaron/
	@cp -rf ./rc ../2048_PFG/macaron/

$(OBJ): $(OBJDIR)/%.o : %.cpp
ifeq ($(Q),@)
	@echo "[Compiling $<]"
endif
	@$(CXX) $(CC_OPTS) -M $< | sed -e 's/\(.*\)\.o/\$$\(OBJDIR\)\/\1.o/g' > $(@:.o=.d)
	$(Q) $(CXX) $(CC_OPTS) -o $@ -c $<;

clean:
	@echo "Clean project ..."
	@rm -rf $(OBJDIR)
	@rm -f $(BINDIR)/$(BINNAME)
	@rm -rf ../2048_PFG/macaron/
#case PARA
ifeq ($(PARA),1)
	$(Q) find `pwd` -name "*.bdf" -exec rm -f {} \;
endif

ifneq "$(MAKECMDGOALS)" "clean"
    -include $(DEP)
endif

log_gccver:
#	[ '$(wildcard .GCCver.log)' ] && rm .GCCver.log
	@$(CC) -v &> .GCCver.log

check_gccver: log_gccver
#	@echo "$(GCC_VERSION)"
	@[ '$(shell grep "$(GCC_VERSION)" .GCCver.log)' ] || ( echo "GCC version should be $(GCC_VERSION). please check it!!" ;exit 99)


# **********************************************
# Check supplement package setup
# **********************************************
check_photosphere:
ifneq ($(PHOTOSPHERE_ROOT), )
	@echo -e ">> ************************************************************************"
	@echo ">> Your photosphere package's root path are : "
	@echo ">>"
	@echo -e ">>\t \" $(PHOTOSPHERE_ROOT) \" "
	@echo ">>"
	@echo ">> Photosphere package are necessary for building MUF. "
	@echo ">> Please obtain this package and make sure its integrity."
	@echo -e ">> ************************************************************************"
	@sleep 1
else
	@echo -e ">> ************************************************************************"
	@echo ">> Your photosphere package's root path are not set yet."
	@echo ">>"
	@echo ">> Photosphere package are necessary for building MUF. "
	@echo ">> Please obtain this package and make sure its integrity."
	@echo ">>"
	@echo ">> by exporting environment variable:"
	@echo ">>     $>export PHOTOSPHERE_ROOT=~/P4/DAILEO/Supernova/photosphere"
	@echo ">>"
	@echo ">> or you can dynamically assign this variable by:"
	@echo ">>     $>make BUILD=target PHOTOSPHERE_ROOT=~/P4/DAILEO/Supernova/photosphere lib"
	@echo ">>"
	@echo ">>"
	@echo ">> [ABORTED]"
	@echo -e ">> ************************************************************************"
	@exit 99
endif

