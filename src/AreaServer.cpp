#include "smlib3d\\smd3d.h"
#include "smwsock.h"

#include "character.h"
#include "Sound\\dxwav.h"
#include "particle.h"
#include "fileread.h"
#include "playmain.h"
#include "drawsub.h"
#include "netplay.h"
#include "field.h"
#include "hobaram\\holinkheader.h"
#include "sinbaram\\sinlinkheader.h"

#include "effectsnd.h"
#include "playsub.h"
#include "skillsub.h"
#include "Language\\language.h"

#include "AreaServer.h"

TRANS_SEVER_MAP	TransAreaServer;
int	AreaServerMode = 0;

HANDLE	hAreaServerContol_Thread = 0;
DWORD	dwAreaServerContol_ID = 0;

CRITICAL_SECTION	cSection_Main;

smWINSOCK	*lpWSockServer_Area[2] = { 0,0 };
smWINSOCK	*lpWSockServer_DispArea[2] = { 0,0 };
smWINSOCK	*lpWSockServer_AreaTemp = 0;


//������ ������Ʈ ��ȣ ����
int AreaSetObjectSerial( smWINSOCK *lpsmSock )
{

	//������ 
	smTRNAS_PLAYERINFO	TransPlayerInfo;

	memcpy( &TransPlayerInfo.smCharInfo , &lpCurPlayer->smCharInfo , sizeof( smCHAR_INFO ) );

	SetMaxDamage( &TransPlayerInfo.smCharInfo );	//�ִ� ������ ��������ü�� ����

	TransPlayerInfo.size = sizeof( smTRNAS_PLAYERINFO );
	TransPlayerInfo.code = smTRANSCODE_PLAYERINFO2;
	TransPlayerInfo.dwObjectSerial = lpCurPlayer->dwObjectSerial;

	TransPlayerInfo.x = lpCurPlayer->pX;
	TransPlayerInfo.y = lpCurPlayer->pY;
	TransPlayerInfo.z = lpCurPlayer->pZ;

	lpsmSock->Send3( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	return TRUE;
}


DWORD	dwDebugAreaIP[4];
DWORD	dwDebugAreaCount = 0;
DWORD	dwDebugAreaConnCount = 0;
DWORD	dwDebugAreaStep = 0;

DWORD WINAPI AreaServer_ControlThread( void *pInfo )
{
	HANDLE	hThread;
	DWORD	dwTime;
//	int		cnt;

	DWORD	dwSvrIP[2];
	int		SvrCode;
	DWORD	dwIP,dwIP_Back;

	int		MainStageNum;
	int		BackStageNum;
	char	szBuff[256];
	char	*lpBuff;
	int		Counter=0;
	sFIELD	*StgField[2];

	smWINSOCK	*lpsmSock;


	hThread = GetCurrentThread();
	dwTime = GetCurrentTime();

	while(1) {
	
		dwDebugAreaStep = 1;			//����� ���

		Sleep(1000);

		dwDebugAreaStep = 2;			//����� ���

		if ( quit ) break;

		StgField[0] = StageField[ 0 ];
		StgField[1] = StageField[ 1 ];
		if ( !StgField[0] ) StgField[0] = StgField[1];
		if ( !StgField[1] ) StgField[1] = StgField[0];

		dwDebugAreaStep = 3;			//����� ���

		if ( lpCurPlayer->OnStageField>=0 && StgField[ 0 ] && StgField[ 1 ] ) {

			dwDebugAreaStep = 4;			//����� ���

			EnterCriticalSection( &cSection_Main );			//ũ��ƼĮ ���� ����

			dwDebugAreaStep = 5;			//����� ���

			MainStageNum = Counter&1;
			BackStageNum = (Counter+1)&1;

			SvrCode = StgField[ MainStageNum ]->FieldCode;
			dwIP = TransAreaServer.dwIP[ SvrCode ];

			dwIP_Back = TransAreaServer.dwIP[ StgField[ BackStageNum ]->FieldCode ];

			if ( lpWSockServer_Area[0] ) dwSvrIP[0] = lpWSockServer_Area[0]->acc_sin.sin_addr.S_un.S_addr;
			else dwSvrIP[0]=0;

			if ( lpWSockServer_Area[1] ) dwSvrIP[1] = lpWSockServer_Area[1]->acc_sin.sin_addr.S_un.S_addr;
			else dwSvrIP[1]=0;

			dwDebugAreaStep = 6;			//����� ���

			if ( dwSvrIP[0] ) {
				if ( dwSvrIP[0]!=dwIP && dwSvrIP[0]!=dwIP_Back ) {
					if ( smConfig.DebugMode ) 
					{
						wsprintf( szBuff , "Area shutdown server (%s)" , lpWSockServer_Area[0]->szIPAddr );
						AddChatBuff( szBuff , 0 );
					}

					lpWSockServer_Area[0]->CloseSocket();
					lpWSockServer_Area[0] = 0;
					dwSvrIP[0]=0;
				}
			}

			if ( dwSvrIP[1] ) {
				if ( dwSvrIP[1]!=dwIP && dwSvrIP[1]!=dwIP_Back ) {
					if ( smConfig.DebugMode ) {
						wsprintf( szBuff , "Area shutdown server (%s)" , lpWSockServer_Area[1]->szIPAddr );
						AddChatBuff( szBuff , 0 );
					}

					lpWSockServer_Area[1]->CloseSocket();
					lpWSockServer_Area[1] = 0;
					dwSvrIP[1]=0;
				}
			}


			dwDebugAreaStep = 7;			//����� ���

			dwDebugAreaIP[0] = dwIP;
			dwDebugAreaIP[1] = dwSvrIP[0];
			dwDebugAreaIP[2] = dwSvrIP[1];

			LeaveCriticalSection( &cSection_Main );			//ũ��ƼĮ ���� ����

			dwDebugAreaStep = 8;			//����� ���

			if ( dwIP!=dwSvrIP[0] && dwIP!=dwSvrIP[1] ) {
				//������ �����ؾ� �� ��Ȳ

				dwDebugAreaStep = 9;			//����� ���

				dwDebugAreaConnCount++;

				lpBuff = ConvStrIP(dwIP);
				lpWSockServer_AreaTemp = smConnectSock3( lpBuff  , TCP_SERVPORT );


				dwDebugAreaStep = 10;			//����� ���

				if ( quit ) break;


				dwDebugAreaStep = 11;			//����� ���

				if ( lpWSockServer_AreaTemp ) {

					dwDebugAreaStep = 12;			//����� ���

					EnterCriticalSection( &cSection_Main );			//ũ��ƼĮ ���� ����

					dwDebugAreaStep = 13;			//����� ���

					if ( smConfig.DebugMode ) 
					{
						wsprintf( szBuff , "Region Server Connection (%s)" , lpBuff );
						AddChatBuff( szBuff , 0 );
					}


					lpsmSock = lpWSockServer_Area[MainStageNum];
					lpWSockServer_Area[MainStageNum] = lpWSockServer_AreaTemp;

					if ( smConfig.DebugMode )
					{
						wsprintf( szBuff , "Local server settings (%d)" , MainStageNum );
						AddChatBuff( szBuff , 0 );
					}

					dwDebugAreaStep = 14;			//����� ���

					//////////// �̰����� smWsockExtendServer �� �������ش� [����]
					//������ ������Ʈ ��ȣ ����
					AreaSetObjectSerial( lpWSockServer_AreaTemp );

					if ( smConfig.DebugMode ) 
						SendAdminMode( TRUE , lpWSockServer_AreaTemp );		//����� ����

					dwDebugAreaStep = 15;			//����� ���

					if ( lpsmSock ) {
						if ( !lpWSockServer_Area[BackStageNum] ) {
							lpWSockServer_Area[BackStageNum] = lpsmSock;
						}
						else {
							if ( smConfig.DebugMode ) {
								wsprintf( szBuff , "�������� ���� (%s)" , lpsmSock->szIPAddr );
								AddChatBuff( szBuff , 0 );
							}
							//���� ����
							lpsmSock->CloseSocket();
						}
					}
					lpWSockServer_AreaTemp = 0;

					dwDebugAreaStep = 16;			//����� ���

					LeaveCriticalSection( &cSection_Main );			//ũ��ƼĮ ���� ����
				}
				else {
					dwDebugAreaStep = 17;			//����� ���

					EnterCriticalSection( &cSection_Main );			//ũ��ƼĮ ���� ����

					dwDebugAreaStep = 18;			//����� ���

					if ( smConfig.DebugMode ) {
						wsprintf( szBuff , "�������� ������� (%s)" , lpBuff );
						AddChatBuff( szBuff , 0 );
					}
					LeaveCriticalSection( &cSection_Main );			//ũ��ƼĮ ���� ����

					dwDebugAreaStep = 19;			//����� ���

				}
			}

			dwDebugAreaCount++;

			dwDebugAreaStep = 20;			//����� ���

			EnterCriticalSection( &cSection_Main );			//ũ��ƼĮ ���� ����

			dwDebugAreaStep = 21;			//����� ���

			if ( dwIP && dwIP==dwIP_Back ) {
				if ( lpWSockServer_Area[0] && lpWSockServer_Area[0]->acc_sin.sin_addr.S_un.S_addr==dwIP ) {
					lpWSockServer_DispArea[0] = lpWSockServer_Area[0];
					lpWSockServer_DispArea[1] = lpWSockServer_Area[0];
				}
				if ( lpWSockServer_Area[1] && lpWSockServer_Area[1]->acc_sin.sin_addr.S_un.S_addr==dwIP ) {
					lpWSockServer_DispArea[0] = lpWSockServer_Area[1];
					lpWSockServer_DispArea[1] = lpWSockServer_Area[1];
				}
			}
			else {
				lpWSockServer_DispArea[0] = lpWSockServer_Area[0];
				lpWSockServer_DispArea[1] = lpWSockServer_Area[1];
			}

			dwDebugAreaStep = 22;			//����� ���

			LeaveCriticalSection( &cSection_Main );			//ũ��ƼĮ ���� ����

			dwDebugAreaStep = 23;			//����� ���

		}


		dwDebugAreaStep = 24;			//����� ���

		Counter++;
	}

	dwDebugAreaStep = 25;			//����� ���

	ExitThread( TRUE );
	return TRUE;
}


int RecvAreaServerMap( TRANS_SEVER_MAP *lpTransServerMap )
{

	ZeroMemory( &TransAreaServer , sizeof(TRANS_SEVER_MAP) );
	memcpy( &TransAreaServer , lpTransServerMap , lpTransServerMap->size );
	AreaServerMode = TRUE;

	if ( !hAreaServerContol_Thread ) {

		hAreaServerContol_Thread = CreateThread( NULL , 0, AreaServer_ControlThread , 0 , 0, &dwAreaServerContol_ID );

	}


	return TRUE;
}

/*
//�ε��� ȭ�� ǥ��
int OpenTimeCheckThread()
{
	DWORD	dwTimeCheckThreadID;

	if ( !hTimeCheckThread ) {
		hTimeCheckThread = CreateThread( NULL , 0, TimeCheckThread , 0 , 0, &dwTimeCheckThreadID );
	}

	return TRUE;
}
*/