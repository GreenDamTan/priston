/*----------------------------------------------------------------------------*
*	���ϸ� :  sinInterFace.cpp	
*	�ϴ��� :  �������̽��� ���� ���� �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "..\\field.h"
/*----------------------------------------------------------------------------*
*								�������� 			
*-----------------------------------------------------------------------------*/	
cINTERFACE	cInterFace;
sSHADOWSTATE sShadowState[3];

int sinBmpLength[5] = {0,0,0,0,0}; //������ ���� ���׹̳� ����ġ 

////////////////ä�ð��� Flag
int sinChatEnter = 0;         //ä�� �÷� (����Ű�� �Է��ϸ� ä�� â�� ���´� )
int SubChatHeight = 0;	      //ä��â ���� ��ġ ��ǥ �� 	
int sinChatDisplayMode = 0;   //���� ä���� ��� 
int sinChatInputMode = 0;     //ä�� �Է� ��� 
////////////////

/////////////Test Key Msg
int TestKeyAniFlag = 0;

////////////////��ư���� Flag
int ExitButtonClick = 0;
int HelpButtonClick = 0;
int MouseButtonPosi=0;
int sinMenuBottonPosi = 0;
int sinRunCameraMapPosi = 0; //�ȱ� ī�޶� �� 
int SunandMoonFlag[2] = {0,0}; //0 �� 1 �� 
int InterButtonDown = 0;  //�������̽� ��ư �ٿ� �÷� 
int sinFireShow = 1;      //���� �����ش� 

////////////////��ų ������ (����� ���ǰ� ���� �ʴ� )
int ReSizelpGage = 0;     //������ �׸��� �����Ѵ� 


////////////////
int InitStateFlag = 0;    //�ʱ�ȭ �÷� 


//////////////// ��� ������ 
char szInfoMsgBuff5[512];  //����� ��� �ٷ� ����ġ �� ��ġ ��� ���� 
char szInfoMsgBuff6[128];  //����� ��� �ٷ� ����ġ �� ��ġ ��� ���� 
POINT InfoMessageBoxPos;  //�ؽ��� ǥ���� ��ǥ 


//////////////// ���� (�ٽý��� )
int ReStartFlag = 0;  //���� ������ �ٽý��� �������̽��� �����ش� 
POINT ReStartMainXY = {0,0}; //�ٽý��� ��ǥ 
int ReStartIndex = 0;
POINT ReStartTextXY = {0,0};
char szReStartMsg[128];
char szReStartMsg2[128];

int RestartCheckExp = 0;
int ReStartOptionIndex = 0;
int ShowExpPercentFlag = 0;

//////////////// ��ų ���� ����  (�ٽý��� )

int SkillNpcFlag = 0;  //���� ������ �ٽý��� �������̽��� �����ش� 
POINT SkillMasterMainXY = {0,0}; //�ٽý��� ��ǥ 
int SkillMasterIndex = 0;
POINT SkillMasterTextXY = {0,0};
char szSkillMasterMsg[128];
char szSkillMasterMsg2[128];
int SkillMasterOptionIndex = 0;

////////��ų ��� ������ ����Ʈ 
int NotUseSkillEffect[3] = {0,0,0};

int SkillMouseButtonPosi[2] = {0,0};

/////////// �� ��
int DayOrNightFlag = 0;
int sinGageTime2 = 0;


int ReStartButtonRect[3][4] = {
	{21,22,21+44,22+44},			//�ʵ忡�� ���� 
	{72,22,72+44,22+44},			//�������� ���� 
	{123,22,123+44,22+44},			//���� ���� 
	
};

int ButtonRect[6][5]= {   //�ڽ� ���� ��ư 
	{648,560,648+25,560+27},
	{673,560,673+25,560+27},
	{698,560,698+25,560+27},
	{723,560,723+25,560+27},
	{748,560,748+25,560+27},
	{772,560,772+25,560+27},

};
/*
int ButtonRect[6][5]= {   //�ڽ� ���� ��ư 
	{655,566,678,598},
	{678,566,701,598},
	{701,566,724,598},
	{724,566,747,598},
	{747,566,770,598},
	{770,566,792,598},

};
*/
int sinMenuRect[3][4] = {  //���� ���� ....
	{692,492,784,511},
	{692,517,784,536},
	{692,542,784,561}, //���� 


};

int sinRunCameraMap[3][4] = { //�ȱ� , ī�޶� , �� 
	{569,555,569+26,555+26},
	{569+26,555,569+26+26,555+26},
	{569+26+26,555,569+26+26+26,555+26},

};


// ���� ���� 
int ChangeJobButtonclick = 0;


//PK����
int sinPKWarningExpFlag = 0;
int sinPKWarnigExpTime = 0;
DIRECTDRAWSURFACE lpGrowInterLeft;
DIRECTDRAWSURFACE lpGrowInterMiddle;
DIRECTDRAWSURFACE lpGrowInterRight;


//���� ����Ʈ���� ===================================================================
struct HAEFFECT{
	int   Flag;
	int   Time;
	POINT Posi;
	POINT Size;
	int   Mat;
	int   Alpha;
};
char *HaEffectFilePath[] = {
	0,
	"Image\\SinImage\\Inter\\defense.tga", //1
	"Image\\SinImage\\Inter\\block.tga",   //2
	"Image\\SinImage\\Inter\\Evade.tga",   //3
	0,
};
#define  MAX_HAEFFECTNUM 10 //�Ѱ���

int HA_EffectIndex[4]={0,}; //

HAEFFECT HaEffect[MAX_HAEFFECTNUM];
//=====================================================================================

/*----------------------------------------------------------------------------*
*							Ŭ���� �ʱ�, ���� 
*-----------------------------------------------------------------------------*/	
cINTERFACE::cINTERFACE()
{
	memset(&HaEffect,0,sizeof(HAEFFECT)); //��������Ʈ  �ʱ�ȭ 
}
cINTERFACE::~cINTERFACE()
{


}
/*----------------------------------------------------------------------------*
*							     �ʱ�ȭ 
*-----------------------------------------------------------------------------*/	
void cINTERFACE::Init()
{

	//ReStart���� �� ��ǥ�� ���Ѵ� 
	ReStartMainXY.x = (WinSizeX/2) - (187/2);
	ReStartMainXY.y = (WinSizeY/2) - (127/2)-100;

	ReStartTextXY.x = ReStartMainXY.x+23;
	ReStartTextXY.y = ReStartMainXY.y+73;

	SkillMasterMainXY.x = (WinSizeX/2) - (187/2)-30;
	SkillMasterMainXY.y = (WinSizeY/2) - (127/2)-100;

	SkillMasterTextXY.x = SkillMasterMainXY.x+23;
	SkillMasterTextXY.y = SkillMasterMainXY.y+73;



	InitState(); //����� , ��� , �ٷ��� �ʱ�ȭ�Ѵ� 

	//MatMain = CreateTextureMaterial( "Image\\SinImage\\Inter\\Inter.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	MatMenu[0] = CreateTextureMaterial( "Image\\SinImage\\Inter\\Menu-1.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatMenu[1] = CreateTextureMaterial( "Image\\SinImage\\Inter\\Menu-2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	MatChatDefault[0] = CreateTextureMaterial( "Image\\SinImage\\Inter\\Chat\\ChatDefault-1.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );//ä�ñ⺻ 
	MatChatDefault[1] = CreateTextureMaterial( "Image\\SinImage\\Inter\\Chat\\ChatDefault-2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );//ä�ñ⺻ 
	
	MatChatSmall[0] = CreateTextureMaterial( "Image\\SinImage\\Inter\\Chat\\ChatSmall-1.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );  //���� ä��â
	MatChatSmall[1] = CreateTextureMaterial( "Image\\SinImage\\Inter\\Chat\\ChatSmall-2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );  //���� ä��â				

	MatChatDoc[0] =	CreateTextureMaterial( "Image\\SinImage\\Inter\\Chat\\ChatDoc-1.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );  //���� â 
	MatChatDoc[1] = CreateTextureMaterial( "Image\\SinImage\\Inter\\Chat\\ChatDoc-2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );  //���� â 

	
	MatExitMenu = CreateTextureMaterial( "Image\\SinImage\\Inter\\ExitMenu.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );  //���� â 


	MatClock        = CreateTextureMaterial("Image\\SinImage\\Inter\\Clock.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA); //�ð� 
	MatComPass      = CreateTextureMaterial("Image\\SinImage\\Inter\\Compass.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA); //��ħ�� 

	MatLife			= CreateTextureMaterial("Image\\SinImage\\Inter\\ShadowLife.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA); //�ð� 
	MatMana			= CreateTextureMaterial("Image\\SinImage\\Inter\\ShadowMana.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA); //�ð� 
	MatStamina		= CreateTextureMaterial("Image\\SinImage\\Inter\\ShadowStamina.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA); //�ð� 


	Load();

	
}
/*----------------------------------------------------------------------------*
*						    Bmp������ �ε��Ѵ� 
*-----------------------------------------------------------------------------*/	
void cINTERFACE::Load()
{
	lpLIFE   =  LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Bar_Life.bmp" );
	lpMP     =  LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Bar_Mana.bmp" );
	lpSTM    =  LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Bar_Stamina.bmp" );

	lpButton[0] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bStatus.bmp" ); //���� �̹��� 
	lpButton[1] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bInvenTory.bmp" );
	lpButton[2] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bSkill.bmp" );
	lpButton[3] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bParty.bmp" );
	lpButton[4] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bQuest.bmp" );
	lpButton[5] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bSystem.bmp" );

	lpButton2[0] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bStatus-1.bmp" ); //Ŭ�� �̹��� 
	lpButton2[1] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bInvenTory-1.bmp" );
	lpButton2[2] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bSkill-1.bmp" );
	lpButton2[3] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bParty-1.bmp" );
	lpButton2[4] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bQuest-1.bmp" );
	lpButton2[5] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\bSystem-1.bmp" );

	lpInfoBox[0] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\StatusInfo.bmp" ); //ǳ�� ����
	lpInfoBox[1] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\InvenInfo.bmp" ); //ǳ�� ����
	lpInfoBox[2] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\SkillInfo.bmp" ); //ǳ�� ����
	lpInfoBox[3] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\PartyInfo.bmp" ); //ǳ�� ����
	lpInfoBox[4] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\QuestInfo.bmp" ); //ǳ�� ����
	lpInfoBox[5] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\SystemInfo.bmp" ); //ǳ�� ����


	lpMenuButton[0] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Option.bmp" ); //�ɼ� 
	lpMenuButton[1] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Replay.bmp" ); //�ٽý��� 
	lpMenuButton[2] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Exit.bmp" ); //������


	lpWalk = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Button\\Walk.bmp" ); //�ȱ� 

	lpChatShowButton = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Chat\\ChatDownButton.bmp" ); //ä�ù�ư  
	

	lpSun  = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Flash\\Sun.bmp" ); //�� 
	lpMoon = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Flash\\Moon.bmp" ); //�� 

	lpGage[0] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Skill\\P-skill.bmp" );  //����  ��ų������
	lpGage[1] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Skill\\P-skill2.bmp" );  //������ ��ų������

	lpBar_Anger		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\sinGage\\Bar_Anger.bmp" );  //������ ��ü  
	lpBar_Time		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\sinGage\\Bar_Time.bmp" );  //������ ��ü  
	lpBar_Exp		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\sinGage\\Bar_Exp.bmp" );  //������ ��ü  
	lpButton_Anger	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\sinGage\\Button_Anger.bmp" );  //������ ��ü   
	
	lpRunInfo		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\Run.bmp");  //�ٱ�ǳ�� ���� 
	lpWalkInfo      = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\Walk.bmp"); //�ȱ�ǳ�� ���� 
	
	lpCameraHandInfo= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\Camera_Hand.bmp");   //���� ī�޶� ǳ�� ���� 
	lpCameraFixInfo = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\Camera_Fix.bmp");   //���� ī�޶� ǳ�� ���� 
	lpCameraAutoInfo= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\Camera_Auto.bmp");   //�ڵ� ī�޶� ǳ�� ���� 
	
	lpMapOnInfo		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\MapOn.bmp");   //�ڵ� ī�޶� ǳ�� ���� 
	lpMapOffInfo	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ButtonInfo\\MapOff.bmp");   //���� ī�޶� ǳ�� ���� 

	lpMapOnImage		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Button\\MapOnImage.bmp");  //��ǥ�� �� �̹��� 
	lpCameraImage[0]	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Button\\AutoCameraImage.bmp"); //ī�޶� Auto�̹��� 
	lpCameraImage[1]	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Button\\PixCameraImage.bmp"); //ī�޶� ���� �̹���

    lpLIFESpot		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Bar_LifeSpot.bmp");        //������ 
    lpSTMSpot		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Bar_StaminaSpot.bmp");        //���׹̳� 
    lpMANASpot		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\Bar_ManaSpot.bmp");        //�г������ 

	lpReStartMain		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ReStart\\ReStartMain.bmp"); //�ٽý��� ���� 
	lpReStartButton[0]	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ReStart\\ReStart_Field.bmp"); //�ٽý��� ��ư 
	lpReStartButton[1]	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ReStart\\ReStart_Town.bmp"); //�ٽý��� ��ư 
	lpReStartButton[2]	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ReStart\\ReStart_Exit.bmp"); //�ٽý��� ��ư 


	lpSelectSkill_Main[0]     = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\SkillIcon\\G_Select_Skill.bmp"); //��ų ���� 0 �׷��� 1 Ȱ�� 
	lpSelectSkill_Main[1]     = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\SkillIcon\\Select_Skill.bmp"); //��ų ���� 0 �׷��� 1 Ȱ�� 
	
	lpSelectChangeJob[0] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\SkillIcon\\G_Select_ChangeJob.bmp"); //����        0 �׷��� 2 Ȱ�� 
	lpSelectChangeJob[1] = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\SkillIcon\\Select_ChangeJob.bmp"); //����        0 �׷��� 2 Ȱ�� 

	lpTime7 = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\SkillIcon\\Select_ChangeJob.bmp"); //�ð� ������

	lpGrowInterLeft = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\inter_01.bmp"); //Ȯ�� �������̽�
	lpGrowInterMiddle = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\inter_02.bmp"); 
	lpGrowInterRight = LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\inter_03.bmp"); 


}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cINTERFACE::Release()
{
	int i;

	if(lpLIFE){
		lpLIFE->Release();
		lpLIFE = 0;

	}
	if(lpMP){
		lpMP->Release();
		lpMP = 0;
	}
	if(lpSTM){
		lpSTM->Release();
		lpSTM = 0;
	}

	for(i = 0 ; i < 6 ; i++){
		if(lpButton[i]){
			lpButton[i]->Release();
			lpButton[i] = 0;

		}
		if(lpButton2[i]){
			lpButton2[i]->Release();
			lpButton2[i] = 0;
		}
		if(lpInfoBox[i]){
			lpInfoBox[i]->Release();
			lpInfoBox[i] = 0;
		}
	}
	for(i=0 ; i < 3 ; i++){
		if(lpMenuButton[i]){
			lpMenuButton[i]->Release();
			lpMenuButton[i] = 0;
		}
	}

	if(lpWalk){
		lpWalk->Release();
		lpWalk = 0;	
	}
	if(lpChatShowButton){
		lpChatShowButton->Release();
		lpChatShowButton = 0;
	}	
	if(lpSun){
		lpSun->Release();
		lpSun = 0;

	}
	if(lpMoon){
		lpMoon->Release();
		lpMoon = 0;
	}
	if(lpGage[0]){
		lpGage[0]->Release();
		lpGage[0] = 0;
	}
	if(lpBar_Anger){
		lpBar_Anger->Release();
		lpBar_Anger = 0;
	}
	if(lpBar_Time){
		lpBar_Time->Release();
		lpBar_Time = 0;
	}
	if(lpBar_Exp){
		lpBar_Exp->Release();
		lpBar_Exp = 0;
	}
	if(lpButton_Anger){
		lpButton_Anger->Release();
		lpButton_Anger = 0;
	}

	if(lpRunInfo){
		lpRunInfo->Release();
		lpRunInfo = 0;
	}
	if(lpWalkInfo){
		lpWalkInfo->Release();
		lpWalkInfo = 0;
	}
	
	if(lpCameraAutoInfo){
		lpCameraAutoInfo->Release();
		lpCameraAutoInfo  = 0;
	}
	if(lpCameraHandInfo){
		lpCameraHandInfo->Release();
		lpCameraHandInfo = 0;
	}
	if(lpCameraFixInfo){
		lpCameraFixInfo->Release();
		lpCameraFixInfo = 0;
	}

	if(lpMapOnInfo){
		lpMapOnInfo->Release();
		lpMapOnInfo = 0;
	}
	if(lpMapOffInfo){
		lpMapOffInfo->Release();
		lpMapOffInfo = 0;
	}
	if(lpMapOnImage){
		lpMapOnImage->Release();
		lpMapOnImage = 0;
	}
	if(lpCameraImage[0]){
		lpCameraImage[0]->Release();
		lpCameraImage[0] = 0;
	}
	if(lpCameraImage[1]){
		lpCameraImage[1]->Release();
		lpCameraImage[1] = 0;
	}

	if(lpLIFESpot){
		lpLIFESpot->Release();
		lpLIFESpot = 0;
	}
    if(lpSTMSpot){
		lpSTMSpot->Release();
		lpSTMSpot = 0;
	}
    if(lpMANASpot){
		lpMANASpot->Release();
		lpMANASpot = 0;
	}
	if(lpReStartMain){
		lpReStartMain->Release();
		lpReStartMain = 0;

	}

	for(i=0; i < 3 ; i++){
		if(lpReStartButton[i]){
			lpReStartButton[i]->Release();
			lpReStartButton[i] = 0;
		}
	}
	if(lpReStartMain){
		lpReStartMain->Release();
		lpReStartMain = 0;
		
	}
	for(i=0; i < 3 ; i++){	
		if(lpReStartButton[i]){
			lpReStartButton[i]->Release();
			lpReStartButton[i] = 0;
			
		}
	}
	for(i=0; i < 2 ; i++){	
		if(lpSelectSkill_Main[i]){
			lpSelectSkill_Main[i]->Release();
			lpSelectSkill_Main[i] = 0;

		}

		if(lpSelectChangeJob[i] ){
			lpSelectChangeJob[i]->Release();
			lpSelectChangeJob[i] = 0;
	
		}
	}
}		
/*----------------------------------------------------------------------------*
*							     �׸���  
*-----------------------------------------------------------------------------*/	
int sinWinSizeX[] = {1024,1280,1600};
int sinWinSizeXindex[] = {2,6,11};
int nCheckSkillDraw = 0;		//�ؿ�
void cINTERFACE::Draw()
{

	int i = 0; 
	int j = 0;
	//dsDrawTexImage( MatMain , 0,  0, 256, 128 , 255 );  //�������̽� ���� 

	dsDrawTexImage( cInterFace.MatMenu[0] , 800-256-256, 600-128, 256, 128 , 255 );  //�������̽� ����      
	dsDrawTexImage( cInterFace.MatMenu[1] , 800-256, 600-64, 256, 64 , 255 );      //�������̽� ���� 

	//Ȯ�� �������̽� 
	DrawSprite(800,WinSizeY-64,lpGrowInterLeft,0,0,66,64,777);  // Ȯ���������̽� �÷�         

	for(i = 0; i < 3; i++){
		if(sinWinSizeX[i] == WinSizeX){
			for(j = 0; j < sinWinSizeXindex[i] ; j++){
				DrawSprite(800+66+(64*j),WinSizeY-64,lpGrowInterMiddle,0,0,64,64,777);
			}
		}
	}

	DrawSprite(800+66+(64*j),WinSizeY-64,lpGrowInterRight,0,0,40,64,777);


	//DrawSprite(100,200,lpGrowInterLeft,0,0,80,80);

	///////////////////////�ܻ�ȿ�� 
	for(i = 0 ; i < 3 ; i++){
		if(sShadowState[i].Flag){
			dsDrawColorBox( sShadowState[i].Color ,sShadowState[i].Posi.x,sShadowState[i].Posi.y,
				sShadowState[i].Size.x, sShadowState[i].Size.y );

		}
	}

	DrawSprite(319,500+(94-sinBmpLength[0]),lpLIFE,0,(94-sinBmpLength[0]),16,94 ); //������        
	DrawSprite(465,500+(94-sinBmpLength[1]),lpMP  ,0,(94-sinBmpLength[1]), 16,94); //����   
	DrawSprite(303,518+(76-sinBmpLength[2]),lpSTM ,0,(76-sinBmpLength[2]), 8,76);  //���׹̳� 


//	DrawSprite(319,500+(94-sinBmpLength[0]),lpLIFE,0,0,16, sinBmpLength[0]); //������   
//	DrawSprite(465,500+(94-sinBmpLength[1]),lpMP  ,0,0, 16,sinBmpLength[1]); //����   
//	DrawSprite(303,518+(76-sinBmpLength[2]),lpSTM ,0,0, 8,sinBmpLength[2]);  //���׹̳� 

	/*
	if(sinBmpLength[0] <= 123 && sinBmpLength[0]  !=0) //������ 
		DrawSprite(111+sinBmpLength[0]-4,4 ,lpLIFESpot,0,0, 4,17); //������ ���ٸ� 
	*/
/*
	if(sinChatEnter){
		dsDrawTexImage( MatChatDoc[0] , 0, 600-32-SubChatHeight, 256, 32 , 255 );    //ä��â����  
		dsDrawTexImage( MatChatDoc[1] , 256, 600-32-SubChatHeight, 128, 32 , 255 );  
	}

	if(ChatFlag){	
		if(SubChatHeight){
				dsDrawTexImage( MatChatSmall[0] , 0, 600-152-128-21, 256, 128 , 255 );     //����  ä��â 
				dsDrawTexImage( MatChatSmall[1] , 256, 600-152-128-21, 128, 128 , 255 );   //����  ä��â 
			}
			else{
				dsDrawTexImage( MatChatDefault[0] , 0, 600-256, 256, 256 , 255 );   //�⺻ ä��â 
				dsDrawTexImage( MatChatDefault[1] , 256, 600-256, 128, 256 , 255 ); //�⺻ ä��â 
			}
		
	}
*/ 
	if(ExitButtonClick){    
		dsDrawTexImage( MatExitMenu , 698, 504, 128, 64 , 255 );     //������ â   

		if(sinMenuBottonPosi ==1 )
			DrawSprite(698,504,lpMenuButton[0],0,0,96,19); //����  

		if(sinMenuBottonPosi ==2 )
			DrawSprite(692,517,lpMenuButton[1],0,0,93,20); //�ٽý���  

		if(sinMenuBottonPosi ==3 )
			DrawSprite(698,535,lpMenuButton[2],0,0,96,19); //������ 


	}

	//ä��Ŀ�� ǥ�� 
	if ( hFocusWnd && sinChatInputMode && !usehFocus) {   
	//if ( hFocusWnd && sinChatInputMode ) {  
		DrawSprite(7,580-SubChatHeight,lpChatShowButton,0,0,22,19); //ä��
	}
//	if(sinChatEnter){
//		DrawSprite(7,579-SubChatHeight,lpChatShowButton,0,0,22,19); //ä��
//	}

	if(DayOrNightFlag ==1){ //��     
		DrawSprite(363,589,lpSun,0,0,13,13); //��  
		DrawSprite(375,593,lpBar_Time,0,0,sinGageTime2,5); //������ 
	}
	else{	//��
		DrawSprite(426,589,lpMoon,0,0,13,13); //�� 
		DrawSprite(375+(50-sinGageTime2),593,lpBar_Time,(50-sinGageTime2),0,50,5); //������ 

	}

	/*
	//�ؿʹ� 
	if(SunandMoonFlag[0]) 
		DrawSprite(12,62,lpSun,0,0,12,12); //�� 

	if(SunandMoonFlag[1])
		DrawSprite(57,7,lpMoon,0,0,12,12); //��
	*/


	int TempGage = 0;

	/////////////��ų ������ 
	if(sinSkill.pLeftSkill){		 //���� ��ų 
		TempGage = (int)((41.0f*(float)sinSkill.pLeftSkill->GageLength)/35.0f);
		DrawSprite(338 ,542+(41-TempGage), lpGage[0],0,41-TempGage,16,41); 
	}

	if(sinSkill.pRightSkill){		//������ ��ų 
		TempGage = (int)((41.0f*(float)sinSkill.pRightSkill->GageLength)/35.0f);
		DrawSprite(446 ,542+(41-TempGage), lpGage[1],0,41-TempGage,16,41); 

	}

	/*
	if(SelectInterSkill){  //�������̽� ��ų ���� 
		DrawSprite(sLeftRightSkill[SelectInterSkill-1].BoxRect.left-3,
			sLeftRightSkill[SelectInterSkill-1].BoxRect.top-3,cSkill.lpSelectSkill,0,0,44,44);

	}
	*/

	
	//�г� ������ (�ӽ�)
	//DrawSprite(82,18,lpBar_Anger,0,0,27,40-ReSizelpGage); //������ 

	//�г� ��ư 
	//DrawSprite(82,18,lpButton_Anger,0,0,27,40-ReSizelpGage); //������ 

	//����ġ �ٸ� ���ڰŸ��� 
	if(sinPKWarningExpFlag){    
		if(sinPKWarnigExpTime <= 15)
			DrawSprite(485,508+(86-sinBmpLength[3]),lpBar_Exp,0,(86-sinBmpLength[3]),6,86 ); //����ġ        


	}
	else
		DrawSprite(485,508+(86-sinBmpLength[3]),lpBar_Exp,0,(86-sinBmpLength[3]),6,86 ); //����ġ        


	//�ð�
	//DrawSprite(505,588,lpBar_Time,0,0,sinBmpLength[4],5); //������ 

	if(!sInterFlags.RunFlag) //�޸��Ⱑ �ƴϸ� 
		DrawSprite(575,565,lpWalk,0,0,24,25); //�ȱ� 
	
	if(sInterFlags.CameraAutoFlag == 1) //ī�޶� �ڵ��̸� 
		DrawSprite(575+24,565,lpCameraImage[0],0,0,24,25);

	if(sInterFlags.CameraAutoFlag == 2) //ī�޶� �����̸�
		DrawSprite(575+24,565,lpCameraImage[1],0,0,24,25);


	if(sInterFlags.MapOnFlag)
	{
		DrawSprite(575+24+24,565,lpMapOnImage,0,0,24,25);
	}

	switch(sinRunCameraMapPosi){
	case 1:
		if(sInterFlags.RunFlag){//�޸���� 
			DrawSprite(575+12-(76/2),565-29,lpRunInfo,0,0,77,27);
		}
		else{ //�ȱ��̸� 
			DrawSprite(575+12-(76/2),565-29,lpWalkInfo,0,0,77,27);
			
		}
	break;
	case 2:
		if(sInterFlags.CameraAutoFlag == 0){ //ī�޶� ���� 
			DrawSprite(575+24+12-(76/2),565-29,lpCameraHandInfo,0,0,77,27);
			cHelpPet.PetMessage("*Camera_Hand",0);
		}
		if(sInterFlags.CameraAutoFlag == 2){ //ī�޶� ���� 
			DrawSprite(575+24+12-(76/2),565-29,lpCameraFixInfo,0,0,77,27);
			cHelpPet.PetMessage("*Camera_Fix",0);

		}
		if(sInterFlags.CameraAutoFlag ==1 ){//ī�޶� �ڵ� 
			DrawSprite(575+26+13-(76/2),565-29,lpCameraAutoInfo,0,0,77,27);
			cHelpPet.PetMessage("*Camera_Auto",0);
			
		}
	break;
	case 3:
		if(sInterFlags.MapOnFlag){ //�ʺ����ֱ� 
			DrawSprite(575+24+24+12-(76/2),565-29,lpMapOffInfo,0,0,77,27);
			
		}
		else{  //�ʼ���� 
			DrawSprite(575+24+24+12-(76/2),565-29,lpMapOnInfo,0,0,77,27);

		}
	break;

	}

	for(int t = 0 ; t < 6 ; t++){  
		if(MouseButtonPosi == t+1){
			if(InterButtonDown){
				DrawSprite(648+(t*25),560,lpButton2[t],0,0,25,27); //ĳ���� �������ͽ�

			}
			else{
				DrawSprite(648+(t*25),560,lpButton[t],0,0,25,27); //ĳ���� �������ͽ�
				DrawSprite(648-(77-23-1)+(t*25),560-27,lpInfoBox[t],0,0,77,27); //ĳ���� �������ͽ�
			}
		}
	}
/*

	switch(MouseButtonPosi){
	case 1:
		if(InterButtonDown)
			DrawSprite(651,567,lpButton2[0],0,0,24,25); //ĳ���� �������ͽ�
		else{
			DrawSprite(651,567,lpButton[0],0,0,24,25); //ĳ���� �������ͽ�
			DrawSprite(651-(77-23-1),567-27,lpInfoBox[0],0,0,77,27); //ĳ���� �������ͽ�
			
		}
	break;

	case 2:
		if(InterButtonDown)
			DrawSprite(678+1,576+1,lpButton2[1],0,0,,24,25); //�κ��丮 
		else {
			DrawSprite(678+1,576+1,lpButton[1],0,0,24,25); //�κ��丮 
			DrawSprite(678+1-(77-23-1),576+1-27,lpInfoBox[1],0,0,77,27); //�κ��丮 
		}
	break;

	case 3:
		if(InterButtonDown)
			DrawSprite(701+1,576+1,lpButton2[2],0,0,24,25); //��ų  
		else {
			DrawSprite(701+1,576+1,lpButton[2],0,0,24,25); //��ų  
			DrawSprite(701+1-(77-23-2),576+1-27,lpInfoBox[2],0,0,77,27); //��ų  
		}
	break;

	case 4:
		if(InterButtonDown)
			DrawSprite(724+1,576+1,lpButton2[3],0,0,24,25); //��Ƽ
		else{
			DrawSprite(724+1,576+1,lpButton[3],0,0,24,25); //��Ƽ
			DrawSprite(724+1-(77-23-2),576+1-27,lpInfoBox[3],0,0,77,27); //��Ƽ
		}
	break;

	case 5:
		if(InterButtonDown)
			DrawSprite(747+1,576+1,lpButton2[4],0,0,24,25); //����Ʈ
		else{ 
			DrawSprite(747+1,576+1,lpButton[4],0,0,24,25); //����Ʈ
			DrawSprite(747+1-(77-23-1),576+1-27,lpInfoBox[4],0,0,77,27); //����Ʈ
		}
	break;

	case 6:
		if(InterButtonDown)
			DrawSprite(770+1,576+1,lpButton2[5],0,0,24,25); //�ý��� 

		else{
			DrawSprite(770+1,576+1,lpButton[5],0,0,24,25); //�ý��� 
			DrawSprite(770+1-(77-23-1),576+1-27,lpInfoBox[5],0,0,77,27); //�ý��� 
		}
	break;

	}
*/

	//DrawClockArrow(61,57,1000);                   //�ð�ٴ��� �׸��� 
	//DrawCompass(63,52,lpCurPlayer->Angle.y&ANGCLIP);  //��ħ���� �׸��� 


	if(SkillNpcFlag){
		DrawSprite(SkillMasterMainXY.x,SkillMasterMainXY.y,lpReStartMain,0,0,188,128); //��ų ���� ���� 
		DrawSprite(SkillMasterMainXY.x+ReStartButtonRect[0][0],SkillMasterMainXY.y+ReStartButtonRect[0][1],lpSelectSkill_Main[0],0,0,44,44); 
		DrawSprite(SkillMasterMainXY.x+ReStartButtonRect[1][0],SkillMasterMainXY.y+ReStartButtonRect[1][1],lpSelectChangeJob[0],0,0,44,44); 
		if(SkillMasterIndex){
			if(SkillMasterIndex == 1 && SkillMasterOkFlag[0])//��ų 
				DrawSprite(SkillMasterMainXY.x+ReStartButtonRect[SkillMasterIndex-1][0],SkillMasterMainXY.y+ReStartButtonRect[SkillMasterIndex-1][1],lpSelectSkill_Main[1],0,0,44,44); 
			if(SkillMasterIndex == 2 && SkillMasterOkFlag[1])//���� 
				DrawSprite(SkillMasterMainXY.x+ReStartButtonRect[SkillMasterIndex-1][0],SkillMasterMainXY.y+ReStartButtonRect[SkillMasterIndex-1][1],lpSelectChangeJob[1],0,0,44,44); 
			if(SkillMasterIndex == 3)//���� 
				DrawSprite(SkillMasterMainXY.x+ReStartButtonRect[SkillMasterIndex-1][0],SkillMasterMainXY.y+ReStartButtonRect[SkillMasterIndex-1][1],lpReStartButton[2],0,0,44,44); 

		}
	}
	
	////////��ų��ư �ݱ� (�������̽��� �����ִ� ����� �̰����� �̵�)
	if(ShowSkillClose){ //�ݱ� ��ư 
		DrawSprite(269,571+(256-sinMoveKindInter[SIN_SKILL]),cShop.lpExit,0,0,20,20);
		DrawSprite(269,571-27+(256-sinMoveKindInter[SIN_SKILL]),cInvenTory.lpExitInfo,0 ,0 , 47,27);
	}
	//����Ʈ���ڸ� �����ش�.=====================================================================
	
	for(int k=0;k<10;k++){
		if(HaEffect[k].Flag){
			dsDrawTexImage(HaEffect[k].Mat, HaEffect[k].Posi.x,HaEffect[k].Posi.y,HaEffect[k].Size.x,
				HaEffect[k].Size.y,HaEffect[k].Alpha );  //�������̽� ����   
		}
	}
	//============================================================================================



	/////////////////////////////////// ä�� ���� ////////
	renderDevice.EndScene();
	HDC hdc;

	char strBuff2[256];
	char strBuff[256];
	int len=0;
	int x=0,y=0;

	renderDevice.lpDDSBack->GetDC(&hdc);

	SelectObject( hdc , hFont ); 
    SetBkMode( hdc, TRANSPARENT );

	char *lpInput;


	if ( hFocusWnd && sinChatInputMode && !usehFocus) {     
	//if ( hFocusWnd && sinChatInputMode ) { 
		//DrawSprite(7,582-SubChatHeight,lpChatShowButton,0,0,22,19); //ä��

		GetWindowText( hTextWnd , strBuff2 , 240 );
		strBuff2[242]=0x0D;
		len = lstrlen( strBuff2 );

		SetBkMode( hdc, TRANSPARENT );
		
		wsprintf( strBuff, "  %s" , strBuff2 );

		len=lstrlen( strBuff );       
		if ( len>=44 ) {
			lpInput = &strBuff[len-44];
			len=44;
		}
		else 
			lpInput = strBuff;

		if ( sinChatInputMode==1 ) {
			x = 16 ; y = 584-SubChatHeight;
		}
		else {
			x = 16 ; y = 584-SubChatHeight;
		}
		
		SetTextColor( hdc, RGB(0, 0, 0) );

#ifdef _LANGUAGE_VEITNAM
		dsTextLineOut( hdc , x+6, y , lpInput , len );
		SetTextColor( hdc, RGB(255, 255, 255) );
		dsTextLineOut( hdc , x+5 , y-1 , lpInput , len );
#else
		dsTextLineOut( hdc , x+1, y+1 , lpInput , len );
		SetTextColor( hdc, RGB(255, 255, 255) );
		dsTextLineOut( hdc , x , y , lpInput , len );
#endif
		
	}
	
#include "Language\\language.h"
#ifdef _LANGUAGE_JAPANESE
#include <imm.h>

		if ( ImeChatLen!=len || ImePositon.x!=len || ImePositon.y!=y ) {
			HIMC	h=ImmGetContext(hwnd);
			COMPOSITIONFORM	cf;

			int addPoint = 0;

			if(smConfig.ScreenSize.y == 600) addPoint = 0;
			else if(smConfig.ScreenSize.y == 768) addPoint = 163;
			else if(smConfig.ScreenSize.y == 1024) addPoint = 420;

			if ( h ) {
				cf.dwStyle = CFS_FORCE_POSITION;
				cf.ptCurrentPos.x = x+6*len;
				cf.ptCurrentPos.y = y + addPoint;
				ImmSetCompositionWindow( h, &cf );

				ImePositon.x = len;
				ImePositon.y = y;
				ImeChatLen = len;
			}
		}
#endif


	if ( sinChatDisplayMode ) {                   
		if ( sinChatDisplayMode==1 ) {
			if(NoticeChatIndex == 1){ //����ä�ø�

				//����ä��
				x = 16 ; y = 570-SubChatHeight-125; i = 3;
				DisplayChatMessage3( hdc , x , y , 44 , i , 5 );

				//�Ϲ�ä��
				x = 16 ; y = 570-SubChatHeight-20; i = 8;
				DisplayChatMessage2( hdc , x , y , 44 , i );

			}
			else if(NoticeChatIndex == 2){
				x = 16 ; y = 570-SubChatHeight-20; i = 12;
				DisplayChatMessage3( hdc , x , y , 44 , i , 5 );
			}
			else{
				x = 16 ; y = 570-SubChatHeight-20; i = 12;
				DisplayChatMessage2( hdc , x , y , 44 , i );
			}
		}
		else {
			if(NoticeChatIndex == 2){
				x = 16 ; y = 570-SubChatHeight-5; i = 8;
				DisplayChatMessage3( hdc , x , y , 44 , i , 5 );
			}
			else{
				x = 16 ; y = 570-SubChatHeight-5; i = 8;
				DisplayChatMessage2( hdc , x , y , 44 , i );
			}
		}
		//ä�� ���� ȭ�鿡 ǥ��
		//if(NoticeChatIndex != 2 )
		//	DisplayChatMessage2( hdc , x , y , 44 , i );
	}

	renderDevice.lpDDSBack->ReleaseDC(hdc);

	int dsDrawOffsetArrayBackup = dsDrawOffsetArray;

	dsDrawOffsetArray = dsARRAY_TOP;

////////////////////// ��ų 
	if(SkillMasterFlag == 1 ){ 
		for(i = 0; i < SkillUpInfo.x ; i++){
			for(int j = 0; j< SkillUpInfo.y ; j++){
				if(i == 0 && j == 0 ) //������� 
					dsDrawTexImage( cItem.MatItemInfoBox_TopLeft , SkillUpInfoPosi.x+(i*16) , SkillUpInfoPosi.y+(j*16), 16, 16 , 255 );
				if(j == 0 && i !=0 && i+1 < SkillUpInfo.x ) //���
					dsDrawTexImage( cItem.MatItemInfoBox_TopCenter , SkillUpInfoPosi.x+(i*16) , SkillUpInfoPosi.y+(j*16), 16, 16 , 255 );
				if(j == 0 && i+1 == SkillUpInfo.x) //�������
					dsDrawTexImage( cItem.MatItemInfoBox_TopRight , SkillUpInfoPosi.x+(i*16) , SkillUpInfoPosi.y+(j*16), 16, 16 , 255 );

				if(i == 0 && j != 0 && j+1 != SkillUpInfo.y) //���� �ٰŸ� 
					dsDrawTexImage( cItem.MatItemInfoBox_Left , SkillUpInfoPosi.x+(i*16) , SkillUpInfoPosi.y+(j*16), 16, 16 , 255 );
				if(i != 0 && j != 0 && i+1 !=SkillUpInfo.x && j+1 !=SkillUpInfo.y) //��� �丷
					dsDrawTexImage( cItem.MatItemInfoBox_Center , SkillUpInfoPosi.x+(i*16) , SkillUpInfoPosi.y+(j*16), 16, 16 , 255 );
				if(i+1 == SkillUpInfo.x && j != 0 && j+1 != SkillUpInfo.y) //���� �ٰŸ� 
					dsDrawTexImage( cItem.MatItemInfoBox_Right , SkillUpInfoPosi.x+(i*16) , SkillUpInfoPosi.y+(j*16), 16, 16 , 255 );
				
				if(i == 0 && j+1 == SkillUpInfo.y) //�عٴ� ����
					dsDrawTexImage( cItem.MatItemInfoBox_BottomLeft , SkillUpInfoPosi.x+(i*16) , SkillUpInfoPosi.y+(j*16), 16, 16 , 255 );
				if(i != 0 && j+1 == SkillUpInfo.y && i+1 !=SkillUpInfo.x)//�عٴ� ���
					dsDrawTexImage( cItem.MatItemInfoBox_BottomCenter , SkillUpInfoPosi.x+(i*16) , SkillUpInfoPosi.y+(j*16), 16, 16 , 255 );
				if(j+1 == SkillUpInfo.y && i+1 ==SkillUpInfo.x)//�عٴ� ������ 
					dsDrawTexImage( cItem.MatItemInfoBox_BottomRight , SkillUpInfoPosi.x+(i*16) , SkillUpInfoPosi.y+(j*16), 16, 16 , 255 );


			}
		}
		
		if(ShowSkillUpInfo){
			for( i = 0 ; i < 10 ; i ++ ) {
				if(	SkillIconIndex[i]){
					DrawSprite(SkillUpInfoPosi.x+SkillIconPosi.x+(i*18)-10,SkillUpInfoPosi.y+SkillIconPosi.y,cSkill.lpWeaponIcon[SkillIconIndex[i]],0,0,18,24); 
				}
			}

		}
		
		renderDevice.EndScene();

		HDC	hdc;

		int BoldFlag = 0;
		int MasteryLen = 0;
		int MasteryShowX = 0;

		char strBuff[128];
		memset(strBuff,0,sizeof(strBuff));
		renderDevice.lpDDSBack->GetDC(&hdc);
		SelectObject( hdc, sinFont);
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, RGB(255,255,255) );

		if(SkillMasterFlag == 1 && !ShowSkillUpInfo){
			SelectObject( hdc, sinBoldFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(222,231,255) );
			dsTextLineOut(hdc,100,225-80,SkillUseInfoText[0]   , lstrlen(SkillUseInfoText[0]) );
			
			//��ų����Ʈ�� ����ش�.
			if(sinSkill.SkillPoint||sinSkill.SkillPoint4){
				SelectObject( hdc, sinFont);
				SetBkMode( hdc, TRANSPARENT );
				SetTextColor( hdc, RGB(255,255,255) );

				dsTextLineOut(hdc,100,260-80,SkillUseInfoText[1], lstrlen(SkillUseInfoText[1]) );
				dsTextLineOut(hdc,100,280-80,SkillUseInfoText[2], lstrlen(SkillUseInfoText[2]) );
				wsprintf(strBuff,sinMsgSkillPoint,sinSkill.SkillPoint);
				SelectObject( hdc, sinBoldFont);
				SetBkMode( hdc, TRANSPARENT );
				SetTextColor( hdc, RGB(247,243,193) );
				dsTextLineOut(hdc,100,310-80,strBuff   , lstrlen(strBuff) );

				wsprintf(strBuff,sinMsgEliteSkillPoint,sinSkill.SkillPoint4);  
				SelectObject( hdc, sinBoldFont);
				SetBkMode( hdc, TRANSPARENT );
				SetTextColor( hdc, RGB(147,243,193) );
				dsTextLineOut(hdc,103,310-55,strBuff   , lstrlen(strBuff) );
			}
			else{
				SelectObject( hdc, sinFont);
				SetBkMode( hdc, TRANSPARENT );
				SetTextColor( hdc, RGB(255,64,64) );

				dsTextLineOut(hdc,100,260-80,SkillUseInfoText[3], lstrlen(SkillUseInfoText[3]) );
				dsTextLineOut(hdc,100,280-80,SkillUseInfoText[4], lstrlen(SkillUseInfoText[4]) );

			}


		}
		renderDevice.lpDDSBack->ReleaseDC(hdc);
		renderDevice.BeginScene();
	}

	//���� ��ư
	if(SkillMasterFlag && !ShowSkillUpInfo && (CheckQuestButtonShow1 ||CheckQuestButtonShow2 ||CheckQuestButtonShow3) )    //��ų������ �����ִ� �ɼ��� �ƴҰ�쿡�� ���� ��ư�� ���� 
	{
		if(sinChangeJobButtonShow || sinChangeJobButtonShow2 || sinChangeJobButtonShow3 || sinChangeJobButtonShow4 )
		{
			DrawSprite(240,380,lpSelectChangeJob[1],0,0,44,44); 
			nCheckSkillDraw = 1;
		}
		else
		{
			DrawSprite(240,380,lpSelectChangeJob[0],0,0,44,44); 
			nCheckSkillDraw = 2;
		}
	}

	if(SkillMasterFlag && !ShowSkillUpInfo && (nCheckSkillDraw == 1))
	{
		DrawSprite(240,380,lpSelectChangeJob[1],0,0,44,44); 
	}

	if(SkillMasterFlag && !ShowSkillUpInfo && (nCheckSkillDraw == 2))
	{
		DrawSprite(240,380,lpSelectChangeJob[0],0,0,44,44); 
	}

	dsDrawOffsetArray = dsDrawOffsetArrayBackup;

	/////////////////////��ų ���� �ڽ� 
	if(SkillInfoShowFlag && !ShowSkillUpInfo){ 
		if(SkillInfoShowFlag == 2)
			dsDrawOffsetArray = dsARRAY_TOP;
		for(i = 0; i < SkillBoxSize.x ; i++){
			for(int j = 0; j< SkillBoxSize.y ; j++){
				if(i == 0 && j == 0 ) //������� 
					dsDrawTexImage( cItem.MatItemInfoBox_TopLeft , SkillInfoBoxPosi.x+(i*16) , SkillInfoBoxPosi.y+(j*16), 16, 16 , 255 );
				if(j == 0 && i !=0 && i+1 < SkillBoxSize.x ) //���
					dsDrawTexImage( cItem.MatItemInfoBox_TopCenter , SkillInfoBoxPosi.x+(i*16) , SkillInfoBoxPosi.y+(j*16), 16, 16 , 255 );
				if(j == 0 && i+1 == SkillBoxSize.x) //�������

					dsDrawTexImage( cItem.MatItemInfoBox_TopRight , SkillInfoBoxPosi.x+(i*16) , SkillInfoBoxPosi.y+(j*16), 16, 16 , 255 );

				if(i == 0 && j != 0 && j+1 != SkillBoxSize.y) //���� �ٰŸ� 
					dsDrawTexImage( cItem.MatItemInfoBox_Left , SkillInfoBoxPosi.x+(i*16) , SkillInfoBoxPosi.y+(j*16), 16, 16 , 255 );
				if(i != 0 && j != 0 && i+1 !=SkillBoxSize.x && j+1 !=SkillBoxSize.y) //��� �丷
					dsDrawTexImage( cItem.MatItemInfoBox_Center , SkillInfoBoxPosi.x+(i*16) , SkillInfoBoxPosi.y+(j*16), 16, 16 , 255 );
				if(i+1 == SkillBoxSize.x && j != 0 && j+1 != SkillBoxSize.y) //���� �ٰŸ� 
					dsDrawTexImage( cItem.MatItemInfoBox_Right , SkillInfoBoxPosi.x+(i*16) , SkillInfoBoxPosi.y+(j*16), 16, 16 , 255 );
				
				if(i == 0 && j+1 == SkillBoxSize.y) //�عٴ� ����
					dsDrawTexImage( cItem.MatItemInfoBox_BottomLeft , SkillInfoBoxPosi.x+(i*16) , SkillInfoBoxPosi.y+(j*16), 16, 16 , 255 );
				if(i != 0 && j+1 == SkillBoxSize.y && i+1 !=SkillBoxSize.x)//�عٴ� ���
					dsDrawTexImage( cItem.MatItemInfoBox_BottomCenter , SkillInfoBoxPosi.x+(i*16) , SkillInfoBoxPosi.y+(j*16), 16, 16 , 255 );
				if(j+1 == SkillBoxSize.y && i+1 ==SkillBoxSize.x)//�عٴ� ������ 
					dsDrawTexImage( cItem.MatItemInfoBox_BottomRight , SkillInfoBoxPosi.x+(i*16) , SkillInfoBoxPosi.y+(j*16), 16, 16 , 255 );


			}
		}
		for( i = 0 ; i < 10 ; i ++ ) {
			if(	SkillIconIndex[i]){
				DrawSprite(SkillInfoBoxPosi.x+SkillIconPosi.x+(i*18),SkillInfoBoxPosi.y+SkillIconPosi.y,cSkill.lpWeaponIcon[SkillIconIndex[i]],0,0,18,24); 

			}
		}
	}
	///////// �����, ����ġ ����� �۾��� �ߺ��̰����� ��� 
	if(InfoMessageBoxPos.x != 0 || InfoMessageBoxPos.y != 0 ){ //ǥ����ġ�� �������� ǥ���Ѵ�   
		dsDrawTexImage( cItem.MatItemInfoBox_Center , InfoMessageBoxPos.x-5,InfoMessageBoxPos.y-1, lstrlen(szInfoMsgBuff5)*6, 13 , 255 );
	}

	//����ġ �޹���� ���� ��~ (�������̽� �ٲ�)
	//if(ShowExpPercentFlag)  
	//	dsDrawTexImage( cItem.MatItemInfoBox_Center , 489-5,543-1, lstrlen(szInfoMsgBuff6)*6, 13 , 255 );

 
	//��ų �ʱ�ȭ �ڽ��� ���� 
	cSkill.ShowClearSkillPointButton(SkillInfoBoxPosi.x + (SkillBoxSize.x*16) , SkillInfoBoxPosi.y + (SkillBoxSize.y*16));

	renderDevice.BeginScene();

}
/*----------------------------------------------------------------------------*
*							     ����  
*-----------------------------------------------------------------------------*/	
DWORD dwCheckTime; //�ð��� üũ�Ѵ� 
int   CheckNotUseSkillTime[3] = {0,0,0};
int   CheckNotUseSkillCount[3] = {0,0,0};
int   CheckTime2[3] ={0,0,0};
int   CheckExpFlag = 0;
int   CheckExpFlag2 = 0;
int   CheckExpTime = 0;

DWORD dwTestKeyTime = 0;
int   TeskKeyMessageCnt = 0;
char *szTestKeyMsg[6] = {"��!  &(^_^)&    ",
						 "��!  /(^_^)/   ",
						 "��!  *(^_^)*  ",
						 "",
						 "",
						 "",};
					
void cINTERFACE::Main()
{
	if ( GetFieldMapCursor()==TRUE && cInterFace.sInterFlags.MapOnFlag ) {
		cHelpPet.PetMessage("*Mini_Map",0);
	}


	/////////////////// ä�� (�̹��� ������ �ֻ����� ���� ������) 
	if(sinChatEnter){       

		/*
		if(SubChatHeight){                 
			dsDrawTexImage( cInterFace.MatChatDoc[0] , 4, 576-SubChatHeight, 242, 32 , 255 );    //ä��â����  
			dsDrawTexImage( cInterFace.MatChatDoc[1] , 246, 576-SubChatHeight, 164, 32 , 255 );    //ä��â����  
		}
		else
			dsDrawTexImage( cInterFace.MatChatDoc[0] , 4, 576-SubChatHeight, 296, 32 , 255 );    //ä��â����  
		*/

	}

	

	if(cInterFace.ChatFlag){	         
		if(SubChatHeight){  
			if (0< pCursorPos.x  && 300 > pCursorPos.x  && 
					294 < pCursorPos.y  && 294+116 > pCursorPos.y  ){
						if(!ChatKindIndexNum){
							cHelpPet.PetMessage("*ChatBox",0);
						}

			}
		}
		else{
			if (0< pCursorPos.x  && 305 > pCursorPos.x  && 
					600-192 < pCursorPos.y  && 600-192+210 > pCursorPos.y  ){
						if( !ChatKindIndexNum){
							cHelpPet.PetMessage("*ChatBox",0);
						}

			}
		}
	}

	//Pet �޼���
	if (365< pCursorPos.x  && 435 > pCursorPos.x  && 
			589 < pCursorPos.y  && 596 > pCursorPos.y  ){
				cHelpPet.PetMessage("*DayandNight",0);

	}


	GetWordTimeDisplay(); //�ð踦 �׸��� 
	//PK�� ����ġ ��� �ش� 
	
	if(sinPKWarningExpFlag){ 
		sinPKWarnigExpTime++;
		if(sinPKWarnigExpTime > 30)
			sinPKWarnigExpTime = 0;

	}
	else
		sinPKWarnigExpTime = 0;


	//////////////�׽�Ʈ Ű����
	if(TestKeyAniFlag){
		dwTestKeyTime++;
		if(dwTestKeyTime > 30){
			dwTestKeyTime = 0;
			SendChatMessageToServer(szTestKeyMsg[TeskKeyMessageCnt]);
			TeskKeyMessageCnt++;
			if(TeskKeyMessageCnt >= 6){
				TestKeyAniFlag = 0;
				dwTestKeyTime  = 0;
				TeskKeyMessageCnt = 0;

			}
		}
	}


	sinMenuBottonPosi = 0;
	
	if ( lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD && lpCurPlayer->FrameCounter>100 ) {
		ReStartFlag =1;
		if(!CheckExpFlag && !CheckExpFlag2){
			if(!CheckFieldContExp())
				RestartCheckExp = 1;

			CheckExpFlag = 1;
		}

	}
	if(CheckExpFlag2){ //���� 
		CheckExpTime++;
		if(CheckExpTime > 100){
			CheckExpFlag2 = 0;
			CheckExpTime = 0;

		}
	}

	if(dwGameHour >= 8 && dwGameHour <=18){
		SunandMoonFlag[0] = 1;
		SunandMoonFlag[1] = 0;
	}
	else{
		SunandMoonFlag[0] = 0;
		SunandMoonFlag[1] = 1;

	}
	if(sinLoadOK){
		if(!InitStateFlag)InitState();
		CheckingState();
	}

	MouseButtonPosi = 0;
	for(int i=0;i<6;i++){
		if ( ButtonRect[i][0] < pCursorPos.x  && ButtonRect[i][2] > pCursorPos.x  && 
			ButtonRect[i][1] < pCursorPos.y  && ButtonRect[i][3] > pCursorPos.y  ){
			MouseButtonPosi = i+1;
			switch(MouseButtonPosi){
				case 1:
					cHelpPet.PetMessage("*CarStatus",0);
				break;
				case 2:
					cHelpPet.PetMessage("*InvenTory",0);
				break;
				case 3:
					cHelpPet.PetMessage("*Skill",0);
				break;
				case 4:
					cHelpPet.PetMessage("*Party",0);
				break;
				case 5:
					cHelpPet.PetMessage("*Quest",0);
				break;
				case 6:
					cHelpPet.PetMessage("*System",0);
				break;

			}
		}
	}
	sinRunCameraMapPosi = 0; //�ʱ�ȭ 
	for(int i=0 ;i<3 ; i++){
		if ( sinRunCameraMap[i][0] < pCursorPos.x  && sinRunCameraMap[i][2] > pCursorPos.x  && 
			sinRunCameraMap[i][1] < pCursorPos.y  && sinRunCameraMap[i][3] > pCursorPos.y  ){
			sinRunCameraMapPosi = i+1;
			switch(sinRunCameraMapPosi){
				case 1:
					cHelpPet.PetMessage("*RunAndWalk",0);
				break;
				case 2:
					//cHelpPet.PetMessage("*RunAndWalk",0);
				break;
				case 3:
					//cHelpPet.PetMessage("*Mini_Map",0);
				break;

			}
		}
	}

	if(ExitButtonClick){
		if ( 699 < pCursorPos.x  && 795 > pCursorPos.x  && 
			 535 < pCursorPos.y  && 555 > pCursorPos.y  )
			 sinMenuBottonPosi = 3;

		if ( 698 < pCursorPos.x  && 795 > pCursorPos.x  && 
			 504 < pCursorPos.y  && 524 > pCursorPos.y  )
			 sinMenuBottonPosi = 1;
		
		

	

		///////////////�ɼǹ�  �ٽý����� ���°���� ��ġ�� ���� 
		
		/*
		for(i=0 ;i<3 ; i++)
			if ( sinMenuRect[i][0] < pCursorPos.x  && sinMenuRect[i][2] > pCursorPos.x  && 
				 sinMenuRect[i][1] < pCursorPos.y  && sinMenuRect[i][3] > pCursorPos.y  )
				sinMenuBottonPosi = i+1;
		*/
			
	}

	////////////////////  ä�ð��� (�����ϴϱ� �Ȱǵ帰��)
	if(cInvenTory.OpenFlag || cCharStatus.OpenFlag || cSkill.OpenFlag){
		SubChatHeight = 170;
		sinChatInputMode =0;
		sinChatDisplayMode = 0;
		if(SubChatHeight>=170){
			SubChatHeight = 170;

		}

	}
	else
		SubChatHeight =0;

	if(sinChatEnter){
		if(SubChatHeight >=170)
			sinChatInputMode = 2;
		else 
			sinChatInputMode = 1;

	}
	else
		sinChatInputMode = 0;

	if(ChatFlag){
		if(SubChatHeight >=170)
			sinChatDisplayMode = 2;
		else 
			sinChatDisplayMode = 1;

	}
	else
		sinChatDisplayMode = 0;

	//////////////////////ä�ð��� Flag �� 

	CheckingState(); // ����� ,��� , �ٷ��� ��ȭġ�� üũ�ؼ� ����ġ��ŭ Bmp�� �����ش� 
	ShowParaState(); // ����� ��� �ٷ� ����ġ ��ġ�� ǥ���Ѵ� 
	ShowExpPercent(); //����ġ�� ��������� �����ش�


	///////////////////// ���� (���� �ٽý��� �ʵ� , ���� , ���� )
	ReStartIndex = 0; //�ʱ�ȭ 
	memset(szReStartMsg,0,lstrlen(szReStartMsg)); //�ʱ�ȭ 
	memset(szReStartMsg2,0,lstrlen(szReStartMsg2)); //�ʱ�ȭ 

	if(ReStartFlag){
		sinMenuBottonPosi = 0;
		for(int i=0 ;i<3 ; i++){
			if ( ReStartMainXY.x+ReStartButtonRect[i][0] < pRealCursorPos.x  && ReStartMainXY.x+ReStartButtonRect[i][2] > pRealCursorPos.x  && 
				ReStartMainXY.y+ReStartButtonRect[i][1] < pRealCursorPos.y  && ReStartMainXY.y+ReStartButtonRect[i][3] > pRealCursorPos.y  ){
				if(RestartCheckExp){ //����ġ�� �����Ͽ� �ʵ忡�� �����Ҽ����� �ܿ� 
					if( i ==0 ){
						ReStartIndex = i+1;
						lstrcpy(szReStartMsg,ReStartMsg[3]);
						lstrcpy(szReStartMsg2,ReStartMsg2[3]);

					}
					else{
						ReStartIndex = i+1;
						lstrcpy(szReStartMsg,ReStartMsg[i]);
						lstrcpy(szReStartMsg2,ReStartMsg2[i]);
					}

				}
				else if(i ==0&&lpCurPlayer->OnStageField>=0 && StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_CASTLE){
					ReStartIndex=0;
				}
				else{
					ReStartIndex = i+1;
					lstrcpy(szReStartMsg,ReStartMsg[i]);
					lstrcpy(szReStartMsg2,ReStartMsg2[i]);
				}

			}
		}
		dwCheckTime++;
		if(dwCheckTime > 70*20){ //10�ʰ� �Ѿ�� 
			ReStartFlag = 0; //�÷� �ʱ�ȭ 
			ReStartOptionIndex = RESTART_TOWN; //Ÿ��� �����Ѵ� 
			CheckExpFlag = 0;
			CheckExpFlag2 = 1;
			RestartCheckExp = 0; //�÷����� �ʱ�ȭ�Ѵ�
		}
	
	}
	else
		dwCheckTime = 0; //�÷��� ������ �ʱ�ȭ�Ѵ� 
	///////////////////// ��ų , ���� 
	SkillMasterIndex = 0; //�ʱ�ȭ 
	memset(szSkillMasterMsg,0,lstrlen(szSkillMasterMsg)); //�ʱ�ȭ 
	memset(szSkillMasterMsg2,0,lstrlen(szSkillMasterMsg2)); //�ʱ�ȭ 

	/////////Ű���� ��ų 
	if(SkillNpcFlag){
		
		sinMenuBottonPosi = 0;
		for ( int i = 0; i<3; i++ )
		{
			if ( SkillMasterMainXY.x+ReStartButtonRect[i][0] < pCursorPos.x  && SkillMasterMainXY.x+ReStartButtonRect[i][2] > pCursorPos.x  && 
				SkillMasterMainXY.y+ReStartButtonRect[i][1] < pCursorPos.y  && SkillMasterMainXY.y+ReStartButtonRect[i][3] > pCursorPos.y  ){
				SkillMasterIndex = i+1;
				lstrcpy(szSkillMasterMsg,SkillMaster[i]);
				lstrcpy(szSkillMasterMsg2,SkillMaster2[i]);

			}
		}
	}
	//��ų ������� ������ ȿ�� 
	for(int t = 0; t < 3 ; t++){
		if(NotUseSkillElement[t]){
			CheckNotUseSkillTime[t]++;
			if(CheckNotUseSkillTime[t] > 12 ){
				if(NotUseSkillEffect[t])NotUseSkillEffect[t] = 0;
				else NotUseSkillEffect[t] = 1;
				CheckNotUseSkillTime[t] = 0;
				CheckNotUseSkillCount[t]++;
				if(CheckNotUseSkillCount[t] > 4){
					CheckNotUseSkillCount[t] = 0;
					CheckNotUseSkillTime[t] = 0;
					NotUseSkillEffect[t] = 0;
					NotUseSkillElement[t] = 0;
					SkillMouseButtonPosi[0] = 0;
					SkillMouseButtonPosi[1] = 0;


				}

			}
		}
		if(NotUseSkillEffect[t]){
			CheckTime2[t]++;	//�������� �Ⱥ��δٰ� �ؼ� ��������� (��ġ���)

		}
		else
			CheckTime2[t] = 0;
		if(CheckTime2[t] > 20) //���⼭ �������ش� 
			NotUseSkillEffect[t] = 0;


	}
	
	//����Ʈ���ڸ� �����ش�====================================================
	for(int k = 0 ; k < MAX_HAEFFECTNUM ; k++){      
		if(HaEffect[k].Flag){ 
			if(HaEffect[k].Time<3000)
				HaEffect[k].Time+=20;
			
			switch(HaEffect[k].Time/255){
				case 0:
					HaEffect[k].Alpha=255;
				break;
				case 1:
					HaEffect[k].Alpha=255;
				break;
				case 2:	
				case 3:
				case 4:
				case 5:
				case 6:
					//���ڰ� ������� ȿ��
					HaEffect[k].Alpha-=10;
					HaEffect[k].Posi.x-=1;
					HaEffect[k].Posi.y-=1; 
					HaEffect[k].Size.x+=3;
					HaEffect[k].Size.y+=3;
					if(HaEffect[k].Alpha<=0){
						HaEffect[k].Alpha=0;
						HaEffect[k].Time=0;
						HaEffect[k].Flag=0;
					}
				break;
			};
		}
	}
	//===========================================================================

}
/*----------------------------------------------------------------------------*
*							     ���� 
*-----------------------------------------------------------------------------*/	
void cINTERFACE::Close()
{
	Release();
}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/	
void cINTERFACE::LButtonDown(int x , int y)
{
	
	switch(sinRunCameraMapPosi){
		case 1:
			//cHelpPet.PetMessage("*RunAndWalk",1);
		break;
		case 3:
			//cHelpPet.PetMessage("*Mini_Map",1);
		break;
	}
	/*

	if(sInterFlags.CameraAutoFlag == 0){ //ī�޶� ���� 
		cHelpPet.PetMessage("*Camera_Hand",1);
	}
	if(sInterFlags.CameraAutoFlag == 2){ //ī�޶� ���� 
		cHelpPet.PetMessage("*Camera_Fix",1);
	}
	if(sInterFlags.CameraAutoFlag ==1 ){//ī�޶� �ڵ� 
		cHelpPet.PetMessage("*Camera_Auto",1);
	}

	*/

	//Pet �޼���
	//PetMessage
	if (365< pCursorPos.x  && 435 > pCursorPos.x  &&  
			589 < pCursorPos.y  && 596 > pCursorPos.y  ){
				cHelpPet.PetMessage("*DayandNight",1);

	}

	//Pet �޼���
	if(pCursorPos.x > 315 && pCursorPos.x < 337 && pCursorPos.y > 500 && pCursorPos.y < 594){//������ 
		cHelpPet.PetMessage("*Bar_Life",1); 
	}
	if(pCursorPos.x > 463 && pCursorPos.x < 483 && pCursorPos.y > 498 && pCursorPos.y < 595){//���
		cHelpPet.PetMessage("*Bar_Mana",1);
	}
	if(pCursorPos.x > 300 && pCursorPos.x < 313 && pCursorPos.y > 513 && pCursorPos.y < 595){//�ٷ�
		cHelpPet.PetMessage("*Bar_Stamina",1);
	}	
	if(pCursorPos.x > 483 && pCursorPos.x < 483+10 && pCursorPos.y > 513 && pCursorPos.y < 595){//����ġ
			cHelpPet.PetMessage("*Bar_Exp",1);
	}	
	if(pCursorPos.x > 500 && pCursorPos.x < 570 && pCursorPos.y > 540 && pCursorPos.y < 594){//����ġ 
		cHelpPet.PetMessage("*Exp_Num",1);
	}
	if ( GetFieldMapCursor()==TRUE && cInterFace.sInterFlags.MapOnFlag ) {
		cHelpPet.PetMessage("*Mini_Map",1); 

	}




	if(cInterFace.ChatFlag){	         
		if(SubChatHeight){ 
			if (0< pCursorPos.x  && 300 > pCursorPos.x  && 
					294 < pCursorPos.y  && 294+116 > pCursorPos.y  ){
						if(!ChatKindIndexNum){
							cHelpPet.PetMessage("*ChatBox",1);
						}

			}
		}
		else{
			if (0< pCursorPos.x  && 305 > pCursorPos.x  && 
					600-192 < pCursorPos.y  && 600-192+210 > pCursorPos.y  ){
						if(!ChatKindIndexNum){
							cHelpPet.PetMessage("*ChatBox",1);
						}

			}
		}
	}

	SkillMouseButtonPosi[0] = 1;
	ChangeJobButtonclick = 0;
	ReStartOptionIndex = 0; //�ε��� �ʱ�ȭ 
	//ä��â�ø��� 
	if(x > 7 && 22 > x && y > 579-SubChatHeight && y < 579+19-SubChatHeight){ 
		if(!ChatFlag)ChatFlag = 1;
		else ChatFlag = 0;
		if(ChatFlag)sinChatEnter = 1;
	}
	switch(sinRunCameraMapPosi){
	case 1: //�ȱ� �޸��� 
		if(sInterFlags.RunFlag)sInterFlags.RunFlag = 0;
		else sInterFlags.RunFlag =1;
	break;
	case 2: //ī�޶� �ڵ� ���� 
		sInterFlags.CameraAutoFlag++;
		if(sInterFlags.CameraAutoFlag > 2)sInterFlags.CameraAutoFlag = 0;

	break;
	case 3: //����� 
		if(sInterFlags.MapOnFlag)sInterFlags.MapOnFlag = 0;
		else sInterFlags.MapOnFlag = 1;

	break;
	
	};

	if(sinMenuBottonPosi == 3){ //������ �����Ѵ� 
			QuitGame();
	}

	char *HelpFilePath = "image\\Sinimage\\Help\\Help.sin"; 
	if(sinMenuBottonPosi == 1){ //������ ���� (ȣ���������� -50�ش޶���)
			cSinHelp.sinShowHelp(SIN_HELP_KIND_NORMAL,208,61-50,11,11, RGBA_MAKE(19,27,20,165),HelpFilePath); 
			ExitButtonClick = 0; 
			HelpBoxKindIndex = SIN_HELP;
	}

	if(MouseButtonPosi){ //��ư�����ǿ��� ���콺 ��ư�� Ŭ�������� 
		InterButtonDown = 1;
		switch(MouseButtonPosi){ //PetMessage
			case 1:
				cHelpPet.PetMessage("*CarStatus",1);
			break;
			case 2:
				cHelpPet.PetMessage("*InvenTory",1);
			break;
			case 3:
				cHelpPet.PetMessage("*Skill",1);
			break;
			case 4:
				cHelpPet.PetMessage("*Party",1);
			break;
			case 5:
				cHelpPet.PetMessage("*Quest",1);
			break;
			case 6:
				cHelpPet.PetMessage("*System",1);
			break;
		}

	}
	if(ReStartFlag){    //���� �ɼ� ��ư 
		switch(ReStartIndex){
		case RESTART_TOWN:
			ReStartOptionIndex = RESTART_TOWN;
			ReStartFlag = 0;
			RestartCheckExp = 0; //�ʱ�ȭ���ش� 
		
		break;
		case RESTART_FEILD:
			if(RestartCheckExp)break;
			ReStartOptionIndex = RESTART_FEILD;
			ReStartFlag = 0;
			RestartCheckExp = 0; //�ʱ�ȭ���ش� 
		
		break;
		case RESTART_EXIT:
			ReStartOptionIndex = RESTART_EXIT;
			ReStartFlag = 0;
			RestartCheckExp = 0; //�ʱ�ȭ���ش� 

		break;

		}

		CheckExpFlag2 = 1;
		CheckExpFlag = 0;

	}
	//��ų , ���� 
	if(SkillMasterIndex){

		if(SkillMasterIndex==1 &&  SkillMasterOkFlag[0]){
			SkillMasterFlag = SkillMasterIndex; //��ų 
			SkillNpcFlag = 0;
			cSkill.OpenFlag = 1;
			cInterFace.CheckAllBox(SIN_SKILL); //â�� �ϳ��� ���� 

		}
		if(SkillMasterIndex==2 &&  SkillMasterOkFlag[1]){
			SkillMasterFlag = SkillMasterIndex; //���� 
			SkillNpcFlag = 0;
			////////////////////// �޼��� �ڽ��� ����ش� 
			sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1);
			cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,sinJobList->szName2);


		}

		if(SkillMasterIndex==3){
			SkillMasterFlag = SkillMasterIndex; //����  
			SkillNpcFlag = 0;
		}
	}

}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cINTERFACE::LButtonUp(int x , int y)
{
	if(MouseButtonPosi){
		switch(MouseButtonPosi){
		case 1: //ĳ���� �������ͽ� 
			if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
			else cCharStatus.OpenFlag = SIN_OPEN;
			cInterFace.CheckAllBox(SIN_CHARSTATUS); //â�� �ϳ��� ���� 
			if(cCharStatus.OpenFlag){
				if(!sinFireShow){
					StartMenuFlame(0,350);
					sinPlaySound( 0 );
				
				}
				else
					sinPlaySound(SIN_SOUND_SHOW_INTER);
				sinFireShow = 1;
			}

		break;
		case 2: //�κ��丮 
			if(cInvenTory.OpenFlag)cInvenTory.OpenFlag = SIN_CLOSE;
			else cInvenTory.OpenFlag = SIN_OPEN;
			cInterFace.CheckAllBox(SIN_INVENTORY); //â�� �ϳ��� ���� 
			if(cInvenTory.OpenFlag){
				if(!sinFireShow){
					StartMenuFlame(0,350);
					sinPlaySound( 0 );
				}
				else
					sinPlaySound(SIN_SOUND_SHOW_INTER);

				sinFireShow = 1;
			}

		break;
		case 3: //��ų 

			if(cSkill.OpenFlag == SIN_CLOSE)cSkill.OpenFlag = SIN_OPEN;
			else cSkill.OpenFlag = SIN_CLOSE;
			cInterFace.CheckAllBox(SIN_SKILL); //â�� �ϳ��� ���� 
			if(cSkill.OpenFlag){
				if(!sinFireShow){
					StartMenuFlame(0,350);
					sinPlaySound( 0 );
				}
				else
					sinPlaySound(SIN_SOUND_SHOW_INTER);

				sinFireShow = 1;
			}
			
		break;

		case 4:
			ShowParty();

		break;

		case 5:
			ShowQuest();
			
		break;

		case 6:
			if(!ExitButtonClick)ExitButtonClick =1;
			else ExitButtonClick = 0;
			cInterFace.CheckAllBox(SIN_SYSTEM); //â�� �ϳ��� ���� 

		break;
		}

	}

	InterButtonDown = 0; //��ư���϶��� �ʱ�ȭ 


}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/	
void cINTERFACE::RButtonDown(int x , int y)
{

	if ( GetFieldMapCursor()==TRUE && cInterFace.sInterFlags.MapOnFlag ) {
		cHelpPet.PetMessage("*Mini_Map",2);

	}


	if(MouseButtonPosi){ //��ư�����ǿ��� ���콺 ��ư�� Ŭ�������� 
		switch(MouseButtonPosi){ //PetMessage
			case 1:
				cHelpPet.PetMessage("*CarStatus",2);
			break;
			case 2:
				cHelpPet.PetMessage("*InvenTory",2);
			break;
			case 3:
				cHelpPet.PetMessage("*Skill",2);
			break;
			case 4:
				cHelpPet.PetMessage("*Party",2);
			break;
			case 5:
				cHelpPet.PetMessage("*Quest",2);
			break;
			case 6:
				cHelpPet.PetMessage("*System",2);
			break;
		}

	}

	switch(sinRunCameraMapPosi){
		case 1:
			cHelpPet.PetMessage("*RunAndWalk",2);
		break;
		case 2:
			if(sInterFlags.CameraAutoFlag == 0){ //ī�޶� ���� 
				cHelpPet.PetMessage("*Camera_Hand",2);
			}
			if(sInterFlags.CameraAutoFlag == 2){ //ī�޶� ���� 
				cHelpPet.PetMessage("*Camera_Fix",2);
			}
			if(sInterFlags.CameraAutoFlag ==1 ){//ī�޶� �ڵ� 
				cHelpPet.PetMessage("*Camera_Auto",2);
			}

		break;
		case 3:
			//cHelpPet.PetMessage("*Mini_Map",2);
		break;
	}



	//Pet �޼���
	//PetMessage
	if (365< pCursorPos.x  && 435 > pCursorPos.x  && 
			589 < pCursorPos.y  && 596 > pCursorPos.y  ){
				cHelpPet.PetMessage("*DayandNight",2);

	}

	//Pet �޼���
	if(pCursorPos.x > 315 && pCursorPos.x < 337 && pCursorPos.y > 500 && pCursorPos.y < 594){//������ 
		cHelpPet.PetMessage("*Bar_Life",2);
	}
	if(pCursorPos.x > 463 && pCursorPos.x < 483 && pCursorPos.y > 498 && pCursorPos.y < 595){//���
		cHelpPet.PetMessage("*Bar_Mana",2);
	}
	if(pCursorPos.x > 300 && pCursorPos.x < 313 && pCursorPos.y > 513 && pCursorPos.y < 595){//�ٷ�
		cHelpPet.PetMessage("*Bar_Stamina",2);
	}	
	if(pCursorPos.x > 483 && pCursorPos.x < 483+10 && pCursorPos.y > 513 && pCursorPos.y < 595){//����ġ
		cHelpPet.PetMessage("*Bar_Exp",2);  
	}	
	if(pCursorPos.x > 500 && pCursorPos.x < 570 && pCursorPos.y > 540 && pCursorPos.y < 594){//����ġ 
		cHelpPet.PetMessage("*Exp_Num",2);
	}



	if(cInterFace.ChatFlag){	         
		if(SubChatHeight){ 
			if (0< pCursorPos.x  && 300 > pCursorPos.x  && 
					294 < pCursorPos.y  && 294+116 > pCursorPos.y  ){
						if(!ChatKindIndexNum){
							cHelpPet.PetMessage("*ChatBox",2);
						}

			}
		}
		else{
			if (0< pCursorPos.x  && 305 > pCursorPos.x  && 
					600-192 < pCursorPos.y  && 600-192+210 > pCursorPos.y  ){
						if(!ChatKindIndexNum){
							cHelpPet.PetMessage("*ChatBox",2);
						}

			}
		}
	}

	SkillMouseButtonPosi[1] = 1;
}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cINTERFACE::RButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	
char InputSecretCode[256];
char sinBuff[256];
int  ResetQuset3Flag = 0;

void cINTERFACE::KeyDown()
{
	/////////////��ȣ�� ���� �ִ´� 
	if(hFocusWnd){  
		//cHelpPet.PetMessage("*ChatBox",3); �ϴ� ä���� ����
		if(smConfig.DebugMode){
			GetWindowText( hFocusWnd , InputSecretCode , 90 );
			if ( InputSecretCode[0] ) {
				if ( InputSecretCode[0]=='/'){
						memset(sinBuff,0,sizeof(sinBuff));
						wsprintf( sinBuff , "%s" , InputSecretCode );
				}
			}
		}
	}

	//PK�� ����ġ ��� �Ѵ� 
	if( VRKeyBuff[VK_CONTROL]  ){
		CheckExpPercentControl();

	}
	else
		sinPKWarningExpFlag = 0;


//	char *p=0;
//	char sinCheatLevelBuff[256];
//	char sinLevelTempBuff[256];
//	int sinCheatLevel = 0;

	int t = 0;	

	//ä�� â ���� (�����ϴϱ� �׳� ���д� )
	if(sinGetKeyClick(VK_RETURN)){ //����Ű�� �Է��ϸ�     
		if( VRKeyBuff[VK_CONTROL]  ){
			if(!ChatFlag)
				ChatFlag = 1;
			else ChatFlag = 0;
		
		}
//�ؿ� ��ɾ���� ���� Ȯ���� ��.
		if(!lstrcmpi(sinBuff ,"/����������������")||!lstrcmpi(sinBuff ,"/PassMature"))
		{
			sinBuff[0] = 0;
			sinSecretFlag = 1;
			TestKeyAniFlag = 1;
			cInvenTory.ReFormInvenItem();
			cInvenTory.ReFormPotionNum();
			ReformCharForm();//������ 

		}
		if(!lstrcmpi(sinBuff ,"/�����ٴ�!") || !lstrcmpi(sinBuff ,"/FuckingInven!"))
		{
			memset(cInvenTory.InvenItem,0,sizeof(sITEM)*100);
			memset(cInvenTory.InvenItemTemp,0,sizeof(sITEM)*100);
			cInvenTory.ReFormInvenItem();
			cInvenTory.ReFormPotionNum();
			sinChar->Money = 0;
			ReformCharForm();//������ 
			SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ�
			SendChatMessageToServer(" T.T Oh, No ~~~");//" T.T ���� �� ����  Ʋ�Ⱦ� ~~~");

		}
		

		char sinTestBuff5[256] ,sinTestBuff6[256];

		if(smConfig.DebugMode){ //������ �ø������� �ƽξ�~~
			if(sinBuff[0]){
				memset(sinTestBuff5,0,sizeof(sinTestBuff5));
				memset(sinTestBuff6,0,sizeof(sinTestBuff6));

				sscanf( sinBuff, "%s %s", sinTestBuff5, sinTestBuff6 );			
			
				if ( !lstrcmp( sinTestBuff5 , "/�ӱ�100����Ʈ" ) || !lstrcmpi(sinBuff ,"/MrIm's100"))
				{
					if(sinTestBuff6[0]){
						#ifdef	_WINMODE_DEBUG
						if(atoi(&sinTestBuff6[0]) == 1){
							if(chaQuest.sHaQuest100LV.CODE){
								memset(&chaQuest.sHaQuest100LV,0,sizeof(sQUEST_ELEMENTARY));
								cSkill.CancelContinueSkill(CHANGE_ELEMENTARY_QUEST);
							}
						}
						if(atoi(&sinTestBuff6[0]) == 2){
							if(haElementaryQuestLog)
								haElementaryQuestLog = (haElementaryQuestLog - QUESTBIT_FURYOFPHANTOM );
							chaQuest.sHaQuest100LV.State = 12;
						}
						SaveGameData();
						SendChatMessageToServer("MrIm: It's me");
						memset(sinTestBuff5,0,sizeof(sinTestBuff5));
						memset(sinTestBuff6,0,sizeof(sinTestBuff6));
						memset(sinBuff,0,sizeof(sinBuff));
						#endif
					}
				
				}

//�ؿ�
				if ( !lstrcmp( sinTestBuff5 , "/�ӱ��ʺ�����Ʈ" ) || !lstrcmpi(sinTestBuff5 ,"/MrIm'sbeginner"))//(sinBuff ,"/MrIm'sbeginner"))
				{
					if(sinTestBuff6[0])
					{
						#ifdef	_WINMODE_DEBUG
						for(int i=0;i<8;i++)   //���� �������� ����Ʈ�� �ʱ�ȭ
						{
							if(chaQuest.sHaQuestElementary[i].CODE)
							{
								memset(&chaQuest.sHaQuestElementary[i],0,sizeof(sQUEST_ELEMENTARY));
								cSkill.CancelContinueSkill(CHANGE_ELEMENTARY_QUEST);
							}
						}
						SaveGameData();
						SendChatMessageToServer("MrIm:Reset OK");
						memset(sinTestBuff5,0,sizeof(sinTestBuff5));
						memset(sinTestBuff6,0,sizeof(sinTestBuff6));
						memset(sinBuff,0,sizeof(sinBuff));
						#endif
					}
				
				}

				if ( lstrcmp( sinTestBuff5 , "/�̹��4������Ʈ" )==0 || lstrcmp( sinTestBuff5 , "/Lee4thRankUp" )==0)
				{
					if(sinTestBuff6[0]){
						#ifdef	_WINMODE_DEBUG
						ResetQuset3Flag = atoi(sinTestBuff6);
						//����Ʈ ����
						sinCompleteChangeJob4(ResetQuset3Flag);
						memset(sinTestBuff5,0,sizeof(sinTestBuff5));
						memset(sinTestBuff6,0,sizeof(sinTestBuff6));
						memset(sinBuff,0,sizeof(sinBuff));
						#endif
					}
				}

				if ( lstrcmp( sinTestBuff5 , "/�̹��3������Ʈ" )==0  || lstrcmp( sinTestBuff5 , "/Lee3thRankUp" )==0)
				{
					if(sinTestBuff6[0]){
						#ifdef	_WINMODE_DEBUG
						ResetQuset3Flag = atoi(sinTestBuff6);
						if(ResetQuset3Flag == 1){
							memset(&sinQuest_ChangeJob3,0,sizeof(sQUEST_CHANGEJOB3));
							SaveGameData();
							SendChatMessageToServer("MrLee:Retry change Job Quest~ ");
						}
						if(ResetQuset3Flag == 2){
							CheckChangeJob3QuestItem(0);
						}
						memset(sinTestBuff5,0,sizeof(sinTestBuff5));
						memset(sinTestBuff6,0,sizeof(sinTestBuff6));
						memset(sinBuff,0,sizeof(sinBuff));
						#endif
					}
				}
				if ( lstrcmp( sinTestBuff5 , "/�̹����������Ʈ" )==0 || lstrcmp( sinTestBuff5 , "/LeeResetQuest" )==0)
				{
					if(sinTestBuff6[0]){
						#ifdef	_WINMODE_DEBUG
						ResetQuset3Flag = atoi(sinTestBuff6);
						switch(ResetQuset3Flag){
							case 30:
								memset(&sinQuest_Level30,0,sizeof(sQUEST_CHANGEJOB3));
								SaveGameData();
								SendChatMessageToServer("MrLee: Clear 30th Quest ~");
							break;
							case 55:
								memset(&sinQuest_Level55,0,sizeof(sQUEST_CHANGEJOB3));
								memset(&sinQuest_Level55_2,0,sizeof(sQUEST_CHANGEJOB3));
								SaveGameData();
								SendChatMessageToServer("MrLee: Clear 55th Quest ~");
							break;
							case 70:
								memset(&sinQuest_Level70,0,sizeof(sQUEST_CHANGEJOB3));
								SaveGameData();
								SendChatMessageToServer("MrLee: Clear 70th Quest ~");

							break;
							case 80:
								memset(&sinQuest_Level80,0,sizeof(sQUEST_CHANGEJOB3));
								SaveGameData();
								SendChatMessageToServer("MrLee: Clear 80th Quest ~");

							break;
							case 85:
								memset(&sinQuest_Level85,0,sizeof(sQUEST_CHANGEJOB3));
								SaveGameData();
								SendChatMessageToServer("MrLee: Clear 85th Quest ~");

							break;
							case 90:
								memset(&sinQuest_Level90,0,sizeof(sQUEST_CHANGEJOB3));
								SaveGameData();
								SendChatMessageToServer("MrLee: Clear 90th Quest ~");

							break;

							case 802:
								memset(&sinQuest_Level80_2,0,sizeof(sQUEST_CHANGEJOB3));
								SaveGameData();
								SendChatMessageToServer("MrLee: Clear 80_2nd Quest ~");

							break;

							case 902:
								memset(&sinQuest_Level90_2,0,sizeof(sQUEST_CHANGEJOB3));
								SaveGameData();
								SendChatMessageToServer("MrLee: Clear 90_2nd Quest ~(re-connect) ");

							break;
						}
						if(ResetQuset3Flag == 1){
							memset(&sinQuest_ChangeJob3,0,sizeof(sQUEST_CHANGEJOB3));
							SaveGameData();
							SendChatMessageToServer("MrLee: Try again Quest~");
						}
						if(ResetQuset3Flag == 2){
							CheckChangeJob3QuestItem(0);
						}
						memset(sinTestBuff5,0,sizeof(sinTestBuff5));
						memset(sinTestBuff6,0,sizeof(sinTestBuff6));
						memset(sinBuff,0,sizeof(sinBuff));
						
						#endif

					}


				}

				//p = GetWord( sinLevelTempBuff , sinBuff);
				if ( lstrcmp( sinTestBuff5 , "/�̹����������" )==0  || lstrcmp( sinTestBuff5 , "/LeeWhereIs" )==0)
				{
					if(sinTestBuff6[0]){
						#ifdef	_WINMODE_DEBUG
						CheckCharForm(); //����   
						sinChar->Level = atoi(sinTestBuff6);
						//sinChar->Exp = (int)cCharStatus.sinGetNextExp();

						SetExp64( sinChar , ExpLevelTable[sinChar->Level-1] ); //����ġ ����
						CodeXorCharInfo_Exp(); //Xor ����

						ReformCharForm(); //������ 
						SendChatMessageToServer("MrLee: OK! finished Level UP /(-_-)/ ");
						memset(sinTestBuff5,0,sizeof(sinTestBuff5));
						memset(sinTestBuff6,0,sizeof(sinTestBuff6));
						memset(sinBuff,0,sizeof(sinBuff));
						
						#endif

					}
				}
				if ( lstrcmp( sinTestBuff5 , "/�̹��������佺ų" )==0  ||lstrcmp( sinTestBuff5 , "/LeeHelpMeSkill" )==0) {
					#ifdef	_WINMODE_DEBUG
					cSkill.InitSkillPoint(1);
					SendChatMessageToServer("MrLee: Skill reset/(-_-)/ ");
					memset(sinTestBuff5,0,sizeof(sinTestBuff5));
					memset(sinTestBuff6,0,sizeof(sinTestBuff6));
					memset(sinBuff,0,sizeof(sinBuff));
					#endif
				}
				if ( lstrcmp( sinTestBuff5 , "/�̹��������佺��" )==0  ||lstrcmp( sinTestBuff5 , "/LeeHelpMeStat" )==0) {
					#ifdef	_WINMODE_DEBUG
					sinChar->wVersion[1] = 0;
					cCharStatus.InitCharStatus();				
					cInvenTory.SetItemToChar();				
					if ( sinChar->Life[1]<sinGetLife() ) {
						sinSetLife( sinChar->Life[1] );
						ReformCharForm();//������ 
						ResetEnergyGraph(4);
					}
					SendChatMessageToServer("MrLee: Stat Reset/(-_-)/ ");
					memset(sinTestBuff5,0,sizeof(sinTestBuff5));
					memset(sinTestBuff6,0,sizeof(sinTestBuff6));
					memset(sinBuff,0,sizeof(sinBuff));
					#endif
				}
				if ( lstrcmp( sinTestBuff5 , "/�̹������������" )==0 ) {
					#ifdef	_WINMODE_DEBUG
					if(MouseItem.Flag){
						MouseItem.sItemInfo.SpecialItemFlag[0] = CHECK_GIVE_ITEM;
					}
					SendChatMessageToServer("�̹��: �߾���(-_-)/ ");
					memset(sinTestBuff5,0,sizeof(sinTestBuff5));
					memset(sinTestBuff6,0,sizeof(sinTestBuff6));
					memset(sinBuff,0,sizeof(sinBuff));

					//sinChar->wVersion[1] = 0;
					//cCharStatus.InitCharStatus();				
					//cInvenTory.SetItemToChar();				

					/*
					if ( sinChar->Life[1]<sinGetLife() ) {
						sinSetLife( sinChar->Life[1] );
						ReformCharForm();//������ 
						ResetEnergyGraph(4);
					}
					SendChatMessageToServer("�̹��: �����ʱ�ȭ/(-_-)/ ");
					memset(sinTestBuff5,0,sizeof(sinTestBuff5));
					memset(sinTestBuff6,0,sizeof(sinTestBuff6));
					memset(sinBuff,0,sizeof(sinBuff));
					*/
					#endif
				}
				if ( lstrcmp( sinTestBuff5 , "/�̹������" )==0  ||lstrcmp( sinTestBuff5 , "/PassRankUp" )==0)
				{
					#ifdef	_WINMODE_DEBUG
					SendChatMessageToServer("MeLee: it's good /(-_-)/ ");
					sinChar->ChangeJob++; //������ �Ѵ� 
					if(sinChar->ChangeJob > 3)sinChar->ChangeJob = 3;
					cSkill.SearchUseSkill();
					memset(sinTestBuff5,0,sizeof(sinTestBuff5));
					memset(sinTestBuff6,0,sizeof(sinTestBuff6));
					memset(sinBuff,0,sizeof(sinBuff));
					#endif
				}

				if ( lstrcmp( sinTestBuff5 , "/�̹����������Ʈ" )==0  ||lstrcmp( sinTestBuff5 , "/LeeResetRankUp" )==0) { //1�� �۽�ũ�� 2�� ����̿� 
					if(sinTestBuff6[0]){
						#ifdef	_WINMODE_DEBUG
						ResetQuset3Flag = atoi(sinTestBuff6);
						if(ResetQuset3Flag == 1){
							memset(&sinQuest_ChangeJob,0,sizeof(sQUEST_CHANGEJOB));
							SaveGameData();
							SendChatMessageToServer("MrLee: Clear Job Quest of Temscron 2th");
						}
						if(ResetQuset3Flag == 2){
							
							memset(&sinQuest_ChangeJob2,0,sizeof(sQUEST_CHANGEJOB));
							SaveGameData();
							SendChatMessageToServer("MrLee: Clear Job Quest of Moryon 2th");
						}
						if(ResetQuset3Flag == 3){
							
							memset(&sinQuest_ChangeJob3,0,sizeof(sQUEST_CHANGEJOB3));
							SaveGameData();
							SendChatMessageToServer("MrLee: Clear Job Quest of 3th ");
						}
						if(ResetQuset3Flag == 4){
							if(sinChar->ChangeJob==3){
								sinChar->ChangeJob=2;
							}
							memset(&sinQuest_ChangeJob4,0,sizeof(sQUEST_CHANGEJOB3));
							SaveGameData();
							SendChatMessageToServer("MrLee:Reconnect again!! ");
						}

						memset(sinTestBuff5,0,sizeof(sinTestBuff5));
						memset(sinTestBuff6,0,sizeof(sinTestBuff6));
						memset(sinBuff,0,sizeof(sinBuff));
						#endif
					}

				}
	
				if ( lstrcmp( sinTestBuff5 , "/�̹������Ʈ�ʱ�ȭ" )==0 ) {  
					#ifdef	_WINMODE_DEBUG

					SendChatMessageToServer("�̹��: ������������Ʈ�ʱ�ȭ");
					ClearNowQuest();
					memset(sinTestBuff5,0,sizeof(sinTestBuff5));
					memset(sinTestBuff6,0,sizeof(sinTestBuff6));
					memset(sinBuff,0,sizeof(sinBuff));
					#endif
				}

				if ( lstrcmp( sinTestBuff5 , "/�̹������" )==0  || lstrcmp( sinTestBuff5 , "/LeeForce" )==0) {
					#ifdef	_WINMODE_DEBUG
					SendChatMessageToServer("MrLee: I like force /(-_-)/ ");
					for(t = 0; t < 10 ; t++){
						if(ContinueSkill[t].Flag){
							if(ContinueSkill[t].CODE ==SKILL_FORCE_ORB){
								ContinueSkill[t].CheckTime = ContinueSkill[t].UseTime*70; //continue ��ų �ʱ�ȭ
								cInvenTory.SetItemToChar();
								break;

							}
						}
					}
					memset(sinTestBuff5,0,sizeof(sinTestBuff5));
					memset(sinTestBuff6,0,sizeof(sinTestBuff6));
					memset(sinBuff,0,sizeof(sinBuff));
					#endif
				}
				if ( lstrcmp( sinTestBuff5 , "/�̹����Ű�ʱ�ȭ" )==0  ||lstrcmp( sinTestBuff5 , "/LeeShotKeyReset" )==0) {   
					#ifdef	_WINMODE_DEBUG
					cSkill.SkillShortKeyInit();
					SendChatMessageToServer("MtLee: ShortKey Skill reset ");
					memset(sinTestBuff5,0,sizeof(sinTestBuff5));
					memset(sinTestBuff6,0,sizeof(sinTestBuff6));
					memset(sinBuff,0,sizeof(sinBuff));
					#endif
				}
				if ( lstrcmp( sinTestBuff5 , "/��������" )==0 ) {  
					#ifdef	_WINMODE_DEBUG
					for(int i=0 ; i<4 ; i++)
					{
						chaPremiumitem.UpKeepItem[i].PosiX   = 0;
						chaPremiumitem.UpKeepItem[i].PosiY   = 0;
						chaPremiumitem.UpKeepItem[i].Visible = false;
						chaPremiumitem.UpKeepItem[i].TGAImageNumber = 0;
						chaPremiumitem.UpKeepItem[i].IconTime[0] = 0;   
						chaPremiumitem.UpKeepItem[i].IconTime[1] = 0;
						chaPremiumitem.UpKeepItem[i].Alpha       = 255;
						chaPremiumitem.UpKeepItem[i].Alphakind   = 0;
						chaPremiumitem.UpKeepItem[i].Per		 = 0;
						memset(chaPremiumitem.UpKeepItem[i].Id,NULL,sizeof(chaPremiumitem.UpKeepItem[i].Id));
					}
					SendChatMessageToServer("�̹��:�����ʱ�ȭ ");
					memset(sinTestBuff5,0,sizeof(sinTestBuff5));
					memset(sinTestBuff6,0,sizeof(sinTestBuff6));
					memset(sinBuff,0,sizeof(sinBuff));
					#endif
				}
			}
		}
	}
	if(!hFocusWnd) //��Ŀ���� ������ ä��â�� ���´� 
		sinChatEnter = 0;
	else sinChatEnter = 1;
	if(ChatFlag)sinChatEnter = 1;
	////////ä�ð��� �� ..

}
/*----------------------------------------------------------------------------*
*		������ ���� ���׹̳��� üũ�ؼ� �����ش� 
*-----------------------------------------------------------------------------*/	
int DefaultLength[4] = {94,94,76,86};
float sinOldNowState[4] = {0,0,0,0};  
float sinOldMaxState[4] = {0,0,0,0};

//����� , ��� , �ٷ� �� �ʱ�ȭ�Ѵ� 
void cINTERFACE::InitState()
{

	float sinTemp;
	float sinMaxState[4] = {(float)sinChar->Life[1],(float)sinChar->Mana[1] ,(float)sinChar->Stamina[1]};  
	float sinNowState[4] = {(float)sinGetLife(),(float)sinGetMana() ,(float)sinGetStamina()};  

	for(int i=0 ;i < 3; i++){
		if(sinMaxState[i])
			sinTemp = (sinNowState[i]/sinMaxState[i])*100; 
		else
			sinTemp = 0;
		sinBmpLength[i] = (int)((DefaultLength[i]*(sinTemp/100)));
		sinOldNowState[i] = sinNowState[i];

	}

	if ( sinChar->Level<79 ) {	
		sinMaxState[3] = (float)(ExpLevelTable[sinChar->Level] - ExpLevelTable[sinChar->Level-1]);
		sinNowState[3] = (float)((GetXorExp64( sinChar))- ExpLevelTable[sinChar->Level-1]);
	}
	else {
		sinMaxState[3] = (float)((ExpLevelTable[sinChar->Level] - ExpLevelTable[sinChar->Level-1])/1000000);
		sinNowState[3] = (float)(((GetXorExp64( sinChar))- ExpLevelTable[sinChar->Level-1])/1000000);
	}

	InitStateFlag = 1;

}

//����� , ��� , �ٷ� �� ��ȭ�� ���� Bmp�� ũ�⸦ ��ȭ ��Ų��
void cINTERFACE::CheckingState()
{
	
	float	sinTemp;
	//������ ���� ���׹̳� ����ġ 
	float sinMaxState[4] = {(float)(sinChar->Life[1]),(float)sinChar->Mana[1] ,(float)sinChar->Stamina[1]};// ,(float)sinChar->Next_Exp};  
	float sinNowState[4] = {(float)sinGetLife(),(float)sinGetMana() ,(float)sinGetStamina()};// ,(float)sinChar->Exp};  
	
	//sinMaxState[3] = (float)(ExpLevelTable[sinChar->Level] - ExpLevelTable[sinChar->Level-1]);
	//sinNowState[3] = (float)((sinChar->Exp^CodeXorExp)- ExpLevelTable[sinChar->Level-1]);

	if ( sinChar->Level<79 ) {	
		sinMaxState[3] = (float)(ExpLevelTable[sinChar->Level] - ExpLevelTable[sinChar->Level-1]);
		sinNowState[3] = (float)((GetXorExp64( sinChar))- ExpLevelTable[sinChar->Level-1]);
	}
	else {
		sinMaxState[3] = (float)((ExpLevelTable[sinChar->Level] - ExpLevelTable[sinChar->Level-1])/1000000);
		sinNowState[3] = (float)(((GetXorExp64( sinChar))- ExpLevelTable[sinChar->Level-1])/1000000);
	}

	for(int i = 0 ; i < 4 ; i++){
		if(sinOldNowState[i] != sinNowState[i] || sinOldNowState[i] != sinMaxState[i]){ //�÷��� ��ġ 
			if(sinOldNowState[i] != sinNowState[i]){ //��ġ��ȭ 
				sinTemp = ((sinNowState[i])/sinMaxState[i])*100; 
				sinBmpLength[i] = (int)((DefaultLength[i]*(sinTemp/100)));
				if(sinBmpLength[i] >DefaultLength[i])sinBmpLength[i] = DefaultLength[i]; //�̹��� ũ�⺸��
				sinOldNowState[i] = sinNowState[i];

			}
	
		}
	}
	
}



//����� ��� �ٷ� ����ġ ��ġ�� ǥ���Ѵ� 
void cINTERFACE::ShowParaState()
{

	InfoMessageBoxPos.x = 0; //��ǥ �ʱ�ȭ  
	InfoMessageBoxPos.y = 0;

	szInfoMsgBuff5[0] = 0;
	lstrcpy(szInfoMsgBuff5,""); //���ڰ� ������ ���� �ߴ� 

	//��Ʈ�� �����ٰ��ؼ� �׳� �ڸ��ٲ� (��ġ�� ����)
	if(pCursorPos.x > 315 && pCursorPos.x < 337 && pCursorPos.y > 500 && pCursorPos.y < 594){//������ 
		if(AddVirtualLife[1]){
			AddVirtualLife[0] = (int)((float)sinGetLife() * ((float)sinVirtualLifePercent / 100.f));
		}
			cHelpPet.PetMessage("*Bar_Life",0);
			wsprintf(szInfoMsgBuff5,sinStateName[0],(int)sinGetLife()+AddVirtualLife[0],(int)sinChar->Life[1]+AddVirtualLife[1]);
			InfoMessageBoxPos.x = 343;   
			InfoMessageBoxPos.y = 500;

	}
	/*
	if(pCursorPos.x > 463 && pCursorPos.x < 483 && pCursorPos.y > 498 && pCursorPos.y < 595){//���
		wsprintf(szInfoMsgBuff5,sinStateName[1],(int)sinGetMana(),(int)sinChar->Mana[1]);
		cHelpPet.PetMessage("*Bar_Mana",0);
		InfoMessageBoxPos.x = 490;
		InfoMessageBoxPos.y = 498;

	}
	if(pCursorPos.x > 300 && pCursorPos.x < 313 && pCursorPos.y > 513 && pCursorPos.y < 595){//�ٷ�
		cHelpPet.PetMessage("*Bar_Stamina",0);
		wsprintf(szInfoMsgBuff5,sinStateName[2],(int)sinGetStamina(),(int)sinChar->Stamina[1]);
		InfoMessageBoxPos.x = 320;
		InfoMessageBoxPos.y = 513;

	}
	*/
	
	if(pCursorPos.x > 463 && pCursorPos.x < 483 && pCursorPos.y > 498 && pCursorPos.y < 595){//���
		// ����� - �ν��� ������(���)
		if(AddVirtualMana[1]){
			AddVirtualMana[0] = (int)((float)sinGetMana() * ((float)sinVirtualManaPercent / 100.f));
		}
		wsprintf(szInfoMsgBuff5,sinStateName[1],(int)sinGetMana(),(int)sinChar->Mana[1]);
		cHelpPet.PetMessage("*Bar_Mana",0);
		InfoMessageBoxPos.x = 490;
		InfoMessageBoxPos.y = 498;

	}
	if(pCursorPos.x > 300 && pCursorPos.x < 313 && pCursorPos.y > 513 && pCursorPos.y < 595){//�ٷ�
		// ����� - �ν��� ������(ü��)
		if(AddVirtualStamina[1]){
			AddVirtualStamina[0] = (int)((float)sinGetStamina() * ((float)sinVirtualStaminaPercent / 100.f));
		}
		cHelpPet.PetMessage("*Bar_Stamina",0);
		wsprintf(szInfoMsgBuff5,sinStateName[2],(int)sinGetStamina(),(int)sinChar->Stamina[1]);
		InfoMessageBoxPos.x = 320;
		InfoMessageBoxPos.y = 513;

	}	

	if(pCursorPos.x > 483 && pCursorPos.x < 483+10 && pCursorPos.y > 513 && pCursorPos.y < 595){//����ġ
		cHelpPet.PetMessage("*Bar_Exp",0);

	}	
	if(pCursorPos.x > 500 && pCursorPos.x < 570 && pCursorPos.y > 540 && pCursorPos.y < 594){//����ġ 
		cHelpPet.PetMessage("*Exp_Num",0);
	}

	//int ExpPercent  = 0;
	//int ExpPercentSpot = 0;
	//int ExpSize = 0;
	//int ExpNowSize  = 0;


	/*
	if(pCursorPos.x > 570 && pCursorPos.x < 645 && pCursorPos.y > 586 && pCursorPos.y < 594){//����ġ 
		if(sinChar->Level > 79 ){ //sinStateName[4] �� Mega ǥ��   
			//wsprintf(szInfoMsgBuff5,sinStateName[4],cCharStatus.sinGetNowExp(),cCharStatus.sinGetNextExp());
			ExpSize = (int)(GetNextExp( sinChar->Level )/1000000) - (int)(GetNextExp( sinChar->Level-1 )/1000000);  
			ExpNowSize = cCharStatus.sinGetNowExp()-(int)(GetNextExp( sinChar->Level-1 )/1000000);
			ExpPercent = (int)(10000.0f/(float)ExpSize * (float)ExpNowSize); 
			ExpPercentSpot = (int)(ExpPercent%100);
			ExpPercent = ExpPercent/100; 
			if(ExpPercentSpot < 10 ){
				wsprintf(szInfoMsgBuff5,ExpPercent3,ExpPercent,ExpPercentSpot,"%  ");
			}
			else{
				wsprintf(szInfoMsgBuff5,ExpPercent2,ExpPercent,ExpPercentSpot,"%  ");
			}


		}
		else{ //sinStateName[3] �� �Ϲ� ����ġ ǥ�� 
			//Percent�� ǥ�� 
			ExpSize = (int)GetNextExp( sinChar->Level ) - (int)GetNextExp( sinChar->Level-1 );  
			ExpNowSize = cCharStatus.sinGetNowExp()-(int)GetNextExp( sinChar->Level-1 );
			ExpPercent = (int)(10000.0f/(float)ExpSize * (float)ExpNowSize); 
			ExpPercentSpot = (int)(ExpPercent%100);
			ExpPercent = ExpPercent/100;
			if(ExpPercentSpot < 10 ){
				wsprintf(szInfoMsgBuff5,ExpPercent3,ExpPercent,ExpPercentSpot,"%  ");
			}
			else{
				wsprintf(szInfoMsgBuff5,ExpPercent2,ExpPercent,ExpPercentSpot,"%  ");
			}

			//wsprintf(szInfoMsgBuff5,sinStateName[3],cCharStatus.sinGetNowExp(),cCharStatus.sinGetNextExp());
		}
		InfoMessageBoxPos.x = 488;    
		InfoMessageBoxPos.y = 537;

	}	
	*/

}

//�ؽ�Ʈ�� ǥ�����ش� 
void cINTERFACE::DrawInterText()
{
	HDC	hdc;

	renderDevice.lpDDSBack->GetDC(&hdc);
	SelectObject( hdc, sinFont);
    SetBkMode( hdc, TRANSPARENT );
	SetTextColor( hdc, RGB(255,255,255) );

	if(InfoMessageBoxPos.x != 0 || InfoMessageBoxPos.y != 0) //ǥ����ġ�� �������� ǥ���Ѵ�   
		dsTextLineOut(hdc,InfoMessageBoxPos.x,InfoMessageBoxPos.y , szInfoMsgBuff5,lstrlen(szInfoMsgBuff5));
	if(ShowExpPercentFlag){  
		SetTextColor( hdc, RGB(202,254,251) );
		dsDrawOffsetArray = dsARRAY_BOTTOM;
		dsTextLineOut(hdc,511,552 , szInfoMsgBuff6,lstrlen(szInfoMsgBuff6));
		dsDrawOffsetArray = dsARRAY_TOP;

	}

	/*
	SetTextColor( hdc, RGB(255,255,255) );
	if(ReStartFlag){
		if(ReStartIndex== 1 && RestartCheckExp ) //����ġ�� �ȵǼ� �ʵ忡�� �����Ҽ����� ��쿡�� ���� ����������
			SetTextColor( hdc, RGB(255,0,0) );
		else
			SetTextColor( hdc, RGB(255,255,255) );
		
		if(szReStartMsg[0]){
			dsTextLineOut(hdc,ReStartTextXY.x,ReStartTextXY.y+2 , szReStartMsg,lstrlen(szReStartMsg));
		}
		if(szReStartMsg2[0]){
			dsTextLineOut(hdc,ReStartTextXY.x,ReStartTextXY.y+18 , szReStartMsg2,lstrlen(szReStartMsg2));
		}

	}
	*/

	SetTextColor( hdc, RGB(255,255,255) );
	if(SkillNpcFlag){
		if(szSkillMasterMsg[0]){
			dsTextLineOut(hdc,SkillMasterTextXY.x,SkillMasterTextXY.y+2 , szSkillMasterMsg,lstrlen(szSkillMasterMsg));
		}
		if(szReStartMsg2[0]){
			dsTextLineOut(hdc,SkillMasterTextXY.x,SkillMasterTextXY.y+18 , szSkillMasterMsg2,lstrlen(szSkillMasterMsg2));
		}

	}

	renderDevice.lpDDSBack->ReleaseDC(hdc);

}

//�ڽ��� �ϳ��� �����ش�  
void cINTERFACE::CheckAllBox(int ButtonFlag)
{

	int i = 0;
	for(i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){        
		if(sSinHelp[i].KindFlag){
			if(( ButtonFlag ==SIN_CRAFTITEM || ButtonFlag ==SIN_AGING || ButtonFlag == SIN_SMELTING || ButtonFlag == SIN_MANUFACTURE ))return;
		}
		//***** �ڷ���Ʈ ����â ���������� �ŷ��� ������ ��ҽ�Ų��  �ڷ���Ʈ ���׼���( ���� 060621 )
		if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL ){
			if ( cTrade.OpenFlag ) {
					//�ŷ����̸� �ŷ� ����
					SendRequestTrade( cTrade.TradeCharCode , 3 );	//�ŷ� ��� �䱸
					cTrade.CancelTradeItem();						//�ŷ�â �ݱ�
			}
			if(( ButtonFlag == SIN_MYSHOP || ButtonFlag == SIN_TRADE ))return;		
		}
	}
	
	//â�� �������� �ٸ� â�� �������� 
	if(cWareHouse.OpenFlag){  
		if(ButtonFlag == SIN_TRADE ||ButtonFlag == SIN_SHOP || 
			ButtonFlag == SIN_CRAFTITEM || ButtonFlag == SIN_AGING ||ButtonFlag == SIN_MYSHOP
			|| ButtonFlag == SIN_CHARSHOP || ButtonFlag == SIN_SMELTING || ButtonFlag == SIN_MANUFACTURE )return;
		
	}
	//���λ����� ���������� �ٸ�â�� ��������
	if(MyShopSendButton || cMyShop.OpenFlag || cCharShop.OpenFlag){
		if(ButtonFlag == SIN_TRADE ||ButtonFlag == SIN_SHOP || ButtonFlag == SIN_WAREHOUSE ||
			ButtonFlag == SIN_CRAFTITEM || ButtonFlag == SIN_AGING  ||
			ButtonFlag == SIN_CHARSHOP || ButtonFlag == SIN_SMELTING || ButtonFlag == SIN_MANUFACTURE )return;

	}
	//�ٸ� â�� ���������� ���λ����� ĳ���ͻ����� ������ ����
	if(cShop.OpenFlag || cCraftItem.OpenFlag || cAging.OpenFlag || cTrade.OpenFlag ||
		cCharShop.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag ){ // ������ - �ͽ��� ���� â �߰�
			if(ButtonFlag == SIN_MYSHOP || ButtonFlag == SIN_CHARSHOP)return;

		}

	if(cShop.OpenFlag || cCraftItem.OpenFlag || cAging.OpenFlag || cTrade.OpenFlag ||
		cMyShop.OpenFlag || cCharShop.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag ){
			if(ButtonFlag == SIN_CHARSHOP)return;

		}
	
	switch(ButtonFlag){
	case SIN_CHARSTATUS: //ĳ���� �������ͽ� 	
		if(cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_CLOSE;
		if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
		ExitButtonClick = 0; //�ý��� â �ݱ� 
		
	break;
	case SIN_SKILL: //��ųâ 
		if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
		if(cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_CLOSE;
		ExitButtonClick = 0; //�ý��� â �ݱ� 
		
		break;
	case SIN_INVENTORY: //�κ��丮 
		if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
		if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
		ExitButtonClick = 0; //�ý��� â �ݱ� 

		break;

	case SIN_TRADE: //Ʈ���̵� 
		if(cMixtureReset.OpenFlag)	cMixtureReset.OpenFlag = SIN_CLOSE;	// ������ - �ͽ��� ���� â �߰�
		if(cAging.OpenFlag)cAging.OpenFlag = SIN_CLOSE;
		if(cCraftItem.OpenFlag)cCraftItem.OpenFlag = SIN_CLOSE;
		if(cShop.OpenFlag)cShop.OpenFlag = SIN_CLOSE; //������ �ݾ��ش� 
		if(cWareHouse.OpenFlag)cWareHouse.OpenFlag = SIN_CLOSE; //������ �ݾ��ش� 
		if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
		if(!cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_OPEN; //�κ��丮�� ���������� �����ش� 		 
		if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
		// pluto ����
		if( SmeltingItem.OpenFlag )
		{
			SmeltingItem.OpenFlag = SIN_CLOSE;
		}
		// pluto ����
		if( ManufactureItem.m_OpenFlag )
		{
			ManufactureItem.m_OpenFlag = SIN_CLOSE;
		}
		cTrade.ReFormTradeItem();	//Ʈ���̵� ������ üũ�� �ʱ�ȭ 
		cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�

	break;

	case SIN_SHOP: //���� 
		if(cMixtureReset.OpenFlag)	cMixtureReset.OpenFlag = SIN_CLOSE;	// ������ - �ͽ��� ���� â �߰�
		if(cAging.OpenFlag)cAging.OpenFlag = SIN_CLOSE;
		if(cCraftItem.OpenFlag)cCraftItem.OpenFlag = SIN_CLOSE;
		if(cTrade.OpenFlag)cTrade.OpenFlag = SIN_CLOSE; 
		if(cWareHouse.OpenFlag)cWareHouse.OpenFlag = SIN_CLOSE; 
		if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
		if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
		// pluto ����
		if( SmeltingItem.OpenFlag )
		{
			SmeltingItem.OpenFlag = SIN_CLOSE;
		}
		// pluto ����
		if( ManufactureItem.m_OpenFlag )
		{
			ManufactureItem.m_OpenFlag = SIN_CLOSE;
		}
		if(!cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_OPEN; //�κ��丮�� ���������� �����ش� 		 
		if(!cShop.OpenFlag) cShop.OpenFlag = SIN_OPEN; //������ ���������� �����ش� 
		cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
				
	break;

	case SIN_SYSTEM: //�ý��� 
		if(cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_CLOSE;
		if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
		if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;

	break;
	case SIN_ALL_CLOSE:
		if(cTrade.OpenFlag)break;
		if(cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_CLOSE;
		if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
		if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
		if(cShop.OpenFlag)cShop.OpenFlag = SIN_CLOSE;
		if(cTrade.OpenFlag)cTrade.OpenFlag = SIN_CLOSE;
		// �庰 - ����â, ����â ����
		// pluto ����
	//	if( SmeltingItem.OpenFlag )
	//	{
	//		SmeltingItem.OpenFlag = SIN_CLOSE;
	//	}
		// pluto ����
	//	if( ManufactureItem.m_OpenFlag )
	//	{
	//		ManufactureItem.m_OpenFlag = SIN_CLOSE;
	//	}
		ExitButtonClick = 0; 
	break;

	case SIN_WAREHOUSE: //â�� 
		//�̴ϸ��̶������ܿ쿡�� â�� �������ʴ´�
		if(g_IsDxProjectZoomIn)break;

		if(cMixtureReset.OpenFlag)	cMixtureReset.OpenFlag = SIN_CLOSE;	// ������ - �ͽ��� ���� â �߰�
		if(cTrade.OpenFlag)cTrade.OpenFlag = SIN_CLOSE; 
		if(cShop.OpenFlag)cShop.OpenFlag = SIN_CLOSE; 
		if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
		if(cAging.OpenFlag)cAging.OpenFlag = SIN_CLOSE;
		if(cCraftItem.OpenFlag)cCraftItem.OpenFlag = SIN_CLOSE;
		if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
		if(!cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_OPEN; //�κ��丮�� ���������� �����ش� 		 
		if(!cWareHouse.OpenFlag) cWareHouse.OpenFlag = SIN_OPEN; //������ ���������� �����ش� 		 
		// pluto ����
		if( SmeltingItem.OpenFlag )
		{
			SmeltingItem.OpenFlag = SIN_CLOSE;
		}
		// pluto ����
		if( ManufactureItem.m_OpenFlag )
		{
			ManufactureItem.m_OpenFlag = SIN_CLOSE;
		}
		cWareHouse.ReFormWareHouse();
		cWareHouse.CheckCopyItem(); //����� �������� �ִ����� üũ�Ѵ� 
		cWareHouse.DeleteCopyItem(); //â���� ����������� �˻��ؼ� �����ش� 
		cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
	

	break;

	case SIN_CRAFTITEM: //������ ���� 
		if(cMixtureReset.OpenFlag)	cMixtureReset.OpenFlag = SIN_CLOSE;	// ������ - �ͽ��� ���� â �߰�
		if(cTrade.OpenFlag)cTrade.OpenFlag = SIN_CLOSE; 
		if(cShop.OpenFlag)cShop.OpenFlag = SIN_CLOSE; 
		if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
		if(cAging.OpenFlag)cAging.OpenFlag = SIN_CLOSE;
		if(cWareHouse.OpenFlag) cWareHouse.OpenFlag = SIN_CLOSE;
		if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
		if(!cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_OPEN; //�κ��丮�� ���������� �����ش� 		 
		if(!cCraftItem.OpenFlag) cCraftItem.OpenFlag = SIN_OPEN; //������ ���������� �����ش� 		 
		// pluto ����
		if( SmeltingItem.OpenFlag )
		{
			SmeltingItem.OpenFlag = SIN_CLOSE;
		}
		// pluto ����
		if( ManufactureItem.m_OpenFlag )
		{
			ManufactureItem.m_OpenFlag = SIN_CLOSE;
		}
		cCraftItem.ReFormCraftItem(); //�ͽ��� �������� Ȯ�����ش� 
		cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�

		
	break;
	case SIN_AGING:	//����¡ 
		if(cMixtureReset.OpenFlag)	cMixtureReset.OpenFlag = SIN_CLOSE;	// ������ - �ͽ��� ���� â �߰�
		if(cTrade.OpenFlag)cTrade.OpenFlag = SIN_CLOSE; 
		if(cShop.OpenFlag)cShop.OpenFlag = SIN_CLOSE; 
		if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
		if(cCraftItem.OpenFlag)cCraftItem.OpenFlag = SIN_CLOSE;
		if(cWareHouse.OpenFlag) cWareHouse.OpenFlag = SIN_CLOSE;
		if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
		if(!cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_OPEN; //�κ��丮�� ���������� �����ش� 		 
		if(!cAging.OpenFlag)cAging.OpenFlag = SIN_OPEN;
		// pluto ����
		if( SmeltingItem.OpenFlag )
		{
			SmeltingItem.OpenFlag = SIN_CLOSE;
		}
		// pluto ����
		if( ManufactureItem.m_OpenFlag )
		{
			ManufactureItem.m_OpenFlag = SIN_CLOSE;
		}
		cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
//		cCraftItem.ReFormCraftItem(); //�ͽ��� �������� Ȯ�����ش� 

		break;
	case SIN_MIXTURE_RESET:	// ������ - �ͽ��� ���� â ���� ���� �ݴ´�.
		if(cTrade.OpenFlag)			cTrade.OpenFlag			= SIN_CLOSE;	// Ʈ���̵� â �ݱ�
		if(cShop.OpenFlag)			cShop.OpenFlag			= SIN_CLOSE;	// ���� â �ݱ�
		if(cSkill.OpenFlag)			cSkill.OpenFlag			= SIN_CLOSE;	// ��ų â �ݱ�
		if(cCraftItem.OpenFlag)		cCraftItem.OpenFlag		= SIN_CLOSE;	// ���� â �ݱ�
		if(cWareHouse.OpenFlag)		cWareHouse.OpenFlag		= SIN_CLOSE;	// â�� �ݱ�
		if(cCharStatus.OpenFlag)	cCharStatus.OpenFlag	= SIN_CLOSE;	// ĳ������ â �ݱ�
		if(cAging.OpenFlag)			cAging.OpenFlag			= SIN_CLOSE;	// ����¡ â �ݱ�
		if( SmeltingItem.OpenFlag )	SmeltingItem.OpenFlag	= SIN_CLOSE;	// ���� â �ݱ�
		if( ManufactureItem.m_OpenFlag )	ManufactureItem.m_OpenFlag = SIN_CLOSE;	// ���� â �ݱ�

		if(!cInvenTory.OpenFlag)	cInvenTory.OpenFlag		= SIN_OPEN;		// �κ��丮�� ���������� �����ش� 	
		if(!cMixtureReset.OpenFlag)	// �ͽ��� ���� â�� ���������� �����ش�.
		{
			cMixtureReset.OpenFlag = SIN_OPEN;
		}
		cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
		break;
	case SIN_SMELTING:		// pluto ����
		if(cMixtureReset.OpenFlag)	cMixtureReset.OpenFlag = SIN_CLOSE;	// ������ - �ͽ��� ���� â �߰�
		if( cTrade.OpenFlag )
		{
			cTrade.OpenFlag = SIN_CLOSE;
		}

		if( cShop.OpenFlag )
		{
			cShop.OpenFlag = SIN_CLOSE;
		}

		if( cSkill.OpenFlag )
		{
			cSkill.OpenFlag = SIN_CLOSE;
		}

		if( cCraftItem.OpenFlag )
		{
			cCraftItem.OpenFlag = SIN_CLOSE;
		}

		if( cWareHouse.OpenFlag )
		{
			cWareHouse.OpenFlag = SIN_CLOSE;
		}

		if( cCharStatus.OpenFlag )
		{
			cCharStatus.OpenFlag = SIN_CLOSE;
		}

		if( cAging.OpenFlag )
		{
			cAging.OpenFlag = SIN_CLOSE;
		}
		// pluto ����
		if( ManufactureItem.m_OpenFlag )
		{
			ManufactureItem.m_OpenFlag = SIN_CLOSE;
		}
		if( !cInvenTory.OpenFlag )
		{
			cInvenTory.OpenFlag = SIN_OPEN;
		}
		if( !SmeltingItem.OpenFlag )
		{
			SmeltingItem.OpenFlag = SIN_OPEN;
		}
		cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
	break;
	case SIN_MANUFACTURE:		// pluto ����
		if(cMixtureReset.OpenFlag)	cMixtureReset.OpenFlag = SIN_CLOSE;	// ������ - �ͽ��� ���� â �߰�
		if( cTrade.OpenFlag )
		{
			cTrade.OpenFlag = SIN_CLOSE;
		}

		if( cShop.OpenFlag )
		{
			cShop.OpenFlag = SIN_CLOSE;
		}

		if( cSkill.OpenFlag )
		{
			cSkill.OpenFlag = SIN_CLOSE;
		}

		if( cCraftItem.OpenFlag )
		{
			cCraftItem.OpenFlag = SIN_CLOSE;
		}

		if( cWareHouse.OpenFlag )
		{
			cWareHouse.OpenFlag = SIN_CLOSE;
		}

		if( cCharStatus.OpenFlag )
		{
			cCharStatus.OpenFlag = SIN_CLOSE;
		}

		if( cAging.OpenFlag )
		{
			cAging.OpenFlag = SIN_CLOSE;
		}
		if( SmeltingItem.OpenFlag )
		{
			SmeltingItem.OpenFlag = SIN_CLOSE;
		}
		if( !cInvenTory.OpenFlag )
		{
			cInvenTory.OpenFlag = SIN_OPEN;
		}
		if( !ManufactureItem.m_OpenFlag )
		{
			ManufactureItem.m_OpenFlag = SIN_OPEN;
			// pluto ���� ������ ����, ���� ���� ���� �÷���
			if( !g_Manufacture_ItemInfo[0].ItemCode ) // ó�� ����â ������ ������
			{
				smTRANS_COMMAND	TransCommand;
				TransCommand.code =	smTRANSCODE_MANUFACTURE_WEIGHT_PRICE;
				TransCommand.size = sizeof(smTRANS_COMMAND);
				TransCommand.WParam = 0;
				TransCommand.LParam = 0;
				TransCommand.SParam = 0;
				smWINSOCK* lpsmSock;
				lpsmSock = GetAreaServerSock();		//���� �������ϱ�
				if( lpsmSock )
				{
					lpsmSock->Send2( (char*)&TransCommand, TransCommand.size, TRUE );
				}
			}
		}
		cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
	break;
	case SIN_MYSHOP:
		if(lpCurPlayer->OnStageField>=0 && StageField[ lpCurPlayer->OnStageField ]->State == FIELD_STATE_VILLAGE ){
			if(cMixtureReset.OpenFlag)cMixtureReset.OpenFlag = SIN_CLOSE;	// ������ - �ͽ��� ���� â �߰�
			if(cTrade.OpenFlag)cTrade.OpenFlag = SIN_CLOSE; 
			if(cShop.OpenFlag)cShop.OpenFlag = SIN_CLOSE; 
			if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
			if(cAging.OpenFlag)cAging.OpenFlag = SIN_CLOSE;
			if(cCraftItem.OpenFlag)cCraftItem.OpenFlag = SIN_CLOSE;
			if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
			// pluto ����
			if( SmeltingItem.OpenFlag )
			{
				SmeltingItem.OpenFlag = SIN_CLOSE;
			}
			// pluto ����
			if( ManufactureItem.m_OpenFlag )
			{
				ManufactureItem.m_OpenFlag = SIN_CLOSE;
			}
			if(cCharShop.OpenFlag){
				cCharShop.OpenFlag = 0; //������ �ݴ´�
				memset(cCharShop.CharShopItem,0,sizeof(sITEM)*30); //�������� �����ش�
			}
			if(!cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_OPEN; //�κ��丮�� ���������� �����ش� 		 
			if(!cMyShop.OpenFlag) cMyShop.OpenFlag = SIN_OPEN; //������ ���������� �����ش� 
			cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
		}
		
	break;
	case SIN_CHARSHOP:
		if(lpCurPlayer->OnStageField>=0 && StageField[ lpCurPlayer->OnStageField ]->State == FIELD_STATE_VILLAGE){
			if(cMixtureReset.OpenFlag)cMixtureReset.OpenFlag = SIN_CLOSE;	// ������ - �ͽ��� ���� â �߰�
			if(cTrade.OpenFlag)cTrade.OpenFlag = SIN_CLOSE; 
			if(cShop.OpenFlag)cShop.OpenFlag = SIN_CLOSE; 
			if(cSkill.OpenFlag) cSkill.OpenFlag = SIN_CLOSE;
			if(cAging.OpenFlag)cAging.OpenFlag = SIN_CLOSE;
			if(cCraftItem.OpenFlag)cCraftItem.OpenFlag = SIN_CLOSE;
			if(cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
			// pluto ����
			if( SmeltingItem.OpenFlag )
			{
				SmeltingItem.OpenFlag = SIN_CLOSE;
			}
			// pluto ����
			if( ManufactureItem.m_OpenFlag )
			{
				ManufactureItem.m_OpenFlag = SIN_CLOSE;
			}
			if(cMyShop.OpenFlag){
				cMyShop.OpenFlag = SIN_CLOSE;
				cMyShop.OpenFlag = 0;
				memset(cMyShop.MyShopItem,0,sizeof(sITEM)*30); //�������� �����ش�
				memset(MyShopItemIndex,0,sizeof(int)*100);
				MyShopSendButton = 0; //��Ϲ�ư �ʱ�ȭ
			}
			if(!cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_OPEN; //�κ��丮�� ���������� �����ش� 		 
			if(!cCharShop.OpenFlag) cCharShop.OpenFlag = SIN_OPEN; //������ ���������� �����ش� 		 
			cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
		}
	break;

	}
	

	
}
//�ð� �ٴ��� �����δ� 
int cINTERFACE::DrawClockArrow( int x, int y, int Angle )
{

	/////////////�ð� �������� �����δ� 
	//sinBmpLength[4] = 

	float fx[4],fy[4];
	int sinY,cosY;
	int dx[4],dy[4];
	int cnt;

	int sinTimeAngle;
	float TempTime , TempTime2 , TempTime3;

	TempTime2 = (float)dwGameHour*60;
	TempTime3 = (float)(TempTime2+dwGameMin)/1440;
	
	TempTime = (float)TempTime3*100;
	
	//sinTimeAngle = (int)2190+(780*(TempTime/100));
	sinTimeAngle = (int)(2190+(780*(TempTime/100)));

	sinY = GetSin[ sinTimeAngle&ANGCLIP ];
	cosY = GetCos[ sinTimeAngle&ANGCLIP ];

	dx[0] = 8; dy[0] = 48;
	dx[1] = 8; dy[1] = 32;
	dx[2] =  -8; dy[2] = 48;
	dx[3] =  -8; dy[3] = 32;
/*
	dx[0] = -8; dy[0] = -8;
	dx[1] = -8; dy[1] =  8;
	dx[2] =  8; dy[2] = -8;
	dx[3] =  8; dy[3] =  8;
*/
	for( cnt=0;cnt<4;cnt++) {
		fx[cnt] = (float) (( dy[cnt]*sinY + dx[cnt]*cosY )>>8);
		fy[cnt] = (float) (( dy[cnt]*cosY - dx[cnt]*sinY )>>8);
		fx[cnt] /= 256;
		fy[cnt] /= 256;
		fx[cnt] += (float)x;
		fy[cnt] += (float)y;
	}


	psDrawTexImage2( smMaterial[cInterFace.MatClock].smTexture[0] , fx , fy, 0,0,1,1,255,0 );
//	psDrawTexImage2( 0 , fx , fy, 0,0,1,1,255,0 );


	return TRUE;
}

//��ħ���� �׸��� 
int cINTERFACE::DrawCompass(int x , int y ,int Angle)
{

	float fx[4],fy[4];
	int sinY,cosY;
	int dx[4],dy[4];
	int cnt;

	sinY = GetSin[ Angle&ANGCLIP ];
	cosY = GetCos[ (Angle+ANGLE_180)&ANGCLIP ];

	dx[0] = 8; dy[0] = 13+16;
	dx[1] = 8; dy[1] = 13;
	dx[2] =  -8; dy[2] = 13+16;
	dx[3] =  -8; dy[3] = 13;

	for( cnt=0;cnt<4;cnt++) {
		fx[cnt] = (float) (( dy[cnt]*sinY + dx[cnt]*cosY )>>8);
		fy[cnt] = (float) (( dy[cnt]*cosY - dx[cnt]*sinY )>>8);
		fx[cnt] /= 256;
		fy[cnt] /= 256;
		fx[cnt] += (float)x;
		fy[cnt] += (float)y;
	}


	psDrawTexImage2( smMaterial[cInterFace.MatComPass].smTexture[0] , fx , fy, 0,0,1,1,255,0 );


	return TRUE;
}

/*
//�̹����� �ε��Ѵ� 
void cINTERFACE::LoadReStartImage()
{
	lpReStartMain		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ReStart\\ReStartMain.bmp"); //�ٽý��� ���� 
	lpReStartButton[0]	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ReStart\\ReStart_Field.bmp"); //�ٽý��� ��ư 
	lpReStartButton[1]	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ReStart\\ReStart_Town.bmp"); //�ٽý��� ��ư 
	lpReStartButton[2]	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Inter\\ReStart\\ReStart_Exit.bmp"); //�ٽý��� ��ư 
	
}


void cINTERFACE::ReleaseReStartImage()
{
	dwCheckTime = 0;			//�ð� �ʱ�ȭ 
	ReStartImageLoadFlag = 0;	//�ε� �÷� �ʱ�ȭ 
	if(lpReStartMain){
		lpReStartMain->Release();
		lpReStartMain = 0;
		
	}
	for(int i=0; i < 3 ; i++){	
		if(lpReStartButton[i]){
			lpReStartButton[i]->Release();
			lpReStartButton[i] = 0;
			
		}
	}
}
*/


//��Ʈ�� Ű�� �������� ����ġ�� 4%�̸��̸� ����ġ �ٰ� ���ڰŸ��� (����ġ ���̺� �������� ��� ���Ƶ�)
int cINTERFACE::CheckExpPercentControl()
{
	/*
	int ExpBefore , ExpNext; 
	float ExpPersent , Result;

	if(sinChar->Level > 1  ){
		ExpBefore = (int)GetNextExp(sinChar->Level - 1);
		//ExpBefore ^= CodeXorExp;
		ExpNext   = (int)GetNextExp(sinChar->Level);
		//ExpNext   ^= CodeXorExp;

		//ExpPersent = ((float)ExpBefore/(float)ExpNext)*100.0f;

		ExpPersent = (float)(ExpNext - ExpBefore);
		
		Result = ((float)((sinChar->Exp^CodeXorExp)-ExpBefore)/(float)ExpPersent)*100.0f;

		//Result = ((float)ExpPersent/(float)sinChar->Exp)*100.0f;

		if(Result < 4.0f)  
			sinPKWarningExpFlag = 1;

	}
	*/

	return TRUE;
}

/////////////�ð��� ���ؼ� ǥ���Ѵ� 
int cINTERFACE::GetWordTimeDisplay()
{
	float TempfHour = (float)dwGameHour;
	//float TempfHour = (float)TestTime4;
	if(TempfHour >= 23 || TempfHour < 4)
		DayOrNightFlag = 2;
	else 
		DayOrNightFlag = 1;

	if(DayOrNightFlag == 1 ){
		sinGageTime2 = (int)(50.0f*((TempfHour-4.0f)/19.0f));
	}
	else{
		sinGageTime2 = (int)(50.0f*((TempfHour+1)/5.0f));
	}
	if(sinGageTime2 >  50)sinGageTime2 = 50;
	
	return TRUE;
}

//����ġPercent�� �����ش� 
void cINTERFACE::ShowExpPercent()
{

	/*
	if(cInvenTory.OpenFlag || cCharStatus.OpenFlag ||cSkill.OpenFlag){
		ShowExpPercentFlag = 0;
		return;
	}
	else
		ShowExpPercentFlag = 1;
	*/
	ShowExpPercentFlag = 1; 

	int ExpPercent  = 0;
	int ExpPercentSpot = 0;
	int ExpSize = 0;
	int ExpNowSize  = 0;

	szInfoMsgBuff6[0] = 0;

	if(sinChar->Level > 79 ){ 
		ExpSize = (int)((GetNextExp( sinChar->Level ) - GetNextExp( sinChar->Level-1 ))/1000000);
		ExpNowSize = (int)(cCharStatus.sinGetNowExp()-(GetNextExp( sinChar->Level-1 )/1000000));
		ExpPercent = (int)(100000.0f/(float)ExpSize * (float)ExpNowSize); 
		ExpPercentSpot = (int)(ExpPercent%1000);
		ExpPercent = ExpPercent/1000; 

		if(ExpPercent >=100){ //�� ��ġ�� ���� -_-
			ExpPercent = 99;
			ExpPercentSpot =999;
		}

		if(ExpPercentSpot < 10 ){
			wsprintf(szInfoMsgBuff6,"%d.00%d%s",ExpPercent,ExpPercentSpot,"%  ");
		}
		else if ( ExpPercentSpot >= 10 &&  ExpPercentSpot < 100 ){
			wsprintf(szInfoMsgBuff6,"%d.0%d%s",ExpPercent,ExpPercentSpot,"%  ");
		}
		else
			wsprintf(szInfoMsgBuff6,"%d.%d%s",ExpPercent,ExpPercentSpot,"%  ");

	}
	else{ 
		ExpSize = (int)GetNextExp( sinChar->Level ) - (int)GetNextExp( sinChar->Level-1 );  
		ExpNowSize = cCharStatus.sinGetNowExp()-(int)GetNextExp( sinChar->Level-1 );
		ExpPercent = (int)(100000.0f/(float)ExpSize * (float)ExpNowSize); 
		ExpPercentSpot = (int)(ExpPercent%1000);
		ExpPercent = ExpPercent/1000;

		//���� ���� ����ġ�� ���� �����Ѵ�
		cHelpPet.PetExp = ExpPercent;

		if(ExpPercentSpot < 10 ){
			wsprintf(szInfoMsgBuff6,"%d.00%d%s",ExpPercent,ExpPercentSpot,"%  ");
		}
		else if ( ExpPercentSpot >= 10 &&  ExpPercentSpot < 100 ){
			wsprintf(szInfoMsgBuff6,"%d.0%d%s",ExpPercent,ExpPercentSpot,"%  ");
		}
		else
			wsprintf(szInfoMsgBuff6,"%d.%d%s",ExpPercent,ExpPercentSpot,"%  ");

	}

}


//��������Ʈ�� �����ش�============================================================================================.
int cINTERFACE::SetStringEffect(int Index)
{
	if(!HA_EffectIndex[Index]){
			HA_EffectIndex[Index]=CreateTextureMaterial(HaEffectFilePath[Index], 0, 0, 0,0, SMMAT_BLEND_ALPHA); 
			ReadTextures();	//�ؽ��� �ε�
	}
	for(int i=0;i<MAX_HAEFFECTNUM;i++){
		if(!HaEffect[i].Flag){
			HaEffect[i].Flag = 1;
			HaEffect[i].Time = 0;
			HaEffect[i].Mat  = HA_EffectIndex[Index];
			HaEffect[i].Posi.x = 354;
			HaEffect[i].Posi.y = 488;
			HaEffect[i].Size.x = 128;
			HaEffect[i].Size.y = 64;
			return true;
		}
	}
	return false;
	
}
//===================================================================================================================
