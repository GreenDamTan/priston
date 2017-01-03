/*----------------------------------------------------------------------------*
*	���ϸ� :  sinShop.cpp	
*	�ϴ��� :  ������ �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\clan_Enti.h"
#include "..\\CurseFilter.h"		//IsCurse �Լ�(�ؿ�)

/*----------------------------------------------------------------------------*
*								�������� 			
*-----------------------------------------------------------------------------*/	
cSHOP		cShop;
cMYSHOP		cMyShop;
cCHARSHOP	cCharShop;
sMYSHOP     sMyShop;       //������ MyShop ����ü
sMYSHOP     sRecvMyShop;   //�޴� MyShop ����ü
sMYSHOP_ITEM_SERVER sMyShop_Server;

sITEMINFO   MyShopPotion;

int CloseMyShopFlag = 0;
int CloseCharShopFlag = 0;
int SelectShopButtonPosi = 0;  //������ ��ư ��ġ�� 
int MyShopItemIndex[100] ={0,};


int sinShopKind = 1;  //���� ������ ����  1  ���� 2 ��� 3 ���� (defalt �� ����)

int ShopArrowPosi = 0;

RECT ShopItemRect={0,0,0,0};

int	GridRow , GridCol ,StartGirdX , StartGirdY; //�׸��� 

int ShopItemPrice=0;//���ǰ��� 
int ShowShopItemIndex=0; //�������� �ε����� �����Ѵ� 

int TalkNpcState = 0; //���� ���� NPC�� �Բ� ������� 

int haCharShopDelayFlag=0;

//��ǥ�������� �ϳ�������
int ReStartButtonRect2[3][4] = {
	{21,22,21+44,22+44},			//�ʵ忡�� ���� 
	{72,22,72+44,22+44},			//�������� ���� 
	{123,22,123+44,22+44},			//���� ���� 
	
};

int ShopButtonPosi[9][4] = {
	{269,278+sinInterHeight2,269+28,278+28+sinInterHeight2}, //�ٰ�ġ��
	{238,278+sinInterHeight2,238+28,278+28+sinInterHeight2}, //��ġ�� 
	{238,309+sinInterHeight2,238+28,309+28+sinInterHeight2}, //�ȱ�  
	{269,309+sinInterHeight2,269+28,309+28+sinInterHeight2}, //��� 
	{279,182+sinInterHeight2,279+20,182+20+sinInterHeight2}, //������ ȭ��ǥ 
	{257,182+sinInterHeight2,257+20,182+20+sinInterHeight2}, //���� ȭ��ǥ 
	{304,314+sinInterHeight2,304+20,314+20+sinInterHeight2}, //������ 
	{231,136+sinInterHeight2,231+42,136+46+sinInterHeight2}, //������
	{281,136+sinInterHeight2,281+42,136+46+sinInterHeight2}, //�����

};

//�ݾ��� ��ġ 
int ShopGoldEdit[2][4] = {
	{231,213+5+sinInterHeight2,231+120,213+5+24+sinInterHeight2}, //���� ���� 
	{231,243+5+sinInterHeight2,231+120,243+5+24+sinInterHeight2}, //���� 

};

//////////������ �����ټ��ִ¾������� �Ѱ�ġ 
int ShowShopItemIndex2 = 0;

int BuyItemServerFlag = 0;
int BuyItemIndexCopy = 0; //�������� ������ �������� �ε��� 

int SelectMyShopButtonPosi = 0;
int SelectMyShopItemIndex = 0;

int MyShopSendButton   = 0;
int MyShopExpBox	   = 0;
int MyShopExpButtonOK  = 0;
int MyShopExpButtonCancel  = 0;

int SelectCharShopButtonPosi = 0;

int MyShopExpBoxX = 250;
int MyShopExpBoxY = 100;

int MyShopItemCancelIndex[2] = {0,0};


/*----------------------------------------------------------------------------*
*							Ŭ���� �ʱ�, ���� 
*-----------------------------------------------------------------------------*/	
cSHOP::cSHOP()
{

}
cSHOP::~cSHOP()
{

}
/*----------------------------------------------------------------------------*
*							     �ʱ�ȭ 
*-----------------------------------------------------------------------------*/	
void cSHOP::Init()
{
	//MatShop[0] =  CreateTextureMaterial( "Image\\SinImage\\Shopall\\Shop-1.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatShop[1] =  CreateTextureMaterial( "Image\\SinImage\\Shopall\\Trade-2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatShop[2] =  CreateTextureMaterial( "Image\\SinImage\\Shopall\\Warehouse-2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	Load();

	MatMyShop_Button = CreateTextureMaterial( "Image\\SinImage\\Shopall\\MyShopButton.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

}
/*----------------------------------------------------------------------------*
*						    Bmp������ �ε��Ѵ� 
*-----------------------------------------------------------------------------*/	
void cSHOP::Load()
{

	lpShopMain = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\shop-1.bmp");
	lpShopMain2 = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\shop-2.bmp");

	lpGoldEdit = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\my_money.bmp");

	lpAllRepair = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\AllRepair.bmp");
	lpRepair    = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Repair.bmp");

	lpSell      = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Sell.bmp");
	lpBuy       = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Buy.bmp") ;

	lpAllRepair_D = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\DefaultAllRepair.bmp");
	lpRepair_D    = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\DefaultRepair.bmp");

	lpSell_D      = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Sell0.bmp");
	lpBuy_D       = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Buy0.bmp") ;

	lpRightArrow = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\RightArrow.bmp");
	lpLeftArrow  = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\LeftArrow.bmp");

	lpTabAttack  = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\TabAttack.bmp");
	lpTabDefense = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\TabDefense.bmp");
	lpTabEtc     = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\TabEtc.bmp");

	lpTabAttack_D  = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\TabAttack0.bmp");
	lpTabDefense_D = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\TabDefense0.bmp");
	lpTabEtc_D     = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\TabEtc.bmp");

	lpExit       = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\ButtonExit.bmp");
	lpGrid       = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Grid.bmp");

	lpRepairAllInfo	= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\ButtonInfo\\RepairAllInfo.bmp");	
	lpRepairInfo	= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\ButtonInfo\\RepairInfo.bmp");
	lpSellInfo		= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\ButtonInfo\\SellInfo.bmp");
	lpBuyInfo		= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\ButtonInfo\\BuyInfo.bmp");

	lpDefaultRepairAll = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\DefaultAllRepair.bmp");	
	lpDefaultRepair    = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\DefaultRepair.bmp");

	lpTitle_Attack = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\title-shop.bmp");
	lpTitle_Etc = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\title-TabEtc.bmp");
	lpTitle_Trade = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Trade.bmp");
	lpTitle_WareHouse = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Warehouse.bmp");

	lpWeightEdit = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\weight.bmp");

	lpNoUseBox = LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\NoUseBox.bmp");

	lpCraftItemMain			= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\CraftItem\\CraftItemMain.bmp");
	lpCraftItemButton		= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\CraftItem\\Button.bmp");
	lpCraftItemButtonInfo	= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\CraftItem\\ButtonInfo.bmp");
	lpTitle_CraftItem		= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\CraftItem\\Title.bmp");
	lpTitle_Aging			= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Aging\\Aging_Title.bmp");
	lpAging_Info			= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Aging\\ButtonInfo.bmp");

	// pluto ����
	lpSmeltingItemMain		= LoadDibSurfaceOffscreen( "Image\\SinImage\\Shopall\\SmeltingItem\\SmeltingMain.bmp" );
	lpTitle_SmeltingItem	= LoadDibSurfaceOffscreen( "Image\\SinImage\\Shopall\\SmeltingItem\\SmeltingTitle.bmp");
	lpSmeltingItemButtonInfo = LoadDibSurfaceOffscreen( "Image\\SinImage\\Shopall\\SmeltingItem\\ButtonInfo.bmp");

	// pluto ����
	lpManufactureItemMain	= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\Manufacture\\ManufactureMain.bmp" );
	lpTitle_ManufactureItem = LoadDibSurfaceOffscreen( "Image\\SinImage\\Shopall\\Manufacture\\ManufactureTitle.bmp" );
	lpManufactureItemButtonInfo = LoadDibSurfaceOffscreen( "Image\\SinImage\\Shopall\\Manufacture\\ButtonInfo.bmp" );

	// ������ - �ͽ��� ������ ���� �̹��� �ε� ( ���� lpCraftItemButton(��ư) ���� �ʿ��ҵ� )
	lpMResetTitle			= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\CraftItem\\MResetTitle.bmp");
	lpMResetMain			= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\CraftItem\\MResetMain.bmp");
	lpMResetButton			= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\CraftItem\\MResetButton.bmp");
	lpMResetButtonInfo		= LoadDibSurfaceOffscreen("Image\\SinImage\\Shopall\\CraftItem\\MResetButtonInfo.bmp");

	lpMyShopExp				= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_exp.bmp");
	lpMyShopExp_			= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_exp_.bmp");
	lpMyShopExp_T			= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_expT.bmp");
	lpMyShopNoSale			= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_nosale.bmp");
	lpMyShopNoSale_			= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_nosale_.bmp");
	lpMyShopNoSale_T		= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_nosaleT.bmp");
	lpMyShopSale			= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_sale.bmp");
	lpMyShopSale_			= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_sale_.bmp");
	lpMyShopSale_T			= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_saleT.bmp");
	lpTitle_MyShop			= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_title.bmp");
	lpImage_MyShop			= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_image.bmp");
	lpMyShop_T				= LoadDibSurfaceOffscreen("Image\\SinImage\\indishop\\indishop_T.bmp");

}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cSHOP::Release()
{
	if(lpAllRepair){
		lpAllRepair->Release();
		lpAllRepair = 0;
	}
	if(lpRepair){
		lpRepair->Release();
		lpRepair = 0;
	}
	if(lpSell){
		lpSell->Release();
		lpSell = 0;
	}
	if(lpBuy){
		lpBuy->Release();
		lpBuy = 0;
	}
	if(lpAllRepair_D){
		lpAllRepair_D->Release();
		lpAllRepair_D = 0;
	}
	if(lpRepair_D){
		lpRepair_D->Release();
		lpRepair_D = 0;
	}
	if(lpSell_D){
		lpSell_D->Release();
		lpSell_D = 0;
	}
	if(lpBuy_D){
		lpBuy_D->Release();
		lpBuy_D = 0;
	}

	if(lpRightArrow){
		lpRightArrow->Release();
		lpRightArrow = 0;
	}
	if(lpLeftArrow){
		lpLeftArrow->Release();
		lpLeftArrow = 0;
	}
	if(lpTabAttack){
		lpTabAttack->Release();
		lpTabAttack = 0;
	}
	if(lpTabDefense){
		lpTabDefense->Release();
		lpTabDefense = 0;
	}
	if(lpTabEtc){
		lpTabEtc->Release();
		lpTabEtc = 0;
	}
	if(lpTabAttack_D){
		lpTabAttack_D->Release();
		lpTabAttack_D = 0;
	}
	if(lpTabDefense_D){
		lpTabDefense_D->Release();
		lpTabDefense_D = 0;
	}
	if(lpTabEtc_D){
		lpTabEtc_D->Release();
		lpTabEtc_D = 0;
	}

	if(lpExit){
		lpExit->Release();
		lpExit = 0;
	}
	if(lpGrid){
		lpGrid->Release();
		lpGrid = 0;
	}
	if(lpRepairAllInfo){
		lpRepairAllInfo->Release();
		lpRepairAllInfo = 0;
	}
	if(lpRepairInfo){
		lpRepairInfo->Release();
		lpRepairInfo = 0;
	}
	if(lpSellInfo){
		lpSellInfo->Release();
		lpSellInfo = 0;
	}
	if(lpBuyInfo){
		lpBuyInfo->Release();
		lpBuyInfo = 0;
	}
	if(lpDefaultRepairAll){
		lpDefaultRepairAll->Release();
		lpDefaultRepairAll = 0;
	}
	if(lpDefaultRepair){
		lpDefaultRepair->Release();
		lpDefaultRepair = 0;
	}
	if(lpTitle_Attack){
		lpTitle_Attack->Release();
		lpTitle_Attack = 0;
	}
	if(lpTitle_Etc){
		lpTitle_Etc->Release();
		lpTitle_Etc = 0;
	}
	if(lpTitle_Trade){
		lpTitle_Trade->Release();
		lpTitle_Trade = 0;
	}
	if(lpTitle_WareHouse){
		lpTitle_WareHouse->Release();
		lpTitle_WareHouse = 0;
	}
	if(lpCraftItemMain){
		lpCraftItemMain->Release();
		lpCraftItemMain = 0;
	}

	if(lpCraftItemButton){
		lpCraftItemButton->Release();
		lpCraftItemButton = 0;
	}
	if(lpCraftItemButtonInfo){
		lpCraftItemButtonInfo->Release();
		lpCraftItemButtonInfo = 0;
	}
	if(lpTitle_CraftItem){
		lpTitle_CraftItem->Release();
		lpTitle_CraftItem = 0;
	}

	// pluto ����
	if( lpSmeltingItemMain )
	{
		lpSmeltingItemMain->Release();
		lpSmeltingItemMain = 0;
	}

	if( lpTitle_SmeltingItem )
	{
		lpTitle_SmeltingItem->Release();
		lpTitle_SmeltingItem = 0;
	}

	if( lpSmeltingItemButtonInfo )
	{
		lpSmeltingItemButtonInfo->Release();
		lpSmeltingItemButtonInfo = 0;
	}

	// pluto ����
	if( lpManufactureItemMain )
	{
		lpManufactureItemMain->Release();
		lpManufactureItemMain = 0;
	}
	if( lpTitle_ManufactureItem )
	{
		lpTitle_ManufactureItem->Release();
		lpTitle_ManufactureItem = 0;
	}
	if( lpManufactureItemButtonInfo)
	{
		lpManufactureItemButtonInfo->Release();
		lpManufactureItemButtonInfo = 0;
	}

	// ������ - �ͽ��� ���� ����
	if( lpMResetTitle )
	{
		lpMResetTitle->Release();
		lpMResetTitle = 0;
	}
	if( lpMResetMain )
	{
		lpMResetMain->Release();
		lpMResetMain = 0;
	}
	if( lpMResetButton )
	{
		lpMResetButton->Release();
		lpMResetButton = 0;
	}
	if( lpMResetButtonInfo )
	{
		lpMResetButtonInfo->Release();
		lpMResetButtonInfo = 0;
	}
}
/*----------------------------------------------------------------------------*
*							     �׸���  
*-----------------------------------------------------------------------------*/	
void cSHOP::Draw()
{


	////////////////���� (�ʵ�, ����, ����)
	if(ReStartFlag){
		DrawSprite(ReStartMainXY.x,ReStartMainXY.y,cInterFace.lpReStartMain,0,0,188,128); //�ٽý��� ���� 
		if(ReStartIndex && RestartCheckExp){
			if(ReStartIndex != 1)
				DrawSprite(ReStartMainXY.x+ReStartButtonRect2[ReStartIndex-1][0],ReStartMainXY.y+ReStartButtonRect2[ReStartIndex-1][1],cInterFace.lpReStartButton[ReStartIndex-1],0,0,44,44); //�ٽý��� ���� 
		}
		if(ReStartIndex== 1 && !RestartCheckExp ){
			DrawSprite(ReStartMainXY.x+ReStartButtonRect2[ReStartIndex-1][0],ReStartMainXY.y+ReStartButtonRect2[ReStartIndex-1][1],cInterFace.lpReStartButton[ReStartIndex-1],0,0,44,44); //�ٽý��� ���� 
		}
	}

	int i=0,j=0;

	//���λ��� ���
	DrawSprite(0-(256+128-sinMoveKindInter[SIN_MYSHOP]),90+sinInterHeight2,cShop.lpShopMain2,0,0,344,264);	       
 
	//ĳ���� ����  
	DrawSprite(0-(256+128-sinMoveKindInter[SIN_CHARSHOP]),90+sinInterHeight2,cShop.lpShopMain2,0,0,344,264);	       
	
	DrawSprite(0-(256+128-sinMoveKindInter[SIN_SHOP]),90+sinInterHeight2,lpShopMain,0,0,344,264);	       
	//dsDrawTexImage( MatShop[0] , 0 - (256+128-sinMoveKindInter[SIN_SHOP]),  96  , 256, 256 , 255 );       
	//dsDrawTexImage( MatShop[2] , 256-(256+128-sinMoveKindInter[SIN_SHOP])-25, 96 ,128 , 256 , 255 );   

 	if(sinShopKind == SIN_WEAPON || sinShopKind == SIN_DEFENSE){    

		DrawSprite(239-(256+128-sinMoveKindInter[SIN_SHOP]),87,lpTabAttack_D,0,0,41,45);	
		DrawSprite(280-(256+128-sinMoveKindInter[SIN_SHOP]),87,lpTabDefense_D,0,0,41,45);	
		DrawSprite(ShopButtonPosi[SINALLREPAIR-1][0]-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINALLREPAIR-1][1],lpAllRepair_D,0,0,28,28); 
		DrawSprite(ShopButtonPosi[SINREPAIR-1][0]-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINREPAIR-1][1],lpRepair_D,0,0,28,28);
		
		DrawSprite(225-(256+128-sinMoveKindInter[SIN_SHOP]),47,lpTitle_Attack,0,0,111,32);	

		if(sinShopKind == SIN_WEAPON)
			DrawSprite(239-(256+128-sinMoveKindInter[SIN_SHOP]),87,lpTabAttack,0,0,41,45);	
		if(sinShopKind == SIN_DEFENSE)
			DrawSprite(280-(256+128-sinMoveKindInter[SIN_SHOP]),87,lpTabDefense,0,0,41,45);	


		//if(SelectShopButtonPosi == 8)
		//	DrawSprite(231-(256+128-sinMoveKindInter[SIN_SHOP]),136,lpTabAttack,0,0,42,46);	

		//if(SelectShopButtonPosi == 9)
		//	DrawSprite(281-(256+128-sinMoveKindInter[SIN_SHOP]),136,lpTabDefense,0,0,42,46);	
			

	}

	if(sinShopKind == SIN_ETC){ 
		DrawSprite(232-(256+128-sinMoveKindInter[SIN_SHOP]),93,lpTabEtc_D,0,0,98,40);	
		DrawSprite(225-(256+128-sinMoveKindInter[SIN_SHOP]),47,lpTitle_Etc,0,0,111,32);	

	}
	DrawSprite(ShopButtonPosi[SINSELL-1][0]-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINSELL-1][1],lpSell_D,0,0,28,28);		
	DrawSprite(ShopButtonPosi[SINBUY-1][0]-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINBUY-1][1],lpBuy_D ,0,0,28,28);		

	//DrawSprite(231-(256+128-sinMoveKindInter[SIN_SHOP]),213,lpGoldEdit,0,0,99,24); //���� ���� ��ġ 
	DrawSprite(231-(256+128-sinMoveKindInter[SIN_SHOP]),243+sinInterHeight2,lpGoldEdit,0,0,99,24);

	DrawSprite(282-(256+128-sinMoveKindInter[SIN_SHOP]),182+sinInterHeight2,lpRightArrow,0,0,20,20); //ȭ��ǥ 
	DrawSprite(260-(256+128-sinMoveKindInter[SIN_SHOP]),182+sinInterHeight2,lpLeftArrow,0,0,20,20);

	//���� ȭ��ǥ 
	if(ShopArrowPosi == 0 || ShopArrowPosi == 3)   
		DrawSprite(260-(256+128-sinMoveKindInter[SIN_SHOP]),132,cInvenTory.lpInvenArrow[0],0,0,20,20);				
	if(ShopArrowPosi == 1 || ShopArrowPosi == 4)
		DrawSprite(282-(256+128-sinMoveKindInter[SIN_SHOP]),132,cInvenTory.lpInvenArrow[1],0,0,20,20);		


	//���λ��� 
	DrawSprite(ShopButtonPosi[SINALLREPAIR-1][0]-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopButtonPosi[SINALLREPAIR-1][1],lpMyShopNoSale_,0,0,28,28);   
	DrawSprite(ShopButtonPosi[SINREPAIR-1][0]-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopButtonPosi[SINREPAIR-1][1],lpMyShopSale_,0,0,28,28);
	//DrawSprite(ShopButtonPosi[SINBUY-1][0]-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopButtonPosi[SINBUY-1][1],lpMyShopExp_,0,0,28,28);		
	DrawSprite(238-(256+128-sinMoveKindInter[SIN_MYSHOP]),90,lpImage_MyShop,0,0,53,59);	
	DrawSprite(225-(256+128-sinMoveKindInter[SIN_MYSHOP]),47,lpTitle_MyShop,0,0,111,32);	


	//ĳ���� ����
	DrawSprite(ShopButtonPosi[SINBUY-1][0]-(256+128-sinMoveKindInter[SIN_CHARSHOP]),ShopButtonPosi[SINBUY-1][1],lpBuy_D,0,0,28,28);		
	DrawSprite(238-(256+128-sinMoveKindInter[SIN_CHARSHOP]),90,lpImage_MyShop,0,0,53,59);	
	DrawSprite(225-(256+128-sinMoveKindInter[SIN_CHARSHOP]),47,lpTitle_MyShop,0,0,111,32);	
 
	

	//��ġ�°� �Ƹ������ʾ� ������ ��¦�غ��� Ǫ�� -_-
	HDC	hdc;
	char strBuff[128];
	memset(strBuff,0,sizeof(strBuff));
	renderDevice.lpDDSBack->GetDC(&hdc);
	SelectObject( hdc, sinFont);
    SetBkMode( hdc, TRANSPARENT );
	SetTextColor( hdc, RGB(255,255,255) );

	int Money2 =0;

	int len3 = 0;

	if(cMyShop.OpenFlag){  
		for(int i = 0 ; i < 30 ; i++){
			if(cMyShop.MyShopItem[i].Flag){      
				if(cMyShop.MyShopItem[i].Class == ITEM_CLASS_POTION){
					Money2 += cMyShop.MyShopItem[i].sItemInfo.Money * cMyShop.MyShopItem[i].sItemInfo.PotionCount;

				}
				else{
					Money2 += cMyShop.MyShopItem[i].sItemInfo.Money;

				}
				
			}
		}

		NumLineComa(cMyShop.GetLimitMoney(Money2) , strBuff);
		len3 = lstrlen(strBuff);
		//wsprintf(strBuff,"%d",cMyShop.GetLimitMoney(Money2)); //�������� �ݾ�  
		dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[1][0],ShopGoldEdit[1][2], strBuff)-(256+128-sinMoveKindInter[SIN_MYSHOP])-(len3/2),ShopGoldEdit[1][1],
					strBuff,lstrlen(strBuff));

		memset(strBuff,0,sizeof(strBuff));
		NumLineComa(Money2 , strBuff);
		len3 = lstrlen(strBuff);
		//wsprintf(strBuff,"%d",Money2 ); //�Ǹ� �Ѿ� 
		dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[0][0],ShopGoldEdit[0][2], strBuff)-(256+128-sinMoveKindInter[SIN_MYSHOP])-(len3/2),ShopGoldEdit[0][1],
					strBuff,lstrlen(strBuff));

	}
	if(cCharShop.OpenFlag){
		memset(strBuff,0,sizeof(strBuff));
		NumLineComa(sinChar->Money , strBuff);
		len3 = lstrlen(strBuff);
		//wsprintf(strBuff,"%d",sinChar->Money); //�������� �ݾ�  
		dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[1][0],ShopGoldEdit[1][2], strBuff)-(256+128-sinMoveKindInter[SIN_MYSHOP])-(len3/2),ShopGoldEdit[1][1],
					strBuff,lstrlen(strBuff));

	}
	renderDevice.lpDDSBack->ReleaseDC(hdc);


	switch(SelectCharShopButtonPosi){    
		case 4:
			DrawSprite(ShopButtonPosi[SINBUY-1][0]-(256+128-sinMoveKindInter[SIN_CHARSHOP]),ShopButtonPosi[SINBUY-1][1],lpBuy ,0,0,28,28);		
			DrawSprite(ShopButtonPosi[SINBUY-1][0]-26-(256+128-sinMoveKindInter[SIN_CHARSHOP]),ShopButtonPosi[SINBUY-1][1]-27,lpBuyInfo,0,0,77,27);
		break;
		case SINSHOPEXIT:
			DrawSprite(304-(256+128-sinMoveKindInter[SIN_CHARSHOP]),313+sinInterHeight2,lpExit,0,0,20,20);	
			DrawSprite(314-10-(256+128-sinMoveKindInter[SIN_CHARSHOP]),313-27+sinInterHeight2,cInvenTory.lpExitInfo,0 ,0 , 47,27);
		break;

	}


	//����� �Ǹ� �������� Ȱ��ȭ ���ѳ��´�
	if(MyShopSendButton){
		DrawSprite(ShopButtonPosi[SINREPAIR-1][0]-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopButtonPosi[SINREPAIR-1][1],lpMyShopSale,0,0,28,28);			
	}

	switch(SelectMyShopButtonPosi){   
		case 1:  
			DrawSprite(ShopButtonPosi[SINALLREPAIR-1][0]-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopButtonPosi[SINALLREPAIR-1][1],lpMyShopNoSale,0,0,28,28);
			DrawSprite(ShopButtonPosi[SINALLREPAIR-1][0]-26-(256+128-sinMoveKindInter[SIN_MYSHOP])-8,ShopButtonPosi[SINALLREPAIR-1][1]-27,lpMyShopNoSale_T,0,0,100,26);
		break;
		
		case 2:
			DrawSprite(ShopButtonPosi[SINREPAIR-1][0]-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopButtonPosi[SINREPAIR-1][1],lpMyShopSale,0,0,28,28);			
			DrawSprite(ShopButtonPosi[SINREPAIR-1][0]-26-(256+128-sinMoveKindInter[SIN_MYSHOP])-8,ShopButtonPosi[SINREPAIR-1][1]-27,lpMyShopSale_T,0,0,100,26);

		break;
		case 4:
			//DrawSprite(ShopButtonPosi[SINBUY-1][0]-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopButtonPosi[SINBUY-1][1],lpMyShopExp,0,0,28,28);		
			//DrawSprite(ShopButtonPosi[SINBUY-1][0]-26-(256+128-sinMoveKindInter[SIN_MYSHOP])-8,ShopButtonPosi[SINBUY-1][1]-27,lpMyShopExp_T,0,0,100,26);
		break;
		case SINSHOPEXIT:
			DrawSprite(304-(256+128-sinMoveKindInter[SIN_MYSHOP]),313+sinInterHeight2,lpExit,0,0,20,20);	
			DrawSprite(314-10-(256+128-sinMoveKindInter[SIN_MYSHOP]),313-27+sinInterHeight2,cInvenTory.lpExitInfo,0 ,0 , 47,27);
		break;


	}
	
	switch(SelectShopButtonPosi){ 
		case SINALLREPAIR:
			if(sinShopKind != 3){ //���� ��簡 �ƴϸ� �׷��ش� 
				DrawSprite(ShopButtonPosi[SINALLREPAIR-1][0]-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINALLREPAIR-1][1],lpAllRepair,0,0,28,28);
				DrawSprite(ShopButtonPosi[SINALLREPAIR-1][0]-26-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINALLREPAIR-1][1]-27,lpRepairAllInfo,0,0,77,27);
			}
		break;
		case SINREPAIR:
			if(sinShopKind != 3){ //���� ��簡 �ƴϸ� �׷��ش� 
				DrawSprite(ShopButtonPosi[SINREPAIR-1][0]-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINREPAIR-1][1],lpRepair,0,0,28,28);		
				DrawSprite(ShopButtonPosi[SINREPAIR-1][0]-26-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINREPAIR-1][1]-27,lpRepairInfo,0,0,77,27);
			} 
		break;
		case SINSELL:
			DrawSprite(ShopButtonPosi[SINSELL-1][0]-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINSELL-1][1],lpSell,0,0,28,28);		
			DrawSprite(ShopButtonPosi[SINSELL-1][0]-26-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINSELL-1][1]-27,lpSellInfo,0,0,77,27);
		break;
		case SINBUY:
			DrawSprite(ShopButtonPosi[SINBUY-1][0]-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINBUY-1][1],lpBuy ,0,0,28,28);		
			DrawSprite(ShopButtonPosi[SINBUY-1][0]-26-(256+128-sinMoveKindInter[SIN_SHOP]),ShopButtonPosi[SINBUY-1][1]-27,lpBuyInfo,0,0,77,27);
		break;
		case SINRIGHTARROW: 
			DrawSprite(282-(256+128-sinMoveKindInter[SIN_SHOP]),182+sinInterHeight2,cInvenTory.lpInvenArrow[1],0,0,20,20);		

		break;
		case SINLEFTARROW:		
			DrawSprite(260-(256+128-sinMoveKindInter[SIN_SHOP]),182+sinInterHeight2,cInvenTory.lpInvenArrow[0],0,0,20,20);				
		break;
		case SINSHOPEXIT:
			DrawSprite(304-(256+128-sinMoveKindInter[SIN_SHOP]),313+sinInterHeight2,lpExit,0,0,20,20);	
			DrawSprite(314-10-(256+128-sinMoveKindInter[SIN_SHOP]),313-27+sinInterHeight2,cInvenTory.lpExitInfo,0 ,0 , 47,27);
		
		break;
					
	}

	/*
	switch(SelectShopButtonPosi){ 
		case SINALLREPAIR:
			if(sinShopKind != 3){ //���� ��簡 �ƴϸ� �׷��ش� 
				DrawSprite(264-(256+128-sinMoveKindInter[SIN_SHOP]),306+sinInterHeight2,lpAllRepair,0,0,28,28);
				DrawSprite(264-26-(256+128-sinMoveKindInter[SIN_SHOP]),306-27+sinInterHeight2,lpRepairAllInfo,0,0,77,27);
			}
		break;
		case SINREPAIR:
			if(sinShopKind != 3){ //���� ��簡 �ƴϸ� �׷��ش� 
				DrawSprite(231-(256+128-sinMoveKindInter[SIN_SHOP]),306+sinInterHeight2,lpRepair,0,0,28,28);		
				DrawSprite(231-26-(256+128-sinMoveKindInter[SIN_SHOP]),306-27+sinInterHeight2,lpRepairInfo,0,0,77,27);
			} 
		break;
		case SINSELL:
			DrawSprite(231-(256+128-sinMoveKindInter[SIN_SHOP]),275+sinInterHeight2,lpSell,0,0,28,28);		
			DrawSprite(231-26-(256+128-sinMoveKindInter[SIN_SHOP]),275-27+sinInterHeight2,lpSellInfo,0,0,77,27);
		break;
		case SINBUY:
			DrawSprite(263-(256+128-sinMoveKindInter[SIN_SHOP]),275+sinInterHeight2,lpBuy ,0,0,28,28);		
			DrawSprite(263-26-(256+128-sinMoveKindInter[SIN_SHOP]),275-27+sinInterHeight2,lpBuyInfo,0,0,77,27);
		break;
		case SINRIGHTARROW: 
			DrawSprite(282-(256+128-sinMoveKindInter[SIN_SHOP]),182+sinInterHeight2,cInvenTory.lpInvenArrow[1],0,0,20,20);		

		break;
		case SINLEFTARROW:		
			DrawSprite(260-(256+128-sinMoveKindInter[SIN_SHOP]),182+sinInterHeight2,cInvenTory.lpInvenArrow[0],0,0,20,20);				
		break;
		case SINSHOPEXIT:
			DrawSprite(304-(256+128-sinMoveKindInter[SIN_SHOP]),313+sinInterHeight2,lpExit,0,0,20,20);	
			DrawSprite(314-10-(256+128-sinMoveKindInter[SIN_SHOP]),313-27+sinInterHeight2,cInvenTory.lpExitInfo,0 ,0 , 47,27);
		break;
					
	}
	*/

	//����Ҽ����� �������� üũ�Ѵ� 
	if(cShop.OpenFlag){ 
		for(int cnt = 0 ;cnt < 30 ; cnt++){
			if(ShowShopItem[cnt].Flag == 1){ //�κ��丮�� �������� ������ �������� �׸��� 
				if(ShowShopItem[cnt].sItemInfo.NotUseFlag){
					dsDrawColorBox( sinInvenColor[2] ,ShowShopItem[cnt].x-(256+128-sinMoveKindInter[SIN_SHOP]),ShowShopItem[cnt].y, ShowShopItem[cnt].w, ShowShopItem[cnt].h );
				}

			}
		}
	}


	if(ShowShopItemIndex){
		for(i=0; i<GridRow;i++){
			for(j=0;j<GridCol;j++){
				DrawSprite(StartGirdX+(22*i)-(256+128-sinMoveKindInter[SIN_SHOP]),StartGirdY+(22*j) ,lpGrid,0,0, 22,22); 

			}
		}
		dsDrawColorBox( sinInvenColor[0] ,ShopItemRect.left-(256+128-sinMoveKindInter[SIN_SHOP]),ShopItemRect.top, ShopItemRect.right, ShopItemRect.bottom );

	}

	for( i=0; i < 30 ; i++) //�����ټ��ִ� �ִ�ġ�� �������� �����ش�  
		if(ShowShopItem[i].Flag){
			DrawSprite(ShowShopItem[i].x-(256+128-sinMoveKindInter[SIN_SHOP]),ShowShopItem[i].y ,ShowShopItem[i].lpItem,0,0, ShowShopItem[i].w,ShowShopItem[i].h); 
			

		}


	//MyShopItem�� �׸���

	if(SelectMyShopItemIndex){  
		dsDrawColorBox( sinInvenColor[4] ,cMyShop.MyShopItem[SelectMyShopItemIndex-1].x-(256+128-sinMoveKindInter[SIN_MYSHOP]),
					cMyShop.MyShopItem[SelectMyShopItemIndex-1].y, cMyShop.MyShopItem[SelectMyShopItemIndex-1].w, cMyShop.MyShopItem[SelectMyShopItemIndex-1].h );	
	}

	if(cMyShop.OpenFlag){   
		for(i = 0 ; i < 30 ; i++){
			if(cMyShop.MyShopItem[i].Flag){
				DrawSprite(cMyShop.MyShopItem[i].x-(256+128-sinMoveKindInter[SIN_MYSHOP]) , cMyShop.MyShopItem[i].y, cMyShop.MyShopItem[i].lpItem ,0,0, cMyShop.MyShopItem[i].w , cMyShop.MyShopItem[i].h);
				if((cMyShop.MyShopItem[i].CODE & sinITEM_MASK2) == sinFO1 ){
					cInvenTory.DrawForceOrbEffect(cMyShop.MyShopItem[i].x-(256+128-sinMoveKindInter[SIN_MYSHOP]) , cMyShop.MyShopItem[i].y);
				}
			}
		}
	}


	int x , y;
	if(cMyShop.OpenFlag){  
		if(MyShopMouseItem.Flag){
			if(!sMessageBox2[MESSAGE_MYSHOP_ITEM].Flag){ //�޼��� �ڽ��� ��������쿡�� ��������ʴ´�(�߸� -_-)
				x = pCursorPos.x-(MyShopMouseItem.w/2);
				y = pCursorPos.y -(MyShopMouseItem.h/2);

				dsDrawOffsetArray = dsARRAY_TOP;
				DrawSprite(x , y , MyShopMouseItem.lpItem,0,0,MyShopMouseItem.w,MyShopMouseItem.h);
				dsDrawColorBox( RGBA_MAKE(0,0,128,125) ,x ,y,MyShopMouseItem.w ,MyShopMouseItem.h);
				DrawSprite(x , y , MyShopMouseItem.lpItem,0,0,MyShopMouseItem.w,MyShopMouseItem.h);
			}
		}
	}


	if(SelectMyShopItemIndex){  
		dsDrawColorBox( sinInvenColor[4] ,cCharShop.CharShopItem[SelectMyShopItemIndex-1].x-(256+128-sinMoveKindInter[SIN_CHARSHOP]),
					cCharShop.CharShopItem[SelectMyShopItemIndex-1].y, cCharShop.CharShopItem[SelectMyShopItemIndex-1].w, cCharShop.CharShopItem[SelectMyShopItemIndex-1].h );	
	}


	//CharShop �������� �׸���
	if(cCharShop.OpenFlag){   
		for(i = 0 ; i < 30 ; i++){
			if(cCharShop.CharShopItem[i].Flag){
				DrawSprite(cCharShop.CharShopItem[i].x-(256+128-sinMoveKindInter[SIN_CHARSHOP]) , cCharShop.CharShopItem[i].y, cCharShop.CharShopItem[i].lpItem ,0,0, cCharShop.CharShopItem[i].w , cCharShop.CharShopItem[i].h);
				if((cCharShop.CharShopItem[i].CODE & sinITEM_MASK2) == sinFO1 ){
					cInvenTory.DrawForceOrbEffect(cCharShop.CharShopItem[i].x-(256+128-sinMoveKindInter[SIN_CHARSHOP]), cCharShop.CharShopItem[i].y);
				}
			}
		}
	}


	//���� ��ư
	if(MyShopSendButton && !cMyShop.OpenFlag){    
		//DrawSprite(50,200, lpMyShop_Button ,0,0, 93 , 79);
		dsDrawTexImage(MatMyShop_Button,10,220,70,70,255);   
	}


	int BackUpPosX , BackUpPosY = 0;
	int BackUpButton2X , BackUpButton2Y = 0;
	int BackUpButton3X , BackUpButton3Y = 0;


	//ȫ������ �Է��Ѵ�
 	if(MyShopExpBox){              
		BackUpButton2X = 35;    
		BackUpButton2Y = 157;
		BackUpButton3X = 115;
		BackUpButton3Y = 157;

		BackUpPosX = BackStartPos.x;
		BackUpPosY = BackStartPos.y;

		BackStartPos.x = MyShopExpBoxX;
		BackStartPos.y = MyShopExpBoxY;

		Interface_clanmenu.DrawBox(BackStartPos.x,BackStartPos.y,2,3);	            
		dsDrawTexImage(MatHelpBoxImage[9],BackStartPos.x+7,BackStartPos.y+35,180,15,255);
		
		
        dsDrawTexImage(MatMyShopPRBoxLine,BackStartPos.x+28,BackStartPos.y+50,254,90,255);

		DrawSprite(BackStartPos.x+BackUpButton2X,BackStartPos.y+BackUpButton2Y, cMessageBox.lpButtonBox07,  0, 0, 48, 23, 1);
		DrawSprite(BackStartPos.x+BackUpButton3X,BackStartPos.y+BackUpButton3Y, cMessageBox.lpButtonBox07,  0, 0, 48, 23, 1);

		if(MyShopExpButtonOK)  
			DrawSprite(BackStartPos.x+BackUpButton2X+7,BackStartPos.y+BackUpButton2Y+3, cMessageBox.lpButtonYes07,  0, 0, 32, 16, 1);
		else DrawSprite(BackStartPos.x+BackUpButton2X+7,BackStartPos.y+BackUpButton2Y+3, cMessageBox.lpButtonYesGlay07,  0, 0, 32, 16, 1);
		if(MyShopExpButtonCancel)
			DrawSprite(BackStartPos.x+BackUpButton3X+7,BackStartPos.y+BackUpButton3Y+3, cMessageBox.lpButtonNo07,  0, 0, 32, 16, 1);
		else DrawSprite(BackStartPos.x+BackUpButton3X+7,BackStartPos.y+BackUpButton3Y+3, cMessageBox.lpButtonNoGlay07,  0, 0, 32, 16, 1);

		BackStartPos.x = BackUpPosX;
		BackStartPos.y = BackUpPosY;

	}


}
/*----------------------------------------------------------------------------*
*							     ����  
*-----------------------------------------------------------------------------*/	
DWORD dwBuyItemServerTime = 0;
int MyShopItemSellMoney2 = 0;
DWORD dwBuyCharShopTime = 0;
void cSHOP::Main()
{
	int i = 0; 
	//���λ��� �̿�� 1�ʵ����̸� �ش�.
	if(haCharShopDelayFlag){
		dwBuyCharShopTime++;
		if( dwBuyCharShopTime >= 1*70){
			haCharShopDelayFlag = 0;
			dwBuyCharShopTime = 0;
		}
	}

	cMyShop.AutoCloseShop();

	MyShopExpBoxX = 300;
	MyShopExpBoxY = 170;

	CheckShopNpcState();      //NPC���� ������� üũ�Ѵ� 
	SelectShopButtonPosi = 0; //�ʱ�ȭ 
	ShowShopItemIndex = 0;
	ShopItemPrice = 0;

	CloseMyShopFlag = 0;
	CloseCharShopFlag = 0;


	int BackUpPosX , BackUpPosY = 0;

	//ȫ������ �Է��Ѵ�
	MyShopExpButtonOK = 0;
	MyShopExpButtonCancel = 0;

	int BackUpButton2X , BackUpButton2Y = 0;
	int BackUpButton3X , BackUpButton3Y = 0;

	if(MyShopExpBox){ 
		BackUpPosX = BackStartPos.x; 
		BackUpPosY = BackStartPos.y;

		BackStartPos.x = MyShopExpBoxX;
		BackStartPos.y = MyShopExpBoxY;

		BackUpButton2X = 35;    
		BackUpButton2Y = 157;
		BackUpButton3X = 115;
		BackUpButton3Y = 157;

		if(pCursorPos.x >BackStartPos.x+BackUpButton2X && pCursorPos.x < BackStartPos.x+BackUpButton2X+32 && 
			pCursorPos.y >BackStartPos.y+BackUpButton2Y && pCursorPos.y < BackStartPos.y+BackUpButton2Y+16)
			MyShopExpButtonOK = 1;
		
		if(pCursorPos.x >BackStartPos.x+BackUpButton3X && pCursorPos.x < BackStartPos.x+BackUpButton3X+32 && 
			pCursorPos.y >BackStartPos.y+BackUpButton3Y && pCursorPos.y < BackStartPos.y+BackUpButton3Y+16)
			MyShopExpButtonCancel = 1;

		BackStartPos.x = BackUpPosX;
		BackStartPos.y = BackUpPosY;

	}

	/////////////���� ����
	if(cMyShop.OpenFlag){ 
		if(pCursorPos.x >= DownTradePosi[T_CLOSE_POSI][0] && pCursorPos.x <= DownTradePosi[T_CLOSE_POSI][2] &&
			pCursorPos.y >= DownTradePosi[T_CLOSE_POSI][1] && pCursorPos.y <= DownTradePosi[T_CLOSE_POSI][3]){
				CloseMyShopFlag  = 1;
		}
	}

	/////////////ĳ���� ����
	if(cCharShop.OpenFlag){
		if(pCursorPos.x >= DownTradePosi[T_CLOSE_POSI][0] && pCursorPos.x <= DownTradePosi[T_CLOSE_POSI][2] &&
			pCursorPos.y >= DownTradePosi[T_CLOSE_POSI][1] && pCursorPos.y <= DownTradePosi[T_CLOSE_POSI][3]){
				CloseCharShopFlag  = 1;
		}
	}

	SelectMyShopButtonPosi = 0; 
	if(cMyShop.OpenFlag){
		if(!MyShopExpBox){
			for( i=0;i<9;i++)
				if ( ShopButtonPosi[i][0] < pCursorPos.x  && ShopButtonPosi[i][2] > pCursorPos.x && 
					 ShopButtonPosi[i][1] < pCursorPos.y  && ShopButtonPosi[i][3] > pCursorPos.y && !MouseItem.Flag)
					 SelectMyShopButtonPosi = i+1;
		}
			
	}

	SelectCharShopButtonPosi = 0;
	if(cCharShop.OpenFlag){
		for( i=0;i<9;i++)
			if ( ShopButtonPosi[i][0] < pCursorPos.x  && ShopButtonPosi[i][2] > pCursorPos.x && 
				 ShopButtonPosi[i][1] < pCursorPos.y  && ShopButtonPosi[i][3] > pCursorPos.y && !MouseItem.Flag)
				 SelectCharShopButtonPosi = i+1;
			
	}


	int Index2 = 0;
	int BackUpX = 0 , BackUpY = 0; 
	SelectMyShopItemIndex = 0;
	if(cMyShop.OpenFlag){
		for(i=0 ; i < 30 ; i++){ 
			if(cMyShop.MyShopItem[i].Flag){
				if ( cMyShop.MyShopItem[i].x < pCursorPos.x  && cMyShop.MyShopItem[i].x + cMyShop.MyShopItem[i].w > pCursorPos.x  && 
						cMyShop.MyShopItem[i].y < pCursorPos.y  && cMyShop.MyShopItem[i].y + cMyShop.MyShopItem[i].h > pCursorPos.y ){
						Index2 = cMyShop.SearchShopItemToInven(&cMyShop.MyShopItem[i]);
						if(Index2){
							SelectMyShopItemIndex = i+1;
							BackUpX = cInvenTory.InvenItem[Index2-1].x;
							BackUpY = cInvenTory.InvenItem[Index2-1].y;
							cInvenTory.InvenItem[Index2-1].x = cMyShop.MyShopItem[i].x;
							cInvenTory.InvenItem[Index2-1].y = cMyShop.MyShopItem[i].y;
							sinShowItemInfoFlag = 1; //�������� ������ �����ش� 
							MyShopItemSellMoney2 = cMyShop.MyShopItem[i].sItemInfo.Money;
							cItem.ShowItemInfo(&cInvenTory.InvenItem[Index2-1],3,Index2-1);
							cInvenTory.InvenItem[Index2-1].x = BackUpX;
							cInvenTory.InvenItem[Index2-1].y = BackUpY;

						}
					}
			}
		}
	}
	if(cCharShop.OpenFlag){
		for(i=0 ; i < 30 ; i++){ 
			if(cCharShop.CharShopItem[i].Flag){
				if ( cCharShop.CharShopItem[i].x < pCursorPos.x  && cCharShop.CharShopItem[i].x + cCharShop.CharShopItem[i].w > pCursorPos.x  && 
						cCharShop.CharShopItem[i].y < pCursorPos.y  && cCharShop.CharShopItem[i].y + cCharShop.CharShopItem[i].h > pCursorPos.y ){
						SelectMyShopItemIndex = i+1;
						Index2 = cCharShop.SearchMyShopItem(cCharShop.CharShopItem[i].sItemInfo.CODE , cCharShop.CharShopItem[i].sItemInfo.ItemHeader.Head ,cCharShop.CharShopItem[i].sItemInfo.ItemHeader.dwChkSum);
						if(Index2){
							sinShowItemInfoFlag = 1; //�������� ������ �����ش� 
							MyShopItemSellMoney2 = sRecvMyShop.sMyShopItem[Index2-1].Price;
							cItem.ShowItemInfo(&cCharShop.CharShopItem[i],3);
						}
				}
			}
		}
	}


	//�������� �������� �춧 �����߻��� 10���� �ٽ� �ʱ�ȭ���ش� 
	if(BuyItemServerFlag){
	 	dwBuyItemServerTime++;
		if( dwBuyItemServerTime >= 10*70){
			BuyItemServerFlag = 0;
			dwBuyItemServerTime = 0;
		}

	}
	


	if(OpenFlag){
		for( i=0;i<9;i++)
			if ( ShopButtonPosi[i][0] < pCursorPos.x  && ShopButtonPosi[i][2] > pCursorPos.x && 
				 ShopButtonPosi[i][1] < pCursorPos.y  && ShopButtonPosi[i][3] > pCursorPos.y && !MouseItem.Flag)
				 SelectShopButtonPosi = i+1;
			
	}
	if(OpenFlag){ 
		for(i =0; i <30 ; i++){
			cInvenTory.CharOnlySetItem(&ShowShopItem[i]);		//ĳ������ ������
			cInvenTory.CheckRequireItemToSet(&ShowShopItem[i]); //�ɷ�ġ�� �´� ������
			if ( ShowShopItem[i].x < pCursorPos.x  && ShowShopItem[i].x + ShowShopItem[i].w > pCursorPos.x && 
				ShowShopItem[i].y < pCursorPos.y  && ShowShopItem[i].y + ShowShopItem[i].h > pCursorPos.y ){
				if(ShowShopItem[i].Flag){
					if(!MouseItem.Flag){
						ShopItemRect.left   = ShowShopItem[i].x;
						ShopItemRect.top    = ShowShopItem[i].y;
						ShopItemRect.right  = ShowShopItem[i].w;
						ShopItemRect.bottom = ShowShopItem[i].h;
					
						GridRow = ShowShopItem[i].w/22;
						GridCol = ShowShopItem[i].h/22;
						StartGirdX = ShowShopItem[i].x;
						StartGirdY = ShowShopItem[i].y;
						sinShowItemInfoFlag = 1;  

						ShopItemPrice = ShowShopItem[i].sItemInfo.Price; //������ �ִ´� 
	     				cItem.ShowItemInfo(&ShowShopItem[i] , 1); //������ 1
						ShowShopItemIndex = i+1;
					}
				}
			}
		}

	}
}
/*----------------------------------------------------------------------------*
*							     ���� 
*-----------------------------------------------------------------------------*/	
void cSHOP::Close()
{
	Release();
}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/	
void cSHOP::LButtonDown(int x , int y)
{
	int cnt = 0;
	sITEM TempItem;
	
	//������ Ŭ��
	if(MyShopSendButton && !cMyShop.OpenFlag){   
		if(x >= 10 && x <= 10+70 && y >= 220 && y<= 220+70){
			cInterFace.CheckAllBox(SIN_MYSHOP);
		}
		//DrawSprite(50,200, lpMyShop_Button ,0,0, 93 , 79);
	}

	memset(&TempItem,0,sizeof(sITEM));
	
	MyShopItemCancelIndex[0] = 0; 
	MyShopItemCancelIndex[1] = 0;
	int Index3 = 0;

	if(cMyShop.OpenFlag&&!MyShopMouseItem.Flag){
		//��ϵ� �������� ����Ѵ�
		if(!MyShopSendButton){
			for(int i=0 ; i < 30 ; i++){ 
				if(cMyShop.MyShopItem[i].Flag){
					if ( cMyShop.MyShopItem[i].x < pCursorPos.x  && cMyShop.MyShopItem[i].x + cMyShop.MyShopItem[i].w > pCursorPos.x  && 
							cMyShop.MyShopItem[i].y < pCursorPos.y  && cMyShop.MyShopItem[i].y + cMyShop.MyShopItem[i].h > pCursorPos.y ){
							Index3 = cMyShop.SearchShopItemToInven(&cMyShop.MyShopItem[i]);
							if(Index3){
								cMessageBox.ShowMessage3(MESSAGE_MYSHOP_ITEM_CANCEL,cInvenTory.InvenItem[Index3-1].sItemInfo.ItemName);
								MyShopItemCancelIndex[0] = Index3;
								MyShopItemCancelIndex[1] = i+1;
								break;
							}

					}
				}
			}
		}
	}

	//���λ���
	if(cMyShop.OpenFlag){   
		if(MyShopMouseItem.Flag){
			if(!sinMyShopItemPass && !MyShopSendButton){
				if(!cMyShop.SetMyShopItem(&MyShopMouseItem))
					memset(&MyShopMouseItem,0,sizeof(sITEM)); //����� �ȵǸ������ش�
			}
		}
		if(CloseMyShopFlag){
			if(!MyShopSendButton){
				cMyShop.OpenFlag = 0;
				memset(cMyShop.MyShopItem,0,sizeof(sITEM)*30); //�������� �����ش�
				memset(MyShopItemIndex,0,sizeof(int)*100);
				MyShopSendButton = 0; //��Ϲ�ư �ʱ�ȭ
			}
			else{
				cMyShop.OpenFlag = 0;

			}
		}
	}
	
	//ĳ���� ����
	int Index2 = 0;
	if(cCharShop.OpenFlag){ 
		if(CloseCharShopFlag){
			cCharShop.OpenFlag = 0; //������ �ݴ´�
			memset(cCharShop.CharShopItem,0,sizeof(sITEM)*30); //�������� �����ش�

		}

		memset(&sMyShop_Server,0,sizeof(sMYSHOP_ITEM_SERVER));
		if(SelectMyShopItemIndex){ 
			if(!haCharShopDelayFlag){  //�ش繰���� �ŷ��Ϸ��Ŀ��� Ŭ���� �ȴ�.
				cCharShop.BuyItem(SelectMyShopItemIndex-1); //�ε����� ���Ŀ� �־��ش�
				haCharShopDelayFlag = 1;
			}
		}

/*
		if(SelectMyShopItemIndex && CursorClass == SIN_CURSOR_BUY){
			cCharShop.BuyItem(SelectMyShopItemIndex-1); //�ε����� ���Ŀ� �־��ش�

		}
*/
	}

	///////���� ȫ��
	if(MyShopExpButtonOK){ 
		lstrcpy(cMyShop.szSendDoc,cMyShop.szDoc);
		MyShopExpBox = 0; 
		isDrawClanMenu = 0;
		hFocusWnd = 0;  
		cInterFace.ChatFlag = 0;
		//SetWindowText( hFocusWnd , "");
		sinChatEnter = 0; //���Ƿ� �۾��� �Ⱥ��̰��Ѵ�
		sinMessageBoxShowFlag = 0; //�ٸ����������� �޼����ڽ� �÷��� �������� 
		MyShopSendButton = 1;
		cMyShop.SendMyShopItem();
		SetIME_Mode(0);

	} 
	if(MyShopExpButtonCancel){
		MyShopExpBox = 0; 
		isDrawClanMenu = 0;
		hFocusWnd = 0;  
		cInterFace.ChatFlag = 0;
		//SetWindowText( hFocusWnd , "");
		sinChatEnter = 0; //���Ƿ� �۾��� �Ⱥ��̰��Ѵ�
		sinMessageBoxShowFlag = 0; //�ٸ����������� �޼����ڽ� �÷��� �������� 
		//cMyShop.szSendDoc[0] = 0;  //�����κ��� �޼��� �ʱ�ȭ
		SetIME_Mode(0);
		
	}

	int j = 0;
	int CheckMyShopItemFlag = 0;

	//���� ����
	switch(SelectMyShopButtonPosi){
		case 1: //�Ǹ�����
			MyShopSendButton = 0;
			SendClosePersonalTrade();

		break;
		case 2: //�ǸŽ��� 
			for(j = 0 ; j < 100 ;j++){ 
				if(MyShopItemIndex[j]){ 
					CheckMyShopItemFlag = 1;
				}
			}

			if(CheckMyShopItemFlag){
				if(!MyShopSendButton ){
					//
					if(!MyShopSendButton){
						SetIME_Mode(1);
						MyShopExpBox = 1;   
						hFocusWnd = hTextWnd;  
						//SetWindowText( hFocusWnd , "");
						cInterFace.ChatFlag = 0;
						sinChatEnter = 0;
						sinMessageBoxShowFlag = 1;
					}

//					MyShopSendButton = 1;
//					cMyShop.SendMyShopItem();
				}
			}
			else{
				cMessageBox.ShowMessage(MESSAGE_NOT_SALE_ITEM);

			}

		break;
		case 4: //ȫ��
			/*
			if(!MyShopSendButton){
				MyShopExpBox = 1;   
				hFocusWnd = hTextWnd;  
				//SetWindowText( hFocusWnd , "");
				cInterFace.ChatFlag = 0;
				sinChatEnter = 0;
				sinMessageBoxShowFlag = 1;
			}
			*/
		break;

	}

	//ĳ���� ����
	switch(SelectCharShopButtonPosi){
		case 3: //���

		break;


	}


	sITEMPRICE sinItemPrice;
	
	if(ShowShopItemIndex){ //������ ��� 
		if(CursorClass == 1 || CursorClass == SIN_CURSOR_BUY){ 
			if(ShowShopItem[ShowShopItemIndex-1].Class == ITEM_CLASS_POTION){ //���� 
				pMessageItem = &ShowShopItem[ShowShopItemIndex-1];
				cMessageBox.ShowMessage2(MESSAGE_BUY_ITEM);
				BuyItemIndexCopy = ShowShopItemIndex;

			}
			else{ //�׳� ������ 
				pMessageItem = &ShowShopItem[ShowShopItemIndex-1];
				cMessageBox.ShowMessage2(MESSAGE_BUY_ITEM_DEFALT);
				BuyItemIndexCopy = ShowShopItemIndex;
			}
			
		}

	}

	switch(SelectShopButtonPosi){
		case 1: //�ٰ�ġ��
			if(sinShopKind == 3)break; //���� �����ϰ�쿡�� break;
			for(cnt = 0 ; cnt < INVENTORY_MAXITEM ; cnt++){
				if(cInvenTory.InvenItem[cnt].Flag){
//					if(cInvenTory.InvenItem[cnt].ItemPosition){
						sinItemPrice = cInvenTory.GetInvenItemPrice(&cInvenTory.InvenItem[cnt]);
					//���� �ִ°� ��ŭ�� ��ģ�� 
					if(!cInvenTory.RepairInvenItem(&cInvenTory.InvenItem[cnt] ,sinItemPrice.RepairCost))  
						continue; 
//					}
				}
			}
			sinPlaySound(SIN_SOUND_REPAIR_ITEM);

		break;

		case 2: //��ġ�� 
			if(sinShopKind == 3)break; //���� �����ϰ�쿡�� break;
			CursorClass = SIN_CURSOR_REPAIR;
		break;

		case 3: //�ȱ� 
			CursorClass = SIN_CURSOR_SELL;

		break;

		case 4: //���ǻ��
			CursorClass = SIN_CURSOR_BUY;
			//cMessageBox.ShowMessage(MESSAGE_HELP_1);

		break;

		case 5: //������ ȭ��ǥ 
			ShopArrowPosi = 1;

			if(sinShopKind == 2)//�� �̸� 
				CopyShopItemToShow(ShowShopItemIndex2,1);

			else
				CopyShopItemToShow(ShowShopItemIndex2,0);


		break;
		case 6: //���� ȭ��ǥ 

			ShopArrowPosi =0;
			if(sinShopKind == 2)//�� �̸� 
				CopyShopItemToShow(0,1);

			else
				CopyShopItemToShow(0,0);

		break;

		case 7: //������ 
			OpenFlag = 0;

		break;

		case 8: //���ⱸ
			if(sinShopKind==3)break; //��ȭ�̸� �극��ũ 
			sinShopKind = 1;
			ShowShopItemIndex2 = 0;
			CopyShopItemToShow(0,0);   //����� 0-30
			ShopArrowPosi = 0;       //Tab�� ������ �ʱ�ȭ �Ѵ� 
		break;

		case 9: //��
			if(sinShopKind==3)break; //��ȭ�̸� �극��ũ 
			sinShopKind = 2;
			ShowShopItemIndex2 = 0;
			CopyShopItemToShow(0,1);   //���� 30-60  
			ShopArrowPosi = 0;       //Tab�� ������ �ʱ�ȭ �Ѵ� 

	
		break;
		default:
			if(MouseItem.Flag){    //�������� ��ġ ���� 
				CursorClass = 0; //���콺 Ŀ���� ���ش� 
			}
			else{
				CursorClass = 1;

			}
		break;
	}


	//ĳ���� ������ ���콺 Ŀ��
	if(cCharShop.OpenFlag){ 
		if(SelectCharShopButtonPosi == 4){
			CursorClass = SIN_CURSOR_BUY;

		}
	}

	//������������ �ʱ�ȭ�Ѵ�
	DeleteShopItem();

	/////////////////////////////////������ ��ġ�⸦ ���ֱ� ���� ���� �������� ���ÿ��θ� üũ���ش� 
	////////////��ȣ������ �̿����� !!!!���������� 
	if(MouseItem.Flag)
		cInvenTory.CheckRequireItemToSet(&MouseItem); //�������� ���õɼ��ִ����� üũ�Ѵ� 

}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cSHOP::LButtonUp(int x , int y)
{


}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/	
void cSHOP::RButtonDown(int x , int y)
{
	
	sITEM TempItem;
    memset(&TempItem,0,sizeof(sITEM));
	if(cCharShop.OpenFlag){
		if(SelectMyShopItemIndex){ 
			cCharShop.BuyItem(SelectMyShopItemIndex-1); //�ε����� ���Ŀ� �־��ش�
		}
	}

	if(ShowShopItemIndex){ //������ ��ư���� �������� ���
		if(CursorClass == 1 || CursorClass == SIN_CURSOR_BUY){ 
			if(ShowShopItem[ShowShopItemIndex-1].Class == ITEM_CLASS_POTION){
				pMessageItem = &ShowShopItem[ShowShopItemIndex-1];
				cMessageBox.ShowMessage2(MESSAGE_BUY_ITEM);
				BuyItemIndexCopy = ShowShopItemIndex;

			}
			else{
				if(!BuyItemServerFlag){
					SendBuyItemToServer(&ShowShopItem[ShowShopItemIndex-1]);
					
				}
			}
		}
	}
}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cSHOP::RButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	
void cSHOP::KeyDown()
{
	//������������ �ʱ�ȭ�Ѵ�
	DeleteShopItem();
}
/*----------------------------------------------------------------------------*
*	
*-----------------------------------------------------------------------------*/	
void cSHOP::CheckShopNpcState() 
{
	if(!DispEachMode){
		if(TalkNpcState){
			cShop.OpenFlag = 0;
			cInvenTory.OpenFlag = 0;
			TalkNpcState = 0;
		}

	}

}

//�������� �޾ƿ� �������� ������ ���������� �����Ѵ� 
void cSHOP::CopyShopItemToShow(int Index,int Kind)
{
	memset(ShowShopItem,0,sizeof(sITEM)*30); //������ �������� �ʱ�ȭ�Ѵ� 

	for(int i= 0 ; i < 30 -Index; i++){
		if(GetShopItemXY(&ShopItem[i+Index+(Kind*30)])){//�ѹ��� �����ִ� ������ 
			memcpy(&ShowShopItem[i],&ShopItem[i+Index+(Kind*30)],sizeof(sITEM));

		}
		else{//�������� ������ 
			ShowShopItemIndex2 = i+Index;
			break;

		}
			
	}

}
//�������� ���õ� ��ǥ�� ���ؿ´� 
int cSHOP::GetShopItemXY(sITEM *pItem)
{

	int cntx,cnty;
	int cx,cy;

	RECT	rect;
	int		flag;

	int StartX = 21;
	int StartY = 136+sinInterHeight2;

	cx = (22*SHOP_ROW)-pItem->w; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22*SHOP_COL)-pItem->h; //�������� �����ִ� ���� �� Y��ǥ

	for( cntx=StartX ; cntx <= StartX+cx ; cntx+=22){//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
		for( cnty=StartY ; cnty <= StartY+cy ; cnty+=22){ 
			flag = 0;
			for(int i=0; i < 30 ; i++){
				if(ShowShopItem[i].Flag){
					rect.left   = ShowShopItem[i].x;
					rect.right  = ShowShopItem[i].x+ShowShopItem[i].w;
					rect.top    = ShowShopItem[i].y;
					rect.bottom = ShowShopItem[i].y+ShowShopItem[i].h;

					if( cntx > rect.left-pItem->w && cntx< rect.right && 
						cnty > rect.top-pItem->h && cnty< rect.bottom )
						flag++;
				}
			}
			if ( flag==0 ) {
				pItem->x = cntx;
				pItem->y = cnty;
				return TRUE;
			}
		}
	}
	return FALSE;
}

//�������� �������� ��� 
int cSHOP::SendBuyItemToServer(sITEM *pItem , int ItemCount )
{
	if(!cInvenTory.CheckSetEmptyArea(pItem)){ 
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE); //���� �ʰ�  
		return FALSE;
	}
	//--------------------------------------------------------------------------//
	#ifdef HASIEGE_MODE
		//������ �������� ������ ���Խ� ��üũ
		if(cShop.haBuyMoneyCheck(pItem->sItemInfo.Price*ItemCount)==0){
	#else
		if((sinChar->Money - (pItem->sItemInfo.Price*ItemCount)) < 0){
	#endif
	//---------------------------------------------------------------------------//
		cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY); //������
		return FALSE;
	}
	if(pItem->sItemInfo.Weight+sinChar->Weight[0] > sinChar->Weight[1]){
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);    //�����ʰ� 
		return FALSE;

	}
	BuyItemServerFlag = 1; //�ߺ����� ������ �޼����� ������ �ʱ� ���ؼ� �÷��� �ش� 

	TRANS_BUY_SHOPITEM	TransBuyShopItem;

	TransBuyShopItem.code = smTRANSCODE_SHOPTITEM;
	TransBuyShopItem.size = sizeof(TRANS_BUY_SHOPITEM);
	TransBuyShopItem.ItemCount = ItemCount;
	memcpy( &TransBuyShopItem.sItem , pItem , sizeof(sITEM) );
	if ( smWsockDataServer ) {
		return smWsockDataServer->Send2( (char *)&TransBuyShopItem , TransBuyShopItem.size , TRUE );
	}
	return FALSE;
}

//������ ��� 
int cSHOP::RecvBuyItemToServer(sITEM *pItem ,int ItemCount )
{

	BuyItemServerFlag = 0;
	CheckCharForm();//���� 

	//���� ������ ��� �������д� 
    //--------------------------------------------------------------------------//
	#ifdef HASIEGE_MODE
		//������ �������뿡 ���� ������
		if(haBuyMoneyCheck(pItem->sItemInfo.Price*ItemCount)){
	#else
		if((sinChar->Money - (pItem->sItemInfo.Price*ItemCount)) >= 0){
	#endif
	//---------------------------------------------------------------------------//
	    //sinChar->Money -= (pItem->sItemInfo.Price*ItemCount); //�������� 
		sinMinusMoney(pItem->sItemInfo.Price*ItemCount,1);
		if(pItem->Class == ITEM_CLASS_POTION){ //������ ��� 
			pItem->sItemInfo.PotionCount = ItemCount; //��Ե� �ټ��� ������ üũ�Ѵ� 

		}
		ReformCharForm();//������ 
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 
		if(cInvenTory.CheckRequireItemToSet(pItem)){ //�ϴ� ��������� �ɼ��ִ��� üũ�Ѵ� 
			if(CheckItemForm(&pItem->sItemInfo)){
				ReformItem( &pItem->sItemInfo );  //������ ���� �ޱ� 
			}
			else{
				SendSetHackUser(3); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 
				return FALSE;
			}
			
			if(cInvenTory.AutoSetInvenItem(pItem)){ //�κ��丮�� ���� �������� ���ýÿ��� �÷��� �ش� 
				cInvenTory.SetItemToChar(); //�Ķ���� ���� 
				if(ShowShopItemIndex){
					ShowShopItem[ShowShopItemIndex-1].Flag = 0; //�������� �����ش� 
				}
				else{
					if(BuyItemIndexCopy){
						ShowShopItem[BuyItemIndexCopy-1].Flag = 0; //�������� �����ش� 

					}
				}
				BuyItemIndexCopy = 0;
				ShowShopItemIndex = 0; //������ �ε��� �ʱ�ȭ
				cInvenTory.CheckWeight();
				if(pItem->Class == ITEM_CLASS_POTION || pItem->Class == ITEM_CLASS_ECORE) //������ ��� 				
					pItem->Flag = 1; //�����ϰ�쿡�� �������� ������ �ʴ´�
				SaveGameData(); //���� ����Ÿ ���� 
				return TRUE;
			}
			else {
				//���� ����
				CheckCharForm();//���� 				
				//sinChar->Money +=(pItem->sItemInfo.Price*ItemCount); //������ ���������ô� ���� �ٽ� �������ش� 
				sinPlusMoney(pItem->sItemInfo.Price*ItemCount);
				ReformCharForm();//������ 
				SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

			}

		}
	}
	else
		cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY); //������ �޼��� 

	return FALSE;
}


//������ �������� �Ǵ� 
int cSHOP::SellItemToShop(sITEM *pItem,int ItemCount)
{

	////////////////////////// ġƮ�� �������̸� ���� ��´� 
	if(!CheckItemForm(&pItem->sItemInfo)){ 
		SendSetHackUser(4); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 
		pItem->Flag = 0;  //ġƮ�� �������� ���ֹ����� 
		CheckCharForm();//���� 
		//sinChar->Money -= pItem->SellPrice*ItemCount;
		sinMinusMoney(pItem->SellPrice*ItemCount);
		if(sinChar->Money < 0 )sinChar->Money = 0;
		ReformCharForm();//������ 
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ�
		sinPlaySound(pItem->SoundIndex);
		return FALSE;

	}
	//�����Ҽ��ִ� �ݾ��� �������ȶ��� �����Ѵ� 
	if(sinChar->Level <= 10){
		if(sinChar->Money+pItem->SellPrice*ItemCount > 200000){
			cMessageBox.ShowMessage(MESSAGE_OVER_MONEY);
			cInvenTory.ReFormInvenItem();
			return FALSE;

		}
	}
	else{
		switch(sinChar->ChangeJob){
			case 0:
				if((sinChar->Money +pItem->SellPrice*ItemCount) >=(sinChar->Level*200000)-1800000) {
					cMessageBox.ShowMessage(MESSAGE_OVER_MONEY);
					cInvenTory.ReFormInvenItem();
					return FALSE;
				}
				
			break;
			case 1:
				if( ( sinChar->Money + pItem->SellPrice*ItemCount) >=10000000 ){
					cMessageBox.ShowMessage(MESSAGE_OVER_MONEY);
					cInvenTory.ReFormInvenItem();
					return FALSE;
				}
			break;
			case 2:
				if( ( sinChar->Money + pItem->SellPrice*ItemCount) >=50000000 ){
					cMessageBox.ShowMessage(MESSAGE_OVER_MONEY);
					cInvenTory.ReFormInvenItem();
					return FALSE;
				}

			break;
			case 3:
				if( ( sinChar->Money + pItem->SellPrice*ItemCount) >=100000000 ){
					cMessageBox.ShowMessage(MESSAGE_OVER_MONEY);
					cInvenTory.ReFormInvenItem();
					return FALSE;
				}

			break;


		}

	}
/*
	CheckCharForm();//���� 
	//sinChar->Money += pItem->SellPrice*ItemCount;
	sinPlusMoney(pItem->SellPrice*ItemCount);
	ReformCharForm();//������ 
	SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ�
	sinPlaySound(pItem->SoundIndex);
	SendSaveThrowItem(&pItem->sItemInfo);
	pItem->Flag = 0;
	ResetInvenItemCode();		//�̺��丮 ������ �ڵ� �ʱ�ȭ **************************
*/

	//���ڵ� ����üũ 
	if(pItem->Class == ITEM_CLASS_WEAPON_TWO && CursorClass == SIN_CURSOR_SELL) //Ŀ���� �������� �Ȱ�쿡���ش�
		cInvenTory.PickUpTwoHandItem(pItem);

	//�Ǹ��� �������� ������ ����
	SellItemToServer( &pItem->sItemInfo , ItemCount );

	sinPlaySound(pItem->SoundIndex);
	pItem->Flag = 0;
	ResetInvenItemCode();		//�̺��丮 ������ �ڵ� �ʱ�ȭ **************************

	return TRUE;
}

//���� �ؽ�Ʈ�� ǥ���Ѵ� 
void cSHOP::DrawShopText()
{
	if(ReStartFlag)
	{
		HDC	hdc;
		char strBuff[128];
		memset(strBuff,0,sizeof(strBuff));
		renderDevice.lpDDSBack->GetDC(&hdc);
		SelectObject( hdc, sinFont);
		SetBkMode( hdc, TRANSPARENT ); 
		SetTextColor( hdc, RGB(255,255,255) );

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
		renderDevice.lpDDSBack->ReleaseDC(hdc);

	}

	if(!sinMoveKindInter[SIN_SHOP] && !sinMoveKindInter[SIN_MYSHOP] && !sinMoveKindInter[SIN_CHARSHOP])return; //������ ���������� ����

	HDC	hdc;
	char strBuff[128];
	memset(strBuff,0,sizeof(strBuff));
	renderDevice.lpDDSBack->GetDC(&hdc);
	SelectObject( hdc, sinFont);
    SetBkMode( hdc, TRANSPARENT ); 
	SetTextColor( hdc, RGB(255,255,255) );

	if(OpenFlag){   
		NumLineComa(sinChar->Money, strBuff);
		//wsprintf(strBuff,"%d",sinChar->Money); //���� 
		dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[1][0],ShopGoldEdit[1][2], strBuff)-(256+128-sinMoveKindInter[SIN_SHOP]),ShopGoldEdit[1][1],
					strBuff,lstrlen(strBuff));

		if(ShopItemPrice){
			memset(strBuff,0,sizeof(strBuff));
			////���� ������ ��������� ������ ǥ�����ش�.
			//--------------------------------------------------------------------------//
			#ifdef HASIEGE_MODE
				//������ �������뿡 ���� ������
				NumLineComa(haShopItemPrice(ShopItemPrice), strBuff);
			#else
				NumLineComa(ShopItemPrice, strBuff);
			#endif
			//---------------------------------------------------------------------------//
			
			//wsprintf(strBuff,"%d",ShopItemPrice); //���� ���� 
			dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[0][0],ShopGoldEdit[0][2], strBuff)-(256+128-sinMoveKindInter[SIN_SHOP]),ShopGoldEdit[0][1],
					strBuff,lstrlen(strBuff));
		}

	}
	int BackUpPosX , BackUpPosY = 0; 
	int Start = 0 , End = 0 ,Conut = 0;

	char szBuff2[64];

	if(cMyShop.OpenFlag){
		for(int i = 0 ; i < 30 ; i++){
			if(cMyShop.MyShopItem[i].Flag){      
				if(cMyShop.MyShopItem[i].Class == ITEM_CLASS_POTION){
					wsprintf(strBuff,"%d",cMyShop.MyShopItem[i].sItemInfo.PotionCount);
					dsTextLineOut(hdc,cMyShop.MyShopItem[i].x-(256+128-sinMoveKindInter[SIN_MYSHOP]) , cMyShop.MyShopItem[i].y,strBuff,lstrlen(strBuff));

				}
			
			}
		}
		/*
		wsprintf(strBuff,"%d",cMyShop.GetLimitMoney(Money2)); //�������� �ݾ�  
		dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[1][0],ShopGoldEdit[1][2], strBuff)-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopGoldEdit[1][1],
					strBuff,lstrlen(strBuff));

		wsprintf(strBuff,"%d",Money2 ); //�Ǹ� �Ѿ� 
		dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[0][0],ShopGoldEdit[0][2], strBuff)-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopGoldEdit[0][1],
					strBuff,lstrlen(strBuff));
					*/

	}

	/*
	if(cMyShop.OpenFlag){
		for(int i = 0 ; i < 30 ; i++){
			if(cMyShop.MyShopItem[i].Flag){  
				if(cMyShop.MyShopItem[i].Class == ITEM_CLASS_POTION){
					wsprintf(strBuff,"%d",cMyShop.MyShopItem[i].sItemInfo.PotionCount);
					dsTextLineOut(hdc,cMyShop.MyShopItem[i].x , cMyShop.MyShopItem[i].y,strBuff,lstrlen(strBuff));
					Money2 += sRecvMyShop.sMyShopItem[i].Price * cMyShop.MyShopItem[i].sItemInfo.PotionCount;

				}
				else{
					Money2 += sRecvMyShop.sMyShopItem[i].Price;

				}
				
			}
		}
		wsprintf(strBuff,"%d",Money2); //���� 
		dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[1][0],ShopGoldEdit[1][2], strBuff)-(256+128-sinMoveKindInter[SIN_SHOP]),ShopGoldEdit[1][1],
					strBuff,lstrlen(strBuff));

		wsprintf(strBuff,"%d",cMyShop.GetLimitMoney(Money2)); //���� ���� 
		dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[0][0],ShopGoldEdit[0][2], strBuff)-(256+128-sinMoveKindInter[SIN_SHOP]),ShopGoldEdit[0][1],
					strBuff,lstrlen(strBuff));

	}
	*/
	if(cCharShop.OpenFlag){ 
		for(int i = 0 ; i < 30 ; i++){
			if(cCharShop.CharShopItem[i].Flag){  
				if(cCharShop.CharShopItem[i].Class == ITEM_CLASS_POTION){
					wsprintf(strBuff,"%d",cCharShop.CharShopItem[i].sItemInfo.PotionCount);
					dsTextLineOut(hdc,cCharShop.CharShopItem[i].x-(256+128-sinMoveKindInter[SIN_CHARSHOP]) , cCharShop.CharShopItem[i].y,strBuff,lstrlen(strBuff));

				}
			}
		}
		/*
		wsprintf(strBuff,"%d",sinChar->Money); //�������� �ݾ�  
		dsTextLineOut(hdc,CheckEditSize(ShopGoldEdit[1][0],ShopGoldEdit[1][2], strBuff)-(256+128-sinMoveKindInter[SIN_MYSHOP]),ShopGoldEdit[1][1],
					strBuff,lstrlen(strBuff));
		*/

	}

	//ȫ������ �Է��Ѵ�
 	if(MyShopExpBox){   
		BackUpPosX = BackStartPos.x;
		BackUpPosY = BackStartPos.y; 

		BackStartPos.x = MyShopExpBoxX;
		BackStartPos.y = MyShopExpBoxY; 


		//��Ŵ� Ÿ��Ʋ ����
		
		SelectObject( hdc, sinBoldFont);   
		SetBkMode( hdc, TRANSPARENT ); 
		SetTextColor( hdc, RGB(65,177,240) );
		dsTextLineOut(hdc,BackStartPos.x+60,BackStartPos.y+20,MyShopExpDoc7,lstrlen(MyShopExpDoc7));

		sinChatEnter = 0; //���Ƿ� �۾��� �Ⱥ��̰��Ѵ�
		cInterFace.ChatFlag = 0;
		hFocusWnd = hTextWnd;  
		GetWindowText( hFocusWnd , cMyShop.szDoc , 128 );

		SelectObject( hdc, sinFont);
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, RGB(255,255,255) );


		int len = 0;
		int line = 1;
		len = lstrlen(cMyShop.szDoc);

		line += (len/20);  

		for(int  i = 0; i < len ; i++){     
			memset(szBuff2,0,sizeof(szBuff2));
			while(1){
				if(cMyShop.szDoc[End] == NULL)break;
				if(cMyShop.szDoc[End] & 0x80){ //�ѱ��̸�..
					End   += 2;
					Conut += 2;
				}
				else{ //�����̸�..
					End++;
					Conut++;
				}
				if(Conut > 20)break;
			}
			memcpy(szBuff2 , &cMyShop.szDoc[Start],End - Start); 
/*
//�ؿ� �߱�(���λ����̸�����)
#ifdef _LANGUAGE_CHINESE
			ConvertStringTHAI(szBuff2, lstrlen(szBuff2)+16);
#endif
*/
			dsTextLineOut(hdc,BackStartPos.x+32,BackStartPos.y+60+(i*20),szBuff2,lstrlen(szBuff2));

			if(cMyShop.szDoc[End] != NULL ){
				if(cMyShop.szDoc[End] == ' ')
					End += 1;
				Start = End;
				Conut = 0;

			}
			else break;
			
		}

		BackStartPos.x = BackUpPosX;
		BackStartPos.y = BackUpPosY;

	}
	renderDevice.lpDDSBack->ReleaseDC(hdc);
}

//������������ ����� 
void cSHOP::DeleteShopItem()
{
	if(!OpenFlag){
		if(cShop.ShopItem){
			memset(&cShop.ShopItem,0,sizeof(sITEM)*60); //������ ���� �ʱ�ȭ 
		}
		if(ShowShopItem){
			memset(&ShowShopItem,0,sizeof(sITEM)*30); //������ ���� �ʱ�ȭ 

		}
	}
}


//���� �������� üũ�� �ؼ� �޼����ڽ��� ����
int cSHOP::CheckHighRankItem(sITEM *pItem)
{
	// pluto ����, ����, ��, ������ �߰�
	DWORD Code[] = {sinOS1,sinQT1,sinSP1,sinGP1,sinQW1,sinGF1,sinMA1,sinMA2,sinSE1,sinBI1, sinBI2, sinWR1, sinDR1, sinPR1, sinPR2, sinPR3 };
	int Cnt = 0;
	while(1){
		if(!Code[Cnt])break;
		if((pItem->sItemInfo.CODE & sinITEM_MASK2) == Code[Cnt])return TRUE;
		Cnt++;

	}
	if((pItem->sItemInfo.CODE&sinITEM_MASK3) >sin30)return TRUE;
	if(pItem->sItemInfo.Level >= 41)return TRUE;
	if(pItem->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT ||
		pItem->sItemInfo.ItemKindCode == ITEM_KIND_AGING )return TRUE;

	return FALSE;
}

//�������� ���õ� ������ ã�´� 
int cMYSHOP::SetMyShopItem(sITEM *pItem)
{
	int i;
	int SetFlag = 0;
	int SetCount = 0; 
	
	int MyShopStartX = 21;  
	int MyShopStartY = 136-50;
	int MyShopEndX   = MyShopStartX + (22*9);
	int MyShopEndY   = MyShopStartY + (22*9);

	if(MyShopStartX <= pItem->x+11 && MyShopEndX > pItem->x && 
		MyShopStartY <=pItem->y+11 && MyShopEndY > pItem->y){ //�κ��丮 �ڽ��� �������� üũ 
			SetFlag = 1;

	}
	

	if(!SetFlag)return FALSE; 

	//�������ش�
	int cntx,cnty;
	int cx,cy;

	RECT	rect;
	int		flag;

	cx = (22*9)-pItem->w; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22*9)-pItem->h; //�������� �����ִ� ���� �� Y��ǥ

	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
	for( cntx=MyShopStartX ; cntx <= MyShopStartX+cx ; cntx+=22){
		for( cnty=MyShopStartY ; cnty <= MyShopStartY+cy ; cnty+=22){ 
			flag = 0;
			for( i=0 ; i<30 ; i++){
				if(MyShopItem[i].Flag){
					rect.left   = MyShopItem[i].x;
					rect.right  = MyShopItem[i].x+MyShopItem[i].w;
					rect.top    = MyShopItem[i].y;
					rect.bottom = MyShopItem[i].y+MyShopItem[i].h;

					if( cntx > rect.left-pItem->w && cntx< rect.right && 
						cnty > rect.top-pItem->h && cnty< rect.bottom )
						flag++;
				}
			}
			if ( flag==0 ) 
			{     
				if((pItem->CODE & sinITEM_MASK2) == sinPZ1 || (pItem->CODE & sinITEM_MASK2) == sinPZ2 || (pItem->CODE & sinITEM_MASK2) == sinBI1 ||
					(pItem->CODE & sinITEM_MASK2) == sinBI2 )
				{
					//if(pItem->sItemInfo.PotionCount == 1){
					cMessageBox.ShowMessage(MESSAGE_NOT_MYSHOP_ITEM);
					return TRUE;
					//}
						
				}
				else if( (pItem->CODE & sinITEM_MASK2) == sinDA1 || (pItem->CODE & sinITEM_MASK2) == sinDA2)
				{
					if( //��Ƽ�� �ڽ�Ƭ   //��Ű�� ĳ��������
						(pItem->CODE & sinITEM_MASK3) == sin31 || (pItem->CODE & sinITEM_MASK3) == sin32 || (pItem->CODE & sinITEM_MASK3) == sin33 || 
						(pItem->CODE & sinITEM_MASK3) == sin34 || 
						(pItem->CODE & sinITEM_MASK3) == sin35 || (pItem->CODE & sinITEM_MASK3) == sin36 || (pItem->CODE & sinITEM_MASK3) == sin37 || 
						(pItem->CODE & sinITEM_MASK3) == sin38 ||
						(pItem->CODE & sinITEM_MASK3) == sin39 || (pItem->CODE & sinITEM_MASK3) == sin40 || (pItem->CODE & sinITEM_MASK3) == sin41 || 
						(pItem->CODE & sinITEM_MASK3) == sin42 ||
						(pItem->CODE & sinITEM_MASK3) == sin43 || (pItem->CODE & sinITEM_MASK3) == sin44 || (pItem->CODE & sinITEM_MASK3) == sin45 || 
						(pItem->CODE & sinITEM_MASK3) == sin46 ||
						(pItem->CODE & sinITEM_MASK3) == sin54 || (pItem->CODE & sinITEM_MASK3) == sin55 ) // ����� - ������ ���� �߰� - ���λ��� ����
					{
						cMessageBox.ShowMessage(MESSAGE_NOT_MYSHOP_ITEM);
						return TRUE;
					}
				}
				else if( (pItem->CODE & sinITEM_MASK2) == sinOA2 ) // ����� - ���� �ϸ�(7��, 30��) �߰�
				{
					if( (pItem->CODE & sinITEM_MASK3) == sin31 || (pItem->CODE & sinITEM_MASK3) == sin32 ||
						(pItem->CODE & sinITEM_MASK3) == sin33  ||	// �庰 - ���� �ϸ�(1��))
						(pItem->CODE & sinITEM_MASK3) == sin34 )	// �庰 - ���� �ϸ�(1�ð�)
					{
						cMessageBox.ShowMessage(MESSAGE_NOT_MYSHOP_ITEM);
						return TRUE;
					}
				}
				else if( (pItem->CODE & sinITEM_MASK2) == sinDB1 )  // ����� - ���ǵ� ����(7��, 30��) �߰�
				{
					if( (pItem->CODE & sinITEM_MASK3) == sin31 || (pItem->CODE & sinITEM_MASK3) == sin32 || 
						 (pItem->CODE & sinITEM_MASK3) == sin33  || // �庰 - ���ǵ� ����(1��) �߰�
						  (pItem->CODE & sinITEM_MASK3) == sin34 ) // �庰 - ���ǵ� ����(1�ð�) �߰�
					{
						cMessageBox.ShowMessage(MESSAGE_NOT_MYSHOP_ITEM);
						return TRUE;
					}
				}

				else if( (pItem->CODE & sinITEM_MASK2) == sinOR2 )  // �庰 - ��Ʈ��(7��) �߰�
				{
					if( (pItem->CODE & sinITEM_MASK3) == sin33 )
					{
						cMessageBox.ShowMessage(MESSAGE_NOT_MYSHOP_ITEM);
						return TRUE;
					}
				}


				else if( (pItem->CODE & sinITEM_MASK2) == sinOA1 )  // �庰 - ������ �����(7��) �߰�
				{
					if( (pItem->CODE & sinITEM_MASK3) == sin36 )
					{
						cMessageBox.ShowMessage(MESSAGE_NOT_MYSHOP_ITEM);
						return TRUE;
					}
				}


				else if( (pItem->CODE & sinITEM_MASK2) == sinOA1 )  // �庰 - ĵ������ ��Ʈ�ƹķ�(7��) �߰�
				{
					if( (pItem->CODE & sinITEM_MASK3) == sin37 )
					{
						cMessageBox.ShowMessage(MESSAGE_NOT_MYSHOP_ITEM);
						return TRUE;
					}
				}

				else if( (pItem->CODE & sinITEM_MASK2) == sinGF1 )  // �庰 - ������� ã�ƶ� �ƹķ�
				{
					if( (pItem->CODE & sinITEM_MASK3) == sin07 ||  (pItem->CODE & sinITEM_MASK3) == sin08 )
					{
						cMessageBox.ShowMessage(MESSAGE_NOT_MYSHOP_ITEM);
						return TRUE;
					}
				}


				cMessageBox.ShowMessage2(MESSAGE_MYSHOP_ITEM);
				return TRUE;
			}
	
		}
	}
	cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
	return FALSE;
}

//�������� �����Ѵ�
int cMYSHOP::LastSetMyShopItem(sITEM *pItem)
{
	//�������ش�
	int cntx,cnty;
	int cx,cy;
	int MyShopStartX = 21;  
	int MyShopStartY = 136-50;
	int MyShopEndX   = MyShopStartX + (22*9);
	int MyShopEndY   = MyShopStartY + (22*9);

	RECT	rect;
	int		flag;

	cx = (22*9)-pItem->w; //�������� �����ִ� ���� �� X��ǥ 
	cy = (22*9)-pItem->h; //�������� �����ִ� ���� �� Y��ǥ

	

	//�ڵ尡 ���� �ʰų� �������� ������� ���� ���� �����Ҷ� �˻��ؼ� ��ġ�� ã�Ƴ��� 
	for( cntx=MyShopStartX ; cntx <= MyShopStartX+cx ; cntx+=22){
		for( cnty=MyShopStartY ; cnty <= MyShopStartY+cy ; cnty+=22){ 
			flag = 0;
			for(int i=0 ; i<30 ; i++){
				if(MyShopItem[i].Flag){
					rect.left   = MyShopItem[i].x;
					rect.right  = MyShopItem[i].x+MyShopItem[i].w;
					rect.top    = MyShopItem[i].y;
					rect.bottom = MyShopItem[i].y+MyShopItem[i].h;

					if( cntx > rect.left-pItem->w && cntx< rect.right && 
						cnty > rect.top-pItem->h && cnty< rect.bottom )
						flag++;
				}
			}
			if ( flag==0 ) {
				for(int  j = 0 ; j < 30 ; j++){
					if(!cMyShop.MyShopItem[j].Flag){
						memcpy(&cMyShop.MyShopItem[j],pItem,sizeof(sITEM));
						cMyShop.MyShopItem[j].x = cntx;
						cMyShop.MyShopItem[j].y = cnty;
						SetShopItemToInven(pItem);
						pItem->Flag = 0;
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

//�������� ����Ѵ�
int cMYSHOP::SetShopItemToInven(sITEM *pItem)
{
	int i = 0,j = 0;
	for(i = 0; i < INVENTORY_MAXITEM ; i++){
		if(cInvenTory.InvenItem[i].Flag){
			if(i+1 == pItem->sItemInfo.Index &&
				cInvenTory.InvenItem[i].sItemInfo.ItemHeader.Head == pItem->sItemInfo.ItemHeader.Head &&
				cInvenTory.InvenItem[i].sItemInfo.ItemHeader.dwChkSum == pItem->sItemInfo.ItemHeader.dwChkSum ){
					MyShopItemIndex[i] = 1; //��ϵ� �������� �迭�� 1�� ���� 
					return TRUE;
			}
		}
	}
	return TRUE;
}

//������ ��ϵ��ִ� �������� �κ��丮���� ã�´�
int cMYSHOP::SearchShopItemToInven(sITEM *pItem)
{
	int i = 0,j = 0;
	for(i = 0; i < INVENTORY_MAXITEM ; i++){
		if(cInvenTory.InvenItem[i].Flag){
			if(i+1 == pItem->sItemInfo.Index &&
				cInvenTory.InvenItem[i].sItemInfo.ItemHeader.Head == pItem->sItemInfo.ItemHeader.Head &&
				cInvenTory.InvenItem[i].sItemInfo.ItemHeader.dwChkSum == pItem->sItemInfo.ItemHeader.dwChkSum 
				){
					return i+1;
			}
		}
	}
	return FALSE;
}

//������ �������� ������ 
int cMYSHOP::SendMyShopItem()
{

	int i = 0,j = 0,index=0;

	//�ʱ�ȭ
	memset(&sMyShop,0,sizeof(sMYSHOP)); 
	for(i = 0; i < 30 ; i++){
		if(cMyShop.MyShopItem[i].Flag){
			index = SearchShopItemToInven(&cMyShop.MyShopItem[i]);
			if(index){
				for(j = 0 ; j < 30 ;j++){
					if(!sMyShop.sMyShopItem[j].sItem.CODE){
						sMyShop.sMyShopItem[j].Price = cMyShop.MyShopItem[i].sItemInfo.Money;
						sMyShop.sMyShopItem[j].Posi[0] = cMyShop.MyShopItem[i].x;
						sMyShop.sMyShopItem[j].Posi[1] = cMyShop.MyShopItem[i].y;
						memcpy(&sMyShop.sMyShopItem[j].sItem,&cInvenTory.InvenItem[index-1].sItemInfo,sizeof(sITEMINFO));
						break;
					}
				}
			}
		}
	}

	//�������� ������ ������
	sMyShop.CHAR_CODE = lpCurPlayer->dwObjectSerial; //ĳ���� �ڵ�
	SendOpenPersonalTrade( cMyShop.szSendDoc , &sMyShop );
	//cCharShop.RecvShopItem(&sMyShop);

	return TRUE;
}

//��ϵ� �����۰� ������ ������������ �������� üũ�Ѵ�
int SerchsMyShopItem(sITEM *pItem)
{
	int i = 0,j = 0;
	for(i = 0; i < 30 ; i++){
		if(sMyShop.sMyShopItem[i].sItem.ItemHeader.Head == pItem->sItemInfo.ItemHeader.Head &&
			sMyShop.sMyShopItem[i].sItem.ItemHeader.dwChkSum == pItem->sItemInfo.ItemHeader.dwChkSum &&
			sMyShop.sMyShopItem[i].sItem.CODE == pItem->CODE){
				return i+1;
			}

	}
	return 0;

}

//�������� ���ſ�û�� ���� �������� �޴´�
int cMYSHOP::RecvMyShopItem( DWORD dwCharCode , sMYSHOP_ITEM_SERVER *lpShopItem )
{
	
	smCHAR *lpChar;
	char   szChatBuff[128];

	//CheckInvenEmpty
	int i = 0 , j = 0 , index = 0;
	memset(&sMyShop_Server,0,sizeof(sMYSHOP_ITEM_SERVER));
	DWORD 	MASK_CODE = 0;
	MASK_CODE = (lpShopItem->CODE & sinITEM_MASK2);

	int ExitItemFlag = 0;
	int index5 =0;

	switch(lpShopItem->SendFlag){
		case 1:  //���ſ�û 
			if(MyShopSendButton){ 
				for(i = 0; i < 30 ; i++){
					if(cMyShop.MyShopItem[i].Flag){
						index = SearchShopItemToInven(&cMyShop.MyShopItem[i]); 
						if(index){
							if(lpShopItem->CkSum == cInvenTory.InvenItem[index-1].sItemInfo.ItemHeader.dwChkSum &&
								lpShopItem->Head == cInvenTory.InvenItem[index-1].sItemInfo.ItemHeader.Head &&
								lpShopItem->CODE == cInvenTory.InvenItem[index-1].sItemInfo.CODE){

									ExitItemFlag = 1; //�������� ���翩��
								
								//�����κ��� �����۰� ���� ��ϵ� �������� üũ�Ѵ�
								index5 = SerchsMyShopItem(&cMyShop.MyShopItem[i]);
								if(!index5){
									//���������� ����Ÿ���۵� 
									break;

								}

								//������ ����������쿡
								if(MASK_CODE == sinPM1 || MASK_CODE == sinPL1 || MASK_CODE == sinPS1 ){
									if(lpShopItem->Price != sMyShop.sMyShopItem[index5-1].Price*lpShopItem->sTime){
										memcpy(&sMyShop_Server,lpShopItem,sizeof(sMYSHOP_ITEM_SERVER));
										sMyShop_Server.SendFlag = 7;
										sMyShop_Server.Price = sMyShop.sMyShopItem[index5-1].Price;
										Send_PersonalShopItem( sMyShop_Server.Buyer , &sMyShop_Server );
										UpdateMyShopList(&sMyShop);
										break;
									}

								}
								else{
									if(lpShopItem->Price != sMyShop.sMyShopItem[index5-1].Price){
										memcpy(&sMyShop_Server,lpShopItem,sizeof(sMYSHOP_ITEM_SERVER));
										sMyShop_Server.SendFlag = 7;
										sMyShop_Server.Price = sMyShop.sMyShopItem[index5-1].Price;
										Send_PersonalShopItem( sMyShop_Server.Buyer , &sMyShop_Server );
										UpdateMyShopList(&sMyShop);
										break;
									}
								}

								lpChar = FindAutoPlayer( dwCharCode ); //������ �簣 ������ ã�´�
								CheckCharForm();//���� 	
								//����
								if(MASK_CODE == sinPM1 || MASK_CODE == sinPL1 || MASK_CODE == sinPS1 ){
									if(cInvenTory.InvenItem[index-1].sItemInfo.PotionCount >= (int)lpShopItem->sTime){
										sinPlusMoney(lpShopItem->Price);
										cInvenTory.InvenItem[index-1].sItemInfo.PotionCount -=lpShopItem->sTime;
										cMyShop.MyShopItem[i].sItemInfo.PotionCount -=lpShopItem->sTime;
										sMyShop.sMyShopItem[index5-1].sItem.PotionCount -=lpShopItem->sTime;
										if(cMyShop.MyShopItem[i].sItemInfo.PotionCount == 0){
											cInvenTory.InvenItem[index-1].Flag = 0;
											cMyShop.MyShopItem[i].Flag = 0; //�������� �κ��ʱ�ȭ 
											memset(&sMyShop.sMyShopItem[index5-1],0,sizeof(sMYSHOP_ITEM)); //�������������κ� �ʱ�ȭ

										}
										if ( lpChar ) {
#ifdef _LANGUAGE_THAI
											wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
												cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->sTime,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_ENGLISH		
											wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
												lpShopItem->sTime,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_CHINESE
											wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
												lpShopItem->sTime,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_BRAZIL		
											wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
												lpShopItem->sTime,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_VEITNAM
											wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
												lpShopItem->sTime,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_ARGENTINA		
											wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
												lpShopItem->sTime,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
											wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
												cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price,lpShopItem->sTime);
#endif
#endif
#endif
#endif
#endif
#endif
											AddChatBuff( szChatBuff , 6 );

										}
										memcpy(&sMyShop_Server,lpShopItem,sizeof(sMYSHOP_ITEM_SERVER)); 
										sMyShop_Server.SendFlag = 2;

									}
									else{
										//����ó��
										memcpy(&sMyShop_Server,lpShopItem,sizeof(sMYSHOP_ITEM_SERVER)); 
										sMyShop_Server.sTime = cInvenTory.InvenItem[index-1].sItemInfo.PotionCount;
										cMyShop.MyShopItem[i].sItemInfo.PotionCount =cInvenTory.InvenItem[index-1].sItemInfo.PotionCount;
										sMyShop.sMyShopItem[index5-1].sItem.PotionCount =cInvenTory.InvenItem[index-1].sItemInfo.PotionCount;
										sMyShop_Server.SendFlag = 6;

									}

								}
								else{
									if ( lpChar ) {
#ifdef _LANGUAGE_THAI
										wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
											cInvenTory.InvenItem[index-1].sItemInfo.ItemName,1,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_ENGLISH		
										wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
											1,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_CHINESE
										wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
											1,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_BRAZIL
										wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
											1,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_VEITNAM		
										wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
											1,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_ARGENTINA
										wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
											1,cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price);
#else
										wsprintf(szChatBuff,BuyMyShopItem7,lpChar->smCharInfo.szName,
											cInvenTory.InvenItem[index-1].sItemInfo.ItemName,lpShopItem->Price,1);
#endif
#endif
#endif
#endif
#endif
#endif
										AddChatBuff( szChatBuff , 6 );

									}
									sinPlusMoney(lpShopItem->Price);
									cInvenTory.InvenItem[index-1].Flag = 0;
									cMyShop.MyShopItem[i].Flag = 0; //�������� �κ��ʱ�ȭ 
									memset(&sMyShop.sMyShopItem[index5-1],0,sizeof(sMYSHOP_ITEM)); //�������������κ� �ʱ�ȭ
									memcpy(&sMyShop_Server,lpShopItem,sizeof(sMYSHOP_ITEM_SERVER)); 
									sMyShop_Server.SendFlag = 2;

								} 
								Send_PersonalShopItem( dwCharCode , &sMyShop_Server );
								UpdateMyShopList(&sMyShop);
								cInvenTory.ReFormInvenItem();
								ReformCharForm();//������ 
								ResetInvenItemCode();
								cInvenTory.ReFormPotionNum();
								ResetPotion();
								cInvenTory.CheckWeight();
								SaveGameData();
								break;
							}
						}
					}
				}
				//�������� ���� 
				if(!ExitItemFlag){
					memcpy(&sMyShop_Server,lpShopItem,sizeof(sMYSHOP_ITEM_SERVER));
					sMyShop_Server.SendFlag = 3;
					Send_PersonalShopItem( sMyShop_Server.Buyer , &sMyShop_Server );
				}
			}
			else{
				memcpy(&sMyShop_Server,lpShopItem,sizeof(sMYSHOP_ITEM_SERVER));
				sMyShop_Server.SendFlag = 4;
				Send_PersonalShopItem( sMyShop_Server.Buyer , &sMyShop_Server );
			}			
		break;
		case 2:	//���� 
			//������ ������ �׸��� ������
			lpChar = FindAutoPlayer( dwCharCode ); //������ �Ǹ��� ������ ã�´�
			index = cCharShop.SearchMyShopItem(lpShopItem->CODE , lpShopItem->Head , lpShopItem->CkSum);
			if(index){ 
				CheckCharForm();//���� 				
				//����
				if(MASK_CODE == sinPM1 || MASK_CODE == sinPL1 || MASK_CODE == sinPS1 ){
					if ( lpChar ) {
#ifdef	_LANGUAGE_THAI	
						wsprintf(szChatBuff,BuyMyShopItem8,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,
							lpChar->smCharInfo.szName,lpShopItem->sTime,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_ENGLISH		
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							lpShopItem->sTime,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
//�߱�
#ifdef	_LANGUAGE_CHINESE
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							lpShopItem->sTime,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
#ifdef  _LANGUAGE_BRAZIL
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							lpShopItem->sTime,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_VEITNAM
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							lpShopItem->sTime,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
#ifdef  _LANGUAGE_ARGENTINA
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							lpShopItem->sTime,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price,lpShopItem->sTime);
#endif
#endif
#endif
#endif
#endif
#endif
						AddChatBuff( szChatBuff , 6 );

					}

					//������ ī��Ʈ ���� 
					sRecvMyShop.sMyShopItem[index-1].sItem.PotionCount = lpShopItem->sTime;
					cCharShop.CharShopItem[index-1].sItemInfo.PotionCount -= lpShopItem->sTime;
					sinMinusMoney(lpShopItem->Price);
					cCharShop.SetCharShopItemToInven(&sRecvMyShop.sMyShopItem[index-1].sItem);
					if(cCharShop.CharShopItem[index-1].sItemInfo.PotionCount == 0){
						cCharShop.CharShopItem[index-1].Flag = 0;
						memset(&sRecvMyShop.sMyShopItem[index-1].sItem,0,sizeof(sITEMINFO));
					}
				}
				else{
					if ( lpChar ) {
#ifdef	_LANGUAGE_THAI	
						wsprintf(szChatBuff,BuyMyShopItem8,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,
							lpChar->smCharInfo.szName,1,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_ENGLISH		
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							1,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
//�߱�
#ifdef	_LANGUAGE_CHINESE
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							1,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
#ifdef _LANGUAGE_BRAZIL
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							1,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
#ifdef	_LANGUAGE_VEITNAM
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							1,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
#ifdef _LANGUAGE_ARGENTINA
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							1,sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price);
#else
						wsprintf(szChatBuff,BuyMyShopItem8,lpChar->smCharInfo.szName,
							sRecvMyShop.sMyShopItem[index-1].sItem.ItemName,lpShopItem->Price,1);
#endif
#endif
#endif
#endif
#endif
#endif
						AddChatBuff( szChatBuff , 6 );
					}
					sinMinusMoney(lpShopItem->Price);
					cCharShop.CharShopItem[index-1].Flag = 0;
					cCharShop.SetCharShopItemToInven(&sRecvMyShop.sMyShopItem[index-1].sItem);
					memset(&sRecvMyShop.sMyShopItem[index-1].sItem,0,sizeof(sITEMINFO));

				}
				cInvenTory.ReFormInvenItem();
				ReformCharForm();//������ 
				ResetInvenItemCode();
				cInvenTory.ReFormPotionNum();
				ResetPotion(); //ResetPotion2 ���̳��� ResetPotion���� ���ش� 
				cInvenTory.CheckWeight();
				SaveGameData();

			}
		break;
		case 3: //ex) �ȸ�
			cMessageBox.ShowMessage(MESSAGE_MYSHOP_ITEM_DOWN);
		break;
		case 4: //��Ÿ ���
			cMessageBox.ShowMessage(MESSAGE_MYSHOP_CLOSE);
		break;
		case 5: //�Ǹ��ڸ� ã��������
			cMessageBox.ShowMessage(MESSAGE_MYSHOP_NOT_SEARCH);
		break;
		case 6: //���హ���� ��������
			index = cCharShop.SearchMyShopItem(lpShopItem->CODE , lpShopItem->Head , lpShopItem->CkSum);
			if(index){
				//����
				if(MASK_CODE == sinPM1 || MASK_CODE == sinPL1 || MASK_CODE == sinPS1 ){
					sRecvMyShop.sMyShopItem[index-1].sItem.PotionCount = lpShopItem->sTime;
					cCharShop.CharShopItem[index-1].sItemInfo.PotionCount = lpShopItem->sTime;
					cMessageBox.ShowMessage(MESSAGE_MYSHOP_POTION_NUM2);
				}
			}
		break;
		case 7: //���ݺ���
			index = cCharShop.SearchMyShopItem(lpShopItem->CODE , lpShopItem->Head , lpShopItem->CkSum);
			if(index){ 
				sRecvMyShop.sMyShopItem[index-1].Price = lpShopItem->Price;
				cCharShop.CharShopItem[index-1].sItemInfo.Price = lpShopItem->Price;
				cMessageBox.ShowMessage(MESSAGE_MYSHOP_CHANGE_PRICE);
			}
		break;

	}

	return TRUE;
}

int cCHARSHOP::RecvShopItem(sMYSHOP *sMyShop)
{
	
	char szFilePath[256];
	sITEM TempItem;
	for(int i=0; i < 30 ; i++){  
		if(sMyShop->sMyShopItem[i].Price){ //�ϴ��� �������� üũ
			for(int j=0 ; j < MAX_ITEM ; j++){
				if(sMyShop->sMyShopItem[i].sItem.CODE == sItem[j].CODE){
					wsprintf(szFilePath ,"Image\\sinImage\\Items\\%s\\it%s.bmp",sItem[j].ItemFilePath,sItem[j].LastCategory);
					memcpy(&TempItem ,&sItem[j],sizeof(sITEM));  //����ü�� ���ǵ� ������ �����Ѵ� 

					
					//���� �Ӵ� ����
				   if((sMyShop->sMyShopItem[i].sItem.CODE & sinITEM_MASK2) == sinPZ1 || (sMyShop->sMyShopItem[i].sItem.CODE & sinITEM_MASK2) == sinPZ2){
						if(sMyShop->sMyShopItem[i].sItem.PotionCount==2){
							wsprintf(szFilePath ,"Image\\sinImage\\Items\\%s\\it%s.bmp",sItem[j].ItemFilePath,sItem[j].LastCategory);
							memcpy(&TempItem ,&sItem[j],sizeof(sITEM));  //����ü�� ���ǵ� ������ �����Ѵ� 
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath); //�̹����� �����о��ش�
						}
						
					}

					memcpy(&TempItem.sItemInfo ,&sMyShop->sMyShopItem[i].sItem,sizeof(sITEMINFO)); //�ϴ� ������ �����Ѵ� 
					if(!sItem[j].lpTempItem) //�̹����� ������ �ε��Ѵ�
						sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
					TempItem.lpItem = sItem[j].lpTempItem; //������ �ִ� �̹��� �����͸� �־��ش� 
					TempItem.x = sMyShop->sMyShopItem[i].Posi[0];
					TempItem.y = sMyShop->sMyShopItem[i].Posi[1];
					memcpy(&CharShopItem[i],&TempItem,sizeof(sITEM));
					CharShopItem[i].Flag = 1;
					break;

				}
			}
		}
	}
	memcpy(&sRecvMyShop,sMyShop,sizeof(sMYSHOP));
	cInterFace.CheckAllBox(SIN_CHARSHOP);
	return TRUE;
}


//���� �������� ã�Ƽ� �ε����� �����Ѵ�
int cCHARSHOP::SearchMyShopItem(DWORD CODE ,DWORD Head , DWORD CheckSum)
{
	for(int i = 0 ; i < 30 ; i++){
		if(sRecvMyShop.sMyShopItem[i].sItem.ItemHeader.Head == Head &&
			sRecvMyShop.sMyShopItem[i].sItem.ItemHeader.dwChkSum == CheckSum &&
			sRecvMyShop.sMyShopItem[i].sItem.CODE == CODE ){
				return i+1;

		}
	}
	return FALSE;
	
}

//���λ��������� �������� �κ��丮�� �����Ѵ�
int cCHARSHOP::SetCharShopItemToInven(sITEMINFO *pItem_Info)
{
	char szTestBuff2[256];
	sITEM TempItem;
	szTestBuff2[0] = 0; //�����ʱ�ȭ  
	memset(&TempItem,0,sizeof(sITEM));
	for(int j=0 ; j < MAX_ITEM ; j++){ 
		if(pItem_Info->CODE == sItem[j].CODE){	
			wsprintf(szTestBuff2 ,"Image\\sinImage\\Items\\%s\\it%s.bmp",sItem[j].ItemFilePath,sItem[j].LastCategory);
			if(!sItem[j].lpTempItem)
				sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szTestBuff2);
			memcpy(&TempItem,&sItem[j],sizeof(sITEM));
			memcpy(&TempItem.sItemInfo,pItem_Info,sizeof(sITEMINFO));
			TempItem.lpItem = sItem[j].lpTempItem;
			TempItem.Flag =1;
			cInvenTory.CheckRequireItemToSet(&TempItem); 
			cInvenTory.AutoSetInvenItem(&TempItem);
			cInvenTory.ReFormInvenItem(); //�������� ����Ÿ�� �����Ѵ� 
			break;
		}
	}
	return TRUE;
}

//�ƾ����� ���
int cCHARSHOP::BuyItem(int Index)
{
	
	int Index2 = 0; 
	DWORD MASK_CODE = 0;
	memset(&MyShopPotion,0,sizeof(sITEMINFO));
	Index2 = cCharShop.SearchMyShopItem(cCharShop.CharShopItem[Index].sItemInfo.CODE , cCharShop.CharShopItem[SelectMyShopItemIndex-1].sItemInfo.ItemHeader.Head ,cCharShop.CharShopItem[Index].sItemInfo.ItemHeader.dwChkSum);
	if(Index2){
		sMyShop_Server.Buyer = lpCurPlayer->dwObjectSerial;
		sMyShop_Server.CkSum = sRecvMyShop.sMyShopItem[Index2-1].sItem.ItemHeader.dwChkSum;
		sMyShop_Server.Head  = sRecvMyShop.sMyShopItem[Index2-1].sItem.ItemHeader.Head;
		sMyShop_Server.Price = sRecvMyShop.sMyShopItem[Index2-1].Price;
		sMyShop_Server.sTime = sRecvMyShop.sMyShopItem[Index2-1].sItem.ItemHeader.dwTime;
		sMyShop_Server.CODE  = sRecvMyShop.sMyShopItem[Index2-1].sItem.CODE;
		sMyShop_Server.SendFlag = 1;
		TempItem.CODE = sRecvMyShop.sMyShopItem[Index2-1].sItem.CODE;

		//����� ����
		MASK_CODE = (TempItem.CODE & sinITEM_MASK2); 
		if(MASK_CODE == sinPM1 || MASK_CODE == sinPL1 || MASK_CODE == sinPS1 ){
			if(cInvenTory.CheckInvenEmpty(&TempItem)){
				memcpy(&MyShopPotion,&sRecvMyShop.sMyShopItem[Index2-1].sItem,sizeof(sITEMINFO));
				MyShopPotion.ItemHeader.dwVersion = sRecvMyShop.sMyShopItem[Index2-1].Price; //������ �������ִ´�
				MyShopPotion.PotionCount = cCharShop.CharShopItem[Index].sItemInfo.PotionCount; //�����ϴϱ� �ǵ�������
				cMessageBox.ShowMessage2(MESSAGE_MYSHOP_POTION);
				return TRUE;
			}
			else{
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);

			}
		}

		//����� �Ϲ� ������ 
		if( (sinChar->Money - (int)sMyShop_Server.Price) >=0 ){ 
			if(cInvenTory.CheckInvenEmpty(&TempItem)){
				if(sinChar->Weight[0]+sRecvMyShop.sMyShopItem[Index2-1].sItem.Weight <= sinChar->Weight[1] ){
					cMessageBox.ShowMessage3(MESSAGE_MYSHOP_ITEM_BUY,sRecvMyShop.sMyShopItem[Index2-1].sItem.ItemName);
				}
				else{
					cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
				}
			}
			else{
				cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);

			}
		}
		else{
			cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
		}

	}
	return TRUE;
}

//���������� �ݾ��� �����Ѵ�
int cMYSHOP::GetLimitMoney(int Money)
{

	int CheckMoney = 0;  
	if(sinChar->Level <= 10){
		 return 200000 - (Money - sinChar->Money);

	}
	else{
		switch(sinChar->ChangeJob){
			case 0:
				return (((sinChar->Level*200000)-1800000) - (Money + sinChar->Money));
					
			break;

			case 1:
				return ((10000000) - (Money + sinChar->Money));
				if( ( sinChar->Money + Money) > 10000000 )return TRUE;
			break;
			case 2:
				return ((50000000) - (Money + sinChar->Money));
			break;
			case 3:
				return ((100000000) - (Money + sinChar->Money));
			break;

		}
	}
	return FALSE;
}
//������ ���ȸ��� ������ �ݴ´�
int cMYSHOP::AutoCloseShop()
{
	int i = 0;
	int CheckFlag = 0;  
	if(MyShopSendButton){   
		CheckFlag = 1;
		for(i = 0 ; i < 30 ; i++){
			if(cMyShop.MyShopItem[i].Flag){
				CheckFlag = 2;

			}
		}
	}
	//�������ݴ´�
	if(MyShopSendButton){
		if(CheckFlag == 1){
			cMyShop.OpenFlag = 0;
			memset(cMyShop.MyShopItem,0,sizeof(sITEM)*30); //�������� �����ش�
			memset(MyShopItemIndex,0,sizeof(int)*100);
			MyShopSendButton = 0; //��Ϲ�ư �ʱ�ȭ
			SendClosePersonalTrade();
		}
	}
	return TRUE;
}

//���λ����� �Ѿ��� ���ؿ´� 
int cMYSHOP::GetTotalMyShopItemMoney()
{
	int TotalMoney = 0;
	if(cMyShop.OpenFlag){
		for(int i=0 ; i < 30 ; i++){ 
			if(cMyShop.MyShopItem[i].Flag){
				if(cMyShop.MyShopItem[i].Class == ITEM_CLASS_POTION){
					TotalMoney +=cMyShop.MyShopItem[i].sItemInfo.Money*cMyShop.MyShopItem[i].sItemInfo.PotionCount;
				}
				else{
					TotalMoney +=cMyShop.MyShopItem[i].sItemInfo.Money;
				}
			}
		}
	}
	return TotalMoney;
}
/*----------------------------------------------------------------------------*
*	             ������ �������濡 ���� �Լ� 
*-----------------------------------------------------------------------------*/
//<ha>������ �����Ҽ� �ִ��� üũ�Ѵ�.
int cSHOP::haBuyMoneyCheck(int BuyMoney)
{
	//ĳ���� �ӴϿ� ���� ������ ���Ѵ�.
	if(sinChar->Money-(BuyMoney+(BuyMoney*cSinSiege.GetTaxRate())/100)>=0){
		return TRUE;
	}
	return FALSE;
}
//<ha>���� ������ ��������� ������ ǥ�����ش�.
int cSHOP::haShopItemPrice(int Money)
{
	
	if(Money){
		Money+=((Money*cSinSiege.GetTaxRate())/100);
		return Money; 
	}
	return FALSE;
}