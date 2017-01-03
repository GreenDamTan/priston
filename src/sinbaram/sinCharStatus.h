/*----------------------------------------------------------------------------*
*	���ϸ� :  sinCharStatus.h	
*	�ϴ��� :  ĳ���� �������ͽ��� ��缳���� �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#define MAX_CHAR_RECT_POSI			30  //ĳ���� �������ͽ����� ���� RECT�� MAX 

#define POINT_STRENGTH				1
#define POINT_SPIRIT				2
#define POINT_TALENT				3
#define POINT_DEXTERITY				4
#define POINT_HEALTH				5


#define SIN_TEXT_COLOR_WHITE		0
#define SIN_TEXT_COLOR_RED			1
#define SIN_TEXT_COLOR_YELLOW		2
#define SIN_TEXT_COLOR_BLUE			3
#define SIN_TEXT_COLOR_GREEN		4
#define SIN_TEXT_COLOR_GOLD			5
#define SIN_TEXT_COLOR_ORANGE		6
#define SIN_TEXT_COLOR_PINK			7

/*----------------------------------------------------------------------------*
*				(ĳ���� �������ͽ�)ĳ���� ���� ����ü
*-----------------------------------------------------------------------------*/
struct sCHARRECTPOSI{

	char Para[64];
	RECT Rect;

};

struct sDISPLAYSTATE{
	short Damage[2];
	short PercentDamage[2];
	int   Absorb;
	int   Attack_Rate;
	int   Defense;
	DWORD   MaxTime;
	DWORD   Time;
	int   Flag;
};
/*----------------------------------------------------------------------------*
*				(ĳ���� �������ͽ�)class
*-----------------------------------------------------------------------------*/	
class cCHARSTATUS{

public:
	int OpenFlag;       //������ �� �����ϱ� ������ ��ġ�� ����� �ٲ�� �ȵȴ� 
	int MatStatus[4];   //ĳ���� �������ͽ� ��Ʈ���� �ѹ��� �����Ѵ� 

	DIRECTDRAWSURFACE    lpRegiBox[5];
	DIRECTDRAWSURFACE    lpChangeArrow[2];
	DIRECTDRAWSURFACE    lpSelectArrow[2];
	DIRECTDRAWSURFACE	lpStatusPoint;


public:
	cCHARSTATUS();
	~cCHARSTATUS();

	void Init();
	void Load();
	void Release();
	void Close();
	void Draw();
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); 
	
	void OtherStateShow();	//ĳ���ͽ����̽��ͽ��� �������ʴ� ���ڸ� �����ش� 
	
	void DrawCharText();	//ĳ���� �ɷ�ġ�� �����ش� 

	void CheckLevel();		//������ üũ�Ѵ� 

	void BackUpsinCharState(smCHAR *pChar); //ĳ���� �ɷ�ġ�� ����Ѵ� (��ŷ������ ����)
	
	int sinGetNowExp();		//������ ����ġ�� ���´� 
	
	int sinGetNextExp();	//���� ������ ����ġ�� ���´� 

	int InitCharStatus(int kind=0);   //ĳ���� ���� �ʱ�ȭ

	int InitCharStatus_Attribute(int kind);   // ����� - ĳ���� �Ӽ��� ���� �ʱ�ȭ ������

	int UseGravityScrool(); // �庰 - �׶��Ƽ ��ũ��
	
	int CheckChageStateDisplay(); //ĳ������ ���� ���¸� üũ�� Stats�� ���÷������ش�
	
};

extern cCHARSTATUS cCharStatus;

extern int sinMoveKindInter[MAX_SIN_KIND_INTER]; //���� ���� �����̴� �� 

extern int OldLevel;		//������ �����Ѵ� 
extern int sinLevelPoint;	//���� ����Ʈ�� �����Ѵ� 
extern int OldLevelTemp;	//������ �����Ѵ� �� �� �����ϴ� (��? ���۳ѵ��� ��ġ�����ϵ���)

extern  int TempNewCharacterInit[5][6];
extern  int MorNewCharacterInit[5][6];
extern  sDISPLAYSTATE sDisplayState[10];

extern  int ChatKindIndexNum;
extern  int NoticeChatIndexNum;
extern  int NoticeChatIndex;
/*�ܺ� extern */
extern INT64 ExpLevelTable[]; //����ġ ���̺� 


