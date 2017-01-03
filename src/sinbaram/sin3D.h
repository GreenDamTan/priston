/*----------------------------------------------------------------------------*
*	���ϸ� :  sin3D.h	
*	�ϴ��� :  3D���� �۾��� �Ѵ� 
*	�ۼ��� :  ����������Ʈ 2002�� 1��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

////////////////////DEFINE
#define SIN_EFFECT_RAGE_OF_SINBARAM			100	//������ ���� �Źٶ� 


#define SIN_RAND_1000000  (rand()%1000000) 
#define SIN_RAND_100000   (rand()%100000) 
#define SIN_RAND_10000    (rand()%10000) 
#define SIN_RAND_1000     (rand()%1000) 
#define SIN_RAND_100      (rand()%100) 
#define SIN_RAND_10       (rand()%10) 


/////////////////////�Źٶ� 3D
class cSIN3D{

public:
	int sinEffect2D_Line;		//��ƼŬ ��Ʈ���� 
	int MatParticleRed[4];		//��ƼŬ ����
	int MatParticleBlue[4];		//��ƼŬ ���
	int MatParticleGreen[4];	//��ƼŬ �׸�
	int	MatParticleGold[4];		//��ƼŬ �ݰ��� 
	int	MatEffectLine;			//������ ����Ʈ 

	int MatParticle1;			//�׳� ��ƼŬ 
	

	////////////2D����Ʈ 
	int	MatWideCircle;			//�а� �����¿� 
	

	////////////�𵨸� ����Ÿ 
	smPAT3D     *Rage_Of_SinBaRam;


public:

	cSIN3D();
	~cSIN3D();

	void Init(); //Ŭ���� �ʱ�ȭ
	void Load();
	void Release();
	void Draw(int x, int y,int z,int ax, int ay, int az);		//ȣ����Ʈ �׸���);
	void DrawText();
	void Close();//Ŭ���� ���� 
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); 

	//////////////////////////3D���� �Լ�
	//����Ʈ �׸��� 
	int sinDrawTexture(int x ,int y, int z , int AngleX, int AngleY ,int AngleZ , smFACE2D *lpFace,int sinAngle);

};

extern cSIN3D cSin3D;