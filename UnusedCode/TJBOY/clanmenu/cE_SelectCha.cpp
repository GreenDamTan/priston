#define CE_SELECTCHA_CPP

#include "tjclan.h"

#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "cE_SelectCha.h"

CSelectCha::CSelectCha()
{
	nHowDownClanName = 0;
}

CSelectCha::~CSelectCha()
{
}

void CSelectCha::Main()
{
	switch(C_menuN)
	{	
	case CLAN_GETCLANNAME:
		switch(C_menuN2)
		{
		case 0:
			ANSdata.ret_val = -1;
			WebDB.Resume() ;		//��db�� �簳�Ѵ�.
			WebDB.isuserclanname(cldata.szID,cldata.gserver);		//�� id�� Ŭ���������� �ľ��Ѵ�.						
			C_menuN2=1;
			break;
		case 1:
			switch(ANSdata.ret_val) 
			{
			case 1: //����
				WebDB.Suspend();
				nHowDownClanName = 1;
				strcpy(cldata.szChaClanName,ANSdata.ret_Buf);				
				break;
			case 0:
				nHowDownClanName = 2;
				WebDB.Suspend();
				C_menuN = CLAN_AllEnd;
				
				break;
			case 100:
				WebDB.Suspend();
				nHowDownClanName = 2;
				C_menuN = CLAN_AllEnd;				
				break;
			case 1004:
				WebDB.Suspend();
				nHowDownClanName = 2;
				C_menuN = CLAN_AllEnd;				
				break;
			default:
				if(ANSdata.ret_val != -1){
					WebDB.Suspend();
					nHowDownClanName = 2;
					C_menuN = CLAN_AllEnd;
				}
				
				break;
			}
			break;

		}

		break;
	}
	
}

int nSelectCha = 0;

extern int nSelectChaCount;
extern char	szConnServerName[16];	//������ �����̸��� ����ִ� ����
void SelectCha_ClansMark(int num)
{
	int i;
	char server;
	char uniqueclannum[64];
	char path[128];
	char szBMPName[255];
	strcpy(g_ChaClan[0].ClanName, "��Ÿ");
	strcpy(g_ChaClan[0].szMarkNum , "70029");
	strcpy(g_ChaClan[1].ClanName, "��Ÿ1");
	strcpy(g_ChaClan[1].szMarkNum , "70030");
	strcpy(g_ChaClan[2].ClanName, "��Ÿ2");
	strcpy(g_ChaClan[2].szMarkNum , "70031");
	strcpy(g_ChaClan[3].ClanName, "��Ÿ3");
	strcpy(g_ChaClan[3].szMarkNum , "70032");
	strcpy(g_ChaClan[4].ClanName, "��Ÿ4");
	strcpy(g_ChaClan[4].szMarkNum , "70033");
	
	nSelectCha = num;

	WebDB.Resume();
	WebDB.GetClansMark(g_ChaClan[0].ClanName,g_ChaClan[0].szMarkNum,&g_ChaClan[0]);
	
	/*for(i =0; i < 5; i++)
	{
		server = g_ChaClan[i].szMarkNum[0];
		strcpy(uniqueclannum,&g_ChaClan[i].szMarkNum[3]);
		
		wsprintf(szBMPName,"%c_%s_16.%s",server ,uniqueclannum,SAVE_EXE);
		wsprintf(path,"%s\\%s\\%s",SAVE_DIR,szConnServerName,szBMPName);
		#ifdef  USE_PROSTONTALE
		g_ChaClan[i].hClanMark = LoadDibSurfaceOffscreen(path);					
		#else			
		g_ChaClan[i].hClanMark = (HBITMAP)LoadImage(NULL,path,
						IMAGE_BITMAP,
						0,0,LR_LOADFROMFILE);	
		#endif

		if(!g_ChaClan[i].hClanMark)
		{
			WebDB.Resume();
			WebDB.GetClansMark(g_ChaClan[i].ClanName,g_ChaClan[i].szMarkNum,&g_ChaClan[i]);
		}

		
	}*/

	

	//void LoadGetImage(int num);
	//LoadGetImage(num);
	
}


void LoadGetImage(int num)
{
	char server;
	char uniqueclannum[64];
	char path[128];
	char szBMPName[255];

	int i;
	for(i =0; i < num; i++)
	{
		if(!g_ChaClan[i].hClanMark)
		{
			server = g_ChaClan[i].szMarkNum[0];
			strcpy(uniqueclannum,&g_ChaClan[i].szMarkNum[3]);
		
			wsprintf(szBMPName,"%c_%s_16.%s",server ,uniqueclannum,SAVE_EXE);
			wsprintf(path,"%s\\%s\\%s",SAVE_DIR,szConnServerName,szBMPName);

			#ifdef  USE_PROSTONTALE
			g_ChaClan[i].hClanMark = LoadDibSurfaceOffscreen(path);					
			#else			
			g_ChaClan[i].hClanMark = (HBITMAP)LoadImage(NULL,path,
							IMAGE_BITMAP,
							0,0,LR_LOADFROMFILE);	
		}

		
	#endif			
}
	}
		
	