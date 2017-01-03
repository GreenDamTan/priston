#define CE_CMAKE_CPP

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>

#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��

	#include "sinbaram\\SinLinkHeader.h"
	#include "HoBaram\\HoLinkHeader.h"
	#include "Language\\language.h"
	#include "Language\\jts.h"
	#include "HoBaram\\HoTextFile.h"
#endif

#include "tjclan.h"
#include "cE_Cmake.h"
#include "cE_report.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "..\\ygy\\ygyheader.h"
#include "CurseFilter.h"	//IsCurse �Լ��κ�(�ؿ�)

//HWND hFocusWnd�� ����ϰ� �ִ����� ���θ� �˾Ƴ�.
int usehFocus = 0;
int oldLIMITchattingWnd  = 0;
extern char	szConnServerName[16];	//������ �����̸��� ����ִ� ����
extern int g_ClanMakeMoney;


//*****************************************************************************
CLANEntity_make::CLANEntity_make()
{
	m_bEventClanMoney = false;
}
CLANEntity_make::~CLANEntity_make()
{

}


//*****************************************************************************
int CLANEntity_make::Init(int Life)
{
	life = Life;
	ZeroMemory(szClanMakeMsg,sizeof(szClanMakeMsg));
	return 1;
}

int CLANEntity_make::Clear()
{
	return 1;
}


//*****************************************************************************
void CLANEntity_make::Main()
{
	char szFile[256];
	char szFile2[256];
	char server;
	char UniqueClanMark[64];
	int  nMoney = 500000;
	if(life ==0) return;

	int i;
	switch(C_menuN)
	{
	case CLAN_ABILITYDOWN:
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
	case CLANMAKE_REAMMAKR_ERROR:
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
	case CLAN_GO_MAKE:
		C_menuN = CLAN_isCheckClanJang;
		menuInit(C_menuN);
		C_menuN2 = 1;
		break;
	case CLAN_isCheckClanJang_LOADING:
		if(ANSdata.ret_val !=-1) C_menuN = CLAN_isCheckClanJang;
		break;
	//========================================================================================//
	// Ŭ�� ����� �޴� START
	//========================================================================================//
	case CLAN_isCheckClanJang:			//ktj �� id�� Ŭ�������� ��ϵǾ����� ������츸 Ŭ���� ����������Ƿ�
		switch(C_menuN2) {				//    �װͿ� ���� üũ�� �Ѵ�.
		case 1:
			ANSdata.ret_val= -1;
			WebDB.Resume() ;		//��db�� �簳�Ѵ�.
			WebDB.isCheckClanJang(cldata.szID,cldata.gserver);
			C_menuN2=2;
			break;
		case 2:			
			//ktj : �ӽ� ����� Ŭ�������� ��ϵǾ��ִٰ� ��.
			//if(ANSdata.ret_val != -1) {
			//	ANSdata.ret_val = 1;
			//}


			switch(ANSdata.ret_val) 
			{
			case -1:
				//C_menuN = CLAN_isCheckClanJang_LOADING;
				//menuInit(C_menuN);
				break;
			case 1://Ŭ�������� ��ϵǾ� ����				
				WebDB.Suspend(); 
				C_menuN2=0;				
				//ygy : �� �츱��
				//wsprintf(szErrorMsg, szisCheckClanJangErrorMsg);
				C_menuN = CLAN_isCheckClanJang_Result;
				menuInit(C_menuN);
				//cClanReport.InitMsg(szisCheckClanJangErrorMsg);
				break;
			case 0://Ŭ�������� ��ϵǾ� ���� ����
				WebDB.Suspend();
				C_menuN2=0;
				C_menuN = CLAN_MAKE_is;
				menuInit(C_menuN);
				//=============================================================
				cldata.myPosition = CLANNO_REGESTER;
				ZeroMemory(cldata.name, sizeof(cldata.name)  );
				ZeroMemory(cldata.explanation, sizeof(cldata.explanation)  );
				ZeroMemory(cldata.ClanZang_Name, sizeof(cldata.ClanZang_Name)  );
				ZeroMemory(cldata.ClanMark, sizeof(cldata.ClanMark)  );
				cldata.intClanMark = 0;
				ZeroMemory(cldata.PFlag, sizeof(cldata.PFlag));
				//=============================================================
				//CLAN_ERROR_ret=CLAN_NAME_is;		//�����޴��������� �ٽ� ���ư���.
				break;			

			case 100:								//Ŭ���� ������� ����(Ŭ����ũ����޴���..)
				//fd2(" �����. ret_val== 100 ");
				WebDB.Suspend(); 
				C_menuN2=0;
				
				//ygy : �� �츱��
				wsprintf(szErrorMsg,"%s (1)", clanszError_Msg.szMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				//ygy : �� �츱��
				wsprintf(szErrorMsg,"%s (0)", clanszError_Msg.szMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				break;

			default:
				if(ANSdata.ret_val != -1) {		//���������� ���� �ͼ� ������ ����.
					WebDB.Suspend();
					C_menuN2=0;
					//ygy : �� �츱��
					wsprintf(szErrorMsg,"%s (2)", clanszError_Msg.szMsg);
					C_menuN = CLAN_ERROR;
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				}
				break;
			}
			break;
		}
		break;
	case CLANMAKE_NOMONEY:
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
	case CLAN_isCheckClanJang_Result:
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










										//================================================
	case CLAN_MAKE_is:					//Ŭ���Ἲ�ǻ� Ȯ�� Y/N
		i = chkeckMenuN();				//================================================
		switch(i) {
		case 0:		//yes
			menuMouseOver[0] = 1;
			break;
		case 1:		//no
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
		}
		//ygy : �� �츱��
		if(MOUSE0())
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				//ktj : ��� �����ǻ縦 �������� ����� ���� �ִ��� �˾Ƴ��� ��ƾ�� �־����.
				//if((cldata.money-MAKEMONEY) < 0) {	//���� ������ 255�� ����.				
				//--------------------------------------------------------------
				// 2005�� 12�� 20�� �ϴ�� ����
				// Dec : �űԼ��� Ŭ�� â�� ���
				//--------------------------------------------------------------
				//�ؿ� �ּ�
				/*
				if( m_bEventClanMoney == false ){
					g_ClanMakeMoney = 0;
				}*/
				if((cldata.money-g_ClanMakeMoney) < 0) {	//���� ������ 255�� ����.				
								//if((cldata.money-0) < 0) {	//���� ������ 255�� ����.				
					//ygy : �� �츱��
					//wsprintf(szErrorMsg, szMoneyErrorMsg );
					C_menuN = CLANMAKE_NOMONEY;
					menuInit(C_menuN);
					//menuInit(C_menuN);
					//cClanReport.InitMsg(szMoneyErrorMsg);
					
					CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
					break;
				}


				if(cldata.ability < ABILITY) {	//�ɷ�ġ�� �����Ͽ� ���� ó��
					//ygy : �� �츱��
					//wsprintf(szErrorMsg, szAbilityErrorMsg);
					C_menuN = CLAN_ABILITYDOWN;
					menuInit(C_menuN);
					CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
					break;
				}


				//ktj : �̺κ� �ٽ� ����:
				C_menuN = CLAN_MONEY_is;			//Y : Ŭ���Ἲ�ϰٴ�.  ��������ǻ�ΰ���.
				menuInit(C_menuN );
				
				break;

			case 1: //Ŭ�� ����⸦ ���
				//ygy : �� �츱��
				//wsprintf(szErrorMsg, szCancelMakeClan );
				C_menuN = CLAN_ERROR;			//���
				cClanReport.InitMsg(clanCancelMakeClan.szMsg);
				cClanReport.menuInit(C_menuN,clanCancelMakeClan.pos.x,clanCancelMakeClan.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				break;

			default:
				break;
			}
		}
		break;
										//================================================
	case CLAN_MONEY_is:					// ��������ǻ繯��
		i = chkeckMenuN();				//================================================
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		case 1:
			menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : �� �츱��
		if(MOUSE0())
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:

				//ktj : �� ���ҽ�Ű��. : ���⼭ ���ϰ� �� ��������� �Ѵ�.
				//int sinMinusMoney(int Money);
				//sinMinusMoney(500000);

				//�����ÿ� �ִ´�.
				//int sinPlusMoney(int Money);
				//sinPlusMoney(500000);

				C_menuN = CLAN_NAME_is;		//Y : ��������Ѵ�. Ŭ���̸��������� ����.
				keydownEnt=0;				//CLAN_NAME_is ���� ����Ű��뿩���ӿ�����
				menuInit(C_menuN );
				C_menuN2 = -1;
				break;

			case 1:
				//ygy : �� �츱��
				//wsprintf(szErrorMsg, szCancelMakeClan );
				
				C_menuN = CLAN_ERROR;			//���
				cClanReport.InitMsg(clanCancelMakeClan.szMsg);
				cClanReport.menuInit(C_menuN,clanCancelMakeClan.pos.x,clanCancelMakeClan.pos.y);
				//menuInit(C_menuN);
				CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				break;

			default:
				break;
			}
		}
		break;
										//================================================
	case CLAN_NAME_is:					//Ŭ���̸� ����.
										//================================================

		//START ==================== ���� : Ŭ���̸��� ������ �Ѱ��ְ� �´���Ȯ���ϴ·�ƾ���� Ȯ�ι�ư�� ������ ����Ǿ���
		switch(C_menuN2) {
		case 1:
			ANSdata.ret_val= -1;
			WebDB.Resume() ;		//��db�� �簳�Ѵ�.
			WebDB.isCheckClanName(cldata.name,cldata.gserver);
			C_menuN2=2;
			break;
		case 2:			

			switch(ANSdata.ret_val) 
			{
			case 0://Ŭ���̸��� �ߺ����� ����
				WebDB.Suspend();				
				C_menuN2=0;
				C_menuN = CLAN_MARK_expl;;
				menuInit(C_menuN);
				
				break;
			case 1://Ŭ���̸��� �ߺ���
				
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_NAME_BEING;
				menuInit(C_menuN);
				break;
			
			case 100:								//Ŭ���� ������� ����(Ŭ����ũ����޴���..)				
				WebDB.Suspend(); 
				C_menuN2=0;

				wsprintf(szErrorMsg,"%s (3)", clanszError_Msg.szMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);				
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.				
				
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				wsprintf(szErrorMsg,"%s (4)", clanszError_Msg.szMsg);

				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				
				break;

			default:
				if(ANSdata.ret_val != -1) {		//���������� ���� �ͼ� ������ ����.
					WebDB.Suspend();
					C_menuN2=0;
					wsprintf(szErrorMsg,"%s (5)", clanszError_Msg.szMsg);
					C_menuN = CLAN_ERROR;
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				}
				break;
			}
			break;
		}
		//END ==================== ���� : Ŭ���̸��� ������ �Ѱ��ְ� �´���Ȯ���ϴ·�ƾ���� Ȯ�ι�ư�� ������ ����Ǿ���.

		
		
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		//case 1:
		//	menuMouseOver[1] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}

		//ygy : Ŭ���̸� ������ ����Ű ���� �� �츱��
		if(keydownEnt==1) {
			keydownEnt=0;
			goto keydownenter;
		}

		//ygy : �� �츱��
		if(MOUSE0())			//���������
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:

keydownenter:
				//ktj : Ŭ���̸����� Ŭ���������� �������� Ȯ���ϰ� �ٽ� Ŭ���̸� �������� �ǵ��ƿ��� ��ƾ�� ����Ǿ���.
				//C_menuN = CLAN_MARK_expl;
				//menuInit(C_menuN);
				if(cldata.name[0] ==0)
				{
					#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
					SendMessage( hFocusWnd , EM_SETLIMITTEXT, oldLIMITchattingWnd, 0 );	//����78����
					hFocusWnd = 0;				//�Է¹޴ºκл���.
					usehFocus = 0;
					SetIME_Mode(0);

					#endif
					C_menuN = CLAN_NONAME;		//������ : �����°ɷΰ���.
					menuInit(C_menuN );
					break;
				}
				C_menuN2 =1;
				#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
				SendMessage( hFocusWnd , EM_SETLIMITTEXT, oldLIMITchattingWnd, 0 );	//����78����
				hFocusWnd = 0;				//�Է¹޴ºκл���.
				usehFocus = 0;
				SetIME_Mode(0);
				#endif
	
				break;
			/*case 1:
				wsprintf(szErrorMsg, szCancelMakeClan);
				C_menuN = CLAN_ERROR;			//���
				menuInit(C_menuN);
				break;*/

			default:
				break;
			}
		}
		break;
	case CLAN_NONAME:
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : �� �츱��
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				C_menuN = CLAN_NAME_is;		
				menuInit(C_menuN );
				//C_menuN2 =-1;
				//CLAN_ERROR_ret=-1;
				break;		
			}
		}
		break;
	case CLAN_NAME_BEING:
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : �� �츱��
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				C_menuN = CLAN_NAME_is;		//������ : �����°ɷΰ���.
				menuInit(C_menuN );

				//C_menuN2 =-1;
				//CLAN_ERROR_ret=-1;
				break;		
			}
		}
		break;
									//================================================
	case CLAN_MARK_expl:			//Ŭ����ũ����
									//================================================
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			
			break;
		}

		//ygy : �� �츱��
		if(MOUSE0())
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				C_menuN = CLAN_INSPECTION_expl;		//Ȯ�� : �ɻ�Ⱓ�������� ����.
				menuInit(C_menuN );
				break;				
			
			}
		}
		break;

									//================================================
	case CLAN_INSPECTION_expl:		//�ɻ�Ⱓ����
									//================================================
		//START ==================== ���� : Ŭ���̸��� ������ �Ѱ��ְ� �´���Ȯ���ϴ·�ƾ���� Ȯ�ι�ư�� ������ ����Ǿ���
		switch(C_menuN2) {
		case 1:
			ANSdata.ret_val= -1;
			WebDB.Resume() ;		//��db�� �簳�Ѵ�.
#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			WebDB.make_Clan(cldata.szID,cldata.gserver,cldata.ChaName,
							cldata.name,	//�Է� �޾ƿ� Ŭ�� �̸�.
							"�������.",sinChar->JOB_CODE,sinChar->Level);
#else
			WebDB.make_Clan(cldata.szID,cldata.gserver,cldata.ChaName,
							cldata.name,	//�Է� �޾ƿ� Ŭ�� �̸�.
							"�������.",1,50);//ĳ���� Ÿ��,����
#endif
			C_menuN2=2;
			break;
		case 2:
			//fd2("== ��ã���� %d ", ANSdata.ret_val);
			//if(ANSdata.ret_val != -1) {
			//	ANSdata.ret_val =  ANSdata.ret_val;
				//ANSdata.ret_val =  0;
			//}

			switch(ANSdata.ret_val) 
			{
			case 0://Ʈ����� ����
				WebDB.Suspend();
				//ygy : �� �츱��
				//wsprintf(szErrorMsg, szTranzationError);
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (6)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				//menuInit(C_menuN);
				CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				break;
			case 1://���� ����
				
				WebDB.Suspend(); 
				C_menuN2=0;
				cldata.myPosition = CLANCHIP;
				C_menuN = CLANMAKE_DOWN_INFO;
				menuInit(C_menuN);

				//ParsingMakeClanMoney(ANSdata.ret_Buf,&nMoney);
				//C_menuN = CLAN_MAKE_FINISH;
				//C_menuN = CLAN_MARK_expl;
              			
/*

#ifdef VOICE_CHAT
			   if(bCheckClan)
			   {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   cldata.name, 0, cldata.myPosition);
				  // bCheckClan = FALSE;

			   }
#endif

*/
				#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��

				//ktj : �� ���ҽ�Ű��. ( ���������)
				//int sinMinusMoney(int Money);
				int sinMinusMoney(int Money,int Kind=0);
				sinMinusMoney(g_ClanMakeMoney);
				

				//�����ÿ� �ִ´�.
				//int sinPlusMoney(int Money);
				//sinPlusMoney(500000);



				#endif






				break;
			case 2://ĳ���Ͱ� �̹� �ٸ� Ŭ���� �Ҽ�
				WebDB.Suspend();
				C_menuN2=0;
				//ygy : �� �츱��
				wsprintf(szErrorMsg,"%s (7)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_NAME_is;		//�����޴��������� �ٽ� ���ư���.
				break;
			case 3://Ŭ���̸� �ߺ�
				WebDB.Suspend();
				C_menuN2=0;
				//ygy : �� �츱��
				//wsprintf(szErrorMsg, szClanNameError);
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (8)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				//menuInit(C_menuN);
				CLAN_ERROR_ret=CLAN_NAME_is;		//�����޴��������� �ٽ� ���ư���.
				break;

			case 100:								//Ŭ���� ������� ����(Ŭ����ũ����޴���..)				
				WebDB.Suspend(); 
				C_menuN2=0;
				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (9)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();				
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg,"%s (10)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				
				break;
			default:
				if(ANSdata.ret_val != -1) {		//���������� ���� �ͼ� ������ ����.
					WebDB.Suspend();
					C_menuN2=0;					
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg,"%s (11)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
					CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				}
				break;
			}
			break;
		}
		//END ==================== ���� : Ŭ���̸��� ������ �Ѱ��ְ� �´���Ȯ���ϴ·�ƾ���� Ȯ�ι�ư�� ������ ����Ǿ���.
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		case 3:
			menuMouseOver[3] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : �� �츱��
		if(MOUSE0())
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				//C_menuN =CLAN_AllEnd;	//Ȯ�� : �����°ɷΰ���.
				//C_menuN = CLAN_MAKE_FINISH;
				C_menuN2 =1;
				break;	
			case 3:
				//ygy : �� �츱��
				//wsprintf(szErrorMsg, szCancelMakeClan);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanCancelMakeClan.szMsg);
				cClanReport.menuInit(C_menuN,clanCancelMakeClan.pos.x,clanCancelMakeClan.pos.y);
				CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������� �ٽ� ���ư���.
				//C_menuN =CLAN_AllEnd
				break;
			}
		}
		break;
									//================================================
	case CLAN_NotMake:				//Ŭ���Ἲ���ϴ� ���ư���
									//================================================
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : �� �츱��
		if(MOUSE0())			//���������
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				C_menuN =CLAN_AllEnd;	//Ȯ�� : �����°ɷΰ���.
				break;				
			}
		}
		break;
	case CLANMAKE_DOWN_INFO:
		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;			

			WebDB.Resume() ;		//��db�� �簳�Ѵ�.
			WebDB.isClanMember(cldata.szID,cldata.gserver,cldata.ChaName);		//�� id�� Ŭ���������� �ľ��Ѵ�.						
			C_menuN2=1;
			break;
		case 1:

			//if(ANSdata.ret_val != -1) 
			//	ANSdata.ret_val= ANSdata.ret_val;			

			switch(ANSdata.ret_val) 
			{
			case 0://�����   				
				WebDB.Suspend(); 
				bIsLoading = FALSE;				
				C_menuN=CLAN_AllEnd;				
				cldata.myPosition = CLANNO_REGESTER;				
				C_menuN2 = -1;
				CLAN_ERROR_ret=-1;
				
				break;


			case 1://Ŭ�������
				WebDB.Suspend(); //��db ����
				bIsLoading = FALSE;
				//ygy : �� �츱��
				ParsingIsClanMember(ANSdata.ret_Buf);				
				cldata.myPosition = CLANUSER;			//Ŭ���� ������ �ڵ�� 100����				
				C_menuN2=0;
				//C_menuN=UPDATE_CLANWON;
				//C_menuN=CLAN_AllEnd;

#ifdef VOICE_CHAT
				if(bCheckClan)
			    {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   cldata.name,(DWORD)atoi(cldata.ClanMark), cldata.myPosition);
				   //bCheckClan = FALSE;

			    }
#endif

				break;

			case 2://Ŭ������
				WebDB.Suspend(); //��db ����
				//bIsLoading = FALSE;
				//ygy : �� �츱��
				ParsingIsClanMember(ANSdata.ret_Buf);
				WriteMyClanInfo();
				C_menuN2= 0;			
				C_menuN = CLANMAKE_READ_CLANMARK;
				menuInit(C_menuN);
				//C_menuN=UPDATE_CLANWON;
				//C_menuN=CLAN_AllEnd;
				cldata.myPosition = CLANCHIP;				//Ŭ���� ������ �ڵ�� 101����
#ifdef VOICE_CHAT
				if(bCheckClan)
			    {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   cldata.name,(DWORD)cldata.intClanMark, cldata.myPosition);
				   //bCheckClan = FALSE;

			    }
#endif

				break;
			case 3: //Ŭ���� ��ü��
				WebDB.Suspend(); 
				ParsingIsClanMember2(ANSdata.ret_Buf);
				bIsLoading = FALSE;
				C_menuN2=0;
				C_menuN=CLAN_AllEnd;
				cldata.myPosition = CLANNO_REGESTER;
			
#ifdef VOICE_CHAT
				if(bCheckClan)
			    {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   NULL,0, cldata.myPosition);
				   //bCheckClan = FALSE;

			    }
#endif
				
				break;
			case 100:
				WebDB.Suspend();				
				cldata.myPosition = CLAN_NOCONNECT;
				C_menuN=CLAN_AllEnd;
				menuInit(C_menuN);
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();				
				cldata.myPosition = CLAN_NOCONNECT;
				C_menuN=CLAN_AllEnd;
				menuInit(C_menuN);
				
				break;
			default:
				if(ANSdata.ret_val != -1) {		//���������� ���� �ͼ� ������ ����.
					WebDB.Suspend();
					bIsLoading = FALSE;
					C_menuN2=0;
					C_menuN=CLAN_AllEnd;
					cldata.myPosition = CLAN_NOCONNECT;
					}
			}
		}
		break;
	//======================================================================
	case CLANMAKE_READ_CLANMARK:
	//======================================================================
		
		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;
			
			WebDB.Resume();		//��db�� �簳�Ѵ�.
			//WebDB.MarkREAD(cldata.ClanMark);
			WebDB.MarkREAD(cldata.name,cldata.ClanMark);
			C_menuN2=1;
			break;
		case 1:		

			if(ANSdata.ret_val != -1) 
				ANSdata.ret_val = ANSdata.ret_val;

			
			switch(ANSdata.ret_val) 
			{
				//ygy
			case 1://Ŭ�� ��ũ �б� ����
				WebDB.Suspend();
				//wsprintf(szFile,"image\\clanDATA\\%s.%s",cldata.name,cldata.ClanMark);
				server = cldata.ClanMark[0];
				strcpy(UniqueClanMark,&cldata.ClanMark[3]);
				wsprintf(szFile,"%s\\%s\\%c_%s.%s",SAVE_DIR,szConnServerName,server,UniqueClanMark,SAVE_EXE );
				wsprintf(szFile2,"%s\\%s\\%c_%s_16.%s",SAVE_DIR,szConnServerName,server,UniqueClanMark,SAVE_EXE);
				#ifdef  USE_PROSTONTALE
				if(cldata.hClanMark16) {
					cldata.hClanMark16->Release();
					cldata.hClanMark16 = NULL;
				}

				if(cldata.hClanMark){
					cldata.hClanMark->Release();
					cldata.hClanMark = NULL;
				}

				cldata.hClanMark16= LoadDibSurfaceOffscreen(szFile2);
				cldata.hClanMark= LoadDibSurfaceOffscreen(szFile);
				
				#else
				cldata.hClanMark16  = (HBITMAP)LoadImage(NULL,szFile2,
							IMAGE_BITMAP,
							0,0,
							LR_LOADFROMFILE);
				cldata.hClanMark = (HBITMAP)LoadImage(NULL,szFile,
							IMAGE_BITMAP,
							0,0,
							LR_LOADFROMFILE);
				#endif
				C_menuN = CLAN_MAKE_FINISH;
				menuInit(C_menuN);
				C_menuN2 = -1;
				/*if(cldata.myPosition == CLANUSER)			//Ŭ���� ������ �ڵ�� 100����)
				{
					C_menuN = CLAN_USER_ReadDATA;		
					C_menuN2=0;	
					if(strcmp(cldata.stats,"0") == 0)
					{
						C_menuN = CLAN_USER_NOMAKE;
						menuInit(C_menuN );									
						C_menuN2=0;
						//C_menuN2=-1;
					}
					else
					{
						C_menuN = CLAN_USER;
						menuInit(C_menuN );									
						C_menuN2=0;
						//C_menuN2=-1;
					}				
				}
				if(cldata.myPosition == CLANCHIP)
				{
					C_menuN = CLAN_CHIP_ReadDATA;		
					menuInit(C_menuN);
					C_menuN2=0;	
				}*/
				
				
				break;
			case 2://Ŭ�� ��ũ �б� ����
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;				//Ŭ���Ἲ���ϴ� ���ư���� �ϴ¸޴��� ����.
				wsprintf(szErrorMsg,"%s (65)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);
				
				//wsprintf(szClanMakeMsg, szReadClanMarkErrorMsg);
				//C_menuN = CLANMAKE_REAMMAKR_ERROR;
				//cClanReport.InitMsg(szReadClanMarkErrorMsg);
				//menuInit(C_menuN);
				//CLAN_ERROR_ret=CLAN_AllEnd;		//�����޴��������ı׳ɳ�����.			
				C_menuN2=-1;	
				break;	
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				//wsprintf(szErrorMsg,"%s (15)",szError_Msg);
				C_menuN = CLAN_ERROR;

				wsprintf(szErrorMsg,"%s (66)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);		
				cClanReport.menuInit(C_menuN,clanszError_Msg.pos.x,clanszError_Msg.pos.y);

				
				//cClanReport.InitMsg(clanServerConnectErrorMsg.szMsg);
				//cClanReport.menuInit(C_menuN,clanServerConnectErrorMsg.pos.x, clanServerConnectErrorMsg.pos.y);
				//wsprintf(szClanMakeMsg, clanServerConnectErrorMsg.szMsg);
				//C_menuN = CLANMAKE_REAMMAKR_ERROR;
				//cClanReport.InitMsg(szServerConnectErrorMsg);				
				//menuInit(C_menuN);
				C_menuN2=-1;	
				break;
			
			default:
				if(ANSdata.ret_val != -1) {		//���������� ���� �ͼ� ������ ����.
					WebDB.Suspend();

					C_menuN =CLAN_AllEnd;		//������ : �����°ɷΰ���.
					C_menuN2 = -1;
					CLAN_ERROR_ret=-1;
				}
				break;
			}
		}
			break;
	case CLAN_MAKE_FINISH:
		i = chkeckMenuN();
		switch(i) {
		case 0:
			menuMouseOver[0] = 1;
			break;
		default:
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}
		//ygy : �� �츱��
		if(MOUSE0())			
		{	
			MOUSE0()=0;
			switch(i) 
			{
			case 0:
				//C_menuN = UPDATE_ISCLANMEMBER;
				//C_menuN2 = 0;
				C_menuN =CLAN_AllEnd;		//������ : �����°ɷΰ���.
				C_menuN2 = -1;
				CLAN_ERROR_ret=-1;
				//menuInit(C_menuN );

				//C_menuN2 =-1;
				//CLAN_ERROR_ret=-1;
				break;		
			}
		}
		break;
	}
	
}



//*****************************************************************************
void CLANEntity_make::RenderMain()
{
	if(life ==0) return;

}




//***************************************************************************************
void CLANEntity_make::menuInit(int num)
{
	int bx = 0, by =0;	
	
	switch(num){
		case CLAN_ABILITYDOWN:
			menuSu = 3;
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else			
			bx = 64;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left = BackStartPos.x + clanAbilityErrorMsg.pos.x;
			menuPos[1].top = BackStartPos.y + clanAbilityErrorMsg.pos.y;

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
		case CLANMAKE_REAMMAKR_ERROR:
			menuSu = 3;
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else			
			bx = 64;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left = BackStartPos.x + 30;
			menuPos[1].top = BackStartPos.y + 40;

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
		case CLANMAKE_NOMONEY:
			menuSu = 3;
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else			
			bx = 64;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left = BackStartPos.x + clanMoneyErrorMsg.pos.x;
			menuPos[1].top = BackStartPos.y + clanMoneyErrorMsg.pos.y;

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
		case CLAN_isCheckClanJang_Result:
			menuSu = 3;
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else			
			bx = 64;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x + BUTTON1_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y + BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left = BackStartPos.x + 18;
			menuPos[1].top = BackStartPos.y + 40;

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
		case CLANMAKE_DOWN_INFO:
			menuPos[0].left = BackStartPos.x + clanClanMakedowninfo.pos.x;
			menuPos[0].top = BackStartPos.y + clanClanMakedowninfo.pos.y;
			menuPos[0].right = 0;
			menuPos[0].bottom = 0;
			break;
		case CLANMAKE_READ_CLANMARK:
			menuPos[0].left = BackStartPos.x + clanClanMakedowninfo.pos.x;
			menuPos[0].top = BackStartPos.y + clanClanMakedowninfo.pos.y;
			menuPos[0].right = 0;
			menuPos[0].bottom = 0;
			break;
		
		case CLAN_isCheckClanJang:
			//�ؽ�Ʈ ��� ��ġ
			menuPos[0].left = BackStartPos.x + clanLoading.pos.x;
			menuPos[0].top = BackStartPos.y + clanLoading.pos.y;
			menuPos[0].right = 0;
			menuPos[0].bottom = 0;
			break;
		case CLAN_MAKE_is:
			menuSu = 2;			

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif
			
			menuPos[0].left = BackStartPos.x + BUTTON2_X;//174;		//yes
			menuPos[0].top = BackStartPos.y + + BUTTON2_Y;//353;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;



			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else			
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x + BUTTON3_X;//254		//no
			menuPos[1].top = BackStartPos.y + BUTTON3_Y; //353
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[2].left = BackStartPos.x + clanMake1.pos.x;
			menuPos[2].top = BackStartPos.y + clanMake1.pos.y;
			menuPos[2].right = menuPos[2].left+280;
			menuPos[2].bottom = menuPos[2].top+250;

			//yes, no �׵θ� �ڽ�
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON2_BOX_X;//166;		//yes �׵θ� �ڽ�
			menuPos[3].top = BackStartPos.y + BUTTON2_BOX_Y;//348
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else			
			bx = 68;
			by = 27;
			#endif
			menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;	//246	//no �׵θ� �ڽ�
			menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;//348
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;

			break;
		case CLAN_MONEY_is:
			menuSu = 2;			
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else
			
			bx = 32;
			by = 16;
			#endif
			
			menuPos[0].left = BackStartPos.x + BUTTON2_X;		//yes
			menuPos[0].top = BackStartPos.y + BUTTON2_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;



			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else
			
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x + BUTTON3_X;		//no
			menuPos[1].top = BackStartPos.y + BUTTON3_Y;
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[2].left = BackStartPos.x + clanMake2.pos.x;
			menuPos[2].top =  BackStartPos.y + clanMake2.pos.y;
			menuPos[2].right = menuPos[2].left+280;
			menuPos[2].bottom = menuPos[2].top+250;

			//yes, no �׵θ� �ڽ�
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON2_BOX_X;		//yes �׵θ� �ڽ�
			menuPos[3].top = BackStartPos.y + BUTTON2_BOX_Y;
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;		//no �׵θ� �ڽ�
			menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;

			break;
		case CLAN_NAME_is: //Ŭ�� ��Ī

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			//ktj : �̸��� �Է��ϱ� ���� �Է�����, �Է¹��� ����.
			if(hFocusWnd==0) {
				ZeroMemory(cldata.name, sizeof(cldata.name));
				hFocusWnd = hTextWnd;
				SetWindowText( hFocusWnd , "" );

				oldLIMITchattingWnd = SendMessage( hFocusWnd , EM_GETLIMITTEXT, 0, 0 );	//�������� �о�״ٻ����.(78����)
				SendMessage( hFocusWnd , EM_SETLIMITTEXT, 12, 0 );
				usehFocus = 1;
#ifdef	_LANGUAGE_JAPANESE		//C7
				SetIME_Mode( 1 );
#endif	
			}
			#endif

			menuSu = 2;
			
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif

			menuPos[0].left = BackStartPos.x +BUTTON1_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y +BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;


		/*  #ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else
			GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
			bx = 32;
			by = 16;
			#endif

			menuPos[1].left = BackStartPos.x +BUTTON2_X;		//���
			menuPos[1].top = BackStartPos.y +BUTTON2_Y;
			menuPos[1].right = menuPos[1].left+ bx;
			menuPos[1].bottom = menuPos[1].top + by;
			*/

			//�ؽ�Ʈ ��� ��ġ
			menuPos[2].left	= BackStartPos.x + 60;
			menuPos[2].top		= BackStartPos.y + 40;
			menuPos[2].right	=menuPos[2].left + 190;
			menuPos[2].bottom	=menuPos[2].top +	180;

			//Ȯ��, ��� �׵θ� �ڽ�
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[3].left = BackStartPos.x + BUTTON1_BOX_X;		//Ȯ�� �׵θ� �ڽ�
			menuPos[3].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else
			
			bx = 68;
			by = 27;
			#endif

			menuPos[4].left = BackStartPos.x + CLANINPUTNAME_X;		//Ŭ�� �̸� �Է�
			menuPos[4].top = BackStartPos.y + CLANINPUTNAME_Y;
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;
			
			
			break;
		case CLAN_NONAME:
			//menuSu = 3;			
			menuSu = 1;
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
			menuPos[1].left = BackStartPos.x + clanNoName.pos.x;
			menuPos[1].top = BackStartPos.y + clanNoName.pos.y;

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
		case CLAN_NAME_BEING:
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
			menuPos[1].left = BackStartPos.x + clanClanNameError.pos.x;
			menuPos[1].top = BackStartPos.y + clanClanNameError.pos.y;

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
		case CLAN_MARK_expl: //Ŭ�� ��ũ Ȯ��
			menuSu = 1;
			
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else
			bx = 32;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x +BUTTON1_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y +BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left	= BackStartPos.x + clanMake4.pos.x;
			menuPos[1].top		= BackStartPos.y + clanMake4.pos.y;
			menuPos[1].right	=menuPos[1].left + 190;
			menuPos[1].bottom	=menuPos[1].top +180;

			//Ȯ�� �׵θ�
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//Ȯ�� �׵θ� �ڽ�
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;
			break;
		case CLAN_INSPECTION_expl:		//Ŭ���ɻ�
			menuSu = 5;
			
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else

			bx = 32;
			by = 16;
			#endif
			menuPos[0].left = BackStartPos.x +BUTTON2_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y +BUTTON2_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left	= BackStartPos.x + clanMake5.pos.x;
			menuPos[1].top		= BackStartPos.y + clanMake5.pos.y;
			menuPos[1].right	=menuPos[1].left + 260;
			menuPos[1].bottom	=menuPos[1].top + 180;

			//Ȯ�� �׵θ�
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[2].left = BackStartPos.x + BUTTON2_BOX_X;		//Ȯ�� �׵θ� �ڽ�
			menuPos[2].top = BackStartPos.y + BUTTON2_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			#else

			bx = 32;
			by = 16;
			#endif
			menuPos[3].left = BackStartPos.x +BUTTON3_X;		//���
			menuPos[3].top = BackStartPos.y +BUTTON3_Y;
			menuPos[3].right = menuPos[3].left + bx;
			menuPos[3].bottom = menuPos[3].top + by;

			//��� �׵θ�
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;		//��� �׵θ� �ڽ�
			menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;
			break;
		case CLAN_NotMake:				//Ŭ���� ������ ���Ѵٴ� �޴�
			menuSu = 3;
			
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 64;
			#else
			bx = 64;
			by = 64;
			#endif
			menuPos[0].left = BackStartPos.x +BUTTON1_X;		//Ȯ��
			menuPos[0].top = BackStartPos.y +BUTTON1_Y;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left	= BackStartPos.x + clanMsg254.pos.x;
			menuPos[1].top		= BackStartPos.y + clanMsg254.pos.y;
			menuPos[1].right	=menuPos[1].left + 260;
			menuPos[1].bottom	=menuPos[1].top + 180;

			//Ȯ�� �׵θ�
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 68;
			by = 27;
			#else
			bx = 68;
			by = 27;
			#endif

			menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;		//Ȯ�� �׵θ� �ڽ�
			menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;
			break;
		case CLAN_MAKE_FINISH:
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
			menuPos[1].left = BackStartPos.x + clanMake6.pos.x;
			menuPos[1].top = BackStartPos.y + clanMake6.pos.y;

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
		//======================= Ŭ�� ������ �޴� ======================end//
			
	}
}

void CLANEntity_make::Draw()
{
int i =0,nIconCount=0;
	int nIconPos = 0;

	int nSaveTop = 0,nAdd = 0;
	int nStrLen = 0;
	char szDrawMenuMsg[2024];
	ZeroMemory(szDrawMenuMsg,sizeof(char)*2024);

#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
#else
	HDC hdc;
	HDC sorDC;
	BITMAP bit;
	int bx,by;
	
	RECT rect;	
	int j =0;
	
//	HFONT font, oldfont;

	char string[500];


#endif

	
#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
	switch(C_menuN)
	{	
	case CLAN_ABILITYDOWN:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(clanAbilityErrorMsg.szMsg, menuPos[1].left, menuPos[1].top);		
		break;
	case CLANMAKE_REAMMAKR_ERROR:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(szClanMakeMsg, menuPos[1].left, menuPos[1].top);		
		break;

	case CLANMAKE_NOMONEY:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		wsprintf(szDrawMenuMsg,clanMoneyErrorMsg.szMsg,g_ClanMakeMoney);
		Draw_C_Text(szDrawMenuMsg, menuPos[1].left, menuPos[1].top);		
		break;
	case CLAN_isCheckClanJang_Result:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(clanisCheckClanJangErrorMsg.szMsg, menuPos[1].left, menuPos[1].top);		
		break;
	case CLAN_isCheckClanJang:
		Draw_C_Text(clanLoading.szMsg, menuPos[0].left, menuPos[0].top);
		break;
	case 0:		//Ŭ���Ἲ�ǻ� Ȯ���� ���� ��ƾ���ư�.
		break;
	case CLAN_MAKE_is:		//Ŭ���Ἲ�ǻ� Ȯ�� Y/N : �׸�	
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hYes_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[1]>0)	DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[1].left,menuPos[1].top, hNo_Gray,  0, 0, 32, 16, 1);
		Draw_C_Text(clanMake1.szMsg, menuPos[2].left, menuPos[2].top);
		
		break;

	case CLAN_MONEY_is:		// ��������ǻ繯��
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[1]>0)	DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Gray,  0, 0, 32, 16, 1);

		wsprintf(szDrawMenuMsg,clanMake2.szMsg,g_ClanMakeMoney);
		Draw_C_Text(szDrawMenuMsg, menuPos[2].left, menuPos[2].top);
		break;

	case CLAN_NAME_is:		//Ŭ���̸� ����.
		DrawSprite(menuPos[3].left,menuPos[3].top, hButton_Box,  0, 0, 48, 23, 1);
	//	DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 27, 1);
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
	//	if(menuMouseOver[1]>0)	DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Yellow,  0, 0, 32, 16, 1);
	//	else					DrawSprite(menuPos[1].left,menuPos[1].top, hCancel_Gray,  0, 0, 32, 16, 1);
		Draw_C_Text(szMsg3 , menuPos[2].left, menuPos[2].top);



		//ktj : OK�ߵȴ�.
		if(hFocusWnd != 0) {
			GetWindowText( hFocusWnd , cldata.name, 64 );
			
			void KeyFilter(char *szBuffer);
			KeyFilter(cldata.name);

			//Draw_C_Text(cldata.name, 65, 200, 1);
			//DrawSprite(BackStartPos.x+160-50,BackStartPos.y+196, hClanNameInput,  0, 0, 100, 20, 1);
			DrawSprite(menuPos[4].left, menuPos[4].top, hClanNameInput,  0, 0, 100, 20, 1);
			//Draw_C_Text(cldata.name, BackStartPos.x+160-50+1, BackStartPos.y+200, 1);	//Ŭ���̸��Է¹ޱ����� 1��)
			Draw_C_Text(cldata.name, menuPos[4].left+1, menuPos[4].top+4, 1);	//Ŭ���̸��Է¹ޱ����� 1��)
		}
		break;
	case CLAN_NONAME:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(clanNoName.szMsg, menuPos[1].left, menuPos[1].top);
		break;
	case CLAN_NAME_BEING:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		//Draw_C_Text(szClanNameError, menuPos[1].left, menuPos[1].top);clanClanNameError.szMsg
		Draw_C_Text(clanClanNameError.szMsg, menuPos[1].left, menuPos[1].top);
		break;
	case CLAN_MARK_expl:		//Ŭ����ũ����
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		Draw_C_Text(clanMake4.szMsg , menuPos[1].left, menuPos[1].top);
		
		break;
	case CLAN_INSPECTION_expl:		//�ɻ�Ⱓ����
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		DrawSprite(menuPos[4].left,menuPos[4].top, hButton_Box,  0, 0, 48, 23, 1);
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		if(menuMouseOver[3]>0)	DrawSprite(menuPos[3].left,menuPos[3].top, hCancel_Yellow,  0, 0, 32, 16, 1);
		else					DrawSprite(menuPos[3].left,menuPos[3].top, hCancel_Gray,  0, 0, 32, 16, 1);
		wsprintf(szDrawMenuMsg,clanMake5.szMsg,cldata.name);
		Draw_C_Text(szDrawMenuMsg,menuPos[1].left, menuPos[1].top);
	
		break;

	case CLAN_NotMake:	//Ŭ���Ἲ���ϴ� ���ư���
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0)	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);		
		else					DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);		
		//Draw_C_Text(szMsg254 , menuPos[1].left, menuPos[1].top);
		Draw_C_Text(clanMsg254.szMsg, menuPos[1].left, menuPos[1].top);
		
		
		break;
	case CLAN_MAKE_FINISH:
		DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);
		
		Draw_C_Text(clanMake6.szMsg, menuPos[1].left, menuPos[1].top);
		break;
	case CLANMAKE_DOWN_INFO:
		Draw_C_Text(clanClanMakedowninfo.szMsg, menuPos[0].left, menuPos[0].top);
		break;
	case CLANMAKE_READ_CLANMARK:
		Draw_C_Text(clanClanMakedowninfo.szMsg, menuPos[0].left, menuPos[0].top);
		break;
	
	}
#else
	

	HDC destdc;
	
	switch(C_menuN) {
		case CLAN_ABILITYDOWN:
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
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanAbilityErrorMsg.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
		case CLANMAKE_NOMONEY:
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
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanMoneyErrorMsg.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );

			break;
		case CLAN_isCheckClanJang_Result:
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
			Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanisCheckClanJangErrorMsg.szMsg);

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );

			break;
		case CLANMAKE_DOWN_INFO:
		case CLANMAKE_READ_CLANMARK:
			hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);

		destdc  = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off��ũ������.				
		Text_Out(destdc,menuPos[0].left,menuPos[0].top,clanClanMakedowninfo.szMsg);
			DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
			break;
	case CLAN_isCheckClanJang:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);

		destdc  = CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off��ũ������.				
		
		
		//dsTextLineOut(hdc,menuPos[1].left,menuPos[1].left,szMsg6,lstrlen(szMsg6));
		//Draw_Text(menuPos[1].left,menuPos[1].top,szMsg6);
		Text_Out(destdc,menuPos[0].left,menuPos[0].top,clanLoading.szMsg);

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	case 0:		//Ŭ���Ἲ�ǻ� Ȯ���� ���� ��ƾ���ư�.

		break;
	case CLAN_MAKE_is:		//Ŭ���Ἲ�ǻ� Ȯ�� Y/N : �׸�		
	
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc=CreateCompatibleDC(hdc);

		SelectObject(destdc, Dback.hb );		//off��ũ������.

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
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
			SelectObject(sorDC, hNo_Yellow);

			GetObject(hNo_Yellow,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC, hNo_Gray);

			GetObject(hNo_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			
			BitBlt(destdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
		}

		wsprintf(string,"%s",szMsg1 );
		rect.left = menuPos[2].left;
		rect.top  = menuPos[2].top;
		rect.right = menuPos[2].right;
		rect.bottom =menuPos[2].bottom;
		//DrawText(GetDC(g_hwnd),string,-1,&rect,DT_LEFT | DT_WORDBREAK);
		//Draw_Text(menuPos[2].left,menuPos[2].top,string);
		Text_Out(destdc,menuPos[2].left,menuPos[2].top,clanMake1.szMsg);

		
		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		

		break;

	case CLAN_MONEY_is:		// ��������ǻ繯��		
		if(m_hClanName)
		{
			ShowWindow(m_hClanName,SW_HIDE);
		}
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc=CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off��ũ������.


		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

		if(menuMouseOver[0]>0)
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
		if(menuMouseOver[1]>0)
		{
			SelectObject(sorDC, hCancel_Yellow);

			GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
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
		
		wsprintf(string,"%s",szMsg2 );
		rect.left = menuPos[2].left;
		rect.top  = menuPos[2].top;
		rect.right = menuPos[2].right;
		rect.bottom =menuPos[2].bottom;		
		//DrawText(GetDC(g_hwnd),string,-1,&rect,DT_LEFT | DT_WORDBREAK);
		//Draw_Text(menuPos[2].left,menuPos[2].top,string);
		Text_Out(destdc,menuPos[2].left,menuPos[2].top,clanMake2.szMsg);
		
		
		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );

		break;
	case CLAN_NAME_is:		//Ŭ���̸� ����.
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		sorDC  = CreateCompatibleDC(hdc);
		destdc=CreateCompatibleDC(hdc);		
		SelectObject(destdc, Dback.hb );		//off��ũ������.
		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

		if(menuMouseOver[0] >0)
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
	/*	if(menuMouseOver[1] >0)
		{
			SelectObject(sorDC, hCancel_Yellow);
			GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			BitBlt(hdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC, hCancel_Gray);
			GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			BitBlt(hdc,menuPos[1].left,menuPos[1].top,menuPos[1].right,menuPos[1].bottom,sorDC,0,0,SRCCOPY);
		}	
		*/

		SelectObject(sorDC, hClanNameInput);

		GetObject(hClanNameInput,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
	
		
		BitBlt(destdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);

		wsprintf(string,"%s",szMsg3 );
		
		//Draw_Text(menuPos[2].left,menuPos[2].top,string);
		Text_Out(destdc,menuPos[2].left,menuPos[2].top,string);
		ShowWindow(m_hClanName,SW_SHOW);

		
		GetWindowText( m_hClanName, cldata.name, 64 );

		//SelectObject(sorDC,hButton_Box);
		//GetObject(hButton_Box,sizeof(BITMAP),&bit);
		//bx = bit.bmWidth;
		//by = bit.bmHeight;		
		//BitBlt(hdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
		DeleteDC(sorDC);
		DeleteDC(destdc);

		ReleaseDC(g_hwnd, hdc );
		
		break;
	case CLAN_NONAME:
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);				
		destdc=CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off��ũ������.
		
		SelectObject(sorDC,hButton_Box);		
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
		
		
		//Draw_Text(menuPos[1].left,menuPos[1].top,szNoName );
		Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanNoName.szMsg);

		
		

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
		
	case CLAN_NAME_BEING:
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
		
		
		
		//Draw_Text(menuPos[1].left,menuPos[1].top,szClanNameError);
		Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanClanNameError.szMsg);		

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
		
	case CLAN_MARK_expl:		//Ŭ����ũ����
		if(m_hClanName)
		{
			ShowWindow(m_hClanName,SW_HIDE);
		}
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc=CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off��ũ������.

		SelectObject(sorDC,hButton_Box);
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
		
		
		
		//Draw_Text(menuPos[1].left,menuPos[1].top,string);
		Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanMake4.szMsg);

		
		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	case CLAN_INSPECTION_expl:		//�ɻ�Ⱓ����
		if(m_hClanName)
		{
			ShowWindow(m_hClanName,SW_HIDE);
		}
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc=CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off��ũ������.

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(destdc,menuPos[2].left,menuPos[2].top,menuPos[2].right,menuPos[2].bottom,sorDC,0,0,SRCCOPY);

		SelectObject(sorDC,hButton_Box);
		GetObject(hButton_Box,sizeof(BITMAP),&bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;		
		BitBlt(hdc,menuPos[4].left,menuPos[4].top,menuPos[4].right,menuPos[4].bottom,sorDC,0,0,SRCCOPY);
			
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

		if(menuMouseOver[3] > 0)
		{
			SelectObject(sorDC, hCancel_Yellow);

			GetObject(hCancel_Yellow,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
		
			
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}
		else
		{
			SelectObject(sorDC, hCancel_Gray);

			GetObject(hCancel_Gray,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
		
			
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);
		}
			
		
		wsprintf(szDrawMenuMsg,clanMake5.szMsg,cldata.name);		
		//Draw_Text(menuPos[1].left,menuPos[1].top,szDrawMenuMsg);
		Text_Out(destdc,menuPos[1].left,menuPos[1].top,szDrawMenuMsg);

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		
		break;
	case CLAN_MAKE_FINISH:
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
		
		
		//Draw_Text(menuPos[1].left,menuPos[1].top,szClanMakeFinishMsg);
		Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanMake6.szMsg);

		
		

		DeleteDC(sorDC);
		DeleteDC(destdc);
		ReleaseDC(g_hwnd, hdc );
		break;
	case CLAN_NotMake:	//Ŭ���Ἲ���ϴ� ���ư���
		hdc = GetDC(g_hwnd);
		sorDC  = CreateCompatibleDC(hdc);
		destdc=CreateCompatibleDC(hdc);
		SelectObject(destdc, Dback.hb );		//off��ũ������.

		SelectObject(sorDC,hButton_Box);
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
			
		
		wsprintf(string,"%s",szMsg254 );
		
		//Draw_Text(menuPos[1].left,menuPos[1].top,string);
		Text_Out(destdc,menuPos[1].left,menuPos[1].top,clanMsg254.szMsg);


		DeleteDC(sorDC);
		ReleaseDC(g_hwnd, hdc );
		
		break;
	case CLAN_AllEnd:	//Ŭ�� ������ Ŭ�����ϴ¼��úκ�.
		break;
		}
#endif
}




void KeyFilter(char *szBuffer)
{
	#ifdef  USE_PROSTONTALE
	int start;
	int result=0;
	
	if(hFocusWnd != NULL && hTextWnd != NULL)
	{
		//KeyFilter
		for(int index = 0; index < (int)lstrlen(szBuffer); index++)
		{
#ifdef _LANGUAGE_JAPANESE
			if(index>0)
			{
				if((((szBuffer[index-1]&0xff) >= 0x81) && ((szBuffer[index-1]&0xff) <= 0x9F))
					||  (((szBuffer[index-1]&0xff) >= 0xE0) && ((szBuffer[index-1]&0xff) <= 0xEF)))
				{
						index = index;
				}
				else if( (szBuffer[index] >= 0x1f && szBuffer[index] <= 0x2f) ||
					(szBuffer[index] >= 0x3a && szBuffer[index] <= 0x40) ||
					(szBuffer[index] == 0x5c ) ||
					(szBuffer[index] == 0x5e ) ||
					(szBuffer[index] == 0x5f ) ||
					(szBuffer[index] == 0x60 ) ||
					(szBuffer[index] >= 0x7b && szBuffer[index] <= 0x7f) ) 					
				{
					szBuffer[index] = 0;
					SetWindowText(hFocusWnd, 0);
					SetWindowText(hFocusWnd, szBuffer);
					SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
					break;
				}
			}
			else
			{
				if( (szBuffer[index] >= 0x1f && szBuffer[index] <= 0x2f) ||
					(szBuffer[index] >= 0x3a && szBuffer[index] <= 0x40) ||
					(szBuffer[index] == 0x5c ) ||
					(szBuffer[index] == 0x5e ) ||
					(szBuffer[index] == 0x5f ) ||
					(szBuffer[index] == 0x60 ) ||
					(szBuffer[index] >= 0x7b && szBuffer[index] <= 0x7f) )
				{
					szBuffer[index] = 0;
					SetWindowText(hFocusWnd, 0);
					SetWindowText(hFocusWnd, szBuffer);
					SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
					break;
				}
			}
#else
			if( (szBuffer[index] < 48 && szBuffer[index] > 0) ||
				(szBuffer[index] >= 58 && szBuffer[index] <= 64) ||
				(szBuffer[index] >= 91 && szBuffer[index] <= 96) ||
 				(szBuffer[index] >= 123 && szBuffer[index] <= 127) )
			{
				szBuffer[index] = 0;
				SetWindowText(hFocusWnd, 0);
				SetWindowText(hFocusWnd, szBuffer);
				SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
				break;
			}
#endif

			/*
			if(szBuffer[index] == '@' ||
			   szBuffer[index] == '~' ||
			   szBuffer[index] == '`' ||
			   szBuffer[index] == '!' ||
			   szBuffer[index] == '#' ||
			   szBuffer[index] == '$' ||
			   szBuffer[index] == '%' ||
			   szBuffer[index] == '&' ||
			   szBuffer[index] == '*' ||
			   szBuffer[index] == '+' ||
			   szBuffer[index] == '=' ||
			   szBuffer[index] == '|' ||
			   szBuffer[index] == '\\'||
			   szBuffer[index] == '{' ||
			   szBuffer[index] == '}' ||
			   szBuffer[index] == ':' ||
			   szBuffer[index] == ';' ||
			   szBuffer[index] == '"' ||
			   szBuffer[index] == '\'' ||
			   szBuffer[index] == ',' ||
			   szBuffer[index] == '.' ||
			   szBuffer[index] == '?' ||
			   szBuffer[index] == '/' ||
			   szBuffer[index] == '\t' ||
			   szBuffer[index] == ' ' ||
			   szBuffer[index] == '<' ||
			   szBuffer[index] == '>' ||
			   szBuffer[index] == '|')
			{
				szBuffer[index] = 0;
				SetWindowText(hFocusWnd, 0);
				SetWindowText(hFocusWnd, szBuffer);
				SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
				break;
			}
			*/

		}
		
		if(lstrlen(szBuffer) >= 1)
		{
			for( int index = 0; index < (int)lstrlen(szBuffer); index++)
			{

				//�ѱ��� ���..
				if((BYTE)szBuffer[index] >= 0x80)
				{

#ifdef _LANGUAGE_CHINESE

					//�߱��� ���� ( GB2312 )
					int fstByte = (BYTE)szBuffer[index]; 
					int sndByte = (BYTE)szBuffer[index+1]; 

					if ( fstByte<0xA1 || fstByte>0xF7 || sndByte<0xA1 || sndByte>0xFE ) //2����Ʈ�ε� �߱����ڵ� �ƴ�
					{
						szBuffer[index] = 0;
						SetWindowText(hFocusWnd, 0);
						SetWindowText(hFocusWnd, szBuffer);
						SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
						break;
					}
					else
						index++;

#endif

#ifdef _LANGUAGE_KOREAN
					//�ѱ� ����
					int fstByte = (BYTE)szBuffer[index]; 
					int sndByte = (BYTE)szBuffer[index+1]; 

					result = (fstByte-0xb0)*94 + sndByte-0xa1; 

					if (result < 0 || result >= 2350) //2����Ʈ�ε� �ѱ۾ƴ�.
					{
						szBuffer[index] = 0;
						SetWindowText(hFocusWnd, 0);
						SetWindowText(hFocusWnd, szBuffer);
						SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
						break;
					}
					else
						index++;
#endif

				}
			}
		}

#ifdef _LANGUAGE_JAPANESE
		//IsJTS ( �Ϻ� ���� )
		result = IsJTS(szBuffer);
		if ( result>=0 ) {
			szBuffer[result] = 0;

			SetWindowText(hFocusWnd, 0);
			SetWindowText(hFocusWnd, szBuffer);
			SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
		}

#endif
#ifdef _LANGUAGE_THAI
		result = IsTHAI(szBuffer);
		if ((result>=0)&&(IsCurse(szBuffer)==0))
		{
			szBuffer[result] = 0;

			SetWindowText(hFocusWnd, 0);
			SetWindowText(hFocusWnd, szBuffer);
			SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
		}
		else if(IsCurse(szBuffer)!=0)
		{
			SetWindowText(hFocusWnd,"");
			ZeroMemory(szBuffer,sizeof(char)*20);
		}
#endif
//���� ���� ó���� �־�� �Ѵ�.
#ifdef _LANGUAGE_ENGLISH
		result = IsENG(szBuffer);
		if (result>=0)
		{
			szBuffer[result] = 0;

			SetWindowText(hFocusWnd, 0);
			SetWindowText(hFocusWnd, szBuffer);
			SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
		}
		/*
		else 
		{
			SetWindowText(hFocusWnd,"");
			ZeroMemory(szBuffer,sizeof(char)*256);
		}*/

#endif
#ifdef _LANGUAGE_TAIWAN
		//�����߱� ���� ( ��ü )
		result = IsTaiwan(szBuffer);
		if ( result>=0 ) {
			szBuffer[result] = 0;

			SetWindowText(hFocusWnd, 0);
			SetWindowText(hFocusWnd, szBuffer);
			SendMessage(hFocusWnd, EM_SETSEL, lstrlen(szBuffer), lstrlen(szBuffer)+1);
		}
#endif

		SendMessage(hFocusWnd, EM_GETSEL, (WPARAM)&start, NULL);
	}

#endif
}