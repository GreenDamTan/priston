#include "common.h"

#include "smlib3d\\smd3d.h"

#include "smwsock.h"
#include "character.h"
#include "playmain.h"
#include "fileread.h"

#include "particle.h"
#include "sinbaram\\sinlinkheader.h"
#include "record.h"
#include "netplay.h"
#include "playsub.h"
#include "field.h"
#include "cracker.h"
#include "Language\\language.h"
#include "Language\\jts.h"
#include "TextMessage.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\help.h"
#include "damage.h"
#include "skillsub.h"

#include "AreaServer.h"

#include "BellatraFontEffect.h"
#include "Server\\onserver.h"

//######################################################################################
//�� �� �� : �� �� ��
#include "WinInt\\WinIntThread.h"
#include "WinInt\\WavIntHttp.h"
#include "WinInt\\ZipLib.h"
//######################################################################################

#include "CurseFilter.h"		//IsCurse �Լ�(�ؿ�)

#ifdef _XTRAP_GUARD_4_
#include ".\\XTrapSrcD5\\Client\\XTrap4Client.h"	//XTrapD5
//#include ".\\XTrapSrc4\\XTrap.h"
#endif

//����� - XignCode
#ifdef _XIGNCODE_CLIENT
#include "Xigncode\Client\zwave_sdk_client.h"
#endif

extern int WinSizeX;
extern int WinSizeY;
extern HWND hwnd;

//��ų��� �Լ���!!
//���� ��ų üũ
//int cSKILL::CancelContinueSkill(DWORD CODE)


//������ ����� (������ ĳ���� ���� �������� )
#define SERVER_CHAR_INFO

//�ڵ� ���� �ð� ( 5�� ���� )
#define RECORD_TIME_STEP		(5*60*1000)
//���� �ε� ��ٸ��� �ð� ( 10�� )
#define NET_LOADING_WAIT_LIMIT		10000

//ũ��Ƽ�� ���� ����
extern CRITICAL_SECTION	cSection;
extern rsRECORD_DBASE	rsRecorder;					//������ ���ӵ���Ÿ �����ġ
DWORD	dwLastRecordTime = 0;

CRITICAL_SECTION	cServSection;

int Debug_SendCount = 0;
int Debug_RecvCount1 = 0;
int Debug_RecvCount2 = 0;
int Debug_RecvCount3 = 0;

int NetWorkInitFlag = 0;
int	BellatraEffectInitFlag = 0;			//����Ʈ�� ����Ʈ �ʱ�ȭ �÷�
SBL_SetFontEffect SoD_SetFontEffect;	//SoD �� ��Ʈ ����Ʈ


DWORD	dwTime_ServerT = 0;		//���ӽ� �ð� (������ Time_T )
DWORD	dwTime_ConnectMS = 0;	//���ӽ� �ð� (Ŭ���̾�Ʈms)


//#define	CLIENT_VERSION_NUM		2235

//�߱�
#ifdef	_LANGUAGE_CHINESE
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		1031
#else
#define	CLIENT_VERSION_NUM		15031
#endif
#endif

//�Ϻ�
#ifdef	_LANGUAGE_JAPANESE

#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		4100
#else
#define	CLIENT_VERSION_NUM		15100
#endif
#endif

//�븸
#ifdef _LANGUAGE_TAIWAN
#define	CLIENT_VERSION_NUM		3003
#endif

#ifdef _LANGUAGE_ENGLISH
//#ifdef _LANGUAGE_PHILIPIN
//	#ifndef	_WINMODE_DEBUG
//	#define	CLIENT_VERSION_NUM		4060		//�ʸ���
//	#else
//	#define	CLIENT_VERSION_NUM		15060		//�ʸ���GM
//	#endif
//#else
	#ifndef	_WINMODE_DEBUG
	#define	CLIENT_VERSION_NUM		3122		//����
	#else
	#define	CLIENT_VERSION_NUM		15122		//����GM
#endif
//#endif

#endif

//��Ʈ��
#ifdef _LANGUAGE_VEITNAM
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		10565
#else
#define	CLIENT_VERSION_NUM		15565
#endif
#endif

//�±�
#ifdef _LANGUAGE_THAI
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		3223
#else
#define	CLIENT_VERSION_NUM		15223
#endif
#endif

//�����
#ifdef _LANGUAGE_BRAZIL
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		4135
#else
#define	CLIENT_VERSION_NUM		15135
#endif
#endif

//�Ƹ���Ƽ��
#ifdef _LANGUAGE_ARGENTINA
#ifndef	_WINMODE_DEBUG
#define	CLIENT_VERSION_NUM		4005
#else
#define	CLIENT_VERSION_NUM		15005
#endif
#endif

#ifdef _LANGUAGE_KOREAN
#define	CLIENT_VERSION_NUM		3000
#endif

#define CLIENT_VERSION_CHECK	(-CLIENT_VERSION_NUM*2)
/////////////////////////// ���� ���� ///////////////////////////////////////////
int	Client_Version = CLIENT_VERSION_NUM;		//���� ���� Ŭ���̾�Ʈ Ȯ�ο�
int	Server_LimitVersion = 167;					//���� ���� ���� Ȯ�ο�
/////////////////////////////////////////////////////////////////////////////////

int	Version_WareHouse = 1;						//â�� ���嵥��Ÿ ����
int	Version_CharInfo = 1;						//ĳ������Ÿ ���嵥��Ÿ ����


smWINSOCK	*smWsockServer =0;					//NPC-����-������
smWINSOCK	*smWsockDataServer =0;				//����Ÿ ���� ����
smWINSOCK	*smWsockUserServer =0;				//���� �÷��� ����Ÿ ���� ����
smWINSOCK	*smWsockExtendServer =0;			//�ʵ� Ȯ�� ����

//���� �翬�� Ƚ��
int	ReconnDataServer = 0;
int	ReconnServer = 0;

char	szConnServerName[16];					//������ �����̸�

DWORD	dwRecvUserServerTime = 0;
DWORD	dwRecvDataServerTime = 0;
DWORD	dwRecvServerTime = 0;
DWORD	dwRecvExtendServerTime = 0;

//�ְ� �ޱ� ����ȭ ����
int	rsRecvDataServer = TRUE;
int	rsRecvServer = TRUE;
int	rsRecvUserServer = TRUE;
int	rsRecvExtendServer = TRUE;

#define CHATBUFF_MAX		64
#define CHATBUFF_MASK		63

DWORD dwMyIP;

CHATBUFF ChatBuff[CHATBUFF_MAX];
CHATBUFF ChatBuff2[6][CHATBUFF_MAX];


int	ChatBuffCnt=0;
int	ChatBuffCnt2[6]= { 0,0,0,0,0,0 };
int ChatDispCnt=0;
int	ChatBuffCompCnt=0;

char *szDefaultServIP =	"211.50.44.170";

char *szNetLogFile = "debugnet.log";
FILE *fpNetLog = 0;

//�������� ���� ������
int	DisconnectFlag = 0;
int	DisconnectServerCode = 0;

//��ŷ ���
int	WarningHack = 0;

DWORD	dwQuestDelayTime = 0;		//����Ʈâ ������ Ÿ��

DWORD	dwLoadingTime = 0;			//�ε� �õ� �ð�

char szServIP[32];
DWORD dwServPort;
char szDataServIP[32];
DWORD dwDataServPort;
char szUserServIP[32];
DWORD dwUserServPort;
char szExtendServIP[32];
DWORD dwExtendServPort;



DWORD	dwConnectedClientTime =0;			//������ ����� Ŭ���̾�Ʈ �ð�
DWORD	dwConnectedServerTime =0;			//������ ����� ���� �ð�
DWORD	dwLastRecvGameServerTime =0;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�
DWORD	dwLastRecvGameServerTime2 =0;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�
DWORD	dwLastRecvGameServerTime3 =0;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�
DWORD	dwLastRecvGameServerTime4 =0;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�

typedef DWORD (*LPFN_CheckMem)( TRANS_FUNC_MEMORY *TransFuncMem , smTRANS_COMMAND	*lpTransCommand );
LPFN_CheckMem	fnChkMem2;

//����ġ�� �� �ŷ� �ݾ��� ��Ż�� ����Ѵ�
static int	CompWareHouseMoney = 0;
int	WareHouseSubMoney = 0;					//â���� ���� ���� �ݾ�
int TotalSubMoney = 0;						//���� ���� �ݾ�
int TotalAddMoney = 0;						//���� ���� �ݾ�
int	TotalAddExp = 0;						//���� ����ġ
int	TotalSubExp = 0;						//�پ�� ����ġ


smTRANS_COMMAND	*lpTransVirtualPotion = 0;


LPFN_CheckMem	fnChkMem;
DWORD	dwMemFunChkCode = 0;
//�޸� �˻� ��� ����
int	RecvMemFuncData( TRANS_FUNC_MEMORY *TransFuncMem );
int	RecvMemFuncData2( TRANS_FUNC_MEMORY *TransFuncMem );
//�������� �޸� ���� ���ؼ� ��������
DWORD funcCheckMemSum( DWORD FuncPoint , int count );
//���˻� �Ͽ� ������ ������
int CheckProcessModule();


//char *szServIP =	"211.50.44.170";
smPLAYDATA	PlayData;
char	TransBuff[smSOCKBUFF_SIZE];
int		TransLen;
//smCHAR_INFO	CharInfo;

//���� ������ ����
TRANS_ITEMINFO	TransThrowItem;
//�ֱ� ���� ������
TRANS_ITEMINFO	TransRecvItem;

//������ ��ȯŰ ��õ���
TRANS_TRADE_ITEMKEY	LastTransTradeItemKey;
DWORD				dwLastTransTradeKeyTime = 0;

int					InitClanMode = 0;			//Ŭ�� �ʱ�ȭ ���

DWORD				dwYahooTime = 0;			//��ȣ �����ð�

int					ServerHideMode = 0;			//���� ��������� ���


//////////////// PK �ʵ� ���� //////////////////
PK_FIELD_STATE	PK_FieldState;		//Pk �ʵ� ���� ����ü


//Ʈ���̵� ������ Ȯ�� ����ü
struct TRANS_TRADE_CHECKITEM	{
	int size,code;
	DWORD	dwSender;
	DWORD	dwRecver;
	DWORD	dwSum;

	DWORD	dwSendItemCode[MAX_TRADE_ITEM];
	DWORD	dwSendItemSum[MAX_TRADE_ITEM];

	DWORD	dwRecvItemCode[MAX_TRADE_ITEM];
	DWORD	dwRecvItemSum[MAX_TRADE_ITEM];
};


////////////////// ������ ��ȯ ���� ///////////////
int		TradeItemSucessFlag = 0;
int		TradeRecvItemKeyFlag = 0;
int		TradeSendSucessFlag = 0;
DWORD	dwTradeMaskTime = 0;
int		TradeItemCancelCount = 0;

TRANS_TRADEITEMS	TransLastRecvTrade;
/*
//���λ��� ����
struct	TRANS_MYSHOP_ITEM {
	int		size,code;
	DWORD	dwSender;
	DWORD	dwRecver;
	DWORD	dwSum;
	DWORD	Temp[4];

	sMYSHOP	sMyShop;
};
*/
TRANS_TRADEITEMS	TransTradeItems_MyShop;			//���λ��� ���� ����

/////////////////// â�� ///////////////////////////
DWORD	dwLastWareHouseChkSum = 0;

////////////// ����¡ ������ �����ۼ��� /////////////
int		TransAgingItemFlag;

smTRANS_COMMAND	TransServerConnectInfo;

//�ٸ� ������ ���� ��Ų��
int ConnectOtherPlayer( DWORD dwIP );

int HoRecvMessage( DWORD dwCode , void *RecvBuff );

//������ ��ȯ ����Ű ����
int RecvTradeSucessKey( TRANS_TRADE_ITEMKEY *lpTransTradeItemKey , sTRADE *lpTrade );
//������ ��ȯ ����Ű ����
int SendTradeSucessKey( sTRADE *lpTrade , DWORD dwSender );
//������ ��ȯ ���� Ȯ�� ����
int RecvTradeCheckItem( TRANS_TRADE_CHECKITEM *lpTransTradeCheckItem );
//ũ�� ������ �Ű�
int SendCrackWindow( HWND hWnd );

//�ӽ� ����� ���� �������� Ȯ���Ͽ� ��ġ�ϸ� ������ ����
int ThrowItemToServer( smTRANS_COMMAND_EX *lpTransCommand );
//������ Ȯ�� ��� ����
int RecvCheckItemFromServer( TRANS_ITEM_CODE *lpTransItemCode );
//Ŭ���̾�Ʈ ��� ��ġ�� ���� ������ ����
int	SendClientFuncPos();

//���μ��� �ð� �����ϴ� ������ ����
int OpenTimeCheckThread();
//���μ��� �ð� �������� ������ �ۺ�
int SendPlayTimerMax();


//�ӽ������ ������ ť���� ã��
TRANS_ITEMINFO	*FindRecvTransItemQue( DWORD dwCode , DWORD dwHead ,DWORD dwChkSum );
//������ ť�� �ӽ� ����
int	PushRecvTransItemQue( TRANS_ITEMINFO *lpTransItemInfo );

//��ų ���� ��Ŷ ����
int RecvProcessSkill( smTRANS_COMMAND *lpTransCommand );
//��Ƽ ��ų ��Ŷ ����
int RecvPartySkillFromServer( TRANS_PARTY_SKILL *lpTransPartySkill );

//����Ʈ ������ �Լ� ���͸�
int FiltQuestItem( TRANS_ITEMINFO	*lpTransItemInfo , DWORD dwPacketCode );
//��ũ �ھ� ��� ����
int SucessLinkCore( smTRANS_COMMAND_EX *lpTransCommandEx );
//SoD ���� ���� ����
int RecvSodGameInfomation( void *Info );

//�������� ������ �Լ�
int RecvForceOrbItem( TRANS_ITEMINFO_GROUP2 *lpTransItemGroup2 );

//����Ʈ�� ��Ŷ����
int XTrap_Recv( smTRANS_COMMAND *lpPacket , smWINSOCK *lpsmSock );

//���� ���� ĳ�� ����
rsBLESS_CASTLE	rsBlessCastleOld;

//�Ӽ����� �۽� �ð�
DWORD	dwResistance_SendingTime = 0;


//������ ���� �����ڵ� ���
#define	ATT_MONSTER_MAX	128
#define	ATT_MONSTER_MASK	(ATT_MONSTER_MAX-1)

DWORD	dwAttMonsterCodeList[ATT_MONSTER_MAX];
int		AttMonsterCodeCount = 0;

//���� �ڵ� ť�� ����
int ClearAttMonsterCode()
{
	int cnt;
	for(cnt=0;cnt<ATT_MONSTER_MAX;cnt++) {
		dwAttMonsterCodeList[cnt] = 0;
	}
	return TRUE;
}

int	AddAttMonsterCode( DWORD dwCode )
{
	int mcnt;
	if ( dwCode ) {
		mcnt = (AttMonsterCodeCount-1)&ATT_MONSTER_MASK;
		if ( dwAttMonsterCodeList[mcnt]==dwCode) return TRUE;
		mcnt = AttMonsterCodeCount&ATT_MONSTER_MASK;
		dwAttMonsterCodeList[mcnt]=dwCode;
		AttMonsterCodeCount++;
		return TRUE;
	}

	return FALSE;
}

int	CheckAttMonsterCode( DWORD dwCode )
{
	int cnt;
	for(cnt=0;cnt<ATT_MONSTER_MAX;cnt++) {
		if ( dwAttMonsterCodeList[cnt]==dwCode ) return TRUE;
	}
	return FALSE;
}


///////////////////// ������ ���� ////////////////////

	//(smpacket.h / record.h / netplay.h)

	//smTRANSCODE_ID_GETUSERINFO: -> smTRANSCODE_ID_SETUSERINFO
	//ID�� ����� ĳ���� ����Ʈ �� ������ ����

	//smTRANSCODE_INSRECORDDATA:
	//ID�� ���ο� ĳ���� ����

	//smTRANSCODE_DELRECORDDATA:
	//ID�� ���ο� ĳ���� ����

	//smTRANSCODE_ISRECORDDATA: -> smTRANS_CHAR_COMMAND2
	//ĳ���Ͱ� ���� �ϴ��� Ȯ��

//���� ���� ���� ��� �ڸ��
//int	TransUserCommand ( DWORD dwCode , char *szID , char *szName )

///////////////////////////////////////////////////////

///////////////////// �ޱ� ���� ////////////////////
	//smTRANSCODE_ID_SETUSERINFO:
	//ID�� ����� ĳ���� ����Ʈ �� ������ ���� 
	//(TRANS_USERCHAR_INFO ����ü�� ��� ���� )

	//smTRANSCODE_ISRECORDDATA:
	//ĳ���Ͱ� ���� �ϴ��� Ȯ�� 
	//( smTRANS_CHAR_COMMAND2 ->wParam �� ��� ���� )
///////////////////////////////////////////////////////

//���� ����


//smWINSOCK *ConnectServer_Main();

//���� ���� Ȯ��
int CheckServerInfo( smTRANS_COMMAND *lpTransCommand )
{
	if ( (-Client_Version*2)!=CLIENT_VERSION_CHECK ) {
		//������ ���� ���� ���� �Ұ�
		SetGameError( 1 );
		return TRUE;
	}

	if ( Client_Version<lpTransCommand->WParam ) {
		//������ ���� ���� ���� �Ұ�
		SetGameError( 1 );
		return TRUE;
	}
	if ( lpTransCommand->LParam>0 ) {
		//�ο��� ���Ƽ� ���� �Ұ�
		SetGameError( 2 );
		return TRUE;
	}
	SetGameError( 0 );
	return TRUE;
}

//���μ��� ���� ����
int SendProcessInfo()
{
	TRANS_CHATMESSAGE	smTransChatMessage;

	if ( smWsockDataServer ) 
	{
		smTransChatMessage.code = smTRANSCODE_PROCESSINFO;
		smTransChatMessage.dwIP = dwExeCheckSum;
		smTransChatMessage.dwObjectSerial = dwExeCheckSum;

		lstrcpy( smTransChatMessage.szMessage , szProcessPath );
		smTransChatMessage.size = 32+lstrlen( szProcessPath );

		return smWsockDataServer->Send2( (char *)&smTransChatMessage , smTransChatMessage.size , TRUE );
	}
	return FALSE;
}


//�� �÷��� ����
smWINSOCK *ConnectServer_Main()
{
	/*
	//�׽�Ʈ ����
	smConfig.szServerIP[0] = 0;
	smConfig.szDataServerIP[0] = 0;
	smConfig.szUserServerIP[0] = 0;
	*/

	if ( !smWsockServer ) 
	{
		//���� ���� ����
		if ( smConfig.szServerIP[0] )
			lstrcpy( szServIP , smConfig.szServerIP );
		else
			lstrcpy( szServIP , szDefaultServIP );

		if ( smConfig.dwServerPort )
			dwServPort =  smConfig.dwServerPort;
		else
			dwServPort = TCP_SERVPORT;

		smWsockServer = smConnectSock( szServIP , (WORD)dwServPort );
	}

	if ( smWsockServer && !smWsockDataServer )
	{
		//����Ÿ ���� ����
		if ( smConfig.szDataServerIP[0] && lstrcmpi( smConfig.szDataServerIP , smConfig.szServerIP )!=0 ) 
		{
			lstrcpy( szDataServIP , smConfig.szDataServerIP );
			dwDataServPort =  smConfig.dwDataServerPort;
			smWsockDataServer = smConnectSock( szDataServIP , (WORD)dwDataServPort );
		}
		else 
		{
			//����Ÿ ������ ���Ӽ����� ���� ���
			lstrcpy( szDataServIP , szServIP );
			dwDataServPort =  dwServPort;
			smWsockDataServer = smWsockServer;
		}
	}

	if ( smWsockServer && smWsockDataServer && !smWsockUserServer ) 
	{
		//���� ���� ����
		if ( smConfig.szUserServerIP[0] && lstrcmpi( smConfig.szUserServerIP , smConfig.szServerIP )!=0 ) {
			lstrcpy( szUserServIP , smConfig.szUserServerIP );
			dwUserServPort =  smConfig.dwUserServerPort;
			smWsockUserServer = smConnectSock( szUserServIP , (WORD)dwUserServPort );
		}
		else {
			//���� ������ ���Ӽ����� ���� ���
			lstrcpy( szUserServIP , szServIP );
			dwUserServPort =  dwServPort;
			smWsockUserServer = smWsockServer;
		}
	}

	if ( smWsockServer && smWsockDataServer && !smWsockExtendServer )
	{
		//Ȯ�� ���� ����
		if ( smConfig.szExtendServerIP[0] && lstrcmpi( smConfig.szExtendServerIP , smConfig.szServerIP )!=0 ) {
			lstrcpy( szExtendServIP , smConfig.szExtendServerIP );
			dwExtendServPort =  smConfig.dwExtendServerPort;
			smWsockExtendServer = smConnectSock( szExtendServIP , (WORD)dwExtendServPort );
		}
		else 
		{
			//Ȯ�� ������ ���Ӽ����� ���� ���
			lstrcpy( szExtendServIP , szServIP );
			dwExtendServPort =  dwServPort;
			smWsockExtendServer = smWsockServer;
		}
	}


	if ( smWsockServer && smWsockDataServer && smWsockUserServer )
	{

		if ( smConfig.DebugMode ) {
			//fpNetLog = fopen( szNetLogFile , "wb" );
		}

		return smWsockServer;
	}
	
	return NULL;
}

//�������� ���� ��� ����
int DisconnectServerFull()
{

	if ( smWsockDataServer && smWsockDataServer!=smWsockServer ) 
	{
		smWsockDataServer->CloseSocket();
		smWsockDataServer = 0;
	}
	if ( smWsockUserServer && smWsockUserServer!=smWsockServer ) 
	{
		smWsockUserServer->CloseSocket();
		smWsockUserServer = 0;
	}
	if ( smWsockExtendServer && smWsockExtendServer!=smWsockServer ) 
	{
		smWsockExtendServer->CloseSocket();
		smWsockExtendServer = 0;
	}
	if ( smWsockServer ) 
	{
		smWsockServer->CloseSocket();
		smWsockServer = 0;
	}

	DisconnectFlag = 0;

	return TRUE;
}
/*
//���� ���� ����
smWINSOCK *ConnectServer_InfoMain();
//���� ���� ����
smWINSOCK *ConnectServer_GameMain( char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2 );
//���� ���� ���� ����
int DisconnectServer_GameMain();

*/

//���� ���� ����
smWINSOCK *ConnectServer_InfoMain()
{
	if ( !smWsockDataServer )
	{
		//����Ÿ ���� ����
		if ( smConfig.szDataServerIP[0] ) 
		{
			dwDataServPort =  TCP_SERVPORT;
			smWsockDataServer = smConnectSock( smConfig.szDataServerIP , (WORD)dwDataServPort );
			ZeroMemory( &TransServerConnectInfo , sizeof( smTRANS_COMMAND ) );
			return smWsockDataServer;
		}
	}


	return NULL;
}

//���� ���� ����
smWINSOCK *ConnectServer_GameMain( char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2 )
{
	int ServerInfoFlag = 0;

	lstrcpy( smConfig.szServerIP , szIP1 );
	lstrcpy( smConfig.szUserServerIP , szIP2 );

	if ( smWsockServer && lstrcmp( smWsockServer->szIPAddr , szIP1 )!=0 ) {
		DisconnectServer_GameMain();
	}
	if ( smWsockUserServer && lstrcmp( smWsockUserServer->szIPAddr , szIP2 )!=0 ) {
		DisconnectServer_GameMain();
	}

	if ( !smWsockServer )
	{
		//���� ���� ����
		if ( smWsockDataServer && lstrcmp( smWsockDataServer->szIPAddr , szIP1 )==0 && dwDataServPort==dwPort1 ) 
		{
			smWsockServer = smWsockDataServer;
			dwServPort = dwDataServPort;

			ServerInfoFlag = TRUE;
		}
		else 
		{
			smWsockServer = smConnectSock( szIP1 , (WORD)dwPort1 );
			dwServPort = dwPort1;
		}
		lstrcpy( smConfig.szServerIP , szIP1 );
	}

	if ( !smWsockUserServer )
	{
		if ( smWsockServer && lstrcmp( smWsockServer->szIPAddr , szIP2 )==0 && dwServPort==dwPort2 )
		{

			smWsockUserServer = smWsockServer;
			dwUserServPort = dwServPort;
		}
		else 
		{
			smWsockUserServer = smConnectSock( szIP2 , (WORD)dwPort2 );
			dwUserServPort = dwPort2;
		}
		lstrcpy( smConfig.szUserServerIP , szIP2 );
	}

	
	if ( smWsockServer && smWsockUserServer ) 
	{
		if ( ServerInfoFlag ) {
			//���� ���� Ȯ��
			CheckServerInfo( &TransServerConnectInfo );
		}

		return smWsockServer;
	}
	return NULL;
}

//���� ���� ����
smWINSOCK *ConnectServer_GameMain( char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2 , char *szIP3, DWORD dwPort3 )
{
	int ServerInfoFlag = 0;

	lstrcpy( smConfig.szServerIP , szIP1 );
	lstrcpy( smConfig.szUserServerIP , szIP2 );
	lstrcpy( smConfig.szExtendServerIP , szIP3 );


	if ( smWsockServer && lstrcmp( smWsockServer->szIPAddr , szIP1 )!=0 ) {
		DisconnectServer_GameMain();
	}
	if ( smWsockUserServer && lstrcmp( smWsockUserServer->szIPAddr , szIP2 )!=0 ) {
		DisconnectServer_GameMain();
	}
	if ( smWsockExtendServer && lstrcmp( smWsockExtendServer->szIPAddr , szIP3 )!=0 ) {
		DisconnectServer_GameMain();
	}

	if ( !smWsockServer ) 
	{
		//���� ���� ����
		if ( smWsockDataServer && lstrcmp( smWsockDataServer->szIPAddr , szIP1 )==0 && dwDataServPort==dwPort1 ) 
		{
			smWsockServer = smWsockDataServer;
			dwServPort = dwDataServPort;

			ServerInfoFlag = TRUE;
		}
		else 
		{
			smWsockServer = smConnectSock( szIP1 , (WORD)dwPort1 );
			dwServPort = dwPort1;
		}
		lstrcpy( smConfig.szServerIP , szIP1 );
	}

	if ( !smWsockUserServer )
	{
		if ( smWsockServer && lstrcmp( smWsockServer->szIPAddr , szIP2 )==0 && dwServPort==dwPort2 )
		{
			smWsockUserServer = smWsockServer;
			dwUserServPort = dwServPort;
		}
		else
		{
			smWsockUserServer = smConnectSock( szIP2 , (WORD)dwPort2 );
			dwUserServPort = dwPort2;
		}
		lstrcpy( smConfig.szUserServerIP , szIP2 );
	}

	if ( !smWsockExtendServer ) 
	{
		if ( (smWsockServer && lstrcmp( smWsockServer->szIPAddr , szIP3 )==0 && dwServPort==dwPort3) || !szIP3[0] )
		{
			smWsockExtendServer = smWsockServer;
			dwExtendServPort = dwServPort;
		}
		else 
		{
			smWsockExtendServer = smConnectSock( szIP3 , (WORD)dwPort3 );
			dwExtendServPort = dwPort3;
		}
		lstrcpy( smConfig.szExtendServerIP , szIP3 );
	}

	if ( smWsockServer && smWsockUserServer && smWsockExtendServer) 
	{
		if ( ServerInfoFlag )
		{
			//���� ���� Ȯ��
			CheckServerInfo( &TransServerConnectInfo );
		}

		return smWsockServer;
	}
	return NULL;
}


//���� ���� ���� ����
int DisconnectServer_GameMain()
{
	if ( smWsockExtendServer && smWsockExtendServer!=smWsockServer ) 
	{
		if ( smWsockDataServer!=smWsockExtendServer )
		{
			smWsockExtendServer->CloseSocket();
		}
	}
	smWsockExtendServer = 0;

	if ( smWsockUserServer && smWsockUserServer!=smWsockServer ) 
	{
		if ( smWsockDataServer!=smWsockUserServer ) 
		{
			smWsockUserServer->CloseSocket();
		}
	}
	smWsockUserServer = 0;


	if ( smWsockServer && smWsockDataServer!=smWsockServer )
	{
		if ( smWsockDataServer!=smWsockServer )
		{
			smWsockServer->CloseSocket();
		}
	}
	smWsockServer = 0;

	DisconnectFlag = 0;

	return TRUE;
}

//���� �ڵ�� ������ ã��
smWINSOCK *GetServerSock( int ServerCode )
{
	switch( ServerCode )
	{
	case PLAY_SERVER_CODE_EXTEND:
		if ( smWsockExtendServer && smWsockServer!=smWsockExtendServer )
			return smWsockExtendServer;
		else
			return smWsockUserServer;

	case PLAY_SERVER_CODE_USER:
		return smWsockUserServer;

	case PLAY_SERVER_CODE_MAIN:
		return smWsockServer;

	case PLAY_SERVER_CODE_AREA1:
		return lpWSockServer_DispArea[0];

	case PLAY_SERVER_CODE_AREA2:
		return lpWSockServer_DispArea[1];

	}

	return NULL;
}

//�������� ������ ã��
smWINSOCK *GetAreaServerSock()
{
	int ServerCode;

	if ( lpCurPlayer->OnStageField>=0 ) 
	{
		if ( AreaServerMode ) 
		{
			return lpWSockServer_DispArea[lpCurPlayer->OnStageField];
		}
		else 
		{
			ServerCode=StageField[ lpCurPlayer->OnStageField ]->ServerCode;
			return GetServerSock(ServerCode);
		}
	}

	return NULL;
}


//������ ������ ã��
scITEM *FindScItem( int x, int z )
{
	int cnt;

	for( cnt=0;cnt<DISP_ITEM_MAX;cnt++) 
	{
		if ( scItems[cnt].Flag && scItems[cnt].pX==x && scItems[cnt].pZ==z )
		{
			return &scItems[cnt];
		}
	}

	return NULL;
}

//�� ������ ����ã��
scITEM *FindEmptyScItem()
{
	int cnt;

	for( cnt=0;cnt<DISP_ITEM_MAX;cnt++) {
		if ( !scItems[cnt].Flag ) return &scItems[cnt];
	}

	return NULL;
}

//��ȯ �ŷ� �Ÿ� Ȯ��
int GetTradeDistance( smCHAR *lpChar )
{
	int	x,y,z;
	int dist;

	x = (lpCurPlayer->pX - lpChar->pX)>>FLOATNS;
	y = (lpCurPlayer->pY - lpChar->pY)>>FLOATNS;
	z = (lpCurPlayer->pZ - lpChar->pZ)>>FLOATNS;

	dist = x*x+y*y+z*z;

	if ( dist<(256*256) ) return TRUE;

	return FALSE;
}

//��ȯ �ŷ� �Ÿ� Ȯ��
int GetTradeDistanceFromCode( DWORD dwObjectSerial )
{
	smCHAR *lpChar;

	lpChar = FindAutoPlayer( dwObjectSerial );

	if ( lpChar && lpChar->DisplayFlag && lpChar->smCharInfo.szName[0] ) {

		return  GetTradeDistance( lpChar );

	}
	return FALSE;
}

//���θ��� ĳ������ Ȯ��
int	CheckStartCharInfo()
{
	//������ ���� �ѹ��� Ȯ��
	smCHAR_INFO	smCharInfo;
	int cnt;

	memcpy( &smCharInfo , sinChar , sizeof(smCHAR_INFO) );

	CheckCharForm();
	CodeXorExp = dwPlayTime+smCharInfo.Exp;						//����ġ ���� �ڵ�
	CodeXorExp_High = dwPlayTime|(dwPlayTime*dwPlayTime);						//����ġ ���� �ڵ�
	CodeXorLife = (dwPlayTime*smCharInfo.Life[0])&0xFFFF;		//����� ���� �ڵ�
	cnt = smCharInfo.Dexterity+smCharInfo.Health+smCharInfo.Spirit+smCharInfo.Strength+smCharInfo.Talent;

	if ( smCharInfo.Level>=2 || cnt>102 || smCharInfo.Exp || smCharInfo.ChangeJob || smCharInfo.Money ) 
	{
		//ĳ���� �����߻� ������Ȥ
		SendSetHackUser2( 6000 , smCharInfo.Money );
		WarningHack = TRUE;
		return FALSE;
	}

	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;

	lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
	ReformCharForm();

	return TRUE;
}

//�޸� �˻� ��� ����2
int	RecvMemFuncData2( TRANS_FUNC_MEMORY *TransFuncMem )
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = -1;
	fnChkMem2 = (LPFN_CheckMem)((void *)TransFuncMem->szData);
	fnChkMem2( TransFuncMem , &smTransCommand );

	ZeroMemory( TransFuncMem , 1500 );

	if ( smTransCommand.size>0 && smWsockDataServer ) 
	{
		smTransCommand.code = smTRANSCODE_FUNCTION_MEM2;
		smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}

	return TRUE;
}



sSERVER_MONEY	sServerMoney[3] = { {0,0,0,0,0},{0,0,0,0,0} };
sSERVER_EXP		sServerExp[3] = { {0,0,0,0},{0,0,0,0} };
DWORD			dwLastSendTotalExpMoenyTime = 0;

//���� ����ġ ������Ÿ ��ġ�� ������ ������ Ȯ��
int	SendTotalExpMoney()
{
	TRANS_TOTAL_EXPMONEY	TransTotalExpMoney;

	if ( (dwLastSendTotalExpMoenyTime+30000)>dwPlayTime ) return FALSE;

	TransTotalExpMoney.code = smTRANSCODE_CHECK_EXPMONEY;
	TransTotalExpMoney.size = sizeof( TRANS_TOTAL_EXPMONEY );

	if ( smWsockServer ) {
		memcpy( &TransTotalExpMoney.sServerExp , &sServerExp[0] , sizeof(sSERVER_EXP) );
		memcpy( &TransTotalExpMoney.sServerMoney , &sServerMoney[0] , sizeof(sSERVER_MONEY) );
		smWsockServer->Send2( (char *)&TransTotalExpMoney , TransTotalExpMoney.size , TRUE );
	}
	if ( smWsockUserServer && smWsockUserServer!=smWsockServer ) {
		memcpy( &TransTotalExpMoney.sServerExp , &sServerExp[1] , sizeof(sSERVER_EXP) );
		memcpy( &TransTotalExpMoney.sServerMoney , &sServerMoney[1] , sizeof(sSERVER_MONEY) );
		smWsockUserServer->Send2( (char *)&TransTotalExpMoney , TransTotalExpMoney.size , TRUE );
	}
	if ( smWsockExtendServer && smWsockExtendServer!=smWsockServer ) {
		memcpy( &TransTotalExpMoney.sServerExp , &sServerExp[2] , sizeof(sSERVER_EXP) );
		memcpy( &TransTotalExpMoney.sServerMoney , &sServerMoney[2] , sizeof(sSERVER_MONEY) );
		smWsockExtendServer->Send2( (char *)&TransTotalExpMoney , TransTotalExpMoney.size , TRUE );
	}

	dwLastSendTotalExpMoenyTime = dwPlayTime;

	return TRUE;
}

//�������� ���� ���� �� Ȯ��
int CheckServerMoney( smWINSOCK *lpsmSock , TRANS_ITEMINFO	*lpTransItemInfo )
{
	sSERVER_MONEY	*lpServerMoney;

	lpServerMoney = 0;

	if ( lpsmSock==smWsockServer ) {
		lpServerMoney = &sServerMoney[0];
	}
	else {
		if ( lpsmSock==smWsockUserServer ) {
			lpServerMoney = &sServerMoney[1];
		}
		else if ( lpsmSock==smWsockExtendServer ) {
			lpServerMoney = &sServerMoney[2];
		}
	}

	if ( lpServerMoney ) {
		lpServerMoney->InputMoney += lpTransItemInfo->Item.Money;
		lpServerMoney->Counter++;
		lpServerMoney->TotalX = lpTransItemInfo->x;
		lpServerMoney->TotalY = lpTransItemInfo->y;
		lpServerMoney->TotalZ = lpTransItemInfo->z;

/*
					TransItemInfo.x = dwPlayServTime;											//�ص� Ű
					TransItemInfo.y = TransItemInfo.Item.ItemHeader.dwChkSum;					//�ص� Ű2
					TransItemInfo.z = rsPlayInfo[cnt].spMoney_Out^(TransItemInfo.x+TransItemInfo.y);	//��Ż ��� ����
*/

		int cnt;
		cnt = lpServerMoney->TotalZ^(lpServerMoney->TotalX+lpServerMoney->TotalY);
		cnt += 200;
		if ( cnt<lpServerMoney->InputMoney ) {
			//SendSetHackUser( 82 );
		}
	}

	return TRUE;
}

//�������� ���� ����ġ�� �� Ȯ��
int CheckServerExp( smWINSOCK *lpsmSock , smTRANS_COMMAND	*lpTransCommand )
{
	sSERVER_EXP	*lpServerExp;

	lpServerExp = 0;

	if ( lpsmSock==smWsockServer ) 
	{
		lpServerExp = &sServerExp[0];
	}
	else 
	{
		if ( lpsmSock==smWsockUserServer ) 
		{
			lpServerExp = &sServerExp[1];
		}
		else if ( lpsmSock==smWsockExtendServer ) 
		{
			lpServerExp = &sServerExp[2];
		}
	}

	if ( lpServerExp ) 
	{
		lpServerExp->InputExp += lpTransCommand->WParam;
		lpServerExp->Counter++;
		lpServerExp->Total1 = lpTransCommand->SParam;
		lpServerExp->Total2 = lpTransCommand->EParam;

		// �庰 - ����ġ ���� ���� (800000 - > 1600000 = ����ġ 2�� �̺�Ʈ)
		int cnt = (lpServerExp->Total1^lpServerExp->Total2) + 1600000;		//������ ����
		if ( cnt<lpServerExp->InputExp ) 
		{
			SendSetHackUser( 83 );
		}

	}

	return TRUE;
}

//�ٸ� �÷��̾���� ���� �����带 ����
int CreateConnectThread( DWORD dwIP );
//�ٸ� �÷��̾���� �������� ������ ����
int CreateDisconnectThread( smWINSOCK *lpsmSock );

//�ٸ� �ٸ� �÷��̾��� �� ������ ã��
int FindNewOtherPlayer()
{
	int cnt;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( !chrOtherPlayer[cnt].Flag )
			return cnt;
	}

	return -1;
}

//�ڵ� �÷��̾ ã�´�
smCHAR *FindAutoPlayer( DWORD dwObjectSerial )
{
	int cnt;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial==dwObjectSerial ) {
			return &chrOtherPlayer[cnt];
		}
	}

	return NULL;
}

// ������ȣ�� ã�´�
smCHAR *FindChrPlayer( DWORD dwObjectSerial )
{
	int cnt;

	if ( lpCurPlayer->dwObjectSerial==dwObjectSerial ) 
		return lpCurPlayer ;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial==dwObjectSerial ) {
			return &chrOtherPlayer[cnt];
		}
	}

	return NULL;
}

//������Ƽ������ ã�´�
smCHAR *FindDeadPartyUser()
{
	int cnt;
	smCHAR *lpChar;
	int dist,x,y,z;
	int	NearDist;

	lpChar = 0;
	NearDist = 300*300;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].PartyFlag && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_USER ) {
			if (chrOtherPlayer[cnt].MotionInfo->State==CHRMOTION_STATE_DEAD ) {

				x = (lpCurPlayer->pX-chrOtherPlayer[cnt].pX)>>FLOATNS;
				y = (lpCurPlayer->pY-chrOtherPlayer[cnt].pY)>>FLOATNS;
				z = (lpCurPlayer->pZ-chrOtherPlayer[cnt].pZ)>>FLOATNS;

				dist = x*x+z*z+y*y;

				if ( dist<NearDist ) {
					lpChar = &chrOtherPlayer[cnt];
					NearDist = dist;
				}
			}
		}
	}

	return lpChar;
}

//�ֱ��� ���͸� ã�´�
smCHAR *FindNearMonster( smCHAR *lpCurChar )
{
	int cnt;
	smCHAR *lpChar;
	int dist,x,y,z;
	int	NearDist;

	lpChar = 0;
	NearDist = 300*300;

	if ( !lpCurChar ) return NULL;

	if ( lpCurChar!=lpCurPlayer ) {
		x = (lpCurChar->pX-lpCurPlayer->pX)>>FLOATNS;
		y = (lpCurChar->pY-lpCurPlayer->pY)>>FLOATNS;
		z = (lpCurChar->pZ-lpCurPlayer->pZ)>>FLOATNS;

		dist = x*x+z*z+y*y;
		if ( dist<NearDist ) {
			if ( !lpCurChar->smCharInfo.ClassClan || lpCurChar->smCharInfo.ClassClan!=lpCurPlayer->smCharInfo.ClassClan ) {
				lpChar = lpCurPlayer;
				NearDist = dist;
			}
		}
	}

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_ENEMY && lpCurChar!=&chrOtherPlayer[cnt] ) {
			if (chrOtherPlayer[cnt].smCharInfo.Brood!=smCHAR_MONSTER_USER && chrOtherPlayer[cnt].smCharInfo.Life[0]>0 &&
				(!chrOtherPlayer[cnt].smCharInfo.ClassClan || chrOtherPlayer[cnt].smCharInfo.ClassClan!=lpCurChar->smCharInfo.ClassClan ) ) 
			{	
				x = (lpCurChar->pX-chrOtherPlayer[cnt].pX)>>FLOATNS;
				y = (lpCurChar->pY-chrOtherPlayer[cnt].pY)>>FLOATNS;
				z = (lpCurChar->pZ-chrOtherPlayer[cnt].pZ)>>FLOATNS;

				dist = x*x+z*z+y*y;

				if ( dist<NearDist ) {
					lpChar = &chrOtherPlayer[cnt];
					NearDist = dist;
				}
			}
		}
	}

	return lpChar;
}

//�� �÷��̾ ã�´�
smCHAR *FindEmptyChar()
{
	int cnt;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( !chrOtherPlayer[cnt].Flag )
			return &chrOtherPlayer[cnt];
	}

	return NULL;
}

//ä��â�� ���� ����
int AddChatBuff( char *szMsg , DWORD dwIP )
{
	int ChatCnt = ChatBuffCnt & CHATBUFF_MASK;
	ChatBuff[ ChatCnt ].dwIP = dwIP;
	lstrcpy( ChatBuff[ ChatCnt ].szMessage , szMsg );
	ChatBuffCnt++;
	ChatDispCnt = 70*10;
	if ( ChatScrollPoint )
		ChatScrollPoint++;

	return ChatBuffCnt;
}

//ä��â�� ���� ����
int AddChatBuff( char *szMsg )
{
	return AddChatBuff( szMsg , 0 );
}

int	ChatBuffFilter( char *szMsg , DWORD dwIP )
{
	int cnt,len,cnt2;
	char	szBuff[256];
	char	szBuff2[64];

	if ( dwIP>=256 ) return TRUE;

	len = lstrlen( szMsg );

	for ( cnt=0;cnt<len;cnt++ ) {
		if ( szMsg[cnt]=='>' ) {
			szBuff[cnt]=0;
			break;
		}
		else {
			szBuff[cnt] = szMsg[cnt];
		}
	}

	if ( cnt<len ) {
		if ( lstrcmpi( szBuff , "From" )==0 ) {
			cnt2=0;
			szBuff[0] = 0;
			for(cnt=cnt+1;cnt<len;cnt++) {
				if ( szMsg[cnt]==':' ) {
					szBuff[cnt2] = 0;
					break;
				}
				else {
					if ( szMsg[cnt]!=' ' )
						szBuff[cnt2++] = szMsg[cnt];
				}
			}
			if ( cnt<len  && szBuff[0] ) {

				for(cnt2=0;cnt2<15;cnt2++) {
					//0~15
					if ( InterfaceParty.chat_IDs( 2, cnt2, szBuff2 )==TRUE ) {
						//id�� ��������.(���ӿ��θ� �˷��ֱ� ������)
						//�Ӹ� �ź��� ���� Ȯ��
						if ( lstrcmpi( szBuff , szBuff2 )==0 ) {
							//char *mgRefuseWhisper = "�Ӹ��� �źεǾ����ϴ�";
							wsprintf( szBuff , "/** %s %s" , szBuff2 , mgRefuseWhisper );
							SendChatMessageToServer( szBuff );
							return FALSE;
						}
					}
				}

				InterfaceParty.latest_Insert(szBuff);	//�ӼӸ��ѻ���� ����Ʈ �߰�.
			}

			return TRUE;
		}

		if ( lstrcmpi( szBuff , "To" )==0 ) {
			cnt2=0;
			szBuff[0] = 0;
			for(cnt=cnt+1;cnt<len;cnt++) {
				if ( szMsg[cnt]==':' ) {
					szBuff[cnt2] = 0;
					break;
				}
				else {
					if ( szMsg[cnt]!=' ' )
						szBuff[cnt2++] = szMsg[cnt];
				}
			}
			if ( cnt<len && szBuff[0] ) {
				InterfaceParty.latest_Insert(szBuff);	//�ӼӸ��ѻ���� ����Ʈ �߰�.
			}
			return TRUE;
		}
	}





	return TRUE;
}


//�ý��� �ð��� ���ӽð����� ��ȯ
DWORD	ConvSysTimeToGameTime(	DWORD dwTime )
{
	return dwTime/GAME_WORLDTIME_MIN;
}

//DispLoading

#define RECV_DATA_QUE_COUNT		64
#define RECV_DATA_QUE_MASK		63


class	rsTRANS_SERVER {
	char	TransBuff[smSOCKBUFF_SIZE];

	rsRECORD_DBASE	*lpRecorder;


	smTHREADSOCK	*RecvDataQue[RECV_DATA_QUE_COUNT];
	int				RecvDataQuePush;
	int				RecvDataQuePop;

	TRANS_PLAYPOS	TransPlayPosLast;
	int				TransPlayPosCount;

public:
	smWINSOCK	*lpsmSock;


	//Ŭ���� �ʱ�ȭ
	int	Init( smWINSOCK *lpsmsock );
	//������ �޼��� ����
	int SendChat( char *szMessage );
	//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
	int SendPlayData( smCHAR *lpChar );

	//������ ���� ���� �޼��� ó��
	int RecvMessage( smTHREADSOCK *SockInfo );

	//�޼��� ť�� �޼����� ó��
	int RecvMessageQue();

};

/*
		case smTRANSCODE_ATTACKDATA:
		case smTRANSCODE_PLAYDATA1:
		case smTRANSCODE_PLAYDATA2:
		case smTRANSCODE_PLAYDATA3:
		case smTRANSCODE_TRANSPLAYDATA:
		case smTRANSCODE_PLAYDATAGROUP:
		case smTRANSCODE_PLAYITEM:
*/

//lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_STAND
//lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_WALK
//lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_RUN


//�޼��� ť�� �޼����� ó��
int rsTRANS_SERVER::RecvMessageQue()
{
	int cnt;

	if ( RecvDataQuePop>=RecvDataQuePush ) return TRUE;
	if ( MessageLoading ) return FALSE;

	//ũ��ƼĮ ���� ����
	EnterCriticalSection( &cSection );

	while(1) {
		if ( RecvDataQuePop>=RecvDataQuePush ) break;

		cnt = RecvDataQuePop & RECV_DATA_QUE_MASK;

		if ( RecvDataQue[cnt] ) {
			RecvMessage( RecvDataQue[cnt] );

			delete RecvDataQue[cnt];
			RecvDataQue[cnt] = 0;
		}

		RecvDataQuePop++;
	}

	//ũ��ƼĮ ���� ����
	LeaveCriticalSection( &cSection );

	return TRUE;
}


DWORD	dwGetCharInfoTime = 0;

//������ ���� ���� �޼��� ó��
int rsTRANS_SERVER::RecvMessage( smTHREADSOCK *pData )
{
	DWORD *IPData;
//	smCHAR *lpPlayer;

//	int action;
//	int DataCnt;
	int cnt;
	int x,z;
	int ServerCode;
//	int mx,my,mz;
//	int dDist;
	smCHAR	*lpChar;

//	smWINSOCK *lpsmFindSock;
	int ChatCnt;
//	int	TempBuff[8];
	int RcvCnt;


	TRANS_ITEMINFO				*lpTransItemInfo;
	//TRANS_AREAINFO_HEADER	*lpAreaInfoHeader;
//	TRANS_IPLIST			*lpTransIp;
	smPLAYDATA					*lpPlayData1;
	smTRANS_COMMAND				TransCommand;
	smTRANS_COMMAND_EX			TransCommandEx;
	smTRANS_COMMAND				*lpTransCommand;
	smTRANS_COMMAND_EX			*lpTransCommandEx;
	smTRANS_ACITON_ITEM			*lpTransActionItem;
	smTRANS_CHAR_COMMAND		*lpTransCharCommand;
	smTRNAS_PLAYERINFO			*lpTransPlayerInfo;
	TRANS_CHATMESSAGE			*lpTransChatMessage;
	TRANS_POST_ITEM				*lpTransPostItem;


	//smTRANS_SELL_ITEM		*lpTransSellItem;
	DWORD	dwTime;
	DWORD	dwDataKey;
	smCHAR_INFO	*lpCharInfo_MsgBack = 0;
	int		addExp , addMoney;
	DWORD	dwEncCode;
	//char	szBuff[128];

	IPData = (DWORD *)( pData->Buff );

	if ( fpNetLog ) 
	{
		char szLogMsg[128];
		wsprintf( szLogMsg , "Recv %x %d\r\n" , IPData[1] , IPData[0] );
		fwrite( szLogMsg , lstrlen( szLogMsg ) , 1, fpNetLog);
	}


	if ( MessageLoading ) 
	{
		switch ( IPData[1] )
		{
			//�����ص� �Ǵ� ����Ÿ
			case smTRANSCODE_ATTACKDATA2:
			case smTRANSCODE_ATTACKDATA:
			case smTRANSCODE_PLAYDATA1:
			case smTRANSCODE_PLAYDATA2:
			case smTRANSCODE_PLAYDATA3:
			case smTRANSCODE_TRANSPLAYDATA:
			case smTRANSCODE_PLAYDATAGROUP:
			case smTRANSCODE_PLAYITEM:
				return TRUE;
		}

		//����Ÿ �ε� ���� / ť�� ���� �ߴٰ� ���߿� ó��
		cnt = RecvDataQuePush-RecvDataQuePop;

		//ť ���� ����
		if ( cnt>=RECV_DATA_QUE_COUNT ) return FALSE;

		cnt = RecvDataQuePush & RECV_DATA_QUE_MASK;

		if ( !RecvDataQue[cnt] )
		{
			RecvDataQue[cnt] = new smTHREADSOCK;
			if ( RecvDataQue[cnt] ) 
			{
				memcpy( RecvDataQue[cnt] , pData , sizeof(smTHREADSOCK) );
				RecvDataQuePush++;
				return TRUE;
			}
		}
		return FALSE;
	}

	dwEncCode = (IPData[1]&0xFFFF0000);
	if ( dwEncCode==smTRANSCODE_ENCODE_PACKET || dwEncCode==smTRANSCODE_ENCODE_PACKET2 )
	{
		smTHREADSOCK smEncData;

		addExp = 0;
		addMoney = 0;

		memcpy( &smEncData , pData , sizeof(smTHREADSOCK)-smSOCKBUFF_SIZE );

		if ( dwEncCode==smTRANSCODE_ENCODE_PACKET2 ) {
			if ( fnDecodePacket )
				dwDataKey = fnDecodePacket( (BYTE *)pData->Buff , (BYTE *)smEncData.Buff );
			else
				return FALSE;
		}
		else
			dwDataKey = DecodePacket( (BYTE *)pData->Buff , (BYTE *)smEncData.Buff );

		if ( !dwDataKey )
			return FALSE;

		cnt = dwDataKey&0xFFFF;

		if ( pData->smMySock->CheckEncRecvPacket( cnt )==TRUE ) {
			//���� ����Ÿ �� ���� ��� ( ���� ���� )
			return FALSE;
		}

		if ( cnt<(pData->smMySock->EncPacketRecvCount-ENC_PACKET_REC_MAX) ) {
			//���� ������ ����Ÿ�ų� �����ִ� ���
			return FALSE;
		}
		pData->smMySock->PushEncRecvPacket( cnt );			//��Ŷ ��ȣ ���


		pData = &smEncData;
		IPData = (DWORD *)( pData->Buff );

		lpCharInfo_MsgBack = new smCHAR_INFO;
		if ( lpCharInfo_MsgBack ) {
			memcpy( lpCharInfo_MsgBack , &lpCurPlayer->smCharInfo , sizeof(smCHAR_INFO) );
		}
	}
	else 
	{
		if ( IPData[1]==smTRANSCODE_PUTITEM || IPData[1]==smTRANSCODE_ADDEXP )
			return FALSE;

	}

#ifdef _DEBUG
	Utils_Log(LOG_DEBUG, "Recv %s (%x) %d\r\n", GetTranscodeNameByOpcode(IPData[1]), IPData[1], IPData[0]);
#endif

	switch( IPData[1] ) 
	{
		case smTRANSCODE_CHATMESSAGE:
			//������ ���� ä�� ���ڸ� ����
			ChatCnt = ChatBuffCnt & CHATBUFF_MASK;
			ChatBuff[ ChatCnt ].dwIP = IPData[2];
			lstrcpy( ChatBuff[ ChatCnt ].szMessage , &pData->Buff[3*4] );
			ChatBuffCnt++;
			ChatDispCnt = 70*10;
			if ( ChatScrollPoint ) {
				ChatScrollPoint++;
			}
			
			break;

		case smTRANSCODE_PLAYLIST:
			/*
			//������ PlayerList ����
			lpAreaInfoHeader= (TRANS_AREAINFO_HEADER *)pData->Buff;
			lpTransIp		= (TRANS_IPLIST *)&pData->Buff[sizeof(TRANS_AREAINFO_HEADER)];

			//����Ʈ�� �м���
			for(cnt=0;cnt<lpAreaInfoHeader->TransListCnt;cnt++) {
				//������ �Ÿ��� ���
				mx = (lpCurPlayer->pX - lpTransIp->Pos.x)>>FLOATNS;
				my = (lpCurPlayer->pY - lpTransIp->Pos.y)>>FLOATNS;
				mz = (lpCurPlayer->pZ - lpTransIp->Pos.z)>>FLOATNS;
				dDist = mx*mx + mz*mz;
				//dDist = mx*mx + my*my + mz*mz;

				if ( dDist<DIST_TRANSLEVEL_CONNECT ) {
					//���� ������� ����

					//���Ͽ� ũ��ƼĮ ���� ����
					smEnterCSectionSocket();
					lpsmFindSock = smFindSocket2( lpTransIp->dwIP );
					//���Ͽ� ũ��ƼĮ ���� ����
					smLeaveCSectionSocket();

					if ( !lpsmFindSock ) {
						//�޼��� �����忡�� �����ϵ���...�޼��� ����
						//PostMessage( hwnd , SWM_MCONNECT , (WPARAM)lpTransIp->dwIP , 0 );
						//ConnectOtherPlayer( lpTransIp->dwIP );	//��������
						CreateConnectThread(lpTransIp->dwIP);		//������ ���� ����
					}
					lpTransIp++;
				}
			}
			//ũ��ƼĮ ���� ����
			//LeaveCriticalSection( &cServSection );

			if ( lpAreaInfoHeader->Stats==FALSE ) {
				//��� ���� ����Ÿ�� ������ �䱸
				TempBuff[0] = sizeof( int )*2;
				TempBuff[1] = smTRANSCODE_CONTINUEDATA;
				pData->smMySock->Send( TransBuff , TempBuff[0] , TRUE );
			}
			*/
			break;

		case smTRANSCODE_PLAYERINFO:
			//��� �÷��̾��� ����Ÿ ���� ����
			lpTransPlayerInfo = (smTRNAS_PLAYERINFO *)pData->Buff;

			lpChar = FindAutoPlayer( lpTransPlayerInfo->dwObjectSerial );

			if ( lpChar ) {
				lpChar->RecvPlayData( pData );
				//ĳ���� ��Ƽ ���Կ��� Ȯ��
				RequestPartyPlayers( lpChar );
			}

			//���� ����Ÿ�� ��� ĳ�ÿ� ����
			if ( lpTransPlayerInfo->smCharInfo.State==smCHAR_STATE_ENEMY && lpTransPlayerInfo->smCharInfo.Brood!=smCHAR_MONSTER_USER )
				SaveCharInfoCache(lpTransPlayerInfo);

			break;

		case smTRANSCODE_PLAYERINFO_Q:
			//��� �÷��̾��� ����Ÿ ���� ����
			lpChar = FindAutoPlayer( ((smTRNAS_PLAYERINFO_QUICK *)pData->Buff)->dwObjectSerial );

			if ( lpChar ) {
				lpChar->RecvPlayData( pData );
				//ĳ���� ��Ƽ ���Կ��� Ȯ��
				RequestPartyPlayers( lpChar );
			}
			break;

		case smTRANSCODE_PLAYDATAGROUP:
			//�÷��̾� ������ ������ �׷����� ���� ����

			smPLAYDATA_GROUP *lpPlayDataGroup;
			char *lpTransBuff;

			lpPlayDataGroup = (smPLAYDATA_GROUP *)pData->Buff;
			lpTransBuff = pData->Buff+sizeof(smPLAYDATA_GROUP);

			if ( !(lpPlayDataGroup->dwSendCode&0x80000000) ) {
				rsRecvUserServer = TRUE;
			}

			ServerCode = 0;
			RcvCnt = 0;

			if ( smWsockUserServer==pData->smMySock ) {
				ServerCode = PLAY_SERVER_CODE_USER;
			}
			else {
				if ( smWsockExtendServer==pData->smMySock ) 
					ServerCode = PLAY_SERVER_CODE_EXTEND;
				else
					ServerCode = PLAY_SERVER_CODE_MAIN;
			}

			if ( AreaServerMode && ServerCode==PLAY_SERVER_CODE_MAIN ) {
				if ( lpWSockServer_DispArea[0]==pData->smMySock ) {
					ServerCode = PLAY_SERVER_CODE_AREA1;
				}
				else  if ( lpWSockServer_DispArea[1]==pData->smMySock ) {
					ServerCode = PLAY_SERVER_CODE_AREA2;
				}
			}

			for( cnt=0;cnt<lpPlayDataGroup->PlayDataCnt;cnt++) {

				lpPlayData1 = (smPLAYDATA *)lpTransBuff;
				lpChar = FindAutoPlayer( lpPlayData1->dwObjectSerial );

				if ( lpChar ) {
					lpChar->RecvPlayData2( lpTransBuff );
					if ( lpChar->PlayBuffCnt<(lpChar->PlayBuffPosi_End-70) ) {
						lpChar->PlayBuffCnt = lpChar->PlayBuffPosi_End-70;
					}
				}
				else {
					if ( RcvCnt<8 || (RcvCnt<16 && ((int *)lpTransBuff)[1]==smTRANSCODE_TRANSPLAYDATA) ) {
						RcvCnt++;
						lpChar = FindEmptyChar();
						if ( lpChar ) {
							//ĳ���� �ű� ���
							lpChar->Init();
							lpChar->Flag = TRUE;
							lpChar->smCharInfo.szName[0]=0;
							lpChar->ActionPattern = 99;
							lpChar->Pattern = 0;
							lpChar->AutoPlayer = TRUE;
							lpChar->TransSock = 0;
							lpChar->AutoPlayer = TRUE;
							lpChar->DisplayFlag = FALSE;
							lpChar->MotionInfo = 0;

							lpChar->dwObjectSerial = lpPlayData1->dwObjectSerial;
							lpChar->ServerCode = ServerCode;

							lpChar->ResetAttackTrans();

							//����ٰ� �� �� ĳ���� ���� ������ �����޶� ������ �䱸�ϴ�
							//���� �߰� �ؾ���

							lpTransPlayerInfo = GetCharInfoCache( lpTransBuff );
							if ( lpTransPlayerInfo ) {
								//ĳ�ÿ��� ���� ����Ÿ ����
								lpChar->SetTransPlayerInfo( lpTransPlayerInfo );

								//ĳ���� ��Ƽ ���Կ��� Ȯ��
								RequestPartyPlayers( lpChar );
							}
							else {
								//���� ĳ���� ���� ������ �䱸 
								TransCommand.code = smTRANSCODE_GETPLAYERINFO;
								TransCommand.size = sizeof( smTRANS_COMMAND );
								TransCommand.WParam = lpPlayData1->dwObjectSerial;

								if ( pData->smMySock->sock ) {
									pData->smMySock->Send( (char *)&TransCommand , TransCommand.size , TRUE );
								}
							}
						}
					}	
				}
				lpTransBuff +=lpPlayData1->size;
			}

			lpTransCommand = (smTRANS_COMMAND *)lpTransBuff;
			if ( lpTransCommand->code==smTRANSCODE_PLAY_COMMAND && lpTransCommand->WParam==lpCurPlayer->dwObjectSerial ) {
				//���� ���މ���
				switch( lpTransCommand->LParam ) {
					case smCOMMAND_PLAY_BABEL:
						//�ٺ� ������
						dwBabelPlayTime = dwPlayTime+5000;		//�ٺ������ð� 5�� ����
						BabelPlayField = lpTransCommand->SParam;
						break;
/*
					case smCOMMAND_PLAY_KELVEZU:
						//�̺��� ������
						dwKelvezuPlayTime = dwPlayTime+5000;		//�̺��������ð� 5�� ����
						KelvezuPlayField = lpTransCommand->SParam;
						break;
*/
					case smCOMMAND_PLAY_PK_FIELD:
						//PK ��� �ʵ�
						PK_FieldState.dwPlay_FieldTime = dwPlayTime+10000;		//�����ð� 10�� ����
						PK_FieldState.FieldCode = lpTransCommand->SParam;
						break;

					case smCOMMAND_PLAY_BELSS_CASTLE:
						//���� ĳ��
						memcpy( &rsBlessCastleOld , &rsBlessCastle , sizeof( rsBLESS_CASTLE ) );

						lpTransCommandEx = (smTRANS_COMMAND_EX *)lpTransBuff;

						rsBlessCastle.CastleMode = lpTransCommandEx->SParam>>16;
						rsBlessCastle.DefenceLevel = lpTransCommandEx->SParam&0xFFFF;
						rsBlessCastle.dwMasterClan = lpTransCommandEx->EParam;
						((int *)rsBlessCastle.TimeSec)[0] = lpTransCommandEx->WxParam;
						((int *)rsBlessCastle.bCrystalSolderCount)[0] = lpTransCommandEx->LxParam;

						chaSiege.ShowPlayTime(rsBlessCastle.TimeSec[1]-rsBlessCastle.TimeSec[0]);

						//����ĳ�� ������ �� ų�� ī��Ʈ
						lpCurPlayer->sBlessCastle_Damage[0] += lpTransCommandEx->SxParam&0xFFFF;
						lpCurPlayer->sBlessCastle_Damage[1] += lpTransCommandEx->SxParam>>16;

						if ( rsBlessCastle.CastleMode==3 ) {
							if ( rsBlessCastleOld.CastleMode==1 ) {
								//���� ĳ�� ���� ��ô��
								RestartPlayCount = 400;		//6�� ���� ����
								StopBGM();
								esPlayContSound( 14 );
								chaSiege.ShowExitMessage();		//������ ����� �޼���

								if ( rsBlessCastle.dwMasterClan && rsBlessCastle.dwMasterClan==GetClanCode(lpCurPlayer->smCharInfo.ClassClan) ) {
									lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_YAHOO);
								}
							}
						}
						else if ( rsBlessCastleOld.CastleMode && rsBlessCastle.CastleMode==0 ) {
							//�������� 
							if ( !lpCurPlayer->smCharInfo.ClassClan || rsBlessCastle.dwMasterClan!=GetClanCode(lpCurPlayer->smCharInfo.ClassClan) ) {
								//Ŭ������ ������� �������� ������
								WarpField2( rsCASTLE_FIELD );
							}
						}

						//�Ӽ����� 1�� �������� ����
						if ( (dwResistance_SendingTime+1000*60)<dwPlayTime ) {
							SendResistanceToServer();
						}

						if ( rsBlessCastle.dwMasterClan!=rsBlessCastleOld.dwMasterClan ) {
							//���� �ٲ���
							//ĳ�� ������ ������Ʈ
							UpdateCastleMasterClan( rsBlessCastle.dwMasterClan );
						}


						break;
				}
			}
			break;

		case smTRANSCODE_ATTACKDATA2:
			#ifdef _USE_DYNAMIC_ENCODE
				if ( fnDecodeDamagePacket ) 
					fnDecodeDamagePacket( 0 , pData->Buff );

				if ( ((TRANS_ATTACKDATA *)pData->Buff)->dwDamageChkSum!=dm_GetDamgeChkSum_S2V((TRANS_ATTACKDATA *)pData->Buff) ) {
					SendSetHackUser3( 8580 , ((TRANS_ATTACKDATA *)pData->Buff)->Power , (DWORD)fnDecodeDamagePacket );
					break;
				}
			#else
				dm_DecodePacket(0, pData->Buff);

				if (((TRANS_ATTACKDATA *)pData->Buff)->dwDamageChkSum != dm_GetDamgeChkSum_S2V((TRANS_ATTACKDATA *)pData->Buff)) 
				{
					SendSetHackUser3(8580, ((TRANS_ATTACKDATA *)pData->Buff)->Power, (DWORD)dm_DecodePacket);
					break;
				}
			#endif
		case smTRANSCODE_ATTACKDATA:
			CheckInvenItemToServer();		//�κ��丮�� ������ �������� ������ ������ �˻�
			Record_RecvDamage( pData->smMySock , ((TRANS_ATTACKDATA *)pData->Buff)->Power ); //���� ���ݷ� ���

		case smTRANSCODE_PLAYDATA1:
		case smTRANSCODE_PLAYDATA2:
		case smTRANSCODE_PLAYDATA3:
		case smTRANSCODE_TRANSPLAYDATA:
			//���� �÷��� ����Ÿ �Լ�

			ServerCode = 0;

			if ( smWsockUserServer==pData->smMySock ) {
				ServerCode = PLAY_SERVER_CODE_USER;
			}
			else {
				if ( smWsockExtendServer!=smWsockServer && smWsockExtendServer==pData->smMySock ) 
					ServerCode = PLAY_SERVER_CODE_EXTEND;
				else
					ServerCode = PLAY_SERVER_CODE_MAIN;
			}


			if ( AreaServerMode && ServerCode==PLAY_SERVER_CODE_MAIN ) {
				if ( lpWSockServer_DispArea[0]==pData->smMySock ) {
					ServerCode = PLAY_SERVER_CODE_AREA1;
				}
				else  if ( lpWSockServer_DispArea[1]==pData->smMySock ) {
					ServerCode = PLAY_SERVER_CODE_AREA2;
				}
			}


			lpPlayData1 = (smPLAYDATA *)pData->Buff;
			lpChar = FindAutoPlayer( lpPlayData1->dwObjectSerial );
			if ( lpChar ) {
				lpChar->RecvPlayData( pData );
				if ( lpChar->PlayBuffCnt<(lpChar->PlayBuffPosi_End-70) ) {
					lpChar->PlayBuffCnt = lpChar->PlayBuffPosi_End-70;
				}
			}
/*
			else {
				if ( dwGetCharInfoTime && (dwGetCharInfoTime+300)>dwPlayTime ) break;

				dwGetCharInfoTime = dwPlayTime;

				lpChar = FindEmptyChar();
				if ( lpChar ) {
					//ĳ���� �ű� ���
					lpChar->Init();
					lpChar->Flag = TRUE;
					lpChar->smCharInfo.szName[0]=0;
					lpChar->ActionPattern = 99;
					lpChar->Pattern = 0;
					lpChar->AutoPlayer = TRUE;
					lpChar->TransSock = 0;
					lpChar->AutoPlayer = TRUE;

					lpChar->dwObjectSerial = lpPlayData1->dwObjectSerial;
					lpChar->ServerCode = ServerCode;

					//����ٰ� �� �� ĳ���� ���� ������ �����޶� ������ �䱸�ϴ�
					//���� �߰� �ؾ���


//					Send_GetCharInfo( lpPlayData1->dwObjectSerial );
				}

			}
*/
			break;

		case smTRANSCODE_ATTACK_SLASH_LIST:
			//ƨ�ܼ� �´� ������ ���� ��� ����
			memcpy( &Trans_SplashCharList , pData->Buff , sizeof(TRANS_SKIL_ATTACKDATA) );
			break;


		case smTRANSCODE_PLAYITEM:
			//������ ���� ������ ��� ����
			smTRANS_ITEMS	*lpTransItems;
			scITEM			*lpScItem;

			lpTransItems = (smTRANS_ITEMS *)pData->Buff;

			if ( smWsockUserServer==pData->smMySock ) {
				ServerCode = PLAY_SERVER_CODE_USER;
			}
			else {
				if ( smWsockExtendServer!=smWsockServer && smWsockExtendServer==pData->smMySock ) 
					ServerCode = PLAY_SERVER_CODE_EXTEND;
				else
					ServerCode = PLAY_SERVER_CODE_MAIN;
			}

			if ( AreaServerMode && ServerCode==PLAY_SERVER_CODE_MAIN ) {
				if ( lpWSockServer_DispArea[0]==pData->smMySock ) {
					ServerCode = PLAY_SERVER_CODE_AREA1;
				}
				else  if ( lpWSockServer_DispArea[1]==pData->smMySock ) {
					ServerCode = PLAY_SERVER_CODE_AREA2;
				}
			}

			for( cnt=0;cnt<lpTransItems->Count;cnt++) {
				lpScItem = FindScItem( lpTransItems->Items[cnt].x , lpTransItems->Items[cnt].z );
				if ( lpScItem ) {
					lpScItem->dwLastTransTime = dwPlayTime;
				}
				else {
					//�� ������ ����ã��
					lpScItem = FindEmptyScItem();
					if ( lpScItem )  {
						lpScItem->SetTransItem( &lpTransItems->Items[cnt] );
						lpScItem->ServerCode = ServerCode;
					}
				}
			}
			break;

		case smTRANSCODE_DELITEM:
			//������ ����
			lpTransActionItem = ( smTRANS_ACITON_ITEM *)pData->Buff;
			lpScItem = FindScItem( lpTransActionItem->x , lpTransActionItem->z );
			if ( lpScItem ) {
	
				//ũ��ƼĮ ���� ����
				EnterCriticalSection( &cDrawSection );
				smEnterTextureCriticalSection();

				lpScItem->Close();

				//ũ��ƼĮ ���� ����
				smLeaveTextureCriticalSection();
				LeaveCriticalSection( &cDrawSection );

			}
			break;

		case smTRANSCODE_PUTITEM:
			//���� ������ ����Ÿ�� ������ ���� ����
			lpTransItemInfo = ( TRANS_ITEMINFO *)pData->Buff;


			if ( FiltQuestItem( lpTransItemInfo , smTRANSCODE_PUTITEM )==FALSE ) break;		//����Ʈ ������ ���͸�


			//if ( !VRKeyBuff['J'] && PushRecvTransItemQue( lpTransItemInfo )==TRUE ) {
			if ( PushRecvTransItemQue( lpTransItemInfo )==TRUE ) {
				//�ϴ� ������ ť�� �������� ����Ÿ ������ ����
				TransCommandEx.code = smTRANSCODE_GETITEM;
				TransCommandEx.size = sizeof( smTRANS_COMMAND_EX );
				TransCommandEx.WParam = lpTransItemInfo->Item.CODE;
				TransCommandEx.LParam = lpTransItemInfo->Item.ItemHeader.Head;
				TransCommandEx.SParam = lpTransItemInfo->Item.ItemHeader.dwChkSum;
				TransCommandEx.EParam = lpTransItemInfo->Item.Money^smTRANSCODE_GETITEM^lpTransItemInfo->Item.ItemHeader.dwChkSum;

				TransCommandEx.WxParam = lpTransItemInfo->dwSeCode[0];
				TransCommandEx.LxParam = lpTransItemInfo->dwSeCode[1];
				TransCommandEx.SxParam = lpTransItemInfo->dwSeCode[2];
				TransCommandEx.ExParam = lpTransItemInfo->dwSeCode[3];
				if ( smWsockDataServer ) 
					smWsockDataServer->Send2( (char *)&TransCommandEx , TransCommandEx.size , TRUE );

				break;
			}

			//���హ�� ��
			ComparePotion();

			memcpy( &TransRecvItem , lpTransItemInfo , sizeof(TRANS_ITEMINFO) );

			//������ ���� Ȯ��
			if ( CheckItemForm( &lpTransItemInfo->Item )==TRUE ) {
				if ( lpTransItemInfo->Item.CODE==(sinGG1|sin01) ) {
					if ( cInvenTory.CheckMoneyLimit(lpTransItemInfo->Item.Money)==TRUE ) {
						//�ֿ�� ���̴�
						lpCurPlayer->smCharInfo.Money += lpTransItemInfo->Item.Money;
						ReformCharForm();					//ĳ���� ���� ����
						sinPlaySound( SIN_SOUND_COIN , 400 );

						addMoney = lpTransItemInfo->Item.Money;

						CheckServerMoney( pData->smMySock , lpTransItemInfo );

					}
					else {	
						//������ ������
						ThrowPutItem2( &lpTransItemInfo->Item , lpCurPlayer->pX, lpCurPlayer->pY , lpCurPlayer->pZ );
					}
				}
				else {
					if ( (lpTransItemInfo->Item.CODE&sinITEM_MASK2)==sinQT1 || lpTransItemInfo->Item.ItemKindCode==ITEM_KIND_QUEST_WEAPON ) {
						//����Ʈ ������ ȹ��
						/*
						if ( sinQuest_ChangeJob.CODE == SIN_QUEST_CODE_CHANGEJOB ) {		//����Ʈ������
							if ( sinSetQuestItem( &lpTransItemInfo->Item )==TRUE ) {
								SaveGameData();								//������ ����
							}
						}
						*/
						if ( sinSetQuestItem( &lpTransItemInfo->Item )==TRUE ) {
							SaveGameData();								//������ ����
						}
					}
					else {
						sinSetInvenItem( &lpTransItemInfo->Item );

						//������ 2000�� �̻� �������� �־��� ( ��ΰ� ���� )
						if ( lpTransItemInfo->Item.Price>=3000 )
							SaveGameData();						//������ ����

						if ( (lpTransItemInfo->Item.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
							//���� ���� ���� ����
							ResetPotion2();
						}
					}
				}

			}
			ZeroMemory( &TransRecvItem , sizeof(TRANS_ITEMINFO) );
			ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			break;

		case smTRANSCODE_GETITEM:
			//���� ������ ����Ÿ�� ������ ���� ����
			lpTransCommand = ( smTRANS_COMMAND *)pData->Buff;

			lpTransItemInfo = FindRecvTransItemQue( lpTransCommand->WParam , lpTransCommand->LParam , lpTransCommand->SParam );

			if ( !lpTransItemInfo ) break;

			if ( FiltQuestItem( lpTransItemInfo , smTRANSCODE_GETITEM )==FALSE ) break;		//����Ʈ ������ ���͸�

			//���హ�� ��
			ComparePotion();

			memcpy( &TransRecvItem , lpTransItemInfo , sizeof(TRANS_ITEMINFO) );

			//������ ���� Ȯ��
			if ( CheckItemForm( &lpTransItemInfo->Item )==TRUE ) {
				if ( lpTransItemInfo->Item.CODE==(sinGG1|sin01) ) {
					if ( cInvenTory.CheckMoneyLimit(lpTransItemInfo->Item.Money)==TRUE ) {
						//�ֿ�� ���̴�
						lpCurPlayer->smCharInfo.Money += lpTransItemInfo->Item.Money;
						ReformCharForm();					//ĳ���� ���� ����
						sinPlaySound( SIN_SOUND_COIN , 400 );

						addMoney = lpTransItemInfo->Item.Money;

						CheckServerMoney( pData->smMySock , lpTransItemInfo );

					}
					else {	
						//������ ������
						ThrowPutItem2( &lpTransItemInfo->Item , lpCurPlayer->pX, lpCurPlayer->pY , lpCurPlayer->pZ );
					}
				}
				else {
					if ( (lpTransItemInfo->Item.CODE&sinITEM_MASK2)==sinQT1 || lpTransItemInfo->Item.ItemKindCode==ITEM_KIND_QUEST_WEAPON ) {
						//����Ʈ ������ ȹ��
						/*
						if ( sinQuest_ChangeJob.CODE == SIN_QUEST_CODE_CHANGEJOB ) {		//����Ʈ������
							if ( sinSetQuestItem( &lpTransItemInfo->Item )==TRUE ) {
								SaveGameData();								//������ ����
							}
						}
						*/
						if ( sinSetQuestItem( &lpTransItemInfo->Item )==TRUE ) {
							SaveGameData();								//������ ����
						}
					}
					else {
						if ( (lpTransItemInfo->Item.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
							if ( smConfig.DebugMode && VRKeyBuff['P'] && VRKeyBuff['O'] ) {
								lpTransItemInfo->Item.PotionCount *= 2;
							}
						}

						sinSetInvenItem( &lpTransItemInfo->Item );

						//������ 2000�� �̻� �������� �־��� ( ��ΰ� ���� )
						if ( lpTransItemInfo->Item.Price>=3000 )
							SaveGameData();						//������ ����

						if ( (lpTransItemInfo->Item.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
							//����׽�Ʈ
							//���� ���� ���� ����
							ResetPotion2();
						}
					}
				}

			}
			ZeroMemory( &TransRecvItem , sizeof(TRANS_ITEMINFO) );
			ZeroMemory( lpTransItemInfo , sizeof(TRANS_ITEMINFO) );
			
			ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			break;


		case smTRANSCODE_MESSAGEBOX:
			//�޼��� �ڽ� ��� �䱸
			lpTransChatMessage = (TRANS_CHATMESSAGE *)pData->Buff;
			cMessageBox.ShowMessageEvent(lpTransChatMessage->szMessage);
			break;

		case smTRANSCODE_WHISPERMESSAGE:
			//NPC ĳ���Ͱ� ��ȭ�� ...

			lpTransChatMessage = (TRANS_CHATMESSAGE *)pData->Buff;

			lpChar = FindAutoPlayer( lpTransChatMessage->dwObjectSerial );
			if ( lpChar && lpTransChatMessage->dwIP!=4 ) { //&& lpChar->smCharInfo.State!=smCHAR_STATE_USER ) {
				lstrcpy( lpChar->szChatMessage , lpTransChatMessage->szMessage );
				lpChar->dwChatMessageTimer = dwPlayTime + 1000*5;

				if ( lpChar->smCharInfo.State==smCHAR_STATE_USER ) {
					//ä��â�� �޼��� �߰�
					ChatCnt = ChatBuffCnt & CHATBUFF_MASK;
					ChatBuff[ ChatCnt ].dwIP = smCHAR_STATE_USER;
					lstrcpy( ChatBuff[ ChatCnt ].szMessage , lpTransChatMessage->szMessage );
					ChatBuffCnt++;
					ChatDispCnt = 70*10;
					if ( ChatScrollPoint ) {
						ChatScrollPoint++;
					}

					if ( chrEachPlayer.Flag && chrEachMaster==lpChar ) {
						lstrcpy( chrEachPlayer.szChatMessage , lpTransChatMessage->szMessage );
					}
				}
				//######################################################################################
				//�� �� �� : �� �� ��
				else if( lpChar->smCharInfo.State == smCHAR_STATE_NPC )
				{
					if( FindTalkSound( lpChar->szChatMessage ) == FALSE )
					{
						StopTalkSound();

						lstrcpy( g_szPlayTalkSoundName, g_szTalkSoundName );
						g_PlayTalkSoundSum = g_TalkSoundSum;

						/*// ���� ���� �������� ��û�Ѵ�.
						if( IsErrorWinIntThread() == FALSE )
						{
							char szDownName[64];
							lstrcpy( szDownName, g_szTalkSoundName );
							SetExtChangeZip( szDownName );
							WavIntHttpPush( szDownName );
                            WinIntThreadResume();
						}*/
					}
					
					lstrcpy( chrEachPlayer.szChatMessage, lpChar->szChatMessage );
					chrEachPlayer.TalkSoundSum = g_TalkSoundSum;
				}
				//######################################################################################


			}
			else {
				ChatCnt = ChatBuffCnt & CHATBUFF_MASK;
				ChatBuff[ ChatCnt ].dwIP = lpTransChatMessage->dwIP;
				lstrcpy( ChatBuff[ ChatCnt ].szMessage , lpTransChatMessage->szMessage );
	
				if ( ChatBuffFilter( ChatBuff[ ChatCnt ].szMessage , ChatBuff[ ChatCnt ].dwIP )==TRUE ) {
					ChatBuffCnt++;
					ChatDispCnt = 70*10;

					if ( lpTransChatMessage->dwIP==1 || lpTransChatMessage->dwIP==3 ) {
						//�ͼӸ� ����
						if ( !cInterFace.ChatFlag ) {
							//�Ӹ� ���� �Ҹ�
							esPlaySound( 8, 360 );
							cInterFace.ChatFlag = 1;
						}
					}
				}
			}

			break;

		case smTRANSCODE_CONNECTED:
			//���� ���� ( ������ ���� ������ȣ ���� )
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			//lpCurPlayer->dwObjectSerial = lpTransCommand->WParam;

			SaveCheckSkill();			//��ų�� ���� Ȯ��
			lpCurPlayer->dwObjectSerial = lpCurPlayer->smCharInfo.dwObjectSerial;
			ReformSkillInfo();			//��ų��ȣ�� ���� �ʱ�ȭ

			if ( pData->smMySock==smWsockServer ) {
				//�ý��� �ð��� ���ӽð����� ��ȯ
				dwGameWorldTime = ConvSysTimeToGameTime(lpTransCommand->LParam);
				dwGameHour = dwGameWorldTime/60;
				dwGameHour = dwGameHour-(((int)(dwGameHour/24))*24);			//���ӳ��ο����� ��
				dwGameMin = dwGameWorldTime-(((int)(dwGameWorldTime/60))*60);		//���ӳ��ο����� ��


				//���� �ð� ���
				dwConnectedClientTime = GetCurrentTime();
				dwConnectedServerTime = lpTransCommand->LParam;
				dwLastRecvGameServerTime = dwConnectedClientTime;

				//���� ������ ������Ʈ ��ȣ ���� ���� ( ���� ������ ��츸 ����� )
				if ( smWsockServer!=smWsockUserServer )
					SendSetObjectSerial_First( lpCurPlayer->smCharInfo.dwObjectSerial );

					//SendSetObjectSerial( lpTransCommand->WParam );

				//������ ��Ʈ��ũ Ǯ�� ��� ����
				SendNetworkQuality( smConfig.NetworkQuality );


				//���μ��� ���� ����
				SendProcessInfo();

				//������ ���� ����
				//if ( smConfig.DebugMode ) SendAdminMode( TRUE );

			}
			//dwGameWorldTime = 0;
			break;

		case smTRANSCODE_RECORDDATA:
			//������ ����� ĳ���� ������ ����Ÿ �Լ�
			if ( !lpRecorder ) {
				lpRecorder = new rsRECORD_DBASE;
				lpRecorder->TransLastPartCount = 0;
				ZeroMemory( lpRecorder->TransDataBlock , 64 );
			}

			//������ ���� ĳ���� ���� ����
			if ( lpRecorder->RecvRecordDataFromServer((TRANS_RECORD_DATAS *)pData->Buff)==TRUE ) {

				if ( AdminCharDisable ) {
					ZeroMemory( cInvenTory.InvenItem , sizeof(sITEM)*INVENTORY_MAXITEM  );
					ZeroMemory( cInvenTory.InvenItemTemp , sizeof(sITEM)*INVENTORY_MAXITEM );

					lpCurPlayer->SetTool( 0 , hvPOSI_LHAND );
					lpCurPlayer->SetTool( 0 , hvPOSI_RHAND );
				}

				lpRecorder->ResotrRecordData( &lpCurPlayer->smCharInfo , cInvenTory.InvenItem ,cInvenTory.InvenItemTemp , &MouseItem );

				if ( smConfig.DebugMode ) 
					AddChatBuff( lpRecorder->TransRecordData.GameSaveInfo.szMasterID );

				delete lpRecorder;
				lpRecorder = 0;

				dwLoadingTime = 0;
			
				//���� �⺻���� ������
				if ( ( lpCurPlayer->smCharInfo.JOB_CODE==3 || lpCurPlayer->smCharInfo.JOB_CODE==5 || lpCurPlayer->smCharInfo.JOB_CODE==7 ) 
					&& lpCurPlayer->Pattern ) {

					lpCurPlayer->SetPattern( lpCurPlayer->Pattern );
				}

///////////////////////////////////////////////////////////////////////
				if ( smConfig.DebugMode && smConfig.szFile_Player[0] ) {

					lstrcpy( lpCurPlayer->smCharInfo.szModelName , smConfig.szFile_Player );
					lpCurPlayer->smCharInfo.szModelName2[0] = 0;

					CharLower( lpCurPlayer->smCharInfo.szModelName );			//�ҹ��� ��ȯ

					lpCurPlayer->smCharInfo.State=smCHAR_STATE_USER;						//���� �Ӽ�

					if ( smConfig.szFile_Enemy[0] ) { 
						if ( strstr( lpCurPlayer->smCharInfo.szModelName , "monster" )!=0 )	//���� �Ӽ�
							lpCurPlayer->smCharInfo.State=smCHAR_STATE_ENEMY;
						if ( strstr( lpCurPlayer->smCharInfo.szModelName , "npc" )!=0 )		//NPC �Ӽ�
							lpCurPlayer->smCharInfo.State=smCHAR_STATE_NPC;
					}

					AddLoaderPattern( lpCurPlayer , lpCurPlayer->smCharInfo.szModelName , lpCurPlayer->smCharInfo.szModelName2 );
					ReformCharForm();

					//AdminCharDisable = TRUE;

					//SetLoadPattern( lpCurPlayer , smConfig.szFile_Player ,0 , &Posi , &Angle );
				}
//////////////////////////////////////////////////////////

				lpCurPlayer->Flag = TRUE;

				if ( !lpCurPlayer->smCharInfo.dwEventTime_T || GetPlayTime_T()>lpCurPlayer->smCharInfo.dwEventTime_T ) {
					lpCurPlayer->smCharInfo.SizeLevel = 1;
					lpCurPlayer->smCharInfo.dwEventTime_T = 0;
				}

				//��Ų ���� ��Ȯ��
				if ( lpCurPlayer->smCharInfo.SizeLevel>0x1000 && lpCurPlayer->smCharInfo.SizeLevel<=0x1002 ) {
					ChangeModelSkin( lpCurPlayer , 1 , lpCurPlayer->smCharInfo.SizeLevel+3-0x1000 , 0 , 0 );			//�׽�Ʈ ( ��ū�� )
					ReformCharForm();
					cInvenTory.IncreViewDamage ( 15 );		//������ �������� �÷��ش�
				}
				else if(lpCurPlayer->smCharInfo.dwEventTime_T > 0 && lpCurPlayer->smCharInfo.SizeLevel==1) // ����� - 2010 ������ �̺�Ʈ - �౸�� ���� ���(���� ��)
				{
					ChangeModelSkin( lpCurPlayer , 0 , 6 , 0 , 0 );	// �౸�� �� �� // ���� �� �ð��� ���� ������� �౸�� �� ����
					ReformCharForm();
				}
				else {
					if ( lpCurPlayer->smCharInfo.ChangeJob>=0 && lpCurPlayer->smCharInfo.ChangeJob<4 ) {
						ChangeModelSkin( lpCurPlayer , 0 , lpCurPlayer->smCharInfo.ChangeJob , 0 , GetFaceHairSkin(lpCurPlayer) );			//�׽�Ʈ ( �Ϲݾ� ) // ��ū�� ���� ����
						ReformCharForm();
					}
				}

#ifdef  __CLANSUBCHIP__
				if ( cldata.myPosition==100 || cldata.myPosition==101 || cldata.myPosition==104) {
#else
				if ( cldata.myPosition==100 || cldata.myPosition==101 ) {
#endif
					//Ŭ�� ���Կ�
					lpCurPlayer->smCharInfo.ClassClan = cldata.intClanMark;
					lpCurPlayer->dwClanManageBit = cldata.CNFlag;
				}
				else
					lpCurPlayer->smCharInfo.ClassClan = 0;

				if ( cldata.myPosition )
					InitClanMode = cldata.myPosition;

				if ( !AdminCharDisable ) {
					//���� ���� �ð�
					rsRecorder.dwNextRecordTime = dwPlayTime+RECORD_TIME_STEP;

					lpCurPlayer->SendCharInfo( smWsockServer );			//�÷��̾� ���� ����
				}
				else {
					AddLoaderPattern( lpCurPlayer , lpCurPlayer->smCharInfo.szModelName , lpCurPlayer->smCharInfo.szModelName2 );
					//ĳ���� ���� ���� �ޱ�
					ReformCharForm();
				}

				//�������� 0�ΰ�� �������� ����
				if ( lpCurPlayer->smCharInfo.Life[0]==0 ) {
					//�ʵ� ����
					lpCurPlayer->MoveFlag = FALSE;
					WarpStartField( &x,&z );
					lpCurPlayer->SetPosi( x,0, z ,0,0,0 );
					TraceCameraPosi.x = lpCurPlayer->pX;
					TraceCameraPosi.y = lpCurPlayer->pY;
					TraceCameraPosi.z = lpCurPlayer->pZ;
					TraceTargetPosi.x = lpCurPlayer->pX;
					TraceTargetPosi.y = lpCurPlayer->pY;
					TraceTargetPosi.z = lpCurPlayer->pZ;

					lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_RESTART );		//���۸��
					StartEffect( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, EFFECT_GAME_START1 );
					SetDynLight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 100, 100,100,0, 200 );
					CharPlaySound( lpCurPlayer );
					dwBattleTime = 0;
					dwBattleQuitTime = 0;

					//�ٴ� Ȯ��
					lpCurPlayer->OnStageField = -1;
					if ( smGameStage[0] ) {
						cnt = smGameStage[0]->GetFloorHeight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ ,lpCurPlayer->Pattern->SizeHeight );	
						lpCurPlayer->OnStageField = 0;
					}
					if ( cnt==CLIP_OUT && smGameStage[1] ) {
						lpCurPlayer->OnStageField = 1;
					}
				}

				//sinSkill.SkillPoint = lpCurPlayer->smCharInfo.Level;
				cnt = lpCurPlayer->smCharInfo.Life[0];
				if ( cnt>lpCurPlayer->smCharInfo.Life[1] || cnt<0 ) {
					//�߸��� ����� ���� ����
					CheckCharForm();
					lpCurPlayer->smCharInfo.Life[0] = lpCurPlayer->smCharInfo.Life[1]/2;
					ReformCharForm();
				}

				CheckCharForm();
				CodeXorExp = dwPlayTime+lpCurPlayer->smCharInfo.Exp;					//����ġ ���� �ڵ�
				CodeXorExp_High = dwPlayTime|(dwPlayTime*dwPlayTime);						//����ġ ���� �ڵ�
				CodeXorLife = (dwPlayTime*lpCurPlayer->smCharInfo.Life[0])&0xFFFF;		//����� ���� �ڵ�
				lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
				lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;
				lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
				ReformCharForm();

				ResetPotion();						//���� ���� ���� ����
				ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ

				ReformSkillInfo();					//��ų��ȣ�� ���� �ʱ�ȭ

				//���μ��� ���� ����
				SendProcessInfo();

				//������ �׷��� ýũ �ʱ�ȭ
				ResetEnergyGraph( 4 );

				//�̺��丮 �� �˻� �ʱ�ȭ
				ResetInvenMoney();

				CheckPlaySkill();		//��ų����
			}

			break;

		case smTRANSCODE_FAILRECORDDATA:
			//�ε� ����
			lpTransCharCommand = (smTRANS_CHAR_COMMAND *)pData->Buff;
			lpCurPlayer->smCharInfo.dwObjectSerial = lpTransCharCommand->WParam;
			lpCurPlayer->smCharInfo.dwLoginServerSafeKey = lpTransCharCommand->LParam;		//�α��� ���� ����Ű ��ġ

			if ( !lpCurPlayer->smCharInfo.JOB_CODE && !smConfig.DebugMode ) {
				//�ε� ����
				//quit = TRUE;				//���� ������
				Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 11");
				DisconnectFlag = GetCurrentTime();
				break;
			}


			//���ʷ� ����
			//���� ���� �ð�
			if ( !AdminCharDisable ) {

				//���θ��� ĳ������ Ȯ��
				CheckStartCharInfo();
				rsRecorder.dwNextRecordTime = dwPlayTime+RECORD_TIME_STEP;
				dwLoadingTime = 0;
				lpCurPlayer->SendCharInfo( smWsockServer );			//�÷��̾� ���� ����

				InitHelp();											//ĳ���� ó�� ������ ����
				cHelpPet.CheckPetData();							//���� �� ����
				cPCBANGPet.CheckPetData();
			}
			break;

		case smTRANSCODE_RECORDRESULT:
			//���� ��� �Լ�
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			rsRecorder.dwRecordTime = 0;
			rsRecorder.dwNextRecordTime = dwPlayTime+RECORD_TIME_STEP;
			dwLastRecordTime = dwPlayTime;
			if ( lpTransCommand->WParam ) {
				RecordFailCount = 0;				//���� ���� ���� ī���� �ʱ�ȭ
				if ( smConfig.DebugMode ) AddChatBuff( "SaveGame Sucess" );
			}
			else {
				RecordFailCount++;					//���� ����
				if ( smConfig.DebugMode ) AddChatBuff( "SaveGame Failed" );
			}

			//���� ĳ�� ����
 			cSinSiege.SetTaxRate( lpTransCommand->SParam );	//���� ����
			//���� ĳ�� ������ Ŭ�� ����
			SetBlessCastleMaster( lpTransCommand->EParam , 0 );

			break;

		case smTRANSCODE_BLESSCASTLE_TAX:
			//���� ĳ�� ���� ���� �Լ�
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
 			cSinSiege.SetTaxRate( lpTransCommand->SParam );	//���� ����
			break;

		case smTRANSCODE_ADDEXP:
		case smTRANSCODE_ADDEXP_PARTY:
			//����ġ ȹ��
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			lpChar = FindAutoPlayer( lpTransCommand->LParam );
			if ( lpChar && !lpChar->RecvExp ) {
				if ( lpTransCommand->SParam==((lpTransCommand->WParam*3 + lpTransCommand->LParam*13)*2002) ) {
					//����ġ �߰�
					CheckServerExp( pData->smMySock , lpTransCommand );

					AddExp( lpTransCommand->WParam );
					lpChar->RecvExp += lpTransCommand->WParam;
					addExp = lpTransCommand->WParam;

					if ( IPData[1]==smTRANSCODE_ADDEXP ) {
						CheckChangeJob3QuestItem( lpChar->smCharInfo.StatePoint );
					}
				}
				else {
					//��ŷ �õ��� ���� �ڵ� �Ű� ( ����ġ ���� ���� )
					SendSetHackUser2( 1800 , lpTransCommand->WParam );
				}
			}
			else {
				//��ŷ �õ��� ���� �ڵ� �Ű� ( ����ġ ���� ���� )
				//SendSetHackUser2( 1810 , lpTransCommand->WParam );
			}
			break;


		case smTRANSCODE_SERVER_INFO:
			TRANS_SERVER_LIST	*lpTransServerList;

			lpTransServerList = (TRANS_SERVER_LIST *)pData->Buff;
			sinItemTime = lpTransServerList->dwServerTime_t;			//TIME_T ����
			dwTime_ServerT = sinItemTime;			//���ӽ� �ð� (������ Time_T )
			dwTime_ConnectMS = GetCurrentTime();	//���ӽ� �ð� (Ŭ���̾�Ʈms)
			///////////////// Ŭ�� ���� ���� /////////////////////
			//lpTransServerList->ClanServerCount 

			cnt = lpTransServerList->ClanServerCount;
			if ( cnt ) {
				cnt=lpTransServerList->ServerCount;
				web_DB_ip_port_init( lpTransServerList->ServerInfo[cnt].szServerIp1 , lpTransServerList->ServerInfo[cnt].dwServerPort1 , 
					lpTransServerList->ServerInfo[cnt].szServerIp2 , lpTransServerList->ServerInfo[cnt].dwServerPort2 ,
					lpTransServerList->ClanTicket );
			}
			//////////////////////////////////////////////////////

			CheckProcessModule();		//���˻� �Ͽ� ������ ������

			//////////////////////////////////////////////////////
			HoRecvMessage( IPData[1] , pData->Buff );
			break;

		case smTRANSCODE_ID_SETUSERINFO:
			if ( smConfig.DebugMode ) {
				//����� ��� ��� ĳ�� ���� ����
				TRANS_USERCHAR_INFO	*lpTransUserCharInfo = (TRANS_USERCHAR_INFO	*)pData->Buff;
				FILE	*fp;
				for(cnt=0;cnt<lpTransUserCharInfo->PlayUserCount;cnt++) {
					if ( !lpTransUserCharInfo->CharInfo[cnt].szModelName2[0] ) {
						fp = fopen(lpTransUserCharInfo->CharInfo[cnt].szModelName,"rb");
						if( !fp ) {
							x = lstrlen(lpTransUserCharInfo->CharInfo[cnt].szModelName);
							lpTransUserCharInfo->CharInfo[cnt].szModelName[x-1]='x';
							fp = fopen(lpTransUserCharInfo->CharInfo[cnt].szModelName,"rb");
						}
						if ( fp ) {
							fclose(fp);
						}
						else {
							lstrcpy( lpTransUserCharInfo->CharInfo[cnt].szModelName,
								"char\\monster\\hopy\\hopy.ini" );
						}
					}
				}
			}

		case smTRANSCODE_ISRECORDDATA:
		case smTRANSCODE_INSRECORDDATA:
		case smTRANSCODE_FAILCONNECT:
			//ȣ�޼��� ���ù�
			HoRecvMessage( IPData[1] , pData->Buff );
			break;

		case smTRANSCODE_SHOP_ITEMLIST:
			//���� ������ ��� ����
			
			//���హ�� ��
			ComparePotion();

			sinRecvMessage( IPData[1] , pData->Buff );

			Send_GetBlessCastleTax();		//���� ���� �䱸
/*
			sITEMINFO *lpItemInfo;
			BYTE *lpBuff;
			lpTransSellItem = (smTRANS_SELL_ITEM *)pData->Buff;

			lpItemInfo = new sITEMINFO[ lpTransSellItem->ItemCounter ];
			lpBuff = (BYTE *)pData->Buff+sizeof(smTRANS_SELL_ITEM);
			for( cnt=0;cnt<lpTransSellItem->ItemCounter;cnt++) {
				lpBuff += DecodeCompress( lpBuff , (BYTE *)&lpItemInfo[cnt] );
			}

			delete lpItemInfo;
*/
			break;

		case smTRANSCODE_SKILL_MENU:
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			//��ų �޴� ���� �䱸

			if ( lpCurPlayer->OnStageField>=0 ) {

				if ( dwQuestDelayTime>dwPlayTime ) lpTransCommand->WParam = 0;

				if ( StageField[ lpCurPlayer->OnStageField ]->FieldCode==3 ) {
					//��ī����
					if ( lpCurPlayer->smCharInfo.JOB_CODE<5 ) {
						sinSkillMaster(lpTransCommand->WParam);
					}
				}
				if ( StageField[ lpCurPlayer->OnStageField ]->FieldCode==21 ) {
					//�ʶ���
					if ( lpCurPlayer->smCharInfo.JOB_CODE>=5 )
					{
						sinSkillMaster(lpTransCommand->WParam);
					}
				}

				dwQuestDelayTime = dwPlayTime+1000*5;		//5�� ������
			}

			//sinSkillMaster(lpTransCommand->WParam);
			break;

		case smTRANSCODE_VERSION:
			//���� Ȯ��
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			if ( pData->smMySock==smWsockDataServer ) {
				//����Ÿ ���� ���� ���
				memcpy( &TransServerConnectInfo , lpTransCommand , sizeof(smTRANS_COMMAND) );
			}
			if ( pData->smMySock==smWsockDataServer || pData->smMySock==smWsockServer ) {
				//���� ���� Ȯ��
				CheckServerInfo( lpTransCommand );
			}
			break;

		case smTRANSCODE_CHECK_NETSTATE:
			//����� Ȯ�� ��Ŷ

			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			//�ý��� �ð��� ���ӽð����� ��ȯ
			dwTime = ConvSysTimeToGameTime(lpTransCommand->SParam);
			//�ð����̰� ���� ���� �ð躸��
			if ( abs((long)dwGameWorldTime-(long)dwTime)>10 ) 
				dwGameWorldTime=dwTime;

			dwLastRecvGameServerTime = GetCurrentTime();
			rsRecvServer = TRUE;

			if ( lpTransCommand->LParam ) {
				//����Ÿ �������� ���� ��Ʈ������Ÿ�� LParam���� ������ nProtect �˻� �ǽ�
				if ( Check_nProtect()==FALSE ) {
					quit = TRUE;
				}
			}

			if ( lpTransCommand->WParam && lpTransCommand->EParam ) {
				//�������� �޸� ���� ���ؼ� ��������
				dwMemFunChkCode = funcCheckMemSum( lpTransCommand->WParam , lpTransCommand->EParam );
				dwMemFunChkCode = (dwMemFunChkCode<<16)|(lpTransCommand->WParam&0xFFFF);
			}

			if ( smConfig.DebugMode && DisplayDebug ) 
				AddChatBuff( "Recv(1)" );
			break;

		case smTRANSCODE_CHECK_NETSTATE2:
		case smTRANSCODE_CHECK_NETSTATE20:
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			if ( lpTransCommand->LParam ) {
				//����Ÿ �������� ���� ��Ʈ������Ÿ�� LParam���� ������ nProtect �˻� �ǽ�
				if ( Check_nProtect()==FALSE ) {
					quit = TRUE;
				}
			}

			dwLastRecvGameServerTime2 = GetCurrentTime();
			rsRecvDataServer = TRUE;

			if ( lpTransCommand->WParam && lpTransCommand->EParam ) {
				//�������� �޸� ���� ���ؼ� ��������
				dwMemFunChkCode = funcCheckMemSum( lpTransCommand->WParam , lpTransCommand->EParam );
				dwMemFunChkCode = (dwMemFunChkCode<<16)|(lpTransCommand->WParam&0xFFFF);
			}

			if ( smConfig.DebugMode && DisplayDebug ) AddChatBuff( "Recv(2)" );
			break;

		case smTRANSCODE_CHECK_NETSTATE3:
			dwLastRecvGameServerTime3 = GetCurrentTime();
			rsRecvUserServer = TRUE;
			if ( smConfig.DebugMode && DisplayDebug ) AddChatBuff( "Recv(3)" );
			break;

		case smTRANSCODE_CHECK_NETSTATE4:
		case smTRANSCODE_CHECK_NETSTATE5:
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			if ( AreaServerMode ) {
				if ( lpWSockServer_Area[0] && lpWSockServer_Area[0]==(smWINSOCK *)lpTransCommand->EParam ) {
					lpWSockServer_Area[0]->dwDeadLockTime = GetCurrentTime();
					if ( smConfig.DebugMode && DisplayDebug ) AddChatBuff( "Recv(50)" );
				}
				if ( lpWSockServer_Area[1] && lpWSockServer_Area[1]==(smWINSOCK *)lpTransCommand->EParam ) {
					lpWSockServer_Area[1]->dwDeadLockTime = GetCurrentTime();
					if ( smConfig.DebugMode && DisplayDebug ) AddChatBuff( "Recv(51)" );
				}
			}
			else {
				dwLastRecvGameServerTime4 = GetCurrentTime();
				rsRecvExtendServer = TRUE;
				if ( smConfig.DebugMode && DisplayDebug ) AddChatBuff( "Recv(4)" );
			}
			break;

		case smTRANSCODE_WARPFIELD:
			//������ ��ġ ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			if ( lpTransCommand->WParam==rsSOD_FIELD && 
				lpCurPlayer->OnStageField>=0 && StageField[lpCurPlayer->OnStageField]->FieldCode==rsSOD_VILLAGE ) {
					//SOD �� ���� ���� �ε�ȭ�� ǥ��
					if ( !BellatraEffectInitFlag ) {
						CreateBellatraFontEffect();
						BellatraEffectInitFlag = TRUE;
					}

					if ( SoD_SetFontEffect.eBL_Type!=E_BL_LODING ) {
						SetBellatraFontEffect( E_BL_LODING, &SoD_SetFontEffect );
						sSodScore.SodNextStageNum = -2;
						sSodScore.dwSoD_NextStageTime = dwPlayTime+3000;
						sSodScore.NextRound = 0;
						StopBGM();
					}
			}


			CloseEachPlayer();

			WarpFieldNearPos( lpTransCommand->WParam  , lpTransCommand->SParam, lpTransCommand->LParam , &x, &z );

			lpCurPlayer->SetPosi(lpTransCommand->SParam,0, lpTransCommand->LParam  ,0,0,0 );
			TraceCameraPosi.x = lpCurPlayer->pX;
			TraceCameraPosi.y = lpCurPlayer->pY;
			TraceCameraPosi.z = lpCurPlayer->pZ;
			TraceTargetPosi.x = lpCurPlayer->pX;
			TraceTargetPosi.y = lpCurPlayer->pY;
			TraceTargetPosi.z = lpCurPlayer->pZ;
	
			//�ٴ� Ȯ��
			lpCurPlayer->OnStageField = -1;
			if ( smGameStage[0] ) {
				cnt = smGameStage[0]->GetFloorHeight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ ,lpCurPlayer->Pattern->SizeHeight );	
				lpCurPlayer->OnStageField = 0;
			}
			if ( cnt==CLIP_OUT && smGameStage[1] ) {
				lpCurPlayer->OnStageField = 1;
			}
			break;

		case smTRANSCODE_TRADEITEMS:
			//������ ��ȯâ ���� ����
			//��ȯâ ������ ����
			if ( cTrade.OpenFlag && cTrade.TradeCharCode==((TRANS_TRADEITEMS *)pData->Buff)->dwSender ) {
				RecvTradeItems( (TRANS_TRADEITEMS *)pData->Buff );
			}
			break;

		case smTRANSCODE_TRADE_ITEMKEY:
			//Ʈ���̵� �Ϸ� Ű ����
			//������ ��ȯ ����Ű ����
			if ( DisconnectFlag ) 
				break;
			//if ( cTrade.OpenFlag && !TradeRecvItemKeyFlag ) {
			if ( !TradeRecvItemKeyFlag ) {
				if ( RecvTradeSucessKey( (TRANS_TRADE_ITEMKEY *)pData->Buff , &sTradeRecv )==TRUE ) {
					TradeRecvItemKeyFlag = TRUE;
					//���� ���� �Ϸ�
					if ( TradeItemSucessFlag ) {
						if ( sTrade.Money ) AddInvenMoney(-(sTrade.Money-193));			//�κ� �� ýũ
						if ( sTradeRecv.Money ) AddInvenMoney(sTradeRecv.Money-193);	//�κ� �� ýũ

						memset( sTrade.TradeItem , 0 , sizeof(sITEM)*MAX_TRADE_ITEM );
						sTrade.Money = 0;
						ReformTrade( &sTrade );

						CloseEachPlayer();
						cTrade.CheckOkTradeItem();
						ResetPotion();						//���� ���� ���� ����
						SaveGameData();						//������ ����
						ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
					}
				}
				else {
					//���� ����
					cTrade.CancelTradeItem();
				}
			}
			break;

		case smTRANSCODE_TRADE_READYITEM:
			//������ ��ȯ ���� ���� Ȯ��
			//if ( cTrade.OpenFlag ) {
				//if(sTrade.CheckFlag && sTradeRecv.CheckFlag){ //��ȯ!		
				if ( DisconnectFlag ) 
					break;
				if ( TradeItemSucessFlag ) 
					break;

				if ( sTrade.CheckFlag ) {
					//������ ��ȯ ���� Ȯ�� ����
					TradeItemSucessFlag = TRUE;
					sTradeRecv.CheckFlag = TRUE;
					if ( RecvTradeCheckItem( (TRANS_TRADE_CHECKITEM *)pData->Buff )==TRUE ) {
						SendTradeSucessKey( ((TRANS_TRADE_CHECKITEM *)pData->Buff)->dwSender );

						if ( TradeRecvItemKeyFlag ) {
							if ( sTrade.Money ) AddInvenMoney(-(sTrade.Money-193));			//�κ� �� ýũ
							if ( sTradeRecv.Money ) AddInvenMoney(sTradeRecv.Money-193);	//�κ� �� ýũ

							memset( sTrade.TradeItem , 0 , sizeof(sITEM)*MAX_TRADE_ITEM );
							sTrade.Money = 0;
							ReformTrade( &sTrade );

							CloseEachPlayer();
							cTrade.CheckOkTradeItem();
							ResetPotion();						//���� ���� ���� ����
							SaveGameData();						//������ ����
							ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
						}
					}
					break;
				}
				//}
				//}
			//}
			//��ȯ ���� ����
			cTrade.CancelTradeItem();
			break;

		case smTRANSCODE_TRADE_REQUST:
			//������ ��ȯ ��û
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			lpChar = FindAutoPlayer( lpTransCommand->SParam );

			if ( lpChar && lpChar->DisplayFlag && lpChar->smCharInfo.szName[0] && 
				!cWareHouse.OpenFlag && !cCraftItem.OpenFlag &&	!cAging.OpenFlag && !cMyShop.OpenFlag || cMixtureReset.OpenFlag) { // ������ - �ͽ��� ���� â �߰�

				switch( lpTransCommand->LParam ) {
				case 0:
					//��ȯ �ŷ� �Ÿ� Ȯ��
					if ( GetTradeDistance( lpChar )==FALSE ) break;

					if ( lpCurPlayer->smCharInfo.Weight[0]>lpCurPlayer->smCharInfo.Weight[1] ) {
						cMessageBox.ShowMessageEvent(mgWeightOver);		//�����ʰ�
						break;
					}


					//�ŷ� �䱸 ����
					if( !cTrade.OpenFlag ) {
						sinRequestTrade( lpTransCommand->SParam , lpChar->smCharInfo.szName );
						if ( DispEachMode==0 ) {//chrEachMaster==0 ) {
							OpenEachPlayer( lpChar );
							DispEachMode = 0;
							if ( lpChar ) InterfaceParty.latest_Insert(lpChar->smCharInfo.szName);	//�ŷ��� ��� �ֱٸ�Ͽ� ���
						}
					}
					break;
				case 1:
					//��ȯ �ŷ� �Ÿ� Ȯ��
					if ( GetTradeDistance( lpChar )==FALSE ) break;
					if ( cShop.OpenFlag ) break;

					if ( lpCurPlayer->smCharInfo.Weight[0]>lpCurPlayer->smCharInfo.Weight[1] ) {
						cMessageBox.ShowMessageEvent(mgWeightOver);		//�����ʰ�
						break;
					}

					//�ŷ� ���� ����
					if( !cTrade.OpenFlag )
						SendRequestTrade( lpTransCommand->SParam , 2 );
				case 2:
					//�ŷ�â ����
					if( !cTrade.OpenFlag ) {
						cTrade.OpenFlag = TRUE;
						cInvenTory.OpenFlag = TRUE;
						cInterFace.CheckAllBox( SIN_TRADE );
						cTrade.TradeCharCode = lpTransCommand->SParam;
						TradeItemSucessFlag = 0;
						TradeRecvItemKeyFlag = 0;
						TradeSendSucessFlag = 0;
						TradeItemCancelCount = 0;
						TransLastRecvTrade.code = 0;
						memset(&sTrade,0,sizeof(sTRADE)); 
						memset(&sTradeRecv,0,sizeof(sTRADE)); 
						ReformTrade( (void *)&sTrade );
						ReformTrade( (void *)&sTradeRecv );
						lstrcpy( cTrade.szTradeCharName , lpChar->smCharInfo.szName );

						if ( lpCurPlayer->MoveFlag ) {
							//�̵����� ĳ���� ���� ��Ű��
							CancelAttack();
							lpCurPlayer->MoveFlag = 0;
						}
						EachTradeButton = 0;
						OpenEachPlayer(lpChar);
						DispEachMode = TRUE;
						if ( lpChar ) InterfaceParty.latest_Insert(lpChar->smCharInfo.szName);	//�ŷ��� ��� �ֱٸ�Ͽ� ���
					}
					break;
				case 3:
					//�ŷ� ���
					if( cTrade.OpenFlag ) {
						if ( TradeRecvItemKeyFlag ) {
							memset( sTrade.TradeItem , 0 , sizeof(sITEM)*MAX_TRADE_ITEM );
							sTrade.Money = 0;
							ReformTrade( &sTrade );
						}
						cTrade.CancelTradeItem();
					}
					break;
				}
			}
			break;

		case smTRANSCODE_FINDCRACK:
			//������ ���� ũ�� �˻� �䱸
			//ũ�� ýũ
			CheckCracker((TRANS_FIND_CRACKER *)pData->Buff );
			break;


		case smTRANSCODE_PARTY_REQUEST:
			//��Ƽ ��û ����
			if ( !cWareHouse.OpenFlag ) {
				lpTransCharCommand = (smTRANS_CHAR_COMMAND *)pData->Buff;
				SetInterfaceParty( lpTransCharCommand->szName , lpTransCharCommand->WParam );
			}
			break;

		case smTRANSCODE_PARTY_UPDATE:
			//��Ƽ���� ����
			TRANS_PARTY_USERS	*lpTransPartyUsers;

			lpTransPartyUsers = (TRANS_PARTY_USERS *)pData->Buff;

			if ( !InterfaceParty.PartyMemberCount ) {
				//InterfaceParty.PartyPosState = PARTY_START;
				InterfaceParty.Main_menuSet(1);	//���θ޴� ����Ʈ, ����, ģ���� �ٲ�.
				if ( InterfaceParty.PartyPosState==PARTY_NONE )	ShowParty();
			}


			InterfaceParty.MouseDownPartyMember = -1;
/*
			//MAX_PARTY_MEMBER
			for(cnt=0;cnt<lpTransPartyUsers->PartyUserCount;cnt++) {
				InterfaceParty.PartyMember[cnt].ChrCode = lpTransPartyUsers->PartyUser[cnt].dwObjectSerial;
				InterfaceParty.PartyMember[cnt].Life  = (lpTransPartyUsers->PartyUser[cnt].Life[0]*100)/lpTransPartyUsers->PartyUser[cnt].Life[1];
				InterfaceParty.PartyMember[cnt].Level = lpTransPartyUsers->PartyUser[cnt].Level;
				InterfaceParty.PartyMember[cnt].CharacterPos.x = lpTransPartyUsers->PartyUser[cnt].x;
				InterfaceParty.PartyMember[cnt].CharacterPos.z = lpTransPartyUsers->PartyUser[cnt].z;
				InterfaceParty.PartyMember[cnt].CharacterPos.y = 0;
				lstrcpy( InterfaceParty.PartyMember[cnt].Name , lpTransPartyUsers->PartyUser[cnt].szName );
				lstrcpy( InterfaceParty.PartyMember[cnt].ModelName1 , lpTransPartyUsers->PartyUser[cnt].szModelName );
				lstrcpy( InterfaceParty.PartyMember[cnt].ModelName2 , lpTransPartyUsers->PartyUser[cnt].szModelName2 );
				InterfaceParty.PartyMember[0].JangFlag = 0;
			}
			InterfaceParty.PartyMemberCount = lpTransPartyUsers->PartyUserCount;
			InterfaceParty.PartyMember[0].JangFlag = TRUE;
*/
			//MAX_PARTY_MEMBER
			for(cnt=0;cnt<lpTransPartyUsers->PartyUserCount;cnt++) {

				hoPartyMember	save_PM;
				ZeroMemory((void*)&save_PM, sizeof(save_PM));

				save_PM.ChrCode = lpTransPartyUsers->PartyUser[cnt].dwObjectSerial;
				save_PM.Life  = (lpTransPartyUsers->PartyUser[cnt].Life[0]*100)/lpTransPartyUsers->PartyUser[cnt].Life[1];
				save_PM.Level = lpTransPartyUsers->PartyUser[cnt].Level;
				save_PM.CharacterPos.x = lpTransPartyUsers->PartyUser[cnt].x;
				save_PM.CharacterPos.z = lpTransPartyUsers->PartyUser[cnt].z;
				save_PM.CharacterPos.y = 0;
				lstrcpy( save_PM.Name , lpTransPartyUsers->PartyUser[cnt].szName );
				lstrcpy( save_PM.ModelName1 , lpTransPartyUsers->PartyUser[cnt].szModelName );
				lstrcpy( save_PM.ModelName2 , lpTransPartyUsers->PartyUser[cnt].szModelName2 );
				save_PM.JangFlag = 0;
				if(cnt==0)
					save_PM.JangFlag = TRUE;

				InterfaceParty.AddMember(&save_PM,cnt);
			}
			InterfaceParty.AddMember_memberSu(lpTransPartyUsers->PartyUserCount);

			//��� ĳ���� ��Ƽ ���Կ��� Ȯ��
			RequestPartyPlayers( 0 );
			break;

		case smTRANSCODE_PARTY_PLAYUPDATE:
			//��Ƽ �÷��� ���� ����
			TRANS_PARTY_PLAYINFO	*lpTransPartyPlayInfo;
			lpTransPartyPlayInfo = (TRANS_PARTY_PLAYINFO *)pData->Buff;
/*
			for(cnt=0;cnt<lpTransPartyPlayInfo->PartyUserCount;cnt++) {
				if ( InterfaceParty.PartyMember[cnt].ChrCode==lpTransPartyPlayInfo->PlayInfo[cnt].dwObjectSerial ) {
					InterfaceParty.PartyMember[cnt].Life  = (lpTransPartyPlayInfo->PlayInfo[cnt].Life[0]*100)/lpTransPartyPlayInfo->PlayInfo[cnt].Life[1];
					InterfaceParty.PartyMember[cnt].Level = lpTransPartyPlayInfo->PlayInfo[cnt].Level;
					InterfaceParty.PartyMember[cnt].CharacterPos.x = lpTransPartyPlayInfo->PlayInfo[cnt].x;
					InterfaceParty.PartyMember[cnt].CharacterPos.z = lpTransPartyPlayInfo->PlayInfo[cnt].z;
				}
			}
*/
			for(cnt=0;cnt<lpTransPartyPlayInfo->PartyUserCount;cnt++) {

				hoPartyMember	save_PM;
				ZeroMemory((void*)&save_PM, sizeof(save_PM));

				save_PM.ChrCode = lpTransPartyPlayInfo->PlayInfo[cnt].dwObjectSerial;
				save_PM.Life  = (lpTransPartyPlayInfo->PlayInfo[cnt].Life[0]*100)/lpTransPartyPlayInfo->PlayInfo[cnt].Life[1];
				save_PM.Level = lpTransPartyPlayInfo->PlayInfo[cnt].Level;
				save_PM.CharacterPos.x = lpTransPartyPlayInfo->PlayInfo[cnt].x;
				save_PM.CharacterPos.z = lpTransPartyPlayInfo->PlayInfo[cnt].z;

				InterfaceParty.PARTY_PLAYUPDATE(&save_PM,cnt);
			}

			if ( lpTransPartyPlayInfo->PlayInfo[0].dwObjectSerial==lpCurPlayer->dwObjectSerial ) {
				//���� ���� ( ���μ����� ��Ƽ���� ������Ʈ )
				if ( smWsockServer && smWsockServer!=smWsockUserServer ) {
					smWsockServer->Send( (char *)lpTransPartyPlayInfo , lpTransPartyPlayInfo->size , TRUE );
				}
				if ( smWsockExtendServer && smWsockExtendServer!=smWsockServer ) {
					smWsockExtendServer->Send( (char *)lpTransPartyPlayInfo , lpTransPartyPlayInfo->size , TRUE );
				}
			}
			break;

		case smTRANSCODE_PARTY_RELEASE:
			//��Ƽ Ż�� �Ǵ� �ػ�
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
/*
			InterfaceParty.PartyMemberCount = 0;
			InterfaceParty.PartyPosState = PARTY_END;
			InterfaceParty.MouseDownPartyMember = -1;
*/
			InterfaceParty.PARTY_RELEASE();

			//��� ĳ���� ��Ƽ ���Կ��� Ȯ��
			RequestPartyPlayers( 0 );

			//���μ����� ��Ƽ �ػ����� ����
			if ( smWsockServer && smWsockServer!=smWsockUserServer ) {
				smWsockServer->Send( (char *)lpTransCommand , lpTransCommand->size , TRUE );
			}
			//���μ����� ��Ƽ �ػ����� ����
			if ( smWsockExtendServer && smWsockExtendServer!=smWsockServer ) {
				smWsockExtendServer->Send( (char *)lpTransCommand , lpTransCommand->size , TRUE );
			}
			break;

		case smTRANSCODE_WAREHOUSE:
			//������ ���� â�� ����Ÿ �Լ�
			//â�� ��������
			if ( !cWareHouse.OpenFlag && !cTrade.OpenFlag && !cMyShop.OpenFlag ) {
				if ( LoadWareHouse( (TRANS_WAREHOUSE *)pData->Buff , &sWareHouse )==TRUE ) {
					cWareHouse.LoadWareHouseItemIamge();
					cWareHouse.BackUpInvenItem();
					ResetInvenItemCode();		//�̺��丮 ������ �ڵ� �ʱ�ȭ
					ResetInvenMoney();			//�̺��丮 �� �˻� �ʱ�ȭ
				}
			}
			break;

		case smTRANSCODE_OPEN_WAREHOUSE:
			//â�� ����Ÿ �䱸
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			if ( lpCurPlayer->smCharInfo.Weight[0]>lpCurPlayer->smCharInfo.Weight[1] ) {
				cMessageBox.ShowMessageEvent(mgWeightOver);		//�����ʰ�
				break;
			}

			if ( smWsockDataServer && !cWareHouse.OpenFlag && !cTrade.OpenFlag ) {
				smWsockDataServer->Send2( (char *)lpTransCommand , lpTransCommand->size , TRUE );
			}
			break;

		case smTRANSCODE_OPEN_MIXITEM:
			//������ ����â ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			if ( lpCurPlayer->smCharInfo.Weight[0]>lpCurPlayer->smCharInfo.Weight[1] ) {
				cMessageBox.ShowMessageEvent(mgWeightOver);		//�����ʰ�
				break;
			}

			if ( lpTransCommand->WParam==200 ) {
				//���ݼ���
				cInterFace.CheckAllBox(SIN_CRAFTITEM); 
				cCraftItem.ForceFlag = 1; 
			}
			else
				cInterFace.CheckAllBox( SIN_CRAFTITEM );		//������ �ͽ�

			break;

		case smTRANSCODE_OPEN_COLLECT:
			//���Ž� TV �̺�Ʈ ( ���� ��� )
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			if ( lpTransCommand->WParam==5 ) {
				//SOD ȸ�� ó��
				cSinSod2.ShowSod2MessageBox();
			}
			else
				sinShowEventSmashingTV();	//SOD ����

			break;

		case smTRANSCODE_OPEN_EVENTGIFT:
			//��ǰ �̺�Ʈ
			//if ( sinSendEventItem( 0 )==TRUE ) {

			if(sinChar->wVersion[1] == 1)
				cMessageBox.ShowMessage(MESSAGE_ALREADY_INITSTATE);
			else
				cMessageBox.ShowMessage3(  MESSAGE_EVENT_GYUNGFUM , "" );
			//}
			break;

		case smTRANSCODE_OPEN_AGING:
			//����¡â ����
			if ( lpCurPlayer->smCharInfo.Weight[0]>lpCurPlayer->smCharInfo.Weight[1] ) {
				cMessageBox.ShowMessageEvent(mgWeightOver);		//�����ʰ�
				break;
			}
			cInterFace.CheckAllBox(SIN_AGING );
			break;
		case smTRANSCODE_OPEN_SMELTING:
			// pluto ����
			if( lpCurPlayer->smCharInfo.Weight[0]>lpCurPlayer->smCharInfo.Weight[1] )
			{
				cMessageBox.ShowMessageEvent( mgWeightOver );
				break;
			}
			cInterFace.CheckAllBox( SIN_SMELTING );
			break;
		case smTRANSCODE_OPEN_MANUFACTURE:
			// pluto ����
			if( lpCurPlayer->smCharInfo.Weight[0]>lpCurPlayer->smCharInfo.Weight[1] )
			{
				cMessageBox.ShowMessageEvent( mgWeightOver );
				break;
			}
			cInterFace.CheckAllBox( SIN_MANUFACTURE );
			break;
		case smTRANSCODE_OPEN_MIXTURE_RESET:	
			// ������ - �ͽ��� ���� â ����
			if( lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1] )
			{
				cMessageBox.ShowMessageEvent( mgWeightOver );
				break;
			}
			cInterFace.CheckAllBox( SIN_MIXTURE_RESET );
			break;
		case smTRANSCODE_SAVETHROWITEM2:
			//���� ������ ��� Ȯ�� ���� ( ������ ������ �� )
			ThrowItemToServer((smTRANS_COMMAND_EX *)pData->Buff );
			break;

		case smTRANSCODE_CLOSECLIENT:
			//������ Ŭ���̾�Ʈ ������
			DisconnectServerCode = 0;
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 13");
			DisconnectFlag = GetCurrentTime();
			break;

		case smTRANSCODE_PLAYERKILLING:
			//PK��,����ġ ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			//�׾ ����ġ ����
			if ( DeadPlayerExp( lpCurPlayer )==TRUE ) {
				//����ġ ������ ( �������� ���� )
				//WarpPrisonField( &x,&z );
				//lpCurPlayer->SetPosi( x,0, z ,0,0,0 );
				//TraceCameraPosi.x = lpCurPlayer->pX;
				//TraceCameraPosi.y = lpCurPlayer->pY;
				//TraceCameraPosi.z = lpCurPlayer->pZ;
				//TraceTargetPosi.x = lpCurPlayer->pX;
				//TraceTargetPosi.y = lpCurPlayer->pY;
				//TraceTargetPosi.z = lpCurPlayer->pZ;

				//lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_RESTART );		//���۸��
				//StartEffect( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, EFFECT_GAME_START1 );
				//SetDynLight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 100, 100,100,0, 200 );
				//CharPlaySound( lpCurPlayer );
				//dwBattleTime = 0;
				//dwBattleQuitTime = 0;

				////�ٴ� Ȯ��
				//lpCurPlayer->OnStageField = -1;
				//if ( smGameStage[0] ) {
				//	cnt = smGameStage[0]->GetFloorHeight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ ,lpCurPlayer->Pattern->SizeHeight );	
				//	lpCurPlayer->OnStageField = 0;
				//}
				//if ( cnt==CLIP_OUT && smGameStage[1] ) {
				//	lpCurPlayer->OnStageField = 1;
				//}
				//if ( smConfig.DebugMode ) {
				//	lpCurPlayer->smCharInfo.wPlayerKilling[0] = 16*2;			//2�е��� ������ ����
				//}
				//else {
				//	lpCurPlayer->smCharInfo.wPlayerKilling[0] = 16*30;			//30�е��� ������ ����
				//}
			}
			break;

		case smTRANSCODE_SEND_DATASERVER:
			//����Ÿ ������ ���� ����
			if ( smWsockDataServer ) {
				smWsockDataServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			}
			break;

		case smTRANSCODE_SEND_GAMESERVER:
			//���� ������ ���� ����
			if ( smWsockServer ) {
				smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			}
			if ( smWsockUserServer && smWsockUserServer!=smWsockServer ) {
				smWsockUserServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			}
			if ( smWsockExtendServer && smWsockExtendServer!=smWsockServer ) {
				smWsockExtendServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			}
			break;

		case smTRANSCODE_SEND_AREASERVER:
			//���������� ���� ����
			lpsmSock = GetAreaServerSock();		//�������� ������ ã��
			if ( lpsmSock )
				lpsmSock->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			else
				smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );

			break;

		case smTRANSCODE_SEND_AGAME_SERVER:
			//���� �������� ��� ����
			lpsmSock = GetAreaServerSock();		//�������� ������ ã��

			for( int i = 0 ; i < SINUPKEEPITEM_MAX ; i++ )
			{

				if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::THIRD_EYES )
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_ThirdEyesTime = 0;
					}
				}

				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::EXPUP_POTION )
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_ExpUpPotionTime = 0;
					}
				}

				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::VAMPIRIC_CUS )
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_VampiricCuspidTime = 0;
					}
				}

				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::MANA_RECHAR_P )
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_ManaRechargingPTime = 0;
					}
				}
			}



			if ( lpsmSock ) {
				lpsmSock->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );

				if ( lpsmSock!=smWsockServer )
					smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			}
			else
				smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );

			break;

		case smTRANSCODE_FUNCTION_MEM2:
			//�˻� �޸� ����� ��ä�� ����2
			RecvMemFuncData2( (TRANS_FUNC_MEMORY *)pData->Buff );
			break;

		case smTRANSCODE_CRAFTITEM:
			//������ �ͽ��� ��� ����
			if ( ((sCRAFTITEM_SERVER *)pData->Buff)->Result==TRUE ) {

				cCraftItem.sinRecvCraftItemResult( (sCRAFTITEM_SERVER *)pData->Buff );

				cInvenTory.DeleteInvenItemToServer( ((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.CODE ,
					((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
					((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum );


				//�κ��丮�� ������ �������� ã�� �˻��Ͽ� ã�´�
				if ( FindInvenItem( ((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.CODE ,
					((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
					((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum )!=0 ) {
						//�κ��丮 ������ ���� ����
						SendInvenItemError( 0 , ((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.CODE ,
							((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
							((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum );
						break;
					}

				ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
				SaveGameData();
				break;

			}
			cCraftItem.sinRecvCraftItemResult( (sCRAFTITEM_SERVER *)pData->Buff );
			ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			SaveGameData();
			break;

		case smTRANSCODE_AGINGITEM:
			//������ ����¡ ��� ����
			if ( ((sCRAFTITEM_SERVER *)pData->Buff)->Result ) {

				cAging.sinRecvAgingItemResult( (sCRAFTITEM_SERVER *)pData->Buff );

				cInvenTory.DeleteInvenItemToServer( ((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.CODE ,
					((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
					((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum );


				//�κ��丮�� ������ �������� ã�� �˻��Ͽ� ã�´�
				if ( FindInvenItem( ((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.CODE ,
					((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
					((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum )!=0 ) {
						//�κ��丮 ������ ���� ����
						SendInvenItemError( 0 , ((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.CODE ,
							((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
							((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum );
						break;
					}

				ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
				SaveGameData();
				break;

			}
			cAging.sinRecvAgingItemResult( (sCRAFTITEM_SERVER *)pData->Buff );
			ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			SaveGameData();
			break;

		case smTRANSCODE_AGING_UPGRADE:
			//����¡ �Ϸ�� ������ �ޱ�
			sinRecvAgingIncreStateItem( &((TRANS_ITEMINFO *)pData->Buff)->Item );
			TransAgingItemFlag = FALSE;
			ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			SaveGameData();

			//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ� ������
			SendCommandUser( smCOMMNAD_USER_AGINGUP , 0 , 0 , 0 );

			if ( (((TRANS_ITEMINFO *)pData->Buff)->Item).CODE==cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].CODE &&
				cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.ItemName[0] ) {

				//������
				sinSetCharItem(cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].CODE , cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].SetModelPosi , TRUE );
			}
			else
			if ( (((TRANS_ITEMINFO *)pData->Buff)->Item).CODE==cInvenTory.InvenItem[ sInven[1].ItemIndex-1 ].CODE &&
				cInvenTory.InvenItem[ sInven[1].ItemIndex-1 ].sItemInfo.ItemName[0] ) {

				//�޼�
				sinSetCharItem(cInvenTory.InvenItem[ sInven[1].ItemIndex-1 ].CODE , cInvenTory.InvenItem[ sInven[1].ItemIndex-1 ].SetModelPosi , TRUE );

			}
			break;

		case smTRANSCODE_MAKE_LINKCORE:
			//��ϵ� ��ũ �ھ� ����
			cInvenTory.RecvLinkCore( &((TRANS_ITEMINFO *)pData->Buff)->Item );
			ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			SaveGameData();
			break;

		case smTRANSCODE_USE_LINKCORE:
			//��ũ �ھ� ��� ����
			SucessLinkCore( (smTRANS_COMMAND_EX *)pData->Buff );
			break;

		case smTRANSCODE_WINGITEM:
			//�� ������ ȹ�� ����
			/*
			if ( ((sCRAFTITEM_SERVER *)pData->Buff)->Result!=FALSE ) {
				wsprintf( szBuff, mgRecvItem , ((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.ItemName );
				cMessageBox.ShowMessageEvent( szBuff );
			}
			*/
			RecvWingItem((sCRAFTITEM_SERVER *)pData->Buff);
			ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			SaveGameData();
			break;

		case smTRANSCODE_ITEM_FORCEORB:
			//�������� ������ ȹ�� ����
			RecvForceOrbItem( (TRANS_ITEMINFO_GROUP2 *)pData->Buff );
			ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			SaveGameData();
			break;

		case smTRANSCODE_SHOPTITEM:
			//���హ�� ��
			if ( (((TRANS_BUY_SHOPITEM *)pData->Buff)->sItem.sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1)) {
				ComparePotion();
			}

			//�������� ������ ����
			cShop.RecvBuyItemToServer( &((TRANS_BUY_SHOPITEM *)pData->Buff)->sItem , ((TRANS_BUY_SHOPITEM *)pData->Buff)->ItemCount );

			if ( (((TRANS_BUY_SHOPITEM *)pData->Buff)->sItem.sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1)){
				//���� ���� ���� ����
				ResetPotion();
			}

			ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			break;

		case smTRANSCODE_SHOP_SELLITEM:
			//������ �� ��� ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			//�κ��丮�� ������ �������� ã�� �˻��Ͽ� ã�´�
			if ( FindInvenItem( lpTransCommand->WParam , lpTransCommand->LParam , lpTransCommand->SParam )==0 ) {
				CheckCharForm();//���� 
				sinPlusMoney(lpTransCommand->EParam);
				ReformCharForm();//������ 
				AddInvenMoney( lpTransCommand->EParam );
			}
			else {
				SendInvenItemError( 0 , lpTransCommand->WParam , lpTransCommand->LParam , lpTransCommand->SParam );
			}
			break;

		case smTRANSCODE_CHECKITEM:
		case smTRANSCODE_ERRORITEM:
		case smTRANSCODE_CLEARPOTION:
			//������ ���� Ȯ�� �ڵ� ����
			RecvCheckItemFromServer( (TRANS_ITEM_CODE *)pData->Buff );
			break;

		case smTRANSCODE_GETCLIENT_FUNCPOS:
			//Ŭ���̾�Ʈ �����ǰ� �䱸
			SendClientFuncPos();
			break;

		case smTRANSCODE_FUNCTION_MEM:
			//�˻� �޸� ����� ��ä�� ����
			RecvMemFuncData( (TRANS_FUNC_MEMORY *)pData->Buff );
			break;

		case smTRANSCODE_PARTY_POTION:
			//���� ȸ��
			SetPartyPosion( (smTRANS_COMMAND *)pData->Buff );
			break;

		case smTRANSCODE_HEALING:
		case smTRANSCODE_GRAND_HEALING:
		case smTRANSCODE_VAMPRIC_CUSPID:
		case smTRANSCODE_VAMPRIC_CUSPID_EX: // �庰 - ���Ǹ� Ŀ���� EX
		case smTRANSCODE_MANA_RECHARGING:
			//ġ�� �ޱ�
			SetPartyPosion( (smTRANS_COMMAND *)pData->Buff );
			break;

		case smTRANSCODE_SAVECLIENT:
			//Ŭ���̾�Ʈ ����Ÿ ���� �䱸 ����
			SaveGameData();
			break;

		case smTRANSCODE_DEAD_PLAYER:
			//ĳ���� ���̱�
			if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_DEAD );
				sinSetLife( 0 );
				ReformCharForm();							//ĳ���� ���� ����
				CharPlaySound( lpCurPlayer );
				DeadPlayerExp( lpCurPlayer );				//�׾ ����ġ ���� (PK���������� ����)
				sinCheckQuest85Die();						//����Ʈ ������ �׾��� ���
				SaveGameData();
				ResetEnergyGraph( 0 );						//������ �׷��� ýũ �ʱ�ȭ
			}
			break;

		case smTRANSCODE_COMMAND_USER:
			//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ�
			RecvCommandUser( (smTRANS_COMMAND_EX *)pData->Buff );
			break;

		case smTRANSCODE_NPROTECT:
			//nProtect ���
			switch ( ((smTRANS_COMMAND *)pData->Buff)->WParam ) {
				case 100:
					//nProtect ����
					Remove_nProtect();
					break;
			}
			break;

		case smTRANSCODE_GAMEGUARD_AUTH:
			//GameGuard ����ýũ
			npAuth(((smTRANS_COMMAND_DWORD *)pData->Buff)->WParam);
			break;

		case smTRANSCODE_OPEN_CLANMENU:
			//Ŭ�� �޴� ����
			void CLANmenu_meetNPC();	//netplay.cpp���� �����.
			CLANmenu_meetNPC();

			///////////////// �׽�Ʈ �Ӹ�Ŀ���� //////////////////
			if ( !lpCurPlayer->smCharInfo.dwEventTime_T && smConfig.DebugMode && VRKeyBuff[VK_CONTROL] ) {
				ChangeBigHeadMode( (rand()%2)+1 , 60*5 );
				StartSkill( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 0,0,0, SKILL_UP1 );
				SkillPlaySound( SKILL_SOUND_LEARN , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );			//��ų ȿ����
			}
			break;

		case smTRANSCODE_COMIC_SKIN:
			//�ڹ� ���� ��ȯ ( ��ū�� )
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			if ( !lpCurPlayer->smCharInfo.dwEventTime_T ) {
				ChangeBigHeadMode( lpTransCommand->WParam , lpTransCommand->LParam );
				StartSkill( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 0,0,0, SKILL_UP1 );
				SkillPlaySound( SKILL_SOUND_LEARN , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );			//��ų ȿ����

				// ����� - 2010 ������ �̺�Ʈ - �౸�� ���� ���
				if(!lpTransCommand->SParam) // ��ū�� ������ ����� ��쿡�� �������� �÷��ش�
					cInvenTory.IncreViewDamage ( 15 );		//������ �������� �÷��ش�
			}
			break;

		case smTRANSCODE_CLAN_SERVICE:
			//Ŭ�� ���� ����
			RecvClanJoinService( (smTRANS_CHAR_COMMAND2 *)pData->Buff );
			break;


		case smTRANSCODE_VIRTURAL_POTION:
			//����,���,�ٷ� ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			if ( !lpTransVirtualPotion ) {
				lpTransVirtualPotion = new smTRANS_COMMAND;
				if ( lpTransVirtualPotion ) {
					memcpy( lpTransVirtualPotion , lpTransCommand , sizeof(smTRANS_COMMAND) ); 
				}
			}
			break;

		case smTRANSCODE_PROCESS_SKILL:
		case smTRANSCODE_PROCESS_SKILL2:
			//��ųó�� ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			RecvProcessSkill( lpTransCommand );
			break;

		case smTRANSCODE_PROCESS_CLAN_SKILL:
			//Ŭ�� ��ų ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			chaSiege.SetClanSkill(lpTransCommand->WParam);

			//���������� ������ ����
			lpsmSock = GetAreaServerSock();
			if ( lpsmSock ) lpsmSock->Send2( (char *)lpTransCommand , lpTransCommand->size , TRUE );
			break;

		case smTRANSCODE_CANCEL_SKILL:
			//��ų ���
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			switch( lpTransCommand->WParam ) {
				case SKILL_PLAY_METAL_GOLEM:
					cnt = SKILL_METAL_GOLEM;
					break;
				case SKILL_PLAY_RECALL_WOLVERIN:
					cnt = SKILL_RECALL_WOLVERIN;
					break;
				case SKILL_PLAY_FIRE_ELEMENTAL:
					cnt = SKILL_FIRE_ELEMENTAL;
					break;
			}
			cSkill.CancelContinueSkill( cnt );
			break;

		case smTRANSCODE_PARTY_SKILL:
			//��Ƽ ��ų ��Ŷ ����
			 RecvPartySkillFromServer( (TRANS_PARTY_SKILL *)pData->Buff );
			 break;

		case smTRANSCODE_CLAN_UPDATE:
			//Ŭ�� ����Ÿ ������Ʈ
			Updatecldata();
			break;

		case smTRANSCODE_QUEST_MESSAGE:
			//����Ʈ ���� �޼���
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			if ( lpTransCommand->WParam==SIN_QUEST_CODE_CHANGEJOB4 ) {
				CancelAttack();

				lpChar = FindAutoPlayer(  lpTransCommand->LParam );
				if ( lpChar ) {
					OpenEachPlayer( lpChar );
					DispEachMode = TRUE;
				}

				sinBattleNpcResult( 1 );		//3�� ���� �����͸� �̱�
			}
			else if ( lpTransCommand->WParam==HAQUEST_CODE_FURYOFPHANTOM ) {
				//ǻ�� ��Ʋ �¸�
				chaQuest.RecvBattleNpcResult(1);	
			}
			else
				sinMorayionNpcChangeJob_Two();

			break;

 		case smTRANSCODE_ATTACK_RESULT:
			//���� �õ� ��� ���� ( ũ��Ƽ�� ���� ���� )
			if ( lpCurPlayer->AttackCritcal>=0 ) {	
				lpCurPlayer->AttackCritcal = ((smTRANS_COMMAND *)pData->Buff)->WParam;
			}
			break;

		case smTRANSCODE_DISP_DAMAGE:
			//���ݹ��� �� ��¿�
			LastAttackDamage = ((smTRANS_COMMAND *)pData->Buff)->WParam;
			break;

		case smTRANSCODE_DAMAGE_ENCODE_MEM:
			//������ ��ȣȭ ��� ����
			RecvDamagePacketModule((TRANS_FUNC_MEMORY *)pData->Buff );
			break;

		case smTRANSCODE_DAMAGE_ENCODE_MEM2:
			//������ ��ȣȭ ��� ����
			RecvDamagePacketModule2((TRANS_FUNC_MEMORY *)pData->Buff );
			break;

		case smTRANSCODE_PACKET_DYNFUNC:
			//��Ŷ ���� ��ȣȭ ��� ���� 
			RecvDynPacketModule((TRANS_FUNC_MEMORY *)pData->Buff );
			break;

		case smTRANSCODE_GETPLAYERINFO:
			//ĳ���� ���� ������Ʈ �䱸 ( ������Ʈ ��Ŷ�� �Ҿ� ���� �ɷ� ���� - ��߼� )
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			if ( lpTransCommand->LParam!=lpCurPlayer->smCharInfo.bUpdateInfo[0] ) {
				SendSetObjectSerial( lpCurPlayer->dwObjectSerial );
			}
			break;

		case smTRANSCODE_AREA_SERVER_MAP:
			//���� ������ ���� �Լ�
			RecvAreaServerMap( (TRANS_SEVER_MAP *)pData->Buff );
			break;

		case smTRANSCODE_ITEM_EXPRESS:
			//������ ��� ���� �Լ�
			lpTransPostItem = (TRANS_POST_ITEM *)pData->Buff;   
/*
			if ( lpTransPostItem->dwItemFlag ) {
				cInvenTory.SetInvenToItemInfo( (sITEMINFO *)(pData->Buff+sizeof(TRANS_POST_ITEM) ));
				ResetInvenItemCode();
			}
*/
			if ( lpTransPostItem->dwItemFlag ) {
				ComparePotion();			//���హ�� ��
				cInvenTory.SetInvenToItemInfo( (sITEMINFO *)(pData->Buff+sizeof(TRANS_POST_ITEM) ));
				ResetInvenItemCode();

				if ( (((sITEMINFO *)(pData->Buff+sizeof(TRANS_POST_ITEM) ))->CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
					//���� ���� ���� ����
					ResetPotion2();
				}
				SaveGameData();
			}

			if ( lpCurPlayer->smCharInfo.Weight[0]>lpCurPlayer->smCharInfo.Weight[1] ) {
				cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);    //�����ʰ� �޼���
				break;
			}

			if ( lpTransPostItem->dwItemCode ) { //���� �������� ������� ������ üũ���� �޼����ڽ�������

				// pluto �ؿ� ���� ������ ����
				if( sinChar->Weight[0] + lpTransPostItem->Weight > sinChar->Weight[1] )
				{
					cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);    //�����ʰ� �޼���
					break;
				}

				memset(&sMessageBox_RecvItem.RecvItem,0,sizeof(sITEM));
				sMessageBox_RecvItem.RecvItem.CODE = lpTransPostItem->dwItemCode;
				sMessageBox_RecvItem.Flag = 1;
				sMessageBox_RecvItem.szItem[0] = 0;
				cInvenTory.OpenFlag = 1;
				sMessageBox_RecvItem.Param[0] = lpTransPostItem->dwParam[0];

				//���� ��� 
				if(sMessageBox_RecvItem.RecvItem.CODE == (sinGG1|sin01)){
					cInvenTory.LoadMoneyExpImage(&sMessageBox_RecvItem.RecvItem);
					sMessageBox_RecvItem.RecvItem.SellPrice =  lpTransPostItem->dwItemJobCode;
					lstrcpy(sMessageBox_RecvItem.szDoc,lpTransPostItem->szDoc);
					break;

				}
				//����ġ�� ��� 
				if(sMessageBox_RecvItem.RecvItem.CODE == (sinGG1|sin02)){
					cInvenTory.LoadMoneyExpImage(&sMessageBox_RecvItem.RecvItem);
					sMessageBox_RecvItem.RecvItem.OldX =  lpTransPostItem->dwItemJobCode;
					lstrcpy(sMessageBox_RecvItem.szDoc,lpTransPostItem->szDoc);
					break;
				}
				//�����ϰ��
				if((sMessageBox_RecvItem.RecvItem.CODE & sinITEM_MASK1) == 	0x04000000){
					sMessageBox_RecvItem.RecvItem.PotionCount =  lpTransPostItem->dwItemJobCode;
					cInvenTory.CheckInvenEmpty(&sMessageBox_RecvItem.RecvItem); //�̹����ε�
					lstrcpy(sMessageBox_RecvItem.szItem,lpTransPostItem->szItemName);
					lstrcpy(sMessageBox_RecvItem.szDoc,lpTransPostItem->szDoc);
					break;

				}

				//������
				cInvenTory.CheckInvenEmpty(&sMessageBox_RecvItem.RecvItem); //�̹����ε�
				lstrcpy(sMessageBox_RecvItem.szItem,lpTransPostItem->szItemName);
				lstrcpy(sMessageBox_RecvItem.szDoc,lpTransPostItem->szDoc);
			}
			break;

		case smTRANSCODE_SOD_RESULT:
			//SoD ��� ���
			cSinHelp.ShowSodRanking((TRANS_SOD_RESULT *)pData->Buff,1);
			break;

		case smTRANSCODE_SOD_RESULT2:
			//SoD ��� ��� ( NPC�� ���� ��� )
			cSinHelp.ShowSodRanking((TRANS_SOD_RESULT *)pData->Buff,0);
			break;

		case smTRANSCODE_SOD_INFOMATION:
			//SoD ���� ���� ����
			RecvSodGameInfomation( pData->Buff );
			break;


		case smTRANSCODE_WING_NPC:
			//Wing ����Ʈ NPC ������
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			if ( lpTransCommand->LParam ) {
				//��¥ ��������Ʈ (�ڷ���Ʈ)
				switch( lpTransCommand->LParam ) {
				case 1:
					sinShowTeleportMap();
					break;
				case 2:
					haShowCastleTeleportMap();
					break;
				case 3:
					TeleportDungeon();	// pluto �߰��������� �ڷ���Ʈ
					break;
				}
			}

			switch( lpTransCommand->WParam ) 
			{
				case 0:
				case 1:
					cInvenTory.sinWingQuestNpc();		//�� ����Ʈ
					break;
				case 2:
					cInvenTory.SendPuzzleNpc();			//������߱�
					break;
				case 3:
					cInvenTory.CheckEventItem();		//�ٺ��ǻ�
					break;
				case 4:
					cInvenTory.CheckFoxEventItem();		//����ȣ�̺�Ʈ
					break;
				case 5:
					cInvenTory.CheckCristalEventItem();	//ũ����Ż ��� �̺�Ʈ
					break;
				case 8:
					cInvenTory.CheckWatermelonEventItem();	// ����� - ���� �̺�Ʈ(ȣ��)  //����� - ���ϴ��� �ҿ� �̺�Ʈ  
					break;
				case 9:
					cInvenTory.CheckValentineEventItem();	// �庰 - �߷�Ÿ�� �̺�Ʈ
					break;
				case 10:
					cInvenTory.CheckPristonAlphabetEventItem();	// ����� - ���ĺ� ���� �̺�Ʈ
					break;
				case 11:
					cInvenTory.CheckCandydaysEventItem();	// �庰 - ĵ������
				break;
				case 12:
					cInvenTory.CheckMagicalGreenEventItem();	// �庰 - �����ñ׸�
				break;

				case 13:
					cInvenTory.CheckTearOfKaraEventItem();	// �庰 - ī���� ����
				break;

				case 14:
					cInvenTory.CheckFindinvestigatorEventItem();	// �庰 - ������� ã�ƶ�(���� ������)
				break;
				case 15:
					cInvenTory.CheckFindinvestigatorKamiuEventItem();	// �庰 - ������� ã�ƶ�(ī����)
				break;
				case 16:
					cInvenTory.CheckFindinvestigatorEtoEventItem();	// �庰 - ������� ã�ƶ�(����)
				break;

			}
			break;

		case smTRANSCODE_OPEN_STARPOINT:
			//�� ����Ʈ NPC ������
			StarShopNPC();
			break;

		case smTRANSCODE_STARPOINT:
			//�� ����Ʈ ��
			sinStarPoint( ((smTRANS_COMMAND *)pData->Buff)->WParam );
			//���� WParam �� ���� �����ݾ� ǥ�� 
			break;

		case smTRANSCODE_CLANMONEY:
			//Ŭ���Ӵ� ����Ÿ
			sinRecvClanMoney( ((smTRANS_COMMAND *)pData->Buff)->WParam , ((smTRANS_COMMAND *)pData->Buff)->LParam );
			break;

		case smTRANSCODE_OPEN_GIVEMONEY:
			//�� ��� NPC ������
			cMessageBox.ShowMessage2(MESSAGE_GIVE_MONEY);
			break;

		case smTRANSCODE_CHANGE_EVENT:
			//�̺�Ʈ ���� ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			if ( sBiInfo ) {
				sBiInfo->EventPlay[0] = lpTransCommand->WParam&0xFFFF;
				sBiInfo->EventPlay[1] = (lpTransCommand->WParam>>16)&0xFFFF;
				sBiInfo->PCRNo = lpTransCommand->LParam;
				cInvenTory.SetItemToChar();					//���� ����
			}

			if ( lpTransCommand->EParam ) {
				GoNotice_chking=1;								//�ȳ��޼��� ����
			}
			break;

		case smTRANSCODE_CALL_MYSHOP:
			//���λ��� ������ ���� �䱸
			if ( lpCurPlayer->dwTradeMsgCode )
				Send_MyShopItemList( (smTRANS_COMMAND *)pData->Buff );
			break;

		case smTRANSCODE_MYSHOP_ITEM:
			//���λ��� ������ ��� ����
			Recv_MyShopItemList((TRANS_TRADEITEMS *)pData->Buff);
			break;

		case smTRANSCODE_MYSHOP_MESSAGE:
			//���λ��� �޼��� ����
			lpTransChatMessage = (TRANS_CHATMESSAGE *)pData->Buff;

			lpChar = FindAutoPlayer( lpTransChatMessage->dwObjectSerial );
			if ( lpChar && lpChar->dwTradeMsgCode ) {

/*
//�߱� ���λ��� Ư����� ������
#ifdef _LANGUAGE_CHINESE
				ConvertStringTHAI(lpTransChatMessage->szMessage,  strlen(lpTransChatMessage->szMessage));
#endif
*/
				wsprintf( lpChar->szTradeMessage , "%s: %s" , lpChar->smCharInfo.szName , lpTransChatMessage->szMessage );
			}
			break;

		case smTRANSCODE_MYSHOP_TRADE:
			//���� ���� ���� ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;

			cMyShop.RecvMyShopItem( lpTransCommand->SParam ,
				(sMYSHOP_ITEM_SERVER *)(((smTRANS_COMMAND_BUFF *)pData->Buff)->Buff) );

			break;

		case smTRANSCODE_SUCCESS_EVATION:
			//�߰�ȸ�� ���� ����
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			//��ºκ� �Ŀ� �߰�
			AddTimeEffect( smTRANSCODE_SUCCESS_EVATION , dwPlayTime+500 , lpTransCommand->WParam ,lpTransCommand->LParam,lpTransCommand->SParam );
			break;

		case smTRANSCODE_BLESSCASTLE_INFO:
			//����ĳ�� �������� ����
			RecvBlessCastInfo( pData->Buff );
			break;

		case smTRANSCODE_MEDIA_NPC:
			//���� ��� NPC
			lpTransChatMessage = (TRANS_CHATMESSAGE *)pData->Buff;			
			chaSiege.ShowCastlePlayMovie(lpTransChatMessage->szMessage,
				lpTransChatMessage->szMessage+lstrlen(lpTransChatMessage->szMessage)+1,
				0 );

			break;

		case smTRANSCODE_EXITINFO_URL:
			//����� �������� ����
			lpTransChatMessage = (TRANS_CHATMESSAGE *)pData->Buff;
			lstrcpy( szExitInfo_URL , lpTransChatMessage->szMessage );
			break;

		case smTRANSCODE_PUBLIC_POLLING:
			//�������� �ǽ� ��Ŷ
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			cSinHelp.ShowResearchMenu(lpTransCommand->WParam,lpTransCommand->LParam);
			break;

		case smTRANSCODE_NOTICE_BOX:
			//���� ����â ǥ��
			//�ʺ� ������ ���̵� ��
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			cHelp_Menu.haGuidHelpLevel_1(lpTransCommand->WParam);
			break;

		case smTRANSCODE_PCBANG_PET:
			//PC�� �� ����
			//cPCBANGPet.PetKind = TRUE;
			//cPCBANGPet.ShowPet();
			break;
		case smTRANSCODE_PHENIX_PET:	// pluto ��(�ؿ�) ����
			//cPCBANGPet.PetKind = TRUE;
			//cPCBANGPet.ShowPet();
			break;

		case smTRANSCODE_HELP_PET: // ����� - ���� ����� �� �߰�
			break;

#ifdef _xTrap_GUARD
		case smTRANSCODE_XTRAP_PACKET:	//XTrap ��Ŷ����
			XTrap_Recv( (smTRANS_COMMAND *)pData->Buff , smWsockDataServer );
			break;
#endif

		case smTRANSCODE_SMELTINGITEM:	// pluto ����
			SmeltingItem.sinRecvSmeltingItemResult( (sSMELTINGITEM_SERVER *)pData->Buff );
			ResetInvenItemCode();
			SaveGameData();
			break;
		case smTRANSCODE_MANUFACTURE:	// pluto ����
			ManufactureItem.RecvManufactureItemResult( (SManufactureItem_Server *)pData->Buff );
			ResetInvenItemCode();
			SaveGameData();
			break;
		case smTRANSCODE_MIXTURE_RESET_ITEM:
			// ������ - �ͽ��� ���� Ŭ���̾�Ʈ ó��
			cMixtureReset.sinRecvMixtureItemResetResult( (sMIXTURE_RESET_ITEM_SERVER *)pData->Buff );
			ResetInvenItemCode();
			SaveGameData();
			break;
		case smTRANSCODE_MANUFACTURE_WEIGHT_PRICE:	// pluto ���� ������ ����, ���� ���� ���� �÷���
            ManufactureItemResultInfo( (SManufacture_ResultItemInfo_Server *)pData->Buff );
			break;
		case smTRANSCODE_SEND_AGAME_PRIMEITEM_DATA2:		// pluto ���� ���ེ ���� ����Ʈ ���� ����
			//���� �������� ��� ����
			lpsmSock = GetAreaServerSock();		//�������� ������ ã��
			for( int i = 0 ; i < SINUPKEEPITEM_MAX ; i++ )
			{
				if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::MIGHT_OF_AWELL )	// pluto ����Ʈ ���� ����	����Ʈ ���� ����2
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_MightofAwellTime = 0;
						chaPremiumitem.m_MightofAwellWeight = 0;
					}
				}

				if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::MANA_REDUCE_P )
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_ManaReducePotiontime = 0;
						chaPremiumitem.m_ManaReducePotionValue = 0;
					}
				}
				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::MIGHT_OF_AWELL )	// pluto ����Ʈ ���� ����	����Ʈ ���� ����2
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_MightofAwellTime = 0;
						chaPremiumitem.m_MightofAwellWeight = 0;
					}
				}
				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::PHENIX_PET )	// pluto ��(�ؿ�)
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_PhenixPetTime = 0;
						cPCBANGPet.ClosePet();
					}
				}
			}
			if ( lpsmSock ) {
				lpsmSock->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );

				if ( lpsmSock!=smWsockServer )
					smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			}
			else
				smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			break;

			// ����� - ���� ����� ��
		case smTRANSCODE_SEND_AGAME_PRIMEITEM_DATA3:
			//���� �������� ��� ����
			lpsmSock = GetAreaServerSock();		//�������� ������ ã��
			for( int i = 0 ; i < SINUPKEEPITEM_MAX ; i++ )
			{
				if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_TERRY )	// ����� - ���� ����� �� �߰�
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						// �庰 - �׸� �Ӽ�
						chaPremiumitem.m_TerryTime = 0;

						chaPremiumitem.m_HelpPetTime = 0;
						sinChar->GravityScroolCheck[1] = 0;
						cHelpPet.ClosePet();
					}
				}
				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_NEPSYS )	// ����� - ���� ����� �� �߰�
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_HelpPetTime = 0;
						cHelpPet.ClosePet();
					}
				}
				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_IO )	// ����� - ���� ����� �� �߰�
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_HelpPetTime = 0;
						cHelpPet.ClosePet();
					}
				}
				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_MUTE )	// ����� - ���� ����� �� �߰�
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_HelpPetTime = 0;
						sinChar->GravityScroolCheck[1] = 0;
						cHelpPet.ClosePet();
					}
				}
				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::STAMINA_REDUCE_P ) // ����� - �ٷ� ���ེ ����
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_StaminaReducePotiontime = 0;
						chaPremiumitem.m_StaminaReducePotionValue = 0;
						chaPremiumitem.SetStaminaReducePotionTime(chaPremiumitem.m_StaminaReducePotiontime);
					}
				}

				else if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::VAMPIRIC_CUS_EX ) // �庰 - ���Ǹ� Ŀ���� EX
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_VampiricCuspidEXTime = 0;
					}
				}
			}
            
			for( int i = 0 ; i < SINUPKEEPITEM_MAX ; i++ )
			{
				if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::VAMPIRIC_CUS_EX )
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_VampiricCuspidEXTime = 0;
					}
				}
				
			}


			if ( lpsmSock ) {
				lpsmSock->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );

				if ( lpsmSock!=smWsockServer )
					smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			}
			else
				smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );

			break;


			case smTRANSCODE_SEND_AGAME_PRIMEITEM_DATA4:		// �庰 - ���Ǹ� Ŀ���� EX
			//���� �������� ��� ����
			lpsmSock = GetAreaServerSock();		//�������� ������ ã��
			for( int i = 0 ; i < SINUPKEEPITEM_MAX ; i++ )
			{
				if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::VAMPIRIC_CUS_EX )
				{
					if( chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0 )
					{
						chaPremiumitem.m_VampiricCuspidEXTime = 0;
					}
				}
				
			}
			if ( lpsmSock ) {
				lpsmSock->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );

				if ( lpsmSock!=smWsockServer )
					smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			}
			else
				smWsockServer->Send2( ((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff , ((smTRANS_COMMAND *)(((TRANS_SEND_DATASERVER *)pData->Buff)->szBuff))->size , TRUE );
			break;

		case smTRANSCODE_COUPLERING:		// pluto Ŀ�ø�
			{
				cInvenTory.DeleteCoupleRing();
			}
			break;

			//����� - ���� ������ �ʱ�ȭ ġƮ
		case smTRANSCODE_PREMIUMITEM_INIT:
			lpTransCommand = (smTRANS_COMMAND *)pData->Buff;
			for( int i = 0 ; i < SINUPKEEPITEM_MAX ; i++ )
			{
				if( chaPremiumitem.UpKeepItem[i].TGAImageNumber == lpTransCommand->WParam)
				{
					chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.UpKeepItem[i].IconTime[0];
					if(lpTransCommand->WParam == 8 || lpTransCommand->WParam == 9 || lpTransCommand->WParam == 10 || lpTransCommand->WParam == 11) // ����� - ����� �� �ʱ�ȭ
					{
						cHelpPet.ClosePet();
					}
					else if(lpTransCommand->WParam == 12) // ����� - �Ǵн� �� �ʱ�ȭ
					{
						cPCBANGPet.ClosePet();
					}
					else if(lpTransCommand->WParam == 6) // ����� - ���� ���ེ ���� �ʱ�ȭ
					{
						chaPremiumitem.m_ManaReducePotiontime = 0;
						chaPremiumitem.SetManaReducePotionTime(chaPremiumitem.m_ManaReducePotiontime);
					}
					else if(lpTransCommand->WParam == 7) // ����� - �ٷ� ���ེ ���� �ʱ�ȭ
					{
						chaPremiumitem.m_StaminaReducePotiontime = 0;
						chaPremiumitem.SetStaminaReducePotionTime(chaPremiumitem.m_StaminaReducePotiontime);
					}
				}	
			}
			break;

#ifdef _XTRAP_GUARD_4_CLIENT
		case smTRANSCODE_XTRAP_PACKET:	//XTrap ��Ŷ����
			XTrap_Recv( (smTRANS_COMMAND *)pData->Buff , smWsockDataServer );
			break;
#endif
		
//����� - XignCode
#ifdef _XIGNCODE_CLIENT
		case smTRANSCODE_XIGNCODE_PACKET:
			XIGNCODE_PACKET *pack = (XIGNCODE_PACKET *)pData->Buff;
			Xigncode_Client_Recv(smWsockDataServer, pack);
			break;
#endif
		}

		if ( lpCharInfo_MsgBack ) {

			if ( addExp ) {

				cnt = (lpCurPlayer->smCharInfo.Exp^CodeXorExp)-(lpCharInfo_MsgBack->Exp^CodeXorExp);
				if ( cnt>addExp ) {
					memcpy( &lpCurPlayer->smCharInfo ,lpCharInfo_MsgBack, sizeof(smCHAR_INFO) );
					SaveGameData();						//������ ����
					lpCurPlayer->smCharInfo.Exp ++;
				}
			}

			if ( addMoney ) {
				cnt = lpCurPlayer->smCharInfo.Money - lpCharInfo_MsgBack->Money;

				if ( cnt>=addMoney ) {
					if ( addMoney>=2000 )
						SaveGameData();						//������ ����
				}
				else {
					//����Ÿ�� Ʋ��
					lpCurPlayer->smCharInfo.Money = lpCharInfo_MsgBack->Money;		//������Ű�鼭 ����������
				}

				//�κ��丮 �˻�뵷 �߰�
				AddInvenMoney(addMoney );

			}

			delete lpCharInfo_MsgBack;
		}

	return TRUE;
}

int rsTRANS_SERVER::Init( smWINSOCK *lpsmsock )
{

	lpsmSock = lpsmsock;
	lpRecorder = 0;

	//ũ��ƼĮ ���� �ʱ�ȭ
	InitializeCriticalSection( &cServSection );

	RecvDataQuePush = 0;
	RecvDataQuePop = 0;

	ZeroMemory( RecvDataQue , RECV_DATA_QUE_COUNT*4 );

	return TRUE;
}


// ������ ���ڿ��� �߶󳽴�
extern int szSpaceSorting( char *lpString );

//ä�� ���ڸ� ������ ����
int rsTRANS_SERVER::SendChat( char *szMessage )
{
	int len;

	if ( !szMessage || !szMessage[0] ) return FALSE;

	if ( szMessage[0]=='/' && ( szMessage[1]==';' || szMessage[1]==':' ) ) {
		// ������ ���ڿ��� �߶󳽴�
		szSpaceSorting( szMessage );
	}

	if ( szMessage[0]=='`' && szMessage[1]=='/' ) {
		len = lstrlen( szMessage+1 ) + 9;
		lstrcpy( TransBuff+8 , szMessage+1 );
		((int *)TransBuff)[0] = len;
		((int *)TransBuff)[1] = smTRANSCODE_CHATMESSAGE;
		if ( smWsockDataServer ) smWsockDataServer->Send( TransBuff , len , TRUE );
		return TRUE;
	}

	if ( szMessage[0]=='~' && szMessage[1]=='/' ) {
		//�缭���� ���ÿ� �޼��� ���� '~'����
		len = lstrlen( szMessage+1 ) + 9;
		lstrcpy( TransBuff+8 , szMessage+1 );
		((int *)TransBuff)[0] = len;
		((int *)TransBuff)[1] = smTRANSCODE_CHATMESSAGE;
		if ( smWsockServer ) smWsockServer->Send( TransBuff , len , TRUE );
		if ( smWsockServer!=smWsockUserServer && smWsockUserServer ) smWsockUserServer->Send( TransBuff , len , TRUE );
		if ( smWsockServer!=smWsockExtendServer && smWsockExtendServer ) smWsockExtendServer->Send( TransBuff , len , TRUE );
		if ( AreaServerMode ) {
			if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->Send( TransBuff , len , TRUE );
			if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->Send( TransBuff , len , TRUE );
		}

	}
	else {

		if ( szMessage[0]=='@' ) {
			//���� ä��

			wsprintf( TransBuff+8 , "%s: %s" , PlayerName , szMessage+1 );
			len = lstrlen( TransBuff+8 ) + 9;
			((int *)TransBuff)[0] = len;
			((int *)TransBuff)[1] = smTRANSCODE_PARTY_CHATMSG;

			if ( smWsockServer ) smWsockServer->Send( TransBuff , len , TRUE );
			AddChatBuff( TransBuff+8 , 4 );
			return TRUE;
		}
		else {
			lstrcpy( TransBuff+8 , szMessage );
			len = lstrlen( szMessage ) + 9;

			((int *)TransBuff)[0] = len;
			((int *)TransBuff)[1] = smTRANSCODE_CHATMESSAGE;
		}

		if ( szMessage[0]=='/' ) {

			if( szMessage[1]=='c' && szMessage[2]=='o' && szMessage[3]=='u' && szMessage[4]=='p' &&
				szMessage[5]=='l' && szMessage[6]=='e' )
			{
				if ( smWsockUserServer && cInvenTory.SearchItemCode( sinSP1|sin15 ) ) smWsockUserServer->Send( TransBuff , len , TRUE );
				return TRUE;
			}

			if ( szMessage[1]=='/' ) {
				if ( smWsockUserServer ) smWsockUserServer->Send( TransBuff , len , TRUE );
			}
			else {
				if ( smWsockServer ) smWsockServer->Send( TransBuff , len , TRUE );
			}

			return TRUE;
		}

		if ( AreaServerMode ) {
			if ( lpCurPlayer->OnStageField>=0 && lpWSockServer_DispArea[lpCurPlayer->OnStageField] ) {
				lpWSockServer_DispArea[lpCurPlayer->OnStageField]->Send( TransBuff , len , TRUE );
			}
		}
		else
			if ( smWsockUserServer ) smWsockUserServer->Send( TransBuff , len , TRUE );

	}
	return TRUE;
}

//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
int rsTRANS_SERVER::SendPlayData( smCHAR *lpChar )
{
//	int cnt;
//	int	*lpTransBuff;
	TRANS_PLAYPOS *lpTransPlayPos;
	int	dist,x,y,z;

	TransPlayPosCount++;

	if ( (TransPlayPosCount&1)==0 ) {
		x = (TransPlayPosLast.rsPlayPos.x-lpChar->pX)>>FLOATNS;
		y = (TransPlayPosLast.rsPlayPos.y-lpChar->pY)>>FLOATNS;
		z = (TransPlayPosLast.rsPlayPos.z-lpChar->pZ)>>FLOATNS;
		dist = x*x+y*y+z*z;

		if ( sinGetLife()>0 && dist<(16*16*16) ) {
			//��ġ�� ���Ѱ� ���� ��� ( 1���� �ɷ��� ���� )
			return TRUE;
		}
	}

	lpTransPlayPos = (TRANS_PLAYPOS *)TransBuff;
	lpTransPlayPos->code = smTRANSCODE_POSITION;
	lpTransPlayPos->size = sizeof( TRANS_PLAYPOS );

	if ( lpCurPlayer->OnStageField>=0 )
		lpTransPlayPos->rsPlayPos.Area = StageField[lpCurPlayer->OnStageField]->FieldCode;
	else
		lpTransPlayPos->rsPlayPos.Area = -1;

	lpTransPlayPos->rsPlayPos.x = lpChar->pX;
	lpTransPlayPos->rsPlayPos.y = lpChar->pY;
	lpTransPlayPos->rsPlayPos.z = lpChar->pZ;

	if ( lpCurPlayer->MotionInfo && lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_DEAD )
		lpTransPlayPos->Hp[0] = 0;
	else
		lpTransPlayPos->Hp[0] = sinGetLife();

	lpTransPlayPos->Hp[1] = lpChar->smCharInfo.Life[1];

	if ( smWsockServer )
		smWsockServer->Send2( (char *)lpTransPlayPos , lpTransPlayPos->size , TRUE );

	if ( smWsockExtendServer && smWsockExtendServer!=smWsockServer )
		smWsockExtendServer->Send2( (char *)lpTransPlayPos , lpTransPlayPos->size , TRUE );


	int	MainArea , SubArea;

	if ( AreaServerMode ) {
		if ( lpCurPlayer->OnStageField>=0 ) {
			MainArea = (lpCurPlayer->OnStageField)&1;
			SubArea = (lpCurPlayer->OnStageField+1)&1;

			if ( lpWSockServer_DispArea[SubArea] && lpWSockServer_DispArea[MainArea]!=lpWSockServer_DispArea[SubArea] ) {
				lpWSockServer_DispArea[SubArea]->Send2( (char *)lpTransPlayPos , lpTransPlayPos->size , TRUE );
			}
		}
	}


	return TRUE;
}


//���� ��� Ŭ���� ����
rsTRANS_SERVER	rsMainServer;







/*
//�÷��̾� ������ ���濡 ����
int SendPlayerInfo( smWINSOCK *lpsmSock )
{

	smTRNAS_PLAYERINFO	*lpTransPlayerInfo;

	lpTransPlayerInfo = (smTRNAS_PLAYERINFO *)TransBuff;

	lstrcpy( lpTransPlayerInfo->smCharInfo.szName , PlayerName );
	lstrcpy( lpTransPlayerInfo->smCharInfo.szModelName , lpCurPlayer->lpDinaPattern->szPatName );

	lpTransPlayerInfo->size = sizeof( smTRNAS_PLAYERINFO );
	lpTransPlayerInfo->code = smTRANSCODE_PLAYERINFO;
	lpsmSock->Send( (char *)lpTransPlayerInfo , lpTransPlayerInfo->size );

	return TRUE;
}
*/

#define SAVE_CHAT_COMMAND_MAX		16
#define SAVE_CHAT_COMMAND_MASK		15

char szSaveChatCommands[SAVE_CHAT_COMMAND_MAX][256];
int ChatCommandSaveCnt = 0;
int ChatSaveRecallCnt = 0;

//ä�� �ڸǵ� ����
int SaveChatCommand( char *szMessage )
{
	int len,cnt;
	int mcnt;
	char szBuff[256];

	len = lstrlen( szMessage );
	lstrcpy( szBuff , szMessage );

	for(cnt=0;cnt<len;cnt++) {
		if ( szBuff[cnt]==' ' ) {
			szBuff[cnt]=0;
			break;
		}
	}
	lstrcat( szBuff," " );

	//���Ϲ��� Ȯ��
	for( cnt=0;cnt<ChatCommandSaveCnt;cnt++) {
		if ( cnt>=SAVE_CHAT_COMMAND_MAX ) break;
		if ( lstrcmp( szSaveChatCommands[cnt] , szBuff )==0 ) 
			return TRUE;
	}

	mcnt = ChatCommandSaveCnt & SAVE_CHAT_COMMAND_MASK;
	lstrcpy( szSaveChatCommands[mcnt] , szBuff );

	ChatCommandSaveCnt++;
	ChatSaveRecallCnt = ChatCommandSaveCnt;

	return TRUE;
}

//ä�� ���� �ڸǵ� ���� 
int RecallSavedChatCommand( HWND hChatWnd , int Arrow )
{
	int mcnt;
	int len;

	if ( ChatCommandSaveCnt<=0 ) return FALSE;

	if ( Arrow ) {
		ChatSaveRecallCnt--;
		if ( ChatSaveRecallCnt<0 ) {
			ChatSaveRecallCnt=ChatCommandSaveCnt-1;
		}
	}
	else {
		ChatSaveRecallCnt++;
		if ( ChatSaveRecallCnt>=ChatCommandSaveCnt ) {
			ChatSaveRecallCnt=ChatCommandSaveCnt-1;
			SetWindowText( hChatWnd , "" );
			return TRUE;
		}
	}

	mcnt = ChatSaveRecallCnt & SAVE_CHAT_COMMAND_MASK;
	SetWindowText( hChatWnd , szSaveChatCommands[mcnt] );

	len = lstrlen(szSaveChatCommands[mcnt]);
	SendMessage(hChatWnd,EM_SETSEL, len , len );

	return TRUE;
}

// ���ڿ� �� ª�� ������ ���̸�ŭ ���Ͽ� �� ���� ������ ��ȯ
int rsCompString( char *src1 , char *src2 )
{
	int cnt;
	int len1,len2;
	int len;

	len1 = lstrlen( src1 );
	len2 = lstrlen( src2 );

	if ( len1<len2 ) len = len1;
	else len = len2;

	if ( !len ) return NULL;

	for( cnt=0; cnt<len1; cnt++ ) {
		if ( src1[cnt]==0 ) break;
		if ( src1[cnt]!=src2[cnt] ) return NULL;
	}

	return cnt;
}



char szLastChatMessage[256];
DWORD	dwLastChatTime;
DWORD	dwContiueChatCount = 0;
#include "CurseFilter.h"		//�弳����

char *rsGetWord(char *q , char *p);		//������ ����� ��

extern int WaveCameraMode;

//ä�� ���ڸ� ������ ����
int SendChatMessageToServer( char *szChatMessage )
{

	char szMessage[256];
	char *lpMsg=0;
	int	 cnt;
	int	 WhispFlag;
	char szBuff[256];
	smWINSOCK	*lpsmSock;

	if ( szChatMessage[0]=='/' && ( szChatMessage[1]==';' || szChatMessage[1]==':' ) ) WhispFlag=TRUE;
	else WhispFlag=FALSE;

	if ( dwLastChatTime && dwLastChatTime>dwPlayTime ) return FALSE;

	if ( dwLastChatTime && (dwPlayTime-dwLastChatTime)<2000 && WhispFlag==FALSE ) {
		dwContiueChatCount++;
		if ( dwContiueChatCount>10 ) {

			wsprintf( szMessage , mgContinueChat , lpCurPlayer->smCharInfo.szName );
			lstrcpy( lpCurPlayer->szChatMessage , szMessage );
			lpCurPlayer->dwChatMessageTimer = dwPlayTime + 1000*7;

			dwLastChatTime = lpCurPlayer->dwChatMessageTimer;
			return FALSE;
		}
	}
	else
		dwContiueChatCount = 0;

	lstrcpy( szMessage , szChatMessage );

	for(cnt=0;cnt<64;cnt++) {
		if ( !szMessage[cnt] ) break;
		if ( szMessage[cnt]==':' && szMessage[cnt+1]==' ' ) {
			lpMsg = szMessage+cnt+2;
			break;
		}
	}

	//��ȣ!
	if ( lpMsg && (lstrcmpi( mgYahoo , lpMsg )==0 || lstrcmpi( "yahoo" , lpMsg )==0 )) {

		dwYahooTime = dwPlayTime + 60*1000;		//1�а� ��ȿ

		if ( lpCurPlayer && lpCurPlayer->MotionInfo ) {
			if ( lpCurPlayer->MotionInfo->State<0x100 ) {
				if ( lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_YAHOO ) ) {
					//lpCurPlayer->HideWeapon = TRUE;
				}
			}
		}
		return TRUE;
	}


	if ( szMessage[0]!='/' && szMessage[0]!='~' && szMessage[0]!='`' && szMessage[0]!='@' ) {
		AddChatBuff( szMessage , 256 );

#ifdef _LANGUAGE_KOREAN
		cnt = 0;
		lpMsg = szMessage;
		while(1) {
			if ( !szMessage[cnt] ) break;
			if ( szMessage[cnt]==':' && szMessage[cnt+1]==' ' ) {
				lpMsg = &szMessage[cnt+2];
				break;
			}
			cnt++;
		}
		ConvertString(lpMsg,lstrlen(lpMsg)+16);
		////////////////////////////////////
			//szMessage[78] = 0;
			//AddChatBuff( szMessage , 0 );	//�׽�Ʈ��
		/////////////////////////////////////
#endif
#ifdef _LANGUAGE_CHINESE
		cnt = 0;
		lpMsg = szMessage;
		while(1)
		{
			if ( !szMessage[cnt] ) break;
			if ( szMessage[cnt]==':' && szMessage[cnt+1]==' ' )
			{
				lpMsg = &szMessage[cnt+2];
				break;
			}
			cnt++;
		}
		ConvertStringTHAI(lpMsg,lstrlen(lpMsg)+16);
		////////////////////////////////////
			//szMessage[78] = 0;
			//AddChatBuff( szMessage , 0 );	//�׽�Ʈ��
		/////////////////////////////////////
#endif
//�±�����
#ifdef _LANGUAGE_THAI
		cnt = 0;
		lpMsg = szMessage;
		while(1) {
			if ( !szMessage[cnt] ) break;
			if ( szMessage[cnt]==':' && szMessage[cnt+1]==' ' ) {
				lpMsg = &szMessage[cnt+2];
				break;
			}
			cnt++;
		}
		ConvertStringTHAI(lpMsg,lstrlen(lpMsg)+16);
		////////////////////////////////////
			//szMessage[78] = 0;
			//AddChatBuff( szMessage , 0 );	//�׽�Ʈ��
		/////////////////////////////////////
#endif
		lstrcpy( lpCurPlayer->szChatMessage , szMessage );
		lpCurPlayer->dwChatMessageTimer = dwPlayTime + 1000*7;
	}
	else {
		if ( szMessage[1]!='*' && szMessage[0]!='@' ) {
			//ä�� �ڸǵ� ����
			SaveChatCommand( szMessage );
		}

		char *lpBuff;
		int cnt,len;


		if ( szMessage[0]=='/' )
		{
#ifndef _LANGUAGE_PHILIPIN
			//��Ʈ�� ����
			len = rsCompString( "/latency" , szMessage );
			if ( !len )	len = rsCompString( "/LATENCY" , szMessage );

			if ( len ) {
				lpBuff =  szMessage + len;
				lpBuff = rsGetWord( szBuff , lpBuff );
				cnt = atoi(szBuff);
				if ( cnt<0 || cnt>500 || !szBuff[0] ) cnt=30;
				if ( cnt<10 ) cnt=0;
				smTransTurbRcvMode = cnt;

				wsprintf( szMessage , ">latency ( %d ms )" , cnt );
				AddChatBuff( szMessage , 0 );

				return TRUE;
			}
#endif

			//ī�޶� �ⷷ�Ÿ�
			len = rsCompString( "/camera_wave_off" , szMessage );
			if ( len ) {
				WaveCameraMode = FALSE;
				return TRUE;
			}

			//ī�޶� �ⷷ�Ÿ�
			len = rsCompString( "/camera_wave_on" , szMessage );
			if ( len ) {
				WaveCameraMode = TRUE;
				return TRUE;
			}


		}


#ifdef	_WINMODE_DEBUG
		int num;//,x,z;
		int r,g,b;

		if ( smConfig.DebugMode ) {

			//��� ��� ����
			len = rsCompString( "/dark" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				lpBuff = rsGetWord( szBuff , lpBuff );
				cnt = atoi(szBuff);
				if ( cnt>-256 && cnt<256 ) DarkLevel_Fix = cnt;
				else DarkLevel_Fix = 0;

				return TRUE;
			}

			//��� ���� ����
			len = rsCompString( "/color" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				lpBuff = rsGetWord( szBuff , lpBuff );
				cnt = atoi(szBuff);
				if ( cnt>-110 && cnt<110 ) r = cnt;
				else return TRUE;
				lpBuff = rsGetWord( szBuff , lpBuff );
				cnt = atoi(szBuff);
				if ( cnt>-110 && cnt<110 ) g = cnt;
				else return TRUE;
				lpBuff = rsGetWord( szBuff , lpBuff );
				cnt = atoi(szBuff);
				if ( cnt>-110 && cnt<110 ) b = cnt;
				else return TRUE;

				BackColor_mR = r;
				BackColor_mG = g;
				BackColor_mB = b;

				return TRUE;
			}

			//�ð� ����
			len = rsCompString( "/time_offset" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				lpBuff = rsGetWord( szBuff , lpBuff );
				cnt = atoi(szBuff);

				dwGameTimeOffset = cnt;
				return TRUE;
			}

			//����ġ ��
			len = rsCompString( "/expup" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				lpBuff = rsGetWord( szBuff , lpBuff );
				cnt = atoi(szBuff);

				AddExp( cnt );

				wsprintf(szBuff, ">( %d )" , cnt );
				AddChatBuff( szBuff );
				return TRUE;
			}

			//���� ��� �Է�
			len = rsCompString( "/talk" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {

						if ( chrEachMaster && chrEachMaster->dwObjectSerial ) {
							wsprintf( szBuff+8 , "/talk %d %s" , chrEachMaster->dwObjectSerial , lpBuff );
							len = lstrlen( szBuff+8 )+10;
							((int *)szBuff)[0] = len;
							((int *)szBuff)[1] = smTRANSCODE_CHATMESSAGE;
							lpsmSock = GetAreaServerSock();		//�������� ������ ã��
							if ( lpsmSock ) 
								lpsmSock->Send2( szBuff , len , TRUE );
						}
						return TRUE;
					}
					lpBuff++;
				}
			}

			//���� ��� ����
			len = rsCompString( "/set_absorb" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {

						if ( chrEachMaster && chrEachMaster->dwObjectSerial ) {
							wsprintf( szBuff+8 , "/set_absorb %d %s" , chrEachMaster->dwObjectSerial , lpBuff );
							len = lstrlen( szBuff+8 )+10;
							((int *)szBuff)[0] = len;
							((int *)szBuff)[1] = smTRANSCODE_CHATMESSAGE;
							lpsmSock = GetAreaServerSock();		//�������� ������ ã��
							if ( lpsmSock ) 
								lpsmSock->Send2( szBuff , len , TRUE );
						}
						return TRUE;
					}
					lpBuff++;
				}
			}

			//��ǥ��������
			len = rsCompString( "/player_location" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				lpBuff = rsGetWord( szBuff , lpBuff );
				lpCurPlayer->pX = atoi(szBuff)*fONE;
				lpBuff = rsGetWord( szBuff , lpBuff );
				lpCurPlayer->pY = atoi(szBuff)*fONE;
				lpBuff = rsGetWord( szBuff , lpBuff );
				lpCurPlayer->pZ = atoi(szBuff)*fONE;
				return TRUE;
			}


			//������ ���
			len = rsCompString( "/play_movie" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						return Play_ParkPlayer( lpBuff , 200, 200, 320 , 240 , 120 );
					}
					lpBuff++;
				}
			}

			//������ ����
			len = rsCompString( "/stop_movie" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						return Stop_ParkPlayer();
					}
					lpBuff++;
				}
			}

			//�޸� �ı�
			len = rsCompString( "/broke_mem" , szMessage );
			if ( len ) {
				char szBuff[256];
				wsprintf(szBuff, "%d" , Remove_nProtect );
				AddChatBuff( szBuff );
				((BYTE *)Remove_nProtect)[10] = 0xFF;
				return TRUE;
			}


			//�ٸ� ĳ���� �ε�
			len = rsCompString( "/load" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						return SendCopyOtherUser( lpBuff , 0);
					}
					lpBuff++;
				}
			}

			//�� ���� �ٲٱ�
			len = rsCompString( "/body" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						SetPlayerBodyPattern( lpBuff );		//�� ���� �ٲٱ�
						return TRUE;
					}
					lpBuff++;
				}
			}

			//�� ���� �ٲٱ�
			len = rsCompString( "/mymodel" , szMessage );
			if ( len && lpCurPlayer->smCharInfo.Level<=3 ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						SetPlayerPattern( lpBuff );		//�� ���� �ٲٱ�
						return TRUE;
					}
					lpBuff++;
				}
			}

			//�Ӹ� ���� �ٲٱ�
			len = rsCompString( "/myhairmodel" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						ChangeHairModel( atoi(lpBuff) );
						return TRUE;
					}
					lpBuff++;
				}
			}


			//������ ĳ���� �ε�
			len = rsCompString( "/deload" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						return SendCopyOtherUser( lpBuff , 1 );
					}
					lpBuff++;
				}
			}

			//��� ĳ���� �ε�
			len = rsCompString( "/baload" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						return SendCopyOtherUser( lpBuff , 2 );
					}
					lpBuff++;
				}
			}

			//ĳ���� �̸� ����
			len = rsCompString( "/ren" , szMessage );
			if ( len && AdminCharDisable ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						lstrcpy( lpCurPlayer->smCharInfo.szName , lpBuff );
						ReformCharForm();
						return TRUE;
					}
					lpBuff++;
				}
			}

			//�ٸ� ������ �̵�
			len = rsCompString( "/�ʵ庯��" , szMessage );
			if (!len)
				len = rsCompString( "/field" , szMessage );

			if ( len )
			{
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++)
				{
					if ( lpBuff[0]==0 ) 
						break;
					if ( lpBuff[0]!=' ' )
					{
						//return SendCopyOtherUser(lpBuff);
						num = atoi(lpBuff);

						//���� �ʵ�
						WarpField2( num );

/*
						//�ʵ� ����
						if ( WarpField( num , &x,&z ) ) {
							lpCurPlayer->SetPosi( x,0,z  ,0,0,0 );
							TraceCameraPosi.x = lpCurPlayer->pX;
							TraceCameraPosi.y = lpCurPlayer->pY;
							TraceCameraPosi.z = lpCurPlayer->pZ;
							TraceTargetPosi.x = lpCurPlayer->pX;
							TraceTargetPosi.y = lpCurPlayer->pY;
							TraceTargetPosi.z = lpCurPlayer->pZ;
	
							//�ٴ� Ȯ��
							lpCurPlayer->OnStageField = -1;
							if ( smGameStage[0] ) {
								cnt = smGameStage[0]->GetFloorHeight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ ,lpCurPlayer->Pattern->SizeHeight );	
								lpCurPlayer->OnStageField = 0;
							}
							if ( cnt==CLIP_OUT && smGameStage[1] ) {
								lpCurPlayer->OnStageField = 1;
							}
						}
*/
						return TRUE;
					}
					lpBuff++;
				}
			}

			//if ( AdminCharDisable ) {
				//���� ����
				len = rsCompString( "/save" , szMessage );
				if ( len ) {
					cnt = AdminCharDisable;
					AdminCharDisable = FALSE;
					SaveGameData();
					AdminCharDisable = cnt;
				}
			//}

			//�ٸ� ĳ���� �Ӹ�����
			len = rsCompString( "/face_path" , szMessage );
			if ( len ) {
				lpBuff =  szMessage + len;
				len = lstrlen( lpBuff );
				for(cnt=0;cnt<len;cnt++) {
					if ( lpBuff[0]==0 ) break;
					if ( lpBuff[0]!=' ' ) {
						//return SendCopyOtherUser( lpBuff , 0);
						//lstrcpy( lpCurPlayer->smCharInfo.szModelName2 , lpBuff );


						FILE *fp = fopen( lpBuff , "rb" );
						if ( !fp ) return FALSE;
						fclose (fp);

						//�� �����Ѱ� ����
						SetJobFace( lpBuff );
						return TRUE;
					}
					lpBuff++;
				}
			}

			if ( AreaServerMode ) {
				len = rsCompString( "/show" , szMessage );
				if ( !len ) len = rsCompString( "~/show" , szMessage );
				if ( len ) {
					ServerHideMode = FALSE;
				}
				len = rsCompString( "/hide" , szMessage );
				if ( !len ) len = rsCompString( "~/hide" , szMessage );
				if ( len ) {
					ServerHideMode = TRUE;
				}
			}

		}
#endif

	}

	if ( !smConfig.DebugMode && szMessage[0]!='/' ) {
		if ( (dwPlayTime-dwLastChatTime)<5000 && lstrcmpi( szLastChatMessage , szMessage )==0 ) {
			//5�� �ȿ� ���� �޼��� �������� ������ ���� ����
			return TRUE;
		}
	}

	lstrcpy( szLastChatMessage , szMessage );
	dwLastChatTime = dwPlayTime;

#ifdef _LANGUAGE_KOREAN
	if ( (szMessage[0]=='/' && ( szMessage[1]==';' || szMessage[1]==':' )) ||
		(szMessage[1]=='/' && ( szMessage[2]==';' || szMessage[2]==':' )) ) {
			//�Ӹ� ���͸�

			cnt = 0;
			lpMsg = szMessage;
			while(1) {
				if ( !szMessage[cnt] ) break;
				if ( szMessage[cnt]==' ' ) {
					lpMsg = &szMessage[cnt+1];
					break;
				}
				cnt++;
			}
			ConvertString(lpMsg,lstrlen(lpMsg)+16);
		}
#endif
#ifdef _LANGUAGE_CHINESE
	if ( (szMessage[0]=='/' && ( szMessage[1]==';' || szMessage[1]==':' )) ||
		(szMessage[1]=='/' && ( szMessage[2]==';' || szMessage[2]==':' )) ) {
			//�Ӹ� ���͸�

			cnt = 0;
			lpMsg = szMessage;
			while(1) {
				if ( !szMessage[cnt] ) break;
				if ( szMessage[cnt]==' ' ) {
					lpMsg = &szMessage[cnt+1];
					break;
				}
				cnt++;
			}
			ConvertStringTHAI(lpMsg,lstrlen(lpMsg)+16);
		}
#endif
//�±�����
#ifdef _LANGUAGE_THAI
	if ( (szMessage[0]=='/' && ( szMessage[1]==';' || szMessage[1]==':' )) ||
		(szMessage[1]=='/' && ( szMessage[2]==';' || szMessage[2]==':' )) ) {
			//�Ӹ� ���͸�

			cnt = 0;
			lpMsg = szMessage;
			while(1) {
				if ( !szMessage[cnt] ) break;
				if ( szMessage[cnt]==' ' ) {
					lpMsg = &szMessage[cnt+1];
					break;
				}
				cnt++;
			}
			ConvertStringTHAI(lpMsg,lstrlen(lpMsg)+16);
		}
#endif

	szMessage[78] = 0;
	return rsMainServer.SendChat( szMessage );
}


//ä�� ��ũ�� ����Ʈ
int ChatScrollPoint = 0;

//ä�� ���� ȭ�鿡 ǥ��
int DisplayChatMessage( HDC hdc , int x, int y , int MaxLine )
{
	int cnt;
	int chatCnt;
	char *lpString;
	int start,end;

	if ( ChatDispCnt==0 ) return FALSE;

	if ( ChatScrollPoint>CHATBUFF_MASK )
		ChatScrollPoint=CHATBUFF_MASK;
	if ( ChatScrollPoint<0 )
		ChatScrollPoint=0;

	start = ChatBuffCnt-MaxLine-ChatScrollPoint;
	end = ChatBuffCnt-ChatScrollPoint;

	for( cnt=start;cnt<end;cnt++) 
	{
		if ( cnt>=0 )
		{
			chatCnt = cnt & CHATBUFF_MASK;
			lpString = ChatBuff[chatCnt].szMessage;

			SetTextColor( hdc, RGB(0, 0, 0) );
			dsTextLineOut( hdc , x+1, y+1 , lpString , lstrlen( lpString ) );
			if ( ChatBuff[chatCnt].dwIP==dwMyIP ) 
				SetTextColor( hdc, RGB(255, 128, 96) );
			else {
				SetTextColor( hdc, RGB(255, 255, 128) );

				switch(ChatBuff[chatCnt].dwIP) {
				case 1:
					SetTextColor( hdc, RGB(255, 128, 96) );
					break;
				case 2:
					SetTextColor( hdc, RGB(96, 256, 255) );
					break;
				}
			}
			dsTextLineOut( hdc , x, y , lpString , lstrlen( lpString ) );
		}
		y+=16;
	}

	return TRUE;
}







//250 , 60 ,342 , 70

//��Ʈ�� ���� ī����
int GetNextLineCount( char *szString , int LineMax )
{
	int cnt;
	int len;
	char *lp;

	len = lstrlen( szString );
	if ( len<=LineMax ) return 0;

	lp = szString;

	cnt = 0;

	while(1) {
		//2����Ʈ �ڵ� Ȯ��
		if ( CheckCode_2Byte(lp)==2 ) {
			cnt+=2;
			lp+=2;
		}
		else {
			cnt++;
			lp++;
		}

		if ( cnt>(LineMax-2) ) break;
	}


	return cnt;
}

int SetIpColor( HDC hdc , DWORD dwIP )
{

	if ( dwIP==dwMyIP ) {
		SetTextColor( hdc, RGB(255, 128, 96) );
		return TRUE;
	}

	switch( dwIP ) {
	case 0:
		SetTextColor( hdc, RGB(255, 128, 96) );				//�����޼��� (�����۾�)
		break;
	case 1:
//		SetTextColor( hdc, RGB(255, 190, 150) );
		SetTextColor( hdc, RGB(150, 190, 255) );			//�Ӹ�
		break;
	case 2:
//		SetTextColor( hdc, RGB(255, 140, 120) );
		SetTextColor( hdc, RGB(150, 190, 255) );			//
		break;
	case 3:
		SetTextColor( hdc, RGB(230, 160, 255) );			//����
		break;
	case 4:
		SetTextColor( hdc, RGB(200, 255, 0 ) );				//����ä��
		break;
	case 5:
		SetTextColor( hdc, RGB(255, 255, 80 ) );			//Ŭ��ä��
		break;
	case 6:
		SetTextColor( hdc, RGB(200, 255, 255 ) );			//�ŷ�ä��
		break;

	case 9:
		SetTextColor( hdc, RGB(200, 128, 128 ) );			//���� ����â
		break;

	default:
//		SetTextColor( hdc, RGB(180, 160, 255) );
		SetTextColor( hdc, RGB(255, 255, 192) );
	}
/*
	if ( dwIP>=0x010101 ) {
		//Custom Color
		SetTextColor( hdc, dwIP );
	}
*/

	return TRUE;
}

static int	DisplayChatFilter = -1;
int	DispChatMsgHeight = 6;			//�ֱ� ǥ�õ� ä��â �� ����
int	DispChatMode = 0;

//ä��â ���͸�
int	SetChatMsgFilter( int mode )
{

	DisplayChatFilter = -1;
	ChatScrollPoint = 0;

	switch( mode ) {
		case 0:			//��ü
			DisplayChatFilter = -1;
			break;

		case 1:			//Ŭ��
			DisplayChatFilter = 5;
			break;

		case 2:			//����
			DisplayChatFilter = 4;
			break;

		case 3:			//�ŷ�
			DisplayChatFilter = 6;
			break;

		case 4:			//�Ӹ�
			DisplayChatFilter = 1;
			break;
	}

	DispChatMode = mode;

	return TRUE;
}

int ChatBuffSort()
{
	int cnt,cnt2;
	int mCnt,nCnt;
	int	Col;

	if ( ChatBuffCnt<=ChatBuffCompCnt ) return FALSE;

	for( cnt=ChatBuffCompCnt;cnt<ChatBuffCnt;cnt++ ) {
		mCnt = cnt&CHATBUFF_MASK;
		Col = 0;
		switch( ChatBuff[mCnt].dwIP ) {
		case 5:
			Col = 1;
			break;
		case 4:
			Col = 2;
			break;

		case 6:
			Col = 3;
			break;
		case 1:
			Col = 4;
			break;

		case 9:
			Col = 5;
			break;
		}
		nCnt = ChatBuffCnt2[Col]&CHATBUFF_MASK;
		memcpy( &ChatBuff2[Col][nCnt] , &ChatBuff[mCnt] , sizeof(CHATBUFF) );
		ChatBuffCnt2[Col]++;

		if ( Col>0 && Col<5 ) {
			nCnt = ChatBuffCnt2[0]&CHATBUFF_MASK;
			memcpy( &ChatBuff2[0][nCnt] , &ChatBuff[mCnt] , sizeof(CHATBUFF) );
			ChatBuffCnt2[0]++;
		}

		if ( Col==0 && ChatBuff[mCnt].dwIP<10 ) {
			for(cnt2=1;cnt2<5;cnt2++) {
				nCnt = ChatBuffCnt2[cnt2]&CHATBUFF_MASK;
				memcpy( &ChatBuff2[cnt2][nCnt] , &ChatBuff[mCnt] , sizeof(CHATBUFF) );
				ChatBuffCnt2[cnt2]++;
			}
		}
	}

	ChatBuffCompCnt = ChatBuffCnt;

	return TRUE;
}
//ä�� ���� ȭ�鿡 ǥ�� ( ���� )
int DisplayChatMessage3( HDC hdc , int x, int y , int StrMax , int LineMax , int Mode )
{
	int result;
	int ModeBackup = DispChatMode;
	int	ScrollBackup = 	ChatScrollPoint;

	DispChatMode = Mode;
	if ( LineMax<=6 )
		ChatScrollPoint = 0;

	result = DisplayChatMessage2( hdc , x, y , StrMax , LineMax );

	DispChatMode = ModeBackup;
	ChatScrollPoint = ScrollBackup;
	return result;
}

//ä�� ���� ȭ�鿡 ǥ��
int DisplayChatMessage2( HDC hdc , int x, int y , int StrMax , int LineMax )
{
	int cnt;
	int chatCnt;
	char *lpString;
	int LineCnt;
	int CutPos;
	int len;
	int start,end;
	int fl_Count;

	DWORD shColor = RGB( 74,74,74 );


	//if ( ChatDispCnt==0 ) return FALSE;
	ChatBuffSort();

	LineCnt = 0;

	if ( ChatScrollPoint>(CHATBUFF_MAX-32) )
		ChatScrollPoint=(CHATBUFF_MAX-32);
	if ( ChatScrollPoint<0 )
		ChatScrollPoint=0;

	fl_Count = DispChatMode;
	if ( fl_Count<0 ) fl_Count = 0;

	start = ChatBuffCnt2[fl_Count]-1-ChatScrollPoint;
	end = ChatBuffCnt2[fl_Count]-32-ChatScrollPoint;


//	for( cnt=ChatBuffCnt-1;cnt>=ChatBuffCnt-32;cnt--) {

	DispChatMsgHeight = 0;

	for( cnt=start;cnt>=end;cnt--) {
		if ( cnt>=0 ) {
			chatCnt = cnt & CHATBUFF_MASK;

//#ifdef _LANGUAGE_CHINESE //�߱� ä��â Ư����� ������
//			lpString = ChatBuff2[fl_Count][chatCnt].szMessage;
//			ConvertStringTHAI(lpString, lstrlen(lpString)+16);
//#else
			lpString = ChatBuff2[fl_Count][chatCnt].szMessage;
//#endif

			//if ( DisplayChatFilter<0 || DisplayChatFilter==ChatBuff[chatCnt].dwIP || ChatBuff[chatCnt].dwIP==0 || ChatBuff[chatCnt].dwIP==3 ) {


				DispChatMsgHeight++;			//���� ������

				len = lstrlen( lpString );
				if ( len>StrMax ) {
					//��Ʈ�� ���� ī����
					CutPos = GetNextLineCount( lpString , StrMax );

					SetTextColor( hdc, shColor );
					dsTextLineOut( hdc , x-1, y-1 , lpString+CutPos , len-CutPos );
					SetIpColor( hdc ,  ChatBuff2[fl_Count][chatCnt].dwIP );
					dsTextLineOut( hdc , x, y , lpString+CutPos , len-CutPos );

					y-=12;
					LineCnt++;
					if ( LineCnt>=LineMax ) break;

	#ifdef	_LANGUAGE_ENGLISH
					//���� �ٹٲ޽� '-' �߰�
					char szStrBuff[256];

					memcpy( szStrBuff , lpString , CutPos );
					szStrBuff[CutPos] = 0;


					if ( lpString[CutPos]>' ' && lpString[CutPos+1]>' ' ) {
						szStrBuff[CutPos] = '-';
						szStrBuff[CutPos+1] = 0;
					}

					len = lstrlen(szStrBuff);

					SetTextColor( hdc, shColor );
					dsTextLineOut( hdc , x-1, y-1 , szStrBuff , len );
					SetIpColor( hdc ,  ChatBuff2[fl_Count][chatCnt].dwIP );
					dsTextLineOut( hdc , x, y , szStrBuff , len );
#else
#ifdef	_LANGUAGE_ARGENTINA
					// �Ƹ���Ƽ�� �ٹٲ� kyle
					char szStrBuff[256];

					memcpy( szStrBuff , lpString , CutPos );
					szStrBuff[CutPos] = 0;


					if ( lpString[CutPos]>' ' && lpString[CutPos+1]>' ' ) {
						szStrBuff[CutPos] = '-';
						szStrBuff[CutPos+1] = 0;
					}

					len = lstrlen(szStrBuff);

					SetTextColor( hdc, shColor );
					dsTextLineOut( hdc , x-1, y-1 , szStrBuff , len );
					SetIpColor( hdc ,  ChatBuff2[fl_Count][chatCnt].dwIP );
					dsTextLineOut( hdc , x, y , szStrBuff , len );
#else
					SetTextColor( hdc, shColor );
					dsTextLineOut( hdc , x-1, y-1 , lpString , CutPos );
					SetIpColor( hdc ,  ChatBuff2[fl_Count][chatCnt].dwIP );
#ifdef _LANGUAGE_BRAZIL
					char semi[MAX_PATH],presemi[MAX_PATH],backsemi[MAX_PATH];
					ZeroMemory(presemi,MAX_PATH);
					ZeroMemory(backsemi,MAX_PATH);
					bool bsemi=0;
					strcpy(semi,lpString);
					int k=0, prelen=0, lenth=0;
					lenth =strlen(semi);
					//���ڿ� ���ʴ�� �˻�
					for(int i=0; i<lenth;i++)
					{
						backsemi[k]=semi[i];
						k++;
						if((bsemi==0) && (semi[i]==':'))
						{
							strcpy(presemi,backsemi);
							ZeroMemory(backsemi,MAX_PATH);
							bsemi++;
							k=0;
						}
					}
					//:�־����� ������ �ٲ���
					if(bsemi)
					{
						SIZE size;
						//:�չ��ڿ�
						prelen=strlen(presemi);
						SetTextColor( hdc, RGB(100,255,50) );
						dsTextLineOut( hdc , x , y , presemi , prelen );
						GetTextExtentPoint(hdc, presemi, prelen, &size);
						//�ڹ��ڿ�
						int j=0;
						j=x+size.cx;
						SetIpColor( hdc ,  ChatBuff2[fl_Count][chatCnt].dwIP );
						dsTextLineOut( hdc , j , y , backsemi , CutPos-prelen );
					}
					else dsTextLineOut( hdc , x, y , lpString , CutPos );
#else
					dsTextLineOut( hdc , x, y , lpString , CutPos );
#endif
#endif
#endif

					y-=12;
					LineCnt++;
					if ( LineCnt>=LineMax ) break;
					
				}
				else {

					SetTextColor( hdc, shColor );
					dsTextLineOut( hdc , x-1, y-1 , lpString , len );
					SetIpColor( hdc ,  ChatBuff2[fl_Count][chatCnt].dwIP );
#ifdef _LANGUAGE_BRAZIL
					char semi[MAX_PATH],presemi[MAX_PATH],backsemi[MAX_PATH];
					ZeroMemory(presemi,MAX_PATH);
					ZeroMemory(backsemi,MAX_PATH);
					bool bsemi=0;
					strcpy(semi,lpString);
					int k=0, prelen=0, lenth=0;
					lenth =strlen(semi);
					//���ڿ� ���ʴ�� �˻�
					for(int i=0; i<lenth;i++)
					{
						backsemi[k]=semi[i];
						k++;
						if((bsemi==0) && (semi[i]==':'))
						{
							strcpy(presemi,backsemi);
							ZeroMemory(backsemi,MAX_PATH);
							bsemi++;
							k=0;
						}
					}
					//:�־����� ������ �ٲ���
					if(bsemi)
					{
						SIZE size;
						//:�չ��ڿ�
						prelen=strlen(presemi);
						SetTextColor( hdc, RGB(100,255,50) );
						dsTextLineOut( hdc , x , y , presemi , prelen );
						GetTextExtentPoint(hdc, presemi, prelen, &size);
						//�ڹ��ڿ�
						int j=0;
						j=x+size.cx;
						SetIpColor( hdc ,  ChatBuff2[fl_Count][chatCnt].dwIP );
						dsTextLineOut( hdc , j , y , backsemi , strlen(backsemi) );
					}
					else dsTextLineOut( hdc , x, y , lpString , len );
#else
					dsTextLineOut( hdc , x, y , lpString , len );
#endif

					y-=12;
					LineCnt++;
					if ( LineCnt>=LineMax ) break;
				}
			//}
		}
	}

	return TRUE;
}

//�� �÷��̾�� ����
int ConnectPlayer( smWINSOCK *lpsmSock )
{
	Utils_Log(LOG_DEBUG, "ConnectPlayer");
	int OthPlayerCnt;

	if ( ServerMode )
		return Serv_ConnectPlayer( lpsmSock );


	//ũ��ƼĮ ���� ����
	EnterCriticalSection( &cSection );

	OthPlayerCnt = FindNewOtherPlayer();
	if ( OthPlayerCnt>=0 )
	{
		lpsmSock->ExtData1 = (void *)&chrOtherPlayer[OthPlayerCnt];
		chrOtherPlayer[OthPlayerCnt].Init();
		chrOtherPlayer[OthPlayerCnt].TransSendWait = 1;
		chrOtherPlayer[OthPlayerCnt].TransSock = lpsmSock;
		chrOtherPlayer[OthPlayerCnt].TransLastSendCnt = PlayCounter-70;
		chrOtherPlayer[OthPlayerCnt].Flag = 1;
		chrOtherPlayer[OthPlayerCnt].smCharInfo.szName[0] = 0;
		chrOtherPlayer[OthPlayerCnt].ActionPattern = 99;
		chrOtherPlayer[OthPlayerCnt].Pattern = 0;
		chrOtherPlayer[OthPlayerCnt].AutoPlayer = 0;

		lpCurPlayer->SendCharInfo( lpsmSock );
//		SendPlayerInfo( lpsmSock );

	}

	//ũ��ƼĮ ���� ����
	LeaveCriticalSection( &cSection );

	return TRUE;
}

//�÷��̾� ���� ������
int DisconnectPlayer( smWINSOCK *lpsmSock )
{
	//	int cnt;
	smCHAR *lpPlayer;
	DWORD	dwTime;

	dwTime = GetCurrentTime();

	if ( ServerMode )
		return Serv_DisconnectPlayer( lpsmSock );

	if ( smWsockServer && lpsmSock->sock==smWsockServer->sock )
	{
		AddChatBuff( "Connection lost to Server" );
		if ( smWsockServer==smWsockDataServer )
			smWsockDataServer = 0;
		if ( smWsockServer==smWsockUserServer )
			smWsockUserServer = 0;
		if ( smWsockServer==smWsockExtendServer )
			smWsockExtendServer = 0;

		smWsockServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 14");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 1;
		//�ŷ����̸� �ŷ� ��� ��Ŵ
		if ( cTrade.OpenFlag ) {
			SendRequestTrade( cTrade.TradeCharCode , 3 );			//�ŷ� ��� �䱸
			cTrade.CancelTradeItem();								//�ŷ�â �ݱ�
		}
		return TRUE;
	}

	if ( smWsockDataServer && lpsmSock->sock == smWsockDataServer->sock )
	{
		AddChatBuff( "Connection lost to Dataserver" );
		smWsockDataServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 15");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 2;

		//�ŷ����̸� �ŷ� ��� ��Ŵ
		if ( cTrade.OpenFlag ) {
			SendRequestTrade( cTrade.TradeCharCode , 3 );			//�ŷ� ��� �䱸
			cTrade.CancelTradeItem();								//�ŷ�â �ݱ�
		}
		return TRUE;
	}

	if ( smWsockUserServer && lpsmSock->sock==smWsockUserServer->sock ){
		AddChatBuff( "Connection lost to Server" );
		smWsockUserServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 16");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 3;
		//�ŷ����̸� �ŷ� ��� ��Ŵ
		if ( cTrade.OpenFlag ) {
			SendRequestTrade( cTrade.TradeCharCode , 3 );			//�ŷ� ��� �䱸
			cTrade.CancelTradeItem();					//�ŷ�â �ݱ�
		}
		return TRUE;
	}

	if ( smWsockExtendServer && lpsmSock->sock==smWsockExtendServer->sock ){
		AddChatBuff( "Connection lost to Server" );
		smWsockExtendServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 18");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 3;
		//�ŷ����̸� �ŷ� ��� ��Ŵ
		if ( cTrade.OpenFlag ) {
			SendRequestTrade( cTrade.TradeCharCode , 3 );		//�ŷ� ��� �䱸
			cTrade.CancelTradeItem();							//�ŷ�â �ݱ�
		}
		return TRUE;

	}

	char szBuff[256];

	//���� ���� ���� ������
	if ( AreaServerMode ) {
		if ( lpWSockServer_Area[0] && lpWSockServer_Area[0]->sock==lpsmSock->sock ) {
			if ( smConfig.DebugMode ) {
				wsprintf( szBuff , "Disconnect Area Server (%s)" , lpWSockServer_Area[0]->szIPAddr );
				AddChatBuff( szBuff , 0 );
			}
			if ( lpWSockServer_Area[0]==lpWSockServer_DispArea[0] ) lpWSockServer_DispArea[0]=0;
			if ( lpWSockServer_Area[0]==lpWSockServer_DispArea[1] ) lpWSockServer_DispArea[1]=0;

			lpWSockServer_Area[0] = 0;
			return TRUE;
		}
		if ( lpWSockServer_Area[1] && lpWSockServer_Area[1]->sock==lpsmSock->sock ) {
			if ( smConfig.DebugMode ) {
				wsprintf( szBuff , "Disconnect Area Server (%s)" , lpWSockServer_Area[1]->szIPAddr );
				AddChatBuff( szBuff , 0 );
			}

			if ( lpWSockServer_Area[1]==lpWSockServer_DispArea[0] ) lpWSockServer_DispArea[0]=0;
			if ( lpWSockServer_Area[1]==lpWSockServer_DispArea[1] ) lpWSockServer_DispArea[1]=0;

			lpWSockServer_Area[1] = 0;
			return TRUE;
		}
	}



	//ũ��ƼĮ ���� ����
	EnterCriticalSection( &cSection );

	lpPlayer = (smCHAR *)lpsmSock->ExtData1; 
	if ( lpPlayer ) {
		lpPlayer->Close();
	}

	//ũ��ƼĮ ���� ����
	LeaveCriticalSection( &cSection );

	return TRUE;
}



//���������� ������ ����Ÿ�� ���� ī����
int Ts_LastSendCounter;
DWORD	dwLastSendPosiTime = 0;		//������ ���� �ð� (��ġ)
DWORD	dwLastSendPlayTime = 0;		//������ ���� �ð� (��ǵ���Ÿ)

//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
int SendPlayDataToServer()
{
	/*
	int cnt;
	int	*lpTransBuff;
	rsPLAYPOS *lpTransPlayPos;

	lpTransBuff = (int *)TransBuff;
	lpTransPlayPos = (rsPLAYPOS *)&lpTransBuff[2];
	lpTransBuff[0] = sizeof( rsPLAYPOS ) + sizeof( int )*2;
	lpTransBuff[1] = smTRANSCODE_POSITION;

	lpTransPlayPos->Area = 0;
	lpTransPlayPos->x = lpCurPlayer->pX;
	lpTransPlayPos->y = lpCurPlayer->pY;
	lpTransPlayPos->z = lpCurPlayer->pZ;

	smWsockServer->Send( TransBuff , lpTransBuff[0] );

	return TRUE;
	*/

/*
	if ( smWsockServer!=smWsockUserServer && rsRecvServer ) {
		rsRecvServer = 0;
		return rsMainServer.SendPlayData( lpCurPlayer );
	}
*/
	if ( smWsockServer!=smWsockUserServer || AreaServerMode ) {
		return rsMainServer.SendPlayData( lpCurPlayer );
	}

	return TRUE;
}


//�ٸ� ������ ���� ��Ų��
int ConnectOtherPlayer( DWORD dwIP )
{
	smWINSOCK *lpsmSock;
	int OthPlayerCnt;

	OthPlayerCnt = FindNewOtherPlayer();
	if ( OthPlayerCnt>=0 ) {
		lpsmSock = smConnectSock2( ConvStrIP( dwIP ) , TCP_GAMEPORT );
		if ( lpsmSock ) {
			ConnectPlayer( lpsmSock );
			return TRUE;
		}
	}
	return FALSE;
}
//�ٸ� �÷��̾���� ���� ������
DWORD WINAPI ConnectPlayThreadProc( void *pInfo )
{

	ConnectOtherPlayer( (DWORD)pInfo );

	ExitThread( TRUE );
	return TRUE;
}
//�ٸ� �÷��̾���� ���� ���� ������
DWORD WINAPI DisconnectPlayThreadProc( void *pInfo )
{

	((smWINSOCK *)pInfo )->CloseSocket();

	ExitThread( TRUE );
	return TRUE;
}



//�ٸ� �÷��̾���� ���� �����带 ����
int CreateConnectThread( DWORD dwIP )
{
	DWORD dwMsgId;
	CreateThread( NULL , 0, ConnectPlayThreadProc , (void *)dwIP , 0, &dwMsgId );

	return TRUE;
}

//�ٸ� �÷��̾���� �������� ������ ����
int CreateDisconnectThread( smWINSOCK *lpsmSock )
{
	DWORD dwMsgId;
	CreateThread( NULL , 0, DisconnectPlayThreadProc , (void *)lpsmSock , 0, &dwMsgId );

	return TRUE;
}


//�Ÿ��� �� �ٸ� �������� ������ ���´�
int DisconnectFarPlayer()
{
	int mx,mz,dDist;
	int cnt;
	int timeover;

	for(cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		//�Ÿ��� Ȯ���Ͽ� �� ��쿡�� ������ ���������
		if ( chrOtherPlayer[cnt].Flag ) {
			
			if ((chrOtherPlayer[cnt].dwLastTransTime+DIS_TIME_OVER)<dwPlayTime )
				timeover = TRUE;
			else
				timeover = FALSE;

			//�Ÿ����
			mx = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX)>>FLOATNS;
			mz = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ)>>FLOATNS;
			dDist = mx*mx + mz*mz;

			if ( (chrOtherPlayer[cnt].DisplayFlag && dDist>DIST_TRANSLEVEL_DISCONNECT) || timeover ) {
				//���� ����/ ���� ����
				chrOtherPlayer[cnt].Flag = 0;

				if ( chrOtherPlayer[cnt].TransSock ) {
					chrOtherPlayer[cnt].TransSock->CloseSocket();
				}
				else {
					//ũ��ƼĮ ���� ����
					EnterCriticalSection( &cSection );
					chrOtherPlayer[cnt].Close();
					//ũ��ƼĮ ���� ����
					LeaveCriticalSection( &cSection );
				}
			}
		}
	}

	return TRUE;
}

DWORD	dwExtendServ_RecvTime = 0;
DWORD	dwUserServ_RecvTime = 0;
DWORD	dwMainServ_RecvTime = 0;
DWORD	dwDataServ_RecvTime = 0;

//����Ÿ �Լ�
int RecvPlayData( smTHREADSOCK *pData )
{

	DWORD *IPData;
	//smCHAR *lpPlayer;

	DWORD	dwTime;

	//ũ��ƼĮ ���� ����
	EnterCriticalSection( &cSection );


	IPData = (DWORD *)( pData->Buff );

	
/*
	if ( pData->smMySock==smWsockServer || 
		 pData->smMySock==smWsockUserServer || 
		 pData->smMySock==smWsockDataServer ||
		  pData->smMySock==smWsockExtendServer ) {
*/
		dwTime = GetCurrentTime();

		if ( pData->smMySock==smWsockDataServer ) 
		{
			dwRecvDataServerTime = dwTime;
			dwDataServ_RecvTime = dwTime;
		}
		if ( pData->smMySock==smWsockUserServer ) 
		{
			dwRecvUserServerTime = dwTime;
			dwUserServ_RecvTime = dwTime;
			Debug_RecvCount2++;
		}
		if ( pData->smMySock==smWsockExtendServer ) {
			dwRecvExtendServerTime = dwTime;
			dwExtendServ_RecvTime = dwTime;
			Debug_RecvCount3++;
		}
		if ( pData->smMySock==smWsockServer ) {
			Debug_RecvCount1++;
			dwRecvServerTime = dwTime;
			dwMainServ_RecvTime = dwTime;
		}

		//������ ����Ÿ ó��
		rsMainServer.RecvMessage( pData );
/*
	}
	else {
		//Ŭ���̾�Ʈ���� ����..

		lpPlayer = (smCHAR *)pData->smMySock->ExtData1;
		if ( lpPlayer ) lpPlayer->RecvPlayData( pData );
	}
*/
	//ũ��ƼĮ ���� ����
	LeaveCriticalSection( &cSection );

	return TRUE;
}

//�޼��� ť�� �޼����� ó��
int PlayRecvMessageQue()
{
	return rsMainServer.RecvMessageQue();
}


int LastSendCnt;
/*
int SendPlayData( smCHAR *player )
{
	int cnt;

	for(cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].Pattern 
			&& chrOtherPlayer[cnt].AutoPlayer==0 ) {
			chrOtherPlayer[cnt].SendPlayData( player );
		}
	}

	return TRUE;
}
*/
#define	NET_SENDING_WAIT_TIME		400
#define	NET_SENDING_WAIT_TIME2		1000

int NetStandCnt = 0;
extern int GameMode;

DWORD	NetScoopTime = 2000;

//�� �÷��� ����
int NetWorkPlay()
{
//	char *szIP;
	int cnt;
	DWORD	dwTime;
	DWORD	dwTime2;
	smWINSOCK	*lpsmSock;

	//����� - XignCode
	#ifdef _XIGNCODE_CLIENT
	ZCWAVE_Init();
	#endif

	if ( NetWorkInitFlag==0 ) 
	{
		if ( !smWsockServer || !smWsockDataServer || !smWsockUserServer || !smWsockExtendServer ) 
		{ 
			if ( ConnectServer_Main()==NULL )
				return FALSE;
			//smWsockServer = smConnectSock( szServIP , dwServPort );
		}

		//Sleep( 60*1000*3 );		//3�� ���

		NetWorkInitFlag = TRUE;
	
		rsMainServer.Init( smWsockServer );

		//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
		SendPlayDataToServer();

		//������ ���� ����
		if ( smConfig.DebugMode ) 
			SendAdminMode( TRUE );


//		if ( smWsockServer )
//			lpCurPlayer->SendCharInfo( smWsockServer );			//�÷��̾� ���� ����

		//����� ĳ���� ����Ÿ �ҷ����� �䱸
		if ( smWsockDataServer )
		{
			SendGetRecordData( lpCurPlayer->smCharInfo.szName );

			//���� IP ýũ ( �������� ������ IP�뺸 )
			if ( smWsockServer && smWsockDataServer!=smWsockServer ) 
			{
				SendCheckIP( 1 , smWsockServer );
			}
			if ( smWsockUserServer && smWsockDataServer!=smWsockUserServer ) 
			{
				SendCheckIP( 2 , smWsockUserServer );
			}
			if ( smWsockExtendServer && smWsockExtendServer!=smWsockServer && smWsockDataServer!=smWsockExtendServer )
			{
				SendCheckIP( 3 , smWsockExtendServer );
			}
		}

		ChatBuffCnt=0;
		ChatDispCnt=0;

		Ts_LastSendCounter = PlayCounter;
		TransThrowItem.code = 0;			//������ ������ �ʱ�ȭ

		RecordFailCount = 0;			//���� ���� ī����
		dwLastWareHouseChkSum = 0;		//â�� üũ�� �ڵ�

		TransAgingItemFlag = 0;			//����¡ �Ϸ� ������ ���� �÷�


		ZeroMemory( &TransRecvItem , sizeof(TRANS_ITEMINFO) );

		Trans_SplashCharList.code = 0;	//���÷��� ���� ���� �ʱ�ȭ

		ZeroMemory( &PK_FieldState , sizeof(PK_FIELD_STATE) );	//PK ���� �ʱ�ȭ
		PK_FieldState.FieldCode = -1;


		//���� �ڵ� ť�� ����
		ClearAttMonsterCode();

		Init_RecordDamage();			//���ݷ� ��� �ʱ�ȭ

		switch( smConfig.NetworkQuality ) 
		{
			case 0:
				NetScoopTime = 3000;
				break;
			case 1:
				NetScoopTime = 2000;
				break;
			case 2:
				NetScoopTime = 1000;
				break;
			case 3:
				NetScoopTime = 5000;
				smTransTurbRcvMode = 50;
				break;
		}

	}
	else
	{
		if ( cTrade.OpenFlag ) {
			//������ ��ȯ�� �Ϸ� Ȯ��
			if ( !TradeSendSucessFlag && sTrade.CheckFlag && sTradeRecv.CheckFlag ) {
				SendTradeCheckItem(cTrade.TradeCharCode);
				TradeSendSucessFlag = TRUE;
			}
		}
		if ( dwTradeMaskTime ) {
			//Ʈ���̵� ����ũ Ÿ�̸� �ʱ�ȭ
			if ( dwTradeMaskTime<dwPlayTime ) dwTradeMaskTime = 0;
		}

		dwTime = dwPlayTime -  dwLastSendPosiTime;
		dwTime2 = dwPlayTime - dwRecvServerTime;

		//�������� ����Ÿ ����
		if ( (PlayCounter-Ts_LastSendCounter)>PLAYSERVER_SENDCOUNTER ) {//&& dwTime>NET_SENDING_WAIT_TIME &&
//			dwTime2<NET_SENDING_WAIT_TIME ) {
			//������ �÷��̾��� ����Ÿ�� ��ġ�� ����
			SendPlayDataToServer();
			//�Ÿ��� �� �ٸ� �������� ������ ���´�
			DisconnectFarPlayer();

			//ī���� ����
			Ts_LastSendCounter = PlayCounter;
			dwLastSendPosiTime = dwPlayTime;
		}

//		if ( (xxcnt&0x3)==3 ) SendPlayData( lpCurPlayer );

		//�ٸ� ��������� ����Ÿ ��ȯ
		//SendPlayData( lpCurPlayer );

		if ( smWsockUserServer ) {// && rsRecvUserServer ) {
			//�÷��̾� ����Ÿ�� ������ ���� (40/70) �ʴ��� ���� 
			//if ( (lpCurPlayer->PlayBuffCnt&0x3F)==0 ) {

			dwTime = dwPlayTime -  dwLastSendPlayTime;
			dwTime2 = dwPlayTime - dwRecvUserServerTime;


			if ( (lpCurPlayer->PlayBuffCnt&0x3F)==0 && lpCurPlayer->dwObjectSerial && 
				dwTime>NET_SENDING_WAIT_TIME ) {//&& dwTime2<NET_SENDING_WAIT_TIME2 ) {

				if ( smConfig.DebugMode && AdminCharDisable ) {
					if ( smWsockServer==smWsockUserServer ) {
						rsMainServer.SendPlayData( lpCurPlayer );
					}
				}
				else {
					if ( AreaServerMode ) {
						if ( lpCurPlayer->OnStageField>=0 ) {
							lpsmSock = lpWSockServer_DispArea[lpCurPlayer->OnStageField];
						}

					}
					else {
						lpsmSock = smWsockUserServer;
					}

					if ( lpsmSock ) {
						lpCurPlayer->MakeTransPlayData( lpCurPlayer->srTransBuff , 0x40 );
						if ( ((smTRANS_PLAYDATA *)lpCurPlayer->srTransBuff)->PlayBuffCnt>1 ) {
							//for(int cnt=0;cnt<40;cnt++) 
							lpsmSock->Send( lpCurPlayer->srTransBuff , ((smTRANS_PLAYDATA *)lpCurPlayer->srTransBuff)->size , TRUE );
							NetStandCnt = 0;
						}
						else {
							//���ִ� ���� ��Ʈ�� ���� �ּ� ( �ѹ� ������ ��Ʈ�p ���� )
							//if ( NetStandCnt==0 )
							//for(int cnt=0;cnt<40;cnt++) 
							lpsmSock->Send( lpCurPlayer->srTransBuff , ((smTRANS_PLAYDATA *)lpCurPlayer->srTransBuff)->size , TRUE );
							NetStandCnt++;
						}
					}
				}

				Debug_SendCount++;

				rsRecvUserServer = FALSE;
				dwLastSendPlayTime = dwPlayTime;
			}
		}

		//���� ���� �ε� ��
		if ( dwLoadingTime ) {
			dwTime = GetCurrentTime();
			if ( dwTime>(dwLoadingTime+NET_LOADING_WAIT_LIMIT) ) {
				//�ð� �ʰ� ����
				Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 19");
				DisconnectFlag = GetCurrentTime();
				dwLoadingTime = 0;
			}
		}


		cnt = PlayCounter&0x1FF;
		if ( cnt==0 ) {
			//����� ýũ ( �޼����� ���� ���� ������ ��Ʈ�� �ٿ� )
			TransCheckNetwork();
		}
		else if (cnt==0x80 ) {
			//���μ��� �ð� �������� ������ �ۺ�
			SendPlayTimerMax();
		}



		if ( (PlayCounter&0x1F)==0 ) {
			dwTime = GetCurrentTime();
			if ( GameMode==2 && dwMainServ_RecvTime && smWsockServer && (dwTime-dwMainServ_RecvTime)>3000 ) {

				if ( smWsockServer->WaitReadMessage>=0 ) {
					smWsockServer->WaitReadMessage+=24;
					if ( smConfig.DebugMode )
						AddChatBuff( "Scoop Recv Route(Main)" );
				}
				else {
					if ( smConfig.DebugMode )
						AddChatBuff( "Error Server(Main)" );
				}
			}
			if ( GameMode==2 && smWsockUserServer && dwUserServ_RecvTime &&
				smWsockServer!=smWsockUserServer && (dwTime-dwUserServ_RecvTime)>3000 ) {
				//dwUserServ_RecvTime = dwTime;
				if ( smWsockUserServer->WaitReadMessage>=0 ) {
					smWsockUserServer->WaitReadMessage+=24;
					if ( smConfig.DebugMode )
						AddChatBuff( "Scoop Recv Route(User)" );
				}
				else {
					if ( smConfig.DebugMode )
						AddChatBuff( "Error Server(User)" );
				}
			}
			if ( GameMode==2 && smWsockExtendServer && dwExtendServ_RecvTime &&
				smWsockServer!=smWsockExtendServer && (dwTime-dwExtendServ_RecvTime)>3000 ) {
				//dwUserServ_RecvTime = dwTime;
				if ( smWsockExtendServer->WaitReadMessage>=0 ) {
					smWsockExtendServer->WaitReadMessage+=24;
					if ( smConfig.DebugMode )
						AddChatBuff( "Scoop Recv Route(Extend)" );
				}
				else {
					if ( smConfig.DebugMode )
						AddChatBuff( "Error Server(Extend)" );
				}
			}

//			if ( GameMode==2 && dwLoadingTime && 
			if ( GameMode==2 && ( (dwTime-dwDataServ_RecvTime)>7000 || dwLoadingTime ) && smWsockDataServer && smWsockDataServer!=smWsockServer ) 
			{
				smWsockDataServer->WaitReadMessage+=24;
				if ( smConfig.DebugMode && DisplayDebug )
						AddChatBuff( "Scoop Recv Route(Data)" );
			}

			if ( GameMode==2 && AreaServerMode ) 
			{
				if ( lpWSockServer_Area[0] && lpWSockServer_Area[0]->dwLastRecvTime ) 
				{
					if ( (dwTime-lpWSockServer_Area[0]->dwLastRecvTime)>2000 )
					{
						lpWSockServer_Area[0]->WaitReadMessage+=24;
						smCheckWaitMessage( lpWSockServer_Area[0] );
					}
				}
				if ( lpWSockServer_Area[1] && lpWSockServer_Area[1]->dwLastRecvTime ) 
				{
					if ( (dwTime-lpWSockServer_Area[1]->dwLastRecvTime)>2000 )
					{
						lpWSockServer_Area[1]->WaitReadMessage+=24;
						smCheckWaitMessage( lpWSockServer_Area[1] );
					}
				}

/*
	if ( AreaServerMode ) {
		if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );
		if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );
	}
*/

			}

				//dwLastRecvTime


		}

/*
	for(cnt=start;cnt<CONNECTMAX;cnt+=0x40 ) {
		if ( rsPlayInfo[cnt].lpsmSock && rsPlayInfo[cnt].dwObjectSerial &&//) {
			(dwPlayServTime-rsPlayInfo[cnt].dwLastRecvTime)<5000 ) {
			rsPlayInfo[cnt].lpsmSock->WaitReadMessage++;
		}
	}
*/	
	}
#ifndef _LANGUAGE_CHINESE	//������ ���
#ifndef _LANGUAGE_VEITNAM	//������ ���
#ifndef _LANGUAGE_PHILIPIN	//������ ���
#ifndef _LANGUAGE_ENGLISH	//������ ���
	HWND	hWnd;

	if ( (PlayCounter&0x1FF)==0 && !smConfig.DebugMode ) {
		hWnd = WinFocusCrack();
		//������ ��Ŀ���� ����
		if ( hWnd ) {
			//ũ�� ������ �Ű�
			SendCrackWindow( hWnd );
		}
	}
#endif
#endif
#endif
#endif


	//�ŷ��� Ȯ�λ�� ��õ� ( 0.8���� )
	if ( dwLastTransTradeKeyTime && (dwLastTransTradeKeyTime+800)<dwPlayTime ) 
	{
		if ( smWsockDataServer )
			smWsockDataServer->Send2( (char *)&LastTransTradeItemKey , LastTransTradeItemKey.size , TRUE ) ;

		dwLastTransTradeKeyTime = 0;
	}


	if ( ChatDispCnt>0 ) ChatDispCnt--;

	return TRUE;
}


//������ ������
int ThrowPutItem( sITEM *lpItem , int x, int y , int z )
{

	//���� ���¿� ���� �߻��� ������ ������ ���� ( 4�� �̻� ������ ���� ���� )
	if ( (dwRecvServerTime+4000)<dwPlayTime || QuitSave || TransThrowItem.code ) 
		return FALSE;

	if ( AdminCharDisable || WarningHack || !smWsockDataServer || !smWsockServer ) 
		return FALSE;

	//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
	if ( !SendSaveThrowItem2( &lpItem->sItemInfo ) )
		return FALSE;

	TransThrowItem.code =	smTRANSCODE_THROWITEM;
	TransThrowItem.size = sizeof(TRANS_ITEMINFO);
	TransThrowItem.x = x;
	TransThrowItem.y = y;
	TransThrowItem.z = z;

	memcpy( &TransThrowItem.Item , &lpItem->sItemInfo  , sizeof(sITEMINFO) );

	return TRUE;
}
//������ ������
int ThrowPutItem2( sITEMINFO *lpItem , int x, int y , int z )
{

	//���� ���¿� ���� �߻��� ������ ������ ���� ( 4�� �̻� ������ ���� ���� )
	if ( (dwRecvServerTime+4000)<dwPlayTime || QuitSave || TransThrowItem.code ) 
		return FALSE;

	if ( AdminCharDisable || WarningHack || !smWsockDataServer || !smWsockServer ) 
		return FALSE;

	//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
	if ( !SendSaveThrowItem2( lpItem ) )
		return FALSE;

	TransThrowItem.code =	smTRANSCODE_THROWITEM;
	TransThrowItem.size = sizeof(TRANS_ITEMINFO);
	TransThrowItem.x = x;
	TransThrowItem.y = y;
	TransThrowItem.z = z;

	memcpy( &TransThrowItem.Item , lpItem  , sizeof(sITEMINFO) );

/*
	if ( lpItem->CODE==(sinGG1|sin01) ) {
		SendSaveMoney();
	}
*/
	//return smWsockServer->Send( (char *)&TransItemInfo , TransItemInfo.size , TRUE );

	return TRUE;
}


//���� ������ ����
//TRANS_ITEMINFO	TransThrowItem;
//			ThrowItemToServer((TRANS_ITEMINFO *)pData->Buff );

int	ThrowPotionCount = 0;

//�ӽ� ����� ���� �������� Ȯ���Ͽ� ��ġ�ϸ� ������ ����
int ThrowItemToServer( smTRANS_COMMAND_EX *lpTransCommand )
{
	smTRANS_COMMAND	smTransCommand;
	smWINSOCK	*lpsmSock;
	DWORD	dwItemCodeMask;
	int		cnt;

	if ( TransThrowItem.code && 
		TransThrowItem.Item.CODE==(DWORD)lpTransCommand->WParam &&
		TransThrowItem.Item.ItemHeader.Head==(DWORD)lpTransCommand->LParam &&
		TransThrowItem.Item.ItemHeader.dwChkSum==(DWORD)lpTransCommand->SParam ) {

		if ( (TransThrowItem.Item.CODE&sinITEM_MASK1)!=(sinPM1&sinITEM_MASK1) )	{
			cInvenTory.DeleteInvenItemToServer(TransThrowItem.Item.CODE , TransThrowItem.Item.ItemHeader.Head , TransThrowItem.Item.ItemHeader.dwChkSum );

			//�κ��丮�� ������ �������� ã�� �˻��Ͽ� ã�´�
			if ( FindInvenItem( TransThrowItem.Item.CODE , 
				TransThrowItem.Item.ItemHeader.Head , 
				TransThrowItem.Item.ItemHeader.dwChkSum )!=0 ) {

					SendInvenItemError( 0 , TransThrowItem.Item.CODE , 
							TransThrowItem.Item.ItemHeader.Head , 
							TransThrowItem.Item.ItemHeader.dwChkSum );

					TransThrowItem.code = 0;
					return TRUE;
			}
		}
		else {
			if ( (dwPlayTime-dwLastRecordTime)>4000 && TransThrowItem.Item.PotionCount>=10 ) {
				//������ 10�� �̻� �׷����� ������� (���� / ���ູ�� 10���̻� �ȵǵ��� )
				SaveGameData();
				ThrowPotionCount = 0;
			}
			else {
				ThrowPotionCount += TransThrowItem.Item.PotionCount;
				if ( ThrowPotionCount>=15 ) {		//���� ���� ������ 15�� �ʰ��� �����ϰ� �÷� �ʱ�ȭ
					ThrowPotionCount = 0;
					SaveGameData();
				}
			}
		}

		if ( (TransThrowItem.Item.CODE&sinITEM_MASK2)==sinBC1 ) {

			if ( TransThrowItem.Item.PotionCount==100 )		//�ӽ� 100 ������ 200���� �ٲ�
				TransThrowItem.Item.PotionCount = 200;

			if 	( TransThrowItem.Item.CODE==(sinBC1|sin04) && TransThrowItem.Item.PotionCount==200) {
				//��Ƽ ȸ�� ( ����� ���� )
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.code = smTRANSCODE_PARTY_POTION;
				smTransCommand.WParam =	3000;	//�����
				smTransCommand.LParam =	3000;	//���
				smTransCommand.SParam =	3000;	//�ٷ�
				smTransCommand.EParam = lpCurPlayer->dwObjectSerial;
				
				if ( smWsockUserServer )
					smWsockUserServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

				SetPartyPosion( &smTransCommand );

				TransThrowItem.code = 0;
				return TRUE;
			}
		}

		if ( !TransThrowItem.Item.SpecialItemFlag[0] ) {		//( ���� ������ �������� ����� )
			TransThrowItem.dwSeCode[0] = lpTransCommand->WxParam;
			TransThrowItem.dwSeCode[1] = lpTransCommand->LxParam;
			TransThrowItem.dwSeCode[2] = lpTransCommand->SxParam;
			TransThrowItem.dwSeCode[3] = lpTransCommand->ExParam;

			dwItemCodeMask = TransThrowItem.Item.CODE&sinITEM_MASK2;

			// �庰 - �ҿｺ��
			if ( ((dwItemCodeMask==sinGP1 || dwItemCodeMask==sinCH1 || dwItemCodeMask==sinFO1 || dwItemCodeMask==sinGP2 ) && TransThrowItem.Item.PotionCount==100) ||
				(TransThrowItem.Item.CODE==(sinGG1|sin01) && TransThrowItem.Item.ItemKindCode==smTRANSCODE_COLLECTMONEY ) ||
				( dwItemCodeMask==sinBC1 && TransThrowItem.Item.PotionCount==200) ||
				( dwItemCodeMask==sinBI1 && TransThrowItem.Item.PotionCount==300) || 
				( dwItemCodeMask==sinBI2 && TransThrowItem.Item.PotionCount==300) )
			{
				lpsmSock = GetAreaServerSock();		//�������� ������ ã��
				if ( lpsmSock )
					lpsmSock->Send2( (char *)&TransThrowItem , TransThrowItem.size , TRUE );
				else
					smWsockServer->Send2( (char *)&TransThrowItem , TransThrowItem.size , TRUE );

				if ( dwItemCodeMask==sinFO1 ) {
					//�������� ����
					cnt = (TransThrowItem.Item.CODE&sinITEM_MASK3)>>8;
					cnt --;
					if ( cnt>=0 && cnt<12 ) {  // ����� : ���� ���� �߰��� ���� ���� 12�� ����
						lpCurPlayer->dwForceOrbCode = TransThrowItem.Item.CODE;		//�������� ���� �ڵ�
						lpCurPlayer->dwForceOrbTime = dwPlayTime + ForceOrbUseTime[cnt]*1000;
						AssaParticle_ShelltomWeapon( lpCurPlayer , ForceOrbUseTime[cnt]*70 , cnt );

						//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ� ������
						SendCommandUser( smCOMMNAD_USER_FORCEORB , 0 , 0 , 0 );
						StartSkill( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 0,0,0, SKILL_UP1 );
						SkillPlaySound( SKILL_SOUND_LEARN , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );			//��ų ȿ����
					}
					// ����� - ���� ���� �߰�
					else if ( cnt>=20 && cnt<32 ) {  
						lpCurPlayer->dwForceOrbCode = TransThrowItem.Item.CODE;		//�������� ���� �ڵ�
						lpCurPlayer->dwForceOrbTime = dwPlayTime + MagicForceOrbUseTime[cnt-20]*1000;
						// ����� - ���� ������ �Ϲ� ������ ���� ����Ʈ�� �����Ѵ�.
						AssaParticle_ShelltomWeapon( lpCurPlayer , MagicForceOrbUseTime[cnt-20]*70 , cnt ); //

						//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ� ������
						SendCommandUser( smCOMMNAD_USER_FORCEORB , 0 , 0 , 0 );
						StartSkill( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 0,0,0, SKILL_UP1 );
						SkillPlaySound( SKILL_SOUND_LEARN , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );			//��ų ȿ����
					}
					else if ( cnt>=34 && cnt<37 ) {  
						lpCurPlayer->dwForceOrbCode = TransThrowItem.Item.CODE;		//�������� ���� �ڵ�
						lpCurPlayer->dwForceOrbTime = dwPlayTime + BillingMagicForceOrbUseTime[cnt-34]*1000;
						// ����� - ���� ���� ������ �̶���, ���丣��, �̴ϱ׸��� ���� ����Ʈ�� �����Ѵ�.
						AssaParticle_ShelltomWeapon( lpCurPlayer , BillingMagicForceOrbUseTime[cnt-34]*70 , cnt ); //

						//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ� ������
						SendCommandUser( smCOMMNAD_USER_FORCEORB , 0 , 0 , 0 );
						StartSkill( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 0,0,0, SKILL_UP1 );
						SkillPlaySound( SKILL_SOUND_LEARN , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );			//��ų ȿ����
					}
				}

				//���� ������ ���� �ð� ǥ��
				if ( dwItemCodeMask==sinBI1 || dwItemCodeMask==sinBI2 ) {
					if ( TransThrowItem.Item.CODE==(sinBI1|sin12) ) {
						chaPremiumitem.SetThirdEyesTime( PRIME_ITEM_TIME_THIRDEYES );
					}
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin13) ) {
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_EXPUP );
					}
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin14) ) {
						chaPremiumitem.SetThirdEyesTime( PRIME_ITEM_TIME_THIRDEYESx7 );
					}
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin15) ) {
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_EXPUPx7 );
					}
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin23) ) {	//����
						cnt = (rand()%8192)*2;
						DWORD	dwTable = GetTableNum( cnt );
						AddTimeEffect( SKILL_EFFECT_FIREFLOWER , dwPlayTime , (dwTable>>24)&0xFF,0,0, lpCurPlayer );
						AddTimeEffect( SKILL_EFFECT_FIREFLOWER , dwPlayTime+500 , (dwTable>>16)&0xFF,0,0, lpCurPlayer );
						AddTimeEffect( SKILL_EFFECT_FIREFLOWER , dwPlayTime+1000 , (dwTable>>8)&0xFF,0,0, lpCurPlayer );
						AddTimeEffect( SKILL_EFFECT_FIREFLOWER , dwPlayTime+1500 , dwTable&0xFF,0,0, lpCurPlayer );
						lpCurPlayer->dwNextMotionCode = CHRMOTION_STATE_YAHOO;
						SendCommandUser( smCOMMNAD_USER_FIREFLOWER , cnt , 0 , 0 );
					}
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin24) ) {		//��Ʈ����û
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_EXPUP );
					}
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin25) ) {		//��Ʈ����û
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_EXPUPx7 );
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin26) )	// pluto ����Ʈ ���� ���� 7��
					{
						chaPremiumitem.SetMightOfAwellTime( PRIME_ITEM_TIME_EXPUPx7 );
						chaPremiumitem.SetMightOfAwellWeight( 300 );
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin27) )	// pluto ����Ʈ ���� ���� 30��
					{
						chaPremiumitem.SetMightOfAwellTime( PRIME_ITEM_TIME_PACKAGE4 );
						chaPremiumitem.SetMightOfAwellWeight( 300 );
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin28) )		// pluto ���� ���ེ ���� 1��
					{
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_EXPUP );
						chaPremiumitem.SetManaReducePotionValue( 30 ); // pluto ���� ���ེ ���� 30% ����
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin29) )		// pluto ���� ���ེ ���� 7��
					{
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_EXPUPx7 );
						chaPremiumitem.SetManaReducePotionValue( 30 ); // pluto ���� ���ེ ���� 30% ����
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin30) )	// pluto ����Ʈ ���� ����2 7��
					{
						chaPremiumitem.SetMightOfAwellTime( PRIME_ITEM_TIME_EXPUPx7 );
						chaPremiumitem.SetMightOfAwellWeight( 500 );
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin31) )	// pluto ����Ʈ ���� ����2 30��
					{
						chaPremiumitem.SetMightOfAwellTime( PRIME_ITEM_TIME_PACKAGE4 );
						chaPremiumitem.SetMightOfAwellWeight( 500 );
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin32) )	// pluto ��(�ؿ�)
					{
						chaPremiumitem.SetPhenixPetTime( PRIME_ITEM_TIME_EXPUP );
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin33) )	// pluto ��(�ؿ�)
					{
						chaPremiumitem.SetPhenixPetTime( PRIME_ITEM_TIME_THIRDEYESx7 );
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin34) )	// pluto ��(�ؿ�)
					{
						chaPremiumitem.SetPhenixPetTime( PRIME_ITEM_TIME_VAMPCUSPID );
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin43) ) {		// ����� - ����ġ���� ����(100% 1�� �߰�)
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_EXPUP );
					}
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin44) ) {		// ����� - ����ġ���� ����(100% 7�� �߰�)
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_EXPUPx7 );
					}
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin50) ) {		// ����� - ����ġ���� ����(100% 30�� �߰�)
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE4 ); // 30��
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin51) )	// ����� - �ȴн� ��(30��) �߰�
					{
						chaPremiumitem.SetPhenixPetTime( PRIME_ITEM_TIME_PACKAGE4 );
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin52) )	// ����� - ���� ����� �� �߰�
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE4 ); // �켱 �׽�Ʈ�� 1������ ����(60) 

						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Terry + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin53) )	// ����� - ���� ����� �� �߰�
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE4 ); 
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Nepsys + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin54) )	// ����� - ���� ����� �� �߰�
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE4 );  
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Io + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin55) )	// ����� - ���� ����� �� �߰�
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE4 ); 
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Mute + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}


					else if( TransThrowItem.Item.CODE == (sinBI1|sin69) )	// �庰 - ���� ����� �� �߰�
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE2 );
						
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Terry + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin70) )	// �庰 - ���� ����� �� �߰�
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE2 ); 
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Nepsys + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin71) )	// �庰 - ���� ����� �� �߰�
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE2 );  
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Io + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin72) )	// �庰 - ���� ����� �� �߰�
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE2 ); 
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Mute + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}

					// �庰 - ����ġ���� ����(100% 1�ð�)
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin80) ) {		
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE5 ); // 1�ð�
					}

					// �庰 - �Ǵн���(1�ð�)
					else if( TransThrowItem.Item.CODE == (sinBI1|sin77) )	// pluto ��(�ؿ�)
					{
						chaPremiumitem.SetPhenixPetTime( PRIME_ITEM_TIME_PACKAGE5 );
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}

					else if( TransThrowItem.Item.CODE == (sinBI1|sin65) )	// �庰 - ���� ����� �� �߰�(�׸�) 7��
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE3 );
						
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Terry + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin66) )	// �庰 - ���� ����� �� �߰�(�ܽý�)
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE3 ); 
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Nepsys + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin67) )	// �庰 - ���� ����� �� �߰�(�̿�)
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE3 );  
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Io + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin68) )	// �庰 - ���� ����� �� �߰�(��Ʈ)
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE3 ); 
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Mute + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}

					else if( TransThrowItem.Item.CODE == (sinBI1|sin73) )	// �庰 - ���� ����� �� �߰�(�׸� 1�ð�) 
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE5 );
						
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Terry + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin74) )	// �庰 - ���� ����� �� �߰�(�ܽý� 1�ð�)
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE5 ); 
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Nepsys + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin75) )	// �庰 - ���� ����� �� �߰�(�̿� 1�ð�)
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE5 );  
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Io + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin76) )	// �庰 - ���� ����� �� �߰�(��Ʈ 1�ð�)
					{
						chaPremiumitem.SetHelpPetTimeTime( PRIME_ITEM_TIME_PACKAGE5 ); 
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Mute + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}

					// �庰 - ��� ������(1�ð�)
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin78) ) {
						chaPremiumitem.SetThirdEyesTime( PRIME_ITEM_TIME_PACKAGE5 );
					}

					// �庰 - ����ġ��������(1�ð�)
					else if ( TransThrowItem.Item.CODE==(sinBI1|sin79) ) 
					{		
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE5 );
					}

					// �庰 - ���� ���ེ(1�ð�)
					else if( TransThrowItem.Item.CODE == (sinBI1|sin83) )		
					{
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE5 );
						chaPremiumitem.SetManaReducePotionValue( 30 ); // pluto ���� ���ེ ���� 30% ����
					}

					else if( TransThrowItem.Item.CODE == (sinBI1|sin91) )		 // ����� - �ٷ� ���ེ ����(1��)
					{
						chaPremiumitem.SetStaminaReducePotionTime( PRIME_ITEM_TIME_EXPUP ); // PRIME_ITEM_TIME_EXPUP
						chaPremiumitem.SetStaminaReducePotionValue( 30 );			// ����� - �ٷ� ���ེ ���� 30% ����
					}
					else if( TransThrowItem.Item.CODE == (sinBI1|sin92) )		 // ����� - �ٷ� ���ེ ����(7��)
					{
						chaPremiumitem.SetStaminaReducePotionTime( PRIME_ITEM_TIME_EXPUPx7 ); // PRIME_ITEM_TIME_EXPUPx7
						chaPremiumitem.SetStaminaReducePotionValue( 30 );			// ����� - �ٷ� ���ེ ���� 30% ����
					}

					
					if( TransThrowItem.Item.CODE == ((sinBI2|sin01))){
						int test = 1;
					}

					switch( TransThrowItem.Item.CODE ) 
					{
					case (sinBI2|sin01):
					case (sinBI2|sin05):
					case (sinBI2|sin09):
					case (sinBI2|sin23):	// pluto ���丮�� ��Ű�� 3�ð�
						chaPremiumitem.SetThirdEyesTime( PRIME_ITEM_TIME_PACKAGE1 );
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE1 );
						break;
					case (sinBI2|sin02):
					case (sinBI2|sin06):
					case (sinBI2|sin10):
					case (sinBI2|sin24):	// pluto ���丮�� ��Ű�� 1��
						chaPremiumitem.SetThirdEyesTime( PRIME_ITEM_TIME_PACKAGE2 );
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE2 );
						break;
					case (sinBI2|sin03):
					case (sinBI2|sin07):
					case (sinBI2|sin11):
					case (sinBI2|sin25):	// pluto ���丮�� ��Ű�� 7��
						chaPremiumitem.SetThirdEyesTime( PRIME_ITEM_TIME_PACKAGE3 );
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE3 );
						break;
					case (sinBI2|sin04):
					case (sinBI2|sin08):
					case (sinBI2|sin12):
						chaPremiumitem.SetThirdEyesTime( PRIME_ITEM_TIME_PACKAGE4 );
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE4 );
						break;
					case (sinBI2|sin26):			// pluto ����� ��Ű��2 3�ð�
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE1 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE1 );
						chaPremiumitem.SetManaReducePotionValue( 10 ); // pluto ���� ���ེ ���� 10% ����
						break;
					case (sinBI2|sin27):			// pluto ����� ��Ű��2 1��
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE2 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE2 );
						chaPremiumitem.SetManaReducePotionValue( 10 ); // pluto ���� ���ེ ���� 10% ����
						break;
					case (sinBI2|sin28):			// pluto ����� ��Ű��2 7��
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE3 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE3 );
						chaPremiumitem.SetManaReducePotionValue( 10 ); // pluto ���� ���ེ ���� 10% ����
						break;
					case (sinBI2|sin29):			// pluto �ǹ� ��Ű��2 3�ð�
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE1 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE1 );
						chaPremiumitem.SetManaReducePotionValue( 20 ); // pluto ���� ���ེ ���� 20% ����
						break;
					case (sinBI2|sin30):			// pluto �ǹ� ��Ű��2 1��
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE2 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE2 );
						chaPremiumitem.SetManaReducePotionValue( 20 ); // pluto ���� ���ེ ���� 20% ����
						break;
					case (sinBI2|sin31):			// pluto �ǹ� ��Ű��2 7��
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE3 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE3 );
						chaPremiumitem.SetManaReducePotionValue( 20 ); // pluto ���� ���ེ ���� 20% ����
						break;
					case (sinBI2|sin32):			// pluto ��� ��Ű��2 3�ð�
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE1 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE1 );
						chaPremiumitem.SetManaReducePotionValue( 30 ); // pluto ���� ���ེ ���� 30% ����
						break;
					case (sinBI2|sin33):			// pluto ��� ��Ű��2 1��
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE2 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE2 );
						chaPremiumitem.SetManaReducePotionValue( 30 ); // pluto ���� ���ེ ���� 30% ����
						break;
					case (sinBI2|sin34):			// pluto ��� ��Ű��2 7��
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE3 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE3 );
						chaPremiumitem.SetManaReducePotionValue( 30 ); // pluto ���� ���ེ ���� 30% ����
						break;
					case (sinBI2|sin35):			// pluto ���丮�� ��Ű��2 3�ð�
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE1 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE1 );
						chaPremiumitem.SetManaReducePotionValue( 40 ); // pluto ���� ���ེ ���� 40% ����
						break;
					case (sinBI2|sin36):			// pluto ���丮�� ��Ű��2 1��
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE2 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE2 );
						chaPremiumitem.SetManaReducePotionValue( 40 ); // pluto ���� ���ེ ���� 40% ����
						break;
					case (sinBI2|sin37):			// pluto ���丮�� ��Ű��2 7��
						chaPremiumitem.SetExpUpPotionTime( PRIME_ITEM_TIME_PACKAGE3 );
						chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_PACKAGE3 );
						chaPremiumitem.SetManaReducePotionValue( 40 ); // pluto ���� ���ེ ���� 40% ����
						break;
					}
				}

			}
			else {
				smWsockServer->Send2( (char *)&TransThrowItem , TransThrowItem.size , TRUE );
			}
		}

	}

	TransThrowItem.code = 0;
	return TRUE;
}


//���� ���� ���� �߰�
int SendAddStartPoint( int x, int z )
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_ADD_STARTPOSI;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.LParam = x;
	TransCommand.WParam = z;
	TransCommand.SParam = TRUE;

	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//�������� ������ ã��
	if ( lpsmSock ) {
		return lpsmSock->Send( (char *)&TransCommand , TransCommand.size , TRUE );
	}
	return NULL;
}

//���� �������� ����
int SendDeleteStartPoint( int x, int z )
{
	smTRANS_COMMAND	TransCommand;
	scITEM	*lpItem;
	smWINSOCK *lpsmSock;


	TransCommand.code =	smTRANSCODE_DEL_STARTPOSI;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.LParam = x;
	TransCommand.WParam = z;
	TransCommand.SParam = TRUE;

	//������ ������ ã��
	lpItem = FindScItem( x, z );
	if ( lpItem ) {
		lpsmSock = GetServerSock(lpItem->ServerCode);		//�������� ������ ã��
		lpItem->Close();
	}

	if ( lpsmSock ) {
		return lpsmSock->Send( (char *)&TransCommand , TransCommand.size , TRUE );
	}
	return NULL;
}



//NPC ĳ���� �߰�
int SendAdd_Npc( smCHAR *lpCharSample , int state )
{
	smTRNAS_PLAYERINFO	smTransPlayerInfo;

	smTransPlayerInfo.code =	smTRANSCODE_ADD_NPC;
	smTransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	memcpy( &smTransPlayerInfo.smCharInfo , &lpCharSample->smCharInfo , sizeof( smCHAR_INFO ) );
	smTransPlayerInfo.smCharInfo.State = 0;

	//NPC �������� �� ���̸�2�� �־ ����
	lstrcpy( smTransPlayerInfo.smCharInfo.szModelName2 , NpcSelectedName );

	smTransPlayerInfo.smCharInfo.Life[0] = 100;
	smTransPlayerInfo.smCharInfo.Mana[0] = 100;
	smTransPlayerInfo.smCharInfo.Life[1] = 100;

	smTransPlayerInfo.x = lpCharSample->pX;
	smTransPlayerInfo.y = lpCharSample->pY;
	smTransPlayerInfo.z = lpCharSample->pZ;

	smTransPlayerInfo.ax = lpCharSample->Angle.x;
	smTransPlayerInfo.ay = lpCharSample->Angle.y;
	smTransPlayerInfo.az = lpCharSample->Angle.z;

	smTransPlayerInfo.state = state;

	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//�������� ������ ã��
	if ( lpsmSock ) {
		return lpsmSock->Send( (char *)&smTransPlayerInfo , smTransPlayerInfo.size , TRUE );
	}

	return NULL;
}

//NPC ĳ���� ����
int SendDelete_Npc( smCHAR *lpCharSample )
{
	smTRNAS_PLAYERINFO	smTransPlayerInfo;
	smWINSOCK *lpsmSock;

	smTransPlayerInfo.code =	smTRANSCODE_DEL_NPC;
	smTransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	memcpy( &smTransPlayerInfo.smCharInfo , &lpCharSample->smCharInfo , sizeof( smCHAR_INFO ) );

	smTransPlayerInfo.x = lpCharSample->pX;
	smTransPlayerInfo.y = lpCharSample->pY;
	smTransPlayerInfo.z = lpCharSample->pZ;

	smTransPlayerInfo.ax = lpCharSample->Angle.x;
	smTransPlayerInfo.ay = lpCharSample->Angle.y;
	smTransPlayerInfo.az = lpCharSample->Angle.z;

	smTransPlayerInfo.dwObjectSerial = lpCharSample->dwObjectSerial;

	lpsmSock = GetServerSock(lpCharSample->ServerCode);		//�������� ������ ã��
	lpCharSample->Close();

	if ( lpsmSock ) {
		return lpsmSock->Send( (char *)&smTransPlayerInfo , smTransPlayerInfo.size , TRUE );
	}
	return NULL;
}

//����� ĳ���� ����Ÿ �ҷ����� �䱸
int SendGetRecordData( char *szName )
{
	smTRANS_CHAR_COMMAND	smTransCharCommand;

	if ( smWsockDataServer ) 
	{
		smTransCharCommand.code = smTRANSCODE_GETRECORDDATA;
		smTransCharCommand.size = sizeof( smTRANS_CHAR_COMMAND );

		smTransCharCommand.WParam = dwExeCheckSum;					//�������� üũ��
		smTransCharCommand.LParam = FALSE;
		smTransCharCommand.SParam = Client_Version;					//Ŭ���̾�Ʈ ����

		lstrcpy( smTransCharCommand.szName , szName );

		dwLoadingTime = GetCurrentTime();			//�ε� �õ� �ð� ���

		return smWsockDataServer->Send( (char *)&smTransCharCommand , smTransCharCommand.size , TRUE );
	}

	return FALSE;
}

char *szCmdOpenMonster[10] = {
	"ȩ���",
	"ȩ���",
	"ȩ���",
	"ŷȣ��",
	"Ÿ��ź",
	0,0,0,0,0
};


//���� ����
int SendOpenMonster( int State )
{
	smTRANS_CHAR_COMMAND	smTransCharCommand;
	char *szMonName;

	smTransCharCommand.code = smTRANSCODE_OPENMONSTER;
	smTransCharCommand.size = sizeof( smTRANS_CHAR_COMMAND );
	smTransCharCommand.WParam = State;
	smTransCharCommand.LParam = 0;
	smTransCharCommand.SParam = 0;

	if ( State>=0 ) {
		szMonName = smConfig.szCmdOpenMonster[State];
		if ( !szMonName[0] ) szMonName = szCmdOpenMonster[State];
	}

	if ( State>=0 && szMonName )
		lstrcpy( smTransCharCommand.szName , szMonName );
	else
		smTransCharCommand.szName[0];


	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//�������� ������ ã��
	if ( lpsmSock ) {
		return lpsmSock->Send( (char *)&smTransCharCommand , smTransCharCommand.size , TRUE );
	}

	return FALSE;

}

//�� �ڵ� ����Ÿ�� ����
int	CodeXorCharInfo()
{
	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;
	lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
	return TRUE;
}

//��ų�� ���� Ȯ��
int SaveCheckSkill()
{

	ReformSkillInfo();

	for(int j=1;j < SIN_MAX_USE_SKILL; j++){
		if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point){
			if ( cSkill.CheckSkillPointForm( &sinSkill.UseSkill[j] )==FALSE ) 
				return FALSE;
			if ( cSkill.CheckSkillMasteryForm( &sinSkill.UseSkill[j] )==FALSE )
				return FALSE;
		}
	}

	return TRUE;
}

//��ų��ȣ�� ���� �ʱ�ȭ
int ReformSkillInfo()
{
	for(int j=1;j < SIN_MAX_USE_SKILL; j++){
		cSkill.ReformSkillMasteryForm( &sinSkill.UseSkill[j] );
		cSkill.ReformSkillPointForm( &sinSkill.UseSkill[j] );
	}
	return TRUE;
}

//������ ����
int	SaveGameData()
{
	if ( smWsockDataServer && !rsRecorder.dwRecordTime && 
		lpCurPlayer->smCharInfo.JOB_CODE && !AdminCharDisable && !cTrade.OpenFlag && !cWareHouse.OpenFlag && !QuitSave ) 
	{
		//���హ�� ��
		//if ( ComparePotion()>0 ) return FALSE;		//���� ����

		if ( !WarningHack && CheckCharForm()!=FALSE && SaveCheckSkill()==TRUE ) 
		{
			CodeXorCharInfo();
			ReformCharForm();

			rsRecorder.MakeRecordData( &lpCurPlayer->smCharInfo , cInvenTory.InvenItem ,cInvenTory.InvenItemTemp , &MouseItem  ) ;
			rsRecorder.SendRecordDataToServer( smWsockDataServer ) ;
			rsRecorder.dwRecordTime = dwPlayTime;					//���� ���� �ð� Ȯ��

			if ( smConfig.DebugMode ) 
				AddChatBuff( "GameSave" );

			CodeXorCharInfo();
			ReformCharForm();
			CheckPlaySkill();		//��ų����
			return TRUE;
		}
	}
	return FALSE;
}

//���� ���� ���� ( FALSE ������ ( �����κ��� ����� ���� ���� ) )
int GetSaveResult()
{
	if ( rsRecorder.dwRecordTime ) return FALSE;

	return TRUE;
}


///////////////////// ������ ���� ////////////////////
	//smTRANSCODE_ID_GETUSERINFO:
	//ID�� ����� ĳ���� ����Ʈ �� ������ ����

	//smTRANSCODE_INSRECORDDATA:
	//ID�� ���ο� ĳ���� ����

	//smTRANSCODE_DELRECORDDATA:
	//ID�� ���ο� ĳ���� ����

	//smTRANSCODE_ISRECORDDATA:
	//ĳ���Ͱ� ���� �ϴ��� Ȯ��

//���� ���� ���� ��� �ڸ��
//int	TransUserCommand ( DWORD dwCode , char *szID , char *szName )

///////////////////////////////////////////////////////

///////////////////// �ޱ� ���� ////////////////////
	//smTRANSCODE_ID_SETUSERINFO:
	//ID�� ����� ĳ���� ����Ʈ �� ������ ���� 
	//(TRANS_USERCHAR_INFO ����ü�� ��� ���� )

	//smTRANSCODE_ISRECORDDATA:
	//ĳ���Ͱ� ���� �ϴ��� Ȯ�� 
	//( smTRANS_CHAR_COMMAND2 ->wParam �� ��� ���� )
///////////////////////////////////////////////////////

smTRANS_COMMAND	smMessageCommandBuff;

//���� ���� ���� ��� �ڸ�� ( ����Ÿ ������ )
int	TransUserCommand ( DWORD dwCode , char *szID , char *szName )
{
	smTRANS_CHAR_COMMAND2	TransCharCommand2;

	TransCharCommand2.code = dwCode;
	TransCharCommand2.size = sizeof(smTRANS_CHAR_COMMAND2);

#ifdef _LANGUAGE_KOREAN
	if ( dwCode==smTRANSCODE_ID_GETUSERINFO ) {
		int cnt,len;

		len = lstrlen(szName);
		for(cnt=0;cnt<len;cnt++) {
			if ( (szName[cnt]&0x80)!=0 ) {
				smMessageCommandBuff.code = smTRANSCODE_FAILCONNECT;
				smMessageCommandBuff.size = sizeof( smTRANS_COMMAND );
				smMessageCommandBuff.WParam = -19;			//Ư������ ����
				smMessageCommandBuff.LParam = 0;
				smMessageCommandBuff.EParam = 2;
				HoRecvMessage( smTRANSCODE_FAILCONNECT , &smMessageCommandBuff );
				return FALSE;
			}
		}
	}
#endif

	if ( szID )
		lstrcpy( TransCharCommand2.szId , szID );

	if ( szName )
		lstrcpy( TransCharCommand2.szName , szName );

	return smWsockDataServer->Send2( (char *)&TransCharCommand2 , TransCharCommand2.size , TRUE );
}

//ĳ���� ���� ���� ����
int	Send_GetCharInfo( DWORD	dwObjectSerial , int ServerCode )
{
	smTRANS_COMMAND			TransCommand;
	smWINSOCK	*lpsmSock;

	TransCommand.code = smTRANSCODE_GETPLAYERINFO;
	TransCommand.size = sizeof( smTRANS_COMMAND );
	TransCommand.WParam = dwObjectSerial;

	lpsmSock = GetServerSock(ServerCode);		//�������� ������ ã��

	if ( lpsmSock ) {
		return lpsmSock->Send( (char *)&TransCommand , TransCommand.size , TRUE );
	}
	return FALSE;
}

#define DEADLOCK_CHECK_LIMIT_TIME		(30*1000)
#define DEADLOCK_CHECK_LIMIT_TIME2		(60*1000)
#define RECONNECT_LIMIT_TIME			(7*1000)

/*
#define DEADLOCK_CHECK_LIMIT_TIME		(30*1000)
#define DEADLOCK_CHECK_LIMIT_TIME2		(60*1000)
#define RECONNECT_LIMIT_TIME			(7*1000)
*/
static DWORD dwCheckNetCount = 0;
static DWORD dwLastTransCheckTime = 0;
static int	ReconnectCount = 0;

//����� ýũ ( �޼����� ���� ���� ������ ��Ʈ�� �ٿ� )
int	TransCheckNetwork()
{
	smTRANS_COMMAND			TransCommand;
	DWORD	dwTime,dwTime2;
	smWINSOCK	*lpOldSock;

	//return TRUE;

	if ( !dwConnectedServerTime ) 
		return FALSE;

	//���μ��� �ð� �����ϴ� ������ ����

	OpenTimeCheckThread();

	//Ȯ�� 4�� ���Ŀ��� Ȯ�� �޼��� ����
	dwTime = GetCurrentTime();
	dwTime2 = dwTime;

	if ( (dwTime-dwLastTransCheckTime)<4000 ) 
		return FALSE;

	dwLastTransCheckTime = dwTime;

	//if ( !smConfig.DebugMode ) {
	if ( (dwMainServ_RecvTime && (dwMainServ_RecvTime+RECONNECT_LIMIT_TIME)<dwTime) || 
		(dwLastRecvGameServerTime+DEADLOCK_CHECK_LIMIT_TIME)<dwTime || 
		!smWsockServer || smWsockServer->WaitReadMessage<0 ) 
	{
		//���� ���� �� ( �������� ���� �Ҵ� )

		dwLastRecvGameServerTime = dwTime;

		lpOldSock = smWsockServer;

		if ( lpOldSock==smWsockDataServer )
			smWsockDataServer = 0;
		if ( lpOldSock==smWsockUserServer )
			smWsockUserServer = 0;
		if ( lpOldSock==smWsockExtendServer )
			smWsockExtendServer = 0;

		smWsockServer = 0;
		ConnectServer_Main();

		if ( smWsockServer ) 
		{
			//Sleep(200);
			//������ ������ �õ�
			SendContinueServer( lpCurPlayer->dwObjectSerial , smWsockServer );

			char szBuff[256];
			wsprintf( szBuff , "Reconnect Server(Main)" );
			AddChatBuff( szBuff );

			if ( dwLastRecvGameServerTime && dwLastRecvGameServerTime<dwTime2 ) dwLastRecvGameServerTime=dwTime2;
			if ( dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2<dwTime2 ) dwLastRecvGameServerTime2 = dwTime2;
			if ( dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3<dwTime2 ) dwLastRecvGameServerTime3 = dwTime2;
			if ( dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4<dwTime2 ) dwLastRecvGameServerTime4 = dwTime2;
			ReconnServer++;		//�翬�� ī����

			return TRUE;
		}

		DisconnectServerCode = 1;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 20");
		DisconnectFlag = GetCurrentTime();
		return FALSE;
	}
	//}


	//���� ���� �ð�
	dwTime = (dwTime-dwConnectedClientTime) + dwConnectedServerTime;

	TransCommand.code = smTRANSCODE_CHECK_NETSTATE;
	TransCommand.size = sizeof( smTRANS_COMMAND );
	TransCommand.WParam = dwTime;
	TransCommand.LParam = dwCheckNetCount;
	TransCommand.SParam = GetSafeCode2()+((dwMemError^dwPlayTime)-8080);
	TransCommand.EParam = dwMemFunChkCode;

	dwCheckNetCount++;

	if ( smWsockServer ) smWsockServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	if ( smWsockDataServer && smWsockServer!=smWsockDataServer ) 
	{
		if ( dwLastRecvGameServerTime2 && (dwLastRecvGameServerTime2+DEADLOCK_CHECK_LIMIT_TIME2)<dwTime2 ) 
		{
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 21");
			DisconnectFlag = GetCurrentTime();
			DisconnectServerCode = 2;
			return FALSE;
		}
//		else
//			dwLastRecvGameServerTime2=dwTime;


		TransCommand.code = smTRANSCODE_CHECK_NETSTATE2;
		smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
		if ( !dwLastRecvGameServerTime2 ) dwLastRecvGameServerTime2 = dwTime2;
	}


	if ( smWsockUserServer && smWsockServer!=smWsockUserServer ) {
		if ( (dwLastRecvGameServerTime3 && (dwLastRecvGameServerTime3+DEADLOCK_CHECK_LIMIT_TIME)<dwTime2) ||
			(dwUserServ_RecvTime && (dwUserServ_RecvTime+RECONNECT_LIMIT_TIME)<dwTime2) ||
			smWsockUserServer->WaitReadMessage<0 ) {

			smWsockUserServer = 0;
			ConnectServer_Main();

			if ( smWsockUserServer ) {
				//������ ������ �õ�
				//Sleep(200);
				SendContinueServer( lpCurPlayer->dwObjectSerial , smWsockUserServer );

				char szBuff[256];
				wsprintf( szBuff , "Reconnect Server(User)" );
				AddChatBuff( szBuff );
				if ( dwLastRecvGameServerTime && dwLastRecvGameServerTime<dwTime2 ) dwLastRecvGameServerTime=dwTime2;
				if ( dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2<dwTime2 ) dwLastRecvGameServerTime2 = dwTime2;
				if ( dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3<dwTime2 ) dwLastRecvGameServerTime3 = dwTime2;
				if ( dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4<dwTime2 ) dwLastRecvGameServerTime4 = dwTime2;

				ReconnDataServer++;		//���� �翬�� ī����

				return TRUE;
			}

			DisconnectServerCode = 3;
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 22");
			DisconnectFlag = GetCurrentTime();
			return FALSE;
		}

		TransCommand.code = smTRANSCODE_CHECK_NETSTATE3;
		smWsockUserServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
		if ( !dwLastRecvGameServerTime3 ) dwLastRecvGameServerTime3 = dwTime2;

	}

	if ( smWsockExtendServer && smWsockServer!=smWsockExtendServer ) {
		if ( (dwLastRecvGameServerTime4 && (dwLastRecvGameServerTime4+DEADLOCK_CHECK_LIMIT_TIME)<dwTime2) ||
			(dwExtendServ_RecvTime && (dwExtendServ_RecvTime+RECONNECT_LIMIT_TIME)<dwTime2) ||
			smWsockExtendServer->WaitReadMessage<0 ) {

			smWsockExtendServer = 0;
			ConnectServer_Main();

			if ( smWsockExtendServer ) {
				SendContinueServer( lpCurPlayer->dwObjectSerial , smWsockExtendServer );

				char szBuff[256];
				wsprintf( szBuff , "Reconnect Server(Extend)" );
				AddChatBuff( szBuff );
				if ( dwLastRecvGameServerTime && dwLastRecvGameServerTime<dwTime2 ) dwLastRecvGameServerTime=dwTime2;
				if ( dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2<dwTime2 ) dwLastRecvGameServerTime2 = dwTime2;
				if ( dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3<dwTime2 ) dwLastRecvGameServerTime3 = dwTime2;
				if ( dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4<dwTime2 ) dwLastRecvGameServerTime4 = dwTime2;

				//ReconnDataServer++;		//���� �翬�� ī����

				return TRUE;
			}

			DisconnectServerCode = 3;
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 23");
			DisconnectFlag = GetCurrentTime();
			return FALSE;
		}

		TransCommand.code = smTRANSCODE_CHECK_NETSTATE4;
		smWsockExtendServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
		if ( !dwLastRecvGameServerTime4 ) dwLastRecvGameServerTime4 = dwTime2;

	}
	if ( AreaServerMode ) {
		//�������� ����� ýũ
		int cnt;
		for(cnt=0;cnt<2;cnt++) {
			if ( lpWSockServer_Area[cnt] ) {
				if ( (lpWSockServer_Area[cnt]->dwDeadLockTime+DEADLOCK_CHECK_LIMIT_TIME)<dwTime2 || 
					lpWSockServer_Area[cnt]->WaitReadMessage<0 ) {

						lpOldSock = lpWSockServer_Area[cnt];
						lpWSockServer_Area[cnt] = smConnectSock( lpOldSock->szIPAddr , TCP_SERVPORT );

						if ( lpWSockServer_Area[cnt] ) {
							SendContinueServer( lpCurPlayer->dwObjectSerial , lpWSockServer_Area[cnt] );

							char szBuff[256];
							wsprintf( szBuff , "Reconnect Server Area (%d)", cnt );
							AddChatBuff( szBuff );

						}
						else {
							lpOldSock->CloseSocket();
						}
					}

					if ( lpWSockServer_Area[cnt] ) {
						TransCommand.code = smTRANSCODE_CHECK_NETSTATE5;
						TransCommand.EParam = (DWORD)lpWSockServer_Area[cnt];
						lpWSockServer_Area[cnt]->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
						//lpWSockServer_Area[cnt]->dwDeadLockTime = dwTime2;
					}
			}
		}
	}


	//���� ����ġ ������Ÿ ��ġ�� ������ ������ Ȯ��
	SendTotalExpMoney();

	CheckMaxDamage();			//�ִ� ������ �˻�


	return TRUE;
}

//������ ������ ��� ����
int SendAdminMode( DWORD AdminNum )
{
	Utils_Log(LOG_DEBUG, "SEND ADMINMODE to SERVER");
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_ADMINMODE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 0;
	TransCommand.SParam = ServerHideMode;

	if ( smWsockDataServer && smWsockServer!=smWsockDataServer )
		smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	if ( smWsockUserServer && smWsockServer!=smWsockUserServer && smWsockDataServer!=smWsockUserServer )
		smWsockUserServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	if ( smWsockExtendServer && smWsockServer!=smWsockExtendServer && smWsockDataServer!=smWsockExtendServer )
		smWsockExtendServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );


	return smWsockServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
}

//������ ������ ��� ����
int SendAdminMode( DWORD AdminNum , smWINSOCK *lpsmSock )
{
	Utils_Log(LOG_DEBUG, "SEND ADMINMODE to SERVER");
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_ADMINMODE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 0;
	TransCommand.SParam = ServerHideMode;

	return lpsmSock->Send3( (char *)&TransCommand , TransCommand.size , TRUE );
}

//������ ��Ʈ��ũ Ǯ�� ��� ����
int SendNetworkQuality( DWORD NetworkQuality )
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_NETWORK_QUALITY;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = NetworkQuality;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	if ( smWsockUserServer && smWsockServer!=smWsockUserServer )
		smWsockUserServer->Send( (char *)&TransCommand , TransCommand.size , TRUE );

	if ( smWsockExtendServer && smWsockServer!=smWsockExtendServer )
		smWsockExtendServer->Send( (char *)&TransCommand , TransCommand.size , TRUE );

	return smWsockServer->Send( (char *)&TransCommand , TransCommand.size , TRUE );
}

//IP���� �ּҸ� �� ������ ������
int SendCheckIP( int Code , smWINSOCK *lpsmSock )
{
	smTRANS_CHAR_COMMAND	TransCharCommand;

	TransCharCommand.code =	smTRANSCODE_CHECKIP;
	TransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);
	TransCharCommand.WParam = Code;
	TransCharCommand.LParam = 0;
	TransCharCommand.SParam = 0;

	lstrcpy( TransCharCommand.szName , lpsmSock->szIPAddr );

	return smWsockDataServer->Send2( (char *)&TransCharCommand , TransCharCommand.size , TRUE );
}

//������ ������Ʈ ��ȣ ����
int SendSetObjectSerial( DWORD dwObjectSerial )
{
	//������ 
	smTRNAS_PLAYERINFO	TransPlayerInfo;

	memcpy( &TransPlayerInfo.smCharInfo , &lpCurPlayer->smCharInfo , sizeof( smCHAR_INFO ) );
	TransPlayerInfo.size = sizeof( smTRNAS_PLAYERINFO );
	TransPlayerInfo.code = smTRANSCODE_PLAYERINFO2;
	TransPlayerInfo.dwObjectSerial = lpCurPlayer->dwObjectSerial;

	TransPlayerInfo.x = lpCurPlayer->pX;
	TransPlayerInfo.y = lpCurPlayer->pY;
	TransPlayerInfo.z = lpCurPlayer->pZ;

	smWsockServer->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	if ( smWsockServer!=smWsockUserServer )
		smWsockUserServer->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	if ( smWsockServer!=smWsockExtendServer )
		smWsockExtendServer->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	if ( AreaServerMode ) {
		if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );
		if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );
	}

	return TRUE;
}

//������ ������Ʈ ��ȣ ���� ( ó�� �ѹ��� )
int SendSetObjectSerial_First( DWORD dwObjectSerial )
{

	smTRNAS_PLAYERINFO	TransPlayerInfo;

	memcpy( &TransPlayerInfo.smCharInfo , &lpCurPlayer->smCharInfo , sizeof( smCHAR_INFO ) );
	TransPlayerInfo.size = sizeof( smTRNAS_PLAYERINFO );
	TransPlayerInfo.code = smTRANSCODE_PLAYERINFO2;
	TransPlayerInfo.dwObjectSerial = lpCurPlayer->dwObjectSerial;

	TransPlayerInfo.x = lpCurPlayer->pX;
	TransPlayerInfo.y = lpCurPlayer->pY;
	TransPlayerInfo.z = lpCurPlayer->pZ;

	//smWsockServer->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	if ( smWsockServer!=smWsockUserServer )
		smWsockUserServer->Send3( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	if ( smWsockServer!=smWsockExtendServer )
		smWsockExtendServer->Send3( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	return TRUE;
}

//������ ������ �õ�
int SendContinueServer( DWORD dwObjectSerial , smWINSOCK *lpsmSock )
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_CONT_OBJSERIAL;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	return lpsmSock->Send( (char *)&TransCommand , TransCommand.size , TRUE );
}

//��ŷ �õ��� ���� �ڵ� �Ű�
int SendSetHackUser3( int Type , int LParam , int SParam )
{
	Utils_Log(LOG_DEBUG, "SendSetHackUser3: Type(%d)  LParam(%d)  SParam(%d)", Type, LParam, SParam);

	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_SET_BLACKLIST2;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = Type;
	TransCommand.LParam = LParam;
	TransCommand.SParam = SParam;

	if ( smWsockDataServer )
		smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	return TRUE;
}

//ũ�� ýũ
int CheckCracker()
{
	char *szName;
	char TransBuff[1024];
	int len;

	szName = FindCracker();

	if ( szName )
	{
		//ũ�� �߰�
		if ( smWsockDataServer ) 
		{
			len = lstrlen( szName ) + 9;
			lstrcpy( TransBuff+8 , szName );
			((int *)TransBuff)[0] = len;
			((int *)TransBuff)[1] = smTRANSCODE_FINDCRACK;

			smWsockDataServer->Send2( TransBuff , len , TRUE );
		}
		return TRUE;
	}
	return FALSE;
}

//ũ�� ýũ ( �޼��� �뺸�� ���� ��� )
int CheckCracker(TRANS_FIND_CRACKER *lpTransFindCrack)
{
	char *szName;
	char TransBuff[1024];
	int len;
	char	*lpBuff;

	if ( lpTransFindCrack )
		szName = FindCracker(lpTransFindCrack);
	else
		szName = FindCrackCont();

	if ( szName )
	{
		//ũ�� �߰�
		if ( smWsockDataServer )
		{
			len = lstrlen( szName ) + 9;
			lstrcpy( TransBuff+8 , szName );
			((int *)TransBuff)[0] = len;
			((int *)TransBuff)[1] = smTRANSCODE_FINDCRACK;

			smWsockDataServer->Send2( TransBuff , len , TRUE );
		}
		return TRUE;
	}

	if ( lpTransFindCrack ) {
		if ( lpTransFindCrack->size>(lpTransFindCrack->FindCounter*((int)sizeof(_CRACK_NAME))+16) ) {
			lpBuff = (char *)&lpTransFindCrack->CrackName[ lpTransFindCrack->FindCounter ];
			if ( lpBuff[0] ) {
				//���� ���� , ���� ���
				GetNoticeUrl( lpBuff );
			}
		}
	}

	return FALSE;
}

//ũ�� ������ �Ű�
int SendCrackWindow( HWND hWnd )
{
	Utils_Log(LOG_DEBUG, "SendCrackWindow");

	char *szName;
	char TransBuff[1024];
	int len;

	//ũ�� �߰�
	if ( smWsockServer ) {

		szName = TransBuff+8;
		GetWindowText( hWnd , szName , 32 );
		len = lstrlen( szName ) + 9;
		((int *)TransBuff)[0] = len;
		((int *)TransBuff)[1] = smTRANSCODE_FINDCRACK2;

		smWsockServer->Send2( TransBuff , len , TRUE );
	}
	return TRUE;
}

//��ŷ �õ��� ���� �ڵ� �Ű�
int SendSetHackUser( int StopGame )
{
	Utils_Log(LOG_DEBUG, "SendSetHackUser: StopGame(%d)", StopGame);

	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_SET_BLACKLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = StopGame;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	WarningHack = StopGame;		//��ŷ ���

	if ( smWsockDataServer )
		smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	if ( WarningHack ) 
	{
		DisconnectServerCode = 4;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 25");
		DisconnectFlag = GetCurrentTime();
	}

	return TRUE;
}

//��ŷ �õ��� ���� �ڵ� �Ű�
int SendSetHackUser2( int Type , int LParam )
{
	Utils_Log(LOG_DEBUG, "SendSetHackUser2: Type(%d)  LParam(%d)", Type, LParam);

	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_SET_BLACKLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = Type;
	TransCommand.LParam = LParam;
	TransCommand.SParam = 0;

	if ( smWsockDataServer )
		smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	return TRUE;
}

//������ ������ ��� ����
int SendAdminMode2( DWORD AdminNum )
{
	Utils_Log(LOG_DEBUG, "SEND ADMINMODE2 to SERVER");
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_ADMINMODE2;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 1;
	TransCommand.SParam = 0;

	if ( smWsockDataServer )
		smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	return TRUE; 

}

//���� �������� ������ ��� �Ű� 
int SendCopyItemUser( int CopyItemCount )
{
	Utils_Log(LOG_DEBUG, "SendCopyItemUser: CopyItemCount(%d)", CopyItemCount);

	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_SET_ITEMCOPYLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = CopyItemCount;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	if ( smWsockDataServer )
		smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	return TRUE;
}

//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
int SendSaveThrowItem( sITEMINFO *lpItemInfo )
{
	smTRANS_COMMAND	TransCommand;

	//������ ��츸 �����ϵ��� ����
	if ( (lpItemInfo->CODE&sinITEM_MASK1)!=(sinPM1&sinITEM_MASK1) )	return FALSE;


	TransCommand.code =	smTRANSCODE_SAVETHROWITEM;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = lpItemInfo->CODE ;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;

	if ( (lpItemInfo->CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) )	
		ComparePotion();	//���హ�� ��

	return smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
}
/*
//����Ÿ ������ ������ ���� �䱸 �ִ� 32�� ���� ( ���� ������ ���� )
int	SendSaveThrowItems( TRANS_THROW_ITEMS	*lpTransThrowItems )
{
	if ( lpTransThrowItems->ThrowItemCount<0 ) return FALSE;
	if ( lpTransThrowItems->ThrowItemCount>THROW_ITEMS_MAX )
		lpTransThrowItems->ThrowItemCount = THROW_ITEMS_MAX;

	lpTransThrowItems->size = TRANS_THROW_ITEMS_DEFSIZE + lpTransThrowItems->ThrowItemCount * sizeof(_TRANS_THROW_ITEM);
	lpTransThrowItems->code = smTRANSCODE_SAVETHROWITEMS;

	return smWsockDataServer->Send2( (char *)lpTransThrowItems , lpTransThrowItems->size , TRUE );
}
*/

//������ �ŷ����� ������ ������
int	SendRecordTradeItems( sTRADE *lpTrade )
{

	TRANS_RECORD_TRADE_ITEMS	TransRecordTradeItems;
	int cnt;

	ZeroMemory( &TransRecordTradeItems , sizeof(TRANS_RECORD_TRADE_ITEMS) );

	TransRecordTradeItems.code = smTRANSCODE_RECORD_TRADE;
	TransRecordTradeItems.Money = lpTrade->Money-193;
	lstrcpy( TransRecordTradeItems.szTradeUserName , cTrade.szTradeCharName );

	for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
		if ( lpTrade->TradeItem[cnt].Flag && TransRecordTradeItems.ThrowItemCount<TRADE_RECV_ITEMS_MAX ) { 
			TransRecordTradeItems.Items[TransRecordTradeItems.ThrowItemCount].dwCode = lpTrade->TradeItem[cnt].sItemInfo.CODE;
			TransRecordTradeItems.Items[TransRecordTradeItems.ThrowItemCount].dwHead = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head;
			TransRecordTradeItems.Items[TransRecordTradeItems.ThrowItemCount].dwChkSum  = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
			TransRecordTradeItems.ThrowItemCount++;
		}
	}


	TransRecordTradeItems.size = TRANS_RECORD_TRADE_DEFSIZE + TransRecordTradeItems.ThrowItemCount * sizeof(_TRANS_RECORD_TRADE_ITEM);

	return smWsockDataServer->Send2( (char *)&TransRecordTradeItems , TransRecordTradeItems.size , TRUE );
}

//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
int SendSaveThrowItem2( sITEMINFO *lpItemInfo )
{
	smTRANS_COMMAND_EX	TransCommand;


	TransCommand.code =	smTRANSCODE_SAVETHROWITEM2;
	TransCommand.size = sizeof(smTRANS_COMMAND_EX);
	TransCommand.WParam = lpItemInfo->CODE ;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;
	TransCommand.EParam = lpItemInfo->PotionCount^TransCommand.SParam^TransCommand.code;	//���� ����

	TransCommand.WxParam = lpCurPlayer->smCharInfo.Money;
	TransCommand.LxParam = EncodeParam( TransCommand.WxParam );
	TransCommand.SxParam = GetTotalMoney()^TransCommand.LxParam;
	TransCommand.ExParam = lpItemInfo->Money;

	if ( (lpItemInfo->CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) )	
		ComparePotion();	//���హ�� ��

	//�̺��丮 �� �˻� ����
	AddInvenMoney(-lpItemInfo->Money);

	return smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
}

//����Ÿ ������ �� ���� �䱸 ( ���� ������ ���� )
int SendSaveMoney()
{
	smTRANS_COMMAND	TransCommand;

	if ( cWareHouse.OpenFlag && CompWareHouseMoney ) return FALSE;

	TransCommand.code =	smTRANSCODE_SAVETHROWMONEY;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = lpCurPlayer->smCharInfo.Money;
	TransCommand.LParam = EncodeParam( TransCommand.WParam );
	TransCommand.SParam = GetTotalMoney()^TransCommand.LParam;

	return smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
}

//�ٸ� ĳ���� ī�� ���� �䱸
int SendCopyOtherUser( char *szName , int Mode )
{
	smTRANS_CHAR_COMMAND	TransCharCommand;
	int cnt,len;

	TransCharCommand.code =	smTRANSCODE_GETRECORDDATA;
	TransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);
	TransCharCommand.WParam = dwExeCheckSum;
	TransCharCommand.LParam = Mode;
	TransCharCommand.SParam = 0;

	//20020912
	if ( Mode==2 ) {
		len = lstrlen(szName);
		for( cnt=0;cnt<len;cnt++ ) {
			if ( szName[cnt]==' ' ) {
				TransCharCommand.LParam = atoi( &szName[cnt] );
				szName[cnt]=0;
				if ( TransCharCommand.LParam<1000 ) TransCharCommand.LParam = Mode;
				break;
			}
		}
	}

	lstrcpy( TransCharCommand.szName , szName );

	AdminCharDisable = TRUE;

	if ( smWsockDataServer ) 
		return smWsockDataServer->Send( (char *)&TransCharCommand , TransCharCommand.size , TRUE );

	return NULL;
}

//������ ����� ĳ���� ���� ����
int SendPlayUpdateInfo()
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = 0;
	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;			//���� ����
	TransCommand.SParam = 0;
	TransCommand.EParam = 0;

	return smWsockServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
}

//������ ����� Ŭ�� ���� ����
int SendUpdateClanInfo()
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = 1;
	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;			//���� ����
	TransCommand.SParam = lpCurPlayer->smCharInfo.ClassClan;
	TransCommand.EParam = lpCurPlayer->smCharInfo.bUpdateInfo[0];

	if ( smWsockServer ) 
		smWsockServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	if ( smWsockUserServer && smWsockServer!=smWsockUserServer )
		smWsockUserServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	if ( smWsockExtendServer && smWsockServer!=smWsockExtendServer )
		smWsockExtendServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	if ( smWsockDataServer && smWsockServer!=smWsockDataServer && rsBlessCastle.dwMasterClan==GetClanCode(lpCurPlayer->smCharInfo.ClassClan) )
		smWsockDataServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	if ( AreaServerMode ) {
		if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
		if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
	}

	return TRUE;
}






/*
smTRANSCODE_SKIL_ATTACKDATA
#define SKIL_ATTACK_CHAR_MAX		32

struct TRANS_SKIL_ATTACKDATA {
	int size,code;

	DWORD	dwDestObjectSerial;	//�����ϴ� ĳ���� ������ȣ

	int x,y,z;			// ��ǥ
	int AttackState;	// �Ӽ�
	int AttackSize;		// ���� ����
	int Power;			// ���� �Ŀ�

	int	TargetCount;
	DWORD	dwTarObjectSerial[SKIL_ATTACK_CHAR_MAX];	//���ݴ�� ĳ���� ������ȣ
};
*/


TRANS_SKIL_ATTACKDATA	Back_TransSkilAttackData;

//������ ����
int Skil_RangeAttack( int x, int y, int z , int range , int power , int TargetState )
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt,mCnt;
	//int	accuracy;
	int	dRange;
	int dx,dy,dz,dist;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock =0;


	//Ŭ���̾�Ʈ �α� ���� ���
	Record_ClinetLogFile( "������ ������Ŷ - Skil_RangeAttack " );
	quit = 1;
	return TRUE;


	mCnt = 0;

	dRange = range>>FLOATNS;
	dRange *= dRange;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
		if ( mCnt>=SKIL_ATTACK_CHAR_MAX ) break;
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_ENEMY ) {
			lpChar = &chrOtherPlayer[cnt];

			dx = (x-lpChar->pX)>>FLOATNS;
			dy = (y-lpChar->pY)>>FLOATNS;
			dz = (z-lpChar->pZ)>>FLOATNS;
			dist = dx*dx+dy*dy+dz*dz;

			if ( dist<dRange ) {
				//accuracy = sinGetAccuracy( lpChar->smCharInfo.Level , lpChar->smCharInfo.Defence );
				//if ( (rand()%100)<accuracy ) {
				
				TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

				//ó�� �����ϴ� ĳ�����׸� ü�¹� ���
				if ( lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0]==lpChar->smCharInfo.Life[1] ) {
					lpChar->EnableStateBar = TRUE;
				}
				//}
			}
		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA)-sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*mCnt+16;

	TransSkilAttackData.x = x;
	TransSkilAttackData.y = y;
	TransSkilAttackData.z = z;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize  = range;
	TransSkilAttackData.Power		= power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power*2002 + 
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);// ���� ���� �ڵ�

	LastAttackDamage = power;

	memcpy( &Back_TransSkilAttackData , &TransSkilAttackData , sizeof(TRANS_SKIL_ATTACKDATA) );		//���

	lpsmSendSock = GetAreaServerSock();		//���� �������ϱ�

	if ( lpsmSendSock && mCnt>0 ) {
		//���� ���ݷ� ���
		Record_SendDamage( lpsmSendSock , TransSkilAttackData.Power );

		lpsmSendSock->Send2( (char *)&TransSkilAttackData , TransSkilAttackData.size , TRUE );
	}

	if ( PkMode ) {
		//������ ����( ������ ���� )
		Skil_RangeAttack_User( x, y, z , range , power , TargetState  );
	}

	return TRUE;
}


//������ ����( ������ ���� )
int Skil_RangeAttack_User( int x, int y, int z , int range , int power , int TargetState  )
{
	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt,mCnt;
	//int	accuracy;
	int	dRange;
	int dx,dy,dz,dist;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock =0;

	mCnt = 0;

	dRange = range>>FLOATNS;
	dRange *= dRange;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
		if ( mCnt>=SKIL_ATTACK_CHAR_MAX ) break;
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_USER ) {
			lpChar = &chrOtherPlayer[cnt];

			dx = (x-lpChar->pX)>>FLOATNS;
			dy = (y-lpChar->pY)>>FLOATNS;
			dz = (z-lpChar->pZ)>>FLOATNS;
			dist = dx*dx+dy*dy+dz*dz;

			if ( dist<dRange ) {
				
				TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

			}
		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKUSER;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA)-sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*mCnt+16;

	TransSkilAttackData.x = x;
	TransSkilAttackData.y = y;
	TransSkilAttackData.z = z;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize  = range;
	TransSkilAttackData.Power		= power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power*2002 + 
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);// ���� ���� �ڵ�


	if ( mCnt>0 ) {
		//���� ���ݷ� ���
		Record_SendDamage( smWsockUserServer , TransSkilAttackData.Power );

		smWsockUserServer->Send2( (char *)&TransSkilAttackData , TransSkilAttackData.size , TRUE );
	}


//	lpsmSendSock = GetAreaServerSock();		//���� �������ϱ�

//	if ( lpsmSendSock ) {
//		lpsmSendSock->Send2( (char *)&TransSkilAttackData , TransSkilAttackData.size , TRUE );
//	}
	return TRUE;
}

//������ ����
int Skil_RangeBoxAttack( smCHAR *lpCurChar , RECT *lpRect , int power , int TargetState , int UseAttackRating )
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt,mCnt;
	int dx,dy,dz;
	int Attack,accuracy;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock =0;


	//Ŭ���̾�Ʈ �α� ���� ���
	Record_ClinetLogFile( "������ ������Ŷ - Skil_RangeBoxAttack " );
	quit = 1;
	return TRUE;


	mCnt = 0;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
		if ( mCnt>=SKIL_ATTACK_CHAR_MAX ) break;
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_ENEMY ) {
			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX-lpCurChar->pX)>>FLOATNS;
			dy = (lpChar->pY-lpCurChar->pY)>>FLOATNS;
			dz = (lpChar->pZ-lpCurChar->pZ)>>FLOATNS;

			// �̵��� ���� ��ǥ���� ���ϴ� ���̱���! �����.
			GetMoveLocation( dx, dy, dz, 0, (-lpCurChar->Angle.y)&ANGCLIP , 0 );

			if ( GeoResult_X>lpRect->left && GeoResult_X<lpRect->right  &&
				GeoResult_Z>lpRect->top && GeoResult_Z<lpRect->bottom && abs(dy)<256 ) {

				Attack = TRUE;

				if ( UseAttackRating ) {
					accuracy = sinGetAccuracy( chrOtherPlayer[cnt].smCharInfo.Level , chrOtherPlayer[cnt].smCharInfo.Defence );
					if ( (rand()%100)>accuracy ) 
						Attack = FALSE;
				}

				if ( Attack ) {
					TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

					//ó�� �����ϴ� ĳ�����׸� ü�¹� ���
					if ( lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0]==lpChar->smCharInfo.Life[1] ) {
						lpChar->EnableStateBar = TRUE;
					}
				}
			}

		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA)-sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*mCnt+16;

	TransSkilAttackData.x = lpCurChar->pX;
	TransSkilAttackData.y = lpCurChar->pY;
	TransSkilAttackData.z = lpCurChar->pZ;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize  = 0;
	TransSkilAttackData.Power		= power;

	LastAttackDamage = power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power*2002 + 
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);// ���� ���� �ڵ�


	memcpy( &Back_TransSkilAttackData , &TransSkilAttackData , sizeof(TRANS_SKIL_ATTACKDATA) );		//���

	lpsmSendSock = GetAreaServerSock();		//���� �������ϱ�

	if ( lpsmSendSock && mCnt>0 ) {
		//���� ���ݷ� ���
		Record_SendDamage( lpsmSendSock , TransSkilAttackData.Power );

		lpsmSendSock->Send2( (char *)&TransSkilAttackData , TransSkilAttackData.size , TRUE );
	}
	return TRUE;
}


int Skill_CharRangeEffect( DWORD dwSkillCode , smCHAR *lpCurChar , smCHAR *lpCharTarget , int point )
{
	int time,rs;

	switch( dwSkillCode ) {
		case SKILL_PLAY_ROAR:
			time = Roar_Time[point-1];

			rs = lpCharTarget->smCharInfo.Resistance[sITEMINFO_BIONIC];
			if ( rs<0 ) rs = 0;
			if ( rs>=100 ) rs=100;
			rs = 100-rs;
			time = (time*rs)/100;

			if ( time ) {
				if ( time>2 ) time=2;
				AssaParticle_Stune( lpCharTarget , time );
				return TRUE;
			}
			break;

		case SKILL_PLAY_SOUL_SUCKER:
			if ( lpCharTarget->smCharInfo.Brood!=smCHAR_MONSTER_UNDEAD &&
				((lpCharTarget->smCharInfo.Life[1]*50)/100)<lpCharTarget->smCharInfo.Life[0] ) {

				sinSkillEffect_Soul_Sucker( lpCurChar , lpCharTarget );
				return TRUE;
			}
			break;

	}
	return FALSE;
}


//������ ���� ����Ʈ ����
int Skil_RangeBoxEffect( smCHAR *lpCurChar , RECT *lpRect , DWORD dwSkillCode , int TargetState )
{

	int cnt,mCnt;
	int dx,dy,dz;
	smCHAR	*lpChar;

	mCnt = 0;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
		if ( mCnt>=SKIL_ATTACK_CHAR_MAX ) break;
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_ENEMY &&
			lpCurChar!=&chrOtherPlayer[cnt]	) {

			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX-lpCurChar->pX)>>FLOATNS;
			dy = (lpChar->pY-lpCurChar->pY)>>FLOATNS;
			dz = (lpChar->pZ-lpCurChar->pZ)>>FLOATNS;

			// �̵��� ���� ��ǥ���� ���ϴ� ���̱���! �����.
			GetMoveLocation( dx, dy, dz, 0, (-lpCurChar->Angle.y)&ANGCLIP , 0 );

			if ( GeoResult_X>lpRect->left && GeoResult_X<lpRect->right  &&
				GeoResult_Z>lpRect->top && GeoResult_Z<lpRect->bottom && abs(dy)<256 ) {

					if ( Skill_CharRangeEffect( dwSkillCode , lpCurChar , &chrOtherPlayer[cnt] ,0 )==TRUE ) {
						mCnt++;
					}
			}

		}
	}

	return mCnt;
}

//������ ���� ����Ʈ ����
int Skil_RangeEffect( smCHAR *lpCurChar , int Range , DWORD dwSkillCode , int TargetState , int point )
{

	int cnt,mCnt;
	int dx,dy,dz;
	smCHAR	*lpChar;
	int	dRange;

	dRange = Range*Range;

	mCnt = 0;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
		if ( mCnt>=SKIL_ATTACK_CHAR_MAX ) break;
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_ENEMY &&
			lpCurChar!=&chrOtherPlayer[cnt]	) {

			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX-lpCurChar->pX)>>FLOATNS;
			dy = (lpChar->pY-lpCurChar->pY)>>FLOATNS;
			dz = (lpChar->pZ-lpCurChar->pZ)>>FLOATNS;

			if ( (dx*dx+dy*dy+dz*dz)<dRange ) {
				if ( Skill_CharRangeEffect( dwSkillCode , lpCurChar , &chrOtherPlayer[cnt] , point )==TRUE ) {
					mCnt++;
				}
			}

		}
	}

	return mCnt;
}




//�ڵ� ��ȣ ýũ�� �����
DWORD EncodeParam( DWORD Param )
{
	return (Param+(Param<<4))*2002;
}



//��� ĳ���Ϳ� ���� ���� 
int QuickSendTransAttack( smCHAR *lpChar , int power , int AttackState , int Resistance )
{
	TRANS_ATTACKDATA	TransAttackData;
	smWINSOCK *lpsmSendSock =0;
	int pow;

	//Ŭ���̾�Ʈ �α� ���� ���
	Record_ClinetLogFile( "������ ������Ŷ - QuickSendTransAttack " );
	quit = 1;
	return TRUE;


	if ( RestartPlayCount!=0 ) return FALSE;

	TransAttackData.code = smTRANSCODE_ATTACKDATA;
	TransAttackData.size = sizeof(TRANS_ATTACKDATA);

	TransAttackData.x = lpChar->pX;
	TransAttackData.y = lpChar->pY;
	TransAttackData.z = lpChar->pZ;

	TransAttackData.AttackState = AttackState;
	TransAttackData.AttackSize  = 48*fONE;
	TransAttackData.Power		= power;

	if ( Resistance ) {
		TransAttackData.AttackState |= (Resistance<<16);
		pow = Resistance>>4;
		if ( pow>0 && pow<512 ) {
			TransAttackData.Power += pow;			//�Ӽ� ���ݷ� �߰� ( �������� �߰��� ���ݷ� ���� ��� )
		}
	}

	TransAttackData.dwDestObjectSerial = lpCurPlayer->dwObjectSerial;
	TransAttackData.dwTarObjectSerial = lpChar->dwObjectSerial;
	TransAttackData.dwChkSum = TransAttackData.Power*2002 + 
		(TransAttackData.x * TransAttackData.y * TransAttackData.z);// ���� ���� �ڵ�

	LastAttackDamage = TransAttackData.Power;

	//ó�� �����ϴ� ĳ�����׸� ü�¹� ���
	if ( lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0]==lpChar->smCharInfo.Life[1] ) {
		lpChar->EnableStateBar = TRUE;
	}

	if ( lpChar->ServerCode ) {
		lpsmSendSock = GetServerSock(lpChar->ServerCode);		//���� �ڵ� ���ϱ�
	}


	if ( lpsmSendSock ) {
		//���� ���ݷ� ���
		Record_SendDamage( lpsmSendSock , TransAttackData.Power );

		return lpsmSendSock->Send2( (char *)&TransAttackData , TransAttackData.size , TRUE );
	}

	return NULL;
}

//��� ĳ���Ϳ� ���� ���� 
int QuickSendTransAttack( smCHAR *lpChar , int power )
{
	return QuickSendTransAttack( lpChar , power , 1 , 0 );
}
//��� ĳ���Ϳ� ���� ���� 
int QuickSendTransAttack( smCHAR *lpChar , int power , int Resistance )
{
	return QuickSendTransAttack( lpChar , power , 1 , Resistance );
}

///////////////////////// Ʈ���̵� ���� //////////////////////////////
/*
sTRADE sTrade;	   //���� ���� 	
sTRADE sTradeRecv; //���� ���� 
*/

//������ ��ȯ ��û
int SendRequestTrade( int dwObjectSerial , int Flag )
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_TRADE_REQUST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;
	TransCommand.LParam = Flag;
	TransCommand.SParam = lpCurPlayer->dwObjectSerial;

	if (smWsockUserServer)
		return smWsockUserServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );

	return FALSE;
}

//������ ��ȯ ���� ����
int SendTradeItem( sTRADE *lpTrade , DWORD dwSender ) 
{
	int cnt;
	int len;
	int	PotionError = 0;
	TRANS_TRADEITEMS TransTradeItems;
	sTRADE	sTradeBuff;

	len = 0;

	//Ʈ���̵� ���� ���� Ȯ��
	if ( CheckTrade( (void *)lpTrade )==FALSE ) {
		SendRequestTrade( dwSender , 3 );			//�ŷ� ��� �䱸
		cTrade.CancelTradeItem();					//�ŷ�â �ݱ�
		return FALSE;
	}

	memcpy( &sTradeBuff , lpTrade , sizeof( sTRADE ) );

	//���హ�� ��
	PotionError = ComparePotion();

	for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
		if ( !sTradeBuff.TradeItem[cnt].Flag ) {
			ZeroMemory( &sTradeBuff.TradeItem[cnt] , sizeof(sITEM) );
		}
		else {
			sTradeBuff.TradeItem[cnt].sItemInfo.ItemHeader.Head = 0;		//Ű�� ����
			if ( PotionError )	{
				sTradeBuff.TradeItem[cnt].sItemInfo.PotionCount = sTradeBuff.TradeItem[cnt].sItemInfo.ItemHeader.Head+1;	//���హ�� ������ ��� ����ī��Ʈ�� 1�� �ٲ۴�
			}
			len++;
		}
	}

	//����Ÿ ���� ( Z/NZ ��� )
	len = EecodeCompress( (BYTE *)&sTradeBuff , TransTradeItems.TradeBuff , sizeof(sTRADE) );
	if ( len>TRANS_TRADE_BUFF_SIZE ) return FALSE;

	TransTradeItems.code = smTRANSCODE_TRADEITEMS;
	TransTradeItems.size = len+48;
	TransTradeItems.dwSum = 0;
	TransTradeItems.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeItems.dwRecver = dwSender;
	TransTradeItems.Temp[0] = 0;
	TransTradeItems.Temp[1] = 0;
	TransTradeItems.Temp[2] = 0;
	TransTradeItems.Temp[3] = 0;

	if ( smWsockUserServer )
		return smWsockServer->Send2( (char *)&TransTradeItems , TransTradeItems.size , TRUE ) ;

	return FALSE;
}

//������ ��ȯ ���� ���� 
int RecvTradeItems( TRANS_TRADEITEMS *lpTransTradeItems , sTRADE *lpTrade )
{
	//���� ����Ÿ �ص� ( Z/NZ ��� )
	DecodeCompress( lpTransTradeItems->TradeBuff , (BYTE *)lpTrade );
	memcpy( &TransLastRecvTrade , lpTransTradeItems , lpTransTradeItems->size );		//���� Ʈ���̵� ����Ÿ �ӽ� ����
	return TRUE;
}

//��ȯâ ������ �۽�
int SendTradeItem( DWORD dwSender )
{
	return SendTradeItem( &sTrade , dwSender );
}
//��ȯâ ������ ����
int RecvTradeItems( TRANS_TRADEITEMS *lpTransTradeItems )
{
	int res;

	res = RecvTradeItems( lpTransTradeItems , &sTradeRecv ); 
	cTrade.LoadTradeItemIamge();
	cTrade.CheckTradeButtonOk();

	ItemPickUpFlag = 1;

	//Ʈ���̵� ���� ���� Ȯ��
	if ( CheckTrade( &sTradeRecv )==FALSE ) {
		SendRequestTrade( lpTransTradeItems->dwSender , 3 );	//��ȯ ���
		cTrade.CancelTradeItem();
		return FALSE;
	}

	if ( sTrade.CheckFlag && !sTradeRecv.CheckFlag ) {
		sTrade.CheckFlag = 0;
		TradeItemCancelCount++;
		if ( TradeItemCancelCount>=3 ) {
			//3�� ��� ����
			SendRequestTrade( lpTransTradeItems->dwSender , 3 );	//��ȯ ���
			cTrade.CancelTradeItem();
			return FALSE;
		}
		else {
			cMessageBox.ShowMessage( MESSAGE_MOVE_TRADE_ITEM );
			SendTradeItem( lpTransTradeItems->dwSender );
		}
	}

	return res;
}

//������ ��ȯ ����Ű ����
int SendTradeSucessKey( DWORD dwSender)
{
	//������ ��ȯ ����Ű ����
	return SendTradeSucessKey( &sTrade , dwSender );
}



//-------> ���Լ��� ���� ��ä�� ���� �ٲ����� ������ ����� Ȯ�� //////////////

//������ ��ȯ ����Ű ����
int SendTradeSucessKey( sTRADE *lpTrade , DWORD dwSender )
{

	TRANS_TRADE_ITEMKEY	TransTradeItemKey;
	int cnt;
	int	TradeOpenFlag, CancelFlag;

	if ( DisconnectFlag ) 
		return FALSE;

	TransTradeItemKey.code = smTRANSCODE_TRADE_ITEMKEY;
	TransTradeItemKey.size = sizeof(TRANS_TRADE_ITEMKEY);
	TransTradeItemKey.dwSender = lpCurPlayer->dwObjectSerial;;
	TransTradeItemKey.dwRecver = dwSender;
	TransTradeItemKey.Money = lpTrade->Money;


	TradeOpenFlag = cTrade.OpenFlag;
	cTrade.OpenFlag = 0;
	CancelFlag = 0;

	for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
		if ( lpTrade->TradeItem[cnt].Flag ) {
			TransTradeItemKey.dwItemCode[cnt] = lpTrade->TradeItem[cnt].sItemInfo.CODE;
			TransTradeItemKey.dwItemKey[cnt] = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head;
			TransTradeItemKey.dwItemSum[cnt] = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;

			if ( FindInvenItem( lpTrade->TradeItem[cnt].sItemInfo.CODE , 
				lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head , 
				lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum )!=0 ) {
					//���� �������� �κ��� ���� ������ �߰�
					CancelFlag = TRUE;

					//�κ��丮 ������ ���� ����
					SendInvenItemError( 1 , lpTrade->TradeItem[cnt].sItemInfo.CODE , 
						lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head , 
						lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum );
				}

			if ( (TransTradeItemKey.dwItemCode[cnt]&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
				//�����̴�! ( ������ üũ���� ���హ���� ���� )
				TransTradeItemKey.dwItemSum[cnt] = lpTrade->TradeItem[cnt].sItemInfo.PotionCount&0xFFFF;
				TransTradeItemKey.dwItemSum[cnt] |= (lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum)<<16;
				TransTradeItemKey.dwItemSum[cnt] ^= (TransTradeItemKey.dwItemSum[cnt]>>16)^0x5cbc;
			}

		}
		else {
			TransTradeItemKey.dwItemCode[cnt] = 0;
			TransTradeItemKey.dwItemKey[cnt] = 0;
			TransTradeItemKey.dwItemSum[cnt] = 0;
		}
	}

	cTrade.OpenFlag = TradeOpenFlag;

	if ( !CancelFlag ) {

/*
		TRANS_THROW_ITEMS	TransThrowItems;
		
		TransThrowItems.ThrowItemCount = 0;

		for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
			if ( lpTrade->TradeItem[cnt].Flag ) {
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwCode   = lpTrade->TradeItem[cnt].sItemInfo.CODE;
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwHead   = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head;
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwChkSum = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
				TransThrowItems.ThrowItemCount++;
				//SendSaveThrowItem( &lpTrade->TradeItem[cnt].sItemInfo );
			}
		}
		//����Ÿ ������ ������ ���� �䱸 �ִ� 32�� ���� ( ���� ������ ���� )
		SendSaveThrowItems( &TransThrowItems );

		if ( smWsockServer )
			smWsockServer->Send2( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;

		if ( smWsockUserServer && smWsockServer!=smWsockUserServer )
			smWsockUserServer->Send2( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;
*/
		if ( smWsockDataServer )
			smWsockDataServer->Send2( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;


		//������ ��ȯŰ
		memcpy( &LastTransTradeItemKey , &TransTradeItemKey , sizeof(TRANS_TRADE_ITEMKEY) );
		dwLastTransTradeKeyTime = dwPlayTime;
	}

	dwTradeMaskTime = dwPlayTime+3000;

	return TRUE;
}


//������ ��ȯ ����Ű ����
int RecvTradeSucessKey( TRANS_TRADE_ITEMKEY *lpTransTradeItemKey , sTRADE *lpTrade )
{
	int cnt;
	sTRADE sChkTrade;
	DWORD	dwChkSum1,dwChkSum2;
	int	ItemFlag;

	dwTradeMaskTime = dwPlayTime+3000;

	if ( TransLastRecvTrade.code ) {
		//������ ���� ����Ÿ�� ���� ����Ÿ �ٽ� �ص� ( Z/NZ ��� )
		DecodeCompress( TransLastRecvTrade.TradeBuff , (BYTE *)&sChkTrade );
		for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
			memcpy( &lpTrade->TradeItem[cnt].sItemInfo , &sChkTrade.TradeItem[cnt].sItemInfo , sizeof(sITEMINFO) );
		}
	}
	//cTrade.LoadTradeItemIamge();

	for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
		if ( lpTrade->TradeItem[cnt].Flag ) {

			dwChkSum1 = lpTransTradeItemKey->dwItemSum[cnt];
			dwChkSum2 = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
			if ( (lpTrade->TradeItem[cnt].sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
				//�����ΰ��
				ItemFlag = 1;
				dwChkSum1 = dwChkSum1>>16;
				dwChkSum2 = dwChkSum2&0xFFFF;
			}
			else
				ItemFlag = 0;

			if ( lpTransTradeItemKey->dwItemCode[cnt]==lpTrade->TradeItem[cnt].sItemInfo.CODE && dwChkSum1==dwChkSum2 ) {
//			lpTransTradeItemKey->dwItemSum[cnt]==lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {

				lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head = lpTransTradeItemKey->dwItemKey[cnt];

				//�߸��� ������ ����
				if ( !lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head )
					lpTrade->TradeItem[cnt].Flag = 0;
				else {
					if ( ItemFlag==1 ) {
						//���� ������
						ReformItem( &lpTrade->TradeItem[cnt].sItemInfo );
					}
				}
			}
			else {
				return FALSE;
			}
		}
		else {
			if ( lpTransTradeItemKey->dwItemCode[cnt] ) 
				return FALSE;

		}
	}

	//SendRecordTradeItems( lpTrade );			//��ȯ ���� ������ ���

	return TRUE;
}



//������ ��ȯ ���� Ȯ��
int SendTradeCheckItem( DWORD dwSender )
{

	TRANS_TRADE_CHECKITEM	TransTradeCheckItem;
	int cnt;

	if ( DisconnectFlag ) 
		return FALSE;

	TransTradeCheckItem.code = smTRANSCODE_TRADE_READYITEM;
	TransTradeCheckItem.size = sizeof(TRANS_TRADE_CHECKITEM);
	TransTradeCheckItem.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeCheckItem.dwRecver = dwSender;

	if ( sTrade.Money ) {
		SendSaveMoney();			//�� ���� ( ������� )
	}

	for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
		if ( sTrade.TradeItem[cnt].Flag ) {
			TransTradeCheckItem.dwSendItemCode[cnt] = sTrade.TradeItem[cnt].sItemInfo.CODE;
			TransTradeCheckItem.dwSendItemSum[cnt] = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;

			//SendSaveThrowItem( &sTrade.TradeItem[cnt].sItemInfo );		//������ ���� ����

			//������ ���� Ȯ��
			if ( CheckItemForm( &sTrade.TradeItem[cnt].sItemInfo )==FALSE ) 
				return FALSE;		//�߸��� ������ 

		}
		else {
			TransTradeCheckItem.dwSendItemCode[cnt] = 0;
			TransTradeCheckItem.dwSendItemSum[cnt] = 0;
		}
	}


	for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
		if ( sTradeRecv.TradeItem[cnt].Flag ) {
			TransTradeCheckItem.dwRecvItemCode[cnt] = sTradeRecv.TradeItem[cnt].sItemInfo.CODE;
			TransTradeCheckItem.dwRecvItemSum[cnt] = sTradeRecv.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
		}
		else {
			TransTradeCheckItem.dwRecvItemCode[cnt] = 0;
			TransTradeCheckItem.dwRecvItemSum[cnt] = 0;
		}
	}
/*
	TRANS_THROW_ITEMS	TransThrowItems;
	TransThrowItems.ThrowItemCount = 0;
	for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
		if ( sTrade.TradeItem[cnt].Flag ) {
			TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwCode   = sTrade.TradeItem[cnt].sItemInfo.CODE;
			TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwHead   = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.Head;
			TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwChkSum = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
			TransThrowItems.ThrowItemCount++;
		}
	}
	//����Ÿ ������ ������ ���� �䱸 �ִ� 32�� ���� ( ���� ������ ���� )
	SendSaveThrowItems( &TransThrowItems );
*/

	if ( smWsockServer )
		smWsockServer->Send2( (char *)&TransTradeCheckItem , TransTradeCheckItem.size , TRUE ) ;

	if ( smWsockUserServer && smWsockServer!=smWsockUserServer )
		smWsockUserServer->Send2( (char *)&TransTradeCheckItem , TransTradeCheckItem.size , TRUE ) ;

	dwTradeMaskTime = dwPlayTime+3000;

	return TRUE;
}

//������ ��ȯ ���� Ȯ�� ����
int RecvTradeCheckItem( TRANS_TRADE_CHECKITEM *lpTransTradeCheckItem )
{
	int cnt;

	dwTradeMaskTime = dwPlayTime+3000;

	if ( DisconnectFlag ) 
		return FALSE;

	for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
		if ( sTrade.TradeItem[cnt].Flag ) { 
			if ( lpTransTradeCheckItem->dwRecvItemCode[cnt]!=sTrade.TradeItem[cnt].sItemInfo.CODE ||
			( (lpTransTradeCheckItem->dwRecvItemCode[cnt]&sinITEM_MASK1)!=(sinPM1&sinITEM_MASK1) &&			
			lpTransTradeCheckItem->dwRecvItemSum[cnt]!=sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum) ) {

				return FALSE;

			}
		}
		else {
			if ( lpTransTradeCheckItem->dwRecvItemCode[cnt] ) 
				return FALSE;
		}

		if ( sTradeRecv.TradeItem[cnt].Flag ) { 
			if ( lpTransTradeCheckItem->dwSendItemCode[cnt]!=sTradeRecv.TradeItem[cnt].sItemInfo.CODE ||
			( (lpTransTradeCheckItem->dwSendItemCode[cnt]&sinITEM_MASK1)!=(sinPM1&sinITEM_MASK1) &&			
			lpTransTradeCheckItem->dwSendItemSum[cnt]!=sTradeRecv.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum ) ) {

				return FALSE;

			}
		}
		else {
			if ( lpTransTradeCheckItem->dwSendItemCode[cnt] ) 
				return FALSE;
		}
	}

	return TRUE;
}







//��Ƽ ��� 
int SendJoinPartyUser( DWORD dwObjectCode )
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_PARTY_JOIN;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectCode;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	return smWsockUserServer->Send( (char *)&TransCommand , TransCommand.size , TRUE );
}

//��Ƽ �������̽� ��ư�Է� 
int	PartyButton( int Num , int Button )
{
	// Num ��Ƽ�� ��ȣ
	// Button  0-���� 1-�ػ� 2-�߹� 3-Ż��
	DWORD	dwCode;
	smTRANS_COMMAND	smTransCommand;

	if ( !InterfaceParty.PartyMemberCount ) return FALSE;
	
	dwCode = 0;

	if ( Num>=0 ) 
		dwCode = InterfaceParty.PartyMember[Num].ChrCode;
	else 
		dwCode = 0;

	smTransCommand.size = sizeof( smTRANS_COMMAND );
	smTransCommand.code = smTRANSCODE_PARTY_COMMAND;
	smTransCommand.WParam = dwCode;
	smTransCommand.LParam = Button;
	smTransCommand.SParam = 0;

	if ( Button<3 && InterfaceParty.PartyMember[0].ChrCode!=lpCurPlayer->dwObjectSerial ) {
		//��Ƽ�� ����
		return FALSE;
	}

	return smWsockUserServer->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );
}





////////////////////////// ���� ����ġ �̵� ��� ///////////////

int	SetTotalSubMoney( int Money )
{
	TotalSubMoney += Money;

	return TRUE;
}

int	SetTotalAddMoney( int Money )
{
	TotalAddMoney += Money;

	return TRUE;
}

int	SetTotalAddExp( int Exp )
{
	TotalAddExp += Exp;

	return TRUE;
}

int	SetTotalSubExp( int Exp )
{
	TotalSubExp += Exp;

	return TRUE;
}

int	GetTotalMoney()
{
	int m;

	m = sServerMoney[0].InputMoney+sServerMoney[1].InputMoney+sServerMoney[2].InputMoney+WareHouseSubMoney+TotalAddMoney-TotalSubMoney;

	return m;
}

int GetTotalSubExp()
{
	int m;

	m = (sServerExp[0].InputExp+sServerExp[1].InputExp+sServerExp[2].InputExp)-(TotalAddExp-TotalSubExp);

	return m;
}

int GetTotalExp()
{
	int m;

	m = sServerExp[0].InputExp+sServerExp[1].InputExp+sServerExp[2].InputExp+TotalAddExp-TotalSubExp;

	return m;
}

//â�� ����
int	SaveWareHouse( sWAREHOUSE *lpWareHouse , TRANS_WAREHOUSE *lpTransWareHouse )
{
	TRANS_WAREHOUSE	TransWareHouse;
	//TRANS_THROW_ITEMS	TransThrowItems;
	sWAREHOUSE	WareHouseCheck;
	int	CompSize;
	int cnt;
	DWORD	dwChkSum;
	char	*szComp1, *szComp2;
	int	flag;

	if ( lpTransWareHouse ) flag = 1;
	else flag = 0;

	if ( !flag && QuitSave )	return FALSE;

	for( cnt=0;cnt<100;cnt++) {
		if ( !lpWareHouse->WareHouseItem[cnt].Flag ) {
			ZeroMemory( &lpWareHouse->WareHouseItem[cnt] , sizeof(sITEM ) );
		}
	}


	CompSize = EecodeCompress( (BYTE *)lpWareHouse , (BYTE *)TransWareHouse.Data , sizeof(sWAREHOUSE) , sizeof( sITEM )*100 );


	//�뷮 ýũ
	if ( !flag && CompSize>(smSOCKBUFF_SIZE-256) ) 
		return FALSE;

	if ( flag && CompSize>(smSOCKBUFF_SIZE-140) ) 
		return FALSE;


	DecodeCompress( (BYTE *)TransWareHouse.Data , (BYTE *)&WareHouseCheck , sizeof(sWAREHOUSE) );

	szComp1 = (char *)lpWareHouse;
	szComp2 = (char *)&WareHouseCheck;

	dwChkSum = 0;

	for( cnt=0;cnt<sizeof(sWAREHOUSE);cnt++ ) {
		if ( szComp1[cnt]!=szComp2[cnt] ) {
			return FALSE;
		}
		else {
			dwChkSum += szComp1[cnt]*(cnt+1);
		}
	}


	//â�� ���������� ���������� ���� ���� �˻��ϱ� ���� ����
	WareHouseSubMoney += (CompWareHouseMoney-lpWareHouse->Money);
	CompWareHouseMoney = 0;
/*
	if ( !flag ) {
		//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
		SendSaveMoney();				//��

		TransThrowItems.ThrowItemCount = 0;
		for( cnt=0;cnt<THROW_ITEMS_MAX;cnt++) {
			if ( lpWareHouse->WareHouseItem[cnt].Flag ) {
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwCode   = lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE;
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwHead   = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwChkSum = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;
				TransThrowItems.ThrowItemCount++;
			}
		}
		//����Ÿ ������ ������ ���� �䱸 �ִ� 32�� ���� ( ���� ������ ���� )
		SendSaveThrowItems( &TransThrowItems );
	}
*/

	TransWareHouse.code = smTRANSCODE_WAREHOUSE;
	TransWareHouse.size = sizeof(TRANS_WAREHOUSE)-(sizeof(sWAREHOUSE)-CompSize);
	TransWareHouse.DataSize = CompSize;
	TransWareHouse.dwChkSum = dwChkSum;
	TransWareHouse.wVersion[0] = Version_WareHouse;
	TransWareHouse.wVersion[1] = 0;

	if ( !flag ) {
		//�� ���� ( �������� ����� ���� / �ҷ��ö��� �������� �ʱ�ȭ ����� �� )
		TransWareHouse.WareHouseMoney = lpWareHouse->Money ^ (dwChkSum^smTRANSCODE_WAREHOUSE);
		TransWareHouse.UserMoney = lpCurPlayer->smCharInfo.Money ^ (dwChkSum^smTRANSCODE_WAREHOUSE);
	}

	TransWareHouse.dwTemp[0] = 0;
	TransWareHouse.dwTemp[1] = 0;
	TransWareHouse.dwTemp[2] = 0;
	TransWareHouse.dwTemp[3] = 0;
	TransWareHouse.dwTemp[4] = 0;

	dwLastWareHouseChkSum = dwChkSum;				//���� ���� â�� �ڵ�

	if ( flag ) {
		memcpy( lpTransWareHouse , &TransWareHouse , sizeof(TRANS_WAREHOUSE) );
		return TRUE;
	}

	if ( TransWareHouse.size>smSOCKBUFF_SIZE ) TransWareHouse.size = smSOCKBUFF_SIZE;

	if ( smWsockDataServer ) {
		smWsockDataServer->Send2( (char *)&TransWareHouse , TransWareHouse.size , TRUE );

		SaveGameData();						//������ ����
		return TRUE;
	}

	return FALSE;
}
//â�� ����
int	SaveWareHouse( sWAREHOUSE *lpWareHouse  )
{
	return SaveWareHouse( lpWareHouse , 0 );
}

//â�� ��������
int	LoadWareHouse( TRANS_WAREHOUSE *lpTransWareHouse , sWAREHOUSE *lpWareHouse , int flag )
{
	sWAREHOUSE	WareHouseCheck;
	int cnt;
	DWORD	dwChkSum;
	char	*szComp;
	char	szMsgBuff[128];

	if ( lpTransWareHouse->DataSize==0 ) {
		//�ʱⰪ �ƹ��͵� ����
		ZeroMemory( lpWareHouse , sizeof(sWAREHOUSE) );
		lpWareHouse->Money = 2023;
		lpWareHouse->Weight[0] = 197;
		CompWareHouseMoney = lpWareHouse->Money;	//â�� ���������� ���������� ���� ���� �˻��ϱ� ���� ����
		return TRUE;
	}

	DecodeCompress( (BYTE *)lpTransWareHouse->Data , (BYTE *)&WareHouseCheck );

	dwChkSum = 0;

	szComp = (char *)&WareHouseCheck;

	for( cnt=0;cnt<sizeof(sWAREHOUSE);cnt++ ) {
		dwChkSum += szComp[cnt]*(cnt+1);
	}
	if ( dwChkSum==lpTransWareHouse->dwChkSum ) {

		if ( !flag && dwLastWareHouseChkSum && dwLastWareHouseChkSum!=lpTransWareHouse->dwChkSum && !smConfig.DebugMode ) {
			//������ ������ â�� ����Ÿ�� �ű� �ε� ����Ÿ�� �ڵ尡 Ʋ��
			return FALSE;
		}

		memcpy( lpWareHouse , &WareHouseCheck , sizeof(sWAREHOUSE) );

		CompWareHouseMoney = lpWareHouse->Money;	//â�� ���������� ���������� ���� ���� �˻��ϱ� ���� ����

		for( cnt=0;cnt<100;cnt++) {
			if ( lpWareHouse->WareHouseItem[cnt].Flag ) {

				//2�� ���� ��
				if ( !lpWareHouse->WareHouseItem[cnt].sItemInfo.Temp0 )
					lpWareHouse->WareHouseItem[cnt].sItemInfo.Temp0 = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum+lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE;

				//CheckCopyItem( &lpWareHouse->WareHouseItem[cnt].sItemInfo );		//���� �Ǵ� �ҷ������� �˻�

				if ( !lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwTime ) {
					//�ڵ� ����
					lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwTime = 
						(lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.Head>>2)^(lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum<<2);
				}

				//���� Ȯ��
				if ( (lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE&sinITEM_MASK2) == sinSP1){
					//���� ( ����ī���Ͱ� ������ �����ִ� ���� )
					if ( lpWareHouse->WareHouseItem[cnt].sItemInfo.PotionCount>1 )	lpWareHouse->WareHouseItem[cnt].Flag =0;

					// �庰 - SP1 ������ â���� ������� ����
					/*
					//�����Ⱓ Ȯ��
					if ( lpWareHouse->WareHouseItem[cnt].sItemInfo.dwCreateTime<(sinItemTime-(60*60*24*12)) || 
						lpWareHouse->WareHouseItem[cnt].sItemInfo.dwCreateTime>(sinItemTime+(60*60*24*7)) ) {
							lpWareHouse->WareHouseItem[cnt].Flag =0;
						}
						*/
				}

				if ( DeleteEventItem_TimeOut( &lpWareHouse->WareHouseItem[cnt].sItemInfo )==TRUE ) {
					//�̺�Ʈ ������ ��¥���� ����
					lpWareHouse->WareHouseItem[cnt].Flag = 0;
					wsprintf( szMsgBuff , mgItemTimeOut , lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemName );
					AddChatBuff( szMsgBuff , 0 );
				}
			}
		}
		return TRUE;
	}

	return FALSE;
}

//â�� ��������
int	LoadWareHouse( TRANS_WAREHOUSE *lpTransWareHouse , sWAREHOUSE *lpWareHouse )
{
	return LoadWareHouse( lpTransWareHouse , lpWareHouse , 0 );
}

//�̺�Ʈ ������ ����
int SendCollectMoney( int Money )
{
/*
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_COLLECTMONEY;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = Money;							//����� ��
	TransCommand.LParam = lpCurPlayer->smCharInfo.Money;		//������ ��
	TransCommand.SParam = 0;

	AddInvenMoney( -Money );				//�κ� �� Ȯ��

	if ( smWsockServer ) {
		return smWsockServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
	}
*/

	sITEM	sItemMoney;
	int j;


	memset(&sItemMoney,0,sizeof(sITEM));
	sItemMoney.sItemInfo.CODE = sinGG1|sin01;
	for(j=0 ; j < MAX_ITEM ; j++){
		if(sItem[j].CODE == sItemMoney.sItemInfo.CODE){ //�����̸� 
			memcpy(&sItemMoney,&sItem[j],sizeof(sITEM));
			sItemMoney.sItemInfo.Money = Money;			//�ݾ��� �Ѱ��ش� 
			sItemMoney.sItemInfo.ItemKindCode = smTRANSCODE_COLLECTMONEY;
			ReformItem( &sItemMoney.sItemInfo );		//���� 
			if(sinThrowItemToFeild(&sItemMoney)){
				AddInvenMoney( -Money );				//�κ� �� Ȯ��
				return TRUE;
			}
		}
	}

	return FALSE;
}

//PK���ؼ� ����
int PlayerKilling( DWORD dwObjectSerial )
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code =	smTRANSCODE_PLAYERKILLING;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;				//���� �ڵ�
	TransCommand.LParam = lpCurPlayer->dwObjectSerial;		//�� �ڵ�
	TransCommand.SParam = 0;

	if ( smWsockUserServer ) {
		return smWsockUserServer->Send2( (char *)&TransCommand , TransCommand.size , TRUE );
	}
	return FALSE;
}



//�ͽ��� ����Ÿ�� ������ ����
int SendCraftItemToServer( void *lpsCraftItem_Send )
{

	sCRAFTITEM_SERVER	*lpCraftItem_Send = (sCRAFTITEM_SERVER *)lpsCraftItem_Send;

	lpCraftItem_Send->size = sizeof(sCRAFTITEM_SERVER);
	lpCraftItem_Send->code = smTRANSCODE_CRAFTITEM;

	if ( smWsockDataServer ) {
		return smWsockDataServer->Send2( (char *)lpCraftItem_Send , lpCraftItem_Send->size , TRUE );
	}

	return FALSE;
}

//������ �� ������ ����Ÿ�� ������ ����
int SendWingItemToServer( void *lpsCraftItem_Send )
{

	sCRAFTITEM_SERVER	*lpCraftItem_Send = (sCRAFTITEM_SERVER *)lpsCraftItem_Send;

	lpCraftItem_Send->size = sizeof(sCRAFTITEM_SERVER);
	lpCraftItem_Send->code = smTRANSCODE_WINGITEM;

	if ( smWsockDataServer ) {
		return smWsockDataServer->Send2( (char *)lpCraftItem_Send , lpCraftItem_Send->size , TRUE );
	}

	return FALSE;
}


//����¡ ����Ÿ�� ������ ����
int SendAgingItemToServer( void *lpsCraftItem_Send )
{

	sCRAFTITEM_SERVER	*lpCraftItem_Send = (sCRAFTITEM_SERVER *)lpsCraftItem_Send;

	lpCraftItem_Send->size = sizeof(sCRAFTITEM_SERVER);
	lpCraftItem_Send->code = smTRANSCODE_AGINGITEM;

	if ( smWsockDataServer ) {
		return smWsockDataServer->Send2( (char *)lpCraftItem_Send , lpCraftItem_Send->size , TRUE );
	}

	return FALSE;
}

//��ũ�ھ� ��� ����
int SendLinkCoreToServer( sITEMINFO *lpItem )
{
	TRANS_ITEMINFO	TransItemInfo;
	int len;

	TransItemInfo.size = sizeof(TRANS_ITEMINFO);
	TransItemInfo.code = smTRANSCODE_MAKE_LINKCORE;
	memcpy( &TransItemInfo.Item , lpItem , sizeof( sITEMINFO ) );

	TransItemInfo.x = lpCurPlayer->pX;
	TransItemInfo.y = lpCurPlayer->pY;
	TransItemInfo.z = lpCurPlayer->pZ;

	TransItemInfo.dwSeCode[0] = 0;
	TransItemInfo.dwSeCode[1] = 0;
	TransItemInfo.dwSeCode[2] = 0;
	TransItemInfo.dwSeCode[3] = 0;

	len = lstrlen(lpItem->ItemName);
	if ( lpItem->ItemName[len+1] ) {
		TransItemInfo.code = smTRANSCODE_USE_LINKCORE;

		if ( lpCurPlayer->OnStageField>=0 && StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_ROOM ) {
			//�濡���� ��ȯ������ ��� �Ұ�
			return FALSE;
		}

		if ( smWsockServer )
			return smWsockServer->Send2( (char *)&TransItemInfo , TransItemInfo.size , TRUE );
	}

	if ( smWsockDataServer ) {
		return smWsockDataServer->Send2( (char *)&TransItemInfo , TransItemInfo.size , TRUE );
	}

	return FALSE;
}

int SucessLinkCore( smTRANS_COMMAND_EX *lpTransCommandEx )
{
	int x,z,cnt;

	sITEMINFO	*lpItem;

	//�κ��丮�� ������ �������� ã�� �˻��Ͽ� ã�´� ( ��ũ �ھ� ���� )
	lpItem = FindInvenItem( lpTransCommandEx->WxParam , lpTransCommandEx->LxParam , lpTransCommandEx->SxParam );

	if ( lpItem ) {
		SendUseItemCodeToServer( lpItem );
		cInvenTory.DeleteInvenItemToServer(lpTransCommandEx->WxParam , lpTransCommandEx->LxParam , lpTransCommandEx->SxParam);
		ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ

		if ( FindInvenItem( lpTransCommandEx->WxParam , lpTransCommandEx->LxParam , lpTransCommandEx->SxParam )!=0 ) return FALSE;

	}
	else
		return FALSE;


    CloseEachPlayer();

	WarpFieldNearPos( lpTransCommandEx->EParam  , lpTransCommandEx->WParam, lpTransCommandEx->SParam , &x, &z );

	lpCurPlayer->SetPosi(lpTransCommandEx->WParam, lpTransCommandEx->LParam, lpTransCommandEx->SParam  ,0,0,0 );
	TraceCameraPosi.x = lpCurPlayer->pX;
	TraceCameraPosi.y = lpCurPlayer->pY;
	TraceCameraPosi.z = lpCurPlayer->pZ;
	TraceTargetPosi.x = lpCurPlayer->pX;
	TraceTargetPosi.y = lpCurPlayer->pY;
	TraceTargetPosi.z = lpCurPlayer->pZ;
	
	//�ٴ� Ȯ��
	lpCurPlayer->OnStageField = -1;
	if ( smGameStage[0] ) {
		cnt = smGameStage[0]->GetFloorHeight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ ,lpCurPlayer->Pattern->SizeHeight );	
		lpCurPlayer->OnStageField = 0;
	}
	if ( cnt==CLIP_OUT && smGameStage[1] ) {
		lpCurPlayer->OnStageField = 1;
	}

	StartEffect( lpCurPlayer->pX,lpCurPlayer->pY+48*fONE,lpCurPlayer->pZ, EFFECT_RETURN1 );
	SkillPlaySound( SKILL_SOUND_LEARN , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );			//��ų ȿ����
	RestartPlayCount = 350;		//5�� ���� ����

	return TRUE;
}


//����¡ �Ϸ�� �������� ������ ����
int SendAgingUpgradeItemToServer( sITEMINFO *lpItem )
{
	TRANS_ITEMINFO	TransItemInfo;

	if ( TransAgingItemFlag ) return FALSE;

	TransItemInfo.code = smTRANSCODE_AGING_UPGRADE;
	TransItemInfo.size = sizeof(TRANS_ITEMINFO);
	TransItemInfo.x = 0;
	TransItemInfo.y = 0;
	TransItemInfo.z = 0;
	memcpy( &TransItemInfo.Item , lpItem , sizeof( sITEMINFO ) );

	if ( smWsockDataServer ) 
	{
		if ( smWsockDataServer->Send2( (char *)&TransItemInfo , TransItemInfo.size , TRUE )==TRUE )
		{
			TransAgingItemFlag = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

/*
//������ Ȯ��
#define smTRANSCODE_CHECKITEM			0x50320300
//������ ���� Ȯ��
#define smTRANSCODE_ERRORITEM			0x50320310

struct TRANS_ITEMINFO {
	int	size,code;

	//char	szItemName[32];
	//DWORD	ItemCode;

	sITEMINFO	Item;

	int x,y,z;

};

struct	TRANS_ITEM_CODE {
	int size,code;

	DWORD	dwFlag;

	DWORD	dwItemCode;
	DWORD	dwHead;
	DWORD	dwChkSum;
	DWORD	dwTime;
}


*/


//�κ��丮�� ���హ���� ���� 1�� ����
int ClearInvenPotionCount()
{
	int cnt;

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if( cInvenTory.InvenItem[cnt].Flag && (cInvenTory.InvenItem[cnt].sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ){
			cInvenTory.InvenItem[cnt].sItemInfo.PotionCount = 1;
		}
	}

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if( cInvenTory.InvenItemTemp[cnt].Flag && (cInvenTory.InvenItemTemp[cnt].sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ){
			cInvenTory.InvenItemTemp[cnt].sItemInfo.PotionCount = 1;
		}
	}

	if ( cTrade.OpenFlag ) {
		for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
			if ( sTrade.TradeItem[cnt].Flag && (sTrade.TradeItem[cnt].sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
				sTrade.TradeItem[cnt].sItemInfo.PotionCount = 1;
			}
		}
	}

	if ( MouseItem.Flag && (MouseItem.sItemInfo.CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) )
		MouseItem.sItemInfo.PotionCount = 1;

	cInvenTory.ReFormPotionNum();

	SaveGameData();

	return TRUE;
}

//�κ��丮�� ������ ã�� (�ڵ�)
sITEMINFO *FindItemFromCode( DWORD dwItemCode )
{
	int cnt;

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if( cInvenTory.InvenItem[cnt].Flag && cInvenTory.InvenItem[cnt].sItemInfo.CODE==dwItemCode ){
			return &cInvenTory.InvenItem[cnt].sItemInfo;
		}
	}
	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if( cInvenTory.InvenItemTemp[cnt].Flag && cInvenTory.InvenItemTemp[cnt].sItemInfo.CODE==dwItemCode ){
			return &cInvenTory.InvenItemTemp[cnt].sItemInfo;
		}
	}

	return NULL;
}



DWORD	dwLastCheckItemTime = 0;

//�������� ������ ������ Ȯ��
int	SendCheckItemToServer( sITEMINFO *lpItem )
{

	TRANS_ITEMINFO	TransItemInfo;

	//return FALSE;

	if ( (dwLastCheckItemTime+10000)>dwPlayTime ) {
		//������ 10�� �̸�
		return FALSE;
	}
	//if ( !lpItem->ItemName[0] ) return FALSE;

	TransItemInfo.code = smTRANSCODE_CHECKITEM;
	TransItemInfo.size = sizeof(TRANS_ITEMINFO);

	memcpy( &TransItemInfo.Item , lpItem , sizeof(sITEMINFO) );

	TransItemInfo.x = 0;
	TransItemInfo.y = 0;
	TransItemInfo.z = 0;

	if ( smWsockUserServer ) {
		smWsockUserServer->Send2( (char *)&TransItemInfo , TransItemInfo.size , TRUE );
	}
	dwLastCheckItemTime = dwPlayTime;

	return TRUE;
}

//������ Ȯ�� ��� ����
int RecvCheckItemFromServer( TRANS_ITEM_CODE *lpTransItemCode )
{

	switch( lpTransItemCode->code ) {
		case smTRANSCODE_CHECKITEM:
			//������ ���� Ȯ�� �ڵ� ����
			//cInvenTory.ResetInvenItemToServer( lpTransItemCode->dwItemCode , lpTransItemCode->dwHead ,
			//	lpTransItemCode->dwChkSum , lpTransItemCode->dwTime );
			break;

		case smTRANSCODE_ERRORITEM:
			//������ ����Ȯ�� �ڵ� ���� ( ������ ���� )
			cInvenTory.DeleteInvenItemToServer(lpTransItemCode->dwItemCode , lpTransItemCode->dwHead , lpTransItemCode->dwChkSum );

			//�κ��丮�� ������ �������� ã�� �˻��Ͽ� ã�´�
			if ( FindInvenItem( lpTransItemCode->dwItemCode , lpTransItemCode->dwHead , lpTransItemCode->dwChkSum )==0 ) {

				SendInvenItemError( 0 , lpTransItemCode->dwItemCode , lpTransItemCode->dwHead , lpTransItemCode->dwChkSum );
				ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
			}

			break;

		case smTRANSCODE_CLEARPOTION:
			//�κ��丮�� ���హ���� ���� 1�� ����
			ClearInvenPotionCount();
			break;
	}

	return TRUE;
}

//�Ǹ��� �������� ������ ����
int	SellItemToServer( sITEMINFO *lpItem , int Count )
{

	TRANS_ITEMINFO	TransItemInfo;

	TransItemInfo.code = smTRANSCODE_SHOP_SELLITEM;
	TransItemInfo.size = sizeof(TRANS_ITEMINFO);

	memcpy( &TransItemInfo.Item , lpItem , sizeof(sITEMINFO) );

#ifdef _TEST_SERVER
	//ġƮ �ù����̼�
	if ( smConfig.DebugMode && VRKeyBuff[VK_CONTROL] && VRKeyBuff['N'] && VRKeyBuff['B'] ) {
		//TransItemInfo.Item.Price  = 100000000;
		lpItem->Durability[0] = lpItem->Durability[1]*100;
	}
#endif

	TransItemInfo.x = Count;
	TransItemInfo.y = lpCurPlayer->smCharInfo.Money;
	TransItemInfo.z = 0;

	if ( smWsockDataServer ) {
		return smWsockDataServer->Send2( (char *)&TransItemInfo , TransItemInfo.size , TRUE );
	}

	return FALSE;
}





int Chk_InventPosCount  = 1;
DWORD	dwCheckInvenItemTime = 0;

//�κ��丮�� ������ �������� ������ ������ �˻�
int CheckInvenItemToServer()
{
	int cnt;

	if ( dwCheckInvenItemTime>dwPlayTime ) return TRUE;

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if(cInvenTory.InvenItem[cnt].ItemPosition==Chk_InventPosCount ){
			//�������� ������ ������ Ȯ��
			SendCheckItemToServer( &cInvenTory.InvenItem[cnt].sItemInfo );
		}
	}

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if(cInvenTory.InvenItemTemp[cnt].ItemPosition==Chk_InventPosCount ){
			//�������� ������ ������ Ȯ��
			SendCheckItemToServer( &cInvenTory.InvenItemTemp[cnt].sItemInfo );
		}
	}

	Chk_InventPosCount++;
	dwCheckInvenItemTime = dwPlayTime+55*1000;

	if ( Chk_InventPosCount>=11 || Chk_InventPosCount<1 ) Chk_InventPosCount = 1;

	return TRUE;
}

////////////////////////////// �޸� Ÿ�� ýũ ������ ///////////////////////////////

HANDLE	hTimeCheckThread = 0;
int		TimeCheckCounter;

DWORD WINAPI TimeCheckThread( void *pInfo )
{
	HANDLE	hThread;
	DWORD	dwTime;
	DWORD	dwLastTime;
	int		cnt;

	hThread = GetCurrentThread();
	dwTime = GetCurrentTime();
	dwLastTime = dwTime;
	TimeCheckCounter = 0;

	while(1) {
		dwTime = GetCurrentTime();
		cnt = dwTime-dwLastTime;
		if ( cnt>PlayTimerMax ) PlayTimerMax = cnt;
		dwLastTime = dwTime;
		TimeCheckCounter++;
		Sleep( 800 );
	}

	ExitThread( TRUE );
	return TRUE;
}


//�ε��� ȭ�� ǥ��
int OpenTimeCheckThread()
{
	DWORD	dwTimeCheckThreadID;

	if ( !hTimeCheckThread ) {
		hTimeCheckThread = CreateThread( NULL , 0, TimeCheckThread , 0 , 0, &dwTimeCheckThreadID );
	}

	return TRUE;
}

int	PlayTimerMax_SendingCount = 0;

int SendPlayTimerMax()
{
	smTRANS_COMMAND	smTransCommand;

	if ( PlayTimerMax>=10000 || (PlayTimerMax_SendingCount&0x7)==0 ) 
	{
		smTransCommand.size = sizeof( smTRANS_COMMAND );
		smTransCommand.code = smTRANSCODE_PROCESS_TIMEMAX;
		smTransCommand.WParam = PlayTimerMax;
		smTransCommand.LParam = TimeCheckCounter;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		if ( smWsockDataServer ) 
			smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

		PlayTimerMax = 0;
	}

	PlayTimerMax_SendingCount ++;

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////




//�������� �޸� ���� ���ؼ� ��������
DWORD funcCheckMemSum( DWORD FuncPoint , int count )
{
	int cnt;
	DWORD dwSum;
	int	xcnt;
	DWORD *lpGetPrtectPoint;


	lpGetPrtectPoint = (DWORD *)FuncPoint;

	dwSum = 0;
	xcnt = 1;

	for( cnt=0;cnt<count;cnt++ ) {
		xcnt += cnt;
		dwSum += lpGetPrtectPoint[cnt]*xcnt;
	}

	return dwSum;
}

/*
		dwResult = fnChkMem( TransFuncMem->Param[0] , TransFuncMem->Param[1] );

		smTransCommand.code = smTRANSCODE_FUNCTON_RESULT;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = TransFuncMem->Param[0];
		smTransCommand.LParam = TransFuncMem->Param[1];
		smTransCommand.SParam = TransFuncMem->Param[2];
		smTransCommand.EParam = dwResult;

*/
//�������� �޸� ���� ���ؼ� ��������
//DWORD funcCheckMemSum2( DWORD FuncPoint , int count )

///////////////////////////////////////////////////////////////////////////////

DWORD	dwFuncList[][2] = {
	{ (DWORD)funcCheckMemSum<<1				,	512		},
	{ (DWORD)Check_PlaySubReleaseFunc<<1	,	512		},
	{ (DWORD)PlayPat3D<<1					,	2048	},
	{ (DWORD)Check_CodeSafe<<1				,	1024	},
	{ (DWORD)AddExp<<1						,	512		},
	{ (DWORD)sinSetLife<<1					,	512		},
	{ (DWORD)NetWorkPlay<<1					,	2800	},

	{ (DWORD)E_Shield_BlockRate<<1			,	((Meteo_UseMana-E_Shield_BlockRate)+10) },		//��ų ���̺� ����

	{ (DWORD)Code_VRamBuffOpen<<1			,	3600	},			//������ ���� ���� ýũ �Ѵ�
	{ 0,0 }
};



//Ŭ���̾�Ʈ ��� ��ġ�� ���� ������ ����
int	SendClientFuncPos()
{

	TRANS_CLIENT_FUNPOS	TransClientFuncPos;
	int cnt;


	ZeroMemory( &TransClientFuncPos , sizeof(TRANS_CLIENT_FUNPOS) );

	TransClientFuncPos.code = smTRANSCODE_CLIENT_FUNCPOS;
	TransClientFuncPos.size = sizeof(TRANS_CLIENT_FUNPOS);
	TransClientFuncPos.ClientVersion = Client_Version;

	cnt = 0;
	TransClientFuncPos.dwFunc_CheckMemSum = dwFuncList[cnt][0]>>1;
	TransClientFuncPos.dwFuncValue[cnt].dwFunc = dwFuncList[cnt][0]>>1;
	TransClientFuncPos.dwFuncValue[cnt].dwLen = dwFuncList[cnt][1];
	TransClientFuncPos.dwFuncValue[cnt].dwChkSum = funcCheckMemSum( TransClientFuncPos.dwFuncValue[cnt].dwFunc , TransClientFuncPos.dwFuncValue[cnt].dwLen );
	//TransClientFuncPos.dwFuncValue[cnt].dwChkSum = fnChkMem( TransClientFuncPos.dwFuncValue[cnt].dwFunc , TransClientFuncPos.dwFuncValue[cnt].dwLen );

	while(1) {

		if ( dwFuncList[cnt][0]==0 ) break;
		if ( cnt>=dwFUNC_VALUE_MAX )  break;

		TransClientFuncPos.dwFuncValue[cnt].dwFunc = dwFuncList[cnt][0]>>1;
		TransClientFuncPos.dwFuncValue[cnt].dwLen = dwFuncList[cnt][1];
		TransClientFuncPos.dwFuncValue[cnt].dwChkSum = funcCheckMemSum( TransClientFuncPos.dwFuncValue[cnt].dwFunc , TransClientFuncPos.dwFuncValue[cnt].dwLen );

		cnt++;
	}

	TransClientFuncPos.FuncCount = cnt;

	//������ ����
	if ( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)&TransClientFuncPos , TransClientFuncPos.size , TRUE );

	return FALSE;
}


//�޸� �˻� ��� ����
int	RecvMemFuncData( TRANS_FUNC_MEMORY *TransFuncMem )
{
	smTRANS_COMMAND	smTransCommand;

	fnChkMem = (LPFN_CheckMem)((void *)TransFuncMem->szData);

	smTransCommand.size = 0;

	fnChkMem( TransFuncMem , &smTransCommand );

	if ( smWsockDataServer && smTransCommand.size>0 ) 
		smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return TRUE;
}


////////////////////////////// ������ ���� ���� ťó�� //////////////////////////

#define	RECV_TRANSITEM_QUE_MAX			256
#define	RECV_TRANSITEM_QUE_MASK			255

TRANS_ITEMINFO	Recv_TransItemInfo_Que[RECV_TRANSITEM_QUE_MAX];

int	RecvTransItemQue_Push = 0;
int	RecvTransItemQue_Pop = 0;

//������ ť�� �ӽ� ����
int	PushRecvTransItemQue( TRANS_ITEMINFO *lpTransItemInfo )
{
	int mCnt;

	mCnt = RecvTransItemQue_Push & RECV_TRANSITEM_QUE_MASK;

	memcpy( &Recv_TransItemInfo_Que[mCnt] , lpTransItemInfo , sizeof(TRANS_ITEMINFO) );
	RecvTransItemQue_Push++;
	RecvTransItemQue_Pop = RecvTransItemQue_Push-RECV_TRANSITEM_QUE_MASK;
	if ( RecvTransItemQue_Pop<0 ) RecvTransItemQue_Pop = 0;

	return TRUE;
}

//�ӽ������ ������ ť���� ã��
TRANS_ITEMINFO	*FindRecvTransItemQue( DWORD dwCode , DWORD dwHead ,DWORD dwChkSum )
{
	int cnt,mCnt;

	for(cnt=RecvTransItemQue_Pop;cnt<RecvTransItemQue_Push;cnt++) {
		mCnt = cnt&RECV_TRANSITEM_QUE_MASK;

		if ( Recv_TransItemInfo_Que[mCnt].Item.CODE==dwCode &&
			Recv_TransItemInfo_Que[mCnt].Item.ItemHeader.Head==dwHead &&
			Recv_TransItemInfo_Que[mCnt].Item.ItemHeader.dwChkSum==dwChkSum ) {

				return &Recv_TransItemInfo_Que[mCnt];
			}
	}

	return NULL;
}

//�ӽ������ ������ ť�� ������ ��������� �˻�
int CheckRecvTrnsItemQue()
{
	int cnt,mCnt;

	for(cnt=RecvTransItemQue_Pop;cnt<RecvTransItemQue_Push;cnt++) {
		mCnt = cnt&RECV_TRANSITEM_QUE_MASK;
		if ( Recv_TransItemInfo_Que[mCnt].Item.CODE ) return FALSE;
	}

	return TRUE;
}

//����� ������ �ڵ� ������ ����
int SendUseItemCodeToServer( sITEMINFO *lpItem )
{
	smTRANS_COMMAND_EX	smTransCommandEx;

	smTransCommandEx.code =	smTRANSCODE_USEITEM_CODE;
	smTransCommandEx.size = sizeof(smTRANS_COMMAND);
	smTransCommandEx.WParam = lpItem->CODE;
	smTransCommandEx.LParam = lpItem->ItemHeader.Head;
	smTransCommandEx.SParam = lpItem->ItemHeader.dwChkSum;
	smTransCommandEx.EParam = lpItem->PotionCount;

	smTransCommandEx.WxParam = 0;
	smTransCommandEx.LxParam = 0;
	smTransCommandEx.SxParam = 0;
	smTransCommandEx.ExParam = 0;

	if ( smWsockDataServer ) {
		return smWsockDataServer->Send2( (char *)&smTransCommandEx , smTransCommandEx.size , TRUE );
	}

	return FALSE;
}

//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ�
int RecvCommandUser( smTRANS_COMMAND_EX *lpTransCommandEx )
{
	smCHAR *lpChar;
	smCHAR *lpChar2;

	switch( lpTransCommandEx->WParam ) {
		case smCOMMNAD_USER_WARP:
			//���� ����Ʈ ǥ��
			StartEffect( lpTransCommandEx->LParam,lpTransCommandEx->SParam,lpTransCommandEx->EParam, EFFECT_RETURN1 );
			SkillPlaySound( SKILL_SOUND_LEARN , lpTransCommandEx->LParam,lpTransCommandEx->SParam,lpTransCommandEx->EParam );			//��ų ȿ����
			break;


		case smCOMMNAD_USER_AGINGUP:
			//�ڵ� �÷��̾ ã�´�
			lpChar = FindChrPlayer( lpTransCommandEx->WxParam );
			if ( lpChar ) {
				StartEffect(lpChar->pX, lpChar->pY, lpChar->pZ, EFFECT_AGING);
				esPlaySound( 7 , GetDistVolume( lpChar->pX, lpChar->pY, lpChar->pZ ) , 1600 );	//���� ����
			}
			break;

		case smCOMMNAD_USER_HOLY_INCANTATION:
			lpChar = FindChrPlayer( lpTransCommandEx->LParam );
			if ( lpChar ) {
				AssaParticle_HolyIncantationPart( lpChar , lpTransCommandEx->SParam ); 
			}
			break;

		case smCOMMNAD_USER_RESURRECTION:
			lpChar = FindChrPlayer( lpTransCommandEx->LParam );
			if ( lpChar ) {
				AssaParticle_ResurrectionPart( lpChar );
			}
			break;

		case smCOMMNAD_USER_FORCEORB:		//���� ���
			//�ڵ� �÷��̾ ã�´�
			lpChar = FindChrPlayer( lpTransCommandEx->WxParam );
			if ( lpChar ) {
				StartSkill( lpChar->pX,lpChar->pY,lpChar->pZ, 0,0,0, SKILL_UP1 );
				SkillPlaySound( SKILL_SOUND_LEARN , lpChar->pX,lpChar->pY,lpChar->pZ );			//��ų ȿ����
			}
			break;

		case smCOMMNAD_USER_VALENTO:
			//�߷��� �ҿＭĿ ����Ʈ
			lpChar = FindChrPlayer( lpTransCommandEx->LParam );
			lpChar2 = FindChrPlayer( lpTransCommandEx->SParam );
			if ( lpChar && lpChar2 ) {
				sinSkillEffect_Soul_Sucker( lpChar , lpChar2 );
			}
			break;

		case smCOMMNAD_USER_FIREFLOWER:
			lpChar = FindChrPlayer( lpTransCommandEx->WxParam );
			if ( lpChar && lpChar!=lpCurPlayer ) {
				DWORD	dwTable = GetTableNum( lpTransCommandEx->LParam );
				AddTimeEffect( SKILL_EFFECT_FIREFLOWER , dwPlayTime+1000 , (dwTable>>24)&0xFF,0,0, lpChar );
				AddTimeEffect( SKILL_EFFECT_FIREFLOWER , dwPlayTime+1500 , (dwTable>>16)&0xFF,0,0, lpChar );
				AddTimeEffect( SKILL_EFFECT_FIREFLOWER , dwPlayTime+2000 , (dwTable>>8)&0xFF,0,0, lpChar );
				AddTimeEffect( SKILL_EFFECT_FIREFLOWER , dwPlayTime+2500 , dwTable&0xFF,0,0, lpChar );
			}
			break;

	}

	return TRUE;
}

//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ� ������
int	SendCommandUser( int wParam , int lParam , int sParam , int eParam )
{
	smTRANS_COMMAND_EX	smTransCommandEx;

	smTransCommandEx.code = smTRANSCODE_COMMAND_USER;
	smTransCommandEx.size = sizeof(smTRANS_COMMAND_EX);
	smTransCommandEx.WParam  = wParam;
	smTransCommandEx.LParam  = lParam;
	smTransCommandEx.SParam  = sParam;
	smTransCommandEx.EParam  = eParam;

	smTransCommandEx.WxParam = lpCurPlayer->dwObjectSerial;
	smTransCommandEx.LxParam = 0;
	smTransCommandEx.SxParam = 0;
	smTransCommandEx.ExParam = 0;

	if ( smWsockUserServer ) {
		return smWsockUserServer->Send( (char *)&smTransCommandEx , smTransCommandEx.size , TRUE );
	}
	return FALSE;
}

//���������� ��� �ڵ� ����
int	SendCommand_AreaServer( DWORD dwCode , int wParam , int lParam , int sParam , int eParam )
{
	smTRANS_COMMAND	smTransCommand;
	smWINSOCK	*lpsmSock;

	smTransCommand.code = dwCode;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam  = wParam;
	smTransCommand.LParam  = lParam;
	smTransCommand.SParam  = sParam;
	smTransCommand.EParam  = eParam;

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) {
		return lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}

	return FALSE;
}

//���������� ��� �ڵ� ����
int	SendCommand_DataServer( DWORD dwCode , int wParam , int lParam , int sParam , int eParam )
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.code = dwCode;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam  = wParam;
	smTransCommand.LParam  = lParam;
	smTransCommand.SParam  = sParam;
	smTransCommand.EParam  = eParam;

	if ( smWsockDataServer ) {
		return smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}

	return FALSE;
}

struct	sDAMAGE_RECORD {
	int	Damage;
	int	Count;
};

sDAMAGE_RECORD	sRecvDamage[3];
sDAMAGE_RECORD	sSendDamage[3];

////////////////////////////////////////////////////////////////////////////
//���ݷ� ��� �ʱ�ȭ
int	Init_RecordDamage()
{
	ZeroMemory( sRecvDamage , sizeof(sDAMAGE_RECORD)*3 );
	ZeroMemory( sSendDamage , sizeof(sDAMAGE_RECORD)*3 );

	return TRUE;
}

//���� ���ݷ� ���
int	Record_RecvDamage( smWINSOCK *lpsmSock , int Damage )
{
	smTRANS_COMMAND	smTransCommand;

	if ( smWsockServer==lpsmSock ) {
		sRecvDamage[0].Damage -= Damage;
		sRecvDamage[0].Count--;

		if ( (sRecvDamage[0].Count&0x1FF)==0 ) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[0].Count;
			smTransCommand.LParam = sRecvDamage[0].Damage;
			smTransCommand.SParam = sSendDamage[0].Count;
			smTransCommand.EParam = sSendDamage[0].Damage;
			if ( lpsmSock ) lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}
	}
	else if ( smWsockUserServer==lpsmSock ) {
		sRecvDamage[1].Damage -= Damage;
		sRecvDamage[1].Count--;

		if ( (sRecvDamage[1].Count&0x1FF)==0 ) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[1].Count;
			smTransCommand.LParam = sRecvDamage[1].Damage;
			smTransCommand.SParam = sSendDamage[1].Count;
			smTransCommand.EParam = sSendDamage[1].Damage;
			if ( lpsmSock ) lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}

	}
	else if ( smWsockExtendServer==lpsmSock ) {
		sRecvDamage[2].Damage -= Damage;
		sRecvDamage[2].Count--;

		if ( (sRecvDamage[2].Count&0x1FF)==0 ) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[2].Count;
			smTransCommand.LParam = sRecvDamage[2].Damage;
			smTransCommand.SParam = sSendDamage[2].Count;
			smTransCommand.EParam = sSendDamage[2].Damage;
			if ( lpsmSock ) lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}
	}

	return TRUE;
}

//���� ���ݷ� ���
int	Record_SendDamage( smWINSOCK *lpsmSock , int Damage )
{
	smTRANS_COMMAND	smTransCommand;

	if ( smWsockServer==lpsmSock ) {
		sSendDamage[0].Damage -= Damage;
		sSendDamage[0].Count--;

		if ( (sRecvDamage[0].Count&0x1FF)==0 ) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[0].Count;
			smTransCommand.LParam = sRecvDamage[0].Damage;
			smTransCommand.SParam = sSendDamage[0].Count;
			smTransCommand.EParam = sSendDamage[0].Damage;
			if ( lpsmSock ) lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}
	}
	else if ( smWsockUserServer==lpsmSock ) {
		sSendDamage[1].Damage -= Damage;
		sSendDamage[1].Count--;

		if ( (sRecvDamage[1].Count&0x1FF)==0 ) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[1].Count;
			smTransCommand.LParam = sRecvDamage[1].Damage;
			smTransCommand.SParam = sSendDamage[1].Count;
			smTransCommand.EParam = sSendDamage[1].Damage;
			if ( lpsmSock ) lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}

	}
	else if ( smWsockExtendServer==lpsmSock ) {
		sSendDamage[2].Damage -= Damage;
		sSendDamage[2].Count--;

		if ( (sRecvDamage[2].Count&0x1FF)==0 ) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[2].Count;
			smTransCommand.LParam = sRecvDamage[2].Damage;
			smTransCommand.SParam = sSendDamage[2].Count;
			smTransCommand.EParam = sSendDamage[2].Damage;
			if ( lpsmSock ) lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}
	}

	return TRUE;
}

//��Ƽ������ ��ų ��� ���� ������
int	SendPartySkillToServer( DWORD dwSkillCode , int SkillLevel , int Around , int wParam , int lParam , int sParam , int eParam )
{
	int x,y,z,dist;
	int cnt;
	int PartyCount;
	TRANS_PARTY_SKILL	TransPartySkill;
	smWINSOCK	*lpsmSock;

	TransPartySkill.code = smTRANSCODE_PARTY_SKILL;
	TransPartySkill.dwSkillCode = dwSkillCode;
	TransPartySkill.wParam = wParam;
	TransPartySkill.lParam = lParam;
	TransPartySkill.sParam = sParam;
	TransPartySkill.eParam = eParam;

	PartyCount = 0;

	TransPartySkill.dwPartyUser[PartyCount++] = lpCurPlayer->dwObjectSerial;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial && chrOtherPlayer[cnt].PartyFlag ) {
			x = ( lpCurPlayer->pX - chrOtherPlayer[cnt].pX )>>FLOATNS;
			y = ( lpCurPlayer->pY - chrOtherPlayer[cnt].pY )>>FLOATNS;
			z = ( lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ )>>FLOATNS;
			dist = x*x+y*y+z*z;
			if ( dist<(Around*Around) ) {
				TransPartySkill.dwPartyUser[PartyCount++] = chrOtherPlayer[cnt].dwObjectSerial;
			}
		}
	}

	TransPartySkill.PartyCount = PartyCount;
	TransPartySkill.Point = SkillLevel;
	TransPartySkill.size = sizeof(TRANS_PARTY_SKILL) - (8-PartyCount)*4;

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) lpsmSock->Send2( (char *)&TransPartySkill , TransPartySkill.size , TRUE );

	return TRUE;
}

//��ų ���
int SendCancelSkillToServer( DWORD dwSkillCode , int lParam , int sParam , int eParam )
{
	smWINSOCK	*lpsmSock;
	smTRANS_COMMAND smTransCommand;

	smTransCommand.code = smTRANSCODE_CANCEL_SKILL;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwSkillCode;
	smTransCommand.LParam = lParam;
	smTransCommand.SParam = sParam;
	smTransCommand.EParam = eParam;

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return TRUE;
}

//��ų ����
int SendUpdateSkillToServer( DWORD dwSkillCode , int lParam , int sParam , int eParam )
{
	smWINSOCK	*lpsmSock;
	smTRANS_COMMAND smTransCommand;

	smTransCommand.code = smTRANSCODE_UPDATEL_SKILL;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwSkillCode;
	smTransCommand.LParam = lParam;
	smTransCommand.SParam = sParam;
	smTransCommand.EParam = eParam;

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return TRUE;
}

//������ ��ų���� ����
int SendProcessSKillToServer( DWORD dwSkillCode , int point , int Param1, int Param2 ) 
{

	smWINSOCK	*lpsmSock;
	smTRANS_COMMAND smTransCommand;

	smTransCommand.code = smTRANSCODE_PROCESS_SKILL2;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwSkillCode|(point<<8);
	smTransCommand.LParam = lpCurPlayer->dwObjectSerial;
	smTransCommand.SParam = Param1;
	smTransCommand.EParam = Param2;

	smTransCommand.LParam =	dm_GetCommandChkSum( &smTransCommand , lpCurPlayer->dwObjectSerial ); //�ڸǵ� ýũ��

	#ifdef _USE_DYNAMIC_ENCODE
		if ( fnEncodeDamagePacket ) 
		{
			fnEncodeDamagePacket(&smTransCommand);									//��Ŷ ��ȣȭ

			lpsmSock = GetAreaServerSock();
			if ( lpsmSock ) 
				lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
		}
	#else
		dm_EncodePacket(&smTransCommand);										//��Ŷ ��ȣȭ

		lpsmSock = GetAreaServerSock();
		if (lpsmSock)
			lpsmSock->Send2((char *)&smTransCommand, smTransCommand.size, TRUE);
	#endif

	return TRUE;


}

//��Ƽ ��ų ��Ŷ ����
int RecvPartySkillFromServer( TRANS_PARTY_SKILL *lpTransPartySkill )
{
	int cnt,cnt2,cnt3;
	int time;
	int flag;
	//smCHAR *lpChar;

	switch( lpTransPartySkill->dwSkillCode ) {
		case SKILL_PLAY_HOLY_VALOR:
			//Ȧ���뷯
			if ( lpTransPartySkill->Point<1 || lpTransPartySkill->Point>10 ) break;

			time = Holy_Valor_Time[lpTransPartySkill->Point-1];
			for( cnt=1;cnt<lpTransPartySkill->PartyCount;cnt++ ) {
				if ( lpCurPlayer->dwObjectSerial==lpTransPartySkill->dwPartyUser[cnt] ) {

					flag = 0;
					//if ( (lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_HOLY_BODY)!=0 ) flag ++;
					for(cnt3=0;cnt3<10;cnt3++) {
						if ( ContinueSkill[cnt].Flag ) {
							if ( ContinueSkill[cnt].CODE==SKILL_DRASTIC_SPIRIT || ContinueSkill[cnt].CODE==SKILL_HOLY_BODY ) {
								flag++;
							}
						}
					}

					if ( flag ) {
						//Ȧ���ٵ� ����� ( ��ų�� ��� ) / ������ƽ ���Ǹ�Ʈ ������϶��� ���
						//- �߰����� - Ȧ�� �뷯�� �̹� ������̸� ����Ʈ���� �ߺ� ǥ�õ��� �ʵ��� ó�� 
						//(���߿� ������ ����Ʈ�� ���� )

						SendCancelSkillToServer( lpTransPartySkill->dwSkillCode , 0,0,0 );
					}
					else {
						//��ų ����Ʈ ����
						AssaParticle_HolyValor_Member( lpCurPlayer , time );
						sSKILL sSkill;

						ZeroMemory( &sSkill , sizeof(sSKILL) );
						sSkill.CODE = SKILL_HOLY_VALOR;
						sSkill.UseTime = time;
						sSkill.Flag = TRUE;
						sSkill.Point = lpTransPartySkill->Point;
						sinContinueSkillSet( &sSkill );
					}
				}
				else {
					for( cnt2=0;cnt2<OTHER_PLAYER_MAX;cnt2++) {
						if ( chrOtherPlayer[cnt2].Flag && 
							chrOtherPlayer[cnt2].dwObjectSerial==lpTransPartySkill->dwPartyUser[cnt] ) {
								AssaParticle_HolyValor_Member( &chrOtherPlayer[cnt2] , time );
							}
					}
				}
			}
			break;
/*
		case SKILL_PLAY_TRIUMPH_OF_VALHALLA:
			//Ʈ���������Ҷ�
			if ( lpTransPartySkill->Point<1 || lpTransPartySkill->Point>10 ) break;

			time = T_Of_Valhalla_Time[lpTransPartySkill->Point-1];

			lpChar = FindChrPlayer( lpTransPartySkill->dwPartyUser[0] );		//������

			if ( lpChar ) {
				if ( lpChar!=lpCurPlayer ) {
					AssaParticle_TriumphOfValhallaMember( lpChar , lpCurPlayer , time );
				}

				for( cnt=1;cnt<lpTransPartySkill->PartyCount;cnt++ ) {
					for( cnt2=0;cnt2<OTHER_PLAYER_MAX;cnt2++) {
						if ( chrOtherPlayer[cnt2].Flag && 
							chrOtherPlayer[cnt2].dwObjectSerial==lpTransPartySkill->dwPartyUser[cnt] ) {

								if ( lpChar!=&chrOtherPlayer[cnt2] ) {
									AssaParticle_TriumphOfValhallaMember( lpChar , &chrOtherPlayer[cnt2] , time );
								}
							}
					}
				}

			}
			break;
*/

		case SKILL_PLAY_EXTINCTION:
			//���� ����Ʈ ���� ( ���Ͳ� )
			for( cnt=0;cnt<lpTransPartySkill->PartyCount;cnt++ ) {
				for( cnt2=0;cnt2<OTHER_PLAYER_MAX;cnt2++) {
					if ( chrOtherPlayer[cnt2].Flag && 
						chrOtherPlayer[cnt2].dwObjectSerial==lpTransPartySkill->dwPartyUser[cnt] ) {
							AssaParticle_Extinction( &chrOtherPlayer[cnt2] );
							SkillPlaySound( SKILL_SOUND_SKILL_EXTINTION	 , chrOtherPlayer[cnt2].pX,chrOtherPlayer[cnt2].pY,chrOtherPlayer[cnt2].pZ );
						}
				}
			}
			break;

	}


	return TRUE;
}

DWORD	dwLastClanUserCode = 0;

//Ŭ�� �޼��� ������
int SendClanJoinService( DWORD dwMsgCode , smCHAR *lpChar  )
{
	smTRANS_CHAR_COMMAND2	smTransCharCommand;

#ifdef  __CLANSUBCHIP__
	if ( cldata.myPosition!=101 && cldata.myPosition!=104) return FALSE;
#else
	if ( cldata.myPosition!=101 ) return FALSE;
#endif

	smTransCharCommand.code = smTRANSCODE_CLAN_SERVICE;
	smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND2);
	smTransCharCommand.WParam = dwMsgCode;
	smTransCharCommand.LParam = lpChar->dwObjectSerial;
	smTransCharCommand.SParam = lpCurPlayer->dwObjectSerial;
	lstrcpy( smTransCharCommand.szName , cldata.name );
	lstrcpy( smTransCharCommand.szId , UserAccount );

	if ( smWsockUserServer ) 
		smWsockUserServer->Send2( (char *)&smTransCharCommand , smTransCharCommand.size , TRUE );

	return TRUE;
}

//Ŭ�� ���� ���� �޼��� ������
int SendJoinClan()
{
	smTRANS_CHAR_COMMAND2	smTransCharCommand;

	if ( !dwLastClanUserCode ) return FALSE;

	smTransCharCommand.code = smTRANSCODE_CLAN_SERVICE;
	smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND2);
	smTransCharCommand.WParam = 2;
	smTransCharCommand.LParam = dwLastClanUserCode;
	smTransCharCommand.SParam = lpCurPlayer->dwObjectSerial;
	lstrcpy( smTransCharCommand.szName , lpCurPlayer->smCharInfo.szName );
	lstrcpy( smTransCharCommand.szId , UserAccount );

	if ( smWsockUserServer ) 
		smWsockUserServer->Send2( (char *)&smTransCharCommand , smTransCharCommand.size , TRUE );

	return TRUE;
}

//Ŭ�� �޼��� �ޱ�
int RecvClanJoinService( smTRANS_CHAR_COMMAND2 *lpTransCharCommand )
{
	smCHAR *lpChar;

	switch( lpTransCharCommand->WParam ) {
		case 1:		//���� ����
			if ( cldata.myPosition==102 || cldata.myPosition==103 ) {

				lpChar = FindAutoPlayer( lpTransCharCommand->SParam );
				if ( lpChar ) {
					ClanJoin( lpTransCharCommand->szName , lpChar->smCharInfo.szName );
					OpenEachPlayer( lpChar );
					DispEachMode = TRUE;
					dwLastClanUserCode = lpTransCharCommand->SParam;
				}
			}
			break;

		case 2:
#ifdef  __CLANSUBCHIP__
			if ( cldata.myPosition==101 ||  cldata.myPosition==104  ) {
#else
			if ( cldata.myPosition==101 ) {
#endif
				lpChar = FindAutoPlayer( lpTransCharCommand->SParam );
				if ( lpChar ) 
					menu_joinOpen_Chip( 1 , lpTransCharCommand->szId , lpTransCharCommand->szName , lpChar->smCharInfo.JOB_CODE , lpChar->smCharInfo.Level );
				else
					menu_joinOpen_Chip( 1 , lpTransCharCommand->szId , lpTransCharCommand->szName , 0 , 0 );
			}
			break;
	}

	return TRUE;
}


/////////// DivineLightning ��ų ���� �Լ� ///////////////////

DWORD	dwSkill_DivineLightning_Target[8];
int		DivineLightning_Target_Count = 0;
int		DivineLightning_FindCount = 0;

//����� ����Ʈ�� ��ų ���� ( �غ��Լ� - ���ΰ��� )
int	SkillPlay_DivineLightning_Select( smCHAR *lpChar , int SkillPoint )
{
	int cnt,TargetCount;
	int x,y,z,dist;
	int	LightningCount;
	int cCount;
	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	smWINSOCK *lpsmSendSock =0;

	//Ŭ���̾�Ʈ �α� ���� ���
	Record_ClinetLogFile( "������ ������Ŷ - SkillPlay_DivineLightning_Select " );
	quit = 1;
	return TRUE;

	if ( lpChar!=lpCurPlayer ) return FALSE;

	TargetCount = 0;

	LightningCount = Divine_Lightning_Num[ SkillPoint-1 ];

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {

		cCount = (DivineLightning_FindCount+cnt)&OTHER_PLAYER_MASK;

		if ( chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial && 
			lpChar!=&chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State==smCHAR_STATE_ENEMY &&
			chrOtherPlayer[cCount].smCharInfo.Brood!=smCHAR_MONSTER_USER &&
			chrOtherPlayer[cCount].smCharInfo.Life[0]>0 ) {

			x = ( lpChar->pX - chrOtherPlayer[cCount].pX )>>FLOATNS;
			y = ( lpChar->pY - chrOtherPlayer[cCount].pY )>>FLOATNS;
			z = ( lpChar->pZ - chrOtherPlayer[cCount].pZ )>>FLOATNS;

			dist = x*x+y*y+z*z;

			if ( dist<(160*160) ) {
				dwSkill_DivineLightning_Target[TargetCount] = chrOtherPlayer[cCount].dwObjectSerial;
				TransSkilAttackData.dwTarObjectSerial[TargetCount] = chrOtherPlayer[cCount].dwObjectSerial;
				TargetCount++;

				if ( chrOtherPlayer[cCount].smCharInfo.Life[1] && chrOtherPlayer[cCount].smCharInfo.Life[0]==chrOtherPlayer[cCount].smCharInfo.Life[1] ) {
					chrOtherPlayer[cCount].EnableStateBar = TRUE;
				}
				if ( TargetCount>=LightningCount ) {
					DivineLightning_FindCount = cCount;
					break;
				}
			}
		}
	}

	TransSkilAttackData.TargetCount = TargetCount;
	DivineLightning_Target_Count = TargetCount;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA)-sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*TargetCount+16;

	TransSkilAttackData.x = lpChar->pX;
	TransSkilAttackData.y = lpChar->pY;
	TransSkilAttackData.z = lpChar->pZ;

	TransSkilAttackData.AttackState = 103;			//�𵥵� ����
	TransSkilAttackData.AttackSize  = 160*160;
	TransSkilAttackData.Power		= GetRandomPos( Divine_Lightning_Damage[SkillPoint-1][0] , Divine_Lightning_Damage[SkillPoint-1][1] );

	lpsmSendSock = GetAreaServerSock();		//���� �������ϱ�

	if ( lpsmSendSock && TargetCount>0 ) {
		//���� ���ݷ� ���
		Record_SendDamage( lpsmSendSock , TransSkilAttackData.Power );

		lpsmSendSock->Send2( (char *)&TransSkilAttackData , TransSkilAttackData.size , TRUE );
	}

	return TRUE;
}

//����� ����Ʈ�� ��ų ���� ( ����Ʈ ���� )
int SkillPlay_DivineLightning_Effect( smCHAR *lpChar , int SkillPoint )
{
	int cnt,TargetCount;
	int x,y,z,dist;
	int	LightningCount;
	int cCount;
	smCHAR *lpTarChar;

	TargetCount = 0;

	if ( lpChar==lpCurPlayer ) {
		for( cnt=0;cnt<DivineLightning_Target_Count;cnt++ ) {
			lpTarChar = FindAutoPlayer( dwSkill_DivineLightning_Target[cnt] );
			if ( lpTarChar ) {
				AssaParticle_DivineLighting( lpTarChar );
				TargetCount++;
			}
		}
	}
	else {
		LightningCount = Divine_Lightning_Num[ SkillPoint-1 ];

		for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
			cCount = (DivineLightning_FindCount+cnt)&OTHER_PLAYER_MASK;
			if ( chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial &&
				lpChar!=&chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State==smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cCount].smCharInfo.Brood!=smCHAR_MONSTER_USER &&
				chrOtherPlayer[cCount].smCharInfo.Life[0]>0 ) {

				x = ( lpChar->pX - chrOtherPlayer[cCount].pX )>>FLOATNS;
				y = ( lpChar->pY - chrOtherPlayer[cCount].pY )>>FLOATNS;
				z = ( lpChar->pZ - chrOtherPlayer[cCount].pZ )>>FLOATNS;

				dist = x*x+y*y+z*z;

				if ( dist<(160*160) ) {
					AssaParticle_DivineLighting( &chrOtherPlayer[cCount] );
					TargetCount++;
					if ( TargetCount>=LightningCount ) {
						DivineLightning_FindCount = cCount;
						break;
					}
				}
			}
		}

		if ( TargetCount<LightningCount && lpChar->smCharInfo.State==smCHAR_STATE_ENEMY ) {
			x = ( lpChar->pX - lpCurPlayer->pX )>>FLOATNS;
			y = ( lpChar->pY - lpCurPlayer->pY )>>FLOATNS;
			z = ( lpChar->pZ - lpCurPlayer->pZ )>>FLOATNS;

			dist = x*x+y*y+z*z;

			if ( dist<(160*160) ) {
				AssaParticle_DivineLighting( lpCurPlayer );
				TargetCount++;
			}
		}

	}


	return TargetCount;
}


//����� ����Ʈ�� ��ų ���� ( �ӹ̷ε� )
int SkillPlay_MummyLord_Effect( smCHAR *lpChar , int Range )
{
	int cnt,TargetCount;
	int x,y,z,dist;
	int	dRange = Range*Range;

	TargetCount = 0;

	if ( lpChar->smCharInfo.Brood!=smCHAR_MONSTER_USER ) {
		//���Ͱ� ���� ����
		for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
			if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
				lpChar!=&chrOtherPlayer[cnt] && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_USER &&
				chrOtherPlayer[cnt].smCharInfo.Life[0]>0 ) 
			{

					x = ( lpChar->pX - chrOtherPlayer[cnt].pX )>>FLOATNS;
					y = ( lpChar->pY - chrOtherPlayer[cnt].pY )>>FLOATNS;
					z = ( lpChar->pZ - chrOtherPlayer[cnt].pZ )>>FLOATNS;

					dist = x*x+y*y+z*z;

					if ( dist<dRange ) {
						ParkAssaParticle_DivineLighting( &chrOtherPlayer[cnt] );
						TargetCount++;
					}
			
			}

		}

		if ( lpChar->smCharInfo.State==smCHAR_STATE_ENEMY ) {
			x = ( lpChar->pX - lpCurPlayer->pX )>>FLOATNS;
			y = ( lpChar->pY - lpCurPlayer->pY )>>FLOATNS;
			z = ( lpChar->pZ - lpCurPlayer->pZ )>>FLOATNS;

			dist = x*x+y*y+z*z;

			if ( dist<dRange ) {
				ParkAssaParticle_DivineLighting( lpCurPlayer );
				TargetCount++;
			}
		}
	}
	else {
		//������ ���� �� ( ���� ���� )
		for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
			if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
				lpChar!=&chrOtherPlayer[cnt] && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cnt].smCharInfo.Life[0]>0 ) 
			{

					x = ( lpChar->pX - chrOtherPlayer[cnt].pX )>>FLOATNS;
					y = ( lpChar->pY - chrOtherPlayer[cnt].pY )>>FLOATNS;
					z = ( lpChar->pZ - chrOtherPlayer[cnt].pZ )>>FLOATNS;

					dist = x*x+y*y+z*z;

					if ( dist<dRange ) {
						ParkAssaParticle_DivineLighting( &chrOtherPlayer[cnt] );
						TargetCount++;
					}
			
			}
		}
	}


	return TargetCount;
}


//���� ���Ǿ� ��ų ���� ( ����Ʈ ���� )
int SkillPlay_VenomSpear_Effect( smCHAR *lpChar , int SkillPoint )
{
	int cnt,TargetCount;
	int x,y,z,dist;
	int	LightningCount;
	int cCount;
	smCHAR *lpTarChar;
	int range;

	range = VenomSpear_Range[ SkillPoint-1 ];
	range *= range;

	TargetCount = 0;

	if ( lpChar==lpCurPlayer ) {
		for( cnt=0;cnt<DivineLightning_Target_Count;cnt++ ) {
			lpTarChar = FindAutoPlayer( dwSkill_DivineLightning_Target[cnt] );
			if ( lpTarChar ) {
				AssaParticle_VeonmSpear( lpTarChar );
				TargetCount++;
			}
		}
	}
	else {
		LightningCount = Divine_Lightning_Num[ SkillPoint-1 ];

		for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
			cCount = (DivineLightning_FindCount+cnt)&OTHER_PLAYER_MASK;
			if ( chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial &&
				lpChar!=&chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State==smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cCount].smCharInfo.Brood!=smCHAR_MONSTER_USER &&
				chrOtherPlayer[cCount].smCharInfo.Life[0]>0 ) {

				x = ( lpChar->pX - chrOtherPlayer[cCount].pX )>>FLOATNS;
				y = ( lpChar->pY - chrOtherPlayer[cCount].pY )>>FLOATNS;
				z = ( lpChar->pZ - chrOtherPlayer[cCount].pZ )>>FLOATNS;

				dist = x*x+y*y+z*z;

				if ( dist<range ) {
					AssaParticle_VeonmSpear( &chrOtherPlayer[cCount] );
					TargetCount++;
					if ( TargetCount>=LightningCount ) {
						DivineLightning_FindCount = cCount;
						break;
					}
				}
			}
		}
	}


	return TargetCount;
}

//���� ��ų ����Ʈ ���� ( ����Ʈ ���� )
int SkillPlay_Monster_Effect( smCHAR *lpChar , int EffectKind , int Range )
{
	int cnt,TargetCount;
	int x,y,z,dist;
	int dDist = Range*Range;


	TargetCount = 0;

	x = ( lpChar->pX - lpCurPlayer->pX )>>FLOATNS;
	y = ( lpChar->pY - lpCurPlayer->pY )>>FLOATNS;
	z = ( lpChar->pZ - lpCurPlayer->pZ )>>FLOATNS;

	dist = x*x+y*y+z*z;

	if ( dist<dDist ) {
		ParkAssaParticle_ChaosKaraSkill_User(lpCurPlayer);	// ī����ī�� �����̾ (����)
		TargetCount++;
	}


	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
			lpChar!=&chrOtherPlayer[cnt] && chrOtherPlayer[cnt].smCharInfo.State==smCHAR_STATE_USER &&
			chrOtherPlayer[cnt].smCharInfo.Life[0]>0 ) 
		{
			x = ( lpChar->pX - chrOtherPlayer[cnt].pX )>>FLOATNS;
			y = ( lpChar->pY - chrOtherPlayer[cnt].pY )>>FLOATNS;
			z = ( lpChar->pZ - chrOtherPlayer[cnt].pZ )>>FLOATNS;

			dist = x*x+y*y+z*z;

			if ( dist<dDist ) {
				ParkAssaParticle_ChaosKaraSkill_User(&chrOtherPlayer[cnt]);	// ī����ī�� �����̾ (����)
				TargetCount++;
			}
		}
	}

	return TargetCount;
}




//��ų ���� ��Ŷ ����
int RecvProcessSkill( smTRANS_COMMAND *lpTransCommand )
{
	int cnt;
	int dwSkillCode , point;
	smCHAR	*lpChar, *lpChar2;
	POINT3D	Pos1,Pos2;

	dwSkillCode = lpTransCommand->WParam&0xFF;
	point = (lpTransCommand->WParam>>8)&0xF;

	switch( dwSkillCode ) {
		case SKILL_PLAY_HOLY_REFLECTION:
			//Ȧ�� ���÷��� Ÿ�� ����Ʈ
			sinSkillEffect_Holy_Reflection_Defense( lpCurPlayer );
			break;

		case SKILL_PLAY_ENCHANT_WEAPON:
			//��þƮ ����
			cnt = Enchant_Weapon_Time[point-1];				//lpTransCommand->LParam-1];
			lpCurPlayer->EnchantEffect_Point = point;		//lpTransCommand->LParam;

			cSkill.SetEnchant_Weapon(cnt,point);

			switch( lpTransCommand->EParam ) {
				case 0:
					AssaParticle_EnchantWeaponIceJang( lpCurPlayer , cnt );
					lpCurPlayer->WeaponEffect = sITEMINFO_ICE+1;
					lpCurPlayer->dwWeaponEffectTime = dwPlayTime + cnt*1000;
					break;
				case 1:
					AssaParticle_EnchantWeaponLightJang( lpCurPlayer , cnt );
					lpCurPlayer->WeaponEffect = sITEMINFO_LIGHTING+1;
					lpCurPlayer->dwWeaponEffectTime = dwPlayTime + cnt*1000;
					break;
				case 2:
					AssaParticle_EnchantWeaponFireJang( lpCurPlayer , cnt );
					lpCurPlayer->WeaponEffect = sITEMINFO_FIRE+1;
					lpCurPlayer->dwWeaponEffectTime = dwPlayTime + cnt*1000;
					break;
			}
			break;

		case SKILL_PLAY_RESURRECTION:
			//������ �츰��
			if ( lpCurPlayer->MotionInfo && lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_DEAD ) {
				SendProcessSKillToServer( SKILL_PLAY_RESURRECTION , point , 0, 0 );

				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_RESTART );		//���۸��
				StartEffect( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, EFFECT_GAME_START1 );
				SetDynLight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 100, 100,100,0, 200 );
				CharPlaySound( lpCurPlayer );

				sinSetLife( lpCurPlayer->smCharInfo.Life[1]/2 );
				ResetEnergyGraph( 4 );								//������ �׷��� ýũ �ʱ�ȭ
				lpCurPlayer->MoveFlag = FALSE;
				RestartPlayCount = 350;								//5�� ���� ����
				ReStartFlag = 0;
			}
			break;

		case SKILL_PLAY_VIRTUAL_LIFE:
			//����� ������ ����
			cSkill.SetVirtualLife( Virtual_Life_Time[ point-1 ] , point );
			AssaParticle_VirtualLife( lpCurPlayer , Virtual_Life_Time[ point-1 ] );
			break;

		case SKILL_PLAY_LOVELY_LIFE:
			//����� ���ݷ�
			if ( lpTransCommand->LParam==lpCurPlayer->dwObjectSerial ) {
				cSkill.SetVirtualLife( Virtual_Life_Time[ point-1 ] , point );
				AssaParticle_VirtualLife( lpCurPlayer , Virtual_Life_Time[ point-1 ] );
				SkillPlaySound( SKILL_SOUND_SKILL_VIRTUAL_LIFE	 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );
			}
			else {
				lpChar = FindChrPlayer( lpTransCommand->LParam );
				if ( lpChar ) {
					AssaParticle_VirtualLife( lpChar , Virtual_Life_Time[ point-1 ] );
					SkillPlaySound( SKILL_SOUND_SKILL_VIRTUAL_LIFE	 , lpChar->pX,lpChar->pY,lpChar->pZ );
				}
			}
			break;

		case SKILL_PLAY_VANISH:
			//��Ͻ� ��ų ����
			lpCurPlayer->PlayVanish = 0;
			cSkill.CancelContinueSkill( SKILL_VANISH );

			if( dwM_BlurTime ) dwM_BlurTime = dwPlayTime;
			break;

		case SKILL_PLAY_TRIUMPH_OF_VALHALLA:
			//Ʈ���̾���
			cSkill.SetT_Of_Valhalla( point , lpTransCommand->EParam );
			AssaParticle_TriumphOfValhalla( lpCurPlayer , T_Of_Valhalla_Time[ point-1 ] );
			break;

		case SKILL_PLAY_SPARK_SHIELD:
			//����ũ���� ����Ʈ

			// ������ȣ�� ã�´�
			lpChar = FindChrPlayer( lpTransCommand->LParam );
			lpChar2 = FindChrPlayer( lpTransCommand->SParam );
			if ( lpChar && lpChar2 ) {
				//����ũ ����
				if ( lpChar->HvLeftHand.PatTool ) {
					if ( lpChar->GetToolBipPoint( &lpChar->HvLeftHand , &Pos1.x,&Pos1.y,&Pos1.z ) ) {

						Pos2.x = lpChar2->pX;
						Pos2.y = lpChar2->pY+24*fONE;
						Pos2.z = lpChar2->pZ;

						AssaParticle_SparkShieldDefence( &Pos1 , &Pos2 );
						SkillPlaySound( SKILL_SOUND_SKILL_SPARK2	 , lpChar->pX,lpChar->pY,lpChar->pZ );
					}
				}
			}
			break;

		case SKILL_PLAY_HALL_OF_VALHALLA:
			//Ȧ������Ҷ�
			cSkill.HellOfValhalla( Hall_Of_Valhalla_Time[point-1] , point , lpTransCommand->SParam , lpTransCommand->EParam , 1 );
			SkillValhallaHallOfValhallaHandEffect( lpCurPlayer , (float)lpTransCommand->EParam );
			lpCurPlayer->dwHallOfValhallaTime = dwPlayTime+Hall_Of_Valhalla_Time[point-1]*1000;	//Ȧ������Ҷ� �����ð�
			break;

		case SKILL_PLAY_FORCE_OF_NATURE:
			//�������� ������
			cSkill.ForeceOfNature( Force_Of_Nature_Time[point-1] , point , 1 );
			SkillSagittarionForceOfNature1( lpCurPlayer , (float)Force_Of_Nature_Time[point-1] );
			lpCurPlayer->dwForceOfNatureTime = dwPlayTime+Force_Of_Nature_Time[point-1]*1000;	//�������� ������ �����ð�
			break;


		case SKILL_PLAY_SOD_ITEM:
			//SOD ��¦ ������ ����Ʈ

			switch( lpTransCommand->SParam ) {
				case 1:	//��ź
					SetBellatraFontEffect( E_BL_QUAKE_SEAL );
					SetBellatraFontEffect( E_BL_FONT_QUAKE_SEAL );
					EffectWaveCamera(50, 3);
					SkillPlaySound( SKILL_SOUND_SKILL_DIASTROPHISM	 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );
					break;

				case 2:	//�ð�
					SetBellatraFontEffect( E_BL_STUN_SEAL );
					SetBellatraFontEffect( E_BL_FONT_STUN_SEAL );
					EffectWaveCamera(50, 3);
					SkillPlaySound( SKILL_SOUND_SKILL_DIASTROPHISM	 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );
					break;

				case 3:	//����
					SetBellatraFontEffect( E_BL_FREEZE_SEAL );
					SetBellatraFontEffect( E_BL_FONT_FREEZE_SEAL );
					EffectWaveCamera(50, 3);
					SkillPlaySound( SKILL_SOUND_SKILL_DIASTROPHISM	 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );
					break;

				case 4:	//�䳢
					SetBellatraFontEffect( E_BL_RABBIE_SEAL );
					SetBellatraFontEffect( E_BL_FONT_RABBIE_SEAL );
					EffectWaveCamera(50, 3);
					SkillPlaySound( SKILL_SOUND_SKILL_DIASTROPHISM	 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );
					break;

				case 5:	//�޼���
					SetBellatraFontEffect( E_BL_STYGIAN_SEAL );
					SetBellatraFontEffect( E_BL_FONT_STYGIAN_SEAL );
					break;

				case 6:	//�¾����
					SetBellatraFontEffect( E_BL_GUARDIAN_SEAL );
					SetBellatraFontEffect( E_BL_FONT_GUARDIAN_SEAL );
					break;

				case 7:	//õ������
					SetBellatraFontEffect( E_BL_POINT_SEAL );
					SetBellatraFontEffect( E_BL_FONT_POINT_SEAL );
					break;
			}
			esPlaySound( 19, 360 );		//SOD ������ �Ҹ�
			break;

		case SKILL_PLAY_HEALING:
			//���� ������ �������
			lpChar = FindChrPlayer( lpTransCommand->LParam );
			if ( lpChar ) {
				sinEffect_Healing2( lpChar );		//���� ����Ʈ
				SkillPlaySound( SKILL_SOUND_SKILL_HEALING , lpChar->pX,lpChar->pY,lpChar->pZ );		//���� ����
			}
			break;

		case SKILL_PLAY_BLESS_SIEGE_ITEM:
			//���� ĳ�� ���� ������ ���

			switch( lpTransCommand->SParam ) {

			////////////// ������ /////////////////
			case (sinBI1|sin05):	//������ũ��
				lpCurPlayer->PlayInvincible = 70*30;		//30��
				break;
			case (sinBI1|sin06):	//ũ��Ƽ�� ��ũ��
				break;
			case (sinBI1|sin07):	//�߰� ȸ�� ��ũ��
				break;

			//////////////// ������ //////////////////

			case (sinBC1|sin01):	//������ũ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_S_INVU);
				lpCurPlayer->PlayInvincible = 70*30;		//30��
				break;
			case (sinBC1|sin02):	//ũ��Ƽ�� ��ũ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_S_CRITICAL);
				break;
			case (sinBC1|sin03):	//�߰� ȸ�� ��ũ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_S_EVASION);
				break;
		/*
			case (sinBC1|sin04):	//��Ƽ�� ���� ȸ��
			case (sinBC1|sin05):	//��Ȱ ��ũ��
				return FALSE;
		*/
			case (sinBC1|sin06):	//���̾� ũ����Ż ���׼�
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_R_FIRE_C);
				break;
			case (sinBC1|sin07):	//���̽� ũ����Ż ���׼�
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_R_ICE_C);
				break;
			case (sinBC1|sin08):	//����Ʈ�� ũ����Ż ���׼�
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_R_LIGHTING_C);
				break;
			case (sinBC1|sin09):	//�� ������ ���ݷ� ��ȭ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_FIGHTER);
				break;
			case (sinBC1|sin10):	//�� ��ī�ϼ� ���ݷ� ��ȭ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_MECHANICIAN);
				break;
			case (sinBC1|sin11):	//������ũ�� ���ݷ� ��ȭ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_PIKEMAN);
				break;
			case (sinBC1|sin12):	//�� ���� ���ݷ� ��ȭ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_ARCHER);
				break;
			case (sinBC1|sin13):	//�� ����Ʈ ���ݷ� ��ȭ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_KNIGHT);
				break;
			case (sinBC1|sin14):	//�� ��Ż��Ÿ ���ݷ� ��ȭ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_ATALANTA);
				break;
			case (sinBC1|sin15):	//�� ������ ���ݷ� ��ȭ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_MAGICIAN);
				break;
			case (sinBC1|sin16):	//�� ������Ƽ�� ���ݷ� ��ȭ��
				//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_PRIESTESS);
				break;
			}
			break;

	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////

//int ReadClanInfo(DWORD dwClanNum)
//CLANINFO ClanInfo[CLAN_MAX]; //Ŭ�� ���� ����
//
//cldata ������Ʈ��
//void Updatecldata();

//Ŭ�� ������Ʈ �䱸 �޼���
//#define	smTRANSCODE_CLAN_UPDATE			0x50320C10


//Ŭ���� ���� ���ۿ� �Լ�
int SendClanCommandUser( smWINSOCK *lpsmSock , DWORD	dwCode , int Param1, int Param2 , CLANWONLIST *lpClanUserList , int ClanUserCount )
{

	TRANS_CLAN_COMMAND_USER	TransClanUser;
	int	cnt,len;
	char *lpBuff;
	DWORD	dwSpeedSum;
	int	BuffSize;

	TransClanUser.code = dwCode;

	TransClanUser.Param[0] = Param1;
	TransClanUser.Param[1] = Param2;
	TransClanUser.Param[2] = 0;
	TransClanUser.Param[3] = 0;
	TransClanUser.UserCount = ClanUserCount;

	lpBuff = TransClanUser.szUserBuff;

	for(cnt=0;cnt<ClanUserCount;cnt++) {
		dwSpeedSum = GetSpeedSum( lpClanUserList->clanWon[cnt] );
		((DWORD *)lpBuff)[0] = dwSpeedSum;
		lpBuff+=sizeof(DWORD);
		lstrcpy( lpBuff , lpClanUserList->clanWon[cnt] );
		len = lstrlen(lpClanUserList->clanWon[cnt]);
		lpBuff+=len+1;
	}

	BuffSize = lpBuff-TransClanUser.szUserBuff;
	BuffSize += 16;

	TransClanUser.size = 32+BuffSize;

	if ( lpsmSock && ClanUserCount>0 ) {
		lpsmSock->Send2( (char *)&TransClanUser , TransClanUser.size , TRUE );
	}

	return TRUE;
}

//Ŭ���� ���� �ص�
int	RecvClanCommand( TRANS_CLAN_COMMAND_USER *lpTransClanUser , _CLAN_USER_INFO *ClanUserInfo )// char **lpClanUserName , DWORD *dwUserSpeedSum )
{
	int cnt;
	int cnt2;
	char *lpBuff;
	char	ch;

	lpBuff = lpTransClanUser->szUserBuff;

	if ( lpTransClanUser->UserCount>=CLAN_USER_MAX ) return FALSE;

	for( cnt=0;cnt<lpTransClanUser->UserCount;cnt++ ) {
		ClanUserInfo[cnt].dwSpeedSum = ((DWORD *)lpBuff)[0];
		lpBuff+=sizeof(DWORD);
		for(cnt2=0;cnt2<32;cnt2++) {
			ch = *lpBuff;
			ClanUserInfo[cnt].szName[cnt2] = ch;
			lpBuff++;
			if ( !ch ) break;
		}
		ClanUserInfo[cnt].szName[31] = 0;
	}

	return TRUE;
}

//����Ʈ ������ �Լ� ���͸�
int FiltQuestItem( TRANS_ITEMINFO	*lpTransItemInfo , DWORD dwPacketCode )
{
	int cnt;

	if ( chaQuest.sHaQuestElementary[6].CODE != HAQUEST_CODE_ELEMENTARY_G ) { 
		
		cnt = cInvenTory.SearchItemCode( sinQT1|sin16 );
		if ( cnt >= 1 ){ DeleteQuestItem(sinQT1|sin16); }
		if( lpTransItemInfo->Item.CODE==(sinQT1|sin16) ){return FALSE; }		//���ټ���

	}

	switch( sinQuest_ChangeJob2.CODE ) {
		case SIN_QUEST_CODE_CHANGEJOB2_NPC_M:
			//���� ������ (�ִ� ���� Ȯ�� )
			if ( lpTransItemInfo->Item.CODE==(sinMA2|sin01) ) {
				if ( !cInvenTory.SearchItemCode( sinMA1|sin01 ) ) return FALSE;		//������ ���°��
				if ( cInvenTory.SearchItemCode( sinQT1|sin04 ) ) return FALSE;		//�ξ����� �ִ� ���
				cnt  = cInvenTory.SearchItemCode( sinMA2|sin01 );
				if ( cnt>=9 ) return FALSE;
			}
			if ( lpTransItemInfo->Item.CODE==(sinMA2|sin02) ) return FALSE;
			return TRUE;

		case SIN_QUEST_CODE_CHANGEJOB2_NPC_D:
			//�⸧ ������ (�ִ� ���� Ȯ�� )
			if ( lpTransItemInfo->Item.CODE==(sinMA2|sin02) ) {
				if ( !cInvenTory.SearchItemCode( sinMA1|sin01 ) ) return FALSE;		//������ ���°��
				if ( cInvenTory.SearchItemCode( sinQT1|sin05 ) ) return FALSE;		//�߸��� �ִ� ���
				cnt  = cInvenTory.SearchItemCode( sinMA2|sin02 );
				if ( cnt>=7 ) return FALSE;
			}
			if ( lpTransItemInfo->Item.CODE==(sinMA2|sin01) ) return FALSE;
			return TRUE;
	}

	//3�� ���� ����Ʈ
	//����
	if ( lpTransItemInfo->Item.CODE==(sinQT1|sin06) ) {
		if ( sinQuest_ChangeJob3.CODE==SIN_QUEST_CODE_CHANGEJOB3 ) {
			cnt  = cInvenTory.SearchItemCode( sinQT1|sin06 );
			if ( cnt>=3 ) return FALSE;
			return TRUE;
		}
		return FALSE;
	}

	//���� ����Ʈ�� ���� 1
	if ( lpTransItemInfo->Item.CODE==(sinQT1|sin09) ) {
		if ( sinQuest_Level80_2.CODE==SIN_QUEST_CODE_LEVEL80_2 ) {
			cnt  = cInvenTory.SearchItemCode( sinQT1|sin09 );
			if ( cnt>=1 ) return FALSE;
			return TRUE;
		}
		return FALSE;
	}
	//���� ����Ʈ�� ���� 2
	if ( lpTransItemInfo->Item.CODE==(sinQT1|sin10) ) {
		if ( sinQuest_Level80_2.CODE==SIN_QUEST_CODE_LEVEL80_2 ) {
			cnt  = cInvenTory.SearchItemCode( sinQT1|sin10 );
			if ( cnt>=1 ) return FALSE;
			return TRUE;
		}
		return FALSE;
	}
	//���� ����Ʈ�� ���� 3
	if ( lpTransItemInfo->Item.CODE==(sinQT1|sin11) ) {
		if ( sinQuest_Level80_2.CODE==SIN_QUEST_CODE_LEVEL80_2 ) {
			cnt  = cInvenTory.SearchItemCode( sinQT1|sin11 );
			if ( cnt>=1 ) return FALSE;
			return TRUE;
		}
		return FALSE;
	}


	//3�� ���� ����Ʈ�� 
	if ( lpTransItemInfo->Item.CODE==(sinQT1|sin13) ) {

		if ( sinQuest_ChangeJob4.CODE==SIN_QUEST_CODE_CHANGEJOB4 && sinQuest_ChangeJob4.State==5 ) {
			cnt  = cInvenTory.SearchItemCode( sinQT1|sin13 );
			if ( cnt>=1 || CheckAttMonsterCode( sinQuest_ChangeJob4.Kind )==FALSE ) 
				return FALSE;

			return TRUE;
		}
		return FALSE;
	}



	//������ ��ġ ����Ʈ 
	if ( lpTransItemInfo->Item.CODE==(sinQT1|sin16) ) {

		if ( chaQuest.sHaQuestElementary[6].CODE==HAQUEST_CODE_ELEMENTARY_G ) {
			cnt  = cInvenTory.SearchItemCode( sinQT1|sin16 );
			if ( cnt>=1 ) 
				return FALSE;

			if ( dwPacketCode==smTRANSCODE_GETITEM ) {
				//������ ȹ�� �޼���
				char szBuff[256];
				//wsprintf( szBuff, mgGetQuestItem , lpTransItemInfo->Item.ItemName );//_ignore_//�ؿ�
				AddChatBuff( szBuff , 6 );
			}
			return TRUE;
		}
		return FALSE;
	}


	if ( (lpTransItemInfo->Item.CODE&sinITEM_MASK2)==sinMA1 ||
		(lpTransItemInfo->Item.CODE&sinITEM_MASK2)==sinMA2 ) {
			//����Ʈ�� ���°�� ����Ʈ ������ ���� ���

			return FALSE;
		}

	return TRUE;
}

POINT GetProcessModule() ;

//���˻� �Ͽ� ������ ������
int CheckProcessModule()
{
	POINT p;
	OSVERSIONINFO VersionInfo;
	BOOL Result;
	smTRANS_COMMAND	smTransCommand;


	return TRUE;

	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	ZeroMemory( &smTransCommand , sizeof(smTRANS_COMMAND) );
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_CLIENTINFO;

	Result = GetVersionEx(&VersionInfo);
	p = GetProcessModule();

	if (Result != FALSE) {
		smTransCommand.WParam = VersionInfo.dwPlatformId;
		smTransCommand.LParam = (VersionInfo.dwMajorVersion*100)+ VersionInfo.dwMinorVersion;
		smTransCommand.LParam = (smTransCommand.LParam<<16)|(VersionInfo.dwBuildNumber&0xFFFF);
		smTransCommand.SParam = p.x;
		smTransCommand.EParam = p.y;

		smTransCommand.WParam ^= ((DWORD *)(UserAccount+0))[0];
		smTransCommand.LParam ^= ((DWORD *)(UserAccount+1))[0];
		smTransCommand.SParam ^= ((DWORD *)(UserAccount+2))[0];
		smTransCommand.EParam ^= ((DWORD *)(UserAccount+3))[0];

		if ( smWsockDataServer ) 
			smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}

	return TRUE;
}

//�Ȱ� ������ ������
int SendFalconDamage( smCHAR *lpChar )
{
	DWORD point;
	int cnt;

	if ( lpChar==lpCurPlayer && lpChar->chrAttackTarget ) {
		point = ((SkillFalconPoint^((DWORD)chrOtherPlayer^lpCurPlayer->dwObjectSerial))+1)<<8;
		point |= SKILL_PLAY_FALCON;
		cnt = dm_SendTransDamage( lpChar->chrAttackTarget  , 0,0 , 0, 0, point , FALSE );
		DamageExp( lpChar->chrAttackTarget , cnt );
		return cnt;
	}
	return NULL;
}

//��̼ҵ� ������ ������
int SendDancingSwordDamage( smCHAR *lpChar )
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if ( lpChar ) {

		wpDamage[0] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1];

		point = 1<<8;
		point |= SKILL_PLAY_DANCING_SWORD;
		cnt = dm_SendTransDamage( lpChar  , wpDamage[0],wpDamage[1] , 0, 0, point , FALSE );
		DamageExp( lpChar , cnt );

		if ( (rand()%2)==0 ) 
			SkillPlaySound( SKILL_SOUND_SKILL_DANCING_SWORD_ATK1 , lpChar->pX,lpChar->pY,lpChar->pZ );
		else
			SkillPlaySound( SKILL_SOUND_SKILL_DANCING_SWORD_ATK2 , lpChar->pX,lpChar->pY,lpChar->pZ );

		return cnt;
	}
	return FALSE;
}


//�ޱ׳�ƽ ���Ǿ� ������ ������
int SendMegneticSphereDamage( smCHAR *lpChar )
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if ( lpChar ) {

		wpDamage[0] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1];

		point = 1<<8;
		point |= SKILL_PLAY_MAGNETIC_SPHERE;
		cnt = dm_SendTransDamage( lpChar  , wpDamage[0],wpDamage[1] , 0, 0, point , FALSE );
		DamageExp( lpChar , cnt );

		if ( (rand()%2)==0 ) 
			SkillPlaySound( SKILL_SOUND_SKILL_DANCING_SWORD_ATK1 , lpChar->pX,lpChar->pY,lpChar->pZ );
		else
			SkillPlaySound( SKILL_SOUND_SKILL_DANCING_SWORD_ATK2 , lpChar->pX,lpChar->pY,lpChar->pZ );

		return cnt;
	}
	return FALSE;
}

//�ӽ��� ������ ������
int SendMuspellDamage( smCHAR *lpChar )
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if ( lpChar ) {

		wpDamage[0] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1];

		point = 1<<8;
		point |= SKILL_PLAY_SUMMON_MUSPELL;
		cnt = dm_SendTransDamage( lpChar  , wpDamage[0],wpDamage[1] , 0, 0, point , FALSE );
		DamageExp( lpChar , cnt );

		if ( (rand()%2)==0 ) 
			SkillPlaySound( SKILL_SOUND_SKILL_DANCING_SWORD_ATK1 , lpChar->pX,lpChar->pY,lpChar->pZ );
		else
			SkillPlaySound( SKILL_SOUND_SKILL_DANCING_SWORD_ATK2 , lpChar->pX,lpChar->pY,lpChar->pZ );

		return cnt;
	}
	return FALSE;
}



//������ �� ������
int SendLowLevelPetDamage( smCHAR *lpChar , int petType )
{
    DWORD point;
	int cnt;
	int wpDamage[2];

	if ( lpChar ) {

		wpDamage[0] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1];

		point = (1+petType)<<8;
		point |= SKILL_PLAY_PET_ATTACK;
		cnt = dm_SendTransDamage( lpChar  , wpDamage[0],wpDamage[1] , 0, 0, point , FALSE );
		DamageExp( lpChar , cnt );
		
		return cnt;
	}
	return FALSE;
}


//PC��� �� ������
int SendPCBangPetDamage( smCHAR *lpChar , int petType )
{
    DWORD point;
	int cnt;
	int wpDamage[2];

	if ( lpChar ) {

		wpDamage[0] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[ sInven[0].ItemIndex-1 ].sItemInfo.Damage[1];

		point = (1+petType)<<8;
		point |= SKILL_PLAY_PET_ATTACK2;
		cnt = dm_SendTransDamage( lpChar  , wpDamage[0],wpDamage[1] , 0, 0, point , FALSE );
		DamageExp( lpChar , cnt );
		
		return cnt;
	}
	return FALSE;
}



//������ �ù� ���� ���� �䱸
int	SendItemExpress( DWORD	dwItemCode , char *szPassCode )
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory( (char *)&smTransCommand , sizeof( smTRANS_COMMAND ) );
	smTransCommand.code = smTRANSCODE_ITEM_EXPRESS;
	smTransCommand.size = sizeof( smTRANS_COMMAND );
	smTransCommand.WParam = dwItemCode;

	if ( szPassCode )
		smTransCommand.LParam = GetSpeedSum( szPassCode );

	if ( smWsockDataServer ) 
		return smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return FALSE;
}

//������ �ù� ���� ���� �䱸
int	SendItemExpress( DWORD	dwItemCode )
{
	return	SendItemExpress( dwItemCode , 0 );
}


//��ȣ ��� ������ ������
int SendClanYahooMotion()
{
	smTRANS_COMMAND	smTransCommand;
	smWINSOCK *lpsmSock;

	if ( lpCurPlayer->smCharInfo.ClassClan ) {
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_YAHOO_MOTION;
		smTransCommand.WParam = dwPlayTime;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		lpsmSock = GetAreaServerSock();		//�������� ������ ã��
		if ( lpsmSock )
			lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

		return TRUE;

	}
	return FALSE;
}

//�� ����Ʈ ������ ������ ������
int	SendStarPointToServer( int Price , int Cash )
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_STARPOINT;
	smTransCommand.WParam = Price;
	smTransCommand.LParam = Cash;
	smTransCommand.SParam = 0;
	smTransCommand.EParam = 0;

	if ( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return FALSE;
}

//����ѵ��� ������ ������
int	SendGiveMoneyToServer( int Money )
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_GIVEMONEY;
	smTransCommand.WParam = Money;
	smTransCommand.LParam = 0;
	smTransCommand.SParam = 0;
	smTransCommand.EParam = 0;

	if ( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return FALSE;
}

//Ŭ���Ӵ� ������ ������ ������
int	SendClanMoneyToServer( int Money , int Flag , int Kind )
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_CLANMONEY;
	smTransCommand.WParam = Money;
	smTransCommand.LParam = Flag;
	smTransCommand.SParam = Kind;
	smTransCommand.EParam = 0;

	if ( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return FALSE;
}

int	SendClanMoneyToServer( int Money , int Flag )
{
	return	SendClanMoneyToServer( Money,Flag,0 );
}

//�̿��� ������ ������ ������
int	SendPaymentMoneyToServer( int Money , int Flag )
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_PAYMENT_MONEY;
	smTransCommand.WParam = Flag;
	smTransCommand.LParam = Money;
	smTransCommand.SParam = 0;
	smTransCommand.EParam = 0;

	if ( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return FALSE;
}



/*
//���λ��� ����
#define smTRANSCODE_OPEN_PERTRADE	0x48478A90
//���λ��� �θ���
#define smTRANSCODE_CALL_PERTRADE	0x48478AA0
//���λ��� ������
#define	smTRANSCODE_MYSHOP_ITEM		0x48478AB0

int	Send_ShowMyShopItem( smCHAR *lpChar );							//���λ��� ������ ���� �䱸
int	Send_MyShopItemList( smTRANS_COMMAND	*lpTransCommand );		//���λ��� ������ ���� ���濡 ������
int	Recv_MyShopItemList( TRANS_TRADEITEMS *lpTransTradeItems );		//���λ��� ������ ���� ����
*/


//���λ��� ������ ���� �䱸
int	Send_ShowMyShopItem( smCHAR *lpChar )
{
	smTRANS_COMMAND	smTransCommand;
	int	result;
	smWINSOCK	*lpsmSock;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_CALL_MYSHOP;
	smTransCommand.WParam = 0;
	smTransCommand.LParam = lpChar->dwObjectSerial;
	smTransCommand.SParam = lpCurPlayer->dwObjectSerial;
	smTransCommand.EParam = 0;

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) 
		result = lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return result;
}

//���λ��� ������ ���� ���濡 ������
int	Send_MyShopItemList( smTRANS_COMMAND	*lpTransCommand )
{
	int	result;
	smWINSOCK	*lpsmSock;

	TransTradeItems_MyShop.dwRecver = lpTransCommand->SParam;

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) 
		result = lpsmSock->Send2( (char *)&TransTradeItems_MyShop , TransTradeItems_MyShop.size , TRUE );

	return result;
}


//���λ��� ������ ���� ����
int	Recv_MyShopItemList( TRANS_TRADEITEMS *lpTransTradeItems )
{
	sMYSHOP MyShopItem;
	smCHAR	*lpChar;

	//���� ����Ÿ �ص� ( Z/NZ ��� )
	DecodeCompress( lpTransTradeItems->TradeBuff , (BYTE *)&MyShopItem );

	//��ľ� �Լ� ȣ�� ����
	cCharShop.RecvShopItem( &MyShopItem );


	lpChar = FindAutoPlayer( lpTransTradeItems->dwSender );

	if ( lpChar && lpChar->DisplayFlag && lpChar->smCharInfo.szName[0] ) {
		OpenEachPlayer( lpChar );
		DispEachMode = TRUE;
	}

	return TRUE;
}


//���� ���� ���� ����
int	Send_PersonalShopItem( DWORD dwCharCode , void *lpShopItem )
{
	smTRANS_COMMAND_BUFF	smtBuff;


	smtBuff.smTransCommand.code = smTRANSCODE_MYSHOP_TRADE;
	smtBuff.smTransCommand.size = sizeof(smTRANS_COMMAND)+sizeof(sMYSHOP_ITEM_SERVER);
	smtBuff.smTransCommand.WParam = 0;
	smtBuff.smTransCommand.LParam = dwCharCode;
	smtBuff.smTransCommand.SParam = lpCurPlayer->dwObjectSerial;
	smtBuff.smTransCommand.EParam = 0;

	memcpy( smtBuff.Buff , lpShopItem , sizeof(sMYSHOP_ITEM_SERVER) );

	if ( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)&smtBuff , smtBuff.smTransCommand.size , TRUE );

	return FALSE;
}

//���� ���� ������ ���� ������Ʈ
int	UpdateMyShopList( void *lpMyShop )
{
	sMYSHOP *lpMyShopItem=(sMYSHOP *)lpMyShop;
	int	result = 0;
	int		len;

	ZeroMemory( &TransTradeItems_MyShop , sizeof( TRANS_TRADEITEMS ) );

	//����Ÿ ���� ( Z/NZ ��� )
	len = EecodeCompress( (BYTE *)lpMyShopItem , TransTradeItems_MyShop.TradeBuff , sizeof(sMYSHOP) );
	if ( len>TRANS_TRADE_BUFF_SIZE ) return FALSE;

	TransTradeItems_MyShop.code = smTRANSCODE_MYSHOP_ITEM;
	TransTradeItems_MyShop.size = len+48;
	TransTradeItems_MyShop.dwSum = 0;
	TransTradeItems_MyShop.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeItems_MyShop.dwRecver = 0;
	TransTradeItems_MyShop.Temp[0] = 0;
	TransTradeItems_MyShop.Temp[1] = 0;
	TransTradeItems_MyShop.Temp[2] = 0;
	TransTradeItems_MyShop.Temp[3] = 0;

	return TRUE;
}

//���� ���� ����
int SendOpenPersonalTrade( char *szTradeMsg , void *lpPersTrade )
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK	*lpsmSock;
	int	result = 0;
	DWORD	dwCode;
	sMYSHOP *lpMyShopItem=(sMYSHOP *)lpPersTrade;
	int		len;

	//smTRANSCODE_OPEN_PERTRADE
	//lpMyShopItem->

	ZeroMemory( &TransTradeItems_MyShop , sizeof( TRANS_TRADEITEMS ) );
	//memcpy( &TransMyShop.sMyShop , lpMyShopItem , sizeof(sMYSHOP) );

	ZeroMemory( &TransChatMessage , sizeof(TRANS_CHATMESSAGE) );

	//����Ÿ ���� ( Z/NZ ��� )
	len = EecodeCompress( (BYTE *)lpMyShopItem , TransTradeItems_MyShop.TradeBuff , sizeof(sMYSHOP) );
	if ( len>TRANS_TRADE_BUFF_SIZE ) return FALSE;

	TransTradeItems_MyShop.code = smTRANSCODE_MYSHOP_ITEM;
	TransTradeItems_MyShop.size = len+48;
	TransTradeItems_MyShop.dwSum = 0;
	TransTradeItems_MyShop.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeItems_MyShop.dwRecver = 0;
	TransTradeItems_MyShop.Temp[0] = 0;
	TransTradeItems_MyShop.Temp[1] = 0;
	TransTradeItems_MyShop.Temp[2] = 0;
	TransTradeItems_MyShop.Temp[3] = 0;


	TransChatMessage.code = smTRANSCODE_OPEN_MYSHOP;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lstrcpy( TransChatMessage.szMessage , szTradeMsg );

	dwCode = GetSpeedSum(szTradeMsg);
	TransChatMessage.dwIP = dwCode;

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) 
//�ؿ� �߱�(���λ����̸�����)
//#ifdef _LANGUAGE_CHINESE
//		ConvertStringTHAI((char *)&TransChatMessage, lstrlen((char *)&TransChatMessage)+16);
//#endif
		result = lpsmSock->Send2( (char *)&TransChatMessage , TransChatMessage.size , TRUE );

	if ( result ) {
/*
//�ؿ� �߱�(���λ����̸�����)
#ifdef _LANGUAGE_CHINESE
		ConvertStringTHAI(szTradeMsg, lstrlen(szTradeMsg)+16);
#endif
*/
		wsprintf( lpCurPlayer->szTradeMessage , "%s: %s" , lpCurPlayer->smCharInfo.szName , szTradeMsg );
		lpCurPlayer->dwTradeMsgCode = dwCode;
	}

	return result;
}

//���� �ݱ�
int SendClosePersonalTrade()
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK	*lpsmSock;
	int	result = 0;

	ZeroMemory( &TransChatMessage , sizeof(TRANS_CHATMESSAGE) );

	TransChatMessage.code = smTRANSCODE_OPEN_MYSHOP;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) 
		result = lpsmSock->Send2( (char *)&TransChatMessage , TransChatMessage.size , TRUE );

	lpCurPlayer->dwTradeMsgCode = 0;
	lpCurPlayer->szTradeMessage[0] = 0;

	return result;
}

// �庰 - �Է�â
int SendsServerDoc( char *szTradeMsg )
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK	*lpsmSock;
	int	result = 0;


	ZeroMemory( &TransChatMessage , sizeof(TRANS_CHATMESSAGE) );

	TransChatMessage.code = smTRANSCODE_ITEMDOC;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lstrcpy( TransChatMessage.szMessage , szTradeMsg );

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) 
		result = lpsmSock->Send2( (char *)&TransChatMessage , TransChatMessage.size , TRUE );

	return result;
}


//��¥ Ȯ�� ���� ������ 
int DeleteEventItem_TimeOut( sITEMINFO	*lpsItem )
{
	// ����� - ����¡ ������, ��ų ������ �߰�
	if( lpsItem->CODE == (sinBI1|sin32) || lpsItem->CODE == (sinBI1|sin33) || lpsItem->CODE == (sinBI1|sin34) || lpsItem->CODE == (sinBI1|sin51) ||// pluto ��(�ؿ�)
	//	lpsItem->CODE == (sinBI1|sin35) || lpsItem->CODE == (sinBI1|sin36) || lpsItem->CODE == (sinBI1|sin37) || lpsItem->CODE == (sinBI1|sin38) )	//��ǥ
		lpsItem->CODE == (sinBI1|sin36) || lpsItem->CODE == (sinBI1|sin37) ||
		lpsItem->CODE == (sinBI1|sin38) || lpsItem->CODE == (sinBI1|sin39) ||
		lpsItem->CODE == (sinBI1|sin40) || lpsItem->CODE == (sinBI1|sin41) ||
		lpsItem->CODE == (sinBI1|sin42) || lpsItem->CODE == (sinBI1|sin43) ||
		lpsItem->CODE == (sinBI1|sin44) || lpsItem->CODE == (sinBI1|sin45) ||
		lpsItem->CODE == (sinBI1|sin46) || lpsItem->CODE == (sinBI1|sin47) ||
		lpsItem->CODE == (sinBI1|sin48) || lpsItem->CODE == (sinBI1|sin49) ||
		lpsItem->CODE == (sinBI1|sin50) || lpsItem->CODE == (sinBI1|sin52) || // ����� - ���� ����� �� �߰�
		lpsItem->CODE == (sinBI1|sin53) || lpsItem->CODE == (sinBI1|sin54) ||

		lpsItem->CODE == (sinBI1|sin65) || lpsItem->CODE == (sinBI1|sin66) || // �庰 - ���� ����� �� (7��, 1��)
		lpsItem->CODE == (sinBI1|sin67) || lpsItem->CODE == (sinBI1|sin68) ||
		lpsItem->CODE == (sinBI1|sin69) || lpsItem->CODE == (sinBI1|sin70) ||
		lpsItem->CODE == (sinBI1|sin71) || lpsItem->CODE == (sinBI1|sin72) ||
		lpsItem->CODE == (sinBI1|sin55) || lpsItem->CODE == (sinBI1|sin77) ||	// �庰 - �Ǵн���(1�ð�)
		lpsItem->CODE == (sinBI1|sin62) || lpsItem->CODE == (sinBI1|sin63) ||	// ����� - ����¡ ������(2��) ������ �߰� D, E
		lpsItem->CODE == (sinBI1|sin64) ||										// ����� - ����¡ ������(2��) ������ �߰� F
		lpsItem->CODE == (sinBI1|sin73) || lpsItem->CODE == (sinBI1|sin74) || // �庰 - ���� ����� �� �߰�(�׸� 1�ð� �� 4��)
		lpsItem->CODE == (sinBI1|sin75) || lpsItem->CODE == (sinBI1|sin76) ) // �庰 - ���� ����� �� (1�ð�)
	{
		return FALSE;
	}

	struct	tm	tm;
	time_t		ttm;//,ttm2;
	DWORD		dwCreateTime;

	DWORD		dwItemCode;

	dwItemCode = lpsItem->CODE&sinITEM_MASK2;

	DWORD NowTime = GetPlayTime_T(); // ���� �ð�(second����)

	//�Ѻ� 14��
	if( lpsItem->CODE == (sinDA1|sin48) || lpsItem->CODE == (sinDA2|sin48) )
	{
		if( lpsItem->dwCreateTime+(60*60*24*14) < NowTime )
			return true;
	}
	//�ڽ�Ƭ 7��
	if(  lpsItem->CODE == (sinDA1|sin31) || lpsItem->CODE == (sinDA1|sin33) ||
		 lpsItem->CODE == (sinDA1|sin35) || lpsItem->CODE == (sinDA1|sin37) ||
		 lpsItem->CODE == (sinDA1|sin39) || lpsItem->CODE == (sinDA1|sin41) ||
		 lpsItem->CODE == (sinDA1|sin43) || lpsItem->CODE == (sinDA1|sin45) ||
		 lpsItem->CODE == (sinDA2|sin31) || lpsItem->CODE == (sinDA2|sin33) ||
		 lpsItem->CODE == (sinDA2|sin35) || lpsItem->CODE == (sinDA2|sin37) ||
		 lpsItem->CODE == (sinDA2|sin39) || lpsItem->CODE == (sinDA2|sin41) ||
		 lpsItem->CODE == (sinDA2|sin43) || lpsItem->CODE == (sinDA2|sin45) ||
		 lpsItem->CODE == (sinDB1|sin31) || // ����� - ���ǵ� ����(7��) �߰�
		 lpsItem->CODE == (sinOA2|sin31) || // ����� - ���� �ϸ�(7��) �߰�
		 lpsItem->CODE == (sinOR2|sin31) || // ����� - ���� ���� �� �߰�(�ٺ�)
		 lpsItem->CODE == (sinOR2|sin32) )  // ����� - ���� ���� �� �߰�(ǻ��)
	{
		if( lpsItem->dwCreateTime+(60*60*24*7) < NowTime )
			return true;
	}
	//�ڽ�Ƭ 30��
	if(  lpsItem->CODE == (sinDA1|sin32) || lpsItem->CODE == (sinDA1|sin34) ||
		 lpsItem->CODE == (sinDA1|sin36) || lpsItem->CODE == (sinDA1|sin38) ||
		 lpsItem->CODE == (sinDA1|sin40) || lpsItem->CODE == (sinDA1|sin42) || 
		 lpsItem->CODE == (sinDA1|sin44) || lpsItem->CODE == (sinDA1|sin46) || 
		 lpsItem->CODE == (sinDA1|sin54) || lpsItem->CODE == (sinDA1|sin55) || // ����� - ������ ���� �߰�
		 lpsItem->CODE == (sinDA2|sin32) || lpsItem->CODE == (sinDA2|sin34) ||
		 lpsItem->CODE == (sinDA2|sin36) || lpsItem->CODE == (sinDA2|sin38) ||
		 lpsItem->CODE == (sinDA2|sin40) || lpsItem->CODE == (sinDA2|sin42) ||
		 lpsItem->CODE == (sinDA2|sin44) || lpsItem->CODE == (sinDA2|sin46) ||
		 lpsItem->CODE == (sinDA2|sin54) || lpsItem->CODE == (sinDA2|sin55) || // ����� - ������ ���� �߰�
		 lpsItem->CODE == (sinDB1|sin32) || // ����� - ���ǵ� ����(30��) �߰�
		 lpsItem->CODE == (sinOA2|sin32) || // ����� - ���� �ϸ�(30��) �߰�
		 lpsItem->CODE == (sinSP1|sin34) )  // ����� - ȣ���� ĸ��(30��) �߰�

	{
		if( lpsItem->dwCreateTime+(60*60*24*30) < NowTime )
			return true;
	}

	// 1��
	if(	lpsItem->CODE == ( sinDB1|sin33 ) || lpsItem->CODE == ( sinOA2|sin33 )) // �庰 - ���ǵ� ����(1��) �߰� // �庰 - ���� �ϸ�(1��)
	{
		if( lpsItem->dwCreateTime+(60*60*24*1) < NowTime )
			return true;
	}

	// 1�ð�
	if(	lpsItem->CODE == ( sinDB1|sin34) || lpsItem->CODE == ( sinOA2|sin34 )) // �庰 - ���ǵ� ����(1�ð�) �߰� // �庰 - ���� �ϸ�(1�ð�)
	{
		if( lpsItem->dwCreateTime+(60*60) < NowTime )
			return true;
	}

	//�ۼ��� : �����, �ۼ��� : 08.04.07
	//��  �� : Ŭ��ġ�� �� ������ 5���Ŀ� ����
	if(lpsItem->CODE == (sinOR2|sin06) || lpsItem->CODE == (sinOR2|sin07) ||
	   lpsItem->CODE == (sinOR2|sin08) || lpsItem->CODE == (sinOR2|sin09) ||
	   lpsItem->CODE == (sinOR2|sin10) || lpsItem->CODE == (sinOR2|sin11) ||
	   lpsItem->CODE == (sinOR2|sin12) || lpsItem->CODE == (sinOR2|sin13) ||
	   lpsItem->CODE == (sinOR2|sin14) || lpsItem->CODE == (sinOR2|sin15) ||
	   lpsItem->CODE == (sinOR2|sin16) || lpsItem->CODE == (sinOR2|sin17) ||
	   lpsItem->CODE == (sinOR2|sin18) || lpsItem->CODE == (sinOR2|sin19) ||
	   lpsItem->CODE == (sinOR2|sin20) || lpsItem->CODE == (sinOR2|sin21) ||
	   lpsItem->CODE == (sinOR2|sin22) || lpsItem->CODE == (sinOR2|sin23) ||
	   lpsItem->CODE == (sinOR2|sin24) || lpsItem->CODE == (sinOR2|sin25))
	{
		// �����ð� + 5�� < ����ð�
		if( lpsItem->dwCreateTime+(60*60*24*5) < NowTime )
			return TRUE;
	}

	//�ۼ��� : �����, �ۼ��� : 09.12.08
	//��  �� : ��Ÿ ��, ��Ÿ �ƹķ� ������ 3�� �Ŀ� ����
	if( lpsItem->CODE == (sinOR2|sin27) || lpsItem->CODE == (sinOA1|sin32) )  // ��Ÿ ��, ��Ÿ �ƹķ� �߰�
	{
	//	if( lpsItem->dwCreateTime+0 < NowTime ) // ����� : ��Ÿ��, ��Ÿ�ƹķ��� �ͽ��ĵǸ鼭 �ð��� �ʱ�ȭ �Ǿ� ������ ������Ű�� ����.
		// �����ð� + 3�� < ����ð�
		if( lpsItem->dwCreateTime+(60*60*24*3) < NowTime ) 
			return TRUE;
	}

	//�ۼ��� : �����, �ۼ��� : 09.07.31
	//��  �� : �̺�Ʈ ��, �̺�Ʈ �ƹķ� ������ 7�� �Ŀ� ����
	if( lpsItem->CODE == (sinOR2|sin28) || lpsItem->CODE == (sinOA1|sin33) )  // �̺�Ʈ ��, �̺�Ʈ �ƹķ� �߰�
	{
		// �����ð� + 7�� < ����ð�
		if( lpsItem->dwCreateTime+(60*60*24*7) < NowTime ) 
			return TRUE;
	}

	//�ۼ��� : �����, �ۼ��� : 09.12.17
	//��  �� : �̺�Ʈ ��, �̺�Ʈ �ƹķ� ������ 1�ð� �Ŀ� ����
	if( lpsItem->CODE == (sinOR2|sin29) || lpsItem->CODE == (sinOA1|sin34) )  // �̺�Ʈ ��, �̺�Ʈ �ƹķ� �߰�
	{
		//	if( lpsItem->dwCreateTime+0 < NowTime ) // ����� - �̺�Ʈ��, �̺�Ʈ�ƹķ� ������ ������Ű�� ����.
		//		return TRUE;
		// �����ð� + 1�ð� < ����ð�
		if( lpsItem->dwCreateTime+(60*60) < NowTime ) 
			return TRUE;
	}

	//�ۼ��� : �����, �ۼ��� : 09.12.17
	//��  �� : �̺�Ʈ ��, �̺�Ʈ �ƹķ� ������ 1�� �Ŀ� ����
	if( lpsItem->CODE == (sinOR2|sin30) || lpsItem->CODE == (sinOA1|sin35) )  // �̺�Ʈ ��, �̺�Ʈ �ƹķ� �߰�
	{
		//	if( lpsItem->dwCreateTime+0 < NowTime ) // ����� - �̺�Ʈ��, �̺�Ʈ�ƹķ� ������ ������Ű�� ����.
		//		return TRUE;
		// �����ð� + 1�� < ����ð�
		if( lpsItem->dwCreateTime+(60*60*24) < NowTime ) 
			return TRUE;
	}

	//�ۼ��� : �庰, �ۼ��� : 10.02.02
	//��  �� : ��Ʈ�� ������ 7�� �Ŀ� ����
	if( lpsItem->CODE == (sinOR2|sin33) )  // ��Ʈ�� �߰�
	{
		//	if( lpsItem->dwCreateTime+0 < NowTime ) // ����� - �̺�Ʈ��, �̺�Ʈ�ƹķ� ������ ������Ű�� ����.
		//		return TRUE;
		// �����ð� + 7�� < ����ð�
		if( lpsItem->dwCreateTime+(60*60*24*7) < NowTime ) 
			return TRUE;
	}


	// 7��
	if(	lpsItem->CODE == ( sinOA1|sin36 ) || lpsItem->CODE == ( sinOA1|sin37) || // �庰 - ���� �ƹķ�, ��Ʈ �ƹķ�
		lpsItem->CODE == ( sinOR2|sin36 ) || lpsItem->CODE == ( sinOR2|sin37) || // �庰 - �ҿｺ��
		lpsItem->CODE == ( sinOR2|sin38 ) || lpsItem->CODE == ( sinOR2|sin39) ||
		lpsItem->CODE == ( sinOR2|sin40 ) )
	{
		if( lpsItem->dwCreateTime+(60*60*24*7) < NowTime )
			return TRUE;
	}

	// �庰 - �ҿｺ�� 14�� // �庰 - ���� �̺�Ʈ �ƹķ�, ��
	if(	lpsItem->CODE == ( sinOA1|sin39) ||
		lpsItem->CODE == ( sinOA1|sin40 ) || lpsItem->CODE == ( sinOA1|sin41) ||
		lpsItem->CODE == ( sinOA1|sin42 ) || 
		lpsItem->CODE == ( sinOA1|sin38 ) || lpsItem->CODE == ( sinOR2|sin34) || lpsItem->CODE == ( sinOR2|sin35 ) )
	{
		if( lpsItem->dwCreateTime+(60*60*24*14) < NowTime )
			return TRUE;
	}

	

	//(�������� , ������� ��¥ Ȯ���Ͽ� ����)
	if ( lpsItem->CODE!=(sinOR2|sin01) &&
		dwItemCode!=sinPZ1 && dwItemCode!=sinPZ2 ) 
		return FALSE;

#ifdef _LANGUAGE_THAI_DELETE_RING
		if(lpsItem->CODE==(sinOR2|sin01)) return TRUE;
#endif

	tm.tm_year = 2004 - 1900;	// 2004��
	tm.tm_mon = 4-1;			// 4��
	tm.tm_mday = 1;			// 1��
	tm.tm_hour = 15;			// 15��
	tm.tm_min = 30;				// 30��
	tm.tm_sec = 0;				// 0��
	ttm = mktime( &tm );		//���� �ð�

	dwCreateTime = (DWORD)ttm;

	if ( lpsItem->dwCreateTime<dwCreateTime ) return TRUE;

	return FALSE;
}

int RecvSodGameInfomation( void *Info )
{
	smTRANS_COMMAND_SOD	*lpTransCommand = (smTRANS_COMMAND_SOD *)Info;

	if ( !BellatraEffectInitFlag ) {
		CreateBellatraFontEffect();
		BellatraEffectInitFlag = TRUE;
	}

	switch( lpTransCommand->smTransCommand.WParam ) {
		case 1:
			if ( SoD_SetFontEffect.eBL_Type==E_BL_LODING ) {
				CheckBellatraFontEffect( &SoD_SetFontEffect , TRUE );	//�ε� ����
			}

			if ( lpTransCommand->smTransCommand.SParam<0 ) {
				//SoD ���� ����Ʈ
				//Sod_SodEffect( lpTransCommand->smTransCommand.LParam+1 );
				sSodScore.EffectStartCount = 70*3;
				sSodScore.EffectCode = lpTransCommand->smTransCommand.LParam+1;
			}
			else {
				if ( lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_DEAD ) break;		//������� ����

				sSodScore.SodNextStageNum = lpTransCommand->smTransCommand.SParam;
				sSodScore.dwSoD_NextStageTime = dwPlayTime+9000;
				sSodScore.dwSoD_CloseStageTime = dwPlayTime+5000;
				sSodScore.NextRound = lpTransCommand->smTransCommand.LParam+1;

				StopBGM();
				esPlayContSound( 14 );
				lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_YAHOO);

				SetBellatraFontEffect( E_BL_FONT_STAGE );
				SetBellatraFontEffect( E_BL_FONT_COMPLETE );
				SoDGateFlag = TRUE;
			}
			break;

		case 3:
			//���� ���ư���
			if ( lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_DEAD ) break;		//������� ����

			sSodScore.SodNextStageNum = -1;
			sSodScore.dwSoD_NextStageTime = dwPlayTime+8000;
			sSodScore.dwSoD_CloseStageTime = dwPlayTime+6000;
			sSodScore.NextRound = -1;

			StopBGM();
			esPlayContSound( 15 );

			SetBellatraFontEffect( E_BL_FONT_STAGE );
			SetBellatraFontEffect( E_BL_FONT_FAIL );
			SoDGateFlag = TRUE;
			break;

		case 4:
			//��� Ŭ����
			if ( lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_DEAD ) break;		//������� ����

			sSodScore.SodNextStageNum = lpTransCommand->smTransCommand.SParam;
			sSodScore.dwSoD_NextStageTime = dwPlayTime+7000;
			sSodScore.NextRound = lpTransCommand->smTransCommand.LParam+1;

			StopBGM();
			esPlayContSound( 16 );
			lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_YAHOO);

			//SetBellatraFontEffect( E_BL_FONT_STAGE );
			SetBellatraFontEffect( E_BL_FONT_CON );
			SoDGateFlag = FALSE;

			//ȹ���� �ݾ� ���
			char szBuff[128];
			wsprintf( szBuff , mgSOD_Clear , lpTransCommand->smTransCommand.EParam );
			cMessageBox.ShowMessageEvent(szBuff);

			break;

		case 2:
			//���� ��� ����
			if ( lpTransCommand->smTransCommand.LParam ) {
				//������ ȹ�濡 ���� ���� ����
				sSodScore.ScoreEffectCount = 255;

			}
			else {
				//�ֱ������� �������� ��ü ����


			}

			if ( !sSodScore.dwSoD_NextStageTime ) {
				if ( sSodScore.Round!=lpTransCommand->smTransCommand.SParam ) {
					switch( lpTransCommand->smTransCommand.SParam ) {
					case 1:		//1���� ����
						sSodScore.dwPlayTime = dwPlayTime;
						PlayBGM_Direct( BGM_CODE_SOD1 );
						break;

					case 4:
						PlayBGM_Direct( BGM_CODE_SOD2 );
						break;

					case 7:
						PlayBGM_Direct( BGM_CODE_SOD3 );
						break;
					}
				}
				sSodScore.Round  = lpTransCommand->smTransCommand.SParam;
			}

			sSodScore.dwDispTime = dwPlayTime+10*1000;
			sSodScore.MyTeam = lpTransCommand->MyTeam;
			sSodScore.Score  = lpTransCommand->MyScore;
			sSodScore.TeamScore[0] = lpTransCommand->TeamScore[0];
			sSodScore.TeamScore[1] = lpTransCommand->TeamScore[1];
			sSodScore.TeamScore[2] = lpTransCommand->TeamScore[2];
			sSodScore.TeamScore[3] = lpTransCommand->TeamScore[3];

			break;

		case smCODE_SOD_EFFECT:					//�ֹ� �ý��� �߰� - SOD ����Ʈ ����
			if ( !BellatraEffectInitFlag ) {
				CreateBellatraFontEffect();
				BellatraEffectInitFlag = TRUE;
			}
			SetBellatraFontEffect((EBL_FontEffectType)lpTransCommand->smTransCommand.LParam);
			break;

	}

	return TRUE;
}

//�������� ������ �Լ�
int RecvForceOrbItem( TRANS_ITEMINFO_GROUP2 *lpTransItemGroup2 )
{
	TRANS_ITEMINFO_GROUP	TransItemGroup;

	ZeroMemory( &TransItemGroup , sizeof(TRANS_ITEMINFO_GROUP) );
	memcpy( &TransItemGroup , lpTransItemGroup2 , TRANS_GROUPITEM_HEADER_SIZE );

	DecodeCompress( (BYTE *)lpTransItemGroup2->szBuff , (BYTE *)TransItemGroup.sItemInfo , sizeof(sITEMINFO)*TRANS_GROUPITEM_MAX );

	//�ڻ�� �Լ� ȣ��
	//���� �������� �޴´�
	sinRecvForceOrb(TransItemGroup.sItemInfo , TransItemGroup.ItemCount);


	return TRUE;
}

//����Ʈ ��� ������
int SendQuestCommandToServer( DWORD dwQuestCode , int Param1, int Param2, int Param3 )
{

	smTRANS_COMMAND	smTransCommand;
	int	result = 0;
	smWINSOCK *lpsmSock;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = 	smTRANSCODE_QUEST_COMMAND;
	smTransCommand.WParam = dwQuestCode;
	smTransCommand.LParam = Param1;
	smTransCommand.SParam = Param2;
	smTransCommand.EParam = Param3;

	if ( dwQuestCode>=HAQUEST_CODE_ELEMENTARY_A && dwQuestCode<=HAQUEST_CODE_FURYOFPHANTOM ) 
	{
		lpsmSock = smWsockDataServer;
		if ( lpsmSock ) 
			result = lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

		return result;
	}

	lpsmSock = smWsockServer;
	if ( lpsmSock ) 
		result = lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return result;
}

//���� ����Ʈ ����
int Start_QuestArena( DWORD dwQuestCode , int Param1, int Param2 )
{
	smTRANS_COMMAND	smTransCommand;
	int	result = 0;
	smWINSOCK *lpsmSock;

	//if ( dwQuestCode==SIN_QUEST_CODE_CHANGEJOB4 ) {

		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = 	smTRANSCODE_QUEST_COMMAND;
		smTransCommand.WParam = dwQuestCode;
		smTransCommand.LParam = QUEST_ARENA_FIELD;
		smTransCommand.SParam = Param1;
		smTransCommand.EParam = Param2;

		lpsmSock = GetAreaServerSock();
		if ( lpsmSock ) 
			result = lpsmSock->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

		if ( result )
			WarpField2( QUEST_ARENA_FIELD ); 

	//}

	return FALSE;
}

//����ĳ�� �������� ������ ����
int	SendBlessCastleToServer( smTRANS_BLESSCASTLE *lpBlessCastleSetup , int Mode )
{
	smWINSOCK *lpsmSock;

	lpBlessCastleSetup->smTransCommand.size = sizeof(smTRANS_BLESSCASTLE);
	lpBlessCastleSetup->smTransCommand.code = smTRANSCODE_BLESSCASTLE_INFO;
	lpBlessCastleSetup->smTransCommand.WParam = 1;
	lpBlessCastleSetup->smTransCommand.LParam = Mode;
	lpBlessCastleSetup->smTransCommand.SParam = 0;
	lpBlessCastleSetup->smTransCommand.EParam = 0;

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) 
		return lpsmSock->Send2( (char *)lpBlessCastleSetup , lpBlessCastleSetup->smTransCommand.size , TRUE );

	return FALSE;
}

//����ĳ�� ���� ���� ������ �䱸
int	Send_GetBlessCastleTax()
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory( &smTransCommand , sizeof(smTRANS_COMMAND) );

	smTransCommand.code = smTRANSCODE_BLESSCASTLE_INFO;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.SParam = cSinSiege.GetTaxRate();

	if ( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return FALSE;
}
//����ĳ�� �������� ����
int RecvBlessCastInfo( void *lpPacket )
{
	smTRANS_BLESSCASTLE *lpBlessCastleSetup=(smTRANS_BLESSCASTLE *)lpPacket;
	rsUSER_LIST_TOP10	*lpClanListTop10;

	switch( lpBlessCastleSetup->smTransCommand.WParam ) {
		case 1:		//�������� ����
			chaSiege.ShowSiegeMenu( lpBlessCastleSetup );
			break;

		case 2:		//���� ���� ����
			lpClanListTop10 = (rsUSER_LIST_TOP10 *)lpPacket;
			//���� ��� �Լ� ȣ��
			chaSiege.ShowSiegeScore( lpClanListTop10 );
			break;
	}

	return TRUE;
}

//���� ĳ�� ������ Ŭ�� ����
int SetBlessCastleMaster( DWORD dwClanCode , int Mode )
{

	if ( !Mode && lpCurPlayer->OnStageField>=0 && StageField[lpCurPlayer->OnStageField]->FieldCode==rsCASTLE_FIELD ) {

	}
	else  {
		if ( rsBlessCastle.dwMasterClan!=dwClanCode) {
			//ĳ�� ������ ������Ʈ
			UpdateCastleMasterClan( rsBlessCastle.dwMasterClan );
		}

		rsBlessCastle.dwMasterClan = dwClanCode;
	}

	return TRUE;
}

//�Ӽ����� ������ ����
int	SendResistanceToServer()
{
	smTRANS_RESISTANCE	smTransResistance;
	smWINSOCK *lpsmSock;

	dwResistance_SendingTime = dwPlayTime;

	smTransResistance.code = smTRANSCODE_RESISTANCE;
	smTransResistance.size = sizeof(smTRANS_RESISTANCE);
	smTransResistance.Param = 0;
	memcpy( smTransResistance.Resistance , lpCurPlayer->smCharInfo.Resistance , sizeof(short)*8 );
	smTransResistance.Absorb = lpCurPlayer->smCharInfo.Absorption;

	lpsmSock = GetAreaServerSock();
	if ( lpsmSock ) 
		return lpsmSock->Send2( (char *)&smTransResistance , smTransResistance.size , TRUE );

	return FALSE;
}

//�������� ��� ������ ������
int	SendPublicPollingToServer( int PollCode , int PollCnt , BYTE *bPollingData )
{
	smTRANS_COMMAND_POLLING	smTransPolling;

	ZeroMemory( &smTransPolling , sizeof(smTRANS_COMMAND_POLLING) );

	smTransPolling.smTransCommand.code = smTRANSCODE_PUBLIC_POLLING;
	smTransPolling.smTransCommand.size = sizeof( smTRANS_COMMAND_POLLING );
	smTransPolling.smTransCommand.WParam = PollCode;
	smTransPolling.smTransCommand.LParam = PollCnt;
	memcpy( smTransPolling.bPolling , bPollingData , PollCnt ); 

	if ( smWsockDataServer )
		smWsockDataServer->Send2( (char *)&smTransPolling , smTransPolling.smTransCommand.size , TRUE );


	return TRUE;
}


#ifdef _xTrap_GUARD
	#include "XTrapSrc\\XTrap.h"
	//����Ʈ�� ��Ŷ����
	int XTrap_Recv( smTRANS_COMMAND *lpPacket , smWINSOCK *lpsmSock )
	{
		smTRANS_XTRAP_ADDR		*lpTransXTrapAddr;
		smTRANS_XTRAP_SECURITY	*lpTransXTrapSecurity;
		UCHAR KFBuf[CREATEKF_OUT_SIZE*2] = {0,};

		//FILE *fp;
		//char szBuff[256];


		switch( lpPacket->WParam ) 
		{
		case smXTRAP_COMMAND_LOGIN:
			lpTransXTrapAddr = (smTRANS_XTRAP_ADDR *)lpPacket;
			lpTransXTrapSecurity = (smTRANS_XTRAP_SECURITY *)lpPacket;

			CreateKFEx(NULL, (PUCHAR)lpTransXTrapAddr->Addr, 2, KFBuf);

			lpTransXTrapSecurity->smTransCommand.size = sizeof(smTRANS_XTRAP_SECURITY);
			lpTransXTrapSecurity->smTransCommand.code = smTRANSCODE_XTRAP_PACKET;
			lpTransXTrapSecurity->smTransCommand.WParam = smXTRAP_COMMAND_SECURITY;
			memcpy( &lpTransXTrapSecurity->serialkey, KFBuf, CREATEKF_OUT_SIZE*2);
			lpsmSock->Send2( (char *)lpTransXTrapSecurity , lpTransXTrapSecurity->smTransCommand.size , TRUE );

			break;

		case smXTRAP_COMMAND_SECURITY:
			if ( lpPacket->LParam==0 ) SendLogToMgr();
			DisconnectServerCode = 4;
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 30");
			DisconnectFlag = GetCurrentTime();
			break;

		}
		return TRUE;
	};
#else
	#ifdef _XTRAP_GUARD_4_CLIENT
		#include "XTrapSrcD5\Client\XTrap4Client.h"	//XTrapD5
//		#include "XTrapSrc4\XTrap.h"
		//����Ʈ�� ��Ŷ����
		int XTrap_Recv( smTRANS_COMMAND *lpPacket , smWINSOCK *lpsmSock )
		{
			char debug[512];
			smTRANS_XTRAP_ADDR		*lpTransXTrapAddr = (smTRANS_XTRAP_ADDR *)lpPacket;
			
			sprintf(debug,"XTrap_Recv");
			OutputDebugString(debug);

			if(lpPacket->WParam == XTRAP_CMD_SEEDKEY)
			{//�������� ��Ŷ�� ������
				sprintf(debug,"CMD_SEEDKEY");
				OutputDebugString(debug);
			
				sprintf( debug, "before : Packet Buf: %02X %02X %02X %02X %02X %02X", 
					lpTransXTrapAddr->PacketBuf[0],
					lpTransXTrapAddr->PacketBuf[1],
					lpTransXTrapAddr->PacketBuf[2],
					lpTransXTrapAddr->PacketBuf[3],
					lpTransXTrapAddr->PacketBuf[4],
					lpTransXTrapAddr->PacketBuf[5] );
				OutputDebugString( debug );
				
				//----[ 1. ������ ���� ���� ���� �˻� ]
				smTRANS_XTRAP_ADDR	smTransXTrapAddr;
				memset(&smTransXTrapAddr,0,sizeof(smTransXTrapAddr));

				unsigned int usResult = 1;
				//usResult = XTrapCC_STEP2( (PUCHAR)lpTransXTrapAddr->PacketBuf, (PUCHAR)smTransXTrapAddr.PacketBuf, XTRAP_PROTECT_PE|XTRAP_PROTECT_TEXT);
				usResult = XTrap_CS_Step2( (PUCHAR)lpTransXTrapAddr->PacketBuf, (PUCHAR)smTransXTrapAddr.PacketBuf,
					XTRAP_PROTECT_PE|XTRAP_PROTECT_TEXT|XTRAP_PROTECT_EXCEPT_VIRUS);
				
				//----[ 2. ��� ���� ������ ������ ]
				smTransXTrapAddr.smTransCommand.size = sizeof(smTRANS_XTRAP_ADDR);
				smTransXTrapAddr.smTransCommand.code = smTRANSCODE_XTRAP_PACKET;
				smTransXTrapAddr.smTransCommand.WParam = XTRAP_CMD_UNIQKEY;

				if ( smWsockDataServer ) 
					smWsockDataServer->Send( (char *)&smTransXTrapAddr , smTransXTrapAddr.smTransCommand.size , TRUE );		

				//----[ 3. �Լ� ���ϰ� �˻� ]
				if( usResult == 0 )
				{//����
				}
				else
				{//������
					DisconnectServerCode = 4;
					Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 31");
					DisconnectFlag = GetCurrentTime();
				}

				sprintf( debug, "after : Packet Buf: %02X %02X %02X %02X %02X %02X", 
					lpTransXTrapAddr->PacketBuf[0],
					lpTransXTrapAddr->PacketBuf[1],
					lpTransXTrapAddr->PacketBuf[2],
					lpTransXTrapAddr->PacketBuf[3],
					lpTransXTrapAddr->PacketBuf[4],
					lpTransXTrapAddr->PacketBuf[5] );
				OutputDebugString( debug );

			}

			return TRUE;
		};

		VOID __stdcall XTrapCS_SendUniqKey	(PUCHAR pUniqKey)
		{}

	#else
		//����Ʈ�� ��Ŷ����
		int XTrap_Recv( smTRANS_COMMAND *lpPacket , smWINSOCK *lpsmSock )
		{
			return TRUE;
		}

	#endif
#endif



int	HackTrap_OpenFlagMask = 0;

//���� ýũ�� NPC��� ������ ������ �˻��뺸
int HackTrap_CheckOpenFlag()
{
	int ChkCnt = 0;
	
	// ����� - ������忡�� �̵� ���� ������ ���ÿ��� ��ŷ �α׸� �ȳ����. cShop.UseItemFlag==0
	if ( cShop.OpenFlag && cShop.UseItemFlag==0) {								//���� 
		if ( (HackTrap_OpenFlagMask&sinNPC_SHOP)==0 ) {
			HackTrap_OpenFlagMask|=sinNPC_SHOP;
			SendHackTrapToServer(100,sinNPC_SHOP);
		}
		ChkCnt++;
	}

	if (  cWareHouse.OpenFlag  ) {						//â�� 
		if ( (HackTrap_OpenFlagMask&sinNPC_WARE)==0 ) {
			HackTrap_OpenFlagMask|=sinNPC_WARE;
			SendHackTrapToServer(100,sinNPC_WARE);
		}
		ChkCnt++;
	}
	if ( cCraftItem.OpenFlag ){							// ���� 
		if( cCraftItem.ForceFlag )return false;

		if ( (HackTrap_OpenFlagMask&sinNPC_MIX)==0 ) {
		HackTrap_OpenFlagMask|=sinNPC_MIX;
		SendHackTrapToServer(100,sinNPC_MIX);
		}
	
		ChkCnt++;
	}													//���� ���� 
	if( cCraftItem.ForceFlag ) {
		if(  cCraftItem.OpenFlag )return false;

		if ( (HackTrap_OpenFlagMask&sinNPC_FORCE)==0 ) {
			HackTrap_OpenFlagMask|=sinNPC_FORCE;
			SendHackTrapToServer(100,sinNPC_FORCE);
		} 
		ChkCnt++;
	}
	if (   cAging.OpenFlag ) {							//����¡
		if ( (HackTrap_OpenFlagMask&sinNPC_AGING)==0 ) {
			HackTrap_OpenFlagMask|=sinNPC_AGING;
			SendHackTrapToServer(100,sinNPC_AGING);
		}
		ChkCnt++;
	}

	// pluto ����
	if( SmeltingItem.OpenFlag ) // pluto NPC �߰� �Ҷ� HackTrap.h ��⵵ �߰��ض� (�ʵ�NPC��� ���)
	{
		if( (HackTrap_OpenFlagMask&sinNPC_SMELTING)==0 )
		{
			HackTrap_OpenFlagMask|=sinNPC_SMELTING;
			SendHackTrapToServer(100,sinNPC_SMELTING);
		}
		ChkCnt++;
	}
	// pluto ����
	if( ManufactureItem.m_OpenFlag )
	{
		if( (HackTrap_OpenFlagMask&sinNPC_MANUFACTURE)==0 )
		{
			HackTrap_OpenFlagMask|=sinNPC_MANUFACTURE;
			SendHackTrapToServer(100,sinNPC_MANUFACTURE);
		}
		ChkCnt++;
	}

	// ������ - �ͽ��� ���� 
	if( cMixtureReset.OpenFlag )
	{
		if( ( HackTrap_OpenFlagMask & sinNPC_MIXTURE_RESET ) == 0 )
		{
			HackTrap_OpenFlagMask |= sinNPC_MIXTURE_RESET;
			SendHackTrapToServer( 100, sinNPC_MIXTURE_RESET );
		}

		ChkCnt++;
	}

	if (  SkillMasterFlag ) {							//��ų������
		if ( (HackTrap_OpenFlagMask&sinNPC_SKILL)==0 ) {
			HackTrap_OpenFlagMask|=sinNPC_SKILL;
			SendHackTrapToServer(100,sinNPC_SKILL);
		}
		ChkCnt++;
	}
   
	if ( !ChkCnt )
		HackTrap_OpenFlagMask = 0;

	return	TRUE;
}


//���� ýũ�� ����
int	SendHackTrapToServer( DWORD dwTrapCode , int Param )
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory( &smTransCommand , sizeof(smTRANS_COMMAND) );

	smTransCommand.code = smTRANSCODE_HACKTRAP;
	smTransCommand.size = sizeof( smTRANS_COMMAND );
	smTransCommand.WParam = dwTrapCode;
	smTransCommand.LParam = Param;

	if ( dwTrapCode==100 ) {
		//NPC ��ɱ����� ȣ��
		if ( lpCurPlayer->OnStageField>=0 ) {
			smTransCommand.SParam = StageField[lpCurPlayer->OnStageField]->FieldCode;
		}
		else
			return FALSE;
	}

	if ( smWsockDataServer )
		smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );


	return TRUE;
}

// pluto ���� ����Ÿ�� ������ ����
int SendSmeltingItemToServer( void *lpsSmeltingItem_Send )
{
	sSMELTINGITEM_SERVER	*lpSmeltingItem_Send = ( sSMELTINGITEM_SERVER *)lpsSmeltingItem_Send;

	lpSmeltingItem_Send->size = sizeof( sSMELTINGITEM_SERVER);
	lpSmeltingItem_Send->code = smTRANSCODE_SMELTINGITEM;

	if( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)lpSmeltingItem_Send, lpSmeltingItem_Send->size, TRUE );

	return FALSE;
}
// pluto ���� ����Ÿ�� ������ ����
int SendManufactureItemToServer( void *lpsManufactureItem_Send )
{
	SManufactureItem_Server *lpManufactureItem_Send = ( SManufactureItem_Server *)lpsManufactureItem_Send;

	lpManufactureItem_Send->size = sizeof( SManufactureItem_Server );
	lpManufactureItem_Send->code = smTRANSCODE_MANUFACTURE;

	if( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)lpManufactureItem_Send, lpManufactureItem_Send->size, TRUE );

	return FALSE;
}

// ������ - �ͽ��� ���� �������� ������ �����Ѵ�.
int SendMixtureResetItemToServer( void *lpsMixtureResetItem_Send )
{
	// �������� ������ ����
	sMIXTURE_RESET_ITEM_SERVER *lpMixtureResetItem_Send = (sMIXTURE_RESET_ITEM_SERVER *)lpsMixtureResetItem_Send;

	// ������� �ͽ��� ������ ����� �ڵ带 ����
	lpMixtureResetItem_Send->size	= sizeof( sMIXTURE_RESET_ITEM_SERVER );
	lpMixtureResetItem_Send->code	= smTRANSCODE_MIXTURE_RESET_ITEM;

	// ������ ������ ������!!!
	if( smWsockDataServer )
		return smWsockDataServer->Send2( (char *)lpMixtureResetItem_Send, lpMixtureResetItem_Send->size, TRUE );

	return FALSE;
}

#ifdef _XIGNCODE_CLIENT
// ����� - XignCode

int Xigncode_Client_Start()
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory( &smTransCommand , sizeof(smTRANS_COMMAND) );

	smTransCommand.code = smTRANSCODE_XIGNCODE_START;
	smTransCommand.size = sizeof( smTRANS_COMMAND );
	smTransCommand.WParam = TRUE;

	if ( smWsockDataServer )
		smWsockDataServer->Send2( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return TRUE;
}


XIGNCODE_PACKET return_packet;

int Xigncode_Client_Recv(smWINSOCK *lpsmSock, XIGNCODE_PACKET *pack)
{
//	XIGNCODE_PACKET return_packet;
	memset(&return_packet, 0, sizeof(return_packet));

	if( ZCWAVE_Probe((char *)pack->data, (char *)return_packet.data, sizeof(return_packet.data)))
	{
		return_packet.size = sizeof(return_packet);
		return_packet.code = smTRANSCODE_XIGNCODE_PACKET;
		lpsmSock->Send2( (char *)&return_packet, return_packet.size, TRUE);
	}
	
	return TRUE;
}
#endif
/*
struct smTRANS_COMMAND_POLLING {
	smTRANS_COMMAND		smTransCommand;
	BYTE	bPolling[10];
};
//��������
#define	smTRANSCODE_PUBLIC_POLLING		0x50320A70
//������� �ڽ�
#define	smTRANSCODE_NOTICE_BOX			0x50320A80
*/
