
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
