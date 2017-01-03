
#define CE_CJOIN_CPP
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>

#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��

#include "sinbaram\\SinLinkHeader.h"
#include "HoBaram\\HoLinkHeader.h"
#include "Language\\language.h"
#include "HoBaram\\HoTextFile.h"
#endif

#include "tjclan.h"
#include "cE_report.h"
#include "cE_CJoin.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"

//void CheckMsgPos(CLANMSG* clanmsg,int width, int height);

CLANEntity_JOIN::CLANEntity_JOIN()
{
	nJoinAnswer = 0;
	ZeroMemory(szTemp,sizeof(szTemp));
	ZeroMemory(szTemp1,sizeof(szTemp1));
}

CLANEntity_JOIN::~CLANEntity_JOIN()
{
}

int CLANEntity_JOIN::Init(int Life)
{
	life = Life;
	ZeroMemory(szUserJoin,sizeof(szUserJoin));
	return 1;
}

int CLANEntity_JOIN::Clear()
{
	return 1;
}

void CLANEntity_JOIN::Main()
{
	int i;
	//char		szErrorMsg[256];
	switch(C_menuN)
	{
	case CLAN_CLANWONOVER:
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		if(MOUSE0())			
		{	
			MOUSE0()=0;			
			switch(i) 
			{
			case 0:
				C_menuN = CLAN_AllEnd;
				//C_menuN = CLAN_ERROR_ret;		//������ : �����°ɷΰ���.
				//menuInit(C_menuN );

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				break;

			}
		}
		break;		
	case CLAN_JOIN_before:

		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;
			
			WebDB.Resume() ;		//��db�� �簳�Ѵ�.
			WebDB.isClanMember(cldata.szID,cldata.gserver,cldata.ChaName);		//�� id�� Ŭ���������� �ľ��Ѵ�.
			C_menuN2=1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before���� WebDB.isClanMember(���̵� : %s,���Ӽ��� : %s,ĳ�����̸� : %s)",cldata.szID,cldata.gserver,cldata.ChaName);
#endif
			break;
		case 1:
			switch(ANSdata.ret_val) 
			{
			case 100:						//�Ϲ� Ŭ����
				
				WebDB.Suspend(); //��db ����

				C_menuN = CLAN_JOIN_After;
				menuInit(C_menuN );
				joinAnswer = 100;			//�̴̹ٸ�Ŭ�������Ե��־� ���ԺҰ�

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before���� �����ڵ� 100");
#endif
				break;

			case 1:                         //Ŭ��Ĩ
			case 2:                         //Ŭ����
			case 5:						//��Ŭ��
			case 101:						//Ŭ����( ����Ÿ�б�� ����)
			case 103:						//Ŭ���� �ɻ���
			case 104:						//Ŭ���� �ɻ���
				WebDB.Suspend();
				C_menuN = CLAN_JOIN_After;		
				menuInit(C_menuN );
				joinAnswer = 101;			//�̴̹ٸ�Ŭ���� Ŭ�����̶� ���ԺҰ�

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before���� Ŭ�� ���ԺҰ�(%d)",ANSdata.ret_val);
#endif

				break;
			case 4:
			case 3:
			case 0:						//�����.
			case 105:						//����
				WebDB.Suspend(); 
				C_menuN = CLAN_JOIN;
				//C_menuN = CLANUSER_JOIN;
				menuInit(C_menuN );

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before���� Ŭ�����԰���(%d)",ANSdata.ret_val);
#endif

				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (55)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before���� ������ �����Ҽ� ����(%d)",ANSdata.ret_val);
#endif
				break;
			default:
				if(ANSdata.ret_val != -1) {		//���������� ���� �ͼ� ������ ����.
					WebDB.Suspend();

					C_menuN = CLAN_ERROR;				//Ŭ���Ἲ���ϴ� ���ư���� �ϴ¸޴��� ����.
					wsprintf(szErrorMsg,"%s (56)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);		
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);

					C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_before���� ����Ʈ����(%d)",ANSdata.ret_val);
#endif
				}
				break;
			}
			break;
		}
		break;

	case CLANUSER_JOIN:

		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			i = i;
			switch(i) 
			{
			case 0:
				C_menuN = CLAN_AllEnd;
				//C_menuN = CLAN_ERROR_ret;		//������ : �����°ɷΰ���.
				//menuInit(C_menuN );

				C_menuN2 =-1;
				CLAN_ERROR_ret=-1;
				break;	
			default:
				i = i;
				break;

			}
		}
		break;
		

	case CLAN_JOIN:
		i = chkeckMenuN();				//================================================
		memset(menuMouseOver,-1,sizeof(int)*30);
		switch(i) {
		case 0:		//yes
			menuMouseOver[0] = 1;
			break;
		case 1:		//no
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		if(MOUSE0())
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:		
				
				wsprintf(szUserJoin, clanClanJoinMsg.szMsg,cldata.szJoinClanName,cldata.szJoinClanChipName,cldata.ChaName);				
				//void CheckMsgPos(CLANMSG* clanmsg);
				CheckMsgPos(&clanClanJoinMsg,BOX_MAX_WIDTH,0);								
				C_menuN = CLANUSER_JOIN;
				
				menuInit(C_menuN);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//�����޴��������� �ٽ� ���ư���.
				nJoinAnswer = 1;

#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��

				SendJoinClan();						//Ŭ�� ���� ���� �޼��� ������ ( ����� )
#endif

				break;
			case 1:
				nJoinAnswer = 0;
				C_menuN = CLAN_AllEnd;			//N : Ŭ���� ���Ծ��Ѵ�.
				break;

			default:
				break;
			}
		}
		break;

	case CLAN_JOIN_After:						//�ϴ� ������.
		C_menuN = CLAN_AllEnd;
		break;

	//ȣ��Clan�߰�
	case CLAN_IS_CHECK_MEMBER_END:
		C_menuN = CLAN_AllEnd;
		break;

	//(Ŭ���� Ż������ 4���� �Ǿ����� �˻�)
	case CLAN_IS_CHECK_MEMBER:
		switch(C_menuN2)
		{
		case 0:
			ANSdata.ret_val = -1;
			WebDB.Resume();
			WebDB.isCheckClanMember(cldata.gserver, cldata.TargetChar->smCharInfo.szName);
			C_menuN2 = 1;
			break;
		case 1:
			if(ANSdata.ret_val != -1)
			{
				//Ŭ�� ������ ���ѵ� �Ǵ� ĳ����
       			if(ANSdata.ret_val == 0)
				{
					WebDB.Suspend();
					//���� ���
					//A������ Ŭ�� Ż�� �� 4���� ��� ���� �ʾ� Ŭ���� ����� �Ҽ� �����ϴ�.
					CLANMSG clanMsg;
					wsprintf(clanMsg.szMsg, "%s", "Error");
					clanMsg.pos.x = 0;
					clanMsg.pos.y = 0;
					CheckMsgPos(&clanMsg,BOX_MAX_WIDTH,0);
		
					C_menuN = CLAN_ERROR;
					
					char TempBufferMsg[256];
					wsprintf(TempBufferMsg, ClanIs4DayCheckMsg.szMsg, cldata.TargetChar->smCharInfo.szName); 
					cClanReport.InitMsg(TempBufferMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					
					CLAN_ERROR_ret=CLAN_IS_CHECK_MEMBER_END;		//�����޴��������� �ٽ� ���ư���.

				}
				else if(ANSdata.ret_val == 1)
				{
 					WebDB.Suspend();
					SendClanJoinService( 1 , cldata.TargetChar );
					
					C_menuN = CLAN_IS_CHECK_MEMBER_END;
					C_menuN2= 0;
					OpenFlag = 1;
					joinAnswer = 0;
					ClanCharMove = 1;

				}
				else
				{
 					WebDB.Suspend();
					//���� ���
					//A������ Ŭ�� Ż�� �� 4���� ��� ���� �ʾ� Ŭ���� ����� �Ҽ� �����ϴ�.
					CLANMSG clanMsg;
					wsprintf(clanMsg.szMsg, "%s", "Error");
					clanMsg.pos.x = 0;
					clanMsg.pos.y = 0;
					CheckMsgPos(&clanMsg,BOX_MAX_WIDTH,0);
		
					C_menuN = CLAN_ERROR;
					
					char TempBufferMsg[256];
					//wsprintf(TempBufferMsg, ClanIs4DayCheckMsg.szMsg, cldata.TargetChar->smCharInfo.szName); 
					#ifdef	_LANGUAGE_JAPANESE
					wsprintf(TempBufferMsg, "�N�����E��\n4���o�߂�?�F�b�N��Error");
					#else
   					wsprintf(TempBufferMsg, "Ŭ��Ż��\n4�ϰ��üũ��Error");
					#endif
					cClanReport.InitMsg(TempBufferMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					
					CLAN_ERROR_ret=CLAN_IS_CHECK_MEMBER_END;		//�����޴��������� �ٽ� ���ư���.
				}
			}
		}
		break;
										//================================================
	case CLAN_JOIN_chip:				//�����ϰٴٴ� ����� Ŭ���� ��Ͻ�Ų��.
		switch(C_menuN2) {				//================================================
		case 0:
			ANSdata.ret_val= -1;
			
			WebDB.Resume() ;		//��db�� �簳�Ѵ�.//Ŭ�������� ���Խ�Ŵ
#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
					WebDB.joinClanMember(cldata.szID,cldata.gserver,cldata.ChaName,
				//Ŭ���̸�	//Ŭ�������� ����һ���� Ķ�����̸�, Ŭ�������� ����һ��id
				cldata.name, cldata.clanJoinCharID,  cldata.clanJoinID, sinChar->Level, cldata.clanJoinJob ,cldata.clanJoinLevel,cldata.isSubChip);//Ÿ��
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip���� WebDB.joinClanMember(���̵� : %s,���Ӽ��� : %s,Ŭ���� : %s,Ŭ���̸� : %s, ������ ĳ���� : %s,  ������ ���̵� : %s, ���� : %d, �� : %d" ,cldata.szID,cldata.gserver,cldata.ChaName,cldata.name, cldata.clanJoinCharID,  cldata.clanJoinID, sinChar->Level, sinChar->JOB_CODE );
#endif

#else
					WebDB.joinClanMember(cldata.szID,cldata.gserver,cldata.ChaName,
				//Ŭ���̸�	//Ŭ�������� ����һ���� Ķ�����̸�, Ŭ�������� ����һ��id
				cldata.name, cldata.clanJoinCharID,  cldata.clanJoinID,70  ,1,50,cldata.isSubChip);
#endif

	


			C_menuN2=1;

			break;
		case 1:
			if(ANSdata.ret_val != -1) {
				ANSdata.ret_val = ANSdata.ret_val;

			}

			switch(ANSdata.ret_val) 
			{
				//ygy
			case 0: //Ʈ����� ����
				WebDB.Suspend();

				//wsprintf(szErrorMsg, szTranzationError);
				C_menuN = CLAN_ERROR;
				menuInit(C_menuN);
				wsprintf(szErrorMsg,"%s (64)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//�����޴��������� �ٽ� ���ư���.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip���� Ʈ����ǿ���");
#endif
				break;
			case 1: //���� ����
				WebDB.Suspend(); //��db ����
				C_menuN = CLAN_JOIN_chip_YES;
				menuInit(C_menuN );
				
				AddClanWon();

				void SetClanWon();
				//SetClanWon();


				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip���� ���Լ���");
#endif
				break;
			case 2: //���� ���Խ�Ű���� Ŭ������ �̹� �ٸ� Ŭ���� ���������� ���(�ڱ� Ŭ������)
				WebDB.Suspend();


				
				C_menuN = CLAN_ERROR;				
				wsprintf(szErrorMsg,"%s (49)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//�����޴��������� �ٽ� ���ư���.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip���� ���� ���Խ�Ű���� Ŭ������ �̹� �ٸ� Ŭ���� ���������� ���(�ڱ� Ŭ������)");
#endif
				break;
			case 3://Ŭ���� �������� �ʰų� Ŭ�� ���� �ƴѰ��
				WebDB.Suspend();

				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (50)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//�����޴��������� �ٽ� ���ư���.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip���� Ŭ���� �������� �ʰų� Ŭ�� ���� �ƴѰ��");
#endif
				break;
			case 4://Ŭ�� ��� �����÷ο� 
				WebDB.Suspend();
				C_menuN = CLAN_CLANWONOVER;
				cClanJoin.menuInit(C_menuN);
				
				//C_menuN = CLAN_ERROR;
				//wsprintf(szErrorMsg,"%s (51)",clanszError_Msg.szMsg);
				//cClanReport.InitMsg(szErrorMsg);		
				//cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//�����޴��������� �ٽ� ���ư���.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip���� Ŭ�� ��� �����÷ο�");
#endif
				break;
			case 100:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (52)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);						
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_JOIN_After;		//�����޴��������� �ٽ� ���ư���.

				C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip���� �����ڵ� 100");
#endif

				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (53)",clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip���� ������ ���� �ȵ�");
#endif
				break;
				//end ygy

			
			default:
				if(ANSdata.ret_val != -1) {		//���������� ���� �ͼ� ������ ����.
					WebDB.Suspend();

					
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (54)",clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);		
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_JOIN_After;		//�����޴��������� �ٽ� ���ư���.

					C_menuN2=0;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_JOIN_chip���� ����Ʈ����");
#endif
				}
				break;
			}
			break;
		}
		break;

	case CLAN_JOIN_chip_NO:				//���������ʰٴٰ���.		
		wsprintf(szTemp, clanClanJoinCancelMsg.szMsg, cldata.clanJoinCharID);
		
		

		strcpy(szTemp1,clanszError_Msg.szMsg);
		strcpy(clanszError_Msg.szMsg,szTemp);

		//void CheckMsgPos(CLANMSG* clanmsg);
		CheckMsgPos(&clanszError_Msg,BOX_MAX_WIDTH,0);
		
		C_menuN = CLAN_ERROR;
		cClanReport.InitMsg(szTemp);		
		cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);

		strcpy(clanszError_Msg.szMsg,szTemp1);

		CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
		break;
	case CLAN_JOIN_chip_YES:				//�����Ѵ���.  //�������� ĳ���Ƶ�
		wsprintf(szTemp, clanClanJoinsucesssMsg.szMsg, cldata.clanJoinCharID);

		strcpy(szTemp1,clanszError_Msg.szMsg);
		strcpy(clanszError_Msg.szMsg,szTemp);
		
		//void CheckMsgPos(CLANMSG* clanmsg);
		CheckMsgPos(&clanszError_Msg,BOX_MAX_WIDTH,0);

		strcpy(szTemp,clanszError_Msg.szMsg);

		C_menuN = CLAN_ERROR;
		cClanReport.InitMsg(szTemp);		
		cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
		strcpy(clanszError_Msg.szMsg,szTemp1);
		CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.




		void SetClanWon();
		SetClanWon();

		break;

	}
}

void CLANEntity_JOIN::menuInit(int num)
{
	int bx =0, by=0;
	switch(num){		
		case CLAN_CLANWONOVER:
			menuSu = 3;
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left = BackStartPos.x + clanClanWonOver.pos.x;
			menuPos[1].top = BackStartPos.y + clanClanWonOver.pos.y;

					#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif
			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//Ȯ��
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;
			break;
		case CLANUSER_JOIN:
			menuSu = 3;
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left = BackStartPos.x + clanClanJoinMsg.pos.x;
			menuPos[1].top = BackStartPos.y + clanClanJoinMsg.pos.y;

					#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif
			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//Ȯ��
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;

			break;
		case CLAN_JOIN:
			//Ŭ������ �����϶�� �����Ҷ� ������� �ߴ� �޴�  =================================START
	
			menuSu = 5;
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[0].left = BackStartPos.x +  BUTTON2_X;		//yes
			menuPos[0].top =  BackStartPos.y +   BUTTON2_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;


			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x +  BUTTON3_X;		//no
			menuPos[1].top =  BackStartPos.y +   BUTTON3_Y;
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			menuPos[2].left = BackStartPos.x +  150;		//Ŭ�� ��ũ
			menuPos[2].top =  BackStartPos.y +   150;
			menuPos[2].right = menuPos[2].left+ bx;
			menuPos[2].bottom = menuPos[2].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[3].left = BackStartPos.x + clanJoin.pos.x;
			menuPos[3].top = BackStartPos.y + clanJoin.pos.y;
			menuPos[3].right = menuPos[3].left + 280;
			menuPos[3].bottom = menuPos[3].top + 250;

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[4].left = BackStartPos.x + BUTTON2_BOX_X;		//yes �׵帮 �ڽ�
			menuPos[4].top = BackStartPos.y +BUTTON2_BOX_Y;
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else
			bx = 64;
			by = 16;
			#endif

			menuPos[5].left = BackStartPos.x + BUTTON3_BOX_X;		//no �׵帮 �ڽ�
			menuPos[5].top = BackStartPos.y +BUTTON3_BOX_Y;
			menuPos[5].right = menuPos[6].left + bx;
			menuPos[5].bottom = menuPos[6].top + by;			
		//Ŭ������ �����϶�� �����Ҷ� ������� �ߴ� �޴�  =================================END
			break;
	}

}

void CLANEntity_JOIN::Draw()
{
	char string[256];
#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
	switch(C_menuN){
		case CLAN_CLANWONOVER:
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
			
			Draw_C_Text(clanClanWonOver.szMsg, menuPos[1].left, menuPos[1].top);		
			break;
		case CLANUSER_JOIN:
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
			
			Draw_C_Text(szUserJoin, menuPos[1].left, menuPos[1].top);		
			break;
		case CLAN_JOIN:
			DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
			DrawSprite(menuPos[5].left,menuPos[5].top, hButton_Box,  0, 0, 48, 23, 1);
			if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);
			else	DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);
			
			if(menuMouseOver[1]>0) DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Yellow,  0, 0, 32, 16, 1);	
			else	DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Gray,  0, 0, 32, 16, 1);			
			
			//wsprintf(string,szJoin,cldata.szJoinClanName,cldata.szJoinClanChipName);
			wsprintf(string,clanJoin.szMsg,cldata.szJoinClanName,cldata.szJoinClanChipName);
			Draw_C_Text(string, menuPos[3].left, menuPos[3].top);		
			break;
	}
#else
	HDC destdc;
	HDC hdc;
	HDC sorDC;
	BITMAP bit;
	int bx,by;
	switch(C_menuN){
		case CLAN_CLANWONOVER:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);		
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off��ũ������.
			SelectObject(sorDC, hButton_Box);		
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;	
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			if(menuMouseOver[0] > 0)
			{
				SelectObject(sorDC, hOk_Yellow);
				GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;	
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hOk_Gray);
				GetObject(hOk_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;	
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}		
			
			//Draw_Text(menuPos[1].left,menuPos[1].top,szReport);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanClanWonOver.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANUSER_JOIN:
			hdc = GetDC(g_hwnd);
			sorDC  = CreateCompatibleDC(hdc);		
			destdc=CreateCompatibleDC(hdc);

			SelectObject(destdc, Dback.hb );		//off��ũ������.
			SelectObject(sorDC, hButton_Box);		
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;	
			BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

			if(menuMouseOver[0] > 0)
			{
				SelectObject(sorDC, hOk_Yellow);
				GetObject(hOk_Yellow,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;	
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}
			else
			{
				SelectObject(sorDC, hOk_Gray);
				GetObject(hOk_Gray,sizeof(BITMAP),&bit);
				bx = bit.bmWidth;
				by = bit.bmHeight;	
				BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
			}		
			
			//Draw_Text(menuPos[1].left,menuPos[1].top,szReport);
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,szUserJoin);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;

		case CLAN_JOIN:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off��ũ������.

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[5].left,menuPos[5].top,menuPos[5].right,menuPos[5].bottom,sorDC,0,0,SRCCOPY);

		if(menuMouseOver[0]>0)
		{
			SelectObject(sorDC, hYes_Yellow);

			GetObject(hYes_Yellow,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);			
		}
		else
		{
			SelectObject(sorDC, hYes_Gray);

			GetObject(hYes_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[0].left,menuPos[0].top,menuPos[0].right,menuPos[0].bottom,sorDC,0,0,SRCCOPY);
		}
		if(menuMouseOver[1]>0)
		{
			SelectObject(sorDC, hCancel_Yellow);

			GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC, hCancel_Gray);

			GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
		}
		
		
		//wsprintf(string,szJoin,cldata.szJoinClanName,cldata.szJoinClanChipName);
		wsprintf(string,clanJoin.szMsg,cldata.szJoinClanName,cldata.szJoinClanChipName);
		//DrawText(GetDC(g_hwnd),string,-1,&rect,DT_LEFT | DT_WORDBREAK);
		Text_Out(destdc,menuPos[3].left,menuPos[3].top,string);
	

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	}
#endif

}

void CLANEntity_JOIN::AddClanWon()
{
	int i;
	for(i =0; i < CLANWON_MAX-1; i++){	
		if(cldata.clanWon[i][0] == 0){
			strcpy(cldata.clanWon[i],cldata.clanJoinCharID);
			nClanWon_Num +=1;

			cldata.nTotalClanWonCount = nClanWon_Num ;
			cldata.nCurrentClanWon_Num +=1;
			break;
		}
	}




	//=============== ktj : �����
	void fd2( char *fmt, ...);

	for(i =0; i < CLANWON_MAX-1; i++){	
		if(cldata.clanWon[i][0] != 0){
			fd2("Ŭ��������Ʈ  cldata.clanWon[%d] = %s  ", i, cldata.clanWon[i]);
		}
	}

}




//========================================================================================Ŭ������
//���������� �����Ҽ� �ִ� Ŭ������ ���̺� �ʱ� ����
//
//makeclanlevel : ó�� Ŭ���� ����� �ִ� ���� 
//makeclanlevel_clanwon : ó�� Ŭ���� ���鶧 �����Ҽ� �ִ� �ʱ� Ŭ������
//addclanwonlevel_start : ������ �� �ִ� Ŭ���� ���� ó�� �����Ҷ� ����
//addlevel : �����ϴ� �����ܰ�
//addclanwon_num : �����ϴ� Ŭ���� �ܰ�
//maxlevel : �ִ� ����

//============================================================================
//ktj : ����50�̻���� Ŭ������Ⱑ���Ұ�
//============================================================================

//int nClanWonLevelTable[CLANWONLEVEL_TABLE] = {
int nClanWonLevelTable[] = {
		0,0,0,0,0,0,0,0,0,0, //0~9
		0,0,0,0,0,0,0,0,0,0, //10~19
		0,0,0,0,0,0,0,0,0,0, //20~29
		0,0,0,0,0,0,0,0,0,0, //30~39
		20,21,   //40~41
		22,23,   //42~43
		24,25,   //44~45
		26,27,   //46~47
		28,29,   //48~49
		30,31, //50~51
		32,33, //52~53
		34,35, //54~55
		36,37, //56~57
		38,39, //58~59
		40,41, //60~61
		42,43, //62~63
		44,45, //64~65
		46,47, //66~67
		48,49, //68~69
		50,51, //70,71
		52,53, //72,73
		54,55, //74,75
		56,57, //76,77
		58,59, //78,79
		60,61, //80,81
		62,63, //82.83
		64,65, //84,85
		66,67, //86,87
		68,69, //80~89
		70,71, //90,91
		72,73, //92,93
		74,75, //94,95
		76,77, //96,97
		78,80, //98~99

		/*
		2,2,2,2,2,2,2,2,2,2, //0~9
		2,2,2,2,2,2,2,2,2,2, //10~19
		2,2,2,2,2,2,2,2,2,2, //20~29
		2,2,2,2,2,2,2,2,2,2, //30~39
		2,2,2,2,2,2,2,2,2,2, //40~49
		2,2,2,2,2,2,2,2,2,2, //50~59
		2,2,2,2,2,2,2,2,2,2, //60~69
		3,//70
		4,//71
		5,//72
		6,//73
		7,//74
		8,//75
		9,//76
		10,//77
		11,//78
		12,//79
		20,20,20,20,20,20,20,20,20,20,//80~89
		2,20,20,20,20,20,20,20,20,20,//90~99
		*/
};

void CLANEntity_JOIN::SetLevelTable()
{	

	

	//ZeroMemory(cldata.nLevelTable,sizeof(cldata.nLevelTable));

	/*cldata.nLevelTable[0][0] = makeclanlevel;
	cldata.nLevelTable[0][1] = makeclanlevel_clanwon; 

	nMaxLevel = maxlevel - addclanwonlevel_start;
	nMaxLevel = (nMaxLevel / addlevel) + 1;

	for(cnt = 1; cnt <= nMaxLevel; cnt++)
	{
		cldata.nLevelTable[cnt][0] = addclanwonlevel_start + nAddLevel;
		nAddClanwon +=addclanwon_num;
		cldata.nLevelTable[cnt][1] = makeclanlevel_clanwon + nAddClanwon;
		nAddLevel += addlevel;
	}*/

}

BOOL CLANEntity_JOIN::CheckClanWon(int level)
{	
	int nMaxClanWonCount = 0;	
	nMaxClanWonCount = nClanWonLevelTable[level];
	if(nMaxClanWonCount > cldata.nCurrentClanWon_Num) {  //��ϰ���
	//if(nMaxClanWonCount > 15) {
		
		return TRUE;
	}


	/* 2003.8.5 ��Ŭ��Ĩ�� ���Ա����Ҷ� CLAN_CLANWONOVER�� �޼����� ���� ���� ����
	C_menuN2 = 0;
	OpenFlag = 1;
	ClanCLCharMove = 1;	
	C_menuN = CLAN_CLANWONOVER;
	cClanJoin.menuInit(C_menuN);
	*/

	return TRUE;
}


int CLANEntity_JOIN::GetMaxClanWonCount(int level)
{
	return nClanWonLevelTable[level];	
}

//���� ���̺� �ʱ�ȭ

//���������� �����Ҽ� �ִ� Ŭ������ ���̺� �ʱ� ����
//
//makeclanlevel : ó�� Ŭ���� ����� �ִ� ���� 
//makeclanlevel_clanwon : ó�� Ŭ���� ���鶧 �����Ҽ� �ִ� �ʱ� Ŭ������
//addclanwonlevel_start : ������ �� �ִ� Ŭ���� ���� ó�� �����Ҷ� ����
//addlevel : �����ϴ� �����ܰ�
//addclanwon_num : �����ϴ� Ŭ���� �ܰ�
//maxlevel : �ִ� ����

void SetLevelTable(int makeclanlevel,int makeclanlevel_clanwon,int addclanwonlevel_start, int addlevel, int addclanwon_num,int maxlevel)
{
	cClanJoin.SetLevelTable();
}

// 0 �̸� ���̻� Ŭ������ �߰� �Ҽ� ����(��ü�޴����� ��Ͻ�ų�����ٰ� ���´�)
BOOL CheckClanWon(int level)
{
	BOOL bReturn = 0;
	bReturn = cClanJoin.CheckClanWon(level);	
	return bReturn;
}

