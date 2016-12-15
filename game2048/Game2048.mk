include $(PHOTOSPHERE_ROOT)/projects/var_common.mk
include $(SWCFGPATH_DAILEO)/$(PROJ_MODE)/config.mk

ifeq ($(FPU_ENABLE),1)
FLOATING = hardfloat
else
FLOATING = softfloat
endif

CUR_PROJ_PATH=$(shell pwd)
RC_PATH=rc
#define link type(dynamic/static)
LINK_TYPE = dynamic


#ui bin file naming
BINNAME = game2048.bin
BINDIR = ../2048_PFG/$(TARGET_CPU)-$(FLOATING)/release/bin

#source code directory
SRCDIR = ./src

VPATH =  $(SRCDIR) :  ./src :

#Build Commands:

SRC+= ./src/Game2048App.cpp
SRC+= ./src/Game2048Frame.cpp
SRC+= ./src/MAppReg.cpp
SRC+= ./src/play_wav.cpp
SRC+= ./src/main.cpp

INCDIR += include

ifeq ($(M3D_ENABLE), 1)
INCDIR += ../../../../../develop/include/
INCDIR += ../../../../../develop/include/muf/base/base3d
INCDIR += ../../../../../develop/include/muf/base/animation3d
INCDIR += ../../../../../develop/include/muf/widget/widget3d
INCDIR += ../../../../../develop/include/m3d/mstgl
endif

INCDIR += ../../../../msrv/multimedia/mminterface/include
INCDIR += ../../../../msrv/multimedia/mmplayer/include
INCDIR += ../../../../msrv/multimedia/mmbrowser/include
INCDIR += ../../../../msrv/control/inc
INCDIR += ../../../../msrv/common/inc
INCDIR += ../../../../../develop/include/MStarSDK



CC_OPTS+= -Wall -g -DTARGET_BUILD -DMSOS_TYPE_LINUX -DNEW_IMG

LD_OPTS+= $(SN_MUF_LIBS)

#MSTAR_IPC
ifeq ($(MSTAR_IPC), 1)
CC_OPTS+=  -DMSTAR_IPC
LD_OPTS += -lSNApi
endif

LD_OPTS+=  -lMStarDMP

LD_OPTS+=  -lasound

#THUMBNAIL_ENABLED
ifeq ($(THUMBNAIL_ENABLED), 0)
CC_OPTS+=  -DTHUMBNAIL_ENABLED=0
else
#thumbnail enabled only DMP enabled

CC_OPTS+=  -DTHUMBNAIL_ENABLED=1

endif

LD_OPTS+= -L../2048_PFG/$(TARGET_CPU)-$(FLOATING)/lib
LD_OPTS+= -Wl,--rpath-link ../2048_PFG/$(TARGET_CPU)-$(FLOATING)/lib

ifeq ($(LINK_TYPE),dynamic)
   ifeq ($(M3D_ENABLE), 1)
    LD_OPTS += -lmsrv_common
   endif
endif

LD_OPTS+= $(SN_LIBS)
