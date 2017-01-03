#ifndef _HO_OPENING_H_
#define _HO_OPENING_H_

#define FADE_IN		0
#define FADE_OUT	1

#define ERROR_NONE				     0	   //���� ����..
#define ERROR_CONNECT			     1	   //���� ����...
#define ERROR_NO_ACCOUNT		     2     //������ ����...
#define ERROR_NO_PASSWORD		     3     //�н����� Ʋ��.
#define ERROR_NO_BETA_TESTER	     4     //��Ÿ �׽��Ͱ� �ƴ�..
#define ERROR_NO_ACCOUNT_IN		     5     //�̹� ���̵�� ������..
#define ERROR_NO_FILTER_PASSWORD     6     //Password�� ������ ���� ����..
#define ERROR_NO_VERSION		     7     //Version�� ���� ����...
#define ERROR_NO_LIMIT			     8     //������ �ο��� ����...
#define ERROR_NO_SERVER_CONNECT      9     //�������� ������ ���� �����ϴ� 

#define ERROR_NO_USE_DATE_OVER	    10     //���Ⱓ ���� 
#define ERROR_NO_USE_TIME_OVER	    11     //���ð� ���� 
#define ERROR_DELETE_ACCOUNT_NOTIFY 12     //���� ���� ���� ��û��.

#define ERROR_SERVER_TRAFFIC_BUSY	13     //������ ȥ���Ͽ��� 30���Ŀ� �̿��� �ֽñ� �ٶ��ϴ�.
#define ERROR_SERVER_WAIT_MOMNENT	14     //����� �̿��� �ֽʽÿ�
#define ERROR_THIRD_CONNECT         15     //3ȸ�̻� ����
#define ERROR_HAN_PASSWORD          16     //�н����忡 �ѱ� �Է�
#define ERROR_12YEARS		        17     //12�� ���� ���� �Ұ�
#define ERROR_HOMEAGREE		        18     //Ȩ���������� ������������

#define ERROR_UNTIL_LOGINDENIED		23
#define ERROR_LOGIN_DENIED			24

enum {STATE_START, STATE_OPENING, STATE_LOGIN_INPUT, STATE_SERVER_SELECT, STATE_END}; //Opening�� ���� ���� �÷���..

enum {STATE_FADE_NONE, STATE_FADE_IN, STATE_FADE_OUT};

class HoOpening
{
	
//private:
public:

	smPAT3D     *PatCamera;
	POINT3D     PatCameraWorldPos;
	smOBJ3D     *ObjCamera;
	int			CameraFrame;
	
	smSTAGE3D	*Stage;				//�� ����
	
	POINT3D	CaAngle;
	int		TarDist;
	
	//Login Dragon â ����
	int			GameLogo[6];
	int			TriGlowLogo;
	
	//���� �߰�
	int			ButtonLoginImage[2];
	int			ButtonQuitImage[2];
	int			ButtonNewJoinImage[2];
	int			PristonLogoImage;
	//��� ȭ�� Fade Out����
	BOOL		CinemaFadeOutFlag;
	
	int			CinemaFadeState;

	int			CinemaBlendValue;
	void		InitCinemaFadeEffect();
	void		MainCinemaFadeEffect();
	void		DrawCinemaFadeEffect();

	//Login Server â ����.
	int			LoginInputState;
	BOOL		StartQuitFlag;
	
	//Opening �۾� ���� ����...
	int			WorkState;

	int			CurrentWorkState;
	int			NextWorkState;

	//���� ���� ����...
	BOOL		StartConnectServerFlag; //������ ������...
	BOOL		ConnectServerSucessFlag; //������ ���� ����...
	int			ServerWaitTimeCount;
	
	
	int			ConnectError;	  //���� ���� ����...
	int			ConnectErrorCount;
	HoMessage	Message;		  //Error Message��...
	BOOL		FocusWndFlag;

	BOOL		MouseDownFlag;

private:
	//int			DrawWide();

	//FadeIn, FadeOut����.
	int			BlackBoxImage;
	int			WhiteBoxImage;
	
	BOOL		FadeCurrentState;	//0:�ƹ��͵� �Ⱥ��� fade In�� �ؾ���. 1:ȭ�鿡 �ٺ��� fade out�� �ؾ���..
	BOOL		FadeNextState;
	
	int			ScreenBlendValue;
	int			LogoBlendValue;
	int			LoginBlendValue;
	int			ServerSelectBlendValue;

	int			MouseDownServerIndex;
	int			OpeningBgmVolume;
	
	int			MainFadeIn(); 
	int			MainFadeOut();
	int			MainFade();
	int			DrawFade();
	int			FastFade();

	HoMessageBox	MessageBox;
	
	int		startLogoX;
	int		startLogoY;
	int		startLogoAddX;
	int		startAccountAddX;
	int		startAccountAddY;
	int		m_AddWidth;
	int		m_AddHeight;

	//######################################################################################
	//�� �� �� : �� �� ��
	smMATRIX	m_CameraRotInvert;
	int			m_IsDrawOpening;
	int			m_AutoCameraFrameTime;
	int			m_ParticleSpawnTimeCount;
	//######################################################################################

public:
	//int			AddObject(char	*name);
	int			LoginDataServer();
	int			LoginGameServer(int serverIndex);

	int		Init();
	int		Load();
	void	Close();
	
	int		Main();
	int		Draw();
	

	//ȭ�� ����...
	void	SetFade();
	int		KeyFilter(char *message);
	void    SetConnectError(int errorCode) { ConnectError = errorCode; }
	HoOpening();
	~HoOpening();


	//######################################################################################
	//�� �� �� : �� �� ��
	int  DebugModeLoadFieldInfo(void);
	void SetCameraCharPos(void);
	void SetParticleEffectSpawn(void);
	//######################################################################################
};

extern HWND hTextWnd;
#endif