
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

#include "MMainFrame.h"
#include "MEvtHandle.h"
#include "MAppReg.h"
/*@ </Include> @*/

/*@ <Definitions> @*/
/*@ </Definitions> @*/

#include <assert.h>
#include "Game2048Frame.h"
#include "Game2048App.h"
#include "MSystem.h"
#include "MGfx.h"
#include "Num.rc.h"

//(*MAppReg
REG_DEFAULT_MAPP(Game2048App)
//*)

//(*EventHandler
//*)

BEGIN_EVENT_MAP(Game2048App, MApplet)
//(*EventMap
//*)
END_EVENT_MAP();

//(*EventHandlerImp
//*)

/*@ <Operation ID=Im23a7112em1212d833fa5mm6709> @*/
BOOL Game2048App::InitResource()
{
    //(*MainFrameRect
    Rect rc = {0,0,1280,720};
    //*)

    Game2048Frame *mf = new Game2048Frame(this, rc);
    this->SetMainFrame(mf);
    mf->InvalidateClientArea(rc);

    //create dfb window
    {
        MSystem *sys = MSystem::GetInstance();
        IDirectFBDisplayLayer *layer = sys->dfb_layer;
        mf->CreateDFBWindow(layer);
    }

    return true;
}
BOOL Game2048App::ExitResource()
{
    Game2048Frame *mf = dynamic_cast<Game2048Frame *>(this->GetMainFrame());
    if (NULL == mf)
        assert(0);

    delete mf;
    return true;
}
/*@ </Operation ID=Im23a7112em1212d833fa5mm66e4> @*/

