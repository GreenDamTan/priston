/*----------------------------------------------------------------------------*
*	���ϸ� :  sinEffect2.h	
*	�ϴ��� :  ����Ʈ ���� �۾� (������ ����Ʈ )
*	�ۼ��� :  ����������Ʈ 2002�� 6��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#define SIN_MAX_EFFECT2			1000

#define SIN_EFFECT_STUN			1
#define SIN_EFFECT_ICEVIGO		2
#define SIN_EFFECT_BOSSAURA		3
#define SIN_EFFECT_BOSSAURA2	4


#define SIN_EFFECT_NO_TIME		0xFFFF0000

#define SIN_EFFECT_FACE			1
#define SIN_EFFECT_MESH			2

/*----------------------------------------------------------------------------*
*						Effect2 Ŭ���� 
*-----------------------------------------------------------------------------*/	
class cSINEFFECT2{

public:

	DWORD CODE;
	int   Time;
	int   Max_Time;
	int   Index;

public:

	//////// ��ǥ���� 
	POINT3D		Posi;
	POINT3D		Angle;
	POINT3D		AngleTemp;			//��꿡���� ���ȴ� 
	POINT3D		RanderAngle;		//������ Angle
	POINT3D		SortPosi;			//���� ��ǥ 
	int			FaceAngleY;			//���̽� �ޱ�Y
	POINT3D		DesPosi;			//������ ��ġ ��ǥ(���Ǽ������� �����Ѵ�)
	int			AddHeight;			

	//////// Draw ���� 
	int			Flag;				//�÷� 
	smFACE2D	sinFace;			//���̽� 
	smPAT3D     *sinPatMesh;		//���� 
	smOBJ3D		*sinObj;			//������Ʈ 
	int			ObjMat;				//������Ʈ ��Ʈ����

	//////// �ִϸ��̼� 
	int			AniCount;
	int			AniTime;
	int			AniMax;
	int			AniReverseNum;		//�ִϸ��̼� �����÷� ( 1�ٸ����� 2 ���� ) 

	int			*lpMatAni;

	//////// Move ���� 
	int			MoveKind;
	POINT3D		MoveSpeed;
	int			Gravity;
	int			DesSpeed;			//������ ������ ���ǵ� 
	POINT3D		MoveAngle;			//������ ���ؼ� ������ 
	int			MoveStartTime;		
	
	//////// Roatate ���� 
	POINT3D		RotateDistance;
	int			RotateSpeed;
	int			RotateSpeedCnt;
	POINT3D		RotatePosi;
	POINT3D		CharDistance;
	int			RotateAngle;

	//////// Size���� 			
	POINT		Size;
	int			SizeIncreTime;
	int			SizeDecreTime;
	int			SizeAmount;
	int			ReSizingNum;
	int			ReSizingAmount;

	//////// Char ���� 
	smCHAR		*pChar;		//�ڽ� 
	smCHAR		*DesChar;	//����(����)
	POINT3D		CharPosi;		//���� ��ġ 
	POINT3D		DesCharPosi;	//������ ��ġ 

	//////// Color ���� 
	int			Color_A;
	int			Color_R;
	int			Color_G;
	int			Color_B;
	
	int			AlphaCount;
	int			AlphaTime;
	int			AlphaAmount;

	int			AlphaReverse_A;	 //���� �� ���� ��
	int			AlphaReverseNum; //���� ������ 

	int			AlphaFadeInFlag;	 //���� ���̵���

	//////// Process
	int			ProcessCount;

	////////
	int			CopyFlag;
	//////// Action
	int			ActionTime[10]; //�ٸ� �׼��� ������� 
	int			BoneFlag;		//�׷������ʴ� ����

	int			CrashCheckCount[3];
public:


};
extern POINT3D	sinPosi2;
extern POINT3D	sinAngle2;

/*----------------------------------------------------------------------------*
*						 ����Ʈ ���� struct
*-----------------------------------------------------------------------------*/	

//���� ���� ����Ʈ (�ٴ�)
int setBossMonsterEffect(smCHAR *pChar,int ProcessCount);

//���� ���� ����Ʈ2 (����)
int setBossMonsterEffect2(POINT3D Posi);

//����  
int sinSetEffect_Stun(smCHAR *pChar ,int Time);

//������ ��� 
int sinSetEffect_IceVigo(smCHAR *pChar ,  int Time);
/*----------------------------------------------------------------------------*
*						     ��ƼŬ 	
*-----------------------------------------------------------------------------*/	
int sinSetParticleRound(POINT3D Posi);   //�ձ۰� ������ ����Ʈ 

int sinSetParticleTail(POINT3D Posi);    //�����ʿ��� �𳯸��� ��ƼŬ 

int ParticleTestKey(); //��ƼŬ �׽�Ʈ Ű 

int sinHitParticle(int Mat , POINT3D Posi);		//Ÿ�� ��ƼŬ 

int sinTornadoParticle(int Mat , POINT3D Posi); //ȸ���� ��ƼŬ 

int sinSparkParticle(int Mat , POINT3D Posi);   //����ũ ��ƼŬ 

int sinParticleLine(int Mat , POINT3D Posi , int R ,int G , int B); //��ƼŬ ���� 

int sinGetMoveLocation2(cSINEFFECT2 *pEffect);		//�̵��� ��ǥ�� ���Ѵ� 

int sinSetLightEffect(int R , int G , int B , int A , POINT3D Posi);

int sinSongPyeunEffect(POINT3D Posi); //���� ��ƼŬ 

/*----------------------------------------------------------------------------*
*					
*-----------------------------------------------------------------------------*/	

//Effect �̹��� �ʱ�ȭ 
int InitEffect2();

//����Ʈ�� ���õ� ����� ã�Ƽ� Index�� �����Ѵ� 
int sinSearchEmptyIndex();

//����Ʈ�� �׸��� 
int sinDrawEffect2(int x, int y,int z,int ax, int ay, int az);

//2D �̹����� �׸��� 
int sinDrawTexture2(cSINEFFECT2 *pSinEffect2);

//������Ʈ�� �����Ѵ� 
int sinCreateObject(cSINEFFECT2 *pEffect);

//����Ʈ�� �����δ� 
int sinMoveEffect2(cSINEFFECT2 *pEffect);

//EFFECT ACTIVE!
int sinActiveEffect2();

//���� ���� 
int sinBubbleSort2(cSINEFFECT2 *list, int num);

//�׽�Ʈ �ؽ�Ʈ 
int EffectTestDrawText2();

//ĳ���� ������ ����Ʈ�� �����Ѵ� 
int sinCheckCharState(cSINEFFECT2 *pEffect);

//����Ʈ �ʱ�ȭ 
int sinReleaseEffect();

//��ǥ�� �����ؼ� �����Ѵ� 
int sinReSetObjPosi(cSINEFFECT2 *pEffect);

//����Ʈ �⺻���� 
int sinEffectDefaultSet(int Index , int Kind ,  smCHAR *pChar=0 , POINT3D *pPosi=0 , int Y=0 );

//Rotate ��ǥ ����
int sinSetRotatePosiReSet(cSINEFFECT2 *pEffect);
////////////////// TEST
int CheeseTestEffectImage(); //ġ���� �׽�Ʈ �̹��� 

/*----------------------------------------------------------------------------*
*								Extern 
*-----------------------------------------------------------------------------*/	
extern cSINEFFECT2 cSinEffect2[SIN_MAX_EFFECT2];					//����Ʈ Ŭ���� 1000�� 
extern sinEFFECT_ZDEPTH sinEffect_Zdepth2[SIN_MAX_EFFECT2];		    //������ ���� �ε��� ����ü 


int SetDynLight( int x,int y,int z, int r, int g, int b, int a, int power , int DecPower);

//��ġ�� �̹��� 
extern int Matstun;