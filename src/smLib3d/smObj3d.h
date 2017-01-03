#include "smDsx.h"

//#include "smMatrix.h"
class smPAT3D;

#define OBJ_FRAME_SEARCH_MAX		32

//######################################################################################
//�� �� �� : �� �� ��
#define OBJ_HEAD_TYPE_NEW_NORMAL				0x80000000
//######################################################################################

// �������� ���ؽ� ������
class smPOINT3D {
public:

	smVERTEX	*pVertex;			// ��ǥ ����Ʈ

	int			rx , ry , rz;		// ȸ�� ��ǥ
	int			wx , wy , wz;		// ���� ��ǥ
	int			sx , sy , sz;
	int			ox , oy , oz;

	int			X,Y,Z;				// ī�޶� ���� ��ǥ
	int			x2d,y2d;			//������ �� 2D ��ǥ
	BYTE		Clip2d[4];			//ȭ��� ���ؽ� Ȯ��

	smPOINT3D();
	smPOINT3D( smVERTEX *pv );
	~smPOINT3D();

	void SetTo( smVERTEX *pv );
	void xform2d();
	void GlobalRotate(int *trig);
	//   ���� �߽� ��ǥ �̵�
	void Move(int dx, int dy, int dz);
	//   ī�޶� ��ǥ
	void GlobalXform();

};


//TM ������ 
struct smFRAME_POS {
	int StartFrame;
	int EndFrame;
	int PosNum;
	int PosCnt;
};

struct smDFILE_HEADER {
	char szHeader[24];
	int ObjCounter;
	int MatCounter;
	int MatFilePoint;
	int First_ObjInfoPoint;
	int TmFrameCounter;
	smFRAME_POS TmFrame[OBJ_FRAME_SEARCH_MAX];		//�������� ���������� ���� ������
};

struct smDFILE_OBJINFO {
	char szNodeName[32];
	int Length;
	int ObjFilePoint;
};

//######################################################################################
//�� �� �� : �� �� ��
struct SMotionStEndInfo
{
	DWORD	StartFrame;
	DWORD	EndFrame;
};
//######################################################################################

class smOBJ3D {
	DWORD		Head;
public:

	smVERTEX	*Vertex;				// ���ؽ�
	smFACE		*Face;					// ���̽�
	smTEXLINK	*TexLink;				//�ؽ��� ��ǥ ����Ʈ

	smOBJ3D		**Physique;				// �� ���ؽ��� ����ũ ������Ʈ

	smVERTEX	ZeroVertex;				// ������Ʈ �߾� ���ؽ� ��

	int maxZ,minZ;
	int maxY,minY;
	int maxX,minX;

	int dBound;							// �ٿ�� ����� �� ^2
	int Bound;							// �ٿ�� ����� ��

	int MaxVertex;
	int MaxFace;

	int nVertex;
	int nFace;

	int nTexLink;

	int ColorEffect;					// ����ȿ�� ��� ����
	DWORD ClipStates;					// Ŭ���� ����ũ ( �� Ŭ���κ� ��� ���� ) 

	POINT3D Posi;
	POINT3D CameraPosi;
	POINT3D Angle;
	int	Trig[8];

	// �ִϸ��̼� ����
	char	NodeName[32];		// ������Ʈ�� ��� �̸�
	char	NodeParent[32];		// �θ� ������Ʈ�� �̸�
	smOBJ3D	*pParent;			// �θ� ������Ʈ ������

	smMATRIX	Tm;				// �⺻ TM ���
	smMATRIX	TmInvert;		// Tm �� ����� 
	//smMATRIX	TmResult;		// �ִϸ��̼� ���
	smFMATRIX	TmResult;		// �ִϸ��̼� ���
	smMATRIX	TmRotate;		// �⺻�� ȸ�� ��� 

	smMATRIX	mWorld;			// ������ǥ ��ȯ ���
	smMATRIX	mLocal;			// ��Į��ǥ ��ȯ ���

	int		lFrame;				// ���� ������

	float	qx,qy,qz,qw;		// ȸ�� ���ʹϾ�
	int		sx,sy,sz;			// ������ ��ǥ
	int		px,py,pz;			// ������ ��ǥ

	smTM_ROT	*TmRot;			// �����Ӻ� ȸ�� �ִϸ��̼�
	smTM_POS	*TmPos;			// �����Ӻ� ������ �ִϸ��̼�
	smTM_SCALE	*TmScale;		// �����Ӻ� ������ �ִϸ��̼�

	//######################################################################################
	//�� �� �� : �� �� ��
	smFMATRIX	*TmPrevRot;		// �����Ӻ� �ִϸ��̼� ���
	//######################################################################################

	int TmRotCnt;
	int TmPosCnt;
	int TmScaleCnt;

	//TM ������ ��ġ ( �������� ������ ã�Ⱑ ���� )
	smFRAME_POS TmRotFrame[OBJ_FRAME_SEARCH_MAX];		//ROT������ [������ , ��ġ]
	smFRAME_POS TmPosFrame[OBJ_FRAME_SEARCH_MAX];		//POS������ [������ , ��ġ]
	smFRAME_POS TmScaleFrame[OBJ_FRAME_SEARCH_MAX];		//SCALE������ [������ , ��ġ]
	int TmFrameCnt;									//TM������ ī���� (��ü����)

	smOBJ3D();
	~smOBJ3D();
	smOBJ3D( int nv , int nf );
	// �ʱ�ȭ��ƾ ( ���ؽ� , ���̽��� �� , �׸��� ����ũ ��뿩��  )
	void Init( int nv, int nf , int pFlag=0 );
	// ���ؽ� ��ǥ �߰�
	int AddVertex( int x, int y, int z );
	// ���̽��� ���ؽ� ��ȣ �߰�
	int AddFace( int a, int b, int c );

	//�ؽ��� ��ǥ�� ���¿� ���缭 �����Ѵ�
	int ReformTexture( int Type , int MatNum , int MatCnt , POINT3D *pos , POINT3D *size );

	//���̽��� ��Ʈ���� �Է�
	int SetFaceMaterial( int FaceNum , int MatNum  );
	//�ؽ��� ��ǥ������ �߰��Ѵ� 
	int AddTexLink( int FaceNum , DWORD *hTex , smFTPOINT *t1 , smFTPOINT *t2 , smFTPOINT *t3 );
	//�ؽ��� ��ǥ������ �߰��Ѵ� ( TPOINT ��� )
	int AddTexLinkTP( int FaceNum , DWORD *hTex , smTPOINT *t1 , smTPOINT *t2 , smTPOINT *t3 );

	// ���̽� �� �ؽ��� ���ÿ� �߰�
	int AddFace( int a, int b, int c , smTPOINT *t1 , smTPOINT *t2, smTPOINT *t3 );

	// �븻�� ���� ( 0���� �ʱ�ȭ )
	void ZeroNormals();
	// ���̵� �븻�� ����
	void SetGNormals();
	// ���̽��� ��Ʈ���󺰷� ����
	void SortMatrial();
	// ��ġ�ų� ������ �ʴ� VERTEX�� ã�Ƴ��� �����Ѵ�
	void SortVertex();
	// TM�� ������� ����Ͽ� ���� ��ǥ ��ȯ
	void ReformTM();
	// ����ũ�� ���� TM�� ������� ����Ͽ� ���� ��ǥ ��ȯ
	void ReformPhysique();

	// ������Ʈ ������ �غ�
	int	 BeginRender();
	// �̰� �ٺ���.
	int  TestRotate( int x , int y , int z ,  POINT3D *LocalAngle , POINT3D *GlobalAngle );
	// ������Ŀ����� ������ǥ ���ϱ�  
	int  WorldForm();

	//######################################################################################
	//�� �� �� : �� �� ��
	//ȸ�� ���ʹϾ��� ���� , �߰� �������� �࿭�� ���� 
	int GetRotFrame( smFMATRIX &gmat, smFMATRIX *PrevRot, smTM_ROT *tmRot, int frame, SMotionStEndInfo *lpFrameInfo=NULL );

	//�̵� ��ǥ�� ���� , �߰� �������� �̵� ��ǥ�� ���� 
	int GetPosFrame( float &x, float &y, float &z, smTM_POS *tmPos, int frame );
	//######################################################################################

	//������ ��ǥ�� ���� , �߰� �������� ������ ��ǥ�� ���� 
	int GetScaleFrame( int &x, int &y, int &z , smTM_SCALE *tmSclae , int frame );

	//�ִϸ��̼� ������  ��ġ�� ã�´� ( ���� ���� �ִϸ��̼� ��� )
	int GetTmFramePos( int frame );
	//�ִϸ��̼� ������ ȸ�� ��ġ�� ã�´� ( ���� ���� �ִϸ��̼� ��� )
	int GetTmFrameRot( int frame );
	//�ִϸ��̼� ������ ũ�⸦ ã�´� ( ���� ���� �ִϸ��̼� ��� )
	int GetTmFrameScale( int frame );

	//######################################################################################
	//�� �� �� : �� �� ��	
	int TmTalkAnimation( int frame );
	int TmAnimation( int frame, int ax, int ay, int az, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo=NULL );
	
	// �ִϸ��̼� �� ������ǥ ȸ�� ��� �����
	int  TmAnimation( int frame, int ax , int ay , int az, SMotionStEndInfo *lpFrameInfo=NULL );
	//######################################################################################

	// ���� ����
	//void SetColor( int flag , int alpha );

	// ������Ʈ�� ��ǥ �� ���� ����
	void SetPosi( POINT3D *point , POINT3D *angle );
	// D3D������� ������Ƽ�꿡 ������ ( ī�޶� ��ǥ�� ī�޶� ���� �Է� )
	int  RenderD3D( POINT3D *camera , POINT3D *angle );

	//######################################################################################
	//�� �� �� : �� �� ��
	int	 RenderD3dOpening( POINT3D *camera, POINT3D *angle );
	//######################################################################################

	// ������ ������ �׽�Ʈ (ī�޶� ��ǥ�� ī�޶� ���� �Է� )
	int TestRender(POINT3D *camera , POINT3D *angle );
	//�׽�Ʈ �Ϸ������ �������� �簳 �Ѵ�
	int ActionRenderD3D();


	//���Ϸ� ������ ũ�⸦ �̸� ��´�
	int  GetSaveSize();
	//���Ϸ� ����
	int  SaveFile( HANDLE hFile );
	//���Ϸ� ���� ����Ÿ �ε�
	int  LoadFile( HANDLE hFile , smPAT3D *PatPhysique=0 );
};

class smPAT3D {
	DWORD		Head;
public:
	smOBJ3D		*obj3d[128];
	BYTE		TmSort[128];					// ���ϸ��̼� ��� ���� (��ũ ������)

	smPAT3D		*TmParent;

	smMATERIAL_GROUP	*smMaterialGroup;		//��Ʈ���� �׷�

	int MaxFrame;
	int Frame;

	int SizeWidth , SizeHeight;					// ���� ���� �� �ִ�ġ 

	int nObj3d;
	DRZTEXTURE2 *hD3DTexture;

	POINT3D Posi;
	POINT3D Angle;
	POINT3D CameraPosi;

	int dBound;							// �ٿ�� ����� �� ^2
	int Bound;							// �ٿ�� ����� ��

	smFRAME_POS TmFrame[OBJ_FRAME_SEARCH_MAX];		//�������� ���������� ���� ������
	int TmFrameCnt;

	int		TmLastFrame;
	POINT3D TmLastAngle;

	smPAT3D();
	~smPAT3D();

	//�ʱ�ȭ
	void	Init();
	//�� Ŭ���� �ݱ�
	void	Close();

	// ������Ʈ �߰�
	int		AddObject( smOBJ3D *obj );
	// ������Ʈ �� �θ� �ڽ� ���踦 ����Ͽ� ���� 
	int		LinkObject();
	//�θ� ���ϱ� ��� ������Ʈ�� �θ��� Ư�� ������Ʈ�� ���� ��Ų��
	int		LinkParentObject( smPAT3D *patParent , smOBJ3D *objParent=0 );
	// �� ������Ʈ�� ����ũ�� ������ǥ ��ȯ
	int		ReformPhysique();
	// �� ������Ʈ�� ������ǥ ��ȯ
	int		ReformTM();

	// ���������� ������Ʈ ���ϱ�
	smOBJ3D *GetObjectFromName( char *name );
	
	//######################################################################################
	//�� �� �� : �� �� ��
	int		SetFrame( int frame, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo=NULL );
	int		TmTalkAnimation( int frame );
	int		TmAnimation( int frame , int ax, int ay, int az, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo=NULL );

	// �����ӿ� ���� �ִϸ��̼� �� ���ϱ� 
	int		SetFrame( int frame, SMotionStEndInfo *lpFrameInfo=NULL );
	// ������Ʈ�� �ִϸ��̼� �� ȸ�� �� ������� ����
	int		TmAnimation( int frame , int ax, int ay, int az, SMotionStEndInfo *lpFrameInfo=NULL );
	//######################################################################################

	// �� ������Ʈ�� �븻�� ����( 0�Ƿ� �ʱ�ȭ )
	void	ZeroNormals();
	// ������ ��ǥ �� ���� ����
	void	SetPosi( POINT3D *point , POINT3D *angle );
	// ������ ��ǥ�� ����ǥ�� ����
	void	SetFixPosi();

	//######################################################################################
	//�� �� �� : �� �� ��
	int		RenderD3D( POINT3D *camera, POINT3D *angle, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo=NULL );
	int		ChoiseRenderD3D( POINT3D *camera, POINT3D *angle, char **szNodeList, int ListCnt, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo=NULL );

	// D3D������� ������Ƽ�꿡 ������ ( ī�޶� ��ǥ�� ī�޶� ���� �Է� )
	int		RenderD3D( POINT3D *camera , POINT3D *angle, SMotionStEndInfo *lpFrameInfo=NULL );
	// D3D������� ������Ƽ�꿡 ������ ������ ( ī�޶� ��ǥ�� ī�޶� ���� �Է� )
	int		ChoiseRenderD3D( POINT3D *camera , POINT3D *angle , char **szNodeList , int ListCnt, SMotionStEndInfo *lpFrameInfo=NULL );
	//######################################################################################

	//######################################################################################
	//�� �� �� : �� �� ��
	int		RenderD3dOpening( POINT3D *camera, POINT3D *angle );
	//######################################################################################

	//����Ÿ�� ���Ϸ� ����
	int		SaveFile( char *szFile );
	//����Ÿ�� ���Ͽ��� �ε�
	int		LoadFile( char *szFile , char *szNodeName=0 );

};

extern	DWORD		smObjFixPos;			//������ ��ǥ ����

int smGetMapHeight( int x, int z );
int smAddVertexLight( smRGB *LightRGB , smLIGHT3D *light , POINT3D *posi , POINT3D *normal );
int smAddVertexLightSpot( smRGB *LightRGB , smLIGHT3D *light , POINT3D *posi , POINT3D *normal );

//���� ���� ������ ã�� �ùٸ��� Ȯ�� (���� Ȯ��) 
int smFindPatFile( char *szfile , char *FileExt );


