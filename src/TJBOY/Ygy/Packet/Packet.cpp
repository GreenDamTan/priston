#include <windows.h>
#include "..\\YgyHeader.h"



CPacket::CPacket()
{
}
CPacket::~CPacket()
{
}


VOID CPacket::Init()
{	
	ZeroMemory(memMapReadData,sizeof(memMapReadData));
}
VOID CPacket::Clear()
{	
}


/*========================================================================================
Ŭ���� ������ ĳ���͸� �����Ҷ�
=========================================================================================*/
VOID CPacket::SetSelectCha(char *id,char *chaname, char *clanname,DWORD clannum)
{
#if 0
	ZeroMemory(&m_sSelectCha,sizeof(SELECTCHA));
	m_sSelectCha.index.dwIndex = INSIDE_SELECTCHA;
	if(id != NULL)
		strncpy(m_sSelectCha.szID,id,strlen(id));
	if(chaname != NULL)
		strncpy(m_sSelectCha.szChaName,chaname,strlen(chaname));
	if(clanname !=NULL)
		strncpy(m_sSelectCha.szClanName,clanname,strlen(clanname));
	m_sSelectCha.dwClanNum = clannum;
#endif
	
	
	ZeroMemory(&m_userInfo,sizeof(m_userInfo));
	m_userInfo.sIndex.dwIndex = INSIDE_USERINFO;
	if(id != NULL)
		strncpy(m_userInfo.szID,id,strlen(id));
	if(chaname != NULL)
		strncpy(m_userInfo.szCharName, chaname,strlen(chaname));
	if(clanname !=NULL)
		strncpy(m_userInfo.szClanName,clanname,strlen(clanname));
	m_userInfo.dwClanNum = clannum;
}


// ��öȣ
// Ŭ��ä�� ����
int AddChatBuff( char *szMsg , DWORD dwIP );

/*======================================================================================
���� ����Ÿ�� �Ľ�
========================================================================================*/
VOICTALKUSERDATA voiceUser;		//���ѻ������Ʈ��
char voiceInfoStr[256]={0,};	//���̽� Ŭ���̾�Ʈ ����

void fd2( char *fmt, ...);

BOOL talk_check(VOICTALKUSERDATA *vUser)
{
	static tCnt=0;
	static char  talk[15][32];
	static DWORD ttime[15];

	int i, j;

	//10���̻� ���� ����Ÿ�� ����
	DWORD t = timeGetTime();
	for(i=0; i<15; i++) {

		if(ttime[i] != 0 && (t - ttime[i])  >= 1000*15) {
			ttime[i] = 0;
		}
	}
	

	//����Ÿ�� ���� ���Ѵ�.
	for(i=0; i<8; i++) {
		if(vUser->uID[i][0]==0) continue;
		for(j=0; j<15; j++) 
		{
			if(ttime[j]==0) continue;

			//�̹� ���̺�������� �Ѿ��.
			if( strcmp( vUser->uID[i], talk[j]) ==0 ) 
			{
				goto jmp1;
			}
		}




		//÷���ͼ� ���ϰ� ���̺��.
		char szchatbuf[64];
		wsprintf(szchatbuf,"%s ���� ���Ͽ����ϴ�.", vUser->uID[i]);
		AddChatBuff(szchatbuf, 5);


		//���ڸ� ã�Ƽ� �ִ´�.
		for(j=0; j<15; j++) 
		{
			if(ttime[j]==0) 
			{
				ZeroMemory(&talk[ j ], sizeof(talk[ j ]));
				strcpy( talk[ j ], vUser->uID[i]);
				
				ttime[ j ] = t;
				break;
			}
		}


jmp1:
		continue;

	}
	return TRUE;
}

VOID CPacket::Parsing()
{	

	DWORD dwIndex = 0;
	m_Index = (INDEX*)memMapReadData;
	dwIndex = m_Index->dwIndex;
	switch(dwIndex)
	{

	case GOPRIS_VUSERIDs:
		{
		VOICTALKUSERDATA *vU = (VOICTALKUSERDATA*)memMapReadData;
		memcpy(&voiceUser, vU, sizeof( VOICTALKUSERDATA ));
		voiceUser.time = timeGetTime();
		talk_check(vU);
		}
		
		break;

	case GOPRIS_VCLIENTINFO:
		{
		VCLIENTINFO *vci = (VCLIENTINFO *)memMapReadData;
		
		
		if(vci->vcl_state==VC_STATE_NOSERVER){		//���̽������� ����.
			wsprintf(voiceInfoStr, "%s", VC_STATE_NOSERVER_STR);
		}
		if(vci->vcl_state==VC_STATE_NOTCONNECT){	//���̽������� �����Ҽ� ����.

			wsprintf(voiceInfoStr, "%s", VC_STATE_NOTCONNECT_STR);
		}
		if(vci->vcl_state==VC_STATE_CONNECT){		//���̽������� connect
		
			wsprintf(voiceInfoStr, "%s", VC_STATE_CONNECT_STR);
		}
		if(vci->vcl_state==VC_STATE_DISCONNECT){	//���̽������� disconnect
			wsprintf(voiceInfoStr, "%s", VC_STATE_DISCONNECT_STR);
		}



		}
		break;


#if 0
	/*
	case P_IDANDNAME:
		m_SendIDandChaName = (P_IDandCHANANE*)memMapReadData;
		break;
		*/
	case P_OUT:
		m_SendOut = (P_Out*)memMapReadData;
		break;
		/*
	case P_CLANINFO:
		m_SendClanInfo = (Packet_CLANINFO*)memMapReadData;
		break;
		*/

#endif

	
	}	
}




VOID CPacket::SetClanChat(char *msg)
{
	m_sClanChat.sIndex.dwIndex = INSIDE_CLANCHAT;
	strncpy(m_sClanChat.szMsg,msg,strlen(msg));
}

