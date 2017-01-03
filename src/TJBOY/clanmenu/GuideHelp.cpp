//---------------------------------------------------------------------------
// GuideHelp.cpp : ĳ���Ͱ� �������� �Ǿ����� �ش緹���� �´� ���̵带 �ڵ�
//                 ���� �����ִ� ���䰡�̵��� ���� �κ��̴�.
//
// latest modify : 2004. 03. 04 by teze(Tae-Jin Kim.)
// Original Developer : Gwang-Yul Yoo
//
//
// (��)�������� �ҽ��ڵ� ���� ����
//
// �� �ҽ� �ڵ�� (��)Ʈ���̱۷ο����Ľ� ���ο����� ����� ��ȿ�մϴ�. 
// (��)Ʈ���̱۷ο����Ľ� �ܿ��� ����� ��� �׿� �մ��� ���� ��ġ��
// �������� �ֽ��ϴ�. 
//
// �������� ���Ͽ� ���� ��� �Ǹ��� (��)�������� �� �ֽ��ϴ�.
//
//
//      Copyright (c) 2003 PRISTON.
//      All Rights Reserved.
//
// (��)PRISTON Seoul South. Korea
// http://kr.pristontale.com
//
// teze�� ���� �ۼ��� �ּ��� ��Ȯ�� Ȯ���� ��ģ ���� �ƴϴ�. �׸��� �������
// �����ؾ��� ��ȿ� ���� ���ݾ� ���� �ξ���. teze�� �ۼ��� �ּ��� by teze��
// �� Ű���带 �޾� �ξ���. ������ �����ص� �ȴ�.
//---------------------------------------------------------------------------

// extern Ŭ������ ������ ���� ���Ǵ�. �������� ���� ��� ��Ŭ������ extern���� ����ȴ�.by teze
#define GUIDEHELP_CPP

//public headers
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>


//custom headers
#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
	#include "sinbaram\\SinLinkHeader.h"
	#include "HoBaram\\HoLinkHeader.h"
	#include "Language\\language.h"
	#include "HoBaram\\HoTextFile.h"
	#include "cE_CViewClanInfo.h"
#endif

#include "tjscroll.h"
#include "tjclan.h"
#include "cE_Notice.h"
#include "GuideHelp.h"

//======================================================================================================
// Static Variabel declare
// : �̷� �κе��� �ɹ� �Ӽ������� ��ȯ�� �ʿ��ϴ� by teze

extern int GuideHelpButton[3]; //���̵� ����
extern BOOL IsDrawGuideHelp;

//int g_LevelLabel[]  0 1 2 3 4 5 6 7 8 9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
int g_LevelLabel[] = {1,2,3,4,5,6,7,8,9,10,11,12,15,17,20,22,25,30,39,40,50,55,60,65,70,75,79,80};
int g_MorianLabel[]= {1,2,3,4,5,6,7,8,9,10,11,12,15,17,20,22,25,30,39,40,50,55,60,65,70,75,79,80};
//int g_LevelLabel[] = {1,2,3,4,5,6,7,8,9,10,12,15,17,20,22,25,30,39,40,50,55,60,65,70,80};//temp comment
//int g_MorianLabel[]= {1,2,3,4,5,6,7,8,9,10,12,15,17,20,22,25,30,39,40,50,55,60,65,70,80};//temp comment

//int nStrLen[] = {19,11,10,6,12,8,11,6,12,24,18,24,13,33,22,12,12,19,12,16,14,14};


//���̵��ǽ��� ������ ��µ� �޴��� ũ�⸦ �����ϱ� ���ؼ� �Ѷ����� ���� ũ�⸦ �����ϱ����� ����ε�
//�ٵ� ���� ���̵� ������ ���̰� ���µ� ������ �� �������� �𸣰���. �ϳ��� �ڵ�κп��� ��(Line : 883)
//by teze

//StrLenth Index   0   1  2  3   4  5   6  7   8   9  10  11  12  13  14  15  16  17  18  19  20  21
int nStrLen[] = {178,102,92,56,108,72,102,56,204,216,168,216,118,246,196,112,224,174,112,148,128,128};

POINT ScrollSavePoint;

extern BOOL bKeyDown;
extern int nKeyDown;
extern BOOL bKeyUp;
extern int nKeyUp;			
extern BOOL bLButtonDown;

// end of declare
//=======================================================================================================


CGuideHelp::CGuideHelp()
{
	m_GuideHelpN = -1;
	nQuetion_Icon =0,nQuetion_Icon1 =0;
	nHelp = nHelpSee = nQuideHelp = nDuru = nDuru1 =0;
	hOff = NULL;
	hOn = NULL;
	bIsOn = TRUE;
	nLevelHelpCount =0;
	ZeroMemory(LevelHelp,sizeof(LevelHelp));	
	ZeroMemory(bHelpIcon,sizeof(bHelpIcon));
	bDoing = FALSE;
	nHelpCountNamugi = 0;

	GuideImage = new smMATERIAL_GROUP(1);
	nGuideImageNum = 0;
	
}

CGuideHelp::~CGuideHelp()
{
	delete GuideImage;
	Clear();
}

void CGuideHelp::ClearImage()
{
	GuideImage->Close();
}

void CGuideHelp::LoadGuideImage(char* file)
{
	ClearImage();
	GuideImage->Init(1);
	nGuideImageNum = GuideImage->CreateTextureMaterial(file,0, 0, 128,128, SMMAT_BLEND_ALPHA );
	GuideImage->ReadTextures();

}

int CGuideHelp::Init(int Life)
{
	life = Life;
	
	return 1;
}

int CGuideHelp::Clear()
{
	if(hOff) 
	{
		hOff->Release();
		hOff = NULL;
	}
	if(hOn) 
	{
		hOn->Release();
		hOn = NULL;
	}
	return 1;
}


int nPos=0;
void CGuideHelp::Main()
{
	int i;
	
	if(!bDoing) return;
	switch(m_GuideHelpN)
	{
	case GUIDEHELP_LIST:
		i = chkeckMenuN();
		switch(i) {
		case 0:				
			menuMouseOver[0] = 1;
			break;
		case 2:
			menuMouseOver[2] = 1;
			break;
		case 5:
			menuMouseOver[2] = 1;
			break;
		case 12:
			case 13:
			case 14:
			case 15:
			case 16:  //5
			case 17:
			case 18:
			case 19:
			case 20:
			case 21: //10
			case 22:
			case 23:
				if(!bHelpIcon[i+nScl_Bar-12])
				{
					ZeroMemory(bHelpIcon,sizeof(bHelpIcon));
					bHelpIcon[i+nScl_Bar-12] =!bHelpIcon[i+nScl_Bar-12];
				}				
				break;
			default:
				ZeroMemory(bHelpIcon,sizeof(bHelpIcon));
				memset(menuMouseOver,-1,sizeof(int)*30);
				break;
		}
		if(TJwheel != 0) {			//ktj : wheel>0 �� ������ ������  wheel<0 �� �ڷ� ������
			scrIconPos.top = tscr_Guide.ret_WHEELpoint(TJwheel , scrIconPos.top);
			nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
			TJwheel = 0;
		}
		//if(MOUSE0())			
		if(GuideHelpButton[0])
		{	
			//MOUSE0()=0;
			
			if(bKeyUp||bKeyDown)
			{
				i = 11;
			}
			switch(i) 
			{
			case 11:
				if(bKeyDown)
				{
					ScrollSavePoint.y+=10;
					if(ScrollSavePoint.y > menuPos[11].top + 246)
					{
						ScrollSavePoint.y = menuPos[11].top + 246;
					}
					nScl_Bar = tscr_Guide.topScrollLineNum(ScrollSavePoint.y);
					if(nHelpCountNamugi == nScl_Bar)
					{
						//nHelpCountNamugi = nScl_Bar;
						scrIconPos.top = menuPos[11].top + 246;
						ScrollSavePoint.y = scrIconPos.top;
					}
					else
					{
						//ktj : ��ũ�� ������ ��ġ�缳��
						scrIconPos.top =  ScrollSavePoint.y-8;//BackStartPos.y + SCROOBAR_TOP;
					}
					GuideHelpButton[0] = 0;
					break;
				}
				if(bKeyUp)
				{
					ScrollSavePoint.y-=10; 
					if(ScrollSavePoint.y < menuPos[11].top)
					{
						ScrollSavePoint.y = menuPos[11].top;
					}
					nScl_Bar = tscr_Guide.topScrollLineNum(ScrollSavePoint.y);
					//ktj : ��ũ�� ������ ��ġ�缳��
					scrIconPos.top =  ScrollSavePoint.y-8;//BackStartPos.y + SCROOBAR_TOP;			
					GuideHelpButton[0] = 0;
					
					break;
				}
				bLButtonDown = TRUE;
				
				ScrollSavePoint = pCursorPos;				
				nScl_Bar = tscr_Guide.topScrollLineNum(pCursorPos.y);
				
				if(nHelpCountNamugi == nScl_Bar)
				{
					scrIconPos.top = menuPos[11].top + 246;
					ScrollSavePoint.y = scrIconPos.top;
				}
				if(pCursorPos.y > menuPos[11].top + 246)
				{
					scrIconPos.top = menuPos[11].top + 246;
				}
				else if(pCursorPos.y < menuPos[11].top)
				{
					scrIconPos.top = menuPos[11].top;
				}
				else
				{
					//ktj : ��ũ�� ������ ��ġ�缳��
					scrIconPos.top =  pCursorPos.y;//BackStartPos.y + SCROOBAR_TOP;				
				}
				
				break;
			
			case 0: //Ȯ��
				IsDrawGuideHelp = FALSE;
				m_GuideHelpN = -1;
				bDoing = FALSE;
				isDrawClanMenu =0;
				GuideHelpButton[0] = 0;
				TJBscrollWheelClear();
				TJBscrollWheelClear_Guide();
				break;
			case 5:
			case 2: //�󿭾� ����������ش�
				IsDrawGuideHelp = FALSE;
				m_GuideHelpN = -1;
				isDrawClanMenu =0;
				TJBscrollWheelClear();
				TJBscrollWheelClear_Guide();
				int sinShowHelp();
				sinShowHelp();
				GuideHelpButton[0] = 0;
				break;
			case 9: //������ ������ ���� �������� ���� üũ ���� �κ�
				bIsOn = !bIsOn;
				GuideHelpButton[0] = 0;
				break;
			
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:  //5
			case 17:
			case 18:
			case 19:
			case 20:
			case 21: //10
			case 22:
			case 23:			
				if(!bLButtonDown)
				{
					nPos = (i+nScl_Bar-12);
					if(sinChar->JOB_CODE < 5)
					{
						CheckLevel(g_LevelLabel[nLevelHelpCount-nPos-1],sinChar->JOB_CODE);
					}
					else
					{
						//CheckLevel(g_MorianLabel[i+nScl_Bar-12],sinChar->JOB_CODE);
						CheckLevel(g_MorianLabel[nLevelHelpCount-nPos-1],sinChar->JOB_CODE);
					}

					m_GuideHelpN = GUIDEHELP_CONTENT;
					menuInit(m_GuideHelpN);
					GuideHelpButton[0] = 0;
				}
				break;
			default:
				if(bLButtonDown)
				{
					//���� 2003.10.30
					if(pCursorPos.y < menuPos[11].top)
					{
						scrIconPos.top = menuPos[11].top;
						nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
					}
					else if(pCursorPos.y > menuPos[11].top + 246)
					{
						scrIconPos.top = menuPos[11].top + 246;
						nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
					}
					else
					{
						scrIconPos.top = pCursorPos.y;
						nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
					}
					//������
					/*if(pCursorPos.y < menuPos[11].top)
					{
						pCursorPos.y = menuPos[11].top;
					}
					if(pCursorPos.y > menuPos[11].top + 246)
					{
						pCursorPos.y = menuPos[11].top + 246;
					}
					ScrollSavePoint = pCursorPos;
					nScl_Bar = tscr_Guide.topScrollLineNum(pCursorPos.y);
					if(nHelpCountNamugi == nScl_Bar)
					{
						scrIconPos.top = menuPos[11].top + 246;
						ScrollSavePoint.y = scrIconPos.top;
					}
					//ktj : ��ũ�� ������ ��ġ�缳��
					scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;					
					*/
				}
				else
					GuideHelpButton[0] = 0;
				break;
			}
		}
	
		break;
	case GUIDEHELP_CONTENT:
		i = chkeckMenuN();
		switch(i) {
		case 0:				
			menuMouseOver[0] = 1;
			break;
		case 2:
			menuMouseOver[2] = 1;
			break;
		case 5:
			menuMouseOver[2] = 1;
			break;
		case 10:
		case 11:
			bHelpIcon[10] = 1;
			break;		
		default:			
			ZeroMemory(bHelpIcon,sizeof(bHelpIcon));
			memset(menuMouseOver,-1,sizeof(int)*30);
			break;
		}

		if(TJwheel != 0) {			//ktj : wheel>0 �� ������ ������  wheel<0 �� �ڷ� ������
			scrIconPos.top = tscr_Guide.ret_WHEELpoint(TJwheel , scrIconPos.top);
			nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
			TJwheel = 0;
		}
		
		//if(MOUSE0())			
		if(GuideHelpButton[0])
		{	
			//MOUSE0()=0;
			//GuideHelpButton[0]=0;
			if(bKeyUp||bKeyDown)
			{
				i = 15;
			}
			switch(i) 
			{
			case 0:
				IsDrawGuideHelp = FALSE;
				m_GuideHelpN = -1;
				bDoing = FALSE;
				isDrawClanMenu = 0;
				TJBscrollWheelClear();
				TJBscrollWheelClear_Guide();
				GuideHelpButton[0] = 0;
				break;
			case 5:
			case 2: //�󿭾� ����������ش�
				IsDrawGuideHelp = FALSE;
				m_GuideHelpN = -1;
				isDrawClanMenu =0;
				TJBscrollWheelClear();
				TJBscrollWheelClear_Guide();
				int sinShowHelp();
				sinShowHelp();
				GuideHelpButton[0]=0;
				break;
			case 9: //������ ������ ���� �������� ���� üũ ���� �κ�
				bIsOn = !bIsOn;
				GuideHelpButton[0]=0;
				break;
			case 6:
			case 7:
			case 11:
			case 10:
				SetDoing(TRUE);				
				CheckLevel(sinChar->Level);				
				SetHelpN(GUIDEHELP_LIST);
				menuInit(GUIDEHELP_LIST);
				IsDrawGuideHelp = TRUE;
				GuideHelpButton[0]=0;
				break;			
			case 15:
				if(bKeyDown)
				{
					//pCursorPos.y = scrIconPos.top;
					scrIconPos.top+=10;
					if(scrIconPos.top > menuPos[15].top + 246)
					{
						scrIconPos.top = menuPos[15].top + 246;
					}
					nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
					//scrIconPos.top = pCursorPos.y;
					/*if(nHelpCountNamugi == nScl_Bar)
					{
						//nHelpCountNamugi = nScl_Bar;
						scrIconPos.top = menuPos[11].top + 246;
						ScrollSavePoint.y = scrIconPos.top;
					}
					else
					{
						//ktj : ��ũ�� ������ ��ġ�缳��
						scrIconPos.top =  ScrollSavePoint.y-8;//BackStartPos.y + SCROOBAR_TOP;
					}*/
					GuideHelpButton[0] = 0;
					break;
				}
				if(bKeyUp)
				{
					//ScrollSavePoint.y-=10; 
					scrIconPos.top-=10;
					if(scrIconPos.top < menuPos[15].top)
					{
						scrIconPos.top = menuPos[15].top;
					}
					nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
					//ktj : ��ũ�� ������ ��ġ�缳��
					//scrIconPos.top =  ScrollSavePoint.y-8;//BackStartPos.y + SCROOBAR_TOP;			
					GuideHelpButton[0] = 0;
					
					break;
				}
				bLButtonDown = TRUE;
				
				//ScrollSavePoint = pCursorPos.y;				
				//nScl_Bar = tscr_Guide.topScrollLineNum(pCursorPos.y);

				if(pCursorPos.y > menuPos[15].top + 246)
				{
					scrIconPos.top = menuPos[15].top + 246; 
					nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
				}
				else if(pCursorPos.y < menuPos[15].top)
				{
					scrIconPos.top = menuPos[15].top;
					nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
				}
				else
				{
					nScl_Bar = tscr_Guide.topScrollLineNum(pCursorPos.y);
					scrIconPos.top = pCursorPos.y;
				}


				
				
				/*if(nHelpCountNamugi == nScl_Bar)
				{
					scrIconPos.top = menuPos[15].top + 246;
					ScrollSavePoint.y = scrIconPos.top;
				}
				if(pCursorPos.y > menuPos[15].top + 246)
				{
					scrIconPos.top = menuPos[15].top + 246;
				}
				else if(pCursorPos.y < menuPos[15].top)
				{
					scrIconPos.top = menuPos[15].top;
				}
				else
				{
					//ktj : ��ũ�� ������ ��ġ�缳��
					scrIconPos.top =  pCursorPos.y;//BackStartPos.y + SCROOBAR_TOP;				
				}*/
				/*nScl_Bar = tscr_Guide.topScrollLineNum(pCursorPos.y);
				//ktj : ��ũ�� ������ ��ġ�缳��
				if(pCursorPos.y > menuPos[15].top + 246)
				{
					scrIconPos.top = menuPos[15].top + 246;
				}
				else
					scrIconPos.top =  pCursorPos.y-8;//BackStartPos	
					*/
				
				break;
			default:
				if(bLButtonDown)
				{
					if(pCursorPos.y < menuPos[15].top)
					{
						scrIconPos.top = menuPos[15].top;
						nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
					}
					else if(pCursorPos.y > menuPos[15].top + 246)
					{
						scrIconPos.top = menuPos[15].top + 246;
						nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
					}
					else
					{
						scrIconPos.top = pCursorPos.y;
						nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
					}

					//ScrollSavePoint = pCursorPos;
					
					/*if(nHelpCountNamugi == nScl_Bar)
					{
						scrIconPos.top = menuPos[11].top + 246;
						ScrollSavePoint.y = scrIconPos.top;
					}*/
					//ktj : ��ũ�� ������ ��ġ�缳��
					//scrIconPos.top =  pCursorPos.y-8;//BackStartPos.y + SCROOBAR_TOP;					
				}
				else
					GuideHelpButton[0] = 0;
				break;
			}
		}

		break;
	}
}



void CGuideHelp::Draw()
{
	int i;
	int MenuPos=0;
	int nCap =0;
	if(!bDoing) return;
	switch(m_GuideHelpN)
	{
	case GUIDEHELP_LIST:
		DrawSprite(menuPos[1].left,menuPos[1].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);

		DrawSprite(menuPos[4].left,menuPos[4].top, hClanName,  0, 0, CLANNAME_WIDTH, CLANNAME_HEIGHT, 1);


		renderDevice.BeginScene();
		dsDrawTexImage( nLine, menuPos[3].left,menuPos[3].top,  403, 10,       255);
		if(menuMouseOver[2]>0)
		{
			dsDrawTexImage( nQuetion_Icon, menuPos[2].left,menuPos[2].top,     32, 32,       255);
		}
		else
		{
			dsDrawTexImage( nQuetion_Icon1, menuPos[2].left,menuPos[2].top,     32, 32,       255);
		}
		dsDrawTexImage( nHelp, menuPos[5].left,menuPos[5].top,     64, 32,       255);
		dsDrawTexImage( nDuru, menuPos[6].left,menuPos[6].top,     32, 32,       255);
		dsDrawTexImage( nQuideHelp, menuPos[7].left,menuPos[7].top,     64, 32,       255);
		dsDrawTexImage( nHelpSee, menuPos[8].left,menuPos[8].top,     64, 32,       255);
		renderDevice.EndScene();

//		Draw_Title_Text("��   ��   ��", menuPos[10].left, menuPos[10].top);
#ifdef _LANGUAGE_JAPANESE
		Draw_Title_Text("�K   �C   �h", menuPos[10].left, menuPos[10].top);
#else
		Draw_Title_Text("G  U  I  D  E", menuPos[10].left, menuPos[10].top);
#endif
		//Draw_Title_Text("���̵� �� �� ��", menuPos[10].left + 51-5, menuPos[10].top + 11);
		//Draw_C_Text("���̵� �� �� ��", menuPos[10].left + 50, menuPos[10].top + 11);
		//Draw_C_Text("���̵� �� �� ��", menuPos[10].left + 51, menuPos[10].top + 11);

		MenuPos = 12;		
		for(i=nScl_Bar; i<nScl_Bar+LEVELHELP_COUNT_MAX; i++)
		{
			//if(i >= LEVELHELP_COUNT_MAX) break;
 			if(i >= nLevelHelpCount) break;
			menuPos[MenuPos].right = menuPos[MenuPos].left + 300;//(6*lstrlen(LevelHelp[i]) + 30);
			renderDevice.BeginScene();
			if(bHelpIcon[i])
			{
				
				sinDrawTexImage( nDuru, (float)menuPos[MenuPos].left, (float)menuPos[MenuPos].top , 24, 24);
			}
			else
			{
				sinDrawTexImage( nDuru1, (float)menuPos[MenuPos].left, (float)menuPos[MenuPos].top , 24, 24);
				
			}
			renderDevice.EndScene();
			if(bHelpIcon[i])
			{
				//
				//Draw_C_Text(LevelHelp[nLevelHelpCount-i-1], menuPos[MenuPos].left+30, menuPos[MenuPos].top+5);
				Draw_Help_Text2(LevelHelp[nLevelHelpCount-i-1], menuPos[MenuPos].left+30, menuPos[MenuPos].top+5);
				
			}
			else
			{
				//Draw_C_Text(LevelHelp[nLevelHelpCount-i-1], menuPos[MenuPos].left+30, menuPos[MenuPos].top+5);
				Draw_Help_Text(LevelHelp[nLevelHelpCount-i-1], menuPos[MenuPos].left+30, menuPos[MenuPos].top+5);
			}
			MenuPos++;			
		}

		MenuPos = 11;

		if(nLevelHelpCount> LEVELHELP_COUNT_MAX)
		{
			renderDevice.BeginScene();
			sinDrawTexImage(nScl_Bar_tj,  (float)menuPos[MenuPos].left, (float)menuPos[MenuPos].top, 8, 256);
			sinDrawTexImage( nScl_icon_tj , (float)scrIconPos.left, (float)scrIconPos.top, 16, 16);
			renderDevice.EndScene();
		}

		

		if(bIsOn)
		{
			DrawSprite(menuPos[9].left,menuPos[9].top, hOn,  0, 0, 36, 24, 1);
		}
		else
		{
			DrawSprite(menuPos[9].left,menuPos[9].top, hOff,  0, 0, 36, 24, 1);
		}
		
		break;
	case GUIDEHELP_CONTENT:
 		DrawSprite(menuPos[1].left,menuPos[1].top, hButton_Box,  0, 0, 48, 23, 1);		
		if(menuMouseOver[0]>0) DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Yellow,  0, 0, 32, 16, 1);
		else	DrawSprite(menuPos[0].left,menuPos[0].top, hOk_Gray,  0, 0, 32, 16, 1);

		DrawSprite(menuPos[4].left,menuPos[4].top, hClanName,  0, 0, CLANNAME_WIDTH, CLANNAME_HEIGHT, 1);


		renderDevice.BeginScene();
		dsDrawTexImage( nLine, menuPos[3].left,menuPos[3].top,  403, 10,       255);
		if(menuMouseOver[2]>0)
		{
			dsDrawTexImage( nQuetion_Icon, menuPos[2].left,menuPos[2].top,     32, 32,       255);
		}
		else
		{
			dsDrawTexImage( nQuetion_Icon1, menuPos[2].left,menuPos[2].top,     32, 32,       255);
		}

		dsDrawTexImage( nHelp, menuPos[5].left,menuPos[5].top,     64, 32,       255);
		dsDrawTexImage( nDuru, menuPos[6].left,menuPos[6].top,     32, 32,       255);
		dsDrawTexImage( nQuideHelp, menuPos[7].left,menuPos[7].top,     64, 32,       255);
		dsDrawTexImage( nHelpSee, menuPos[8].left,menuPos[8].top,     64, 32,       255);
		renderDevice.EndScene();

//		Draw_Title_Text("��   ��   ��", menuPos[12].left, menuPos[12].top);
#ifdef _LANGUAGE_JAPANESE
		Draw_Title_Text("�K   �C   �h", menuPos[12].left, menuPos[12].top);
#else
		Draw_Title_Text("G  U  I  D  E", menuPos[12].left, menuPos[12].top);
#endif
		//Draw_C_Text("���̵� �� �� ��", menuPos[12].left + 51-5, menuPos[12].top + 11);

		renderDevice.BeginScene();
		//�� �ܰ�� �Ѱ��ִ� �̹��� �κ�
		if(bHelpIcon[10])
		{
			sinDrawTexImage( nDuru, (float)menuPos[10].left, (float)menuPos[10].top , 24, 24);
		}
		else
		{
			sinDrawTexImage( nDuru1, (float)menuPos[10].left, (float)menuPos[10].top , 24, 24);
			
		}		
		//�� �ܰ�� �Ѱ��ִ� �۾� �κ�
		dsDrawTexImage( nGuideList, menuPos[11].left,menuPos[11].top,     64, 32,       255);
		
		//����ȿ� �� �̹���
		if((g_LevelLabel[nLevelHelpCount-nPos-1]!= 9) && (g_LevelLabel[nLevelHelpCount-nPos-1]!= 39))
		{
			dsDrawTexImageFloat( nGuideImageNum, (float)menuPos[14].left,(float)menuPos[14].top,     128, 128,       255);
		}
		//dsDrawTexImage( nGuideImageNum, menuPos[14].left,menuPos[14].top,     128, 128,       255);
		

		renderDevice.EndScene();

		if(bIsOn)
		{
			DrawSprite(menuPos[9].left,menuPos[9].top, hOn,  0, 0, 36, 24, 1);
		}
		else
		{
			DrawSprite(menuPos[9].left,menuPos[9].top, hOff,  0, 0, 36, 24, 1);
		}

		MenuPos = 15;
		if(nHeight_Count > GUIDEHLEP_CONTENT_HEIGHT_MAX)
		{
			renderDevice.BeginScene();
			sinDrawTexImage(nScl_Bar_tj,  (float)menuPos[MenuPos].left, (float)menuPos[MenuPos].top, 8, 256);
			sinDrawTexImage( nScl_icon_tj , (float)scrIconPos.left, (float)scrIconPos.top, 16, 16);
			renderDevice.EndScene();
		}

		//nCap = 0;
		MenuPos = 16;
		for(i=nScl_Bar; i<nScl_Bar + GUIDEHLEP_CONTENT_HEIGHT_MAX; i++)
		{
			if(i >=nHeight_Count) break; 
			if(nHeight_Count >  GUIDEHLEP_CONTENT_HEIGHT_MAX)
				Draw_C_Text(m_HelpContent[i], menuPos[MenuPos].left, menuPos[MenuPos].top);
			else
				Draw_C_Text(m_HelpContent[i], menuPos[MenuPos].left + 20, menuPos[MenuPos].top);
			MenuPos++;
		}

		break;
	}
}



void CGuideHelp::Load()
{
	nQuetion_Icon = CreateTextureMaterial( "image\\Sinimage\\Help\\help_icon01.tga", 0, 0, 64,64, SMMAT_BLEND_ALPHA );
	nQuetion_Icon1 = CreateTextureMaterial( "image\\Sinimage\\Help\\help_icon01_.tga", 0, 0, 64,64, SMMAT_BLEND_ALPHA );
	nHelp = CreateTextureMaterial( "image\\Sinimage\\Help\\help_icon01t.tga", 0, 0, 64,64, SMMAT_BLEND_ALPHA );
	nHelpSee = CreateTextureMaterial( "image\\Sinimage\\Help\\help_icon00.tga", 0, 0, 64,64, SMMAT_BLEND_ALPHA );
	nQuideHelp = CreateTextureMaterial( "image\\Sinimage\\Help\\help_icon02t.tga", 0, 0, 64,64, SMMAT_BLEND_ALPHA );
	nDuru = CreateTextureMaterial( "image\\Sinimage\\Help\\help_icon02.tga", 0, 0, 64,64, SMMAT_BLEND_ALPHA );
	nDuru1 = CreateTextureMaterial( "image\\Sinimage\\Help\\help_icon02_.tga", 0, 0, 64,64, SMMAT_BLEND_ALPHA );
	nLine = CreateTextureMaterial( "image\\Sinimage\\Help\\Box_Line.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	hOn = LoadDibSurfaceOffscreen("image\\Sinimage\\Help\\G_help-On.bmp");
	hOff = LoadDibSurfaceOffscreen("image\\Sinimage\\Help\\G_help-Off.bmp");

	nScl_icon_tj = CreateTextureMaterial( "image\\Sinimage\\Help\\scl_icon.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	
	nScl_Bar_tj= CreateTextureMaterial( "image\\Sinimage\\Help\\scl_bar.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	nGuideList = CreateTextureMaterial( "image\\Sinimage\\Help\\help_Glist.tga", 0, 0, 64,32, SMMAT_BLEND_ALPHA );


}




void CGuideHelp::menuInit(int num)
{
	int nScrollPos = 0;
	int i =0;
	int nCap =0;

	

	switch(num)
	{
		case GUIDEHELP_LIST: //������ ���� ���� ����
	 	menuSu  = 25;					
		
 		menuPos[1].left   = 392;//NOTICE_X + 185 ;		//�ڽ�
		menuPos[1].top    = 411;//NOTICE_Y + 381;
		menuPos[1].right  = menuPos[0].left + 64;
		menuPos[1].bottom = menuPos[0].top + 16;
		
		menuPos[0].left   = 392+9;//NOTICE_X + 185 + 9; //Ȯ��
		menuPos[0].top    = 411+4;//NOTICE_Y + 381 + 4; //NOTICE_Y + 381 + 4;
		menuPos[0].right  = menuPos[1].left + 32;
		menuPos[0].bottom = menuPos[1].top + 16;

		menuPos[2].left   = 208+25;                    //���� ����ǥ �̹��� ��ġ
		menuPos[2].top    = 74;
		menuPos[2].right  = menuPos[2].left + 32;
		menuPos[2].bottom = menuPos[2].top + 32;

		menuPos[3].left   = 208+8;                      //����
		menuPos[3].top    = 61+48;
		menuPos[3].right  = menuPos[3].left + 64;
		menuPos[3].bottom = menuPos[3].top + 16;

		menuPos[4].left   = 340;//NOTICE_X + 115 + 18;  //Ÿ��Ʋ
		menuPos[4].top    = 75;
		menuPos[4].right  = menuPos[4].left + 64;
		menuPos[4].bottom = menuPos[4].top + 32;

		

		menuPos[5].left   = 208+55;                    //���� �۾� ��ġ
		menuPos[5].top    = 74;
		menuPos[5].right  = menuPos[5].left + 64;
		menuPos[5].bottom = menuPos[5].top + 32;

		menuPos[6].left   = 208+300+5;                 //���̵� ���� �̹���
		menuPos[6].top    = 75;
		menuPos[6].right  = menuPos[6].left + 32;
		menuPos[6].bottom = menuPos[6].top + 32;

		menuPos[7].left   = 208+340;                   //���̵� ���� �۾�
		menuPos[7].top    = 75;
		menuPos[7].right  = menuPos[7].left+64;
		menuPos[7].bottom = menuPos[7].top + 32;

		menuPos[8].left   = NOTICE_X + 185 +100;       //���� ���� �̹���
		menuPos[8].top    = 409;
		menuPos[8].right  = menuPos[8].left + 64;
		menuPos[8].bottom = menuPos[8].top + 16;

		menuPos[9].left   = menuPos[8].left+64 ;		    //�¿���
		menuPos[9].top    = 411;
		menuPos[9].right  = menuPos[9].left + 64;
		menuPos[9].bottom = menuPos[9].top + 16;

		menuPos[10].left  = menuPos[4].left + 35;//115 + 18;		Ÿ��Ʋ ����
		menuPos[10].top   = menuPos[4].top + 9;
		
		//by woodongdong
 		for(int index = 0; index <= 10; index++)
		{
			menuPos[index].top    -= 50;
			menuPos[index].bottom -= 50;
		}

		//��ũ�����������
		nScrollPos = 11;
		//================��ũ�� ��if(nLevelHelpCount> LEVELHELP_COUNT_MAX)
		{			

			menuPos[nScrollPos].left = 602;//BackStartPos.x + NOTICE_SCROLLBAR_LEFT;
			menuPos[nScrollPos].top =  125;//BackStartPos.y + 10;//SCROOBAR_TOP ;
			menuPos[nScrollPos].right = menuPos[nScrollPos].left + 8;
			menuPos[nScrollPos].bottom = menuPos[nScrollPos].top + 290;
			
			//by woodongdong
			menuPos[nScrollPos].top    -= 50;
			menuPos[nScrollPos].bottom -= 50;
			
			//ktj ��ũ�Ѱ���
			nScl_Bar = 0;
			tscr_Guide.Init(1);
			tscr_Guide.Init(	menuPos[nScrollPos].top,//BackStartPos.y + SCROOBAR_TOP, 
					menuPos[nScrollPos].top + 256,//290,
					nLevelHelpCount, LEVELHELP_COUNT_MAX);
					
			scrIconPos.left  = 602-4;//SCROLLBAR_LEFT-4;
			scrIconPos.top =  menuPos[nScrollPos].top  ;
			ScrollSavePoint.y = scrIconPos.top;
		}
		
 		nScrollPos = 12;
		for(i =0; i <LEVELHELP_COUNT_MAX; i++)
		{
			if(i>=nLevelHelpCount) break;
			menuPos[nScrollPos].left = menuPos[5].left;//NOTICE_X + 130;
			menuPos[nScrollPos].top  = NOTICE_Y + 80 + nCap;
			//by woodongdong
			menuPos[nScrollPos].top -= 50;
			
			//StrLen�� ���� ������ �κ�. �����ص� �Ǵ����� ���� ������ �ʿ��� by teze
   	 		menuPos[nScrollPos].right  = menuPos[nScrollPos].left + nStrLen[i];//(6*lstrlen(LevelHelp[i]) + 30);//24;
			menuPos[nScrollPos].bottom = menuPos[nScrollPos].top + 24;
			
			nScrollPos ++;
			nCap+=24;
		}		
		break;

	case GUIDEHELP_CONTENT:
 		nScl_Bar = 0;
		menuSu = 16;
		menuPos[1].left = 392;//NOTICE_X + 185 ;		    //�ڽ�
		menuPos[1].top = 411;//NOTICE_Y + 381;
		menuPos[1].right = menuPos[0].left + 64;
		menuPos[1].bottom = menuPos[0].top + 16;

	
		
		menuPos[0].left = 392+9;//NOTICE_X + 185 + 9;		//Ȯ��
		menuPos[0].top = 411+4;//NOTICE_Y + 381 + 4;//NOTICE_Y + 381 + 4;
		menuPos[0].right = menuPos[1].left + 32;
		menuPos[0].bottom = menuPos[1].top + 16;

		menuPos[3].left = 208+8;           //����
		menuPos[3].top = 61+48;
		menuPos[3].right = menuPos[3].left + 64;
		menuPos[3].bottom = menuPos[3].top + 16;

		menuPos[4].left = 340;//OTICE_X + 115 + 18;     //Ÿ��Ʋ
		menuPos[4].top =  75;
		menuPos[4].right = menuPos[4].left + 64;
		menuPos[4].bottom = menuPos[4].top + 32;

		menuPos[2].left = 208+25;          //���� ����ǥ �̹��� ��ġ
		menuPos[2].top =  74;
		menuPos[2].right = menuPos[2].left + 32;
		menuPos[2].bottom = menuPos[2].top + 32;

		menuPos[5].left = 208+55;     //���� �۾� ��ġ
		menuPos[5].top =  74;
		menuPos[5].right = menuPos[5].left + 64;
		menuPos[5].bottom = menuPos[5].top + 32;

		menuPos[6].left = 208+300+5;     //���̵� ���� �̹���
		menuPos[6].top =  75;
		menuPos[6].right = menuPos[6].left + 32;
		menuPos[6].bottom = menuPos[6].top + 32;

		menuPos[7].left = 208+340;     //���̵� ���� �۾�
		menuPos[7].top =  75;
		menuPos[7].right = menuPos[7].left+64;
		menuPos[7].bottom = menuPos[7].top + 32;

		menuPos[8].left = NOTICE_X + 185 +100;		    //���� ���� �̹���
		menuPos[8].top = 409;
		menuPos[8].right = menuPos[8].left + 64;
		menuPos[8].bottom = menuPos[8].top + 16;

		menuPos[9].left = menuPos[8].left+64 ;		    //�¿���
		menuPos[9].top = 411;
		menuPos[9].right = menuPos[9].left + 64;
		menuPos[9].bottom = menuPos[9].top + 16;

		menuPos[10].left = NOTICE_X + 18 +8;		    //���̵� ���� �η縶�� �̹���
		menuPos[10].top = 411;
		menuPos[10].right = menuPos[10].left + 24;
		menuPos[10].bottom = menuPos[10].top + 16;

		menuPos[11].left = menuPos[10].right-3;		    //���̵� ����
		menuPos[11].top = 409;
		menuPos[11].right = menuPos[11].left + 64;
		menuPos[11].bottom = menuPos[11].top + 32;

		menuPos[12].left = menuPos[4].left + 35;//115 + 18;		Ÿ��Ʋ ����
		menuPos[12].top =  menuPos[4].top+9;

		menuPos[13].left = NOTICE_X + 18+18 ;;//115 + 18;		���� ���� ��ġ
		menuPos[13].top =  NOTICE_Y + 75;

		menuPos[14].left = 430;		    //���̵� �̹���
		menuPos[14].top = 270;
		menuPos[14].right = menuPos[14].left + 64;
		menuPos[14].bottom = menuPos[14].top + 32;

		//by woodongdong
		for(int index = 0; index <= 14; index++)
			menuPos[index].top -= 50;
		

		//================��ũ�� ��

		nScrollPos = 15;
		if(nHeight_Count> GUIDEHLEP_CONTENT_HEIGHT_MAX)
		{

			menuPos[nScrollPos].left = 602;//BackStartPos.x + NOTICE_SCROLLBAR_LEFT;
			menuPos[nScrollPos].top =  125;//BackStartPos.y + 10;//SCROOBAR_TOP ;
			menuPos[nScrollPos].right = menuPos[nScrollPos].left + 8;
			menuPos[nScrollPos].bottom = menuPos[nScrollPos].top + 290;

			//ktj ��ũ�Ѱ���
			nScl_Bar = 0;
			tscr_Guide.Init(1);
			tscr_Guide.Init(	menuPos[nScrollPos].top,//BackStartPos.y + SCROOBAR_TOP, 
					menuPos[nScrollPos].top + 256,//290,
					nHeight_Count, GUIDEHLEP_CONTENT_HEIGHT_MAX);
					
			scrIconPos.left  = 602-4;//SCROLLBAR_LEFT-4;
			scrIconPos.top =  menuPos[nScrollPos].top  ;
			ScrollSavePoint.y = scrIconPos.top;
		}

		nScrollPos = 16;
		nCap = 0;
		for(int i = 0; i < GUIDEHLEP_CONTENT_HEIGHT_MAX; i++)
		{
			menuPos[nScrollPos].left = menuPos[13].left - 20;//BackStartPos.x + NOTICE_SCROLLBAR_LEFT;
			menuPos[nScrollPos].top =  menuPos[13].top + nCap;//BackStartPos.y + 10;//SCROOBAR_TOP ;
			nCap+=20;
			nScrollPos++;
		}

		break;
	}
}

void CGuideHelp::SetHelpN(int num)
{
	m_GuideHelpN = num;
}

void CGuideHelp::SetStartPos(int x, int y)
{
	m_Start.x = x;
	m_Start.y = y;
}

void CGuideHelp::InitLevelHelpLabel()
{	
//�ؿ� ��(GetPrivateProfileString �� �κ� ���δ�)
#define LEVELLABEL "./image/clanImage/Help/AutoHelp/LevelSubj.ini"

	if(sinChar->JOB_CODE < 5) // �۽�ũ�а� ����̾��� �����ΰ� ���� by teze
	{
		GetPrivateProfileString("Level_1","Subj","Error",LevelHelp[0],1024,LEVELLABEL);
		GetPrivateProfileString("Level_2","Subj","Error",LevelHelp[1],1024,LEVELLABEL);
		GetPrivateProfileString("Level_3","Subj","Error",LevelHelp[2],1024,LEVELLABEL);
		GetPrivateProfileString("Level_4","Subj","Error",LevelHelp[3],1024,LEVELLABEL);
		GetPrivateProfileString("Level_5","Subj","Error",LevelHelp[4],1024,LEVELLABEL);
		GetPrivateProfileString("Level_6","Subj","Error",LevelHelp[5],1024,LEVELLABEL);
		GetPrivateProfileString("Level_7","Subj","Error",LevelHelp[6],1024,LEVELLABEL);
		GetPrivateProfileString("Level_8","Subj","Error",LevelHelp[7],1024,LEVELLABEL);
		GetPrivateProfileString("Level_9","Subj","Error",LevelHelp[8],1024,LEVELLABEL);
		GetPrivateProfileString("Level_10","Subj","Error",LevelHelp[9],1024,LEVELLABEL);
		GetPrivateProfileString("Level_11","Subj","Error",LevelHelp[10],1024,LEVELLABEL);
		GetPrivateProfileString("Level_12","Subj","Error",LevelHelp[11],1024,LEVELLABEL);		
		GetPrivateProfileString("Level_15","Subj","Error",LevelHelp[12],1024,LEVELLABEL);		
		GetPrivateProfileString("Level_17","Subj","Error",LevelHelp[13],1024,LEVELLABEL);
		GetPrivateProfileString("Level_20","Subj","Error",LevelHelp[14],1024,LEVELLABEL);
		GetPrivateProfileString("Level_22","Subj","Error",LevelHelp[15],1024,LEVELLABEL);
		GetPrivateProfileString("Level_25","Subj","Error",LevelHelp[16],1024,LEVELLABEL);
		GetPrivateProfileString("Level_30","Subj","Error",LevelHelp[17],1024,LEVELLABEL);
		GetPrivateProfileString("Level_39","Subj","Error",LevelHelp[18],1024,LEVELLABEL);
		GetPrivateProfileString("Level_40","Subj","Error",LevelHelp[19],1024,LEVELLABEL);
		GetPrivateProfileString("Level_50","Subj","Error",LevelHelp[20],1024,LEVELLABEL);
		GetPrivateProfileString("Level_55","Subj","Error",LevelHelp[21],1024,LEVELLABEL);
		GetPrivateProfileString("Level_60","Subj","Error",LevelHelp[22],1024,LEVELLABEL);
		GetPrivateProfileString("Level_65","Subj","Error",LevelHelp[23],1024,LEVELLABEL);
		GetPrivateProfileString("Level_70","Subj","Error",LevelHelp[24],1024,LEVELLABEL);
		GetPrivateProfileString("Level_75","Subj","Error",LevelHelp[25],1024,LEVELLABEL);
		GetPrivateProfileString("Level_79","Subj","Error",LevelHelp[26],1024,LEVELLABEL);
		GetPrivateProfileString("Level_80","Subj","Error",LevelHelp[27],1024,LEVELLABEL);		
	}
	else
	{
		
		GetPrivateProfileString("Level_1","Subj","Error",LevelHelp[0],1024,LEVELLABEL);
		GetPrivateProfileString("Level_2","Subj","Error",LevelHelp[1],1024,LEVELLABEL);
		GetPrivateProfileString("Level_3","Subj","Error",LevelHelp[2],1024,LEVELLABEL);
		GetPrivateProfileString("Level_4","Subj","Error",LevelHelp[3],1024,LEVELLABEL);
		GetPrivateProfileString("Level_5","Subj","Error",LevelHelp[4],1024,LEVELLABEL);
		GetPrivateProfileString("Level_6","Subj","Error",LevelHelp[5],1024,LEVELLABEL);
		GetPrivateProfileString("Level_7","Subj","Error",LevelHelp[6],1024,LEVELLABEL);
		GetPrivateProfileString("Level_8","Subj","Error",LevelHelp[7],1024,LEVELLABEL);
		GetPrivateProfileString("Level_9","Subj","Error",LevelHelp[8],1024,LEVELLABEL);
		GetPrivateProfileString("Level_10","Subj","Error",LevelHelp[9],1024,LEVELLABEL);
		GetPrivateProfileString("Level_11","Subj","Error",LevelHelp[10],1024,LEVELLABEL);
		GetPrivateProfileString("Level_12","Subj","Error",LevelHelp[11],1024,LEVELLABEL);		
		GetPrivateProfileString("Level_15","Subj","Error",LevelHelp[12],1024,LEVELLABEL);		
		GetPrivateProfileString("Level_17","Subj","Error",LevelHelp[13],1024,LEVELLABEL);
		GetPrivateProfileString("Level_20","Subj","Error",LevelHelp[14],1024,LEVELLABEL);
		GetPrivateProfileString("Level_22","Subj","Error",LevelHelp[15],1024,LEVELLABEL);
		GetPrivateProfileString("Level_25","Subj","Error",LevelHelp[16],1024,LEVELLABEL);
		GetPrivateProfileString("Level_30","Subj","Error",LevelHelp[17],1024,LEVELLABEL);
		GetPrivateProfileString("Level_39","Subj","Error",LevelHelp[18],1024,LEVELLABEL);
		GetPrivateProfileString("Level_40","Subj","Error",LevelHelp[19],1024,LEVELLABEL);
		GetPrivateProfileString("Level_50","Subj","Error",LevelHelp[20],1024,LEVELLABEL);
		GetPrivateProfileString("Level_55","Subj","Error",LevelHelp[21],1024,LEVELLABEL);
		GetPrivateProfileString("Level_60","Subj","Error",LevelHelp[22],1024,LEVELLABEL);
		GetPrivateProfileString("Level_65","Subj","Error",LevelHelp[23],1024,LEVELLABEL);
		GetPrivateProfileString("Level_70","Subj","Error",LevelHelp[24],1024,LEVELLABEL);
		GetPrivateProfileString("Level_75","Subj","Error",LevelHelp[25],1024,LEVELLABEL);
		GetPrivateProfileString("Level_79","Subj","Error",LevelHelp[26],1024,LEVELLABEL);
		GetPrivateProfileString("Level_80","Subj","Error",LevelHelp[27],1024,LEVELLABEL);		
	}
/*
	if(sinChar->JOB_CODE < 5) // �۽�ũ�а� ����̾��� �����ΰ� ���� by teze
	{		
		strcpy(LevelHelp[0],"�������� ���Ͽ� ���Ű��� ȯ���մϴ�");
		strcpy(LevelHelp[1],"����Ʈ �й�� �̷���");
		strcpy(LevelHelp[2],"���� ���� ����ϱ�");
		strcpy(LevelHelp[3],"�̴ϸ� ����");
		strcpy(LevelHelp[4],"2������ ��� �����ϱ�");
		strcpy(LevelHelp[5],"���� �ϴ� ���");
		strcpy(LevelHelp[6],"�ڵ� ����");
		strcpy(LevelHelp[7],"���� ����Ʈ �̿���");
		strcpy(LevelHelp[8],"Ưȭ������");
		strcpy(LevelHelp[9],"��ų ����");		
		strcpy(LevelHelp[10],"���� ���� �ý���");//HoDong //11����
		strcpy(LevelHelp[11],"�������� ���������� ������,�ͽ�ó �ý���"); //12����
		strcpy(LevelHelp[12],"������ ���� ������ ���� ��� ����¡ �ý���");
		strcpy(LevelHelp[13],"��θ� �ϳ��� �����ִ� Ŭ���ý���");	
		strcpy(LevelHelp[14],"���� ������ ��ų�� ��� �� �ִ� �����ý���");
		strcpy(LevelHelp[15],"���� ���� ���� �׺���");
		strcpy(LevelHelp[16],"�÷��� ž(����Ʈ��)�� �����غ���");
		strcpy(LevelHelp[17],"���� ���縸�� �� �� �ִ� ���� ���ѱ���");
		strcpy(LevelHelp[18],"���� 40�� �̷���.");
		strcpy(LevelHelp[19],"��밨�� 1���� ������ Ŭ���� ������");
		strcpy(LevelHelp[20],"��밨�� 2���� ������");
		strcpy(LevelHelp[21],"���� ����� �縷�� ������ Ž������");
		strcpy(LevelHelp[22],"��밨�� 3���� ������");
		strcpy(LevelHelp[23],"����� ������ �Ա��� ���ȴ�");
		strcpy(LevelHelp[24],"����� ���� 1���� ������");
		strcpy(LevelHelp[25],"����� ���� 2���� ������");
		strcpy(LevelHelp[26],"������� ȥ���� ö��");//HoDong
		strcpy(LevelHelp[27],"ȥ���� ö�η�, �׸��� �ٺ��� ��������");//by teze
	}
	else
	{
		
		strcpy(LevelHelp[0],"�������� ���Ͽ� ���Ű��� ȯ���մϴ�");
		strcpy(LevelHelp[1],"����Ʈ �й�� �̷���");
		strcpy(LevelHelp[2],"���� ���� ����ϱ�");
		strcpy(LevelHelp[3],"�̴ϸ� ����");
		strcpy(LevelHelp[4],"2������ ��� �����ϱ�");
		strcpy(LevelHelp[5],"���� �ϴ� ���");
		strcpy(LevelHelp[6],"�ڵ� ����");
		strcpy(LevelHelp[7],"���� ����Ʈ �̿���");
		strcpy(LevelHelp[8],"Ưȭ������");
		strcpy(LevelHelp[9],"��ų ����");		
		strcpy(LevelHelp[10],"���� ���� �ý���");//HoDong //11����
		strcpy(LevelHelp[11],"�������� ���������� ������,�ͽ�ó �ý���"); //12����
		strcpy(LevelHelp[12],"������ ���� ������ ���� ��� ����¡ �ý���");
		strcpy(LevelHelp[13],"��θ� �ϳ��� �����ִ� Ŭ���ý���");	
		strcpy(LevelHelp[14],"���� ������ ��ų�� ��� �� �ִ� �����ý���");
		strcpy(LevelHelp[15],"���� ���� ���� �׺���");
		strcpy(LevelHelp[16],"�÷��� ž(����Ʈ��)�� �����غ���");
		strcpy(LevelHelp[17],"���� ���縸�� �� �� �ִ� ���� ���ѱ���");
		strcpy(LevelHelp[18],"���� 40�� �̷���.");
		strcpy(LevelHelp[19],"��밨�� 1���� ������ Ŭ���� ������");
		strcpy(LevelHelp[20],"��밨�� 2���� ������");
		strcpy(LevelHelp[21],"���� ����� �縷�� ������ Ž������");
		strcpy(LevelHelp[22],"��밨�� 3���� ������");
		strcpy(LevelHelp[23],"����� ������ �Ա��� ���ȴ�");
		strcpy(LevelHelp[24],"����� ���� 1���� ������");
		strcpy(LevelHelp[25],"����� ���� 2���� ������");
		strcpy(LevelHelp[26],"������� ȥ���� ö��");//HoDong
		strcpy(LevelHelp[27],"ȥ���� ö�η�, �׸��� �ٺ��� ��������");//by teze
	}
*/
}



void CGuideHelp::CheckLevel(int level)
{
	if(level == 1)
		nLevelHelpCount = 1;
	
	if(level == 2)
		nLevelHelpCount = 2;
	
	if(level == 3)
		nLevelHelpCount = 3;
	
	if(level == 4)
		nLevelHelpCount = 4;
	
	if(level == 5)
		nLevelHelpCount = 5;
	
	if(level == 6)
		nLevelHelpCount = 6;
	
	if(level == 7)
		nLevelHelpCount = 7;
	
	if((level == 8)||(level==9))
		nLevelHelpCount = 8;
	
	if(level == 9)
		nLevelHelpCount = 9;
	
	if(level == 10)
		nLevelHelpCount = 10;
	
	if(level == 11)
		nLevelHelpCount = 11;

	if((level >=12) &&(level < 15))
		nLevelHelpCount = 12;
	
	if((level == 15) ||(level==16))
		nLevelHelpCount = 13;

	if((level >=17) &&(level < 20))
		nLevelHelpCount = 14;

	if((level == 20) ||(level==21))
	{
		//if(sinChar->JOB_CODE <5)
		nLevelHelpCount = 15;
	}
	if((level >=22) &&(level < 25))
		nLevelHelpCount = 16;		
	
	if((level >=25) &&(level < 30))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 17;
		}
		else
		{
			nLevelHelpCount = 17;
		}
	}
	if((level >=30) &&(level < 40))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 18;
		}
		else
		{
			nLevelHelpCount = 18;
		}
	}

	if((level >=39))
		nLevelHelpCount = 19;
	
	if((level >=40) &&(level < 50))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 20;
		}
		else
		{
			nLevelHelpCount = 20;
		}
	}
	if((level >=50) &&(level < 55))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 21;
		}
		else
		{
			nLevelHelpCount = 21;
		}
	}
	if((level >=55) &&(level < 60))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 22;
		}
		else
		{
			nLevelHelpCount = 22;
		}
	}
	if((level >=60) &&(level < 65))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 23;
		}
		else
		{
			nLevelHelpCount = 23;
		}
	}
	if((level >=65) &&(level < 70))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 24;
		}
		else
		{
			nLevelHelpCount = 24;
		}
	}
	/*
	if((level >=70) &&(level < 80))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 24;
		}
		else
		{
			nLevelHelpCount = 24;
		}
	}*/
	//by teze
	if((level >=70) &&(level < 75))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 25;
		}
		else
		{
			nLevelHelpCount = 25;
		}
	}
	//by teze
	if((level >= 75) &&(level < 79))
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 26;
		}
		else
		{
			nLevelHelpCount = 26;
		}
	}

	if(level == 79)
		nLevelHelpCount = 27;

	if(level >= 80)
	{
		if(sinChar->JOB_CODE <5)
		{
			nLevelHelpCount = 28;
		}
		else
		{
			nLevelHelpCount = 28;
		}
	}

	//���ܰ� �߻��� �����ع����� �����. ���� ������ ����� ���
	if(nLevelHelpCount > LEVELHELP_COUNT_MAX)
	{
		nHelpCountNamugi = nLevelHelpCount - LEVELHELP_COUNT_MAX;
	}
}

void CGuideHelp::GetHelpContent(char* section,char* key,char* ImageKey)
{
	DWORD dwSaveSize = 100;
	char  szSaveStr[100];
	char  szImageDir[100];
	DWORD dwReturn,dwFileSize=0,dwRead = 0;
	HANDLE hFile = NULL;
	char *HelpContent;

	ZeroMemory(m_HelpContent,sizeof(m_HelpContent));
	ZeroMemory(szImageDir,sizeof(szImageDir));

	dwReturn = GetPrivateProfileString(section,key,"Error",szSaveStr,1024,HELP_CONTENT);

	if(dwReturn == 0) return;	
	dwReturn = GetPrivateProfileString(section,ImageKey,"Error",szImageDir,1024,HELP_CONTENT);
	if(dwReturn == 0) return;

	LoadGuideImage(szImageDir);

	hFile = CreateFile( szSaveStr, GENERIC_READ, FILE_SHARE_READ,
                        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL );

	if(hFile == INVALID_HANDLE_VALUE) return ;

	dwFileSize = GetFileSize(hFile,NULL);

	HelpContent = new char[dwFileSize];
	ZeroMemory(HelpContent,sizeof(char)*dwFileSize);

	ReadFile(hFile,HelpContent,dwFileSize,&dwRead,NULL);
	CloseHandle(hFile);

	Parsing(HelpContent);

	delete[] HelpContent;	

}

void CGuideHelp::Parsing(char* str)
{
	int nCnt,nCnt2,nCnt3;
	nCnt=nCnt2=nCnt3 = 0;
	nHeight_Count = 0;
	while(1)
	{
		if(str[nCnt3] == 0) break;
		
		if(str[nCnt3] == 0x0d)
		{
			nCnt3++;
			if(str[nCnt3] == 0) break;
			if(str[nCnt3] ==0x0a)
			{
				if(str[nCnt3+1] == 0x0d) break;
				else
				{
					nCnt3++;
					nCnt++;
					nCnt2 = 0;
				}
			}
			else
			{
				nCnt++;
				nCnt2 = 0;
				m_HelpContent[nCnt][nCnt2] = str[nCnt3];
			}
			
			continue;
		}
		else
		{
			m_HelpContent[nCnt][nCnt2] = str[nCnt3];
		}
		nCnt3++;
		nCnt2++;
	}

	nHeight_Count = nCnt+1;
	if(nHeight_Count > GUIDEHLEP_CONTENT_HEIGHT_MAX)
		nSaveNamugi = nHeight_Count - GUIDEHLEP_CONTENT_HEIGHT_MAX;

	
	
}

void CGuideHelp::CheckLevel(int level,int type)
{	

	switch(level)
	{
	case 1: //1~4  �۽�ũ��, 5~8 ����̾�
		if(type <5)
		{
			//�۽�ũ�п��� ĳ���� ó�� �����Ҷ�
			GetHelpContent("CreateCha_Temscron","Key0","Guide1");
		}
		else
		{
			//����̾𿡼� ĳ���� ó�� �����Ҷ�
			GetHelpContent("CreateCha_Moraion","Key1","Guide2");
		}
		break;
	case 2: //����2
		GetHelpContent("Level_2","Key2","Guide3");
		
		break;
	case 3:
		GetHelpContent("Level_3","Key3","Guide4");
		
		break;
	case 4:
		GetHelpContent("Level_4","Key4","Guide5");
		break;
	case 5:
		GetHelpContent("Level_5","Key5","Guide6");
		break;
	case 6:
		GetHelpContent("Level_6","Key6","Guide7");
		
		break;
	case 7:
		GetHelpContent("Level_7","Key24","Guide26");
		
		break;
	case 8:
		GetHelpContent("Level_8","Key7","Guide8");
		break;
	case 9:
		GetHelpContent("Level_9","Key25","Guide27");
		break;
	case 10:
		
		if(type <5)
		{
			//�۽�ũ��
			GetHelpContent("Level_10_Temscron","Key8","Guide9");
		}
		else
		{
			//����̾�
			GetHelpContent("Level_10_Moraion","Key9","Guide10");
		}
		break;

	//by HoDong
	case 11:
		GetHelpContent("Level_11","Key11","Guide11");
		break;

	case 12:
		
		GetHelpContent("Level_12","Key10","Guide11");
		break;
	case 15:
		
		GetHelpContent("Level_15","Key11","Guide12");
		break;
	case 17:
		GetHelpContent("Level_17","Key12","Guide13");
		break;
	case 20:
		
		if(type<5)
		{		
			GetHelpContent("Level_20_Temscron","Key13","Guide14");
		}
		else
		{
			GetHelpContent("Level_20_Moraion","Key24","Guide15");
		}
		break;
	case 22:
		
		GetHelpContent("Level_22","Key14","Guide16");
		break;
	case 25:
		
		GetHelpContent("Level_25","Key15","Guide17");
		break;
	case 30:
		
		GetHelpContent("Level_30","Key16","Guide18");
		break;
	case 39:
		GetHelpContent("Level_39","Key26","Guide26");
		break;
	case 40:
		
		GetHelpContent("Level_40","Key17","Guide19");
		break;
	case 50:
		
		GetHelpContent("Level_50","Key18","Guide20");
		break;
	case 55:
		GetHelpContent("Level_55","Key19","Guide21");
		break;
	case 60:
		GetHelpContent("Level_60","Key20","Guide22");
		break;
	case 65:
		
		GetHelpContent("Level_65","Key21","Guide23");
		break;
	case 70:
		
		GetHelpContent("Level_70","Key22","Guide24");
		break;
	//by teze
	case 75:
		GetHelpContent("Level_75","Key28","Guide24");
		break;
	//by HoDong
	case 79:
		GetHelpContent("Level_79","Key79","Guide79");
		break;

	case 80:
		GetHelpContent("Level_80","Key23","Guide25");
		break;
	}
	
}



void CGuideHelp::Draw_Help_Text(char *str, int x, int y, int sw)
{

	char strbuf[1024];

	HDC hdc;
	HFONT oldFont;
	renderDevice.lpDDSBack->GetDC(&hdc);
	SetBkMode( hdc, TRANSPARENT );

	if(hFont != NULL)
		oldFont = (HFONT)SelectObject( hdc , hFont );
	//if(sinBoldFont!= NULL)
	//	oldFont = (HFONT)SelectObject( hdc , sinBoldFont );


	ZeroMemory(strbuf, sizeof(strbuf));

	int strL = strlen(str);
	int cnt=0;
	int i=0;

	while(1) {
		

		strbuf[i] = str[cnt];
		cnt++;
		if(strbuf[i] == '\n')  {
			strbuf[i] =0;
			i=0;
jmp1:


			
			if(sw==2){
				SetTextColor( hdc, RGB(100, 100, 100));	//�׸��ڿ� ���ڻ�
				//SetTextColor( hdc, RGB(255, 255,255));	//�׸��ڿ� ���ڻ�
				dsTextLineOut(hdc, x+1, y+1, strbuf, lstrlen(strbuf));

				SetTextColor( hdc, RGB(255, 240, 200));	//�Ϲݱ��ڻ�
				dsTextLineOut(hdc, x, y, strbuf, lstrlen(strbuf));
			}
			else     {
				SetTextColor( hdc, RGB(255, 240, 200));	//�Ϲݱ��ڻ�
				dsTextLineOut(hdc, x, y, strbuf, lstrlen(strbuf));
			}
			
			/*
			if(sw==2){
				SetTextColor( hdc, RGB(255, 255, 255));	//Ŭ���̸��Է½� ���				
			}
			else {
				SetTextColor( hdc, RGB(230, 255, 240));	//�Ϲݱ��ڻ�
			}
			dsTextLineOut(hdc, x, y, strbuf, lstrlen(strbuf));
			*/
			

			ZeroMemory(strbuf, sizeof(strbuf));
			y+=16;

			if(cnt>=strL) break;
		}
		else if(cnt>=strL)
		{
			goto jmp1;
		}
		else {
			i++;
		}
	}
	

	if(sw==1) 
	{
		static cnt=0;
		cnt++;
		if(cnt<20) {
			SetTextColor( hdc, RGB(255, 255, 255));
			dsTextLineOut(hdc, x + (lstrlen(str)*6) , y-16, "|", 1);
		}
		if(cnt>=40) cnt=0;
	}

	if(oldFont)
		SelectObject( hdc, oldFont);
	renderDevice.lpDDSBack->ReleaseDC(hdc);

}

void CGuideHelp::Draw_Help_Text2(char *str, int x, int y, int sw)
{

	char strbuf[1024];

	HDC hdc;
	HFONT oldFont;
	renderDevice.lpDDSBack->GetDC(&hdc);
	SetBkMode( hdc, TRANSPARENT );

	if(hFont != NULL)
		oldFont = (HFONT)SelectObject( hdc , hFont );
	//if(sinBoldFont!= NULL)
	//	oldFont = (HFONT)SelectObject( hdc , sinBoldFont );


	ZeroMemory(strbuf, sizeof(strbuf));

	int strL = strlen(str);
	int cnt=0;
	int i=0;

	while(1) {
		

		strbuf[i] = str[cnt];
		cnt++;
		if(strbuf[i] == '\n')  {
			strbuf[i] =0;
			i=0;
jmp1:


			
			if(sw==2){
				SetTextColor( hdc, RGB(100, 100, 100));	//�׸��ڿ� ���ڻ�
				//SetTextColor( hdc, RGB(255, 255,255));	//�׸��ڿ� ���ڻ�
				dsTextLineOut(hdc, x+1, y+1, strbuf, lstrlen(strbuf));

				SetTextColor( hdc, RGB(255, 200, 100));	//�Ϲݱ��ڻ�
				dsTextLineOut(hdc, x, y, strbuf, lstrlen(strbuf));
			}
			else     {
				SetTextColor( hdc, RGB(255, 200, 100));	//�Ϲݱ��ڻ�
				dsTextLineOut(hdc, x, y, strbuf, lstrlen(strbuf));
			}
			
			/*
			if(sw==2){
				SetTextColor( hdc, RGB(255, 255, 255));	//Ŭ���̸��Է½� ���				
			}
			else {
				SetTextColor( hdc, RGB(230, 255, 240));	//�Ϲݱ��ڻ�
			}
			dsTextLineOut(hdc, x, y, strbuf, lstrlen(strbuf));
			*/
			

			ZeroMemory(strbuf, sizeof(strbuf));
			y+=16;

			if(cnt>=strL) break;
		}
		else if(cnt>=strL)
		{
			goto jmp1;
		}
		else {
			i++;
		}
	}
	

	if(sw==1) 
	{
		static cnt=0;
		cnt++;
		if(cnt<20) {
			SetTextColor( hdc, RGB(255, 255, 255));
			dsTextLineOut(hdc, x + (lstrlen(str)*6) , y-16, "|", 1);
		}
		if(cnt>=40) cnt=0;
	}

	if(oldFont)
		SelectObject( hdc, oldFont);
	renderDevice.lpDDSBack->ReleaseDC(hdc);

}

void CGuideHelp::Draw_Title_Text(char *str, int x, int y, int sw)
{

	char strbuf[1024];

	HDC hdc;
	HFONT oldFont;
	renderDevice.lpDDSBack->GetDC(&hdc);
	SetBkMode( hdc, TRANSPARENT );

	//if(hFont != NULL)
		//oldFont = (HFONT)SelectObject( hdc , hFont );
	if(sinBoldFont!= NULL)
		oldFont = (HFONT)SelectObject( hdc , sinBoldFont );


	ZeroMemory(strbuf, sizeof(strbuf));

	int strL = strlen(str);
	int cnt=0;
	int i=0;

	while(1) {
		

		strbuf[i] = str[cnt];
		cnt++;
		if(strbuf[i] == '\n')  {
			strbuf[i] =0;
			i=0;
jmp1:


			
			if(sw==2){
				SetTextColor( hdc, RGB(100, 100, 100));	//�׸��ڿ� ���ڻ�
				//SetTextColor( hdc, RGB(255, 255,255));	//�׸��ڿ� ���ڻ�
				dsTextLineOut(hdc, x+1, y+1, strbuf, lstrlen(strbuf));

				SetTextColor( hdc, RGB(255, 240, 200));	//�Ϲݱ��ڻ�
				dsTextLineOut(hdc, x, y, strbuf, lstrlen(strbuf));
			}
			else     {
				SetTextColor( hdc, RGB(255, 240, 200));	//�Ϲݱ��ڻ�
				dsTextLineOut(hdc, x, y, strbuf, lstrlen(strbuf));
			}
			
			/*
			if(sw==2){
				SetTextColor( hdc, RGB(255, 255, 255));	//Ŭ���̸��Է½� ���				
			}
			else {
				SetTextColor( hdc, RGB(230, 255, 240));	//�Ϲݱ��ڻ�
			}
			dsTextLineOut(hdc, x, y, strbuf, lstrlen(strbuf));
			*/
			

			ZeroMemory(strbuf, sizeof(strbuf));
			y+=16;

			if(cnt>=strL) break;
		}
		else if(cnt>=strL)
		{
			goto jmp1;
		}
		else {
			i++;
		}
	}
	

	if(sw==1) 
	{
		static cnt=0;
		cnt++;
		if(cnt<20) {
			SetTextColor( hdc, RGB(255, 255, 255));
			dsTextLineOut(hdc, x + (lstrlen(str)*6) , y-16, "|", 1);
		}
		if(cnt>=40) cnt=0;
	}

	if(oldFont)
		SelectObject( hdc, oldFont);
	renderDevice.lpDDSBack->ReleaseDC(hdc);

}




int CGuideHelp::dsDrawTexImageFloat( int Mat , float x, float y, float w, float h , int Transp , int specular )
{
	int clipLeft;
	int clipTop;
	int clipRight;
	int clipBottom;
	
	clipLeft = 0;
	clipTop = 0;
	clipRight = WinSizeX;
	clipBottom = WinSizeY;
	
	//Clipping ������ ������ �����...
	if ((x >= clipRight) || (y >= clipBottom) || ((x + w) <= clipLeft) || ((y+h) <= clipTop))
		return FALSE;
	
	//���� �簢 ������ ũ��� 0, 0, 127, 127 ������ �̷��� ���� ���. �� �ȼ��� ������ 0, 0, 128, 128�� �����Ѵ�.
	float x1 = x;
	float y1 = y;
	float x2 = (x+w);
	float y2 = (y+h);
	
	/*
	if(x1 < clipLeft)
		x1 = clipLeft;
	if(y1 < clipTop)
		y1 = clipTop;

	if(x2 >= clipRight)
		x2 = clipRight-1;

	if(y2 >= clipBottom)
		y2 = clipBottom-1;
	*/

	//x1, y1, x2, y2 �� Ŭ���� �簢�����̴�..
	D3DTLVERTEX tlVertex[4];
	int cnt;

	for(cnt=0;cnt<4;cnt++)
	{
		tlVertex[cnt].rhw = 1.f;
		tlVertex[cnt].color	= RGBA_MAKE( 255,255,255,Transp );
		tlVertex[cnt].specular  =  RGBA_MAKE( specular,specular,specular,0 );
		tlVertex[cnt].sz = 0;
	}
	/*
	tlVertex[0].sx = x1;
	tlVertex[0].sy = y1;
	tlVertex[0].tu = 1.0f*( (x1-(float)x)/w);
	tlVertex[0].tv = 1.0f*( (y1-(float)y)/h);
	
	tlVertex[1].sx = x1;
	tlVertex[1].sy = y2;
	tlVertex[1].tu = 1.0f*( (x1-(float)x)/w);
	tlVertex[1].tv = 1.0f*( (y2-(float)y)/h);
	
	tlVertex[2].sx = x2;
	tlVertex[2].sy = y1;
	tlVertex[2].tu = 1.0f*( (x2-(float)x)/w);
	tlVertex[2].tv = 1.0f*( (y1-(float)y)/h);
	
	tlVertex[3].sx = x2;
	tlVertex[3].sy = y2;
	tlVertex[3].tu = 1.0f*( (x2-(float)x)/w);
	tlVertex[3].tv = 1.0f*( (y2-(float)y)/h);
	*/
	
	//�̰� ��¥...
	/*
	tlVertex[0].sx = x1;
	tlVertex[0].sy = y1;
	tlVertex[0].tu = 1.0f*(float)( (x1-(float)x)/(float)w);
	tlVertex[0].tv = 1.0f*(float)( (y1-(float)y)/(float)h);
	
	tlVertex[1].sx = x1;
	tlVertex[1].sy = y2;
	tlVertex[1].tu = 1.0f*(float)( (x1-(float)x)/(float)w);
	tlVertex[1].tv = 1.0f*(float)( (y2-(float)y)/(float)h);
	
	tlVertex[2].sx = x2;
	tlVertex[2].sy = y1;
	tlVertex[2].tu = 1.0f*(float)( (x2-(float)x)/(float)w);
	tlVertex[2].tv = 1.0f*(float)( (y1-(float)y)/(float)h);
	
	tlVertex[3].sx = x2;
	tlVertex[3].sy = y2;
	tlVertex[3].tu = 1.0f*(float)( (x2-(float)x)/(float)w);
	tlVertex[3].tv = 1.0f*(float)( (y2-(float)y)/(float)h);
	*/

	
	tlVertex[0].sx = x1;
	tlVertex[0].sy = y1;
	tlVertex[0].tu = 0.0f;
	tlVertex[0].tv = 0.0f;
	
	tlVertex[1].sx = x1;
	tlVertex[1].sy = y2;
	tlVertex[1].tu = 0.0f;
	tlVertex[1].tv = 1.0f;
	
	tlVertex[2].sx = x2;
	tlVertex[2].sy = y1;
	tlVertex[2].tu = 1.0f;
	tlVertex[2].tv = 0.0f;
	
	tlVertex[3].sx = x2;
	tlVertex[3].sy = y2;
	tlVertex[3].tu = 1.0f;
	tlVertex[3].tv = 1.0f;
	
	/*
	tlVertex[0].sx = x;
	tlVertex[0].sy = y;
	tlVertex[0].tu = 0;
	tlVertex[0].tv = 0;

	tlVertex[1].sx = x;
	tlVertex[1].sy = y+h;
	tlVertex[1].tu = 0;
	tlVertex[1].tv = 1;

	tlVertex[2].sx = x+w;
	tlVertex[2].sy = y;
	tlVertex[2].tu = 1;
	tlVertex[2].tv = 0;

	tlVertex[3].sx = x+w;
	tlVertex[3].sy = y+h;
	tlVertex[3].tu = 1;
	tlVertex[3].tv = 1;
	*/

	//�������� �׸���
	//lpD3DDevice->SetRenderState( D3DRENDERSTATE_CULLMODE , D3DCULL_NONE );
	if (renderDevice.IsDevice() != NULL)
	{
		  // Turn on Z-buffering
		renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
		renderDevice.SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
		//renderDevice.SetRenderState( D3DRENDERSTATE_ZFUNC,   D3DCMP_LESSEQUAL );
		//renderDevice.SetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_USEW );//TRUE );


		// null out the texture handle
		renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, 0);

		// turn on dithering
		//renderDevice.D3DDevice->SetRenderState( D3DRENDERSTATE_DITHERENABLE, TRUE );

		renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_WRAP);
		//renderDevice.SetRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE ); 

		// D3DFILTER_LINEAR
		//renderDevice.SetRenderState( D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_LINEAR );
		//renderDevice.SetRenderState( D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEAR );
 
		renderDevice.SetRenderState(D3DRENDERSTATE_SPECULARENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);

		renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE);

		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);

		smRender.AlphaTestDepth = 60;

		renderDevice.SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_POINT);
		renderDevice.SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_POINT);

		//renderDevice.SetRenderState( D3DRENDERSTATE_TEXTUREMIN, D3DTFN_POINT );
		//renderDevice.SetRenderState( D3DRENDERSTATE_TEXTUREMAG, D3DTFN_POINT );
		

		if(GuideImage->smMaterial[Mat].smTexture[0] != NULL)
		{
			if(GuideImage->smMaterial[Mat].smTexture[0]->lpD3DTexture != NULL)
			{
				if (FAILED(renderDevice.SetTexture(0, GuideImage->smMaterial[Mat].smTexture[0]->lpD3DTexture)))
				{
					OutputDebugString("SetTexture Error");
					return FALSE;
				}

				
				if (FAILED(renderDevice.DrawPrimitive(D3DPT_TRIANGLESTRIP,
								D3DFVF_TLVERTEX  ,
								tlVertex,
								4,
								NULL)))
				{
					OutputDebugString("DrawPrimitive Error");
					return FALSE;
				}
			}
		}
		
		renderDevice.SetTextureStageState(cnt, D3DTSS_MINFILTER, D3DTFN_LINEAR);
		renderDevice.SetTextureStageState(cnt, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
		//renderDevice.D3DDevice->SetRenderState( D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_WRAP);
		//renderDevice.SetRenderState( D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEAR );
		//renderDevice.SetRenderState( D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_LINEAR );
	}	
	//renderDevice.SetRenderState( D3DRENDERSTATE_CULLMODE , D3DCULL_CW );

	return TRUE;
}

void CGuideHelp::GuidHelpEnd()
{
	IsDrawGuideHelp = FALSE;
	m_GuideHelpN = -1;
	bDoing = FALSE;
	isDrawClanMenu = 0;
	TJBscrollWheelClear();
	TJBscrollWheelClear_Guide();
}


