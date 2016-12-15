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
* File  : main.cpp
**********************************************************************/

//---------------------------------------------------------------------------
// Include Files
//---------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include "MTypes.h"
#include "MSystem.h"
#include "MAppReg.h"
#include "RcManager.h"

#include "Game2048App.h"

#ifdef TARGET_BUILD
#include "mlogger.h"
#include "CommUIInterface.h"
#endif

#include "apm.h"
#include "SNApiInterface.h"
#include <sys/time.h>

#define OSD_RESOLUTION_WIDTH  1280
#define OSD_RESOLUTION_HEIGHT 720

#define DFBLAYER 0



void SetDFB(bool show)
{

    DFBResult ret;
    IDirectFBDisplayLayer * dlna_dfb_layer;
    IDirectFB *dfb=MSystem::GetInstance()->dfb;
    MSystem *sys = MSystem::GetInstance();
    IDirectFBScreen *dfbscreen;
    int screen_width; int screen_height;
    DFBDisplayLayerConfig  dlc;
    printf("[DLNA UI]____________________layer=%d\n",DFBLAYER);

    ret = dfb->GetDisplayLayer( dfb, DFBLAYER, &dlna_dfb_layer );
    if (ret) {
        DirectFBError( "IDirectFB::GetDisplayLayer() failed", ret );
        exit( 3 );
    }

    sys->dfb_layer = dlna_dfb_layer;

    ret = dfb->GetScreen(dfb,0,&dfbscreen);
    if (ret) {
        DirectFBError( "IDirectFB::GetScreen() failed", ret );
        exit( 3 );
    }
    dfbscreen->GetSize(dfbscreen,&screen_width,&screen_height);
    dfbscreen->Release(dfbscreen);

    /* Set the cooperative level. */
    dlna_dfb_layer->SetCooperativeLevel(dlna_dfb_layer, DLSCL_ADMINISTRATIVE);

    dlna_dfb_layer->SetBackgroundMode(dlna_dfb_layer, DLBM_COLOR);
    dlna_dfb_layer->SetBackgroundColor(dlna_dfb_layer, 0x00, 0x00, 0x00, 0x00);

    dlna_dfb_layer->GetConfiguration(dlna_dfb_layer,&dlc);

    dlc.flags = (DFBDisplayLayerConfigFlags)(DLCONF_OPTIONS|DLCONF_HEIGHT|DLCONF_WIDTH|DLCONF_BUFFERMODE|DLCONF_PIXELFORMAT);
    if (show == true)
    {
        printf("show: ALPHACHANNEL\n");
        dlna_dfb_layer->SetOpacity(dlna_dfb_layer, 0xff);
        dlc.options = (DFBDisplayLayerOptions)(DLOP_ALPHACHANNEL);
    }
    else
    {
        printf("show: OPACITY\n");
        dlna_dfb_layer->SetOpacity(dlna_dfb_layer, 0);
        dlc.options = (DFBDisplayLayerOptions)(DLOP_OPACITY);
    }

    if((screen_width < OSD_RESOLUTION_WIDTH) || (screen_height < OSD_RESOLUTION_HEIGHT))
    {
        dlc.height = screen_height;
        dlc.width = screen_width;
    }
    else
    {
        dlc.height = OSD_RESOLUTION_HEIGHT;
        dlc.width = OSD_RESOLUTION_WIDTH;
    }

    dlc.pixelformat = DSPF_ARGB;
    dlc.buffermode = DLBM_BACKVIDEO;
    dlna_dfb_layer->SetConfiguration( dlna_dfb_layer, &dlc );
    dlna_dfb_layer->SetScreenLocation(dlna_dfb_layer,0.0,0.0,1.0,1.0);

}


int OnGetFocus(void)
{
    printf("[2048 UI]: OnGetFocus()\n");
    return 0;
}

int OnLoseFocus(void)
{
    printf("[2048 UI]: OnLoseFocus()\n");
    return 0;
}

int OnDestroy(void)
{
    printf("[2048 UI]: OnDestroy()\n");
    return 0;
}

int OnGetResource(APM_RESOURCE_e res)
{
    printf("[2048 UI]: OnResourceAvailable()\n");
    //do not disable hotkey, hotkey should be received by SN defined at apm.def
    //APM_RequestToDisableHotkey(_disabled_hotkey, sizeof(_disabled_hotkey)/sizeof(int));
    SetDFB(true);
	return 0;
}

int OnLoseResource(APM_RESOURCE_e res)
{
    printf("[2048 UI]: OnLoseResource()\n");
    SetDFB(true);
    return 0;
}

APM_APPCTRL_HANDLER_t game2048_handler =
{
    OnGetFocus,
    OnLoseFocus,
    OnDestroy,
    OnGetResource,
    OnLoseResource,
    0,
};

int main(int argc, char *argv[])
{
	
	std::cout<<"++++++++++++++++++44444++++++++++++++++=hellomain 2048"<<std::endl;
	std::cout<<"++++++++++++++++++44444++++++++++++++++=hellomain 2048"<<std::endl;
	std::cout<<"++++++++++++++++++44444++++++++++++++++=hellomain 2048"<<std::endl;
	std::cout<<"++++++++++++++++++44444++++++++++++++++=hellomain 2048"<<std::endl;
	std::cout<<"++++++++++++++++++44444++++++++++++++++=hellomain 2048"<<std::endl;
	std::cout<<"++++++++++++++++++44444++++++++++++++++=hellomain 2048"<<std::endl;
	std::cout<<"++++++++++++++++++44444++++++++++++++++=hellomain 2048"<<std::endl;
	std::cout<<"++++++++++++++++++44444++++++++++++++++=hellomain 2048"<<std::endl;
	
		MResourcePkg::SetDefaultRcDir((char *)"../../macaron/rc/rc/bin");
	
		MSystem::Initialize(0, NULL, FALSE);
		t_IPC_DFB_Config my2048_dfbcfg;
		my2048_dfbcfg.pixelformat = DSPF_ARGB;
		MSystem::GetInstance()->ipc_dfb_config = &my2048_dfbcfg ;
		MSystem::InitializeDFBLayer();	
		if(APM_Join(&game2048_handler) != 0)
		{
			printf("APM client test program aborted.\n");
			return -1;
		}
		MApplet *gameapp = new (std::nothrow) Game2048App();		
		ASSERT(gameapp);		
		gameapp->Run(0, NULL);	
		
		MSystem::Run();
		APM_Leave();		
		MSystem::Shutdown();
		return 0;

}
