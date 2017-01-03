
#ifdef _xTrap_GUARD

#include "IntNetwork.h"

#include <windows.h>
// [XTrapIC #1] XTrap.h ���� ��Ŭ���
#include "XTrap.h"
#include ".\xtrapgameclient.h"

#include "..\language.h"

//���̺귯�� ��Ŭ���
#pragma comment( lib, "ws2_32.lib")
#pragma comment( lib, "XTrapSrc\\XTrapIC_mt.lib" )

class CXTrapGameClient	g_XTrap;

CXTrapGameClient::CXTrapGameClient(void)
{
}

CXTrapGameClient::~CXTrapGameClient(void)
{
	XTrapWinsockEnd();
}

BOOL CXTrapGameClient::XTrap_StartUp(char *szGameTitle, char *szXTrap_MGR_ServerIP)
{
	if( szGameTitle==NULL ) 
		return false;

#ifdef __USE_XTRAP_MANAGER_LOG

	if( szXTrap_MGR_ServerIP==NULL )
		return false;

	SetXTrapMessagerIP(szXTrap_MGR_ServerIP);
#endif

// [XTrapIC #2] XTrap ���� �κ� ( Process�� Initial Entry Point �κ� )
#ifdef __XTRAPMODULE_H
	char	sGameName[MAX_PATH]	= {'\0',};

#ifdef _LANGUAGE_CHINESE
	DWORD	ApiVersion			= 0xA5001207;		//XTrap ����
	DWORD	VendorCode			= 0x13026020;		//WiseLogic���� �ذ�
	DWORD	KeyboardType		= 0x00000000;
	DWORD	PatchType			= 0x00000010;		// Enable : 0x10, Disable : 0x00
	DWORD	ModuleType			= 0x01FFFFF2;
#else
#ifdef _LANGUAGE_BRAZIL
	DWORD	ApiVersion			= 0xA5001150;		//XTrap ����
	DWORD	VendorCode			= 0x13026220;		//WiseLogic���� �ذ�
	DWORD	KeyboardType		= 0x00000000;
	DWORD	PatchType			= 0x00000010;		// Enable : 0x10, Disable : 0x00
	DWORD	ModuleType			= 0x0FFFFFFF;
#else
#ifdef _LANGUAGE_JAPANESE
	DWORD	ApiVersion			= 0xA5001150;		//XTrap ����
	DWORD	VendorCode			= 0x13026030;		//WiseLogic���� �ذ�
	DWORD	KeyboardType		= 0x00000000;
	DWORD	PatchType			= 0x00000010;		// Enable : 0x10, Disable : 0x00
	DWORD	ModuleType			= 0x01FFFFF2;
#else
#ifdef _LANGUAGE_THAI
	DWORD	ApiVersion			= 0xA5001150;		//XTrap ����
	DWORD	VendorCode			= 0x13026050;		//WiseLogic���� �ذ�
	DWORD	KeyboardType		= 0x00000000;
	DWORD	PatchType			= 0x00000010;		// Enable : 0x10, Disable : 0x00
	DWORD	ModuleType			= 0x01FFFFF2;
#else
#ifdef _LANGUAGE_PHILIPIN
	DWORD	ApiVersion			= 0xA5001150;		//XTrap ����
	DWORD	VendorCode			= 0x13026070;		//WiseLogic���� �ذ�
	DWORD	KeyboardType		= 0x00000000;
	DWORD	PatchType			= 0x00000010;		// Enable : 0x10, Disable : 0x00
	DWORD	ModuleType			= 0x01FFFFF2;
#else
#ifdef _LANGUAGE_VEITNAM
	DWORD	ApiVersion			= 0xA5001150;		//XTrap ����
	DWORD	VendorCode			= 0x13026080;		//WiseLogic���� �ذ�
	DWORD	KeyboardType		= 0x00000000;
	DWORD	PatchType			= 0x00000010;		// Enable : 0x10, Disable : 0x00
	DWORD	ModuleType			= 0x01FFFFF2;
#else
	//�ӽð�
	DWORD	ApiVersion			= 0xA5001150;		//XTrap ����
	DWORD	VendorCode			= 0x00000010;		//WiseLogic���� �ذ�
	DWORD	KeyboardType		= 0x00000000;
	DWORD	PatchType			= 0x00000010;		// Enable : 0x10, Disable : 0x00
	DWORD	ModuleType			= 0x0FFFFFFF;
#endif
#endif
#endif
#endif
#endif
#endif
	strcpy( sGameName, szGameTitle );

#ifdef __USE_XTRAP_MANAGER_LOG
	SetXTrapMgrInfo( szXTrap_MGR_ServerIP );		// Option
#endif

//	SetXTrapPatchHttpUrl( "Http://patch.wiselogic.co.kr:10080/xtrap" );		// Option

#ifdef _LANGUAGE_CHINESE //X-Trap
	SetXTrapPatchHttpUrl( "Http://222.73.230.8/xtrap" );					// �߱� Xtrap ��ġ����(�۵�)
#else
#ifdef _LANGUAGE_BRAZIL	//X-Trap
	SetXTrapPatchHttpUrl( "http://download.priston.com.br/xtrap" );			// ����� Xtrap ��ġ����(�۵�)
#else
#ifdef _LANGUAGE_ARGENTINA	//X-Trap
	SetXTrapPatchHttpUrl( "" );			// ����� Xtrap ��ġ����(�۵�)
#else
#ifdef _LANGUAGE_JAPANESE	//X-Trap
	SetXTrapPatchHttpUrl( "http://xtrap.pristontale.jp/xtrap" );			// �Ϻ� Xtrap ��ġ����(�۵�)
#else
#ifdef _LANGUAGE_PHILIPIN	//X-Trap
	SetXTrapPatchHttpUrl( "http://gupdate.netplay.ph/updates/X-trap" );		// �ʸ��� Xtrap ��ġ����
#else
#ifdef _LANGUAGE_VEITNAM	//X-Trap
	SetXTrapPatchHttpUrl( "http://xtrap.ptv.com.vn" );						// ��Ʈ�� Xtrap ��ġ����(�۵�)
#else
#ifdef _LANGUAGE_THAI		//X-Trap
	SetXTrapPatchHttpUrl( "http://patch.pt.in.th/xtrap" );					// �±� Xtrap ��ġ����
#endif
#endif
#endif
#endif
#endif
#endif
#endif

	SetXTrapStartInfo(  sGameName,
						ApiVersion,
						VendorCode,
						KeyboardType,
						PatchType,
						ModuleType );

	XTrapStart();
	XTrapKeepAlive();
#endif

//	xTrapConnect(GetXTrapMessagerIP());
//	SendLoginData();

	return TRUE;
}


//######################################################################################
//�� �� �� : 2006-08-11 ���� 6:18:57
//�� �� �� : xTrapConnect()
//�� �� �� : szServerIP
//�� �� �� : ���� ������ TRUE, ���н� FALSE
//��    �� : ������ �ʱ�ȭ �ϸ�, XTRap �޽��� ������ �����Ѵ�. (IDC�� �系�� �ִ� IP�� ������� �ȴ�.)
//�� �� �� : �� ö ȣ
//######################################################################################
BOOL CXTrapGameClient::xTrapConnect(const char *szServerIP)
{ 
	if(!XTrapWinsockStart())
		return false;

	if(!ConnectXTrapMessagerServer(szServerIP))
		return false;

	return true;
}

#endif