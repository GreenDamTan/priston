/*=============================================================================================
�޸𸮸ʰ� ������ ��Ʈ���ϴ� Ŭ����
==============================================================================================*/
#pragma once
#ifndef __PROCESS_H__
#define __PROCESS_H__


#define WM_CALLMEMMAP				WM_USER+3   //�޸𸮸ʿ� �޼����� �ִٰ� �뺸
#define WM_INITREMOTEPROCESS		WM_USER+4
#define WM_GAMECALLMAP              WM_USER+5

#define _CLANWON_MAX   90





#define REMOTEPROCESS_EXE  "vrun.dat"
#define REMOTEPROCESS_NAME  "VoiceChat"


class CProcess
{
public:
	CProcess();
	~CProcess();
public:
	BOOL	Init();
	VOID	Clear();
	VOID	Main();

	VOID    CheckIPandPort(char *ip, int port);
	int		InitRemoteProcess();
	int		InitRmoteProcessWnd();

	VOID	TestInputData(HWND hWnd,WPARAM wParam,LPARAM lParam,char *temp);
	VOID	PacketParsing();

	VOID    SetStep(int step);
	
	VOID	SetSelectCha(char *id,char *chaname, char *clanname,DWORD clannum, int utype);
	VOID	UserOut();


	//���߿� Ŭ��ä���Ұ�
	VOID    SetClanChat(char *msg);


		// ��öȣ (����ũ �¿���)
	VOID	SetMicOnOFF();



public:
	inline BOOL            GetbIsStart(){return m_IsStart;}

private:
	CMemMap     *m_cMemMap;
	CPacket     m_Packet;

	HANDLE      m_hRemoteProcess;  //vrun.dat�� �ڵ�� �����ڵ�
	HWND        m_hRemoteWnd;	

	int         m_nStep;
	//int         m_nKeyDown;

	BOOL        m_bIsMemMap;

	BOOL        m_IsStart;

	//ktj : ������ ������ ��Ƶд�.
	USERINFO	m_userInfo;
};

#endif

#ifdef PROCESS_CPP
class CProcess g_CtrlProcess;
#else
extern class CProcess g_CtrlProcess;
#endif


VOID SetClanWon(char *chaname,char* clanname,DWORD clannum);