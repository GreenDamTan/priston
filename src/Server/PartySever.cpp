#define _WIN32_WINNT    0x0403  /* Get new material from the headers */
#define _WIN32_WINDOWS  0x0403

#include <windows.h>
#include <io.h>
#include <stdio.h>         /* for sprintf                           */
#include <string.h>        /* for strlen                            */
#include <memory.h>
#include <process.h>       /* for _beginthread                      */
//#include <winsock2.h>
//#include <mswsock.h>

#include "..\\resource.h"
#include "..\\smwsock.h"

#include "..\\smlib3d\\smd3d.h"
#include "..\\character.h"
#include "..\\fileread.h"
#include "..\\playmain.h"
#include "..\\field.h"
#include "..\\sinbaram\\sinlinkheader.h"
#include "Language\\language.h"
#include "partymsg.h"
#include "onserver.h"

extern DWORD	dwPlayServTime;

/*
	//��Ƽ��
	int	CreateParty( rsPLAYINFO *lpPlayInfo );			//��Ƽ���� - ( �ʱ� ��Ƽ�� ���� )
	int	JoinPartyPlayer( rsPLAYINFO *lpPlayInfo );		//��Ƽ�� ����
	int	DeletePartyPlayer( rsPLAYINFO *lpPlayInfo );	//��Ƽ���� Ż��
	int	UpdatePartyPlayer();							//��Ƽ�� ������Ʈ
	int	LeavePartyMaster( rsPLAYINFO *lpPlayInfo );		//��Ƽ�� ����
	int	ReleaseParty();									//��Ƽ�� �ػ�

	//��Ƽ��
	int	JoinParty( rsPLAYINFO *lpPlayMaster );			//��Ƽ ���� ��û
	int	SecedeParty ( rsPLAYINFO *lpPlayMaster );		//��Ƽ Ż��
*/
/*
	////////////// ��Ƽ ���� ���� ///////////////////
	DWORD			dwPartyInfo;						//��Ƽ ����
	rsPLAYINFO		*lpPartyMaster;						//��Ƽ�� ����Ʈ
	DWORD			PartyMasterObjectCode;				//��Ƽ�� ������Ʈ �ڵ�
	rsPLAYINFO		*lpPartyPlayers[PARTY_PLAYER_MAX];	//��Ƽ�� ����Ʈ
	DWORD			dwPartyObjectCode[PARTY_PLAYER_MAX];//��Ƽ���� ������Ʈ �ڵ�
*/

//ĳ���͸� ã�´� ( ���߿� ���� ������ �ְ� �ڵ�ȭ ���� �ٲ� ��� �� )
rsPLAYINFO *srFindUserFromSerial( DWORD dwObjectSerial );
//������ �����ۼ��� ����ü�� �����ڵ� ���� ( �����Ͻ� ���������� ���� )
int	rsRegist_ItemSecCode( rsPLAYINFO *lpPlayInfo , TRANS_ITEMINFO *lpTransItemInfo , int NewItem );


//��Ƽ������ ����Ÿ ����
int rsPLAYINFO::SendPartyData( rsPLAYINFO *lpPlayInfo_From , char *szData  )
{
	int size;
	int cnt;


	size = ((smTRANS_COMMAND *)szData)->size;

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]!=lpPlayInfo_From ) {
			if ( lpPartyPlayers[cnt]->lpsmSock ) {
				lpPartyPlayers[cnt]->lpsmSock->Send2( szData , size , TRUE );
			}
		}
	}


	return TRUE;
}

//��Ƽ������ �޼��� ����
int rsPLAYINFO::SendPartyMessage( char *szMessage , DWORD dwCode )
{
	TRANS_CHATMESSAGE	TransChatMessage;
	int cnt;

	if ( lpPartyMaster ) {
		lstrcpy( TransChatMessage.szMessage , szMessage );
		TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
		TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
		TransChatMessage.dwIP = dwCode;
		TransChatMessage.dwObjectSerial = 0;

		for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
			if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]->lpsmSock ) {
				lpPartyPlayers[cnt]->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
			}
		}
	}
	else {
		lstrcpy( TransChatMessage.szMessage , szMessage );
		TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
		TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
		TransChatMessage.dwIP = 0;
		TransChatMessage.dwObjectSerial = 0;
		if ( lpsmSock ) 
			lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
	}

	return TRUE;
}


//��Ƽ����
int rsPLAYINFO::CreateParty( rsPLAYINFO *lpPlayInfo )
{
	int cnt;

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		lpPartyPlayers[cnt]=0;
		dwPartyObjectCode[0] = 0;
	}

	PartyUpdateFlag = 0;
	dwPartyInfo = PARTY_STATE_MASTER;
	lpPartyMaster = this;

	//��Ƽ�� ����
	lpPartyPlayers[0] = this;
	dwPartyObjectCode[0] = dwObjectSerial;
	if ( this!=lpPlayInfo ) {
		lpPartyPlayers[1] = lpPlayInfo;
		dwPartyObjectCode[1] = lpPlayInfo->dwObjectSerial;
	
		lpPlayInfo->dwPartyInfo = PARTY_STATE_MEMBER;
		lpPlayInfo->lpPartyMaster = this;
	}

	UpdatePartyPlayer();

	return TRUE;
}

//��Ƽ�� ����
int	rsPLAYINFO::JoinPartyPlayer( rsPLAYINFO *lpPlayInfo )
{
	int cnt;

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]==lpPlayInfo ) {
			//�̹� ���Ե� ����
			return FALSE;
		}
	}

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( !lpPartyPlayers[cnt] ) {
			lpPartyPlayers[cnt] = lpPlayInfo;
			dwPartyObjectCode[cnt] = lpPlayInfo->dwObjectSerial;
			lpPlayInfo->dwPartyInfo = PARTY_STATE_MEMBER;
			lpPlayInfo->lpPartyMaster = this;
			UpdatePartyPlayer();
			return TRUE;
		}
	}

	return FALSE;
}

//��Ƽ���� Ż��
int	rsPLAYINFO::DeletePartyPlayer( rsPLAYINFO *lpPlayInfo )
{
	int cnt;
	smTRANS_COMMAND		TransCommand;

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]==lpPlayInfo ) {
			lpPartyPlayers[cnt]->dwPartyInfo = PARTY_STATE_NONE;
			lpPartyPlayers[cnt]->lpPartyMaster = NULL;
			lpPartyPlayers[cnt] = 0;
			dwPartyObjectCode[cnt] = 0;
			UpdatePartyPlayer();


			if ( lpPlayInfo->lpsmSock ) {
				TransCommand.code = smTRANSCODE_PARTY_RELEASE;
				TransCommand.size = sizeof(smTRANS_COMMAND);
				TransCommand.WParam = dwObjectSerial;
				TransCommand.LParam = 0;
				TransCommand.SParam = 0;

				lpPlayInfo->lpsmSock->Send( (char *)&TransCommand , TransCommand.size , TRUE );
			}
			return TRUE;
		}
	}

	return FALSE;
}

//��Ƽ�� ����
int	rsPLAYINFO::ChangePartyPlayer( rsPLAYINFO *lpOldPlayInfo , rsPLAYINFO *lpNewPlayInfo )
{
	int cnt;

	if ( !lpPartyMaster ) {
		//��Ƽ���� �ƴ� ��� ( ��Ƽ�� ���� )
		lpNewPlayInfo->dwPartyInfo = PARTY_STATE_NONE;
		lpNewPlayInfo->lpPartyMaster = NULL;
		return FALSE;
	}

	//����� ��Ƽ���� ��Ƽ���� ���
	if ( lpOldPlayInfo==lpPartyMaster ) {
		for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
			if ( lpPartyPlayers[cnt] ) {
				lpPartyPlayers[cnt]->lpPartyMaster = lpNewPlayInfo;
			}
		}
		lpNewPlayInfo->dwPartyInfo = PARTY_STATE_MASTER;
		lpNewPlayInfo->lpPartyMaster = lpNewPlayInfo;
	}

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyMaster->lpPartyPlayers[cnt]==lpOldPlayInfo ) {
			lpPartyMaster->lpPartyPlayers[cnt] = lpNewPlayInfo;
			break;
		}
	}

	lpOldPlayInfo->dwPartyInfo = PARTY_STATE_NONE;
	lpOldPlayInfo->lpPartyMaster = NULL;

	//lpNewPlayInfo->dwPartyInfo = PARTY_STATE_NONE;
	//lpNewPlayInfo->lpPartyMaster = NULL;

	return TRUE;
}

//��Ƽ�� ���� ������Ʈ
int	rsPLAYINFO::UpdatePartyPlayer()
{
	int cnt,cnt2;
	TRANS_PARTY_USERS	TransPartyUsers;
	int	LevelAvg;

	cnt = 0;
	cnt2 = 0;

	if ( !lpPartyMaster ) return FALSE;

	//��Ƽ�� ����
	TransPartyUsers.PartyUser[cnt2].dwObjectSerial = lpPartyMaster->dwObjectSerial;
	lstrcpy( TransPartyUsers.PartyUser[cnt2].szName , lpPartyMaster->smCharInfo.szName );
	lstrcpy( TransPartyUsers.PartyUser[cnt2].szModelName , lpPartyMaster->smCharInfo.szModelName );
	lstrcpy( TransPartyUsers.PartyUser[cnt2].szModelName2 , lpPartyMaster->smCharInfo.szModelName2 );
	TransPartyUsers.PartyUser[cnt2].Life[0] = lpPartyMaster->smCharInfo.Life[0];
	TransPartyUsers.PartyUser[cnt2].Life[1] = lpPartyMaster->smCharInfo.Life[1];
	TransPartyUsers.PartyUser[cnt2].Level = lpPartyMaster->smCharInfo.Level;
	TransPartyUsers.PartyUser[cnt2].x = lpPartyMaster->Position.x;
	TransPartyUsers.PartyUser[cnt2].z = lpPartyMaster->Position.z;
	LevelAvg = lpPartyMaster->smCharInfo.Level;
	cnt2++;

	//��Ƽ�� ����
	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]!=lpPartyMaster ) {
			TransPartyUsers.PartyUser[cnt2].dwObjectSerial = lpPartyPlayers[cnt]->dwObjectSerial;
			lstrcpy( TransPartyUsers.PartyUser[cnt2].szName , lpPartyPlayers[cnt]->smCharInfo.szName );
			lstrcpy( TransPartyUsers.PartyUser[cnt2].szModelName , lpPartyPlayers[cnt]->smCharInfo.szModelName );
			lstrcpy( TransPartyUsers.PartyUser[cnt2].szModelName2 , lpPartyPlayers[cnt]->smCharInfo.szModelName2 );
			TransPartyUsers.PartyUser[cnt2].Life[0] = lpPartyPlayers[cnt]->smCharInfo.Life[0];
			TransPartyUsers.PartyUser[cnt2].Life[1] = lpPartyPlayers[cnt]->smCharInfo.Life[1];
			TransPartyUsers.PartyUser[cnt2].Level = lpPartyPlayers[cnt]->smCharInfo.Level;
			TransPartyUsers.PartyUser[cnt2].x = lpPartyPlayers[cnt]->Position.x;
			TransPartyUsers.PartyUser[cnt2].z = lpPartyPlayers[cnt]->Position.z;
			LevelAvg += lpPartyPlayers[cnt]->smCharInfo.Level;
			cnt2 ++;
		}
	}

	PartyUserCount = cnt2;
	PartyLevelAverage = LevelAvg/cnt2; 

	ZeroMemory( TransPartyUsers.dwTemp , sizeof(DWORD)*8 );

	TransPartyUsers.dwObjectSerial = dwObjectSerial;
	TransPartyUsers.PartyUserCount = cnt2;
	TransPartyUsers.size = sizeof(TRANS_PARTY_USERS)-((PARTY_PLAYER_MAX-cnt2)*sizeof(_PARTY_USER));
	TransPartyUsers.code = smTRANSCODE_PARTY_UPDATE;

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]->lpsmSock ) {
			lpPartyPlayers[cnt]->lpsmSock->Send( (char *)&TransPartyUsers , TransPartyUsers.size , TRUE );
		}
	}

	return TRUE;
}
/*
struct _PARTY_PLAYINFO {
	DWORD	dwObjectSerial;			//��ü ������ȣ
	DWORD	Level;					//����
	short	Life[2];				//�����
};

struct	TRANS_PARTY_PLAYINFO {
	int		size,code;

	DWORD	dwObjectSerial;
	DWORD	dwTemp;

	int		PartyUserCount;

	_PARTY_PLAYINFO	PlayInfo[PARTY_PLAYER_MAX];
};
*/

//��Ƽ�� ���� ������Ʈ
int	rsPLAYINFO::UpdatePartyPlayInfo()
{
	int cnt,cnt2;
	int	LevelAvg;
	TRANS_PARTY_PLAYINFO	TransPartyPlayInfo;

	cnt = 0;
	cnt2 = 0;

	PartyUpdateFlag = 0;

	//��Ƽ�� ����
	TransPartyPlayInfo.PlayInfo[cnt2].dwObjectSerial = dwObjectSerial;
	TransPartyPlayInfo.PlayInfo[cnt2].Life[0] = smCharInfo.Life[0];
	TransPartyPlayInfo.PlayInfo[cnt2].Life[1] = smCharInfo.Life[1];
	TransPartyPlayInfo.PlayInfo[cnt2].Level = smCharInfo.Level;
	TransPartyPlayInfo.PlayInfo[cnt2].x = Position.x;
	TransPartyPlayInfo.PlayInfo[cnt2].z = Position.z;
	LevelAvg = smCharInfo.Level;
	cnt2++;

	//��Ƽ�� ����
	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]!=this ) {
			TransPartyPlayInfo.PlayInfo[cnt2].dwObjectSerial = lpPartyPlayers[cnt]->dwObjectSerial;
			TransPartyPlayInfo.PlayInfo[cnt2].Life[0] = lpPartyPlayers[cnt]->smCharInfo.Life[0];
			TransPartyPlayInfo.PlayInfo[cnt2].Life[1] = lpPartyPlayers[cnt]->smCharInfo.Life[1];
			TransPartyPlayInfo.PlayInfo[cnt2].Level = lpPartyPlayers[cnt]->smCharInfo.Level;
			TransPartyPlayInfo.PlayInfo[cnt2].x = lpPartyPlayers[cnt]->Position.x;
			TransPartyPlayInfo.PlayInfo[cnt2].z = lpPartyPlayers[cnt]->Position.z;
			LevelAvg += lpPartyPlayers[cnt]->smCharInfo.Level;
			cnt2 ++;
		}
	}

	PartyUserCount = cnt2;
	PartyLevelAverage = LevelAvg/cnt2; 

	TransPartyPlayInfo.dwTemp = 0;

	TransPartyPlayInfo.dwObjectSerial = dwObjectSerial;
	TransPartyPlayInfo.PartyUserCount = cnt2;
	TransPartyPlayInfo.size = sizeof(TRANS_PARTY_PLAYINFO);
	TransPartyPlayInfo.code = smTRANSCODE_PARTY_PLAYUPDATE;

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]->lpsmSock ) {
			lpPartyPlayers[cnt]->lpsmSock->Send( (char *)&TransPartyPlayInfo , TransPartyPlayInfo.size , TRUE );
		}
	}

	return TRUE;
}



//���� ������ ��Ƽ�� ���� ������Ʈ /smTRANSCODE_PARTY_PLAYUPDATE
int	rsPLAYINFO::UpdateServerPartyPlayer( TRANS_PARTY_PLAYINFO *lpTransPartyPlayInfo )
{
	int cnt,cnt2;
	int LevelAvg;
	rsPLAYINFO *lpPlayInfo;
	DWORD	dwPartyCode1 , dwPartyCode2;

	LevelAvg = 0;

	if ( dwPartyInfo==PARTY_STATE_MASTER ) {

		dwPartyCode1 = 0;
		dwPartyCode2 = 0;

		for( cnt=0;cnt<lpTransPartyPlayInfo->PartyUserCount;cnt++ ) {
			dwPartyCode1 += lpTransPartyPlayInfo->PlayInfo[cnt].dwObjectSerial;
		}

		cnt = 0;
		for ( cnt2=0;cnt2<PARTY_PLAYER_MAX;cnt2++ ) {
			if ( lpPartyPlayers[cnt2] ) {
				dwPartyCode2 += lpPartyPlayers[cnt2]->dwObjectSerial;
				cnt++;
			}
		}

		if ( cnt==lpTransPartyPlayInfo->PartyUserCount && dwPartyCode1==dwPartyCode2 ) {
			//��Ƽ������ ���Ѱ� ����
			return TRUE;

		}
	}

	//////////// ���� ������Ʈ ///////////////
	//��Ƽ�� ���� ����
	if ( dwPartyInfo==PARTY_STATE_MASTER ) {
		for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
			if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]->lpsmSock ) {
				lpPartyPlayers[cnt]->dwPartyInfo = PARTY_STATE_NONE;
				lpPartyPlayers[cnt]->lpPartyMaster = NULL;
			}
		}
	}

	ZeroMemory( lpPartyPlayers , sizeof(rsPLAYINFO *)*PARTY_PLAYER_MAX );

	for( cnt=0;cnt<lpTransPartyPlayInfo->PartyUserCount;cnt++ ) {
		if ( lpTransPartyPlayInfo->PlayInfo[cnt].dwObjectSerial==dwObjectSerial ) {
			dwPartyInfo = PARTY_STATE_MASTER;
			lpPartyMaster = this;
			lpPartyPlayers[cnt] = this;
			LevelAvg += smCharInfo.Level;
		}
		else {
			lpPlayInfo = srFindUserFromSerial( lpTransPartyPlayInfo->PlayInfo[cnt].dwObjectSerial );
			if ( lpPlayInfo ) {
				lpPartyPlayers[cnt] = lpPlayInfo;
				lpPlayInfo->dwPartyInfo = PARTY_STATE_MEMBER;
				lpPlayInfo->lpPartyMaster = this;
				LevelAvg += lpPlayInfo->smCharInfo.Level;
			}
		}
	}

	//��Ƽ�� ���� ��շ��� ���
	PartyUserCount = lpTransPartyPlayInfo->PartyUserCount;
	if ( PartyUserCount ) PartyLevelAverage = LevelAvg/PartyUserCount; 

	return TRUE;
}



//���� ���̺� ����ġ ȹ��
int srGetTotalExp( int Exp , int Level );

// ����� - ��Ƽ ����ġ ȹ�淮 ����
int	PartyExpTable[8][2] = {
	{	100	,	0 },	// 100% - 1��
	{	65	,	65 },	// 130%	- 2��
	{	50	,	50 },	// 150% - 3��
	{	45	,	45 },	// 180% - 4��
	{	40	,	40 },	// 200% - 5��
	{	37	,	37 },	// 222% - 6��
	{	22	,	21 },	// 
	{	20	,	20 }	//
};

//��Ƽ�� ����ġ ȹ��
int rsPLAYINFO::GetPartyExp( smCHAR *lpChar , rsPLAYINFO *lpPlayInfo )
{
	smTRANS_COMMAND	smTransCommand;
	int	Exp;
	int	PartyExp[2];
	int	cnt;
	int x,z,dist;
	int Sucess = 0;
	int	ExpUp;

	if ( dwPartyInfo!=PARTY_STATE_MASTER || !lpsmSock ) return FALSE;

	//����ġ ȹ��
	smTransCommand.size = sizeof( smTRANS_COMMAND );
	smTransCommand.WParam = 0;										//ȹ�� ����ġ
	smTransCommand.LParam = lpChar->dwObjectSerial;					//���� ������ȣ
	Exp = srGetTotalExp( lpChar->smMonsterInfo.GetExp , lpChar->smCharInfo.Level-PartyLevelAverage );

	PartyExp[0] = (Exp*PartyExpTable[PartyUserCount-1][0])/100;			//���� ȹ��
	PartyExp[1] = (Exp*PartyExpTable[PartyUserCount-1][1])/100;			//��Ƽ�� ȹ��


	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]->lpsmSock ) {

			if ( lpPartyPlayers[cnt]==lpPlayInfo ) {
				smTransCommand.code = smTRANSCODE_ADDEXP;
				smTransCommand.WParam = PartyExp[0];										//���� ȹ��
				Sucess++;
			}
			else {
				smTransCommand.code = smTRANSCODE_ADDEXP_PARTY;
				x = (lpPlayInfo->Position.x - lpPartyPlayers[cnt]->Position.x)>>FLOATNS;
				z = (lpPlayInfo->Position.z - lpPartyPlayers[cnt]->Position.z)>>FLOATNS;
				dist = x*x+z*z;
				if ( dist<PARTY_GETTING_DIST )
					smTransCommand.WParam = PartyExp[1];										//��Ƽ�� ȹ��
				else
					smTransCommand.WParam = 0;
			}

			if ( smTransCommand.WParam ) {
				ExpUp = 0;

				if ( lpPartyPlayers[cnt]->dwTime_PrimeItem_ExpUp>(DWORD)tServerTime ) {
					switch( lpPartyPlayers[cnt]->dwPrimeItem_PackageCode ) 
					{
					case PRIME_ITEM_PACKAGE_NONE:
						ExpUp += 30;
						break;
					case PRIME_ITEM_PACKAGE_BRONZE:
						ExpUp += 10;
						break;
					case PRIME_ITEM_PACKAGE_SILVER:
						ExpUp += 20;
						break;
					case PRIME_ITEM_PACKAGE_GOLD:
						ExpUp += 30;
						break;
					case PRIME_ITEM_PACKAGE_ULTRA:		//��Ʈ����û	50%
						ExpUp += 50;
						break;
					case PRIME_ITEM_PACKAGE_NONE_50_EXPUP: // ����� - ����ġ���� ����(50%) ������ ����
						ExpUp += 50; // ����� - ����ġ 50% ��
						break; 
					case PRIME_ITEM_PACKAGE_NONE_100_EXPUP: // ����� - ����ġ���� ����(100%) ������ ����
						ExpUp += 100; // ����� - ����ġ 100% ��
						break; 
					}
				}
				if ( lpPartyPlayers[cnt]->Bl_RNo>0 )
					ExpUp += 20;

				if( lpPartyPlayers[cnt]->dwTime_PrimeItem_PhenixPet > 0 )	// pluto ��(�ؿ�) ����
				{
					ExpUp += 20;
				}

				//�ð��� ����ġ �й� ( ��Ʈ�� )
				if ( rsServerConfig.ExpGameTimeMode ) {
					smTransCommand.WParam = (smTransCommand.WParam*rsGetExp_GameTime( lpPartyPlayers[cnt] ))/100;
				}

				if ( ExpUp ) {
					//���� ������ ����� �Ǵ� PC�� ( ����ġ 30% + 20% �߰� )
					smTransCommand.WParam += (smTransCommand.WParam*ExpUp)/100;
				}

				lpPartyPlayers[cnt]->spEXP_Out += smTransCommand.WParam;		//���� ����ġ ���
				smTransCommand.SParam = (smTransCommand.WParam*3 + smTransCommand.LParam*13)*2002;	//���� ���� �������� �ڵ�
				smTransCommand.EParam = smTransCommand.SParam^lpPartyPlayers[cnt]->spEXP_Out;

				lpPartyPlayers[cnt]->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
			}
		}
	}

	if ( !Sucess )	return FALSE;

	return TRUE;
}

//��Ƽ�� �� ȹ��
int rsPLAYINFO::GetPartyMoney( sITEMINFO *lpDefItemInfo , rsPLAYINFO *lpPlayInfo )
{
	TRANS_ITEMINFO	TransItemInfo;
	int	PartyMoney[2];
	int	cnt;
	int x,z,dist;
	int Sucess = 0;

	if ( dwPartyInfo!=PARTY_STATE_MASTER || !lpsmSock ) return FALSE;

	PartyMoney[1] = lpDefItemInfo->Money/PartyUserCount;					//��Ƽ�� ȹ��
	PartyMoney[0] = PartyMoney[1]+(lpDefItemInfo->Money%PartyUserCount);	//���� ȹ��


	TransItemInfo.code = smTRANSCODE_PUTITEM;
	TransItemInfo.size = sizeof(TRANS_ITEMINFO);
	memcpy( &TransItemInfo.Item , lpDefItemInfo , sizeof( sITEMINFO ) );
	//lpsmSock->Send( (char *)&TransItemInfo , TransItemInfo.size , TRUE );


	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]->lpsmSock ) {

			if ( lpPartyPlayers[cnt]==lpPlayInfo ) {
				wsprintf( TransItemInfo.Item.ItemName , sinGold , PartyMoney[0] );
				TransItemInfo.Item.Money = PartyMoney[0];
				ReformItem( &TransItemInfo.Item );						//������ ����
				Sucess++;
			}
			else {
				x = (lpPlayInfo->Position.x - lpPartyPlayers[cnt]->Position.x)>>FLOATNS;
				z = (lpPlayInfo->Position.z - lpPartyPlayers[cnt]->Position.z)>>FLOATNS;
				dist = x*x+z*z;
				if ( dist<PARTY_GETTING_DIST ) {			//��Ƽ ���� ���� �Ÿ�
					wsprintf( TransItemInfo.Item.ItemName , sinGold , PartyMoney[1] );
					TransItemInfo.Item.Money = PartyMoney[1];
					ReformItem( &TransItemInfo.Item );						//������ ����
				}
				else {
					TransItemInfo.Item.Money = 0;
				}
			}

			if ( TransItemInfo.Item.Money ) {
				lpPartyPlayers[cnt]->spMoney_Out += TransItemInfo.Item.Money;

				TransItemInfo.x = dwPlayServTime;											//�ص� Ű
				TransItemInfo.y = TransItemInfo.Item.ItemHeader.dwChkSum;					//�ص� Ű2
				TransItemInfo.z = lpPartyPlayers[cnt]->spMoney_Out^(TransItemInfo.x+TransItemInfo.y);	//��Ż ��� ����

				rsRegist_ItemSecCode( lpPartyPlayers[cnt] , &TransItemInfo , 0 );	//������ �����ۼ��� ����ü�� �����ڵ� ���� ( �����Ͻ� ���������� ���� )

				lpPartyPlayers[cnt]->lpsmSock->Send2( (char *)&TransItemInfo , TransItemInfo.size , TRUE );
			}

		}
	}
	if ( !Sucess )	return FALSE;

	return TRUE;
}

//��Ƽ�� ä�ø޼��� ����
int rsPLAYINFO::PartyChatting( rsPLAYINFO *lpPlayInfo )
{
	int sLen,cnt;
	TRANS_CHATMESSAGE	TransChatMessage;
	

	sLen = lstrlen(lpPlayInfo->szChatMessage)+1;
	TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
	TransChatMessage.size = 32+lstrlen(lpPlayInfo->szChatMessage);
	TransChatMessage.dwIP = 4;			//�Ķ��۾� (�Ӹ��� ���� )
	TransChatMessage.dwObjectSerial = lpPlayInfo->dwObjectSerial;
	memcpy( TransChatMessage.szMessage , lpPlayInfo->szChatMessage , sLen );

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] && lpPartyPlayers[cnt]->lpsmSock && lpPartyPlayers[cnt]!=lpPlayInfo) {

			lpPartyPlayers[cnt]->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
		}
	}


	return TRUE;
}


//��Ƽ�� ����
int	rsPLAYINFO::LeavePartyMaster( rsPLAYINFO *lpPlayInfo )
{
	int cnt;
	char szBuff[128];

	if ( dwPartyInfo!=PARTY_STATE_MASTER ) return FALSE;
	if ( lpPlayInfo==this ) return FALSE;

	dwPartyInfo = PARTY_STATE_MEMBER;

	if ( !lpPlayInfo ) {
		//�ڵ� ���� 
		for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
			if ( lpPartyPlayers[cnt] && lpPartyMaster!=lpPartyPlayers[cnt] ) {
				lpPlayInfo = lpPartyPlayers[cnt];
				break;
			}
		}
	}

	if ( !lpPlayInfo ) return FALSE;
	if ( !lpPlayInfo->lpsmSock ) return FALSE;

	//��Ƽ�� ����
	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] ) {

			lpPartyPlayers[cnt]->lpPartyMaster = lpPlayInfo;		//��Ƽ�� ������ ����
			lpPartyPlayers[cnt]->PartyMasterObjectCode = lpPlayInfo->dwObjectSerial;
			if ( lpPartyPlayers[cnt]==lpPlayInfo ) {
				lpPartyPlayers[cnt]->dwPartyInfo = PARTY_STATE_MASTER;
			}
		}
	}

	memcpy( lpPlayInfo->lpPartyPlayers , lpPartyPlayers , sizeof( rsPLAYINFO *) *PARTY_PLAYER_MAX );
	memcpy( lpPlayInfo->dwPartyObjectCode , dwPartyObjectCode , sizeof( DWORD ) *PARTY_PLAYER_MAX );

	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		lpPartyPlayers[cnt]=0;
		dwPartyObjectCode[0] = 0;
	}

	lpPlayInfo->UpdatePartyPlayer();


	//wsprintf( szBuff , "�������� %s������ ����Ǿ����ϴ�" , lpPlayInfo->smCharInfo.szName );
	wsprintf( szBuff , srPartyMsg1 , lpPlayInfo->smCharInfo.szName );
	lpPlayInfo->SendPartyMessage( szBuff , 2 );

	return TRUE;
}

//��Ƽ �ػ�
int	rsPLAYINFO::ReleaseParty()
{
	int cnt;
	smTRANS_COMMAND		TransCommand;

	if ( lpPartyMaster!=this ) return FALSE;

	//SendPartyMessage( "���ᰡ �ػ�Ǿ����ϴ�" , 2 );
	SendPartyMessage( srPartyMsg2 , 2 );

	dwPartyInfo = PARTY_STATE_NONE;

	TransCommand.code = smTRANSCODE_PARTY_RELEASE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	//��Ƽ�� ����
	for(cnt=0;cnt<PARTY_PLAYER_MAX;cnt++) {
		if ( lpPartyPlayers[cnt] ) {

			lpPartyPlayers[cnt]->dwPartyInfo = PARTY_STATE_NONE;
			lpPartyPlayers[cnt]->lpPartyMaster = NULL;

			if ( lpPartyPlayers[cnt]->lpsmSock )
				lpPartyPlayers[cnt]->lpsmSock->Send( (char *)&TransCommand , TransCommand.size , TRUE );

			lpPartyPlayers[cnt] = 0;
		}
	}


	return TRUE;
}


//��Ƽ ����
int	rsPLAYINFO::JoinParty( rsPLAYINFO *lpPlayMaster )
{
	char	szBuff[128];

	if ( dwPartyInfo ) return FALSE;

	if ( lpPlayMaster && lpPlayMaster->lpsmSock && lpPlayMaster->dwPartyInfo==PARTY_STATE_MASTER ) {

		//��Ƽ�� ����
		if ( lpPlayMaster->JoinPartyPlayer( this )==TRUE ) {
			//����
			//wsprintf( szBuff , "%s���� ���ῡ ���� �߽��ϴ�" , smCharInfo.szName );
			wsprintf( szBuff , srPartyMsg3 , smCharInfo.szName );
			SendPartyMessage( szBuff , 2 );
		}
		else {
			//����
			//wsprintf( szBuff , "���ῡ ���� �Ҽ� �����ϴ�" );
			wsprintf( szBuff , srPartyMsg4 );
			SendPartyMessage( szBuff , 2 );
		}
	}

	return TRUE;
}


//��Ƽ Ż��
int	rsPLAYINFO::SecedeParty()
{
	char	szBuff[128];

	rsPLAYINFO	*lpPlayInfo;


	if ( dwPartyInfo && lpPartyMaster && lpPartyMaster->lpsmSock && lpPartyMaster->dwPartyInfo==PARTY_STATE_MASTER ) {
		//��Ƽ���� Ż��

		if ( this==lpPartyMaster ) {
			LeavePartyMaster(0);		//��Ƽ�� ����
		}

		lpPlayInfo = lpPartyMaster;

		if ( lpPlayInfo->PartyUserCount<=2 ) {
			//wsprintf( szBuff , "%s�� ���ῡ�� Ż���߽��ϴ�" , smCharInfo.szName );
			wsprintf( szBuff , srPartyMsg5 , smCharInfo.szName );
			lpPlayInfo->SendPartyMessage( szBuff , 2 );

			//��Ƽ�� ���� ��Ƽ �ػ�
			lpPlayInfo->ReleaseParty();
			return TRUE;
		}

		if ( lpPlayInfo->DeletePartyPlayer( this ) ) {
			//wsprintf( szBuff , "%s�� ���ῡ�� Ż���߽��ϴ�" , smCharInfo.szName );
			wsprintf( szBuff , srPartyMsg5 , smCharInfo.szName );
			lpPlayInfo->SendPartyMessage( szBuff , 2 );

			return TRUE;
		}
	}


	return FALSE;
}

rsPLAYINFO *LastDisPlayer;
rsPLAYINFO *LastDisPartyMaster;
DWORD		LastDisPartyInfo;

//��Ƽ�� ���� ������
int	rsPLAYINFO::DisconnectPartyUser()
{
	char szBuff[128];
	rsPLAYINFO *pMaster;

	if ( !lpPartyMaster || !dwPartyInfo ) return NULL;

	LastDisPlayer = this;
	LastDisPartyMaster = lpPartyMaster;
	LastDisPartyInfo = dwPartyInfo;



	pMaster = lpPartyMaster;

	if ( this==lpPartyMaster ) {
		LeavePartyMaster(0);		//��Ƽ�� ����
	}


	//wsprintf( szBuff , "%s�� ���ῡ�� Ż���߽��ϴ�" , smCharInfo.szName );
	wsprintf( szBuff , srPartyMsg5 , smCharInfo.szName );
	lpPartyMaster->SendPartyMessage( szBuff , 2 );

	if ( lpPartyMaster->PartyUserCount<=2 ) {
		lpPartyMaster->ReleaseParty();		//��Ƽ�� ���� ��Ƽ �ػ�
		return TRUE;
	}

	//��Ƽ���� Ż��
	lpPartyMaster->DeletePartyPlayer( this );


	lpPartyMaster = pMaster;

	return TRUE;
}


//////////////////////// ���� �Ӵ� ���� ///////////////////////
/*
	int	AddServerMoney( int Money , int WhereParam=0 );		//���� �Ӵ� �߰�
	int	SubServerMoney( int Money , int WhereParam=0 );		//���� �Ӵ� ����
	int	SetServerMoney( int Money , int WhereParam=0 );		//���� �Ӵ� ����
*/

/*
#define	WHERE_TRADE				300
#define	WHERE_GIFT_EXPRESS		600
#define	WHERE_AGINGITEM			400
#define	WHERE_CRAFTITEM			420
#define	WHERE_FORECORB			440
#define	WHERE_BUY_ITEM			200
#define	WHERE_BUY_POTION		210
#define	WHERE_SELL_ITEM			220
#define	WHERE_LOAD_PLAYER		100
#define	WHERE_THROW_ITEM		120
#define	WHERE_GET_ITEM			130
#define	WHERE_STAR_POINT		620
#define	WHERE_CLAN_MONEY		630
#define	WHERE_GIVE_MONEY		640
#define	WHERE_PERSONAL_SHOP		320
#define	WHERE_OPEN_WAREHOUES	500
*/

char *GetWhereParamString( int WareParam )
{
	char *lpString;

	lpString = "Unknow";

#ifdef _W_SERVER

	switch(WareParam) {

	case	WHERE_TRADE:
			lpString = "Trade";	break;
	case	WHERE_GIFT_EXPRESS:
			lpString = "GiftExpress";	break;
	case	WHERE_AGINGITEM:	
			lpString = "AgingItem";	break;
	case	WHERE_CRAFTITEM:	
			lpString = "CraftItem";	break;
	case	WHERE_FORECORB:	
			lpString = "ForceOrb";	break;
	case	WHERE_BUY_ITEM:		
			lpString = "BuyItem";	break;
	case	WHERE_BUY_POTION:
			lpString = "BuyPotion";	break;
	case	WHERE_SELL_ITEM:
			lpString = "SellItem";	break;
	case	WHERE_LOAD_PLAYER:
			lpString = "LoadPlayer";	break;
	case	WHERE_THROW_ITEM:
			lpString = "ThrowItem";	break;
	case	WHERE_GET_ITEM:
			lpString = "GetItem";	break;
	case	WHERE_STAR_POINT:
			lpString = "StarPoint";	break;
	case	WHERE_CLAN_MONEY:
			lpString = "ClanMoney";	break;
	case	WHERE_GIVE_MONEY:
			lpString = "GiveMoney";	break;
	case	WHERE_PERSONAL_SHOP:
			lpString = "PersonalShop";	break;
	case	WHERE_OPEN_WAREHOUES:
			lpString = "WareHouse";	break;
	}
#endif

	return lpString;
}


#define	CHK_SERVER_MONEY_MAX	100000000
#define	CHK_SERVER_MONEY_MAX2	150000000
#define	CHK_SERVER_MONEY_MIN	-150000000

int	rsPLAYINFO::AddServerMoney( int Money , int WhereParam )	//���� �Ӵ� �߰�
{
	int money = ServerMoney;

	ServerMoney += Money;

	if ( (money>=0 && ServerMoney<0) || Money<0 || Money>CHK_SERVER_MONEY_MAX ) {
		//�� ���� �÷ο� ( �α� ����� )

		smTRANS_COMMAND_EX	smTransCommandEx;

		char szStrBuff[128];
		wsprintf( szStrBuff , "*ADD_MONEY [%s]*" , GetWhereParamString(WhereParam) );

		smTransCommandEx.WParam = 8840;
		smTransCommandEx.WxParam = WhereParam;
		smTransCommandEx.LxParam = (int)szStrBuff;
		smTransCommandEx.LParam = money;
		smTransCommandEx.SParam = Money;
		smTransCommandEx.EParam = ServerMoney;
		RecordHackLogFile( this , &smTransCommandEx );

		if ( ServerMoney<CHK_SERVER_MONEY_MIN )
			ServerMoney = CHK_SERVER_MONEY_MIN;

		return FALSE;
	}

	return TRUE;
}

int	rsPLAYINFO::SubServerMoney( int Money , int WhereParam )	//���� �Ӵ� ����
{
	int money = ServerMoney;

	ServerMoney -= Money;

	if ( (money>=0 && ServerMoney<0) || Money<0 || Money>CHK_SERVER_MONEY_MAX ) {
		//�� ���� �÷ο� ( �α� ����� )

		smTRANS_COMMAND_EX	smTransCommandEx;

		char szStrBuff[128];
		wsprintf( szStrBuff , "*SUB_MONEY [%s]*" , GetWhereParamString(WhereParam) );

		smTransCommandEx.WParam = 8840;
		smTransCommandEx.WxParam = WhereParam;
		smTransCommandEx.LxParam = (int)szStrBuff;
		smTransCommandEx.LParam = money;
		smTransCommandEx.SParam = Money;
		smTransCommandEx.EParam = ServerMoney;
		RecordHackLogFile( this , &smTransCommandEx );

		if ( ServerMoney<CHK_SERVER_MONEY_MIN )
			ServerMoney = CHK_SERVER_MONEY_MIN;


		return FALSE;
	}

	return TRUE;
}

int	rsPLAYINFO::SetServerMoney( int Money , int WhereParam )	//���� �Ӵ� ����
{
	ServerMoney = Money;

	if ( Money>CHK_SERVER_MONEY_MAX || Money<0 ) {
		//�� ���� �÷ο� ( �α� ����� )

		smTRANS_COMMAND_EX	smTransCommandEx;

		char szStrBuff[128];
		wsprintf( szStrBuff , "*SET_MONEY [%s]*" , GetWhereParamString(WhereParam) );

		smTransCommandEx.WParam = 8840;
		smTransCommandEx.WxParam = WhereParam;
		smTransCommandEx.LxParam = (int)szStrBuff;
		smTransCommandEx.LParam = 0;
		smTransCommandEx.SParam = Money;
		smTransCommandEx.EParam = ServerMoney;
		RecordHackLogFile( this , &smTransCommandEx );

		if ( ServerMoney<CHK_SERVER_MONEY_MIN )
			ServerMoney = CHK_SERVER_MONEY_MIN;

		if ( ServerMoney>CHK_SERVER_MONEY_MAX2 )
			ServerMoney = 0;

		return FALSE;
	}

	return TRUE;
}


int	rsPLAYINFO::CallBack_ClanMarkNum( int ClanMarkNum )			//Ŭ����ũ ��ȣ �˷���
{
	DWORD dwCode;
	smTRANS_COMMAND	smTransCommand;

	if ( ClanMarkNum==-1 ) return TRUE;

	dwCode = GetClanCode( ClanMarkNum );
	if ( dwClanCode!=dwCode ) {

		smTransCommand.WParam = 8870;
		smTransCommand.LParam = dwClanCode;
		smTransCommand.SParam = dwCode;
		smTransCommand.EParam = rsBlessCastle.dwMasterClan;
		RecordHackLogFile( this , &smTransCommand );

		SucessCheck_ClanCode = -1;
		//Ŭ�� ��ũ �ڵ尡 Ʋ����
		return FALSE;
	}

	dwClanCode = dwCode;
	SucessCheck_ClanCode = TRUE;

	return TRUE;
}
