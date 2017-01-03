#pragma once
#ifndef PT_NPKCRYPT_H
#define PT_NPKCRYPT_H
//////////////////////////////////////////////////////////////////////////
//
// Űũ��Ʈ ��� ������ ��Ŭ��� �մϴ�. 
//

#ifndef	_nProtect_For_Debug
#include "npkcrypt.h"
#endif

class CKeyCrypt
{

public:
	CKeyCrypt(void);
	~CKeyCrypt(void);


	BOOL OnLoadKeyCrypt();
	BOOL OnExitKeyCrypt();
	BOOL OnRegisterwindow(HWND hEdit);

//////////////////////////////////////////////////////////////////////////
//
// Űũ��Ʈ �ڵ��� ���� �մϴ�. �ڵ��� Űũ��Ʈ�� ����ɶ� �ʿ��ϹǷ�, ���� ����
// ������ ��� ������ ���� �ϼž� �մϴ�.
//

public:
	HKCRYPT		m_hKCrypt;

//////////////////////////////////////////////////////////////////////////
};
#endif

#ifdef PT_NPKCRYPT_CPP

	class CKeyCrypt KeyCrypt;

#else

	extern class CKeyCrypt KeyCrypt;

#endif
