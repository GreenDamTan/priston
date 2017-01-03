
#ifndef _NET_PLAY_HEADER

#define	_NET_PLAY_HEADER

extern smWINSOCK	*smWsockServer;
extern smWINSOCK	*smWsockDataServer;

extern int ChatScrollPoint;			//ä�� ��ũ�� ����Ʈ
extern int DispChatMsgHeight;			//�ֱ� ǥ�õ� ä��â �� ����
extern int DispChatMode;			//ä�� ��� ���

extern int BellatraEffectInitFlag;	//����Ʈ�� ����Ʈ �ʱ�ȭ �÷�

extern char	szConnServerName[16];	//������ �����̸�

extern int	Version_WareHouse;						//â�� ���嵥��Ÿ ����
extern int	Version_CharInfo;						//ĳ������Ÿ ���嵥��Ÿ ����

extern smTRANS_COMMAND	*lpTransVirtualPotion;		//���� ����

extern DWORD	dwTime_ServerT ;		//���ӽ� �ð� (������ Time_T )
extern DWORD	dwTime_ConnectMS ;		//���ӽ� �ð� (Ŭ���̾�Ʈms)

extern int		InitClanMode;			//Ŭ�� �ʱ�ȭ ���

extern DWORD	dwYahooTime;			//��ȣ �����ð�


//����� ����Ʈ�� ��ų���� ����
extern DWORD	dwSkill_DivineLightning_Target[8];
extern int		DivineLightning_Target_Count;
extern int		DivineLightning_FindCount;


//�ڵ� �÷��̾ ã�´�
smCHAR *FindAutoPlayer( DWORD dwObjectSerial );
// ������ȣ�� ã�´�
smCHAR *FindChrPlayer( DWORD dwObjectSerial );
//������Ƽ������ ã�´�
smCHAR *FindDeadPartyUser();
//�ֱ��� ���͸� ã�´�
smCHAR *FindNearMonster( smCHAR *lpCurChar );


//���� �ڵ� ť�� ����
int ClearAttMonsterCode();
int	AddAttMonsterCode( DWORD dwCode );
int	CheckAttMonsterCode( DWORD dwCode );


//���� �ڵ�� ������ ã��
smWINSOCK *GetServerSock( int ServerCode );
//�������� ������ ã��
smWINSOCK *GetAreaServerSock();

//����Ÿ �Լ�
int RecvPlayData( smTHREADSOCK *pData );
//�޼��� ť�� �޼����� ó��
int PlayRecvMessageQue();

//ä��â ���͸�
int	SetChatMsgFilter( int mode );
//ä�� ���� ȭ�鿡 ǥ��
int DisplayChatMessage( HDC hdc , int x, int y , int MaxLine );
//ä�� ���� ȭ�鿡 ǥ��
int DisplayChatMessage2( HDC hdc , int x, int y , int StrMax , int LineMax );
int DisplayChatMessage3( HDC hdc , int x, int y , int StrMax , int LineMax , int Mode );
//ä�� ���� �ڸǵ� ���� 
int RecallSavedChatCommand( HWND hChatWnd , int Arrow );

//ä�� ���ڸ� ������ ����
int SendChatMessageToServer( char *szMessage );
//�ٸ� ������ ���� ��Ų��
int ConnectOtherPlayer( DWORD dwIP );
//�� �÷��� ����
int NetWorkPlay();
//ä��â�� ���� ����
int AddChatBuff( char *szMsg , DWORD dwIP );
//ä��â�� ���� ����
int AddChatBuff( char *szMsg );
//�ڵ� ��ȣ ýũ�� �����
DWORD EncodeParam( DWORD Param );

//������ ������
int ThrowPutItem( sITEM *lpItem , int x, int y , int z );
//������ ������
int ThrowPutItem2( sITEMINFO *lpItem , int x, int y , int z );

//���� �������� ����
int SendDeleteStartPoint( int x, int z );
//���� ���� ���� �߰�
int SendAddStartPoint( int x, int z );
//NPC ĳ���� �߰�
int SendAdd_Npc( smCHAR *lpCharSample , int state );
//NPC ĳ���� ����
int SendDelete_Npc( smCHAR *lpCharSample );
//����� ĳ���� ����Ÿ �ҷ����� �䱸
int SendGetRecordData( char *szName );


//������ ����
int	SaveGameData();
//���� ���� ���� ( FALSE ������ ( �����κ��� ����� ���� ���� ) )
int GetSaveResult();

//���� ���� ���� ��� �ڸ��
int	TransUserCommand ( DWORD dwCode , char *szID , char *szName );

//ĳ���� ���� ���� ����
int	Send_GetCharInfo( DWORD	dwObjectSerial , int ServerCode );
//����� ýũ ( �޼����� ���� ���� ������ ��Ʈ�� �ٿ� )
int	TransCheckNetwork();

//������ ������ ��� ����
int SendAdminMode( DWORD AdminNum );
//������ ������ ��� ����
int SendAdminMode2( DWORD AdminNum );
//������ ������ ��� ����
int SendAdminMode( DWORD AdminNum , smWINSOCK *lpsmSock );


//������ ��Ʈ��ũ Ǯ�� ��� ����
int SendNetworkQuality( DWORD NetworkQuality );
//IP���� �ּҸ� �� ������ ������
int SendCheckIP( int Code , smWINSOCK *lpsmSock );

//������ ������Ʈ ��ȣ ����
int SendSetObjectSerial( DWORD dwObjectSerial );
//������ ������Ʈ ��ȣ ���� ( ó�� �ѹ��� )
int SendSetObjectSerial_First( DWORD dwObjectSerial );

//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
int SendSaveThrowItem( sITEMINFO *lpItemInfo );
//����Ÿ ������ ������ ���� �䱸 ( ���� ������ ���� )
int SendSaveThrowItem2( sITEMINFO *lpItemInfo );
//����Ÿ ������ �� ���� �䱸 ( ���� ������ ���� )
int SendSaveMoney();
//������ ������ �õ�
int SendContinueServer( DWORD dwObjectSerial , smWINSOCK *lpsmSock );
//ũ�� ýũ �Ű�
int CheckCracker();
//ũ�� ýũ
int CheckCracker(TRANS_FIND_CRACKER *lpTransFindCrack);
//��ŷ �õ��� ���� �ڵ� �Ű�
int SendSetHackUser( int StopGame );
//��ŷ �õ��� ���� �ڵ� �Ű�
int SendSetHackUser2( int Type , int LParam );
//��ŷ �õ��� ���� �ڵ� �Ű�
int SendSetHackUser3( int Type , int LParam , int SParam );
//���� �������� ������ ��� �Ű� 
int SendCopyItemUser( int CopyItemCount );
//�ٸ� ĳ���� ī�� ���� �䱸
int SendCopyOtherUser( char *szName , int Mode );
//������ ����� ĳ���� ���� ����
int SendPlayUpdateInfo();
//������ ����� Ŭ�� ���� ����
int SendUpdateClanInfo();
//���� ����
int SendOpenMonster( int State );

//������ ��ȯ ��û
int SendRequestTrade( int dwObjectSerial , int Flag );
//��ȯâ ������ �۽�
int SendTradeItem( DWORD dwSender );
//��ȯâ ������ ����
int RecvTradeItems( TRANS_TRADEITEMS *lpTransTradeItems );
//������ ��ȯ ����Ű ����
int SendTradeSucessKey( DWORD dwSender);
//������ ��ȯ ���� Ȯ��
int SendTradeCheckItem( DWORD dwSender );
//��ȯ �ŷ� �Ÿ� Ȯ��
int GetTradeDistanceFromCode( DWORD dwObjectSerial );
//��ȯ �ŷ� �Ÿ� Ȯ��
int GetTradeDistance( smCHAR *lpChar );

//������ ����
int Skil_RangeAttack( int x, int y, int z , int range , int power , int TargetState );
//������ ����( ������ ���� )
int Skil_RangeAttack_User( int x, int y, int z , int range , int power , int TargetState );
//������ ����
int Skil_RangeBoxAttack( smCHAR *lpCurChar , RECT *lpRect , int power , int TargetState , int UseAttackRating );
//������ ���� ����Ʈ ����
int Skil_RangeBoxEffect( smCHAR *lpCurChar , RECT *lpRect , DWORD dwSkillCode , int TargetState );
//������ ���� ����Ʈ ����
int Skil_RangeEffect( smCHAR *lpCurChar , int Range , DWORD dwSkillCode , int TargetState , int point );


//��� ĳ���Ϳ� ���� ���� 
int QuickSendTransAttack( smCHAR *lpChar , int power , int AttackState , int Resistance );
int QuickSendTransAttack( smCHAR *lpChar , int power );
int QuickSendTransAttack( smCHAR *lpChar , int power , int Resistance );



////////////////////////// ��Ƽ ���� /////////////////////////
//��Ƽ ��� 
int SendJoinPartyUser( DWORD dwObjectCode );


///////////////////////// â�� //////////////////////////////
//â�� ����
struct sWAREHOUSE;
int	SaveWareHouse( sWAREHOUSE *lpWareHouse , TRANS_WAREHOUSE *lpTransWareHouse );
int	SaveWareHouse( sWAREHOUSE *lpWareHouse  );
//â�� ��������
int	LoadWareHouse( TRANS_WAREHOUSE *lpTransWareHouse , sWAREHOUSE *lpWareHouse , int flag );
int	LoadWareHouse( TRANS_WAREHOUSE *lpTransWareHouse , sWAREHOUSE *lpWareHouse );

//////////////////////// ��α� //////////////////////////////
int SendCollectMoney( int Money );				//������ ����� �� ����

/////////////////////// PK ////////////////////////////////////
//PK���ؼ� ����
int PlayerKilling( DWORD dwObjectSerial );

//���� ����
smWINSOCK *ConnectServer_Main();
//�������� ���� ��� ����
int DisconnectServerFull();

//���� ���� ����
smWINSOCK *ConnectServer_InfoMain();
//���� ���� ����
smWINSOCK *ConnectServer_GameMain( char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2 );
//���� ���� ����
smWINSOCK *ConnectServer_GameMain( char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2 , char *szIP3, DWORD dwPort3 );
//���� ���� ���� ����
int DisconnectServer_GameMain();


extern smWINSOCK	*smWsockServer ;					//NPC-����-������
extern smWINSOCK	*smWsockDataServer ;				//����Ÿ ���� ����
extern smWINSOCK	*smWsockUserServer ;				//���� �÷��� ����Ÿ ���� ����
extern smWINSOCK	*smWsockExtendServer ;				//�ʵ�Ȯ�� ���� ����

//���� �翬�� Ƚ��
extern	int	ReconnDataServer;
extern	int	ReconnServer;

//�������� ���� ������
extern int	DisconnectFlag;
extern int	DisconnectServerCode;
//��ŷ ���
extern int	WarningHack;

//Ʈ���̵� �õ� �÷�
extern int TradeSendSucessFlag;
extern DWORD	dwTradeMaskTime;
extern int	TradeItemSucessFlag;

extern	DWORD	dwLastRecvGameServerTime;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�
extern	DWORD	dwLastRecvGameServerTime2;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�
extern	DWORD	dwLastRecvGameServerTime3;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�
extern	DWORD	dwLastRecvGameServerTime4;		//�ֱ� ������ ���� ��Ŷ�� �Լ��� �ð�


//////////////// PK �ʵ� ���� //////////////////
struct	PK_FIELD_STATE {
	DWORD	dwPlay_FieldTime;
	int		FieldCode;
};
extern PK_FIELD_STATE	PK_FieldState;		//Pk �ʵ� ���� ����ü


//���� �ð� �ʰ� ĳ���� ����
#define DIS_TIME_OVER		30000
//���� �ð� �ʰ� ĳ���� ��ǥ��
#define DISPLAY_TRANS_TIME_OVER		7000
#define DISPLAY_TRANS_TIME_OVER_FAR	4000

//���� ���� �ڵ�
#define PLAY_SERVER_CODE_MAIN		1
#define PLAY_SERVER_CODE_USER		2
#define PLAY_SERVER_CODE_EXTEND		3

#define PLAY_SERVER_CODE_AREA1		11
#define PLAY_SERVER_CODE_AREA2		12


extern char *szNetLogFile;
extern FILE *fpNetLog;

extern TRANS_SKIL_ATTACKDATA	Back_TransSkilAttackData;

//�ֱ� ���� ������
extern TRANS_ITEMINFO	TransRecvItem;


//����ġ�� �� �ŷ� �ݾ��� ��Ż�� ����Ѵ�
int	SetTotalAddMoney( int Money );
int	SetTotalSubMoney( int Money );
int	SetTotalAddExp( int Exp );
int	SetTotalSubExp( int Exp );
int GetTotalExp();
int	GetTotalMoney();
int GetTotalSubExp();


// pluto ���� ����Ÿ�� ������ ����
int SendSmeltingItemToServer( void *lpsSmeltingItem_Send );
// pluto ���� ����Ÿ�� ������ ����
int SendManufactureItemToServer( void *lpsManufactureItem_Send );
// ������ - �ͽ��� ���� �������� ������ �����Ѵ�.
int SendMixtureResetItemToServer( void *lpsMixtureResetItem_Send );

//�ͽ��� ����Ÿ�� ������ ����
int SendCraftItemToServer( void *lpsCraftItem_Send );
//����¡ ����Ÿ�� ������ ����
int SendAgingItemToServer( void *lpsCraftItem_Send );
//����¡ �Ϸ�� �������� ������ ����
int SendAgingUpgradeItemToServer( sITEMINFO *lpItem );
//��ũ�ھ� ��� ����
int SendLinkCoreToServer( sITEMINFO *lpItem );
//������ �� ������ ����Ÿ�� ������ ����
int SendWingItemToServer( void *lpsCraftItem_Send );

//�Ǹ��� �������� ������ ����
int	SellItemToServer( sITEMINFO *lpItem , int Count );

extern DWORD	dwLastCheckItemTime;
//�������� ������ ������ Ȯ��
int	SendCheckItemToServer( sITEMINFO *lpItem );
//�κ��丮�� ������ �������� ������ ������ �˻�
int CheckInvenItemToServer();


//�������� �޸� ���� ���ؼ� ��������
DWORD funcCheckMemSum( DWORD FuncPoint , int count );

//�ӽ������ ������ ť�� ������ ��������� �˻�
int CheckRecvTrnsItemQue();
//����� ������ �ڵ� ������ ����
int SendUseItemCodeToServer( sITEMINFO *lpItem );

//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ� ������
int	SendCommandUser( int wParam , int lParam , int sParam , int eParam );
//�ֺ� ĳ���Ϳ� �̺�Ʈ ��� �ڵ�
int RecvCommandUser( smTRANS_COMMAND_EX *lpTransCommandEx );

//���������� ��� �ڵ� ����
int	SendCommand_AreaServer( DWORD dwCode , int wParam , int lParam , int sParam , int eParam );
//����Ÿ������ ��� �ڵ� ����
int	SendCommand_DataServer( DWORD dwCode , int wParam , int lParam , int sParam , int eParam );

//���ݷ� ��� ����ȭ
int	Init_RecordDamage();
//���� ���ݷ� ���
int	Record_SendDamage( smWINSOCK *lpsmSock , int Damage );
//���� ���ݷ� ���
int	Record_RecvDamage( smWINSOCK *lpsmSock , int Damage );

//��Ƽ������ ��ų ��� ���� ������
int	SendPartySkillToServer( DWORD dwSkillCode , int SkillLevel , int Around , int wParam , int lParam , int sParam , int eParam );
//��ų ���
int SendCancelSkillToServer( DWORD dwSkillCode , int lParam , int sParam , int eParam );
//������ ��ų���� ����
int SendProcessSKillToServer( DWORD dwSkillCode , int point , int Param1, int Param2 );
//��ų ����
int SendUpdateSkillToServer( DWORD dwSkillCode , int lParam , int sParam , int eParam );


//Ŭ�� �޼��� �ޱ�
int RecvClanJoinService( smTRANS_CHAR_COMMAND2 *lpTransCharCommand );
//Ŭ�� �޼��� ������
int SendClanJoinService( DWORD dwMsgCode , smCHAR *lpChar  );
//Ŭ�� ���� ���� �޼��� ������
int SendJoinClan();

//����� ����Ʈ�� ��ų ���� ( �غ��Լ� - ���ΰ��� )
int	SkillPlay_DivineLightning_Select( smCHAR *lpChar , int SkillPoint );
//����� ����Ʈ�� ��ų ���� ( ����Ʈ ���� )
int SkillPlay_DivineLightning_Effect( smCHAR *lpChar , int SkillPoint );
//���� ���Ǿ� ��ų ���� ( ����Ʈ ���� )
int SkillPlay_VenomSpear_Effect( smCHAR *lpChar , int SkillPoint );
//���� ��ų ����Ʈ ���� ( ����Ʈ ���� )
int SkillPlay_Monster_Effect( smCHAR *lpChar , int EffectKind , int Range );
//����� ����Ʈ�� ��ų ���� ( �ӹ̷ε� )
int SkillPlay_MummyLord_Effect( smCHAR *lpChar , int Range );


//�ð� ���ϱ� (time_t)
DWORD	GetPlayTime_T();
//�Ӹ��� Ŀ���� ��� ����
int	ChangeBigHeadMode( int Mode , int Time );

//Ŭ���� ���� �ص�
int	RecvClanCommand( TRANS_CLAN_COMMAND_USER *lpTransClanUser , _CLAN_USER_INFO *ClanUserInfo );

//�Ȱ� ������ ������
int SendFalconDamage( smCHAR *lpChar );
//��̼ҵ� ������ ������
int SendDancingSwordDamage( smCHAR *lpChar );
//������ �� ������
int SendLowLevelPetDamage( smCHAR *lpChar , int petType );
//PC��� �� ������
int SendPCBangPetDamage( smCHAR *lpChar , int petType );

//�ޱ׳�ƽ ���Ǿ� ������ ������
int SendMegneticSphereDamage( smCHAR *lpChar );
//�ӽ��� ������ ������
int SendMuspellDamage( smCHAR *lpChar );


//�ִ� ������ Ȯ��
int	SendMaxDamageToServer( WORD MinDamage , WORD MaxDamage , WORD Critical );
//�ִ� ������ ��������ü�� ����
int SetMaxDamage( smCHAR_INFO *lpCharInfo );

//��ų��ȣ�� ���� �ʱ�ȭ
int ReformSkillInfo();
//��ų�� ���� Ȯ��
int SaveCheckSkill();

//������ �ù� ���� ���� �䱸
int	SendItemExpress( DWORD	dwItemCode , char *szPassCode );
//������ �ù� ���� ���� �䱸
int	SendItemExpress( DWORD	dwItemCode );

//��ȣ ��� ������ ������
int SendClanYahooMotion();
//�� ����Ʈ ������ ������ ������
int	SendStarPointToServer( int Price , int Cash );
//����ѵ��� ������ ������
int	SendGiveMoneyToServer( int Money );


#define	CLANMONEY_KIND_SOD		0
#define	CLANMONEY_KIND_CASTLE	1

//Ŭ���Ӵ� ������ ������ ������
int	SendClanMoneyToServer( int Money , int Flag , int Kind );
int	SendClanMoneyToServer( int Money , int Flag );
//�̿��� ������ ������ ������
int	SendPaymentMoneyToServer( int Money , int Flag );


//////////// ���� ���� ���� �Լ� /////////////////
int SendOpenPersonalTrade( char *szTradeMsg , void *lpPersTrade );	//���� ���� ����
int SendClosePersonalTrade();										//���� �ݱ�
int	Send_ShowMyShopItem( smCHAR *lpChar );							//���λ��� ������ ���� �䱸
int	Send_MyShopItemList( smTRANS_COMMAND	*lpTransCommand );		//���λ��� ������ ���� ���濡 ������
int	Recv_MyShopItemList( TRANS_TRADEITEMS *lpTransTradeItems );		//���λ��� ������ ���� ����
int	Send_PersonalShopItem( DWORD dwCharCode , void *lpShopItem );	//���� ���� ���� ����
int	UpdateMyShopList( void *lpMyShop );								//���� ���� ������ ���� ������Ʈ

// �庰 - �Է�â
int SendsServerDoc(char *szTradeMsg );



//��¥ Ȯ�� ���� ������
int DeleteEventItem_TimeOut( sITEMINFO	*lpsItem );			//�̺�Ʈ �����Ͽ� ������ ����

//����Ʈ ��� ������
int SendQuestCommandToServer( DWORD dwQuestCode , int Param1, int Param2, int Param3 );

//���� ����Ʈ ����
int Start_QuestArena( DWORD dwQuestCode , int Param1, int Param2 );

//����ĳ�� �������� ������ ����
int	SendBlessCastleToServer( smTRANS_BLESSCASTLE *lpBlessCastleSetup , int Mode );
//����ĳ�� �������� ����
int RecvBlessCastInfo( void *lpPacket );
//����ĳ�� ���� ���� ������ �䱸
int	Send_GetBlessCastleTax();
//���� ĳ�� ������ Ŭ�� ����
int SetBlessCastleMaster( DWORD dwClanCode , int Mode );

//�Ӽ����� ������ ����
int	SendResistanceToServer();


//�κ��丮�� ������ ã�� (�ڵ�)
sITEMINFO *FindItemFromCode( DWORD dwItemCode );

//�������� ��� ������ ������
int	SendPublicPollingToServer( int PollCode , int PollCnt , BYTE *bPollingData );

//���� ýũ�� NPC��� ������ ������ �˻��뺸
int HackTrap_CheckOpenFlag();
//���� ýũ�� ����
int	SendHackTrapToServer( DWORD dwTrapCode , int Param );

#ifdef _XIGNCODE_CLIENT
// ����� - XignCode
int Xigncode_Client_Recv(smWINSOCK *lpsmSock, XIGNCODE_PACKET *pack);
int Xigncode_Client_Start();
#endif

#endif