#ifndef _SMREND3D_H_
#define _SMREND3D_H_

//D3D�� TLVERTEX ��Ƽ �ؽ��� ����
struct D3DTLVERTEX8 {
    float sx, sy, sz;
	float rhw;
	D3DCOLOR color;
	D3DCOLOR specular;
	float tu1, tv1;
	float tu2, tv2;
	float tu3, tv3;
	float tu4, tv4;
	float tu5, tv5;
	float tu6, tv6;
	float tu7, tv7;
	float tu8, tv8;
};

struct DRZD3DTLVERTEX{
	float sx;
	float sy;
	float sz;
	float rhw;
	D3DCOLOR color;
	D3DCOLOR specular;
	float tu;
	float tv;
};

#define D3DFVF_TLVERTEX0		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX0
#define D3DFVF_TLVERTEX1		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX1
#define D3DFVF_TLVERTEX2		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX2
#define D3DFVF_TLVERTEX3		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX3
#define D3DFVF_TLVERTEX4		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX4
#define D3DFVF_TLVERTEX5		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX5
#define D3DFVF_TLVERTEX6		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX6
#define D3DFVF_TLVERTEX7		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX7
#define D3DFVF_TLVERTEX8		D3DFVF_XYZRHW |D3DFVF_DIFFUSE |D3DFVF_SPECULAR |D3DFVF_TEX8



// �ѹ��� ������ �ϴ� ���̽��� ���ؽ��� �ִ�ġ
#define RENDER_FACE_MAX		20000
#define RENDER_VERTEX_MAX	20000

#define TEMPTEXLINK_MAX		20000
#define REND_LIGHT_MAX		64

//�⺻ Ŭ���� ���� ��
#define RENDCLIP_DEFAULT_SHIFT_PERSPECTIVE_WIDTH		1
#define RENDCLIP_WIDE_MULT_PERSPECTIVE_HEIGHT			(int)( ((float)3.36) * fONE )
#define RENDCLIP_DEFAULT_MULT_PERSPECTIVE_HEIGHT		(int)( ((float)2.7) * fONE )


//######################################################################################
//�� �� �� : �� �� ��
#define smRENDER_TYPE_ICE			0x01
//######################################################################################


class smRENDER3D {

public:
	//######################################################################################
	//�� �� �� : �� �� ��	
	DWORD		m_dwRenderType;
	//######################################################################################

	//######################################################################################
	// �� �� �� : �� �� ��
	int			m_FogIsRend;
	int			m_FogMode;
	float		m_fFogStNum;
	float		m_fFogEndNum;
	DWORD		m_dwFogColor;
	//######################################################################################

	//######################################################################################
	//�� �� �� : �� �� ��
	int				m_GameFieldView;
	int				m_GameFieldViewStep;
	
	smRENDVERTEX   *RendVertex;								//������ �� ���ؽ� 
	smRENDFACE	   *RendFace;								//������ �� ���̽�
	//######################################################################################

	smTEXLINK		TempTexLink[ TEMPTEXLINK_MAX ];		//�ӽ� �ؽ��� ��ũ
	smRENDMATRIAL	RendMatrial[ MATRIAL_MAX ];			//�������� ���̽��� ��Ʈ���� ���̺�

	DWORD			RendMatrialList[ MATRIAL_MAX ];		//���� �������� �ʿ��� ��Ʈ���� ���̺��� ������ ���

	smMATERIAL		*smRendMaterial;		//������ �Ҹ�Ʈ���� ������
	int				nMaterial;				//������ �� ��ü ��Ʈ���� ����

	DWORD			dwMatDispMask;			//��Ʈ���� �Ӽ� ǥ�� ����ũ
	DWORD			DeviceRendMode;			//������ ���� ������ ��� (����ī��)

	int RMatListCnt;						//������ ��Ʈ���� ����Ʈ�� ī���� ������
	int RMatListRearCnt;					//������ ��Ʈ���� ����Ʈ ī���� ������

	int nRendVertex;						//������ �� ���ؽ� ��
	int nRendFace;							//������ �� ���̽� ��
	int nTempTexLink;						//�ӽ� �ؽ�����ǥ ���� ��

	int	SMSHIFT_PERSPECTIVE_WIDTH;			//�¿� Ŭ���� ������ ( 1 -> ��ü 2-> ���� ) 
	int	SMMULT_PERSPECTIVE_HEIGHT;			//���� Ŭ���� ������ ( (float)3.36*fONE ��ü (float)2.7)*fONE ���̵� )

	//////////// STAGE3D�� ������������ �ʿ��� ��� /////////////
	smSTAGE_VERTEX	*Vertex;				//���ؽ� ����Ʈ
	smSTAGE_FACE	*Face;					//���̽� ����Ʈ
	smTEXLINK		*TexLink;				//�ؽ��� ��ǥ ����Ʈ
	/////////////////////////////////////////////////////////////

	//######################################################################################
	//�� �� �� : �� �� ��
	smRENDLIGHT		smRendObjLight[REND_LIGHT_MAX];
	int				nRendObjLight;

	int				bApplyRendObjLight;
	//######################################################################################

	smRENDLIGHT		smRendLight[REND_LIGHT_MAX];	//�������� ��
	int	nRendLight;									//�������� ���� ��


	DWORD SumCount;							// ������ ýũ ī���� 

	int		CameraPosiMask;					// ī�޶� ��ġ ����ũ ( TRUE �� ī�޶� �Է� �Ұ� )

	smMATRIX	mCamera;					// ī�޶� ��ǥ ��ȯ ���

	int		CameraPosiX , CameraPosiY, CameraPosiZ;	//ī�޶� ��ǥ
	int		CameraAngX , CameraAngY , CameraAngZ;	//ī�޶� ����

	int		TexPosiX,TexPosiY,TexPosiZ;				//�ؽ��� ��ǥ ������ �߽� ��ġ
	int		TexSizeX,TexSizeY,TexSizeZ;				//�ؽ��� ���� ũ��

	DWORD	ClipStates;						// Ŭ���ΰ� ( ���� Ŭ������ ���� ������ ��Ʈ�� ���� )
	DWORD	RendStatTime;

	int		Color_R, Color_G , Color_B , Color_A ;		// �������� ���� ��

	int		TextureFormState[8];						//�ؽ��� ��ǥ ���� �Ӽ�

	DWORD	AlphaTestDepth;								//���� �׽�Ʈ ��

	int RendAlphaFlag;

	int	ObjectVertexScale;								//������Ʈ�� ���ؽ� ������ ��ȯ
	int	ZWriteAuto;										//Z���� ���� �ڵ� ����

	int	DistortionMesh;									//�޽� �ְ�

	
	//######################################################################################
	// �� �� �� : �� �� ��
	int   isRenderOutLine;
	float fOutLineRatio;
	float fOutLineX;
	float fOutLineY; 
	DWORD OutLineColor;
	//######################################################################################

	smRENDER3D();
	~smRENDER3D();

	//######################################################################################
	//�� �� �� : �� �� ��
	void CreateRenderBuff( int FieldViewType );
	void SetGameFieldViewStep(void);
	//######################################################################################

	// �������� ī�޶� ��ġ�� �Է� 
	int SetCameraPosi ( int x, int y, int z, int angX, int angY, int angZ );
	// �������� ī�޶� ��ġ�� �Է� ( ȸ���� ��ķ� ���� )
	int SetCameraPosi ( int x, int y, int z, smEMATRIX *lpeMatrix );

	// �������� ī�޶� ��ġ�� �Է� ( ȸ���� ��ķ� ���� ) - Close���� ��ȿ
	int OpenCameraPosi ( int x, int y, int z, smEMATRIX *lpeMatrix );
	// ī�޶� ��ġ ����
	int CloseCameraPosi();

	//######################################################################################
	//�� �� �� : �� �� ��
	// �������� ���� �߰� �Ѵ�
	int	AddDynamicLight( int type, int x, int y, int z, int r, int g, int b, int a, int Range );

	// ���� ��� ���� �Ѵ�
	int ClearObjLight();
	//######################################################################################

	// �������� ���� �߰� �Ѵ�
	int	AddDynamicLight( int x, int y, int z, int r, int g, int b, int a, int Range );
	// ���� ��� ���� �Ѵ�
	int ClearLight();

	// ��ǥ�� 2D �������� ��ȯ�Ͽ� �����ش�  
	int GetRect2D ( int x, int y, int z, int width, int height, RECT *Rect , POINT *Pos=0 );
	// ī�޶� Z��ǥ�� ������  
	int GetDepthZ ( int x, int y, int z );

	// ���޽� ���� �ʱ�ȭ ( �������� �Ӽ� ���� )
	int InitStageMesh ( smSTAGE_VERTEX *StageVertex , DWORD sum );
	// ���޽� ���� �ʱ�ȭ 
	int Init();
	// �ؽ��� �߽� ��ǥ ����
	int SetTexPosi( int x, int y, int z , int sx, int sy, int sz );
	// Ŭ���ΰ� ����
	int SetClipStates( DWORD States );
	// ��Ʈ���� �׷� ����
	int SetMaterialGroup( smMATERIAL_GROUP *MatGroup );
	//�ؽ��� ��ǥ������ �߰��Ѵ� 
	int AddRendTempTexLink( smRENDFACE *lpRendFace , DWORD *hTex , 
							  float u0 , float v0 , float u1 , float v1 , float u2 , float v2 );

	//�������� ���ؽ��� ������ ���ؽ� �߰� ( ���� ��ǥ �ҽ� ��� )
	smRENDVERTEX *AddStageVertex ( smSTAGE_VERTEX *sv , smMATERIAL *lpMaterial  );
	//�������� ���̽��� ������ ���̽� �߰�
	smRENDFACE *AddStageFace ( smSTAGE_FACE *face );


	//������Ʈ ���ؽ��� ������ ���ؽ� �߰� ( ���� ��ǥ �ҽ� ��� )
	//######################################################################################
	//�� �� �� : �� �� ��
	//smRENDVERTEX *AddObjectVertex ( smVERTEX *sv );
	smRENDVERTEX *AddObjectVertex( smVERTEX *sv, DWORD dwObjHead=0 );
	//######################################################################################

	//������Ʈ ���̽��� ������ ���̽� �߰�
	smRENDFACE *AddObjectFace ( smFACE *face );

	//2D �̹��� ������Ʈ �߰�
	smRENDFACE *AddFace2D ( smFACE2D *face );
	//2D �̹��� ������Ʈ �߰� ( ȸ�� ���� )
	smRENDFACE *AddFace2D ( smFACE2D *face , int angle );

	//2D �̹��� ������Ʈ �߰� ( ȭ����ǥ�� ���� �׸��� )
	smRENDFACE *AddFace2D_Direct ( smFACE2D *face );

	//######################################################################################
	//�� �� �� : �� �� ��
	//������ ���ؽ� �߰� ( ��ǥ ���� �Է� )
	//smRENDVERTEX *AddRendVertex ( int x, int y, int z, short *pLight );
	smRENDVERTEX *AddRendVertex ( int x, int y, int z, short *pLight, int nx=0, int ny=0, int nz=0 );
	//######################################################################################

	//������ ���̽� �߰� ( ���ؽ� ������ ���� �Է� )
	smRENDFACE *AddRendFace ( smRENDVERTEX *v0 , smRENDVERTEX *v1, smRENDVERTEX *v2 , 
										DWORD matrial , smTEXLINK *lpTexLink );
	//���̽� ���̵尪 ���� ( DotProduct )
	int GetFaceSide( smRENDFACE *rendface );
	// ������ ���̽��� Ŭ���� ( dwClipMode�� SMCLIP_[����] ) 
	int AddRendFaceClip( smRENDFACE *rendface , DWORD dwClipMode );
	// ������ ���̽��� �ؽ��� Ŭ���� 
	int AddRendFaceTextureClip( smRENDFACE *rendface );

	//��Ʈ���� ����Ʈ�� ��� �߰�
	int AddRendMaterialList( int MatNum );

	//######################################################################################
	//�� �� �� : �� �� ��
	int SetDynamicObjLight( smRENDVERTEX *sv, int z );
	//######################################################################################
	
	//���̳��� ����Ʈ ����
	int SetDynamicLight( smRENDVERTEX *sv , int z );
	// Z ���� ���� ����ġ�� BackColor���� �������� �����Ѵ� 
	int SetColorZclip( smRENDVERTEX *sv , int z );
	// ���ؽ��� ������ �־��� ���� �������� �ܰ������� ����
	int ShiftLight( smRENDVERTEX *sv , int lev , int r,int g,int b, int a );
	// �������ؽ��� ������ ���Ѵ�
	int AddLight( smRENDVERTEX *sv, int r,int g,int b , int a);
	//�������ؽ��� sLight�� Color �� Specular �� ���� �Ѵ�
	int SetRendColor( smRENDVERTEX *sv );
	// ��ü ������ ���̽��� Ŭ����  
	int ClipRendFace();
	// ���ؽ��� 2D��ǥ�� ��ȯ  
	int GeomVertex2D( int ColorEffect=TRUE );
	// ������ Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
	smRENDVERTEX * AddRendVertex_ClipLeft( smRENDFACE *rendface , int a, int b , float *fMultPoint );
	// �������� Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
	smRENDVERTEX * AddRendVertex_ClipRight( smRENDFACE *rendface , int a, int b , float *fMultPoint );
	// ���� Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
	smRENDVERTEX * AddRendVertex_ClipTop( smRENDFACE *rendface , int a, int b , float *fMultPoint );
	// �Ʒ��� Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
	smRENDVERTEX * AddRendVertex_ClipBottom( smRENDFACE *rendface , int a, int b , float *fMultPoint );
	// Z NEAR Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
	smRENDVERTEX * AddRendVertex_ClipNearZ( smRENDFACE *rendface , int a, int b , float *fMultPoint );
	// Z FAR Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
	smRENDVERTEX * AddRendVertex_ClipFarZ( smRENDFACE *rendface , int a, int b , float *fMultPoint );
	// TEXTURE Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
	smRENDVERTEX * AddRendVertex_ClipTexture( smRENDFACE *rendface , int a, int b , float *fMultPoint );

	int SetD3DRendBuff(float *RenderBuff, int matrial, int TexClip);
	int SetD3DRendBuff2(float *RenderBuff, int matrial, int TexClip);

	//######################################################################################
	// �� �� �� : �� �� ��
	int SetD3DRendBuff3(float *RenderBuff, int matrial, int TexClip, short sColors[4]);
	//######################################################################################

	//######################################################################################
	// �� �� �� : �� �� ��
	int SetItem2PassD3DRendState( smMATERIAL *lpMaterial );
	//######################################################################################

	// D3D������ �Ӽ� ���� 
	int SetD3DRendState( smMATERIAL *lpMaterial , int TexClip );
	// D3D������ �Ӽ� ���� ���ĸ� ����
	int SetD3DRendStateOnlyAlpha( smMATERIAL *lpMaterial , int TexClip );

	//��Ʈ���� ����� D3D�� ������
	int RenderD3D( int MatNum );
	// D3D�� ������
	int RenderD3D();

	//######################################################################################
	// �� �� �� : �� �� ��
	void SetD3D_OutLine( int isSet, int Dist=0, float fsetX=0, float fsetY=0, DWORD Color=0 );
	int  RenderD3D_OutLine(void);
	int	 RenderD3D_OutLine( int MatNum );
	//######################################################################################

	//2d ��ǥ�� ���Ե� �������� ���̽��� ã�´�
	int FindRenderFace2D( smRENDFACE **lpRendFace , int x, int y );

};

//������ Ŭ���� 
extern smRENDER3D	smRender;

//�������� ����Ʈ ����
void smSetRendLightVect( int x, int y, int z );


//######################################################################################
//�� �� �� : �� �� ��
extern int   g_IsDxProjectZoomIn;
extern float g_fZoomInAdd_x;
extern float g_fZoomInAdd_y;
extern float g_fZoomInDiv_x;
extern float g_fZoomInDiv_y;

extern void SetDxProjection( float radians, int SizeX, int SizeY, float near_z, float far_z );
extern void DxProjectionWork( smRENDVERTEX *lpRendVer );
//######################################################################################


//######################################################################################
//�� �� �� : �� �� ��
extern int  CreateItem2PassTexture(void);
extern void SetItem2PassTexture( int TexHandleNum, int TexFormState = smTEXSTATE_FS_SCROLL10 );
//######################################################################################

#endif