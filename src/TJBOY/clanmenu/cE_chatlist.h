#pragma once
#ifndef __CE_CHATTING_HPP__
#define __CE_CHATTING_HPP__

#include <windows.h>
#include "clan_Enti.h"

//=========================================================
#define BOX_X                 656
#define MOVE_X                 400
#define MOVE_Y                 0



#ifdef USE_PROSTONTALE		//=========================================================

#define CHATING_DISPLAY_MAX       8

#define CHATING_CLANWON_LIST_X    620
#define CHATING_CLANWON_LIST_Y    204 //197
#define CHATING_CLANWON_LIST_CAP  20



#define CHATICON_X               625//( 90 + MOVE_X)  //ä�þ����� ǥ�� top          
#define CHATCLOSE_X              740//(200 + MOVE_X)
#define CHATICON_Y               375//(230 + MOVE_Y)


#define CHATLIST_SBAR_X	         770//(225 + MOVE_X)
#define CHATLIST_SBAR_Y           193//(45 + MOVE_Y)
#define CHATLIST_MENU                     150

#else						//=========================================================

#define CHATING_DISPLAY_MAX       8
#define CHATING_CLANWON_LIST_X    620
#define CHATING_CLANWON_LIST_Y    130
#define CHATING_CLANWON_LIST_CAP   23

#define CHATICON_X               640//( 90 + MOVE_X)  //ä�þ����� ǥ�� top          
#define CHATCLOSE_X              730//(200 + MOVE_X)
#define CHATICON_Y               325//(230 + MOVE_Y)
#define CHATLIST_SBAR_X	         770//(225 + MOVE_X)
#define CHATLIST_SBAR_Y           140//(45 + MOVE_Y)
#define CHATLIST_MENU                     105

#endif						//=========================================================

//====================================================================
#define CHATLIST_CLANCHIP                 1
#define CHATLIST_CLANUSER				2
#define CHATLIST_CLANNO_REGESTER			3				
#define CHATLIST_BOX_Y					147







//#define CLANWON_MAX 100
typedef struct _tagCHATLIST
{ 
	int MyPositon;              //Ŭ��������, Ŭ��������, Ŭ���̵������ üũ
	char  clanWon[CLANWON_MAX][30];        //Ŭ���������� ������ Ŭ������ ����(Ŭ���� ����)
	char  szUserID[40];           //����� ���̵�
	char  szGameServer[40];        //������ ������
	char  szChaName[40];           //����� ĳ���� �̸�
	char  szClanZang_Name[40];
}CHATLIST;

class CLANEntity_chatting: public CLANEntity
{
public:

	//ktj : �ӽ�
	void chat_WhisperPartyPlayer2(int num);
	void chat_WhisperPartyPlayer(int num);
	void chat_WhisperPartyPlayer_close();

	char szFile_hMark[128];

	CLANEntity_chatting();
	~CLANEntity_chatting();

	int Init(int Life);
	void Close();


	void Main();
	void Draw();
	void InitClanWon();
	void InitClanChatList();
	void MoveClose();


	void menuInit(int num);
	void SortClanWonList();
	void Load();
	void Load2();		//2��°�ε��
	
	
	//void OpenDrawBox();
	//void CloseDrawBox();
	//void CloseDraw();



	
	void DrawBox();
	void Draw_C_menuN();
	void NoScroll();

	int  chkMenu(int x, int y, RECT *r);
	int  chkeckMenuN();
	void DoubleClick(int i);


	//void DrawChatList();


	void ChatListParsigClanWon(char* data);
	void ChatListUserData(char *id, char *gserver, char *chaid);
	void ChatListParsingIsClanMember(char* data);
	void SetBoxDraw(BOOL bFlag){bIsBox = bFlag;}
	void SetOpenChatList(BOOL bFlag){bIsClose = bFlag;}
	void DisplayInfo();
public:
#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
	int	hChatListBoxLine;	//���

	int	hChatListBoxTitle2;

	//DIRECTDRAWSURFACE	hChatListBoxLine; //Ȯ��
	DIRECTDRAWSURFACE	hChatListBoxTitle;
	DIRECTDRAWSURFACE	hChatIcon[2];
	DIRECTDRAWSURFACE	hButtonClose[2];
	DIRECTDRAWSURFACE	hClanWon[2];
	DIRECTDRAWSURFACE	hChatClanWon;
	//��öȣ
#ifdef PARKMENU
	DIRECTDRAWSURFACE	hMicOption[2];
	DIRECTDRAWSURFACE	hMicOptToolTip;
    int	hChatMicOpt;	// ����ũ �ɼ� ������
	DIRECTDRAWSURFACE	hMicOptTitle;	// ����ũ�ɼ� Ÿ��Ʋ �̹���
	DIRECTDRAWSURFACE	hMicSortLine;	// ����ũ�ɼ� ������
	DIRECTDRAWSURFACE	hMicONBack;	// ����ũon,off��׶���
	DIRECTDRAWSURFACE	hMicONChk;	// ����ũ onüũ�̹�
	DIRECTDRAWSURFACE	hMicVolBar;	// ����ũ�ɼ� ������
	DIRECTDRAWSURFACE	hMicVOLUPDOWN[2];	// ����ũ ���� up,downȰ��ȭ
	//int	hMicChatIcon;	// ����ũä��ǥ�� ������
#endif
	//��öȣ
	DIRECTDRAWSURFACE	hClanChatToolTip;
	DIRECTDRAWSURFACE	hBtnCloseToolTip;

#else
	HBITMAP  hChatListBoxLine;
	HBITMAP  hChatListBoxTitle;
	HBITMAP  hChatIcon[2];
	HBITMAP  hButtonClose[2];
	HBITMAP  hClanWon[2];
	HBITMAP hChatListParty;
	HBITMAP hChatListFriend;
	HBITMAP hChatClanWon;
	//��öȣ
#ifdef PARKMENU
	HBITMAP	hMicOption[2];
	HBITMAP	hMicOptTitle;	// ����ũ�ɼ� Ÿ��Ʋ �̹���
	HBITMAP	hMicSortLine;	// ����ũ�ɼ� ������
	HBITMAP	hMicONBack;	// ����ũon,off��׶���
	HBITMAP	hMicONChk;	// ����ũ onüũ�̹�
	HBITMAP	hMicVolBar;	// ����ũ�ɼ� ������
	HBITMAP	hMicVOLUPDOWN[2];	// ����ũ ���� up,downȰ��ȭ
	HBITMAP	hMicOptToolTip;
#endif
	HBITMAP	hClanChatToolTip;
	HBITMAP	hBtnCloseToolTip;
#endif
	
	int nScl_Bar;           //��ũ�ѹ�
	RECT scrIconPos;		//��ũ�ѹ��� �����̴¾�������ġ����
	int nScl_Save;
	int nTotalClanWon;
	int nRest_Num;
	CHATLIST   ClanChatList;
	BOOL bIsBox;
	char szErrorMsg[255];
	int nBoxMove;
	BOOL bIsClose;
	BOOL bIsDrawBoxFinish;	
	int OCcnt;
	int ChatListBoxX;
	int ChatListBoxY;

	int cnt;
	int AddSize;
	RECT ChatList_menuPos[30];		//�޴�����.
	int ChatList_menuSu;
	
	//==================================================
	//
	int Chatting_button;			//����Ŀ on,off
	
	//��öȣ
#ifdef PARKMENU
	int MicOpt_button;			// ����ũ �ɼ� over, down
	int nVol;			// ����ũ ������ ��
#endif
	//==================================================

	int nSaveClanWonPos;
	int nDoublClick;
	
};


#endif 





#ifdef CE_CHATLIST_CPP

	class CLANEntity_chatting cClanChatting;

#else

	extern class CLANEntity_chatting cClanChatting;

#endif
