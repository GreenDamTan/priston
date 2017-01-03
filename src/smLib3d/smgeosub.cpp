#include "smd3d.h"

#define		ANG45		(ANGLE_90/2)
#define		ANG90		ANGLE_90
#define		ANG180		ANGLE_180
#define		ANG270		ANGLE_270
#define		ANG360		ANGLE_360

#define SH_ANGF_SFLOAT	(5+FLOATNS)

int	GeoResult_X;
int	GeoResult_Y;
int	GeoResult_Z;

//���� �ﰢ�� �ȿ� ��� �ִ���
bool PointInTri(float a1, float a2, float a3, float b1, float b2, float b3, float mouseX, float mouseY)
{
	bool abVert=false,bcVert=false,caVert=false; //���� �������� �ƴ���

	float   m1, m2, m3;		//����
	float   bb1, bb2, bb3;	        //y����
    bool    direction;              //������ ���ؼ� ������ ��ġ true�̸� �������� false�̸� ������ �Ʒ�
    int     inside=0;                 //x,y�� ���� � �ȿ� ��� �ִ��� 3�̸� �ﰢ���ȿ� �ִ�..


    // �� ���� ���� �����ĵ��� ���Ѵ�.
    if ((a2-a1) != 0.0f)
    {
		m1 = (b2-b1)/(a2-a1);   //���⸦ ���Ѵ�.
		bb1 = (b1)-(m1*a1);     //y������ ���Ѵ�.
	}
	else if((a2-a1)==0.0f)
        abVert = true;

	if ((a3-a2)!=0.0f)
	{
		m2 = (b3-b2)/(a3-a2);   //���⸦ ���Ѵ�.
        bb2 = (b2)-(m2*a2);     //y������ ���Ѵ�.
    }
	else if((a3-a2)==0.0f)
        bcVert = true;

	if((a1-a3)!=0.0f)
    {
		m3 = (b1-b3)/(a1-a3);   //���⸦ ���Ѵ�.
        bb3 = (b3)-(m3*a3);     //y������ ���Ѵ�.
    }
	else if((a1-a3)==0.0f)
        caVert = true;

        //�ﰢ���� ������ ���Ѵ�.
        float centerX = (a1+a2+a3)/3;
        float centerY = (b1+b2+b3)/3;


        if( (m1*centerX)+bb1-centerY >= 0) //0���� ũ�� ������ ������ ���� �ִ�.
                direction = true;          //������ ������ ���� �ִ�.
        else
                direction = false;         //������ ������ �Ʒ��� �ִ�.

        if (abVert==true)
        {
                if ( (a1<mouseX)&&(a1<centerX))
                        inside++;
                else if ( (a1>mouseX)&&(a1>centerX))
                        inside++;
        }
        else
        {
                if(direction==true)     //������ ������ ���� ���� ���
                {
                        if( (m1*mouseX)+bb1-mouseY >= 0)//���콺 ��ǥ�� ������ �Ʒ��� ������ �� �鿡 ���Ѵ�.
                        //if(mouseY <= (m1*mouseX)+bb1)
                                inside++;
                }
                else if(direction==false) //������ ������ �Ʒ��� ���� ���
                {
                        if( (m1*mouseX)+bb1-mouseY <= 0)//���콺 ��ǥ�� ������ ���� �־�� �� �鿡 ���Ѵ�.
                        //if (mouseY >= (m1*mouseX)+bb1)
                                inside++;
                }
        }

        if ( (m2*centerX)+bb2-centerY >= 0) //0���� ������ ������ ������ ���� �ִ�.
                direction = true;
        else
                direction = false;

        if (bcVert==true)
        {
                if( a2<mouseX && a2<centerX)
                        inside++;
                else if( a2>mouseX && a2>centerX)
                        inside++;
        }
        else
        {
                //������ ������ ���� ���� ���
                if(direction==true)
                {
                        if( (m2*mouseX)+bb2-mouseY >= 0)
                        //if(mouseY <= (m2*mouseX)+bb2)
                                inside++;
                }else if(direction==false)
                {
                        //if(mouseY >= (m2*mouseX)+bb2)
                        if( (m2*mouseX)+bb2-mouseY <= 0)
                                inside++;
                }
        }

        if ( (m3*centerX)+bb3-centerY >= 0)
                direction = true;
        else
                direction = false;

        if (caVert == true)
        {
                if ( a3<mouseX && a3<centerX)
                        inside++;
                else if( a3>mouseX && a3>centerX)
                        inside++;
        }
        else
        {
                if(direction==true)
                {
                        //if (mouseY <= (m3*mouseX)+bb3)
                        if ( (m3*mouseX)+bb3-mouseY >= 0)
                                inside++;
                }else if(direction==false)
                {
                        //if (mouseY >= m3*mouseX+bb3)
                        if ( (m3*mouseX)+bb3-mouseY <= 0)
                                inside++;
                }
        }

        if(inside!=3)
                return false;


        return true;
}

/*
//���� �ﰢ�� �ȿ� ��� �ִ���(�ﰢ�� ���ο� ��ġ�� ���� ���� �ʴ°ɷ� ó��) ->ȣ�ٶ� ����
bool    PointInTri(int a1, int b1, int a2, int b2, int a3, int b3, int pointX, int pointY)
{
    bool abVert=false,bcVert=false,caVert=false; //���� �������� �ƴ���

    float   m1, m2, m3;		        //����
    float   bb1, bb2, bb3;	        //y����
    bool    direction;              //������ ���ؼ� ������ ��ġ true�̸� �������� false�̸� ������ �Ʒ�
    int     inside=0;                //x,y�� ���� � �ȿ� ��� �ִ��� 3�̸� �ﰢ���ȿ� �ִ�..


    // �� ���� ���� �����ĵ��� ���Ѵ�.
    if(a1 != a2)
    {
        m1  = float((b2-b1)/(a2-a1));   //���⸦ ���Ѵ�.
        bb1 = float((b1)-(m1*a1));     //y������ ���Ѵ�.
    }else if(a1 == a2)
    abVert = true;

    if(a2 != a3)
    {
        m2  = float((b3-b2)/(a3-a2));   //���⸦ ���Ѵ�.
        bb2 = float((b2)-(m2*a2));     //y������ ���Ѵ�.
    }else if(a2 = a3)
    bcVert = true;

    if(a1 != a3)
    {
        m3  = float((b1-b3)/(a1-a3));   //���⸦ ���Ѵ�.
        bb3 = float((b3)-(m3*a3));     //y������ ���Ѵ�.
    }else if(a1 == a3)
    caVert = true;

    //�ﰢ���� ������ ���Ѵ�.
    float centerX = (float)((a1+a2+a3)/3);
    float centerY = (float)((b1+b2+b3)/3);

    if( (m1*centerX)+bb1-centerY >= 0) //0���� ũ�� ������ ������ ���� �ִ�.
        direction = true;          //������ ������ ���� �ִ�.
    else
        direction = false;         //������ ������ �Ʒ��� �ִ�.

    if (abVert==true)
    {
        if ( (a1<pointX)&&(a1<centerX))
            inside++;
        else if ( (a1>pointX)&&(a1>centerX))
            inside++;
    }
    else
    {
        if(direction==true)     //������ ������ ���� ���� ���
        {
            if( (m1*pointX)+bb1-pointY > 0)//���콺 ��ǥ�� ������ �Ʒ��� ������ �� �鿡 ���Ѵ�.
                inside++;
        }
        else if(direction==false) //������ ������ �Ʒ��� ���� ���
        {
            if( (m1*pointX)+bb1-pointY < 0)//���콺 ��ǥ�� ������ ���� �־�� �� �鿡 ���Ѵ�.
                inside++;
        }
    }

    if ( (m2*centerX)+bb2-centerY >= 0) //0���� ������ ������ ������ ���� �ִ�.
        direction = true;
    else
        direction = false;

    if (bcVert==true)
    {
        if( a2<pointX && a2<centerX)
            inside++;
        else if( a2>pointX && a2>centerX)
            inside++;
    }
    else
    {
        //������ ������ ���� ���� ���
        if(direction==true)
        {
            if( (m2*pointX)+bb2-pointY > 0)
                inside++;
        }else if(direction==false)
        {
            if( (m2*pointX)+bb2-pointY < 0)
                inside++;
        }
    }

    if ( (m3*centerX)+bb3-centerY >= 0)
        direction = true;
    else
        direction = false;

    if (caVert == true)
    {
        if ( a3<pointX && a3<centerX)
            inside++;
        else if( a3>pointX && a3>centerX)
            inside++;
    }
    else
    {
        if(direction==true)
        {
            if ( (m3*pointX)+bb3-pointY > 0)
                inside++;
        }else if(direction==false)
        {
            if ( (m3*pointX)+bb3-pointY < 0)
                inside++;
        }
    }

    if(inside!=3)
        return false;

    return true;
}
*/

// �̵��� ���� ��ǥ���� ���ϴ� ���̱���! �����.
void GetMoveLocation(int x, int y, int z, int AngX, int AngY, int AngZ )
{
	int i,j,k;
	int dx,dy,dz;

	int sinX, sinY, sinZ;
	int cosX, cosY, cosZ;

	AngX &=ANGCLIP;		
	AngY &=ANGCLIP;
	AngZ &=ANGCLIP;

	sinX = sdGetSin[AngX]>>2;
	sinY = sdGetSin[AngY]>>2;
	sinZ = sdGetSin[AngZ]>>2;
	cosX = sdGetCos[AngX]>>2;
	cosY = sdGetCos[AngY]>>2;
	cosZ = sdGetCos[AngZ]>>2;

	// now Z axis rotation
    i = (x * cosZ - y * sinZ) >> SH_ANGF_SFLOAT;
    j = (x * sinZ + y * cosZ) >> SH_ANGF_SFLOAT;
	k = z;

	// now X axis rotation
    dy = (j * cosX - k * sinX) >> SH_ANGF_SFLOAT;
    dz = (j * sinX + k * cosX) >> SH_ANGF_SFLOAT;
	k = dz;

	// now Y axis
    dx = (k * sinY + i * cosY) >> SH_ANGF_SFLOAT;
    dz = (k * cosY - i * sinY) >> SH_ANGF_SFLOAT;

	GeoResult_X=dx;
	GeoResult_Y=dy;
	GeoResult_Z=dz;
}

// Rotate matrix about X axis
void smERotateXMatrixSin( smEMATRIX &m , int sine , int cosine )
{
	m._22 = cosine;
	m._23 = sine;
	m._32 = -sine;
	m._33 = cosine;
}

// Rotate matrix about Y axis
void smERotateYMatrixSin( smEMATRIX &m , int sine , int cosine )
{
	m._11 = cosine;
	m._13 = -sine;
	m._31 = sine;
	m._33 = cosine;
}

// Rotate matrix about Z axis
void smERotateZMatrixSin( smEMATRIX &m , int sine , int cosine )
{
	m._11 = cosine;
	m._12 = sine;
	m._21 = -sine;
	m._22 = cosine;
}

// Rotate matrix about X axis
static void smFRotateXMatrixSin( smFMATRIX &m , float sine , float cosine )
{
	m._22 = cosine;
	m._23 = sine;
	m._32 = -sine;
	m._33 = cosine;
}

// Rotate matrix about Y axis
static void smFRotateYMatrixSin( smFMATRIX &m , float sine , float cosine )
{
	m._11 = cosine;
	m._13 = -sine;
	m._31 = sine;
	m._33 = cosine;
}

// Rotate matrix about Z axis
static void smFRotateZMatrixSin( smFMATRIX &m , float sine , float cosine )
{
	m._11 = cosine;
	m._12 = sine;
	m._21 = -sine;
	m._22 = cosine;
}

void smEMatrixFromFMatrix( smEMATRIX &m , smFMATRIX &d )
{
	m._11 = (int)( d._11 * 32768 );
	m._12 = (int)( d._12 * 32768 );
	m._13 = (int)( d._13 * 32768 );
	m._14 = (int)( d._14 * 32768 );

	m._21 = (int)( d._21 * 32768 );
	m._22 = (int)( d._22 * 32768 );
	m._23 = (int)( d._23 * 32768 );
	m._24 = (int)( d._24 * 32768 );

	m._31 = (int)( d._31 * 32768 );
	m._32 = (int)( d._32 * 32768 );
	m._33 = (int)( d._33 * 32768 );
	m._34 = (int)( d._34 * 32768 );

	m._41 = (int)( d._41 * 32768 );
	m._42 = (int)( d._42 * 32768 );
	m._43 = (int)( d._43 * 32768 );
	m._44 = (int)( d._44 * 32768 );
}

/*
//ȸ���� ����
int smBBALL::ModifyRotate()
{
	double x,y,z,w,s;
	double sx,sy,sz,sw;
	double CosX , SinX , CosY , SinY;
	smDMATRIX *mat;

	SetDispRotate();

	x = 0;
	y = 0;
	z = 1000;

	mat = &dmBallMatrix;

	sx = x * mat->_11 +
		 y * mat->_21 +
		 z * mat->_31 ;
	sy = x * mat->_12 +
		 y * mat->_22 +
		 z * mat->_32 ;
	sz = x * mat->_13 +
		 y * mat->_23 +
		 z * mat->_33 ;


		x = sx;
		y = -sy;
		z = sz;

		w = sqrt( x*x + z*z );
		s = sqrt( w*w + y*y );

		CosX = w / s;
		SinX = y / s;
		if ( w==0 ) {
			CosY = 0;
			SinY = 1.0f;
		}
		else {
			CosY = z / w;
			SinY = x / w;
		}

		smDMATRIX	fMatX , fMatY;

		smDIdentityMatrix( fMatX );
		smDIdentityMatrix( fMatY );

		smDRotateXMatrixSin( fMatX , SinX , CosX );
		smDRotateYMatrixSin( fMatY , SinY , CosY );

		smDMatrixMult( dmBallMatrix , fMatX, fMatY );

	return TRUE;
}
*/

// ī�޶� ���� ��Ʈ���� ����� ( sx,sy,sz -ī�޶�  tx,ty,tz - ������ )
int MakeTraceMatrix ( eCAMERA_TRACE *lpeTrace , int sx, int sy, int sz, int tx, int ty, int tz )
{
	double dx,dy,dz,du,dw;
	double wx,wy,wz,ww;
	float  fCosX,fCosY,fSinX,fSinY;

	smFMATRIX	fMat , fInvMat , fMatRotX , fMatRotY;

	dx = ((double)(tx-sx))/fONE;
	dy = -((double)(ty-sy))/fONE;
	dz = ((double)(tz-sz))/fONE;

	wx = dx*dx;
	wy = dy*dy;
	wz = dz*dz;

	ww = wx+wz;
	dw = sqrt( ww );
	du = sqrt( ww+wy );

	fCosX = (float)(dw/du);
	fSinX = (float)(dy/du);
	if ( dw==0 ) {
		fCosY = 0.0f;
		fSinY = 1.0f;
	}
	else {
		fCosY = (float)(dz/dw);
		fSinY = (float)(dx/dw);
	}

	//smEIdentityMatrix( eMatrix );
	smFIdentityMatrix( fMatRotX );
	smFIdentityMatrix( fMatRotY );
	smFIdentityMatrix( fInvMat );

	smFRotateXMatrixSin( fMatRotX , fSinX , fCosX );
	smFRotateYMatrixSin( fMatRotY , fSinY , fCosY );

	smFMatrixMult( fMat , fMatRotX , fMatRotY );

//	smEMatrixFromFMatrix( lpeTrace->eRotMatrix , fMat );

	smFMatrixInvert( fInvMat, fMat );
	smEMatrixFromFMatrix( lpeTrace->eRotMatrix , fInvMat );

	lpeTrace->x = sx;
	lpeTrace->y = sy;
	lpeTrace->z = sz;

	lpeTrace->tx = tx;
	lpeTrace->ty = ty;
	lpeTrace->tz = tz;

	GetRadian3D( sx,sy,sz,tx,ty,tz );
	lpeTrace->AngX = (GeoResult_X)&ANGCLIP;
	lpeTrace->AngY = (GeoResult_Y)&ANGCLIP;
	

	return TRUE;
}

int TestTraceMatrix()
{
	int x,y,z;
	int tx,ty,tz;

	eCAMERA_TRACE	eTrace;

	tx = 100*fONE;
	ty = 50*fONE;
	tz = 300*fONE;

	x = y= z = 0;

	MakeTraceMatrix ( &eTrace , x,y,z, tx,ty,tz );

	smEMatrixShift( eTrace.eRotMatrix , 4 );
/*
	tx = 0;
	ty = 0;
	tz = 600*fONE;
*/

	int rx,ry,rz;


		rx = tx * eTrace.eRotMatrix._11 +
			 ty * eTrace.eRotMatrix._21 +
			 tz * eTrace.eRotMatrix._31 ;

		ry = tx * eTrace.eRotMatrix._12 +
			 ty * eTrace.eRotMatrix._22 +
			 tz * eTrace.eRotMatrix._32 ;

		rz = tx * eTrace.eRotMatrix._13 +
			 ty * eTrace.eRotMatrix._23 +
			 tz * eTrace.eRotMatrix._33 ;
/*
		rx >>=11;
		ry >>=11;
		rz >>=11;
*/

		rx /=2048;
		ry /=2048;
		rz /=2048;

		double dx,dy,dz;
		dx = tx/fONE;
		dy = ty/fONE;
		dz = tz/fONE;

		x=(int)(sqrt(dx*dx+dy*dy+dz*dz)*fONE);


	// ī�޶� ���� ��Ʈ���� ����� ( sx,sy,sz -ī�޶�  tx,ty,tz - ������ )
	//int MakeTraceMatrix ( eCAMERA_TRACE *lpeTrace , int sx, int sy, int sz, int tx, int ty, int tz )
/*
		x -= CameraPosiX;
		y -= CameraPosiY;
		z -= CameraPosiZ;

		//�߰��ϸ鼭 ī�޶� ��ȯ ��Ʈ������ ���Ͽ� ī�޶� ��ǥ�� ��ȯ�Ͽ� ����
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

		RendVertex[ nRendVertex ].tx = x ;
		RendVertex[ nRendVertex ].ty = y ;
		RendVertex[ nRendVertex ].tz = z ;
*/

	return TRUE;
}

// 2���� ������ ������ ��ǥ�� ���Ͽ� ũ�� �ڽ��� �ִ����� �����ϴ� �Լ��δ�!
// ��� �Ÿ��� ���� �����ϴ� �뵵��.
int CheckLocateBox( int sx, int sz , int tx, int tz , int size )
{
	int lx,lz;

	lx = abs( tx - sx );
	lz = abs( tz - sz );

	if ( lx < size && lz < size ) return 1;

	return NULL;
}

//  ����� ��ġ�� �ľ��Ͽ� �̵��ؾ� �� ������ �˷��ش�.
void GetTraceAngle( POINT3D *posi , POINT3D *angle , int tx , int ty , int tz )
{
	GetRadian3D( posi->x, posi->y, posi->z, tx, ty, tz );

	GeoResult_Y = GetSubAngle( angle->y , GeoResult_Y ) ;
	GeoResult_X = GetSubAngle( angle->x , (GeoResult_X & ANGCLIP ) ) ;
}

// �ΰ��� ���� ���Ѵ�.
int GetSubAngle( int a1, int a2 )
{
	int	i;

	i = (a2 - a1);
	
	if ( i > ANG180 ) i = -ANG360 + i;
	if ( i<=-ANG180 ) i = ANG360 + i;
	
	return i;
}

// ���� ������ 3���� ���� ���ϴ� �Լ� ( �̼����� ®�� )
void GetRadian3D(int x1 , int y, int y1, int x2, int y_, int y2 )
{
	int flag = 0;


	if (x1 > x2)
	{
	   flag |= 0x0001;
	   x1 ^= x2;
	   x2 ^= x1;
	   x1 ^= x2;
	}

	if (y1 > y2)
	{
	   flag |= 0x0002;
	   y1 ^= y2;
	   y2 ^= y1;
	   y1 ^= y2;
	}

	x2 -= (x1-1); y2 -= (y1-1);

	if (y2 > x2)
	{
	   flag |= 0x0004;
	   y2 ^= x2;
	   x2 ^= y2;
	   y2 ^= x2;
	}

  y2 <<= 8;
  y2 /= x2;
  x1 = (y2 * ANG45);
  x1 >>= 8;

  y2 = zDCos(x1)>>8;

  if (!y2) y2=1;

//  y1 = ((x2 << 16) / y2) >> 8;
  y1 = ((x2 << 8) / y2);

  x2 ^= x2;
  if (y > y_)
  {
   x2 |=  0x0001;
   y ^= y_;
   y_ ^= y;
   y ^= y_;
  }

  y2 = y_ - (y-1);

  if (y2 > y1)
  {
   x2 |= 0x0002;
   y2 ^= y1;
   y1 ^= y2;
   y2 ^= y1;
  }

  switch (flag)
  {
   case 2:x1 +=  ANG90;break;
   case 3:x1 = ANG270 - x1;break;
   case 0:x1 = ANG90 - x1;break;
   case 1:x1 += ANG270;break;
   case 6:x1 = ANG180 - x1;break;
   case 7:x1 += ANG180;break;
   case 5:x1 = ANG360 - x1;break;
  }
  GeoResult_Y = x1;

  y2 <<= 8;
  y2 /= y1;
  x1 = (y2 * ANG45);
  x1 >>= 8;

  switch (x2)
  {
   case 3:x1 = (ANG90 - x1)  ;break;
   case 0:x1 = (ANG360 - x1)  ;break;
   case 2:x1 = (ANG270 + x1) ;break;
  }

  GeoResult_X = x1;
}

// ���� ������ 2���� ���� ���ϴ� �Լ� ( �̰͵� ���� �̼����� ®�� )
int GetRadian2D(int x1 ,int y1, int x2, int y2 )
{

	int flag = 0;

  if (x1 > x2)
  {
   flag |= 0x0001;
   x1 ^= x2;x2 ^= x1;x1 ^= x2;
  }

  if (y1 > y2)
  {
   flag |= 0x0002;
   y1 ^= y2;y2 ^= y1;y1 ^= y2;
  }

  x2 -= (x1-1); y2 -= (y1-1);

  if (y2 > x2)
  {
   flag |= 0x0004;
   y2 ^= x2;x2 ^= y2;y2 ^= x2;
  }

  y2 <<= 8;
  y2 /= x2;
  x1 = (y2 * ANG45);
  x1 >>= 8;

  

  switch (flag)
  {
   case 2:x1 +=  ANG90;break;
   case 3:x1 = ANG270 - x1;break;
   case 0:x1 = ANG90 - x1;break;
   case 1:x1 += ANG270;break;
   case 6:x1 = ANG180 - x1;break;
   case 7:x1 += ANG180;break;
   case 5:x1 = ANG360 - x1;break;
  }
  return x1;
}

// ���� ������ �Ÿ��� ���ϴ� �Լ� ( ���� �̼����� ������� )
int GetDistance(int x1 , int y, int y1, int x2, int y_, int y2 )
{
  if (x1 > x2)
  {
   x1 ^= x2;x2 ^= x1;x1 ^= x2;
  }

  if (y1 > y2)
  {
   y1 ^= y2;y2 ^= y1;y1 ^= y2;
  }

  x2 -= (x1-1); y2 -= (y1-1);

  if (y2 > x2)
  {
   y2 ^= x2;x2 ^= y2;y2 ^= x2;
  }

  y2 <<= 8;
  y2 /= x2;
  x1 = (y2 * ANG45);
  x1 >>= 8;

  y2 = zDCos(x1)>>8;

  if (!y2) y2=1;

  y1 = ((x2 << 8) / y2); 

 
  if (y > y_)
  {
   y ^= y_;y_ ^= y;y ^= y_;
  }

  y2 = y_ - (y-1);

  if (y2 > y1)
  {
   y2 ^= y1;y1 ^= y2;y2 ^= y1;
  }

  y2 <<= 8;
  y2 /= y1;
  x1 = (y2 * ANG45);
  x1 >>= 8;

  y2 = zDCos(x1)>>8;
 if (!y2) y2=1;
  x2 = ((y1 << 8) / y2);

  return x2;

}


