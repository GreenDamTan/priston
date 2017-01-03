/*----------------------------------------------------------------------------*
*	���ϸ� :  sinHelp.cpp	
*	�ϴ��� :  �Źٶ� �����
*	�ۼ��� :  ����������Ʈ 2002�� 7��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"
#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "..\\field.h"


/*----------------------------------------------------------------------------*
*								�������� 			
*-----------------------------------------------------------------------------*/	
cSINHELP cSinHelp;
sSINHELP sSinHelp[SIN_MAX_HELP_NUMBER];
char szFileReadBuf2[80*128]; //���� ������ ������ �д´� 
sSOD_RANK sSod_Rank[10];

sTONY_TEST_IMAGE sTonyTest[10];	//Tony�� �׽�Ʈ �̹��� 

char szHelpDocBuff[100][128];

////////////////////////// HELP �ڽ� �̺�Ʈ �÷�
 int HelpBoxKindIndex = 0;

//////////////////////////�̹����� �ٲ� �󷷶׶� ������ĺ����������� �����Ƽ� ���״�
int MatHelpBoxImage[10];

//////// ��ũ�� ��ǥ���� (����â�� ūâ�� ���̸� �д�)
int TempScrollPosiY = 50;

int WingItemQuestIndex = 0;

////////////////////////// ���̵� ����
int MatGuideHelpTitle = 0;
int MatGuideHelpIcon = 0;
int MatGuideHelpIconGlay = 0;

int MatHelpTitle = 0;
int MatHelpTitleIcon = 0;
int MatHelpTitleIconGlay = 0;

int GuideHelpFlag = 0;
char *TeleportFilePath   = "image\\Sinimage\\help\\Teleport.sin";
int sinTeleportIndex = 0;
int sinFallgameIndex = 0;		//Ⱦ��ũ��
int sinTeleportMoneyIndex = 0;
int TeleportUseCose[] = {100,100,500,1000};

//���� ĳ�� �ڷ���Ʈ
char *haC_TeleportFilePath   = "image\\Sinimage\\help\\CastleTeleport.sin";

char *haC_Not_TeleportFilePath  = "image\\Sinimage\\help\\CastleTeleport_.sin";

int  haC_TeleportIndex=0;
int  haC_TeleportMoney=0;
//��������
char *haResearchFilePath1  = "image\\Sinimage\\help\\Research1.sin";
char *haResearchFilePath2  = "image\\Sinimage\\help\\Research2.sin";
char *haResearchFilePath3  = "image\\Sinimage\\help\\Research3.sin";

int   haResearchIndex = 0;
//�ڷ���Ʈ �ھ�
shaResearch haResearch;

/*----------------------------------------------------------------------------*
*							Ŭ���� �ʱ�, ���� 
*-----------------------------------------------------------------------------*/	
cSINHELP::cSINHELP()
{
	

}
cSINHELP::~cSINHELP()
{

}
/*----------------------------------------------------------------------------*
*							     �ʱ�ȭ 
*-----------------------------------------------------------------------------*/	
void cSINHELP::Init()
{
	
	//MatHelpCorner = CreateTextureMaterial( "Image\\SinImage\\Help\\HelpBoxCorner.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatHelpLine   = CreateTextureMaterial( "Image\\SinImage\\Help\\HelpBoxLine.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	MatScrollButton 	= CreateTextureMaterial( "Image\\SinImage\\Help\\scl_icon.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatScrollBar		= CreateTextureMaterial( "Image\\SinImage\\Help\\scl_bar.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	MatHelpBoxImage[0] = CreateTextureMaterial( "Image\\SinImage\\Help\\HelpBoxCorner.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatHelpBoxImage[1] = CreateTextureMaterial( "Image\\SinImage\\Help\\HelpBoxLine.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	
	char szBuff[128];
	for(int i = 2 ; i < 9 ; i++){
		wsprintf(szBuff,"Image\\SinImage\\Help\\box%d.tga",i+1);
		MatHelpBoxImage[i] = CreateTextureMaterial( szBuff , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	}
	MatHelpBoxImage[9] = CreateTextureMaterial( "Image\\SinImage\\Help\\Box_Line.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	MatGuideHelpTitle   = CreateTextureMaterial( "Image\\SinImage\\Help\\help_icon02t.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatGuideHelpIcon    = CreateTextureMaterial( "Image\\SinImage\\Help\\help_icon02.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatGuideHelpIconGlay= CreateTextureMaterial( "Image\\SinImage\\Help\\help_icon02_.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	MatHelpTitle        = CreateTextureMaterial( "Image\\SinImage\\Help\\help_icon01t.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatHelpTitleIcon    = CreateTextureMaterial( "Image\\SinImage\\Help\\help_icon01.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatHelpTitleIconGlay= CreateTextureMaterial( "Image\\SinImage\\Help\\help_icon01_.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );


	Load();

	//���̵� ���� �ε�
	if(!bCheckHostNameandPort)
		Interface_clanmenu.NoticeMessage_Init();

	//�ʱ�ȭ
	ScrollLineCount=0;
	cSinHelp.NPCTeleportFlag =0;

}
/*----------------------------------------------------------------------------*
*						    Bmp������ �ε��Ѵ� 
*-----------------------------------------------------------------------------*/	
void cSINHELP::Load()
{
	lpHelpTitle			= LoadDibSurfaceOffscreen( "Image\\SinImage\\Help\\box-test-title.bmp" );
	lpChangeJobTitle	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Help\\box-test-title.bmp" );
	lpHelpOk			= LoadDibSurfaceOffscreen( "Image\\SinImage\\Help\\box-test-title2.bmp" );


}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cSINHELP::Release()
{


}
/*----------------------------------------------------------------------------*
*							     �׸���  
*-----------------------------------------------------------------------------*/	
void cSINHELP::Draw()
{

	int i,j,k;
	for(i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){        
		if(sSinHelp[i].KindFlag){
			for(j = 0; j < sSinHelp[i].SizeW ; j++){
				for(k = 0; k < sSinHelp[i].SizeH ; k++){
					if(sSinHelp[i].MatFlag[j][k]){
						sinDrawTexImage(sSinHelp[i].sHelp_Box[j][k]->Mat,
							sSinHelp[i].sHelp_Box[j][k]->x,
							sSinHelp[i].sHelp_Box[j][k]->y,
							sSinHelp[i].sHelp_Box[j][k]->w,
							sSinHelp[i].sHelp_Box[j][k]->h,
							sSinHelp[i].sHelp_Box[j][k]->surW,
							sSinHelp[i].sHelp_Box[j][k]->surH,
							sSinHelp[i].sHelp_Box[j][k]->tLeft,
							sSinHelp[i].sHelp_Box[j][k]->tTop,
							sSinHelp[i].sHelp_Box[j][k]->tRight,
							sSinHelp[i].sHelp_Box[j][k]->tBottom,
							sSinHelp[i].sHelp_Box[j][k]->Transp);
					}
				}
			}
			//���� ���  
			for(j = 0; j < sSinHelp[i].SizeW-2 ; j++){
				for(k = 0; k < sSinHelp[i].SizeH-2 ; k++){
						sinDrawTexImage(MatHelpBoxImage[4],
							(float)sSinHelp[i].PosiX+64+(32*j),
							(float)sSinHelp[i].PosiY+32+(32*k),
							32.0f,32.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);
					
				}
			}
			//���� ����
			if(sSinHelp[i].KindFlag != 	SIN_HELP_KIND_SOD){       
				sinDrawTexImage(MatHelpBoxImage[9],(float)sSinHelp[i].PosiX+8,(float)sSinHelp[i].PosiY+48,
								(float)(sSinHelp[i].SizeW*32)+50,10.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,255);    
			}

			
			//���� ���̵� ���� 
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_NORMAL){ 
				dsDrawTexImage(MatGuideHelpTitle,sSinHelp[i].PosiX+340,sSinHelp[i].PosiY+14,64,32,255);      
				if(GuideHelpFlag){
					dsDrawTexImage(MatGuideHelpIcon,sSinHelp[i].PosiX+300+5,sSinHelp[i].PosiY+14,32,32,255);
				}
				else{
					dsDrawTexImage(MatGuideHelpIconGlay,sSinHelp[i].PosiX+300+5,sSinHelp[i].PosiY+14,32,32,255);
				}
			
				dsDrawTexImage(MatHelpTitle,sSinHelp[i].PosiX+55,sSinHelp[i].PosiY+13,64,32,255);     
				dsDrawTexImage(MatHelpTitleIcon,sSinHelp[i].PosiX+25,sSinHelp[i].PosiY+13,32,32,255);
			}

			/*
			dsDrawColorBox(sSinHelp[i].BackColor , sSinHelp[i].PosiX+32 , sSinHelp[i].PosiY+32
				,((sSinHelp[i].SizeW-1)*64) , ((sSinHelp[i].SizeH-1)*64 )); 
			*/

			//���� ��ũ�� 
			if(sSinHelp[i].ScrollFlag){   
				//��ũ�� �� ��� 
				sinDrawTexImage(MatScrollBar,(float)(sSinHelp[i].PosiX+(sSinHelp[i].SizeW-1)*32+80-6),
					(float)(sSinHelp[i].PosiY+TempScrollPosiY) , (float)8,(float)256);                 

				//��ũ�� ��ư 
				sinDrawTexImage(MatScrollButton,(float)sSinHelp[i].ScrollButton.x,
					(float)sSinHelp[i].ScrollButton.y-7,  (float)16,(float)16);        

			}
			//SOD ��ŷ 
			if(sSinHelp[i].KindFlag == 	SIN_HELP_KIND_SOD){ 
				DrawSprite(sSinHelp[i].PosiX+18, sSinHelp[i].PosiY+22,lpSodMenu,0 ,0 , 318,33); 
				for(int h = 0 ; h < 10 ; h++){
					DrawSprite(sSinHelp[i].PosiX+18, sSinHelp[i].PosiY+59+(h*24),lpSodLine,0 ,0 , 318,21); 
					if(sSod_Rank[h].Flag){
						DrawSprite(sSinHelp[i].PosiX+18+34, sSinHelp[i].PosiY+59+(h*24)-1,lpSodJobFace[sSod_Rank[h].Job-1],0,0,23,19);
					}
					
				}
			}	

			else{
				DrawSprite(sSinHelp[i].TitlePosi.x, sSinHelp[i].TitlePosi.y,lpHelpTitle,0 ,0 , 152,28); 

			}

			//����Ʈ�ϰ��  
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_QUEST){
				//���ڸ� �����ش� 
				//DrawSprite(sSinHelp[i].TitlePosi.x+20,sSinHelp[i].TitlePosi.y+10,lpChangeJobTitle,0 ,0 , 139,32);

			} 
			//��ư�� ����ش�  
			if(sSinHelp[i].KindFlag != SIN_HELP_KIND_WARPGATE && sSinHelp[i].KindFlag != SIN_HELP_KIND_INITPOINT&&sSinHelp[i].KindFlag!=SIN_HELP_KIND_RESEARCH){
			
					DrawSprite(sSinHelp[i].ButtonPosi.x,sSinHelp[i].ButtonPosi.y,lpHelpOk,0,0,48,23); //������ 
					DrawSprite(sSinHelp[i].ButtonPosi.x+9,sSinHelp[i].ButtonPosi.y+4,cMessageBox.lpButtonOk[0],0,0,32,16); //Ȯ�α��� 			
					
					if(sSinHelp[i].ButtonShowFlag) //��ư�� Ȱ��ȭ�϶� 
						DrawSprite(sSinHelp[i].ButtonPosi.x+9,sSinHelp[i].ButtonPosi.y+4,cMessageBox.lpButtonOk[1],0,0,32,16); //Ȯ�α��� 					
			}
			//��������
			if(sSinHelp[i].KindFlag ==SIN_HELP_KIND_RESEARCH){
				sSinHelp[i].ButtonPosi.x=250;
				DrawSprite(sSinHelp[i].ButtonPosi.x,sSinHelp[i].ButtonPosi.y,lpHelpOk,0,0,48,23); //������ 
				DrawSprite(sSinHelp[i].ButtonPosi.x+9,sSinHelp[i].ButtonPosi.y+4,cMessageBox.lpButtonCancel[0],0,0,32,16); //Ȯ�α��� 			
				if(sSinHelp[i].ButtonShowFlag) //��ư�� Ȱ��ȭ�϶� 
					DrawSprite(sSinHelp[i].ButtonPosi.x+9,sSinHelp[i].ButtonPosi.y+4,cMessageBox.lpButtonCancel[1],0,0,32,16); //Ȯ�α��� 
				haResearch.ButtonPosi.x=150,haResearch.ButtonPosi.y=sSinHelp[i].ButtonPosi.y;
				DrawSprite(haResearch.ButtonPosi.x,haResearch.ButtonPosi.y,lpHelpOk,0,0,48,23); //������ 
				DrawSprite(haResearch.ButtonPosi.x+9,haResearch.ButtonPosi.y+4,cMessageBox.lpButtonOk[0],0,0,32,16); //Ȯ�α��� 			
				if(haResearch.ButtonShowFlag) //��ư�� Ȱ��ȭ�϶� 
					DrawSprite(haResearch.ButtonPosi.x+9,haResearch.ButtonPosi.y+4,cMessageBox.lpButtonOk[1],0,0,32,16); //Ȯ�α��� 

			}
		}
	}

	///////���� Tony�� TEST �̹���
	for(i = 0 ; i < 10 ; i++){
		if(sTonyTest[i].Flag){
			if(sTonyTest[i].FileFormat == TONY_TGA)
				dsDrawTexImage( sTonyTest[i].Mat , sTonyTest[i].Posi.x, sTonyTest[i].Posi.y, sTonyTest[i].Size.x , sTonyTest[i].Size.y , 255 );   
			if(sTonyTest[i].FileFormat == TONY_BMP)
				DrawSprite(sTonyTest[i].Posi.x, sTonyTest[i].Posi.y , sTonyTest[i].lpTony ,0,0,sTonyTest[i].Size.x,sTonyTest[i].Size.y);
		}
	}

	////////// �κ��丮 ���� ����Ʈ�� �׸��� (����Ʈ�� ���� �������� ���̰��Ѵ� )
	for(int i = 0 ; i < MAX_INVEN_PARTICLE ; i++){
		if(sInven_Effect[i].Time){
			switch(sInven_Effect[i].Kind){
			case SIN_EFFECT_SHOW_INVEN:
				if(cInvenTory.OpenFlag)
					cInvenTory.sinDrawInvenTexImage(&sInven_Effect[i].InvenFace);
			break;

			case SIN_EFFECT_SHOW_WAREHOUSE:
				if(cWareHouse.OpenFlag)
					cInvenTory.sinDrawInvenTexImage(&sInven_Effect[i].InvenFace);
			break;

			case SIN_EFFECT_SHOW_TRADE:
				if(cTrade.OpenFlag)
					cInvenTory.sinDrawInvenTexImage(&sInven_Effect[i].InvenFace);
			break;

			}

		}
	}

}
/*----------------------------------------------------------------------------*
*							     ����  
*-----------------------------------------------------------------------------*/	
void cSINHELP::Main()
{
	GuideHelpFlag = 0; 
	sinTeleportIndex = 0;
	haC_TeleportIndex = 0;
	int sinTelePortLevel = 4;
	for(int i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){         
		if(sSinHelp[i].KindFlag){ 
			sSinHelp[i].ButtonShowFlag = 0;
			haResearch.ButtonShowFlag = 0;
			//��ư Ȱ��ȭ
			if(sSinHelp[i].ButtonPosi.x+18 < pCursorPos.x  && sSinHelp[i].ButtonPosi.x+18+32 > pCursorPos.x &&
				sSinHelp[i].ButtonPosi.y+7 < pCursorPos.y  && sSinHelp[i].ButtonPosi.y+7+16 > pCursorPos.y)
				sSinHelp[i].ButtonShowFlag = 1;
	
			if(sSinHelp[i].ScrollMouseDownFlag){
				if(pCursorPos.y > sSinHelp[i].ScrollMAXPosiY_UP-10 && pCursorPos.y < sSinHelp[i].ScrollMAXPosiY_DOWN+10  ){
					sSinHelp[i].ScrollButton.y = pCursorPos.y;
					if(sSinHelp[i].ScrollButton.y < sSinHelp[i].ScrollMAXPosiY_UP)sSinHelp[i].ScrollButton.y = sSinHelp[i].ScrollMAXPosiY_UP;
					if(sSinHelp[i].ScrollButton.y > sSinHelp[i].ScrollMAXPosiY_DOWN)sSinHelp[i].ScrollButton.y = sSinHelp[i].ScrollMAXPosiY_DOWN;
				}
			} 
			//���̵� ����
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_NORMAL){
				if(sSinHelp[i].PosiX+300 < pCursorPos.x  && sSinHelp[i].PosiX+300+100 > pCursorPos.x &&
					sSinHelp[i].PosiY+7 < pCursorPos.y  && sSinHelp[i].PosiY+7+32 > pCursorPos.y)
					GuideHelpFlag = 1; 
			}

			if(isDrawClanMenu) //Ŭ�� �޴��� ������쿡�� ������ �� �����ش� 
				memset(&sSinHelp[i],0,sizeof(sSINHELP)); 

			//Warp����Ʈ ������ ��ġ��� �Ѵ�
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_WARPGATE){
				if(!sSinWarpGate.ShowFlag){
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
				}
			}
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_INITPOINT){
				if(sinInitPointFlag7 == 0){
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
				}
			}
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT){
				// �庰 - �ʵ� ���� ���� ���� �̺�Ʈ
			//	if(sinChar->Level < 20) sinTelePortLevel = 3;	// ������ - 20�̻� �̵� ���� <2010.04.29>
				if(sinChar->Level < 55) sinTelePortLevel = 3;
				for(int j = 0; j < sinTelePortLevel ; j++){         
					if(sSinHelp[i].PosiX+20 < pCursorPos.x  && sSinHelp[i].PosiX + sSinHelp[i].SizeW*32 > pCursorPos.x &&
						sSinHelp[i].PosiY+230+(13*j) < pCursorPos.y  && sSinHelp[i].PosiY+230+(13*(j+1)) > pCursorPos.y)
						sinTeleportIndex = j+1;
				}
			}

			// pluto ���� ���� ����
			if( sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_MILTER )
			{
				for(int j = 0; j < 1 ; j++)
				{         
					if(sSinHelp[i].PosiX+20 < pCursorPos.x  && sSinHelp[i].PosiX + sSinHelp[i].SizeW*32 > pCursorPos.x &&
						sSinHelp[i].PosiY+270+(13*j) < pCursorPos.y  && sSinHelp[i].PosiY+270+(13*(j+1)) > pCursorPos.y)
					{
						sinTeleportIndex = j+1;
					}
				}
			}
			//<ha>������ �ڷ���Ʈ�� ���⼭  ���ش�.
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_C_TELEPORT){
				if(sSinHelp[i].PosiX+20 < pCursorPos.x  && sSinHelp[i].PosiX + sSinHelp[i].SizeW*32 > pCursorPos.x &&
						sSinHelp[i].PosiY+230+(13*3) < pCursorPos.y  && sSinHelp[i].PosiY+230+(13*4)> pCursorPos.y)
								haC_TeleportIndex = 1;
			}
			//<ha>��������
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_RESEARCH){
				haResearchIndex=0;
				if(haResearch.ButtonPosi.x+18 < pCursorPos.x  && haResearch.ButtonPosi.x+18+32 > pCursorPos.x &&
					haResearch.ButtonPosi.y+7 < pCursorPos.y  && haResearch.ButtonPosi.y+7+16 > pCursorPos.y)
						haResearch.ButtonShowFlag = 1;

				for(int j=0;j<sSinHelp[i].LineCnt+1;j++){
					if(sSinHelp[i].PosiX+20 < pCursorPos.x  && sSinHelp[i].PosiX + sSinHelp[i].SizeW*32> pCursorPos.x &&
					   sSinHelp[i].PosiY+70+(13*j) < pCursorPos.y  && sSinHelp[i].PosiY+70+(13*(j+1))> pCursorPos.y){	
						haResearchIndex = j-j/8;			
					}
				}
			}
			//100LV����Ʈ
			chaQuest.iFuryQuestIndex[0]=0;
			if(sSinHelp[i].KindFlag == HAQUEST_CODE_FURYOFPHANTOM){
				for(int j=0;j<8;j++){
					if(sSinHelp[i].PosiX+20 < pCursorPos.x  && sSinHelp[i].PosiX + sSinHelp[i].SizeW*20> pCursorPos.x &&
					   sSinHelp[i].PosiY+236+(14*j)-ScrollLineCount*14 < pCursorPos.y  && sSinHelp[i].PosiY+236+(14*(j+1))-ScrollLineCount*14> pCursorPos.y){	
						   //���ý� â ���
						   if(pCursorPos.y < 340)
							 chaQuest.iFuryQuestIndex[0]=j+1;			
					}
				}
			}
			chaPremiumitem.TelePortCore.FieldIndex[0]=0;
			short Temp=0;
			//�ڷ���Ʈ �ھ� 
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL){
				if(chaPremiumitem.TelePortCore.PageIndex == 3)
					Temp = 3;	//kyle //�ν�Ʈ���Ϸ���, �ν�Ʈ����  // �庰 - �ڷ���Ʈ �ھ� �ʵ� �߰�
				else
					Temp = 10;
				for(int j=0;j<Temp;j++){		
					if(sSinHelp[i].PosiX+20 < pCursorPos.x  && sSinHelp[i].PosiX + sSinHelp[i].SizeW*20> pCursorPos.x &&
					   sSinHelp[i].PosiY+106+(14*j)-ScrollLineCount*14 < pCursorPos.y  && sSinHelp[i].PosiY+106+(14*(j+1))-ScrollLineCount*14> pCursorPos.y){	
						   //���ý� â ���
						   //if(pCursorPos.y < 340)
						   chaPremiumitem.TelePortCore.FieldIndex[0]=j+1;
					}
				}
			}
		}
	}
}
/*----------------------------------------------------------------------------*
*							     ���� 
*-----------------------------------------------------------------------------*/	
void cSINHELP::Close()
{

}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/

void cSINHELP::LButtonDown(int x , int y)
{

	sITEM TempItem;

	//���̵� ����
	if(GuideHelpFlag){
		//���̵� ������ ����ش�

//������==========���̵嵵���� ���� ����ش�
void InitGuideHelp();
InitGuideHelp();
//������===========���̵� ���� ��


		for(int i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){        
			if(sSinHelp[i].KindFlag){
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_NORMAL){
						HelpBoxKindIndex = 0;
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
				}
			}
		}
	}

	char strBuff[128];
	int cnt=0;
	int Tempcnt=0,Tempcnt2=0;
	int k,p;
	for(int i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){        
		if(sSinHelp[i].KindFlag){
			//�ڷ���Ʈ �ھ�
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL){				
				if(chaPremiumitem.TelePortCore.FieldIndex[0]){
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					chaPremiumitem.TelePortCore.FieldIndex[1] = chaPremiumitem.TelePortCore.FieldIndex[0]+(chaPremiumitem.TelePortCore.PageIndex*10);
					cMessageBox.ShowMessage3(MESSAGE_TELEPORT_CORE,chaPremiumitem.TelePortCore.FieldName);		
					chaPremiumitem.Using_ItemKind = 0;
					//chaPremiumitem.UseTeleportScrollItem(chaPremiumitem.Teleport_ItemIndex);
				}		
			}
			//100LV����Ʈ
			if(sSinHelp[i].KindFlag == HAQUEST_CODE_FURYOFPHANTOM){				
				if(chaQuest.iFuryQuestIndex[0] && chaQuest.sHaQuest100LV.State == 12){
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					chaQuest.iFuryQuestIndex[1] = 0;
					chaQuest.iFuryQuestIndex[1] = chaQuest.iFuryQuestIndex[0];
					chaQuest.showFuryQuestItem();
					chaQuest.sHaQuest100LV.State++;
					break; //��������Ʈ�� ����� ���ɼ��� �����Ƿ� break�Ѵ�
				}
	
			}	
			//��������
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_RESEARCH){
				if(haResearch.ButtonPosi.x+18 < pCursorPos.x  && haResearch.ButtonPosi.x+18+32 > pCursorPos.x &&
					haResearch.ButtonPosi.y+7 < pCursorPos.y  && haResearch.ButtonPosi.y+7+16 > pCursorPos.y){
					//����
					cMessageBox.ShowMessage(MESSAGE_THANKS_RESEARCH);
					cSinHelp.SendResearch();
					memset(&sSinHelp[i],0,sizeof(sSINHELP));
					break;
				}
				if(haResearchIndex){
					for(k=0;k < sSinHelp[i].LineCnt-1;k++){
						wsprintf(strBuff,"%s",sSinHelp[i].szDocBuff[k]);
						cnt = lstrlen(strBuff);
						for(p = 0; p < cnt; p++){
							if(strBuff[p] == '>'){
								Tempcnt++;
							}
							if(strBuff[p] == ')'){
								if((haResearchIndex+haResearch.Result[0])==k){
									haResearch.Result[Tempcnt] = (haResearchIndex+haResearch.Result[0]); //line��
									haResearch.Value[Tempcnt-1]=atoi(&strBuff[p-1]);	                 //���� ����Ÿ 
								}
								if(haResearch.Value[0]==2){    //1)�ݴ� ���ý� 2������
									haResearch.Result[1+1] = 0;
									haResearch.Value[1] = 0;
								}
								if(haResearch.Value[0]==1){    //1)���� ���ý� 3������
									haResearch.Result[2+1] = 0;
									haResearch.Value[2] = 0;
								}							
							}		
						}
					}
				}
			}
			//�ڷ���Ʈ 
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT){
				if(sinTeleportIndex){
					if(sinTeleportIndex == 4){
						// �庰 - �ʵ� ���� ���� ���� �̺�Ʈ
						if(FieldLimitLevel_Table[12] > sinChar->Level)break; //�縷���� �ʵ巾��
					//	if( sinChar->Level < 20 )	break;	// ������ - ������ 20���� ������ �ڷ���Ʈ �Ұ� <2010.04.29>
					}
					//--------------------------------------------------------------------------//
					#ifdef HASIEGE_MODE
						 //<ha>������ �������� �ڷ���Ʈ
						if(cShop.haBuyMoneyCheck(TeleportUseCose[sinTeleportIndex-1])){ //üũ�Ѵ�.
					#else
						if(TeleportUseCose[sinTeleportIndex-1] <= sinChar->Money){ //üũ�Ѵ�.
					#endif
					//---------------------------------------------------------------------------//
					 	sinTeleportMoneyIndex = sinTeleportIndex;
						cMessageBox.ShowMessage3(MESSAGE_TELEPORT,sinTeleportDoc[sinTeleportIndex-1]);
						cSinHelp.NPCTeleportFlag = 0;
						HelpBoxKindIndex = 0;
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
						break; //��������Ʈ�� ����� ���ɼ��� �����Ƿ� break�Ѵ�
					}
					else{
						cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);

					}
				}
			}
			
			// pluto ���� ���� ����
			if( sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_MILTER )
			{
				if(sinTeleportIndex)
				{
					sinTeleportMoneyIndex = sinTeleportIndex;
					cMessageBox.ShowMessage3( MESSAGE_TELEPORT_DUNGEON, EndlessGameName );
					cSinHelp.NPCTeleportFlag = 0;
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					break; //��������Ʈ�� ����� ���ɼ��� �����Ƿ� break�Ѵ�
				}

			}
			//���� ĳ�� �ڷ���Ʈ 
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_C_TELEPORT){
				if(haC_TeleportIndex){
					if(haC_TeleportMoney <= sinChar->Money){ //üũ�Ѵ�.
					 	cMessageBox.ShowMessage3(MESSAGE_CASTLE_TELEPORT,HaWarpGateName[0]);
						HelpBoxKindIndex = 0;
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
						break; //��������Ʈ�� ����� ���ɼ��� �����Ƿ� break�Ѵ�
					}
					else{
						cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);

					}
				}
			}

			if(sSinHelp[i].ScrollButton.x < x &&  sSinHelp[i].ScrollButton.x+16 > x &&
				sSinHelp[i].ScrollButton.y-7 < y &&  sSinHelp[i].ScrollButton.y+16+7 > y){
					sSinHelp[i].ScrollMouseDownFlag = 1;
			}
			if(sSinHelp[i].ScrollButton.x < x &&  sSinHelp[i].ScrollButton.x+16 > x &&
				sSinHelp[i].ScrollMAXPosiY_UP < y &&  sSinHelp[i].ScrollMAXPosiY_DOWN > y){
					sSinHelp[i].ScrollButton.y = pCursorPos.y;
			}
	
            //��Ⱑ Ȯ�� ��ư�� �������̴� 
			if(sSinHelp[i].ButtonPosi.x-20 < x &&  sSinHelp[i].ButtonPosi.x+68+20 > x &&
				sSinHelp[i].ButtonPosi.y-20 < y &&  sSinHelp[i].ButtonPosi.y+27+20 > y){ 

				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL){
					chaPremiumitem.Using_ItemKind =0;	
					
				}
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT){
					cSinHelp.NPCTeleportFlag = 0;						
				}
			
				// pluto ���� ���� ����
				if( sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_MILTER)
				{
					cSinHelp.NPCTeleportFlag = 0;
				}

				if(sSinHelp[i].KindFlag == MESSAGE_FALLGAME)
				{
					cSinHelp.NPCTeleportFlag = 0;						
				}	
				//100LV ����Ʈ
				if(sSinHelp[i].KindFlag == HAQUEST_CODE_FURYOFPHANTOM)
				{				
					switch(chaQuest.sHaQuest100LV.State){
						case 0:
							cMessageBox.ShowMessage3(MESSAGE_QUEST_100LV,ha100LVQuestName);								
						break;
						case 1:
							chaQuest.sHaQuest100LV.State=2;
						break;
						case 3:
						case 4:
							if(chaQuest.FuryBattleFlag)
							{
								chaQuest.sHaQuest100LV.State++;
								WarpField2(START_FIELD_MORYON);  //���ư���	
								chaQuest.FuryBattleFlag = false;
							}
							else
								cMessageBox.ShowMessage3(MESSAGE_QUEST_100LVBATTLEOK,"");						
						break;
						case 5:
							chaQuest.sHaQuest100LV.State=6;
							chaQuest.showFuryQuest();               //���� ������ ���� ȣ�����ش�.
						break;
						case 8:
						case 9:
						case 10:
						case 11:
							if(chaQuest.FuryBattleFlag){
								chaQuest.sHaQuest100LV.State++;
								WarpField2(START_FIELD_MORYON);  //���ư���	
								chaQuest.FuryBattleFlag = false;
							}
							else 
								cMessageBox.ShowMessage3(MESSAGE_QUEST_100LVBATTLEOK,"");										
						break;
						case 13:
							if(chaQuest.iFuryQuestIndex[1])
								cMessageBox.ShowMessage3(MESSAGE_QUEST_100LVITEMOK,ha100LVQuestItemName[chaQuest.iFuryQuestIndex[1]-1]);
						break;
					}
					
					chaQuest.bFuryQuestFlag = false;
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					break; //��������Ʈ�� ����� ���ɼ��� �����Ƿ� break�Ѵ�					
				}
				//�������� �ʱ�ȭ
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_RESEARCH){
					for(int i=0;i<10;i++){
						haResearch.Result[i] = 0;
						haResearch.Value[i] = 0;
					}
				}
				//��������Ʈ	30
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_Q_LEVEL30){
					if(sinQuest_Level30.State == 0){
						cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL30,""); 
					}
					if(sinQuest_Level30.State == 2){ //���� 30 ����Ʈ �Ϸ� 
						CheckCharForm();//���� 
						sinChar->StatePoint += 5; //��������Ʈ 5�߰�
						cMessageBox.ShowMessage(MESSAGE_PLUS_STATE_POINT5);
						sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
						sinQuest_levelLog |= QUESTBIT_LEVEL_30; //�ڵ带 �������ش�
						sinQuest_Level30.State = 3;
						sinQuest_Level30.BackUpCode = sinQuest_Level30.CODE;
						sinQuest_Level30.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						EndQuest_Code(sinQuest_Level30.BackUpCode);
						sinQuest_Level30.CODE = 0;
						ReformCharForm();//������


					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					break; //��������Ʈ�� ����� ���ɼ��� �����Ƿ� break�Ѵ�
				}
				//��������Ʈ 55 (�۽�ũ��)
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_Q_LEVEL55){
					if(sinQuest_Level55.State == 0){
						cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL55,""); 
					}
					if(sinQuest_Level55.State == 2){ //���� 30 ����Ʈ �Ϸ� 
						CheckCharForm();//���� 
						sinSkill.SkillPoint += 1; //��ų����Ʈ 1�߰�
						cMessageBox.ShowMessage(MESSAGE_PLUS_SKILL_POINT);
						sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
						sinQuest_levelLog |= QUESTBIT_LEVEL_55; //�ڵ带 �������ش�
						sinQuest_Level55.State = 3;
						sinQuest_Level55.BackUpCode = sinQuest_Level55.CODE;
						sinQuest_Level55.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						EndQuest_Code(sinQuest_Level55.BackUpCode);
						sinQuest_Level55.CODE = 0;

						ReformCharForm();//������

					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					break;

				}
				//��������Ʈ 55 (����̿�)
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_Q_LEVEL55_2){
					if(sinQuest_Level55_2.State == 0){
						cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL55_2,""); 
					}
					if(sinQuest_Level55_2.State == 2){ //���� 30 ����Ʈ �Ϸ� 
						CheckCharForm();//���� 
						sinSkill.SkillPoint += 1; //��ų����Ʈ 1�߰�
						cMessageBox.ShowMessage(MESSAGE_PLUS_SKILL_POINT);
						sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
						sinQuest_levelLog |= QUESTBIT_LEVEL_55; //�ڵ带 �������ش�
						sinQuest_Level55_2.State = 3;
						sinQuest_Level55_2.BackUpCode = sinQuest_Level55_2.CODE;
						sinQuest_Level55_2.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						EndQuest_Code(sinQuest_Level55_2.BackUpCode);
						sinQuest_Level55_2.CODE = 0;
						ReformCharForm();//������

					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					break;
				}

				//��������Ʈ 70
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_Q_LEVEL70){
					if(sinQuest_Level70.State == 0){
						cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL70,""); 
					}
					if(sinQuest_Level70.State == 2){ //���� 30 ����Ʈ �Ϸ� 
						CheckCharForm();//���� 
						sinChar->StatePoint += 5; //��������Ʈ 5�߰�
						sinSkill.SkillPoint += 1; //��ų����Ʈ 1�߰�
						cMessageBox.ShowMessage(MESSAGE_PLUS_POINT_70);
						sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
						sinQuest_levelLog |= QUESTBIT_LEVEL_70; //�ڵ带 �������ش�
						sinQuest_Level70.State = 3;
						sinQuest_Level70.BackUpCode = sinQuest_Level70.CODE;
						sinQuest_Level70.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						EndQuest_Code(sinQuest_Level70.BackUpCode);
						sinQuest_Level70.CODE = 0;
						ReformCharForm();//������

					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					break;
				}
				//��������Ʈ 80
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_Q_LEVEL80){
					if(sinQuest_Level80.State == 0){
						cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL80,""); 
					}
					if(sinQuest_Level80.State == 2){ //���� 30 ����Ʈ �Ϸ� 
						CheckCharForm();//���� 
						sinChar->StatePoint += 5; //��������Ʈ 5�߰�
						sinSkill.SkillPoint += 2; //��ų����Ʈ 1�߰�
						cMessageBox.ShowMessage(MESSAGE_PLUS_POINT_80);
						sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
						sinQuest_levelLog |= QUESTBIT_LEVEL_80; //�ڵ带 �������ش�
						sinQuest_Level80.State = 3;
						sinQuest_Level80.BackUpCode = sinQuest_Level80.CODE;
						sinQuest_Level80.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						EndQuest_Code(sinQuest_Level80.BackUpCode);
						sinQuest_Level80.CODE = 0;
						ReformCharForm();//������

					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					break;
				}
				//��������Ʈ 85
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_Q_LEVEL85){
					if(sinQuest_Level85.State == 0){
						cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL85,""); 
					}
					if(sinQuest_Level85.State == 1){ 
						//��� ������ �ٽ�ȹ��
						if(!cInvenTory.SearchItemCode(sinQT1|sin08)){
							SendQuestCommandToServer( SIN_QUEST_CODE_LEVEL85 , 0, 0, 0 );
						}
						sinQuest_Level85.State = 2;
						sinQuest_Level85.Monster[0] =0; //���� ��ɼ� �ʱ�ȭ
					}
					if(sinQuest_Level85.State == 3){ //���� 30 ����Ʈ �Ϸ� 
						CheckCharForm();//���� 
						sinQuest_levelLog |= QUESTBIT_LEVEL_85; //�ڵ带 �������ش�
						sinQuest_Level85.State = 4;
						sinQuest_Level85.BackUpCode = sinQuest_Level85.CODE;
						sinQuest_Level85.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						EndQuest_Code(sinQuest_Level85.BackUpCode);
						sinQuest_Level85.CODE = 0;
						ReformCharForm();//������
						//������ ����� 1% ����ġ���� 

					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					break;
				}
				//��������Ʈ 90
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_Q_LEVEL90){
					if(sinQuest_Level90.State == 0){
						cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL90,""); 
					}
					if(sinQuest_Level90.State == 2){ //���� 30 ����Ʈ �Ϸ� 
						CheckCharForm();//���� 
						sinQuest_levelLog |= QUESTBIT_LEVEL_90; //�ڵ带 �������ش�
						sinQuest_Level90.State = 3;
						sinQuest_Level90.BackUpCode = sinQuest_Level90.CODE;
						sinQuest_Level90.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						EndQuest_Code(sinQuest_Level90.BackUpCode);
						sinQuest_Level90.CODE = 0;
						ReformCharForm();//������
						cInvenTory.SetItemToChar();
						if(!cInvenTory.SearchItemCode(sinQT1|sin07)){
							SendQuestCommandToServer( SIN_QUEST_CODE_LEVEL90 , 0, 0, 0 );
						}

					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
				}
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_Q_LEVEL80_2){
					if(sinQuest_Level80_2.State == 0){
						cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL80_2,""); 
					}
					if(sinQuest_Level80_2.State == 1){ 
						sinQuest_Level80_2.State = 2; 
						SetQuestBoard();
						//	SaveGameData();
						//cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL80_2_1,""); 
					}
					if(sinQuest_Level80_2.State == 3){ 
						TempItem.CODE = (sinQT1 | sin12);
						cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
						if(	cInvenTory.CheckInvenEmpty(&TempItem)){
							if(sinChar->Weight[0]+1 <= sinChar->Weight[1]){
								SendWingQuestItem(); //���� �������� �������� ������
							}
							else{
								cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
								break;
							}
						}
						else{
							cMessageBox.ShowMessage(MESSAGE_OVER_SPACE); //��������
							break;
						}
						sinQuest_Level80_2.State = 4; 
						DeleteQuestItem(sinQT1|sin09);
						DeleteQuestItem(sinQT1|sin10);
						DeleteQuestItem(sinQT1|sin11);

						HelpBoxKindIndex = 0;
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
						cInvenTory.SetItemToChar();
						SetQuestBoard();
						break;
						
					}
					if(sinQuest_Level80_2.State == 4){
						CheckCharForm();//���� 
						sinQuest_levelLog |= QUESTBIT_LEVEL_80_2; 
						sinQuest_Level80_2.BackUpCode = sinQuest_Level80_2.CODE;
						sinQuest_Level80_2.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						EndQuest_Code(sinQuest_Level80_2.BackUpCode);
						sinQuest_Level80_2.CODE = 0;
						sinQuest_Level80_2.State = 5;
						//�ƹ�������
						//SendQuestCommandToServer( SIN_QUEST_CODE_LEVEL80_2 , 0, 0, 0 );
						
						//������й�
						//cInvenTory.SetItemToChar(); 
						DeleteQuestItem(sinQT1|sin12); //��õ�� ����
						//����Ʈ �������� ���� ������ ��������Ʈ�� ����
						ReformCharStatePoint(sinChar , sinQuest_levelLog);
						ReformCharForm();//������
						cInvenTory.SetItemToChar();
						sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
						cMessageBox.ShowMessage(MESSAGE_PLUS_POINT_80_2);
						SetQuestBoard();

					}
					
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
				}

				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_Q_LEVEL90_2){
					if(sinQuest_Level90_2.State == 0){
						cMessageBox.ShowMessage3(MESSAGE_QUEST_LEVEL90_2,""); 
					}
					if(sinQuest_Level90_2.State == 1){ 
						//���ָ� ������
						sinQuest_Level90_2.State = 2;
						cInvenTory.SetItemToChar();
						SetQuestBoard();
					}
					if(sinQuest_Level90_2.State == 3){ 
						CheckCharForm();//���� 
						sinQuest_levelLog |= QUESTBIT_LEVEL_90_2; 
						sinQuest_Level90_2.BackUpCode = sinQuest_Level90_2.CODE;
						sinQuest_Level90_2.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						EndQuest_Code(sinQuest_Level90_2.BackUpCode);
						sinQuest_Level90_2.CODE = 0;
						sinQuest_Level90_2.State = 4;
						DeleteQuestItem(sinQT1|sin13); //�ο� �ƹ��� ����
						//����Ʈ �������� ���� ������ ��������Ʈ�� ����
						ReformCharStatePoint(sinChar , sinQuest_levelLog);
						ReformCharForm();//������
						cInvenTory.SetItemToChar();
						sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
						cMessageBox.ShowMessage(MESSAGE_PLUS_POINT_90_2);
						SetQuestBoard();

					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
				}
			

				if(ChangeJobButonCheck2){
					ChangeJobButonCheck2 = 0;
					break; //��ư�̺�Ʈ ���������� �����ش� 
				}
				if(sSinHelp[i].KindFlag == SIN_HELP_KIND_QUEST){ //������ ������ ���ܼ� �̷��� �����ߵ�!
					if(ChangeJobButonCheck2){
						ChangeJobButonCheck2 = 0;
						break; //��ư�̺�Ʈ ���������� �����ش� 
					}
					//��� �۽�ũ��
					if(sinChar->ChangeJob == 0){ //1������
						if (sinChar->JOB_CODE <= 4 || sinChar->JOB_CODE == 9)
						{
							switch(sinQuest_ChangeJob.State)
							{
								case  0:
									cMessageBox.ShowMessage3(MESSAGE_QUEST,""); //��¿������ ���� 
									memset(&sSinHelp[i],0,sizeof(sSINHELP)); 

								break;
								case SIN_QUEST_PROGRESS:
									sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1);
									cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,sinJobList->szName2);
									HelpBoxKindIndex = 0;
									memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
									
								break;
								case SIN_QUEST_COMPLETE:

								break;
							}
						}
						//��� ����̿� 
						if (sinChar->JOB_CODE >4 && sinChar->JOB_CODE <= 8 || sinChar->JOB_CODE == 10)
						{
							switch(sinQuest_ChangeJob2.State)
							{   
								case  0: 
									if((FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB2_NPC_M) ||FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB2_NPC_D)) && sinChar->ChangeJob < 1){
										sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1);
										cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,sinJobList->szName2);
										HelpBoxKindIndex = 0;

									}
									else
									{
										cMessageBox.ShowMessage3(MESSAGE_QUEST_CHANGEJOB_MORAYOIN,""); 
										
									}
									memset(&sSinHelp[i],0,sizeof(sSINHELP));

								break;
								//�ؿ� �ΰ��� ���ǰ������ʴ�
								case SIN_QUEST_PROGRESS:
	//								sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1);
	//								cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,sinJobList->szName2);
	//								HelpBoxKindIndex = 0;
	//								memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
								break;
								case 4:
									/*
									sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1);
									cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,sinJobList->szName2);
									HelpBoxKindIndex = 0;
									memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
									*/

								break;
							}
						}
					}
					if(sinChar->ChangeJob == 1 ){ //�۽�ũ�� ����̿� 3������  
						switch(sinQuest_ChangeJob3.State){
							case 0:
								cMessageBox.ShowMessage3(MESSAGE_QUEST_CHANGEJOB3,""); 
							break;
							case 1: //���� ������

							break;
							case 2: //���� ������ �ٸ����
								//�������� �޴´�
								//�������� �޾ƿ��� �Լ� 
								switch(sinChar->JOB_CODE)
								{
									case 1:		//������
										TempItem.CODE = (sinWA1 | sin08);
									break;
									case 2:		//��ī�ϼ�
										TempItem.CODE = (sinWC1 | sin08);
									break;
									case 3:		//����
										TempItem.CODE = (sinWS1 | sin10);
									break;
									case 4:		//����ũ��
										TempItem.CODE = (sinWP1 | sin09);
									break;
									case 5:		//��Ż��Ÿ
										TempItem.CODE = (sinWT1 | sin09);
									break;
									case 6:		//����Ʈ
										TempItem.CODE = (sinWS2 | sin10);
									break;
									case 7:		//������
										TempItem.CODE = (sinWM1 | sin09);
									break;
									case 8:		//������Ƽ��
										TempItem.CODE = (sinWM1 | sin09);
									break;
								
								}
								//����Ʈâ�� ����������� â�� �����Ѵ�
								cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�

								//�������� ���� ������ �ɶ� �޼����� ������ ������ �����
								if(	cInvenTory.CheckInvenEmpty(&TempItem)){
									if(sinChar->Weight[0]+35 <= sinChar->Weight[1]){
										SendWingQuestItem(); //���� �������� �������� ������
										sinQuest_ChangeJob3.State = 3; 
										DeleteQuestItem((sinQT1|sin06));
										StartQuest_Code(sinQuest_ChangeJob3.CODE);
									}
									else{
										sinQuest_ChangeJob3.State = 1;
										cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
									}
								}
								else{
									sinQuest_ChangeJob3.State = 1;
									cMessageBox.ShowMessage(MESSAGE_OVER_SPACE); //��������
								}
							break;
							case 3: //������ ������ ������

							break;
							case 4: //���� ������ �ް� ������ �Ѵ�
								sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1);
								cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,sinJobList->szName2);
								HelpBoxKindIndex     = 0;
								sinChar->wVersion[1] = 0;
								
							break;
					
							break;
						}
						HelpBoxKindIndex = 0; //�ϴ��� ������ ����Ʈ �Ϸ��������� ������ â�� �ݾ��ش�
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					}
					//3�� ��������Ʈ �Ϸ�
					if(sinQuest_ChangeJob3.CODE && sinChar->ChangeJob == 2 && sinQuest_ChangeJob3.State == 6){
						//���⼭ ����Ʈ�� ������ HP�� 15�÷��ش�
						sinQuest_ChangeJob3.BackUpCode = sinQuest_ChangeJob3.CODE;
						sinChar->wVersion[1]      = 0;
						sinQuest_ChangeJob3.CODE  = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
						sinQuest_ChangeJob3.State = 0;
						EndQuest_Code(sinQuest_ChangeJob3.BackUpCode);
						cMessageBox.ShowMessage(MESSAGE_INCRE_LIFE15);
						cInvenTory.SetItemToChar();
						HelpBoxKindIndex = 0; //�ϴ��� ������ ����Ʈ �Ϸ��������� ������ â�� �ݾ��ش�
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 

					}
					//3�������� �޼����� ��ٷ� �����ʱ����ؼ�
					if(sSinHelp[i].KindFlag == SIN_HELP_KIND_QUEST && sinChar->ChangeJob == 2 ){ //�۽�ũ�� ����̿� 4������  
						switch(sinQuest_ChangeJob4.State){  
							case 0:
								if(sinQuest_ChangeJob4.Monster[1]){
									sinQuest_ChangeJob4.State = 1;
									sinChangeJob4MonSet(&sinQuest_ChangeJob4); //���͸� �����Ѵ�
									SetQuestBoard();
								}
								else{
									cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB4,"");

								}
							break;
							case 2:
								sinChangeJob4MonSet(&sinQuest_ChangeJob4); //���͸� �����Ѵ�
								sinQuest_ChangeJob4.State = 3; 
								SaveGameData();
								SetQuestBoard();
								
							break;
							case 4:
							case 7:
								sinChangeJob4MonSet(&sinQuest_ChangeJob4); //���͸� �����Ѵ�
								sinQuest_ChangeJob4.State = 5; 
								SetQuestBoard();
							break;
							case 6:
								if( MouseItem.Flag )		// pluto ���� �ο� �ƹķ� ���´�
								{
									return;
								}
								DeleteQuestItem(sinQT1|sin13);
								sinQuest_ChangeJob4.State = 7;
								//npc�� �ο�� �Լ�
								Start_QuestArena(SIN_QUEST_CODE_CHANGEJOB4 , 0, 0 );
							break;
							
							case 8: //npc�¸�
								sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1);
								cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB4_2,sinJobList->szName2);
							break;

						}
						HelpBoxKindIndex = 0; //�ϴ��� ������ ����Ʈ �Ϸ��������� ������ â�� �ݾ��ش�
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					}
				}

				if(sSinHelp[i].KindFlag ==SIN_HELP_KIND_QUEST_WING){  
					//�������� ����Ʈ
					switch(WingItemQuestIndex){
						case 0: //��Ż��
							cMessageBox.ShowMessage3(MESSAGE_RECVITEM,WingItemName[WingItemQuestIndex]);

						break;
						case 1: //�ǹ���
							cMessageBox.ShowMessage3(MESSAGE_RECVITEM,WingItemName[WingItemQuestIndex]);

						break;
						case 2: //�����
							cMessageBox.ShowMessage3(MESSAGE_RECVITEM,WingItemName[WingItemQuestIndex]);

						break;
						case 3: //���̾���
							cMessageBox.ShowMessage3(MESSAGE_RECVITEM,WingItemName[WingItemQuestIndex]);

						break;
						case 4: //���̾ƽ���
							cMessageBox.ShowMessage3(MESSAGE_RECVITEM,WingItemName[WingItemQuestIndex]);
						break;
						case 5: //�ͽ�Ʈ���� �ν�Ʈ ���Ϸ��� �����߰� 
							cMessageBox.ShowMessage3(MESSAGE_RECVITEM,WingItemName[WingItemQuestIndex]);
						break;

					}
					memset(&sSinHelp[i],0,sizeof(sSINHELP));

				}

				switch(HelpBoxKindIndex){ 
					case SIN_HELP:
						if(sSinHelp[i].KindFlag == SIN_HELP_KIND_NORMAL){
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
						}
					break;
					case SIN_CHANGEJOB_MESSAGE:

					break;
					case SIN_EVENT_SMASHTV:
						if(sSinHelp[i].KindFlag == SIN_HELP_KIND_SMASHTV){
							cMessageBox.ShowMessage3(MESSAGE_EVENT_SMASHTV,""); 
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
						}
					break;
					case SIN_MESSAGE_DEFAULT:
						HelpBoxKindIndex = 0;
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					break;
					case SIN_POST_BOX:
						HelpBoxKindIndex = 0;
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 

						/* ���� �ϴ��� ���� -_-;
						if(sSinHelp[i].KindFlag == SIN_HELP_KIND_POSTBOX){
							if(cInvenTory.CheckInvenEmpty(sPostbox_Item[0].CODE)){
								cMessageBox.ShowMessage3(MESSAGE_POSTBOX,sPostbox_Item[0].szName);
							}
							else{
								cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
							}
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
						}
						*/
					break;
					case SIN_SOD:
						HelpBoxKindIndex = 0;
						memset(&sSinHelp[i],0,sizeof(sSINHELP)); 

					break;

				}
/*----------------------------------------------------------------------------*
*					 �ʺ� Ʃ�丮�� ����Ʈ			
*-----------------------------------------------------------------------------*/

				//������ ���� üũ�� �ӽ� ������
				sITEM TempItem;
				TempItem.w = ITEMSIZE*2;
				TempItem.h = ITEMSIZE*4;
				TempItem.sItemInfo.Weight = 60;
				TempItem.Flag = 1;
				//�ǰ��� â������
				if(sSinHelp[i].KindFlag == HAQUEST_CODE_ELEMENTARY_A){
					if(chaQuest.sHaQuestElementary[Quest_A].State == 0)
						cMessageBox.ShowMessage3(MESSAGE_QUEST_ElementaryA,""); 
				
					//����Ʈ �Ϸ�
					if(chaQuest.sHaQuestElementary[Quest_A].State == 100){
						//���� üũ
						if(sinChar->Weight[0]+TempItem.sItemInfo.Weight > sinChar->Weight[1]){
							cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
							chaQuest.sHaQuestElementary[Quest_A].State = 2;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));							
							break;

						}
						//�������ֱ� 
						if(cInvenTory.CheckSetEmptyArea(&TempItem)){
							SendQuestCommandToServer( HAQUEST_CODE_ELEMENTARY_A ,2, 0, 0 );
							haElementaryQuestLog |= QUESTBIT_ELEMENTARY_A; //�ڵ带 �������ش�
							sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
							chaQuest.sHaQuestElementary[Quest_A].State = 100;
							chaQuest.sHaQuestElementary[Quest_A].BackUpCode = chaQuest.sHaQuestElementary[Quest_A].CODE;
							chaQuest.sHaQuestElementary[Quest_A].CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
							EndQuest_Code(chaQuest.sHaQuestElementary[Quest_A].BackUpCode);
							SaveGameData();
		
						}
						else{
							cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
							chaQuest.sHaQuestElementary[Quest_A].State = 2;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));
							break;
						}
						
					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP));
					break;

				}
				//�η��� ��� 
				if(sSinHelp[i].KindFlag == HAQUEST_CODE_ELEMENTARY_B){
					if(chaQuest.sHaQuestElementary[Quest_B].State == 0)
							cMessageBox.ShowMessage3(MESSAGE_QUEST_ElementaryB,""); 

					//����Ʈ �Ϸ�
					if(chaQuest.sHaQuestElementary[Quest_B].State == 100){
						//���� üũ
						if(sinChar->Weight[0]+TempItem.sItemInfo.Weight > sinChar->Weight[1]){
							cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
							chaQuest.sHaQuestElementary[Quest_B].State = 3;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));							
							break;

						}
						//�������ֱ� 
						if(cInvenTory.CheckSetEmptyArea(&TempItem)){
							cSkill.CancelContinueSkill(CHANGE_ELEMENTARY_QUEST);
							SendQuestCommandToServer( HAQUEST_CODE_ELEMENTARY_B ,2, 0, 0 );
							haElementaryQuestLog |= QUESTBIT_ELEMENTARY_B; //�ڵ带 �������ش�
							sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
							chaQuest.sHaQuestElementary[Quest_B].State = 100;
							chaQuest.sHaQuestElementary[Quest_B].BackUpCode = chaQuest.sHaQuestElementary[Quest_B].CODE;
							chaQuest.sHaQuestElementary[Quest_B].CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
							EndQuest_Code(chaQuest.sHaQuestElementary[Quest_B].BackUpCode);
							SaveGameData();
						}
						else{
							cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
							chaQuest.sHaQuestElementary[Quest_B].State = 3;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));
							break;
						}
						
					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP));
					break;
				}
				//���� ���
				if(sSinHelp[i].KindFlag == HAQUEST_CODE_ELEMENTARY_C){
					if(chaQuest.sHaQuestElementary[Quest_C].State == 0)
							cMessageBox.ShowMessage3(MESSAGE_QUEST_ElementaryC,""); 

					//����Ʈ �Ϸ�
					if(chaQuest.sHaQuestElementary[Quest_C].State == 100){
						//���� üũ
						if(sinChar->Weight[0]+TempItem.sItemInfo.Weight > sinChar->Weight[1]){
							cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
							chaQuest.sHaQuestElementary[Quest_C].State = 1;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));							
							break;

						}
						//�������ֱ� 
						if(cInvenTory.CheckSetEmptyArea(&TempItem)){
							DeleteQuestItem(sinQT1|sin14);
							SendQuestCommandToServer( HAQUEST_CODE_ELEMENTARY_C ,2, 10, 0 );
							haElementaryQuestLog |= QUESTBIT_ELEMENTARY_C; //�ڵ带 �������ش�
							sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
							chaQuest.sHaQuestElementary[Quest_C].State = 100;
							chaQuest.sHaQuestElementary[Quest_C].BackUpCode = chaQuest.sHaQuestElementary[Quest_C].CODE;
							chaQuest.sHaQuestElementary[Quest_C].CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
							EndQuest_Code(chaQuest.sHaQuestElementary[Quest_C].BackUpCode);
							SaveGameData();
						}
						else{
							cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
							chaQuest.sHaQuestElementary[Quest_C].State = 1;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));
							break;
						}
					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP));
					break;
				}
				//��� ����
				if(sSinHelp[i].KindFlag == HAQUEST_CODE_ELEMENTARY_D){
					if(chaQuest.sHaQuestElementary[Quest_D].State == 0)
							cMessageBox.ShowMessage3(MESSAGE_QUEST_ElementaryD,""); 
					//����Ʈ �Ϸ�
					if(chaQuest.sHaQuestElementary[Quest_D].State == 100){
						//���� üũ
						if(sinChar->Weight[0]+TempItem.sItemInfo.Weight > sinChar->Weight[1]){
							cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
							chaQuest.sHaQuestElementary[Quest_D].State = 3;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));							
							break;

						}
						//�������ֱ� 
						if(cInvenTory.CheckSetEmptyArea(&TempItem)){
							SendQuestCommandToServer( HAQUEST_CODE_ELEMENTARY_D ,2, 0, 0 );
							cSkill.CancelContinueSkill(CHANGE_ELEMENTARY_QUEST);
							haElementaryQuestLog |= QUESTBIT_ELEMENTARY_D; //�ڵ带 �������ش�
							sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
							chaQuest.sHaQuestElementary[Quest_D].State = 100;
							chaQuest.sHaQuestElementary[Quest_D].BackUpCode = chaQuest.sHaQuestElementary[Quest_D].CODE;
							chaQuest.sHaQuestElementary[Quest_D].CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
							EndQuest_Code(chaQuest.sHaQuestElementary[Quest_D].BackUpCode);
							SaveGameData();
						}
						else{
							cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
							chaQuest.sHaQuestElementary[Quest_D].State = 3;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));
							break;
						}
					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP));
					break;
				}
				//�𵥵��� ����
				if(sSinHelp[i].KindFlag == HAQUEST_CODE_ELEMENTARY_E){
					if(chaQuest.sHaQuestElementary[Quest_E].State == 0)
							cMessageBox.ShowMessage3(MESSAGE_QUEST_ElementaryE,""); 
					//����Ʈ �Ϸ�
					if(chaQuest.sHaQuestElementary[Quest_E].State == 100){
						//���� üũ
						if(sinChar->Weight[0]+TempItem.sItemInfo.Weight > sinChar->Weight[1]){
							cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
							chaQuest.sHaQuestElementary[Quest_E].State = 3;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));							
							break;

						}
						//�������ֱ� 
						if(cInvenTory.CheckSetEmptyArea(&TempItem)){	
							cSkill.CancelContinueSkill(CHANGE_ELEMENTARY_QUEST);
							SendQuestCommandToServer( HAQUEST_CODE_ELEMENTARY_E ,2, 0, 0 );
							haElementaryQuestLog |= QUESTBIT_ELEMENTARY_E; //�ڵ带 �������ش�
							sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
							chaQuest.sHaQuestElementary[Quest_E].State = 100;
							chaQuest.sHaQuestElementary[Quest_E].BackUpCode = chaQuest.sHaQuestElementary[Quest_E].CODE;
							chaQuest.sHaQuestElementary[Quest_E].CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
							EndQuest_Code(chaQuest.sHaQuestElementary[Quest_E].BackUpCode);
							SaveGameData();
						}
						else{
							cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
							chaQuest.sHaQuestElementary[Quest_E].State = 3;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));
							break;
						}

					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP));
					break;
				}
				//�ܴ����� ����
				if(sSinHelp[i].KindFlag == HAQUEST_CODE_ELEMENTARY_F){
					if(chaQuest.sHaQuestElementary[Quest_F].State == 0)
							cMessageBox.ShowMessage3(MESSAGE_QUEST_ElementaryF,""); 
					//����Ʈ �Ϸ�
					if(chaQuest.sHaQuestElementary[Quest_F].State == 100){
						//���� üũ
						if(sinChar->Weight[0]+TempItem.sItemInfo.Weight > sinChar->Weight[1]){
							cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
							chaQuest.sHaQuestElementary[Quest_F].State = 3;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));							
							break;

						}
						//�������ֱ� 
						if(cInvenTory.CheckSetEmptyArea(&TempItem)){
							cSkill.CancelContinueSkill(CHANGE_ELEMENTARY_QUEST);
							SendQuestCommandToServer( HAQUEST_CODE_ELEMENTARY_F ,2, 20, 0 );
							haElementaryQuestLog |= QUESTBIT_ELEMENTARY_F; //�ڵ带 �������ش�
							sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
							chaQuest.sHaQuestElementary[Quest_F].State = 100;
							chaQuest.sHaQuestElementary[Quest_F].BackUpCode = chaQuest.sHaQuestElementary[Quest_F].CODE;
							chaQuest.sHaQuestElementary[Quest_F].CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
							EndQuest_Code(chaQuest.sHaQuestElementary[Quest_F].BackUpCode);
							SaveGameData();
						}
						else{
							cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
							chaQuest.sHaQuestElementary[Quest_F].State = 3;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));
							break;
						}

					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP));
					break;
				}
				//�Ҿ���� ��ġ
				if(sSinHelp[i].KindFlag == HAQUEST_CODE_ELEMENTARY_G)
				{
					if(chaQuest.sHaQuestElementary[Quest_G].State == 0)
							cMessageBox.ShowMessage3(MESSAGE_QUEST_ElementaryG,""); 
					//����Ʈ �Ϸ�
					if(chaQuest.sHaQuestElementary[Quest_G].State == 100)
					{
						//���� üũ
						if(sinChar->Weight[0]+TempItem.sItemInfo.Weight > sinChar->Weight[1]){
							cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
							chaQuest.sHaQuestElementary[Quest_G].State = 2;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));
							break;

						}
						//�������ֱ� 
						if(cInvenTory.CheckSetEmptyArea(&TempItem)){
							SendQuestCommandToServer( HAQUEST_CODE_ELEMENTARY_G ,2, 0, 0 );
							haElementaryQuestLog |= QUESTBIT_ELEMENTARY_G; //�ڵ带 �������ش�
							sinPlaySound(SIN_SOUND_EAT_POTION2);//��� ����
							chaQuest.sHaQuestElementary[Quest_G].State = 100;
							chaQuest.sHaQuestElementary[Quest_G].BackUpCode = chaQuest.sHaQuestElementary[Quest_G].CODE;
							chaQuest.sHaQuestElementary[Quest_G].CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
							EndQuest_Code(chaQuest.sHaQuestElementary[Quest_G].BackUpCode);
							SaveGameData();
						}
						else{
							cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
							chaQuest.sHaQuestElementary[Quest_G].State = 2;
							HelpBoxKindIndex = 0;
							memset(&sSinHelp[i],0,sizeof(sSINHELP));
							break;
						}
					}
					HelpBoxKindIndex = 0;
					memset(&sSinHelp[i],0,sizeof(sSINHELP));
					break;
				}
				//��ư�ٿ� �̺�Ʈ ���� �ϴ� �� �����̴� 
				HelpBoxKindIndex = 0;
				memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
				
				
							
			}
		}
		break;
	}
}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cSINHELP::LButtonUp(int x , int y)
{
	for(int i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){   
		if(sSinHelp[i].KindFlag){
			if(sSinHelp[i].ScrollMouseDownFlag){
					sSinHelp[i].ScrollMouseDownFlag = 0;

			}
		}
	}
}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/	
void cSINHELP::RButtonDown(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cSINHELP::RButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	
int KeyFlag = 0;
void cSINHELP::KeyDown()
{
	
	if(smConfig.DebugMode){ 
		//if(VRKeyBuff['8']){
		if(sinGetKeyClick('6')){
			memset(&sSinHelp,0,sizeof(sSINHELP)*10);

		}
		if(sinGetKeyClick('A')){ 
			

		}
		if(sinGetKeyClick('7')){    
			for(int i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){   
				if(sSinHelp[i].KindFlag){
					sSinHelp[i].ScrollButton.y++;

				}
			}

		//	sinReadFileData("Image\\sinImage\\Help\\Test.txt");
		//	sinShowHelp(SIN_HELP_KIND_NORMAL,150,150,6,6, RGBA_MAKE(128,15,15,125)); 
 
		} 
		/*
		if(sinGetKeyClick('8')){  
			//sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1);
			//cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,sinJobList->szName2);
			/*
			if(!KeyFlag){
				TonyGetFileData();
				KeyFlag = 1;
			}
			else{
				memset(&sTonyTest,0,sizeof(sTONY_TEST_IMAGE)*10);
				KeyFlag = 0;

			}
			*/
		//	for(int i=0 ; i < 64 ; i++){
		//		sinReadFileDataDivide(&szHelpDocBuff[i][0],128);

		//	}
		//	sinShowHelp(SIN_HELP_KIND_NORMAL,150,150,6,5, RGBA_MAKE(0,128,15,125)); 

		//}
		/*
		if(sinGetKeyClick('5')){          
			//sinShowHelp(SIN_HELP_KIND_NORMAL,175,93,7,5, RGBA_MAKE(0,15,128,125)); 
		}
		*/
	}
	sinGetKeyScrollDown(); // ���� ��ũ��
}

/*----------------------------------------------------------------------------*
*							TEXT �� ����ش�
*-----------------------------------------------------------------------------*/	
void sinCopyText(char to[] , char from[])
{
	int i = 0;
	int j = 0;
	int cnt = lstrlen(from);
	while( cnt > j){
		if(from[j] == '$'){ 
		}
		else{
			to[i] = from[j];
			i++;
		}
		j++;
	}
}

void cSINHELP::DrawText()
{
	HDC	hdc;
	int sinTest5 = 0;
	int Len = 0;
	int cnt = 0;
	char strBuff[1024];
	char strBuff2[1024]; 
	renderDevice.lpDDSBack->GetDC(&hdc);

	SelectObject( hdc, sinFont);
    SetBkMode( hdc, TRANSPARENT ); 
	SetTextColor( hdc, RGB(255,255,255) ); 

	int i,j, tHeight,TempLine ,TempLine2,TempLine3,TempLine4;
	for(i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){         
		if(sSinHelp[i].KindFlag){      
			tHeight = (sSinHelp[i].SizeH-2)*32 / 15;    
			TempLine = sSinHelp[i].ScrollButton.y   - (sSinHelp[i].PosiY+32+7) ;

			TempLine2 = sSinHelp[i].ScrollMAXPosiY_DOWN - sSinHelp[i].ScrollMAXPosiY_UP;
			TempLine3 = (int)(((float)TempLine/(float)TempLine2)*100.0f);
			TempLine4 = (int)((float)(sSinHelp[i].LineCnt-((float)tHeight-1.0f))/100.0f *(float)TempLine3);
			if(sSinHelp[i].LineCnt > 21 ) //(���߿� ���ľ���)
				TempLine4 -= 7; //��¦������ -0-
			if(TempLine4 <0 )TempLine4 = 0;

			//��ũ�� �Ÿ� ī��Ʈ ����
			ScrollLineCount = TempLine4;
			//TempLine4 -= 16;
			//if(

			/*
			SelectObject( hdc, sinFont);
			SetBkMode( hdc, TRANSPARENT ); 
			SetTextColor( hdc, RGB(255,255,180) ); 
			*/
			//haGoon
			haResearch.Number = 0;
			//�ڷ���Ʈ �ھ�
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL){
				for(j=0 ; j < tHeight-1 ; j++){
					if(sSinHelp[i].szDocBuff[j+TempLine4]){
						wsprintf(strBuff,"%s",sSinHelp[i].szDocBuff[j+TempLine4]);
						cnt = lstrlen(strBuff);
						for(int p = 0; p < cnt; p++){
							SelectObject( hdc, sinFont);
							if(strBuff[p] == '.'){
								if(atoi(&strBuff[p-1])==chaPremiumitem.TelePortCore.FieldIndex[0]-1 && chaPremiumitem.TelePortCore.FieldIndex[0]!=0){
									SetTextColor( hdc, RGB(255,100,45) );
									strcpy(chaPremiumitem.TelePortCore.FieldName,&strBuff[p+1]);							
								}
								else
									SetTextColor( hdc, RGB(240,240,240) );
							}
							 
						}
						dsTextLineOut(hdc,sSinHelp[i].PosiX+30 ,sSinHelp[i].PosiY+66+(15*j),strBuff,lstrlen(strBuff)); 
					}
					else{
						sinTest5 = 1;
					}									
				}
			}
			//100LV ����Ʈ
			else if(sSinHelp[i].KindFlag == HAQUEST_CODE_FURYOFPHANTOM){  
				for(j=0 ; j < tHeight-1 ; j++){
					if(sSinHelp[i].szDocBuff[j+TempLine4]){
						wsprintf(strBuff,"%s",sSinHelp[i].szDocBuff[j+TempLine4]);
						cnt = lstrlen(strBuff);
						for(int p = 0; p < cnt; p++){
							if(strBuff[p] == '*'){
								if(atoi(&strBuff[p+1])==chaQuest.iFuryQuestIndex[0])
									SetTextColor( hdc, RGB(255,100,45) );
								else
									SetTextColor( hdc, RGB(240,240,240) );
							}
						}
						if(chaQuest.iFuryQuestIndex[1]){
							if(haSearchString(strBuff,'[')){
								SelectObject( hdc, sinBoldFont);
								SetTextColor( hdc,  RGB(128,255,128)  ); 						
							}
							else{
								SelectObject( hdc, sinFont);
								SetTextColor( hdc, RGB(240,240,240) ); 						
							}
						}
						
						dsTextLineOut(hdc,sSinHelp[i].PosiX+30 ,sSinHelp[i].PosiY+66+(15*j),strBuff,lstrlen(strBuff)); 
					    
					}
					else{
						sinTest5 = 1;
					}
				}
			}
			//��������
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_RESEARCH ){ //������ �������� ���ڻ��� �޸����ش� 
				for(j=0 ; j < tHeight-1 ; j++){
					if(sSinHelp[i].szDocBuff[j+TempLine4]){
						haResearch.Result[0]=TempLine4;
						wsprintf(strBuff,"%s",sSinHelp[i].szDocBuff[j+TempLine4]);
						cnt = lstrlen(strBuff);
						for(int p = 0; p < cnt; p++){
							if(strBuff[p] == '>'){
								haResearch.Number++;
							}
						}
						if(j>1){
							if(haResearch.Result[1]-TempLine4==j)
								SetTextColor( hdc, RGB(255,100,45) );
							else if(haResearch.Result[2]-TempLine4==j)
								SetTextColor( hdc, RGB(255,100,45) );
							else if(haResearch.Result[3]-TempLine4==j)
								SetTextColor( hdc, RGB(255,100,45) );
							else
								SetTextColor( hdc, RGB(255,255,255) );
						}
						dsTextLineOut(hdc,sSinHelp[i].PosiX+30 ,sSinHelp[i].PosiY+66+(15*j),strBuff,lstrlen(strBuff)); 
					    
					}
					else{
						sinTest5 = 1;
					}
				}
			}
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT ){ //�ڷ���Ʈ�ϰ�� ���ڻ��� �޸����ش� 
				for(j=0 ; j < tHeight-1 ; j++){
					if(sSinHelp[i].szDocBuff[j+TempLine4]){
						wsprintf(strBuff,"%s",sSinHelp[i].szDocBuff[j+TempLine4]);
						if( sinTeleportIndex+10 == j){
							SelectObject( hdc, sinFont);
							SetBkMode( hdc, TRANSPARENT ); 
							SetTextColor( hdc, RGB(255,171,45) ); 
						}
						else{
							SelectObject( hdc, sinFont);
							SetBkMode( hdc, TRANSPARENT ); 
							SetTextColor( hdc, RGB(255,255,255) ); 

						}
						if(sinChar->Level < FieldLimitLevel_Table[12] && j == 14){  
							SelectObject( hdc, sinFont);
							SetBkMode( hdc, TRANSPARENT ); 
							SetTextColor( hdc, RGB(128,128,128) ); 

						}
						dsTextLineOut(hdc,sSinHelp[i].PosiX+30 ,sSinHelp[i].PosiY+66+(15*j),strBuff,lstrlen(strBuff)); 
					}
					else{
						sinTest5 = 1;
					}
				}
			}
			// pluto ���� ���� ����
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_MILTER ){ //�ڷ���Ʈ�ϰ�� ���ڻ��� �޸����ش� 
				for(j=0 ; j < tHeight-1 ; j++){
					if(sSinHelp[i].szDocBuff[j+TempLine4]){
						wsprintf(strBuff,"%s",sSinHelp[i].szDocBuff[j+TempLine4]);
						if( sinTeleportIndex+13 == j){
							SelectObject( hdc, sinFont);
							SetBkMode( hdc, TRANSPARENT ); 
							SetTextColor( hdc, RGB(255,171,45) ); 
						}
						else{
							SelectObject( hdc, sinFont);
							SetBkMode( hdc, TRANSPARENT ); 
							SetTextColor( hdc, RGB(255,255,255) ); 

						}
						if(sinChar->Level < FieldLimitLevel_Table[12] && j == 14){  
							SelectObject( hdc, sinFont);
							SetBkMode( hdc, TRANSPARENT ); 
							SetTextColor( hdc, RGB(128,128,128) ); 

						}
						dsTextLineOut(hdc,sSinHelp[i].PosiX+30 ,sSinHelp[i].PosiY+66+(15*j),strBuff,lstrlen(strBuff)); 
					}
					else{
						sinTest5 = 1;
					}
				}
			}
			else  if(sSinHelp[i].KindFlag == SIN_HELP_KIND_C_TELEPORT){
				for(j=0 ; j < tHeight-1 ; j++){
					if(sSinHelp[i].szDocBuff[j+TempLine4]){
						wsprintf(strBuff,"%s",sSinHelp[i].szDocBuff[j+TempLine4]);
						if( haC_TeleportIndex+13 == j){
							SelectObject( hdc, sinFont);
							SetBkMode( hdc, TRANSPARENT ); 
							SetTextColor( hdc, RGB(255,171,45) ); 
						}
						else{
							SelectObject( hdc, sinFont);
							SetBkMode( hdc, TRANSPARENT ); 
							SetTextColor( hdc, RGB(255,255,255) ); 

						}
						dsTextLineOut(hdc,sSinHelp[i].PosiX+30 ,sSinHelp[i].PosiY+66+(15*j),strBuff,lstrlen(strBuff)); 
					}
					else{
						sinTest5 = 1;
					}
				}
			}
			else{
				for(j=0 ; j < tHeight-1 ; j++){  
					if(sSinHelp[i].szDocBuff[j+TempLine4]){
						wsprintf(strBuff,"%s",sSinHelp[i].szDocBuff[j+TempLine4]); 
						//���� �̸��� �ֱ����� ����
						if(sinQuest_Level90_2.CODE){
							cnt = lstrlen(strBuff);
							for(int p = 0; p < cnt; p++){
								if(strBuff[p] == '$' && strBuff[p+1] == '$'){
									memset(strBuff2,0,sizeof(strBuff2));
									sinCopyText(strBuff2,strBuff);
									wsprintf(strBuff,strBuff2,sinLevelQuest90_2Mon[sinQuest_Level90_2.Kind-1]);
									break;
								}
							}
						}
					
						dsTextLineOut(hdc,sSinHelp[i].PosiX+30 ,sSinHelp[i].PosiY+66+(15*j),strBuff,lstrlen(strBuff)); 
					}
					else{
						sinTest5 = 1;
					}
				}
			}

			SelectObject( hdc, sinBoldFont);
			SetBkMode( hdc, TRANSPARENT ); 
			SetTextColor( hdc, RGB(255,200,100) ); 
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_NORMAL){ //����            
				/*
				SelectObject( hdc, sinFont);
				SetTextColor( hdc, RGB(255,255,255) ); 
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+29 ,sSinHelp[i].TitlePosi.y+10,HelpTitle8,lstrlen(HelpTitle8)); 
				*/
				SelectObject( hdc, sinBoldFont);
				SetTextColor( hdc, RGB(255,255,240) ); 
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,HelpTitle8,lstrlen(HelpTitle8)); 

			}
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_SOD){
				for(int h = 0 ; h < 10 ; h++){      
					if(sSod_Rank[h].Flag){						
						wsprintf(strBuff,"%d",h+1); 
						SelectObject( hdc, sinBoldFont);
						SetBkMode( hdc, TRANSPARENT ); 
						SetTextColor( hdc, RGB(255,200,100) ); 

						if(h==9){
							dsTextLineOut(hdc,sSinHelp[i].PosiX+20 ,sSinHelp[i].PosiY+64+(h*24),strBuff,lstrlen(strBuff)); 
						}
						else{
							dsTextLineOut(hdc,sSinHelp[i].PosiX+25 ,sSinHelp[i].PosiY+64+(h*24),strBuff,lstrlen(strBuff)); 
						}
						SelectObject( hdc, sinFont);
						SetBkMode( hdc, TRANSPARENT ); 
						SetTextColor( hdc, RGB(222,231,255) );
						Len = lstrlen(sSod_Rank[h].Name);
						dsTextLineOut(hdc,sSinHelp[i].PosiX+25+(116-((Len*6)/2)) ,sSinHelp[i].PosiY+64+(h*24),sSod_Rank[h].Name,lstrlen(sSod_Rank[h].Name));

						SelectObject( hdc, sinFont);
						SetBkMode( hdc, TRANSPARENT ); 
						SetTextColor( hdc, RGB(128,255,128) ); 
						wsprintf(strBuff,"%d",sSod_Rank[h].Score);
						Len = lstrlen(strBuff);
						dsTextLineOut(hdc,sSinHelp[i].PosiX+25+97+(116-((Len*6)/2)) ,sSinHelp[i].PosiY+64+(h*24),strBuff,lstrlen(strBuff));


						SelectObject( hdc, sinFont);
						SetBkMode( hdc, TRANSPARENT ); 
						SetTextColor( hdc, RGB(110,165,250) ); 
						wsprintf(strBuff,"%d",sSod_Rank[h].KillNum);
						Len = lstrlen(strBuff);
						dsTextLineOut(hdc,sSinHelp[i].PosiX+25+97+70+(116-((Len*6)/2)) ,sSinHelp[i].PosiY+64+(h*24),strBuff,lstrlen(strBuff));
					}					
				}
				//sSod_Rank

			}
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_C_TELEPORT){
				SelectObject( hdc, sinBoldFont);
				SetTextColor( hdc, RGB(255,255,240) ); 
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,HaWarpGateName[0],lstrlen(HaWarpGateName[0]));
			}
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_WARPGATE){//HaWarpGateName
				SelectObject( hdc, sinBoldFont);
				SetTextColor( hdc, RGB(255,255,240) ); 
				if(sSinWarpGate.GateSelect >-1)
					dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,WarpGateName[sSinWarpGate.GateSelect+1],lstrlen(WarpGateName[sSinWarpGate.GateSelect+1])); 
				else 
					dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,WarpGateName[0],lstrlen(WarpGateName[0])); 
			}
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_INITPOINT){
				SelectObject( hdc, sinBoldFont);
				SetTextColor( hdc, RGB(255,255,240) ); 
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,sinWarningName5,lstrlen(sinWarningName5)); 

			}
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_BABEL){
				SelectObject( hdc, sinBoldFont);
				SetTextColor( hdc, RGB(255,255,240) ); 
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,QuestTitle8,lstrlen(QuestTitle8)); 

			}
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_RESEARCH){
				SelectObject( hdc, sinBoldFont);
				SetTextColor( hdc, RGB(255,255,240) ); 
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,ResearchTitle8,lstrlen(ResearchTitle8)); 
			}
			else if(sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL){
				SelectObject( hdc, sinBoldFont);
				SetTextColor( hdc, RGB(255,255,240) ); 
#ifdef _LANGUAGE_ARGENTINA
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+17 ,sSinHelp[i].TitlePosi.y+4,TeleportTitle8,lstrlen(TeleportTitle8)); 
#else
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,TeleportTitle8,lstrlen(TeleportTitle8)); 
#endif
			}
			else if( sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_MILTER )	// pluto ���� ���� ����
			{
				SelectObject( hdc, sinBoldFont);
				SetTextColor( hdc, RGB(255,255,240) ); 
#ifdef _LANGUAGE_ARGENTINA
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+17 ,sSinHelp[i].TitlePosi.y+4,TeleportTitle8,lstrlen(TeleportTitle8)); 	
#else
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,TeleportTitle8,lstrlen(TeleportTitle8)); 
#endif
			}
			else{//����Ʈ 
				SelectObject( hdc, sinBoldFont);
				SetBkMode( hdc, TRANSPARENT ); 
				SetTextColor( hdc, RGB(255,255,240) ); 
				dsTextLineOut(hdc,sSinHelp[i].TitlePosi.x+35 ,sSinHelp[i].TitlePosi.y+9,QuestTitle8,lstrlen(QuestTitle8));
			}
			
		}
	}
	renderDevice.lpDDSBack->ReleaseDC(hdc);
}

/*----------------------------------------------------------------------------*
*							�Źٶ� ����̸� �����ش� 
*-----------------------------------------------------------------------------*/	
int  cSINHELP::sinShowHelp(int Kind , int PosiX , int PosiY , int SizeW , int SizeH , DWORD BackColor,char *File)
{
	// pluto
	int flag = 0;
	//if( Kind != SIN_HELP_KIND_TELEPORT_SCROLL ) return FALSE;	
	if(isDrawClanMenu)return FALSE; //Ŭ���޴��� �������� �����Ѵ�

	if( Kind != SIN_HELP_KIND_TELEPORT_SCROLL && cSinHelp.sinGetHelpShowFlagNum() )	// ���� �ڷ���Ʈ �ھ� ������ �־���
	{
		flag = cSinHelp.sinGetHelpShowFlagNum();
		// 40 <- ǻ�� ����Ʈ ���� ��, �� ��ư
		if( flag != 40 )
		{
			return FALSE;
		}
	}
		
	//dsGetDrawOffset( &PosiX , &PosiY );


	int i = 0 , j = 0 , k = 0;

	//���� ���� ������ ���ִ��� �˻��ؼ� �����Ѵ� 
	for(i = 0; i < SIN_MAX_HELP_NUMBER ; i++){   
		//if(sSinHelp[i].KindFlag && sSinHelp[i].KindFlag == Kind)
		if(sSinHelp[i].KindFlag){
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_WARPGATE ){ 
				if(sSinHelp[i].Code != sSinWarpGate.GateSelect){ //�ߺ����� ��� ������� �����Ѵ�
					memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
					sSinHelp[i].Code = sSinWarpGate.GateSelect;
					break;
				}
				else{
					return FALSE;
				}
			}
			else if(sSinHelp[i].KindFlag == HAQUEST_CODE_FURYOFPHANTOM || sSinHelp[i].KindFlag == SIN_HELP_KIND_TELEPORT_SCROLL ){				
				memset(&sSinHelp[i],0,sizeof(sSINHELP)); 
				break;
			}
			else{
				chaPremiumitem.Using_ItemKind = 0;
				return FALSE;
			}
		}
	}

	
	
   //<ha>������ �������� ��������Ʈ
  
   int  Wapecnt=0;
   int  WapeGateMoney[6]={100,300,500,1000,2000, 4000};//�� ��������Ʈ  ����
   int  TeleportMoney[4]={100,100,500,1000};     //�ڷ���Ʈ ����
   char StrTemp[128];
   
	
   //������ �����Ѵ� 
	for(i = 0; i < SIN_MAX_HELP_NUMBER ; i++){  
		if(!sSinHelp[i].KindFlag){ 
			memset(szHelpDocBuff,0,(char)100*128); 
			
			//���� �ؽ�Ʈ�� �д´� 
			//sinReadFileData("Image\\sinImage\\Help\\Test.txt"); 
			sinReadFileData(File);

			//�о���� ������ ���κ��� ���ۿ� �����Ѵ� 
			sinReadFileDataDivide(&szHelpDocBuff[0][0],128,SizeW*8);

		
			for(int t=0 ; t < 80 ; t++){
				if(Kind == SIN_HELP_KIND_WARPGATE){
					if(haSearchString(&szHelpDocBuff[t][0],'%')){
						//--------------------------------------------------------------------------//
						#ifdef HASIEGE_MODE
							//<ha>������ �������� ��������Ʈ ����ǥ��
							wsprintf(StrTemp,&szHelpDocBuff[t][0],cShop.haShopItemPrice(WapeGateMoney[Wapecnt]));
						#else
							wsprintf(StrTemp,&szHelpDocBuff[t][0],WapeGateMoney[Wapecnt]);
						#endif
						//---------------------------------------------------------------------------//
						lstrcpy(&szHelpDocBuff[t][0],StrTemp);
						Wapecnt++;
					}
				}
				//�ڷ���Ʈ
				if(Kind == SIN_HELP_KIND_TELEPORT){
					if(haSearchString(&szHelpDocBuff[t][0],'%')){
						//--------------------------------------------------------------------------//
						#ifdef HASIEGE_MODE
							//<ha>������ �������뿡 �ڷ���Ʈ ���� ǥ��
							wsprintf(StrTemp,&szHelpDocBuff[t][0],cShop.haShopItemPrice(TeleportMoney[Wapecnt]));
						#else
							wsprintf(StrTemp,&szHelpDocBuff[t][0],TeleportMoney[Wapecnt]);
						#endif
						//---------------------------------------------------------------------------//
						lstrcpy(&szHelpDocBuff[t][0],StrTemp);
						Wapecnt++;
					}
				}
				//���� ĳ�� �ڷ���Ʈ-------------------------------------------------------
				if(Kind == SIN_HELP_KIND_C_TELEPORT){
					if(haSearchString(&szHelpDocBuff[t][0],'%')){
						wsprintf(StrTemp,&szHelpDocBuff[t][0],haC_TeleportMoney);
						lstrcpy(&szHelpDocBuff[t][0],StrTemp);
						Wapecnt++;
					}
				}
				//--------------------------------------------------------------------------

				
				//����� ��Ʈ�� ���� (����������)
				if(szHelpDocBuff[t][0] || szHelpDocBuff[t+1][0] || szHelpDocBuff[t+2][0] || szHelpDocBuff[t+3][0]){
					sSinHelp[i].szDocBuff[t] = &szHelpDocBuff[t][0];
					sSinHelp[i].LineCnt++;
				}

			}
			sSinHelp[i].LineCnt++; //��ĭ�� �÷��ش� (�ӽù���)

			sSinHelp[i].KindFlag = Kind;    
			sSinHelp[i].PosiX = PosiX;  
			sSinHelp[i].PosiY = PosiY;
			sSinHelp[i].SizeW = SizeW;  
			sSinHelp[i].SizeH = SizeH;
			sSinHelp[i].BackColor = BackColor;
 
			sSinHelp[i].ScrollButton.x = (sSinHelp[i].PosiX+(sSinHelp[i].SizeW-1)*32+80-10);           
			//��¦ �����غ���  
			if(sSinHelp[i].SizeH <= 10 ){
				TempScrollPosiY = 53;
				sSinHelp[i].ScrollButton.y = sSinHelp[i].PosiY+TempScrollPosiY;

			}
			else{
				TempScrollPosiY = 64;
				sSinHelp[i].ScrollButton.y = sSinHelp[i].PosiY+64;
			}
			//sSinHelp[i].ScrollButton.y = (sSinHelp[i].PosiY+(sSinHelp[i].SizeH*5)); 

			sSinHelp[i].ScrollMAXPosiY_UP =   (sSinHelp[i].PosiY+TempScrollPosiY+7);
			sSinHelp[i].ScrollMAXPosiY_DOWN = (sSinHelp[i].PosiY+TempScrollPosiY+7 + 236);   

			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_NORMAL){    
				sSinHelp[i].TitlePosi.x = (sSinHelp[i].PosiX + ((sSinHelp[i].SizeW*32)+64)/2 - (152/2));
				sSinHelp[i].TitlePosi.y = sSinHelp[i].PosiY+14;
				sSinHelp[i].ButtonPosi.x = (sSinHelp[i].PosiX + ((sSinHelp[i].SizeW*32)+64)/2 - (48/2));
				sSinHelp[i].ButtonPosi.y = (sSinHelp[i].PosiY + (sSinHelp[i].SizeH)*32)-2;
			}
			else{
				sSinHelp[i].TitlePosi.x = (sSinHelp[i].PosiX + ((sSinHelp[i].SizeW*32)+64)/2 - (152/2));
				sSinHelp[i].TitlePosi.y = sSinHelp[i].PosiY+14;
				sSinHelp[i].ButtonPosi.x = (sSinHelp[i].PosiX + ((sSinHelp[i].SizeW*32)+64)/2 - (48/2));
				sSinHelp[i].ButtonPosi.y = (sSinHelp[i].PosiY + (sSinHelp[i].SizeH)*32)-2;

			}

			///////���� (��~~ ȯ���ϰڳ�)
			if(sSinHelp[i].LineCnt  > 16)
				sSinHelp[i].ScrollFlag = 1; //��ũ���� �־��ش� 
			
			//���� �ڽ� ���� �Ҵ� 
			//sSinHelp[i].sHelp_Box = new sSINHELP_BOX[SizeW][SizeH];
			for(j = 0 ; j < SizeW ; j++){   
				for(k = 0 ; k < SizeH ; k++){
					sSinHelp[i].sHelp_Box[j][k] = new sSINHELP_BOX;

					///////���� ��� 
					if(j == 0 && k == 0){  
						sSinHelp[i].sHelp_Box[j][k]->Mat = MatHelpBoxImage[0]; 
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY;
						sSinHelp[i].sHelp_Box[j][k]->w = 64;
						sSinHelp[i].sHelp_Box[j][k]->h = 32;

						/*
						sSinHelp[i].sHelp_Box[j][k]->Mat = cSinHelp.MatHelpCorner; 
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY;
						sSinHelp[i].sHelp_Box[j][k]->surW = 0;
						sSinHelp[i].sHelp_Box[j][k]->surH = 0;
						*/

					}
					
					///////���� ��� 
					if(j == SizeW-1 && k == 0){ 
						sSinHelp[i].sHelp_Box[j][k]->Mat = MatHelpBoxImage[2];
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+((SizeW-1)*32)+32;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY;
						sSinHelp[i].sHelp_Box[j][k]->w = 64;
						sSinHelp[i].sHelp_Box[j][k]->h = 32;

						/*
						sSinHelp[i].sHelp_Box[j][k]->Mat = cSinHelp.MatHelpCorner;
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+((SizeW-1)*64);
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY;
						sSinHelp[i].sHelp_Box[j][k]->surW = 64;
						sSinHelp[i].sHelp_Box[j][k]->surH = 0;
						*/

					}
					///////���� �ϴ� 
					if(j == SizeW-1 && k == SizeH-1){
						sSinHelp[i].sHelp_Box[j][k]->Mat = MatHelpBoxImage[8];
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+((SizeW-1)*32)+32;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+((SizeH-1)*32);
						sSinHelp[i].sHelp_Box[j][k]->w = 64;
						sSinHelp[i].sHelp_Box[j][k]->h = 64;

						/*
						sSinHelp[i].sHelp_Box[j][k]->Mat = cSinHelp.MatHelpCorner;
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+((SizeW-1)*64);
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+((SizeH-1)*64);
						sSinHelp[i].sHelp_Box[j][k]->surW = 64;
						sSinHelp[i].sHelp_Box[j][k]->surH = 64;
						*/

					}
										
					///////���� �ϴ�
					if(j == 0 && k == SizeH-1){
						sSinHelp[i].sHelp_Box[j][k]->Mat = MatHelpBoxImage[6];
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+((SizeH-1)*32);
						sSinHelp[i].sHelp_Box[j][k]->w = 64;
						sSinHelp[i].sHelp_Box[j][k]->h = 64;


						/*
						sSinHelp[i].sHelp_Box[j][k]->Mat = cSinHelp.MatHelpCorner;
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+((SizeH-1)*64);
						sSinHelp[i].sHelp_Box[j][k]->surW = 0;
						sSinHelp[i].sHelp_Box[j][k]->surH = 64;
						*/

					}
					/////// ���߰� 
					if( j != 0 && k == 0 && j != SizeW-1){ 
						sSinHelp[i].sHelp_Box[j][k]->Mat = MatHelpBoxImage[1];
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+(j*32)+32;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY;
						sSinHelp[i].sHelp_Box[j][k]->w = 32;
						sSinHelp[i].sHelp_Box[j][k]->h = 32;

						/*
						sSinHelp[i].sHelp_Box[j][k]->Mat = cSinHelp.MatHelpLine;
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+(j*64);
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY;
						sSinHelp[i].sHelp_Box[j][k]->surW = 0;
						sSinHelp[i].sHelp_Box[j][k]->surH = 0;
						*/

					}

					/////// �������߰� 
					if( j == SizeW-1 && k != 0 &&  k != SizeH-1){  
						sSinHelp[i].sHelp_Box[j][k]->Mat = MatHelpBoxImage[5]; 
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+((SizeW-1)*32)+32;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+(k*32);
						sSinHelp[i].sHelp_Box[j][k]->w = 64;
						sSinHelp[i].sHelp_Box[j][k]->h = 32;

						/*
						sSinHelp[i].sHelp_Box[j][k]->Mat = cSinHelp.MatHelpLine; 
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+((SizeW-1)*64);
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+(k*64);
						sSinHelp[i].sHelp_Box[j][k]->surW = 64;
						sSinHelp[i].sHelp_Box[j][k]->surH = 0;
						*/

					}
					/////// �Ʒ����߰� 
					if( j != 0 && j != SizeW-1 && k == SizeH-1){   
						sSinHelp[i].sHelp_Box[j][k]->Mat = MatHelpBoxImage[7];
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+(j*32)+32;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+((SizeH-1)*32);
						sSinHelp[i].sHelp_Box[j][k]->w = 32;
						sSinHelp[i].sHelp_Box[j][k]->h = 64;

						/*
						sSinHelp[i].sHelp_Box[j][k]->Mat = cSinHelp.MatHelpLine;
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX+(j*64);
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+((SizeH-1)*64);
						sSinHelp[i].sHelp_Box[j][k]->surW = 64;
						sSinHelp[i].sHelp_Box[j][k]->surH = 64;
						*/
					}
					/////// �����߰� 
					if( j == 0 && k != SizeH-1 && k != 0){    
						sSinHelp[i].sHelp_Box[j][k]->Mat = MatHelpBoxImage[3];
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+(k*32);
						sSinHelp[i].sHelp_Box[j][k]->w = 64;
						sSinHelp[i].sHelp_Box[j][k]->h = 32;


						/*
						sSinHelp[i].sHelp_Box[j][k]->Mat = cSinHelp.MatHelpLine;
						sSinHelp[i].MatFlag[j][k] = 1;
						sSinHelp[i].sHelp_Box[j][k]->x = (float)PosiX;
						sSinHelp[i].sHelp_Box[j][k]->y = (float)PosiY+(k*64);
						sSinHelp[i].sHelp_Box[j][k]->surW = 0;
						sSinHelp[i].sHelp_Box[j][k]->surH = 64;
						*/

					}
					sSinHelp[i].sHelp_Box[j][k]->surW = 0;
					sSinHelp[i].sHelp_Box[j][k]->surH = 0;
//					sSinHelp[i].sHelp_Box[j][k]->w = 64;
//					sSinHelp[i].sHelp_Box[j][k]->h = 64;
					sSinHelp[i].sHelp_Box[j][k]->Transp = 255;
				}

			}
			break;
		}

	}
	
	return TRUE;
}
/*----------------------------------------------------------------------------*
*							  DrawTexture
*-----------------------------------------------------------------------------*/	
int sinDrawTexImage( int Mat, float x, float y, float w, float h, float surW, float surH, float tLeft, float tTop, float tRight, float tBottom, int Transp)
{
	
	D3DTLVERTEX tlVertex[4];
	int cnt;

	for(cnt=0;cnt<4;cnt++)
	{
		tlVertex[cnt].rhw = 1;
		tlVertex[cnt].color	= RGBA_MAKE( 255,255,255,Transp );
		tlVertex[cnt].specular  =  RGBA_MAKE( 0,0,0,0 );
		tlVertex[cnt].sz = 0;
	}
/*
	tlVertex[0].sx = x;  
	tlVertex[0].sy = y;
	tlVertex[0].tu = 0;
	tlVertex[0].tv = 0;

	tlVertex[1].sx = x+w;
	tlVertex[1].sy = y;
	tlVertex[1].tu = 0;
	tlVertex[1].tv = 1;

	tlVertex[2].sx = x;
	tlVertex[2].sy = y+h;
	tlVertex[2].tu = 1;
	tlVertex[2].tv = 0;

	tlVertex[3].sx = (x+w);
	tlVertex[3].sy = (y+h);
	tlVertex[3].tu = 1;
	tlVertex[3].tv = 1;
*/

	tlVertex[0].sx = x+surW; 
	tlVertex[0].sy = y+surH;
	tlVertex[0].tu = 0;
	tlVertex[0].tv = 0;

	tlVertex[1].sx = x+surH;
	tlVertex[1].sy = (y+h)-surW;
	tlVertex[1].tu = 0;
	tlVertex[1].tv = 0.996f;

	tlVertex[2].sx = (x+w)-surH;
	tlVertex[2].sy = y+surW;
	tlVertex[2].tu = 0.996f;
	tlVertex[2].tv = 0;

	tlVertex[3].sx = (x+w)-surW;
	tlVertex[3].sy = (y+h)-surH;
	tlVertex[3].tu = 0.996f;  
	tlVertex[3].tv = 0.996f;


	// Turn on Z-buffering (�̰Ͷ����� �̹����� �ȳ��Ե� ������ �𸥴� ��������)
	//lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, FALSE );


	// null out the texture handle
	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, 0);

	// turn on dithering
	renderDevice.SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE);

	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_WRAP);
	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE);

	// D3DFILTER_LINEAR
	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_LINEAR);
	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEAR);

	renderDevice.SetRenderState(D3DRENDERSTATE_SPECULARENABLE, TRUE);
	renderDevice.SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);

	renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE);

	renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);

	smRender.AlphaTestDepth = 60; 


	renderDevice.SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_POINT);
	renderDevice.SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_POINT);

	//renderDevice.SetRenderState( D3DRENDERSTATE_TEXTUREMIN, D3DTFN_POINT );
	//renderDevice.SetRenderState( D3DRENDERSTATE_TEXTUREMAG, D3DTFN_POINT );
	



	if(smMaterial[Mat].smTexture[0] != NULL) 
	{
		if(smMaterial[Mat].smTexture[0]->lpD3DTexture != NULL)
		{
			if (FAILED(renderDevice.SetTexture(0, smMaterial[Mat].smTexture[0]->lpD3DTexture)))
			{
				OutputDebugString("SetTexture Error");
				return FALSE;
			}

			
			if (FAILED(renderDevice.DrawPrimitive(D3DPT_TRIANGLESTRIP,
							D3DFVF_TLVERTEX  ,
							tlVertex,
							4,

							NULL)))
			{
				OutputDebugString("DrawPrimitive Error");
				return FALSE;
			}

			
		}

		
		//renderDevice.SetTextureStageState( cnt, D3DTSS_MINFILTER, D3DTFN_LINEAR  );
        //renderDevice.SetTextureStageState( cnt, D3DTSS_MAGFILTER, D3DTFG_LINEAR  );
	}	


	return TRUE;
}

/*----------------------------------------------------------------------------*
*							������ �д´� 
*-----------------------------------------------------------------------------*/	
int cSINHELP::sinReadFileData(char *name)
{

	HANDLE hFile;
    DWORD dwAcess =0;
	hFile = CreateFile( name, 
				GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );


	ZeroMemory((void*)szFileReadBuf2, sizeof(szFileReadBuf2));

    if( hFile != INVALID_HANDLE_VALUE ) {
		DWORD rbyte;
		int chk = ReadFile(hFile, (LPVOID)szFileReadBuf2, sizeof(szFileReadBuf2 ), &rbyte, NULL);

        //���� �ݱ�
        CloseHandle( hFile );
		return TRUE;
    }

	return FALSE;		//���Ͼ���.

}

/*----------------------------------------------------------------------------*
*						������ ����Ÿ�� ���ۿ� �����Ѵ� 
*-----------------------------------------------------------------------------*/	
int cSINHELP::sinReadFileDataDivide(char *SaveBuf , int Linelen , int Size)
{

	int i, bufC=0,cnt=0,cnt2=0; 
	char c;
	int CheckByteFlag = 0;
	int CountType = 0;
	for(i=0; ; i++) {

		while(1) { 
			c = szFileReadBuf2[cnt++]; 
			
			if(c==0) break;	
			if(c==13) {
				cnt++;		//13, 10�� ĳ������������ ���� ����.
				break;
			}
			SaveBuf[i*Linelen + bufC] = c; 
			bufC++;

		}
		bufC = 0;
		if(c==0) break;					//���̸� ��
	}
	return TRUE;
}

/*
int cSINHELP::sinReadFileDataDivide(char *SaveBuf , int Linelen , int Size)
{

	int i, bufC=0,cnt=0,cnt2=0;
	char c;
	int CheckByteFlag = 0;
	int CountType = 0;
	for(i=0; ; i++) {

		while(1) { 
			c = szFileReadBuf2[cnt++]; 
			
			if(c==0) break;	
			if(c==13) {
				cnt++;		//13, 10�� ĳ������������ ���� ����.
				break;
			}


			SaveBuf[i*Linelen + bufC] = c; 
			bufC++;

			//���ڿ��� �������ʰ� �ڸ��� 
			CheckByteFlag = CheckCode_2Byte(&c);
			
			if(bufC > Size ){ //������ �������ش� 
				if(CountType == 1){
					CountType = 0;
					break;

				}
				if(CheckByteFlag == 1){
					break;

				}
				else if ( CheckByteFlag == 2 )
					CountType = 1;

				
			}

		}
		bufC = 0;
		if(c==0) break;					//���̸� ��
	}

	return TRUE;
}
*/
/*----------------------------------------------------------------------------*
*						Tony�� �׽�Ʈ ���� 
*-----------------------------------------------------------------------------*/	
static char sinDecode[512];
static char sinLine[512];
int cSINHELP::TonyGetFileData()
{
	//�ϴ� �ʱ�ȭ�� �Ѵ����� üũ�Ѵ� 
	memset(&sTonyTest,0,sizeof(sTONY_TEST_IMAGE)*10);

	char *szFilePath = "Image\\SinImage\\Tony\\";
	char szTempBuffFile[128];
	char szTempBuff[128];


	FILE	*fp;
	char strBuff[64];
	char *p;
//	char *pb;
//	int cnt;
	int Count = 0;
	int NextFlag = 0;

	wsprintf(szTempBuffFile,"%sTony.INI",szFilePath);
	fp = fopen( szTempBuffFile , "rb" );
	if ( fp==NULL ) return FALSE;

	while( !feof( fp ) )					//  feof: file end���� �о�� 
	{
		NextFlag = 0;
		if( fgets( sinLine, 255, fp ) == NULL)	
			break;

		p = GetWord( sinDecode , sinLine);
		
		//pb=p;p=GetWord(strBuff,p);if(strBuff[0]==34)p=GetString(strBuff,pb);	 " " ���� ��Ʈ���� ���� ã���� ����

		if ( !NextFlag && lstrcmp( sinDecode , "*�̹��������̸�" )==0 ) {
			p=GetWord(strBuff,p);
			lstrcpy(sTonyTest[Count].ImageFileName , strBuff);

			NextFlag = TRUE;

		}
		if ( !NextFlag && lstrcmp( sinDecode , "*�̹�����ġ" )==0 ) {
			p=GetWord(strBuff,p);
			sTonyTest[Count].Posi.x  = atoi(strBuff);
			p=GetWord(strBuff,p);
			sTonyTest[Count].Posi.y  = atoi(strBuff);
			NextFlag = TRUE;

		}
		if ( !NextFlag && lstrcmp( sinDecode , "*�̹���ũ��" )==0 ) {
			p=GetWord(strBuff,p);
			sTonyTest[Count].Size.x  = atoi(strBuff);
			p=GetWord(strBuff,p);
			sTonyTest[Count].Size.y  = atoi(strBuff);
			NextFlag = TRUE;

		}
		if ( !NextFlag && lstrcmp( sinDecode , "*��������" )==0 ) {
			p=GetWord(strBuff,p);
			if(lstrcmp(strBuff,"TGA")==0)
				sTonyTest[Count].FileFormat = TONY_TGA;
			if(lstrcmp(strBuff,"BMP")==0)
				sTonyTest[Count].FileFormat = TONY_BMP;

		}

		if ( !NextFlag && lstrcmp( sinDecode , "*��" )==0 ) {
			sTonyTest[Count].Flag = 1;
			//��⼭ ������ �ε��Ѵ� 
			wsprintf(szTempBuff,"%s%s",szFilePath,sTonyTest[Count].ImageFileName);
			if(sTonyTest[Count].FileFormat == TONY_TGA){
				//sTonyTest[Count].Mat =  CreateTextureMaterial( szTempBuff , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
				sTonyTest[Count].Mat = CreateTextureMaterial("Image\\SinImage\\Tony\\shop-1.tga", 0, 0, 0,0, SMMAT_BLEND_ALPHA );

			}
			if(sTonyTest[Count].FileFormat == TONY_BMP)
				sTonyTest[Count].lpTony =  LoadDibSurfaceOffscreen(szTempBuff);

			Count++;

		}

	}
	if (fp) fclose(fp);
	return TRUE;

}

///��ũ�� �Է� �Լ�
int cSINHELP::sinGetScrollMove(int Num)
{

	int i;
	for(i = 0; i < SIN_MAX_HELP_NUMBER ; i++){ 
		if(sSinHelp[i].KindFlag){ 
			if(sSinHelp[i].ScrollFlag){
				sSinHelp[i].ScrollButton.y += Num;
				if(sSinHelp[i].ScrollButton.y < sSinHelp[i].ScrollMAXPosiY_UP)sSinHelp[i].ScrollButton.y = sSinHelp[i].ScrollMAXPosiY_UP;
				if(sSinHelp[i].ScrollButton.y > sSinHelp[i].ScrollMAXPosiY_DOWN)sSinHelp[i].ScrollButton.y = sSinHelp[i].ScrollMAXPosiY_DOWN;
				return TRUE;

			}

		}
	}

	return FALSE;
	
}
///Ű�ٿ� �Է� �Լ� 
int cSINHELP::sinGetKeyScrollDown()
{
	int i;
	for(i = 0; i < SIN_MAX_HELP_NUMBER ; i++){ 
		if(sSinHelp[i].KindFlag){ 
			if(sSinHelp[i].ScrollFlag){
				if(sinGetKeyClick(VK_PRIOR)){
					sSinHelp[i].ScrollButton.y -=30;

				}

				if(sinGetKeyClick(VK_NEXT)){
					sSinHelp[i].ScrollButton.y +=30;

				}
				
				if(VRKeyBuff[VK_UP]){
					sSinHelp[i].ScrollButton.y --;

				}
				if(VRKeyBuff[VK_DOWN]){
					sSinHelp[i].ScrollButton.y ++;

				}
				
				if(sSinHelp[i].ScrollButton.y < sSinHelp[i].ScrollMAXPosiY_UP)sSinHelp[i].ScrollButton.y = sSinHelp[i].ScrollMAXPosiY_UP;
				if(sSinHelp[i].ScrollButton.y > sSinHelp[i].ScrollMAXPosiY_DOWN)sSinHelp[i].ScrollButton.y = sSinHelp[i].ScrollMAXPosiY_DOWN;
	

			}
		}
	}
	return TRUE;
}

//������ ���ִ����� �˾ƿ´� 
int cSINHELP::sinGetHelpShowFlag()
{
	int i;
	for(i = 0; i < SIN_MAX_HELP_NUMBER ; i++){ 
		if(sSinHelp[i].KindFlag){ 
			return TRUE;
		}
	}
	extern BOOL IsDrawGuideHelp;
	if(IsDrawGuideHelp)return TRUE;
	return FALSE;
}	
//����
struct sSCORESORT{
	int	Score;
	int Index;
};
sSCORESORT sScoreSort[11];

//SOD ��ŷ�� ���� 
int cSINHELP::ShowSodRanking(TRANS_SOD_RESULT *SodResult ,int MyGame)
{
	int  i = 0 , j = 0;   
	//�̹����� �ʿ��Ѱ�쿡�� ������ �ε��Ѵ� 
	if(!lpSodMenu) 
		lpSodMenu = LoadDibSurfaceOffscreen( "Image\\SinImage\\Help\\sod_list.bmp" );
	if(!lpSodLine)
		lpSodLine = LoadDibSurfaceOffscreen( "Image\\SinImage\\Help\\sod_list-line.bmp" );

	char szBuff[128];	 
	for( i = 0 ; i < 8 ; i++){
		if(!lpSodJobFace[i]){
			wsprintf(szBuff,"Image\\SinImage\\Help\\sod_cr-%d.bmp",i+1);
			lpSodJobFace[i] = LoadDibSurfaceOffscreen(szBuff );
		}
	}

	
	//�׽�Ʈ 
	/*
	SodResult->Result[0].Score = 9000;
	lstrcpy(SodResult->Result[0].szName,"���㵢��");
	SodResult->Result[0].Monster = 20;
	SodResult->Result[0].JobCode = 1;

	SodResult->Result[1].Score = 90003;
	lstrcpy(SodResult->Result[1].szName,"¯��");
	SodResult->Result[1].Monster = 12;
	SodResult->Result[1].JobCode = 3;
	SodResult->Result[2].Score = 900;
	lstrcpy(SodResult->Result[2].szName,"����");
	SodResult->Result[2].Monster = 4;
	SodResult->Result[2].JobCode = 3;
	SodResult->Result[3].Score = 96550;
	lstrcpy(SodResult->Result[3].szName,"sinbaram");
	SodResult->Result[3].Monster = 111;
	SodResult->Result[3].JobCode = 5;
	SodResult->Result[4].Score = 9;
	lstrcpy(SodResult->Result[4].szName,"+_+");
	SodResult->Result[4].Monster = 22420;
	SodResult->Result[4].JobCode = 2;
	SodResult->Result[5].Score = 9001;
	lstrcpy(SodResult->Result[5].szName,"�̹������");
	SodResult->Result[5].Monster = 201;
	SodResult->Result[5].JobCode = 1;

	SodResult->ResultCount = 6;
	*/
	
	
	memset(sScoreSort,0,sizeof(sSCORESORT)*11);
	memset(sSod_Rank,0,sizeof(sSOD_RANK)*10);
	for(i = 0;i < SodResult->ResultCount ; i++){
		sSod_Rank[i].Flag = 1;
		sScoreSort[i].Score = SodResult->Result[i].Score;
		if(sScoreSort[i].Score < 0) sScoreSort[i].Score = 0;
		sScoreSort[i].Index = i;

	}
	int Temp=0;
	for( i = 10; i > 0; i--){
		for( j = 0; j < i; j++){
			if(sScoreSort[j].Score < sScoreSort[j+1].Score){
				Temp = sScoreSort[j+1].Score;
				sScoreSort[j+1].Score = sScoreSort[j].Score;
				sScoreSort[j].Score = Temp;

				Temp = sScoreSort[j+1].Index;
				sScoreSort[j+1].Index = sScoreSort[j].Index;
				sScoreSort[j].Index = Temp;

			}
		}
	}

	for( j = 0; j < 10 ; j++){ 
		if(sSod_Rank[j].Flag){
			sSod_Rank[j].Job = SodResult->Result[sScoreSort[j].Index].JobCode;
			lstrcpy(sSod_Rank[j].Name,SodResult->Result[sScoreSort[j].Index].szName);
			sSod_Rank[j].Score = SodResult->Result[sScoreSort[j].Index].Score;
			if(sSod_Rank[j].Score < 0 )sSod_Rank[j].Score = 0;
			sSod_Rank[j].KillNum = SodResult->Result[sScoreSort[j].Index].Monster;
			if(sSod_Rank[j].KillNum < 0 )sSod_Rank[j].KillNum = 0;
		}
	}
	
	HelpBoxKindIndex = SIN_SOD;   
	if(MyGame){
		cSinHelp.sinShowHelp(SIN_HELP_KIND_SOD,QuestMessageBoxPosi2.x+170,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),"");  
	}
	else cSinHelp.sinShowHelp(SIN_HELP_KIND_SOD,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),"");  
	
	return TRUE;
}	

//Sod �ڽ��� ���ִ����� ã�´�
int cSINHELP::GetSodRankBoxState()
{
	for(int i = 0; i < SIN_MAX_HELP_NUMBER ; i++){   
		if(sSinHelp[i].KindFlag){
			if(sSinHelp[i].KindFlag == SIN_HELP_KIND_SOD){ 
				if(sSinHelp[i].PosiX  < QuestMessageBoxPosi2.x+170)	return TRUE;

			}
		}
	}
	return FALSE;
}



//�����������ش�
int sinShowHelp()
{
	char *HelpFilePath = "image\\Sinimage\\Help\\Help.sin"; 
	cSinHelp.sinShowHelp(SIN_HELP_KIND_NORMAL,208,61-50,11,11, RGBA_MAKE(19,27,20,165),HelpFilePath); 
	HelpBoxKindIndex = SIN_HELP;
	return TRUE;
}

//�ڷ���Ʈ�Ҽ��ִ� ��Ҹ� �����ش�
int sinShowTeleportMap()
{
	cSinHelp.NPCTeleportFlag = 1;
	cSinHelp.sinShowHelp(SIN_HELP_KIND_TELEPORT,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),TeleportFilePath);  
	return TRUE;
	
}
// pluto �������� �ڷ���Ʈ
int TeleportDungeon()
{
	char *TeleportText = "image\\Sinimage\\Help\\EndlessTeleport.sin";
	cSinHelp.NPCTeleportFlag = 1;
#ifdef _LANGUAGE_CHINESE
	if(sinChar->Level >= 102)
	{
		cSinHelp.sinShowHelp(SIN_HELP_KIND_TELEPORT_MILTER,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),TeleportText);
	}
#else
		cSinHelp.sinShowHelp(SIN_HELP_KIND_TELEPORT_MILTER,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),TeleportText);
#endif
	//cMessageBox.ShowMessage3( MESSAGE_TELEPORT_DUNGEON, "������ ž" );
	return TRUE;
}
//haGoon-------------------------------------------------------------------/
//���� ĳ�� �ڷ���Ʈ ��Ҹ� �����ش�,.
int haShowCastleTeleportMap()
{
	haC_TeleportMoney = sinChar->Level*500;    //�ڷ���Ʈ �Ӵϸ� �����Ѵ�.
	cSinHelp.sinShowHelp(SIN_HELP_KIND_C_TELEPORT,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125), haC_TeleportFilePath);


#ifdef _LANGUAGE_PHILIPIN
	haC_TeleportMoney = sinChar->Level*250;    //�ڷ���Ʈ �Ӵϸ� �����Ѵ�.
#endif

	// �庰 - ���� ĳ��
	/*
	if ( sBiInfo && (sBiInfo->EventPlay[0]&BIMASK_ADULT_USER)!=0 ){
		cSinHelp.sinShowHelp(SIN_HELP_KIND_C_TELEPORT,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),												haC_TeleportFilePath);  
	}
	else{
		cMessageBox.ShowMessage(MESSAGE_NOT_CASTLE_TELEPORT);
		cSinHelp.sinShowHelp(SIN_HELP_KIND_C_TELEPORT,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),												haC_Not_TeleportFilePath);  
	}
	*/
	return TRUE;
}
//haGoon
//�������� �޴��� �����ش�.-------------------------------------------------------------------------//
int cSINHELP::ShowResearchMenu(int Kind,int Flag)
{
	switch(Kind){
		case 1:
			haResearch.Index = Kind;
			if(Flag)
				cSinHelp.sinShowHelp(SIN_HELP_KIND_RESEARCH,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),												haResearchFilePath1);  
			else
				cMessageBox.ShowMessage(MESSAGE_NOT_RESEARCH);

		case 2:
			haResearch.Index = Kind;
			if(Flag)
				cSinHelp.sinShowHelp(SIN_HELP_KIND_RESEARCH,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),												haResearchFilePath2);  
			else
				cMessageBox.ShowMessage(MESSAGE_NOT_RESEARCH);
		break;	
		case 3:
			haResearch.Index = Kind;
			if(Flag)
				cSinHelp.sinShowHelp(SIN_HELP_KIND_RESEARCH,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),												haResearchFilePath3);  
			else
				cMessageBox.ShowMessage(MESSAGE_NOT_RESEARCH);
		break;
	}
		
	return TRUE;
}
//�������� ����� ������ ������.
int cSINHELP::SendResearch()
{
	for(int i=0;i<haResearch.Number;i++)
		haResearch.bSendPollingData[i] = haResearch.Value[i];
	SendPublicPollingToServer(haResearch.Index ,haResearch.Number,haResearch.bSendPollingData);
	return TRUE;
}

// pluto ���� �ڷ���Ʈ �ھ� ����
int cSINHELP::sinGetHelpShowFlagNum()
{
	int KindFlag = 0;
	for( int i = 0; i < SIN_MAX_HELP_NUMBER; i++ )
	{
		if( sSinHelp[i].KindFlag )
		{
			KindFlag = sSinHelp[i].KindFlag;
			return KindFlag;
		}
	}
	return KindFlag;
}