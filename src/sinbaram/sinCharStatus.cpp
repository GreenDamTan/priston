/*----------------------------------------------------------------------------*
*	���ϸ� :  sinCharStatus.cpp	
*	�ϴ��� :  ĳ���� �������ͽ��� ���� ���� �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"
#include "Language\\Language.h"
#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\cE_CViewClanInfo.h"

/*----------------------------------------------------------------------------*
*								�������� 			
*-----------------------------------------------------------------------------*/	
cCHARSTATUS cCharStatus;
sDISPLAYSTATE sDisplayState[10];
sDISPLAYSTATE sDisplayStateShow;


int OldLevel;					//������ �����Ѵ� 

int OldLevelTemp;				//������ �����Ѵ� �� �� �����ϴ� (��? ���۳ѵ��� ��ġ�����ϵ���)

int sinLevelPoint= 0;		    //���� ����Ʈ�� �����Ѵ� 

int sinPointButtonDown = 0;     

int ShowCharArrow=0;

int RegiInfoPosi = 0;

int ReHeight3 = 2;

sCHARRECTPOSI	sCharRectPosi[MAX_CHAR_RECT_POSI]={
	//�⺻����
	{"Class"    ,{ 75   , 442+ReHeight3  , 160 , 454  }}, //0
	{"Name"     ,{ 75   , 463+ReHeight3  , 160 , 479  }}, //1
	{"Level"    ,{ 75   , 503+ReHeight3  , 128 , 504  }}, //2
	{"Exp"      ,{ 75   , 543+ReHeight3  , 160 , 549  }}, //3
	{"Next_Exp" ,{ 75   , 563+ReHeight3  , 160 , 574  }}, //4
	{"Life"		,{ 391  , 429+ReHeight3  , 482 , 440  }}, //5
	{"Mana"     ,{ 391  , 454+ReHeight3  , 482 , 466  }}, //6
	{"Stamina"  ,{ 391  , 479+ReHeight3  , 482 , 490  }}, //7

	//�ɷ�ġ 
	{"Strength" ,{ 585 , 433+ReHeight3 , 632 , 444  }}, //8
	{"Spirit"   ,{ 585 , 454+ReHeight3 , 632 , 464  }}, //9  
	{"Talent"   ,{ 585 , 473+ReHeight3 , 632 , 485  }}, //10  
	{"Dexterity",{ 585 , 493+ReHeight3 , 632 , 504  }}, //11
	{"Health"   ,{ 585 , 513+ReHeight3 , 632 , 524  }}, //12	

	//���� 
	{"Bionic"   ,{ 217+3 , 449+8+ReHeight3 , 248  , 468 }}, //13
	{"Poison"   ,{ 286+3 , 448+8+ReHeight3 , 315  , 469 }}, //14
	{"Fire"     ,{ 217+3 , 486+8+ReHeight3 , 248  , 469 }}, //15
	{"Lighting" ,{ 286+3 , 486+8+ReHeight3 , 315  , 505 }}, //16
	{"Ice"      ,{ 217+3 , 523+8+ReHeight3 , 248  , 543 }}, //17

	//���� ��� ���� 
	{"Attack_Rating" ,{ 710 , 455+ReHeight3-1 , 769 , 464 }}, //18
	{"Attack_Damage" ,{ 710 , 474+ReHeight3-1 , 769 , 484 }}, //19
	{"Defence"       ,{ 710 , 495+ReHeight3-1 , 769 , 504 }}, //20
	{"Speed"         ,{ 710 , 515+ReHeight3-1 , 769 , 524 }}, //21
	{"Absorption"    ,{ 710 , 534+ReHeight3-1 , 769 , 545 }}, //22
	{"Point"         ,{ 585 , 533+ReHeight3-1 , 632 , 544 }}, //23

	
};

int RegiBox[5][4] = {
	{ 185 , 445 , 216  , 475 }, //���̿�
	{ 185 , 482 , 216  , 512 }, //��
	{ 185 , 519 , 216  , 550 }, //����	
	{ 251 , 445 , 281  , 475 }, //��
	{ 251 , 482 , 281  , 512 }, //����


};

int PointButton[6][4]={
	{565+1,431+1,583,449},
	{565+1,452+1,583,469},
	{565+1,472+1,583,488},
	{565+1,491+1,583,508},
	{565+1,511+1,583,529},
	{565+1,531+1,583,549},

};

int TempStatePoint[5] = {0,0,0,0,0}; //����� ������Ʈ�� ���� �ְ� �ϱ����ؼ� Temp�� �д� 

int PointButtonPosi =0;

DIRECTDRAWSURFACE	lpStatusMain;

////////////////// ä���̹��� 
int MatChatBotton_Glay = 0;
DIRECTDRAWSURFACE lpChatButton[5];
DIRECTDRAWSURFACE lpChatButton_Notice[3];
DIRECTDRAWSURFACE lpChatLine;
int NoticeChatIndex = 0;
int NoticeChatIndexNum = 0;
int ChatKindIndex = 0;
int ChatKindIndexNum = 0;

// ����ũ
int sinMatMike = 0;
/*----------------------------------------------------------------------------*
*							Ŭ���� �ʱ�, ���� 
*-----------------------------------------------------------------------------*/	
cCHARSTATUS::cCHARSTATUS()
{
	OpenFlag = 0;

}

cCHARSTATUS::~cCHARSTATUS()
{

}

/*----------------------------------------------------------------------------*
*							     �ʱ�ȭ 
*-----------------------------------------------------------------------------*/	
void cCHARSTATUS::Init()
{

	//MatStatus[0] =  CreateTextureMaterial( "Image\\SinImage\\Status\\Status-1.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatStatus[1] =  CreateTextureMaterial( "Image\\SinImage\\Status\\Status-2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatStatus[2] =  CreateTextureMaterial( "Image\\SinImage\\Status\\Status-3.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatStatus[3] =  CreateTextureMaterial( "Image\\SinImage\\Status\\Status-4.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	Load();

	//�ؽ�Ʈ ���� ������� �ʱ�ȭ�� �ش� 
	SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_SPEED] = SIN_TEXT_COLOR_WHITE;


	MatChatBotton_Glay = CreateTextureMaterial( "Image\\SinImage\\Inter\\Chat\\Chatting06.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	//����ũ
	sinMatMike      = CreateTextureMaterial("Image\\SinImage\\Inter\\mike.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA); //�ð� 

}
/*----------------------------------------------------------------------------*
*						    Bmp������ �ε��Ѵ� 
*-----------------------------------------------------------------------------*/	
void cCHARSTATUS::Load()
{
	lpRegiBox[0]    = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\BioInfo.bmp");
	lpRegiBox[1]    = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\FireInfo.bmp");
	lpRegiBox[2]    = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\IceInfo.bmp");
	lpRegiBox[3]    = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\PoisonInfo.bmp");
	lpRegiBox[4]    = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\LightInfo.bmp");
	

    lpChangeArrow[0]   = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\ChangeArrow.bmp");
    lpSelectArrow[0]   = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\SelectArrow.bmp");
	lpSelectArrow[1]   = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\SelectArrow2.bmp");
	lpStatusPoint	   = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\StatusPoint.bmp");

	lpStatusMain	   = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\Status.bmp");

	lpChatButton[0]  = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_01.bmp");
	lpChatButton[1]  = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_02.bmp");
	lpChatButton[2]  = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_03.bmp");
	lpChatButton[3]  = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_04.bmp");
	lpChatButton[4]  = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_05.bmp");

	lpChatButton_Notice[0]  = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_N11.bmp");
	lpChatButton_Notice[1]  = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_N12.bmp");
	lpChatButton_Notice[2]  = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_N22.bmp");

	lpChatLine = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\n_line.bmp");
}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cCHARSTATUS::Release()
{
	int i;
	for(i=0 ;i < 5 ; i++){
		if(lpRegiBox[i]){
			lpRegiBox[i]->Release();
			lpRegiBox[i] = 0;

		}

	}
	if(lpChangeArrow[0]){
		lpChangeArrow[0]->Release();
		lpChangeArrow[0] = 0;
	}
	if(lpSelectArrow[0]){
		lpSelectArrow[0]->Release();
		lpSelectArrow[0] = 0;
	}
	if(lpSelectArrow[1]){
		lpSelectArrow[1]->Release();
		lpSelectArrow[1] = 0;
	}
	if(lpStatusPoint){
		lpStatusPoint->Release();

	}

}
/*----------------------------------------------------------------------------*
*							     �׸���  
*-----------------------------------------------------------------------------*/	
extern BOOL bMic; //����ũ
void cCHARSTATUS::Draw()
{

	int TempLen = 0; 
	int TempCount2 = 0;
	//lstrcpy(cldata.name,"�޷�Ŭ��¥��");       
	
	/////////////////// ä�� (�̹��� ������ �ֻ����� ���� ������) 
	if(sinChatEnter)
	{       
		if(SubChatHeight)
		{                 
			dsDrawTexImage( cInterFace.MatChatDoc[0] , 4, 576-SubChatHeight, 242, 32 , 255 );    //ä��â����  
			dsDrawTexImage( cInterFace.MatChatDoc[1] , 246, 576-SubChatHeight, 164, 32 , 255 );    //ä��â����  
		}
		else
			dsDrawTexImage( cInterFace.MatChatDoc[0] , 4, 576-SubChatHeight, 296, 32 , 255 );    //ä��â����  

	}

	if(cInterFace.ChatFlag)
	{	          
		if(SubChatHeight)
		{ 
				dsDrawTexImage( cInterFace.MatChatSmall[0] , 0, 294, 300, 116 , 255 );     //����  ä��â  
				//dsDrawTexImage( cInterFace.MatChatSmall[1] , 256, 600-152-128-21, 128, 128 , 255 );   //����  ä��â 
		}
		else
		{
			dsDrawTexImage( cInterFace.MatChatDefault[0] , 0, 600-192, 305, 210 , 255 );   //�⺻ ä��â    
			
			//ä�� ��ư 
			dsDrawTexImage( MatChatBotton_Glay , 100, 564, 256, 16 , 255 );   //�⺻ ��â    
			DrawSprite(100+ChatKindIndex*40,564 , lpChatButton[ChatKindIndex],0,0,40,16);
			
			//���� 
			DrawSprite(4,564 , lpChatButton_Notice[NoticeChatIndex],0,0,45,16);

			if(NoticeChatIndex == 1){
				DrawSprite(10,460 , lpChatLine,0,0,287,3);
				//����ũ 
				if(bMic)
					dsDrawTexImage( sinMatMike , 283, 464, 32, 32 , 255 );   //�⺻ ��â    
			}
			else
			{
				//����ũ
				if(bMic)
					dsDrawTexImage( sinMatMike , 283, 409, 32, 32 , 255 );   //�⺻ ��â    
			}
			
		}
	}

	if(sinMoveKindInter[SIN_CHARSTATUS] != 0)
	{
		DrawSprite(0,600-sinMoveKindInter[SIN_CHARSTATUS]+56 , lpStatusMain,0,0,800,200);
		/*
		dsDrawTexImage( MatStatus[0] , 0, 600-sinMoveKindInter[SIN_CHARSTATUS], 256, 256 , 255 );       
		dsDrawTexImage( MatStatus[1] , 256, 600-sinMoveKindInter[SIN_CHARSTATUS], 256, 256 , 255 );   
		dsDrawTexImage( MatStatus[2] , 256+256, 600-sinMoveKindInter[SIN_CHARSTATUS], 256, 256 , 255 );       
		dsDrawTexImage( MatStatus[3] , 256+256+256, 600-sinMoveKindInter[SIN_CHARSTATUS], 32, 256 , 255 );   
		*/

		if(ShowCharArrow)
		{ 
			//if(pCursorPos.x > 251 && pCursorPos.x < 269 && pCursorPos.y > 565 && pCursorPos.y < 583) 
			DrawSprite(251,565+(256-sinMoveKindInter[SIN_CHARSTATUS]),cShop.lpExit,0,0,20,20);
			DrawSprite(251,565-27+(256-sinMoveKindInter[SIN_CHARSTATUS]),cInvenTory.lpExitInfo,0 ,0 , 47,27);
		}	
			
		if(sinChar->StatePoint)//�ɷ�ġ�� �ø���������   
		{
			DrawSprite(520,531+(256-sinMoveKindInter[SIN_CHARSTATUS]),lpStatusPoint,0,0,115,19); 
			for(int i=0 ; i< 5 ;i++) //���� ��ų�� ������ 5���� ��ųġ 
			{
						DrawSprite(PointButton[i][0],PointButton[i][1]+(256-sinMoveKindInter[SIN_CHARSTATUS]),lpChangeArrow[0],0,0,18,17);
			}
			if(sinPointButtonDown)
			{
				if(PointButtonPosi <= 5 &&  PointButtonPosi != 0)
					DrawSprite(PointButton[PointButtonPosi-1][0]+1,PointButton[PointButtonPosi-1][1]+(256-sinMoveKindInter[SIN_CHARSTATUS])+1,lpSelectArrow[0],0,0,18,17);

				if(PointButtonPosi == 6)
					DrawSprite(PointButton[PointButtonPosi-1][0]+1,PointButton[PointButtonPosi-1][1]+(256-sinMoveKindInter[SIN_CHARSTATUS])+1,lpSelectArrow[1],0,0,18,17);
			}
		}
		//Ŭ����ũ�� ����ش� 
#ifdef  __CLANSUBCHIP__
		if(cldata.myPosition == 100 || cldata.myPosition == 101 || cldata.myPosition == 104){          
#else
		if(cldata.myPosition == 100 || cldata.myPosition == 101){          
#endif
			if(cldata.hClanMark16)
			{
				TempLen = lstrlen(cldata.name);
				TempCount2 = 6 - (TempLen/2);
				DrawSprite(CheckEditSize(sCharRectPosi[1].Rect.left,sCharRectPosi[1].Rect.right,cldata.name )-((12-TempLen/2))-TempCount2,
					sCharRectPosi[1].Rect.top+15+(256-sinMoveKindInter[SIN_CHARSTATUS]),cldata.hClanMark16,0,0,16,16);
			}
		}

		switch(RegiInfoPosi)
		{
		case 1:
			DrawSprite(RegiBox[0][0]+3,RegiBox[0][1]-27+(256-sinMoveKindInter[SIN_CHARSTATUS]),lpRegiBox[0],0,0,70,27);
		break;
		case 2:
			DrawSprite(RegiBox[1][0]+3,RegiBox[1][1]-27+(256-sinMoveKindInter[SIN_CHARSTATUS]),lpRegiBox[1],0,0,70,27);
		break;
		case 3:
			DrawSprite(RegiBox[2][0]+3,RegiBox[2][1]-27+(256-sinMoveKindInter[SIN_CHARSTATUS]),lpRegiBox[2],0,0,70,27);
		break;
		case 4:
			DrawSprite(RegiBox[3][0]+3,RegiBox[3][1]-27+(256-sinMoveKindInter[SIN_CHARSTATUS]),lpRegiBox[3],0,0,70,27);
		break;
		case 5:
			DrawSprite(RegiBox[4][0]+3,RegiBox[4][1]-27+(256-sinMoveKindInter[SIN_CHARSTATUS]),lpRegiBox[4],0,0,70,27);
		break;

		}
	}
}
/*----------------------------------------------------------------------------*
*							     ����  
*-----------------------------------------------------------------------------*/	
DWORD dwSkillTextTime = 0;
DWORD dwSkillTextTimeFlag = 0;
void cCHARSTATUS::Main()
{

	if(cSkill.SearchContiueSkill(SKILL_FROST_JAVELIN)){ 
		dwSkillTextTime++;
		if(dwSkillTextTime > 70){
			switch(dwSkillTextTimeFlag){
				case 0:
					dwSkillTextTimeFlag = 1;
				break;
				case 1:
					dwSkillTextTimeFlag = 2;
				break;
				case 2:
					dwSkillTextTimeFlag = 1;
				break;
			}
			dwSkillTextTime = 0;
		}
	}
	else dwSkillTextTimeFlag = 0;

	int  i;
	ShowCharArrow=0;
	RegiInfoPosi = 0;
	PointButtonPosi = 0;

	///////////////ä�� ����
	ChatKindIndexNum = 0; 
	if(cInterFace.ChatFlag){	    
		if(!SubChatHeight){
			for(int t = 0 ; t < 5 ; t++){
				if(pCursorPos.x > 100+(t*40) && pCursorPos.x < 100+((t+1)*40) && pCursorPos.y > 563 && pCursorPos.y < 563+16) 
					ChatKindIndexNum = t+1;
					switch(ChatKindIndexNum){
						case 1: //ä����ü
							cHelpPet.PetMessage("*ChatTab_All",0);

						break;
						case 2: //ä��Ŭ��
							cHelpPet.PetMessage("*ChatTab_Clan",0);

						break;
						case 3: //ä�õ���
							cHelpPet.PetMessage("*ChatTab_Party",0);

						break;
						case 4: //ä�� �ŷ�
							cHelpPet.PetMessage("*ChatTab_Trade",0);

						break;
						case 5: //ä�� �Ӹ�
							cHelpPet.PetMessage("*ChatTab_Whisper",0);

						break;
					}
				
			}
		}
	}

	//���� ä�� 
	NoticeChatIndexNum = 0;
	if(cInterFace.ChatFlag){	    
		if(!SubChatHeight){
			if(pCursorPos.x > 0 && pCursorPos.x < 45 && pCursorPos.y > 563 && pCursorPos.y < 563+16) {
				NoticeChatIndexNum = 1;
			}
		}
	}
	if(pCursorPos.x > 251 && pCursorPos.x < 269 && pCursorPos.y > 565 && pCursorPos.y < 583) 
		ShowCharArrow=1;
	if(OpenFlag){			
		for(i=0 ; i < 8 ; i++)
			if(pCursorPos.x >= RegiBox[i][0] && pCursorPos.x <= RegiBox[i][2] &&
				pCursorPos.y >= RegiBox[i][1] && pCursorPos.y <= RegiBox[i][3]){
				RegiInfoPosi = i+1;

			}
		for(i=0 ; i < 6 ; i++)
			if(pCursorPos.x >= PointButton[i][0] && pCursorPos.x <= PointButton[i][2] &&
				pCursorPos.y >= PointButton[i][1] && pCursorPos.y <= PointButton[i][3]){
				PointButtonPosi = i+1;
			}
		
	}
	else{ //ĳ���� �������ͽ��� ���������� �����Ҽ� �ִ� �ɷ¼�ġ �ʱ�ȭ 
		for(i = 0; i < 5 ; i++) 
			TempStatePoint[i] = 0; 

	}

	if(sinLoadOK)
		CheckLevel();

}
/*----------------------------------------------------------------------------*
*							     ���� 
*-----------------------------------------------------------------------------*/	
void cCHARSTATUS::Close()
{

}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/	
void cCHARSTATUS::LButtonDown(int x , int y)
{
	int cnt;

	//*����ä��
	if(NoticeChatIndexNum){ 
		NoticeChatIndex++;
		if(NoticeChatIndex >=3)NoticeChatIndex=0;

	}

	if(ChatKindIndexNum){
		ChatKindIndex = ChatKindIndexNum-1;
		SetChatMsgFilter(ChatKindIndex);
		switch(ChatKindIndexNum){
			case 1: //ä����ü
				cHelpPet.PetMessage("*ChatTab_All",1);

			break;
			case 2: //ä��Ŭ��
				cHelpPet.PetMessage("*ChatTab_Clan",1);

			break;
			case 3: //ä�õ���
				cHelpPet.PetMessage("*ChatTab_Party",1);

			break;
			case 4: //ä�� �ŷ�
				cHelpPet.PetMessage("*ChatTab_Trade",1);

			break;
			case 5: //ä�� �Ӹ�
				cHelpPet.PetMessage("*ChatTab_Whisper",1);

			break;
		}

	}

	if(OpenFlag){
		if(x > 251 && x < 269 && y > 565 && y < 583) //üũǥ�ø� ������ â�� �ݴ´� 
			OpenFlag = SIN_CLOSE;
		
		if(smConfig.DebugMode && VRKeyBuff['B']){          
			if(sinChar->StatePoint >9){
				sinPointButtonDown = 1;
				switch(PointButtonPosi){
				case 1: //��
					CheckCharForm(); //���� 
					sinChar->Strength +=9;
					sinChar->StatePoint -=9;
					for(cnt = 0; cnt < 4 ;cnt++) //��ĭ�� ������ ����    
						TempStatePoint[cnt] = TempStatePoint[cnt+1];	
					TempStatePoint[4] = POINT_STRENGTH;
					ReformCharForm(); //������ 
					cInvenTory.SetItemToChar();
					cInvenTory.CheckDamage(); //������ �ְ������� ������

				break;
				case 2: //���ŷ� 
					CheckCharForm(); //���� 
					sinChar->Spirit +=9;
					sinChar->StatePoint -=9;
					for(cnt = 0; cnt < 4 ; cnt++)
						TempStatePoint[cnt] = TempStatePoint[cnt+1];	
					TempStatePoint[4] = POINT_SPIRIT;
					ReformCharForm(); //������ 
					cInvenTory.SetItemToChar();
					cInvenTory.CheckDamage(); //������ �ְ������� ������


				break;

				case 3: //��� 
					CheckCharForm(); //���� 
					sinChar->Talent+=9;
					sinChar->StatePoint -=9;
					for(cnt = 0; cnt < 4 ;cnt++) //��ĭ�� ������ ����    
						TempStatePoint[cnt] = TempStatePoint[cnt+1];	
					TempStatePoint[4] = POINT_TALENT;
					ReformCharForm(); //������ 
					cInvenTory.SetItemToChar();

				break;

				case 4: //��ø�� 
					CheckCharForm(); //���� 
					sinChar->Dexterity+=9;
					sinChar->StatePoint -=9;
					for(cnt = 0; cnt < 4 ;cnt++) //��ĭ�� ������ ����    
						TempStatePoint[cnt] = TempStatePoint[cnt+1];	
					TempStatePoint[4] = POINT_DEXTERITY;
					ReformCharForm(); //������ 
					cInvenTory.SetItemToChar();
					cInvenTory.CheckDamage(); //������ �ְ������� ������

				break;
			
				case 5: //�ǰ� 
					CheckCharForm(); //���� 
					sinChar->Health+=9;
					sinChar->StatePoint -=9;
					for(cnt = 0; cnt < 4 ;cnt++) //��ĭ�� ������ ����    
						TempStatePoint[cnt] = TempStatePoint[cnt+1];	
					TempStatePoint[4] = POINT_HEALTH;
					ReformCharForm(); //������ 
					cInvenTory.SetItemToChar();
					cInvenTory.CheckDamage(); //������ �ְ������� ������

				break;

				case 6: //�������ͽ��ɷ�ġ�� �ǵ����� 
					for(cnt = 4; cnt >= 0 ; cnt--){
						if(TempStatePoint[cnt]){
							switch(TempStatePoint[cnt]){
							case POINT_STRENGTH:
								CheckCharForm(); //���� 
								sinChar->Strength -=9;
								sinChar->StatePoint +=9;
								TempStatePoint[cnt] = 0;
								ReformCharForm(); //������ 
								cInvenTory.SetItemToChar();
								cInvenTory.CheckDamage(); //������ �ְ������� ������
							break;
							case POINT_SPIRIT:
								CheckCharForm(); //���� 
								sinChar->Spirit -=9;
								sinChar->StatePoint +=9;
								TempStatePoint[cnt] = 0;
								ReformCharForm(); //������ 
								cInvenTory.SetItemToChar();
								cInvenTory.CheckDamage(); //������ �ְ������� ������
							break;
							case POINT_TALENT:
								CheckCharForm(); //���� 
								sinChar->Talent -=9;
								sinChar->StatePoint +=9;
								TempStatePoint[cnt] = 0;
								ReformCharForm(); //������ 
								cInvenTory.SetItemToChar();
								cInvenTory.CheckDamage(); //������ �ְ������� ������
							break;
							case POINT_DEXTERITY:
								CheckCharForm(); //���� 
								sinChar->Dexterity -=9;
								sinChar->StatePoint +=9;
								TempStatePoint[cnt] = 0;
								ReformCharForm(); //������ 
								cInvenTory.SetItemToChar();
								cInvenTory.CheckDamage(); //������ �ְ������� ������

							break;
							case POINT_HEALTH:
								CheckCharForm(); //���� 
								sinChar->Health -=9;
								sinChar->StatePoint +=9;
								TempStatePoint[cnt] = 0;
								ReformCharForm(); //������ 
								cInvenTory.SetItemToChar();		
								cInvenTory.CheckDamage(); //������ �ְ������� ������
							break;

							}
						break;
						}
					}

				break;
				}
			}

		}
		

		if(sinChar->StatePoint >0){
			sinPointButtonDown = 1;
			switch(PointButtonPosi){
			case 1: //��
				CheckCharForm(); //���� 
				sinChar->Strength ++;
				sinChar->StatePoint --;
				for(cnt = 0; cnt < 4 ;cnt++) //��ĭ�� ������ ����    
					TempStatePoint[cnt] = TempStatePoint[cnt+1];	
				TempStatePoint[4] = POINT_STRENGTH;
				ReformCharForm(); //������ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //������ �ְ������� ������

			break;
			case 2: //���ŷ� 
				CheckCharForm(); //���� 
				sinChar->Spirit ++;
				sinChar->StatePoint --;
				for(cnt = 0; cnt < 4 ; cnt++)
					TempStatePoint[cnt] = TempStatePoint[cnt+1];	
				TempStatePoint[4] = POINT_SPIRIT;
				ReformCharForm(); //������ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //������ �ְ������� ������


			break;

			case 3: //��� 
				CheckCharForm(); //���� 
				sinChar->Talent++;
				sinChar->StatePoint --;
				for(cnt = 0; cnt < 4 ;cnt++) //��ĭ�� ������ ����    
					TempStatePoint[cnt] = TempStatePoint[cnt+1];	
				TempStatePoint[4] = POINT_TALENT;
				ReformCharForm(); //������ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //������ �ְ������� ������

			break;

			case 4: //��ø�� 
				CheckCharForm(); //���� 
				sinChar->Dexterity++;
				sinChar->StatePoint --;
				for(cnt = 0; cnt < 4 ;cnt++) //��ĭ�� ������ ����    
					TempStatePoint[cnt] = TempStatePoint[cnt+1];	
				TempStatePoint[4] = POINT_DEXTERITY;
				ReformCharForm(); //������ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //������ �ְ������� ������

			break;
		
			case 5: //�ǰ� 
				CheckCharForm(); //���� 
				sinChar->Health++;
				sinChar->StatePoint --;
				for(cnt = 0; cnt < 4 ;cnt++) //��ĭ�� ������ ����    
					TempStatePoint[cnt] = TempStatePoint[cnt+1];	
				TempStatePoint[4] = POINT_HEALTH;
				ReformCharForm(); //������ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //������ �ְ������� ������

			break;

			case 6: //�������ͽ��ɷ�ġ�� �ǵ����� 
				for(cnt = 4; cnt >= 0 ; cnt--){
					if(TempStatePoint[cnt]){
						switch(TempStatePoint[cnt]){
						case POINT_STRENGTH:
							CheckCharForm(); //���� 
							sinChar->Strength --;
							sinChar->StatePoint ++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //������ 
							cInvenTory.SetItemToChar();
						break;
						case POINT_SPIRIT:
							CheckCharForm(); //���� 
							sinChar->Spirit --;
							sinChar->StatePoint ++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //������ 
							cInvenTory.SetItemToChar();
							cInvenTory.CheckDamage(); //������ �ְ������� ������
						break;
						case POINT_TALENT:
							CheckCharForm(); //���� 
							sinChar->Talent --;
							sinChar->StatePoint ++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //������ 
							cInvenTory.SetItemToChar();
							cInvenTory.CheckDamage(); //������ �ְ������� ������
						break;
						case POINT_DEXTERITY:
							CheckCharForm(); //���� 
							sinChar->Dexterity --;
							sinChar->StatePoint ++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //������ 
							cInvenTory.SetItemToChar();		
							cInvenTory.CheckDamage(); //������ �ְ������� ������

						break;
						case POINT_HEALTH:
							CheckCharForm(); //���� 
							sinChar->Health --;
							sinChar->StatePoint ++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //������ 
							cInvenTory.SetItemToChar();		
							cInvenTory.CheckDamage(); //������ �ְ������� ������
						break;

						}
					break;
					}
				}

			break;
			}

		}
		
	}
	
	
}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cCHARSTATUS::LButtonUp(int x , int y)
{
	sinPointButtonDown = 0; //��ư���϶� �ʱ�ȭ 
}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/	
void cCHARSTATUS::RButtonDown(int x , int y)
{
	if(ChatKindIndexNum){
		switch(ChatKindIndexNum){
			case 1: //ä����ü
				cHelpPet.PetMessage("*ChatTab_All",2);

			break;
			case 2: //ä��Ŭ��
				cHelpPet.PetMessage("*ChatTab_Clan",2);

			break;
			case 3: //ä�õ���
				cHelpPet.PetMessage("*ChatTab_Party",2);

			break;
			case 4: //ä�� �ŷ�
				cHelpPet.PetMessage("*ChatTab_Trade",2);

			break;
			case 5: //ä�� �Ӹ�
				cHelpPet.PetMessage("*ChatTab_Whisper",2);

			break;
		}

	}

}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cCHARSTATUS::RButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	
void cCHARSTATUS::KeyDown()
{

	if(sinGetKeyClick('C')){
		cHelpPet.PetMessage("*CarStatus",3);
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
	}
}

/*----------------------------------------------------------------------------*
*
*-----------------------------------------------------------------------------*/	
DWORD CharStatusColor[10] = {RGB( 255,255,255) , RGB( 255,0,0 ) ,RGB( 255,220,0),
					RGB( 142,230,254 ) , RGB( 128,255,128 ) ,RGB( 255,225,0 ),
					RGB( 255,180,200) , RGB(255,230,10) ,   RGB( 255,220,0 ),
					RGB( 255,220,0 ) 

};

int sinBackupTextColor = 0;

//ĳ���� �ɷ�ġ�� �����ش� 
void cCHARSTATUS::DrawCharText()
{

	CheckChageStateDisplay(); //���÷��̵� ���� üũ�Ѵ�
	if(!sinMoveKindInter[SIN_CHARSTATUS])return; //ĳ���� �������ͽ��� ���������� �����Ѵ� 

	int TempLen = 0;
	HDC	hdc;
	char strBuff[128];
	memset(strBuff,0,sizeof(strBuff));
	renderDevice.lpDDSBack->GetDC(&hdc);
	SelectObject( hdc, sinFont);
    SetBkMode( hdc, TRANSPARENT );
	SetTextColor( hdc, RGB(255,255,255) );
	

	/*ĳ���� �⺻ ���� */
	wsprintf(strBuff,"%s",cInvenTory.JobName); //���� �ڵ� 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[0].Rect.left,sCharRectPosi[0].Rect.right, strBuff),sCharRectPosi[0].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));
	
	wsprintf(strBuff,"%s",sinCharDisplay.szName); //ĳ���� �̸�  
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[1].Rect.left,sCharRectPosi[1].Rect.right, strBuff),sCharRectPosi[1].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

#ifdef  __CLANSUBCHIP__
	if(cldata.myPosition == 100 || 
		cldata.myPosition == 101 || cldata.myPosition == 104){  
#else
	if(cldata.myPosition == 100 || cldata.myPosition == 101){  
#endif
		TempLen = lstrlen(cldata.name);
		wsprintf(strBuff,"%s",cldata.name ); //ĳ���� �̸� 
		dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[1].Rect.left,sCharRectPosi[1].Rect.right, strBuff)+TempLen,sCharRectPosi[1].Rect.top+20+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));
	}

	wsprintf(strBuff,"%d",sinCharDisplay.Level); //���� 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[2].Rect.left,sCharRectPosi[2].Rect.right, strBuff),sCharRectPosi[2].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	
	if(sinChar->Level > 79){
		wsprintf(strBuff,"%dM",cCharStatus.sinGetNowExp()); //���� ����ġ 
		dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[3].Rect.left,sCharRectPosi[3].Rect.right, strBuff),sCharRectPosi[3].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
					strBuff,lstrlen(strBuff));

	}
	else{
		wsprintf(strBuff,"%d",cCharStatus.sinGetNowExp()); //���� ����ġ 
		dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[3].Rect.left,sCharRectPosi[3].Rect.right, strBuff),sCharRectPosi[3].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
					strBuff,lstrlen(strBuff));

	}
	

	if(sinChar->Level > 79){
		wsprintf(strBuff,"%dM",cCharStatus.sinGetNextExp()); //Experience the next level up
		dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[4].Rect.left,sCharRectPosi[4].Rect.right, strBuff),sCharRectPosi[4].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
					strBuff,lstrlen(strBuff));
	}
	else{
		wsprintf(strBuff,"%d",cCharStatus.sinGetNextExp()); //Experience the next level up
		dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[4].Rect.left,sCharRectPosi[4].Rect.right, strBuff),sCharRectPosi[4].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
					strBuff,lstrlen(strBuff));

	}

	if(AddVirtualLife[1]){
		AddVirtualLife[0] = ((int)sinGetLife() * sinVirtualLifePercent)/100;

	}

	// ����� - �ν��� ������(���) 
	if(AddVirtualMana[1]){
		AddVirtualMana[0] = ((int)sinGetMana() * sinVirtualManaPercent)/100;

	}

	// ����� - �ν��� ������(�ٷ�)
	if(AddVirtualStamina[1]){
		AddVirtualStamina[0] = ((int)sinGetStamina() * sinVirtualStaminaPercent)/100;

	}

	SetTextColor( hdc, CharStatusColor[SkillTextColor[SIN2_LIFE]]); 
	wsprintf(strBuff,"%d/%d",sinGetLife()+AddVirtualLife[0],sinCharDisplay.Life[1]+AddVirtualLife[1]); // ������ 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[5].Rect.left,sCharRectPosi[5].Rect.right, strBuff),sCharRectPosi[5].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));
//	SetTextColor( hdc, RGB(255,255,255) );
	
	// ����� - �ν��� ������(���)
	SetTextColor( hdc, CharStatusColor[SkillTextColor[SIN2_MANA]]); 
	wsprintf(strBuff,"%d/%d",sinGetMana(),sinCharDisplay.Mana[1]); //���� ����  
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[6].Rect.left,sCharRectPosi[6].Rect.right, strBuff),sCharRectPosi[6].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff,lstrlen(strBuff));

	// ����� - �ν��� ������(�ٷ�)
	SetTextColor( hdc, CharStatusColor[SkillTextColor[SIN2_STAMINA]]); 
	wsprintf(strBuff,"%d/%d",sinGetStamina(),sinCharDisplay.Stamina[1]); //���� ���׹̳�  
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[7].Rect.left,sCharRectPosi[7].Rect.right, strBuff),sCharRectPosi[7].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff,lstrlen(strBuff));

	SetTextColor( hdc, RGB(255,255,255) );

	
	/*ĳ���� �ɷ�ġ */
	wsprintf(strBuff,"%d",sinCharDisplay.Strength); //�� 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[8].Rect.left,sCharRectPosi[8].Rect.right, strBuff),sCharRectPosi[8].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));


	wsprintf(strBuff,"%d",sinCharDisplay.Spirit); //���ŷ�  
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[9].Rect.left,sCharRectPosi[9].Rect.right, strBuff),sCharRectPosi[9].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	

	wsprintf(strBuff,"%d",sinCharDisplay.Talent); //���   
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[10].Rect.left,sCharRectPosi[10].Rect.right, strBuff),sCharRectPosi[10].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	

	wsprintf(strBuff,"%d",sinCharDisplay.Dexterity); //��ø��   
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[11].Rect.left,sCharRectPosi[11].Rect.right, strBuff),sCharRectPosi[11].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"%d",sinCharDisplay.Health); //�ǰ�    
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[12].Rect.left,sCharRectPosi[12].Rect.right, strBuff),sCharRectPosi[12].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	if(sinChar->StatePoint){
		wsprintf(strBuff,"%d",sinChar->StatePoint); //����Ʈ      
		dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[23].Rect.left,sCharRectPosi[23].Rect.right, strBuff),sCharRectPosi[23].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
					strBuff,lstrlen(strBuff));

	}
//#define SIN_TEXT_COLOR_WHITE		0
//#define SIN_TEXT_COLOR_RED			1
//#define SIN_TEXT_COLOR_YELLOW		2
//#define SIN_TEXT_COLOR_BLUE			3

//#define SIN2_ATTACK_RATE		0
//#define SIN2_DAMAGE				1
//#define SIN2_DEFENSE			2
//#define SIN2_ABSORB				3
//#define SIN2_SPEED				4

//SkillTextColor[SIN2_ATTACK_RATE]
	/*���� ��� */

	
	SetTextColor( hdc, CharStatusColor[SkillTextColor[SIN2_ATTACK_RATE]]);
	wsprintf(strBuff,"%d",sinCharDisplay.Attack_Rating+sDisplayStateShow.Attack_Rate); //���� ������  
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[18].Rect.left,sCharRectPosi[18].Rect.right, strBuff),sCharRectPosi[18].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	
	//���ν�Ʈ ��� 
	if(dwSkillTextTimeFlag == 1){
		SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_BLUE;
	}
	if(dwSkillTextTimeFlag == 2){
		SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_YELLOW;
	}

	//�������� ���̳ʽ��� �ɰ�츦 �������ش�
	int DisplayDamage2[2] = {0,0};
	SetTextColor( hdc, CharStatusColor[SkillTextColor[SIN2_DAMAGE]]);
	DisplayDamage2[0] = sinCharDisplay.Attack_Damage[0]+sinTempDamage2[0]+sDisplayStateShow.Damage[0];
	DisplayDamage2[1] = sinCharDisplay.Attack_Damage[1]+sinTempDamage2[1]+sDisplayStateShow.Damage[1];
	if(DisplayDamage2[0] < 0 )DisplayDamage2[0] = 0;
	if(DisplayDamage2[1] < 0 )DisplayDamage2[1] = 0;
	wsprintf(strBuff,"%d-%d",DisplayDamage2[0],DisplayDamage2[1]); //������
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[19].Rect.left,sCharRectPosi[19].Rect.right, strBuff),sCharRectPosi[19].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	SetTextColor( hdc, CharStatusColor[SkillTextColor[SIN2_DEFENSE]]);
	wsprintf(strBuff,"%d",sinCharDisplay.Defence+sDisplayStateShow.Defense); //���潺 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[20].Rect.left,sCharRectPosi[20].Rect.right, strBuff),sCharRectPosi[20].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	SetTextColor( hdc, CharStatusColor[SkillTextColor[SIN2_ABSORB]]);
	wsprintf(strBuff,"%d",sinCharDisplay.Absorption+sinTempAbsorb+sDisplayStateShow.Absorb); //����� 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[21].Rect.left,sCharRectPosi[21].Rect.right, strBuff),sCharRectPosi[21].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));


	SetTextColor( hdc, CharStatusColor[SkillTextColor[SIN2_SPEED]]);
	wsprintf(strBuff,"%d",sinCharDisplay.Move_Speed); //�ӵ� 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[22].Rect.left,sCharRectPosi[22].Rect.right, strBuff),sCharRectPosi[22].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));
	


	//���׷� 
	SetTextColor( hdc, RGB(255,255,255) );
	wsprintf(strBuff,"%d",sinCharDisplay.Resistance[0]); //��ü  
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[13].Rect.left,sCharRectPosi[13].Rect.right, strBuff),sCharRectPosi[13].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"%d",sinCharDisplay.Resistance[5]); //�� 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[14].Rect.left,sCharRectPosi[14].Rect.right, strBuff),sCharRectPosi[14].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"%d",sinCharDisplay.Resistance[2]); //�� 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[15].Rect.left,sCharRectPosi[15].Rect.right, strBuff),sCharRectPosi[15].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"%d",sinCharDisplay.Resistance[4]); //���� 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[16].Rect.left,sCharRectPosi[16].Rect.right, strBuff),sCharRectPosi[16].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"%d",sinCharDisplay.Resistance[3]); //�ñ� 
	dsTextLineOut(hdc,CheckEditSize(sCharRectPosi[17].Rect.left,sCharRectPosi[17].Rect.right, strBuff),sCharRectPosi[17].Rect.top+(256-sinMoveKindInter[SIN_CHARSTATUS]),
				strBuff,lstrlen(strBuff));

	///////////���ڻ��� �ٲܼ��ְ� �� �����̴� �ε�!!


	renderDevice.lpDDSBack->ReleaseDC(hdc);

}

//ĳ���ͽ����̽��ͽ��� �������ʴ� ���ڸ� �����ش� 
void cCHARSTATUS::OtherStateShow()
{
	HDC	hdc;
	char strBuff[128];
	memset(strBuff,0,sizeof(strBuff));
	renderDevice.lpDDSBack->GetDC(&hdc);
    SetBkMode( hdc, TRANSPARENT );
	SetTextColor( hdc, RGB(255,255,255) );
	wsprintf(strBuff,"���ݼӵ�:%d",sinCharDisplay.Attack_Speed); //���ݼӵ�
	dsTextLineOut(hdc,100,300,strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"����:%d",sinCharDisplay.Chance_Block); //����
	dsTextLineOut(hdc,200,300,strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"ũ��Ƽ��:%d",sinCharDisplay.Critical_Hit); //ũ��Ƽ��
	dsTextLineOut(hdc,300,300,strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"���ñ���:%d",sinCharDisplay.Shooting_Range); //�����Ÿ� 
	dsTextLineOut(hdc,400,300,strBuff,lstrlen(strBuff));

	renderDevice.lpDDSBack->ReleaseDC(hdc);
	
}

//������ üũ�Ѵ� 
void cCHARSTATUS::CheckLevel()
{
	int UpLevelPoint;
	if(OldLevelTemp-176 != OldLevel)return;
	if(sinChar->Level > OldLevel){
		cInvenTory.SetItemToChar(); //������ �ö󰨿� ���� �ɷ�ġ ���� 
		UpLevelPoint = sinChar->Level - OldLevel;
		CheckCharForm(); //���� 
		if((sinChar->Level%2) == 0 && sinChar->Level >= 10){	//������ �ö� ��ŭ��ų����Ʈ�� �÷��ش� 	
				sinSkill.SkillPoint++;   //��ų ����Ʈ�� �÷��ش� 
		}
		if((sinChar->Level%2) == 0 && sinChar->Level >= 60){	//������ �ö� ��ŭ��ų����Ʈ�� �÷��ش� 	
				sinSkill.SkillPoint4++;   //��ų ����Ʈ�� �÷��ش� 
		}
		

		sinLevelPoint = sinChar->Level - OldLevel;
		
		if((sinQuest_levelLog & QUESTBIT_LEVEL_80_2) == QUESTBIT_LEVEL_80_2){
			if((sinQuest_levelLog & QUESTBIT_LEVEL_90_2) == QUESTBIT_LEVEL_90_2){
				if(sinChar->Level >=90){ //����Ʈ�� �����ϰ��Ŀ��� ����Ʈ�� 10���ö󰣴�
					sinChar->StatePoint +=3;
				}
			}
			if(sinChar->Level >=80){ //����Ʈ�� �����ϰ��Ŀ��� ����Ʈ�� 7���ö󰣴�
				sinChar->StatePoint += 2;
			}
		}
		sinChar->StatePoint += sinLevelPoint*5; 
		OldLevel = sinChar->Level;
		OldLevelTemp = OldLevel+176;
		ReformCharForm(); //������ 
		sinSetLife(sinChar->Life[1]);	//�������� ������ �ڱⰡ üũ�Ѵ� 
		sinSetMana(sinChar->Mana[1]);
		sinSetStamina(sinChar->Stamina[1]);
		cInvenTory.CheckDamage(); //������ �ְ������� ������

		//Pet �޼��� ����
		if(cHelpPet.PetKind && cHelpPet.PetShow){
			cHelpPet.LevelFlag = 1;
			switch(sinChar->Level){
				case 2:
					cHelpPet.PetMessageHelp("*Level_Message2-1");
					cHelpPet.PetMessageHelp("*Level_Message2-2");
				break;
				case 3:
					cHelpPet.PetMessageHelp("*Level_Message3");
				break;
				case 4:
					cHelpPet.PetMessageHelp("*Level_Message4");
				break;
				case 5:
					cHelpPet.PetMessageHelp("*Level_Message5");
				break;
				case 6:
					cHelpPet.PetMessageHelp("*Level_Message6-1");
					cHelpPet.PetMessageHelp("*Level_Message6-2");
				break;
				case 7:
					cHelpPet.PetMessageHelp("*Level_Message7");
				break;
				case 8:
					cHelpPet.PetMessageHelp("*Level_Message8");
				break;
				case 9:
					cHelpPet.PetMessageHelp("*Level_Message9-1");
					cHelpPet.PetMessageHelp("*Level_Message9-2");
				break;
				case 10:
					/*
					if(sinChar->JOB_CODE < 5 ){ //�۽�ũ��
						cHelpPet.PetMessageHelp("*Level_Message10_T");
					}
					else{ //����̿�
						cHelpPet.PetMessageHelp("*Level_Message10_M");
					}
					*/
					cHelpPet.PetMessageHelp("*Level_Message10-1");
					cHelpPet.PetMessageHelp("*Level_Message10-2");
				break;
			}

		}		
		//������ �׷��� ýũ �ʱ�ȭ *****************************************
		ResetEnergyGraph( 3 );

		//������ �������� ���� ��� ������ ���õ� ������ ����ش�
		void InitHelp();
		InitHelp();
		//====================end
		
		
	}
}

//ĳ���� �ɷ�ġ�� ����Ѵ� (��ŷ������ ���ؼ� )
void cCHARSTATUS::BackUpsinCharState(smCHAR *pChar)
{
}

//������ ����ġ�� ���´� 
int cCHARSTATUS::sinGetNowExp()
{

	//level<80
	//INT64 aa = GetXorExp64();
	//int ab = (int)(aa/1000000);
	//(int)  /INT64 GetXorExp64( smCHAR_INFO *smCharInfo )


	INT64 sinExp64 = 0;
	int   sinExp = 0;
	sinExp64 = GetXorExp64(sinChar);
	if(sinChar->Level > 79 ){
		sinExp  = (int)(sinExp64/1000000);

	}
	else{
		sinExp  = (int)(sinExp64);
	}

	return sinExp;
	
}

//���� ������ ����ġ�� ���´� 
int cCHARSTATUS::sinGetNextExp()
{
	//(int) /INT64 GetNextExp( int Level )

	int sinExp = 0;
	if(sinChar->Level > 79 ){

		sinExp = (int)((GetNextExp( sinChar->Level )/1000000));

	}
	else{
		sinExp  = (int)GetNextExp( sinChar->Level );
	}

	return sinExp;
}

//ĳ���� �������ͽ��� �ʱ�ȭ�Ѵ� 
int cCHARSTATUS::InitCharStatus(int kind)
{
	int Total,Total2;

	//kind 0�̺�Ʈ �ʱ�ȭ kind 1��罺�� �ʱ�ȭ
	switch(kind){
		case 0:
			if(sinChar->wVersion[1] == 1){
				sinSkillPontInitButtonFlag = 0;
				cMessageBox.ShowMessage(MESSAGE_ALREADY_INITSTATE);
				return FALSE;
			}
			sinSkillPontInitButtonFlag = 1;
		 	sinInitPointPassFlag = 1;
		break;
	}
	cMessageBox.ShowMessage(MESSAGE_GYUNGFUM_OK);
	Total = sinChar->Strength+sinChar->Spirit+sinChar->Talent+sinChar->Dexterity+sinChar->Health+sinChar->StatePoint;

	for(int i = 0 ; i < 4 ; i++)
	{
		if(sinChar->JOB_CODE == TempNewCharacterInit[i][0] )
		{
			sinChar->Strength  = TempNewCharacterInit[i][1];
			sinChar->Spirit    = TempNewCharacterInit[i][2];
			sinChar->Talent    = TempNewCharacterInit[i][3];
			sinChar->Dexterity = TempNewCharacterInit[i][4];
			sinChar->Health    = TempNewCharacterInit[i][5];
			Total2 = sinChar->Strength+sinChar->Spirit+sinChar->Talent+sinChar->Dexterity+sinChar->Health;
			sinChar->StatePoint = Total-Total2;
			sinChar->wVersion[1] = 1;
			ReformCharForm();//������ 
			cInvenTory.SetItemToChar();
			return TRUE;
		}
		if(sinChar->JOB_CODE == MorNewCharacterInit[i][0] ){
			sinChar->Strength  = MorNewCharacterInit[i][1];
			sinChar->Spirit    = MorNewCharacterInit[i][2];
			sinChar->Talent    = MorNewCharacterInit[i][3];
			sinChar->Dexterity = MorNewCharacterInit[i][4];
			sinChar->Health    = MorNewCharacterInit[i][5];
			Total2 = sinChar->Strength+sinChar->Spirit+sinChar->Talent+sinChar->Dexterity+sinChar->Health;
			sinChar->StatePoint = Total-Total2;
			sinChar->wVersion[1] = 1;
			ReformCharForm();//������ 
			cInvenTory.SetItemToChar();
			return TRUE;
		}

	}

	return TRUE;
}

//ĳ������ ���� ���¸� üũ�� Stats�� ���÷������ش�
int cCHARSTATUS::CheckChageStateDisplay()
{
	//�ð�üũ 
	memset(&sDisplayStateShow,0,sizeof(sDISPLAYSTATE));    
	for(int i = 0 ; i < 10 ; i++){
		if(sDisplayState[i].MaxTime){
			sDisplayStateShow.Absorb      += sDisplayState[i].Absorb;
			sDisplayStateShow.Attack_Rate += sDisplayState[i].Attack_Rate;
			sDisplayStateShow.Damage[0]   += sDisplayState[i].Damage[0];
			sDisplayStateShow.Damage[1]   += sDisplayState[i].Damage[1];
			if(sDisplayState[i].PercentDamage[0])
				sDisplayStateShow.Damage[0]   += (short)(sinChar->Attack_Damage[0]*sDisplayState[i].PercentDamage[0])/100;
			if(sDisplayState[i].PercentDamage[1])
				sDisplayStateShow.Damage[1]   += (short)(sinChar->Attack_Damage[1]*sDisplayState[i].PercentDamage[1])/100;
			
			sDisplayStateShow.Defense     += sDisplayState[i].Defense;

			if(sDisplayStateShow.Absorb <0)
				SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_RED;
			if(sDisplayStateShow.Defense <0)
				SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_RED;
			if(sDisplayStateShow.Damage[0] <0 || sDisplayStateShow.Damage[1] )
				SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_RED;


			if(sDisplayState[i].MaxTime <= dwPlayTime){
				memset(&sDisplayState[i],0,sizeof(sDISPLAYSTATE));
				cInvenTory.SetItemToChar();
			}

		}

	}
	//LV 90����Ʈ ���� 
	if(sinQuest_Level90_2.CODE && sinQuest_Level90_2.State >= 2 ){
		SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_RED;

	}
	return TRUE;
}

// �庰 - �׶��Ƽ ��ũ��
int cCHARSTATUS::UseGravityScrool()
{
	float	fstrength, fhealth,  ftalent , fLevel ,fSpirit;


	fstrength = (float)sinChar->Strength;
	fhealth   = (float)sinChar->Health;
	ftalent   = (float)sinChar->Talent;
	fSpirit   = (float)sinChar->Spirit;
	fLevel    = (float)sinChar->Level;


	if( sinChar->GravityScroolCheck[0] == 0 ) 
	{
		sinChar->Weight[1] = (short)(((fstrength*2) +(fhealth*1.5) + fLevel*3)+60);
		
	}

	if( sinChar->GravityScroolCheck[0] > 0 )
	{
		sinChar->Weight[1] = (short)(((fstrength*2) +(fhealth*1.5) + fLevel*3)+60) + ( sinChar->GravityScroolCheck[0] )*50;
		
	}



	sinChar->wVersion[1] = 1;
	ReformCharForm();//������ 11111111111111
	cInvenTory.SetItemToChar();

	return TRUE;
}

// ����� - ĳ���� �Ӽ��� ���� �ʱ�ȭ ������
int cCHARSTATUS::InitCharStatus_Attribute(int kind)
{
	int Total,Total2;
	int Brood_code = 0; // ���� ����
	int Job_Num = 0;

	cMessageBox.ShowMessage(MESSAGE_GYUNGFUM_OK);
	Total = sinChar->Strength+sinChar->Spirit+sinChar->Talent+sinChar->Dexterity+sinChar->Health+sinChar->StatePoint;
	
	for(int i = 0 ; i < 4 ; i++)
	{
		if(sinChar->JOB_CODE == TempNewCharacterInit[i][0])
		{
			Brood_code = BROOD_CODE_TEMPSKRON;
			Job_Num = i;
		}
		if(sinChar->JOB_CODE == MorNewCharacterInit[i][0])
		{
			Brood_code = BROOD_CODE_MORAYION;
			Job_Num = i;
		}
	}

	if(Brood_code == BROOD_CODE_TEMPSKRON)  // �۽�ũ��
	{
		switch(kind)
		{
		case 1: // ��
			sinChar->Strength  = TempNewCharacterInit[Job_Num][1];
			break;
		case 2: // ���ŷ�
			sinChar->Spirit    = TempNewCharacterInit[Job_Num][2];
			break;
		case 3: // ���
			sinChar->Talent    = TempNewCharacterInit[Job_Num][3];
			break;
		case 4: // ��ø��
			sinChar->Dexterity = TempNewCharacterInit[Job_Num][4];
			break;
		case 5: // �ǰ�
			sinChar->Health    = TempNewCharacterInit[Job_Num][5];
			break;		
		}
	}
	else // ����̿�
	{
		switch(kind)
		{
		case 1: // ��
			sinChar->Strength  = MorNewCharacterInit[Job_Num][1];
			break;
		case 2: // ���ŷ�
			sinChar->Spirit    = MorNewCharacterInit[Job_Num][2];
			break;
		case 3: // ���
			sinChar->Talent    = MorNewCharacterInit[Job_Num][3];
			break;
		case 4: // ��ø��
			sinChar->Dexterity = MorNewCharacterInit[Job_Num][4];
			break;
		case 5: // �ǰ�
			sinChar->Health    = MorNewCharacterInit[Job_Num][5];
			break;		
		}
	}	

	sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
	StartEffect(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, EFFECT_AGING); //����¡����Ʈ

	Total2 = sinChar->Strength+sinChar->Spirit+sinChar->Talent+sinChar->Dexterity+sinChar->Health;
	sinChar->StatePoint = Total-Total2;
	sinChar->wVersion[1] = 1;
	ReformCharForm();//������ 
	cInvenTory.SetItemToChar();

	return TRUE;

}
