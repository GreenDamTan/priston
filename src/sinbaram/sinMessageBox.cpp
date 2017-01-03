/*----------------------------------------------------------------------------*
*	���ϸ� :  sinMessageBox.cpp
*	�ϴ��� :  ���� �޼��� �ڽ����� �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"
#include "..\\skillsub.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\clan_Enti.h"
#include "..\\field.h"

/*----------------------------------------------------------------------------*
*								�������� 			
*-----------------------------------------------------------------------------*/	
#define SMASHTV_MONEY	300

sITEM MessageBoxItem; //�޼��� �ڽ����� ���� ����ü 
sITEM *pMessageItem;  //�޼��� �ڽ����� ���� ������ ������ 
sITEM *pMessageItem2;  //�޼��� �ڽ����� ���� ������ ������ (�ǵ�� �÷��� �Ѱ� ���ͱ�)

cMESSAGEBOX cMessageBox;
int MessageTimeCount = 0 ;

int sinMessageBoxShowFlag = 0; //���� �޼����ڽ��� �ֳ� ������ �÷����� �˼��ִ� 

int MessageArrowPosi = 0;      //�ڽ��� ȭ��ǥ ������ 
int MessageButtonPosi = 0;     //��ư ������ 

int CountTemp = 0;

int CheckCountSize[10] = {0,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
int MoneyWeight = 10000;

int ExpressItemFlag = 0;

int sinSkillPontInitButtonFlag = 0; //��ų�ʱ�ȭ �޼��� �ڽ� 
int sinGiveMoneyCheckFlag = 0; //��α� �ѹ��� üũ
int sinGiveMoneyAmount = 0;
char szGiveMoneyBuff[32];

int sinInitPointFlag7 = 0;
int sinInitPointPassFlag = 0; //ù�� �ѹ��� ����Ʈ üũ�� �н��Ѵ�


sMESSAGEBOX_RECVITEM sMessageBox_RecvItem;

DIRECTDRAWSURFACE lpButtonYes07;
DIRECTDRAWSURFACE lpButtonYesGlay07;
DIRECTDRAWSURFACE lpButtonNo07;
DIRECTDRAWSURFACE lpButtonNoGlay07;
DIRECTDRAWSURFACE lpButtonBox07;

//��������
DIRECTDRAWSURFACE lpStarIcon;

DIRECTDRAWSURFACE lpStarShopClose_;
DIRECTDRAWSURFACE lpStarShopClose;


//��������
sSTAR_SHOP sStarShop;

//����� ������
sLOST_ITEM sLost_Item;
int GiftPrice = 0;
int MatMyShopPRBoxLine = 0;
/*----------------------------------------------------------------------------*
*							Ŭ���� �ʱ�, ���� 
*-----------------------------------------------------------------------------*/	
cMESSAGEBOX::cMESSAGEBOX()
{

}
cMESSAGEBOX::~cMESSAGEBOX()
{

}
/*----------------------------------------------------------------------------*
*							     �ʱ�ȭ 
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::Init()
{
	/////////////////////// ���¸� ǥ���ϴ� �޼��� 
	MatBoxLeft	= CreateTextureMaterial( "Image\\SinImage\\MessageBox\\CutBox\\BoxLeft.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );   //���� �ڽ� �׸� 
	MatBoxRight	= CreateTextureMaterial( "Image\\SinImage\\MessageBox\\CutBox\\BoxRight.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );   //���� �ڽ� �׸� 
	MatBoxCenter = CreateTextureMaterial( "Image\\SinImage\\MessageBox\\CutBox\\BoxCenter.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );  //�ϴ� �ڽ� �׸� 
	
	MatBoxMain = CreateTextureMaterial( "Image\\SinImage\\MessageBox\\BoxMain.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );  //�ڽ� ���� 

	//ȫ���ڽ� ����
	MatMyShopPRBoxLine = CreateTextureMaterial( "Image\\SinImage\\Inven\\PR_Box.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );  //�ڽ� ���� 

	Load();
}
/*----------------------------------------------------------------------------*
*						    Bmp������ �ε��Ѵ� 
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::Load()
{

	lpButtonYes07		= LoadDibSurfaceOffscreen("image\\Sinimage\\MessageBox\\Button_Yes_Yellow.bmp");
	lpButtonYesGlay07	= LoadDibSurfaceOffscreen("image\\Sinimage\\MessageBox\\Button_Yes_Glay.bmp");
	lpButtonNo07		= LoadDibSurfaceOffscreen("image\\Sinimage\\MessageBox\\Button_No_Yellow.bmp");
	lpButtonNoGlay07	= LoadDibSurfaceOffscreen("image\\Sinimage\\MessageBox\\Button_No_Glay.bmp");	
	lpButtonBox07       = LoadDibSurfaceOffscreen("image\\Sinimage\\Help\\box-test-title2.bmp");

	lpArrowUpDown[0]  = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Arrow_Up.bmp"); // 0 Up  1 Down
	lpArrowUpDown[1]  = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Arrow_Down.bmp"); // 0 Up  1 Down

	lpButtonNo[0]     = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Button_No_Glay.bmp"); 
	lpButtonNo[1]     = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Button_No_Yellow.bmp"); 

	lpButtonYes[0]    = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Button_Yes_Glay.bmp"); 
	lpButtonYes[1]    = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Button_Yes_Yellow.bmp"); 

	lpButtonOk[0]     = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Button_Ok_Glay.bmp"); 
	lpButtonOk[1]     = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Button_Ok_Yellow.bmp"); 
	
	lpButtonCancel[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Button_Cancel_Glay.bmp"); 
	lpButtonCancel[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\Button_Cancel_Yellow.bmp"); 

	lpCount = LoadDibSurfaceOffscreen("Image\\SinImage\\MessageBox\\COUNT.bmp"); 

	//��������
	lpStarIcon = LoadDibSurfaceOffscreen("image\\Sinimage\\MessageBox\\Star.bmp");
	lpStarShopClose = LoadDibSurfaceOffscreen("image\\Sinimage\\MessageBox\\c.bmp");
	lpStarShopClose_ = LoadDibSurfaceOffscreen("image\\Sinimage\\MessageBox\\c1.bmp");

}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::Release()
{
	if(lpArrowUpDown[0]){
		lpArrowUpDown[0]->Release();
		lpArrowUpDown[0] = 0;

	}
	if(lpArrowUpDown[1]){
		lpArrowUpDown[1]->Release();
		lpArrowUpDown[1] = 0;

	}
	if(lpButtonNo[0]){
		lpButtonNo[0]->Release();
		lpButtonNo[0] = 0;

	}
	if(lpButtonNo[1]){
		lpButtonNo[1]->Release();
		lpButtonNo[1] = 0;

	}
	if(lpButtonYes[0]){
		lpButtonYes[0]->Release();
		lpButtonYes[0] = 0;

	}
	if(lpButtonYes[1]){
		lpButtonYes[1]->Release();
		lpButtonYes[1] = 0;
	}

	if(lpButtonOk[0]){
		lpButtonOk[0]->Release();
		lpButtonOk[0] = 0;
	}
	if(lpButtonOk[1]){
		lpButtonOk[1]->Release();
		lpButtonOk[1] = 0;
	}
	if(lpButtonCancel[0]){
		lpButtonCancel[0]->Release();
		lpButtonCancel[0] = 0;
	}
	if(lpButtonCancel[1]){
		lpButtonCancel[1]->Release();
		lpButtonCancel[1] = 0;
	}

}
/*----------------------------------------------------------------------------*
*							     �׸���  
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::Draw()
{
	int i =0;   
	int x = 0 , y = 0;

	//����� ������
	if(sLost_Item.Flag){      
		Interface_clanmenu.DrawBox(BackStartPos.x,BackStartPos.y,4,5);	    

		if(sLost_Item.LostItem[0].CODE){ //����� �������� �̹����� �����ش�
			x = (60 - sLost_Item.LostItem[0].w )/2;
			y = (80 - sLost_Item.LostItem[0].h )/2;

			DrawSprite(BackStartPos.x+20+x,BackStartPos.y+80+y,sLost_Item.LostItem[0].lpItem,0,0,sLost_Item.LostItem[0].w,sLost_Item.LostItem[0].h);
		}
		if(sLost_Item.LostItem[1].CODE){ //����� �������� �̹����� �����ش�
			x = (60 - sLost_Item.LostItem[0].w )/2;
			y = (80 - sLost_Item.LostItem[0].h )/2;

			DrawSprite(BackStartPos.x+20+x+50,BackStartPos.y+80+y+22,sLost_Item.LostItem[1].lpItem,0,0,sLost_Item.LostItem[1].w,sLost_Item.LostItem[1].h);
		}

		DrawSprite(BackStartPos.x+BUTTON2_BOX_X+35,BackStartPos.y+BUTTON2_BOX_Y, lpButtonBox07,  0, 0, 48, 23, 1); 
		if(sLost_Item.BuyOk){
			DrawSprite(BackStartPos.x+BUTTON2_X+35,BackStartPos.y+BUTTON2_BOX_Y+4, lpButtonOk[1],  0, 0, 32, 16, 1);
		}
		else DrawSprite(BackStartPos.x+BUTTON2_X+35,BackStartPos.y+BUTTON2_BOX_Y+4, lpButtonOk[0],  0, 0, 32, 16, 1);

	}
	////////////////////�� ������ ����
	if(sStarShop.Flag){        
		Interface_clanmenu.DrawBox(BackStartPos.x,BackStartPos.y,4,5);	    

		DrawSprite(BackStartPos.x+60,BackStartPos.y+60, lpStarIcon,  0, 0, 32, 32, 1);
		DrawSprite(BackStartPos.x+BUTTON2_BOX_X+35,BackStartPos.y+BUTTON2_BOX_Y, lpButtonBox07,  0, 0, 48, 23, 1);
		if(sStarShop.ButtonClose)
			DrawSprite(BackStartPos.x+BUTTON2_X+35,BackStartPos.y+BUTTON2_Y, lpStarShopClose_,  0, 0, 32, 16, 1);
		else DrawSprite(BackStartPos.x+BUTTON2_X+35,BackStartPos.y+BUTTON2_Y, lpStarShopClose,  0, 0, 32, 16, 1);

		DrawSprite(BackStartPos.x+BUTTON2_BOX_X+35,BackStartPos.y+160, lpButtonBox07,  0, 0, 48, 23, 1); 
		if(sStarShop.BuyOk){
			DrawSprite(BackStartPos.x+BUTTON2_X+35,BackStartPos.y+163, lpButtonYes07,  0, 0, 32, 16, 1);
		}
		else DrawSprite(BackStartPos.x+BUTTON2_X+35,BackStartPos.y+163, lpButtonYesGlay07,  0, 0, 32, 16, 1);
	}
	////////////////////

	//������ �ޱ� �޼��� �ڽ�
	if(sMessageBox_RecvItem.Flag){   
		Interface_clanmenu.DrawBox(BackStartPos.x,BackStartPos.y,4,5);	
		DrawSprite(BackStartPos.x+BUTTON2_BOX_X,BackStartPos.y+BUTTON2_BOX_Y, lpButtonBox07,  0, 0, 48, 23, 1);
		DrawSprite(BackStartPos.x+BUTTON3_BOX_X,BackStartPos.y+BUTTON3_BOX_Y, lpButtonBox07,  0, 0, 48, 23, 1);
	
		if(sMessageBox_RecvItem.ButtonYes)
			DrawSprite(BackStartPos.x+BUTTON2_X,BackStartPos.y+BUTTON2_Y, lpButtonYes07,  0, 0, 32, 16, 1);
		else DrawSprite(BackStartPos.x+BUTTON2_X,BackStartPos.y+BUTTON2_Y, lpButtonYesGlay07,  0, 0, 32, 16, 1);
		if(sMessageBox_RecvItem.ButtonNo)
			DrawSprite(BackStartPos.x+BUTTON3_X,BackStartPos.y+BUTTON3_Y, lpButtonNo07,  0, 0, 32, 16, 1);
		else DrawSprite(BackStartPos.x+BUTTON3_X,BackStartPos.y+BUTTON3_Y, lpButtonNoGlay07,  0, 0, 32, 16, 1);

		if(sMessageBox_RecvItem.RecvItem.CODE){ //���� �������� �̹����� �׷��ش� 
			x = (60 - sMessageBox_RecvItem.RecvItem.w )/2;
			y = (80 - sMessageBox_RecvItem.RecvItem.h )/2;

			DrawSprite(BackStartPos.x+20+x,BackStartPos.y+80+y,sMessageBox_RecvItem.RecvItem.lpItem,0,0,sMessageBox_RecvItem.RecvItem.w,sMessageBox_RecvItem.RecvItem.h);
		}
	}
	for(i = 0 ; i < MESSAGE3_MAX ; i++){
		if(sMessageBox3[i].Flag){
			dsDrawTexImage( MatBoxMain , sMessageBox3[i].x , sMessageBox3[i].y , 256, 128 , 255 );  //���
			DrawSprite(sMessageBox3[i].x+40  , sMessageBox3[i].y+95 , lpButtonOk[0] ,0 ,0 , 32 , 16);	
			DrawSprite(sMessageBox3[i].x+94 , sMessageBox3[i].y+95 , lpButtonCancel[0] ,0 ,0 , 32 , 16);
		
			if(MessageButtonPosi == 1)//Ȯ�� 
				DrawSprite(sMessageBox3[i].x+40  , sMessageBox3[i].y+95 , lpButtonOk[1] ,0 ,0 , 32 , 16);
			if(MessageButtonPosi == 2)//��� 
				DrawSprite(sMessageBox3[i].x+94 , sMessageBox3[i].y+95 , lpButtonCancel[1] ,0 ,0 , 32 , 16);
		}
	}

	for(i = 0 ; i < 20 ; i++){  
		if(sMessageBox2[i].Flag){
			dsDrawTexImage( MatBoxMain , sMessageBox2[i].x , sMessageBox2[i].y , 256, 128 , 255 );  //���
			DrawSprite(sMessageBox2[i].x+34 , sMessageBox2[i].y+62 , lpCount ,0 ,0 , 109 , 25);
			if( i != MESSAGE_BUY_STAR){ //�� �ƴϿ��� �ϱ����� ��¦ ����
				DrawSprite(sMessageBox2[i].x+40  , sMessageBox2[i].y+95 , lpButtonOk[0] ,0 ,0 , 32 , 16);	
				DrawSprite(sMessageBox2[i].x+94 , sMessageBox2[i].y+95 , lpButtonCancel[0] ,0 ,0 , 32 , 16);
				if(MessageButtonPosi == 1)//Ȯ�� 
					DrawSprite(sMessageBox2[i].x+40  , sMessageBox2[i].y+95 , lpButtonOk[1] ,0 ,0 , 32 , 16);
				if(MessageButtonPosi == 2)//��� 
					DrawSprite(sMessageBox2[i].x+94 , sMessageBox2[i].y+95 , lpButtonCancel[1] ,0 ,0 , 32 , 16);
			}	
			if(MessageArrowPosi == 1) //Up
				DrawSprite(sMessageBox2[i].x+124 , sMessageBox2[i].y+62 , lpArrowUpDown[0] ,0 ,0 , 19 , 13);
			if(MessageArrowPosi == 2) //Down
				DrawSprite(sMessageBox2[i].x+124 , sMessageBox2[i].y+75 , lpArrowUpDown[1] ,0 ,0 , 19 , 13);
			
			if( i== MESSAGE_BUY_STAR){
				DrawSprite(sMessageBox2[i].x+40  , sMessageBox2[i].y+95 , lpButtonYes[0] ,0 ,0 , 32 , 16);	
				DrawSprite(sMessageBox2[i].x+94 , sMessageBox2[i].y+95 , lpButtonNo[0] ,0 ,0 , 32 , 16);
				if(MessageButtonPosi == 1)//Ȯ�� 
					DrawSprite(sMessageBox2[i].x+40  , sMessageBox2[i].y+95 , lpButtonYes[1] ,0 ,0 , 32 , 16);
				if(MessageButtonPosi == 2)//��� 
					DrawSprite(sMessageBox2[i].x+94 , sMessageBox2[i].y+95 , lpButtonNo[1] ,0 ,0 , 32 , 16);
			}
		}
	}
	
	for(i = 0 ; i < 100 ; i++){
		if(sMessageBox[i].Flag){
			dsDrawTexImage( MatBoxLeft , sMessageBox[i].x , sMessageBox[i].y, 32, 64 , 255 );  //���� 
			int j = 0;
			for(j=0 ; j< sMessageBox[i].Line ; j++){
				dsDrawTexImage( MatBoxCenter , sMessageBox[i].x+32+(j*16) , sMessageBox[i].y, 16, 64, 255 );//���
			}
			dsDrawTexImage( MatBoxRight , sMessageBox[i].x+32+(j*16) , sMessageBox[i].y , 32, 64 , 255 );  //���
	
		}
	}
}
/*----------------------------------------------------------------------------*
*							     ����  
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::Main()
{

    MessageArrowPosi = 0;      //�ڽ��� ȭ��ǥ ������ 
    MessageButtonPosi = 0;     //��ư ������ 

	int i=0;
	for( i = 0 ; i < 100 ; i++){ 
		if(sMessageBox[i].Flag){
			MessageTimeCount++;
			if(	i == MESSAGE_MOVE_TRADE_ITEM){	//������ �޼����� ��� �����ش� 
				if(MessageTimeCount > 70*5){ //3���� �޼����� �����ش� 
					sMessageBox[i].Flag = 0;
					MessageTimeCount = 0;    //�ʱ�ȭ 
				}
			}
			else{
				 /////// �̺�Ʈ ���� ���ھ �ð��� ������ش� (����� ��ġ�� ����)
				if( i == MESSAGE_SCORE ) {
					if(MessageTimeCount > 70*6){ //3���� �޼����� �����ش� 
						sMessageBox[i].Flag = 0;
						MessageTimeCount = 0;    //�ʱ�ȭ 
					}

				}
				else{
					if(MessageTimeCount > 70*3){ //3���� �޼����� �����ش� 
						sMessageBox[i].Flag = 0;
						MessageTimeCount = 0;    //�ʱ�ȭ 
					}
				}
			}
		}
	}

	if(sLost_Item.Flag){
		BackStartPos.x = 101;
		BackStartPos.y = 114; 
		sLost_Item.BuyOk = 0;
			if(pCursorPos.x >BackStartPos.x+BUTTON2_X+35 && pCursorPos.x < BackStartPos.x+BUTTON2_X+35+32 && 
				pCursorPos.y >BackStartPos.y+BUTTON2_BOX_Y && pCursorPos.y < BackStartPos.y+BUTTON2_BOX_Y+16)
				sLost_Item.BuyOk = 1;

	}

	//��������
	if(sStarShop.Flag){    
		BackStartPos.x = 101;
		BackStartPos.y = 114;
		sStarShop.ButtonClose = 0;
		if(!sinMessageBoxShowFlag){ //�޼��� �ڽ��� �ߺ����� �ߴ°� ���´�
			if(pCursorPos.x >BackStartPos.x+BUTTON2_X+35 && pCursorPos.x < BackStartPos.x+BUTTON2_X+35+32 && 
				pCursorPos.y >BackStartPos.y+BUTTON2_Y && pCursorPos.y < BackStartPos.y+BUTTON2_Y+16)
				sStarShop.ButtonClose = 1;

			sStarShop.BuyOk = 0;
			if(pCursorPos.x >BackStartPos.x+BUTTON2_X+35 && pCursorPos.x < BackStartPos.x+BUTTON2_X+35+32 && 
				pCursorPos.y >BackStartPos.y+160 && pCursorPos.y < BackStartPos.y+160+16)
				sStarShop.BuyOk = 1;
		}
	}

	//������ �ù� ���� 
	if(sMessageBox_RecvItem.Flag){
		BackStartPos.x = 101;
		BackStartPos.y = 114;
		sMessageBox_RecvItem.ButtonYes = 0;
		sMessageBox_RecvItem.ButtonNo = 0;
		if(pCursorPos.x >BackStartPos.x+BUTTON2_X && pCursorPos.x < BackStartPos.x+BUTTON2_X+32 && 
			pCursorPos.y >BackStartPos.y+BUTTON2_Y && pCursorPos.y < BackStartPos.y+BUTTON2_Y+16)
			sMessageBox_RecvItem.ButtonYes = 1;
		
		if(pCursorPos.x >BackStartPos.x+BUTTON3_X && pCursorPos.x < BackStartPos.x+BUTTON3_X+32 && 
			pCursorPos.y >BackStartPos.y+BUTTON3_Y && pCursorPos.y < BackStartPos.y+BUTTON3_Y+16)
			sMessageBox_RecvItem.ButtonNo = 1;
	}

	for( i = 0 ; i < 20 ; i++){
		if(sMessageBox2[i].Flag){
			if(pCursorPos.x >sMessageBox2[i].x+124 && pCursorPos.x < sMessageBox2[i].x+124+19 && 
				pCursorPos.y >sMessageBox2[i].y+62 && pCursorPos.y < sMessageBox2[i].y+62+13)
				MessageArrowPosi = 1; //ȭ��ǥ �� 

			if(pCursorPos.x >sMessageBox2[i].x+124 && pCursorPos.x < sMessageBox2[i].x+124+19 && 
				pCursorPos.y >sMessageBox2[i].y+75 && pCursorPos.y < sMessageBox2[i].y+75+13)
				MessageArrowPosi = 2; //ȭ��ǥ �ٿ� 

			if(pCursorPos.x >sMessageBox2[i].x+40 && pCursorPos.x < sMessageBox2[i].x+40+32 && 
				pCursorPos.y >sMessageBox2[i].y+95 && pCursorPos.y < sMessageBox2[i].y+98+16)
				MessageButtonPosi = 1; //OK

			if(pCursorPos.x >sMessageBox2[i].x+94 && pCursorPos.x < sMessageBox2[i].x+94+32 && 
				pCursorPos.y >sMessageBox2[i].y+95 && pCursorPos.y < sMessageBox2[i].y+95+16)
				MessageButtonPosi = 2; //Cancel
		}

	}
	for( i = 0 ; i <MESSAGE3_MAX ; i++){
		if(sMessageBox3[i].Flag){
			if(pCursorPos.x >sMessageBox3[i].x+40 && pCursorPos.x < sMessageBox3[i].x+40+32 && 
				pCursorPos.y >sMessageBox3[i].y+95 && pCursorPos.y < sMessageBox3[i].y+98+16)
				MessageButtonPosi = 1; //OK

			if(pCursorPos.x >sMessageBox3[i].x+94 && pCursorPos.x < sMessageBox3[i].x+94+32 && 
				pCursorPos.y >sMessageBox3[i].y+95 && pCursorPos.y < sMessageBox3[i].y+95+16)
				MessageButtonPosi = 2; //Cancel
		}

	}
	//////////////////// ���� �޼��� �ڽ�
	//��α��� �ٽ��ѹ� Ȯ���غ���
	if(sinGiveMoneyCheckFlag){
		sinGiveMoneyCheckFlag = 0;
		cMessageBox.ShowMessage3(MESSAGE_GIVE_MONEY_CHECK,szGiveMoneyBuff); 

	}

}
/*----------------------------------------------------------------------------*
*							     ���� 
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::Close()
{
	Release();
}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::LButtonDown(int x , int y)
{

	int i = 0;
	
	if(sLost_Item.Flag){  
		if(sLost_Item.BuyOk){
			memset(&sLost_Item,0,sizeof(sLOST_ITEM));
			isDrawClanMenu = 0;
			OverDay_Item_Flag = 0;
		}
	}

	//��������
	if(sStarShop.Flag){    
		if(sStarShop.ButtonClose){
			sStarShop.Flag = 0;
			isDrawClanMenu = 0;
		}
		if(sStarShop.BuyOk){
			sStarShop.BuyOk = 0;
			//sStarShop.Flag = 0;
			//isDrawClanMenu = 0;
			cMessageBox.ShowMessage2(MESSAGE_BUY_STAR);

		}

	}

	//������ �ù輭��
	if(sMessageBox_RecvItem.Flag){  
		if(sMessageBox_RecvItem.ButtonYes){ //������ ���� ������ �ڵ庸�� 
			if(cInvenTory.CheckInvenEmpty(&sMessageBox_RecvItem.RecvItem)){
				if(sMessageBox_RecvItem.Param[0]){
					cMessageBox.ShowMessage3(MESSAGE_SECRET_NUM,SecretNumName);
					isDrawClanMenu = 0;
				}
				else{
					if(sMessageBox_RecvItem.RecvItem.CODE == (sinGG1|sin01)){ //���� ���� ���� ó�����ش�
						if(!cInvenTory.CheckMoneyLimit(sMessageBox_RecvItem.RecvItem.SellPrice)){
							cMessageBox.ShowMessage(MESSAGE_OVER_MONEY);
							isDrawClanMenu = 0;
							sMessageBox_RecvItem.Flag = 0;
						}
						else{
							SendItemExpress(sMessageBox_RecvItem.RecvItem.CODE);
							isDrawClanMenu = 0;
							sMessageBox_RecvItem.Flag = 0;
						}
					}
					else{
						SendItemExpress(sMessageBox_RecvItem.RecvItem.CODE);
						isDrawClanMenu = 0;
						sMessageBox_RecvItem.Flag = 0;
					}

				}
			}
			else{
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				isDrawClanMenu = 0;
				sMessageBox_RecvItem.Flag = 0;
			}

		}
		if(sMessageBox_RecvItem.ButtonNo){
			sMessageBox_RecvItem.Flag = 0;
			isDrawClanMenu = 0;
		}
	}

	//////////��ų ���� �޼��� 
	for( i = 0 ; i < MESSAGE3_MAX ; i++){    
		if(sMessageBox3[i].Flag){
			if(MessageButtonPosi == 1){ //Ȯ�� 
				CheckOkMessageBox2(i);
				//LearnSkillEffect(0);
			}
			if(MessageButtonPosi == 2){ //���
				if( i == MESSAGE_QUEST_100LVITEMOK)
				{
					chaQuest.sHaQuest100LV.State--;
					chaQuest.showFuryQuestItem(1);
				}
				if( i == MESSAGE_CHANGE_JOB4_2)
				{
					if (sinChar->JOB_CODE <= 4 || sinChar->JOB_CODE == 9)
						WarpField2(START_FIELD_NUM); //���ư���
					else
						WarpField2(START_FIELD_MORYON); //���ư���
				}
				if(i == MESSAGE_SELL_HIGHRANK_ITEM){
					MouseItem.Flag = 1;
					ResetInvenItemCode();
					cInvenTory.AutoSetInvenItem(&MouseItem);
					
				}
				//�ɷ�ġ �ʱ�ȭ�� ��ų�ʱ�ȭ������ �ӽ÷� �־��ش� 
				if(i == MESSAGE_EVENT_GYUNGFUM){
					if(sinChar->wVersion[1] == 1){
						sinSkillPontInitButtonFlag = 0;
					}
					else sinSkillPontInitButtonFlag = 1;
						
				}
				//��������Ʈ 
				if(i == MESSAGE_WARP){ 
					WingWarpGate_Field(-1);

				}
				if( i ==MESSAGE_SECRET_NUM){
					hFocusWnd = 0;  
					cInterFace.ChatFlag = 0;
					SetWindowText( hFocusWnd , "");
					sinChatEnter = 0; //���Ƿ� �۾��� �Ⱥ��̰��Ѵ�
				}
				//100LV����Ʈ
				if( i == MESSAGE_QUEST_100LVITEMOK){
					chaQuest.iFuryQuestIndex[1]=0;
				}

				sinMessageBoxShowFlag  = 0;
				sMessageBox3[i].Flag   = 0;
				ShowSkillUpInfo = 0;
				sinInitPointFlag7 = 0;
			}
		}
	}

	for( i = 0 ; i < 20 ; i++){ 
		if(sMessageBox2[i].Flag){
			if(MessageArrowPosi == 1){
				if(i != MESSAGE_BUY_ITEM_DEFALT)
					sMessageBox2[i].Count ++;
				if(i == 3 || i == MESSAGE_MOVE_MONEY_TRADE || i == MESSAGE_MOVE_MONEY_MOVE || i == MESSAGE_GIVE_MONEY){//���̸� 
					if(sinChar->Money < sMessageBox2[i].Count){
						sMessageBox2[i].Count = sinChar->Money;
					}
				}
				if(i == MESSAGE_SOD2_GET_MONEY  ){
					if(cInvenTory.CheckMoneyLimit(sMessageBox2[i].Count)){ 
						if(sMessageBox2[i].Count > (int)cSinSod2.ClanMoney){
							sMessageBox2[i].Count = (int)cSinSod2.ClanMoney;
						}
					}
					else
						sMessageBox2[i].Count /= 10;
				}
				if(i == MESSAGE_SIEGE_SET_MERCENRAY){

//#ifdef _TEST_SERVER	
					//��üũ
		//			if((sinChar->Money - ((haMercenrayMoney[haMercenrayIndex-1])/10)*sMessageBox2[i].Count) < 0){

//#else
					//��üũ
					if((sinChar->Money - (haMercenrayMoney[haMercenrayIndex-1])*sMessageBox2[i].Count) < 0){
//#endif	
						sMessageBox2[i].Count-=1;
						cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY); //������
						break;
					}
					if(cSinSiege.MercenaryNum[haMercenrayIndex-1]+sMessageBox2[i].Count > 20){
						sMessageBox2[i].Count-=1;
						ShowMessage(MESSAGE_MERCENRAY_OVER);
						break;
					}
					if(sinChar->Money <= sMessageBox2[i].Count*(int)1){
						sMessageBox2[i].Count-=1;
						ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
						//CloseMessage(); //�޼����ڽ����ݾ��ش� 
					}
					
				}
				if(i == MESSAGE_MYSHOP_POTION|| i == MESSAGE_SIEGE_GET_MONEY){ 
					//��
					if(sinChar->Money <= sMessageBox2[i].Count*(int)MyShopPotion.ItemHeader.dwVersion){
						sMessageBox2[i].Count = sinChar->Money/(int)MyShopPotion.ItemHeader.dwVersion;
					} 
					//���� 
					if(sinChar->Weight[0]+sMessageBox2[i].Count > sinChar->Weight[1]){
						sMessageBox2[i].Count = sinChar->Weight[1]-sinChar->Weight[0];

					}
					//���� 
					if((int)MyShopPotion.PotionCount < sMessageBox2[i].Count){
						sMessageBox2[i].Count = (int)MyShopPotion.PotionCount;

					}
					/*
					//�� �ѵ� �ʰ� 
					if(!cInvenTory.CheckMoneyLimit(sMessageBox2[i].Count*(int)MyShopPotion.ItemHeader.dwVersion)){
						while(1){
							sMessageBox2[i].Count--;
							if(cInvenTory.CheckMoneyLimit(sMessageBox2[i].Count*(int)MyShopPotion.ItemHeader.dwVersion))
								break;
							if(sMessageBox2[i].Count <= 0){
								sMessageBox2[i].Count = 0;
								break;
							}
						}
					}
					*/
				}

				if(i == MESSAGE_BUY_STAR){
					/*���� �Ǹ����� �ʴ´�.*/
					//if(sinChar->Money <= sMessageBox2[i].Count*100000){
					//	sMessageBox2[i].Count = sinChar->Money/100000;
					//}
					sMessageBox2[i].Count = 0;
				}

				if(i == MESSAGE_MOVE_MONEY_MOVE) { //â�� ���� �ű涧 
					if(sinChar->Money < sMessageBox2[i].Count){
						sMessageBox2[i].Count = sinChar->Money;
					}

					if( (sWareHouse.Money-2023) + sMessageBox2[i].Count > 100000000 ){
						sMessageBox2[i].Count -= ((sWareHouse.Money-2023) + sMessageBox2[i].Count) - 100000000;
					}

				}

				if( i == MESSAGE_MOVE_MONEY_RECV){
					if(sWareHouse.Money-2023 < sMessageBox2[i].Count)
						sMessageBox2[i].Count = sWareHouse.Money-2023;
					if(sinChar->Level <= 10){
						if(sinChar->Money+sMessageBox2[i].Count > 200000){
							sMessageBox2[i].Count = 200000 - sinChar->Money;
							if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;

						}
					}
					else{
						switch(sinChar->ChangeJob){
							case 0:
								if((sinChar->Money + sMessageBox2[i].Count) > (sinChar->Level*200000)-1800000){
									sMessageBox2[i].Count = (sinChar->Level*200000)-1800000 - sinChar->Money;
									if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;

								}
							break;

							case 1:
								if( ( sinChar->Money + sMessageBox2[i].Count) > 10000000 ){
									sMessageBox2[i].Count = 10000000 - sinChar->Money;
									if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;
								}
							break;
							case 2:
								if( ( sinChar->Money + sMessageBox2[i].Count) > 50000000 ){
									sMessageBox2[i].Count = 50000000 - sinChar->Money;
									if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;
								}
							break;
							//4������(�ӽ�)
							case 3:
								if( ( sinChar->Money + sMessageBox2[i].Count) > 100000000 ){
									sMessageBox2[i].Count = 100000000 - sinChar->Money;
									if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;
								}


							break;


						}
					}

				}
				
				if(i == MESSAGE_EVENT_SMASHTV){//��α� 
					if(sinChar->Money < sMessageBox2[i].Count){
						sMessageBox2[i].Count = sinChar->Money;
						if(sMessageBox2[i].Count < (sinChar->Level * SMASHTV_MONEY)){
							ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
							CloseMessage(); //�޼����ڽ����ݾ��ش� 


						}
					}
					if( sMessageBox2[i].Count > (sinChar->Level * SMASHTV_MONEY)){ 
						sMessageBox2[i].Count = (sinChar->Level * SMASHTV_MONEY);
					}
				}

				if(i == 2 || i == 1){ //������ �����⳪ �ȱ��̸� 
					if(pMessageItem->sItemInfo.PotionCount < sMessageBox2[i].Count){
							sMessageBox2[i].Count = pMessageItem->sItemInfo.PotionCount;
					}
				}
				if(i==0){//������ ����̸� 
					CountTemp = sMessageBox2[i].Count*pMessageItem->sItemInfo.Price;
					if(CountTemp > sinChar->Money){
						ShowMessage(MESSAGE_NOT_ENOUGH_MONEY); //�޼����� �����ش� 
						for(int k=0; k < CountTemp ; k++){
							if((CountTemp-k) <= sinChar->Money){
								sMessageBox2[i].Count =(CountTemp-k)/pMessageItem->sItemInfo.Price;
								break;
							}

						}

					}
					CountTemp = (sMessageBox2[i].Count*pMessageItem->sItemInfo.Weight)+sinChar->Weight[0];
					if(sinChar->Weight[1] < CountTemp){ //���Ը� �Ѿ��
						ShowMessage(MESSAGE_OVER_WEIGHT); //�޼����� �����ش� 
						for(int p=0; p < CountTemp ; p++){
							if((CountTemp-p) <= sinChar->Weight[1]){
								sMessageBox2[i].Count -=p;
								break;
							}
						}
					}
				}
			}
			if(MessageArrowPosi == 2){
				if(i != MESSAGE_BUY_ITEM_DEFALT){
					sMessageBox2[i].Count --;
					if(sMessageBox2[i].Count < 0){
						sMessageBox2[i].Count = 0;
					}
				}
			}
			if(MessageButtonPosi == 1){ //Ȯ�� 
				CheckOkMessageBox(i);
			}
			if(MessageButtonPosi == 2){ //��� 
				sinMessageBoxShowFlag  = 0;
				sMessageBox2[i].Flag   = 0;
				cSinSod2.ClanMasterMessageBoxFlag = 0;
				if( i == MESSAGE_MOVE_MONEY_TRADE)
					sTrade.Money = 0;
				//���� ��ħ ����.
				//if(i == MESSAGE_THROW_ITEM)
				//	sinThrowItemToFeild(&MouseItem);//�������� ������
			}
		}
	}
}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::LButtonUp(int x , int y)
{

	//��ų ����Ʈ �ʱ�ȭ �ڽ��� ���� ( �̺�Ʈ�� �ߺ����� ó���Ǽ��� ��¦ ����)
	if(sinSkillPontInitButtonFlag){ 
		sinSkillPontInitButtonFlag = 0;
		cMessageBox.ShowMessage3(MESSAGE_INIT_SKILL_POINT,sinSkillPointName); 
	}

}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::RButtonDown(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::RButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	
void cMESSAGEBOX::KeyDown()
{
	int i = 0;

	char szKeyBuffPad[10]={VK_NUMPAD0,VK_NUMPAD1,VK_NUMPAD2,VK_NUMPAD3,VK_NUMPAD4,
		VK_NUMPAD5,VK_NUMPAD6,VK_NUMPAD7,VK_NUMPAD8,VK_NUMPAD9};
	char szKeyBuff[10]={'0','1','2','3','4','5','6','7','8','9'}; //Ű���� 

	for(i = 0 ; i < 20 ; i++){  
   		if(sMessageBox2[i].Flag){ 
			for(int j=0; j < 10 ; j++){
				if(sinGetKeyClick(szKeyBuff[j])||sinGetKeyClick(szKeyBuffPad[j])){
					if(i == 3 || i ==MESSAGE_MOVE_MONEY_TRADE || i == MESSAGE_MOVE_MONEY_MOVE || i ==MESSAGE_GIVE_MONEY){  //���ϰ�쿡 
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount);
						if(sinChar->Money < sMessageBox2[i].Count){
							sMessageBox2[i].Count = sinChar->Money;
						}
					}
					if(i == MESSAGE_SIEGE_GET_MONEY){//<ha>������ ��ã��
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount); 
						//ã�� ���� 0�̸� ã���� ����.
						if((int)cSinSiege.TotalTax <= 0){
							cMessageBox.ShowMessage(MESSAGE_NOT_CASTLE_TOTALMONEY);
							break;
						}
						if(cInvenTory.CheckMoneyLimit(sMessageBox2[i].Count)){ 
							if(sMessageBox2[i].Count > (int)cSinSiege.TotalTax){
								sMessageBox2[i].Count = (int)cSinSiege.TotalTax;
							}
						}
						else
							//sMessageBox2[i].Count = (int)cSinSiege.TotalTax;
						    sMessageBox2[i].Count /= 10;
				
					}
					/*
					if(i == MESSAGE_SIEGE_SET_MERCENRAY){
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount);
						if(cSinSiege.MercenaryNum[haMercenrayIndex-1]+sMessageBox2[i].Count > 20){
							sMessageBox2[i].Count = 0;
						}

					}
					*/
					
		
 					if (i == MESSAGE_SOD2_GET_MONEY ){
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount); 
						if(cInvenTory.CheckMoneyLimit(sMessageBox2[i].Count)){ 
							if(sMessageBox2[i].Count > (int)cSinSod2.ClanMoney){
								sMessageBox2[i].Count = (int)cSinSod2.ClanMoney;
							}
						}
						else
							sMessageBox2[i].Count /= 10;
					}

					if(i == MESSAGE_MYSHOP_POTION){ 
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount);

						//����
						if(sinChar->Money <= sMessageBox2[i].Count*(int)MyShopPotion.ItemHeader.dwVersion){
							sMessageBox2[i].Count = sinChar->Money/(int)MyShopPotion.ItemHeader.dwVersion;
						}
						//���� 
						if(sinChar->Weight[0]+sMessageBox2[i].Count > sinChar->Weight[1]){
							sMessageBox2[i].Count = sinChar->Weight[1]-sinChar->Weight[0];

						}
						//���� 
						if((int)MyShopPotion.PotionCount < sMessageBox2[i].Count){
							sMessageBox2[i].Count = (int)MyShopPotion.PotionCount;

						}
						//�� �ѵ� �ʰ� 
						/*
						if(!cInvenTory.CheckMoneyLimit(sMessageBox2[i].Count*(int)MyShopPotion.ItemHeader.dwVersion)){
							while(1){
								sMessageBox2[i].Count--;
								if(cInvenTory.CheckMoneyLimit(sMessageBox2[i].Count*(int)MyShopPotion.ItemHeader.dwVersion))
									break;
								if(sMessageBox2[i].Count <= 0){
									sMessageBox2[i].Count = 0;
									break;
								}
							}
						}
						*/
					}

					if(i == MESSAGE_BUY_STAR){
						/*���� �Ǹ����� �ʴ´�.*/
						//if(sinChar->Money <= sMessageBox2[i].Count*100000){
						//	sMessageBox2[i].Count = sinChar->Money/100000;
						//}
						sMessageBox2[i].Count = 0;
					}

					if(i == MESSAGE_MOVE_MONEY_MOVE) { //â�� ���� �ű涧 
						if(sinChar->Money < sMessageBox2[i].Count){
							sMessageBox2[i].Count = sinChar->Money;
						}

						if( (sWareHouse.Money-2023) + sMessageBox2[i].Count > 100000000 ){
							sMessageBox2[i].Count -= ((sWareHouse.Money-2023) + sMessageBox2[i].Count) - 100000000;
						}

					}
					//���� ���
					if( i == MESSAGE_MYSHOP_ITEM){    
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount);

						if(MyShopMouseItem.Class == ITEM_CLASS_POTION){
							if(!cInvenTory.CheckMoneyLimit(cMyShop.GetTotalMyShopItemMoney()+(sMessageBox2[i].Count*MyShopMouseItem.sItemInfo.PotionCount))){
								while(1){
									sMessageBox2[i].Count /= 10;
									if(cInvenTory.CheckMoneyLimit(cMyShop.GetTotalMyShopItemMoney()+(sMessageBox2[i].Count*MyShopMouseItem.sItemInfo.PotionCount))){
										ShowMessage(MESSAGE_OVER_MONEY);
										break;
									}
									if(sMessageBox2[i].Count <= 0 ){
										sMessageBox2[i].Count = 0;
										ShowMessage(MESSAGE_OVER_MONEY);
										break;
									}
								}
							}
						}
						else{
							if(!cInvenTory.CheckMoneyLimit(cMyShop.GetTotalMyShopItemMoney()+sMessageBox2[i].Count)){
								while(1){
									sMessageBox2[i].Count /= 10;
									if(cInvenTory.CheckMoneyLimit(cMyShop.GetTotalMyShopItemMoney()+sMessageBox2[i].Count)){
										ShowMessage(MESSAGE_OVER_MONEY);
										break;
									}
									if(sMessageBox2[i].Count <= 0 ){
										sMessageBox2[i].Count = 0;
										ShowMessage(MESSAGE_OVER_MONEY);
										break;
									}
								}
							}

						}
					}


					if( i == MESSAGE_MOVE_MONEY_RECV){
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount);
						if(sWareHouse.Money-2023 < sMessageBox2[i].Count)
							sMessageBox2[i].Count = sWareHouse.Money-2023;
						if(sinChar->Level <= 10){
							if(sinChar->Money+sMessageBox2[i].Count > 200000){
								sMessageBox2[i].Count = 200000 - sinChar->Money;
								if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;

							}
						}
						else{
							switch(sinChar->ChangeJob){
								case 0:
									if((sinChar->Money + sMessageBox2[i].Count) > (sinChar->Level*200000)-1800000){
										sMessageBox2[i].Count = (sinChar->Level*200000)-1800000 - sinChar->Money;
										if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;

									}
								break;
								case 1:
									if( ( sinChar->Money + sMessageBox2[i].Count) > 10000000 ){
										sMessageBox2[i].Count = 10000000 - sinChar->Money;
										if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;
									}
								break;
								case 2:
									if( ( sinChar->Money + sMessageBox2[i].Count) > 50000000 ){
										sMessageBox2[i].Count = 50000000 - sinChar->Money;
										if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;
									}
								break;
								//4������(�ӽ�)
								case 3:
									if( ( sinChar->Money + sMessageBox2[i].Count) > 100000000 ){
										sMessageBox2[i].Count = 100000000 - sinChar->Money;
										if(sMessageBox2[i].Count < 0 ) sMessageBox2[i].Count = 0;
									}
								break;

							}

						}

					}
					
					if(i == MESSAGE_EVENT_SMASHTV){//���Ž� TV
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount);
						if(sinChar->Money < sMessageBox2[i].Count){
							sMessageBox2[i].Count = sinChar->Money;
							if(sMessageBox2[i].Count < (sinChar->Level * SMASHTV_MONEY)){
								ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
								CloseMessage(); //�޼����ڽ����ݾ��ش� 
							}
						}
						if( sMessageBox2[i].Count > (sinChar->Level * SMASHTV_MONEY)){ 
							sMessageBox2[i].Count = (sinChar->Level * SMASHTV_MONEY);

						}
		
					}

					if(i == 2 || i ==1 ){//���� �ȱ⳪ �������ϰ�쿡 
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount); 
						if(pMessageItem && sMessageBox2[i].Count >=  pMessageItem->sItemInfo.PotionCount )
							sMessageBox2[i].Count = pMessageItem->sItemInfo.PotionCount;
					}
					if(i ==0 ){ //���� ����ϰ�� 
						sMessageBox2[i].szCount[sMessageBox2[i].Line]=szKeyBuff[j];
						sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount);
						CountTemp = sMessageBox2[i].Count*pMessageItem->sItemInfo.Price;
						if(CountTemp > sinChar->Money){
							for(int k=0; k < CountTemp ; k++){
								if((CountTemp-k) <= sinChar->Money){
									sMessageBox2[i].Count =(CountTemp-k)/pMessageItem->sItemInfo.Price;
									break;
								}
							}
						}

						CountTemp = (sMessageBox2[i].Count*pMessageItem->sItemInfo.Weight)+sinChar->Weight[0];
						if(sinChar->Weight[1] < CountTemp){ //���Ը� �Ѿ��
							for(int p=0; p < CountTemp ; p++){
								if((CountTemp-p) <= sinChar->Weight[1]){
									sMessageBox2[i].Count -=p;
									break;
								}
							}
						}
					}
				}	
			}
			if(sinGetKeyClick(VK_BACK)){
				if(i !=MESSAGE_BUY_ITEM_DEFALT){
					if(sMessageBox2[i].Count <=0)break;
					sMessageBox2[i].szCount[sMessageBox2[i].Line-1]=0;
					sMessageBox2[i].Count = atoi(sMessageBox2[i].szCount);
				}
			}
			if(sinGetKeyClick(VK_RETURN)){ //Ȯ�� ��ư 
				CheckOkMessageBox(i);
			}
		}

	}
	if(sinGetKeyClick(VK_ESCAPE)){
		for(int i = 0 ; i < 20 ; i++){
			if(sMessageBox2[i].Flag  ){
				sMessageBox2[i].Flag = 0;
				sinMessageBoxShowFlag = 0;
			
			}

		}
		cSinSod2.ClanMasterMessageBoxFlag = 0;
		
		/////////// â�� �ݾ��ش�  (�����Ҹ� ����Ҽ��ִ� �ȵ�!!!!)
		/*
		if(cWareHouse.OpenFlag){
			cWareHouse.OpenFlag = 0;
			cWareHouse.CloseWareHouse();
		}
		*/

	}
	//////////��ų ���� �޼��� 
	for( i = 0 ; i < MESSAGE3_MAX ; i++){
		if(sMessageBox3[i].Flag){
			if(sinGetKeyClick(VK_RETURN)){ //Ȯ�� ��ư 
				CheckOkMessageBox2(i);
				//LearnSkillEffect(0);
			}

		}
	}
}
/*----------------------------------------------------------------------------*
*				�̺�Ʈ�� �޼����� �����ش�  
*-----------------------------------------------------------------------------*/	
int cMESSAGEBOX::ShowMessageEvent(char *Msg)
{ 
	for(int i = 0 ; i < 49 ; i++){ //�̺�Ʈ ���� �޼����� �����ϰ� �������ش�
		if(sMessageBox[i].Flag)
			sMessageBox[i].Flag = 0;
	}

	sMessageBox[49].Flag = 1; //�÷��� �ش� 
	lstrcpy(sMessageBox[49].MessageDoc,Msg);
	sMessageBox[49].len = lstrlen(Msg);
	sMessageBox[49].x = 400-((sMessageBox[49].len*10)/2)+40; 
	sMessageBox[49].y = 5; 
	sMessageBox[49].Line = (sMessageBox[49].len*10/16)-6;
#ifdef _LANGUAGE_ENGLISH
	sMessageBox[49].Line = (sMessageBox[49].len*10/16)-3;
#endif
#ifdef _LANGUAGE_JAPANESE
	sMessageBox[49].Line = (sMessageBox[49].len*10/16)-4;
#endif
	return TRUE;
}
/*----------------------------------------------------------------------------*
*		�޼��� �ڽ� ȣ�� (�������� ������ ) *�޼����� �����ְų� *������ְų� 
*-----------------------------------------------------------------------------*/	
int cMESSAGEBOX::ShowMessage(int Kind)   
{ //�޼����� �����ش� 
	for(int i = 0 ; i < 100 ; i++){
		if(sMessageBox[i].Flag)
			sMessageBox[i].Flag = 0;
	}
	sMessageBox[Kind].Flag = 1; //�÷��� �ش� 
	if(Kind == MESSAGE_100LVITEM_OK){
		//�̺�Ʈ �޼����� ������ �̸��� �ֱ����� TempBuff���
		lstrcpy(sMessageBox[Kind].MessageDoc2,ha100LVQuestItemName[chaQuest.iFuryQuestIndex[1]-1]);
		lstrcat(sMessageBox[Kind].MessageDoc2,sMessageBox[Kind].MessageDoc);
		sMessageBox[Kind].len = lstrlen(sMessageBox[Kind].MessageDoc2);		
	}
	else
		sMessageBox[Kind].len = lstrlen(sMessageBox[Kind].MessageDoc);
	if(Kind == MESSAGE_MOVE_TRADE_ITEM ||Kind == MESSAGE_NOTEXIT_ITEMIMAGE ){	//������ �޼����� ��� �����ش� ,���� �����޼�����
		sMessageBox[Kind].x = 400-((sMessageBox[Kind].len*10)/2)+40; 
		sMessageBox[Kind].y = 300-(64/2); 
		sMessageBox[Kind].Line = (sMessageBox[Kind].len*10/16)-6;
#ifdef _LANGUAGE_ENGLISH
		sMessageBox[Kind].Line = (sMessageBox[Kind].len*10/16)-3;
#endif
#ifdef _LANGUAGE_JAPANESE
	sMessageBox[Kind].Line = (sMessageBox[Kind].len*10/16)-4;
#endif
	}
	else{
		sMessageBox[Kind].x = 400-((sMessageBox[Kind].len*10)/2)+40; 
		//sMessageBox[Kind].y = 300-(64/2); 
		sMessageBox[Kind].y = 5; 
		sMessageBox[Kind].Line = (sMessageBox[Kind].len*10/16)-6;
#ifdef _LANGUAGE_ENGLISH
		sMessageBox[Kind].Line = (sMessageBox[Kind].len*10/16)-3;
#endif
#ifdef _LANGUAGE_JAPANESE
	sMessageBox[Kind].Line = (sMessageBox[Kind].len*10/16)-4;
#endif
	}
	
	return TRUE;
}

int cMESSAGEBOX::ShowMessage2(int Kind)   
{ //����� �ִ� �޼��� �ڽ� 
	
	for(int i = 0 ; i < 20 ; i++){
		if(sMessageBox2[i].Flag)
			sMessageBox2[i].Flag = 0;
	}


	sMessageBox2[Kind].Flag = 1;   //�÷��� �ش� 
	memset(sMessageBox2[Kind].szCount,0,sizeof(sMessageBox2[Kind].szCount));
	if(Kind == MESSAGE_BUY_ITEM_DEFALT)
		sMessageBox2[Kind].Count = 1; //ī��Ʈ�� 1�� �ش�  (�׳� ������ ����ϰ�� )
	else if(Kind == 1)
		sMessageBox2[Kind].Count = pMessageItem->sItemInfo.PotionCount; //�ȼ��ִ� �������� �ִ�ġ 

	else
		sMessageBox2[Kind].Count = 0; //ī��Ʈ�� 0�� �ش� 

	if(MESSAGE_MYSHOP_ITEM == Kind){
		if(MyShopMouseItem.Class == ITEM_CLASS_POTION){
			lstrcpy(sMessageBox2[Kind].MessageDoc1,PotionCntDoc2);
		}
		else{
			lstrcpy(sMessageBox2[Kind].MessageDoc1,ItemPrice7);

		}
	}

	sMessageBox2[Kind].x    = 300;  
	sMessageBox2[Kind].y    = 200; 
	sMessageBox2[Kind].Line = 1; //���� 
	sinMessageBoxShowFlag   = 1;
	hFocusWnd = 0; //�޼��� �ڽ��� ������ ä�� ��Ŀ���� ���δ� 

	//���޼��� �ڽ��� �߸� �������ش� 
	if(Kind == MESSAGE_MOVE_MONEY_TRADE){
		if(sTrade.Money){
			if(!CheckTrade((void *)&sTrade)) //�߸��� ����Ÿ �̸� �ŷ�â�� �ݴ´�
				cTrade.CancelTradeItem();
			CheckCharForm();//���� 
			TempShowMoney = 0; //�ʱ�ȭ  
			//sinChar->Money += sTrade.Money-193;
			sinPlusMoney(sTrade.Money-193);
			sTrade.Money = 0;
			ReformCharForm();//������ 	
			SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
			ReformTrade((void *)&sTrade);
			SendTradeItem(cTrade.TradeCharCode);
		}
	}
	
	return TRUE;
}

//�޼����� ÷���� �޼��� �ڽ��� �����ش� 
int cMESSAGEBOX::ShowMessage3(int Kind , char *Doc) 
{
	int i=0; 
	for(i = 0 ; i < 20 ; i++){ 
		if(sMessageBox2[i].Flag)
			sMessageBox2[i].Flag = 0; 
	}
	for(i = 0 ; i < MESSAGE3_MAX ; i++){
		if(sMessageBox3[i].Flag)
			sMessageBox3[i].Flag = 0;
	}
	sMessageBox3[Kind].Flag = 1;   //�÷��� �ش� 
	sMessageBox3[Kind].x    = 300;  
	sMessageBox3[Kind].y    = 200; 
	sinMessageBoxShowFlag   = 1;
	lstrcpy(sMessageBox3[Kind].RecvMessage,Doc);
	
	if( Kind == MESSAGE_FALLGAME){
		sMessageBox3[Kind].x += 120;
		sMessageBox3[Kind].y += 100;
	}
	if(Kind == MESSAGE_INIT_SKILL_POINT){ //�߸� ���� -_-
		sMessageBox3[Kind].x +=10;

	}
	if(Kind == MESSAGE_EVENT_GYUNGFUM){  
		sMessageBox3[Kind].x +=10;
		//sMessageBox3[Kind].y 
		sinInitPointFlag7 = 1;
		cSinHelp.sinShowHelp(SIN_HELP_KIND_INITPOINT,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x-3,QuestMessageBoxSize2.y-1, RGBA_MAKE(0,15,128,125),WarningInitPoint[0]);
	}
	if(Kind == MESSAGE_SECRET_NUM){
		hFocusWnd = hTextWnd;  
		SetWindowText( hFocusWnd , "");
		cInterFace.ChatFlag = 0;
		
	}

	if( Kind == MESSAGE_TELEPORT_DUNGEON ) // pluto �߰����� �޼���
	{
		sMessageBox3[Kind].x += 120;
		sMessageBox3[Kind].y += 100;
	}
	return TRUE;
}


int cMESSAGEBOX::CloseMessage()
{//�޼��� �ڽ��� �ݾ��ش� 
	for(int i = 0 ; i < 20 ; i++){
		if(sMessageBox2[i].Flag){
			sMessageBox2[i].Flag = 0;
			memset(sMessageBox2[i].szCount,0,sizeof(sMessageBox2[i].szCount));
			sMessageBox2[i].Count = 0;
		}
	}
	sinMessageBoxShowFlag = 0;
	

	return TRUE;
}

//�޼��� �ڽ��� ������ �����ش� 
void cMESSAGEBOX::DrawMessageText()
{


	HDC	hdc;
	int TempFlag = 0;
	char szTemp2[256];
	for(int i = 0 ; i < 100 ; i++){
		if(sMessageBox[i].Flag){
			TempFlag = 1;
		}
	}


	int len = 0;
	int XPosi = 0;
	char szBuffDoc[128];
	char szBuffDoc2[128];
	int Yposi = 0; 
	int PassNum =0; 

	//lstrcpy(sMessageBox_RecvItem.szDoc,"�ڻ󿭸��� ���� ��������1����1��������������������  ioi ");
	//������ �ù輭��  ���� 256 
	//sinBoldFont

	char HoldStarNum[32];   
	//��������
	if(sStarShop.Flag)
	{                        
		renderDevice.lpDDSBack->GetDC(&hdc);
		SelectObject( hdc, sinBoldFont);
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, RGB(222,231,255) );
		dsTextLineOut(hdc,BackStartPos.x +48,BackStartPos.y+35,HoldStarNumDoc,lstrlen(HoldStarNumDoc));

		wsprintf(HoldStarNum,"%s","x");
		dsTextLineOut(hdc,BackStartPos.x +120,BackStartPos.y+75,HoldStarNum,lstrlen(HoldStarNum));

		wsprintf(HoldStarNum,"%d",sStarShop.Count);
		dsTextLineOut(hdc,BackStartPos.x +160,BackStartPos.y+75,HoldStarNum,lstrlen(HoldStarNum));

		dsTextLineOut(hdc,BackStartPos.x +83,BackStartPos.y+140,StarItemBuy7,lstrlen(StarItemBuy7));

		SelectObject( hdc, sinFont);
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, RGB(222,231,25) );
		dsTextLineOut(hdc,BackStartPos.x +53,BackStartPos.y+120,ChangeMoney7,lstrlen(ChangeMoney7));
		renderDevice.lpDDSBack->ReleaseDC(hdc);

	}

	char szTempBuff5[128];
	if(sLost_Item.Flag)
	{ 
		renderDevice.lpDDSBack->GetDC(&hdc);
		SelectObject( hdc, sinMessageFont);
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, RGB(128,255,128) );
		if(OverDay_Item_Flag){
			wsprintf(szTempBuff5,"%s %s",sLost_Item.szDoc,ItemName87);
			dsTextLineOut(hdc,BackStartPos.x +110,BackStartPos.y+90,szTempBuff5,lstrlen(szTempBuff5));

		}
		else
		{
			dsTextLineOut(hdc,BackStartPos.x +110,BackStartPos.y+90,Quset3ItemDoc15,lstrlen(Quset3ItemDoc15));
			renderDevice.lpDDSBack->ReleaseDC(hdc);
		}

		renderDevice.lpDDSBack->GetDC(&hdc);
		SelectObject( hdc, sinMessageFont);
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, RGB(255,255,255) );
		dsTextLineOut(hdc,BackStartPos.x +110,BackStartPos.y+120,Quset3ItemDoc16,lstrlen(Quset3ItemDoc16));
		renderDevice.lpDDSBack->ReleaseDC(hdc);

	}

	if(sMessageBox_RecvItem.Flag)
	{        
		renderDevice.lpDDSBack->GetDC(&hdc);
		SelectObject( hdc, sinBoldFont);
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, RGB(222,231,255) ); 
		memset(szBuffDoc,0,sizeof(szBuffDoc));
		memset(szBuffDoc2,0,sizeof(szBuffDoc2));

		len = lstrlen(sMessageBox_RecvItem.szDoc); 
		if((len*7) > 256){
			if((BYTE)sMessageBox_RecvItem.szDoc[256/7] & 0x80)
				PassNum = 0;
			else
				PassNum = 1;

			memcpy(szBuffDoc,sMessageBox_RecvItem.szDoc,(256/7)+PassNum);
			lstrcpy(szBuffDoc2,&sMessageBox_RecvItem.szDoc[(256/7)+PassNum]);
			dsTextLineOut(hdc,BackStartPos.x +10,BackStartPos.y+25,szBuffDoc,lstrlen(szBuffDoc));
			dsTextLineOut(hdc,BackStartPos.x +10,BackStartPos.y+40,szBuffDoc2,lstrlen(szBuffDoc2));
		}
		else{
			XPosi = ((256-(len*6))/2);
			if(XPosi <10 )XPosi = 10;
			dsTextLineOut(hdc,BackStartPos.x +XPosi,BackStartPos.y+30,sMessageBox_RecvItem.szDoc,lstrlen(sMessageBox_RecvItem.szDoc));
		}
		renderDevice.lpDDSBack->ReleaseDC(hdc);

		//��� ��
		if(sMessageBox_RecvItem.RecvItem.SellPrice)
		{
			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc, sinMessageFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(128,255,128) );
			wsprintf(szBuffDoc,"%d",sMessageBox_RecvItem.RecvItem.SellPrice);
			dsTextLineOut(hdc,BackStartPos.x +120,BackStartPos.y+90,szBuffDoc,lstrlen(szBuffDoc));
			renderDevice.lpDDSBack->ReleaseDC(hdc);

			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc, sinMessageFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(255,255,255) );
			dsTextLineOut(hdc,BackStartPos.x +130,BackStartPos.y+120,Money5,lstrlen(Money5));
			renderDevice.lpDDSBack->ReleaseDC(hdc);


		}
		//��� ����ġ
		else if(sMessageBox_RecvItem.RecvItem.OldX)
		{
			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc, sinMessageFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(128,255,128) );
			wsprintf(szBuffDoc,"%d",sMessageBox_RecvItem.RecvItem.OldX);
			dsTextLineOut(hdc,BackStartPos.x +120,BackStartPos.y+90,szBuffDoc,lstrlen(szBuffDoc));
			renderDevice.lpDDSBack->ReleaseDC(hdc);

			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc, sinMessageFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(255,255,255) );
			dsTextLineOut(hdc,BackStartPos.x +120,BackStartPos.y+120,Exp5,lstrlen(Exp5));
			renderDevice.lpDDSBack->ReleaseDC(hdc);

		}
		//��� ����
		else if(sMessageBox_RecvItem.RecvItem.PotionCount)
		{
			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc, sinMessageFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(128,255,128) );
			wsprintf(szBuffDoc,"%d",sMessageBox_RecvItem.RecvItem.PotionCount);
			dsTextLineOut(hdc,BackStartPos.x +120,BackStartPos.y+90,szBuffDoc,lstrlen(szBuffDoc));
			renderDevice.lpDDSBack->ReleaseDC(hdc);

			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc, sinMessageFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(255,255,255) );
			dsTextLineOut(hdc,BackStartPos.x +120,BackStartPos.y+120,RecvPotion7,lstrlen(RecvPotion7));
			renderDevice.lpDDSBack->ReleaseDC(hdc);


		}
		//��� ������ 
		else
		{
			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc, sinMessageFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(128,255,128) );
			dsTextLineOut(hdc,BackStartPos.x +110,BackStartPos.y+90,sMessageBox_RecvItem.szItem,lstrlen(sMessageBox_RecvItem.szItem));
			renderDevice.lpDDSBack->ReleaseDC(hdc);

			renderDevice.lpDDSBack->GetDC(&hdc);
			SelectObject( hdc, sinMessageFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(255,255,255) );
			dsTextLineOut(hdc,BackStartPos.x +120,BackStartPos.y+120,Itemul,lstrlen(Itemul));
			renderDevice.lpDDSBack->ReleaseDC(hdc);
		}

		renderDevice.lpDDSBack->GetDC(&hdc);
		SelectObject( hdc, sinMessageFont);
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, RGB(255,255,255) );
		dsTextLineOut(hdc,BackStartPos.x +110,BackStartPos.y+140,ItemRecvOk,lstrlen(ItemRecvOk));
		renderDevice.lpDDSBack->ReleaseDC(hdc);
		
	}
	
	if(sinMessageBoxShowFlag || TempFlag){

	
	int i=0;
	int TempLenght = 0;

	renderDevice.lpDDSBack->GetDC(&hdc);
	SelectObject( hdc, sinMessageFont);
    SetBkMode( hdc, TRANSPARENT );
	SetTextColor( hdc, RGB(255,255,255) );
	char szBuff5[128];
	memset(szBuff5,0,sizeof(szBuff5));


	for(i = 0 ; i < 100 ; i++){
		if(sMessageBox[i].Flag){ //��ġ��� ����� 
			if(i == MESSAGE_100LVITEM_OK){
				//�̺�Ʈ �޼����� ������ �̸��� �ֱ����� ���
				dsTextLineOut(hdc,sMessageBox[i].x+43,sMessageBox[i].y+17,sMessageBox[i].MessageDoc2,sMessageBox[i].len );
			}
			else
				dsTextLineOut(hdc,sMessageBox[i].x+43,sMessageBox[i].y+17,sMessageBox[i].MessageDoc,sMessageBox[i].len );
		}
	}

	char szBuff[128];
	char szBuff15[128];
	int len2 = 0;  
	memset(szBuff,0,sizeof(szBuff));
	memset(szBuff15,0,sizeof(szBuff15));
	
	for(i = 0 ; i < 20 ; i++){
		if(sMessageBox2[i].Flag){ //��ġ��� ����� 
			dsTextLineOut(hdc,sMessageBox2[i].x+45,sMessageBox2[i].y+15,sMessageBox2[i].MessageDoc1,lstrlen(sMessageBox2[i].MessageDoc1) );
			dsTextLineOut(hdc,sMessageBox2[i].x+45,sMessageBox2[i].y+40,sMessageBox2[i].MessageDoc2,lstrlen(sMessageBox2[i].MessageDoc2) );
	 		wsprintf(sMessageBox2[i].szCount,"%d",sMessageBox2[i].Count); 
			NumLineComa(sMessageBox2[i].Count, szBuff5);
			for(int t = 0 ; t < 10 ; t ++){
				if(CheckCountSize[t] > sMessageBox2[i].Count){ 
					sMessageBox2[i].Line = t;
					break;
				}
			}
			if(i == MESSAGE_MYSHOP_POTION){ //���ǻ���ϰ�쿡�� ������ ǥ�����ش�            
				NumLineComa(sMyShop_Server.Price*sMessageBox2[i].Count,szBuff);
				len2 = lstrlen(szBuff);
				//wsprintf(szBuff15,"(%s) %s",szBuff5,szBuff);
				sMessageBox2[i].TextSetX =  (int)(((sMessageBox2[i].x + 60)) -(((sMessageBox2[i].Line) * 6.5)/2));

				wsprintf(szBuff15,"%s %s",BuyPotionKind,szBuff5);
				len2 = lstrlen(szBuff15);
				dsTextLineOut(hdc,sMessageBox2[i].TextSetX-(len2),sMessageBox2[i].y+58,szBuff15,lstrlen(szBuff15) );	

				wsprintf(szBuff15,"%s %d",BuyPotionMoney,sMyShop_Server.Price*sMessageBox2[i].Count);
				len2 = lstrlen(szBuff15);
				dsTextLineOut(hdc,sMessageBox2[i].TextSetX-(len2),sMessageBox2[i].y+72,szBuff15,lstrlen(szBuff15) );	

				//dsTextLineOut(hdc,sMessageBox2[i].TextSetX,sMessageBox2[i].y+72,sMessageBox2[i].szCount,lstrlen(sMessageBox2[i].szCount) );	
//				dsTextLineOut(hdc,sMessageBox2[i].TextSetX,sMessageBox2[i].y+72,szBuff15,lstrlen(szBuff15) );	

			}
			else{
				len2 = lstrlen(szBuff5);
				sMessageBox2[i].TextSetX =  (int)((sMessageBox2[i].x + 85-(len2)) -(((sMessageBox2[i].Line) * 6.5)/2));
				//dsTextLineOut(hdc,sMessageBox2[i].TextSetX,sMessageBox2[i].y+72,sMessageBox2[i].szCount,lstrlen(sMessageBox2[i].szCount) );	
				dsTextLineOut(hdc,sMessageBox2[i].TextSetX,sMessageBox2[i].y+72,szBuff5,lstrlen(szBuff5) );	
			}
		}
	}
	for( i = 0 ; i < MESSAGE3_MAX ; i++){ 
		if(sMessageBox3[i].Flag){  
			TempLenght = (int)((172-(lstrlen(sMessageBox3[i].RecvMessage)*5.5))/2-5);
			SelectObject( hdc, sinBoldFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(222,231,255) );
			dsTextLineOut(hdc,sMessageBox3[i].x+TempLenght,sMessageBox3[i].y+20,sMessageBox3[i].RecvMessage,lstrlen(sMessageBox3[i].RecvMessage) );
			SelectObject( hdc, sinFont);
			SetBkMode( hdc, TRANSPARENT );
			SetTextColor( hdc, RGB(255,255,255) );
			TempLenght = (int)((172-(lstrlen(sMessageBox3[i].MessageDoc1)*5.5))/2-5);
			dsTextLineOut(hdc,sMessageBox3[i].x+TempLenght,sMessageBox3[i].y+35+8,sMessageBox3[i].MessageDoc1,lstrlen(sMessageBox3[i].MessageDoc1) );
			if( i == MESSAGE_EVENT_SMASHTV){ //�յ��� ��ġ�� ��,.��
				wsprintf(szTemp2,"%d%s",sinChar->Level*SMASHTV_MONEY , sMessageBox3[i].MessageDoc2);	
				dsTextLineOut(hdc,sMessageBox3[i].x+48,sMessageBox3[i].y+55+5,szTemp2,lstrlen(szTemp2) );
			}
			else if( i ==MESSAGE_SECRET_NUM){
				sinChatEnter = 0; //���Ƿ� �۾��� �Ⱥ��̰��Ѵ�
				cInterFace.ChatFlag = 0;
				hFocusWnd = hTextWnd;  
				GetWindowText( hFocusWnd , sMessageBox3[i].MessageDoc2 , 15 );
				dsTextLineOut(hdc,sMessageBox3[i].x+48,sMessageBox3[i].y+55+5,sMessageBox3[i].MessageDoc2,lstrlen(sMessageBox3[i].MessageDoc2) );

			}
			else{
				TempLenght = (int)((172-(lstrlen(sMessageBox3[i].MessageDoc2)*5.5))/2-5);
				dsTextLineOut(hdc,sMessageBox3[i].x+TempLenght+2,sMessageBox3[i].y+55+5,sMessageBox3[i].MessageDoc2,lstrlen(sMessageBox3[i].MessageDoc2) );
			}
		}
	}
	renderDevice.lpDDSBack->ReleaseDC(hdc);
	}

}
int sinTeleportIndexArray[4] = {0,18,7,12};
///�޼����� ���ϵ� �޼��� �ڽ� 
int cMESSAGEBOX::CheckOkMessageBox2(int Kind)
{

	int Temp = 0;
	int TempTalent = 0;
	sITEM TempItem;

	switch(Kind){    
		case MESSAGE_MASTER_SKILL:   //��ų ���� 
			if(pMasterSkill){
				//--------------------------------------------------------------------------//
				#ifdef HASIEGE_MODE
					//<ha>������ �������뿡 ��ų ����
					if(cShop.haBuyMoneyCheck(sinMasterSkill_Money[pMasterSkill->Skill_Info.SkillNum]+(sinMasterSkill_PlusMoney[pMasterSkill->Skill_Info.SkillNum]*(pMasterSkill->Point)))){
				#else
					if(	0 <=  (sinChar->Money - (sinMasterSkill_Money[pMasterSkill->Skill_Info.SkillNum]+(sinMasterSkill_PlusMoney[pMasterSkill->Skill_Info.SkillNum]*(pMasterSkill->Point))))){
				#endif
				//---------------------------------------------------------------------------//	
					CheckCharForm();//���� 
					//sinChar->Money -= sinMasterSkill_Money[pMasterSkill->Skill_Info.SkillNum]+(sinMasterSkill_PlusMoney[pMasterSkill->Skill_Info.SkillNum]*(pMasterSkill->Point));
					//<ha>������ ���������� ��ų �Ӵϸ� ������ ������.
					SendPaymentMoneyToServer((sinMasterSkill_Money[pMasterSkill->Skill_Info.SkillNum]+(sinMasterSkill_PlusMoney[pMasterSkill->Skill_Info.SkillNum]*(pMasterSkill->Point)))+
						((sinMasterSkill_Money[pMasterSkill->Skill_Info.SkillNum]+(sinMasterSkill_PlusMoney[pMasterSkill->Skill_Info.SkillNum]*(pMasterSkill->Point)))*cSinSiege.GetTaxRate())/100,1);

					
					sinMinusMoney(sinMasterSkill_Money[pMasterSkill->Skill_Info.SkillNum]+(sinMasterSkill_PlusMoney[pMasterSkill->Skill_Info.SkillNum]*(pMasterSkill->Point)),1);
					cSkill.CheckSkillPointForm(pMasterSkill);
					pMasterSkill->Point++;
					cSkill.ReformSkillPointForm(pMasterSkill);
					
					//4����ų�ϰ��
					if(Skill4ButtonIndex > 13){
						sinSkill.SkillPoint4--;
					}
					//1.2.3�� ��ų�ϰ��
					if(Skill4ButtonIndex <= 13){
						sinSkill.SkillPoint --;
					}
					
					ReformCharForm();//������ 
					SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
					sinPlaySound(SIN_SOUND_COIN);
					switch(pMasterSkill->CODE){
						case SKILL_POISON_ATTRIBUTE:	//���Ӽ� ���� 
							pUseSkill = pMasterSkill;
							pUseSkill->Skill_Info.FuncPointer();
							break;
						case SKILL_MELEE_MASTERY:       //������ ���� 
							pUseSkill = pMasterSkill;
							pUseSkill->Skill_Info.FuncPointer();
							break;
						case SKILL_FIRE_ATTRIBUTE:      //�ҼӼ� ���� 
							pUseSkill = pMasterSkill;
							pUseSkill->Skill_Info.FuncPointer();
							break;
						case SKILL_ICE_ATTRIBUTE:       //���� �Ӽ� ���� 
							pUseSkill = pMasterSkill;
							pUseSkill->Skill_Info.FuncPointer();
							break;
						case SKILL_SHOOTING_MASTERY:    //Ȱ ������ ����  
							pUseSkill = pMasterSkill;
							pUseSkill->Skill_Info.FuncPointer();
							break;
						case SKILL_MAXIMIZE:			//�ִ� ������ ���� 
							pUseSkill = pMasterSkill;
							pUseSkill->Skill_Info.FuncPointer();
							break;
						case SKILL_WEAPONE_DEFENCE_MASTERY:	//���� ���� 
							pUseSkill = pMasterSkill;
							pUseSkill->Skill_Info.FuncPointer();
							break;
						case SKILL_DIONS_EYE:	//���߷� ����  
							pUseSkill = pMasterSkill;
							pUseSkill->Skill_Info.FuncPointer();
							break;
						case SKILL_PHYSICAL_TRANING: //�ٷ·� �߰� 
							cInvenTory.SetItemToChar();
							break;
						case SKILL_FIRE_JAVELIN:
							cInvenTory.SetItemToChar();
							break;
						case SKILL_MEDITATION:
							cInvenTory.SetItemToChar();
							break;
						case SKILL_MENTAL_MASTERY:
							cInvenTory.SetItemToChar();
							break;
						case SKILL_THROWING_MASTERY:
							cInvenTory.SetItemToChar();
							break;
						case SKILL_MECHANIC_WEAPON:
							cInvenTory.SetItemToChar();
							break;
						case SKILL_CRITICAL_MASTERY:
							cInvenTory.SetItemToChar();
							break;
						case SKILL_SWORD_MASTERY:
							cInvenTory.SetItemToChar();
							break;
						case SKILL_BOOST_HEALTH:
							cInvenTory.SetItemToChar();
							break;
						case SKILL_EVASION_MASTERY:
							cInvenTory.SetItemToChar();
							break;

					};
					cInvenTory.CheckDamage(); //�������� üũ�ؼ� �����κ�����
					LearnSkillEffect(0);
					TempTalent = (int)(sinChar->Talent/3);   ////////���õ��� �����Ѵ� 
					if(TempTalent > 50 )TempTalent = 50;
					Temp = 10000 -  (TempTalent*100);
					
					if(Temp >=pMasterSkill->UseSkillCount)
						pMasterSkill->UseSkillCount /=2;
					else
						pMasterSkill->UseSkillCount = Temp/2;
					SaveGameData();

				}
				else
					ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);

			}
			
		break;
		case MESSAGE_CHANGE_JOB:
			if(sinQuest_ChangeJob3.CODE){ 
				TempItem.CODE = (sinOR2|sin02);
				if(!cInvenTory.CheckInvenEmpty(&TempItem)){
					ShowMessage(MESSAGE_OVER_SPACE);
					break;
				}

			}
			///////////////// �̰��� ����Ʈ ���� �޼����� ���´� 
			sinChar->ChangeJob++; //������ �Ѵ� 
			cSkill.OpenFlag = 1;
			cInterFace.CheckAllBox(SIN_SKILL); //â�� �ϳ��� ���� 
			SkillMasterFlag = 1;  //��ų�� �Ҵ��Ҽ��ְ� ���ش�

			if(sinQuest_ChangeJob.CODE){ //�۽�ũ��
				DeleteQuestItem((sinQT1|sin01)); //����Ʈ �������� �����ش�
				DeleteQuestItem((sinQT1|sin02));
				DeleteQuestItem((sinQT1|sin03));

				sinQuest_ChangeJob.State = SIN_QUEST_COMPLETE;
				sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob);
				lstrcpy(cInvenTory.JobName,sinJobList->szName2);
				
				sinQuest_ChangeJob.BackUpCode = sinQuest_ChangeJob.CODE;
				sinQuest_ChangeJob.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
				sinChar->wVersion[1]    = 0;
				EndQuest_Code(sinQuest_ChangeJob.BackUpCode);
				
				cInvenTory.ReFormInvenItem();
				sinSkillPontInitButtonFlag = 1; //��ų ����Ʈ �ʱ�ȭ �ڽ��� ���� 
			}
			if(FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB2_NPC_M) ||FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB2_NPC_D)){ //����̿�
				sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob);
				lstrcpy(cInvenTory.JobName,sinJobList->szName2);
				sinSkillPontInitButtonFlag = 1; //��ų ����Ʈ �ʱ�ȭ �ڽ��� ���� 
				//�̺κп��� ����Ʈ�ڵ���� �ʱ�ȭ�� ����
				sinQuest_ChangeJob2.CODE = 0;
				
			}
			if(sinQuest_ChangeJob3.CODE){ 
			//if(FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB3)){
				sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob);
				lstrcpy(cInvenTory.JobName,sinJobList->szName2);
				sinSkillPontInitButtonFlag = 1; //��ų ����Ʈ �ʱ�ȭ �ڽ��� ���� 
				//�̺κп��� ����Ʈ�ڵ���� �ʱ�ȭ�� ����
				//sinQuest_ChangeJob3.BackUpCode = sinQuest_ChangeJob3.CODE;
				//sinQuest_ChangeJob3.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
				//EndQuest_Code(sinQuest_ChangeJob3.BackUpCode);
				sinQuest_ChangeJob3.State = 5;
				StartQuest_Code(sinQuest_ChangeJob3.CODE);
			}

	
			SaveGameData(); //������ �����Ѵ�
			ChangeJobFace();
		break;
		case MESSAGE_QUEST:	 //����Ʈ ����޼��� ǥ�� 
			sinQuest_ChangeJob.CODE = SIN_QUEST_CODE_CHANGEJOB;
			sinQuest_ChangeJob.State = SIN_QUEST_PROGRESS;
			SaveGameData();
			StartQuest_Code(sinQuest_ChangeJob.CODE);
		break;
		case MESSAGE_QUEST_CHANGEJOB_MORAYOIN:
			sinQuest_ChangeJob2.State = SIN_QUEST_PROGRESS;
			// sinQuest_ChangeJob2.Kind = GetRandomPos(1,2); //��������Ʈ�� A,B�� �����Ѵ�
			//sinQuest_ChangeJob2.Kind = 2;
			SaveGameData();
			StartQuest_Code(sinQuest_ChangeJob2.CODE); //���õǴ� �κ��� ���߿� �ٽú����Ѵ� 
			switch(sinQuest_ChangeJob2.Kind){
				case 1:
				sinQuest_ChangeJob2.CODE = SIN_QUEST_CODE_CHANGEJOB2_NPC_M; //���� �ҸӴ� ��������Ʈ
				break;
				case 2:
				sinQuest_ChangeJob2.CODE = SIN_QUEST_CODE_CHANGEJOB2_NPC_D; //���� �ҸӴ� ��������Ʈ
				break;
			}
			SaveGameData();
			StartQuest_Code(sinQuest_ChangeJob2.CODE); //���õǴ� �κ��� ���߿� �ٽú����Ѵ� 
			//����Ʈ�������� �ִ����� üũ�ؼ� ���������� 
			cInvenTory.ClearQuestItem();
		break;
		case MESSAGE_QUEST_CHANGEJOB3: //3������ ����Ʈ 
			sinQuest_ChangeJob3.CODE  = SIN_QUEST_CODE_CHANGEJOB3;
			sinQuest_ChangeJob3.State = 1;
			SaveGameData();
			StartQuest_Code(sinQuest_ChangeJob3.CODE); //���õǴ� �κ��� ���߿� �ٽú����Ѵ� 
		break;

		case MESSAGE_INIT_SKILL_POINT: //��ų ����Ʈ �ʱ�ȭ 
			cSkill.InitSkillPoint();
		break;
		case MESSAGE_EVENT_GYUNGFUM: //�ɷ�ġ �ʱ�ȭ�� ����Ѵ� 
			cCharStatus.InitCharStatus();	 			
			cInvenTory.SetItemToChar();				
			if ( sinChar->Life[1]<sinGetLife() ) {
				sinSetLife( sinChar->Life[1] );
				ReformCharForm();//������ 
				ResetEnergyGraph(4);
			}
			sinInitPointFlag7 = 0;
			//cMessageBox.ShowMessage(MESSAGE_WARNING_WEIGHT);
			//sinSendEventItem(1); //�̺�Ʈ �������� �����κ����� �����Ѵ� 
		break;
		
		case MESSAGE_EVENT_SMASHTV: //�̺�Ʈ ���޽� TV 
			if(sinChar->Money < (sinChar->Level * SMASHTV_MONEY)){  
				CloseMessage(); //�޼����ڽ����ݾ��ش� 
				ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
				break;

			}
			CheckCharForm();//���� 
			//sinChar->Money -= sMessageBox2[Kind].Count;
			sinMinusMoney((sinChar->Level * SMASHTV_MONEY));
			ReformCharForm();//������ 	
			SendCollectMoney((sinChar->Level * SMASHTV_MONEY));
			sinPlaySound(SIN_SOUND_COIN);
			//ShowMessage(MESSAGE_GIVE_MONEY_TRUE);
			//��⼭ �̺�Ʈ�� �����ϸ� �ǰڽɴ� �ε�!
			
		break;
		case MESSAGE_POSTBOX:
			//���⼭ ������ �޴� �Լ��� ȣ���Ѵ� 
			//SendItemExpress( ExpressItemCode );
			/*
			if(sPostbox_Item[PostBoxIndexCnt+1].CODE){
				PostBoxIndexCnt++;
			}
			else PostBoxIndexCnt =0; //0���� �÷��� �ʱ�ȭ�ؼ� üũ�Ѵ� 
			*/
			

		break;
		case MESSAGE_LINKCORE:
			//cInvenTory.SetLinkCore(); //��ũ�ھ �����Ѵ�
			cInvenTory.UnionCoreParticle2(UnionCorePosi.x , UnionCorePosi.y); //����Ʈ 
			sinPlaySound(SIN_SOUND_EAT_POTION2); //���Ͽ��ھ� ����
			SendLinkCoreToServer(&TempLinkCoreItem);	

		break;
		case MESSAGE_RECVITEM:			//�������� ������ Ȯ��â
			SendWingQuestItem();
			sinbaram_Stop = 1;
			//WingItemQuestIndex
			//������ �������� ������ 
		break;

		case MESSAGE_WARP:
			if(sSinWarpGate.GateUseIndex > 2 && sSinWarpGate.MoveFieldNum >1 && !sSinWarpGate.SameAreaFlag ){ 
				CheckCharForm(); //���� 
				//<ha>������ ���������� ��������Ʈ �Ӵϸ� ������ ������.
				SendPaymentMoneyToServer(WarpGateUseCost[sSinWarpGate.GateUseIndex-4]+((WarpGateUseCost[sSinWarpGate.GateUseIndex-4]*cSinSiege.GetTaxRate())/100),2);
				sinMinusMoney(WarpGateUseCost[sSinWarpGate.GateUseIndex-4],1); //��������
				ReformCharForm(); //������ 
			}
			if(sHaWarpGate.GateUseIndex > 0 &&sHaWarpGate.MoveFieldNum >0&&!sSinWarpGate.SameAreaFlag ){
				CheckCharForm(); //���� 
				//<ha>������ ���������� ��������Ʈ �Ӵϸ� ������ ������.
				SendPaymentMoneyToServer(WarpGateUseCost[sSinWarpGate.GateUseIndex-4]+((WarpGateUseCost[sSinWarpGate.GateUseIndex-4]*cSinSiege.GetTaxRate())/100),2);
				sinMinusMoney(WarpGateUseCost[sSinWarpGate.GateUseIndex-4],1); //��������
				ReformCharForm(); //������ 
				WingWarpGate_Field(haWarpGateCODE[sHaWarpGate.MoveFieldNum-1]);	
			}
			else
				WingWarpGate_Field(sinWarpGateCODE[sSinWarpGate.MoveFieldNum]);

			SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

		break;
		case MESSAGE_SECRET_NUM:
			SendItemExpress(sMessageBox_RecvItem.RecvItem.CODE,sMessageBox3[Kind].MessageDoc2);
			isDrawClanMenu = 0;
			sMessageBox_RecvItem.Flag = 0;
			hFocusWnd = hTextWnd;
			SetWindowText(hFocusWnd,"");
			cInterFace.ChatFlag = 0;
		break;
		case MESSAGE_GIFT_TICKET:
			//GiftPrice (�縸ŭ ������)
			SendStarPointToServer(GiftPrice,0);
			DeleteGiftItem();
			StarShopNPC();
		break;
		case MESSAGE_SELL_HIGHRANK_ITEM:
			MouseItem.Flag = 1; //��¥�� ���� ��ȭȭȱ
			ResetInvenItemCode(); //���콺�� �÷��� ������ üũ���ش�
			cShop.SellItemToShop(&MouseItem);
			
		break; 
		case MESSAGE_GIVE_MONEY_CHECK:
			CheckCharForm();//���� 
			sinMinusMoney2(sinGiveMoneyAmount);
			ReformCharForm();//������ 	
			//���⼭ ������ ���� ������
			SendGiveMoneyToServer(sinGiveMoneyAmount);
			sinPlaySound(SIN_SOUND_COIN);
			sinGiveMoneyAmount = 0;

		break;
		case MESSAGE_PUZZLE_SEND:
			if(cInvenTory.CheckSendPuzzle()){ 
				SendWingQuestItem();
			}
		break;
		case MESSAGE_MYSHOP_ITEM_BUY:
			if(cCharShop.OpenFlag){
				Send_PersonalShopItem(sRecvMyShop.CHAR_CODE,&sMyShop_Server); //������ ������ �������� ������ ������
			}
		break;
		case MESSAGE_MYSHOP_ITEM_CANCEL:
			MyShopItemIndex[MyShopItemCancelIndex[0]-1] = 0; 
			cMyShop.MyShopItem[MyShopItemCancelIndex[1]-1].Flag = 0;
			memset(&cMyShop.MyShopItem[MyShopItemCancelIndex[1]-1],0,sizeof(sITEM));
		break;
		case MESSAGE_EVENTITEM_SEND:
			if(cInvenTory.SendEventItem()){ 
				SendWingQuestItem();
			}
		break;
		case MESSAGE_FOXEVENTITEM_SEND:
			if(cInvenTory.SendFoxEventItem()){   
				SendWingQuestItem();
			}
		case MESSAGE_QUEST_LEVEL30:
			sinQuest_Level30.CODE  = SIN_QUEST_CODE_LEVEL30;
			sinQuest_Level30.State = 1;
			sinQuest_Level30.Monster[1] = 20; //�ٰ� 20����
			SaveGameData();
			StartQuest_Code(sinQuest_Level30.CODE); 
		break;
		case MESSAGE_QUEST_LEVEL55:
			sinQuest_Level55.CODE  = SIN_QUEST_CODE_LEVEL55;
			sinQuest_Level55.State = 1;
			sinQuest_Level55.Monster[1] = 20; //���� 20����
			SaveGameData();
			StartQuest_Code(sinQuest_Level55.CODE); 
		break;
		case MESSAGE_QUEST_LEVEL55_2:
			sinQuest_Level55_2.CODE  = SIN_QUEST_CODE_LEVEL55_2;
			sinQuest_Level55_2.State = 1;
			sinQuest_Level55_2.Monster[1] = 20; //���̾�� 20����
			SaveGameData();
			StartQuest_Code(sinQuest_Level55_2.CODE); 
		break;

		case MESSAGE_QUEST_LEVEL70:
			sinQuest_Level70.CODE  = SIN_QUEST_CODE_LEVEL70;
			sinQuest_Level70.State = 1;
			sinQuest_Level70.Monster[1] = 10; //Ÿ��Ǭ 10����
			SaveGameData();
			StartQuest_Code(sinQuest_Level70.CODE); 

		break;
		case MESSAGE_QUEST_LEVEL80:
			sinQuest_Level80.CODE  = SIN_QUEST_CODE_LEVEL80;
			sinQuest_Level80.State = 1;
			sinQuest_Level80.Monster[1] = 5; //���� 5����
			SaveGameData();
			StartQuest_Code(sinQuest_Level80.CODE); 

		break;
		case MESSAGE_QUEST_LEVEL85:
			sinQuest_Level85.CODE  = SIN_QUEST_CODE_LEVEL85;
			sinQuest_Level85.State = 2; //���°� 1�̸� ��� 
			sinQuest_Level85.Monster[1] = 10; //�׷��׽�ũ 10����
			SaveGameData();
			StartQuest_Code(sinQuest_Level85.CODE); 
			if(!cInvenTory.SearchItemCode(sinQT1|sin08)){
				SendQuestCommandToServer( SIN_QUEST_CODE_LEVEL85 , 0, 0, 0 );
			}

		break;
		case MESSAGE_QUEST_LEVEL90:
			sinQuest_Level90.CODE  = SIN_QUEST_CODE_LEVEL90;
			sinQuest_Level90.State = 1; 
			sinQuest_Level90.Monster[1] = 10; //�׷��׽�ũ 10����
			SaveGameData();
			StartQuest_Code(sinQuest_Level90.CODE); 
		break;
		case MESSAGE_QUEST_LEVEL80_2:
			sinQuest_Level80_2.CODE  = SIN_QUEST_CODE_LEVEL80_2;
			sinQuest_Level80_2.State = 1; 
			SaveGameData();
			StartQuest_Code(sinQuest_Level80_2.CODE); 
		break;
		case MESSAGE_QUEST_LEVEL90_2:
			sinQuest_Level90_2.CODE  = SIN_QUEST_CODE_LEVEL90_2;
			sinQuest_Level90_2.State = 1; 
			SaveGameData();
			StartQuest_Code(sinQuest_Level90_2.CODE); 
		break;

		case MESSAGE_TELEPORT:
			CheckCharForm(); //���� 
			//<ha>������ ���������� �ڷ���Ʈ �Ӵϸ� ������ ������.
			SendPaymentMoneyToServer(TeleportUseCose[sinTeleportMoneyIndex-1]+((TeleportUseCose[sinTeleportMoneyIndex-1]*cSinSiege.GetTaxRate())/100),2);
			
			sinMinusMoney(TeleportUseCose[sinTeleportMoneyIndex-1],1); //��������
			WarpField2( sinTeleportIndexArray[sinTeleportMoneyIndex-1] );
			sinPlaySound(SIN_SOUND_COIN);
			ReformCharForm(); //������ 
		break;
		case MESSAGE_CHANGE_JOB4: //����Ʈ ����
			sinQuest_ChangeJob4.CODE  = SIN_QUEST_CODE_CHANGEJOB4;
			sinQuest_ChangeJob4.State = 1; 
			StartQuest_Code(sinQuest_ChangeJob4.CODE); 
			sinChangeJob4MonSet(&sinQuest_ChangeJob4); //���͸� �����Ѵ�
			SaveGameData();
		break;
		case MESSAGE_CHANGE_JOB4_2: //����Ʈ �Ϸ�
			///////////////// �̰��� ����Ʈ ���� �޼����� ���´� 
			sinChar->ChangeJob++; //������ �Ѵ� 
			cSkill.OpenFlag = 1;
			cInterFace.CheckAllBox(SIN_SKILL); //â�� �ϳ��� ���� 
			SkillMasterFlag = 1;  //��ų�� �Ҵ��Ҽ��ְ� ���ش�
			sinQuest_ChangeJob4.State = 0;
			sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob);
			lstrcpy(cInvenTory.JobName,sinJobList->szName2);
				
			sinQuest_ChangeJob4.BackUpCode = sinQuest_ChangeJob4.CODE;
			sinQuest_ChangeJob4.CODE = 0; //����Ʈ�� �Ϸ�Ǹ� �ڵ尡 ������� 
			sinChar->wVersion[1]     = 0;
			EndQuest_Code(sinQuest_ChangeJob4.BackUpCode);
				
			cInvenTory.ReFormInvenItem();
			sinSkillPontInitButtonFlag = 1; //��ų ����Ʈ �ʱ�ȭ �ڽ��� ���� 
			SaveGameData(); //������ �����Ѵ�

			ChangeJobFace();
			if (sinChar->JOB_CODE <= 4 || sinChar->JOB_CODE == 9)
				WarpField2(START_FIELD_NUM); //���ư���
			else
				WarpField2(START_FIELD_MORYON); //���ư���

			break;
		case MESSAGE_CRISTALEVENTITEM_SEND: //�ϰ����� ũ����Ż�� ����
			if(cInvenTory.SendCristalEventItem()){   
				SendWingQuestItem();
			}
			break;
		case MESSAGE_WATERMELON_EVENTITEM: // ����� - ���� ��ƿ���
			if(cInvenTory.SendWatermelonEventItem()){   
				SendWingQuestItem();
			}
			break;
		case MESSAGE_VALENTINE_EVENTITEM: // �庰 - �߷�Ÿ�� ���ݸ� ��ƿ���
			if(cInvenTory.SendValentineEventItem())
			{   
				SendWingQuestItem();
			}
			break;
		case MESSAGE_PRISTON_ALPHABET_EVENTITEM: // ����� - ���ĺ� ���� �̺�Ʈ
			if(cInvenTory.SendPristonAlphabetEventItem()){   
				SendWingQuestItem();
			}
			break;

		case MESSAGE_CANDYDAYS_EVENTITEM: // �庰 - ĵ������
			if(cInvenTory.SendCandydaysEventItem())
			{   
				SendWingQuestItem();
			}
		break;

		case MESSAGE_MAGICALGREEN_EVENTITEM: // �庰 - �����ñ׸�
			if(cInvenTory.SendMagicalGreenEventItem())
			{   
				SendWingQuestItem();
			}
		break;

		case MESSAGE_MAGICALGREEN_EVENTITEM_EMERALD: // �庰 - �����ñ׸� ���޶���
			if(cInvenTory.SendMagicalGreenEmeraldEventItem())
			{   
				SendWingQuestItem();
			}
		break;

		case MESSAGE_TEAROFKARA_EVENTITEM: // �庰 - ī���� ����
			if(cInvenTory.SendTearOfKaraEventItem())
			{   
				SendWingQuestItem();
			}
		break;

		case MESSAGE_FIND_EVENTITEM: // �庰 - ������� ã�ƶ� - ���� ������
			if(cInvenTory.SendFindinvestigatorNineEventItem())
			{   
				SendWingQuestItem();
			}
		break;

		case MESSAGE_FINDKAMIU_EVENTITEM: // �庰 - ������� ã�ƶ� - ī����
			if(cInvenTory.SendFindinvestigatorTaleEventItem())
			{   
				SendWingQuestItem();
			}
		break;

		case MESSAGE_FINDETO_EVENTITEM:	// �庰 - ������� ã�ƶ� - ����
			if(cInvenTory.SendFindinvestigatorTaleEtoEventItem())
			{   
				SendWingQuestItem();
			}
		break;

		case MESSAGE_CASTLE_TELEPORT :      //���� ĳ�� �ڷ���Ʈ
			CheckCharForm(); //���� 

			WarpField2( 33 );
			sinMinusMoney(haC_TeleportMoney); //��������
			sinPlaySound(SIN_SOUND_COIN);
			ReformCharForm(); //������ 
			
			// �庰 - ���� ĳ��
			/*
			//<ha>15���̻� ������ �ʵ�� �̵��Ѵ�.
			if ( sBiInfo && (sBiInfo->EventPlay[0]&BIMASK_ADULT_USER)!=0 ) {
					WarpField2( 33 );
					sinMinusMoney(haC_TeleportMoney); //��������
					sinPlaySound(SIN_SOUND_COIN);
					ReformCharForm(); //������ 
			}
			else{
				cMessageBox.ShowMessage(MESSAGE_NOT_CASTLE_TELEPORT);

			}
			*/

			
		break;
		case MESSAGE_CASTLE_BUYTOWER:       //���� ĳ�� ũ����Ż Ÿ���� ���.
			if((sinChar->Money - haTowerMoney) < 0){
					cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY); //������
					break;
			}
			for(int i=0;i<6;i++){
				if(cSinSiege.TowerTypeDraw[i][0]){
					cSinSiege.TowerTypeDraw[i][1] = haSendTowerIndex;
				}

			}

			cSinSiege.Price=haTowerMoney;
			sinMinusMoney(haTowerMoney);

			chaSiege.SendServerSiegeData(); //�ٽ� �������ش�.
		//	ReformCharForm();//������
		//	SendSaveMoney();
			sinPlaySound(SIN_SOUND_COIN);


		break;
		//�ǰ��� â������
		case MESSAGE_QUEST_ElementaryA:
 			chaQuest.sHaQuestElementary[Quest_A].CODE  = HAQUEST_CODE_ELEMENTARY_A;
			chaQuest.sHaQuestElementary[Quest_A].State = 1;
			if(!cInvenTory.SearchItemCode(sinQT1|sin15)){
				SendQuestCommandToServer( HAQUEST_CODE_ELEMENTARY_A , 1, 0, 0 );
			}
			SaveGameData();
			StartQuest_Code(chaQuest.sHaQuestElementary[Quest_A].CODE);
		break;
		//�η��� ����
		case MESSAGE_QUEST_ElementaryB:
			chaQuest.sHaQuestElementary[Quest_B].CODE  = HAQUEST_CODE_ELEMENTARY_B;
			chaQuest.sHaQuestElementary[Quest_B].State = 1;
			chaQuest.haElementaryQuest_B();
			SaveGameData();
			StartQuest_Code(chaQuest.sHaQuestElementary[Quest_B].CODE);
		break;
		//���� ���
		case MESSAGE_QUEST_ElementaryC:
			chaQuest.sHaQuestElementary[Quest_C].CODE  = HAQUEST_CODE_ELEMENTARY_C;
			chaQuest.sHaQuestElementary[Quest_C].State = 1;
			if(!cInvenTory.SearchItemCode(sinQT1|sin14)){
				SendQuestCommandToServer( HAQUEST_CODE_ELEMENTARY_C ,1, 0, 0 );
			}
			SaveGameData();
			StartQuest_Code(chaQuest.sHaQuestElementary[Quest_C].CODE);		
		break;
		//��� ����
		case MESSAGE_QUEST_ElementaryD:
			chaQuest.sHaQuestElementary[Quest_D].CODE  = HAQUEST_CODE_ELEMENTARY_D;
			chaQuest.sHaQuestElementary[Quest_D].State = 1;
			chaQuest.haElementaryQuest_D();
			SaveGameData();
			StartQuest_Code(chaQuest.sHaQuestElementary[Quest_D].CODE);
		break;
		//�𵥵��� ����
		case MESSAGE_QUEST_ElementaryE:
			chaQuest.sHaQuestElementary[Quest_E].CODE  = HAQUEST_CODE_ELEMENTARY_E;
			chaQuest.sHaQuestElementary[Quest_E].State = 1;
			chaQuest.haElementaryQuest_E();
			SaveGameData();
			StartQuest_Code(chaQuest.sHaQuestElementary[Quest_E].CODE);
		break;
		//�ܴ����� ����
		case MESSAGE_QUEST_ElementaryF:
			chaQuest.sHaQuestElementary[Quest_F].CODE  = HAQUEST_CODE_ELEMENTARY_F;
			chaQuest.sHaQuestElementary[Quest_F].State = 1;
			chaQuest.haElementaryQuest_F();
			SaveGameData();
			StartQuest_Code(chaQuest.sHaQuestElementary[Quest_F].CODE);
		break;
		//�Ҿ���� ��ġ
		case MESSAGE_QUEST_ElementaryG:
			chaQuest.sHaQuestElementary[Quest_G].CODE  = HAQUEST_CODE_ELEMENTARY_G;
			chaQuest.sHaQuestElementary[Quest_G].State = 1;
			SaveGameData();
			StartQuest_Code(chaQuest.sHaQuestElementary[Quest_G].CODE);
		break;
		case  MESSAGE_EVENT_ITEMOK:
			chaPremiumitem.UsePremiumItem(cInvenTory.iEventBuyItemKind);
		break;
		//100LV �˽�Ʈ 
		case MESSAGE_QUEST_100LV:
			chaQuest.sHaQuest100LV.CODE  = HAQUEST_CODE_FURYOFPHANTOM ;
			chaQuest.sHaQuest100LV.State = 1;
			StartQuest_Code(chaQuest.sHaQuest100LV.CODE);
			chaQuest.showFuryQuest();
			SaveGameData();
		break;
		case MESSAGE_QUEST_100LVITEMOK:
			//������ ���� üũ�� �ӽ� ������
			sITEM TempItem;
			TempItem.w = ITEMSIZE*2;
			TempItem.h = ITEMSIZE*4;
			TempItem.sItemInfo.Weight = 60;
			TempItem.Flag = 1;

			if(sinChar->Weight[0]+TempItem.sItemInfo.Weight > sinChar->Weight[1]){
				cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT); //�����ʰ�
				chaQuest.sHaQuest100LV.State = 12;
				chaQuest.iFuryQuestIndex[1] = 0;			
				break;
			}
			//�������ֱ� 
			if(cInvenTory.CheckSetEmptyArea(&TempItem)){
				SendQuestCommandToServer( HAQUEST_CODE_FURYOFPHANTOM ,2, chaQuest.iFuryQuestIndex[1], 0 );
				haElementaryQuestLog |= QUESTBIT_FURYOFPHANTOM; //�ڵ带 �������ش�
				chaQuest.sHaQuest100LV.State = 100;
				chaQuest.sHaQuest100LV.BackUpCode = chaQuest.sHaQuest100LV.CODE;
				chaQuest.sHaQuest100LV.CODE =0;
				EndQuest_Code(chaQuest.sHaQuest100LV.BackUpCode);
				cMessageBox.ShowMessage(MESSAGE_100LVITEM_OK);
				SaveGameData();
			}
			else{
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
				chaQuest.sHaQuest100LV.State = 12;
				chaQuest.iFuryQuestIndex[1] = 0;
				break;						
			}
			chaQuest.iFuryQuestIndex[1] = 0;
			SaveGameData();
		break;
		case MESSAGE_QUEST_100LVBATTLEOK:   //��������� �̵� 
			if(chaQuest.sHaQuest100LV.State == 3 || 
				chaQuest.sHaQuest100LV.State == 8 || 
				chaQuest.sHaQuest100LV.State == 10)
				chaQuest.sHaQuest100LV.State++;
			Start_QuestArena(HAQUEST_CODE_FURYOFPHANTOM , chaQuest.sHaQuest100LV.State/5+1, 0 );
			SetQuestBoard();
		break;
		case MESSAGE_TELEPORT_CORE:        //�ڷ���Ʈ �ھ�
			chaPremiumitem.Using_ItemKind = 0;
			chaPremiumitem.UseTeleportScrollItem(chaPremiumitem.TelePortCore.ItemIndex);
		break;
/*		
		case MESSAGE_STARDUST_SEND:			
			if(cInvenTory.SendStarDustItem()){		//������ ���縦 �����ش� //ũ��������
				SendWingQuestItem();
			}
		break;
*/
		case MESSAGE_FALLGAME:   //
			WarpField2( 39 );
		break;
		case MESSAGE_MANUFACTURE:		// pluto ���� ������ ���� �޼���
			sinCraftItemStartFlag = 1;
			SetInterEffectManufacture();
			MixCancelButtonDelayFlag = 1; // pluto ����� ����� �ܺ� ������ �ٲ�� -_-
		break;
		case MESSAGE_TELEPORT_DUNGEON:		// pluto �߰��������� �����Ѵ�.
			cSinHelp.NPCTeleportFlag = 0;
			WarpField2( 40 );
			break;
	}
	sinMessageBoxShowFlag  = 0; //ó���� �����ش����� �ڽ��� �ݾ��ش� 
	sinInitPointFlag7 = 0;
	sMessageBox3[Kind].Flag   = 0; 
	ShowSkillUpInfo = 0; //��ų ������ �����ش� 
	
	return TRUE;
}


//Ȯ�� �� ������ ��쳪 ���͸� �Է����� ��� 
int cMESSAGEBOX::CheckOkMessageBox(int Kind)
{
	int j=0;

	if(sMessageBox2[Kind].Count <= 0){ //���� �������� ������  
		sinMessageBoxShowFlag  = 0;
		sMessageBox2[Kind].Flag   = 0; 
		cSinSod2.ClanMasterMessageBoxFlag = 0; //�Է��� ���� ���¿��� ���ϵɶ� �ʱ�ȭ���ش�
		return FALSE;
	}

	memset(&MessageBoxItem,0,sizeof(sITEM)); //�ʱ�ȭ 
	switch(Kind){
		case MESSAGE_BUY_ITEM:   //������ ��� 
			if(!BuyItemServerFlag){		
				cShop.SendBuyItemToServer(pMessageItem,sMessageBox2[Kind].Count);
				pMessageItem = 0;
				cInvenTory.CheckWeight(); //���Ժ��� 
				
			}

		break;
		case MESSAGE_SELL_ITEM:  //������ �ȱ� 
			memcpy(&MessageBoxItem,pMessageItem,sizeof(sITEM)); //������ ���� 
			if(sMessageBox2[Kind].Count >= pMessageItem->sItemInfo.PotionCount){
				cShop.SellItemToShop(&MessageBoxItem,MessageBoxItem.sItemInfo.PotionCount);
				pMessageItem->Flag=0; //�÷� �ʱ�ȭ 
				sInven[pMessageItem->ItemPosition-1].ItemIndex = 0; //������ �ε��� �ʱ�ȭ 
			}
			else{
				pMessageItem->sItemInfo.PotionCount -= sMessageBox2[Kind].Count;
				MessageBoxItem.sItemInfo.PotionCount = sMessageBox2[Kind].Count;
				cShop.SellItemToShop(&MessageBoxItem,MessageBoxItem.sItemInfo.PotionCount);
			}
			pMessageItem=0; //������ �ʱ�ȭ 
			cInvenTory.CheckWeight(); //���Ժ��� 

		break;
		case MESSAGE_THROW_ITEM: //������ ������ 
			if(cMixtureReset.OpenFlag)break;	// ������ - �ͽ��� ���� â �߰�
			if(cWareHouse.OpenFlag)break;
			if(cTrade.OpenFlag)break;
			if(cAging.OpenFlag)break;
			if(cCraftItem.OpenFlag)break;
			if(sSinWarpGate.ShowFlag)break;
			if(!pMessageItem)break;
			// pluto ����
			if( SmeltingItem.OpenFlag )
			{
				break;
			}
			// pluto ����
			if( ManufactureItem.m_OpenFlag )
			{
				break;
			}

			memcpy(&MessageBoxItem,pMessageItem,sizeof(sITEM)); //������ ���� 
			if(sMessageBox2[Kind].Count >= pMessageItem->sItemInfo.PotionCount){
				if(sinThrowItemToFeild(&MessageBoxItem)){
					sinPlaySound(MessageBoxItem.SoundIndex);
					pMessageItem->Flag=0; //�÷� �ʱ�ȭ 
					sInven[pMessageItem->ItemPosition-1].ItemIndex = 0; //������ �ε��� �ʱ�ȭ 
					sinSaveGame(MessageBoxItem.sItemInfo.CODE ,sMessageBox2[Kind].Count);
				}
			}
			else{
				pMessageItem->sItemInfo.PotionCount -= sMessageBox2[Kind].Count;
				MessageBoxItem.sItemInfo.PotionCount = sMessageBox2[Kind].Count;
			
				if(sinThrowItemToFeild(&MessageBoxItem)){
					sinPlaySound(MessageBoxItem.SoundIndex);
					sinSaveGame(MessageBoxItem.sItemInfo.CODE ,sMessageBox2[Kind].Count);
		
				}

			}
			pMessageItem=0; //������ �ʱ�ȭ 
			cInvenTory.CheckWeight(); //���Ժ��� 
			cInvenTory.ReFormPotionNum();	//���� ���� üũ 
		break;
		case MESSAGE_THROW_MONEY: //�������� 
			if(cMixtureReset.OpenFlag)break;	// ������ - �ͽ��� ���� â �߰�
			if(cWareHouse.OpenFlag)break;
			if(cTrade.OpenFlag)break;
			if(cAging.OpenFlag)break;
			if(cCraftItem.OpenFlag)break;
			if(sSinWarpGate.ShowFlag)break;
			// pluto ����
			if( SmeltingItem.OpenFlag )
			{
				break;
			}
			// pluto ����
			if( ManufactureItem.m_OpenFlag )
			{
				break;
			}
			memset(&MessageBoxItem,0,sizeof(sITEM));
			MessageBoxItem.sItemInfo.CODE = sinGG1|sin01;
			for(j=0 ; j < MAX_ITEM ; j++){
				if(sItem[j].CODE == MessageBoxItem.sItemInfo.CODE){ //�����̸� 
				memcpy(&MessageBoxItem,&sItem[j],sizeof(sITEM));
				break;
				}
			}
			if(sMessageBox2[Kind].Count > sinChar->Money){
				SendSetHackUser2(4100,1); //��ŷ�� ������ �Ű��Ѵ� 
				sMessageBox2[Kind].Count = sinChar->Money;

			}
			MessageBoxItem.sItemInfo.Money = sMessageBox2[Kind].Count; //�ݾ��� �Ѱ��ش� 
			wsprintf(MessageBoxItem.sItemInfo.ItemName,sinGold,MessageBoxItem.sItemInfo.Money);
			ReformItem( &MessageBoxItem.sItemInfo ); //���� 
			if(sinThrowItemToFeild(&MessageBoxItem)){
				sinPlaySound(MessageBoxItem.SoundIndex);
				CheckCharForm();//���� 
				//sinChar->Money -=MessageBoxItem.sItemInfo.Money;
				sinMinusMoney(MessageBoxItem.sItemInfo.Money);
				ReformCharForm();//������ 	
				SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
			}
		break;
		case  MESSAGE_MOVE_MONEY_TRADE://���ŷ�
			if(!CheckTrade((void *)&sTrade)) //�߸��� ����Ÿ �̸� �ŷ�â�� �ݴ´�
				cTrade.CancelTradeItem();
			CheckCharForm();//���� 
			if(sinChar->Money - sMessageBox2[Kind].Count < 0){
				SendSetHackUser2(4100,2); //��ŷ�� ������ �Ű��Ѵ� 
				sMessageBox2[Kind].Count = sinChar->Money;

			}
			//sinChar->Money -= sMessageBox2[Kind].Count;
			sinMinusMoney(sMessageBox2[Kind].Count);
			ReformCharForm();//������ 	
			SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
			sTrade.Money = sMessageBox2[Kind].Count;
			TempShowMoney = sTrade.Money;
			sTrade.Money = sTrade.Money+193; 
			sinPlaySound(SIN_SOUND_COIN);
			ReformTrade((void *)&sTrade);
			sTrade.CheckFlag = 0; //���� �ű�ԵǸ� �÷��� �ʱ�ȭ 
			SendTradeItem(cTrade.TradeCharCode);
			
		break;
		case MESSAGE_MOVE_MONEY_MOVE:
			if(sinChar->Money - sMessageBox2[Kind].Count < 0){
				SendSetHackUser2(4100,3); //��ŷ�� ������ �Ű��Ѵ� 
				sMessageBox2[Kind].Count = sinChar->Money;

			}

			CheckCharForm();//���� 
			//sinChar->Money -= sMessageBox2[Kind].Count;
			sinMinusMoney2(sMessageBox2[Kind].Count);
			ReformCharForm();//������ 	
			//SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
			sWareHouse.Money += sMessageBox2[Kind].Count;
			sinPlaySound(SIN_SOUND_COIN);
			WareHouseSaveFlag = 1;

		break;

		case MESSAGE_MOVE_MONEY_RECV:
			if((sWareHouse.Money-2023) - sMessageBox2[Kind].Count < 0){
				SendSetHackUser2(4100,4); //��ŷ�� ������ �Ű��Ѵ� 
				sMessageBox2[Kind].Count = sWareHouse.Money-2023;

			}

			CheckCharForm();//���� 
			//sinChar->Money += sMessageBox2[Kind].Count;
			sinPlusMoney2(sMessageBox2[Kind].Count);
			ReformCharForm();//������ 	
			//SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
			sWareHouse.Money -= sMessageBox2[Kind].Count;
			sinPlaySound(SIN_SOUND_COIN);
			WareHouseSaveFlag = 1;

		break;
		case MESSAGE_BUY_ITEM_DEFALT: //�׳� ������ ����ϰ�� 
			if(!BuyItemServerFlag){
				cShop.SendBuyItemToServer(pMessageItem,sMessageBox2[Kind].Count);
				cInvenTory.CheckWeight(); //���Ժ��� 
				
			}

		break;
		case MESSAGE_BUY_STAR: //���� ���
			SendStarPointToServer(sMessageBox2[Kind].Count*100000,1); //������ ������ 
			CheckCharForm();//���� 
			sinMinusMoney2(sMessageBox2[Kind].Count*100000);
			sinPlaySound(SIN_SOUND_COIN);
			ReformCharForm();//������ 	
			StarShopNPC();
		break;

		case MESSAGE_GIVE_MONEY:
			sinGiveMoneyCheckFlag = 1;
			sinGiveMoneyAmount = sMessageBox2[Kind].Count;
			wsprintf(szGiveMoneyBuff,"%d",sinGiveMoneyAmount);

			/*
			if(sinChar->Money - sMessageBox2[Kind].Count < 0){
				SendSetHackUser2(4100,5); //��ŷ�� ������ �Ű��Ѵ� 
				sMessageBox2[Kind].Count = sinChar->Money;

			}

			CheckCharForm();//���� 
			sinMinusMoney2(sMessageBox2[Kind].Count);
			ReformCharForm();//������ 	
			//���⼭ ������ ���� ������
//			SendGiveMoneyToServer(sMessageBox2[Kind].Count);
			sinPlaySound(SIN_SOUND_COIN);
			*/

		break;
		case MESSAGE_MYSHOP_ITEM: //���λ����� ������ ��� 
			if(cInvenTory.CheckMoneyLimit(sMessageBox2[Kind].Count)){ 
				MyShopMouseItem.sItemInfo.Money = sMessageBox2[Kind].Count;
				cMyShop.LastSetMyShopItem(&MyShopMouseItem);
			}

			/*
			if(MyShopMouseItem.Class == ITEM_CLASS_POTION){
				if(cInvenTory.CheckMoneyLimit(sMessageBox2[Kind].Count*MyShopMouseItem.sItemInfo.PotionCount)){
					MyShopMouseItem.sItemInfo.Money = sMessageBox2[Kind].Count;
					cMyShop.LastSetMyShopItem(&MyShopMouseItem);
				}
			}
			else{
				if(cInvenTory.CheckMoneyLimit(sMessageBox2[Kind].Count)){ 
					MyShopMouseItem.sItemInfo.Money = sMessageBox2[Kind].Count;
					cMyShop.LastSetMyShopItem(&MyShopMouseItem);
				}
			}
			*/
		break;
		case MESSAGE_MYSHOP_POTION:
			if(cCharShop.OpenFlag){
				sMyShop_Server.sTime = sMessageBox2[Kind].Count;
				sMyShop_Server.Price *= sMessageBox2[Kind].Count; 
				Send_PersonalShopItem(sRecvMyShop.CHAR_CODE,&sMyShop_Server); //������ ������ �������� ������ ������
			}
		break;
		case MESSAGE_SOD2_GET_MONEY:
			if(sinMessageBox_New.Flag && cSinSod2.BoxIndex ==1 && sMessageBox2[Kind].Count){
				if(cSinSod2.UserType == 6){ 
					SendClanMoneyToServer(sMessageBox2[Kind].Count,0);
				}
			}
			cSinSod2.ClanMasterMessageBoxFlag = 0;
			//���⼭ ����� ã�´�
			//cSinSod2.ClanMoney

		break;
		case MESSAGE_SIEGE_GET_MONEY: //<ha>������ ���� �����Ѿ׿��� ���� ã�´�. //����ȸ�� ��ư������ ����â �߰� �ű⼭ ���� ������ Ȯ�� ������ ���� ����
			if(haSiegeMenuFlag)		//�ؿ�
			{
				if( (cSinSiege.TotalTax >= sMessageBox2[Kind].Count) &&		//�ؿ� ����
					(cSinSiege.ExpectedTotalTax >= sMessageBox2[Kind].Count))
				{
					cSinSiege.ExpectedTotalTax = cSinSiege.ExpectedTotalTax - sMessageBox2[Kind].Count; //�ؿ� ����
					SendClanMoneyToServer(sMessageBox2[Kind].Count,0,1);
				}
			}
		break;
		case MESSAGE_SIEGE_SET_MERCENRAY:   //<ha>������ �뺴����
			
			if(haSiegeMenuFlag){
				 //���� �뺴 ������ ����Ÿ�� �����Ѵ�.
				if(haMercenrayIndex){
					cSinSiege.MercenaryNum[haMercenrayIndex-1]+=sMessageBox2[Kind].Count; 
					cSinSiege.Price=haMercenrayMoney[haMercenrayIndex-1]*sMessageBox2[Kind].Count;
					chaSiege.SendServerSiegeData(); //�����Ѵ�.
					cSinSiege.Price = 0;
				}
			}
			//�뺴������ ���ش�.
			sinMinusMoney(haMercenrayMoney[haMercenrayIndex-1]*sMessageBox2[Kind].Count);
			sinPlaySound(SIN_SOUND_COIN);
		break;

/*

		case MESSAGE_EVENT_SMASHTV: //�̺�Ʈ ���޽� TV 
			///////////////// ���� �������� �������� �ʾҽ�
			if(sinChar->Money - sMessageBox2[Kind].Count < 0 || sMessageBox2[Kind].Count > 100000){
				SendSetHackUser2(4100,5); //��ŷ�� ������ �Ű��Ѵ� 
				sMessageBox2[Kind].Count = sinChar->Money;

			}
			if(sMessageBox2[Kind].Count < 100000){
				CloseMessage(); //�޼����ڽ����ݾ��ش� 
				ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
				return FALSE;

			}
			CheckCharForm();//���� 
			//sinChar->Money -= sMessageBox2[Kind].Count;
			sinMinusMoney(sMessageBox2[Kind].Count);
			ReformCharForm();//������ 	
			SendCollectMoney(sMessageBox2[Kind].Count);
			sinPlaySound(SIN_SOUND_COIN);
			//ShowMessage(MESSAGE_GIVE_MONEY_TRUE);
			//��⼭ �̺�Ʈ�� �����ϸ� �ǰڽɴ� �ε�!
			
		break;

*/

	}
	sinMessageBoxShowFlag  = 0; //ó���� �����ش����� �ڽ��� �ݾ��ش� 
	sMessageBox2[Kind].Flag   = 0; 
	memset(&MessageBoxItem,0,sizeof(sITEM)); //�ʱ�ȭ 
	cInvenTory.ReFormPotionNum();	//���� ���� ���� 
	return TRUE;
}

//������ npc
int StarShopNPC()
{
	//��ǰ���� ������� ���� �ٲ۴�
	SendStarPointToServer(0,0);//�ϴ� ����� �޴´� 
	sITEM *pItem=0;
	GiftPrice = 0;
	sStarShop.Flag = 1;
	for(int i = 0; i < INVENTORY_MAXITEM*2 ; i++){
		if(i<100){
			if(!cInvenTory.InvenItem[i].Flag)continue;
			pItem = &cInvenTory.InvenItem[i];
		}
		else{
			if(!cInvenTory.InvenItemTemp[i-100].Flag)continue;
			pItem = &cInvenTory.InvenItemTemp[i-100];
		}
		if(pItem->sItemInfo.CODE ==(sinGF1|sin01)){
			GiftPrice += pItem->sItemInfo.Price;
		}
	}	
	if(GiftPrice){
		cMessageBox.ShowMessage3(MESSAGE_GIFT_TICKET,"��ǰ������");
	}
	return TRUE;
}

//�������� ����Ʈ�� �޴´�
int sinStarPoint(int Point)
{
	sStarShop.Count = Point;
	return TRUE;
}

//��ǰ�� �������� �����
int DeleteGiftItem()
{
	sITEM *pItem=0;
	for(int i = 0; i < INVENTORY_MAXITEM*2 ; i++){
		if(i<100){
			if(!cInvenTory.InvenItem[i].Flag)continue;
			pItem = &cInvenTory.InvenItem[i];
		}
		else{
			if(!cInvenTory.InvenItemTemp[i-100].Flag)continue;
			pItem = &cInvenTory.InvenItemTemp[i-100];
		}
		if(pItem->sItemInfo.CODE ==(sinGF1|sin01)){
			pItem->Flag =0;
			
		}
	}	
	cInvenTory.ReFormInvenItem(); //�������� ����� �������ش�
	ReformCharForm();//������ 
	return TRUE;

}