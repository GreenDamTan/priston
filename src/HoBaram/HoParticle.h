#ifndef _HO_PARTICLE_H_
#define _HO_PARTICLE_H_

#ifndef D3D_OVERLOADS
#define D3D_OVERLOADS
#endif


//��� ���� �Ǿ�� �ϴ� ��ƼŬ�鿡 ���� ����...(��, ����, �ǵ�) 
#define MAX_PARTICLES	200
#define RANDOM_NUM	(((FLOAT)rand()-(FLOAT)rand())/RAND_MAX)

#define MIN_SPEED		0.0f	// unit/��    (��������)
#define MIN_LIFETIME	0.1f	// �ּ� ����. (�ʴ���)
#define MIN_SIZE		2.0f	// Particle�� �ּ� ũ��.

#define MIN_SPREAD		0.01f	// in degrees
#define MIN_EMISSION	1.0f	// in particles / sec
#define MIN_GRAVITY		-3.0f	// as a multiple of normal gravity 
#define MIN_ALPHA		0.0f	// as a ratio 

#define MAX_SPEED		500.0f	// unit/�� (��������)
#define MAX_LIFETIME	10.0f	// Frame�� �ְ� ����(�ʴ���)
#define MAX_SIZE		20.0f	// Particle�� �ְ� ũ��.

#define MAX_SPREAD		180.0f	// in degrees
#define MAX_EMISSION    1000.0f	// in particles / sec
#define MAX_GRAVITY		3.0f	// as a multiple of normal gravity 
#define MAX_ALPHA		255.0f	// as a ratio 


// Radnom���� �ּ�, �ִ� ���� �ȿ� ��� �Ѵ�.
#define Clamp(x, min, max)  x = (x<min  ? min : x<max ? x : max);

enum ParticleSystemType
{
	PS_FIRE,
	PS_DUST,
	PS_BLOOD
};

class HoParticleSystem;
class HoParticle
{
public:
	HoParticle();
	~HoParticle();
public:
	D3DVECTOR		PrevLocation;	//���������� ������Ʈ�� ��ƼŬ�� ��ġ.
	D3DVECTOR		Location;		//������ ��ƼŬ�� ��ġ.
	
	D3DVECTOR		Velocity;		//��ƼŬ�� �ӵ� ����.
	
	D3DCOLORVALUE	Color;			//������ Į��.
	D3DCOLORVALUE	ColorDelta;     //Į���� ��ȭ��
	
	float			Age;			//��ƼŬ�� ����� �ð�..
	float			LifeTime;		//��ƼŬ�� ������� �ð�.
	
	float			Size;			//��ƼŬ�� ������...
	float			SizeDelta;		//��ƼŬ�� ������ ��ȭ��..
	
	float			Alpha;			//���İ�..
	float			AlphaDelta;		//���� ��ȭ��...

	float			Gravity;		//�߷�...
	float			GravityDelta;	//�߷� ��ȭ��...

	float			TimeCount;
	
	HoParticleSystem*	Parent;
	void			SetParent(HoParticleSystem* parent){ Parent = parent;}

public:
	bool Main();
};


class HoParticleSystem : public HoEffectObject
{
public:
	HoParticleSystem();
	virtual ~HoParticleSystem();
	
	void SetState(BOOL startFlag) { Age = -1.0f;}
	//��� ��ƼŬ���� �����Ѵ�.
	virtual int	  Main();
	virtual int	  Draw(int x, int y, int z, int ax, int ay, int az);
	virtual void  Init();
			int	  Start(int x, int y, int z, int matNum, int particleCode = 0);
	void		  GetLocation(D3DVECTOR& vLocation);
	void		  TranslateWorld(int x, int y, int z);


	//ktj : MONSTER_IMP_SHOT3�ǰ�� shadow�� ������ ��ƼŬ�� ��ġ�� �˰Գ����Ƿ� �Ѱ��� ���������Ѱ���.

	int make_pSu;
	int ParticleCode;

public:

	

	BOOL		 AutoDieFlag;		 //�ڵ����� �ð��� �Ǹ� �Ҹ�Ǵ� ��ƼŬ...
	int			 MaterialNum;		//���� ����ϴ� ��ƼŬ�� Texture(��ȣ)
	
	unsigned int ParticlesAlive;	//��ƼŬ�� ��� �ִ���..
	unsigned int ParticlesPerSec;	//��ƼŬ �ʴ� �Ѹ��� ����...

	D3DVECTOR	 PrevLocation;		//��ƼŬ�� ���� ��ġ
	D3DVECTOR	 Location;			//��ƼŬ�� ���� ��ġ

	D3DVECTOR	 Velocity;			//��ƼŬ�� �ӵ�
	D3DVECTOR    Direction;			//��ƼŬ�� ����..

	//Var���� ���� �� �ְ� ���� �� ���� �ȴ�.(�⺻�����δ� 0�̴�)
	float	GravityStart;			//�߷� ���۰�.
	float	GravityEnd;

	float	SizeStart;				//������ ���۰�.
	float	SizeEnd;

	float   AlphaStart;				//���� ���۰�.
	float	AlphaEnd;
	
	D3DCOLORVALUE	ColorStart;		//Į�� ��ŸƮ ��.
	D3DCOLORVALUE	ColorEnd;
	
	//��ƼŬ�� Ƣ�� ������ �����ϰ�....(��ƼŬ�� ������ �� �ʿ��� ��ƾ)..
	float	Theta;
	float	Speed;				//��ƼŬ�� ���ݾ� ��鸰��...
	float	Life;				//��ƼŬ�� ��� �ִ� �ð�...

	float	EmissionResidue;	//��ƼŬ�� ���� �Ѹ��� ���� �� ����ϴ� ����...
	float	Age;				//��ƼŬ �ý����� ��� ���� �ð�..
	
	float	TimeCount;
	HoParticle Particles[MAX_PARTICLES];
	smFACE2D			Face2d;
};


class HoParticleBow : public HoParticleSystem
{
public:
	HoParticleBow();
	~HoParticleBow();
	
	int Main();
	int Start(int x, int y, int z, int r, int g, int b, int a, int matNum);
	int StartFire(int x, int y, int z, int r, int g, int b, int a, int matNum);

};

//PikeMan�� PikeWind�� ���� ���... ����� ���� �ö󰡴°͸� ��..
class HoParticleTornado : public HoParticleSystem
{
public:
	int LocalAngle;
	HoParticleTornado();
	~HoParticleTornado();

	int Main();
	int Start(int originX, int originY, int originZ, int localX, int localY, int localZ, int matNum);
};


//� �������� ���� ���� ��ƼŬ.. (���� ������ �� ������ �����ϸ� ���� �������� ����������
//                                  ��ƼŬ�� �Ѹ��� ���� ����.)
class HoParticleDest : public HoParticleSystem
{
public:
	int DestX, DestY, DestZ;
	int LocalAngle;
	BOOL EndWaveFlag;
	HoParticleDest();
	~HoParticleDest();
	
	int Main();
	void SetAge(float age) { Age = age;}
	int Start(int worldX, int worldY, int worldZ, int destX, int destY, int destZ, int matNum, int type);
	
};

class HoParticleSpark : public HoParticleSystem
{
public:
	HoParticleSpark();
	~HoParticleSpark();
	void  Init();
	//int   Main();
	int   Draw(int x, int y, int z, int ax, int ay, int az);
	
	bool AddFace2D( smFACE2D *face, int particleIndex);

	//void SetMaterialNum(int matNum);
	void Start(int x, int y, int z, int matNum, int type);

};



#endif