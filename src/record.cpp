
#include "smlib3d\\smd3d.h"

#include "smwsock.h"
#include "character.h"
#include "avictrl.h"
#include "playmain.h"
#include "Sound\\dxwav.h"
#include "fileread.h"

#include "particle.h"
#include "netplay.h"

#include "sinbaram\\sinlinkheader.h"
#include "hobaram\\holinkheader.h"

#include "record.h"
#include "playsub.h"

#include "field.h"
#include "Language\\language.h"

#include "TextMessage.h"
#include "Server\\onserver.h"


char	*szRecordHeader = "RC 1.50";			//���� ����ü
DWORD	dwRecordVersion = 150;					//��� ����ü ����

char	*szRecordUserDataDir = "userdata";
char	*szRecordUserBackupDataDir = "userdata_backup";
char	*szRecordUserInfoDir = "userinfo";
char	*szRecordWareHouseDir = "warehouse";
char	*szRecordDeleteDir = "deleted";
char	*szPostBoxDir = "PostBox";

sLAST_QUEST_INFO	RecordLastQuestInfo;			//���� ����Ʈ ����

//#define	CHAR_NAME_MAXLEN	(18+6)
#define	CHAR_NAME_MAXLEN	(16+2)

int	Permit_CheckMoney = TOTAL_CHECK_MONEY_MAX;
int	Permit_CheckExp = TOTAL_CHECK_EXP_MAX;

extern rsSERVER_CONFIG		rsServerConfig;				//���� ���� ����
extern DWORD				dwPlayServTime;				//���� �ð�
extern time_t				tServerTime;				//������ �ð� ( ���� ó���� )
extern int					Server_DebugCount;						//����� ó�� ī����

//ĳ���� ���� �ڵ带 ����
DWORD GetNewObjectSerial();
//��ŷ �õ� ��� ���Ϸ� ����
int RecordHackLogFile( rsPLAYINFO *lpPlayInfo , void *lpTransCommand );
//���� �ŷ����� Ȯ��
int	rsGetTradePotionInfo( rsPLAYINFO *lpPlayInfo , DWORD dwCode );
//����Ʈ Ƽ�� ������ Ư�� ������ �κ��丮����
int rsPutItem_PointTicket( rsPLAYINFO *lpPlayInfo , int Price );


/*
//������ ��
#define ITEM_CODER_MAX		1000000
#define COPY_ITEM_MAX		1000

struct	sITEM_CODE_CMP {
	DWORD	dwCode;
	DWORD	dwKey;
	DWORD	dwSum;

	DWORD	OpenCounter;
};

struct sITEMCODER {
	sITEM_CODE_CMP	sItemCode[ITEM_CODER_MAX];
	int sItemCodeCount;
	int CopyItemCount;
};

struct sCOPYITEM {
	int sItemCodeCount;
	sITEM_CODE_CMP sItemCode[COPY_ITEM_MAX];
};
*/

//extern HINSTANCE hinst; 
#include "resource.h"

static char LastAcessID[64];



//////////////////////////////////////////////////////////////////////////////////////////////
struct sREC_DATABUFF {
	rsPLAYINFO	*lpPlayInfo;
	DWORD		dwConnectCount;
	char		szName[32];
	char		szFileName[128];
	char		szBackupFileName[128];
	TRANS_RECORD_DATA	TransRecData;
};

#define	REC_DATABUFF_MAX		128
#define	REC_DATABUFF_MASK		127
#define	REC_DATABUFF_LIMIT		64

sREC_DATABUFF	*sRecDataBuff =0;
int				sRecDataBuffCount;
HANDLE			hRecThread =0;
DWORD			dwRecThreadId;
DWORD			dwLastRecDataTime = 0;

//ũ��Ƽ�� ����
CRITICAL_SECTION	cRecDataSection;				//���� ����� ũ��Ƽ�ü���
CRITICAL_SECTION	cSaveDataSection;				//�������� ũ��Ƽ�ü���

//����DB�� ����Ÿ ����䱸
int rsSaveRecData( TRANS_RECORD_DATA *lpTransRecordData , rsPLAYINFO *lpPlayInfo , 
				  char *szFileName , char *szBackupFileName );
//���� ������� ����Ÿ �ִ��� Ȯ��
int CheckRecWaitData( char *szName );

//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����
int	OpenStarPointEvent( rsPLAYINFO *lpPlayInfo , smCHAR_INFO *lpCharInfo );
//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����
int	CloseStarPointEvent( rsPLAYINFO *lpPlayInfo , smCHAR_INFO *lpCharInfo );
//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����
int	OpenStarPointTicket( rsPLAYINFO *lpPlayInfo );


////////////////////////////////////////////////////////////////////////////////////////////


int	EncodeFileName( char *szName , char *szDecodeName )
{
	int	len;
	int cnt;
	int cnt2;
	BYTE ch1,ch2;
	DWORD ch;

	len = lstrlen(szName);

	cnt2 = 0;

	for( cnt=0;cnt<len;cnt++ ) {
		ch = (DWORD)szName[cnt];
		ch1 = (BYTE)(ch>>4)&0xF;
		ch2 = (BYTE)ch&0xF;
		ch1 += 'a';
		ch2 += 'a';
		szDecodeName[ cnt2++ ] = (char)ch1;
		szDecodeName[ cnt2++ ] = (char)ch2;
	}
	szDecodeName[cnt2] = 0;

	return TRUE;
}


static int GetUserCode( char *szName )
{
	int cnt,len;
	BYTE ch;
	BYTE *lpData = (BYTE *)szName;

	len = lstrlen( szName );

	ch = 0;
	/*
		if ( ch>='a' && ch<='z' ) {//�빮�� �ҹ��ڷ�
			Sum2 += (ch-0x20)*(cnt+1);
	*/

	for(cnt=0;cnt<len;cnt++) {
		if ( lpData[cnt]>='a' && lpData[cnt]<='z' ) {
			ch += ( lpData[cnt]-0x20 );				//�ҹ��� �빮�ڷ� ���
		}
		else
			ch += lpData[cnt];
	}

	return ch;
}


//������ ���� ��� ���ϱ�
int GetPostBoxFile( char *szID , char *szFileName )
{

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "%s\\%d\\��%s.dat" , szPostBoxDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "%s\\%d\\��%s.dat" , szPostBoxDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "%s\\%d\\%s.dat" , szPostBoxDir , GetUserCode(szID) , szID );
	
	return TRUE;
}


static int GetUserInfoFile( char *szID , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserInfoDir , szID );

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "DataServer\\%s\\%d\\��%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "DataServer\\%s\\%d\\��%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "DataServer\\%s\\%d\\%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
	
	return TRUE;
}

int GetUserDataFile( char *szName , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserDataDir , szName );
	wsprintf( szFileName , "DataServer\\%s\\%d\\%s.dat" , szRecordUserDataDir , GetUserCode(szName) , szName );

	return TRUE;
}

int GetTT_ServerPath( char *szServerID , char *szTTServerPath )
{
	int cnt;

	szTTServerPath[0] = 0;

	for(cnt=0;cnt<rsServerConfig.TT_DataServer_Count;cnt++) {
		if ( lstrcmpi( szServerID , rsServerConfig.TT_DataServer[cnt].szServerID )==0 ) {
			lstrcpy( szTTServerPath , rsServerConfig.TT_DataServer[cnt].szServerPath );
			return TRUE;
		}
	}

	return FALSE;
}


int GetRealID( char *szID , char *szRealID )
{

	int cnt,len;

	len = lstrlen( szID );
	lstrcpy( szRealID , szID );
	for(cnt=len-1;cnt>=0;cnt--) {
		if ( szRealID[cnt]=='@' ) {
			szRealID[cnt] = 0;
			return TRUE;
		}
	}


	return FALSE;
}

//����ID ���ϱ�
int SetServerID( char *szID , char *szServerID )
{
	char szFile[64];
	int cnt,len;

	if ( rsServerConfig.TT_DataServer_Count ) {
		lstrcpy( szFile , szID );
		len = lstrlen(szFile);
		for(cnt=len-1;cnt>=0;cnt--) {
			if ( szFile[cnt]=='@' ) {
				lstrcpy( szServerID , szFile+cnt+1 );
				szServerID[4] = 0;
				return TRUE;
			}
		}
	}

	return FALSE;
}


static int GetUserInfoFile2( char *szID , char *szFileName , char *szServerID )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserInfoDir , szID );
	char szTTServerPath[128];
	GetTT_ServerPath( szServerID , szTTServerPath );

	char szRealID[64];

	GetRealID( szID , szRealID );

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "%s\\DataServer\\%s\\%d\\��%s.dat" , szTTServerPath, szRecordUserInfoDir , GetUserCode(szRealID) , szRealID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "%s\\DataServer\\%s\\%d\\��%s.dat" , szTTServerPath,szRecordUserInfoDir , GetUserCode(szRealID) , szRealID );
		return TRUE;
	}

	wsprintf( szFileName , "%s\\DataServer\\%s\\%d\\%s.dat" , szTTServerPath , szRecordUserInfoDir , GetUserCode(szRealID) , szRealID );
	
	return TRUE;
}

int GetUserDataFile2( char *szName , char *szFileName , char *szServerID )
{
	char szTTServerPath[128];
	GetTT_ServerPath( szServerID , szTTServerPath );
/*
	char szRealID[64];
	int cnt,len;

	len = lstrlen( szID );
	lstrcpy( szRealID , szID );
	for(cnt=len-1;cnt>=0;cnt--) {
		if ( szRealID[cnt]=='@' ) {
			szRealID[cnt] = 0;
			break;
		}
	}
*/
	wsprintf( szFileName , "%s\\DataServer\\%s\\%d\\%s.dat" , szTTServerPath, szRecordUserDataDir , GetUserCode(szName) , szName );

	return TRUE;
}


static int GetDeleteDataFile( char *szName , char *szFileName )
{
	wsprintf( szFileName , "DataServer\\%s\\%s.dat" , szRecordDeleteDir , szName );

	return TRUE;
}


static int GetUserDataBackupFile( char *szName , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserBackupDataDir , szName );
	wsprintf( szFileName , "DataServer\\%s\\%d\\%s.dat" , szRecordUserBackupDataDir , GetUserCode(szName) , szName );

	return TRUE;
}

static int GetWareHouseFile( char *szName , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.war" , szRecordWareHouseDir , szName );

	char *szID = szName;

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "DataServer\\%s\\%d\\��%s.dat" , szRecordWareHouseDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "DataServer\\%s\\%d\\��%s.dat" , szRecordWareHouseDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "DataServer\\%s\\%d\\%s.war" , szRecordWareHouseDir , GetUserCode(szName) , szName );

	return TRUE;
}

static int GetUserDataFile_BackupDay( char *szName , char *szFileName , int Day )
{
	wsprintf( szFileName , "%s\\%d\\%s\\%d\\%s.dat" , 
		rsServerConfig.szBackupPath , Day ,
		szRecordUserDataDir , GetUserCode(szName) , szName );

	return TRUE;
}

static int GetWareHouseFile_Backup( char *szName , char *szFileName , int Day )
{

	char *szID = szName;
	char szBuff[256];

	wsprintf( szBuff , "%s\\%d" , rsServerConfig.szBackupPath , Day ); 

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "%s\\%s\\%d\\��%s.dat" , szBuff, szRecordWareHouseDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "%s\\%s\\%d\\��%s.dat" , szBuff, szRecordWareHouseDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "%s\\%s\\%d\\%s.war" , szBuff, szRecordWareHouseDir , GetUserCode(szName) , szName );

	return TRUE;
}



//����Ÿ ���弭�� ���丮 ����
int CreateDataServerDirectory()
{
	int cnt;
	char szBuff[256];

	CreateDirectory( "DataServer" , NULL );				//���丮 ����
	
	wsprintf( szBuff , "DataServer\\%s" , szRecordUserInfoDir );
	CreateDirectory( szBuff , NULL );				//���丮 ����
	wsprintf( szBuff , "DataServer\\%s" , szRecordUserDataDir );
	CreateDirectory( szBuff , NULL );				//���丮 ����
	wsprintf( szBuff , "DataServer\\%s" , szRecordWareHouseDir );
	CreateDirectory( szBuff , NULL );				//���丮 ����
	wsprintf( szBuff , "DataServer\\%s" , szRecordUserBackupDataDir );
	CreateDirectory( szBuff , NULL );				//���丮 ����
	wsprintf( szBuff , "DataServer\\%s" , szRecordDeleteDir );
	CreateDirectory( szBuff , NULL );				//���丮 ����


	for( cnt=0;cnt<256;cnt++ ) {
		wsprintf( szBuff , "DataServer\\%s\\%d" , szRecordUserInfoDir , cnt );
		CreateDirectory( szBuff , NULL );			//���丮 ����
	}
	for( cnt=0;cnt<256;cnt++ ) {
		wsprintf( szBuff , "DataServer\\%s\\%d" , szRecordUserDataDir , cnt );
		CreateDirectory( szBuff , NULL );			//���丮 ����
	}
	for( cnt=0;cnt<256;cnt++ ) {
		wsprintf( szBuff , "DataServer\\%s\\%d" , szRecordWareHouseDir , cnt );
		CreateDirectory( szBuff , NULL );			//���丮 ����
	}
	for( cnt=0;cnt<256;cnt++ ) {
		wsprintf( szBuff , "DataServer\\%s\\%d" , szRecordUserBackupDataDir , cnt );
		CreateDirectory( szBuff , NULL );			//���丮 ����
	}

	//������ ���丮 �����
	if ( CreateDirectory( szPostBoxDir , NULL ) ) {			//���丮 ����
		for( cnt=0;cnt<256;cnt++ ) {
			wsprintf( szBuff , "%s\\%d" , szPostBoxDir , cnt );
			CreateDirectory( szBuff , NULL );				//���丮 ����
		}
	}

	return TRUE;
}



//ĳ���� ���� ���� �ڵ� ���ϱ�
DWORD	GetCharInfoCode( smCHAR_INFO *lpCharInfo )
{
	int cnt;
	int size;
	BYTE *lpCharBuff;
	DWORD	dwKey;
	DWORD	dwCode;

	size = sizeof(smCHAR_INFO);
	lpCharBuff = (BYTE *)lpCharInfo;

	dwKey = 0;

	for( cnt=0;cnt<size;cnt++ ) {
		dwKey += lpCharBuff[cnt];
	}

	dwCode = 0;

	for( cnt=0;cnt<size;cnt++ ) {
		dwCode += (dwKey+cnt)*lpCharBuff[cnt];
	}

	return dwCode;
}

//sUSESKILL sinSkill;
/*
/////////////��ų ���̺� ����ü 
struct sSKILL{  
	char	sSkillName[32];			//��ų �̸� 
	DWORD	CODE;					//��ų �ڵ� 
	char    FileName[32];			//��ų ���� �̸� 
	int     Flag;					//��밡�� �÷� 
	int     Use;					//����Ʈ �Ҵ簡�� �÷� 
	int     Point;					//��ų�� �Ҵ�� ����Ʈ 
	int     ShortKey;				//��� Ű 
	int     MousePosi;				//��ų ���콺 ������ 
	int     Position;				
	int     UseTime;				//��� �ð� 
	int     CheckTime;				//���ð��� üũ�Ѵ� 
	int     GageLength;				//�����͸� ������ ���� 
	float     GageLength2;			//�����͸� ������ ���� 
	float   Mastery;				//�����͸� ���� ��ġ 
	int     UseSkillCount;			//��ų��� ��ġ 
	float	UseSkillMastery;		//���� �ö󰡴� ��ġ 
	int     UseSkillMasteryGage;	//���� �ö󰡴� ������ 
	int     UseSkillFlag;			//1�� ��� 0�� ���Ұ� 
	int     PlusState[5];			//�÷��� 
	sSKILL_INFO Skill_Info;
};

/////////////���� ���ǰ��ִ� ��ų ����ü 
struct sUSESKILL{
	sSKILL UseSkill[SIN_MAX_USE_SKILL]; //����Ҽ��ִ� ��ų 
	sSKILLBOX SkillBox[SIN_MAX_USE_SKILL];
	sSKILL *pLeftSkill;		//���콺 ���� ��ư�� �Ҵ�� ��ų				
	sSKILL *pRightSkill;	//���콺 �����ʿ� �Ҵ�� ��ų 
	int   SkillPoint;

};
//����� ��ų ����
struct	RECORD_SKILL {
	BYTE	bSkillPoint[SIN_MAX_USE_SKILL];			//��ų ����Ʈ
	WORD	wSkillMastery[SIN_MAX_USE_SKILL];		//��ų ���õ�
	BYTE	bShortKey[SIN_MAX_USE_SKILL];			//��� Ű
	WORD	wSelectSkill[2];						//���õ� ��ų
	//int		RemainPoint;
};

*/

//��ų ����
int	RecordSkill( RECORD_SKILL *lpRecordSkill )
{
	int cnt;
	int mcnt;

	for(cnt=0;cnt<SIN_MAX_USE_SKILL;cnt++) {
		mcnt = cnt&15;							//��ų ���� ���� ������ ���� ( �迭�� 16���� ��Ƴ��� 16�� �迭�� 0�� ���� )

		lpRecordSkill->bSkillPoint[mcnt] = sinSkill.UseSkill[cnt].Point;
		if ( sinSkill.UseSkill[cnt].Point>255 ) lpRecordSkill->bSkillPoint[mcnt]=255;

		if ( sinSkill.UseSkill[cnt].UseSkillCount<10000 )		//��ų���õ� �ְ�ġ
			lpRecordSkill->wSkillMastery[mcnt] = sinSkill.UseSkill[cnt].UseSkillCount;
		else
			lpRecordSkill->wSkillMastery[mcnt] = 10000;

		lpRecordSkill->bShortKey[mcnt] = sinSkill.UseSkill[cnt].ShortKey|(sinSkill.UseSkill[cnt].MousePosi<<4);
	}

	lpRecordSkill->wSelectSkill[0] = 0;
	lpRecordSkill->wSelectSkill[1] = 0;

	if ( sinSkill.pLeftSkill && sinSkill.pLeftSkill->CODE!=SKILL_NORMAL_ATTACK ) {
		lpRecordSkill->wSelectSkill[0] = sinSkill.pLeftSkill->Skill_Info.SkillNum+1;
	}
	if ( sinSkill.pRightSkill && sinSkill.pRightSkill->CODE!=SKILL_NORMAL_ATTACK ) {
		lpRecordSkill->wSelectSkill[1] = sinSkill.pRightSkill->Skill_Info.SkillNum+1;
	}

	return TRUE;
}



//����Ʈ�� ȹ���� ��ų ����Ʈ
int GetSkillPoint_LevelQuest( int Level , DWORD dwLevelQuestLog )
{
	int Point = 0;

	if ( dwLevelQuestLog&QUESTBIT_LEVEL_30 && Level>=30 ) {

	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_55 && Level>=55 ) {
		Point++;
	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_70 && Level>=70 ) {
		Point++;
	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_80 && Level>=80 ) {
		Point+=2;
	}

	return Point;
}

//����Ʈ�� ȹ���� ���� ����Ʈ
int GetStatePoint_LevelQuest( int Level , DWORD dwLevelQuestLog )
{
	int Point = 0;

	if ( dwLevelQuestLog&QUESTBIT_LEVEL_30 && Level>=30 ) {
		Point += 5;
	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_55 && Level>=55 ) {

	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_70 && Level>=70 ) {
		Point += 5;
	}
	if ( dwLevelQuestLog&QUESTBIT_LEVEL_80 && Level>=80 ) {
		Point += 5;
	}

	if ( dwLevelQuestLog&QUESTBIT_LEVEL_80_2 && Level>=80 ) {		//80�� �̻� ����7�� �ش� (+2)
		Point += (Level-79)*2;
	}

	if ( dwLevelQuestLog&QUESTBIT_LEVEL_90_2 && Level>=90 ) {		//90�� �̻� ����10�� �ش� (+2+3)
		Point += (Level-89)*3;
	}

	return Point;
}


//��ų ����
int	RestoreSkill( RECORD_SKILL *lpRecordSkill , DWORD dwLevelQuestLog )
{
	int cnt,mcnt;
	int	Point,EPoint;

	Point = 0;
	EPoint = 0;

	if ( lpRecordSkill->bSkillPoint[0]>0 && lpRecordSkill->bSkillPoint[15]==0 ) {
		lpRecordSkill->bSkillPoint[0] = 0;
		lpRecordSkill->wSkillMastery[0] = 0;
		lpRecordSkill->bShortKey[0] = 0;
	}

	for(cnt=0;cnt<SIN_MAX_USE_SKILL;cnt++) {
		mcnt = cnt&15;							//��ų ���� ���� ������ ���� ( �迭�� 16���� ��Ƴ��� 16�� �迭�� 0�� ���� )

		if ( cnt>0 ) {	//�ָ��� ����
			sinSkill.UseSkill[cnt].Point = lpRecordSkill->bSkillPoint[mcnt];
			sinSkill.UseSkill[cnt].UseSkillCount = lpRecordSkill->wSkillMastery[mcnt];
			sinSkill.UseSkill[cnt].ShortKey = lpRecordSkill->bShortKey[mcnt]&0xF;
			sinSkill.UseSkill[cnt].MousePosi = lpRecordSkill->bShortKey[mcnt]>>4;
			sinSkill.UseSkill[cnt].Position = cnt+1;
			//Point += sinSkill.UseSkill[cnt].Point;
		}
	}

	for(cnt=1;cnt<13;cnt++) {
		Point += sinSkill.UseSkill[cnt].Point;
	}
	for(cnt=13;cnt<SIN_MAX_USE_SKILL;cnt++) {
		EPoint += sinSkill.UseSkill[cnt].Point;
	}


	sinSkill.UseSkill[0].Point = 1;
	sinSkill.UseSkill[0].UseSkillFlag = 1;
	sinSkill.UseSkill[0].Position = 1;


	sinSkill.pLeftSkill = &sinSkill.UseSkill[ lpRecordSkill->wSelectSkill[0] ];
	sinSkill.pRightSkill = &sinSkill.UseSkill[ lpRecordSkill->wSelectSkill[1] ];

	if ( lpCurPlayer->smCharInfo.Level>=10 ) {
		sinSkill.SkillPoint = ((lpCurPlayer->smCharInfo.Level-8)/2)-Point;
		sinSkill.SkillPoint += GetSkillPoint_LevelQuest(lpCurPlayer->smCharInfo.Level,dwLevelQuestLog);		//����Ʈ�� ���� ����Ʈ

		if ( sinSkill.SkillPoint<0 ) {
			//��ų ����Ʈ ���� ( ��ų �ʱ�ȭ )
			for(cnt=0;cnt<13;cnt++) {
				sinSkill.UseSkill[cnt].Point = 0;
				sinSkill.UseSkill[cnt].UseSkillCount = 0;
			}
			sinSkill.SkillPoint = 0;
		}
	}
	else {
		sinSkill.SkillPoint = 0;
	}


	if ( lpCurPlayer->smCharInfo.Level>=60 ) {
		sinSkill.SkillPoint4 = ((lpCurPlayer->smCharInfo.Level-58)/2)-EPoint;

		if ( sinSkill.SkillPoint4<0 ) {
			//��ų ����Ʈ ���� ( ��ų �ʱ�ȭ )
			for(cnt=13;cnt<SIN_MAX_USE_SKILL;cnt++) {
				sinSkill.UseSkill[cnt].Point = 0;
				sinSkill.UseSkill[cnt].UseSkillCount = 0;
			}
			sinSkill.SkillPoint4 = 0;
		}
	}
	else {
		sinSkill.SkillPoint4 = 0;
	}


	cInvenTory.SetItemToChar();
	cSkill.InitReformSkillPointForm();

	return TRUE;
}


//��ų ýũ
int	CheckSkillPoint( int Level , RECORD_SKILL *lpRecordSkill , int *spTotal , DWORD dwLevelQuestLog )
{
	int cnt,mcnt;
	int	Point,EPoint;
	int SkillPoint;
	int ExtraPoint;

	Point = 0;
	EPoint = 0;

	//for(cnt=1;cnt<SIN_MAX_USE_SKILL;cnt++) {
	for(cnt=1;cnt<13;cnt++) {
		if ( lpRecordSkill->bSkillPoint[cnt]>10 ) return FALSE;
		Point += abs(lpRecordSkill->bSkillPoint[cnt]);
	}

	for(cnt=13;cnt<SIN_MAX_USE_SKILL;cnt++) {
		mcnt = cnt&15;
		if ( lpRecordSkill->bSkillPoint[mcnt]>10 ) return FALSE;
		EPoint += abs(lpRecordSkill->bSkillPoint[mcnt]);
	}

	if ( spTotal ) *spTotal = (Point+(EPoint<<16));

	if ( Level>=10 ) {
		SkillPoint = ((Level-8)/2)-Point;
		SkillPoint += GetSkillPoint_LevelQuest( Level , dwLevelQuestLog );		//����Ʈ�� ȹ���� ��ų ����Ʈ

		if ( SkillPoint<0 ) {
			//��ų ����Ʈ ���� ( ��ų �ʱ�ȭ )
			return FALSE;
		}
	}
	else {
		if ( Point>0 ) return FALSE;
	}

	if ( Level>=60 ) {
		ExtraPoint = ((Level-58)/2)-EPoint;
		if ( ExtraPoint<0 ) {
			//Ȯ�彺ų ����Ʈ ���� ( ��ų �ʱ�ȭ )
			return FALSE;
		}
	}
	else {
		if ( EPoint>0 ) return FALSE;
	}

	return TRUE;
}


//99
//ĳ���� ������Ʈ ���� ���Ӱ� �����Ѵ�
int ReformCharStatePoint( smCHAR_INFO *lpCharInfo , DWORD dwLevelQuestLog )
{
	int Total;
	int NewState;

	Total = abs(lpCharInfo->Strength) + 
			abs(lpCharInfo->Spirit) + 
			abs(lpCharInfo->Dexterity) +
			abs(lpCharInfo->Health) +
			abs(lpCharInfo->Talent) +
			abs(lpCharInfo->StatePoint);

	//99�� �⺻ġ
	NewState = 99+((lpCharInfo->Level-1)*5);
	NewState += GetStatePoint_LevelQuest( lpCharInfo->Level , dwLevelQuestLog );		//����Ʈ�� ȹ���� ���� ����Ʈ

	lpCharInfo->StatePoint += (NewState-Total);

	if ( lpCharInfo->StatePoint<=-10 ) {
		//ĳ���� �ɷ�ġ ���� �߻�
		lpCharInfo->Strength = 1; 
		lpCharInfo->Spirit = 8;
		lpCharInfo->Dexterity = 1;
		lpCharInfo->Health = 8;
		lpCharInfo->Talent = 1;
		if ( lpCharInfo->StatePoint<0 ) lpCharInfo->StatePoint = 0;

		return FALSE;
	}
	if ( lpCharInfo->StatePoint<0 ) lpCharInfo->StatePoint = 0;

	return TRUE;
}


//���� ���� ����Ÿ ���
int RecordGameData( sGAME_SAVE_INFO *lpGameSaveInfo )
{
	lpGameSaveInfo->Head = dwRecordVersion;

	lpGameSaveInfo->CameraMode = cInterFace.sInterFlags.CameraAutoFlag;

	if ( lpCurPlayer->OnStageField>=0 ) 
		lpGameSaveInfo->PlayStageNum = StageField[lpCurPlayer->OnStageField]->FieldCode;
	else
		lpGameSaveInfo->PlayStageNum = -1;

	//�α� �ƿ� ��ǥ
	lpGameSaveInfo->PosX = lpCurPlayer->pX;
	lpGameSaveInfo->PosZ = lpCurPlayer->pZ;

	//������ ����� ����ID ����
	if ( lstrlen(UserAccount)<32 ) {
		lstrcpy( lpGameSaveInfo->szMasterID , UserAccount );
	}

	//Ŭ���̾�Ʈ���� ������ ȹ���� ����ġ�� �� ���
	lpGameSaveInfo->TotalExp = GetTotalExp()^lpGameSaveInfo->PosX;
	lpGameSaveInfo->TotalMoney = GetTotalMoney()^lpGameSaveInfo->PosZ;
	lpGameSaveInfo->TotalSubExp = GetTotalSubExp();

	//��ų ����
	RecordSkill( &lpGameSaveInfo->SkillInfo );
	//�⺻���� ����Ű����
	lpGameSaveInfo->ShortKey_NormalAttack = sinSkill.UseSkill[0].ShortKey|( sinSkill.UseSkill[0].MousePosi<<4 );


	sQUEST_INFO	QuestInfo;
	DWORD	QuestCode;

	ZeroMemory( &QuestInfo, sizeof(sQUEST_INFO) );
	QuestCode = sinSaveQuest( &QuestInfo.Data );
	QuestInfo.wQuestCode[0] = (WORD)QuestCode;
    memcpy( &lpGameSaveInfo->QuestInfo , &QuestInfo , sizeof(sQUEST_INFO) );

	memcpy( &lpGameSaveInfo->LastQuestInfo , &RecordLastQuestInfo , sizeof(sLAST_QUEST_INFO) );		//���� ����Ʈ ���

	lpGameSaveInfo->sPetInfo[0] = (short)cHelpPet.PetKind;
	lpGameSaveInfo->sPetInfo[1] = (short)cHelpPet.PetShow;

	lpGameSaveInfo->dwLevelQuestLog = sinQuest_levelLog;	
	lpGameSaveInfo->dwElementaryQuestLog = haElementaryQuestLog;	

	

	return TRUE;
}

//���� ���� ����Ÿ ���
int RestoreGameData( sGAME_SAVE_INFO *lpGameSaveInfo )
{
	cInterFace.sInterFlags.CameraAutoFlag = lpGameSaveInfo->CameraMode;
	if ( lpGameSaveInfo->QuestInfo.wQuestCode[0] ) {
		sinLoadQuest( lpGameSaveInfo->QuestInfo.wQuestCode[0] , &lpGameSaveInfo->QuestInfo.Data );
	}

	memcpy( &RecordLastQuestInfo , &lpGameSaveInfo->LastQuestInfo , sizeof(sLAST_QUEST_INFO) );		//���� ����Ʈ ���
	//����Ʈ���� ���忡 ����
	SetQuestBoard();
	
	if ( sBiInfo ) {
		sBiInfo->PCRNo = lpGameSaveInfo->PCRNo;
		sBiInfo->EventPlay[0] = lpGameSaveInfo->EventPlay[0];
		sBiInfo->EventPlay[1] = lpGameSaveInfo->EventPlay[1];
	}

	cHelpPet.PetKind = (int)lpGameSaveInfo->sPetInfo[0];
	cHelpPet.PetShow = (int)lpGameSaveInfo->sPetInfo[1];
	sinQuest_levelLog = lpGameSaveInfo->dwLevelQuestLog;
	haElementaryQuestLog = lpGameSaveInfo->dwElementaryQuestLog;
/*
	if ( sBiInfo->PCRNo>0 ) {
		//�ǽù� �����
		cPCBANGPet.PetKind = TRUE;
		cPCBANGPet.ShowPet();
	}
*/
	//���� ĳ�� ����
	cSinSiege.SetTaxRate( lpGameSaveInfo->BlessCastleTax );	//���� ����
	//���� ĳ�� ������ Ŭ�� ����
	SetBlessCastleMaster( lpGameSaveInfo->dwBlessCastleMaster , 0 );

	return TRUE;
}

//����� ����Ʈ ��� �߰�
int Record_LastQuest( WORD	wQuestCode )
{
	int cnt;

	cnt = RecordLastQuestInfo.LastQuestCount&LAST_QUEST_MASK;
	RecordLastQuestInfo.wLastQuest[cnt] = wQuestCode;
	RecordLastQuestInfo.LastQuestCount++;

	return TRUE;
}

//���� ����Ʈ �˻�
int FindLastQuestCode( WORD wQuestCode )
{
	int cnt,mcnt,start;

	if ( RecordLastQuestInfo.LastQuestCount>LAST_QUEST_MASK )
		start = RecordLastQuestInfo.LastQuestCount-LAST_QUEST_MASK;
	else
		start = 0;

	for(cnt=start;cnt<RecordLastQuestInfo.LastQuestCount;cnt++) {
		mcnt = cnt&LAST_QUEST_MASK;
		if ( RecordLastQuestInfo.wLastQuest[mcnt]==wQuestCode ) {
			return TRUE;		//����Ʈ ã�Ҵ�
		}
	}
	return FALSE;
}

/*
#define	LAST_QUEST_MAX		32
#define	LAST_QUEST_MASK		31

struct	sLAST_QUEST_INFO {
	WORD wLastQuest[LAST_QUEST_MAX];
	int	LastQuestCount;
}
*/
//��� ����Ÿ ������ �̻� ���� Ȯ��
int CheckRecordDataItem( TRANS_RECORD_DATA *lpRecordData )
{
	int DataSize , size;
	int ItemCount;
	int cnt;
	int SizeCount;

	BYTE *lpRecordItem;

	DataSize = lpRecordData->DataSize;
	ItemCount = lpRecordData->ItemCount;
	lpRecordItem = lpRecordData->Data;

	SizeCount = 0;

	for( cnt=0;cnt<ItemCount;cnt++ ) {

		size = ((int *)lpRecordItem)[0];
		SizeCount += size;
		lpRecordItem += size;

		if ( SizeCount>DataSize || SizeCount<0 )
			return FALSE;
	}

	if ( SizeCount==DataSize ) return TRUE;

	return FALSE;
}
/*
//����� ������ ����Ÿ�� 150 �������� ������ȯ
int	ConvertItem_Server150(TRANS_RECORD_DATA *lpTransRecordData )
{
	BYTE	*lpRecItem;
	sRECORD_ITEM	sRecordItem[INVENTORY_MAXITEM*2];

	int cnt;
	int CompSize;

	lpRecItem = (BYTE *)lpTransRecordData->Data;

	for( cnt=0;cnt<lpTransRecordData->ItemCount;cnt++ ) {
		//���� ����Ÿ �ص� ( Z/NZ ��� )
		DecodeCompress( (BYTE *)lpRecItem , (BYTE *)&sRecordItem[cnt] );
		rsReformItem_Server( &sRecordItem[cnt].sItemInfo );					//������ �ű� ����
		lpRecItem += ((int *)lpRecItem)[0];
	}

	lpTransRecordData->DataSize = 0;
	lpRecItem = (BYTE *)lpTransRecordData->Data;

	for( cnt=0;cnt<lpTransRecordData->ItemCount;cnt++ ) {
		//����Ÿ ���� ( Z/NZ ��� )
		CompSize = EecodeCompress( (BYTE *)&sRecordItem[cnt] , (BYTE *)lpRecItem , sizeof(sRECORD_ITEM) );
		lpRecItem += CompSize;
		lpTransRecordData->DataSize += CompSize;
	}

	lpTransRecordData->size = ((DWORD)lpRecItem)-((DWORD)lpTransRecordData);

	return TRUE;
}
*/



//������ ����Ÿ�� �����Ͽ� ����
int rsRECORD_DBASE::MakeRecordData( smCHAR_INFO *lpCharInfo , sITEM *lpItems , sITEM *lpItems2 , sITEM *lpMouseItem )
{
	int cnt;

	BYTE	*lpRecItem;
	sRECORD_ITEM	sRecordItem;
	int		CompSize;

	lstrcpy( TransRecordData.szHeader , szRecordHeader );
	memcpy( &TransRecordData.smCharInfo , lpCharInfo , sizeof( smCHAR_INFO ) );

	TransRecordData.smCharInfo.wVersion[0] = Version_CharInfo;
	//TransRecordData.smCharInfo.wVersion[1] = 0;

	ZeroMemory( &TransRecordData.GameSaveInfo , sizeof(sGAME_SAVE_INFO) );

	TransRecordData.ThrowItemCount = 0;

	RecordGameData( &TransRecordData.GameSaveInfo );
	//ĳ���� ���� �ڵ� ����
	TransRecordData.GameSaveInfo.dwChkSum_CharInfo = GetCharInfoCode( lpCharInfo );

	TransRecordData.ItemCount = 0; 
	lpRecItem = TransRecordData.Data;
	TransRecordData.DataSize = 0;

	for( cnt=0;cnt<INVENTORY_MAXITEM;cnt++ ) {
		if ( lpItems[cnt].Flag ) {
/*
			if ( !lpItems[cnt].sItemInfo.ItemName[0] || 
				(lpItems[cnt].CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ||
				lpItems[cnt].sItemInfo.ItemHeader.dwChkSum==(lpItems[cnt].sItemInfo.Temp0-lpItems[cnt].sItemInfo.CODE) ) {
*/
				sRecordItem.ItemCount=cnt;
				sRecordItem.x = lpItems[cnt].x;
				sRecordItem.y = lpItems[cnt].y;
				sRecordItem.ItemPosition = lpItems[cnt].ItemPosition;
				memcpy( &sRecordItem.sItemInfo , &lpItems[cnt].sItemInfo , sizeof( sITEMINFO ) );

				//����Ÿ ���� ( Z/NZ ��� )
				CompSize = EecodeCompress( (BYTE *)&sRecordItem , (BYTE *)lpRecItem , sizeof(sRECORD_ITEM) );

				lpRecItem += CompSize;
				TransRecordData.DataSize += CompSize;

				TransRecordData.ItemCount++;
/*
			}
			else {
				cnt = cnt;
			}
*/
		}
	}

	TransRecordData.ItemSubStart = TransRecordData.ItemCount;

	for( cnt=0;cnt<INVENTORY_MAXITEM;cnt++ ) {
		if ( lpItems2[cnt].Flag ) {
			sRecordItem.ItemCount=cnt;
			sRecordItem.x = lpItems2[cnt].x;
			sRecordItem.y = lpItems2[cnt].y;
			sRecordItem.ItemPosition = lpItems2[cnt].ItemPosition;
			memcpy( &sRecordItem.sItemInfo , &lpItems2[cnt].sItemInfo , sizeof( sITEMINFO ) );

			//����Ÿ ���� ( Z/NZ ��� )
			CompSize = EecodeCompress( (BYTE *)&sRecordItem , (BYTE *)lpRecItem , sizeof(sRECORD_ITEM) );

			lpRecItem += CompSize;
			TransRecordData.DataSize += CompSize;

			TransRecordData.ItemCount++;
		}
	}

	if ( lpMouseItem && lpMouseItem->Flag ) {
		//���콺�� �������� ��� �ִ� ���
		sRecordItem.ItemCount=0;
		sRecordItem.x = 0;
		sRecordItem.y = 0;
		sRecordItem.ItemPosition = -1;
		memcpy( &sRecordItem.sItemInfo , &lpMouseItem->sItemInfo , sizeof( sITEMINFO ) );

		//����Ÿ ���� ( Z/NZ ��� )
		CompSize = EecodeCompress( (BYTE *)&sRecordItem , (BYTE *)lpRecItem , sizeof(sRECORD_ITEM) );

		lpRecItem += CompSize;
		TransRecordData.DataSize += CompSize;

		TransRecordData.ItemCount++;
	}


	TransRecordData.code = smTRANSCODE_RECORDDATA;
	TransRecordData.size = ((DWORD)lpRecItem)-((DWORD)&TransRecordData);

	return TRUE;
}


int ClientRecordPotion[3][4];
int ClientRecordPotionFlag = 0;

int ResetClientRecordPotion( sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int cnt,cnt2;

	if ( !lpGameSaveInfo->sPotionUpdate[0] || lpGameSaveInfo->sPotionUpdate[0]!=lpGameSaveInfo->sPotionUpdate[1] ) 
		return FALSE;

	ClientRecordPotionFlag = TRUE;

	for(cnt=0;cnt<3;cnt++)
		for(cnt2=0;cnt2<4;cnt2++)
			ClientRecordPotion[cnt][cnt2] = lpGameSaveInfo->sPotionCount[cnt][cnt2];


	return TRUE;
}

//���� �˻��
int AddRecordPotion( DWORD dwPotionCode , int PotionCount )
{
	int Count=PotionCount;
	int x=-1;
	int y=-1;

	if ( ClientRecordPotionFlag==0 ) return 0;

	switch( dwPotionCode ) {
		case (sinPL1|sin01):		//����(��)
			x=0;y=0;
			break;
		case (sinPL1|sin02):		//����(��)
			x=0;y=1;
			break;
		case (sinPL1|sin03):		//����(��)
			x=0;y=2;
			break;
		case (sinPL1|sin04):		//����(��)
			x=0;y=3;
			break;

		case (sinPM1|sin01):		//���(��)
			x=1;y=0;
			break;
		case (sinPM1|sin02):		//���(��)
			x=1;y=1;
			break;
		case (sinPM1|sin03):		//���(��)
			x=1;y=2;
			break;
		case (sinPM1|sin04):		//���(��)
			x=1;y=3;
			break;

		case (sinPS1|sin01):		//�ٷ�(��)
			x=2;y=0;
			break;
		case (sinPS1|sin02):		//�ٷ�(��)
			x=2;y=1;
			break;
		case (sinPS1|sin03):		//�ٷ�(��)
			x=2;y=2;
			break;
		case (sinPS1|sin04):		//�ٷ�(��)
			x=2;y=3;
			break;
	}

	if ( x>=0 && y>=0 ) {
		ClientRecordPotion[x][y] += Count;
		return ClientRecordPotion[x][y];
	}

	return 0;
}



//�ҷ��� ����Ÿ ������ �ش� ��ġ�� ����
int rsRECORD_DBASE::ResotrRecordData( smCHAR_INFO *lpCharInfo , sITEM *lpItems , sITEM *lpItems2 , sITEM *lpMouseItem )
{
	int cnt;
	int cnt2,cnt3;
	sITEM *lpChkItem[512];
	int		ChkCnt;
	int		CopyItemCount;
	int		BadItemCount;
	int		SetFlag;
	DWORD	dwItemCode;
	int		WeightError = 0;
	int		PostionError = 0;
	int		SvrPotionError = 0;
	int		ReformItemCount = 0;
	DWORD	dwCode;
	sITEMINFO	*lpsItemInfo;

	BYTE	*lpRecItem;
	sITEM	*lpSaveItem;
	sRECORD_ITEM	sRecordItem;
	sITEM	sItem_Buff;
	char	szMsgBuff[128];
	int		PotionCnt;

	//char	szTestBuff[4096];


	//HANDLE	hResource;
	//HANDLE	hLoadRes;
	//sCOPYITEM	*lpCopyItems=0;

	//ĳ���� ���� ���� Ȯ��
	CheckCharForm();

	memcpy( lpCharInfo , &TransRecordData.smCharInfo , sizeof( smCHAR_INFO ) );
	lpCharInfo->bUpdateInfo[0] = 0;

	//���� ���� ���
	ResetClientRecordPotion( &TransRecordData.GameSaveInfo );

	//ĳ���� ���� ���� Ȯ��
	CheckCharForm();
	//ũ�� ýũ
	CheckCracker();
/*
	hResource = FindResource( hinst , MAKEINTRESOURCE(IDR_GOODITEM1), "GOODITEM" );
	hLoadRes = LoadResource( hinst , (HRSRC)hResource );
	lpCopyItems = (sCOPYITEM *)LockResource( hLoadRes );
*/
	if ( smConfig.DebugMode && smConfig.szFile_Player[0] ) {
		//��ڴ� ��Ų �ٲٱ� ����
		lstrcpy( lpCharInfo->szModelName ,  smConfig.szFile_Player );
		lpCharInfo->szModelName[0] = 0;
	}

	ZeroMemory( lpItems , sizeof( sITEM ) * INVENTORY_MAXITEM );
	ZeroMemory( lpItems2 , sizeof( sITEM ) * INVENTORY_MAXITEM );

	lpRecItem = (BYTE *)&TransRecordData.Data;

	ChkCnt = 0;
	CopyItemCount = 0;
	BadItemCount = 0;

/*
	if ( lpCharInfo->Weight[0]<0 || lpCharInfo->Weight[0]>lpCharInfo->Weight[1] ) {
		WeightError = TRUE;
		//��ŷ �õ��� ���� �ڵ� �Ű�
		SendSetHackUser2( 1900 , lpCharInfo->Weight[0] );
	}
*/
	// pluto ����Ʈ ���� ����  ĳ���� ������ ����
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell )
	{
		if( lpCharInfo->Weight[0]<0 || lpCharInfo->Weight[0]>lpCharInfo->Weight[1] + 300 ) {
			WeightError = TRUE;
			//��ŷ �õ��� ���� �ڵ� �Ű�
			SendSetHackUser2( 1900 , lpCharInfo->Weight[0] );
		}
	}
	else if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell2 )
	{
		if( lpCharInfo->Weight[0]<0 || lpCharInfo->Weight[0]>lpCharInfo->Weight[1] + 500 ) {
			WeightError = TRUE;
			//��ŷ �õ��� ���� �ڵ� �Ű�
			SendSetHackUser2( 1900 , lpCharInfo->Weight[0] );
		}
	}
    else
	{
		if( lpCharInfo->Weight[0]<0 || lpCharInfo->Weight[0]>lpCharInfo->Weight[1] ) {
		WeightError = TRUE;
		//��ŷ �õ��� ���� �ڵ� �Ű�
		SendSetHackUser2( 1900 , lpCharInfo->Weight[0] );
		}
	}

	for( cnt=0;cnt<TransRecordData.ItemCount;cnt++ ) {

		lpsItemInfo = 0;
		//���� ����Ÿ �ص� ( Z/NZ ��� )
		DecodeCompress( (BYTE *)lpRecItem , (BYTE *)&sRecordItem );
/*
		//����Ÿ ���� ( Z/NZ ��� )
		EecodeCompress( (BYTE *)&sRecordItem , (BYTE *)szTestBuff , sizeof(sRECORD_ITEM) );

		if ( ((int *)szTestBuff)[0]!=((int *)lpRecItem)[0] ) {
			SetFlag = SetFlag;
		}
*/

		SetFlag = TRUE;
		dwItemCode = sRecordItem.sItemInfo.CODE&sinITEM_MASK2;

		//������ ������ ����
		for( cnt2=0;cnt2<TransRecordData.ThrowItemCount;cnt2++) {
			if ( TransRecordData.ThrowItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
				TransRecordData.ThrowItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
				TransRecordData.ThrowItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
				SetFlag = FALSE;
				break;
			}
		}

/*
#define sinWA1			0x01010000	//Axes
#define sinWC1			0x01020000	//Claws
#define sinWH1			0x01030000	//Hammer & So On
#define sinWM1			0x01040000	//Magicial Stuffs
#define sinWP1			0x01050000	//Poles & Spears
#define sinWS1			0x01060000	//Shooters
#define sinWS2			0x01070000	//Swords
#define sinWT1			0x01080000	//Throwing Arms

#define sinDA1			0x02010000	//Armor
#define sinDB1			0x02020000	//Boots
#define sinDG1			0x02030000	//Gloves
#define sinDS1			0x02040000	//Shields
*/
		//PostionError
		if ( sRecordItem.ItemPosition>3 ) {
			dwCode = sRecordItem.sItemInfo.CODE>>24;
			if ( dwCode==1 ) {
				PostionError++;				//���� ����
			}
			dwCode = sRecordItem.sItemInfo.CODE>>16;
			if ( dwCode==0x0201 || dwCode==0x0204 ) {
				PostionError++;				//���� , ����
			}
		}


/*
		///////////////// �Ҹ��� ���Ƹ� ���� ////////////////////////
		if ( sRecordItem.sItemInfo.CODE==33622016 && 
			sRecordItem.sItemInfo.ItemHeader.Head==2242593061 && 
			sRecordItem.sItemInfo.ItemHeader.dwChkSum==4294914487 ) {

			//���� ������ ����
			SetFlag = FALSE;
			//��ŷ �õ��� ���� �ڵ� �Ű�
			SendSetHackUser2( 1960 , sRecordItem.sItemInfo.CODE );
		}
*/

		if ( sRecordItem.ItemPosition!=2 && CheckItemForm( &sRecordItem.sItemInfo )==FALSE ) {
			//�߸��� ������ ����
			SetFlag = FALSE;
			//��ŷ �õ��� ���� �ڵ� �Ű�
			SendSetHackUser2( 1950 , 0 );
		}

		//������ ������ȣ�� 0 �ΰ�� ���� ( ������ ������ ���� )
		if ( !sRecordItem.sItemInfo.ItemHeader.Head || !sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
			if ( sRecordItem.sItemInfo.ItemName[0] ) {
				SetFlag = FALSE;
				//��ŷ �õ��� ���� �ڵ� �Ű�
				SendSetHackUser2( 1950 , 0 );
			}
		}

		// �庰 - SP1 ������ �κ����� ������� ����
		/* ������� ���ش�
		//���� Ȯ�� ( ���ڷ��� )
		if ( (sRecordItem.sItemInfo.CODE&sinITEM_MASK2) == sinSP1 || (sRecordItem.sItemInfo.CODE&sinITEM_MASK2) == sinCH1 ){
			//���� ( ����ī���Ͱ� ������ �����ִ� ���� )
			if ( sRecordItem.sItemInfo.PotionCount>1 )	SetFlag = FALSE;

			//�����Ⱓ Ȯ��
			if ( sRecordItem.sItemInfo.dwCreateTime<(sinItemTime-(60*60*24*12)) || 
				sRecordItem.sItemInfo.dwCreateTime>(sinItemTime+(60*60*24*7)) ) {
				SetFlag = FALSE;
			}
		}
		*/

		//��ʸ�Ʈ ���� ���� �α��� �� ��� Ư�� ������ ����
		if ( TransRecordData.GameSaveInfo.TT_ServerID ) {
			if ( (sRecordItem.sItemInfo.CODE&sinITEM_MASK2) == sinBC1 ) {		//���� ������
				SetFlag = FALSE;
			}
		}


		//���� 0 ¥�� ����
		if ( (sRecordItem.sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
			if ( sRecordItem.sItemInfo.PotionCount<=0 ) {
				SetFlag = FALSE;
			}
			else {
				//���� �˻��
				PotionCnt = AddRecordPotion( sRecordItem.sItemInfo.CODE , -sRecordItem.sItemInfo.PotionCount );
				if ( PotionCnt<0 ) {
					sRecordItem.sItemInfo.PotionCount+=PotionCnt;
					SvrPotionError -= PotionCnt;		//������ ���� ���

					if ( sRecordItem.sItemInfo.PotionCount<=0 ) {
						SetFlag = FALSE;
					}
				}
			}
		}

#ifdef _DELETE_FUCKIN_ITEM
		if ( sRecordItem.sItemInfo.CODE==(sinOR1|sin21) ||
				sRecordItem.sItemInfo.CODE==(sinOA1|sin21)

			/*sRecordItem.sItemInfo.CODE==(sinOR1|sin22) ||
				sRecordItem.sItemInfo.CODE==(sinOR1|sin23) ||
				sRecordItem.sItemInfo.CODE==(sinOR1|sin24)*/
			)
			{
				memcpy( &sItem_Buff.sItemInfo , &sRecordItem.sItemInfo , sizeof( sITEMINFO ) );
				OverDay_Item_Delete( &sItem_Buff );
			}
#endif

		if ( DeleteEventItem_TimeOut( &sRecordItem.sItemInfo )==TRUE ) {
			//�̺�Ʈ ������ ��¥���� ����
			SetFlag = FALSE;

			wsprintf( szMsgBuff , mgItemTimeOut , sRecordItem.sItemInfo.ItemName );
			AddChatBuff( szMsgBuff , 0 );
		
		//	if ( sRecordItem.sItemInfo.CODE==(sinOR2|sin01) ) 
			if ( sRecordItem.sItemInfo.CODE==(sinOR2|sin01) ||  // ��������
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin06) || sRecordItem.sItemInfo.CODE==(sinOR2|sin07) || //Ŭ��ġ����
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin08) || sRecordItem.sItemInfo.CODE==(sinOR2|sin09) || //Ŭ��ġ����
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin10) || sRecordItem.sItemInfo.CODE==(sinOR2|sin11) || //Ŭ��ġ����
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin12) || sRecordItem.sItemInfo.CODE==(sinOR2|sin13) || //Ŭ��ġ����
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin14) || sRecordItem.sItemInfo.CODE==(sinOR2|sin15) || //Ŭ��ġ����
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin16) || sRecordItem.sItemInfo.CODE==(sinOR2|sin17) || //Ŭ��ġ����
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin18) || sRecordItem.sItemInfo.CODE==(sinOR2|sin19) || //Ŭ��ġ����
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin20) || sRecordItem.sItemInfo.CODE==(sinOR2|sin21) || //Ŭ��ġ����
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin22) || sRecordItem.sItemInfo.CODE==(sinOR2|sin23) || //Ŭ��ġ���� 
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin24) || sRecordItem.sItemInfo.CODE==(sinOR2|sin25) || //Ŭ��ġ����
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin31) || sRecordItem.sItemInfo.CODE==(sinOR2|sin32) || // ����� - ���� ���� �� �߰�(�ٺ�, ǻ��)
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin27) || sRecordItem.sItemInfo.CODE==(sinOA1|sin32) || //����� - ��Ÿ��, ��Ÿ�ƹķ� �߰�
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin28) || sRecordItem.sItemInfo.CODE==(sinOA1|sin33) || //����� - �̺�Ʈ ��, �̺�Ʈ �ƹķ� �߰�(7��)
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin29) || sRecordItem.sItemInfo.CODE==(sinOA1|sin34) || //����� - �̺�Ʈ ��, �̺�Ʈ �ƹķ� �߰�(1�ð�)
				 sRecordItem.sItemInfo.CODE==(sinOR2|sin30) || sRecordItem.sItemInfo.CODE==(sinOA1|sin35) || //����� - �̺�Ʈ ��, �̺�Ʈ �ƹķ� �߰�(1��)
				 sRecordItem.sItemInfo.CODE==(sinOA1|sin36) || sRecordItem.sItemInfo.CODE==(sinOA1|sin37) || // �庰 - ���� �����, ��Ʈ �ƹķ�
				  sRecordItem.sItemInfo.CODE==(sinOR2|sin33) ||																		// �庰 - ��Ʈ��

				 sRecordItem.sItemInfo.CODE==(sinDA1|sin31) || sRecordItem.sItemInfo.CODE==(sinDA2|sin31) || // ����� - �ڽ�Ƭ �߰�
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin32) || sRecordItem.sItemInfo.CODE==(sinDA2|sin32) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin33) || sRecordItem.sItemInfo.CODE==(sinDA2|sin33) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin34) || sRecordItem.sItemInfo.CODE==(sinDA2|sin34) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin35) || sRecordItem.sItemInfo.CODE==(sinDA2|sin35) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin36) || sRecordItem.sItemInfo.CODE==(sinDA2|sin36) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin37) || sRecordItem.sItemInfo.CODE==(sinDA2|sin37) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin38) || sRecordItem.sItemInfo.CODE==(sinDA2|sin38) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin39) || sRecordItem.sItemInfo.CODE==(sinDA2|sin39) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin40) || sRecordItem.sItemInfo.CODE==(sinDA2|sin40) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin41) || sRecordItem.sItemInfo.CODE==(sinDA2|sin41) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin42) || sRecordItem.sItemInfo.CODE==(sinDA2|sin42) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin43) || sRecordItem.sItemInfo.CODE==(sinDA2|sin46) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin44) || sRecordItem.sItemInfo.CODE==(sinDA2|sin44) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin45) || sRecordItem.sItemInfo.CODE==(sinDA2|sin45) ||
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin46) || sRecordItem.sItemInfo.CODE==(sinDA2|sin46) ||
				 sRecordItem.sItemInfo.CODE==(sinSP1|sin34) || // ����� - ȣ���� ĸ�� �߰�
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin54) || sRecordItem.sItemInfo.CODE==(sinDA1|sin55) || // ����� - ������ ���� �߰�
				 sRecordItem.sItemInfo.CODE==(sinDA2|sin54) || sRecordItem.sItemInfo.CODE==(sinDA2|sin55) ||  // ����� - ������ ���� �߰�
				 sRecordItem.sItemInfo.CODE==(sinDA1|sin60) || sRecordItem.sItemInfo.CODE==(sinDA1|sin61) || // �庰 - ����
				 sRecordItem.sItemInfo.CODE==(sinDA2|sin60) || sRecordItem.sItemInfo.CODE==(sinDA2|sin61) ||  // �庰 - ����

				 // �庰 - �ҿｺ�� ��� �����۵�
				sRecordItem.sItemInfo.CODE == (sinOR2|sin36)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin37)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin38)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin39)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin40)	||
				sRecordItem.sItemInfo.CODE == (sinOA1|sin39)	||
				sRecordItem.sItemInfo.CODE == (sinOA1|sin40)	||
				sRecordItem.sItemInfo.CODE == (sinOA1|sin41)	||
				sRecordItem.sItemInfo.CODE == (sinOA1|sin42)	||

				// �庰 - ���� �̺�Ʈ �ƹķ�, ��
				sRecordItem.sItemInfo.CODE == (sinOA1|sin38)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin34)	||
				sRecordItem.sItemInfo.CODE == (sinOR2|sin35)  || 

				// �庰 - ���� ����
				sRecordItem.sItemInfo.CODE == (sinDS1|sin31)	||
				sRecordItem.sItemInfo.CODE == (sinDS1|sin32)	||
				sRecordItem.sItemInfo.CODE == (sinDS1|sin33)	

			//	 sRecordItem.sItemInfo.CODE==(sinBI1|sin99) )  // �庰 - â��
			)


			{
				memcpy( &sItem_Buff.sItemInfo , &sRecordItem.sItemInfo , sizeof( sITEMINFO ) );
			//	OverDay_Item_Delete( &sItem_Buff ); // ��¥�� ������������ ��������� �޽���â���� �����ش�
			}
		}


//		if(pUsePotion->sItemInfo.PotionCount >=2){ //������ ���� ���δ� 

/*
		if ( sRecordItem.sItemInfo.CODE==(sinWS2|sin10) ) {
			//�߸��� Į ���� ( �ӵ� 6->7 )
			if ( sRecordItem.sItemInfo.Attack_Speed==6 ) {
				sRecordItem.sItemInfo.Attack_Speed = 7;
				//������ ���� �ޱ�
				ReformItem( &sRecordItem.sItemInfo );
			}
			//ũ��Ƽ�� 10->12
			if ( sRecordItem.sItemInfo.Critical_Hit<10 ) {
				sRecordItem.sItemInfo.Critical_Hit = 12;
				//������ ���� �ޱ�
				ReformItem( &sRecordItem.sItemInfo );
			}
			//Ưȭ���ݼӵ� 2->1
			if ( sRecordItem.sItemInfo.JobItem.Add_Attack_Speed==2 ) {
				sRecordItem.sItemInfo.JobItem.Add_Attack_Speed = 1;
				//������ ���� �ޱ�
				ReformItem( &sRecordItem.sItemInfo );
			}
			//Ưȭ���ݷ� 4 -> 5
			if ( sRecordItem.sItemInfo.JobItem.Lev_Damage[1]==4 ) {
				sRecordItem.sItemInfo.JobItem.Lev_Damage[1] = 5;
				//������ ���� �ޱ�
				ReformItem( &sRecordItem.sItemInfo );
			}

		}


		if ( sRecordItem.sItemInfo.CODE==(sinWA1|sin08) ) {
			//����
			//Ưȭ���ݷ� 6 -> 5
			if ( sRecordItem.sItemInfo.JobItem.Lev_Damage[1]==6 ) {
				sRecordItem.sItemInfo.JobItem.Lev_Damage[1] = 5;
				//������ ���� �ޱ�
				ReformItem( &sRecordItem.sItemInfo );
			}
		}

		if ( sRecordItem.sItemInfo.CODE==(sinWA1|sin10) ) {
			//����
			//Ưȭ���ݼӵ� 2->1 , 0->1
			if ( sRecordItem.sItemInfo.JobItem.Add_Attack_Speed==0 || sRecordItem.sItemInfo.JobItem.Add_Attack_Speed==2 ) {
				if ( sRecordItem.sItemInfo.JobCodeMask ) {
					sRecordItem.sItemInfo.JobItem.Add_Attack_Speed = 1;
					//������ ���� �ޱ�
					ReformItem( &sRecordItem.sItemInfo );
				}
			}
			if ( !sRecordItem.sItemInfo.JobCodeMask && sRecordItem.sItemInfo.JobItem.Add_Attack_Speed==1 ) {
				sRecordItem.sItemInfo.JobItem.Add_Attack_Speed = 0;
				//������ ���� �ޱ�
				ReformItem( &sRecordItem.sItemInfo );
			}
		}

		/////////////////////////////// 6��5�� ///////////////////////////////

		if ( sRecordItem.sItemInfo.CODE==(sinDS1|sin08) ) {
			//������ ����
			if ( sRecordItem.sItemInfo.JobCodeMask && 
				sRecordItem.sItemInfo.JobItem.Add_fBlock_Rating==4 ) {

				sRecordItem.sItemInfo.JobItem.Add_fBlock_Rating = 3;
				//������ ���� �ޱ�
				ReformItem( &sRecordItem.sItemInfo );
			}
		}
		if ( sRecordItem.sItemInfo.CODE==(sinDS1|sin09) ) {
			//Ŭ�� ����
			if ( sRecordItem.sItemInfo.JobCodeMask ) {
				if ( sRecordItem.sItemInfo.JobItem.Add_fBlock_Rating==4 ) {
					sRecordItem.sItemInfo.JobItem.Add_fBlock_Rating = 3;
					ReformItem( &sRecordItem.sItemInfo );	//������ ���� �ޱ�
				}
				if ( sRecordItem.sItemInfo.JobItem.Add_Defence>19 ) {
					sRecordItem.sItemInfo.JobItem.Add_Defence = 19;
					ReformItem( &sRecordItem.sItemInfo );	//������ ���� �ޱ�
				}
			}
		}
		if ( sRecordItem.sItemInfo.CODE==(sinDS1|sin10) ) {
			//Ŭ�� ����
			if ( sRecordItem.sItemInfo.JobCodeMask ) {
				if ( sRecordItem.sItemInfo.JobItem.Add_fBlock_Rating==5 ) {
					sRecordItem.sItemInfo.JobItem.Add_fBlock_Rating = 2;
					ReformItem( &sRecordItem.sItemInfo );	//������ ���� �ޱ�
				}
				if ( sRecordItem.sItemInfo.JobItem.Add_Defence>20 ) {
					sRecordItem.sItemInfo.JobItem.Add_Defence = 20;
					ReformItem( &sRecordItem.sItemInfo );	//������ ���� �ޱ�
				}
			}
			if ( sRecordItem.sItemInfo.fBlock_Rating>19 ) {
				sRecordItem.sItemInfo.fBlock_Rating = 19;
				ReformItem( &sRecordItem.sItemInfo );	//������ ���� �ޱ�
			}
		}

		if ( sRecordItem.sItemInfo.CODE==(sinWA1|sin06) ) {
			//��Ʈ����
			if ( !sRecordItem.sItemInfo.ItemKindCode ) {
				if ( sRecordItem.sItemInfo.Damage[0]>8 ) {
					sRecordItem.sItemInfo.Damage[0]=8;
					ReformItem( &sRecordItem.sItemInfo );	//������ ���� �ޱ�
				}
			}
		}
*/

		//CheckCopyItem( &sRecordItem.sItemInfo );				//���� �Ǵ� �ҷ������� �˻�
/*
		dwCode = (sRecordItem.sItemInfo.ItemHeader.Head>>2)^(sRecordItem.sItemInfo.ItemHeader.dwChkSum<<2);

		if ( !sRecordItem.sItemInfo.ItemHeader.dwTime ) {
			//�ڵ� ����
			sRecordItem.sItemInfo.ItemHeader.dwTime = dwCode;
			lpsItemInfo = &sRecordItem.sItemInfo;
			ReformItemCount ++;
		}
		if ( sRecordItem.sItemInfo.ItemHeader.dwTime==dwCode ) {
			//������ �õ�
			lpsItemInfo = &sRecordItem.sItemInfo;
			ReformItemCount ++;
		}
*/
		if ( SetFlag ) {

			//������ �ƴ� ������ �� �˻�
			if ( dwItemCode!=sinPM1 && dwItemCode!=sinPL1 && dwItemCode!=sinPS1 ) {
				for( cnt3=0;cnt3<ChkCnt;cnt3++ ) {
					/*
					if ( sRecordItem.sItemInfo.CODE==lpChkItem[cnt3]->sItemInfo.CODE &&
						sRecordItem.sItemInfo.ItemHeader.Head==lpChkItem[cnt3]->sItemInfo.ItemHeader.Head &&
						sRecordItem.sItemInfo.ItemHeader.dwChkSum==lpChkItem[cnt3]->sItemInfo.ItemHeader.dwChkSum ) {
					*/

					//������ 2���� ���Ѵ�
					if ( sRecordItem.sItemInfo.ItemName[0] && CompareItems( &sRecordItem.sItemInfo , &lpChkItem[cnt3]->sItemInfo )==TRUE ) {
						CopyItemCount++;
						break;
					}
				}
			}
			else {
				cnt3 = ChkCnt;
			}

			//2�� ���� ��
			if ( !sRecordItem.sItemInfo.Temp0 )
				sRecordItem.sItemInfo.Temp0 = sRecordItem.sItemInfo.ItemHeader.dwChkSum+sRecordItem.sItemInfo.CODE;

			if ( cnt3>=ChkCnt ) {

				if ( cnt<TransRecordData.ItemSubStart ) 
					lpSaveItem = &lpItems[sRecordItem.ItemCount];
				else
					lpSaveItem = &lpItems2[sRecordItem.ItemCount];


				if( sRecordItem.ItemPosition==-1 ) {
					//���콺�� ��� �ִ� ������
					if ( lpMouseItem ) 
						lpSaveItem = lpMouseItem;
					else
						lpSaveItem = &lpItems2[INVENTORY_MAXITEM-1];

					sRecordItem.ItemPosition = 0;
				}

				lpSaveItem->x = sRecordItem.x;
				lpSaveItem->y = sRecordItem.y;
				lpSaveItem->ItemPosition = sRecordItem.ItemPosition;
				lpSaveItem->Flag = TRUE;
				memcpy( &lpSaveItem->sItemInfo, &sRecordItem.sItemInfo , sizeof( sITEMINFO ) );

				if ( dwItemCode!=sinPM1 && dwItemCode!=sinPL1 && dwItemCode!=sinPS1 ) 
					lpChkItem[ChkCnt++] = lpSaveItem;

			}
/*
			if ( ReformItemCount<5 && lpsItemInfo ) {
			//if ( ReformItemCount<100 && lpsItemInfo ) {
				//�������� ������ ������ Ȯ��
				if ( lpsItemInfo->ItemName[0] ) {
					dwLastCheckItemTime = 0;
					SendCheckItemToServer( lpsItemInfo );
				}
			}
*/
		}
		lpRecItem += ((int *)lpRecItem)[0];
	}

	//���� ���� ����Ÿ ���
	RestoreGameData( &TransRecordData.GameSaveInfo );

	//���� �ȵ� �� ����
	if ( TransRecordData.GameSaveInfo.LastMoeny>0 ) {
		CheckCharForm();	//ĳ���� ���� ���� Ȯ��
		lpCharInfo->Money = TransRecordData.GameSaveInfo.LastMoeny-1;
		ReformCharForm();	//ĳ���� ���� ���� �ޱ�
	}
/*
	//�����Ҽ� �ִµ�
	cnt = lpCharInfo->Level*5-40;
	if ( cnt<10 ) cnt=10;
	cnt *= 10000;
	if ( lpCharInfo->Money>cnt ) {
		SendSetHackUser2( 1960, lpCharInfo->Money );			//�� �ѵ� �ʰ�
		CheckCharForm();	//ĳ���� ���� ���� Ȯ��
		lpCharInfo->Money = cnt;
		ReformCharForm();	//ĳ���� ���� ���� �ޱ�
		SendSaveMoney();
	}
*/

	if ( CopyItemCount ) {
		//���� �������� ������ ��� �Ű� 
		SendCopyItemUser( CopyItemCount );
	}
	if ( BadItemCount ) {
		//������ �������� ������ ��� �Ű�
		SendCopyItemUser( BadItemCount+10000 );
	}
	if ( PostionError ) {
		PostionError += 10000;
		SendSetHackUser2( 1900,PostionError );		//��������� ���� ����
		SendSetHackUser2( 99,0 );					//��������� ���� ����
	}

	if ( SvrPotionError ) {
		SvrPotionError += 20000;
		SendSetHackUser2( 1900,SvrPotionError );	//��������� ���� ����
		SendSetHackUser2( 99,0 );					//��������� ���� ����
	}



	//UnlockResource( hResource );
	CheckCharForm();
	cInvenTory.LoadItemInfo();
	CheckCharForm();

	//��ų ����
	RestoreSkill( &TransRecordData.GameSaveInfo.SkillInfo , TransRecordData.GameSaveInfo.dwLevelQuestLog );
	//�⺻���� ����� ����
	sinSkill.UseSkill[0].ShortKey = TransRecordData.GameSaveInfo.ShortKey_NormalAttack&0xF;
	sinSkill.UseSkill[0].MousePosi = TransRecordData.GameSaveInfo.ShortKey_NormalAttack>>4;
	
	// ����� - �ν��� ������(�����) ����
	if ( TransRecordData.GameSaveInfo.wLifeBoosterUsing[0] && TransRecordData.GameSaveInfo.wLifeBoosterUsing[1] ) {
		cSkill.SetBoosterItem( sinBC1+TransRecordData.GameSaveInfo.wLifeBoosterUsing[0] , TransRecordData.GameSaveInfo.wLifeBoosterUsing[1]*60 );

		lpCurPlayer->dwLifeBoosterCode = sinBC1+TransRecordData.GameSaveInfo.wLifeBoosterUsing[0];	// �ν��� ���� �ڵ�
		lpCurPlayer->dwLifeBoosterTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wLifeBoosterUsing[1]*1000; // �ν��� ��� �� ���� �ð� ����
	}
	// ����� - �ν��� ������(���) ����
	if ( TransRecordData.GameSaveInfo.wManaBoosterUsing[0] && TransRecordData.GameSaveInfo.wManaBoosterUsing[1] ) {
		cSkill.SetBoosterItem( sinBC1+TransRecordData.GameSaveInfo.wManaBoosterUsing[0] , TransRecordData.GameSaveInfo.wManaBoosterUsing[1]*60 );

		lpCurPlayer->dwManaBoosterCode = sinBC1+TransRecordData.GameSaveInfo.wManaBoosterUsing[0];	// �ν��� ���� �ڵ�
		lpCurPlayer->dwManaBoosterTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wManaBoosterUsing[1]*1000; // �ν��� ��� �� ���� �ð� ����
	}
	// ����� - �ν��� ������(�ٷ�) ����
	if ( TransRecordData.GameSaveInfo.wStaminaBoosterUsing[0] && TransRecordData.GameSaveInfo.wStaminaBoosterUsing[1] ) {
		cSkill.SetBoosterItem( sinBC1+TransRecordData.GameSaveInfo.wStaminaBoosterUsing[0] , TransRecordData.GameSaveInfo.wStaminaBoosterUsing[1]*60 );

		lpCurPlayer->dwStaminaBoosterCode = sinBC1+TransRecordData.GameSaveInfo.wStaminaBoosterUsing[0];	// �ν��� ���� �ڵ�
		lpCurPlayer->dwStaminaBoosterTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wStaminaBoosterUsing[1]*1000; // �ν��� ��� �� ���� �ð� ����
	}

	// �庰 - ��ų ������
	if ( TransRecordData.GameSaveInfo.wSkillDelayUsing[0] && TransRecordData.GameSaveInfo.wSkillDelayUsing[1] ) {
		cSkill.SetSkillDelayItem( sinBC1+TransRecordData.GameSaveInfo.wSkillDelayUsing[0] , TransRecordData.GameSaveInfo.wSkillDelayUsing[1]*60 );

		lpCurPlayer->dwSkillDelayCode = sinBC1+TransRecordData.GameSaveInfo.wSkillDelayUsing[0];	
		lpCurPlayer->dwSkillDelayTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wSkillDelayUsing[1]*1000;
	}

	//���� ���� ����
	if ( TransRecordData.GameSaveInfo.wForceOrbUsing[0] && TransRecordData.GameSaveInfo.wForceOrbUsing[1] ) {
		// ����� - ���� ���� ���� �߰� 
		if( TransRecordData.GameSaveInfo.wForceOrbUsing[0]>=sin01 && TransRecordData.GameSaveInfo.wForceOrbUsing[0]<=sin12 ) // �Ϲ� ����
		{
			cInvenTory.SetForceOrb( sinFO1+TransRecordData.GameSaveInfo.wForceOrbUsing[0] , TransRecordData.GameSaveInfo.wForceOrbUsing[1] );
		}
		else if( TransRecordData.GameSaveInfo.wForceOrbUsing[0]>=sin21 && TransRecordData.GameSaveInfo.wForceOrbUsing[0]<=sin32 ) // ���� ����
		{
			cInvenTory.SetMagicForceOrb( sinFO1+TransRecordData.GameSaveInfo.wForceOrbUsing[0] , TransRecordData.GameSaveInfo.wForceOrbUsing[1] );
		}
		else if( TransRecordData.GameSaveInfo.wForceOrbUsing[0]>=sin35 && TransRecordData.GameSaveInfo.wForceOrbUsing[0]<=sin37 ) // ���� ���� ����
		{
			cInvenTory.SetBillingMagicForceOrb( sinFO1+TransRecordData.GameSaveInfo.wForceOrbUsing[0] , TransRecordData.GameSaveInfo.wForceOrbUsing[1] );
		}

		lpCurPlayer->dwForceOrbCode = sinFO1+TransRecordData.GameSaveInfo.wForceOrbUsing[0];		//�������� ���� �ڵ�
		lpCurPlayer->dwForceOrbTime = dwPlayTime + (DWORD)TransRecordData.GameSaveInfo.wForceOrbUsing[1]*1000;
		AssaParticle_ShelltomWeapon( lpCurPlayer ,  (DWORD)TransRecordData.GameSaveInfo.wForceOrbUsing[1]*70, ((TransRecordData.GameSaveInfo.wForceOrbUsing[0])>>8)-1 );
	}

	//�����̾� ������ ���ð� ǥ��
	int	PrimeItem_Time;
	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_X2 ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_X2-GetPlayTime_T();
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetThirdEyesTime( PrimeItem_Time );
			switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode){
				case PRIME_ITEM_PACKAGE_NONE:
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],50);
				break;
				case PRIME_ITEM_PACKAGE_BRONZE:
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],20);
				break;
				case PRIME_ITEM_PACKAGE_SILVER:	
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],30);
				break;
				case PRIME_ITEM_PACKAGE_GOLD:
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],40);
				break;
				case PRIME_ITEM_PACKAGE_ULTRA:	// pluto ���丮�� ��Ű��
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],50);
				break;
				default:	// ����� - ����ġ���� ����(50%)�����۰� �ߺ� ����� ���(PRIME_ITEM_PACKAGE_NONE_50_EXPUP , PRIME_ITEM_PACKAGE_NONE_100_EXPUP)
					chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0],50);
				break;
			}
		}
	}
	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_ExpUp ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_ExpUp-GetPlayTime_T();
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetExpUpPotionTime( PrimeItem_Time );
			switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode){
			case PRIME_ITEM_PACKAGE_NONE:
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],30);
				break;
			case PRIME_ITEM_PACKAGE_BRONZE:
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],10);
				break;
			case PRIME_ITEM_PACKAGE_SILVER:	
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],20);
				break;
			case PRIME_ITEM_PACKAGE_GOLD:
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],30);
				break;
#ifdef _LANGUAGE_VEITNAM
			case PRIME_ITEM_PACKAGE_ULTRA:	//��Ʈ����û
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],50);
				break;
#else
			case PRIME_ITEM_PACKAGE_ULTRA:
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],40);
				break;
#endif

			case PRIME_ITEM_PACKAGE_BRONZE2:	// pluto ����� ��Ű��2
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],10);
				break;
			case PRIME_ITEM_PACKAGE_SILVER2:	// pluto �ǹ� ��Ű��2
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],20);
				break;
			case PRIME_ITEM_PACKAGE_GOLD2:	// pluto ��� ��Ű��2
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],30);
				break;
			case PRIME_ITEM_PACKAGE_ULTRA2: // pluto ���丮�� ��Ű��2
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],40);
				break;
			case PRIME_ITEM_PACKAGE_NONE_50_EXPUP: // ����� - ����ġ���� ����(50%) ������ ���� 
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],50);
				break;
			case PRIME_ITEM_PACKAGE_NONE_100_EXPUP: // ����� - ����ġ���� ����(100%) ������ ���� 
				chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1],100);
				break;
			}
		}
	}

	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_VampCuspid ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_VampCuspid-GetPlayTime_T();
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetVampiricCuspidTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS,chaPremiumitem.m_VampiricCuspidTime,true,UpKeepItemName[2]);
		}
	}
	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_ManaRecharg ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_ManaRecharg-GetPlayTime_T();
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetManaRechargingPTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::MANA_RECHAR_P,chaPremiumitem.m_ManaRechargingPTime,true,UpKeepItemName[3]);
		}
	}

	// �庰 - ���Ǹ� Ŀ���� EX
	if ( TransRecordData.GameSaveInfo.dwTime_PrimeItem_VampCuspid_EX ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_VampCuspid_EX-GetPlayTime_T();
		if ( PrimeItem_Time>0 ){
			chaPremiumitem.SetVampiricCuspidEXTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS_EX,chaPremiumitem.m_VampiricCuspidEXTime,true,UpKeepItemName[11]);
		}
	}

	/*
	if (TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode ) {
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode - GetPlayTime_T();
		if( PrimeItem_Time < 0 ){
			
			chaPremiumitem.SetThirdEyesTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES,chaPremiumitem.m_ThirdEyesTime,true,UpKeepItemName[0]);
			
			chaPremiumitem.SetExpUpPotionTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION,chaPremiumitem.m_ExpUpPotionTime,true,UpKeepItemName[1]);
		}
	}
	*/

	// pluto ���� ���ེ ����
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_ManaReduce )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_ManaReduce-GetPlayTime_T();
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetManaReducePotionTime( PrimeItem_Time );
			switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode )
			{
				case PRIME_ITEM_PACKAGE_NONE:
					chaPremiumitem.SetManaReducePotionValue( 30 );	// pluto ���� ���ེ ���� 30% ����
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 30);
					break;
				case PRIME_ITEM_PACKAGE_BRONZE2:
					chaPremiumitem.SetManaReducePotionValue( 10 );	// pluto ���� ���ེ ���� 10% ����
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 10);
					break;
				case PRIME_ITEM_PACKAGE_SILVER2:
					chaPremiumitem.SetManaReducePotionValue( 20 );	// pluto ���� ���ེ ���� 20% ����
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 20);
					break;
				case PRIME_ITEM_PACKAGE_GOLD2:
					chaPremiumitem.SetManaReducePotionValue( 30 );	// pluto ���� ���ེ ���� 30% ����
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 30);
					break;
				case PRIME_ITEM_PACKAGE_ULTRA2:
					chaPremiumitem.SetManaReducePotionValue( 40 );	// pluto ���� ���ེ ���� 40% ����
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 40);
					break;
				default:	// ����
					chaPremiumitem.SetManaReducePotionValue( 30 );	// pluto ���� ���ེ ���� 30% ����
					chaPremiumitem.SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, chaPremiumitem.m_ManaReducePotiontime, true, UpKeepItemName[5], 30);
					break;
			}
		}
	}
	// pluto ����Ʈ ���� ����
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell-GetPlayTime_T();
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetMightOfAwellTime( PrimeItem_Time );
			chaPremiumitem.SetMightOfAwellWeight( 300 );
			chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 300);
			//switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode )
			//{
			//	case PRIME_ITEM_PACKAGE_NONE:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 20);
			//		break;
			//	case PRIME_ITEM_PACKAGE_BRONZE2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 10);
			//		break;
			//	case PRIME_ITEM_PACKAGE_SILVER2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 20);
			//		break;
			//	case PRIME_ITEM_PACKAGE_GOLD2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 30);
			//		break;
			//	case PRIME_ITEM_PACKAGE_ULTRA2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 40);
			//		break;
			//}
		}
	}
	// pluto ����Ʈ ���� ����2
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell2 )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_MightofAwell2-GetPlayTime_T();
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetMightOfAwellTime( PrimeItem_Time );
			chaPremiumitem.SetMightOfAwellWeight( 500 );
			chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 500);
			//switch( TransRecordData.GameSaveInfo.dwPrimeItem_PackageCode )
			//{
			//	case PRIME_ITEM_PACKAGE_NONE:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 20);
			//		break;
			//	case PRIME_ITEM_PACKAGE_BRONZE2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 10);
			//		break;
			//	case PRIME_ITEM_PACKAGE_SILVER2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 20);
			//		break;
			//	case PRIME_ITEM_PACKAGE_GOLD2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 30);
			//		break;
			//	case PRIME_ITEM_PACKAGE_ULTRA2:
			//		chaPremiumitem.SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, chaPremiumitem.m_MightofAwellTime, true, UpKeepItemName[4], 40);
			//		break;
			//}
		}
	}

	// pluto ��(�ؿ�)
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_PhenixPet )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_PhenixPet-GetPlayTime_T();
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetPhenixPetTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem( nsPremiumItem::PHENIX_PET, chaPremiumitem.m_PhenixPetTime, true, UpKeepItemName[6], 500);
			cPCBANGPet.PetKind = TRUE;
			cPCBANGPet.ShowPet();
		}
		else if( PrimeItem_Time < 0 ) 
		{
			lpCharInfo->GravityScroolCheck[1] = 0;
			cPCBANGPet.ClosePet();	
		}
	}

	// ����� - ���� ����� �� �߰�
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_HelpPet )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_HelpPet-GetPlayTime_T();
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetHelpPetTimeTime( PrimeItem_Time );
			chaPremiumitem.SetUpKeepItem( cHelpPet.PetKind+7, chaPremiumitem.m_HelpPetTime, true, UpKeepItemName[cHelpPet.PetKind+6], 500);
			//	chaPremiumitem.SetUpKeepItem( nsPremiumItem::HELP_PET, chaPremiumitem.m_HelpPetTime, true, UpKeepItemName[6], 500);

			 for( int m = 0 ; m < SINUPKEEPITEM_MAX ; m++ )
			{
				if(chaPremiumitem.UpKeepItem[m].TGAImageNumber == nsPremiumItem::HELP_PET_TERRY)
				{
					lpCharInfo->GravityScroolCheck[1] = 1;
				}
			}

			cHelpPet.ShowPet(); 
		}
		else if( PrimeItem_Time < 0 ) 
		{
			lpCharInfo->GravityScroolCheck[1] = 0;
			cHelpPet.ClosePet();	
		}
	}

	else 
	{
		lpCharInfo->GravityScroolCheck[1] = 0;
		cHelpPet.ClosePet(); // ����� - ����� �� ���� ���� ����
	}

	// ����� - �ٷ� ���ེ ����
	if( TransRecordData.GameSaveInfo.dwTime_PrimeItem_StaminaReduce )
	{
		PrimeItem_Time = TransRecordData.GameSaveInfo.dwTime_PrimeItem_StaminaReduce-GetPlayTime_T();
		if( PrimeItem_Time > 0 )
		{
			chaPremiumitem.SetStaminaReducePotionTime( PrimeItem_Time );
			chaPremiumitem.SetStaminaReducePotionValue( 30 );	// ����� - �ٷ� ���ེ ���� 30% ����
			chaPremiumitem.SetUpKeepItem( nsPremiumItem::STAMINA_REDUCE_P, chaPremiumitem.m_StaminaReducePotiontime, true, UpKeepItemName[12], 30);

		}
	}

	return TRUE;
}


//������ ����Ÿ�� �����Ͽ� ������ ����
int rsRECORD_DBASE::SendRecordDataToServer( smWINSOCK *lpsmSock  )
{
	int cnt;
	int PartTotal;
	char *lpData;
	int TotalLen;
	int len;

	TRANS_RECORD_DATAS TransRecord;
	TRANS_RECORD_DATA *lpTransRecord;

	lpTransRecord = &TransRecordData;
	lpData = (char *)lpTransRecord;

	cnt = 0;
	TotalLen = 0;
	PartTotal = lpTransRecord->size / TRANS_RECORD_LEN;
	if ( (lpTransRecord->size%TRANS_RECORD_LEN)!=0 ) {
		if ( lpTransRecord->size>TRANS_RECORD_LEN )
			PartTotal++;
	}

	while(1) {

		len = lpTransRecord->size - TotalLen;
		if ( len>TRANS_RECORD_LEN ) len = TRANS_RECORD_LEN;

		TransRecord.code = smTRANSCODE_RECORDDATA;
		TransRecord.size = len+32;
		TransRecord.Count = cnt;
		TransRecord.Total = PartTotal;
		TransRecord.TransSize = len;

		memcpy( TransRecord.szData , &lpData[TotalLen] , len );

		lpsmSock->Send2( (char *)&TransRecord , TransRecord.size , TRUE );
		cnt++;

		TotalLen+=len;
		if ( TotalLen>=lpTransRecord->size ) break;
	}

	return TRUE;
}

//�� ��� ���Ϸ� ����
extern int RecordHackLogMoney( smCHAR_INFO *lpCharInfo );
//������ ����Ÿ�� �����Ͽ� Ŭ���̾�Ʈ�� ����
int rsRECORD_DBASE::SendRecordDataToClient( rsPLAYINFO *lpPlayInfo , char *szName , smWINSOCK *lpsmSock , int Mode )
{
	int cnt;
	int PartTotal;
	char *lpData;
	int TotalLen;
	int len;
	char szFile[256];
	FILE *fp;
	INT64	exp64;
	smTRANS_COMMAND	smTransCommand;

	TRANS_RECORD_DATAS TransRecord;
	TRANS_RECORD_DATA *lpTransRecord;

	lpData = (char *)&TransRecordData;
	lpTransRecord = &TransRecordData;

	switch( Mode ) {
		case 0:
		//���Ϻҷ�����
			GetUserDataFile( szName , szFile );
			break;

		case 1:
			//�������� �ҷ�����
			GetDeleteDataFile( szName , szFile );
			break;

		case 2:
			//������� �ҷ�����
			GetUserDataBackupFile( szName , szFile );
			break;
	}

	if ( Mode>1000 ) {
		//20020912
		GetUserDataFile_BackupDay( szName , szFile , Mode );

		WIN32_FIND_DATA		ffd;
		HANDLE				hFind;
 		hFind = FindFirstFile( szFile , &ffd );
		if ( hFind==INVALID_HANDLE_VALUE ) return FALSE;
		FindClose( hFind );
	}

/*
	if ( Mode==TRUE ) {
		//�������� �ҷ�����
		GetDeleteDataFile( szName , szFile );
	}
	else {
		//���Ϻҷ�����
		GetUserDataFile( szName , szFile );
	}
*/
	if ( sRecDataBuff ) EnterCriticalSection( &cSaveDataSection );

	//���� ������� ����Ÿ �ִ��� Ȯ��
	if ( CheckRecWaitData( szName )==TRUE ) return FALSE;

	lpTransRecord->code = 0;
	lpTransRecord->size = 0;

	fp = fopen( szFile , "rb" );
	if ( fp ) {
		fread( lpTransRecord , sizeof(TRANS_RECORD_DATA) , 1, fp );
		fclose( fp );
	}
	else {
		if ( sRecDataBuff ) LeaveCriticalSection( &cSaveDataSection );
		return FALSE;
	}
	if ( sRecDataBuff ) LeaveCriticalSection( &cSaveDataSection );


	if ( lpTransRecord->code==0 && lpTransRecord->size==0 ) 
		return FALSE;			//�ӽ� ���� ����Ÿ �̹Ƿ� ���� ( �ű� ĳ���� �������� )

	if ( lpTransRecord->size<0 || lpTransRecord->size>sizeof(TRANS_RECORD_DATA) )
		return FALSE;			//���� ����Ÿ

/*
	//���� 32��Ʈ ����ġ �ʱ�ȭ
	if ( lpTransRecord->smCharInfo.Level<80 && lpTransRecord->smCharInfo.Exp_High ) {
		lpTransRecord->smCharInfo.Exp_High = 0;
	}
*/

	exp64 = GetExp64( &lpTransRecord->smCharInfo );
	if ( exp64<0 ) {	//����ġ ���� 0���� �۴�
		exp64 = 0;
		SetExp64( &lpTransRecord->smCharInfo , exp64 );
	}

	//������ ����ġ�� �´��� Ȯ��
	if ( CheckLevelExp( lpTransRecord->smCharInfo.Level , exp64 )==FALSE ) {
		//����ġ�� ���� �߻�
		lpTransRecord->smCharInfo.Level = GetLevelFromExp( exp64 );
	}

#ifndef _MODE_EXP64
	if ( lpTransRecord->smCharInfo.Level>=80 || GetExp64( &lpTransRecord->smCharInfo )>ExpLevelTable[99] ) {
		smTransCommand.WParam = 8560;
		smTransCommand.LParam = lpTransRecord->smCharInfo.Level;
		smTransCommand.SParam = lpTransRecord->smCharInfo.Exp;
		smTransCommand.EParam = 0;
		RecordHackLogFile( lpPlayInfo , &smTransCommand );

		//���� 1 �� �ٿ�
		lpTransRecord->smCharInfo.Level = 1;
		lpTransRecord->smCharInfo.Exp = 0;
		lpTransRecord->smCharInfo.Exp_High = 0;
		fp = fopen( szFile , "wb" );
		if ( fp ) {
			fwrite( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1, fp );
			fclose( fp );
		}
		return FALSE;
	}
#endif

	//���� ����ġ ��� ���� ����
	lpTransRecord->smCharInfo.Next_Exp = (int)GetNextExp( lpTransRecord->smCharInfo.Level );

	//ĳ���� ������Ʈ ���� ���Ӱ� �����Ѵ�
	ReformCharStatePoint( &lpTransRecord->smCharInfo , lpTransRecord->GameSaveInfo.dwLevelQuestLog );

	lpTransRecord->smCharInfo.ClassClan = 0;			//Ŭ�� ���� �ʱ�ȭ

	//���� 20 ���ϰ� ���� ������ ���� ���
	if ( lpTransRecord->smCharInfo.Level<20 && lpTransRecord->smCharInfo.ChangeJob!=0 ) {
		lpTransRecord->smCharInfo.ChangeJob = 0;
	}

	lpTransRecord->GameSaveInfo.PCRNo			= lpPlayInfo->Bl_RNo;				//PC�� ����
	lpTransRecord->GameSaveInfo.EventPlay[0]	= lpPlayInfo->Bl_Meter;				//��� �̺�Ʈ

	lpTransRecord->GameSaveInfo.sPotionUpdate[1] = rsServerConfig.PotionMonitor;

	lpTransRecord->GameSaveInfo.BlessCastleTax = rsBlessCastle.Tax;					//���� ĳ�� ����
	lpTransRecord->GameSaveInfo.dwBlessCastleMaster = rsBlessCastle.dwMasterClan;	//���� ĳ�� ������

	//�ʿ����� ���� ������
	lpPlayInfo->sLowLevel_PresentItem[0] = lpTransRecord->smCharInfo.sPresentItem[0];
	lpPlayInfo->sLowLevel_PresentItem[1] = lpTransRecord->smCharInfo.sPresentItem[1];
	//���Ӽ����� ������Ʈ ���� ����
	rsUpdateServerParam( lpPlayInfo , smUPDATE_PARAM_LOWUSER_PRESENT , lpPlayInfo->sLowLevel_PresentItem[0] , lpPlayInfo->sLowLevel_PresentItem[1] , FALSE );


	if ( lpPlayInfo->AdminMode && lpPlayInfo->sLowLevel_PresentItem[0]==0 )
		lpPlayInfo->sLowLevel_PresentItem[0] = 1;

	//��ʸ�Ʈ ���� ID
	if ( lpPlayInfo->szServerID[0] )
		lpTransRecord->GameSaveInfo.TT_ServerID = ((DWORD *)lpPlayInfo->szServerID)[0];
	else
		lpTransRecord->GameSaveInfo.TT_ServerID = 0;


	//SoD�� ���� ����
	if ( lpTransRecord->GameSaveInfo.PlayStageNum==rsSOD_FIELD ) lpTransRecord->GameSaveInfo.PlayStageNum = rsSOD_VILLAGE;
	if ( lpTransRecord->GameSaveInfo.PlayStageNum==QUEST_ARENA_FIELD ) {	//���� ������
		if (lpTransRecord->smCharInfo.JOB_CODE <= 4 || lpTransRecord->smCharInfo.JOB_CODE == 9)
			lpTransRecord->GameSaveInfo.PlayStageNum = START_FIELD_NUM;
		else
			lpTransRecord->GameSaveInfo.PlayStageNum = START_FIELD_MORYON;
	}

	////////////////// ������ �ǽɵǴ� ��� /////////////////////
/*
	if ( lpTransRecord->smCharInfo.Money>=8000000 ) {
		//800���� �̻� ������
		RecordHackLogMoney( &lpTransRecord->smCharInfo );
	}
*/
	////////////////////////////////////////////////////////////
	//if ( !lpTransRecord->smCharInfo.dwObjectSerial )
	if ( !lpPlayInfo->dwObjectSerial )
		lpTransRecord->smCharInfo.dwObjectSerial = GetNewObjectSerial();			//���� ������ȣ ����
	else
		lpTransRecord->smCharInfo.dwObjectSerial = lpPlayInfo->dwObjectSerial;

	//��ū�� ���� �̺�Ʈ
	if ( rsServerConfig.Event_ComicBirthDay==1 && (lpPlayInfo->Bl_Meter&BIMASK_BIRTHDAY_USER)!=0 ) {
		lpTransRecord->smCharInfo.SizeLevel = (rand()%1)+0x1001;
		lpTransRecord->smCharInfo.dwEventTime_T = ( DWORD )tServerTime + ( 60 * 24 * 6 );
	}

	if ( (lpPlayInfo->Bl_Meter&BIMASK_VIP_USER)!=0 && rsServerConfig.FreeLevel && lpTransRecord->smCharInfo.Level<rsServerConfig.FreeLevel ) {
		lpTransRecord->GameSaveInfo.EventPlay[0] -=BIMASK_VIP_USER;		//�׾����� ��ġ Ȱ�� �̺�Ʈ ���� (���᷹�� �����)
	}

	if ( rsServerConfig.EventPlay_BitMask )
		lpTransRecord->GameSaveInfo.EventPlay[0] |= (short)rsServerConfig.EventPlay_BitMask;		//�̺�Ʈ �÷��� ���� ��Ʈ����


	//ĳ���� �Ӽ� ���� Ȯ��
	if ( !lpPlayInfo->AdminMode ) {
		if ( lpTransRecord->smCharInfo.dwCharSoundCode!=0 || lpTransRecord->smCharInfo.State!=smCHAR_STATE_USER ) {

			//ĳ���� �⺻ �Ӽ� ����
			smTransCommand.WParam = 8730;
			smTransCommand.LParam = lpTransRecord->smCharInfo.State;
			smTransCommand.SParam = lpTransRecord->smCharInfo.dwCharSoundCode;
			smTransCommand.EParam = 0;
			RecordHackLogFile( lpPlayInfo , &smTransCommand );

			//���� / ����
			lpTransRecord->smCharInfo.dwCharSoundCode = 0;
			lpTransRecord->smCharInfo.State = smCHAR_STATE_USER;
		}
	}

	//ĳ���� ���� ����
	ReformCharForm( &lpTransRecord->smCharInfo );

	if ( rsServerConfig.Event_StarPointTicket ) 
		OpenStarPointEvent( lpPlayInfo , &lpTransRecord->smCharInfo );		//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����


	//������ 140 ���Ϲ���
	if ( lpTransRecord->GameSaveInfo.Head<140 &&
		lstrcmp( lpTransRecord->szHeader , szRecordHeader )!=0 ) {
		//������ ����Ÿ �߰� ĳ���� ���� �ڵ� ���� ���
		lpTransRecord->GameSaveInfo.dwChkSum_CharInfo = GetCharInfoCode( &lpTransRecord->smCharInfo );
	}

	//if (  rsServerConfig.TestSeverMode || lpTransRecord->GameSaveInfo.SaveTime<(tServerTime-(60*60*24*10)) ) {		//�������� 10�� �̻�� ����Ÿ �÷��ʱ�ȭ
		//�׽�Ʈ ������ �ɷ�ġ �ʱ�ȭ �÷��� 0���� ���� ( �ɷ�ġ �ʱ�ȭ Ƚ�� ���� )
		lpTransRecord->smCharInfo.wVersion[1] = 0;
	//}


	if ( rsServerConfig.FixedStartField )
		lpTransRecord->GameSaveInfo.PlayStageNum = rsServerConfig.FixedStartField;				//�����ʵ� ���� ����

	//�α��� ���� ����Ű ����
	rsSet_LoginServerSafeKey( &lpTransRecord->smCharInfo );


/*
	//������ 140->150 ������
	if ( lpTransRecord->GameSaveInfo.Head<dwRecordVersion ) {
		//����� ������ ����Ÿ�� 150 �������� ������ȯ
		ConvertItem_Server150(lpTransRecord);
	}
*/


	cnt = 0;
	TotalLen = 0;
	PartTotal = lpTransRecord->size / TRANS_RECORD_LEN;
	if ( (lpTransRecord->size%TRANS_RECORD_LEN)!=0 ) {
		if ( lpTransRecord->size>TRANS_RECORD_LEN )
			PartTotal++;
	}

	while(1) {

		len = lpTransRecord->size - TotalLen;
		if ( len>TRANS_RECORD_LEN ) len = TRANS_RECORD_LEN;

		TransRecord.code = smTRANSCODE_RECORDDATA;
		TransRecord.size = len+32;
		TransRecord.Count = cnt;
		TransRecord.Total = PartTotal;
		TransRecord.TransSize = len;

		memcpy( TransRecord.szData , &lpData[TotalLen] , len );

		lpsmSock->Send2( (char *)&TransRecord , TransRecord.size , TRUE );
		cnt++;

		TotalLen+=len;
		if ( TotalLen>=lpTransRecord->size ) break;
	}
/*
	if ( TransRecordData.smCharInfo.Life[0]==0 ) {
		//�����鼭 ����� ��� ���� ��ǥ ������ ���� �� ����
		TransRecordData.smCharInfo.Life[0] = 1;
		TransRecordData.GameSaveInfo.PlayStageNum = 3;
		TransRecordData.GameSaveInfo.PosX = 746752;
		TransRecordData.GameSaveInfo.PosZ = -4464384;

		fp = fopen( szFile , "wb" );
		if ( fp ) {
			fwrite( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1, fp );
			fclose( fp );
		}
	}
*/
	return TRUE;
}

#define	RECORD_ITEM_INFO_HEAD_SIZE		44

//�ҷ��� ������ ����� ���������� ����
int rsRECORD_DBASE::MakeRecordItemList( rsPLAYINFO *lpPlayInfo )
{
	int cnt,cnt2;

	BYTE	*lpRecItem;
	sRECORD_ITEM	sRecordItem;
	int		size;
	int		BuffSize;
	smTRANS_COMMAND_EX	smTransCommand;

	sTHROW_ITEM_INFO	ThrowItemInfo[THROW_ITEM_INFO_MAX];			//������ ������ ����
	int					ThrowItemCount;								//������ ������ ī����

	lpRecItem = (BYTE *)&TransRecordData.Data;
	BuffSize = 0;
	ThrowItemCount = 0;

	ZeroMemory( lpPlayInfo->ServerPotion , sizeof(int)*3*4 );

	if ( TransRecordData.ItemCount>RECORD_ITEM_MAX )  TransRecordData.ItemCount = RECORD_ITEM_MAX; 
	if ( TransRecordData.ThrowItemCount>THROW_ITEM_INFO_MAX ) TransRecordData.ThrowItemCount = THROW_ITEM_INFO_MAX;

	for( cnt=0;cnt<TransRecordData.ItemCount;cnt++ ) {
		if ( cnt>=(INVENTORY_MAXITEM*2) ) break;

		if ( rsServerConfig.PotionMonitor ) {
			//���൥��Ÿ ���� ������ �ص��� ( Z/NZ ��� ) - ���� �迭 �������� Ȯ���Ͽ� ���� �ľ�
			DecodeCompress_ItemPotion( lpPlayInfo , (BYTE *)lpRecItem , (BYTE *)&sRecordItem , RECORD_ITEM_INFO_HEAD_SIZE , &TransRecordData );
		}
		else {
			//���� ����Ÿ �ص� ( Z/NZ ��� )
			DecodeCompress( (BYTE *)lpRecItem , (BYTE *)&sRecordItem , RECORD_ITEM_INFO_HEAD_SIZE );
		}

		if ( sRecordItem.sItemInfo.CODE && sRecordItem.sItemInfo.ItemHeader.Head && 
			sRecordItem.sItemInfo.ItemHeader.dwChkSum &&
			(sRecordItem.sItemInfo.CODE&sinITEM_MASK1)!=(sinPM1&sinITEM_MASK1) ) {

			//������ ������ Ȯ��
			for( cnt2=0;cnt2<TransRecordData.ThrowItemCount;cnt2++ ) {
				if ( TransRecordData.ThrowItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
					TransRecordData.ThrowItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
					TransRecordData.ThrowItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

						if ( ThrowItemCount<THROW_ITEM_INFO_MAX ) {
							//������ �����ϴ� ������ Ȯ�� ( ������ ������ ��� ���� )
							memcpy( &ThrowItemInfo[ThrowItemCount++] , &TransRecordData.ThrowItemInfo[cnt2] ,sizeof(sTHROW_ITEM_INFO) );
						}
						break;
					}
			}

			if ( cnt2>=TransRecordData.ThrowItemCount ) {
				for( cnt2=0;cnt2<cnt;cnt2++ ) {
					//��������� �˻�
					if ( lpPlayInfo->InvenItemInfo[cnt2].dwCode && 
						lpPlayInfo->InvenItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
						lpPlayInfo->InvenItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
						lpPlayInfo->InvenItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

							//�α׿� ���
							smTransCommand.WParam = 8070;
							smTransCommand.WxParam = 1;
							smTransCommand.LxParam = (int)"*INVENTORY";
							smTransCommand.LParam = sRecordItem.sItemInfo.CODE;
							smTransCommand.SParam = sRecordItem.sItemInfo.ItemHeader.Head;
							smTransCommand.EParam = sRecordItem.sItemInfo.ItemHeader.dwChkSum;
							RecordHackLogFile( lpPlayInfo , &smTransCommand );
							break;
						}
				}
				if ( cnt2>=cnt ) {
					lpPlayInfo->InvenItemInfo[cnt].dwCode = sRecordItem.sItemInfo.CODE;
					lpPlayInfo->InvenItemInfo[cnt].dwKey  = sRecordItem.sItemInfo.ItemHeader.Head;
					lpPlayInfo->InvenItemInfo[cnt].dwSum  = sRecordItem.sItemInfo.ItemHeader.dwChkSum;
				}
			}
		}

		size = ((int *)lpRecItem)[0];
		BuffSize += size;
		lpRecItem += size;
		if ( BuffSize>=(sizeof(sRECORD_ITEM)*RECORD_ITEM_MAX) ) break;			//���� ũ�� �ʰ���
	}


	lpPlayInfo->ThrowItemCount = ThrowItemCount;
	memcpy( lpPlayInfo->ThrowItemInfo , ThrowItemInfo , sizeof(sTHROW_ITEM_INFO)*ThrowItemCount );


	return TRUE;
}


//������ ����Ÿ�� �����Ͽ� ����
int rsRECORD_DBASE::RecvRecordDataFromServer( TRANS_RECORD_DATAS *lpTransRecord )
{

	char *lpData;

	lpData = (char *)&TransRecordData;

	memcpy( &lpData[lpTransRecord->Count*TRANS_RECORD_LEN] ,  lpTransRecord->szData , lpTransRecord->TransSize );
	TransDataBlock[lpTransRecord->Count] = 1;

	int cnt = 0;
	for(cnt=0;cnt<lpTransRecord->Total;cnt++)
		if ( !TransDataBlock[cnt] ) break;

	if ( cnt==lpTransRecord->Total && TransRecordData.code==smTRANSCODE_RECORDDATA ) {
		//���� �Ϸ�
		TransRecordData.code=0;
		return TRUE;
	}

/*
	if ( lpTransRecord->Count>0 ) {
		if ( TransLastPartCount!=lpTransRecord->Count-1 ) 
			return FALSE;		//���� ����
	}

	if ( lpTransRecord->Count>=lpTransRecord->Total-1 && TransRecordData.code==smTRANSCODE_RECORDDATA ) {
		//���� �Ϸ�
		TransRecordData.code=0;
		return TRUE;
	}
*/

	TransLastPartCount = lpTransRecord->Count;

	return FALSE;
}

#ifdef _W_SERVER
int	CheckPlayExpTable[15] = {
	40000,			//0
	100000,			//1
	300000,			//2
	500000,			//3
	800000,			//4
	1200000,		//5
	1600000,		//6
	2000000,		//7
	3000000,		//8
	4000000,		//9
	4000000,		//10
	6000000,		//11
	6000000,		//12
	8000000,		//13
	8000000			//14
};
#else
int	CheckPlayExpTable[15] = {
	0,		//0
	0,		//1
	0,		//2
	0,		//3
	0,		//4
	0,		//5
	0,		//6
	0,		//7
	0,		//8
	0,		//9
	0,		//10
	0,		//11
	0,		//12
	0,		//13
	0		//14
};
#endif

//������ ����Ÿ�� �����Ͽ� ����
int rsRECORD_DBASE::RecvRecordDataFromClient( TRANS_RECORD_DATAS *lpTransRecord , rsPLAYINFO *lpPlayInfo )
//int Level , int CrackUser , char *lpRecordMemBuff )
{

	char *lpData;
	//char *lpHexBuff;
	char szFile[256];
	char szBackupFile[256];
	FILE	*fp;
	int		size;
	int		cnt;
	int		CopyVal;
	char	*lpBuff;
	smTRANS_COMMAND	smTransCommand;

	int	Level = lpPlayInfo->smCharInfo.Level;
	int CrackUser = lpPlayInfo->CrackWarning;
	char *lpRecordMemBuff = lpPlayInfo->lpRecordDataBuff;

	lpData = (char *)&TransRecordData;


	memcpy( &lpData[lpTransRecord->Count*TRANS_RECORD_LEN] ,  lpTransRecord->szData , lpTransRecord->TransSize );

	if ( lpTransRecord->Count>0 ) {
		if ( TransLastPartCount!=lpTransRecord->Count-1 ) 
			return -4;		//���� ����
	}
	else {
		//ù���� ���� ����Ÿ�� ���� ���� ��� ����
		if ( lpTransRecord->Count!=TransLastPartCount ) return -4;
	}

	if ( lpTransRecord->Count>=lpTransRecord->Total-1 && TransRecordData.code==smTRANSCODE_RECORDDATA ) {
		//���Ϸ� ����
		//wsprintf( szFile , "userdata\\%s.dat" , TransRecordData.smCharInfo.szName );

		TransRecordData.GameSaveInfo.SaveTime = (int)tServerTime;		//���� ���� �ð� ( Time_T ��� )

		if ( CheckCharForm( &TransRecordData.smCharInfo )==FALSE ) {
			//ĳ���� ���� �ڵ� Ȯ�� ����
			TransRecordData.code=0;
			return -1;
		}

		if ( lpPlayInfo->AdminMode<3 && lstrcmp( TransRecordData.smCharInfo.szName , lpPlayInfo->szName )!=0 ) {
			//ĳ���� �̸��� Ʋ�� ����
			TransRecordData.code=0;
			return -15;
		}

		if ( TransRecordData.size<=0 ) {
			return -4;						//ũ�� ����
		}

		if ( TransRecordData.smCharInfo.Level<Level ) {
			//���߿� ����� ������ ���� �������� �۴�
			TransRecordData.code=0;
			return -2;
		}

		//��� ����Ÿ ������ �̻� ���� Ȯ��
		if ( CheckRecordDataItem( &TransRecordData )==FALSE ) {
			//������ ����Ÿ �̻� �߰�

			//�ֱ� ���� ���� ���
			fp = fopen( "DataServer\\LastError.dat" , "wb" );
			if ( fp ) {
				cnt = fwrite( &TransRecordData , TransRecordData.size , 1, fp );
				fclose( fp );
			}

			TransRecordData.code=0;
			return -5;
		}

#ifndef _SERVER_MODE_OLD			//������ ȣȯ��

		if ( lpPlayInfo->smCharInfo.Level>0 && 
			TransRecordData.smCharInfo.Level>=6 && 
			abs( lpPlayInfo->smCharInfo.Level-TransRecordData.smCharInfo.Level )>=2 &&
			lpPlayInfo->AdminMode<3 ) {
				//���� 2�ܰ� �̻� �޻��
				return -12;
			}

		INT64	exp,exp2;
		int		money;

		//���� ����ġ�� �������� ����� ��ġ�� ������� Ȯ��
		exp = GetExp64( &TransRecordData.smCharInfo );
		if ( exp<0 ) return -6;			//����ġ�� 0 ���� �۴� (�����߻�)
		
		exp = exp - lpPlayInfo->spExp_Start;
		money = TransRecordData.smCharInfo.Money - lpPlayInfo->spMoney_Start;


/*
		if ( TransRecordData.smCharInfo.Level<79 &&	//���� 79�� ����ġ�� ������ �ʰ� ���� ����
			abs( exp-(TransRecordData.GameSaveInfo.TotalExp^TransRecordData.GameSaveInfo.PosX) )>(Permit_CheckExp*TransRecordData.smCharInfo.Level) ) {

			TransRecordData.code=0;
			return -6;
		}

		if ( abs( money-(TransRecordData.GameSaveInfo.TotalMoney^TransRecordData.GameSaveInfo.PosZ) )>Permit_CheckMoney ) {
			TransRecordData.code=0;
			return -7;
		}
*/

		if ( ReformCharStatePoint( &TransRecordData.smCharInfo , TransRecordData.GameSaveInfo.dwLevelQuestLog )==FALSE ) {
			//ĳ���� ���� ���� �߻�
			TransRecordData.code=0;
			return -8;
		}

		exp2 = 50000;
		if ( TransRecordData.smCharInfo.Level>=20 && TransRecordData.smCharInfo.Level<40 ) exp2 = 200000;
		else if ( TransRecordData.smCharInfo.Level>=40 && TransRecordData.smCharInfo.Level<60 ) exp2 = 400000;
		else if ( TransRecordData.smCharInfo.Level>=60 && TransRecordData.smCharInfo.Level<100 ) exp2 = 800000;
		else if ( TransRecordData.smCharInfo.Level>=100 && TransRecordData.smCharInfo.Level<CHAR_LEVEL_MAX ) exp2 = 1500000;

		if ( exp>exp2 ) {	//����ġ ȹ�淮 ū��츸 �ش�
			exp2 = exp-TransRecordData.GameSaveInfo.TotalSubExp;		//���� �������� ����ġ
			//�������� ���� ����ġ ���� ���嶧�� ���� ����ġ�� �� Ŭ��� ( 2�� ���� ū��� ) / �� 50%�� �����Ѵ�
			if ( (exp2*2)>exp ) {
				TransRecordData.code=0;
				return -9;
			}
		}

		INT64 tExp,sExp;
		///////////////////////////// ���Ӽ��� ����ġ Ȯ�� ////////////////////
		tExp = lpPlayInfo->dwGameServerExp[0] + lpPlayInfo->dwGameServerExp[1] + lpPlayInfo->dwGameServerExp[2] + lpPlayInfo->dwGameServerExp[3] +
				lpPlayInfo->dwGameServerExp[4] + lpPlayInfo->dwGameServerExp[5] + lpPlayInfo->dwGameServerExp[6] + lpPlayInfo->dwGameServerExp[7];
		//���� ������ ���� ���� ����ġ
		exp = GetExp64( &TransRecordData.smCharInfo )-lpPlayInfo->spExp_Start;
		
		if ( exp>tExp ) 
		{
			sExp = exp-tExp;			//���� ȹ�� ����ġ

			exp2 = CheckPlayExpTable[TransRecordData.smCharInfo.Level/10];

			if ( rsServerConfig.Event_ExpUp )
				exp2 = (exp*rsServerConfig.Event_ExpUp)/100;		//����ġ�� �̺�Ʈ�϶�

#ifdef _LANGUAGE_ARGENTINA		//�Ƹ���Ƽ�� �ؿ� KYle
			if ( rsServerConfig.Event_ExpUp_latin )
				exp2 = (exp*rsServerConfig.Event_ExpUp_latin)/100;		//����ġ�� �̺�Ʈ�϶�
#endif
			if ( rsServerConfig.ExpFixMode>0 )
			{
				exp2 = (exp2*rsServerConfig.ExpFixMode)/100;
			}
			else
			{
				exp2 /= 2;		//50 %
			}


			if ( sExp>exp2 && sExp>(tExp/2) ) 
			{

				//���Ӽ������� �� ����ġ�� Ŭ���̾�Ʈ ����ġ�� ���� ���� ŭ
				//����ġ ���� ����
				if ( (lpPlayInfo->dwLastSaveTime+10*1000)<dwPlayServTime )
					lpPlayInfo->RecordWarningCount ++;

				lpPlayInfo->RecordWarningExp = TransRecordData.smCharInfo.Exp; 

				if ( rsServerConfig.ExpFixMode ) 
				{
					SetExp64( &TransRecordData.smCharInfo , lpPlayInfo->spExp_Start+tExp+(tExp/10) );
					//TransRecordData.smCharInfo.Exp = (int)(lpPlayInfo->spExp_Start+tExp+(tExp/10));
				}
				//return -13;
			}
			else 
			{
				lpPlayInfo->RecordWarningCount = 0;
				lpPlayInfo->RecordWarningExp = 0; 
			}
		}
		else 
		{
			lpPlayInfo->RecordWarningCount = 0;
			lpPlayInfo->RecordWarningExp = 0; 
		}
		///////////////////////////////////////////////////////////////////////////

		//��ų ýũ
		if ( CheckSkillPoint( TransRecordData.smCharInfo.Level , &TransRecordData.GameSaveInfo.SkillInfo , 0 , TransRecordData.GameSaveInfo.dwLevelQuestLog )==FALSE ) {
			//��ų����Ʈ ���� ����
			return	-10;
		}
		else {
			if ( rsServerConfig.Disable_DecSkillPoint ) {
				//��ų ����Ʈ ���� �Ұ� 
				//( �߱��� - ��ų����Ʈ ���ȴ� �÷ȴ� �ϴ� ��ŷ - �ѹ� �ø��� ������ ���������� ���� )
				for( cnt=0;cnt<SKILL_POINT_COLUM_MAX;cnt++ ) {
					if ( TransRecordData.GameSaveInfo.SkillInfo.bSkillPoint[cnt]<lpPlayInfo->bSkillPoint[cnt] ) {
						//��ų ����Ʈ�� �پ��� ( ��ų����Ʈ ���� �߻� ��Ŵ )
						return	-10;
					}
				}
			}
		}

		//��ų ����Ʈ ���ۿ� ����
		memcpy( lpPlayInfo->bSkillPoint , TransRecordData.GameSaveInfo.SkillInfo.bSkillPoint , SKILL_POINT_COLUM_MAX );

		if ( TransRecordData.smCharInfo.Weight[0]<0 || 
			TransRecordData.smCharInfo.Weight[0]>(TransRecordData.smCharInfo.Weight[1]+1500) ) {
			
				if ( TransRecordData.smCharInfo.Weight[1]!=0 ) {
					//���� ����
					TransRecordData.code=0;
					return -11;
				}
		}

		// ���� �ڵ� ���� ( ������ �����߿� �ٲ������ ���� )
		if ( lpPlayInfo->smCharInfo.JOB_CODE==0 ) 
			lpPlayInfo->smCharInfo.JOB_CODE = TransRecordData.smCharInfo.JOB_CODE;

		if ( lpPlayInfo->AdminMode<2 && lpPlayInfo->smCharInfo.JOB_CODE!=TransRecordData.smCharInfo.JOB_CODE ) {
			TransRecordData.code=0;
			return -14;
		}


#endif

	//�ʿ����� ���� ������
	TransRecordData.smCharInfo.sPresentItem[0] = lpPlayInfo->sLowLevel_PresentItem[0];
	TransRecordData.smCharInfo.sPresentItem[1] = lpPlayInfo->sLowLevel_PresentItem[1];


	if ( rsServerConfig.Event_StarPointTicket ) 
		CloseStarPointEvent( lpPlayInfo , &TransRecordData.smCharInfo );		//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����


		GetUserDataFile( lpPlayInfo->szName , szFile );
		GetUserDataBackupFile( lpPlayInfo->szName , szBackupFile );
/*
		//����� ��ŷ�� ������ ����ġ �鸸 ��Ƽ� ����
		if ( TransRecordData.smCharInfo.Defence>=500 || TransRecordData.smCharInfo.Absorption>=20 || TransRecordData.smCharInfo.Attack_Damage[1]>=80 || CrackUser ) {
			cnt = 200000;
			if ( TransRecordData.smCharInfo.Exp>cnt ) {
				TransRecordData.smCharInfo.Exp -= cnt;
			}
			else {
				TransRecordData.smCharInfo.Exp = 0;
			}
		}
*/
		if ( FieldLimitLevel_Table[TransRecordData.GameSaveInfo.PlayStageNum]>TransRecordData.smCharInfo.Level ) {
			if ( !lpPlayInfo->AdminMode && FieldLimitLevel_Table[TransRecordData.GameSaveInfo.PlayStageNum]!=1000 ) {
				//�ʵ� ���� ���� ��� ���
				TransRecordData.smCharInfo.Life[0] = 0;
				lpPlayInfo->dwHopeDisconnectTime = dwPlayServTime+5000;		//5���� ���� ����

				//���� ���� ħ��
				smTransCommand.WParam = 1840;
				smTransCommand.SParam = lpPlayInfo->smCharInfo.Level;
				smTransCommand.LParam = TransRecordData.GameSaveInfo.PlayStageNum;
				RecordHackLogFile( lpPlayInfo , &smTransCommand );
			}
		}

		if ( TransRecordData.smCharInfo.Life[0]==0 ) {
			//�����鼭 ����� ��� ���� ��ǥ ������ ���� �� ����
			TransRecordData.smCharInfo.Life[0] = TransRecordData.smCharInfo.Life[1]/2;

			if (TransRecordData.smCharInfo.JOB_CODE <= 4 || TransRecordData.smCharInfo.JOB_CODE == 9)
			{
				//�۽�ũ��
				TransRecordData.GameSaveInfo.PlayStageNum = START_FIELD_NUM;
				TransRecordData.GameSaveInfo.PosX = 746752;
				TransRecordData.GameSaveInfo.PosZ = -4464384;
			}
			else
			{
				TransRecordData.GameSaveInfo.PlayStageNum = START_FIELD_MORYON;
				TransRecordData.GameSaveInfo.PosX = 505344;
				TransRecordData.GameSaveInfo.PosZ = 18948864;
			}
		}
		//������ ����� ����ID ����
		lstrcpy( TransRecordData.GameSaveInfo.szMasterID , lpPlayInfo->szID );

		//����DB�� ����Ÿ ����䱸
		//if ( rsSaveRecData( &TransRecordData , 0,  szFile , szBackupFile )==FALSE ) {
			//������ ���� ����
			size = TransRecordData.size;
			if ( size<srRECORD_DEFAULT_SIZE ) size=srRECORD_DEFAULT_SIZE;

			if ( lpRecordMemBuff && size<=srRECORD_MEMORY_SIZE ) {
				memcpy( lpRecordMemBuff , &TransRecordData , TransRecordData.size );	//�޸� ������ ���� ���
			}
			else {
				CopyVal = CopyFile( szFile, szBackupFile , FALSE );		//���Ϲ��

				lpBuff = lpPlayInfo->lpRecordDataBuff;
				lpPlayInfo->lpRecordDataBuff = (char *)&TransRecordData;

				rsSaveThrowData( lpPlayInfo );
				rsRecordMemoryBuff_CheckInvenItem( lpPlayInfo );

				fp = fopen( szFile , "wb" );
				if ( fp ) {
					cnt = fwrite( &TransRecordData , size , 1, fp );
					fclose( fp );
				}

				lpPlayInfo->lpRecordDataBuff = lpBuff;

				if ( lpRecordMemBuff )
					((TRANS_RECORD_DATA *)lpRecordMemBuff)->size = 0;

			}

		TransRecordData.code=0;
		return TRUE;
	}

	TransLastPartCount = lpTransRecord->Count;

	return FALSE;
}

//�޸� ���۸� ���Ϸ� ����
int rsRecordMemoryBuffToFile( rsPLAYINFO *lpPlayInfo , char *szName ,  char *lpRecordMemBuff )
{
	char szFile[256];
	char szBackupFile[256];
	FILE	*fp;
	int		size;

	if ( !szName || !szName[0] ) return FALSE;
	if ( lpPlayInfo->szServerID[0] ) return TRUE;


	if ( lpRecordMemBuff && ((TRANS_RECORD_DATA *)lpRecordMemBuff)->size>0 ) {

		GetUserDataFile( szName , szFile );
		GetUserDataBackupFile( szName , szBackupFile );

		CopyFile( szFile, szBackupFile , FALSE );		//���Ϲ��

		size = ((TRANS_RECORD_DATA *)lpRecordMemBuff)->size;
		if ( size<srRECORD_DEFAULT_SIZE ) size=srRECORD_DEFAULT_SIZE;

		if ( lpPlayInfo ) {
			//���� ���� ����
			rsSaveServerPotion( lpPlayInfo , &((TRANS_RECORD_DATA *)lpRecordMemBuff)->GameSaveInfo );
			//�������� ��� ����
			rsSaveServerForce( lpPlayInfo , &((TRANS_RECORD_DATA *)lpRecordMemBuff)->GameSaveInfo );
		}



		fp = fopen( szFile , "wb" );
		if ( fp ) {
			fwrite( lpRecordMemBuff , size , 1, fp );
			fclose( fp );
			//((TRANS_RECORD_DATA *)lpRecordMemBuff)->size = 0;
			return TRUE;
		}
	}

	return FALSE;
}


//������ ������ �޸𸮹��� ����Ÿ�� ���� 
int rsSaveThrowData( rsPLAYINFO *lpPlayInfo )
{
	TRANS_RECORD_DATA	*lpTransRecordData;
	sTHROW_ITEM_INFO *lpThrowItemList;
	int cnt;

	if ( !lpPlayInfo->lpRecordDataBuff ) return FALSE;


	lpThrowItemList = lpPlayInfo->ThrowItemInfo;
	lpTransRecordData = (TRANS_RECORD_DATA *)lpPlayInfo->lpRecordDataBuff;
	
	cnt = lpPlayInfo->ThrowItemCount;
	if ( cnt>THROW_ITEM_INFO_MAX )  cnt = THROW_ITEM_INFO_MAX;

	memcpy( &lpTransRecordData->ThrowItemInfo , lpThrowItemList , sizeof(sTHROW_ITEM_INFO)*cnt );
	lpTransRecordData->ThrowItemCount = cnt;
/*
	memcpy( &lpTransRecordData->ThrowItemInfo , lpThrowItemList , sizeof(sTHROW_ITEM_INFO)*lpPlayInfo->ThrowItemCount );
	lpTransRecordData->ThrowItemCount = lpPlayInfo->ThrowItemCount;
*/
	//������ ����� ( ���� ����Ȱ����� ������츸 ���� - ������� )
	if ( lpPlayInfo->UnsaveMoney>=0 && lpTransRecordData->smCharInfo.Money>lpPlayInfo->UnsaveMoney ) 
		lpTransRecordData->GameSaveInfo.LastMoeny = lpPlayInfo->UnsaveMoney+1;			//�� ���

	lpPlayInfo->ThrowItemCount = 0;
	lpPlayInfo->UnsaveMoney = -1;

	return TRUE;
}


//������ �޸𸮹����� �������� �ùٸ��� ���� Ȯ��
int rsRecordMemoryBuff_CheckInvenItem( rsPLAYINFO *lpPlayInfo , int Mode )
{

	BYTE	*lpRecItem;
	TRANS_RECORD_DATA	*lpTransRecordData;
	sRECORD_ITEM	sRecordItem;
	int cnt,cnt2,cnt3;
	INT64 money;
	smTRANS_COMMAND_EX	smTransCommand;
	int	size,BuffSize;
	int	flag;

	sTHROW_ITEM_INFO	ThrowItemInfo[THROW_ITEM_INFO_MAX];			//������ ������ ����
	int				ThrowItemCount =0;											//������ ������ ī����


	if ( !lpPlayInfo->lpRecordDataBuff )	return FALSE;

//#ifdef _LANGUAGE_KOREAN
	if ( lpPlayInfo->AdminMode>=3 ) return FALSE;
//#endif

	lpTransRecordData = (TRANS_RECORD_DATA *)lpPlayInfo->lpRecordDataBuff;

	lpRecItem = (BYTE *)&lpTransRecordData->Data;
	BuffSize = 0;


	for( cnt=0;cnt<lpTransRecordData->ItemCount;cnt++ ) {
		if ( cnt>=(INVENTORY_MAXITEM*2) ) break;

		//���� ����Ÿ �ص� ( Z/NZ ��� )
		DecodeCompress( (BYTE *)lpRecItem , (BYTE *)&sRecordItem , RECORD_ITEM_INFO_HEAD_SIZE );


		if ( sRecordItem.sItemInfo.CODE && 
			sRecordItem.sItemInfo.ItemHeader.Head && sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

			flag = 0;

			if ( (sRecordItem.sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
				//������ [ ���� ]
				if ( lpPlayInfo->TradePotionInfoCount>0 ) {
					//�ŷ� ���� ���� (�ŷ����� ������ ������� ���� ��� , �ŷ��ߴ� ����� ���� ���� ��� ���� )
					if ( rsGetTradePotionInfo( lpPlayInfo , sRecordItem.sItemInfo.CODE )==TRUE ) {
						flag = 0;
					}
				}
				else {
					flag=-1;
				}
			}
			else {
				//�Ϲ� �������� ���

				//�κ��丮 �˻�
				for( cnt2=0;cnt2<INVEN_ITEM_INFO_MAX;cnt2++ ) {
					if ( lpPlayInfo->InvenItemInfo[cnt2].dwCode &&
						lpPlayInfo->InvenItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
						lpPlayInfo->InvenItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
						lpPlayInfo->InvenItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
							lpPlayInfo->InvenItemInfo[cnt2].dwCode = 0;
							flag++;
						}
				}

				//â�� �˻�
				if ( lpPlayInfo->OpenWarehouseInfoFlag ) {
					for( cnt2=0;cnt2<100;cnt2++ ) {
						if ( lpPlayInfo->WareHouseItemInfo[cnt2].dwCode &&
							lpPlayInfo->WareHouseItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
							lpPlayInfo->WareHouseItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
							lpPlayInfo->WareHouseItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

								lpPlayInfo->WareHouseItemInfo[cnt2].dwCode = 0;
								flag++;
							}
					}
				}
			}

			if ( flag==0 ) {
				//�������� ���� ���� ���� ( ���� ��Ŵ )
				for( cnt2=0;cnt2<lpTransRecordData->ThrowItemCount;cnt2++ ) {
					if ( lpTransRecordData->ThrowItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
						lpTransRecordData->ThrowItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
						lpTransRecordData->ThrowItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
							break;
						}
				}

				if ( ThrowItemCount<THROW_ITEM_INFO_MAX ) {

					for(cnt3=0;cnt3<ThrowItemCount;cnt3++) {
						if ( ThrowItemInfo[cnt3].dwCode==sRecordItem.sItemInfo.CODE &&
							ThrowItemInfo[cnt3].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
							ThrowItemInfo[cnt3].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {
								break;
							}
					}
					if ( cnt3>=ThrowItemCount ) {
						ThrowItemInfo[ThrowItemCount].dwCode	= sRecordItem.sItemInfo.CODE;
						ThrowItemInfo[ThrowItemCount].dwKey		= sRecordItem.sItemInfo.ItemHeader.Head;
						ThrowItemInfo[ThrowItemCount].dwSum		= sRecordItem.sItemInfo.ItemHeader.dwChkSum;
						ThrowItemCount++;
					}

				}

				if ( cnt2>=lpTransRecordData->ThrowItemCount ) {
					//�α׿� ���
					smTransCommand.WParam = 8000;
					smTransCommand.WxParam = 1;
					smTransCommand.LxParam = (int)"*RECORD ITEM";
					smTransCommand.LParam = sRecordItem.sItemInfo.CODE;
					smTransCommand.SParam = sRecordItem.sItemInfo.ItemHeader.Head;
					smTransCommand.EParam = sRecordItem.sItemInfo.ItemHeader.dwChkSum;
					RecordHackLogFile( lpPlayInfo , &smTransCommand );
				}
			}
			else {
				if ( flag>0 ) {
					//�������� ���� ������ ������ �ڵ忡 �ִ��� ��Ȯ��( ���� ��Ŵ )
					for( cnt2=0;cnt2<lpTransRecordData->ThrowItemCount;cnt2++ ) {
						if ( lpTransRecordData->ThrowItemInfo[cnt2].dwCode==sRecordItem.sItemInfo.CODE &&
							lpTransRecordData->ThrowItemInfo[cnt2].dwKey==sRecordItem.sItemInfo.ItemHeader.Head &&
							lpTransRecordData->ThrowItemInfo[cnt2].dwSum==sRecordItem.sItemInfo.ItemHeader.dwChkSum ) {

								if ( ThrowItemCount<THROW_ITEM_INFO_MAX ) {
									ThrowItemInfo[ThrowItemCount].dwCode	= sRecordItem.sItemInfo.CODE;
									ThrowItemInfo[ThrowItemCount].dwKey		= sRecordItem.sItemInfo.ItemHeader.Head;
									ThrowItemInfo[ThrowItemCount].dwSum		= sRecordItem.sItemInfo.ItemHeader.dwChkSum;
									ThrowItemCount++;
								}
								break;
							}
					}
				}
			}

			if ( flag>1 ) {
				//��������� �߰� ó��

				//�α׿� ���
				smTransCommand.WParam = 8000;
				smTransCommand.WxParam = flag;
				smTransCommand.LxParam = (int)"*RECORD COPIED ITEM";
				smTransCommand.LParam = sRecordItem.sItemInfo.CODE;
				smTransCommand.SParam = sRecordItem.sItemInfo.ItemHeader.Head;
				smTransCommand.EParam = sRecordItem.sItemInfo.ItemHeader.dwChkSum;
				RecordHackLogFile( lpPlayInfo , &smTransCommand );
			}
		}

		size = ((int *)lpRecItem)[0];
		BuffSize += size;
		lpRecItem += size;
		if ( BuffSize>=(sizeof(sRECORD_ITEM)*RECORD_ITEM_MAX) ) break;			//���� ũ�� �ʰ���
	}
	if ( ThrowItemCount>0 ) {
		memcpy( lpTransRecordData->ThrowItemInfo , ThrowItemInfo, sizeof(sTHROW_ITEM_INFO)*ThrowItemCount );
		lpTransRecordData->ThrowItemCount = ThrowItemCount;
	}

	//â�� ������ ������ â�� �˻�

	sWAREHOUSE WareHouseCheck;
	TRANS_WAREHOUSE	TransWareHouse;
	int	WareHouseFixFlag = 0;
	char szFileName[128];
	char szItemName[64];
	FILE *fp;


	if ( lpPlayInfo->OpenWarehouseInfoFlag && (lpPlayInfo->dwDataError&rsDATA_ERROR_WAREHOUSE)==0 ) {
		GetWareHouseFile( lpPlayInfo->szID , szFileName );

		fp = fopen( szFileName , "rb" );
		if ( fp ) {
			fread( &TransWareHouse , sizeof(TRANS_WAREHOUSE), 1 , fp );
			fclose(fp);
		}
		else {
			lpPlayInfo->OpenWarehouseInfoFlag = 0;
		}
	}


skip_Warehouse:

	if ( lpPlayInfo->OpenWarehouseInfoFlag ) {

		//â�� ���� Ǯ� ������ �˻��Ͽ� ����
		DecodeCompress( (BYTE *)TransWareHouse.Data , (BYTE *)&WareHouseCheck , sizeof(sWAREHOUSE) );

			//������ â�� Ȯ��
			DWORD	dwChkSum = 0;
			char	*szComp = (char *)&WareHouseCheck;

			for( cnt=0;cnt<sizeof(sWAREHOUSE);cnt++ ) {
				dwChkSum += szComp[cnt]*(cnt+1);
			}
			if ( dwChkSum!=TransWareHouse.dwChkSum ) {
				lpPlayInfo->OpenWarehouseInfoFlag = 0;
				goto skip_Warehouse;
			}



		for(cnt=0;cnt<100;cnt++) {
			if ( WareHouseCheck.WareHouseItem[cnt].Flag ) {

				flag = 0;
				for( cnt2=0;cnt2<INVEN_ITEM_INFO_MAX;cnt2++ ) {
					if ( lpPlayInfo->InvenItemInfo[cnt2].dwCode &&
						lpPlayInfo->InvenItemInfo[cnt2].dwCode==WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE &&
						lpPlayInfo->InvenItemInfo[cnt2].dwKey==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head &&
						lpPlayInfo->InvenItemInfo[cnt2].dwSum==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {
							lpPlayInfo->InvenItemInfo[cnt2].dwCode = 0;
							flag++;
						}
				}

				for( cnt2=0;cnt2<100;cnt2++ ) {
					if ( lpPlayInfo->WareHouseItemInfo[cnt2].dwCode &&
						lpPlayInfo->WareHouseItemInfo[cnt2].dwCode==WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE &&
						lpPlayInfo->WareHouseItemInfo[cnt2].dwKey==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head &&
						lpPlayInfo->WareHouseItemInfo[cnt2].dwSum==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {

						lpPlayInfo->WareHouseItemInfo[cnt2].dwCode = 0;
						flag++;
					}
				}

				if ( !flag ) {
					WareHouseCheck.WareHouseItem[cnt].Flag = 0;
					WareHouseFixFlag ++;


					//����Ÿ ������ �� �ֱ� ������ ���ۿ� �̵��� �α� ���
					memcpy( szItemName , WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemName , 32 );
					szItemName[31] = 0;

					//�α׿� ���
					smTransCommand.WParam = 8000;
					smTransCommand.WxParam = 3;
					smTransCommand.LxParam = (int)szItemName;
					smTransCommand.LParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE;
					smTransCommand.SParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
					smTransCommand.EParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;

					RecordHackLogFile( lpPlayInfo , &smTransCommand );
				}

				if ( flag>1 ) {
					//�α׿� ���
					smTransCommand.WParam = 8000;
					smTransCommand.WxParam = flag;
					smTransCommand.LxParam = (int)"*RECORD COPIED ITEM IN WAREHOUSE";
					smTransCommand.LParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE;
					smTransCommand.SParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
					smTransCommand.EParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;
					RecordHackLogFile( lpPlayInfo , &smTransCommand );
				}
			}
		}

		if ( WareHouseCheck.Money ) {
			lpPlayInfo->WareHouseMoney = (WareHouseCheck.Money-2023);
			money = lpTransRecordData->smCharInfo.Money;
			money += lpPlayInfo->WareHouseMoney;

			if( lpTransRecordData->smCharInfo.Money<0 ) {
				//ĳ�� �ָӴ��� ���� - �ΰ�� â���� ������ ����
				lpTransRecordData->smCharInfo.Money = 0;
				money = lpPlayInfo->WareHouseMoney;
			}
		}
		else {
			lpPlayInfo->WareHouseMoney = 0;
			money = lpTransRecordData->smCharInfo.Money;
		}



		if ( money>lpPlayInfo->ServerMoney ) {
			//�α׿� ���
			smTransCommand.WParam = 8010;
			smTransCommand.LParam = 2;
			smTransCommand.SParam = lpPlayInfo->ServerMoney;
			smTransCommand.EParam = (int)money;
			RecordHackLogFile( lpPlayInfo , &smTransCommand );

			cnt2 = (int)(money-lpPlayInfo->ServerMoney);
			lpTransRecordData->smCharInfo.Money -= cnt2;
			lpPlayInfo->UnsaveMoney = -1;
			lpTransRecordData->GameSaveInfo.LastMoeny = -1;

			if ( lpTransRecordData->smCharInfo.Money<0 ) {
				WareHouseCheck.Money += lpTransRecordData->smCharInfo.Money;
				if ( WareHouseCheck.Money<2023 ) WareHouseCheck.Money = 2023;
				WareHouseFixFlag ++;

				lpTransRecordData->smCharInfo.Money=0;
			}

		}

		if ( WareHouseFixFlag && !Mode ) {

			//â�� ����
			if ( SaveWareHouse( &WareHouseCheck , &TransWareHouse )==TRUE ) {
				//â�� ����Ÿ ����
				rsSaveWareHouseData( lpPlayInfo->szID , &TransWareHouse );
			}
		}
	}
	else {
	//if ( WareHouseCheck.Money ) 
	//			lpPlayInfo->ServerMoney += WareHouseCheck.Money-2023;

		//�� ��ġ �˻��Ͽ� ����
		if ( lpTransRecordData->smCharInfo.Money>lpPlayInfo->ServerMoney ) {

			//�α׿� ���
			smTransCommand.WParam = 8010;
			smTransCommand.LParam = 1;
			smTransCommand.SParam = lpPlayInfo->ServerMoney;
			smTransCommand.EParam = lpTransRecordData->smCharInfo.Money;
			RecordHackLogFile( lpPlayInfo , &smTransCommand );

			lpTransRecordData->smCharInfo.Money = lpPlayInfo->ServerMoney;
			lpPlayInfo->UnsaveMoney = -1;
			if ( lpTransRecordData->smCharInfo.Money<0 ) lpTransRecordData->smCharInfo.Money=0;
		}

		money = lpTransRecordData->GameSaveInfo.LastMoeny;

		if ( money &&  (money-1)>lpPlayInfo->ServerMoney ) {

			//�α׿� ���
			smTransCommand.WParam = 8010;
			smTransCommand.LParam = 3;
			smTransCommand.SParam = lpPlayInfo->ServerMoney;
			smTransCommand.EParam = lpTransRecordData->GameSaveInfo.LastMoeny-1;
			RecordHackLogFile( lpPlayInfo , &smTransCommand );

			lpTransRecordData->GameSaveInfo.LastMoeny = lpPlayInfo->ServerMoney+1;
			if ( lpTransRecordData->GameSaveInfo.LastMoeny<0 ) lpTransRecordData->GameSaveInfo.LastMoeny=0;
		}

	}



	return TRUE;
}


//ĳ���� ���� ��� ���Ϸ� ����
int RecordDeleteCharacterError( char *szID , char *szName );

//�ش� ID�� �ش��ϴ� ĳ���� ����Ÿ�� Ŭ���̾�Ʈ�� ������
int rsRECORD_DBASE::SendUserDataToClient( char *szID , smWINSOCK *lpsmSock , char *szServerID )
{
	char	szFile[256];
	char	szFile2[256];
	char	szFileInfo[256];
	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;
	sPLAY_USER_DATA		sPlayUserData;
	TRANS_USERCHAR_INFO	TransUserCharInfo;
	_TRANS_CHAR_INFO	*lpCharInfo;
	FILE	*fp;
	int		cnt;
	int		FindCnt;
	int		DeleteCnt;
	int		LevelMax = 0;
	int		CharNameMax = CHAR_NAME_MAXLEN;

	//���Ϻҷ�����
	//wsprintf( szFileInfo , "userInfo\\%s.dat" , szID );

	if ( rsServerConfig.TT_DataServer_Count )
		CharNameMax = CHAR_NAME_MAXLEN+6;

	GetUserInfoFile( szID , szFileInfo );

	lstrcpy( LastAcessID , szID );

 	hFind = FindFirstFile( szFileInfo , &ffd );
	FindClose( hFind );

	if ( hFind==INVALID_HANDLE_VALUE ) {

		//CreateDirectory( szRecordUserInfoDir , NULL );			//���丮 ����

		//������ ������ �ű� ����
		ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );
		lstrcpy( sPlayUserData.szID , szID );
		lstrcpy( sPlayUserData.szHeader , "PS_TAILID 1.10" );

		fp = fopen( szFileInfo , "wb" );
		if ( !fp ) return FALSE;
		fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
		fclose(fp);
	}
	else {
		ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );

		fp = fopen( szFileInfo , "rb" );
		if ( !fp ) return FALSE;
		fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
		fclose(fp);

		if ( lstrcmpi( sPlayUserData.szID , szID )!=0 ) {
			//������ ������ �ű� ����
			ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );
			lstrcpy( sPlayUserData.szID , szID );
			lstrcpy( sPlayUserData.szHeader , "PS_TAILID 1.10" );

			fp = fopen( szFileInfo , "wb" );
			if ( !fp ) return FALSE;
			fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
			fclose(fp);
		}
	}

	TransUserCharInfo.code = smTRANSCODE_ID_SETUSERINFO;
	TransUserCharInfo.size = sizeof(TRANS_USERCHAR_INFO);
	lstrcpy( TransUserCharInfo.szID , szID );

	FindCnt = 0;
	DeleteCnt = 0;

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( sPlayUserData.szCharName[cnt][0] ) {
			//ĳ���� ���Ͽ��� ����Ÿ �Լ� 

			//wsprintf( szFile , "userdata\\%s.dat" , sPlayUserData.szCharName[cnt] );
			GetUserDataFile( sPlayUserData.szCharName[cnt] , szFile );

			ZeroMemory( &TransRecordData , sizeof(TRANS_RECORD_DATA) );

			fp = fopen( szFile , "rb" );
			if ( fp ) {
				fread( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1 , fp );
				fclose(fp);

				if ( lstrcmpi( TransRecordData.smCharInfo.szName , sPlayUserData.szCharName[cnt] )==0 &&
					lstrlen(sPlayUserData.szCharName[cnt])<CharNameMax ) {	//CHAR_NAME_MAXLEN  ) {

					lpCharInfo = (_TRANS_CHAR_INFO *)&TransUserCharInfo.CharInfo[FindCnt];

					lstrcpy( lpCharInfo->szName , TransRecordData.smCharInfo.szName );
					lstrcpy( lpCharInfo->szModelName , TransRecordData.smCharInfo.szModelName );
					lstrcpy( lpCharInfo->szModelName2 , TransRecordData.smCharInfo.szModelName2 );

					lpCharInfo->Brood = TransRecordData.smCharInfo.Brood;
					lpCharInfo->dwArmorCode = 0;
					//lpCharInfo->JOB_CODE = TransRecordData.smCharInfo.JOB_CODE |(TransRecordData.smCharInfo.ChangeJob<<24);
					lpCharInfo->JOB_CODE = TransRecordData.smCharInfo.JOB_CODE;
					lpCharInfo->Level = TransRecordData.smCharInfo.Level;

					if ( rsServerConfig.FixedStartField )
						lpCharInfo->StartField =rsServerConfig.FixedStartField;				//�����ʵ� ���� ����
					else
						lpCharInfo->StartField =TransRecordData.GameSaveInfo.PlayStageNum;

					if ( lpCharInfo->StartField==rsSOD_FIELD ) lpCharInfo->StartField = rsSOD_VILLAGE;
					if ( lpCharInfo->StartField==QUEST_ARENA_FIELD ) {
						if (lpCharInfo->JOB_CODE <= 4 || lpCharInfo->JOB_CODE == 9)
							lpCharInfo->StartField = START_FIELD_NUM;
						else
							lpCharInfo->StartField = START_FIELD_MORYON;
					}

					if ( TransRecordData.smCharInfo.wPlayerKilling[0]>0 ) {
						//������ �����ִ� ����
						lpCharInfo->PosX = PrisonX;
						lpCharInfo->PosZ = PrisonZ;
					}
					else {
						lpCharInfo->PosX = TransRecordData.GameSaveInfo.PosX;
						lpCharInfo->PosZ = TransRecordData.GameSaveInfo.PosZ;
					}

					if ( LevelMax<lpCharInfo->Level ) LevelMax=lpCharInfo->Level;

					FindCnt++;
				}
				else {
					if ( TransRecordData.size ) {
						//����ĳ������ ���
						GetDeleteDataFile( sPlayUserData.szCharName[cnt] , szFile2 );
						CopyFile( szFile , szFile2 , FALSE );

						RecordDeleteCharacterError( szID , szFile );
					}

					DeleteFile( szFile );

					//������ �̸� ����
					sPlayUserData.szCharName[cnt][0] = 0;
					DeleteCnt++;
				}
			}
			else {
				//������ �̸� ����
				sPlayUserData.szCharName[cnt][0] = 0;
				DeleteCnt++;
			}
		}
	}

	if ( FindCnt>5 ) FindCnt=5;

	//Ŭ���̾�Ʈ�� ����
	TransUserCharInfo.PlayUserCount = FindCnt;
	lpsmSock->Send2( (char *)&TransUserCharInfo , TransUserCharInfo.size , TRUE );


	//������ ����Ÿ ������ �ٽ� ����
	if ( DeleteCnt ) {
		fp = fopen( szFileInfo , "wb" );
		if ( fp ) {
			fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
			fclose(fp);
		}
	}


	return LevelMax;
}

//ĳ���� ����Ÿ ���� ����
int rsRECORD_DBASE::InsertCharData( char *szID , char *szName , int Mode )
{
	char	szFile[256];
	sPLAY_USER_DATA		sPlayUserData;
	FILE	*fp;
	int		cnt;

	if ( !Mode && IsData( szName )==TRUE ) return FALSE;

	//���Ϻҷ�����
	//wsprintf( szFile , "userInfo\\%s.dat" , szID );
	GetUserInfoFile( szID , szFile );

	fp = fopen( szFile , "rb" );
	if ( !fp ) return FALSE;

	fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
	fclose(fp);

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( !sPlayUserData.szCharName[cnt][0] ) {
			lstrcpy( sPlayUserData.szCharName[cnt] , szName );
			fp = fopen( szFile , "wb" );
			if ( fp ) {
				fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
				fclose(fp);
			}

			////////// ���⿡ �ӽ� ���� ����Ÿ ���� ���� ( ��� �޸� Zero ���� ) //////////////
			if ( !Mode ) {
				GetUserDataFile( sPlayUserData.szCharName[cnt] , szFile );
				ZeroMemory( &TransRecordData , sizeof(TRANS_RECORD_DATA) );

				fp = fopen( szFile , "wb" );
				if ( fp ) {
					fwrite( &TransRecordData , srRECORD_MEMORY_SIZE , 1, fp );
					fclose( fp );
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}


//ĳ���� ����Ÿ ����
int rsRECORD_DBASE::DeleteCharData( char *szID , char *szName )
{
	char szFile[256];
	char szDelFile[256];
	char szDelBackupFile[256];
	FILE	*fp;
	int		cnt;
	sPLAY_USER_DATA		sPlayUserData;

//	wsprintf( szFile , "userdata\\%s.dat" , szName );



	GetUserDataFile( szName , szDelFile );

	fp = fopen( szDelFile , "rb" );
	if ( fp ) {
		fread( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1, fp );
		fclose( fp );
	}

	//���Ϻҷ�����
//	wsprintf( szFile , "userInfo\\%s.dat" , szID );
	GetUserInfoFile( szID , szFile );

	fp = fopen( szFile , "rb" );
	if ( !fp ) return FALSE;

	fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
	fclose(fp);

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( sPlayUserData.szCharName[cnt][0] &&
			lstrcmpi( sPlayUserData.szCharName[cnt] , szName )==0 ) {
			
			sPlayUserData.szCharName[cnt][0] = 0;

			fp = fopen( szFile , "wb" );
			if ( fp ) {
				fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
				fclose(fp);
				if ( TransRecordData.smCharInfo.Level>=10 ) {
					//���� 10�̻��� ĳ���ʹ� ��� ����
					GetDeleteDataFile( szName , szDelBackupFile );
					if ( TransRecordData.smCharInfo.Level>=20 )
						CopyFile( szDelFile , szDelBackupFile , TRUE );
					else
						CopyFile( szDelFile , szDelBackupFile , FALSE );
				}

				DeleteFile( szDelFile );
			}
			return TRUE;
		}
	}

	return TRUE;
}

#include	"checkname.h"		//������ ����

//ĳ���Ͱ� ���� �ϴ��� ������ �˻��Ͽ� Ȯ��
int rsRECORD_DBASE::IsData( char *szName )
{
	char	szFile[256];
	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;

	//���Ϻҷ�����
	//wsprintf( szFile , "userdata\\%s.dat" , szName );

	//CreateDirectory( szRecordUserDataDir , NULL );			//���丮 ����
	//CreateDirectory( szRecordUserBackupDataDir , NULL );			//���丮 ����

	if ( lstrlen(szName)>=CHAR_NAME_MAXLEN  ) return TRUE;

	if(!c_CheckName(".\\CharacterNameList",szName)) return TRUE;    // <--- ��� �߰� �� �ʴϴ�.(������)

	GetUserDataFile( szName , szFile );

 	hFind = FindFirstFile( szFile , &ffd );
	FindClose( hFind );
	if ( hFind!=INVALID_HANDLE_VALUE ) {
		return TRUE;
	}

	return FALSE;
}





//������ ������ ����Ÿ ���� 
int rsRECORD_DBASE::SaveThrowData( char *szName , sTHROW_ITEM_INFO *lpThrowItemList , int Count , int SaveMoney )
{
	char szFile[256];
	FILE	*fp;

	if ( !szName[0] ) return FALSE;

	//wsprintf( szFile , "userdata\\%s.dat" , szName );
	GetUserDataFile( szName , szFile );

	fp = fopen( szFile , "rb" );
	if ( !fp ) return FALSE;

	fread( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1 , fp );
	fclose(fp);

	memcpy( &TransRecordData.ThrowItemInfo , lpThrowItemList , sizeof(sTHROW_ITEM_INFO)*Count );
	TransRecordData.ThrowItemCount = Count;

	//������ ����� ( ���� ����Ȱ����� ������츸 ���� - ������� )
	if ( SaveMoney>=0 && TransRecordData.smCharInfo.Money>SaveMoney ) 
		TransRecordData.GameSaveInfo.LastMoeny = SaveMoney+1;			//�� ���


	fp = fopen( szFile , "wb" );
	if ( !fp ) return FALSE;

	fwrite( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1 , fp );
	fclose(fp);

	return TRUE;
}



/*
	//â�� ����Ÿ ����
	int SaveWareHouseData( TRANS_WAREHOUSE *lpTransWareHouse );
	//â�� ����Ÿ �ε�
	int LoadWareHouseData( smWINSOCK *lpsmSock );
*/

//â�� ����Ÿ ����
int rsSaveWareHouseData( char *szID , TRANS_WAREHOUSE *lpTransWareHouse )
{
	char szFileName[128];
	FILE	*fp;

	GetWareHouseFile( szID , szFileName );

	//CreateDirectory( szRecordWareHouseDir , NULL );			//���丮 ����

	fp = fopen( szFileName , "wb" );
	if ( !fp ) return FALSE;
	fwrite( lpTransWareHouse , lpTransWareHouse->size , 1 , fp );
	fclose(fp);

	return TRUE;
}

//â�� ����Ÿ ����
//int rsLoadWareHouseData( char *szID , smWINSOCK *lpsmSock )
int rsLoadWareHouseData( rsPLAYINFO *lpPlayInfo )
{
	char szFileName[128];
	TRANS_WAREHOUSE TransWareHouse;
	sWAREHOUSE	WareHouseCheck;
	FILE	*fp;
	smTRANS_COMMAND_EX	smTransCommand;
	int	CopiedItemFlag;

	//int cnt;

	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;
	int	Money;
	int cnt,cnt2;


	GetWareHouseFile( lpPlayInfo->szID , szFileName );

 	hFind = FindFirstFile( szFileName , &ffd );
	FindClose( hFind );
	if ( hFind!=INVALID_HANDLE_VALUE ) {
		fp = fopen( szFileName , "rb" );
		if ( fp ) {
			fread( &TransWareHouse , sizeof(TRANS_WAREHOUSE), 1 , fp );
			fclose(fp);
		}
	}
	else {
		TransWareHouse.code = smTRANSCODE_WAREHOUSE;
		TransWareHouse.size = sizeof(TRANS_WAREHOUSE)-sizeof(sWAREHOUSE);
		TransWareHouse.DataSize = 0;
		TransWareHouse.dwChkSum = 0;
		TransWareHouse.wVersion[0] = Version_WareHouse;
		TransWareHouse.wVersion[1] = 0;
		TransWareHouse.WareHouseMoney = 0;
		TransWareHouse.UserMoney = 0;
		TransWareHouse.dwTemp[0] = 0;
		TransWareHouse.dwTemp[1] = 0;
		TransWareHouse.dwTemp[2] = 0;
		TransWareHouse.dwTemp[3] = 0;
		TransWareHouse.dwTemp[4] = 0;

	}
/*
	//â�� ������ ���� ��ȯ ( ������ â�� )
	if ( TransWareHouse.wVersion[0]<Version_WareHouse ) {
		sWAREHOUSE	sWareHouse;
		//â�� ��������
		if ( LoadWareHouse( &TransWareHouse , &sWareHouse , 1 )==FALSE )
			return FALSE;

		//������ ���� ������
		for( cnt=0;cnt<100;cnt++) {
			if ( sWareHouse.WareHouseItem[cnt].Flag ) {
				rsReformItem_Server( &sWareHouse.WareHouseItem[cnt].sItemInfo );
			}
		}

		//â�� ����ü�� ����
		if ( SaveWareHouse( &sWareHouse , &TransWareHouse )==FALSE ) 
			return FALSE;

		//�ٽ� ����
		fp = fopen( szFileName , "wb" );
		if ( fp ) {
			fwrite( &TransWareHouse , TransWareHouse.size , 1 , fp );
			fclose(fp);
		}
	}
*/
	if ( TransWareHouse.size>=smSOCKBUFF_SIZE ) TransWareHouse.size = smSOCKBUFF_SIZE;

	Money = TransWareHouse.WareHouseMoney;			//������ ����� ��

	TransWareHouse.WareHouseMoney = 0;
	TransWareHouse.UserMoney = 0;
	CopiedItemFlag = 0;


	Server_DebugCount = 500;


	if ( !lpPlayInfo->OpenWarehouseInfoFlag ) {

		Server_DebugCount = 510;

		lpPlayInfo->OpenWarehouseInfoFlag = TRUE;
		ZeroMemory( lpPlayInfo->WareHouseItemInfo , sizeof(sTHROW_ITEM_INFO)*100 );

		if ( TransWareHouse.DataSize ) {
			//â�� ���� Ǯ� ������ �˻��Ͽ� ����
			DecodeCompress( (BYTE *)TransWareHouse.Data , (BYTE *)&WareHouseCheck , sizeof(sWAREHOUSE) );

			//������ â�� Ȯ��
			DWORD	dwChkSum = 0;
			char	*szComp = (char *)&WareHouseCheck;

			for( cnt=0;cnt<sizeof(sWAREHOUSE);cnt++ ) {
				dwChkSum += szComp[cnt]*(cnt+1);
			}
			if ( dwChkSum!=TransWareHouse.dwChkSum ) {
				lpPlayInfo->OpenWarehouseInfoFlag = FALSE;
				lpPlayInfo->dwDataError |= rsDATA_ERROR_WAREHOUSE;
				return 0;
			}


			for( cnt=0;cnt<100;cnt++ ) {
				if ( WareHouseCheck.WareHouseItem[cnt].Flag ) {

					for(cnt2=0;cnt2<INVEN_ITEM_INFO_MAX;cnt2++) {
						//��������� �˻�
						if ( lpPlayInfo->InvenItemInfo[cnt2].dwCode &&
							lpPlayInfo->InvenItemInfo[cnt2].dwCode==WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE &&
							lpPlayInfo->InvenItemInfo[cnt2].dwKey==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head &&
							lpPlayInfo->InvenItemInfo[cnt2].dwSum==WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {

								//�α׿� ���
								smTransCommand.WParam = 8070;
								smTransCommand.WxParam = 2;
								smTransCommand.LxParam = (int)"*WAREHOUSE";
								smTransCommand.LParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE;
								smTransCommand.SParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
								smTransCommand.EParam = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;
								RecordHackLogFile( lpPlayInfo , &smTransCommand );
								CopiedItemFlag++;
								break;

							}
					}

					if ( cnt2>=INVEN_ITEM_INFO_MAX ) {
						lpPlayInfo->WareHouseItemInfo[cnt].dwCode = WareHouseCheck.WareHouseItem[cnt].sItemInfo.CODE;
						lpPlayInfo->WareHouseItemInfo[cnt].dwKey = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
						lpPlayInfo->WareHouseItemInfo[cnt].dwSum = WareHouseCheck.WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;
					}
				}
			}

			if ( WareHouseCheck.Money ) 
				lpPlayInfo->AddServerMoney( WareHouseCheck.Money-2023 , WHERE_OPEN_WAREHOUES );
				//lpPlayInfo->ServerMoney += WareHouseCheck.Money-2023;
		}
		Server_DebugCount = 520;
	}

	if ( !CopiedItemFlag ) {
		if ( lpPlayInfo->lpsmSock )
			lpPlayInfo->lpsmSock->Send2( (char *)&TransWareHouse , TransWareHouse.size , TRUE );
	}
	else {
		//��������� ������ �ִ»�� �ڵ����� �ñ�� �Ѵ�
		smTransCommand.code = smTRANSCODE_CLOSECLIENT;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = 0;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		if ( lpPlayInfo->lpsmSock )
			lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
/*
		/////////////////// ����� ���� ��� /////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		fp = fopen( "(debug)warhouse-info.dat" , "wb" );
		if ( fp ) {
			fwrite( lpPlayInfo->WareHouseItemInfo , sizeof(sTHROW_ITEM_INFO)*100 , 1, fp );
			fclose(fp);
		}
		fp = fopen( "(debug)inven-info.dat" , "wb" );
		if ( fp ) {
			fwrite( lpPlayInfo->InvenItemInfo , sizeof(sTHROW_ITEM_INFO)*300 , 1, fp );
			fclose(fp);
		}
		fp = fopen( "(debug)Warehouse-data.dat" , "wb" );
		if ( fp ) {
			fwrite( &TransWareHouse , TransWareHouse.size , 1, fp );
			fclose(fp);
		}
		fp = fopen( "(debug)item-Info.dat" , "wb" );
		if ( fp ) {
			fwrite( lpPlayInfo->lpRecordDataBuff , sizeof(TRANS_RECORD_DATA) , 1, fp );
			fclose(fp);
		}
*/
		//////////////////////////////////////////////////////////////////////////////////////
	}

	Server_DebugCount = 0;

	return Money;
}

//â�� ���� (������)
int rsLoadWareHouseData_Admin( rsPLAYINFO *lpPlayInfo , char *szID , int Day )
{
	char szFileName[128];
	TRANS_WAREHOUSE TransWareHouse;
	FILE	*fp;

	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;


	if ( Day==0 )
		GetWareHouseFile( szID , szFileName );
	else
		GetWareHouseFile_Backup( szID , szFileName , Day );

 	hFind = FindFirstFile( szFileName , &ffd );
	FindClose( hFind );
	if ( hFind!=INVALID_HANDLE_VALUE ) {
		fp = fopen( szFileName , "rb" );
		if ( fp ) {
			fread( &TransWareHouse , sizeof(TRANS_WAREHOUSE), 1 , fp );
			fclose(fp);
		}
	}
	else {
		return FALSE;
	}

	if ( TransWareHouse.size>=smSOCKBUFF_SIZE ) TransWareHouse.size = smSOCKBUFF_SIZE;

	lpPlayInfo->OpenWarehouseInfoFlag = 0;

	if ( lpPlayInfo->lpsmSock )
		lpPlayInfo->lpsmSock->Send2( (char *)&TransWareHouse , TransWareHouse.size , TRUE );

	return TRUE;
}


//�ش� ID�� �ش� ĳ������ �����ϴ��� Ȯ��
int rsCheckAccountChar( char *szID , char *szName )
{
	char	szFileInfo[128];
	sPLAY_USER_DATA		sPlayUserData;
	FILE	*fp;
	int		cnt;

	GetUserInfoFile( szID , szFileInfo );

	ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );

	fp = fopen( szFileInfo , "rb" );
	if ( !fp ) return FALSE;
	fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
	fclose(fp);

	if ( lstrcmpi( sPlayUserData.szID , szID )!=0 ) return FALSE;

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( sPlayUserData.szCharName[cnt][0] ) {
			if ( lstrcmpi( sPlayUserData.szCharName[cnt] , szName )==0 ) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

//�������� ��� ����
int rsLoadServerForce( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int cnt;
	int cnt2; // ����� - �ν��� ������(�����, ���, �ٷ�)

	if ( lpGameSaveInfo->wForceOrbUsing[0] && lpGameSaveInfo->wForceOrbUsing[1] ) {
		// ����� - ���� ���� ���� �߰�
		cnt = lpGameSaveInfo->wForceOrbUsing[0]>>8;
		if(lpGameSaveInfo->wForceOrbUsing[0]>=sin26)
		{
			cnt -= 16;
		}
		cnt --;
		if(cnt>=0 && cnt<12 )  // ����� : ���� ���� �߰��� ���� ���� 12�� ���� // �Ϲ� ����
		{
			if (lpGameSaveInfo->wForceOrbUsing[1]<=ForceOrbUseTime[cnt]) {
				lpPlayInfo->dwForceOrb_SaveCode = sinFO1+lpGameSaveInfo->wForceOrbUsing[0];
				lpPlayInfo->dwForceOrb_SaveTime = dwPlayServTime+lpGameSaveInfo->wForceOrbUsing[1]*1000;
				lpPlayInfo->dwForceOrb_SaveDamage = ForceOrbDamage[cnt];
			}
		}
		else if( cnt>=20 && cnt < 32) // ���� ����
		{
			if (lpGameSaveInfo->wForceOrbUsing[1]<=MagicForceOrbUseTime[cnt-20]) {
				lpPlayInfo->dwForceOrb_SaveCode = sinFO1+lpGameSaveInfo->wForceOrbUsing[0];
				lpPlayInfo->dwForceOrb_SaveTime = dwPlayServTime+lpGameSaveInfo->wForceOrbUsing[1]*1000;
				lpPlayInfo->dwForceOrb_SaveDamage = MagicForceOrbDamage[cnt-20];
			}
		}
		else if( cnt>=34 && cnt < 37) // ���� ���� ����
		{
			if (lpGameSaveInfo->wForceOrbUsing[1]<=BillingMagicForceOrbUseTime[cnt-34]) {
				lpPlayInfo->dwForceOrb_SaveCode = sinFO1+lpGameSaveInfo->wForceOrbUsing[0];
				lpPlayInfo->dwForceOrb_SaveTime = dwPlayServTime+lpGameSaveInfo->wForceOrbUsing[1]*1000;
				lpPlayInfo->dwForceOrb_SaveDamage = BillingMagicForceOrbDamage[cnt-34];
			}
		}	
	}

	// ����� - �ν��� ������(�����) ��� ����
	if ( lpGameSaveInfo->wLifeBoosterUsing[0] && lpGameSaveInfo->wLifeBoosterUsing[1] ) 
	{
		cnt2 = lpGameSaveInfo->wLifeBoosterUsing[0]>>8; // sin21 -> 21 / sin22 -> 22 / sin23 -> 23
		cnt2 -= 21;
		if (lpGameSaveInfo->wLifeBoosterUsing[1]<=BoosterItem_UseTime[cnt2]/60) 
		{ 
			lpPlayInfo->dwLifeBooster_SaveCode = sinBC1+lpGameSaveInfo->wLifeBoosterUsing[0]; // ������ �ν��� �ڵ�
			lpPlayInfo->dwLifeBooster_SaveTime = dwPlayServTime+lpGameSaveInfo->wLifeBoosterUsing[1]*60*1000; // ������ �ν��� ����� ���� �ð� ����
			lpPlayInfo->dwLifeBooster_SaveData = BoosterItem_DataPercent[0]; // �ν��� ������ �����ۼ�Ʈ
		}
	}
	// ����� - �ν��� ������(���) ��� ����
	if ( lpGameSaveInfo->wManaBoosterUsing[0] && lpGameSaveInfo->wManaBoosterUsing[1] ) 
	{
		cnt2 = lpGameSaveInfo->wManaBoosterUsing[0]>>8; // sin24 -> 24 / sin25 -> 25 / sin26 -> 26
		if(lpGameSaveInfo->wManaBoosterUsing[0]>=sin26) // sin26���ʹ�  -16�� ����� �ǹٸ� ������ ���´�.
		{
			cnt2 -= 16;
		}
		cnt2 -= 24;
		if (lpGameSaveInfo->wManaBoosterUsing[1]<=BoosterItem_UseTime[cnt2]/60) 
		{ 
			lpPlayInfo->dwManaBooster_SaveCode = sinBC1+lpGameSaveInfo->wManaBoosterUsing[0]; // ������ �ν��� �ڵ�
			lpPlayInfo->dwManaBooster_SaveTime = dwPlayServTime+lpGameSaveInfo->wManaBoosterUsing[1]*60*1000; // ������ �ν��� ����� ���� �ð� ����
			lpPlayInfo->dwManaBooster_SaveData = BoosterItem_DataPercent[1]; // �ν��� ������ �����ۼ�Ʈ
		}
	}
	// ����� - �ν��� ������(�ٷ�) ��� ����
	if ( lpGameSaveInfo->wStaminaBoosterUsing[0] && lpGameSaveInfo->wStaminaBoosterUsing[1] ) 
	{
		cnt2 = lpGameSaveInfo->wStaminaBoosterUsing[0]>>8; // sin27 -> 27 / sin28 -> 28 / sin29 -> 29
		if(lpGameSaveInfo->wStaminaBoosterUsing[0]>=sin26) // sin26���ʹ�  -16�� ����� �ǹٸ� ������ ���´�.
		{
			cnt2 -= 16;
		}
		cnt2 -= 27;
		if (lpGameSaveInfo->wStaminaBoosterUsing[1]<=BoosterItem_UseTime[cnt2]/60) 
		{ 
			lpPlayInfo->dwStaminaBooster_SaveCode = sinBC1+lpGameSaveInfo->wStaminaBoosterUsing[0]; // ������ �ν��� �ڵ�
			lpPlayInfo->dwStaminaBooster_SaveTime = dwPlayServTime+lpGameSaveInfo->wStaminaBoosterUsing[1]*60*1000; // ������ �ν��� ����� ���� �ð� ����
			lpPlayInfo->dwStaminaBooster_SaveData = BoosterItem_DataPercent[2]; // �ν��� ������ �����ۼ�Ʈ
		}
	}

	// �庰 - ��ų ������
	if ( lpGameSaveInfo->wSkillDelayUsing[0] && lpGameSaveInfo->wSkillDelayUsing[1] ) 
	{
		cnt2 = lpGameSaveInfo->wSkillDelayUsing[0]>>8; // sin27 -> 27 / sin28 -> 28 / sin29 -> 29
		if(lpGameSaveInfo->wSkillDelayUsing[0]>=sin26) // sin26���ʹ�  -16�� ����� �ǹٸ� ������ ���´�.
		{
			cnt2 -= 16;
		}
		cnt2 -= 27;
		if (lpGameSaveInfo->wSkillDelayUsing[1]<=nSkillDelay_UseTime[cnt2-3]/60) 
		{ 
			lpPlayInfo->dwSkillDelay_SaveCode = sinBC1+lpGameSaveInfo->wSkillDelayUsing[0]; 
			lpPlayInfo->dwSkillDelay_SaveTime = dwPlayServTime+lpGameSaveInfo->wSkillDelayUsing[1]*60*1000; 
		//	lpPlayInfo->dwSkillDelay_SaveData = BoosterItem_DataPercent[2]; 
		}
	}
	
	//������ ������ ��ȿ�Ⱓ ����
	if ( lpGameSaveInfo->dwTime_PrimeItem_X2>(DWORD)tServerTime )
		lpPlayInfo->dwTime_PrimeItem_X2 = lpGameSaveInfo->dwTime_PrimeItem_X2;
	else
		lpPlayInfo->dwTime_PrimeItem_X2 = 0;

	if ( lpGameSaveInfo->dwTime_PrimeItem_ExpUp>(DWORD)tServerTime )
		lpPlayInfo->dwTime_PrimeItem_ExpUp = lpGameSaveInfo->dwTime_PrimeItem_ExpUp;
	else
		lpPlayInfo->dwTime_PrimeItem_ExpUp = 0;

	if ( lpPlayInfo->dwTime_PrimeItem_X2 || lpGameSaveInfo->dwTime_PrimeItem_ExpUp ) {
		lpPlayInfo->dwPrimeItem_PackageCode = lpGameSaveInfo->dwPrimeItem_PackageCode;		//��Ű�� ���� ����
	}

	if ( lpGameSaveInfo->dwTime_PrimeItem_VampCuspid>(DWORD)tServerTime )
		lpPlayInfo->dwTime_PrimeItem_VampCuspid = lpGameSaveInfo->dwTime_PrimeItem_VampCuspid;
	else
		lpPlayInfo->dwTime_PrimeItem_VampCuspid = 0;

	// �庰 - ���Ǹ� Ŀ���� EX
	if ( lpGameSaveInfo->dwTime_PrimeItem_VampCuspid_EX>(DWORD)tServerTime )
		lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX = lpGameSaveInfo->dwTime_PrimeItem_VampCuspid_EX;
	else
		lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX = 0;

	if ( lpGameSaveInfo->dwTime_PrimeItem_ManaRecharg>(DWORD)tServerTime )
		lpPlayInfo->dwTime_PrimeItem_ManaRecharg = lpGameSaveInfo->dwTime_PrimeItem_ManaRecharg;
	else
		lpPlayInfo->dwTime_PrimeItem_ManaRecharg = 0;

	if( lpGameSaveInfo->dwTime_PrimeItem_ManaReduce > (DWORD)tServerTime )		// pluto ���� ���ེ ����
	{
		lpPlayInfo->dwTime_PrimeItem_ManaReduce = lpGameSaveInfo->dwTime_PrimeItem_ManaReduce;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_ManaReduce = 0;
	}

	if( lpGameSaveInfo->dwTime_PrimeItem_MightofAwell > (DWORD)tServerTime )		// pluto ����Ʈ ���� ����
	{
		lpPlayInfo->dwTime_PrimeItem_MightofAwell = lpGameSaveInfo->dwTime_PrimeItem_MightofAwell;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_MightofAwell = 0;
	}

	if( lpGameSaveInfo->dwTime_PrimeItem_MightofAwell2 > (DWORD)tServerTime )		// pluto ����Ʈ ���� ����2
	{
		lpPlayInfo->dwTime_PrimeItem_MightofAwell2 = lpGameSaveInfo->dwTime_PrimeItem_MightofAwell2;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_MightofAwell2 = 0;
	}

	if( lpGameSaveInfo->dwTime_PrimeItem_PhenixPet > (DWORD)tServerTime )		// pluto ��(�ؿ�) ����
	{
		lpPlayInfo->dwTime_PrimeItem_PhenixPet = lpGameSaveInfo->dwTime_PrimeItem_PhenixPet;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_PhenixPet = 0;
		cPCBANGPet.ClosePet();
	}

	// ����� - ���� ����� �� �߰�
	if( lpGameSaveInfo->dwTime_PrimeItem_HelpPet > (DWORD)tServerTime )		
	{
		lpPlayInfo->dwTime_PrimeItem_HelpPet = lpGameSaveInfo->dwTime_PrimeItem_HelpPet;

	}
	else
	{
		lpPlayInfo->smCharInfo.GravityScroolCheck[1] = 0;
		lpPlayInfo->dwTime_PrimeItem_HelpPet = 0;
		cHelpPet.ClosePet(); // ����� - ����� �� ���� ���� ����
	}

	// ����� - �ٷ� ���ེ ����
	if( lpGameSaveInfo->dwTime_PrimeItem_StaminaReduce > (DWORD)tServerTime )		
	{
		lpPlayInfo->dwTime_PrimeItem_StaminaReduce = lpGameSaveInfo->dwTime_PrimeItem_StaminaReduce;
	}
	else
	{
		lpPlayInfo->dwTime_PrimeItem_StaminaReduce = 0;
	}

	return TRUE;

}

//�������� ��� ����
int rsSaveServerForce( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int sec;
	int LifeBooster_sec; // ����� - �ν��� ������(�����)
	int ManaBooster_sec; // ����� - �ν��� ������(���)
	int StaminaBooster_sec; // ����� - �ν��� ������(�ٷ�)
	int nSkillDelay_sec; // �庰 - ��ų ������

	sec = (lpPlayInfo->dwForceOrb_SaveTime-dwPlayServTime)/1000;

	LifeBooster_sec = (lpPlayInfo->dwLifeBooster_SaveTime - dwPlayServTime)/1000/60; // ����� - �ν��� ������(�����)��� �� ���� �ð� ���� 
	ManaBooster_sec = (lpPlayInfo->dwManaBooster_SaveTime - dwPlayServTime)/1000/60; // ����� - �ν��� ������(���)��� �� ���� �ð� ���� 
	StaminaBooster_sec = (lpPlayInfo->dwStaminaBooster_SaveTime - dwPlayServTime)/1000/60; // ����� - �ν��� ������(�ٷ�)��� �� ���� �ð� ���� 
	nSkillDelay_sec = (lpPlayInfo->dwSkillDelay_SaveTime - dwPlayServTime)/1000/60; // �庰 - ��ų ������

	//���� ����
	if ( lpPlayInfo->dwForceOrb_SaveTime && lpPlayInfo->dwForceOrb_SaveTime>dwPlayServTime ) {
		lpGameSaveInfo->wForceOrbUsing[0] = (WORD)(lpPlayInfo->dwForceOrb_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wForceOrbUsing[1] = (WORD)sec;
	}
	else {
		lpGameSaveInfo->wForceOrbUsing[0] = 0;
		lpGameSaveInfo->wForceOrbUsing[1] = 0;
	}

	// ����� - �ν��� ������(�����) ��� �� ���� �ð� ����
	if ( lpPlayInfo->dwLifeBooster_SaveTime && lpPlayInfo->dwLifeBooster_SaveTime>dwPlayServTime ) 
	{
		lpGameSaveInfo->wLifeBoosterUsing[0] = (WORD)(lpPlayInfo->dwLifeBooster_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wLifeBoosterUsing[1] = (WORD)LifeBooster_sec;
	}
	else {
		lpGameSaveInfo->wLifeBoosterUsing[0] = 0;
		lpGameSaveInfo->wLifeBoosterUsing[1] = 0;
	}
	// ����� - �ν��� ������(���) ��� �� ���� �ð� ����
	if ( lpPlayInfo->dwManaBooster_SaveTime && lpPlayInfo->dwManaBooster_SaveTime>dwPlayServTime ) 
	{
		lpGameSaveInfo->wManaBoosterUsing[0] = (WORD)(lpPlayInfo->dwManaBooster_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wManaBoosterUsing[1] = (WORD)ManaBooster_sec;
	}
	else {
		lpGameSaveInfo->wManaBoosterUsing[0] = 0;
		lpGameSaveInfo->wManaBoosterUsing[1] = 0;
	}
	// ����� - �ν��� ������(�ٷ�) ��� �� ���� �ð� ����
	if ( lpPlayInfo->dwStaminaBooster_SaveTime && lpPlayInfo->dwStaminaBooster_SaveTime>dwPlayServTime ) 
	{
		lpGameSaveInfo->wStaminaBoosterUsing[0] = (WORD)(lpPlayInfo->dwStaminaBooster_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wStaminaBoosterUsing[1] = (WORD)StaminaBooster_sec;
	}
	else {
		lpGameSaveInfo->wStaminaBoosterUsing[0] = 0;
		lpGameSaveInfo->wStaminaBoosterUsing[1] = 0;
	}

	// �庰 - ��ų ������
	if ( lpPlayInfo->dwSkillDelay_SaveTime && lpPlayInfo->dwSkillDelay_SaveTime>dwPlayServTime ) 
	{
		lpGameSaveInfo->wSkillDelayUsing[0] = (WORD)(lpPlayInfo->dwSkillDelay_SaveCode&sinITEM_MASK3);
		lpGameSaveInfo->wSkillDelayUsing[1] = (WORD)nSkillDelay_sec;
	}
	else {
		lpGameSaveInfo->wSkillDelayUsing[0] = 0;
		lpGameSaveInfo->wSkillDelayUsing[1] = 0;
	}

	//������ ������ ��ȿ�Ⱓ ����
	if ( lpPlayInfo->dwTime_PrimeItem_X2>(DWORD)tServerTime )
		lpGameSaveInfo->dwTime_PrimeItem_X2 = lpPlayInfo->dwTime_PrimeItem_X2;
	else
		lpGameSaveInfo->dwTime_PrimeItem_X2 = 0;

	if ( lpPlayInfo->dwTime_PrimeItem_ExpUp>(DWORD)tServerTime )
		lpGameSaveInfo->dwTime_PrimeItem_ExpUp = lpPlayInfo->dwTime_PrimeItem_ExpUp;
	else
		lpGameSaveInfo->dwTime_PrimeItem_ExpUp = 0;

	if ( lpPlayInfo->dwTime_PrimeItem_X2 || lpGameSaveInfo->dwTime_PrimeItem_ExpUp ) {
		lpGameSaveInfo->dwPrimeItem_PackageCode = lpPlayInfo->dwPrimeItem_PackageCode;		//��Ű�� ����
	}

	if ( lpPlayInfo->dwTime_PrimeItem_VampCuspid>(DWORD)tServerTime )
		lpGameSaveInfo->dwTime_PrimeItem_VampCuspid = lpPlayInfo->dwTime_PrimeItem_VampCuspid;
	else
		lpGameSaveInfo->dwTime_PrimeItem_VampCuspid = 0;

	// �庰 - ���Ǹ� Ŀ���� EX
	if ( lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX>(DWORD)tServerTime )
		lpGameSaveInfo->dwTime_PrimeItem_VampCuspid_EX = lpPlayInfo->dwTime_PrimeItem_VampCuspid_EX;
	else
		lpGameSaveInfo->dwTime_PrimeItem_VampCuspid_EX = 0;

	if ( lpPlayInfo->dwTime_PrimeItem_ManaRecharg>(DWORD)tServerTime )
		lpGameSaveInfo->dwTime_PrimeItem_ManaRecharg = lpPlayInfo->dwTime_PrimeItem_ManaRecharg;
	else
		lpGameSaveInfo->dwTime_PrimeItem_ManaRecharg = 0;

	if( lpPlayInfo->dwTime_PrimeItem_ManaReduce > (DWORD)tServerTime )		// pluto ���� ���ེ ����
	{
		lpGameSaveInfo->dwTime_PrimeItem_ManaReduce = lpPlayInfo->dwTime_PrimeItem_ManaReduce;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_ManaReduce = 0;
	}

	if( lpPlayInfo->dwTime_PrimeItem_MightofAwell > (DWORD)tServerTime )		// pluto ����Ʈ ���� ����
	{
		lpGameSaveInfo->dwTime_PrimeItem_MightofAwell = lpPlayInfo->dwTime_PrimeItem_MightofAwell;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_MightofAwell = 0;
	}

	if( lpPlayInfo->dwTime_PrimeItem_MightofAwell2 > (DWORD)tServerTime )		// pluto ����Ʈ ���� ����2
	{
		lpGameSaveInfo->dwTime_PrimeItem_MightofAwell2 = lpPlayInfo->dwTime_PrimeItem_MightofAwell2;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_MightofAwell2 = 0;
	}

	if( lpPlayInfo->dwTime_PrimeItem_PhenixPet > (DWORD)tServerTime )		// pluto ��(�ؿ�)
	{
		lpGameSaveInfo->dwTime_PrimeItem_PhenixPet = lpPlayInfo->dwTime_PrimeItem_PhenixPet;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_PhenixPet = 0;
		cPCBANGPet.ClosePet();
	}

	// ����� - ���� ����� �� �߰� ��� �ð�, �� ���� ����
	if( lpPlayInfo->dwTime_PrimeItem_HelpPet > (DWORD)tServerTime )
	{
		lpGameSaveInfo->dwTime_PrimeItem_HelpPet = lpPlayInfo->dwTime_PrimeItem_HelpPet;
	}
	else
	{
		lpPlayInfo->smCharInfo.GravityScroolCheck[1] = 0;
		lpGameSaveInfo->dwTime_PrimeItem_HelpPet = 0;
		cHelpPet.ClosePet(); // ����� - ����� �� ���� ���� ����
	}

	// ����� - �ٷ� ���ེ ����	
	if( lpPlayInfo->dwTime_PrimeItem_StaminaReduce > (DWORD)tServerTime )		
	{
		lpGameSaveInfo->dwTime_PrimeItem_StaminaReduce = lpPlayInfo->dwTime_PrimeItem_StaminaReduce;
	}
	else
	{
		lpGameSaveInfo->dwTime_PrimeItem_StaminaReduce = 0;
	}

	return TRUE;

}
//���� ���� ����
int rsSaveServerPotion( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int cnt1,cnt2;

	if ( rsServerConfig.PotionMonitor && lpPlayInfo->AdminOperMode==0 ) {
		lpGameSaveInfo->sPotionUpdate[0] = rsServerConfig.PotionMonitor;
		lpGameSaveInfo->sPotionUpdate[1] = 0;

		for(cnt1=0;cnt1<3;cnt1++)
			for(cnt2=0;cnt2<4;cnt2++) {
				lpGameSaveInfo->sPotionCount[cnt1][cnt2] = (short)lpPlayInfo->ServerPotion[cnt1][cnt2];
			}

	}
	else {
		lpGameSaveInfo->sPotionUpdate[0] = 0;
		lpGameSaveInfo->sPotionUpdate[1] = 0;
	}

	return TRUE;
}


//���� ������ ��
int rsCompareServerPotion( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo )
{
	int cnt1,cnt2,pot;
	int OverPotion[3][4];
	int OverPotion2[3] = { 0,0,0 };
	int ErrFlag;
	smTRANS_COMMAND	smTransCommand;

	if ( rsServerConfig.PotionMonitor && 
		lpGameSaveInfo->sPotionUpdate[0] && lpGameSaveInfo->sPotionUpdate[0]==lpGameSaveInfo->sPotionUpdate[1] ) {

		ErrFlag = 0;

		for(cnt1=0;cnt1<3;cnt1++) {
			for(cnt2=0;cnt2<4;cnt2++) {
				if ( ((int)lpGameSaveInfo->sPotionCount[cnt1][cnt2])<lpPlayInfo->ServerPotion[cnt1][cnt2]) {
					pot = lpPlayInfo->ServerPotion[cnt1][cnt2]-lpGameSaveInfo->sPotionCount[cnt1][cnt2];
					OverPotion[cnt1][cnt2] = pot;
					OverPotion2[cnt1]+=pot;
					ErrFlag++;
				}
				else
					OverPotion[cnt1][cnt2] = 0;
			}
		}

		if ( ErrFlag ) {
			//���� ó��
			smTransCommand.WParam = 8800;
			smTransCommand.LParam = OverPotion2[0];
			smTransCommand.SParam = OverPotion2[1];
			smTransCommand.EParam = OverPotion2[2];
			RecordHackLogFile( lpPlayInfo , &smTransCommand );

			//���హ�� �ٽ� ���� ( ������� )
			for(cnt1=0;cnt1<3;cnt1++) {
				for(cnt2=0;cnt2<4;cnt2++) {
					lpPlayInfo->ServerPotion[cnt1][cnt2] = lpGameSaveInfo->sPotionCount[cnt1][cnt2];
					if ( lpPlayInfo->ServerPotion[cnt1][cnt2]<0 ) lpPlayInfo->ServerPotion[cnt1][cnt2] = 0;
				}
			}
		}
	}

	return TRUE;
}


//�ٸ� �������� ������ �ҷ��� ����
int ImportTTServerUser( char *szID , char *szServerID )
{
	char szRealID[32];
	char szTTServerPath[128];
	sPLAY_USER_DATA		sPlayUserData;
	char szFile[256];
	char szFile2[256];
	TRANS_RECORD_DATA	TransRecordData;
	char szName[64];
	int	size;
	int cnt;
	FILE	*fp;

	GetTT_ServerPath( szServerID , szTTServerPath );
	GetRealID( szID , szRealID );


	GetUserInfoFile2( szID , szFile , szServerID );

	ZeroMemory( &sPlayUserData , sizeof( sPLAY_USER_DATA ) );
	fp = fopen( szFile , "rb" );
	if ( !fp ) return FALSE;
	fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp );
	fclose(fp);

	for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
		if ( sPlayUserData.szCharName[cnt][0] ) {
			//ĳ���� ���Ͽ��� ����Ÿ �Լ� 

			GetUserDataFile2( sPlayUserData.szCharName[cnt] , szFile2 , szServerID );

			wsprintf( szName , "%s@%s" , sPlayUserData.szCharName[cnt] , szServerID );
			lstrcpy( sPlayUserData.szCharName[cnt] , szName );

			ZeroMemory( &TransRecordData , sizeof(TRANS_RECORD_DATA) );
			fp = fopen( szFile2 , "rb" );
			if ( fp ) {
				fread( &TransRecordData , sizeof(TRANS_RECORD_DATA) , 1 , fp );
				fclose(fp);
			}
			if ( TransRecordData.size>0 ) {
				GetUserDataFile( sPlayUserData.szCharName[cnt] , szFile2 );
				lstrcpy( TransRecordData.smCharInfo.szName , szName );
				lstrcpy( TransRecordData.GameSaveInfo.szMasterID , szID );

				size = TransRecordData.size;
				if ( size<16384 ) size = 16384;

				fp = fopen( szFile2 , "wb" );
				if ( fp ) {
					fwrite( &TransRecordData , size , 1 , fp );
					fclose(fp);
				}
			}
		}
	}

	GetUserInfoFile( szID , szFile );

	lstrcpy( sPlayUserData.szID , szID );

	fp = fopen( szFile , "wb" );
	if ( fp ) {
		fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1 , fp );
		fclose(fp);
	}

	return TRUE;
}


////////////////////////// �����带 ���� ���� ////////////////////////


//���� ����Ÿ�� �����Ű�� ������
DWORD WINAPI RecDataThreadProc( void *pInfo )
{
	HANDLE	hThread;
	int cnt;
	int size;
	FILE *fp;
	sREC_DATABUFF	*recDataBuff;
	int	recDataBuffCount;

	recDataBuff	= new sREC_DATABUFF[REC_DATABUFF_MAX];


	hThread = GetCurrentThread();
	dwLastRecDataTime = GetCurrentTime();

	while(1) {
		SuspendThread(hThread);

		if ( sRecDataBuffCount==0 ) {
			dwLastRecDataTime = GetCurrentTime();
			continue;
		}

		//������ ����Ÿ�� �ִ� �޸� ���� ������ �´�
		//ũ��ƼĮ ���� ����
		EnterCriticalSection( &cRecDataSection );
		recDataBuffCount = sRecDataBuffCount;
		memcpy( recDataBuff , sRecDataBuff , sizeof(sREC_DATABUFF)*sRecDataBuffCount );
		sRecDataBuffCount = 0;
		dwLastRecDataTime = GetCurrentTime();
		//ũ��ƼĮ ���� ����
		LeaveCriticalSection( &cRecDataSection );

		//����� ����Ÿ�� �ϵ��ũ�� �����Ѵ�
		//ũ��ƼĮ ���� ����
		EnterCriticalSection( &cSaveDataSection );
		for( cnt=0;cnt<recDataBuffCount;cnt++ ) {
			//������ ���� ����
			size = recDataBuff[cnt].TransRecData.size;
			if ( size<16000 ) size=16000;

			CopyFile( recDataBuff[cnt].szFileName , recDataBuff[cnt].szBackupFileName , FALSE );		//���Ϲ��
			fp = fopen( recDataBuff[cnt].szFileName , "wb" );
			if ( fp ) {
				fwrite( &recDataBuff[cnt].TransRecData , size , 1, fp );
				fclose( fp );
			}
		}
		//ũ��ƼĮ ���� ����
		LeaveCriticalSection( &cSaveDataSection );
	}

	ExitThread( TRUE );
	return TRUE;
}

//����DB�Լ� �ʱ�ȭ
int rsInitDataBase()
{
	//ũ��ƼĮ ���� �ʱ�ȭ
	InitializeCriticalSection( &cRecDataSection );
	//ũ��ƼĮ ���� �ʱ�ȭ
	InitializeCriticalSection( &cSaveDataSection );

	if ( !sRecDataBuff ) {
		sRecDataBuff = new sREC_DATABUFF[REC_DATABUFF_MAX];
		if ( !sRecDataBuff ) return FALSE;
	}

	if ( !hRecThread ) {
		hRecThread = CreateThread( NULL , 0, RecDataThreadProc , 0 , 0, &dwRecThreadId );
		if ( !hRecThread ) {
			delete sRecDataBuff;
			sRecDataBuff = 0;
			return FALSE;
		}
	}
	sRecDataBuffCount = 0;

	return TRUE;
}

//����DB�Լ� ����ȭ
int rsCloseDataBase()
{
	if ( sRecDataBuff ) {
		delete sRecDataBuff;
		sRecDataBuff = 0;
	}

	return TRUE;
}

//����DB�� ����Ÿ ����䱸
int rsSaveRecData( TRANS_RECORD_DATA *lpTransRecordData , rsPLAYINFO *lpPlayInfo , 
				  char *szFileName , char *szBackupFileName )
{
	if ( sRecDataBuff && hRecThread ) {

		if ( sRecDataBuffCount>=REC_DATABUFF_MAX ) return FALSE;
		if ( sRecDataBuffCount>=REC_DATABUFF_LIMIT ) ResumeThread( hRecThread );

		EnterCriticalSection( &cRecDataSection );				//ũ��ƼĮ ���� ����

		sRecDataBuff[sRecDataBuffCount].lpPlayInfo = lpPlayInfo;
		if ( lpPlayInfo ) {
			sRecDataBuff[sRecDataBuffCount].dwConnectCount  = lpPlayInfo->dwConnectCount;
		}
		else {
			sRecDataBuff[sRecDataBuffCount].dwConnectCount  = 0;
		}

		lstrcpy( sRecDataBuff[sRecDataBuffCount].szName , lpTransRecordData->smCharInfo.szName );

		lstrcpy( sRecDataBuff[sRecDataBuffCount].szFileName , szFileName );
		lstrcpy( sRecDataBuff[sRecDataBuffCount].szBackupFileName , szBackupFileName );
		memcpy( &sRecDataBuff[sRecDataBuffCount].TransRecData , lpTransRecordData , lpTransRecordData->size );
		sRecDataBuffCount++;

		LeaveCriticalSection( &cRecDataSection );				//ũ��ƼĮ ���� ����
		return TRUE;
	}

	return FALSE;
}

//���� ������� ����Ÿ �ִ��� Ȯ��
int CheckRecWaitData( char *szName )
{


	return FALSE;
}

//�ð� Ȯ���Ͽ� ����õ�
int rsTimeRecData()
{
	DWORD dwTime;

	dwTime = GetCurrentTime();

	if ( hRecThread && (dwTime-dwLastRecDataTime)>5000 ) {			//����õ����� 5�� ����
		ResumeThread( hRecThread );						//���� ������ Ȱ��ȭ
		return TRUE;
	}

	return FALSE;
}










/////////////////////////////// ������ ���� ////////////////////////////////
//������ ���� �ε�
int	rsLoadPostBox( rsPLAYINFO	*lpPlayInfo )
{
	rsPOST_BOX_ITEM		*lpPostBox;
	_POST_BOX_ITEM		*lpPostItem;

	char	szFileName[64];
	FILE	*fp;
	char	*p;
	char	 *pb;
	char	szLine[512];
	char	strBuff[512];
	int		cnt;

	if ( !lpPlayInfo->szID[0] ) return FALSE;
	if ( lpPlayInfo->lpPostBoxItem ) return TRUE;

	GetPostBoxFile( lpPlayInfo->szID , szFileName );

	fp = fopen( szFileName , "rb" );
	if ( !fp ) return FALSE;

	lpPlayInfo->lpPostBoxItem = new rsPOST_BOX_ITEM;

	if ( !lpPlayInfo->lpPostBoxItem ) {
		fclose(fp);
		return FALSE;
	}

	ZeroMemory( lpPlayInfo->lpPostBoxItem , sizeof(rsPOST_BOX_ITEM) );

	lpPostBox = lpPlayInfo->lpPostBoxItem;

	while( !feof( fp ) )//  feof: file end���� �о�� 
	{
		if( fgets( szLine, 500, fp ) == NULL)	break;
		if ( lpPostBox->ItemCounter>=POST_ITEM_MAX ) break;

		szLine[500] = 0;

		lpPostItem = &lpPostBox->PostItem[ lpPostBox->ItemCounter ];

		if ( szLine[0] ) {
			p = szLine;

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//���� ĳ���� �̸�
			if ( strBuff[0] ) {
				strBuff[31] = 0;
				lstrcpy( lpPostItem->szCharName , strBuff );
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//������ �ڵ�
			if ( strBuff[0] ) {
				strBuff[31] = 0;
				lstrcpy( lpPostItem->szItemCode , strBuff );

				if ( lstrcmpi( strBuff , "MONEY" )==0 ) {		//��
					lpPostItem->dwItemCode = sinGG1|sin01;
				}
				if ( lstrcmpi( strBuff , "EXP" )==0 ) {			//����ġ
					lpPostItem->dwItemCode = sinGG1|sin02;
				}

				if ( !lpPostItem->dwItemCode ) {
					for(cnt=0;cnt<MAX_ITEM;cnt++) {
						if ( lstrcmpi( sItem[cnt].LastCategory , strBuff )==0 ) {
							lpPostItem->dwItemCode = sItem[cnt].CODE;
							break;
						}
					}
				}
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//Ưȭ �ڵ� ( ���� �ڵ� )
			if ( strBuff[0] ) {
				strBuff[31] = 0;
				lstrcpy( lpPostItem->szSpeJob , strBuff );
				lpPostItem->dwJobCode = atoi( strBuff );
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//����
			if ( strBuff[0] ) {
				strBuff[127] = 0;
				lstrcpy( lpPostItem->szDoc , strBuff );
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//�����ڵ�
			if ( strBuff[0] ) {
				strBuff[63] = 0;
				lstrcpy( lpPostItem->szFormCode , strBuff );
				lpPostItem->dwFormCode = atoi(strBuff);
			}

			pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	//��ȣ�ڵ�
			if ( strBuff[0] ) {
				strBuff[16] = 0;
				lstrcpy( lpPostItem->szPassCode , strBuff );
				lpPostItem->dwPassCode = GetSpeedSum( strBuff );
				lpPostItem->dwParam[0] = TRUE;
			}


			lpPostItem->Flag ++;
			lpPostBox->ItemCounter++;
		}
	}
	fclose(fp);

	DeleteFile( szFileName );			//���� ����

	return TRUE;
}

//������ ���� ����
int	rsSavePostBox( rsPLAYINFO	*lpPlayInfo )
{
	char	szFileName[64];
	char	strBuff[512];
	int		cnt;

	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;


	if ( !lpPlayInfo->szID[0] ) return FALSE;
	if ( !lpPlayInfo->lpPostBoxItem ) return FALSE;

	GetPostBoxFile( lpPlayInfo->szID , szFileName );

	hFile = CreateFile( szFileName , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile==INVALID_HANDLE_VALUE ) {
		return FALSE;
	}

	FileLength = GetFileSize( hFile , NULL );
	SetFilePointer( hFile , FileLength , NULL , FILE_BEGIN );

	for(cnt=0;cnt<lpPlayInfo->lpPostBoxItem->ItemCounter;cnt++) {

		if ( lpPlayInfo->lpPostBoxItem->PostItem[cnt].Flag ) {

			wsprintf( strBuff , "%s		%s		%s		\"%s\"	%s	%s\r\n", 
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szCharName,
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szItemCode, 
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szSpeJob,
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szDoc ,
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szFormCode,
				lpPlayInfo->lpPostBoxItem->PostItem[cnt].szPassCode );

			WriteFile( hFile , strBuff , lstrlen(strBuff) , &dwAcess , NULL );
		}
	}

	CloseHandle( hFile );

	return TRUE;
}

//�����ۼ��� ���� ����
int	rsAddPostBox_Present( rsPLAYINFO *lpPlayInfo )
{
	char	szFileName[64];
	int		len;

	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;

	char	strBuff[16384];

	if ( !lpPlayInfo->szID[0] ) return FALSE;

	hFile = CreateFile( "Present.dat" , GENERIC_READ , FILE_SHARE_READ, NULL ,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL );

	if ( hFile ==INVALID_HANDLE_VALUE ) return FALSE;

	len = GetFileSize( hFile,NULL );
	if ( len>16384 ) return FALSE;

	ReadFile( hFile , strBuff, len , &dwAcess , NULL );
	CloseHandle( hFile );

	GetPostBoxFile( lpPlayInfo->szID , szFileName );

	hFile = CreateFile( szFileName , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile==INVALID_HANDLE_VALUE ) {
		return FALSE;
	}

	FileLength = GetFileSize( hFile , NULL );
	SetFilePointer( hFile , FileLength , NULL , FILE_BEGIN );

	WriteFile( hFile , strBuff , len , &dwAcess , NULL );

	CloseHandle( hFile );

	return TRUE;
}


//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����
int	OpenStarPointEvent( rsPLAYINFO *lpPlayInfo , smCHAR_INFO *lpCharInfo )
{
	int cnt;

	if ( !lpCharInfo ) {
		if ( lpPlayInfo->CharLevelMax<rsServerConfig.Event_StarPointTicket ) {

			if ( !lpPlayInfo->CharLevelMax ) {
				lpPlayInfo->Event_StarTicketLevel = 3+(rand()%6);
			}
			else {
				cnt = (lpPlayInfo->CharLevelMax/10)+1;
				cnt = cnt*10;
				lpPlayInfo->Event_StarTicketLevel = cnt+ ( rand()%10 );
			}
		}

		return TRUE;
	}


	//�� ��ǰ�� �߻���Ű��
	if ( lpPlayInfo->CharLevelMax<rsServerConfig.Event_StarPointTicket &&
		lpCharInfo->Level>=lpPlayInfo->CharLevelMax ) {		//�ִ� ������ Ȯ���Ͽ� ����

		if ( lpCharInfo->sEventParam[0]==CHAR_EVENT_STARPOINT ) {

		}
		else {
			lpCharInfo->sEventParam[0] = CHAR_EVENT_STARPOINT;
			lpCharInfo->sEventParam[1] = 0;
		}

		cnt = (lpCharInfo->Level/10)+1;
		cnt = cnt*10;

		if ( lpCharInfo->Level>lpCharInfo->sEventParam[1] ) {
			//��ǰ���� �߻���ų ����
			lpCharInfo->sEventParam[1] = cnt+ ( rand()%10 );
		}

		lpPlayInfo->Event_StarTicketLevel = lpCharInfo->sEventParam[1];
	}

	return TRUE;
}


//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����
int	CloseStarPointEvent( rsPLAYINFO *lpPlayInfo , smCHAR_INFO *lpCharInfo )
{

	if ( lpPlayInfo->Event_StarTicketLevel ) {
		lpCharInfo->sEventParam[0] = CHAR_EVENT_STARPOINT;
		lpCharInfo->sEventParam[1] = lpPlayInfo->Event_StarTicketLevel;
	}

	return TRUE;

}

//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����
int	OpenStarPointTicket( rsPLAYINFO *lpPlayInfo )
{
	int cnt;
	int Price;

	if ( lpPlayInfo->Event_StarTicketLevel && 
		lpPlayInfo->Event_StarTicketLevel<rsServerConfig.Event_StarPointTicket &&
		lpPlayInfo->smCharInfo.Level>=lpPlayInfo->Event_StarTicketLevel ) {

		cnt = (lpPlayInfo->smCharInfo.Level/10)+1;
		cnt = cnt*10;

		lpPlayInfo->Event_StarTicketLevel = cnt+ ( rand()%10 );

		Price = 100000;

		switch( cnt ) {
			case 10:
				Price = 200000;
				break;
			case 20:
				Price = 300000;
				break;
			case 30:
				Price = 500000;
				break;
			case 40:
				Price = 1000000;
				break;
		}

#ifdef	_TEST_SERVER
		//�׽�Ʈ ���������� �����ڸ� ������ �߻�
		if ( !lpPlayInfo->AdminMode )
			return TRUE;
#endif

		//����Ʈ Ƽ�� ������ Ư�� ������ �κ��丮����
		rsPutItem_PointTicket( lpPlayInfo , Price );

	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////


//#define _CONV_CLOSE_USER
//#define _CHECK_HACK_USER

////////////////////// ���� ��� ��� / ��ȯ /////////////////////////////

//��� ����
int SaveCloseUserRecord( char *szUserName , int Level , int Exp , int Money  )
{
	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	char	szFileName[128];
	char	szBuff[256];
	int		TotalMoney = 0;


	wsprintf( szFileName , "UserRecord.txt"  );

	wsprintf( szBuff , "%s		����(%d)	����ġ(%d)	��(%d)\r\n",szUserName , Level , Exp , Money );

	hFile = CreateFile( szFileName , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile==INVALID_HANDLE_VALUE ) return FALSE;

	FileLength = GetFileSize( hFile , NULL );
	SetFilePointer( hFile , FileLength , NULL , FILE_BEGIN );
	WriteFile( hFile , szBuff , lstrlen(szBuff) , &dwAcess , NULL );
	CloseHandle( hFile );

	return TRUE;
}

//â�� �� ���
int SaveWareHouseRecord( char *szUserName , int Money , int Weight1,int Weight2 )
{
	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	char	szFileName[128];
	char	szBuff[256];
	int		TotalMoney = 0;


	wsprintf( szFileName , "UserRecord.txt"  );

	wsprintf( szBuff , "ID( %s )	â��(%d)	����( %d/%d )\r\n",szUserName , Money  , Weight1 ,Weight2 );

	hFile = CreateFile( szFileName , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile==INVALID_HANDLE_VALUE ) return FALSE;

	FileLength = GetFileSize( hFile , NULL );
	SetFilePointer( hFile , FileLength , NULL , FILE_BEGIN );
	WriteFile( hFile , szBuff , lstrlen(szBuff) , &dwAcess , NULL );
	CloseHandle( hFile );

	return TRUE;
}

/*
/////////////////////////////////////////// ���� ///////////////////////////////////////////
int TotalMoney =0;			//�� �ݾ� * 1000��


//������ ����Ÿ�� �����Ͽ� Ŭ���̾�Ʈ�� ����
int ConvertInitGameBetaData( char *szName )
{
	int cnt;
	char *lpData;
	char szFile[64];
	FILE *fp;
	int level;
	int	recFlag = 0;

	TRANS_RECORD_DATA TransRecordData;
	TRANS_RECORD_DATA *lpTransRecord;

	lpData = (char *)&TransRecordData;
	lpTransRecord = &TransRecordData;

	//���Ϻҷ�����
	wsprintf( szFile , "userdata\\%s.dat" , szName );
	//GetUserDataFile( szName , szFile );

	fp = fopen( szFile , "rb" );
	if ( fp ) {
		fread( lpTransRecord , sizeof(TRANS_RECORD_DATA) , 1, fp );
		fclose( fp );
	}
	else
		return FALSE;

	if ( lpTransRecord->smCharInfo.Level>=60 ||
		lpTransRecord->smCharInfo.Money>=10000000 ) {
		//����Ÿ ���
		SaveCloseUserRecord( szName , lpTransRecord->smCharInfo.Level , lpTransRecord->smCharInfo.Exp , lpTransRecord->smCharInfo.Money  );
	}

	//���� 20% �ٿ�
	level =  ( lpTransRecord->smCharInfo.Level * 80 )/100;
	if ( ((lpTransRecord->smCharInfo.Level * 80 )%100)>0 ) level ++;	//������ ����
	lpTransRecord->smCharInfo.Level = level;
	TotalMoney += lpTransRecord->smCharInfo.Money;						//�� ���� �հ�
	lpTransRecord->smCharInfo.Money = level * 1000;						//�� ���� * 1000

	//���� ����ġ ����
	lpTransRecord->smCharInfo.Next_Exp = GetNextExp( lpTransRecord->smCharInfo.Level );
	lpTransRecord->smCharInfo.Exp = GetNextExp( lpTransRecord->smCharInfo.Level-1 );

	switch( lpTransRecord->smCharInfo.JOB_CODE ) {
	case 1:
		lstrcpy( lpTransRecord->smCharInfo.szModelName , "char\\tmABCD\\B001.ini" );
		break;
	case 2:
		lstrcpy( lpTransRecord->smCharInfo.szModelName , "char\\tmABCD\\A001.ini" );
		break;
	case 3:
		lstrcpy( lpTransRecord->smCharInfo.szModelName , "char\\tmABCD\\D001.ini" );
		break;
	case 4:
		lstrcpy( lpTransRecord->smCharInfo.szModelName , "char\\tmABCD\\C001.ini" );
		break;
	}

	//�ɷ�ġ �ʱ�ȭ
	for( cnt=0;cnt<4;cnt++ ) {
		if ( TempNewCharacterInit[ cnt ][0]==(int)lpTransRecord->smCharInfo.JOB_CODE ) {
			lpTransRecord->smCharInfo.Strength	= TempNewCharacterInit[ cnt ][1];
			lpTransRecord->smCharInfo.Spirit	= TempNewCharacterInit[ cnt ][2];
			lpTransRecord->smCharInfo.Talent	= TempNewCharacterInit[ cnt ][3];
			lpTransRecord->smCharInfo.Dexterity	= TempNewCharacterInit[ cnt ][4];
			lpTransRecord->smCharInfo.Health	= TempNewCharacterInit[ cnt ][5];
			break;
		}
	}

	//ĳ���� ������Ʈ ���� ���Ӱ� �����Ѵ�
	ReformCharStatePoint( &lpTransRecord->smCharInfo );

	//ĳ���� ���� ����
	ReformCharForm( &lpTransRecord->smCharInfo );

	if ( lpTransRecord->GameSaveInfo.Head<dwRecordVersion &&
		lstrcmp( lpTransRecord->szHeader , szRecordHeader )!=0 ) {
		//������ ����Ÿ �߰� ĳ���� ���� �ڵ� ���� ���
		lpTransRecord->GameSaveInfo.dwChkSum_CharInfo = GetCharInfoCode( &lpTransRecord->smCharInfo );
	}
	//��ų �ʱ�ȭ
	ZeroMemory( &lpTransRecord->GameSaveInfo.SkillInfo , sizeof( RECORD_SKILL ) );

	lpTransRecord->ItemCount = 0; 
	lpTransRecord->DataSize = 0;
	lpTransRecord->GameSaveInfo.LastMoeny = 0;
	lpTransRecord->ThrowItemCount = 0;

	GetUserDataFile( szName , szFile );

	lpTransRecord->size = sizeof(TRANS_RECORD_DATA)-(sizeof(sRECORD_ITEM)*RECORD_ITEM_MAX);

	fp = fopen( szFile , "wb" );
	if ( fp ) {
		fwrite( lpTransRecord , lpTransRecord->size , 1, fp );
		fclose( fp );
	}


	return TRUE;
}

int InitBetaUserData()
{
	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;
	char	szFindPath[64];
	char	szUserName[64];
	int		cnt,len;

	lstrcpy( szFindPath , "UserData\\*.dat" );

	hFindHandle = FindFirstFile( szFindPath , &fd );
	if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
		while(1) {
			lstrcpy( szUserName , fd.cFileName );
			len = lstrlen( szUserName );

			for( cnt=0;cnt<len;cnt++ ) {
				if ( szUserName[cnt]=='.' ) {
					szUserName[cnt] = 0;
					break;
				}
			}

			//������ ����Ÿ�� �����Ͽ� Ŭ���̾�Ʈ�� ����
			ConvertInitGameBetaData( szUserName );

			//���� ���� ã��
			if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
		}
	}

	FindClose( hFindHandle );

	return TRUE;
}

int InitBetaUserInfo()
{
	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;
	char	szFindPath[64];
	char	szUserName[64];
	char	szSrcFile[128];
	char	szConvertFile[128];
	int		cnt,len;

	lstrcpy( szFindPath , "UserInfo\\*.dat" );

	hFindHandle = FindFirstFile( szFindPath , &fd );
	if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
		while(1) {
			lstrcpy( szUserName , fd.cFileName );
			len = lstrlen( szUserName );

			for( cnt=0;cnt<len;cnt++ ) {
				if ( szUserName[cnt]=='.' ) {
					szUserName[cnt] = 0;
					break;
				}
			}

			//������ ����Ÿ�� �����Ͽ� Ŭ���̾�Ʈ�� ����
			//ConvertInitGameBetaData( szUserName );
			GetUserInfoFile( szUserName , szConvertFile );
			wsprintf( szSrcFile , "UserInfo\\%s" , fd.cFileName );

			CopyFile( szSrcFile , szConvertFile , FALSE );

			//���� ���� ã��
			if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
		}
	}

	FindClose( hFindHandle );

	return TRUE;
}

//������ ����Ÿ�� �����Ͽ� Ŭ���̾�Ʈ�� ����
//int ConvertInitGameBetaData( char *szName )

//Ŭ���� ��Ÿ �׽��� �ʱ�ȭ
int	InitCloseBetaUser()
{
	//����Ÿ ���弭�� ���丮 ����
	CreateDataServerDirectory();

	InitBetaUserData();
	InitBetaUserInfo();

	return TRUE;
}















*/








int PeekMonMsg();
int	MonQuit = 0;
HWND hDialog;
HWND hWndMon;
extern HINSTANCE hinst;								// current instance
//�޽��� ���׷��̵� ���� �޴�â ���ν���
LONG APIENTRY DialogProc( HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam );
LONG APIENTRY MonitorProc( HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam );

#define RECORD_MONEY_LIMIT		100000


int CheckUserData( char *szName )
{
	char *lpData;
	char szFile[256];
	FILE *fp;
	int	recFlag = 0;

	TRANS_RECORD_DATA TransRecordData;
	TRANS_RECORD_DATA *lpTransRecord;

	lpData = (char *)&TransRecordData;
	lpTransRecord = &TransRecordData;

	GetUserDataFile( szName , szFile );

	fp = fopen( szFile , "rb" );
	if ( fp ) {
		fread( lpTransRecord , sizeof(TRANS_RECORD_DATA) , 1, fp );
		fclose( fp );
	}
	else
		return FALSE;

	if ( lpTransRecord->smCharInfo.Level>=70 ||
		lpTransRecord->smCharInfo.Money>=RECORD_MONEY_LIMIT ) {
		//����Ÿ ���
		SaveCloseUserRecord( szName , lpTransRecord->smCharInfo.Level , lpTransRecord->smCharInfo.Exp , lpTransRecord->smCharInfo.Money  );
	}

/*
	fp = fopen( szFile , "wb" );
	if ( fp ) {
		fwrite( lpTransRecord , lpTransRecord->size , 1, fp );
		fclose( fp );
	}
*/

	return TRUE;
}

int CheckWareHouseData( char *szFile )
{
	TRANS_WAREHOUSE	TransWareHouse;
	sWAREHOUSE	WareHouse;
	FILE *fp;
	int cnt,len;
	char szName[256];

	fp = fopen( szFile , "rb" );
	if ( fp ) {
		fread( &TransWareHouse , sizeof(TRANS_WAREHOUSE) , 1, fp );
		fclose( fp );
	}
	else
		return FALSE;
	
	DecodeCompress( (BYTE *)&TransWareHouse.Data , (BYTE *)&WareHouse );

	if ( WareHouse.Weight[0]>WareHouse.Weight[1] || WareHouse.Money>=RECORD_MONEY_LIMIT ) {

		lstrcpy( szName , szFile );
		len = lstrlen( szName );

		for( cnt=0;cnt<len;cnt++ ) {
			if ( szName[cnt]=='.' ) {
				szName[cnt] = 0;
				break;
			}
		}
		for( cnt=cnt;cnt>=0;cnt-- ) {
			if ( szName[cnt]=='\\' )  {
				cnt++;
				break;
			}
		}
		SaveWareHouseRecord( szName+cnt , WareHouse.Money , WareHouse.Weight[0],WareHouse.Weight[1]);
	}

	return TRUE;
}

int CheckUserDataFull()
{
	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;
	char	szFindPath[64];
	char	szUserName[64];
	char	szMsgBuff[256];
	int		cnt,len;
	int		UserCnt;
	int		CharCount = 0;

	for( UserCnt=0;UserCnt<256;UserCnt++ ) {

		wsprintf( szFindPath , "DataServer\\UserData\\%d\\*.dat" ,UserCnt );

		wsprintf( szMsgBuff , "ĳ���� Ȯ���� ( %d/255 ) - %d", UserCnt,CharCount );
		SetWindowText(GetDlgItem( hDialog , IDC_EDIT1 ) , szMsgBuff );


		hFindHandle = FindFirstFile( szFindPath , &fd );
		if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
			while(1) {
				lstrcpy( szUserName , fd.cFileName );
				len = lstrlen( szUserName );

				for( cnt=0;cnt<len;cnt++ ) {
					if ( szUserName[cnt]=='.' ) {
						szUserName[cnt] = 0;
						break;
					}
				}

				CheckUserData( szUserName );
				CharCount++;

				PeekMonMsg();		//�޼��� �߰� ó��

				//���� ���� ã��
				if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
				if ( MonQuit ) break;
			}
		}
		FindClose( hFindHandle );
	}

	return TRUE;
}

int CheckWareHouseDataFull()
{
	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;
	char	szFindPath[64];
	char	szUserName[64];
	char	szUserPath[64];
	char	szMsgBuff[256];

	int		cnt,len;
	int		UserCnt;
	int		CharCount = 0;

	for( UserCnt=0;UserCnt<256;UserCnt++ ) {

		wsprintf( szFindPath , "DataServer\\WareHouse\\%d\\*.war",UserCnt );

		wsprintf( szMsgBuff , "�����ۺ����� Ȯ���� ( %d/255 ) - %d", UserCnt , CharCount );
		SetWindowText(GetDlgItem( hDialog , IDC_EDIT1 ) , szMsgBuff );

		hFindHandle = FindFirstFile( szFindPath , &fd );
		if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
			while(1) {
				lstrcpy( szUserName , fd.cFileName );
				len = lstrlen( szUserName );

				for( cnt=0;cnt<len;cnt++ ) {
					if ( szUserName[cnt]=='.' ) {
						szUserName[cnt] = 0;
						break;
					}
				}

				wsprintf( szUserPath , "DataServer\\WareHouse\\%d\\%s" ,UserCnt, fd.cFileName );
				CheckWareHouseData( szUserPath );

				CharCount++;
				PeekMonMsg();		//�޼��� �߰� ó��

				//���� ���� ã��
				if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
				if ( MonQuit ) break;
			}
		}
		FindClose( hFindHandle );
	}

	return TRUE;
}

// ���� ����â â ����
HWND CreateDialogWnd();

//������ ��ϵ� ��ü ����Ÿ�� Ȯ���Ͽ� �ǽɰ��� ������ ã�´�
int	CheckServerRecordData()
{
	DeleteFile( "UserRecord.txt" );
	CreateDialogWnd();
	CheckUserDataFull();
	if ( MonQuit ) return FALSE;
	CheckWareHouseDataFull();
	if ( MonQuit ) return FALSE;

	return TRUE;
}


// ���� ����â â ����
HWND CreateDialogWnd()
{

	char *szAppName = "������������ - �����";
	WNDCLASS wndclass;


		wndclass.style			=	CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
		wndclass.lpfnWndProc	=	MonitorProc;
		wndclass.cbClsExtra		=	0;
		wndclass.cbWndExtra		=	0;
		wndclass.hInstance		=	hinst;
		wndclass.hIcon			=	NULL;
		wndclass.hCursor		=	LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground	=	(HBRUSH)GetStockObject(BLACK_BRUSH);
		wndclass.lpszMenuName	=	NULL;
		wndclass.lpszClassName	=	szAppName;
		RegisterClass(&wndclass);

		hWndMon=CreateWindow(szAppName,szAppName,
				  WS_OVERLAPPEDWINDOW| WS_POPUP|WS_EX_TOPMOST,CW_USEDEFAULT,
				  CW_USEDEFAULT,320,240,NULL,NULL,
				  hinst,NULL);


	ShowWindow(hWndMon,SW_HIDE);
	UpdateWindow(hWndMon);


	hDialog = CreateDialog( hinst, "IDD_DIALOG", hWndMon, (DLGPROC)DialogProc );
/*
	int x,y;
	x = (GetSystemMetrics( SM_CXSCREEN )/2) - 128;
	y = (GetSystemMetrics( SM_CYSCREEN )/2) - 90 ;
	SetWindowPos( hDialog , 0, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
*/
	ShowWindow( hDialog,SW_SHOW);
	UpdateWindow( hDialog );

	return hDialog;
}

//�޽��� ���׷��̵� ���� �޴�â ���ν���
LONG APIENTRY DialogProc( HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam )
{

	//char szTemp[128];

    switch (messg)
    {
        case WM_INITDIALOG:
			break;

		case WM_CLOSE:
			DestroyWindow( hWnd );
			break;

		case WM_PAINT:
			break;

        case WM_COMMAND:
			switch( LOWORD(wParam) ) {
			case IDOK:
				SendMessage( hWnd , WM_CLOSE , 0, 0 );
				MonQuit = TRUE;	
				break;
			}

			break;

		default:
			return FALSE;
			break;
    }
	return FALSE;
}

LONG APIENTRY MonitorProc( HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam )
{

	//char szTemp[128];

    switch (messg)
    {
		case WM_CREATE:
			break;

		default: 
			return DefWindowProc(hWnd,messg,wParam,lParam);
			break;

    }
	return FALSE;
}

int PeekMonMsg()
{
	MSG msg;

	while (1) {
	    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	    {
	      TranslateMessage(&msg);
	      DispatchMessage(&msg);
	    }
		else
			break;
	}
	return TRUE;
}








///////////////////// ĳ���� ��� ����Ÿ�� ���� //////////////////////


static char decode[512];
static char line[512];

static char *GetWord(char *q , char *p)
{

		while ( (*p == 32) || (*p == 9) ) // SPACE or TAB or ':'�� ���� ��Ŵ
		{
			p++;
		}
	
		while ( (*p != 32) && (*p != 9) ) // SPACE or TAB
		{
			if ( *p == '\n' || *p=='\r' ) break;
			*q++ = *p++;
		}
		*q++ = 0;	// end of one string, put Null character

	return p;
}






/*
static int GetUserInfoFile( char *szID , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserInfoDir , szID );

	if ( szID[4]==0 && szID[3]>='0' && szID[3]<='9' && (
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='m' || szID[2]=='M')) || 
		((szID[0]=='l' || szID[0]=='L') && (szID[1]=='p' || szID[1]=='P') && (szID[2]=='t' || szID[2]=='T')) ) 
		) {
 		wsprintf( szFileName , "DataServer\\��%s\\%d\\%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	if ( szID[3]==0 && 
		((szID[0]=='p' || szID[0]=='P') && (szID[1]=='r' || szID[1]=='R') && (szID[2]=='n' || szID[2]=='N')) ||
		((szID[0]=='c' || szID[0]=='C') && (szID[1]=='o' || szID[1]=='O') && (szID[2]=='n' || szID[2]=='N')) 
		) {
 		wsprintf( szFileName , "DataServer\\��%s\\%d\\%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
		return TRUE;
	}

	wsprintf( szFileName , "DataServer\\%s\\%d\\%s.dat" , szRecordUserInfoDir , GetUserCode(szID) , szID );
	
	return TRUE;
}

static int GetUserDataFile( char *szName , char *szFileName )
{
	//wsprintf( szFileName , "%s\\%s.dat" , szRecordUserDataDir , szName );
	wsprintf( szFileName , "DataServer\\%s\\%d\\%s.dat" , szRecordUserDataDir , GetUserCode(szName) , szName );

	return TRUE;
}
*/


//ĳ���� ���� ���Ͽ��� �ص��Ͽ� �����Ѵ�
int RestoreBackupData( char *szListFile , char *BackupPath )
{

	FILE	*fp , *fp2 , *fp3;
	char strBuff[256];
	char *p;
	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;
	sPLAY_USER_DATA		sPlayUserData;
	int cnt;
	int flag;

	char szInfoFile[256];
	char szSrcDataFile[256];
	char szDestDataFile[256];
	char szLogBuff[256];
	char szFile[256];

	fp = fopen( szListFile , "rb" );
	if ( fp==NULL ) return FALSE;

	fp3 = fopen( "BackupData.log" , "wb" );
	if ( fp3==NULL ) {
		fclose(fp);
		return FALSE;
	}

	CreateDirectory( "BackupUser" , 0 );

	while( !feof( fp ) )//  feof: file end���� �о�� 
	{
		if( fgets( line, 255, fp ) == NULL)	break;

		p = GetWord( decode , line);

		///////////////// �̸� ///////////////////////
		if ( decode[0] ) {
			p=GetWord(strBuff,p);

			if ( strBuff[0] ) {

				///////////////// ��� ���� ///////////////////
				GetUserInfoFile( decode , szInfoFile );
				GetUserDataFile( strBuff , szDestDataFile );
				lstrcpy( szSrcDataFile , BackupPath );
				lstrcat( szSrcDataFile , szDestDataFile );
				///////////////////////////////////////////////

				flag = TRUE;

			 	hFind = FindFirstFile( szSrcDataFile , &ffd );
				FindClose( hFind );
				if ( hFind!=INVALID_HANDLE_VALUE ) {
					//�ҽ� ����Ÿ ���� ����
					fp2 = fopen( szInfoFile , "rb" );
					if ( fp2 ) {
						fread( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp2 );
						fclose(fp2);

						for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
							if ( sPlayUserData.szCharName[cnt][0] ) {
								if ( lstrcmpi( sPlayUserData.szCharName[cnt] , strBuff )==0 ) {
									break;
								}
							}
						}

						if ( cnt>=sPLAY_CHAR_MAX ) {
							//�����ȿ� ĳ���� ����
						 	hFind = FindFirstFile( szDestDataFile , &ffd );
							FindClose( hFind );
							if ( hFind==INVALID_HANDLE_VALUE ) {
								//���� ĳ���� ���� ���� �ʴ´� . ������ ĳ�� ����
								for( cnt=0;cnt<sPLAY_CHAR_MAX;cnt++) {
									if ( !sPlayUserData.szCharName[cnt][0] ) {
										lstrcpy( sPlayUserData.szCharName[cnt] , strBuff );
										fp2 = fopen( szInfoFile , "wb" );
										if ( fp2 ) {
											fwrite( &sPlayUserData , sizeof( sPLAY_USER_DATA ) , 1, fp2 );
											fclose(fp2);
										}
										break;
									}
								}
							}
							else {
								flag = FALSE;
								wsprintf( szLogBuff , "%s (%s) -> ���� ( �ű�ĳ�������� )\r\n", decode , strBuff );
								fwrite( szLogBuff , lstrlen( szLogBuff ) , 1, fp3 );
							}
						}

						if ( flag ) {
							//���� ����

							wsprintf( szFile , "BackupUser\\%s.dat" ,strBuff );
							CopyFile( szDestDataFile , szFile , TRUE );

							CopyFile( szSrcDataFile , szDestDataFile , FALSE );

							wsprintf( szLogBuff , "%s (%s) -> ����\r\n", decode , strBuff );
							fwrite( szLogBuff , lstrlen( szLogBuff ) , 1, fp3 );
						}
					}
					else {
						wsprintf( szLogBuff , "%s (%s) -> ���� ( �����̾��� )\r\n", decode , strBuff );
						fwrite( szLogBuff , lstrlen( szLogBuff ) , 1, fp3 );
					}
				}
				else {
					wsprintf( szLogBuff , "%s (%s) -> ���� ( �ҽ����Ͼ��� )\r\n", decode , strBuff );
					fwrite( szLogBuff , lstrlen( szLogBuff ) , 1, fp3 );
				}
			}
		}
	}


	fclose(fp3);
	fclose(fp);

	return TRUE;
}


/*
	DWORD	dwLoginServerIP;		//�α����� ������ IP
	DWORD	dwLoginServerSafeKey;	//�α����� �������� ���� ����Ű
*/
#ifdef _W_SERVER

#ifdef _LANGUAGE_ENGLISH
#ifndef _LANGUAGE_PHILIPIN
#define	LOGIN_SERVER_KEY		0x512234a5		//����
#define	LOGIN_SERVER_SHIFT		6
#else 
#define	LOGIN_SERVER_KEY		0x542634c3
#define	LOGIN_SERVER_SHIFT		4
#endif
#endif

//�߱�
#ifdef _LANGUAGE_CHINESE
#define	LOGIN_SERVER_KEY		0x512234a5
#define	LOGIN_SERVER_SHIFT		6
#endif

#ifdef _LANGUAGE_BRAZIL
#define	LOGIN_SERVER_KEY		0x532254a5
#define	LOGIN_SERVER_SHIFT		5
#endif

#ifdef _LANGUAGE_ARGENTINA		//�Ƹ���Ƽ�� (����� ���ð� �ӽ� ����)
#define	LOGIN_SERVER_KEY		0x532254a5
#define	LOGIN_SERVER_SHIFT		5
#endif

#ifdef _LANGUAGE_VEITNAM
#define	LOGIN_SERVER_KEY		0x442934c3
#define	LOGIN_SERVER_SHIFT		3
#endif

#ifdef _LANGUAGE_JAPANESE
#define	LOGIN_SERVER_KEY		0x542634c5
#define	LOGIN_SERVER_SHIFT		6
/*
#define	LOGIN_SERVER_KEY		0x542634c3
#define	LOGIN_SERVER_SHIFT		4
*/
#endif

#ifdef _LANGUAGE_THAI
#define	LOGIN_SERVER_KEY		0x533734c3
#define	LOGIN_SERVER_SHIFT		7
#endif


//�α��� ���� ���� Ű ����
DWORD	rsGetLoginSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode )
{
	DWORD	dwCode = dwSpdSumCode;
	DWORD	dwCode2 = lpCharInfo->dwObjectSerial ^ LOGIN_SERVER_KEY;

	if ( !dwCode ) 
		dwCode = GetSpeedSum( lpCharInfo->szName );

	dwCode = dwCode * ((dwCode2>>LOGIN_SERVER_SHIFT)|(dwCode2<<LOGIN_SERVER_SHIFT));

	return	dwCode;
}

//�α��� ���� ����Ű ����
int rsSet_LoginServerSafeKey( smCHAR_INFO *lpCharInfo )
{

	lpCharInfo->dwLoginServerSafeKey = rsGetLoginSafeKey( lpCharInfo );

	return TRUE;
}

//�α��� ���� ����Ű Ȯ��
int	rsCheck_LoginServerSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode )
{
	if ( lpCharInfo->dwLoginServerSafeKey==rsGetLoginSafeKey( lpCharInfo , dwSpdSumCode ) ) 
		return TRUE;

	return FALSE;
}

#else
DWORD	rsGetLoginSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode )
{
	return TRUE;
}
int rsSet_LoginServerSafeKey( smCHAR_INFO *lpCharInfo )
{
	return TRUE;
}
int	rsCheck_LoginServerSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode )
{
	return TRUE;
}
#endif







