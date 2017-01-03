#include "common.h"
#include "smd3d.h"


#define		CLIP_OUT		-32767
#define		ANGCLIP			ANGLE_MASK


int	DEBUG_OBJSUM =0;

/*
int MidX	=		640	/2;
int MidY	=		480	/2;
int	viewdistZ = 640;//9;//8;
*/

/*
extern int	DispPolyCnt;
extern int GetMapPoly( int x, int z , POINT3D *p[] );
extern POINT3D pLight;
extern smLIGHTLEVEL *smLightD3D( int r, int g, int b );
extern int smConvLightD3D( smRGB *color , smRGB *specular );
extern int smSetColorZclip( int z , smRGB *color , smRGB *specular );

extern void smLightD3D( smLIGHTLEVEL *LightLevel , smRGB *color , smRGB *specular );
extern int smMergeDarkness( smRGB *light, int r,int g,int b );
extern int MouseW, MouseH;
*/
extern int smMergeLight( smRGB *light, int r,int g,int b );

// �������� ���õ� ���ؽ� ������
//smPOINT3D	*smRendVertex = new smPOINT3D [ RENDER_VERTEX_MAX ];
smPOINT3D	smTestVertex;
smVERTEX	smWorldVertex[ RENDER_VERTEX_MAX ];

DWORD		smObjFixPos = 0;			//������ ��ǥ ����


/*
int smAddVertexLight( smRGB *LightRGB , smLIGHT3D *light , POINT3D *posi , POINT3D *normal )
{

	POINT3D p;
	POINT3D c;
	int dx;
	int r,g,b;
	int col;

	p.x =  posi->x - light->x;
	p.z =  posi->z - light->z;
	p.y =  posi->y - light->y;

	dx = (p.x * p.x + p.z * p.z + p.y * p.y )>>2;
	col = DotProduct( &p,normal )>>8;

	if ( col>0 ) {
		col = 65535-dx;
		if ( col<0 ) col = 0;
	}
	else {
		col = 0;
	}


//	dx = (p.x * p.x + p.z * p.z + p.y * p.y )>>16;
//	col = DotProduct( &p,normal )>>8;

//	if ( dx!=0 ) {
//		col = col/dx;
//	}

	if ( col>65535 ) col = 65535;
	if ( col<0 ) col = 0;

	r = (light->r * col) >> 8; 
	g = (light->g * col) >> 8; 
	b = (light->b * col) >> 8; 

	smMergeLight( LightRGB , r,g,b );

	return TRUE;
}

int smAddVertexLightSpot( smRGB *LightRGB , smLIGHT3D *light , POINT3D *posi , POINT3D *normal )
{

	POINT3D p1,p2,normalp;
	int dx;
	int r,g,b;
	int col;

	p1.x =  posi->x - light->x;
	p1.z =  posi->z - light->z;
	p1.y =  posi->y - light->y;
				
	p2.x = 255;
	p2.y = 0;
	p2.z = 255;


//	normalp.x = 0 ;
//	normalp.y = -255;
//	normalp.z = 0;
//	if ( dx!=0 ) {
//		col = col/dx;
//	}


	col = DotProduct( &p2, &p1 ) >>8;
	dx = (p1.x * p1.x + p1.z * p1.z + p1.y * p1.y )>>2;


	if ( col>0 ) {
		col = DotProduct( &p1,normal )>>8;

		if ( col>0 ) {
			col = 65535-dx;
			if ( col<0 ) col = 0;
		}
		else {
			col = 0;
		}
	}
	else {
		col = 0;
	}

	if ( col>65535 ) col = 65535;
	if ( col<0 ) col = 0;

	r = (light->r * col) >> 8; 
	g = (light->g * col) >> 8; 
	b = (light->b * col) >> 8; 

	smMergeLight( LightRGB , r,g,b );

	return TRUE;
}




int GetPolyHeight( POINT3D *p1 , POINT3D *p2, POINT3D *p3 , int x, int z )
{

//	smGEOMAP	*gtop,*gmid,*gbot;

	POINT3D		*p[3];
	POINT3D		*ptop, *pmid , *pbot;

	int cnt;

	float lx , tx , bx;
	float ly , ty , by;
//	int   ly , ty , by;
	int   lz , tz , bz;

	int   x1 , x2;
	int   y1 , y2;
	int	  ye,xe;
	float	yl;

	p[0] = p1;
	p[1] = p2;
	p[2] = p3;

	ptop = p1;
	pbot = p2;
	pmid = p3;
	
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

	if ( lz!=0 ) lx = ((float)(pbot->x - ptop->x)) / ((float)lz) ;
	else lx = 0;
	if ( tz!=0 ) tx = ((float)(pmid->x - ptop->x)) / ((float)tz) ;
	else tx = 0;
	if ( bz!=0 ) bx = ((float)(pbot->x - pmid->x)) / ((float)bz) ;
	else bx = 0;

	x1 = ( lx * ((float)( z - ptop->z )) ) + ptop->x;

	if( z<pmid->z ) { 
		x2 = ( tx * ((float)( z - ptop->z )) ) + ptop->x;
	}
	else {
		x2 = ( bx * ((float)( z - pmid->z )) ) + pmid->x;
	}


	if ( lz!=0 ) ly = ((float)(pbot->y - ptop->y)) / ((float)lz) ;
	else ly = 0;
	if ( tz!=0 ) ty = ((float)(pmid->y - ptop->y)) / ((float)tz) ;
	else ty = 0;
	if ( bz!=0 ) by = ((float)(pbot->y - pmid->y)) / ((float)bz) ;
	else by = 0;

	y1 = ( ly * ((float)( z - ptop->z )) ) + ptop->y;

	if( z<pmid->z ) { 
		y2 = ( ty * ((float)( z - ptop->z )) ) + ptop->y;
	}
	else {
		y2 = ( by * ((float)( z - pmid->z )) ) + pmid->y;
	}

	if ( x1>x2 )  { 
		cnt=x1; x1=x2; x2=cnt;  
		cnt=y1; y1=y2; y2=cnt;  
	}

	if ( x<x1 || x>x2 ) 
		return CLIP_OUT;


	xe = x2-x1;
	ye = y2-y1;

	if ( xe!=0 ) yl = ((float)ye)/((float)xe);
	else yl = (float)y1;

	return ( yl * ((float)(x - x1 )) )+y1;

}

int smGetMapHeight( int x, int z )
{
	POINT3D	*MapPoly[3];
	POINT3D	FloorPoly[3];

	MapPoly[0] = &FloorPoly[0];
	MapPoly[1] = &FloorPoly[1];
	MapPoly[2] = &FloorPoly[2];

	GetMapPoly( x, z , &MapPoly[0] );
	return GetPolyHeight( MapPoly[0] , MapPoly[1] , MapPoly[2] , x, z );

}
*/



//ȭ�鿡 ��� ���θ� �׽�Ʈ
int TestScreenView( int x , int y , int z ,  int Bound , POINT3D *LocalAngle , POINT3D *GlobalAngle )
{
	int bounds;
	int pz;
	int gx,gy,gz;
	int Trig[6];

	smPOINT3D	smTestVertex;
	smVERTEX	ZeroVertex;

	ZeroVertex.x = 0;
	ZeroVertex.y = 0;
	ZeroVertex.z = 0;

	gx = (-GlobalAngle->x) & ANGCLIP;
	gy = (-GlobalAngle->y) & ANGCLIP;
	gz = (-GlobalAngle->z) & ANGCLIP;

	smTestVertex.SetTo( &ZeroVertex );
	smTestVertex.Move( x , y, z );

	Trig[0] = GetSin[gx]>>FLOATDS;
	Trig[1] = GetCos[gx]>>FLOATDS;
	Trig[2] = GetSin[gy]>>FLOATDS;
	Trig[3] = GetCos[gy]>>FLOATDS;
	Trig[4] = GetSin[gz]>>FLOATDS;
	Trig[5] = GetCos[gz]>>FLOATDS;

	smTestVertex.GlobalRotate( Trig );

	pz = smTestVertex.Z >> FLOATNS;

	if ( (pz+Bound)>0 && (pz<ViewPointLen) ) {
		if ( pz==0 ) pz = 1;
		pz=abs(pz);
		bounds = (Bound * viewdistZ) / pz;

		if ( (smTestVertex.x2d+bounds)>0 &&
			(smTestVertex.x2d - bounds)<renderDevice.GetScreenWidth() &&
			 (smTestVertex.y2d+bounds)>0  &&
			 (smTestVertex.y2d - bounds)<renderDevice.GetScreenHeight())

			 return TRUE;
	}

	return FALSE;
}



//////////////////////// smPOINT3D Function ////////////////////////

smPOINT3D::smPOINT3D()
{

}

smPOINT3D::smPOINT3D( smVERTEX *pv )
{
    SetTo( pv );
}

smPOINT3D::~smPOINT3D()
{

}

void smPOINT3D::SetTo( smVERTEX *pv )
{
	pVertex = pv;

    rx = pv->x;
    ry = pv->y;
    rz = pv->z;

    sx = pv->nx;
    sy = pv->ny;
    sz = pv->nz;
}

//ȭ�鿡 ����
void smPOINT3D::xform2d()
{

    if (Z > 0)
    {
		x2d = MidX + ((X * viewdistZ) / Z);
		y2d = MidY - ((Y * viewdistZ) / Z);
    }
    else
    {
		x2d = CLIP_OUT;
		y2d = CLIP_OUT;
    }
}

//���� ��ǥ ȸ��
void smPOINT3D::GlobalRotate(int *trig)
{
	int i,j,k;

	// Y �� ȸ��
    i = ( wz * trig[2] + wx * trig[3]) >> FLOATNS;
	j = wy;
    k = (wz * trig[3] - wx * trig[2]) >> FLOATNS;

	// X �� ȸ��
    Y = (j * trig[1] - k * trig[0]) >> FLOATNS;
    Z = (j * trig[0] + k * trig[1]) >> FLOATNS;
	j = Y;

	// Z �� ȸ��
    X = (i * trig[5] - j * trig[4]) >> FLOATNS;
    Y = (i * trig[4] + j * trig[5]) >> FLOATNS;

    xform2d();
}

//   ���� �߽� ��ǥ �̵�
void smPOINT3D::Move(int dx, int dy, int dz)
{
	ox = dx<<FLOATNS;
	oy = dy<<FLOATNS;
	oz = dz<<FLOATNS;

	GlobalXform();
}

//   ī�޶� ��ǥ
void smPOINT3D::GlobalXform()
{
    wx = rx + ox;
    wy = ry + oy;
    wz = rz + oz;
}


/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////[ OBJ3D Ŭ���� ]///////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////


smOBJ3D::smOBJ3D()
{
	NodeName[0] = 0;
	NodeParent[0] = 0;
	smIdentityMatrix( Tm );

	pParent = 0;

	TmRot = 0;
	TmPos = 0;
	TmScale = 0;
	TmRotCnt = 0;
	TmPosCnt = 0;
	TmScaleCnt = 0;
	TmPrevRot = 0;

	Face=0;
	Vertex=0;
	TexLink=0;

	Physique = 0;

}

smOBJ3D::~smOBJ3D()
{

	if ( TmScale ) delete TmScale;
	if ( TmPos ) delete TmPos;
	if ( TmRot ) delete TmRot;
	if ( TmPrevRot ) delete TmPrevRot;

	if ( TexLink ) delete TexLink;
	if ( Physique ) delete Physique;
	if ( Face ) delete Face;
	if ( Vertex ) delete Vertex;

}

smOBJ3D::smOBJ3D( int nv , int nf )
{
	NodeName[0] = 0;
	NodeParent[0] = 0;
	smIdentityMatrix( Tm );

	pParent = 0;

	TmRot = 0;
	TmPos = 0;
	TmScale = 0;
	TmPrevRot = 0;
	TmRotCnt = 0;
	TmPosCnt = 0;
	TmScaleCnt = 0;

	Face=0;
	Vertex=0;
	TexLink=0;

	Physique = 0;

	Init( nv , nf , 0 );

}

//=========================== [ �ʱ�ȭ �� ���� ] =============================//

// �ʱ�ȭ��ƾ ( ���ؽ� , ���̽��� �� , �׸��� ����ũ ��뿩��  )
void smOBJ3D::Init( int nv, int nf , int pFlag  )
{
	int cnt;

	MaxVertex	= nv;
	MaxFace	= nf;

	nVertex	= 0;
	nFace		= 0;

	dBound		= 0;							// �ٿ�� ����� �� ^2
	Bound		= 0;							// �ٿ�� ����� �� 

	maxX = maxY = maxZ = 0;
	minX = minY = minZ = 0;

	ColorEffect = TRUE;							// ���� ȿ�� ��� �⺻�� 
	//Ŭ���� ��尪
	ClipStates = ( SMCLIP_NEARZ | SMCLIP_FARZ | SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM );

	Vertex = new smVERTEX [ nv ];
	Face = new smFACE [ nf ];

	TexLink = new smTEXLINK[ nf * 2 ];
	nTexLink = 0;

	// ����ũ �ִϸ��̼� ����
	if ( pFlag ) {
		Physique = new smOBJ3D *[ nv ];
		for( cnt=0;cnt<nv;cnt++) Physique[cnt] = 0;
	}

	Posi.x = 0;
	Posi.y = 0;
	Posi.z = 0;

	Angle.x = 0;
	Angle.y = 0;
	Angle.z = 0;

	ZeroVertex.x = 0;
	ZeroVertex.y = 0;
	ZeroVertex.z = 0;

	TmFrameCnt = 0;

	smIdentityMatrix( mLocal );



}

// ���ؽ� ��ǥ �߰�
int smOBJ3D::AddVertex( int x, int y, int z )
{
	int bound;
	int mx,my,mz;

	Vertex[nVertex].x = x;
	Vertex[nVertex].y = y;
	Vertex[nVertex].z = z;
	nVertex ++;


	// ��ǥ �ִ�ġ ����
	mx = abs(x);
	my = abs(y);
	mz = abs(z);

	if ( maxX<mx ) maxX = mx;
	if ( maxZ<my ) maxZ = my;
	if ( maxY<mz ) maxY = mz;

	//�ٿ�� ����� �����
	mx = x>>FLOATNS;
	my = y>>FLOATNS;
	mz = z>>FLOATNS;

	bound = abs ( (mx*mx) + (my*my) + (mz*mz) );

	if ( dBound<bound ) dBound = bound;

	if ( nVertex==MaxVertex ) 
		Bound = (int)sqrt( (long double)dBound );

	return nVertex-1;
}

// ���̽��� ���ؽ� ��ȣ �߰�
int smOBJ3D::AddFace( int a, int b, int c )
{
	Face[nFace].v[0] = a;
	Face[nFace].v[1] = b;
	Face[nFace].v[2] = c;
	Face[nFace].v[3] = 0;			// Texture

	Face[nFace].lpTexLink = 0;

	nFace ++;

	return nFace-1;
}

//���̽��� ��Ʈ���� �Է�
int smOBJ3D::SetFaceMaterial( int FaceNum , int MatNum  )
{
	Face[ FaceNum ].v[3] = MatNum;

	return TRUE;
}

//�ؽ��� ��ǥ������ �߰��Ѵ� 
int smOBJ3D::AddTexLink( int FaceNum , DWORD *hTex , smFTPOINT *t1 , smFTPOINT *t2 , smFTPOINT *t3 )
{
	int cnt;
	smTEXLINK	*tl;

	//��ǥ����
	TexLink[ nTexLink ].u[0] = t1->u;
	TexLink[ nTexLink ].v[0] = t1->v;
	TexLink[ nTexLink ].u[1] = t2->u;
	TexLink[ nTexLink ].v[1] = t2->v;
	TexLink[ nTexLink ].u[2] = t3->u;
	TexLink[ nTexLink ].v[2] = t3->v;
	TexLink[ nTexLink ].hTexture = hTex;
	TexLink[ nTexLink ].NextTex = 0;

	//������ ���̽��� �����Ų��
	if ( !Face[FaceNum].lpTexLink ) {
		//������ �ؽ�����ǥ�� ���
		Face[FaceNum].lpTexLink = &TexLink[ nTexLink ];
	}
	else {
		//�̹� ������ִ� ��ǥ�� �ִ°��
		tl = Face[FaceNum].lpTexLink;

		for( cnt=0;cnt<8;cnt++ ) {
			if ( !tl->NextTex ) {
				//���������� ����� ��ǥ�� ã�� ����
				tl->NextTex = &TexLink[ nTexLink ];
				break;
			}
			else {
				//���� ��ǥ�� �ѱ�
				tl = tl->NextTex;
			}

		}
	}

	nTexLink++;

	return nTexLink-1;
}

//�ؽ��� ��ǥ������ �߰��Ѵ� 
int smOBJ3D::AddTexLinkTP( int FaceNum , DWORD *hTex , smTPOINT *t1 , smTPOINT *t2 , smTPOINT *t3 )
{
	smFTPOINT f1,f2,f3;

	f1.u = ((float)t1->u)/256;
	f1.v = ((float)t1->v)/256;
	f2.u = ((float)t2->u)/256;
	f2.v = ((float)t2->v)/256;
	f3.u = ((float)t3->u)/256;
	f3.v = ((float)t3->v)/256;

	return AddTexLink( FaceNum , hTex, &f1, &f2, &f3 );
}

// ���̽� �� �ؽ��� ���ÿ� �߰�
int smOBJ3D::AddFace( int a, int b, int c , smTPOINT *t1 , smTPOINT *t2, smTPOINT *t3 )
{
	int num;
	smFTPOINT f1,f2,f3;

	f1.u = ((float)t1->u)/256;
	f1.v = ((float)t1->v)/256;
	f2.u = ((float)t2->u)/256;
	f2.v = ((float)t2->v)/256;
	f3.u = ((float)t3->u)/256;
	f3.v = ((float)t3->v)/256;

	num = AddFace( a, b, c );
	AddTexLink( num , 0, &f1, &f2, &f3 );

	return num;
}

//�ؽ��� ��ǥ�� ���¿� ���缭 �����Ѵ�
int smOBJ3D::ReformTexture( int Type , int MatNum , int MatCnt , POINT3D *pos , POINT3D *size )
{
	int cnt,cnt2;
	smTEXLINK *lpTexLink;
	int rx,ry,rz;
	float fx,fy,fz;
	int vn;

	for( cnt=0;cnt<nFace;cnt++) {
		if ( MatNum==Face[cnt].v[3] ) {
			lpTexLink = Face[cnt].lpTexLink;
			for(cnt2=0;cnt2<MatCnt;cnt2++) 
				lpTexLink = lpTexLink->NextTex;

			switch( Type ) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
				for(cnt2=0;cnt2<3;cnt2++) {
					vn = Face[cnt].v[cnt2];
					rx = smWorldVertex[ vn ].x;
					ry = smWorldVertex[ vn ].y;
					rz = smWorldVertex[ vn ].z;

					rx += pos->x;
					ry += pos->y;
					rz += pos->z;

					rx = ( (rx<<(FLOATNS)) / size->x );
					fx = ( (float)rx) /fONE;
					rz = ( (rz<<(FLOATNS)) / size->z );
					fz = ( (float)rz) /fONE;
					ry = ( (ry<<(FLOATNS)) / size->y );
					fy = ( (float)ry )/fONE;

					fx += 0.50;
					fz += 0.50;
					fy += 0.50;

					lpTexLink->u[cnt2] = -fx;
					lpTexLink->v[cnt2] = -fy;
				}
				break;
			}
		}
	}


	return TRUE;
}

// �븻�� ���� ( 0���� �ʱ�ȭ )
void smOBJ3D::ZeroNormals()
{
	int cnt;

    for (cnt = 0; cnt < nVertex; cnt++) {
		Vertex[cnt].nx = 0;
		Vertex[cnt].ny = 0;
		Vertex[cnt].nz = 0;
	}

	//######################################################################################
	//�� �� �� : �� �� ��
	Head &= (~OBJ_HEAD_TYPE_NEW_NORMAL);
	//######################################################################################
}

// ���̵� �븻�� ����
void smOBJ3D::SetGNormals()
{
	int cnt;
	float i, j, k, d;
	smVERTEX *p1,*p2,*p3;
	int *VNormalCnt;

	VNormalCnt = new int[nVertex];

    for (cnt = 0; cnt < nVertex; cnt++) {
		Vertex[cnt].nx = 0;
		Vertex[cnt].ny = 0;
		Vertex[cnt].nz = 0;
		VNormalCnt[cnt] = 0;
	}

	// Normal �� ��� �븻 ��ǥ ����Ͽ� Vertex�� ����
    for (cnt = 0; cnt < nFace; cnt++) {

		p1 = &Vertex[ Face[cnt].v[0] ];
		p2 = &Vertex[ Face[cnt].v[1] ];
		p3 = &Vertex[ Face[cnt].v[2] ];

 
		i = (float) (((p2->y - p1->y) * (p3->z - p1->z)) - ((p2->z - p1->z) * (p3->y - p1->y)));
		j = (float) (((p2->z - p1->z) * (p3->x - p1->x)) - ((p2->x - p1->x) * (p3->z - p1->z)));
		k = (float) (((p2->x - p1->x) * (p3->y - p1->y)) - ((p2->y - p1->y) * (p3->x - p1->x)));

		d = (float) sqrt((double) ((i * i) + (j * j) + (k * k)));
		i = (float) (i / d * 32767);
		j = (float) (j / d * 32767);
		k = (float) (k / d * 32767);
		
		//Normal->SetTo( (long) i, (long) j, (long) k );
		p1->nx += (int)i; p1->ny += (int)j; p1->nz += (int)k; 
		VNormalCnt[ Face[cnt].v[0] ]++;

		p2->nx += (int)i; p2->ny += (int)j; p2->nz += (int)k; 
		VNormalCnt[ Face[cnt].v[1] ]++;

		p3->nx += (int)i; p3->ny += (int)j; p3->nz += (int)k; 
		VNormalCnt[ Face[cnt].v[2] ]++;
	}

	// Vertex �븻�� ����� ���Ͽ� �Ϸ�
    for (cnt = 0; cnt < nVertex; cnt++) {
	    if ( VNormalCnt[cnt] ) {
			Vertex[cnt].nx /= VNormalCnt[cnt];
			Vertex[cnt].ny /= VNormalCnt[cnt];
			Vertex[cnt].nz /= VNormalCnt[cnt];

			//######################################################################################
			// �� �� �� : �� �� ��
			Vertex[cnt].nx += Vertex[cnt].x;
			Vertex[cnt].ny += Vertex[cnt].y;
			Vertex[cnt].nz += Vertex[cnt].z;
			//######################################################################################

/*
			d = (float) sqrt(	Vertex[cnt].nx * Vertex[cnt].nx + 
								Vertex[cnt].ny * Vertex[cnt].ny + 
								Vertex[cnt].nz * Vertex[cnt].nz);

			if (d==0) d=1;

			Vertex[cnt].nx = (32768 * Vertex[cnt].nx) / (int)d;
			Vertex[cnt].ny = (32768 * Vertex[cnt].ny) / (int)d;
			Vertex[cnt].nz = (32768 * Vertex[cnt].nz) / (int)d;
*/
		}
	}

	delete VNormalCnt;

}

// ��Ƽ �ؽ����� ��� ��Ʈ�ؾ� �ѹ��� �׸��Ⱑ ���ϴ�
void smOBJ3D::SortMatrial()
{
	int cnt1,cnt2;

	smFACE tempFace;

//	for( cnt=0;cnt<16;cnt++) {
		for( cnt1=0;cnt1<nFace;cnt1++) {
			for( cnt2=cnt1;cnt2<nFace;cnt2++) {
				if ( Face[cnt1].v[3] > Face[cnt2].v[3] ) {
					memcpy( &tempFace , &Face[cnt1] , sizeof( smFACE ) );
					memcpy( &Face[cnt1] , &Face[cnt2] , sizeof( smFACE ) );
					memcpy( &Face[cnt2] , &tempFace , sizeof( smFACE ) );
				}
			}
		}
//	}
}

// ��ġ�ų� ������ �ʴ� VERTEX�� ã�Ƴ��� �����Ѵ�
void smOBJ3D::SortVertex()
{
	int cnt1,cnt2,cnt3,cnt4;
	int x,y,z;


	// ��ġ�� VERTEX ����
	for( cnt1=1;cnt1<nVertex;cnt1++ ) {
		for( cnt2=0;cnt2<cnt1;cnt2++ ) {
			x = abs( Vertex[cnt1].x - Vertex[cnt2].x );
			y = abs( Vertex[cnt1].y - Vertex[cnt2].y );
			z = abs( Vertex[cnt1].z - Vertex[cnt2].z );

			if ( x==0 && y==0 && z==0 && cnt1<nVertex ) {// ���� ���
				for( cnt3=0;cnt3<nFace;cnt3++) {

					if ( Face[cnt3].v[0]==cnt1 ) Face[cnt3].v[0]=cnt2;
					if ( Face[cnt3].v[1]==cnt1 ) Face[cnt3].v[1]=cnt2;
					if ( Face[cnt3].v[2]==cnt1 ) Face[cnt3].v[2]=cnt2;

					if ( Face[cnt3].v[0]>cnt1 ) Face[cnt3].v[0]--;
					if ( Face[cnt3].v[1]>cnt1 ) Face[cnt3].v[1]--;
					if ( Face[cnt3].v[2]>cnt1 ) Face[cnt3].v[2]--;

				}
				nVertex--;
				if ( cnt1!=nVertex ) {
					for( cnt4=cnt1;cnt4<nVertex;cnt4++ ) 
						memcpy( &Vertex[cnt4] , &Vertex[cnt4+1] , sizeof( smVERTEX ) );

					if ( Physique ) {					
						for( cnt4=cnt1;cnt4<nVertex;cnt4++ ) 
							Physique[cnt4] = Physique[cnt4+1];
					}
				}
			}
		}
	}


	// ��� ���� �ʴ� VERTEX ����
	short	*pUseVertex = new short[nVertex];

	ZeroMemory( pUseVertex , sizeof( short )*nVertex );
	for(cnt1=0;cnt1<nFace;cnt1++) {
		pUseVertex[ Face[cnt1].v[0] ]++;
		pUseVertex[ Face[cnt1].v[1] ]++;
		pUseVertex[ Face[cnt1].v[2] ]++;

		if ( Face[cnt1].v[0]>=nVertex ||
			Face[cnt1].v[1]>=nVertex ||
			Face[cnt1].v[2]>=nVertex ) {

			cnt1 = cnt1;

		}

	}

	for ( cnt1=0; cnt1<nVertex; cnt1++) {
		if ( pUseVertex[ cnt1 ]==0 ) {
			for( cnt3=0;cnt3<nFace;cnt3++) {
				if ( Face[cnt3].v[0]>cnt1 ) Face[cnt3].v[0]--;
				if ( Face[cnt3].v[1]>cnt1 ) Face[cnt3].v[1]--;
				if ( Face[cnt3].v[2]>cnt1 ) Face[cnt3].v[2]--;
			}
			nVertex--;
			if ( cnt1!=nVertex ) {
				for( cnt4=cnt1;cnt4<nVertex;cnt4++ ) 
					memcpy( &Vertex[cnt4] , &Vertex[cnt4+1] , sizeof( smVERTEX ) );

				if ( Physique ) {					
					for( cnt4=cnt1;cnt4<nVertex;cnt4++ ) 
						Physique[cnt4] = Physique[cnt4+1];
				}
			}
		}
	}

	delete pUseVertex;

}

// TM�� ������� ����Ͽ� ���� ��ǥ ��ȯ
void smOBJ3D::ReformTM()
{
	int cnt;
	int x,y,z;
	smMATRIX q;
	int scale;

	//������ ���� ����
	if ( !pParent ) {
		scale = (sx+sy+sz)/3;
		if ( scale!=0 ) {
			Tm._11 = (Tm._11<<FLOATNS) / scale;
			Tm._12 = (Tm._12<<FLOATNS) / scale;
			Tm._13 = (Tm._13<<FLOATNS) / scale;
			Tm._21 = (Tm._21<<FLOATNS) / scale;
			Tm._22 = (Tm._22<<FLOATNS) / scale;
			Tm._23 = (Tm._23<<FLOATNS) / scale;
			Tm._31 = (Tm._31<<FLOATNS) / scale;
			Tm._32 = (Tm._32<<FLOATNS) / scale;
			Tm._33 = (Tm._33<<FLOATNS) / scale;

			//######################################################################################
			//�� �� �� : �� �� ��
			//px =0;
			//py =0;
			//pz =0;
			//######################################################################################
		}
	}
	else {
		scale = ( pParent->sx+pParent->sy+pParent->sz )/3;

		if ( scale!=0 ) {
			Tm._11 = (Tm._11<<FLOATNS) / scale;
			Tm._12 = (Tm._12<<FLOATNS) / scale;
			Tm._13 = (Tm._13<<FLOATNS) / scale;
			Tm._21 = (Tm._21<<FLOATNS) / scale;
			Tm._22 = (Tm._22<<FLOATNS) / scale;
			Tm._23 = (Tm._23<<FLOATNS) / scale;
			Tm._31 = (Tm._31<<FLOATNS) / scale;
			Tm._32 = (Tm._32<<FLOATNS) / scale;
			Tm._33 = (Tm._33<<FLOATNS) / scale;
		}
	}

	smMatrixInvert( q, Tm );
	memcpy( &TmInvert , &q, sizeof( smMATRIX ) );


	for( cnt=0;cnt<nVertex;cnt++ ) {

		x = Vertex[cnt].x * q._11 +
			Vertex[cnt].y * q._21 +
			Vertex[cnt].z * q._31 ;

		y = Vertex[cnt].x * q._12 +
			Vertex[cnt].y * q._22 +
			Vertex[cnt].z * q._32 ;

		z = Vertex[cnt].x * q._13 +
			Vertex[cnt].y * q._23 +
			Vertex[cnt].z * q._33 ;

		Vertex[cnt].x = (x>>FLOATNS)+q._41;
		Vertex[cnt].y = (y>>FLOATNS)+q._42;
		Vertex[cnt].z = (z>>FLOATNS)+q._43;

		//######################################################################################
		// �� �� �� : �� �� ��
		x = Vertex[cnt].nx * q._11 +
			Vertex[cnt].ny * q._21 +
			Vertex[cnt].nz * q._31 ;

		y = Vertex[cnt].nx * q._12 +
			Vertex[cnt].ny * q._22 +
			Vertex[cnt].nz * q._32 ;

		z = Vertex[cnt].nx * q._13 +
			Vertex[cnt].ny * q._23 +
			Vertex[cnt].nz * q._33 ;

		Vertex[cnt].nx = (x>>FLOATNS)+q._41;
		Vertex[cnt].ny = (y>>FLOATNS)+q._42;
		Vertex[cnt].nz = (z>>FLOATNS)+q._43;
		//######################################################################################
	}
}

// ����ũ�� ���� TM�� ������� ����Ͽ� ���� ��ǥ ��ȯ
void smOBJ3D::ReformPhysique()
{
	int cnt;
	int x,y,z;
	smMATRIX q;
	smOBJ3D *pBone;

	for( cnt=0;cnt<nVertex;cnt++ ) {

		pBone = Physique[cnt];

		smMatrixInvert( q, pBone->Tm );

		x = Vertex[cnt].x * q._11 +
			Vertex[cnt].y * q._21 +
			Vertex[cnt].z * q._31 ;

		y = Vertex[cnt].x * q._12 +
			Vertex[cnt].y * q._22 +
			Vertex[cnt].z * q._32 ;

		z = Vertex[cnt].x * q._13 +
			Vertex[cnt].y * q._23 +
			Vertex[cnt].z * q._33 ;


		Vertex[cnt].x = (x>>FLOATNS)+q._41;
		Vertex[cnt].y = (y>>FLOATNS)+q._42;
		Vertex[cnt].z = (z>>FLOATNS)+q._43;

		//######################################################################################
		// �� �� �� : �� �� ��
		x = Vertex[cnt].nx * q._11 +
			Vertex[cnt].ny * q._21 +
			Vertex[cnt].nz * q._31 ;

		y = Vertex[cnt].nx * q._12 +
			Vertex[cnt].ny * q._22 +
			Vertex[cnt].nz * q._32 ;

		z = Vertex[cnt].nx * q._13 +
			Vertex[cnt].ny * q._23 +
			Vertex[cnt].nz * q._33 ;


		Vertex[cnt].nx = (x>>FLOATNS)+q._41;
		Vertex[cnt].ny = (y>>FLOATNS)+q._42;
		Vertex[cnt].nz = (z>>FLOATNS)+q._43;
		//######################################################################################
	}
}


//=========================== [ ������  ���� ] =============================//

// �̰� �ٺ���.
int smOBJ3D::TestRotate( int x , int y , int z ,  POINT3D *LocalAngle , POINT3D *GlobalAngle )
{
	int bounds;
	int pz;

	int gx,gy,gz;

	gx = (-GlobalAngle->x) & ANGCLIP;
	gy = (-GlobalAngle->y) & ANGCLIP;
	gz = (-GlobalAngle->z) & ANGCLIP;


	smTestVertex.SetTo( &ZeroVertex );
	smTestVertex.Move( x , y, z );

	Trig[0] = GetSin[gx]>>FLOATDS;
	Trig[1] = GetCos[gx]>>FLOATDS;
	Trig[2] = GetSin[gy]>>FLOATDS;
	Trig[3] = GetCos[gy]>>FLOATDS;
	Trig[4] = GetSin[gz]>>FLOATDS;
	Trig[5] = GetCos[gz]>>FLOATDS;

	smTestVertex.GlobalRotate( Trig );

	pz = smTestVertex.Z >> FLOATNS;

//	if ( (pz+Bound)>0 && (pz-Bound<ViewPointLen) ) {
	if ( (pz+Bound)>0 && (pz<ViewPointLen) ) {
		if ( pz==0 ) pz = 1;
		pz=abs(pz);
		bounds = (Bound * viewdistZ) / pz;

		if ( (smTestVertex.x2d+bounds)>0 &&
			(smTestVertex.x2d - bounds)<renderDevice.GetScreenWidth() &&
			 (smTestVertex.y2d+bounds)>0  &&
			 (smTestVertex.y2d - bounds)<renderDevice.GetScreenHeight())

			 return TRUE;
	}

	return FALSE;
}

// ������Ʈ ������ �غ�
int smOBJ3D::BeginRender()
{
	int cnt;

	if ( nVertex>RENDER_VERTEX_MAX ) return FALSE;

	//######################################################################################
	//�� �� �� : �� �� ��
	for( cnt=0; cnt < nVertex; cnt++ )
		smRender.AddObjectVertex( &smWorldVertex[cnt], Head );
	//######################################################################################

	for (cnt = 0; cnt<nFace; cnt++)
		smRender.AddObjectFace( &Face[cnt] );

	return TRUE;

}

//�� �� �� : �� �� ��
//ȸ�� ���ʹϾ��� ���� , �߰� �������� �࿭�� ���� 
int smOBJ3D::GetRotFrame( smFMATRIX &gmat, smFMATRIX *PrevRot, smTM_ROT *tmRot, int frame, SMotionStEndInfo *lpFrameInfo )
//######################################################################################
{

	float qx,qy,qz,qw;
	float ax,ay,az,aw;
	float bx,by,bz,bw;
	int cnt;
	int s,e,ch,sh;
	float alpha;

	cnt = 0;

	if ( tmRot[cnt].frame>frame ) return frame;

	while(1) {
		s = tmRot[cnt].frame;
		e = tmRot[cnt+1].frame;
		if ( s <= frame && e> frame ) break;
		cnt++;
	}

	ch = e-s;
	sh = frame - s;

	alpha = (float)sh / (float)ch;

	ax = tmRot[cnt].x;
	ay = tmRot[cnt].y;
	az = tmRot[cnt].z;
	aw = tmRot[cnt].w;
	
	bx = tmRot[cnt+1].x;
	by = tmRot[cnt+1].y;
	bz = tmRot[cnt+1].z;
	bw = tmRot[cnt+1].w;

	//######################################################################################
	//�� �� �� : �� �� ��
	if( lpFrameInfo && e > (int)lpFrameInfo->EndFrame )
	{
		::CopyMemory( &gmat, &PrevRot[cnt], sizeof(smFMATRIX) );
		return s;
	}
	//######################################################################################

	//smTM_ROT tRot;

	D3DMath_QuaternionSlerp( qx,qy,qz,qw, 0,0,0,0, bx,by,bz,bw, alpha );

	//######################################################################################
	//�� �� �� : �� �� ��
	smFMatrixFromQuaternion( gmat , qx,qy,qz,qw );
	smFMatrixMult( gmat , PrevRot[cnt], gmat );
	//######################################################################################

	return s;
}

//�� �� �� : �� �� ��
//�̵� ��ǥ�� ���� , �߰� �������� �̵� ��ǥ�� ���� 
int smOBJ3D::GetPosFrame( float &x, float &y, float &z, smTM_POS *tmPos, int frame )
{
	int s,e,ch,sh;
	int	cnt = 0;

	//######################################################################################
	//�� �� �� : �� �� ��
	float alpha;
	//######################################################################################

	if ( tmPos[cnt].frame>frame ) return frame;

	while(1) {
		s = tmPos[cnt].frame;
		e = tmPos[cnt+1].frame;
		if ( s <= frame && e> frame ) break;
		cnt++;
	}

	ch = e-s;
	sh = frame - s;

	//######################################################################################
	//�� �� �� : �� �� ��
	alpha = float(sh) / float(ch);	

	x = tmPos[cnt].x + ((tmPos[cnt+1].x - tmPos[cnt].x) * alpha);
	y = tmPos[cnt].y + ((tmPos[cnt+1].y - tmPos[cnt].y) * alpha);
	z = tmPos[cnt].z + ((tmPos[cnt+1].z - tmPos[cnt].z) * alpha);
	//######################################################################################

	return s;
}

//������ ��ǥ�� ���� , �߰� �������� ������ ��ǥ�� ���� 
int smOBJ3D::GetScaleFrame( int &x, int &y, int &z , smTM_SCALE *tmSclae , int frame )
{
	int s,e,ch,sh;
	int	cnt = 0;
	int alpha;

	if ( tmSclae[cnt].frame>frame ) return frame;

	while(1) {
		s = tmSclae[cnt].frame;
		e = tmSclae[cnt+1].frame;
		if ( s <= frame && e> frame ) break;
		cnt++;
	}

	ch = e-s;
	sh = frame - s;
	alpha = (sh<<8) / ch;

	x = tmSclae[cnt].x+ ( ((tmSclae[cnt+1].x - tmSclae[cnt].x) * alpha )>>8 );
	y = tmSclae[cnt].y+ ( ((tmSclae[cnt+1].y - tmSclae[cnt].y) * alpha )>>8 );
	z = tmSclae[cnt].z+ ( ((tmSclae[cnt+1].z - tmSclae[cnt].z) * alpha )>>8 );
	return s;
}

//�ִϸ��̼� ������ ��ġ�� ã�´� ( ���� ���� �ִϸ��̼� ��� )
int smOBJ3D::GetTmFramePos( int frame )
{
	int cnt;

	if ( TmFrameCnt>0 ) {
		for(cnt=0;cnt<TmFrameCnt;cnt++) {
			if ( TmPosFrame[cnt].PosCnt>0 && TmPosFrame[cnt].StartFrame<=frame &&
				TmPosFrame[cnt].EndFrame>frame ) {

				return TmPosFrame[cnt].PosNum;

			}
		}

	}

	return -1;
}

//�ִϸ��̼� ������ ũ�⸦ ã�´� ( ���� ���� �ִϸ��̼� ��� )
int smOBJ3D::GetTmFrameScale( int frame )
{
	int cnt;

	if ( TmFrameCnt>0 ) {
		for(cnt=0;cnt<TmFrameCnt;cnt++) {
			if ( TmScaleFrame[cnt].PosCnt>0 && TmScaleFrame[cnt].StartFrame<=frame &&
				TmScaleFrame[cnt].EndFrame>frame ) {

				return TmScaleFrame[cnt].PosNum;

			}
		}

	}

	return -1;
}

//�ִϸ��̼� ������ ��ġ�� ã�´� ( ���� ���� �ִϸ��̼� ��� )
int smOBJ3D::GetTmFrameRot( int frame )
{
	int cnt;

	if ( TmFrameCnt>0 ) {
		for(cnt=0;cnt<TmFrameCnt;cnt++) {
			if ( TmRotFrame[cnt].PosCnt>0 && TmRotFrame[cnt].StartFrame<=frame &&
				TmRotFrame[cnt].EndFrame>frame ) {

				return TmRotFrame[cnt].PosNum;

			}
		}

	}

	return -1;
}

//�� �� �� : �� �� ��
int smOBJ3D::TmTalkAnimation( int frame )
{
	//######################################################################################
	//�� �� �� : �� �� ��
	smFMATRIX qmat;
	//######################################################################################

	int cnt;
	int NumTmRot , NumTmPos , NumTmScale;

	smFMATRIX	fMat;
	smMATRIX	Mat;

	//######################################################################################
	//�� �� �� : �� �� ��
	// Animation ��� ���ϱ�
	smFIdentityMatrix( qmat );
	//######################################################################################

	//frame = 0;

	NumTmRot = GetTmFrameRot( frame );
	NumTmPos = GetTmFramePos( frame );
	NumTmScale = GetTmFrameScale( frame );


	if( TmFrameCnt && (NumTmRot >= 0 || NumTmPos >= 0 || NumTmScale > 0) )
	{
		if( TmRotCnt > 0 && NumTmRot >= 0 )
		{
			cnt = GetRotFrame( qmat, &TmPrevRot[ NumTmRot ], &TmRot[NumTmRot], frame );
		}
		else
		{
			//######################################################################################
			//�� �� �� : �� �� ��
			smFMatrixFromMatrix( qmat, TmRotate );
			//######################################################################################
		}

		// ������ ��� ����
		if( TmScaleCnt > 0  && NumTmScale >= 0 )
		{
			smIdentityMatrix( Mat );
			GetScaleFrame( Mat._11, Mat._22, Mat._33, &TmScale[ NumTmScale ], frame );
			//######################################################################################
			//�� �� �� : �� �� ��
			smFMatrixFromMatrix( fMat, Mat );
			smFMatrixMult( qmat, qmat, fMat );
			//######################################################################################
		}

		if( TmPosCnt > 0 && NumTmPos >= 0 )
		{
			GetPosFrame( qmat._41, qmat._42, qmat._43, &TmPos[ NumTmPos ], frame );
		}
		else
		{
			//######################################################################################
			//�� �� �� : �� �� ��
			//if( smObjFixPos )
			//{
				//######################################################################################
				//�� �� �� : �� �� ��
				//qmat._41 = float(Tm._41) / fONE;
				//qmat._42 = float(Tm._42) / fONE;
				//qmat._43 = float(Tm._43) / fONE;
				//######################################################################################
			//}
			//else
			//######################################################################################
			{
				//######################################################################################
				//�� �� �� : �� �� ��
				qmat._41 = float(px) / fONE;
				qmat._42 = float(py) / fONE;
				qmat._43 = float(pz) / fONE;
				//######################################################################################
			}
		}

		memcpy( &TmResult, &qmat, sizeof(qmat) );
	}
	else
		smFIdentityMatrix( TmResult );

	return TRUE;
}

//�� �� �� : �� �� ��
int smOBJ3D::TmAnimation( int frame , int ax, int ay, int az, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo )
{
	//######################################################################################
	//�� �� �� : �� �� ��
	smFMATRIX qmat;
	//######################################################################################

	smMATRIX	rX,rY,rZ;
	int cnt;
	int NumTmRot , NumTmPos , NumTmScale;

	smFMATRIX	fMat;
	smMATRIX	Mat;

	//######################################################################################
	//�� �� �� : �� �� ��
	// Animation ��� ���ϱ�
	smFIdentityMatrix( qmat );
	//######################################################################################

	//frame = 0;

	NumTmRot = GetTmFrameRot( frame );
	NumTmPos = GetTmFramePos( frame );
	NumTmScale = GetTmFrameScale( frame );

	//######################################################################################
	//�� �� �� : �� �� ��	
	smFMATRIX *lpTalkResult = NULL;	
	for( int cnt2=0; cnt2 < TalkPattern->nObj3d; cnt2++ )
	{
		if( stricmp(TalkPattern->obj3d[cnt2]->NodeName, NodeName) == 0 )
		{
			lpTalkResult = &TalkPattern->obj3d[cnt2]->TmResult;
			break;
		}
	}
	//######################################################################################

	if( lpTalkResult == NULL )
	{
		if( (!TmFrameCnt && (TmRotCnt >  0 || TmPosCnt >  0 || TmScaleCnt > 0) ) || 
			( TmFrameCnt && (NumTmRot >= 0 || NumTmPos >= 0 || NumTmScale > 0) ) )
		{
			if( TmFrameCnt )
			{
				if( TmRotCnt > 0 && NumTmRot >= 0 )
				{
					cnt = GetRotFrame( qmat, &TmPrevRot[ NumTmRot ], &TmRot[NumTmRot], frame, lpFrameInfo );
				}
				else
				{
					//######################################################################################
					//�� �� �� : �� �� ��
					smFMatrixFromMatrix( qmat, TmRotate );
					//######################################################################################
				}

				// ������ ��� ����
				if( TmScaleCnt > 0  && NumTmScale >= 0 )
				{
					smIdentityMatrix( Mat );
					GetScaleFrame( Mat._11, Mat._22, Mat._33, &TmScale[ NumTmScale ], frame );
					//######################################################################################
					//�� �� �� : �� �� ��
					smFMatrixFromMatrix( fMat, Mat );
					smFMatrixMult( qmat, qmat, fMat );
					//######################################################################################
				}

				if( TmPosCnt > 0 && NumTmPos >= 0 )
				{
					GetPosFrame( qmat._41, qmat._42, qmat._43, &TmPos[ NumTmPos ], frame );
				}
				else
				{
					//######################################################################################
					//�� �� �� : �� �� ��
					//if( smObjFixPos )
					//{
						//######################################################################################
						//�� �� �� : �� �� ��
						//qmat._41 = float(Tm._41) / fONE;
						//qmat._42 = float(Tm._42) / fONE;
						//qmat._43 = float(Tm._43) / fONE;
						//######################################################################################
					//}					
					//else
					//######################################################################################
					{
						//######################################################################################
						//�� �� �� : �� �� ��
						qmat._41 = float(px) / fONE;
						qmat._42 = float(py) / fONE;
						qmat._43 = float(pz) / fONE;
						//######################################################################################
					}
				}
			}
			else
			{
				// �ִϸ��̼� ��Ŀ� ȸ�� ��� ���� 
				if( TmRotCnt > 0 && TmRot[TmRotCnt-1].frame > frame )
				{
					cnt = GetRotFrame( qmat, TmPrevRot,  TmRot, frame, lpFrameInfo );
				}
				else
				{
					//######################################################################################
					//�� �� �� : �� �� ��
					smFMatrixFromMatrix( qmat, TmRotate );
					//######################################################################################
				}

				// ������ ��� ����
				if( TmScaleCnt > 0  && TmScale[TmScaleCnt-1].frame > frame )
				{
					smIdentityMatrix( Mat );
					GetScaleFrame( Mat._11, Mat._22, Mat._33 , TmScale , frame );
					//######################################################################################
					//�� �� �� : �� �� ��
					smFMatrixFromMatrix( fMat, Mat );
					smFMatrixMult( qmat, qmat, fMat );
					//######################################################################################
				}

				// �̵� ��� ����
				if( TmPosCnt > 0 && TmPos[TmPosCnt-1].frame > frame )
				{
					GetPosFrame( qmat._41, qmat._42, qmat._43, TmPos, frame );
				}
				else
				{
					//######################################################################################
					//�� �� �� : �� �� ��
					//if( smObjFixPos )
					//{
						//######################################################################################
						//�� �� �� : �� �� ��
						//qmat._41 = float(Tm._41) / fONE;
						//qmat._42 = float(Tm._42) / fONE;
						//qmat._43 = float(Tm._43) / fONE;
						//######################################################################################
					//}
					//else
					//######################################################################################
					{
						//######################################################################################
						//�� �� �� : �� �� ��
						qmat._41 = float(px) / fONE;
						qmat._42 = float(py) / fONE;
						qmat._43 = float(pz) / fONE;
						//######################################################################################
					}
				}
			}
		}
		else
		{
			if( pParent )
			{
				//######################################################################################
				//�� �� �� : �� �� ��
				smMatrixMult( Mat, Tm, pParent->TmInvert );
				smFMatrixFromMatrix( qmat, Mat );
				//######################################################################################
			}
			else 
			{
				//######################################################################################
				//�� �� �� : �� �� ��
				smFMatrixFromMatrix( qmat, Tm );
				//######################################################################################
			}
		}
	}
	else
		memcpy( &qmat, lpTalkResult, sizeof(qmat) );

	// �θ��� �ִϸ��̼� ����� ���Ѵ�
	if( pParent )
	{
		//######################################################################################
		//�� �� �� : �� �� ��
		smFMatrixMult( TmResult, qmat, pParent->TmResult );
		//######################################################################################
	}
	else
	{
		//######################################################################################
		//�� �� �� : �� �� ��
		memcpy( &TmResult, &qmat, sizeof(qmat) );
		//######################################################################################
	}

	if( ax==0 && ay==0 && az==0 )
	{
		smIdentityMatrix( mLocal );
		smMatrixFromFMatrix( mWorld , TmResult );
	}
	else
	{
		smIdentityMatrix( rX );
		smIdentityMatrix( rY );
		smIdentityMatrix( rZ );

		smRotateXMatrix( rX , ax );
		smRotateZMatrix( rY , ay );
		smRotateYMatrix( rZ , az );

		//######################################################################################
		//�� �� �� : �� �� ��
		if( smRender.m_dwRenderType )
		{
			smMatrixMult( mLocal, rY, rX );
			smMatrixMult( mLocal, mLocal, rZ );
		}
		else
		{
			smMatrixMult( mLocal , rZ , rX );
			smMatrixMult( mLocal , mLocal , rY );
		}
		//######################################################################################

		smMatrixFromFMatrix( Mat , TmResult );
		smMatrixMult( mWorld , Mat, mLocal );
	}

	return TRUE;
}

// �ִϸ��̼� �� ������ǥ ȸ�� ��� �����
int smOBJ3D::TmAnimation( int frame , int ax, int ay, int az, SMotionStEndInfo *lpFrameInfo )
{
	//######################################################################################
	//�� �� �� : �� �� ��
	smFMATRIX qmat;
	//######################################################################################

	smMATRIX	rX,rY,rZ;
	int cnt;
	int NumTmRot , NumTmPos , NumTmScale;

	smFMATRIX	fMat;
	smMATRIX	Mat;

	//######################################################################################
	//�� �� �� : �� �� ��
	// Animation ��� ���ϱ�
	smFIdentityMatrix( qmat );
	//######################################################################################

	//frame = 0;

	NumTmRot = GetTmFrameRot( frame );
	NumTmPos = GetTmFramePos( frame );
	NumTmScale = GetTmFrameScale( frame );


	if( (!TmFrameCnt && (TmRotCnt >  0 || TmPosCnt >  0 || TmScaleCnt > 0) ) || 
		( TmFrameCnt && (NumTmRot >= 0 || NumTmPos >= 0 || NumTmScale > 0) ) )
	{
		if( TmFrameCnt )
		{
			if( TmRotCnt > 0 && NumTmRot >= 0 )
			{
				cnt = GetRotFrame( qmat, &TmPrevRot[ NumTmRot ], &TmRot[NumTmRot], frame, lpFrameInfo );
			}
			else
			{
				//######################################################################################
				//�� �� �� : �� �� ��
				smFMatrixFromMatrix( qmat, TmRotate );
				//######################################################################################
			}

			// ������ ��� ����
			if( TmScaleCnt > 0  && NumTmScale >= 0 )
			{
				smIdentityMatrix( Mat );
				GetScaleFrame( Mat._11, Mat._22, Mat._33, &TmScale[ NumTmScale ], frame );
				//######################################################################################
				//�� �� �� : �� �� ��
				smFMatrixFromMatrix( fMat, Mat );
				smFMatrixMult( qmat, qmat, fMat );
				//######################################################################################
			}

			if( TmPosCnt > 0 && NumTmPos >= 0 )
			{
				GetPosFrame( qmat._41, qmat._42, qmat._43, &TmPos[ NumTmPos ], frame );
			}
			else
			{
				//######################################################################################
				//�� �� �� : �� �� ��
				//if( smObjFixPos )
				//{
					//######################################################################################
					//�� �� �� : �� �� ��
					//qmat._41 = float(Tm._41) / fONE;
					//qmat._42 = float(Tm._42) / fONE;
					//qmat._43 = float(Tm._43) / fONE;
					//######################################################################################
				//}
				//else
				//######################################################################################
				{
					//######################################################################################
					//�� �� �� : �� �� ��
					qmat._41 = float(px) / fONE;
					qmat._42 = float(py) / fONE;
					qmat._43 = float(pz) / fONE;
					//######################################################################################
				}
			}
		}
		else
		{
			// �ִϸ��̼� ��Ŀ� ȸ�� ��� ���� 
			if( TmRotCnt > 0 && TmRot[TmRotCnt-1].frame > frame )
			{
				cnt = GetRotFrame( qmat, TmPrevRot,  TmRot, frame, lpFrameInfo );
			}
			else
			{
				//######################################################################################
				//�� �� �� : �� �� ��
				smFMatrixFromMatrix( qmat, TmRotate );
				//######################################################################################
			}

			// ������ ��� ����
			if( TmScaleCnt > 0  && TmScale[TmScaleCnt-1].frame > frame )
			{
				smIdentityMatrix( Mat );
				GetScaleFrame( Mat._11, Mat._22, Mat._33 , TmScale , frame );
				//######################################################################################
				//�� �� �� : �� �� ��
				smFMatrixFromMatrix( fMat, Mat );
				smFMatrixMult( qmat, qmat, fMat );
				//######################################################################################
			}

			// �̵� ��� ����
			if( TmPosCnt > 0 && TmPos[TmPosCnt-1].frame > frame )
			{
				GetPosFrame( qmat._41, qmat._42, qmat._43, TmPos, frame );
			}
			else
			{
				//######################################################################################
				//�� �� �� : �� �� ��
				//if( smObjFixPos )
				//{
					//######################################################################################
					//�� �� �� : �� �� ��
					//qmat._41 = float(Tm._41) / fONE;
					//qmat._42 = float(Tm._42) / fONE;
					//qmat._43 = float(Tm._43) / fONE;
					//######################################################################################
				//}
				//else
				//######################################################################################
				{
					//######################################################################################
					//�� �� �� : �� �� ��
					qmat._41 = float(px) / fONE;
					qmat._42 = float(py) / fONE;
					qmat._43 = float(pz) / fONE;
					//######################################################################################
				}
			}
		}
	}
	else
	{
		if( pParent )
		{
			//######################################################################################
			//�� �� �� : �� �� ��
			smMatrixMult( Mat, Tm, pParent->TmInvert );
			smFMatrixFromMatrix( qmat, Mat );
			//######################################################################################
		}
		else 
		{
			//######################################################################################
			//�� �� �� : �� �� ��
			smFMatrixFromMatrix( qmat, Tm );
			//######################################################################################
		}
	}

	// �θ��� �ִϸ��̼� ����� ���Ѵ�
	if( pParent )
	{
		//######################################################################################
		//�� �� �� : �� �� ��
		smFMatrixMult( TmResult, qmat, pParent->TmResult );
		//######################################################################################
	}
	else
	{
		//######################################################################################
		//�� �� �� : �� �� ��
		memcpy( &TmResult, &qmat, sizeof(qmat) );
		//######################################################################################
	}

	if( ax==0 && ay==0 && az==0 )
	{
		smIdentityMatrix( mLocal );
		smMatrixFromFMatrix( mWorld , TmResult );
	}
	else
	{
		smIdentityMatrix( rX );
		smIdentityMatrix( rY );
		smIdentityMatrix( rZ );

		smRotateXMatrix( rX , ax );
		smRotateZMatrix( rY , ay );
		smRotateYMatrix( rZ , az );

		//######################################################################################
		//�� �� �� : �� �� ��
		if( smRender.m_dwRenderType )
		{
			smMatrixMult( mLocal, rY, rX );
			smMatrixMult( mLocal, mLocal, rZ );
		}
		else
		{
			smMatrixMult( mLocal , rZ , rX );
			smMatrixMult( mLocal , mLocal , rY );
		}
		//######################################################################################

		smMatrixFromFMatrix( Mat , TmResult );
		smMatrixMult( mWorld , Mat, mLocal );
	}

	return TRUE;
}

// ������Ŀ����� ������ǥ ���ϱ�  
int smOBJ3D::WorldForm()
{
	smVERTEX	*pV;
	smMATRIX	*mMat;

	int cnt;
	int rx,ry,rz;

	if ( Physique ) {
		for( cnt=0; cnt<nVertex ; cnt ++ ) {
			// Vertex ��ǥ ȸ��

			pV = &Vertex[cnt];
			mMat = &Physique[cnt]->mWorld;

			rx = pV->x * mMat->_11 +
				 pV->y * mMat->_21 +
				 pV->z * mMat->_31 ;

			ry = pV->x * mMat->_12 +
				 pV->y * mMat->_22 +
				 pV->z * mMat->_32 ;

			rz = pV->x * mMat->_13 +
				 pV->y * mMat->_23 +
				 pV->z * mMat->_33 ;

			smWorldVertex[cnt].x = (rx>>FLOATNS) + mMat->_41;
			smWorldVertex[cnt].z = (ry>>FLOATNS) + mMat->_42;
			smWorldVertex[cnt].y = (rz>>FLOATNS) + mMat->_43;

			//######################################################################################
			//�� �� �� : �� �� ��
			if( Head & OBJ_HEAD_TYPE_NEW_NORMAL )
				mMat = &Physique[cnt]->mWorld;
			else
				mMat = &Physique[cnt]->mLocal;
			//######################################################################################

			rx = pV->nx * mMat->_11 +
				 pV->ny * mMat->_21 +
				 pV->nz * mMat->_31 ;

			ry = pV->nx * mMat->_12 +
				 pV->ny * mMat->_22 +
				 pV->nz * mMat->_32 ;

			rz = pV->nx * mMat->_13 +
				 pV->ny * mMat->_23 +
				 pV->nz * mMat->_33 ;

			smWorldVertex[cnt].nx = (rx>>FLOATNS) + mMat->_41;
			smWorldVertex[cnt].nz = (ry>>FLOATNS) + mMat->_42;
			smWorldVertex[cnt].ny = (rz>>FLOATNS) + mMat->_43;


		}
	}
	else {
		for( cnt=0; cnt<nVertex ; cnt ++ ) {
			// Vertex ��ǥ ȸ��

			pV = &Vertex[cnt];

			rx = pV->x * mWorld._11 +
				 pV->y * mWorld._21 +
				 pV->z * mWorld._31 ;

			ry = pV->x * mWorld._12 +
				 pV->y * mWorld._22 +
				 pV->z * mWorld._32 ;

			rz = pV->x * mWorld._13 +
				 pV->y * mWorld._23 +
				 pV->z * mWorld._33 ;

			smWorldVertex[cnt].x = (rx>>FLOATNS) + mWorld._41;
			smWorldVertex[cnt].z = (ry>>FLOATNS) + mWorld._42;
			smWorldVertex[cnt].y = (rz>>FLOATNS) + mWorld._43;

			//######################################################################################
			//�� �� �� : �� �� ��
			if( Head & OBJ_HEAD_TYPE_NEW_NORMAL )
				mMat = &mWorld;
			else
				mMat = &mLocal;

			rx = pV->nx * mMat->_11 +
				 pV->ny * mMat->_21 +
				 pV->nz * mMat->_31 ;

			ry = pV->nx * mMat->_12 +
				 pV->ny * mMat->_22 +
				 pV->nz * mMat->_32 ;

			rz = pV->nx * mMat->_13 +
				 pV->ny * mMat->_23 +
				 pV->nz * mMat->_33 ;

			smWorldVertex[cnt].nx = (rx>>FLOATNS) + mMat->_41;
			smWorldVertex[cnt].nz = (ry>>FLOATNS) + mMat->_42;
			smWorldVertex[cnt].ny = (rz>>FLOATNS) + mMat->_43;
			//######################################################################################
		}
	}

	return TRUE;
}

// ������Ʈ�� ��ǥ �� ���� ����
void smOBJ3D::SetPosi( POINT3D *point , POINT3D *angle )
{

	Posi.x = point->x;
	Posi.y = point->y;
	Posi.z = point->z;

	Angle.x = angle->x;
	Angle.y = angle->y;
	Angle.z = angle->z;

}

// ������ ������ �׽�Ʈ (ī�޶� ��ǥ�� ī�޶� ���� �Է� )
int smOBJ3D::TestRender(POINT3D *camera , POINT3D *angle )
{
	int x,y,z;

	x = (Posi.x - camera->x)>>FLOATNS ;
	y = (Posi.y - camera->y)>>FLOATNS ;
	z = (Posi.z - camera->z)>>FLOATNS ;

	if ( TestRotate( x,y,z, &Angle , angle )==TRUE ) {

		if ( smObjFixPos ) {
			//������ ��ǥ�� ī�޶� ��ü
			CameraPosi.x = camera->x;
			CameraPosi.y = camera->y;
			CameraPosi.z = camera->z;
		}
		else {
			CameraPosi.x = camera->x - Posi.x;
			CameraPosi.y = camera->y - Posi.y;
			CameraPosi.z = camera->z - Posi.z;
		}

		smRender.SetCameraPosi( CameraPosi.x,CameraPosi.y,CameraPosi.z, angle->x,angle->y,angle->z );

		WorldForm();

		return TRUE;
	}


	return FALSE;
}

//�׽�Ʈ �Ϸ������ �������� �簳 �Ѵ�
int smOBJ3D::ActionRenderD3D()
{
	//�������� �ӽ� ���� �ʱ�ȭ ( ���ؽ� ������ �˷��� )
	smRender.Init();
	BeginRender();
	smRender.SetClipStates( ClipStates );				// Ŭ���� ����
	smRender.ClipRendFace();							// ��ü ������ ���̽��� Ŭ����
	smRender.GeomVertex2D( ColorEffect );				// ���ؽ��� 2D��ǥ�� ��ȯ
	smRender.RenderD3D();

	return TRUE;
}

// D3D������� ������Ƽ�꿡 ������ ( ī�޶� ��ǥ�� ī�޶� ���� �Է� )
int smOBJ3D::RenderD3D(POINT3D *camera , POINT3D *angle )
{
	int x,y,z;

	x = (Posi.x - camera->x)>>FLOATNS ;
	y = (Posi.y - camera->y)>>FLOATNS ;
	z = (Posi.z - camera->z)>>FLOATNS ;

	if ( TestRotate( x,y,z, &Angle , angle )==TRUE ) {

		if ( smObjFixPos ) {
			//������ ��ǥ�� ī�޶� ��ü
			CameraPosi.x = camera->x;
			CameraPosi.y = camera->y;
			CameraPosi.z = camera->z;
		}
		else {
			CameraPosi.x = camera->x - Posi.x;
			CameraPosi.y = camera->y - Posi.y;
			CameraPosi.z = camera->z - Posi.z;
		}

		WorldForm();

		smRender.SetCameraPosi( CameraPosi.x,CameraPosi.y,CameraPosi.z, angle->x,angle->y,angle->z );

		ActionRenderD3D();
		return TRUE;

	}
	else {
		return 0;
	}

}

//�� �� �� : �� �� ��
int smOBJ3D::RenderD3dOpening(POINT3D *camera , POINT3D *angle )
{
	int x,y,z;

	x = (Posi.x - camera->x)>>FLOATNS ;
	y = (Posi.y - camera->y)>>FLOATNS ;
	z = (Posi.z - camera->z)>>FLOATNS ;

	if( smObjFixPos )
	{
		//������ ��ǥ�� ī�޶� ��ü
		CameraPosi.x = camera->x;
		CameraPosi.y = camera->y;
		CameraPosi.z = camera->z;
	}
	else
	{
		CameraPosi.x = camera->x - Posi.x;
		CameraPosi.y = camera->y - Posi.y;
		CameraPosi.z = camera->z - Posi.z;
	}

	WorldForm();

	smRender.SetCameraPosi( CameraPosi.x,CameraPosi.y,CameraPosi.z, angle->x,angle->y,angle->z );

	ActionRenderD3D();
	return TRUE;
}

//���Ϸ� ������ ũ�⸦ �̸� ��´�
int smOBJ3D::GetSaveSize()
{
	int size;

	size	 = sizeof( smOBJ3D );
	size	+= sizeof( smVERTEX ) * nVertex;
	size	+= sizeof( smFACE ) * nFace;
	size	+= sizeof( smTEXLINK ) * nTexLink;

	size	+= sizeof( smTM_ROT ) * TmRotCnt;
	size	+= sizeof( smTM_POS ) * TmPosCnt;
	size	+= sizeof( smTM_SCALE ) * TmScaleCnt;
	size	+= sizeof( smMATRIX ) * TmRotCnt;

	if ( Physique )
		size += 32 * nVertex;

	return size;
}

//���Ϸ� ����
int smOBJ3D::SaveFile( HANDLE hFile )
{
	DWORD	dwAcess;
	char	szBuff[64];
	int		cnt;
	int		size;

	Head = 0x41424344;

	//######################################################################################
	//�� �� �� : �� �� ��
	Head |= OBJ_HEAD_TYPE_NEW_NORMAL;
	//######################################################################################

	size =	WriteFile( hFile , &Head	, sizeof( smOBJ3D )					, &dwAcess , NULL );
	size+=	WriteFile( hFile , Vertex	, sizeof( smVERTEX )	* nVertex	, &dwAcess , NULL );
	size+=	WriteFile( hFile , Face		, sizeof( smFACE )		* nFace		, &dwAcess , NULL );
	size+=	WriteFile( hFile , TexLink	, sizeof( smTEXLINK )	* nTexLink	, &dwAcess , NULL );

	size+=	WriteFile( hFile , TmRot	, sizeof( smTM_ROT )	* TmRotCnt	, &dwAcess , NULL );
	size+=	WriteFile( hFile , TmPos	, sizeof( smTM_POS )	* TmPosCnt	, &dwAcess , NULL );
	size+=	WriteFile( hFile , TmScale	, sizeof( smTM_SCALE )	* TmScaleCnt, &dwAcess , NULL );
	size+=	WriteFile( hFile , TmPrevRot, sizeof( smMATRIX )	* TmRotCnt	, &dwAcess , NULL );

	if ( Physique ) {
		for( cnt=0; cnt<nVertex; cnt++ ) {
			ZeroMemory( szBuff , 32 );
			lstrcpy( szBuff , Physique[cnt]->NodeName );
			size+=	WriteFile( hFile , szBuff, 32 , &dwAcess , NULL );
		}
	}

	return size;
}

//���Ϸ� ���� ����Ÿ �ε�
int smOBJ3D::LoadFile( HANDLE hFile , smPAT3D *PatPhysique )
{
	DWORD	dwAcess;
	char	*szBuff;
	int		cnt;
	int		len;
	smTEXLINK	*lpOldTexLink;
	int		SubTexLink;
	
	len=ReadFile( hFile , &Head			, sizeof( smOBJ3D )					, &dwAcess , NULL );

	lpOldTexLink = TexLink;

	//���ο� �޸� �� �Ҵ�
	Vertex	= new smVERTEX[ nVertex ];
	len+= ReadFile( hFile , Vertex		, sizeof( smVERTEX ) * nVertex		, &dwAcess , NULL );

	Face	= new smFACE[ nFace ];
	len+= ReadFile( hFile , Face		, sizeof( smFACE ) * nFace			, &dwAcess , NULL );

	TexLink = new smTEXLINK[ nTexLink ];
	len+= ReadFile( hFile , TexLink		, sizeof( smTEXLINK ) * nTexLink	, &dwAcess , NULL );

	TmRot		= new smTM_ROT[ TmRotCnt ];
	len+= ReadFile( hFile , TmRot		, sizeof( smTM_ROT ) * TmRotCnt		, &dwAcess , NULL );

	TmPos		= new smTM_POS[ TmPosCnt ];
	len+= ReadFile( hFile , TmPos		, sizeof( smTM_POS ) * TmPosCnt		, &dwAcess , NULL );

	TmScale		= new smTM_SCALE[ TmScaleCnt ];
	len+= ReadFile( hFile , TmScale		, sizeof( smTM_SCALE ) * TmScaleCnt	, &dwAcess , NULL );

	//######################################################################################
	//�� �� �� : �� �� ��
	TmPrevRot	= new smFMATRIX[ TmRotCnt ];	
	len+= ReadFile( hFile , TmPrevRot	, sizeof( smFMATRIX ) * TmRotCnt		, &dwAcess , NULL );
	//######################################################################################


	//�ؽ��� ��ǥ�� ��巹�� �����ͷ� �� �����Ƿ� ���� ������
	SubTexLink = TexLink-lpOldTexLink;

	for( cnt=0;cnt<nTexLink;cnt++) {
		if ( TexLink[cnt].NextTex ) {
			SubTexLink = TexLink[cnt].NextTex-lpOldTexLink;
			TexLink[cnt].NextTex = TexLink + SubTexLink;
//			TexLink[cnt].NextTex += SubTexLink;
		}
	}

	for( cnt=0;cnt<nFace;cnt++) {
		if ( Face[cnt].lpTexLink ) {
			SubTexLink = Face[cnt].lpTexLink-lpOldTexLink;
			Face[cnt].lpTexLink = TexLink + SubTexLink;
//			Face[cnt].lpTexLink += SubTexLink;
		}
	}

	if ( Physique && PatPhysique ) {
		Physique = new smOBJ3D *[ nVertex ];
		szBuff = new char[ nVertex * 32 ];
		len+= ReadFile( hFile , szBuff	, nVertex * 32		, &dwAcess , NULL );

		for( cnt=0; cnt<nVertex ; cnt++ ) {
			Physique[cnt] = PatPhysique->GetObjectFromName( szBuff+cnt*32 );
		}

		delete szBuff;
	}

	return len;
}

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////// PAT3D Ŭ���� [OBJECT�� �ټ��� ���� �Ͽ� ����] ///////////////////// 
/////////////////////////////////////////////////////////////////////////////////////////

smPAT3D::smPAT3D()
{
	Init();

}

smPAT3D::~smPAT3D()
{
	Close();
}

void smPAT3D::Init()
{

	nObj3d = 0;
	hD3DTexture = 0;
	TmParent = 0;

	MaxFrame = 0;
	Frame = 0;

	SizeWidth = 0;
	SizeHeight = 0;

	Bound = 0;
	dBound = 0;

	TmFrameCnt = 0;

	TmLastFrame = -1;
	TmLastAngle.x = -1;
	TmLastAngle.y = -1;
	TmLastAngle.z = -1;

	for( int cnt=0;cnt<128;cnt++) TmSort[cnt]=cnt;

	smMaterialGroup = 0;

}

//�� Ŭ���� �ݱ�
void smPAT3D::Close()
{
	int cnt;

	for(cnt=nObj3d-1;cnt>=0;cnt--) {
		delete obj3d[cnt];
	}

	if ( smMaterialGroup ) 
		delete smMaterialGroup;
}

// ������Ʈ �߰�
int smPAT3D::AddObject( smOBJ3D *obj )
{
	int frame;

	if ( nObj3d< 128 ) {
		obj3d[ nObj3d ] = obj;
		nObj3d ++;

		frame = 0;

		if ( obj->TmRotCnt>0 && obj->TmRot ) 
			frame = obj->TmRot[ obj->TmRotCnt-1 ].frame;
		if ( obj->TmPosCnt>0 && obj->TmPos ) 
			frame = obj->TmPos[ obj->TmPosCnt-1 ].frame;
		if ( MaxFrame<frame ) 
			MaxFrame = frame;

		//ũ�� ���� ����
		if ( SizeWidth < obj->maxX ) SizeWidth = obj->maxX;
		if ( SizeWidth < obj->maxZ ) SizeWidth = obj->maxZ;
		if ( SizeHeight < obj->maxY ) SizeHeight = obj->maxY;

		//�ٿ�� ����� ��
		if ( Bound<obj->Bound ) {
			Bound = obj->Bound;
			dBound = obj->dBound;
		}


		return TRUE;
	}

	return FALSE;
}

// �� ������Ʈ�� ������ǥ ��ȯ
int smPAT3D::ReformTM()
{
	int cnt;

	for( cnt=0; cnt<nObj3d; cnt++ ) {
		obj3d[cnt]->SetGNormals();
		obj3d[cnt]->ReformTM();
	}

	return TRUE;
}

// �� ������Ʈ�� ����ũ�� ������ǥ ��ȯ
int smPAT3D::ReformPhysique()
{
	int cnt;

	for( cnt=0; cnt<nObj3d; cnt++ )  {
		obj3d[cnt]->SetGNormals();
		obj3d[cnt]->ReformPhysique();
	}
	MaxFrame = TmParent->MaxFrame;

	return TRUE;
}

//Connect to consider the object-specific parent-child relationship
int smPAT3D::LinkObject()
{

  int cnt, cnt2,ocnt;

	// ��� �������� �θ� �ڽ� ����
	for( cnt=0; cnt<nObj3d ; cnt++ ) {
		if ( obj3d[cnt]->NodeParent[0] ) {
			for( ocnt=0; ocnt<nObj3d; ocnt++ ) {
				if ( _stricmp( obj3d[cnt]->NodeParent , obj3d[ocnt]->NodeName )==0 ) {
					obj3d[cnt]->pParent = obj3d[ocnt];
					break;
				}
			}
		}
		else 
			cnt2 = 0;
	}

	int NodeCnt =0;

	// TM�ִϸ��̼� ���� ���� ���� ( ��ũ������ ���� )
	for( cnt=0;cnt<128;cnt++) TmSort[cnt]=0;

	// ROOT(�θ� ����) ������Ʈ  
	for( cnt=0;cnt<nObj3d; cnt++ ) {
		if ( obj3d[cnt]->pParent==0 ) 
			TmSort[NodeCnt++] = cnt;
	}

	// �θ� �޷��ִ� �ڽ��� ã�� ������� ����
	for( cnt2=0;cnt2<nObj3d; cnt2++ ) {
		for( cnt=0; cnt<nObj3d; cnt++ ) {
			if ( obj3d[cnt]->pParent!=0 && obj3d[TmSort[cnt2]]==obj3d[cnt]->pParent ) {
				TmSort[NodeCnt++] = cnt;
			}
		}
	}  

	return TRUE;
}

//Obtaining the object as a node name
smOBJ3D *smPAT3D::GetObjectFromName( char *name )
{
	int cnt;
	
	for( cnt=0;cnt<nObj3d; cnt++ ) {
		if ( _stricmp( obj3d[cnt]->NodeName , name )==0 ) 
			return obj3d[cnt];
	}

	return NULL;
}

// �� ������Ʈ�� �븻�� ����( 0�Ƿ� �ʱ�ȭ )
void smPAT3D::ZeroNormals()
{
	int cnt;

	for( cnt=0; cnt<nObj3d ; cnt++ ) {
		obj3d[ cnt ]->ZeroNormals();
	}

}

//Set the pattern of the coordinates and angle
void smPAT3D::SetPosi( POINT3D *point , POINT3D *angle )
{
	int cnt;

	Posi.x = point->x;
	Posi.y = point->y;
	Posi.z = point->z;

	Angle.x = angle->x;
	Angle.y = angle->y;
	Angle.z = angle->z;


	for( cnt=0; cnt<nObj3d ; cnt++ ) {
		obj3d[ cnt ]->SetPosi( point , angle );
	}

}

//Holding the coordinate of the pattern to the original coordinates
void smPAT3D::SetFixPosi()
{
	int cnt;
	POINT3D	pos,ang;


	Posi.x = obj3d[0]->Tm._41;
	Posi.y = obj3d[0]->Tm._43;
	Posi.z = obj3d[0]->Tm._42;

	Angle.x = 0;
	Angle.y = 0;
	Angle.z = 0;

	ang.x = 0;
	ang.y = 0;
	ang.z = 0;

	for( cnt=0; cnt<nObj3d ; cnt++ ) {
		pos.x = obj3d[cnt]->Tm._41;
		pos.y = obj3d[cnt]->Tm._43;
		pos.z = obj3d[cnt]->Tm._42;
		obj3d[ cnt ]->SetPosi( &pos , &ang );
	}

}

int smPAT3D::RenderD3D( POINT3D *camera, POINT3D *angle, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo )
{
	int RenderCnt;
	int RCnt;
	int cnt;
	int cnt2,cnt3;
	int x,y,z;
//	int texcnt;
	smMATERIAL *lpMaterial;
	POINT3D pSize;
	POINT3D pPos;

	pPos.x = 0;
	pPos.z = 0;
	pPos.y = 0;

	x = (Posi.x - camera->x)>>FLOATNS ;
	y = (Posi.y - camera->y)>>FLOATNS ;
	z = (Posi.z - camera->z)>>FLOATNS ;

	RenderCnt = 0;


	if ( smMaterialGroup ) {
		smRender.SetMaterialGroup( smMaterialGroup );
		if ( smMaterialGroup->ReformTexture ) {
			if ( TestScreenView( x , y , z ,  Bound , &Angle , angle )==TRUE )
			{
				//######################################################################################
				//�� �� �� : �� �� ��
				SetFrame( Frame, TalkPattern, lpFrameInfo );
				//######################################################################################

				for( cnt=0; cnt<nObj3d ; cnt++ )
				{
					//######################################################################################
					//�� �� �� : �� �� ��
					if( obj3d[cnt]->TestRender( camera , angle ) )
					{
						for(cnt2=0;cnt2<(int)smMaterialGroup->MaterialCount;cnt2++)
						{
							//######################################################################################
							//�� �� �� : �� �� ��							
							lpMaterial = &smMaterialGroup->smMaterial[ cnt2 ];
							//lpMaterial = smMaterialGroup->smMaterial;
							//######################################################################################

							if ( lpMaterial->ReformTexture ) {
								for(cnt3=0;cnt3<(int)lpMaterial->TextureCounter;cnt3++) {
									if ( lpMaterial->TextureFormState[cnt3] ) {
										pSize.x = Bound<<(FLOATNS+1);
										pSize.y = Bound<<(FLOATNS+1);
										pSize.z = Bound<<(FLOATNS+1);
										obj3d[cnt]->ReformTexture( lpMaterial->TextureFormState[cnt3] ,
											cnt2,cnt3,&pPos,&pSize );
										break;
									}
								}
							}
						}

						RCnt= obj3d[cnt]->ActionRenderD3D();
						RenderCnt+=RCnt;
					}
					//######################################################################################
				}
				return RenderCnt;
			}
		}
	}

	if ( TestScreenView( x , y , z ,  Bound , &Angle , angle )==TRUE )
	{
		//######################################################################################
		//�� �� �� : �� �� ��
		SetFrame( Frame, TalkPattern, lpFrameInfo );
		//######################################################################################

		for( cnt=0; cnt<nObj3d ; cnt++ ) {
			RCnt= obj3d[cnt]->RenderD3D( camera , angle );
			RenderCnt+=RCnt;
		}
		DEBUG_OBJSUM ++;
	}

	return RenderCnt;
}

int smPAT3D::ChoiseRenderD3D( POINT3D *camera, POINT3D *angle, char **szNodeList, int ListCnt, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo )
{
	int RenderCnt;
	int RCnt;
	int cnt;
	int cnt2;//,cnt3;
	int x,y,z;
//	int texcnt;
//	smMATERIAL *lpMaterial;
//	POINT3D pSize;
	POINT3D pPos;

	pPos.x = 0;
	pPos.z = 0;
	pPos.y = 0;

	x = (Posi.x - camera->x)>>FLOATNS ;
	y = (Posi.y - camera->y)>>FLOATNS ;
	z = (Posi.z - camera->z)>>FLOATNS ;

	RenderCnt = 0;


	if( smMaterialGroup )
		smRender.SetMaterialGroup( smMaterialGroup );

	if( TestScreenView( x, y, z,  Bound, &Angle, angle ) == TRUE )
	{
		//######################################################################################
		//�� �� �� : �� �� ��
		SetFrame( Frame, TalkPattern, lpFrameInfo );
		//######################################################################################

		for( cnt2=0; cnt2 < ListCnt; cnt2++ )
		{
			for( cnt=0; cnt < nObj3d; cnt++ )
			{
				if( lstrcmpi( szNodeList[cnt2], obj3d[cnt]->NodeName ) == 0 )
				{
					RCnt = obj3d[cnt]->RenderD3D( camera, angle );
					RenderCnt += RCnt;
				}
			}
		}
		DEBUG_OBJSUM ++;
	}
	return RenderCnt;
}

//D3D rendering a primitive way (input coordinates and the camera angle camera)
int smPAT3D::RenderD3D( POINT3D *camera , POINT3D *angle, SMotionStEndInfo *lpFrameInfo )
{
	int RenderCnt;
	int RCnt;
	int cnt;
	int cnt2,cnt3;
	int x,y,z;
//	int texcnt;
	smMATERIAL *lpMaterial;
	POINT3D pSize;
	POINT3D pPos;

	pPos.x = 0;
	pPos.z = 0;
	pPos.y = 0;

	x = (Posi.x - camera->x)>>FLOATNS ;
	y = (Posi.y - camera->y)>>FLOATNS ;
	z = (Posi.z - camera->z)>>FLOATNS ;

	RenderCnt = 0;

	if ( smMaterialGroup )
	{
		smRender.SetMaterialGroup( smMaterialGroup );
		if ( smMaterialGroup->ReformTexture )
		{
			if ( TestScreenView( x , y , z ,  Bound , &Angle , angle )==TRUE )
			{
				SetFrame( Frame, lpFrameInfo );

				for( cnt=0; cnt<nObj3d ; cnt++ )
				{
					if( obj3d[cnt]->TestRender( camera , angle ) )
					{
						for(cnt2=0;cnt2<(int)smMaterialGroup->MaterialCount;cnt2++)
						{					
							lpMaterial = &smMaterialGroup->smMaterial[ cnt2 ];

							if ( lpMaterial->ReformTexture )
							{
								for(cnt3=0;cnt3<(int)lpMaterial->TextureCounter;cnt3++)
								{
									if ( lpMaterial->TextureFormState[cnt3] )
									{
										pSize.x = Bound<<(FLOATNS+1);
										pSize.y = Bound<<(FLOATNS+1);
										pSize.z = Bound<<(FLOATNS+1);
										obj3d[cnt]->ReformTexture( lpMaterial->TextureFormState[cnt3] ,
											cnt2,cnt3,&pPos,&pSize );
										break;
									}
								}
							}
						}

						RCnt= obj3d[cnt]->ActionRenderD3D();
						RenderCnt+=RCnt;
					}
					//######################################################################################
				}
				return RenderCnt;
			}
		}
	}

	if ( TestScreenView( x , y , z ,  Bound , &Angle , angle )==TRUE ) {
		SetFrame( Frame, lpFrameInfo );

		for( cnt=0; cnt<nObj3d ; cnt++ ) 
		{
			RCnt= obj3d[cnt]->RenderD3D( camera , angle );
			RenderCnt+=RCnt;
		}
		DEBUG_OBJSUM ++;
	}

	return RenderCnt;
}

//D3D rendering primitives optional manner (input coordinates and the camera angle camera)
int smPAT3D::ChoiseRenderD3D( POINT3D *camera , POINT3D *angle , char **szNodeList , int ListCnt, SMotionStEndInfo *lpFrameInfo )
{
	int RenderCnt;
	int RCnt;
	int cnt;
	int cnt2;//,cnt3;
	int x,y,z;
//	int texcnt;
//	smMATERIAL *lpMaterial;
//	POINT3D pSize;
	POINT3D pPos;

	pPos.x = 0;
	pPos.z = 0;
	pPos.y = 0;

	x = (Posi.x - camera->x)>>FLOATNS ;
	y = (Posi.y - camera->y)>>FLOATNS ;
	z = (Posi.z - camera->z)>>FLOATNS ;

	RenderCnt = 0;


	if ( smMaterialGroup ) {
		smRender.SetMaterialGroup( smMaterialGroup );
	}

	if ( TestScreenView( x , y , z ,  Bound , &Angle , angle )==TRUE ) {
		SetFrame( Frame, lpFrameInfo );

		for( cnt2=0; cnt2<ListCnt ; cnt2++ ) {
			for( cnt=0; cnt<nObj3d ; cnt++ ) {
				if ( lstrcmpi( szNodeList[cnt2] , obj3d[cnt]->NodeName )==0 ) {
					RCnt= obj3d[cnt]->RenderD3D( camera , angle );
					RenderCnt+=RCnt;
				}
			}
		}
		DEBUG_OBJSUM ++;
	}
	return RenderCnt;
}

int smPAT3D::RenderD3dOpening( POINT3D *camera , POINT3D *angle )
{
	int RenderCnt = 0;
	int RCnt = 0;
	int cnt;

	if ( smMaterialGroup )
	{
		smRender.SetMaterialGroup( smMaterialGroup );
        SetFrame( Frame );

		for( cnt=0; cnt<nObj3d ; cnt++ )
		{
			RCnt= obj3d[cnt]->RenderD3dOpening( camera , angle );
			RenderCnt+=RCnt;
		}
	}

	DEBUG_OBJSUM ++;

	return RenderCnt;
}

//�� �� �� : �� �� ��
int smPAT3D::SetFrame( int frame, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo )
{
	if( frame < 0 || TalkPattern->Frame < 0 )
		return FALSE;

	TalkPattern->TmTalkAnimation( TalkPattern->Frame );

	if( TmParent )
		TmParent->TmAnimation( frame, Angle.x, Angle.y, Angle.z, TalkPattern, lpFrameInfo );
	else
		TmAnimation( frame, Angle.x, Angle.y, Angle.z, TalkPattern, lpFrameInfo );
	return TRUE;
}

int smPAT3D::TmTalkAnimation( int frame )
{
	int cnt;

	if( TmLastFrame == frame )
		return TRUE;

	TmLastFrame = frame;
	for( cnt=0; cnt < nObj3d; cnt++ )
	{
		obj3d[ TmSort[cnt] ]->TmTalkAnimation( frame );
	}

	return TRUE;
}

int smPAT3D::TmAnimation( int frame, int ax, int ay, int az, smPAT3D *TalkPattern, SMotionStEndInfo *lpFrameInfo )
{
	int cnt;

	if ( TmLastFrame == frame && TmLastAngle.x == ax && TmLastAngle.y == ay && TmLastAngle.z == az )
		return TRUE;

	TmLastFrame = frame;
	TmLastAngle.x = ax;
	TmLastAngle.y = ay;
	TmLastAngle.z = az;

	for( cnt=0; cnt < nObj3d; cnt++ )
		obj3d[ TmSort[cnt] ]->TmAnimation( frame , ax, ay, az, TalkPattern, lpFrameInfo );

	return TRUE;

}

//Obtaining a value corresponding to the animation frame
int smPAT3D::SetFrame( int frame, SMotionStEndInfo *lpFrameInfo )
{
	int cnt;

	if ( frame<0 ) return FALSE;

	if ( TmParent ) {
		TmParent->TmAnimation( frame , Angle.x , Angle.y , Angle.z, lpFrameInfo );
		for( cnt=0; cnt<nObj3d ; cnt++ ) {
			if ( obj3d[cnt]->pParent ) {
				memcpy( &obj3d[cnt]->mWorld , &obj3d[cnt]->pParent->mWorld , sizeof( smMATRIX ) );
			}
		}
	}
	else
		TmAnimation( frame , Angle.x , Angle.y , Angle.z, lpFrameInfo );

	return TRUE;
}

//Animation and rotate world matrix, such as setting per-object
int smPAT3D::TmAnimation( int frame , int ax, int ay, int az, SMotionStEndInfo *lpFrameInfo )
{
	int cnt;

	if ( TmLastFrame==frame && 
		TmLastAngle.x==ax && TmLastAngle.y==ay && TmLastAngle.z==az )
		return TRUE;

	TmLastFrame = frame;
	TmLastAngle.x = ax;
	TmLastAngle.y = ay;
	TmLastAngle.z = az;


	for( cnt=0; cnt<nObj3d ; cnt++ ) {
		obj3d[TmSort[cnt]]->TmAnimation( frame , ax, ay, az, lpFrameInfo ) ;
	}

	return TRUE;

}

//Connects all the objects define the mouth of the parent to a specific object's parent
int smPAT3D::LinkParentObject( smPAT3D *patParent , smOBJ3D *objParent )
{

	int cnt;

	TmParent = patParent;

	//if ( objParent ) {
		for( cnt=0; cnt<nObj3d ; cnt++ ) {
			obj3d[ cnt ]->pParent = objParent;
		}
	//}

	return TRUE;

}

//�� �� �� : �� �� ��
static char *szSMDFileHeader = "SMD Model data Ver 0.62";
//static char *szSMDFileHeader = "SMD Model data Ver 1.00";

//Storing data in a file
int	smPAT3D::SaveFile( char *szFile )
{

	HANDLE	hFile;
	DWORD	dwAcess;
	int		cnt;
	int		pFile;
	int		size;

	smDFILE_HEADER	FileHeader;
	smDFILE_OBJINFO *FileObjInfo;


	lstrcpy( FileHeader.szHeader , szSMDFileHeader );

	//��� ����
	if ( smMaterialGroup )
		FileHeader.MatCounter = smMaterialGroup->MaterialCount;
	else
		FileHeader.MatCounter = 0;

	FileHeader.ObjCounter = nObj3d;


	FileObjInfo = new smDFILE_OBJINFO [ nObj3d ];

	//���� ������
	pFile  = sizeof( smDFILE_HEADER ) + sizeof( smDFILE_OBJINFO ) * nObj3d;// + sizeof( smPAT3D );

	FileHeader.MatFilePoint = pFile;

	//��Ʈ���� ����Ÿ ���� ũ��
	if ( smMaterialGroup ) 
		pFile+= smMaterialGroup->GetSaveSize();

	FileHeader.First_ObjInfoPoint = pFile;

	FileHeader.TmFrameCounter = TmFrameCnt;
	memcpy( FileHeader.TmFrame , TmFrame , sizeof( smFRAME_POS ) * OBJ_FRAME_SEARCH_MAX );


	//������ OBJ�����Ͷ� ���� ���� ��
	for( cnt=0;cnt<nObj3d;cnt++) {
		lstrcpy( FileObjInfo[cnt].szNodeName , obj3d[cnt]->NodeName );
		size = obj3d[cnt]->GetSaveSize();
		FileObjInfo[cnt].ObjFilePoint = pFile;
		FileObjInfo[cnt].Length = size;

		pFile += size;
	}

	//���Ϸ� ����
	hFile = CreateFile( szFile , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	//��� ����
	WriteFile( hFile , &FileHeader , sizeof( smDFILE_HEADER ) , &dwAcess , NULL );
	WriteFile( hFile , FileObjInfo , sizeof( smDFILE_OBJINFO ) * nObj3d, &dwAcess , NULL );
	//WriteFile( hFile , &Head , sizeof( smPAT3D ), &dwAcess , NULL );

	//��Ʈ���� ����Ÿ ����
	if ( smMaterialGroup ) smMaterialGroup->SaveFile( hFile );

	//������Ʈ ����Ÿ ����
	for( cnt=0;cnt<nObj3d;cnt++) {
		size = obj3d[cnt]->SaveFile( hFile );
		if ( !size ) {		//������!
			CloseHandle( hFile );
			delete FileObjInfo;
			return FALSE;
		}
	}

	//�ڵ� �ݱ� ����
	CloseHandle( hFile );
	delete FileObjInfo;

	return TRUE;
}

//Loading data from files
int	smPAT3D::LoadFile( char *szFile , char *szNodeName )
{
	HANDLE	hFile;
	DWORD	dwAcess;
	int		cnt;
	int		size;
	smOBJ3D	*obj;
	smPAT3D *BipPat;

	smDFILE_HEADER	FileHeader;
	smDFILE_OBJINFO *FileObjInfo;

	Init();

	BipPat = smASE_GetPhysique();

	hFile = CreateFile( szFile , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );

	//�׷��� ������ �о��
	size=ReadFile( hFile , &FileHeader , sizeof( smDFILE_HEADER ) , &dwAcess , NULL );

	//����� Ʋ�� ( ������ Ʋ���ų�.. )
	if ( lstrcmp( FileHeader.szHeader , szSMDFileHeader )!=0 ) 
	{
		//�ڵ� �ݱ� ����
		CloseHandle( hFile );
		return FALSE;
	}

	FileObjInfo = new smDFILE_OBJINFO [ FileHeader.ObjCounter ];
	size=ReadFile( hFile , FileObjInfo , sizeof( smDFILE_OBJINFO ) * FileHeader.ObjCounter, &dwAcess , NULL );

	TmFrameCnt = FileHeader.TmFrameCounter;
	memcpy( TmFrame , FileHeader.TmFrame , sizeof( smFRAME_POS ) * OBJ_FRAME_SEARCH_MAX );

	//��Ʈ���� �о��
	if ( FileHeader.MatCounter ) {
		smMaterialGroup = new smMATERIAL_GROUP;
		smMaterialGroup->LoadFile( hFile );
	}

	if ( szNodeName ) {
		//������Ʈ 1���� �ҷ���
		for(cnt=0;cnt<FileHeader.ObjCounter;cnt++) {
			if ( lstrcmpi( szNodeName , FileObjInfo[cnt].szNodeName )==0 ) {
				obj = new smOBJ3D;
				if ( obj ) {
					SetFilePointer( hFile , FileObjInfo[cnt].ObjFilePoint , NULL , FILE_BEGIN );
					obj->LoadFile( hFile , BipPat );
					AddObject( obj );
				}
				break;
			}
		}
	}
	else {
		//��� ������Ʈ �ҷ���
		for(cnt=0;cnt<FileHeader.ObjCounter;cnt++) {
			obj = new smOBJ3D;
			if ( obj ) {
				obj->LoadFile( hFile , BipPat );
				AddObject( obj );
			}
		}
		LinkObject();
	}

	TmParent = BipPat;

	//�ڵ� �ݱ� ����
	CloseHandle( hFile );
	delete FileObjInfo;

	return TRUE;
}

//Find the correct pattern storage file check (check version)
int smFindPatFile( char *szfile , char *FileExt )
{
	HANDLE	hFile;
	DWORD	dwAcess;

	smDFILE_HEADER	FileHeader;

	//���� ã��
	//######################################################################################
	//�� �� �� : �� �� ��
	char *szFileName = smFindFile( szfile, FileExt );	
	if( szFileName )
	//######################################################################################
	{
		//######################################################################################
		//�� �� �� : �� �� ��		
		hFile = CreateFile( szFileName, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		//######################################################################################

		//����� ������ �о��
		ReadFile( hFile , &FileHeader , sizeof( smDFILE_HEADER ) , &dwAcess , NULL );

		//�ڵ� ����
		CloseHandle( hFile );

		//����� Ʋ�� ( ������ Ʋ���ų�.. )
		if ( lstrcmp( FileHeader.szHeader , szSMDFileHeader )!=0 ) {
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}