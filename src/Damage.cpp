#include "common.h"

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
#include "damage.h"
#include "areaserver.h"

/*
struct TRANS_SKIL_ATTACKDATA2 {
	int size,code;

	DWORD	dwDestObjectSerial;	//�����ϴ� ĳ���� ������ȣ

	DWORD	dwChkSum;

	int		x,y,z;			// ��ǥ
	int		AttackState;	// �Ӽ�
	int		AttackSize;		// ���� ����
	short	Power[2];		// ���� �Ŀ�
	short	Power2[2];		// ���� �Ŀ� ( �߰� ���� )
	short	Critical[2];	// ũ��Ƽ��
	int		SkillCode;		// ��ų�ڵ�
	DWORD	dwTime;			// Ŭ���̾�Ʈ �ð�
	int		AttackCount;	// ���� ī����
	int		Temp[4];		// ����

	DWROD	dwMainTargetObject;		//�� Ÿ��

	int	TargetCount;
	DWORD	dwTarObjectSerial[SKIL_ATTACK_CHAR_MAX];	//���ݴ�� ĳ���� ������ȣ
};

//�� ���� ��Ŷ
#define smTRANSCODE_ATTACK_DAMAGE		0x50322000
#define smTRANSCODE_RANGE_DAMAGE		0x50322010

struct TRANS_ATTACKDATA2 {
	int size,code;

	DWORD	dwChkSum;

	DWORD	dwDestObjectSerial;	//�����ϴ� ĳ���� ������ȣ
	DWORD	dwTarObjectSerial;	//���ݴ�� ĳ���� ������ȣ

	int		x,y,z;			// ��ǥ
	int		AttackState;	// �Ӽ�
	int		AttackSize;		// ���� ����
	short	Power[2];		// ���� �Ŀ�
	short	Power2[2];		// ���� �Ŀ� ( �߰� ���� )
	short	Critical[2];	// ũ��Ƽ��
	int		SkillCode;		// ��ų�ڵ�
	DWORD	dwTime;			// Ŭ���̾�Ʈ �ð�
	int		AttackCount;	// ���� ī����
	int		Temp[4];		// ����
};

*/

//�������� ������ ����
//int dm_SendRangeDamage( int x, int y, int z, smCHAR *lpMainTraget ,int PowParam1, int PowParam2, int AttackState , int Registance ,  DWORD dwSkillCode )
//int dm_QuickSendTransAttack( smCHAR *lpChar , int power , int AttackState , int Resistance )

LPFN_EncodeDamagePacket	fnEncodeDamagePacket = 0;
LPFN_DecodeDamagePacket	fnDecodeDamagePacket = 0;


TRANS_SKIL_ATTACKDATA2	*lpTransSkill_AttackData = 0;
smCHAR	*lpSelected_Char[SKIL_ATTACK_CHAR_MAX];
int	dmSelected_CharCnt;
int	dmUseAccuracy;
DWORD	dmAttackCount = 0;

//���� ���� ýũ��
DWORD	dm_GetRangeDamgeChkSum( TRANS_SKIL_ATTACKDATA2 *lpTransSkilAttackData2 )
{
	DWORD	dwChkSum;

	dwChkSum = lpTransSkilAttackData2->code;
	dwChkSum += lpTransSkilAttackData2->x^lpTransSkilAttackData2->y^lpTransSkilAttackData2->z;
	dwChkSum += lpTransSkilAttackData2->AttackCount^lpTransSkilAttackData2->dwTime;
	dwChkSum ^= ((DWORD *)lpTransSkilAttackData2->Power)[0];
	dwChkSum ^= ((DWORD *)lpTransSkilAttackData2->Power2)[0];
	dwChkSum ^= ((DWORD *)lpTransSkilAttackData2->Critical)[0];
	dwChkSum ^= ((DWORD *)lpTransSkilAttackData2->MotionCount)[0];
	dwChkSum += lpTransSkilAttackData2->SkillCode;
	dwChkSum ^= 0x65028086;

	return dwChkSum;
}

//���� ���� ýũ��
DWORD	dm_GetDamgeChkSum( TRANS_ATTACKDATA2 *lpTransAttackData2 )
{
	DWORD	dwChkSum;

	dwChkSum = lpTransAttackData2->code;
	dwChkSum += lpTransAttackData2->x^lpTransAttackData2->y^lpTransAttackData2->z;
	dwChkSum += lpTransAttackData2->AttackCount^lpTransAttackData2->dwTime;
	dwChkSum ^= ((DWORD *)lpTransAttackData2->Power)[0];
	dwChkSum ^= ((DWORD *)lpTransAttackData2->Power2)[0];
	dwChkSum ^= ((DWORD *)lpTransAttackData2->Critical)[0];
	dwChkSum ^= ((DWORD *)lpTransAttackData2->MotionCount)[0];
	dwChkSum += lpTransAttackData2->SkillCode;
	dwChkSum ^= 0x68098051;

	return dwChkSum;
}

//�ڸǵ� ýũ��
DWORD	dm_GetCommandChkSum( smTRANS_COMMAND *lpTransCommand , DWORD dwObjectSerial )
{
	DWORD	dwChkSum;

	dwChkSum = lpTransCommand->code&0x037C70FF;
	dwChkSum += dwObjectSerial<<5;
	dwChkSum ^= lpTransCommand->WParam ^ lpTransCommand->SParam;
	dwChkSum ^= lpTransCommand->code<<2;
	dwChkSum ^= lpTransCommand->LParam ^ lpTransCommand->EParam;
	dwChkSum ^= 0x009821CC;

	return dwChkSum;
}

//S2C ���� ýũ��
DWORD	dm_GetDamgeChkSum_S2V( TRANS_ATTACKDATA *lpTransAttackData )
{
	DWORD	dwChkSum;

	dwChkSum = lpTransAttackData->code;
	dwChkSum += lpTransAttackData->x^lpTransAttackData->y^lpTransAttackData->z;
	dwChkSum ^= lpTransAttackData->AttackState+lpTransAttackData->AttackSize;
	dwChkSum ^= lpTransAttackData->Power<<5;
	dwChkSum ^= lpTransAttackData->dwChkSum;
	dwChkSum ^= ((DWORD *)lpTransAttackData->sRating)[0];
	dwChkSum ^= 0x80516809;

	return dwChkSum;
}

//ó�� �����ϴ� ���� �������� ���
int dm_OpenFireAttack( smCHAR *lpChar , DWORD SkillCode )
{

	lpChar->dwLastRecvAttackTime = dwPlayTime;		//�ش� ���͸� ������ �ð�

	if ( lpChar->smCharInfo.State==smCHAR_STATE_ENEMY && lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0]==lpChar->smCharInfo.Life[1] ) {
		lpChar->EnableStateBar = TRUE;
		return TRUE;
	}
	return FALSE;
}

//ó�� �����ϴ� ���� �������� ��� ( ���� )
int dm_OpenFireAttackRange( DWORD dwSkillCode )
{
	DWORD SkillCode;
	int cnt;
	int OpenCount=0;
	smCHAR *lpChar;



	SkillCode = dwSkillCode&0xFF;

	if ( SkillCode==SKILL_PLAY_ROAR ) return FALSE;

	for( cnt=0;cnt<dmSelected_CharCnt;cnt++ ) {
		//ó�� �����ϴ� ���� �������� ���
		lpChar = lpSelected_Char[cnt];
		if ( lpChar ) {
			lpChar->dwLastRecvAttackTime = dwPlayTime;		//�ش� ���͸� ������ �ð�
			if ( lpChar->smCharInfo.State==smCHAR_STATE_ENEMY && lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0]==lpChar->smCharInfo.Life[1] ) {
				lpChar->EnableStateBar = TRUE;
				OpenCount++;

				AddAttMonsterCode( lpChar->smCharInfo.StatePoint );	//���� ���ݸ�� ť������
			}
		}
	}

	return OpenCount;
}


//��� ĳ���Ϳ� ���� ���� 
int dm_SendTransDamage( smCHAR *lpChar , int PowParam1, int PowParam2, int AttackState, int Resistance, DWORD dwSkillCode , int UseAccuracy ,  smWINSOCK *lpsmSock )
{
	TRANS_ATTACKDATA2	TransAttackData;
	smWINSOCK *lpsmSendSock =0;
	int	result;
	int dmg;
	int rnd;

	if ( ( abs( ( long )dwPlayTime - ( long )dwLastMouseMoveTime ) > 1024 * 60 * 3 ||
		abs( ( long )dwPlayTime - ( long )dwLastCharMoveTime ) > 1024 * 60 * 3 ||
		abs( ( long )dwFuncChkTime - ( long )dwPlayTime ) > 14 * 60 * 1024 ) ||
		 RestartPlayCount!=0 ) 
	{
		//Attack invalid when no mouse movement more than a few minutes (bots automatically play defense)
		//Invalid attacks when standing in place for more than a few minutes characters (bots automatically play defense)
		return FALSE;
	}

	dmAttackCount++;

	ZeroMemory( &TransAttackData , sizeof(TRANS_ATTACKDATA2) );

	if ( UseAccuracy ) 
	{
		int	temp , accuracy , pow;

		switch( lpChar->smCharInfo.State ) 
		{
			case smCHAR_STATE_ENEMY:
				//���߷��� �����Ѵ� 
				dwBattleTime = dwPlayTime;						//���� �ð� ���

				temp = lpCurPlayer->smCharInfo.Attack_Rating;
				//��ī��Ʈȣũ ( ������ ���� )
				if ( lpCurPlayer->HoSkillCode ) {
					switch( lpCurPlayer->HoSkillCode ) {
						case SKILL_SCOUT_HAWK:
							pow = GetSkillPower( lpCurPlayer , lpChar , 0,0,0 );
							if ( pow ) {
								lpCurPlayer->smCharInfo.Attack_Rating += (lpCurPlayer->smCharInfo.Attack_Rating*pow)/100;
							}
							break;				
					}
				}

				accuracy = sinGetAccuracy( lpChar->smCharInfo.Level , lpChar->smCharInfo.Defence );
				lpCurPlayer->smCharInfo.Attack_Rating = temp;

				rnd = rand()%100;
#ifdef	_LANGUAGE_KOREAN
				if ( smConfig.DebugMode && VRKeyBuff[VK_SHIFT] ) {
					char szBuff[256];
					wsprintf( szBuff , "Probability: %d/%d" , rnd , accuracy );
					AddChatBuff( szBuff , 0 );
				}
#endif
				if ( rnd>accuracy ) {
					return FALSE;		//����� ����
				} 
				break;

			case smCHAR_STATE_NPC:
				//TransAttackData.AttackState = Resistance;
				if ( MouseItem.Flag ) return FALSE;				//���콺�� ������ ��� ������ NPC ȣ�� ����

				AttackState = Resistance;
				Resistance = 0;
				break;
		}
	}

	AddAttMonsterCode( lpChar->smCharInfo.StatePoint );	//���� ���ݸ�� ť������

	TransAttackData.code = smTRANSCODE_ATTACK_DAMAGE;
	TransAttackData.size = sizeof(TRANS_ATTACKDATA2);

	TransAttackData.x = lpChar->pX;
	TransAttackData.y = lpChar->pY;
	TransAttackData.z = lpChar->pZ;

	TransAttackData.Power[0] = lpCurPlayer->smCharInfo.Attack_Damage[0];
	TransAttackData.Power[1] = lpCurPlayer->smCharInfo.Attack_Damage[1];

	if ( PowParam1 || PowParam2 ) 
	{
		TransAttackData.Power2[0] = PowParam1;
		TransAttackData.Power2[1] = PowParam2;
	}
	else 
	{
		if ( !dwSkillCode ) 
		{
			//���� �������� ���� ����
			TransAttackData.Power2[0] = TransAttackData.Power[0]-cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0];
			TransAttackData.Power2[1] = TransAttackData.Power[1]-cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1];
		}
	}

	if ( ((lpCurPlayer->dwActionItemCode^sinWM1)&sinITEM_MASK2)!=0 )		//������ ũ��Ƽ�� ����
		TransAttackData.Critical[0] = lpCurPlayer->smCharInfo.Critical_Hit;

	switch( lpCurPlayer->smCharInfo.JOB_CODE )
	{
		case JOBCODE_PRIESTESS:
		case JOBCODE_MAGICIAN:
			TransAttackData.Critical[1] = lpCurPlayer->smCharInfo.Spirit;			//���ŷ� ����
			break;

		case JOBCODE_MECHANICIAN:
		case JOBCODE_FIGHTER:
		case JOBCODE_PIKEMAN:
		case JOBCODE_KNIGHT:
			TransAttackData.Critical[1] = lpCurPlayer->smCharInfo.Strength;			//�� ����
			break;

		case JOBCODE_ARCHER:
		case JOBCODE_ATALANTA:
			TransAttackData.Critical[1] = lpCurPlayer->smCharInfo.Dexterity;		//��ø ����
			break;
		//DRZ_NEWCLASS (add defintion for new classes)
	}

	TransAttackData.AttackState = AttackState;
	TransAttackData.AttackSize  = 48*fONE;

	TransAttackData.MotionCount[0] = lpCurPlayer->MotionEvent;
	TransAttackData.AttackCount = dmAttackCount;
	TransAttackData.dwTime = dwPlayTime;
	TransAttackData.SkillCode = dwSkillCode;

	TransAttackData.dwWeaponCode = lpCurPlayer->dwActionItemCode;

	if ( lpCurPlayer->OnStageField>=0 && lpChar->smCharInfo.State==smCHAR_STATE_ENEMY ) 
		TransAttackData.Area[0] = StageField[ lpCurPlayer->OnStageField ]->FieldCode;
	else
		TransAttackData.Area[0] = -1;

	TransAttackData.dwDestObjectSerial = lpCurPlayer->dwObjectSerial;
	TransAttackData.dwTarObjectSerial = lpChar->dwObjectSerial;
	lpChar->TryAttackCount++;


	TransAttackData.dwChkSum = dm_GetDamgeChkSum(&TransAttackData);


	CheckMaxDamage();		//ó�� �ѹ��� ����ǰ� ������

	if ( lpChar!=lpCurPlayer )
		lpsmSendSock = GetServerSock(lpChar->ServerCode);		//���� �ڵ� ���ϱ�
	else 
		lpsmSendSock = smWsockUserServer;

	if ( lpsmSock ) lpsmSendSock=lpsmSock;

	Utils_Log(LOG_DEBUG, "SEND ATTACK DATA");

	#ifdef _USE_DYNAMIC_ENCODE
		if ( lpsmSendSock && fnEncodeDamagePacket && fnDecodeDamagePacket )
		{
			dmg = GetRandomPos( TransAttackData.Power[0] , TransAttackData.Power[1] );

			fnEncodeDamagePacket( &TransAttackData );			//��ȣȭ

			result = lpsmSendSock->Send2( (char *)&TransAttackData , TransAttackData.size , TRUE );
			if ( result )
			{
				dm_OpenFireAttack( lpChar , dwSkillCode );		//ó�� �����ϴ� ���� �������� ���
				return dmg;
			}
		}
	#else
		Utils_Log(LOG_DEBUG, "SEND ATTACK DATA - static encode");
		if ( lpsmSendSock )
		{
			dmg = GetRandomPos(TransAttackData.Power[0], TransAttackData.Power[1]);

			dm_EncodePacket(&TransAttackData);				//��ȣȭ

			result = lpsmSendSock->Send2((char *)&TransAttackData, TransAttackData.size, TRUE);
			if (result)
			{
				dm_OpenFireAttack(lpChar, dwSkillCode);		//ó�� �����ϴ� ���� �������� ���
				return dmg;
			}
		}
	#endif

	return NULL;
}


//��� ĳ���Ϳ� ���� ���� 
int dm_SendTransDamage( smCHAR *lpChar , int PowParam1, int PowParam2, int AttackState, int Resistance, DWORD dwSkillCode , int UseAccuracy )
{

	return dm_SendTransDamage( lpChar , PowParam1, PowParam2, AttackState, Resistance, dwSkillCode , UseAccuracy , 0 );
}

int dm_OpenTransRangeDamage()
{
	if ( !lpTransSkill_AttackData ) lpTransSkill_AttackData = new TRANS_SKIL_ATTACKDATA2;

	ZeroMemory( lpTransSkill_AttackData , sizeof(TRANS_SKIL_ATTACKDATA2) );
	dmSelected_CharCnt = 0;

	if ( lpTransSkill_AttackData ) return TRUE;

	return FALSE;
}

int dm_SendTransRangeDamage()
{
	smWINSOCK *lpsmSendSock;
	int result;

	if ( RestartPlayCount!=0 ) return FALSE;
	if ( !lpTransSkill_AttackData ) return FALSE;

	CheckMaxDamage();		//ó�� �ѹ��� ����ǰ� ������

	lpsmSendSock = GetAreaServerSock();		//���� �������ϱ�

#ifdef _USE_DYNAMIC_ENCODE
	if ( lpsmSendSock && lpTransSkill_AttackData->TargetCount>0 && fnEncodeDamagePacket && fnDecodeDamagePacket )
	{
		//���� ���ݷ� ���
		//Record_SendDamage( lpsmSendSock , lpTransSkill_AttackData->TargetCount );

		fnEncodeDamagePacket( lpTransSkill_AttackData );			//��ȣȭ

		result = lpsmSendSock->Send2( (char *)lpTransSkill_AttackData , lpTransSkill_AttackData->size , TRUE );

		//ó�� �����ϴ� ���� �������� ��� ( ���� )
		dm_OpenFireAttackRange(lpTransSkill_AttackData->SkillCode);
	}
#else
	if (lpsmSendSock && lpTransSkill_AttackData->TargetCount > 0)
	{
		//���� ���ݷ� ���
		//Record_SendDamage( lpsmSendSock , lpTransSkill_AttackData->TargetCount );

		dm_EncodePacket(lpTransSkill_AttackData);			//��ȣȭ

		result = lpsmSendSock->Send2((char *)lpTransSkill_AttackData, lpTransSkill_AttackData->size, TRUE);

		//ó�� �����ϴ� ���� �������� ��� ( ���� )
		dm_OpenFireAttackRange(lpTransSkill_AttackData->SkillCode);
	}
#endif

	ZeroMemory( lpTransSkill_AttackData , sizeof(TRANS_SKIL_ATTACKDATA2) );
	delete	lpTransSkill_AttackData;
	lpTransSkill_AttackData = 0;

	return TRUE;
}



//������ ���� ���� ���� (��)
int dm_SelectRange( int x, int y, int z , int range , int UseAttackRating , int SelCount )
{

	int cnt;
	int	dRange;
	int dx,dy,dz,dist;
	smCHAR	*lpChar;
	int Attack;
	int accuracy;
	int ChkCount = 0;

	dm_OpenTransRangeDamage();
	dmSelected_CharCnt = 0;
	dmUseAccuracy = UseAttackRating;

	dRange = range*range;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
		if ( lpTransSkill_AttackData->TargetCount>=SKIL_ATTACK_CHAR_MAX ) break;
		if ( chrOtherPlayer[cnt].Flag && 
			chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_ENEMY && 
			chrOtherPlayer[cnt].smCharInfo.Brood!=smCHAR_MONSTER_USER && 
			chrOtherPlayer[cnt].smCharInfo.Life[0]>0 ) 
		{
			lpChar = &chrOtherPlayer[cnt];

			dx = (x-lpChar->pX)>>FLOATNS;
			dy = (y-lpChar->pY)>>FLOATNS;
			dz = (z-lpChar->pZ)>>FLOATNS;
			dist = dx*dx+dy*dy+dz*dz;

			if ( dist<dRange && abs(dy)<65) {
				if ( !SelCount || SelCount>ChkCount ) {

					Attack = TRUE;

					if ( UseAttackRating ) {
						accuracy = sinGetAccuracy( chrOtherPlayer[cnt].smCharInfo.Level , chrOtherPlayer[cnt].smCharInfo.Defence );
						if ( (rand()%100)>accuracy ) 
							Attack = FALSE;
					}

					if ( Attack ) {	
						lpTransSkill_AttackData->dwTarObjectSerial[lpTransSkill_AttackData->TargetCount++] = chrOtherPlayer[cnt].dwObjectSerial;
						lpSelected_Char[dmSelected_CharCnt++] = &chrOtherPlayer[cnt];
					}
				}

				ChkCount++;
			}
		}
	}

	return dmSelected_CharCnt;
}

//������ ���� ���� ���� (��)
int dm_SelectRange( int x, int y, int z , int range , int UseAttackRating )
{
	return dm_SelectRange( x,y,z, range, UseAttackRating , 0 );

}

//������ ���� ���� ���� (�ڽ�)
int dm_SelectRangeBox( smCHAR *lpCurChar , RECT *lpRect , int UseAttackRating )
{

	int cnt;
	int dx,dy,dz;
	int Attack,accuracy;
	smCHAR	*lpChar;

	dm_OpenTransRangeDamage();
	dmSelected_CharCnt = 0;
	dmUseAccuracy = UseAttackRating;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
		if ( lpTransSkill_AttackData->TargetCount>=SKIL_ATTACK_CHAR_MAX ) break;
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_ENEMY && chrOtherPlayer[cnt].smCharInfo.Life[0]>0
			&& chrOtherPlayer[cnt].smCharInfo.Brood!=smCHAR_MONSTER_USER) { // �庰 - ������ ���ݿ� ũ����Ż ���ʹ� ���� �ʰ� �Ѵ�
			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX-lpCurChar->pX)>>FLOATNS;
			dy = (lpChar->pY-lpCurChar->pY)>>FLOATNS;
			dz = (lpChar->pZ-lpCurChar->pZ)>>FLOATNS;

			// �̵��� ���� ��ǥ���� ���ϴ� ���̱���! �����.
			GetMoveLocation( dx, dy, dz, 0, (-lpCurChar->Angle.y)&ANGCLIP , 0 );

			if ( GeoResult_X>lpRect->left && GeoResult_X<lpRect->right  &&
				GeoResult_Z>lpRect->top && GeoResult_Z<lpRect->bottom && abs(dy)<256 ) {

				Attack = TRUE;

				if ( UseAttackRating ) {
					accuracy = sinGetAccuracy( chrOtherPlayer[cnt].smCharInfo.Level , chrOtherPlayer[cnt].smCharInfo.Defence );
					if ( (rand()%100)>accuracy ) 
						Attack = FALSE;
				}

				if ( Attack ) {
					lpTransSkill_AttackData->dwTarObjectSerial[lpTransSkill_AttackData->TargetCount++] = chrOtherPlayer[cnt].dwObjectSerial;
					lpSelected_Char[dmSelected_CharCnt++] = &chrOtherPlayer[cnt];
				}
			}

		}
	}

	return dmSelected_CharCnt;
}

//������ ���� ���� ���� (����� ����Ʈ��)
int dm_SelectDamageCount( smCHAR *lpCurChar , int SelCount , int Size )
{

	int cnt;
	int dx,dy,dz,dist;
	int cCount;
	smCHAR	*lpChar;
	int dSize;

	dm_OpenTransRangeDamage();
	dmSelected_CharCnt = 0;
	dmUseAccuracy = 0;

	if ( lpCurChar!=lpCurPlayer ) return FALSE;

	dSize = Size*Size;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {

		cCount = (DivineLightning_FindCount+cnt)&OTHER_PLAYER_MASK;

		if ( chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial && 
			lpCurChar!=&chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State==smCHAR_STATE_ENEMY &&
			chrOtherPlayer[cCount].smCharInfo.Brood!=smCHAR_MONSTER_USER &&
			chrOtherPlayer[cCount].smCharInfo.Life[0]>0 ) {

				lpChar = &chrOtherPlayer[cCount];

				dx = (lpChar->pX-lpCurChar->pX)>>FLOATNS;
				dy = (lpChar->pY-lpCurChar->pY)>>FLOATNS;
				dz = (lpChar->pZ-lpCurChar->pZ)>>FLOATNS;

				dist = dx*dx+dy*dy+dz*dz;

				if ( dist<dSize && abs(dy)<65) {
					lpTransSkill_AttackData->dwTarObjectSerial[lpTransSkill_AttackData->TargetCount++] = lpChar->dwObjectSerial;

					lpSelected_Char[dmSelected_CharCnt] = lpChar;
					dwSkill_DivineLightning_Target[dmSelected_CharCnt] = lpChar->dwObjectSerial;
					dmSelected_CharCnt++;

					if ( dmSelected_CharCnt>=SelCount ) {
						DivineLightning_FindCount = cCount;
						break;
					}

				}
			}
	}
	DivineLightning_Target_Count = dmSelected_CharCnt;

	return dmSelected_CharCnt;
}

//������ ���� ���� ���� (ü�ζ���Ʈ��)
int dm_SelectDamageCount( smCHAR *lpCurChar , smCHAR *lpTragetChar, int SelCount , int Size )
{

	int cnt;
	int dx,dy,dz,dist;
	int cCount;
	smCHAR	*lpChar;
	int dSize;

	dm_OpenTransRangeDamage();
	dmSelected_CharCnt = 0;
	dmUseAccuracy = 0;

	if ( lpCurChar!=lpCurPlayer ) return FALSE;

	dSize = Size*Size;

	lpSelected_Char[dmSelected_CharCnt] = lpTragetChar;
	dwSkill_DivineLightning_Target[dmSelected_CharCnt] = lpTragetChar->dwObjectSerial;
	lpTransSkill_AttackData->dwTarObjectSerial[lpTransSkill_AttackData->TargetCount++] = lpTragetChar->dwObjectSerial;
	dmSelected_CharCnt++;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {

		cCount = (DivineLightning_FindCount+cnt)&OTHER_PLAYER_MASK;

		if ( chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial && 
			lpTragetChar!=&chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State==smCHAR_STATE_ENEMY &&
			chrOtherPlayer[cCount].smCharInfo.Brood!=smCHAR_MONSTER_USER &&
			chrOtherPlayer[cCount].smCharInfo.Life[0]>0 ) {

				lpChar = &chrOtherPlayer[cCount];

				dx = (lpChar->pX-lpTragetChar->pX)>>FLOATNS;
				dy = (lpChar->pY-lpTragetChar->pY)>>FLOATNS;
				dz = (lpChar->pZ-lpTragetChar->pZ)>>FLOATNS;

				dist = dx*dx+dy*dy+dz*dz;

				if ( dist<dSize && abs(dy)<70 ) {
					lpTransSkill_AttackData->dwTarObjectSerial[lpTransSkill_AttackData->TargetCount++] = lpChar->dwObjectSerial;

					lpSelected_Char[dmSelected_CharCnt] = &chrOtherPlayer[cCount];
					dwSkill_DivineLightning_Target[dmSelected_CharCnt] = lpChar->dwObjectSerial;
					dmSelected_CharCnt++;

					if ( dmSelected_CharCnt>=SelCount ) {
						DivineLightning_FindCount = cCount;
						break;
					}

				}
			}
	}
	DivineLightning_Target_Count = dmSelected_CharCnt;

	return dmSelected_CharCnt;
}


//������ ���� ���� ���� (ü�ζ���Ʈ��)
int dm_SelectDamageChainCount( smCHAR *lpCurChar , smCHAR *lpTragetChar, int SelCount , int Size )
{

	int cnt,cnt2,cnt3;
	int dx,dy,dz,dist;
	int cCount;
	smCHAR	*lpChar;
	smCHAR	*lpLinkChar;
	int dSize;
	int minDist;
	smCHAR *lpMinChar;

	dm_OpenTransRangeDamage();
	dmSelected_CharCnt = 0;
	dmUseAccuracy = 0;

	//if ( lpCurChar!=lpCurPlayer ) return FALSE;

	dSize = Size*Size;

	lpSelected_Char[dmSelected_CharCnt] = lpTragetChar;
	dwSkill_DivineLightning_Target[dmSelected_CharCnt] = lpTragetChar->dwObjectSerial;
	lpTransSkill_AttackData->dwTarObjectSerial[lpTransSkill_AttackData->TargetCount++] = lpTragetChar->dwObjectSerial;
	dmSelected_CharCnt++;

	lpLinkChar = lpTragetChar;

	for( cnt2=1;cnt2<SelCount;cnt2++) {
		minDist = dSize;
		lpMinChar = 0;

		for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {

			cCount = cnt;

			if ( chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial && 
				lpLinkChar!=&chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State==smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cCount].smCharInfo.Brood!=smCHAR_MONSTER_USER &&
				chrOtherPlayer[cCount].smCharInfo.Life[0]>0 &&
				lpCurChar!=&chrOtherPlayer[cCount] ) {

					lpChar = &chrOtherPlayer[cCount];

					for(cnt3=0;cnt3<dmSelected_CharCnt;cnt3++) {
						if ( lpSelected_Char[cnt3]==lpChar ) { 
							lpChar = 0;
							break;
						}
					}

					if ( lpChar ) {
						dx = (lpChar->pX-lpLinkChar->pX)>>FLOATNS;
						dy = (lpChar->pY-lpLinkChar->pY)>>FLOATNS;
						dz = (lpChar->pZ-lpLinkChar->pZ)>>FLOATNS;

						dist = dx*dx+dz*dz;

						if ( dist<minDist && abs(dy)<70 ) {
							lpMinChar = lpChar;
							minDist = dist;
						}
					}
				}
		}
		if ( lpMinChar ) {

			lpTransSkill_AttackData->dwTarObjectSerial[lpTransSkill_AttackData->TargetCount++] = lpMinChar->dwObjectSerial;

			lpSelected_Char[dmSelected_CharCnt] = lpMinChar;
			dwSkill_DivineLightning_Target[dmSelected_CharCnt] = lpMinChar->dwObjectSerial;
			dmSelected_CharCnt++;

			lpLinkChar = lpMinChar;
		}
		//if ( cnt>=OTHER_PLAYER_MAX ) break;
	}

	DivineLightning_Target_Count = dmSelected_CharCnt;

	return dmSelected_CharCnt;
}

//���õ� �������� ��Ͽ� ĳ�� ���� �߰�
int dm_AddRangeDamage( smCHAR *lpChar )
{
	int cnt;

	if ( lpTransSkill_AttackData->TargetCount>=SKIL_ATTACK_CHAR_MAX || !lpChar ) return FALSE;

	for(cnt=0;cnt<lpTransSkill_AttackData->TargetCount;cnt++) {
		if ( lpTransSkill_AttackData->dwTarObjectSerial[cnt]==lpChar->dwObjectSerial ) 
			return dmSelected_CharCnt;		//�̹� �߰��� ����
	}

	//��Ͽ� ���� ��� �߰�
	lpTransSkill_AttackData->dwTarObjectSerial[lpTransSkill_AttackData->TargetCount++] = lpChar->dwObjectSerial;
	lpSelected_Char[dmSelected_CharCnt++] = lpChar;

	return dmSelected_CharCnt;
}


//dm_SelectRange

//�������� ������ ����
int dm_SendRangeDamage( int x, int y, int z, smCHAR *lpMainTraget ,int PowParam1, int PowParam2, int AttackState , int Registance ,  DWORD dwSkillCode )
{
	int cnt;

	if ( !lpTransSkill_AttackData ) return FALSE;
	if ( !lpTransSkill_AttackData->TargetCount && !lpMainTraget ) return FALSE;

	dmAttackCount++;

	lpTransSkill_AttackData->code = smTRANSCODE_RANGE_DAMAGE;
	lpTransSkill_AttackData->size = (sizeof(TRANS_SKIL_ATTACKDATA2)-sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	lpTransSkill_AttackData->size += sizeof(DWORD)*lpTransSkill_AttackData->TargetCount+16;

	lpTransSkill_AttackData->x = x;
	lpTransSkill_AttackData->y = y;
	lpTransSkill_AttackData->z = z;
	lpTransSkill_AttackData->AttackCount = dmAttackCount;
	lpTransSkill_AttackData->dwTime = dwPlayTime;
	lpTransSkill_AttackData->AttackState = AttackState;
	if ( lpMainTraget ) {
		if ( dmUseAccuracy ) {
			for(cnt=0;cnt<lpTransSkill_AttackData->TargetCount;cnt++) {
				if ( lpTransSkill_AttackData->dwTarObjectSerial[cnt]==lpMainTraget->dwObjectSerial ) {
					lpTransSkill_AttackData->dwMainTargetObject = lpMainTraget->dwObjectSerial;
					break;
				}
			}
		}
		else
			lpTransSkill_AttackData->dwMainTargetObject = lpMainTraget->dwObjectSerial;

		lpMainTraget->TryAttackCount++;
	}
	lpTransSkill_AttackData->SkillCode = dwSkillCode;
	lpTransSkill_AttackData->Critical[0] = lpCurPlayer->smCharInfo.Critical_Hit;
	lpTransSkill_AttackData->dwDestObjectSerial = lpCurPlayer->dwObjectSerial;
	lpTransSkill_AttackData->Power[0] = lpCurPlayer->smCharInfo.Attack_Damage[0];
	lpTransSkill_AttackData->Power[1] = lpCurPlayer->smCharInfo.Attack_Damage[1];
	lpTransSkill_AttackData->Power2[0] = PowParam1;
	lpTransSkill_AttackData->Power2[1] = PowParam2;

	switch( lpCurPlayer->smCharInfo.JOB_CODE ) 
	{
		case JOBCODE_PRIESTESS:
		case JOBCODE_MAGICIAN:
			lpTransSkill_AttackData->Critical[1] = lpCurPlayer->smCharInfo.Spirit;			//���ŷ� ����
			break;

		case JOBCODE_MECHANICIAN:
		case JOBCODE_FIGHTER:
		case JOBCODE_PIKEMAN:
		case JOBCODE_KNIGHT:
			lpTransSkill_AttackData->Critical[1] = lpCurPlayer->smCharInfo.Strength;			//�� ����
			break;

		case JOBCODE_ARCHER:
		case JOBCODE_ATALANTA:
			lpTransSkill_AttackData->Critical[1] = lpCurPlayer->smCharInfo.Dexterity;		//��ø ����
			break;
		//DRZ_NEWCLASS (add defintion for new classes)
	}

	lpTransSkill_AttackData->AttackSize = 240;

	lpTransSkill_AttackData->dwWeaponCode = lpCurPlayer->dwActionItemCode;

	if ( lpCurPlayer->OnStageField>=0 )
		lpTransSkill_AttackData->Area[0] = StageField[ lpCurPlayer->OnStageField ]->FieldCode;

	lpTransSkill_AttackData->dwChkSum = dm_GetRangeDamgeChkSum(lpTransSkill_AttackData);

	return dm_SendTransRangeDamage();
}



//							N M C F  S X
WORD	wLimitDamage[8] = { 0,0,0,0, 0,0,0,0 };
//							0 1 2 3  4 5

//������ ��ȣȭ ��� ����
int RecvDamagePacketModule( TRANS_FUNC_MEMORY *lpTransFuncModule )
{
	BYTE *lpBuff;

	lpBuff = new BYTE[lpTransFuncModule->size];
	memcpy( lpBuff , lpTransFuncModule->szData , lpTransFuncModule->Param[0] );

	fnEncodeDamagePacket = (LPFN_EncodeDamagePacket)lpBuff;

//	if ( (wLimitDamage[3]^wLimitDamage[5])==0 )
//		SendMaxDamageToServer( 0,0,0 );

	ZeroMemory( wLimitDamage , sizeof(WORD)*8 );

	return TRUE;
}

//������ ��ȣȭ ��� ����
int RecvDamagePacketModule2( TRANS_FUNC_MEMORY *lpTransFuncModule )
{
	BYTE *lpBuff;

	if ( !fnDecodeDamagePacket )
		lpBuff = new BYTE[lpTransFuncModule->size];
	else
		lpBuff = (BYTE *)fnDecodeDamagePacket;

	memcpy( lpBuff , lpTransFuncModule->szData , lpTransFuncModule->Param[0] );
	fnDecodeDamagePacket = (LPFN_DecodeDamagePacket)lpBuff;

	return TRUE;
}

//��Ŷ ���� ��ȣȭ ��� ����
int RecvDynPacketModule( TRANS_FUNC_MEMORY *lpTransFuncModule )
{
	char *lpBuff;

	if ( !fnEncodePacket )
		lpBuff = new char[lpTransFuncModule->size];
	else
		lpBuff = (char *)fnEncodePacket;

	memcpy( lpBuff , lpTransFuncModule->szData , lpTransFuncModule->Param[0] );
	fnEncodePacket = (LPFN_EncodePacket)(lpBuff+lpTransFuncModule->Param[1]);
	fnDecodePacket = (LPFN_DecodePacket)(lpBuff+lpTransFuncModule->Param[2]);

	return TRUE;
}


int CheckMaxDamage()
{
	if ( (wLimitDamage[3]^wLimitDamage[5])==0 && lpCurPlayer->smCharInfo.JOB_CODE>0 )
		cInvenTory.CheckDamage();

	return TRUE;
}

int SetMaxDamage( smCHAR_INFO *lpCharInfo )
{
	WORD	wDamage[8];
	WORD	wSum;

	wDamage[0] = wLimitDamage[0] ^ wLimitDamage[5];
	wDamage[1] = wLimitDamage[1] ^ wLimitDamage[5];
	wDamage[2] = wLimitDamage[2] ^ wLimitDamage[5];
	wDamage[3] = wLimitDamage[3] ^ wLimitDamage[5];
	wDamage[4] = wLimitDamage[4] ^ wLimitDamage[5];

	wSum = wDamage[0]*3+wDamage[1]*5+wDamage[2]*7+wDamage[3]*9;

	if ( wSum!=wDamage[4] || lpCurPlayer->smCharInfo.JOB_CODE==0 ) return FALSE;

	lpCharInfo->Attack_Damage[0] = wDamage[0];
	lpCharInfo->Attack_Damage[1] = wDamage[1];
	lpCharInfo->Critical_Hit = wDamage[2];

	return TRUE;
}

int	SendMaxDamageToServer( WORD MinDamage , WORD MaxDamage , WORD Critical )
{
	WORD	wTime = (WORD)dwPlayTime;

	WORD	wDamage[8];
	WORD	wSum;
	smTRANS_COMMAND	smTransCommand;

	wDamage[0] = wLimitDamage[0] ^ wLimitDamage[5];
	wDamage[1] = wLimitDamage[1] ^ wLimitDamage[5];
	wDamage[2] = wLimitDamage[2] ^ wLimitDamage[5];
	wDamage[3] = wLimitDamage[3] ^ wLimitDamage[5];
	wDamage[4] = wLimitDamage[4] ^ wLimitDamage[5];

	wSum = wDamage[0]*3+wDamage[1]*5+wDamage[2]*7+wDamage[3]*9;

	if ( wSum!=wDamage[4] || lpCurPlayer->smCharInfo.JOB_CODE==0 ) 
		return FALSE;
/*
	if ( wDamage[3]==0 && !MinDamage && !MaxDamage && !Critical ) {
		MinDamage = wDamage[0];
		MaxDamage = wDamage[1];
		Critical = wDamage[2];

		wDamage[0]=0;
		wDamage[1]=0;
		wDamage[2]=0;
	}
*/
	if ( wDamage[0]<MinDamage || wDamage[1]<MaxDamage || wDamage[2]<Critical || wDamage[3]==0 )
	{
		//�ű� ����
		smTransCommand.code = smTRANSCODE_LIMIT_DAMAGE;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = 0;
		smTransCommand.LParam = MinDamage;
		smTransCommand.SParam = MaxDamage;
		smTransCommand.EParam = Critical;
		smTransCommand.WParam = dm_GetCommandChkSum( &smTransCommand , lpCurPlayer->dwObjectSerial );

#ifdef _USE_DYNAMIC_ENCODE
		if ( fnEncodeDamagePacket )
		{
			fnEncodeDamagePacket( &smTransCommand );			//��ȣȭ

			if ( smWsockServer ) 
				smWsockServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
			if ( smWsockUserServer && smWsockServer!=smWsockUserServer ) 
				smWsockUserServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
			if ( smWsockExtendServer && smWsockServer!=smWsockExtendServer ) 
				smWsockExtendServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

			if ( lpWSockServer_Area[0] )
				lpWSockServer_Area[0]->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
			if ( lpWSockServer_Area[1] && lpWSockServer_Area[0]!=lpWSockServer_Area[1] )
				lpWSockServer_Area[1]->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

			wLimitDamage[3] = wDamage[3]+1;
		}
#else
		dm_EncodePacket(&smTransCommand);			//��ȣȭ

		if (smWsockServer)
			smWsockServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		if (smWsockUserServer && smWsockServer != smWsockUserServer)
			smWsockUserServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
			smWsockExtendServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);

		if (lpWSockServer_Area[0])
			lpWSockServer_Area[0]->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		if (lpWSockServer_Area[1] && lpWSockServer_Area[0] != lpWSockServer_Area[1])
			lpWSockServer_Area[1]->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);

		wLimitDamage[3] = wDamage[3] + 1;
#endif
		wLimitDamage[0] = MinDamage;
		wLimitDamage[1] = MaxDamage;
		wLimitDamage[2] = Critical;
		wLimitDamage[4] = wLimitDamage[0]*3+wLimitDamage[1]*5+wLimitDamage[2]*7+wLimitDamage[3]*9;
		wLimitDamage[5] = wTime;

		wLimitDamage[0] ^= wLimitDamage[5];
		wLimitDamage[1] ^= wLimitDamage[5];
		wLimitDamage[2] ^= wLimitDamage[5];
		wLimitDamage[3] ^= wLimitDamage[5];
		wLimitDamage[4] ^= wLimitDamage[5];
	}

	return TRUE;
}

DWORD dwSpeedProtectCode = 0;
DWORD dwSkillProtectCode = 0;

DWORD GetSpeedProtectCode( smCHAR *lpChar )
{
	DWORD	dwCode;

	dwCode = lpChar->frame;
	dwCode += lpChar->FrameStep<<2;
	dwCode += lpChar->MoveSpeed<<5;
	dwCode += lpChar->AttackSpeed<<7;
	dwCode = ( dwCode&0xFFFFEFFF );

	if ( (dwSpeedProtectCode&0x1000)==0 ) return dwCode;

	return NULL;
}

DWORD GetSkillProtectCode( smCHAR *lpChar )
{
	DWORD	dwCode;

	dwCode = lpChar->AttackSkil;
	dwCode += lpChar->frame<<4;
	dwCode = ( dwCode&0xFFFFEFFF );

	if ( (dwSkillProtectCode&0x1000)==0 ) return dwCode;

	return NULL;
}


//�ӵ� ���� Lock
int	LockSpeedProtect( smCHAR *lpChar )
{
	DWORD	dwCode;

	dwCode = GetSpeedProtectCode( lpChar );
	if ( dwCode )
		dwSpeedProtectCode = dwCode;

	dwCode = GetSkillProtectCode( lpChar );
	if ( dwCode )
		dwSkillProtectCode = dwCode;


	return TRUE;
}

//�ӵ� ���� UnLock
int UnlockSpeedProtect( smCHAR *lpChar )
{
	DWORD dwCode;

	smTRANS_COMMAND	smTransCommand;
	ZeroMemory( &smTransCommand , sizeof(smTRANS_COMMAND) );


	dwCode = GetSpeedProtectCode( lpChar );

	if ( dwCode==NULL ) {
		dwSpeedProtectCode++;
		return TRUE;
	}

	if ( dwSpeedProtectCode!=dwCode ) {
		//���ǵ� �ڵ� ����
		smTransCommand.code = TRUE;
		smTransCommand.WParam = 100;
		smTransCommand.LParam = lpChar->FrameStep;
		smTransCommand.SParam = 0;

		dwSpeedProtectCode|=0x1000;
	}

	dwCode = GetSkillProtectCode( lpChar );

	if ( dwCode==NULL ) {
		dwSkillProtectCode++;
		return TRUE;
	}

	if ( dwSkillProtectCode!=dwCode )
	{
		//��ų �ڵ� ����
		smTransCommand.code = TRUE;
		smTransCommand.WParam = 101;
		smTransCommand.LParam = lpChar->AttackSkil;
		smTransCommand.SParam = 0;

		dwSkillProtectCode|=0x1000;
	}

	if ( smTransCommand.code==TRUE ) 
	{
		smTransCommand.code = smTRANSCODE_WARNING_CLIENT;
		smTransCommand.EParam = 0;
		smTransCommand.EParam = dm_GetCommandChkSum( &smTransCommand , lpCurPlayer->dwObjectSerial );
		smTransCommand.size = sizeof(smTRANS_COMMAND);

		#ifdef _USE_DYNAMIC_ENCODE
			if ( fnEncodeDamagePacket ) 
			{
				fnEncodeDamagePacket( &smTransCommand );			//��ȣȭ
				if ( smWsockDataServer )
					smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
			}
		#else
			dm_EncodePacket(&smTransCommand);			//��ȣȭ
			if (smWsockDataServer)
				smWsockDataServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		#endif
	}

	return TRUE;
}


int	dm_EncodePacket( void *lpPacket )
{
	DWORD size,code;
	DWORD cnt,cnt2,cnt3;
	DWORD dwCode;
	DWORD *lpDword = (DWORD *)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /=4;


	dwCode = (code^(code<<7)) + size*6604;
	cnt2 = (code+lpCurPlayer->dwObjectSerial)&0xFFCF;
	cnt3 = cnt2&0x3C;

	for( cnt=2;cnt<size;cnt++ ) {
		dwCode = dwCode^(cnt2<<13)^lpDword[cnt];
		lpDword[cnt] = dwCode;
		cnt2+=cnt2+cnt3;
	}

	return TRUE;
}

int	dm_DecodePacket(DWORD dwObjectSerial, void *lpPacket)
{
	DWORD size, code;
	DWORD cnt, cnt2, cnt3;
	DWORD dwCode;
	DWORD dwCode2;
	DWORD *lpDword = (DWORD *)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /= 4;

	dwCode = (code ^ (code << 7)) + size * 6604;
	cnt2 = (code + dwObjectSerial) & 0xFFCF;
	cnt3 = cnt2 & 0x3C;

	for (cnt = 2; cnt<size; cnt++) {
		dwCode2 = lpDword[cnt];
		lpDword[cnt] = dwCode2 ^ (cnt2 << 13) ^ dwCode;
		dwCode = dwCode2;
		cnt2 += cnt2 + cnt3;
	}

	return TRUE;
}

/*
#define	DAMAGE_PACKET_KEY	0xBBAA3840
#define	DAMAGE_PACKET_MUL	0x11223344

int	dm_DecodePacket2( DWORD dwKey1, DWORD dwKey2 , void *lpPacket );

int	dm_EncodePacket2( void *lpPacket )
{
	DWORD size,code;
	DWORD cnt,cnt2,cnt3;
	DWORD dwCode;
	DWORD *lpDword = (DWORD *)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /=4;


	dwCode = (code^(code<<7)) + size*DAMAGE_PACKET_MUL;
	cnt2 = (code+DAMAGE_PACKET_KEY)&0xF5ABCF;
	cnt3 = cnt2&0x3C;

	for( cnt=2;cnt<size;cnt++ ) {
		dwCode = dwCode^(cnt2<<6)^lpDword[cnt];
		lpDword[cnt] = dwCode;
		cnt2+=cnt2+cnt3;
	}

	return TRUE;
}


int	dm_DecodePacket2( DWORD dwKey1, DWORD dwKey2 , void *lpPacket )
{

#ifdef _W_SERVER
	DWORD size,code;
	DWORD cnt,cnt2,cnt3;
	DWORD dwCode;
	DWORD dwCode2;
	DWORD *lpDword = (DWORD *)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /=4;

	dwCode = (code^(code<<7)) + size*dwKey2;
	cnt2 = (code+dwKey1)&0xF5ABCF;
	cnt3 = cnt2&0x3C;

	for( cnt=2;cnt<size;cnt++ ) {
		dwCode2 = lpDword[cnt];
		lpDword[cnt] = dwCode2^(cnt2<<6)^dwCode;
		dwCode = dwCode2;
		cnt2+=cnt2+cnt3;
	}
#endif
	return TRUE;
}
*/

//0x1f - ffaa3840
//0x13 - 11223344
/*
struct	TRANS_FUNC_MEMORY	{
	int		size,code;
	int		Param[8];
	char	szData[6000];
};
	fnChkMem = (LPFN_CheckMem)((void *)TransFuncMem->szData);

	smTransCommand.size = 0;

	fnChkMem( TransFuncMem , &smTransCommand );

typedef DWORD (*LPFN_CheckMem)( TRANS_FUNC_MEMORY *TransFuncMem , smTRANS_COMMAND	*lpTransCommand );
LPFN_CheckMem	fnChkMem2;

*/

