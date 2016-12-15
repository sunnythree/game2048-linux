/*@ <FileComment ID=1243004285187> @*/
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
/*@ </FileComment ID=1243004285187> @*/

/*@ <Include> @*/
#include "Game2048Frame.h"
#include "MApplet.h"
#include "AbsContainer.h"
#include "Container.h"
#include "MWnd.h"
#include "MEvtHandle.h"
#include "MGfx.h"
#include "Str.rc.h"
#include "Color.rc.h"
#include "Image.rc.h"

#include "Bool.rc.h"
#include "Num.rc.h"
#include "RcManager.h"
#include "MEvent.h"
/*@ </Include> @*/
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <string.h>
#include <stdlib.h>

#include <sstream>
#include <fcntl.h>
#include <fstream>  
#include <time.h>
#include <math.h>
#include "play_wav.h"
#include "asoundlib.h"

//#include "MSrv_SSSound.h"
//#include "MSrv_Control.h"

#define IS_MANUAL_HIDDEN 1
#define DEFAULT_FONT_FILE           "/applications/DLNA/macaron/rc/font/DroidSansFallback.ttf"
#define HSCOREFILE "/Customer/.hscore"

#if defined(MSTAR_IPC)
#include "MSystem.h"
#include "apm.h"
#endif

/****************************************/
/* Event Map
*****************************************/
BEGIN_EVENT_MAP(Game2048Frame, MMainFrame)
ON_CREATE(&Game2048Frame::OnCreate)
ON_DESTROY(&Game2048Frame::OnDestroy)
ON_KEY(&Game2048Frame::OnKey)
ON_TIMER(ID_GAME2048TIMER, &Game2048Frame::OnTimer)
END_EVENT_MAP();
/* macro for a safe call to DirectFB functions */
#define DFBCHECK(x...) \
        {                                                                      \
           err = x;                                                            \
           if (err != DFB_OK) {                                                \
              fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );           \
              DirectFBErrorFatal( #x, err );                                   \
           }                                                                   \
        }

//WIN
#define IMAGEGAMEWINBG			"../../macaron/rc/img/2048/wfbg.png"
#define IMAGEGAMEWIN  			"../../macaron/rc/img/2048/you-win.png"
#define IMAGEGAMEWINFEXIT 		"../../macaron/rc/img/2048/exit-f.png"
#define IMAGEGAMEWINFCONTINUE 	"../../macaron/rc/img/2048/continue-f.png"
#define IMAGEGAMEWINEXIT 		"../../macaron/rc/img/2048/exit.png"
#define IMAGEGAMEWINCONTINUE 	"../../macaron/rc/img/2048/continue.png"

//FAIL
#define IMAGEGAMEFAILBG			"../../macaron/rc/img/2048/wfbg.png"
#define IMAGEGAMEOVER 			"../../macaron/rc/img/2048/you-lost.png"
#define IMAGEGAMEOVERFEXIT 		"../../macaron/rc/img/2048/exit-f.png"
#define IMAGEGAMEOVERFRESTART 	"../../macaron/rc/img/2048/restart-f.png"
#define IMAGEGAMEOVEREXIT 		"../../macaron/rc/img/2048/exit.png"
#define IMAGEGAMEOVERRESTART 	"../../macaron/rc/img/2048/restart.png"

//MENU
#define IMAGEGAMEMENU 			"../../macaron/rc/img/2048/menu.png"
#define IMAGEGAMEMENUEXIT		"../../macaron/rc/img/2048/menu-exit.png"       
#define IMAGEGAMEMENURESTART	"../../macaron/rc/img/2048/menu-restart.png"
#define IMAGEGAMEMENUBACKONE	"../../macaron/rc/img/2048/menu-backone.png"
#define IMAGEGAMEMENUFORCUS		"../../macaron/rc/img/2048/menu-forcus.png"
#define IMAGEGAMEMENUEXITN		"../../macaron/rc/img/2048/menu-exit-n.png"
#define IMAGEGAMEMENURESTARTN	"../../macaron/rc/img/2048/menu-restart-n.png"
#define IMAGEGAMEMENUBACKONEN	"../../macaron/rc/img/2048/menu-backone-n.png"

//START
#define IMAGEGAMESTART			"../../macaron/rc/img/2048/start.png"
//INFO
#define IMAGEGAMEINFO			"../../macaron/rc/img/2048/info.png"

//MAIN PAGE
#define IMAGEGAMELEFT 			"../../macaron/rc/img/2048/left.png"
#define IMAGEGAMERIGHT 	 		"../../macaron/rc/img/2048/right.png"
//best score
#define IMAGEGAMEBEST			"../../macaron/rc/img/2048/best.png"
#define IMAGEGAMESCORE 	 		"../../macaron/rc/img/2048/score.png"


#define GAMEVIDEOSPECIALH        "../../macaron/rc/video/h.wav"


//GAME LEFR PART
#define GAMEMAPLEFTWIDTH ((width-height)/4+height)    //left width
#define GAMEMAPLEFTHEIGHT height
#define GAMEMAPLEFTSTARTX 0
#define GAMEMAPLEFTSTARTY 0


//GAME CENTER PART
#define GAMECENTERSHRINK    20
#define GAMECENTERSTARTX    (width-height)/4+GAMECENTERSHRINK	  //start point of board
#define GAMECENTERSTARTY    GAMECENTERSHRINK					  //start point of board
#define GAMECENTERWIDTH     (height-GAMECENTERSHRINK*2)
#define GAMECENTERHEIGHT    (height-GAMECENTERSHRINK*2)


//GAME RIGHT PART
#define GAMEMAPRIGHTWIDTH (width-height)/4*3    //right width
#define GAMEMAPRIGHTHEIGHT height
#define GAMEMAPRIGHTSTARTX (width-height)/4 + height
#define GAMEMAPRIGHTSTARTY 0

//GAME WIN
#define GAMEOVERWIDTH  GAMECENTERWIDTH			  //win widht
#define GAMEOVERHEIGHT GAMECENTERHEIGHT/2
#define GAMEOVERSTARTX GAMECENTERSTARTX+(GAMECENTERWIDTH-GAMEOVERWIDTH)/2
#define GAMEOVERSTARTY GAMECENTERSTARTY+(GAMECENTERHEIGHT-GAMEOVERHEIGHT)/2

//GAME FIAL
#define GAMEWINWIDTH   GAMECENTERWIDTH			  //fail widht
#define GAMEWINHEIGHT  GAMECENTERHEIGHT/2
#define GAMEWINSTARTX  GAMECENTERSTARTX+(GAMECENTERWIDTH-GAMEOVERWIDTH)/2
#define GAMEWINSTARTY  GAMECENTERSTARTY+(GAMECENTERHEIGHT-GAMEOVERHEIGHT)/2

//UNITS
#define GAMEMAPSCALEEWIDTH  50                //all of scale width 
#define GAMEMAPSCALEEHEIGHT  50				  //all of scale height 
#define GAME2048UNITWIDTH (height-GAMEMAPSCALEEWIDTH-GAMECENTERSHRINK*2)/4       //one unit width
#define GAME2048UNITHEIGHT (height-GAMEMAPSCALEEHEIGHT-GAMECENTERSHRINK*2)/4	  //one unit height

//ANIMATIONS CONTION
#define GAMEUNITMOVEWSCALE width/4            //animation one step width
#define GAMEUNITMOVEHSCALE (height-40)/4	  //animation one step width
#define GAMEMOVESCALE 4
#define GAMEFONTSIZE  50
#define GAMECIRCLER   20


#define CSCOREPOSHEIGHT 520
#define CSCOREHEIGHT    60
#define CSCOREWIDTH     GAMEMAPRIGHTWIDTH/2

#define HSCOREPOSHEIGHT 620
#define HSCOREHEIGHT    60
#define HSCOREWIDTH     GAMEMAPRIGHTWIDTH/2


ColorRGB unitcolors[11] = {
	{238, 228, 218},        // 2
	{237, 224, 200},		// 4   
	{242, 177, 121},		// 8
	{245, 149, 99},			//16
	{246, 124, 95},			//32
	{246, 94, 59},			//64
	{237, 207, 114},		//128
	{237, 204, 97},			//256
	{246, 206, 70},			//512
	{237, 197, 63},			//1024
	{234, 185, 19}			//2048
};

static pthread_mutex_t m_MutSound;
static pthread_cond_t m_CondSound;

static bool volatile soundThreadFlag=true;
static bool volatile g_game_move=false;

Game2048Frame::Game2048Frame(MApplet * pParentApp, Rect & stRect)
        : MMainFrame(pParentApp, stRect),width(1280),height(720)
{
    /* -> add your code here */
	std::cout<<"HelloFrame"<<std::endl;

	
	DFBResult ret;
	DFBSurfaceDescription surdsc;


	sys = MSystem::GetInstance();

	dfb = sys->dfb;
	desc.flags = DFDESC_HEIGHT;
	desc.height = 60;	
	if(dfb==NULL)
		printf("get dgb fial\n");
	else
		printf("get dgb sucess\n");
	ret = dfb->CreateFont( dfb, DEFAULT_FONT_FILE, &desc, &font );
    if (ret)printf("WidgetManager::drawSubtitle create font fail. %s \n",  DirectFBErrorString(ret));
	else printf("creat font sucess\n");
	
	desc.flags = DFDESC_HEIGHT;
	desc.height = 40;		
	ret = dfb->CreateFont( dfb, DEFAULT_FONT_FILE, &desc, &font_l );
    if (ret)printf("WidgetManager::drawSubtitle create font_l fail. %s \n",  DirectFBErrorString(ret));
	else printf("creat font sucess\n");
	
	desc.flags = DFDESC_HEIGHT;
	desc.height = 80;	
	ret = dfb->CreateFont( dfb, DEFAULT_FONT_FILE, &desc, &font_g );
    if (ret)printf("WidgetManager::drawSubtitle create font_g fail. %s \n",  DirectFBErrorString(ret));
	else printf("creat font sucess\n");
	
	surdsc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS | DSDESC_PIXELFORMAT);
	surdsc.caps  = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING ); 
	surdsc.pixelformat = DSPF_ARGB;
	ret =dfb->CreateSurface( dfb, &surdsc, &windowsurface );	
    if (ret)
    {
        printf("WidgetManager::drawSubtitle create CreateSurface fail. %s \n",  DirectFBErrorString(ret));
    }
	else{
		printf("creat CreateSurface sucess\n");
	}
	if(windowsurface){
		windowsurface->SetFont (windowsurface, font);	
		printf("SetFont sucess\n");
	}else printf("SetFont fail\n");
	
	windowsurface->SetBlittingFlags( windowsurface, (DFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL );
	windowsurface->SetPorterDuff( windowsurface, DSPD_SRC_OVER );
	
}

Game2048Frame::~Game2048Frame()
{
	int llhcore=0;
		printf("Game2048Frame::~Game2048Frame\n");
		
		windowsurface->Clear(windowsurface,0,0,0,0);
		windowsurface->SetColor(windowsurface,0,0,0,0);
		windowsurface->FillRectangle(windowsurface,0,0,width,height);
		windowsurface->Flip( windowsurface, NULL,(DFBSurfaceFlipFlags)DSFLIP_NONE);
		ifstream in(HSCOREFILE);
		if(!in.is_open())
		{
			FILE *a;
			std::cout <<  "open error" << std::endl;
			a = fopen(HSCOREFILE,"w+");
			if( a == NULL )
				std::cout << "create file error";
			else {
				fclose(a);
				std::cout<< "create file sucess";
				ofstream out(HSCOREFILE);
				out.is_open();
				out << hscore;
				in.close();
			}
		}
		else
		{
		std::cout << "open file sucess";
		in >> llhcore;
		in.close();
		if(hscore<0 || hscore >65536)hscore=0;	
		printf("llhcore read form file = %d\n",llhcore);
		}		
		if(llhcore < hscore){
			ofstream out(HSCOREFILE);
			if(!out.is_open())
			{
				std::cout <<  "open error" << std::endl;
				FILE *a;
				a = fopen(HSCOREFILE,"w+");
				if( a == NULL )
					std::cout << "create file error";
				else 
						fclose(a);
			} 
			else
			{
				std::cout << "open file sucess";
				out << hscore;
				out.close();
			}

		}
		font_g->Release(font_g);
		font->Release(font);
		font_l->Release(font_l);
		windowsurface->Release(windowsurface);

}

static void * game2048_playwav(void *pData)
{
	int rc;
	int ret;
	int size;
	snd_pcm_t* handle; //PCI设备句柄
	snd_pcm_hw_params_t* params;//硬件信息和PCM流配置
	unsigned int val;
	int dir=0;
	snd_pcm_uframes_t frames;
	struct WAV_HEADER wav_header;
	char *buffer;
	int channels;
	int frequency;
	int bit;
	int datablock;
//	int count=0;
	int nread;
   //	  unsigned char ch[100]; //用来存储wav文件的头信息
	FILE *fp;

	
	fp=fopen(GAMEVIDEOSPECIALH,"rb");
	if(fp==NULL)
	{
		perror("open file failed:\n");
		exit(1);
	}		
	nread=fread(&wav_header,1,sizeof(wav_header),fp);
	printf("fread byte is:%d\n",nread);
  
	channels=wav_header.wChannels;
	frequency=wav_header.nSamplesPersec;
	bit=wav_header.wBitsPerSample;
	datablock=wav_header.wBlockAlign;
   
    rc=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if(rc<0)
    {
	   perror("\nopen PCM device failed:");
	   pthread_exit(NULL);
    }

	snd_pcm_hw_params_alloca(&params); //分配params结构体
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_alloca:");
		pthread_exit(NULL);
	}
	 rc=snd_pcm_hw_params_any(handle, params);//初始化params
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_any:");
		pthread_exit(NULL);
	}
	rc=snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); //初始化访问权限
	if(rc<0)
	{
		perror("\nsed_pcm_hw_set_access:");
		pthread_exit(NULL);

	}

	//采样位数
	switch(bit/8)
	{
	case 1:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
			break ;
	case 2:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
			break ;
	case 3:snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S24_LE);
			break ;

	}
	rc=snd_pcm_hw_params_set_channels(handle, params, channels); //设置声道,1表示单声>道，2表示立体声
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_set_channels:");
		pthread_exit(NULL);
	}
	val = frequency;
	rc=snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir); //设置>频率
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_set_rate_near:");
		pthread_exit(NULL);
	}

	rc = snd_pcm_hw_params(handle, params);
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params: ");
		pthread_exit(NULL);
	}

	rc=snd_pcm_hw_params_get_period_size(params, &frames, &dir); 
	if(rc<0)
	{
		perror("\nsnd_pcm_hw_params_get_period_size:");
		pthread_exit(NULL);
	}
	size = frames * datablock; 
	buffer =(char*)malloc(size*300);
	printf("size = %d\n",size);
	printf("frames = %d\n",(int)frames);
	memset(buffer,0,sizeof(buffer));
	
	fseek(fp,58,SEEK_SET); //定位歌曲到数据区		
	ret = fread(buffer, 1, size*300, fp);
	if (ret != size)
	{
			printf("no enough data read\n");
	 }
	printf("fread = %d\n",ret);
	fclose(fp);
	printf("fclose fp\n");
	

	while(soundThreadFlag)
	{
		pthread_mutex_lock(&m_MutSound);
		pthread_cond_wait(&m_CondSound,&m_MutSound);
		pthread_mutex_unlock(&m_MutSound);			
		snd_pcm_prepare(handle);
		if(g_game_move)
			snd_pcm_writei(handle, buffer, frames*300);	
		else
			snd_pcm_writei(handle, buffer, frames*150);	
		snd_pcm_start(handle);	
//		printf("once again\n");
	}
	printf("exit while\n");
	free(buffer);	
	printf("free buffer\n");
	snd_pcm_close(handle);
	printf("snd_pcm_close \n");
	return NULL;
}


void Game2048Frame::drawAlphaPic(IDirectFBSurface *windowsurface,char * name,DFBRectangle lrect,char mode)
{
	DFBSurfaceDescription	sdsc;
	IDirectFBImageProvider *lprovider;
	IDirectFBSurface *tempsurf;
	DFBRectangle llrect;
	/* create the temporary surface */
	sdsc.flags		 = (DFBSurfaceDescriptionFlags)( DSDESC_PIXELFORMAT | DSDESC_WIDTH | DSDESC_HEIGHT);
//	sdsc.caps		 = DSCAPS_PREMULTIPLIED;
	sdsc.pixelformat = DSPF_ARGB;
	sdsc.width		 = lrect.w;
	sdsc.height 	 = lrect.h;

	if(dfb->CreateSurface( dfb, &sdsc, &tempsurf))printf("CreateSurface eror\n");
	if(dfb->CreateImageProvider( dfb, (char *)name, &lprovider))printf("CreateImage eror\n");
	llrect.x=0;
	llrect.y=0;
	llrect.w=lrect.w;
	llrect.h=lrect.h;
	tempsurf->Clear(tempsurf,0,0,0,0);
	lprovider->RenderTo( lprovider, tempsurf, &llrect );
	lprovider->Release( lprovider );
	
	windowsurface->SetBlittingFlags( windowsurface, (DFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL );
//	windowsurface->SetPorterDuff( windowsurface, DSPD_SRC_OVER );
	windowsurface->Blit( windowsurface, tempsurf, &llrect, lrect.x,lrect.y);
	tempsurf->Release(tempsurf);
	if(mode){
		DFBRegion lregion={lrect.x,lrect.y,lrect.x+lrect.w-1,lrect.y+lrect.h-1};
		windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
	}
}

BOOL Game2048Frame::OnCreate(void * arg1, void * arg2, void * arg3)
{
	int ret ;		
	std::cout<<"OnCreate"<<std::endl;
	
	ret = pthread_mutex_init(&m_MutSound,NULL);
	if(ret)printf("pthread_mutex_init error\n");
	ret = pthread_cond_init(&m_CondSound,NULL);
	if(ret)printf("pthread_cond_init error\n");
	
	ret= pthread_create(&tThreadSoundID, NULL, game2048_playwav,NULL);
	if (ret)
	{
		ASSERT(0);
	}
	printf("child thread create sucess\n");	
//	game_startAnimations();
	start_game();
	init_map();
	return false;
}

BOOL Game2048Frame::OnDestroy(void * arg1, void * arg2, void * arg3)
{
	std::cout<<"OnDestroy"<<std::endl;
	soundThreadFlag=false;
	pthread_mutex_lock(&m_MutSound);
	pthread_cond_signal(&m_CondSound);
	pthread_mutex_unlock(&m_MutSound);		
	if((pthread_join(tThreadSoundID,NULL) != 0)){
		printf("child thread exit error\n");
	}else
		printf("child thread exit sucess\n");

    return true;
}

BOOL Game2048Frame::OnTimer(void* arg1, void* arg2, void* arg3)
{
//	std::cout<<"OnTimer"<<std::endl;
	play_animations();
    return true;
}


BOOL Game2048Frame::OnKey(void* arg1, void* arg2, void* arg3)
{
//	std::cout<<"OnKey"<<std::endl;
	
	U32 keycode = (U32)arg2;
	printf("Game2048Frame::OnKey");
	if(gamemode<0 || gamemode>4)gamemode=0;
	switch(gamemode){
	  case GAME_NOMEL:
		  game_mode_nomal(keycode);
		  break;
	  case GAME_MENU:
		  game_mode_menu(keycode);
		  break;
	  case GAME_INFO:
		  game_mode_info(keycode);
		  break;
	  case GAME_OVER:
		  game_mode_over(keycode);
		  break;
	  case GAME_WIN:
		  game_mode_win( keycode);
		  break;
	  default:break;
	
	}
	  return TRUE;
}

bool Game2048Frame::CreatOneUnit(int id,int number)
{
	if(id<0)return false;
	allunits[id/4][id%4]=number;
	blankcount--;
	return true;
}

bool Game2048Frame::CreatOneUnit_ani(int id,int number)
{
	int i=0,j=0,k=0;
	char buf[8];
	ColorRGB tlcolor;	
	IDirectFBFont *llfont;
	int heightofllfont=0;
	if(id<0)return false;
	i=id/4;j=id%4;
	allunits[i][j]=number;
	blankcount--;

	k=0;
	while(k < GAME2048UNITWIDTH/2)
	{
		k += 20;	
		if(k > GAME2048UNITWIDTH/2){
			tlcolor = unitcolors[getBitCount(allunits[i][j])];
			windowsurface->SetColor(windowsurface,tlcolor.r,tlcolor.g,tlcolor.b,0xff);
			drawRoundRectangle(windowsurface,IdToGetWidth(i*4+j),IdToGetHeight(i*4+j),GAME2048UNITWIDTH,GAME2048UNITWIDTH,GAMECIRCLER);				
			snprintf( buf, sizeof(buf), "%d",allunits[i][j]);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE	
			if(allunits[i][j]>8)
				windowsurface->SetColor(windowsurface,0xff,0xff,0xff,0xff);
			else
				windowsurface->SetColor(windowsurface,0,0,0,0xff);
			if(allunits[i][j]<100)windowsurface->SetFont (windowsurface, font_g);
			else if(allunits[i][j]<10000)windowsurface->SetFont (windowsurface, font);
			else windowsurface->SetFont (windowsurface, font_l);
			windowsurface->GetFont(windowsurface,&llfont);
			llfont->GetHeight(llfont,&heightofllfont);
			windowsurface->DrawString(windowsurface, buf, -1, IdToGetWidth(i*4+j)+GAME2048UNITWIDTH/2, IdToGetHeight(i*4+j)+GAME2048UNITWIDTH/2+heightofllfont/4,(DFBSurfaceTextFlags) DSTF_CENTER);		
			llfont->Release(llfont);
			DFBRegion lregion={IdToGetWidth(i*4+j),IdToGetHeight(i*4+j),IdToGetWidth(i*4+j)+GAME2048UNITWIDTH,IdToGetHeight(i*4+j)+GAME2048UNITHEIGHT};
			windowsurface->Flip( windowsurface,&lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );

		}else{
			tlcolor = unitcolors[getBitCount(allunits[i][j])];
			windowsurface->SetColor(windowsurface,tlcolor.r,tlcolor.g,tlcolor.b,0xff);
			drawRoundRectangle(windowsurface,IdToGetWidth(i*4+j)+GAME2048UNITWIDTH/2-k,IdToGetHeight(i*4+j)+GAME2048UNITWIDTH/2-k,k+k,k+k,GAMECIRCLER);				
			snprintf( buf, sizeof(buf), "%d",allunits[i][j]);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE	
			if(allunits[i][j]>8)
				windowsurface->SetColor(windowsurface,0xff,0xff,0xff,0xff);
			else
				windowsurface->SetColor(windowsurface,0,0,0,0xff);
			
			if(allunits[i][j]<100)windowsurface->SetFont (windowsurface, font_g);
			else if(allunits[i][j]<10000)windowsurface->SetFont (windowsurface, font);
			else windowsurface->SetFont (windowsurface, font_l);
			windowsurface->GetFont(windowsurface,&llfont);
			llfont->GetHeight(llfont,&heightofllfont);
			if(k>25)
				windowsurface->DrawString(windowsurface, buf, -1, IdToGetWidth(i*4+j)+GAME2048UNITWIDTH/2, IdToGetHeight(i*4+j)+GAME2048UNITWIDTH/2+heightofllfont/4,(DFBSurfaceTextFlags) DSTF_CENTER);		
			llfont->Release(llfont);

			DFBRegion lregion={IdToGetWidth(i*4+j)+GAME2048UNITWIDTH/2-k,IdToGetHeight(i*4+j)+GAME2048UNITWIDTH/2-k,IdToGetWidth(i*4+j)+GAME2048UNITWIDTH/2+k,IdToGetHeight(i*4+j)+GAME2048UNITWIDTH/2+k};
			windowsurface->Flip( windowsurface,&lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
		}
	}
	
return true;
}

int Game2048Frame::GetSrandNumber()
{
	int result;
	srand((unsigned )time(NULL));
	result = rand();
	result %= 4;
	return ((result>2)?4:2);
}
int Game2048Frame::GetSrandAnyNumber(int denominator)
{
	int result;
	if(denominator <= 0)
		return denominator;
	srand((unsigned )time(NULL));
	result = rand();
	result %= denominator;
return result;
}

int Game2048Frame::GetSrandUniqueId()
{
	int i,j;
	int id=0,count=0,temcount=0;
	if(blankcount <= 0)return -1;
	temcount = GetSrandAnyNumber(blankcount);
	count=0;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(allunits[i][j]==0){
				if(count==temcount){
					id=i*4+j;
					goto label;
				}
				count++;
			}
		}
	}
	label:
	return id;
}


void Game2048Frame::init_map()
{
	windowsurface->SetFont (windowsurface, font);
	windowsurface->SetColor(windowsurface,249,249,239,0xff);
	windowsurface->FillRectangle(windowsurface,0,0,width,height);					
	windowsurface->SetColor(windowsurface,187,173,160,0xff);
	windowsurface->DrawString(windowsurface, "2048", -1,80,100,(DFBSurfaceTextFlags) DSTF_CENTER);
	windowsurface->Flip( windowsurface,NULL,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
	draw_map(); 		
	update_hscore();	
	update_cscore();
}


void Game2048Frame::left_updatemap()
{
    int i, j, k;
    bool move = false, combine = false,onemove=false;
//	std::cout<<"left_updatemap"<<std::endl;
        for (i = 0; i < 4; i++)
        {
            j = 0, k = 0;
            while (true)
            {
            	onemove=false;
                while (j < 4 && allunits[i][j] == 0)
                    j++;
                if (j > 3)
                    break;
                if (j != k){
					std::swap(allunits[i][k], allunits[i][j]);
                    move = true;onemove=true;
					ani[i][j].start.x=IdToGetWidth(i*4+j);
					ani[i][j].stop.x=IdToGetWidth(i*4+k);
					ani[i][j].move=true;
					ani[i][j].score=allunits[i][k];
					ani[i][j].scale = (ani[i][j].start.x-ani[i][j].stop.x)/GAMEMOVESCALE;
					ani[i][j].direct=LEFT;
				}
                if (k > 0 && allunits[i][k] == allunits[i][k - 1] && !combine)
                {			
                    allunits[i][k - 1] <<= 1;
					if(onemove){						
						ani[i][j].stop.x=IdToGetWidth(i*4+(k-1));
						ani[i][j].scale = (ani[i][j].start.x-ani[i][j].stop.x)/GAMEMOVESCALE;
					}else{
						ani[i][j].start.x=IdToGetWidth(i*4+k);
						ani[i][j].stop.x=IdToGetWidth(i*4+(k-1));
						ani[i][j].move=true;
						ani[i][j].direct=LEFT;
						ani[i][j].scale = (ani[i][j].start.x-ani[i][j].stop.x)/GAMEMOVESCALE;						
						ani[i][j].score = allunits[i][k];
						move=true;onemove=true;
					}					
                    combine = true;
                    cscore += allunits[i][k - 1];
                    blankcount++;				
                    allunits[i][k] = 0;	
					g_game_move=true;
                }
                else
                    k++;
				if(onemove == false){
					ani[i][j].move=false;
					ani[i][j].score=allunits[i][j];
				}
                j++;
			}
			
			combine = false;
		}		
		if(move){
			gamemove=true;
			isanimations=true;
		}
}

void Game2048Frame::right_updatemap()
{
    int i, j, k;
    bool move = false, combine = false,onemove=false;
//	std::cout<<"right_updatemap"<<std::endl;
	for (i = 0; i < 4; i++)
	{
		j = 3, k = 3;
		while (true)
		{
			onemove=false;
			while (j > -1 && allunits[i][j] == 0)
				j--;
			if(j<0)break;
			if (j != k){
				std::swap(allunits[i][k], allunits[i][j]);
				move = true;onemove=true;
				ani[i][j].start.x=IdToGetWidth(i*4+j);
				ani[i][j].stop.x=IdToGetWidth(i*4+k);
				ani[i][j].score=allunits[i][k];				
				ani[i][j].scale = (ani[i][j].stop.x-ani[i][j].start.x)/GAMEMOVESCALE;
				ani[i][j].move=true;
				ani[i][j].direct=RIGHT;	
			}
			if (k < 3 && allunits[i][k] == allunits[i][k + 1] && !combine)
			{
				allunits[i][k + 1] <<= 1;
				if(onemove){
					ani[i][j].stop.x=IdToGetWidth(i*4+(k+1));
					ani[i][j].scale = (ani[i][j].stop.x-ani[i][j].start.x)/GAMEMOVESCALE;
				}else{
					ani[i][j].start.x=IdToGetWidth(i*4+k);
					ani[i][j].stop.x=IdToGetWidth(i*4+(k+1));
					ani[i][j].move=true;
					ani[i][j].score=allunits[i][k]; 			
					ani[i][j].direct=RIGHT;
					ani[i][j].scale = (ani[i][j].stop.x-ani[i][j].start.x)/GAMEMOVESCALE;
					move=true;onemove=true;
				}
				combine = true;
				cscore += allunits[i][k + 1];
				blankcount++;
				allunits[i][k] = 0;			
				g_game_move=true;
			}
			else
				k--;
			if(onemove == false){
				ani[i][j].move=false;
				ani[i][j].score=allunits[i][j];
				j--;
			}
		}
		
		combine = false;
	}
	if(move){
		isanimations=true;
		gamemove=true;
	}			

}


void Game2048Frame::up_updatemap()
{
    int i, j, k;
    bool move = false, combine = false,onemove=false;
//	std::cout<<"up_updatemap"<<std::endl;
	for (i = 0; i < 4; i++)
	{
		j = 0, k = 0;
		while (true)
		{
			onemove=false;
			while (j < 4 && allunits[j][i] == 0)
				j++;
			if (j > 3)
				break;
			if (j != k){			
				move = true;onemove=true;			
				std::swap(allunits[k][i], allunits[j][i]);
				ani[j][i].start.y=IdToGetHeight(i+j*4); 				
				ani[j][i].stop.y=IdToGetHeight(i+k*4);
				ani[j][i].move=true;
				ani[j][i].score = allunits[k][i];
				ani[j][i].direct=UP;
				ani[j][i].scale = (ani[j][i].start.y-ani[j][i].stop.y)/GAMEMOVESCALE;
			}
			if (k > 0 && allunits[k][i] == allunits[k - 1][i] && !combine)
			{
				allunits[k - 1][i] <<= 1;
				if(onemove){
					ani[j][i].stop.y=IdToGetHeight(i+(k-1)*4);					
					ani[j][i].scale = (ani[j][i].start.y-ani[j][i].stop.y)/GAMEMOVESCALE;
				}else{
					ani[j][i].start.y=IdToGetHeight(i+k*4); 				
					ani[j][i].stop.y=IdToGetHeight(i+(k-1)*4);					
					ani[j][i].score = allunits[k][i];
					ani[j][i].move=true;
					ani[j][i].direct=UP;
					ani[j][i].scale = (ani[j][i].start.y-ani[j][i].stop.y)/GAMEMOVESCALE;
					move=true;onemove=true;
				}
				combine = true;
				cscore += allunits[k - 1][i];
				blankcount++;		
				allunits[k][i] = 0;	
				g_game_move=true;
			}
			else
				k++;
			if(onemove == false){
				ani[j][i].move=false;
				ani[j][i].score=allunits[j][i];
				j++;
			}
		}
		
		combine = false;
	}
	if(move){
		isanimations=true;
		gamemove=true;
	}			

}
void Game2048Frame::down_updatemap()
{
    int i, j, k;
    bool move = false, combine = false,onemove=false;
//	std::cout<<"down_updatemap"<<std::endl;
	for (i = 0; i < 4; i++)
	{
		j = 3, k = 3;
		while (true)
		{
			onemove=false;
			while (j > -1 && allunits[j][i] == 0)
				j--;
			if (j < 0)
				break;
			if (j != k){				
				move = true;onemove=true;			
				std::swap(allunits[k][i], allunits[j][i]);
				ani[j][i].start.y=IdToGetHeight(i+j*4); 				
				ani[j][i].stop.y=IdToGetHeight(i+k*4);
				ani[j][i].move=true;
				ani[j][i].score = allunits[k][i];
				ani[j][i].direct=DOWN;
				ani[j][i].scale = (ani[j][i].stop.y-ani[j][i].start.y)/GAMEMOVESCALE;
				
			}
			if (k < 3 && allunits[k][i] == allunits[k + 1][i] && !combine)
			{
				allunits[k + 1][i] <<= 1;
				if(onemove){
					ani[j][i].stop.y=IdToGetHeight(i+(k+1)*4);					
					ani[j][i].scale = (ani[j][i].stop.y-ani[j][i].start.y)/GAMEMOVESCALE;
				}else{
					ani[j][i].start.y=IdToGetHeight(i+k*4); 				
					ani[j][i].stop.y=IdToGetHeight(i+(k+1)*4);					
					ani[j][i].move=true;
					ani[j][i].score = allunits[k][i];
					ani[j][i].direct=DOWN;
					ani[j][i].scale = (ani[j][i].stop.y-ani[j][i].start.y)/GAMEMOVESCALE;
					move=true;onemove=true;
				}
				combine = true;
				cscore += allunits[k + 1][i];
				blankcount++;		
				allunits[k][i] = 0;						
				g_game_move=true;
			}
			else
				k--;
			if(onemove == false){
				ani[j][i].move=false;
				ani[j][i].score=allunits[j][i];
				j--;
			}
		}
		
		combine = false;
	}
	if(move){
		isanimations=true;
		gamemove=true;
	}			
}
void Game2048Frame::gameover(int flag)
{
	DFBRectangle lrect;
	DFBSurfaceDescription	sdsc;
	IDirectFBSurface *tempsurf;

	if(flag){
		/* create the temporary surface */
		sdsc.flags		 = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS | DSDESC_PIXELFORMAT | DSDESC_WIDTH | DSDESC_HEIGHT);
		sdsc.caps		 = DSCAPS_PREMULTIPLIED;
		sdsc.pixelformat = DSPF_ARGB;
		sdsc.width		 = GAMECENTERWIDTH;
		sdsc.height 	 = GAMECENTERHEIGHT;
		if(dfb->CreateSurface( dfb, &sdsc, &tempsurf))printf("CreateSurface eror\n");
		tempsurf->Clear(tempsurf,0,0,0,0);
		tempsurf->SetColor(tempsurf,243,212,110,200);
		drawRoundRectangle(tempsurf,0,0,GAMECENTERWIDTH,GAMECENTERHEIGHT,GAMECIRCLER);
		windowsurface->Blit( windowsurface, tempsurf, NULL, GAMECENTERSTARTX, GAMECENTERSTARTY);
		tempsurf->Release(tempsurf);
		
		
		windowsurface->SetColor(windowsurface,0xff,0xff,0xff,0xff);
		windowsurface->SetFont (windowsurface, font_g);
		windowsurface->DrawString(windowsurface, "You Lost", -1, GAMEOVERSTARTX+GAMEOVERWIDTH/2,GAMEOVERSTARTY+GAMEOVERHEIGHT/2-30,(DFBSurfaceTextFlags) DSTF_CENTER);
		
		lrect.x=GAMEOVERSTARTX+GAMEOVERWIDTH/8+30;
		lrect.y=GAMEOVERSTARTY+GAMEOVERHEIGHT/8*5;
		lrect.w=GAMEWINWIDTH/4;
		lrect.h=GAMEOVERHEIGHT/4;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMEOVEREXIT,lrect,0);
		
		lrect.x=GAMEOVERSTARTX+GAMEOVERWIDTH/8*5-30;
		lrect.y=GAMEOVERSTARTY+GAMEOVERHEIGHT/8*5;
		lrect.w=GAMEWINWIDTH/4;
		lrect.h=GAMEOVERHEIGHT/4;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMEOVERRESTART,lrect,0);

	}	
	switch(overfocusId){
		case 0:
			lrect.x=GAMEOVERSTARTX+GAMEOVERWIDTH/8+30;
			lrect.y=GAMEOVERSTARTY+GAMEOVERHEIGHT/8*5;
			lrect.w=GAMEWINWIDTH/4;
			lrect.h=GAMEOVERHEIGHT/4;
			drawAlphaPic(windowsurface,(char *)IMAGEGAMEOVERFEXIT,lrect,0);
			break;
		case 1:
			lrect.x=GAMEOVERSTARTX+GAMEOVERWIDTH/8*5-30;
			lrect.y=GAMEOVERSTARTY+GAMEOVERHEIGHT/8*5;
			lrect.w=GAMEWINWIDTH/4;
			lrect.h=GAMEOVERHEIGHT/4;
			drawAlphaPic(windowsurface,(char *)IMAGEGAMEOVERFRESTART,lrect,0);
			break;
		default:break;

	}	
	

	DFBRegion lregion={GAMECENTERSTARTX,GAMECENTERSTARTY,GAMECENTERSTARTX+GAMECENTERWIDTH,GAMECENTERSTARTY+GAMECENTERHEIGHT};
	windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) 0 );	
	windowsurface->SetFont (windowsurface, font);
}
void Game2048Frame::gamewin(int flag)
{
	DFBRectangle lrect;
	DFBSurfaceDescription	sdsc;
	IDirectFBSurface *tempsurf;

	if(flag){
		/* create the temporary surface */
		sdsc.flags		 = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS | DSDESC_PIXELFORMAT | DSDESC_WIDTH | DSDESC_HEIGHT);
		sdsc.caps		 = DSCAPS_PREMULTIPLIED;
		sdsc.pixelformat = DSPF_ARGB;
		sdsc.width		 = GAMECENTERWIDTH;
		sdsc.height 	 = GAMECENTERHEIGHT;
		if(dfb->CreateSurface( dfb, &sdsc, &tempsurf))printf("CreateSurface eror\n");		
		tempsurf->Clear(tempsurf,0,0,0,0);
		tempsurf->SetColor(tempsurf,243,212,110,200);
		drawRoundRectangle(tempsurf,0,0,GAMECENTERWIDTH,GAMECENTERHEIGHT,GAMECIRCLER);
		windowsurface->SetBlittingFlags( windowsurface, (DFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL );
		windowsurface->SetPorterDuff( windowsurface, DSPD_SRC_OVER );
		windowsurface->Blit( windowsurface, tempsurf, NULL, GAMECENTERSTARTX, GAMECENTERSTARTY);
		tempsurf->Release(tempsurf);
		
		
		windowsurface->SetColor(windowsurface,0xff,0xff,0xff,0xff);
		windowsurface->SetFont (windowsurface, font_g);
		windowsurface->DrawString(windowsurface, "You Win", -1, GAMEWINSTARTX+GAMEWINWIDTH/2,GAMEWINSTARTY+GAMEWINHEIGHT/2-30,(DFBSurfaceTextFlags) DSTF_CENTER);
		
		lrect.x=GAMEWINSTARTX+GAMEWINWIDTH/8+30;
		lrect.y=GAMEWINSTARTY+GAMEWINHEIGHT/8*5;
		lrect.w=GAMEWINWIDTH/4;
		lrect.h=GAMEWINHEIGHT/4;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMEWINEXIT,lrect,0);
		
		lrect.x=GAMEWINSTARTX+GAMEWINWIDTH/8*5-30;
		lrect.y=GAMEWINSTARTY+GAMEWINHEIGHT/8*5;
		lrect.w=GAMEWINWIDTH/4;
		lrect.h=GAMEWINHEIGHT/4;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMEWINCONTINUE,lrect,0);

	}	
	
	switch(winfocusId){
		case 0:
			lrect.x=GAMEWINSTARTX+GAMEWINWIDTH/8+30;
			lrect.y=GAMEWINSTARTY+GAMEWINHEIGHT/8*5;
			lrect.w=GAMEWINWIDTH/4;
			lrect.h=GAMEWINHEIGHT/4;
			drawAlphaPic(windowsurface,(char *)IMAGEGAMEWINFEXIT,lrect,0);
			break;
		case 1:
			lrect.x=GAMEWINSTARTX+GAMEWINWIDTH/8*5-30;
			lrect.y=GAMEWINSTARTY+GAMEWINHEIGHT/8*5;
			lrect.w=GAMEWINWIDTH/4;
			lrect.h=GAMEWINHEIGHT/4;
			drawAlphaPic(windowsurface,(char *)IMAGEGAMEWINFCONTINUE,lrect,0);
			break;
		default:break;
		}
	DFBRegion lregion={GAMECENTERSTARTX,GAMECENTERSTARTY,GAMECENTERSTARTX+GAMECENTERWIDTH,GAMECENTERSTARTY+GAMECENTERHEIGHT};
	windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) 0 );	
	windowsurface->SetFont (windowsurface, font);
}


void Game2048Frame::play_animations()
{
	int i,j;	
	bool movelc=false;
//	std::cout<<"play_animations"<<std::endl;
	for( i=0;i<4;i++){
		for( j=0;j<4;j++){
			
			if(ani[i][j].move){
				switch(ani[i][j].direct){
					case RIGHT:
						if(ani[i][j].start.x < (ani[i][j].stop.x)){
							ani[i][j].start.x += ani[i][j].scale;
							ani[i][j].start.y = IdToGetHeight(i*4+j);
							movelc = true;
							if(ani[i][j].start.x > ani[i][j].stop.x){
								movelc = false;
								ani[i][j].start.x=ani[i][j].stop.x;	
								ani[i][j].move = false;
								ani[i][j].score = allunits[i][j];
							}
						}
						break;
					case LEFT:
						if(ani[i][j].start.x > (ani[i][j].stop.x)){
							ani[i][j].start.x -= ani[i][j].scale;					
							ani[i][j].start.y = IdToGetHeight(i*4+j);
							movelc = true;							
							if(ani[i][j].start.x < ani[i][j].stop.x){
								movelc = false;
								ani[i][j].start.x=ani[i][j].stop.x;								
								ani[i][j].move = false;
								ani[i][j].score = allunits[i][j];
							}
						}
						break;
					case UP:
						if(ani[i][j].start.y > (ani[i][j].stop.y)){
							ani[i][j].start.y -= ani[i][j].scale;
							ani[i][j].start.x = IdToGetWidth(i*4 +j);
							movelc = true;
							if(ani[i][j].start.y < ani[i][j].stop.y){
								movelc = false;
								ani[i][j].start.y=ani[i][j].stop.y;		
								ani[i][j].move = false;
								ani[i][j].score = allunits[i][j];
							}
						}
						break;
					case DOWN:
						if(ani[i][j].start.y < (ani[i][j].stop.y)){
							ani[i][j].start.y += ani[i][j].scale;
							ani[i][j].start.x = IdToGetWidth(i*4 +j);
							movelc = true;
							if(ani[i][j].start.y > ani[i][j].stop.y){
								movelc = false;
								ani[i][j].start.y=ani[i][j].stop.y;	
								ani[i][j].move = false;
								ani[i][j].score = allunits[i][j];
							}
						}
						break;
					default:break;					
				}
			}
		}
	}
	if(movelc){
		SetTimer(20,ID_GAME2048TIMER,E_TIMER_CONTI);
		draw_Animations();
	}
	else{
		KillTimer(ID_GAME2048TIMER);
			if(gamemove){
				gamemove=false;
				draw_map();						
				CreatOneUnit_ani(GetSrandUniqueId(),GetSrandNumber());	
				update_cscore();
				if(checkGameOver()){	
					gamemodeh=gamemode;
					gamemode=GAME_OVER;
					gameover(1);
				}
				if(game_conginueflag == false){
					if(checkWin()){								
						gamemodeh=gamemode;
						gamemode=GAME_WIN;
						gamewin(1);					
						game_conginueflag=true;
					}
				}
				
			}
		}
}
void Game2048Frame::draw_Animations()
{
	char buf[8];
	int i,j;	
	ColorRGB tlcolor;
	IDirectFBFont *llfont;
	int heightofllfont=0;
	
	windowsurface->SetColor(windowsurface,249,249,239,0xff);
	windowsurface->FillRectangle(windowsurface,GAMECENTERSTARTX,GAMECENTERSTARTY,GAMECENTERWIDTH,GAMECENTERHEIGHT);
//	std::cout<<"draw_Animations"<<std::endl;
	windowsurface->SetColor(windowsurface,187,173,160,0xff);	
	drawRoundRectangle(windowsurface,GAMECENTERSTARTX,GAMECENTERSTARTY,GAMECENTERWIDTH,GAMECENTERHEIGHT,GAMECIRCLER);
	for( i=0;i<4;i++){
		for( j=0;j<4;j++){
			windowsurface->SetColor(windowsurface,208,198,188,0xff);
			drawRoundRectangle(windowsurface,IdToGetWidth(i*4+j),IdToGetHeight(i*4+j),GAME2048UNITWIDTH,GAME2048UNITHEIGHT,GAMECIRCLER);				
		}
	}
	for( i=0;i<4;i++){
		for( j=0;j<4;j++){
			
		     if(ani[i][j].move ){
				//draw animations 				
				tlcolor = unitcolors[getBitCount(ani[i][j].score)];
				windowsurface->SetColor(windowsurface,tlcolor.r,tlcolor.g,tlcolor.b,0xff);
				drawRoundRectangle(windowsurface,ani[i][j].start.x,ani[i][j].start.y,GAME2048UNITWIDTH,GAME2048UNITHEIGHT,GAMECIRCLER);				
				snprintf( buf, sizeof(buf), "%d",ani[i][j].score);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
				if(ani[i][j].score>8)
					windowsurface->SetColor(windowsurface,187,173,160,0xff);
				else
					windowsurface->SetColor(windowsurface,0,0,0,0xff);
				
				if(ani[i][j].score<100)windowsurface->SetFont (windowsurface, font_g);
				else if(ani[i][j].score<10000)windowsurface->SetFont (windowsurface, font);
				else windowsurface->SetFont (windowsurface, font_l);
				
				windowsurface->GetFont(windowsurface,&llfont);
				llfont->GetHeight(llfont,&heightofllfont);
				windowsurface->DrawString(windowsurface, buf, -1, ani[i][j].start.x+GAME2048UNITWIDTH/2, ani[i][j].start.y+GAME2048UNITHEIGHT/2+heightofllfont/4,(DFBSurfaceTextFlags) DSTF_CENTER);
				llfont->Release(llfont);
				
			}else{
				if(ani[i][j].score > 0 && ani[i][j].score < 65535){ 			
					tlcolor = unitcolors[getBitCount(ani[i][j].score)];
					windowsurface->SetColor(windowsurface,tlcolor.r,tlcolor.g,tlcolor.b,0xff);
					drawRoundRectangle(windowsurface,IdToGetWidth(i*4+j),IdToGetHeight(i*4+j),GAME2048UNITWIDTH,GAME2048UNITHEIGHT,GAMECIRCLER);				
					snprintf( buf, sizeof(buf), "%d",ani[i][j].score);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
					if(ani[i][j].score>8)
						windowsurface->SetColor(windowsurface,187,173,160,0xff);
					else
						windowsurface->SetColor(windowsurface,0,0,0,0xff);
					if(ani[i][j].score<100)windowsurface->SetFont (windowsurface, font_g);
					else if(ani[i][j].score<10000)windowsurface->SetFont (windowsurface, font);
					else windowsurface->SetFont (windowsurface, font_l);
					
					windowsurface->GetFont(windowsurface,&llfont);
					llfont->GetHeight(llfont,&heightofllfont);
					windowsurface->DrawString(windowsurface, buf, -1, IdToGetWidth(i*4+j)+GAME2048UNITWIDTH/2, IdToGetHeight(i*4+j)+GAME2048UNITHEIGHT/2+heightofllfont/4,(DFBSurfaceTextFlags) DSTF_CENTER);
					llfont->Release(llfont);
				}
			}
		}
	}
	
	DFBRegion lregion={GAMECENTERSTARTX,GAMECENTERSTARTY,GAMECENTERSTARTX+GAMECENTERWIDTH-1,GAMECENTERSTARTY+GAMECENTERHEIGHT-1};
	windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
}
void Game2048Frame::draw_map()
{
	int i,j;
	char buf[8];
	ColorRGB tlcolor;		
	IDirectFBFont *llfont;
	int heightofllfont=0;
//	std::cout<<"draw_map"<<std::endl;
	windowsurface->SetColor(windowsurface,249,249,239,0xff);
	windowsurface->FillRectangle(windowsurface,GAMECENTERSTARTX,GAMECENTERSTARTY,GAMECENTERWIDTH,GAMECENTERHEIGHT);
	windowsurface->SetColor(windowsurface,187,173,160,0xff);
	drawRoundRectangle(windowsurface,GAMECENTERSTARTX,GAMECENTERSTARTY,GAMECENTERWIDTH,GAMECENTERHEIGHT,GAMECIRCLER);
	for( i=0;i<4;i++){	  
	   for( j=0;j<4;j++){	   
		
			if(allunits[i][j]){ 
				tlcolor = unitcolors[getBitCount(allunits[i][j])];
				windowsurface->SetColor(windowsurface,tlcolor.r,tlcolor.g,tlcolor.b,0xff);
				drawRoundRectangle(windowsurface,IdToGetWidth(i*4+j),IdToGetHeight(i*4+j),GAME2048UNITWIDTH,GAME2048UNITHEIGHT,GAMECIRCLER);				
				snprintf( buf, sizeof(buf), "%d",allunits[i][j]);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
				if(allunits[i][j]>8)
					windowsurface->SetColor(windowsurface,187,173,160,0xff);
				else
					windowsurface->SetColor(windowsurface,0,0,0,0xff);
				
				if(allunits[i][j]<100)windowsurface->SetFont (windowsurface, font_g);
				else if(allunits[i][j]<10000)windowsurface->SetFont (windowsurface, font);
				else windowsurface->SetFont (windowsurface, font_l);
				windowsurface->GetFont(windowsurface,&llfont);
				llfont->GetHeight(llfont,&heightofllfont);
				windowsurface->DrawString(windowsurface, buf, -1, IdToGetWidth(i*4+j)+GAME2048UNITWIDTH/2, IdToGetHeight(i*4+j)+GAME2048UNITHEIGHT/2+heightofllfont/4,(DFBSurfaceTextFlags) DSTF_CENTER);
				llfont->Release(llfont);
			}
			else{
				windowsurface->SetColor(windowsurface,208,198,188,0xff);
				drawRoundRectangle(windowsurface,IdToGetWidth(i*4+j),IdToGetHeight(i*4+j),GAME2048UNITWIDTH,GAME2048UNITHEIGHT,GAMECIRCLER);				
			}
	   }
	}	
	DFBRegion lregion={GAMECENTERSTARTX,GAMECENTERSTARTY,GAMECENTERSTARTX+GAMECENTERWIDTH-1,GAMECENTERSTARTY+GAMECENTERHEIGHT-1};
	windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
}

void Game2048Frame::update_cscore()
{
	char buf[8];
	DFBRectangle lrect;
	
	if(cscore >hscore){
		hscore=cscore;
	}	
	update_hscore();
	
	windowsurface->SetColor(windowsurface,249,249,239,0xff);	
	windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,CSCOREPOSHEIGHT,CSCOREWIDTH*2,CSCOREHEIGHT);			
	
	lrect.x=GAMEMAPRIGHTSTARTX+20;
	lrect.y=CSCOREPOSHEIGHT;
	lrect.w=CSCOREWIDTH-40;
	lrect.h=CSCOREHEIGHT;
	drawAlphaPic(windowsurface,(char *)IMAGEGAMESCORE,lrect,0);
	
	windowsurface->SetFont (windowsurface, font_l);			
	//	std::cout<<"update_score"<<std::endl;
	windowsurface->SetColor(windowsurface,193,178,157,0xff);	
	snprintf( buf, sizeof(buf), "%d",cscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
	windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+CSCOREWIDTH/2*3,CSCOREPOSHEIGHT+ CSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
	DFBRegion lregion={GAMEMAPRIGHTSTARTX,CSCOREPOSHEIGHT,width,CSCOREPOSHEIGHT+CSCOREHEIGHT-1};
	windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
	windowsurface->SetFont (windowsurface, font);			
}

void Game2048Frame::update_hscore()
{
	char buf[8];
	DFBRectangle lrect;
	//	std::cout<<"update_score"<<std::endl;
	
	windowsurface->SetColor(windowsurface,249,249,239,0xff);	
	windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,HSCOREPOSHEIGHT,HSCOREWIDTH*2,HSCOREHEIGHT);			
	
	lrect.x=GAMEMAPRIGHTSTARTX+20;
	lrect.y=HSCOREPOSHEIGHT;
	lrect.w=HSCOREWIDTH-40;
	lrect.h=HSCOREHEIGHT;
	drawAlphaPic(windowsurface,(char *)IMAGEGAMEBEST,lrect,0);

	windowsurface->SetFont (windowsurface, font_l);		
	//	std::cout<<"update_score"<<std::endl;
	windowsurface->SetColor(windowsurface,193,178,157,0xff);	
	snprintf( buf, sizeof(buf), "%d",hscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
	windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+HSCOREWIDTH+HSCOREWIDTH/2,HSCOREPOSHEIGHT+ HSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
	DFBRegion lregion={GAMEMAPRIGHTSTARTX,HSCOREPOSHEIGHT,width,HSCOREPOSHEIGHT+HSCOREHEIGHT-1};
	windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
	windowsurface->SetFont (windowsurface, font);			
}

bool Game2048Frame::checkGameOver()
{
    // ѭ\BB\B7\BC\EC\B2\E2\CAǷ\F1\BA\AC\D3\D0\CF\E0\C1ڵ\C4\CF\E0ͬ\CA\FD\C2\EB
//    std::cout<<"blankcount="<<blankcount<<std::endl;
    if(blankcount>0)return false;
    for (int i = 0; i < 4; i++)
	{
        for (int j = 0; j < 4; j++)
        {
            if (j != 3 && allunits[i][j] == allunits[i][j + 1])
                return false;
            if (i != 3 && allunits[i][j] == allunits[i + 1][j])
                return false;
        }
    }
    return true;

}

bool Game2048Frame::checkWin()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (allunits[i][j] == 2048)
                return true;
    return false;
}


int Game2048Frame::getBitCount(int n)
{
    // ѭ\BB\B7\BB\F1ȡ\CA\FD\D7ֶ\FE\BD\F8\D6\C6λ\CA\FD
    int c = 0;
    while (n >>= 1)
        c++;
    // \B7\B5\BB\D8λ\CA\FD-1
    return c - 1;
}

bool Game2048Frame::game_mode_nomal( U32 keycode)
{
	g_game_move=false;
	switch(keycode){
		
		case MVK_EXIT:		
			MSystem::GetInstance()->uState = E_SYSTEM_STATE_SHUTDOWN;
			GetParentApp()->Kill();
		  break; 
		  
		case MVK_SELECT:	
	    case MVK_MENU:
			gamemodeh=gamemode;
			gamemode=GAME_MENU;
			play_menu_enter();
		    break;	
		case MVK_INFO:
			gamemodeh=gamemode;
			gamemode=GAME_INFO;
			play_info_enter();
			break;	
		case MVK_UP:	
//			record_history();
//			up_updatemap();
			gameover(1);
			break;
		case MVK_DOWN:	  
//			record_history();
//			down_updatemap();
			gamewin(1);
			break;
		case MVK_LEFT:			
			record_history();
			left_updatemap();
			break;
		case MVK_RIGHT:   
			record_history();
			right_updatemap();
			break;
		default:
			break;
	}
	
	
	if(isanimations == true){	
        pthread_mutex_lock(&m_MutSound);
        pthread_cond_signal(&m_CondSound);
        pthread_mutex_unlock(&m_MutSound);
		SetTimer(1,ID_GAME2048TIMER,E_TIMER_CONTI);
		isanimations=false;
	}  
	return true;
}
bool Game2048Frame::game_mode_menu( U32 keycode)
{
	int i,j;
	switch(keycode){
		case MVK_EXIT:
			MSystem::GetInstance()->uState = E_SYSTEM_STATE_SHUTDOWN;
			GetParentApp()->Kill();
		  break;  		  
		case MVK_INFO:
			gamemodeh=gamemode;
			gamemode=GAME_INFO;
			play_info_enter();
			return true;
		case MVK_RETURN:
//			std::cout<<"back ward"<<std::endl;
			switch(gamemodeh){
				case GAME_NOMEL:
				case GAME_INFO:
				case GAME_MENU: 					
					gamemodeh=gamemode;
					gamemode=GAME_NOMEL;				
					play_menu_out();
					break;
				case GAME_OVER:
					gamemodeh=gamemode;
					gamemode=GAME_OVER;	
					play_info_out();
					break;
				case GAME_WIN:
					gamemodeh=gamemode;
					gamemode=GAME_WIN;	
					play_info_out();
					break;
			}
			return true;	
		case MVK_UP:	
			menufocusId--;
			if(menufocusId<0)menufocusId=2;	
			draw_menu(width-GAMEMAPRIGHTWIDTH);
			break;
		case MVK_DOWN:	 
			menufocusId++;
			if(menufocusId>2)menufocusId=0;
			draw_menu(width-GAMEMAPRIGHTWIDTH);
			break;
		case MVK_SELECT:  
			switch(menufocusId){
				case 0:
					MSystem::GetInstance()->uState = E_SYSTEM_STATE_SHUTDOWN;
					GetParentApp()->Kill();
					return true;
				case 1:				
					gamemodeh=gamemode;
					gamemode=GAME_NOMEL;	
					start_game();
					init_map();
					return true;
				case 2:
					gamemodeh=gamemode;
					gamemode=GAME_NOMEL;
					if(hblankcountflag){
						blankcount = hblankcount;
						cscore = hcscore;
						hscore = hhscore;
						for( i=0;i<4;i++){
							for( j=0;j<4;j++){
								allunits[i][j] = hallunits[i][j];
							}
						}	
						init_map();
					}
					return true;
				default:break;

			}
				break;
		default:
			break;
	}
	return true;

}
bool Game2048Frame::game_mode_info( U32 keycode)
{
	
	switch(keycode){
		case MVK_EXIT:
			MSystem::GetInstance()->uState = E_SYSTEM_STATE_SHUTDOWN;
			GetParentApp()->Kill();
		  break;  		  
	    case MVK_MENU:
			gamemodeh=gamemode;
			gamemode=GAME_MENU;
			play_menu_enter();
		    break;	
		case MVK_RETURN:
			std::cout<<"back ward"<<std::endl;			
			switch(gamemodeh){
				case GAME_NOMEL:
				case GAME_INFO:
				case GAME_MENU: 				
					gamemodeh=gamemode;
					gamemode=GAME_NOMEL;				
					play_info_out();
					break;
				case GAME_OVER:
					gamemodeh=gamemode;
					gamemode=GAME_OVER;	
					play_info_out();
					break;
				case GAME_WIN:
					gamemodeh=gamemode;
					gamemode=GAME_WIN;	
					play_info_out();
					break;
			}
			break;	
		default:
			break;
	}
	return true;

}
bool Game2048Frame::game_mode_over( U32 keycode)
{	
	switch(keycode){
		case MVK_EXIT:
			MSystem::GetInstance()->uState = E_SYSTEM_STATE_SHUTDOWN;
			GetParentApp()->Kill();
		  break;  		  
	    case MVK_MENU:
			gamemodeh=gamemode;
			gamemode=GAME_MENU;
			play_menu_enter();
		    break;	
		case MVK_INFO:
			gamemodeh=gamemode;
			gamemode=GAME_INFO;	
			play_info_enter();
			break;	
		case MVK_RETURN:
			break;	
		case MVK_LEFT:	
			overfocusId--;
			if(overfocusId<0)overfocusId=1; 
			gameover(0);
			break;
		case MVK_RIGHT:	 
			overfocusId++;
			if(overfocusId>1)overfocusId=0;
			gameover(0);
			break;
		case MVK_SELECT:  
			switch(overfocusId){
				case 0:
					MSystem::GetInstance()->uState = E_SYSTEM_STATE_SHUTDOWN;
					GetParentApp()->Kill();
					return true;
				case 1: 			
					gamemodeh=gamemode;
					gamemode=GAME_NOMEL;	
					start_game();
					init_map();
					return true;
				default:break;
		
			}
			break;

		default:
			break;
	}
	return true;
	
}
bool Game2048Frame::game_mode_win( U32 keycode)
{
	switch(keycode){
		case MVK_EXIT:
			MSystem::GetInstance()->uState = E_SYSTEM_STATE_SHUTDOWN;
			GetParentApp()->Kill();
		  break;  		  
	    case MVK_MENU:
			gamemodeh=gamemode;
			gamemode=GAME_MENU;
			play_menu_enter();
		    break;	
		case MVK_INFO:
			gamemodeh=gamemode;
			gamemode=GAME_INFO;
			play_info_enter();
			break;	
		
		case MVK_RETURN:
			break;	
		case MVK_LEFT:	
			winfocusId--;
			if(winfocusId<0)winfocusId=1; 
			gamewin(0);
			break;
		case MVK_RIGHT:  
			winfocusId++;
			if(winfocusId>1)winfocusId=0;
			gamewin(0);
			break;
		case MVK_SELECT:  
			switch(winfocusId){
				case 0:
					MSystem::GetInstance()->uState = E_SYSTEM_STATE_SHUTDOWN;
					GetParentApp()->Kill();
					return true;
				case 1: 			
					gamemodeh=gamemode;
					gamemode=GAME_NOMEL;	
					init_map();
					return true;
				default:break;
		
			}
			break;

		default:
			break;
	}
	return true;
}
void Game2048Frame::draw_menu(int startxxx)
{	
	DFBResult ret;	
	DFBRectangle lrect;
	IDirectFBImageProvider *lprovider;
	
	windowsurface->SetColor(windowsurface,68,62,56,0xff); 	
	windowsurface->FillRectangle(windowsurface,startxxx,0,GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTHEIGHT);
	windowsurface->SetColor(windowsurface,238,228,208,0xff);
	windowsurface->DrawString(windowsurface, "menu", -1,startxxx+GAMEMAPRIGHTWIDTH/2,100,(DFBSurfaceTextFlags) DSTF_CENTER);


	ret = dfb->CreateImageProvider (dfb, IMAGEGAMEMENUEXITN, &lprovider);
	if(ret)printf("CreateImageProvider fail\n");
	else{
		lrect.x=startxxx;
		lrect.y=GAMEMAPRIGHTSTARTY+GAMEMAPRIGHTHEIGHT/4;
		lrect.w=GAMEMAPRIGHTWIDTH;
		lrect.h=GAMEMAPRIGHTHEIGHT/8;
		lprovider->RenderTo (lprovider,windowsurface, &lrect);
		lprovider->Release(lprovider);
	} 

	ret = dfb->CreateImageProvider (dfb, IMAGEGAMEMENURESTARTN, &lprovider);
	if(ret)printf("CreateImageProvider fail\n");
	else{
		lrect.x=startxxx;
		lrect.y=GAMEMAPRIGHTSTARTY+GAMEMAPRIGHTHEIGHT/2;
		lrect.w=GAMEMAPRIGHTWIDTH;
		lrect.h=GAMEMAPRIGHTHEIGHT/8;
		lprovider->RenderTo (lprovider,windowsurface, &lrect);
		lprovider->Release(lprovider);
	}
	ret = dfb->CreateImageProvider (dfb, IMAGEGAMEMENUBACKONEN, &lprovider);
	if(ret)printf("CreateImageProvider fail\n");
	else{
		lrect.x=startxxx;
	   lrect.y=GAMEMAPRIGHTSTARTY+GAMEMAPRIGHTHEIGHT/4*3;
	   lrect.w=GAMEMAPRIGHTWIDTH;
	   lrect.h=GAMEMAPRIGHTHEIGHT/8;
	   lprovider->RenderTo (lprovider,windowsurface, &lrect);
	   lprovider->Release(lprovider);
	}

		switch(menufocusId){
			case 0:				
				ret = dfb->CreateImageProvider (dfb, IMAGEGAMEMENUEXIT, &lprovider);
				if(ret)printf("CreateImageProvider fail\n");
				else{
					lrect.x=startxxx;
					lrect.y=GAMEMAPRIGHTSTARTY+GAMEMAPRIGHTHEIGHT/4;
					lrect.w=GAMEMAPRIGHTWIDTH;
					lrect.h=GAMEMAPRIGHTHEIGHT/8;
					lprovider->RenderTo (lprovider,windowsurface, &lrect);
					lprovider->Release(lprovider);
				} 
				break;
			case 1:
				ret = dfb->CreateImageProvider (dfb, IMAGEGAMEMENURESTART, &lprovider);
				if(ret)printf("CreateImageProvider fail\n");
				else{
					lrect.x=startxxx;
					lrect.y=GAMEMAPRIGHTSTARTY+GAMEMAPRIGHTHEIGHT/2;
					lrect.w=GAMEMAPRIGHTWIDTH;
					lrect.h=GAMEMAPRIGHTHEIGHT/8;
					lprovider->RenderTo (lprovider,windowsurface, &lrect);
					lprovider->Release(lprovider);
				}
				break;
			case 2:		
				ret = dfb->CreateImageProvider (dfb, IMAGEGAMEMENUBACKONE, &lprovider);
				if(ret)printf("CreateImageProvider fail\n");
				else{
					lrect.x=startxxx;
				    lrect.y=GAMEMAPRIGHTSTARTY+GAMEMAPRIGHTHEIGHT/4*3;
					lrect.w=GAMEMAPRIGHTWIDTH;
				    lrect.h=GAMEMAPRIGHTHEIGHT/8;
				    lprovider->RenderTo (lprovider,windowsurface, &lrect);
				    lprovider->Release(lprovider);
				}
				break;
			default:break;

		}	
		
		DFBRegion lregion={GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTSTARTX+GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTSTARTY+GAMEMAPRIGHTHEIGHT};
		windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
}

void Game2048Frame::draw_info(int startxxx)
{
	DFBResult ret;	
	DFBRectangle lrect;

	IDirectFBImageProvider *lprovider;
	ret = dfb->CreateImageProvider (dfb, IMAGEGAMEINFO, &lprovider);
	if(ret)printf("CreateImageProvider fail\n");
	else{
		lrect.x=startxxx;
		lrect.y=GAMEMAPRIGHTSTARTY;
		lrect.w=GAMEMAPRIGHTWIDTH;
		lrect.h=GAMEMAPRIGHTHEIGHT;
		lprovider->RenderTo (lprovider,windowsurface, &lrect);
		lprovider->Release(lprovider);
	}

	DFBRegion lregion={GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTSTARTX+GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTSTARTY+GAMEMAPRIGHTHEIGHT};
	windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
}


void Game2048Frame::record_history()
{
	int i,j;
		hblankcount = blankcount;
		hblankcountflag=true;
		hcscore = cscore;
		hhscore = hscore;
		for( i=0;i<4;i++){	  
		   for( j=0;j<4;j++){ 
			  hallunits[i][j] = allunits[i][j];
		   }
		}	  
}

void Game2048Frame::start_game()
{
		memset(allunits,0,sizeof(allunits));
		memset(ani,0,sizeof(ani));
		realcount=0;
		blankcount=16;
		hblankcount=0;
		hblankcountflag=false;
		isanimations=false;
		gamemove=false;
		game_conginueflag=false;
		cscore=0;
		hscore=0;
		gamemodeh=GAME_NOMEL;
		gamemode=GAME_NOMEL;
		menufocusId=0;
		winfocusId=0;
		overfocusId=0;
	//	ret = dfb->CreateVideoProvider( dfb, "",&videoprovider);
	//	if(ret)printf("CreateVideoProvider fail\n");
	//	else printf("CreateVideoProvider sucess\n");
	//	videoprovider->PlayTo( videoprovider, videosurface,NULL, NULL, NULL ));
	
		ifstream in(HSCOREFILE);
		if(!in.is_open())
		{
			FILE *a;
			std::cout <<  "open error" << std::endl;
			a = fopen(HSCOREFILE,"w+");
			if( a == NULL )
				std::cout << "create file error";
			else {
				fclose(a);
				std::cout<< "create file sucess";
				ofstream out(HSCOREFILE);
				out.is_open();
				out << hscore;
				in.close();
			}
		}
		else
		{
		std::cout << "open file sucess";
		in >> hscore;
		in.close();
		if(hscore<0 || hscore >65536)hscore=0;
		printf("hscore read form file = %d\n",hscore);
		}		
			
			
		CreatOneUnit(GetSrandUniqueId(),GetSrandNumber()); 
		CreatOneUnit(GetSrandUniqueId(),GetSrandNumber());

 
}

int Game2048Frame::IdToGetWidth(int id)
{
	return  (GAMECENTERSTARTX+(GAMECENTERHEIGHT-10)/4*(id%4)+GAMEMAPSCALEEWIDTH/5);
}
int Game2048Frame::IdToGetHeight(int id)
{
	return (GAMECENTERSTARTY+(GAMECENTERHEIGHT-10)/4*(id/4)+GAMEMAPSCALEEHEIGHT/5);
}
void Game2048Frame::game_startAnimations()
{
	DFBResult ret;	
	int k=0;
	DFBRectangle lrect;

	IDirectFBImageProvider *lprovider;
	ret = dfb->CreateImageProvider (dfb, IMAGEGAMESTART, &lprovider);
	if(ret)printf("CreateImageProvider fail\n");
	else{
			printf("CreateImageProvider sucess\n");
			k=0;
			
			windowsurface->Clear(windowsurface,0,0,0,0xff);
			while(k<20)
			{
				k ++;		
				lrect.x=width/2-k*25;
				lrect.y=height/2-k*15;
				lrect.w=k*50;
				lrect.h=k*30;
				lprovider->RenderTo (lprovider,windowsurface, &lrect);
				DFBRegion lregion={lrect.x,lrect.y,lrect.x+lrect.w,lrect.y+lrect.h};
				windowsurface->Flip( windowsurface, &lregion,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
			}
			k=10000;
			while(k)k--;
			
			k=20;
			while(k)
			{
				k -= 2;	
				windowsurface->Clear(windowsurface,0,0,0,0xff);
				lrect.x=width/2-k*25;
				lrect.y=height/2-k*15;
				lrect.w=k*50;
				lrect.h=k*30;
				lprovider->RenderTo (lprovider,windowsurface, &lrect);
				windowsurface->Flip( windowsurface, NULL,(DFBSurfaceFlipFlags) (DSFLIP_WAIT | DSFLIP_ONSYNC) );
			}
			lprovider->Release(lprovider);
		}	

}

void Game2048Frame::play_menu_enter()
{
	int k=0;
	char buf[4];
	DFBRectangle lrect;
	windowsurface->SetColor(windowsurface,249,249,239,0xff);
	windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTHEIGHT);					
	while(k<GAMEMAPRIGHTWIDTH)
	{
		k+=50;		
		if(k>GAMEMAPRIGHTWIDTH)k=GAMEMAPRIGHTWIDTH;
		
		windowsurface->SetColor(windowsurface,249,249,239,0xff);
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTHEIGHT);

		lrect.x=GAMEMAPRIGHTSTARTX+20;
		lrect.y=HSCOREPOSHEIGHT;
		lrect.w=HSCOREWIDTH-40;
		lrect.h=HSCOREHEIGHT;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMEBEST,lrect,0);

		lrect.x=GAMEMAPRIGHTSTARTX+20;
		lrect.y=CSCOREPOSHEIGHT;
		lrect.w=CSCOREWIDTH-40;
		lrect.h=CSCOREHEIGHT;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMESCORE,lrect,0);


		windowsurface->SetFont (windowsurface, font_l); 		
		//	std::cout<<"update_score"<<std::endl;
		windowsurface->SetColor(windowsurface,249,249,239,0xff);	
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX+CSCOREWIDTH,CSCOREPOSHEIGHT,CSCOREWIDTH,CSCOREHEIGHT);			
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX+HSCOREWIDTH,HSCOREPOSHEIGHT,HSCOREWIDTH,HSCOREHEIGHT);			
		windowsurface->SetColor(windowsurface,193,178,157,0xff);	
		snprintf( buf, sizeof(buf), "%d",cscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
		windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+CSCOREWIDTH/2*3,CSCOREPOSHEIGHT+ CSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
		snprintf( buf, sizeof(buf), "%d",hscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
		windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+HSCOREWIDTH+HSCOREWIDTH/2,HSCOREPOSHEIGHT+ HSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
		windowsurface->SetFont (windowsurface, font);			


		draw_menu(width-k);
	}
}
void Game2048Frame::play_menu_out()
{
	int k=0;
	DFBRectangle lrect;
	char buf[8];
	windowsurface->SetColor(windowsurface,249,249,239,0xff);
	windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTHEIGHT);
	while(k<GAMEMAPRIGHTWIDTH)
	{
		k+=50;	
		if(k>GAMEMAPRIGHTWIDTH)k=GAMEMAPRIGHTWIDTH;
		
		windowsurface->SetColor(windowsurface,249,249,239,0xff);
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTHEIGHT);

		lrect.x=GAMEMAPRIGHTSTARTX+20;
		lrect.y=HSCOREPOSHEIGHT;
		lrect.w=HSCOREWIDTH-40;
		lrect.h=HSCOREHEIGHT;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMEBEST,lrect,0);

		lrect.x=GAMEMAPRIGHTSTARTX+20;
		lrect.y=CSCOREPOSHEIGHT;
		lrect.w=CSCOREWIDTH-40;
		lrect.h=CSCOREHEIGHT;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMESCORE,lrect,0);


		windowsurface->SetFont (windowsurface, font_l); 		
		//	std::cout<<"update_score"<<std::endl;
		windowsurface->SetColor(windowsurface,249,249,239,0xff);	
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX+CSCOREWIDTH,CSCOREPOSHEIGHT,CSCOREWIDTH,CSCOREHEIGHT);			
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX+HSCOREWIDTH,HSCOREPOSHEIGHT,HSCOREWIDTH,HSCOREHEIGHT);			
		windowsurface->SetColor(windowsurface,193,178,157,0xff);	
		snprintf( buf, sizeof(buf), "%d",cscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
		windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+CSCOREWIDTH/2*3,CSCOREPOSHEIGHT+ CSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
		snprintf( buf, sizeof(buf), "%d",hscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
		windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+HSCOREWIDTH+HSCOREWIDTH/2,HSCOREPOSHEIGHT+ HSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
		windowsurface->SetFont (windowsurface, font);			

		draw_menu(GAMEMAPRIGHTSTARTX+k);
	}	
}
void Game2048Frame::play_info_enter()
{
	int k=0;
	char buf[8];
	DFBRectangle lrect;
	windowsurface->SetColor(windowsurface,249,249,239,0xff);
	windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTHEIGHT);					
	while(k<GAMEMAPRIGHTWIDTH)
	{
		k+=50;		
		if(k>GAMEMAPRIGHTWIDTH)k=GAMEMAPRIGHTWIDTH;
		
		windowsurface->SetColor(windowsurface,249,249,239,0xff);
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTHEIGHT);

		lrect.x=GAMEMAPRIGHTSTARTX+20;
		lrect.y=HSCOREPOSHEIGHT;
		lrect.w=HSCOREWIDTH-40;
		lrect.h=HSCOREHEIGHT;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMEBEST,lrect,0);

		lrect.x=GAMEMAPRIGHTSTARTX+20;
		lrect.y=CSCOREPOSHEIGHT;
		lrect.w=CSCOREWIDTH-40;
		lrect.h=CSCOREHEIGHT;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMESCORE,lrect,0);


		windowsurface->SetFont (windowsurface, font_l); 		
		//	std::cout<<"update_score"<<std::endl;
		windowsurface->SetColor(windowsurface,249,249,239,0xff);	
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX+CSCOREWIDTH,CSCOREPOSHEIGHT,CSCOREWIDTH,CSCOREHEIGHT);			
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX+HSCOREWIDTH,HSCOREPOSHEIGHT,HSCOREWIDTH,HSCOREHEIGHT);			
		windowsurface->SetColor(windowsurface,193,178,157,0xff);	
		snprintf( buf, sizeof(buf), "%d",cscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
		windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+CSCOREWIDTH/2*3,CSCOREPOSHEIGHT+ CSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
		snprintf( buf, sizeof(buf), "%d",hscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
		windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+HSCOREWIDTH+HSCOREWIDTH/2,HSCOREPOSHEIGHT+ HSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
		windowsurface->SetFont (windowsurface, font);			

		draw_info(width-k);
	}

}
void Game2048Frame::play_info_out()
{
	int k=0;
	DFBRectangle lrect;
	windowsurface->SetColor(windowsurface,249,249,239,0xff);
	windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTHEIGHT);
	while(k<GAMEMAPRIGHTWIDTH)
	{
		k+=50;	
		char buf[8];
		if(k>GAMEMAPRIGHTWIDTH)k=GAMEMAPRIGHTWIDTH;
		windowsurface->SetColor(windowsurface,249,249,239,0xff);
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX,GAMEMAPRIGHTSTARTY,GAMEMAPRIGHTWIDTH,GAMEMAPRIGHTHEIGHT);

		lrect.x=GAMEMAPRIGHTSTARTX+20;
		lrect.y=HSCOREPOSHEIGHT;
		lrect.w=HSCOREWIDTH-40;
		lrect.h=HSCOREHEIGHT;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMEBEST,lrect,0);

		lrect.x=GAMEMAPRIGHTSTARTX+20;
		lrect.y=CSCOREPOSHEIGHT;
		lrect.w=CSCOREWIDTH-40;
		lrect.h=CSCOREHEIGHT;
		drawAlphaPic(windowsurface,(char *)IMAGEGAMESCORE,lrect,0);


		windowsurface->SetFont (windowsurface, font_l); 		
		//	std::cout<<"update_score"<<std::endl;
		windowsurface->SetColor(windowsurface,249,249,239,0xff);	
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX+CSCOREWIDTH,CSCOREPOSHEIGHT,CSCOREWIDTH,CSCOREHEIGHT);			
		windowsurface->FillRectangle(windowsurface,GAMEMAPRIGHTSTARTX+HSCOREWIDTH,HSCOREPOSHEIGHT,HSCOREWIDTH,HSCOREHEIGHT);			
		windowsurface->SetColor(windowsurface,193,178,157,0xff);	
		snprintf( buf, sizeof(buf), "%d",cscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
		windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+CSCOREWIDTH/2*3,CSCOREPOSHEIGHT+ CSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
		snprintf( buf, sizeof(buf), "%d",hscore);//\B8\F1ʽ\BB\AF\B3\C9\D7ַ\FB\B4\AE				
		windowsurface->DrawString(windowsurface, buf, -1, GAMEMAPRIGHTSTARTX+HSCOREWIDTH+HSCOREWIDTH/2,HSCOREPOSHEIGHT+ HSCOREHEIGHT/2+10, (DFBSurfaceTextFlags)(DSTF_CENTER));
		windowsurface->SetFont (windowsurface, font);			
		draw_info(GAMEMAPRIGHTSTARTX+k);
	}
}

#define PI 3.1415
void Game2048Frame::drawRoundRectangle(IDirectFBSurface *windowsurface,int x,int y,int width,int height,char r)
{

	const float lscale=6.0;
	int i=0;
	float ldegree=PI/2.0/lscale;	
	float cdegree=0;
	windowsurface->FillRectangle(windowsurface,x+r-1,y,width-2*r+2,height);
	windowsurface->FillRectangle(windowsurface,x,y+r-1,width,height-2*r+2);
	// 1
	cdegree=0;
	for(i=0;i<lscale;i++)
	{
		windowsurface->FillTriangle(windowsurface,x+r-r*cos(cdegree),y+r-r*sin(cdegree),
												  x+r-r*cos(cdegree+ldegree),y+r-r*sin(cdegree+ldegree),
												  x+r,y+r);
		cdegree+=ldegree;
	}
	
	// 2
	cdegree=0;
	for(i=0;i<lscale;i++)
	{
		windowsurface->FillTriangle(windowsurface,x+width-r+r*cos(cdegree),y+r-r*sin(cdegree),
												  x+width-r+r*cos(cdegree+ldegree),y+r-r*sin(cdegree+ldegree),
												  x+width-r,y+r);
		cdegree+=ldegree;
	}
	
	// 3
	cdegree=0;
	for(i=0;i<lscale;i++)
	{
		windowsurface->FillTriangle(windowsurface,x+width-r+r*cos(cdegree),y+height-r+r*sin(cdegree),
												  x+width-r+r*cos(cdegree+ldegree),y+height-r+r*sin(cdegree+ldegree),
												  x+width-r,y+height-r);
		cdegree+=ldegree;
	}
	
	// 4	
	cdegree=0;
	for(i=0;i<lscale;i++)
	{
		windowsurface->FillTriangle(windowsurface,x+r-r*cos(cdegree),y+height-r+r*sin(cdegree),
												  x+r-r*cos(cdegree+ldegree),y+height-r+r*sin(cdegree+ldegree),
												  x+r,y+height-r);
		cdegree+=ldegree;
	}
}

void Game2048Frame::drawRoundRectangle_c(IDirectFBSurface *windowsurface,int x,int y,int width,int height,char r,const float scalse)
{

	const float lscale=scalse;
	int i=0;
	float ldegree=PI/2.0/lscale;	
	float cdegree=0;
	windowsurface->FillRectangle(windowsurface,x+r-1,y,width-2*r+2,height);
	windowsurface->FillRectangle(windowsurface,x,y+r-1,width,height-2*r+2);
	// 1
	cdegree=0;
	for(i=0;i<lscale;i++)
	{
		windowsurface->FillTriangle(windowsurface,x+r-r*cos(cdegree),y+r-r*sin(cdegree),
												  x+r-r*cos(cdegree+ldegree),y+r-r*sin(cdegree+ldegree),
												  x+r,y+r);
		cdegree+=ldegree;
	}
	
	// 2
	cdegree=0;
	for(i=0;i<lscale;i++)
	{
		windowsurface->FillTriangle(windowsurface,x+width-r+r*cos(cdegree),y+r-r*sin(cdegree),
												  x+width-r+r*cos(cdegree+ldegree),y+r-r*sin(cdegree+ldegree),
												  x+width-r,y+r);
		cdegree+=ldegree;
	}
	
	// 3
	cdegree=0;
	for(i=0;i<lscale;i++)
	{
		windowsurface->FillTriangle(windowsurface,x+width-r+r*cos(cdegree),y+height-r+r*sin(cdegree),
												  x+width-r+r*cos(cdegree+ldegree),y+height-r+r*sin(cdegree+ldegree),
												  x+width-r,y+height-r);
		cdegree+=ldegree;
	}
	
	// 4	
	cdegree=0;
	for(i=0;i<lscale;i++)
	{
		windowsurface->FillTriangle(windowsurface,x+r-r*cos(cdegree),y+height-r+r*sin(cdegree),
												  x+r-r*cos(cdegree+ldegree),y+height-r+r*sin(cdegree+ldegree),
												  x+r,y+height-r);
		cdegree+=ldegree;
	}
}


