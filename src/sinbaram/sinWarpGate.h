/*----------------------------------------------------------------------------*
*	���ϸ� :  sinWarpGate.h	
*	�ϴ��� :  �Źٶ� ��������Ʈ
*	�ۼ��� :  ����������Ʈ 2003�� 6��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	


/*----------------------------------------------------------------------------*
*						�Źٶ� ��������Ʈ struct
*-----------------------------------------------------------------------------*/	
struct sSINWARPGATE{
	POINT GatePosi[10];	//�ϴ��� 10���������� �����ϼ��ְ� ��´�
	int ShowFlag;		//���� �����ش�
	int GateSelect;		//�̵��Ұ��� ��ġ�� �ε���ȭ�ؼ� �����Ѵ�
	int MoveFieldNum;	//�̵��Ұ��� �ε��� 
	int SameAreaFlag;   //���� �������� üũ�Ѵ�

	int GateUseIndex;	//�������� Ȱ��ȭ 
	POINT MapPosi;		//���� ǥ�õǴ� ��ġ
	DWORD EffectTime;	//����Ʈ �ð��� ī��Ʈ�Ѵ�
	int   EffectFlag;	//����Ʈ�� ǥ���Ҽ��ֳ��� Ȯ���Ѵ�

	POINT CancelPosi;	//�ݱ��ư 
	int   CancelFlag;   //�ݱ� �÷�

};

/*----------------------------------------------------------------------------*
*						�Źٶ� ��������Ʈ class
*-----------------------------------------------------------------------------*/	
class cSINWARPGATE{

public:

	int MatWarpMap;
	int MatIcon;
	int MatSelect;

	DIRECTDRAWSURFACE lpCancelBox;
	DIRECTDRAWSURFACE lpCancelButton;
	DIRECTDRAWSURFACE lpCancelButton_G;

//	DIRECTDRAWSURFACE	lpHelpTitle;

public:

	cSINWARPGATE();
	~cSINWARPGATE();

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

	////////////��������Ʈ �����Լ�
	
	//����Ҽ��ִ� ��������Ʈ�� ã�´�
	int SerchUseWarpGate();

	//��������Ʈ ����Ʈ
	int SetWarGateSelectEffect(int x , int y);

};

/*----------------------------------------------------------------------------*
*							extern
*-----------------------------------------------------------------------------*/	
extern cSINWARPGATE cSinWarpGate;
extern sSINWARPGATE sSinWarpGate;
extern int WarpGateUseCost[10];
extern int sinWarpGateCODE[10];
extern int FieldLimitLevel_Table[];

extern sSINWARPGATE sHaWarpGate;
extern int haWarpGateCODE[10];

