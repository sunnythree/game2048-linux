/**********************************************************************
 Copyright (c) 2006-2010 MStar Semiconductor, Inc.
 All rights reserved.

 Unless otherwise stipulated in writing, any and all information contained
 herein regardless in any format shall remain the sole proprietary of
 MStar Semiconductor Inc. and be kept in strict confidence
 (MStar Confidential Information) by the recipient.
 Any unauthorized act including without limitation unauthorized disclosure,
 copying, use, reproduction, sale, distribution, modification, disassembling,
 reverse engineering and compiling of the contents of MStar Confidential
 Information is unlawful and strictly prohibited. MStar hereby reserves the
 rights to any and all damages, losses, costs and expenses resulting therefrom.

* Class : MAppFactory
* File  : MAppReg.h
**********************************************************************/

#ifndef __MAPP_REG_H__
#define __MAPP_REG_H__

#include <MApplet.h>

class MAppFactory
{
public:
    MAppFactory() {
        next = root;
        root = this;
    }

    static MAppFactory* GetDefault();

    MAppFactory* Next() {
        return next;
    }

    virtual MApplet* Create() = 0;

    virtual bool IsDefault() {
        return false;
    }

private:
    static MAppFactory* root;
    MAppFactory* next;
};

#define REG_MAPP(M) \
class MAppFactory_##M : public MAppFactory { \
public:\
    virtual MApp* Create() { return new M; } \
}; \
static MAppFactory_##M s_factory_inst_##M;

#define REG_DEFAULT_MAPP(M) \
class MAppFactory_##M : public MAppFactory { \
public:\
    virtual bool IsDefauilt() { return true; } \
    virtual MApplet* Create() { return new M; } \
}; \
static MAppFactory_##M s_factory_inst_##M;

#endif
