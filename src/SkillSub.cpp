#include <windows.h>

#include "smlib3d\\smd3d.h"
#include "smwsock.h"

#include "character.h"
#include "playmain.h"
#include "effectsnd.h"
#include "fileread.h"
#include "netplay.h"
#include "makeshadow.h"
#include "particle.h"
#include "field.h"
#include "hoBaram\\holinkheader.h"
#include "sinbaram\\sinlinkheader.h"
#include "playsub.h"
#include "skillsub.h"
#include "damage.h"
#include "areaserver.h"
/*
//��ī�� ��
int Mechanic_Bomb_Damage[10][2] = {{2,5},{3,8},{4,11},{5,14},{6,17},{7,20},{8,23},{9,26},{10,29},{10,32}};
int Mechanic_Bomb_Attack_Range[10] = {50,53,56,59,62,65,68,71,74,77};
int Mechanic_Bomb_UseMana[10] = {8,12,16,20,25,30,36,42,49,56};  

//���̺� 
int Raving_Damage[10]  = {12,17,22,27,32,37,42,47,52,57};
int Ravind_Speed[10]   = {1,1,1,2,2,2,3,3,3,4};
int Raving_UseLife[10] = {10,14,18,20,24,28,30,32,34,38};
int Raving_UseMana[10] = {2,3,5,7,9,11,14,17,20,23};

*/

static sSKILL Skill_ScoutHawk;

//int	ScoutHawk_LevelRange



//��ų�� �����Ѵ�
int OpenPlaySkill( sSKILL *lpSkill )
{
	int flag;
	//int	sp,ep;
	int cnt;
	short wpDamage[2];
	//smTRANS_COMMAND	smTransCommand;

	pUseSkill = lpSkill;

	flag = 0;

	if ( lpCurPlayer->OnStageField>=0 && StageField[ lpCurPlayer->OnStageField ]->State==FIELD_STATE_VILLAGE )
		return flag;

	if ( sinCheckSkillUseOk( lpSkill )==FALSE ) return flag;
	if ( lpSkill->Point>10 ) return flag;

	wpDamage[0] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0];
	wpDamage[1] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1];

	lpSkill->SkillTaget_CODE = 0;

	switch( lpSkill->Skill_Info.CODE ) {
				
	case SKILL_EXTREME_SHIELD:
		//�ͽ�Ʈ�� ����
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
				lpCurPlayer->BeginSkill( SKILL_PLAY_EXTREME_SHIELD , 0 , 0 ,0,0,0 );
				lpCurPlayer->AttackSkil = SKILL_PLAY_EXTREME_SHIELD;
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 100;
				lpCurPlayer->MotionLoop = 1;
				flag++;
			}
		}
		break;

	case SKILL_PHYSICAL_ABSORB:
		//����Ŭ ������
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_PHYSICAL_ABSORB;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 70;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_PHYSICAL_ABSORB , lpSkill->Point , 0, 0 );

				//��ų ���� ����
				SkillPlaySound( SKILL_SOUND_PHYSICAL_ABSORB3 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, TRUE );
				flag++;
			}
		}
		break;

	case SKILL_POISON_ATTRIBUTE:
		//������ ��Ʈ����
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->BeginSkill( SKILL_PLAY_POISON_ATTRIBUTE , 0 , 0 ,0,0,0 );
			flag++;
		}
		break;


	case SKILL_PIKE_WIND:

		//����ũ ����
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			if ( lpSkill->Point ) {
				lpCurPlayer->BeginSkill(SKILL_PLAY_PIKEWIND,0,0,0,0,0);
				lpCurPlayer->AttackSkil = SKILL_PLAY_PIKEWIND;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;
/*
				sp = Pike_Wind_Damage[lpSkill->Point-1][0];
				ep = Pike_Wind_Damage[lpSkill->Point-1][1];

				Skil_RangeAttack( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, Pike_Wind_Push_Lenght[lpSkill->Point-1]*fONE , 
					GetRandomPos( sp, ep ) , 1 );
*/

				//������ ���� ���� ���� (��)
				dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, Pike_Wind_Push_Lenght[lpSkill->Point-1] , FALSE );
				//�������� ������ ����
				dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

				flag++;
			}
		}
		break;

	case SKILL_SCOUT_HAWK:
		//��ī��Ʈ ȣũ
		if ( lpCurPlayer->HoSkillCode!=SKILL_SCOUT_HAWK && lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			lpCurPlayer->AttackSkil = SKILL_PLAY_SCOUT_HAWK;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,0,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			memcpy( &Skill_ScoutHawk , lpSkill , sizeof( sSKILL ) );
			lpCurPlayer->HideWeapon = TRUE;
			flag++;
		}
		break;

	/////////////////////////////////// 2�ܰ� ��ų ///////////////////////////////////////

	//	SKILL_PLAY_GREAT_SMASH
	case SKILL_MAXIMIZE:
		//�ƽø�����
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE && lpSkill->Point ) {

			lpCurPlayer->AttackSkil = SKILL_PLAY_MAXIMIZE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = 70;
			lpCurPlayer->MotionLoop = 1;

			flag++;
		}
		break;

	case SKILL_AUTOMATION:
		//������̼�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE && lpSkill->Point ) {

			lpCurPlayer->AttackSkil = SKILL_PLAY_AUTOMATION;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = 70;
			lpCurPlayer->MotionLoop = 1;
				
			SkillPlaySound( SKILL_SOUND_SKILL_AUTOMATION3 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, TRUE );

			flag++;
		}
		break;

	case SKILL_GROUND_PIKE:
		//�׶��� ����ũ ( �����Ӽ� ���� ) 10-(���ͼӼ�/10) ��
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			if ( lpSkill->Point ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_GROUND_PIKE;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,0,0,0,0);
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

/*
				sp = G_Pike_Damage[lpSkill->Point-1][0];
				ep = G_Pike_Damage[lpSkill->Point-1][1];

				Skil_RangeAttack( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, G_Pike_Range[lpSkill->Point-1]*fONE , 
					GetRandomPos( sp, ep ) , 3 );
*/
				//������ ���� ���� ���� (��)
				dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, G_Pike_Range[lpSkill->Point-1] , FALSE );
				//�������� ������ ����
				dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

				flag++;
			}
		}
		break;


	case SKILL_VENOM_SPEAR:
		//���� ���Ǿ�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			if ( lpSkill->Point ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_VENOM_SPEAR;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,0,0,0,0);
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;
/*
				//������ ���� ���� ���� (��)
				dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, G_Pike_Range[lpSkill->Point-1] , FALSE );
				//�������� ������ ����
				dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );
*/
				flag++;
			}
		}
		break;

	case SKILL_VANISH:
		//��Ͻ�		//���̵� ���� ������
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			if ( lpSkill->Point ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_VANISH;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,0,0,0,0);
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				flag++;
			}
		}
		break;



	case SKILL_FALCON:
		//����
		if ( lpCurPlayer->HoSkillCode!=SKILL_FALCON && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			if ( lpSkill->Point ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_FALCON;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,0,0,0,0);
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;
				lpCurPlayer->HideWeapon = TRUE;

				SkillFalconPoint = (lpSkill->Point-1)^lpCurPlayer->dwObjectSerial^((DWORD)chrOtherPlayer);

				SendProcessSKillToServer( SKILL_PLAY_FALCON , lpSkill->Point , 0, 0 );
			}
		}
		break;

	case SKILL_GOLDEN_FALCON:
		//��� ����
		if ( lpCurPlayer->HoSkillCode!=SKILL_GOLDEN_FALCON && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			if ( lpSkill->Point ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_GOLDEN_FALCON;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,0,0,0,0);
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;
				lpCurPlayer->HideWeapon = TRUE;

				SkillFalconPoint = (lpSkill->Point-1)^lpCurPlayer->dwObjectSerial^((DWORD)chrOtherPlayer);

				cnt = cSkill.GetSkillPoint( SKILL_FALCON );			//������ ����Ʈ
				SendProcessSKillToServer( SKILL_PLAY_GOLDEN_FALCON , lpSkill->Point , cnt, 0 );
			}
		}
		break;


	case SKILL_ROAR:
		//�ξ� ( ����Ȯ��  ((�������Ӽ� + 100 )-���� ���Ӽ�) %
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			if ( lpSkill->Point ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_ROAR;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,0,0,0,0);
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				//�о��
				//Skil_RangeAttack( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, Roar_Range[lpSkill->Point-1]*fONE , Roar_Time[lpSkill->Point-1] , 2 );

				//������ ���� ���� ���� (��)
				dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, Roar_Range[lpSkill->Point-1] , FALSE );
				//�������� ������ ����
				dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

				flag++;
			}
		}
		break;

	case SKILL_CONCENTRATION:
		//�ܼ�Ʈ
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			if ( lpSkill->Point ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_CONCENTRATION;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,0,0,0,0);
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				flag++;
			}
		}
		break;

	case SKILL_SWIFT_AXE:
		//����Ʈ�׽�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			if ( lpSkill->Point ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_SWIFT_AXE;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,0,0,0,0);
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				flag++;
			}
		}
		break;

	/////////////////////////////////// 3�ܰ� ��ų ///////////////////////////////////////

	case SKILL_METAL_ARMOR:
		//��Ż �Ƹ�
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_METAL_ARMOR;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 70;
				lpCurPlayer->MotionLoop = 1;

				cnt = cSkill.GetSkillPoint( SKILL_PHYSICAL_ABSORB );			//�������� ����Ʈ

				SendProcessSKillToServer( SKILL_PLAY_METAL_ARMOR , lpSkill->Point , cnt , 0 );
				//��ų ���� ����
				//SkillPlaySound( SKILL_SOUND_PHYSICAL_ABSORB3 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, TRUE );
				flag++;
			}
		}
		break;

	case SKILL_SPARK_SHIELD:
		//����ũ����
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_SPARK_SHIELD;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 70;
				lpCurPlayer->MotionLoop = 1;

				cnt = cSkill.GetSkillPoint( SKILL_SPARK );			//����ũ�� ����Ʈ

				SendProcessSKillToServer( SKILL_PLAY_SPARK_SHIELD , lpSkill->Point , cnt, 0 );
				//��ų ���� ����
				//SkillPlaySound( SKILL_SOUND_PHYSICAL_ABSORB3 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, TRUE );
				flag++;
			}
		}
		break;


		/////////////////////////////////////////////////////////////////////////
		///////////////////////////// ����̿� ��ų /////////////////////////////
		/////////////////////////////////////////////////////////////////////////

		//////////////////////////// ����Ʈ /////////////////////////////////////

	case SKILL_HOLY_BODY:
		//����Ŭ ������
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_HOLY_BODY;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 70;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_HOLY_BODY , lpSkill->Point , 0, 0 );

				//��ų ���� ����
				SkillPlaySound( SKILL_SOUND_PHYSICAL_ABSORB3 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, TRUE );
				flag++;
			}
		}
		break;
		//////////////////////// 2�� ��ų /////////////////////////
	case SKILL_HOLY_VALOR:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_HOLY_VALOR;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = 70;
			lpCurPlayer->MotionLoop = 1;
			flag++;
		}
		break;

	case SKILL_DRASTIC_SPIRIT:
		//�巡��ƽ���Ǹ�Ʈ
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_DRASTIC_SPIRIT;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = 70;
			lpCurPlayer->MotionLoop = 1;
			flag++;
		}
		break;

	case SKILL_DIVINE_INHALATION:
		//����� ���淹�̼�
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_DIVINE_INHALATION;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 70;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_DIVINE_INHALATION , lpSkill->Point , 0, 0 );
				//��ų ���� ����
				//SkillPlaySound( SKILL_SOUND_PHYSICAL_ABSORB3 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, TRUE );

				flag++;
			}
		}
		break;

		//////////////////////////// ��Ż��Ÿ /////////////////////////////////////
	case SKILL_WINDY:
		//����
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_WINDY;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			flag++;
		}
		break;

	case SKILL_TRIUMPH_OF_VALHALLA:
		//���Ҷ�
		if ( !lpCharSelPlayer && 
			GetAssaUseEffect(lpCurPlayer,SKILL_TRIUMPH_OF_VALHALLA)==FALSE && 
			!cSkill.SearchContiueSkill(SKILL_HALL_OF_VALHALLA) &&
			lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			lpCurPlayer->AttackSkil = SKILL_PLAY_TRIUMPH_OF_VALHALLA;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//��Ƽ������ ��ų ��� ���� ������
			SendProcessSKillToServer( SKILL_PLAY_TRIUMPH_OF_VALHALLA , lpSkill->Point , 0, lpCurPlayer->smCharInfo.Level );
			lpCurPlayer->chrAttackTarget = 0; 
			cSkill.SetT_Of_Valhalla( lpSkill->Point , lpCurPlayer->smCharInfo.Level );

			flag++;
		}
		break;
/*
	case SKILL_LIGHTNING_JAVELIN:
		//����Ʈ�� ���
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_LIGHTNING_JAVELIN;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			flag++;
		}
		break;
*/

		//////////////////////////// ������Ƽ�� /////////////////////////////////////
	case SKILL_HEALING:
		//����
		if ( lpCurPlayer->MotionInfo->State<0x100 && !lpCharSelPlayer ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_HEALING;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 70;
				lpCurPlayer->MotionLoop = 1;

				cnt = GetRandomPos( Healing_Heal[lpSkill->Point-1][0]+wpDamage[0], Healing_Heal[lpSkill->Point-1][1]+wpDamage[0] );

				sinSetLife( sinGetLife()+cnt );
				ResetEnergyGraph( 0 );


/*
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.code = smTRANSCODE_PARTY_HEALING;
				smTransCommand.WParam =	cnt;	//�����
				smTransCommand.LParam =	0;		//
				smTransCommand.SParam =	0;		//�����Ÿ�
				smTransCommand.EParam = lpCurPlayer->dwObjectSerial;

				if ( smWsockUserServer && InterfaceParty.PartyMemberCount )
					smWsockUserServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
*/
				flag++;
			}
		}
		break;
/*
	case SKILL_DIVINE_LIGHTNING:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_DIVINE_LIGHTNING;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			flag++;
		}
		break;
*/
	case SKILL_DIVINE_LIGHTNING:
		//����� ����Ʈ��
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			//SkillPlay_DivineLightning_Select( lpCurPlayer , lpSkill->Point );
			lpCurPlayer->AttackSkil = SKILL_PLAY_DIVINE_LIGHTNING;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,0,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = 85;
			lpCurPlayer->MotionLoop = 1;

			flag++;
		}
		break;

	case SKILL_HOLY_REFLECTION:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_HOLY_REFLECTION;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			flag++;
		}
		break;
	case SKILL_GRAND_HEALING:
		if ( InterfaceParty.PartyMemberCount>0 && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_GREAT_HEALING;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = 70;
			lpCurPlayer->MotionLoop = 1;
/*
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_GRAND_HEALING;
			smTransCommand.WParam =	lpSkill->Point;					//��ų����Ʈ
			smTransCommand.LParam =	GetRandomPos(Grand_Healing[lpSkill->Point-1][0] , Grand_Healing[lpSkill->Point-1][1] );
			smTransCommand.SParam =	Grand_Healing_Range[lpSkill->Point-1];
			smTransCommand.EParam = lpCurPlayer->dwObjectSerial;	//

			if ( smWsockUserServer )
				smWsockUserServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
*/
			if ( AreaServerMode ) 
				dm_SendTransDamage( lpCurPlayer , wpDamage[0],wpDamage[1] ,0,0, lpCurPlayer->AttackSkil , FALSE , smWsockServer );	
			else
				dm_SendTransDamage( lpCurPlayer , wpDamage[0],wpDamage[1] ,0,0, lpCurPlayer->AttackSkil , FALSE );	

			flag++;
		}
		break;

	case SKILL_RESURRECTION:		//������ �һ�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_RESURRECTION;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			flag++;
		}
		break;

	case SKILL_EXTINCTION:		//�𵥵� �Ҹ�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_EXTINCTION;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//������ ���� ���� ���� (��)
			dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 160 , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

			flag++;
		}
		break;

	case SKILL_VIRTUAL_LIFE:		//������ ������
		if ( !lpCharSelPlayer && GetAssaUseEffect(lpCurPlayer,SKILL_VIRTUAL_LIFE)==FALSE && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_VIRTUAL_LIFE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			SendProcessSKillToServer( SKILL_PLAY_VIRTUAL_LIFE , lpSkill->Point , 0, 0 );
			cSkill.SetVirtualLife( Virtual_Life_Time[ lpSkill->Point-1 ] , lpSkill->Point );
			lpCurPlayer->chrAttackTarget = 0;

			flag++;
		}
		break;

		//////////////////////////// ������ /////////////////////////////////////
	case SKILL_AGONY:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_AGONY;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 70;
				lpCurPlayer->MotionLoop = 1;
				flag++;

				//������ �׷��� ýũ �ʱ�ȭ
				ResetEnergyGraph( 3 );
			}
		}
		break;

	case SKILL_ZENITH:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_ZENITH;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 70;
				lpCurPlayer->MotionLoop = 1;

				//SendProcessSKillToServer( SKILL_PLAY_ZENITH , lpSkill->Point , 0, 0 );
				//�Ӽ����� ������ ����
				SendResistanceToServer();

				flag++;
			}
		}
		break;

	case SKILL_ENCHANT_WEAPON:
		if ( lpCurPlayer->MotionInfo->State<0x100 && !lpCharSelPlayer && !lpCurPlayer->WeaponEffect ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
				lpCurPlayer->chrAttackTarget = 0;

				lpCurPlayer->AttackSkil = SKILL_PLAY_ENCHANT_WEAPON;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				cnt = rand()%3;
				lpCurPlayer->AttackSkil |= ( cnt<<12 );
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 70;
				lpCurPlayer->MotionLoop = 1;
				flag++;
			}
		}
		break;


	case SKILL_ENERGY_SHIELD:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
				lpCurPlayer->AttackSkil = SKILL_PLAY_ENERGY_SHIELD;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_ENERGY_SHIELD , lpSkill->Point , 0, 0 );

				flag++;
			}
		}
		break;

	case SKILL_DIASTROPHISM:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_DIASTROPHISM;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 120;
				lpCurPlayer->MotionLoop = 1;
				flag++;
/*
				//������ ���� ���� ���� (��)
				dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, Diastrophism_Area[lpSkill->Point-1] , FALSE );

				//�������� ������ ����
				dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0,
					cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0] ,
					cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1] ,
					0, 0, lpCurPlayer->AttackSkil );
*/

			}
		}
		break;

	case SKILL_SPIRIT_ELEMENTAL:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_SPIRIT_ELEMENTAL;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_SPIRIT_ELEMENTAL , lpSkill->Point , 0, 0 );

				flag++;
			}
		}
		break;

	case SKILL_DANCING_SWORD:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_DANCING_SWORD;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				cnt = rand()%2;
				lpCurPlayer->AttackSkil |= ( cnt<<12 );
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_DANCING_SWORD , lpSkill->Point , cnt, 0 );

				flag++;
			}
			
		}
		break;


	////////////////////// 4�� ��ų ////////////////////////
	///��ī�ϼ�
	case SKILL_COMPULSION:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_COMPULSION;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				//������ ���� ���� ���� (��)
				dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, Compulsion_Area[lpSkill->Point-1] , FALSE );
				//�������� ������ ����
				dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

				flag++;
			}
		}
		break;

	case SKILL_MAGNETIC_SPHERE:		//��̼ҵ�� ����ϴٰ� ��
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_MAGNETIC_SPHERE;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_MAGNETIC_SPHERE , lpSkill->Point , 0, 0 );

				flag++;
			}
		}
		break;

	case SKILL_METAL_GOLEM:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_METAL_GOLEM;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_METAL_GOLEM , lpSkill->Point , 0, 0 );

				flag++;
			}
		}
		break;

	///������
	case SKILL_BERSERKER:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_BERSERKER;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_BERSERKER , lpSkill->Point , 0, 0 );

				flag++;
			}
		}
		break;
/*
	case SKILL_CYCLONE_STRIKE:
		//CYCLONE_STRIKE
		if ( lpCurPlayer->MotionInfo->State<0x100 && !lpCharSelPlayer ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_CYCLONE_STRIKE;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				//������ ���� ���� ���� (��)
				dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, Cyclone_Strike_Area[lpSkill->Point-1] , TRUE , Cyclone_Strike_AttackNum[lpSkill->Point-1] );
				//�������� ������ ����
				dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ,  0 , 0 , 0 ,0,0, lpCurPlayer->AttackSkil );

				flag++;
			}
		}
		return TRUE;
*/

	//����ũ��
		/*
	case SKILL_ASSASSIN_EYE:
		if ( lpCurPlayer->MotionInfo->State<0x100 && !lpCharSelPlayer ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_ASSASSIN_EYE;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_ASSASSIN_EYE , lpSkill->Point , 0, 0 );

				flag++;
			}
		}
		break;
		*/
	case SKILL_VAGUE:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_VAGUE;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_VAGUE , lpSkill->Point , 0, 0 );

				flag++;
			}
		}
		break;

	//��ó
	case SKILL_RECALL_WOLVERIN:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_RECALL_WOLVERIN;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;
				lpCurPlayer->HideWeapon = TRUE;
				flag++;

				SendProcessSKillToServer( SKILL_PLAY_RECALL_WOLVERIN , lpSkill->Point , 0, 0 );
			}
		}
		break;

	case SKILL_FORCE_OF_NATURE:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_FORCE_OF_NATURE;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_FORCE_OF_NATURE , lpSkill->Point , 0, 0 );
				cSkill.ForeceOfNature( Force_Of_Nature_Time[lpSkill->Point-1] , lpSkill->Point );

				lpCurPlayer->dwForceOfNatureTime = dwPlayTime+Force_Of_Nature_Time[lpSkill->Point-1]*1000;	//�������� ������ �����ð�

				flag++;
			}
		}
		break;

	//����Ʈ
	case SKILL_GODLY_SHIELD:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_GODLY_SHIELD;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_GODLY_SHIELD , lpSkill->Point , 0, 0 );
				flag++;
			}
		}
		break;

	case SKILL_GOD_BLESS:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_GODS_BLESS;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_GODS_BLESS , lpSkill->Point , 0, 0 );
				flag++;
			}
		}
		break;

	//��Ż��Ÿ
	case SKILL_HALL_OF_VALHALLA:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( GetAssaUseEffect(lpCurPlayer,SKILL_TRIUMPH_OF_VALHALLA)==FALSE && lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_HALL_OF_VALHALLA;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				cnt = cSkill.GetSkillPoint( SKILL_TRIUMPH_OF_VALHALLA );			//Ʈ���̾������Ҷ��� ����Ʈ
				SendProcessSKillToServer( SKILL_PLAY_HALL_OF_VALHALLA , lpSkill->Point , cnt, lpCurPlayer->smCharInfo.Level );

				cSkill.HellOfValhalla( Hall_Of_Valhalla_Time[lpSkill->Point-1] , lpSkill->Point , cnt , lpCurPlayer->smCharInfo.Level , 0 );

				lpCurPlayer->dwHallOfValhallaTime = dwPlayTime+Hall_Of_Valhalla_Time[lpSkill->Point-1]*1000;	//Ȧ������Ҷ� �����ð�

				flag++;
			}
		}
		break;

	case SKILL_FROST_JAVELIN:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_FROST_JAVELIN;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_FROST_JAVELIN , lpSkill->Point , 0, 0 );
				flag++;
			}
		}
		break;

	//������Ƽ��
	case SKILL_REGENERATION_FIELD:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer() == TRUE ){ // && InterfaceParty.PartyMemberCount>0 && lpSkill->PartyFlag && lpSkill->SkillTaget_CODE != 0) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_REGENERATION_FIELD;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				cSkill.Regeneration_Field( Regeneration_Field_Time[lpSkill->Point-1] , lpSkill->Point , 0 );		//����� �߰�
				flag++;
			}
		}
		break;

	case SKILL_SUMMON_MUSPELL:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_SUMMON_MUSPELL;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_SUMMON_MUSPELL , lpSkill->Point , 0, 0 );

				flag++;
			}
		}
		break;

	//������
	case SKILL_FIRE_ELEMENTAL:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_FIRE_ELEMENTAL;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				SendProcessSKillToServer( SKILL_PLAY_FIRE_ELEMENTAL , lpSkill->Point , 0, 0 );

				flag++;
			}
		}
		break;
	case SKILL_DISTORTION:
		if ( lpCurPlayer->MotionInfo->State<0x100 ) {
			if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

				lpCurPlayer->AttackSkil = SKILL_PLAY_DISTORTION;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0, 0 ,0,0,0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = 80;
				lpCurPlayer->MotionLoop = 1;

				//������ ���� ���� ���� (��)
				dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, Distortion_Area[lpSkill->Point-1] , FALSE );
				//�������� ������ ����
				dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );
								
/*
				//�׽�Ʈ��
				if ( dmSelected_CharCnt ) {
					for(cnt=0;cnt<dmSelected_CharCnt;cnt++) {
						lpSelected_Char[cnt]->PlayDistortion = 70*5;
					}
				}
				
				*/
				flag++;
			}
			
		}
		break;


	}

	if ( flag ) CheckPlaySkill();		////��ų����

	return flag;
}


int	NormalAttackRange;			//�⺻ ���� �Ÿ�
int	NormalAttackMode;
int	LastAttackRange;

//��ų ���� �Ÿ��� ���Ѵ�
int GetSkillDistRange( sSKILL *lpSkill , int AttackRange , int AttackMode )
{
	int cnt;

	NormalAttackRange = AttackRange;
	NormalAttackMode = AttackMode;

	switch( lpSkill->CODE ) {
	case SKILL_MECHANIC_BOMB:
		if ( lpSkill->Point ) {
			//��ī�й� �����Ÿ� 
			cnt = Mechanic_Bomb_Attack_Range[lpSkill->Point-1];
			return	(cnt*3*fONE);
		}
		break;

	case SKILL_SPARK:
		if ( lpSkill->Point ) {
			//����ũ �����Ÿ� 
			return	140*fONE;
		}
		break;

	case SKILL_EXPANSION:
		//�ͽ��Ҽ�
		if ( lpSkill->Point ) {
			cnt = AttackRange+((AttackRange-50*fONE) * Expansion_Size[lpSkill->Point-1])/100;
			LastAttackRange = cnt;
			return cnt;
		}
		break;

	case SKILL_VENGEANCE:
		if ( lpSkill->Point ) {
			cnt = AttackRange+Vengeance_Range[lpSkill->Point-1]*fONE;
			LastAttackRange = cnt;
			return cnt;
		}
		break;

	case SKILL_TORNADO:
		//����̵�
		if ( lpSkill->Point ) {
			cnt = Tornado_Range[lpSkill->Point-1];
			if ( AttackRange>cnt ) break;
			LastAttackRange = cnt;
			return cnt;
		}
		break;

	case SKILL_SWORD_BLAST:
		//�ҵ� ����Ʈ
		if ( lpSkill->Point ) {
			cnt = Sword_Blast_ShootingRange[lpSkill->Point-1];
			if ( AttackRange>cnt ) break;
			LastAttackRange = cnt;
			return cnt;
		}
		break;

	case SKILL_SHIELD_STRIKE:
		//���� ��Ʈ����ũ
		return	110*fONE;

	case SKILL_HEALING:
		//����
		return	180*fONE;
	case SKILL_HOLY_BOLT:
		//Ȧ����Ʈ
		return	170*fONE;
	case SKILL_MULTISPARK:
		//��Ƽ����ũ
		return	210*fONE;
	case SKILL_HOLY_MIND:
		//Ȧ�����ε�
		return	160*fONE;

	case SKILL_DIVINE_LIGHTNING:
		return 160*fONE;

	case SKILL_FIRE_BOLT:
		if ( lpSkill->Point ) {
			//���̾� ��Ʈ
			return	200*fONE;
		}
		break;
	case SKILL_FIRE_BALL:
		if ( lpSkill->Point ) {
			//���̾� ��
			return	FireBall_Range[lpSkill->Point-1]*fONE;
		}
		break;

	case SKILL_WATORNADO:
		if ( lpSkill->Point ) {
			return Watornado_Range[lpSkill->Point-1]*fONE;
		}
		break;

	case SKILL_ENCHANT_WEAPON:
		return	140*fONE;

	case SKILL_DEAD_RAY:
		if ( lpSkill->Point ) {
			return	Dead_Ray_Range[lpSkill->Point-1]*fONE;
		}
		break;

	case SKILL_HOLY_INCANTATION:
		return	120*fONE;

	case SKILL_VIGOR_BALL:
		return	230*fONE;

	case SKILL_TRIUMPH_OF_VALHALLA:
		return	200*fONE;

	case SKILL_PERFORATION:
		if ( lpSkill->Point ) {
			return	AttackRange+(Perforation_Attack_Range[lpSkill->Point-1]*fONE);
		}
		break;

	case SKILL_VIRTUAL_LIFE:
		return 200*fONE;

	}

	return NULL;
}

//���� ��õ�
int RetryPlayAttack( smCHAR *lpChar ) {

	return lpCurPlayer->PlayAttackFromPosi( lpChar->pX , lpChar->pY , lpChar->pZ ,
							NormalAttackRange , NormalAttackMode ) ;

}


//������ ����
int PlaySkillSubLife( int Life )
{
	int sLife;

	CheckCharForm();
	//�÷��̾� ����
	sLife = sinGetLife();
	sLife -= Life;
	if ( sLife<=0 ) sLife = 1;
	sinSetLife( sLife );
	//ReformCharForm();					//ĳ���� ���� ����

	return sLife;
}


//��ų ������ �õ�
int PlaySkillAttack( sSKILL *lpSkill , smCHAR *lpChar )
{

	int cnt;
	int	temp;//,temp2,temp3;
	int x,y,z;
	smTRANS_COMMAND	smTransCommand;
	RECT	rect;
	smWINSOCK	*lpsmSock;
	short wpDamage[2];
	int	 skFlag;

	lpCurPlayer->AttackSkil = 0;

	if ( !lpSkill ) return FALSE;

	if ( lpSkill->CODE==SKILL_HEALING || lpSkill->CODE==SKILL_ENCHANT_WEAPON || 
		lpSkill->CODE==SKILL_VIRTUAL_LIFE || lpSkill->CODE==SKILL_TRIUMPH_OF_VALHALLA ) {
			skFlag = TRUE;
		}
	else {
		skFlag = FALSE;
	}


	if ( !PkMode && lpChar->smCharInfo.State!=smCHAR_STATE_ENEMY && skFlag==FALSE )
		return FALSE;

	if ( sinCheckSkillUseOk( lpSkill )==FALSE || lpSkill->Point>10 ) {
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) return FALSE;;
		return TRUE;
	}

	CheckPlaySkill();		////��ų����

	pUseSkill = lpSkill;


	wpDamage[0] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0];
	wpDamage[1] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1];

	lpSkill->SkillTaget_CODE = lpChar->dwObjectSerial;

	switch( lpSkill->CODE ) {
	case SKILL_EXTREME_SHIELD:
		//�ͽ�Ʈ�� ����
		return TRUE;

	case SKILL_MECHANIC_BOMB:
		//��ī�й�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_MECHANIC_BOMB;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->HideWeapon = TRUE;
		}
		return TRUE;

	case SKILL_PHYSICAL_ABSORB:
		return TRUE;

	case SKILL_RAVING:				//���̺�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

			lpCurPlayer->AttackSkil = SKILL_PLAY_RAVING;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				lpCurPlayer->MotionLoop = Ravind_Speed[lpSkill->Point-1];
				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , Raving_Damage[lpSkill->Point-1] );
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt ) { 
					lpCurPlayer->AttackCritcal = -1;				//������
				}
				else {
					//������ ����
					PlaySkillSubLife( ((int)(Raving_UseLife[lpSkill->Point-1]*10)*lpCurPlayer->smCharInfo.Life[1])/1000 );
					//�������� ���� ����ġ ���
					DamageExp( lpChar , cnt );
				}

			}
			else {
				lpCurPlayer->MotionLoop = 1;
			}

			lpCurPlayer->MotionLoopSpeed = 90+(10*lpCurPlayer->MotionLoop);
			if ( lpCurPlayer->MotionLoopSpeed<100 ) lpCurPlayer->MotionLoopSpeed = 100;


	//lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;


			lpCurPlayer->AttackAnger = 150;
			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_IMPACT:				//����Ʈ

		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_IMPACT;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
/*
			//���ݼӵ�
			cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
			if ( cnt<0 ) cnt=0;
			if ( cnt>6 ) cnt=6;
			cnt+=2;					//�ӵ� +2
			lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;
			lpCurPlayer->MotionLoop = 1;
*/
			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				temp = lpCurPlayer->smCharInfo.Attack_Rating;
				lpCurPlayer->smCharInfo.Attack_Rating += Impact_Attack_Rating[lpSkill->Point-1];
				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , Impact_Damage[lpSkill->Point-1] );
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
				lpCurPlayer->smCharInfo.Attack_Rating = temp;

				if ( !cnt )
					lpCurPlayer->AttackCritcal = -1;				//������
				else {
					//�������� ���� ����ġ ���
					DamageExp( lpChar , cnt );
				}
			}
			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_PIKE_WIND:
		return TRUE;

	case SKILL_CRITICAL_HIT:
		//ũ��Ƽ�� ��
		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

			lpCurPlayer->AttackSkil = SKILL_PLAY_CRITICAL_HIT;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
/*
			//ũ��Ƽ�� �缳��
			cnt = lpCurPlayer->smCharInfo.Critical_Hit;
			lpCurPlayer->smCharInfo.Critical_Hit += Critical_Hit_Critical[lpSkill->Point-1];

			if ( (rand()%100)<sinGetCritical(lpChar->smCharInfo.Level)) {
				lpCurPlayer->AttackCritcal = TRUE;			//ũ��Ƽ�� ���� ����
			}
			else {
				lpCurPlayer->AttackCritcal = FALSE;
			}
			lpCurPlayer->smCharInfo.Critical_Hit = cnt;
*/
			//���� 2�� �õ�
			cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
			if ( cnt<0 ) cnt=0;
			if ( cnt>6 ) cnt=6;
			cnt+=2;					//�ӵ� +2.5
			lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt+32))>>FLOATNS;

			//lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed , 2 );
			lpCurPlayer->MotionLoop = 2;
			break;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_JUMPING_CRASH:
		//���� Ŭ����
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			/*
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackCritcal = 1;
			lpCurPlayer->BeginSkill(SKILL_PLAY_JUMPING_CRASH,0,lpChar,0,0,0);
			lpCurPlayer->AttackSkil = SKILL_PLAY_JUMPING_CRASH;
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			*/

			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_JUMPING_CRASH;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				temp = lpCurPlayer->smCharInfo.Attack_Rating;
				//temp2 = lpCurPlayer->smCharInfo.Attack_Damage[1];
				//temp3 = lpCurPlayer->smCharInfo.Attack_Damage[0];
				//���߷� �� �ִ� ������ ����
				lpCurPlayer->smCharInfo.Attack_Rating += 
					( lpCurPlayer->smCharInfo.Attack_Rating * Jumping_Crash_Attack_Rating[lpSkill->Point-1])/100;
/*
				lpCurPlayer->smCharInfo.Attack_Damage[0] += 
					(lpCurPlayer->smCharInfo.Attack_Damage[0]*Jumping_Crash_Damage[lpSkill->Point-1])/100;
				lpCurPlayer->smCharInfo.Attack_Damage[1] += 
					(lpCurPlayer->smCharInfo.Attack_Damage[1]*Jumping_Crash_Damage[lpSkill->Point-1])/100;

				cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0  );

				lpCurPlayer->smCharInfo.Attack_Damage[1] = temp2;
				lpCurPlayer->smCharInfo.Attack_Damage[0] = temp3;
*/
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
				lpCurPlayer->smCharInfo.Attack_Rating = temp;

				if ( !cnt )
					lpCurPlayer->AttackCritcal = -1;				//������
				else 
					DamageExp( lpChar , cnt );					//�������� ���� ����ġ ���
			}
			lpCurPlayer->MotionLoopSpeed = 80;
			lpCurPlayer->MotionLoop = 1;
			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_SCOUT_HAWK:
		//��ī��Ʈ ȣũ
		return TRUE;

	case SKILL_WIND_ARROW:
		//���� ���ο� 

		if ( lpCurPlayer->CheckShootingTest(lpChar)==FALSE ) {
			lpCurPlayer->AttackCritcal = -1;				//������
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			return TRUE;
		}

		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			if ( lpCurPlayer->CheckShootingTest(lpChar)==TRUE ) {

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

				lpCurPlayer->AttackSkil = SKILL_PLAY_WIND_ARROW;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , Wind_Arrow_Damage[lpSkill->Point-1] );
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )	
					lpCurPlayer->AttackCritcal = -1;				//������
				else 
					DamageExp( lpChar , cnt );					//�������� ���� ����ġ ���


				//����ӵ� ���� 
				/*
				cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
				if ( cnt<0 ) cnt=0;
				if ( cnt>6 ) cnt=6;
				cnt += Wind_Arrow_Speed[lpSkill->Point-1];
				lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;
				*/
				lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed , Wind_Arrow_Speed[lpSkill->Point-1] );

				lpCurPlayer->MotionLoop = 1;
			}
			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_PERFECT_AIM:
/*
int Perfect_Aim_Attack_Rate[10] = {26,27,29,32,36,41,47,54,62,71};
int Perfect_Aim_Damage[10] = {1,2,3,4,5,6,7,8,9,10};
*/

		//����Ʈ ����
		if ( lpCurPlayer->CheckShootingTest(lpChar)==FALSE ) {
			lpCurPlayer->AttackCritcal = -1;				//������
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			return TRUE;
		}

		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

				lpCurPlayer->AttackSkil = SKILL_PLAY_PERFECT_AIM;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

				temp = lpCurPlayer->smCharInfo.Attack_Rating;
				//temp2 = lpCurPlayer->smCharInfo.Attack_Damage[0];
				//temp3 = lpCurPlayer->smCharInfo.Attack_Damage[1];

				//���߷� / ���ݷ� ����
				lpCurPlayer->smCharInfo.Attack_Rating += 
					(lpCurPlayer->smCharInfo.Attack_Rating * Perfect_Aim_Attack_Rate[lpSkill->Point-1])/100;
/*
				cnt = Perfect_Aim_Damage[lpSkill->Point-1];
				lpCurPlayer->smCharInfo.Attack_Damage[0] += cnt;
				lpCurPlayer->smCharInfo.Attack_Damage[1] += cnt;
				cnt = lpCurPlayer->smCharInfo.Level/Perfect_Aim_Damage_LV[lpSkill->Point-1];
				lpCurPlayer->smCharInfo.Attack_Damage[0] += cnt;
				lpCurPlayer->smCharInfo.Attack_Damage[1] += cnt;
*/
				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 );
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )	
					lpCurPlayer->AttackCritcal = -1;				//������
				else
					DamageExp( lpChar , cnt );					//�������� ���� ����ġ ���

				//����,���ݷ� ��ġ ����
				lpCurPlayer->smCharInfo.Attack_Rating = temp;
				//lpCurPlayer->smCharInfo.Attack_Damage[0] = temp2;
				//lpCurPlayer->smCharInfo.Attack_Damage[1] = temp3;


			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

		/////////////////////////////// 2�� ��ų /////////////////////////////////////

	case SKILL_GREAT_SMASH:
		//�׷���Ʈ���Ž� 
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_GREAT_SMASH;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = 110;
			lpCurPlayer->MotionLoop = 1;

			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				temp = lpCurPlayer->smCharInfo.Attack_Rating;
				//temp2 = lpCurPlayer->smCharInfo.Attack_Damage[1];
				//temp3 = lpCurPlayer->smCharInfo.Attack_Damage[0];

				//���߷� ����
				lpCurPlayer->smCharInfo.Attack_Rating += G_Smash_Attack_Rate[lpSkill->Point-1];
/*
				cnt = lpCurPlayer->smCharInfo.Level/G_Smash_Attack_Rage[lpSkill->Point-1];
				lpCurPlayer->smCharInfo.Attack_Damage[0] = (lpCurPlayer->smCharInfo.Attack_Damage[0]+cnt)/2;
				lpCurPlayer->smCharInfo.Attack_Damage[1] = (lpCurPlayer->smCharInfo.Attack_Damage[1]+cnt)/2;

				cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0  );
				Skil_RangeAttack( lpChar->pX, lpChar->pY, lpChar->pZ, 70*fONE , 
					GetRandomPos( lpCurPlayer->smCharInfo.Attack_Damage[0], lpCurPlayer->smCharInfo.Attack_Damage[1] ) , 0 );

				lpCurPlayer->smCharInfo.Attack_Damage[1] = temp2;
				lpCurPlayer->smCharInfo.Attack_Damage[0] = temp3;

				if ( !cnt )
					lpCurPlayer->AttackCritcal = -1;				//������
				else 
					DamageExp( lpChar , cnt );					//�������� ���� ����ġ ���
*/

				//������ ���� ���� ���� (��)
				dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, 70 , TRUE );
				//�������� ������ ����
				dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ, lpChar ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

				lpCurPlayer->smCharInfo.Attack_Rating = temp;
			}
			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_MAXIMIZE:
	case SKILL_AUTOMATION:
		return TRUE;

	case SKILL_SPARK:
		//����ũ

		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_SPARK;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->HideWeapon = TRUE;
		}
		return TRUE;


	case SKILL_GRAND_SMASH:
		//�׷��彺�Ž� 
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_GRAND_SMASH;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed , 4 );
			lpCurPlayer->MotionLoop = 1;

			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				temp = lpCurPlayer->smCharInfo.Attack_Rating;

				//���߷� ����
				lpCurPlayer->smCharInfo.Attack_Rating += Grand_Smash_AttackRate[lpSkill->Point-1];

				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
				if ( !cnt ) { 
					lpCurPlayer->AttackCritcal = -1;				//������
				}

				lpCurPlayer->smCharInfo.Attack_Rating = temp;
			}

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;



	case SKILL_GROUND_PIKE:
		return TRUE;

	case SKILL_TORNADO:
		//����̵�

		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_TORNADO;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			cnt = GetRandomPos( lpCurPlayer->smCharInfo.Attack_Damage[0],lpCurPlayer->smCharInfo.Attack_Damage[1] );
			cnt += (cnt*Tornado_Damage[lpSkill->Point-1])/100;

			//Skil_RangeAttack( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, (Tornado_Range[lpSkill->Point-1]+8)*fONE , cnt , 0 );

			x = (lpCurPlayer->pX - lpChar->pX)>>FLOATNS;
			z = (lpCurPlayer->pZ - lpChar->pZ)>>FLOATNS;

			y = (int)sqrt( x*x+z*z );
			//if ( y>Tornado_Range[lpSkill->Point-1] ) y=Tornado_Range[lpSkill->Point-1];
			y+=8;
			//y*=fONE;

			//Skil_RangeAttack( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, y , cnt , 0 );

			//������ ���� ���� ���� (��)
			dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, y , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );


		}
		return TRUE;

	case SKILL_CHAIN_LANCE:
		//ü�η���
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_CHAIN_LANCE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed );
			lpCurPlayer->MotionLoop = 1;

			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				//temp = lpCurPlayer->smCharInfo.Attack_Rating;
				//���߷� ����
				//lpCurPlayer->smCharInfo.Attack_Rating += Chain_Lance_AttackRate[lpSkill->Point-1];

				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
				if ( !cnt ) { 
					lpCurPlayer->AttackCritcal = -1;				//������
				}

				//lpCurPlayer->smCharInfo.Attack_Rating = temp;
			}
			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


	case SKILL_ARROW_OF_RAGE:
		//���ο����������
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			//RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_ARROWOF_RAGE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = 120;
			lpCurPlayer->MotionLoop = 1;
		}
		return TRUE;

	case SKILL_FALCON:
		//����
		return TRUE;

	case SKILL_AVALANCHE:
		//�ƹ߶�ü
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

			lpCurPlayer->AttackSkil = SKILL_PLAY_AVALANCHE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->AttackCriticalCount = 0;

			//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , Avalanche_Damage[lpSkill->Point-1] );
			cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

			if ( !cnt ) { 
				lpCurPlayer->AttackCritcal = -1;				//������
			}
			if ( lpCurPlayer->AttackCritcal>0 ) lpCurPlayer->AttackCriticalCount++;
/*
int Avalanche_Speed[10]		= {1,1,1,2,2,2,3,3,3,4};
int Avalanche_Damage[10]	= {8,17,25,31,36,40,43,45,46,47};
int Avalanche_ArrowNum[10]	= {3,3,3,4,4,4,5,5,5,6};
*/
			//���ݼӵ�
			/*
			cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
			if ( cnt<0 ) cnt=0;
			if ( cnt>6 ) cnt=6;
			cnt+=Avalanche_Speed[lpSkill->Point-1];					//�ӵ� +2
			lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;
			*/
			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed , Avalanche_Speed[lpSkill->Point-1] );
			lpCurPlayer->MotionLoop = Avalanche_ArrowNum[lpSkill->Point-1];

			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


	case SKILL_ELEMENTAL_SHOT:
		//������Ż ��
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

			lpCurPlayer->AttackSkil = SKILL_PLAY_ELEMENTAL_SHOT;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			cnt = rand()%2;
			lpCurPlayer->AttackSkil |= ( cnt<<12 );

			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->AttackCriticalCount = 0;

			if ( cnt==0 ) {
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , FALSE );
				if ( !cnt ) { 
					lpCurPlayer->AttackCritcal = -1;				//������
				}
			}
			else {
				//������ ���� ���� ���� (��)
				dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, 75 , FALSE );
				//�������� ������ ����
				dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ, lpChar ,0, 0, 0, 0, lpCurPlayer->AttackSkil );
			}

			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed , 3 );
			lpCurPlayer->MotionLoop = 2;

			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_BOMB_SHOT:
		//�� ��
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

			lpCurPlayer->AttackSkil = SKILL_PLAY_BOMB_SHOT;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->AttackCriticalCount = 0;

			//cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

			//������ ���� ���� ���� (��)
			dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, BombShot_Area[lpSkill->Point-1] , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ, lpChar ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

			lpCurPlayer->MotionLoopSpeed = (GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed )*80)/100;
			lpCurPlayer->MotionLoop = 2;

			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_PERFORATION:
		//�������̼�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

			lpCurPlayer->AttackSkil = SKILL_PLAY_PERFORATION;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->AttackCriticalCount = 0;

			//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , Avalanche_Damage[lpSkill->Point-1] );
			//cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

			rect.left = -40;
			rect.right = 40;
			rect.top = 0;
			rect.bottom = lpCurPlayer->smCharInfo.Shooting_Range+Perforation_Attack_Range[lpSkill->Point-1];
			rect.bottom+=20;
			rect.bottom += 20;

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpCurPlayer , &rect , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed );
			lpCurPlayer->MotionLoop = 1;

			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


	case SKILL_TRIPLE_IMPACT:				
		//Ʈ���� ����Ʈ
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_TRIPLE_IMPACT;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

			switch( T_Impact_Hit[lpSkill->Point-1] ) {
			case 1:
				//1�� ġ��
				cnt = lpCurPlayer->AttackSkil;
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );
				lpCurPlayer->AttackSkil = cnt;				
				break;
			case 2:
				//2��ġ��
				cnt = lpCurPlayer->AttackSkil;
				lpCurPlayer->AttackSkil = SKILL_PLAY_IMPACT;
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->AttackSkil = cnt;				
				break;
			case 3:
				//3��ġ��
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				break;
			}
/*
			//���ݼӵ�
			cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
			if ( cnt<0 ) cnt=0;
			if ( cnt>6 ) cnt=6;
			cnt+=2;					//�ӵ� +2
			lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;
			lpCurPlayer->MotionLoop = 1;
*/

			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , T_Impact_Damage[lpSkill->Point-1] );
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )
					lpCurPlayer->AttackCritcal = -1;				//������
				else {
					//�������� ���� ����ġ ���
					DamageExp( lpChar , cnt );
				}
			}

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_BRUTAL_SWING:				
		//�θ�Ż ����
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_BRUTAL_SWING;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
/*
			//���ݼӵ�
			cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
			if ( cnt<0 ) cnt=0;
			if ( cnt>6 ) cnt=6;
			cnt+=2;					//�ӵ� +2
			lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;
			lpCurPlayer->MotionLoop = 1;
*/
			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				/*
				temp = lpCurPlayer->smCharInfo.Critical_Hit;
				lpCurPlayer->smCharInfo.Critical_Hit += (lpCurPlayer->smCharInfo.Critical_Hit* B_Swing_Critical[lpSkill->Point-1])/100;
				cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , B_Swing_Damage[lpSkill->Point-1] );
				lpCurPlayer->smCharInfo.Critical_Hit = temp;
				*/
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )
					lpCurPlayer->AttackCritcal = -1;				//������
				else {
					//�������� ���� ����ġ ���
					DamageExp( lpChar , cnt );
				}
			}
			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_ROAR:
		return TRUE;

	case SKILL_RAGE_OF_ZECRAM:				
		//��ũ�� ( ���� ȭ �Ӽ� % �谨)
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_RAGEOF_ZECRAM;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
/*
			//���ݼӵ�
			cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
			if ( cnt<0 ) cnt=0;
			if ( cnt>6 ) cnt=6;
			cnt+=2;					//�ӵ� +2
			lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;
			lpCurPlayer->MotionLoop = 1;
*/
			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , R_Zecram_Damage[lpSkill->Point-1] , (sITEMINFO_FIRE+1) );	//�ҼӼ� ����
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )
					lpCurPlayer->AttackCritcal = -1;				//������
				else {
					//�������� ���� ����ġ ���
					DamageExp( lpChar , cnt );
				}
			}
			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_AVANGING_CRASH:				
		//�¡ũ����
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_AVANGING_CRASH;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			temp = lpCurPlayer->smCharInfo.Attack_Rating;

			//���߷� ����
			lpCurPlayer->smCharInfo.Attack_Rating += 
					(lpCurPlayer->smCharInfo.Attack_Rating * A_Crash_AttackRate[lpSkill->Point-1])/100;

			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				cnt = dm_SendTransDamage( lpChar , cSkill.GetSkillPoint(SKILL_BRUTAL_SWING)  ,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )
					lpCurPlayer->AttackCritcal = -1;				//������
				else {
					//�������� ���� ����ġ ���
					DamageExp( lpChar , cnt );
				}
			}

			lpCurPlayer->smCharInfo.Attack_Rating = temp;

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_BONE_CRASH:				
		//�����Ž�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_BONE_SMASH;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )
					lpCurPlayer->AttackCritcal = -1;				//������
				else {
					//�������� ���� ����ġ ���
					DamageExp( lpChar , cnt );
				}
			}

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


	case SKILL_EXPANSION:
		//�ͽ��Ҽ�
		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

			lpCurPlayer->AttackSkil = SKILL_PLAY_EXPANSION;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);


			rect.left = -30;
			rect.right = 30;
			rect.top = 0;
			rect.bottom = LastAttackRange>>FLOATNS;
			rect.bottom += 20;

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpCurPlayer , &rect , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, lpChar ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

/*
			temp = (LastAttackRange*40)/100;			//40%��ġ
			lpCurPlayer->AttackCritcal = 2;
			lpCurPlayer->chrAttackTarget = lpChar;

			GetMoveLocation( 0 , 0, temp , 0, lpCurPlayer->Angle.y , 0 );
			x = lpCurPlayer->pX+GeoResult_X;
			z = lpCurPlayer->pZ+GeoResult_Z;

			cnt = GetRandomPos( lpCurPlayer->smCharInfo.Attack_Damage[0] , lpCurPlayer->smCharInfo.Attack_Damage[1] );
			cnt += (cnt*Expansion_Damage[lpSkill->Point-1])/100;
			Skil_RangeAttack( x, lpCurPlayer->pY, z, temp , cnt , 0 );

			for( cnt=0;cnt<Back_TransSkilAttackData.TargetCount;cnt++) {
				if ( Back_TransSkilAttackData.dwTarObjectSerial[cnt]==lpChar->dwObjectSerial ) break;
			}

			if ( cnt==Back_TransSkilAttackData.TargetCount ) {
				QuickSendTransAttack( lpChar , Back_TransSkilAttackData.Power );
			}
			//ó�� �����ϴ� ���� �������� ���
			if ( lpChar->smCharInfo.State==smCHAR_STATE_ENEMY && lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0]==lpChar->smCharInfo.Life[1] ) {
				lpChar->EnableStateBar = TRUE;
			}
*/
/*
			//������ ���� ���� ���� (��)
			dm_SelectRange( x, lpCurPlayer->pY, z, temp>>FLOATNS , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( x, lpCurPlayer->pY, z, lpChar ,0, 0, 0, 0, lpCurPlayer->AttackSkil );
*/

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill ) {
			if ( RetryPlayAttack( lpChar ) ) break;
			AttCancel = TRUE;					//���� ��Ҹ�� ( �����Ÿ��� ª�� ��� )
		}
		return TRUE;


		///////////////////////////////////// ����Ʈ ///////////////////////////

	case SKILL_SWORD_BLAST:				//�ҵ� ����Ʈ
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_SWORD_BLAST;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
/*
			//���ݼӵ�
			cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
			if ( cnt<0 ) cnt=0;
			if ( cnt>6 ) cnt=6;
			cnt+=2;					//�ӵ� +2
			lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;
			lpCurPlayer->MotionLoop = 1;
*/
			//cnt = GetRandomPos( Sword_Blast_Damage[ lpSkill->Point-1 ][0] , Sword_Blast_Damage[ lpSkill->Point-1 ][1]+(lpCurPlayer->smCharInfo.Level/2) );
			rect.left = -40;
			rect.right = 40;
			rect.top = 0;
			rect.bottom = Sword_Blast_ShootingRange[lpSkill->Point-1]+50;

			//������ ����
			//Skil_RangeBoxAttack( lpCurPlayer , &rect , cnt , 0 , 0 );

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpCurPlayer , &rect , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, lpChar ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_DOUBLE_CRASH:				//���� ũ����
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_DOUBLE_CRASH;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
/*
			//���ݼӵ�
			cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
			if ( cnt<0 ) cnt=0;
			if ( cnt>6 ) cnt=6;
			//cnt+=2;					//�ӵ� +2
			lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;
			lpCurPlayer->MotionLoop = 1;
*/
			//���� ����� �ӵ� ����
			if ( lpSkill->Point ) {
/*
				if ( (rand()%100)<(sinGetCritical( lpChar->smCharInfo.Level )+D_Crash_Critical[lpSkill->Point-1] ))
					lpCurPlayer->AttackCritcal = TRUE;			//ũ��Ƽ�� ���� ����
				else
					lpCurPlayer->AttackCritcal = FALSE;

				cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , D_Crash_Damage[lpSkill->Point-1] );
*/
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )
					lpCurPlayer->AttackCritcal = -1;				//������
				else {
					//�������� ���� ����ġ ���
					DamageExp( lpChar , cnt );
				}
			}

/*
			temp = (LastAttackRange*40)/100;			//40%��ġ
			lpCurPlayer->AttackCritcal = 2;
			lpCurPlayer->chrAttackTarget = lpChar;

			GetMoveLocation( 0 , 0, temp , 0, lpCurPlayer->Angle.y , 0 );
			x = lpCurPlayer->pX+GeoResult_X;
			z = lpCurPlayer->pZ+GeoResult_Z;
	
			cnt = GetRandomPos( lpCurPlayer->smCharInfo.Attack_Damage[0] , lpCurPlayer->smCharInfo.Attack_Damage[1] );
			Skil_RangeAttack( x, lpCurPlayer->pY, z, temp , cnt , 0 );
*/
			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_BRANDISH:
		//�귣��
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_BRANDISH;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , lpChar ,0,0,0 );
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

/*
			cnt = GetRandomPos( lpCurPlayer->smCharInfo.Attack_Damage[0] , lpCurPlayer->smCharInfo.Attack_Damage[1] );
			cnt += (cnt*Brandish_Damage[lpSkill->Point-1])/100;
			Skil_RangeAttack( lpChar->pX, lpChar->pY, lpChar->pZ, Brandish_Range[lpSkill->Point-1]*fONE , cnt , 0 );
*/
			//������ ���� ���� ���� (��)
			dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, Brandish_Range[lpSkill->Point-1] , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ,  0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_PIERCING:
		//�Ǿ��
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_PIERCING;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , 0 ,0,0,0 );
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );


			//���߷� ����
			temp = lpCurPlayer->smCharInfo.Attack_Rating;
			lpCurPlayer->smCharInfo.Attack_Rating += Piercing_Attack_Rating[lpSkill->Point-1];

			//cnt = GetRandomPos( lpCurPlayer->smCharInfo.Attack_Damage[0] , lpCurPlayer->smCharInfo.Attack_Damage[1] );
			//cnt += (cnt*Piercing_Damage[lpSkill->Point-1])/100;
			rect.left = -40;
			rect.right = 40;
			rect.top = 0;
			rect.bottom = 110+30;
			//������ ����
			//Skil_RangeBoxAttack( lpCurPlayer , &rect , cnt , 0 , TRUE );

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpCurPlayer , &rect , TRUE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );


			lpCurPlayer->smCharInfo.Attack_Rating = temp;
			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;
/*
	case SKILL_FLAME_BRANDISH:
		//������ �귣��
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_FLAME_BRANDISH;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , lpChar ,0,0,0 );
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//������ ���� ���� ���� (��)
			dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, 80 , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ,  lpChar ,cSkill.GetSkillPoint(SKILL_BRANDISH), 0, 0, 0, lpCurPlayer->AttackSkil );

			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;
*/
	case SKILL_HOLY_INCANTATION:
		//Ȧ���������̼�
		if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD && lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_HOLY_INCANTATION;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , lpChar ,0,0,0 );
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_GRAND_CROSS:
		//�׷���ũ�ν�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_GRAND_CROSS;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , lpChar ,0,0,0 );
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed , 1 );
			lpCurPlayer->MotionLoop = 1;

			//���߷� 
			temp = lpCurPlayer->smCharInfo.Attack_Rating;

			lpCurPlayer->smCharInfo.Attack_Rating += (lpCurPlayer->smCharInfo.Attack_Rating*Grand_Cross_AttackRate[lpSkill->Point-1])/100;

			cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

			if ( !cnt )	
				lpCurPlayer->AttackCritcal = -1;				//������
			else
				DamageExp( lpChar , cnt );					//�������� ���� ����ġ ���

			//����,���ݷ� ��ġ ����
			lpCurPlayer->smCharInfo.Attack_Rating = temp;

			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


		////////////////////////////////// ��Ż��Ÿ ///////////////////////////

	case SKILL_SHIELD_STRIKE:	
		//�ǵ� ��Ʈ����ũ
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_SHIELD_STRIKE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = 90;
			lpCurPlayer->MotionLoop = 1;

			//cnt = GetRandomPos( S_Strike_Damage[ lpSkill->Point-1 ][0] , S_Strike_Damage[ lpSkill->Point-1 ][1] );
			rect.left = -50;
			rect.right = 50;
			rect.top = 0;
			rect.bottom = 160;

			//������ ����
			//Skil_RangeBoxAttack( lpCurPlayer , &rect , cnt , 5 , 0 );

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpCurPlayer , &rect , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );


			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_FARINA:
		//�ĸ���
		if ( lpCurPlayer->CheckShootingTest(lpChar)==FALSE ) {
			lpCurPlayer->AttackCritcal = -1;				//������
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			return TRUE;
		}

		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

				lpCurPlayer->AttackSkil = SKILL_PLAY_FARINA;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

				temp = lpCurPlayer->smCharInfo.Attack_Rating;

				//���ݼӵ�
				/*
				cnt = lpCurPlayer->smCharInfo.Attack_Speed-6;
				cnt += Farina_Speed[lpSkill->Point-1];
				if ( cnt<0 ) cnt=0;
				if ( cnt>6 ) cnt=6;
				lpCurPlayer->MotionLoopSpeed = (80*(fONE + 32*cnt))>>FLOATNS;
				*/
				lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed , Farina_Speed[lpSkill->Point-1] );
				lpCurPlayer->MotionLoop = 1;


				//���߷� 
				lpCurPlayer->smCharInfo.Attack_Rating += 
					Farina_AttackRate[lpSkill->Point-1] + lpCurPlayer->smCharInfo.Level;

				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 );
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )	
					lpCurPlayer->AttackCritcal = -1;				//������
				else
					DamageExp( lpChar , cnt );					//�������� ���� ����ġ ���

				//����,���ݷ� ��ġ ����
				lpCurPlayer->smCharInfo.Attack_Rating = temp;


			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


	case SKILL_VIGOR_SPEAR:
		//��Ž��Ǿ�
		if ( lpCurPlayer->CheckShootingTest(lpChar)==FALSE ) {
			lpCurPlayer->AttackCritcal = -1;				//������
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			return TRUE;
		}

		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

				lpCurPlayer->AttackSkil = SKILL_PLAY_VIGOR_SPEAR;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

/*
				temp = lpCurPlayer->smCharInfo.Attack_Damage[0];
				temp2 = lpCurPlayer->smCharInfo.Attack_Damage[1];

				cnt = GetRandomPos( V_Spear_Damage[lpSkill->Point-1][0] , V_Spear_Damage[lpSkill->Point-1][1] );
				//���ݷ�
				lpCurPlayer->smCharInfo.Attack_Damage[0] += cnt;
				lpCurPlayer->smCharInfo.Attack_Damage[1] += cnt;
*/
				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 );
				cnt = dm_SendTransDamage( lpChar , wpDamage[0],0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
				if ( !cnt )	
					lpCurPlayer->AttackCritcal = -1;				//������
				else
					DamageExp( lpChar , cnt );					//�������� ���� ����ġ ���
/*
				lpCurPlayer->smCharInfo.Attack_Damage[0] = temp;
				lpCurPlayer->smCharInfo.Attack_Damage[1] = temp2;
*/

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


	case SKILL_TWIST_JAVELIN:
		//Ʈ����Ʈ ���
		if ( lpCurPlayer->CheckShootingTest(lpChar)==FALSE ) {
			lpCurPlayer->AttackCritcal = -1;				//������
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			return TRUE;
		}

		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

				lpCurPlayer->AttackSkil = SKILL_PLAY_TWIST_JAVELIN;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

				temp = lpCurPlayer->smCharInfo.Attack_Rating;
				lpCurPlayer->smCharInfo.Attack_Rating += (lpCurPlayer->smCharInfo.Attack_Rating*Twist_Javelin_Attack_Rating[lpSkill->Point-1])/100;

				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , Twist_Javelin_Damage[lpSkill->Point-1] );
				cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

				if ( !cnt )	
					lpCurPlayer->AttackCritcal = -1;				//������
				else
					DamageExp( lpChar , cnt );					//�������� ���� ����ġ ���

				lpCurPlayer->smCharInfo.Attack_Rating = temp;

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_SOUL_SUCKER:	
		//�ҿ� ��Ŀ
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_SOUL_SUCKER;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = 90;
			lpCurPlayer->MotionLoop = 1;

			rect.left = -30;
			rect.right = 30;
			rect.top = 0;
			rect.bottom = 200;

			//������ ���� ( ����� ��� )
			//Skil_RangeBoxAttack( lpCurPlayer , &rect , lpSkill->Point , 4 , 0 );

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpCurPlayer , &rect , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_FIRE_JAVELIN:
		//���̾� ���
		if ( lpCurPlayer->CheckShootingTest(lpChar)==FALSE ) {
			lpCurPlayer->AttackCritcal = -1;				//������
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			return TRUE;
		}

		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

				lpCurPlayer->AttackSkil = SKILL_PLAY_FIRE_JAVELIN;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

				//�� �Ӽ� ������ �߰�
				//cnt = (GetRandomPos( Fire_Javelin_Damage[lpSkill->Point-1][0],Fire_Javelin_Damage[lpSkill->Point-1][1] ))<<4;
				//cnt |= sITEMINFO_FIRE+1;

				//cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 , 0 ,  cnt );
				dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , FALSE );

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_SPLIT_JAVELIN:
		//���ø� ���
		if ( lpCurPlayer->CheckShootingTest(lpChar)==FALSE ) {
			lpCurPlayer->AttackCritcal = -1;				//������
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			return TRUE;
		}

		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

				lpCurPlayer->AttackSkil = SKILL_PLAY_SPLIT_JAVELIN;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				/////////////////// ���� ���� �̸� ���� ////////////////////

				temp = lpCurPlayer->smCharInfo.Attack_Rating;
				lpCurPlayer->smCharInfo.Attack_Rating += Split_Javelin_AttackRate[lpSkill->Point-1];
				z = Split_Javelin_AttackNum[lpSkill->Point-1];
				x = 0;
				y = sinGetAccuracy( lpChar->smCharInfo.Level , lpChar->smCharInfo.Defence );

				for(cnt=0;cnt<z;cnt++) {
					if ( (rand()%100)<y ) {
						x++;
					}
				}

				if ( x>0 ) {
					dm_SendTransDamage( lpChar , x,0 , 0, 0, lpCurPlayer->AttackSkil , FALSE );
				}
				else {
					lpCurPlayer->AttackCritcal = -1;
				}

				lpCurPlayer->AttackSkil |= ( x<<12 );
				lpCurPlayer->smCharInfo.Attack_Rating = temp;

				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

				lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed , 1 );
				lpCurPlayer->MotionLoopSpeed -= (lpCurPlayer->MotionLoopSpeed*20)/100;	//�ӵ� 20%����
				lpCurPlayer->MotionLoop = 1;
				//lpCurPlayer->MotionLoopSpeed = 200;
				//lpCurPlayer->MotionLoop = 4;

			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_TRIUMPH_OF_VALHALLA:
		if ( lpChar->smCharInfo.State==smCHAR_STATE_USER && 
			GetAssaUseEffect(lpChar,SKILL_TRIUMPH_OF_VALHALLA)==FALSE && 
			GetAssaUseEffect(lpChar,SKILL_HALL_OF_VALHALLA)==FALSE && 
			lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			RetryPlayAttack( lpChar ); //���� ��õ�
			lpCurPlayer->AttackSkil = SKILL_PLAY_TRIUMPH_OF_VALHALLA;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil , 0 , lpChar ,0,0,0 );
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			return TRUE;
		}
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


	case SKILL_LIGHTNING_JAVELIN:
		//����Ʈ�� ���
		if ( lpCurPlayer->CheckShootingTest(lpChar)==FALSE ) {
			lpCurPlayer->AttackCritcal = -1;				//������
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			return TRUE;
		}
		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

			lpCurPlayer->AttackSkil = SKILL_PLAY_LIGHTNING_JAVELIN;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//������ ���� ���� ���� (��)
			dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, 100 , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ,  lpChar ,0, 0, 0, 0, lpCurPlayer->AttackSkil );


			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;




	case SKILL_STORM_JAVELIN:
		//���� ���
		if ( lpCurPlayer->CheckShootingTest(lpChar)==FALSE ) {
			lpCurPlayer->AttackCritcal = -1;				//������
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			return TRUE;
		}

		if ( lpSkill->Point && lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			RetryPlayAttack( lpChar ); //���� ��õ�

				//lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_ATTACK );

				lpCurPlayer->AttackSkil = SKILL_PLAY_STORM_JAVELIN;
				lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
				lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0);


				rect.left = -30;
				rect.right = 30;
				rect.top = 0;
				rect.bottom=300;
				rect.bottom+=20;
				rect.bottom += 20;

				//������ ���� ���� ���� (�ڽ�)
				dm_SelectRangeBox( lpCurPlayer , &rect , FALSE );
				//�������� ������ ����
				dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );


				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed );
				lpCurPlayer->MotionLoop = 2;



			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


		/////////////////////////// ������Ƽ�� //////////////////////////
	case SKILL_HEALING:
		//���� ( ������ ġ�� )
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_HEALING;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = 70;
			lpCurPlayer->MotionLoop = 1;

			dm_SendTransDamage( lpChar , wpDamage[0],wpDamage[1] ,0,0, lpCurPlayer->AttackSkil , FALSE );	


			/*
			cnt = GetRandomPos( Healing_Heal[lpSkill->Point-1][0] , Healing_Heal[lpSkill->Point-1][1] );

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_HEALING;
			smTransCommand.WParam =	cnt;							//�����
			smTransCommand.LParam =	0;								//
			smTransCommand.SParam =	lpChar->dwObjectSerial;			//ġ�� ���
			smTransCommand.EParam = lpCurPlayer->dwObjectSerial;	//
			lpsmSock = GetAreaServerSock();		//���� �������ϱ�
			if ( lpsmSock )
				lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
			*/

		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_HOLY_BOLT:
		//Ȧ�� ��Ʈ
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_HOLY_BOLT;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;

			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( 8 );
			lpCurPlayer->MotionLoop = 1;

/*
			temp  = lpCurPlayer->smCharInfo.Attack_Damage[0];
			temp2 = lpCurPlayer->smCharInfo.Attack_Damage[1];

			if ( sInven[1].ItemIndex ) {
				cnt = GetRandomPos( HolyBolt_Damage[lpSkill->Point-1][0] +  cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0], 
					HolyBolt_Damage[lpSkill->Point-1][1]+ cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1]+(lpCurPlayer->smCharInfo.Level/3) );
			}
			else {
				cnt = GetRandomPos( HolyBolt_Damage[lpSkill->Point-1][0], 
					HolyBolt_Damage[lpSkill->Point-1][1]+(lpCurPlayer->smCharInfo.Level/3) );
			}

			//Undead 100% ����
			if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD )
				cnt += cnt;


			//���ݷ�
			lpCurPlayer->smCharInfo.Attack_Damage[0] = cnt;
			lpCurPlayer->smCharInfo.Attack_Damage[1] = cnt;

			cnt = lpCurPlayer->SendTransAttack( lpChar , smWsockServer , 0 );
*/

			cnt = dm_SendTransDamage( lpChar , wpDamage[0],wpDamage[1] , 0, 0, lpCurPlayer->AttackSkil , FALSE );

			if ( !cnt )	
				lpCurPlayer->AttackCritcal = -1;				//������
			else
				DamageExp( lpChar , cnt );						//�������� ���� ����ġ ���
/*
			lpCurPlayer->smCharInfo.Attack_Damage[0] = temp;
			lpCurPlayer->smCharInfo.Attack_Damage[1] = temp2;
*/
			return TRUE;

		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_MULTISPARK:
		//��Ƽ ����ũ
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			cnt = M_Spark_Num[lpSkill->Point-1];

			cnt = GetRandomPos( (cnt/2)+1 , cnt );

			lpCurPlayer->AttackSkil = SKILL_PLAY_MULTI_SPARK;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->AttackSkil |= (cnt<<12);						//��ų �Ķ���� ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//cnt *= GetRandomPos( M_Spark_Damage[lpSkill->Point-1][0] , M_Spark_Damage[lpSkill->Point-1][1] );
			//QuickSendTransAttack( lpChar , cnt );

			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( 8 );
			lpCurPlayer->MotionLoop = 1;

			cnt = dm_SendTransDamage( lpChar , wpDamage[0],wpDamage[1] , 0, 0, lpCurPlayer->AttackSkil , FALSE );
			if ( cnt )	
				DamageExp( lpChar , cnt );						//�������� ���� ����ġ ���
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_HOLY_MIND:
		//Ȧ�� ���ε�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_HOLY_MIND;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill( lpCurPlayer->AttackSkil,0,lpChar,0,0,0 );

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			lpCurPlayer->MotionLoopSpeed = 70;
			lpCurPlayer->MotionLoop = 1;

			cnt = HolyMind_DecDamage[lpSkill->Point-1];

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_HOLYMIND;
			smTransCommand.WParam =	HolyMind_DecDamage[lpSkill->Point-1];							//�����
			smTransCommand.LParam =	15;		//HolyMind_Time[lpSkill->Point-1];								//
			smTransCommand.SParam =	lpChar->dwObjectSerial;			//ġ�� ���
			smTransCommand.EParam = lpCurPlayer->dwObjectSerial;	//

			lpsmSock = GetAreaServerSock();		//���� �������ϱ�
			if ( lpsmSock )
				lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_DIVINE_LIGHTNING:
	case SKILL_HOLY_REFLECTION:
	case SKILL_GRAND_HEALING:

	case SKILL_RESURRECTION:
	case SKILL_EXTINCTION:
		return TRUE;

	case SKILL_VIRTUAL_LIFE:
		//���x������

		if ( lpChar->smCharInfo.State==smCHAR_STATE_USER && GetAssaUseEffect(lpChar,SKILL_VIRTUAL_LIFE)==FALSE && lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			lpCurPlayer->AttackSkil = SKILL_PLAY_VIRTUAL_LIFE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			SendProcessSKillToServer( SKILL_PLAY_VIRTUAL_LIFE , lpSkill->Point , lpChar->dwObjectSerial , 0 );
			return TRUE;
		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;


	case SKILL_VIGOR_BALL:
		//��ź�
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_VIGOR_BALL;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( 7 );
			lpCurPlayer->MotionLoop = 1;

			DamageExp( lpChar , 1 );						//�������� ���� ����ġ ���

			return TRUE;

		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

		//////////////////////////// ������ /////////////////////////////////////

	case SKILL_FIRE_BOLT:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_FIRE_BOLT;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = 100;
			lpCurPlayer->MotionLoop = 1;

/*
			temp  = lpCurPlayer->smCharInfo.Attack_Damage[0];
			temp2 = lpCurPlayer->smCharInfo.Attack_Damage[1];

			if ( sInven[1].ItemIndex ) {
				cnt = GetRandomPos( FireBolt_Damage[lpSkill->Point-1][0] +  cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0], 
					FireBolt_Damage[lpSkill->Point-1][1]+ cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1]+(lpCurPlayer->smCharInfo.Level/2) );
			}
			else {
				cnt = GetRandomPos( FireBolt_Damage[lpSkill->Point-1][0], 
					FireBolt_Damage[lpSkill->Point-1][1]+(lpCurPlayer->smCharInfo.Level/2) );
			}

			lpCurPlayer->smCharInfo.Attack_Damage[0] = cnt;
			lpCurPlayer->smCharInfo.Attack_Damage[1] = cnt;

			lpCurPlayer->SendTransAttack( lpChar , 0, 0 , 0, (sITEMINFO_FIRE+1) );

			lpCurPlayer->smCharInfo.Attack_Damage[0] = temp;
			lpCurPlayer->smCharInfo.Attack_Damage[1] = temp2;
*/
			cnt = dm_SendTransDamage( lpChar , wpDamage[0],wpDamage[1] ,0, 0, lpCurPlayer->AttackSkil , FALSE );
			if ( cnt )	
				DamageExp( lpChar , cnt );						//�������� ���� ����ġ ���

		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_FIRE_BALL:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_FIRE_BALL;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = 100;
			lpCurPlayer->MotionLoop = 1;

/*
			if ( sInven[1].ItemIndex ) {
				cnt = GetRandomPos( FireBall_Damage[lpSkill->Point-1][0] +  cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0], 
					FireBall_Damage[lpSkill->Point-1][1]+ cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1]);
			}
			else {
				cnt = GetRandomPos( FireBall_Damage[lpSkill->Point-1][0], FireBall_Damage[lpSkill->Point-1][1] );
			}

			cnt>>=1;
			QuickSendTransAttack( lpChar , cnt , 0, (sITEMINFO_FIRE+1) );
			Skil_RangeAttack( lpChar->pX, lpChar->pY, lpChar->pZ, FireBall_Area[lpSkill->Point-1]*fONE , cnt , 100 );//100�� �� �Ӽ�
*/
			//������ ���� ���� ���� (��)
			dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, FireBall_Area[lpSkill->Point-1] , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ,  lpChar , wpDamage[0],wpDamage[1] ,	0, 0, lpCurPlayer->AttackSkil );


		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_WATORNADO:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_COLUMN_OF_WATER;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = 90;
			lpCurPlayer->MotionLoop = 1;
/*
			cnt = GetRandomPos( Watornado_Damage[ lpSkill->Point-1 ][0] , Watornado_Damage[ lpSkill->Point-1 ][1] );
			cnt>>=1;

			QuickSendTransAttack( lpChar , cnt );
			Skil_RangeAttack( lpChar->pX, lpChar->pY, lpChar->pZ, Watornado_Area[ lpSkill->Point-1 ]*fONE , cnt , 104 );//104�� ���� �Ӽ�
*/

			//������ ���� ���� ���� (��)
			dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, Watornado_Area[lpSkill->Point-1] , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ,  lpChar , wpDamage[0],wpDamage[1] ,0,0, lpCurPlayer->AttackSkil );


		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_ENCHANT_WEAPON:
		if ( !lpChar->WeaponEffect && lpChar->PartyFlag && lpChar->smCharInfo.State==smCHAR_STATE_USER &&
			lpSkill->Skill_Info.FuncPointer()==TRUE ) {

			lpCurPlayer->AttackSkil = SKILL_PLAY_ENCHANT_WEAPON;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			cnt = rand()%3;
			lpCurPlayer->AttackSkil |= ( cnt<<12 );
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = 90;
			lpCurPlayer->MotionLoop = 1;
			return TRUE;

		}
		lpCurPlayer->chrAttackTarget = 0;
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;

	case SKILL_DEAD_RAY:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_DEAD_RAY;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			lpCurPlayer->MotionLoopSpeed = 100;
			lpCurPlayer->MotionLoop = 1;
/*
			temp = 0;

			//���� ���� �˻�
			for(cnt=0;cnt<INVENTORY_MAXITEM;cnt++) {
				if( cInvenTory.InvenItem[cnt].Flag && cInvenTory.InvenItem[cnt].ItemPosition==1 ){
					//���ݷ� Ȯ��
					temp = GetRandomPos(  cInvenTory.InvenItem[cnt].sItemInfo.Damage[0] ,  cInvenTory.InvenItem[cnt].sItemInfo.Damage[1] );
					break;
				}
			}

			cnt = GetRandomPos( Dead_Ray_Damage[ lpSkill->Point-1 ][0] , Dead_Ray_Damage[ lpSkill->Point-1 ][1] );
			cnt += temp;

			QuickSendTransAttack( lpChar , cnt , 0, sITEMINFO_NONE );
*/
			cnt = dm_SendTransDamage( lpChar , wpDamage[0],wpDamage[1] , 0, 0, lpCurPlayer->AttackSkil , FALSE );

			if ( cnt )	
				DamageExp( lpChar , cnt );						//�������� ���� ����ġ ���

		}
		//��������
		if ( lpSkill==sinSkill.pLeftSkill && RetryPlayAttack( lpChar ) ) break;
		return TRUE;



		// ��Ÿ ��ų
	case SKILL_HOLY_BODY:
 	case SKILL_DRASTIC_SPIRIT:
	case SKILL_HOLY_VALOR:
	case SKILL_DIVINE_INHALATION:


	case SKILL_WINDY:

	case SKILL_AGONY:
	case SKILL_ZENITH:

	case SKILL_ENERGY_SHIELD:
	case SKILL_DIASTROPHISM:
	case SKILL_SPIRIT_ELEMENTAL:
	case SKILL_DANCING_SWORD:


		return TRUE;


	////////////////////// 4�� ��ų ////////////////////////
	///��ī�ϼ�
	case SKILL_IMPULSION:
		//IMPULSION
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_IMPULSION;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//������ ���� ���� ���� (��)
			dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, Impulsion_Range[lpSkill->Point-1] , TRUE , Impulsion_LightingNum[lpSkill->Point-1] );
			//�������� ������ ����
			dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ,  0 , 0 , 0 ,0,0, lpCurPlayer->AttackSkil );

		}
		return TRUE;

	//������
	case SKILL_DETORYER:
		//DESTROYER
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_DESTROYER;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;

			cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
			if ( !cnt )
				lpCurPlayer->AttackCritcal = -1;				//������

			//�ӵ� +1 ����
			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( lpCurPlayer->smCharInfo.Attack_Speed , 1 );
			lpCurPlayer->MotionLoop = 1;
		}
		return TRUE;

	case SKILL_CYCLONE_STRIKE:
		//CYCLONE_STRIKE
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_CYCLONE_STRIKE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//������ ���� ���� ���� (��)
			dm_SelectRange( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, Cyclone_Strike_Area[lpSkill->Point-1] , TRUE , Cyclone_Strike_AttackNum[lpSkill->Point-1] );
			dm_AddRangeDamage( lpChar );			//���õ� �������� ��Ͽ� ĳ�� ���� �߰�
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ,  lpChar , 0 , 0 ,0,0, lpCurPlayer->AttackSkil );
		}
		return TRUE;
	//����ũ��
	case SKILL_ASSASSIN_EYE:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_ASSASSIN_EYE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;
			dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , FALSE );
		}
		return TRUE;

	case SKILL_CHARGING_STRIKE:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_CHARGING_STRIKE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;
			SkillChargingFlag = 1;

			lpCurPlayer->MotionLoopSpeed = 60+(Charging_Strike_Time[lpSkill->Point-1]*2);
			lpCurPlayer->MotionLoop = 1;

			SkillPlaySound( SKILL_SOUND_SKILL_CHARGING	 , lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ );
		}
		return TRUE;
	case SKILL_SHADOW_MASTER:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_SHADOW_MASTER;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			//temp = lpCurPlayer->smCharInfo.Attack_Rating;
			//lpCurPlayer->smCharInfo.Attack_Rating += Shadow_Master_AddHit[lpSkill->Point-1];				//���߷� ����
			cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
			//lpCurPlayer->smCharInfo.Attack_Rating = temp;
			if ( !cnt )	
				lpCurPlayer->AttackCritcal = -1;				//������

			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( 7 );
			lpCurPlayer->MotionLoop = 1;

		}
		return TRUE;
	//��ó
	case SKILL_PHOENIX_SHOT:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_PHOENIX_SHOT;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;
			SkillChargingFlag = 1;
			lpCurPlayer->MotionLoopSpeed = 60+(Phoenix_Shot_Time[lpSkill->Point-1]*2);
			//lpCurPlayer->MotionLoopSpeed = 80+Phoenix_Shot_Time[lpSkill->Point];
			lpCurPlayer->MotionLoop = 1;
			
			SkillPlaySound( SKILL_SOUND_SKILL_CHARGING	 , lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ );
		}
		return TRUE;
	//����Ʈ
	case SKILL_DIVINE_PIERCING:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_DIVINE_PIERCING;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);

			if ( Divine_Piercing_AttackNum[lpSkill->Point-1]>=7 ) {
				cnt = lpCurPlayer->AttackSkil;
				lpCurPlayer->AttackSkil = SKILL_PLAY_DIVINE_PIERCING3;			//��� �ӽ� ����
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->AttackSkil = cnt;
			}
			else if ( Divine_Piercing_AttackNum[lpSkill->Point-1]>=5 ) {
				cnt = lpCurPlayer->AttackSkil;
				lpCurPlayer->AttackSkil = SKILL_PLAY_DIVINE_PIERCING2;			//��� �ӽ� ����
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
				lpCurPlayer->AttackSkil = cnt;
			}
			else
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );


			//temp = lpCurPlayer->smCharInfo.Attack_Rating;
			//lpCurPlayer->smCharInfo.Attack_Rating += Divine_Piercing_AddHit[lpSkill->Point-1];				//���߷� ����
			cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
			//lpCurPlayer->smCharInfo.Attack_Rating = temp;
			if ( !cnt )	
				lpCurPlayer->AttackCritcal = -1;				//������

			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( 7 );
			lpCurPlayer->MotionLoop = 1;

		}
		return TRUE;
	case SKILL_SWORD_OF_JUSTICE:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_SWORD_OF_JUSTICE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );

			rect.left = -70;
			rect.right = 70;
			rect.top = 0;
			rect.bottom=Sword_Of_Justice_Area[lpSkill->Point-1];
			rect.bottom+=20;
			rect.bottom += 20;

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpCurPlayer , &rect , TRUE );
			dm_AddRangeDamage( lpChar );			//���õ� �������� ��Ͽ� ĳ�� ���� �߰�
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );
		}
		return TRUE;
	//��Ż��Ÿ
	case SKILL_X_RAGE:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_X_RAGE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;


			//������ ���� ���� ���� (��)
			dm_SelectRange( lpChar->pX, lpChar->pY, lpChar->pZ, X_Rage_Area[lpSkill->Point-1] , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ,  0 , 0 , 0 ,0,0, lpCurPlayer->AttackSkil );

		}
		return TRUE;
	case SKILL_VENGEANCE:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_VENGEANCE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;

			lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame( 6 );
			lpCurPlayer->MotionLoop = 1;

			cnt = dm_SendTransDamage( lpChar , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
			if ( !cnt )
				lpCurPlayer->AttackCritcal = -1;				//������
		}


		return TRUE;

	//������Ƽ��
	case SKILL_GLACIAL_SPIKE:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_GLACIAL_SPIKE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;
/*
			rect.left = -30;
			rect.right = 30;
			rect.top = 0;
			rect.bottom=300;
			rect.bottom+=20;
			rect.bottom += 20;

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpCurPlayer , &rect , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,0, 0, 0, 0, lpCurPlayer->AttackSkil );
*/
		}
		return TRUE;

	case SKILL_CHAIN_LIGHTNING:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_CHAIN_LIGHTNING;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;
		}
		return TRUE;

	//������
	case SKILL_FLAME_WAVE:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_FLAME_WAVE;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;

			rect.left = -60;
			rect.right = 60;
			rect.top = 0;
			rect.bottom=300;
			rect.bottom+=20;
			rect.bottom += 20;

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpCurPlayer , &rect , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0 ,wpDamage[0],wpDamage[1],  0, 0, lpCurPlayer->AttackSkil );

		}
		return TRUE;
	case SKILL_M_METEO:
		if ( lpSkill->Skill_Info.FuncPointer()==TRUE ) {
			lpCurPlayer->AttackSkil = SKILL_PLAY_METEO;
			lpCurPlayer->AttackSkil |= (lpSkill->Point<<8);				//��ų ����Ʈ ���� ����Ʈ�� ����
			lpCurPlayer->BeginSkill(lpCurPlayer->AttackSkil,0,lpChar,0,0,0);
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_SKILL );
			//lpCurPlayer->HideWeapon = TRUE;
		}
		return TRUE;

		// ��Ÿ ��ų
	case SKILL_COMPULSION:
	case SKILL_MAGNETIC_SPHERE:		//��̼ҵ�� ����ϴٰ� ��
	case SKILL_METAL_GOLEM:
	case SKILL_BERSERKER:
	case SKILL_VAGUE:
	case SKILL_RECALL_WOLVERIN:
	case SKILL_FORCE_OF_NATURE:
	case SKILL_GODLY_SHIELD:
	case SKILL_GOD_BLESS:
	case SKILL_HALL_OF_VALHALLA:
	case SKILL_FROST_JAVELIN:
	case SKILL_REGENERATION_FIELD:
	case SKILL_SUMMON_MUSPELL:
	case SKILL_FIRE_ELEMENTAL:
	case SKILL_DISTORTION:


		return TRUE;


	}

	if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_ATTACK && lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_SKILL ) {
		RetryPlayAttack( lpChar );
	}

	return FALSE;
}

//��ų ���� ���� �õ�
int PlaySkillLoop( int SkillCode , smCHAR *lpChar )
{
	int point,param;
	int	cnt;
	smCHAR	*chrAttackTarget;
	RECT	rect;

	point = (SkillCode>>8)&0xF;
	param = (SkillCode>>12)&0xF;

	switch( SkillCode&0xFF ) {

	case SKILL_PLAY_CRITICAL_HIT:
		//ũ��Ƽ�� ��
		if ( lpChar->chrAttackTarget && point ) {
			if ( !lpChar->chrAttackTarget->smCharInfo.Life[0] ) {
				lpChar->MotionLoop = 0;			//���ӵ��� ���
				break;
			}
/*
			//ũ��Ƽ�� �缳��
			cnt = lpChar->smCharInfo.Critical_Hit;
			lpCurPlayer->smCharInfo.Critical_Hit += Critical_Hit_Critical[point-1];

			if ( (rand()%100)<sinGetCritical(lpChar->smCharInfo.Level)) {
				lpChar->AttackCritcal = TRUE;			//ũ��Ƽ�� ���� ����
			}
			else {
				lpChar->AttackCritcal = FALSE;
			}
			lpChar->smCharInfo.Critical_Hit = cnt;

			cnt = lpChar->SendTransAttack( lpChar->chrAttackTarget , smWsockServer , 0  );
*/
			cnt = dm_SendTransDamage( lpChar->chrAttackTarget , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
			if ( !cnt ) 
				lpChar->AttackCritcal = -1;
			else  {
				lpChar->AttackCritcal = 0;
				DamageExp( lpChar->chrAttackTarget , cnt );					//�������� ���� ����ġ ���
			}

			//lpChar->Angle.y = GetRadian2D( lpChar->pX,lpChar->pZ , lpChar->chrAttackTarget->pX, lpChar->chrAttackTarget->pZ );
			//lpChar->SetMotionFromCode(CHRMOTION_STATE_ATTACK);
		}
		break;

	case SKILL_PLAY_RAVING:				//���̺�
		if ( lpChar->chrAttackTarget && point ) {

			if ( !lpChar->chrAttackTarget->smCharInfo.Life[0] ) {
				lpChar->MotionLoop = 0;			//���ӵ��� ���
				break;
			}
/*
			if ( (rand()%100)<sinGetCritical( lpChar->chrAttackTarget->smCharInfo.Level ) )
				lpChar->AttackCritcal = TRUE;			//ũ��Ƽ�� ���� ����
			else
				lpChar->AttackCritcal = FALSE;

			cnt = lpChar->SendTransAttack( lpChar->chrAttackTarget , smWsockServer , 0 , Raving_Damage[point-1] );
*/
			cnt = dm_SendTransDamage( lpChar->chrAttackTarget , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );

			if ( !cnt ) lpChar->AttackCritcal = -1;
			else {
				//������ ����
				lpChar->AttackCritcal = 0;
				PlaySkillSubLife( ((int)(Raving_UseLife[point-1]*10)*lpChar->smCharInfo.Life[1])/1000 );
				DamageExp( lpChar->chrAttackTarget , cnt );					//�������� ���� ����ġ ���
			}

			lpChar->AttackAnger = 150;
			lpChar->Angle.y = GetRadian2D( lpChar->pX,lpChar->pZ , lpChar->chrAttackTarget->pX, lpChar->chrAttackTarget->pZ );
			//lpCurPlayer->AttackCritcal = 1;
		}
		break;

	case SKILL_PLAY_AVALANCHE:
			//�ƹ߶�ü
		if ( lpChar->chrAttackTarget && point ) {

			if ( !lpChar->chrAttackTarget->smCharInfo.Life[0] ) {
				lpChar->MotionLoop = 0;			//���ӵ��� ���
				break;
			}
			/*
			if ( (rand()%100)<sinGetCritical( lpChar->chrAttackTarget->smCharInfo.Level ) && lpChar->AttackCriticalCount==0 ) {
				lpChar->AttackCritcal = TRUE;			//ũ��Ƽ�� ���� ����
				lpChar->AttackCriticalCount++;
			}
			else
				lpChar->AttackCritcal = FALSE;

			cnt = lpChar->SendTransAttack( lpChar->chrAttackTarget , smWsockServer , 0 , Avalanche_Damage[point-1] );
			if ( !cnt ) lpChar->AttackCritcal = -1;
			*/

			cnt = dm_SendTransDamage( lpChar->chrAttackTarget , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
			if ( !cnt ) 
				lpChar->AttackCritcal = -1;
			else { 
				lpChar->AttackCritcal = 0;
				DamageExp( lpChar->chrAttackTarget , cnt );					//�������� ���� ����ġ ���
			}


			lpChar->Angle.y = GetRadian2D( lpChar->pX,lpChar->pZ , lpChar->chrAttackTarget->pX, lpChar->chrAttackTarget->pZ );
			//lpCurPlayer->AttackCritcal = 1;
		}
		break;


	case SKILL_PLAY_ELEMENTAL_SHOT:
		if ( lpChar->chrAttackTarget && point ) {

			if ( !lpChar->chrAttackTarget->smCharInfo.Life[0] ) {
				lpChar->MotionLoop = 0;			//���ӵ��� ���
				break;
			}

			chrAttackTarget = lpChar->chrAttackTarget;

			if ( param==0 ) {	//��
				cnt = dm_SendTransDamage( lpChar->chrAttackTarget , 0,0 , 0, 0, lpChar->AttackSkil , FALSE );
				if ( !cnt ) { 
					lpChar->AttackCritcal = -1;				//������
				}
				else {
					lpChar->AttackCritcal = 0;
				}
			}
			else {					//����
				//������ ���� ���� ���� (��)
				dm_SelectRange( chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 75 , FALSE );
				//�������� ������ ����
				dm_SendRangeDamage( chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, chrAttackTarget ,0, 0, 0, 0, lpChar->AttackSkil );
			}
		}
		break;

	case SKILL_PLAY_STORM_JAVELIN:
		if ( lpChar->chrAttackTarget && point ) {

			chrAttackTarget = lpChar->chrAttackTarget;

			rect.left = -30;
			rect.right = 30;
			rect.top = 0;
			rect.bottom=300;
			rect.bottom+=20;
			rect.bottom += 20;

			//������ ���� ���� ���� (�ڽ�)
			dm_SelectRangeBox( lpChar , &rect , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( lpChar->pX, lpChar->pY, lpChar->pZ, 0 ,0, 0, 0, 0, lpChar->AttackSkil );
		}
		break;

	case SKILL_PLAY_BOMB_SHOT:
		if ( lpChar->chrAttackTarget && point ) {

			chrAttackTarget = lpChar->chrAttackTarget;

			//������ ���� ���� ���� (��)
			dm_SelectRange( chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, BombShot_Area[point-1] , FALSE );
			//�������� ������ ����
			dm_SendRangeDamage( chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, chrAttackTarget ,0, 0, 0, 0, lpChar->AttackSkil );
		}
		break;

	}

	return TRUE;
}


//�ܻ�� ��ų���� ����
int SetSkillMotionBlurColor( int dwSkillCode )
{
	switch( dwSkillCode&0xFF ) {
	case SKILL_PLAY_CRITICAL_HIT:
		//ũ��Ƽ�� ��
		smRender.Color_R += 256;
		smRender.Color_G += -64;
		smRender.Color_B += 256;
		return TRUE;

	case SKILL_PLAY_RAVING:
		//���̺�
		smRender.Color_R += 256;
		smRender.Color_G += -64;
		smRender.Color_B += -64;
		return TRUE;

	case SKILL_PLAY_IMPACT:
		//����Ʈ
		smRender.Color_R += 256;
		smRender.Color_G += 256;
		smRender.Color_B += -64;
		return TRUE;

	case SKILL_PLAY_TRIPLE_IMPACT:
		//Ʈ���� ����Ʈ
		smRender.Color_R += 256;
		smRender.Color_G += -64;
		smRender.Color_B += 256;
		return TRUE;

	case SKILL_PLAY_BRUTAL_SWING:
		//Ʈ���� ����Ʈ
		smRender.Color_R += -64;
		smRender.Color_G += 256;
		smRender.Color_B += 128;
		return TRUE;

	case SKILL_PLAY_CHAIN_LANCE:
		//ü�η���
		smRender.Color_R += 256;
		smRender.Color_G -= 64;
		smRender.Color_B -= 64;
		break;

	}

	return FALSE;
}


//���� ��ų Ȯ��
int GetSkillPower( smCHAR *lpChar , smCHAR *lpChar2 , int dx, int dy, int dz )
{
	int x,y,z,dist;

	switch(  lpChar->HoSkillCode ) {
	case SKILL_SCOUT_HAWK:
		if ( Skill_ScoutHawk.Point ) {
			x = (lpChar->HoSkillPos.x - lpChar2->pX)>>FLOATNS;
			y = (lpChar->HoSkillPos.y - lpChar2->pY)>>FLOATNS;
			z = (lpChar->HoSkillPos.z - lpChar2->pZ)>>FLOATNS;
			dist = x*x+y*y+z*z;

			if ( dist<(512*512) ) 
				return Scout_Hawk_Attack_Rate[ Skill_ScoutHawk.Point-1 ];
		}
		break;
	}

	return NULL;
}

//��ų ���� ����Ʈ
int	LearnSkillEffect( smCHAR *lpChar )
{
	if ( lpChar ) {
		StartSkill( lpChar->pX,lpChar->pY,lpChar->pZ, 0,0,0, SKILL_UP1 );
		SkillPlaySound( SKILL_SOUND_LEARN , lpChar->pX,lpChar->pY,lpChar->pZ );			//��ų ȿ����
		//StartEffect( lpChar->pX,lpChar->pY,lpChar->pZ, EFFECT_PHYSICAL_ABSORB_DAMAGE );
	}
	else {
		StartSkill( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 0,0,0, SKILL_UP1 );
		SkillPlaySound( SKILL_SOUND_LEARN , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );			//��ų ȿ����
		//StartEffect( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, EFFECT_PHYSICAL_ABSORB_DAMAGE );
	}

	return TRUE;
}

int LastSkillMastery = (-10000*SIN_MAX_USE_SKILL) ^ ((DWORD)LearnSkillEffect);

//��ų����
int CheckPlaySkill()
{
	int cnt;
	int point;
	int OverPoint;
	int SkillCount = 0;
	int	LastSkillCount;
	int SkillError = 0;
	int	SubMastery;
	smTRANS_COMMAND	smTransCommand;

	point = sinSkill.SkillPoint;

	for(cnt=1;cnt<13;cnt++) {
		point += sinSkill.UseSkill[cnt].Point;
		SkillCount += sinSkill.UseSkill[cnt].UseSkillCount;
		if ( sinSkill.UseSkill[cnt].Point>10 ) point += 16384;
	}

	LastSkillCount = LastSkillMastery^((DWORD)LearnSkillEffect);
	LastSkillCount = -LastSkillCount;

	SubMastery = SkillCount-LastSkillCount;
	if ( (SubMastery)>300 ) //��ų���õ� 300 �̻� ������
	{	
		// ����� - ��ų ������ �������� ����� ���� skillError�� �ƴϹǷ�
		if(cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.CODE==(sinBI1|sin39) ||
		   cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.CODE==(sinBI1|sin40) ||
		   cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.CODE==(sinBI1|sin41) )
		{
			SkillError = 0;
		}
		else
			SkillError = 2;

		smTransCommand.LParam = SubMastery;
	}

	LastSkillMastery = (-SkillCount)^((DWORD)LearnSkillEffect);

	if ( lpCurPlayer->smCharInfo.Level>=10 ) {
		OverPoint = ((lpCurPlayer->smCharInfo.Level-8)/2)+1;	//-point;
		OverPoint +=  GetSkillPoint_LevelQuest( lpCurPlayer->smCharInfo.Level , sinQuest_levelLog );	//����Ʈ�� ȹ���� ��ų ����Ʈ

		if ( OverPoint<point ) {
			//��ų ����Ʈ ����
			SkillError = 1;
			smTransCommand.LParam = point;
		}
	}
	else {
		if ( point>0 ) {
			SkillError = 1;
			smTransCommand.LParam = point;
		}
	}

	if ( SkillError ) {
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_WARNING_SKILL;
		smTransCommand.WParam = SkillError;
		//smTransCommand.LParam = SubMastery;
		smTransCommand.SParam = lpCurPlayer->smCharInfo.Level;
		smTransCommand.EParam = 0;
		smTransCommand.EParam = dm_GetCommandChkSum( &smTransCommand , lpCurPlayer->dwObjectSerial );

		#ifdef _USE_DYNAMIC_ENCODE
			if ( fnEncodeDamagePacket ) 
			{
				fnEncodeDamagePacket( &smTransCommand );			//��ȣȭ

				if ( smWsockDataServer )
					smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
			}
		#else
			dm_EncodePacket(&smTransCommand);					//��ȣȭ

			if (smWsockDataServer) 
				smWsockDataServer->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
		#endif
	}


	return TRUE;
}






