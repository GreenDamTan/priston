#ifndef _SMSTAGE3D_H_
#define _SMSTAGE3D_H_

#define ADD_TEMPFACE	2048
#define ADD_TEMPVERTEX	2048
#define MAP_SIZE		256

//�ɾ� �ٴҼ� �ִ� �ִ� ������
extern int Stage_StepHeight;
extern int smStage_WaterChk;

class smSTAGE3D
{
public:
	DWORD	Head;

	//DRZ_EDIT (change to DWORD new MapStage usage)
	//WORD *StageArea[MAP_SIZE][MAP_SIZE];	//������ ���̽� �� ���� ����Ʈ
	DWORD *StageArea[MAP_SIZE][MAP_SIZE];	//������ ���̽� �� ���� ����Ʈ

	POINT *AreaList;						//������ ���̽� ����Ʈ �ӽ����� 
	int AreaListCnt;						//���� ���̽��� Ŀ����

	int	MemMode;							//�޸� ���� ��� ( ���� )

	DWORD SumCount;							// ������ ýũ ī���� 
	int CalcSumCount;						// ���� ��ȣ ī����

	smSTAGE_VERTEX		*Vertex;			//���ؽ� ����Ʈ
	smSTAGE_FACE		*Face;				//���̽� ����Ʈ
	smTEXLINK			*TexLink;			//�ؽ��� ��ǥ ����Ʈ
	smLIGHT3D			*smLight;			//���� ����

	smMATERIAL_GROUP	*smMaterialGroup;	//��Ʈ���� �׷�

	smSTAGE_OBJECT		*StageObject;		//���� �������� ������Ʈ��
	smMATERIAL			*smMaterial;		//��Ʈ���� ����Ʈ ������

	int nVertex;							//���ؽ� ��
	int nFace;								//���̽� ��
	int nTexLink;							//�ؽ�����ǥ ���� ��
	int	nLight;								//���� ��

	int	nVertColor;								//���ؽ� �� �Է� ī����

	int	Contrast;								//ä�� ( ���� )
	int Bright;									//�� ( ��� ���� )

	POINT3D VectLight;							//���̵�� ����Ʈ ����

	//DRZ_EDIT (change to DWORD new MapStage usage)
	//WORD	*lpwAreaBuff;						//������ ���� ����
	DWORD	*lpwAreaBuff;						//������ ���� ����

	int		wAreaSize;							//������ ���� ���� ũ��
	RECT	StageMapRect;						//��ü STAGE�� �����ϴ� ���� ��ġ

	smSTAGE3D();
	~smSTAGE3D();
	smSTAGE3D( int nv , int nf );

	//######################################################################################
	//�� �� �� : �� �� ��
	void Init(void);
	//######################################################################################

	int Init( int nv , int nf );
	int Close();
	//���ؽ� ��ǥ �߰�
	int AddVertex ( int x, int y, int z );
	//���̽� �߰�
	int AddFace ( int a, int b, int c , int matrial=0 );
	//���̽��� ��Ʈ���� �Է�
	int SetFaceMaterial( int FaceNum , int MatNum  );

	//���ؽ��� �� ����
	int SetVertexColor ( DWORD NumVertex , BYTE r , BYTE g, BYTE b , BYTE a=255 );
	//�ؽ��� ��ǥ������ �߰��Ѵ� 
	int AddTexLink( int FaceNum , DWORD *hTex , 
							  smFTPOINT *t1 , smFTPOINT *t2 , smFTPOINT *t3 );
	
	//�븻 ���� ���Ͽ� ���ؽ��� ���̵� ����� �߰��Ѵ�
	//######################################################################################
	//�� �� �� : �� �� ��
	int SetVertexShade( int isSetLight = TRUE );
	//######################################################################################

	//��� ���ؽ��� ������ �߰� �Ѵ�
	int AddVertexLightRound( POINT3D *LightPos , int r, int g, int b, int Range );

	//���� ���� �ʱ�ȭ ( ���� ���� )
	int	InitDynLight( int nl );
	//��� ���ؽ��� ������ �߰� �Ѵ�
	int AddDynLight( int type , POINT3D *LightPos , int r, int g, int b, int Range );

	//######################################################################################
	//�� �� �� : �� �� ��
	int CheckFaceIceFoot( POINT3D *Pos, POINT3D *Angle, int CheckHeight );
	//######################################################################################

	//�ӽ� ���̽� ����� �ۼ��Ѵ�
	int MakeAreaFaceList( int x,int z, int width, int height , int top , int bottom );

	//���̽��� �ش��ϴ� ���̸� ���Ѵ�
	int GetPolyHeight( smSTAGE_FACE *face , int x, int z );
	//������ �ش��ϴ� ���̽��� �˻��Ͽ� ���̸� ���Ѵ�
	int GetAreaHeight( int ax, int az , int x, int z );
	//������ �ش��ϴ� ���̽��� �˻��Ͽ� ���̸� ���Ѵ� ( ������ ���� ��ȿ ó�� )
	int GetAreaHeight2( int ax, int az , int x, int z );

	//���̽��� �ش��ϴ� ���̸� ���Ѵ�
	int GetHeight( int x, int z  );
	//���̽��� �ش��ϴ� ���̸� ���Ѵ�
	int GetHeight2( int x, int z );

	//�̵��� ��ġ�� ��ֹ��� �˻��Ѵ�
	int CheckSolid( int sx, int sy, int sz , int dx, int dy, int dz );

	int GetThroughPlane( smSTAGE_FACE *face , POINT3D *sp , POINT3D *ep );

	//���� 1���� ��ġ�� ���� ���� ( ��� ������ )
	int GetPlaneProduct( smSTAGE_FACE *face , POINT3D *p );
	//�� ������ ���̽��� �����ϴ��� �˻��Ѵ� ( ��� ������ �������� ���� ��� TRUE �׿��� ��� FALSE )
	int GetTriangleImact( smSTAGE_FACE *face, smLINE3D *pLines , int LineCnt );
	//�̵��� ����� ��ġ�� �˻��Ͽ� ��ǥ�� �����ش�
	int CheckNextMove( POINT3D *Posi, POINT3D *Angle , POINT3D *MovePosi, int dist , int ObjWidth , int ObjHeight , int CheckOverLap=0 );

	//������ ������ �Ÿ��� �մ� ���п� �������� �浹�ϴ��� Ȯ�� 
	int CheckVecImpact( POINT3D *Posi, POINT3D *Angle , int dist );
	//������ ������ �Ÿ��� �մ� ���п� �������� �浹�ϴ��� Ȯ�� 
	int CheckVecImpact2( int sx,int sy,int sz, int ex,int ey,int ez );

	//���� ��ġ������ �ٴ� ���̸� ���Ѵ�
	int GetFloorHeight( int x, int y, int z , int ObjHeight );
	//���� ��ġ������ �ٴ� ���̸� ���Ѵ�
	int GetFloorHeight2( int x, int y, int z , int ObjHeight );
	//���� ���̿� ��ġ�� �������� �ִ� �� Ȯ���� ������ ��ü ���̸� ���Ѵ�
	int GetEmptyHeight( int x, int y, int z , int ObjHeight );

	//�ٴ� ������ �鿡 y==height �� ��ġ�ϴ��� Ȯ��
	int CheckFloorFaceHeight( int x, int y, int z , int hSize );


	// StageArea�� 0 ���� �ʱ�ȭ
	void clearStageArea();
	// �ﰢ���̽��� ��ġ�� ����(64x64) �� ���Ͽ� AreaList �� ���� 
	int getPolyAreas( POINT3D *ip1 , POINT3D *ip2, POINT3D *ip3 );
	//�� �������� ��ġ�� ���� ������ ����� �����Ѵ�
	int SetupPolyAreas();
	// �������� ���� ����� ����
	int RenderGeom();

	//ī�޶� ��ġ�� ���缭 �׸��� 
	int DrawStage(int x , int y, int z, int angX, int angY, int angZ , smEMATRIX *eRotMatrix=0 );
	//ī�޶� ��ġ�� ���缭 �׸��� ( ���Ե� ������Ʈ ���� )
	int DrawStage2(int x , int y, int z, int angX, int angY, int angZ , smEMATRIX *eRotMatrix=0 );

	//######################################################################################
	//�� �� �� : �� �� ��
	int DrawOpeningStage(int x, int y, int z, int angX, int angY, int angZ, int FrameStep );
	//######################################################################################

	//����Ÿ�� ���Ϸ� ����
	int	SaveFile( char *szFile );
	//����Ÿ�� ���Ϸ� ���� ����
	int	LoadFile( char *szFile );
};

#endif




