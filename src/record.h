#ifndef	_RECORD_HEADER

#define _RECORD_HEADER

#define RECORD_ITEM_MAX		200
#define	TRANS_RECORD_LEN	(smSOCKBUFF_SIZE-32)


#define	PRIME_ITEM_PACKAGE_NONE			0
#define	PRIME_ITEM_PACKAGE_BRONZE		1
#define	PRIME_ITEM_PACKAGE_SILVER		2
#define	PRIME_ITEM_PACKAGE_GOLD			3
#define	PRIME_ITEM_PACKAGE_ULTRA		4
// pluto ��Ű��2
#define	PRIME_ITEM_PACKAGE_BRONZE2		5
#define	PRIME_ITEM_PACKAGE_SILVER2		6
#define	PRIME_ITEM_PACKAGE_GOLD2		7
#define	PRIME_ITEM_PACKAGE_ULTRA2		8
#define	PRIME_ITEM_PACKAGE_NONE_50_EXPUP		 9 // ����� - ����ġ���� ����(50%) ������ ���� 
#define	PRIME_ITEM_PACKAGE_NONE_100_EXPUP		10 // ����� - ����ġ���� ����(100%) ������ ���� 

//����� ������ ����
struct	sRECORD_ITEM {
	int	ItemCount;
	int x,y;			        //�������� �׷��� ��ǥ 
	int	ItemPosition;			//�ڵ尡 ����

	sITEMINFO sItemInfo;		//������ ����
};

#define	_SIN_MAX_USE_SKILL	16

//����� ��ų ����
struct	RECORD_SKILL {
	BYTE	bSkillPoint[_SIN_MAX_USE_SKILL];			//��ų ����Ʈ
	WORD	wSkillMastery[_SIN_MAX_USE_SKILL];		//��ų ���õ�
	BYTE	bShortKey[_SIN_MAX_USE_SKILL];			//��� Ű
	WORD	wSelectSkill[2];						//���õ� ��ų
	//int		RemainPoint;
};

//����Ʈ ���� ����
struct	sQUEST_INFO	{
	WORD	wQuestCode[2];

	DWORD	Data[7];
};

#define	LAST_QUEST_MAX		32
#define	LAST_QUEST_MASK		31

struct	sLAST_QUEST_INFO {
	WORD wLastQuest[LAST_QUEST_MAX];
	int	LastQuestCount;
};

extern sLAST_QUEST_INFO	RecordLastQuestInfo;			//���� ����Ʈ ����


//���� ����� ����Ÿ ����ü ( ĳ���� ���� �̿ܿ� ����Ʈ �� ��Ƽ ���� )
struct	sGAME_SAVE_INFO {
	DWORD	Head;

	int		PlayStageNum;			//�÷������� ��ġ
	int		CameraMode;				//ī�޶� ������
	int		PosX,PosZ;				//�α׾ƿ� �� ��ǥ
	int		LastMoeny;				//������ ����ȵ� ��

	DWORD	dwChkSum_CharInfo;		//ĳ���� ���� ýũ�� ( ���� ��ŷ�� ���� ���� ���� )

	RECORD_SKILL	SkillInfo;		//��ų ����	[ ũ�� 17*sizeof( DWORD ) ]

	int		SaveTime;				//������ �ð� ( Time_T )

	int		PCRNo;					//PC�� ����
	short	EventPlay[2];			//�̺�Ʈ����

	short	sPetInfo[2];			//������ ����

	short	sPotionUpdate[2];		//���� ������Ʈ ����
	short	sPotionCount[3][4];		//���� ���� ����
	WORD	wForceOrbUsing[2];		//�������� ��� [0]->FO1(XX) [1]->Time(Sec)

	DWORD	dwLevelQuestLog;		//���� ����Ʈ ���� ����

	int		ShortKey_NormalAttack;	//�⺻ ���� ����Ű ����

	int		BlessCastleTax;			//���� ĳ�� ���� ����
	DWORD	dwBlessCastleMaster;	//���� ĳ�� ������ (Ŭ���ڵ�)

	DWORD	dwElementaryQuestLog;	//�ʺ� ����Ʈ ���� ����

	//�ð��� ���� ������ ����
	DWORD	dwTime_PrimeItem_X2;			//������ 2�� ��� // ��������
	DWORD	dwTime_PrimeItem_ExpUp;			//����ġ�� ����
	DWORD	dwTime_PrimeItem_VampCuspid;	//��ä���ִ� ��������ȿ�ð�
	DWORD	dwTime_PrimeItem_ManaRecharg;	//����ä���ִ� ������ ��ȿ�ð�
	DWORD	dwPrimeItem_PackageCode;		//��Ű���������ڵ�
	DWORD	dwTime_PrimeItem_MightofAwell;	// pluto ����Ʈ ���� ����	������ +300
	DWORD	dwTime_PrimeItem_MightofAwell2;	// pluto ����Ʈ ���� ����	������ +500
	DWORD	dwTime_PrimeItem_ManaReduce;	// pluto ���� ���ེ ����
	DWORD	dwTime_PrimeItem_PhenixPet;		// pluto ��(�ؿ�)
	DWORD	dwTime_PrimeItem_HelpPet;		// ����� - ���� ����� �� �߰�
	DWORD	dwTime_PrimeItem_VampCuspid_EX;	// �庰 - ���Ǹ� Ŀ���� EX
	DWORD	dwTime_PrimeItem_StaminaReduce;	// ����� - �ٷ� ���ེ ����

//	DWORD	dwTemp1[7];				//���� ����
//	DWORD	dwTemp1[3];				//���� ����	// pluto �ð��� ���� ������ ���� �߰�
//	DWORD	dwTemp1[1];				//���� ����	// ����� �ð��� ���� ������ ���� �߰�(����� �� �߰�) // �庰 - ���Ǹ� Ŀ���� EX
//	        dwTemp1 -> ��� ����    //���� ����	// ����� �ð��� ���� ������ ���� �߰�(�ٷ� ���ེ ����) 

	int		TotalSubExp;			//�׳� �ļ� ���� ����ġ
	int		TotalMoney;				//Ŭ���̾�Ʈ���� ������ ȹ���� ��
	int		TotalExp;				//Ŭ���̾�Ʈ���� ������ ȹ���� ����ġ

	char	szMasterID[32];			//ĳ���� ���� ID

	sQUEST_INFO			QuestInfo;		//����Ʈ �������� ( �ϴ� 1���� ������� )	- ũ�� DWORD *8
	sLAST_QUEST_INFO	LastQuestInfo;	//���� �Ϸ�� ����Ʈ

	DWORD	TT_ServerID;				//��ʸ�Ʈ ����ID

	WORD	wLifeBoosterUsing[2];		// ����� - �ν��� ������(�����)
	WORD	wManaBoosterUsing[2];		// ����� - �ν��� ������(���)
	WORD	wStaminaBoosterUsing[2];	// ����� - �ν��� ������(�ٷ�)

	WORD	wSkillDelayUsing[2];	// �庰 - ��ų ������

	DWORD	dwTemp2[34];			//���� ���� //	dwTemp2[38] -> dwTemp2[35] // ����� - �ν��� ������(�����, ���, �ٷ�) // �庰 - ��ų ������ 35->34
	//DWORD	dwTemp[(121-17)];				//���� ����
	// �庰 - �������
	//DWORD	dwTemp3[2096];
};


//����� ��ü ���� 
struct	TRANS_RECORD_DATA {
	int size,code;

	char	szHeader[8];

	smCHAR_INFO smCharInfo;

	sGAME_SAVE_INFO	GameSaveInfo;

	sTHROW_ITEM_INFO	ThrowItemInfo[THROW_ITEM_INFO_MAX];		//������ ������ ����
	int		ThrowItemCount;										//������ ������ ī����

	int		ItemCount;
	int		ItemSubStart;

//	sRECORD_ITEM	RecordItems[RECORD_ITEM_MAX];
	int		DataSize;
	BYTE	Data[ sizeof(sRECORD_ITEM)*RECORD_ITEM_MAX ];
};

//����� ���� ��Ʈ
struct	TRANS_RECORD_DATAS {
	int	size,code;
	int	Count , Total;
	int	TransSize;

	char	szData[TRANS_RECORD_LEN];
};
#define sPLAY_CHAR_MAX		6

//�̸� , ���� , ���� , ���� 


struct	sPLAY_USER_DATA {
	char	szHeader[16];						//���� ���
	char	szID[32];							//ID

	char	szCharName[sPLAY_CHAR_MAX][32];		//ĳ���� �̸�
};



struct	_TRANS_CHAR_INFO {
	char	szName[32];				//�̸�
	char	szModelName[64];		//�� �̸�
	char	szModelName2[64];		//�� �̸�2
	DWORD	JOB_CODE;				//���� �ڵ� 
	int		Level;					//���� 
	DWORD	Brood;					//����
	DWORD	dwArmorCode;			//���� �ڵ�
	int		StartField;				//������ ���� �ʵ�
	int		PosX,PosZ;				//�α׾ƿ� ��ǥ
	DWORD	dwTemp[13];				//���� ����
};

struct	TRANS_USERCHAR_INFO {
	int		size,code;

	char	szID[32];
	int		PlayUserCount;						//ĳ���� ����ŭ ī��Ʈ

	_TRANS_CHAR_INFO	CharInfo[sPLAY_CHAR_MAX];
};


class	rsRECORD_DBASE {
public:
	TRANS_RECORD_DATA	TransRecordData;
	int	TransLastPartCount;
	BYTE	TransDataBlock[64];
	DWORD	dwRecordTime;				//����õ� �ð�
	DWORD	dwNextRecordTime;			//���� ���� �ð�

	//������ ����Ÿ�� �����Ͽ� ����
	int MakeRecordData( smCHAR_INFO *lpCharInfo , sITEM *lpItems , sITEM *lpItems2 , sITEM *lpMouseItem=0 );
	//�ҷ��� ����Ÿ ������ �ش� ��ġ�� ����
	int	ResotrRecordData( smCHAR_INFO *lpCharInfo , sITEM *lpItems , sITEM *lpItems2 , sITEM *lpMouseItem=0 );

	//Ŭ���̾�Ʈ ��
	//������ ����Ÿ�� �����Ͽ� ������ ����
	int SendRecordDataToServer( smWINSOCK *lpsmSock );
	//������ ����Ÿ�� �����Ͽ� ���� ( ���� �Ϸ�Ǹ� TRUE ��ȯ )
	int RecvRecordDataFromServer( TRANS_RECORD_DATAS *lpTransRecord );

	//������
	//������ ����Ÿ�� �����Ͽ� ���� ( ���� �Ϸ�Ǹ� TRUE ��ȯ )
	int RecvRecordDataFromClient( TRANS_RECORD_DATAS *lpTransRecord , rsPLAYINFO *lpPlayInfo );		//int Level , int CrackUser , char *lpRecordMemBuff=0 );

	//����� ����Ÿ�� �ҷ��ͼ� ����
	int SendRecordDataToClient( rsPLAYINFO *lpPlayInfo , char *szName , smWINSOCK *lpsmSock , int Mode=0);

	//�ҷ��� ������ ����� ���������� ����
	int MakeRecordItemList( rsPLAYINFO *lpPlayInfo );


	//������ ����Ÿ ��Ʈ��
	//ĳ���� ����Ÿ ����
	int DeleteCharData( char *szID , char *szName );
	//ĳ���� ����Ÿ ���� ����
	int InsertCharData( char *szID , char *szName , int Mode=0 );

	//Ĵ���� ����Ÿ�� �����ϴ��� Ȯ��
	int IsData( char *szName );
	//�ش� ID�� �ش��ϴ� ĳ���� ����Ÿ�� Ŭ���̾�Ʈ�� ������
	int SendUserDataToClient( char *szID , smWINSOCK *lpsmSock , char *szServerID );

	//������ ������ ����Ÿ ����
	int SaveThrowData( char *szName , sTHROW_ITEM_INFO *lpThrowItemList , int Count , int SaveMoney );

};

int GetUserDataFile( char *szName , char *szFileName );

//�޸� ���۸� ���Ϸ� ����
int rsRecordMemoryBuffToFile( rsPLAYINFO *lpPlayInfo , char *szName ,  char *lpRecordMemBuff );
//int rsRecordMemoryBuffToFile( char *szName ,  char *lpRecordMemBuff );

//������ �޸𸮹����� �������� �ùٸ��� ���� Ȯ��
int rsRecordMemoryBuff_CheckInvenItem( rsPLAYINFO *lpPlayInfo , int Mode=0 );
//������ ������ �޸𸮹��� ����Ÿ�� ���� 
int rsSaveThrowData( rsPLAYINFO *lpPlayInfo );


//â�� ����Ÿ ����
int rsSaveWareHouseData( char *szID , TRANS_WAREHOUSE *lpTransWareHouse );
//â�� ����Ÿ ����
int rsLoadWareHouseData( rsPLAYINFO *lpPlayInfo );//char *szID , smWINSOCK *lpsmSock );
//�ش� ID�� �ش� ĳ������ �����ϴ��� Ȯ��
int rsCheckAccountChar( char *szID , char *szName );
//â�� ���� (������)
int rsLoadWareHouseData_Admin( rsPLAYINFO *lpPlayInfo , char *szID , int Day );

//�������� ��� ����
int rsLoadServerForce( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo );
//�������� ��� ����
int rsSaveServerForce( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo );
//���� ���� ����
int rsSaveServerPotion( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo );
//���� ������ ��
int rsCompareServerPotion( rsPLAYINFO *lpPlayInfo , sGAME_SAVE_INFO *lpGameSaveInfo );


//����Ÿ ���弭�� ���丮 ����
int CreateDataServerDirectory();

//����� ����Ʈ ��� �߰�
int Record_LastQuest( WORD	wQuestCode );
//���� ����Ʈ �˻�
int FindLastQuestCode( WORD wQuestCode );

//��ų ýũ
int	CheckSkillPoint( int Level , RECORD_SKILL *lpRecordSkill , int *spTotal , DWORD dwLevelQuestLog );



//������ ���� ����
int	rsSavePostBox( rsPLAYINFO	*lpPlayInfo );
//������ ���� �ε�
int	rsLoadPostBox( rsPLAYINFO	*lpPlayInfo );
//�����ۼ��� ���� ����
int	rsAddPostBox_Present( rsPLAYINFO *lpPlayInfo );

//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����
int	OpenStarPointEvent( rsPLAYINFO *lpPlayInfo , smCHAR_INFO *lpCharInfo );
//�� ����Ʈ �̺�Ʈ Ƽ�� �߻� ����
int	OpenStarPointTicket( rsPLAYINFO *lpPlayInfo );

//�ٸ� �������� ������ �ҷ��� ����
int ImportTTServerUser( char *szID , char *szServerID );
//����ID ���ϱ�
int SetServerID( char *szID , char *szServerID );

//////////////////////////////////////////////////
//�ð� Ȯ���Ͽ� ����õ�
int rsTimeRecData();
//����DB�Լ� �ʱ�ȭ
int rsInitDataBase();
//����DB�Լ� ����ȭ
int rsCloseDataBase();
/////////////////////////////////////////////////

//����Ʈ�� ȹ���� ��ų ����Ʈ
int GetSkillPoint_LevelQuest( int Level , DWORD dwLevelQuestLog );
//����Ʈ�� ȹ���� ���� ����Ʈ
int GetStatePoint_LevelQuest( int Level , DWORD dwLevelQuestLog );
//ĳ���� ���� ����Ʈ Ȯ�� �� �缳��
int ReformCharStatePoint( smCHAR_INFO *lpCharInfo , DWORD dwLevelQuestLog );


//�α��� ���� ����Ű Ȯ��
int	rsCheck_LoginServerSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode=0 );
//�α��� ���� ����Ű ����
int rsSet_LoginServerSafeKey( smCHAR_INFO *lpCharInfo );
//�α��� ���� ���� Ű ����
DWORD	rsGetLoginSafeKey( smCHAR_INFO *lpCharInfo , DWORD dwSpdSumCode=0 );



extern int	Permit_CheckMoney ;
extern int	Permit_CheckExp ;


struct	sBILLING_INFO {
	int	PCRNo;
	int	EventPlay[2];
};

extern sBILLING_INFO	*sBiInfo;


#endif