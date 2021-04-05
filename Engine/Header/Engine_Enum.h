#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	//화면 창 or 전체모드
	enum WINMODE		{ MODE_FULL, MODE_WIN };

	//마우스 상태
	enum MOUSEKEYSTATE  { DIM_LB, DIM_RB, DIM_MB, DIM_END  };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	//월드행렬 관련
	enum INFO			{ INFO_RIGHT, 
						  INFO_UP, 
						  INFO_LOOK, 
						  INFO_POS, 
						  INFO_END };

	enum ROTATION		{ ROT_X, ROT_Y, ROT_Z, ROT_END };
	

	//System
	enum TIMERID { TIMER_IMMEDIATE, TIMER_FPS60, TIMER_END };
	enum FRAMEID { FRAME_FPS60, FRAME_END };
	enum FONTID  { FONT_DEFAULT, FONT_ACCENT, FONT_END};


	//Utility
	enum LAYERID		{ LAYER_ENVIR, LAYER_LOGIC, LAYER_UI, LAYER_END };
	enum COMPONENTID	{ COM_DYNAMIC, COM_STATIC, COM_END };

	//Resouces
	enum TEXTUREID		{ TEX_NORMAL, TEX_CUBE, TEX_END };

	enum BUFFERID		{ BUFFER_TRICOL, 
						  BUFFER_RCCOL, 
						  BUFFER_RCTEX, 
						  BUFFER_TERRAINTEX,
						  BUFFER_HTERRAINTEX,
						  BUFFER_CUBETEX,
						  BUFFER_TETRATEX,
						  BUFFER_END };

	enum RENDERID		{ RENDER_PRIORITY, 
						  RENDER_NONALPHA, 
						  RENDER_ALPHA, 
						  RENDER_UI, 
						  RENDER_END };

	enum TERRAINTYPE
	{
		TYPE_FLOOR, TYPE_WALL, TYPE_END,
	};

	enum TERRAINOPTION
	{
		OPTION_BLOCKED, OPTION_PASS, OPTION_END,
	};
}

#endif // Engine_Enum_h__
