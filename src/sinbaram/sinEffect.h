/*----------------------------------------------------------------------------*
*	���ϸ� :  sinEffect.h	
*	�ϴ��� :  ����Ʈ ���� �۾� 
*	�ۼ��� :  ����������Ʈ 2002�� 6��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#define		SIN_MAX_EFFECT				1000	//����Ʈ �̹��� ������ �ִ�ġ 
#define		SIN_MAX_PROCESS				100		//����Ʈ ���μ��� 

//////// ����Ʈ�� TYPE
#define		SIN_EFFECT_PARTICLE			0x01000000
#define		SIN_EFFECT_FACE2D			0x02000000
#define		SIN_EFFECT_OBJECT			0x03000000
#define		SIN_EFFECT_PATMESH			0x04000000

/////// ����Ʈ ����
#define SIN_EFFECT_1					0x00000001
#define SIN_EFFECT_2					0x00000002
#define SIN_EFFECT_3					0x00000003
#define SIN_EFFECT_4					0x00000004
#define SIN_EFFECT_5					0x00000005
#define SIN_EFFECT_6					0x00000006
#define SIN_EFFECT_7					0x00000007
#define SIN_EFFECT_8					0x00000008
#define SIN_EFFECT_9					0x00000009
#define SIN_EFFECT_10					0x00000010

/////// ����Ʈ ����ũ 

#define	sinEFFECT_MASK1	0xFF000000
#define	sinEFFECT_MASK2	0xFFFF0000
#define	sinEFFECT_MASK3	0x0000FFFF


////////������ ����Ʈ

#define		SIN_PARTICLE_SPOUT			(SIN_EFFECT_PARTICLE | SIN_EFFECT_1)		//������
#define		SIN_PARTICLE_GATHER			(SIN_EFFECT_PARTICLE | SIN_EFFECT_2)		//������
#define		SIN_PARTICLE_ROTATE_RISE	(SIN_EFFECT_PARTICLE | SIN_EFFECT_3)		//ȸ�� ����� 
#define		SIN_PARTICLE_WIDE			(SIN_EFFECT_PARTICLE | SIN_EFFECT_4)		//�а� ��������

#define		SIN_EFFECT_WIDE_LINE		(SIN_EFFECT_OBJECT | SIN_EFFECT_1)			//������Ʈ ������� 
#define		SIN_EFFECT_RISE_LINE		(SIN_EFFECT_OBJECT | SIN_EFFECT_2)			//������Ʈ ��¶��� 

///////���� �� ��ƼŬ
#define		SIN_PARTICLE_COLOR_GOLD		1		//�ݻ�
#define		SIN_PARTICLE_COLOR_BLUE		2		//û��
#define		SIN_PARTICLE_COLOR_RED		3		//����
#define		SIN_PARTICLE_COLOR_GREEN	4		//���

///////����Ʈ�� ����
#define		SIN_EFFECT_TYPE_NORMAL		1		//����
#define		SIN_EFFECT_TYPE_FADEON		2		//��ƼŬ�� ���� (����ѻ���)
#define		SIN_EFFECT_TYPE_EXPAND		3		//��ƼŬ�� Ŀ���� 
#define		SIN_EFFECT_TYPE_DECREASE	4		//��ƼŬ�� �۾����� 
#define		SIN_EFFECT_TYPE_FLICKER		5		//�����̴�
#define		SIN_EFFECT_TYPE_FADEOUT		6		//������ �������

///////����Ʈ�� ����
#define		SIN_EFFECT_DEAD				0		//��ƼŬ�� �������
#define		SIN_EFFECT_ALIVE			1		//��ƼŬ�� ����ִ�


/*----------------------------------------------------------------------------*
*						    ����Ʈ ����ü 
*-----------------------------------------------------------------------------*/	
struct sinEFFECT{
	int			CODE;				//����
	int			Color;				//��
	int			State;				//���� 
	
	smFACE2D	sinFace;			//���̽�
	
	POINT3D		MoveSpeed;			//�̵� �ӵ�
	POINT3D		MoveAngle;			//ȸ�� ��ǥ
	int			MoveGravity;		//�̵� �߷�
	
	POINT3D		ObjectPosi;			//������Ʈ�� ������ǥ (FACE�� ������� ������ ����)
	int			ObjectMat;			//������Ʈ ��Ʈ����
	POINT3D		ObjectSize;			//������Ʈ ������ 
	
	
	int			FaceAngle;			//ȸ����
	
	int			LifeTime;			//��� �ð� (70������)
	int			LifeTime_MAX;		//��� �ð� �ִ�ġ

	int			AniCount;			//���ϸ��̼� ī��Ʈ
	int			ChangeSize;			//���̽��� ũ�⸦ ��ȭ��Ų��

	int			RotateRadian;		//ȸ���� ����
	int			RotateDistance;		//ȸ���� ����� �Ÿ� 

	int			TypeSize;			//ũ����� Ÿ��
	int			TypeFade;			//Fade���� Ÿ��
	smPAT3D     *sinPatMesh;		//���� 
	smOBJ3D		*sinObj;			//������Ʈ 
	
	int			MeshFlag;			//���� �Ž������� üũ�Ѵ� 
	
};
/*----------------------------------------------------------------------------*
*						    ����Ʈ ǥ���� ���� ����ü
*-----------------------------------------------------------------------------*/	
struct sinEFFECT_PROCESS{
	int Kind;
	int Time;
	POINT3D	Posi;
	int Index[100];

};

/*----------------------------------------------------------------------------*
*						    ������ �ϱ����� ����ü
*-----------------------------------------------------------------------------*/	
struct sinEFFECT_ZDEPTH{
	int		Index;
	int		ZDepth;
	
};

/*----------------------------------------------------------------------------*
*						      �Լ� 
*-----------------------------------------------------------------------------*/	
smFACE2D sinSetFace(int Mat,int x , int y, int z, int w, int h , int R , int G , int B , int A);

//����Ʈ ���� 
int sinStartEffect(int Name , int x , int y, int z);

//����Ʈ�� �����Ѵ� 
int sinSetEffect(int CODE , int x, int y, int z , int Size , int Color=0 , int AngleY = 0 ); 

//����Ʈ ��ȭ
int sinActiveEffect();

//�ִϸ��̼� 
int sinEffectAni(sinEFFECT *pEffect);

//����Ʈ�� �����δ� 
int sinMoveEffect(sinEFFECT *pEffect);

//����Ʈ�� �׸��� 
int sinDrawEffect(int x, int y,int z,int ax, int ay, int az);

//����Ʈ�� ZDepth�� ���� �����Ѵ� 
int sinBubbleSort(sinEFFECT_ZDEPTH *list, int num);

//����Ʈ Process
int sinEffectProcess();

//����Ʈ Process����
int sinSetEffectProcess(int Kind , int x, int y , int z);

//����Ʈ Object�� �����Ѵ� 
int sinCreateEffectObject(sinEFFECT *pEffect);

//�̵��� ���� ��ǥ���� ���Ѵ� 
int sinGetMoveLocation(sinEFFECT *pEffect);

//����׿� TEXT�� ����ش� 
int EffectTestDrawText();

////////////TEST
int TestObjectDraw(int x, int y, int z, int ax, int ay, int az );

int TestCreateEffectObject(); //�ӽ� ������Ʈ�� ������ش� 
/*----------------------------------------------------------------------------*
*						   Extern
*-----------------------------------------------------------------------------*/	
extern smTEXRECT sinTexRect;
extern sinEFFECT sinEffect[SIN_MAX_EFFECT];
extern sinEFFECT_ZDEPTH	sinEffect_ZDepth[SIN_MAX_EFFECT];
