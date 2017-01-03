#ifndef	_FILEREAD_HEADER

#define	_FILEREAD_HEADER

//ȭ�� �� ĳ���� �� ���ϵ� �⺻ ����
extern smCONFIG	smConfig;

//���� ������ �ص��Ͽ� �����ش� (Info������)
smCONFIG *smConfigDecode( char *iniFile );

//ũ������������ �о� �´�
int ReadCustomCracker( char *szCrackFile , TRANS_FIND_CRACKER *lpTransFindCrack  );


//�� ���� ������ �ص��Ͽ� �����ش� (Info������)
int smModelDecode( char *iniFile , smMODELINFO *Info );

//ĳ���� ���� ���Ͽ��� �ص��Ͽ� �����Ѵ�
int smCharDecode( char *szCharFile , smCHAR_INFO *lpCharInfo , smCHAR_MONSTER_INFO	*lpMonInfo , char *lpDialogMessage=0 );

extern char	szShotCutMessage[10][100];
int	ReadShotcutMessage( char *szFileName );		//���� �޼��� �о� ����


//���� ���� ����ü
struct rsSTG_MONSTER {
	char	szMonsterName[32];			//���� �̸�
	int		OpenPercentage;				//������

	smCHAR_INFO	*lpCharInfo;			//���� ���� ������
	int		NumOpenStart;				//������ ���� ��
};

#define rsSTG_MONSTER_MAX	50
#define rsBOSS_MONSTER_MAX	16

struct sBOSS_MONSTER {
	smCHAR_INFO		*MasterMonster;			//������ ����
	smCHAR_INFO		*SlaveMonster;			//���� ����
	int				SlaveCount;				//���� ���� ����
	BYTE			bOpenTime[32];			//��Ÿ���� �ð�
	int				OpenTimeCount;			//�ð� ��
};

struct	rsSTG_MONSTER_LIST {

	rsSTG_MONSTER	rsMonster[rsSTG_MONSTER_MAX];

	int		PecetageCount;
	int		Counter;
	int		LimitMax;						//���� ���Ѽ� / �ִ�ġ
	int		OpenInterval;					//���� �ֱ�
	int		OpenLimit;						//������ �ִ� ������
	DWORD	dwIntervalTime;					//���� ���� ���ѽð�

	sBOSS_MONSTER	sBossMonsters[rsBOSS_MONSTER_MAX];		//�θ�� ���� ��Ʈ
	int	BossMonsterCount;									//�θ�� ���� ���� ����

};


#define SPECIAL_JOB_RANDOM_MAX			12

//�⺻ ������ ����
struct	sDEF_ITEMINFO	{

	sITEMINFO Item;

	short	sDurability[2];			//������
	short	sResistance[8][2];		//���� ���׷�
	short	sDamage[4];				//���ݷ�
	short	sAttack_Rating[2];		//���߷�
	short	sDefence[2];			//����
	float	fBlock_Rating[2];		//����
	float	fAbsorb[2];				//�����
	float	fSpeed[2];				//�̵��ӵ�
	int		Increase_Life[2];		//������ ����
	int		Increase_Mana[2];		//���� ����
	int		Increase_Stamina[2];	//���׹̳� ����

	float	fMana_Regen[2];			//�������
	float	fLife_Regen[2];			//���������
	float	fStamina_Regen[2];		//���׹̳����


	/////////////////// Ưȭ ���� /////////////////////
	short	sSpecial_Defence[2];			//����
	float	fSpecial_Absorb[2];				//�����
	float	fSpecial_fSpeed[2];				//�̵��ӵ�

	float	fSpecial_Magic_Mastery[2];		//�������õ�
	float	fSpecial_Mana_Regen[2];			//��� ���

	int     Lev_Attack_Rating[2];			//���߷� 

	DWORD	dwJobBitCode_Random[SPECIAL_JOB_RANDOM_MAX];			//����Ưȭ ���� ��� 
	int		JobBitCodeRandomCount;

	//////////////////// �߻��� /////////////////////////

	short	sGenDay[2];										//�Ϸ翡 �߻��Ҽ� �ִ¾�/�Ϸ絿�� �߻��ȼ�

	BYTE	*DefCompressData[ sizeof(sITEMINFO) ];	//Item ����Ÿ�� ���� ����
	int		DefCompressDataLen;						//���൥��Ÿ�� ����
};


struct JOBNAME_BIT {
	char *szName;
	DWORD dwBitMask;
};

extern JOBNAME_BIT BitMaskJobNames[];


//���� ���� ���� ���Ͽ��� �ص��Ͽ� �����Ѵ�
int DecodeOpenMonster( char *szCharFile , rsSTG_MONSTER_LIST *lpStgMonList , smCHAR_INFO *lpCharInfoList, int ListCnt );
//������ ���� ������ �ص��Ͽ� �����ش� 
int DecodeItemInfo( char *szItemFile , sDEF_ITEMINFO *lpDefItem  );
//������ ����
int CreateDefItem( sITEMINFO *lpItem , sDEF_ITEMINFO *lpDefItem , int SpJobCode=0 );
//������ ����
int CreateDefItem( sITEMINFO *lpItem , sDEF_ITEMINFO *lpDefItem , int SpJobCode , int ItemNormam );

//���� ������ ����
int SetLowerItem( sDEF_ITEMINFO *lpDefItem  );

//�ּ� �ִ�ġ���� ������ ����
float GetRandomFloatPos( float Min , float Max );
int GetRandomPos( int Min , int Max );

//SOD����Ʈ�� ���� ���� �ҷ�����
int	Open_SODConfig( char *szFileName , void *lphCoreMonster , int *SodTax , char *szSodText , int *SodScoreDiv );

//Mechanican
#define		JOB_CODE_MECHANICIAN		0x00000001		//T1
#define		JOB_CODE_MECHANICMASTER		0x00000010		//T2
#define		JOB_CODE_METALLEADER		0x00000100		//T3
#define		JOB_CODE_HEAVYMETAL			0x00001000		//T4

//Fighter
#define		JOB_CODE_FIGHTER			0x00000002		//T1
#define		JOB_CODE_WARRIOR			0x00000020		//T2
#define		JOB_CODE_CHAMPION			0x00000200		//T3
#define		JOB_CODE_IMMORTALWARRIOR	0x00002000		//T4

//Pikeman
#define		JOB_CODE_PIKEMAN			0x00000004		//T1
#define		JOB_CODE_COMBATANT			0x00000040		//T2
#define		JOB_CODE_LANCER				0x00000400		//T3
#define		JOB_CODE_LANCELOT			0x00004000		//T4

//Archer
#define		JOB_CODE_ARCHER				0x00000008		//T1
#define		JOB_CODE_HUNTERMASTER		0x00000080		//T2
#define		JOB_CODE_DIONS_DISCIPLE		0x00000800		//T3
#define		JOB_CODE_SAGITTARION		0x00008000		//T4

//Knight
#define		JOB_CODE_KNIGHT				0x00010000		//T1
#define		JOB_CODE_PALADIN			0x00100000		//T2
#define		JOB_CODE_HOLYKNIGHT			0x01000000		//T3
#define		JOB_CODE_SAINTKNIGHT		0x10000000		//T4

//Atalanta
#define		JOB_CODE_ATALANTA			0x00020000		//T1
#define		JOB_CODE_VALKYRIE			0x00200000		//T2
#define		JOB_CODE_BRUNHILD			0x02000000		//T3
#define		JOB_CODE_VALHALLA			0x20000000		//T4

//Priest
#define		JOB_CODE_PRIEST				0x00040000		//T1
#define		JOB_CODE_SAINTESS			0x00400000		//T2
#define		JOB_CODE_BISHOP				0x04000000		//T3
#define		JOB_CODE_CELESTIAL			0x40000000		//T4

//Magican
#define		JOB_CODE_MAGICIAN			0x00080000		//T1
#define		JOB_CODE_WIZARD				0x00800000		//T2
#define		JOB_CODE_ROYALKNIGHT		0x08000000		//T3
#define		JOB_CODE_ARCHMAGE			0x80000000		//T4

//Fighter
#define		JOB_CODE_WARLORD			0x00008000		//T5

//Mechanican
#define		JOB_CODE_METALRAGE			0x00008000		//T5

//Pikeman
#define		JOB_CODE_BAYLOLUNT			0x00008000		//T5

//Archer
#define		JOB_CODE_ISKAR				0x00008000		//T5

//Knight
#define		JOB_CODE_VALIANT			0x00008000		//T5

//Atalanta
#define		JOB_CODE_QUEENOFVALHALA		0x00008000		//T5

//Priestess
#define		JOB_CODE_ARCANGLE			0x00008000		//T5

//Magican
#define		JOB_CODE_ELEMENTALMASTER	0x00008000		//T5



#define		BROOD_CODE_TEMPSKRON			1
#define		BROOD_CODE_MORAYION				2
#define		BROOD_CODE_SOPPHETIO			4


struct	JOB_DATA_CODE {
	char	*szName;			//�̸� ����
	char	*szName2;			//�̸� �ѱ�
	DWORD	JobBitCode;			//���� ��Ʈ �ڵ�
	DWORD	JobCode;			//���� �Ϲ� �ڵ�
	DWORD	Brood;				//�ش� ����

	int     Sight;				//�þ�  ****������ �þ� +5 ���� ****
	int     LifeFunction;		//����� �Լ� 
	int     ManaFunction;		//��� �Լ� 
	int     StaminaFunction;	//�ٷ� �Լ� 
	short   DamageFunction[3];  //0 ���� 1 ���Ÿ� 2����  
};


struct	SKILL_DATA_CODE {
	char	*szSkillName;				//��ų �̸�
	int		dwPlaySkillCode;			//��ų �ڵ�
};

extern	JOB_DATA_CODE	JobDataBase[];
extern	SKILL_DATA_CODE	SkillDataCode[];

//���� ���� ���ϱ� ( �⺻ ���� [0~3] , ���� Ƚ�� [0~3] )
JOB_DATA_CODE	*GetJobDataCode(	DWORD BaseJob	, int	JobCnt );

extern int NotItemSpecial; // ������ - �ͽ��� ����

#endif
