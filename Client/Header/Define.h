#ifndef Define_h__
#define Define_h__

/* Define */
#define		WINCX 800
#define		WINCY 600


#define		VTXCNTX 129
#define		VTXCNTZ 129
#define		VTXITV  1

#define		OBJ_DEAD	1
#define		OBJ_NOEVENT 0


/* Enum */
enum RESOURCEID {
	RESOURCE_STATIC,
	RESOURCE_LOGO,
	RESOURCE_STAGE,
	RESOURCE_PLAYER,
	RESOURCE_END
};

enum LOADINGID {
	LOADING_STAGE,
	LOADING_BOSS,
	LOADING_END
};

enum ENVIR_ID {
	ENVIR_BACKGROUND,
	ENVIR_TERRAIN,
	ENVIR_SKYBOX,
	ENVIR_END
};

enum LOGIC_ID {
	LOGIC_OBJECT,
	LOGIC_PLAYER,
	LOGIC_MONSTER,
	LOGIC_PLAYERWEAPON,
	LOGIC_MONSTERWEAPON,
	LOGIC_PLAYERULTIMATE,
	LOGIC_ITEM,
	LOGIC_INTERACTION,
	LOGIC_CAMERA,
	LOGIC_END
};

enum UI_ID {
	UI_LOGO,
	UI_PLAYER,
	UI_EFFECT,
	UI_END
};

/* Struct */
typedef struct tagFrameInfo {
	float fFrameStart;
	float fFrameEnd;
}FRAME;

/* Extern */
extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

const float			g_Gravity = 9.8f;
#endif // Define_h__