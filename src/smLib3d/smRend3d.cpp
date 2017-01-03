#include "common.h"
#include "smd3d.h"

#define SM_DIST_NEARZ				16
#define SMFLOAT_DIST_NEARZ			(SM_DIST_NEARZ<<FLOATNS)
//#define SM_DIST_FARZ				64*26
#define SM_DIST_FARZ				64*25
#define SMFLOAT_DIST_FARZ			(SM_DIST_FARZ<<FLOATNS)

#define SM_DIST_TEXCLIP				64*8
#define SMFLOAT_DIST_TEXCLIP		(SM_DIST_TEXCLIP<<FLOATNS)
/*
#define SMSHIFT_PERSPECTIVE_WIDTH	1
//#define SMMULT_PERSPECTIVE_HEIGHT	(int)( ((float)2.7) * fONE )

#define SMMULT_PERSPECTIVE_HEIGHT	(int)( ((float)3.36) * fONE )
*/

#define smCLIP_DETAIL			(fONE*8)
#define smCLIP_DETAIL_SHIFT		(FLOATNS+3)


#define FLOATS_EM				(FLOATNS+3)


//######################################################################################
//�� �� �� : �� �� ��
//static POINT3D	pLight = { -fONE, 1, fONE/2 };
static POINT3D	pLight = { fONE/4 , fONE/2, -fONE };
//######################################################################################


//######################################################################################
//�� �� �� : �� �� ��
//
#define MAX_ITEM2PASS_TEX										10
static  smTEXTUREHANDLE			*s_Item2PassTexHandleBuff[ MAX_ITEM2PASS_TEX ];
static  char					*s_Item2PassTexName[ MAX_ITEM2PASS_TEX ] = 
{
	"mixS_01.bmp", "mixS_02.bmp", "mixS_03.bmp", "mixS_04.bmp", "mixS_05.bmp",
	"mixM_01.bmp", "mixM_02.bmp", "mixM_03.bmp", "mixM_04.bmp", "mixM_05.bmp",
};

//
int CreateItem2PassTexture(void)
{
	char szImagePath[64] = {0,};
	for( int i=0; i < MAX_ITEM2PASS_TEX; i++ )
	{
		wsprintf( szImagePath, "Image\\sinImage\\Items\\DropItem\\%s", s_Item2PassTexName[i] );
        s_Item2PassTexHandleBuff[i] = smTexture.Add( szImagePath );
		if( s_Item2PassTexHandleBuff[i] == NULL )
            return FALSE;
	}

	return TRUE;
}

//
static  int						 s_Item2PassTexFormState;
static  smTEXTUREHANDLE			*s_lpItem2PassTexHandle;

void SetItem2PassTexture( int TexHandleNum, int TexFormState )
{
	if( TexHandleNum < 0 || TexHandleNum >= MAX_ITEM2PASS_TEX || s_Item2PassTexHandleBuff[TexHandleNum] == NULL )
	{
		s_lpItem2PassTexHandle = NULL;
		return;
	}

	s_lpItem2PassTexHandle   = s_Item2PassTexHandleBuff[ TexHandleNum ];
	s_Item2PassTexFormState  = TexFormState;
}

// �� �� �� : �� �� ��	
extern DWORD dwPlayTime;
static void SetBlinkColor( short sColors[4], smFCOLOR BlinkCol, int BlinkStep )
{
	int r = int(BlinkCol.r * 255);
	int g = int(BlinkCol.g * 255);
	int b = int(BlinkCol.b * 255);
 
	int BlinkTime = 1 << BlinkStep;
	int BlinkMsk  = BlinkTime - 1;
	int bTime     = dwPlayTime & BlinkMsk;

	if( (dwPlayTime & BlinkTime) == 0 )
		bTime = BlinkMsk - bTime;

	sColors[SMC_R] = (bTime * r) >> BlinkStep;
	sColors[SMC_G] = (bTime * g) >> BlinkStep;
	sColors[SMC_B] = (bTime * b) >> BlinkStep;
	//if( a < 0 )
    //    sColors[SMC_A] = (bTime * a) >> BlinkStep;
	//else if( a > 0 )
	//	sColors[SMC_A] = -a;
	//else
		sColors[SMC_A] = 0;
}

//
static int SetBlinkColorVertex( smRENDVERTEX *rv, short sColors[4], BYTE Color[4], BYTE Specular[4] )
{
	short BlinkColor[4] = {0,};
	::CopyMemory( BlinkColor, sColors, sizeof(BlinkColor) );

	int z = rv->tz >> FLOATNS;
	if( z > ccDistZMin )
	{
		int lev = ( (z - ccDistZMin) << (13 - ShiftMAPCELL_MULT) ) >> 8;
		if( lev >= 256 )
			return FALSE;

		BlinkColor[SMC_R] -= ((sColors[SMC_R] * lev ) >> 8);
		BlinkColor[SMC_G] -= ((sColors[SMC_G] * lev ) >> 8);
		BlinkColor[SMC_B] -= ((sColors[SMC_B] * lev ) >> 8);
		BlinkColor[SMC_A] -= ((sColors[SMC_A] * lev ) >> 8);
	}

	for( int cnt=0; cnt < 4; cnt++ )
	{
		BlinkColor[cnt] += rv->bCol[cnt];
		if( BlinkColor[cnt] > 255 )
		{
			short spe = rv->bSpe[cnt] + (BlinkColor[cnt] - 256);
			Color[cnt] = 255;
			if( spe > 255 )
				Specular[cnt] = 255;
			else
				Specular[cnt] = (BYTE)spe;
		}
		else
		{
			Color[cnt]    = (BYTE)BlinkColor[cnt];
			Specular[cnt] = rv->bSpe[cnt];
		}
	}

	return TRUE;
}


//�� �� �� : �� �� ��
//
static float s_DxProjection_w;
static float s_DxProjection_h;
static float s_DxProjection_q1;
static float s_DxProjection_q2;
static int   s_CenterX;
static int   s_CenterY;

//
int			 g_IsDxProjectZoomIn;

float		 g_fZoomInAdd_x;
float		 g_fZoomInAdd_y;
float		 g_fZoomInDiv_x;
float		 g_fZoomInDiv_y;

//
void SetDxProjection( float radians, int SizeX, int SizeY, float near_z, float far_z )
{
	s_DxProjection_h  = float(1.f / tan(radians/2.f));
	s_DxProjection_w  = s_DxProjection_h / (float(SizeX) / float(SizeY));
	s_DxProjection_q1 = far_z / (far_z - near_z);
	s_DxProjection_q2 = -near_z * s_DxProjection_q1;

	s_CenterX = SizeX >> 1;
	s_CenterY = SizeY >> 1;

	//
	float fWinSizeOffset_X = float(SizeX) / 800.f;
	float fWinSizeOffset_Y = float(SizeY) / 600.f;

	g_fZoomInAdd_x = (30.f  * fWinSizeOffset_X);
	g_fZoomInAdd_y = (337.f * fWinSizeOffset_Y);
	g_fZoomInDiv_x = (float(SizeX) / (320.f * fWinSizeOffset_X));
	g_fZoomInDiv_y = (float(SizeY) / (240.f * fWinSizeOffset_Y));
}

//
void DxProjectionWork( smRENDVERTEX *lpRendVer )
{
	float fx, fy, fz;
	fx = float(lpRendVer->tx) / fONE;
	fy = float(lpRendVer->ty) / fONE;
	fz = float(lpRendVer->tz) / fONE;

	float fx_p, fy_p, fz_p,  fw_p;
    fx_p = s_DxProjection_w * fx;
    fy_p = s_DxProjection_h * fy;
    fz_p = (s_DxProjection_q1 * fz) + s_DxProjection_q2;
    fw_p = fz;

	lpRendVer->xy[0]  = ( 1.0f + (fx_p / fw_p) ) * s_CenterX;
	lpRendVer->xy[1]  = ( 1.0f - (fy_p / fw_p) ) * s_CenterY;
	lpRendVer->zb     = fz_p / fw_p;
	lpRendVer->rhw    =  1   / fw_p;

	if( g_IsDxProjectZoomIn )
	{
		//lpRendVer->xy[0] /= 2.5f;
		//lpRendVer->xy[1] /= 2.5f;
		//lpRendVer->xy[0] += 30;
        //lpRendVer->xy[1] += 337;

		if( lpRendVer->xy[0] < 0 )
			lpRendVer->xy[0] = 0;
		else if( lpRendVer->xy[0] > (s_CenterX << 1) )
			lpRendVer->xy[0] = float(s_CenterX << 1);

		if( lpRendVer->xy[1] < 0 )
			lpRendVer->xy[1] = 0;
		else if( lpRendVer->xy[1] > (s_CenterY << 1) )
			lpRendVer->xy[1] = float(s_CenterY << 1);

		lpRendVer->xy[0] /= g_fZoomInDiv_x;
		lpRendVer->xy[1] /= g_fZoomInDiv_y;
		lpRendVer->xy[0] += g_fZoomInAdd_x;
        lpRendVer->xy[1] += g_fZoomInAdd_y;
	}
	else
	{
		lpRendVer->xy[0] -= (s_CenterX - MidX);
		lpRendVer->xy[1] -= (s_CenterY - MidY);
	}
}

//������ Ŭ���� 
smRENDER3D	smRender;

float	RendBuff[64 * 16384];

DWORD D3DFVF_TEXTURE[9] = {
	D3DFVF_TLVERTEX0 , 
	D3DFVF_TLVERTEX1 , 
	D3DFVF_TLVERTEX2 , 
	D3DFVF_TLVERTEX3 , 
	D3DFVF_TLVERTEX4 , 
	D3DFVF_TLVERTEX5 , 
	D3DFVF_TLVERTEX6 , 
	D3DFVF_TLVERTEX7 , 
	D3DFVF_TLVERTEX8
};

DWORD D3DTLVERTEX_SIZE[9] = {
	sizeof( D3DFVF_TLVERTEX0 ),
	sizeof( D3DFVF_TLVERTEX1 ),
	sizeof( D3DFVF_TLVERTEX2 ),
	sizeof( D3DFVF_TLVERTEX3 ),
	sizeof( D3DFVF_TLVERTEX4 ),
	sizeof( D3DFVF_TLVERTEX5 ),
	sizeof( D3DFVF_TLVERTEX6 ),
	sizeof( D3DFVF_TLVERTEX7 ),
	sizeof( D3DFVF_TLVERTEX8 )
};

smRENDER3D::smRENDER3D()
{
	//######################################################################################
	//�� �� �� : �� �� ��
	m_dwRenderType = 0;
	//######################################################################################

	//######################################################################################
	//�� �� �� : �� �� ��
	m_FogIsRend  = FALSE;
	m_FogMode    = FALSE;
	m_fFogStNum  = 400.f;
	m_fFogEndNum = 3000.f;
	m_dwFogColor = 0xC0C0C0C0;
	//######################################################################################

	//######################################################################################
	//�� �� �� : �� �� ��
	nRendObjLight = 0;
	//######################################################################################

	CameraPosiMask = FALSE;
	nRendLight = 0;

	SMSHIFT_PERSPECTIVE_WIDTH = RENDCLIP_DEFAULT_SHIFT_PERSPECTIVE_WIDTH;
	SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_DEFAULT_MULT_PERSPECTIVE_HEIGHT;

	ObjectVertexScale = 0;
	DistortionMesh = 0;			//�޽� �ְ� ���

	//######################################################################################
	//�� �� �� : �� �� ��	
	m_GameFieldView     = 0;
	m_GameFieldViewStep = 0;

	RendVertex = NULL;
	RendFace   = NULL;
	//######################################################################################

}

smRENDER3D::~smRENDER3D()
{
	//�� �� �� : �� �� ��	
	if( RendVertex )
	{
		delete [] RendVertex;
		RendVertex = NULL;
	}

	if( RendFace )
	{
		delete [] RendFace;
		RendFace = NULL;
	}
}

//�� �� �� : �� �� ��
void smRENDER3D::CreateRenderBuff( int FieldViewType )
{
	//
	if( RendVertex )
	{
		delete [] RendVertex;
		RendVertex = NULL;
	}

	if( RendFace )
	{
		delete [] RendFace;
		RendFace = NULL;
	}

	//
	m_GameFieldView = FieldViewType;
	if( m_GameFieldView )
	{
		// ���� �ʵ� Ȯ�� �þ�.
		RendVertex = new smRENDVERTEX[ 40000 ];
		::ZeroMemory( RendVertex, sizeof(smRENDVERTEX) * 40000 );

		RendFace   = new smRENDFACE[ 40000 ];
		::ZeroMemory( RendFace, sizeof(smRENDFACE) * 40000 );
	}
	else
	{
		// ���� ���� �ʵ� �þ�.
		RendVertex = new smRENDVERTEX[ RENDER_VERTEX_MAX ];
		::ZeroMemory( RendVertex, sizeof(smRENDVERTEX) * RENDER_VERTEX_MAX );

		RendFace   = new smRENDFACE[ RENDER_FACE_MAX ];
		::ZeroMemory( RendFace, sizeof(smRENDFACE) * RENDER_FACE_MAX );
	}
}

void smRENDER3D::SetGameFieldViewStep(void)
{
	if( m_GameFieldView == 0 )
		return;

	ViewPointLen = (38 + m_GameFieldViewStep) * SizeMAPCELL;
	ccDistZMin   = ViewPointLen - (20 + (m_GameFieldViewStep/4)) * SizeMAPCELL;
}

// ��ǥ�� 2D �������� ��ȯ�Ͽ� �����ش�  
int smRENDER3D::GetRect2D ( int x, int y, int z, int width, int height, RECT *Rect ,POINT *Pos )
{
	//int rx,ry,rz;
	int cx,cy,cz;
	int w,h;
	int cnt;
	int kz;
	POINT3D p[3];

		x -= CameraPosiX;
		y -= CameraPosiY;
		z -= CameraPosiZ;

		//�߰��ϸ鼭 ī�޶� ��ȯ ��Ʈ������ ���Ͽ� ī�޶� ��ǥ�� ��ȯ�Ͽ� ����

		//######################################################################################
		//�� �� �� : �� �� ��		
		__int64 x_64, y_64, z_64, rx_64, ry_64, rz_64;
		rx_64 = ry_64 = rz_64 = 0;

		x_64 = x;
		y_64 = y;
		z_64 = z;

		rx_64  = ((x_64 * mCamera._11) >> FLOATS_EM);
		rx_64 += ((y_64 * mCamera._21) >> FLOATS_EM);
		rx_64 += ((z_64 * mCamera._31) >> FLOATS_EM);

		ry_64  = ((x_64 * mCamera._12) >> FLOATS_EM);
		ry_64 += ((y_64 * mCamera._22) >> FLOATS_EM);
		ry_64 += ((z_64 * mCamera._32) >> FLOATS_EM);

		rz_64  = ((x_64 * mCamera._13) >> FLOATS_EM);
		rz_64 += ((y_64 * mCamera._23) >> FLOATS_EM);
		rz_64 += ((z_64 * mCamera._33) >> FLOATS_EM);
	
		cx = int(rx_64);
		cy = int(ry_64);
		cz = int(rz_64);
		
		/*
		rx = x * mCamera._11 +
			 y * mCamera._21 +
			 z * mCamera._31 ;

		ry = x * mCamera._12 +
			 y * mCamera._22 +
			 z * mCamera._32 ;

		rz = x * mCamera._13 +
			 y * mCamera._23 +
			 z * mCamera._33 ;

		cx = rx>>FLOATS_EM;
		cy = ry>>FLOATS_EM;
		cz = rz>>FLOATS_EM;
		*/
		//######################################################################################

		w = width>>1;
		h = height>>1;

		p[0].x = cx-w;
		p[1].x = cx+w;
		p[0].y = cy-h;
		p[1].y = cy+h;
		p[0].z = cz;
		p[1].z = cz;

		p[2].x = cx;
		p[2].y = cy;
		p[2].z = cz;
		

		kz = cz;//>>FLOATNS;
		if ( kz<=0 ) return NULL;

		for(cnt=0;cnt<3;cnt++) {

//			p[cnt].x = MidX + ( (p[cnt].x>>FLOATNS) * viewdistZ )/ kz;			// 2D X
//			p[cnt].y = MidY - ( (p[cnt].y>>FLOATNS) * viewdistZ )/ kz;			// 2D Y
			p[cnt].x = MidX + ( p[cnt].x * viewdistZ )/ kz;			// 2D X
			p[cnt].y = MidY - ( p[cnt].y * viewdistZ )/ kz;			// 2D Y

		}

	Rect->top    = p[1].y;
	Rect->bottom = p[0].y;

	Rect->left   = p[0].x;
	Rect->right  = p[1].x;

	if ( Pos ) {
		Pos->x = p[2].x;
		Pos->y = p[2].y;
	}

	return cz;
}

// ī�޶� Z��ǥ�� ������  
int smRENDER3D::GetDepthZ ( int x, int y, int z )
{
	int rz;

	x -= CameraPosiX;
	y -= CameraPosiY;
	z -= CameraPosiZ;

	rz = x * mCamera._13 +
		 y * mCamera._23 +
		 z * mCamera._33 ;

	return (rz>>FLOATS_EM);
}

// �������� ī�޶� ��ġ�� �Է� 
int smRENDER3D::SetCameraPosi ( int x, int y, int z, int angX, int angY, int angZ )
{
	smEMATRIX	rX,rY,rZ,tm1;

	//ī�޶� ��ġ�� ����
	CameraPosiX = x;
	CameraPosiY = y;
	CameraPosiZ = z;

	CameraAngX = angX;
	CameraAngY = angY;
	CameraAngZ = angZ;

	if ( CameraPosiMask ) return FALSE;

	//��� �ʱ�ȭ
	smEIdentityMatrix( rX );
	smEIdentityMatrix( rY );
	smEIdentityMatrix( rZ );

	//ȸ�� �̵� ��Ʈ���� ����
	smERotateXMatrix( rX , GetRadian(-angX) );
	smERotateYMatrix( rY , GetRadian(-angY) );
	smERotateZMatrix( rZ , GetRadian(-angZ) );

	//��Ʈ���� ����
	smEMatrixMult( tm1 , rY , rX );
	smEMatrixMult( (smEMATRIX &)mCamera , tm1 , rZ );
//	smMatrixShift( mCamera , wSHIFT_FLOAT );
	smMatrixShift( mCamera , 4 );

	//smMatrixMult( tm1 , tm1 , rZ );
	//smMatrixMult( mCamera, mTrans , tm1 );

	return TRUE;
}

// �������� ī�޶� ��ġ�� �Է� 
int smRENDER3D::SetCameraPosi ( int x, int y, int z, smEMATRIX *lpeMatrix )
{
	//ī�޶� ��ġ�� ����
	CameraPosiX = x;
	CameraPosiY = y;
	CameraPosiZ = z;

	if ( CameraPosiMask ) return FALSE;

	memcpy( (smEMATRIX *)&mCamera , lpeMatrix , sizeof( smEMATRIX ) );
	smMatrixShift( mCamera , 4 );

	return TRUE;
}

//�� �� �� : �� �� ��
int	smRENDER3D::AddDynamicLight( int type, int x, int y, int z, int r, int g, int b, int a, int Range )
{
	//######################################################################################
	//�� �� �� : �� �� ��		
	__int64 x_64, y_64, z_64, rx_64, ry_64, rz_64;
	//######################################################################################

	//int rx,ry,rz;
	int dRange;

	if( type & SMLIGHT_TYPE_OBJ )
	{  
		if( nRendObjLight < REND_LIGHT_MAX )
		{
			smRendObjLight[ nRendObjLight ].x = x;
			smRendObjLight[ nRendObjLight ].y = y;
			smRendObjLight[ nRendObjLight ].z = z;

			smRendObjLight[ nRendObjLight ].r = r;
			smRendObjLight[ nRendObjLight ].g = g;
			smRendObjLight[ nRendObjLight ].b = b;
			smRendObjLight[ nRendObjLight ].a = a;

			smRendObjLight[ nRendObjLight ].Range = Range;
			dRange = Range>>FLOATNS;
			smRendObjLight[ nRendObjLight ].dRange = dRange*dRange;

			x -= CameraPosiX;
			y -= CameraPosiY;
			z -= CameraPosiZ;

			//######################################################################################
			//�� �� �� : �� �� ��		
			rx_64 = ry_64 = rz_64 = 0;

			x_64 = x;
			y_64 = y;
			z_64 = z;

			rx_64  = ((x_64 * mCamera._11) >> FLOATS_EM);
			rx_64 += ((y_64 * mCamera._21) >> FLOATS_EM);
			rx_64 += ((z_64 * mCamera._31) >> FLOATS_EM);

			ry_64  = ((x_64 * mCamera._12) >> FLOATS_EM);
			ry_64 += ((y_64 * mCamera._22) >> FLOATS_EM);
			ry_64 += ((z_64 * mCamera._32) >> FLOATS_EM);

			rz_64  = ((x_64 * mCamera._13) >> FLOATS_EM);
			rz_64 += ((y_64 * mCamera._23) >> FLOATS_EM);
			rz_64 += ((z_64 * mCamera._33) >> FLOATS_EM);
		
			x = int(rx_64);
			y = int(ry_64);
			z = int(rz_64);

			/*
			rx = x * mCamera._11 +
				 y * mCamera._21 +
				 z * mCamera._31 ;

			ry = x * mCamera._12 +
				 y * mCamera._22 +
				 z * mCamera._32 ;

			rz = x * mCamera._13 +
				 y * mCamera._23 +
				 z * mCamera._33 ;

			x = rx>>FLOATS_EM;
			y = ry>>FLOATS_EM;
			z = rz>>FLOATS_EM;
			*/
			//######################################################################################

			smRendObjLight[ nRendObjLight ].rx = x ;
			smRendObjLight[ nRendObjLight ].ry = y ;
			smRendObjLight[ nRendObjLight ].rz = z ;

			nRendObjLight++;

			return TRUE;
		}
	}
	else
	{
		if( nRendLight < REND_LIGHT_MAX )
		{
			smRendLight[ nRendLight ].x = x;
			smRendLight[ nRendLight ].y = y;
			smRendLight[ nRendLight ].z = z;

			smRendLight[ nRendLight ].r = r;
			smRendLight[ nRendLight ].g = g;
			smRendLight[ nRendLight ].b = b;
			smRendLight[ nRendLight ].a = a;

			smRendLight[ nRendLight ].Range = Range;
			dRange = Range>>FLOATNS;
			smRendLight[ nRendLight ].dRange = dRange*dRange;

			x -= CameraPosiX;
			y -= CameraPosiY;
			z -= CameraPosiZ;

			//######################################################################################
			//�� �� �� : �� �� ��
			rx_64 = ry_64 = rz_64 = 0;

			x_64 = x;
			y_64 = y;
			z_64 = z;

			rx_64  = ((x_64 * mCamera._11) >> FLOATS_EM);
			rx_64 += ((y_64 * mCamera._21) >> FLOATS_EM);
			rx_64 += ((z_64 * mCamera._31) >> FLOATS_EM);

			ry_64  = ((x_64 * mCamera._12) >> FLOATS_EM);
			ry_64 += ((y_64 * mCamera._22) >> FLOATS_EM);
			ry_64 += ((z_64 * mCamera._32) >> FLOATS_EM);

			rz_64  = ((x_64 * mCamera._13) >> FLOATS_EM);
			rz_64 += ((y_64 * mCamera._23) >> FLOATS_EM);
			rz_64 += ((z_64 * mCamera._33) >> FLOATS_EM);
		
			x = int(rx_64);
			y = int(ry_64);
			z = int(rz_64);

			/*
			rx = x * mCamera._11 +
				y * mCamera._21 +
				z * mCamera._31 ;

			ry = x * mCamera._12 +
				y * mCamera._22 +
				z * mCamera._32 ;

			rz = x * mCamera._13 +
				y * mCamera._23 +
				z * mCamera._33 ;

			x = rx>>FLOATS_EM;
			y = ry>>FLOATS_EM;
			z = rz>>FLOATS_EM;
			*/
			//######################################################################################

			smRendLight[ nRendLight ].rx = x ;
			smRendLight[ nRendLight ].ry = y ;
			smRendLight[ nRendLight ].rz = z ;

			nRendLight++;

			return TRUE;
		}
	}

	return FALSE;
}

// ���� ��� ���� �Ѵ�
int smRENDER3D::ClearObjLight()
{
	bApplyRendObjLight = 0;
	nRendObjLight = 0;
	return TRUE;
}

// �������� ���� �߰� �Ѵ�
int	smRENDER3D::AddDynamicLight( int x, int y, int z, int r, int g, int b, int a, int Range )
{
	//int rx,ry,rz;
	int dRange;

	if ( nRendLight<REND_LIGHT_MAX ) {
		smRendLight[ nRendLight ].x = x;
		smRendLight[ nRendLight ].y = y;
		smRendLight[ nRendLight ].z = z;

		smRendLight[ nRendLight ].r = r;
		smRendLight[ nRendLight ].g = g;
		smRendLight[ nRendLight ].b = b;
		smRendLight[ nRendLight ].a = a;

		smRendLight[ nRendLight ].Range = Range;
		dRange = Range>>FLOATNS;
		smRendLight[ nRendLight ].dRange = dRange*dRange;

		x -= CameraPosiX;
		y -= CameraPosiY;
		z -= CameraPosiZ;

		//######################################################################################
		//�� �� �� : �� �� ��		
		__int64 x_64, y_64, z_64, rx_64, ry_64, rz_64;
		rx_64 = ry_64 = rz_64 = 0;

		x_64 = x;
		y_64 = y;
		z_64 = z;

		rx_64  = ((x_64 * mCamera._11) >> FLOATS_EM);
		rx_64 += ((y_64 * mCamera._21) >> FLOATS_EM);
		rx_64 += ((z_64 * mCamera._31) >> FLOATS_EM);

		ry_64  = ((x_64 * mCamera._12) >> FLOATS_EM);
		ry_64 += ((y_64 * mCamera._22) >> FLOATS_EM);
		ry_64 += ((z_64 * mCamera._32) >> FLOATS_EM);

		rz_64  = ((x_64 * mCamera._13) >> FLOATS_EM);
		rz_64 += ((y_64 * mCamera._23) >> FLOATS_EM);
		rz_64 += ((z_64 * mCamera._33) >> FLOATS_EM);
	
		x = int(rx_64);
		y = int(ry_64);
		z = int(rz_64);

		/*
		rx = x * mCamera._11 +
			 y * mCamera._21 +
			 z * mCamera._31 ;

		ry = x * mCamera._12 +
			 y * mCamera._22 +
			 z * mCamera._32 ;

		rz = x * mCamera._13 +
			 y * mCamera._23 +
			 z * mCamera._33 ;

		x = rx>>FLOATS_EM;
		y = ry>>FLOATS_EM;
		z = rz>>FLOATS_EM;
		*/
		//######################################################################################

		smRendLight[ nRendLight ].rx = x ;
		smRendLight[ nRendLight ].ry = y ;
		smRendLight[ nRendLight ].rz = z ;

		nRendLight++;

		return TRUE;
	}

	return FALSE;
}

// ���� ��� ���� �Ѵ�
int smRENDER3D::ClearLight()
{
	nRendLight = 0;

	return TRUE;
}

// �������� ī�޶� ��ġ�� �Է� 
int smRENDER3D::OpenCameraPosi ( int x, int y, int z, smEMATRIX *lpeMatrix )
{

	memcpy( (smEMATRIX *)&mCamera , lpeMatrix , sizeof( smEMATRIX ) );
	smMatrixShift( mCamera , 4 );

	//ī�޶� ��ġ�� ����
	CameraPosiX = x;
	CameraPosiY = y;
	CameraPosiZ = z;

	CameraPosiMask = TRUE;

	return TRUE;
}

// ī�޶� ��ġ ����
int smRENDER3D::CloseCameraPosi()
{
	CameraPosiMask = FALSE;
	return TRUE;
}

// �ؽ��� �߽� ��ǥ ����
int smRENDER3D::SetTexPosi( int x, int y, int z , int sx, int sy, int sz )
{
	TexPosiX = x;
	TexPosiY = y;
	TexPosiZ = z;

	TexSizeX = sx;
	TexSizeY = sy;
	TexSizeZ = sz;

	return TRUE;
}

// �ӽ� �޽ø� ����� �ʱ�ȭ ��
int smRENDER3D::InitStageMesh ( smSTAGE_VERTEX *StageVertex , DWORD sum )
{
	nTempTexLink	= 0;

	nRendVertex		= 0;							//������ �� ���ؽ� ��
	nRendFace		= 0;							//������ �� ���̽� ��

	RMatListCnt		= 0;							//��Ʈ���� ���� ī���� �ʱ�ȭ
	RMatListRearCnt = nMaterial-1;					//��Ʈ���� ���� ī���� �ʱ�ȭ
//	RMatListRearCnt = MATRIAL_MAX-1;				//��Ʈ���� ���� ī���� �ʱ�ȭ

	Vertex = StageVertex;							//StageVertex ������
	
//	nMaterial		=	MATRIAL_MAX;				//������ �� ��ü ��Ʈ���� ����
//	smRendMaterial	=	smMaterial;

	ClipStates = ( SMCLIP_NEARZ | SMCLIP_FARZ | SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM );

	SumCount = sum;

	//��Ʈ���� �� ���̽� ī���͸� 0���� �ʱ�ȭ
	ZeroMemory( RendMatrial , sizeof( smRENDMATRIAL ) * nMaterial );

	RendStatTime = GetCurrentTime();

	return TRUE;
}

// �ӽ� �޽ø� ����� �ʱ�ȭ ��
int smRENDER3D::Init()
{
	return InitStageMesh( 0 , 0 );
}

// Ŭ���ΰ� ����
int smRENDER3D::SetClipStates( DWORD States )
{
	ClipStates = States;
	return TRUE;
}

// ��Ʈ���� �׷� ����
int smRENDER3D::SetMaterialGroup( smMATERIAL_GROUP *MatGroup )
{
	smRendMaterial	= MatGroup->smMaterial;		//��Ʈ���� ������
//	nMaterial		= MatGroup->MaterialCount;	//������ �� ��ü ��Ʈ���� ����
	nMaterial		= MatGroup->MaxMaterial;	//������ �� ��ü ��Ʈ���� ����
	RMatListRearCnt = nMaterial-1;				//��Ʈ���� ���� ī���� �ʱ�ȭ

	return TRUE;
}

//�ؽ��� ��ǥ������ �߰��Ѵ� 
int smRENDER3D::AddRendTempTexLink( smRENDFACE *lpRendFace, DWORD *hTex,  float u0, float v0, float u1, float v1, float u2, float v2 )
{
	smTEXLINK	*tl;

	//��ǥ����
	TempTexLink[ nTempTexLink ].u[0] = u0;
	TempTexLink[ nTempTexLink ].v[0] = v0;
	TempTexLink[ nTempTexLink ].u[1] = u1;
	TempTexLink[ nTempTexLink ].v[1] = v1;
	TempTexLink[ nTempTexLink ].u[2] = u2;
	TempTexLink[ nTempTexLink ].v[2] = v2;
	TempTexLink[ nTempTexLink ].hTexture = 0;//hTex;
	TempTexLink[ nTempTexLink ].NextTex = 0;

	//������ ���̽��� �����Ų��
	if ( !lpRendFace->lpTexLink ) 
	{
		//������ �ؽ�����ǥ�� ���
		lpRendFace->lpTexLink = &TempTexLink[ nTempTexLink ];
	}
	else 
	{
		//�̹� ������ִ� ��ǥ�� �ִ°��
		tl = lpRendFace->lpTexLink;

		for(int cnt=0;cnt<8;cnt++ ) 
		{
			if ( !tl->NextTex ) 
			{
				//���������� ����� ��ǥ�� ã�� ����
				tl->NextTex = &TempTexLink[ nTempTexLink ];
				break;
			}
			else 
			{
				//���� ��ǥ�� �ѱ�
				tl = tl->NextTex;
			}

		}
	}

	nTempTexLink++;

	return nTempTexLink-1;
}

//�������� ���ؽ��� ������ ���ؽ� �߰� ( ���� ��ǥ �ҽ� ��� )
smRENDVERTEX *smRENDER3D::AddStageVertex ( smSTAGE_VERTEX *sv , smMATERIAL *lpMaterial )
{
	int rx,rz; //,ry 
	int x,y,z;
	int ad;
	DWORD ClipStatus;
	int ShiftMesh;

	DWORD *dwColor , *dwDefColor;

	int ttCnt , ttFlag;

	/*
	{ "wind_z1:" , sMATS_SCRIPT_WINDZ1 },
	{ "wind_z2:" , sMATS_SCRIPT_WINDZ2 },
	{ "wind_x1:" , sMATS_SCRIPT_WINDX1 },
	{ "wind_x2:" , sMATS_SCRIPT_WINDX2 },

	{ "water:" , sMATS_SCRIPT_WATER }
	*/

	// �� �� �� : �� �� ��
	if( lpMaterial->WindMeshBottom && (lpMaterial->UseState & sMATS_SCRIPT_BLINK_COLOR) == 0 )
	{
		//Ư�� ȿ���� ���� ��ǥ ����
		switch( lpMaterial->WindMeshBottom & 0x7FF ) 
		{
		case sMATS_SCRIPT_WINDX1:
			ttCnt = (RendStatTime>>2)&0xFF;
			ttFlag = (RendStatTime>>10)&1;

			if ( !ttFlag ) ttCnt = 255-ttCnt;
			ShiftMesh = (GetCos[ttCnt+256] )>>5;

			z =  sv->z;
			y =  sv->y;
			x =  ( sv->x - ShiftMesh );
			break;

		case sMATS_SCRIPT_WINDZ1:
			ttCnt = (RendStatTime>>2)&0xFF;
			ttFlag = (RendStatTime>>10)&1;

			if ( !ttFlag ) ttCnt = 255-ttCnt;
			ShiftMesh = (GetCos[ttCnt+256] )>>5;

			x =  sv->x;
			y =  sv->y;
			z =  ( sv->z - ShiftMesh );
			break;

		case sMATS_SCRIPT_WINDX2:
			ttCnt = (RendStatTime>>2)&0xFF;
			ttFlag = (RendStatTime>>10)&1;

			if ( !ttFlag ) ttCnt = 255-ttCnt;
			ShiftMesh = (GetCos[ttCnt+256] )>>3;

			z =  sv->z;
			y =  sv->y;
			x =  ( sv->x - ShiftMesh );
			break;

		case sMATS_SCRIPT_WINDZ2:
			ttCnt = (RendStatTime>>2)&0xFF;
			ttFlag = (RendStatTime>>10)&1;

			if ( !ttFlag ) ttCnt = 255-ttCnt;
			ShiftMesh = (GetCos[ttCnt+256] )>>3;

			x =  sv->x;
			y =  sv->y;
			z =  ( sv->z - ShiftMesh );
			break;

		case sMATS_SCRIPT_WATER:
			x = sv->x;
			z = sv->z;
			y = sv->y;

			rx = (((x<<3)+RendStatTime)>>1);
			rz = (((z<<3)+RendStatTime)>>1);

			rx = GetSin[ rx&ANGCLIP ];
			rz = GetSin[ rz&ANGCLIP ];

			x += rx>>4;
			z += rz>>4;
			break;
		}
	}
	else
	{
		x = sv->x;
		y = sv->y;
		z = sv->z;
	}
/*
			x = sv->x;
			z = sv->z;
			y = sv->y;

			rx = (((x<<3)+RendStatTime)>>1);
			rz = (((z<<3)+RendStatTime)>>1);

			rx = GetSin[ rx&ANGCLIP ];
			rz = GetSin[ rz&ANGCLIP ];

			x += rx>>4;
			z += rz>>4;
*/
		x -= CameraPosiX;
		y -= CameraPosiY;
		z -= CameraPosiZ;
/*
		double dx,dy,dz;
		double drx,dry,drz;

		dx = (double)x/(float)fONE;
		dy = (double)y/(float)fONE;
		dz = (double)z/(float)fONE;
		//�߰��ϸ鼭 ī�޶� ��ȯ ��Ʈ������ ���Ͽ� ī�޶� ��ǥ�� ��ȯ�Ͽ� ����
		drx = dx * mDCamera._11 +
			 dy * mDCamera._21 +
			 dz * mDCamera._31 ;

		dry = dx * mDCamera._12 +
			 dy * mDCamera._22 +
			 dz * mDCamera._32 ;

		drz = dx * mDCamera._13 +
			 dy * mDCamera._23 +
			 dz * mDCamera._33 ;

		RendVertex[nRendVertex].dx = drx;
		RendVertex[nRendVertex].dy = dry;
		RendVertex[nRendVertex].dz = drz;

		x = drx*fONE;
		y = dry*fONE;
		z = drz*fONE;

*/

		//�߰��ϸ鼭 ī�޶� ��ȯ ��Ʈ������ ���Ͽ� ī�޶� ��ǥ�� ��ȯ�Ͽ� ����

		//######################################################################################
		//�� �� �� : �� �� ��		
		__int64 x_64, y_64, z_64, rx_64, ry_64, rz_64;
		rx_64 = ry_64 = rz_64 = 0;

		x_64 = x;
		y_64 = y;
		z_64 = z;

		rx_64  = ((x_64 * mCamera._11) >> FLOATS_EM);
		rx_64 += ((y_64 * mCamera._21) >> FLOATS_EM);
		rx_64 += ((z_64 * mCamera._31) >> FLOATS_EM);

		ry_64  = ((x_64 * mCamera._12) >> FLOATS_EM);
		ry_64 += ((y_64 * mCamera._22) >> FLOATS_EM);
		ry_64 += ((z_64 * mCamera._32) >> FLOATS_EM);

		rz_64  = ((x_64 * mCamera._13) >> FLOATS_EM);
		rz_64 += ((y_64 * mCamera._23) >> FLOATS_EM);
		rz_64 += ((z_64 * mCamera._33) >> FLOATS_EM);

		x = int(rx_64);
		y = int(ry_64);
		z = int(rz_64);
		
		/*rx = x * mCamera._11 +
			 y * mCamera._21 +
			 z * mCamera._31 ;

		ry = x * mCamera._12 +
			 y * mCamera._22 +
			 z * mCamera._32 ;

		rz = x * mCamera._13 +
			 y * mCamera._23 +
			 z * mCamera._33 ;

		x = rx>>FLOATS_EM;
		y = ry>>FLOATS_EM;
		z = rz>>FLOATS_EM;*/
		//######################################################################################

		RendVertex[ nRendVertex ].tx = x ;
		RendVertex[ nRendVertex ].ty = y ;
		RendVertex[ nRendVertex ].tz = z ;
		RendVertex[ nRendVertex ].lpSourceVertex = sv;
		ClipStatus = 0;

		// Ŭ���� Z
		if ( z<=SMFLOAT_DIST_NEARZ ) 
			ClipStatus |= SMCLIP_NEARZ;
		if ( z>SMFLOAT_DIST_FARZ ) 
			ClipStatus |= SMCLIP_FARZ;
		//if ( z<=SMFLOAT_DIST_TEXCLIP ) ClipStatus |= SMCLIP_TEXTURE;
//		else {
			// 45�� Ŭ���� , �¿�
			ad = abs( x )<< SMSHIFT_PERSPECTIVE_WIDTH;

			if ( x<0 ) {
				if ( z<=ad ) ClipStatus |=SMCLIP_LEFT;
			}
			else {
				if ( z<=ad ) ClipStatus |=SMCLIP_RIGHT;
			}
			// 45�� Ŭ���� , ����
			ad = ( abs( y ) * SMMULT_PERSPECTIVE_HEIGHT ) >>FLOATNS;

			if ( y<0 ) {
				if ( z<=ad ) ClipStatus |=SMCLIP_BOTTOM;
			}
			else {
				if ( z<=ad ) ClipStatus |=SMCLIP_TOP;
			}
//		}

		RendVertex[ nRendVertex ].ClipStatus = ClipStatus;


		//���� ����

		dwColor = (DWORD*)RendVertex[ nRendVertex ].sLight;
		dwDefColor =(DWORD *)sv->sDef_Color;

		*dwColor ++ = *dwDefColor++;
		*dwColor	= *dwDefColor;

		nRendVertex ++;

	return &RendVertex[ nRendVertex-1 ];

}

//�������� ���̽��� ������ ���̽� �߰�
smRENDFACE *smRENDER3D::AddStageFace ( smSTAGE_FACE *face )
{
	int cnt;
	smSTAGE_VERTEX *sv;
	DWORD ClipStatus;
	smMATERIAL *lpMaterial;

	if ( SumCount!=face->sum ) 
	{
		ClipStatus = 0;
		lpMaterial = &smRendMaterial[ face->Vertex[3] ];
		face->sum = SumCount;

		if ( lpMaterial->UseState & dwMatDispMask ) 
			return NULL;

		for( cnt=0; cnt<3 ; cnt ++ ) 
		{
			sv = &Vertex[ face->Vertex[cnt] ];
			if ( sv->sum!=SumCount ) {
				//����� ����������Ʈ�� �߰����� ���� ���ؽ�
				sv->sum = SumCount;
				sv->lpRendVertex = AddStageVertex( sv , lpMaterial );
			}
			RendFace[ nRendFace ].lpRendVertex[cnt] = sv->lpRendVertex;
			ClipStatus |= sv->lpRendVertex->ClipStatus;			//Ŭ���� ���� ����
		}

		RendFace[ nRendFace ].Matrial   = face->Vertex[3];		//��Ʈ���� ����
		RendFace[ nRendFace ].lpTexLink = face->lpTexLink;	//�ؽ�����ǥ���� ������ ����
		RendFace[ nRendFace ].ClipStatus = ClipStatus;



/*
	POINT3D p1,p2,p3,p;

	p.x = 25*64*fONE;
	p.z = 25*64*fONE;
	p.y = 500*fONE;
	
		sv = &Vertex[ face->Vertex[0] ];
		p1.x = sv->x;
		p1.y = sv->y;
		p1.z = sv->z;

		sv = &Vertex[ face->Vertex[1] ];
		p2.x = sv->x;
		p2.y = sv->y;
		p2.z = sv->z;

		sv = &Vertex[ face->Vertex[2] ];
		p3.x = sv->x;
		p3.y = sv->y;
		p3.z = sv->z;

		if ( GetPlaneProduct( &p1 ,&p2 , &p3 , &p )>0 )
			RendFace[ nRendFace ].ClipStatus = SMCLIP_DISPLAYOUT;

*/

		nRendFace ++;
		return &RendFace[ nRendFace-1 ];
	}

	return NULL;
}

//������Ʈ ���ؽ��� ������ ���ؽ� �߰� ( ���� ��ǥ �ҽ� ��� )
//�� �� �� : �� �� ��
smRENDVERTEX *smRENDER3D::AddObjectVertex ( smVERTEX *sv, DWORD dwObjHead )
{
	int rx,ry,rz;
	int x,y,z;
	int ad;
	DWORD ClipStatus;

		x = sv->x;
		y = sv->y;
		z = sv->z;

		if ( ObjectVertexScale ) {
			x = (x*ObjectVertexScale)/fONE;
			y = (y*ObjectVertexScale)/fONE;
			z = (z*ObjectVertexScale)/fONE;
		}

		x -= CameraPosiX;
		y -= CameraPosiY;
		z -= CameraPosiZ;

		//�߰��ϸ鼭 ī�޶� ��ȯ ��Ʈ������ ���Ͽ� ī�޶� ��ǥ�� ��ȯ�Ͽ� ����

		//######################################################################################
		//�� �� �� : �� �� ��
		__int64 x_64, y_64, z_64, rx_64, ry_64, rz_64;
		rx_64 = ry_64 = rz_64 = 0;

		x_64 = x;
		y_64 = y;
		z_64 = z;

		rx_64  = ((x_64 * mCamera._11) >> FLOATS_EM);
		rx_64 += ((y_64 * mCamera._21) >> FLOATS_EM);
		rx_64 += ((z_64 * mCamera._31) >> FLOATS_EM);

		ry_64  = ((x_64 * mCamera._12) >> FLOATS_EM);
		ry_64 += ((y_64 * mCamera._22) >> FLOATS_EM);
		ry_64 += ((z_64 * mCamera._32) >> FLOATS_EM);

		rz_64  = ((x_64 * mCamera._13) >> FLOATS_EM);
		rz_64 += ((y_64 * mCamera._23) >> FLOATS_EM);
		rz_64 += ((z_64 * mCamera._33) >> FLOATS_EM);
	
		x = int(rx_64);
		y = int(ry_64);
		z = int(rz_64);
		
		/*
		rx = x * mCamera._11 +
			 y * mCamera._21 +
			 z * mCamera._31 ;

		ry = x * mCamera._12 +
			 y * mCamera._22 +
			 z * mCamera._32 ;

		rz = x * mCamera._13 +
			 y * mCamera._23 +
			 z * mCamera._33 ;

		x = rx>>FLOATS_EM;
		y = ry>>FLOATS_EM;
		z = rz>>FLOATS_EM;

		///////////////////////////////

		rx = x * (mCamera._11>>1) +
			 y * (mCamera._21>>1) +
			 z * (mCamera._31>>1) ;

		ry = x * (mCamera._12>>1) +
			 y * (mCamera._22>>1) +
			 z * (mCamera._32>>1) ;

		rz = x * (mCamera._13>>1) +
			 y * (mCamera._23>>1) +
			 z * (mCamera._33>>1) ;

		x = rx>>(FLOATS_EM-1);
		y = ry>>(FLOATS_EM-1);
		z = rz>>(FLOATS_EM-1);
		*/
		//######################################################################################

		if ( DistortionMesh ) {

			rx = (((x>>1)+RendStatTime));
			ry = (((y>>1)+RendStatTime));
			rz = (((z>>1)+RendStatTime));

			rx = GetSin[ rx&ANGCLIP ];
			ry = GetSin[ ry&ANGCLIP ];
			rz = GetCos[ rz&ANGCLIP ];

			x += rx>>7;
			y += ry>>7;
			z += rz>>7;

		}

		RendVertex[ nRendVertex ].tx = x ;
		RendVertex[ nRendVertex ].ty = y ;
		RendVertex[ nRendVertex ].tz = z ;

		ClipStatus = 0;

		// Ŭ���� Z
		if ( z<=SMFLOAT_DIST_NEARZ ) ClipStatus |= SMCLIP_NEARZ;
		if ( z>SMFLOAT_DIST_FARZ ) ClipStatus |= SMCLIP_FARZ;
		//if ( z<=SMFLOAT_DIST_TEXCLIP ) ClipStatus |= SMCLIP_TEXTURE;
		//if ( z>0 ) {

			// 45�� Ŭ���� , �¿�
			ad = abs( x )<< SMSHIFT_PERSPECTIVE_WIDTH;

			if ( x<0 ) {
				if ( z<=ad ) ClipStatus |=SMCLIP_LEFT;
			}
			else {
				if ( z<=ad ) ClipStatus |=SMCLIP_RIGHT;
			}

			// 45�� Ŭ���� , ����
			ad = ( abs( y ) * SMMULT_PERSPECTIVE_HEIGHT ) >>FLOATNS;
			if ( y<0 ) {
				if ( z<=ad ) ClipStatus |=SMCLIP_BOTTOM;
			}
			else {
				if ( z<=ad ) ClipStatus |=SMCLIP_TOP;
			}

		//}
		RendVertex[ nRendVertex ].ClipStatus = ClipStatus;


//		POINT3D	pLight;
		int gShade;
		DWORD r,g,b;
		short *sColor;
/*
		// ����Ʈ ����
		pLight.x = -fONE;
		pLight.y = -fONE;
		pLight.z = fONE/2;
*/

		//######################################################################################
		//�� �� �� : �� �� ��
		if( dwObjHead & OBJ_HEAD_TYPE_NEW_NORMAL )
		{
			x = sv->nx;
			y = sv->ny;
			z = sv->nz;

			if( ObjectVertexScale )
			{
				x = (x*ObjectVertexScale) / fONE;
				y = (y*ObjectVertexScale) / fONE;
				z = (z*ObjectVertexScale) / fONE;
			}
			
			x -= CameraPosiX;
			y -= CameraPosiY;
			z -= CameraPosiZ;

			//
			x_64 = x;
			y_64 = y;
			z_64 = z;

			rx_64  = ((x_64 * mCamera._11) >> FLOATS_EM);
			rx_64 += ((y_64 * mCamera._21) >> FLOATS_EM);
			rx_64 += ((z_64 * mCamera._31) >> FLOATS_EM);

			ry_64  = ((x_64 * mCamera._12) >> FLOATS_EM);
			ry_64 += ((y_64 * mCamera._22) >> FLOATS_EM);
			ry_64 += ((z_64 * mCamera._32) >> FLOATS_EM);

			rz_64  = ((x_64 * mCamera._13) >> FLOATS_EM);
			rz_64 += ((y_64 * mCamera._23) >> FLOATS_EM);
			rz_64 += ((z_64 * mCamera._33) >> FLOATS_EM);
		
			x = int(rx_64);
			y = int(ry_64);
			z = int(rz_64);

			/*
			rx = x * mCamera._11 +
				 y * mCamera._21 +
				 z * mCamera._31 ;

			ry = x * mCamera._12 +
				 y * mCamera._22 +
				 z * mCamera._32 ;

			rz = x * mCamera._13 +
				 y * mCamera._23 +
				 z * mCamera._33 ;

			x = rx>>FLOATS_EM;
			y = ry>>FLOATS_EM;
			z = rz>>FLOATS_EM;
			*/

			RendVertex[ nRendVertex ].nx = x;
			RendVertex[ nRendVertex ].ny = y;
			RendVertex[ nRendVertex ].nz = z;
		}
		else
		{
			RendVertex[ nRendVertex ].nx = 0;
			RendVertex[ nRendVertex ].ny = 0;
			RendVertex[ nRendVertex ].nz = 0;
		}
		//######################################################################################


		//######################################################################################
		//�� �� �� : �� �� ��
		if( dwObjHead & OBJ_HEAD_TYPE_NEW_NORMAL )
		{
			x = sv->nx - sv->x;
			y = sv->ny - sv->y;
			z = sv->nz - sv->z;
			gShade = ((x * pLight.x) + (y * pLight.y) + (z * pLight.z)) >> FLOATNS;

			gShade = abs(gShade);
			gShade = gShade / 140;												//CONTRAST
			r = g = b = DEFAULT_BRIGHT-50;										// BRIGHT

			r += gShade;
			g += int(gShade * 0.93f);
			b += int(gShade * 0.84f); 
		}
		else
		{
			gShade = ((sv->nx * pLight.x) + (sv->ny * pLight.y) + (sv->nz * pLight.z)) >> FLOATNS;
			gShade /= DEFAULT_CONTRAST+64;										//CONTRAST
			gShade += DEFAULT_BRIGHT+48;										//BRIGHT

			r = g = b = gShade;
		}
		//######################################################################################
		
		sColor = RendVertex[ nRendVertex ].sLight;
		*sColor++ = (short)r;
		*sColor++ = (short)g;
		*sColor++ = (short)b;
		*sColor   = 255;

		nRendVertex ++;

	return &RendVertex[ nRendVertex-1 ];
}

//������Ʈ ���̽��� ������ ���̽� �߰�
smRENDFACE *smRENDER3D::AddObjectFace ( smFACE *face )
{
	int cnt;
	smRENDVERTEX *sv;
	DWORD ClipStatus;

		ClipStatus = 0;

		for( cnt=0; cnt<3 ; cnt ++ ) {
			sv = &RendVertex[ face->v[cnt] ];
			RendFace[ nRendFace ].lpRendVertex[cnt] = sv;
			ClipStatus |= sv->ClipStatus;					//Ŭ���� ���� ����
		}

		RendFace[ nRendFace ].Matrial   = face->v[3];		//��Ʈ���� ����
		RendFace[ nRendFace ].lpTexLink = face->lpTexLink;	//�ؽ�����ǥ���� ������ ����
		RendFace[ nRendFace ].ClipStatus = ClipStatus;

		nRendFace ++;
		return &RendFace[ nRendFace-1 ];

	return NULL;
}

//2D �̹��� ������Ʈ �߰�
smRENDFACE *smRENDER3D::AddFace2D ( smFACE2D *face )
{
	int x,y,z;
	//int rx , ry, rz;
	int width , height;

	smRENDVERTEX *v[4];
	smRENDFACE	 *rf;
	short		sColor[4];

	sColor[ SMC_A ] = face->Transparency;
	sColor[ SMC_R ] = face->r;
	sColor[ SMC_G ] = face->g;
	sColor[ SMC_B ] = face->b;

	x = face->x;
	y = face->y;
	z = face->z;

	//�߽� ��ǥ�� ī�޶� ��ǥ�� ��ȯ
	x -= CameraPosiX;
	y -= CameraPosiY;
	z -= CameraPosiZ;

	//######################################################################################
	//�� �� �� : �� �� ��		
	__int64 x_64, y_64, z_64, rx_64, ry_64, rz_64;
	rx_64 = ry_64 = rz_64 = 0;

	x_64 = x;
	y_64 = y;
	z_64 = z;

	rx_64  = ((x_64 * mCamera._11) >> FLOATS_EM);
	rx_64 += ((y_64 * mCamera._21) >> FLOATS_EM);
	rx_64 += ((z_64 * mCamera._31) >> FLOATS_EM);

	ry_64  = ((x_64 * mCamera._12) >> FLOATS_EM);
	ry_64 += ((y_64 * mCamera._22) >> FLOATS_EM);
	ry_64 += ((z_64 * mCamera._32) >> FLOATS_EM);

	rz_64  = ((x_64 * mCamera._13) >> FLOATS_EM);
	rz_64 += ((y_64 * mCamera._23) >> FLOATS_EM);
	rz_64 += ((z_64 * mCamera._33) >> FLOATS_EM);

	x = int(rx_64);
	y = int(ry_64);
	z = int(rz_64);

	/*
	rx = x * mCamera._11 +
		 y * mCamera._21 +
		 z * mCamera._31 ;

	ry = x * mCamera._12 +
		 y * mCamera._22 +
		 z * mCamera._32 ;

	rz = x * mCamera._13 +
		 y * mCamera._23 +
		 z * mCamera._33 ;

	x = rx>>FLOATS_EM;
	y = ry>>FLOATS_EM;
	z = rz>>FLOATS_EM;
	*/
	//######################################################################################

//	z -= 12*fONE;
	z -= 24*fONE;


	if ( z<SMFLOAT_DIST_NEARZ ) return NULL;

	width  = face->width >> 1;
	height = face->height >> 1;

	//������ ���ؽ� �߰� ( ��ǥ ���� �Է� )

	v[0] = AddRendVertex ( x-width , y-height , z, sColor );
	v[1] = AddRendVertex ( x+width , y-height , z, sColor );
	v[2] = AddRendVertex ( x-width , y+height , z, sColor );
	v[3] = AddRendVertex ( x+width , y+height , z, sColor );

	//ù��° ������ ( 1 - 2 - 3 );
	rf = &RendFace[ nRendFace++ ];
	rf->lpRendVertex[0] = v[0];
	rf->lpRendVertex[1] = v[1];
	rf->lpRendVertex[2] = v[2];
	rf->Matrial    = face->MatNum ;		//��Ʈ���� ����
	rf->ClipStatus = v[0]->ClipStatus|v[1]->ClipStatus|v[2]->ClipStatus;
	rf->lpTexLink  = 0;

	//�ؽ��� ����
	AddRendTempTexLink( rf , 0,	face->TexRect.left , face->TexRect.bottom,
								face->TexRect.right , face->TexRect.bottom,
								face->TexRect.left , face->TexRect.top );


	//�ι�° ������ ( 2 - 4 - 3 );
	rf = &RendFace[ nRendFace++ ];
	rf->lpRendVertex[0] = v[1];
	rf->lpRendVertex[1] = v[3];
	rf->lpRendVertex[2] = v[2];
	rf->Matrial    = face->MatNum ;		//��Ʈ���� ����
	rf->ClipStatus = v[1]->ClipStatus|v[2]->ClipStatus|v[3]->ClipStatus;
	rf->lpTexLink  = 0;

	//�ؽ��� ����
	AddRendTempTexLink( rf , 0,	face->TexRect.right , face->TexRect.bottom,
								face->TexRect.right , face->TexRect.top,
								face->TexRect.left , face->TexRect.top );


	return &RendFace[ nRendFace-1 ];
}

//2D �̹��� ������Ʈ �߰� ( ȸ�� ���� )
smRENDFACE *smRENDER3D::AddFace2D ( smFACE2D *face , int angle )
{
	int x,y,z;
	//int rx , ry, rz;
	int width , height;

	smRENDVERTEX *v[4];
	smRENDFACE	 *rf;
	short		sColor[4];

	sColor[ SMC_A ] = face->Transparency;
	sColor[ SMC_R ] = face->r;
	sColor[ SMC_G ] = face->g;
	sColor[ SMC_B ] = face->b;

	x = face->x;
	y = face->y;
	z = face->z;

	//�߽� ��ǥ�� ī�޶� ��ǥ�� ��ȯ
	x -= CameraPosiX;
	y -= CameraPosiY;
	z -= CameraPosiZ;

	//######################################################################################
	//�� �� �� : �� �� ��		
	__int64 x_64, y_64, z_64, rx_64, ry_64, rz_64;
	rx_64 = ry_64 = rz_64 = 0;

	x_64 = x;
	y_64 = y;
	z_64 = z;

	rx_64  = ((x_64 * mCamera._11) >> FLOATS_EM);
	rx_64 += ((y_64 * mCamera._21) >> FLOATS_EM);
	rx_64 += ((z_64 * mCamera._31) >> FLOATS_EM);

	ry_64  = ((x_64 * mCamera._12) >> FLOATS_EM);
	ry_64 += ((y_64 * mCamera._22) >> FLOATS_EM);
	ry_64 += ((z_64 * mCamera._32) >> FLOATS_EM);

	rz_64  = ((x_64 * mCamera._13) >> FLOATS_EM);
	rz_64 += ((y_64 * mCamera._23) >> FLOATS_EM);
	rz_64 += ((z_64 * mCamera._33) >> FLOATS_EM);

	x = int(rx_64);
	y = int(ry_64);
	z = int(rz_64);

	/*
	rx = x * mCamera._11 +
		 y * mCamera._21 +
		 z * mCamera._31 ;

	ry = x * mCamera._12 +
		 y * mCamera._22 +
		 z * mCamera._32 ;

	rz = x * mCamera._13 +
		 y * mCamera._23 +
		 z * mCamera._33 ;

	x = rx>>FLOATS_EM;
	y = ry>>FLOATS_EM;
	z = rz>>FLOATS_EM;
	*/
	//######################################################################################

//	z -= 12*fONE;
	z -= 24*fONE;


	if ( z<SMFLOAT_DIST_NEARZ ) return NULL;

	width  = face->width >> 1;
	height = face->height >> 1;


	//ȸ�� ����
	int		ex[4],ey[4];
	int		mx,my;
	int		sin,cos;

	sin = GetSin[angle&ANGCLIP]>>FLOATNS;
	cos = GetCos[angle&ANGCLIP]>>FLOATNS;

	mx = -width;	my = -height;
	ex[0] = (mx * cos - my * sin) >> FLOATNS;
    ey[0] = (mx * sin + my * cos) >> FLOATNS;

	mx = width;	my = -height;
	ex[1] = (mx * cos - my * sin) >> FLOATNS;
    ey[1] = (mx * sin + my * cos) >> FLOATNS;

	mx = -width;	my = +height;
	ex[2] = (mx * cos - my * sin) >> FLOATNS;
    ey[2] = (mx * sin + my * cos) >> FLOATNS;

	mx = width;	my = height;
	ex[3] = (mx * cos - my * sin) >> FLOATNS;
    ey[3] = (mx * sin + my * cos) >> FLOATNS;


	//������ ���ؽ� �߰� ( ��ǥ ���� �Է� )
	v[0] = AddRendVertex ( x+ex[0] , y+ey[0] , z, sColor );
	v[1] = AddRendVertex ( x+ex[1] , y+ey[1] , z, sColor );
	v[2] = AddRendVertex ( x+ex[2] , y+ey[2] , z, sColor );
	v[3] = AddRendVertex ( x+ex[3] , y+ey[3] , z, sColor );

	//ù��° ������ ( 1 - 2 - 3 );
	rf = &RendFace[ nRendFace++ ];
	rf->lpRendVertex[0] = v[0];
	rf->lpRendVertex[1] = v[1];
	rf->lpRendVertex[2] = v[2];
	rf->Matrial    = face->MatNum ;		//��Ʈ���� ����
	rf->ClipStatus = v[0]->ClipStatus|v[1]->ClipStatus|v[2]->ClipStatus;
	rf->lpTexLink  = 0;

	//�ؽ��� ����
	AddRendTempTexLink( rf , 0,	face->TexRect.left , face->TexRect.bottom,
								face->TexRect.right , face->TexRect.bottom,
								face->TexRect.left , face->TexRect.top );


	//�ι�° ������ ( 2 - 4 - 3 );
	rf = &RendFace[ nRendFace++ ];
	rf->lpRendVertex[0] = v[1];
	rf->lpRendVertex[1] = v[3];
	rf->lpRendVertex[2] = v[2];
	rf->Matrial    = face->MatNum ;		//��Ʈ���� ����
	rf->ClipStatus = v[1]->ClipStatus|v[2]->ClipStatus|v[3]->ClipStatus;
	rf->lpTexLink  = 0;

	//�ؽ��� ����
	AddRendTempTexLink( rf , 0,	face->TexRect.right , face->TexRect.bottom,
								face->TexRect.right , face->TexRect.top,
								face->TexRect.left , face->TexRect.top );


	return &RendFace[ nRendFace-1 ];
}

//2D �̹��� ������Ʈ �߰� ( ȭ����ǥ�� ���� �׸��� )
smRENDFACE *smRENDER3D::AddFace2D_Direct ( smFACE2D *face )
{
	int cnt;
	int x1,x2,y1,y2;

	smRENDVERTEX *v[4];
	smRENDFACE	 *rf;
	DWORD		dwColor;

	dwColor	= 0xFFFFFFFF;
	((BYTE *)&dwColor)[ SMC_A ] = face->Transparency;

	for( cnt =0; cnt<4; cnt++ ) {
		v[cnt] = &RendVertex[ nRendVertex++ ];
		v[cnt]->zb = smConvZ3D( float(face->z) );
		v[cnt]->rhw = 1;
		v[cnt]->ClipStatus = 0;
		((DWORD *)v[cnt]->bCol)[0] = dwColor;
		((DWORD *)v[cnt]->bSpe)[0] = 0;
	}

	x1 = face->x; x2 = x1+face->width ;
	y1 = face->y; y2 = y1+face->height;

	v[0]->xy[0] = (float)x1;
	v[0]->xy[1] = (float)y1;
	v[1]->xy[0] = (float)x2;
	v[1]->xy[1] = (float)y1;
	v[2]->xy[0] = (float)x1;
	v[2]->xy[1] = (float)y2;
	v[3]->xy[0] = (float)x2;
	v[3]->xy[1] = (float)y2;

	//ù��° ������ ( 1 - 2 - 3 );
	rf = &RendFace[ nRendFace++ ];
	rf->lpRendVertex[0] = v[0];
	rf->lpRendVertex[1] = v[1];
	rf->lpRendVertex[2] = v[2];
	rf->Matrial    = face->MatNum ;		//��Ʈ���� ����
	rf->ClipStatus = 0;
	rf->lpTexLink  = 0;

	//�ؽ��� ����
	AddRendTempTexLink( rf , 0,	face->TexRect.left , face->TexRect.top,
								face->TexRect.right , face->TexRect.top,
								face->TexRect.left , face->TexRect.bottom );


	//�ι�° ������ ( 2 - 4 - 3 );
	rf = &RendFace[ nRendFace++ ];
	rf->lpRendVertex[0] = v[1];
	rf->lpRendVertex[1] = v[3];
	rf->lpRendVertex[2] = v[2];
	rf->Matrial    = face->MatNum ;		//��Ʈ���� ����
	rf->ClipStatus = 0;
	rf->lpTexLink  = 0;

	//�ؽ��� ����
	AddRendTempTexLink( rf , 0,	face->TexRect.right , face->TexRect.top,
								face->TexRect.right , face->TexRect.bottom,
								face->TexRect.left , face->TexRect.bottom );


	return &RendFace[ nRendFace-1 ];
}


//�� �� �� : �� �� ��
//������ ���ؽ� �߰� ( ��ǥ ���� �Է� )
smRENDVERTEX *smRENDER3D::AddRendVertex ( int x, int y, int z, short *pLight, int nx, int ny, int nz )
{
	DWORD ClipStatus;
	int	ad;

	RendVertex[ nRendVertex ].tx = x ;
	RendVertex[ nRendVertex ].ty = y ;
	RendVertex[ nRendVertex ].tz = z ;

	//######################################################################################
	//�� �� �� : �� �� ��
	RendVertex[ nRendVertex ].nx = nx;
	RendVertex[ nRendVertex ].ny = ny;
	RendVertex[ nRendVertex ].nz = nz;
	//######################################################################################

	ClipStatus = 0;

	// Ŭ���� Z
	if ( z<SMFLOAT_DIST_NEARZ ) ClipStatus |= SMCLIP_NEARZ;
	if ( z>SMFLOAT_DIST_FARZ ) ClipStatus |= SMCLIP_FARZ;
	//if ( z<SMFLOAT_DIST_TEXCLIP ) ClipStatus |= SMCLIP_TEXTURE;
//	else {
		// 45�� Ŭ���� , �¿�
		ad = abs( x )<< SMSHIFT_PERSPECTIVE_WIDTH;
		if ( x<0 ) {
			if ( z<ad ) ClipStatus |=SMCLIP_LEFT;
		}
		else {
			if ( z<ad ) ClipStatus |=SMCLIP_RIGHT;
		}

		// 45�� Ŭ���� , ����
		ad = ( abs( y ) * SMMULT_PERSPECTIVE_HEIGHT ) >>FLOATNS;
		if ( y<0 ) {
			if ( z<ad ) ClipStatus |=SMCLIP_BOTTOM;
		}
		else {
			if ( z<ad ) ClipStatus |=SMCLIP_TOP;
		}
//	}

	RendVertex[ nRendVertex ].ClipStatus = ClipStatus;

	//����� ����ŧ�� ����
	DWORD *dwLight , *dwpLight;

	dwLight = (DWORD *)RendVertex[ nRendVertex ].sLight;
	dwpLight = (DWORD *)pLight;

	*dwLight++	= *dwpLight++;
	*dwLight	= *dwpLight;

	nRendVertex ++;

	return &RendVertex[ nRendVertex-1 ];
}

//������ ���̽� �߰� ( ���ؽ� ������ ���� �Է� )
smRENDFACE *smRENDER3D::AddRendFace ( smRENDVERTEX *v0, smRENDVERTEX *v1, smRENDVERTEX *v2, DWORD matrial, smTEXLINK *lpTexLink )
{
	RendFace[ nRendFace ].lpRendVertex[0] = v0;
	RendFace[ nRendFace ].lpRendVertex[1] = v1;
	RendFace[ nRendFace ].lpRendVertex[2] = v2;

	RendFace[ nRendFace ].Matrial   = matrial;			//��Ʈ���� ����
	RendFace[ nRendFace ].lpTexLink = lpTexLink;		//�ؽ�����ǥ���� ������ ����

	RendFace[ nRendFace ].ClipStatus = v0->ClipStatus|v1->ClipStatus|v2->ClipStatus;

	nRendFace ++;
	return &RendFace[ nRendFace-1 ];
}

//���̽� ���̵尪 ���� ( DotProduct )
int smRENDER3D::GetFaceSide( smRENDFACE *rendface )
{
	smRENDVERTEX	*v0;
	smRENDVERTEX	*v1;
	smRENDVERTEX	*v2;

	v0 = rendface->lpRendVertex[0];
	v1 = rendface->lpRendVertex[1];
	v2 = rendface->lpRendVertex[2];

	int temp1 , temp2, temp3;
	int tempX , tempY, tempZ;
	int tempA , tempB, tempC;

	tempX = v1->tx - v0->tx;
	tempY = v1->ty - v0->ty;
	tempZ = v1->tz - v0->tz;

	tempA = v2->tx - v0->tx;
	tempB = v2->ty - v0->ty;
	tempC = v2->tz - v0->tz;

	temp1 = ( ( tempY * tempC ) - ( tempZ * tempB ) )>>FLOATNS;   
	temp2 = ( ( tempX * tempC ) - ( tempZ * tempA ) )>>FLOATNS;   
	temp3 = ( ( tempX * tempB ) - ( tempY * tempA ) )>>FLOATNS;   

//	return ( temp1 * v1->tx ) + ( temp2 * v1->ty ) + ( temp3 * v1->tz );
	return temp3;
}

// ������ ���̽��� Ŭ���� ( dwClipMode�� SMCLIP_[����] ) 
int smRENDER3D::AddRendFaceClip( smRENDFACE *rendface , DWORD dwClipMode )
{

	smRENDVERTEX *v[3];
	smRENDVERTEX *cv[2];			//���� ���ؽ�
	float		 fMultPoint[2];
	float		 u0,v0 , u1,v1;

	int cnt;
	int StatsClips[3];
	int midcnt , nexcnt , lascnt;

	smTEXLINK	*texLink;
	smRENDFACE	*rFace[2];


	//���� ���̽��� ������� �ʴ´�
	rendface->ClipStatus |= SMCLIP_DISPLAYOUT;

	// ���̽��� ���� ������ ���� ��� ǥ�� ���� ���� ( NULL ������ �����ְ� ���� ���� )
	//if ( GetFaceSide( rendface )<0 ) return NULL;
	
	StatsClips[0] = rendface->lpRendVertex[0]->ClipStatus & dwClipMode ;
	StatsClips[1] = rendface->lpRendVertex[1]->ClipStatus & dwClipMode ;
	StatsClips[2] = rendface->lpRendVertex[2]->ClipStatus & dwClipMode ;

	//��� ���ؽ��� ȭ�� �ۿ� ��ġ �ϸ� ǥ������ ���� ( NULL ���� �����ְ� ���� ���� )
	if ( StatsClips[0] && StatsClips[1] && StatsClips[2] ) return NULL;

	v[0] = rendface->lpRendVertex[0];
	v[1] = rendface->lpRendVertex[1];
	v[2] = rendface->lpRendVertex[2];

	for( midcnt=0;midcnt<3;midcnt++) {
		nexcnt = midcnt+1;
		if ( nexcnt==3 ) nexcnt = 0;
		if ( StatsClips[midcnt]==0 && StatsClips[nexcnt]!=0 ) break;
	}

	lascnt = midcnt-1;
	if ( lascnt<0 ) lascnt = 2;

	if ( !StatsClips[lascnt] ) {
		//������ ������ 2�� ¥��
		switch( dwClipMode ) {
		case SMCLIP_LEFT:
			cv[0] = AddRendVertex_ClipLeft( rendface , midcnt, nexcnt ,  &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipLeft( rendface , lascnt, nexcnt ,  &fMultPoint[1] );
			break;
		case SMCLIP_RIGHT:
			cv[0] = AddRendVertex_ClipRight( rendface , midcnt, nexcnt ,  &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipRight( rendface , lascnt, nexcnt ,  &fMultPoint[1] );
			break;
		case SMCLIP_TOP:
			cv[0] = AddRendVertex_ClipTop( rendface , midcnt, nexcnt ,  &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipTop( rendface , lascnt, nexcnt ,  &fMultPoint[1] );
			break;
		case SMCLIP_BOTTOM:
			cv[0] = AddRendVertex_ClipBottom( rendface , midcnt, nexcnt ,  &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipBottom( rendface , lascnt, nexcnt ,  &fMultPoint[1] );
			break;
		case SMCLIP_NEARZ:
			cv[0] = AddRendVertex_ClipNearZ( rendface , midcnt, nexcnt ,  &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipNearZ( rendface , lascnt, nexcnt ,  &fMultPoint[1] );
			break;
		case SMCLIP_FARZ:
			cv[0] = AddRendVertex_ClipFarZ( rendface , midcnt, nexcnt ,  &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipFarZ( rendface , lascnt, nexcnt ,  &fMultPoint[1] );
			break;
		}

		// ���̽� �߰� 
		// ( v[midcnt] ) -> cv[0] -> v[lascnt]
		rFace[0] = AddRendFace( v[midcnt] , cv[0] , v[lascnt] , rendface->Matrial , NULL );
		// ( cv[0] ) ->  cv[1] -> v[lascnt]
		rFace[1] = AddRendFace( cv[0] , cv[1] , v[lascnt] , rendface->Matrial , NULL );


		// �ؽ��� ��ǥ�� ���������� ����
		texLink = rendface->lpTexLink;
		for( cnt=0;cnt<8;cnt++) {
			if ( !texLink ) break;
			u0 = texLink->u[midcnt] + ( texLink->u[nexcnt] - texLink->u[midcnt] ) * fMultPoint[0];
			v0 = texLink->v[midcnt] + ( texLink->v[nexcnt] - texLink->v[midcnt] ) * fMultPoint[0];
			u1 = texLink->u[lascnt] + ( texLink->u[nexcnt] - texLink->u[lascnt] ) * fMultPoint[1];
			v1 = texLink->v[lascnt] + ( texLink->v[nexcnt] - texLink->v[lascnt] ) * fMultPoint[1];

			// ( v[midcnt] ) -> tv[0] -> v[lascnt]
			AddRendTempTexLink( rFace[0] , texLink->hTexture , 
				texLink->u[midcnt] , texLink->v[midcnt] , u0 , v0 , texLink->u[lascnt] , texLink->v[lascnt] );

			// ( tv[0] ) ->  tv[1] -> v[lascnt]
			AddRendTempTexLink( rFace[1] , texLink->hTexture , 
				u0 , v0 , u1 , v1 , texLink->u[lascnt] , texLink->v[lascnt] );

			texLink = texLink->NextTex;
		}
		/*
		GetCrossLeft( tv[0] , face , cnt, nexcnt );
		GetCrossLeft( tv[1] , face , lascnt , nexcnt );
		*/

	}
	else {
		//������ ������ 1�� ¥��
		switch( dwClipMode ) {
		case SMCLIP_LEFT:
			cv[0] = AddRendVertex_ClipLeft( rendface , midcnt, nexcnt , &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipLeft( rendface , midcnt, lascnt , &fMultPoint[1] );
			break;
		case SMCLIP_RIGHT:
			cv[0] = AddRendVertex_ClipRight( rendface , midcnt, nexcnt , &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipRight( rendface , midcnt, lascnt , &fMultPoint[1] );
			break;
		case SMCLIP_TOP:
			cv[0] = AddRendVertex_ClipTop( rendface , midcnt, nexcnt , &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipTop( rendface , midcnt, lascnt , &fMultPoint[1] );
			break;
		case SMCLIP_BOTTOM:
			cv[0] = AddRendVertex_ClipBottom( rendface , midcnt, nexcnt , &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipBottom( rendface , midcnt, lascnt , &fMultPoint[1] );
			break;
		case SMCLIP_NEARZ:
			cv[0] = AddRendVertex_ClipNearZ( rendface , midcnt, nexcnt , &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipNearZ( rendface , midcnt, lascnt , &fMultPoint[1] );
			break;
		case SMCLIP_FARZ:
			cv[0] = AddRendVertex_ClipFarZ( rendface , midcnt, nexcnt , &fMultPoint[0] );
			cv[1] = AddRendVertex_ClipFarZ( rendface , midcnt, lascnt , &fMultPoint[1] );
			break;
		}

		// ���̽� �߰� 
		// ( v[midcnt] ) -> cv[0] -> cv[1]
		rFace[0] = AddRendFace( v[midcnt] , cv[0] , cv[1] , rendface->Matrial , NULL );

		// �ؽ��� ��ǥ�� ���������� ����
		texLink = rendface->lpTexLink;
		for( cnt=0;cnt<8;cnt++) {
			if ( !texLink ) break;
			u0 = texLink->u[midcnt] + ( texLink->u[nexcnt] - texLink->u[midcnt] ) * fMultPoint[0];
			v0 = texLink->v[midcnt] + ( texLink->v[nexcnt] - texLink->v[midcnt] ) * fMultPoint[0];
			u1 = texLink->u[midcnt] + ( texLink->u[lascnt] - texLink->u[midcnt] ) * fMultPoint[1];
			v1 = texLink->v[midcnt] + ( texLink->v[lascnt] - texLink->v[midcnt] ) * fMultPoint[1];

			// ( v[cnt] ) -> cv[0] -> cv[1]
			AddRendTempTexLink( rFace[0] , texLink->hTexture , 
				texLink->u[midcnt] , texLink->v[midcnt] , u0 , v0 , u1, v1 );

			texLink = texLink->NextTex;
		}

		/*

		GetCrossLeft( tv[0] , face , cnt, nexcnt );
		GetCrossLeft( tv[1] , face , cnt, lascnt );
		*/

	}

	return TRUE;
}

// ������ ���̽��� �ؽ��� Ŭ���� 
int smRENDER3D::AddRendFaceTextureClip( smRENDFACE *rendface )
{
	smRENDVERTEX *v[3];
	smRENDVERTEX *cv[2];			//���� ���ؽ�
	float		 fMultPoint[2];
	float		 u0,v0 , u1,v1;
	int cnt;
	int StatsClips[3];
	int midcnt , nexcnt , lascnt;

	smTEXLINK	*texLink;
	smRENDFACE	*rFace[3];

	StatsClips[0] = rendface->lpRendVertex[0]->ClipStatus & SMCLIP_TEXTURE ;
	StatsClips[1] = rendface->lpRendVertex[1]->ClipStatus & SMCLIP_TEXTURE ;
	StatsClips[2] = rendface->lpRendVertex[2]->ClipStatus & SMCLIP_TEXTURE ;

	//��� ���ؽ��� ȭ�� �ۿ� ��ġ �ϸ� ǥ������ ���� ( NULL ���� �����ְ� ���� ���� )
	if ( StatsClips[0] && StatsClips[1] && StatsClips[2] ) return NULL;

	//���� ���̽��� ������� �ʴ´�
	rendface->ClipStatus |= SMCLIP_DISPLAYOUT;

	v[0] = rendface->lpRendVertex[0];
	v[1] = rendface->lpRendVertex[1];
	v[2] = rendface->lpRendVertex[2];

	for( midcnt=0;midcnt<3;midcnt++) {
		nexcnt = midcnt+1;
		if ( nexcnt==3 ) nexcnt = 0;
		if ( StatsClips[midcnt]==0 && StatsClips[nexcnt]!=0 ) break;
	}

	lascnt = midcnt-1;
	if ( lascnt<0 ) lascnt = 2;

	if ( !StatsClips[lascnt] ) {
		//������ ������ 2�� ¥��
		cv[0] = AddRendVertex_ClipTexture( rendface , midcnt, nexcnt ,  &fMultPoint[0] );
		cv[1] = AddRendVertex_ClipTexture( rendface , lascnt, nexcnt ,  &fMultPoint[1] );

		// ���̽� �߰� 
		// ( v[midcnt] ) -> cv[0] -> v[lascnt]
		rFace[0] = AddRendFace( v[midcnt] , cv[0] , v[lascnt] , rendface->Matrial , NULL );
		// ( cv[0] ) ->  cv[1] -> v[lascnt]
		rFace[1] = AddRendFace( cv[0] , cv[1] , v[lascnt] , rendface->Matrial , NULL );
		// ( cv[0] ) -> v[nexcnt] -> cv[1]
		rFace[2] = AddRendFace( cv[0] , v[nexcnt] , cv[1] , rendface->Matrial , NULL );

		// �ؽ��� ��ǥ�� ���������� ����
		texLink = rendface->lpTexLink;
		for( cnt=0;cnt<8;cnt++) {
			if ( !texLink ) break;
			u0 = texLink->u[midcnt] + ( texLink->u[nexcnt] - texLink->u[midcnt] ) * fMultPoint[0];
			v0 = texLink->v[midcnt] + ( texLink->v[nexcnt] - texLink->v[midcnt] ) * fMultPoint[0];
			u1 = texLink->u[lascnt] + ( texLink->u[nexcnt] - texLink->u[lascnt] ) * fMultPoint[1];
			v1 = texLink->v[lascnt] + ( texLink->v[nexcnt] - texLink->v[lascnt] ) * fMultPoint[1];

			// ( v[midcnt] ) -> tv[0] -> v[lascnt]
			AddRendTempTexLink( rFace[0] , texLink->hTexture , 
				texLink->u[midcnt] , texLink->v[midcnt] , u0 , v0 , texLink->u[lascnt] , texLink->v[lascnt] );

			// ( tv[0] ) ->  tv[1] -> v[lascnt]
			AddRendTempTexLink( rFace[1] , texLink->hTexture , 
				u0 , v0 , u1 , v1 , texLink->u[lascnt] , texLink->v[lascnt] );

			// ( cv[0] ) -> v[nexcnt] ->  cv[1] 
			AddRendTempTexLink( rFace[2] , texLink->hTexture , 
				u0 , v0 , texLink->u[nexcnt] , texLink->v[nexcnt] , u1 , v1  );

			texLink = texLink->NextTex;
		}

	}
	else {
		//������ ������ 1�� ¥��
		cv[0] = AddRendVertex_ClipTexture( rendface , midcnt, nexcnt , &fMultPoint[0] );
		cv[1] = AddRendVertex_ClipTexture( rendface , midcnt, lascnt , &fMultPoint[1] );

		// ���̽� �߰� 
		// ( v[midcnt] ) -> cv[0] -> cv[1]
		rFace[0] = AddRendFace( v[midcnt] , cv[0] , cv[1] , rendface->Matrial , NULL );
		// cv[0] -> v[nexcnt] -> v[lascnt]
		rFace[1] = AddRendFace( cv[0] , v[nexcnt] , v[lascnt] , rendface->Matrial , NULL );
		// cv[0] -> v[lascnt] -> cv[1]
		rFace[2] = AddRendFace( cv[0] , v[lascnt] , cv[1] , rendface->Matrial , NULL );


		// �ؽ��� ��ǥ�� ���������� ����
		texLink = rendface->lpTexLink;
		for( cnt=0;cnt<8;cnt++) {
			if ( !texLink ) break;
			u0 = texLink->u[midcnt] + ( texLink->u[nexcnt] - texLink->u[midcnt] ) * fMultPoint[0];
			v0 = texLink->v[midcnt] + ( texLink->v[nexcnt] - texLink->v[midcnt] ) * fMultPoint[0];
			u1 = texLink->u[midcnt] + ( texLink->u[lascnt] - texLink->u[midcnt] ) * fMultPoint[1];
			v1 = texLink->v[midcnt] + ( texLink->v[lascnt] - texLink->v[midcnt] ) * fMultPoint[1];

			// ( v[midcnt] ) -> cv[0] -> cv[1]
			AddRendTempTexLink( rFace[0] , texLink->hTexture , 
				texLink->u[midcnt] , texLink->v[midcnt] , u0 , v0 , u1, v1 );

			// cv[0] -> v[nexcnt] -> v[lascnt]
			AddRendTempTexLink( rFace[1] , texLink->hTexture , 
				u0, v0, texLink->u[nexcnt] , texLink->v[nexcnt] , texLink->u[lascnt] , texLink->v[lascnt] );

			// cv[0] -> v[lascnt] -> cv[1]
			AddRendTempTexLink( rFace[2] , texLink->hTexture , 
				u0, v0, texLink->u[lascnt] , texLink->v[lascnt] , u1, v1 );

			texLink = texLink->NextTex;
		}

	}

	return TRUE;
}

//�� �� �� : �� �� ��
int smRENDER3D::SetDynamicObjLight( smRENDVERTEX *sv, int z )
{
	int cnt;

	int sx,sy,sz;
	int dist;
	int range,drange;
	int power;
	int r,g,b,a;
	int drg;

	if( z < ccDistZMin + (6*64) )
	{
		for( cnt=0; cnt < nRendObjLight; cnt++ )
		{
			sx = abs(sv->tx - smRendObjLight[cnt].rx);
			sy = abs(sv->ty - smRendObjLight[cnt].ry);
			sz = abs(sv->tz - smRendObjLight[cnt].rz);
			range = smRendObjLight[cnt].Range;
			drange = smRendObjLight[cnt].dRange;

			if( sx<range && sy<range && sz<range )
			{
				sx>>=FLOATNS;
				sy>>=FLOATNS;
				sz>>=FLOATNS;

				dist = sx*sx+sz*sz+sy*sy;
				if( dist < drange )
				{
					drg = (drange>>FLOATNS);
					if( drg == 0 ) 
						drg = 1;

					power = (drange-dist)/drg;
					r = (smRendObjLight[cnt].r * power)>>FLOATNS;
					g = (smRendObjLight[cnt].g * power)>>FLOATNS;
					b = (smRendObjLight[cnt].b * power)>>FLOATNS;
					a = (smRendObjLight[cnt].a * power)>>FLOATNS;
					AddLight( sv , r, g, b, a );

					if ( sv->sLight[SMC_R]>540 ) sv->sLight[SMC_R] = 540;
					if ( sv->sLight[SMC_G]>540 ) sv->sLight[SMC_G] = 540;
					if ( sv->sLight[SMC_B]>540 ) sv->sLight[SMC_B] = 540;
				}
			}
		}
	}

	return TRUE;
}

// ���̳��� ����Ʈ ����
int smRENDER3D::SetDynamicLight( smRENDVERTEX *sv , int z )
{
	int cnt;

	int sx,sy,sz;
	int dist;
	int range,drange;
	int power;
	int r,g,b,a;
	int drg;


	if ( z < ccDistZMin+(6*64) ) {
		for( cnt=0;cnt<nRendLight;cnt++) {
			sx = abs(sv->tx - smRendLight[cnt].rx);
			sy = abs(sv->ty - smRendLight[cnt].ry);
			sz = abs(sv->tz - smRendLight[cnt].rz);
			range = smRendLight[cnt].Range;
			drange = smRendLight[cnt].dRange;

			if ( sx<range && sy<range && sz<range ) {
				sx>>=FLOATNS;
				sy>>=FLOATNS;
				sz>>=FLOATNS;

				dist = sx*sx+sz*sz+sy*sy;
				if ( dist<drange ) {
					drg = (drange>>FLOATNS);
					if ( drg==0 ) drg=1;
					power = (drange-dist)/drg;
					r = (smRendLight[cnt].r * power)>>FLOATNS;
					g = (smRendLight[cnt].g * power)>>FLOATNS;
					b = (smRendLight[cnt].b * power)>>FLOATNS;
					a = (smRendLight[cnt].a * power)>>FLOATNS;
					AddLight( sv , r, g, b, a );

					if ( sv->sLight[SMC_R]>540 ) sv->sLight[SMC_R] = 540;
					if ( sv->sLight[SMC_G]>540 ) sv->sLight[SMC_G] = 540;
					if ( sv->sLight[SMC_B]>540 ) sv->sLight[SMC_B] = 540;

				}
			}
		}
	}

	return TRUE;
}

// Z ���� ���� ����ġ�� BackColor���� �������� �����Ѵ� 
int smRENDER3D::SetColorZclip( smRENDVERTEX *sv , int z )
{ 
	if ( z > ccDistZMin ) {
		int result;
		int lev;

		lev = (z-ccDistZMin) << ( 13 - ShiftMAPCELL_MULT );
		result = ShiftLight( sv, lev>>8, ccBackColor.r>>8 , ccBackColor.g>>8, ccBackColor.b>>8 ,0 );
		return result;
	}

	return FALSE;
}

// ���ؽ��� ������ �־��� ���� �������� �ܰ������� ����
int smRENDER3D::ShiftLight( smRENDVERTEX *sv , int lev , int r,int g,int b, int a )
{
	int cFlag;
	int dlev;

	int col,spe;

	if ( lev>=256 ) {
		lev=255;
		cFlag = 1;
	}
	else
		cFlag = 0;

	dlev = lev;

	// RED
	col = sv->bCol[0];
	spe = sv->bSpe[0];
	col -= ( col * dlev )>>8;
	spe += ( ( r-spe ) * dlev )>>8;
	sv->bCol[0]=col;
	sv->bSpe[0]=spe;

	// GREEN
	col = sv->bCol[1];
	spe = sv->bSpe[1];
	col -= ( col * dlev )>>8;
	spe += ( ( g-spe ) * dlev )>>8;
	sv->bCol[1]=col;
	sv->bSpe[1]=spe;

	// BLUE
	col = sv->bCol[2];
	spe = sv->bSpe[2];
	col -= ( col * dlev )>>8;
	spe += ( ( b-spe ) * dlev )>>8;
	sv->bCol[2]=col;
	sv->bSpe[2]=spe;

	// ALPHA
	col = sv->bCol[3];
	spe = sv->bSpe[3];
	col -= ( col * dlev )>>8;
	spe += ( ( a-spe ) * dlev )>>8;
	sv->bCol[3]=col;
	sv->bSpe[3]=spe;

	if ( cFlag ) return TRUE;

	return FALSE;
}

//sLight ���� ���Ѵ�
int smRENDER3D::AddLight( smRENDVERTEX *sv, int r,int g,int b , int a )
{

	sv->sLight[ SMC_R ] += r;
	sv->sLight[ SMC_G ] += g;
	sv->sLight[ SMC_B ] += b;
	sv->sLight[ SMC_A ] += a;

	return TRUE;
}

//�������ؽ��� sLight�� Color �� Specular �� ���� �Ѵ�
int smRENDER3D::SetRendColor( smRENDVERTEX *sv )
{
	int col , spe;
	int cnt;

	for( cnt=0;cnt<4;cnt++ ) {
		col = sv->sLight[ cnt ];
		spe = 0;
		if ( col<0 )	{ col=0; spe=0; }
		if ( col>255 )	{ spe=col-256; col=255; }
		if ( spe>255 )  spe = 255;
		sv->bCol[ cnt ] = col;
		sv->bSpe[ cnt ] = spe;
	}

	return TRUE;
}

//��Ʈ���� ����Ʈ�� ��� �߰�
int smRENDER3D::AddRendMaterialList( int MatNum )
{

	if ( !RendMatrial[ MatNum ].RegistRendList ) {
		if ( smRendMaterial[MatNum].MapOpacity==TRUE || smRendMaterial[MatNum].Transparency!=0 ) {
			RendMatrialList[ RMatListRearCnt-- ] = MatNum;
		}
		else {
			RendMatrialList[ RMatListCnt++ ] = MatNum;
		}

		RendMatrial[ MatNum ].RegistRendList = TRUE;
	}

	return TRUE;

}

// ��ü ������ ���̽��� Ŭ����  
int smRENDER3D::ClipRendFace()
{
	int cnt;
	DWORD ClipStatus;
	smRENDMATRIAL	*rmat;
	int nrface;

	int ClipCnt;
	DWORD ClipValue;
	smTEXTUREHANDLE	*smTex;



	ClipValue = 1;

	for( ClipCnt=0;ClipCnt<6;ClipCnt++) {
		if ( ClipStates&ClipValue ) {
			for( cnt=0; cnt<nRendFace; cnt++ ) {
				ClipStatus = RendFace[ cnt ].ClipStatus;
				if ( !(ClipStatus&SMCLIP_DISPLAYOUT) && (ClipStatus&ClipValue) ) {
					AddRendFaceClip( &RendFace[cnt], ClipValue );
				}
			}
		}
		ClipValue<<=1;



		// FARZ Ŭ�� ( Alpha�� ��Ʈ������ ��� �ϴ� ��츸 �ش� �ǰ� ���� )
		if ( ClipValue==SMCLIP_FARZ )
		{
			//######################################################################################
			//�� �� �� : �� �� ��
			if( m_GameFieldView == 0 || m_GameFieldViewStep == 1 )
			{
				for( cnt=0; cnt<nRendFace; cnt++ )
				{
					smTex = smRendMaterial[ RendFace[cnt].Matrial ].smTexture[0];
					if ( smTex && smTex->NameA[0] )
					{
						ClipStatus = RendFace[ cnt ].ClipStatus;
						if ( !(ClipStatus&SMCLIP_DISPLAYOUT) && (ClipStatus&ClipValue) )
							AddRendFaceClip( &RendFace[cnt], ClipValue );
					}
				}
			}
			//######################################################################################
			ClipValue<<=1;
			ClipCnt++;
		}
	}


	// TEXTURE Ŭ���� 
	nrface = nRendFace;

	for( cnt=0; cnt<nrface; cnt++ ) {
		ClipStatus = RendFace[ cnt ].ClipStatus;
		if ( !(ClipStatus&SMCLIP_DISPLAYOUT) && (ClipStatus&ClipValue) ) {
			if ( smRendMaterial[ RendFace[cnt].Matrial ].MatFrame!=MaterialFrame ) 
				ReSwapMaterial( &smRendMaterial[ RendFace[cnt].Matrial ] );
			//######################################################################################
			//�� �� �� : �� �� ��
			//if ( smRendMaterial[ RendFace[cnt].Matrial ].TextureClip )
			//	AddRendFaceTextureClip( &RendFace[cnt] );
			//######################################################################################
		}
	}

	int MatNum;

	for( cnt=0; cnt<nRendFace; cnt++ ) {
		ClipStatus = RendFace[ cnt ].ClipStatus;
		if ( !(ClipStatus&SMCLIP_DISPLAYOUT) ) {
						//��Ʈ���� �� ������ ���̽� ����
						MatNum = RendFace[cnt].Matrial; 
						rmat = &RendMatrial[ MatNum  ];

						//######################################################################################
						//�� �� �� : �� �� ��
						/*if ( RendFace[cnt].ClipStatus&SMCLIP_TEXTURE && smRendMaterial[ MatNum ].TextureClip ) {

							//�ش� ��Ʈ������ ���̽�ī���� ����
							rmat->TexClip_MatrialCounter ++;
							if ( !rmat->TexClip_LastLinkFace ) {
								//����� ��Ʈ������ ����
								rmat->TexClip_StartFace = &RendFace[cnt];
								rmat->TexClip_LastLinkFace = &RendFace[cnt];
								AddRendMaterialList( MatNum );
							}
							else {
								//���� ��Ʈ���� ���̽��� ����
								rmat->TexClip_LastLinkFace->NexRendFace = &RendFace[cnt];
								rmat->TexClip_LastLinkFace = &RendFace[cnt];
							}

						}
						else*/
						//######################################################################################
						{

							//�ش� ��Ʈ������ ���̽�ī���� ����
							rmat->MatrialCounter ++;
							if ( !rmat->LastLinkFace ) {
								//����� ��Ʈ������ ����
								rmat->StartFace = &RendFace[cnt];
								rmat->LastLinkFace = &RendFace[cnt];
								AddRendMaterialList( MatNum );
							}
							else {
								//���� ��Ʈ���� ���̽��� ����
								rmat->LastLinkFace->NexRendFace = &RendFace[cnt];
								rmat->LastLinkFace = &RendFace[cnt];
							}
						}


						RendFace[ cnt ].lpRendVertex[0]->ClipStatus |= SMCLIP_DISPLAYIN;
						RendFace[ cnt ].lpRendVertex[1]->ClipStatus |= SMCLIP_DISPLAYIN;
						RendFace[ cnt ].lpRendVertex[2]->ClipStatus |= SMCLIP_DISPLAYIN;
		}
	}


	return TRUE;
}

// ���ؽ��� 2D��ǥ�� ��ȯ  
int smRENDER3D::GeomVertex2D( int ColorEffect )
{

	//######################################################################################
	//�� �� �� : �� �� ��
	int				 cnt, z;
	smRENDVERTEX	*rv;

	if( ColorEffect == TRUE )
	{
		//���� �� ǥ�� ����
		for( cnt=0; cnt < nRendVertex; cnt++ )
		{
			rv = &RendVertex[ cnt ];
			//Ŭ���ο� �ش����� �ʴ� ���ؽ��� 2D��ǥ�� ���ú�ȯ�Ѵ�
			if ( rv->ClipStatus & SMCLIP_DISPLAYIN )
			{
				z = int( float(rv->tz) / fONE );
				DxProjectionWork( rv );

				//######################################################################################
				//�� �� �� : �� �� ��
				if( bApplyRendObjLight && nRendObjLight )
					SetDynamicObjLight( rv, z );
				//######################################################################################

				if ( nRendLight ) SetDynamicLight( rv , z );		// ���̳��� ����Ʈ ����
				AddLight( rv, Color_R , Color_G , Color_B, Color_A );
				SetRendColor( rv );

				SetColorZclip( rv , z);							// ���Ÿ� ���� ����
			}
		}
	}
	else
	{
		for( cnt=0; cnt < nRendVertex; cnt++ )
		{
			rv = &RendVertex[ cnt ];
			//Ŭ���ο� �ش����� �ʴ� ���ؽ��� 2D��ǥ�� ���ú�ȯ�Ѵ�
			if ( rv->ClipStatus & SMCLIP_DISPLAYIN )
			{
				z = int( float(rv->tz) / fONE );
				DxProjectionWork( rv );

				//######################################################################################
				//�� �� �� : �� �� ��
				if( bApplyRendObjLight && nRendObjLight )
					SetDynamicObjLight( rv, z );
				//######################################################################################

				if ( nRendLight ) SetDynamicLight( rv , z );		// ���̳��� ����Ʈ ����
				AddLight( rv, Color_R , Color_G , Color_B, Color_A );
				SetRendColor( rv );
			}
		}
	}	
	//######################################################################################


	/*	
	//////////////////////////////////////////////////////////////////////////////////////////////	
	int				cnt;
	int				x,y,z;
	float			fz;
	smRENDVERTEX	*rv;

	if ( ColorEffect==TRUE ) {
		//���� �� ǥ�� ����
		for( cnt=0;cnt<nRendVertex;cnt++ ) {
			rv = &RendVertex[ cnt ];
			//Ŭ���ο� �ش����� �ʴ� ���ؽ��� 2D��ǥ�� ���ú�ȯ�Ѵ�
			if ( rv->ClipStatus & SMCLIP_DISPLAYIN ) {
				x = rv->tx ;
				y = rv->ty ;
				z = rv->tz ;
				fz = (float)z/fONE;

				rv->xy[0] = MidX + ( (float)( x * viewdistZ ))/ (float)z ;			// 2D X
				rv->xy[1] = MidY - ( (float)( y * viewdistZ ))/ (float)z ;			// 2D Y

				z >>= FLOATNS;
				rv->zb = smConvZ3D( fz );						// Z Buffer Depth
				rv->rhw = 1/fz;									// RHW

				//######################################################################################
				//�� �� �� : �� �� ��
				if( bApplyRendObjLight && nRendObjLight )
					SetDynamicObjLight( rv, z );
				//######################################################################################

				if ( nRendLight ) SetDynamicLight( rv , z );		// ���̳��� ����Ʈ ����
				AddLight( rv, Color_R , Color_G , Color_B, Color_A );
				SetRendColor( rv );

				SetColorZclip( rv , z);							// ���Ÿ� ���� ����

			}
		}
	}
	else {
		for( cnt=0;cnt<nRendVertex;cnt++ ) {
			rv = &RendVertex[ cnt ];
			//Ŭ���ο� �ش����� �ʴ� ���ؽ��� 2D��ǥ�� ���ú�ȯ�Ѵ�
			if ( rv->ClipStatus & SMCLIP_DISPLAYIN ) {
				x = rv->tx ;
				y = rv->ty ;
				z = rv->tz ;
				fz = (float)z/fONE;

				rv->xy[0] = MidX + ( (float)( x * viewdistZ ))/ (float)z ;			// 2D X
				rv->xy[1] = MidY - ( (float)( y * viewdistZ ))/ (float)z ;			// 2D Y

				z >>= FLOATNS;
				rv->zb = smConvZ3D( fz );								// Z Buffer Depth
				rv->rhw = 1/fz;											// RHW

				//######################################################################################
				//�� �� �� : �� �� ��
				if( bApplyRendObjLight && nRendObjLight )
					SetDynamicObjLight( rv, z );
				//######################################################################################

				if ( nRendLight ) SetDynamicLight( rv , z );		// ���̳��� ����Ʈ ����
				AddLight( rv, Color_R , Color_G , Color_B, Color_A );
				SetRendColor( rv );

			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	*/
	

	return TRUE;
}

// ������ Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
smRENDVERTEX * smRENDER3D::AddRendVertex_ClipLeft( smRENDFACE *rendface , int a, int b , float *fMultPoint )
{
	int		x1 , x2;
	int		z1 , z2;
	int		s1 , s2;
	float	fMult;
	int		Mult;
	int		cnt;

	int		x,y,z;
	short	sCol[4];

	smRENDVERTEX *t1;
	smRENDVERTEX *t2;


	// 45�� ���� ���а� ������ ���� �������� ���Ͽ� ���� 
	t1 = rendface->lpRendVertex[a];
	t2 = rendface->lpRendVertex[b];

	x1 = t1->tx   ;
	z1 = -(x1<<SMSHIFT_PERSPECTIVE_WIDTH) ;
	x2 = t2->tx   ;
	z2 = -(x2<<SMSHIFT_PERSPECTIVE_WIDTH) ;

	s1 = abs( t1->tz - z1 );
	s2 = abs( t2->tz - z2 );

	// �������� ���� ���� ������ ��
	fMult = (float)((1/((double)(s2 + s1)))*(double)s1);
	Mult  = (int)( fMult * smCLIP_DETAIL );

	// ������ ��ǥ 
	x = t1->tx + ( ( ( t2->tx - t1->tx ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	y = t1->ty + ( ( ( t2->ty - t1->ty ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	z = (-x)<<SMSHIFT_PERSPECTIVE_WIDTH;

	// ������ ���� 
	for( cnt=0;cnt<4;cnt++ ) {
		sCol[cnt] = (short)((int)t1->sLight[cnt] + (( ( (int)t2->sLight[cnt] - (int)t1->sLight[cnt] ) * Mult )>>smCLIP_DETAIL_SHIFT ) );
	}

/*
	x = t1->tx + (int)( ( (double)( t2->tx - t1->tx ) * fMult ));
	y = t1->ty + (int)( ( (double)( t2->ty - t1->ty ) * fMult ));
	z = (-x)<<SMSHIFT_PERSPECTIVE_WIDTH;
*/

	*fMultPoint = fMult;

	//######################################################################################
	//�� �� �� : �� �� ��
	// ������ ���ؽ��� ������ ���ؽ��� �߰�
	//return AddRendVertex ( x, y, z, sCol );
	int nx = t2->nx - t2->tx;
	int ny = t2->ny - t2->ty;
	int nz = t2->nz - t2->tz;
	return AddRendVertex ( x, y, z, sCol, x+nx, y+ny, z+nz );
	//######################################################################################

}

// ������ Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
smRENDVERTEX * smRENDER3D::AddRendVertex_ClipRight( smRENDFACE *rendface , int a, int b , float *fMultPoint )
{
	int		x1 , x2;
	int		z1 , z2;
	int		s1 , s2;
	float	fMult;
	int		Mult;
	int		cnt;

	int		x,y,z;
	short	sCol[4];

	smRENDVERTEX *t1;
	smRENDVERTEX *t2;


	// 45�� ���� ���а� ������ ���� �������� ���Ͽ� ���� 
	t1 = rendface->lpRendVertex[a];
	t2 = rendface->lpRendVertex[b];

	x1 = t1->tx   ;
	z1 = (x1<<SMSHIFT_PERSPECTIVE_WIDTH) ;
	x2 = t2->tx   ;
	z2 = (x2<<SMSHIFT_PERSPECTIVE_WIDTH) ;

	s1 = abs( t1->tz - z1 );
	s2 = abs( t2->tz - z2 );

	// �������� ���� ���� ������ ��
	fMult = (float)((1/((double)(s2 + s1)))*(double)s1);
	Mult  = (int)( fMult * smCLIP_DETAIL );

	// ������ ��ǥ 
	x = t1->tx + ( ( ( t2->tx - t1->tx ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	y = t1->ty + ( ( ( t2->ty - t1->ty ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	z = x<<SMSHIFT_PERSPECTIVE_WIDTH ;

	// ������ ���� 
	for( cnt=0;cnt<4;cnt++ ) {
		sCol[cnt] = (short)((int)t1->sLight[cnt] + (( ( (int)t2->sLight[cnt] - (int)t1->sLight[cnt] ) * Mult )>>smCLIP_DETAIL_SHIFT) );
	}

/*
	x = t1->tx + ( ( (double)( t2->tx - t1->tx ) * fMult ) );
	y = t1->ty + ( ( (double)( t2->ty - t1->ty ) * fMult ) );
	z = x<<SMSHIFT_PERSPECTIVE_WIDTH ;
*/

	*fMultPoint = fMult;

	//######################################################################################
	//�� �� �� : �� �� ��
	// ������ ���ؽ��� ������ ���ؽ��� �߰�
	//return AddRendVertex ( x, y, z, sCol );
	int nx = t2->nx - t2->tx;
	int ny = t2->ny - t2->ty;
	int nz = t2->nz - t2->tz;
	return AddRendVertex ( x, y, z, sCol, x+nx, y+ny, z+nz );
	//######################################################################################
}
	
// �Ʒ��� Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
smRENDVERTEX * smRENDER3D::AddRendVertex_ClipBottom( smRENDFACE *rendface , int a, int b , float *fMultPoint )
{
	int		y1 , y2;
	int		z1 , z2;
	int		s1 , s2;
	float	fMult;
	int		Mult;
	int		cnt;

	int		x,y,z;
	short	sCol[4];

	smRENDVERTEX *t1;
	smRENDVERTEX *t2;



	// 45�� ���� ���а� ������ ���� �������� ���Ͽ� ���� 
	t1 = rendface->lpRendVertex[a];
	t2 = rendface->lpRendVertex[b];

	y1 = t1->ty   ;
	z1 = -( ( y1 * SMMULT_PERSPECTIVE_HEIGHT ) >> FLOATNS );
	y2 = t2->ty   ;
	z2 = -( ( y2 * SMMULT_PERSPECTIVE_HEIGHT ) >> FLOATNS );

	s1 = abs( t1->tz - z1 );
	s2 = abs( t2->tz - z2 );

	// �������� ���� ���� ������ ��
	fMult = (float)((1/((double)(s2 + s1)))*(double)s1);
//	Mult  = (int)( fMult * fONE );
	Mult  = (int)( fMult * smCLIP_DETAIL );

	// ������ ��ǥ 
	x = t1->tx + ( ( ( t2->tx - t1->tx ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	y = t1->ty + ( ( ( t2->ty - t1->ty ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	z = ((-y) * SMMULT_PERSPECTIVE_HEIGHT ) >> FLOATNS;

	// ������ ���� 
	for( cnt=0;cnt<4;cnt++ ) {
		sCol[cnt] = (short)((int)t1->sLight[cnt] + (( ( (int)t2->sLight[cnt] - (int)t1->sLight[cnt] ) * Mult )>>smCLIP_DETAIL_SHIFT ) );
	}

/*
	x = t1->tx + (int)( ( (double)( t2->tx - t1->tx ) * fMult )  );
	y = t1->ty + (int)( ( (double)( t2->ty - t1->ty ) * fMult )  );
	z = ((-y) * SMMULT_PERSPECTIVE_HEIGHT ) >> FLOATNS;
*/


	*fMultPoint = fMult;

	//######################################################################################
	//�� �� �� : �� �� ��
	// ������ ���ؽ��� ������ ���ؽ��� �߰�
	//return AddRendVertex ( x, y, z, sCol );
	int nx = t2->nx - t2->tx;
	int ny = t2->ny - t2->ty;
	int nz = t2->nz - t2->tz;
	return AddRendVertex ( x, y, z, sCol, x+nx, y+ny, z+nz );
	//######################################################################################
}

// ���� Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
smRENDVERTEX * smRENDER3D::AddRendVertex_ClipTop( smRENDFACE *rendface , int a, int b , float *fMultPoint )
{
	int		y1 , y2;
	int		z1 , z2;
	int		s1 , s2;
	float	fMult;
	int		Mult;
	int		cnt;

	int		x,y,z;
	short	sCol[4];

	smRENDVERTEX *t1;
	smRENDVERTEX *t2;



	// 45�� ���� ���а� ������ ���� �������� ���Ͽ� ���� 
	t1 = rendface->lpRendVertex[a];
	t2 = rendface->lpRendVertex[b];

	y1 = t1->ty   ;
	z1 = ( ( y1 * SMMULT_PERSPECTIVE_HEIGHT ) >> FLOATNS );
	y2 = t2->ty   ;
	z2 = ( ( y2 * SMMULT_PERSPECTIVE_HEIGHT ) >> FLOATNS );

	s1 = abs( t1->tz - z1 );
	s2 = abs( t2->tz - z2 );

	// �������� ���� ���� ������ ��
	fMult = (float)((1/((double)(s2 + s1)))*(double)s1);
//	Mult  = (int)( fMult * fONE );
	Mult  = (int)( fMult * smCLIP_DETAIL );

	// ������ ��ǥ 
	x = t1->tx + ( ( ( t2->tx - t1->tx ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	y = t1->ty + ( ( ( t2->ty - t1->ty ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	z = y<<SMSHIFT_PERSPECTIVE_WIDTH;
	z = ( y * SMMULT_PERSPECTIVE_HEIGHT ) >> FLOATNS;

/*
	x = t1->tx + (int)( ( (double)( t2->tx - t1->tx ) * fMult ) );
	y = t1->ty + (int)( ( (double)( t2->ty - t1->ty ) * fMult ) );
	z = y<<SMSHIFT_PERSPECTIVE_WIDTH;
	z = ( y * SMMULT_PERSPECTIVE_HEIGHT ) >> FLOATNS;
*/

	// ������ ���� 
	for( cnt=0;cnt<4;cnt++ ) {
		sCol[cnt] = (short)((int)t1->sLight[cnt] + (( ( (int)t2->sLight[cnt] - (int)t1->sLight[cnt] ) * Mult )>>smCLIP_DETAIL_SHIFT ) );
	}

	*fMultPoint = fMult;

	//######################################################################################
	//�� �� �� : �� �� ��
	// ������ ���ؽ��� ������ ���ؽ��� �߰�
	//return AddRendVertex ( x, y, z, sCol );
	int nx = t2->nx - t2->tx;
	int ny = t2->ny - t2->ty;
	int nz = t2->nz - t2->tz;
	return AddRendVertex ( x, y, z, sCol, x+nx, y+ny, z+nz );
	//######################################################################################
}

// Z NEAR Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
smRENDVERTEX * smRENDER3D::AddRendVertex_ClipNearZ( smRENDFACE *rendface , int a, int b , float *fMultPoint )
{
	int		s1 , s2;
	float	fMult;
	int		Mult;
	int		cnt;

	int		x,y,z;
	short	sCol[4];

	smRENDVERTEX *t1;
	smRENDVERTEX *t2;



	// 45�� ���� ���а� ������ ���� �������� ���Ͽ� ���� 
	t1 = rendface->lpRendVertex[a];
	t2 = rendface->lpRendVertex[b];

	s1 = abs( t1->tz - SMFLOAT_DIST_NEARZ );
	s2 = abs( t2->tz - SMFLOAT_DIST_NEARZ );

	// �������� ���� ���� ������ ��
	fMult = (float)((1/((double)(s2 + s1)))*(double)s1);
//	Mult  = (int)( fMult * fONE );
	Mult  = (int)( fMult * smCLIP_DETAIL );

	// ������ ��ǥ 
	x = t1->tx + ( ( ( t2->tx - t1->tx ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	y = t1->ty + ( ( ( t2->ty - t1->ty ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	z = SMFLOAT_DIST_NEARZ;

	// ������ ���� 
	for( cnt=0;cnt<4;cnt++ ) {
		sCol[cnt] = (short)((int)t1->sLight[cnt] + (( ( (int)t2->sLight[cnt] - (int)t1->sLight[cnt] ) * Mult )>>smCLIP_DETAIL_SHIFT ) );
	}

	*fMultPoint = fMult;

	//######################################################################################
	//�� �� �� : �� �� ��
	// ������ ���ؽ��� ������ ���ؽ��� �߰�
	//return AddRendVertex ( x, y, z, sCol );
	int nx = t2->nx - t2->tx;
	int ny = t2->ny - t2->ty;
	int nz = t2->nz - t2->tz;
	return AddRendVertex ( x, y, z, sCol, x+nx, y+ny, z+nz );
	//######################################################################################
}

// Z FAR Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
smRENDVERTEX * smRENDER3D::AddRendVertex_ClipFarZ( smRENDFACE *rendface , int a, int b , float *fMultPoint )
{
	int		s1 , s2;
	float	fMult;
	int		Mult;
	int		cnt;

	int		x,y,z;
	short	sCol[4];

	smRENDVERTEX *t1;
	smRENDVERTEX *t2;


	t1 = rendface->lpRendVertex[a];
	t2 = rendface->lpRendVertex[b];

	s1 = abs( t1->tz - SMFLOAT_DIST_FARZ );
	s2 = abs( t2->tz - SMFLOAT_DIST_FARZ );

	// �������� ���� ���� ������ ��
	fMult = (float)((1/((double)(s2 + s1)))*(double)s1);
	Mult  = (int)( fMult * smCLIP_DETAIL );

	// ������ ��ǥ 
	x = t1->tx + ( ( ( t2->tx - t1->tx ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	y = t1->ty + ( ( ( t2->ty - t1->ty ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	z = SMFLOAT_DIST_FARZ;

	// ������ ���� 
	for( cnt=0;cnt<4;cnt++ ) {
		sCol[cnt] = (short)((int)t1->sLight[cnt] + (( ( (int)t2->sLight[cnt] - (int)t1->sLight[cnt] ) * Mult )>>smCLIP_DETAIL_SHIFT ) );
	}

	*fMultPoint = fMult;

	//######################################################################################
	//�� �� �� : �� �� ��
	// ������ ���ؽ��� ������ ���ؽ��� �߰�
	//return AddRendVertex ( x, y, z, sCol );
	int nx = t2->nx - t2->tx;
	int ny = t2->ny - t2->ty;
	int nz = t2->nz - t2->tz;
	return AddRendVertex ( x, y, z, sCol, x+nx, y+ny, z+nz );
	//######################################################################################
}

// TEXTURE Ŭ�������� ���Ͽ� ������ ���ؽ��� �߰��Ѵ� ( fMultPoint ���� ��ǥ�� ���ϱ� ���� �������� ��  ) 
smRENDVERTEX * smRENDER3D::AddRendVertex_ClipTexture( smRENDFACE *rendface , int a, int b , float *fMultPoint )
{
	int		s1 , s2;
	float	fMult;
	int		Mult;
	int		cnt;

	int		x,y,z;
	short	sCol[4];

	smRENDVERTEX *t1;
	smRENDVERTEX *t2;



	// 45�� ���� ���а� ������ ���� �������� ���Ͽ� ���� 
	t1 = rendface->lpRendVertex[a];
	t2 = rendface->lpRendVertex[b];

	s1 = abs( t1->tz - SMFLOAT_DIST_TEXCLIP );
	s2 = abs( t2->tz - SMFLOAT_DIST_TEXCLIP );

	// �������� ���� ���� ������ ��
	fMult = (float)((1/((double)(s2 + s1)))*(double)s1);
	Mult  = (int)( fMult * smCLIP_DETAIL );

	// ������ ��ǥ 
	x = t1->tx + ( ( ( t2->tx - t1->tx ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	y = t1->ty + ( ( ( t2->ty - t1->ty ) * Mult ) >> smCLIP_DETAIL_SHIFT );
	z = SMFLOAT_DIST_TEXCLIP;

	// ������ ���� 
	for( cnt=0;cnt<4;cnt++ ) {
		sCol[cnt] = (short)((int)t1->sLight[cnt] + (( ( (int)t2->sLight[cnt] - (int)t1->sLight[cnt] ) * Mult )>>smCLIP_DETAIL_SHIFT ) );
	}

	*fMultPoint = fMult;

	// ������ ���ؽ��� ������ ���ؽ��� �߰�
	return AddRendVertex ( x, y, z, sCol );

}

// D3D �������� ���� ���۰� ����
int smRENDER3D::SetD3DRendBuff(float *RenderBuff, int matrial, int TexClip)
{
	smRENDFACE *rendface;
	smRENDVERTEX *rv;
	smTEXLINK *tl;
	int cnt;
	int vcnt , tcnt , mcnt;
	int rendcnt;

	BYTE	dwSpecular[4];
	BYTE	dwColor[4];

	int wtime = (RendStatTime>>6)&0xFF;
	float fwtime = (float)wtime / 256;

	if ( TexClip==TRUE ) 
	{
		//�ؽ��� Ŭ���� ū �ؽ��� ( �ٰŸ��� )
		rendface = RendMatrial[ matrial ].TexClip_StartFace;
		mcnt = RendMatrial[ matrial ].TexClip_MatrialCounter;
	}
	else 
	{
		//�Ϲ����� ���� �ؽ��� ( ���Ÿ��� )
		rendface = RendMatrial[ matrial ].StartFace;
		mcnt = RendMatrial[ matrial ].MatrialCounter;
	}

	rendcnt = 0;

	for ( cnt=0; cnt<mcnt; cnt++ )
	{
		if ( !(rendface->ClipStatus&SMCLIP_DISPLAYOUT) )
		{
			for( vcnt=0;vcnt<3;vcnt++ )
			{
				rv = rendface->lpRendVertex[vcnt];

				*(RenderBuff++) = (float)rv->xy[0];
				*(RenderBuff++) = (float)rv->xy[1];
				*(RenderBuff++) = (float)rv->zb;
				*(RenderBuff++) = (float)rv->rhw;

				if ( RendAlphaFlag && rv->tz>(ccDistZMin<<FLOATNS) ) 
				{
					*((DWORD *)dwColor) = *((DWORD *)rv->bCol);
					*((DWORD *)dwSpecular) = *((DWORD *)rv->bSpe);
					dwSpecular[3] = dwSpecular[3]/2 + dwSpecular[3]/3;
					dwColor[3] = dwColor[3]/2 + dwColor[3]/3;

					*((DWORD *)(RenderBuff++)) = *((DWORD *)dwColor) ;
					*((DWORD *)(RenderBuff++)) = *((DWORD *)dwSpecular) ;
				}
				else 
				{
					*((DWORD *)(RenderBuff++)) = *((DWORD *)rv->bCol) ;
					*((DWORD *)(RenderBuff++)) = *((DWORD *)rv->bSpe) ;
				}

				tl = rendface->lpTexLink;
				for ( tcnt=0;tcnt<8;tcnt++ )
				{				
					if( tl == NULL )
					{
						if( tcnt == 1 && s_lpItem2PassTexHandle )
							tl = rendface->lpTexLink;
						else
                            break;
					}

					if ( TextureFormState[tcnt] )
					{
						switch( TextureFormState[tcnt] ) 
						{
						case smTEXSTATE_FS_FORMX:
							/*
							z = rv->wz- TexPosiZ;
							y = rv->wy- TexPosiY;
	
							z = ( (z<<FLOATNS) / TexSizeZ );
							fz = ( (float)z) /fONE;
							y = ( (y<<FLOATNS) / TexSizeY );
							fy = -( (float)y )/fONE;

							fz -= 0.5;
							fy -= 0.5;

							*(RenderBuff++) = (float)fz;
							*(RenderBuff++) = (float)fy;
							break;
							*/
						case smTEXSTATE_FS_FORMY:
						case smTEXSTATE_FS_FORMZ:

						case smTEXSTATE_FS_NONE:
							//���� ����
							*(RenderBuff++) = (float)tl->u[vcnt];
							*(RenderBuff++) = (float)tl->v[vcnt];
							break;

						case smTEXSTATE_FS_SCROLL:
							//��ũ��
							*(RenderBuff++) = (float)(tl->u[vcnt] + fwtime);
							*(RenderBuff++) = (float)(tl->v[vcnt]);//+fwtime);					
							break;

						case smTEXSTATE_FS_REFLEX:
							//*(RenderBuff++) = (float)( tl->u[vcnt] + ((float)(rv->tx+(rv->tz>>12)))/65536) ;
							//*(RenderBuff++) = (float)( tl->v[vcnt] + ((float)(rv->ty+(rv->tz>>12)))/65536) ;
							//������ ����. *(RenderBuff++) = (float)( tl->u[vcnt] + ((float)(rv->tx>>1)/65536)) ;
							//������ ����. *(RenderBuff++) = (float)( tl->v[vcnt] + ((float)(rv->ty>>2)/65536)) ;
							//*(RenderBuff++) = (float)tl->v[vcnt];
							//*(RenderBuff++) = (float)( tl->v[vcnt] + ((float)(rv->ty)/65536)) ;
							//######################################################################################
							//�� �� �� : �� �� ��
							{
								float fEx  = float(rv->tx) / fONE;
								float fEy  = float(rv->ty) / fONE;
								float fEz  = float(rv->tz) / fONE;
								float fLen = (float)sqrt( (fEx*fEx) + (fEy*fEy) + (fEz*fEz) );		
								fEx /= fLen;
								fEy /= fLen;
								fEz /= fLen;		
			
								float fNx = float(rv->nx - rv->tx) / 32767.f;
								float fNy = float(rv->ny - rv->ty) / 32767.f;
								float fNz = float(rv->nz - rv->tz) / 32767.f;

								float fDotNum = (fEx*fNx) + (fEy*fNy) + (fEz*fNz);
								float fx = fEx - (fDotNum * fNx * 2);
								float fy = fEy - (fDotNum * fNy * 2);
								
								*(RenderBuff++) = (fx * 0.5f) + 0.5f;
								*(RenderBuff++) = (-fy * 0.5f) + 0.5f;
							}
							//######################################################################################
							break;

						}

						if ( TextureFormState[tcnt]>=smTEXSTATE_FS_SCROLL2 && TextureFormState[tcnt]<=smTEXSTATE_FS_SCROLL10 )
						{
							//��ũ�� ( ��� 2~10 )
							*(RenderBuff++) = (float)(tl->u[vcnt] + fwtime* (TextureFormState[tcnt] - (smTEXSTATE_FS_SCROLL2 - 2)));
							*(RenderBuff++) = (float)(tl->v[vcnt]);
						}
						//######################################################################################
						//�� �� �� : ������
						else if( TextureFormState[tcnt]>=smTEXSTATE_FS_SCROLLSLOW1 && TextureFormState[tcnt]<=smTEXSTATE_FS_SCROLLSLOW4) //������ �߰�..
						{
 							int factor = smTEXSTATE_FS_SCROLLSLOW4 - TextureFormState[tcnt] + 4;
							wtime  = (RendStatTime >> 6) & (0xFFFF >> factor);

							fwtime = float(wtime) / float(0xFFFF >> factor);
							*(RenderBuff++) = (float)(tl->u[vcnt] + fwtime);
							*(RenderBuff++) = (float)(tl->v[vcnt]);
						}
						//######################################################################################
					}
					else {
						//���� ����
						*(RenderBuff++) = (float)tl->u[vcnt];
						*(RenderBuff++) = (float)tl->v[vcnt];
					}

					tl = tl->NextTex;
				}
				rendcnt++;
			}
		}
		rendface = rendface->NexRendFace;
	}

	return rendcnt;
}

// D3D �������� ���� ���۰� ����
int smRENDER3D::SetD3DRendBuff2(float *RenderBuff, int matrial, int TexClip)
{
	smRENDFACE *rendface;
	smRENDVERTEX *rv;
	smTEXLINK *tl;
	int cnt;
	int vcnt , tcnt , mcnt;
	int rendcnt;

	int wtime = (RendStatTime>>6)&0xFF;
	float fwtime = (float)wtime / 256;


	if ( TexClip==TRUE ) {
		//�ؽ��� Ŭ���� ū �ؽ��� ( �ٰŸ��� )
		rendface = RendMatrial[ matrial ].TexClip_StartFace;
		mcnt = RendMatrial[ matrial ].TexClip_MatrialCounter;
	}
	else {
		//�Ϲ����� ���� �ؽ��� ( ���Ÿ��� )
		rendface = RendMatrial[ matrial ].StartFace;
		mcnt = RendMatrial[ matrial ].MatrialCounter;
	}

	rendcnt = 0;

	for ( cnt=0; cnt<mcnt; cnt++ ) {

		if ( !(rendface->ClipStatus&SMCLIP_DISPLAYOUT) ) {
			for( vcnt=0;vcnt<3;vcnt++ ) {

				rv = rendface->lpRendVertex[vcnt];

				*(RenderBuff++) = (float)rv->xy[0];
				*(RenderBuff++) = (float)rv->xy[1];
				*(RenderBuff++) = (float)rv->zb;
				*(RenderBuff++) = (float)rv->rhw;

				*((DWORD *)(RenderBuff++)) = (*((DWORD *)rv->bCol))|0x00FFFFFF;
				*((DWORD *)(RenderBuff++)) = 0x00000000;

				tl = rendface->lpTexLink;
				for ( tcnt=0;tcnt<8;tcnt++ )
				{
					//######################################################################################
					// �� �� �� : �� �� ��
					if( tl == NULL )
					{
						if( tcnt == 1 && s_lpItem2PassTexHandle )
							tl = rendface->lpTexLink;
						else
                            break;
					}
					//if ( !tl ) break;
					//######################################################################################					

					

					//////////////////////////////////////////////////////////////////
					if ( TextureFormState[tcnt] ) {
						switch( TextureFormState[tcnt] ) {

						case smTEXSTATE_FS_FORMX:
							/*
							z = rv->wz- TexPosiZ;
							y = rv->wy- TexPosiY;
	
							z = ( (z<<FLOATNS) / TexSizeZ );
							fz = ( (float)z) /fONE;
							y = ( (y<<FLOATNS) / TexSizeY );
							fy = -( (float)y )/fONE;

							fz -= 0.5;
							fy -= 0.5;

							*(RenderBuff++) = (float)fz;
							*(RenderBuff++) = (float)fy;
							break;
							*/
						case smTEXSTATE_FS_FORMY:
						case smTEXSTATE_FS_FORMZ:

						case smTEXSTATE_FS_NONE:
							//���� ����
							*(RenderBuff++) = (float)tl->u[vcnt];
							*(RenderBuff++) = (float)tl->v[vcnt];
							break;

						case smTEXSTATE_FS_SCROLL:
							//��ũ��
							*(RenderBuff++) = (float)(tl->u[vcnt]+fwtime);
							*(RenderBuff++) = (float)(tl->v[vcnt]);//+fwtime);					
							break;

						case smTEXSTATE_FS_REFLEX:
							//*(RenderBuff++) = (float)( tl->u[vcnt] + ((float)(rv->tx+(rv->tz>>12)))/65536) ;
							//*(RenderBuff++) = (float)( tl->v[vcnt] + ((float)(rv->ty+(rv->tz>>12)))/65536) ;
							//������ ����. *(RenderBuff++) = (float)( tl->u[vcnt] + ((float)(rv->tx>>1)/65536)) ;
							//������ ����. *(RenderBuff++) = (float)( tl->v[vcnt] + ((float)(rv->ty>>2)/65536)) ;
							//*(RenderBuff++) = (float)tl->v[vcnt];
							//*(RenderBuff++) = (float)( tl->v[vcnt] + ((float)(rv->ty)/65536)) ;
							//######################################################################################
							//�� �� �� : �� �� ��
							{
								float fEx  = float(rv->tx) / fONE;
								float fEy  = float(rv->ty) / fONE;
								float fEz  = float(rv->tz) / fONE;
								float fLen = (float)sqrt( (fEx*fEx) + (fEy*fEy) + (fEz*fEz) );		
								fEx /= fLen;
								fEy /= fLen;
								fEz /= fLen;		
			
								float fNx = float(rv->nx - rv->tx) / 32767.f;
								float fNy = float(rv->ny - rv->ty) / 32767.f;
								float fNz = float(rv->nz - rv->tz) / 32767.f;

								float fDotNum = (fEx*fNx) + (fEy*fNy) + (fEz*fNz);
								float fx = fEx - (fDotNum * fNx * 2);
								float fy = fEy - (fDotNum * fNy * 2);
								
								*(RenderBuff++) = (fx * 0.5f) + 0.5f;
								*(RenderBuff++) = (-fy * 0.5f) + 0.5f;
							}
							//######################################################################################
							break;
						}

						if ( TextureFormState[tcnt]>=smTEXSTATE_FS_SCROLL2 && TextureFormState[tcnt]<=smTEXSTATE_FS_SCROLL10 ) {
							//��ũ�� ( ��� 2~10 )
							*(RenderBuff++) = (float)(tl->u[vcnt]+fwtime* (TextureFormState[tcnt]-(smTEXSTATE_FS_SCROLL2-2)) );
							*(RenderBuff++) = (float)(tl->v[vcnt]);
						}
						//######################################################################################
						//�� �� �� : ������
						else if( TextureFormState[tcnt]>=smTEXSTATE_FS_SCROLLSLOW1 && TextureFormState[tcnt]<=smTEXSTATE_FS_SCROLLSLOW4) //������ �߰�..
						{
 							int factor = smTEXSTATE_FS_SCROLLSLOW4 - TextureFormState[tcnt] + 4;
							wtime  = (RendStatTime >> 6) & (0xFFFF >> factor);

							fwtime = float(wtime) / float(0xFFFF >> factor);
							*(RenderBuff++) = (float)(tl->u[vcnt] + fwtime);
							*(RenderBuff++) = (float)(tl->v[vcnt]);
						}
						//######################################################################################
					}
					else {
						//���� ����
						*(RenderBuff++) = (float)tl->u[vcnt];
						*(RenderBuff++) = (float)tl->v[vcnt];
					}
					//////////////////////////////////////////////////////////////////

					//���� ����
					//*(RenderBuff++) = (float)tl->u[vcnt];
					//*(RenderBuff++) = (float)tl->v[vcnt];

					tl = tl->NextTex;
				}
				rendcnt++;
			}
		}
		rendface = rendface->NexRendFace;
	}

	return rendcnt;
}

// �� �� �� : �� �� ��
// D3D �������� ���� ���۰� ����  : smD3D ����. �� Ư�� ȿ�� ����
int smRENDER3D::SetD3DRendBuff3( float *RenderBuff , int matrial , int TexClip, short sColors[4] )
{
	smRENDFACE *rendface;
	smRENDVERTEX *rv;
	smTEXLINK *tl;
	int cnt;
	int vcnt , tcnt , mcnt;
	int rendcnt;

	BYTE	dwSpecular[4];
	BYTE	dwColor[4];

	int wtime = (RendStatTime>>6)&0xFF;
	float fwtime = (float)wtime / 256;

	if ( TexClip==TRUE ) {
		//�ؽ��� Ŭ���� ū �ؽ��� ( �ٰŸ��� )
		rendface = RendMatrial[ matrial ].TexClip_StartFace;
		mcnt = RendMatrial[ matrial ].TexClip_MatrialCounter;
	}
	else {
		//�Ϲ����� ���� �ؽ��� ( ���Ÿ��� )
		rendface = RendMatrial[ matrial ].StartFace;
		mcnt = RendMatrial[ matrial ].MatrialCounter;
	}

	rendcnt = 0;

	for ( cnt=0; cnt<mcnt; cnt++ )
	{
		if ( !(rendface->ClipStatus&SMCLIP_DISPLAYOUT) )
		{
			for( vcnt=0;vcnt<3;vcnt++ )
			{

				rv = rendface->lpRendVertex[vcnt];

				*(RenderBuff++) = (float)rv->xy[0];
				*(RenderBuff++) = (float)rv->xy[1];
				*(RenderBuff++) = (float)rv->zb;
				*(RenderBuff++) = (float)rv->rhw;


				if ( RendAlphaFlag && rv->tz>(ccDistZMin<<FLOATNS) )
				{
					*((DWORD *)dwColor) = *((DWORD *)rv->bCol);
					*((DWORD *)dwSpecular) = *((DWORD *)rv->bSpe);
					dwSpecular[3] = dwSpecular[3]/2 + dwSpecular[3]/3;
					dwColor[3] = dwColor[3]/2 + dwColor[3]/3;

					*((DWORD *)(RenderBuff++)) = *((DWORD *)dwColor) ;
					*((DWORD *)(RenderBuff++)) = *((DWORD *)dwSpecular) ;
				}
				else
				{
					if( SetBlinkColorVertex(rv, sColors, dwColor, dwSpecular) )
					{
                        *((DWORD *)(RenderBuff++)) = *((DWORD *)dwColor) ;
                        *((DWORD *)(RenderBuff++)) = *((DWORD *)dwSpecular) ;
					}
					else
					{
						*((DWORD *)(RenderBuff++)) = *((DWORD *)rv->bCol) ;
						*((DWORD *)(RenderBuff++)) = *((DWORD *)rv->bSpe) ;
					}
				}

				tl = rendface->lpTexLink;
				for ( tcnt=0;tcnt<8;tcnt++ )
				{
					if ( !tl ) break;
					if ( TextureFormState[tcnt] ) {
						switch( TextureFormState[tcnt] ) {

						case smTEXSTATE_FS_FORMX:
						case smTEXSTATE_FS_FORMY:
						case smTEXSTATE_FS_FORMZ:

						case smTEXSTATE_FS_NONE:
							//���� ����
							*(RenderBuff++) = (float)tl->u[vcnt];
							*(RenderBuff++) = (float)tl->v[vcnt];
							break;

						case smTEXSTATE_FS_SCROLL:
							//��ũ��
							*(RenderBuff++) = (float)(tl->u[vcnt]+fwtime);
							*(RenderBuff++) = (float)(tl->v[vcnt]);//+fwtime);					
							break;

						case smTEXSTATE_FS_REFLEX:
							*(RenderBuff++) = (float)( tl->u[vcnt] + ((float)(rv->tx>>1)/65536)) ;
							*(RenderBuff++) = (float)( tl->v[vcnt] + ((float)(rv->ty>>2)/65536)) ;
							break;

						}

						if ( TextureFormState[tcnt]>=smTEXSTATE_FS_SCROLL2 && TextureFormState[tcnt]<=smTEXSTATE_FS_SCROLL10 ) {
							//��ũ�� ( ��� 2~10 )
							*(RenderBuff++) = (float)(tl->u[vcnt]+fwtime* (TextureFormState[tcnt]-(smTEXSTATE_FS_SCROLL2-2)) );
							*(RenderBuff++) = (float)(tl->v[vcnt]);
						}
						//######################################################################################
						//�� �� �� : ������
						else if( TextureFormState[tcnt]>=smTEXSTATE_FS_SCROLLSLOW1 && TextureFormState[tcnt]<=smTEXSTATE_FS_SCROLLSLOW4) //������ �߰�..
						{
 							int factor = smTEXSTATE_FS_SCROLLSLOW4 - TextureFormState[tcnt] + 4;
							wtime  = (RendStatTime >> 6) & (0xFFFF >> factor);

							fwtime = float(wtime) / float(0xFFFF >> factor);
							*(RenderBuff++) = (float)(tl->u[vcnt] + fwtime);
							*(RenderBuff++) = (float)(tl->v[vcnt]);
						}
						//######################################################################################
					}
					else {
						//���� ����
						*(RenderBuff++) = (float)tl->u[vcnt];
						*(RenderBuff++) = (float)tl->v[vcnt];
					}

					tl = tl->NextTex;
				}
				rendcnt++;
			}
		}
		rendface = rendface->NexRendFace;
	}

	return rendcnt;
}

// �� �� �� : �� �� ��
int smRENDER3D::SetItem2PassD3DRendState( smMATERIAL *lpMaterial )
{
	DWORD cnt;
	int	MapOpacity;
	int MapDualRend;

	MapOpacity = 0;
	MapDualRend = 0;
	
	if( lpMaterial->TextureType == SMTEX_TYPE_MULTIMIX )
	{
		//
		for( cnt=0; cnt < 2; cnt++ )
		{
			//
			if( cnt == 0 )
                TextureFormState[cnt] = lpMaterial->TextureFormState[cnt];
			else // if( cnt == 1 )
				TextureFormState[cnt] = s_Item2PassTexFormState;

			//
			renderDevice.SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, cnt);

			if( cnt == 0 )
				renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_MODULATE);
			else // if( cnt == 1 )				
				renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_ADD);

			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_MINFILTER, D3DTFN_LINEAR);
			renderDevice.SetTextureStageState(cnt, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

			//
			if( cnt == 0 )
				renderDevice.SetTexture(cnt, lpMaterial->smTexture[cnt]->lpD3DTexture);
			else // if( cnt == 1 )
				renderDevice.SetTexture(cnt, s_lpItem2PassTexHandle->lpD3DTexture);

			//
			if( cnt == 0 )
			{
				if( lpMaterial->smTexture[cnt]->MapOpacity )
					MapOpacity += lpMaterial->smTexture[cnt]->MapOpacity;
			}
		}

		//
		for( cnt=2; cnt < 8; cnt++ )
		{
			renderDevice.SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, 0);

			renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_DISABLE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_MINFILTER, D3DTFN_POINT);
			renderDevice.SetTextureStageState(cnt, D3DTSS_MAGFILTER, D3DTFG_POINT);

			renderDevice.SetTexture(cnt, 0);
		}		
	}
	else
	{
		for( cnt=0; cnt < 8; cnt++ )
		{
			renderDevice.SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, 0);

			renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_DISABLE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_MINFILTER, D3DTFN_POINT);
			renderDevice.SetTextureStageState(cnt, D3DTSS_MAGFILTER, D3DTFG_POINT);

			renderDevice.SetTexture(cnt, 0);
		}
	}

	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE);

	switch( lpMaterial->BlendType )
	{
	case SMMAT_BLEND_NONE:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
		break;
	case SMMAT_BLEND_ALPHA:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		break;
	case SMMAT_BLEND_COLOR:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		break;

	case SMMAT_BLEND_ADDCOLOR:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		break;

	case SMMAT_BLEND_SHADOW:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
		break;

	case SMMAT_BLEND_LAMP:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		break;

	case SMMAT_BLEND_INVSHADOW:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
		break;

	}

	if ( lpMaterial->TwoSide ) 
		renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
	else 
		renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);

	if ( MapOpacity ) {
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAREF, AlphaTestDepth);
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	else {
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
	}

	if ( ZWriteAuto ) {
		if ( lpMaterial->Transparency>0.2f )
			renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		else
			renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	}


	return MapDualRend;
}

// D3D������ �Ӽ� ���� 
int smRENDER3D::SetD3DRendState( smMATERIAL *lpMaterial , int TexClip )
{
	DWORD cnt;
	int	MapOpacity;
	int MapDualRend;

	MapOpacity = 0;
	MapDualRend = 0;

	switch( lpMaterial->TextureType ) {
	case SMTEX_TYPE_MULTIMIX:
		for( cnt=0;cnt<lpMaterial->TextureCounter;cnt++ ) {

			TextureFormState[cnt] = lpMaterial->TextureFormState[cnt];

			renderDevice.SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, cnt);

			if ( lpMaterial->TextureStageState[cnt] )
				renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, lpMaterial->TextureStageState[cnt]);
			else
				renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_MODULATE);

			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

			renderDevice.SetTextureStageState(cnt, D3DTSS_MINFILTER, D3DTFN_LINEAR);
			renderDevice.SetTextureStageState(cnt, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

			if ( TexClip==TRUE && lpMaterial->smTexture[cnt]->lpD3DTextureLarge ) 
				renderDevice.SetTexture(cnt, lpMaterial->smTexture[cnt]->lpD3DTextureLarge);
			else
				renderDevice.SetTexture(cnt, lpMaterial->smTexture[cnt]->lpD3DTexture);

			if ( lpMaterial->smTexture[cnt]->MapOpacity ) {
				MapOpacity += lpMaterial->smTexture[cnt]->MapOpacity;
				if ( cnt>0 ) {
					renderDevice.SetTexture(cnt, 0);
					MapDualRend = TRUE;
				}
			}
		}

		for( cnt=lpMaterial->TextureCounter; cnt<8; cnt++ ) {
			
			renderDevice.SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, 0);

			
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_DISABLE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_MINFILTER, D3DTFN_POINT);
			renderDevice.SetTextureStageState(cnt, D3DTSS_MAGFILTER, D3DTFG_POINT);

			renderDevice.SetTexture(cnt, 0);
		}

		break;

	case SMTEX_TYPE_ANIMATION:

		cnt = 0;

		renderDevice.SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, cnt);

		renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_MODULATE);
		renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

		renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		renderDevice.SetTextureStageState(cnt, D3DTSS_MINFILTER, D3DTFN_LINEAR);
		renderDevice.SetTextureStageState(cnt, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

		if ( lpMaterial->AnimationFrame==SMTEX_AUTOANIMATION )
			cnt = (RendStatTime>>lpMaterial->Shift_FrameSpeed)&lpMaterial->FrameMask;
		else
			cnt = lpMaterial->AnimationFrame;

		renderDevice.SetTexture(0, lpMaterial->smAnimTexture[cnt]->lpD3DTexture);
		MapOpacity += lpMaterial->smAnimTexture[cnt]->MapOpacity;

		for( cnt=1; cnt<8; cnt++ )
		{
			renderDevice.SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, 0);


			renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_DISABLE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			renderDevice.SetTextureStageState(cnt, D3DTSS_MINFILTER, D3DTFN_POINT);
			renderDevice.SetTextureStageState(cnt, D3DTSS_MAGFILTER, D3DTFG_POINT);

			renderDevice.SetTexture(cnt, 0);
		}


		break;
	}

	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE);

	switch( lpMaterial->BlendType )
	{
	case SMMAT_BLEND_NONE:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
		break;
	case SMMAT_BLEND_ALPHA:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		break;
	case SMMAT_BLEND_COLOR:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		break;

	case SMMAT_BLEND_ADDCOLOR:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		break;

	case SMMAT_BLEND_SHADOW:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
		break;

	case SMMAT_BLEND_LAMP:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		break;

	case SMMAT_BLEND_INVSHADOW:
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
		break;

	}

/*
	renderDevice.SetRenderState( D3DRENDERSTATE_CULLMODE , D3DCULL_CW );
	renderDevice.SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
	renderDevice.SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA  );
	renderDevice.SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA  );

	renderDevice.SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	renderDevice.SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	renderDevice.SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	renderDevice.SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	renderDevice.SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );

	renderDevice.SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	renderDevice.SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );



            lpD3DDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
            lpD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
            lpD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );

            lpD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
            lpD3DDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
            lpD3DDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

            lpD3DDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_LINEAR );
            lpD3DDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFG_LINEAR );


			lpD3DDevice->SetTexture( 1, lpMaterial->smTexture[1]->lpD3DTexture  );
			lpD3DDevice->SetTexture( 0, lpMaterial->smTexture[0]->lpD3DTexture  );


            lpD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
            lpD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
            lpD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
            lpD3DDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
            lpD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
            lpD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );

			lpD3DDevice->SetTexture( 1, lpMaterial->smTexture[0]->lpD3DTexture  );
			lpD3DDevice->SetTexture( 0, lpMaterial->smTexture[1]->lpD3DTexture  );
*/


	if ( lpMaterial->TwoSide ) 
		renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
	else 
		renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);

	if ( MapOpacity ) {
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAREF, AlphaTestDepth);
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	else {
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
	}

	if ( ZWriteAuto ) {
		if ( lpMaterial->Transparency>0.2f )
			renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		else
			renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	}


	return MapDualRend;
}


// D3D������ �Ӽ� ���� ���ĸ� ����
int smRENDER3D::SetD3DRendStateOnlyAlpha( smMATERIAL *lpMaterial , int TexClip )
{
	DWORD cnt;
	DWORD MapCnt;

	MapCnt = 0;

	for( cnt=0;cnt<lpMaterial->TextureCounter;cnt++ )
	{
			if ( lpMaterial->smTexture[cnt]->MapOpacity )
			{
				renderDevice.SetTextureStageState(MapCnt, D3DTSS_TEXCOORDINDEX, cnt);

				renderDevice.SetTextureStageState(MapCnt, D3DTSS_COLOROP, D3DTOP_MODULATE);
				renderDevice.SetTextureStageState(MapCnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				renderDevice.SetTextureStageState(MapCnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

				renderDevice.SetTextureStageState(MapCnt, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				renderDevice.SetTextureStageState(MapCnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
				renderDevice.SetTextureStageState(MapCnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	
				renderDevice.SetTextureStageState(MapCnt, D3DTSS_MINFILTER, D3DTFN_LINEAR);
				renderDevice.SetTextureStageState(MapCnt, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

				if ( TexClip==TRUE && lpMaterial->smTexture[cnt]->lpD3DTextureLarge )
					renderDevice.SetTexture(MapCnt, lpMaterial->smTexture[cnt]->lpD3DTextureLarge);
				else
					renderDevice.SetTexture(MapCnt, lpMaterial->smTexture[cnt]->lpD3DTexture);

				MapCnt++;

				break;
			}

	}

	for( cnt=MapCnt; cnt<8; cnt++ ) {
			
		renderDevice.SetTextureStageState(cnt, D3DTSS_TEXCOORDINDEX, 0);

			
		renderDevice.SetTextureStageState(cnt, D3DTSS_COLOROP, D3DTOP_DISABLE);
		renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		renderDevice.SetTextureStageState(cnt, D3DTSS_COLORARG2, D3DTA_CURRENT);

		renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		renderDevice.SetTextureStageState(cnt, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

		renderDevice.SetTextureStageState(cnt, D3DTSS_MINFILTER, D3DTFN_POINT);
		renderDevice.SetTextureStageState(cnt, D3DTSS_MAGFILTER, D3DTFG_POINT);

		renderDevice.SetTexture(cnt, 0);
	}

	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE);

	//���ĺ��� ����
	renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);

	if ( lpMaterial->TwoSide ) 
		renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
	else 
		renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);
/*
	if ( MapOpacity ) {
		renderDevice.SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE , TRUE );
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAREF, AlphaTestDepth );
		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL  );
	}
	else {
		renderDevice.SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE , FALSE );
	}
*/
	renderDevice.SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);

	return TRUE;
}

//��Ʈ���� ����� D3D�� ������
int smRENDER3D::RenderD3D( int MatNum )
{
	int rendcnt;

	if ( smRendMaterial[MatNum].TextureCounter==0 && smRendMaterial[MatNum].AnimTexCounter==0 ) 
		return FALSE;

	short sColors[4] = {0,};
	if( smRendMaterial[MatNum].UseState & sMATS_SCRIPT_BLINK_COLOR )
		SetBlinkColor( sColors, smRendMaterial[MatNum].Diffuse, smRendMaterial[MatNum].WindMeshBottom );

	//�Ϲ� ��Ʈ������ ���̽� �׸���
	if ( RendMatrial[MatNum].MatrialCounter )
	{

		int TexCnt = 0;
		if( s_lpItem2PassTexHandle )
		{
            RendAlphaFlag = SetItem2PassD3DRendState( &smRendMaterial[ MatNum ] );	//��Ʈ����� ������ ����
			TexCnt = 1;
		}
		else
		{
			RendAlphaFlag = SetD3DRendState(&smRendMaterial[MatNum], FALSE);		//��Ʈ����� ������ ����
		}

		if ( smRendMaterial[MatNum].SelfIllum!=0 )
			rendcnt = SetD3DRendBuff2( RendBuff , MatNum , FALSE );		//����� ���� ���� 
		else if( smRendMaterial[MatNum].UseState & sMATS_SCRIPT_BLINK_COLOR )
			rendcnt = SetD3DRendBuff3( RendBuff , MatNum , FALSE, sColors ); //����� ���� ���� 
		else
			rendcnt = SetD3DRendBuff( RendBuff , MatNum , FALSE );		//����� ���� ���� 

		renderDevice.DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_TEXTURE[smRendMaterial[MatNum].TextureCounter+TexCnt],
                                   (LPVOID)RendBuff, rendcnt, NULL );
		//######################################################################################

		if ( RendAlphaFlag ) 
		{
			//���� �ռ� ��Ʈ������ ��� ���ĸ� �ٽ� ������
			SetD3DRendStateOnlyAlpha( &smRendMaterial[MatNum] , FALSE );
			renderDevice.DrawPrimitive(D3DPT_TRIANGLELIST,
								D3DFVF_TEXTURE[ smRendMaterial[MatNum].TextureCounter ] ,
								(LPVOID)RendBuff,
								rendcnt,
								NULL);

		}
	}

	//�ؽ��� Ŭ���ε� ū �ؽ��� ���̽� �׸��� 
	if ( RendMatrial[MatNum].TexClip_MatrialCounter )
	{
		RendAlphaFlag = SetD3DRendState( &smRendMaterial[MatNum] , TRUE );				//��Ʈ����� ������ ����
		//if ( RendAlphaFlag ) SetD3DRendStateOnlyAlpha( &smRendMaterial[MatNum] , TRUE );
		if ( smRendMaterial[MatNum].SelfIllum!=0 )
			rendcnt = SetD3DRendBuff2( RendBuff , MatNum , TRUE );		//����� ���� ���� 
		//######################################################################################
		// �� �� �� : �� �� ��
		else if( smRendMaterial[MatNum].UseState & sMATS_SCRIPT_BLINK_COLOR )
			rendcnt = SetD3DRendBuff3( RendBuff , MatNum , TRUE, sColors ); //����� ���� ���� 
		else
			rendcnt = SetD3DRendBuff( RendBuff , MatNum , TRUE );		//����� ���� ���� 
		//######################################################################################

		renderDevice.DrawPrimitive(D3DPT_TRIANGLELIST,
									D3DFVF_TEXTURE[ smRendMaterial[MatNum].TextureCounter ] ,
									(LPVOID)RendBuff,
									rendcnt,
									NULL);

		if ( RendAlphaFlag ) 
		{
			//���� �ռ� ��Ʈ������ ��� ���ĸ� �ٽ� ������
			SetD3DRendStateOnlyAlpha( &smRendMaterial[MatNum] , TRUE );
			renderDevice.DrawPrimitive(D3DPT_TRIANGLELIST,
								D3DFVF_TEXTURE[ smRendMaterial[MatNum].TextureCounter ] ,
								(LPVOID)RendBuff,
								rendcnt,
								NULL);
		}
	}

	return rendcnt;
}

// D3D�� ������
int smRENDER3D::RenderD3D()
{
	int cnt;
	int TotalRendCnt;
	int	MatCnt;

	int	RendLatterCount;
	int	RendLatterBuff[1024];

	RendLatterCount = 0;

	if (DeviceRendMode)
		renderDevice.BeginScene();

	TotalRendCnt = 0;

	if( isRenderOutLine > 0 )
		TotalRendCnt += RenderD3D_OutLine();

	//��Ʈ���� ���� ������ �� �����
	for( cnt=0;cnt<RMatListCnt;cnt++ )
	{
		MatCnt = RendMatrialList[cnt];
		if ( (smRendMaterial[MatCnt].MeshState&sMATS_SCRIPT_RENDLATTER)==0 ) 
		{
			TotalRendCnt += RenderD3D( MatCnt );
		}
		else 
		{
			if ( RendLatterCount<1024 )
				RendLatterBuff[RendLatterCount++]=MatCnt;
			else
				TotalRendCnt+= RenderD3D( MatCnt );
		}
	}

	//��Ʈ���� ���� ������ �� �����
	for( cnt=RMatListRearCnt+1;cnt<nMaterial;cnt++ )
	{
		MatCnt = RendMatrialList[cnt];
		if ( (smRendMaterial[MatCnt].MeshState & sMATS_SCRIPT_RENDLATTER) == 0 ) 
		{
			TotalRendCnt+= RenderD3D( MatCnt );
		}
		else
		{
			if ( RendLatterCount<1024 )
				RendLatterBuff[RendLatterCount++]=MatCnt;
			else
				TotalRendCnt+= RenderD3D( MatCnt );
		}
	}

	//���߿� ������ �ϵ��� ������ ��Ʈ���� ���� ��� �����
	for( cnt=0;cnt<RendLatterCount;cnt++ )
	{
		TotalRendCnt+= RenderD3D( RendLatterBuff[cnt] );
	}

	if (DeviceRendMode) renderDevice.EndScene();

	if ( ZWriteAuto )
		renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);

	return TotalRendCnt;
}

//�������� ����Ʈ ����
void smSetRendLightVect( int x, int y, int z )
{
	pLight.x = x;
	pLight.y = y;
	pLight.z = z;
}

// �� �� �� : �� �� ��
void smRENDER3D::SetD3D_OutLine( int isSet, int Dist, float fsetX, float fsetY, DWORD Color )
{
	isRenderOutLine = isSet;
	if( isRenderOutLine <= 0 )
		return;

	if( Dist < 300 )
        fOutLineRatio = 1.f;
    else if( Dist < 1300 )
		fOutLineRatio = 1.f - float(Dist - 300) / 1000.f;
	else
		fOutLineRatio = 0.f;

	fOutLineX    = fsetX;
	fOutLineY    = fsetY;
	OutLineColor = Color;
}

// �� �� �� : �� �� ��
int smRENDER3D::RenderD3D_OutLine(void)
{
	smMATERIAL *lpMaterial;
	int MatNum, cnt, TotalRendCnt = 0;

	//
	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREFACTOR, OutLineColor);
	renderDevice.SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	renderDevice.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	renderDevice.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	renderDevice.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	renderDevice.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	renderDevice.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	renderDevice.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	renderDevice.SetTexture(1, NULL);
	renderDevice.SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	renderDevice.SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	renderDevice.SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	renderDevice.SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_POINT);
	renderDevice.SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_POINT);
	renderDevice.SetTextureStageState(1, D3DTSS_MINFILTER, D3DTFN_POINT);
	renderDevice.SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTFG_POINT);

	//��Ʈ���� ���� ������ �� �����
	for( cnt=0; cnt < RMatListCnt; cnt++ )
	{
		MatNum	   = RendMatrialList[ cnt ];
		lpMaterial = &smRendMaterial[ MatNum ];
		
		if( lpMaterial->TwoSide ) 
			renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
		else 
			renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);

		if( lpMaterial->smTexture[0] )
		{
			renderDevice.SetTexture(0, lpMaterial->smTexture[0]->lpD3DTexture);

			if( lpMaterial->smTexture[0]->MapOpacity )
			{
				renderDevice.SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
				renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAREF, AlphaTestDepth);
				renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			}
			else
			{
				renderDevice.SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
			}
		}

		switch( lpMaterial->BlendType )
		{
		case SMMAT_BLEND_NONE:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
			break;
		case SMMAT_BLEND_ALPHA:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		case SMMAT_BLEND_COLOR:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			break;
		case SMMAT_BLEND_ADDCOLOR:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
			break;
		case SMMAT_BLEND_SHADOW:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR);
			break;
		case SMMAT_BLEND_LAMP:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
			break;
		case SMMAT_BLEND_INVSHADOW:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			break;
		}

		TotalRendCnt += RenderD3D_OutLine( MatNum );
	}

	//��Ʈ���� ���� ������ �� �����
	for( cnt=RMatListRearCnt+1; cnt < nMaterial; cnt++ )
	{
		MatNum	   = RendMatrialList[ cnt ];
		lpMaterial = &smRendMaterial[ MatNum ];

		if( lpMaterial->TwoSide ) 
			renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
		else 
			renderDevice.SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);

		if( lpMaterial->smTexture[0] )
		{
			renderDevice.SetTexture(0, lpMaterial->smTexture[0]->lpD3DTexture);

			if( lpMaterial->smTexture[0]->MapOpacity )
			{
				renderDevice.SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
				renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAREF, AlphaTestDepth);
				renderDevice.SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			}
			else
			{
				renderDevice.SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
			}
		}

		switch( lpMaterial->BlendType )
		{
		case SMMAT_BLEND_NONE:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
			break;
		case SMMAT_BLEND_ALPHA:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			break;
		case SMMAT_BLEND_COLOR:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			break;
		case SMMAT_BLEND_ADDCOLOR:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
			break;
		case SMMAT_BLEND_SHADOW:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR);
			break;
		case SMMAT_BLEND_LAMP:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
			break;
		case SMMAT_BLEND_INVSHADOW:
			renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
			renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			break;
		}

		TotalRendCnt += RenderD3D_OutLine( MatNum );
	}

	return TotalRendCnt;
}

// �� �� �� : �� �� ��
int smRENDER3D::RenderD3D_OutLine( int MatNum )
{
	if( smRendMaterial[MatNum].TextureCounter <= 0 && smRendMaterial[MatNum].AnimTexCounter <= 0 )
		return 0;

	if( RendMatrial[MatNum].MatrialCounter <= 0 )
		return 0;

	//BYTE dwSpecular[4];
	BYTE dwColor[4];
	
	float   *RenderBuff = RendBuff;
	smRENDFACE *rendface   = RendMatrial[ MatNum ].StartFace;
	int		    mcnt       = RendMatrial[ MatNum ].MatrialCounter;

	int   rendcnt = 0;
	for( int cnt=0; cnt < mcnt; cnt++ )
	{
		if( ! (rendface->ClipStatus & SMCLIP_DISPLAYOUT) )
		{
			for( int vcnt=0; vcnt < 3; vcnt++ )
			{
				smRENDVERTEX *rv = rendface->lpRendVertex[vcnt];
				smTEXLINK    *tl = rendface->lpTexLink;

				*(RenderBuff++) = (float)rv->xy[0] + (fOutLineX * fOutLineRatio);
				*(RenderBuff++) = (float)rv->xy[1] + (fOutLineY * fOutLineRatio);
				*(RenderBuff++) = (float)(rv->zb + (rv->rhw/2.f));
				*(RenderBuff++) = (float)rv->rhw;

				if( smRendMaterial[MatNum].SelfIllum != 0 )
					*((DWORD*)(RenderBuff++)) = (*((DWORD*)rv->bCol)) | 0x00FFFFFF;
				else
				{
					if( RendAlphaFlag && rv->tz > (ccDistZMin<<FLOATNS) )
					{
						*((DWORD*)dwColor) = *((DWORD*)rv->bCol);
						dwColor[3] = dwColor[3]/2 + dwColor[3]/3;

						*((DWORD*)(RenderBuff++)) = *((DWORD*)dwColor);
					}
					else
						*((DWORD*)(RenderBuff++)) = *((DWORD*)rv->bCol);
				}

				*(RenderBuff++) = (float)tl->u[vcnt];
				*(RenderBuff++) = (float)tl->v[vcnt];

				rendcnt++;
			}
		}

		rendface = rendface->NexRendFace;
	}

	renderDevice.DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1,
								(LPVOID)RendBuff, rendcnt, NULL );
	return rendcnt;
}

//���̽��� �ش��ϴ� ���̸� ���Ѵ�
int smGetPolyHeight( POINT3D *p1 , POINT3D *p2 , POINT3D *p3 , int x, int z );

//2d ��ǥ�� ���Ե� �������� ���̽��� ã�´�
int smRENDER3D::FindRenderFace2D( smRENDFACE **lpRendFace , int x, int y )
{
	int cnt,vcnt;
	POINT3D lpVertex2D[3];
	int z;

	smRENDFACE *lpLastRendFace = 0;
	int			LastDepthZ = 65536;
	//int			Mat;

//smMaterial[ face->Vertex[3] ].MeshState & SMMAT_STAT_CHECK_FACE

	for(cnt=0;cnt<nRendFace;cnt++) {
		//Mat = smMaterial[ RendFace[cnt].Matrial ].MeshState & SMMAT_STAT_CHECK_FACE;
		if ( (RendFace[cnt].ClipStatus&SMCLIP_DISPLAYOUT)==0 ) {//&& Mat) {
			for( vcnt=0;vcnt<3;vcnt++ ) {
				lpVertex2D[vcnt].x = (int)RendFace[cnt].lpRendVertex[vcnt]->xy[0];
				lpVertex2D[vcnt].z = (int)RendFace[cnt].lpRendVertex[vcnt]->xy[1];
				lpVertex2D[vcnt].y = int(RendFace[cnt].lpRendVertex[vcnt]->zb*65536);
			}

			z = smGetPolyHeight( &lpVertex2D[0],&lpVertex2D[1],&lpVertex2D[2] , x,y );
			if ( z!=CLIP_OUT && z<LastDepthZ ) {
				lpLastRendFace = &RendFace[cnt];
				LastDepthZ = z;
			}
		}
	}

	if ( lpLastRendFace ) {
		*lpRendFace = lpLastRendFace;
		return LastDepthZ;
	}

	lpRendFace = 0;
	return NULL;
}

//���̽��� �ش��ϴ� ���̸� ���Ѵ�
int smGetPolyHeight( POINT3D *p1 , POINT3D *p2 , POINT3D *p3 , int x, int z )
{

	POINT3D		*p[3];
	POINT3D		*ptop, *pmid , *pbot;

	int cnt;

	int lx , tx , bx;
	int ly , ty , by;
	int   lz , tz , bz;

	int   x1 , x2;
	int   y1 , y2;
	int	  ye,xe;
	int	  yl;

	p[0] = p1;
	p[1] = p2;
	p[2] = p3;

	ptop = p[0];
	pbot = p[1];
	pmid = p[2];
	
	//�ֻ��� ����Ʈ
	for(cnt=0;cnt<3;cnt++) {
		if ( p[cnt]->z<ptop->z ) ptop = p[cnt];
	}

	if ( ptop==pbot ) pbot = pmid;

	//������ ����Ʈ
	for(cnt=0;cnt<3;cnt++) {
		if ( p[cnt]->z>pbot->z && ptop!=p[cnt] ) pbot = p[cnt];
	}

	//�߰� ����Ʈ
	for(cnt=0;cnt<3;cnt++) {
		if ( ptop!=p[cnt] && pbot!=p[cnt] ) {
			pmid = p[cnt];
			break;
		}
	}

	if ( z<ptop->z || z>pbot->z ) 
		return CLIP_OUT;

	lz = pbot->z - ptop->z;
	tz = pmid->z - ptop->z;
	bz = pbot->z - pmid->z;

	if ( lz!=0 ) lx = ((pbot->x - ptop->x)<<FLOATNS) / lz ;
	else lx = 0;
	if ( tz!=0 ) tx = ((pmid->x - ptop->x)<<FLOATNS) / tz ;
	else tx = 0;
	if ( bz!=0 ) bx = ((pbot->x - pmid->x)<<FLOATNS) / bz ;
	else bx = 0;

	x1 = (( lx * ( z - ptop->z ) )>>FLOATNS) + ptop->x;

	if( z<pmid->z ) { 
		x2 = (( tx * ( z - ptop->z ) )>>FLOATNS) + ptop->x;
	}
	else {
		x2 = (( bx * ( z - pmid->z ) )>>FLOATNS) + pmid->x;
	}


	if ( lz!=0 ) ly = ((pbot->y - ptop->y)<<FLOATNS) / lz ;
	else ly = 0;
	if ( tz!=0 ) ty = ((pmid->y - ptop->y)<<FLOATNS) / tz ;
	else ty = 0;
	if ( bz!=0 ) by = ((pbot->y - pmid->y)<<FLOATNS) / bz ;
	else by = 0;

	y1 = (( ly * ( z - ptop->z ) ) >>FLOATNS) + ptop->y;

	if( z<pmid->z ) { 
		y2 = (( ty * ( z - ptop->z ) )>>FLOATNS) + ptop->y;
	}
	else {
		y2 = (( by * ( z - pmid->z ) )>>FLOATNS) + pmid->y;
	}

	if ( x1>x2 )  { 
		cnt=x1; x1=x2; x2=cnt;  
		cnt=y1; y1=y2; y2=cnt;  
	}

	if ( x<x1 || x>x2 ) 
		return CLIP_OUT;


	xe = x2-x1;
	ye = y2-y1;

	if ( xe!=0 ) yl = (ye<<FLOATNS) / xe;
	else yl = y1;

	return (( yl * (x - x1 ))>>FLOATNS)+y1;
}
