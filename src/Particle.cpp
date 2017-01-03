#include "smlib3d\\smd3d.h"
#include "particle.h"

extern DWORD dwPlayTime;

int		SetParticleCnt = 0;

class smPARTICLE {

public:
	int InUse;

	int BaseMaterial;
	int Material;

	int AnimType;

	DWORD	dwStartTime;

	POINT	DefSize;

	POINT3D Posi;
	POINT3D DispPosi;
	POINT	DispSize;

	POINT3D	MovePosi;

	int		Counter;
	int		Angle;
	int		SizeAround;
	int		EndCounter;
	int		ColR,ColG,ColB,ColA;
	int		MatFrame;					//��Ʈ���� ���� ������

	smPARTICLE();
	~smPARTICLE();
	int SetParticle( int x, int y, int z , int Material , int Type , int Count , int Param );
	int Play();
	int SetRender();


};

smPARTICLE::smPARTICLE()
{

}

smPARTICLE::~smPARTICLE()
{

}


int smPARTICLE::SetParticle( int x, int y, int z , int MatNum , int Type , int Count , int Param )
{

	int Temp;
//	int cosX,cosY,sinX,sinY;
	int angY,angX;


	InUse = TRUE;
	AnimType = Type;
	Counter = 0;
	Material = MatNum;
	BaseMaterial = MatNum;
	EndCounter = 1024;
	MatFrame = -1;

	dwStartTime	= dwPlayTime;

	Posi.x = x;
	Posi.y = y;
	Posi.z = z;

	DispPosi.x = x;
	DispPosi.y = y;
	DispPosi.z = z;

	switch( Type ) {
	case SMPARTICLETYPE_SHINE1:

		DispSize.x = fONE*16;
		DispSize.y = fONE*16;

		ColA = 255;

		Temp = Count&3;

		switch( Temp ) {
		case 0:
			ColR = 255;
			ColG = 255;
			ColB = 255;
			break;
		case 1:
			ColR = 128;
			ColG = 128;
			ColB = 255;
			break;
		case 2:
			ColR = 255;
			ColG = 128;
			ColB = 128;
			break;
		case 3:
			ColR = 128;
			ColG = 255;
			ColB = 128;
			break;
		}

		Angle = ((timeGetTime()-(Count<<4))<<6) & ANGCLIP;

		EndCounter = 128;
		break;


	case SMPARTICLETYPE_BLOOD1:
		//�� �ڱ�
		DispSize.x = fONE*24;
		DispSize.y = fONE*24;
		ColR = 255;
		ColG = 255;
		ColB = 255;
		ColA = 255;
		break;

	case SMPARTICLETYPE_BLOOD2:
		//�� �ڱ�
		DispSize.x = fONE*2;
		DispSize.y = fONE*2;
		ColR = 255;
		ColG = 255;
		ColB = 255;
		ColA = 255;

		angY = Param+(rand()%512)-256;
		angX = (rand()%512)-256;
		angY &= ANGCLIP;
		angX &= ANGCLIP;

		GetMoveLocation( 0 , 0, -fONE*8 , angX, angY, 0 );

		DispPosi.x += GeoResult_X;
		DispPosi.y += GeoResult_Y;
		DispPosi.z += GeoResult_Z;


		GetMoveLocation( 0 , 0, 32768 , angX, angY, 0 );

		MovePosi.x = GeoResult_X;
		MovePosi.y = GeoResult_Y;
		MovePosi.z = GeoResult_Z;
		break;
	}

	return TRUE;
}

int smPARTICLE::Play()
{

	switch( AnimType ) {
	case SMPARTICLETYPE_SHINE1:

		Angle &= ANGCLIP;
		SizeAround = 16*fONE;

		DispPosi.x = Posi.x+ (((GetSin[Angle]>>FLOATDS)*SizeAround)>>FLOATNS);
		DispPosi.z = Posi.z+ (((GetCos[Angle]>>FLOATDS)*SizeAround)>>FLOATNS);
		DispPosi.y = Posi.y;

		Posi.y += 128;

		Angle+= 32;

		if ( DispSize.x>fONE )	DispSize.x -= 32;
		DispSize.y = DispSize.x;

		break;

	case SMPARTICLETYPE_BLOOD1:
		//���۱� ǥ��
		Material = BaseMaterial + (Counter>>2);
		if ( Material>=BaseMaterial+4 ) {
			InUse=0;
			return FALSE;
		}
		break;

	case SMPARTICLETYPE_BLOOD2:
		DispPosi.x += (MovePosi.x>>6); 
		DispPosi.y += (MovePosi.y>>6); 
		DispPosi.z += (MovePosi.z>>6); 
		//���۱� ǥ��
		Material = BaseMaterial + (Counter>>2);
		if ( Material>=BaseMaterial+4 ) {
			InUse=0;
			return FALSE;
		}
		break;
	}

	Counter ++;

	if ( Counter>EndCounter ) {
		InUse=0;
		return FALSE;
	}

	return TRUE;
}

int smPARTICLE::SetRender()
{

	smFACE2D face2d;

	face2d.TexRect.left = 0;
	face2d.TexRect.right = 1;
	face2d.TexRect.top = 0;
	face2d.TexRect.bottom = 1;

	face2d.width  = DispSize.x;
	face2d.height = DispSize.y;
	face2d.Transparency = ColA;
	face2d.r = ColR; face2d.g = ColG; face2d.b = ColB;
	face2d.x = DispPosi.x;
	face2d.z = DispPosi.z;
	face2d.y = DispPosi.y;
	face2d.MatNum = Material;

	if ( smRender.AddFace2D( &face2d ) )
		return TRUE;

	return FALSE;
}


smPARTICLE	smParticle[PARTICLE_MAX];
int			ParticleCnt =0;
int			LastParticle =0;



//��ƼŬ �ʱ�ȭ
int InitParticle()
{
	ZeroMemory( smParticle , sizeof( smPARTICLE ) * PARTICLE_MAX );

	return TRUE;
}

//��ƼŬ �߰�
int SetParticle( int x, int y, int z , int MatNum , int Type ,int Param )
{
	int cnt;

	for( cnt=0;cnt<PARTICLE_MAX;cnt++) {
		LastParticle++;
		if ( LastParticle>=PARTICLE_MAX ) LastParticle = 0;

		if ( smParticle[ LastParticle ].InUse==0 ) break;
	}

	if ( cnt==PARTICLE_MAX ) return FALSE;

	smParticle[ LastParticle ].SetParticle( x,y,z,MatNum , Type , ParticleCnt , Param );

	ParticleCnt++;
	SetParticleCnt++;

	return TRUE;
}

//��ƼŬ ����
int PlayParticle()
{
	int cnt;

	for( cnt=0;cnt<PARTICLE_MAX;cnt++) {
		if ( smParticle[cnt].InUse ) smParticle[cnt].Play();
	}
	return TRUE;
}

//��ƼŬ �����
int DrawParticle(int x, int y, int z, int ax, int ay, int az )
{
	int cnt;
	int flag=0;

	smRender.Init();
	smRender.SetCameraPosi( x,y,z, ax,ay,az );

	for( cnt=0;cnt<PARTICLE_MAX;cnt++) {
		if ( smParticle[cnt].InUse ) 
			flag |= smParticle[cnt].SetRender();
	}

	if ( flag ) {
		smRender.SetClipStates( SMCLIP_LEFT|SMCLIP_RIGHT|SMCLIP_TOP|SMCLIP_BOTTOM );
		smRender.ClipRendFace();							// ��ü ������ ���̽��� Ŭ����
		smRender.GeomVertex2D( FALSE );				// ���ؽ��� 2D��ǥ�� ��ȯ  
		smRender.RenderD3D();
	}

	return TRUE;
}

/*
//�������� �� ����ü
struct smRENDLIGHT {
	int type;
	int x,y,z;
	int rx,ry,rz;
	int Range;
	int dRange;
	int r,g,b,a;
};
*/

//#define DYN_LIGHT_TYPE_SPARK		1
//#define DYN_LIGHT_TYPE_SPARK		1

class	scDYNLIGHT {
public:

	int		Flag;
	int		Count;

	int		Power;				//����
	int		pX,pY,pZ;			//��ġ
	int		R,G,B,A;			//���� �߶���

	int		Range;				//����
	int		DecPower;			//���� �Ŀ�

	void	Init();
	void	Close();
	int		Set( int x, int y, int z, int r, int g, int b, int a, int power , int type , int decPower=10 );
	int		Main();
	int		Apply( smRENDER3D *lpRender );

};

void scDYNLIGHT::Init()
{

}

void scDYNLIGHT::Close()
{
	Flag = FALSE;
}

int scDYNLIGHT::Set( int x, int y, int z, int r, int g, int b, int a, int power , int type , int decPower )
{
	Flag = TRUE;

	pX = x;
	pY = y;
	pZ = z;

	R = r;
	G = g;
	B = b;
	A = a;

	Power = power;
	DecPower = decPower;

	return TRUE;
}

int	scDYNLIGHT::Main()
{
	if ( !Flag ) return FALSE;

	if ( Power>0 ) {
		Power -= DecPower;
		if ( Power<=0 ) {
			Power = 0;
			Flag = 0;
			return TRUE;
		}
	}

	return TRUE;
}

int	scDYNLIGHT::Apply( smRENDER3D *lpRender )
{
	int r,g,b,a;

	if ( !Flag ) return FALSE;

	Range = (Power>>1)*fONE;
	if ( Range<64*fONE ) Range = 64*fONE;

	r = (Power*R)>>FLOATNS;
	g = (Power*G)>>FLOATNS;
	b = (Power*B)>>FLOATNS;
	a = (Power*A)>>FLOATNS;

	lpRender->AddDynamicLight( pX, pY, pZ , r,g,b,a, Range );
		
//int	smRENDER3D::AddDynamicLight( int x, int y, int z, int r, int g, int b, int a, int Range )

	return TRUE;
}

#define scLIGHT_MAX		80

scDYNLIGHT	scDynLight[scLIGHT_MAX];


//���� ���� �ʱ�ȭ
int InitDynLight()
{
	int cnt;

	for( cnt=0;cnt<scLIGHT_MAX;cnt++) {
		scDynLight[cnt].Init();
	}

	return TRUE;
}


//���� ���� �ݱ�
int CloseDynLight()
{
	int cnt;

	for( cnt=0;cnt<scLIGHT_MAX;cnt++) {
		scDynLight[cnt].Close();
	}

	return TRUE;
}


//���� ���� ����
int DynLightMain()
{
	int cnt;

	for( cnt=0;cnt<scLIGHT_MAX;cnt++) {
		if ( scDynLight[cnt].Flag )
			scDynLight[cnt].Main();
	}

	return TRUE;
}



//���� ���� �������� ����
int DynLightApply()
{
	int cnt;

	for( cnt=0;cnt<scLIGHT_MAX;cnt++) {
		if ( scDynLight[cnt].Flag )
			scDynLight[cnt].Apply( &smRender );
	}

	return TRUE;
}


//���� ���� ����
int SetDynLight( int x,int y,int z, int r, int g, int b, int a, int power , int DecPower )
{
	int cnt;

	for( cnt=0;cnt<scLIGHT_MAX;cnt++)
		if ( !scDynLight[cnt].Flag ) break;

	if ( cnt<scLIGHT_MAX ) {
		scDynLight[cnt].Set( x, y, z, r, g, b, a, power , TRUE , DecPower );
		return TRUE;
	}

	return FALSE;
}
