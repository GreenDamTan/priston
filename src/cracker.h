
extern DWORD	dwExeCheckSum;		//�������� üũ��
extern char		szProcessPath[256];	//�������� ���

//ũ���� ã�Ƽ� �̸��� ��ȯ�Ѵ� (�������� )
char *FindCracker();
//ũ���� ã�� �̸� ��ȯ ( �¶��� )
char *FindCracker( TRANS_FIND_CRACKER *lpTransFindCrack );
//ũ���� �ٽ� ã�´� ( �¶��� )
char *FindCrackCont();
//������ ��Ŀ���� ����
HWND WinFocusCrack();
// �������� ���μ����� ����� ���Ѵ�.
int CheckCrackProcess( int Flag=0 );

//�ش� ����� �˻��Ͽ� ýũ�� ��ȯ
DWORD	GetModuleCheckSum( char *szModule );

int	CheckDLL_Module();		//���μ��� ��� �˻�


//nProtect ����
int Start_nProtect( int deMode );
//nProtect HWND
int SetHwnd_nProtect( HWND hWnd );
//nProtect ����
int Remove_nProtect();
//nProtect Ȯ��
int	Check_nProtect();
//nProtect Ȯ��
int Check_nProtectDLL();

//����� ID ���� ���Ӱ��� �뺸
int	npSetUserID( char *szID );

//���Ӱ��� ������ ýũ
int npAuth( DWORD dwAuthValue );

// ���ӿ��� NPMON.EXE�� �����ִ� �ڵ����� �޽��� (��������� ����)
#define WM_NPROTECT_FORCEEXIT		(WM_USER + 10010)

//////////////////////Npmon.exe�� �������� ������ �޽���/////////////////////////////////
#define WM_NPROTECT_EXIT_TWO		(WM_USER + 10001)

//- ������ ���ǵ����� ����� ��� (������(SPEEDER)�� ���� ���ܰ���)
//������ ĸ�Ǹ��� �����Ͽ� ������ �޽��� ����. (npmon 4.10.0.35 ���� �����)
#define WM_NPROTECT_SPEEDHACK		(WM_USER + 10011) //����ڿ��� �������� �޽��� : ���ǵ����� �߰ߵǾ����ϴ�.
#define WM_NPROTECT_SH_ERROR		(WM_USER + 10012) //������ �������� �α����Ͽ� ������ �����Ͻñ� �ٶ��ϴ�.

//- ������ ����Ʈ ���̽��� ����� ���
//������ ĸ�Ǹ��� �����Ͽ� ������ �޽��� ����. (npmon 4.10.0.35 ���� �����)
#define WM_NPROTECT_SOFTICE			(WM_USER + 10111) //����Ʈ ���̽��� �߰� �Ǿ����ϴ�.
///////////////////////////////////////////////////////////////////////////////////////////////






