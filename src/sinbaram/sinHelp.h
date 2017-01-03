/*----------------------------------------------------------------------------*
*	���ϸ� :  sinHelp.h	
*	�ϴ��� :  �Źٶ� �����
*	�ۼ��� :  ����������Ʈ 2002�� 7��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
 
#define SIN_MAX_HELP_NUMBER		10		//�������� ���̽� �ִ� ���� 

#define SIN_HELP_KIND_NORMAL		1		//�⺻ ����
#define SIN_HELP_KIND_QUEST			2		//����Ʈ ���� ����
#define SIN_HELP_KIND_SMASHTV		3		//���Ž� TV�̺�Ʈ 
#define SIN_HELP_KIND_POSTBOX		4		//PostBox
#define SIN_HELP_KIND_SOD			5		//PostBox
#define SIN_HELP_KIND_QUEST_WING	6		//�� ����Ʈ�� �����Ѵ�
#define SIN_HELP_KIND_WARPGATE		7		//��������Ʈ ������ �����ش�
#define SIN_HELP_KIND_INITPOINT		8		//�հ� �ʱ�ȭ�ϴ°� ����
#define SIN_HELP_KIND_BABEL			9		//�ٺ� �̺�Ʈ
#define SIN_HELP_KIND_Q_LEVEL30		10		//��������Ʈ30
#define SIN_HELP_KIND_Q_LEVEL55		11		//��������Ʈ55
#define SIN_HELP_KIND_Q_LEVEL55_2	12		//��������Ʈ55 ����̿�
#define SIN_HELP_KIND_Q_LEVEL70		13		//��������Ʈ70
#define SIN_HELP_KIND_Q_LEVEL80		14		//��������Ʈ80
#define SIN_HELP_KIND_Q_LEVEL85		15		//��������Ʈ85
#define SIN_HELP_KIND_Q_LEVEL90		16		//��������Ʈ90
#define SIN_HELP_KIND_TELEPORT		17		//�ڷ���Ʈ 
#define SIN_HELP_KIND_Q_LEVEL80_2	18		//��������Ʈ80_2
#define SIN_HELP_KIND_Q_LEVEL90_2	19		//��������Ʈ80_2
#define SIN_HELP_KIND_C_TELEPORT    20      //���� ĳ�� �ڷ���Ʈ 
#define SIN_HELP_KIND_RESEARCH      21      //��������
#define SIN_HELP_KIND_TELEPORT_SCROLL   22      //�ڷ���Ʈ �ھ�
#define SIN_HELP_KIND_TELEPORT_MILTER	23	// pluto ���� ����





#define SIN_HELP_COLOR_RED		1		//����ȭ�� ��
#define SIN_HELP_COLOR_YELLOW	2
#define SIN_HELP_COLOR_GREEN	3
#define SIN_HELP_COLOR_BLUE		4

///////TONY �׸��׽�Ʈ ���� 
#define TONY_TGA				1
#define TONY_BMP				2


/////// ���� �޼��� 
#define SIN_MESSAGE_DEFAULT		98
#define SIN_HELP				99	
#define SIN_CHANGEJOB_MESSAGE	100
#define SIN_EVENT_SMASHTV		101
#define SIN_POST_BOX			102
#define SIN_SOD					103



/*----------------------------------------------------------------------------*
*						�Źٶ� ����� struct
*-----------------------------------------------------------------------------*/	
struct sSINHELP_BOX{
	int Mat;
	float x,  y,  w , h;
	float surW,  surH;
	float tLeft,  tTop,  tRight,  tBottom;
	int Transp;

};

struct sSINHELP{
	int KindFlag;
	int PosiX , PosiY;
	int SizeW , SizeH;
	DWORD BackColor;
	sSINHELP_BOX *sHelp_Box[20][20];
	int MatFlag[20][20];

	///////// Doc ���� 
	int LineCnt;
	char *szDocBuff[100];

	///////// ��ũ�� 
	int		ScrollFlag;
	POINT	ScrollButton;
	POINT	ScrollMousePosi;
	int		ScrollMAXPosiY_UP;
	int		ScrollMAXPosiY_DOWN;
	int		ScrollMouseDownFlag;

	///////// ��ư 
	POINT	TitlePosi;
	int		ButtonOkFlag;
	POINT	ButtonPosi;
	int		ButtonShowFlag;

	//////// CODE
	int    Code; //�ߺ����� �����°� �����Ѵ�

};
/*----------------------------------------------------------------------------*
*						Tony�� Test �̹���
*-----------------------------------------------------------------------------*/	
struct sTONY_TEST_IMAGE{
	int Flag;
	char ImageFileName[64];
	POINT	Posi;
	POINT	Size;
	DWORD   BackColor;
	POINT	BackPosi;
	POINT	BackSize;
	int		FileFormat;

	int		Mat;
	DIRECTDRAWSURFACE lpTony;

};

struct sSOD_RANK{
	int		Flag;
	int		Job;
	char	Name[64];
	int		Score;
	int		KillNum;

};
/*----------------------------------------------------------------------------*
*						���� ���� ����ü
*-----------------------------------------------------------------------------*/	
struct shaResearch{
	int   Result[10];
	int   Index;
	int   Value[10];
	int   Number;
	BYTE  bSendPollingData[10];
	POINT ButtonPosi;
	int   ButtonShowFlag;
};
/*----------------------------------------------------------------------------*
*						�Źٶ� ����� class
*-----------------------------------------------------------------------------*/	
class cSINHELP{

public:
	int MatHelpCorner;
	int MatHelpLine;

	int MatScrollBar;
	int MatScrollButton;

	DIRECTDRAWSURFACE	lpHelpTitle;
	DIRECTDRAWSURFACE	lpChangeJobTitle;
	DIRECTDRAWSURFACE	lpHelpOk;
	DIRECTDRAWSURFACE	lpSodMenu;
	DIRECTDRAWSURFACE	lpSodLine;
	DIRECTDRAWSURFACE	lpSodJobFace[8];

	

public:
	int                     ScrollLineCount;  //����â�� ��ũ���� �����ҽÿ� ���μ�
	int                     NPCTeleportFlag;
	
public:

	cSINHELP();
	~cSINHELP();

	void Init(); //Ŭ���� �ʱ�ȭ
	void Load();
	void Release();
	void Draw();	
	void DrawText();
	void Close();//Ŭ���� ���� 
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); 

	//������ �����ش� 
	int sinShowHelp(int Kind , int PosiX , int PosiY , int SizeW , int SizeH , DWORD BackColor,char *File);
	//������ �д´� 
	int sinReadFileData(char *name);

	//���� ������ ����Ÿ�� ���ۿ� �����Ѵ� 
	int sinReadFileDataDivide(char *SaveBuf , int Linelen ,int Size);

	////////TONY�� �׽�Ʈ ���� 
	int TonyGetFileData();
	
	////////�ٰ� Ű�Է����� ��ũ�����Ѵ�
	int sinGetScrollMove(int Num);
	int sinGetKeyScrollDown();

	//������ ���ִ����� �˾ƿ´� 
	int sinGetHelpShowFlag();

	//SOD ��ŷ�� ���� 
	int ShowSodRanking(TRANS_SOD_RESULT *SodResult ,int MyGame=0);

	//Sod ��ŷâ�� ���ִ��� ã�´�
	int GetSodRankBoxState();

	//*haGoon
	//���� ����
	int ShowResearchMenu(int Kind,int Flag);

	//������ ������
	int SendResearch();

	// pluto ���� �ڷ���Ʈ �ھ� ����
	int sinGetHelpShowFlagNum();
};
int TeleportDungeon();	// pluto �߰��������� ����
int haShowCastleTeleportMap();
int sinShowTeleportMap();
int sinShowHelp();
int sinDrawTexImage( int Mat, float x, float y, float w, float h, float surW=0.0f, float surH=0.0f, float tLeft=0.0f, float tTop=0.0f, float tRight=0.0f, float tBottom=0.0f, int Transp=255);
extern sSINHELP sSinHelp[SIN_MAX_HELP_NUMBER];
extern cSINHELP cSinHelp;
extern int HelpBoxKindIndex;
extern int WingItemQuestIndex;
extern int MatHelpBoxImage[10];
extern int sinTeleportIndex;
extern int sinTeleportMoneyIndex;
extern int TeleportUseCose[];
extern int haC_TeleportMoney;
extern char *haC_Not_TeleportFilePath;
extern int haTeleport_PageIndex;


