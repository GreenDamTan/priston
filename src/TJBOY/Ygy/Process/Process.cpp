#define PROCESS_CPP
#include <windows.h>
#include <stdio.h>
#include "..\\YgyHeader.h"
//ktj : vrun.dat luncher.ini



extern void fd( char *fmt, ...);
extern void fd2( char *fmt, ...);

extern HWND hwnd; //�������� �ڵ� 



#if 0//ktj

//���̺귯���� ����ϸ� dll�Լ����� �Ϲ��Լ�ó�� ��밡���ؼ� ���ϰ� �����ִ�.
//lib�� ��δ� �׻� ������Ʈ�� ��Ʈ�������� ����� ã���� �����Ƿ� ����
#pragma comment(lib, "tjboy/HookDll.lib") 
#include "../../khook.h"
UINT  g_uMsg = 0;

//��ŷdll���� ������ �޽�����ȣ
#define WM_KEYBOARDHOOK_ACTIVITY (WM_USER + 11)
//////////////////////////////////////////////////////
//
// dll�� ��ŸƮ �Լ�
//
//////////////////////////////////////////////////////
void dll_open()
{
	g_uMsg = WM_KEYBOARDHOOK_ACTIVITY;
	
	if (InstallKeyboardHook (hwnd, g_uMsg))
		fd2("Hook installed OK\n");
	else
		fd2("Hook install FAILED\n");
} 

//////////////////////////////////////////////////////
//
// dll�� end �Լ�(*.lib�Ⱦ��� ��ߵǴ°ǵ� �ϴܳ��ܵ�)
//
//////////////////////////////////////////////////////
void dll_close()
{
#if 0
	if(Hook != NULL) {
		UnhookWindowsHookEx(Hook);
		//DLL�� �޸𸮿��� ����.
		FreeLibrary(hLib);
		Hook = NULL;
	}
#endif

}

#endif

extern char UserAccount[256];		//id�� ����ִ� ����
extern char	szConnServerName[16];	//������ �����̸��� ����ִ� ����

int vrunRuning = 0;					//vrun.dat�� ���࿩��

int gameServerPORT=0;
char gameServerIP[16]={0,};
BOOL bMic = 0;

char g_AllClanWon[_CLANWON_MAX][STRING_MAX];

CProcess::CProcess()
{
	m_cMemMap			 = NULL;	
	m_hRemoteProcess	 = NULL;
	m_hRemoteWnd		 = NULL;	
	//m_nKeyDown			 = -1;
	m_bIsMemMap			 = FALSE;
	m_IsStart			 = FALSE;

}

CProcess::~CProcess()
{
}



extern HWND hwnd; //�������� �ڵ� 
int CProcess::InitRemoteProcess()
{
	fd2("InitRemoteProcess()00");
	m_hRemoteWnd = FindWindow(REMOTEPROCESS_NAME, NULL);
	fd2("InitRemoteProcess()11 m_hRemoteWnd  %d", m_hRemoteWnd );

	if(m_hRemoteWnd)  //�������� Ŭ���̾�Ʈ�� ������.
	{
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,0, (LPARAM)hwnd);
		return 1;
	}
	return 0;
	






	/*
	int static chk = 0;

	if(chk ==1)
	{


	fd2("InitRemoteProcess()00");
	m_hRemoteWnd = FindWindow(REMOTEPROCESS_NAME, NULL);
	fd2("InitRemoteProcess()11 m_hRemoteWnd  %d", m_hRemoteWnd );

	if(m_hRemoteWnd)  //�������� Ŭ���̾�Ʈ�� ������.
	{
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,0, (LPARAM)hwnd);
		return 1;
	}
	return 0;
	
	}







chk = 1;


	fd2("InitRemoteProcess() 00");

	PROCESS_INFORMATION pi={0,};
	STARTUPINFO si={0,};

	//if(CreateProcess(NULL,REMOTEPROCESS_EXE,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)  == 0)
	char buf[256];
	//ktj : ����
	//wsprintf(buf, "%s %d", REMOTEPROCESS_EXE, hwnd);
	wsprintf(buf, "%s %d", "vrun.exe", hwnd);
	

	if(CreateProcess(NULL,buf,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)  == 0)
	{
		fd2("InitRemoteProcess() 11 error");

		return -1;
	}


	m_hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pi.dwProcessId);


	fd2("InitRemoteProcess() 22 ");


	//�����Ų �ڵ鰪�� �˾Ƴ���.
	DWORD dwExitCode;
	int nCount = 0;

	if(m_hRemoteProcess == NULL){
		fd2("InitRemoteProcess() 33 error");
		return -1;
	}
	GetExitCodeProcess(m_hRemoteProcess,&dwExitCode);
	if(dwExitCode == STILL_ACTIVE)
	{
getloop:	
		Sleep(100);

		m_hRemoteWnd = FindWindow(REMOTEPROCESS_NAME, NULL);
		if(m_hRemoteWnd== NULL)
		{
			DWORD dwError;
			dwError = GetLastError();
			dwError = dwError;
			if(nCount > 100)
			{
				fd2("InitRemoteProcess() 44 error ");
				return -4;
			}
			nCount++;
			   goto getloop;
		}
		else
		{
			fd2("InitRemoteProcess() 55 OK ");
			return 1;
		}
	}

	fd2("InitRemoteProcess() 66 error ");
	return -5;
	*/

}



int CProcess::InitRmoteProcessWnd()
{
	m_nStep	 = INSIDE_GAMELOGIN;
	return 1;
}


/*===========================================================================================
����� �޸� �ʰ� �������μ��� �ڵ��� ������ �������ؼ� �ʱ�ȭ �ϴ� ����
============================================================================================*/
BOOL CProcess::Init()
{	
	
	if(m_IsStart)
	{
		

		
		m_cMemMap = (CMemMap*) new CMemMap;
		if(m_cMemMap == NULL)
		{
			return FALSE;
			m_bIsMemMap = FALSE;
		}

		m_cMemMap->SetMemMapSize(MEMORYMAP_MAX);  //MEMORYMAP_MAX  256
		m_cMemMap->SetName(MEMORYMAP_NAME);
		m_cMemMap->InitMemoryMap();
		m_bIsMemMap = TRUE;

		
		


		//���� ���μ��� �ڵ��� ������ ��
		InitRmoteProcessWnd();
	}
	

	return TRUE;
}


/*===========================================================================================
����� �޸� ���� �����ϴ� �Լ�
============================================================================================*/
VOID CProcess::Clear()
{
	if(!m_IsStart) return;

	UserOut();	//������ ��������..


	if(m_hRemoteProcess != NULL)
		TerminateProcess(m_hRemoteProcess,0);

	m_Packet.Clear();
	m_cMemMap->Clear();
	if((m_cMemMap != NULL) && m_bIsMemMap)
	{
		delete m_cMemMap;
	}
}

/*===========================================================================================
1. �޸� �ʿ��� ����Ÿ�� ������ �� m_cMemMap->ReadMemoryMap
2. �޸� �ʿ� �ִ� ����Ÿ�� �Űܿ� m_Packet.GetData()
3. ������ �� ����Ÿ�� �Ľ�			m_Packet.Parsing()
============================================================================================*/
void PacketParsing()	//winmain.cpp
{
	g_CtrlProcess.PacketParsing();
}
VOID CProcess::PacketParsing()
{
	m_cMemMap->ReadMemoryMap((char*)m_Packet.GetData());
	m_Packet.Parsing();
}

VOID CProcess::Main()
{
	if(!m_IsStart) return;

	switch(m_nStep)
	{
	case INSIDE_GAMELOGIN:

#if 0
		m_Packet.SetLogin(UserAccount);
		m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetGameLogin());
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,(WPARAM)0,0);
#endif

		//ktj : ip, port������.
		IPPORT ipport;
		ZeroMemory(&ipport, sizeof(ipport));

		ipport.sIndex.dwIndex = INSIDE_IPPORT;
		strcpy(ipport.ip,gameServerIP);
		ipport.port = gameServerPORT;
		//ipport.Chwnd;


		m_cMemMap->WriteMemoryMap((char*)&ipport, sizeof(ipport));
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,0,0);

		m_nStep = -1;
		break;

	case INSIDE_USERINFO:
		m_userInfo.sIndex.dwIndex = INSIDE_USERINFO;
		m_cMemMap->WriteMemoryMap((char*)&m_userInfo, sizeof(m_userInfo));
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,0,0);
		m_nStep = -1;
		break;

	case INSIDE_USERINFOCHG:
		m_userInfo.sIndex.dwIndex = INSIDE_USERINFOCHG;
		m_cMemMap->WriteMemoryMap((char*)&m_userInfo, sizeof(m_userInfo));
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,0,0);
		m_nStep = -1;
		break;




	case INSIDE_MICONOFF:
		VOICEONOFF vm;
		ZeroMemory(&vm, sizeof(vm));

		vm.sIndex.dwIndex = INSIDE_MICONOFF;
		
		//bMic = !bMic;
		vm.bMicONOFF = bMic;

		m_cMemMap->WriteMemoryMap((char*)&vm, sizeof(vm));
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,0,0);
		m_nStep = -1;
		break;













/*
	//ktj : ������.
	case INSIDE_SELECTCHACLAN:
		m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetSelectChaClan());
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,(WPARAM)20000,0);
		m_nStep = -1;
		 break;	
	case INSIDE_GAME_OUT:
		m_Packet.SetUserOut("ycy916","��Ʈ������",30043);
		m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetUserOut());
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,(WPARAM)20000,0);
		m_nStep = -1;
		break;

	case INSIDE_WAVRECORD:
		m_Packet.SetWavRecord();
		m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetWavRecord());
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,(WPARAM)20000,0);
		m_nStep = -1;		
		break;
	case INSIDE_WAVEPLAY:
		m_Packet.SetWavPlay();
		m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetWavPlay());
		SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,(WPARAM)20000,0);
		m_nStep = -1;
		break;
*/

	}
}




// ��öȣ (����ũ �¿���)
VOID	ParkSetMicOnOFF();

VOID	ParkSetMicOnOFF()
{
	g_CtrlProcess.SetMicOnOFF();
}
VOID	CProcess::SetMicOnOFF()
{
	if(!m_IsStart) return;

	m_nStep = INSIDE_MICONOFF;
}








VOID CProcess::TestInputData(HWND hWnd,WPARAM wParam,LPARAM lParam,char *temp)
{
/*

	switch(wParam)
	{
	case VK_RETURN:
		switch(m_nKeyDown)
		{
		case -1:		
			strcpy(temp,"���ӽ���................");
			InvalidateRect(hWnd,NULL,1);
			m_nKeyDown++;
			//m_nStep = INSIDE_GAMELOGIN;
			break;
		case 0:
			strcpy(temp,"���ӷα��� ȭ��........");
			InvalidateRect(hWnd,NULL,1);
			m_nStep = INSIDE_GAMELOGIN;
			m_nKeyDown++;
			break;
		case 1:	
			strcpy(temp,"�ɸ��� ����........");
			InvalidateRect(hWnd,NULL,1);
			m_nStep = INSIDE_USERINFO;
			m_nKeyDown++;
			break;	
		case 2:
			strcpy(temp,"Ŭ���� ���Ե� ĳ���� ����");
			InvalidateRect(hWnd,NULL,1);
			m_nStep = INSIDE_SELECTCHACLAN;
			m_nKeyDown++;
			break;
		}
		break;
	case VK_ESCAPE:		
		strcpy(temp,"��������");
		InvalidateRect(hWnd,NULL,1);
		m_nStep = INSIDE_GAME_OUT;
		m_nKeyDown = -1;
		break;
	case VK_F1:
		m_Packet.SetIDandChaName("ycy916","��Ʈ����");
		m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetIDandName());
		SendMessage(hWnd,WM_CALLMEMMAP,(WPARAM)20000,0);
		break;
	case VK_F2:
		m_Packet.SetClanInfo(3020203,"�׽�ƮŬ��");
		m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetClanInfo());
		SendMessage(hWnd,WM_CALLMEMMAP,(WPARAM)20000,0);
		break;
	case VK_F3:
		m_Packet.SetUserOut("yoo","��Ʈ������",30043);
		m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetUserOut());
		SendMessage(hWnd,WM_CALLMEMMAP,(WPARAM)20000,0);
		break;
	}
*/
}

VOID CProcess::SetStep(int step)
{
	m_nStep = step;
}





//===========================================================================//
#if 0
VOID CProcess::SetSelectCha(char *id,char *chaname, char *clanname,DWORD clannum)
{
	if(!m_IsStart) return;
	
	//ktj : ���⼭ ����Ÿ�� �������� ������ �ɰ�쿡�� ������ ó���� �ؾ��ҵ�.
	ZeroMemory(&m_userInfo,sizeof(m_userInfo));
	m_userInfo.sIndex.dwIndex = INSIDE_USERINFO;
	if(id != NULL)
		strncpy(m_userInfo.szID,id,strlen(id));
	if(chaname != NULL)
		strncpy(m_userInfo.szCharName, chaname,strlen(chaname));
	if(clanname !=NULL)
		strncpy(m_userInfo.szClanName,clanname,strlen(clanname));
	m_userInfo.dwClanNum = clannum;


	//������쿡�� �̷���
	m_nStep = INSIDE_USERINFO;
}
#endif

int firstFlag = 0;
VOID CProcess::SetSelectCha(char *id,char *chaname, char *clanname,DWORD clannum, int utype)
{
	if(!m_IsStart) return;

	//����Ÿ�� �� ó�� ���õɶ� =========================
	if(firstFlag==0) {
		firstFlag = 1;	
	
		//ktj : ���⼭ ����Ÿ�� �������� ������ �ɰ�쿡�� ������ ó���� �ؾ��ҵ�.
		ZeroMemory(&m_userInfo,sizeof(m_userInfo));
		m_userInfo.sIndex.dwIndex = INSIDE_USERINFO;
		//if(id != NULL)
		//	strncpy(m_userInfo.szID,id,strlen(id));

		extern char UserAccount[256];		//id�� ����ִ� ����
		strncpy(m_userInfo.szID,UserAccount,strlen(UserAccount));


		if(chaname != NULL)
			strncpy(m_userInfo.szCharName, chaname,strlen(chaname));
		if(clanname !=NULL)
			strncpy(m_userInfo.szClanName,clanname,strlen(clanname));
		m_userInfo.dwClanNum = clannum;
		m_userInfo.utype = utype;

		//�����̸�����

		

		if(strlen(szConnServerName)<15) {
			strncpy(m_userInfo.szConnServerName,szConnServerName,strlen(szConnServerName));
		
		
			fd2("��������Ŭ��: �����̸�  %s ",szConnServerName);
		}
		else {
			//©��ߵɵ�
			szConnServerName[15] = 0;
			fd2("��������Ŭ��: �����̸� �̻�????????????????????? %s ",szConnServerName);
		}
		//������쿡�� �̷���
		m_nStep = INSIDE_USERINFO;
		return;
	}


	//������ ������ �ִ� Ŭ�� ��ȣ�� ��ݻ��� ���� Ŭ����ȣ�� ������
	//���ο� ����Ÿ�� ���� �ʿ䰡 ����.
	if(m_userInfo.dwClanNum == clannum) {
		return;
	}



	//ktj : ���⼭ ����Ÿ�� �������� ������ �ɰ�쿡�� ������ ó���� �ؾ��ҵ�.
	m_userInfo.sIndex.dwIndex = INSIDE_USERINFOCHG;
	if(clanname !=NULL)
		strncpy(m_userInfo.szClanName,clanname,strlen(clanname));
	else {
		memset(&m_userInfo.szClanName, 0, sizeof(m_userInfo.szClanName));
	}
	m_userInfo.dwClanNum = clannum;
	m_userInfo.utype = utype;

	//Ŭ���̾������ų� �ٲ��� db�� �ٲٱ�����.
	m_nStep = INSIDE_USERINFOCHG;	
}



VOID CProcess::UserOut()
{
	if(!m_IsStart) return;
	/*
	m_Packet.SetUserOut(NULL,NULL,0);
	m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetUserOut());
	SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,(WPARAM)20000,0);
	*/
	P_Out pout;
	pout.sIndex.dwIndex = INSIDE_GAME_OUT;

	m_cMemMap->WriteMemoryMap((char*)&pout, sizeof(pout));
	SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,(WPARAM)0,0);
	Sleep(1000*2);	//���μ����������� �ð��� �ֱ�����.
}

//"mapedit.INI" ���Ͼ��� "INITGAME"�� ���� �ɼ��� �д´�.
int read_GameOption(char *optStr, char *rStr)
{
    char fname[] = "luncher.INI";
    char folder[] = ".\\";
    char strFilename[128];
    strcpy ( strFilename , folder);
    strcat ( strFilename , fname );

    if(rStr==NULL) { //int��
        return GetPrivateProfileInt("INITGAME", optStr, 0, strFilename );
    }
    //string��
    GetPrivateProfileString("INITGAME", optStr, NULL, rStr, 128, strFilename);
    return 0;
}


int InitGAMEopt()
{
    FILE *fp;
    fp = fopen("luncher.ini", "rb");
    if(fp==NULL) {
        return 0;
    }
    else {                      //���������� ýũ(mapedit.ini����)
        fclose(fp);             //�ϴ� �ݰ� ���� ����Ÿ�� mapedit.ini���� ������.

		//���� ���� ip,port
		read_GameOption("gameServerIP", gameServerIP );
		gameServerPORT = read_GameOption("gameServerPORT", NULL );
    }
	return TRUE;
}

/*
void abc()
{
g_CtrlProcess.CheckIPandPort(0, 0); 
}
*/


VOID CProcess::CheckIPandPort(char *ip, int port)
{
	
	//ktj : auth.cpp : Ŭ�������� ���ٸ� �۵����Ѵ�.
	extern BOOL bip_port_error;
	if(bip_port_error==TRUE) {
		m_IsStart = FALSE;
		return;

	}
	


	fd2("CheckIPandPort() 00 ");


	if(InitGAMEopt() == 0)	//luncher.ini�� ������ ��Ŭ���̾�Ʈ�κ��� ip,port�� �޾ƿ´�.
	{
		if((ip != NULL ) &&(port != 0))
		{
			fd2("CheckIPandPort() 11 ");

			gameServerPORT = port;
			strncpy(gameServerIP,ip,strlen(ip));

			if(InitRemoteProcess() == 1) {
				vrunRuning = 1;					//vrun.dat�� ���࿩��
				m_IsStart = TRUE;
				fd2("CheckIPandPort() 22 OK ");
			}
		}
		else	//ini������ ���� ���������� ip,port������ ������ �׳��̰ɷ� �Ѵ�.
		{


			fd2("CheckIPandPort() 33 ");
			//m_IsStart = FALSE;
			gameServerPORT = 7000;
			strcpy(gameServerIP,"211.44.231.13");

			if(InitRemoteProcess() == 1) {
				vrunRuning = 1;					//vrun.dat�� ���࿩��
				m_IsStart = TRUE;

				fd2("CheckIPandPort() 44 OK ");
			}

		}
	}
	else //luncher.ini�� ������ �ű⼭ �޾ƿ´�.
	{
		if(gameServerPORT != 0 && gameServerIP[0] != 0)
		{

			fd2("CheckIPandPort() 55 ");
			if(InitRemoteProcess() == 1) {
				vrunRuning = 1;					//vrun.dat�� ���࿩��
				m_IsStart = TRUE;
				fd2("CheckIPandPort() 66 ok ");
			}
		}
		else
		{
			m_IsStart = FALSE;

			fd2("CheckIPandPort() 77 false ");
		}
	}
}

VOID CProcess::SetClanChat(char *msg)
{
/*
	if(!m_IsStart) return;
	m_Packet.SetClanChat(msg);
#ifdef __YOUDEBUG
	MiLae_Debug_txt("Ŭ���޼��� : %s",msg);
#endif
	m_cMemMap->WriteMemoryMap((char*)&m_Packet.GetClanChat());
	SendMessage(m_hRemoteWnd,WM_CALLMEMMAP,(WPARAM)20000,0);

#ifdef __YOUDEBUG
	MiLae_Debug_txt("Ŭ���޼��� : WM_CALLMEMMAP ȣ��");
#endif
*/

}