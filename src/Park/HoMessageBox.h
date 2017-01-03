#ifndef _HoMessageBox_H_
#define _HoMessageBox_H_

#define	BUTTON_STATE_NONE	0
#define BUTTON_STATE_IN		1
#define BUTTON_STATE_DOWN	2


#define BOX_ONE				0
#define BOX_TWO				1
#define BOX_THREE			2
#include <string>
using namespace std;

int HoDrawTexImage(int Mat, float x, float y, float w, float h, float texX, float texY, float texW, float texH, int Transp);
int HoDrawTexImage(int Mat , int x, int y, int texX, int texY, int texW, int texH, int Transp);

//�� �׵θ��� �޽����� ����ϴ� Class
class HoMessage
{
private:
	int						BoxImage1[9];
	int						BoxImage2[4];
	int						BoxImage3[10];
	int                     BoxImage4[10];
    
	int						ClanBoxImage[3];
	int                     ClanBoxImage2[9];

	int						ShopBoxSellImage1[10];
	int						ShopBoxSellImage2[10];

	int						ShopBoxByeImage1[10];
	int						ShopBoxByeImage2[10];

	int						BellaIconImage[3];
	

	//�� �Ʒ� �������� �ǵ��� ���ÿ�
	int						Width;
	int						TextStartPosY;
	static BOOL				MessageFrameSelectFlag;   //�޽����� ���� ���� ��� Frame�� �� �׸����� �ƴ���..

public:
	HDC						TextDC;
	HFONT					OldFont;

public:
	int						PosX, PosY;
	char					DrawMessageBuffer[256];
	char					DrawCharacterNameBuffer[256];

	HoMessage();
	~HoMessage();
	int Init();
	
	//message1�� message2�� �Ʒ�,���� �׷����� �޽���â
	void DrawTwoLineMessage(int x, int y, char *message1, char *message2, DWORD color1 = RGB(0,0,0), DWORD color2 = RGB(0,0,0), DIRECTDRAWSURFACE clanMark = NULL, BOOL selected = FALSE, int bellaMarkIndex = -1);
	
	//�Ϲ����� ä�� �޽��� â(message�� ������ ����̸����� :�� ����̸� �����ؼ� ����̸��� color1������ ��� ������ ���ڴ� ����Ʈ ������ ĥ�Ѵ�.)
	void DrawCharacterMessage(int x, int y, char *message, int lineSize = 30, DIRECTDRAWSURFACE clanMark = NULL, char *clanName = NULL, DWORD color1 = RGB(0,0,0), BOOL selected = FALSE, int bellaMarkIndex = -1);
	//�Ϲ����� ä�� �޽����� ����� �� �����⸦ ��´�.
	void DrawCharacterMessageFrame(int x, int y, int messageWidth, int messageHeight, bool tailFlag = false, BOOL selected = FALSE); 
	//ä�� �޽������� Ŭ�� �޽����� �ִ� ��� Ŭ�� �޽����� ��� �Լ�
	void DrawClanMessage(int x, int y, char *message, int lineSize, DWORD color = RGB(255,255,255));
	//Ŭ�� �޽��� �޽���â �̹���
	void DrawClanMessageFrame(int x, int y, int messageWidth, int messageHeight);
	//Ŭ�� ��ũ
	void DrawClanMark(DIRECTDRAWSURFACE clanMark, char *clanName, int x, int y, BOOL frameFlag = FALSE);
	//Ŭ�� �̸� 
	void DrawClanName(int x, int y, char *clanName);
	//���� Ʈ�� ��ũ ��� �Լ�.
	void DrawBellaIcon(int x, int y, int bellaIndex = 1);
	

	//���� ���� �춧 �޽���
	void DrawShopSellMessage(int x, int y, char *message, int lineSize = 30, DIRECTDRAWSURFACE clanMark = NULL,DWORD color1 = RGB(0,0,0), BOOL selected = FALSE);
	void DrawShopSellMessageFrame(int x, int y, int messageWidth, int messageHeight, bool tailFlag = false, BOOL selected = FALSE);
	
	//���� ���� �ȶ� �޽���
	void DrawShopByeMessage(int x, int y, char *message, int lineSize = 30, DIRECTDRAWSURFACE clanMark = NULL,DWORD color1 = RGB(0,0,0), BOOL selected = FALSE);
	void DrawShopByeMessageFrame(int x, int y, int messageWidth,  int messageHeight, bool tailFlag = false, BOOL selected = FALSE);
	
	
	

	//�� �Ʒ� �Լ����� �ǵ��� ������
	int GetWidth()			{return Width;}
	static void SetMessageFrameSelect(BOOL flag = TRUE)
	{
		MessageFrameSelectFlag = flag;
	}
	void Draw(int x, int y, char *message, int lineSize = 0, int addLine = 0, int boxAttrib = BOX_ONE);
	void DrawMessageBox(int x, int y, char *message, int boxAttrib = BOX_ONE, int Color = RGB(255, 255, 255));

	//�ؿ� �Լ�����κ�
	char	textdata[12][64];
	POINT	textsize;
	int textarrange(char *teststr,bool clanm);
	void DrawCharacterMessageFrame1(int x, int y, int messageWidth, int messageHeight,int linecount, bool tailFlag, BOOL selected);
	void DrawShopSellMessageFrame1(int x, int y, int messageWidth, int messageHeight,int linecount, bool tailFlag,  BOOL selected);
	void DrawShopByeMessageFrame1(int x, int y, int messageWidth, int messageHeight,int linecount, bool tailFlag, BOOL selected);
};








//��ư �Ѱ��� ����ϴ� Class
//3��ư (�̹��� �ΰ�) ���콺�� ��ư���� ���� ��� �����ϰ� ������ ��ư�� �Ʒ��� ���� �����δ�.
class HoButton
{
private:
	int				StateFirstImage;
	int				StateThirdImage;
	
	BOOL			ButtonInDownFlag;			//Button �ȿ��� MouseDown ����..
	
	static	BOOL	ButtonUseFlag;

	int				State;						//Button�� ����...
	int				PosX;
	int				PosY;

public:
	BOOL Main(int mouseX, int mouseY,int downFlag = 0);
	BOOL Draw(int x, int y, int BlendFlag = 255);
	void Init(char *stateFirstImage, char *stateThirdImage = NULL);
	void Init(char *stateFirstImage, int sizeWidth, int sizeHeight);
	
	void InitCrashRect(int left, int top, int right, int bottom);
	
	int				SizeWidth;
	int				SizeHeight;

	RECT			CrashRect;		//�׸��� �浹 ���� �˻� (��� ��ǥ�̴�)

	HoButton();
	~HoButton();

};

class HoMessageBox
{
private:
	HoMessage	Message;		//Message���..
	HoButton	Button[2];		//Button�� ���...
	
public:
	BOOL		Flag;
	HoMessageBox();
	~HoMessageBox();

	BOOL	Main(int mouseX, int mouseY, int downFlag = 0);
	BOOL	Draw(int x, int y, char *message, int Color = RGB(255, 255, 255));
	void	Init();

};

extern HoMessage	ChatMessage;

BOOL	TextGetDC();
BOOL	TextReleaseDC();
BOOL	TextSelectObject(HFONT font);
BOOL    TextSetColor(COLORREF color);
BOOL	dsTextLineOut(int x, int y, char *lpszString, int nCount);


BOOL	EngSetFont(int fontSize);
BOOL	EngSetTextColor(COLORREF color);
BOOL	EngdsTextLineOut(int x, int y, char *message, int nCount);

void    SetMessageFrameSelect(BOOL flag = TRUE);

//#define dsTextLineOut_ENGLISH
#endif