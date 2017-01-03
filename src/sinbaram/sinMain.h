/*----------------------------------------------------------------------------*
*	���ϸ� :  sinMain.h
*	�ϴ��� :  �Źٶ� ���� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#define		 SINLBUTTONDOWN			10
#define		 SINLBUTTONUP 			11
#define		 SINRBUTTONDOWN			110
#define		 SINRBUTTONUP 			111

#define		 SINMOUSEMOVE			2
#define		 SINKEYDOWN				3

#define		 SIN_CURSOR_DEFAULT		1
#define		 SIN_CURSOR_SELL		2
#define		 SIN_CURSOR_BUY			3
#define		 SIN_CURSOR_REPAIR		4
#define		 SIN_CURSOR_ALLREPAIR	5
#define		 SIN_CURSOR_ATTACK		6
#define		 SIN_CURSOR_GETITEM1	7
#define		 SIN_CURSOR_GETITEM2	8
#define		 SIN_CURSOR_TALK		9



/*----------------------------------------------------------------------------*
*							�Լ� 

*-----------------------------------------------------------------------------*/	

void sinInit();

void sinMain();

void sinDraw();

void sinDrawText();


void sinClose();

void sinProc(int Message);


/*----------------------------------------------------------------------------*
*							   class 
*-----------------------------------------------------------------------------*/	
class cSINBARAM_UI{ //Ŭ������ �⺻�� �ȴ�

public:
	int OpenFlag;       //������ �� �����ϱ� ������ ��ġ�� ����� �ٲ�� �ȵȴ� 
	
public:
	cSINBARAM_UI();
	~cSINBARAM_UI();

	void Init(); //Ŭ���� �ʱ�ȭ
	void Load();
	void Release();
	void Draw();
	void Close();//Ŭ���� ���� 
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); //Ű üũ 
};
/*----------------------------------------------------------------------------*
*							extern ���� 
*-----------------------------------------------------------------------------*/	
extern int	 quit;
extern int	 MouseButton[3];	//���콺 �迭 
extern BYTE  VRKeyBuff[256];    //Ű�Է� ����  
extern POINT pCursorPos;		//���콺 ��ǥ 	
extern POINT pRealCursorPos;    //Ȯ�� ���콺 ��ǥ

extern HWND  hFocusWnd; //ä�� ��Ŀ�� 

extern int WinSizeX;
extern int WinSizeY;
extern HWND hwnd;

extern smCHAR_INFO *sinChar;		//ĳ���� ���� ����ü 
extern smCHAR_INFO sinCharDisplay; //ĳ���� ���� ����ü (��ŷ������ ���)
extern int sinbaram_Stop;