int InitLogSql();
int CloseLogSql();
int RecordLogSql( DWORD	dwRecCode , char *szIP , char *lpUserID , char *lpCharName  , char *szGPCode, int Race , int JobCode , INT64 Exp64 , DWORD dwLevel , DWORD dwMoney );
int RecordGameLogSql( DWORD	dwRecCode , char *szIP , char *lpUserID , char *lpCharName  , char *szGPCode, int JobCode , char *szGuid , int BlockTime );

#define		LOG_SQL_LOGIN			0
#define		LOG_SQL_NEWCHAR			1
#define		LOG_SQL_DELCHAR			2
#define		LOG_SQL_LOGOUT			3


#define		GAMELOG_SQL_LOGIN		0x100
#define		GAMELOG_SQL_LOGOUT		0x101

#define		GAMELOG_SQL_SOD			0x102
#define		GAMELOG_SQL_STARPOINT	0x104
#define		GAMELOG_SQL_GIVEMONEY	0x106
#define		GAMELOG_SQL_CLANMONEY	0x107
#define		GAMELOG_SQL_SIEGETAX	0x108
#define		GAMELOG_SQL_POLLRECORD	0x109

#define		GAMELOG_SQL_EVENTJOIN	0x110

#define		BILLING_SQL_LOGOUT		0x201
#define		BILLING_SQL_PCUSE		0x202
#define		BILLING_SQL_PC_SRESERV	0x203
#define		BILLING_SQL_PC_TIME		0x204
#define		BILLING_SQL_BILL_CHANGE	0x205
#define		BILLING_SQL_EVENT_LOGIN		0x206
#define		BILLING_SQL_EVENT_LOGOUT	0x207

#define		BILLING_DISPLAY			0x210


#define		BILLING_URS_EXPINED		0x400

#define		CLAN_USER_TICKET		0x801
#define		CLAN_INSERT_POINT		0x802
#define		CLAN_GET_MARKCODE		0x803

#define		EGT_GET_EXPGAMETIME		0x901

#define		LOAD_DLL_NONE			0
#define		LOAD_DLL_SQL			1
#define		LOAD_DLL_URS			2

extern		int UseDLL_Code;			//��� DLL �ڵ�
extern		int	UseDLL_Expined;			//���� Expined ���

//extern HINSTANCE hLogSqlDLL;					// Handle to DLL
int	bSql_LogOn(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *szGuid , char *lpszPCRNo , char *lpszGubunCode );
int	bSql_Logout(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t DisuseDay, int UsePeriod, char *szGuid , int PCRNo);
int	bSql_EventLogon( rsPLAYINFO *lpPlayInfo );	//�̺�Ʈ ����
int	bSql_EventLogOut( rsPLAYINFO *lpPlayInfo );	//�̺�Ʈ �α׾ƿ�

//#ifdef _LANGUAGE_VEITNAM
int	bSql_GetExpGameTime( rsPLAYINFO *lpPlayInfo , int GamePlayTime );		//ExpGameTime ��Ϲ� ���� (��Ʈ��)
//#endif

int	bSql_GetGPCode(char *GPCode, char *PG);
int	bSql_PCUse( rsPLAYINFO *lpPlayInfo );
int	bSql_PCSearchReserv( rsPLAYINFO *lpPlayInfo );		//���� ���� Ȯ��
int	bSql_ChangeUserIdGpCodeGame( int PCRNo );	//���� ����

int	bSql_PCRNoReservProduct( rsPLAYINFO *lpPlayInfo );	//PC�� �߰� �ð� ǥ��
int	bSql_GetQueLogOut( char *szID );				//������� ť�� �ش� ������ �α� �ƿ��� �ִ��� Ȯ��
int	bSql_SODRecord( rsPLAYINFO *lpPlayInfo , int Level , int SodScore , int SodKillCount , int SodTotalScore , int SodTotalSucessUser , int SodTotalUser , int SodCode );
int	bSql_StarPoint( rsPLAYINFO *lpPlayInfo , int StarPoint , int CashFlag );	//Star Point ��Ϲ� ����
int	bSql_GiveMoney( rsPLAYINFO *lpPlayInfo , int Money );		//GiveMoney ���
int	bSql_PollRecord( rsPLAYINFO *lpPlayInfo , int PollKind , int PollCount , BYTE *PollData );//Polling ��Ϲ� ����
int	bSql_ClanMoney( rsPLAYINFO *lpPlayInfo , int Cash , int CashFlag , int Kind );	//SOD ���� Ŭ�� ��â�� 
int	bClan_GetClanMarkCode( rsPLAYINFO *lpPlayInfo );								//Ŭ�� ��ũ �ڵ� ���ϱ�

int	bSql_EventJoin( rsPLAYINFO *lpPlayInfo );	//�̺�Ʈ ����
int	bSql_RecordCurrency();						//��ȭ���� ���� DB�� ���

int b_SqlGetLossCount();
int b_SqlGetQueSize();
int b_SqlGetQueProcessTime( DWORD *pTimeMax , DWORD *pAvgTime );

int	bUrs_LoginIP(char *lpszPID, char *lpszPWD, char *lpszIP );
int	bUrs_Expined( rsPLAYINFO *lpPlayInfo );

int	bSql_ServerDown();
int	bSql_ServerExit();



struct	_LogITEM {
	DWORD	dwCode;
	DWORD	dwINo[2];
};


//������ ����ü
struct LogITEM {
	int 		size;			// ����� ����ü�� ũ��
	char 		UserID[32];  		// ����� ID
	char 		CharName[24]; 		// ����� ĳ���� �̸�
	long 		IP; 			// ����� IP            <===== �߰�
	int 		Flag;			// ������ ����
	int		ItemCount;		// ó���� ������ ����
	_LogITEM	Item[32];		// ������ ����
};

//�ŷ� ������ ó�� ����ü
struct LogITEM_TRADE {
	int 		size;			// ����� ����ü�� ũ��
	char 		UserID[32];  		// ����� ID
	char 		CharName[24]; 		// ����� ĳ���� �̸�
	long		IP;			// ����� IP         <========= �߰� 
	int 		Flag;			// ������ ����

	char 		TUserID[32];		// �ŷ� ���̵�
	char 		TCharName[24];		// �ŷ� ĳ���� �̸�
	long 		TIP;			// �ŷ� IP           <========= �߰� 
	int		Money;			// �ŷ��� �ݾ�
	int		ItemCount;		// �ŷ��� ������ ����
	_LogITEM 	Item[32];		// ������ ����
};


//������ ����ü
struct LogGiftITEM {
	int 		size;		// ����� ����ü�� ũ��
	char 		UserID[32];  	// ����� ID
	char 		CharName[24]; 	// ����� ĳ���� �̸�
	long 		IP; 		// ����� IP
	int 		Flag;		// ������ ����    (�����۱���13)
	long		GNo;		// ������ȣ
	char 		TEL[16]; 	// ����� ��ȭ��ȣ
	_LogITEM 	Gitem;		// ������ ����
};

/*
//������ ����ü
struct LogITEM {
	int 		size;		// ����� ����ü�� ũ��
	char 		UserID[32];  	// ����� ID
	char 		CharName[24]; 	// ����� ĳ���� �̸�
	int 		Flag;		// ������ ����
	int		ItemCount;	// ó���� ������ ����
	_LogITEM	Item[32];	// ������ ����
};

//�ŷ� ������ ó�� ����ü
struct LogITEM_TRADE {
	int 		size;		// ����� ����ü�� ũ��
	char 		UserID[32];  	// ����� ID
	char 		CharName[24]; 	// ����� ĳ���� �̸�
	int 		Flag;		// ������ ����

	char 		TUserID[32];  	// ����� ID
	char 		TCharName[24];	// �ŷ� ĳ���� �̸�
	int		Money;		// �ŷ��� �ݾ�
	int		ItemCount;	// �ŷ��� ������ ����
	_LogITEM 	Item[32];	// ������ ����
};
*/

struct	LogITEM_QUE {
	int		LogType;
	char	szBuff[sizeof(LogITEM_TRADE)];			//���� ū ����ü LogITEM_TRADE ũ��
};

#define		LOG_ITEM_QUE_MAX		1024
#define		LOG_ITEM_QUE_MASK		1023


struct	LogEVENT_QUE {
	int		LogType;
	char	szBuff[512];
};
#define		LOG_EVENT_QUE_MAX		256
#define		LOG_EVENT_QUE_MASK		255

//������ �α� ť�� ����Ÿ ����
int b_PopLogItemQue(LogITEM_QUE *lpLogItemQue);
//������ �α� ť�� ����Ÿ ����
int	b_PushLogItemQue( int type , int size,  void *lpLogItemData );

int b_SqlGetItemQueSize();			//������ �α� ť ũ��
int b_SqlGetItemLossCount();		//������ �α� ���� ī����


//�̺�Ʈ �α� ť�� ����Ÿ ����
int	b_PushLogEventQue( int type , int size, void *lpLogEventData );
//�̺�Ʈ �α� ť�� ����Ÿ ����
int b_PopLogEventQue( LogEVENT_QUE *lpLogEventQue);
int b_SqlGetEventQueSize();
int b_SqlGetEventLossCount();


//(����) PC�� ���� IP����
int	b_IPRecord(char *lpszServerName, struct JUNGIP *JungIP);
//(����) PC�� ���� IP����
int	b_IPRecordTime(char *lpszServerName, struct JUNGIP *JungIP);

#define	JUNGIP_MAX	1024

struct JUNGIP {
    int	  IPCount;	// ó���� IP ����
    DWORD IP[JUNGIP_MAX];	// IP ����
};  


//Ŭ�� Ƽ�� ����
int bClan_GetTicket();
//Ŭ�� Ƽ�� ó��
int	bClan_UserTicket( rsPLAYINFO *lpPlayInfo , int gubun );
//Ŭ�� ����Ʈ ���
int	bClan_InsertPoint( rsPLAYINFO *lpPlayInfo , int point );


/////////////////////////////// �߱��� ���� DLL ////////////////////////////////
/*
extern HINSTANCE hUsrLogInDLL;					// Handle to DLL
typedef int (*LPFNUSRLOGIN)(const char *ID,const char *PW, const char *IP );
typedef int (*LPFNUSRLOGOUT)(const char *ID, const char *IP );
typedef int (*LPFNUSRLOGOUT_IP_BLOCK)(const char *ID, const char *IP , const int min );
extern LPFNUSRLOGIN URSLoginIP;				// Function pointer
extern LPFNUSRLOGOUT URSLogoutIP;				// Function pointer
extern LPFNUSRLOGOUT_IP_BLOCK URSLogoutIPBlock;				// Function pointer
*/

//nSprite �߱� �����ڵ�
DWORD nSprite_GetResult( DWORD nRecvNum );
int	nSprite_Init();			//nSprite �ʱ�ȭ

//////////////////////////////////////////////////////////////////////////////
