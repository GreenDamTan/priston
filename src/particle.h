#define PARTICLE_MAX	500

//��ƼŬ �ڵ�
#define SMPARTICLETYPE_SHINE1		0x0001
#define SMPARTICLETYPE_BLOOD1		0x0010
#define SMPARTICLETYPE_BLOOD2		0x0011

//��ƼŬ �ʱ�ȭ
int InitParticle();
//��ƼŬ �߰�
int SetParticle( int x, int y, int z , int MatNum , int Type , int Param=0 );
//��ƼŬ ����
int PlayParticle();
//��ƼŬ �����
int DrawParticle(int x, int y, int z, int ax, int ay, int az );


//���� ���� �ʱ�ȭ
int InitDynLight();
//���� ���� �ݱ�
int CloseDynLight();
//���� ���� ����
int SetDynLight( int x,int y,int z, int r, int g, int b, int a, int power , int DecPower=10 );
//���� ���� �������� ����
int DynLightApply();
//���� ���� ����
int DynLightMain();
