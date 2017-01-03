
#define CE_ISCLANMEMBER_CPP
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
#include "cE_isclanmember.h"
#include "tjscroll.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "..\\ygy\\ygyheader.h"

extern char	szConnServerName[16];	//������ �����̸��� ����ִ� ����

//*****************************************************************************
CLANEntity_IsClanMember::CLANEntity_IsClanMember()
{

}

CLANEntity_IsClanMember::~CLANEntity_IsClanMember()
{

}


//*****************************************************************************
int CLANEntity_IsClanMember::Init(int Life)
{
	life = Life;
	return 1;
}

int CLANEntity_IsClanMember::Clear()
{
	return 1;
}


//*****************************************************************************
extern BOOL bGetChaClan;
//extern int  nGetChaClan;   //0: ����ȵ� 1: ����ǰ����� 2: ����Ϸ�  3: ����
extern int nStatusCount;
//Ŭ�������� Ŭ�� ������ ������
extern BOOL bCheckClan;  
void CLANEntity_IsClanMember::Main()
{
	if(life ==0) return;

	switch(C_menuN)
	{
	case CLAN_isClanWon:

		switch(C_menuN2) {
		case 0:
			ANSdata.ret_val= -1;			

			WebDB.Resume() ;		//��db�� �簳�Ѵ�.
			WebDB.isClanMember(cldata.szID,cldata.gserver,cldata.ChaName);		//�� id�� Ŭ���������� �ľ��Ѵ�.						
			C_menuN2=1;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� WebDB.isClanMember(���̵� : %s,���Ӽ��� : %s,ĳ�����̸� : %s)",cldata.szID,cldata.gserver,cldata.ChaName);
#endif
			break;
		case 1:

			if(ANSdata.ret_val != -1) 
				ANSdata.ret_val= ANSdata.ret_val;			

			switch(ANSdata.ret_val) 
			{
			case 0://�����   				
				WebDB.Suspend(); 
				if(bGetChaClan) ++nStatusCount;
				
				bIsLoading = FALSE;
				C_menuN2=0;
				C_menuN=CLAN_AllEnd;
				cldata.myPosition = CLANNO_REGESTER;
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� �����");
#endif     
#ifdef VOICE_CHAT
			   if(bCheckClan)
			   {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   NULL,0, cldata.myPosition);
				   //bCheckClan = FALSE;

			   }
#endif

				
				break;


			case 1://Ŭ�������
				WebDB.Suspend(); //��db ����
				bIsLoading = FALSE;
				//ygy : �� �츱��
				cldata.myPosition = CLANUSER;			//Ŭ���� ������ �ڵ�� 100����
				ParsingIsClanMember(ANSdata.ret_Buf);
				if(bGetChaClan)
				{
					if(CheckMark(bGetChaClan) ==1)
					{
						C_menuN2=0;
						C_menuN=CLAN_AllEnd;
						break;
					}
					else
					{
						WriteMyClanInfo();
					
						
					
						C_menuN2=0;
						C_menuN=MYCLAN_UPDATE;
					
					}
				}
				else
				{
						WriteMyClanInfo();				
						C_menuN2=0;
						C_menuN=MYCLAN_UPDATE;
				}
#ifdef VOICE_CHAT
				if(bCheckClan)
			    {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   cldata.name,(DWORD)atoi(cldata.ClanMark), cldata.myPosition);
				   //bCheckClan = FALSE;

			    }
#endif
				
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� Ŭ�������");
#endif				
				//C_menuN=CLAN_AllEnd;
				break;

			case 2://Ŭ������
				WebDB.Suspend(); //��db ����
				bIsLoading = FALSE;
				//ygy : �� �츱��
				cldata.myPosition = CLANCHIP;				//Ŭ���� ������ �ڵ�� 101����
				ParsingIsClanMember(ANSdata.ret_Buf);
				cldata.isSubChip[0] = '0';
				if(bGetChaClan)
				{
					if(CheckMark(bGetChaClan) ==1)
					{
						C_menuN2=0;
						C_menuN=CLAN_AllEnd;
						break;
					}
					else
					{
						WriteMyClanInfo();
						C_menuN2=0;
						C_menuN=MYCLAN_UPDATE;
						//C_menuN=CLAN_AllEnd;
						
					}				
				}
				else
				{
						WriteMyClanInfo();
						C_menuN2=0;
						C_menuN=MYCLAN_UPDATE;
						//C_menuN=CLAN_AllEnd;
						
				}
				
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� Ŭ����");
#endif				
#ifdef VOICE_CHAT
			if(bCheckClan)
			 {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   cldata.name,(DWORD)atoi(cldata.ClanMark), cldata.myPosition);
				   //bCheckClan = FALSE;

			 }
#endif
				
				
				break;
			case 4: //��ü�� Ŭ����
				WebDB.Suspend(); 
				bIsLoading = FALSE;
				C_menuN2=0;
				C_menuN=CLAN_AllEnd;
				cldata.myPosition = CLANNO_REGESTER;
				if(bGetChaClan) 
				{
					++nStatusCount;
					bGetChaClan = FALSE;
				}
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� Ŭ������ü�� Ŭ����");
#endif
#ifdef VOICE_CHAT
			if(bCheckClan)
			 {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   NULL,0, cldata.myPosition);
				   //bCheckClan = FALSE;

			 }
#endif
				break;
			
			case 3: //Ŭ���� ��ü��  Ŭ�� ��
				WebDB.Suspend(); 
				ParsingIsClanMember2(ANSdata.ret_Buf);
				bIsLoading = FALSE;
				C_menuN2=0;
				C_menuN=CLAN_AllEnd;
				cldata.myPosition = CLANNO_REGESTER;
				if(bGetChaClan) 
				{
					++nStatusCount;
					bGetChaClan = FALSE;
				}
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� Ŭ���� ��ü��  Ŭ�� ��");
#endif
#ifdef VOICE_CHAT
			if(bCheckClan)
			 {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   NULL,0, cldata.myPosition);
				   //bCheckClan = FALSE;

			 }
#endif
				break;
			case 5: //��Ŭ��Ĩ�ϰ��
				WebDB.Suspend(); //��db ����
				bIsLoading = FALSE;
				//ygy : �� �츱��
				cldata.myPosition = CLAN_SUBCHIP;			//Ŭ���� ������ �ڵ�� 100����
				ParsingIsClanMember(ANSdata.ret_Buf);
				cldata.isSubChip[0] = '1';
				if(bGetChaClan)
				{
					if(CheckMark(bGetChaClan) ==1)
					{
						C_menuN2=0;
						C_menuN=CLAN_AllEnd;
						break;
					}
					else
					{
						WriteMyClanInfo();
					
						
					
						C_menuN2=0;
						C_menuN=MYCLAN_UPDATE;
					
					}
				}
				else
				{
						WriteMyClanInfo();				
						C_menuN2=0;
						C_menuN=MYCLAN_UPDATE;
				}
				
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� Ŭ�������");
#endif				
				
#ifdef VOICE_CHAT
			if(bCheckClan)
			 {
				   g_CtrlProcess.SetSelectCha(cldata.szID,cldata.ChaName,
					   cldata.name,(DWORD)atoi(cldata.ClanMark), cldata.myPosition);
				   //bCheckClan = FALSE;

			 }
#endif
				break;
			case 100:
				WebDB.Suspend();
				bIsLoading = FALSE;				
				cldata.myPosition = CLAN_NOCONNECT;
				C_menuN=CLAN_AllEnd;
				if(bGetChaClan) 
				{
					++nStatusCount;
					bGetChaClan = FALSE;
				}
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� �����ڵ� 100");
#endif
				break;
			case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				bIsLoading = FALSE;
				//ygy : �� �츱��
				//wsprintf(szErrorMsg, szServerConnectErrorMsg);
				cldata.myPosition = CLAN_NOCONNECT;
				C_menuN=CLAN_AllEnd;
				menuInit(C_menuN);
				if(bGetChaClan) 
				{
					++nStatusCount;
					bGetChaClan = FALSE;
				}
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� ������ �����Ҽ� ����");
#endif
				
				break;
			default:
				if(ANSdata.ret_val != -1) {		//���������� ���� �ͼ� ������ ����.
					WebDB.Suspend();
					bIsLoading = FALSE;
					C_menuN2=0;
					C_menuN=CLAN_AllEnd;
					cldata.myPosition = CLAN_NOCONNECT;
					if(bGetChaClan) 
					{
						++nStatusCount;
						bGetChaClan = FALSE;
					}
#ifdef CLAN_DEBUG
			DebugMessage("CLAN_isClanWon���� ����Ʈ����");
#endif
					}
				break;
				}
			break;
			}
		break;
		}
	
}



//*****************************************************************************
void CLANEntity_IsClanMember::RenderMain()
{
	if(life ==0) return;

}




//***************************************************************************************
void CLANEntity_IsClanMember::menuInit(int num)
{
	
}





