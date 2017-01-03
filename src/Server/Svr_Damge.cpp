#include <windows.h>
#include <io.h>
#include <stdio.h>         /* for sprintf                           */
#include <string.h>        /* for strlen                            */
#include <memory.h>
#include <process.h>       /* for _beginthread                      */

#include "common.h"

#include "..\\smwsock.h"
#include "..\\smlib3d\\smd3d.h"
#include "..\\character.h"
#include "..\\fileread.h"
#include "..\\playmain.h"
#include "..\\field.h"
#include "..\\sinbaram\\sinlinkheader.h"
#include "..\\effectsnd.h"

#include "..\\record.h"
#include "..\\playsub.h"
#include "Language\\language.h"

#include "onserver.h"
#include "svr_Damage.h"
#include "..\\damage.h"
#ifdef _LANGUAGE_VEITNAM
#include "..\\veitnam\\v_event.h"
#endif

#define	PK_POWER_DIVIDE	5
#define	PK_SCORE_DIVIDE	10

//������ �ʵ� ��ǥ ���� ( ���߿� ������Ϸ� �������� )
extern int CastleBattleZone_LineZ;


//�⺻ ���� ������
WORD	rsDefaultDamage[10][2] = {
	{	7,	14	},
	{	17,	40	},
	{	25,	50	},
	{	45,	74	},
	{	50,	80	},
	{	55,	86	},
	{	60,	90	},
	{	65,	100	},
	{	70,	110	},
	{	70,	110	}
};

int	dm_Critical_Temp;			//ũ��Ƽ�� �� �ӽ� ����
int dm_SkillCode_Temp;			//��ų�ڵ� �ӽú���
//���� ũ��Ƽ�� 
int GetCritical( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , int Critical_Hit );
int GetCritical2( rsPLAYINFO *lpPlayInfo , rsPLAYINFO *lpPlayInfo2 , int Critical_Hit );

//ĳ���� ������ ���
int	rsRecordCharDamage( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , int Damage );
//ĳ���� PK ���
int	rsRecord_PKDamage( rsPLAYINFO *lpPlayInfo , rsPLAYINFO *lpPlayInfo2 , int Damage );


//�ٸ� ���� ���� ��Ŷ ������ ( ���� ���� )
int rsSendAttackUser( rsPLAYINFO *lpPlayInfo , rsPLAYINFO *lpPlayInfo2, int Power , int AttackState , short sParam1=0 , short sParam2=0 );

//���������۽�ų����
int	rsBillingItemSkill( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , int HitMonsters , int Power , int SkillCode );


#define	DISP_DAMAGE

#ifdef DISP_DAMAGE
static char szDispDamage[128];
static char szDispDamage2[128];	//�ؿ�

//������ ���� ���
int rsDisplayDamgeToClient( rsPLAYINFO *lpPlayInfo , char *szDmgMsg )
{
	TRANS_CHATMESSAGE	TransChatMessage;

	if ( lpPlayInfo->AdminMode<=2 ) return FALSE;

	lstrcpy( TransChatMessage.szMessage , szDmgMsg );
	TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
	TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
	TransChatMessage.dwIP = 0;
	TransChatMessage.dwObjectSerial = 0;
	lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );

	return TRUE;
}
#endif


//���� ���� ����
int rsRecvAttackData_Old( rsPLAYINFO *lpPlayInfo , TRANS_ATTACKDATA *lpTransAttackData , smCHAR *lpTargetChar , int Type )
{
#ifdef	_W_SERVER

	DWORD	dwTime;
	TRANS_CHATMESSAGE	TransChatMessage;
	smCHAR	*lpChar;
	smTRANS_COMMAND		smTransCommand;
	int		x,y,z;
	int		cnt,len;
	int		ang,ang2;
	int		regs;
	smWINSOCK	*lpsmSock;
	rsPLAYINFO	*lpPlayInfo2;

	int	Dmg1,Dmg2,Dmg3,Dmg4;

	Dmg1=0;	Dmg2=0;	Dmg3=0; Dmg4=0;

	lpsmSock = lpPlayInfo->lpsmSock;
	dwTime = dwPlayServTime;

		//���� ����Ÿ ���
		lpPlayInfo->Recv_AttackCount++;
		lpPlayInfo->Recv_AttackDamage +=lpTransAttackData->Power;

		if ( lpPlayInfo->BadPlayer==TRUE ) return FALSE;
		if ( lpPlayInfo->HideMode && lpPlayInfo->AdminMode<4 ) return FALSE;

		if ( lpTargetChar ) lpChar = lpTargetChar;
		else lpChar = srFindCharFromSerial( lpTransAttackData->dwTarObjectSerial );

		if ( !lpChar || lpChar->smCharInfo.State!=smCHAR_STATE_NPC ) {
			lpPlayInfo->AttackCount++;
		}

		if ( (dwPlayServTime-lpPlayInfo->dwAttackTime)>5000 ) {
			//���� Ÿ�̹� ���
			lpPlayInfo->dwAttackTime = dwPlayServTime;
			if ( lpPlayInfo->AttackCount>=40 ) {
				//5�ʵ��� 10ȸ �̻� ���� ����

				smTransCommand.WParam = 1823;
				smTransCommand.SParam = lpPlayInfo->AttackCount;
				smTransCommand.LParam = lpPlayInfo->AttackCount;

				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
				rsSendDataServer( lpPlayInfo->lpsmSock , &smTransCommand );

				if ( lpPlayInfo->AttackCount>=80 )
					lpPlayInfo->BadPlayer = TRUE;
					//DisconnectUser( lpsmSock );			//30ȸ �̻� ���� ��� ���� ����

				return FALSE;
			}
			lpPlayInfo->AttackCount = 0;
		}

		if ( !Type && lpTransAttackData->dwChkSum!=(DWORD)(lpTransAttackData->Power*2002+(lpTransAttackData->x*lpTransAttackData->y*lpTransAttackData->z))) {
			//ýũ�� Ȯ�� ����
			smTransCommand.WParam = 1820;
			smTransCommand.SParam = lpTransAttackData->Power;
			smTransCommand.LParam = lpTransAttackData->Power;
			//RecordHackLogFile( lpPlayInfo , &smTransCommand );

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			rsSendDataServer( lpPlayInfo->lpsmSock , &smTransCommand );

			//DisconnectUser( lpsmSock );
			lpPlayInfo->BadPlayer = TRUE;
			return FALSE;
		}

		if ( lpTransAttackData->Power>=9000 ) {
			//���ݷ� ũ�� �͹��� ����
			smTransCommand.WParam = 1821;
			smTransCommand.SParam = lpTransAttackData->Power;
			smTransCommand.LParam = lpTransAttackData->Power;
			//RecordHackLogFile( lpPlayInfo , &smTransCommand );

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			rsSendDataServer( lpPlayInfo->lpsmSock , &smTransCommand );

			//DisconnectUser( lpsmSock );
			lpPlayInfo->BadPlayer = TRUE;
			return FALSE;
		}


		if ( abs(( (long)(dwTime-lpPlayInfo->dwRecvTimePlayBuff) ))>7*1000 ) {			// pluto ����ȯ
			//7�� �̻� ��� ����Ÿ ������ ������ ��� ( ������������ ���� )
			return FALSE;
		}

		//�ϵ��ھ� �̺�Ʈ�� ( �̺�Ʈ ���͸� ���� �ۿ��� �����ϸ� ��ȿ )
		if ( lpChar && lpChar->PartyFlag==rsHARDCORE_EVENT_FLAG ) {
			if ( lpPlayInfo->EventMode!=rsHARDCORE_EVENT_FLAG ) {
				return FALSE;
			}
			/*
			if ( lpPlayInfo->EventScore>100000 ) {
				lpTransAttackData->Power>>=1;			//10�� �̻� ���� ȹ���� ��� ���ݷ��� ������ ���� ( �ʹ� ���ؼ� �ǽ� )
			}
			*/
		}

		if ( lpChar ) {
			lpChar->dwLastTransTime = dwPlayServTime;		//�ֱ� Ȱ��ȭ�� �ð�

			x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
			y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
			z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
			if ( abs(y)>100 || (x*x+z*z)>(800*800) ) return FALSE;		//�Ÿ��� �հ�� ����

			x = (lpPlayInfo->Position.x-lpTransAttackData->x)>>FLOATNS;
			y = (lpPlayInfo->Position.y-lpTransAttackData->y)>>FLOATNS;
			z = (lpPlayInfo->Position.z-lpTransAttackData->z)>>FLOATNS;
			//if ( (x*x+y*y+z*z)>(64*8)*(64*8) ) break;					

			if ( lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD && lpChar->MotionInfo->State!=CHRMOTION_STATE_WARP ) {
				
				Dmg1 = lpTransAttackData->Power;

				//�ϴ� ���� ��츸..
				if ( lpChar->smCharInfo.State ) {

#ifndef	_LANGUAGE_KOREAN
							//ĳ���Ϳ� ���Ϳ��� �������� ū��� ���� ����
							cnt = lpChar->smCharInfo.Level-lpPlayInfo->smCharInfo.Level;
							if ( cnt>20 ) {
								cnt += (rand()%(cnt-20));
								if ( cnt>25 ) {
									if ( lpPlayInfo->smCharInfo.Level<10 ) cnt=20;
									else cnt = lpPlayInfo->smCharInfo.Level * 2;

									if ( cnt<lpTransAttackData->Power ) {
										lpTransAttackData->Power = GetRandomPos( cnt/2 , cnt );
									}
									if ( (lpTransAttackData->AttackState&0xFFFF)>1 ) {
										lpTransAttackData->AttackState &=0xFFFF0000;
									}
								}
							}
#endif

							if ( lpPlayInfo->BadPlayer==2 || lpPlayInfo->BadPlayer==4 ) {
								//��ŷ ���� ���ݷ� ����
								cnt = lpPlayInfo->smCharInfo.Level/10;
								if ( cnt<1 || cnt>8 ) cnt = 1;

								cnt = GetRandomPos( rsDefaultDamage[cnt][0] , rsDefaultDamage[cnt][1] );
								if ( cnt<lpTransAttackData->Power ) {
									lpTransAttackData->Power = cnt;
								}
							}

							//�ֱ� ��ǥ ���� ���
							lpPlayInfo->lpChrTarget = lpChar;
							lpPlayInfo->dwTarObjSerial = lpChar->dwObjectSerial;

							lpChar->PlayStunCount = 0;			//�߸���� ����

							//��þƮ ���� ��ų ����
							if ( lpPlayInfo->dwSkill_EnchantWeapon_Time ) {
								if ( lpPlayInfo->dwSkill_EnchantWeapon_Time>dwPlayServTime ) {
									if ( ((lpTransAttackData->AttackState>>16)&0xF)==0 ) {
										len = lpPlayInfo->dwSkill_EnchantWeapon_Param&0xFF;		//����Ʈ
										switch(lpPlayInfo->dwSkill_EnchantWeapon_Param>>8) {
											case 0:		//���� ( ���������� ���� �����Ƿ� ����)
												/*
												lpTransAttackData->Power += GetRandomPos(
													Enchant_Weapon_Damage_Ice[len-1][0] , Enchant_Weapon_Damage_Ice[len-1][1] );
													*/

												// �庰 - ��ų ����
												lpTransAttackData->Power += GetRandomPos(
													Enchant_Weapon_Damage_Ice[len-1][0] , Enchant_Weapon_Damage_Ice[len-1][1] );
												lpTransAttackData->AttackState |= (sITEMINFO_ICE+1)<<16;
												lpTransAttackData->AttackState |= (Enchant_Weapon_Damage_Ice[len-1][1])<<(16+4);
													
											//	lpTransAttackData->AttackState |= (sITEMINFO_ICE+1)<<16;
											//	lpTransAttackData->AttackState |= (1)<<(16+4);
												break;
											case 1:		//����
												lpTransAttackData->Power += GetRandomPos(
													Enchant_Weapon_Damage_Lightning[len-1][0] , Enchant_Weapon_Damage_Lightning[len-1][1] );
												lpTransAttackData->AttackState |= (sITEMINFO_LIGHTING+1)<<16;
												lpTransAttackData->AttackState |= (Enchant_Weapon_Damage_Lightning[len-1][1])<<(16+4);
												break;

											case 2:		//��
												lpTransAttackData->Power += GetRandomPos(
													Enchant_Weapon_Damage_Fire[len-1][0] , Enchant_Weapon_Damage_Fire[len-1][1] );
												lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;
												lpTransAttackData->AttackState |= (Enchant_Weapon_Damage_Fire[len-1][1])<<(16+4);
												break;
										}
									}
								}
								else {	//���� ( �ð��ʰ� )
									lpPlayInfo->dwSkill_EnchantWeapon_Time = 0;
									lpPlayInfo->dwSkill_EnchantWeapon_Param = 0;
								}
							}

							//�Ӽ� ������ ���
							regs = (lpTransAttackData->AttackState>>16)&0xF;
							if ( regs && regs<sITEMINFO_NONE ) {
								cnt = lpTransAttackData->AttackState>>(16+4);		//�Ӽ� �� ( ���� ��Ʈ�� ��ü �������� �Ӽ� �������� ����� )
								if ( !cnt ) 
									cnt = lpTransAttackData->Power;

								//�Ӽ��� ���� ���׷� ����
								len = lpChar->smCharInfo.Resistance[regs-1];
								if ( len ) {					//�Ӽ� ���׷� ���
									if ( len>=100 ) len = 100;
									if ( len<=-100 ) len = -100;
									lpTransAttackData->Power -= ((cnt*len)/100);

									if ( lpTransAttackData->Power<0 ) lpTransAttackData->Power=0;
								}

								switch(regs-1) { //(lpTransAttackData->AttackState>>16)-1) {
									case sITEMINFO_ICE:
										if ( lpPlayInfo->dwSkill_EnchantWeapon_Time>dwPlayServTime && (lpPlayInfo->dwSkill_EnchantWeapon_Param>>8)==0 ) {
											//���� ���� �ӵ� ���� ( ��üƮ �϶� ���� )
											lpChar->PlaySlowCount = 16*3;									//�ӵ� ������ ( 3�� )
											lpChar->PlaySlowCount -= (lpChar->PlaySlowCount*len)/100;
											lpChar->PlaySlowSpeed = 230-10*(lpPlayInfo->dwSkill_EnchantWeapon_Param&0xFF);	//������ ���� �ӵ�
											lpChar->PlayDistortion = 0;			//����� ����
											break;
										}
										if ( lpPlayInfo->dwSkill_DancingSword_Time>dwPlayServTime && (lpPlayInfo->dwSkill_DancingSword_Param&0xFF)==0 ) {
											//��̼ҵ� ���� ����
											lpChar->PlaySlowCount = 16*5;					//�ӵ� ������ ( 5�ʿ��� ��� )
											lpChar->PlaySlowCount -= (lpChar->PlaySlowCount*len)/100;
											lpChar->PlaySlowSpeed = 230-10*(lpPlayInfo->dwSkill_DancingSword_Param>>16);	//������ ���� �ӵ�
											lpChar->PlayDistortion = 0;			//����� ����
											break;
										}
										if ( dm_SkillCode_Temp==SKILL_PLAY_PET_ATTACK ) {
											//�� ��������
											lpChar->PlaySlowCount = 16*10;					//�ӵ� ������ ( 10�ʿ��� ��� )
											lpChar->PlaySlowCount -= (lpChar->PlaySlowCount*len)/100;
											lpChar->PlaySlowSpeed = 180;
											lpChar->PlayDistortion = 0;			//����� ����
											break;
										}
										if ( lpPlayInfo->dwSkill_FrostJavelin_Time>dwPlayServTime ) {
											//���ν�Ʈ��� ��������
											//�ӵ� ������
											lpChar->PlaySlowCount = 16*Frost_Javelin_IceTime[lpPlayInfo->dwSkill_FrostJavelin_Param];
											lpChar->PlaySlowCount -= (lpChar->PlaySlowCount*len)/100;
											lpChar->PlaySlowSpeed = 240-(240*Frost_Javelin_SpeedSubPercent[lpPlayInfo->dwSkill_FrostJavelin_Param])/100;
											lpChar->PlayDistortion = 0;			//����� ����
											break;
										}
										break;

								}

							}

							cnt = ( lpTransAttackData->Power * lpChar->smCharInfo.Absorption )/100;
							cnt = lpTransAttackData->Power-cnt;

							Dmg2 = lpTransAttackData->Power;
							Dmg3 = cnt;
							Dmg4 = cnt;

							//�ڱ⺸�� ������ ū ���͸� �ѹ濡 ���̴� ���
							if ( (lpChar->smCharInfo.Life[1]/4)<cnt &&
								lpChar->smCharInfo.Level>lpPlayInfo->smCharInfo.Level ) {

								cnt = GetRandomPos( lpChar->smCharInfo.Life[1]/8 , lpChar->smCharInfo.Life[1]/4 );
							}

							if ( cnt>0 ) {
								//�� ������ ��Ͻ��� ����ġ �켱 ȹ��
								if ( lpChar->smCharInfo.Life[0]==lpChar->smCharInfo.Life[1] ) {
									if ( !lpChar->lpExpAttackPlayInfo ) {
										//������ ���
										lpChar->lpExpAttackPlayInfo = lpPlayInfo;
										lpChar->ExpAttackLife = 0;
										lpChar->dwExpAttackTime = dwTime;
									}
								}
								if ( lpChar->lpExpAttackPlayInfo ) {
									if ( lpChar->lpExpAttackPlayInfo==lpPlayInfo ) {
										//�������� ���� ���
										lpChar->ExpAttackLife += cnt;
										lpChar->dwExpAttackTime = dwTime;
									}
									else {
										if ( (dwTime-lpChar->dwExpAttackTime)>15000 ) {
											//15�� �̻� ���ݾ���/ �������� ���
											lpChar->lpExpAttackPlayInfo = 0;
										}
									}
								}

								/////////////////// ��ų ó�� /////////////////////
								//HOLY_VALOR ( ���ݷ� % ���� )
								if ( lpPlayInfo->dwSkill_HolyValor_Time ) {
									if ( lpPlayInfo->dwSkill_HolyValor_Time>dwPlayServTime ) {
										if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) 
											cnt += (cnt*lpPlayInfo->dwSkill_HolyValor_Param)/100;	//�𵥵� ���ݷ� ����
									}
									else {	//���� ( �ð��ʰ� )
										lpPlayInfo->dwSkill_HolyValor_Time = 0;
										lpPlayInfo->dwSkill_HolyValor_Param = 0;
									}
								}

								Dmg4 = cnt;

								///////////////////////////////////////////////////
								//���� ( ���� ���� )
								lpChar->smCharInfo.Life[0] -= cnt;

								if ( lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle ) rsRecordCharDamage( lpPlayInfo , lpChar , cnt );	//�������� ����ϴ� ��

								if ( lpChar->smCharInfo.Level>14 ) {
									//���� 14�̻� 20%�̻� Ÿ�ݽÿ� �ة�
									cnt = (cnt*5)/lpChar->smCharInfo.Life[1];
								}
								else {
									//���� 10���� 10%�̻� Ÿ�ݽÿ� �ة�
									cnt = (cnt*10)/lpChar->smCharInfo.Life[1];
								}

								if ( lpChar->smCharInfo.Life[0]>0 && ( cnt>=1 || (lpTransAttackData->AttackState&0xFFFF)>1 ) && 
									(rand()%100)<lpChar->smMonsterInfo.DamageStunPers ) {
									//�����Ŀ��� ���ų� ũ��Ƽ�� ������ ���
									//������ �ڷ� �������� ��
									ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
									ang = (ang2+ANGLE_180) & ANGCLIP;
									lpChar->Angle.y = ang;
									lpChar->MoveAngle( 10 );
									lpChar->Angle.y = ang2;

									if ( lpChar->smCharInfo.Life[0] && 
										lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
										lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT && 
										lpChar->MotionInfo->State!=CHRMOTION_STATE_WARP &&
										lpChar->MotionInfo->State!=CHRMOTION_STATE_SKILL ) {

										lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
									}
								}
							}
							lpPlayInfo->dwLastAttackTime = dwTime;


#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 )
{
	//�ؿ�
	wsprintf( szDispDamage , ">%s Damaged ( %d %d %d %d )" , lpChar->smCharInfo.szName , Dmg1 , Dmg2, Dmg3, Dmg4 );
	rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );

#ifdef _LANGUAGE_VEITNAM
	//������ ����HP ���	//�ؿ�
	wsprintf( szDispDamage2 , ">%s (hp:%d, Total:%d) )" , lpChar->smCharInfo.szName , lpChar->smCharInfo.Life[0], lpChar->smCharInfo.Life[1]);
	rsDisplayDamgeToClient( lpPlayInfo , szDispDamage2 );
#endif
}
#endif


				}
				else {
					//�Ϲ� NPC

					if ( abs( ( long )lpPlayInfo->dwLastAttackTime - ( long )dwTime )>1500 )
					{		//1.5�� �̻� ����� ����
						lpPlayInfo->dwLastAttackTime = dwTime;

						if ( lpChar->smMonsterInfo.QuestCode && lpChar->smMonsterInfo.QuestCode==(lpTransAttackData->AttackState>>16) ) {
							//����Ʈ ���� NPC
							if ( rsProcessQuest( lpPlayInfo , lpChar , lpTransAttackData->AttackState>>16 , lpTransAttackData->AttackState&0xFFFF )==TRUE )
								return TRUE;
						}

						if ( lpChar->szChatMessage[0] && (!lpChar->smMonsterInfo.EventNPC || lpChar->smMonsterInfo.EventNPC==5) ) {
							//���� �޼��� ����
							if ( lpChar->smMonsterInfo.NpcMsgCount>1 ) {
								//��Ƽ �޼���
								TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
								TransChatMessage.dwIP = 0;
								TransChatMessage.dwObjectSerial = lpChar->dwObjectSerial;
								lstrcpy( TransChatMessage.szMessage , 
									lpChar->smMonsterInfo.lpNpcMessage[ rand()%lpChar->smMonsterInfo.NpcMsgCount ]   );

								TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
								lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
							}
							else {
								//���� �޼���
								TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
								TransChatMessage.size = 32+lstrlen(lpChar->szChatMessage);
								TransChatMessage.dwIP = 0;
								TransChatMessage.dwObjectSerial = lpChar->dwObjectSerial;
								lstrcpy( TransChatMessage.szMessage , lpChar->szChatMessage );
								lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
							}
						}
						if ( lpChar->smMonsterInfo.SellAttackItemCount || 
							lpChar->smMonsterInfo.SellDefenceItemCount ||
							lpChar->smMonsterInfo.SellEtcItemCount ) {

							//���� ������ ��� Ŭ���̾�Ʈ�� ����
							SendShopItemList( lpsmSock , lpChar );
						}

						if ( lpChar->smMonsterInfo.SkillMaster ) {
							//��ų ���� NPC
							if ( lpPlayInfo->AdminMode ) {
								SendSkillMenu( lpsmSock , lpChar , 2 );
							}
							else
								SendSkillMenu( lpsmSock , lpChar , lpChar->smMonsterInfo.SkillChangeJob );

						}
						if ( lpChar->smMonsterInfo.WareHouseMaster ) {
							//������ ���� NPC
							SendOpenWareHouse( lpsmSock );
						}

						if ( lpChar->smMonsterInfo.ItemMix ) {
							//������ ���� NPC
							SendOpenMixItem( lpsmSock , lpChar->smMonsterInfo.ItemMix );
						}
	
						// pluto ������ ���� NPC
						if( lpChar->smMonsterInfo.Smelting )
						{
							SendOpenSmelting( lpsmSock );
						}
						// pluto ������ ���� NPC
						if( lpChar->smMonsterInfo.Manufacture )
						{
							SendOpenManufacture( lpsmSock );
						}
						if ( lpChar->smMonsterInfo.CollectMoney ) {
							//�ҿ��̿����� ��� NPC
							SendOpenCollectMoney( lpsmSock );
						}
#ifdef _LANGUAGE_VEITNAM
						if ( lpChar->smMonsterInfo.WowEvent ) 
						{
							wsprintf( TransChatMessage.szMessage , "EventGirl: %s",szBadEventGirl_Captor);
							TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
							TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
							TransChatMessage.dwIP = 0;
							TransChatMessage.dwObjectSerial = 0;
							lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );

							lpChar->Close();
							return TRUE;
						}
#endif

						if ( lpChar->smMonsterInfo.EventGift ) {
							//���� ��÷ �̺�Ʈ
							SendOpenEventGift( lpsmSock );
						}
						if ( lpChar->smMonsterInfo.GiftExpress ) {
							//���� ���
							smTransCommand.size = sizeof( smTRANS_COMMAND );
							smTransCommand.code = smTRANSCODE_ITEM_EXPRESS;
							smTransCommand.WParam = 0;
							smTransCommand.LParam = 0;
							smTransCommand.SParam = 0;
							rsSendDataServer( lpsmSock , &smTransCommand );
							//SendOpenGiftExpress( lpPlayInfo , 0 );
						}

						if ( lpChar->smMonsterInfo.WingQuestNpc ) {
							smTransCommand.size = sizeof( smTRANS_COMMAND );
							smTransCommand.code = smTRANSCODE_WING_NPC;
							smTransCommand.WParam = lpChar->smMonsterInfo.WingQuestNpc;
							smTransCommand.LParam = 0;
							smTransCommand.SParam = 0;
							lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
						}

						if ( lpChar->smMonsterInfo.TelePortNpc ) {
							int temp = 0;							
							if( lpChar->smMonsterInfo.TelePortNpc == 3) // pluto ���Ͱ� �������� �����޼��� ���ö� ������Ʈ �޼��� ��� ���ϰ� �Ϸ��� ����������
							{
								temp = -1;
							}
								smTransCommand.size = sizeof( smTRANS_COMMAND );
								smTransCommand.code = smTRANSCODE_WING_NPC;
								smTransCommand.WParam = temp;
								smTransCommand.LParam = lpChar->smMonsterInfo.TelePortNpc;
								smTransCommand.SParam = 0;
								lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
						}


						if ( lpChar->smMonsterInfo.StarPointNpc && lpPlayInfo->smCharInfo.Level>=lpChar->smMonsterInfo.StarPointNpc ) {
							//���� 20�� ������ , �� ����Ʈ NPC
							smTransCommand.size = sizeof( smTRANS_COMMAND );
							smTransCommand.code = smTRANSCODE_OPEN_STARPOINT;
							smTransCommand.WParam = 0;
							smTransCommand.LParam = 0;
							smTransCommand.SParam = 0;
							lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
						}

						if ( lpChar->smMonsterInfo.GiveMoneyNpc ) {
							//�� �����
							smTransCommand.size = sizeof( smTRANS_COMMAND );
							smTransCommand.code = smTRANSCODE_OPEN_GIVEMONEY;
							smTransCommand.WParam = 0;
							smTransCommand.LParam = 0;
							smTransCommand.SParam = 0;
							lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
						}



						if ( lpChar->smMonsterInfo.EventNPC ) {
							if ( lpChar->smMonsterInfo.EventNPC==5 ) {
								//SOD ȸ���
								smTransCommand.size = sizeof( smTRANS_COMMAND );
								smTransCommand.code = smTRANSCODE_OPEN_COLLECT;
								smTransCommand.WParam = 5;
								smTransCommand.LParam = 0;
								smTransCommand.SParam = 0;
								lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
							}
							else {
								//SOD ���嵵���
								SendOpenEvent( lpChar , lpPlayInfo , lpChar->smMonsterInfo.EventNPC );
							}

							if ( lpChar->MotionInfo->State<0x100 ) {
								lpChar->SetMotionFromCode(CHRMOTION_STATE_ATTACK);
							}
						}


						if ( lpChar->smMonsterInfo.ItemAging ) {
							//������ ����¡ NPC
							SendOpenAgingItem( lpsmSock );
						}	

						if ( lpChar->smMonsterInfo.ClanNPC ) {
							//Ŭ�� NPC
							SendOpenClanMenu( lpsmSock );
						}

						if ( lpChar->smMonsterInfo.BlessCastleNPC ) {
							//����ĳ�� �������� �۽�
							rsSendBlessCastInfo( lpPlayInfo , lpChar->smMonsterInfo.BlessCastleNPC );
						}

						if ( lpChar->smMonsterInfo.PollingNpc ) {
							//�������� NPC
							smTransCommand.size = sizeof( smTRANS_COMMAND );
							smTransCommand.code = smTRANSCODE_PUBLIC_POLLING;
							smTransCommand.WParam = lpChar->smMonsterInfo.PollingNpc;
							smTransCommand.LParam = 0;
							smTransCommand.SParam = 0;
							smTransCommand.EParam = 0;
							rsSendDataServer( lpsmSock , &smTransCommand );
						}
						

						if ( lpChar->smMonsterInfo.szMediaPlayNPC_Title && lpChar->smMonsterInfo.szMediaPlayNPC_Path ) {
							//������ ��� NPC

							lstrcpy( TransChatMessage.szMessage , lpChar->smMonsterInfo.szMediaPlayNPC_Path );
							len = lstrlen(TransChatMessage.szMessage);
							lstrcpy( TransChatMessage.szMessage+len+1 , lpChar->smMonsterInfo.szMediaPlayNPC_Title );

							TransChatMessage.code = smTRANSCODE_MEDIA_NPC;
							TransChatMessage.size = 34+len+lstrlen(lpChar->smMonsterInfo.szMediaPlayNPC_Title);
							TransChatMessage.dwIP = 0;
							TransChatMessage.dwObjectSerial = lpChar->dwObjectSerial;
							lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
						}

						// ������ - �ͽ��� ���� ( �ͽ��� ���� npc�� �����ϸ� â�� ����� �޽����� ������ �Լ� ȣ�� )
						if( lpChar->smMonsterInfo.MixtureReset )
						{
							SendOpenMixtureItemReset( lpsmSock );
						}

					}
				}

				if ( lpChar->smCharInfo.Life[0]<=0 ) {
					lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );

					lpChar->lpExt2 = (void *)lpPlayInfo;


				}
			}
		}
		else {
			//���� PK �Ұ� ���� ����
			//ĳ���͸� ã�´� ( ���߿� ���� ������ �ְ� �ڵ�ȭ ���� �ٲ� ��� �� )
			lpPlayInfo2 = srFindUserFromSerial( lpTransAttackData->dwTarObjectSerial );
			if ( lpPlayInfo2 ) {
				if ( rsServerConfig.Enable_PK || lpPlayInfo->smCharInfo.State!=lpPlayInfo2->smCharInfo.State ) {

					if ( lpPlayInfo->AdminMode || ( lpPlayInfo->smCharInfo.Level>LIMIT_PK_LEVEL && lpPlayInfo2->smCharInfo.Level>LIMIT_PK_LEVEL ) ) {
						//���� ���� 10 ���ϴ� PK����
						//lpPlayInfo2->lpsmSock->Send( (char *)lpTransAttackData , lpTransAttackData->size , TRUE );

						//�ٸ� ���� ���� ��Ŷ ������ ( ���� ���� )
						rsSendAttackUser( lpPlayInfo , lpPlayInfo2, lpTransAttackData->Power/PK_POWER_DIVIDE , 0x80 );
					}
				}
			}
		}
#endif		
		return TRUE;

}

//���� ������ ����
int rsRecvRangeAttackData_Old( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA *lpTransSkilAttackData , int Type )
{

	smTRANS_COMMAND		smTransCommand;

	//������ ��ų ����
	//���� ����Ÿ ���
	lpPlayInfo->Recv_AttackCount++;
	lpPlayInfo->Recv_AttackDamage +=lpTransSkilAttackData->Power;

	if ( lpPlayInfo->HideMode && lpPlayInfo->AdminMode<4 ) return FALSE;

	RecvRangeAttack( lpPlayInfo , lpTransSkilAttackData );

	if ( lpTransSkilAttackData->Power>=1000 ) {
		smTransCommand.WParam = 1830;
		smTransCommand.SParam = lpTransSkilAttackData->Power;
		smTransCommand.LParam = smTransCommand.SParam;
		RecordHackLogFile( lpPlayInfo , &smTransCommand );

		DisconnectUser( lpPlayInfo->lpsmSock );
	}

	//��ī�ϼ� ��ų�� ATTACKDATA�� ���ÿ� �ö󰡴� ��찡 ���� ����

	return TRUE;
}

//���� ������ ���� ����
int rsRecvRangeAttackUserData_Old( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA *lpTransSkilAttackData )
{

	smTRANS_COMMAND		smTransCommand;

	//������ ��ų ���� ( ���� ���� )
	//���� ����Ÿ ���
	lpPlayInfo->Recv_AttackCount++;
	lpPlayInfo->Recv_AttackDamage +=lpTransSkilAttackData->Power;

	if ( lpPlayInfo->HideMode && lpPlayInfo->AdminMode<4 ) return FALSE;

	RecvRangeAttackUser( lpPlayInfo , lpTransSkilAttackData );

	if ( lpTransSkilAttackData->Power>=1000 ) {
		smTransCommand.WParam = 1830;
		smTransCommand.SParam = lpTransSkilAttackData->Power;
		smTransCommand.LParam = smTransCommand.SParam;
		RecordHackLogFile( lpPlayInfo , &smTransCommand );

		DisconnectUser( lpPlayInfo->lpsmSock );
	}

	return TRUE;
}

//�Ÿ� �˻� ( ���Ϳ� ��ǥ���� �Ÿ� )
int	rsCheckAttackRange( int dx,int dy, int dz, smCHAR *lpChar , int dAttackSize )
{
	int x,y,z;

	x = (dx-lpChar->pX)>>FLOATNS;
	y = (dy-lpChar->pY)>>FLOATNS;
	z = (dz-lpChar->pZ)>>FLOATNS;

	if ( abs(y)>100 || (x*x+z*z)>dAttackSize )
		return FALSE;		//�Ÿ��� �հ�� ����

	return TRUE;
}

//�Ÿ� �˻� ( �÷��̾�� ��ǥ���� �Ÿ� )
int	rsCheckAttackRange( int dx,int dy,int dz ,rsPLAYINFO *lpPlayInfo )
{
	int x,y,z;

	x = (dx-lpPlayInfo->Position.x)>>FLOATNS;
	y = (dy-lpPlayInfo->Position.y)>>FLOATNS;
	z = (dz-lpPlayInfo->Position.z)>>FLOATNS;

	if ( abs(y)>100 || (x*x+z*z)>(300*300) ) 
		return FALSE;		//�Ÿ��� �հ�� ����

	return TRUE;
}


//�ٸ� ���� ���� ��Ŷ ������ ( ���� ���� )
int rsSendAttackUser( rsPLAYINFO *lpPlayInfo , rsPLAYINFO *lpPlayInfo2, int Power , int AttackState , short sParam1 , short sParam2 )
{

	TRANS_ATTACKDATA	TransAttackData;
	smTRANS_COMMAND		smTransCommand;
	int	temp;

	// pluto PK��� �ʵ� ����
	if ( !lpPlayInfo->AdminMode )
	{
		if( rsServerConfig.Enable_PKField_All )
		{
			// pluto PK��� �ʵ� ���� ����
			if ( !rsServerConfig.Enable_PK || lpPlayInfo->Position.Area < 11 && lpPlayInfo->Position.Area > 34 )
				return FALSE;			//���� �Ϲ������� PK����

			if ( rsServerConfig.BlessCastleMode==2 && !lpPlayInfo->AdminMode ) return FALSE;
		}
		else
		{
			if ( !rsServerConfig.Enable_PK && 
				( sField[lpPlayInfo->Position.Area].State!=FIELD_STATE_CASTLE || lpPlayInfo->Position.z>CastleBattleZone_LineZ ) &&
				lpPlayInfo->Position.Area!=rsServerConfig.Enable_PKField )
				return FALSE;			//���� �Ϲ������� PK����

			if ( rsServerConfig.BlessCastleMode==2 && !lpPlayInfo->AdminMode && lpPlayInfo->Position.Area!=rsServerConfig.Enable_PKField ) return FALSE;
		}
	}



	TransAttackData.code = smTRANSCODE_ATTACKDATA;
	TransAttackData.size = sizeof(TRANS_ATTACKDATA);

	TransAttackData.x = lpPlayInfo2->Position.x;
	TransAttackData.y = lpPlayInfo2->Position.y;
	TransAttackData.z = lpPlayInfo2->Position.z;


	TransAttackData.AttackState = AttackState;
	TransAttackData.AttackSize  = 32*fONE;
	TransAttackData.Power		= Power;

	//���� ������ �ð����� Ȯ��
	if (  lpPlayInfo->dwSiegeItem_Scroll_Time && lpPlayInfo->dwSiegeItem_Scroll_Time<dwPlayServTime ) {
		lpPlayInfo->dwSiegeItem_Scroll_Code = 0;
		lpPlayInfo->dwSiegeItem_Scroll_Time = 0;
	}
	if (  lpPlayInfo->dwSiegeItem_Stone1_Time && lpPlayInfo->dwSiegeItem_Stone1_Time<dwPlayServTime ) {
		lpPlayInfo->dwSiegeItem_Stone1_Code = 0;
		lpPlayInfo->dwSiegeItem_Stone1_Time = 0;
	}
	if (  lpPlayInfo->dwSiegeItem_Stone2_Time && lpPlayInfo->dwSiegeItem_Stone2_Time<dwPlayServTime ) {
		lpPlayInfo->dwSiegeItem_Stone2_Code = 0;
		lpPlayInfo->dwSiegeItem_Stone2_Time = 0;
	}

	//���� ������ �ð����� Ȯ��
	if (  lpPlayInfo2->dwSiegeItem_Scroll_Time && lpPlayInfo2->dwSiegeItem_Scroll_Time<dwPlayServTime ) {
		lpPlayInfo2->dwSiegeItem_Scroll_Code = 0;
		lpPlayInfo2->dwSiegeItem_Scroll_Time = 0;
	}
	if (  lpPlayInfo2->dwSiegeItem_Stone1_Time && lpPlayInfo2->dwSiegeItem_Stone1_Time<dwPlayServTime ) {
		lpPlayInfo2->dwSiegeItem_Stone1_Code = 0;
		lpPlayInfo2->dwSiegeItem_Stone1_Time = 0;
	}
	if (  lpPlayInfo->dwSiegeItem_Stone2_Time && lpPlayInfo->dwSiegeItem_Stone2_Time<dwPlayServTime ) {
		lpPlayInfo->dwSiegeItem_Stone2_Code = 0;
		lpPlayInfo->dwSiegeItem_Stone2_Time = 0;
	}

	if ( lpPlayInfo2->dwSiegeItem_Scroll_Code==(sinBC1|sin01) ) {	//������ũ��
		if ( lpPlayInfo2->Position.Area==rsCASTLE_FIELD ) return FALSE;
	}
	if ( lpPlayInfo2->dwSiegeItem_Scroll_Code==(sinBI1|sin05) )		//������ũ��
		return FALSE;

	if ( lpPlayInfo2->dwSiegeItem_Scroll_Code==(sinBC1|sin03) ) {	//�߰� ȸ��
		if ( lpPlayInfo2->Position.Area==rsCASTLE_FIELD ) {
			if ( (rand()%100)<(int)lpPlayInfo2->dwSiegeItem_Scroll_Param ) {
#ifdef DISP_DAMAGE
				if ( lpPlayInfo2->AdminMode>2 ) {
					wsprintf( szDispDamage , ">%s ���� �߰�ȸ�� ( Evade Scrorll[%d] )" , lpPlayInfo->smCharInfo.szName , lpPlayInfo2->dwSiegeItem_Scroll_Param );
					rsDisplayDamgeToClient( lpPlayInfo2 , szDispDamage );
				}
#endif
				//�߰�ȸ�� ���� Ŭ�󸮾�Ʈ�� ���
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof( smTRANS_COMMAND );
				smTransCommand.WParam = 0;
				smTransCommand.LParam = lpPlayInfo2->dwSiegeItem_Scroll_Code;
				smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo2->lpsmSock->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );
				return FALSE;
			}
		}
	}
	if ( lpPlayInfo2->dwSiegeItem_Scroll_Code==(sinBI1|sin07) ) {	//�߰� ȸ��
		if ( (rand()%100)<(int)lpPlayInfo2->dwSiegeItem_Scroll_Param ) {
#ifdef DISP_DAMAGE
			if ( lpPlayInfo2->AdminMode>2 ) {
				wsprintf( szDispDamage , ">%s ���� �߰�ȸ�� ( Evade Scrorll[%d] )" , lpPlayInfo->smCharInfo.szName , lpPlayInfo2->dwSiegeItem_Scroll_Param );
				rsDisplayDamgeToClient( lpPlayInfo2 , szDispDamage );
			}
#endif
			//�߰�ȸ�� ���� Ŭ�󸮾�Ʈ�� ���
			smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
			smTransCommand.size = sizeof( smTRANS_COMMAND );
			smTransCommand.WParam = 0;
			smTransCommand.LParam = lpPlayInfo2->dwSiegeItem_Scroll_Code;
			smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
			smTransCommand.EParam = 0;
			lpPlayInfo2->lpsmSock->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );
			return FALSE;
		}
	}

	if ( lpPlayInfo->dwSiegeItem_Stone2_Code && lpPlayInfo2->smCharInfo.JOB_CODE==lpPlayInfo->dwSiegeItem_Stone2_Code ) {
		//������ ��ȭ ��ũ��
		TransAttackData.Power += (TransAttackData.Power*lpPlayInfo->dwSiegeItem_Stone2_Param)/100;

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s ���ݷ� ��ȭ�� ( ����:%d )( %d )" , lpPlayInfo2->smCharInfo.szName , lpPlayInfo->dwSiegeItem_Stone2_Code , TransAttackData.Power );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
	}


	if ( lpPlayInfo2->dwSkill_PhysicalAbsorb_Time ) {	//���ݷ� ���� (����� �̸�����)- ������ ���
		if ( lpPlayInfo2->dwSkill_PhysicalAbsorb_Time>dwPlayServTime ) {
			TransAttackData.Power -= lpPlayInfo2->dwSkill_PhysicalAbsorb_Param;
			if ( TransAttackData.Power<0 ) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_PhysicalAbsorb_Time = 0;
			lpPlayInfo2->dwSkill_PhysicalAbsorb_Param = 0;
		}
	}
	if ( lpPlayInfo2->dwSkill_ExtreamShield_Time ) {	//�������� - �ͽ�Ʈ�� ����
		if ( lpPlayInfo2->dwSkill_ExtreamShield_Time>dwPlayServTime ) {
			TransAttackData.AttackState |= (lpPlayInfo2->dwSkill_ExtreamShield_Param<<16);
		}
		else {
			lpPlayInfo2->dwSkill_ExtreamShield_Time = 0;
			lpPlayInfo2->dwSkill_ExtreamShield_Param = 0;
		}
	}

	/////////////////////////////////////////////////////////////////////


	if ( lpPlayInfo2->dwSkill_MetalArmor_Time ) {	//���ݷ� ���� (����� �̸�����)- ��Ż�Ƹ� ���
		if ( lpPlayInfo2->dwSkill_MetalArmor_Time>dwPlayServTime ) {
			TransAttackData.Power -= lpPlayInfo2->dwSkill_MetalArmor_Param;
			if ( TransAttackData.Power<0 ) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_MetalArmor_Time = 0;
			lpPlayInfo2->dwSkill_MetalArmor_Param = 0;
		}
	}


	if ( lpPlayInfo2->dwSkill_VirtualLife_Time ) {	//���� ������� ������ ���ݷ� ����
		if ( lpPlayInfo2->dwSkill_VirtualLife_Time>dwPlayServTime ) {
			TransAttackData.Power -= (TransAttackData.Power*lpPlayInfo2->dwSkill_VirtualLife_Param)/100;
			if ( TransAttackData.Power<0 ) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_VirtualLife_Time = 0;
			lpPlayInfo2->dwSkill_VirtualLife_Param = 0;
		}
	}

	if ( lpPlayInfo2->dwSkill_EnergyShield_Time ) {	//���������� ���ݷ� ����
		if ( lpPlayInfo2->dwSkill_EnergyShield_Time>dwPlayServTime ) {
			temp = (TransAttackData.Power*lpPlayInfo2->dwSkill_EnergyShield_Param)/100;
			TransAttackData.Power -= temp;
			TransAttackData.AttackState |= (temp<<16);
			if ( TransAttackData.Power<0 ) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_EnergyShield_Time = 0;
			lpPlayInfo2->dwSkill_EnergyShield_Param = 0;
		}
	}

	if ( lpPlayInfo2->dwSkill_Compulsion_Time ) {	//Compulsion ����� �߰�
		if ( lpPlayInfo2->dwSkill_Compulsion_Time>dwPlayServTime ) {
			TransAttackData.Power -= lpPlayInfo2->dwSkill_Compulsion_Param;
			if ( TransAttackData.Power<0 ) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_Compulsion_Time = 0;
			lpPlayInfo2->dwSkill_Compulsion_Param = 0;
		}
	}

	if ( lpPlayInfo2->dwSkill_Berserker_Time ) {	//����Ŀ ����� ����
		if ( lpPlayInfo2->dwSkill_Berserker_Time>dwPlayServTime ) {
			TransAttackData.Power -= Berserker_SubAbsorb[lpPlayInfo2->dwSkill_Berserker_Param];
			if ( TransAttackData.Power<0 ) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_Berserker_Time = 0;
			lpPlayInfo2->dwSkill_Berserker_Param = 0;
		}
	}
	

	//���̱� �߰�ȸ��
	if ( lpPlayInfo2->dwSkill_Vague_Time ) {
		if ( lpPlayInfo2->dwSkill_Vague_Time>dwPlayServTime ) {
			if ( (rand()%100)<Vague_EvasionPercent[lpPlayInfo2->dwSkill_Vague_Param] ) {

#ifdef DISP_DAMAGE
				if ( lpPlayInfo2->AdminMode>2 ) {
					wsprintf( szDispDamage , ">%s ���� �߰�ȸ�� ( Vague[%d] )" , lpPlayInfo->smCharInfo.szName , Vague_EvasionPercent[lpPlayInfo2->dwSkill_Vague_Param] );
					rsDisplayDamgeToClient( lpPlayInfo2 , szDispDamage );
				}
#endif

				//�߰�ȸ�� ���� Ŭ�󸮾�Ʈ�� ���
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof( smTRANS_COMMAND );
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_VAGUE;
				smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo2->lpsmSock->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );

				return FALSE;		//�߰� ȸ�� ����
			}
		}
		else {
			lpPlayInfo2->dwSkill_Vague_Time = 0;
			lpPlayInfo2->dwSkill_Vague_Param = 0;
		}
	}

	if ( lpPlayInfo2->dwSkill_GodlyShied_Time ) {	//���ݷ� ���� (����� �̸�����)- ���鸮������
		if ( lpPlayInfo2->dwSkill_GodlyShied_Time>dwPlayServTime ) {
			TransAttackData.Power -= (TransAttackData.Power*Godly_Shield_AbsorbPercent[lpPlayInfo2->dwSkill_GodlyShied_Param])/100;
			if ( TransAttackData.Power<0 ) return FALSE;
		}
		else {
			lpPlayInfo2->dwSkill_GodlyShied_Time = 0;
			lpPlayInfo2->dwSkill_GodlyShied_Param = 0;
		}
	}


	//Ȧ������Ҷ� �߰�ȸ��
	if ( lpPlayInfo2->dwSkill_HallOfValhalla_Time ) {
		if ( lpPlayInfo2->dwSkill_HallOfValhalla_Time>dwPlayServTime ) {
			if ( (rand()%100)<(int)lpPlayInfo2->wSkill_HallOfValhalla_Param[0] ) {
#ifdef DISP_DAMAGE
				if ( lpPlayInfo2->AdminMode>2 ) {
					wsprintf( szDispDamage , ">%s ���� �߰�ȸ�� ( Hall of Valhalla [%d] )" , lpPlayInfo->smCharInfo.szName , lpPlayInfo2->wSkill_HallOfValhalla_Param[0] );
					rsDisplayDamgeToClient( lpPlayInfo2 , szDispDamage );
				}
#endif
				//�߰�ȸ�� ���� Ŭ�󸮾�Ʈ�� ���
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof( smTRANS_COMMAND );
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_HALL_OF_VALHALLA;
				smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo2->lpsmSock->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );

				return FALSE;		//�߰� ȸ�� ����
			}
		}
		else {
			lpPlayInfo2->dwSkill_HallOfValhalla_Time = 0;
		}
	}

	if ( lpPlayInfo2->dwSkill_SummonMuspell_Time ) {	//���� ������
		if ( lpPlayInfo2->dwSkill_SummonMuspell_Time>dwPlayServTime ) {

			if ( (rand()%100)<Summon_Muspell_BlockPercent[ lpPlayInfo2->dwSkill_SummonMuspell_Param ] ) {
#ifdef DISP_DAMAGE
				if ( lpPlayInfo2->AdminMode>2 ) {
					wsprintf( szDispDamage , ">%s ���� �߰� ȸ�� ( Summon Muspell [%d] )" , lpPlayInfo->smCharInfo.szName , Summon_Muspell_BlockPercent[ lpPlayInfo2->dwSkill_SummonMuspell_Param ] );
					rsDisplayDamgeToClient( lpPlayInfo2 , szDispDamage );
				}
#endif
				//�߰�ȸ�� ���� Ŭ�󸮾�Ʈ�� ���
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof( smTRANS_COMMAND );
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_SUMMON_MUSPELL;
				smTransCommand.SParam = lpPlayInfo->dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo2->lpsmSock->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );

				return FALSE;
			}
		}
		else {
			lpPlayInfo2->dwSkill_SummonMuspell_Time = 0;
			lpPlayInfo2->dwSkill_SummonMuspell_Param = 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////

	//TransAttackData.sRating[0] = lpPlayInfo->smCharInfo.Level;
	//TransAttackData.sRating[1] = lpPlayInfo->smCharInfo.Attack_Rating;

	TransAttackData.sRating[0] = sParam1;
	TransAttackData.sRating[1] = sParam2;

	TransAttackData.dwDestObjectSerial = lpPlayInfo->dwObjectSerial;
	TransAttackData.dwTarObjectSerial = lpPlayInfo2->dwObjectSerial;
	TransAttackData.dwChkSum = TransAttackData.Power*2002 + 
		(TransAttackData.x * TransAttackData.y * TransAttackData.z);// ���� ���� �ڵ�



	if ( lpPlayInfo2->lpsmSock ) {

		//���� ����Ÿ ���
		lpPlayInfo2->Send_AttackCount++;
		lpPlayInfo2->Send_AttackDamage+=TransAttackData.Power;


		//ĳ���� PK ���
		rsRecord_PKDamage( lpPlayInfo , lpPlayInfo2 , TransAttackData.Power );

		if ( rsServerConfig.CrazyPacketMode && lpPlayInfo2->dwDecPacketCode && 
			lpPlayInfo2->dwDecPacketTime2<dwPlayServTime ) {					//ũ������ ��Ŷ ���

			//S2C ���� ýũ�� ( ũ������ ��Ŷ )
			TransAttackData.code = smTRANSCODE_ATTACKDATA2;
			TransAttackData.dwDamageChkSum = dm_GetDamgeChkSum_S2V( &TransAttackData );		//S2C ���� ýũ��

			if ( lpPlayInfo2 ) 
				rsEncodeDamagePacket( lpPlayInfo2 , &TransAttackData );
		}
		else
			TransAttackData.dwDamageChkSum = dm_GetDamgeChkSum_S2V( &TransAttackData );		//S2C ���� ýũ��

		lpPlayInfo2->lpsmSock->Send2( (char *)&TransAttackData , TransAttackData.size , TRUE );
	}

	return TRUE;
}




int RecvRangeAttack_User( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA *lpTransSkilAttackData , int AreaNum , int PlaySkillCode )
{


	//smCHAR *lpChar;
	rsPLAYINFO	*lpCharInfo;
	int cnt,cnt2,pow;
	//int ang,ang2;
	//int x,y,z,dist;
	int	rs,time;
	//int life,TotalLife;
	int	StunFlag;
	//smTRANS_COMMAND	smTransCommand;
	int	dDist;
	int	SkillCode , SkillPoint;
	int	StnPers;
	int	Dmg1, Dmg2, Dmg3, Dmg4;
	int SlowSpeed;

	//TRANS_PARTY_SKILL		TransPartySkill;

	SkillCode = PlaySkillCode&0xFF;
	SkillPoint = PlaySkillCode>>8;
	if ( SkillPoint ) {
		SkillPoint --;
		if ( SkillPoint<0 || SkillPoint>=10 ) SkillPoint = 0;
	}


	STG_AREA *lpStgArea;

	if ( AreaNum<STAGE_AREA_MAX && StageArea[AreaNum].lpStage ) lpStgArea = &StageArea[AreaNum];
	else {
		return FALSE;
	}

	if ( lpPlayInfo->HideMode && lpPlayInfo->AdminMode<4 ) return FALSE;

	//�Ÿ� �˻� ( �÷��̾�� ��ǥ���� �Ÿ� )
	if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y,lpTransSkilAttackData->z ,lpPlayInfo )==FALSE )
		return FALSE;

	dDist = lpTransSkilAttackData->AttackSize+60;
	dDist *= dDist;



	switch( lpTransSkilAttackData->AttackState ) {
	case 0:		//�Ϲ� ������
	case 100:	//������ ���� ��ũ�� ( �� ���� )
	case 101:	//����ũ ( �� ���� )
	case 103:	//����� ������ ( �𵥵� 50% ���� )
	case 104:	//���� ( ���� )
	case 105:	//��þƮ ����
	case 106:	//�� ����

		for( cnt2=0;cnt2<CONNECTMAX;cnt2++ ) {

			lpCharInfo = &rsPlayInfo[cnt2];

			if ( lpCharInfo && lpCharInfo->lpsmSock && lpCharInfo->smCharInfo.State ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpCharInfo->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						//�Ÿ��˻�
						//if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						pow = lpTransSkilAttackData->Power;
						StunFlag = TRUE;

						Dmg1 = pow;

						if ( lpTransSkilAttackData->AttackState==100 ) {
							//�� ���� ( �� ���׷� ���� )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_FIRE];
							if ( SkillCode==SKILL_PLAY_METEO ) {
								rs/=2;		//��Ƽ�� �ҼӼ� 50% ����
							}
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}
						if ( lpTransSkilAttackData->AttackState==101 ) {
							//�� ���� ( ���� ���׷� ���� )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}
						if ( lpTransSkilAttackData->AttackState==105 ) {
							//�� ���� ( ���� ���׷� ���� ) - ��þƮ
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
							StunFlag = FALSE;
						}
						if ( lpTransSkilAttackData->AttackState==104 ) {
							//���� ���� ( ���� ���׷� ���� )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_ICE];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}

						if ( lpTransSkilAttackData->AttackState==106 ) {
							//�� ���� ( ���� ���Ǿ� )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_POISON];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}

						StnPers = rand()%100;

						Dmg2 = pow;

						if ( lpTransSkilAttackData->AttackState==103 ) {
							//����� ������ ( �𵥵� 50% ���� )
							if ( lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
								pow += pow/2;
							}
							//�� ���� ( ���� ���׷� ���� )
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_LIGHTING]/10;
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}


						switch( SkillCode ) {
							case SKILL_PLAY_MECHANIC_BOMB:
								//��ī�й� ( ����Ʈ ��ī�� ���Ϳ��� 50%�߰� )
								if ( lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_MUTANT ||
									lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_MECHANIC ) {
										pow += pow/2;
									}
									break;

							case SKILL_PLAY_SPARK:
								//����ũ ( ��ī�� ���Ϳ��� 50% �߰� )
								if ( lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_MECHANIC ) {
									pow += pow/2;
								}
								break;

							case SKILL_PLAY_EXPANSION:
								//�ͽ��Ҽ� ( ����Ʈ ���Ϳ��� 30% �߰� )
								if ( lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_MUTANT ) {
									pow += (pow*30)/100;
								}
								break;

							case SKILL_PLAY_SWORD_BLAST:
								//�ҵ� ����Ʈ ( ��� , ����Ʈ ���� 30% �߰� )
								if ( lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_NORMAL ||
									lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_MUTANT ) {
										pow += (pow*30)/100;
									}
									break;

							case SKILL_PLAY_PIERCING:
								//�Ǿ�� ( ����Ʈ ���Ϳ��� 30% �߰� )
								if ( lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_MUTANT ) {
									pow += (pow*30)/100;
								}
								break;

							case SKILL_PLAY_VENOM_SPEAR:
								//���� ���Ǿ� ������
								//lpChar->PlayPoison[1] = VenomSpear_Time[SkillPoint]*17;
								//lpChar->PlayPoison[0] = GetRandomPos( VenomSpear_Damage[SkillPoint][0] , VenomSpear_Damage[SkillPoint][1] );
								break;

							case SKILL_PLAY_PERFORATION:			//�������̼�
								if ( (rand()%100)<GetCritical2( lpPlayInfo , lpCharInfo , dm_Critical_Temp ) ) {
									pow = (pow*170)/100;						//ũ��Ƽ�� ���ݰ���
#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( Critical )" , lpCharInfo->smCharInfo.szName );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif

								}
								break;

							case SKILL_PLAY_LIGHTNING_JAVELIN:
								if ( lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
									pow += pow/2;
								}
								break;

							case SKILL_PLAY_FLAME_BRANDISH:
								//StnPers = 100;
								break;

							case SKILL_PLAY_BOMB_SHOT:
								if ( lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_DEMON ) {
									pow += (pow*BombShot_DemonDamage[SkillPoint])/100;
								}
								break;

							case SKILL_PLAY_DIASTROPHISM:
								if (  lpCharInfo->smCharInfo.wPlayClass[0]!=MONSTER_CLASS_BOSS )
									StnPers = 0;
								break;

							case SKILL_PLAY_IMPULSION:
								// ��ī�� ���Ϳ��� 30% �߰� 
								if ( lpCharInfo->smCharInfo.Brood==smCHAR_MONSTER_MECHANIC ) {
									pow += (pow*30)/100;
								}
								break;

							case SKILL_PLAY_CYCLONE_STRIKE:
								if ( (rand()%100)<GetCritical2( lpPlayInfo , lpCharInfo , dm_Critical_Temp ) ) {
									pow = (pow*170)/100;						//ũ��Ƽ�� ���ݰ���
#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( Critical )" , lpCharInfo->smCharInfo.szName );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
								}
								break;

							case SKILL_PLAY_CHAIN_LIGHTNING:
								if (  lpCharInfo->smCharInfo.wPlayClass[0]!=MONSTER_CLASS_BOSS &&
									lpCharInfo->smCharInfo.Resistance[sITEMINFO_LIGHTING]<=20 )
									StnPers = 0;
								break;


						}



						Dmg3 = pow;

						if ( pow>0 ) {
							rsSendAttackUser( lpPlayInfo , lpCharInfo , pow/PK_POWER_DIVIDE , 0x80 );

							Dmg4 = pow;


#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( %d %d %d %d )" , lpCharInfo->smCharInfo.szName, Dmg1 , Dmg2, Dmg3, Dmg4 );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
						}
						break;
					}
				}
			}
		}



	case 2:	//�ξ�
	case 5:	//���彺Ʈ����ũ
		for( cnt2=0;cnt2<CONNECTMAX;cnt2++ ) {

			lpCharInfo = &rsPlayInfo[cnt2];

			if ( lpCharInfo && lpCharInfo->lpsmSock && lpCharInfo->smCharInfo.State ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpCharInfo->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						//�Ÿ��˻�
						//if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						//���Ӽ� �� ���� �߸����� ���� 
						rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if ( rs<0 ) rs = 0;
						if ( rs>=100 ) rs=100;
						rs = 100-rs;

						if ( lpTransSkilAttackData->AttackState==2 ) {
							//�ξ�
							//if ( (rand()%100)>rs ) {
								//lpChar->PlayStunCount = lpTransSkilAttackData->Power<<4;		//�߸����� ����
								//lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;
							//}
							rsSendAttackUser( lpPlayInfo , lpCharInfo , 1 , 0x80 , -SKILL_PLAY_ROAR , (lpTransSkilAttackData->Power*rs)/100 );

						}
						else if ( lpTransSkilAttackData->AttackState==5 ) {
							//���� ��Ʈ����ũ
							/*
							pow = ( lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption )/100;
							pow = lpTransSkilAttackData->Power-pow;
							if ( pow>0 ) {
								//���� ( ���� ���� )
								lpChar->smCharInfo.Life[0] -= pow;
								if ( lpChar->lpAttackDamageList ) rsRecordCharDamage( lpPlayInfo , lpChar , pow );	//�������� ����ϴ� ��

							}
							lpChar->PlayStunCount = 16*10;			//�߸����� 10��
							lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;
							*/
							rsSendAttackUser( lpPlayInfo , lpCharInfo , lpTransSkilAttackData->Power/PK_POWER_DIVIDE , 0x80 , -SKILL_PLAY_SHIELD_STRIKE , (10*rs)/100 );


#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( %d %d )" , lpCharInfo->smCharInfo.szName, lpTransSkilAttackData->Power , pow );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
						}
						break;
					}
				}
			}
		}
		break;

	case 3:
		if ( SkillCode==SKILL_PLAY_GLACIAL_SPIKE ) {
			//�۷��� ������ũ
			time = 8;
			SlowSpeed = 200;
		}
		else {
			//�׶��� ����ũ
			time = 10;
			SlowSpeed = 128;
			cnt2 = lpTransSkilAttackData->AttackSize>>FLOATNS;
			for(cnt=0;cnt<10;cnt++) {
				//�׶��� ����ũ �ð��� ã�´�
				if ( G_Pike_Range[cnt]==cnt2 ) {
					time = G_Pike_Time[cnt];
					break;
				}
			}
		}

		for( cnt2=0;cnt2<CONNECTMAX;cnt2++ ) {
			lpCharInfo = &rsPlayInfo[cnt2];

			if ( lpCharInfo && lpCharInfo->lpsmSock && lpCharInfo->smCharInfo.State ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpCharInfo->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						//�Ÿ��˻�
						//if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

							pow = lpTransSkilAttackData->Power;
							Dmg1 = pow;
							rs = lpCharInfo->smCharInfo.Resistance[sITEMINFO_ICE];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
							Dmg2 = pow;

/*
							lpChar->PlaySlowCount = time;		//�ӵ� ������
							lpChar->PlaySlowSpeed = SlowSpeed;

							lpChar->PlayStunCount = 0;			//�߸���� ����

							if ( rs ) {
								if ( rs<0 ) rs = 0;
								lpChar->PlaySlowCount -= (rs*time)/100;
								if ( lpChar->PlaySlowCount<0 ) lpChar->PlaySlowCount = 0;
							}

							//if ( (10-rs/10)
							//10-(���ͼӼ�/10) ��
*/
							if ( pow>0 ) {
								//���� ( ���� ���� )
								rsSendAttackUser( lpPlayInfo , lpCharInfo , pow/PK_POWER_DIVIDE , 0x80 , -SkillCode , time );

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( %d %d )" , lpCharInfo->smCharInfo.szName, Dmg1 , Dmg2  );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
							}							
						//}

						break;
					}
				}
			}
		}
		break;

	case 7:
		//�����
//	int	rs,time;
		time = Distortion_Time[SkillPoint];
		rs = 240-(240*Distortion_SpeedSubPercent[SkillPoint])/100;

		for( cnt2=0;cnt2<CONNECTMAX;cnt2++ ) {
			lpCharInfo = &rsPlayInfo[cnt2];

			if ( lpCharInfo && lpCharInfo->lpsmSock && lpCharInfo->smCharInfo.State ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpCharInfo->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {
/*
							lpChar->PlaySlowCount = time;		//�ӵ� ������
							lpChar->PlaySlowSpeed = rs;
							lpChar->PlayDistortion = time;

							lpChar->PlayStunCount = 0;			//�߸���� ����
*/
							rsSendAttackUser( lpPlayInfo , lpCharInfo , 1 , 0x80 , -SkillCode , time );

							break;
					}
				}
			}
		}

		break;

	}

	return TRUE;
}


int RecvRangeAttack( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA *lpTransSkilAttackData , int AreaNum , int PlaySkillCode )
{

	smCHAR *lpChar;
	int cnt,cnt2,pow;
	int ang,ang2;
	int x,y,z,dist;
	int	rs,time;
	int life,TotalLife;
	int	StunFlag;
	smTRANS_COMMAND	smTransCommand;
	int	dDist;
	int	SkillCode , SkillPoint;
	int	StnPers;
	int	Dmg1, Dmg2, Dmg3, Dmg4;
	int SlowSpeed;

	TRANS_PARTY_SKILL		TransPartySkill;

	SkillCode = PlaySkillCode&0xFF;
	SkillPoint = PlaySkillCode>>8;
	if ( SkillPoint ) {
		SkillPoint --;
		if ( SkillPoint<0 || SkillPoint>=10 ) SkillPoint = 0;
	}


	STG_AREA *lpStgArea;

	if ( AreaNum<STAGE_AREA_MAX && StageArea[AreaNum].lpStage ) lpStgArea = &StageArea[AreaNum];
	else {
		return FALSE;
	}

	if ( lpPlayInfo->HideMode && lpPlayInfo->AdminMode<4 ) return FALSE;

	//�Ÿ� �˻� ( �÷��̾�� ��ǥ���� �Ÿ� )
	if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y,lpTransSkilAttackData->z ,lpPlayInfo )==FALSE )
		return FALSE;

	dDist = lpTransSkilAttackData->AttackSize+60;
	dDist *= dDist;



	switch( lpTransSkilAttackData->AttackState ) {
	case 0:		//�Ϲ� ������
	case 100:	//������ ���� ��ũ�� ( �� ���� )
	case 101:	//����ũ ( �� ���� )
	case 103:	//����� ������ ( �𵥵� 50% ���� )
	case 104:	//���� ( ���� )
	case 105:	//��þƮ ����
	case 106:	//�� ����

		for( cnt2=0;cnt2<STG_MONSTER_MAX;cnt2++ ) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if ( lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						if ( !lpPlayInfo->lpChrTarget ) lpPlayInfo->lpChrTarget = lpChar;		//�켱 ��ǥ ĳ������ ����

						if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_USER && 
							lpChar->HoSkillCode && 
							lpChar->lpMasterPlayInfo==lpPlayInfo ) {
								continue;	//���� ��ȯ���� ������ ����
							}

						//�Ÿ��˻�
						if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						pow = lpTransSkilAttackData->Power;
						StunFlag = TRUE;

						Dmg1 = pow;

						if ( lpTransSkilAttackData->AttackState==100 ) {
							//�� ���� ( �� ���׷� ���� )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_FIRE];
							if ( SkillCode==SKILL_PLAY_METEO ) {
								rs/=2;		//��Ƽ�� �ҼӼ� 50% ����
							}
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}
						if ( lpTransSkilAttackData->AttackState==101 ) {
							//�� ���� ( ���� ���׷� ���� )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}
						if ( lpTransSkilAttackData->AttackState==105 ) {
							//�� ���� ( ���� ���׷� ���� ) - ��þƮ
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
							StunFlag = FALSE;
						}
						if ( lpTransSkilAttackData->AttackState==104 ) {
							//���� ���� ( ���� ���׷� ���� )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_ICE];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}

						if ( lpTransSkilAttackData->AttackState==106 ) {
							//�� ���� ( ���� ���Ǿ� )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_POISON];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}

						StnPers = rand()%100;

						Dmg2 = pow;

						if ( lpTransSkilAttackData->AttackState==103 ) {
							//����� ������ ( �𵥵� 50% ���� )
							if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
								pow += pow/2;
							}
							//�� ���� ( ���� ���׷� ���� )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING]/10;
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}


						switch( SkillCode ) {
							case SKILL_PLAY_MECHANIC_BOMB:
								//��ī�й� ( ����Ʈ ��ī�� ���Ϳ��� 50%�߰� )
								if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_MUTANT ||
									lpChar->smCharInfo.Brood==smCHAR_MONSTER_MECHANIC ) {
										pow += pow/2;
									}
									break;

							case SKILL_PLAY_SPARK:
								//����ũ ( ��ī�� ���Ϳ��� 50% �߰� )
								if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_MECHANIC ) {
									pow += pow/2;
								}
								break;

							case SKILL_PLAY_EXPANSION:
								//�ͽ��Ҽ� ( ����Ʈ ���Ϳ��� 30% �߰� )
								if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_MUTANT ) {
									pow += (pow*30)/100;
								}
								break;

							case SKILL_PLAY_SWORD_BLAST:
								//�ҵ� ����Ʈ ( ��� , ����Ʈ ���� 30% �߰� )
								if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_NORMAL ||
									lpChar->smCharInfo.Brood==smCHAR_MONSTER_MUTANT ) {
										pow += (pow*30)/100;
									}
									break;

							case SKILL_PLAY_PIERCING:
								//�Ǿ�� ( ����Ʈ ���Ϳ��� 30% �߰� )
								if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_MUTANT ) {
									pow += (pow*30)/100;
								}
								break;

							case SKILL_PLAY_VENOM_SPEAR:
								//���� ���Ǿ� ������
								lpChar->PlayPoison[1] = VenomSpear_Time[SkillPoint]*17;
								lpChar->PlayPoison[0] = GetRandomPos( VenomSpear_Damage[SkillPoint][0] , VenomSpear_Damage[SkillPoint][1] );
								break;

							case SKILL_PLAY_PERFORATION:			//�������̼�
								if ( (rand()%100)<GetCritical( lpPlayInfo , lpChar , dm_Critical_Temp ) ) {
									pow = (pow*170)/100;						//ũ��Ƽ�� ���ݰ���
#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( Critical )" , lpChar->smCharInfo.szName );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif

								}
								break;

							case SKILL_PLAY_LIGHTNING_JAVELIN:
								if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
									pow += pow/2;
								}
								break;

							case SKILL_PLAY_FLAME_BRANDISH:
								//StnPers = 100;
								break;

							case SKILL_PLAY_BOMB_SHOT:
								if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_DEMON ) {
									pow += (pow*BombShot_DemonDamage[SkillPoint])/100;
								}
								break;

							case SKILL_PLAY_DIASTROPHISM:
								//lpChar->lpTargetPlayInfo = 0;
								//lpChar->dwNextMotionCode = CHRMOTION_STATE_STAND;
								if (  lpChar->smCharInfo.wPlayClass[0]!=MONSTER_CLASS_BOSS )
									StnPers = 0;

								if ( lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_DEATHKNIGHT || lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_BABEL || lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_KELVEZU || lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_MOKOVA ) 
									StnPers = 100;
								break;

							case SKILL_PLAY_IMPULSION:
								// ��ī�� ���Ϳ��� 30% �߰� 
								if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_MECHANIC ) {
									pow += (pow*30)/100;
								}
								break;

							case SKILL_PLAY_CYCLONE_STRIKE:
								if ( (rand()%100)<GetCritical( lpPlayInfo , lpChar , dm_Critical_Temp ) ) {
									pow = (pow*170)/100;						//ũ��Ƽ�� ���ݰ���
#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( Critical )" , lpChar->smCharInfo.szName );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
								}
								break;

							case SKILL_PLAY_CHAIN_LIGHTNING:
								//lpChar->lpTargetPlayInfo = 0;
								//lpChar->dwNextMotionCode = CHRMOTION_STATE_STAND;
								if (  lpChar->smCharInfo.wPlayClass[0]!=MONSTER_CLASS_BOSS &&
									lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING]<=20 )
									StnPers = 0;
								break;


						}

						lpChar->PlayStunCount = 0;			//�߸���� ����

						Dmg3 = pow;

						pow -= ( pow * lpChar->smCharInfo.Absorption )/100;
						//pow = lpTransSkilAttackData->Power-pow;

						Dmg4 = pow;

						if ( pow>0 ) {
							//���� ( ���� ���� )
							lpChar->smCharInfo.Life[0] -= pow;

							if ( lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle ) rsRecordCharDamage( lpPlayInfo , lpChar , pow );	//�������� ����ϴ� ��

							if ( StunFlag && lpChar->smCharInfo.Life[0] && StnPers<lpChar->smMonsterInfo.DamageStunPers ) {
								//������ �ڷ� �������� ��

								if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
									lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {

										ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
										ang = (ang2+ANGLE_180) & ANGCLIP;
										lpChar->Angle.y = ang;
										lpChar->MoveAngle( 10 );
										lpChar->Angle.y = ang2;

										//4% �̻�
										if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power || StnPers==0 ) {
											lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
										}
								}
							}

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( %d %d %d %d )" , lpChar->smCharInfo.szName, Dmg1 , Dmg2, Dmg3, Dmg4 );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif


						}


						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;
	case 1:
		//����ũ ����
		for( cnt2=0;cnt2<STG_MONSTER_MAX;cnt2++ ) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if ( lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						//�Ÿ��˻�
						if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
						y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
						z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
						if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//�Ÿ��� �հ�� ����

						dist = (int)sqrt(x*x+z*z);
						dist = lpTransSkilAttackData->AttackSize - dist;

				

						if ( lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_DEATHKNIGHT && lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_BABEL && lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_KELVEZU && lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_MOKOVA ) 
						{
							if ( lpChar->smCharInfo.Life[0] ) {
								//�ڷ� �������� ��
								ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
								ang = (ang2+ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->OverLapPosi2 = FALSE;
								lpChar->MoveAngle( dist );
								lpChar->Angle.y = ang2;

								if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
									lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
										//4% �̻�
										if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power ) {
											lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
										}
								}
								lpChar->lpTargetPlayInfo = 0;
								lpChar->dwNextMotionCode = CHRMOTION_STATE_STAND;
							}
						}

						pow = ( lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption )/100;
						pow = lpTransSkilAttackData->Power-pow;

						lpChar->PlayStunCount = 0;			//�߸���� ����

						if ( pow>0 ) {
							//���� ( ���� ���� )
							lpChar->smCharInfo.Life[0] -= pow;
						}

						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;

					}
				}
			}
		}
		break;
	case 2:	//�ξ�
	case 5:	//���彺Ʈ����ũ
	case 201:	//Compulsion
		for( cnt2=0;cnt2<STG_MONSTER_MAX;cnt2++ ) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if ( lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						//�Ÿ��˻�
						if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						//���Ӽ� �� ���� �߸����� ���� 
						rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if ( rs<0 ) rs = 0;
						if ( rs>=100 ) rs=100;
						rs = 100-rs;

						if ( lpTransSkilAttackData->AttackState==2 ) {
							//�ξ�
							//if ( (rand()%100)>rs ) {
								lpChar->PlayStunCount = lpTransSkilAttackData->Power<<4;		//�߸����� ����
								lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;
							//}
						}
						else if ( lpTransSkilAttackData->AttackState==5 ) {
							//���� ��Ʈ����ũ
							pow = ( lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption )/100;
							pow = lpTransSkilAttackData->Power-pow;
							if ( pow>0 ) {
								//���� ( ���� ���� )
								lpChar->smCharInfo.Life[0] -= pow;
								if ( lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle ) rsRecordCharDamage( lpPlayInfo , lpChar , pow );	//�������� ����ϴ� ��

							}
							lpChar->PlayStunCount = 16*10;			//�߸����� 10��
							lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( %d %d )" , lpChar->smCharInfo.szName, lpTransSkilAttackData->Power , pow );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif

						}
						else if (lpTransSkilAttackData->AttackState==201 ) {	//Compulsion

							if ( lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_DEATHKNIGHT && lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_BABEL && lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_KELVEZU && lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_MOKOVA ) 
							{
								lpChar->lpTargetPlayInfo = 0;
								lpChar->lpTargetChar = 0;

								cnt = Compulsion_Time[SkillPoint]*1000;
								//cnt = 12*1000;

								lpChar->lpCompulsionPlayInfo = lpPlayInfo;
								lpChar->dwCompulsionTime = dwPlayServTime+cnt;

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �ü����� ( %d �� )" , lpChar->smCharInfo.szName, cnt/1000 );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
							}

						}

						if ( lpChar->smCharInfo.Life[0] && 
							lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_BABEL &&
							lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_DEATHKNIGHT &&
							lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_KELVEZU &&
							lpChar->smCharInfo.dwCharSoundCode!=snCHAR_SOUND_MOKOVA ) 
						{
							//������ �ڷ� �������� ��
							ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
							ang = (ang2+ANGLE_180) & ANGCLIP;
							lpChar->Angle.y = ang;
							lpChar->MoveAngle( 10 );
							lpChar->Angle.y = ang2;

							if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
								lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
								lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
							}
						}

						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;
	case 3:
		if ( SkillCode==SKILL_PLAY_GLACIAL_SPIKE ) {
			//�۷��� ������ũ
			time = 8*17;
			SlowSpeed = 200;
		}
		else {
			//�׶��� ����ũ
			time = 175;
			SlowSpeed = 0;
			cnt2 = lpTransSkilAttackData->AttackSize>>FLOATNS;
			for(cnt=0;cnt<10;cnt++) {
				//�׶��� ����ũ �ð��� ã�´�
				if ( G_Pike_Range[cnt]==cnt2 ) {
					time = G_Pike_Time[cnt]*17;
					break;
				}
			}
		}

		for( cnt2=0;cnt2<STG_MONSTER_MAX;cnt2++ ) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if ( lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						//�Ÿ��˻�
						if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

							pow = lpTransSkilAttackData->Power;
							Dmg1 = pow;
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_ICE];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
							Dmg2 = pow;
							pow -= ( pow * lpChar->smCharInfo.Absorption )/100;
							//pow = lpTransSkilAttackData->Power-pow;
							Dmg3 = pow;

							//int G_Pike_Time[10]	     = {3,4,5,6,7,8,9,10,11,12};


							lpChar->PlaySlowCount = time;		//�ӵ� ������
							lpChar->PlaySlowSpeed = SlowSpeed;

							lpChar->PlayStunCount = 0;			//�߸���� ����
							lpChar->PlayDistortion = 0;			//����� ����

							if ( rs ) {
								if ( rs<0 ) rs = 0;
								lpChar->PlaySlowCount -= (rs*time)/100;
								if ( lpChar->PlaySlowCount<0 ) lpChar->PlaySlowCount = 0;
							}

							//if ( (10-rs/10)
							//10-(���ͼӼ�/10) ��

							if ( pow>0 ) {
								//���� ( ���� ���� )
								lpChar->smCharInfo.Life[0] -= pow;
								if ( lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle ) rsRecordCharDamage( lpPlayInfo , lpChar , pow );	//�������� ����ϴ� ��

								if ( lpChar->smCharInfo.Life[0] ) {
									//������ �ڷ� �������� ��
									ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
									ang = (ang2+ANGLE_180) & ANGCLIP;
									lpChar->Angle.y = ang;
									lpChar->MoveAngle( 10 );
									lpChar->Angle.y = ang2;

									if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
										lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
											//4% �̻�
											if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power ) {
												lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
											}
									}
								}

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s �������ݹ��� ( %d %d %d )" , lpChar->smCharInfo.szName, Dmg1 , Dmg2, Dmg3 );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
							
						}

						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;
	case 4:	//�ҿＭĿ
		if ( lpTransSkilAttackData->Power<1 || lpTransSkilAttackData->Power>10 ) break;
		TotalLife = 0;

		for( cnt2=0;cnt2<STG_MONSTER_MAX;cnt2++ ) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if ( lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] &&
						((lpChar->smCharInfo.Life[1]*50)/100)<lpChar->smCharInfo.Life[0] &&
						lpChar->smCharInfo.Brood!=smCHAR_MONSTER_UNDEAD ) {	//�𵥵� ����

						//�Ÿ��˻�
						if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						pow = Soul_Sucker_Absorb[ lpTransSkilAttackData->Power-1 ];

						rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if ( rs ) {
							if ( rs>=100 ) rs=100;
							if ( rs<=-100 ) rs=-100;
							pow -= ((pow*rs)/100);
						}


						lpChar->PlayStunCount = 0;			//�߸���� ����

						if ( pow>0 ) {
							//���� ( ���� ���� )
							life = (lpChar->smCharInfo.Life[1]*pow)/100;
							lpChar->smCharInfo.Life[0] -= life;
							if ( lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle ) rsRecordCharDamage( lpPlayInfo , lpChar , life );	//�������� ����ϴ� ��
							if ( lpChar->smCharInfo.Life[0]<0 ) life+=lpChar->smCharInfo.Life[0];
							TotalLife += life;

							if ( lpChar->smCharInfo.Life[0] && (rand()%100)<lpChar->smMonsterInfo.DamageStunPers ) {
								//������ �ڷ� �������� ��
								ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
								ang = (ang2+ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->MoveAngle( 10 );
								lpChar->Angle.y = ang2;

								if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
									lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {

										lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
								}
							}
						}

						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		if ( TotalLife>0 ) {
			//������ ������� �����ش�
			smTransCommand.code = smTRANSCODE_VIRTURAL_POTION;
			smTransCommand.size = sizeof( smTRANS_COMMAND );
			smTransCommand.WParam = TotalLife;
			smTransCommand.LParam = 0;
			smTransCommand.SParam = 0;
			smTransCommand.EParam = 0;
			if ( lpPlayInfo->lpsmSock ) 
				lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}
		break;

	case 6:	//����ƾ��
		if ( lpTransSkilAttackData->Power<1 || lpTransSkilAttackData->Power>10 ) break;
		TotalLife = 0;

		ZeroMemory( &TransPartySkill , sizeof(TRANS_PARTY_SKILL) );

		for( cnt2=0;cnt2<STG_MONSTER_MAX;cnt2++ ) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if ( lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] &&
						lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {	//�𵥵常

						//�Ÿ��˻�
						if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;

						pow = Extinction_Percent[ lpTransSkilAttackData->Power-1 ];
						pow += lpPlayInfo->smCharInfo.Level/5;

						rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if ( rs ) {
							if ( rs>=100 ) rs=100;
							if ( rs<=-100 ) rs=-100;
							pow -= ((pow*rs)/100);
						}

						lpChar->PlayStunCount = 0;			//�߸���� ����

						if ( (rand()%100)<pow ) {

							if( TransPartySkill.PartyCount<8 )
								TransPartySkill.dwPartyUser[TransPartySkill.PartyCount++] = lpChar->dwObjectSerial;

							//���� ( ���� ���� )
							life = (lpChar->smCharInfo.Life[0]*Extinction_Amount[ lpTransSkilAttackData->Power-1 ])/100;
							lpChar->smCharInfo.Life[0] -= life;
							if ( lpChar->lpAttackDamageList || lpChar->lpAttackDamageList_BlessCastle ) rsRecordCharDamage( lpPlayInfo , lpChar , life );	//�������� ����ϴ� ��
							if ( lpChar->smCharInfo.Life[0]<0 ) life+=lpChar->smCharInfo.Life[0];
							TotalLife += life;

							if ( lpChar->smCharInfo.Life[0] && (rand()%100)<lpChar->smMonsterInfo.DamageStunPers ) {
								//������ �ڷ� �������� ��
								ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
								ang = (ang2+ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->MoveAngle( 10 );
								lpChar->Angle.y = ang2;

								if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
									lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {

										lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
								}
							}
						}

						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;
					}
				}
			}
		}

		if( TransPartySkill.PartyCount>0 && lpPlayInfo && lpPlayInfo->lpsmSock ) {
			TransPartySkill.code = smTRANSCODE_PARTY_SKILL;
			TransPartySkill.size = sizeof(TRANS_PARTY_SKILL);
			TransPartySkill.dwSkillCode = SKILL_PLAY_EXTINCTION;
			rsSendCommandUser2( lpPlayInfo , (smTRANS_COMMAND *)&TransPartySkill );
		}

		break;
/*
int Distortion_SpeedSubPercent[10]  = {5,6,7,8,9,10,11,12,13,14};         //�ӵ� ���� 
int Distortion_DamageSubPercent[10] = {5,6,7,8,9,10,11,12,13,14};         //���ݷ� ����
int Distortion_Area[10]             = {120,125,130,135,140,145,150,155,160,165};     
int Distortion_Time[10]             = {4,5,6,7,8,9,10,11,12,13};
int Distortion_UseMana[10]          = {105,110,115,120,125,130,135,140,145,150};   
*/
	case 7:
		//�����
//	int	rs,time;
		time = Distortion_Time[SkillPoint]*17;
		rs = 240-(240*Distortion_SpeedSubPercent[SkillPoint])/100;

		for( cnt2=0;cnt2<STG_MONSTER_MAX;cnt2++ ) {
			lpChar = lpStgArea->lpCharMonster[cnt2];
			if ( lpChar && lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						//�Ÿ��˻�
						if ( rsCheckAttackRange( lpTransSkilAttackData->x,lpTransSkilAttackData->y, lpTransSkilAttackData->z, lpChar , dDist )==FALSE ) break;


							lpChar->PlaySlowCount = time;		//�ӵ� ������
							lpChar->PlaySlowSpeed = rs;
							lpChar->PlayDistortion = time;

							lpChar->PlayStunCount = 0;			//�߸���� ����


							if ( lpChar->smCharInfo.Life[0] ) {
								//������ �ڷ� �������� ��
								ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
								ang = (ang2+ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->MoveAngle( 10 );
								lpChar->Angle.y = ang2;

								if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
									lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
										//4% �̻�
										if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power ) {
											lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
										}
								}
							}
							
							if ( lpChar->smCharInfo.Life[0]<=0 ) {
								lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
								lpChar->lpExt2 = (void *)lpPlayInfo;
							}

							break;
					}
				}
			}
		}
		break;


	}

	return TRUE;
}


int RecvRangeAttack( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA *lpTransSkilAttackData )
{
/*
	smCHAR *lpChar;
	int cnt,cnt2,pow;
	int ang,ang2;
	int x,y,z,dist;
	int	rs,time;
	int life,TotalLife;
	int	StunFlag;
	smTRANS_COMMAND	smTransCommand;

	switch( lpTransSkilAttackData->AttackState ) {
	case 0:		//�Ϲ� ������
	case 100:	//������ ���� ��ũ�� ( �� ���� )
	case 101:	//����ũ ( �� ���� )
	case 103:	//����� ������ ( �𵥵� 50% ���� )
	case 104:	//���� ( ���� )
	case 105:	//��þƮ ����
		for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
			lpChar = &chrAutoPlayer[cnt2];
			if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

					x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
					y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
					z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
					if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//�Ÿ��� �հ�� ����

						pow = lpTransSkilAttackData->Power;
						StunFlag = TRUE;

						if ( lpTransSkilAttackData->AttackState==100 ) {
							//�� ���� ( �� ���׷� ���� )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_FIRE];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}
						if ( lpTransSkilAttackData->AttackState==101 ) {
							//�� ���� ( ���� ���׷� ���� )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}
						if ( lpTransSkilAttackData->AttackState==105 ) {
							//�� ���� ( ���� ���׷� ���� ) - ��þƮ
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
							StunFlag = FALSE;
						}

						if ( lpTransSkilAttackData->AttackState==104 ) {
							//���� ���� ( ���� ���׷� ���� )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_ICE];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}
						if ( lpTransSkilAttackData->AttackState==103 ) {
							//����� ������ ( �𵥵� 50% ���� )
							if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
								pow += pow/2;
							}
							//�� ���� ( ���� ���׷� ���� )
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_LIGHTING]/10;
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
						}


						pow -= ( pow * lpChar->smCharInfo.Absorption )/100;
						//pow = lpTransSkilAttackData->Power-pow;

						if ( pow>0 ) {
							//���� ( ���� ���� )
							lpChar->smCharInfo.Life[0] -= pow;

							if ( StunFlag && lpChar->smCharInfo.Life[0] && (rand()%100)<lpChar->smMonsterInfo.DamageStunPers ) {
								//������ �ڷ� �������� ��
								//ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpTransSkilAttackData->x, lpTransSkilAttackData->z );
								ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
								ang = (ang2+ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->MoveAngle( 10 );
								lpChar->Angle.y = ang2;

								if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
									lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
										//4% �̻�
										if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power ) {
											lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
										}
								}
							}
						}


						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;
	case 1:
		//����ũ ����
		for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
			lpChar = &chrAutoPlayer[cnt2];
			if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
						y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
						z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
						if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//�Ÿ��� �հ�� ����

						//x = ( lpChar->pX-lpPlayInfo->Position.x )>>FLOATNS;
						//z = ( lpChar->pZ-lpPlayInfo->Position.z )>>FLOATNS;

						dist = (int)sqrt(x*x+z*z);
						dist = (lpTransSkilAttackData->AttackSize>>FLOATNS) - dist;

				
							if ( lpChar->smCharInfo.Life[0] ) {
								//�ڷ� �������� ��
								ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
								ang = (ang2+ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->OverLapPosi2 = FALSE;
								lpChar->MoveAngle( dist );
								lpChar->Angle.y = ang2;

								if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
									lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
										//4% �̻�
										if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power ) {
											lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
										}
								}
								lpChar->lpTargetPlayInfo = 0;
								lpChar->dwNextMotionCode = CHRMOTION_STATE_STAND;
							}

						pow = ( lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption )/100;
						pow = lpTransSkilAttackData->Power-pow;

						if ( pow>0 ) {
							//���� ( ���� ���� )
							lpChar->smCharInfo.Life[0] -= pow;
						}

						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;

					}
				}
			}
		}
		break;
	case 2:	//�ξ�
	case 5:	//���彺Ʈ����ũ
		for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
			lpChar = &chrAutoPlayer[cnt2];
			if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
						y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
						z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
						if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//�Ÿ��� �հ�� ����

						//���Ӽ� �� ���� �߸����� ���� 
						rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if ( rs<0 ) rs = 0;
						if ( rs>=100 ) rs=100;
						rs = 100-rs;

						if ( lpTransSkilAttackData->AttackState==2 ) {
							//�ξ�
							//if ( (rand()%100)>rs ) {
								lpChar->PlayStunCount = lpTransSkilAttackData->Power<<4;		//�߸����� ����
								lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;
							//}
						}
						else if ( lpTransSkilAttackData->AttackState==5 ) {
							//���� ��Ʈ����ũ
							pow = ( lpTransSkilAttackData->Power * lpChar->smCharInfo.Absorption )/100;
							pow = lpTransSkilAttackData->Power-pow;
							if ( pow>0 ) {
								//���� ( ���� ���� )
								lpChar->smCharInfo.Life[0] -= pow;
							}
							lpChar->PlayStunCount = 16*10;			//�߸����� 10��
							lpChar->PlayStunCount = ( lpChar->PlayStunCount * rs )/100;
						}


						if ( lpChar->smCharInfo.Life[0] ) {
							//������ �ڷ� �������� ��
							ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
							ang = (ang2+ANGLE_180) & ANGCLIP;
							lpChar->Angle.y = ang;
							lpChar->MoveAngle( 10 );
							lpChar->Angle.y = ang2;

							if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
								lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
								lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
							}
						}

						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;
	case 3:
		//�׶��� ����ũ
		time = 175;
		cnt2 = lpTransSkilAttackData->AttackSize>>FLOATNS;
		for(cnt=0;cnt<10;cnt++) {
			//�׶��� ����ũ �ð��� ã�´�
			if ( G_Pike_Range[cnt]==cnt2 ) {
				time = G_Pike_Time[cnt]*17;
				break;
			}
		}

		for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
			lpChar = &chrAutoPlayer[cnt2];
			if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] ) {

						x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
						y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
						z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
						if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//�Ÿ��� �հ�� ����

							pow = lpTransSkilAttackData->Power;
							rs = lpChar->smCharInfo.Resistance[sITEMINFO_ICE];
							if ( rs ) {
								if ( rs>=100 ) rs=100;
								if ( rs<=-100 ) rs=-100;
								pow -= ((pow*rs)/100);
							}
							pow -= ( pow * lpChar->smCharInfo.Absorption )/100;
							//pow = lpTransSkilAttackData->Power-pow;



							//int G_Pike_Time[10]	     = {3,4,5,6,7,8,9,10,11,12};


							lpChar->PlaySlowCount = time;		//�ӵ� ������
							lpChar->PlaySlowSpeed = 0;

							if ( rs ) {
								if ( rs<0 ) rs = 0;
								lpChar->PlaySlowCount -= (rs*time)/100;
								if ( lpChar->PlaySlowCount<0 ) lpChar->PlaySlowCount = 0;
							}

							//if ( (10-rs/10)
							//10-(���ͼӼ�/10) ��

							if ( pow>0 ) {
								//���� ( ���� ���� )
								lpChar->smCharInfo.Life[0] -= pow;

								if ( lpChar->smCharInfo.Life[0] ) {
									//������ �ڷ� �������� ��
									ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
									ang = (ang2+ANGLE_180) & ANGCLIP;
									lpChar->Angle.y = ang;
									lpChar->MoveAngle( 10 );
									lpChar->Angle.y = ang2;

									if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
										lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {
											//4% �̻�
											if ( ((lpChar->smCharInfo.Life[1]<<2)/100)<lpTransSkilAttackData->Power ) {
												lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
											}
									}
								}
							
						}

						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		break;
	case 4:	//�ҿＭĿ
		if ( lpTransSkilAttackData->Power<1 || lpTransSkilAttackData->Power>10 ) break;
		TotalLife = 0;

		for(cnt2=0;cnt2<AUTOPLAYER_MAX;cnt2++) {
			lpChar = &chrAutoPlayer[cnt2];
			if ( lpChar->Flag && lpChar->smCharInfo.State && lpChar->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
			
					if ( lpChar->dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] &&
						((lpChar->smCharInfo.Life[1]*60)/100)<lpChar->smCharInfo.Life[0] ) {

						x = (lpPlayInfo->Position.x-lpChar->pX)>>FLOATNS;
						y = (lpPlayInfo->Position.y-lpChar->pY)>>FLOATNS;
						z = (lpPlayInfo->Position.z-lpChar->pZ)>>FLOATNS;
						if ( abs(y)>100 || (x*x+z*z)>(800*800) ) break;		//�Ÿ��� �հ�� ����

						pow = Soul_Sucker_Absorb[ lpTransSkilAttackData->Power-1 ];

						rs = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
						if ( rs ) {
							if ( rs>=100 ) rs=100;
							if ( rs<=-100 ) rs=-100;
							pow -= ((pow*rs)/100);
						}

						if ( pow>0 ) {
							//���� ( ���� ���� )
							life = (lpChar->smCharInfo.Life[1]*pow)/100;
							lpChar->smCharInfo.Life[0] -= life;
							if ( lpChar->smCharInfo.Life[0]<0 ) life+=lpChar->smCharInfo.Life[0];
							TotalLife += life;

							if ( lpChar->smCharInfo.Life[0] && (rand()%100)<lpChar->smMonsterInfo.DamageStunPers ) {
								//������ �ڷ� �������� ��
								ang2 = GetRadian2D( lpChar->pX , lpChar->pZ ,lpPlayInfo->Position.x, lpPlayInfo->Position.z );
								ang = (ang2+ANGLE_180) & ANGCLIP;
								lpChar->Angle.y = ang;
								lpChar->MoveAngle( 10 );
								lpChar->Angle.y = ang2;

								if ( lpChar->smCharInfo.Life[0] && lpChar->MotionInfo->State != CHRMOTION_STATE_DAMAGE && 
									lpChar->MotionInfo->State!=CHRMOTION_STATE_EAT) {

										lpChar->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
								}
							}
						}

						if ( lpChar->smCharInfo.Life[0]<=0 ) {
							lpChar->SetMotionFromCode( CHRMOTION_STATE_DEAD );
							lpChar->lpExt2 = (void *)lpPlayInfo;
						}

						break;
					}
				}
			}
		}
		if ( TotalLife>0 ) {
			//������ ������� �����ش�
			smTransCommand.code = smTRANSCODE_VIRTURAL_POTION;
			smTransCommand.size = sizeof( smTRANS_COMMAND );
			smTransCommand.WParam = TotalLife;
			smTransCommand.LParam = 0;
			smTransCommand.SParam = 0;
			smTransCommand.EParam = 0;
			if ( lpPlayInfo->lpsmSock ) 
				lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}
		break;


	}
*/
	return TRUE;
}


//������ ���� ���� ( ���� ���� )
int RecvRangeAttackUser( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA *lpTransSkilAttackData )
{

	int cnt,cnt2;
	TRANS_ATTACKDATA	TransAttackData;

	if ( !lpPlayInfo->AdminMode ) {
		if ( !rsServerConfig.Enable_PK ) return FALSE;			//���� �Ϲ������� PK����
		if ( lpPlayInfo->smCharInfo.Level<=LIMIT_PK_LEVEL ) return FALSE;	//���� 10���� PK����
	}

	if ( (lpTransSkilAttackData->AttackState&0xFFFF)==2 ) //�ξ� PK���� �ش� ����
		return FALSE;


	for( cnt2=0;cnt2<CONNECTMAX;cnt2++ ) {
		if ( rsPlayInfo[cnt2].lpsmSock && rsPlayInfo[cnt2].dwObjectSerial ) {
			for( cnt=0;cnt<lpTransSkilAttackData->TargetCount;cnt++) {
				if ( rsPlayInfo[cnt2].dwObjectSerial==lpTransSkilAttackData->dwTarObjectSerial[cnt] &&
					rsPlayInfo[cnt2].smCharInfo.Level>LIMIT_PK_LEVEL ) {


					TransAttackData.code = smTRANSCODE_ATTACKDATA;
					TransAttackData.size = sizeof(TRANS_ATTACKDATA);

					TransAttackData.x = rsPlayInfo[cnt2].Position.x;
					TransAttackData.y = rsPlayInfo[cnt2].Position.y;
					TransAttackData.z = rsPlayInfo[cnt2].Position.z;


					TransAttackData.AttackState = 1;
					TransAttackData.AttackSize  = 32*fONE;
					TransAttackData.Power		= lpTransSkilAttackData->Power;

					TransAttackData.dwDestObjectSerial = lpPlayInfo->dwObjectSerial;
					TransAttackData.dwTarObjectSerial = rsPlayInfo[cnt2].dwObjectSerial;
					TransAttackData.dwChkSum = TransAttackData.Power*2002 + 
						(TransAttackData.x * TransAttackData.y * TransAttackData.z);// ���� ���� �ڵ�

					//���� ���
					rsPlayInfo[cnt2].Send_AttackCount++;
					rsPlayInfo[cnt2].Send_AttackDamage += lpTransSkilAttackData->Power;

					rsPlayInfo[cnt2].lpsmSock->Send2( (char *)&TransAttackData , TransAttackData.size , TRUE );

					break;
				}
			}
		}
	}

	return TRUE;
}


///////////////////////////// ���ο� ���� ��Ŷ //////////////////////////////////


//������ �������� ��ų �ڵ����� �˻�
int rsCheckSkillLevel( rsPLAYINFO *lpPlayInfo , DWORD dwSkillCode )
{
	DWORD	dwCode = dwSkillCode&0xFF;
	DWORD	dwCodeByte = dwSkillCode&0xF;
	int		lv = lpPlayInfo->smCharInfo.Level;

	if ( dwCode>=0xC0 ) return TRUE;

	if ( dwSkillCode==SKILL_PLAY_PIKEWIND || dwSkillCode==SKILL_PLAY_ENCHANT_WEAPON ) return TRUE;

	if ( dwCodeByte==0 || dwCodeByte>=0xD ) {
		//4����ų
		if ( lv<59 ) return FALSE;
	}
	else if ( dwCodeByte>=9 ) {
		//3�� ��ų
		if ( lv<39 ) return FALSE;
	}
	else if ( dwCodeByte>=5 ) {
		//2�� ��ų
		if ( lv<19 ) return FALSE;
	}
	else if ( dwCodeByte>=1 ) {
		//1�� ��ų
		if ( lv<9 ) return FALSE;
	}

	return TRUE;
}



//��ų ���� ��ȯ
int rsSetSkillAttack( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , TRANS_ATTACKDATA *lpTransAttackData , TRANS_ATTACKDATA2 *lpTransAttackData2 ) 
{
	DWORD	dwSkillCode;
	int		Point;
	int		Param;
	int		pow,pow1,pow2;
	int		power[2];
	int		cnt;
	smTRANS_COMMAND	smTransCommand;
	smTRANS_COMMAND_EX	TransCommandEx;


	dwSkillCode = lpTransAttackData2->SkillCode&0xFF;
	Point = ((lpTransAttackData2->SkillCode>>8)&0xF)-1;
	Param = (lpTransAttackData2->SkillCode>>12)&0xF;

	if ( Point<0 || Point>=10 ) return FALSE;

	dm_SkillCode_Temp = dwSkillCode;

	//������ �������� ��ų �ڵ����� �˻�
	if ( rsCheckSkillLevel( lpPlayInfo , dwSkillCode )==FALSE ) {
		if ( lpPlayInfo->WarningCount<5 ) {
			//���� �α� �����
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			smTransCommand.WParam = 8850;
			smTransCommand.LParam = lpPlayInfo->smCharInfo.Level;
			smTransCommand.SParam = dwSkillCode;
			smTransCommand.EParam = Point;
			rsSendDataServer( lpPlayInfo->lpsmSock , &smTransCommand );
			lpPlayInfo->WarningCount++;
		}
		return FALSE;
	}

	switch( dwSkillCode ) {
		case SKILL_PLAY_RAVING:					//���̺�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;	//������ ����

			lpTransAttackData->Power += (lpTransAttackData->Power*Raving_Damage[Point])/100;
			lpTransAttackData->AttackState = (lpTransAttackData->AttackState&0xFFFF0000)+2;		//ũ��Ƽ�� ó��
			break;

		case SKILL_PLAY_IMPACT:					//����Ʈ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;	//������ ����

			lpTransAttackData->Power += (lpTransAttackData->Power*Impact_Damage[Point])/100;
			lpTransAttackData->AttackState = (lpTransAttackData->AttackState&0xFFFF0000)+2;		//ũ��Ƽ�� ó��
			//lpTransAttackData2->Critical[0] = 0;
			break;

		case SKILL_PLAY_CRITICAL_HIT:			//ũ��Ƽ�� ��
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;	//����ũ�� ����

			lpTransAttackData2->Critical[0] += Critical_Hit_Critical[Point];
			break;

		case SKILL_PLAY_JUMPING_CRASH:			//���� Ŭ����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;	//����ũ�� ����

			lpTransAttackData->AttackState = (lpTransAttackData->AttackState&0xFFFF0000)+2;		//ũ��Ƽ�� ó��
			lpTransAttackData->Power += (lpTransAttackData->Power*Jumping_Crash_Damage[Point])/100;

			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_DEMON ) {
				// ��� ���Ϳ��� 30% �߰�
				lpTransAttackData->Power += (lpTransAttackData->Power*30)/100;
			}
			break;

		case SKILL_PLAY_WIND_ARROW:				//���� �ַο�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;	//���� ����

			lpTransAttackData->Power += (lpTransAttackData->Power*Wind_Arrow_Damage[Point])/100;

			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_NORMAL ) {
				// �븻 ���Ϳ��� 30% �߰�
				lpTransAttackData->Power += (lpTransAttackData->Power*30)/100;
			}
			break;

		case SKILL_PLAY_PERFECT_AIM:			//����Ʈ ����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;	//���� ����

			lpTransAttackData->Power += Perfect_Aim_Damage[Point];
			lpTransAttackData->Power += lpPlayInfo->smCharInfo.Level/Perfect_Aim_Damage_LV[Point];

			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_DEMON ) {
				// ��� ���Ϳ��� 30% �߰�
				lpTransAttackData->Power += (lpTransAttackData->Power*30)/100;
			}
			break;

		case SKILL_PLAY_AVALANCHE:				//��������
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;	//���� ����

			lpTransAttackData->Power += (lpTransAttackData->Power*Avalanche_Damage[Point])/100;
			lpTransAttackData2->Critical[0] >>=1;		//�������� �϶� ũ��Ƽ�� ����Ȯ��
			break;

		case SKILL_PLAY_TRIPLE_IMPACT:			//Ʈ���� ����Ʈ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;	//������ ����

			lpTransAttackData->Power += (lpTransAttackData->Power*T_Impact_Damage[Point])/100;
			if ( lpTransAttackData2->MotionCount[0]==2 ) {
				lpTransAttackData->AttackState = (lpTransAttackData->AttackState&0xFFFF0000)+2;		//ũ��Ƽ�� ó��
				lpTransAttackData2->Critical[0] = 0;
			}
			break;

		case SKILL_PLAY_BRUTAL_SWING:			//�θ�Ż ����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;	//������ ����

			lpTransAttackData->Power += (lpTransAttackData->Power*B_Swing_Damage[Point])/100;
			lpTransAttackData2->Critical[0] += (lpTransAttackData2->Critical[0]*B_Swing_Critical[Point])/100;

			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_DEMON ) {
				//�η�Ż ���� ( ��� ���Ϳ��� 40% �߰� )
				lpTransAttackData->Power += (lpTransAttackData->Power*40)/100;
			}
			break;

		case SKILL_PLAY_RAGEOF_ZECRAM:			//��ũ��
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;	//������ ����

			lpTransAttackData->Power += (lpTransAttackData->Power*R_Zecram_Damage[Point])/100;
			lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;		//�� �Ӽ�
			break;

		case SKILL_PLAY_DOUBLE_CRASH:			//���� ũ����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_KNIGHT ) return FALSE;	//����Ʈ ����

			lpTransAttackData->Power += (lpTransAttackData->Power*D_Crash_Damage[Point])/100;
			lpTransAttackData2->Critical[0] += D_Crash_Critical[Point];		//�������� �� Ʋ�� ( �������� ũ��Ƽ�� ��ġ������ �߰� )
			break;

		case SKILL_PLAY_FARINA:					//�и���
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;	//��Ż��Ÿ ����

			 break;

		case SKILL_PLAY_VIGOR_SPEAR:			//��Ž��Ǿ�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;	//��Ż��Ÿ ����

			lpTransAttackData->Power += GetRandomPos( V_Spear_Damage[Point][0] , V_Spear_Damage[Point][1] );
			lpTransAttackData->Power += lpPlayInfo->smCharInfo.Level/2;
			break;

		case SKILL_PLAY_TWIST_JAVELIN:			//Ʈ����Ʈ ���
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;	//��Ż��Ÿ ����

			lpTransAttackData->Power += (lpTransAttackData->Power*Twist_Javelin_Damage[Point])/100;

			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_DEMON ) {
				//��� ���Ϳ��� 30% �߰�
				lpTransAttackData->Power += (lpTransAttackData->Power*30)/100;
			}
			break;

		case SKILL_PLAY_FIRE_JAVELIN:			//���̾� ���
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;	//��Ż��Ÿ ����

			pow = GetRandomPos( Fire_Javelin_Damage[Point][0],Fire_Javelin_Damage[Point][1] );
			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_MUTANT ) {
				//����Ʈ ���Ϳ��� �ҼӼ� 50% �߰�
				pow += (pow*50)/100;
			}

			lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;		//�� �Ӽ�
			lpTransAttackData->AttackState |= pow<<(16+4);					//��ü�� �ҼӼ� ������
			lpTransAttackData->Power += pow;								//��ü ������
			lpTransAttackData->Power += lpPlayInfo->smCharInfo.Level/3;
			break;

		case SKILL_PLAY_HOLY_BOLT:				//Ȧ�� ��Ʈ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS || !lpChar ) return FALSE;	//������Ƽ�� ����

			power[0] = HolyBolt_Damage[Point][0];// + lpTransAttackData2->Critical[1]/8;
			power[1] = HolyBolt_Damage[Point][1];// + lpTransAttackData2->Critical[1]/4;

			// �庰 - ��ų�뷱�� ����(10.08.10)
			power[0]+=lpTransAttackData2->Power2[0]/3; // ����/3
			power[1]+=lpTransAttackData2->Power2[1]/3; // ����/3
			power[0]+=lpTransAttackData2->Critical[1]/8; // ����/8
			power[1]+=lpTransAttackData2->Critical[1]/6; // ����/6

			pow = GetRandomPos( power[0] , power[1] );

			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD )	pow <<=1;  //�𵥵� ������ ����
			lpTransAttackData->Power = pow;
			lpTransAttackData2->Critical[0] = 0;
			break;

		case SKILL_PLAY_MULTI_SPARK:			//��Ƽ ����ũ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS ) return FALSE;	//������Ƽ�� ����

		//	pow1 = M_Spark_Damage[Point][0]+lpTransAttackData2->Power2[0];//+(lpTransAttackData2->Critical[1]/8);
		//	pow2 = M_Spark_Damage[Point][1]+lpTransAttackData2->Power2[1];//+(lpTransAttackData2->Critical[1]/4);

			 // �庰 - ��ų�뷱�� ����(10.08.10)
			pow1 = M_Spark_Damage[Point][0] + (lpTransAttackData2->Critical[1]/8) + (lpTransAttackData2->Power2[0]/3);
			pow2 = M_Spark_Damage[Point][1] + (lpTransAttackData2->Critical[1]/6) + (lpTransAttackData2->Power2[1]/3);

			pow = GetRandomPos( pow1 , pow2 );
			pow *= Param;
			lpTransAttackData->Power = pow;
			lpTransAttackData2->Critical[0] = 0;
			break;

		case SKILL_PLAY_FIRE_BOLT:					//���̾� ��Ʈ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MAGICIAN ) return FALSE;	//������ ����

			power[0] = FireBolt_Damage[Point][0];//+lpTransAttackData2->Critical[1]/8;
			power[1] = FireBolt_Damage[Point][1];//+lpTransAttackData2->Critical[1]/4;

			if ( lpPlayInfo->dwSkill_SpiritElemental_Time ) {		//���Ǹ�������Ż ������ ����
				if ( lpPlayInfo->dwSkill_SpiritElemental_Time>dwPlayServTime ) {
					power[0]+=(power[0]*lpPlayInfo->dwSkill_SpiritElemental_Param)/100;
					power[1]+=(power[1]*lpPlayInfo->dwSkill_SpiritElemental_Param)/100;
				}
				else {
					lpPlayInfo->dwSkill_SpiritElemental_Time = 0;
					lpPlayInfo->dwSkill_SpiritElemental_Param = 0;
				}
			}

			// �庰 - ��ų�뷱�� ����(10.08.10)
			power[0]+= lpTransAttackData2->Power2[0]/3; // ����/3
			power[1]+= lpTransAttackData2->Power2[1]/3; // ����/3
			
			power[0] += lpTransAttackData2->Critical[1]/8; // ����/8
			power[1] += lpTransAttackData2->Critical[1]/6; // ����/6

			pow = GetRandomPos( power[0] , power[1] );
			lpTransAttackData->Power = pow;


			lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;		//�� �Ӽ�
			lpTransAttackData2->Critical[0] = 0;
			break;

		case SKILL_PLAY_DEAD_RAY:				//��������
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MAGICIAN ) return FALSE;	//������ ����

			pow1 = Dead_Ray_Damage[Point][0];
			pow2 = Dead_Ray_Damage[Point][1];

			if ( lpPlayInfo->dwSkill_SpiritElemental_Time ) {		//���Ǹ�������Ż ������ ����
				if ( lpPlayInfo->dwSkill_SpiritElemental_Time>dwPlayServTime ) {
					pow1+=(pow1*lpPlayInfo->dwSkill_SpiritElemental_Param)/100;
					pow2+=(pow2*lpPlayInfo->dwSkill_SpiritElemental_Param)/100;
				}
				else {
					lpPlayInfo->dwSkill_SpiritElemental_Time = 0;
					lpPlayInfo->dwSkill_SpiritElemental_Param = 0;
				}
			}

			/*

			pow1 += lpTransAttackData2->Critical[1]/8;
			pow2 += lpTransAttackData2->Critical[1]/4;

			pow1 += lpTransAttackData2->Power2[0];
			pow2 += lpTransAttackData2->Power2[1];
			*/

			// �庰 - ��ų�뷱�� ����(10.08.10)
			pow1 += lpTransAttackData2->Power2[0];  // ����
			pow2 += lpTransAttackData2->Power2[1];  // ����
			
			pow1 += lpPlayInfo->smCharInfo.Level/2;   // Lv/2
			pow2 += lpPlayInfo->smCharInfo.Level/2;   // Lv/2

			pow1 +=lpTransAttackData2->Critical[1]/4;  // ����/4
			pow2 +=lpTransAttackData2->Critical[1]/2;  // ����/2



			lpTransAttackData->Power = GetRandomPos( pow1, pow2 );

			lpTransAttackData->AttackState |= (sITEMINFO_NONE)<<16;			//���Ӽ�
			lpTransAttackData2->Critical[0] = 0;
			break;

		case SKILL_PLAY_FALCON:					//����
			if ( lpPlayInfo->smCharInfo.JOB_CODE==JOBCODE_ARCHER ) {
				//���� ����
				if ( !lpPlayInfo->dwSKill_Falcon_Time || lpPlayInfo->dwSKill_Falcon_Time<dwPlayServTime ) {
					lpPlayInfo->dwSKill_Falcon_Time = 0;
					return FALSE;
				}

				lpTransAttackData->Power = GetRandomPos( lpPlayInfo->dwSKill_Falcon_Damage[0] , lpPlayInfo->dwSKill_Falcon_Damage[1] );
				lpTransAttackData2->Critical[0] = 0;

				//�������� ������
				if ( lpPlayInfo->dwSkill_ForceOfNature_Time ) {
					if ( lpPlayInfo->dwSkill_ForceOfNature_Time<dwPlayServTime ) {
						lpPlayInfo->dwSkill_ForceOfNature_Time = 0;
					}
					else {
						lpTransAttackData->Power += lpPlayInfo->wSkill_ForceOfNature_Param[1];
					}
				}
				break;
			}
			return FALSE;

		case SKILL_PLAY_DANCING_SWORD:				//��̼ҵ�
			if ( lpPlayInfo->smCharInfo.JOB_CODE==JOBCODE_MAGICIAN ) {
				//������ ��̼ҵ�
				if ( !lpPlayInfo->dwSkill_DancingSword_Time || lpPlayInfo->dwSkill_DancingSword_Time<dwPlayServTime ) {
					lpPlayInfo->dwSkill_DancingSword_Time = 0;
					lpPlayInfo->dwSkill_DancingSword_Param = 0;
					return FALSE;
				}

				pow1 = lpPlayInfo->dwSkill_DancingSword_Param>>16;		//��ų����Ʈ

				if ( lpPlayInfo->dwSkill_DancingSword_DelayTime ) {
					if ( lpPlayInfo->dwSkill_DancingSword_DelayTime>(lpTransAttackData2->dwTime+1000) ) {
						//������ �ð� ����
						return FALSE;
					}
				}
				//���� �ּ� ���ݿ��� �ð� ��� 
				lpPlayInfo->dwSkill_DancingSword_DelayTime = lpTransAttackData2->dwTime+Dancing_Sword_AttackDelay[pow1]*900;

				if ( lpTransAttackData2->Power[0]<lpTransAttackData2->Power2[0] || lpTransAttackData2->Power[1]<lpTransAttackData2->Power2[1] ) {
					//���ⵥ���� ����
					return FALSE;
				}

				if ( (lpPlayInfo->dwSkill_DancingSword_Param&0xFF)==0 ) //����
				{ 
					// �庰 - ��ų�뷱�� ����(10.08.10)
					lpTransAttackData->Power = GetRandomPos(
						Dancing_Sword_IceDamage[pow1][0] + (lpTransAttackData2->Power2[0]/3) + (lpTransAttackData2->Critical[1]/8),
						Dancing_Sword_IceDamage[pow1][1] + (lpTransAttackData2->Power2[1]/3) + (lpTransAttackData2->Critical[1]/6) );

					lpTransAttackData2->Critical[0] = 0;
					lpTransAttackData->AttackState |= (sITEMINFO_ICE+1)<<16;
				}
				else 	//��
				{
					// �庰 - ��ų�뷱�� ����(10.08.10)
					lpTransAttackData->Power = GetRandomPos(
						Dancing_Sword_FireDamage[pow1][0] + (lpTransAttackData2->Power2[0]/3) + (lpTransAttackData2->Critical[1]/8),
						Dancing_Sword_FireDamage[pow1][1] + (lpTransAttackData2->Power2[1]/3) + (lpTransAttackData2->Critical[1]/6) );

					lpTransAttackData2->Critical[0] = 0;
					lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;
				}
				break;
			}
			return FALSE;


		case SKILL_PLAY_HEALING:				//����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS ) return FALSE;	//������Ƽ�� ����

			if ( lpPlayInfo->sLimitDamage[0]<lpTransAttackData2->Power2[0] || lpPlayInfo->sLimitDamage[1]<lpTransAttackData2->Power2[1] ) {
				//���ⵥ���� ����
				return FALSE;
			}

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_HEALING;
			/*
			smTransCommand.WParam =	GetRandomPos( 
				Healing_Heal[Point][0]+lpTransAttackData2->Power2[0]+(lpTransAttackData2->Critical[1]/8),
				Healing_Heal[Point][1]+lpTransAttackData2->Power2[1]+(lpTransAttackData2->Critical[1]/4) );	//�����
				*/

			// �庰 - ��ų�뷱�� ����(10.08.10)
			smTransCommand.WParam =	GetRandomPos( 
				Healing_Heal[Point][0]+(lpTransAttackData2->Power2[0]/3)+(lpTransAttackData2->Critical[1]/8),
				Healing_Heal[Point][1]+(lpTransAttackData2->Power2[1]/3)+(lpTransAttackData2->Critical[1]/6) );	//�����

			smTransCommand.LParam =	0;								//
			smTransCommand.SParam =	lpTransAttackData2->dwTarObjectSerial;			//ġ�� ���
			smTransCommand.EParam = lpPlayInfo->dwObjectSerial;	//
			rsPlayHealing( &smTransCommand );									//���������Լ�
			return FALSE;



		case SKILL_PLAY_GREAT_HEALING:			//�׷��� ����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS ) return FALSE;	//������Ƽ�� ����

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_GRAND_HEALING;
			smTransCommand.WParam =	Point+1;					//��ų����Ʈ

			// �庰 - ��ų�뷱�� ����(10.08.10)
			smTransCommand.LParam =	GetRandomPos( 
				Grand_Healing[Point][0] + (lpTransAttackData2->Critical[1]/8) + (lpTransAttackData2->Power2[0]/3) , 
				Grand_Healing[Point][1] + (lpTransAttackData2->Critical[1]/6) + (lpTransAttackData2->Power2[1]/3) );

			/*
			smTransCommand.LParam =	GetRandomPos( 
				Grand_Healing[Point][0]+lpTransAttackData2->Power2[0]+(lpTransAttackData2->Critical[1]/8) , 
				Grand_Healing[Point][1]+lpTransAttackData2->Power2[1]+(lpTransAttackData2->Critical[1]/4) );
				*/

			smTransCommand.SParam =	Grand_Healing_Range[Point];
			smTransCommand.EParam = lpPlayInfo->dwObjectSerial;
			rsPlayGrandHealing( lpPlayInfo , &smTransCommand );
			return FALSE;


		case SKILL_PLAY_SPARK_SHIELD:		//����ũ����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MECHANICIAN ) return FALSE;	//��ī�ϼ�
			if ( !lpChar || !lpPlayInfo->dwSkill_SparkShield_Time || lpPlayInfo->dwSkill_SparkShield_Time<dwPlayServTime ) {
				//����ũ ���� ���� ����
				lpPlayInfo->dwSkill_SparkShield_Time = 0;
				return FALSE;
			}

			pow1 = lpPlayInfo->dwSkill_SparkShield_Param>>16;
			pow2 = lpPlayInfo->dwSkill_SparkShield_Param&0xFFFF;
			pow = GetRandomPos(pow1,pow2);
			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_MECHANIC ) {
				//��ī�� ���Ϳ��� �ڼӼ� 50% �߰�
				pow += (pow*50)/100;
			}

			lpTransAttackData->AttackState |= (sITEMINFO_LIGHTING+1)<<16;		//�� �Ӽ�
			lpTransAttackData->Power = pow;										//��ü ������
			lpTransAttackData2->Critical[0] = 0;

			smTransCommand.code = smTRANSCODE_PROCESS_SKILL;;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = SKILL_PLAY_SPARK_SHIELD;
			smTransCommand.LParam = lpPlayInfo->dwObjectSerial;
			smTransCommand.SParam = lpChar->dwObjectSerial;
			rsSendCommandUser2( lpPlayInfo , &smTransCommand );
			break;

		case SKILL_PLAY_GRAND_SMASH:		//�׷��� ���Ž�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MECHANICIAN ) return FALSE;	//��ī�ϼ�
			lpTransAttackData->Power += (lpTransAttackData->Power*Grand_Smash_Damage[Point])/100;
			break;

		case SKILL_PLAY_AVANGING_CRASH:		//���¡ ũ����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;		//������
			lpTransAttackData->Power += (lpTransAttackData->Power*A_Crash_Damage[Point])/100;
			if ( lpTransAttackData2->Power2[0]>0 && lpTransAttackData2->Power2[0]<=10 ) {
				lpTransAttackData2->Critical[0] += B_Swing_Critical[ lpTransAttackData2->Power2[0] ]/2;
			}
			break;

		case SKILL_PLAY_BONE_SMASH:			//�� ���Ž�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;		//������

			lpTransAttackData->Power += (lpTransAttackData->Power*B_Crash_Damage[Point])/100;

			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_DEMON ) {
				//���
				lpTransAttackData->Power += (lpTransAttackData->Power*B_Crash_DemonDamage[Point])/100;
			}
			break;

		case SKILL_PLAY_CHAIN_LANCE:		//ü�η���
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;	//����ũ�� ����
			lpTransAttackData->Power += (lpTransAttackData->Power*Chain_Lance_Damage[Point])/100;
			break;


		case SKILL_PLAY_ELEMENTAL_SHOT:		//������Ż�� 
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;	//���� ����
			if ( Param==0 ) {	//��
				lpTransAttackData2->Critical[0] = 0;
				pow = GetRandomPos( Elemental_Shot_Fire[Point][0], Elemental_Shot_Fire[Point][1] );
				lpTransAttackData->Power += pow;
				lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;		//�� �Ӽ�
				lpTransAttackData->AttackState |= pow<<(16+4);					//��ü�� �ҼӼ� ������
			}
			break;


		case SKILL_PLAY_HOLY_INCANTATION:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_KNIGHT ) return FALSE;	//����Ʈ ����
			//��� �߰�����
			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {

				pow = lpChar->smCharInfo.Resistance[sITEMINFO_BIONIC];		//�� �Ӽ� ����
				if ( pow<0 ) pow=0;
				if ( pow>100 ) pow=100;
				pow = 100-pow;

				pow = (H_Incantation_Success[Point]*pow)/100;
				if ( (rand()%100)<pow ) {
					lpChar->lpMasterPlayInfo = lpPlayInfo;
					lpChar->smMonsterInfo.dwAutoCharCode = 0;
					lpChar->smCharInfo.bUpdateInfo[0]++;
					lpChar->PlayHolyIncantation[0] = (short)lpChar->smCharInfo.Brood;
					lpChar->PlayHolyIncantation[1] = H_Incantation_Time[Point]*17;

					//lpChar->smCharInfo.Life[0] = (lpChar->smCharInfo.Life[0]*H_Incantation_AddLife[Point])/100;
					//if ( lpChar->smCharInfo.Life[0]>lpChar->smCharInfo.Life[1] ) lpChar->smCharInfo.Life[0]=lpChar->smCharInfo.Life[1];
			//		lpChar->smCharInfo.Life[0]=lpChar->smCharInfo.Life[1]; // �庰 - ���� �ҽ�

					// �庰 - Ȧ�� �������̼� ����
					lpChar->smCharInfo.Life[0] = lpChar->smCharInfo.Life[1] = (lpChar->smCharInfo.Life[0]*H_Incantation_AddLife[Point])/100;



					lpChar->smCharInfo.Next_Exp = lpPlayInfo->dwObjectSerial;		//���� �ø��� �ڵ� ����
					lpChar->smCharInfo.Brood = smCHAR_MONSTER_USER;
					lstrcpy( lpChar->smCharInfo.szModelName2+1 , lpPlayInfo->smCharInfo.szName );	//���� �̸�
					lpChar->lpTargetChar = 0;
					lpChar->lpTargetPlayInfo = 0;

					//���� ��Ȥ�� ����Ʈ ȣ��
					ZeroMemory( &TransCommandEx , sizeof(smTRANS_COMMAND_EX) );
					TransCommandEx.WParam = smCOMMNAD_USER_HOLY_INCANTATION;
					TransCommandEx.LParam = lpChar->dwObjectSerial;
					TransCommandEx.SParam = H_Incantation_Time[Point];
					TransCommandEx.EParam = 0;
					rsSendCommandUser( lpPlayInfo , &TransCommandEx );			//����Ʈ ȣ��
				}
			}
			return FALSE;

		case SKILL_PLAY_GRAND_CROSS:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_KNIGHT ) return FALSE;		//����Ʈ ����

			lpTransAttackData->Power += (lpTransAttackData->Power*Grand_Cross_Damage[Point])/100;

			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
				//�𵥵�
				lpTransAttackData->Power += (lpTransAttackData->Power*Grand_Cross_UndeadDamage[Point])/100;
			}
			break;

		case SKILL_PLAY_SPLIT_JAVELIN:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;		//��Ż��Ÿ ����
			if ( lpTransAttackData2->Power2[0]>Split_Javelin_AttackNum[Point] ) return FALSE;

			lpTransAttackData->Power = 0;

			//�߻��� ������ŭ ���ݷ� ���
			for(cnt=0;cnt<lpTransAttackData2->Power2[0];cnt++) {
				pow = GetRandomPos( lpTransAttackData2->Power[0] , lpTransAttackData2->Power[1] );
				pow += (pow*Split_Javelin_Damage[Point])/100;
				lpTransAttackData->Power += pow;
			}
			lpTransAttackData2->Critical[0] = 0;		//ũ��Ƽ�� ���� ����
			break;

		case SKILL_PLAY_VIGOR_BALL:		//��ź�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS || !lpChar ) return FALSE;	//������Ƽ�� ����

			if ( lpTransAttackData2->Power[0]>lpTransAttackData2->Power2[0] && lpTransAttackData2->Power[1]>lpTransAttackData2->Power2[1] ) {

				/*
				pow1 = lpTransAttackData2->Power2[0]+Vigor_Ball_Damage[Point][0]+(lpTransAttackData2->Critical[1]/8);
				pow2 = lpTransAttackData2->Power2[1]+Vigor_Ball_Damage[Point][1]+(lpTransAttackData2->Critical[1]/8);
				*/

				// �庰 - ��ų�뷱�� ����(10.08.10)
				pow1 = Vigor_Ball_Damage[Point][0] + (lpPlayInfo->smCharInfo.Level/2) + (lpTransAttackData2->Critical[1]/6);
				pow2 = Vigor_Ball_Damage[Point][1] + (lpPlayInfo->smCharInfo.Level/2) + (lpTransAttackData2->Critical[1]/3);


				lpTransAttackData->Power = GetRandomPos( pow1, pow2 );
				lpTransAttackData2->Critical[0] = 0;		//ũ��Ƽ�� ���� ����
				break;
			}
			return FALSE;

		case SKILL_PLAY_PET_ATTACK:
			//�꿡 ���� ����

			//����� - ���� ����� ��
			//	if ( lpPlayInfo->smCharInfo.Level>10 ) return FALSE;		//���� 10������

			pow1 =0;
			pow2 =0;

			switch( Point ) 
			{
			case PET_TERRY:
				//	pow1 = 2 + lpPlayInfo->smCharInfo.Level/3;
				//	pow2 = 5 + lpPlayInfo->smCharInfo.Level/3;
				// �庰 - ���� ����� �� ������ ����
				pow1 = lpPlayInfo->smCharInfo.Level / 2;
				pow2 = (lpPlayInfo->smCharInfo.Level * 95)/100;
				lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;
				break;

			case PET_NEPSYS:
				//	pow1 = 1;
				//	pow2 = 2;
				// �庰 - ���� ����� �� ������ ����
				pow1 = lpPlayInfo->smCharInfo.Level / 2;
				pow2 = (lpPlayInfo->smCharInfo.Level * 95)/100;
				lpTransAttackData->AttackState |= (sITEMINFO_ICE+1)<<16;
				break;

			case PET_IO:
				//	pow1 = 1;
				//	pow2 = 6 + lpPlayInfo->smCharInfo.Level/2;
				// �庰 - ���� ����� �� ������ ����
				pow1 = (lpPlayInfo->smCharInfo.Level * 80)/100 + 1;
				pow2 = lpPlayInfo->smCharInfo.Level + (lpPlayInfo->smCharInfo.Level * 80)/100 + 2;
				break;

			// �庰 - ���ο� �巡��
			case PET_MUTE:
				//	pow1 = 1;
				//	pow2 = 6 + lpPlayInfo->smCharInfo.Level/2;
				// �庰 - ���� ����� �� ������ ����
				pow1 = (lpPlayInfo->smCharInfo.Level)/2;
				pow2 = lpPlayInfo->smCharInfo.Level + (lpPlayInfo->smCharInfo.Level * 95)/100 ;
			break;
			}
			
			// ����� - ���� ����� �� ���� ������ 2��� ����
		//	pow1 = 1 + (lpPlayInfo->smCharInfo.Level*80)/100;
		//	pow2 = 2 + (lpPlayInfo->smCharInfo.Level) + (lpPlayInfo->smCharInfo.Level*80)/100;

			lpTransAttackData->Power = GetRandomPos( pow1, pow2 );
			lpTransAttackData2->Critical[0] = 0;		//ũ��Ƽ�� ���� ����
			break;

		case SKILL_PLAY_PET_ATTACK2:
			//PC�� �� ����

			//if ( !lpPlayInfo->Bl_RNo || (dwPlayServTime-lpPlayInfo->dwPCBang_Pet_AttackTime)<1000 ) return FALSE;		//1�� ���� ������ ����
			
			// pluto ��(�ؿ�) ����
			if ( !lpPlayInfo->dwTime_PrimeItem_PhenixPet || (dwPlayServTime-lpPlayInfo->dwPCBang_Pet_AttackTime)<1000 ) return FALSE;		//1�� ���� ������ ����

			lpPlayInfo->dwPCBang_Pet_AttackTime = dwPlayServTime;

			pow1 =0;
			pow2 =0;

			switch( Point ) {
				case PCBANG_PET_BURSTP:
				case PCBANG_PET_BURSTS:
				case PCBANG_PET_BURSTL:
				case PCBANG_PET_BURSTD:
					pow1 = lpPlayInfo->smCharInfo.Level/2+1;
					pow2 = lpPlayInfo->smCharInfo.Level+lpPlayInfo->smCharInfo.Level/2+1;
					break;
			}

			lpTransAttackData->Power = GetRandomPos( pow1, pow2 );
			lpTransAttackData2->Critical[0] = 0;		//ũ��Ƽ�� ���� ����
			break;


		/////////////////// 4����ų ////////////////

		case SKILL_PLAY_IMPULSION:
			if ( lpPlayInfo->smCharInfo.JOB_CODE==JOBCODE_MECHANICIAN ) {
				pow1 = Impulsion_LightingDamage[Point][0];
				pow2 = Impulsion_LightingDamage[Point][1];

				lpTransAttackData->Power = GetRandomPos( pow1, pow2 );

				//��ī�� �� 30% �߰�
				if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_MECHANIC ) {
					lpTransAttackData->Power+=(lpTransAttackData->Power*30)/100;
				}

				lpTransAttackData2->Critical[0] = 0;
				break;
			}
			return FALSE;


		case SKILL_PLAY_MAGNETIC_SPHERE:
			if ( lpPlayInfo->smCharInfo.JOB_CODE==JOBCODE_MECHANICIAN ) {
				if ( !lpPlayInfo->dwSkill_MagneticSphere_Time || lpPlayInfo->dwSkill_MagneticSphere_Time<dwPlayServTime ) {
					lpPlayInfo->dwSkill_MagneticSphere_Time = 0;
					return FALSE;
				}

				pow1 = Magnetic_Sphere_AddDamage[lpPlayInfo->dwSkill_MagneticSphere_Param][0];
				pow2 = Magnetic_Sphere_AddDamage[lpPlayInfo->dwSkill_MagneticSphere_Param][1];

				lpTransAttackData->Power = GetRandomPos( pow1, pow2 );
				lpTransAttackData2->Critical[0] = 0;
				lpTransAttackData->AttackState |= (sITEMINFO_LIGHTING+1)<<16;		//�� �Ӽ�

				//��ī�� �� 30% �߰�
				if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_MECHANIC ) {
					lpTransAttackData->Power+=(lpTransAttackData->Power*30)/100;
				}
				break;
			}
			return FALSE;


		case SKILL_PLAY_DESTROYER:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;

			lpTransAttackData->AttackState = (lpTransAttackData->AttackState&0xFFFF0000)+2;		//ũ��Ƽ�� ó��
			lpTransAttackData->Power += (lpTransAttackData->Power*Destoryer_DamagePercent[Point])/100;

			lpTransAttackData2->Critical[0] += Destoryer_AddCritical[Point];

			if ( lpChar && lpChar->smCharInfo.Brood==smCHAR_MONSTER_DEMON ) {
				// ��� ���Ϳ��� 30% �߰�
				lpTransAttackData->Power += (lpTransAttackData->Power*30)/100;
			}
			break;
/*
		case SKILL_PLAY_CYCLONE_STRIKE:
			if ( lpPlayInfo->smCharInfo.JOB_CODE==JOBCODE_FIGHTER ) {
				lpTransAttackData->Power+= (lpTransAttackData->Power*Cyclone_Strike_DamagePercent[Point])/100;
				//lpTransAttackData2->Critical[0] = 0;
				break;
			}
			return FALSE;
*/
		case SKILL_PLAY_ASSASSIN_EYE:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;

			lpPlayInfo->dwSkill_AssassinEye_Time = dwPlayServTime+Assassin_Eye_Time[Point]*1000;
			lpPlayInfo->dwSkill_AssassinEye_Param = Point;

			if ( lpChar ) {
				lpChar->dwAssassinEyeTime = lpPlayInfo->dwSkill_AssassinEye_Time;
				lpChar->AssassinEyeParam = Assassin_Eye_M_SubCritical[Point];
			}
			return FALSE;

		case SKILL_PLAY_CHARGING_STRIKE:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;

			if ( !lpTransAttackData2->Power2[0] ) 
				lpTransAttackData->Power += (lpTransAttackData->Power*Charging_Strike_CharingDamagePercent[Point])/100;
			else
				lpTransAttackData->Power += (lpTransAttackData->Power*Charging_Strike_DamagePercent[Point])/100;
			break;

		case SKILL_PLAY_SHADOW_MASTER:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;

			lpTransAttackData->Power += (lpTransAttackData->Power*Shadow_Master_DamagePercent[Point])/100;
			break;

		case SKILL_PLAY_PHOENIX_SHOT:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;
				lpTransAttackData->Power += (lpTransAttackData->Power*Phoenix_Shot_DamagePercent[Point])/100;
			break;

		case SKILL_PLAY_DIVINE_PIERCING:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_KNIGHT ) return FALSE;

			lpTransAttackData->Power += (lpTransAttackData->Power*Divine_Piercing_DamagePercent[Point])/100;
			break;

		case SKILL_PLAY_VENGEANCE: // �庰 - ������ ����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;

			lpTransAttackData->Power += (lpTransAttackData->Power*Vengeance_DamagePercent[Point])/100;
			break;

		case SKILL_PLAY_SUMMON_MUSPELL:
			if ( lpPlayInfo->smCharInfo.JOB_CODE==JOBCODE_PRIESTESS ) {
				if ( !lpPlayInfo->dwSkill_SummonMuspell_Time || lpPlayInfo->dwSkill_SummonMuspell_Time<dwPlayServTime ) {
					lpPlayInfo->dwSkill_SummonMuspell_Time = 0;
					return FALSE;
				}

				pow1 = Summon_Muspell_Damage[lpPlayInfo->dwSkill_SummonMuspell_Param][0];
				pow2 = Summon_Muspell_Damage[lpPlayInfo->dwSkill_SummonMuspell_Param][1];

				lpTransAttackData->Power = GetRandomPos( pow1, pow2 );
				lpTransAttackData2->Critical[0] = 0;
				break;
			}

	}


	return TRUE;
}

//��ų ���� ��ȯ
int rsSetSkillPlus( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , TRANS_ATTACKDATA *lpTransAttackData , TRANS_ATTACKDATA2 *lpTransAttackData2 )
{
	int pow;
	DWORD dwWeaponCode;
	smTRANS_COMMAND	smTransCommand;
	DWORD 	dwSkillCode = lpTransAttackData2->SkillCode&0xFF;

	DWORD	dwAttSkillCode = 0;
	
	if ( !lpPlayInfo->AdminMode )
		dwAttSkillCode = lpTransAttackData2->SkillCode;


	if ( lpPlayInfo->dwSkill_Maximize_Time && !dwAttSkillCode ) {
		//�ƽø����� ���� ( �ִ� ���ݷ� ���� �ִ�ġ ��ŭ �߰� )
		if ( lpPlayInfo->dwSkill_Maximize_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_Maximize_Time = 0;
			lpPlayInfo->dwSkill_Maximize_Param = 0;
			return TRUE;
		}

		pow = lpTransAttackData2->Power[1]-lpTransAttackData2->Power2[1];
		if ( pow>=0 && pow<lpTransAttackData2->Power[1] ) {
			lpTransAttackData2->Power[1] += (pow * lpPlayInfo->dwSkill_Maximize_Param)/100;
			lpTransAttackData->Power = GetRandomPos( lpTransAttackData2->Power[0] , lpTransAttackData2->Power[1] );
		}
	}

	if ( lpPlayInfo->dwSkill_AutoMation_Time && !dwAttSkillCode ) {
		//������̼� ���� ( ���ݷ��� ���� ������ �� ������ �߰� )
		if ( lpPlayInfo->dwSkill_AutoMation_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_AutoMation_Time = 0;
			lpPlayInfo->dwSkill_AutoMation_Param = 0;
			return TRUE;
		}

		dwWeaponCode = lpTransAttackData2->dwWeaponCode&sinITEM_MASK2;
		if ( dwWeaponCode==sinWS1 || dwWeaponCode==sinWT1 ) {		//���� Ȱ , ��� �迭�� ����
			pow = lpTransAttackData2->Power[1]-lpTransAttackData2->Power2[1];
			if ( pow>=0 && pow<lpTransAttackData2->Power[1] ) 
				lpTransAttackData2->Power[1] += (pow * lpPlayInfo->dwSkill_AutoMation_Param)/100;

			pow = lpTransAttackData2->Power[0]-lpTransAttackData2->Power2[0];
			if ( pow>=0 && pow<lpTransAttackData2->Power[0] ) 
				lpTransAttackData2->Power[0] += (pow * lpPlayInfo->dwSkill_AutoMation_Param)/100;

			lpTransAttackData->Power = GetRandomPos( lpTransAttackData2->Power[0] , lpTransAttackData2->Power[1] );
		}
	}

	if ( lpPlayInfo->dwSkill_TriumphOfValhalla_Time && !dwAttSkillCode ) {
		//Ʈ���̾��� ���Ҷ�
		if ( lpPlayInfo->dwSkill_TriumphOfValhalla_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_TriumphOfValhalla_Time = 0;
			lpPlayInfo->dwSkill_TriumphOfValhalla_Param = 0;
			return TRUE;
		}

		pow = GetRandomPos( lpTransAttackData2->Power[0] , lpTransAttackData2->Power[1]+(short)lpPlayInfo->dwSkill_TriumphOfValhalla_Param );
		lpTransAttackData->Power = pow;
	}

	if ( lpPlayInfo->dwSkill_HallOfValhalla_Time && !dwAttSkillCode ) {
		//Ȧ���� ���Ҷ�
		if ( lpPlayInfo->dwSkill_HallOfValhalla_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_HallOfValhalla_Time = 0;
			return TRUE;
		}

		pow = GetRandomPos( lpTransAttackData2->Power[0] , lpTransAttackData2->Power[1]+(short)lpPlayInfo->wSkill_HallOfValhalla_Param[1] );
		lpTransAttackData->Power = pow;
	}

	if ( lpPlayInfo->dwSkill_Vanish_Time ) {
		//��Ͻ�
		if ( lpPlayInfo->dwSkill_Vanish_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_Vanish_Time = 0;
			lpPlayInfo->dwSkill_Vanish_Param = 0;
			return TRUE;
		}

		lpTransAttackData->Power += (lpTransAttackData->Power*Vanish_Damage[lpPlayInfo->dwSkill_Vanish_Param])/100;

		lpPlayInfo->dwSkill_Vanish_Time = 0;
		lpPlayInfo->dwSkill_Vanish_Param = 0;

		//��Ͻ� �����ϱ�
		smTransCommand.code = smTRANSCODE_PROCESS_SKILL;
		smTransCommand.size = sizeof( smTRANS_COMMAND );
		smTransCommand.WParam  = SKILL_PLAY_VANISH;
		smTransCommand.LParam  = 0;
		smTransCommand.SParam  = 0;
		smTransCommand.EParam  = 0;
		lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}

	if ( lpPlayInfo->dwSkill_Berserker_Time ) {
		//����Ŀ
		if ( lpPlayInfo->dwSkill_Berserker_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_Berserker_Time = 0;
			lpPlayInfo->dwSkill_Berserker_Param = 0;
			return TRUE;
		}

		lpTransAttackData->Power += Berserker_AddAttack[lpPlayInfo->dwSkill_Berserker_Param];
	}

	if ( lpPlayInfo->dwSkill_ForceOfNature_Time ) {
		//�������� ������
		if ( lpPlayInfo->dwSkill_ForceOfNature_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_ForceOfNature_Time = 0;
			return TRUE;
		}

		if ( dwSkillCode!=SKILL_PLAY_FALCON )
			lpTransAttackData->Power += lpPlayInfo->wSkill_ForceOfNature_Param[0];
	}


	if ( lpPlayInfo->dwSkill_GodsBless_Time ) {
		//������
		if ( lpPlayInfo->dwSkill_GodsBless_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_GodsBless_Time = 0;
			lpPlayInfo->dwSkill_GodsBless_Param = 0;
			return TRUE;
		}

		lpTransAttackData->Power += lpPlayInfo->dwSkill_GodsBless_Param;
	}

	if ( lpPlayInfo->dwSkill_FrostJavelin_Time && !dwAttSkillCode ) {
		//���ν�Ʈ���
		if ( lpPlayInfo->dwSkill_FrostJavelin_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_FrostJavelin_Time = 0;
			lpPlayInfo->dwSkill_FrostJavelin_Param = 0;
			return TRUE;
		}

		dwWeaponCode = lpTransAttackData2->dwWeaponCode&sinITEM_MASK2;
		if ( dwWeaponCode==sinWT1 ) {		//���� ��� �迭�� ����
			pow = GetRandomPos( Frost_Javelin_IceAddDamage[lpPlayInfo->dwSkill_FrostJavelin_Param][0],
								Frost_Javelin_IceAddDamage[lpPlayInfo->dwSkill_FrostJavelin_Param][1] );

			lpTransAttackData->AttackState |= (sITEMINFO_ICE+1)<<16;
			lpTransAttackData->AttackState |= pow<<(16+4);		//��ü�� �����Ӽ� ������
			lpTransAttackData->Power += pow;
		}
	}

	if ( lpPlayInfo->dwBlessSkill_Code==SKILL_PLAY_BLESS_DAMAGE ) {	//Ŭ����ų (�������߰�)
		if ( lpPlayInfo->dwBlessSkill_Time>dwPlayServTime ) {
			lpTransAttackData->Power += (lpTransAttackData->Power*lpPlayInfo->dwBlessSkill_Param)/100;
		}
		else {
			lpPlayInfo->dwBlessSkill_Code = 0;
			lpPlayInfo->dwBlessSkill_Time = 0;
			return TRUE;
		}
	}

	//���� ��ũ��
	if (  lpPlayInfo->dwSiegeItem_Scroll_Time ) {
		if ( lpPlayInfo->dwSiegeItem_Scroll_Time<dwPlayServTime ) {
			lpPlayInfo->dwSiegeItem_Scroll_Code = 0;
			lpPlayInfo->dwSiegeItem_Scroll_Time = 0;
		}
		else {
			switch( lpPlayInfo->dwSiegeItem_Scroll_Code ) {
				case (sinBI1|sin05):					//���� ��ũ��
					lpTransAttackData->Power /= lpPlayInfo->dwSiegeItem_Scroll_Param;			//���ݷ� ����
					break;
				case (sinBI1|sin06):					//ũ��Ƽ�� ��ũ��
					lpTransAttackData2->Critical[0] += (short)lpPlayInfo->dwSiegeItem_Scroll_Param;		//ũ��Ƽ�� 2�� ����
					break;
			}

			if ( lpPlayInfo->Position.Area==rsCASTLE_FIELD ) {
				switch( lpPlayInfo->dwSiegeItem_Scroll_Code ) {
					case (sinBC1|sin01):					//���� ��ũ��
						lpTransAttackData->Power /= lpPlayInfo->dwSiegeItem_Scroll_Param;			//���ݷ� ����
						break;
					case (sinBC1|sin02):					//ũ��Ƽ�� ��ũ��
						lpTransAttackData2->Critical[0] += (short)lpPlayInfo->dwSiegeItem_Scroll_Param;		//ũ��Ƽ�� 2�� ����
						break;
				}
			}
		}
	}


	return TRUE;
}


//���� ũ��Ƽ�� 
int GetCritical( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , int Critical_Hit )
{
	int Result;

	Result = Critical_Hit + (((lpPlayInfo->smCharInfo.Level - lpChar->smCharInfo.Level)*25)/100);

	//��ž��� ��ų ũ��Ƽ�� ��ȭ
	/*
	if ( lpPlayInfo->dwSkill_AssassinEye_Time && Critical_Hit>0 ) {

		if ( lpPlayInfo->dwSkill_AssassinEye_Time>dwPlayServTime ) {	
			Result += Assassin_Eye_AddCritical[ lpPlayInfo->dwSkill_AssassinEye_Param ];
		}
		else {
			lpPlayInfo->dwSkill_AssassinEye_Time = 0;
			lpPlayInfo->dwSkill_AssassinEye_Param = 0;
		}
	}
	*/
	if ( lpChar->dwAssassinEyeTime && Critical_Hit>0 ) {
		if ( lpChar->dwAssassinEyeTime>dwPlayServTime ) {	
			Result -= lpChar->AssassinEyeParam;
		}
		else {
			lpChar->dwAssassinEyeTime = 0;
			lpChar->AssassinEyeParam = 0;
		}
	}



	if(Result > 70)
		Result = 70;
	
	return Result;
}

//���� ũ��Ƽ�� 
int GetCritical2( rsPLAYINFO *lpPlayInfo , rsPLAYINFO *lpPlayInfo2 , int Critical_Hit )
{
	int Result;

	Result = Critical_Hit + (((lpPlayInfo->smCharInfo.Level - lpPlayInfo2->smCharInfo.Level)*25)/100);
	if(Result > 70)
		Result = 70;
	
	return Result;
}

//�������� �Ŀ���
int AddForceOrbPower( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , TRANS_ATTACKDATA	*lpTransAttackData , DWORD dwSkillCode , int MinDamage , int MaxDamage )
{
	int damage = 0;
	int	sJobLevel;
	int	damage2;
	
	// ����� - ���� ����, ���� ���� ������ �Ϲݽ�ų�� ��Ÿ���� ������ ���� �����Ƿ� return FALSE
	if ( (lpPlayInfo->dwForceOrb_Code&sinITEM_MASK3) > sin14 ) return FALSE;
	if ( !lpPlayInfo->dwForceOrb_Time ) return FALSE;
	if ( lpPlayInfo->dwForceOrb_Time<dwPlayServTime ) {
		lpPlayInfo->dwForceOrb_Time = 0;
		lpPlayInfo->dwForceOrb_Code = 0;
		lpPlayInfo->dwForceOrb_Damage = 0;
		return FALSE;
	}

	//�Ϻ� ��ų ���� ����
	switch ( dwSkillCode&0xFF )
	{
	case SKILL_PLAY_RAGEOF_ZECRAM:
	case SKILL_PLAY_SPARK_SHIELD:
	case SKILL_PLAY_FALCON:
	case SKILL_PLAY_DANCING_SWORD:
	case SKILL_PLAY_SUMMON_MUSPELL:	//���չ����� �� ���� ���ݷ� �߰� ���׸� �Ϻ��� �䱸�� ���� (2005��7��14��)
	case SKILL_PLAY_PET_ATTACK:
	case SKILL_PLAY_PET_ATTACK2:


		return FALSE;
	}

	damage = lpPlayInfo->dwForceOrb_Damage;

	// ����� : ���� ���� �߰�
	if ( lpPlayInfo->dwForceOrb_Code==(sinFO1|sin07) || lpPlayInfo->dwForceOrb_Code==(sinFO1|sin08) || lpPlayInfo->dwForceOrb_Code==(sinFO1|sin09) || lpPlayInfo->dwForceOrb_Code==(sinFO1|sin10) ) 
	{
		//��Ű ����, ����� ����, �췹���� ����, �̶��� ���� ���ݷ� 10%����
		damage2 = (MaxDamage+MinDamage)/2;
		if ( damage2>0 && damage2<1000 )
			damage += (damage2*10)/100;
	}
	else if( lpPlayInfo->dwForceOrb_Code==(sinFO1|sin11) )
	{
		//���丣�� ���� 15%����
		damage2 = (MaxDamage+MinDamage)/2;
		if ( damage2>0 && damage2<1000 )
			damage += (damage2*15)/100; // ���ݷ� 15%����
	}
	else if( lpPlayInfo->dwForceOrb_Code==(sinFO1|sin12) )
	{
		//�̴ϱ׸� ���� 20%����
		damage2 = (MaxDamage+MinDamage)/2;
		if ( damage2>0 && damage2<1000 )
			damage += (damage2*20)/100; //���ݷ� 20%����
	}

	if ( dwSkillCode ) {
		sJobLevel = dwSkillCode&0xF;
		sJobLevel --;
		if ( sJobLevel>=0 && sJobLevel<0x10 ) {
			sJobLevel>>=2;
			switch( sJobLevel ) {
				case 0:		//1����ų
					if ( (dwSkillCode&0xFF)==SKILL_PLAY_MULTI_SPARK )
						damage = (damage*50)/100;
					else
						damage = (damage*80)/100;

					break;
				case 1:		//2����ų
					damage = (damage*90)/100;
					break;
				case 2:		//3����ų

					break;
				case 3:		//4����ų
					damage = (damage*150)/100;
					break;
			}

		}
	}

	lpTransAttackData->Power += damage;

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
	if ( lpChar ) {					//�ؿ�
		wsprintf( szDispDamage , ">%s Attack ( ForceOrb + %d ) (%d)" , lpChar->smCharInfo.szName , damage , lpTransAttackData->Power );
		rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
	}
	else {							//�ؿ�
		wsprintf( szDispDamage , "> Attack ( ForceOrb + %d ) (%d)" , damage , lpTransAttackData->Power );
		rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
	}
}
#endif
	return TRUE;
}

// ����� - ���� ���� �߰�, ���� ���� ���� �߰�(�������ݿ� �������� �Ŀ���)
int AddRangeForceOrbPower( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , TRANS_SKIL_ATTACKDATA	*lpTransSkillAttackData , DWORD dwSkillCode , int MinDamage , int MaxDamage )
{
	int damage = 0;
	int	sJobLevel;
	int	damage2;

	if( (lpPlayInfo->dwForceOrb_Code&sinITEM_MASK2) == sinFO1 ) // ����� - �Ϲ� ���� �����߿� ������ ��ų�� ����ϸ� ���Ͻ�Ų��.
	{
		if( (lpPlayInfo->dwForceOrb_Code&sinITEM_MASK3) < sin21 )
			return FALSE;
	}
	if ( !lpPlayInfo->dwForceOrb_Time ) return FALSE;
	if ( lpPlayInfo->dwForceOrb_Time<dwPlayServTime ) {
		lpPlayInfo->dwForceOrb_Time = 0;
		lpPlayInfo->dwForceOrb_Code = 0;
		lpPlayInfo->dwForceOrb_Damage = 0;
		return FALSE;
	}

	//�Ϻ� ��ų ���� ����
	switch ( dwSkillCode&0xFF )
	{
		//	case SKILL_PLAY_RAGEOF_ZECRAM: // ����� - ������ ���� ��ũ�� ��ų�� ���� ���� �� �����Ų��.
	case SKILL_PLAY_SPARK_SHIELD:
	case SKILL_PLAY_FALCON:
	case SKILL_PLAY_DANCING_SWORD:
	case SKILL_PLAY_SUMMON_MUSPELL:	//���չ����� �� ���� ���ݷ� �߰� ���׸� �Ϻ��� �䱸�� ���� (2005��7��14��)
	case SKILL_PLAY_PET_ATTACK:
	case SKILL_PLAY_PET_ATTACK2:
	case SKILL_PLAY_ELEMENTAL_SHOT: // ����� - ���� ���� ���� �� ��ų�� �� �Ӽ��� ���ܽ�Ų��.
	case SKILL_PLAY_SOUL_SUCKER: // ����� - ����Ŀ�� ���� ���� ����� ���ܽ�Ų��.
	case SKILL_PLAY_X_RAGE: // ����� - �ͽ�Ƽ�� �������� ���� ���� ����� ���ܽ�Ų��.
	case SKILL_PLAY_EXPANSION: // ����� - �ͽ��Ҽǵ� ���� ���� ����� ���ܽ�Ų��.

		return FALSE;
	}

	damage = lpPlayInfo->dwForceOrb_Damage;


	// ����� : ���� ���� �߰�
	if ( lpPlayInfo->dwForceOrb_Code==(sinFO1|sin27) || lpPlayInfo->dwForceOrb_Code==(sinFO1|sin28) || lpPlayInfo->dwForceOrb_Code==(sinFO1|sin29) || lpPlayInfo->dwForceOrb_Code==(sinFO1|sin30) ) 
	{
		//��Ű ��������, ����� ��������, �췹���� ��������, �̶��� �������� ���ݷ� 10%����
		damage2 = (MaxDamage+MinDamage)/2;
		if ( damage2>0 && damage2<1000 )
			damage += (damage2*10)/100;
	}
	else if( lpPlayInfo->dwForceOrb_Code==(sinFO1|sin31) || lpPlayInfo->dwForceOrb_Code==(sinFO1|sin35) || 
		lpPlayInfo->dwForceOrb_Code==(sinFO1|sin36) || lpPlayInfo->dwForceOrb_Code==(sinFO1|sin37) )
	{
		//���丣�� ��������, ���� ��������(3��)�� 15%����
		damage2 = (MaxDamage+MinDamage)/2;
		if ( damage2>0 && damage2<1000 )
			damage += (damage2*15)/100; // ���ݷ� 15%����
	}
	else if( lpPlayInfo->dwForceOrb_Code==(sinFO1|sin32) )
	{
		//�̴ϱ׸� �������� 20%����
		damage2 = (MaxDamage+MinDamage)/2;
		if ( damage2>0 && damage2<1000 )
			damage += (damage2*20)/100; //���ݷ� 20%����
	}

	if ( dwSkillCode ) 
	{
		// ����� - ����Ʈ SKILL_PLAY_SWORD_OF_JUSTICE / ������ SKILL_PLAY_METEO ���� 4�� ��ų �ν� ������ ������.
		if(dwSkillCode == 336 || dwSkillCode == 384)
		{
			dwSkillCode += 15;
		}
		sJobLevel = dwSkillCode&0xF;
		sJobLevel --;
		if ( sJobLevel>=0 && sJobLevel<0x10 ) {
			sJobLevel>>=2;
			switch( sJobLevel ) {
	case 0:		//1����ų
		if ( (dwSkillCode&0xFF)==SKILL_PLAY_MULTI_SPARK )
			damage = (damage*50)/100;
		else
			damage = (damage*80)/100;

		break;
	case 1:		//2����ų
		damage = (damage*90)/100;
		break;
	case 2:		//3����ų

		break;
	case 3:		//4����ų
		damage = (damage*150)/100;
		break;
			}

		}
	}

	lpTransSkillAttackData->Power += damage;

#ifdef DISP_DAMAGE
	///////////////////////// ����׿� ������ ���� ��� //////////////////////
	if ( lpPlayInfo->AdminMode>2 ) {
		if ( lpChar ) {
			wsprintf( szDispDamage , ">%s �������� ( ������������ + %d ) (%d)" , lpChar->smCharInfo.szName , damage , lpTransSkillAttackData->Power );
			rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
		}
		else {
			wsprintf( szDispDamage , "> �������� ( ������������ + %d ) (%d)" , damage , lpTransSkillAttackData->Power );
			rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
		}
	}
#endif
	return TRUE;
}

//������ �������� ���� ( PK-PVP )
int rsSendAttackDataToUser(  rsPLAYINFO *lpPlayInfo , rsPLAYINFO *lpPlayInfo2 , TRANS_ATTACKDATA *lpTransAttackData )
{
	int regs,cnt,len;
	int  Param1,Param2;

	if ( !lpPlayInfo->AdminMode ) {
		if( rsServerConfig.Enable_PKField_All )
		{
			// pluto PK��� �ʵ� ���� ����
			if ( !rsServerConfig.Enable_PK || lpPlayInfo->Position.Area < 11 && lpPlayInfo->Position.Area > 34 ) 
				return FALSE;			//���� �Ϲ������� PK����
		}
		else
		{
			if ( !rsServerConfig.Enable_PK && 
				sField[lpPlayInfo->Position.Area].State!=FIELD_STATE_CASTLE && 
				lpPlayInfo->Position.Area!=rsServerConfig.Enable_PKField ) 
				return FALSE;			//���� �Ϲ������� PK����
		}
	}

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 )
{
	//�ؿ�
	wsprintf( szDispDamage , ">%s Damaged ( %d -> %d )" , lpPlayInfo2->smCharInfo.szName, lpTransAttackData->Power , lpTransAttackData->Power/PK_POWER_DIVIDE );
	rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif

	Param1 = 0;
	Param2 = 0;

	//�Ӽ� ������ ���
	regs = (lpTransAttackData->AttackState>>16)&0xF;
	if ( regs && regs<sITEMINFO_NONE ) {
		cnt = lpTransAttackData->AttackState>>(16+4);		//�Ӽ� �� ( ���� ��Ʈ�� ��ü �������� �Ӽ� �������� ����� )
		if ( !cnt ) 
			cnt = lpTransAttackData->Power;

		//�Ӽ��� ���� ���׷� ����
		len = lpPlayInfo2->smCharInfo.Resistance[regs-1];
		if ( len ) {					//�Ӽ� ���׷� ���
			if ( len>=100 ) len = 100;
			if ( len<=-100 ) len = -100;
			lpTransAttackData->Power -= ((cnt*len)/100);

			if ( lpTransAttackData->Power<0 ) lpTransAttackData->Power=0;
		}

		switch(regs-1) { //(lpTransAttackData->AttackState>>16)-1) {
			case sITEMINFO_ICE:
				if ( lpPlayInfo->dwSkill_EnchantWeapon_Time>dwPlayServTime && (lpPlayInfo->dwSkill_EnchantWeapon_Param>>8)==0 ) {
					//���� ���� �ӵ� ���� ( ��üƮ �϶� ���� )
					Param1 = -SKILL_PLAY_ENCHANT_WEAPON;
					Param2 = 230-10*(lpPlayInfo->dwSkill_EnchantWeapon_Param&0xFF);	//������ ���� �ӵ�
					break;
				}
				if ( lpPlayInfo->dwSkill_DancingSword_Time>dwPlayServTime && (lpPlayInfo->dwSkill_DancingSword_Param&0xFF)==0 ) {
					//��̼ҵ� ���� ����
					Param1 = -SKILL_PLAY_DANCING_SWORD;
					Param2 = 230-10*(lpPlayInfo->dwSkill_DancingSword_Param>>16);
					break;
				}
				if ( lpPlayInfo->dwSkill_FrostJavelin_Time>dwPlayServTime ) {
					//���ν�Ʈ��� ��������
					//�ӵ� ������
					Param1 = -SKILL_PLAY_FROST_JAVELIN;
					Param2 = 240-(240*Frost_Javelin_SpeedSubPercent[lpPlayInfo->dwSkill_FrostJavelin_Param])/100;
					break;
				}
				break;

		}
	}


	return rsSendAttackUser( lpPlayInfo , lpPlayInfo2, lpTransAttackData->Power/PK_POWER_DIVIDE  , 0x80 , Param1, Param2 );
}

//���� ���� ����
int rsRecvAttackData( rsPLAYINFO *lpPlayInfo , TRANS_ATTACKDATA2 *lpTransAttackData )
{
	TRANS_ATTACKDATA	TransAttackData;
	smCHAR				*lpChar;
	smTRANS_COMMAND_EX	smTransCommand;
	rsPLAYINFO *lpPlayInfo2;
	char				szDispDamage[64];
	short				sDefAttack[2];
	int					pow;
	DWORD				dwCode;

/*
#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , "> ������Ŷ �Լ�" );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
*/
#ifdef _USE_DYNAMIC_ENCODE
	rsDecodeDamagePacket(lpPlayInfo, lpTransAttackData);						//Decryption
#else
	dm_DecodePacket(lpPlayInfo->dwObjectSerial, lpTransAttackData);			//Decryption
#endif

	if ( lpTransAttackData->dwChkSum!=dm_GetDamgeChkSum(lpTransAttackData) )	//��Ŷ �˻�
	{
		//������ ����Ÿ ������ ������ �α׿� ���
		smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
		smTransCommand.size = sizeof( smTRANS_COMMAND );
		smTransCommand.WParam = 8500;
		smTransCommand.LParam = 1;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		rsRecordDamageError( lpPlayInfo , (smTRANS_COMMAND *)&smTransCommand );
		return FALSE;
	}

	dwCode = lpTransAttackData->SkillCode&0xFF;
	if ( dwCode!=SKILL_PLAY_FALCON && dwCode!=SKILL_PLAY_MAGNETIC_SPHERE && dwCode!=SKILL_PLAY_CHARGING_STRIKE &&
		dwCode!=SKILL_PLAY_SHADOW_MASTER && dwCode!=SKILL_PLAY_DIVINE_PIERCING ) {
		//����, ������ �ٲ� ���ɼ� ���� ����
		if ( lpTransAttackData->AttackCount<=lpPlayInfo->DamagePacketCount || 
			lpTransAttackData->dwTime<=lpPlayInfo->dwDamagePacketTime ) {


#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , "> ������ Ŭ���̾�Ʈ ī���� �ð� ����" );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif

				//������ Ŭ���̾�Ʈ �� ī����,�ð� ����
				return FALSE;
		}

		lpPlayInfo->DamagePacketCount = lpTransAttackData->AttackCount;
		lpPlayInfo->dwDamagePacketTime = lpTransAttackData->dwTime;
	}

	if ( lpTransAttackData->Power[0]>lpPlayInfo->sLimitDamage[0] || 
		lpTransAttackData->Power[1]>lpPlayInfo->sLimitDamage[1] ||
		lpTransAttackData->Critical[0]>lpPlayInfo->sLimitCritical[0] ) {

			//������ ����Ÿ ������ ������ �α׿� ���
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			smTransCommand.size = sizeof( smTRANS_COMMAND_EX );
			smTransCommand.WParam = 8510;
			smTransCommand.WxParam = 1;

			smTransCommand.LParam = lpTransAttackData->Power[0];
			smTransCommand.SParam = lpTransAttackData->Power[1];
			smTransCommand.EParam = lpTransAttackData->Critical[0];

			smTransCommand.LxParam = lpPlayInfo->sLimitDamage[0];
			smTransCommand.SxParam = lpPlayInfo->sLimitDamage[1];
			smTransCommand.ExParam = lpPlayInfo->sLimitCritical[0];

			rsRecordDamageError( lpPlayInfo , (smTRANS_COMMAND *)&smTransCommand );

#ifdef _LANGUAGE_KOREAN
			if ( lpPlayInfo->AdminMode ) {
				TRANS_CHATMESSAGE	TransChatMessage;

				wsprintf( TransChatMessage.szMessage , "���� ��ġ���� ( %d %d %d ) ( %d %d %d )" ,
					lpTransAttackData->Power[0] , lpTransAttackData->Power[1] , lpTransAttackData->Critical[0],
					lpPlayInfo->sLimitDamage[0] , lpPlayInfo->sLimitDamage[1] , lpPlayInfo->sLimitCritical[0] );

				TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
				TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
				TransChatMessage.dwIP = 0;
				TransChatMessage.dwObjectSerial = 0;
				lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
			}
#endif

			//���� ���� ��ġ �ʰ� ����
			lpTransAttackData->Power[0] = lpPlayInfo->sLimitDamage[0];
			lpTransAttackData->Power[1] = lpPlayInfo->sLimitDamage[1];
			lpTransAttackData->Critical[0] = lpPlayInfo->sLimitCritical[0];

	}


	TransAttackData.x = lpTransAttackData->x;
	TransAttackData.y = lpTransAttackData->y;
	TransAttackData.z = lpTransAttackData->z;

	TransAttackData.AttackSize = lpTransAttackData->AttackSize;
	TransAttackData.dwTarObjectSerial = lpTransAttackData->dwTarObjectSerial;
	TransAttackData.dwDestObjectSerial = lpTransAttackData->dwDestObjectSerial;

	sDefAttack[0] = lpTransAttackData->Power[0];
	sDefAttack[1] = lpTransAttackData->Power[1];

	TransAttackData.Power = GetRandomPos( sDefAttack[0] , sDefAttack[1] );
	TransAttackData.AttackState = lpTransAttackData->AttackState;

	dm_SkillCode_Temp = 0;

	if ( lpTransAttackData->Area[0]>=0 )
		lpChar = srFindCharFromSerial( lpTransAttackData->dwTarObjectSerial , lpTransAttackData->Area[0] );
	else
		lpChar = srFindCharFromSerial( lpTransAttackData->dwTarObjectSerial );

	if ( lpTransAttackData->SkillCode ) {
		if ( rsSetSkillAttack( lpPlayInfo , lpChar , &TransAttackData , lpTransAttackData )==FALSE ) 
			return FALSE;
	}

	if ( rsSetSkillPlus( lpPlayInfo , lpChar , &TransAttackData , lpTransAttackData )==FALSE ) return FALSE;

	if ( !lpTransAttackData->SkillCode ) {
		//�Ϲ� ����
		if ( rsServerConfig.Event_Child ) {
			//��� �̺�Ʈ ( ��ū�� ĳ���϶� ���ݰ��� )
			if ( lpPlayInfo->smCharInfo.SizeLevel>0x1000 && lpPlayInfo->smCharInfo.SizeLevel<=0x1002 ) {
				TransAttackData.Power += (TransAttackData.Power*15)/100;	//���ݷ� 15% ����
			}
		}
	}

	//���ݿ� ���ְ� �ɷȴ�
	if ( lpPlayInfo->dwCurse_Attack_Time ) {
		if( lpPlayInfo->dwCurse_Attack_Time>dwPlayServTime)
			TransAttackData.Power -= (TransAttackData.Power*lpPlayInfo->dwCurse_Attack_Param)/100;
		else
			lpPlayInfo->dwCurse_Attack_Time = 0;
	}


	// �庰 - ���ο� �巡��
	if( lpPlayInfo->dwTime_PrimeItem_HelpPet > (DWORD)tServerTime && lpPlayInfo->smCharInfo.GravityScroolCheck[1] == 2)
	{
		TransAttackData.Power += (TransAttackData.Power * 10) / 100; 
	}





//	if ( !lpChar ) return FALSE;

	if ( lpChar ) {

		//�������� ���ݷ� �߰� 
		AddForceOrbPower(lpPlayInfo , lpChar ,  &TransAttackData , lpTransAttackData->SkillCode , sDefAttack[0] , sDefAttack[1]);

		if ( lpTransAttackData->Critical[0] ) {
			if ( (rand()%100)<GetCritical( lpPlayInfo , lpChar , lpTransAttackData->Critical[0] ) ) {
				TransAttackData.AttackState = (TransAttackData.AttackState&0xFFFF0000)+2;		//ũ��Ƽ�� ó��
				pow = TransAttackData.Power;
				TransAttackData.Power = (pow*170)/100;						//ũ��Ƽ�� ���ݰ���

				//��� ����
				if ( lpPlayInfo->dwSkill_AssassinEye_Time ) {
					if ( lpPlayInfo->dwSkill_AssassinEye_Time>dwPlayServTime ) {	
						TransAttackData.Power = (pow*(170+Assassin_Eye_AddCritical[ lpPlayInfo->dwSkill_AssassinEye_Param ]))/100;						//ũ��Ƽ�� ���ݰ���
					}
					else {
						lpPlayInfo->dwSkill_AssassinEye_Time = 0;
						lpPlayInfo->dwSkill_AssassinEye_Param = 0;
					}
				}

				smTransCommand.code = smTRANSCODE_ATTACK_RESULT;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 1;
				smTransCommand.LParam = 0;
				smTransCommand.SParam = 0;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s ���ݽõ� ( ũ��Ƽ�� )" , lpChar->smCharInfo.szName );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif

			}
		}

		//���������۽�ų����
		rsBillingItemSkill( lpPlayInfo , lpChar , 1 , TransAttackData.Power , lpTransAttackData->SkillCode&0xFF );


		rsRecvAttackData_Old( lpPlayInfo , &TransAttackData , lpChar , TRUE );

		if ( lpChar->dwTargetLockTime==0 && (
			lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_DEATHKNIGHT ||  
			lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_BABEL ||  
			lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_CASTLE_CRYSTAL_R ||  
			lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_CASTLE_CRYSTAL_G ||  
			lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_CASTLE_CRYSTAL_B ) )
		{
			//�ٺ� �Ǵ� ũ����Ż ���Ÿ� ������ ������ Ÿ�� ����
			int dist,x,y,z;

			x = (lpPlayInfo->Position.x - lpChar->pX)>>FLOATNS;
			y = (lpPlayInfo->Position.y - lpChar->pY)>>FLOATNS;
			z = (lpPlayInfo->Position.z - lpChar->pZ)>>FLOATNS;
			dist = x*x+y*y+z*z;

			if ( dist<=(lpChar->smCharInfo.Shooting_Range*lpChar->smCharInfo.Shooting_Range) ) {
				lpChar->lpTargetPlayInfo = lpPlayInfo;
				lpChar->dwTargetLockTime = dwPlayServTime+4000;
			}
		}
	}
	else {
		//PK - PVP ����

		lpPlayInfo2 = srFindUserFromSerial( lpTransAttackData->dwTarObjectSerial );
		if ( lpPlayInfo2 ) {
			//�������� ���ݷ� �߰� 
			AddForceOrbPower(lpPlayInfo , 0 ,  &TransAttackData , lpTransAttackData->SkillCode , sDefAttack[0] , sDefAttack[1]);

			if ( lpTransAttackData->Critical[0] ) {
				if ( (rand()%100)<GetCritical2( lpPlayInfo , lpPlayInfo2 , lpTransAttackData->Critical[0] ) ) {
					TransAttackData.AttackState = (TransAttackData.AttackState&0xFFFF0000)+2;		//ũ��Ƽ�� ó��
					pow = TransAttackData.Power;
					TransAttackData.Power = (pow*170)/100;						//ũ��Ƽ�� ���ݰ���

					//��� ����
					if ( lpPlayInfo->dwSkill_AssassinEye_Time ) {
						if ( lpPlayInfo->dwSkill_AssassinEye_Time>dwPlayServTime ) {	
							TransAttackData.Power = (pow*(170+Assassin_Eye_AddCritical[ lpPlayInfo->dwSkill_AssassinEye_Param ]))/100;						//ũ��Ƽ�� ���ݰ���
						}
						else {
							lpPlayInfo->dwSkill_AssassinEye_Time = 0;
							lpPlayInfo->dwSkill_AssassinEye_Param = 0;
						}
					}

					smTransCommand.code = smTRANSCODE_ATTACK_RESULT;
					smTransCommand.size = sizeof(smTRANS_COMMAND);
					smTransCommand.WParam = 1;
					smTransCommand.LParam = 0;
					smTransCommand.SParam = 0;
					smTransCommand.EParam = 0;
					lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s ���ݽõ� ( ũ��Ƽ�� )" , lpPlayInfo2->smCharInfo.szName );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif
				}
			}
			rsSendAttackDataToUser( lpPlayInfo , lpPlayInfo2 , &TransAttackData );
		}
	}


	if ( lpPlayInfo->AdminMode ) {
		smTransCommand.code = smTRANSCODE_DISP_DAMAGE;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = TransAttackData.Power;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;
		lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}

	return TRUE;
}


int rsSetSkillRangeAttack( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , TRANS_ATTACKDATA *lpTransAttackData , TRANS_SKIL_ATTACKDATA *lpTransSkillAttackData , TRANS_SKIL_ATTACKDATA2 *lpTransRangeAttack )
{
	DWORD	dwSkillCode;
	int		Point;
	int		Param;
	int		pow,pow1,pow2;
	int		cnt;
	smTRANS_COMMAND	smTransCommand;

	dwSkillCode = lpTransRangeAttack->SkillCode&0xFF;
	Point = ((lpTransRangeAttack->SkillCode>>8)&0xF)-1;
	Param = (lpTransRangeAttack->SkillCode>>12)&0xF;

	if ( Point<0 || Point>=10 ) return FALSE;

	dm_SkillCode_Temp = dwSkillCode;

	if ( lpPlayInfo->dwSkill_Vanish_Time ) {
		//��Ͻ�
		if ( lpPlayInfo->dwSkill_Vanish_Time<dwPlayServTime ) {
			lpPlayInfo->dwSkill_Vanish_Time = 0;
			lpPlayInfo->dwSkill_Vanish_Param = 0;
			return TRUE;
		}

		lpPlayInfo->dwSkill_Vanish_Time = 0;
		lpPlayInfo->dwSkill_Vanish_Param = 0;

		//��Ͻ� �����ϱ�
		smTransCommand.code = smTRANSCODE_PROCESS_SKILL;
		smTransCommand.size = sizeof( smTRANS_COMMAND );
		smTransCommand.WParam  = SKILL_PLAY_VANISH;
		smTransCommand.LParam  = 0;
		smTransCommand.SParam  = 0;
		smTransCommand.EParam  = 0;
		lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}

	//������ �������� ��ų �ڵ����� �˻�
	if ( rsCheckSkillLevel( lpPlayInfo , dwSkillCode )==FALSE ) {
		if ( lpPlayInfo->WarningCount<5 ) {
			//���� �α� �����
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			smTransCommand.WParam = 8850;
			smTransCommand.LParam = lpPlayInfo->smCharInfo.Level;
			smTransCommand.SParam = dwSkillCode;
			smTransCommand.EParam = Point;
			rsSendDataServer( lpPlayInfo->lpsmSock , &smTransCommand );
			lpPlayInfo->WarningCount++;
		}
		return FALSE;
	}

	switch( dwSkillCode ) {
		case SKILL_PLAY_PIKEWIND:			//����ũ ����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;	//����ũ�� ����

			lpTransSkillAttackData->AttackSize = Pike_Wind_Push_Lenght[Point];
			lpTransSkillAttackData->Power = GetRandomPos( Pike_Wind_Damage[Point][0] ,  Pike_Wind_Damage[Point][1] );
			lpTransSkillAttackData->AttackState = 1;
			return TRUE;

		case SKILL_PLAY_GROUND_PIKE:		//�׶��� ����ũ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;	//����ũ�� ����

			lpTransSkillAttackData->AttackSize = G_Pike_Range[Point];
			lpTransSkillAttackData->Power = GetRandomPos( G_Pike_Damage[Point][0] ,  G_Pike_Damage[Point][1] );
			lpTransSkillAttackData->AttackState = 3;
			return TRUE;

		case SKILL_PLAY_ROAR:				//�ξ�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;	//������ ����

			lpTransSkillAttackData->AttackSize = Roar_Range[Point];
			lpTransSkillAttackData->Power = Roar_Time[Point];
			lpTransSkillAttackData->AttackState = 2;
			return TRUE;

		case SKILL_PLAY_DIVINE_LIGHTNING:	//����� ����Ʈ��
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS ) return FALSE;	//������Ƽ�� ����
			if ( lpTransSkillAttackData->TargetCount>Divine_Lightning_Num[Point] ) return FALSE;	//���� Ÿ�� ������ ����

			/*
			pow1 = Divine_Lightning_Damage[Point][0]+lpTransRangeAttack->Critical[1]/8;
			pow2 = Divine_Lightning_Damage[Point][1]+lpTransRangeAttack->Critical[1]/4;

			pow1 += lpTransRangeAttack->Power2[0];
			pow2 += lpTransRangeAttack->Power2[1];
			*/

			// �庰 - ��ų�뷱�� ����(10.08.10)
			pow1 = Divine_Lightning_Damage[Point][0] + (lpTransRangeAttack->Critical[1]/6);
			pow2 = Divine_Lightning_Damage[Point][1] + (lpTransRangeAttack->Critical[1]/3);

			pow1 += (lpPlayInfo->smCharInfo.Level/2);
			pow2 += (lpPlayInfo->smCharInfo.Level/2);


			lpTransSkillAttackData->Power = GetRandomPos( pow1 , pow2 );
			lpTransSkillAttackData->AttackState = 103;

			return TRUE;

		case SKILL_PLAY_MECHANIC_BOMB:		//��ī�й�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MECHANICIAN ) return FALSE;	//��ī�ϼ� ����

			lpTransSkillAttackData->AttackSize = Mechanic_Bomb_Attack_Range[Point];
			lpTransSkillAttackData->Power = GetRandomPos( Mechanic_Bomb_Damage[Point][0] , Mechanic_Bomb_Damage[Point][1] );
			lpTransAttackData->Power = lpTransSkillAttackData->Power>>1;
			lpTransSkillAttackData->Power>>=1;
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;

		case SKILL_PLAY_GREAT_SMASH:		//�׷���Ʈ ���Ž�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MECHANICIAN ) return FALSE;	//��ī�ϼ� ����

			lpTransSkillAttackData->AttackSize = 70;
/*
			pow = lpPlayInfo->smCharInfo.Level/G_Smash_Attack_Rage[Point];
			pow += GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			pow >>=1;
*/
			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1]+(lpTransRangeAttack->Critical[1]/5) );
			pow += G_Smash_Attack_Rage[Point];
			pow >>=1;

			lpTransSkillAttackData->Power = pow;
			lpTransAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;

		case SKILL_PLAY_SPARK:				//����ũ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MECHANICIAN ) return FALSE;	//��ī�ϼ� ����

			lpTransSkillAttackData->AttackSize = Spark_Area[Point];
			lpTransSkillAttackData->Power = GetRandomPos( Spark_Damage[Point][0] , Spark_Damage[Point][1] );
			lpTransSkillAttackData->Power += lpPlayInfo->smCharInfo.Level/5;
			lpTransSkillAttackData->AttackState = 101;		//����

/*
			lpTransSkillAttackData->AttackSize = Spark_Area[Point];
			lpTransSkillAttackData->Power = GetRandomPos( Spark_Damage[Point][0] , Spark_Damage[Point][1] );
			lpTransAttackData->Power = lpTransSkillAttackData->Power>>1;
			lpTransSkillAttackData->Power>>=1;
			lpTransSkillAttackData->AttackState = 0;
*/
			return TRUE;

		case SKILL_PLAY_TORNADO:			//����̵�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;	//����ũ�� ����

			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			pow += ( pow*Tornado_Damage[Point] )/100;
			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;

		case SKILL_PLAY_ARROWOF_RAGE:		//���ο� ���� ������
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;	//���� ����

			lpTransSkillAttackData->AttackSize = A_Rage_Area[Point];
			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			pow += ( lpPlayInfo->smCharInfo.Level / A_Rage_Damage[Point] );
			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;

		case SKILL_PLAY_RAGEOF_ZECRAM:		//������ ���� ��ũ��
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;	//������ ����

			lpTransSkillAttackData->AttackSize = 70;
			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			pow += ( pow*R_Zecram_Damage[Point] )/100;
			pow >>=1;
			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 100;		//�ҼӼ�
			lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;		//�� �Ӽ�
			return TRUE;

		case SKILL_PLAY_EXPANSION:			//�ͽ��Ҽ�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;	//����ũ�� ����

			lpTransSkillAttackData->AttackSize = 180;
			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			pow += ( pow*Expansion_Damage[Point] )/100;

			//��ų ������ 5% ���ݷ� ���
			pow += (pow*(5*(Point+1)))/100;

			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;


			if ( lpChar ) {
				for( cnt=0;cnt<lpTransRangeAttack->TargetCount;cnt++) {
					if ( lpTransRangeAttack->dwTarObjectSerial[cnt]==lpChar->dwObjectSerial ) {
						lpTransAttackData->Power = 0;
						break;
					}
				}
			}
			return TRUE;

		case SKILL_PLAY_SWORD_BLAST:		//�ҵ� ����Ʈ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_KNIGHT ) return FALSE;	//����Ʈ ����

			pow = GetRandomPos( Sword_Blast_Damage[Point][0] , Sword_Blast_Damage[Point][1]+(lpPlayInfo->smCharInfo.Level/2) );
			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;

		case SKILL_PLAY_BRANDISH:			//�귣��
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_KNIGHT ) return FALSE;	//����Ʈ ����

			lpTransSkillAttackData->AttackSize = Brandish_Range[Point];
			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			pow += ( pow*Brandish_Damage[Point] ) /100;
			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;

		case SKILL_PLAY_PIERCING:			//�Ǿ��
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_KNIGHT ) return FALSE;	//����Ʈ ����

			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			pow += ( pow*Piercing_Damage[Point] ) /100;
			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;

		case SKILL_PLAY_SHIELD_STRIKE:		//���� ��Ʈ����ũ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;	//��Ż��Ÿ ����

			lpTransSkillAttackData->Power = GetRandomPos( S_Strike_Damage[Point][0] , S_Strike_Damage[Point][1]+(lpPlayInfo->smCharInfo.Level/5) );
			lpTransSkillAttackData->AttackState = 5;
			return TRUE;

		case SKILL_PLAY_SOUL_SUCKER:		//�ҿ� ��Ŀ
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;	//��Ż��Ÿ ����

			lpTransSkillAttackData->Power = Point;
			lpTransSkillAttackData->AttackState = 4;
			return TRUE;

		case SKILL_PLAY_FIRE_BALL:			//���̾
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MAGICIAN ) return FALSE;	//������ ����

			lpTransSkillAttackData->AttackSize = FireBall_Area[Point];

			pow1 = FireBall_Damage[Point][0];
			pow2 = FireBall_Damage[Point][1];

			if ( lpPlayInfo->dwSkill_SpiritElemental_Time ) {		//���Ǹ�������Ż ������ ����
				if ( lpPlayInfo->dwSkill_SpiritElemental_Time>dwPlayServTime ) {
					pow1 += ( pow1 * lpPlayInfo->dwSkill_SpiritElemental_Param )/100;
					pow2 += ( pow2 * lpPlayInfo->dwSkill_SpiritElemental_Param )/100;
				}
				else {
					lpPlayInfo->dwSkill_SpiritElemental_Time = 0;
					lpPlayInfo->dwSkill_SpiritElemental_Param = 0;
				}
			}

			/*
			pow1 +=lpTransRangeAttack->Critical[1]/8;
			pow2 +=lpTransRangeAttack->Critical[1]/4;

			pow1 += lpTransRangeAttack->Power2[0];
			pow2 += lpTransRangeAttack->Power2[1];
			*/

			// �庰 - ��ų�뷱�� ����(10.08.10)
			pow1 += lpPlayInfo->smCharInfo.Level/2;
			pow2 += lpPlayInfo->smCharInfo.Level/2;

			pow1 +=lpTransRangeAttack->Critical[1]/6;
			pow2 +=lpTransRangeAttack->Critical[1]/4;

			pow = GetRandomPos( pow1 , pow2 );
	//		lpTransSkillAttackData->Power = (pow*70)/100;
	//		lpTransAttackData->Power = (pow*30)/100;

			lpTransSkillAttackData->Power = (pow*70)/100;
			lpTransAttackData->Power = pow;

			lpTransSkillAttackData->AttackState = 100;
			lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;		//�� �Ӽ�
			return TRUE;

		case SKILL_PLAY_COLUMN_OF_WATER:	//������̵�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MAGICIAN ) return FALSE;	//������ ����

			lpTransSkillAttackData->AttackSize = Watornado_Area[Point];

			pow1 = Watornado_Damage[Point][0];
			pow2 = Watornado_Damage[Point][1];

			if ( lpPlayInfo->dwSkill_SpiritElemental_Time ) {		//���Ǹ�������Ż ������ ����
				if ( lpPlayInfo->dwSkill_SpiritElemental_Time>dwPlayServTime ) {
					pow1 += (pow1*lpPlayInfo->dwSkill_SpiritElemental_Param)/100;
					pow2 += (pow2*lpPlayInfo->dwSkill_SpiritElemental_Param)/100;
				}
				else {
					lpPlayInfo->dwSkill_SpiritElemental_Time = 0;
					lpPlayInfo->dwSkill_SpiritElemental_Param = 0;
				}
			}

			/*
			pow1 += lpTransRangeAttack->Critical[1]/8;
			pow2 += lpTransRangeAttack->Critical[1]/4;

			pow1 += lpTransRangeAttack->Power2[0];
			pow2 += lpTransRangeAttack->Power2[1];
			*/

			// �庰 - ��ų�뷱�� ����(10.08.10)
			pow1 += lpPlayInfo->smCharInfo.Level/2;
			pow2 += lpPlayInfo->smCharInfo.Level/2;

			pow1 +=lpTransRangeAttack->Critical[1]/6;
			pow2 +=lpTransRangeAttack->Critical[1]/3;

			pow = GetRandomPos( pow1 , pow2 );
		//	lpTransSkillAttackData->Power = (pow*70)/100;
		//	lpTransAttackData->Power = (pow*30)/100;

			lpTransSkillAttackData->Power = (pow*70)/100;
			lpTransAttackData->Power = pow;


			lpTransSkillAttackData->AttackState = 104;
			lpTransAttackData->AttackState |= (sITEMINFO_NONE)<<16;		//�� �Ӽ�
			return TRUE;

		case SKILL_PLAY_ENCHANT_WEAPON:		//��þƮ ���� ���⽺�÷�����
			if ( !lpPlayInfo->dwSkill_EnchantWeapon_Time ) return FALSE;			//��þƮ ���� Ȯ��

			lpTransSkillAttackData->AttackSize = 50;
			lpTransSkillAttackData->Power = GetRandomPos( Enchant_Weapon_Damage_Lightning[Point][0] , Enchant_Weapon_Damage_Lightning[Point][1] );
			lpTransSkillAttackData->AttackState = 105;
			return TRUE;

		case SKILL_PLAY_VENOM_SPEAR:		//�����Ǿ�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;	//����ũ�� ����
			if ( lpTransSkillAttackData->TargetCount>VenomSpear_Num[Point] ) return FALSE;	//��â Ÿ�� ������ ����

			//������ �ִ��� �ʰ�
			if ( lpTransRangeAttack->Power2[0]>lpPlayInfo->sLimitDamage[0] )
				lpTransRangeAttack->Power2[0] = lpPlayInfo->sLimitDamage[0];
			if ( lpTransRangeAttack->Power2[1]>lpPlayInfo->sLimitDamage[1] )
				lpTransRangeAttack->Power2[1] = lpPlayInfo->sLimitDamage[1];

			pow1 = lpTransRangeAttack->Power2[0]>>1;
			pow2 = lpTransRangeAttack->Power2[1]>>1;

			lpTransSkillAttackData->Power = GetRandomPos( pow1 , pow2 );
			lpTransSkillAttackData->AttackState = 106;
			lpTransAttackData->AttackState |= (sITEMINFO_POISON+1)<<16;		//�� �Ӽ�

			return TRUE;
/*
		case SKILL_PLAY_BLADE_OF_BLAZE:		//�ҷ��̵� ���� ������
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PIKEMAN ) return FALSE;	//����ũ�� ����

			if ( lpTransRangeAttack->Power[0]<lpTransRangeAttack->Power2[0] || lpTransRangeAttack->Power[1]<lpTransRangeAttack->Power2[1] ) {
				//���ⵥ���� ����
				return FALSE;
			}
			pow1 = B_Of_Blaze_Damage[Point][0] + lpTransRangeAttack->Power2[0];
			pow2 = B_Of_Blaze_Damage[Point][1] + lpTransRangeAttack->Power2[1];

			lpTransSkillAttackData->Power = GetRandomPos( pow1 , pow2 );
			lpTransSkillAttackData->AttackState = 100;
			return TRUE;
*/

		case SKILL_PLAY_ELEMENTAL_SHOT:		//������Ż�� (����)
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;	//���� ����
			if ( Param!=1 ) return FALSE;
			lpTransSkillAttackData->AttackSize = 75;
			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			lpTransAttackData->Power = pow;
			lpTransSkillAttackData->Power = GetRandomPos( Elemental_Shot_Lightning[Point][0] , Elemental_Shot_Lightning[Point][1] );
			lpTransSkillAttackData->AttackState = 105;
			lpTransAttackData->AttackState |= (sITEMINFO_LIGHTING+1)<<16;		//���� �Ӽ�
			return TRUE;

		case SKILL_PLAY_BOMB_SHOT:			//����
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;	//���� ����

			if ( lpChar ) {
				pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
				pow += (pow*BombShot_Damage[Point])/100;

				if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_DEMON ) 
					pow += (pow*BombShot_DemonDamage[Point])/100;

				lpTransAttackData->Power = pow;
			}
			lpTransSkillAttackData->Power = GetRandomPos( BombShot_AddDamage[Point][0],BombShot_AddDamage[Point][1] );
			lpTransSkillAttackData->AttackState = 100;
			lpTransAttackData->AttackState = 0;
			//lpTransAttackData->AttackState |= (sITEMINFO_FIRE+1)<<16;		//�� �Ӽ�
			return TRUE;

		case SKILL_PLAY_PERFORATION:			//�������̼�
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;	//���� ����

			lpTransSkillAttackData->AttackSize = 340+Perforation_Attack_Range[Point];

			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			pow += (pow*Perforation_Damage[Point])/100;
			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;
			dm_Critical_Temp = lpTransRangeAttack->Critical[0];
			return TRUE;
/*
		case SKILL_PLAY_FLAME_BRANDISH:			//������ �귣��
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_KNIGHT ) return FALSE;	//����Ʈ ����

			lpTransSkillAttackData->AttackSize = 80;

			if ( lpChar && lpTransRangeAttack->Power2[0]>0 && lpTransRangeAttack->Power2[0]<=10 ) {

				pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
				pow += pow*(Brandish_Damage[lpTransRangeAttack->Power2[0]-1])/100;//*130)/100;
				pow += (pow*30)/100;

				if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_MUTANT )			//����Ʈ ���ݷ� 30%�߰�
					pow += (pow*30)/100;

				lpTransAttackData->Power = pow;
				lpTransSkillAttackData->AttackState = 0;

				pow = GetRandomPos( Flame_Brandish_Damage[Point][0] , Flame_Brandish_Damage[Point][1] );
				lpTransSkillAttackData->Power = pow;
				lpTransSkillAttackData->AttackState = 100;
				return TRUE;
			}
			break;
*/
		case SKILL_PLAY_LIGHTNING_JAVELIN:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;	//��Ż ����

			lpTransSkillAttackData->AttackSize = 100;
			pow = GetRandomPos( Lightning_Javelin_Damage[Point][0] , Lightning_Javelin_Damage[Point][1]+(lpPlayInfo->smCharInfo.Level/3) );
			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 101;		//����

			lpTransAttackData->Power = GetRandomPos(lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1]+(lpPlayInfo->smCharInfo.Level/3) );
			lpTransAttackData->AttackState = 0;

			if ( lpChar ) {
				if ( lpChar->smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
					lpTransAttackData->Power += lpTransAttackData->Power/2;		//�𵥵� 50%
				}

				//ũ��Ƽ�� ó��
				if ( lpTransRangeAttack->Critical[0] ) {
					if ( (rand()%100)<GetCritical( lpPlayInfo , lpChar , lpTransRangeAttack->Critical[0] ) ) {
						lpTransAttackData->Power = (lpTransAttackData->Power*170)/100;

#ifdef DISP_DAMAGE
///////////////////////// ����׿� ������ ���� ��� //////////////////////
if ( lpPlayInfo->AdminMode>2 ) {
wsprintf( szDispDamage , ">%s ���ݽõ� ( ũ��Ƽ�� )" , lpChar->smCharInfo.szName );
rsDisplayDamgeToClient( lpPlayInfo , szDispDamage );
}
#endif

					}
				}
			}
			return TRUE;

		case SKILL_PLAY_STORM_JAVELIN:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;	//��Ż ����
			lpTransSkillAttackData->AttackSize = 340; // �庰 - ��ų�뷱�� ����(10.08.10) 300 -> 340
			pow = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			pow += (pow*Storm_Javelin_Damage[Point])/100;
			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;


		case SKILL_PLAY_EXTINCTION:		//�ͽ�ƾ��
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS ) return FALSE;	//��Ƽ ����
			lpTransSkillAttackData->Power = Point+1;
			lpTransSkillAttackData->AttackState = 6;
			return TRUE;

		case SKILL_PLAY_DIASTROPHISM:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MAGICIAN ) return FALSE;	//������ ����
			if ( lpTransRangeAttack->Power[0]<lpTransRangeAttack->Power2[0] || lpTransRangeAttack->Power[1]<lpTransRangeAttack->Power2[1] ) {
				//���ⵥ���� ����
				return FALSE;
			}

			lpTransSkillAttackData->AttackSize = Diastrophism_Area[Point];

			pow1 = Diastrophism_Damage[Point][0];
			pow2 = Diastrophism_Damage[Point][1];

			if ( lpPlayInfo->dwSkill_SpiritElemental_Time ) {		//���Ǹ�������Ż ������ ����
				if ( lpPlayInfo->dwSkill_SpiritElemental_Time>dwPlayServTime ) {
					pow1 += (pow1*lpPlayInfo->dwSkill_SpiritElemental_Param)/100;
					pow2 += (pow2*lpPlayInfo->dwSkill_SpiritElemental_Param)/100;
				}
				else {
					lpPlayInfo->dwSkill_SpiritElemental_Time = 0;
					lpPlayInfo->dwSkill_SpiritElemental_Param = 0;
				}
			}

			/*
			pow = GetRandomPos( 
				pow1+lpTransRangeAttack->Power2[0]+(lpTransRangeAttack->Critical[1]/8),
				pow2+lpTransRangeAttack->Power2[1]+(lpTransRangeAttack->Critical[1]/4) );
				*/
			

			// �庰 - ��ų�뷱�� ����(10.08.10)
			pow = GetRandomPos( 
				pow1 + (lpPlayInfo->smCharInfo.Level/2) + (lpTransRangeAttack->Critical[1]/6),
				pow2 + (lpPlayInfo->smCharInfo.Level/2) + (lpTransRangeAttack->Critical[1]/3) );



			lpTransSkillAttackData->Power = pow;
			lpTransSkillAttackData->AttackState = 0;

			return TRUE;


		///////////////////// 4����ų /////////////////////

		case SKILL_PLAY_IMPULSION:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MECHANICIAN ) return FALSE;	//��ī�ϼ� ����

			//lpTransAttackData->Power = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			lpTransSkillAttackData->AttackSize = Impulsion_Range[Point];
			lpTransSkillAttackData->Power = GetRandomPos( Impulsion_LightingDamage[Point][0] , Impulsion_LightingDamage[Point][1] );
			lpTransSkillAttackData->AttackState = 101;
			return TRUE;

		case SKILL_PLAY_COMPULSION:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MECHANICIAN ) return FALSE;	//��ī�ϼ� ����
			lpTransSkillAttackData->AttackState = 201;
			lpTransSkillAttackData->AttackSize = Compulsion_Area[Point];

			lpPlayInfo->dwSkill_Compulsion_Time = dwPlayServTime+Compulsion_Time[Point]*1000;
			lpPlayInfo->dwSkill_Compulsion_Param = Compulsion_AddAbsorb[Point];

			return TRUE;

		case SKILL_PLAY_CYCLONE_STRIKE:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_FIGHTER ) return FALSE;
			if ( lpTransSkillAttackData->TargetCount>Cyclone_Strike_AttackNum[Point]+1 ) return FALSE;	//Ÿ�� ������ ����

			lpTransAttackData->Power = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			lpTransAttackData->Power+= (lpTransAttackData->Power*Cyclone_Strike_DamagePercent[Point])/100;
			lpTransSkillAttackData->AttackSize = Cyclone_Strike_Area[Point];
			lpTransSkillAttackData->Power = GetRandomPos( Cyclone_Strike_AreaDamage[Point][0] , Cyclone_Strike_AreaDamage[Point][1] );
			lpTransSkillAttackData->AttackState = 0;
			dm_Critical_Temp = lpTransRangeAttack->Critical[0];
			return TRUE;

		case SKILL_PLAY_PHOENIX_SHOT:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ARCHER ) return FALSE;

			lpTransSkillAttackData->Power = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			lpTransSkillAttackData->Power+= (lpTransSkillAttackData->Power*Phoenix_Shot_DamagePercent[Point])/100;
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;


		case SKILL_PLAY_SWORD_OF_JUSTICE:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_KNIGHT ) return FALSE;

			lpTransSkillAttackData->Power = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			lpTransSkillAttackData->Power+= (lpTransSkillAttackData->Power*Sword_Of_Justice_DamagePercent[Point])/100;
			lpTransSkillAttackData->AttackSize = Sword_Of_Justice_Area[Point];
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;

		case SKILL_PLAY_X_RAGE:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA ) return FALSE;

			lpTransSkillAttackData->Power = GetRandomPos( lpTransRangeAttack->Power[0] , lpTransRangeAttack->Power[1] );
			lpTransSkillAttackData->Power+= (lpTransSkillAttackData->Power*X_Rage_DamagePercent[Point])/100;
			lpTransSkillAttackData->AttackSize = X_Rage_Area[Point];
			lpTransSkillAttackData->AttackState = 0;
			return TRUE;

		case SKILL_PLAY_GLACIAL_SPIKE:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS ) return FALSE;

			/*
			pow1 = Glacial_Spike_Damage[Point][0]+lpTransRangeAttack->Critical[1]/8;
			pow2 = Glacial_Spike_Damage[Point][1]+lpTransRangeAttack->Critical[1]/4;

			pow1 += lpTransRangeAttack->Power2[0];
			pow2 += lpTransRangeAttack->Power2[1];
			*/

			// �庰 - ��ų�뷱�� ����(10.08.10)
			pow1 = Glacial_Spike_Damage[Point][0] + (lpTransRangeAttack->Critical[1]/6);
			pow2 = Glacial_Spike_Damage[Point][1] + (lpTransRangeAttack->Critical[1]/3);

			pow1 += (lpPlayInfo->smCharInfo.Level/2);
			pow2 += (lpPlayInfo->smCharInfo.Level/2);

			lpTransSkillAttackData->Power = GetRandomPos( pow1 , pow2 );
			lpTransSkillAttackData->AttackState = 3;
			return TRUE;;

		case SKILL_PLAY_CHAIN_LIGHTNING:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS ) return FALSE;
			if ( lpTransSkillAttackData->TargetCount>Chain_Lightning_Num[Point] ) return FALSE;	//Ÿ�� ������ ����
			lpTransSkillAttackData->AttackSize = Chain_Lightning_Range[Point]*Chain_Lightning_Num[Point];

			/*
			pow1 = Chain_Lightning_Damage[Point][0]+lpTransRangeAttack->Critical[1]/8;
			pow2 = Chain_Lightning_Damage[Point][1]+lpTransRangeAttack->Critical[1]/4;

			pow1 += lpTransRangeAttack->Power2[0];
			pow2 += lpTransRangeAttack->Power2[1];
			*/

			// �庰 - ��ų�뷱�� ����(10.08.10)
			pow1 = Chain_Lightning_Damage[Point][0] + (lpTransRangeAttack->Critical[1]/6);
			pow2 = Chain_Lightning_Damage[Point][1] + (lpTransRangeAttack->Critical[1]/3);

			pow1 += (lpPlayInfo->smCharInfo.Level/3);
			pow2 += (lpPlayInfo->smCharInfo.Level/3);

			pow1 += (lpTransRangeAttack->Power2[0]/3);
			pow2 += (lpTransRangeAttack->Power2[1]/3);



			lpTransSkillAttackData->Power = GetRandomPos( pow1 , pow2 );

			lpTransSkillAttackData->AttackState = 101;		//����
			return TRUE;;

		case SKILL_PLAY_FLAME_WAVE:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MAGICIAN ) return FALSE;	//������ ����

			//lpTransSkillAttackData->AttackSize = FireBall_Area[Point];

			pow1 = Flame_Wave_FireDamage[Point][0];
			pow2 = Flame_Wave_FireDamage[Point][1];

			if ( lpPlayInfo->dwSkill_SpiritElemental_Time ) {		//���Ǹ�������Ż ������ ����
				if ( lpPlayInfo->dwSkill_SpiritElemental_Time>dwPlayServTime ) {
					pow1 += ( pow1 * lpPlayInfo->dwSkill_SpiritElemental_Param )/100;
					pow2 += ( pow2 * lpPlayInfo->dwSkill_SpiritElemental_Param )/100;
				}
				else {
					lpPlayInfo->dwSkill_SpiritElemental_Time = 0;
					lpPlayInfo->dwSkill_SpiritElemental_Param = 0;
				}
			}

			/*
			pow1 +=lpTransRangeAttack->Critical[1]/8;
			pow2 +=lpTransRangeAttack->Critical[1]/4;

			pow1 += lpTransRangeAttack->Power2[0];
			pow2 += lpTransRangeAttack->Power2[1];
			*/

			// �庰 - ��ų�뷱�� ����(10.08.10)
			pow1 += lpPlayInfo->smCharInfo.Level/2;
			pow2 += lpPlayInfo->smCharInfo.Level/2;

			pow1 +=lpTransRangeAttack->Critical[1]/6;
			pow2 +=lpTransRangeAttack->Critical[1]/3;

			pow = GetRandomPos( pow1 , pow2 );
			lpTransSkillAttackData->Power = pow;

			lpTransSkillAttackData->AttackState = 100;
			return TRUE;

		case SKILL_PLAY_DISTORTION:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MAGICIAN ) return FALSE;	//������ ����
			//�ְ� ���

			lpTransSkillAttackData->AttackState = 7;		//�ְ�
			return TRUE;

		case SKILL_PLAY_METEO:
			if ( lpPlayInfo->smCharInfo.JOB_CODE!=JOBCODE_MAGICIAN ) return FALSE;	//������ ����

			lpTransSkillAttackData->AttackSize = Meteo_Area[Point];

			pow1 = Meteo_Damage[Point][0];
			pow2 = Meteo_Damage[Point][1];

			if ( lpPlayInfo->dwSkill_SpiritElemental_Time ) {		//���Ǹ�������Ż ������ ����
				if ( lpPlayInfo->dwSkill_SpiritElemental_Time>dwPlayServTime ) {
					pow1 += ( pow1 * lpPlayInfo->dwSkill_SpiritElemental_Param )/100;
					pow2 += ( pow2 * lpPlayInfo->dwSkill_SpiritElemental_Param )/100;
				}
				else {
					lpPlayInfo->dwSkill_SpiritElemental_Time = 0;
					lpPlayInfo->dwSkill_SpiritElemental_Param = 0;
				}
			}

			/*
			pow1 +=lpTransRangeAttack->Critical[1]/8;
			pow2 +=lpTransRangeAttack->Critical[1]/4;

			pow1 += lpTransRangeAttack->Power2[0];
			pow2 += lpTransRangeAttack->Power2[1];
			*/

			// �庰 - ��ų�뷱�� ����(10.08.10)
			pow1 += lpPlayInfo->smCharInfo.Level/3; // Lv/3
			pow2 += lpPlayInfo->smCharInfo.Level/3; // Lv/3

			pow1 += lpTransRangeAttack->Power2[0]/3; // ����/3
			pow2 += lpTransRangeAttack->Power2[1]/3; // ����/3

			pow1 +=lpTransRangeAttack->Critical[1]/6; //����/6
			pow2 +=lpTransRangeAttack->Critical[1]/3; //����/3

			pow = GetRandomPos( pow1 , pow2 );
			lpTransSkillAttackData->Power = pow;

			lpTransSkillAttackData->AttackState = 100;
			return TRUE;

	}

	return FALSE;
}

//�������� ���� ����
int rsRecvRangeAttackData( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA2 *lpTransRangeAttackData )
{

	TRANS_ATTACKDATA		TransAttackData;
	TRANS_SKIL_ATTACKDATA	TransSkillAttackData;
	smCHAR					*lpChar;
	smTRANS_COMMAND_EX	smTransCommand;

#ifdef _USE_DYNAMIC_ENCODE
	rsDecodeDamagePacket(lpPlayInfo, lpTransAttackData);						//Decryption
#else
	dm_DecodePacket(lpPlayInfo->dwObjectSerial, lpTransRangeAttackData);			//Decryption
#endif

	if ( lpTransRangeAttackData->dwChkSum!=dm_GetRangeDamgeChkSum(lpTransRangeAttackData) )	{	//��Ŷ�˻�

		//������ ����Ÿ ������ ������ �α׿� ���
		smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
		smTransCommand.size = sizeof( smTRANS_COMMAND );
		smTransCommand.WParam = 8500;
		smTransCommand.LParam = 2;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		rsRecordDamageError( lpPlayInfo , (smTRANS_COMMAND *)&smTransCommand );
		return FALSE;
	}


	if ( (lpTransRangeAttackData->SkillCode&0xFF)!=SKILL_PLAY_ENCHANT_WEAPON ) {		//��þƮ ����, ������ �ٲ� ���ɼ� ���� ����
		if ( lpTransRangeAttackData->AttackCount<=lpPlayInfo->DamagePacketCount || 
			lpTransRangeAttackData->dwTime<=lpPlayInfo->dwDamagePacketTime ) {
				//������ Ŭ���̾�Ʈ �� ī����,�ð� ����
				return FALSE;
		}

		lpPlayInfo->DamagePacketCount = lpTransRangeAttackData->AttackCount;
		lpPlayInfo->dwDamagePacketTime = lpTransRangeAttackData->dwTime;
	}

	if ( lpTransRangeAttackData->Power[0]>lpPlayInfo->sLimitDamage[0] || 
		lpTransRangeAttackData->Power[1]>lpPlayInfo->sLimitDamage[1] ||
		lpTransRangeAttackData->Critical[0]>lpPlayInfo->sLimitCritical[0] ) {

			//������ ����Ÿ ������ ������ �α׿� ���
			smTransCommand.code = smTRANSCODE_SET_BLACKLIST;
			smTransCommand.size = sizeof( smTRANS_COMMAND_EX );
			smTransCommand.WParam = 8510;
			smTransCommand.WxParam = 1;

			smTransCommand.LParam = lpTransRangeAttackData->Power[0];
			smTransCommand.SParam = lpTransRangeAttackData->Power[1];
			smTransCommand.EParam = lpTransRangeAttackData->Critical[0];

			smTransCommand.LxParam = lpPlayInfo->sLimitDamage[0];
			smTransCommand.SxParam = lpPlayInfo->sLimitDamage[1];
			smTransCommand.ExParam = lpPlayInfo->sLimitCritical[0];

			rsRecordDamageError( lpPlayInfo , (smTRANS_COMMAND *)&smTransCommand );

#ifdef _LANGUAGE_KOREAN
			if ( lpPlayInfo->AdminMode ) {
				TRANS_CHATMESSAGE	TransChatMessage;

				wsprintf( TransChatMessage.szMessage , "���� ��ġ���� ( %d %d %d ) ( %d %d %d )" ,
					lpTransRangeAttackData->Power[0] , lpTransRangeAttackData->Power[1] , lpTransRangeAttackData->Critical[0],
					lpPlayInfo->sLimitDamage[0] , lpPlayInfo->sLimitDamage[1] , lpPlayInfo->sLimitCritical[0] );

				TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
				TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
				TransChatMessage.dwIP = 0;
				TransChatMessage.dwObjectSerial = 0;
				lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
			}
#endif
			//���� ���� ��ġ �ʰ� ����
			lpTransRangeAttackData->Power[0] = lpPlayInfo->sLimitDamage[0];
			lpTransRangeAttackData->Power[1] = lpPlayInfo->sLimitDamage[1];
			lpTransRangeAttackData->Critical[0] = lpPlayInfo->sLimitCritical[0];
	}


	TransAttackData.Power = 0;
	TransSkillAttackData.Power = 0;
	dm_SkillCode_Temp = 0;

	if ( lpTransRangeAttackData->dwMainTargetObject )
		lpChar = srFindCharFromSerial( lpTransRangeAttackData->dwMainTargetObject , lpTransRangeAttackData->Area[0] );
	else 
		lpChar = 0;


	if ( lpTransRangeAttackData->dwMainTargetObject ) {
		TransAttackData.AttackState = 0;
		TransAttackData.AttackSize = 64*64;
		TransAttackData.dwDestObjectSerial = lpTransRangeAttackData->dwDestObjectSerial;
		TransAttackData.x = lpTransRangeAttackData->x;
		TransAttackData.y = lpTransRangeAttackData->y;
		TransAttackData.z = lpTransRangeAttackData->z;
	}

	TransSkillAttackData.AttackSize = lpTransRangeAttackData->AttackSize;
	TransSkillAttackData.dwDestObjectSerial = lpTransRangeAttackData->dwDestObjectSerial;
	TransSkillAttackData.x = lpTransRangeAttackData->x;
	TransSkillAttackData.y = lpTransRangeAttackData->y;
	TransSkillAttackData.z = lpTransRangeAttackData->z;
	TransSkillAttackData.AttackSize = 200;

	if ( lpTransRangeAttackData->TargetCount>0 && lpTransRangeAttackData->TargetCount<SKIL_ATTACK_CHAR_MAX ) {
		memcpy( TransSkillAttackData.dwTarObjectSerial , lpTransRangeAttackData->dwTarObjectSerial , sizeof(DWORD)*lpTransRangeAttackData->TargetCount );
		TransSkillAttackData.TargetCount = lpTransRangeAttackData->TargetCount;
	}
	else
		TransSkillAttackData.TargetCount = 0;


	if ( rsSetSkillRangeAttack( lpPlayInfo , lpChar , &TransAttackData , &TransSkillAttackData , lpTransRangeAttackData )==FALSE ) 
		return FALSE;
	
	// ����� - ���� ���� �߰�, ���� ���� ���� �߰�(�������ݿ� �������� �Ŀ���)
	AddRangeForceOrbPower(lpPlayInfo, lpChar, &TransSkillAttackData, lpTransRangeAttackData->SkillCode, lpTransRangeAttackData->Power[0], lpTransRangeAttackData->Power[1]);

	//���������۽�ų����
	int Total = TransSkillAttackData.TargetCount;
	int AvgPow;

	if ( lpChar ) Total+=1;
	if ( Total ) AvgPow = (TransAttackData.Power+TransSkillAttackData.Power*TransSkillAttackData.TargetCount)/Total;
	else AvgPow = 0;

	rsBillingItemSkill( lpPlayInfo , lpChar , Total , AvgPow , lpTransRangeAttackData->SkillCode&0xFF );

	if ( lpChar && TransAttackData.Power ) {
		//AddForceOrbPower(lpPlayInfo , lpChar ,  &TransAttackData , lpTransRangeAttackData->SkillCode );	//�������� ���ݷ� �߰� 
		rsRecvAttackData_Old( lpPlayInfo , &TransAttackData , lpChar , TRUE );
	}

	if ( TransSkillAttackData.TargetCount>0 ) {
		RecvRangeAttack( lpPlayInfo , &TransSkillAttackData , lpTransRangeAttackData->Area[0] , lpTransRangeAttackData->SkillCode );


		// pluto PK��� �ʵ� ����
		if( rsServerConfig.Enable_PKField_All )
		{
			// pluto PK��� �ʵ� ���� ����
			if ( rsServerConfig.Enable_PK || lpPlayInfo->Position.Area < 11 && lpPlayInfo->Position.Area > 34 )
			{
				//����  PK �׽�Ʈ��
				RecvRangeAttack_User( lpPlayInfo , &TransSkillAttackData , lpTransRangeAttackData->Area[0] , lpTransRangeAttackData->SkillCode );
			}
		}
		else
		{
			if ( rsServerConfig.Enable_PK || lpPlayInfo->Position.Area==rsCASTLE_FIELD || lpPlayInfo->Position.Area==rsServerConfig.Enable_PKField ) {
				//����  PK �׽�Ʈ��
				RecvRangeAttack_User( lpPlayInfo , &TransSkillAttackData , lpTransRangeAttackData->Area[0] , lpTransRangeAttackData->SkillCode );
			}
		}
	}

	if ( lpPlayInfo->AdminMode ) {
		smTRANS_COMMAND	smTransCommand;

		smTransCommand.code = smTRANSCODE_DISP_DAMAGE;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = TransSkillAttackData.Power+TransAttackData.Power;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;
		lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}


	return TRUE;
}


/////////////////////////////////// ��ȣȭ ��� ///////////////////////////////////////////
//////////// ���� ö�� �䱸 /////////////

/*
//��� ���� - ���� �ҽ�
#define	DAMAGE_PACKET_KEY	0xBBAA3840
#define	DAMAGE_PACKET_MUL	0x11223344

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

//������ ��ȣ Ǯ�� �Լ�
int	dm_DecodeDamagePacket( rsPLAYINFO *lpPlayInfo , void *lpPacket )
{
	DWORD size,code;
	DWORD cnt,cnt2,cnt3;
	DWORD dwCode;
	DWORD dwCode2;
	DWORD *lpDword = (DWORD *)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /=4;

	dwCode = (code^(code<<5)) + size*0x00003434;
	cnt2 = (code+0x00008989)&0xBCAFF5;
	cnt3 = cnt2&0x5E;

	for( cnt=2;cnt<size;cnt++ ) {
		dwCode2 = lpDword[cnt];
		lpDword[cnt] = dwCode2^(cnt2<<7)^dwCode;
		dwCode = dwCode2;
		cnt2+=cnt2+cnt3;
	}

	return TRUE;
}
*/

#ifdef _W_SERVER

#define	DM_ENCODE_PACKET_SIZE	0x60
#define	DM_ENCODE_PACKET_KEY1	0x13
#define	DM_ENCODE_PACKET_KEY2	0x1F

BYTE dm_EncodePacket_AsmCode[DM_ENCODE_PACKET_SIZE] = {
	0x53, 0x56, 0x57, 0x8b, 0x7c, 0x24, 0x10, 0x8b, 0x37, 0x8b, 0x47, 0x04, 0xc1, 0xee, 0x02, 0x8b,
	0xd6, 0x69, 0xd2, 0x44, 0x33, 0x22, 0x11, 0x8b, 0xc8, 0xc1, 0xe1, 0x07, 0x33, 0xc8, 0x05, 0x40,
	0x38, 0xaa, 0xff, 0x25, 0xcf, 0xab, 0xf5, 0x00, 0x03, 0xca, 0x8b, 0xd8, 0xba, 0x02, 0x00, 0x00,
	0x00, 0x83, 0xe3, 0x3c, 0x3b, 0xf2, 0x76, 0x1e, 0x55, 0x8d, 0xa4, 0x24, 0x00, 0x00, 0x00, 0x00,
	0x8b, 0xe8, 0xc1, 0xe5, 0x06, 0x33, 0x2c, 0x97, 0x8d, 0x04, 0x43, 0x33, 0xcd, 0x89, 0x0c, 0x97,
	0x42, 0x3b, 0xd6, 0x72, 0xeb, 0x5d, 0x5f, 0x5e, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x5b, 0xc3, 0x90
};

#define	DM_DECODE_PACKET_SIZE	0x70
#define	DM_DECODE_PACKET_KEY1	0x13
#define	DM_DECODE_PACKET_KEY2	0x1F

BYTE dm_DecodePacket_AsmCode[DM_DECODE_PACKET_SIZE] = {
	0x53,0x56,0x57,0x8b,0x7c,0x24,0x14,0x8b,0x37,0x8b,0x47,0x04,0xc1,0xee,0x02,0x8b,
	0xd6,0x69,0xd2,0x34,0x34,0x00,0x00,0x8b,0xc8,0xc1,0xe1,0x05,0x33,0xc8,0x05,0x89,
	0x89,0x00,0x00,0x25,0xf5,0xaf,0xbc,0x00,0x03,0xca,0x8b,0xd8,0xba,0x02,0x00,0x00,
	0x00,0x83,0xe3,0x5e,0x3b,0xf2,0x76,0x2a,0x55,0x8d,0xa4,0x24,0x00,0x00,0x00,0x00,
	0x8b,0x2c,0x97,0x89,0x6c,0x24,0x18,0x8b,0xe8,0xc1,0xe5,0x07,0x33,0x6c,0x24,0x18,
	0x8d,0x04,0x43,0x33,0xe9,0x8b,0x4c,0x24,0x18,0x89,0x2c,0x97,0x42,0x3b,0xd6,0x72,
	0xdf,0x5d,0x5f,0x5e,0xb8,0x01,0x00,0x00,0x00,0x5b,0xc3,0x90,0x90,0x90,0x90,0x90
};

//������ ��ȣȭ Ű����
int rsSetDamagePacketKey( rsPLAYINFO *lpPlayInfo )
{
	DWORD	dwChkSum;
	DWORD	dwObjSum;

	if ( lpPlayInfo->szName[0] )
		dwChkSum = GetSpeedSum( lpPlayInfo->szName );
	else
		dwChkSum = GetSpeedSum( lpPlayInfo->smCharInfo.szName );

	dwObjSum = lpPlayInfo->dwObjectSerial;

	lpPlayInfo->dwDamagePacketKey[0] = (dwChkSum*dwObjSum/7)+0x13;
	lpPlayInfo->dwDamagePacketKey[1] = lpPlayInfo->dwDamagePacketKey[0]^((dwObjSum>>8)+(dwObjSum<<15))+0x1F;

	dwObjSum *= (lpPlayInfo->smCharInfo.JOB_CODE+2);
	lpPlayInfo->dwDamagePacketKey2[0] = (dwChkSum*dwObjSum/11)+0x34;
	lpPlayInfo->dwDamagePacketKey2[1] = lpPlayInfo->dwDamagePacketKey2[0]^((dwObjSum>>7)+(dwObjSum<<16))+0x37;

	return TRUE;
}

//������ ��ȣȭ ����� ��ä�� Ŭ���̾�Ʈ�� ������
int rsSendDamageFuncToClient( rsPLAYINFO *lpPlayInfo )
{
	TRANS_FUNC_MEMORY	TransFuncMemory;

	ZeroMemory( &TransFuncMemory, sizeof(TRANS_FUNC_MEMORY) );

	TransFuncMemory.size = DM_ENCODE_PACKET_SIZE+64+(rand()%3)*4;
	TransFuncMemory.code = smTRANSCODE_DAMAGE_ENCODE_MEM;

	TransFuncMemory.Param[0] = DM_ENCODE_PACKET_SIZE;

	memcpy( TransFuncMemory.szData , dm_EncodePacket_AsmCode , DM_ENCODE_PACKET_SIZE );

	rsSetDamagePacketKey( lpPlayInfo );			//��Ŷ ��ȣȭ�� Ű ����

	((DWORD *)(TransFuncMemory.szData+DM_ENCODE_PACKET_KEY1))[0] = lpPlayInfo->dwDamagePacketKey[1];
	((DWORD *)(TransFuncMemory.szData+DM_ENCODE_PACKET_KEY2))[0] = lpPlayInfo->dwDamagePacketKey[0];

	if ( lpPlayInfo && lpPlayInfo->lpsmSock )
	{
		return lpPlayInfo->lpsmSock->Send2( (char *)&TransFuncMemory , TransFuncMemory.size , TRUE );
	}

	return FALSE;
}

//������ ��ȣȭ ����� ��ä�� Ŭ���̾�Ʈ�� ������
int rsSendDamageFuncToClient2( rsPLAYINFO *lpPlayInfo )
{
	TRANS_FUNC_MEMORY	TransFuncMemory;

	ZeroMemory( &TransFuncMemory, sizeof(TRANS_FUNC_MEMORY) );

	TransFuncMemory.size = DM_DECODE_PACKET_SIZE+64+(rand()%3)*4;
	TransFuncMemory.code = smTRANSCODE_DAMAGE_ENCODE_MEM2;

	TransFuncMemory.Param[0] = DM_DECODE_PACKET_SIZE;

	memcpy( TransFuncMemory.szData , dm_DecodePacket_AsmCode , DM_DECODE_PACKET_SIZE );

	rsSetDamagePacketKey( lpPlayInfo );			//��Ŷ ��ȣȭ�� Ű ����

	((DWORD *)(TransFuncMemory.szData+DM_DECODE_PACKET_KEY1))[0] = lpPlayInfo->dwDamagePacketKey2[1];
	((DWORD *)(TransFuncMemory.szData+DM_DECODE_PACKET_KEY2))[0] = lpPlayInfo->dwDamagePacketKey2[0];

	if ( lpPlayInfo && lpPlayInfo->lpsmSock ) {
		return lpPlayInfo->lpsmSock->Send2( (char *)&TransFuncMemory , TransFuncMemory.size , TRUE );
	}

	return FALSE;
}

//���� ��ȣȭ ����� ��ä�� Ŭ���̾�Ʈ�� ������
int rsSendPacketFuncToClient( rsPLAYINFO *lpPlayInfo , DWORD dwEncPacketCode , BYTE bEncXor , BYTE bDecXor )
{

	TRANS_FUNC_MEMORY	TransFuncMemory;

	ZeroMemory( &TransFuncMemory, sizeof(TRANS_FUNC_MEMORY) );

	TransFuncMemory.size = ENC_PACKET_ASM_SIZE+64+(rand()%3)*4;
	TransFuncMemory.code = smTRANSCODE_PACKET_DYNFUNC;

	TransFuncMemory.Param[0] = ENC_PACKET_ASM_SIZE;
	TransFuncMemory.Param[1] = POS_ENC_START;
	TransFuncMemory.Param[2] = POS_DEC_START;

	memcpy( TransFuncMemory.szData , smwsock_EncodePacket_AsmCode , ENC_PACKET_ASM_SIZE );

	((DWORD *)(TransFuncMemory.szData+POS_ENC_PACCODE))[0]	= dwEncPacketCode;
	((BYTE *)(TransFuncMemory.szData+POS_ENC_XOR))[0]		= bEncXor;
	((BYTE *)(TransFuncMemory.szData+POS_DEC_XOR))[0]		= bDecXor;

	if ( lpPlayInfo && lpPlayInfo->lpsmSock ) {
		return lpPlayInfo->lpsmSock->Send2( (char *)&TransFuncMemory , TransFuncMemory.size , TRUE );
	}

	return FALSE;
}

#define	PACKET_SAFE_CODE		704
#define	PACKET_SAFE_SHIFT1		2
#define	PACKET_SAFE_SHIFT2		3


//��ȣ ��� ����
int	rsProcessSafePacket( rsPLAYINFO *lpPlayInfo )
{

	DWORD	dwChkSum;
	DWORD	dwObjSum;
	BYTE	bEncXor,bDecXor;
	DWORD	dwEncPacketCode;

	if ( !rsServerConfig.CrazyPacketMode ) return FALSE;

	if ( lpPlayInfo->szName[0] )
		dwChkSum = GetSpeedSum( lpPlayInfo->szName );
	else
		dwChkSum = GetSpeedSum( lpPlayInfo->smCharInfo.szName );

	dwObjSum = (lpPlayInfo->dwObjectSerial+PACKET_SAFE_CODE)*dwChkSum;

	dwEncPacketCode = (dwObjSum^dwChkSum)&0x0FFF0000;
	dwEncPacketCode |= smTRANSCODE_ENCODE_PACKET2;
	bEncXor = (BYTE)(dwObjSum^(dwChkSum>>PACKET_SAFE_SHIFT1))&0x7F;
	bDecXor = (BYTE)((dwObjSum>>PACKET_SAFE_SHIFT2)^dwChkSum)&0x7F;

	rsSendPacketFuncToClient( lpPlayInfo , dwEncPacketCode , bDecXor ,bEncXor );

	lpPlayInfo->dwDecPacketCode = dwEncPacketCode;
	lpPlayInfo->bPacketXor[0] = bEncXor;
	lpPlayInfo->bPacketXor[1] = bDecXor;

	lpPlayInfo->lpsmSock->dwEncPacketCode = smTRANSCODE_ENCODE_PACKET2;
	lpPlayInfo->lpsmSock->bEncXor = bEncXor;
	lpPlayInfo->lpsmSock->bDecXor = bDecXor;

	lpPlayInfo->dwDecPacketTime = dwPlayServTime;
	lpPlayInfo->dwDecPacketTime2 = dwPlayServTime+60000;		//1�� ���� ��ȣȭ ��� ���� - ���� ��Ŷ�� ���

	return TRUE;
}

//��ȣ ������� Ȯ���Ͽ� ����
int	rsCompareSafePacket( rsPLAYINFO *lpPlayInfo , DWORD dwRcvPacketCode )
{
	DWORD	dwChkSum;
	DWORD	dwObjSum;
	BYTE	bEncXor,bDecXor;
	DWORD	dwEncPacketCode;

	if ( (dwRcvPacketCode>>28)!=9 ) return FALSE;

	if ( lpPlayInfo->szName[0] )
		dwChkSum = GetSpeedSum( lpPlayInfo->szName );
	else
		dwChkSum = GetSpeedSum( lpPlayInfo->smCharInfo.szName );

	dwObjSum = (lpPlayInfo->dwObjectSerial+PACKET_SAFE_CODE)*dwChkSum;

	dwEncPacketCode = (dwObjSum^dwChkSum)&0x0FFF0000;
	dwEncPacketCode |= smTRANSCODE_ENCODE_PACKET2;
	bEncXor = (BYTE)(dwObjSum^(dwChkSum>>PACKET_SAFE_SHIFT1))&0x7F;
	bDecXor = (BYTE)((dwObjSum>>PACKET_SAFE_SHIFT2)^dwChkSum)&0x7F;

	if ( dwRcvPacketCode==dwEncPacketCode ) {
		lpPlayInfo->dwDecPacketCode = dwEncPacketCode;
		lpPlayInfo->bPacketXor[0] = bEncXor;
		lpPlayInfo->bPacketXor[1] = bDecXor;

		lpPlayInfo->lpsmSock->dwEncPacketCode = smTRANSCODE_ENCODE_PACKET2;
		lpPlayInfo->lpsmSock->bEncXor = bEncXor;
		lpPlayInfo->lpsmSock->bDecXor = bDecXor;

		return TRUE;
	}

	return FALSE;
}

//������ ��ȣ Ǯ�� �Լ�
int	rsDecodeDamagePacket( rsPLAYINFO *lpPlayInfo , void *lpPacket )
{
	DWORD size,code;
	DWORD cnt,cnt2,cnt3;
	DWORD dwCode;
	DWORD dwCode2;
	DWORD *lpDword = (DWORD *)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /=4;

	dwCode = (code^(code<<7)) + size*lpPlayInfo->dwDamagePacketKey[1];
	cnt2 = (code+lpPlayInfo->dwDamagePacketKey[0])&0xF5ABCF;
	cnt3 = cnt2&0x3C;

	for( cnt=2;cnt<size;cnt++ )
	{
		dwCode2 = lpDword[cnt];
		lpDword[cnt] = dwCode2^(cnt2<<6)^dwCode;
		dwCode = dwCode2;
		cnt2+=cnt2+cnt3;
	}

	return TRUE;
}

int	rsEncodeDamagePacket( rsPLAYINFO *lpPlayInfo , void *lpPacket )
{
	DWORD size,code;
	DWORD cnt,cnt2,cnt3;
	DWORD dwCode;
	DWORD *lpDword = (DWORD *)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /=4;


	dwCode = (code^(code<<5)) + size*lpPlayInfo->dwDamagePacketKey2[1];
	cnt2 = (code+lpPlayInfo->dwDamagePacketKey2[0])&0xBCAFF5;
	cnt3 = cnt2&0x5E;

	for( cnt=2;cnt<size;cnt++ ) 
	{
		dwCode = dwCode^(cnt2<<7)^lpDword[cnt];
		lpDword[cnt] = dwCode;
		cnt2+=cnt2+cnt3;
	}

	return TRUE;
}


#else

//������ ��ȣȭ Ű����
int rsSetDamagePacketKey( rsPLAYINFO *lpPlayInfo )
{
	return TRUE;
}

//������ ��ȣȭ ����� ��ä�� Ŭ���̾�Ʈ�� ������
int rsSendDamageFuncToClient( rsPLAYINFO *lpPlayInfo )
{
	return TRUE;
}
//������ ��ȣ Ǯ�� �Լ�
int	rsDecodeDamagePacket( rsPLAYINFO *lpPlayInfo , void *lpPacket )
{
	return TRUE;
}
int	rsEncodeDamagePacket( rsPLAYINFO *lpPlayInfo , void *lpPacket )
{
	return TRUE;
}

#endif



//������ ����Ÿ ������ ������ �α׿� ���
int rsRecordDamageError( rsPLAYINFO *lpPlayInfo , smTRANS_COMMAND *lpTransCommand )
{

	if ( lpPlayInfo->WarningCount<20 ) {
		rsSendDataServer( lpPlayInfo->lpsmSock , lpTransCommand );
		lpPlayInfo->WarningCount++;
	}

	return TRUE;
}


//ATTACK_DAMAGE_LIST_MAX
/*
//�������� �� ������ ����Ŵ
struct	ATTACK_DAMAGE_LIST	{
	DWORD	dwUserCode;
	int		DamageCount;
};
*/

//ĳ���� ������ ���
int	rsRecordCharDamage( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , int Damage )
{
	int cnt;
	int EmptyNum = -1;
	DWORD	dwClanCode;

	if ( lpChar->lpAttackDamageList_BlessCastle ) {
		//���� ĳ�� ũ����Ż Ÿ�� ( Ŭ������ )
		if ( lpPlayInfo->dwClanCode && lpPlayInfo->Position.Area==rsCASTLE_FIELD ) {

			lpPlayInfo->sBlessCastle_Damage[0] += Damage;	//������ ���� �߰�

			for(cnt=0;cnt<ATTACK_DAMAGE_LIST_MAX;cnt++) {
				if ( lpChar->lpAttackDamageList_BlessCastle[cnt].dwUserCode ) {
					dwClanCode = GetClanCode(lpChar->lpAttackDamageList_BlessCastle[cnt].dwUserCode);
					if ( dwClanCode==lpPlayInfo->dwClanCode ) {
						lpChar->lpAttackDamageList_BlessCastle[cnt].DamageCount += Damage;
						lpChar->lpAttackDamageList_BlessCastle[cnt].Count ++;
						return TRUE;
					}
				}
				if ( EmptyNum<0 && lpChar->lpAttackDamageList_BlessCastle[cnt].dwUserCode==0 ) {
					EmptyNum = cnt;
				}

			}

			if ( EmptyNum>=0 ) {
				lpChar->lpAttackDamageList_BlessCastle[EmptyNum].dwUserCode = lpPlayInfo->smCharInfo.ClassClan;
				lpChar->lpAttackDamageList_BlessCastle[EmptyNum].DamageCount = Damage;
				lpChar->lpAttackDamageList_BlessCastle[EmptyNum].Count++;

				return TRUE;
			}
		}
		return TRUE;
	}

	if ( !lpChar->lpAttackDamageList ) return FALSE;

	if ( lpChar->smCharInfo.dwCharSoundCode==snCHAR_SOUND_CASTLE_TOWER_B ) {
		//���Ҷ� ž ( Ŭ������ )
		if ( lpPlayInfo->dwClanCode && lpPlayInfo->Position.Area==rsCASTLE_FIELD ) {

			lpPlayInfo->sBlessCastle_Damage[0] += Damage;	//������ ���� �߰�

			for(cnt=0;cnt<ATTACK_DAMAGE_LIST_MAX;cnt++) {
				if ( lpChar->lpAttackDamageList[cnt].dwUserCode ) {
					dwClanCode = GetClanCode(lpChar->lpAttackDamageList[cnt].dwUserCode);
					if ( dwClanCode==lpPlayInfo->dwClanCode ) {
						lpChar->lpAttackDamageList[cnt].DamageCount += Damage;
						lpChar->lpAttackDamageList[cnt].Count ++;
						return TRUE;
					}
				}
				if ( EmptyNum<0 && lpChar->lpAttackDamageList[cnt].dwUserCode==0 ) {
					EmptyNum = cnt;
				}

			}

			if ( EmptyNum>=0 ) {
				lpChar->lpAttackDamageList[EmptyNum].dwUserCode = lpPlayInfo->smCharInfo.ClassClan;
				lpChar->lpAttackDamageList[EmptyNum].DamageCount = Damage;
				lpChar->lpAttackDamageList[EmptyNum].Count++;

				return TRUE;
			}
		}
	}
	else {
		//�Ϲ� ���� (����)
		for(cnt=0;cnt<ATTACK_DAMAGE_LIST_MAX;cnt++) {
			if ( lpChar->lpAttackDamageList[cnt].dwUserCode==lpPlayInfo->dwObjectSerial ) {
				lpChar->lpAttackDamageList[cnt].DamageCount += Damage;
				lpChar->lpAttackDamageList[cnt].Count ++;
				return TRUE;
			}
			if ( EmptyNum<0 && lpChar->lpAttackDamageList[cnt].dwUserCode==0 ) {
				EmptyNum = cnt;
			}
		}

		if ( EmptyNum>=0 ) {
			lpChar->lpAttackDamageList[EmptyNum].dwUserCode = lpPlayInfo->dwObjectSerial;
			lpChar->lpAttackDamageList[EmptyNum].DamageCount = Damage;
			lpChar->lpAttackDamageList[EmptyNum].Count++;

			return TRUE;
		}
	}

	return FALSE;
}

//ĳ���� PK ���
int	rsRecord_PKDamage( rsPLAYINFO *lpPlayInfo , rsPLAYINFO *lpPlayInfo2 , int Damage )
{
	int cnt;
	ATTACK_DAMAGE_LIST *lpAttackDamageList = 0;
	DWORD dwClanCode;
	int EmptyNum = -1;


	if ( !lpPlayInfo2->dwClanCode || lpPlayInfo2->dwClanCode==lpPlayInfo->dwClanCode ) {
		if ( lpPlayInfo2->dwClanCode!=rsBlessCastle.dwMasterClan && lpPlayInfo->dwClanCode!=rsBlessCastle.dwMasterClan ) 
		{ 
			//����/Ÿ�� ���� Ŭ�� Ȯ���Ͽ� �������� ���������� ���� �Ұ�
			return FALSE;
		}
	}

	if ( rsBlessCastle.lpAttackDamageList ) {
		lpAttackDamageList = rsBlessCastle.lpAttackDamageList;		//ũ����Ż Ÿ�� ������
	}
	else if ( rsBlessCastle.lpChar_ValhallaTower ) {
		//���Ҷ� Ÿ�� ������		
		if ( rsBlessCastle.lpChar_ValhallaTower->Flag && 
			rsBlessCastle.lpChar_ValhallaTower->smCharInfo.dwCharSoundCode==snCHAR_SOUND_CASTLE_TOWER_B &&
			rsBlessCastle.lpChar_ValhallaTower->smCharInfo.Life[0]>0 ) 
		{
			lpAttackDamageList = rsBlessCastle.lpChar_ValhallaTower->lpAttackDamageList;
		}
	}

	if ( !lpAttackDamageList ) return FALSE;

	Damage -= lpPlayInfo->smCharInfo.Absorption;			//����� ����
	Damage /= PK_SCORE_DIVIDE;								//PK ���� 1/10 ����
	if ( Damage<=0 ) {
		Damage = 1;
	}

	lpPlayInfo->sBlessCastle_Damage[0] += Damage;	//������ ���� �߰�

	// ( Ŭ������ )
	if ( lpPlayInfo->dwClanCode && lpPlayInfo->Position.Area==rsCASTLE_FIELD ) {

		for(cnt=0;cnt<ATTACK_DAMAGE_LIST_MAX;cnt++) {
			if ( lpAttackDamageList[cnt].dwUserCode ) {
				dwClanCode = GetClanCode( lpAttackDamageList[cnt].dwUserCode);
				if ( dwClanCode==lpPlayInfo->dwClanCode ) {
					lpAttackDamageList[cnt].DamageCount += Damage;
					lpAttackDamageList[cnt].Count ++;
					return TRUE;
				}
			}
			if ( EmptyNum<0 && lpAttackDamageList[cnt].dwUserCode==0 ) {
				EmptyNum = cnt;
			}
		}
		if ( EmptyNum>=0 ) {
			lpAttackDamageList[EmptyNum].dwUserCode = lpPlayInfo->smCharInfo.ClassClan;
			lpAttackDamageList[EmptyNum].DamageCount = Damage;
			lpAttackDamageList[EmptyNum].Count++;

			return TRUE;
		}
	}

	return TRUE;
}


/*
#ifdef _LANGUAGE_KOREAN
			if ( lpPlayInfo->AdminMode ) {
				TRANS_CHATMESSAGE	TransChatMessage;

				wsprintf( TransChatMessage.szMessage , "���� ��ġ���� ( %d %d %d ) ( %d %d %d )" ,
					lpTransAttackData->Power[0] , lpTransAttackData->Power[1] , lpTransAttackData->Critical[0],
					lpPlayInfo->sLimitDamage[0] , lpPlayInfo->sLimitDamage[1] , lpPlayInfo->sLimitCritical[0] );

				TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
				TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
				TransChatMessage.dwIP = 0;
				TransChatMessage.dwObjectSerial = 0;
				lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
			}
#endif
*/

//���� ������ ��ų ����
int	rsBillingItemSkill( rsPLAYINFO *lpPlayInfo , smCHAR *lpChar , int HitMonsters , int Power , int SkillCode  )
{
	//int cnt,cps;
	float fps,fp1,fp2;
	smTRANS_COMMAND	smTransCommand;
//	TRANS_CHATMESSAGE	TransChatMessage;
	DWORD	dwServTime_T = (DWORD)tServerTime;

	if ( SkillCode==SKILL_PLAY_ROAR || SkillCode==SKILL_PLAY_METAL_GOLEM || SkillCode==SKILL_PLAY_COMPULSION || SkillCode==SKILL_PLAY_EXTINCTION ||
		SkillCode==SKILL_PLAY_RECALL_WOLVERIN || SkillCode==SKILL_PLAY_FIRE_ELEMENTAL || SkillCode==SKILL_PLAY_DISTORTION ||
		SkillCode==SKILL_PLAY_PET_ATTACK || SkillCode==SKILL_PLAY_PET_ATTACK2 || lpPlayInfo->Position.Area==rsCASTLE_FIELD )
	{
		return FALSE;
	}

	//�ؿ� �ӽ�
	if(!lpChar) return FALSE;

	if ( lpChar ) {
		if ( lpChar->smCharInfo.State==smCHAR_STATE_NPC || lpChar->smCharInfo.Brood==smCHAR_MONSTER_USER )
			return FALSE;
	}

	if ( lpPlayInfo->dwTime_PrimeItem_VampCuspid ) {
		if ( lpPlayInfo->dwTime_PrimeItem_VampCuspid>dwServTime_T ) {
			// pluto ���Ǹ� Ŀ���� ��ġ ����
			fp1 = float(lpPlayInfo->smCharInfo.Strength+(((float)Power)/10.0f))/20.0f;
			fp2 = (1.0f+(((float)HitMonsters)/10.0f));
			fps = fp1*fp2*HitMonsters+1.0f;

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_VAMPRIC_CUSPID;
			smTransCommand.WParam =	(int)fps;
			smTransCommand.LParam =	0;								//
			smTransCommand.SParam =	0;
			smTransCommand.EParam = lpPlayInfo->dwObjectSerial;	//

			if ( smTransCommand.WParam )
				lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );


#ifdef _LANGUAGE_KOREAN
			TRANS_CHATMESSAGE	TransChatMessage;
			if ( lpPlayInfo->AdminMode>1 ) {
				wsprintf( TransChatMessage.szMessage , "���Ǹ� Ŀ���� [%d] Stength(%d) HitMonsters(%d) Power(%d) <%d^%d> Time(%d)" ,
					smTransCommand.WParam,lpPlayInfo->smCharInfo.Strength,HitMonsters,Power , (int)fp1 , (int)fp2 , (lpPlayInfo->dwTime_PrimeItem_VampCuspid-dwServTime_T)/60 );

				TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
				TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
				TransChatMessage.dwIP = 0;
				TransChatMessage.dwObjectSerial = 0;
				lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
			}
#endif


		}
		else 
			lpPlayInfo->dwTime_PrimeItem_VampCuspid = 0;
	}


	// �庰 - ���Ǹ� Ŀ���� EX
	if ( lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX ) 
	{
		if ( lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX>dwServTime_T ) 
		{
			// ���Ǹ� Ŀ���� EX ��ġ
			fp1 = float(lpPlayInfo->smCharInfo.Strength+(((float)Power)/10.0f))/15.0f;
			fp2 = (1.0f+(((float)HitMonsters)/7.0f));
			fps = fp1*fp2*HitMonsters+1.0f;

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_VAMPRIC_CUSPID_EX;
			smTransCommand.WParam =	(int)fps;
			smTransCommand.LParam =	0;								//
			smTransCommand.SParam =	0;
			smTransCommand.EParam = lpPlayInfo->dwObjectSerial;	//

			if ( smTransCommand.WParam )
				lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );


#ifdef _LANGUAGE_JAPANESE 
			TRANS_CHATMESSAGE	TransChatMessage;
			if ( lpPlayInfo->AdminMode>1 ) 
			{
				wsprintf( TransChatMessage.szMessage , "VAMPRIC_CUSPID EX [%d] Stength(%d) HitMonsters(%d) Power(%d) <%d^%d> Time(%d)" ,
					smTransCommand.WParam,lpPlayInfo->smCharInfo.Strength,HitMonsters,Power , (int)fp1 , (int)fp2 , (lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX-dwServTime_T)/60 );

				TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
				TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
				TransChatMessage.dwIP = 0;
				TransChatMessage.dwObjectSerial = 0;
				lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
			}
#endif


		}
		else 
			lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX = 0;
	}


	if ( lpPlayInfo->dwTime_PrimeItem_ManaRecharg ) {
		if ( lpPlayInfo->dwTime_PrimeItem_ManaRecharg>dwServTime_T ) {
/*
			fps = float(lpPlayInfo->smCharInfo.Spirit+Power)/50+1;
			fp2 = (1.0f-((float)HitMonsters)/20.0f);
			cps = (int)fp2;
			for(cnt=0;cnt<cps;cnt++) {
				fps *= fps;
			}
*/
			// pluto ���� ����¡ ���� ��ġ ����
			fp1 = float(lpPlayInfo->smCharInfo.Spirit+(((float)Power)/10.0f))/13.0f;
			fp2 = (1.0f+(((float)HitMonsters)/10.0f));
			fps = fp1*fp2*HitMonsters+1.0f;

			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.code = smTRANSCODE_MANA_RECHARGING;
			smTransCommand.WParam =	(int)fps;
			smTransCommand.LParam =	0;								//
			smTransCommand.SParam =	0;
			smTransCommand.EParam = lpPlayInfo->dwObjectSerial;	//
			if ( smTransCommand.WParam )
				lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

#ifdef _LANGUAGE_KOREAN
			TRANS_CHATMESSAGE	TransChatMessage;
			if ( lpPlayInfo->AdminMode>1 ) {
				wsprintf( TransChatMessage.szMessage , "���� ����¡ [%d] Spirit(%d) HitMonsters(%d) Power(%d) <%d^%d> Time(%d)" ,
					smTransCommand.WParam,lpPlayInfo->smCharInfo.Spirit,HitMonsters,Power , (int)fp1 , (int)fp2 , (lpPlayInfo->dwTime_PrimeItem_ManaRecharg-dwServTime_T)/60 );

				TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
				TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
				TransChatMessage.dwIP = 0;
				TransChatMessage.dwObjectSerial = 0;
				lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
			}
#endif


		}
		else
			lpPlayInfo->dwTime_PrimeItem_ManaRecharg = 0;
	}


	return TRUE;
}

