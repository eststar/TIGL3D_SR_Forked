#ifndef Engine_Define_h__
#define Engine_Define_h__

#define		OBJ_DEAD	1
#define		OBJ_NOEVENT 0

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <ctime>

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

//0406 DBG_NEW 부분이 MFC와 겹쳐서 조건부로 정의.
#ifndef _AFX

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif
#else

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DEBUG_NEW 

#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DEBUG_NEW 

#endif
#endif

#endif

#pragma warning(disable : 4251)

using namespace std;

#endif // Engine_Define_h__
