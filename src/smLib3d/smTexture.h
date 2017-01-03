/*
typedef struct _TEXTURE {
	DIRECTDRAWSURFACE lpDDSurface;  // memory texture
	D3DTEXTUREHANDLE    Handle;

	char    			szName[100];
	_TEXTURE			*lpNext;
} TEXTURE, *LPTEXTURE;
*/



#define SWAPTEXTURE_MAX		64


//######################################################################################
//�� �� �� : �� �� ��
extern int		g_IsReadTextures;

extern int		g_DibLastReadWidth;
extern int		g_DibLastReadHeight;
//######################################################################################

//######################################################################################
//�� �� �� : �� �� ��
#define		MIPMAP_FILELIST				5
#define		SQUARE_MODE					1
#define		NOSQUARE_MODE				2

extern int MipMapModeCreate;
extern int TextureSwapMode;
//######################################################################################


//�ؽ��ĸ� SWAP �ϱ� ���� ���� ����ü
struct smSWAPTEXTURE {

	int		UsedTime;				//���������� ���� �ð� ( ������ ī���� �� )
	int		SwapState;				//���� TRUE �̸� ���� �����

    DRZTEXTURE2 lpD3DTexture;
	DIRECTDRAWSURFACE lpDDSurface;

	smTEXTUREHANDLE *smTextureMaster;
};


class smTEXTURE {
public:

	//######################################################################################
	//�� �� �� : �� �� ��
	DWORD			TexSpeedSum[ MAX_TEXTURE ];
	//######################################################################################

	smTEXTUREHANDLE Texture[MAX_TEXTURE];
	DIRECTDRAWSURFACE *lpLastTextureSurface;
    DRZTEXTURE2 *lpLastTexture;

	CRITICAL_SECTION	cTextureSection;			//ũ��Ƽ�� ����

	//�ؽ��ĸ� �޸𸮿� SWAP 
	smSWAPTEXTURE		SwapTexture[SWAPTEXTURE_MAX];
	int					SwapCount;

	int LastTextureCnt;
	int TextureCnt;


	smTEXTURE();
	~smTEXTURE();

	void Init();
	int	 Remove();

	//######################################################################################
	//�� �� �� : �� �� ��
	//int Find( char *name );
	int Find( DWORD dwCode, char *Name );
	//######################################################################################

	smTEXTUREHANDLE *Add( char *name , char *nameA=0 );

	//DRZTEXTURE2 *AddName( char *name );
	//DRZTEXTURE2 *AddName( char *name , char *nameA );

	//�ؽ��ĸ� �����Ѵ� ( 1�ܰ�� �����Ͽ� ��Ʈ������� ��ũ�� ���� ��� ���� ���� )
	int Delete( smTEXTUREHANDLE *lpTex );
	//SWAP �ؽ��ĸ� ����
	int DeleteSwapTexture( int TexNum );
	//ū�ؽ��ĸ� SWAP �Ͽ� ����� 
	int SetLargeTexture( smTEXTUREHANDLE *TexHandle );

	//�ؽ��ĸ� �о� ���δ�
	int ReadTexture();

	//�Ҿ� ���� �ؽ��ĸ� �ٽ� �ҷ� ���δ�
	int Restore();

};

class smMATERIAL_GROUP {
	DWORD	Head;
public:
	smMATERIAL *smMaterial;
	DWORD MaterialCount;

	int ReformTexture;		//������ �ؽ��� ����

	int MaxMaterial;

	int LastSearchMaterial;
	char szLastSearchName[64];

	smMATERIAL_GROUP();
	smMATERIAL_GROUP( int MaterialMax );
	~smMATERIAL_GROUP();

	// ��Ʈ���� �ʱ�ȭ
	void Init( int MaterialMax );
	// ��Ʈ���� ����ȭ
	int Close();

	//������ ����Ÿ�� ũ�⸦ �̸� �˷���
	int GetSaveSize();
	//����Ÿ�� ���Ϸ� ���� 
	int SaveFile( HANDLE hFile );
	//���Ͽ��� ����Ÿ�� �о��
	int LoadFile( HANDLE hFile );

	//������ �ʴ� ��Ʈ���� ����
	int DeleteUnuseMaterial();
	//��Ʈ������ ���
	int AddMaterial( ASE_MATERIAL *aseMaterial , char *szScript=0 );
	//�ؽ��� ��Ʈ���� ����
	int CreateTextureMaterial( char *szBitmap , char *szOpacity , int TwoSide ,int Shade , DWORD TexSwap , DWORD BlendType );
	//��Ʈ���� �ؽ��ĸ� �߰��Ѵ�
	int	AddTexture( int MatNum , char *szBitmap , char *szOpacity=0 );

	//��Ʈ������ ������ �ؽ��ĸ� ã�� �����Ѵ�
	int ReSwapMaterial( smMATERIAL *lpMaterial );
	//��Ʈ������ �ؽ��� �����ΰ��� ������ �����Ѵ� ( �ε� �Ϸ��� ���� )
	int CheckMatreialTextureSwap();
	//�ؽ��� ���ſ� ������
	int IncTextureFrame();
	//�ִϸ��̼� ��Ʈ���� ����
	int CreateAnimationMaterial( char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode );
	//��Ʈ������ ���ϸ��̼� �����ϰ� ����
	int ChangeMaterialToAnimation( smMATERIAL *lpMaterial , char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode );
	//��Ʈ������ ���ϸ��̼� �������� ����
	int SetMaterialAnimFrame( smMATERIAL *lpMaterial , int frame );
	//�ؽ��� �̸����� ��Ʈ������ ã�´�
	smMATERIAL *FindMaterialFromTextureName( char *szName );
	//�ؽ��� �̸����� ���� ��Ʈ������ ã�´�
	smMATERIAL *FindNextMaterialFromTextureName();

	//######################################################################################
	//�� �� �� : �� �� ��
	//�ش� ��Ʈ������ ��� �ؽ��ĸ� �ε�
	int ReadTextures( int MipMap = 0 );
	//######################################################################################

};


//��Ʈ������ �ִ�ġ
#define MATRIAL_MAX			5000

//extern smMATERIAL smMaterial[ MATRIAL_MAX ];
extern smMATERIAL *smMaterial;
extern smMATERIAL_GROUP	*smMaterialGroup;

extern smTEXTURE smTexture;


//���� ��� �޸� ����ġ
extern DWORD	VramTotal;


// ��Ʈ���� �ʱ�ȭ
int InitMaterial();
//��Ʈ���� ����
int CloseMaterial();

//��Ʈ������ ���
int AddMaterial( ASE_MATERIAL *aseMaterial );
//������ �ʴ� ��Ʈ���� ����
int DeleteUnuseMaterial();
//�ؽ��� ��Ʈ���� ����
int CreateTextureMaterial( char *szBitmap , char *szOpacity , int TwoSide ,int Shade , DWORD TexSwap , DWORD BlendType );
//��Ʈ���� �ؽ��ĸ� �߰��Ѵ�
int	AddTexture( int MatNum , char *szBitmap , char *szOpacity=0 );
//�ִϸ��̼� ��Ʈ���� ����
int CreateAnimationMaterial( char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode=TRUE );
//��Ʈ������ ���ϸ��̼� �����ϰ� ����
int ChangeMaterialToAnimation( smMATERIAL *lpMaterial , char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode=TRUE );
//��Ʈ������ ���ϸ��̼� �������� ����
int SetMaterialAnimFrame( smMATERIAL *lpMaterial , int frame );

//��Ʈ������ �ؽ��� �����ΰ��� ������ �����Ѵ� ( �ε� �Ϸ��� ���� )
int CheckMatreialTextureSwap();
//��Ʈ������ ������ �ؽ��ĸ� ã�� �����Ѵ�
int ReSwapMaterial( smMATERIAL *lpMaterial );
//��Ʈ���� �˻�� Ÿ�� ������
extern int MaterialFrame;
//�ؽ��� ���ſ� ������
int IncTextureFrame();

//�ؽ��� �޸� ���� ��� ����
int SetTextureSwapMode( int mode );

//�ؽ����� ���� ���� ( 0-��ȭ�� 1-���� 2-��ȭ�� 3-����ȭ�� )
int smSetTextureLevel( int Level );


//�ؽ��� �̸����� ��Ʈ������ ã�´�
smMATERIAL *FindMaterialFromTextureName( char *szName );
//�ؽ��� �̸����� ���� ��Ʈ������ ã�´�
smMATERIAL *FindNextMaterialFromTextureName();






//�ؽ��� �ʱ�ȭ
int InitTexture();
//��� �ؽ��ĸ� ����
int CloseTexture();

//��� �ؽ��ĸ� �о� ���δ�
int ReadTextures();
//�ؽ��ĸ� ýũ�Ͽ� �Ҿ���� �ؽ��ĸ� �ٽ� �����
int RestoreTextures();

void ReleaseTextureAll();
int PopReleaseTexture();

//�ؽ��� ���� ũ��ƼĮ ���� ����
void smLeaveTextureCriticalSection();
//�ؽ��� ���� ũ��ƼĮ ���� ����
void smEnterTextureCriticalSection();


DIRECTDRAWSURFACE *GetLastTextureSurface();
DRZTEXTURE2 *GetLastTexture();

/////////////////////////[ BMP ���� �ε� ]///////////////////////////////
BYTE *LoadDib( char *Filename );
// Dib�� �����̽��� �׸���
int DrawSurfaceFromDib( DIRECTDRAWSURFACE lpDDSurface , BYTE *lpDib );

//////////////// [ BMP������ �ؽ��� �����̽��� ����� ] ///////////////
DIRECTDRAWSURFACE LoadDibSurface( char *Filename , int SysMem=FALSE );
////////// [ BMP������ �ؽ��� �����̽��� ����� (�����ؽ��Ķ� �ռ� ) ] ///////
DIRECTDRAWSURFACE LoadDibSurfaceAlpha( char *Filename , char *FileAlpha , int SysMem=FALSE );

//######################################################################################
//�� �� �� : �� �� ��
//////////////// [ BMP������ �ؽ��� �����̽��� ����� (�ڵ� ����) ] ///////////////
int smCreateTexture( smTEXTUREHANDLE *lpTexHandle, int MipMap = 0 );
//######################################################################################

DIRECTDRAWSURFACE LoadDibSurfaceOffscreen( char *Filename );

//DRZTEXTURE2 *LoadDibTexture( char *Filename );
//DRZTEXTURE2 *LoadDibTextureAlpha( char *Filename, char *FileAlpha );
int LoadDibTextureNum( char *Filename );


DRZTEXTURE2 GetTextureHandle( int num );
DRZTEXTURE2 GetTextureChildHandle( int num );





int LoadDibMapTexture( char *Filename, int tSize );
int RemoveMapTexture();
int SetMapImage( int x , int y );

int DrawAnimBG( DIRECTDRAWSURFACE lpDDSSrc , int frame );
int LoadAnimBG();
int CloseAnimBG();
int ClearBG( DIRECTDRAWSURFACE lpDDSSrc, DWORD color );



//���� ����
int DisplayError( HDC hdc , int x, int y );
int AddErrorList( char *szErrMsg );


