
#include "..\\Client\\XTrap4Client.h"
#include "MessageProtocol.h"

#define CC4FILE_NUM	2											// ���� MapFile �� ���� ����

UCHAR	sXTrapCS_Buf[CC4FILE_NUM][XTRAP_CC4_FILESIZE_MAP];

int rsXTrap_Init()
{
	char szDbg[512];

	FILE * fp = NULL;
	fp=fopen(szMAP1_FILE_NAME,"rb");
	if ( fp ) 
	{ 
		size_t size = 0;
		size = fread(sXTrapCS_Buf[0] , sizeof(UCHAR), XTRAP_CC4_FILESIZE_MAP , fp ); 
		fclose(fp); fp = NULL;

		sprintf(szDbg,"size1 : %d",size);
		//OutputDebugString(szDbg);
	}
	else
	{
		MessageBox(NULL,"map file 1","error",MB_OK);
	}

	fp = NULL;
	fp=fopen(szMAP2_FILE_NAME,"rb");
	if ( fp )
	{ 
		size_t size = 0;
		size = fread( sXTrapCS_Buf[1] , sizeof(UCHAR), XTRAP_CC4_FILESIZE_MAP , fp ); 
		fclose(fp); fp = NULL;

		sprintf(szDbg,"size2 : %d",size);
		//OutputDebugString(szDbg);
	}
	else
	{
		MessageBox(NULL,"map file 2","error",MB_OK);
	}
	
	unsigned long ulReturn = 0;
	//ulReturn = XTrapCC_ServerInit( rsServerConfig.XTRAP_Timer, CC4FILE_NUM, (PUCHAR)sXTrapCS_Buf );	����
	ulReturn = XTrap_S_Start( rsServerConfig.XTRAP_Timer, CC4FILE_NUM, (PUCHAR)sXTrapCS_Buf ,NULL);	//XTrapD5
	
	if( ulReturn == 0 )
	{
	}
	else
	{
		MessageBox( NULL, "map������ Ʋ���ϴ�.", "����Ʈ�� ����", MB_ICONSTOP);
	}
	return TRUE;
}


//�α��� ���� (����Ʈ�� Ű����)
//
int rsXTrap_SeedKey( rsPLAYINFO *lpPlayInfo )
{
	char szDbg[512] = {0,};
	XTRAPCS_SESSION_BUF *lpXTrapCS_Session_Buf = &lpPlayInfo->XTrapCS_Session_Buf;

	//----[0. 20�� �� �ٽ� ����ǰ� ���� ]
	lpXTrapCS_Session_Buf->dwTime	= dwPlayServTime+1000*20;	// 20 seconds

	//----[1. Xtrap�ʱ�ȭ �κ�,�ѹ��� ����Ǿ�� �Ѵ� ]
	if(lpXTrapCS_Session_Buf->bFirst == true)
	{	
		lpXTrapCS_Session_Buf->bFirst = false;
		//XTrapCC_SessionInit( rsServerConfig.XTRAP_Timer, CC4FILE_NUM, (PUCHAR)sXTrapCS_Buf, (PUCHAR)lpXTrapCS_Session_Buf->m_SessionBuf );	//����
		XTrap_S_SessionInit( rsServerConfig.XTRAP_Timer, CC4FILE_NUM, (PUCHAR)sXTrapCS_Buf, (PUCHAR)lpXTrapCS_Session_Buf->m_SessionBuf );	//����
	}

	//----[2. Xtrap�˻� �κ� ]
	unsigned long ulRet = 0;
	ulRet = XTrap_CS_Step1( (PUCHAR)lpXTrapCS_Session_Buf->m_SessionBuf, (PUCHAR)lpXTrapCS_Session_Buf->m_PacketBuf);

	//----[3. Send PacketBuf ]
	smTRANS_XTRAP_ADDR	smTransXtrapAddr;
	memset(&smTransXtrapAddr, 0x00, sizeof(smTransXtrapAddr));

	smTransXtrapAddr.smTransCommand.size = sizeof(smTransXtrapAddr);
	smTransXtrapAddr.smTransCommand.code = smTRANSCODE_XTRAP_PACKET;
	smTransXtrapAddr.smTransCommand.WParam = XTRAP_CMD_SEEDKEY;
	memcpy(smTransXtrapAddr.PacketBuf,lpXTrapCS_Session_Buf->m_PacketBuf,sizeof(smTransXtrapAddr.PacketBuf));
	int iSendSize = lpPlayInfo->lpsmSock->Send2( (char *)&smTransXtrapAddr , smTransXtrapAddr.smTransCommand.size , TRUE );

	sprintf( szDbg, "SEND SIZE : %d", iSendSize ); //x�ӽ�
	//OutputDebugString( szDbg );

	//----[4. ���ϰ� �˻� ]
	if( ulRet == 0 )
	{//����
	}
	else
	{//������
		smTRANS_COMMAND	smTransCommand;
		smTransCommand.WParam = 8710;
		smTransCommand.LParam = 100;
		smTransCommand.SParam = 0;
		RecordHackLogFile( lpPlayInfo, &smTransCommand );
		lpPlayInfo->dwHopeDisconnectTime = dwPlayServTime+1000*10;		//10���� ����
	}


	//lpXTrapCS_Session_Buf->KeyGenWaitCount ++;

	//sprintf( szDbg, "WAIT COUNT : %d", lpXTrapCS_Session_Buf->KeyGenWaitCount );
	//OutputDebugString( szDbg );

	//int MaxDelay = rsServerConfig.XTRAP_MaxDealy;
	//if ( !MaxDelay ) MaxDelay = 5;

	//if ( lpXTrapCS_Session_Buf->KeyGenWaitCount>MaxDelay )
	//{
	//	//���ѽð� �̻� Ŭ���̾�Ʈ���� XTRAP ������ ���°��
	//	smTransCommand.WParam = 8710;
	//	smTransCommand.LParam = 100;
	//	smTransCommand.SParam = lpXTrapCS_Session_Buf->KeyGenWaitCount;
	//	RecordHackLogFile( lpPlayInfo , &smTransCommand );

	//	lpPlayInfo->dwHopeDisconnectTime = dwPlayServTime+10000;		//10���� ����
	//}


	return TRUE;
}


int rsXTrap_RecvPacket( rsPLAYINFO *lpPlayInfo , smTRANS_COMMAND *lpPacket )
{
	char szDbg[512] = {0,};

	DWORD dwTransCode = lpPacket->WParam;
	XTRAPCS_SESSION_BUF *lpXTrapCS_Session_Buf = &lpPlayInfo->XTrapCS_Session_Buf;

	sprintf( szDbg, "BUF ADDR2 : %08X", lpPlayInfo->XTrapCS_Session_Buf );
	//OutputDebugString( szDbg );

	if ( dwTransCode==XTRAP_CMD_UNIQKEY ) 
	{
		
		sprintf( szDbg, "RECV SIZE : %d", lpPacket->size );
		//OutputDebugString( szDbg );
		
		memcpy( lpXTrapCS_Session_Buf->m_PacketBuf, ((smTRANS_XTRAP_ADDR *)lpPacket)->PacketBuf , sizeof(lpXTrapCS_Session_Buf->m_PacketBuf));
		XTrap_CS_Step3( (PUCHAR)lpXTrapCS_Session_Buf->m_SessionBuf, (PUCHAR)lpXTrapCS_Session_Buf->m_PacketBuf );
	}


	return TRUE;
}
