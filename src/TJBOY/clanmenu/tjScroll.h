


//��ũ�� �����Լ�

class tSCROLL
{
public:
	tSCROLL();
	~tSCROLL();

	int lifef;
	int step;		//�Ѱ��� ������ �ʿ��� ����
	int top, bot;
	int allLineSu;
	int viewLineSu;

	void Init(int flag);
	void Init(int t, int b, int menuSu, int ViewLineSu);
	int topScrollLineNum(int point);


	int ret_WHEELpoint(int wheelN, int point);
};

void TJBscrollWheelClear();

#ifdef TJSCROLL_CPP

	class tSCROLL tscr;
	class tSCROLL tscr_CL;		//ä�ø���Ʈ���ӿ� �����Ұ�
	class tSCROLL tscr_Guide;	//���̵� ���򸻿�
	//class tSCROLL tscr_GuideContent; //���̵� ����

#else 

	extern class tSCROLL tscr;
	extern class tSCROLL tscr_CL;		//ä�ø���Ʈ���ӿ� �����Ұ�
	extern class tSCROLL tscr_Guide;	//���̵� ���򸻿�
	//extern class tSCROLL tscr_GuideContent; //���̵� ���� 

#endif

void TJBscrollWheelClear_Guide();
