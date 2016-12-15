#ifndef __HELLOAPP_H__
#define __HELLOAPP_H__

/*@ <FileComment ID=1242655070025> @*/
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

* Class :
* File  :
**********************************************************************/
/*@ </FileComment ID=1242655070025> @*/

/*@ <Include> @*/
#include "MApplet.h"
/*@ </Include> @*/

/*@ <Definitions> @*/
class Game2048App : public MApplet
{
    DECLARE_EVENT_MAP();

public:

    // ------------------------------------------------------------
    // public operations
    // ------------------------------------------------------------

    BOOL InitResource();

    BOOL ExitResource();

private:

    //(*EventHandler
    BOOL OnWdg1NtyKey(void *arg1, void *arg2, void *arg3);
    //*)

};
/*@ </Definitions> @*/

#endif
