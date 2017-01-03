/*----------------------------------------------------------------------------*
*	���ϸ� :  sinParticle.h	
*	�ϴ��� :  ����Ʈ ���� �۾� 
*	�ۼ��� :  ����������Ʈ 2002�� 6��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/

#define SIN_MAX_INTER_EFFECT		300

#define SIN_INTER_EFFECT_AGING				1		//����¡�� �ͽ����� ����Ʈ 
#define SIN_INTER_EFFECT_AGING2				2		//����¡�� �ͽ����� ����Ʈ 
#define SIN_INTER_EFFECT_AGING_SUCCESS		3		//��������Ʈ 
/*----------------------------------------------------------------------------*
*						   �������̽� ����Ʈ ����ü 
*-----------------------------------------------------------------------------*/	
struct sINTER_EFFECT{
	smFACE2D Face;
	int Time;
	int Max_Time;
	POINT Speed;
	POINT Speed2;		//�� ��ġ�� �� ���� ¥��!
	int Direction;
	int AniCount;
	int AniKeyFlag;		//������ ����ġ���� �̸��̷δ� 
	int AniKeyFlag2;	//��ȣ! �̰Ŷ��� ����ġ �ʱ��� �󾾱� ��ȭ��
	int FadeType;
	int Kind;			//����Ʈ ����
	int Gravity;		//�߷� 

	///// ������ ���� 

};
/*----------------------------------------------------------------------------*
*						   ��ġ�� ����ü 
*-----------------------------------------------------------------------------*/	
struct sINTER_EFFECT3{
	DIRECTDRAWSURFACE lpSuccessEffectAging;
	int Flag;
	int x;
	int y;

	int w;
	int h;

	int MoveX;
	int MoveY;

	int MoveX2;
	int MoveY2;

	int Speed;


};


/*----------------------------------------------------------------------------*
*						          �Լ� 
*-----------------------------------------------------------------------------*/	
int sinSetParticle(int Kind ,int Color ,  int x , int y, int z);


/*----------------------------------------------------------------------------*
*						      ���� ���̽� ��ƼŬ    
*-----------------------------------------------------------------------------*/	
 //�̹����� �ε��Ѵ� 
int InitInterEffect();

//����¡ ����Ʈ�� �����Ѵ�
int SetInterEffectAging();

//����¡ ����Ʈ ��!!
int SetInterEffectAging2();

//�������̽� ����Ʈ�� �����δ� 
int sinActiveInterEffect();

//�������̽� ����Ʈ�� �׸��� 
int sinDrawInterEffect();

//�������̽� ����Ʈ ���μ��� �� ��ġ�� �������� 
int InterEffectAging2Process();

//�ͽ�, ����¡ ���� ����Ʈ 
int sinSuccessCraftEffect();

//�ͽ�, ����¡ ���� ����Ʈ 2
int sinSuccessCraftEffect2();

//��������Ʈ 2�� �����δ� 
int MoveSuccessCraftEffect2();

//���� ����Ʈ 2�� �׸��� 
int DrawSuccessCraftEffect2();

//���� ����Ʈ
int SetPuzzleEffect(int x , int y);

// pluto ���� ����Ʈ�� �����Ѵ�
int SetInterEffectSmelting();

// pluto ���� ����Ʈ�� �����Ѵ�
int SetInterEffectManufacture();

int SetInterEffectMixtureReset();	// ������ - �ͽ��� ���� â�� ù���� ����Ʈ ����

/*----------------------------------------------------------------------------*
*						          Extern 
*-----------------------------------------------------------------------------*/	
extern int   StartInterEffectAging2; //����Ʈ�� ������ �˸��� 
extern int   sinCraftItemStartFlag;
extern int   sinCraftItemStartFlag2;


extern int  sinAgingSuccessFlag2;    //����¡�� �ͽ����� ���� �÷� 
extern DWORD InitCancelMixTime;