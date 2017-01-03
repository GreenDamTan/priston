
#define CHARACTER_USER_MAX			5
#define CHARACTER_JOB_MAX			5


#define CHARACTER_SELECT_STATE		0
#define RACE_SELECT_STATE			1
#define JOB_SELECT_STATE			2
#define FACE_SELECT_STATE			3
#define NAME_SELECT_STATE			4
#define COMPLETE_SELECT_STATE		5
#define START_GAME_STATE			6
#define RETURN_OPENING_STATE        7


#define TEMPSCRON					0
//ĳ���� ���� Ŭ����



//ktj ����.
#define TRIBE_Tempscron 0		//����ũ�� 
#define TRIBE_Moryon	1		//����̾� ����

struct HoNewCharInfo
{
	int	  JobCode;			//����..
	int   Strength;			//�� 
	int   Spirit;			//���ŷ� 
	int   Talent;			//��� 
	int   Dexterity;		//��ø�� 
	int   Health;			//�ǰ� 
	
	
};

/*
struct HoPosImage
{
	int left, int top, int width, int height;
};
*/
//#define		ERROR_NONE					5
#define		ERROR_NONAME_NEWPLAYER		10	//ĳ���� ���� ���� �� 5���� �Ѿ��� ���..
#define		ERROR_MAX_NEWPLAYER			20	//ĳ������ �̸��� �Է� ���� �ʾ��� ���..
#define		ERROR_EXHIST_NEWPLAYER		30  //ĳ������ �̸��� ������....
#define		ERROR_SELECT_CHAR			40	//ĳ���� ������ ����
#define		ERROR_SELECT_RACE			50  //���� ������ ����.
#define		ERROR_SELECT_JOB			60  //���� ������ ����.
#define		ERROR_SELECT_FACE			80  //ĳ���� ���� ���� ����.
#define     ERROR_DELETE_CLAN_MEMBER    90  //Ŭ������ ��� ������ ����.
#define     ERROR_CLAN_SERVER_NONE     100  //Ŭ�� ������ Ȯ���Ҽ� ����.

#define     SUB_SELECT_NONE					0
#define		SUB_SELECT_CHARACTER_MANAGE		1 //ĳ���� ����.
#define		SUB_SELECT_CHARACTER_USER		2 //User ĳ���� ����.
#define		SUB_SELECT_CHARACTER_NEW		3 //New ĳ���� ����.
#define		SUB_SELECT_CHARACTER_NEW_SERVER	4 //�������� ������ �޾ƿ� ���.
#define		SUB_SELECT_BACK					5 //Opening���� ���ư� ���.

class HoLogin
{
public:

	//ktj: �̺κ��� ��� ktj�� ����κ���. =================================== start
	

	int tribeNum;			//������ȣ
	RECT menuPos[2];		//�޴�����.
	//int						MoryonJobLogoImage[CHARACTER_JOB_MAX];	//TempscronJobLogo Image ��ȣ..

							//��� �󱼱׸�.
	int						MorFaceImage[CHARACTER_JOB_MAX][4][2];	//0:Fighter 1:Mechanician 2:Archer 3: Pikeman //���� ����.. //0:���� 1: �����.

	void Init_UseMor();	//0 : ����̾��þȵ�. 1:����̾��õ�.

	//ktj: �̺κ��� ��� ktj�� ����κ���. =================================== end
	
	BOOL					ClanMarkUpdateSuccessFlag;

	
	//���� ���͵�..
	//����ȭ�� ��� �׵θ�
	int			MainBoxFrameImage1[4];	 //0:mainbox_L.tga   1:mainbox_T.tga   2:mainbox_R.tga   3:mainbox_D.tga
	int			MainBoxFrameImage2[4];   //0:mainbox_CN1.tga 1:mainbox_CN2.tga 2:mainbox_CN3.tga 3:mainbox_CN4.tga
	int			TitleBoxFrameImage;		 //�Ӹ����� �ִ� Ÿ��Ʋ �̸�
	int			TitleNameImage;			 //Ÿ��Ʋ�� �� ����..
	
	//���� ��ũ �̹���
	int						TempscronMarkImage;	//TempscronMark Image ��ȣ.
	int						TempscronLogoImage;	//TempscronLogo Image ��ȣ.

	int						MoryonMarkImage;	//MoryonMark Image ��ȣ.
	int						MoryonLogoImage;	//MoryonLogo Image ��ȣ.

	int						TempscronTextImage;
	int						MoryonTextImage;

	int						RaceTextBoxImage[2];
	int						JobTextBoxImage[2];
	
	int						TempscronJobImage[CHARACTER_JOB_MAX][2];
	int						MoryonJobImage[CHARACTER_JOB_MAX][2];
	int						TempscronJobTextImage[CHARACTER_JOB_MAX];
	int						MoryonJobTextImage[CHARACTER_JOB_MAX];


	int						FaceBoxBigImage[2];
	int						FaceBoxSmallImage[3];

	BOOL					FaceSelectFirstFlag;
	int						DrawTitle(int state);

	int						ErrorDelayCount;
	BOOL					StartErrorDelayFlag;

	int						m_ScreenHalf;
private:
	//Stage ����.
	smSTAGE3D	*Stage;
	POINT3D		CaAngle;
	POINT3D		TarPosi;
	int			TarDist;
	
	BOOL		CharacterStopFlag;
	BOOL		CharacterReturnFlag;

	//Login ���º�ȭ ����.
	int SelectCurrentState;		//���� ����
	int SelectNextState;		//���� ����
	
	int  SelectSubState;			//���� ������...			
	//BOOL StartBackOpeningFlag;
	BOOL FocusWndFlag; 
	
	//������...
	int		ErrorSelect;
	
	//���º�ȭ ���� ��ư��.
	HoButton				OkButton;
	HoButton				BackButton;
	HoButton				QuitButton;
	HoButton				DeleteButton;
	HoButton				NewCharButton;
	
	
	//User Character
	smCHAR					UserCharacter[CHARACTER_USER_MAX];		//User�� ���� ĳ����.
	int						UserCharacterCount;
	//Job ���ý� ������ ĳ���͵�.
	smCHAR					JobCharacter[CHARACTER_JOB_MAX];
	int						JobCharacterCount;
	
	//���¿� ���� ǥ�� �̹���.
	//DIRECTDRAWSURFACE	CharacterSelectImage;
	int						NewCharacterImage;
	float					NewCharacterImageBlendCount;
	bool					NewCharacterImageBlendFlag;
	
	int						UserCharacterInfoImage;
	int						UserCharacterInfoBackImage;					

	//Race Select
	//DIRECTDRAWSURFACE	RaceSelectImage;
	//int						TempscronMarkImage;	//TempscronMark Image ��ȣ.
	//int						TempscronLogoImage;	//TempscronLogo Image ��ȣ.
	
	//Job Select
	//DIRECTDRAWSURFACE	JobSelectImage;
	//int						TempscronJobLogoImage[CHARACTER_JOB_MAX];	//TempscronJobLogo Image ��ȣ..

	

	//Face Select 2d �� �̹�����...
	DIRECTDRAWSURFACE	FaceSelectImage;
	int						TempFaceImage[CHARACTER_JOB_MAX][4][2];	//0:Fighter 1:Mechanician 2:Archer 3: Pikeman //���� ����.. //0:���� 1: �����.
	

	//Name Select
	//DIRECTDRAWSURFACE	NameSelectImage;
	
	
	int						MouseDownCharacterNum;
	int						MouseMoveCharacterNum;
	
	int						JobBodySelectNum; //���� ���� �κ�..
	int						JobFaceSelectNum; //�Ӹ� ���� �κ�..
	char					UserCharacterNewName[256];
	
	//MapImage
	DIRECTDRAWSURFACE	GameMapImage;
	int						MapBlendCount;
	BOOL					MapBlendFlag;
	
	//�޽��� ���..
	HoMessage				Message;
	HoMessageBox			MessageBox;


	//���� �̹���(���� âƲ)
	//int						ZoomFaceFrameImage[3];
	//int						GameFrameImage[6];
	int						BlackBoxImage;
	//int						FaceBoxImage;

	HFONT					Font;
	
	//���¿� ���� ��ȭ �Ǵ°͵�.
	int						RaceMarkBlendCount;
	
	int						RaceMarkScaleSize;

	int						LoginVolume;						
public:
	int		Load();		//�ʱ�ȭ ĳ���� ����
	int		Init();
	int		Close();	//���� ĳ���� ����
	int		Main();		//�����Լ� ĳ���� ����
	int		Draw();		//�׸��� �Լ� ĳ���� ����
	
	
	int		SetChangeJobFace(char *charFile1, char *chrFile2, int jobIndex);
	
	int		InitUserCharacter();
	int		DrawUserCharacter(int x, int y, int z, int ax, int ay, int az);
	int		DrawJobCharacter(int x, int y, int z, int ax, int ay, int az);
	int		DrawGameFrame();
	int		DrawLight();
	
	int		DrawMap(int alpha);
	int		DrawCharacterInfo();
	int		MainDefaultButton();
	
	int		KeyFilter();
	
	//Fade ����..
	//FadeIn, FadeOut����.
	BOOL		FadeCurrentState;	//0:�ƹ��͵� �Ⱥ��� fade In�� �ؾ���. 1:ȭ�鿡 �ٺ��� fade out�� �ؾ���..
	BOOL		FadeNextState;

	int		BlendValue;
	int		DrawFade();
	int		MainFade();
	int		MainFadeIn();
	int		MainFadeOut();
	void	SetFade();
	int		ReLoad();

	HoLogin();
	~HoLogin();
private:
	int		DrawSelectSubState();
	int		CameraUpDown();
	int		AddUserCharacter(char *chrFile1, char *chrFile2, int jobCode = 0);
	int		AddJobCharacter(char *chrFile1, char *chrFile2, int index = -1);
	int		GetSelectCharacter( smCHAR *lpChar );
	void	LoadLoadingImage(int level);

};


//��
extern char *szTempPikeFaceName[2][3];
extern char *szTempArcherFaceName[2][3];
extern char *szTempMechFaceName[2][3];
extern char *szTempFighterFaceName[2][3];

#ifdef _LANGUAGE_BRAZIL
#define MAX_LOADING_IMAGE	6
#else
#define MAX_LOADING_IMAGE	19
#endif