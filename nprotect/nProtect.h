class nP
{
public:

	nP();
	~nP();

	int rn;			// ���ϰ�
	int temp;
	int result;		// nProtect����� ���ܰ��� �˷��ش�
	char nperrcode[10];		// nProtect ����� ���ܰ��� �־��ִ� �ӽ� ����
	HINSTANCE m_hInstDll;	// DLL���̺귯���� �ν��Ͻ�

	char test[6][20];

	bool (*pFunc_npchk)(char*);
	int (*pFunc_npkill)(char*);
	
	typedef BOOL(MYCHECKCRC)(char *);
	MYCHECKCRC *pCheckCRC;

public:

	int NProtectCheck(void);
	int NProtectRun(void);
	int NProtectFree(void);
	int findNPMON();
	BOOL CheckCRC(char * FilePath);
	void Init();

// nProtect GameHack SDK //
	bool m_bStart;
	bool IsNT;
	
	HINSTANCE hInstanceNPFSICE; // ����Ʈ ���̽� �˻��� ���Ǵ� DLL Handle
	HINSTANCE hInstanceNPFGM; // GameHack�� ���Ǵ� DLL Handle

	BOOL NPGetWindowsVersion();
	BOOL NPMyLoadLibraryNPFGM();
	BOOL NPEnterProtect();
	BOOL NPReleaseProtect();
	BOOL NPFindSICEProtect();
	void NPFreeLib();
};

extern class nP nPro;
