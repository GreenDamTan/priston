#include "common.h"
#include "smlib3d\\smd3d.h"

#include "smwsock.h"
#include "character.h"
#include "avictrl.h"
#include "playmain.h"
#include "Sound\\dxwav.h"
#include "fileread.h"

#include "particle.h"
#include "netplay.h"

#include "sinbaram\\sinlinkheader.h"
#include "hobaram\\holinkheader.h"

#include "field.h"
#include "effectsnd.h"

#include "record.h"
#include "playsub.h"
#include "skillsub.h"
#include "damage.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\cE_CViewClanInfo.h"

#include "Language\\language.h"

#include "cracker.h"
#include "damage.h"
#include "AreaServer.h"

#include "Server\onserver.h"

extern int WinSizeX;
extern int WinSizeY;
extern HWND hwnd;
extern int MouseButton[3];

//�ʵ�� �ε�
int LoadFieldMap( int PosNum , sFIELD *lpField , smSTAGE3D *lpStage );


//���� ��Ʈ Ŭ������!
smSTAGE3D		*smStage = 0;
smSTAGE3D		*smGameStage[2] = { 0,0 };
sFIELD			*StageField[2] = { 0,0 };


rsRECORD_DBASE	rsRecorder;					//������ ���ӵ���Ÿ �����ġ

char szGameStageName[2][64];
DWORD			dwPlayTime=0;					//������ ������ �ð� ( �÷��� �� )
DWORD			dwMemError=8080;				//�޸� ���� Ȯ�� �ڵ�

int	smStageCount = 0;
int SkipNextField = 0;				//���� �ʵ�� �Ѿ

int	NightDayTime=0;					//�㳷 ����

int invPlayerArmor = 0;

DWORD	dwBattleTime =0;				//�ֱ� ���� �ð�
DWORD	dwBattleQuitTime =0;			//������ ���� �Ϸ� �Ҷ�
DWORD	dwCloseBoxTime = 0;				//�۾�â ���� �޼���

int		RestartPlayCount = 0;					//����� ���� ���� ī��Ʈ

DWORD SkillFalconPoint;					//���� ��ų ����Ʈ �����

int InvArmorFileCode[] =
{ 0 ,9,1, 5 };

int	DarkLevel = 0;					//��� ��
int	BackColor_R = 0;				//���� R
int	BackColor_G = 0;				//���� G
int	BackColor_B = 0;				//���� B

int	BackColor_mR = 0;				//���� ���氪 R
int	BackColor_mG = 0;				//���� ���氪 G
int	BackColor_mB = 0;				//���� ���氪 B

int	DarkLevel_Fix = 0;				//��� �� ����

int	DarkLightRange = 180*fONE;		//��οﶧ ���� ����

DWORD	dwGameWorldTime = 0;			//������ ����ð�
DWORD	dwGameHour = 12;					//������ ��
DWORD	dwGameMin  = 0;					//������ ��
DWORD	dwGameTimeOffset = 0;

int DarkLight = 0;

//���� ����
int WeatherMode = 0;
int Thunder = 0;

//PK���
int	PkMode = 0;

int DispApp_SkillPoint = 0;			//������ �ð����� ��ų ����Ʈ

int rcnt2;

int modelX = 255;
int modelZ = 512;
int wflag = 0;
int wrad = 0;

DWORD	UseEtherCoreCode = 0;			//��Ƽ�� �ھ� ���

int	RecordFailCount =0;				//���� ���� ī����

//���� �޾����� ȸ�� ī����
int	Record_BlockCount = 0;
int	Record_DefenceCount = 0;
int Record_RecvDamageCount = 0;
int Record_TotalRecvDamageCount = 0;


//���� ���콺 ������
DWORD	AutoMouse_LastCheckTime = 0;
DWORD	AutoMouse_StartCheckTime = 0;
int		AutoMouse_WM_Count = 0;
int 	AutoMouse_Distance = 0;
int		AutoMouse_Attack_Count = 0;

//ȭ��
//smPAT3D *PatTest;

//ȭ��
smPAT3D *PatArrow;
//���� ����Ʈ
smPAT3D *PatEffectShield =0;
//����ũ���� ����Ʈ
smPAT3D *PatSparkShield =0;
//��������ҷ��̼�
smPAT3D *PatDivineShield =0;
//���鸮 ����
smPAT3D *PatGodlyShield =0;
//����ĳ�� �հ�
smPAT3D *PatCrown =0;

//�׸���
smPAT3D *PatShadow[SHADOW_PAT_MAX] = { 0,0,0,0 };

//######################################################################################
//�� �� �� : �� �� ��
smPAT3D *PatIceFoot[ ICE_FOOT_PAT_MAX ];
//######################################################################################

int	Moving_DblClick = 0;		//���콺 �̵� ýũ ���� Ŭ��
POINT3D	MousePos_DblClick;

POINT3D	TracePos_DblClick;
int TraceMode_DblClick = 0;

int	ActionGameMode = 0;	//�׼ǰ��� ���


//smCHAR	chrPlayer;
smCHAR		*lpCurPlayer = 0;

DWORD	dwRotPlayDelayTime = 0;			//���ΰ� ������ ���� ������

/*
//ȸ�� �÷��̾� ������ ����
smCHAR *SelectRotPlayer( smCHAR *lpCurrentChar );
//ȸ�� �÷��̾� ������ ����
int	CloseRotPlayer();
//ȸ�� �÷��̾� ������ �ʱ�ȭ
smCHAR *InitRotPlayer();

lpCurPlayer = InitRotPlayer();

*/


scITEM	scItems[ DISP_ITEM_MAX ];			//ITEM

smCHAR	chrOtherPlayer[OTHER_PLAYER_MAX];

smCHAR_INFO	smCharInfo_Back;			//NPC ��ġ�� �⺻ ����Ÿ �ӽ� ����


int	MatEachBox;				//�� Ȯ�� �׵θ� �׸� ��Ʈ����
int	MatEachMenuBox;			//�޴� ��ư �׵θ�
int MatEnergyBox[2];		//������ ��Ʈ����
int MatLight[4];				//���� �� �̹��� ��Ʈ���� //glow.bmp

int	MatEachTradeButton[2];	//�޴� ������ ��ȯ ��ư 
int MatMonBroodImage[8];	//���� ���� �̹���

//���� ȸ�� ���ప
smTRANS_COMMAND	sPartyPosion;
DWORD			dwPartyPosionTime;

DIRECTDRAWSURFACE	lpDDS_ParTradeButton[4][2];			//�޴��� Ʈ���̵� ��ư
DIRECTDRAWSURFACE	lpDDS_ParTradeToolTip[4];			//�޴��� Ʈ���̵� ����


//�Ҳ� �ִϸ��̼�
char *szFlame[8] = {
	"image\\flame1.bmp",
	"image\\flame2.bmp",
	"image\\flame3.bmp",
	"image\\flame4.bmp",
	"image\\flame5.bmp",
	"image\\flame6.bmp",
	"image\\flame7.bmp",
	"image\\flame8.bmp"
};

char *szShine[] = {
	"image\\shine0.bmp",
	"image\\shine2.bmp",
	"image\\shine4.bmp",
	"image\\shine6.bmp",
	"image\\shine8.bmp",
	""
};

char *szBlood1[] = {
	"image\\blood\\b1_001.tga",
	"image\\blood\\b1_002.tga",
	"image\\blood\\b1_003.tga",
	"image\\blood\\b1_004.tga"
};

char *szBlood2[] = {
	"image\\blood\\b2_001.tga",
	"image\\blood\\b2_002.tga",
	"image\\blood\\b2_003.tga",
	"image\\blood\\b2_004.tga"
};


char *szShadowImageFile = "image\\bshadow.bmp";
//char *szShadowImageFile = "image\\shadow.tga";

//######################################################################################
//�� �� �� : �� �� ��
char *g_IceFootImageFileBuff[ ICE_FOOT_PAT_MAX ] =
{
	"image\\mk-r.tga",
	"image\\mk-l.tga",
};
//######################################################################################



//��� �δ� �ʱ�ȭ
void InitStageLoader()
{
	szGameStageName[0][0] = 0;
	szGameStageName[1][0] = 0;
	smGameStage[0]=0;
	smGameStage[1]=0;
}

//����� �о� �´�
smSTAGE3D *LoadStageFromField( sFIELD *lpField , sFIELD *lpSecondField )
{
	smSTAGE3D *lpstg;
	char *szStageFile;
	int cnt;
	int	Bip;
	char	szBuff[128];

	szStageFile = lpField->szName;

	if ( lpField==StageField[0] ) 
		return smGameStage[0];
	if ( lpField==StageField[1] ) 
		return smGameStage[1];

	lpstg = new smSTAGE3D;

	// ����Ʈ ����
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright		= smConfig.MapBright;
	lpstg->Contrast		= smConfig.MapContrast;

	//����� �� �����忡 ���Ͽ� �ε� �Ѵ�
	AddLoaderStage( lpstg , szStageFile );

	if ( !lpstg->StageObject ) 
	{
		//�˼� ���� ����
		//Ŭ���̾�Ʈ �α� ���� ���
		wsprintf( szBuff , "Stage Loading Error ( %s )",szStageFile );
		Record_ClinetLogFile( szBuff );
		delete lpstg;
		return NULL;
	}

	//���� ������Ʈ �߰�
	for( cnt=0;cnt<lpField->StgObjCount;cnt++ )
	{
		Bip = lpField->GetStageObjectName( cnt , szBuff );
		if ( szBuff[0] ) {
			if ( Bip )
				lpstg->StageObject->AddObjectFile( szBuff , szBuff );
			else
				lpstg->StageObject->AddObjectFile( szBuff );
		}
	}

	if (lpField->StgObjCount && renderDevice.IsDevice())
	{
		ReadTextures();
		lpstg->smMaterialGroup->CheckMatreialTextureSwap();	
	}

	if ( lpSecondField && lpSecondField==StageField[0] )
	{
		if ( smGameStage[1] ) 
			delete smGameStage[1];
		smGameStage[1] = lpstg;
		StageField[1] = lpField;
		LoadFieldMap( 1, lpField , lpstg );				//�ʵ�� �ε�
	}
	else
	{
		if ( !lpSecondField || lpSecondField==StageField[1] )
		{
			if ( smGameStage[0] ) 
				delete smGameStage[0];
			smGameStage[0] = lpstg;
			StageField[0] = lpField;
			LoadFieldMap( 0, lpField , lpstg );			//�ʵ�� �ε�
		}
	}


	if ( StageField[0] ) 
		lstrcpy( szGameStageName[0] , StageField[0]->szName );

	if ( StageField[1] ) 
		lstrcpy( szGameStageName[1] , StageField[1]->szName );


	DWORD	dwTime = GetCurrentTime();

	if ( dwLastRecvGameServerTime && dwLastRecvGameServerTime<dwTime ) 
		dwLastRecvGameServerTime=dwTime;
	if ( dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2<dwTime ) 
		dwLastRecvGameServerTime2 = dwTime;
	if ( dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3<dwTime ) 
		dwLastRecvGameServerTime3 = dwTime;
	if ( dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4<dwTime ) 
		dwLastRecvGameServerTime4 = dwTime;

	if ( AreaServerMode )
	{
		//���� ���� �����Ȯ�� �ð� ����
		if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->dwDeadLockTime = dwTime;
		if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->dwDeadLockTime = dwTime;
	}

	return lpstg;
}

//����� �о� �´� 
smSTAGE3D *LoadStageFromField2( sFIELD *lpField , sFIELD *lpSecondField )
{
	smSTAGE3D *lpstg;
	char *szStageFile;
	int cnt;
	int Bip;
	char szBuff[128];


	szStageFile = lpField->szName;

	if ( lpField==StageField[0] ) return smGameStage[0];
	if ( lpField==StageField[1] ) return smGameStage[1];

	lpstg = new smSTAGE3D;

	// ����Ʈ ����
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright		= smConfig.MapBright;
	lpstg->Contrast		= smConfig.MapContrast;

	//����� �ε� �Ѵ�
//	smSTAGE3D_ReadASE( szStageFile , lpstg );
//	lpstg->Head = TRUE;

	//����� �� �����忡 ���Ͽ� �ε� �Ѵ�
	AddLoaderStage( lpstg , szStageFile );

	if ( !lpstg->StageObject ) {
		//�˼� ���� ����
		//Ŭ���̾�Ʈ �α� ���� ���
		wsprintf( szBuff , "Stage Loading Error ( %s )",szStageFile );
		Record_ClinetLogFile( szBuff );
		delete lpstg;
		return NULL;
	}

	//���� ������Ʈ �߰�
	for( cnt=0;cnt<lpField->StgObjCount;cnt++ ) {
		Bip = lpField->GetStageObjectName( cnt , szBuff );
		if ( szBuff[0] ) {
			if ( Bip )
				lpstg->StageObject->AddObjectFile( szBuff , szBuff );
			else
				lpstg->StageObject->AddObjectFile( szBuff );
		}
	}

	if (lpField->StgObjCount && renderDevice.IsDevice()) {
		ReadTextures();
		lpstg->smMaterialGroup->CheckMatreialTextureSwap();	
	}



	if ( lpSecondField && lpSecondField==StageField[0] ) {
		if ( smGameStage[1] ) delete smGameStage[1];
		smGameStage[1] = lpstg;
		StageField[1] = lpField;
		//�ʵ�� �ε�
		LoadFieldMap( 1, lpField , lpstg );
	}
	else {
		if ( !lpSecondField || lpSecondField==StageField[1] ) {
			if ( smGameStage[0] ) delete smGameStage[0];
			smGameStage[0] = lpstg;
			StageField[0] = lpField;
			//�ʵ�� �ε�
			LoadFieldMap( 0, lpField , lpstg );
		}
	}

	if ( StageField[0] ) lstrcpy( szGameStageName[0] , StageField[0]->szName );
	if ( StageField[1] ) lstrcpy( szGameStageName[1] , StageField[1]->szName );

	DWORD	dwTime = GetCurrentTime();
	if ( dwLastRecvGameServerTime && dwLastRecvGameServerTime<dwTime ) dwLastRecvGameServerTime=dwTime;
	if ( dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2<dwTime ) dwLastRecvGameServerTime2 = dwTime;
	if ( dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3<dwTime ) dwLastRecvGameServerTime3 = dwTime;
	if ( dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4<dwTime ) dwLastRecvGameServerTime4 = dwTime;

	if ( AreaServerMode ) {
		//���� ���� �����Ȯ�� �ð� ����
		if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->dwDeadLockTime = dwTime;
		if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->dwDeadLockTime = dwTime;
	}

	return lpstg;
}

//����� �о� �´�
smSTAGE3D *LoadStage( char *szStageFile )
{
	smSTAGE3D *lpstg;

	if ( lstrcmpi( szGameStageName[0] , szStageFile )==0 ) return smGameStage[0];
	if ( lstrcmpi( szGameStageName[1] , szStageFile )==0 ) return smGameStage[1];

	lpstg = new smSTAGE3D;

	// ����Ʈ ����
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright		= smConfig.MapBright;
	lpstg->Contrast	= smConfig.MapContrast;

	//����� �� �����忡 ���Ͽ� �ε� �Ѵ�
	AddLoaderStage( lpstg , szStageFile );

	smStageCount = (smStageCount+1)&1;

	if ( smGameStage[smStageCount] ) delete smGameStage[smStageCount];

	smGameStage[smStageCount] = lpstg;
	//ReadTextures();
	return lpstg;
}

//����� �о� �´�
smSTAGE3D *LoadStage2( char *szStageFile )
{
	smSTAGE3D *lpstg;

	if ( lstrcmpi( szGameStageName[0] , szStageFile )==0 ) return smGameStage[0];
	if ( lstrcmpi( szGameStageName[1] , szStageFile )==0 ) return smGameStage[1];

	lpstg = new smSTAGE3D;

	// ����Ʈ ����
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright		= smConfig.MapBright;
	lpstg->Contrast	= smConfig.MapContrast;

	//����� �� �����忡 ���Ͽ� �ε� �Ѵ�
	smSTAGE3D_ReadASE( szStageFile , lpstg );
	lpstg->Head = TRUE;

	smStageCount = (smStageCount+1)&1;

	if ( smGameStage[smStageCount] ) delete smGameStage[smStageCount];

	smGameStage[smStageCount] = lpstg;
	//ReadTextures();
	return lpstg;
}


int PlayCounter = 0;

char PlayerName[64] = { 0,0,0,0 };


void InitStage()
{
//	int cnt;

	szGameStageName[0][0] = 0;
	szGameStageName[1][0] = 0;

/*
	smStage = new smSTAGE3D;

	// ����Ʈ ����
	smStage->VectLight.x = smConfig.MapLightVector.x;
	smStage->VectLight.y = smConfig.MapLightVector.y;
	smStage->VectLight.z = smConfig.MapLightVector.z;

	smStage->Bright		= smConfig.MapBright;
	smStage->Contrast	= smConfig.MapContrast;


//	smStage = smSTAGE3D_ReadASE( smConfig.szFile_Stage  );
	smStage = smSTAGE3D_ReadASE( smConfig.szFile_Stage , smStage );
*/
//	smStage = LoadStage( smConfig.szFile_Stage );
//	smStage = LoadStage( "field\\forest.ASE" );
/*
	smStage = LoadStage( "field\\village.ASE" );
	smStage = LoadStage( "field\\forest(attach).ASE" );
*/
//	smStage = LoadStage2( "field\\village.ASE" );
//	smStage = LoadStage2( smConfig.szFile_Stage  );
//	smStage = LoadStage2( "field\\forest(attach).ASE" );

	//�ʵ� �� �ʱ�ȭ
	InitFieldMap();

	//�ʵ� �ʱ�ȭ
	InitField();

	if ( GameStartField<0 )
	{
		//�ʵ� ����
		StartField();
	}
	else 
	{
		//����� ������ �ʵ� ����

		 //CharacterPosX
		 //CharacterPosZ

		//WarpField( GameStartField , (int *)&smConfig.Posi_Player.x, (int *)&smConfig.Posi_Player.y );

		 if ( CharacterPosX==PrisonX && CharacterPosZ==PrisonZ ) 
		 {
			 //��������
			WarpPrisonField( (int *)&smConfig.Posi_Player.x, (int *)&smConfig.Posi_Player.y );
		 }
		 else 
		 {
			WarpFieldNearPos( GameStartField , CharacterPosX, CharacterPosZ, (int *)&smConfig.Posi_Player.x, (int *)&smConfig.Posi_Player.y );
		}
	}

	//if ( lpD3DDevice ) ReadTextures();

	smRender.Color_R  = 0;
	smRender.Color_G  = 0;
	smRender.Color_B  = 0;
	smRender.Color_A  = 0;


	//�ʵ�� �ε�
	LoadFieldMap( 0, StageField[0] , smGameStage[0] );

/*
	for( cnt=0;cnt<smConfig.StageObjCnt;cnt++) {
		smStage->StageObject->AddObjectFile( smConfig.szFile_StageObject[cnt] );
	}
*/
}

void CloseStage()
{

	//if( smStage ) delete smStage;

	if ( smGameStage[1] ) delete smGameStage[1];
	if ( smGameStage[0] ) delete smGameStage[0];

}


struct _DRAW_LIGHTLIST {
	smLIGHT3D	*smLight;
	int			x,y,z;
};


_DRAW_LIGHTLIST	DrawLight[64];
_DRAW_LIGHTLIST	DrawLightSmall[64];
int DrawLightCnt;
int DrawLightSmallCnt;


int DrawLightImage( smSTAGE3D *lpStage , int x, int y, int z )
{

// ��ǥ�� 2D �������� ��ȯ�Ͽ� �����ش�  
//int smRENDER3D::GetRect2D ( int x, int y, int z, int width, int height, RECT *Rect ,POINT *Pos )

	POINT	Pos;
	RECT	Rect;

	int cnt,cnt2;
	int dx,dy,dz,dist;
	int tz;

//	smLIGHT3D	*DrawLight[64];
//	smLIGHT3D	*DrawLightSmall[64];

	int x1,y1,z1,x2,y2,z2,x3,y3,z3;
	smRENDFACE *rendface;
	smRENDVERTEX *rv;
	int flg;

	//DDSURFACEDESC2		ddsd;
	//WORD *Dest;
	//DWORD dwZCode;
	//DWORD dwZPoint;

	int type;


	for(cnt=0;cnt<lpStage->nLight;cnt++) {

	  type = lpStage->smLight[cnt].type;

	  if ( (type&smLIGHT_TYPE_LENS) && 
			( !(lpStage->smLight[cnt].type&smLIGHT_TYPE_NIGHT) || NightDayTime ) ) {

		dx = (x-lpStage->smLight[cnt].x)>>FLOATNS;
		dy = (y-lpStage->smLight[cnt].y)>>FLOATNS;
		dz = (z-lpStage->smLight[cnt].z)>>FLOATNS;
		dist = dx*dx+dy*dy+dz*dz;

		if ( dist<DIST_TRANSLEVEL_LOW && abs(dx)<64*38 && abs(dz)<64*38 ) {
			if ( tz=smRender.GetRect2D ( lpStage->smLight[cnt].x,	lpStage->smLight[cnt].y,lpStage->smLight[cnt].z,
				0,0,&Rect,&Pos ) ) {

				if ( Pos.x>0 && Pos.x<WinSizeX && Pos.y>0 && Pos.y<WinSizeY ) {
					tz-=64*fONE;

					if ( tz>(64*22*fONE) ) {
						DrawLightSmall[DrawLightSmallCnt].smLight = &lpStage->smLight[cnt];
						DrawLightSmall[DrawLightSmallCnt].x = Pos.x;
						DrawLightSmall[DrawLightSmallCnt].y = Pos.y;
						DrawLightSmall[DrawLightSmallCnt].z = tz;
						DrawLightSmallCnt++;
					}
					else {
						//dwZPoint = (DWORD)(	smConvZ3D( tz>>FLOATNS )*65535 );
						for(cnt2=0;cnt2<smRender.nRendFace;cnt2++) {
							if ( !(smRender.RendFace[cnt2].ClipStatus&SMCLIP_DISPLAYOUT) ) {
								rendface = &smRender.RendFace[cnt2];

								rv = rendface->lpRendVertex[0];
								x1 = (int)rv->xy[0];
								y1 = (int)rv->xy[1];
								z1 = rv->tz;
								rv = rendface->lpRendVertex[1];
								x2 = (int)rv->xy[0];
								y2 = (int)rv->xy[1];
								z2 = rv->tz;
								rv = rendface->lpRendVertex[2];
								x3 = (int)rv->xy[0];
								y3 = (int)rv->xy[1];
								z3 = rv->tz;

								flg = 0;
								if ( tz>z1 && tz>z2 && tz>z3 ) {

									if ( Pos.x<x1 && Pos.x<x2 && Pos.x<x3 ) flg++;
									if ( Pos.x>x1 && Pos.x>x2 && Pos.x>x3 ) flg++;
									if ( Pos.y<y1 && Pos.y<y2 && Pos.y<y3 ) flg++;
									if ( Pos.y>y1 && Pos.y>y2 && Pos.y>y3 ) flg++;
									if ( flg==0 ) break;

//									if ( PointInTri( (float)x1,(float)x2,(float)x3,(float)y1,(float)y2,(float)y3,(float)Pos.x,(float)Pos.y)==true ) 
//									break;
								}
							}
	
						}

						if ( cnt2>=smRender.nRendFace ) {
							DrawLight[DrawLightCnt].smLight = &lpStage->smLight[cnt];
							DrawLight[DrawLightCnt].x = Pos.x;
							DrawLight[DrawLightCnt].y = Pos.y;
							DrawLight[DrawLightCnt].z = tz;
							DrawLightCnt++;
						}
						else {
							DrawLightSmall[DrawLightSmallCnt].smLight = &lpStage->smLight[cnt];
							DrawLightSmall[DrawLightSmallCnt].x = Pos.x;
							DrawLightSmall[DrawLightSmallCnt].y = Pos.y;
							DrawLightSmall[DrawLightSmallCnt].z = tz;
							DrawLightSmallCnt++;
						}
/*
						flg = 0;
						dsDrawColorBox( 65535 , Pos.x-1 , Pos.y-1 , 4,4 );
						ddsd.dwSize		= sizeof(DDSURFACEDESC2);
						lpDDSZBuffer->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
						Dest = (WORD *)ddsd.lpSurface;
						if ( Dest ) {
							dwZCode = Dest[((Pos.y * ((int)ddsd.lPitch/2))+Pos.x)];
							if (dwZCode>dwZPoint ) flg = 1;

						}
						lpDDSZBuffer->Unlock(NULL);
						if ( flg )
							DrawLight[DrawLightCnt++] = &lpStage->smLight[cnt];
*/
					}

				}
			}
		}
	  }
	}

	return TRUE;
}

int DrawLightImage()
{
	smFACE2D face2d;
	int size;
	smRENDFACE *RendFace;
	int cnt;

	smRender.SetMaterialGroup( smMaterialGroup );

	smRender.Init();

	for(cnt=0;cnt<DrawLightSmallCnt;cnt++) {

		size = 40*fONE;
		/*
		if ( DrawLightSmall[cnt].z>(64*18*fONE) ) {
			size -= (DrawLightSmall[cnt].z - (64*18*fONE))*2;
		}
		*/

		if ( size>0 ) {

			face2d.TexRect.left = 0;
			face2d.TexRect.right = 1;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width  = size;
			face2d.height = size;
			face2d.Transparency = 255;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = DrawLightSmall[cnt].smLight->x;
			face2d.y = DrawLightSmall[cnt].smLight->y;
			face2d.z = DrawLightSmall[cnt].smLight->z;
			face2d.MatNum = MatLight[1];
	
			RendFace=smRender.AddFace2D ( &face2d );
		}
	}

	if ( smRender.nRendFace>0 ) {
		smRender.SetClipStates( SMCLIP_LEFT|SMCLIP_RIGHT|SMCLIP_TOP|SMCLIP_BOTTOM );
		smRender.ClipRendFace();							// ��ü ������ ���̽��� Ŭ����
		smRender.GeomVertex2D( FALSE );				// ���ؽ��� 2D��ǥ�� ��ȯ  
		smRender.RenderD3D();
	}


	smRender.Init();

	for(cnt=0;cnt<DrawLightCnt;cnt++) {
		size = 160*fONE;

		/*
		if ( DrawLight[cnt].z>(64*18*fONE) ) {
			size -= (DrawLight[cnt].z - (64*18*fONE))*2;
		}
		*/

		if ( size>0 ) {

			face2d.TexRect.left = 0;
			face2d.TexRect.right = 1;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width  = size;
			face2d.height = size;
			face2d.Transparency = 255;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = DrawLight[cnt].smLight->x;
			face2d.y = DrawLight[cnt].smLight->y;
			face2d.z = DrawLight[cnt].smLight->z;
			face2d.MatNum = MatLight[0];
				
			RendFace=smRender.AddFace2D ( &face2d );
		}
	}

	if ( smRender.nRendFace>0 ) {
		smRender.SetClipStates( SMCLIP_LEFT|SMCLIP_RIGHT|SMCLIP_TOP|SMCLIP_BOTTOM );
		smRender.ClipRendFace();							// ��ü ������ ���̽��� Ŭ����
		smRender.GeomVertex2D( FALSE );				// ���ؽ��� 2D��ǥ�� ��ȯ  
		for(cnt=0;cnt<smRender.nRendVertex;cnt++) {
			smRender.RendVertex[cnt].zb = 0.001f;
		}
	    //lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, FALSE );
		smRender.RenderD3D();
	    //lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
	}


	return TRUE;

}

int SetStageDynLight(smSTAGE3D *lpStage , int x, int y, int z)
{

	int cnt;
	int dx,dy,dz,dist;
	int type;


	for(cnt=0;cnt<lpStage->nLight;cnt++) {

		dx = (x-lpStage->smLight[cnt].x)>>FLOATNS;
		dy = (y-lpStage->smLight[cnt].y)>>FLOATNS;
		dz = (z-lpStage->smLight[cnt].z)>>FLOATNS;
		dist = dx*dx+dy*dy+dz*dz;

		//smLight->Type |= smLIGHT_TYPE_DYNAMIC;
		//smLight->Type |= smLIGHT_TYPE_NIGHT;

		type = lpStage->smLight[cnt].type;

		if ( dist<0x300000 && (type&smLIGHT_TYPE_DYNAMIC) && abs(dx)<0x2000 && abs(dz)<0x2000 ) {
			if ( (lpStage->smLight[cnt].type&smLIGHT_TYPE_NIGHT) && NightDayTime ) {

				//######################################################################################
				//�� �� �� : �� �� ��
				smRender.AddDynamicLight( lpStage->smLight[cnt].type,
										  lpStage->smLight[cnt].x,lpStage->smLight[cnt].y,lpStage->smLight[cnt].z,
										  lpStage->smLight[cnt].r,lpStage->smLight[cnt].g,lpStage->smLight[cnt].b,
										  0, lpStage->smLight[cnt].Range );
				//######################################################################################
			}
			else {
				if ( DarkLevel>0 ) {
					//######################################################################################
					//�� �� �� : �� �� ��
					smRender.AddDynamicLight( lpStage->smLight[cnt].type,
											  lpStage->smLight[cnt].x,lpStage->smLight[cnt].y,lpStage->smLight[cnt].z,
											  //r,g,b,
											  (lpStage->smLight[cnt].r*DarkLevel)>>8,
											  (lpStage->smLight[cnt].g*DarkLevel)>>8,
											  (lpStage->smLight[cnt].b*DarkLevel)>>8,
											  0 , lpStage->smLight[cnt].Range );
					//######################################################################################
				}
			}
		}
	}

	return TRUE;

}




int doop = 0;



int DisplayStage(int x , int y, int z, int angX, int angY, int angZ )
{
/*
	if ( doop<100 ) {
		smStage->DrawStage( x, y, z, angX, angY, angZ );
		doop++;
		return TRUE;
	}
//smRender.RenderD3D();
//	smRender.GeomVertex2D();
	return smRender.RenderD3D();
*/



// ī�޶� ���� ��Ʈ���� ����� ( sx,sy,sz -ī�޶�  tx,ty,tz - ������ )
//int MakeTraceMatrix ( eCAMERA_TRACE *lpeTrace , int sx, int sy, int sz, int tx, int ty, int tz )
/*
	eCAMERA_TRACE	eTrace;

	MakeTraceMatrix( &eTrace , x,y,z, lpCurPlayer->pX , lpCurPlayer->pY , lpCurPlayer->pZ );

	if ( smGameStage[0] ) smGameStage[0]->DrawStage( x, y, z, eTrace.AngX, eTrace.AngY, angZ ,&eTrace.eRotMatrix );
	if ( smGameStage[1] ) smGameStage[1]->DrawStage( x, y, z, eTrace.AngX, eTrace.AngY, angZ ,&eTrace.eRotMatrix );
*/
	int cnt;
	int r,g,b,l;
/*
	int rnd;
	int dx,dy,dz,dist;

		rnd = GetCurrentTime();
		for(cnt=0;cnt<nLight;cnt++) {

//			smRender.AddDynamicLight( smLight[cnt].x,smLight[cnt].y,smLight[cnt].z,
//				smLight[cnt].r,smLight[cnt].g,smLight[cnt].b, 0 , smLight[cnt].Range );

			dx = (x-smLight[cnt].x)>>FLOATNS;
			dy = (y-smLight[cnt].y)>>FLOATNS;
			dz = (z-smLight[cnt].z)>>FLOATNS;
			dist = dx*dx+dy*dy+dz*dz;
			if ( dist<0x100000	 ) {
					smRender.AddDynamicLight( smLight[cnt].x,smLight[cnt].y,smLight[cnt].z,
						160,160,160,0,256*fONE );
			}
		}
*/


	DrawLightCnt = 0;
	DrawLightSmallCnt = 0;
/*
	smRender.SetCameraPosi( x, y, z, angX, angY, angZ );
	if ( smGameStage[0] ) 
		SetStageDynLight(smGameStage[0] , x,y,z );
	if ( smGameStage[1] ) 
		SetStageDynLight(smGameStage[1] , x,y,z );
*/

	smRender.ZWriteAuto = TRUE;

	//######################################################################################
	//�� �� �� : �� �� ��
	if( MipMapModeCreate )
		renderDevice.SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR);
	//######################################################################################
/*
	smRENDFACE *lpRenderFace[2] = {0,0};
	smRENDFACE *lpSelRendFace = 0;
	int			  DepthZ[2] = { 0,0 };
*/
 	for( cnt=0;cnt<2;cnt++ ) {
		if ( smGameStage[cnt] ) {

			//SetStageDynLight(smGameStage[cnt] , x,y,z );

			if ( DarkLevel>0 && StageField[cnt]->State==FIELD_STATE_VILLAGE ) {
				//���������� ���� ����
				r = smRender.Color_R;
				g = smRender.Color_G;
				b = smRender.Color_B;
				l = smRender.nRendLight;

				smRender.Color_R>>=1;
				smRender.Color_G>>=1;
				smRender.Color_B>>=1;
	
				if ( smRender.nRendLight>0 && smRender.smRendLight[0].Range==DarkLightRange ) {
					smRender.smRendLight[0].Range = 0;
					l = 1;
				}
				else
					l = 0;

				if ( smGameStage[cnt]->DrawStage2( x, y, z, angX, angY, angZ  )==TRUE ) {

					//if ( Moving_DblClick )
					//	DepthZ[cnt] = smRender.FindRenderFace2D( &lpRenderFace[cnt] , MousePos_DblClick.x , MousePos_DblClick.y );

					DrawLightImage( smGameStage[cnt] , x,y,z );
					smGameStage[cnt]->StageObject->Draw( x, y, z, angX, angY, angZ );
				}

				smRender.Color_R = r;
				smRender.Color_G = g;
				smRender.Color_B = b;

				if ( l ) smRender.smRendLight[0].Range=DarkLightRange;



			}
			else {
				if ( smGameStage[cnt]->DrawStage2( x, y, z, angX, angY, angZ  )==TRUE ) {
					//if ( Moving_DblClick )
					//	DepthZ[cnt] = smRender.FindRenderFace2D( &lpRenderFace[cnt] , MousePos_DblClick.x , MousePos_DblClick.y );

					DrawLightImage( smGameStage[cnt] , x,y,z );
					smGameStage[cnt]->StageObject->Draw( x, y, z, angX, angY, angZ );
				}
			}

		}
	}

/*
	if ( Moving_DblClick ) {
		lpSelRendFace = lpRenderFace[0];
		if ( !lpSelRendFace || ( lpRenderFace[1] && DepthZ[1]<DepthZ[0] ) ) {
			lpSelRendFace = lpRenderFace[1];
		}

		int x,y,z;
		int x1,z1,x2,z2;

		if ( lpSelRendFace ) {
			x=y=z=0;

			for(cnt=0;cnt<3;cnt++) { 
				x += ((smSTAGE_VERTEX *)lpSelRendFace->lpRendVertex[cnt]->lpSourceVertex)->x; 
				y += ((smSTAGE_VERTEX *)lpSelRendFace->lpRendVertex[cnt]->lpSourceVertex)->y; 
				z += ((smSTAGE_VERTEX *)lpSelRendFace->lpRendVertex[cnt]->lpSourceVertex)->z; 
			}
			x/=3;
			y/=3;
			z/=3;

			lpCurPlayer->MoveFlag = TRUE;

			x1 = lpCurPlayer->pX>>FLOATNS;
			z1 = lpCurPlayer->pZ>>FLOATNS;

			x2 = x>>FLOATNS;
			z2 = z>>FLOATNS;
		
			lpCurPlayer->Angle.y = GetRadian2D( x1 ,z1, x2, z2 );

			TracePos_DblClick.x = x;
			TracePos_DblClick.y = y;
			TracePos_DblClick.z = z;
			TraceMode_DblClick = TRUE;
		}

		Moving_DblClick = FALSE;
	}
*/


	//######################################################################################
	//�� �� �� : �� �� ��
	if( MipMapModeCreate )
		renderDevice.SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE);
	//######################################################################################

	smRender.ZWriteAuto = FALSE;


//DarkLevel	
	//return smStage->DrawStage( x, y, z, angX, angY, angZ );
/*
	for(cnt=0;cnt<2;cnt++) {
		if ( smGameStage[cnt] ) {
			DrawLightImage( smGameStage[cnt] , x,y,z );
		}
	}
*/

	if ( (MaterialFrame&0x3F)==0 ) {
	//if ( (MaterialFrame&0x7F)==21 ) {
		//�κ��丮 ������ �ڵ� Ȯ��
		CheckInvenItemCode();
	}

	return TRUE;
}

//######################################################################################
//�� �� �� : �� �� ��
void CreatePatIceFoot( int Num, int sizeX, int sizeZ )
{
	int MatShadow = CreateTextureMaterial( g_IceFootImageFileBuff[Num], 0, 0, 0, 0, SMMAT_BLEND_ALPHA );
	
	smTPOINT tPoint[4];
	tPoint[0].u = 0;
	tPoint[0].v = 0;
	tPoint[1].u = 256;
	tPoint[1].v = 0;
	tPoint[2].u = 0;
	tPoint[2].v = 256;
	tPoint[3].u = 256;
	tPoint[3].v = 256;

	smOBJ3D *objIceFoot = new smOBJ3D( 4, 2 );
	objIceFoot->AddVertex( -sizeX , -sizeZ ,0) ;
	objIceFoot->AddVertex(  sizeX , -sizeZ ,0);
	objIceFoot->AddVertex( -sizeX ,  sizeZ ,0);
	objIceFoot->AddVertex(  sizeX ,  sizeZ ,0);

	objIceFoot->AddFace( 0, 1, 2 , &tPoint[2], &tPoint[3], &tPoint[0] );
	objIceFoot->AddFace( 2, 1, 3 , &tPoint[0], &tPoint[3], &tPoint[1] );
	objIceFoot->ZeroNormals();

	objIceFoot->SetFaceMaterial( 0 , MatShadow );
	objIceFoot->SetFaceMaterial( 1 , MatShadow );

	PatIceFoot[ Num ] = new smPAT3D;
	PatIceFoot[ Num ]->AddObject( objIceFoot );
}
//######################################################################################

void CreatePatShadow( int Num , int sizeX , int sizeZ )
{
	smTPOINT tPoint[4];
	smOBJ3D *objShadow;

	int MatShadow;

	MatShadow = CreateTextureMaterial( szShadowImageFile , 0, 0, 0,0, SMMAT_BLEND_SHADOW );
	smMaterial[MatShadow].SelfIllum = 1;

	tPoint[0].u = 0;
	tPoint[0].v = 0;
	tPoint[1].u = 256;
	tPoint[1].v = 0;
	tPoint[2].u = 0;
	tPoint[2].v = 256;
	tPoint[3].u = 256;
	tPoint[3].v = 256;


	objShadow = new smOBJ3D( 4, 2 );
	objShadow->AddVertex( -sizeX , -sizeZ ,0) ;
	objShadow->AddVertex(  sizeX , -sizeZ ,0);
	objShadow->AddVertex( -sizeX ,  sizeZ ,0);
	objShadow->AddVertex(  sizeX ,  sizeZ ,0);

	objShadow->AddFace( 0, 1, 2 , &tPoint[0], &tPoint[1], &tPoint[2] );
	objShadow->AddFace( 2, 1, 3 , &tPoint[2], &tPoint[1], &tPoint[3] );
	objShadow->ZeroNormals();

	objShadow->SetFaceMaterial( 0 , MatShadow );
	objShadow->SetFaceMaterial( 1 , MatShadow );

	PatShadow[Num] = new smPAT3D;
	PatShadow[Num]->AddObject( objShadow );
}

void CreatePatShadow2( int Num , int sizeX , int sizeZ )
{
	smTPOINT tPoint[9];
	smOBJ3D *objShadow;

	int MatShadow;

	MatShadow = CreateTextureMaterial( szShadowImageFile , 0, 0, 0,0, SMMAT_BLEND_SHADOW );
	smMaterial[MatShadow].SelfIllum = 1;

	tPoint[0].u = 0;
	tPoint[0].v = 0;
	tPoint[1].u = 128;
	tPoint[1].v = 0;
	tPoint[2].u = 256;
	tPoint[2].v = 0;

	tPoint[3].u = 0;
	tPoint[3].v = 128;
	tPoint[4].u = 128;
	tPoint[4].v = 128;
	tPoint[5].u = 256;
	tPoint[5].v = 128;

	tPoint[6].u = 0;
	tPoint[6].v = 256;
	tPoint[7].u = 128;
	tPoint[7].v = 256;
	tPoint[8].u = 256;
	tPoint[8].v = 256;


	objShadow = new smOBJ3D( 9, 8 );
	objShadow->AddVertex( -sizeX , -sizeZ ,0) ;
	objShadow->AddVertex(      0 , -sizeZ ,0) ;
	objShadow->AddVertex(  sizeX , -sizeZ ,0);

	objShadow->AddVertex( -sizeX ,	    0 ,0) ;
	objShadow->AddVertex(      0 ,	    0 ,0) ;
	objShadow->AddVertex(  sizeX ,	    0 ,0);

	objShadow->AddVertex( -sizeX ,  sizeZ ,0) ;
	objShadow->AddVertex(      0 ,  sizeZ ,0) ;
	objShadow->AddVertex(  sizeX ,  sizeZ ,0);

	objShadow->AddFace( 0, 1, 3 , &tPoint[0], &tPoint[1], &tPoint[3] );
	objShadow->AddFace( 3, 1, 4 , &tPoint[3], &tPoint[1], &tPoint[4] );

	objShadow->AddFace( 1, 2, 4 , &tPoint[0], &tPoint[2], &tPoint[4] );
	objShadow->AddFace( 4, 2, 5 , &tPoint[4], &tPoint[2], &tPoint[5] );

	objShadow->AddFace( 3, 4, 6 , &tPoint[3], &tPoint[4], &tPoint[6] );
	objShadow->AddFace( 6, 4, 7 , &tPoint[6], &tPoint[4], &tPoint[7] );

	objShadow->AddFace( 4, 5, 7 , &tPoint[4], &tPoint[5], &tPoint[7] );
	objShadow->AddFace( 7, 5, 8 , &tPoint[7], &tPoint[5], &tPoint[8] );

	objShadow->SetFaceMaterial( 0 , MatShadow );
	objShadow->SetFaceMaterial( 1 , MatShadow );
	objShadow->SetFaceMaterial( 2 , MatShadow );
	objShadow->SetFaceMaterial( 3 , MatShadow );
	objShadow->SetFaceMaterial( 4 , MatShadow );
	objShadow->SetFaceMaterial( 5 , MatShadow );
	objShadow->SetFaceMaterial( 6 , MatShadow );
	objShadow->SetFaceMaterial( 7 , MatShadow );

	PatShadow[Num] = new smPAT3D;
	PatShadow[Num]->AddObject( objShadow );
}


//������ ĳ���� ���
int AdminCharMode = 0;
//������ ĳ���� ���� ���濡 ����Ÿ ���� ����
int AdminCharDisable = 0;

//ĳ���� ���� ����Ʈ
static char szDataDirectory[256];
char szCharFileList[256][256];
int CharFileCnt;
int CharPlayCnt = 0;

//���� �̸����� ���� ���丮�� ã�� ����
static char *SetDirectoryFromFile( char *filename )
{
	int len;
	int cnt;

	lstrcpy( szDataDirectory , filename );

	len = lstrlen( szDataDirectory );

	for( cnt=len; cnt>=0 ; cnt-- )
		if ( szDataDirectory[cnt]=='\\' ) break;

	szDataDirectory[cnt+1] = NULL;

	return szDataDirectory;
}

//ĳ���� ���� ����Ʈ ����
int SetCharFileList( char *szCharFile )
{
	CharFileCnt = 0;
	char szFindPath[256];

	SetDirectoryFromFile( szCharFile );

	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;

	lstrcpy( szFindPath , szDataDirectory );
	lstrcat( szFindPath , "\\*.ini" );

	//������ ã��( Ȯ���� ���� ���� )
	hFindHandle = FindFirstFile( szFindPath , &fd );
	if ( hFindHandle==INVALID_HANDLE_VALUE ) 
	{
		FindClose( hFindHandle );
		return 0;
	}
	while(1) 
	{
		lstrcpy( szCharFileList[CharFileCnt] , szDataDirectory );
		lstrcat( szCharFileList[CharFileCnt] , fd.cFileName );
		CharFileCnt++;

		if ( FindNextFile( hFindHandle , &fd )==FALSE ) 
			break;
		if ( CharFileCnt>255 )
			break;
	}
	FindClose( hFindHandle );

	return TRUE;
}

void SetNextPlayerPattern()
{
	if ( lpCurPlayer->PatLoading ) 
		return;

	//������ ĳ���� ���� ���濡 ����Ÿ ���� ����
	AdminCharDisable = TRUE;

	if ( AdminCharMode ) 
	{
		memcpy( &lpCurPlayer->smCharInfo , &smCharInfo_Back , sizeof( smCHAR_INFO ) );
		ReformCharForm();
		AdminCharMode = 0;
		lpCurPlayer->PatLoading = TRUE;
		AddLoaderPattern( lpCurPlayer , lpCurPlayer->smCharInfo.szModelName, lpCurPlayer->smCharInfo.szModelName2 );
		lpCurPlayer->DisplayTools = TRUE;
		return;
	}

	AdminCharMode = 0;
	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern( lpCurPlayer , szCharFileList[CharPlayCnt], lpCurPlayer->smCharInfo.szModelName2 );
	lpCurPlayer->DisplayTools = TRUE;

	CharPlayCnt++;
	if ( CharPlayCnt>=CharFileCnt ) CharPlayCnt = 0;
}

void SetPlayerBodyPattern( char *szBodyFile )
{
	char szBuff[256];

	if ( lpCurPlayer->PatLoading ) 
		return;

	wsprintf( szBuff , "char\\tmABCD\\%s.ini" , szBodyFile );
	FILE *fp = fopen(szBuff, "rb");
	if ( !fp ) return;
	fclose(fp);
/*
	//������ ĳ���� ���� ���濡 ����Ÿ ���� ����
	AdminCharDisable = TRUE;
	AdminCharMode = 0;
*/
	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern( lpCurPlayer , szBuff , lpCurPlayer->smCharInfo.szModelName2 );
	lpCurPlayer->DisplayTools = TRUE;
}

void SetPlayerPattern( char *szFile )
{
	if ( lpCurPlayer->PatLoading ) 
		return;

	FILE *fp = fopen(szFile, "rb");
	if ( !fp ) return;
	fclose(fp);

	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern( lpCurPlayer , szFile , 0 );
	lpCurPlayer->DisplayTools = TRUE;

	ReformCharForm();

	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	SendSetObjectSerial( lpCurPlayer->dwObjectSerial ); 

}

char szNpcFileList[64][64];
int NpcFileCnt;
int NpcPlayCnt = 0;

char *NpcSelectedName = 0;

//ĳ���� ���� ����Ʈ ����
int SetNpcFileList( char *szNpcDirectory )
{
	NpcFileCnt = 0;
	char szFindPath[256];

//	SetDirectoryFromFile( szCharFile );

	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;

	lstrcpy( szFindPath , szNpcDirectory );
	lstrcat( szFindPath , "*.npc" );

	//������ ã��( Ȯ���� ���� ���� )
	hFindHandle = FindFirstFile( szFindPath , &fd );
	if ( hFindHandle==INVALID_HANDLE_VALUE ) 
	{
		FindClose( hFindHandle );
		return 0;

	}
	while(1)
	{
		lstrcpy( szNpcFileList[NpcFileCnt] , szNpcDirectory ); 
		lstrcat( szNpcFileList[NpcFileCnt] , fd.cFileName );
		NpcFileCnt++;

		if ( FindNextFile( hFindHandle , &fd )==FALSE ) 
			break;
		if ( NpcFileCnt>64 ) 
			break;

	}
	FindClose( hFindHandle );

	return TRUE;
}

void SetNextNpcPattern()
{
	if ( lpCurPlayer->PatLoading ) 
		return;

	if ( AdminCharMode==0 )
		memcpy( &smCharInfo_Back , &lpCurPlayer->smCharInfo , sizeof( smCHAR_INFO ) );

	//������ ĳ���� ���� ���濡 ����Ÿ ���� ����
	AdminCharDisable = TRUE;

	AdminCharMode = 1;				//NPC ���� ���


	lpCurPlayer->smCharInfo.dwCharSoundCode = 0;

	//ĳ���� ���� ���Ͽ��� �ص��Ͽ� �����Ѵ�
	if ( smCharDecode( szNpcFileList[NpcPlayCnt] , &lpCurPlayer->smCharInfo ,0 ) ) {
		lpCurPlayer->PatLoading = TRUE;
		AddLoaderPattern( lpCurPlayer , lpCurPlayer->smCharInfo.szModelName );

		lpCurPlayer->DisplayTools = FALSE;
		lpCurPlayer->smCharInfo.SizeLevel = 1;

		NpcSelectedName = szNpcFileList[NpcPlayCnt];

		NpcPlayCnt++;
		if ( NpcPlayCnt>=NpcFileCnt ) NpcPlayCnt = 0;
	}
	ReformCharForm();
}

//////////////// ���� ȸ�� �Լ� ////////////////
int LaunchPartyPostion()
{
	int x,y,z,dist,ddist;
	int cnt;
	smCHAR *lpChar;

	x = lpCurPlayer->pX;
	y = lpCurPlayer->pY+48*fONE;
	z = lpCurPlayer->pZ;

	if ( sPartyPosion.code==smTRANSCODE_PARTY_POTION ) {

		if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
			CheckCharForm();
			if ( sPartyPosion.WParam ) {
				sinSetLife( sinGetLife()+sPartyPosion.WParam );
				StartEffect( x,y,z, EFFECT_POTION1 );
			}
			if ( sPartyPosion.LParam ) {
				sinSetMana( sinGetMana()+sPartyPosion.LParam );	
				StartEffect( x,y,z, EFFECT_POTION2 );
			}
			if ( sPartyPosion.SParam ) {
				sinSetStamina( sinGetStamina()+sPartyPosion.SParam );
				StartEffect( x,y,z, EFFECT_POTION3 );
			}
			ResetEnergyGraph( 3 );						//������ �׷��� ýũ �ʱ�ȭ
			ReformCharForm();
			sinPlaySound(SIN_SOUND_EAT_POTION,GetDistVolume(x,y,z) );			//����Դ� �Ҹ�

		}



		for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
			if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial && chrOtherPlayer[cnt].PartyFlag &&
				chrOtherPlayer[cnt].dwObjectSerial!=sPartyPosion.EParam && 
				chrOtherPlayer[cnt].MotionInfo->State!=CHRMOTION_STATE_DEAD &&
				chrOtherPlayer[cnt].RendSucess ) {

				x = chrOtherPlayer[cnt].pX;
				y = chrOtherPlayer[cnt].pY+48*fONE;
				z = chrOtherPlayer[cnt].pZ;

				if ( sPartyPosion.WParam ) 	StartEffect( x,y,z, EFFECT_POTION1 );
				if ( sPartyPosion.LParam ) 	StartEffect( x,y,z, EFFECT_POTION2 );
				if ( sPartyPosion.SParam ) 	StartEffect( x,y,z, EFFECT_POTION3 );
			}
		}


	}

	switch( sPartyPosion.code ) {
		case smTRANSCODE_HEALING:
			//ġ��
			if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				CheckCharForm();
				if ( sPartyPosion.WParam ) {
					sinSetLife( sinGetLife()+sPartyPosion.WParam );
				}
				ResetEnergyGraph( 0 );						//������ �׷��� ýũ �ʱ�ȭ
				ReformCharForm();
			}
			break;

		case smTRANSCODE_GRAND_HEALING:
			//�׷��� ����
			if ( sPartyPosion.EParam==lpCurPlayer->dwObjectSerial ) {
				lpChar = lpCurPlayer;
			}
			else
				lpChar = FindAutoPlayer( sPartyPosion.EParam );

			if ( !lpChar ) break;

			ddist = sPartyPosion.SParam*sPartyPosion.SParam;

			x = (lpCurPlayer->pX - lpChar->pX)>>FLOATNS;
			y = (lpCurPlayer->pY - lpChar->pY)>>FLOATNS;
			z = (lpCurPlayer->pZ - lpChar->pZ)>>FLOATNS;
			dist = x*x+y*y+z*z;
			if ( dist<ddist && lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				//��ȿ�Ÿ��� ���� ����
				if ( sPartyPosion.LParam ) {
					CheckCharForm();
					sinSetLife( sinGetLife()+sPartyPosion.LParam );
					ResetEnergyGraph( 0 );						//������ �׷��� ýũ �ʱ�ȭ
					ReformCharForm();
				}

				if ( lpChar!=lpCurPlayer )
					sinSkillEffect_Great_Healing_Party( lpCurPlayer );
			}


			//��Ƽ���� ���� ����Ʈ
			for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
				if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial && lpChar!=&chrOtherPlayer[cnt] &&
					chrOtherPlayer[cnt].PartyFlag && chrOtherPlayer[cnt].MotionInfo->State!=CHRMOTION_STATE_DEAD ) {

					x = (chrOtherPlayer[cnt].pX - lpChar->pX)>>FLOATNS;
					y = (chrOtherPlayer[cnt].pY - lpChar->pY)>>FLOATNS;
					z = (chrOtherPlayer[cnt].pZ - lpChar->pZ)>>FLOATNS;
					dist = x*x+y*y+z*z;

					if ( dist<ddist ) {
						sinSkillEffect_Great_Healing_Party( &chrOtherPlayer[cnt] );
					}
				}
			}
			break;

		case smTRANSCODE_VAMPRIC_CUSPID:
			if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				CheckCharForm();
				if ( sPartyPosion.WParam ) {
					sinSetLife( sinGetLife()+sPartyPosion.WParam );
				}
				ResetEnergyGraph( 0 );						//������ �׷��� ýũ �ʱ�ȭ
				ReformCharForm();
			}
			break;

		// �庰 - ���Ǹ� Ŀ���� EX
		case smTRANSCODE_VAMPRIC_CUSPID_EX: 
			if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				CheckCharForm();
				if ( sPartyPosion.WParam ) {
					sinSetLife( sinGetLife()+sPartyPosion.WParam );
				}
				ResetEnergyGraph( 0 );						//������ �׷��� ýũ �ʱ�ȭ
				ReformCharForm();
			}
			break;

		case smTRANSCODE_MANA_RECHARGING:
			if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
				CheckCharForm();
				if ( sPartyPosion.WParam ) {
					sinSetMana( sinGetMana()+sPartyPosion.WParam );
				}
				ResetEnergyGraph( 1 );						//������ �׷��� ýũ �ʱ�ȭ
				ReformCharForm();
			}
			break;

	}




	sPartyPosion.code = 0;
	dwPartyPosionTime = 0;


	return TRUE;
}

//���� ȸ�� ����
int SetPartyPosion( smTRANS_COMMAND *lpTransCommand )
{
	if ( dwPartyPosionTime ) {
		//�̹� ������
		LaunchPartyPostion();
	}

	dwPartyPosionTime = dwPlayTime+1000;
	memcpy( &sPartyPosion , lpTransCommand , sizeof(smTRANS_COMMAND) );

	return TRUE;
}



char szPlayerCode[32];
int CharacterJob = 0;			//���� �ڵ� �Ѱ���

char *szAnimShieldEffectFile[] = {
	"Weapons\\test1.tga",
	"Weapons\\test2.tga",
	"Weapons\\test3.tga",
	"Weapons\\test2.tga"
};

char *szBangImage = "weapons\\bang2.bmp";
char *szClowImage = "weapons\\crown.tga";

extern	char *szNpcInfoPath;

DWORD	dwSoD_ResultTime = 0;		//SoD ��Ȳâ ���� �ð�

sBILLING_INFO	*sBiInfo = 0;

int InitPat3D()
{
	int cnt,cnt2,size;
	POINT3D Posi,Angle;

//	smMODELINFO	modelInfo;
	DWORD nsize = 62;

	if ( !sBiInfo ) {
		sBiInfo = new sBILLING_INFO;
		ZeroMemory( sBiInfo , sizeof(sBILLING_INFO) );
	}

	if ( PlayerName[0]==0 )
		GetComputerName( PlayerName , &nsize );

	InterfaceParty.chatbuild_INIfiles();	//����->ģ��	�ʱ⼳��

	//�ڵ� ���̺� �ʱ�ȭ
	rsRecorder.dwRecordTime = 0;
	rsRecorder.dwNextRecordTime = GetCurrentTime()+10*60*1000;


//�������̽� ������(�±�����)
#ifdef	_LANGUAGE_THAI		//0x57
	cInterFace.sInterFlags.CameraAutoFlag = 0;			//ī�޶� ���� ( �⺻ )
#else	
	cInterFace.sInterFlags.CameraAutoFlag = 2;			//ī�޶� ���� ( �⺻ )
#endif	

	cInterFace.sInterFlags.MapOnFlag = TRUE;			//�̴ϸ� ǥ��
	cInterFace.sInterFlags.RunFlag = TRUE;				//�޸��� ���

	if ( ((GetCurrentTime()>>4)&1)==0 ) lstrcpy( szPlayerCode , "p" );
	else lstrcpy( szPlayerCode , "f" );

	//######################################################################################
	//�� �� �� : �� �� ��
	CreatePatIceFoot( 0, 6*fONE, 6*fONE );
	CreatePatIceFoot( 1, 6*fONE, 6*fONE );
	//######################################################################################

	//�׸��� �����
	CreatePatShadow(0, 12*fONE , 10*fONE );
	CreatePatShadow(1, 17*fONE , 14*fONE );
	CreatePatShadow2(2, 26*fONE , 20*fONE );
	CreatePatShadow2(3, 38*fONE , 30*fONE );

//	sizeX = 10*fONE;
//	sizeZ = 8*fONE;

	Posi.x = smConfig.Posi_Player.x;
	Posi.z = smConfig.Posi_Player.y;
	Posi.y = 0;
	Angle.x = 0;
	Angle.y = 0;
	Angle.z = 0;

//	cnt = GetInvenToryItem(100);
	invPlayerArmor = 0;//cnt;

	//ĳ���� ���� ����Ʈ ����
	//SetCharFileList( smConfig.szFile_Player );
	SetCharFileList( "char\\tmABCD\\a001.ini");
	//SetCharFileList( "char\\montest\\Skeleton.INI" );//smConfig.szFile_Player );

	//DRZ_EDIT (crash here. why do the client need to read ServerDir)
#ifdef _W_SERVER
	//ĳ���� ���� ����Ʈ ����
	SetNpcFileList( szNpcInfoPath );
#endif

	//���־��� ���� �̸� �ε�
	LoadOftenArmor();
	//������ �̸� �ε�
	LoadOftenFace();
	//���־��� ������ �̸� �ε�
	LoadOftenItem();
	//NPC �̸� �ε�
	LoadOftenNpc();

	LoadOftenModels();		//��Ÿ �� �ε�

	//�Źٶ��� ĳ���� ������ ����
	sinChar = &lpCurPlayer->smCharInfo;

	CheckCharForm();

	//�÷��̾� ����
	if ( CharacterName1[0] ) 
	{
		cnt = NewCharInfo.Dexterity+NewCharInfo.Health+NewCharInfo.Spirit+NewCharInfo.Strength+NewCharInfo.Talent;

		lpCurPlayer->smCharInfo.JOB_CODE	= NewCharInfo.JobCode;

		if ( cnt<103 ) 
		{
			//ĳ���� ����Ʈ���� �Ѿ�� �⺻ ����Ÿ ��
			lpCurPlayer->smCharInfo.Dexterity	= NewCharInfo.Dexterity;
			lpCurPlayer->smCharInfo.Health		= NewCharInfo.Health;
			lpCurPlayer->smCharInfo.Spirit		= NewCharInfo.Spirit;
			lpCurPlayer->smCharInfo.Strength	= NewCharInfo.Strength;
			lpCurPlayer->smCharInfo.Talent		= NewCharInfo.Talent;
		}
		else 
		{
			lpCurPlayer->smCharInfo.Dexterity	= 1;
			lpCurPlayer->smCharInfo.Health		= 8;//NewCharInfo.Health;
			lpCurPlayer->smCharInfo.Spirit		= 1;//NewCharInfo.Spirit;
			lpCurPlayer->smCharInfo.Strength	= 8;//NewCharInfo.Strength;
			lpCurPlayer->smCharInfo.Talent		= 1;//NewCharInfo.Talent;
			lpCurPlayer->smCharInfo.StatePoint = 0;//NewCharInfo.Talent;
		}
	}

	//�÷��̾� ����
	if ( CharacterName1[0] ) 
	{
		SetLoadPattern( lpCurPlayer , CharacterName1 , CharacterName2 , &Posi , &Angle );

		cnt2 = 0;
		for(cnt=0;cnt<(int)lpCurPlayer->smMotionInfo->MotionCount;cnt++)
		{
			if ( lpCurPlayer->smMotionInfo->MotionInfo[cnt].State==CHRMOTION_STATE_DEAD )
			{
				//�״� ���� Ȯ��
				size = lpCurPlayer->smMotionInfo->MotionInfo[cnt].EndFrame-lpCurPlayer->smMotionInfo->MotionInfo[cnt].StartFrame;
				if ( size>8 ) cnt2++;
			}

			if ( lpCurPlayer->smMotionInfo->MotionInfo[cnt].State==CHRMOTION_STATE_ATTACK ) 
			{
				size = lpCurPlayer->smMotionInfo->MotionInfo[cnt].EndFrame-lpCurPlayer->smMotionInfo->MotionInfo[cnt].StartFrame;
				if ( size<40 && !smConfig.DebugMode ) 
				{
					DisconnectFlag = GetCurrentTime();
					quit = TRUE;
				}
			}
		}

		if ( cnt2==0 && !smConfig.DebugMode ) 
		{
			//�״� ������ ���� ���
			DisconnectFlag = GetCurrentTime();
			quit = TRUE;
		}
	}
	else
		SetLoadPattern( lpCurPlayer , smConfig.szFile_Player ,0 , &Posi , &Angle );


	lpCurPlayer->smCharInfo.Level = 1;

	lpCurPlayer->smCharInfo.Attack_Damage[0] = 5;
	lpCurPlayer->smCharInfo.Attack_Damage[1] = 10;

	lstrcpy( lpCurPlayer->smCharInfo.szName , PlayerName );
	lpCurPlayer->smCharInfo.State = smCHAR_STATE_USER;				//ĳ���� �Ӽ� ����
	lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_RESTART );		//���۸��

	lpCurPlayer->smCharInfo.SizeLevel = 1;							//���� ũ��
	lpCurPlayer->PatSizeLevel = 1;									//���� ũ��

	lpCurPlayer->smCharInfo.Attack_Rating = 100;
	lpCurPlayer->smCharInfo.Next_Exp = 1000;

	lpCurPlayer->smMonsterInfo.dwAutoCharCode = 0;

	lpCurPlayer->smCharInfo.Life[0] = 1000;
	lpCurPlayer->smCharInfo.Life[1] = 1000;

	lpCurPlayer->MoveMode = TRUE;
	lpCurPlayer->MoveCnt = 0;

	PkMode = 0;				//PK��� �ʱ�ȭ

	ReformCharForm();
	

	//�ٴ� Ȯ��
	lpCurPlayer->OnStageField = -1;
	if ( smGameStage[0] ) {
		cnt = smGameStage[0]->GetFloorHeight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ , lpCurPlayer->Pattern->SizeHeight );	
		lpCurPlayer->OnStageField = 0;
	}
	if ( cnt==CLIP_OUT && smGameStage[1] ) {
		lpCurPlayer->OnStageField = 1;
	}


	for( cnt=0 ; cnt<OTHER_PLAYER_MAX; cnt++ ) {
		chrOtherPlayer[cnt].Init();
	}


	smMATERIAL *mat;

	smASE_SetPhysique(0);
	PatArrow = smASE_Read( "weapons\\arrow.ase" );
	PatEffectShield = smASE_Read( "weapons\\Test8.ASE" );
	PatSparkShield = smASE_Read( "weapons\\Mc3_sparkShield.ASE" );
	PatDivineShield = smASE_Read( "weapons\\Nt3_DivineI.ASE" );
	PatGodlyShield = smASE_Read( "weapons\\Godly_Shield.ASE" );
	PatCrown = smASE_Read( "weapons\\Crown.ASE" );				//����ĳ�� �հ�


	//PatEffectShield = smASE_Read( "weapons\\arrow.ase" );

	if ( PatEffectShield ) {
		PatEffectShield->Frame = 0;

		mat = PatEffectShield->smMaterialGroup->FindMaterialFromTextureName( szBangImage );
		if ( mat ) {
			mat->BlendType = SMMAT_BLEND_ADDCOLOR;
			mat->SelfIllum = 1;
			/*
			PatEffectShield->smMaterialGroup->ChangeMaterialToAnimation( 
				mat, szAnimShieldEffectFile , 4 , SMMAT_BLEND_LAMP , TRUE );

			mat->TextureFormState[0] = 0;
			mat->Shift_FrameSpeed = 8;
			*/

		}
	}

	if ( PatCrown ) {
		PatCrown->Frame = 0;

		mat = PatCrown->smMaterialGroup->FindMaterialFromTextureName( szClowImage );
		if ( mat ) {
			mat->BlendType = SMMAT_BLEND_ADDCOLOR;
			mat->SelfIllum = 1;
		}
	}

/*
	// ����ũ ����Ÿ �ʱ�ȭ
	smASE_SetPhysique( 0 );

	smModelDecode( smConfig.szFile_Enemy , &modelInfo );
	TmPenguin=smASE_ReadBone( modelInfo.szMotionFile );		// �ִϸ��̼� �б�
	PatPenguin=smASE_Read( modelInfo.szModelFile , modelInfo.szModelName );

	for( cnt=0;cnt<3;cnt++) {
		chrPenguin[cnt].Init();
		chrPenguin[cnt].SetPosi( smConfig.Posi_Enemy.x  , 0, smConfig.Posi_Enemy.y ,0,0,0 );
		chrPenguin[cnt].SetPattern( PatPenguin );
		chrPenguin[cnt].SetAnimatioInfo( &modelInfo );
		chrPenguin[cnt].SetTargetPosi( cnt*100 , cnt*100 );
		chrPenguin[cnt].ActionPattern = 1;
	}
*/
	// ����ũ ����Ÿ �ʱ�ȭ
	smASE_SetPhysique( 0 );

	//�׵θ��� �ڽ�
	MatEachBox = CreateTextureMaterial( "image\\cw.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );

	//�޴� ��ư �׵θ�
	MatEachMenuBox = CreateTextureMaterial( "image\\cw-1.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
	MatEachTradeButton[0] = CreateTextureMaterial( "image\\CW-trade0.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
	MatEachTradeButton[1] = CreateTextureMaterial( "image\\CW-trade1.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );

#ifdef	_NEW_PARTRADE

	//��Ƽ Ʈ���̵� ����
	lpDDS_ParTradeButton[0][0] = LoadDibSurfaceOffscreen( "image\\party\\icon-c5_.bmp" );
	lpDDS_ParTradeButton[0][1] = LoadDibSurfaceOffscreen( "image\\party\\icon-c5.bmp" );

	lpDDS_ParTradeButton[1][0] = LoadDibSurfaceOffscreen( "image\\party\\icon-c1_.bmp" );
	lpDDS_ParTradeButton[1][1] = LoadDibSurfaceOffscreen( "image\\party\\icon-c1.bmp" );

	lpDDS_ParTradeButton[2][0] = LoadDibSurfaceOffscreen( "image\\party\\icon-c2_.bmp" );
	lpDDS_ParTradeButton[2][1] = LoadDibSurfaceOffscreen( "image\\party\\icon-c2.bmp" );

	lpDDS_ParTradeButton[3][0] = LoadDibSurfaceOffscreen( "image\\party\\icon_clan_.bmp" );
	lpDDS_ParTradeButton[3][1] = LoadDibSurfaceOffscreen( "image\\party\\icon_clan.bmp" );


	lpDDS_ParTradeToolTip[0] = LoadDibSurfaceOffscreen( "image\\party\\ButtonInfo-c5.bmp" );
	lpDDS_ParTradeToolTip[1] = LoadDibSurfaceOffscreen( "image\\party\\ButtonInfo-c1.bmp" );
	lpDDS_ParTradeToolTip[2] = LoadDibSurfaceOffscreen( "image\\party\\ButtonInfo-c0.bmp" );
	lpDDS_ParTradeToolTip[3] = LoadDibSurfaceOffscreen( "image\\party\\ButtonInfo-c10.bmp" );


#endif

	//������ ������
	MatEnergyBox[0] = CreateTextureMaterial( "image\\Energy_Red.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
	MatEnergyBox[1] = CreateTextureMaterial( "image\\Energy_Blue.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );

	//����
	MatLight[0] = CreateTextureMaterial( "image\\glow01.bmp" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ADDCOLOR );
	smMaterial[ MatLight[0] ].SelfIllum = TRUE;
	MatLight[1] = CreateTextureMaterial( "image\\glow02.bmp" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ADDCOLOR );
	smMaterial[ MatLight[1] ].SelfIllum = TRUE;

	MatMonBroodImage[0] = CreateTextureMaterial( "image\\M_Nm.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
	MatMonBroodImage[1] = CreateTextureMaterial( "image\\M_Mt.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
	MatMonBroodImage[2] = CreateTextureMaterial( "image\\M_Un.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
	MatMonBroodImage[3] = CreateTextureMaterial( "image\\M_D.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
	MatMonBroodImage[4] = CreateTextureMaterial( "image\\M_mc.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );

	//���� �ʱ�ȭ
	InitWeatherSystem();

	//��ƼŬ �ʱ�ȭ
	InitParticle();
	//���� ���� �ʱ�ȭ
	InitDynLight();

	UseEtherCoreCode = 0;			//��ȯ ������ ����ڵ�

	dwPartyPosionTime =0;			//���� ȸ�� ���� �ð�

	//���� ����Ʈ �ʱ�ȭ
	InitTimeEffect();

#ifdef	_SET_PROCTECT_MEM2
	//PlaySub ����ڵ� ���� Ȯ��
	cnt = Check_PlaySubReleaseFunc();
	if ( cnt ) {
		SendSetHackUser2( 1120 , cnt );
	}
#endif

	ResetEnergyGraph( 3 );
	ResetInvenItemCode();				//�̺� ������ �ڵ� �ʱ�ȭ
	ResetInvenMoney();					//�̺��丮 �� �˻� �ʱ�ȭ

	CheckDLL_Module();

	ZeroMemory( &rsBlessCastle , sizeof(rsBLESS_CASTLE) );		//���� ĳ�� �������� �ʱ�ȭ

	return TRUE;

}

int ClosePat3D()
{
	int cnt;

	ClosePatterns();

	//######################################################################################
	//�� �� �� : �� �� ��
	if( PatArrow )
	{
        delete PatArrow;
		PatArrow = NULL;
	}

	if( PatEffectShield )
	{
		delete PatEffectShield;
		PatEffectShield = NULL;
	}
	//######################################################################################

	//######################################################################################
	//�� �� �� : �� �� ��
	for( cnt=0; cnt < ICE_FOOT_PAT_MAX; cnt++ )
	{
        if( PatIceFoot[ cnt ] )
		{
			delete PatIceFoot[ cnt ];
			PatIceFoot[ cnt ] = NULL;
		}
	}
	//######################################################################################


	for(cnt=SHADOW_PAT_MAX-1;cnt>=0;cnt--)
		if( PatShadow[cnt] ) delete PatShadow[cnt];

	//��Ƽ Ʈ���̵� ��ư ����
	if ( lpDDS_ParTradeButton[0][0] )	lpDDS_ParTradeButton[0][0]->Release();
	if ( lpDDS_ParTradeButton[0][1] )	lpDDS_ParTradeButton[0][1]->Release();
	if ( lpDDS_ParTradeButton[1][0] )	lpDDS_ParTradeButton[1][0]->Release();
	if ( lpDDS_ParTradeButton[1][1] )	lpDDS_ParTradeButton[1][1]->Release();
	if ( lpDDS_ParTradeButton[2][0] )	lpDDS_ParTradeButton[2][0]->Release();
	if ( lpDDS_ParTradeButton[2][1] )	lpDDS_ParTradeButton[2][1]->Release();
	if ( lpDDS_ParTradeButton[3][0] )	lpDDS_ParTradeButton[3][0]->Release();
	if ( lpDDS_ParTradeButton[3][1] )	lpDDS_ParTradeButton[3][1]->Release();

	if ( lpDDS_ParTradeToolTip[0] )	lpDDS_ParTradeToolTip[0]->Release();
	if ( lpDDS_ParTradeToolTip[1] )	lpDDS_ParTradeToolTip[1]->Release();
	if ( lpDDS_ParTradeToolTip[2] )	lpDDS_ParTradeToolTip[2]->Release();
	if ( lpDDS_ParTradeToolTip[3] )	lpDDS_ParTradeToolTip[3]->Release();

	if ( sBiInfo ) delete sBiInfo;

	return TRUE;
}

int PartCnt = 0;
//int LoadTest = 0;

int		MsTraceCount = 0;
int		MsTraceMode = 0;
RECT	MsSelRect;
POINT	MsSelPos;

int		SelMouseButton =0;			//��� ���õ� ���콺 ��ư

smCHAR	*lpCharSelPlayer = 0;		//���� ĳ����
smCHAR	*lpCharMsTrace = 0;			//�̵� ��ǥ ĳ����

scITEM	*lpSelItem = 0;				//���õ� ������
scITEM	*lpMsTraceItem = 0;			//�̵� ��ǥ ������

int AttFlag = 0;
int	AttCancel;						//���� ���
int	AttAutoCont = 0;				//���� ���� ���

//���� ����
int TraceAttackPlay()
{
	if ( lpCharSelPlayer ) {
		MsTraceMode = TRUE;
		lpCharMsTrace = lpCharSelPlayer;
		AttFlag = TRUE;
		if ( VRKeyBuff[VK_SHIFT] && lpCharSelPlayer->smCharInfo.State==smCHAR_STATE_ENEMY ) AttAutoCont = TRUE;
		else AttAutoCont = FALSE;


		if ( VRKeyBuff['O'] && smConfig.DebugMode )
			SkillWarriorBetserker( lpCharSelPlayer , 15 );

		return TRUE;
	}

	if ( lpSelItem ) {
		MsTraceMode = TRUE;
		lpMsTraceItem = lpSelItem;
		return TRUE;
	}

	return FALSE;

}

//���� ���
int CancelAttack()
{
	SelMouseButton = 0;
	MsTraceMode = FALSE;
	lpCharMsTrace = 0;
	lpMsTraceItem = 0;
	AttFlag = 0;

	return TRUE;
}

//�������� ���� ����ġ ���
int	DamageExp( smCHAR *lpChar , int AttackDamage )
{
	int AttackResult;

//#ifdef _LANGUAGE_CHINESE
//	lpCurPlayer->AttackExp = 0;
//	return TRUE;
//#endif

	//�����⸸ �ص� ����ġ �ö�
	if ( AttackDamage && lpChar->smCharInfo.State==smCHAR_STATE_ENEMY && 
		lpChar->smCharInfo.Brood!=smCHAR_MONSTER_USER && lpChar->smCharInfo.Life[0]>0 ) {

		AttackResult = (AttackDamage<<8)/lpChar->smCharInfo.Life[1];
		if ( AttackResult>256 ) AttackResult=256;
		AttackResult = ( lpChar->smCharInfo.Exp * AttackResult )>>8;
		AttackResult/=10;
		if ( AttackResult<=0 ) AttackResult = 1;
		lpCurPlayer->AttackExp = AttackResult;
	}

	return TRUE;
}

#define AUTO_DEAD_COUNT		(70*60*8)
int AutoDeathCounter = 0;
int FixedWidth = 200;
DWORD	dwClassDelayTime = 0;

//DWORD	dwLastModuleChk = 0;

int PlayPat3D()
{
	//DWORD dwMod;

	int cnt;
	int attack;
	int	attack_UserMonster;
	int	attack_RegState;

	int wx, wz,x,z;
	int	attackMode;
	sSKILL *lpAttackSkill;
	smWINSOCK *lpsmSock;
	int	skFlag;

#ifdef	_WINMODE_DEBUG

	if ( SkipNextField ) {
		if ( SkipNextField==1 ) {
			WarpNextField( &wx , &wz );
			lpCurPlayer->SetPosi( wx , 0, wz , 0,0,0 );
			SkipNextField = 0;
			TraceCameraPosi.x = lpCurPlayer->pX;
			TraceCameraPosi.y = lpCurPlayer->pY;
			TraceCameraPosi.z = lpCurPlayer->pZ;
			TraceTargetPosi.x = lpCurPlayer->pX;
			TraceTargetPosi.y = lpCurPlayer->pY;
			TraceTargetPosi.z = lpCurPlayer->pZ;
		}
		else {
			//Ŀ���� �ʵ�� �̵�
			WarpCustomField( &wx, &wz );
			lpCurPlayer->SetPosi( wx , 0, wz , 0,0,0 );
			SkipNextField = 0;
			TraceCameraPosi.x = lpCurPlayer->pX;
			TraceCameraPosi.y = lpCurPlayer->pY;
			TraceCameraPosi.z = lpCurPlayer->pZ;
			TraceTargetPosi.x = lpCurPlayer->pX;
			TraceTargetPosi.y = lpCurPlayer->pY;
			TraceTargetPosi.z = lpCurPlayer->pZ;

		}
	}

#endif

	//�κ��丮 �� �˻�
	//CheckInvenMoney();


	lpCurPlayer->Main();

	EachPlayerMain();					//Ȯ�� ĳ���� ����

	//�ð��� �Ǹ� ���ΰ� �ڵ����� �װ� ����
	AutoDeathCounter++;

	if ( PkMode>0 ) PkMode--;			//PK��� �ð� ī����

#ifdef _LANGUAGE_BRAZIL
	if ( VRKeyBuff[VK_CONTROL] ) PkMode = FALSE;
#else
	if ( VRKeyBuff[VK_CONTROL] ) PkMode = TRUE;
#endif

	else PkMode = FALSE;


	//���� ���� �޾Ҵ�
	if ( lpCurPlayer->PlayStunCount ) {
		if ( MsTraceMode ) {
			CancelAttack();
			lpCurPlayer->MoveFlag = 0;
			MouseButton[0] = 0;
			MouseButton[1] = 0;
			MouseButton[2] = 0;
		}
		if ( lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_WALK || lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_RUN ) {
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
			CancelAttack();
			lpCurPlayer->MoveFlag = 0;
			MouseButton[0] = 0;
			MouseButton[1] = 0;
			MouseButton[2] = 0;
		}
		lpCurPlayer->dwNextMotionCode = 0;
		lpCurPlayer->MotionLoop = 0;
	}


	if ( MsTraceMode && lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_ATTACK && lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_EAT &&
		lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_SKILL ) {

		attackMode = FALSE;
		AttCancel = FALSE;
		attack_UserMonster = FALSE;

		CheckPlaySubFunc2();

		if ((MsTraceCount&0x1F)==0 ) {
			//���콺�� ���� ĳ���� ����
			//lpCurPlayer->SetTargetPosi( 0,0 );
			lpCurPlayer->MoveFlag = TRUE;
			lpCurPlayer->Angle.y = GetMouseSelAngle();
		}

		int AttackRange = 0;
		int	AttackResult = 0;

		lpAttackSkill = 0;
		//���õ� ���콺 ��ư���� ���� ��ų ����
		switch( SelMouseButton ) {
		case 1:
			if ( sinSkill.pLeftSkill && sinSkill.pLeftSkill->CODE!=SKILL_NORMAL_ATTACK )
				lpAttackSkill = sinSkill.pLeftSkill;
			break;
		case 2:
			if ( sinSkill.pRightSkill && sinSkill.pRightSkill->CODE!=SKILL_NORMAL_ATTACK )
				lpAttackSkill = sinSkill.pRightSkill;
			break;
		}

		//�������� ���� ��ų �Ұ�
		if ( lpCurPlayer->OnStageField>=0 && StageField[ lpCurPlayer->OnStageField ]->State==FIELD_STATE_VILLAGE )
			lpAttackSkill = 0;

		if ( lpCharMsTrace ) {
			if ( lpCharMsTrace->smCharInfo.Life[0]<=0 || 
				(lpCharMsTrace->dwLastTransTime+DISPLAY_TRANS_TIME_OVER)<dwPlayTime ||
				cTrade.OpenFlag ) {
				//�̹� �׾��� / ȭ�� �ۿ��� �����
				CancelAttack();
			}
			else {
				//if ( !MouseButton[0] ) AttFlag = 0;
				if ( !MouseButton[0] && !MouseButton[1] ) AttFlag = 0;

				if ( lpCurPlayer->smCharInfo.State==smCHAR_STATE_ENEMY ) {
					//���ΰ� ���� ��Ų
					attackMode = TRUE;
				}
				else {
					if ( PkMode )
						attackMode = TRUE;
					//VRKeyBuff[VK_CONTROL];
				}

				if ( lpCharMsTrace->smCharInfo.dwCharSoundCode==snCHAR_SOUND_KELVEZU ) {
					AttackRange = 130*fONE;							//�Ǽ� ���� �Ÿ� 
				}
				else
					AttackRange = 50*fONE;							//�Ǽ� ���� �Ÿ� 

				if ( lpCurPlayer->dwActionItemCode ) {
					AttackRange += lpCurPlayer->AttackToolRange;	//���� ���� ����
				}

				//���Ÿ� ���� �Ÿ�
				if ( lpCurPlayer->ShootingMode && lpCharMsTrace->smCharInfo.State ) {
					if ( lpCharMsTrace->smCharInfo.State!=smCHAR_STATE_USER || attackMode )
						AttackRange = lpCurPlayer->smCharInfo.Shooting_Range*fONE;

					//�ӽ� ����̿� ������ �����Ÿ�
					//if ( lpCurPlayer->smCharInfo.JOB_CODE>=7 ) 
					//	AttackRange = 60*fONE;
				}

				if ( lpCharMsTrace->smCharInfo.dwCharSoundCode==snCHAR_SOUND_CASTLE_TOWER_B )
					AttackRange += 40*fONE;	//���Ҷ�ž ���� ����

				attack = lpCharMsTrace->smCharInfo.State;

				if ( lpCharMsTrace->smCharInfo.State==smCHAR_STATE_USER ) {
					if ( !attackMode ||
						( lpCurPlayer->OnStageField>=0 && StageField[ lpCurPlayer->OnStageField ]->State==FIELD_STATE_VILLAGE) )
					attack = 0;
				}

				if ( lpCharMsTrace->smCharInfo.State==smCHAR_STATE_ENEMY && lpCharMsTrace->smCharInfo.Brood==smCHAR_MONSTER_USER ) {
					if ( !PkMode )	{
						attack_UserMonster = TRUE;;
						attack = 0;
					}
				}

				if ( lpAttackSkill ) {
					if ( lpAttackSkill->CODE==SKILL_HEALING || lpAttackSkill->CODE==SKILL_ENCHANT_WEAPON || 
						lpAttackSkill->CODE==SKILL_VIRTUAL_LIFE || lpAttackSkill->CODE==SKILL_TRIUMPH_OF_VALHALLA ) 
					{
						if ( attack_UserMonster && lpAttackSkill->CODE==SKILL_HEALING )		//��ȯ���� ���� �����ϵ��� ó��
							attack_UserMonster = 0;

						skFlag = 1;
					}
					else
						skFlag = 0;

					if ( ( lpCharMsTrace->smCharInfo.State==smCHAR_STATE_ENEMY && !attack_UserMonster )  || 
						( PkMode && lpCharMsTrace->smCharInfo.State==smCHAR_STATE_USER ) || skFlag  ) {

						//��ų ���� �Ÿ��� ���Ѵ�
						cnt = GetSkillDistRange( lpAttackSkill , AttackRange , attack );
						if ( cnt )
							AttackRange = cnt;

						attack = 0;			//��ų�� �亻 ���� ����
					}
				}

				if ( !lpCurPlayer->ShootingFlag && 
					lpCurPlayer->PlayAttackFromPosi( 
							lpCharMsTrace->pX ,
							lpCharMsTrace->pY ,
							lpCharMsTrace->pZ ,
							AttackRange , attack )==TRUE && 
							lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_EAT ) {//&& 
							//!attack_UserMonster ) {
					
					LastAttackDamage = 0;			//�ֽ� ���� ������ �ʱ�ȭ
					AutoMouse_Attack_Count ++;		//���丶�콺 ������ ī����

					if ( lpCharMsTrace->smCharInfo.State==smCHAR_STATE_USER ) {
						if ( lpCharMsTrace->dwTradeMsgCode ) {		//���λ��� ���� ����
							Send_ShowMyShopItem(lpCharMsTrace);
						}
						else {
							EachTradeButton = 1;

#ifdef  __CLANSUBCHIP__
							if ( (cldata.myPosition==101 || cldata.myPosition==104) && !lpCharMsTrace->smCharInfo.ClassClan )
#else
							if ( cldata.myPosition==101 && !lpCharMsTrace->smCharInfo.ClassClan )
#endif
								EachTradeButtonMode = TRUE;						//Ŭ������ Ŭ���� ����
							else
								EachTradeButtonMode = FALSE;					//Ŭ�� ���� �޴� ����
						}

					}
					else {
						EachTradeButton = 0;
					}

					if ( !lpCharMsTrace->smCharInfo.State ) {
						OpenEachPlayer( lpCharMsTrace );
						DispEachMode = TRUE;
					}

					lpCurPlayer->chrAttackTarget = lpCharMsTrace;
					lpCurPlayer->AttackExp = 0;
				/*
					if ( ((lpCurPlayer->dwActionItemCode^sinWM1)&sinITEM_MASK2)!=0 &&	//���������� ũ��Ƽ�� ����
						(rand()%100)<sinGetCritical( lpCharMsTrace->smCharInfo.Level ) ) {

						lpCurPlayer->AttackCritcal = TRUE;			//ũ��Ƽ�� ���� ����
					}
					else
						lpCurPlayer->AttackCritcal = FALSE;
				*/
					lpCurPlayer->AttackCritcal = FALSE;

					//���� �ܿ����� ���� ��
					if ( !attack_UserMonster && 
						( lpCharMsTrace->smCharInfo.State!=smCHAR_STATE_USER ||
						(lpCurPlayer->OnStageField>=0 && StageField[ lpCurPlayer->OnStageField ]->State!=FIELD_STATE_VILLAGE) ) ) {

						AttackResult = TRUE;

						//��ų ��� ����
						if ( !PlaySkillAttack( lpAttackSkill , lpCharMsTrace ) ) {
							//�߻��� ���� �����Լ�
							if ( attack || lpCharMsTrace->smCharInfo.State!=smCHAR_STATE_USER ) {

								if ( lpCurPlayer->ShootingMode && lpCharMsTrace->smCharInfo.State!=smCHAR_STATE_NPC ) {
									if ( lpCurPlayer->CheckShootingTest(lpCharMsTrace)==TRUE ) {
										//AttackResult = lpCurPlayer->SendTransAttack( lpCharMsTrace, smWsockServer , 0,0, 0 );
										AttackResult =  dm_SendTransDamage( lpCharMsTrace , 0,0 , 0, 0, 
											lpCurPlayer->AttackSkil , TRUE );

									}
									else {
										lpCurPlayer->AttackCritcal = -1;				//������
										lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_STAND );
									}
								}
								else {
									attack_RegState = 0;

									if ( lpCharMsTrace->smCharInfo.State==smCHAR_STATE_NPC ) {
										if (sinQuest_ChangeJob2.CODE) {
											//����Ʈ ����
											attack_RegState = (sinQuest_ChangeJob2.CODE<<16);		//����Ʈ �ڵ�
											if ( sinQuest_ChangeJob2.CODE==SIN_QUEST_CODE_CHANGEJOB2_NPC_M ||
												sinQuest_ChangeJob2.CODE==SIN_QUEST_CODE_CHANGEJOB2_NPC_D ) {
													//���� ����
													if ( !cInvenTory.SearchItemCode( sinMA1|sin01 ) &&
														!cInvenTory.SearchItemCode( sinQT1|sin04 ) && 
														!cInvenTory.SearchItemCode( sinQT1|sin05 ) )
														attack_RegState|=1;

											}
										}

										//if ( smConfig.DebugMode ) {		//����� ��忡���� �׽�Ʈ��
											if ( dwClassDelayTime<dwPlayTime ) {
												if ( lpCharMsTrace->smCharInfo.wPlayClass[0] ) {
													switch( lpCharMsTrace->smCharInfo.wPlayClass[0] ) {
														//NPC ����Ʈ ó��
														case 1001:
															if (lpCurPlayer->smCharInfo.JOB_CODE <= 4 || lpCurPlayer->smCharInfo.JOB_CODE == 9)
																sinCheckLevelQuest30();
															break;
														case 1002:
															if (lpCurPlayer->smCharInfo.JOB_CODE > 4 && lpCurPlayer->smCharInfo.JOB_CODE != 9)
																sinCheckLevelQuest30();
															break;
														case 1003:
															if (lpCurPlayer->smCharInfo.JOB_CODE <= 4 || lpCurPlayer->smCharInfo.JOB_CODE == 9)
																sinCheckLevelQuest55();
															break;
														case 1004:
															if (lpCurPlayer->smCharInfo.JOB_CODE > 4 && lpCurPlayer->smCharInfo.JOB_CODE != 9)
																sinCheckLevelQuest55_2();
															break;
														case 1005:
															sinCheckLevelQuest70();
															sinCheckLevelQuest80();
															sinCheckLevelQuest85();
															break;
														case 1006:
															sinCheckLevelQuest90();
															break;

														case 1010:
															sinCheckLevelQuest80_2(0);
															break;
														case 1011:
															sinCheckLevelQuest80_2(1);
															break;
														case 1012:
															sinCheckLevelQuest80_2(0);
															sinCheckLevelQuest90_2(0);
															break;
														case 1013:
															sinCheckLevelQuest90_2(1);
															chaQuest.showFuryQuest();
															break;
													}

													//Ha����Ʈ NPC ���������� ó��
													HaCheckQuestNpc( lpCharMsTrace->smCharInfo.wPlayClass[0] );

													dwClassDelayTime = dwPlayTime+1000*5;		//5�� ������
												}
											}	
										//}

									}

									//AttackResult = lpCurPlayer->SendTransAttack( lpCharMsTrace, smWsockServer , 0,0, attack_RegState );
									AttackResult =  dm_SendTransDamage( lpCharMsTrace , 0,0 , 0, attack_RegState, 
											lpCurPlayer->AttackSkil , TRUE );

								}

								// �庰 - ��ų ����
								if ( (AttackResult && lpCurPlayer->WeaponEffect==(sITEMINFO_LIGHTING+1) || AttackResult && lpCurPlayer->WeaponEffect==(sITEMINFO_ICE+1) || AttackResult && lpCurPlayer->WeaponEffect==(sITEMINFO_FIRE+1) )
									&& lpCurPlayer->EnchantEffect_Point>0 && lpCurPlayer->EnchantEffect_Point<=10 ) {
									//����Ʈ�� �Ӽ��� ���� ���� �߰�
									/*
									EnchantPower = GetRandomPos( 
										Enchant_Weapon_Damage_Lightning[lpCurPlayer->EnchantEffect_Point-1][0] ,
										Enchant_Weapon_Damage_Lightning[lpCurPlayer->EnchantEffect_Point-1][1] );

									Skil_RangeAttack( lpCharMsTrace->pX, lpCharMsTrace->pY, lpCharMsTrace->pZ, 50*fONE , EnchantPower , 105 );
									*/
									if ( lpCurPlayer->AttackCritcal>=0 ) {
										//������ ���� ���� ���� (��)
										dm_SelectRange( lpCharMsTrace->pX, lpCharMsTrace->pY, lpCharMsTrace->pZ, 50 , TRUE );
										//�������� ������ ����
										dm_SendRangeDamage( lpCharMsTrace->pX, lpCharMsTrace->pY, lpCharMsTrace->pZ, 0 ,0, 0, 0, 0, 
											(SKILL_PLAY_ENCHANT_WEAPON|(lpCurPlayer->EnchantEffect_Point<<8)) );
									}
								}
							}
							if ( !lpAttackSkill && !AttackResult ) {
								lpCurPlayer->AttackCritcal = -1;
							}
							else {
								//�������� ���� ����ġ ���
								DamageExp( lpCharMsTrace , AttackResult );
							}
						}
					}
					/*
					if ( LastAttackDamage==0 ) {
						lpCurPlayer->AttackCritcal = -1;
					}
					*/
				
					if ( !AttackResult )	lpCurPlayer->AttackCritcal = -1;

					if ( !AttCancel ) {

						lpCurPlayer->MoveFlag = FALSE;
						lpCurPlayer->CriticalLog = lpCurPlayer->AttackCritcal;

						if (lpCharMsTrace && ( lpCharMsTrace->smCharInfo.State==smCHAR_STATE_NPC || lpCharMsTrace->dwTradeMsgCode  ) ) {
							MouseButton[0] = 0;
							MouseButton[1] = 0;
							AttAutoCont = FALSE;
						}

						if( !AttFlag && !AttAutoCont ) {
							MsTraceMode = 0;
							lpCharMsTrace = 0;
						}
					}
				}
				else {
					if ( !lpCharMsTrace->Flag || lpCharMsTrace->MotionInfo->State == CHRMOTION_STATE_DEAD ) {
						lpCharMsTrace = NULL;
						MsTraceMode = 0;
						lpCurPlayer->MoveFlag = FALSE;
						AttAutoCont = FALSE;
					}
				}
			}
		}

		if ( lpMsTraceItem ) {
			if ( lpCurPlayer->PlayAttackFromPosi( 
						lpMsTraceItem->pX ,
						lpMsTraceItem->pY ,
						lpMsTraceItem->pZ ,
						8000 , FALSE )==TRUE ) 
			{
				//lpCurPlayer->SendTransAttack( &chrOtherPlayer[MsTracePlayer], smWsockServer , 0);
				//�������� ���Ϸ��� ��
				smTRANS_ACITON_ITEM		TransActionItem;
				char	szBuff[64];
				int		len;

				TransActionItem.code = smTRANSCODE_PUTITEM;
				TransActionItem.size = sizeof(smTRANS_ACITON_ITEM);

				TransActionItem.x = lpMsTraceItem->pX;
				TransActionItem.y = lpMsTraceItem->pY;
				TransActionItem.z = lpMsTraceItem->pZ;
				TransActionItem.lpStgArea = lpMsTraceItem->lpStgArea;


				//�ӽ������ ������ ť�� ������ ��������� �˻�
				if ( CheckRecvTrnsItemQue()==TRUE ) {

					cnt = 0;

					if ( (lpMsTraceItem->ItemCode&sinITEM_MASK2)==sinCH1 )
					{
						switch( lpMsTraceItem->ItemCode ) 
						{
							#ifdef _LANGUAGE_ENGLISH
							case (sinCH1|sin01):
								if ( lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_ATALANTA || 
									lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_ARCHER	||
									lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_PRIESTESS )
								{
									cMessageBox.ShowMessage(MESSAGE_NOT_PICKUP_MALE);
									cnt = 1;										
								}
								break;
							case (sinCH1|sin02):
								if ( lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_MECHANICIAN  ||
									 lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_FIGHTER		||
									 lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_PIKEMAN		||
									 lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_KNIGHT		||
									 lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_MAGICIAN		) {
										
										cMessageBox.ShowMessage(MESSAGE_NOT_PICKUP_FEMALE);
										cnt = 1;										
									}
								break;
							//DRZ_NEWCLASS (add defintion for new classes)
#else
							case (sinCH1|sin01):
							case (sinCH1|sin02):
								//���ڷ�
								if ( lpCurPlayer->smCharInfo.JOB_CODE!=JOBCODE_ATALANTA && 
									lpCurPlayer->smCharInfo.JOB_CODE!=JOBCODE_ARCHER &&
									lpCurPlayer->smCharInfo.JOB_CODE!=JOBCODE_PRIESTESS ) {
										
										cMessageBox.ShowMessage(MESSAGE_NOT_PICKUP_FEMALE);
										cnt = 1;										
									}
								break;
#endif
							case (sinCH1|sin03):
							case (sinCH1|sin04):
								//����
								if ( lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_ATALANTA || 
									lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_ARCHER ||
									lpCurPlayer->smCharInfo.JOB_CODE==JOBCODE_PRIESTESS ) {
										
										cMessageBox.ShowMessage(MESSAGE_NOT_PICKUP_MALE);
										cnt = 1;
										
									}
								break;


						}
					}

					if ( !cnt ) {
						if ( lpMsTraceItem->ItemCode==(sinGG1|sin01) ) {
							lstrcpy( szBuff , lpMsTraceItem->szName );
							len = lstrlen( szBuff );
							for(cnt=0;cnt<len;cnt++) {
								if ( szBuff[cnt]<'0' || szBuff[cnt]>'9' ) {
									szBuff[cnt] = 0;
									break;
								}
							}
							cnt = atoi( szBuff );			//�̸����� �ݾ��� ��´�
							if ( cInvenTory.CheckMoneyLimit(cnt)==TRUE ) {
								lpsmSock = GetServerSock(lpMsTraceItem->ServerCode);		//�������� ������ ã��
								if ( lpsmSock ) {
									lpsmSock->Send2( (char *)&TransActionItem , TransActionItem.size , TRUE );
								}
							}
						}
						else {
							lpsmSock = GetServerSock(lpMsTraceItem->ServerCode);		//�������� ������ ã��
							if ( lpsmSock ) {
								lpsmSock->Send2( (char *)&TransActionItem , TransActionItem.size , TRUE );
							}
						}
					}
				}

				MsTraceMode = 0;
				lpMsTraceItem = NULL;
				lpCurPlayer->MoveFlag = FALSE;
				MouseButton[0] = 0;
			}
			else {
				if ( !lpMsTraceItem->Flag ) {
					lpMsTraceItem=NULL;
					MsTraceMode = 0;
					lpCurPlayer->MoveFlag = FALSE;
				}
			}
		}


	}
	MsTraceCount++;

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag ) {
			chrOtherPlayer[cnt].Main();
			if ( chrOtherPlayer[cnt].dwTarget ) {
				if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_DAMAGE && 
					chrOtherPlayer[cnt].MotionInfo->State==CHRMOTION_STATE_ATTACK ) 
//					lpCurPlayer->ChangeMotion( 67 );
					lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
			}
		}
	}


#ifdef _WINMODE_DEBUG
	//######################################################################################
	//�� �� �� : �� �� ��
	//if ( smConfig.DebugMode && VRKeyBuff[VK_SHIFT] ) {
	if ( smConfig.DebugMode && VRKeyBuff[VK_SHIFT] && ! VRKeyBuff[VK_CONTROL] ) {	
	//######################################################################################
		if ( VRKeyBuff[VK_F4] ) {
			SetNextPlayerPattern();
			VRKeyBuff[VK_F4] = 0;
		}
		if ( VRKeyBuff[VK_F3] ) {
			SetNextNpcPattern();
			VRKeyBuff[VK_F3] = 0;
		}
	}
#endif
	//DRZ_EDIT (remove for prevent crashing)
	//Check_CodeSafe2();				//���α׷� �ڵ�˻�

	//�ʵ� �̴� ��
	MainFieldMap();

	//��ƼŬ ����
	PlayParticle();
	//���� ���� ����
	DynLightMain();

	BGMain();		//��� ����

	//������� ����
	BGM_Main();

	//SoD ����
	SodScoreMain();


	PlayCounter ++;
	//�ʵ� �ڵ� ���� ����
	FieldMain( lpCurPlayer->pX, 0, lpCurPlayer->pZ );


	int fldSky,fflag;

	int	RainTime = 0x200;		//����� �ð�
	int	RainLength = 60*7;		//7�ð�
	int	BackSel = 0;

	fldSky = -1;
	fflag = 0;

	if ( (PlayCounter & 0xFF)==0 && lpCurPlayer->dwObjectSerial ) {
/*
				if ( cldata.myPosition==100 || cldata.myPosition==101 ) {
					//Ŭ�� ���Կ�
					lpCurPlayer->smCharInfo.ClassClan = cldata.intClanMark;
				}
				else
					lpCurPlayer->smCharInfo.ClassClan = 0;

				if ( cldata.myPosition )
					InitClanMode = cldata.myPosition;
*/
/*
		if ( !InitClanMode && cldata.myPosition ) {
			//Ŭ�� ����Ÿ �Լ�
			if ( cldata.myPosition==100 || cldata.myPosition==101 ) {
				//Ŭ�� ���Կ�
				lpCurPlayer->smCharInfo.ClassClan = cldata.intClanMark;
				lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
				SendSetObjectSerial( lpCurPlayer->dwObjectSerial );
			}
			InitClanMode = cldata.myPosition;
		}
*/


		if ( dwSoD_ResultTime<dwPlayTime ) {
			//SoD ��Ȳâ �� ����
			dwSoD_ResultTime = dwPlayTime+12000;		//12�� ����
			if ( cSinHelp.GetSodRankBoxState()==TRUE ) {
				SendCommand_AreaServer( smTRANSCODE_SOD_RESULT2 , 0 , 0 , 0 , 0 );		//SoD ��� ���� �䱸
			}
		}

		if ( cldata.myPosition ) {
			if ( lpCurPlayer->smCharInfo.ClassClan!=cldata.intClanMark ) {
#ifdef  __CLANSUBCHIP__				
				if ( cldata.myPosition==100 || cldata.myPosition==101 || cldata.myPosition==104) {
#else
				if ( cldata.myPosition==100 || cldata.myPosition==101 ) {
#endif
					//Ŭ�� ���Կ�
					lpCurPlayer->smCharInfo.ClassClan = cldata.intClanMark;
					lpCurPlayer->dwClanManageBit = cldata.CNFlag;
					lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
					SendUpdateClanInfo();					//������ ����� Ŭ�� ���� ����

					//SendSetObjectSerial( lpCurPlayer->dwObjectSerial );

					//�׽�Ʈ
					//Ŭ���� ���� ���ۿ� �Լ�
					//SendClanCommandUser( smWsockDataServer , smTRANSCODE_CLAN_UPDATE  , 0,0, cldata.clanWon , cldata.szRegiD char **szClanUserList , int ClanUserCount )

				}
				else {
					cldata.intClanMark = 0;
					lpCurPlayer->smCharInfo.ClassClan = 0;
					lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
					//SendSetObjectSerial( lpCurPlayer->dwObjectSerial );
					SendUpdateClanInfo();					//������ ����� Ŭ�� ���� ����
				}
			}
		}


		if ( lpCurPlayer->smCharInfo.wPlayerKilling[0]>0 ) {
			lpCurPlayer->smCharInfo.wPlayerKilling[0]--;
			if ( lpCurPlayer->smCharInfo.wPlayerKilling[0]==0 ) {

				//�������� �ٽ� ����
				lpCurPlayer->MoveFlag = FALSE;
				WarpStartField( &x,&z );
				lpCurPlayer->SetPosi( x,0, z ,0,0,0 );
				TraceCameraPosi.x = lpCurPlayer->pX;
				TraceCameraPosi.y = lpCurPlayer->pY;
				TraceCameraPosi.z = lpCurPlayer->pZ;
				TraceTargetPosi.x = lpCurPlayer->pX;
				TraceTargetPosi.y = lpCurPlayer->pY;
				TraceTargetPosi.z = lpCurPlayer->pZ;
				lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_RESTART );		//���۸��
				StartEffect( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, EFFECT_GAME_START1 );
				SetDynLight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 100, 100,100,0, 200 );
				CharPlaySound( lpCurPlayer );

				//�ٴ� Ȯ��
				lpCurPlayer->OnStageField = -1;
				if ( smGameStage[0] ) {
					cnt = smGameStage[0]->GetFloorHeight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ ,lpCurPlayer->Pattern->SizeHeight );	
					lpCurPlayer->OnStageField = 0;
				}
				if ( cnt==CLIP_OUT && smGameStage[1] ) {
					lpCurPlayer->OnStageField = 1;
				}
			}
		}

#ifndef _LANGUAGE_CHINESE	//������ ���
#ifndef _LANGUAGE_VEITNAM	//������ ���
#ifndef _LANGUAGE_PHILIPIN	//������ ���
#ifndef _LANGUAGE_ENGLISH	//Windowed Mode
#ifdef _WINMODE_DEBUG
		if ( !smConfig.DebugMode && (PlayCounter&0x3FF)==0 ) {
			if ( GetSystemMetrics( SM_CXSCREEN )!=WinSizeX ) {
				DisconnectFlag = dwPlayTime;
				quit = TRUE;
			}
		}
#else
		if ( GetSystemMetrics( SM_CXSCREEN )!=WinSizeX ) {
			DisconnectFlag = dwPlayTime;
			quit = TRUE;
		}
#endif
#endif
#endif
#endif
#endif

		if ( WeatherMode==0 ) {
			if ( dwGameHour<GAME_HOUR_DAY || dwGameHour>=GAME_HOUR_NIGHT ) {
				BackSel = 2;
			}
			else {
				if ( dwGameHour>=GAME_HOUR_DAY && dwGameHour<GAME_HOUR_GLOW ) 
					BackSel = 0;
				else
					BackSel = 1;
			}
		}

		//������� �ٲٱ�
		if ( lpCurPlayer->OnStageField>=0 ) {

			if ( StageField[ lpCurPlayer->OnStageField ]->FieldCode==rsSOD_FIELD ) {
				//SOD �ʵ� �ϴü���
				cnt = rsGetSoD_AreaCode( lpCurPlayer->pX , lpCurPlayer->pY, lpCurPlayer->pZ );
				switch( cnt ) {
					case 0:
					case 1:
					case 2:
					case 3:
						fldSky = FIELD_BACKIMAGE_SODNONE;
						break;

					case 4:
						fldSky = FIELD_BACKIMAGE_SODSUN;
						break;
					case 5:
						fldSky = FIELD_BACKIMAGE_SODMOON;
						break;
					case 6:
						fldSky = FIELD_BACKIMAGE_SODSKY;
						break;
				}
				if ( cnt==2 )	
					esPlayEffectBgm( 4 );		//�ٶ��� ��
				else
					esPlayEffectBgm( 3 );		//�Ϲ� ��

				if ( fldSky>=0 ) ChangeSky(fldSky);
			}
			else if ( StageField[ lpCurPlayer->OnStageField ]->FieldCode==BabelPlayField && dwBabelPlayTime>dwPlayTime ) {
				//���̾� �ٺ� ����
				ChangeSky(FIELD_BACKIMAGE_IRONBOSS);
				esPlayEffectBgm( 11 );		//�ٺ�
			}
			else {
				fldSky = StageField[ lpCurPlayer->OnStageField ]->BackImageCode[BackSel];

				if ( !WeatherMode && fldSky>=0 ) ChangeSky(fldSky);	//ChangeBG(fldSky);

				if ( WeatherMode==0 ) {
					switch( StageField[lpCurPlayer->OnStageField]->State ) {
					//��� ȿ���� (�ٶ��Ҹ�)
					case FIELD_STATE_RUIN:
						esPlayEffectBgm( 3 );
						break;

					case FIELD_STATE_DESERT: 
						esPlayEffectBgm( 4 );
						break;

					case FIELD_STATE_DUNGEON: 
						if ( StageField[lpCurPlayer->OnStageField]->FieldCode<=16 || 
							StageField[lpCurPlayer->OnStageField]->FieldCode==32 ) {
							esPlayEffectBgm( 5 );		//����
							break;
						}
						if ( StageField[lpCurPlayer->OnStageField]->FieldCode<=23 ) {
							esPlayEffectBgm( 6 );		//����
							break;
						}
						if ( StageField[lpCurPlayer->OnStageField]->FieldCode<=25 ) {
							esPlayEffectBgm( 7 );		//����
							break;
						}
						if ( StageField[lpCurPlayer->OnStageField]->FieldCode<=26 ) {
							esPlayEffectBgm( 8 );		//����
							break;
						}
						break;


					case FIELD_STATE_FOREST:
						if ( dwGameHour<GAME_HOUR_DAY || dwGameHour>=GAME_HOUR_DARKNESS ) {
							esPlayEffectBgm( 2 );			//��
						}
						else {
							esPlayEffectBgm( 1 );			//��
						}
						break;

					case FIELD_STATE_VILLAGE:
						esPlayEffectBgm( 0 );
						break;

					case FIELD_STATE_ROOM:
						esPlayEffectBgm( -1 );
						break;

					case FIELD_STATE_IRON:
						esPlayEffectBgm( 9 );		//���̾�
						break;

					case FIELD_STATE_ICE: 
						esPlayEffectBgm( 12 );
						break;

					}
				}
				else
					esPlayEffectBgm( -1 );

				if ( StageField[lpCurPlayer->OnStageField ]->BackMusicCode )
					ChangeBGM( StageField[ lpCurPlayer->OnStageField ]->BackMusicCode );
			}
				
		}
	}

	if ( (PlayCounter&0x3)==0 ) {
		//�� ���� ��ȭ
		if ( StageField[lpCurPlayer->OnStageField]->State!=FIELD_STATE_ROOM &&
		(dwGameHour<GAME_HOUR_DAY || dwGameHour>=GAME_HOUR_DARKNESS || 
			StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_DUNGEON) )
		{
			NightDayTime = 1;
		}
		else
		{
			NightDayTime = 0;
		}
				
		cnt = DarkLight;
		if( DarkLevel < cnt)
			DarkLevel++;
		if( DarkLevel > cnt)
			DarkLevel--;

		if ( (PlayCounter&0x7)==0 ) {
			if ( BackColor_mR<BackColor_R ) BackColor_R--;
			if ( BackColor_mR>BackColor_R ) BackColor_R++;

			if ( BackColor_mG<BackColor_G ) BackColor_G--;
			if ( BackColor_mG>BackColor_G ) BackColor_G++;

			if ( BackColor_mB<BackColor_B ) BackColor_B--;
			if ( BackColor_mB>BackColor_B ) BackColor_B++;
		}

		if ( smConfig.DebugMode ) {
			if ( DarkLevel_Fix ) DarkLevel = DarkLevel_Fix;
		}
/*
		if ( VRKeyBuff['9'] ) {
			BackColor_mR = 20;
			BackColor_mG = -10;
			BackColor_mB = -10;
			DarkLevel = 220;
		}

		if ( VRKeyBuff['8'] ) {
			BackColor_mR = 20;
			BackColor_mG = 20;
			BackColor_mB = -20;
			DarkLevel = 0;
		}

		if ( VRKeyBuff['7'] ) {
			BackColor_mR = 0;
			BackColor_mG = 0;
			BackColor_mB = 0;
		}
*/

		//���� ���� ī����
		if ((PlayCounter&0xFF)==0) {
/*
			dwMod = GetModuleCheckSum( "wsock32.dll" );
			if ( dwLastModuleChk && dwLastModuleChk!=dwMod ) {
				dwLastModuleChk = dwMod;
			}
			dwLastModuleChk = dwMod;
*/
			//���� ��ũ ���� ī���� ���
			Ptect_GetAttackCount();

			if ( (PlayCounter&0xFFF)==0 ) {
				//�κ��丮 ������ ������ ���� ������
				if ( (PlayCounter&0x3FFF)==0 )
					SendUsingItemListToServer2();
				else
					SendUsingItemListToServer();
			}

			//����� ���� �ϴ� ���� (���Ŀ� �ٽ�)

			// pluto �ؿ� ��
			if( smConfig.WeatherSwitch )
			{
				cnt = dwGameWorldTime&0x1FFF;//4000

				if ( cnt>RainTime && cnt<(RainTime+RainLength) ) {
					if ( cnt>RainTime+60 && cnt<(RainTime+RainLength-60) ) {
						if ( WeatherMode!=2 && (lpCurPlayer->OnStageField>=0 && 
							 StageField[lpCurPlayer->OnStageField]->State!=FIELD_STATE_DUNGEON  && 
							 StageField[lpCurPlayer->OnStageField]->FieldCode!=rsSOD_FIELD		&&
							 StageField[lpCurPlayer->OnStageField]->State != FIELD_STATE_ICE) ) {

							PlayWav( 1 , 1 , 400 , 2205 );
							WeatherMode = 2;	//���� ���
							ChangeSky(0);	//ChangeBG(0);			//�帰 �ϴ�
						}
					}
					else {
						switch(WeatherMode) {
						case 0:
							ChangeSky(0);	//ChangeBG(0);			//�帰 �ϴ�
							break;
						case 2:
							StopWav( 1 );
							break;
						}

						WeatherMode = 1;		//���� �帲
					}
				}
				else
					WeatherMode = 0;		//���� ����
			}


			if ( lpCurPlayer->smCharInfo.dwEventTime_T ) {
				if ( GetPlayTime_T()>lpCurPlayer->smCharInfo.dwEventTime_T ) {
					//Ŀ�� �Ӹ��� �������� �����ش�
					ChangeBigHeadMode( 0 , 0 );
					StartSkill( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 0,0,0, SKILL_UP1 );
					SkillPlaySound( SKILL_SOUND_LEARN , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );			//��ų ȿ����

					//���� ��� �ٲٱ�
					cnt = PlayerArmor;
					PlayerArmor = -1;
					SetPlayerArmor( cnt );

					cInvenTory.IncreViewDamage ( 0 );			//���� ������ �ʱ�ȭ

					if ( RestartPlayCount<350 ) RestartPlayCount=350;		//��õ��� ���� (3��)
				}
			}

			/////////////////////// �ڵ� ���� ���� ////////////////////////
			if ( rsRecorder.dwNextRecordTime<dwPlayTime ) {

				if ( GetSaveResult()==TRUE ) {
					//������ ����
					SaveGameData();
					//Check_CodeSafe2();			//�ڵ庸ȣ
				}
			}
			if ( rsRecorder.dwRecordTime && (rsRecorder.dwRecordTime+1000*8)<dwPlayTime ) {
#ifdef _WINMODE_DEBUG
				if ( smConfig.DebugMode ) AddChatBuff( "SaveGame TimeOut / Retry SaveGame" );
#endif
				RecordFailCount++;
				//���� �ð� �ʰ�
				rsRecorder.dwRecordTime = 0;
				//������ ���� ��õ�
				SaveGameData();
			}

			//6ȸ �̻� ���� ����
			if ( !DisconnectFlag && RecordFailCount>=6 ) {
				//���� ���� ���� ���� ������ �˸�
				SendSetHackUser2( 2700 , RecordFailCount );
				DisconnectFlag = dwPlayTime;
#ifdef _WINMODE_DEBUG
				if ( smConfig.DebugMode ) AddChatBuff( "SaveGame Failed over times" );
#endif
			}

			//�ݵ�� 
			if ( lpCurPlayer->OnStageField>=0 && StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_FOREST &&
				 ( dwGameHour<GAME_HOUR_DAY || dwGameHour>=GAME_HOUR_DARKNESS ) && (PlayCounter&0x1FF)==0 ) {

				if ( ((dwPlayTime>>3)&1)==0 ) {
					StartEffect( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ , EFFECT_BANDI1 );
				}
			}
		}

/*
		if ( lpCurPlayer->OnStageField>=0 && StageField[lpCurPlayer->OnStageField]->FieldCode==13 ) {
			//��ü �Ÿ��� ���� ����
			esPlayObjectSound( 5 , -15385*fONE, 100*fONE , -24949*fONE );
		}
		else {
			esPlayObjectSound( -1 , 0,0,0 );
		}
*/
		if ( lpCurPlayer->OnStageField>=0 ) {
			//��� ȿ���� ���� ( �ֱ��� ȣ�� )
			StageField[lpCurPlayer->OnStageField]->PlayAmbient();
		}
		else {
			if ( ResetLastVolumeDist()>800 ) {
				esPlayObjectSound2( -1 , 0,0,0,0 );
			}
		}

/*
			cnt = 0;
			//��ü �Ÿ��� ���� ����
			esPlayObjectSound2( 0 , 128, -15385*fONE, 100*fONE , -24949*fONE );
			esPlayObjectSound2( 1 , 128, -14047*fONE, 100*fONE , -25762*fONE );
			//cnt += esPlayObjectSound2( 1 , -15385*fONE, 100*fONE , -24949*fONE , 0 );

		}

		if ( ResetLastVolumeDist()>800 ) {
			esPlayObjectSound2( -1 , 0,0,0,0 );
		}
*/
	}
/*
	if ( PatEffectShield ) {
		PatEffectShield->Frame += 80;
		if ( PatEffectShield->MaxFrame>=PatEffectShield->Frame ) 
			PatEffectShield->Frame =0;
	}
*/


	dsMenuCursorPos( &pCursorPos , 4 );
	//��Ƽ �޴� ����
	MainInterfaceParty();
	dsMenuCursorPos( &pCursorPos , 0 );


	//���� �ʵ� �˻�
	if ( lpCurPlayer->OnStageField>=0 && StageField[ lpCurPlayer->OnStageField ] ) {
		StageField[ lpCurPlayer->OnStageField ]->CheckWarpGate( lpCurPlayer->pX , lpCurPlayer->pY , lpCurPlayer->pZ );
	}

	if ( RestartPlayCount>0 ) RestartPlayCount--;


	if ( (PlayCounter&0xFFF)==0 ) {
		//1�� ���� ����
		 //ũ���� �ٽ� ã�´�
		CheckCracker( NULL ); //(�¶��� �˻�)
		//ĳ���� ���� ���� Ȯ��
		CheckCharForm();

#ifdef _WINMODE_DEBUG
		if ( smConfig.DebugMode ) AddChatBuff( "1Min." );
#endif

		//���ΰ� ĳ���� ������ ����
		if ( !lpCurPlayer->HoSkillCode && !lpCurPlayer->dwDispAppSkill ) {

			if ( !dwRotPlayDelayTime || dwRotPlayDelayTime<dwPlayTime ) {
				smCHAR	*lpTempChar;
				smCHAR	*lpOldChar;

				lpOldChar = lpCurPlayer;

				lpTempChar =  SelectRotPlayer( lpCurPlayer );
				if ( lpTempChar ) {
					lpCurPlayer = lpTempChar;
					sinChar = &lpCurPlayer->smCharInfo;

					//����� ������ ����
					for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
						if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].chrAttackTarget==lpOldChar)
							chrOtherPlayer[cnt].chrAttackTarget=lpCurPlayer;
					}

				}
			}
		}
		else {
			dwRotPlayDelayTime = dwPlayTime+30*1000;			//30�� ���Ŀ� ���� �����ϵ��� ����
		}
/*
		if (  (PlayCounter&0x3FFF)==0 && (dwDebugBack^dwDebugXor)!=0 ) {
			//������ ������ ��� ���� Ȯ��
			SendAdminMode2( dwDebugBack^dwDebugXor );
		}
*/
		if (  (PlayCounter&0x3FFF)==0 && smConfig.DebugMode ) {
			//������ ������ ��� ���� Ȯ��
			SendAdminMode2( smConfig.DebugMode );
		}

		//������ �׷��� ���� ýũ
		CheckEnergyGraphError();
	}

	//���� ����Ʈ ����
	TimeEffectMain();
	//������ �׷����� ���� ýũ
	CheckEnergyGraph();

	//sinSetLife(500);


#ifdef	_SET_PROCTECT_MEM2
	//PlaySub ����ڵ� ���� Ȯ��
	if ( (PlayCounter&0x7FF)==0 ) {
		//PlaySub ����ڵ� ���� Ȯ��
		cnt = Check_PlaySubReleaseFunc();
		if ( cnt ) {
			SendSetHackUser2( 1120 , cnt );
		}
	}
#endif

	//���� ȸ�� ����
	if ( dwPartyPosionTime && dwPartyPosionTime<dwPlayTime ) {
		//�̹� ������
		LaunchPartyPostion();
	}

	//cInvenTory.HackCharStateCheck(sinChar,&sinCharDisplay);

	if ( (PlayCounter&0x1FF)==0 ) {
		//�κ��丮 ������ �ڵ� Ȯ��
		//CheckInvenItemCode();
		//������ �׷��� ���� ýũ
		CheckEnergyGraphError();
		//�κ��丮 �� �˻�
		//CheckInvenMoney();
/*
//���� ���콺 ������
DWORD	AutoMouse_LastCheckTime = 0;
DWORD	AutoMouse_StartCheckTime = 0;
int		AutoMouse_WM_Count = 0;
int		AutoMouse_Attack_Count = 0;
*/
		//���� ���콺 ����
		if ( AutoMouse_StartCheckTime ) {
			if ( (AutoMouse_StartCheckTime+30000)<dwPlayTime ) {

				if ( AutoMouse_Distance>20000 && AutoMouse_Attack_Count>20 && AutoMouse_WM_Count>110 && (AutoMouse_LastCheckTime+30*60*1000)<dwPlayTime ) {
					AutoMouse_LastCheckTime = dwPlayTime;
					//�Ű�
					SendSetHackUser2( 8100 , AutoMouse_WM_Count );
					if ( smConfig.DebugMode ) AddChatBuff( ">Warning AutoMouse" , 0 );
				}
				/*
				if ( smConfig.DebugMode ) {
					char szBuff[256];
					wsprintf( szBuff , "CM : Distance( %d ) Target( %d ) WM( %d )" ,
						AutoMouse_Distance , AutoMouse_Attack_Count , AutoMouse_WM_Count );
					AddChatBuff( szBuff );
				}
				*/

				AutoMouse_StartCheckTime = dwPlayTime;
				AutoMouse_WM_Count = 0;
				AutoMouse_Distance = 0;
				AutoMouse_Attack_Count = 0;
			}
		}
		else {
			AutoMouse_StartCheckTime = dwPlayTime;
			AutoMouse_WM_Count = 0;
			AutoMouse_Distance = 0;
			AutoMouse_Attack_Count = 0;
		}


	}

	HackTrap_CheckOpenFlag();			//���� üũ�� NPC�����÷� �˻�

#ifdef _TEST_SERVER
	//ġƮ �ù����̼�
	if ( smConfig.DebugMode && VRKeyBuff[VK_CONTROL] && VRKeyBuff['N'] && sinGetKeyClick('M' ) ) {
		/*
		sinPlusMoney( 2101234532 );
		//lpCurPlayer->smCharInfo.szName[0] = 0;
		lpCurPlayer->smCharInfo.Level = 2;
		ReformCharForm();
		*/
		/*
		if ( MouseItem.Flag ) {
			MouseItem.sItemInfo.PotionCount = 200;
		}
		*/

		if ( !ParkPlaying )
			Play_ParkPlayer( "mms://hapkido.re.kr/hapkido/8833/movie_158.asf" , 200,200, 300,300 , 150 );
		else
			Stop_ParkPlayer();

	}
#endif

	return TRUE;
}

extern int BGMatNum[2];
extern int RestoreTexFlag;
extern POINT	pCursorPos;


//���콺�� ���� �÷��̾���� ������ �����ش�
int GetMouseSelAngle( )
{
	int x1,x2,z1,z2;

	if ( lpCharMsTrace ) {
		x1 = lpCurPlayer->pX>>FLOATNS;
		z1 = lpCurPlayer->pZ>>FLOATNS;

		x2 = lpCharMsTrace->pX>>FLOATNS;
		z2 = lpCharMsTrace->pZ>>FLOATNS;
	
		return GetRadian2D( x1 ,z1, x2, z2 );
	}

	if ( lpMsTraceItem ) {
		x1 = lpCurPlayer->pX>>FLOATNS;
		z1 = lpCurPlayer->pZ>>FLOATNS;

		x2 = lpMsTraceItem->pX>>FLOATNS;
		z2 = lpMsTraceItem->pZ>>FLOATNS;
	
		return GetRadian2D( x1 ,z1, x2, z2 );
	}

	return -1;
}

//2���� �÷��̾��� ������ �����ش�
int GetCharAngle( smCHAR *lpChar1 , smCHAR *lpChar2 )
{
	int x1,x2,z1,z2;

		x1 = lpChar1->pX>>FLOATNS;
		z1 = lpChar1->pZ>>FLOATNS;

		x2 = lpChar2->pX>>FLOATNS;
		z2 = lpChar2->pZ>>FLOATNS;
	
		return GetRadian2D( x1 ,z1, x2, z2 );
}


int testframe = 0;
int	LastRendSucessCount = 0;			//�ֱ� ������ ������ ĳ���� ��
int	DispCharLimitMode = 0;				//0-��ü ǥ�� 1-�ٹ游 ǥ��

#define	DISP_CHAR_LIMIT			20
#define	DISP_CHAR_LIMIT_MAX		30
#define	DISP_CHAR_NEAR_DIST		( (10*64)*(10*64) )

int DrawPat3D( int x, int y, int z, int ax, int ay, int az )
{
//	POINT3D cp , ca;
//	POINT3D pp , pa;

	int cnt;
	int SelZ;
	int sez;
	RECT	rect;
	RECT	SelRect;
	POINT	SelPos;
	RECT	*lpRect;
//	POINT	Pos;
	smCHAR	*lpSelChar;
	scITEM	*lpItem;
	int		dx,dz;
	int		pow;

	POINT	sPos;
	int		RendSucessCount;			//������ ���� ĳ���� ī����
	int		DispFlag;
	int		r,g,b;
	DWORD	dwDispAppSKill;


	dwDispAppSKill = lpCurPlayer->dwDispAppSkill;		//��ų ����
	lpCurPlayer->dwDispAppSkill = 0;

	for(cnt=0;cnt<10;cnt++) {
		if ( ContinueSkill[cnt].Flag ) {
			switch( ContinueSkill[cnt].CODE ) {
			case SKILL_EXTREME_SHIELD:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_EXTREME_SHIELD;
				break;

			case SKILL_PHYSICAL_ABSORB:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_PHYSICAL_ABSORB;
				break;

			case SKILL_AUTOMATION:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_AUTOMATION;
				break;

			case SKILL_MAXIMIZE:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_AUTOMATION;
				break;

			case SKILL_CRITICAL_HIT:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_ANGER;
				break;

			case SKILL_HOLY_BODY:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_PHYSICAL_ABSORB;
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_HOLY_BODY;
				DispApp_SkillPoint = ContinueSkill[cnt].Point;
				break;

			case SKILL_ZENITH:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_ANGER;
				break;

			case SKILL_SPARK_SHIELD:
			case SKILL_DIVINE_INHALATION:
				lpCurPlayer->dwDispAppSkill |=  SKILL_APPMASK_SPARK_SHIELD;
				break;

			case SKILL_GODLY_SHIELD:		//���鸮 ����
			case SKILL_FROST_JAVELIN:		//���ν�Ʈ ���� �� ���
			case SKILL_SUMMON_MUSPELL:		//����ν��� �� ���
				lpCurPlayer->dwDispAppSkill |=  SKILL_APPMASK_GODLY_SHIELD;
				break;
			}
		}
	}

	//������  ���� ����
	if ( (lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_PHYSICAL_ABSORB)==0 && (dwDispAppSKill&SKILL_APPMASK_PHYSICAL_ABSORB)!=0 ) {
		SkillStopSound(SKILL_SOUND_PHYSICAL_ABSORB3);
	}
	//�ƽø����� �Ǵ� ������̼�  ���� ����
	if ( (lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_AUTOMATION)==0 && (dwDispAppSKill&SKILL_APPMASK_AUTOMATION)!=0 ) {
		SkillStopSound(SKILL_SOUND_SKILL_AUTOMATION3);
	}
	//Ȧ���ٵ� ���� ����
	if ( (lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_HOLY_BODY)==0 && (dwDispAppSKill&SKILL_APPMASK_HOLY_BODY)!=0 ) {
		SkillStopSound(SKILL_SOUND_PHYSICAL_ABSORB3);
	}



	/*
				AttackAnger+=2;
				if ( AttackAnger>100 ) AttackAnger=100;
	*/


	SelZ = 65536*16384;

	smRender.SetCameraPosi( x, y, z, x, ay, az );
	if ( smGameStage[0] ) 
		SetStageDynLight(smGameStage[0] , x,y,z );
	if ( smGameStage[1] ) 
		SetStageDynLight(smGameStage[1] , x,y,z );


	// ĳ���Ϳ� ����� ī�޶� ��ġ�� �����Ѵ�
	smCHAR_SetCameraPosi( x,y,z, ax,ay,az );
	if ( !DebugPlayer )	{

		cnt = RestartPlayCount;
		if ( lpCurPlayer->PlayInvincible>0 ) cnt = lpCurPlayer->PlayInvincible;

		if ( cnt && (cnt&0x10)==0 ) {
			r = smRender.Color_R;
			g = smRender.Color_G;
			b = smRender.Color_B;

			smRender.Color_R += 80;
			smRender.Color_B += 80;
			//smRender.Color_B += 80;


			lpCurPlayer->Draw();

			smRender.Color_R = r;
			smRender.Color_G = g;
			smRender.Color_B = b;

		}
		else {
			if ( lpCurPlayer->PlayVanish<=0 ) lpCurPlayer->Draw();
		}

		//�÷��̾� Ŭ�� ������ Ȯ��
		if ( rsBlessCastle.dwMasterClan && lpCurPlayer->smCharInfo.ClassClan )
			SetClanMaster_Player( rsBlessCastle.dwMasterClan );

	}

		lpSelChar = 0;
		RendSucessCount = 0;

		for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
			if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].DisplayAlpha==0 ) {

				DispFlag = TRUE;

				if ( DispCharLimitMode ) {
					//�ֱ� �׷��� ĳ���ͼ��� ���� ��� �ָ� �ִ� ĳ���� ��� ������Ŵ
					dx = (lpCurPlayer->pX-chrOtherPlayer[cnt].pX)>>FLOATNS;
					dz = (lpCurPlayer->pZ-chrOtherPlayer[cnt].pZ)>>FLOATNS;
					if ( (dx*dx+dz*dz)>DISP_CHAR_NEAR_DIST ) DispFlag = FALSE;
				}

				if ( DispFlag ) {

					r = smRender.Color_R;
					g = smRender.Color_G;
					b = smRender.Color_B;

					//��ī��Ʈȣũ
					if ( lpCurPlayer->HoSkillCode && chrOtherPlayer[cnt].smCharInfo.Life[0] ) {
						switch( lpCurPlayer->HoSkillCode ) {
						case SKILL_SCOUT_HAWK:
							pow = GetSkillPower( lpCurPlayer , &chrOtherPlayer[cnt] , 0,0,0 );
							if ( pow ) {
//								chrOtherPlayer[cnt].AttackAnger = 100;
								//if ( (dwPlayTime&0x20)==0 ) {
									pow = DarkLevel>>2;
									smRender.Color_R += 40+pow;
									smRender.Color_G += 40+pow;
									smRender.Color_B += 100+pow;
								//}
							}
							break;
						}
					}

					if ( lpCharSelPlayer && lpCharSelPlayer==&chrOtherPlayer[cnt] ) {
						smRender.Color_R = r+40;
						smRender.Color_G = g+60;
						smRender.Color_B = b+60;
						//######################################################################################
						// �� �� �� : �� �� ��						
						DWORD OutLineColor;
						switch( lpCharSelPlayer->smCharInfo.State )
						{
						case smCHAR_STATE_USER:
							OutLineColor = RGBA_MAKE( 255, 255, 255, 255 );
							break;

						case smCHAR_STATE_NPC:
							OutLineColor = RGBA_MAKE( 0, 255, 100, 255 );
							break;

						default:
							OutLineColor = RGBA_MAKE( 255, 30, 0, 255 );
							break;
						}

						int ix = (lpCharSelPlayer->pX - smCHAR_CameraX) >> FLOATNS;
						int iy = (lpCharSelPlayer->pY - smCHAR_CameraY) >> FLOATNS;
						int iz = (lpCharSelPlayer->pZ - smCHAR_CameraZ) >> FLOATNS;
						
						int Dist = (int)sqrt(ix*ix + iy*iy + iz*iz);
						smRender.SetD3D_OutLine( 1, Dist, 1.4f, 0, OutLineColor );
						//######################################################################################

					}

					if ( !chrOtherPlayer[cnt].RenderLatter && chrOtherPlayer[cnt].AttackIce==0 && chrOtherPlayer[cnt].RendAlpha==0 && chrOtherPlayer[cnt].PlayVanish<=0 )  {
						//( chrOtherPlayer[cnt].RendAlpha==0 || chrOtherPlayer[cnt].MotionInfo->State>=0x100 ))  {

						if ( chrOtherPlayer[cnt].PlayInvincible && (chrOtherPlayer[cnt].PlayInvincible&0x10)==0 ) {		//���� ī��Ʈ
							smRender.Color_R += 80;
							smRender.Color_B += 80;
						}

						chrOtherPlayer[cnt].Draw();
					}
					else {
						chrOtherPlayer[cnt].VirtualDraw();
					}

					smRender.Color_R = r;
					smRender.Color_G = g;
					smRender.Color_B = b;

					//######################################################################################
					// �� �� �� : �� �� ��
					smRender.SetD3D_OutLine( 0 );
					//######################################################################################

					if ( chrOtherPlayer[cnt].RendSucess ) {

						RendSucessCount++;

						sez = chrOtherPlayer[cnt].RendPoint.z;
						lpRect = &chrOtherPlayer[cnt].RendRect2D;

						if ( chrOtherPlayer[cnt].smCharInfo.Life[0]>0 && sez>(96*fONE) && sez<SelZ && 
							lpRect->left<pCursorPos.x && lpRect->right>pCursorPos.x &&
							lpRect->top<pCursorPos.y && lpRect->bottom>pCursorPos.y ) {
	
							SelZ = sez;
							lpSelChar = &chrOtherPlayer[cnt];
						}
					}
				}
				else {
					chrOtherPlayer[cnt].RendSucess = FALSE;
					RendSucessCount++;
				}
			}
		}


		LastRendSucessCount = RendSucessCount;

		//ȭ�� ǥ�� ĳ���� ��� ����
		if ( DispCharLimitMode ) {
			if ( RendSucessCount<DISP_CHAR_LIMIT )
				DispCharLimitMode = FALSE;
		}
		else {
			if ( RendSucessCount>DISP_CHAR_LIMIT_MAX )
				DispCharLimitMode = TRUE;
		}

		if ( VRKeyBuff['A'] ) {
			lpSelChar = 0;
			SelZ = 65536*16384;
		}


		lpItem = 0; 

		for(cnt=0;cnt<DISP_ITEM_MAX;cnt++) {
			if ( scItems[cnt].Flag ) {
				if ( scItems[cnt].ItemCode!=0 || DisplayDebug ) {
				scItems[cnt].Draw();

				smRender.SetCameraPosi( x,y,z,ax,ay,az );

					sez = smRender.GetRect2D( scItems[cnt].pX , scItems[cnt].pY+(16*fONE) ,  scItems[cnt].pZ ,
						32*fONE , 32*fONE , &rect , &sPos );

					scItems[cnt].RendPoint.x = sPos.x;
					scItems[cnt].RendPoint.y = sPos.y;
					scItems[cnt].RendPoint.z = sez;

					if ( sez>0 && sez<SelZ && 
						rect.left<pCursorPos.x && rect.right>pCursorPos.x &&
						rect.top<pCursorPos.y && rect.bottom>pCursorPos.y ) {
	
						SelZ = sez;
						lpItem = &scItems[cnt];
						memcpy( &SelRect , &rect , sizeof( RECT ) );
						memcpy( &SelPos , &sPos , sizeof( POINT ) );				
					}
				}

			}
		}

		if ( lpSelChar ) {
			lpCharSelPlayer = lpSelChar;
			lpItem = 0;
		}
		else
			lpCharSelPlayer = 0;

		if ( lpItem ) {
			lpSelItem = lpItem;
			memcpy( &MsSelRect , &SelRect , sizeof( RECT ) );
			memcpy( &MsSelPos , &SelPos , sizeof( POINT ) );
		}
		else
			lpSelItem = 0;

	////////// ���콺 Ŀ�� ��� ���� /////////

	if ( cShop.OpenFlag || cCharShop.OpenFlag ) {

		lpCharSelPlayer = 0;
		lpSelItem = 0;

		if ( CursorClass==SIN_CURSOR_ATTACK  ||
			CursorClass==SIN_CURSOR_TALK  ||
			CursorClass==SIN_CURSOR_GETITEM2  ||
			CursorClass==SIN_CURSOR_GETITEM1 ) {

			CursorClass = 1;
		}
		return TRUE;
	}

	if ( MouseItem.Flag ) {
		CursorClass = 0;
	}
	else {
		CursorClass = 1;

		if (lpCharSelPlayer) {
			switch(lpCharSelPlayer->smCharInfo.State) {
			case smCHAR_STATE_ENEMY:
				//if ( (dwPlayTime&0x200)==0 )
					CursorClass = SIN_CURSOR_ATTACK;
				break;
			case smCHAR_STATE_USER:
				break;
			case smCHAR_STATE_NPC:
				CursorClass = SIN_CURSOR_TALK;
				break;
			}
		}

		if ( lpSelItem ) {
			if ( MouseButton[0] )
				CursorClass = SIN_CURSOR_GETITEM2;
			else
				CursorClass = SIN_CURSOR_GETITEM1;
		}
	}

	return TRUE;
}

int DrawPat3D_Alpha()
{
	int cnt;
	int DispFlag;
	int dx,dz;
	int ap;
	int	RendFalg;
	int BackFrame;
	int cnt2;


	//��Ͻ���� ������ ó��
	if ( lpCurPlayer->PlayVanish>0 ) {
		ap = lpCurPlayer->DisplayAlpha;		//�ӽþ��� ���
		lpCurPlayer->DisplayAlpha = -160;

		if ( lpCurPlayer->PlayVague ) {
			SetItem2PassTexture( 8 , smTEXSTATE_FS_SCROLL5 );
			lpCurPlayer->DisplayAlpha = -60;
		}
		lpCurPlayer->Draw();

		SetItem2PassTexture( -1 );
		lpCurPlayer->DisplayAlpha = ap;
	}
	//������ ������ �ܻ�
	if ( ( lpCurPlayer->AttackSkil&0xFF )==SKILL_PLAY_SHADOW_MASTER && lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_SKILL ) {
		ap = lpCurPlayer->DisplayAlpha;		//�ӽþ��� ���
		BackFrame = lpCurPlayer->frame;
		lpCurPlayer->DisplayAlpha = -140;
		for(cnt2=0;cnt2<6;cnt2++) {
			lpCurPlayer->frame-=160*2;
			if ( lpCurPlayer->frame>(lpCurPlayer->MotionInfo->StartFrame*160) ) {
				lpCurPlayer->Draw();
			}
			lpCurPlayer->DisplayAlpha-=20;
		}
		lpCurPlayer->frame = BackFrame;
		lpCurPlayer->DisplayAlpha = ap;
	}

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag ) {
			RendFalg = 0;
			if ( chrOtherPlayer[cnt].DisplayAlpha!=0 || chrOtherPlayer[cnt].RendAlpha!=0 || chrOtherPlayer[cnt].AttackIce!=0 || chrOtherPlayer[cnt].RenderLatter || chrOtherPlayer[cnt].PlayVanish )  {
				DispFlag = TRUE;

				if ( DispCharLimitMode && chrOtherPlayer[cnt].AttackIce==0 ) {
					//�ֱ� �׷��� ĳ���ͼ��� ���� ��� �ָ� �ִ� ĳ���� ��� ������Ŵ
					dx = (lpCurPlayer->pX-chrOtherPlayer[cnt].pX)>>FLOATNS;
					dz = (lpCurPlayer->pZ-chrOtherPlayer[cnt].pZ)>>FLOATNS;
					if ( (dx*dx+dz*dz)>DISP_CHAR_NEAR_DIST ) DispFlag = FALSE;
				}

				if ( chrOtherPlayer[cnt].PlayVanish>0 && lpCurPlayer->OnStageField>=0 && StageField[ lpCurPlayer->OnStageField ]->FieldCode==rsCASTLE_FIELD ) {
					//�����ʵ忡���� ��Ͻ�
					if ( lpCharSelPlayer!=&chrOtherPlayer[cnt] ) {
						dx = (lpCurPlayer->pX-chrOtherPlayer[cnt].pX)>>FLOATNS;
						dz = (lpCurPlayer->pZ-chrOtherPlayer[cnt].pZ)>>FLOATNS;
						if ( (dx*dx+dz*dz)>DIST_TRANSLEVEL_NEAR ) DispFlag = FALSE;
					}
				}

				if ( DispFlag ) {
					//######################################################################################
					// �� �� �� : �� �� ��
					if( lpCharSelPlayer && lpCharSelPlayer == &chrOtherPlayer[cnt] )
					{
						DWORD OutLineColor;
						switch( lpCharSelPlayer->smCharInfo.State )
						{
						case smCHAR_STATE_USER:
							OutLineColor = RGBA_MAKE( 255, 255, 255, 255 );
							break;

						case smCHAR_STATE_NPC:
							OutLineColor = RGBA_MAKE( 0, 255, 100, 255 );
							break;

						default:
							OutLineColor = RGBA_MAKE( 255, 30, 0, 255 );
							break;
						}

						int ix = (lpCharSelPlayer->pX - smCHAR_CameraX) >> FLOATNS;
						int iy = (lpCharSelPlayer->pY - smCHAR_CameraY) >> FLOATNS;
						int iz = (lpCharSelPlayer->pZ - smCHAR_CameraZ) >> FLOATNS;
						
						int Dist = (int)sqrt(ix*ix + iy*iy + iz*iz);
						smRender.SetD3D_OutLine( 1, Dist, 1.4f, 0, OutLineColor );						
					}
					//######################################################################################

					ap = chrOtherPlayer[cnt].DisplayAlpha;		//�ӽþ��� ���

					if ( chrOtherPlayer[cnt].PlayVanish>0 ) {
						chrOtherPlayer[cnt].DisplayAlpha=-160;	//��Ͻ� ������ó��
						if ( chrOtherPlayer[cnt].PlayVague ) SetItem2PassTexture( 8 , smTEXSTATE_FS_SCROLL5 );
					}

					if ( chrOtherPlayer[cnt].RendAlpha && ap>chrOtherPlayer[cnt].RendAlpha ) {
						//if ( chrOtherPlayer[cnt].MotionInfo->State<0x100 || chrOtherPlayer[cnt].AttackIce!=0 ) {
							chrOtherPlayer[cnt].DisplayAlpha = chrOtherPlayer[cnt].RendAlpha;	//���� ���� ĳ���� ���

							if ( chrOtherPlayer[cnt].MotionInfo->State>=0x100 && 
								( chrOtherPlayer[cnt].smCharInfo.dwCharSoundCode==snCHAR_SOUND_ILLUSIONKNIGHT || chrOtherPlayer[cnt].PlayVanish>0 ) ) {
									chrOtherPlayer[cnt].DisplayAlpha = ap;
								}

							chrOtherPlayer[cnt].Draw();
							chrOtherPlayer[cnt].DisplayAlpha = ap;		//�ӽþ��� ����
						//}
					}
					else {
						chrOtherPlayer[cnt].Draw();
					}
					RendFalg ++;

					SetItem2PassTexture( -1 );
					chrOtherPlayer[cnt].DisplayAlpha = ap;
					//######################################################################################
					// �� �� �� : �� �� ��
					smRender.SetD3D_OutLine( 0 );
					//######################################################################################

				}
			}

			if ( RendFalg==0 ) {
				if ( ( chrOtherPlayer[cnt].AttackSkil&0xFF )==SKILL_PLAY_SHADOW_MASTER && chrOtherPlayer[cnt].MotionInfo->State==CHRMOTION_STATE_SKILL ) {
					ap = chrOtherPlayer[cnt].DisplayAlpha;		//�ӽþ��� ���
					BackFrame = chrOtherPlayer[cnt].frame;
					chrOtherPlayer[cnt].DisplayAlpha = -160;
					for(cnt2=0;cnt2<4;cnt2++) {
						chrOtherPlayer[cnt].frame-=160*4;
						if ( chrOtherPlayer[cnt].frame>(chrOtherPlayer[cnt].MotionInfo->StartFrame*160) ) {
							chrOtherPlayer[cnt].Draw();
						}
					}
					chrOtherPlayer[cnt].frame = BackFrame;
					chrOtherPlayer[cnt].DisplayAlpha = ap;
				}
			}
		}
	}

	return TRUE;
}

//�׸��� �׸���
int DrawPatShadow(int x, int y, int z, int ax, int ay, int az )
{
	int cnt;

	smCHAR_SetCameraPosi( x,y,z, ax,ay,az );
//	AddDepthZ = -6;


//   lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZFUNC,   D3DCMP_EQUAL            );

	cnt = 0;

	lpCurPlayer->DrawShadow();

	//######################################################################################
	//�� �� �� : �� �� ��
	if( lpCurPlayer->OnStageField >= 0 )
	{
		if( StageField[ lpCurPlayer->OnStageField ]->State == FIELD_STATE_ICE )
			lpCurPlayer->DrawIceFoot();
	}
	//######################################################################################

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].DisplayAlpha==0 && chrOtherPlayer[cnt].RendAlpha==0 ) {
			//�׸��� �׸���
			chrOtherPlayer[cnt].DrawShadow();
		}
	}
//    lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZFUNC,   D3DCMP_LESSEQUAL );
//	AddDepthZ = 0;

	return TRUE;
}

extern BYTE VRKeyBuff[256];

int Draw2DCounter = 0;

int DrawPat2D( int x, int y, int z, int ax, int ay, int az )
{
//	smFACE2D face2d;

//smCHAR	chrPenguin[10];
//	POINT3D posi,angle;
//	int rx,ry,rz;
	int cnt;

	if ( lpCurPlayer->Pattern==0 ) return FALSE;

	smCHAR_SetCameraPosi( x,y,z, ax,ay,az );
		smRender.SetMaterialGroup( smMaterialGroup );

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag ) {
			chrOtherPlayer[cnt].DrawMotionBlur();
		}
	}

	lpCurPlayer->DrawMotionBlur();

	DrawParticle( x,y,z, ax,ay,az );


	
/*
	posi.x = lpCurPlayer->pX;
	posi.y = lpCurPlayer->pY;
	posi.z = lpCurPlayer->pZ;

	angle.x = GetRadian( lpCurPlayer->Angle.x );
	angle.y = GetRadian( lpCurPlayer->Angle.y );
	angle.z = GetRadian( lpCurPlayer->Angle.z );
	angle.y = (-angle.y+ANGLE_180)&ANGCLIP;

	if ( lpCurPlayer->Pattern->TmParent ) {
		lpCurPlayer->Pattern->TmParent->SetPosi(  &posi , &angle );
		lpCurPlayer->Pattern->TmParent->SetFrame ( lpCurPlayer->Pattern->Frame );
	}

//	smFACE2D face2d;

	face2d.TexRect.left = 0;
	face2d.TexRect.right = 1;
	face2d.TexRect.top = 0;
	face2d.TexRect.bottom = 1;
	face2d.width  = 50 << FLOATNS;
	face2d.height = 100 << FLOATNS;
	face2d.Transparency = 255;
	face2d.r = 255; face2d.g = 255; face2d.b = 255;
	face2d.x = (-100)<<FLOATNS;
	face2d.z = (-2500)<<FLOATNS;
	face2d.y = (200<<FLOATNS);


	face2d.MatNum = FlameMaterial;

	smRender.Init();

	smRender.SetCameraPosi( x,y,z, ax,ay,az );

	if ( smRender.AddFace2D ( &face2d ) ) {
		smRender.SetClipStates( SMCLIP_LEFT|SMCLIP_RIGHT|SMCLIP_TOP|SMCLIP_BOTTOM );
		smRender.ClipRendFace();							// ��ü ������ ���̽��� Ŭ����
		smRender.GeomVertex2D( FALSE );				// ���ؽ��� 2D��ǥ�� ��ȯ  
		smRender.RenderD3D();
	}



	face2d.TexRect.left = 0;
	face2d.TexRect.right = 1;
	face2d.TexRect.top = 0;
	face2d.TexRect.bottom = 1;
	face2d.width  = 20 << FLOATNS;
	face2d.height = 20 << FLOATNS;
	face2d.Transparency = 255;
	face2d.r = 255; face2d.g = 255; face2d.b = 255;
	face2d.x = (28*64)<<FLOATNS;
	face2d.z = (28*64)<<FLOATNS;
	face2d.y = (250<<FLOATNS);
	face2d.MatNum = ShineMaterial;

	smRender.Init();

	smRender.SetCameraPosi( x,y,z, ax,ay,az );

	if ( smRender.AddFace2D ( &face2d ) ) {
		smRender.SetClipStates( SMCLIP_LEFT|SMCLIP_RIGHT|SMCLIP_TOP|SMCLIP_BOTTOM );
		smRender.ClipRendFace();							// ��ü ������ ���̽��� Ŭ����
		smRender.GeomVertex2D( FALSE );				// ���ؽ��� 2D��ǥ�� ��ȯ  
		smRender.RenderD3D();
	}
*/

	Draw2DCounter++;

	//���� �̹��� �׸���
	DrawLightImage();

	DrawWeather( ax );


	return TRUE;

}

extern int ViewPointLen ;
//int ViewPointLen = 28*SizeMAPCELL;
extern DWORD smBackColor;

int BGFace[2][3] = {
	{ 0 ,  1 , 2 },
	{ 1 ,  3 , 2 }
};


DRZTEXTURE2 *hBGTexture;
DRZTEXTURE2 *hBGTexture2;
smOBJ3D *smBackGround;

DIRECTDRAWSURFACE *BackVideoSurface;
int BackVideoFrame;
int LastFrame ;

int BGMatNum[2];


#include "pbackground.h"

smBACKGROUND	smBackMesh[2];
smBACKGROUND	*lpCurrentBG;


int InitBackGround()
{

	//smBackMesh[0].Init( 0 );
	//lpCurrentBG = &smBackMesh[0];

	lpCurrentBG = 0;
	//ChangeBG( 0 );
	ChangeSky( 0 );

	return TRUE;
}

int CloseBackGround()
{

	smBackMesh[1].Close();
	smBackMesh[0].Close();
	return TRUE;

}

int BgFrame = 0;

int BGMain()
{
	smBackMesh[0].Main();
	smBackMesh[1].Main();

	if ( lpCurrentBG && lpCurrentBG->FadeShowCount==0 ) {
		if ( &smBackMesh[0]!=lpCurrentBG && smBackMesh[0].smObjBG )
			smBackMesh[0].Close();

		if ( &smBackMesh[1]!=lpCurrentBG && smBackMesh[1].smObjBG )
			smBackMesh[1].Close();
	}

	return TRUE;
}

int ChangeBG( int BgNum )
{

	if ( !lpCurrentBG ) {
		smBackMesh[0].Init( BgNum );
		lpCurrentBG = &smBackMesh[0];
		if ( smBackMesh[0].lpMaterialGroup )
			smBackMesh[0].lpMaterialGroup->ReadTextures();

		smBackMesh[0].FadeShowCount = 0;

		return TRUE;
	}

	if ( lpCurrentBG->BackGroundNum==BgNum ) return FALSE;

	if ( &smBackMesh[0]!=lpCurrentBG && !smBackMesh[0].smObjBG ) {
		smBackMesh[0].Init( BgNum );
		lpCurrentBG = &smBackMesh[0];
		if ( smBackMesh[0].lpMaterialGroup )
			smBackMesh[0].lpMaterialGroup->ReadTextures();
		return TRUE;
	}

	if ( &smBackMesh[1]!=lpCurrentBG && !smBackMesh[1].smObjBG ) {
		smBackMesh[1].Init( BgNum );
		lpCurrentBG = &smBackMesh[1];
		if ( smBackMesh[1].lpMaterialGroup )
			smBackMesh[1].lpMaterialGroup->ReadTextures();
		return TRUE;
	}
	
	return FALSE;
}

int DrawBG( int x, int y, int z, int ax, int ay, int az )
{


	if ( StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_DUNGEON ) return TRUE;
	if ( StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_ROOM ) return TRUE;

	renderDevice.SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);

	if ( smBackMesh[0].FadeShowCount>smBackMesh[1].FadeShowCount ) { 
		smBackMesh[0].Draw( x,y,z,ax,ay,az );
		smBackMesh[1].Draw( x,y,z,ax,ay,az );
	}
	else {
		smBackMesh[1].Draw( x,y,z,ax,ay,az );
		smBackMesh[0].Draw( x,y,z,ax,ay,az );
	}

	renderDevice.SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);

	return TRUE;
}
/*
RECT	PartyTradeButtonRect[3] = {
	{	700,116, 20, 20	},	
	{	725,116, 20, 20	},	
	{	750,116, 20, 20	},	
};

RECT	PartyTradeToolTipRect = { 701,134,70,28 };
*/
/*
RECT	PartyTradeButtonRect[3] = {
	{	700,120, 20, 20	},	
	{	725,120, 20, 20	},	
	{	750,120, 20, 20	}
};

RECT	PartyTradeToolTipRect = { 701,138,70,28 };
*/
RECT	PartyTradeButtonRect[4] = {
	{	00,0, 20, 20	},	
	{	25,0, 20, 20	},	
	{	50,0, 20, 20	},	
	{	75,0, 20, 20	}	
};

RECT	PartyTradeToolTipRect = { 1,18,70,28 };

POINT	PartyButtonPoint = { -1, -1 };

//��Ƽ �ŷ� ��û ��ư ��ġ�� ����
int GetPartyTradeButtonPos( int x, int y )
{
	int cnt,sx;
	int AdjustX;
	int MaxCnt;

	int wx,wy;
	if ( PartyButtonPoint.x<0 ) return -1;

	wx = PartyButtonPoint.x;
	wy = PartyButtonPoint.y;

	if ( EachTradeButtonMode ) { AdjustX = -10; MaxCnt=4; }
	else { AdjustX = 0; MaxCnt=3; }

	for( cnt=0;cnt<MaxCnt;cnt++ ) {
		sx = PartyTradeButtonRect[cnt].left+AdjustX+wx;
		if ( sx<x && (sx+PartyTradeButtonRect[cnt].right)>x &&
			PartyTradeButtonRect[cnt].top+wy<y && (PartyTradeButtonRect[cnt].top+wy+PartyTradeButtonRect[cnt].bottom)>y ) {

				return cnt;

			}
	}

	return -1;
}

//��Ƽ Ʈ���̵� ��û ��Ʈ Ŭ��
int DisplayPartyTradeButton()
{

	int SelNum;
	int cnt;
	int AdjustX;
	int MaxCnt;

	int wx,wy;
	if ( PartyButtonPoint.x<0 ) return -1;
	wx = PartyButtonPoint.x;
	wy = PartyButtonPoint.y;

	if ( EachTradeButtonMode ) { AdjustX = -10; MaxCnt=4; }
	else { AdjustX = 0; MaxCnt=3; }

	SelNum = GetPartyTradeButtonPos( pCursorPos.x , pCursorPos.y );

	for(cnt=0;cnt<MaxCnt;cnt++) {
		if ( SelNum==cnt ) {
			if ( lpDDS_ParTradeButton[cnt][1] ) {
				DrawSprite( PartyTradeButtonRect[cnt].left+AdjustX+wx , PartyTradeButtonRect[cnt].top+wy, 
				lpDDS_ParTradeButton[cnt][1], 0, 0, PartyTradeButtonRect[cnt].right ,PartyTradeButtonRect[cnt].bottom );
			}
		}
		else {
			if ( lpDDS_ParTradeButton[cnt][0] ) {
				DrawSprite( PartyTradeButtonRect[cnt].left+AdjustX+wx , PartyTradeButtonRect[cnt].top+wy, 
				lpDDS_ParTradeButton[cnt][0], 0, 0, PartyTradeButtonRect[cnt].right ,PartyTradeButtonRect[cnt].bottom );
			}
		}
	}

	if ( SelNum>=0 ) {
		if ( lpDDS_ParTradeToolTip[SelNum] ) {
			DrawSprite( PartyTradeButtonRect[SelNum].left-60+AdjustX+wx , PartyTradeToolTipRect.top+wy, 
			lpDDS_ParTradeToolTip[SelNum], 0, 0, PartyTradeToolTipRect.right ,PartyTradeToolTipRect.bottom );
		}
	}

	return TRUE;
}


/////////////////// Ȯ���� ĳ���� //////////////
smCHAR	chrEachPlayer;
smCHAR *chrEachMaster = 0;
int		EachTradeButton = 0;
int		EachTradeButtonMode = 0;
RECT	RectTradeButton;

POINT3D EachCameraPos;
int	DispEachMode = 0;


//BIP�� ��ġ ����Ʈ ��ǥ�� ���Ѵ�
int FindBipPoint( smPAT3D *lpPat , char *szObjNodeName ,int frame, int *nX, int *nY, int *nZ );

//Ȯ�� ĳ���� ����
int OpenEachPlayer( smCHAR *lpChar )
{
	if ( !lpChar || !lpChar->Flag || !lpChar->DisplayFlag ) return FALSE;
	if ( EachTradeButton ) return FALSE;

	memcpy( &chrEachPlayer , lpChar , sizeof( smCHAR ) );
	chrEachPlayer.SetMotionFromCode( CHRMOTION_STATE_STAND );
	chrEachPlayer.ActionPattern = 101;
	chrEachMaster = lpChar;

	chrEachPlayer.Flag = TRUE;

	if ( FindBipPoint( chrEachPlayer.AnimPattern , "Bip01 Head" , chrEachPlayer.frame ,
		&EachCameraPos.x , &EachCameraPos.y, &EachCameraPos.z )==NULL ) {

		EachCameraPos.y = chrEachPlayer.PatHeight-10*fONE;
		EachCameraPos.x = 0;
		EachCameraPos.z = 0;
	}

	//ī�޶� ��ġ ����
	if ( lpChar->smCharInfo.State==smCHAR_STATE_ENEMY || lpChar->smCharInfo.State==smCHAR_STATE_NPC ) {
		EachCameraPos.y += lpChar->smCharInfo.ArrowPosi[0]*fONE;
		EachCameraPos.z += lpChar->smCharInfo.ArrowPosi[1]*fONE;
		//EachCameraPos.y += -8000;//lpChar->smCharInfo.ArrowPosi[0]*fONE;
		//EachCameraPos.z += 8000;//lpChar->smCharInfo.ArrowPosi[1]*fONE;
	}

	return TRUE;
}

//Ȯ�� ĳ���� �ݱ�
int CloseEachPlayer()
{
	chrEachPlayer.Flag = 0;
	chrEachMaster  = 0;
	EachTradeButton = 0;

	return TRUE;
}

//Ȯ�� ĳ���� ����
int EachPlayerMain()
{
	if ( !chrEachMaster || !chrEachMaster->Flag || chrEachMaster->dwObjectSerial!=chrEachPlayer.dwObjectSerial ||
		chrEachMaster->Pattern!=chrEachPlayer.Pattern ) {
		CloseEachPlayer();
		return TRUE;
	}

	if ( chrEachPlayer.Flag ) {

		//######################################################################################
		//�� �� �� : �� �� ��
		if( DispEachMode )
		{
			if( chrEachPlayer.TalkSoundSum && chrEachPlayer.TalkSoundSum != g_PlayTalkSoundSum )
			{
                PlayTalkSound();
				chrEachPlayer.AutoChangeTalkMotion( TALK_MOTION_FILE );
			}
			else if( chrEachPlayer.TalkPattern && chrEachPlayer.TalkFrame == 0 )
			{
				if( IsPlayTalkSound() )
					chrEachPlayer.AutoChangeTalkMotion( TALK_MOTION_FILE );
				else
					chrEachPlayer.AutoChangeTalkMotion( FACIAL_MOTION_FILE );
			}
		}
		else
		{
			if( IsPlayTalkSound() )
				StopTalkSound();

			chrEachPlayer.TalkSoundSum = 0;
			g_PlayTalkSoundSum = 0;
		}
		//######################################################################################

		chrEachPlayer.Main();

		//�̺�Ʈ NPC Ư�� ó��
		if ( chrEachMaster && chrEachMaster->smCharInfo.State==smCHAR_STATE_NPC ) 
		{
			if ( chrEachMaster->MotionInfo->State==CHRMOTION_STATE_ATTACK && chrEachPlayer.MotionInfo->State==CHRMOTION_STATE_STAND )
				chrEachPlayer.SetMotionFromCode( CHRMOTION_STATE_ATTACK );
		}

		if (chrEachMaster->MotionInfo->State==CHRMOTION_STATE_DEAD && chrEachMaster->FrameCounter>0x50 )
			CloseEachPlayer();

	}

	return TRUE;
}

int DrawClanMark_EachBox( DIRECTDRAWSURFACE clanMark , int x, int y)
{
	RECT ddRect;

	ddRect.left   = 0;
	ddRect.right  = 16;
	ddRect.top    = 0;
	ddRect.bottom = 16;

	if (clanMark)
	{
		//renderDevice.lpDDSBack->BltFast(x, y, clanMark, &ddRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		renderDevice.BltFast(renderDevice.lpDDSBack, x, y, clanMark, &ddRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}

	return TRUE;
}

//Enlarge the character drawing
int DrawEachPlayer( float fx , float fy , int SizeMode )
{
	int x,y,z,w,h;
	int mx,my,he;
	POINT3D angle;
//	RECT rect;
	int dist,len;
	HDC	hdc;
	DWORD	dwColor;
	int		ObjSizeBack;
	int		ModeFlag;
	int		ClanX,ClanY,ClanLen;

	if ( chrEachPlayer.Flag && chrEachMaster && chrEachMaster->Flag ) 
	{
		chrEachPlayer.pX = 0;
		chrEachPlayer.pY = 0;
		chrEachPlayer.pZ = 0;

		chrEachPlayer.Angle.x = 0;
		chrEachPlayer.Angle.y = 0;
		chrEachPlayer.Angle.z = 0;

		mx = MidX;
		my = MidY;

		MidX = (int)((float)WinSizeX * fx );
		MidY = (int)((float)WinSizeY * fy );

		int chrHeight;
		
		if ( chrEachPlayer.smCharInfo.State<=TRUE )  
			chrHeight = chrEachPlayer.PatHeight - 8*fONE;
		else
			chrHeight = chrEachPlayer.PatHeight;

		if ( SizeMode )
		{
			//w = (int)((float)WinSizeX/4);
			h = (int)((float)WinSizeY/2);
			w = (int)((float)WinSizeY*1.34f/4);

			w += 20;
			h += 20;

			smRender.SMSHIFT_PERSPECTIVE_WIDTH = RENDCLIP_DEFAULT_SHIFT_PERSPECTIVE_WIDTH*3;
			smRender.SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_DEFAULT_MULT_PERSPECTIVE_HEIGHT*2;

			dist = 30*fONE;
			he = -1*fONE;
		}
		else 
		{
			//w = (int)((float)WinSizeX/8);
			h = (int)((float)WinSizeY/6);
			w = (int)((float)WinSizeY*1.34f/8);

			w += 10;
			h += 10;

			smRender.SMSHIFT_PERSPECTIVE_WIDTH = RENDCLIP_DEFAULT_SHIFT_PERSPECTIVE_WIDTH*4;
			smRender.SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_DEFAULT_MULT_PERSPECTIVE_HEIGHT*6;

			dist = 100*fONE;
			he = 3*fONE;
		}

		dist += EachCameraPos.y;

		//�׵θ� �׸� ���
		if ( MatEachBox>=0 )
		{
			renderDevice.BeginScene();
			dsDrawTexImage( MatEachBox , MidX-(w>>1), MidY-(h>>1), w,h, 255, 0 );

			if ( !SizeMode )
			{
				if ( EachTradeButton )
				{
					/*
					ly = MidY+80;
					wsprintf( szBuff , "Level : %d", chrEachPlayer.smCharInfo.Level );
					dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
					*/
#ifdef	_NEW_PARTRADE
	/*
					int wx = (WinSizeX*256)/800;
					int wy = (120*wx/256)-120;
					wx = (720*wx/256)-720;
					if ( EachTradeButtonMode )
						dsDrawTexImage( MatEachMenuBox , 700+wx, 120+wy, 120,50, 255, 0 );
					else
						dsDrawTexImage( MatEachMenuBox , 700+wx, 120+wy, 120,50, 255, 0 );
	*/

					PartyButtonPoint.y = MidY+(h>>1);

					if ( EachTradeButtonMode )
					{
						dsDrawTexImage( MatEachMenuBox , MidX-60, MidY+(h>>1)-6, 120,50, 255, 0 );
						PartyButtonPoint.x = MidX-46;
					}
					else 
					{
						dsDrawTexImage( MatEachMenuBox , MidX-50, MidY+(h>>1)-6, 100,50, 255, 0 );
						PartyButtonPoint.x = MidX-36;
					}


					//��Ƽ Ʈ���̵� ��û ��Ʈ Ŭ��
					//DisplayPartyTradeButton();
#else

					dsDrawTexImage( MatEachMenuBox , MidX-50, MidY+56, 100,60, 255, 0 );

					RectTradeButton.left = MidX-32;
					RectTradeButton.right = MidX+32;
					RectTradeButton.top = MidY+62;
					RectTradeButton.bottom = MidY+62+32;

					if ( pCursorPos.x>RectTradeButton.left && pCursorPos.x<RectTradeButton.right &&
						pCursorPos.y>RectTradeButton.top && pCursorPos.y<RectTradeButton.bottom ) {
						dsDrawTexImage( MatEachTradeButton[1] , MidX-32, MidY+62, 64,32, 255, 0 );
						EachTradeButton = 2;
					}
					else {
						dsDrawTexImage( MatEachTradeButton[0] , MidX-32, MidY+62, 64,32, 255, 0 );
						EachTradeButton = 1;
					}
#endif
				}
				else 
				{
					/*
					if ( rsBlessCastle.CastleMode && 
						lpCurPlayer->OnStageField>=0 && 
						StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_CASTLE ) 
					{	//���� ĳ�� ų�� ����Ʈ ǥ��
						chaSiege.ShowPickUserKillPoint(MidX-32, MidY+(h>>1)+6,chrEachMaster->sBlessCastle_Damage[1]);
					}
					*/
				}
			}


			/*
			//�޴� ��ư �׵θ�
			MatEachMenuBox = CreateTextureMaterial( "image\\cw-1.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
			MatEachTradeButton[0] = CreateTextureMaterial( "image\\CW-trade0.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
			MatEachTradeButton[1] = CreateTextureMaterial( "image\\CW-trade1.tga" ,0 , TRUE ,FALSE , FALSE , SMMAT_BLEND_ALPHA );
			*/
			renderDevice.EndScene();
		}

		renderDevice.ClearViewport(D3DCLEAR_ZBUFFER);
		
		//Set the camera position used on character
		smCHAR_SetCameraPosi( EachCameraPos.x , EachCameraPos.y+he, dist+EachCameraPos.z, 0, ANGLE_180,0 );

		if ( chrEachMaster->MotionInfo->State>=0x100 ) 
			ModeFlag=TRUE;
		else
			ModeFlag = FALSE;

		//�̺�Ʈ NPC Ư�� ó��
		if ( ModeFlag && chrEachMaster->smCharInfo.State==smCHAR_STATE_NPC && chrEachPlayer.MotionInfo->State==CHRMOTION_STATE_ATTACK )
			ModeFlag = FALSE;
			
		//if ( chrEachMaster->MotionInfo->State>=0x100 ) {
		if ( ModeFlag )
		{
			x = chrEachMaster->pX;
			y = chrEachMaster->pY;
			z = chrEachMaster->pZ;
			memcpy( &angle , &chrEachMaster->Angle ,sizeof( POINT3D ) );

			chrEachMaster->pX = 0;
			chrEachMaster->pY = 0;
			chrEachMaster->pZ = 0;
			ZeroMemory( &chrEachMaster->Angle , sizeof( POINT3D ) );


			ObjSizeBack = chrEachMaster->smCharInfo.wPlayClass[1];			//���� ũ�� �ӽ� ����
			chrEachMaster->smCharInfo.wPlayClass[1] = 0;

			chrEachMaster->Draw();

			chrEachMaster->smCharInfo.wPlayClass[1] = ObjSizeBack;			//���� ũ�� ����

			if ( !SizeMode && chrEachMaster->smCharInfo.State && !EachTradeButton )
			{
				renderDevice.BeginScene();
				chrEachMaster->DrawStateBar( MidX-(w>>1)+5, MidY-(h>>1)+h+2 );
				renderDevice.EndScene();
			}

			chrEachMaster->pX = x;
			chrEachMaster->pY = y;
			chrEachMaster->pZ = z;

			memcpy( &chrEachMaster->Angle, &angle , sizeof( POINT3D ) );
		}
		else 
		{
			chrEachPlayer.AttackAnger = chrEachMaster->AttackAnger;

			ObjSizeBack = chrEachPlayer.smCharInfo.wPlayClass[1];			//���� ũ�� �ӽ� ����
			chrEachPlayer.smCharInfo.wPlayClass[1] = 0;

			chrEachPlayer.Draw();

			chrEachPlayer.smCharInfo.wPlayClass[1] = ObjSizeBack;			//���� ũ�� ����

			if ( !SizeMode && chrEachMaster->smCharInfo.State ) 
			{
				renderDevice.BeginScene();
				chrEachMaster->DrawStateBar( MidX-(w>>1)+5, MidY-(h>>1)+h+2 );
				renderDevice.EndScene();
			}
		}

		if ( SizeMode ) 
		{

			if ( chrEachPlayer.szChatMessage[0] ) 
			{
				dist = lstrlen( chrEachPlayer.szChatMessage );
				if ( dist>34 ) dist=34;
				dist >>=1; 
				dist *=8;
				dist -=8;

				DrawMessage( MidX-dist, MidY-(h>>1)+h+8, chrEachPlayer.szChatMessage ,36, BOX_ONE );
			}
			else 
			{
				if ( chrEachMaster->szChatMessage[0] )
					lstrcpy( chrEachPlayer.szChatMessage , chrEachMaster->szChatMessage );
			}

			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc , hFont );
		    SetBkMode( hdc, TRANSPARENT );

			len = lstrlen( chrEachPlayer.smCharInfo.szName );
			dist = len>>1;
			dist *=8;
			dist -=8;

			SIZE	sizecheck;
			sizecheck.cx=0;
			sizecheck.cy=0;
			GetTextExtentPoint32(hdc,chrEachPlayer.smCharInfo.szName,lstrlen(chrEachPlayer.smCharInfo.szName),&sizecheck);			
			dist = sizecheck.cx>>1;

			SetTextColor( hdc, RGB(0,0,0 ) );
			dsTextLineOut( hdc , MidX-dist+1 , MidY-(h>>1)+h-33 , chrEachPlayer.smCharInfo.szName , len );
			SetTextColor( hdc, RGB(255, 255, 180) );
			dsTextLineOut( hdc , MidX-dist , MidY-(h>>1)+h-32 , chrEachPlayer.smCharInfo.szName , len );

			renderDevice.lpDDSBack->ReleaseDC(hdc);

		}
		else
		{
			if ( !EachTradeButton )
			{
				if ( rsBlessCastle.CastleMode && lpCurPlayer->OnStageField>=0 && 
					StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_CASTLE ) 
				{	//���� ĳ�� ų�� ����Ʈ ǥ��
					if ( chrEachMaster->smCharInfo.State==smCHAR_STATE_USER || chrEachMaster->PkMode_CharState==smCHAR_STATE_USER )
						chaSiege.ShowPickUserKillPoint(MidX-32, MidY+(h>>1)+10,chrEachMaster->sBlessCastle_Damage[1]);
				}
			}

			if ( chrEachPlayer.smCharInfo.ClassClan && chrEachPlayer.ClanInfoNum>=0 ) 
			{
				ClanLen = lstrlen( ClanInfo[chrEachPlayer.ClanInfoNum].ClanInfoHeader.ClanName );

				ClanX = MidX-26;
				ClanY = MidY-(h>>1)+8;

				renderDevice.BeginScene();
				DrawClanMark_EachBox( ClanInfo[chrEachPlayer.ClanInfoNum].hClanMark , ClanX-18, ClanY );
				renderDevice.EndScene();

				if ( ClanLen<12 )
					ClanX+=(12-ClanLen)<<1;

				ClanY+=2;

				if ( pCursorPos.x>(MidX-(w>>1)) && pCursorPos.y<(MidY+(h>>1)) && ClanInfo[chrEachPlayer.ClanInfoNum].ClanInfoHeader.ClanReport[0] )
					DrawClanMessage( (MidX-(w>>1)), 14, ClanInfo[chrEachPlayer.ClanInfoNum].ClanInfoHeader.ClanReport, 42 , RGB(255,255,255) );
			}
			else
				ClanLen = 0;

			if ( chrEachPlayer.smCharInfo.State==smCHAR_STATE_ENEMY && !chrEachPlayer.smCharInfo.ClassClan )
			{
				len = 0;
				switch ( chrEachPlayer.smCharInfo.Brood )
				{
					case smCHAR_MONSTER_MUTANT:
						len = 1;
						break;
					case smCHAR_MONSTER_UNDEAD:
						len = 2;
						break;
					case smCHAR_MONSTER_DEMON:
						len = 3;
						break;
					case smCHAR_MONSTER_MECHANIC:
						len = 4;
						break;

					case smCHAR_MONSTER_USER:
						len = -1;
						break;
				}
				if ( len>=0 )
					dsDrawTexImage( MatMonBroodImage[len] , MidX+(w/2)-24, MidY-(h/2)+8, 16,16, 255, 0 );
			}

			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc , hFont );
		    SetBkMode( hdc, TRANSPARENT );

			len = lstrlen( chrEachPlayer.smCharInfo.szName );
			dist = len>>1;
			dist *=8;
			dist -=8;
		
			SIZE	sizecheck;
			sizecheck.cx=0;
			sizecheck.cy=0;
			GetTextExtentPoint32(hdc,chrEachPlayer.smCharInfo.szName,lstrlen(chrEachPlayer.smCharInfo.szName),&sizecheck);			
			dist = sizecheck.cx>>1;

			SetTextColor( hdc, RGB(0,0,0 ) );
			dsTextLineOut( hdc , MidX-dist+1 , MidY-(h>>1)+h-17 , chrEachPlayer.smCharInfo.szName , len );

			dwColor = RGB(255, 255, 255);			//�ٸ� �÷��̾�

			switch ( chrEachPlayer.smCharInfo.State )
			{
			case TRUE:
				dwColor = RGB(255, 160, 120);			//����
				break;
			case FALSE:
				dwColor = RGB(255, 255, 180);			//NPC
				break;
			}

			SetTextColor( hdc, dwColor );

			dsTextLineOut( hdc , MidX-dist , MidY-(h>>1)+h-16 , chrEachPlayer.smCharInfo.szName , len );

			if ( ClanLen ) 
			{
				SetTextColor( hdc, RGB(0,0,0 ) );
				dsTextLineOut( hdc , ClanX+1 , ClanY+1 , ClanInfo[chrEachPlayer.ClanInfoNum].ClanInfoHeader.ClanName , ClanLen );
				SetTextColor( hdc, RGB(150, 255, 200) );
				dsTextLineOut( hdc , ClanX , ClanY , ClanInfo[chrEachPlayer.ClanInfoNum].ClanInfoHeader.ClanName , ClanLen );
			}

			if ( smConfig.DebugMode && cInterFace.sInterFlags.MapOnFlag )
			{
				dwColor = RGB(255, 255, 255);			//�ٸ� �÷��̾�
				SetTextColor( hdc, dwColor );

#ifdef _WINMODE_DEBUG
				char szBuff[256];
				int ly;

				ly = MidY+80;
				wsprintf( szBuff , "Level   : %d", chrEachPlayer.smCharInfo.Level );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;
				wsprintf( szBuff , "Attack  : %d %d", chrEachPlayer.smCharInfo.Attack_Damage[0],chrEachPlayer.smCharInfo.Attack_Damage[1] );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;
				wsprintf( szBuff , "Defence : %d ", chrEachPlayer.smCharInfo.Defence );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;
				wsprintf( szBuff , "Absorpt : %d ", chrEachPlayer.smCharInfo.Absorption );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;
				wsprintf( szBuff , "Money   : %d ", chrEachPlayer.smCharInfo.Money );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;
				wsprintf( szBuff , "EXP     : %d ", chrEachPlayer.smCharInfo.Exp );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;
				wsprintf( szBuff , "Rating  : %d ", sinGetAccuracy( chrEachPlayer.smCharInfo.Level , chrEachPlayer.smCharInfo.Defence ) );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;
				wsprintf( szBuff , "Critical: %d ", sinGetCritical( chrEachPlayer.smCharInfo.Level ) );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;
				wsprintf( szBuff , "Update  : %d", chrEachPlayer.smCharInfo.bUpdateInfo[0] );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;
				wsprintf( szBuff , "Clan : %d(%d)", chrEachPlayer.smCharInfo.ClassClan , chrEachMaster->ClanInfoNum );
				dsTextLineOut( hdc , MidX-50 , ly , szBuff , lstrlen(szBuff) );
				ly+=16;

				//accuracy = sinGetAccuracy( lpChar->smCharInfo.Level , lpChar->smCharInfo.Defence );


				if ( DisplayDebug ) // && chrEachPlayer.smCharInfo.State==smCHAR_STATE_USER ) {
				{
					/*
					wsprintf( szBuff , "��1:%s", chrEachPlayer.smCharInfo.szModelName );
					dsTextLineOut( hdc , MidX-160 , ly , szBuff , lstrlen(szBuff) );
					ly+=16;
					wsprintf( szBuff , "��2:%s", chrEachPlayer.smCharInfo.szModelName2 );
					dsTextLineOut( hdc , MidX-160 , ly , szBuff , lstrlen(szBuff) );
					ly+=16;
					*/
					wsprintf( szBuff , "Frame   :%d", chrEachMaster->frame);
					dsTextLineOut( hdc , MidX-100 , ly , szBuff , lstrlen(szBuff) );
					ly+=16;
					/*
					wsprintf( szBuff , "�� ������:%d", lpCurPlayer->frame);
					dsTextLineOut( hdc , MidX-100 , ly , szBuff , lstrlen(szBuff) );
					ly+=16;
					*/
					wsprintf( szBuff , "RcvMoton:%d,%d", (chrEachMaster->PlayBuffCnt&PLAYBUFF_MASK) ,(chrEachMaster->PlayBuffPosi_End&PLAYBUFF_MASK) );
					dsTextLineOut( hdc , MidX-100 , ly , szBuff , lstrlen(szBuff) );
					ly+=16;
					//��Ų�� ����
					wsprintf( szBuff , "User Code :%d", chrEachMaster->dwObjectSerial );
					dsTextLineOut( hdc , MidX-100 , ly , szBuff , lstrlen(szBuff) );
					ly+=16;
				}
#endif
			}

			renderDevice.lpDDSBack->ReleaseDC(hdc);
		}

		MidX = mx;
		MidY = my;

		smRender.SMSHIFT_PERSPECTIVE_WIDTH = RENDCLIP_DEFAULT_SHIFT_PERSPECTIVE_WIDTH;
		smRender.SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_DEFAULT_MULT_PERSPECTIVE_HEIGHT;
	}

	if ( chrEachMaster && chrEachMaster->ActionPattern==99 )
	{
		if ( (chrEachMaster->dwLastTransTime+DISPLAY_TRANS_TIME_OVER)<dwPlayTime )
		{
			CloseEachPlayer();
			return TRUE;
		}
	}

	if ( lpCharMsTrace )
	{
		if ( chrEachMaster!=lpCharMsTrace )
		{
			EachTradeButton = 0;
			OpenEachPlayer( lpCharMsTrace );
			DispEachMode = 0;
		}
	}
	else
	{
		if ( lpCharSelPlayer && ( chrEachMaster!=lpCharSelPlayer ) && ( chrEachMaster!=&chrPartyPlayer ) && !DispEachMode ) 
		{
			OpenEachPlayer( lpCharSelPlayer );
			DispEachMode = 0;
		}
	}

	if ( EachTradeButton && chrEachMaster ) 
	{
		//��ȯ �ŷ� �Ÿ� Ȯ��
		if ( GetTradeDistance( chrEachMaster )==FALSE ) 
			EachTradeButton = 0;
	}

	return FALSE;
}

//BIP�� ��ġ ����Ʈ ��ǥ�� ���Ѵ�
int FindBipPoint( smPAT3D *lpPat , char *szObjNodeName , int frame , int *nX, int *nY, int *nZ )
{
	smOBJ3D	*lpObj = lpPat->GetObjectFromName(szObjNodeName);
	if ( !lpObj ) return FALSE;


	smMATRIX *mWorld;
	POINT3D angle;

	ZeroMemory( &angle, sizeof( POINT3D ) );
	angle.y = (-angle.y+ANGLE_180)&ANGCLIP;

	mWorld = &lpObj->mWorld;

	AnimObjectTree( lpObj , frame , angle.x , angle.y , angle.z );

	*nX = mWorld->_41;
	*nZ = mWorld->_42;
	*nY = mWorld->_43;

	return TRUE;
}

//Ŭ���̾�Ʈ �α� ���� ���
int Record_ClinetLogFile( char *szMessage )
{
	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	SYSTEMTIME	st;
	int len;
	char	szFileName[128];
	char	szBuff[256];

	GetLocalTime( &st );
	wsprintf( szFileName , "game.log" , st.wMonth , st.wDay );

	hFile = CreateFile( "game.log" , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile==INVALID_HANDLE_VALUE ) return FALSE;

	wsprintf( szBuff , "%d-%d (%d:%d:%d) - %s" , st.wMonth , st.wDay , st.wHour, st.wMinute , st.wSecond , szMessage );
	len = lstrlen(szBuff);
	if ( szBuff[len-1]!='\n' ) {
		szBuff[len]='\r';
		szBuff[len+1]='\n';
		len +=2;
	}

	FileLength = GetFileSize( hFile , NULL );
	SetFilePointer( hFile , FileLength , NULL , FILE_BEGIN );
	WriteFile( hFile , szBuff , len , &dwAcess , NULL );
	CloseHandle( hFile );

	return TRUE;
}

//////////////// ��öȣ ������ �÷��̾� /////////////
#include "TJBOY\\park\\ParkDXplayer.h"

int ParkPlayInit = 0;		//�ʱ�ȭ �÷�
int ParkPlayMode = 0;		//-�ø����� 0-�Ϲ� +�����ð�(�ִ� 1000)
int ParkPlaying = 0;		//������ �÷�����
RECT ParkPlayRect;

//�÷��̾� �ʱ�ȭ
int Init_ParkPlayer()
{
	//if ( !ParkPlayInit ) {
		ParkPlayInit = TRUE;
		ParkDXPlay.InitPlayer();
		ParkPlayMode = 0;
		ParkPlaying = 0;
		ParkPlayRect.left	= 0;
		ParkPlayRect.top	= 0;
		ParkPlayRect.right	= WinSizeX;
		ParkPlayRect.bottom	= WinSizeY;

		return TRUE;
	//}

	//return FALSE;
}

//�÷��̾� ���
int Play_ParkPlayer( char *szPath , int x, int y, int w, int h , int PlayMode )
{
	if ( ParkPlaying )
		Stop_ParkPlayer();

	Init_ParkPlayer();

	if ( ParkDXPlay.LoadPlayerFile(hwnd, szPath ) ) {
		ParkDXPlay.PlaySize(x,y,w,h);
		ParkDXPlay.PlayPlayer();
		ParkPlayMode = PlayMode;
		ParkPlaying = TRUE;

		ParkPlayRect.left	= x;
		ParkPlayRect.top	= y;
		ParkPlayRect.right	= x+w;
		ParkPlayRect.bottom	= y+h;

		return TRUE;
	}

	return FALSE;
}

//�÷��̾� ����
int Stop_ParkPlayer()
{
	ParkDXPlay.StopPlayer();
	ParkPlayMode = 0;
	ParkPlaying = FALSE;

	return TRUE;
}
