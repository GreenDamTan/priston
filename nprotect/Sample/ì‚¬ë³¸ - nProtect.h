class nP
{
public:

	nP();
	~nP();

	int rn;			// ���ϰ�
	int temp;
	int result;		// nProtect����� ���ܰ��� �˷��ش�
	char nperrcode[10];		// nProtect ����� ���ܰ��� �־��ִ� �ӽ� ����
	//char *test;			// ȭ�ϸ��� �� �ӽ� ������ �Լ�
	HINSTANCE m_hInstDll;	// DLL���̺귯���� �ν��Ͻ�

	char test[6][20];

	bool (*pFunc_npchk)(char*);
	int (*pFunc_npkill)(char*);

public:



	int NProtectCheck(void);
	//int NProtectCheckMON(void);
	int NProtectRun(void);
	int NProtectFree(void);
	int findNPMON();
	void Init();
};

extern class nP nPro;
