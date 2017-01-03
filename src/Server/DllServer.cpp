#include <windows.h>
#include <io.h>
#include <stdio.h>         /* for sprintf                           */
#include <string.h>        /* for strlen                            */
#include <memory.h>
#include <process.h>       /* for _beginthread                      */

#include "..\\resource.h"
#include "..\\smwsock.h"

#include "..\\smlib3d\\smd3d.h"
#include "..\\character.h"
#include "..\\fileread.h"
#include "..\\playmain.h"
#include "..\\field.h"
#include "..\\sinbaram\\sinlinkheader.h"
#include "..\\effectsnd.h"

#include "..\\record.h"
#include "..\playsub.h"

#include "DllServer.h"
#include "onserver.h"

#define		BILLING_METER_RATE



#define	SWM_DISPDEBUG	(WM_USER+77)


char	szServerName[64] = "Unnamed Server";
extern  char *srMsg_ThanksPolling;		//�������� ���� �޼���


CRITICAL_SECTION	cSqlSection;

#ifdef _W_SERVER

#include "PristonSQLDll.h"

//IME ���� ���̺귯�� �߰�
#pragma comment( lib, "Library\\PristonSQLDll.lib" )

struct sLOG_SQLDATA {
	DWORD	dwRecCode;
	char	szUserID[32];
	char	szCharName[32];
	char	szRace[16];
	char	szOccupation[16];
	int 	Occupation;
	DWORD	dwExpPoint;
	INT64	Exp64;
	DWORD	Level;
	DWORD	dwMoney;
	char	szIP[16];
	char	szGuid[128];

	char	szGPCode[16];
	time_t	DisuseDay;
	int		UsePeriod;
	int		PC_RNo;				//PC�� �÷�

	int		Time;

	//�ش� ���� ������
	rsPLAYINFO	*lpPlayInfo;
	DWORD		dwConnectCode;
	//smWINSOCK	*lpsmSock;

};

sLOG_SQLDATA *LogSqlData=0;
int			LogSqlDataCount;
int			LogSqlSendCount;
CRITICAL_SECTION	cLogSqlSection;			//ũ��Ƽ�ü���
HANDLE			hLogSqlThread =0;
DWORD			dwLogSqlThreadId;
int			LogSqlLossCount = 0;

DWORD		LogSqlProcessTime = 0;			//ó�� �ð�
DWORD		LogSqlProcessTimeMax = 0;		//ó�� �ð� �ִ�ġ
int			LogSqlProcessCount = 0;			//ó�� Ƚ��
int			LogSqlProcessQueMax = 0;		//ó������ ť�� �ִ�ġ

int			UseDLL_Code = 0;
int			UseDLL_Expined = 0;

//int			UrsFlag = 0;					//Ursó
//URSLoginIP

#define		LOG_SQL_BUFF_MAX		2048
#define		LOG_SQL_BUFF_MASK		2047
#define		LOG_SQL_BUFF_LIMIT		1023




LogITEM_QUE *LogItemQue=0;
int			LogItemQuePush;
int			LogItemQuePop;
int			LogItemLossCount = 0;
DWORD		LogItemQueProcessTime = 0;		//ó�� �ð�


struct	sLOG_EVENT {
	char	szID[32];
	char	szName[32];
	char	szGPCode[16];		//��ǰ�ڵ�
	char	szIP[16];
	int		Param[32];
};

struct	sCLAN_EVENT {
	char	szID[32];
	char	szName[32];
	char	szIP[16];
	int		TicketNum;
	int		Param[8];
};

struct POLL {
	int		size;
	char	UserID[32];
	char	CharID[32];
	int		PollCode;
	int		PollCount;
	int		PollItem[10];
};

/*
					BillSql.WithSODRecord( logSqlData.szUserID , logSqlData.szCharName , logSqlData.Occupation , 
						logSqlData.dwExpPoint , logSqlData.dwMoney , logSqlData.Level ,
						logSqlData.Time , logSqlData.szGPCode[1] , logSqlData.szGPCode[0] );
*/


LogEVENT_QUE *LogEventQue=0;
int			LogEventQuePush;
int			LogEventQuePop;
int			LogEventLossCount = 0;
DWORD		LogEventQueProcessTime = 0;		//ó�� �ð�


/*
//1) �α��ν� 
PRISTONSQLDLL_API int SQLLoginProcess( char * lpUserID );

//2) ĳ���� ������
PRISTONSQLDLL_API int SQLAddCharProcess( char * lpUserID, char * lpCharID );

//3) ĳ���� ������
PRISTONSQLDLL_API int SQLDelCharProcess( char * lpUserID, char * lpCharID );

//4) �α׾ƿ���, Client ���� ����������
PRISTONSQLDLL_API int SQLLogoutProcess( char * lpUserID, char * lpCharID, char * lpRace, char * lpOccupation, unsigned long nExpPoint, int nLevel );

int InitLogSql();
int CloseLogSql();
int RecordLogSql( DWORD	dwRecCode , char *lpUserID , char *lpCharName , int Race , int JobCode , DWORD dwExp , DWORD dwLevel );

#define		LOG_SQL_LOGIN			0
#define		LOG_SQL_NEWCHAR			1
#define		LOG_SQL_DELCHAR			2
#define		LOG_SQL_LOGOUT			3

*/


/*
extern "C" 
{ 
__declspec(dllimport) int Hello(char *UserID); 
__declspec(dllimport) int SQLLogon(char *UserID, char *Passwd); 
__declspec(dllimport) int GetUseID(char *UserID); 
__declspec(dllimport) int SetGameUser( char *lpszPID,char *lpszCID, char *ServerName, char* pFlag );
__declspec(dllimport) void GameLog( char *lpszPID, int lpszCharType, char *lpszLID , char *lpszGPCode, char *lpszIP, GUID m_guid, int m_nServerPacket, int m_nClientPacket, int nFlag );
__declspec(dllimport) int Hacklog( char *lpszPID, int lpszCharType, char *lpszCID, char *lpszGPCode, int lpszHcode, char *lpszServerName, char * IP);
__declspec(dllimport) void CharLog( char *lpszPID, int lpszCharType, char *lpszLID , char *lpszGPCode, char *lpszIP, char *lpszServerName, int lpszGameLevel, int lpszExp, int lpszGameMoney, int nFlag );
__declspec(dllimport) int PCSearchIP(char* CIP, char *GPCode);
} 
*/

//__declspec(dllimport) int SQLLogon(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *UsePeriod,int inuse);
//__declspec(dllexport) int SQLLogout(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod,int inuse);
//__declspec(dllimport) int UserUseTime( char *lpszPID, char *lpszCID, char *lpszIP, char *ServerName, char *lpszGPCode, int lpszUseTime, char* pFlag );
//__declspec(dllimport) int PCUseTime( char *lpszPID,char *lpszCID, char *ServerName, char *lpszGPCode, int lpszUseTime, char *lpszIP, char* pFlag );
//__declspec(dllimport) int SetGameUser( char *lpszPID,char *lpszCID, char *ServerName, char* pFlag );
//__declspec(dllexport) int GetGPCode(char *GPCode,int PG);
// int IPRecord(char *lpszServerName, struct JUNGIP *JungIP);

#ifdef	BILLING_METER_RATE		//������ ���
typedef int (*LPFN_SQLLogon)(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *s_guid , char *lpszPCRNo , char *lpszGubunCode );
#else
typedef int (*LPFN_SQLLogon)(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *s_guid , char *lpszPCRNo );
#endif

typedef int (*LPFN_EventSQLLogon)(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *lpszS_guid, char *lpszPCRNo, char *lpszGubunCode);
typedef int (*LPFN_SQLLogout)(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t lpszDisuseDay, int lpszUsePeriod, char *s_guid , int PCRNo );
typedef int (*LPFN_EventSQLLogout)(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t lpszDisuseDay, long lpszUsePeriod, char *s_guid, long PCRNo, char *lpszNewGPCode, char *News_guid);

typedef int (*LPFN_UserUseTime)( char *lpszPID, char *lpszCID, char *lpszIP, char *ServerName, char *lpszGPCode, int lpszUseTime, char* pFlag );
typedef int (*LPFN_PCUseTime)( char *lpszPID,char *lpszCID, char *ServerName, char *lpszGPCode, int lpszUseTime, char *lpszIP, char* pFlag );
typedef int (*LPFN_SetGameUser)( char *lpszPID,char *lpszCID, char *ServerName, char* pFlag );
typedef int (*LPFN_GetGPCode)(char *GPCode,char *PG);
typedef int (*LPFN_PCUse)( char *lpszPID, char *lpszGPCode, char *ServerName, char *lpszIP, int lpszRNo, char *s_guid);
typedef int (*LPFN_PCSearchReserv)(char *lpszuserid, int lpszRNo, char *lpszGPCode, char *IP, char *NewRNo, char *NewGPCode, char *NewDisuseDay, char *NewUsePeriod, char *s_guid);
typedef int (*LPFN_ChangeUseridGPCodeGame)( int PCRNo, char *lpszUserID, char *lpszNewGPCOde, char *lpszNewDisUseDay, char *lpszNewUsePeriod, char *lpszNewGUID);


typedef long (*LPFN_PCRNoReservProduct)(int lpszRNo, char *lpszGPCode);


typedef int (*LPFN_CharLog)(char *lpszPID, int lpszCharType, char *lpszLID , char *lpszGPCode, char *lpszIP, char *lpszServerName, int lpszGameLevel, int lpszExp, int lpszGameMoney, int nFlag );
typedef int (*LPFN_GameLog)( char *lpszPID, int lpszCharType, char *lpszCID , char *lpszGPCode, char *lpszIP, char *m_guid, int m_nServerPacket, int m_nClientPacket, int nFlag );
typedef int (*LPFN_CharRecordLog)(char *lpszPID, int lpszCharType, char *lpszLID , char *lpszGPCode, char *lpszIP, char *lpszServerName, int lpszGameLevel, __int64 lpszExp, int lpszGameMoney, int nFlag );

typedef int (*LPFN_SetAllGameUser)();
typedef int (*LPFN_ServerDown)();
typedef int (*LPFN_ServerExit)();

typedef int (*LPFN_LogITEMRecord)( char *lpszPID, LogITEM *LogItem );
typedef int (*LPFN_LogITEMTradeRecord)( char *lpszPID, LogITEM_TRADE *LogItem );
typedef int (*LPFN_LogITEMPostRecord)(char *lpszPID, LogGiftITEM *GiftItem);

typedef int (*LPFN_IPRecord)( char *lpszServerName, JUNGIP *JungIP );
typedef int (*LPFN_IPRecordTime)( char *lpszServerName, JUNGIP *JungIP );

typedef int (*LPFN_WithSODRecordRanks)(char *lpszUserID, char *lpszCharName, int CharType, int point, int KillCount, int GLevel, int TotalPoint, int TotalUser, int SuccessUser, int PCRNo, char *lpszGPCode, char *lpszIP , long servertime );
typedef int (*LPFN_UserStar)( char *lpszPID, int lpszStar);
typedef int (*LPFN_UserStarRecord)( char *lpszPID, int lpszStar , char *GameLogonID , int CharType , int GLevel , int Gubun );
typedef int (*LPFN_Contribution)( char *lpszUserID, char *lpszCharName,  int lpszMoney);
typedef int (*LPFN_ClanMoney)( char *lpszPID, char *GameLogonID , int lpszClanMoney , int Gubun );
typedef int (*LPFN_EventJoin)( char *lpszUserID, char *lpszCharName );

typedef __int64 (*LPFN_SiegeMoney)( char *lpszPID,  char *GameLogonID, __int64 lpszSiegeMoney, int Gubun);
typedef int (*LPFN_SiegeMoneyTax)( char *lpszClanMarkNum, __int64 lpszMx, __int64 lpszAg, __int64 lpszSh, __int64 lpszPo1, __int64 lpszPo2, __int64 lpszPo3, __int64 lpszFo, __int64 lpszWp, __int64 lpszSk, __int64 lpszTt, __int64 lpszTax);
typedef	int (*LPFN_PollRecord)( char *lpszPID , time_t PollTime , struct POLL *PollItem , int Gubun );

//////////////////////////////// �ؿ��� URS ////////////////////////
typedef int (*LPFN_URSLoginIP)(const char *ID,const char *PW, const char *IP );
typedef int (*LPFN_URSLogoutIP)(const char *ID, const char *IP );
typedef int (*LPFN_URSLogoutIPBlock)(const char *ID, const char *IP , const int min );
typedef int (*LPFN_Expined)( char *szID );

/////////////////////////////////////////////////////////////////////

//////////////////////////// CLAN.DLL ////////////////////////
typedef int (*LPFN_GetTicket)();
typedef int (*LPFN_ClanUserTicket)( char *lpszuserid, char *lpszChName, char *lpszIP, int TicketNum , int gubun );
typedef int (*LPFN_ClanUserTicKets)( char *lpszuserid, char *lpszChName, char *lpszIP, int TicketNum , int gubun, int CharType, int CharLevel);
typedef int (*LPFN_ClanPointInsert)( char *lpszuserid, char *lpszChName, char * lpszIP, int TicketNum, int Cpoint );
typedef int (*LPFN_ClanUserMarkNum)( char *lpszuserid, char *lpszChName );

//////////////////////////////////////////////////////////////

//////////////////////// ExpGameTime.DLL //////////////////////
typedef int (*LPFN_GetExpGameTime)( char *lpszuserid, char *lpszChName, long GamePlayTime );
//////////////////////////////////////////////////////////////


class	BILL_SQL {

public:
	HINSTANCE hLogSqlDLL;							// Handle to DLL
	HINSTANCE hLogUrsDLL;							// Handle to DLL
	HINSTANCE hClanDLL;								// Handle to DLL
	HINSTANCE hExpGameTimeDLL;						// Handle to DLL

	LPFN_SQLLogon			SQLLogon;
	LPFN_SQLLogout			SQLLogout;
	LPFN_EventSQLLogout		EventSQLLogout;
	LPFN_EventSQLLogon		EventSQLLogon;
	LPFN_UserUseTime		UserUseTime;
	LPFN_PCUseTime			PCUseTime;
	LPFN_SetGameUser		SetGameUser;
	LPFN_GetGPCode			GetGPCode;
	LPFN_PCUse				PCUse;
	LPFN_PCSearchReserv		PCSearchReserv;
	LPFN_ChangeUseridGPCodeGame	ChangeUseridGPCodeGame;
	LPFN_PCRNoReservProduct	PCRNoReservProduct;


	LPFN_CharLog			CharLog;					// Function pointer
	LPFN_GameLog			GameLog;					// Function pointer
	LPFN_CharRecordLog		CharRecordLog;				// Function pointer

	LPFN_LogITEMRecord		LogITEMRecord;
	LPFN_LogITEMTradeRecord	LogITEMTradeRecord;
	LPFN_LogITEMPostRecord	LogITEMPostRecord;

	LPFN_IPRecord			IPRecord;
	LPFN_IPRecordTime		IPRecordTime;

	LPFN_SetAllGameUser		SetAllGameUser;
	LPFN_WithSODRecordRanks	WithSODRecordRanks;
	LPFN_UserStar			UserStar;
	LPFN_UserStarRecord		UserStarRecord;
	LPFN_Contribution		Contribution;
	LPFN_ClanMoney			ClanMoney;
	LPFN_EventJoin			EventJoin;

	LPFN_SiegeMoney			SiegeMoney;
	LPFN_SiegeMoneyTax		SiegeMoneyTax;
	LPFN_PollRecord			PollRecord;

	LPFN_ServerDown			ServerDown;
	LPFN_ServerExit			ServerExit;


	//////////////// �ؿ� //////////////////

	LPFN_URSLoginIP			URSLoginIP;
	LPFN_URSLogoutIP		URSLogoutIP;
	LPFN_URSLogoutIPBlock	URSLogoutIPBlock;
	LPFN_Expined			Expined;

	//////////////// CLAN  //////////////////

	LPFN_GetTicket			GetTicket;
	LPFN_ClanUserTicket		ClanUserTicket;
	LPFN_ClanUserTicKets	ClanUserTicKets;
	LPFN_ClanPointInsert	ClanPointInsert;
	LPFN_ClanUserMarkNum	ClanUserMarkNum;

	///////////////// ExpGameTime //////////
	LPFN_GetExpGameTime		GetExpGameTime;
	///////////////////////////////////////

	BILL_SQL();
	~BILL_SQL();
	int Init();								//�ʱ�ȭ

};

BILL_SQL::BILL_SQL()
{
	ZeroMemory( this , sizeof(BILL_SQL) );
}
BILL_SQL::~BILL_SQL()
{

}

int BILL_SQL::Init()
{

	hClanDLL = LoadLibrary("Clan");			//Clan.DLL �ε�
	if ( hClanDLL ) {
		GetTicket			=(LPFN_GetTicket)GetProcAddress(hClanDLL,"GetTicket");
		ClanUserTicket		=(LPFN_ClanUserTicket)GetProcAddress(hClanDLL,"ClanUserTicKet");
		ClanUserTicKets		=(LPFN_ClanUserTicKets)GetProcAddress(hClanDLL,"ClanUserTicKets");
		ClanPointInsert		=(LPFN_ClanPointInsert)GetProcAddress(hClanDLL,"ClanPointInsert");
		ClanUserMarkNum		=(LPFN_ClanUserMarkNum)GetProcAddress(hClanDLL,"ClanUserMarkNum");
	}

	hLogSqlDLL = LoadLibrary("SQL");		//Sql.Dll
	if (hLogSqlDLL != NULL) {
		CharLog				=(LPFN_CharLog)GetProcAddress(hLogSqlDLL,"CharLog");
		GameLog				=(LPFN_GameLog)GetProcAddress(hLogSqlDLL,"GameLog");
		CharRecordLog		=(LPFN_CharRecordLog)GetProcAddress(hLogSqlDLL,"CharRecordLog");

		SQLLogon			=(LPFN_SQLLogon)GetProcAddress(hLogSqlDLL,"SQLLogon");
		SQLLogout			=(LPFN_SQLLogout)GetProcAddress(hLogSqlDLL,"SQLLogout");
		EventSQLLogon		=(LPFN_EventSQLLogon)GetProcAddress(hLogSqlDLL,"EventSQLLogon");
		EventSQLLogout		=(LPFN_EventSQLLogout)GetProcAddress(hLogSqlDLL,"EventSQLLogout");

		UserUseTime			=(LPFN_UserUseTime)GetProcAddress(hLogSqlDLL,"UserUseTime");
		PCUseTime			=(LPFN_PCUseTime)GetProcAddress(hLogSqlDLL,"PCUseTime");
		SetGameUser			=(LPFN_SetGameUser)GetProcAddress(hLogSqlDLL,"SetGameUser");
		GetGPCode			=(LPFN_GetGPCode)GetProcAddress(hLogSqlDLL,"GetGPCode");
		PCUse				=(LPFN_PCUse)GetProcAddress(hLogSqlDLL,"PCUse");
		PCSearchReserv		=(LPFN_PCSearchReserv)GetProcAddress(hLogSqlDLL,"PCSearchReserv");
		ChangeUseridGPCodeGame =(LPFN_ChangeUseridGPCodeGame)GetProcAddress(hLogSqlDLL,"ChangeUseridGPCodeGame");

		PCRNoReservProduct	=(LPFN_PCRNoReservProduct)GetProcAddress(hLogSqlDLL,"PCRNoReservProduct");


		SetAllGameUser		=(LPFN_SetAllGameUser)GetProcAddress(hLogSqlDLL,"SetAllGameUser");
		ServerDown			=(LPFN_ServerDown)GetProcAddress(hLogSqlDLL,"ServerDown");
		ServerExit			=(LPFN_ServerExit)GetProcAddress(hLogSqlDLL,"ServerExit");

		LogITEMRecord		=(LPFN_LogITEMRecord)GetProcAddress(hLogSqlDLL,"LogITEMRecord");
		LogITEMTradeRecord	=(LPFN_LogITEMTradeRecord)GetProcAddress(hLogSqlDLL,"LogITEMTradeRecord");
		LogITEMPostRecord	=(LPFN_LogITEMPostRecord)GetProcAddress(hLogSqlDLL,"LogITEMPostRecord");

		IPRecord			=(LPFN_IPRecord)GetProcAddress(hLogSqlDLL,"IPRecord");
		IPRecordTime		=(LPFN_IPRecord)GetProcAddress(hLogSqlDLL,"IPRecordTime");
		//WithSODRecordRanks	=(LPFN_WithSODRecordRanks)GetProcAddress(hLogSqlDLL,"WithSODRecordRanks");
		WithSODRecordRanks	=(LPFN_WithSODRecordRanks)GetProcAddress(hLogSqlDLL,"WithSOD2RecordRanks");
		UserStar			=(LPFN_UserStar)GetProcAddress(hLogSqlDLL,"UserStar");
		UserStarRecord		=(LPFN_UserStarRecord)GetProcAddress(hLogSqlDLL,"UserStarRecord");
		Contribution		=(LPFN_Contribution)GetProcAddress(hLogSqlDLL,"Contribution");
		ClanMoney			=(LPFN_ClanMoney)GetProcAddress(hLogSqlDLL,"ClanMoney");
		EventJoin			=(LPFN_EventJoin)GetProcAddress(hLogSqlDLL,"EventJoin");

		SiegeMoney			=(LPFN_SiegeMoney)GetProcAddress(hLogSqlDLL,"SiegeMoney");
		SiegeMoneyTax		=(LPFN_SiegeMoneyTax)GetProcAddress(hLogSqlDLL,"SiegeMoneyTax");
		PollRecord			=(LPFN_PollRecord)GetProcAddress(hLogSqlDLL,"PollRecord");


		UseDLL_Code = LOAD_DLL_SQL;
		//return LOAD_DLL_SQL;
	}

	hLogUrsDLL = LoadLibrary("URSLogin");		//Urslogin.dll
	if (hLogUrsDLL != NULL) {
		URSLoginIP			=(LPFN_URSLoginIP)GetProcAddress(hLogUrsDLL,"URSLoginIP");
		URSLogoutIP			=(LPFN_URSLogoutIP)GetProcAddress(hLogUrsDLL,"URSLogoutIP");
		URSLogoutIPBlock	=(LPFN_URSLogoutIPBlock)GetProcAddress(hLogUrsDLL,"URSLogoutIPBlock");
		Expined				=(LPFN_Expined)GetProcAddress(hLogUrsDLL,"Expined");

		UseDLL_Code = LOAD_DLL_URS;
		//return	LOAD_DLL_URS;
	}

	hExpGameTimeDLL = LoadLibrary("ExpGameTime");			//ExpGameTime.DLL �ε�
	if ( hExpGameTimeDLL ) {
		GetExpGameTime		=(LPFN_GetExpGameTime)GetProcAddress(hExpGameTimeDLL,"GetExpGameTime");
	}


	return UseDLL_Code;
}



BILL_SQL	BillSql;

int	bSql_LogOn(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *szGuid , char *lpszPCRNo , char *lpszGubunCode )
{
	int result;

	if ( BillSql.SQLLogon ) {
		EnterCriticalSection( &cSqlSection );				//ũ��ƼĮ ���� ����

#ifdef	BILLING_METER_RATE
		result = BillSql.SQLLogon( lpszPID, lpszPWD, lpszIP, lpszGPCode, lpszDisuseDay, lpszUsePeriod, szGuid , lpszPCRNo , lpszGubunCode );
#else
		result = BillSql.SQLLogon( lpszPID, lpszPWD, lpszIP, lpszGPCode, lpszDisuseDay, lpszUsePeriod, szGuid , lpszPCRNo );
		lpszGubunCode[0] = 0;
#endif
		LeaveCriticalSection( &cSqlSection );					//ũ��ƼĮ ���� ����
		return result;
	}
	return -100;
}


int	bUrs_LoginIP(char *lpszPID, char *lpszPWD, char *lpszIP )
{
	int result;

	if ( BillSql.URSLoginIP ) {
		result = BillSql.URSLoginIP( lpszPID, lpszPWD, lpszIP );
		return result;
	}
	return -100;
}

int	bSql_ServerDown()
{
	if ( BillSql.ServerDown ) {
		BillSql.ServerDown();
		return TRUE;
	}

	return FALSE;
}

int	bSql_ServerExit()
{
	if ( BillSql.ServerExit ) {
		BillSql.ServerExit();
		return TRUE;
	}

	return FALSE;
}


//DLL �ε�
int LoadLogSqlDLL()
{

	InitializeCriticalSection( &cSqlSection );

	nSprite_Init();			//nSprite �ʱ�ȭ

	if ( BillSql.Init() ) {

		if ( BillSql.SetAllGameUser ) BillSql.SetAllGameUser();
		if ( BillSql.Expined ) UseDLL_Expined=TRUE;
		else UseDLL_Expined=FALSE;

		return TRUE;
	}
	
/*
	hLogSqlDLL = LoadLibrary("SQL");
	if (hLogSqlDLL != NULL) {
		CharLog = (LPFN_CharLog)GetProcAddress(hLogSqlDLL,"CharLog");
		GameLog = (LPFN_GameLog)GetProcAddress(hLogSqlDLL,"GameLog");

		BillSql.Init();

		if ( BillSql.SetAllGameUser ) BillSql.SetAllGameUser();

		return TRUE;
	}
*/
	return FALSE;

}



//���� ����Ÿ�� �����Ű�� ������
DWORD WINAPI LogSqlDataThreadProc( void *pInfo )
{
	HANDLE	hThread;

	sLOG_SQLDATA	logSqlData;
	sLOG_SQLDATA	logSqlData2;
	int cnt,mcnt;
	int	result;
	int	NextFlag;
	DWORD	dwTime;
	DWORD	dwProcessTime;
	int	QueSz;
	char	szPG[16];
	int		GpCode;
	DWORD	dwIdCode;
	int		MeterCode;			//������/�̺�Ʈ ����
	DWORD	dwPeriodTime2;


	hThread = GetCurrentThread();
	LoadLogSqlDLL();			//�ε� �α�SQL DLL
	Server_SqlReady = TRUE;

	while(1) {
		SuspendThread(hThread);


		for(cnt=LogSqlSendCount;cnt<LogSqlDataCount;cnt++) {
			EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����
			memcpy( &logSqlData , &LogSqlData[cnt&LOG_SQL_BUFF_MASK] , sizeof(sLOG_SQLDATA) );
			LogSqlData[cnt&LOG_SQL_BUFF_MASK].dwRecCode = -1;
			LogSqlSendCount++;
			LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

			if ( rsServerConfig.Disable_LogQue ) continue;

			dwProcessTime = GetCurrentTime();

			//������� ť�� �ִ�ġ ���
			QueSz = LogSqlDataCount-LogSqlSendCount;
			if ( QueSz>LogSqlProcessQueMax ) LogSqlProcessQueMax = QueSz;

			switch(logSqlData.dwRecCode) {
			case LOG_SQL_LOGIN:										//�α���

				EnterCriticalSection( &cSqlSection );
				if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );
				SQLLoginProcess( logSqlData.szUserID );
				if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
				LeaveCriticalSection( &cSqlSection );
				break;

			case LOG_SQL_NEWCHAR:									//ĳ������

				EnterCriticalSection( &cSqlSection );
				if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );

				SQLAddCharProcess( logSqlData.szUserID , logSqlData.szCharName );

				if ( BillSql.CharRecordLog ) {
					// ĳ���� ����
					BillSql.CharRecordLog(logSqlData.szUserID, logSqlData.Occupation , logSqlData.szCharName, logSqlData.szGPCode , logSqlData.szIP, szServerName ,logSqlData.Level,logSqlData.Exp64 ,logSqlData.dwMoney,1); 
				}
				else if ( BillSql.CharLog ) {
					// ĳ���� ����
					BillSql.CharLog(logSqlData.szUserID, logSqlData.Occupation , logSqlData.szCharName, logSqlData.szGPCode , logSqlData.szIP, szServerName ,logSqlData.Level,logSqlData.dwExpPoint,logSqlData.dwMoney,1); 	
				}

				if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
				LeaveCriticalSection( &cSqlSection );
				break;

			case LOG_SQL_DELCHAR:								//ĳ������
				EnterCriticalSection( &cSqlSection );
				if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );

				SQLDelCharProcess( logSqlData.szUserID , logSqlData.szCharName );

				if ( BillSql.CharRecordLog ) {
					// ĳ���� ����
					BillSql.CharRecordLog(logSqlData.szUserID, logSqlData.Occupation , logSqlData.szCharName, logSqlData.szGPCode , logSqlData.szIP, szServerName ,logSqlData.Level,logSqlData.Exp64 ,logSqlData.dwMoney,2); 
				}
				else if ( BillSql.CharLog ) {
					// ĳ���� ����
					BillSql.CharLog(logSqlData.szUserID, logSqlData.Occupation , logSqlData.szCharName, logSqlData.szGPCode , logSqlData.szIP, szServerName ,logSqlData.Level,logSqlData.dwExpPoint,logSqlData.dwMoney,2); 
				}

				if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
				LeaveCriticalSection( &cSqlSection );
				break;

			case LOG_SQL_LOGOUT:									//�α׾ƿ�
				EnterCriticalSection( &cSqlSection );
				if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );

				SQLLogoutProcess( logSqlData.szUserID , logSqlData.szCharName , logSqlData.szRace , 
					logSqlData.szOccupation , logSqlData.dwExpPoint , logSqlData.Level );


				if ( BillSql.CharRecordLog ) {
					// ĳ���� ����
					BillSql.CharRecordLog(logSqlData.szUserID, logSqlData.Occupation , logSqlData.szCharName, logSqlData.szGPCode , logSqlData.szIP, szServerName ,logSqlData.Level,logSqlData.Exp64 ,logSqlData.dwMoney,3); 
				}
				else if ( BillSql.CharLog ) {
					// ĳ���� ���
					BillSql.CharLog(logSqlData.szUserID, logSqlData.Occupation , logSqlData.szCharName, logSqlData.szGPCode, logSqlData.szIP, szServerName ,logSqlData.Level,logSqlData.dwExpPoint,logSqlData.dwMoney,3); 
				}

				if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
				LeaveCriticalSection( &cSqlSection );
				break;

			case GAMELOG_SQL_LOGIN:
				//���� �α� ��
/*
				EnterCriticalSection( &cSqlSection );
				if ( GameLog ) {
					GameLog( logSqlData.szUserID, logSqlData.Occupation , logSqlData.szCharName , logSqlData.szGPCode , logSqlData.szIP, logSqlData.szGuid , 0, 0 , 1 );
				}
				LeaveCriticalSection( &cSqlSection );
*/
				break;
			case GAMELOG_SQL_LOGOUT:
				//���� �α� �ƿ�
/*
				EnterCriticalSection( &cSqlSection );
				if ( GameLog ) {
					GameLog( logSqlData.szUserID, logSqlData.Occupation , logSqlData.szCharName , logSqlData.szGPCode , logSqlData.szIP, logSqlData.szGuid , 0, 0 , 2 );
				}
				LeaveCriticalSection( &cSqlSection );
*/
				if ( BillSql.URSLogoutIPBlock ) {
					//�߱� �α׾ƿ� , ���� �� ���
					BillSql.URSLogoutIPBlock( logSqlData.szUserID, logSqlData.szIP , logSqlData.Time );
				}
				else {
					if ( BillSql.URSLogoutIP ) {
						//�߱� �α׾ƿ�
						BillSql.URSLogoutIP( logSqlData.szUserID, logSqlData.szIP );
					}
				}

				break;

			case BILLING_SQL_LOGOUT:

				if ( BillSql.SQLLogout ) {
					EnterCriticalSection( &cSqlSection );
					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );

					BillSql.SQLLogout( logSqlData.szUserID , logSqlData.szCharName , logSqlData.szIP , logSqlData.szGPCode ,
						logSqlData.DisuseDay , logSqlData.UsePeriod , logSqlData.szGuid , logSqlData.PC_RNo );

					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
					LeaveCriticalSection( &cSqlSection );
				}
				break;

			case BILLING_SQL_EVENT_LOGOUT:

				if ( BillSql.EventSQLLogout ) {
					EnterCriticalSection( &cSqlSection );

					result = BillSql.EventSQLLogout( 
						logSqlData.szUserID , logSqlData.szCharName , logSqlData.szIP , logSqlData.szGPCode ,
						logSqlData.DisuseDay , logSqlData.UsePeriod , logSqlData.szGuid , logSqlData.PC_RNo , 
						logSqlData2.szGPCode , logSqlData2.szGuid );

					if ( result==1 && logSqlData.lpPlayInfo && logSqlData.lpPlayInfo->lpsmSock && logSqlData.lpPlayInfo->dwConnectCount==logSqlData.dwConnectCode ) {

						GpCode = bSql_GetGPCode( logSqlData2.szGPCode, szPG );
						logSqlData.lpPlayInfo->GpType = atoi( szPG );
						logSqlData.lpPlayInfo->Bl_RNo = 0;

						lstrcpy( logSqlData.lpPlayInfo->szGPCode , logSqlData2.szGPCode );
						lstrcpy( logSqlData.lpPlayInfo->szGuid , logSqlData2.szGuid );
					}
					LeaveCriticalSection( &cSqlSection );

					//ť�� �α׾ƿ��� ���� �ִ��� Ȯ���Ͽ� �ڵ� ����
					if ( result==1 ) {
						EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����
						for(cnt=LogSqlSendCount;cnt<LogSqlDataCount;cnt++) {
							mcnt = cnt&LOG_SQL_BUFF_MASK;
							if ( LogSqlData[mcnt].dwRecCode==BILLING_SQL_LOGOUT ) {
								if ( lstrcmpi( LogSqlData[mcnt].szUserID , logSqlData.szUserID )==0 ) {
									lstrcpy( LogSqlData[mcnt].szGuid , logSqlData2.szGuid );
									lstrcpy( LogSqlData[mcnt].szGPCode , logSqlData2.szGPCode );
									break;
								}
							}
						}
						LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����
					}
				}
				break;


			case BILLING_SQL_PCUSE:
				if ( BillSql.PCUse ) {
					EnterCriticalSection( &cSqlSection );
					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );
					result = BillSql.PCUse( logSqlData.szUserID , logSqlData.szGPCode , rsServerConfig.szServerName , logSqlData.szIP ,logSqlData.PC_RNo, logSqlData.szGuid );
					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
					LeaveCriticalSection( &cSqlSection );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
					//��� �� ����
					if ( logSqlData.lpPlayInfo && logSqlData.lpPlayInfo->lpsmSock && logSqlData.lpPlayInfo->dwConnectCount==logSqlData.dwConnectCode ) {

						if ( (result&0xFF000000)==0x40000000 ) {		//����ð� ��������
							dwPeriodTime2 = result&0x00FFFFFF;
							logSqlData.lpPlayInfo->dwPeriodTime2 = (DWORD)tServerTime+dwPeriodTime2;
						}
						else {
							if ( logSqlData.lpPlayInfo->DisuseDay ) {
								logSqlData.lpPlayInfo->DisuseDay = result;
							}
							else {
								dwTime = ( DWORD )tServerTime + result;
								logSqlData.lpPlayInfo->dwPeriodTime = dwTime;
							}
						}

//						dwTime = tServerTime+2*60;
//						if ( result<2*60 && ( !logSqlData.lpPlayInfo->dwPeriodTime || logSqlData.lpPlayInfo->dwPeriodTime>dwTime ) ) {
//							//2�� �Ŀ� ���� ��Ŵ
//							logSqlData.lpPlayInfo->dwPeriodTime = dwTime;
//						}
					}
					LeaveCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
				}
				break;

			case BILLING_URS_EXPINED:
				if ( BillSql.Expined ) {
					//�븸 ����
					EnterCriticalSection( &cSqlSection );
					result = BillSql.Expined( logSqlData.szUserID );
					LeaveCriticalSection( &cSqlSection );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
					if ( logSqlData.lpPlayInfo && logSqlData.lpPlayInfo->lpsmSock && logSqlData.lpPlayInfo->dwConnectCount==logSqlData.dwConnectCode ) {
						if ( result ) {
							dwTime = ( DWORD )tServerTime + result * 60;
							logSqlData.lpPlayInfo->dwPeriodTime = dwTime;
						}
						else
							logSqlData.lpPlayInfo->dwPeriodTime = 0;
					}
					LeaveCriticalSection( &cSerSection );
				}
				break;

			case BILLING_SQL_PC_SRESERV:
				if ( BillSql.PCSearchReserv ) {

					EnterCriticalSection( &cSqlSection );
					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );

					result = BillSql.PCSearchReserv( logSqlData.szUserID , logSqlData.PC_RNo , logSqlData.szGPCode , logSqlData.szIP ,
						(char *)&logSqlData2.PC_RNo , logSqlData2.szGPCode , (char *)&logSqlData2.DisuseDay , (char *)&logSqlData2.UsePeriod , logSqlData.szGuid );

					if ( (result&0xFF000000)==0x40000000 ) {		//����ð� ��������
						dwPeriodTime2 = result&0x00FFFFFF;
						result = 1;
					}
					else
						dwPeriodTime2 = 0;


					if ( result==1 ) GpCode = bSql_GetGPCode( logSqlData2.szGPCode, szPG );

					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
					LeaveCriticalSection( &cSqlSection );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����

					//��� ���� ( ���� ���� )
					if ( result==1 && logSqlData.lpPlayInfo && logSqlData.lpPlayInfo->lpsmSock && logSqlData.lpPlayInfo->dwConnectCount==logSqlData.dwConnectCode ) {

						logSqlData.lpPlayInfo->GpCode = GpCode;

						if ( logSqlData.lpPlayInfo->GpCode>=0 ) {
							lstrcpy( logSqlData.lpPlayInfo->szGPCode , logSqlData2.szGPCode );
							logSqlData.lpPlayInfo->DisuseDay = logSqlData2.DisuseDay;
							logSqlData.lpPlayInfo->UsePeriod = logSqlData2.UsePeriod;
							logSqlData.lpPlayInfo->GpType = atoi( szPG );
							logSqlData.lpPlayInfo->Bl_RNo = logSqlData2.PC_RNo;

							if ( logSqlData.lpPlayInfo->UsePeriod ) {
								logSqlData.lpPlayInfo->dwPeriodTime = ( DWORD )tServerTime + logSqlData.lpPlayInfo->UsePeriod;
							}
							else {
								logSqlData.lpPlayInfo->dwPeriodTime = 0;
							}

							if ( dwPeriodTime2 )		//������ ����ð�
								logSqlData.lpPlayInfo->dwPeriodTime2 = ( DWORD )tServerTime + dwPeriodTime2;
							else
								logSqlData.lpPlayInfo->dwPeriodTime2 = 0;

						}

						if ( logSqlData.lpPlayInfo->Bl_RNo>0 )
							bSql_PCRNoReservProduct( logSqlData.lpPlayInfo );
						else
							srDisplayBillingTime( logSqlData.lpPlayInfo , 0 );		//����� �ð� ǥ��

					}
					LeaveCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
				}
				break;

			case BILLING_SQL_BILL_CHANGE:
				//���� ����
				if ( BillSql.ChangeUseridGPCodeGame ) {

					EnterCriticalSection( &cSqlSection );
					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );

					result = BillSql.ChangeUseridGPCodeGame( logSqlData.PC_RNo , logSqlData2.szUserID , logSqlData2.szGPCode , 
						(char *)&logSqlData2.DisuseDay , (char *)&logSqlData2.UsePeriod , logSqlData.szGuid );

					if ( result==1 ) GpCode = bSql_GetGPCode( logSqlData2.szGPCode, szPG );

					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
					LeaveCriticalSection( &cSqlSection );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����

					//��� ���� ( ���� ���� )
					if ( result==1 ) {
						dwIdCode = GetSpeedSum( logSqlData2.szUserID );
						for(cnt=0;cnt<CONNECTMAX;cnt++) {
							if ( rsPlayInfo[cnt].lpsmSock && rsPlayInfo[cnt].szID[0] && rsPlayInfo[cnt].dwCode_ID==dwIdCode ) {
								if ( lstrcmpi( logSqlData2.szUserID , rsPlayInfo[cnt].szID)==0 ) {

									rsPlayInfo[cnt].GpCode = GpCode;
									if ( rsPlayInfo[cnt].GpCode>=0 ) {

										lstrcpy( rsPlayInfo[cnt].szGPCode , logSqlData2.szGPCode );
										lstrcpy( rsPlayInfo[cnt].szGuid , logSqlData.szGuid );
										rsPlayInfo[cnt].DisuseDay = logSqlData2.DisuseDay;
										rsPlayInfo[cnt].UsePeriod = logSqlData2.UsePeriod;
										rsPlayInfo[cnt].GpType = atoi( szPG );

										if ( rsPlayInfo[cnt].UsePeriod ) {
											rsPlayInfo[ cnt ].dwPeriodTime = ( DWORD )tServerTime + logSqlData.lpPlayInfo->UsePeriod;
										}
										else {
											rsPlayInfo[cnt].dwPeriodTime = 0;
										}
									}

									srDisplayBillingTime( &rsPlayInfo[cnt] , 0 );		//����� �ð� ǥ��
									break;
								}
							}
						}
					}
					LeaveCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
				}
				break;

			case BILLING_SQL_EVENT_LOGIN:
				//�̺�Ʈ �α���
				NextFlag = 0;
				if ( BillSql.EventSQLLogon ) {

					EnterCriticalSection( &cSqlSection );
					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );

					result = BillSql.EventSQLLogon( logSqlData.szUserID , "*FreeLevel" , logSqlData.szIP ,
						logSqlData2.szGPCode , (char *)&logSqlData2.DisuseDay , (char *)&logSqlData2.UsePeriod , 
						logSqlData2.szGuid , (char *)&logSqlData2.PC_RNo , (char *)&MeterCode );

					if ( result==1 ) GpCode = bSql_GetGPCode( logSqlData2.szGPCode, szPG );

					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
					LeaveCriticalSection( &cSqlSection );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����


					//��� ���� ( ���� ���� )
					if ( logSqlData.lpPlayInfo && logSqlData.lpPlayInfo->lpsmSock && logSqlData.lpPlayInfo->dwConnectCount==logSqlData.dwConnectCode ) {
						if ( result>=1 ) {

							logSqlData.lpPlayInfo->GpCode = GpCode;

							if ( logSqlData.lpPlayInfo->GpCode>=0 ) {
								lstrcpy( logSqlData.lpPlayInfo->szGPCode , logSqlData2.szGPCode );
								lstrcpy( logSqlData.lpPlayInfo->szGuid  , logSqlData2.szGuid );

								logSqlData.lpPlayInfo->DisuseDay = logSqlData2.DisuseDay;
								logSqlData.lpPlayInfo->UsePeriod = logSqlData2.UsePeriod;
								logSqlData.lpPlayInfo->GpType = atoi( szPG );
								logSqlData.lpPlayInfo->Bl_RNo = logSqlData2.PC_RNo;

								if ( logSqlData.lpPlayInfo->UsePeriod ) {
									logSqlData.lpPlayInfo->dwPeriodTime = ( DWORD )tServerTime + logSqlData.lpPlayInfo->UsePeriod;
								}
								else {
									logSqlData.lpPlayInfo->dwPeriodTime = 0;
								}
								logSqlData.lpPlayInfo->Bl_Meter = MeterCode;

								//�̺�Ʈ �α׿� ����ó��
								rsEventLogOn_Sucess( logSqlData.lpPlayInfo );
							}

							if ( logSqlData.lpPlayInfo->Bl_RNo>0 ) {
								//PC�� �ѹ��� Ȯ���Ҽ� �ֵ���...
								logSqlData.PC_RNo = logSqlData2.PC_RNo;
								lstrcpy( logSqlData.szGPCode , logSqlData2.szGPCode );
								NextFlag = TRUE;
							}
							else {
								srDisplayBillingTime( logSqlData.lpPlayInfo , 0 );		//����� �ð� ǥ��
							}
						}
						else {
							logSqlData.lpPlayInfo->DisuseDay = tServerTime+60*5;		//5��
							logSqlData.lpPlayInfo->UsePeriod = 0;
							logSqlData.lpPlayInfo->dwPeriodTime = 0;
							logSqlData.lpPlayInfo->Bl_Meter = 0;
							srDisplayBillingTime( logSqlData.lpPlayInfo , 0 );			//����� �ð� ǥ��
						}
					}

					LeaveCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����

				}
				if ( NextFlag==0 ) break;

			case BILLING_SQL_PC_TIME:
				if ( BillSql.PCRNoReservProduct ) {

					EnterCriticalSection( &cSqlSection );
					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 1000+logSqlData.dwRecCode , dwPlayServTime );

					result = BillSql.PCRNoReservProduct( logSqlData.PC_RNo , logSqlData.szGPCode );

					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
					LeaveCriticalSection( &cSqlSection );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
					if ( logSqlData.lpPlayInfo && logSqlData.lpPlayInfo->lpsmSock && logSqlData.lpPlayInfo->dwConnectCount==logSqlData.dwConnectCode ) {
						srDisplayBillingTime( logSqlData.lpPlayInfo , result );			//�ð� ǥ��
					}
					LeaveCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
				}
				break;

			//��Ʈ�� ���ӽð��� ����ġ ȹ��
			case EGT_GET_EXPGAMETIME:

				if ( BillSql.GetExpGameTime ) {

					EnterCriticalSection( &cSqlSection );
					result = BillSql.GetExpGameTime( logSqlData.szUserID , logSqlData.szCharName , logSqlData.Time );
					LeaveCriticalSection( &cSqlSection );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
					if ( logSqlData.lpPlayInfo && logSqlData.lpPlayInfo->lpsmSock && logSqlData.lpPlayInfo->dwConnectCount==logSqlData.dwConnectCode ) 
					{
						/*
						//���ð� ��Ͽ�
						if( result == -1 )// _LANGUAGE_CHINESE , result���� -1�̸� �߱����� �ð����� ����� �ƴ� //�ؿ�
						{
							logSqlData.lpPlayInfo->dwExpGameTimeCheck = dwPlayServTime;
							logSqlData.lpPlayInfo->dwExpGameTime = 0;	// �̰��� 0���� ����� �־�� ����ġ�� �������� ���������� ��ӵȴ�
						}
						else*/
						if ( logSqlData.Time==0 && result != -1) 
						{ 
							logSqlData.lpPlayInfo->dwExpGameTimeCheck = dwPlayServTime;
							logSqlData.lpPlayInfo->dwExpGameTime = result*1000;
						}
					}
					LeaveCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
				}
				break;

/*
			case GAMELOG_SQL_SOD:
				//SoD ��� ����
				if ( BillSql.WithSODRecord ) {
					BillSql.WithSODRecord( logSqlData.szUserID , logSqlData.szCharName , logSqlData.Occupation , 
						logSqlData.dwExpPoint , logSqlData.dwMoney , logSqlData.Level ,
						logSqlData.Time , logSqlData.szGPCode[1] , logSqlData.szGPCode[0] );
				}
				break;
*/
			}


			//ó�� �ð� ���
			dwProcessTime = GetCurrentTime()-dwProcessTime;
			if ( dwProcessTime ) {
				if ( LogSqlProcessTimeMax<dwProcessTime ) LogSqlProcessTimeMax = dwProcessTime;
				LogSqlProcessTime += dwProcessTime;
				LogSqlProcessCount++;
			}

		}
	}

	ExitThread( TRUE );
	return TRUE;
}






int InitLogSql()
{
	LogSqlData = new sLOG_SQLDATA[LOG_SQL_BUFF_MAX];
	LogSqlDataCount = 0;
	LogSqlSendCount = 0;

	LogItemQue = new LogITEM_QUE[LOG_ITEM_QUE_MAX];
	LogItemQuePush = 0;
	LogItemQuePop = 0;

	LogEventQue = new LogEVENT_QUE[LOG_EVENT_QUE_MAX];
	LogEventQuePush = 0;
	LogEventQuePop = 0;



	//ũ��ƼĮ ���� �ʱ�ȭ
	InitializeCriticalSection( &cLogSqlSection );

	hLogSqlThread = CreateThread( NULL , 0, LogSqlDataThreadProc , 0 , 0, &dwLogSqlThreadId );

	if ( rsServerConfig.szServerName[0] ) {
		lstrcpy( szServerName , rsServerConfig.szServerName );
	}

	return TRUE;
}

int CloseLogSql()
{
	if ( LogEventQue ) {
		delete LogEventQue;
		LogEventQue = 0;
	}

	if ( LogItemQue ) {
		delete LogItemQue;
		LogItemQue = 0;
	}

	if ( LogSqlData ) {
		delete LogSqlData;
		LogSqlData = 0;
	}


	return TRUE;
}

int RecordLogSql( DWORD	dwRecCode , char *szIP , char *lpUserID , char *lpCharName , char *szGPCode, int Race , int JobCode , INT64 Exp64 , DWORD dwLevel , DWORD dwMoney )
{

	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {		//���� ����
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = dwRecCode;

	if ( lpUserID )	lstrcpy( LogSqlData[cnt].szUserID , lpUserID );
	else LogSqlData[cnt].szUserID[0] = 0;

	if ( lpCharName )	lstrcpy( LogSqlData[cnt].szCharName , lpCharName );
	else LogSqlData[cnt].szCharName[0] = 0;

	LogSqlData[cnt].szOccupation[0] = 0;
	LogSqlData[cnt].Occupation = 0;

	switch( JobCode ) 
	{
	case 1:
		lstrcpy( LogSqlData[cnt].szOccupation , "Fighter" );
		LogSqlData[cnt].Occupation = 2;
		break;
	case 2:
		lstrcpy( LogSqlData[cnt].szOccupation , "Mechanician" );
		LogSqlData[cnt].Occupation = 1;
		break;
	case 3:
		lstrcpy( LogSqlData[cnt].szOccupation , "Archer" );
		LogSqlData[cnt].Occupation = 4;
		break;
	case 4:
		lstrcpy( LogSqlData[cnt].szOccupation , "Pikeman" );
		LogSqlData[cnt].Occupation = 3;
		break;
	case 5:
		lstrcpy( LogSqlData[cnt].szOccupation , "Atalanta" );
		LogSqlData[cnt].Occupation = 7;
		break;
	case 6:
		lstrcpy( LogSqlData[cnt].szOccupation , "Knight" );
		LogSqlData[cnt].Occupation = 6;
		break;
	case 7:
		lstrcpy( LogSqlData[cnt].szOccupation , "Magician" );
		LogSqlData[cnt].Occupation = 5;
		break;
	case 8:
		lstrcpy( LogSqlData[cnt].szOccupation , "Priestess" );
		LogSqlData[cnt].Occupation = 8;
		break;
	}


	LogSqlData[cnt].szRace[0] = 0;

	if ( JobCode <= 4 || JobCode == 9)
		lstrcpy( LogSqlData[cnt].szRace , "Tempskron" );
	else
		lstrcpy( LogSqlData[cnt].szRace , "Moryon" );

	lstrcpy( LogSqlData[cnt].szIP , szIP );

	if ( szGPCode )	lstrcpy( LogSqlData[cnt].szGPCode , szGPCode );
	else LogSqlData[cnt].szGPCode[0] = 0;



	LogSqlData[cnt].dwMoney  = dwMoney;
	LogSqlData[cnt].dwExpPoint = (DWORD)Exp64;
	LogSqlData[cnt].Exp64 = Exp64;
	LogSqlData[cnt].Level = dwLevel;

	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );

	return TRUE;
}


int RecordGameLogSql( DWORD	dwRecCode , char *szIP , char *lpUserID , char *lpCharName  , char *szGPCode, int JobCode , char *szGuid , int BlockTime )
{

	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = dwRecCode;
	lstrcpy( LogSqlData[cnt].szIP , szIP );

	if ( lpUserID )	lstrcpy( LogSqlData[cnt].szUserID , lpUserID );
	else LogSqlData[cnt].szUserID[0] = 0;

	if ( lpCharName )	lstrcpy( LogSqlData[cnt].szCharName , lpCharName );
	else LogSqlData[cnt].szCharName[0] = 0;

	LogSqlData[cnt].szOccupation[0] = 0;
	LogSqlData[cnt].Occupation = 0;

	LogSqlData[cnt].Time = BlockTime;

	switch( JobCode ) {
	case 1:
		lstrcpy( LogSqlData[cnt].szOccupation , "Fighter" );
		LogSqlData[cnt].Occupation = 2;
		break;
	case 2:
		lstrcpy( LogSqlData[cnt].szOccupation , "Mechanician" );
		LogSqlData[cnt].Occupation = 1;
		break;
	case 3:
		lstrcpy( LogSqlData[cnt].szOccupation , "Archer" );
		LogSqlData[cnt].Occupation = 4;
		break;
	case 4:
		lstrcpy( LogSqlData[cnt].szOccupation , "Pikeman" );
		LogSqlData[cnt].Occupation = 3;
		break;
	case 5:
		lstrcpy( LogSqlData[cnt].szOccupation , "Atalanta" );
		LogSqlData[cnt].Occupation = 7;
		break;
	case 6:
		lstrcpy( LogSqlData[cnt].szOccupation , "Knight" );
		LogSqlData[cnt].Occupation = 6;
		break;
	case 7:
		lstrcpy( LogSqlData[cnt].szOccupation , "Magician" );
		LogSqlData[cnt].Occupation = 5;
		break;
	case 8:
		lstrcpy( LogSqlData[cnt].szOccupation , "Priestess" );
		LogSqlData[cnt].Occupation = 8;
		break;
	}


	LogSqlData[cnt].szRace[0] = 0;

	if ( JobCode <=4 || JobCode == 9)
		lstrcpy( LogSqlData[cnt].szRace , "Tempskron" );
	else
		lstrcpy( LogSqlData[cnt].szRace , "Moryon" );


	if ( szGuid )  lstrcpy( LogSqlData[cnt].szGuid , szGuid );
	else  LogSqlData[cnt].szGuid[0] = 0;

	if ( szGPCode ) lstrcpy( LogSqlData[cnt].szGPCode , szGPCode );
	else LogSqlData[cnt].szGPCode[0] = 0;


	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );

	return TRUE;
}

/*
int	bSql_LogOn(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *UsePeriod,int inuse)
{
	return BillSql.SQLLogon( lpszPID, lpszPWD,,lpszIP,lpszGPCode,lpszDisuseDay,UsePeriod,inuse);
}
*/
/*
	char	szGPCode[16];
	t_time	DisuseDay;
	int		UsePeroid;
*/

//typedef int (*LPFN_SQLLogon)(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *s_guid);
//typedef int (*LPFN_SQLLogout)(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t lpszDisuseDay, int lpszUsePeriod, char *s_guid);

int	bSql_Logout(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t DisuseDay, int UsePeriod, char *szGuid , int PCRNo)
{
	//return BillSql.SQLLogon( lpszPID, lpszPWD,,lpszIP,lpszGPCode,lpszDisuseDay,UsePeriod,inuse);
	//-1 0 
	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_MASK ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = BILLING_SQL_LOGOUT;
	lstrcpy( LogSqlData[cnt].szIP , lpszIP );

	if ( lpszPID )	lstrcpy( LogSqlData[cnt].szUserID , lpszPID );
	else LogSqlData[cnt].szUserID[0] = 0;

	if ( lpszCID )	lstrcpy( LogSqlData[cnt].szCharName , lpszCID );
	else LogSqlData[cnt].szCharName[0]=0;

	LogSqlData[cnt].DisuseDay = DisuseDay;
	LogSqlData[cnt].UsePeriod = UsePeriod;

	if ( szGuid ) lstrcpy( LogSqlData[cnt].szGuid , szGuid );
	else LogSqlData[cnt].szGuid[0] = 0;

	if ( lpszGPCode ) lstrcpy( LogSqlData[cnt].szGPCode , lpszGPCode );
	else LogSqlData[cnt].szGPCode[0] = 0;

	LogSqlData[cnt].PC_RNo = PCRNo;

	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );

	return TRUE;

}

//������� ť�� �ش� ������ �α� �ƿ��� �ִ��� Ȯ��
int	bSql_GetQueLogOut( char *szID )
{
	int cnt,mcnt;
	int Flag = 0;

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����
	for(cnt=LogSqlSendCount;cnt<LogSqlDataCount;cnt++) {
		mcnt = cnt&LOG_SQL_BUFF_MASK;
		if ( LogSqlData[mcnt].dwRecCode==BILLING_SQL_LOGOUT ) {
			if ( lstrcmpi( LogSqlData[mcnt].szUserID , szID )==0 ) {
				Flag = TRUE;
				break;
			}
		}
	}
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	return Flag;
}

int	bSql_GetGPCode(char *GPCode, char *PG)
{
	if ( BillSql.GetGPCode ) {
		return BillSql.GetGPCode(GPCode, PG);
	}
	return -1;
}

//int	bSql_PCUse( rsPLAYINFO *lpPlayInfo ,  char *lpszPID, char *lpszGPCode, char *ServerName, char *lpszIP, int lpszRNo)

int	bSql_PCUse( rsPLAYINFO *lpPlayInfo )
{
	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = BILLING_SQL_PCUSE;

	lstrcpy( LogSqlData[cnt].szUserID , lpPlayInfo->szID  );
	lstrcpy( LogSqlData[cnt].szGPCode , lpPlayInfo->szGPCode );
	lstrcpy( LogSqlData[cnt].szIP , lpPlayInfo->lpsmSock->szIPAddr );
	lstrcpy( LogSqlData[cnt].szGuid , lpPlayInfo->szGuid );

	LogSqlData[cnt].PC_RNo = lpPlayInfo->Bl_RNo;

	//�ش� ���� ������ ���� ( ����� �뺸 �ޱ� ���� )
	LogSqlData[cnt].lpPlayInfo = lpPlayInfo;
	LogSqlData[cnt].dwConnectCode = lpPlayInfo->dwConnectCount;


	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );


	return TRUE;
}

int	bUrs_Expined( rsPLAYINFO *lpPlayInfo )
{
	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = BILLING_URS_EXPINED;

	lstrcpy( LogSqlData[cnt].szUserID , lpPlayInfo->szID  );
	lstrcpy( LogSqlData[cnt].szIP , lpPlayInfo->lpsmSock->szIPAddr );

	//�ش� ���� ������ ���� ( ����� �뺸 �ޱ� ���� )
	LogSqlData[cnt].lpPlayInfo = lpPlayInfo;
	LogSqlData[cnt].dwConnectCode = lpPlayInfo->dwConnectCount;


	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );


	return TRUE;
}



//���� ���� Ȯ��
int	bSql_PCSearchReserv( rsPLAYINFO *lpPlayInfo )
{
	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = BILLING_SQL_PC_SRESERV;

	lstrcpy( LogSqlData[cnt].szUserID , lpPlayInfo->szID  );
	lstrcpy( LogSqlData[cnt].szGPCode , lpPlayInfo->szGPCode );
	lstrcpy( LogSqlData[cnt].szIP , lpPlayInfo->lpsmSock->szIPAddr );
	lstrcpy( LogSqlData[cnt].szGuid , lpPlayInfo->szGuid );

	LogSqlData[cnt].PC_RNo = lpPlayInfo->Bl_RNo;

	//�ش� ���� ������ ���� ( ����� �뺸 �ޱ� ���� )
	LogSqlData[cnt].lpPlayInfo = lpPlayInfo;
	LogSqlData[cnt].dwConnectCode = lpPlayInfo->dwConnectCount;


	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );

	return TRUE;
}

//���� ����
int	bSql_ChangeUserIdGpCodeGame( int PCRNo )
{
	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = BILLING_SQL_BILL_CHANGE;

	LogSqlData[cnt].PC_RNo = PCRNo;

	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );

	return TRUE;
}

//�̺�Ʈ ����
int	bSql_EventLogon( rsPLAYINFO *lpPlayInfo )
{
	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = BILLING_SQL_EVENT_LOGIN;

	lstrcpy( LogSqlData[cnt].szUserID , lpPlayInfo->szID  );
	lstrcpy( LogSqlData[cnt].szIP , lpPlayInfo->lpsmSock->szIPAddr );

	//�ش� ���� ������ ���� ( ����� �뺸 �ޱ� ���� )
	LogSqlData[cnt].lpPlayInfo = lpPlayInfo;
	LogSqlData[cnt].dwConnectCode = lpPlayInfo->dwConnectCount;

	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );

	return TRUE;
}

//�̺�Ʈ �α׾ƿ�
int	bSql_EventLogOut( rsPLAYINFO *lpPlayInfo )
{
	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = BILLING_SQL_EVENT_LOGOUT;
	lstrcpy( LogSqlData[cnt].szIP , lpPlayInfo->lpsmSock->szIPAddr );

	lstrcpy( LogSqlData[cnt].szUserID , lpPlayInfo->szID );
	lstrcpy( LogSqlData[cnt].szCharName , lpPlayInfo->szName );
	lstrcpy( LogSqlData[cnt].szGuid , lpPlayInfo->szGuid );
	lstrcpy( LogSqlData[cnt].szGPCode , lpPlayInfo->szGPCode );

	LogSqlData[cnt].DisuseDay = lpPlayInfo->DisuseDay;
	LogSqlData[cnt].UsePeriod = lpPlayInfo->UsePeriod;
	LogSqlData[cnt].PC_RNo = lpPlayInfo->Bl_RNo;

	//�ش� ���� ������ ���� ( ����� �뺸 �ޱ� ���� )
	LogSqlData[cnt].lpPlayInfo = lpPlayInfo;
	LogSqlData[cnt].dwConnectCode = lpPlayInfo->dwConnectCount;

	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );

	return TRUE;
}


//ExpGameTime ��Ϲ� ����
int	bSql_GetExpGameTime( rsPLAYINFO *lpPlayInfo , int GamePlayTime )
{
	int cnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = EGT_GET_EXPGAMETIME;

	lstrcpy( LogSqlData[cnt].szUserID , lpPlayInfo->szUserExpTimeID );
	lstrcpy( LogSqlData[cnt].szCharName , lpPlayInfo->szUserExpTimeName );

	LogSqlData[cnt].Time = GamePlayTime;

	//�ش� ���� ������ ���� ( ����� �뺸 �ޱ� ���� )
	LogSqlData[cnt].lpPlayInfo = lpPlayInfo;
	LogSqlData[cnt].dwConnectCode = lpPlayInfo->dwConnectCount;


	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );

	return TRUE;
}



//PC�� �߰� �ð� ǥ��
int	bSql_PCRNoReservProduct( rsPLAYINFO *lpPlayInfo )	
{
	int cnt;
	int mcnt;

	if ( (LogSqlDataCount-LogSqlSendCount)>=LOG_SQL_BUFF_LIMIT ) {
		LogSqlLossCount++;
		return FALSE;
	}

	EnterCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	for(cnt=LogSqlSendCount;cnt<LogSqlDataCount;cnt++) {
		mcnt = cnt&LOG_SQL_BUFF_MASK;
		if ( LogSqlData[mcnt].dwRecCode==BILLING_SQL_PC_TIME && LogSqlData[mcnt].lpPlayInfo==lpPlayInfo ) {
			LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����
			return FALSE;
		}
	}

	cnt = LogSqlDataCount & LOG_SQL_BUFF_MASK;

	LogSqlData[cnt].dwRecCode = BILLING_SQL_PC_TIME;

	lstrcpy( LogSqlData[cnt].szUserID , lpPlayInfo->szID  );
	lstrcpy( LogSqlData[cnt].szGPCode , lpPlayInfo->szGPCode );
	lstrcpy( LogSqlData[cnt].szIP , lpPlayInfo->lpsmSock->szIPAddr );

	LogSqlData[cnt].PC_RNo = lpPlayInfo->Bl_RNo;

	//�ش� ���� ������ ���� ( ����� �뺸 �ޱ� ���� )
	LogSqlData[cnt].lpPlayInfo = lpPlayInfo;
	LogSqlData[cnt].dwConnectCode = lpPlayInfo->dwConnectCount;


	LogSqlDataCount ++;
	LeaveCriticalSection( &cLogSqlSection );				//ũ��ƼĮ ���� ����

	ResumeThread( hLogSqlThread );

	return TRUE;
}

//SOD ���� ���
int	bSql_SODRecord( rsPLAYINFO *lpPlayInfo , int Level , int SodScore , int SodKillCount , int SodTotalScore , int SodTotalSucessUser , int SodTotalUser , int SodCode )
{
	sLOG_EVENT sEventData;

	lpPlayInfo->szID[31] = 0;
	lpPlayInfo->szName[31] = 0;
	lpPlayInfo->szGPCode[15] = 0;

	lstrcpy( sEventData.szID , lpPlayInfo->szID );
	lstrcpy( sEventData.szName , lpPlayInfo->szName );
	lstrcpy( sEventData.szGPCode , lpPlayInfo->szGPCode );
	lstrcpy( sEventData.szIP , lpPlayInfo->lpsmSock->szIPAddr );

	sEventData.Param[0] = lpPlayInfo->smCharInfo.JOB_CODE;
	sEventData.Param[1] = SodScore;
	sEventData.Param[2] = SodKillCount;
	sEventData.Param[3] = Level;
	sEventData.Param[4] = SodTotalScore;
	sEventData.Param[5] = SodTotalUser;
	sEventData.Param[6] = SodTotalSucessUser;
	sEventData.Param[7] = lpPlayInfo->Bl_RNo;
	sEventData.Param[8] = SodCode;

	//�̺�Ʈ �α� ť�� ����Ÿ ����
	return b_PushLogEventQue( GAMELOG_SQL_SOD , sizeof(sLOG_EVENT) , &sEventData );
}

//Star Point ��Ϲ� ����
int	bSql_StarPoint( rsPLAYINFO *lpPlayInfo , int StarPoint , int CashFlag )
{
	sLOG_EVENT sEventData;

	if ( !BillSql.UserStar && !BillSql.UserStarRecord ) return FALSE;

	ZeroMemory( &sEventData , sizeof(sLOG_EVENT) );

	lpPlayInfo->szID[31] = 0;
	lpPlayInfo->szName[31] = 0;
	lpPlayInfo->szGPCode[15] = 0;

	lstrcpy( sEventData.szID , lpPlayInfo->szID );
	lstrcpy( sEventData.szName , lpPlayInfo->szName );
	lstrcpy( sEventData.szGPCode , lpPlayInfo->szGPCode );
	lstrcpy( sEventData.szIP , lpPlayInfo->lpsmSock->szIPAddr );

	sEventData.Param[0] = StarPoint;
	sEventData.Param[1] = (DWORD)lpPlayInfo;
	sEventData.Param[2] = lpPlayInfo->dwConnectCount;

	sEventData.Param[3] = lpPlayInfo->smCharInfo.JOB_CODE;
	sEventData.Param[4] = lpPlayInfo->smCharInfo.Level;
	sEventData.Param[5] = CashFlag;


	//�̺�Ʈ �α� ť�� ����Ÿ ����
	return b_PushLogEventQue( GAMELOG_SQL_STARPOINT , sizeof(sLOG_EVENT) , &sEventData );
}

//Polling ��Ϲ� ����
int	bSql_PollRecord( rsPLAYINFO *lpPlayInfo , int PollKind , int PollCount , BYTE *PollData )
{
	sLOG_EVENT sEventData;
	int cnt;

	if ( !BillSql.PollRecord ) return FALSE;

	ZeroMemory( &sEventData , sizeof(sLOG_EVENT) );

	lpPlayInfo->szID[31] = 0;
	lpPlayInfo->szName[31] = 0;
	lpPlayInfo->szGPCode[15] = 0;

	lstrcpy( sEventData.szID , lpPlayInfo->szID );
	lstrcpy( sEventData.szName , lpPlayInfo->szName );
	lstrcpy( sEventData.szGPCode , lpPlayInfo->szGPCode );
	lstrcpy( sEventData.szIP , lpPlayInfo->lpsmSock->szIPAddr );

	sEventData.Param[10] = PollCount;
	sEventData.Param[11] = PollKind;
	sEventData.Param[12] = (DWORD)lpPlayInfo;
	sEventData.Param[13] = lpPlayInfo->dwConnectCount;

	if ( PollCount && PollData ) {
		for(cnt=0;cnt<PollCount;cnt++) {
			sEventData.Param[cnt] = PollData[cnt];
		}
	}

	//�̺�Ʈ �α� ť�� ����Ÿ ����
	return b_PushLogEventQue( GAMELOG_SQL_POLLRECORD , sizeof(sLOG_EVENT) , &sEventData );
}


//�̺�Ʈ ����
int	bSql_EventJoin( rsPLAYINFO *lpPlayInfo )
{
	sLOG_EVENT sEventData;

	if ( !BillSql.EventJoin ) return FALSE;

	ZeroMemory( &sEventData , sizeof(sLOG_EVENT) );

	lpPlayInfo->szID[31] = 0;
	lpPlayInfo->szName[31] = 0;
	lpPlayInfo->szGPCode[15] = 0;

	lstrcpy( sEventData.szID , lpPlayInfo->szID );
	lstrcpy( sEventData.szName , lpPlayInfo->szName );
	lstrcpy( sEventData.szGPCode , lpPlayInfo->szGPCode );
	lstrcpy( sEventData.szIP , lpPlayInfo->lpsmSock->szIPAddr );

	//�̺�Ʈ �α� ť�� ����Ÿ ����
	return b_PushLogEventQue( GAMELOG_SQL_EVENTJOIN , sizeof(sLOG_EVENT) , &sEventData );
}


//SOD Ŭ�� ��â�� 
int	bSql_ClanMoney( rsPLAYINFO *lpPlayInfo , int Cash , int CashFlag , int Kind )
{
	sLOG_EVENT sEventData;

	if ( !BillSql.ClanMoney ) return FALSE;

	ZeroMemory( &sEventData , sizeof(sLOG_EVENT) );

	lpPlayInfo->szID[31] = 0;
	lpPlayInfo->szName[31] = 0;
	lpPlayInfo->szGPCode[15] = 0;

	lstrcpy( sEventData.szID , lpPlayInfo->szID );
	lstrcpy( sEventData.szName , lpPlayInfo->szName );
	lstrcpy( sEventData.szGPCode , lpPlayInfo->szGPCode );
	lstrcpy( sEventData.szIP , lpPlayInfo->lpsmSock->szIPAddr );

	sEventData.Param[0] = Cash;
	sEventData.Param[1] = (DWORD)lpPlayInfo;
	sEventData.Param[2] = lpPlayInfo->dwConnectCount;

	sEventData.Param[3] = lpPlayInfo->smCharInfo.JOB_CODE;
	sEventData.Param[4] = lpPlayInfo->smCharInfo.Level;
	sEventData.Param[5] = CashFlag;
	sEventData.Param[6] = Kind;

	


	//�̺�Ʈ �α� ť�� ����Ÿ ����
	return b_PushLogEventQue( GAMELOG_SQL_CLANMONEY , sizeof(sLOG_EVENT) , &sEventData );
}

//��ȭ���� ���� DB�� ���
int	bSql_RecordCurrency()
{

	sLOG_EVENT sEventData;

	if ( !BillSql.SiegeMoneyTax ) return FALSE;
	
	ZeroMemory( &sEventData , sizeof(sLOG_EVENT) );

	//�̺�Ʈ �α� ť�� ����Ÿ ����
	return b_PushLogEventQue( GAMELOG_SQL_SIEGETAX , sizeof(sLOG_EVENT) , &sEventData );

}



//GiveMoney ���
int	bSql_GiveMoney( rsPLAYINFO *lpPlayInfo , int Money )
{
	sLOG_EVENT sEventData;

	if ( !BillSql.Contribution ) return FALSE;

	ZeroMemory( &sEventData , sizeof(sLOG_EVENT) );

	lpPlayInfo->szID[31] = 0;
	lpPlayInfo->szName[31] = 0;
	lpPlayInfo->szGPCode[15] = 0;

	lstrcpy( sEventData.szID , lpPlayInfo->szID );
	lstrcpy( sEventData.szName , lpPlayInfo->szName );
	lstrcpy( sEventData.szGPCode , lpPlayInfo->szGPCode );
	lstrcpy( sEventData.szIP , lpPlayInfo->lpsmSock->szIPAddr );

	sEventData.Param[0] = Money;
	sEventData.Param[1] = (DWORD)lpPlayInfo;
	sEventData.Param[2] = lpPlayInfo->dwConnectCount;

	//�̺�Ʈ �α� ť�� ����Ÿ ����
	return b_PushLogEventQue( GAMELOG_SQL_GIVEMONEY , sizeof(sLOG_EVENT) , &sEventData );
}




//Ŭ�� Ƽ�� ����
int bClan_GetTicket()
{
	if ( BillSql.GetTicket ) {
		return BillSql.GetTicket();
	}
	return NULL;
}

//Ŭ�� Ƽ�� ó��
int	bClan_UserTicket( rsPLAYINFO *lpPlayInfo , int gubun )
{
	sCLAN_EVENT	sClanEvent;

	if ( !BillSql.hClanDLL || !BillSql.GetTicket ) {
		lpPlayInfo->ClanTicketCode = 0;
		return NULL;
	}

	lpPlayInfo->szID[31] = 0;

	lstrcpy( sClanEvent.szID , lpPlayInfo->szID );
	lstrcpy( sClanEvent.szName , lpPlayInfo->szName );
	lstrcpy( sClanEvent.szIP , lpPlayInfo->lpsmSock->szIPAddr );

	if ( gubun==0 ) {		//Ƽ�� �߱�
		sClanEvent.TicketNum = lpPlayInfo->ClanTicketCode;
	}
	else {	//Ƽ�� ����
//		if ( lpPlayInfo->szName[0] && !lpPlayInfo->smCharInfo.ClassClan )
//			return FALSE;

		sClanEvent.TicketNum = lpPlayInfo->ClanTicketCode;
	}

	sClanEvent.Param[0] = gubun;
	sClanEvent.Param[1] = lpPlayInfo->smCharInfo.JOB_CODE;
	sClanEvent.Param[2] = lpPlayInfo->smCharInfo.Level;

	return b_PushLogEventQue( CLAN_USER_TICKET , sizeof(sCLAN_EVENT) , &sClanEvent );
}


//Ŭ�� ����Ʈ ���
int	bClan_InsertPoint( rsPLAYINFO *lpPlayInfo , int point )
{
	sCLAN_EVENT	sClanEvent;

	if ( !BillSql.hClanDLL || !BillSql.GetTicket ) return NULL;

	lpPlayInfo->szID[31] = 0;

	lstrcpy( sClanEvent.szID , lpPlayInfo->szID );
	lstrcpy( sClanEvent.szName , lpPlayInfo->szName );
	lstrcpy( sClanEvent.szIP , lpPlayInfo->lpsmSock->szIPAddr );

	sClanEvent.TicketNum = lpPlayInfo->ClanTicketCode;
	sClanEvent.Param[0] = point;

	return b_PushLogEventQue( CLAN_INSERT_POINT , sizeof(sCLAN_EVENT) , &sClanEvent );
}

//Ŭ�� ��ũ �ڵ� ���ϱ�
int	bClan_GetClanMarkCode( rsPLAYINFO *lpPlayInfo )
{
	sCLAN_EVENT	sClanEvent;

	if ( !BillSql.hClanDLL || !BillSql.ClanUserMarkNum ) return NULL;

	lpPlayInfo->szID[31] = 0;

	lstrcpy( sClanEvent.szID , lpPlayInfo->szID );
	lstrcpy( sClanEvent.szName , lpPlayInfo->szName );
	lstrcpy( sClanEvent.szIP , lpPlayInfo->lpsmSock->szIPAddr );

	sClanEvent.Param[0] = 0;
	sClanEvent.Param[1] = (DWORD)lpPlayInfo;
	sClanEvent.Param[2] = lpPlayInfo->dwConnectCount;

	lpPlayInfo->dwCheck_MasterClanTime = dwPlayServTime;

	return b_PushLogEventQue( CLAN_GET_MARKCODE , sizeof(sCLAN_EVENT) , &sClanEvent );
}





//(����) PC�� ���� IP����
int	b_IPRecord(char *lpszServerName, struct JUNGIP *JungIP)
{
	if ( BillSql.IPRecord ) {
		return BillSql.IPRecord( lpszServerName , JungIP );
	}
	return TRUE;
}
//(����) PC�� ���� IP����
int	b_IPRecordTime(char *lpszServerName, struct JUNGIP *JungIP)
{
	if ( BillSql.IPRecordTime ) {
		return BillSql.IPRecordTime( lpszServerName , JungIP );
	}
	return TRUE;
}

int b_SqlGetQueSize()
{
	int cnt;
	cnt = LogSqlDataCount-LogSqlSendCount;

	if ( cnt<LogSqlProcessQueMax ) cnt=LogSqlProcessQueMax;

	LogSqlProcessQueMax = 0;

	return cnt;
}

int b_SqlGetLossCount()
{
	return LogSqlLossCount;
}

int b_SqlGetQueProcessTime( DWORD *pTimeMax , DWORD *pAvgTime )
{
	int cnt;

	cnt = LogSqlProcessCount;

	*pTimeMax = LogSqlProcessTimeMax;
	if ( LogSqlProcessCount>0 )
		*pAvgTime = LogSqlProcessTime/LogSqlProcessCount;
	else
		*pAvgTime = 0;


	LogSqlProcessTime = 0;
	LogSqlProcessTimeMax = 0;
	LogSqlProcessCount = 0;

	return cnt;
}





//���� ����Ÿ�� �����Ű�� ������
DWORD WINAPI LogItemDataThreadProc( void *pInfo )
{
	HANDLE	hThread;

	LogITEM_QUE		LogItemData;
	char			szID[32];
	int	LogType;
	LogITEM			*lpLogItem;
	LogITEM_TRADE	*lpLogItemTrade;
	LogGiftITEM		*lpLogGiftItem;


	hThread = GetCurrentThread();

	while(1) {
		while(1) {
			EnterCriticalSection( &cSerSection );			//ũ��ƼĮ ���� ����
			LogType = b_PopLogItemQue(&LogItemData);
			LeaveCriticalSection( &cSerSection );			//ũ��ƼĮ ���� ����
			if ( !LogType ) break;

			//EnterCriticalSection( &cSqlSection );
			if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 100+LogType , dwPlayServTime );
			switch( LogType ) {
			case 1:
				//�Ϲ� ������ �α�
				if ( BillSql.LogITEMRecord ) {
					lpLogItem = (LogITEM  *)LogItemData.szBuff;
					lpLogItem->UserID[31] = 0;
					lstrcpy( szID , lpLogItem->UserID );
					if ( lpLogItem->ItemCount<32 && lpLogItem->size<sizeof(LogITEM_TRADE) ) {
						BillSql.LogITEMRecord( szID , lpLogItem );
					}
				}
				break;

			case 2:
				//�ŷ� ������ �α�
				if ( BillSql.LogITEMTradeRecord ) {
					lpLogItemTrade = (LogITEM_TRADE  *)LogItemData.szBuff;
					lpLogItemTrade->UserID[31] = 0;
					lstrcpy( szID , lpLogItemTrade->UserID );
					if ( lpLogItemTrade->ItemCount<32 && lpLogItemTrade->size<sizeof(LogITEM_TRADE) ) {
						BillSql.LogITEMTradeRecord(  szID , lpLogItemTrade );
					}
				}
				break;

			case 3:
				//������ ������ �α�
				if ( BillSql.LogITEMPostRecord ) {
					lpLogGiftItem = (LogGiftITEM  *)LogItemData.szBuff;
					lpLogGiftItem->UserID[31] = 0;
					lstrcpy( szID , lpLogGiftItem->UserID );
					if ( lpLogGiftItem->size<sizeof(LogITEM_TRADE) ) {
						BillSql.LogITEMPostRecord( szID , lpLogGiftItem );
					}
				}
				break;

			}
			if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );
			//LeaveCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
		}

		SuspendThread(hThread);
		if ( quit ) break;
	}

	ExitThread( TRUE );
	return TRUE;
}

//	hLogSqlThread = CreateThread( NULL , 0, LogSqlDataThreadProc , 0 , 0, &dwLogSqlThreadId );

HANDLE	hLogItemThread=0;
DWORD	dwLogItemThreadID;

//������ �α� ť�� ����Ÿ ����
int	b_PushLogItemQue( int type , int size, void *lpLogItemData )
{
	int cnt;

	if ( !BillSql.LogITEMRecord || !BillSql.LogITEMTradeRecord ) return FALSE;

	if ( !hLogItemThread ) {
		hLogItemThread = CreateThread( NULL , 0, LogItemDataThreadProc , 0 , 0, &dwLogItemThreadID );
		if ( !hLogItemThread ) return FALSE;
	}

	if ( (LogItemQuePush-LogItemQuePop)>LOG_ITEM_QUE_MASK || size>sizeof(LogITEM_TRADE) ) {
		LogItemLossCount ++;
		return FALSE;
	}

	//if ( hDebugWnd ) SendMessage( hDebugWnd , SWM_DISPDEBUG , 1 , dwPlayServTime );

	cnt = LogItemQuePush&LOG_ITEM_QUE_MASK;

	LogItemQue[cnt].LogType = type;
	memcpy( LogItemQue[cnt].szBuff ,lpLogItemData , size );
	LogItemQuePush++;

	ResumeThread(hLogItemThread);

	//if ( hDebugWnd ) SendMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );

	return TRUE;
}

//������ �α� ť�� ����Ÿ ����
int b_PopLogItemQue(LogITEM_QUE *lpLogItemQue)
{
	int cnt;
	int len;

	len = LogItemQuePush-LogItemQuePop;
	if ( len>LOG_ITEM_QUE_MASK ) {
		//ť �䷮ �ʰ�
		LogItemQuePop = LogItemQuePush-LOG_ITEM_QUE_MASK;
	}
	if ( len<=0 ) {
		//ť�� ó���� ����Ÿ ����
		return NULL;
	}

	//if ( hDebugWnd ) SendMessage( hDebugWnd , SWM_DISPDEBUG , 2 , dwPlayServTime );

	cnt = LogItemQuePop&LOG_ITEM_QUE_MASK;

	len = LogItemQue[cnt].LogType;
	memcpy( lpLogItemQue , &LogItemQue[cnt] , sizeof(LogITEM_QUE) );
	LogItemQuePop++;

	//if ( hDebugWnd ) SendMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );

	return len;
}


int b_SqlGetItemQueSize()
{
	return LogItemQuePush-LogItemQuePop;
}
int b_SqlGetItemLossCount()
{
	return LogItemLossCount;
}

/////////////////////////////////// �̺�Ʈ ť /////////////////////////////

//���� �̺�Ʈ ��� �����Ű�� ������
DWORD WINAPI LogEventDataThreadProc( void *pInfo )
{
	HANDLE	hThread;

	LogEVENT_QUE		LogEventData;
	int	LogType;
	sLOG_EVENT			*sLogEvent;
	sCLAN_EVENT			*sClanEvent;
	int	Result;
	rsPLAYINFO			*lpPlayInfo;
	smTRANS_COMMAND		smTransCommand;
	INT64				int64_Money;
	rsCURRENCY_LOG		currency;
	POLL				PollData;
	//TRANS_CHATMESSAGE	TransChatMessage;

	hThread = GetCurrentThread();

	while(1) {
		while(1) {
			EnterCriticalSection( &cSerSection );			//ũ��ƼĮ ���� ����
			LogType = b_PopLogEventQue(&LogEventData);
			LeaveCriticalSection( &cSerSection );			//ũ��ƼĮ ���� ����
			if ( !LogType ) break;

			switch( LogType ) {
/*
			case 1:
				//�Ϲ� ������ �α�
				if ( BillSql.LogITEMRecord ) {
					lpLogItem = (LogITEM  *)LogItemData.szBuff;
					lpLogItem->UserID[31] = 0;
					lstrcpy( szID , lpLogItem->UserID );
					if ( lpLogItem->ItemCount<32 && lpLogItem->size<sizeof(LogITEM_TRADE) ) {
						BillSql.LogITEMRecord( szID , lpLogItem );
					}
				}
*/
			case GAMELOG_SQL_SOD:
				//SoD ��� ����
				if ( BillSql.WithSODRecordRanks ) {

					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 500+LogType , dwPlayServTime );

					sLogEvent = (sLOG_EVENT *)LogEventData.szBuff;

					sLogEvent->szID[31] = 0;
					sLogEvent->szName[31] = 0;
					sLogEvent->szGPCode[15] = 0;

					//EnterCriticalSection( &cSqlSection );

					BillSql.WithSODRecordRanks( sLogEvent->szID , sLogEvent->szName , 
						sLogEvent->Param[0] , sLogEvent->Param[1] , sLogEvent->Param[2] , sLogEvent->Param[3],
						sLogEvent->Param[4] , sLogEvent->Param[5] , sLogEvent->Param[6] , sLogEvent->Param[7],
						sLogEvent->szGPCode , sLogEvent->szIP , sLogEvent->Param[8] );

					//LeaveCriticalSection( &cSqlSection );					//ũ��ƼĮ ���� ����

					if ( hDebugWnd ) PostMessage( hDebugWnd , SWM_DISPDEBUG , 0 , dwPlayServTime );

				}
				break;

			case GAMELOG_SQL_STARPOINT:
				if ( BillSql.UserStar || BillSql.UserStarRecord ) {
					sLogEvent = (sLOG_EVENT *)LogEventData.szBuff;

					sLogEvent->szID[31] = 0;
					sLogEvent->szName[31] = 0;
					sLogEvent->szGPCode[15] = 0;

					if ( BillSql.UserStarRecord ) {
						Result = BillSql.UserStarRecord( sLogEvent->szID , sLogEvent->Param[0] , 
							sLogEvent->szName , sLogEvent->Param[3] , sLogEvent->Param[4] , sLogEvent->Param[5]  );
					}
					else {
						Result = BillSql.UserStar( sLogEvent->szID , sLogEvent->Param[0] );
					}

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����

					lpPlayInfo = (rsPLAYINFO *)sLogEvent->Param[1];

					//��� ���� ( �� ���� ���� )
					if ( lpPlayInfo && lpPlayInfo->lpsmSock && lpPlayInfo->dwConnectCount==sLogEvent->Param[2] ) {
						smTransCommand.size = sizeof(smTRANS_COMMAND);
						smTransCommand.code = smTRANSCODE_STARPOINT;
						smTransCommand.WParam = Result;
						smTransCommand.LParam = 0;
						smTransCommand.SParam = 0;
						smTransCommand.EParam = 0;
						lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
					}
					LeaveCriticalSection( &cSerSection );
					
				}
				break;

			case GAMELOG_SQL_GIVEMONEY:
				if ( BillSql.Contribution ) {
					sLogEvent = (sLOG_EVENT *)LogEventData.szBuff;

					sLogEvent->szID[31] = 0;
					sLogEvent->szName[31] = 0;
					sLogEvent->szGPCode[15] = 0;

					Result = BillSql.Contribution( sLogEvent->szID , sLogEvent->szName , sLogEvent->Param[0] );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����

					lpPlayInfo = (rsPLAYINFO *)sLogEvent->Param[1];

					/*
					//��� ���� ( �� ���� ���� )
					if ( lpPlayInfo && lpPlayInfo->lpsmSock && lpPlayInfo->dwConnectCount==sLogEvent->Param[2] ) {
						smTransCommand.size = sizeof(smTRANS_COMMAND);
						smTransCommand.code = smTRANSCODE_STARPOINT;
						smTransCommand.WParam = Result;
						smTransCommand.LParam = 0;
						smTransCommand.SParam = 0;
						smTransCommand.EParam = 0;
						lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
					}
					*/
					LeaveCriticalSection( &cSerSection );
					
				}
				break;
				
			case CLAN_USER_TICKET:					//Ŭ�� Ƽ��
				sClanEvent = (sCLAN_EVENT *)LogEventData.szBuff;

				sClanEvent->szID[31] = 0;
				sClanEvent->szName[31] = 0;
				sClanEvent->szIP[15] = 0;

				if ( BillSql.ClanUserTicKets ) {
					BillSql.ClanUserTicKets( sClanEvent->szID , sClanEvent->szName , sClanEvent->szIP , sClanEvent->TicketNum , sClanEvent->Param[0], sClanEvent->Param[1] , sClanEvent->Param[2] );
					break;
				}
				if ( BillSql.ClanUserTicket ) {
					BillSql.ClanUserTicket( sClanEvent->szID , sClanEvent->szName , sClanEvent->szIP , sClanEvent->TicketNum , sClanEvent->Param[0] );
					break;
				}

				break;

			case CLAN_INSERT_POINT:					//Ŭ�� ����Ʈ
				if ( BillSql.ClanPointInsert ) {
					sClanEvent = (sCLAN_EVENT *)LogEventData.szBuff;

					sClanEvent->szID[31] = 0;
					sClanEvent->szName[31] = 0;
					sClanEvent->szIP[15] = 0;

					BillSql.ClanPointInsert( sClanEvent->szID , sClanEvent->szName , sClanEvent->szIP , sClanEvent->TicketNum , sClanEvent->Param[0] );
				}
				break;

			case CLAN_GET_MARKCODE:
				if ( BillSql.ClanUserMarkNum ) {
					sClanEvent = (sCLAN_EVENT *)LogEventData.szBuff;

					sClanEvent->szID[31] = 0;
					sClanEvent->szName[31] = 0;
					sClanEvent->szIP[15] = 0;

					Result = BillSql.ClanUserMarkNum( sClanEvent->szID , sClanEvent->szName );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
					lpPlayInfo = (rsPLAYINFO *)sClanEvent->Param[1];
					//��� ���� ( Ŭ�� ��ũ ��ȣ )
					if ( lpPlayInfo && lpPlayInfo->lpsmSock && lpPlayInfo->dwConnectCount==sClanEvent->Param[2] ) {
						lpPlayInfo->CallBack_ClanMarkNum( Result );
					}
					LeaveCriticalSection( &cSerSection );
				}
				break;



			case GAMELOG_SQL_EVENTJOIN:
				if ( BillSql.EventJoin ) {
					sLogEvent = (sLOG_EVENT *)LogEventData.szBuff;

					sLogEvent->szID[31] = 0;
					sLogEvent->szName[31] = 0;
					sLogEvent->szGPCode[15] = 0;

					Result = BillSql.EventJoin( sLogEvent->szID , sLogEvent->szName );
				}
				break;
				

			case GAMELOG_SQL_CLANMONEY:
				if ( BillSql.ClanMoney && BillSql.SiegeMoney ) {
					sLogEvent = (sLOG_EVENT *)LogEventData.szBuff;

					sLogEvent->szID[31] = 0;
					sLogEvent->szName[31] = 0;
					sLogEvent->szGPCode[15] = 0;

					if ( sLogEvent->Param[6] )
						Result = (int)BillSql.SiegeMoney( sLogEvent->szID , sLogEvent->szName , (INT64)sLogEvent->Param[0] , sLogEvent->Param[5] );
					else
						Result = BillSql.ClanMoney( sLogEvent->szID , sLogEvent->szName , sLogEvent->Param[0] , sLogEvent->Param[5] );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����

					lpPlayInfo = (rsPLAYINFO *)sLogEvent->Param[1];

					//��� ���� ( Ŭ���Ӵ� )
					if ( lpPlayInfo && lpPlayInfo->lpsmSock && lpPlayInfo->dwConnectCount==sLogEvent->Param[2] ) {
						smTransCommand.size = sizeof(smTRANS_COMMAND);
						smTransCommand.code = smTRANSCODE_CLANMONEY;
						smTransCommand.WParam = Result;						//�ܾ�

						switch( sLogEvent->Param[5] ) {
							case 0:		//�ܾ���ȸ
								smTransCommand.LParam = 0;
								break;
							
							case 1:		//Ŭ���Ӵ� ã��
								smTransCommand.LParam = sLogEvent->Param[0];		//�߰� �ݾ�
								lpPlayInfo->AddServerMoney( sLogEvent->Param[0] , WHERE_CLAN_MONEY );
								rsClanMoneyLog( lpPlayInfo , smTransCommand.LParam , Result );	//Ŭ�� �Ӵ� ���
								break;

							case 2:		//Ŭ���Ӵ� �ֱ�
								smTransCommand.LParam = -sLogEvent->Param[0];		//�߰� �ݾ�
								lpPlayInfo->SubServerMoney ( sLogEvent->Param[0] , WHERE_CLAN_MONEY );
								rsClanMoneyLog( lpPlayInfo , smTransCommand.LParam , Result );	//Ŭ�� �Ӵ� ���
								break;
						}

						smTransCommand.SParam = sLogEvent->Param[6];
						smTransCommand.EParam = 0;
						lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

					}
					LeaveCriticalSection( &cSerSection );
					
				}
				break;

			case GAMELOG_SQL_SIEGETAX:
				if ( BillSql.SiegeMoneyTax ) {

					sLogEvent = (sLOG_EVENT *)LogEventData.szBuff;
					wsprintf( sLogEvent->szID , "%d" , rsBlessCastle.dwMasterClan );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
					memcpy( &currency , &CurrencyLog , sizeof(rsCURRENCY_LOG) );
					LeaveCriticalSection( &cSerSection );

					currency.int64_CraftMoney += (currency.int64_CraftMoney*rsBlessCastle.Tax)/100;
					currency.int64_AgingMoney += (currency.int64_AgingMoney*rsBlessCastle.Tax)/100;
					currency.int64_ShopMoney += (currency.int64_ShopMoney*rsBlessCastle.Tax)/100;
					currency.int64_ShopPotionMoney[0] += (currency.int64_ShopPotionMoney[0]*rsBlessCastle.Tax)/100;
					currency.int64_ShopPotionMoney[1] += (currency.int64_ShopPotionMoney[1]*rsBlessCastle.Tax)/100;
					currency.int64_ShopPotionMoney[2] += (currency.int64_ShopPotionMoney[2]*rsBlessCastle.Tax)/100;

					int64_Money = currency.int64_CraftMoney+currency.int64_AgingMoney+currency.int64_ShopMoney;
					int64_Money+= currency.int64_ShopPotionMoney[0]+currency.int64_ShopPotionMoney[1]+currency.int64_ShopPotionMoney[2];
					int64_Money+= currency.int64_ForceOrbMoney+currency.int64_WarpgateMoney+currency.int64_LearnSkillMoney;

					Result = BillSql.SiegeMoneyTax( 
													sLogEvent->szID ,					//Ŭ���ڵ�
													currency.int64_CraftMoney , 
													currency.int64_AgingMoney , 
													currency.int64_ShopMoney , 
													currency.int64_ShopPotionMoney[0] , 
													currency.int64_ShopPotionMoney[1] , 
													currency.int64_ShopPotionMoney[2] , 
													currency.int64_ForceOrbMoney,
													currency.int64_WarpgateMoney,
													currency.int64_LearnSkillMoney,
													int64_Money,
													rsBlessCastle.Tax );

					if ( Result ) {
						EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
						ZeroMemory( &CurrencyLog , sizeof(rsCURRENCY_LOG) );
						LeaveCriticalSection( &cSerSection );
					}
				}
				break;


			case GAMELOG_SQL_POLLRECORD:
				if ( BillSql.PollRecord ) {
					sLogEvent = (sLOG_EVENT *)LogEventData.szBuff;

					sLogEvent->szID[31] = 0;
					sLogEvent->szName[31] = 0;
					sLogEvent->szIP[15] = 0;

					ZeroMemory( &PollData , sizeof(POLL) );

					lstrcpy( PollData.UserID , sLogEvent->szID );
					lstrcpy( PollData.CharID , sLogEvent->szName );
					PollData.PollCode = sLogEvent->Param[11];
					PollData.PollCount = sLogEvent->Param[10];

					if ( PollData.PollCount ) {
						memcpy( PollData.PollItem , sLogEvent->Param , sizeof(int)*PollData.PollCount );
						Result = BillSql.PollRecord( sLogEvent->szID , tServerTime , &PollData , 1 );
					}
					else
						Result = BillSql.PollRecord( sLogEvent->szID , tServerTime , &PollData , 0 );

					EnterCriticalSection( &cSerSection );					//ũ��ƼĮ ���� ����
					lpPlayInfo = (rsPLAYINFO *)sLogEvent->Param[12];
					//��� ����
					if ( lpPlayInfo && lpPlayInfo->lpsmSock && lpPlayInfo->dwConnectCount==sLogEvent->Param[13] ) {
						if ( !PollData.PollCount ) {
							smTransCommand.code = smTRANSCODE_PUBLIC_POLLING;
							smTransCommand.size = sizeof ( smTRANS_COMMAND );
							smTransCommand.WParam = sLogEvent->Param[11];
							if ( Result )
								smTransCommand.LParam = 0;
							else
								smTransCommand.LParam = 1;
							smTransCommand.SParam = 0;
							smTransCommand.EParam = 0;
							lpPlayInfo->lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
						}
						else {
							if ( Result ) {
								/*
								TransChatMessage.code = smTRANSCODE_MESSAGEBOX;
								TransChatMessage.dwIP = 0;
								TransChatMessage.dwObjectSerial = 0;
								lstrcpy( TransChatMessage.szMessage , srMsg_ThanksPolling );
								TransChatMessage.size = 32+lstrlen(TransChatMessage.szMessage);
								lpPlayInfo->lpsmSock->Send( (char *)&TransChatMessage , TransChatMessage.size , TRUE );
								*/
							}
						}
					}
					LeaveCriticalSection( &cSerSection );
				}
				break;
			}
		}

		SuspendThread(hThread);
		if ( quit ) break;
	}

	ExitThread( TRUE );
	return TRUE;
}



HANDLE	hLogEventThread=0;
DWORD	dwLogEventThreadID;

//�̺�Ʈ �α� ť�� ����Ÿ ����
int	b_PushLogEventQue( int type , int size, void *lpLogEventData )
{
	int cnt;

	if ( !hLogEventThread ) {
		hLogEventThread = CreateThread( NULL , 0, LogEventDataThreadProc , 0 , 0, &dwLogEventThreadID );
		if ( !hLogEventThread ) return FALSE;
	}

	if ( (LogEventQuePush-LogEventQuePop)>LOG_EVENT_QUE_MASK || size>512 ) {
		LogEventLossCount ++;
		return FALSE;
	}

	cnt = LogEventQuePush&LOG_EVENT_QUE_MASK;

	LogEventQue[cnt].LogType = type;
	memcpy( LogEventQue[cnt].szBuff ,lpLogEventData , size );
	LogEventQuePush++;

	ResumeThread(hLogEventThread);

	return TRUE;
}


//�̺�Ʈ �α� ť�� ����Ÿ ����
int b_PopLogEventQue( LogEVENT_QUE *lpLogEventQue)
{
	int cnt;
	int len;


	len = LogEventQuePush-LogEventQuePop;
	if ( len>LOG_EVENT_QUE_MASK ) {
		//ť �䷮ �ʰ�
		LogEventQuePop = LogEventQuePush-LOG_EVENT_QUE_MASK;
	}
	if ( len<=0 ) {
		//ť�� ó���� ����Ÿ ����
		return NULL;
	}

	if ( !LogEventQue ) return NULL;

	cnt = LogEventQuePop&LOG_EVENT_QUE_MASK;

	len = LogEventQue[cnt].LogType;
	memcpy( lpLogEventQue , &LogEventQue[cnt] , sizeof(LogEVENT_QUE) );
	LogEventQuePop++;

	return len;
}


int b_SqlGetEventQueSize()
{
	return LogEventQuePush-LogEventQuePop;
}
int b_SqlGetEventLossCount()
{
	return LogEventLossCount;
}



/*
LogITEM_QUE *LogEventQue=0;
int			LogEventQuePush;
int			LogEventQuePop;
int			LogEventLossCount = 0;
DWORD		LogEventQueProcessTime = 0;		//ó�� �ð�

struct	LogEVENT_QUE {
	int		LogType;
	char	szBuff[512];
};
#define		LOG_EVENT_QUE_MAX		256
#define		LOG_EVENT_QUE_MASK		255
*/



///////////////////// �߱� nSprite //////////////////////
/*
#ifdef _LANGUAGE_CHINESE

#include "..\\nSprite\\nsprtsvr.h"
#pragma comment(lib,"nSprite\\nsprtsvr.lib")
#define	_nSPRITE

DWORD nSprite_GetResult( DWORD nRecvNum )
{

	return ParseNSpriteVersion( nRecvNum );

	DWORD dwVersion;

	dwVersion = ParseNSpriteVersion( nRecvNum );

	switch( dwVersion ) {
		case NSPRITE_OK:
			return TRUE;
		case VERSION_OLD:
			return -1;
		case EXIST_CHEAT:
			return -1;
	}

	return NULL;

}

int	nSprite_Init()
{
	InitNSprtsvr();
	rsNSpriteMode = TRUE;
	return TRUE;
}

#else
*/
DWORD nSprite_GetResult( DWORD nRecvNum )
{
	return TRUE;
}

int	nSprite_Init()
{
	return TRUE;
}

//#endif

/*
DWORD nSprite_GetResult( int nRecvNum )
{
	return TRUE;
}
*/

#else
int			UseDLL_Expined;

int InitLogSql()
{
	return TRUE;
}
int CloseLogSql()
{
	return TRUE;
}
int RecordLogSql( DWORD	dwRecCode , char *szIP , char *lpUserID , char *lpCharName , char *szGPCode , int Race , int JobCode , INT64 Exp64 , DWORD dwLevel, DWORD dwMoney )
{
	return TRUE;
}
int RecordGameLogSql( DWORD	dwRecCode , char *szIP , char *lpUserID , char *lpCharName  , char *szGPCode, int JobCode , char *szGuid , int BlockTime )
{
	return TRUE;
}

int	bSql_LogOn(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *szGuid , char *lpszPCRNo)
{
	return TRUE;
}

int	bSql_Logout(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t DisuseDay, int UsePeriod, char *szGuid , int PCRNo)
{
	return TRUE;
}


int	bSql_ServerDown()
{
	return TRUE;
}
int	bSql_ServerExit()
{
	return TRUE;
}
int	bSql_GetGPCode(char *GPCode, char *PG)
{
	return TRUE;
}

//int	bSql_PCUse( char *lpszPID, char *lpszGPCode, char *ServerName, char *lpszIP, int lpszRNo)
int	bSql_PCUse( rsPLAYINFO *lpPlayInfo )
{
	return TRUE;
}

int	bUrs_Expined( rsPLAYINFO *lpPlayInfo )
{
	return TRUE;
}

//���� ���� Ȯ��
int	bSql_PCSearchReserv( rsPLAYINFO *lpPlayInfo )
{
	return TRUE;
}
//���� ����
int	bSql_ChangeUserIdGpCodeGame( int PCRNo )
{
	return TRUE;
}
//�̺�Ʈ ����
int	bSql_EventLogon( rsPLAYINFO *lpPlayInfo )
{
	return TRUE;
}
//�̺�Ʈ �α׾ƿ�
int	bSql_EventLogOut( rsPLAYINFO *lpPlayInfo )
{
	return TRUE;
}


//PC�� �߰� �ð� ǥ��
int	bSql_PCRNoReservProduct( rsPLAYINFO *lpPlayInfo )	
{
	return TRUE;
}

//(����) PC�� ���� IP����
int	b_IPRecord(char *lpszServerName, struct JUNGIP *JungIP)
{
	return TRUE;
}
//(����) PC�� ���� IP����
int	b_IPRecordTime(char *lpszServerName, struct JUNGIP *JungIP)
{
	return TRUE;
}

//SOD ���� ���
int	bSql_SODRecord( rsPLAYINFO *lpPlayInfo , int Level , int SodScore , int SodKillCount , int SodTotalScore , int SodTotalSucessUser , int SodTotalUser , int SodCode )
{
	return TRUE;
}
//Star Point ��Ϲ� ����
int	bSql_StarPoint( rsPLAYINFO *lpPlayInfo , int StarPoint , int CashFlag )
{
	return TRUE;
}

//Polling ��Ϲ� ����
int	bSql_PollData( rsPLAYINFO *lpPlayInfo , int PollKind , int PollCount , BYTE *PollData )
{
	return TRUE;
}

//ExpGameTime ��Ϲ� ����
int	bSql_GetExpGameTime( rsPLAYINFO *lpPlayInfo , int GamePlayTime )
{
	return TRUE;
}

//�̺�Ʈ ����
int	bSql_EventJoin( rsPLAYINFO *lpPlayInfo )
{
	return TRUE;
}
//SOD Ŭ�� ��â�� 
int	bSql_ClanMoney( rsPLAYINFO *lpPlayInfo , int Cash , int CashFlag , int Kind )
{
	return	TRUE;
}

//Ŭ�� ��ũ �ڵ� ���ϱ�
int	bClan_GetClanMarkCode( rsPLAYINFO *lpPlayInfo )
{
	return TRUE;
}
//��ȭ���� ���� DB�� ���
int	bSql_RecordCurrency()
{
	return	TRUE;
}

//GiveMoney ���
int	bSql_GiveMoney( rsPLAYINFO *lpPlayInfo , int Money )
{
	return TRUE;
}

int b_SqlGetQueSize()
{
	return TRUE;
}
int b_SqlGetLossCount()
{
	return TRUE;
}
int b_SqlGetQueProcessTime( DWORD *pTimeMax , DWORD *pAvgTime )
{
	return TRUE;
}
//������ �α� ť�� ����Ÿ ����
int b_PopLogItemQue(LogITEM_QUE *lpLogItemQue)
{
	return TRUE;
}
//������ �α� ť�� ����Ÿ ����
int	b_PushLogItemQue( int type , int size, void *lpLogItemData )
{
	return TRUE;
}


int b_SqlGetItemQueSize()
{
	return TRUE;
}
int b_SqlGetItemLossCount()
{
	return TRUE;
}

//������ �α� ť�� ����Ÿ ����
int b_PopLogEventQue(LogEVENT_QUE *lpLogEventQue)
{
	return TRUE;
}
//������ �α� ť�� ����Ÿ ����
int	b_PushLogEventQue( int type , int size, void *lpLogEventData )
{
	return TRUE;
}


int b_SqlGetEventQueSize()
{
	return TRUE;
}
int b_SqlGetEventLossCount()
{
	return TRUE;
}


//Ŭ�� Ƽ�� ó��
int	bClan_UserTicket( rsPLAYINFO *lpPlayInfo , int gubun )
{
	return TRUE;
}
//Ŭ�� ����Ʈ ���
int	bClan_InsertPoint( rsPLAYINFO *lpPlayInfo , int point )
{
	return TRUE;
}


DWORD nSprite_GetResult( DWORD nRecvNum )
{
	return TRUE;
}

#endif












