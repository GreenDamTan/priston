#include <windows.h>
#include <process.h>

#include "smlib3d\\smd3d.h"
#include "smwsock.h"
#include "smPacket.h"
#include "cracker.h"
#include "netplay.h"

char szFindCrackName[256];
static char szWindowName[71];
static char szClassName[31];
static char szAddString[101];

DWORD	dwExeCheckSum = 0;				//�������� üũ��
char	szProcessPath[256] = {0};		//�������� ���


//////////NPROTECT ����/////////////
#include "Language\\language.h"

//extern int DisconnectServerCode;
extern	int quit;

int	CrDebug = 1;
///////////////////////////////////

BOOL CALLBACK EnumWndProc( HWND hWnd, LPARAM lParam )
{

	char *temp_Windowname[7] = { "�����ϴ�", "Game Mas" , "GameHack" ,
							"        " , "��������" , "���ǵ���" };
	char *temp_Classname[4] = { "#3277" , "TWME1" , "#2770" };


	char GW_tempWindowname[9];
	char GW_tempClassname[6];

	int FindFlag = 0;

	
	GetWindowText( hWnd, szWindowName, 71 );
	GetClassName( hWnd, szClassName, 30 );
	
	ZeroMemory(GW_tempWindowname,sizeof(9));
	ZeroMemory(GW_tempClassname,sizeof(6));

	GetWindowText( hWnd, GW_tempWindowname, 9);
	GetClassName( hWnd, GW_tempClassname, 6);

	if ((strcmp(GW_tempWindowname,temp_Windowname[0])==0) && (strcmp(GW_tempClassname,temp_Classname[0])==0))
	{
		FindFlag++;
		//MessageBox(hWnd,"�������ڵ� �߰�","���",MB_OK);
	}

	if ((strcmp(GW_tempWindowname,temp_Windowname[2])==0) && (strcmp(GW_tempClassname,temp_Classname[0])==0))
	{
		FindFlag++;
		//MessageBox(hWnd,"������ �߰�","���",MB_OK);
	}

	if((strcmp(GW_tempWindowname,temp_Windowname[3])==0) && (strcmp(GW_tempClassname,temp_Classname[2])==0))
	{
		FindFlag++;
		//MessageBox(hWnd,"���ǵ���#1 �߰�","���",MB_OK);
	}

	if((strcmp(GW_tempWindowname,temp_Windowname[4])==0) && (strcmp(GW_tempClassname,temp_Classname[2])==0))
	{
		FindFlag++;
		//MessageBox(hWnd,"���ǵ���#2 �߰�","���",MB_OK);
	}

	if((strcmp(GW_tempWindowname,temp_Windowname[5])==0) && (strcmp(GW_tempClassname,temp_Classname[2])==0))
	{
		FindFlag++;
		//MessageBox(hWnd,"���ǵ���#3 �߰�","���",MB_OK);
	}

	if((strcmp(GW_tempWindowname,temp_Windowname[1])==0) && (strcmp(GW_tempClassname,temp_Classname[1])==0))
	{
		FindFlag++;
		//MessageBox(hWnd,"���Ӹ����� �߰�","���",MB_OK);
	}

	if ( FindFlag )
		lstrcpy( szFindCrackName , szWindowName );


    return( TRUE );
}

//ũ���� ã�� �̸� ��ȯ
char *FindCracker()
{
	szFindCrackName[0] = 0;
	EnumWindows( (WNDENUMPROC)EnumWndProc , 0 );

	if ( szFindCrackName[0] ) 
		return szFindCrackName;

	return NULL;
}

TRANS_FIND_CRACKER	TransFindCracker;


//ũ�� ��ġ ����ü�� ũ�� �˻�
BOOL CALLBACK EnumFindCrackWndProc( HWND hWnd, LPARAM lParam )
{

	int FindFlag;
	int cnt;
	
	char _szWindowName[71];
	char _szClassName[31];

	GetWindowText( hWnd, szWindowName, 71 );
	GetClassName( hWnd, szClassName, 30 );

	lstrcpy( _szWindowName , szWindowName );
	lstrcpy( _szClassName , szClassName );

	CharLower( _szWindowName );
	CharLower( _szClassName );


	for( cnt=0;cnt<TransFindCracker.FindCounter;cnt++) {

		FindFlag = 0;

		if ( TransFindCracker.CrackName[cnt].szName1[0] ) {
			FindFlag = 1;
			if ( strstr( _szWindowName , TransFindCracker.CrackName[cnt].szName1 )!=0 ) {
				if ( TransFindCracker.CrackName[cnt].szName2[0] ) {
					if ( strstr( _szWindowName , TransFindCracker.CrackName[cnt].szName2 )==0 ) {
						FindFlag = 0;
					}
				}
			}
			else {
				FindFlag = 0;
			}

			if ( TransFindCracker.CrackName[cnt].szClass[0] ) {
				if ( strstr( _szClassName , TransFindCracker.CrackName[cnt].szClass )==0 ) {
					FindFlag = 0;
				}
			}
		}

		if ( FindFlag ) {
			lstrcpy( szFindCrackName , szWindowName );
			break;
		}
	}	

    return( TRUE );
}

//ũ���� ã�� �̸� ��ȯ
char *FindCracker( TRANS_FIND_CRACKER *lpTransFindCrack )
{
	int cnt;

	memcpy( &TransFindCracker , lpTransFindCrack , lpTransFindCrack->size );

	for(cnt=0;cnt<TransFindCracker.FindCounter;cnt++) {
		CharLower( TransFindCracker.CrackName[cnt].szName1 );
		CharLower( TransFindCracker.CrackName[cnt].szName2 );
		CharLower( TransFindCracker.CrackName[cnt].szClass );
	}

	szFindCrackName[0] = 0;
	EnumWindows( (WNDENUMPROC)EnumFindCrackWndProc , 0 );

	if ( szFindCrackName[0] ) 
		return szFindCrackName;

	return NULL;
}

//ũ���� �ٽ� ã�´�
char *FindCrackCont()
{

	if ( TransFindCracker.code!=smTRANSCODE_FINDCRACK ) 
		return NULL;

	szFindCrackName[0] = 0;
	EnumWindows( (WNDENUMPROC)EnumFindCrackWndProc , 0 );

	if ( szFindCrackName[0] ) 
		return szFindCrackName;

	return NULL;
}

extern HWND	hwnd;

HWND hWnd_CrackFocus = 0;
DWORD	dwTime_CrackFocus = 0;

//������ ��Ŀ���� ����
HWND WinFocusCrack()
{
	HWND	hWnd;
	DWORD	dwTime;

	hWnd = GetForegroundWindow();
	if ( hWnd!=hwnd ) {
		//���� ��Ŀ�� �ܺο� ��ġ
		dwTime = GetCurrentTime();

		if ( !hWnd_CrackFocus ) {
			hWnd_CrackFocus = hWnd;
			dwTime_CrackFocus = dwTime;
		}
		else {
			if ( (dwTime_CrackFocus+5000)<dwTime ) {
				//�ܺ� ������ ��Ŀ�� 5�� �ʰ�
				return hWnd;
			}
		}
	}
	else {
		hWnd_CrackFocus = 0;
	}

	return NULL;
}

#include "tlhelp32.h"

#define	CR_NAME		-9

char CrackProcess[][16] = {
	{ 'a'+CR_NAME,'r'+CR_NAME,'t'+CR_NAME,'m'+CR_NAME,'o'+CR_NAME,'n'+CR_NAME,'e'+CR_NAME, 0 },
	{ 'p'+CR_NAME,'a'+CR_NAME,'t'+CR_NAME,'c'+CR_NAME,'h'+CR_NAME, 0 },
	{ 0,0 }
};

char MyProcess[16] = {
	'.'+CR_NAME,'e'+CR_NAME,'x'+CR_NAME,'e'+CR_NAME, 0 
};
//char szArtMoeny[16] = { 'a'+CR_NAME,'r'+CR_NAME,'t'+CR_NAME,'m'+CR_NAME,'o'+CR_NAME,'n'+CR_NAME,'e'+CR_NAME, 0 };
char MyProcessName[64];

int CheckProcess( char *szProcessName )
{
	char szBuff1[64];
	int nCnt = 0;
	int cnt,len;

	lstrcpy( szBuff1 , szProcessName );
	CharLower( szBuff1 );
	len = lstrlen( szBuff1 );
	for( cnt=0;cnt<len;cnt++ )
		szBuff1[cnt]+=CR_NAME;

	while(1) {
		if ( !CrackProcess[nCnt][0] ) break;
		if ( strstr( szBuff1 , CrackProcess[nCnt] )!=0 ) {
			return TRUE;
		}
		nCnt++;
	}

	return NULL;
}

int CheckMyProcess( char *szProcessName )
{
	int nCnt = 0;
	int cnt,len;

	lstrcpy( MyProcessName , MyProcess );

	len = lstrlen( MyProcessName );
	for( cnt=0;cnt<len;cnt++ )
		MyProcessName[cnt]-=CR_NAME;

	return TRUE;

}

int MyEnumModule98(int pid);


DWORD Check_CodeSafe( DWORD dwStartFunc );
DWORD Check_CodeSafe2();

// �������� ���μ����� ����� ���Ѵ�.
int CheckCrackProcess( int Flag )
{
//	HANDLE hSnap;
//	PROCESSENTRY32 pe;
	DWORD	ProcessID;
	int FindFlag = 0;
//	int MyNameFlag = 0;


	ProcessID = GetCurrentProcessId();
/*
	char szBuff[256];

		FILE *fp;
		fp=fopen( "myproc3.txt","wb" );
		if ( fp ) {
			wsprintf( szBuff, "%d" , ProcessID );
			fwrite( szBuff , lstrlen( szBuff ) , 1, fp );
			fclose(fp);
		}
*/

	CheckMyProcess( "game.exe" );
/*
	hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnap == (HANDLE)-1)
		return NULL;

	pe.dwSize=sizeof(PROCESSENTRY32);
	//SendMessage(hListPro,LB_RESETCONTENT,0,0);
	if (Process32First(hSnap,&pe)) {
		do {
		} while (Process32Next(hSnap,&pe));
	}
	CloseHandle(hSnap);

	//if ( MyNameFlag!=1 ) return 99;
*/
	if ( Flag )
		MyEnumModule98(ProcessID);

	return FindFlag;
}

// pid ���μ����� ��� ����� ���Ѵ�.
int MyEnumModule98(int pid)
{
	HANDLE hSnap;
	MODULEENTRY32 me;
	char str[256];

	str[0] = 0;

	hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pid);
	if (hSnap == (HANDLE)-1)
		return FALSE;

	me.dwSize=sizeof(MODULEENTRY32);
	//SendMessage(hListMod,LB_RESETCONTENT,0,0);
	if (Module32First(hSnap,&me)) {
		do {
			CharLower( me.szExePath );
			if ( strstr( me.szExePath , MyProcessName )!=0 ) {
				lstrcpy( str , me.szExePath );
			}
			//wsprintf(str,"%s",me.szExePath);
			//SendMessage(hListMod,LB_ADDSTRING,0,(LPARAM)str);
		} while (Module32Next(hSnap,&me));
	}
	CloseHandle(hSnap);

	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	DWORD	*szFileBuff;
	int		cnt;
	int		dCnt;
/*
		FILE *fp;
		fp=fopen( "myproc4.txt","wb" );
		if ( fp ) {
			fwrite( str , lstrlen( str ) , 1, fp );
			fclose(fp);
		}
*/
//	lstrcpy( str , "D:\\game����\\���º�Ÿ������\\game.exe" );
//	lstrcpy( str , "E:\\���̷����ɸ���������\\game.exe" );
	if ( str[0] ) {
		dwExeCheckSum = 0;
		hFile = CreateFile( str , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
		if ( hFile!=INVALID_HANDLE_VALUE ) {
			FileLength = GetFileSize( hFile , NULL );
			//dCnt = FileLength/4;
			szFileBuff = new DWORD[ (FileLength/4)+256 ];
			ReadFile( hFile , szFileBuff , FileLength , &dwAcess , NULL );
			CloseHandle( hFile );
/*
			dCnt = 1200*1024;
			dCnt /=4;
			if ( dCnt>0x46000 ) dCnt=0x46000;
*/
			dCnt = FileLength-0x8000;
			dCnt /= 4;

			for( cnt=0x1000;cnt<dCnt;cnt++ ) {
				dwExeCheckSum += szFileBuff[cnt]|(cnt*cnt*szFileBuff[cnt]);
				dwExeCheckSum += (szFileBuff[cnt]>>16)+(szFileBuff[cnt]<<16);
			}
			if ( !dwExeCheckSum ) dwExeCheckSum = 1;
			delete szFileBuff;
		}
	}
	lstrcpy( szProcessPath , str );

	return TRUE;
}

DWORD	GetModuleCheckSum( char *szModule )
{
	DWORD	ProcessID;


	HANDLE hSnap;
	MODULEENTRY32 me;
	int cnt,len;
	DWORD ChkSum =0;
	DWORD *lpBuff;
/*
	DWORD	dwHeaderPos = 0x30;
	DWORD	*lpdwFunc;
	BYTE	*lpFunc;
	DWORD	PointMin,PointMax;
	DWORD	dwStartAddr;
*/

	ProcessID = GetCurrentProcessId();

	hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,ProcessID);
	if (hSnap == (HANDLE)-1)
		return FALSE;

	me.dwSize=sizeof(MODULEENTRY32);
	if (Module32First(hSnap,&me)) {
		do {
			if ( lstrcmpi( me.szModule , szModule )==0 ) {

				lpBuff = (DWORD *)me.modBaseAddr;
/*
				dwStartAddr = (DWORD)lpBuff;
				lpFunc = (BYTE *)lpBuff;
				lpdwFunc = (DWORD *)(dwStartAddr+dwHeaderPos+0xC);
				lpdwFunc = (DWORD *)(lpdwFunc[0]+dwStartAddr);

				PointMax = lpdwFunc[0x4B]+dwStartAddr;
				PointMin = dwStartAddr+0x1000;
*/
				len = me.modBaseSize/sizeof(DWORD);
				for( cnt=0;cnt<len;cnt++) {
					ChkSum+=lpBuff[cnt]*(cnt&0xFFFF);
				}
			}
		} while (Module32Next(hSnap,&me));
	}
	CloseHandle(hSnap);


	//GetSystemDirectory(

	return ChkSum;
}


int	CheckDLL_Module()
{
	DWORD	ProcessID;


	HANDLE hSnap;
	MODULEENTRY32 me;
	int len,cnt;
	DWORD ChkSum =0;
	//DWORD *lpBuff;
	int		FindFlag[2] = {0,0};

	char	szBuff[256];
	char	*lpBuff;

	TRANS_CHATMESSAGE	TransChatMessage;

	if ( GetSystemDirectory( szBuff , 128 )==NULL ) return FALSE;

	len = lstrlen( szBuff );

	szBuff[len++] = '\\';

	lpBuff = szBuff+len;

	szBuff[len++] = 'w';
	szBuff[len++] = 's';
	szBuff[len++] = 'o';
	szBuff[len++] = 'c';
	szBuff[len++] = 'k';
	szBuff[len++] = '3';
	szBuff[len++] = '2';
	szBuff[len++] = '.';
	szBuff[len++] = 'd';
	szBuff[len++] = 'l';
	szBuff[len++] = 'l';
	szBuff[len] = 0;


	ProcessID = GetCurrentProcessId();

	hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,ProcessID);
	if (hSnap == (HANDLE)-1)
		return FALSE;

	me.dwSize=sizeof(MODULEENTRY32);
	if (Module32First(hSnap,&me)) {
		do {
			if ( lstrcmpi( me.szExePath , szBuff )==0 ) {
				FindFlag [0]++;
			}
			else {
				if ( lstrcmpi( lpBuff , me.szModule )==0 ) {
					FindFlag [1]++;
					lstrcpy( TransChatMessage.szMessage , me.szExePath );
					len = lstrlen( TransChatMessage.szMessage );
					TransChatMessage.dwIP = len;
					for(cnt=0;cnt<len;cnt++)
						TransChatMessage.szMessage[cnt] ^= (cnt*0x15);

				}
			}

		} while (Module32Next(hSnap,&me));
	}
	CloseHandle(hSnap);

	if ( FindFlag[0]==1 && FindFlag[1]==0 ) return TRUE;

	if ( FindFlag[1]>0 && smWsockDataServer ) {
		TransChatMessage.code = smTRANSCODE_MODULE_WARNIG;
		TransChatMessage.size = TransChatMessage.dwIP+32;
		TransChatMessage.dwObjectSerial = 0;
		smWsockDataServer->Send2( (char *)&TransChatMessage , TransChatMessage.size , TRUE ); 
	}

	return NULL;
}



//�ؿ�
#ifdef _LANGUAGE_CHINESE
#ifndef _W_SERVER

//#include "nSprite\\nsprite.h"

//#pragma comment(lib,"nSprite\\nsprite.lib")
DWORD	dwNSpriteTime = 0;
extern DWORD dwPlayTime;
extern smWINSOCK	*smWsockDataServer;				//����Ÿ ���� ����

//#define	_nSPRITE

#endif
#endif


//���� ����
#ifdef	_npGAME_GUARD
#ifdef	_LANGUAGE_THAI
#pragma comment(lib,"nprotect\\OLD\\NPGameLib.lib")	//�±�����(�ؿ�)
#else
#pragma comment(lib,"nprotect\\NPGameLib.lib")
#endif

#ifdef	_LANGUAGE_THAI
#include "nprotect\\OLD\\NPGameLib.h"
#else
#include "nprotect\\NPGameLib.h"
#endif

#ifdef _LANGUAGE_KOREAN
#ifdef _TEST_SERVER
CNPGameLib npgl("PristonTaleTest");		//�׽�Ʈ����
#else
CNPGameLib npgl("PristonTale");			//��������
#endif
#endif

#ifdef _LANGUAGE_THAI
CNPGameLib npgl("PristonTaleTA");		//�±�
#endif

/*
#ifdef _LANGUAGE_ENGLISH
#ifndef _LANGUAGE_THAI
CNPGameLib npgl("PristonTaleEng");		//����
#endif
#endif
*/

#ifdef _LANGUAGE_JAPANESE
CNPGameLib npgl("PristonTaleMoriah");		//���Ӱ��� 2 ����
#endif

#ifdef _LANGUAGE_ENGLISH
#ifdef _LANGUAGE_PHILIPIN
CNPGameLib npgl("PristonTalePH");		//�ʸ���
#else
CNPGameLib npgl("PristonTaleEng");		//����
#endif
#endif

//�߱�
#ifdef	_LANGUAGE_CHINESE
CNPGameLib	npgl("PristonTaleEng");		//�ӽ� ������ �����ϰ� ����
#endif

#ifdef _LANGUAGE_BRAZIL	
CNPGameLib npgl("PristonTaleEng");		//�����
#endif
#ifdef _LANGUAGE_ARGENTINA
CNPGameLib npgl("PristonTaleEng");		//�Ƹ���Ƽ��(�ӽ� ����� ���ϼ���)
#endif
#ifdef _LANGUAGE_VEITNAM    //�ӽ�
CNPGameLib npgl("PristonTaleEng");		//��Ʈ��
#endif


int SetQuitURL_GameGuard( DWORD dwMsg );

#else

//nProtect
#include "..\\nProtect\nprotect.h"
nP	nProtect;

#endif


//nProtect ����
int Start_nProtect( int deMode )
{
//	return TRUE;
//	deMode = 0;

#ifdef _LANGUAGE_KOREAN

	CrDebug = deMode;

#endif
#ifdef _LANGUAGE_ENGLISH

	CrDebug = deMode;
	//CrDebug = 0;

#endif
//�߱�
#ifdef _LANGUAGE_CHINESE

	CrDebug = deMode;
	//CrDebug = 0;

#endif
//�����
#ifdef _LANGUAGE_BRAZIL	

	CrDebug = deMode;
	//CrDebug = 0;
#endif
//�Ƹ���Ƽ��
#ifdef _LANGUAGE_ARGENTINA

	CrDebug = deMode;
	//CrDebug = 0;
#endif
//�±�
#ifdef _LANGUAGE_THAI

	CrDebug = deMode;
	//CrDebug = 0;

#endif
//��Ʈ��
#ifdef _LANGUAGE_VEITNAM
	CrDebug = deMode;
#endif	
//�Ϻ�
#ifdef _LANGUAGE_JAPANESE

	CrDebug = deMode;
	//CrDebug = 0;

#endif

#ifdef _LANGUAGE_TAIWAN

#ifndef _LANG_HONGKONG
	//ȫ����������� nProtect ��� ����
	CrDebug = deMode;
#endif

#endif




#ifdef _nSPRITE
	InitNSprite();			//nSprite �ʱ�ȭ
#endif

#ifdef	_npGAME_GUARD

	//CrDebug = 0;			//nProtect ��� ����
#ifndef _WINMODE_DEBUG
	CrDebug = 0;
#endif

	int result;

	//CrDebug = 0;			//�׽�Ʈ

	if ( !CrDebug ) {
/*
#ifdef _TEST_SERVER
		result = npgl.Init( hwnd , "PristonTaleTest" );
#else
		result = npgl.Init( hwnd , "PristonTale" );
#endif
*/
#ifdef _LANGUAGE_THAI//�ؿ�
		result = npgl.Init(hwnd);	//���Ӱ��� 2		
#else
		result = npgl.Init();	//���Ӱ��� 2		
#endif

		if ( result!=NPGAMEMON_SUCCESS ) {
			SetQuitURL_GameGuard( result );
			return FALSE;
		}
	}

#else

	if ( !CrDebug ) {

//		nProtect.Init();
// ���� ������ ���� nProtect ��
//#ifndef _nProtect_For_Debug
//		if ( nProtect.NProtectRun()!=1024 ) 
//			return FALSE;
//#endif

	}
#endif

	return TRUE;
}


//nProtect HWND
int SetHwnd_nProtect( HWND hWnd )
{
#ifdef	_npGAME_GUARD//(�ؿ� �±��� �� �׷���?)
#ifndef _LANGUAGE_THAI
	npgl.SetHwnd( hWnd );
#endif
#endif
	return TRUE;
}

//nProtect Ȯ��
int Check_nProtectDLL()
{

	if ( !CrDebug ) {
#ifndef	_npGAME_GUARD
		if ( nProtect.NProtectCheck()!=0 ) 
			return FALSE;
#endif
	}

	return TRUE;
}



//nProtect ����
int Remove_nProtect()
{

	if ( !CrDebug && DisconnectServerCode!=4 ) {
	

#ifndef	_npGAME_GUARD
		nProtect.NProtectFree();
		SendMessage(FindWindow(NULL,"WWW.NPROTECT.COM"), WM_NPROTECT_FORCEEXIT, NULL, NULL);
#endif
		CrDebug = TRUE;

	}

	return TRUE;
}

//nProtect Ȯ��
int	Check_nProtect()
{

	if ( !CrDebug ) {
#ifndef	_npGAME_GUARD
		if ( nProtect.findNPMON()!=0 )  {
			if ( nProtect.findNPMON()!=0 )  {
				if ( nProtect.findNPMON()!=0 )  {
					return FALSE;
				}
			}
		}
#endif
	}

#ifdef _nSPRITE
	smTRANS_COMMAND_DWORD	smTransCommand;

	if ( (dwNSpriteTime+40*1000)<dwPlayTime ) {
		smTransCommand.code = smTRANSCODE_NSPRITE;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = GetNSpriteVersion();
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;
		dwNSpriteTime = dwPlayTime;

		if ( smWsockDataServer ) 
			smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}
#endif

#ifdef _npGAME_GUARD

//#ifndef _WINMODE_DEBUG
	CrDebug = 0;
//#endif

	int result;
	if ( !CrDebug ) {
		result = npgl.Check();
		if ( result!=NPGAMEMON_SUCCESS ) {
			SetQuitURL_GameGuard( result );
			return FALSE;
		}
	}
#endif

	return TRUE;
}

//����� ID ���� ���Ӱ��� �뺸
int	npSetUserID( char *szID )
{
#ifdef _npGAME_GUARD
	SendUserIDToGameMon( szID );
#endif
	return TRUE;
}

int npAuth( DWORD dwAuthValue )
{
#ifdef _npGAME_GUARD
	npgl.Auth( dwAuthValue );
#endif
	return TRUE;

}

//#define	_npGAME_GUARD

#ifdef	_npGAME_GUARD

extern char szExitInfo_URL[128];
extern char UserAccount[256];		//id�� ����ִ� ����

int SetQuitURL_GameGuard( DWORD dwMsg )
{
	int	LangMode = 0;
	char szID[256];

#ifdef _LANGUAGE_KOREAN
	LangMode = 100;
#endif
#ifdef _LANGUAGE_CHINESE
//	LangMode = 101;
	LangMode = 104;//���� �ӽ�����
#endif
#ifdef _LANGUAGE_JAPANESE
	LangMode = 102;
#endif
#ifdef _LANGUAGE_TAIWAN
	LangMode = 103;
#endif
#ifdef _LANGUAGE_ENGLISH
	LangMode = 104;
#endif
#ifdef _LANGUAGE_BRAZIL
	LangMode = 104;
#endif
#ifdef _LANGUAGE_ARGENTINA//(�ӽ� ����� ���ϼ���)
	LangMode = 104;
#endif
#ifdef _LANGUAGE_THAI
	LangMode = 105;
#endif	
#ifdef _LANGUAGE_VEITNAM
	LangMode = 104;
#endif	

	if ( UserAccount[0] ) 
		lstrcpy( szID , UserAccount );
	else
		lstrcpy( szID , "npuser" );

	wsprintf( szExitInfo_URL , "http://info.pristontale.com/nP_Msg/advice.asp?id=%s&msg=%d&lang=%d",
		szID , dwMsg , LangMode );


	return TRUE;
}


// ���� ����ÿ��� false�� return ���ְ�, �������� �ʴ� ���� true�� return �մϴ�.
bool CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	//TCHAR msg[128];
	smTRANS_COMMAND_DWORD	smTransCommand;

	switch (dwMsg) {

	case NPGAMEMON_COMM_ERROR:
	case NPGAMEMON_COMM_CLOSE:
		DisconnectServerCode = 4;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 4");
		DisconnectFlag = GetCurrentTime();
		quit = 1;
		return false; // �ݵ�� ����

	case NPGAMEMON_INIT_ERROR:
		//wsprintf(msg, "nProtect GameGuard �ʱ�ȭ ���� : %lu", dwArg);
		//MessageBox(hWnd, msg, ��Error��, MB_OK);
		DisconnectServerCode = 4;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 5");
		DisconnectFlag = GetCurrentTime();
		SetQuitURL_GameGuard( dwMsg );		//URL �޼��� ǥ��
		quit = 1;
		return false; // �ݵ�� ����

	case NPGAMEMON_SPEEDHACK:
		//MessageBox(hWnd, �����ǵ����� �����Ǿ����ϴ�.��, ��Warning��, MB_OK);
		DisconnectServerCode = 4;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 1");
		DisconnectFlag = GetCurrentTime();
		SetQuitURL_GameGuard( dwMsg );		//URL �޼��� ǥ��
		quit = 1;
 		return false; // ����

	case NPGAMEMON_GAMEHACK_KILLED:
		//MessageBox(hWnd, ���������� �߰ߵǾ����ϴ�.��, ��Warning��, MB_OK);
 		return true; // ��� ����

	case NPGAMEMON_GAMEHACK_DETECT:
		//MessageBox(hWnd, ���������� �����Ǿ����ϴ�.��, ��Warning��, MB_OK);
		DisconnectServerCode = 4;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 2");
		DisconnectFlag = GetCurrentTime();
		SetQuitURL_GameGuard( dwMsg );		//URL �޼��� ǥ��
		quit = 1;
 		return false; // ����

   case NPGAMEMON_GAMEHACK_DOUBT:
	   //lpszMsg = �����Ӱ��尡 �����Ǿ����ϴ�.��;
		DisconnectServerCode = 4;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 3");
		DisconnectFlag = GetCurrentTime();
		SetQuitURL_GameGuard( dwMsg );		//URL �޼��� ǥ��
		quit = 1;
 		return false; // ����

	case NPGAMEMON_CHECK_CSAUTH:
		//���Ӱ��� ���� Ȯ���ڵ� ������
		if ( smWsockDataServer ) {
			smTransCommand.size = sizeof( smTRANS_COMMAND_DWORD );
			smTransCommand.code = smTRANSCODE_GAMEGUARD_AUTH;
			smTransCommand.WParam = dwArg;
			smTransCommand.LParam = 0;
			smTransCommand.SParam = 0;
			smTransCommand.EParam = 0;
			smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}
		break;

	}

   return true;
}



#endif