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
* File  : MAppReg.cpp
**********************************************************************/

#include "MAppReg.h"

MAppFactory* MAppFactory::root = 0;

MAppFactory* MAppFactory::GetDefault()
{
    MAppFactory* node = MAppFactory::root;

    while (node)
    {
        if (node->IsDefault())
        {
            return node;
        }
        node = node->Next();
    }

    return MAppFactory::root;
}

