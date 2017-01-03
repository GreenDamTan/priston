#pragma once

#include "tjclanDEF.h"

#ifndef __CLAN_VIEWCLANINFO_HPP__
#define __CLAN_VIEWCLANINFO_HPP__



#define CLAN_MAX		500
//#define CLAN_MAX		4
#define CLANINFO_DIR	"savedata\\clanDATA"  //Ŭ�� ������ ����Ǵ� ���丮
typedef struct _tagClanInfoHeader
{
	char		ClanMark_Num[40];   //Ŭ�� ��ũ ��ȣ
	DWORD		nClanMark;
	char		ClanName[40];   //Ŭ�� �̸�		
	char		ClanReport[90];
	char		Gserver[4];
	char		UniqueClaClanInfo_nNum[64];
	
}CLANINFOHEADER;

typedef struct _tagClanInfo
{
	CLANINFOHEADER ClanInfoHeader;
	
#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
	DIRECTDRAWSURFACE    hClanMark;		//16*16���Ϸε�
	DIRECTDRAWSURFACE    hClanMark32;	//32*32���Ϸε�(sod���� �����ֱ�����)
#else
	HBITMAP					hClanMark;
#endif
	DWORD	dwTime;
	int		nStatus;   //0:�۾� ���� 1: �۾������� 2: �۾��Ϸ� 3: ����
	int		nSaveNum;

}CLANINFO;



class CViewClanInfo
{
public:
	CViewClanInfo();
	~CViewClanInfo();
public:
	int Init(int Life);
	int Clear();
	

	void Main();
	char* AddClanInfo(char *marknum,char *szInfo,CLANINFO* ci);
	int DownLoadClanInfo(int num,DWORD dwClanNum );
	int chk_readStrBuf(char *cmd, char *readbuf, char *retBuf);
	void SortDelete();
	void LoadData();
	BOOL SearchDirectory();
	void ReadToFile(char *file,int count);
	void ReadToImage(char *file,int count);
	void MakeDirectory();	
	BOOL CheckWhere(char *MarkNum);
	inline BOOL GetFlag(){return bFlag;}
	void ParsingData(char* cmd, char* data, char* save);
	int readFILE_ClanInfo(DWORD clanNum);
	//inline int  GetCheckDownLoad(){return nCheckDownLoad;}
	//inline void SetCheckDownLoad(int flag){nCheckDownLoad = flag;}
private:

	BOOL bFlag;
	

     
	int		nClanInfo_Menu;
	int		nClanInfo_Menu2;
	char	szClanMarkNum[64];
	int		nLowTime;
	



public:
	//Ŭ������ã���� �����Լ�
	int SearchClan(DWORD dwClanNum );
		//�����Լ� 1.�޸𸮿��� ã��
		int Search_Memory(char *clanNum);
		//�����Լ� 2.���Ͽ��� ã��
		int Search_Files(char *clanNum);

	
};
#endif









#ifdef CLAN_VIEWCLANINFO_CPP


	class CViewClanInfo g_ViewClanInfo;
	CLANINFO ClanInfo[CLAN_MAX]; //Ŭ�� ���� ����
	int	ClanInfo_nNum;	 //Ŭ�� ������ ����� ��ġ
	int		nDoCheck = 0;
	int	nCheckDownLoad = -1;



#else



extern int	nCheckDownLoad;
extern int		nDoCheck;

extern class CViewClanInfo g_ViewClanInfo;
extern CLANINFO ClanInfo[CLAN_MAX];
extern int ClanInfo_nNum;	


#endif