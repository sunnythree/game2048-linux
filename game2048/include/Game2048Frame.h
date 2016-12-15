#ifndef __HELLOFRAME_H__
#define __HELLOFRAME_H__

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
#include "MMainFrame.h"
#include "MApplet.h"
/*@ </Include> @*/

//(*InternalHeaders
#include "AutoGenHeader.h"
#include "AbsContainer.h"
#include "ListContainer.h"
#include "ImageButton.h"
#include "Image.h"
#include "TextBox.h"
#include "GridContainer.h"
#include "Button.h"
#include "ProgressBar.h"
//*)
#include "ImageButton.h"

#include<algorithm>
#include<list>
#include <pthread.h>



enum
{
    //(*Identifiers
    ID_Text1,
    ID_TextGameOver,
    ID_TextHighestScore,
    ID_GAME2048TIMER=100,
};

enum
{
	GAME_NOMEL,
	GAME_MENU,
	GAME_INFO,
	GAME_OVER,
	GAME_WIN
};

enum
{
	RIGHT,
	LEFT,
	UP,
	DOWN,
};

typedef struct {
int r;
int g;
int b;
}ColorRGB;

typedef struct {
	int x;
	int y;
}TPointI;

typedef struct {
	TPointI start;
	TPointI stop;
	int score;
	int direct;
	bool move;
	int scale;
}TAnimations;

class Game2048Frame : public MMainFrame
{
    DECLARE_EVENT_MAP();

public:

    // ------------------------------------------------------------
    // public constructors
    // ------------------------------------------------------------

    Game2048Frame(MApplet * pParentApp, Rect & stRect);
	~Game2048Frame();
    // ------------------------------------------------------------
    // public operations
    // ------------------------------------------------------------


    BOOL OnCreate(void * arg1, void * arg2, void * arg3);

    BOOL OnDestroy(void * arg1, void * arg2, void * arg3);

    BOOL OnTimer(void * arg1, void * arg2, void * arg3);

    BOOL OnKey(void* arg1, void* arg2, void* arg3);

	
	IDirectFBFont *font;
	IDirectFBFont *font_l;
	IDirectFBFont *font_g;
    DFBFontDescription desc;	
	IDirectFBSurface *windowsurface;
	IDirectFBWindow *pwindow;
	MSystem *sys;
	IDirectFB *dfb;
	//IDirectFBVideoProvider *videoprovider;
	

private:
	
	const int width;
	const int height;
	int IdToGetWidth(int id);
	int IdToGetHeight(int id);
	bool CreatOneUnit(int id,int number);
	bool CreatOneUnit_ani(int id,int number);
	int GetSrandNumber();
	int GetSrandUniqueId();
	int GetSrandAnyNumber(int denominator);
	void left_updatemap();
	void right_updatemap();
	void up_updatemap();
	void down_updatemap();

	void gameover(int flag);
	void gamewin(int flag);
	void play_animations();
	void draw_Animations();
	void draw_map();
	void init_map();
	void update_cscore();
	void update_hscore();
	void start_game();
	bool checkWin();
	bool checkGameOver();
	int getBitCount(int n);
	bool game_mode_nomal( U32 keycode);
	bool game_mode_menu( U32 keycode);
	bool game_mode_info( U32 keycode);
	bool game_mode_over( U32 keycode);
	bool game_mode_win( U32 keycode);
	void draw_menu(int startxxx);
	void draw_info(int startxxx);
	void play_menu_enter();
	void play_menu_out();
	void play_info_enter();
	void play_info_out();
	void record_history();
	void record_animations();
	void drawRoundRectangle(IDirectFBSurface *windowsurface,int x,int y,int width,int height,char r);	
	void drawRoundRectangle_c(IDirectFBSurface *windowsurface,int x,int y,int width,int height,char r,const float scalse);
	void game_startAnimations();	
	void drawAlphaPic(IDirectFBSurface *windowsurface,char * name,DFBRectangle lrect,char mode);

	
	int allunits[4][4];	
	int hallunits[4][4];
	TAnimations ani[4][4];
	int realcount;
	int blankcount;
	int hblankcount;
	bool hblankcountflag;
	int cscore;
	int hcscore;
	int hscore;
	int hhscore;
	bool isanimations;
	bool gamemove;
	bool game_conginueflag;
	int gamemode;
	int gamemodeh;
	char menufocusId;
	char winfocusId;
	char overfocusId;	
	pthread_t tThreadSoundID;	
};

#endif
