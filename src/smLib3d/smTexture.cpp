#include "common.h"
#include "smd3d.h"
#include <io.h>


int		g_IsReadTextures = 1;

BYTE	g_DibImageDateBuff[ (1024*768)*4 ];
int		g_DibLastReadWidth;
int		g_DibLastReadHeight;
int		g_TgaLastBitCnt;
int		g_TgaLastSwapHeight;

int						new_smCreateTexture( smTEXTUREHANDLE *lpTexHandle, int MipMap = 0 );

int						new_LoadDib( char *Filename );
int						new_LoadTga( char *Filename );
int						New_DrawSurfaceFromTga( DIRECTDRAWSURFACE lpDDSurface );

DIRECTDRAWSURFACE	new_LoadBmpCreateTexSurface( char *Filename, int MipMap, int &rTexHalf );
DIRECTDRAWSURFACE	new_LoadTgaCreateTexSurface( char *Filename, int MipMap, int &rTexHalf );
DIRECTDRAWSURFACE	new_LoadBmpSurfaceOffscreen( char *Filename );


//����ä���� ���� �ȼ� ���� ����
DDPIXELFORMAT	ddTextureFormat;
//����ä���� ���� �ȼ� ���� ����
DDPIXELFORMAT	ddDefaultTextureFormat;
//���� �ؽ��� ������ ���ϴ� �Լ�
int GetPixelFormatAlphaTexture();

#define ERROR_LIST_MAX		32
#define ERROR_LIST_MASK		(ERROR_LIST_MAX-1)

char szErrorList[ERROR_LIST_MAX][64];
int ErrorListCnt  = 0;


int AddErrorList( char *szErrMsg )
{
	/*
	int cnt;

	cnt = ErrorListCnt & ERROR_LIST_MASK;

	lstrcpy( szErrorList[cnt] , szErrMsg );

	ErrorListCnt++;
	*/
	return ErrorListCnt;
}

int DisplayError( HDC hdc , int x, int y )
{
	int cnt,mcnt;
	int len;
	int dy;

	dy = y;

	for(cnt=ErrorListCnt-ERROR_LIST_MASK;cnt<ErrorListCnt;cnt++) 
	{
		if ( cnt>=0 ) {
			mcnt = cnt & ERROR_LIST_MASK;
			len = lstrlen( szErrorList[mcnt] );
			//dsTextLineOut( hdc , x,dy ,szErrorList[mcnt] , len );
			dy+=14;
		}
	}

	return TRUE;
}

//���� ��� �޸� ����ġ
DWORD	VramTotal = 0;


#define	MAX_MAP_BLINK_COLOR_TIME				8

const char *szBlinkTimeScript[ MAX_MAP_BLINK_COLOR_TIME ] =
{
	"BLINK_TIME1:",
	"BLINK_TIME2:",
	"BLINK_TIME3:",
	"BLINK_TIME4:",
	"BLINK_TIME5:",
	"BLINK_TIME6:",
	"BLINK_TIME7:",
	"BLINK_TIME8:",
};

const DWORD dwBlinkTimeCode[ MAX_MAP_BLINK_COLOR_TIME ] =
{
    9, 10, 11, 12, 13, 14, 15, 16,
};



//�� �� �� : �� �� ��
int MipMapModeCreate;						// �Ӹ� ��� ���� ����.
int TextureSwapMode	= TRUE;					// �׷��� ����Ƽ�� ���ؼ� �Ӹ� ��� ���� �˱�����. ��������.
static int TexturHalfSize = FALSE;			// �ؽ��� �������� ũ�� ����

// 6.0 ������ �Ӹ� ������ ���� �ܰ� 256��256, 128��128, 64��64, 32��32, 16��16
// ���� 256��128 �� ���� �Ҽ��� ������. ī�庰�� �ȵǴ� �͵� �ִٰ� �մϴ�.
// ���� ������ �ִ� 3�ܰ� ������ ��� �ϱ�� ����.
static char MipMapFileListBuff[ MIPMAP_FILELIST ][16] = { "", "_mm1", "_mm2", "_mm3", "_mm4", };

static int  MipMapCount = 0;
static char MipMapFileBuff[ MIPMAP_FILELIST ][126] = { 0,};
static int  MipMapCountA = 0;
static char MipMapFileABuff[ MIPMAP_FILELIST ][126] = { 0,};
static char FileTypeBuff[16] = { 0,};

int		   MaterialFrame;

//�ؽ��� �޸� ���� ��� ����
int SetTextureSwapMode( int mode )
{
	int OldValue;

	OldValue = TextureSwapMode;

	TextureSwapMode = mode;

	return OldValue;
}

//�ؽ����� ���� ���� ( 0-��ȭ�� 1-���� 2-��ȭ�� )
int smSetTextureLevel( int Level )
{
	switch( Level ) {
	case 0:	//( ���� �޸𸮰� ū �ý��ۿ��� ���� )
		//��� �ؽ��ĸ� ���� �޸𸮷� �ø���
		TextureSwapMode = FALSE;
		TexturHalfSize = 0;
		break;

	case 1: //( �޸𸮰� ū �ý��ۿ��� ���� )
		//�ؽ��ĸ� ������ �ý��� �޸𸮿� ����
		TextureSwapMode = TRUE;
		TexturHalfSize = 0;
		break;

	case 2: //( ��� ���� �ý��ۿ��� ��� )
		//��� �ؽ��ĸ� ����ũ��� �ٿ��� ���� �޸𸮿� �ø�
		TextureSwapMode = TRUE;
		TexturHalfSize = 1;
		break;

	case 3: //( VRAM 8�ް� ���� ���� ���� �ý��ۿ��� ��� )
		//��� �ؽ��ĸ� 1/4ũ��� �ٿ��� ���� �޸𸮿� �ø�
		TextureSwapMode = TRUE;
		TexturHalfSize = 2;
		break;
	}

	return TRUE;
}

smTEXTURE::smTEXTURE()
{
	Init();
}

smTEXTURE::~smTEXTURE()
{
//	Remove();
}

void smTEXTURE::Init()
{
	//######################################################################################
	//�� �� �� : �� �� ��
	ZeroMemory( TexSpeedSum, sizeof(TexSpeedSum) );
	//######################################################################################

	ZeroMemory( Texture , sizeof( smTEXTUREHANDLE ) * MAX_TEXTURE ); 
	ZeroMemory( SwapTexture , sizeof( smSWAPTEXTURE ) * SWAPTEXTURE_MAX ); 
	TextureCnt	= 0;
	SwapCount	= 0;

	//ũ��Ƽ�� ���� �ʱ�ȭ
	InitializeCriticalSection( &cTextureSection );

}

int smTEXTURE::Remove()
{
	int cnt;

	//ũ��ƼĮ ���� ����
	EnterCriticalSection( &cTextureSection );

	for( cnt=0; cnt< SWAPTEXTURE_MAX ; cnt++ ) {
		if ( SwapTexture[cnt].SwapState == TRUE ) {
			DeleteSwapTexture( cnt );
		}
	}

	for( cnt=0;cnt<MAX_TEXTURE;cnt++) {
		if ( Texture[cnt].UseCounter>0 ) {
			//�ؽ��� ���� �ڵ� ����
			if ( Texture[cnt].lpD3DTexture ) Texture[cnt].lpD3DTexture->Release();
			if ( Texture[cnt].lpDDSurface ) Texture[cnt].lpDDSurface->Release();
			if ( Texture[cnt].lpDDSSysMemory ) Texture[cnt].lpDDSSysMemory->Release();
			Texture[cnt].UseCounter = 0;
		}
	}
	//ũ��ƼĮ ���� ����
	LeaveCriticalSection( &cTextureSection );

	//ũ��Ƽ�� ���� ����
	DeleteCriticalSection( &cTextureSection );

	return TRUE;
}

//int smTEXTURE::Find( char *name )
int smTEXTURE::Find( DWORD dwCode, char *Name )
{
	int cnt;
	for( cnt=0; cnt < MAX_TEXTURE; cnt++ )
	{
		if( TexSpeedSum[cnt] == dwCode )
		{
			if( Texture[cnt].UseCounter && _stricmp( Texture[cnt].Name, Name ) == 0 )
				return cnt;
		}
	}

	return -1;
}

//�ؽ��� ����� �߰�
smTEXTUREHANDLE *smTEXTURE::Add( char *name , char *nameA )
{
	int TexNum;
	int cnt;

	extern DWORD GetSpeedSum( char *szName );

	DWORD dwCode = GetSpeedSum(name);
	TexNum = Find( dwCode, name );

	if ( TexNum>=0 ) 
	{
		LastTextureCnt = TexNum;
		Texture[TexNum].UseCounter++;
		return &Texture[TexNum];
	}

	for( cnt=0;cnt<MAX_TEXTURE;cnt++) 
	{
		if ( Texture[ cnt ].UseCounter==0 ) 
			break;
	}

	if ( cnt==MAX_TEXTURE ) 
		return NULL;

	TextureCnt = cnt;

	lpLastTextureSurface = &Texture[ TextureCnt ].lpDDSurface;
	lpLastTexture = &Texture[ TextureCnt ].lpD3DTexture;

	//�ʱ� �ڵ� ���� 0 �̴�
	Texture[TextureCnt].lpDDSurface = NULL;
	Texture[TextureCnt].lpD3DTexture = NULL;

	lstrcpy( Texture[ TextureCnt ].Name , name );
	if ( nameA )
		lstrcpy( Texture[ TextureCnt ].NameA , nameA );
	else 
		Texture[ TextureCnt ].NameA[0] = 0;

	Texture[ TextureCnt ].TexSwapMode = TextureSwapMode;			// �⺻������ ���Ҹ�� �㰡
	Texture[ TextureCnt ].UseCounter++;
	Texture[ TextureCnt ].lpD3DTextureLarge = NULL;

	//######################################################################################
	//�� �� �� : �� �� ��	
	TexSpeedSum[ TextureCnt ] = dwCode;
	//######################################################################################

	return &Texture[ TextureCnt ];
}

//�ؽ��ĸ� �����Ѵ� ( 1�ܰ�� �����Ͽ� ��Ʈ������� ��ũ�� ���� ��� ���� ���� )
int smTEXTURE::Delete( smTEXTUREHANDLE *lpTex )
{
	int cnt;

	if ( lpTex->UseCounter<=0 ) return FALSE;

	lpTex->UseCounter--;

	if ( lpTex->UseCounter<=0 ) {
		//���� �ؽ��Ŀ��� ������̸� �����ؽ��ĸ� ã�Ƴ��� ����
		if ( lpTex->lpD3DTextureLarge ) {
			for( cnt=0; cnt<SWAPTEXTURE_MAX ; cnt++ ) {
				if ( SwapTexture[cnt].SwapState && SwapTexture[cnt].smTextureMaster==lpTex ) {
					DeleteSwapTexture( cnt );
					break;
				}
			}
		}
		//�ؽ��� ���� �ڵ� ����
		if ( lpTex->lpDDSurface ) lpTex->lpDDSurface->Release();
		if ( lpTex->lpD3DTexture ) lpTex->lpD3DTexture->Release();
		if ( lpTex->lpDDSSysMemory ) lpTex->lpDDSSysMemory->Release();

		lpTex->lpD3DTexture = 0;
		lpTex->lpDDSurface = 0;
		lpTex->lpDDSSysMemory = 0;

		VramTotal -= lpTex->Width * lpTex->Height  * 2 ;

	}

	return lpTex->UseCounter;
}

//SWAP �ؽ��ĸ� ����
int smTEXTURE::DeleteSwapTexture( int TexNum )
{
	//ũ��ƼĮ ���� ����
	EnterCriticalSection( &cTextureSection );

	if (SwapTexture[TexNum].lpDDSurface) {
		SwapTexture[TexNum].lpDDSurface->Release();
		SwapTexture[TexNum].lpDDSurface = 0;
	}

	if ( SwapTexture[TexNum].lpD3DTexture ) {
		SwapTexture[TexNum].lpD3DTexture->Release();
		SwapTexture[TexNum].lpD3DTexture = 0;
	}

	SwapTexture[TexNum].smTextureMaster->lpD3DTextureLarge = NULL;
	SwapTexture[TexNum].SwapState = FALSE;

	//���� �޸� ��� ����ġ
	VramTotal -= SwapTexture[TexNum].smTextureMaster->Width *  
		 SwapTexture[TexNum].smTextureMaster->Height  * 2 ;

	//ũ��ƼĮ ���� ����
	LeaveCriticalSection( &cTextureSection );

	return TRUE;
}

//ū�ؽ��ĸ� SWAP �Ͽ� ����� 
int smTEXTURE::SetLargeTexture( smTEXTUREHANDLE *TexHandle )
{
	int cnt;
	int OldTex = 0;
	int OldTexNum = 0;

	// ������ �����̽��� ���°�� ����
	if( TexHandle->lpDDSSysMemory==NULL )
		return FALSE;

	// ������ ������� ���� �ؽ��� ���� Ȯ���Ͽ� ������
	for ( cnt=0; cnt<SWAPTEXTURE_MAX ; cnt++ )
	{
		if ( SwapTexture[cnt].SwapState==TRUE && SwapTexture[cnt].smTextureMaster==TexHandle ) 
		{
			SwapTexture[cnt].UsedTime = SwapCount;
			return TRUE;
		}
	}

	// ������� ã�´�
	for( cnt=0 ; cnt<SWAPTEXTURE_MAX ; cnt++ ) 
	{
		if ( SwapTexture[cnt].SwapState==NULL ) break;
	}

	if ( cnt==SWAPTEXTURE_MAX )
	{
		//������� ������ ������� ������ �ؽ��ĸ� ã�´�

		OldTex = SwapTexture[cnt].UsedTime;

		for( cnt=0 ; cnt<SWAPTEXTURE_MAX ; cnt++ )
		{
			if ( SwapTexture[cnt].UsedTime < OldTex )
			{
				OldTex = SwapTexture[cnt].UsedTime;
				OldTexNum = cnt;
			}
		}

		//������ �ؽ��İ� ���� ��� ����
		if ( OldTex==SwapCount ) 
			return FALSE;

		//������ �ؽ��ĸ� �����
		DeleteSwapTexture( OldTexNum );
		cnt = OldTexNum;
	}

	//ũ��ƼĮ ���� ����
	EnterCriticalSection( &cTextureSection );

	// Now create surface
    DRZTEXTURE2 lpD3DTexture;
	DIRECTDRAWSURFACE lpDDSurface;
	DDSURFACEDESC2		ddsd;
	RECT				ddRect;
	int hresult;

	// ���ο� �ؽ��� �����̽� ����
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize		= sizeof(DDSURFACEDESC2);
	ddsd.dwWidth	= TexHandle->Width ;
    ddsd.dwHeight	= TexHandle->Height;
	ddsd.dwFlags	= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;

	//���� ä���� ������ �ȼ� ������ ���������
	if ( TexHandle->NameA[0] ) {
		memcpy( &ddsd.ddpfPixelFormat, &ddTextureFormat, sizeof(DDPIXELFORMAT) );
	}
	else {
		memcpy( &ddsd.ddpfPixelFormat, &ddDefaultTextureFormat, sizeof(DDPIXELFORMAT) );
	}

	//######################################################################################
	//�� �� �� : �� �� ��
	if (renderDevice.GetDeviceDesc_IsHardware())
	{
		//ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_TEXTURE;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	}
    else
		ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY| DDSCAPS_TEXTURE;
	//######################################################################################

	renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);

	// �ؽ��� �ڵ�� ����
	if ( lpDDSurface ) 
	{
		hresult = lpDDSurface->QueryInterface( IID_IDirect3DTexture2, (LPVOID*)&lpD3DTexture );
		if ( hresult != D3D_OK ) 
		{ 
			lpDDSurface->Release();	
			
			LeaveCriticalSection( &cTextureSection );	//ũ��ƼĮ ���� ����
			return FALSE; 
		}
	}
	else 
	{
		LeaveCriticalSection( &cTextureSection );		//ũ��ƼĮ ���� ����
		return FALSE;
	}

	ddRect.left		= 0;
	ddRect.right	= TexHandle->Width;
	ddRect.top		= 0;
	ddRect.bottom	= TexHandle ->Height;

	// ���� ū �׸��� �� �ؽ��ķ� ����
	//lpDDSurface->BltFast( 0, 0, TexHandle->lpDDSSysMemory ,&ddRect , DDBLTFAST_WAIT);
	renderDevice.BltFast(lpDDSurface, 0, 0, TexHandle->lpDDSSysMemory, &ddRect, DDBLTFAST_WAIT);

	// �ڵ� ����
	TexHandle->lpD3DTextureLarge  = lpD3DTexture;
	SwapTexture[cnt].lpD3DTexture = lpD3DTexture;
	SwapTexture[cnt].lpDDSurface  = lpDDSurface;
	SwapTexture[cnt].smTextureMaster = TexHandle;
	SwapTexture[cnt].UsedTime = SwapCount;
	SwapTexture[cnt].SwapState = TRUE;

	//���� �޸� ��� ����ġ
	VramTotal += TexHandle->Width * TexHandle->Height * 2; 

	//ũ��ƼĮ ���� ����
	LeaveCriticalSection( &cTextureSection );


	return TRUE;
}

//�ؽ��� 
int smTEXTURE::ReadTexture()
{
	if( g_IsReadTextures == 0 )
		return 0;

	int cnt;
	char szErrMsg[128];
	int	Result;
	int ReadCount = 0;

	for( cnt=0;cnt<MAX_TEXTURE;cnt++ )
	{
		if (Texture[cnt].UseCounter && !Texture[cnt].lpD3DTexture && Texture[cnt].loadingFailed < 1)
		{
			EnterCriticalSection( &cTextureSection );			//ũ��ƼĮ ���� ����

			Result = new_smCreateTexture( &Texture[cnt] );

			if ( Result<=0 )
			{
				Texture[cnt].loadingFailed = 1;

				switch( Result )
				{
				case 0:
					wsprintf( szErrMsg , "TEX_D3D ���� [%s]" , Texture[cnt].Name );
					break;
				case -1:
					wsprintf( szErrMsg , "TEX_FILE ���� [%s]" , Texture[cnt].Name );
					break;
				case -2:
					wsprintf( szErrMsg , "TEX_ALPHA ���� [%s]" , Texture[cnt].Name );
					break;
				}
				//���� ��� ����
				AddErrorList( szErrMsg );
			}
			ReadCount++;

			LeaveCriticalSection( &cTextureSection );		//ũ��ƼĮ ���� ����

			if ( (ReadCount&0x7)==0 )	
				smPlayWindowMessage();	//�޼��� ó��
		}
	}

	return TRUE;
}

//�Ҿ� ���� �ؽ��ĸ� ����� �ٽ� �ҷ� ���δ�
int smTEXTURE::Restore()
{
	int cnt;
	//int hresult;

	for( cnt=0; cnt< SWAPTEXTURE_MAX ; cnt++ ) 
	{
		if ( SwapTexture[cnt].SwapState == TRUE )
		{
			DeleteSwapTexture( cnt );
		}
	}

	EnterCriticalSection( &cTextureSection );			//ũ��ƼĮ ���� ����

	for( cnt=0;cnt<MAX_TEXTURE;cnt++ )
	{
		if ( Texture[cnt].UseCounter && Texture[cnt].lpD3DTexture ) 
		{
			if ( Texture[cnt].lpD3DTexture ) Texture[cnt].lpD3DTexture->Release();
			if ( Texture[cnt].lpDDSurface ) Texture[cnt].lpDDSurface->Release();
			if ( Texture[cnt].lpDDSSysMemory ) Texture[cnt].lpDDSSysMemory->Release();

			Texture[cnt].lpD3DTexture = 0;
			Texture[cnt].lpDDSurface = 0;
			Texture[cnt].lpDDSSysMemory = 0;
		}
	}
	LeaveCriticalSection( &cTextureSection );		//ũ��ƼĮ ���� ����

	ReadTexture();

	return TRUE;
}

smTEXTURE smTexture;

////////////////////// smMATERIAL_GROUP ////////////////////////

smMATERIAL_GROUP::smMATERIAL_GROUP()
{

}

smMATERIAL_GROUP::smMATERIAL_GROUP( int MaterialMax )
{
	Init( MaterialMax );
}

smMATERIAL_GROUP::~smMATERIAL_GROUP()
{
	Close();
}

// ��Ʈ���� �ʱ�ȭ
void smMATERIAL_GROUP::Init( int MaterialMax )
{

	MaterialCount = 0;
	MaxMaterial = MaterialMax;
	ReformTexture = 0;

	smMaterial = new smMATERIAL[ MaterialMax ];
	ZeroMemory( smMaterial , sizeof( smMATERIAL )*MaterialMax );

}

// ��Ʈ���� ����ȭ
int smMATERIAL_GROUP::Close()
{
	DWORD cnt , tcnt;

	for( cnt=0;cnt<MaterialCount;cnt++ ) {
		if ( smMaterial[cnt].InUse ) {
			//�ؽ��ĸ� �����Ѵ�
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter; tcnt++ ) 
				if ( smMaterial[cnt].smTexture[tcnt] )
					smTexture.Delete (  smMaterial[cnt].smTexture[tcnt]  );

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter; tcnt++ ) 
				if ( smMaterial[cnt].smAnimTexture[tcnt] )
					smTexture.Delete (  smMaterial[cnt].smAnimTexture[tcnt]  );

		}
	}

	delete smMaterial;

	return TRUE;
}

#define TEXFILENAME_SIZE	64

//������ ����Ÿ�� ũ�⸦ �̸� �˷���
int smMATERIAL_GROUP::GetSaveSize()
{
	int size;
	DWORD cnt ,tcnt;
	int	len,alen;

	size	 = sizeof( smMATERIAL_GROUP );

	for(cnt=0;cnt<MaterialCount;cnt++) {
		size+= sizeof( smMATERIAL );
		if ( smMaterial[cnt].InUse ) {
			size += sizeof(int);					//�ؽ��� �̸� ���̰� �� int��
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter ; tcnt++) {
				len = lstrlen( smMaterial[cnt].smTexture[tcnt]->Name )+1;
				alen = lstrlen( smMaterial[cnt].smTexture[tcnt]->NameA )+1;
				size += len;
				size += alen;
			}

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter ; tcnt++) {
				len = lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->Name )+1;
				alen = lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->NameA )+1;
				size += len;
				size += alen;
			}

		}
	}
	return size;
}

//����Ÿ�� ���Ϸ� ���� 
int smMATERIAL_GROUP::SaveFile( HANDLE hFile )
{
	DWORD	dwAcess;
	DWORD cnt ,tcnt;
	int	len;
	int size;

	size =	WriteFile( hFile , &Head	, sizeof( smMATERIAL_GROUP )	, &dwAcess , NULL );

	for(cnt=0;cnt<MaterialCount;cnt++) {
		//��Ʈ���� ����
		size+= WriteFile( hFile , &smMaterial[cnt] , sizeof( smMATERIAL )	, &dwAcess , NULL );

		if ( smMaterial[cnt].InUse ) {

			//�ؽ��� ���� ��Ͽ� ���� �̸� ���� ���
			len = 0;
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter ; tcnt++) {
				len += lstrlen( smMaterial[cnt].smTexture[tcnt]->Name )+1;
				len += lstrlen( smMaterial[cnt].smTexture[tcnt]->NameA )+1;
			}
			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter ; tcnt++) {
				len += lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->Name )+1;
				len += lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->NameA )+1;
			}
			//���ڿ� ���� �̸� ���
			size+= WriteFile( hFile , &len , sizeof(int) , &dwAcess , NULL );

			//�ؽ��� ���� �̸��� ���������� ����
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter ; tcnt++) {
				len = lstrlen( smMaterial[cnt].smTexture[tcnt]->Name )+1;
				size+= WriteFile( hFile , smMaterial[cnt].smTexture[tcnt]->Name , len , &dwAcess , NULL );
				len = lstrlen( smMaterial[cnt].smTexture[tcnt]->NameA )+1;
				size+= WriteFile( hFile , smMaterial[cnt].smTexture[tcnt]->NameA , len , &dwAcess , NULL );
			}

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter ; tcnt++) {
				len = lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->Name )+1;
				size+= WriteFile( hFile , smMaterial[cnt].smAnimTexture[tcnt]->Name , len , &dwAcess , NULL );
				len = lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->NameA )+1;
				size+= WriteFile( hFile , smMaterial[cnt].smAnimTexture[tcnt]->NameA , len , &dwAcess , NULL );
			}

		}
	}

	return size;
}

//���Ͽ��� ����Ÿ�� �ε� 
int smMATERIAL_GROUP::LoadFile( HANDLE hFile )
{
	//DRZ_EDIT (somewhere in this function will the client resized because some error)
	DWORD	dwAcess;
	DWORD	cnt ,tcnt;
	int		StrLen;
	int		size;
	char	szNameBuff[4096];
	char	*lpNameBuff;
	char	*szName , *szNameA;

	//�׷��� ������ �о��
	size=ReadFile( hFile , &Head , sizeof( smMATERIAL_GROUP ) , &dwAcess , NULL );

	//��Ʈ���� �޸� ����
	smMaterial = new smMATERIAL[ MaterialCount ];

	for(cnt=0;cnt<MaterialCount;cnt++)
	{
		//��Ʈ���� ����Ÿ �о��
		size+=	ReadFile( hFile , &smMaterial[cnt] , sizeof( smMATERIAL ) , &dwAcess , NULL );

		if ( smMaterial[cnt].InUse ) 
		{
			//�ؽ��� ���� ���� ũ�� ����
			size+=	ReadFile( hFile , &StrLen , sizeof( int ) , &dwAcess , NULL );
			//�ؽ��� ���� ���� ũ�⸸ŭ ����Ÿ ���� 
			size+=	ReadFile( hFile , szNameBuff , StrLen, &dwAcess , NULL );

			lpNameBuff = szNameBuff;

			//�ؽ��� ����
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter ; tcnt++)
			{
				szName = lpNameBuff;
				lpNameBuff += lstrlen( szName )+1;
				szNameA = lpNameBuff;
				lpNameBuff += lstrlen( szNameA )+1;

				if ( szNameA[0] )
					smMaterial[cnt].smTexture[tcnt] = smTexture.Add( szName , szNameA );
				else
					smMaterial[cnt].smTexture[tcnt] = smTexture.Add( szName );
			}

			//���ϸ��̼� �ؽ��� ����
			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter ; tcnt++)
			{
				szName = lpNameBuff;
				lpNameBuff += lstrlen( szName )+1;
				szNameA = lpNameBuff;
				lpNameBuff += lstrlen( szNameA )+1;

				if ( szNameA[0] )
					smMaterial[cnt].smAnimTexture[tcnt] = smTexture.Add( szName , szNameA );
				else
					smMaterial[cnt].smAnimTexture[tcnt] = smTexture.Add( szName );
			}
		}
	}

	return size;
}

//������ �ʴ� ��Ʈ���� ����
int smMATERIAL_GROUP::DeleteUnuseMaterial()
{
	DWORD cnt , dcnt , tcnt;

	dcnt = 0;
	for( cnt=0;cnt<MaterialCount;cnt++ ) 
	{
		if ( smMaterial[cnt].InUse==TRUE ) 
		{
			//�ؽ��ĸ� �����Ѵ�
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter; tcnt++ ) 
				smTexture.Delete (  smMaterial[cnt].smTexture[tcnt]  );

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter; tcnt++ ) 
				smTexture.Delete (  smMaterial[cnt].smAnimTexture[tcnt]  );

			ZeroMemory( &smMaterial[cnt] , sizeof( smMATERIAL ) );
			//smMaterial[cnt].InUse = FALSE;
			dcnt++;
		}
	}

	return dcnt;
}

//��Ʈ������ ���
int smMATERIAL_GROUP::AddMaterial( ASE_MATERIAL *aseMaterial , char *szScript )
{
	int cnt,len;
	int	AnimCount,FrameSpeed;
	int MatNum;
	char	*lpAnimFileList[16];
	char	*szDirectory;
	char	*lpScript;
	char	szTemp[64];

	MatNum = MaterialCount;
	MaterialCount++;

	smMaterial[MatNum].InUse  = TRUE;
	smMaterial[MatNum].Shade  = TRUE;
	smMaterial[MatNum].BlendType = SMMAT_BLEND_ALPHA;
	smMaterial[MatNum].TwoSide = aseMaterial->TwoSide;
	smMaterial[MatNum].ReformTexture = 0;

	// ���� �ؽ��Ŀ� ���
	if ( !aseMaterial->MAP_OPACITY[0] ) {
		for( cnt=0;cnt<aseMaterial->TextureCounter;cnt++) {
			smMaterial[MatNum].smTexture[cnt] = smTexture.Add( aseMaterial->BITMAP[cnt] );
			smMaterial[MatNum].TextureStageState[cnt] = aseMaterial->BitmapStateState[cnt];
			smMaterial[MatNum].TextureFormState[cnt] = aseMaterial->BitmapFormState[cnt];
			smMaterial[MatNum].ReformTexture += aseMaterial->BitmapFormState[cnt];
		}
		smMaterial[MatNum].MapOpacity = FALSE;
	}
	else {
		//######################################################################################
		//�� �� �� : �� �� ��
		if( aseMaterial->TextureCounter > 1 )
		{
			for( cnt=0; cnt < aseMaterial->TextureCounter-1; cnt++ )
			{
				smMaterial[MatNum].smTexture[cnt] = smTexture.Add( aseMaterial->BITMAP[cnt], aseMaterial->MAP_OPACITY );
				smMaterial[MatNum].TextureStageState[cnt] = aseMaterial->BitmapStateState[cnt];
				smMaterial[MatNum].TextureFormState[cnt] = aseMaterial->BitmapFormState[cnt];
				smMaterial[MatNum].ReformTexture += aseMaterial->BitmapFormState[cnt];
			}

			smMaterial[MatNum].smTexture[cnt] = smTexture.Add( aseMaterial->BITMAP[cnt] );
			smMaterial[MatNum].TextureStageState[cnt] = aseMaterial->BitmapStateState[cnt];
			smMaterial[MatNum].TextureFormState[cnt] = aseMaterial->BitmapFormState[cnt];
			smMaterial[MatNum].ReformTexture += aseMaterial->BitmapFormState[cnt];
		}
		else
		{
			for( cnt=0; cnt < aseMaterial->TextureCounter; cnt++ )
			{
				smMaterial[MatNum].smTexture[cnt] = smTexture.Add( aseMaterial->BITMAP[cnt] , aseMaterial->MAP_OPACITY );
				smMaterial[MatNum].TextureStageState[cnt] = aseMaterial->BitmapStateState[cnt];
				smMaterial[MatNum].TextureFormState[cnt] = aseMaterial->BitmapFormState[cnt];
				smMaterial[MatNum].ReformTexture += aseMaterial->BitmapFormState[cnt];
			}
		}

		smMaterial[MatNum].MapOpacity = TRUE;
		//######################################################################################
	}

	//��ǥ ���� �ؽ��� ���� ýũ�Ͽ� ����
	if ( smMaterial[MatNum].ReformTexture )	ReformTexture ++;

	smMaterial[MatNum].TextureCounter = aseMaterial->TextureCounter;	//�ؽ��ĸ��� ��
	smMaterial[MatNum].Transparency   = aseMaterial->Transparency ;		//������
	smMaterial[MatNum].SelfIllum	  = aseMaterial->SelfIllum ;		//��ü�߱�
	smMaterial[MatNum].Diffuse        = aseMaterial->Diffuse;			//�⺻����

	smMaterial[MatNum].UseState			= aseMaterial->ScriptState;
	smMaterial[MatNum].TextureType		= SMTEX_TYPE_MULTIMIX;

	if( aseMaterial->BlendType )
		smMaterial[MatNum].BlendType = aseMaterial->BlendType;

	//�ٴ� �浹 üũ
	if ( smMaterial[MatNum].Transparency==0 ) 
		smMaterial[MatNum].MeshState		= SMMAT_STAT_CHECK_FACE;

	//���� ��鸮��
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WIND) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDZ1;
		smMaterial[MatNum].MeshState		= 0;
	}
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDX1) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDX1;
		smMaterial[MatNum].MeshState		= 0;
	}
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDX2) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDX2;
		smMaterial[MatNum].MeshState		= 0;
	}
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDZ1) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDZ1;
		smMaterial[MatNum].MeshState		= 0;
	}
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDZ2) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDZ2;
		smMaterial[MatNum].MeshState		= 0;
	}

	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDZ2) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDZ2;
		smMaterial[MatNum].MeshState		= 0;
	}

	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WATER) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WATER;
		smMaterial[MatNum].MeshState		= 0;
	}

	//��Ʈ���� �浹 üũ �㰡
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_NOTPASS) ) {
		smMaterial[MatNum].MeshState		= SMMAT_STAT_CHECK_FACE;	//�㰡
	}
	else {
		if ( (aseMaterial->ScriptState&sMATS_SCRIPT_PASS) ) {
			smMaterial[MatNum].MeshState		= 0;					//�Ұ�
		}
	}
	//������ ���߿� �׸���
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_RENDLATTER) ) {
		smMaterial[MatNum].MeshState |= sMATS_SCRIPT_RENDLATTER;
	}

	//######################################################################################
	//�� �� �� : �� �� ��
	if( (aseMaterial->ScriptState & sMATS_SCRIPT_CHECK_ICE) )
		smMaterial[MatNum].MeshState |= sMATS_SCRIPT_CHECK_ICE;
	//######################################################################################

	//######################################################################################
	//�� �� �� : �� �� ��
	if( (aseMaterial->ScriptState & sMATS_SCRIPT_ORG_WATER) )
		smMaterial[MatNum].MeshState = sMATS_SCRIPT_ORG_WATER;
	//######################################################################################

	//�ڵ� ���ϸ��̼�
	AnimCount = 0;
	if ( szScript ) {
		if ( aseMaterial->ScriptState&sMATS_SCRIPT_ANIM2 ) AnimCount=2;
		if ( aseMaterial->ScriptState&sMATS_SCRIPT_ANIM4 ) AnimCount=4;
		if ( aseMaterial->ScriptState&sMATS_SCRIPT_ANIM8 ) AnimCount=8;
		if ( aseMaterial->ScriptState&sMATS_SCRIPT_ANIM16 ) AnimCount=16;
	}

	if ( AnimCount ) {
		len = lstrlen(szScript);

		for( cnt=0;cnt<len;cnt++) {
			if ( szScript[cnt]=='a' && szScript[cnt+1]=='n' && 
				szScript[cnt+2]=='i' && szScript[cnt+3]=='m' ) {
					for( cnt=cnt;cnt<len;cnt++) {
						if ( szScript[cnt]==':' ) break;
					}
					break;
			}
		}

		if ( cnt<len ) {
			cnt++;
			lpScript = &szScript[cnt];
			FrameSpeed = 0;

			for(cnt=cnt;cnt<len;cnt++) {
				if ( szScript[cnt]==':' ) {
					FrameSpeed = atoi( &szScript[cnt+1] );
					szScript[cnt] = 0;
					break;
				}
			}

			szDirectory = SetDirectoryFromFile( aseMaterial->BITMAP[0] );

			if ( szDirectory ) {
				for(cnt=0;cnt<AnimCount;cnt++) {
					lpAnimFileList[cnt] = new char[64];
					lstrcpy( szTemp , szDirectory );
					lstrcat( szTemp , lpScript );
					wsprintf( lpAnimFileList[cnt] , szTemp , cnt );
				}
			
				ChangeMaterialToAnimation( &smMaterial[MatNum] , lpAnimFileList , AnimCount , smMaterial[MatNum].BlendType , TRUE );
				if ( FrameSpeed ) smMaterial[MatNum].Shift_FrameSpeed = FrameSpeed;			//���ϸ��̼� Ÿ�̸� ����Ʈ

				smMaterial[MatNum].Transparency   = aseMaterial->Transparency ;		//������

				for(cnt=AnimCount-1;cnt>=0;cnt--) delete lpAnimFileList[cnt];
			}
		}
	}

	//######################################################################################
	// �� �� �� : �� �� ��
	if( (aseMaterial->ScriptState & sMATS_SCRIPT_BLINK_COLOR) && smMaterial[MatNum].WindMeshBottom == 0 )
	{
		for( cnt=0; cnt < MAX_MAP_BLINK_COLOR_TIME; cnt++ )
		{
			if( strstr( szScript, szBlinkTimeScript[cnt] ) != 0 )
				break;
		}

		if( cnt >= MAX_MAP_BLINK_COLOR_TIME )
			smMaterial[MatNum].WindMeshBottom = dwBlinkTimeCode[0];
		else
			smMaterial[MatNum].WindMeshBottom = dwBlinkTimeCode[cnt];
	}
	//######################################################################################

	aseMaterial->Regist ++;												//��Ʈ���� ��Ͽ� ���
	aseMaterial->RegistNum = MatNum;

	return MatNum;
}

//�ؽ��� ��Ʈ���� ����
int smMATERIAL_GROUP::CreateTextureMaterial( char *szBitmap , char *szOpacity , int TwoSide ,int Shade , DWORD TexSwap , DWORD BlendType )
{
	int MatNum = MaterialCount;
	MaterialCount++;

	smMaterial[MatNum].InUse  = TRUE;
	smMaterial[MatNum].Shade  = Shade;
	smMaterial[MatNum].TwoSide = TwoSide;

	if ( szOpacity ) 
		smMaterial[MatNum].smTexture[0] = smTexture.Add( szBitmap , szOpacity );
	else 
		smMaterial[MatNum].smTexture[0] = smTexture.Add( szBitmap );

	smMaterial[MatNum].ReformTexture = 0;

	smMaterial[MatNum].TextureStageState[0] = 0;
	smMaterial[MatNum].TextureFormState[0] = 0;

	smMaterial[MatNum].smTexture[0]->TexSwapMode = TexSwap;
	smMaterial[MatNum].MapOpacity = FALSE;

	smMaterial[MatNum].TextureCounter = 1;			//�ؽ��ĸ��� ��
	smMaterial[MatNum].Transparency   = 0;			//������
//	smMaterial[MatNum].Diffuse        = 0;			//�⺻����

	smMaterial[MatNum].UseState			= 0;
	smMaterial[MatNum].TextureType		= SMTEX_TYPE_MULTIMIX;
	smMaterial[MatNum].BlendType		= BlendType;


	smMaterial[MatNum].InUse++;

	return MatNum;
}

//��Ʈ���� �ؽ��ĸ� �߰��Ѵ�
int	smMATERIAL_GROUP::AddTexture( int MatNum , char *szBitmap , char *szOpacity )
{
	int cnt;

	cnt = smMaterial[MatNum].TextureCounter;			//�ؽ��ĸ��� ��

	if ( szOpacity ) 
		smMaterial[MatNum].smTexture[cnt] = smTexture.Add( szBitmap , szOpacity );
	else 
		smMaterial[MatNum].smTexture[cnt] = smTexture.Add( szBitmap );

	if ( smMaterial[MatNum].smTexture[cnt] )
		smMaterial[MatNum].TextureCounter++;

	return TRUE;
}

//�ؽ��� �̸����� ���� ��Ʈ������ ã�´�
smMATERIAL *smMATERIAL_GROUP::FindNextMaterialFromTextureName()
{
	DWORD cnt,cnt2;

	char *szName;

	szName = szLastSearchName;


	//�� ��Ʈ���� ã��
	for( cnt=LastSearchMaterial;cnt<MaterialCount;cnt++) {
		if ( smMaterial[cnt].InUse ) {

			for(cnt2=0;cnt2<smMaterial[cnt].TextureCounter;cnt2++) {
				if ( lstrcmpi( smMaterial[cnt].smTexture[cnt2]->Name , szName )==0 ) {
					return	&smMaterial[cnt];
				}
			}

			for(cnt2=0;cnt2<smMaterial[cnt].AnimTexCounter;cnt2++) {
				if ( lstrcmpi( smMaterial[cnt].smAnimTexture[cnt2]->Name , szName )==0 ) {
					return	&smMaterial[cnt];
				}
			}

		}
	}

	return NULL;
}

//�ؽ��� �̸����� ��Ʈ������ ã�´�
smMATERIAL *smMATERIAL_GROUP::FindMaterialFromTextureName( char *szName )
{
	LastSearchMaterial = 0;

	lstrcpy( szLastSearchName, szName );

	return FindNextMaterialFromTextureName();
}

//��Ʈ������ ���ϸ��̼� �������� ����
int smMATERIAL_GROUP::SetMaterialAnimFrame( smMATERIAL *lpMaterial , int frame )
{
	int OldFrame;

	OldFrame = lpMaterial->AnimationFrame;
	lpMaterial->AnimationFrame = frame;

	return OldFrame;
}

//��Ʈ������ ���ϸ��̼� �����ϰ� ����
int smMATERIAL_GROUP::ChangeMaterialToAnimation( smMATERIAL *lpMaterial , char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode )
{
	int cnt;

	for( cnt=0;cnt<NumTex;cnt++) 
	{
		lpMaterial->smAnimTexture[cnt] = smTexture.Add( szBitmap[cnt] );
		lpMaterial->smAnimTexture[cnt]->TexSwapMode = FALSE;
	}


	lpMaterial->MapOpacity = FALSE;
	lpMaterial->TextureCounter = 1;//NumTex;		//�ؽ��ĸ��� ��
	lpMaterial->Transparency   = 0;			//������

	lpMaterial->AnimTexCounter	= NumTex;		//���ϸ��̼� �ؽ��� ī����
	lpMaterial->FrameMask		= NumTex -1;	//���ϸ��̼� ������ ����ũ
	lpMaterial->Shift_FrameSpeed = 6;			//���ϸ��̼� Ÿ�̸� ����Ʈ

	lpMaterial->UseState			= 0;
	lpMaterial->TextureType		= SMTEX_TYPE_ANIMATION;
	lpMaterial->BlendType		= BlendType;

	//���ϸ��̼� �ڵ�
	if ( AutoAnimMode )	
		lpMaterial->AnimationFrame	= SMTEX_AUTOANIMATION;
	else
		lpMaterial->AnimationFrame	= 0;

	lpMaterial->InUse++;

	return TRUE;
}

//�ִϸ��̼� ��Ʈ���� ����
int smMATERIAL_GROUP::CreateAnimationMaterial( char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode )
{
	int cnt;

	smMATERIAL *lpMaterial;

	cnt = MaterialCount;
	MaterialCount++;

	lpMaterial = &smMaterial[cnt];

	lpMaterial->InUse  = TRUE;
	lpMaterial->Shade  = FALSE;
	lpMaterial->TwoSide = TRUE;

	ChangeMaterialToAnimation( lpMaterial , szBitmap , NumTex , BlendType , AutoAnimMode );

	return cnt;
}

//�ؽ��� ���ſ� ������
int smMATERIAL_GROUP::IncTextureFrame()
{
	return TRUE;
}

//��Ʈ������ �ؽ��� �����ΰ��� ������ �����Ѵ� ( �ε� �Ϸ��� ���� )
int smMATERIAL_GROUP::CheckMatreialTextureSwap()
{
	//######################################################################################
	//�� �� �� : �� �� ��
    return TRUE;
	//######################################################################################

	DWORD cnt , cnt2;

	//�� ��Ʈ���� ã��
	for( cnt=0;cnt<MaterialCount;cnt++) 
		if ( smMaterial[cnt].InUse ) {
			if ( smMaterial[cnt].TextureType==SMTEX_TYPE_MULTIMIX ) {
				for( cnt2=0; cnt2<smMaterial[cnt].TextureCounter; cnt2++ ) {
					if ( smMaterial[cnt].smTexture[cnt2] && smMaterial[cnt].smTexture[cnt2]->lpDDSSysMemory ) {
						smMaterial[cnt].TextureSwap = TRUE;
						break;
					}
				}
			}

			if ( smMaterial[cnt].TextureType==SMTEX_TYPE_ANIMATION ) {
				for( cnt2=0; cnt2<smMaterial[cnt].AnimTexCounter; cnt2++ ) {
					if ( smMaterial[cnt].smAnimTexture[cnt2] && smMaterial[cnt].smAnimTexture[cnt2]->lpDDSSysMemory ) {
						smMaterial[cnt].TextureSwap = TRUE;
						break;
					}
				}
			}

		}

	return TRUE;
}

//��Ʈ������ ������ �ؽ��ĸ� ã�� �����Ѵ�
int smMATERIAL_GROUP::ReSwapMaterial( smMATERIAL *lpMaterial )
{
	//######################################################################################
	//�� �� �� : �� �� ��
    return TRUE;
	//######################################################################################

	DWORD cnt;
	int result;

	lpMaterial->MatFrame = MaterialFrame;

	lpMaterial->TextureClip = FALSE;

	if ( lpMaterial->TextureSwap )
	{
		result = 0;
		for( cnt=0;cnt<lpMaterial->TextureCounter;cnt++) 
		{
			result |= smTexture.SetLargeTexture( lpMaterial->smTexture[cnt] );
		}

		if ( result ) 
		{
			lpMaterial->TextureClip = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

//�ش� ��Ʈ������ ��� �ؽ��ĸ� �ε�
int smMATERIAL_GROUP::ReadTextures( int MipMap )
{
	//######################################################################################
	//�� �� �� : �� �� ��
	if( g_IsReadTextures == 0 )
		return 0;
	//######################################################################################

	DWORD cnt;
	DWORD tcnt;
	int	ReadCount;
	smTEXTUREHANDLE	*lpTexture;

	//�ؽ��� ���� ũ��ƼĮ ���� ����
	smEnterTextureCriticalSection();

	ReadCount = 0;

	for( cnt=0;cnt<MaterialCount;cnt++ ) 
	{
		if ( smMaterial[cnt].InUse ) {
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter; tcnt++ )
			{
				if ( smMaterial[cnt].smTexture[tcnt] ) 
				{
					lpTexture = smMaterial[cnt].smTexture[tcnt];

					if (lpTexture->UseCounter && !lpTexture->lpD3DTexture && lpTexture->loadingFailed < 1)
					{
						if (smCreateTexture(lpTexture, MipMap) < 0)
							lpTexture->loadingFailed = 1;

						ReadCount++;
					}
				}
			}

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter; tcnt++ ) 
			{
				if ( smMaterial[cnt].smAnimTexture[tcnt] ) 
				{
					lpTexture = smMaterial[cnt].smAnimTexture[tcnt];
					if (lpTexture->UseCounter && !lpTexture->lpD3DTexture && lpTexture->loadingFailed < 1)
					{
						if (smCreateTexture(lpTexture, MipMap) < 0)
							lpTexture->loadingFailed = 1;

						ReadCount++;
					}
				}
			}
		}
	}

	//�ؽ��� ���� ũ��ƼĮ ���� ����
	smLeaveTextureCriticalSection();

	return ReadCount;
}


////////////////////////////////////////////////////////////////////////////////////////////////
smMATERIAL *smMaterial;

smMATERIAL_GROUP	*smMaterialGroup;

// ��Ʈ���� �ʱ�ȭ
int InitMaterial()
{
//	smMaterialGroup.Init( MATRIAL_MAX );
	smMaterialGroup = new smMATERIAL_GROUP( MATRIAL_MAX );
	smMaterial = smMaterialGroup->smMaterial;

	MaterialFrame = 0;

	return TRUE;
}

//��Ʈ���� ����
int CloseMaterial()
{
//	smMaterialGroup.Close();
	delete smMaterialGroup;
	return TRUE;
}

//������ �ʴ� ��Ʈ���� ����
int DeleteUnuseMaterial()
{
	return smMaterialGroup->DeleteUnuseMaterial();
}

//��Ʈ������ ���
int AddMaterial( ASE_MATERIAL *aseMaterial )
{
	return smMaterialGroup->AddMaterial( aseMaterial );
}

//�ؽ��� ��Ʈ���� ����
int CreateTextureMaterial( char *szBitmap , char *szOpacity , int TwoSide ,int Shade , DWORD TexSwap , DWORD BlendType )
{
	return smMaterialGroup->CreateTextureMaterial( szBitmap, szOpacity, TwoSide ,Shade ,TexSwap ,BlendType);
}

//��Ʈ���� �ؽ��ĸ� �߰��Ѵ�
int	AddTexture( int MatNum , char *szBitmap , char *szOpacity )
{
	return smMaterialGroup->AddTexture( MatNum , szBitmap , szOpacity );
}

//�ؽ��� �̸����� ���� ��Ʈ������ ã�´�
smMATERIAL *FindNextMaterialFromTextureName()
{
	return smMaterialGroup->FindNextMaterialFromTextureName();
}

//�ؽ��� �̸����� ��Ʈ������ ã�´�
smMATERIAL *FindMaterialFromTextureName( char *szName )
{
	return smMaterialGroup->FindMaterialFromTextureName( szName );
}

//��Ʈ������ ���ϸ��̼� �������� ����
int SetMaterialAnimFrame( smMATERIAL *lpMaterial , int frame )
{
	return smMaterialGroup->SetMaterialAnimFrame( lpMaterial , frame );
}

//��Ʈ������ ���ϸ��̼� �����ϰ� ����
int ChangeMaterialToAnimation( smMATERIAL *lpMaterial , char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode )
{
	return smMaterialGroup->ChangeMaterialToAnimation( lpMaterial, szBitmap, NumTex, BlendType, AutoAnimMode );
}

//�ִϸ��̼� ��Ʈ���� ����
int CreateAnimationMaterial( char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode )
{
	return smMaterialGroup->CreateAnimationMaterial( szBitmap, NumTex, BlendType, AutoAnimMode );
}

//�ؽ��� ���ſ� ������
int IncTextureFrame()
{
	MaterialFrame++;
	return smTexture.SwapCount ++;
}

//��Ʈ������ �ؽ��� �����ΰ��� ������ �����Ѵ� ( �ε� �Ϸ��� ���� )
int CheckMatreialTextureSwap()
{
	return smMaterialGroup->CheckMatreialTextureSwap();
}

//��Ʈ������ ������ �ؽ��ĸ� ã�� �����Ѵ�
int ReSwapMaterial( smMATERIAL *lpMaterial )
{
	return smMaterialGroup->ReSwapMaterial( lpMaterial );
}

struct smTEXTUREFORMAT 
{
	DWORD	BPP_DW;
	DWORD	MinAlpha;			//���ĺ�Ʈ ���ġ ( 0�̸� ���� )
	BOOL	bUsePalette;		//�ȷ�Ʈ ��� ����
	BOOL	bUseFourCC;			//FourCC �ɼ� ( FourCC?? ����������� ���� )
	BOOL	bFoundGoodFormat;	//�߰� Ȯ��
	DDPIXELFORMAT	*pddpf;
};

// �ؽ��� ���� ã�� ���� CALLBACK �Լ�
static HRESULT CALLBACK TextureSearchCallback( DDPIXELFORMAT* pddpf, VOID* param )
{
    if( NULL==pddpf || NULL==param )
        return DDENUMRET_OK;

	smTEXTUREFORMAT* ptsi = (smTEXTUREFORMAT*)param;

    // Skip any funky modes
    if( pddpf->dwFlags & (DDPF_LUMINANCE|DDPF_BUMPLUMINANCE|DDPF_BUMPDUDV) )
        return DDENUMRET_OK;

	// Check for palettized formats
	if( ptsi->bUsePalette )
	{
		if( !( pddpf->dwFlags & DDPF_PALETTEINDEXED8 ) )
			return DDENUMRET_OK;

		// Accept the first 8-bit palettized format we get
        memcpy( ptsi->pddpf, pddpf, sizeof(DDPIXELFORMAT) );
		ptsi->bFoundGoodFormat = TRUE;
        return DDENUMRET_CANCEL;
    }

	// Else, skip any paletized formats (all modes under 16bpp)
	if( pddpf->dwRGBBitCount < 16 )
		return DDENUMRET_OK;

	// Check for FourCC formats
    if( ptsi->bUseFourCC )
	{
		if( pddpf->dwFourCC == 0 )
		    return DDENUMRET_OK;

		return DDENUMRET_CANCEL;
	}

	// Else, skip any FourCC formats
	if( pddpf->dwFourCC != 0 )
		return DDENUMRET_OK;

	// Make sure current alpha format agrees with requested format type
	if( (ptsi->MinAlpha>0) && !(pddpf->dwFlags&DDPF_ALPHAPIXELS) )
		return DDENUMRET_OK;
	if( (ptsi->MinAlpha==0) && (pddpf->dwFlags&DDPF_ALPHAPIXELS) )
		return DDENUMRET_OK;

	DWORD	AlphaBitTest;
	int cnt;

	if( ptsi->MinAlpha>0) {
		AlphaBitTest = pddpf->dwRGBAlphaBitMask;
		for(cnt=0;cnt<32;cnt++) {
			if ( (AlphaBitTest&1) ) break;
			AlphaBitTest>>=1;
		}
		if ( ptsi->MinAlpha>AlphaBitTest ) 
			return DDENUMRET_OK;
	}

    // Check if we found a good match
    if( pddpf->dwRGBBitCount == ptsi->BPP_DW )
    {
        memcpy( ptsi->pddpf, pddpf, sizeof(DDPIXELFORMAT) );
		ptsi->bFoundGoodFormat = TRUE;
        return DDENUMRET_CANCEL;
    }

    return DDENUMRET_OK;
}

int GetPixelFormatAlphaTexture()
{
	smTEXTUREFORMAT TextureFormat;

	if (renderDevice.GetTextureBPP() == 32)
		TextureFormat.BPP_DW = 32;
	else 
		TextureFormat.BPP_DW = 16;

	TextureFormat.MinAlpha = 8;				//���� �ּ�ġ ( 8�ܰ� �̻��� ����ä�� )
	TextureFormat.bUseFourCC = FALSE;
	TextureFormat.bUsePalette = FALSE;
	TextureFormat.bFoundGoodFormat = FALSE;
	TextureFormat.pddpf = &ddTextureFormat;

	renderDevice.EnumTextureFormats(TextureSearchCallback, &TextureFormat);

	if (renderDevice.GetTextureBPP() == 32)
		TextureFormat.BPP_DW = 32;
	else 
		TextureFormat.BPP_DW = 16;

	TextureFormat.MinAlpha = 0;				//���ľ���
	TextureFormat.bUseFourCC = FALSE;
	TextureFormat.bUsePalette = FALSE;
	TextureFormat.bFoundGoodFormat = FALSE;
	TextureFormat.pddpf = &ddDefaultTextureFormat;
	renderDevice.EnumTextureFormats(TextureSearchCallback, &TextureFormat);

	return TRUE;
}

int InitTexture()
{
	smTexture.Init();

	GetPixelFormatAlphaTexture();			//���� �ؽ��� �ȼ����� ã��

	return TRUE;
}

int CloseTexture()
{
	smTexture.Remove();

	return TRUE;
}

int ReadTextures()
{
	//######################################################################################
	//�� �� �� : �� �� ��
	if( g_IsReadTextures == 0 )
		return 0;
	//######################################################################################

	int val;

	val = smTexture.ReadTexture();
	CheckMatreialTextureSwap();

	return val;
}

//�ؽ��ĸ� ýũ�Ͽ� �Ҿ���� �ؽ��ĸ� �ٽ� �����
int RestoreTextures()
{
	return smTexture.Restore();
}

DRZTEXTURE2 GetTextureHandle( int num )
{
	return smTexture.Texture[ num ].lpD3DTexture ;
}

DRZTEXTURE2 GetTextureChildHandle( int num )
{
	if ( smTexture.Texture[num].TexChild )
		return smTexture.Texture[ num ].TexChild->lpD3DTexture ;
	else 
		return NULL;
}

DIRECTDRAWSURFACE *GetLastTextureSurface()
{
	return smTexture.lpLastTextureSurface;
}

DRZTEXTURE2 *GetLastTexture()
{
	return smTexture.lpLastTexture ;
}

//�ؽ��� ���� ũ��ƼĮ ���� ����
void smEnterTextureCriticalSection()
{
	EnterCriticalSection( &smTexture.cTextureSection );
}

//�ؽ��� ���� ũ��ƼĮ ���� ����
void smLeaveTextureCriticalSection()
{
	LeaveCriticalSection( &smTexture.cTextureSection );
}

int hresult;

////////////////// BMP FILE READ ///////////////////
DWORD GetDibInfoHeaderSize(BYTE  *lpDib)
{
	return ((BITMAPINFOHEADER  *)lpDib)->biSize;
}

WORD GetDibWidth(BYTE  *lpDib)
{
	if (GetDibInfoHeaderSize(lpDib)+sizeof(BITMAPCOREHEADER))
		return (WORD) (((BITMAPCOREHEADER  *)lpDib)->bcWidth);
	else
		return (WORD) (((BITMAPINFOHEADER  *)lpDib)->biWidth);
}

WORD GetDibHeight (BYTE  *lpDib)
{
	if (GetDibInfoHeaderSize(lpDib)==sizeof(BITMAPCOREHEADER))
		return (WORD) (((BITMAPCOREHEADER  *) lpDib)->bcHeight);
	else
		return (WORD) (((BITMAPINFOHEADER  *) lpDib)->biHeight);
}

WORD GetDibBitCount(BYTE  *lpDib)
{
	return ((BITMAPINFOHEADER  *)lpDib)->biBitCount;
}

BYTE  *GetDibBitsAddr(BYTE  *lpDib)
{
	DWORD dwNumColors, dwColorTableSize;
	WORD  wBitCount;

	if (GetDibInfoHeaderSize(lpDib)==sizeof(BITMAPCOREHEADER))
	{
		wBitCount=((BITMAPCOREHEADER  *) lpDib)->bcBitCount;
		if(wBitCount!=24)
			dwNumColors=1L<<wBitCount;
		else dwNumColors=0;
		dwColorTableSize=dwNumColors*sizeof(RGBTRIPLE);
	}
	else
	{
		wBitCount=((BITMAPINFOHEADER  *)lpDib)->biBitCount;
		if(GetDibInfoHeaderSize(lpDib)>=36)
			dwNumColors=((BITMAPINFOHEADER  *)lpDib)->biClrUsed;
		else
			dwNumColors=0;
		if(dwNumColors==0)
		{
			if(wBitCount!=24)
				dwNumColors=1L<<wBitCount;
			else
				dwNumColors=0;
		}
		dwColorTableSize=dwNumColors*sizeof(RGBQUAD);
	}
	return lpDib+GetDibInfoHeaderSize(lpDib)+dwColorTableSize;
}

/////////////////////////[ BMP ���� �ε� ]///////////////////////////////
BYTE *LoadDib( char *Filename )
{
	BITMAPFILEHEADER	bmfh;
	BYTE 				*lpDib;
	DWORD				dwDibSize,dwHeaderSize;
	HANDLE	hFile;
	DWORD	dwAcess;


	//������ ����
	hFile = CreateFile( Filename , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) return NULL;
	//��Ʈ�� ��� ����
	ReadFile( hFile , &bmfh , sizeof( BITMAPFILEHEADER ) , &dwAcess , NULL );
	if (bmfh.bfType != *(WORD *)"BM" && bmfh.bfType != *(WORD *)"C8" && bmfh.bfType != *(WORD *)"A8" ) 
	{ CloseHandle( hFile ); return NULL; }

	//��� ��ȣȭ ����
	if ( bmfh.bfType == *(WORD *)"A8" ) {
		BYTE	ch;
		BYTE	*lpHeader;
		int		cnt;

		//����� ��ȣȭ
		lpHeader = (BYTE *)&bmfh;
		for( cnt=0;cnt<sizeof(bmfh);cnt++) {
			ch = (BYTE)( lpHeader[cnt]-(cnt*cnt) );
			lpHeader[cnt] = ch;
		}
	}

	dwDibSize=bmfh.bfSize - sizeof(BITMAPFILEHEADER);
	lpDib = new BYTE [dwDibSize];
	if(lpDib==NULL)	{ CloseHandle( hFile );	return NULL; }
	//��Ʈ�� ����
	ReadFile( hFile , lpDib , dwDibSize , &dwAcess , NULL );
	CloseHandle( hFile );
	dwHeaderSize=GetDibInfoHeaderSize(lpDib);
	if (dwHeaderSize<12||(dwHeaderSize>12 && dwHeaderSize < 16) ) 
	{	delete lpDib; return NULL; }

	return lpDib;
}

//////////////////// TGA ���� �о���̴� ũ���� //////////////////
class cTGA
{
public:
	BYTE	*m_pBuff;
	BYTE	*m_pData;
	int		Width , Height;
	int		SwapHeight;
	int		BitCount;

	cTGA();
	~cTGA();

	int LoadTga( char *Filename );
	int GetWidth();
	int GetHeight();
	int GetBitCount();

};

cTGA::cTGA()
{
	m_pBuff = 0;
	m_pData = 0;
	Width = 0;
	Height = 0;
	BitCount = 0;
}

cTGA::~cTGA()
{
	if ( m_pBuff ) delete m_pData;
}

// TGA���� �ε�
int cTGA::LoadTga( char *Filename )
{
	int len;
	BYTE TGAHeader[0x12];
	WORD *Bit16Buff;
	WORD r,g,b;

	int DestCnt;
	int SrcCnt;

	HANDLE	hFile;
	DWORD	dwAcess;

	//������ ����
	hFile = CreateFile( Filename , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	//TGA ��� ����
	ReadFile( hFile , TGAHeader , 0x12 , &dwAcess , NULL );

	//��� ��ȣȭ ����
	if ( TGAHeader[0]=='G' && TGAHeader[1]=='8' ) {
		BYTE	ch;
		BYTE	*lpHeader;
		int		cnt;

		//����� ��ȣȭ
		lpHeader = (BYTE *)&TGAHeader;
		for( cnt=0;cnt<sizeof(TGAHeader);cnt++) {
			ch = (BYTE)( lpHeader[cnt]-(cnt*cnt) );
			lpHeader[cnt] = ch;
		}
	}

	Width = *((WORD *)(&TGAHeader[0xC]));
	Height = *((WORD *)(&TGAHeader[0xE]));

	if ( (TGAHeader[0x11]&0x20) ) {
		SwapHeight = FALSE;
	}
	else
		SwapHeight = TRUE;

	switch( TGAHeader[0x10] ) {
	case 16:		//555 �� RGB 888 �� ��ȯ
		m_pBuff = new BYTE [Width*Height*3];
		len = Width*Height;
		Bit16Buff = new WORD [ len ];
		ReadFile( hFile , Bit16Buff , len*2 , &dwAcess , NULL );
		//fread( Bit16Buff , len*2 , 1, fp );
		DestCnt = 0;
		for( SrcCnt=0;SrcCnt<len;SrcCnt++ ) {
			r = ( Bit16Buff[SrcCnt]>>10 ) & 0x1F;
			g = ( Bit16Buff[SrcCnt]>>5 ) & 0x1F;
			b = ( Bit16Buff[SrcCnt] ) & 0x1F;

			m_pBuff[ DestCnt   ] = (r<<3) | (r>>2);
			m_pBuff[ DestCnt+1 ] = (g<<3) | (g>>2);
			m_pBuff[ DestCnt+2 ] = (b<<3) | (b>>2);
			DestCnt+=3;
		}
		delete Bit16Buff;
		BitCount = 24;
		break;
	case 24:
		m_pBuff = new BYTE [Width*Height*3];
		ReadFile( hFile , m_pBuff , Width*Height*3 , &dwAcess , NULL );
		//fread( m_pBuff , Width*Height*3 , 1, fp );
		BitCount = 24;
		break;
	case 32:
		m_pBuff = new BYTE [Width*Height*4];
		ReadFile( hFile , m_pBuff , Width*Height*4 , &dwAcess , NULL );
		//fread( m_pBuff , Width*Height*4 , 1, fp );
		BitCount = 32;
		break;
	}
	CloseHandle( hFile );

	m_pData = m_pBuff;

	return TRUE;
}

int cTGA::GetWidth()
{
	return Width;
}

int cTGA::GetHeight()
{
	return Height;
}

int cTGA::GetBitCount()
{
	return BitCount;
}

#define smTEXFILETYPE_BMP		0
#define smTEXFILETYPE_JPG		1
#define smTEXFILETYPE_TGA		2

//������ Ȯ���ڸ� �˻��Ͽ� ���� ������ ã�´�
int GetTextureFileType( char *Filename ) 
{
	int len;
	int cnt;
	char *szFileExt = 0;

	len = lstrlen( Filename );

	for(cnt=len-1;cnt>=0;cnt--) 
	{
		if ( Filename[cnt]=='.' ) 
		{
			szFileExt = Filename+cnt+1;
			break;
		}
	}

	if ( !szFileExt ) 
		return -1;

	if ( ( szFileExt[0]=='j' || szFileExt[0]=='J') && 
		 ( szFileExt[1]=='p' || szFileExt[1]=='P') &&
		 ( szFileExt[2]=='g' || szFileExt[2]=='G') ) 
		 return smTEXFILETYPE_JPG;

	if ( ( szFileExt[0]=='b' || szFileExt[0]=='B') && 
		 ( szFileExt[1]=='m' || szFileExt[1]=='M') &&
		 ( szFileExt[2]=='p' || szFileExt[2]=='P') ) 
		 return smTEXFILETYPE_BMP;

	if ( ( szFileExt[0]=='t' || szFileExt[0]=='T') && 
		 ( szFileExt[1]=='g' || szFileExt[1]=='G') &&
		 ( szFileExt[2]=='a' || szFileExt[2]=='A') ) 
		 return smTEXFILETYPE_TGA;


	if ( ( szFileExt[0]=='s' || szFileExt[0]=='S') && 
		 ( szFileExt[1]=='t' || szFileExt[1]=='T') &&
		 ( szFileExt[2]=='g' || szFileExt[2]=='G') ) 
		 return smTEXFILETYPE_JPG;

	if ( ( szFileExt[0]=='s' || szFileExt[0]=='S') && 
		 ( szFileExt[1]=='t' || szFileExt[1]=='T') &&
		 ( szFileExt[2]=='a' || szFileExt[2]=='A') ) 
		 return smTEXFILETYPE_TGA;

	return -1;
}

// �� ��   : smCreateTexture() �Լ����� ȣ��ÿ��� �ƹ� ���� ������. ���� ���� �ٸ� ������
//	ȣ�� �ߴٸ�.  MipMapCount �� ���� �ʱ�ȭ �Ŀ� ȣ���ؼ� ��� �ؾ� �մϴ�.
int LoadDibSurfaceAlphaMipMap(DIRECTDRAWSURFACE lpDDSurface, char *Filename, char *FileAlpha, int CheckWidth, int CheckHeight)
{
	BYTE 				*lpDib;
	BYTE				*dAddr;
	int					height,width;

	BYTE 				*lpDibAlpha;
	BYTE				*dAddrAlpha;
	int					BitCnt;
	int					AlphaBitCnt=0;
	int					SwapHeight;
	int					SwapHeightAl;

	int FileTypeCl;
	int FileTypeAl;
	CJpeg	*cJpeg =0;
	CJpeg	*cJpegAl =0;
	cTGA	*cTga = 0;
	cTGA	*cTgaAl = 0;
	lpDib = 0;
	lpDibAlpha = 0;

	SwapHeight = TRUE;
	SwapHeightAl = TRUE;

	FileTypeCl = GetTextureFileType( Filename ) ;
	FileTypeAl = GetTextureFileType( FileAlpha ) ;


	switch( FileTypeCl ) {
	case smTEXFILETYPE_TGA:
		cTga = new cTGA;
		if ( cTga->LoadTga( Filename ) ) {
			dAddr = cTga->m_pData;
			width = cTga->GetWidth();
			height = cTga->GetHeight();
			BitCnt = cTga->GetBitCount();
			SwapHeightAl = cTga->SwapHeight;
//			SwapHeight = FALSE;
		}
		else
		{
			//######################################################################################
			//�� �� �� : �� �� ��
			if( cTga ) delete cTga;			
			BitCnt = 0;
			return -1;
			//######################################################################################
		}

		break;

	case smTEXFILETYPE_BMP:
		lpDib = LoadDib( Filename );
		if ( lpDib ) {
			dAddr	=GetDibBitsAddr(lpDib);
			height	=GetDibHeight(lpDib);
			width	=GetDibWidth(lpDib);
			BitCnt = GetDibBitCount(lpDib);
		}
		else
		{
			//######################################################################################
			//�� �� �� : �� �� ��
			if( lpDib ) delete lpDib;
			return -1;
			//######################################################################################
		}
		break;

	case smTEXFILETYPE_JPG:
		cJpeg = new CJpeg;
		cJpeg->LoadJPG( Filename );
		if ( cJpeg->m_pData ) {
			dAddr	=cJpeg->m_pData;
			height	=cJpeg->GetHeight();
			width	=cJpeg->GetWidth();
			BitCnt = 24;
		}
		else
		{
			//######################################################################################
			//�� �� �� : �� �� ��
			if( cJpeg ) delete cJpeg;
			return -1;
			//######################################################################################
		}
		break;
	}

	//######################################################################################
	//�� �� �� : �� �� ��
	if( width != CheckWidth || height != CheckHeight )
	{
		if( lpDib ) delete lpDib;
		if( cTga ) delete cTga;
		if( cJpeg ) delete cJpeg;
		return -1;
	}
	//######################################################################################


	//######################################################################################
	//�� �� �� : �� �� ��
	int ErrorFileTypeAl = 0;
	switch( FileTypeAl )
	{
	case smTEXFILETYPE_TGA:
		cTgaAl = new cTGA;
		if( cTgaAl->LoadTga( FileAlpha ) )
		{
			dAddrAlpha = cTgaAl->m_pData;
			AlphaBitCnt = cTgaAl->GetBitCount();
			SwapHeightAl = cTgaAl->SwapHeight;

			if( CheckWidth != cTgaAl->GetWidth() || CheckHeight != cTgaAl->GetHeight() )
				ErrorFileTypeAl = 1;
		}
		else
		{
			dAddrAlpha = NULL;
			AlphaBitCnt = 0;
		}
		break;

	case smTEXFILETYPE_BMP:
		lpDibAlpha = LoadDib( FileAlpha );
		if( lpDibAlpha )
		{
			dAddrAlpha	=GetDibBitsAddr(lpDibAlpha);
			AlphaBitCnt = GetDibBitCount(lpDibAlpha);

			if( CheckWidth != GetDibWidth(lpDibAlpha) || CheckHeight != GetDibHeight(lpDibAlpha) )
				ErrorFileTypeAl = 1;
		}
		else
		{
			dAddrAlpha = NULL;
			AlphaBitCnt = 0;
		}
		break;

	case smTEXFILETYPE_JPG:
		cJpegAl = new CJpeg;
		cJpegAl->LoadJPG( FileAlpha );
		if( cJpegAl->m_pData )
		{
			dAddrAlpha	=cJpegAl->m_pData;
			AlphaBitCnt = 24;

			if( CheckWidth != cJpegAl->GetWidth() || CheckHeight != cJpegAl->GetHeight() )
				ErrorFileTypeAl = 1;
		}
		else
		{
			dAddrAlpha = NULL;
			AlphaBitCnt = 0;
		}
		break;
	}

	if( ErrorFileTypeAl )
	{
		if( lpDib ) delete lpDib;
		if( cTga ) delete cTga;
		if( cJpeg ) delete cJpeg;
		
		if ( lpDibAlpha ) delete lpDibAlpha;
		if ( cJpegAl ) delete cJpegAl;
		if ( cTgaAl ) delete cTgaAl;

		return -2;
	}
	//######################################################################################

	int i,j,k;
	int r,g,b,a,tp;
	BYTE *Dest;
	DWORD MaskAll;

	DWORD Mask[4];
	DWORD Shift[4];
	DWORD ShiftPixel[4];
	DWORD ShiftTemp;


	Mask[0] = ddTextureFormat.dwRBitMask;
	Mask[1] = ddTextureFormat.dwGBitMask;
	Mask[2] = ddTextureFormat.dwBBitMask;
	Mask[3] = ddTextureFormat.dwRGBAlphaBitMask;

	MaskAll = Mask[0]|Mask[1]|Mask[2]|Mask[3];

	for( i=0; i<4; i++ ) 
	{
		if ( Mask[i] ) {
			ShiftTemp = Mask[i];
			for( j=0;j<32;j++ )
			{
				if ( ShiftTemp&1 ) break;
				ShiftTemp>>=1;
			}
			Shift[i] = j;
			for( j=0;j<32;j++) 
			{
				if ( (ShiftTemp&1)==0 ) break;
				ShiftTemp>>=1;
			}
			ShiftPixel[i] = j;
		}
		else
		{
			Shift[i] = 32;
			ShiftPixel[i] = 32;
		}
	}

	int pBuff,pBuffAl;
	int incByte = 3;
	int incByteAl = 3;
	int pBuff_k;

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	lpDDSurface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
	Dest = (BYTE *)ddsd.lpSurface;

	if ( !Dest ) 
		goto Lop_Quit;

		pBuff_k = 0;
		k=0;
		for(i=0;i<height;i++) 
		{
			switch( BitCnt ) 
			{
			case 24:
				if ( SwapHeight ) pBuff = (height-i-1)*width*3;
				else pBuff = i*width*3;
				incByte = 3;
				break;
			case 32:
				if ( SwapHeight ) pBuff = ((height-i-1)*width)<<2;
				else pBuff = (i*width)<<2;
				incByte = 4;
				break;
			}

			switch( AlphaBitCnt ) {
			case 8:
				if ( SwapHeightAl ) pBuffAl = (height-i-1)*width;
				else pBuffAl = i*width;
				incByteAl = 1;
				break;

			case 24:
				if ( SwapHeightAl ) pBuffAl = (height-i-1)*width*3;
				else pBuffAl = i*width*3;
				incByteAl = 3;
				break;

			case 32:
				if ( SwapHeightAl ) pBuffAl = ((height-i-1)*width)<<2;
				else pBuffAl = (i*width)<<2;
				incByteAl = 4;
				break;
			}

			//if ( (int)ddsd.lPitch!=0 )  k = ((int)ddsd.lPitch*i);			// �����̽� ��ġ ����

			k = pBuff_k;

			for(j=0;j<width;j++) {

				b = dAddr[ pBuff ]; 
				g = dAddr[ pBuff +1]; 
				r = dAddr[ pBuff +2];

				if ( BitCnt==32 ) {
					a = dAddr[ pBuff +3];
				}
				else {
					a = 255;

					switch( AlphaBitCnt ) {
					case 8: 
						a = dAddrAlpha[ pBuffAl ];//(height-i-1)*width + j ];
						break;
					case 24:
					case 32:
						a = dAddrAlpha[ pBuffAl ] + dAddrAlpha[ pBuffAl+1 ] + dAddrAlpha[ pBuffAl+2 ];
						a /=3;
						break;
					}
					pBuffAl+=incByteAl;
				}
				pBuff +=incByte;

				r = (( r>>(8-ShiftPixel[0] ) )<< Shift[0] ) & Mask[0];
				g = (( g>>(8-ShiftPixel[1] ) )<< Shift[1] ) & Mask[1];
				b = (( b>>(8-ShiftPixel[2] ) )<< Shift[2] ) & Mask[2];
				a = (( a>>(8-ShiftPixel[3] ) )<< Shift[3] ) & Mask[3];

				tp = r|g|b|a;											// �����÷��� ��ȯ

//				Dest[ k++ ] = tp&0xFF;									// ������ �����÷���
//				Dest[ k++ ] = tp>>8;

				if ( MaskAll<=0xFFFF ) {
					*((WORD *)&Dest[ k ]) = (WORD)tp;
					k+=2;
				}
				else {
					*((DWORD *)&Dest[ k ]) = tp;
					k+=4;
				}
			}

			pBuff_k += (int)ddsd.lPitch;

		}
		lpDDSurface->Unlock(NULL);
		//######################################################################################

Lop_Quit:

	if ( cJpeg ) 
		delete cJpeg;
	if ( lpDib ) 
		delete lpDib;
	if ( cTga ) 
		delete cTga;

	if ( cJpegAl ) 
		delete cJpegAl;
	if ( lpDibAlpha ) 
		delete lpDibAlpha;
	if ( cTgaAl ) 
		delete cTgaAl;

	return TRUE;
}

/////////[ BMP������ �ؽ��� �����̽��� ����� ( ����ä�� ��� ) ] ///////////
DIRECTDRAWSURFACE LoadDibSurfaceAlpha(char *Filename, char *FileAlpha, int SysMem)
{
	BYTE 				*lpDib;
	BYTE				*dAddr;
	int					height,width;

	BYTE 				*lpDibAlpha;
	BYTE				*dAddrAlpha;
	int					BitCnt;
	int					AlphaBitCnt=0;
	int					SwapHeight;
	int					SwapHeightAl;

	int FileTypeCl;
	int FileTypeAl;
	CJpeg	*cJpeg =0;
	CJpeg	*cJpegAl =0;
	cTGA	*cTga = 0;
	cTGA	*cTgaAl = 0;
	lpDib = 0;
	lpDibAlpha = 0;

	SwapHeight = TRUE;
	SwapHeightAl = TRUE;

	FileTypeCl = GetTextureFileType( Filename ) ;
	FileTypeAl = GetTextureFileType( FileAlpha ) ;


	switch( FileTypeCl )
	{
	case smTEXFILETYPE_TGA:
		cTga = new cTGA;
		if ( cTga->LoadTga( Filename ) ) 
		{
			dAddr = cTga->m_pData;
			width = cTga->GetWidth();
			height = cTga->GetHeight();
			BitCnt = cTga->GetBitCount();
			SwapHeightAl = cTga->SwapHeight;
		}
		else
		{
			if( cTga ) 
				delete cTga;

			BitCnt = 0;
			return NULL;
		}
		break;

	case smTEXFILETYPE_BMP:
		lpDib = LoadDib( Filename );
		if ( lpDib ) 
		{
			dAddr	=GetDibBitsAddr(lpDib);
			height	=GetDibHeight(lpDib);
			width	=GetDibWidth(lpDib);
			BitCnt = GetDibBitCount(lpDib);
		}
		else
		{
			if( lpDib ) 
				delete lpDib;

			return NULL;
		}
		break;

	case smTEXFILETYPE_JPG:
		cJpeg = new CJpeg;
		cJpeg->LoadJPG( Filename );
		if ( cJpeg->m_pData )
		{
			dAddr	=cJpeg->m_pData;
			height	=cJpeg->GetHeight();
			width	=cJpeg->GetWidth();
			BitCnt = 24;
		}
		else
		{
			if( cJpeg ) 
				delete cJpeg;

			return NULL;
		}
		break;
	}

	switch( FileTypeAl ) 
	{
	case smTEXFILETYPE_TGA:
		cTgaAl = new cTGA;
		if ( cTgaAl->LoadTga( FileAlpha ) )
		{
			dAddrAlpha = cTgaAl->m_pData;
			AlphaBitCnt = cTgaAl->GetBitCount();
			SwapHeightAl = cTgaAl->SwapHeight;
		}
		else 
		{
			dAddrAlpha = NULL;
			AlphaBitCnt = 0;
		}
		break;

	case smTEXFILETYPE_BMP:
		lpDibAlpha = LoadDib( FileAlpha );
		if ( lpDibAlpha ) 
		{
			dAddrAlpha	=GetDibBitsAddr(lpDibAlpha);
			AlphaBitCnt = GetDibBitCount(lpDibAlpha);
		}
		else 
		{
			dAddrAlpha = NULL;
			AlphaBitCnt = 0;
		}
		break;

	case smTEXFILETYPE_JPG:
		cJpegAl = new CJpeg;
		cJpegAl->LoadJPG( FileAlpha );
		if ( cJpegAl->m_pData ) 
		{
			dAddrAlpha	=cJpegAl->m_pData;
			AlphaBitCnt = 24;
		}
		else 
		{
			dAddrAlpha = NULL;
			AlphaBitCnt = 0;
		}
		break;
	}

	// Now create surface
	DIRECTDRAWSURFACE lpDDSurface;
	DDSURFACEDESC2		ddsd;

	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

	ddsd.dwSize		= sizeof(DDSURFACEDESC2);
	ddsd.dwFlags	= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH  | DDSD_PIXELFORMAT;
	ddsd.dwWidth	= width;
    ddsd.dwHeight	= height;

    memcpy( &ddsd.ddpfPixelFormat , &ddTextureFormat, sizeof(DDPIXELFORMAT) );

	//######################################################################################
	//�� �� �� : �� �� ��
	if (renderDevice.GetDeviceDesc_IsHardware() && SysMem == FALSE)
	{
		//ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_TEXTURE;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	}
    else
        ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY| DDSCAPS_TEXTURE;
	//######################################################################################

	hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);

	//
    if ( hresult != DD_OK ) 
	{ 
		if ( cJpeg ) delete cJpeg;
		if ( lpDib ) delete lpDib;
		if ( cTga ) delete cTga;
		if ( cJpegAl ) delete cJpegAl;
		if( lpDibAlpha ) delete lpDibAlpha;
		if ( cTgaAl ) delete cTgaAl;
		return NULL; 
	}

	int i,j,k;
	int r,g,b,a,tp;
	BYTE *Dest;
	DWORD MaskAll;

	DWORD Mask[4];
	DWORD Shift[4];
	DWORD ShiftPixel[4];
	DWORD ShiftTemp;


	Mask[0] = ddTextureFormat.dwRBitMask;
	Mask[1] = ddTextureFormat.dwGBitMask;
	Mask[2] = ddTextureFormat.dwBBitMask;
	Mask[3] = ddTextureFormat.dwRGBAlphaBitMask;

	MaskAll = Mask[0]|Mask[1]|Mask[2]|Mask[3];

	for( i=0; i<4; i++ ) 
	{
		if ( Mask[i] ) 
		{
			ShiftTemp = Mask[i];
			for( j=0;j<32;j++ ) 
			{
				if ( ShiftTemp&1 ) break;
				ShiftTemp>>=1;
			}
			Shift[i] = j;
			for( j=0;j<32;j++) 
			{
				if ( (ShiftTemp&1)==0 ) break;
				ShiftTemp>>=1;
			}
			ShiftPixel[i] = j;
		}
		else 
		{
			Shift[i] = 32;
			ShiftPixel[i] = 32;
		}
	}

/*
	for( i=0; i<4; i++ ) {
		if ( Mask[i] ) {
			ShiftTemp = Mask[i];
			for( j=0;j<32;j++ ) {
				if ( ShiftTemp&1 ) break;
				ShiftTemp>>=1;
			}
			Shift[i] = j;
			for( j=0;j<32;j++) {
				if ( (ShiftTemp&1)==0 ) break;
				ShiftTemp>>=1;
			}
			ShiftPixel[i] = j;
		}
		else {
			Shift[i] = 16;
			ShiftPixel[i] = 16;
		}
	}
*/

	int pBuff,pBuffAl;
	int incByte = 3;
	int incByteAl = 3;
	int pBuff_k;

	ddsd.dwSize		= sizeof(DDSURFACEDESC2);
	lpDDSurface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
	Dest = (BYTE *)ddsd.lpSurface;

	if ( !Dest ) 
		goto Lop_Quit;

		pBuff_k = 0;
		k=0;
		for(i=0;i<height;i++)
		{
			switch( BitCnt )
			{
			case 24:
				if ( SwapHeight ) pBuff = (height-i-1)*width*3;
				else pBuff = i*width*3;
				incByte = 3;
				break;
			case 32:
				if ( SwapHeight ) pBuff = ((height-i-1)*width)<<2;
				else pBuff = (i*width)<<2;
				incByte = 4;
				break;
			}

			switch( AlphaBitCnt ) 
			{
			case 8:
				if ( SwapHeightAl ) pBuffAl = (height-i-1)*width;
				else pBuffAl = i*width;
				incByteAl = 1;
				break;
			case 24:
				if ( SwapHeightAl ) pBuffAl = (height-i-1)*width*3;
				else pBuffAl = i*width*3;
				incByteAl = 3;
				break;
			case 32:
				if ( SwapHeightAl ) pBuffAl = ((height-i-1)*width)<<2;
				else pBuffAl = (i*width)<<2;
				incByteAl = 4;
				break;
			}

			k = pBuff_k;

			for(j=0;j<width;j++) 
			{
				b = dAddr[ pBuff ]; 
				g = dAddr[ pBuff +1]; 
				r = dAddr[ pBuff +2];

				if ( BitCnt==32 )
				{
					a = dAddr[ pBuff +3];
				}
				else
				{
					a = 255;

					switch( AlphaBitCnt )
					{
					case 8: 
						a = dAddrAlpha[ pBuffAl ];
						break;
					case 24:
					case 32:
						a = dAddrAlpha[ pBuffAl ] + dAddrAlpha[ pBuffAl+1 ] + dAddrAlpha[ pBuffAl+2 ];
						a /=3;
						break;
					}
					pBuffAl+=incByteAl;
				}
				pBuff +=incByte;

				r = (( r>>(8-ShiftPixel[0] ) )<< Shift[0] ) & Mask[0];
				g = (( g>>(8-ShiftPixel[1] ) )<< Shift[1] ) & Mask[1];
				b = (( b>>(8-ShiftPixel[2] ) )<< Shift[2] ) & Mask[2];
				a = (( a>>(8-ShiftPixel[3] ) )<< Shift[3] ) & Mask[3];

				tp = r|g|b|a;											// �����÷��� ��ȯ

				if ( MaskAll<=0xFFFF )
				{
					*((WORD *)&Dest[ k ]) = (WORD)tp;
					k+=2;
				}
				else 
				{
					*((DWORD *)&Dest[ k ]) = tp;
					k+=4;
				}
			}

			pBuff_k += (int)ddsd.lPitch;

		}
		lpDDSurface->Unlock(NULL);

Lop_Quit:

	if ( cJpeg ) 
		delete cJpeg;
	if ( lpDib ) 
		delete lpDib;
	if ( cTga ) 
		delete cTga;

	if ( cJpegAl ) 
		delete cJpegAl;
	if ( lpDibAlpha ) 
		delete lpDibAlpha;
	if ( cTgaAl ) 
		delete cTgaAl;

	return lpDDSurface;
}

//////////////// [ BMP������ �ؽ��� �����̽��� ����� ] ///////////////
DIRECTDRAWSURFACE LoadDibSurface(char *Filename, int SysMem)
{
	BYTE 				*lpDib;
	BYTE				*dAddr;
	int					height,width;
	
	lpDib = LoadDib( Filename );
	
	if ( lpDib==NULL ) 
		return NULL;

	dAddr	=GetDibBitsAddr(lpDib);
	height	=GetDibHeight(lpDib);
	width	=GetDibWidth(lpDib);

	// Now create surface
	DIRECTDRAWSURFACE lpDDSurface;
	DDSURFACEDESC2		ddsd;

	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

	ddsd.dwSize		= sizeof(DDSURFACEDESC2);
	ddsd.dwFlags	= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.dwWidth	= width;
    ddsd.dwHeight	= height;


	if (renderDevice.GetDeviceDesc_IsHardware() && SysMem == FALSE)
	{
		ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	}
	else
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_TEXTURE;
	}

	hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
    if ( hresult != DD_OK ) { delete lpDib; return NULL; }

	DrawSurfaceFromDib( lpDDSurface , lpDib );
	delete lpDib;

	// RGB( 0,0,0 ) ColorKey;
	DDCOLORKEY ColKey;
	ColKey.dwColorSpaceLowValue=RGB(0,0,0); 
    ColKey.dwColorSpaceHighValue=RGB(255,255,255);  
	lpDDSurface->SetColorKey(DDCKEY_SRCBLT  , &ColKey );

	return lpDDSurface;
}

// Dib�� �����̽��� �׸���
int DrawSurfaceFromDib(DIRECTDRAWSURFACE lpDDSurface, BYTE *lpDib)
{
	int i,j,k;
	DWORD r,g,b,a,tp;
	BYTE *Dest;

	DWORD Shift[4];
	DWORD ShiftPixel[4];
	DWORD ShiftTemp;

	DDSURFACEDESC2		ddsd;
	DDPIXELFORMAT		ddPixelFormat;

	BYTE				*dAddr;
	int					height, width;

	dAddr = GetDibBitsAddr(lpDib);
	height = GetDibHeight(lpDib);
	width = GetDibWidth(lpDib);

	//Find pixel format
	ddPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

	lpDDSurface->GetPixelFormat(&ddPixelFormat);

	DWORD Mask[4];
	Mask[0] = ddPixelFormat.dwRBitMask;
	Mask[1] = ddPixelFormat.dwGBitMask;
	Mask[2] = ddPixelFormat.dwBBitMask;
	Mask[3] = ddPixelFormat.dwRGBAlphaBitMask;

	//Utils_Log(LOG_DEBUG, "BMP:  BitMask: %d  %d  %d  %d", Mask[0], Mask[1], Mask[2], Mask[3]);
	DWORD MaskAll = Mask[0]|Mask[1]|Mask[2]|Mask[3];

	for( i=0; i<4; i++ ) 
	{
		if ( Mask[i] ) 
		{
			ShiftTemp = Mask[i];
			for( j=0;j<32;j++ )
			{
				if ( ShiftTemp&1 ) 
					break;
				ShiftTemp>>=1;
			}
			Shift[i] = j;
			for( j=0;j<32;j++) 
			{
				if ( (ShiftTemp&1)==0 ) 
					break;
				ShiftTemp>>=1;
			}
			ShiftPixel[i] = j;
		}
		else 
		{
			Shift[i] = 32;
			ShiftPixel[i] = 32;
		}
	}

	int pBuff;
	int pBuff_w;
	int pBuff_k;

	if (GetDibBitCount(lpDib)==24)
	{
		ddsd.dwSize		= sizeof(DDSURFACEDESC2);
		lpDDSurface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
		Dest = (BYTE *)ddsd.lpSurface;
		if ( !Dest ) 
		{
			lpDDSurface->Unlock(NULL);
			return FALSE;
		}

		pBuff_w = 0;
		if ( (int)ddsd.lPitch!=0 )
			pBuff_k = (int)ddsd.lPitch*(height-1);
		else 
		{
			if ( MaskAll<=0xFFFF ) 
				pBuff_k = width * (height-1) * 2;
			else
				pBuff_k = width * (height-1) * 4;
		}

		for(i=0;i<height;i++) 
		{
			k = pBuff_k;
			for(j=0;j<width;j++) 
			{
				pBuff = pBuff_w + (j*3);
				b = dAddr[ pBuff ]; 
				g = dAddr[ pBuff +1]; 
				r = dAddr[ pBuff +2];
				a = 255;

				if ( (r+g+b)>0 && r<9 && g<9 && b<9 ) 
				{
					r = g = b = 9;
				}

				r = (( r>>(8-ShiftPixel[0] ) )<< Shift[0] ) & Mask[0];
				g = (( g>>(8-ShiftPixel[1] ) )<< Shift[1] ) & Mask[1];
				b = (( b>>(8-ShiftPixel[2] ) )<< Shift[2] ) & Mask[2];
				a = (( a>>(8-ShiftPixel[3] ) )<< Shift[3] ) & Mask[3];

				tp = r|g|b|a;											// �����÷��� ��ȯ

				if ( MaskAll<=0xFFFF )
				{
					*((WORD *)&Dest[ k ]) = (WORD)tp;
					k+=2;
				}
				else 
				{
					*((DWORD *)&Dest[ k ]) = tp;
					k+=4;
				}
			}
			pBuff_w =  ((pBuff_w+width*3)+3)&0xFFFFFFFC;
			pBuff_k -= (int)ddsd.lPitch;
		}
		lpDDSurface->Unlock(NULL);
	}

	return TRUE;
}

// Jpeg�� �����̽��� �׸���
int DrawSurfaceFromJpeg(DIRECTDRAWSURFACE lpDDSurface, CJpeg *cJpeg)
{
	//######################################################################################
	//�� �� �� : �� �� ��
	int i,j,k;
	DWORD r,g,b,a,tp;		// ����Ÿ �� �ٲ�.
	BYTE *Dest;

	DWORD MaskAll;
	//######################################################################################

	DWORD Mask[4];
	DWORD Shift[4];
	DWORD ShiftPixel[4];
	DWORD ShiftTemp;

	DDSURFACEDESC2		ddsd;
	DDPIXELFORMAT		ddPixelFormat;

	BYTE				*dAddr;
	int					height,width;

	//�ȼ� ���� ���ϱ�
	ddPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	lpDDSurface->GetPixelFormat( &ddPixelFormat );

	dAddr	=cJpeg->m_pData;
	height	=cJpeg->GetHeight();
	width	=cJpeg->GetWidth();

	Mask[0] = ddPixelFormat.dwRBitMask;
	Mask[1] = ddPixelFormat.dwGBitMask;
	Mask[2] = ddPixelFormat.dwBBitMask;
	Mask[3] = ddPixelFormat.dwRGBAlphaBitMask;

	//######################################################################################
	//�� �� �� : �� �� ��
	MaskAll = Mask[0]|Mask[1]|Mask[2]|Mask[3];
	//######################################################################################

	for( i=0; i<4; i++ )
	{
		if ( Mask[i] )
		{
			ShiftTemp = Mask[i];
			for( j=0;j<16;j++ ) {
				if ( ShiftTemp&1 ) break;
				ShiftTemp>>=1;
			}
			Shift[i] = j;
			for( j=0;j<16;j++) {
				if ( (ShiftTemp&1)==0 ) break;
				ShiftTemp>>=1;
			}
			ShiftPixel[i] = j;
		}
		else {
			Shift[i] = 16;
			ShiftPixel[i] = 16;
		}
	}

	int pBuff;

	ddsd.dwSize		= sizeof(DDSURFACEDESC2);
	lpDDSurface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
	Dest = (BYTE *)ddsd.lpSurface;

	k=0;
	for(i=0;i<height;i++) 
	{
		if ( (int)ddsd.lPitch!=0 ) 			// �����̽� ��ġ ����
			k = ((int)ddsd.lPitch*i);
		for(j=0;j<width;j++)
		{		
			pBuff = (height-i-1)*width*3 + (j*3);
			b = dAddr[ pBuff ]; 
			g = dAddr[ pBuff +1]; 
			r = dAddr[ pBuff +2];
			a = 255;
					
			r = (( r>>(8-ShiftPixel[0] ) )<< Shift[0] ) & Mask[0];
			g = (( g>>(8-ShiftPixel[1] ) )<< Shift[1] ) & Mask[1];
			b = (( b>>(8-ShiftPixel[2] ) )<< Shift[2] ) & Mask[2];
			a = (( a>>(8-ShiftPixel[3] ) )<< Shift[3] ) & Mask[3];

			tp = r|g|b|a;											// �����÷��� ��ȯ

			//######################################################################################
			//�� �� �� : �� �� ��
			if( MaskAll <= 0xFFFF )
			{
				*((WORD *)&Dest[ k ]) = (WORD)tp;
				k+=2;
			}
			else
			{
				*((DWORD *)&Dest[ k ]) = tp;
				k+=4;
			}
			//######################################################################################
		}
	}
	
	lpDDSurface->Unlock(NULL);

	return TRUE;
}

// TGA�� �����̽��� �׸��� ( 24��Ʈ �� �ش� / 32��Ʈ�� ����ä�ΰ� ���� �����Ƿ� ���⼭ �׸��� �ʴ´� )
int DrawSurfaceFromTga(DIRECTDRAWSURFACE lpDDSurface, cTGA *cTga)
{
	int i,j,k;
	int r,g,b,a,tp;
	BYTE *Dest;
	int SwapHeight;

	DWORD MaskAll;

	DWORD Mask[4];
	DWORD Shift[4];
	DWORD ShiftPixel[4];
	DWORD ShiftTemp;

	DDSURFACEDESC2		ddsd;
	DDPIXELFORMAT		ddPixelFormat;

	BYTE				*dAddr;
	int					height,width;

	dAddr	=cTga->m_pData;
	height	=cTga->GetHeight();
	width	=cTga->GetWidth();
	SwapHeight = cTga->SwapHeight;

	//�ȼ� ���� ���ϱ�
	ddPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	lpDDSurface->GetPixelFormat( &ddPixelFormat );


	Mask[0] = ddPixelFormat.dwRBitMask;
	Mask[1] = ddPixelFormat.dwGBitMask;
	Mask[2] = ddPixelFormat.dwBBitMask;
	Mask[3] = ddPixelFormat.dwRGBAlphaBitMask;

	MaskAll = Mask[0]|Mask[1]|Mask[2]|Mask[3];

	for( i=0; i<4; i++ ) 
	{
		if ( Mask[i] )
		{
			ShiftTemp = Mask[i];
			for( j=0;j<16;j++ ) 
			{
				if ( ShiftTemp&1 ) 
					break;
				ShiftTemp>>=1;
			}
	
			Shift[i] = j;
			for( j=0;j<16;j++) 
			{
				if ( (ShiftTemp&1)==0 )
					break;
				ShiftTemp>>=1;
			}
			ShiftPixel[i] = j;
		}
		else
		{
			Shift[i] = 16;
			ShiftPixel[i] = 16;
		}
	}

	int pBuff;

	ddsd.dwSize		= sizeof(DDSURFACEDESC2);
	lpDDSurface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
	Dest = (BYTE *)ddsd.lpSurface;

	k=0;
	for(i=0;i<height;i++) 
	{
		if ( (int)ddsd.lPitch!=0 )			// �����̽� ��ġ ����
			k = ((int)ddsd.lPitch*i);
	
		for(j=0;j<width;j++) 
		{	
			if ( SwapHeight )
				pBuff = (height-i-1)*width+j;
			else 
				pBuff = i*width+j;
				
			pBuff *= 3;
			b = dAddr[ pBuff ]; 
			g = dAddr[ pBuff +1]; 
			r = dAddr[ pBuff +2];
			a = 255;
					
			r = (( r>>(8-ShiftPixel[0] ) )<< Shift[0] ) & Mask[0];
			g = (( g>>(8-ShiftPixel[1] ) )<< Shift[1] ) & Mask[1];
			b = (( b>>(8-ShiftPixel[2] ) )<< Shift[2] ) & Mask[2];
			a = (( a>>(8-ShiftPixel[3] ) )<< Shift[3] ) & Mask[3];

			tp = r|g|b|a;											// �����÷��� ��ȯ

			if( MaskAll <= 0xFFFF )
			{
				*((WORD *)&Dest[ k ]) = (WORD)tp;
				k+=2;
			}
			else
			{
				*((DWORD *)&Dest[ k ]) = tp;
				k+=4;
			}
		}
	}

	lpDDSurface->Unlock(NULL);

	return TRUE;
}

//�� �� �� : �� �� ��
int MipMapCreateTextureCheck( int width, int height, int MipMapCount )
{
	if( width > 256 || height > 256 )
		return 0;

	switch( MipMapCount )
	{
	case 5:
		if( width == 256 && height == 256 )
			return 1;
		break;

	case 4:
		if( width >= 128 && height >= 128 )
			return 1;
		break;

	case 3:
		if( width >= 64 && height >= 64 )
			return 1;
		break;

	case 2:
		if( width >= 32 && height >= 32 )
			return 1;
		break;

	case 1:
		return 0;

	default:
		return 0;
	}

	return 0;
}

//////////////// [ BMP������ �ؽ��� �����̽��� ����� (�ڵ� ����) ] ///////////////
int smCreateTexture( smTEXTUREHANDLE *lpTexHandle, int MipMap )
{
	MipMapCount = 0;
	::ZeroMemory( MipMapFileBuff, sizeof(MipMapFileBuff) );
	MipMapCountA = 0;
	::ZeroMemory( MipMapFileABuff, sizeof(MipMapFileABuff) );
	::ZeroMemory( FileTypeBuff, sizeof(FileTypeBuff) );

	char szMipMapFileError[256] = {"MipMap File Error : "};

	BYTE 				*lpDib;
	BYTE				*dAddr;
	int					height,width;
	int					height2,width2;
	int					TexSwap;
	int					TexHalf;


	// Now create surface
    DRZTEXTURE2 lpD3DTexture;
	DIRECTDRAWSURFACE lpDDSurface;
	DIRECTDRAWSURFACE lpDDSurface2;
	DDSURFACEDESC2		ddsd;

	char *Filename;
	CJpeg	*cJpeg = 0;
	cTGA	*cTga = 0;
	int		FileType;
	int		TgaBitCount =0;

	lpDib = 0;
	TexHalf = 0;

	Filename = lpTexHandle->Name;
	FileType = GetTextureFileType( Filename ) ;

	if (FileType < 0)
	{
#ifdef DISPLAY_TEXTURE_LOAD_ERROR
		Utils_Log(LOG_ERROR, "Cannot read Texture (Wrong FileType)\n%s", Filename);
#endif
		return -1;
	}
	switch( FileType )
	{
	case smTEXFILETYPE_TGA:
		cTga = new cTGA;

		if ( cTga->LoadTga( Filename ) )
		{
			dAddr = cTga->m_pData;
			width = cTga->GetWidth();
			height = cTga->GetHeight();
			TgaBitCount = cTga->GetBitCount();
		}
		else 
		{
#ifdef DISPLAY_TEXTURE_LOAD_ERROR
			Utils_Log(LOG_ERROR, "Cannot find Texture\n%s", Filename);
#endif
			delete cTga;
			return -1;
		}
		
		//######################################################################################
		//�� �� �� : �� �� ��
		::strcpy( FileTypeBuff, ".tga" );
		//######################################################################################
		break;

	case smTEXFILETYPE_JPG:
		cJpeg = new CJpeg;
		cJpeg->LoadJPG( Filename );
		//######################################################################################
		//�� �� �� : �� �� ��
		if( ! cJpeg->m_pData )
		{
#ifdef DISPLAY_TEXTURE_LOAD_ERROR
			Utils_Log(LOG_ERROR, "Cannot find Texture\n%s", Filename);
#endif
			delete cJpeg;
			return -1;
		}
		//######################################################################################

		dAddr = cJpeg->m_pData;
		width = cJpeg->GetWidth();
		height = cJpeg->GetHeight();

		//######################################################################################
		//�� �� �� : �� �� ��
		::strcpy( FileTypeBuff, ".jpg" );
		//######################################################################################
		break;

	case smTEXFILETYPE_BMP:
		lpDib = LoadDib( Filename );
		if (lpDib == NULL)
		{
#ifdef DISPLAY_TEXTURE_LOAD_ERROR
			Utils_Log(LOG_ERROR, "Cannot find Texture\n%s", Filename);
#endif
			return -1;
		}

		dAddr	=GetDibBitsAddr(lpDib);
		height	=GetDibHeight(lpDib);
		width	=GetDibWidth(lpDib);

		//######################################################################################
		//�� �� �� : �� �� ��
		::strcpy( FileTypeBuff, ".bmp" );
		//######################################################################################
		break;

	}

	TexSwap=FALSE;
	//######################################################################################
	// �� �� �� : �� �� ��
	// �ؽ��� ���� ��� ���̻� ���ġ ����. ����. �ؽ��� ����Ƽ 1�� �϶���. ���һ��ġ �ʰ�.
	// ���� ����� 128 �̰ų� �� �̻��� ������ ���̴� �͸��� ���Ű�� �߾��.
	
	//�ؽ��� ���� ��� �㰡�� �ؽ���
	//if ( lpTexHandle->TexSwapMode==TRUE ) {
		//�ؽ����� ũ�Ⱑ Ŭ��� SWAP���
	//	if ( width>=128 || height>=128 ) TexSwap = TRUE;
	//}
	//######################################################################################

	lpTexHandle->Width		= width;
	lpTexHandle->Height		= height;

	//######################################################################################
	//�� �� �� : �� �� ��
	if( MipMap && MipMapModeCreate && width < 512 && height < 512 )
	{
		if( MipMapModeCreate == NOSQUARE_MODE || width == height )
		{
			int i, len = ::lstrlen( Filename );
			for( i=len-1; i >= 0; i-- )
			{
				if( Filename[i] == '.' )
				{
					::CopyMemory( MipMapFileBuff[0], Filename, i );
					break;
				}
			}

			// ���� �����¹Ӹ� �ִ� 3�ܰ� ������ ��� �ϱ�� ����.
			for( i=1; i < MIPMAP_FILELIST; i++ )
			{
				::strcpy( MipMapFileBuff[i], MipMapFileBuff[0] );
				::strcat( MipMapFileBuff[i], MipMapFileListBuff[i] );
				::strcat( MipMapFileBuff[i], FileTypeBuff );

				if( PathFileExists( MipMapFileBuff[i] ) )
					MipMapCount++;
				else
					break;
			}

			::strcat( MipMapFileBuff[0], FileTypeBuff );
		}
	}
	//######################################################################################

	if ( !lpTexHandle->NameA[0] && TgaBitCount<32 )
	{
		//�����̽� ����
		ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

		//######################################################################################
		//�� �� �� : �� �� ��
		if( MipMapCount )
		{
			MipMapCount++;

			if( MipMapCreateTextureCheck(width, height, MipMapCount) <= 0 )
			{
				if ( cJpeg ) delete cJpeg;
				if ( cTga ) delete cTga;
				if ( lpDib ) delete lpDib;

				::strcat( szMipMapFileError, Filename );
				Utils_Log(LOG_ERROR, szMipMapFileError);
				return NULL;
			}

			ddsd.dwSize          = sizeof(DDSURFACEDESC2);
			ddsd.dwFlags         = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
			ddsd.dwWidth         = width;
			ddsd.dwHeight        = height;
			ddsd.dwMipMapCount   = MipMapCount;
			ddsd.ddsCaps.dwCaps  = DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_TEXTURE;
			ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
		}
		else
		{
			ddsd.dwSize	  = sizeof(DDSURFACEDESC2);
			ddsd.dwFlags  = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
			ddsd.dwWidth  = width;
			ddsd.dwHeight = height;

			if (renderDevice.GetDeviceDesc_IsHardware() && TexSwap == FALSE)
			{
				//ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_TEXTURE;
				ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
				ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
			}
			else
				ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY| DDSCAPS_TEXTURE;
		}
		//######################################################################################

		memcpy( &ddsd.ddpfPixelFormat , &ddDefaultTextureFormat, sizeof(DDPIXELFORMAT) );

		hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
		width2  = width;
		height2 = height;

		// �εο��� 256x256 �ʰ� �ؽ��� ó�� 
		// ( �ϴ� �޸𸮿� �ø��� ũ�⸦ �ٿ��� ������ �ø� )
		if ( hresult != DD_OK )
		{
			if( TexSwap==FALSE && (width>256 || height>256) )
			{
				//######################################################################################
				//�� �� �� : �� �� ��
				if (renderDevice.GetDeviceDesc_IsHardware())
				{
					ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY| DDSCAPS_TEXTURE;

					hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
					if ( hresult==DD_OK ) TexHalf = TRUE;
				}
				//######################################################################################
			}

			if( hresult != DD_OK )
			{
				if ( cJpeg ) delete cJpeg;
				if ( cTga ) delete cTga;
				if ( lpDib ) delete lpDib;
				return NULL;
			}
		}

		//######################################################################################
		//�� �� �� : �� �� ��
		if( lpDib )
		{
			DrawSurfaceFromDib( lpDDSurface, lpDib );
			delete lpDib;
		}

		if( cJpeg )
		{
            DrawSurfaceFromJpeg( lpDDSurface, cJpeg );
            delete cJpeg;
		}

		if( cTga )
		{
			DrawSurfaceFromTga( lpDDSurface, cTga );
			delete cTga;
		}

		DDSCAPS2 ddsCaps = {0,};
		ddsCaps.dwCaps   = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;


		lpDDSurface2 = lpDDSurface;
		for( int i=1; i < MipMapCount; i++ )
		{
			DIRECTDRAWSURFACE lpDDNextLevel = NULL;
			if( FAILED( lpDDSurface2->GetAttachedSurface(&ddsCaps, &lpDDNextLevel) ) )
			{
				lpDDSurface->Release();
				return NULL;
			}

			width2  >>= 1;
			height2 >>= 1;

			switch( FileType )
			{
			case smTEXFILETYPE_BMP:
				lpDib = LoadDib( MipMapFileBuff[i] );
				if( lpDib == NULL )
				{
					lpDDNextLevel->Release();
					lpDDSurface->Release();
					return NULL;
				}

				if( width2 != GetDibWidth(lpDib) || height2 != GetDibHeight(lpDib) )
				{
					lpDDNextLevel->Release();
					lpDDSurface->Release();

					::strcat( szMipMapFileError, MipMapFileBuff[i] );
					Utils_Log(LOG_ERROR, szMipMapFileError);
					return NULL;
				}

				DrawSurfaceFromDib( lpDDNextLevel, lpDib );
				delete lpDib;
				break;

			case smTEXFILETYPE_JPG:
				cJpeg = new CJpeg;
				cJpeg->LoadJPG( MipMapFileBuff[i] );
				if( cJpeg->m_pData == NULL )
				{
					delete cJpeg;
					lpDDNextLevel->Release();
					lpDDSurface->Release();
					return NULL;
				}

				if( width2 != cJpeg->GetWidth() || height2 != cJpeg->GetHeight() )
				{
					delete cJpeg;
					lpDDNextLevel->Release();
					lpDDSurface->Release();

					::strcat( szMipMapFileError, MipMapFileBuff[i] );
					Utils_Log(LOG_ERROR, szMipMapFileError);
					return NULL;
				}

				DrawSurfaceFromJpeg( lpDDNextLevel, cJpeg );
				delete cJpeg;
				break;

			case smTEXFILETYPE_TGA:
				cTga = new cTGA;
				cTga->LoadTga( MipMapFileBuff[i] );
				if( cTga->m_pBuff == NULL )
				{
					delete cTga;
					lpDDNextLevel->Release();
					lpDDSurface->Release();
					return NULL;
				}

				if( width2 != cTga->GetWidth() || height2 != cTga->GetHeight() )
				{
					delete cTga;
					lpDDNextLevel->Release();
					lpDDSurface->Release();

					::strcat( szMipMapFileError, MipMapFileBuff[i] );
					Utils_Log(LOG_ERROR, szMipMapFileError);
					return NULL;
				}

				DrawSurfaceFromTga( lpDDNextLevel, cTga );
				delete cTga;
                break;
			}

			lpDDSurface2 = lpDDNextLevel;
			lpDDNextLevel->Release();
		}

		lpDDSurface2 = NULL;
		//######################################################################################
		
		lpTexHandle->MapOpacity = FALSE;
	}
	else
	{
		//���� �ؽ���, ����ä�� �ռ�
		if ( cJpeg ) delete cJpeg;
		if ( lpDib ) delete lpDib;
		if ( cTga ) delete cTga;

		//######################################################################################
		//�� �� �� : �� �� ��
		if( MipMapCount )
		{
			int i;
			if( lpTexHandle->NameA[0] )
			{
				::ZeroMemory( FileTypeBuff, sizeof(FileTypeBuff) );

				int len = ::lstrlen( lpTexHandle->NameA );
				for( i=len-1; i >= 0; i-- )
				{
					if( lpTexHandle->NameA[i] == '.' )
					{
						::CopyMemory( MipMapFileABuff[0], lpTexHandle->NameA, i );

						if( stricmp( &lpTexHandle->NameA[i], ".bmp" ) == 0 )
							::strcpy( FileTypeBuff, ".bmp" );
						else if( stricmp( &lpTexHandle->NameA[i], ".jpg" ) == 0 )
							::strcpy( FileTypeBuff, ".jpg" );
						else if( stricmp( &lpTexHandle->NameA[i], ".tga" ) == 0 )
							::strcpy( FileTypeBuff, ".tga" );
						else
							return -1;
						break;
					}
				}

				for( i=1; i < MIPMAP_FILELIST; i++ )
				{
					::strcpy( MipMapFileABuff[i], MipMapFileABuff[0] );
					::strcat( MipMapFileABuff[i], MipMapFileListBuff[i] );
					::strcat( MipMapFileABuff[i], FileTypeBuff );

					if( PathFileExists( MipMapFileABuff[i] ) )
						MipMapCountA++;
					else
						break;
				}

				::strcat( MipMapFileABuff[0], FileTypeBuff );

				//
				if( MipMapCount != MipMapCountA )
					return -1;

				MipMapCountA++;
			}

			MipMapCount++;


			//######################################################################################
			// ����................................................................................
			// ���� ó���߿���. lpTexHandle->NameA ���� ��� ���� ���� ���� ������ �ʾҽ��ϴ�.
			// ���� ���� ���Ľ�Ƽ�� ���� ���İ��� ���� ���. ���� �ؽ��ĸ� �� ����� �߽��ϴ�.
			// �ƽ����� ���İ��� ���� ���� ���ؼ���.
			// ���� ���Ľ�Ƽ�� ���� �ִٰ� �ص�. ��������. �⺻ ������. tga 32bit �� ���.
			// ���Ľ�Ƽ ���İ��� ���� �ϵ��� �Ǿ� �ֱ⿡.
			if( MipMapCreateTextureCheck(width, height, MipMapCount) <= 0 )
			{
				::strcat( szMipMapFileError, Filename );
				Utils_Log(LOG_ERROR, szMipMapFileError);
				return -2;
			}
			//######################################################################################

			ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
			ddsd.dwSize          = sizeof(DDSURFACEDESC2);
			ddsd.dwFlags         = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
			ddsd.dwWidth         = width;
			ddsd.dwHeight        = height;
			ddsd.dwMipMapCount   = MipMapCount;
			ddsd.ddsCaps.dwCaps  = DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_TEXTURE;
			ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
			memcpy( &ddsd.ddpfPixelFormat, &ddTextureFormat, sizeof(DDPIXELFORMAT) );
			
			hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
			if( hresult != DD_OK )
				return -2;

			width2  = width;
			height2 = height;
			if( LoadDibSurfaceAlphaMipMap( lpDDSurface, MipMapFileBuff[0], MipMapFileABuff[0], width2, height2 ) == FALSE )
			{
				lpDDSurface->Release();
				return -2;
			}

			DDSCAPS2 ddsCaps = {0,};
			ddsCaps.dwCaps   = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

			lpDDSurface2 = lpDDSurface;
			for( i=1; i < MipMapCount; i++ )
			{
				DIRECTDRAWSURFACE lpDDNextLevel = NULL;
				if( FAILED( lpDDSurface2->GetAttachedSurface(&ddsCaps, &lpDDNextLevel) ) )
				{
					lpDDSurface->Release();
					return -2;
				}

				width2  >>= 1;
				height2 >>= 1;

				int ErrorAlphaMipMap = LoadDibSurfaceAlphaMipMap( lpDDNextLevel, MipMapFileBuff[i], MipMapFileABuff[i], width2, height2 );
				if( ErrorAlphaMipMap == -1 )
				{
					lpDDNextLevel->Release();
					lpDDSurface->Release();

					::strcat( szMipMapFileError, MipMapFileBuff[i] );
					Utils_Log(LOG_ERROR, szMipMapFileError);
					return -2;
				}
				else if( ErrorAlphaMipMap == -2 )
				{
					lpDDNextLevel->Release();
					lpDDSurface->Release();

					::strcat( szMipMapFileError, MipMapFileABuff[i] );
					Utils_Log(LOG_ERROR, szMipMapFileError);
					return -2;
				}

				lpDDSurface2 = lpDDNextLevel;
				lpDDNextLevel->Release();
			}

			if( ! lpTexHandle->NameA[0] )
				lpTexHandle->NameA[0] = TRUE;

			lpDDSurface2 = NULL;
		}
		else
		{
			lpDDSurface = LoadDibSurfaceAlpha( Filename , lpTexHandle->NameA , TexSwap );
			if( ! lpTexHandle->NameA[0] ) lpTexHandle->NameA[0] = TRUE;
			if( lpDDSurface == NULL )
				return -2;
		}
		//######################################################################################

		lpTexHandle->MapOpacity = TRUE;
	}

	//���� ���� �޸𸮿� ���� ������ �����̽� ����
	if ( TexSwap==TRUE || TexHalf || ( TexturHalfSize && width>32 && height>32 && lpTexHandle->TexSwapMode ) )
	{
		if( TexturHalfSize == 2 ) {
			width2 = width/4;
			height2 = height/4;
		}
		else {
			width2 = width/2;
			height2 = height/2;
		}

		ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize		= sizeof(DDSURFACEDESC2);
		ddsd.dwWidth	= width2;
	    ddsd.dwHeight	= height2;
		ddsd.dwFlags	= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;

		//���� ä���� ������ �ȼ� ������ ���������
		if ( lpTexHandle->MapOpacity ) {
			memcpy( &ddsd.ddpfPixelFormat, &ddTextureFormat, sizeof(DDPIXELFORMAT) );
		}
		else {
			memcpy( &ddsd.ddpfPixelFormat , &ddDefaultTextureFormat, sizeof(DDPIXELFORMAT) );
		}

		//######################################################################################
		//�� �� �� : �� �� ��
		if (renderDevice.GetDeviceDesc_IsHardware())
		{
			//ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_TEXTURE;
			ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
			ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
		}
	    else
	        ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY| DDSCAPS_TEXTURE;
		//######################################################################################

		hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface2, NULL);
		if ( hresult != DD_OK ) { 
			lpDDSurface->Release();
			return NULL; 
		}

		//######################################################################################
		//�� �� �� : �� �� ��

		//�����̽� ��� ����
		RECT srcRect, destRect;

		srcRect.top = 0;
		srcRect.left = 0;
		srcRect.right = width;
		srcRect.bottom = height;

		destRect.top = 0;
		destRect.left = 0;
		destRect.right = width2;
		destRect.bottom = height2;

		//lpDDSurface2->Blt( &destRect, lpDDSurface, &srcRect, DDBLT_WAIT, NULL );
		renderDevice.Blt(lpDDSurface2, &destRect, lpDDSurface, &srcRect, DDBLT_WAIT, NULL);

		/*
		int cntW , cntH , DestCnt , SrcCnt;
		WORD *CopySrc , *CopyDest;

		ZeroMemory( &ddsd2, sizeof(DDSURFACEDESC2) );
		ddsd2.dwSize		= sizeof(DDSURFACEDESC2);

		lpDDSurface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
		CopySrc = (WORD *)ddsd.lpSurface;
		lpDDSurface2->Lock(NULL,&ddsd2,DDLOCK_WAIT,NULL);
		CopyDest = (WORD *)ddsd2.lpSurface;

		if ( CopySrc && CopyDest ) {
			if( TexturHalfSize==2 ) {
				// 1/4 ũ�� �ؽ��� ����
				for( cntH=0; cntH<height ; cntH+=4 ) {
					DestCnt = 0;
					SrcCnt = 0;
					for( cntW=0; cntW<width ; cntW+=4 ) {
						CopyDest[DestCnt] = CopySrc[SrcCnt];
						DestCnt++;
						SrcCnt +=4;
					}
					CopySrc += ddsd.lPitch*2;
					CopyDest += ddsd2.lPitch/2;
				}
			}
			else {
				// 1/2 ũ�� �ؽ��� ����
				for( cntH=0; cntH<height ; cntH+=2 ) {
					DestCnt = 0;
					SrcCnt = 0;
					for( cntW=0; cntW<width ; cntW+=2 ) {
						CopyDest[DestCnt] = CopySrc[SrcCnt];
						DestCnt++;
						SrcCnt +=2;
					}
					CopySrc += ddsd.lPitch;
					CopyDest += ddsd2.lPitch/2;
				}
			}
		}
		lpDDSurface2->Unlock(NULL);
		lpDDSurface->Unlock(NULL);
		*/
		//######################################################################################


		//######################################################################################
		//�� �� �� : �� �� ��
		
		//if ( TexturHalfSize || TexHalf ) {
			//���� ����� ��� �ý��� �޸� �ؽ��� ����
		//	lpDDSurface->Release();
		//}
		//else {
			//�ý��� �޸� ���� ���ɼ���
		//	lpTexHandle->lpDDSSysMemory = lpDDSurface;
		//}

		lpTexHandle->lpDDSurface = lpDDSurface2;
		lpTexHandle->lpDDSSysMemory = NULL;

		if( lpDDSurface2 != NULL )
		{
			hresult = lpDDSurface2->QueryInterface( IID_IDirect3DTexture2, (LPVOID*)&lpD3DTexture );
			if( hresult == D3D_OK )
				lpTexHandle->lpD3DTexture = lpD3DTexture;
			else
			{
				lpDDSurface2->Release();
				lpTexHandle->lpDDSurface = NULL;
				return 0;
			}
		}
		//######################################################################################

		//���� �޸� ��� ����ġ
		VramTotal += width2 * height2 * 2 ;

		return TRUE;
	}

	lpTexHandle->lpDDSurface = lpDDSurface;
	lpTexHandle->lpDDSSysMemory = NULL;
	if ( lpDDSurface!=NULL ) 
	{
		hresult = lpDDSurface->QueryInterface( IID_IDirect3DTexture2, (LPVOID*)&lpD3DTexture );
		if( hresult == D3D_OK )
			lpTexHandle->lpD3DTexture = lpD3DTexture;
		else
		{
			lpDDSurface->Release();
			lpTexHandle->lpDDSurface = NULL;
			return 0;
		}
	}

	//���� �޸� ��� ����ġ
	VramTotal += width * height * 2 ;

	return TRUE;
}

/////////////////[ BMP ������ OFFSCREEN �����̽��� ����� //////////////
DIRECTDRAWSURFACE LoadDibSurfaceOffscreen(char *Filename)
{
	//Lead to new Surface load method...
	return new_LoadBmpSurfaceOffscreen( Filename );
}


extern HWND hwnd;


DIRECTDRAWSURFACE	lpDDSMapTexture[256];
DIRECTDRAWSURFACE	lpDDSMapTextureVideo[8 * 8];
DRZTEXTURE2		lpD3DMapTexture[8*8];

int MapTexCode[8][8];

int MapTextureSize;

int LoadDibMapTexture( char *Filename, int tSize ) 
{
	BYTE 				*lpDib;
	BYTE				*dAddr;
	int					height,width;

	
	lpDib = LoadDib( Filename );
	if ( lpDib==NULL ) return NULL;

	dAddr	=GetDibBitsAddr(lpDib);
	height	=GetDibHeight(lpDib);
	width	=GetDibWidth(lpDib);

	MapTextureSize = tSize;

	int cnt;

	// Now create surface
	DDSURFACEDESC2		ddsd;

	for( cnt=0;cnt<64; cnt++) 
	{
		ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

		ddsd.dwSize		= sizeof(DDSURFACEDESC2);
		ddsd.dwFlags	= DDSD_CAPS |
						  DDSD_HEIGHT |
						  DDSD_WIDTH  ;
		ddsd.dwWidth	= tSize;
	    ddsd.dwHeight	= tSize;

		//######################################################################################
		//�� �� �� : �� �� ��
		if (renderDevice.GetDeviceDesc_IsHardware())
		{
	        //ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_TEXTURE;
			ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
			ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
		}
	    else
	        ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY| DDSCAPS_TEXTURE;
		//######################################################################################

		hresult = renderDevice.CreateSurface(&ddsd, &lpDDSMapTextureVideo[cnt], NULL);
		if ( hresult != DD_OK ) { delete lpDib; return NULL;	}
	}

	for( cnt=0;cnt<256; cnt++) {
		ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

		ddsd.dwSize		= sizeof(DDSURFACEDESC2);
		ddsd.dwFlags	= DDSD_CAPS |
						  DDSD_HEIGHT |
						  DDSD_WIDTH  ;
		ddsd.dwWidth	= tSize;
	    ddsd.dwHeight	= tSize;

        ddsd.ddsCaps.dwCaps =  DDSCAPS_SYSTEMMEMORY|DDSCAPS_TEXTURE;

		hresult = renderDevice.CreateSurface(&ddsd, &lpDDSMapTexture[cnt], NULL);
		if ( hresult != DD_OK ) { delete lpDib; return NULL;	}
	}


	HBITMAP	TempBitmap;
	HDC		TempDC;
	HDC		BitmapDC;
	HDC		hdc;
	int		cntX,cntY,cntH;
	RECT	ddRect;


	TempDC = GetDC( hwnd );
	TempBitmap = CreateCompatibleBitmap( TempDC, width, height );
	BitmapDC = CreateCompatibleDC(TempDC);
	SelectObject( BitmapDC, TempBitmap );
	SetDIBitsToDevice( BitmapDC , 0,0,width,height,0,0, 0,height, (LPSTR) dAddr, (LPBITMAPINFO )lpDib , DIB_RGB_COLORS );

	cnt = 0;
	for( cntY=0;cntY<16;cntY++)
	{
		cntH = 15-cntY;
		for( cntX=0;cntX<16;cntX++) 
		{

			lpDDSMapTexture[cnt]->GetDC( &hdc );

			BitBlt( hdc , 1,1, tSize-2, tSize-2 , BitmapDC, cntX*(tSize-2) , cntH*(tSize-2) , SRCCOPY );

			BitBlt( hdc , 0,0,       tSize, 1 , BitmapDC, cntX*(tSize-2) , cntH*(tSize-2) , SRCCOPY );
			BitBlt( hdc , 0,tSize-1, tSize, 1 , BitmapDC, cntX*(tSize-2) , cntH*(tSize-2)+(tSize-1) , SRCCOPY );

			BitBlt( hdc , 0,0,       1, tSize , BitmapDC, cntX*(tSize-2) , cntH*(tSize-2) , SRCCOPY );
			BitBlt( hdc , tSize-1, 0, 1, tSize , BitmapDC, cntX*(tSize-2)+(tSize-1) , cntH*(tSize-2), SRCCOPY );

			lpDDSMapTexture[cnt]->ReleaseDC( hdc );
			cnt++;
		}
	}

	DeleteDC( BitmapDC );
	DeleteObject( TempBitmap );
	ReleaseDC( hwnd, TempDC );

	delete lpDib;


	for( cnt=0;cnt<64;cnt++) 
	{
	    hresult = lpDDSMapTextureVideo[cnt]->QueryInterface( IID_IDirect3DTexture2, (LPVOID*)&lpD3DMapTexture[cnt] );
		if ( hresult != D3D_OK )
		{
			lpDDSMapTextureVideo[cnt]->Release();
			lpDDSMapTextureVideo[cnt] = 0;
			lpD3DMapTexture[cnt] = 0;
			return NULL;
		}
	}



	for(cntY=0;cntY<8;cntY++) 
	{
		for(cntX=0;cntX<8;cntX++) 
		{
			ddRect.left   = 0;
			ddRect.right  = tSize;
			ddRect.top    = 0;
			ddRect.bottom = tSize;

			//lpDDSMapTextureVideo[cntX + (cntY * 8)]->BltFast(0, 0, lpDDSMapTexture[cntX + (cntY * 16)], &ddRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
			renderDevice.BltFast(lpDDSMapTextureVideo[cntX + (cntY * 8)], 0, 0, lpDDSMapTexture[cntX + (cntY * 16)], &ddRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);


			MapTexCode[cntX][cntY] = cntX+(cntY*16);

		}
	}

	return TRUE;
}

int RemoveMapTexture()
{
	int cnt;

	for(cnt=63;cnt>=0;cnt--) {
		lpD3DMapTexture[ cnt ]->Release();
		lpDDSMapTextureVideo[ cnt ]->Release();
	}

	for(cnt=255;cnt>=0;cnt--) {
		lpDDSMapTexture[ cnt ]->Release();
	}

	return TRUE;
}

int SetMapTextureMem( int x, int y )
{

	RECT ddRect;
	int mx,my;
	int vx,vy;
	int ddval;
	int sc,dc;

	ddRect.left   = 0;
	ddRect.right  = MapTextureSize;
	ddRect.top    = 0;
	ddRect.bottom = MapTextureSize;

	mx = x&15;
	my = y&15;

	vx = x&7;
	vy = y&7;

	dc = vx+(vy<<3);
	sc = mx+(my<<4);

	if ( MapTexCode[vx][vy]!=sc )
	{
		//ddval = lpDDSMapTextureVideo[dc]->BltFast(0,0,lpDDSMapTexture[sc],&ddRect,DDBLTFAST_WAIT|DDBLTFAST_NOCOLORKEY);
		ddval = renderDevice.BltFast(lpDDSMapTextureVideo[dc], 0, 0, lpDDSMapTexture[sc], &ddRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
		MapTexCode[ vx ][ vy ] = sc;
	}

	return ddval;
}

int SetMapTextureWidth( int x, int y )
{
	int cnt;

	for( cnt=0;cnt<8;cnt++ ) {
		SetMapTextureMem( x+cnt, y );
	}

	return TRUE;
}

int SetMapTextureHeight( int x, int y )
{
	int cnt;

	for( cnt=0;cnt<8;cnt++ ) {
		SetMapTextureMem( x, y+cnt );
	}


	return TRUE;
}

int SetMapImage( int x , int y )
{
	int sx,sy;
	int cntX,cntY;

	sx = x>>(ShiftMAPCELL_DIV+3);
	sy = y>>(ShiftMAPCELL_DIV+3);


	for( cntX=sx-3;cntX<sx+4;cntX++) {
		for( cntY=sy-3;cntY<sy+4;cntY++) {
			SetMapTextureMem( cntX, cntY );
		}
	}


/*
	SetMapTextureWidth( sx-3 , sy+3 );
	SetMapTextureWidth( sx-3 , sy-3 );

	SetMapTextureWidth( sx+3 , sy-3 );
	SetMapTextureWidth( sx-3 , sy-3 );
*/
	return TRUE;

}

DIRECTDRAWSURFACE LoadDibBGTexture(char *Filename)
{
	BYTE 				*lpDib;
	BYTE				*dAddr;
	int					height,width;
	DIRECTDRAWSURFACE	lpDDSurface;

	
	lpDib = LoadDib( Filename );
	if ( lpDib==NULL ) return NULL;

	dAddr	=GetDibBitsAddr(lpDib);
	height	=GetDibHeight(lpDib);
	width	=GetDibWidth(lpDib);


	// Now create surface
	DDSURFACEDESC2		ddsd;

	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

	ddsd.dwSize		= sizeof(DDSURFACEDESC2);
	ddsd.dwFlags	= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsd.dwWidth	= width+2;
    ddsd.dwHeight	= height;

	memcpy( &ddsd.ddpfPixelFormat, &ddDefaultTextureFormat, sizeof(DDPIXELFORMAT) );

	ddsd.ddsCaps.dwCaps =  DDSCAPS_SYSTEMMEMORY|DDSCAPS_TEXTURE;

	hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
	if ( hresult != DD_OK ) 
		delete lpDib; return NULL;

	HBITMAP	TempBitmap;
	HDC		TempDC;
	HDC		BitmapDC;
	HDC		hdc;


	TempDC = GetDC( hwnd );
	TempBitmap = CreateCompatibleBitmap( TempDC, width, height );
	BitmapDC = CreateCompatibleDC(TempDC);
	SelectObject( BitmapDC, TempBitmap );
	SetDIBitsToDevice( BitmapDC , 0,0,width,height,0,0, 0,height, (LPSTR) dAddr, (LPBITMAPINFO )lpDib , DIB_RGB_COLORS );

	lpDDSurface->GetDC( &hdc );

	BitBlt( hdc , 1,0, width , height , BitmapDC, 0 , 0 , SRCCOPY );

	BitBlt( hdc , 0,0, 1, height , BitmapDC, 0 , 0 , SRCCOPY );
	BitBlt( hdc , width+1 , 0, 1, height , BitmapDC, width-1, 0 , SRCCOPY );

	lpDDSurface->ReleaseDC( hdc );

	DeleteDC( BitmapDC );
	DeleteObject( TempBitmap );
	ReleaseDC( hwnd, TempDC );

	delete lpDib;

	return lpDDSurface;
}

char *BGTextureName[32] = {
	"clo00.bmp",
	"clo01.bmp",
	"clo02.bmp",
	"clo03.bmp",
	"clo04.bmp",
	"clo05.bmp",
	"clo06.bmp",
	"clo07.bmp",
	"clo08.bmp",
	"clo09.bmp",
	"clo10.bmp",
	"clo11.bmp",
	"clo12.bmp",
	"clo13.bmp",
	"clo14.bmp",
	"clo15.bmp",
	"clo16.bmp",
	"clo17.bmp",
	"clo18.bmp",
	"clo19.bmp",
	"clo20.bmp",
	"clo21.bmp",
	"clo22.bmp",
	"clo23.bmp",
	"clo24.bmp",
	"clo25.bmp",
	"clo26.bmp",
	"clo27.bmp",
	"clo28.bmp",
	"clo29.bmp",
	"clo30.bmp",
	"clo31.bmp"
};


DIRECTDRAWSURFACE	lpDDSAnimBG[32];

int DrawAnimBG(DIRECTDRAWSURFACE lpDDSSrc, int frame)
{
	RECT ddRect;

	ddRect.left   = 0;
	ddRect.right  = 256;
	ddRect.top    = 0;
	ddRect.bottom = 128;

	return renderDevice.BltFast(lpDDSSrc, 0, 90, lpDDSAnimBG[frame & 0x1F], &ddRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
}

int LoadAnimBG()
{
	char strName[64];

	for (int cnt = 0; cnt<32; cnt++) 
	{
		lstrcpy( strName , "cloud\\" );
		lstrcat( strName , BGTextureName[cnt] );
		lpDDSAnimBG[cnt] = LoadDibBGTexture( strName  );
	}

	return TRUE;
}

int ClearBG(DIRECTDRAWSURFACE lpDDSSrc, DWORD color)
{
    // Surface �����
    DDBLTFX bltFx;
    ZeroMemory( &bltFx, sizeof(DDBLTFX) );
    bltFx.dwSize        = sizeof(DDBLTFX);
    bltFx.dwFillColor   = color;

	renderDevice.Blt(lpDDSSrc, NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &bltFx);

	return TRUE;
}

int CloseAnimBG()
{
	for (int cnt = 31; cnt >= 0; cnt--)
	{
		if ( lpDDSAnimBG[cnt] ) 
			lpDDSAnimBG[cnt]->Release();
	}

	return TRUE;
}

int new_smCreateTexture( smTEXTUREHANDLE *lpTexHandle, int MipMap )
{
	//
	MipMapCount = 0;
	::ZeroMemory( MipMapFileBuff, sizeof(MipMapFileBuff) );
	MipMapCountA = 0;
	::ZeroMemory( MipMapFileABuff, sizeof(MipMapFileABuff) );
	::ZeroMemory( FileTypeBuff, sizeof(FileTypeBuff) );

	char *Filename = lpTexHandle->Name;
	int	  FileType = GetTextureFileType( Filename );
	
	if (FileType < 0)
	{
#ifdef DISPLAY_TEXTURE_LOAD_ERROR
		Utils_Log(LOG_ERROR, "Cannot read Texture (Wrong FileType)\n%s", Filename);
#endif
		return -1;
	}

	//Utils_Log(LOG_DEBUG, "CreateTexture: %s", Filename);

	//
	int					TexHalf = 0;
	DIRECTDRAWSURFACE	lpDDSurface = NULL;

	if( FileType == smTEXFILETYPE_BMP )
	{
		lpDDSurface = new_LoadBmpCreateTexSurface( Filename, MipMap, TexHalf );
		if( lpDDSurface )
			lpTexHandle->MapOpacity = FALSE;
		else
		{
#ifdef DISPLAY_TEXTURE_LOAD_ERROR
			Utils_Log(LOG_ERROR, "Cannot find Texture\n%s", Filename);
#endif
			return -1;
		}
	}
	else if( FileType == smTEXFILETYPE_TGA )
	{
		lpDDSurface = new_LoadTgaCreateTexSurface( Filename, MipMap, TexHalf );
		if( lpDDSurface )
		{
			lpTexHandle->MapOpacity = TRUE;
			if( ! lpTexHandle->NameA[0] )
                lpTexHandle->NameA[0] = TRUE;
		}
		else
		{
#ifdef DISPLAY_TEXTURE_LOAD_ERROR
			Utils_Log(LOG_ERROR, "Cannot find Texture\n%s", Filename);
#endif
			return -1;
		}
	}
	else
	{
#ifdef DISPLAY_TEXTURE_LOAD_ERROR
		Utils_Log(LOG_ERROR, "Cannot read Texture. (Wrong FileExtension)\n%s", Filename);
#endif
		return -1;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//
	DRZTEXTURE2 lpD3DTexture = NULL;

	// ���� ������ �ؽ��� ����
	if( TexHalf || (TexturHalfSize && g_DibLastReadWidth > 32 && g_DibLastReadHeight > 32 && lpTexHandle->TexSwapMode) )
	{
		int width2, height2;

		if( TexturHalfSize == 2 )
		{
			width2  = g_DibLastReadWidth  >> 2;
			height2 = g_DibLastReadHeight >> 2;
		}
		else
		{
			width2  = g_DibLastReadWidth  >> 1;
			height2 = g_DibLastReadHeight >> 1;
		}

		DDSURFACEDESC2 ddsd;
		ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

		ddsd.dwSize		= sizeof(DDSURFACEDESC2);
		ddsd.dwFlags	= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		ddsd.dwWidth	= width2;
		ddsd.dwHeight	= height2;	

		if( FileType == smTEXFILETYPE_BMP )
			memcpy( &ddsd.ddpfPixelFormat , &ddDefaultTextureFormat, sizeof(DDPIXELFORMAT) );
		else
			memcpy( &ddsd.ddpfPixelFormat , &ddTextureFormat, sizeof(DDPIXELFORMAT) );

		if (renderDevice.GetDeviceDesc_IsHardware())
		{
			ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
			ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
		}
		else
			ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_TEXTURE;

		DIRECTDRAWSURFACE lpDDSurface2;
		hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface2, NULL);
		if ( hresult != DD_OK )
		{ 
			lpDDSurface->Release();
			lpTexHandle->lpDDSurface = NULL;
			return 0;
		}

		RECT srcRect, destRect;

		srcRect.top    = 0;
		srcRect.left   = 0;
		srcRect.right  = g_DibLastReadWidth;
		srcRect.bottom = g_DibLastReadHeight;

		destRect.top    = 0;
		destRect.left   = 0;
		destRect.right  = width2;
		destRect.bottom = height2;

		renderDevice.Blt(lpDDSurface2, &destRect, lpDDSurface, &srcRect, DDBLT_WAIT, NULL);
		lpDDSurface->Release();

		g_DibLastReadWidth  = width2;
		g_DibLastReadHeight = height2;

		lpTexHandle->Width			= width2;
		lpTexHandle->Height			= height2;
		lpTexHandle->lpDDSurface    = lpDDSurface2;
		lpTexHandle->lpDDSSysMemory = NULL;

		hresult = lpDDSurface2->QueryInterface( IID_IDirect3DTexture2, (LPVOID*)&lpD3DTexture );
		if( hresult == D3D_OK )
			lpTexHandle->lpD3DTexture = lpD3DTexture;
		else
		{
			lpDDSurface2->Release();
			lpTexHandle->lpDDSurface = NULL;
			return 0;
		}

		//���� �޸� ��� ����ġ
		VramTotal += width2 * height2 * 2;
	}
	else
	{
		lpTexHandle->Width			= g_DibLastReadWidth;
		lpTexHandle->Height			= g_DibLastReadHeight;
		lpTexHandle->lpDDSurface    = lpDDSurface;
		lpTexHandle->lpDDSSysMemory = NULL;

		hresult = lpDDSurface->QueryInterface( IID_IDirect3DTexture2, (LPVOID*)&lpD3DTexture );
		if( hresult == D3D_OK )
			lpTexHandle->lpD3DTexture = lpD3DTexture;
		else
		{
			lpDDSurface->Release();
			lpTexHandle->lpDDSurface = NULL;
			return 0;
		}

		//���� �޸� ��� ����ġ
		VramTotal += g_DibLastReadWidth * g_DibLastReadHeight * 2;
	}

	return TRUE;
}

int new_LoadDib( char *Filename )
{
	HANDLE				hFile;
	DWORD				dwAcess;
	BITMAPFILEHEADER	bmfh;

	//������ ����
	hFile = CreateFile( Filename , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return NULL;

	//��Ʈ�� ��� ����
	ReadFile( hFile , &bmfh , sizeof( BITMAPFILEHEADER ) , &dwAcess , NULL );
	if( bmfh.bfType != *(WORD *)"BM" && bmfh.bfType != *(WORD *)"C8" && bmfh.bfType != *(WORD *)"A8" ) 
	{
		CloseHandle( hFile );
		return NULL;
	}

	//��� ��ȣȭ ����
	if( bmfh.bfType == *(WORD *)"A8" )
	{
		BYTE	ch;
		BYTE	*lpHeader;
		int		cnt;

		//����� ��ȣȭ
		lpHeader = (BYTE *)&bmfh;
		for( cnt=0;cnt<sizeof(bmfh);cnt++)
		{
			ch = (BYTE)( lpHeader[cnt]-(cnt*cnt) );
			lpHeader[cnt] = ch;
		}
	}

	DWORD dwDibSize = bmfh.bfSize - sizeof(BITMAPFILEHEADER);
	BYTE *lpDib     = g_DibImageDateBuff;
	
	//��Ʈ�� ����
	ReadFile( hFile , lpDib , dwDibSize , &dwAcess , NULL );
	CloseHandle( hFile );

	DWORD dwHeaderSize = GetDibInfoHeaderSize(lpDib);
	if( dwHeaderSize < 12 || (dwHeaderSize>12 && dwHeaderSize < 16) ) 
		return NULL;

	return TRUE;
}

int new_LoadTga( char *Filename )
{
    BYTE	TGAHeader[0x12];
	HANDLE	hFile;
	DWORD	dwAcess;

	//������ ����
	hFile = CreateFile( Filename , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return NULL;

	//TGA ��� ����
	ReadFile( hFile , TGAHeader , 0x12 , &dwAcess , NULL );

	//��� ��ȣȭ ����
	if ( TGAHeader[0]=='G' && TGAHeader[1]=='8' ) 
	{
		BYTE	ch;
		BYTE	*lpHeader;
		int		cnt;

		//����� ��ȣȭ
		lpHeader = (BYTE *)&TGAHeader;
		for( cnt=0;cnt<sizeof(TGAHeader);cnt++) 
		{
			ch = (BYTE)( lpHeader[cnt]-(cnt*cnt) );
			lpHeader[cnt] = ch;
		}
	}

	g_DibLastReadWidth  = *((WORD *)(&TGAHeader[0xC]));
	g_DibLastReadHeight = *((WORD *)(&TGAHeader[0xE]));

	if ( (TGAHeader[0x11]&0x20) )
		g_TgaLastSwapHeight = FALSE;
	else
		g_TgaLastSwapHeight = TRUE;

	switch( TGAHeader[0x10] )
	{
	//case 16:
	case 24:
		g_TgaLastBitCnt = 24;
		ReadFile( hFile, g_DibImageDateBuff, g_DibLastReadWidth*g_DibLastReadHeight*3, &dwAcess, NULL );
		break;

	case 32:
		g_TgaLastBitCnt = 32;
		ReadFile( hFile, g_DibImageDateBuff, g_DibLastReadWidth*g_DibLastReadHeight*4, &dwAcess, NULL );
		break;

	default:
		CloseHandle( hFile );
		return NULL;
	}

	CloseHandle( hFile );
	return TRUE;
}

int New_DrawSurfaceFromTga(DIRECTDRAWSURFACE lpDDSurface)
{
	BYTE *dAddr  = g_DibImageDateBuff;
	int   width  = g_DibLastReadWidth;
	int   height = g_DibLastReadHeight;

	int i,j,k;
	int r,g,b,a,tp;	
	DWORD MaskAll;

	DWORD Mask[4];
	DWORD Shift[4];
	DWORD ShiftPixel[4];
	DWORD ShiftTemp;

	Mask[0] = ddTextureFormat.dwRBitMask;
	Mask[1] = ddTextureFormat.dwGBitMask;
	Mask[2] = ddTextureFormat.dwBBitMask;
	Mask[3] = ddTextureFormat.dwRGBAlphaBitMask;

	//Utils_Log(LOG_DEBUG, "TGA:  BitMask: %u  %u  %u  %u", Mask[0], Mask[1], Mask[2], Mask[3]);

	MaskAll = Mask[0]|Mask[1]|Mask[2]|Mask[3];

	for( i=0; i<4; i++ )
	{
		if ( Mask[i] )
		{
			ShiftTemp = Mask[i];
			for( j=0;j<32;j++ )
			{
				if ( ShiftTemp&1 ) 
					break;
				ShiftTemp>>=1;
			}
			Shift[i] = j;
			for( j=0;j<32;j++)
			{
				if ( (ShiftTemp&1)==0 )
					break;
				ShiftTemp>>=1;
			}
			ShiftPixel[i] = j;
		}
		else
		{
			Shift[i] = 32;
			ShiftPixel[i] = 32;
		}
	}

	BYTE *Dest;
	int   pBuff;
	int   incByte;
	int   pBuff_k;

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));

	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	lpDDSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);

	Dest = (BYTE *)ddsd.lpSurface;
	if (Dest == NULL)
	{
		lpDDSurface->Release();
		return NULL;
	}

	pBuff_k = 0;
	k = 0;
	for (i = 0; i < height; i++)
	{
		switch (g_TgaLastBitCnt)
		{
		case 24:
			incByte = 3;
			if (g_TgaLastSwapHeight)		pBuff = ((height - i - 1)*width) * 3;
			else							pBuff = (i*width) * 3;
			break;

		case 32:
			incByte = 4;
			if (g_TgaLastSwapHeight)		pBuff = ((height - i - 1)*width) << 2;
			else							pBuff = (i*width) << 2;
			break;
		}

		k = pBuff_k;
		for (j = 0; j < width; j++)
		{
			b = dAddr[pBuff];
			g = dAddr[pBuff + 1];
			r = dAddr[pBuff + 2];

			if (g_TgaLastBitCnt == 32)
				a = dAddr[pBuff + 3];
			else
				a = 255;

			pBuff += incByte;

			r = ((r >> (8 - ShiftPixel[0])) << Shift[0]) & Mask[0];
			g = ((g >> (8 - ShiftPixel[1])) << Shift[1]) & Mask[1];
			b = ((b >> (8 - ShiftPixel[2])) << Shift[2]) & Mask[2];
			a = ((a >> (8 - ShiftPixel[3])) << Shift[3]) & Mask[3];

			tp = r | g | b | a;											//Conversion to High Color

			if (MaskAll <= 0xFFFF)
			{
				*((WORD *)&Dest[k]) = (WORD)tp;
				k += 2;
			}
			else
			{
				*((DWORD *)&Dest[k]) = tp;
				k += 4;
			}
		}

		pBuff_k += (int)ddsd.lPitch;

	}

	lpDDSurface->Unlock(NULL);

	return TRUE;
}

DIRECTDRAWSURFACE new_LoadBmpCreateTexSurface(char *Filename, int MipMap, int &rTexHalf)
{
	if( new_LoadDib(Filename) == NULL )
		return NULL;

	//
	BYTE *lpDib = g_DibImageDateBuff;
	g_DibLastReadWidth  = GetDibWidth(lpDib);
	g_DibLastReadHeight = GetDibHeight(lpDib);

	if( MipMap && MipMapModeCreate && g_DibLastReadWidth < 512 && g_DibLastReadHeight < 512 )
	{
		if( MipMapModeCreate == NOSQUARE_MODE || g_DibLastReadWidth == g_DibLastReadHeight )
		{
			int i, len = ::lstrlen( Filename );
			for( i=len-1; i >= 0; i-- )
			{
				if( Filename[i] == '.' )
				{
					::CopyMemory( MipMapFileBuff[0], Filename, i );
					break;
				}
			}

			// ���� �����¹Ӹ� �ִ� 3�ܰ� ������ ��� �ϱ�� ����.
			for( i=1; i < MIPMAP_FILELIST; i++ )
			{
				::strcpy( MipMapFileBuff[i], MipMapFileBuff[0] );
				::strcat( MipMapFileBuff[i], MipMapFileListBuff[i] );
				::strcat( MipMapFileBuff[i], ".bmp" );

				if( PathFileExists( MipMapFileBuff[i] ) )
					MipMapCount++;
				else
					break;
			}

			::strcat( MipMapFileBuff[0], ".bmp" );
		}
	}

	//
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

	memcpy( &ddsd.ddpfPixelFormat , &ddDefaultTextureFormat, sizeof(DDPIXELFORMAT) );
	ddsd.dwSize	  = sizeof(DDSURFACEDESC2);
	ddsd.dwWidth  = g_DibLastReadWidth;
	ddsd.dwHeight = g_DibLastReadHeight;

	if( MipMapCount )
	{
		MipMapCount++;
		if( MipMapCreateTextureCheck(g_DibLastReadWidth, g_DibLastReadHeight, MipMapCount) <= 0 )
			return NULL;

		ddsd.dwFlags         = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;		
		ddsd.dwMipMapCount   = MipMapCount;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_TEXTURE;
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	}
	else
	{
		ddsd.dwFlags  = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		if (renderDevice.GetDeviceDesc_IsHardware())
		{
			ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
			ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
		}
		else
			ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_TEXTURE;
	}
	
	//
	DIRECTDRAWSURFACE lpDDSurface;
	hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
	if( hresult != DD_OK )
	{
		// 256x256 �ʰ� �ؽ��� �̶��.
		if( g_DibLastReadWidth > 256 || g_DibLastReadHeight > 256 )
		{
			if (renderDevice.GetDeviceDesc_IsHardware())
			{
				ddsd.ddsCaps.dwCaps  = DDSCAPS_SYSTEMMEMORY | DDSCAPS_TEXTURE;
				ddsd.ddsCaps.dwCaps2 = 0;

				hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
				if( hresult == DD_OK )
					rTexHalf = TRUE;
			}
		}

		if( hresult != DD_OK )
            return NULL;
	}

	if( DrawSurfaceFromDib(lpDDSurface, lpDib) == FALSE )
	{
		lpDDSurface->Release();
		return NULL;
	}

	//
	if( MipMapCount && rTexHalf == FALSE )
	{
		DDSCAPS2 ddsCaps;
		ZeroMemory( &ddsCaps, sizeof(DDSCAPS2) );
		ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

		DIRECTDRAWSURFACE lpDDSurface2 = lpDDSurface;
		int					 width2		  = g_DibLastReadWidth;
		int					 height2	  = g_DibLastReadHeight;

		for( int i=1; i < MipMapCount; i++ )
		{
			DIRECTDRAWSURFACE lpDDNextLevel = NULL;
			if( FAILED( lpDDSurface2->GetAttachedSurface(&ddsCaps, &lpDDNextLevel) ) )
			{
				lpDDSurface->Release();
				return NULL;
			}

			width2  >>= 1;
			height2 >>= 1;

			if( new_LoadDib( MipMapFileBuff[i] ) == NULL )
			{
				lpDDNextLevel->Release();
				lpDDSurface->Release();
				return NULL;
			}

			if( width2 != GetDibWidth(lpDib) || height2 != GetDibHeight(lpDib) )
			{
				lpDDNextLevel->Release();
				lpDDSurface->Release();
				return NULL;
			}

			if( DrawSurfaceFromDib(lpDDNextLevel, lpDib) == FALSE )
			{
				lpDDNextLevel->Release();
				lpDDSurface->Release();
				return NULL;
			}

			lpDDSurface2 = lpDDNextLevel;
			lpDDNextLevel->Release();
		}
	}

	//
	return lpDDSurface;
}

DIRECTDRAWSURFACE new_LoadTgaCreateTexSurface(char *Filename, int MipMap, int &rTexHalf)
{
	if (new_LoadTga(Filename) == NULL)
		return NULL;

	if( MipMap && MipMapModeCreate && g_DibLastReadWidth < 512 && g_DibLastReadHeight < 512 )
	{
		if( MipMapModeCreate == NOSQUARE_MODE || g_DibLastReadWidth == g_DibLastReadHeight )
		{
			int i, len = ::lstrlen( Filename );
			for( i=len-1; i >= 0; i-- )
			{
				if( Filename[i] == '.' )
				{
					::CopyMemory( MipMapFileBuff[0], Filename, i );
					break;
				}
			}

			//In peute haeteum to use only Mipmaps up to three steps.
			for( i=1; i < MIPMAP_FILELIST; i++ )
			{
				::strcpy( MipMapFileBuff[i], MipMapFileBuff[0] );
				::strcat( MipMapFileBuff[i], MipMapFileListBuff[i] );
				::strcat( MipMapFileBuff[i], ".tga" );

				if( PathFileExists( MipMapFileBuff[i] ) )
					MipMapCount++;
				else
					break;
			}

			::strcat( MipMapFileBuff[0], ".tga" );
		}
	}

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );

	memcpy( &ddsd.ddpfPixelFormat, &ddTextureFormat, sizeof(DDPIXELFORMAT) );
	ddsd.dwSize	  = sizeof(DDSURFACEDESC2);
	ddsd.dwWidth  = g_DibLastReadWidth;
	ddsd.dwHeight = g_DibLastReadHeight;

	if( MipMapCount )
	{
		MipMapCount++;
		if( MipMapCreateTextureCheck(g_DibLastReadWidth, g_DibLastReadHeight, MipMapCount) <= 0 )
			return NULL;

		ddsd.dwFlags         = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;		
		ddsd.dwMipMapCount   = MipMapCount;
		ddsd.ddsCaps.dwCaps  = DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_TEXTURE;
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	}
	else
	{
		ddsd.dwFlags  = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		if (renderDevice.GetDeviceDesc_IsHardware())
		{
			ddsd.ddsCaps.dwCaps  = DDSCAPS_TEXTURE;
			ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
		}
		else
			ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_TEXTURE;
	}

	//
	DIRECTDRAWSURFACE lpDDSurface;
	hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
	if( hresult != DD_OK )
	{
        // 256x256 �ʰ� �ؽ��� �̶��.
		if( g_DibLastReadWidth > 256 || g_DibLastReadHeight > 256 )
		{
			if (renderDevice.GetDeviceDesc_IsHardware())
			{
				ddsd.ddsCaps.dwCaps  = DDSCAPS_SYSTEMMEMORY | DDSCAPS_TEXTURE;
				ddsd.ddsCaps.dwCaps2 = 0;

				hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
				if( hresult == DD_OK )
					rTexHalf = TRUE;
			}
		}

		if( hresult != DD_OK )
            return NULL;
	}

	//
	if( New_DrawSurfaceFromTga(lpDDSurface) == FALSE )
	{
		lpDDSurface->Release();
		return NULL;
	}

	//
	if( MipMapCount && rTexHalf == FALSE )
	{
		DDSCAPS2 ddsCaps;
		ZeroMemory( &ddsCaps, sizeof(DDSCAPS2) );
		ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

		DIRECTDRAWSURFACE lpDDSurface2 = lpDDSurface;
		
		int width2, height2, OrgWidth, OrgHeight;
		width2  = OrgWidth  = g_DibLastReadWidth;
        height2	= OrgHeight = g_DibLastReadHeight;

		for( int i=1; i < MipMapCount; i++ )
		{
			DIRECTDRAWSURFACE lpDDNextLevel = NULL;
			if( FAILED( lpDDSurface2->GetAttachedSurface(&ddsCaps, &lpDDNextLevel) ) )
			{
				lpDDSurface->Release();
				return NULL;
			}

			width2  >>= 1;
			height2 >>= 1;

			if( new_LoadTga( MipMapFileBuff[i] ) == NULL )
			{
				lpDDNextLevel->Release();
				lpDDSurface->Release();
				return NULL;
			}

			if( width2 != g_DibLastReadWidth || height2 != g_DibLastReadHeight )
			{
				lpDDNextLevel->Release();
				lpDDSurface->Release();
				return NULL;
			}

			if( New_DrawSurfaceFromTga(lpDDNextLevel) == FALSE )
			{
				lpDDNextLevel->Release();
				lpDDSurface->Release();
				return NULL;
			}

			lpDDSurface2 = lpDDNextLevel;
			lpDDNextLevel->Release();
		}

		g_DibLastReadWidth  = OrgWidth;
		g_DibLastReadHeight = OrgHeight;
	}

	return lpDDSurface;
}

DIRECTDRAWSURFACE new_LoadBmpSurfaceOffscreen( char *Filename )
{
	//if file loading failed...
	if( new_LoadDib(Filename) == NULL )
		return NULL;
	
	BYTE *lpDib = g_DibImageDateBuff;
	g_DibLastReadWidth  = GetDibWidth(lpDib);
	g_DibLastReadHeight = GetDibHeight(lpDib);

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));

	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.dwWidth = g_DibLastReadWidth;
	ddsd.dwHeight = g_DibLastReadHeight;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	DIRECTDRAWSURFACE lpDDSurface;
	hresult = renderDevice.CreateSurface(&ddsd, &lpDDSurface, NULL);
	if (hresult != DD_OK)
		return NULL;

	if (DrawSurfaceFromDib(lpDDSurface, lpDib) == FALSE)
	{
		lpDDSurface->Release();
		return NULL;
	}

	DDCOLORKEY ColKey;
	ColKey.dwColorSpaceLowValue = RGB(0, 0, 0);
	ColKey.dwColorSpaceHighValue = RGB(255, 255, 255);
	lpDDSurface->SetColorKey(DDCKEY_SRCBLT, &ColKey);

	return lpDDSurface;
}
