#define CE_NOTICE_CPP
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
#include "tjscroll.h"
#include "cE_report.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "cE_Notice.h"

extern int m_nNoticeCount;
extern int GoNotice_chking;
int nHeight_Count[NOTICECOUNT] = {-1,};

CLANEntity_Notice::CLANEntity_Notice()
{
	ZeroMemory(szNotice,sizeof(szNotice));
	//memset(nHeight_Count,-1,sizeof(nHeight_Count));
	nScl_Bar= 0;
	//m_nNoticeCount= 0;
	

}

CLANEntity_Notice::~CLANEntity_Notice()
{
}


int CLANEntity_Notice::Init(int Life)
{
	life = Life;
	return 1;
}
int CLANEntity_Notice::Clear()
{
	return 1;
}

void CLANEntity_Notice::Main()
{
	int i;
	switch(C_menuN){
	//=====================================================================
	case CLAN_DRAWNOTICE_END:
		--m_nNoticeCount;
		if(m_nNoticeCount > -1)
		{
			OpenFlag = 0;
			//GoNotice_chking = 1;
			C_menuN = CLAN_DRAWNOTICE;
			menuInit(C_menuN);
			C_menuN2 = 0;
			break;
		}		

		extern BOOL bip_port_error;
		if(bip_port_error==1)  {
			OpenFlag = 0;
			C_menuN = 0;
			C_menuN2 = 0;
			//ktj
			ClanCharMove = 0;
			ClanCLCharMove = 0;
			
			void TJBscrollWheelClear()	;	//hoparty.cpp�� ���Ű�����Լ�
			TJBscrollWheelClear();		//hoparty.cpp�� ���Ű�����Լ�
			isDrawClanMenu=0;		//�޴��ȱ׸�
		}
		else {
			C_menuN = CLAN_AllEnd;
			OpenFlag = 0;
			C_menuN = 0;
			C_menuN2 = 0;
			//ktj
			ClanCharMove = 0;
			ClanCLCharMove = 0;
			
			void TJBscrollWheelClear()	;	//hoparty.cpp�� ���Ű�����Լ�
			TJBscrollWheelClear();		//hoparty.cpp�� ���Ű�����Լ�
			isDrawClanMenu=0;		
		}
		break;
	//=====================================================================



		case CLAN_DRAWNOTICE:
			
			//int left, top, right, bottom;
 			//left   = menuPos[0].left;
			//top    = menuPos[0].top;
			//right  = menuPos[0].right;
			//bottom = menuPos[0].bottom;
			//menuPos[0].left   -= 10;
			//menuPos[0].top    -= 5;
			//menuPos[0].right  += 10;
			//menuPos[0].bottom += 60;
			
			 i = chkeckMenuN();
			
			//menuPos[0].left   = left;
			//menuPos[0].top    = top;
			//menuPos[0].right  = right;
			//menuPos[0].bottom = bottom;

			/*
			if(menuPos[0].left <= pCursorPos.x && pCursorPos.x <= menuPos[0].right)
				if(menuPos[0].top <= pCursorPos.y && pCursorPos.y <= menuPos[0].bottom)
					i = 1;
			*/
			
			switch(i) {
			case 0:
				menuMouseOver[0] = 1;
				break;
			default:
				memset(menuMouseOver,-1,sizeof(int)*30);
				break;
			}

			//ktj : ����
			//ygy : �������� ���� TJwheel�������
			if(TJwheel != 0) {			//ktj : wheel>0 �� ������ ������  wheel<0 �� �ڷ� ������
				scrIconPos.top = tscr.ret_WHEELpoint(TJwheel , scrIconPos.top);
				nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
				TJwheel = 0;
			}

			if(MOUSE0())			
			{	
				MOUSE0()=0;
				i = i;
				switch(i) 
				{
				case 0:
					C_menuN = CLAN_DRAWNOTICE_END;
					C_menuN2 =-1;
					CLAN_ERROR_ret=-1;
					break;	
				case 5:
					nScl_Bar = tscr.topScrollLineNum(pCursorPos.y);
					//ktj : ��ũ�� ������ ��ġ�缳��
					scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;
					break;
				default:
					i = i;
					break;

				}
			}
			break;
		case CLAN_NOTICE:
			switch(C_menuN2) {
				case 0:
					ANSdata.ret_val= -1;				
								
					WebDB.Resume() ;		//��db�� �簳�Ѵ�.
					WebDB.ReadNotice(cldata.szID,cldata.ChaName,cldata.gserver);				
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
						OpenFlag = 0;
						WebDB.Suspend(); //��db ����
						C_menuN = CLAN_DRAWNOTICE_END;
						C_menuN2 =-1;
						CLAN_ERROR_ret=-1;
						//ygy : ���� �޼��� �������
						//C_menuN = CLAN_ERROR;
						//wsprintf(szErrorMsg,"%s (63)",szError_Msg);
						//cClanReport.InitMsg(szErrorMsg);
						//cClanReport.menuInit(C_menuN);
						//CLAN_ERROR_ret=255;		//�����޴��������ı׳ɳ�����.
						//C_menuN2=-1;
						break;
					case 1:	
						OpenFlag = 0;
						WebDB.Suspend(); //��db ����						
						NoticeCheck(ANSdata.ret_Buf);
						//++m_nNoticeCount;
						///NoticeCheck("Txt=���������ڿ��δ� 26�� �������������ý���(NEIS) 27�� ���� �� ����� �ſ� �����л�, ����, ��(��)�� 3�� ������ ���� ��NEIS ü�� ������ ���� �����Ű�� �ߴ١��� ��ǻ� NEIS ���� �ߴ��� ���ڷ� �� ���� ������ ��ǥ�ߴ�. �׷��� ���� 16�� �á������������� �����ΰ� ������ ȸ�ǿ� �����ϰ� ���������� �ź��ϰ� ������ �� NEIS ������ �־��� ���·� ������ �ִ�.�ִ� ������ü�� �ѱ������� ������ �� �������Ѹ� ������ �䱸�ϴ� �� �ż��� �ݹ�������, ���� ������� ������ �ѱ������縳���߰�����ȸ������ȸ�� �������� Ⱦ�� �տ� ���ϰ� �׺��� å���� ���� �� ���Ѹ��� �������� �Ѵ١��� �˱��ߴ�.����ȫ(������) �������Ѹ��� 26�� ���� ����ȸ���� ���� ������ �����Խÿ� ������ ������ ��3�� ���ؼ��� NEIS ü���� ��ϵ�, ��2 ���� �л��� ��� 3�� ������ ���� 2004�� 2�� �������� �ѽ������� NEIS ���� ü��(CS)�� ����Ű�� �ߴ١��� ������.�̴� NEIS ������ õ���ؿ� �׵����� ��ħ�� ������ �����ϰ�, 3�� ���� NEIS ���� ������ ���� ü�� ȸ�͸� �����ؿ� �������� ������ ���� ������ ���̴�. �������� �̿� ���� 28�Ϸ� ������ ���������� ����ߴ�. Ư�� ���ѽ������̶� ������ �ޱ�� ������, ��2 ���� �л��� ���� NEIS ���� ü���� �б����������ý���(CS)�� ����Ű�� �����ν� NEIS ������ ��ǻ� �Ұ��������ٴ� �����̴�.");
						//++m_nNoticeCount;
						//NoticeCheck("Txt= Ư�� ���ѽ������̶� ������ �ޱ�� ������, ��2 ���� �л��� ���� NEIS ���� ü���� �б����������ý���(CS)�� ����Ű�� �����ν� NEIS ������ ��ǻ� �Ұ��������ٴ� �����̴�.���������ڿ��δ� 26�� �������������ý���(NEIS) 27�� ���� �� ����� �ſ� �����л�, ����, ��(��)�� 3�� ������ ���� ��NEIS ü�� ������ ���� �����Ű�� �ߴ١��� ��ǻ� NEIS ���� �ߴ��� ���ڷ� �� ���� ������ ��ǥ�ߴ�. �׷��� ���� 16�� �á������������� �����ΰ� ������ ȸ�ǿ� �����ϰ� ���������� �ź��ϰ� ������ �� NEIS ������ �־��� ���·� ������ �ִ�.�ִ� ������ü�� �ѱ������� ������ �� �������Ѹ� ������ �䱸�ϴ� �� �ż��� �ݹ�������, ���� ������� ������ �ѱ������縳���߰�����ȸ������ȸ�� �������� Ⱦ�� �տ� ���ϰ� �׺��� å���� ���� �� ���Ѹ��� �������� �Ѵ١��� �˱��ߴ�.����ȫ(������) �������Ѹ��� 26�� ���� ����ȸ���� ���� ������ �����Խÿ� ������ ������ ��3�� ���ؼ��� NEIS ü���� ��ϵ�, ��2 ���� �л��� ��� 3�� ������ ���� 2004�� 2�� �������� �ѽ������� NEIS ���� ü��(CS)�� ����Ű�� �ߴ١��� ������.�̴� NEIS ������ õ���ؿ� �׵����� ��ħ�� ������ �����ϰ�, 3�� ���� NEIS ���� ������ ���� ü�� ȸ�͸� �����ؿ� �������� ������ ���� ������ ���̴�. �������� �̿� ���� 28�Ϸ� ������ ���������� ����ߴ�.");
						C_menuN = CLAN_DRAWNOTICE;
						menuInit(C_menuN);
						break;
					
					case 100: //������ ��
						OpenFlag = 0;
						WebDB.Suspend(); //��db ����
						C_menuN = CLAN_DRAWNOTICE_END;
						C_menuN2 =-1;
						CLAN_ERROR_ret=-1;
						//ygy : ���� �޼��� �������
						//wsprintf(szErrorMsg, sz100Error);
						//C_menuN = CLAN_ERROR;
						//wsprintf(szErrorMsg,"%s (60)",szError_Msg);
						//cClanReport.InitMsg(szErrorMsg);
						//cClanReport.menuInit(C_menuN);
						//menuInit(C_menuN);
						//CLAN_ERROR_ret=255;		//�����޴��������ı׳ɳ�����.
						//C_menuN2=-1;
						break;
					case ISAO_ERR_TCP_CONNECT:
						OpenFlag = 0;
						WebDB.Suspend();
						C_menuN = CLAN_DRAWNOTICE_END;
						C_menuN2 =-1;
						CLAN_ERROR_ret=-1;
						//ygy : ���� �޼��� �������
						//wsprintf(szErrorMsg,"%s (61)",szError_Msg);
						//C_menuN = CLAN_ERROR;
						//cClanReport.InitMsg(szErrorMsg);
						//cClanReport.menuInit(C_menuN);
						break;
						//end ygy
					
					default:
						if(ANSdata.ret_val != -1) {		//���������� ���� �ͼ� ������ ����.
							OpenFlag = 0;
							WebDB.Suspend();
							C_menuN = CLAN_DRAWNOTICE_END;
							C_menuN2 =-1;
							CLAN_ERROR_ret=-1;
							//ygy : �������
							//C_menuN = CLAN_ERROR;
							//wsprintf(szErrorMsg,"%s (62)",szError_Msg);
							//cClanReport.InitMsg(szErrorMsg);
							//cClanReport.menuInit(C_menuN);					
							//CLAN_ERROR_ret=255;		//�����޴��������ı׳ɳ�����.
							//C_menuN2=-1;
						}
						break;
					}
					break;
			}//C_menu2

			
			break;
	}
		
}

void CLANEntity_Notice::Draw()
{
 	int i,nCap = 0 ,nIconPos =0;

#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
	switch(C_menuN){
		case CLAN_DRAWNOTICE:
			DrawSprite(menuPos[2].left,menuPos[2].top, hButton_Box,  0, 0, 48, 23, 1);		

			if(menuMouseOver[0]>0) 
				DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
			else	
				DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);

			DrawSprite(menuPos[3].left,menuPos[3].top, hClanName,  0, 0, CLANNAME_WIDTH, CLANNAME_HEIGHT		, 1);


			renderDevice.BeginScene();
			dsDrawTexImage( hLine, menuPos[4].left,menuPos[4].top,     403, 10,       255);
			//sinDrawTexImage(hLine,(float)menuPos[4].left,(float)menuPos[4].top,
			//				(float)100, 10.0f,
			//				0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
			renderDevice.EndScene();


			Draw_C_Text("��    ��", menuPos[3].left + 50, menuPos[3].top + 10);
			Draw_C_Text("��    ��", menuPos[3].left + 51, menuPos[3].top + 10);
			Draw_C_Text("��    ��", menuPos[3].left + 50, menuPos[3].top + 11);
			Draw_C_Text("��    ��", menuPos[3].left + 51, menuPos[3].top + 11);


			nIconPos = 5;
			if(nHeight_Count[m_nNoticeCount] > HEIGHT_MAX)
			{
				/*
				DrawSprite(menuPos[nIconPos].left,menuPos[nIconPos].top, hScl_Bar,  0, 0, 8, 256, 1);			

				//ktj : ��ũ�� ������ �׸���
				DrawSprite(scrIconPos.left, scrIconPos.top, hScl_icon,  0, 0, 16, 16, 1);			
				*/
 
				//DrawSprite(menuPos[nIconPos].left, menuPos[nIconPos].top, hScl_Bar,   
				//	0, 0, 8, 160, 1);	
				renderDevice.BeginScene();
				sinDrawTexImage(hScl_Bar_tj,  (float)menuPos[nIconPos].left, (float)menuPos[nIconPos].top, 8, 290);//256);
				sinDrawTexImage( hScl_icon_tj , (float)scrIconPos.left, (float)scrIconPos.top, 16, 16);
				renderDevice.EndScene();

			}


			

			/*
			for(i = 0; i < nHeight_Count; i++)
			{
				if(i > HEIGHT_MAX) break;
				Draw_C_Text(szNotice[i - nScl_Bar], menuPos[1].left, menuPos[1].top+ nCap);									
				nCap += 20;
			}
			*/
			for(i = nScl_Bar, nCap = 0 ; i < nScl_Bar+14; i++)
			{
				if(i >= nHeight_Count[m_nNoticeCount]) break;
				Draw_C_Text(szNotice[m_nNoticeCount][i], menuPos[1].left, menuPos[1].top+ nCap);									
				nCap += 20;
			}
			break;
	}
#else
	HDC hdc;
	HDC sorDC;
	BITMAP bit;
	int bx,by;	

	switch(C_menuN){
		HDC destdc;	
		case CLAN_DRAWNOTICE:
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
			//Text_Out(destdc,menuPos[1].left,menuPos[1].top,szNotice);
			//Text_Out(destdc,menuPos[1].left,menuPos[1].top,"���޼���");

			SelectObject(sorDC, hClanName);
			GetObject(hClanName,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			BitBlt(destdc,menuPos[3].left,menuPos[3].top,menuPos[3].right,menuPos[3].bottom,sorDC,0,0,SRCCOPY);

			Text_Out(destdc,menuPos[3].left + 50,menuPos[3].top+10,"��    ��");


			SelectObject(sorDC, hLine);
			GetObject(hLine,sizeof(BITMAP),&bit);
			StretchBlt(destdc,menuPos[4].left,menuPos[4].top,403,16,sorDC,0,0,16,16,SRCCOPY);

			
			for(i = 0; i < nHeight_Count[m_nNoticeCount]; i++)
			{
				if(i > HEIGHT_MAX) break;
				Text_Out(destdc,menuPos[1].left,menuPos[1].top + nCap,szNotice[i - nScl_Bar]);
				nCap += 20;
			}

			nIconPos = 5;
			if(nHeight_Count[m_nNoticeCount] > HEIGHT_MAX)
			{
						
				SelectObject(sorDC,hScl_Bar);
				GetObject(hScl_Bar,sizeof(BITMAP),&bit);		
				BitBlt(destdc,menuPos[nIconPos].left,menuPos[nIconPos].top,menuPos[nIconPos].right,menuPos[nIconPos].bottom,sorDC,0,0,SRCCOPY);
				nIconPos+=1;

				//SelectObject(sorDC,hScl_icon);
				//GetObject(hScl_icon,sizeof(BITMAP),&bit);
				//BitBlt(destdc,scrIconPos.left,scrIconPos.top,16,16,sorDC,0,0,SRCCOPY);
				//nIconPos+=1;
			

			}
			

			

			
			
			DeleteDC(sorDC);
			DeleteDC(destdc);
			ReleaseDC(g_hwnd, hdc );
			break;
	}

#endif
}


void CLANEntity_Notice::menuInit(int num)
{
	int bx =0, by = 0;
	int nScrollPos=0;
	nScl_Bar = 0;
#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
#else
	BITMAP	bit;
#endif
	switch(num){	
		case CLAN_DRAWNOTICE:
			menuSu = 6;
			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 32;
			by = 16;
			
			#else
			bx = 32;
			by = 16;
			#endif
			menuPos[0].left = NOTICE_X + 185 + 9;		//Ȯ��
			menuPos[0].top = NOTICE_Y + 381 + 4 - 50;
			menuPos[0].right = menuPos[0].left + bx;
			menuPos[0].bottom = menuPos[0].top + by;

			//�ؽ�Ʈ ��� ��ġ
			menuPos[1].left = NOTICE_X + 30;
			menuPos[1].top = NOTICE_Y + 70 - 50;

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			#endif
			menuPos[2].left = NOTICE_X + 185 ;		//Ȯ��
			menuPos[2].top = NOTICE_Y + 381-50;
			menuPos[2].right = menuPos[2].left + bx;
			menuPos[2].bottom = menuPos[2].top + by;

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else
			GetObject(hLine,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			#endif
			menuPos[3].left = NOTICE_X + 115 + 18;//BUTTON1_BOX_X;		//Ȯ��
			menuPos[3].top =  77-50;//BUTTON1_BOX_Y;
			menuPos[3].right = menuPos[2].left + bx;
			menuPos[3].bottom = menuPos[2].top + by;

			#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
			bx = 64;
			by = 16;
			#else
			GetObject(hButton_Box,sizeof(BITMAP),&bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;
			#endif
			menuPos[4].left = NOTICE_X + 7;
			menuPos[4].top = 105-50;
			menuPos[4].right = menuPos[4].left + bx;
			menuPos[4].bottom = menuPos[4].top + by;

			nScrollPos = 5;
			//================��ũ�� ��

			if(nHeight_Count[m_nNoticeCount]> HEIGHT_MAX)			
			{
				#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
				bx = 16;
				by = 16;
				#else
				bx = 16;
				by = 16;
				#endif

				menuPos[nScrollPos].left = BackStartPos.x + NOTICE_SCROLLBAR_LEFT;
				menuPos[nScrollPos].top =  BackStartPos.y + 20 - 50;//SCROOBAR_TOP ;
				menuPos[nScrollPos].right = menuPos[nScrollPos].left + 8;
				menuPos[nScrollPos].bottom = menuPos[nScrollPos].top + 290;

				//ktj ��ũ�Ѱ���				
				tscr.Init(1);
				tscr.Init(	menuPos[nScrollPos].top,//BackStartPos.y + SCROOBAR_TOP, 
						menuPos[nScrollPos].top + 290,
						nHeight_Count[m_nNoticeCount], HEIGHT_MAX);
						
				scrIconPos.left  = BackStartPos.x + NOTICE_SCROLLBAR_LEFT-5;//SCROLLBAR_LEFT-4;
				scrIconPos.top =  menuPos[nScrollPos].top;
			}
			break;
	}
}

#define MAX_CHACOUNT 58
void CLANEntity_Notice::NoticeCheck(char* buf)
{
	int nStrlen = 0,i =0,nCount = 0,nCnt =0;///,
	nHeight_Count[m_nNoticeCount] = 0;
		
	
	buf = strstr(buf,"Txt=");
	buf = &buf[lstrlen("Txt=")];
	nStrlen = lstrlen(buf);
	
	
	ZeroMemory(szNotice[m_nNoticeCount],sizeof(szNotice));
	
	if(nStrlen>NOTICE_MAX)
	{
		nStrlen = NOTICE_MAX;
	}
	while(nHeight_Count[m_nNoticeCount] < NOTICE_HEIGHT )
	{		
		if(buf[i] == 0)
		{
			break;
		}

		if(buf[i] == '\r')
		{			
			if(buf[i+1] == '\n')
			{
				nCount=0;
				szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0;//buf[i];
				++nCount;
				if(i != 0)
				{
					nHeight_Count[m_nNoticeCount]++;
				}
				
				nCnt = 0;
				i++;
				i++;
				continue;
				
			}
			else
			{
				nCount=0;		
				++nCount;
				if(i != 0)
				{
					nHeight_Count[m_nNoticeCount]++;
				}				
				nCnt = 0;
				i++;
				continue;
			}
			
		}		
		else 
		{
			if(IsDBCSLeadByte(buf[i]))
			{
				szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = buf[i];			
				nCnt+=1;
				i+=1;
				szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = buf[i];			
				nCount+=2;;
			}
			else
			{
				//if((nCount == 0) ){//&& (buf[i] == ' ')) {			
				//	i++;
				//	continue;
				//}
				//else
				//{
					if(buf[i] == 0x09) //��Ű
					{
						szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0x20;
						nCnt++;
						szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0x20;
						nCnt++;
						szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0x20;
						nCnt++;
						szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0x20;
					}
					else
					{
						szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = buf[i];			
					}
					++nCount;
				//}
				
			}
			
			
		}
		if(nCount >= (MAX_CHACOUNT))
		{
			nCount = 0;
			nCnt+=1;
			szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = '\n';
			szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt+1] = 0;
			
			nHeight_Count[m_nNoticeCount]++;
			nCnt = 0;
			i++;
			continue;
		}

		nCnt++;
		i++;
		
	}
}


/*
void CLANEntity_Notice::NoticeCheck(char* buf)
{
	int nStrlen = 0,i =0,nCount = 0,nCnt =0;///,
	nHeight_Count = 0;
		
	
	buf = strstr(buf,"Txt=");
	buf = &buf[lstrlen("Txt=")];
	nStrlen = lstrlen(buf);
	
	
	ZeroMemory(szNotice,sizeof(szNotice));
	
	if(nStrlen>NOTICE_MAX)
	{
		nStrlen = NOTICE_MAX;
	}
	while(nHeight_Count < NOTICE_HEIGHT )
	{		
		if(buf[i] == 0)
		{
			break;
		}

		if(buf[i] == '\r')
		{
			i++;
			continue;
		}		

		if(buf[i] == '\n')
		{
			nCount=0;
			szNotice[nHeight_Count][nCnt] = buf[i];
			++nCount;
			nHeight_Count++;
			nCnt = 0;
			i++;
			continue;
			
		}
		else 
		{
			if(IsDBCSLeadByte(buf[i]))
			{
				szNotice[nHeight_Count][nCnt] = buf[i];			
				nCnt+=1;
				i+=1;
				szNotice[nHeight_Count][nCnt] = buf[i];			
				nCount+=2;;
			}
			else
			{
				if((nCount == 0) && (buf[i] == ' ')) {			
					i++;
					continue;
				}
				else
				{
					szNotice[nHeight_Count][nCnt] = buf[i];			
					++nCount;
				}
				
			}
			
			
		}
		if(nCount >= (MAX_CHACOUNT))
		{
			nCount = 0;
			nCnt+=1;
			szNotice[nHeight_Count][nCnt] = '\n';
			szNotice[nHeight_Count][nCnt+1] = 0;
			
			nHeight_Count++;
			nCnt = 0;
			i++;
			continue;
		}

		nCnt++;
		i++;
		
	}
}
*/
/*
void CLANEntity_Notice::NoticeCheck(char* buf)
{
	int nStrlen = 0,i =0,nCount = 0,nCnt =0;
		
	buf = strstr(buf,"Txt=");
	buf = &buf[lstrlen("Txt=")];
	
	
	ZeroMemory(szNotice,sizeof(szNotice));
	nStrlen = lstrlen(buf);
	if(nStrlen>NOTICE_MAX)
	{
		nStrlen = NOTICE_MAX;
	}
	while(i < nStrlen )
	{		
		if(buf[i] == '\r')
		{
			i++;
			continue;
		}		

		if(buf[i] == '\n')
		{
			nCount=0;
			szNotice[nCnt] = buf[i];
			++nCount;
			
		}
		else 
		{
			if(IsDBCSLeadByte(buf[i]))
			{
				szNotice[nCnt] = buf[i];			
				nCnt+=1;
				i+=1;
				szNotice[nCnt] = buf[i];			
				nCount+=2;;
			}
			else
			{
				szNotice[nCnt] = buf[i];			
				++nCount;
			}
			//szNotice[nCnt] = buf[i];			
			
		}
		if(nCount >= (MAX_CHACOUNT))
		{
			nCount = 0;
			nCnt+=1;
			szNotice[nCnt] = '\n';
		}



		nCnt++;
		i++;
		
	}
}

*/