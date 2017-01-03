/*----------------------------------------------------------------------------*
*	���ϸ� :  sinInterFace.h	
*	�ϴ��� :  �������̽��� ���� ���� �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

//////////����� ��ȯ 
#define		RESTART_FEILD		1
#define		RESTART_TOWN		2
#define		RESTART_EXIT		3

/////////��ų ���� �������̽� ǥ��
#define     HA_DEFANCE			1
#define     HA_BLOCK			2
#define     HA_EVASION			3

/*----------------------------------------------------------------------------*
*						(�������̽�  )class
*-----------------------------------------------------------------------------*/	
struct sRUNCAMERAMAP{ //��ư ��� �÷� 

	int RunFlag;			 //�ȱ� �޸��� �÷� 
	int CameraAutoFlag;		 //ī�޶� �ڵ� �÷� 
	int MapOnFlag;			 //�ʺ��� �÷� 

	//�̰��� �������̽����� �Ͼ�� ��� �̺�Ʈ ��ư�� �߰���ų ���� 
	
};

////// ������ ���� ���׹̳� �ܻ�ȿ�� 
struct sSHADOWSTATE{
	int Flag;
	int Time;
	POINT Posi;
	POINT Size;
	DWORD Color;

};


class cINTERFACE{

public:

	sRUNCAMERAMAP	sInterFlags;


	int MatMain;					    //��� �������̽� 
	int MatMenu[2];						//�޴�  
	int MatChatDefault[2];				//ä�ñ⺻ 
	int MatChatSmall[2];				//���� ä��â
	int MatChatDoc[2];					//���� â 
	int MatExitMenu;					//������ �޴� 
	int MatComPass;						//��ħ�� ���� 	

	int MatLife;						//������   �ܻ�ȿ��
	int MatMana;						//����     �ܻ�ȿ��
	int MatStamina;						//���׹̳� �ܻ�ȿ�� 

	DIRECTDRAWSURFACE    lpEXP;      //����ġ  
	DIRECTDRAWSURFACE    lpMP;		//����  	
 	DIRECTDRAWSURFACE    lpLIFE;     //������ 
 	DIRECTDRAWSURFACE    lpSTM;      //���׹̳� 
	DIRECTDRAWSURFACE    lpANGER;    //�г������ 

 	DIRECTDRAWSURFACE    lpLIFESpot;     //������ 
 	DIRECTDRAWSURFACE    lpSTMSpot;      //���׹̳� 
	DIRECTDRAWSURFACE    lpMANASpot;     //����

	DIRECTDRAWSURFACE    lpButton[6];//��ư ������ 
	DIRECTDRAWSURFACE    lpButton2[6];//��ư ������2 

	DIRECTDRAWSURFACE    lpMenuButton[3];//�޴� ��ư 

	DIRECTDRAWSURFACE    lpInfoBox[6]; //ǳ�� ���� 
	DIRECTDRAWSURFACE    lpWalk;		  //�ȱ� 

	DIRECTDRAWSURFACE    lpRunInfo;    //�ٱ�ǳ�� ���� 
	DIRECTDRAWSURFACE    lpWalkInfo;   //�ȱ�ǳ�� ���� 
	
	DIRECTDRAWSURFACE    lpCameraAutoInfo;   //�ڵ� ī�޶� ǳ�� ���� 
	DIRECTDRAWSURFACE    lpCameraHandInfo;   //���� ī�޶� ǳ�� ���� 
	DIRECTDRAWSURFACE    lpCameraFixInfo;   //���� ī�޶� ǳ�� ���� 

	DIRECTDRAWSURFACE    lpMapOnInfo;   //�ڵ� ī�޶� ǳ�� ���� 
	DIRECTDRAWSURFACE    lpMapOffInfo;   //���� ī�޶� ǳ�� ���� 

	DIRECTDRAWSURFACE    lpChatShowButton;	//ūä��â

	DIRECTDRAWSURFACE    lpSun;	//��
	DIRECTDRAWSURFACE    lpMoon;	//��

	DIRECTDRAWSURFACE    lpGage[2]; //0��ü  1������ 

	DIRECTDRAWSURFACE	lpBar_Anger;
	DIRECTDRAWSURFACE	lpBar_Time;
	DIRECTDRAWSURFACE	lpBar_Exp;
	DIRECTDRAWSURFACE	lpBar_ExpWon;

	DIRECTDRAWSURFACE	lpButton_Anger;

	DIRECTDRAWSURFACE	lpMapOnImage;
	DIRECTDRAWSURFACE	lpCameraImage[2]; //0���� 1 ���� 

	DIRECTDRAWSURFACE	lpReStartMain; //�ٽý��� 
	DIRECTDRAWSURFACE	lpReStartButton[3]; //�ٽý��� 

	DIRECTDRAWSURFACE	lpSelectSkill_Main[2];		//��ų ���� 0 �׷��� 1 Ȱ�� 
	DIRECTDRAWSURFACE	lpSelectChangeJob[2];   //����        0 �׷��� 2 Ȱ�� 

	DIRECTDRAWSURFACE	lpTime7;


	int MatClock; //�ð� 
	int ChatFlag; //ä�� �÷� 

public:
	cINTERFACE();
	~cINTERFACE();

	void Init();
	void Load();
	void Release();
	void Close();
	void Draw();
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); 


	void InitState();		//����� , ��� , �ٷ� �� �ʱ�ȭ�Ѵ� 
	void CheckingState();	//����� , ��� , �ٷ� �� ��ȭ�� ���� Bmp�� ũ�⸦ ��ȭ ��Ų��
	void ShowParaState();   //����� ��� �ٷ� ����ġ ��ġ�� ǥ���Ѵ� 

	void DrawInterText();   //�������̽��� �� �ؽ�Ʈ�� ǥ���Ѵ� 

	void CheckAllBox(int ButtonFlag); //�ڽ��� �ϳ��� �����ش� 

	int DrawClockArrow( int x, int y, int Angle ); //�ð踦 �׷��ش� 
	int DrawCompass(int x , int y ,int Angle);     //��ħ���� �׸��� 


	int CheckExpPercentControl(); //��Ʈ���� ������ ����ġ�� 4%�̸��̸� ����ġ �ٰ� ���ڰŸ��� 

	//////////////////����� �̹��� �ε� & ������ 
//	void LoadReStartImage();
//	void ReleaseReStartImage();
	int GetWordTimeDisplay();

	void ShowExpPercent(); //����ġ�� �����ش�

	int  SetStringEffect(int); //����Ʈ���ڸ� �����ش� 

};

extern cINTERFACE	cInterFace;

extern POINT InfoMessageBoxPos;  //�ؽ��� ǥ���� ��ǥ 

/*ä�ð���*/
extern int sinChatEnter;          //ä�� �÷� (����Ű�� �Է��ϸ� ä�� â�� ���´� )
extern int SubChatHeight;	      //ä��â ���� ��ġ ��ǥ �� 	
extern int sinChatDisplayMode ;   //���� ä���� ��� 
extern int sinChatInputMode;      //ä�� �Է� ��� 
////////////

extern int sinFireShow;           //���� �����ش� 
extern int ExitButtonClick;

extern int ReStartFlag;			  //���� ������ �ٽý��� �������̽��� �����ش� 
extern int ReStartOptionIndex;	  //�ٽý��� ���� �ε��� 	

extern int SkillNpcFlag;			  //���� ��ų ���� �������̽��� �����ش� 
extern int SkillMasterOptionIndex;

extern int RestartCheckExp;		  //�ٽý����Ҽ��ִ� ����ġ�� �ֳ� ���� 


extern int psDrawTexImage2( smTEXTUREHANDLE *hTexHandle , float *x, float *y, float fx, float fy, float fw, float fh, int Transp , int specular);

extern int NotUseSkillEffect[3];

extern int ChangeJobButtonclick;

extern int  ResetQuset3Flag;

extern int ReStartFlag;
extern POINT ReStartMainXY;
extern int ReStartIndex;
extern POINT ReStartTextXY;
extern char szReStartMsg[128];
extern char szReStartMsg2[128];


