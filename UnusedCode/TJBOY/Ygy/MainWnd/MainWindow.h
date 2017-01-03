/*****************************************************************************************************
*
*
******************************************************************************************************/

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#define CLASSNAME "Voice Client"
#define WINDOWNAME "���� �ùķ��̼�"





#define WM_CALLVCMSG		WM_USER+2   //�Ϲ� �޼�������
#define WM_CALLMEMMAP		WM_USER+3   //�޸𸮸ʿ� �޼����� �ִٰ� �뺸

typedef struct __tagCallData
{
	DWORD  dwIndex;
	char   szCallData[256];
}CALLDATA,*LPCALLDATA;

class CMainWindow
{
public:
	CMainWindow();
	~CMainWindow();
public:
	VOID		Init();
	VOID		Clear();
	BOOL		InitClass(HINSTANCE hInst);
	BOOL		InitWindow(HINSTANCE hInst);
	BOOL		Create(HINSTANCE hInst);
	int			Run();
	int         Process();
	VOID        Draw_Text(HDC hdc,char *msg);
	LRESULT		MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	inline      VOID Invalid(){InvalidateRect(m_hMainWnd,NULL,1);}
private:
	HWND		m_hMainWnd;
	BOOL		m_bActive; //�����찡 Ȱ��ȭ �Ǿ����� üũ	
	int			g_Process;
	HDC         m_hDC;
	
};
#endif

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef MAINWINDOW_CPP
class CMainWindow g_MainWindow;
#else
extern class CMainWindow g_MainWindow;
#endif


#define SERVER_OUT   0