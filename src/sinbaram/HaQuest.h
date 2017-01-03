/*----------------------------------------------------------------------------*
*	���ϸ� :  haQuest.h	
*	�ϴ��� :  �ű� �ʺ� ����Ʈ 
*	�ۼ��� :  ����������Ʈ 2005�� 5��
*	������ :  �ϴ�� 
*-----------------------------------------------------------------------------*/	

//elementary

//Quest Bit Code
#define QUESTBIT_ELEMENTARY_A			1       //�ǰ��� â������
#define QUESTBIT_ELEMENTARY_B			2       //�ηÿ� ���
#define QUESTBIT_ELEMENTARY_C			4       //���� ���
#define QUESTBIT_ELEMENTARY_D			8       //��� ���� 
#define QUESTBIT_ELEMENTARY_E			16      //�𵥵��� ����
#define QUESTBIT_ELEMENTARY_F			32      //�ܴ����� ����
#define QUESTBIT_ELEMENTARY_G			64      //�Ҿ���� ��ġ
//ǻ���� ȯ��(Fury of phantom)
#define QUESTBIT_FURYOFPHANTOM          128      //ǻ���� ȯ��

/////����Ʈ�� ����
#define HAQUEST_CODE_ELEMENTARY_A		0x0020
#define HAQUEST_CODE_ELEMENTARY_B	    0x0021
#define HAQUEST_CODE_ELEMENTARY_C	    0x0022
#define HAQUEST_CODE_ELEMENTARY_D   	0x0023
#define HAQUEST_CODE_ELEMENTARY_E   	0x0024
#define HAQUEST_CODE_ELEMENTARY_F		0x0025
#define HAQUEST_CODE_ELEMENTARY_G		0x0026
#define HAQUEST_CODE_ELEMENTARY_X		0x0027
//ǻ���� ȯ��(Fury of phantom)
#define HAQUEST_CODE_FURYOFPHANTOM      0x0028              
//
enum eQuestKind{
	Quest_A,
	Quest_B,
	Quest_C,
	Quest_D,
	Quest_E,
	Quest_F,
	Quest_G,
};
/*----------------------------------------------------------------------------*
*						����ü
*-----------------------------------------------------------------------------*/	
struct sQUEST_ELEMENTARY{ 
	DWORD CODE;
	DWORD BackUpCode;
	int   Kind;
	int   State;        //���� Count
	short Monster[2];   //���� ī��Ʈ
};
/*----------------------------------------------------------------------------*
*						Ŭ����
*-----------------------------------------------------------------------------*/
class cHaQuest{
private:
	char szBuff[128];
public:
	//
	sQUEST_ELEMENTARY  sHaQuestElementary[8];	
	char szQuestMonsterName[64];
	int iJob_Index;
	int QuestKeyCode;
	int KindClanIndex;
	//
	cHaQuest();
	~cHaQuest();	
	//
	int HaCheckQuestElementary(int npcNum,int QuestKey=0,int KindClan=0);
	int HaLoadQuest(int CODE ,void *Buff );	//����Ʈ ����Ÿ�� �ε��Ѵ� 
	int HaSaveQuest(void *Buff );		    //����Ʈ ����Ÿ�� ���̺��Ѵ� 
	//
	int haSetQuestTimer(sQUEST_ELEMENTARY *pQuest);
	int haQuestCheck();  //���� �ʺ�  ����Ʈ�� ����������	
	//�ʺ� ���� ����Ʈ
	int haElementaryQuest_A(int npcNum=0);  //�ǰ��� â������
	int haElementaryQuest_B();			    //�η��� ��� 
	int haElementaryQuest_C(int npcNum=0);	//���� ���
	int haElementaryQuest_D(int npcNum=0);	//��� ����
	int haElementaryQuest_E(int npcNum=0);	//�𵥵��� ����
	int haElementaryQuest_F();				//�ܴ����� ����
	int haElementaryQuest_G(int npcNum=0);	//�Ҿ���� ��ġ
//ǻ���� ȯ��(Fury of phantom)
public:
	//
	sQUEST_ELEMENTARY        sHaQuest100LV;	
	bool                     bFuryQuestFlag;
	int                      iFuryQuestIndex[2]; 
	char                     cItemName[32];
	bool                     FuryBattleFlag;
	//������ �̹���
	DIRECTDRAWSURFACE     lpQuestItemImage[8];
	DIRECTDRAWSURFACE     lpQuestButtonImage[4];


	//�ʿ��Ѱ͸� �׽�Ʈ
	void init();
	void Main();
	void Draw();
	void KeyDown(); //Ű üũ
	void LButtonDown(int x , int y);
	void LButtonUp(int x,int y);
	void DrawText();
	void ImageLoad();
	//
	int  showFuryQuest();                  //100LV ����Ʈ ȣ��
	int  showFuryQuestItem(int Kind=0);    //���� ������ 
	int  RecvBattleNpcResult(int flag);    //ǻ������ ���� ��� ��
	int  Recv100LVItem();                  //����������� �޴µ�.

	

};
/*----------------------------------------------------------------------------*
*						extern 
*-----------------------------------------------------------------------------*/	
extern cHaQuest chaQuest;
extern DWORD haElementaryQuestLog;
extern void HaCheckQuestNpc(int NpcCount);
