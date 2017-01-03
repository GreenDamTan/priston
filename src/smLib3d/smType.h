
#define MAP_SIZE			256
#define MAP_CLIP			0xFF	//(AND 0xFF)
#define MAX_CELL			4096
#define MAPTEXTURE_SIZE		64
#define RAYCLIP_ANGLE		ANGLE_45+ANGLE_1
//#define RAYCLIP_ANGLE		260
#define RAYCLIP_ADD			5

#define ANGCLIP				ANGLE_MASK
#define CLIP_OUT			-32767
#define SMFLAG_NONE			0xFFFFFFFF
/*
#define	fONE		512
#define FLOATNS		9
#define FLOATDS		7
*/


///////////////////// ���� �Ҽ��� ���� //////////////////
#define	fONE		256
#define FLOATNS		8
#define FLOATDS		8

//MATRIX�� 16��Ʈ �Ҽ���
#define wfONE		32768
#define wFLOATS		15
#define wSHIFT_FLOAT	(wFLOATS-FLOATNS)

///////////////////// �� ũ�� ���� ���� ///////////////////
#define SizeMAPCELL			64			//�ʻ��� 1 CELL ���� ũ��
#define ShiftMAPCELL_MULT	6		    //�ʻ��� 1 CELL ���� ũ�� ����Ʈ ��
#define ShiftMAPCELL_DIV	6		    //�ʻ��� 1 CELL ���� ũ�� ����Ʈ ����

#define SHIFT_MAPHEIGHT		(ShiftMAPCELL_MULT-3)
#define AND_SizeMAPCELL		(SizeMAPCELL-1)
#define OR_SizeMAPCELL		(0xFFFFFFFF-AND_SizeMAPCELL)

//////////////////// ������ �� ��� �⺻ ��� ////////////////////
#define DEFAULT_CONTRAST			300
#define DEFAULT_BRIGHT				160



////////////////  smRENDER Ŭ���� �䱸 ���� �� ( ClipStatus ) //////////////////
#define SMCLIP_NEARZ		0x00000001
#define SMCLIP_FARZ			0x00000002
#define SMCLIP_LEFT			0x00000004
#define SMCLIP_RIGHT		0x00000008
#define SMCLIP_TOP			0x00000010
#define SMCLIP_BOTTOM		0x00000020
#define SMCLIP_TEXTURE		0x00000040
#define SMCLIP_DISPLAYOUT	0x00000080
#define SMCLIP_DISPLAYIN	0x00000100

#define SMCLIP_VIEWPORT		( SMCLIP_NEARZ | SMCLIP_FARZ | SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM | SMCLIP_DISPLAYOUT )

////////////////// RGBA �迭 //////////////////////
#define SMC_A				3
#define SMC_R				2
#define SMC_G				1
#define SMC_B				0



//######################################################################################
// �� �� �� : �� �� ��
// ������� ���� �϶�� �ϼ���.
// �ؽ��� �ִ�ġ

#include "..\\nettype.h"
#ifdef _W_SERVER
#define MAX_TEXTURE		8000
#else
#define MAX_TEXTURE		5000
#endif
//######################################################################################





//���� �Ҽ��� ���
struct smMATRIX { 
    int _11, _12, _13, _14; 
    int _21, _22, _23, _24; 
    int _31, _32, _33, _34; 
    int _41, _42, _43, _44; 
};

//���� �Ҽ��� ���2
struct smEMATRIX { 
    int _11, _12, _13, _14; 
    int _21, _22, _23, _24; 
    int _31, _32, _33, _34; 
    int _41, _42, _43, _44; 
};


//�ε� �Ҽ� Double �� ���
struct smDMATRIX { 
    double _11, _12, _13, _14; 
    double _21, _22, _23, _24; 
    double _31, _32, _33, _34; 
    double _41, _42, _43, _44; 
};

//�ε� �Ҽ� float �� ���
struct smFMATRIX { 
    float _11, _12, _13, _14; 
    float _21, _22, _23, _24; 
    float _31, _32, _33, _34; 
    float _41, _42, _43, _44; 
};

struct smRGB {
	DWORD r,g,b;
};
struct smLIGHTLEVEL {
	BYTE		cr,cg,cb;
	BYTE		sr,sg,sb;
};

struct smLIGHT3D {
	int type;
	int x,y,z;
	int Range;
	short r,g,b;

};

#define	smLIGHT_TYPE_NIGHT		0x00001
#define	smLIGHT_TYPE_LENS		0x00002
#define	smLIGHT_TYPE_PULSE2		0x00004				// ���̴� ���� ����. ???
//######################################################################################
//�� �� �� : �� �� ��
#define	SMLIGHT_TYPE_OBJ		0x00008
//######################################################################################
#define	smLIGHT_TYPE_DYNAMIC	0x80000



struct POINT3D {
	int x,y,z;
};

struct TPOINT3D {
	int x,y,z;
	float u,v;
	float zb;
	BYTE	bCol[4];
	BYTE	bSpe[4];
};

struct smLINE3D {
	POINT3D sp;
	POINT3D ep;
};

struct smTRECT {
	float u0,v0;
	float u1,v1;
	float u2,v2;
	float u3,v3;
};

struct smTPOINT {
	int u,v;				
};

struct smFTPOINT {
	float u,v;				
};



// 3�� �������� �ؽ��� ��ǥ �� ���� ����Ʈ
struct smTEXLINK {
	float	u[3],v[3];	
	DWORD	*hTexture;
	smTEXLINK	*NextTex;
};

struct smVERTEX{
	int x,y,z;
	int nx,ny,nz;
};

struct smFACE{
	WORD v[4];			// a,b,c , matrial
	smFTPOINT t[3];		// �ؽ��� ����Ʈ

	smTEXLINK	*lpTexLink;		// �ؽ��� ���� ��ǥ 

};

struct smTM_ROT {
	int frame;
	float x,y,z,w;
};
struct smDTM_ROT {
	int frame;
	double x,y,z,w;
};

//######################################################################################
//�� �� �� : �� �� ��
struct smTM_POS {
	int   frame;
	float x, y, z;
};
//######################################################################################

struct smTM_SCALE {
	int frame;
	int x,y,z;
};


// �������� ���� ���ؽ�
struct smRENDVERTEX {

	// ���� ���ؽ�
	int		tx,ty,tz;			// Transrate ī�޶� ��ǥ
	DWORD	ClipStatus;			// Ŭ���� �䱸 ����
//	short	xy[2];				// 2D ��ȯ ��ǥ
	float	xy[2];				// 2D ��ȯ ��ǥ
	float	zb;					// Z Buffer ����
	float	rhw;				// rhw

	//######################################################################################
	//�� �� �� : �� �� ��
	int		nx,ny,nz;
	//######################################################################################

	short	sLight[4];			// ����� ���� (RGBA)

	BYTE	bCol[4];			// ���ؽ� ���� (RGBA)
	BYTE	bSpe[4];			// ���ؽ� Specular (RGBA)

	void	*lpSourceVertex;	//���� ���ؽ� ������
};

// �������� ���� ���̽�
struct smRENDFACE {

	smRENDVERTEX	*lpRendVertex[3];	// a,b,c
	DWORD			Matrial;			// ��Ʈ����
	smTEXLINK		*lpTexLink;			// �ؽ��� ���� ��ǥ 
	DWORD			ClipStatus;			// Ŭ���� �䱸 ����
	smRENDFACE		*NexRendFace;		// ���� ���������̽� ������

};

//�������� �� ����ü
struct smRENDLIGHT {
	int type;
	int x,y,z;
	int rx,ry,rz;
	int Range;
	int dRange;
	int r,g,b,a;
};


//��Ʈ���� ������ ���̽��� ����ü
struct smRENDMATRIAL {

	int RegistRendList;					// ������ ����Ʈ�� ��� ���� ǥ��

	// �Ϲ���
	DWORD		MatrialCounter;			// �������� ���̽��� �� ( ��Ʈ���� �� )
	smRENDFACE	*StartFace;				// �������� ���� ���̽� ������
	smRENDFACE	*LastLinkFace;			// ���������� ������ ���̽�

	// �ؽ��� Ŭ����
	DWORD		TexClip_MatrialCounter;	// �������� ���̽��� �� ( ��Ʈ���� �� )
	smRENDFACE	*TexClip_StartFace;		// �������� ���� ���̽� ������
	smRENDFACE	*TexClip_LastLinkFace;	// ���������� ������ ���̽�
};

//�ؽ��� ����
struct smTEXRECT {
	float	left,	right;
	float	top,	bottom;
};

//2D �̹��� ���̽�
struct smFACE2D {
	int			x, y, z;			// ��ǥ
	int			width , height;		// �̹��� ũ��
	smTEXRECT	TexRect;			// �ؽ��� ����
	int			MatNum;				// ��Ʈ���� ��ȣ
	int			Transparency;		// ����
	int			r,g,b;
};


// ���ؽ�
struct smSTAGE_VERTEX {
	DWORD			sum;				// �ֱ� ���� ��ȣ 
	smRENDVERTEX	*lpRendVertex;		// �������� ���� ���ؽ� ������

	// �ҽ� ���ؽ�
	int				x,y,z;				// ���� ��ǥ
	//BYTE			bDef_Color[4];		// �⺻ ���� (RGBA)(Color )
	//BYTE			bDef_Specular[4];	// �⺻ ���� (RGBA)(Specular )

	short			sDef_Color[4];		// �⺻ ���� ( RGBA )

};	// ���� ũ�� 28 ����Ʈ


// ���̽�
struct smSTAGE_FACE {
	DWORD	sum;				// �ֱ� ���� ��ȣ
	int		CalcSum;			// ��Ÿ ���� ��ȣ

	WORD	Vertex[4];			// a,b,c,Matrial

	smTEXLINK	*lpTexLink;		// �ؽ��� ���� ��ǥ 

	short	VectNormal[4];			// Cross����( Normal )  ( nx , ny , nz , [0,1,0]���� Y ); 
	//int		NormalY;			// ( 0,1,0 ) ������ DotProduct ��
};




#define		CONFIG_KEY_MONSTER_MAX		5

//�⺻ ���� ����ü
struct smCONFIG {

	int		WinMode;					//ȭ�� ���
	POINT	ScreenSize;					//ȭ�� �ػ�
	DWORD	ScreenColorBit;				//���� ��Ʈ
	
	int		TextureQuality;				//�ؽ��� ǰ��
	int		BGM_Mode;					//��� ���� ��� 
	int		NetworkQuality;				//��Ʈ��ũ ǰ��
	int		WeatherSwitch;				// pluto �ؿ� ��

	char	szFile_BackGround[2][64];	//��� ȭ��
	char	szFile_Menu[64];			//�޴� ȭ��
	char	szFile_Player[64];			// ���ΰ�
	POINT	Posi_Player;
	char	szFile_Enemy[64];			// ���۳�
	POINT	Posi_Enemy;

	char	szFile_Stage[64];				//����
	POINT	Posi_Stage;						//���� ���� ��ġ
	char	szFile_StageObject[100][64];	//������ �ö󰡴� ������Ʈ
	int		StageObjCnt;					//���� ������Ʈ ����

	//######################################################################################
	//�� �� �� : �� �� ��
	int		IsStageAniObject[100];
	//######################################################################################

	//�ű� �� �ҷ� �ö��� �����
	int		MapBright;						//���
	int		MapContrast;					//����
	POINT3D	MapLightVector;					//����

	char	szServerIP[32];					//�����ּ�
	DWORD	dwServerPort;					//������Ʈ
	char	szDataServerIP[32];				//����Ÿ �����ּ�
	DWORD	dwDataServerPort;				//����Ÿ ������Ʈ
	char	szUserServerIP[32];				//���� �����ּ�
	DWORD	dwUserServerPort;				//���� ������Ʈ
	char	szExtendServerIP[32];			//Ȯ�� �����ּ�
	DWORD	dwExtendServerPort;				//Ȯ�� ������Ʈ

	DWORD	DebugMode;						//����� ��� ����


	char	szCmdOpenMonster[CONFIG_KEY_MONSTER_MAX][32];		//������ų ���� �̸�
	int		CmdMonsterCount;
};




#define MOTION_LIST_MAX			32
#define MOTION_INFO_MAX			512

//DRZ_EDIT (change this val to 104 for reading new INX files)
//old one > 
//#define	MOTION_TOOL_MAX			52

#define	MOTION_TOOL_MAX			104


#define	MOTION_SKIL_MAX			8


//######################################################################################
//�� �� �� : �� �� ��
// ������ �����ϰ� ������ ���ؼ� (% ���� ����.)  �۵��� �Ҷ���.
// �� ������ 2 �ܰ�� (ǥ��) (���ϱ�).  <- ǥ�� ��� ���ļ� 100%, ���ϱ� ��� ���ļ� 100%
// NPC  ������ 1 �ܰ�� �۵� �մϴ�.      <- ��� ��� ���ļ� 100%.
#define NPC_MOTION_INFO_MAX		 30
#define TALK_MOTION_INFO_MAX	 30

// MotionFrame  ����.
// ������� ����Ÿ�� ����.  "MotionFrame-1" �� ���.
// ���� �������. (��)  MotionFrame �� �״�� ����. 
#define TALK_MOTION_FILE_MAX	 2
#define TALK_MOTION_FILE		 0
#define FACIAL_MOTION_FILE		 1
//######################################################################################

struct smMOTIONINFO
{
	DWORD	State;							//���� TRUE�� ��ȿ
	
	//######################################################################################
	//�� �� �� : �� �� ��
	DWORD	MotionKeyWord_1;
	DWORD	StartFrame;						//���� ������
	DWORD	MotionKeyWord_2;
	DWORD	EndFrame;						//���� ������
	//######################################################################################

	DWORD	EventFrame[4];					//�̺�Ʈ ���� ������

	int		ItemCodeCount;					//�ش� ������ ����Ʈ ī���� ( 0 ����  -1 ��ü �ش� )
	BYTE	ItemCodeList[MOTION_TOOL_MAX];	//�ش� ������ �ڵ� ����Ʈ 
	DWORD	dwJobCodeBit;					//�ش� ������ ��Ʈ ����ũ
	BYTE	SkillCodeList[MOTION_SKIL_MAX];	//�ش� ��ų ��ȣ
	//BYTE	JobCodeList[MOTION_JOB_MAX];	//�ش� ��ų ��ȣ

	int		MapPosition;					//�ش� �� ���� ( 0-������� ���� 1 - �ʵ� 2 )

	DWORD	Repeat;							//�ݺ� ����
	CHAR	KeyCode;						//���� ���� Ű
	int		MotionFrame;					//��� ���� ���� ��ȣ
};

struct _MODELGROUP {
	int ModelNameCnt;
	char szModelName[4][16];
};

struct smMODELINFO {

	char	szModelFile[64];
	char	szMotionFile[64];
	char	szSubModelFile[64];

	_MODELGROUP	HighModel;
	_MODELGROUP	DefaultModel;
	_MODELGROUP	LowModel;

	smMOTIONINFO	MotionInfo[MOTION_INFO_MAX];
	DWORD	MotionCount;

	//######################################################################################
	//�� �� �� : �� �� ��
	DWORD			FileTypeKeyWord;
	DWORD			LinkFileKeyWord;
	//######################################################################################

	//######################################################################################
	//�� �� �� : �� �� ��
	char			szLinkFile[64];
	//######################################################################################

	//######################################################################################
	//�� �� �� : �� �� ��
	// ******************** ���� ******************** 
	// �� ���� 2���� *.ini  *.in  ���ϸ��� ���� ���. �Ѵ� ���� �̸��� *.inx �� �����ⶫ��.
	// ���ϸ��� �޶�� �մϴ�.
	char			szTalkLinkFile[64];	
	char			szTalkMotionFile[64];
	smMOTIONINFO	TalkMotionInfo[ TALK_MOTION_INFO_MAX ];
	DWORD			TalkMotionCount;

	int				NpcMotionRate[NPC_MOTION_INFO_MAX];
	int				NpcMotionRateCnt[ 100 ];

	int				TalkMotionRate[ TALK_MOTION_INFO_MAX ];
	int				TalkMotionRateCnt[ TALK_MOTION_FILE_MAX ][ 100 ];
	//######################################################################################	
};




extern BYTE VRKeyBuff[256];





struct smTEXPOINT {
	float	u,v;
	DWORD	hTexture;
};

struct smFCOLOR {
	float	r,g,b;
};

//�ؽ��� �� ������ ��ǥ �Ӽ�
#define smTEXSTATE_FS_NONE		0
#define smTEXSTATE_FS_FORMX		1
#define smTEXSTATE_FS_FORMY		2
#define smTEXSTATE_FS_FORMZ		3
#define smTEXSTATE_FS_SCROLL	4
#define smTEXSTATE_FS_REFLEX	5

#define smTEXSTATE_FS_SCROLL2	6
#define smTEXSTATE_FS_SCROLL3	7
#define smTEXSTATE_FS_SCROLL4	8
#define smTEXSTATE_FS_SCROLL5	9
#define smTEXSTATE_FS_SCROLL6	10
#define smTEXSTATE_FS_SCROLL7	11
#define smTEXSTATE_FS_SCROLL8	12
#define smTEXSTATE_FS_SCROLL9	13
#define smTEXSTATE_FS_SCROLL10	14

//######################################################################################
//�� �� �� : ������
#define smTEXSTATE_FS_SCROLLSLOW1	15
#define smTEXSTATE_FS_SCROLLSLOW2	16
#define smTEXSTATE_FS_SCROLLSLOW3	17
#define smTEXSTATE_FS_SCROLLSLOW4	18
//######################################################################################


/////////////////////////// Texture Map �ڵ� ���� //////////////////////////
struct smTEXTUREHANDLE {
	char Name[64];
	char NameA[64];
    DRZTEXTURE2 lpD3DTexture;
	DIRECTDRAWSURFACE lpDDSurface;

    DRZTEXTURE2 lpD3DTextureLarge;		//ū�������� �ؽ��� ( �ý��� �޸𸮿��� �������� ����� )
	DIRECTDRAWSURFACE lpDDSSysMemory;		//�ý��� �޸� �ؽ���

	int Width , Height;
	int UsedTime;

	int UseCounter;

	int	MapOpacity;								//�� ���ǽ�Ƽ ���� ( TRUE , FALSE )

	DWORD	TexSwapMode;						//���� �ؽ��ĸ� ��뿩�� ( TRUE / FALSE )

	smTEXTUREHANDLE *TexChild;

	DWORD loadingFailed;
};


/////////////////////////// ASE ���� �о� ���� ��Ʈ���� //////////////////////////
struct ASE_MATERIAL {
	int			Regist;						//smMATERIAL �� ��� ����
	int			RegistNum;					//smMATERIAL �� ��ϵ� ��ȣ

	int			TextureCounter;				//��Ʈ�� ��� ����
	DWORD		UseCounter;					//��� ī���� ( ������ 0 �̸� ���� ���� )

	smFCOLOR	Diffuse;					// Diffuse ����
	float		Transparency;				// ����
	float		SelfIllum;					// ��ü �߱�
	DWORD		TwoSide;					// ��� ǥ�� ��� ( �������� ǥ�� ���� )
	DWORD		ScriptState;				// ��ũ��Ʈ�� ���� �Ӽ�

	//######################################################################################
	//�� �� �� : �� �� ��
	char		BITMAP[8][64];				// Bitmap �̸�
	//######################################################################################

	DWORD		BitmapStateState[8];		// ��Ʈ�� �������� �Ӽ�
	DWORD		BitmapFormState[8];			// ��Ʈ�� �� �Ӽ�

	//######################################################################################
	//�� �� �� : �� �� ��
	char		MAP_OPACITY[64];			// ������ �� �̸�
	//######################################################################################

	float		UVW_U_OFFSET[8];			// �� OFFSET
	float		UVW_V_OFFSET[8];			//
	float		UVW_U_TILING[8];			// �� TILING
	float		UVW_V_TILING[8];
	float		UVW_ANGLE[8];				// �� ANGLE

	int			SubPoint;					// ���� ��Ʈ���� ��ȣ

	int			BlendType;					//���� Ÿ��

};



//////////////////////////////// ��Ʈ����  //////////////////////////////
struct smMATERIAL {
	DWORD			InUse;						//��Ʈ���� ��� ����
	DWORD			TextureCounter;				//�ؽ��� ī����
	smTEXTUREHANDLE	*smTexture[8];				//�ؽ��� �ڵ� ����Ʈ
	//�Ӽ�
	DWORD			TextureStageState[8];		//�׸��� �Ӽ� 
	DWORD			TextureFormState[8];		//�� �Ӽ�  
	int				ReformTexture;				//�ؽ��� �����Ӽ� Ȯ��

	int				MapOpacity;					//�� ���ǽ�Ƽ ���� ( TRUE , FALSE )

	//�Ϲ����� �Ӽ�
	DWORD			TextureType;				//�ؽ��� Ÿ�� ( ��Ƽ�ͽ� / �ִϸ��̼� )
	DWORD			BlendType;					//�귣�� ���� ( SMMAT_BLEND_XXXX )

	DWORD			Shade;						//���̵� ��� ( �뽦�̵� / �׷ν��̵� )
	DWORD			TwoSide;					//��� ��� ����
	DWORD			SerialNum;					//��Ʈ���� ���� ���� ��ȣ

	smFCOLOR		Diffuse;					// Diffuse ����
	float			Transparency;				// ����
	float			SelfIllum;					// ��ü �߱�

	int				TextureSwap;				// �ؽ��� ������
	int				MatFrame;					// ��������� ( ���� ���⸦ ���߱� ���� )
	int				TextureClip;				// �����ο� �ؽ��� Ŭ������ ( TRUE �� �ؽ��� Ŭ���� �㰡 )

	//�޽� ���� �Ӽ�
	int				UseState;					// �뵵 ( ��� �Ӽ� )
	int				MeshState;					// MESH�� ���� �Ӽ��� 

	//Mesh ���� ���� ����
	int				WindMeshBottom;				// �ٶ��ұ� �޽� ���� ���� ��

	//���ϸ��̼� �ؽ��� �Ӽ�
	smTEXTUREHANDLE	*smAnimTexture[32];			//�ִϸ��̼� �ؽ��� �ڵ� ����Ʈ
	DWORD			AnimTexCounter;				//�ֹ̳��̼� �ؽ��� ī����
	DWORD			FrameMask;					//�ִϸ��̼ǿ� ������ ����ũ
	DWORD			Shift_FrameSpeed;			//������ ���� �ӵ� ( Ÿ�̸Ӹ� Shift �Ͽ� ��� )
	DWORD			AnimationFrame;				//������ ��ȣ ( ������ ��� �����Ӱ� / SMTEX_AUTOANIMATION �� �ڵ� )


};

//////////////// ��Ʈ���� �Ӽ� ���� ���� ////////////////////

//TextureType ����
#define SMTEX_TYPE_MULTIMIX		0x0000
#define SMTEX_TYPE_ANIMATION	0x0001


//Texture ���ϸ��̼� �������� �ڵ��� ���
#define SMTEX_AUTOANIMATION		0x100


//MeshState ����
//�ٴ� üũ ����
#define SMMAT_STAT_CHECK_FACE	0x00000001

//
#define SMMAT_BLEND_NONE		0x00
#define SMMAT_BLEND_ALPHA		0x01
#define SMMAT_BLEND_COLOR		0x02
#define SMMAT_BLEND_SHADOW		0x03
#define SMMAT_BLEND_LAMP		0x04
#define SMMAT_BLEND_ADDCOLOR	0x05
#define SMMAT_BLEND_INVSHADOW	0x06



////////////////////////////////////////////////////////////////



//////////////// ASE ����Ʈ ������Ʈ ////////////////////
struct smASE_LIGHT {

	int	x,y,z;
	int r,g,b;

	int Size;
	int Range;
	int	Dist;
	int Type;
};


//ī�޶� ���� ��ǥ
struct eCAMERA_TRACE {
	int x,y,z;					//���� ī�޶� ��ǥ
	int tx,ty,tz;				//��ǥ�� ��ǥ

	smEMATRIX	eRotMatrix;		//ȸ�� ���� ���
	int AngX,AngY;				//������ ���� ( ���� ����ŭ )
};


