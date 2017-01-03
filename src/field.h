#define FILED_GATE_MAX			12
#define FIELD_MAX				256
#define FIELD_AMBENT_MAX		80
class sFIELD;


//�ʵ��� ����Ʈ
struct sFGATE	{
	int	x,z,y;				//�ʵ� ����Ʈ ��ġ
	sFIELD	*lpsField;		//�ʵ� ������
};

//�ʵ��� ����Ʈ
struct sWARPGATE	{
	int	x,z,y;								//�ʵ� ����Ʈ ��ġ
	int	height,size;						//����
	sFGATE	OutGate[FILED_GATE_MAX];		//�ⱸ ��ġ
	int		OutGateCount;					//�ⱸ ī����
	int		LimitLevel;						//��������
	int		SpecialEffect;					//Ư�� ȿ��
};

struct	sAMBIENT_POS {
	int	x,y,z;
	int	round;
	int	AmbentNum;
};

#define FIELD_STATE_VILLAGE		0x100
#define FIELD_STATE_FOREST		0x200
#define FIELD_STATE_DESERT		0x300
#define FIELD_STATE_RUIN		0x400
#define FIELD_STATE_DUNGEON		0x500
#define FIELD_STATE_IRON		0x600
#define FIELD_STATE_ROOM		0x800
#define FIELD_STATE_QUEST_ARENA	FIELD_STATE_DUNGEON

//######################################################################################
//�� �� �� : �� �� ��
#define FIELD_STATE_ICE			0x900
//######################################################################################

#define FIELD_STATE_CASTLE		0xA00
#define FIELD_STATE_ACTION		0xB00


#define FIELD_STATE_ALL			0x1000

#define FIELD_BACKIMAGE_RAIN		0x00
#define FIELD_BACKIMAGE_NIGHT		0x01
#define FIELD_BACKIMAGE_DAY			0x02
#define FIELD_BACKIMAGE_GLOWDAY		0x03
#define FIELD_BACKIMAGE_DESERT		0x04
#define FIELD_BACKIMAGE_GLOWDESERT	0x05
#define FIELD_BACKIMAGE_NIGHTDESERT	0x06
#define FIELD_BACKIMAGE_RUIN1		0x07
#define FIELD_BACKIMAGE_RUIN2		0x08
#define FIELD_BACKIMAGE_NIGHTRUIN1	0x09
#define FIELD_BACKIMAGE_NIGHTRUIN2	0x0A

#define FIELD_BACKIMAGE_GLOWRUIN1	0x0B
#define FIELD_BACKIMAGE_GLOWRUIN2	0x0C


#define FIELD_BACKIMAGE_NIGHTFALL	0x11
#define FIELD_BACKIMAGE_DAYFALL		0x12
#define FIELD_BACKIMAGE_GLOWFALL	0x13

#define FIELD_BACKIMAGE_NIGHTIRON1	0x14
#define FIELD_BACKIMAGE_NIGHTIRON2	0x15
#define FIELD_BACKIMAGE_DAYIRON		0x16
#define FIELD_BACKIMAGE_GLOWIRON	0x17

#define FIELD_BACKIMAGE_SODSKY      0x18    //õ���� Ȧ
#define FIELD_BACKIMAGE_SODMOON     0x19    //���� ��
#define FIELD_BACKIMAGE_SODSUN      0x20    //�¾��� ��
#define FIELD_BACKIMAGE_SODNONE     0x21    //�ƹ��͵� �ȳ����� �ϴ�   ������ ����� �ϴ��� ���� �����ϴ�.
#define FIELD_BACKIMAGE_IRONBOSS    0x22    //���̾� ������ ������ �ϴ�

#define FIELD_BACKIMAGE_DAYSNOW		0x23    //
#define FIELD_BACKIMAGE_GLOWSNOW    0x24    //
#define FIELD_BACKIMAGE_NIGHTSNOW   0x25    //

#define FIELD_BACKIMAGE_DAYGREDDY	 0x26    //
#define FIELD_BACKIMAGE_GLOWGREDDY   0x27    //
#define FIELD_BACKIMAGE_NIGHTGREDDY  0x28    //


#define FIELD_BACKIMAGE_DAYLOST		 0x29    //�ν�Ʈ���� �� (�����߰�)
#define FIELD_BACKIMAGE_GLOWLOST	 0x2A    //�ν�Ʈ���� ���� 
#define FIELD_BACKIMAGE_NIGHTLOST    0x2B    //�ν�Ʈ���� ��

#define FIELD_BACKIMAGE_DAYTEMPLE	 0x2C	//�ν�Ʈ���� ��
#define FIELD_BACKIMAGE_GLOWTEMPLE	 0x2D	//�ν�Ʈ���� ����
#define FIELD_BACKIMAGE_NIGHTTEMPLE	 0x2E	//�ν�Ʈ���� ��


#define	FIELD_EVENT_NIGHTMARE		0x01

#define	FIELD_START_POINT_MAX		8

#define	FIELD_STAGE_OBJ_MAX			50


////////////////// ���� �ʵ� ��ȣ ///////////////////////
#define START_FIELD_NUM			3
#define START_FIELD_NEBISCO		9
#define START_FIELD_MORYON		21
#define START_FIELD_CASTLE		33

struct	ACTION_FIELD_CAMERA {
	POINT3D	FixPos;
	int		LeftX,RightX;
};

//�ʵ� ����
class sFIELD {
	DWORD	head;
public:
	char	szName[64];						//�ʵ� ���� �̸�
	char	szNameMap[64];					//���� ���� �̹��� ����
	char	szNameTitle[64];				//���� �̸� �̹��� ����
	int		State;							//�ʵ� �Ӽ�
	int		BackImageCode[3];				//�⺻ ��� �ϴ� ��ȣ
	int		BackMusicCode;					//������� �ڵ�
	int		FieldEvent;						//�ʵ��� �̺�Ʈ

	int		GateCount;						//�ʵ� ����Ʈ�� ��
	sFGATE	FieldGate[FILED_GATE_MAX];		//�ʵ� ����Ʈ ( ����Ǵ� �ʵ� )

	int		WarpGateCount;					//���� ����Ʈ�� ��
	int		WarpGateActiveNum;				//��������Ʈ Ȱ�� ��ȣ
	sWARPGATE	WarpGate[FILED_GATE_MAX];	//���� ����Ʈ
	POINT3D	PosWarpOut;						//���� ����Ʈ �ⱸ

	sAMBIENT_POS	AmbientPos[FIELD_AMBENT_MAX];	//��� ȿ����
	int		AmbentCount;							//��� ȿ���� ��


	int		LimitLevel;						//��������
	int		FieldSight;						//�ʵ�þ�

	char	*lpStageObjectName[FIELD_STAGE_OBJ_MAX];		//��� ���� ������Ʈ
	DWORD	StgObjBip[FIELD_STAGE_OBJ_MAX];					//��� ���� ������Ʈ BIP�ִϸ��̼� �������
	int		StgObjCount;

	int		cX,cZ;											//�ʵ� �߾�
	int		FieldCode;										//�ʵ��� �ڵ� ��ȣ
	int		ServerCode;

	POINT	StartPoint[FIELD_START_POINT_MAX];				//�ʵ� ���� ����
	int		StartPointCnt;									//���� ���� ����Ʈ

	ACTION_FIELD_CAMERA	ActionCamera;					//�׼��ʵ忡���� ī�޶� ����
	int	AddStageObject( char *szStgObjName , int BipAnimation=0 );	//��� ���� ������Ʈ �߰�
	int	GetStageObjectName( int num , char *szNameBuff );			//��� ���� ������Ʈ �̸� ���

	int	SetCenterPos( int x, int z );								//�ʵ� �߾� ��ǥ �Է�
	int AddGate( sFIELD *lpsField , int x, int z, int y );			//����Ʈ �߰�
	int SetName( char *lpName , char *lpNameMap=0 );				//�̸� ����
	int AddGate2( sFIELD *lpsField , int x, int z, int y );			//����Ʈ �߰�

	int	AddWarpGate( int x, int y, int z, int size , int height );	//��������Ʈ �߰�
	int	AddWarpOutGate( sFIELD *lpsField , int x, int z, int y );	//��������Ʈ �ⱸ �߰�
	int	CheckWarpGate( int x, int y, int z );						//��������Ʈ Ȯ��

	int AddStartPoint( int x, int z );							//���� ��ǥ�� ���� �߰�
	int GetStartPoint( int x, int z , int *mx , int *mz );		//���� ������ ���� ��ǥ�� ��´� 
	int CheckStartPoint( int x, int z );						//���� ��ǥ�� ĳ���Ͱ� �ִ��� Ȯ�� 

	int AddAmbientPos( int x, int y, int z , int round , int AmbCode );	//��� ȿ���� �߰�
	int	PlayAmbient();											//��� ȿ���� ���� ( �ֱ��� ȣ�� )

};



//�ʵ� �ڵ� ���� ����
int FieldMain( int x, int y, int z );
//�ʵ� �ʱ�ȭ
int InitField();
//�ʵ� ����
int StartField();
//�ʵ� ����
int WarpNextField( int *mx, int *mz );
//�ʵ� ����
int WarpField( int FieldNum , int *mx, int *mz );
//�ʵ� ����
int WarpFieldNearPos( int FieldNum , int x, int z , int *mx, int *mz );
//�ʵ� ����
int WarpStartField( int *mx, int *mz );
//Ŀ���� �ʵ�� �̵�
int WarpCustomField( int *mx, int *mz );

//�ʵ� ���� ( ��ȯ�� )
int WarpStartField( int FieldNum , int *mx, int *mz );

//�������� ����
int WarpPrisonField( int *mx, int *mz );

//���� ����� ����
int	WingWarpGate_Field( int dwFieldCode );

//���� �ʵ�
int WarpField2( int Num );

//���� ���� ĳ�� �ʵ�
int WarpCastleField( int MasterClan , int *dx, int *dz );



extern sFIELD	sField[ FIELD_MAX ];
extern int	PrisonX;		//������ġ
extern int	PrisonZ;		//������ġ
extern RECT	PrisonRect;		//���� ����
extern DWORD	dwNextWarpDelay;	//���� ������

/////////////////////// playmain.cpp ���� ////////////////////////
//����� �о� �´� ( ������ ��� )
smSTAGE3D *LoadStageFromField( sFIELD *lpField , sFIELD *lpSecondField );
//����� �о� �´� 
smSTAGE3D *LoadStageFromField2( sFIELD *lpField , sFIELD *lpSecondField );

extern	sFIELD	*StageField[2];

extern  int FieldLimitLevel_Table[];

//######################################################################################
//�� �� �� : �� �� ��
extern int FieldCount;
//######################################################################################
