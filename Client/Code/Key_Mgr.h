#ifndef KeyMagr_h__
#define KeyMagr_h__

#include "Base.h"

#include "Define.h"
#include "Engine_Define.h"
#include "Export_Function.h"

#define KEY_LBUTTON		0X00000001
#define KEY_RBUTTON		0X00000002

#define KEY_W			0X00000004
#define KEY_S			0X00000008
#define KEY_A			0X00000010
#define KEY_D			0X00000020

#define KEY_SPACE		0X00000040
#define KEY_TAB			0X00000080

class CKeyMgr : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)

public:
	explicit CKeyMgr();
	~CKeyMgr() = default;

public:
	void Update_KeyManager();
	bool Key_Up(_ulong ulKey);
	bool Key_Down(_ulong _ulKey);
	bool Key_Pressing(_ulong _ulKey);

public:
	_bool Key_Pressing(_ulong _ulKey1, _ulong _ulKey2);
	_bool Key_PressingDir();

private:
	_ulong m_ulKey;
	_ulong m_ulKeyUP;
	_ulong m_ulKeyDOWN;

};

#endif // KeyMgr_h__