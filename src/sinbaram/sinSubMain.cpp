/*----------------------------------------------------------------------------*
*	���ϸ� :  sinSubMain.cpp	
*	�ϴ��� :  ���� �Լ����� �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "common.h"
#include "sinLinkHeader.h"
#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\Help.h"

#include "..\\FullZoomMap.h" //������

#ifdef _XTRAP_GUARD_4_CLIENT //HEAP TEST MEMORY
#include "..\\XTrapSrcD5\\ClientExt\\XTrap4ClientExt1.h"	//XTrapD5
#endif

/*----------------------------------------------------------------------------*
*								�������� 			
*-----------------------------------------------------------------------------*/	
HFONT sinFont = 0;   //��Ʈ 
HFONT sinMessageFont = 0;   //�޼��� ��Ʈ 
HFONT sinBoldFont = 0;   //�޼��� ��Ʈ 


int sinTestFlag;     //�׽�Ʈ �÷� 
int sinSecretFlag = 0;  //��ȣ �÷� 


int sinMouseButton[2] = { 0,0 };
int sinKeyClick[256];

int MouseButtonUpEvent = 0;   

int MatCursorDefault;       //������ �ȱ� Ŀ�� 
int MatCursorSell;          //������ �ȱ� Ŀ�� 
int MatCursorBuy;			//������ ��� Ŀ�� 
int MatCursorRepair;	   	//��ġ�� Ŀ�� �̹��� 	
int MatCursorAttack;		//���� 
int MatCursorGetItem1;		//������ �ݱ� 
int MatCursorGetItem2;		//������ �ݱ� 2
int MatCursorTalk;			//���ϱ� 

int CursorClass = 1;        //Ŀ������ (default�� 1�� �ش�  �������� ���� �� CursorClass = 0�����ش� )

int sinMoveKindInter[MAX_SIN_KIND_INTER]={0,0,0,0,0,0,0,0,0,0,0,0,0}; //���� ���� �����̴� �� // ������ - �ͽ��� ���� �߰��� ���� �ϳ��� �߰�

int sinSec = 0;         //�ʸ� ���Ѵ� 
int sinMainCounter = 0; //���η����� ���� ī��Ʈ���ش� 

float DeCreaSTM  = 0;
float InCreaSTM  = 0;
float InCreaLIFE = 0;
float InCreaMANA = 0;


char szTestBuff2[64]; //�̹��� �ε�� �̹����� ���� ���� Ȯ�����ش� 
POINT TestImageSize = {0,0};

int Accuracy_Table[][2] = {
	   
#include "sinTable.h"
	-1
};


int ItemIndex1[3] = {0,0,0};	//������ ���� ��ġ�� �ε��� ���� 
int ItemIndex2[3] = {0,0,0};

int ChangeJobButonCheck2 = 0; //��ư�ٿ��� �ѹ� �ųʶٴ� �÷�(�̰͵� ���� �����̴� ����!!)
int ItemImageNotExitFlag = 0;
int DownNum = 0;
/*----------------------------------------------------------------------------*
*							����  ���� ��� 
*-----------------------------------------------------------------------------*/	
char *sinSoundWav[] = { 
	"Image\\SinImage\\Sound\\interface-on.wav",		//0 (�������̽� First Start ���� )
	"Image\\SinImage\\Sound\\Axes.wav",				//1	(Axe)
	"Image\\SinImage\\Sound\\Claws.wav",			//2 (Claws)
	"Image\\SinImage\\Sound\\Hammer.wav",			//3 (Hammer)
	"Image\\SinImage\\Sound\\Magicial_Stuffs.wav",	//4 (Magicial_Stuffs)
	"Image\\SinImage\\Sound\\Poles.wav",			//5 (Poles)
	"Image\\SinImage\\Sound\\Shooters.wav",			//6 (Shooters)
	"Image\\SinImage\\Sound\\Swords.wav",			//7 (Swords)
	"Image\\SinImage\\Sound\\Throwing.wav",			//8 (Throwing)
	"Image\\SinImage\\Sound\\Armor.wav",			//9 (�κ�)
	"Image\\SinImage\\Sound\\Boots.wav",			//10 (Boots)
	"Image\\SinImage\\Sound\\Gloves.wav",			//11 (Gloves)
	"Image\\SinImage\\Sound\\Shields.wav",			//12 (Shields)
	"Image\\SinImage\\Sound\\Amulet.wav",			//13 (Amulet)
	"Image\\SinImage\\Sound\\Armlet.wav",			//14 (Armlet)
	"Image\\SinImage\\Sound\\Ring.wav",				//15 (Ring)
	"Image\\SinImage\\Sound\\Sheltom.wav",			//16 (Sheltom)
	"Image\\SinImage\\Sound\\Potion.wav",			//17 (Potion)
	"Image\\SinImage\\Sound\\Coin.wav",				//18 (Coin)
	"Image\\SinImage\\Sound\\Magicial_weapon.wav",	//19 (Magicial_weapon)
	"Image\\SinImage\\Sound\\drink1.wav",			//20 (����Դ� ����)
	"Image\\SinImage\\Sound\\interface.wav",		//21 (�������̽� ���� *ó�������� �����ϰ� ������ ����)
	"Image\\SinImage\\Sound\\repair.wav",			//22 (������ ����)
	"Image\\SinImage\\Sound\\sheltom-failure.wav",  //23 (�ͽ���, ����¡ ���н� ����)
	"Image\\SinImage\\Sound\\Armor-w.wav",          //24 (����)
	"Image\\SinImage\\Sound\\drink2.wav",           //25 (��¾� ����)
	0
};

/*----------------------------------------------------------------------------*
*					    ������� �ʱ�ȭ
*-----------------------------------------------------------------------------*/	
void InitSub()
{
	
	sinCreatFont(); //��Ʈ���� 	
}

/*----------------------------------------------------------------------------*
*						������� ����
*-----------------------------------------------------------------------------*/	
void MainSub()
{
	sinProc(SINKEYDOWN);		//Ű�Է��� ������ 	
	sinSec++;
	sinMainCounter++; //������ ī���� 

	if(sinSec >= 70){ // 1�ʸ��� �ѹ��� ���� 
		sinRegen();      //���׹̳���� (����� ���׹̳��� ����Ѵ� )
		sinUseStamina(); //���׹̳��� ����Ѵ� 
		sinSec = 0;

	}
	if((sinMainCounter&3) == 0) //70�� 1/4�϶� ���� 
		sinSetRegen();

	if((sinMainCounter % 70) == 0)
		AgingRing();

	////////////////////////////////////�̴ϸʶ����� �̰����λ���
	cSkill.CheckSkillMastery(); //�����͸��� üũ�Ѵ� 
	CheckContinueSkill(); //���������� ���� ��ų�� üũ�ؼ� �ð��� ������ �����ش� 
	

}

/*----------------------------------------------------------------------------*
*						������� �׸��� 
*-----------------------------------------------------------------------------*/	
void DrawSub()     
{

}

/*----------------------------------------------------------------------------*
*						������� ���� 
*-----------------------------------------------------------------------------*/	
void CloseSub()
{
    
}

/*----------------------------------------------------------------------------*
*							  Ŀ�� 
*-----------------------------------------------------------------------------*/	
void InitCursor()
{
	//Ŀ���̹��� �ε� 

	MatCursorDefault  =  CreateTextureMaterial( "Image\\SinImage\\Cursor\\DefaultCursor.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatCursorSell     =  CreateTextureMaterial( "Image\\SinImage\\Cursor\\SellCursor.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatCursorBuy      =  CreateTextureMaterial( "Image\\SinImage\\Cursor\\BuyCursor.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatCursorRepair   =  CreateTextureMaterial( "Image\\SinImage\\Cursor\\RepairCursor.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatCursorAttack	  =  CreateTextureMaterial( "Image\\SinImage\\Cursor\\Attack_Cursor.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );	//���� 
	MatCursorGetItem1 =  CreateTextureMaterial( "Image\\SinImage\\Cursor\\GetItem_Cursor1.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );   //������ �ݱ� 
	MatCursorGetItem2 =  CreateTextureMaterial( "Image\\SinImage\\Cursor\\GetItem_Cursor2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );		//������ �ݱ� 2
	MatCursorTalk     =  CreateTextureMaterial( "Image\\SinImage\\Cursor\\Talk_Cursor.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );			//���ϱ� 


}

void DrawCursor()
{
	POINT sinMousePosi;

	if(g_IsDxProjectZoomIn) //������   
	{
		sinMousePosi.x = g_iFzmCursorPosX;
		sinMousePosi.y = g_iFzmCursorPosY;
	}
	else
	{
		sinMousePosi.x = pCursorPos.x;
		sinMousePosi.y = pCursorPos.y;
	}

	switch(CursorClass)
	{
		case SIN_CURSOR_DEFAULT:
			dsDrawTexImage(MatCursorDefault , sinMousePosi.x , sinMousePosi.y ,32,32,255);
		break;
		case SIN_CURSOR_SELL:
			dsDrawTexImage(MatCursorSell , sinMousePosi.x , sinMousePosi.y ,32,32,255);

		break;

		case SIN_CURSOR_BUY:
			dsDrawTexImage(MatCursorBuy , sinMousePosi.x , sinMousePosi.y ,32,32,255);

		break;
		case SIN_CURSOR_REPAIR:
			dsDrawTexImage(MatCursorRepair , sinMousePosi.x , sinMousePosi.y ,32,32,255);

		break;
		case SIN_CURSOR_ATTACK:
			dsDrawTexImage(MatCursorAttack , sinMousePosi.x , sinMousePosi.y ,32,32,255);

		break;
		case SIN_CURSOR_GETITEM1:
			dsDrawTexImage(MatCursorGetItem1 , sinMousePosi.x , sinMousePosi.y ,32,32,255);
		break;
		case SIN_CURSOR_GETITEM2:
			dsDrawTexImage(MatCursorGetItem2 , sinMousePosi.x , sinMousePosi.y ,32,32,255);
		break;

		case SIN_CURSOR_TALK:
			dsDrawTexImage(MatCursorTalk , sinMousePosi.x , sinMousePosi.y ,32,32,255);

		break;
	}
}

/*----------------------------------------------------------------------------*
*				     Draw a sprite image
*-----------------------------------------------------------------------------*/	
void DrawSprite(int winX ,int winY,DIRECTDRAWSURFACE pdds,int x, int y, int width , int height,int Bltflag)
{
	if(!pdds) 
		return;

    RECT rcRect;

	if(Bltflag != 777)
		dsGetDrawOffset( &winX , &winY );

	int sx = width - x;
	int sy = height - y;

	int dx = WinSizeX - winX;
	int dy = WinSizeY - winY;

	int px = -winX;
	int py = -winY;

	if ( dx>sx ) dx = sx;
	if ( dy>sy ) dy = sy;
	if ( px<0)   px = 0;
	if ( py<0)   py = 0;

	rcRect.top		= y+py;
	rcRect.left		= x+px;
	rcRect.bottom   = y+dy ;
	rcRect.right    = x+dx ;

	if (Bltflag)
	{
		renderDevice.BltFast(renderDevice.lpDDSBack, winX + px, winY + py, pdds, &rcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		//renderDevice.lpDDSBack->BltFast(winX + px, winY + py, pdds, &rcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	}
	else
	{
		renderDevice.BltFast(renderDevice.lpDDSBack, winX, winY, pdds, &rcRect, DDBLTFAST_WAIT);
		//renderDevice.lpDDSBack->BltFast(winX, winY, pdds, &rcRect, DDBLTFAST_WAIT);
	}
}

/*----------------------------------------------------------------------------*
*		Come and rescue information (ASE) of the item and set a pointer (* szImagePath)
*-----------------------------------------------------------------------------*/	
int sinGetItemInfo(DWORD CODE , char *szImagePath)
{

	if(!szImagePath) return FALSE;

	for(int j=0 ; j < MAX_ITEM ; j++){
		if(sItem[j].CODE == CODE){	
			wsprintf(szImagePath ,"Image\\sinImage\\Items\\DropItem\\it%s.ASE",sItem[j].DorpItem);
			return TRUE;
		}
	}
	return FALSE;	
}

/*----------------------------------------------------------------------------*
*					   �������� Data�� �޴´� 
*-----------------------------------------------------------------------------*/	
extern int DecodeCompress( BYTE *lpSrcData , BYTE *lpDestData ); //���൥��Ÿ �ص� ( Z/NZ ��� )
int sinRecvMessage( DWORD dwCode , void *RecvBuff )
{
	sITEMINFO *lpItemInfo;
	smTRANS_SELL_ITEM		*lpTransSellItem;
	BYTE *lpBuff;
	int cnt;

	switch(dwCode) {
	case smTRANSCODE_SHOP_ITEMLIST:
		lpTransSellItem = (smTRANS_SELL_ITEM *)RecvBuff;

		lpItemInfo = new sITEMINFO[ lpTransSellItem->ItemCounter ];
		lpBuff = (BYTE *)RecvBuff+sizeof(smTRANS_SELL_ITEM);
		for( cnt=0;cnt<lpTransSellItem->ItemCounter;cnt++) {
			lpBuff += DecodeCompress( lpBuff , (BYTE *)&lpItemInfo[cnt] );
		}
		sinSetShopItem(lpItemInfo ,lpTransSellItem->ItemCounter, lpTransSellItem->ItemKind ); //���⸸ ���� 

		delete lpItemInfo;
		break;

	}

	return TRUE;
}
int RecvCountFlag = 0; //���� ���� �������� �ʱ�ȭ�� �ѹ��� �Ѵ� 

//������ ���� ������ �������� �޴´� 
int sinSetShopItem(sITEMINFO *pItem,int ItemCount, int ItemKind)
{
	//����Ʈ ���û����ϰ�� ���Ͻ�Ų��
	int i = 0;
	for(i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){        
		if(sSinHelp[i].KindFlag)return FALSE;
	}


	sITEM sinTempItem;  //�����ۿ� ���õ� ������ ��� �־�δ� Temp
	sinShopKind = ItemKind;

	if(sinShopKind == 2 ){ //���� �������´�(�ʱ�ȭ)
		sinShopKind = 1;   //Tab ������ ���� 

	}
	RecvCountFlag++;

	if(RecvCountFlag == 1)
		memset(&cShop.ShopItem,0,sizeof(sITEM)*60); //������ ���� �ʱ�ȭ 
	else
		RecvCountFlag = 0;
	if(sinShopKind == 3)RecvCountFlag=0; //���� 
	

	for(i=0;i< ItemCount; i++){
		if(LoadItemImage(&pItem[i],&sinTempItem)){
			if(ItemKind == 2 ) //�� �̸� 
				memcpy(&cShop.ShopItem[i+30],&sinTempItem,sizeof(sITEM));
			else //���� �ƴϸ� 
				memcpy(&cShop.ShopItem[i],&sinTempItem,sizeof(sITEM));

		}

	}
	TalkNpcState = 1; //������ ���� ��쿡 ..
	cShop.CopyShopItemToShow(0);
	cInterFace.CheckAllBox(SIN_SHOP);  //������ �κ��丮�� �����ش� 
	ShopArrowPosi = 0; //���� ȭ��ǥ �ʱ�ȭ 
	return TRUE;
}

/*----------------------------------------------------------------------------*
*			    ���콺 or �κ��丮�� �������� �����Ѵ� 
*-----------------------------------------------------------------------------*/	
int sinSetQuestItem(sITEMINFO *sItemInfo )
{
	
	sinSetSpecialItemCode(sItemInfo); //����Ⱦ������� �����Ѵ� 

	int sArrowPosi=0 , i=0;
	sITEM sinTempItem;  //�����ۿ� ���õ� ������ ��� �־�δ� Temp

	//if(cInvenTory.SearchItemCode((sinQT1|sin01)))return FALSE; //1���� �ۿ� ����������
	//if(cInvenTory.SearchItemCode((sinQT1|sin02)))return FALSE;
	//if(cInvenTory.SearchItemCode((sinQT1|sin03)))return FALSE;

	if(sinChar->ChangeJob == 0 && !sinQuest_ChangeJob.CODE && !sinQuest_ChangeJob2.CODE){
		if((sItemInfo->CODE == (sinQT1|sin01)) || (sItemInfo->CODE == (sinQT1|sin02)) || (sItemInfo->CODE == (sinQT1|sin03)) ||
		    (sItemInfo->CODE == (sinQT1|sin04)) || (sItemInfo->CODE == (sinQT1|sin05)) || (sItemInfo->CODE == (sinQT1|sin06))){
			return FALSE;
			}
	}

	//////////�� ����Ʈ�� �����ʴ� �������� �����Ҽ�����

	if(sinQuest_ChangeJob.CODE == SIN_QUEST_CODE_CHANGEJOB){
		if(cInvenTory.SearchItemCode(sItemInfo->CODE))return FALSE; //���� �ڵ��� �������� ������ �����Ѵ�
		if(sItemInfo->CODE == (sinQT1|sin04))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin05))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin06))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin16))return FALSE;

	}


	if(sinQuest_ChangeJob2.CODE == SIN_QUEST_CODE_CHANGEJOB2_NPC_M || 
		sinQuest_ChangeJob2.CODE == SIN_QUEST_CODE_CHANGEJOB2_NPC_D  ){
			if(sItemInfo->CODE == (sinQT1|sin01))return FALSE;
			if(sItemInfo->CODE == (sinQT1|sin02))return FALSE;
			if(sItemInfo->CODE == (sinQT1|sin03))return FALSE;
			if(sItemInfo->CODE == (sinQT1|sin06))return FALSE;

	}

	//����Ʈ�� ������ ������������ ���ð�� �����Ѵ�
	if(sinChar->ChangeJob >= 1 ){
		if((sItemInfo->CODE == (sinQT1|sin01)) || (sItemInfo->CODE == (sinQT1|sin02)) || (sItemInfo->CODE == (sinQT1|sin03)) ||
		    (sItemInfo->CODE == (sinQT1|sin04)) || (sItemInfo->CODE == (sinQT1|sin05))){
			return FALSE;
			}
	}

	if(sinQuest_ChangeJob3.CODE == SIN_QUEST_CODE_CHANGEJOB3){
		if(sItemInfo->CODE == (sinQT1|sin01))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin02))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin03))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin04))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin05))return FALSE;

	}
	if(!sinQuest_Level80_2.CODE){
		if(sItemInfo->CODE == (sinQT1|sin09))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin10))return FALSE;
		if(sItemInfo->CODE == (sinQT1|sin11))return FALSE;
	}
	else{
		if(cInvenTory.SearchItemCode(sinQT1|sin09))
			if(sItemInfo->CODE == (sinQT1|sin09))return FALSE;
		if(cInvenTory.SearchItemCode(sinQT1|sin10))
			if(sItemInfo->CODE == (sinQT1|sin10))return FALSE;
		if(cInvenTory.SearchItemCode(sinQT1|sin11))
			if(sItemInfo->CODE == (sinQT1|sin11))return FALSE;

	}

	//if(sItemInfo->CODE

	//����Ʈ ���⸦ �����Ŀ��� ������ ����������
	if(sItemInfo->CODE == (sinQT1|sin06) && sinQuest_ChangeJob3.State >=3){
		return FALSE;
	}

	//����Ʈ�� ������ ������������ ���ð�� �����Ѵ�
	if(sinChar->ChangeJob >= 2 && sItemInfo->CODE == (sinQT1|sin06)){
		return FALSE;
	}

	///////�ڵ带 ������ ��
	switch(sItemInfo->CODE & sinITEM_MASK2){
		case sinQT1:
			sItemInfo->ItemKindCode = ITEM_KIND_QUEST;

		break;
		case sinSP1:
			sItemInfo->ItemKindCode = ITEM_KIND_EVENT;
		break;
	}


	if(LoadItemImage(sItemInfo,&sinTempItem)){
		if(sinTempItem.sItemInfo.PotionCount <= 0 )     //����ī��Ʈ�� �����Ѵ�
			sinTempItem.sItemInfo.PotionCount = 1;

		if(!cInvenTory.AutoSetInvenItem(&sinTempItem,1)){ //����Ʈ�������� ���Կ� ������� �����Ҽ��ִ�
			if(ArrowState[0]==0)sArrowPosi = 2;
			else sArrowPosi = 1;
			sinButtonFlag = sArrowPosi;
			SelectInvenItemIndex = 0; //���׸� �ھ���� �������� �ʱ�ȭ
			cInvenTory.ChangeABItem(sArrowPosi); // 2���� ������ �κ� �ڽ� 
			if(!cInvenTory.AutoSetInvenItem(&sinTempItem,1)){
				//if(sinThrowItemToFeild(&sinTempItem)){          //�������� ������ 
				sinTempItem.Flag = 0;
				return FALSE;
				
			}
		}
	}

	return TRUE;
}

/*----------------------------------------------------------------------------*
*			    ���콺 or �κ��丮�� �������� �����Ѵ� 
*-----------------------------------------------------------------------------*/
//���� ���� ���� �÷�
int CheckPotionDelayFlag = 0;
int sinSetInvenItem(sITEMINFO *sItemInfo ) //�������� ������ �Լ��� ȣ��ȴ�
{

	//g_IsDxProjectZoomIn //�� �÷� ������

	SpecialItemShowFlag = 0; //������ �ݾ��ش�
	sinSetSpecialItemCode(sItemInfo); //����Ⱦ������� �����Ѵ�
	sITEM sinTempItem;  //�����ۿ� ���õ� ������ ��� �־�δ� Temp
	pMessageItem = 0;   //���� ������ ���õ� ������ �ʱ�ȭ (�������� ������ �ذ��̴� )


	if(LoadItemImage(sItemInfo,&sinTempItem)){ 
		if(sinTempItem.sItemInfo.PotionCount <= 0 )     //����ī��Ʈ�� �����Ѵ�
			sinTempItem.sItemInfo.PotionCount = 1;

		//������
		if(MouseItem.Flag || g_IsDxProjectZoomIn){ //���콺�� �������� ������쿡�� �κ��� ��ٷ� �����Ѵ� 
		//if(MouseItem.Flag ){ //���콺�� �������� ������쿡�� �κ��� ��ٷ� �����Ѵ� 
			if(cInvenTory.CheckRequireItemToSet(&sinTempItem)){         //��ٷ� ���õɼ��ִ����� üũ�Ѵ�  
				if(!cInvenTory.AutoSetInvenItem(&sinTempItem)){ //������ ���������� 
					if(sinThrowItemToFeild(&sinTempItem)){          //�������� ������ 
						sinTempItem.Flag = 0;
					}
				
				}
				else{ 
					cInvenTory.SetItemToChar(); //�������� ���õǸ� �ɷ�ġ�� �����Ѵ� 
					cInvenTory.CheckWeight();   //���Ը� �����Ѵ� 

				}
			}

		}
		else{
			if(cInvenTory.OpenFlag){  //���콺�� ���� �κ��� ����
				if(sinTempItem.Class == ITEM_CLASS_POTION){
					if(sinTempItem.sItemInfo.PotionCount + sinChar->Weight[0] >  sinChar->Weight[1]){
						cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
						sinOverWeightCnt++;
						sinThrowItemToFeild(&sinTempItem);          //�������� ������ 
						return FALSE;
					}
				}
				else{
					if(sinTempItem.sItemInfo.Weight+sinChar->Weight[0] >  sinChar->Weight[1]){
						cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
						sinOverWeightCnt++;
						sinThrowItemToFeild(&sinTempItem);          //�������� ������ 
						return FALSE;
					}
				}
				cInvenTory.CheckRequireItemToSet(&sinTempItem); //������ �ɷ�ġ�� ���밡���Ѱ��� üũ�Ѵ� 
				memcpy(&MouseItem , &sinTempItem , sizeof(sITEM));
				//memset(&sinTempItem,0,sizeof(sITEM));
				cInvenTory.ReFormMousePotionNum(); //���콺�� ������ ���É����� üũ�Ѵ�
			}

			else{  //�κ��丮�� ���� ���� 
				if(cInvenTory.CheckRequireItemToSet(&sinTempItem)){         //��ٷ� ���õɼ��ִ����� üũ�Ѵ�  
					if(!cInvenTory.AutoSetInvenItem(&sinTempItem)){ //������ ���������� 
						if(sinThrowItemToFeild(&sinTempItem)){          //�������� ������ 
							sinTempItem.Flag = 0;
						}
					
					}
					else{ 
						cInvenTory.SetItemToChar(); //�������� ���õǸ� �ɷ�ġ�� �����Ѵ� 
						cInvenTory.CheckWeight();   //���Ը� �����Ѵ� 
					}
				}
			}
		}
	}

	////////////�� ��ġ�� �������� 
	if(MouseItem.Flag){    //�������� ��ġ ���� 
		CursorClass = 0; //���콺 Ŀ���� ���ش� 
	}
	else{
		CursorClass = 1;

	}

	SaveGameData();
	return TRUE;
}


//����Ʈ �κ��丮�� ���õɾ�����
DWORD sinSpecialCODE[] = {(sinQT1|sin01),(sinQT1|sin02),(sinQT1|sin03),(sinQT1|sin04),(sinQT1|sin05),(sinQT1|sin06),
						(sinMA1|sin01),(sinMA2|sin01),(sinMA2|sin02),(sinQW1|sin01),(sinQW1|sin02),(sinQW1|sin03),
						(sinQW1|sin04),(sinQW1|sin05),(sinQT1|sin07),(sinQT1|sin08),
						(sinQT1|sin09),(sinQT1|sin10),(sinQT1|sin11),(sinQT1|sin12),(sinQT1|sin13),
						(sinQT1|sin14),(sinQT1|sin15),(sinQT1|sin16),(sinQW1|sin06),0};
//�ν�Ʈ ���Ϸ��� �ͽ�Ʈ���� �����߰� (sinQW1|sin06)
int sinSetSpecialItemCode(sITEMINFO *pItemInfo)
{
	int Count = 0;
	while(1){
		if(sinSpecialCODE[Count]){
			if(pItemInfo->CODE == sinSpecialCODE[Count]){
				pItemInfo->SpecialItemFlag[1] = ITEM_KIND_SPECIAL;
				break;

			}
			Count++;
		}
		else break;
	}
	cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
	return TRUE;
}

 
//�������� �̹����� �ε��Ѵ� 
int LoadItemImage(sITEMINFO *sItemInfo , sITEM *TempItem)
{
	char szFilePath[256];
	szTestBuff2[0] = 0; //�����ʱ�ȭ 
	for(int j=0 ; j < MAX_ITEM ; j++){  
		if(sItemInfo->CODE == sItem[j].CODE){	
			wsprintf(szFilePath ,"Image\\sinImage\\Items\\%s\\it%s.bmp",sItem[j].ItemFilePath,sItem[j].LastCategory);
			memcpy(TempItem ,&sItem[j],sizeof(sITEM));  //����ü�� ���ǵ� ������ �����Ѵ� 
			
			//���� �Ӵ� ����
			if((sItemInfo->CODE & sinITEM_MASK2) == sinPZ1 || (sItemInfo->CODE & sinITEM_MASK2) == sinPZ2){
				if(sItemInfo->PotionCount <= 1){
					for(int k=0 ; k < MAX_ITEM ; k++){  
						if((sinPZ1|sin00)== sItem[k].CODE){
							wsprintf(szFilePath ,"Image\\sinImage\\Items\\%s\\it%s.bmp",sItem[k].ItemFilePath,sItem[k].LastCategory);
							memcpy(TempItem ,&sItem[k],sizeof(sITEM));  //����ü�� ���ǵ� ������ �����Ѵ� 
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath); //�̹����� �����о��ش�
						}
					}
				}
			}

			memcpy(&TempItem->sItemInfo ,sItemInfo,sizeof(sITEMINFO)); //�ϴ� ������ �����Ѵ� 
			if(!sItem[j].lpTempItem) //�̹����� ������ �ε��Ѵ�
				sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
			TempItem->lpItem = sItem[j].lpTempItem; //������ �ִ� �̹��� �����͸� �־��ش� 
			TestImageSize.x = TempItem->w;  //�̹��� ����� ���� 
			TestImageSize.y = TempItem->h;
			if(!TempItem->lpItem){
				lstrcpy(szTestBuff2,sItem[j].LastCategory);
				ItemImageNotExitFlag = 1;
				cMessageBox.ShowMessage(MESSAGE_NOTEXIT_ITEMIMAGE);
				//	return FALSE;	//������ �ε����� �������� �����Ѵ� 
			}
			
			TempItem->Flag = 1;
			return TRUE;

		}

	}
	return FALSE;
}

/*----------------------------------------------------------------------------*
*				         Ű��ư  �� ���콺 �Լ� 
*-----------------------------------------------------------------------------*/	
void sinLButtonUp()
{
	MouseButtonUpEvent = SINLBUTTONUP;
}
void sinRButtonUp()
{
	MouseButtonUpEvent = SINRBUTTONUP;
}
void sinLButtonDown() 
{

}
/*----------------------------------------------------------------------------*
*					Ű�Է°� ���콺 �Է��� �ѹ����� �����Ѵ� 
*-----------------------------------------------------------------------------*/	
int sinGetMouseButton(int num)
{
	if ( MouseButton[num] && !sinMouseButton[num] ) {
		sinMouseButton[num] = TRUE;
		return TRUE;
	}
	return FALSE;
}
//-------------------------------------------------------------------------------
void sinMouseMain()
{
	int cnt;
	for( cnt=0;cnt<2;cnt++)	{
		if ( !MouseButton[cnt] ) {
			if ( sinMouseButton[cnt] ) sinMouseButton[cnt]=FALSE;
		}
	}
}
/*----------------------------------------------------------------------------*
*					Ű���� �Է��� �ѹ��� üũ�ϰ��Ѵ�
*-----------------------------------------------------------------------------*/	
int sinGetKeyClick(int num)
{
	if ( VRKeyBuff[num] && !sinKeyClick[num] ) {
		sinKeyClick[num] = TRUE;
		return TRUE;
	}


	return FALSE;
}
//-----------------------------------------------------------------------------
void sinKeyClickMain()
{
	int cnt;
	for( cnt=0;cnt<255;cnt++)	{
		if ( !VRKeyBuff[cnt] ) {
			if ( sinKeyClick[cnt] ) 
				sinKeyClick[cnt]=FALSE;
		}
	}
}

/*----------------------------------------------------------------------------*
*					    ���� �������̽��� �����ش� 
*-----------------------------------------------------------------------------*/	
void *szKindInter[] = { //void�� �����ͷ� �迭�� ��´� 
	&cCharStatus,
	&cInvenTory,
	&cShop,
	&cSkill,
	&cTrade,
	&cWareHouse,
	&cCraftItem,
	&cAging,
	&cMyShop,
	&cCharShop,
	&SmeltingItem,		// pluto ����
	&ManufactureItem,	// pluto ����
	&cMixtureReset,		// ������ - �ͽ��� ���� ��ü �߰�
	0,
};

class sinFlag{
public:
	int	OpenFlag;
};

int FirstMove[MAX_SIN_KIND_INTER] = {0,0,0,0,0,0,0,0,0,0,0,0,0}; // ������ - �ͽ��� ���� �߰��� ���� �ϳ��� �߰�

void ShowInterFace()
{
	int cnt = 0;
	int StopMove [MAX_SIN_KIND_INTER] = {0,0,0,0,0,0,0,0,0,0,0,0,0}; // ������ - �ͽ��� ���� �߰��� ���� �ϳ��� �߰�

	while(1){
		if(szKindInter[cnt] == 0)break;
		if(((sinFlag *)szKindInter[cnt])->OpenFlag > 0 ){
			if(!FirstMove[cnt]){
				if(cnt == SIN_SHOP || cnt == SIN_TRADE || cnt == SIN_WAREHOUSE  || cnt ==  SIN_CRAFTITEM || cnt == SIN_AGING || cnt == SIN_MYSHOP || 
					cnt == SIN_CHARSHOP || cnt == SIN_SMELTING || cnt == SIN_MANUFACTURE || cnt == SIN_MIXTURE_RESET){ // ������ - �ͽ��� ���� �߰�
					sinMoveKindInter[cnt] += 184+128;
					FirstMove[cnt] = 1;
				
				}
				else{
					sinMoveKindInter[cnt] += 184;
					FirstMove[cnt] = 1;
				}
			}
			else{
				if(cnt == SIN_SHOP || cnt == SIN_TRADE|| cnt == SIN_WAREHOUSE || cnt ==  SIN_CRAFTITEM || cnt == SIN_AGING || cnt == SIN_MYSHOP || 
					cnt == SIN_CHARSHOP || cnt == SIN_SMELTING || cnt == SIN_MANUFACTURE  || cnt == SIN_MIXTURE_RESET){ // ������ - �ͽ��� ���� �߰�
					if(sinMoveKindInter[cnt] >=256+128){
						sinMoveKindInter[cnt] = 256+128;
						StopMove[cnt] = 1;  //Stop�÷� �� ���� 

					}
					if(!StopMove[cnt]){
						sinMoveKindInter[cnt] += 7;
						if(sinMoveKindInter[cnt] >=200+128)sinMoveKindInter[cnt] -=2;
						if(sinMoveKindInter[cnt] >=230+128)sinMoveKindInter[cnt] -=2;
					}

				}
				else{
					if(sinMoveKindInter[cnt] >=256){
						sinMoveKindInter[cnt] = 256;
						StopMove[cnt] = 1;  //Stop�÷� �� ���� 

					}
					if(!StopMove[cnt]){
						sinMoveKindInter[cnt] += 7;
						if(sinMoveKindInter[cnt] >=200)sinMoveKindInter[cnt] -=2;
						if(sinMoveKindInter[cnt] >=230)sinMoveKindInter[cnt] -=2;
					}

				}

			}
		}
		else{
			if(cnt == 1){ //�κ��丮�ϰ�� (��� ���Ƶд�)
/*				CrashItemIndex[0] = 0; //�κ��丮�� ������ �ʱ�ȭ���ش� 
				CrashItemIndex[1] = 0;
				AutoSetItemIndex = 0;       //��չ��⸦ �����Ұ�� �ڵ����� ���õ� �������� �ε��� 
				SelectInvenItemIndex=0;		//���õ� �������� �ε��� 
*/
				SelectPotionIndex = 0;

			}

			if(sinMoveKindInter[cnt] > 0 ){
				sinMoveKindInter[cnt] -= 16;
			}
			else{
				sinMoveKindInter[cnt] = 0;
				FirstMove[cnt] = 0; //���� �÷��� �ʱ�ȭ �Ѵ� 

			}

		}
		cnt++;
	}
}

//��Ʈ ���� 
int sinCreatFont()
{

#ifdef	_LANGUAGE_CHINESE
	sinFont = CreateFont( 12,   //11,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
						GB2312_CHARSET,
                        //HANGEUL_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        //"����ü" );
						"SimSun" );

	sinMessageFont = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
						GB2312_CHARSET,
                        //HANGEUL_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        //"����ü" );
						"SimSun" );

	sinBoldFont    = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
						GB2312_CHARSET,
                        //HANGEUL_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        //"����ü" );
						"SimSun" );

#endif 
#ifdef _LANGUAGE_JAPANESE

#include "..\\Japanese\\j_font.h"

	sinFont = CreateFont( 11,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        FIXED_PITCH | FF_MODERN,
                        j_font );

	sinMessageFont = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        FIXED_PITCH | FF_MODERN,
                        j_font );

	sinBoldFont    = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        FIXED_PITCH | FF_MODERN,
                        j_font );

   
#endif

#ifdef _LANGUAGE_KOREAN


	sinFont = CreateFont( 11,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        HANGEUL_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "����ü" );

	sinMessageFont = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        HANGEUL_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "����ü" );

	sinBoldFont    = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        HANGEUL_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "����ü" );


#endif
#ifdef _LANGUAGE_TAIWAN
	sinFont = CreateFont( 11,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        CHINESEBIG5_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "MingLiu" );

	sinMessageFont = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        CHINESEBIG5_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "MingLiu" );

	sinBoldFont    = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        CHINESEBIG5_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "MingLiu" );


#endif

#ifdef _LANGUAGE_ENGLISH
	sinFont = CreateFont( 16,
                        6,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        DEFAULT_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Comic Sans MS" );

	sinMessageFont = CreateFont( 16,
                        7,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        DEFAULT_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Comic Sans MS" );

	sinBoldFont    = CreateFont( 16,
                        7,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        DEFAULT_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Comic Sans MS" );


#endif
#ifdef _LANGUAGE_THAI
#ifdef _CUSTOM_FONT
/*#ifdef	_ZHOON_TEST
	sinFont = CreateFont(z_fonttest.setdata[2].z_nHeight,
						z_fonttest.setdata[2].z_nWidth,
						z_fonttest.setdata[2].z_nEscapement,
						z_fonttest.setdata[2].z_nOrientation,
						z_fonttest.setdata[2].z_nWeight,
						z_fonttest.setdata[2].z_bItalic,
						z_fonttest.setdata[2].z_bUnderline,
						z_fonttest.setdata[2].z_cStrikeOut,
						z_fonttest.setdata[2].z_nCharSet,
						z_fonttest.setdata[2].z_nOutPrecision,
						z_fonttest.setdata[2].z_nClipPrecision,
						z_fonttest.setdata[2].z_nQuality,
                      z_fonttest.setdata[2].z_nPitchAndFamily,
						z_fonttest.setdata[2].z_lpszFacename);

	sinMessageFont = CreateFont(z_fonttest.setdata[3].z_nHeight,
						z_fonttest.setdata[3].z_nWidth,
						z_fonttest.setdata[3].z_nEscapement,
						z_fonttest.setdata[3].z_nOrientation,
						z_fonttest.setdata[3].z_nWeight,
						z_fonttest.setdata[3].z_bItalic,
						z_fonttest.setdata[3].z_bUnderline,
						z_fonttest.setdata[3].z_cStrikeOut,
						z_fonttest.setdata[3].z_nCharSet,
						z_fonttest.setdata[3].z_nOutPrecision,
						z_fonttest.setdata[3].z_nClipPrecision,
						z_fonttest.setdata[3].z_nQuality,
                      z_fonttest.setdata[3].z_nPitchAndFamily,
						z_fonttest.setdata[3].z_lpszFacename);

	sinBoldFont = CreateFont(z_fonttest.setdata[4].z_nHeight,
						z_fonttest.setdata[4].z_nWidth,
						z_fonttest.setdata[4].z_nEscapement,
						z_fonttest.setdata[4].z_nOrientation,
						z_fonttest.setdata[4].z_nWeight,
						z_fonttest.setdata[4].z_bItalic,
						z_fonttest.setdata[4].z_bUnderline,
						z_fonttest.setdata[4].z_cStrikeOut,
						z_fonttest.setdata[4].z_nCharSet,
						z_fonttest.setdata[4].z_nOutPrecision,
						z_fonttest.setdata[4].z_nClipPrecision,
						z_fonttest.setdata[4].z_nQuality,
                      z_fonttest.setdata[4].z_nPitchAndFamily,
						z_fonttest.setdata[4].z_lpszFacename);
#else*/

	sinFont = CreateFont( 13,
                        0,
                        0,
                        0,
						FW_THIN,
                        FALSE,
                        FALSE,
                        FALSE,
                        THAI_CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
                        DEFAULT_PITCH | FF_DONTCARE ,
                        "MS Sans Serif");

	sinMessageFont = CreateFont( 13,
                        0,
                        0,
                        0,
						FW_THIN,
                        FALSE,
                        FALSE,
                        FALSE,
                        THAI_CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
                        DEFAULT_PITCH | FF_DONTCARE,
                        "MS Sans Serif");

	sinBoldFont    = CreateFont( 13,
                        0,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        THAI_CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
                        DEFAULT_PITCH | FF_DONTCARE,
                        "MS Sans Serif");
//#endif
#else
	sinFont = CreateFont( 11,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        THAI_CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						DRAFT_QUALITY,
                        VARIABLE_PITCH|FF_SWISS,
                        "FreesiaUPC");

	sinMessageFont = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        THAI_CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						DRAFT_QUALITY,
                        VARIABLE_PITCH|FF_SWISS,
                        "FreesiaUPC");

	sinBoldFont    = CreateFont( 12,
                        0,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        THAI_CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						DRAFT_QUALITY,
                        VARIABLE_PITCH|FF_SWISS,
                        "FreesiaUPC");
#endif
#endif
#ifdef _LANGUAGE_VEITNAM
	sinFont = CreateFont( 16,
                        6,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        VIETNAMESE_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Tahoma" );

	sinMessageFont = CreateFont( 16,
                        7,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        VIETNAMESE_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Tahoma" );

	sinBoldFont    = CreateFont( 16,
                        7,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        VIETNAMESE_CHARSET,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Tahoma" );


#endif
#ifdef _LANGUAGE_BRAZIL
	sinFont = CreateFont( 16,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        ANSI_CHARSET|FS_LATIN1,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Comic Sans MS" );

	sinMessageFont = CreateFont( 16,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        ANSI_CHARSET|FS_LATIN1,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Comic Sans MS" );

	sinBoldFont    = CreateFont( 16,
                        0,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        ANSI_CHARSET|FS_LATIN1,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Comic Sans MS" );
#endif
//�Ƹ���Ƽ��
#ifdef _LANGUAGE_ARGENTINA
	sinFont = CreateFont( 16,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
						EASTEUROPE_CHARSET | FS_LATIN2,
						//ANSI_CHARSET|FS_LATIN1,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Tahoma" );

	sinMessageFont = CreateFont( 16,
                        0,
                        0,
                        0,
						FW_NORMAL,
                        FALSE,
                        FALSE,
                        FALSE,
                        EASTEUROPE_CHARSET | FS_LATIN2,
						//ANSI_CHARSET|FS_LATIN1,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Tahoma" );

	sinBoldFont    = CreateFont( 16,
                        0,
                        0,
                        0,
						FW_ULTRABOLD,
                        FALSE,
                        FALSE,
                        FALSE,
                        EASTEUROPE_CHARSET | FS_LATIN2,
						//ANSI_CHARSET|FS_LATIN1,
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY,
                        VARIABLE_PITCH,
                        "Tahoma" );
#endif
	return TRUE;


}

//�ؽ�Ʈ�� ���� ��ġ�� �����Ѵ�
int CheckEditSize(int x , int lx , char *String)
{
	int len = 0;  
	int ResultX = 0;
	int Temp=0,Temp2=0,Temp3=0;

	len = lstrlen(String);

	Temp = lx - x;
	Temp2 = (Temp - len*6)/2;

	return Temp2+x;

}

/*----------------------------------------------------------------------------*
*					����� ���������� ������ ����Ѵ� 
*-----------------------------------------------------------------------------*/	
#define SIN_POTION_LIFE			1
#define SIN_POTION_MANA			2
#define SIN_POTION_STAMINA		3
#define SIN_POTION_SONGPYEUN	4

int sinUsePotion() //����� ���������� ������ ����Ѵ� 
{
	
	CheckCharForm();//���� 
	
	int PotionKind = 0;
	int ResultPotion = 0;
	int TempRandPotion = 0;

	if(pUsePotion == 0) return FALSE;
	if(!pUsePotion->Flag)return FALSE;
	if(MouseItem.Flag){//���콺�� �����ִ� �����۰� ���� �������� ������ �����Ѵ� (�� �׷����� -_-; ���� ����̾ȳ��� �հ������������״� ���д�)
		if(MouseItem.sItemInfo.ItemHeader.Head == pUsePotion->sItemInfo.ItemHeader.Head)
			return FALSE;

	}

	if((pUsePotion->CODE & sinITEM_MASK2) ==sinPL1 ){ //�������ϰ�� 
		TempRandPotion = pUsePotion->sItemInfo.Life[1] -  pUsePotion->sItemInfo.Life[0];
		ResultPotion = pUsePotion->sItemInfo.Life[0]+(rand()%TempRandPotion);
		sinSetLife((sinGetLife()+ResultPotion));	//������ ���� 
		PotionKind = SIN_POTION_LIFE;
	}
	if((pUsePotion->CODE & sinITEM_MASK2) == sinPM1){ //�����ϰ�� 
		TempRandPotion = pUsePotion->sItemInfo.Mana[1] -  pUsePotion->sItemInfo.Mana[0];
		ResultPotion = pUsePotion->sItemInfo.Mana[0]+(rand()%TempRandPotion);
		sinSetMana((sinGetMana()+ResultPotion));	//������ ���� 
		PotionKind = SIN_POTION_MANA;
	}
	if((pUsePotion->CODE & sinITEM_MASK2) == sinPS1 ){ //���׹̳� �����ϰ�� 
		TempRandPotion = pUsePotion->sItemInfo.Stamina[1] -  pUsePotion->sItemInfo.Stamina[0];
		ResultPotion = pUsePotion->sItemInfo.Stamina[0]+(rand()%TempRandPotion);
		sinSetStamina((sinGetStamina()+ResultPotion));	//������ ���� 
		PotionKind = SIN_POTION_STAMINA;
	}
	if((pUsePotion->CODE & sinITEM_MASK2) == sinSP1)
	{
		if( (pUsePotion->CODE & sinITEM_MASK3) == sin01 || (pUsePotion->CODE & sinITEM_MASK3) == sin02 ||
			(pUsePotion->CODE & sinITEM_MASK3) == sin03 || (pUsePotion->CODE & sinITEM_MASK3) == sin15 || // ����� - ���� ������ �߰�
			(pUsePotion->CODE & sinITEM_MASK3) == sin35 ||  // �庰 - �߷�Ÿ�� ���ݸ� ������ �߰�
			 (pUsePotion->CODE & sinITEM_MASK3) == sin36 || (pUsePotion->CODE & sinITEM_MASK3) == sin42 ) // �庰 - ĵ������ // �庰 - ����
		{
			pUsePotion->sItemInfo.PotionCount = 0;			//���� ���� ī��Ʈ 0
			if(cInvenTory.EatSongPyeun(pUsePotion)){
				PotionKind = SIN_POTION_SONGPYEUN;
			}
			else
				PotionKind = 5;
		}
		// ��������
		if( (pUsePotion->CODE & sinITEM_MASK3) == sin05 || (pUsePotion->CODE & sinITEM_MASK3) == sin06 || (pUsePotion->CODE & sinITEM_MASK3) == sin07 ||
			(pUsePotion->CODE & sinITEM_MASK3) == sin08 || (pUsePotion->CODE & sinITEM_MASK3) == sin09 || (pUsePotion->CODE & sinITEM_MASK3) == sin10 )
		{
			PotionKind = 6;		// pluto ��������
		}

		// ����� - ĸ�� ������(ȣ���� ĸ�� ���)
		if( (pUsePotion->CODE & sinITEM_MASK3) == sin34 )
		{
			PotionKind = 7;		// ����� - ȣ���� ĸ�� �������� ����Ҷ� ��ȣ ����� �Ѵ�. 
		}

	}
	if(PotionKind){
		if(pUsePotion->sItemInfo.PotionCount >=2){ //������ ���� ���δ� 
			pUsePotion->sItemInfo.PotionCount--;
			/*
			if(( pUsePotion->sItemInfo.CODE & sinITEM_MASK3 ) == sin03 ||
				( pUsePotion->sItemInfo.CODE & sinITEM_MASK3 ) == sin04 ){
				sinPlaySound(SIN_SOUND_EAT_POTION2); //���� �Դ� �Ҹ� 
			}
			else
			*/
			sinPlaySound(SIN_SOUND_EAT_POTION); //���� �Դ� �Ҹ� 

		}
		else{
			pUsePotion->Flag =0;
			sInven[pUsePotion->ItemPosition-1].ItemIndex = 0; //������ �ʴ´� 
			if(PotionKind != 5){
			/*			
				if(( pUsePotion->sItemInfo.CODE & sinITEM_MASK3 ) == sin03 ||
					( pUsePotion->sItemInfo.CODE & sinITEM_MASK3 ) == sin04 ){
					sinPlaySound(SIN_SOUND_EAT_POTION2); //���� �Դ� �Ҹ� 
				}
				else
			*/

				sinPlaySound(SIN_SOUND_EAT_POTION); //���� �Դ� �Ҹ� 
			}

		} 
	
	pUsePotion = 0; //������ �ʱ�ȭ 		
	ReformCharForm();//������ 
	cInvenTory.CheckWeight(); //���Ը� üũ�Ѵ� 
	cInvenTory.ReFormPotionNum();	//���� ������ üũ�Ѵ� 
	cInvenTory.ReFormInvenItem();   //�κ��丮 ������ ����
	return PotionKind;
	}

	return FALSE;	

}

//�Źٶ� ��� 
void sinRegen()
{

	InCreaSTM  = 0;
	InCreaMANA = 0;
	InCreaLIFE = 0;

	float	fLevel=0;
	float	fStrength=0;
	float	fHealth=0;
	float	fSpirit=0;

	//���׹̳� ��� 
	if(lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_STAND){
		switch(sinChar->StaminaFunction){
			case 1:
				InCreaSTM = (float)(3.8 +sinChar->Level/7+sinChar->Stamina_Regen);
			break;
			case 2:
				InCreaSTM = (float)(3.3 +sinChar->Level/7+sinChar->Stamina_Regen);
			break;
			case 3:
				InCreaSTM = (float)(2.9 +sinChar->Level/7+sinChar->Stamina_Regen);
			break;
			default: //Ȥ�ó� �ؼ� �ϳ� �־�д� 
				InCreaSTM = (float)(3.8 +sinChar->Level/7+sinChar->Stamina_Regen);
			break;
		}

	}
	if(lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_WALK){
		switch(sinChar->StaminaFunction){
			case 1:
				InCreaSTM = (float)((3.8 +sinChar->Level/7+sinChar->Stamina_Regen)*0.6);
			break;
			case 2:
				InCreaSTM = (float)((3.3 +sinChar->Level/7+sinChar->Stamina_Regen)*0.6);
			break;
			case 3:
				InCreaSTM = (float)((2.9 +sinChar->Level/7+sinChar->Stamina_Regen)*0.6);
			break;
			default:
				InCreaSTM = (float)((3.8 +sinChar->Level/7+sinChar->Stamina_Regen)*0.6);
			break;

		}

	}
	fLevel = (float)sinChar->Level;
	fStrength = (float)sinChar->Strength;
	fHealth = (float)sinChar->Health;
	fSpirit = (float)sinChar->Spirit;

	
	if(lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD){
		//������ ���  
		InCreaLIFE = (float)(((fLevel+(fStrength/2)+fHealth)/180 + sinChar->Life_Regen)/1.5);
		//���� ��� 
		InCreaMANA = (fLevel+(fSpirit*1.2f)+(fHealth/2))/115 + sinChar->Mana_Regen;
	}

}

void sinUseStamina()
{

	DeCreaSTM = 0;
	if(lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_RUN){
		//DeCreaSTM = (float)(100+(sinChar->Weight[0]/2))/(100+(sinChar->Strength/5)); 
		//DeCreaSTM = (float)1000/(((sinChar->Weight[1] - sinChar->Weight[0])+(sinChar->Strength/2)+500)+0.5);
		DeCreaSTM = (float)((1000+sinChar->Weight[0]) / (sinChar->Weight[1]+(sinChar->Strength/2)+500)+0.4);
	
	}
}


float fTempLIFE = 0;
float fTempMANA = 0;
float fTempSTM_Incre  = 0;
float fTempSTM_Decre  = 0;

//���׹̳��� �����Ѵ� 
void sinSetRegen()
{
	
	CheckCharForm();//���� 

	//���׹̳� ���� 
	if(DeCreaSTM){ 
		fTempSTM_Decre += DeCreaSTM/(70/4);
		if(fTempSTM_Decre >=1){
			sinSetStamina(sinGetStamina()-(short)fTempSTM_Decre);
			fTempSTM_Decre -=1;
		}

	}
	//���׹̳� ���� 
	if(InCreaSTM){
		fTempSTM_Incre += InCreaSTM/(70/4);
		if(fTempSTM_Incre >= 1){
			sinSetStamina(sinGetStamina()+(short)fTempSTM_Incre);
			while(1){
				if(fTempSTM_Incre > 1)
					fTempSTM_Incre -=1;
				else break;

			}

		}

	}

	//������ ���� 
	if(InCreaLIFE){
		fTempLIFE += InCreaLIFE/(70/4);
		if(fTempLIFE >=1){
			sinSetLife(sinGetLife()+(short)fTempLIFE);
			while(1){
				if(fTempLIFE > 1)
					fTempLIFE -=1;
				else break;
			}

		}
		

	}
	
	//���� ���� 
	if(InCreaMANA){
		fTempMANA += InCreaMANA/(70/4);
		if(fTempMANA >= 1){
			sinSetMana(sinGetMana()+(short)fTempMANA);
			while(1){
				if(fTempMANA > 1)
					fTempMANA -=1;
				else break;
			}


		}

	}
	ReformCharForm();//������ 

}
/*----------------------------------------------------------------------------*
*				 (���� ������ ������ ���� �����Ѵ� )
*-----------------------------------------------------------------------------*/	
int sinGetAC(int AC)
{
	int cnt = 0;
	while(1){
		if(Accuracy_Table[cnt][0] == 0)break;
		if( AC > Accuracy_Table[cnt][0] && AC < Accuracy_Table[cnt+1][0] ){
			return Accuracy_Table[cnt+1][1];	
		}
		cnt++;
	}
	return FALSE;
}

int cnt  = 0;
int AC_R = 0;    //������� ���� �� 
int RealAC=0;    //���̺��� ���� ���� ��ġ 
int Result=0;
float fDesLV = 0;
float fMyLV  = 0;
float fWeifht[2] = {0,0};


//���߷��� �����Ѵ� (��Ȯ�ϴ�) (������ ������������ �ǽ�����������)
int sinGetAccuracy(int desLV , int desDefense)
{
	cnt = 0;
	AC_R = (int)(((float)sinChar->Attack_Rating - (float)desDefense)*1.4);

	while(1){
		if(AC_R <-190){
			RealAC = 50;
			break;
		}
		if(AC_R >2100){
			RealAC = 95;
			break;
		}
		if(Accuracy_Table[cnt][1] == 0)break; 
		if( AC_R >= Accuracy_Table[cnt][0] && AC_R <= Accuracy_Table[cnt+1][0] ){
			RealAC = Accuracy_Table[cnt+1][1];	
			break;

		}
		cnt++;
	}
	fDesLV = (float)desLV;
	fMyLV  = (float)sinChar->Level;
	fWeifht[0] = sinChar->Weight[0];
	fWeifht[1] = sinChar->Weight[1];


	//Result = (int)(RealAC - (((fDesLV - fMyLV)/100)*28)-(( fWeifht[0] / fWeifht[1])*3));
	Result = (int)(RealAC - ((fDesLV - fMyLV)/100)*28);

	if(Result < 30) Result = 30;
	if(Result > 95) Result = 95;
	return Result;
}

//���� ���߷�  
int sinGetMonsterAccuracy(int MonsterLV , int MonsterAttack_Rating) 
{
	cnt = 0;
	AC_R = (MonsterAttack_Rating - sinChar->Defence)*2;

	while(1){
		if(Accuracy_Table[cnt][0] == 0)break;
		if( AC_R > Accuracy_Table[cnt][0] && AC_R < Accuracy_Table[cnt+1][0] ){
			RealAC = Accuracy_Table[cnt+1][1];	
			break;

		}
		cnt++;
	}
	fDesLV = (float)sinChar->Level;
	fMyLV  = (float)MonsterLV;

	Result = (int)(RealAC - ((fDesLV - fMyLV)/100)*50);

	if(Result < 30) Result = 30;
	if(Result > 95) Result = 95;

	return Result;
}

//���� VS ���� ���߷�
int sinGetPVPAccuracy(int MyLevel , int MyAttack_Rating , int DesLevel , int DesDefense)
{
	cnt = 0;
	AC_R = (int)(((float)MyAttack_Rating - (float)DesDefense)*1.4);

	while(1){
		if(AC_R <-190){
			RealAC = 50;
			break;
		}
		if(AC_R >2100){
			RealAC = 95;
			break;
		}
		if(Accuracy_Table[cnt][1] == 0)break; 
		if( AC_R > Accuracy_Table[cnt][0] && AC_R < Accuracy_Table[cnt+1][0] ){
			RealAC = Accuracy_Table[cnt+1][1];	
			break;

		}
		cnt++;
	}
	fDesLV = (float)DesLevel;
	fMyLV  = (float)MyLevel;

	Result = (int)(RealAC - ((fDesLV - fMyLV)/100)*28);

	if(Result < 30) Result = 30;
	if(Result > 95) Result = 95;
	return Result;
}

//���� ũ��Ƽ�� 
int sinGetCritical(int desLV)
{

	fDesLV = (float)desLV;
	fMyLV  = (float)sinChar->Level;

	Result = (int)(sinChar->Critical_Hit + ((fMyLV - fDesLV)/100)*25);
	if(Result > 70)
		Result = 70;
	
	return Result;

}

int sinGetBlockRating(int desLV)
{

	fDesLV = (float)desLV;
	fMyLV  = (float)sinChar->Level;

	Result = (int)(sinChar->Chance_Block+((fMyLV - fDesLV)/100)*25);
	return Result;
	
}

#define SIN_MONSTER_UNDEAD		1
//������� �����Ѵ� 
int sinGetAbsorb(int Type)
{

	switch(Type){
		case SIN_MONSTER_UNDEAD:
			return sinChar->Absorption+sinUndeadAbsorb;
		break;
		default:
			return sinChar->Absorption;

		break;
	}
	return TRUE;
}


void sinReload() //�׷��� ����Ÿ���� �ٽ÷ε��Ѵ� 
{
	cCharStatus.Release(); ///////Release
	cInterFace.Release();
	cInvenTory.Release();
	cItem.Release();
	cMessageBox.Release();
	cShop.Release();
	cSkill.Release();
	cTrade.Release();
	cInvenTory.InvenItemRelease();

	cCharStatus.Load(); ///////Load
	cInterFace.Load();
	cInvenTory.Load();
	cItem.Load();
	cMessageBox.Load();
	cShop.Load();
	cSkill.Load();
	cTrade.Load();
	cInvenTory.InvenItemLoad();

	//������ �׽�Ʈ
	chaSiege.Release();

}

int StopCharMotion(int x ,int y)
{

	/////�ƾ���!! �ϴܶ� �����̴� (������ư�� ������ �������� ���߰� �����ڽ��� �����)
	/* �۽�ũ�� 2������ */
	if(SkillMasterFlag && sinChangeJobButtonShow){    
		//if(x > 240 && x < 240+44 && y > 380 && y < SkillInfoBoxPosi.y+380+44){				
		if(x > 240 && x < 240+44 && y > 380 && y < 380+44){				
			if(CheckNowQuestState(SIN_QUEST_CODE_CHANGEJOB)){
				if(!chaQuest.haQuestCheck())return FALSE; //���� �ʺ� ����Ʈ �� �������̸�
				
				sinChangeJobButtonShow = 0;
				SkillMasterFlag =0; //�޼��� �ڽ��� �����ش� 
				HelpBoxKindIndex = SIN_CHANGEJOB_MESSAGE;
				//��⼭ ����Ʈ �ڽ��� ����ָ�ȴ�  
				cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),QuestFilePath[0]);  
				ChangeJobButonCheck2 = 1;
				return TRUE; 
			}
		}
	}

	char ch=0;
	/* ����̿� 2������ */
	if(SkillMasterFlag && sinChangeJobButtonShow2){    
		//if(x > 240 && x < 240+44 && y > 380 && y < SkillInfoBoxPosi.y+380+44){				
		if(x > 240 && x < 240+44 && y > 380 && y < 380+44){				
			if(CheckNowQuestState(SIN_QUEST_CODE_CHANGEJOB2_NPC_M) || CheckNowQuestState(SIN_QUEST_CODE_CHANGEJOB2_NPC_D)){
				if(!chaQuest.haQuestCheck())return FALSE; //���� �ʺ� ����Ʈ �� �������̸�	
				
				sinChangeJobButtonShow2 = 0;
				SkillMasterFlag =0; //�޼��� �ڽ��� �����ش� 
				//HelpBoxKindIndex = SIN_CHANGEJOB_MESSAGE;
				int len = lstrlen( sinChar->szName );
				for(int cnt=0;cnt<len; cnt++ )
				 	ch += sinChar->szName[cnt];
				if ( (ch&2)==0 ) sinQuest_ChangeJob2.Kind = 1; //���� ����Ʈ
				else sinQuest_ChangeJob2.Kind = 2; //�߸��� ����Ʈ 

				//sinQuest_ChangeJob2.Kind = GetRandomPos(1,2); //��������Ʈ�� A,B�� �����Ѵ�
				//��⼭ ����Ʈ �ڽ��� ����ָ�ȴ�  
				cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),QuestFilePath											[sinQuest_ChangeJob2.Kind+9]);  
				ChangeJobButonCheck2 = 1;
				return TRUE; 
			}
		}
	}

	/* �۽�ũ�� , ����̿� 3������ */
	if(SkillMasterFlag && sinChangeJobButtonShow3){   
		//if(x > 240 && x < 240+44 && y > 380 && y < SkillInfoBoxPosi.y+380+44){				
		if(x > 240 && x < 240+44 && y > 380 && y < 380+44){				
			if(CheckNowQuestState(SIN_QUEST_CODE_CHANGEJOB3)){
				sinChangeJobButtonShow3 = 0;
				SkillMasterFlag =0; //�޼��� �ڽ��� �����ش� 
				cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),QuestFilePath3[0]);  
				ChangeJobButonCheck2 = 1; //��ư�ӽñ⶧���� �̷��� ���ش� ������ ��������!
			}
		}
	}


	/* �۽�ũ�� , ����̿� 4������ */
	if(SkillMasterFlag && sinChangeJobButtonShow4 != 0){ 
		//if(x > 240 && x < 240+44 && y > 380 && y < SkillInfoBoxPosi.y+380+44){				
		if(x > 240 && x < 240+44 && y > 380 && y < 380+44){				
			if(CheckNowQuestState(SIN_QUEST_CODE_CHANGEJOB4)){
				sinChangeJobButtonShow4 = 0;
				SkillMasterFlag =0; //�޼��� �ڽ��� �����ش� 
				cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),QuestFilePath4[0]);  
				ChangeJobButonCheck2 = 1; //��ư�ӽñ⶧���� �̷��� ���ش� ������ ��������!
			}
		}
	}

	for(int i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){   
		if(sSinHelp[i].KindFlag){
			return TRUE; //���� �������� �˾Ƽ� ����� 
	
		}
	}
	//���콺�� �������� �ְų� Ʈ���̵� ���̰ų� â�� ���ְų��ϸ� �������� ����� 
	if(SkillNpcFlag || sinMessageBoxShowFlag || MouseItem.Flag || cWareHouse.OpenFlag || cTrade.OpenFlag || cCraftItem.OpenFlag || cAging.OpenFlag || cMyShop.OpenFlag||
		cCharShop.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag) // ������ - �ͽ��� ���� â �߰�
		return TRUE; //���콺 �������� ������쿡�� �����Ѵ� 

	if(cCharStatus.OpenFlag || cInvenTory.OpenFlag || cSkill.OpenFlag) 
		if(y > 425)return TRUE; //â�� ���� �����Ѵ� 
	if(cShop.OpenFlag) //���� 
		if(x > 0 && x < 384 && y > 40 && y < 370-40) 
			return TRUE;
	if(cWareHouse.OpenFlag) //â�� 
		if(x > 0 && x < 384 && y > 40 && y < 370-40)
			return TRUE;

	if(y > 540 && x>270)    
		return TRUE; //�������̽� ��ư  

	if(ChatKindIndexNum || NoticeChatIndexNum)
		return TRUE; //ä�� ����

	if(RequestTradeButtonFlag)return TRUE;

	if(sSinWarpGate.ShowFlag)return TRUE;

	//ȣ�� �������̽� ���ö� ĳ���� �������� �ʰ�..
	if(HoStopCharMotion())return TRUE;
	if(isDrawClanMenu)return TRUE;

	//������ �������̸� �����ϼ� ����
	if(MyShopSendButton)return TRUE;

	//����� �� ���� �ݱ��϶��� �����ϼ�����
	if(PetMessageOkButtonFlag)return TRUE;
	
	if(ContinueSkillMouseIndex)return TRUE;     //��Ƽ����ų�� ���콺�� ��������
	
	if(haSiegeMenuFlag)return TRUE;             //������ �޴�

	if(haMovieFlag)return TRUE;

	if(chaPremiumitem.UseClickPremiumItemFlag)return TRUE;    //�����̾�  ������ ������ ������ ���� ���� �������� �ʴ´�.

	return FALSE;
}
//�׽�Ʈ�� ���� �޼����� �����ش� 
int CheatHelpOpenFlag = 0;
void sinTestDrawText()
{

	HDC	hdc;
	char strBuff[128];
	renderDevice.lpDDSBack->GetDC(&hdc);

	SelectObject( hdc, sinFont);
    SetBkMode( hdc, TRANSPARENT );
	SetTextColor( hdc, RGB(255,255,0) );

	//sinSTRING sinTestMsg8("������");
	//sinSTRING sinTestMsg9 = sinTestMsg7;


	//�׽�Ʈ
	//dsTextLineOut(hdc,100,300,sinTestMsg9.GetString(),sinTestMsg9.GetLength());



	if(sinTestFlag){ 
	wsprintf(strBuff,"���ݼӵ�:%d",sinChar->Attack_Speed); //���ݼӵ�
	dsTextLineOut(hdc,100,300,strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"����:%d",sinChar->Chance_Block+sinTempBlock); //����
	dsTextLineOut(hdc,200,300,strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"ũ��Ƽ��:%d",sinChar->Critical_Hit); //ũ��Ƽ��
	dsTextLineOut(hdc,300,300,strBuff,lstrlen(strBuff));

	wsprintf(strBuff,"���ñ���:%d",sinChar->Shooting_Range); //�����Ÿ� 
	dsTextLineOut(hdc,400,300,strBuff,lstrlen(strBuff));

	}
	///////////////ġƮŰ ���� 
	if(smConfig.DebugMode){
		if(sinSecretFlag){
			if(CheatHelpOpenFlag){
				wsprintf(strBuff,"(P) ��������ġ���� 10% "); 
				dsTextLineOut(hdc,100,200,strBuff,lstrlen(strBuff));

				wsprintf(strBuff,"(L) �� 200000 "); 
				dsTextLineOut(hdc,100,230,strBuff,lstrlen(strBuff));

				wsprintf(strBuff,"(B) LV 65 �� 300000"); 
				dsTextLineOut(hdc,100,260,strBuff,lstrlen(strBuff));

				wsprintf(strBuff,"(Y) �ͽ��� ������ ���� "); 
				dsTextLineOut(hdc,100,290,strBuff,lstrlen(strBuff));

				wsprintf(strBuff,"(U) ����  "); 
				dsTextLineOut(hdc,100,320,strBuff,lstrlen(strBuff));

				wsprintf(strBuff,"(F) ���õ� Up  "); 
				dsTextLineOut(hdc,100,350,strBuff,lstrlen(strBuff));

				wsprintf(strBuff,"(N) ���õ� Down  "); 
				dsTextLineOut(hdc,100,380,strBuff,lstrlen(strBuff));
			}
		}
	}

	renderDevice.lpDDSBack->ReleaseDC(hdc);

}
/*
//Tempscron 4�� ������ ��...
static char *szTempFighterFaceName[] = {
	"char\\tmABCD\\tmh-b01.inf",
	"char\\tmABCD\\tmh-b02.inf",
	"char\\tmABCD\\tmh-b03.inf"
};

static char *szTempMechFaceName[] = {
	"char\\tmABCD\\tmh-a01.inf",
	"char\\tmABCD\\tmh-a02.inf",
	"char\\tmABCD\\tmh-a03.inf"
};

static char *szTempArcherFaceName[] = {
	"char\\tmABCD\\tfh-D01.inf",
	"char\\tmABCD\\tfh-d02.inf",
	"char\\tmABCD\\tfh-d03.inf"
};
static char *szTempPikeFaceName[] = {
	"char\\tmABCD\\tmh-c01.inf",
	"char\\tmABCD\\tmh-c02.inf",
	"char\\tmABCD\\tmh-c03.inf"
};
*/

/////////////////////////////////TEST KEY
void sinTestKey()
{
	//sCRAFTITEM_SERVER WingItem ;
	//smCHAR_INFO Char_Info;
	int TempExp  = 0;
	int TempExp2 = 0;
	//TRANS_SOD_RESULT Test;
	int CountTemp=0; 
//	sQUEST_CHANGEJOB3 TempJob3;
	//�׽�Ʈ�� ���� 
    
	if(smConfig.DebugMode){
		if(sinGetKeyClick('Y')){		//����¡ �׽�Ʈ

			//cAging.OpenFlag = 1;
			//haElementaryQuestLog | = 
		
			//cSinHelp.SendResearch();
			//cHelp_Menu.haGuidHelpLevel_1(sinDA1|sin01);
			//�׽�Ʈ Ű!!
			//CancelContinueSkill(CLANSKILL_ABSORB);
			//chaSiege.ShowPickUserKillPoint(900,50,10);
			//TestSiegeTemp[0]++;
			//chaSiege.SetClanSkill(1,CLANSKILL_HP);
			
			//sinShowTeleportMap();
			//sinShowSeigeMessageBox();
			//cInvenTory.CheckCristalEventItem();
		    //sinShowTeleportMap();
			//cInvenTory.SendCristalEventItem();
			//sinSetQuestTimer(&TempJob3);
			//sinCheckLevelQuest90_2(1);
			//sinShowTeleportMap();
			//sinCheckLevelQuest80_2(1); //���� 1 �̴���2
			//sinSkill.pRightSkill
			//ReStartFlag = 1;
			//cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),QuestFilePath[9]);  
			//cCraftItem.OpenFlag = 1;
			//cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST,QuestMessageBoxPosi2.x,
			//cInvenTory.ReSetQuestItemPosi();
			//sinCheckLevelQuest85();
			//sinCheckLevelQuest85();
			//sinCheckLevelQuest90();
			//cInterFace.CheckAllBox(SIN_CHARSHOP);
			//sinCheckLevelQuest30();
			//CheckChangeJob3QuestItem(1);
			//sinCheckLevelQuest80();
			//cInvenTory.CheckChristMasEventItem();
			//cInvenTory.SetForceOrb((sinFO1|sin07) , 200);
			//cTrade.OpenFlag = 1;
			//cSinSod2.RecvClanRank("");
			/*
			CountTemp = 0;
			while(CountTemp < 10000){
				sinWingItemQuestServer(&WingItem,&Char_Info);
				CountTemp++;
			}
			*/
			//cInvenTory.CheckEventItem();
			//cSinSod2.ShowSod2MessageBox();
			//cInvenTory.IncreViewDamage(0);
			//cHelpPet.CheckPetData();
			//cHelpPet.GetFileData();
			//cSkill.CancelContinueSkill(SKILL_VANISH);
			//cSkill.SetVirtualLife(100,5);
			//cInterFace.CheckAllBox(SIN_CRAFTITEM); 
			//cCraftItem.ForceFlag = 1; 
			//cInterFace.CheckAllBox(SIN_CHARSHOP); 
			//cAging.OpenFlag = 1;
			//cInvenTory.SendPuzzleNpc();
			//sinWingItemQuestServer( &sWingItem_Send, &lpCurPlayer->smCharInfo );
			//cMyShop.OpenFlag ^= 1;
			//sinChar->ChangeJob++;
			//SaveGameData(); //������ �����Ѵ�
			//ChangeJobFace();

			//cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,"�޷�");
			//SaveGameData(); //������ �����Ѵ�
			//cSkill.SetT_Of_Valhalla(1);
			//CheckChangeJob_QuestItem();
			//sLost_Item.Flag ^=1;
			//SpecialItemShowFlag ^=1;
			//cInvenTory.ChangeSpecialItem();
			//CheckChangeJob3QuestItem(1);
			//cMessageBox.ShowMessage2(MESSAGE_GIVE_MONEY);
			//sinNpcChangeJob3();
			//cAging.OpenFlag ^=1;

			//StarShopNPC();
			//sStarShop.Flag ^=1;

			//sinPlusMoney(1000000000);
			//ReformCharForm(); //������ 
			//SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

			//cAging.OpenFlag ^=1;
			//sStarShop.Flag ^=1;
			//���� ����
			//sinChar->ChangeJob++; //������ �Ѵ� 
			//cSkill.SearchUseSkill();

			//sinQuest_ChangeJob.BackUpCode = SIN_QUEST_CODE_CHANGEJOB;
			//EndQuest_Code(sinQuest_ChangeJob.BackUpCode);

			//cMessageBox.ShowMessage3(  MESSAGE_EVENT_GYUNGFUM , "" );
			//cInvenTory.sinWingQuestNpc();
			//cInvenTory.sinWingQuestNpc();
			//cSinHelp.ShowSodRanking(&Test,1); 
			//cMessageBox.ShowMessage3(MESSAGE_POSTBOX,"�Źٶ� ���̵�");
			//cInvenTory.CheckInvenEmpty(sinDA1|sin15);
			//sinPosBoxNpc();
			//MouseItem.sItemInfo.PotionCount++;

			/*
			for(int j=1;j < SIN_MAX_USE_SKILL; j++){
				if(sinSkill.UseSkill[j].Flag){
					sinSkill.UseSkill[j].Point++;
				}
			}
			*/

			//cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,sinJobList->szName2);
			//cInvenTory.CheckDamage();
			//cInvenTory.ClearQuestItem();
			//cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),QuestFilePath[1]);  
			//sinShowEventSmashingTV();
			//cSinHelp.sinShowHelp(SIN_MESSAGE_DEFAULT,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),QuestFilePath[3]);  
			//sinMorayionNpcChangeJob_Two();
			///sinQuest_ChangeJob2.State = 3;
			//sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1); 
			//cCharStatus.InitCharStatus();
			//cCharStatus.InitCharStatus(); //ĳ���� ���� �ʱ�ȭ ��� ��� 
			//cAging.OpenFlag ^=1;
			//cCraftItem.OpenFlag ^=1;

//			if(cTrade.OpenFlag)cTrade.OpenFlag = 0;
//			else cTrade.OpenFlag = 1;

			//sinChar->ChangeJob++; //������ �Ѵ� 
			//cSkill.SearchUseSkill();
			//cMessageBox.ShowMessage3(MESSAGE_EVENT_SMASHTV,""); 
			//sinShowEventSmashingTV();
			//cMessageBox.ShowMessageEvent("������� �ܽ��Ϳ���~~~~");
			//cSkill.InitSkillPoint();
			//cAging.OpenFlag = 1;
			//cMessageBox.ShowMessage3(MESSAGE_INIT_SKILL_POINT,sinSkillPointName);
			//cInterFace.CheckAllBox(SIN_CRAFTITEM);
			//���� �־��ش� 
 			//lstrcpy(sinChar->szModelName2,"char\\tmABCD\\tmh-b01.inf");
 			//ReformCharForm(); //������ 

		}
		if(sinGetKeyClick('I')){    //����¡ �׽�Ʈ    
			sinCheckAgingLevel(SIN_AGING_ATTACK, false);
		//	sinCheckAgingLevel(SIN_AGING_DEFENSE, false);
			sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMOR, false);
			sinCheckAgingLevel(SIN_AGING_DEFENSE_ORB, false);
			sinCheckAgingLevel(SIN_AGING_DEFENSE_BOOTS, false);   // ����� - ����¡ ������ �߰�(����)
			sinCheckAgingLevel(SIN_AGING_DEFENSE_GLOVES, false);  // ����� - ����¡ ������ �߰�(�尩)
			sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMLETS, false); // ����� - ����¡ ������ �߰�(�ϸ�)
			sinCheckAgingLevel(SIN_AGING_CRITICAL, false);
			sinCheckAgingLevel(SIN_AGING_BLOCK, false);
			ReformCharForm(); //������ 
			if(SelectInvenItemIndex){  
				//OverDay_Item_Delete(&cInvenTory.InvenItem[SelectInvenItemIndex-1]);
				//cInvenTory.DeleteInvenItemToServer(cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.CODE,
				//	cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemHeader.Head,
				//	cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemHeader.dwChkSum);
				
				//sinDownDefenseAging(&cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo,10);
				
				//cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo. = 0;

				/*
				if(cAging.OpenFlag)
					cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemKindCode = ITEM_KIND_MAKE_ELEMENT;
				else
					cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemKindCode = ITEM_KIND_MAKE_MAIN;
				*/
					
				//ReformItem( &cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo);  //������ ���� �ޱ� 
				/*
				cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.JobItem.Lev_Damage[0] = 1;
				cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.JobItem.Lev_Damage[1] = 1;
				cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.JobItem.Lev_Attack_Rating = 1;
				ReformItem( &cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo );
				ReformCharForm();
				cInvenTory.ReFormInvenItem();
				*/
				/*
				sCraftItem.CraftItem[0].sItemInfo.Damage[0] = 500;
				sCraftItem.CraftItem[0].sItemInfo.Damage[1] = 999;
				ReformItem(&sCraftItem.CraftItem[0].sItemInfo);  //������ ���� �ޱ� 
				ReformMixItem(&sCraftItem.CraftItem[0].sItemInfo ); //����¡ ������ 
				cCraftItem.ReFormCraftItem();   //�κ��丮 ������ ���� 
				*/

				//cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.PotionCount = 0;
				//cInvenTory.ReFormPotionNum();
				//cInvenTory.InvenItem[SelectInvenItemIndex-1].LimitTimeFlag = 1;
				//cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.Defence = 136;
				//cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.fAbsorb = 3.3f;
				//cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.JobItem.Add_Defence = 21;
				//cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.JobItem.Add_fAbsorb = 0.6f;
	
				/*	
				cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemKindCode =ITEM_KIND_AGING;
				cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemAgingNum[1] = 1;
				ReformMixItem( &cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo  ); //������ (�������� �κ��丮�� �����ϱ����� ...)
				cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemAgingNum[0]++;
				*/

				//cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemAgingCount[0]++;
//				cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemAgingNum[1]++;
//				cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.ItemKindCode = ITEM_KIND_QUEST;

				/*			
				cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.Attack_Rating =100;
				//cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo.Damage[1] =30;

				ReformItem( &cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo);  //������ ���� �ޱ� 
				cInvenTory.ReFormInvenItem();
				*/
			//	ReformItem(&cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo);  //������ ���� �ޱ� 
			//	ReformMixItem( &cInvenTory.InvenItem[SelectInvenItemIndex-1].sItemInfo  ); //����¡ ������ 
			//	cInvenTory.ReFormInvenItem();   //�κ��丮 ������ ���� 

			}

		}

		if(!sinSecretFlag)return;

		/*
		if(sinGetKeyClick('B')){ 
//	#ifdef	_WINMODE_DEBUG
			CheckCharForm(); //����  
			TempExp = (sinChar->Next_Exp)- (sinChar->Exp^CodeXorExp); 
			TempExp2 = (int)((float)TempExp/10); 

			sinChar->Exp = ((sinChar->Exp^CodeXorExp)+(TempExp2))^CodeXorExp;
			//((sinChar->Exp^CodeXorExp)*2)^CodeXorExp;
			ReformCharForm(); //������ 
//	#endif
		}
		*/
		/*
		if(sinGetKeyClick('N')){
			for(int j=1;j < SIN_MAX_USE_SKILL; j++){
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point){
					sinSkill.UseSkill[j].UseSkillCount -=200;
					if(sinSkill.UseSkill[j].UseSkillCount < 0 )sinSkill.UseSkill[j].UseSkillCount = 0;

				}

			}

		}
		*/

		if(sinGetKeyClick('U')){   
			for(int j=1;j < SIN_MAX_USE_SKILL; j++){
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point){
					sinSkill.UseSkill[j].UseSkillCount +=100;
					cSkill.ReformSkillMasteryForm(0,j); //����Ÿ�� �����Ѵ�

				}

			}
			cSkill.ReFormCharSkillInfo(); //��ų������ �����Ѵ� (��ŷ����)
			ReformCharForm();//������ 

		}
		if(sinGetKeyClick('L')){ 
			//sinChar->Exp +=10000000;

			//CheckCharForm(); //���� 
			//sinChar->Money = 600000;
			//ReformCharForm(); //������ 
			//SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

		}

		if(sinGetKeyClick('H')){   
//			sinChar->ChangeJob++;
//			ChangeJobFace(); //����� �ٲ۴� (��ġ�ϰ�..)
//			cSkill.SearchUseSkill(); //���� ��ų�� ã�´� 

		}
		if(sinGetKeyClick('H')){
			//if(CheatHelpOpenFlag)CheatHelpOpenFlag =0;
			//else CheatHelpOpenFlag = 1;

		}
		/*
		if(sinGetKeyClick('K')){ 
			cSkill.InitSkillPoint();
		}
		if(sinGetKeyClick('K')){ 
			cSkill.InitSkillPoint();
		}
		*/
	}
}


//Ʈ���̵� �䱸�� ������ ���� �����Ѵ� 
int sinRequestTrade(DWORD CODE , char *CharName)
{
	if(cTrade.OpenFlag)
		return FALSE;

	cTrade.TradeRequestFlag = 1;
	cTrade.TradeCharCode = CODE;
	lstrcpy(cTrade.szTradeCharName,CharName);
	return TRUE;
	
}

DWORD dwCurrentTime = 0;

//��ų�� ���� ������ �Ѵ� 
int sinSkillMaster(int ChangeJob)
{

	int i=0,j=0; 
	//����Ʈ ���û����ϰ�� ���Ͻ�Ų��
	for(i = 0 ; i < SIN_MAX_HELP_NUMBER; i++){        
		if(sSinHelp[i].KindFlag)return FALSE;
	}

	int SkillTreeCheckFlag = 0;  
	int ChangeJobItemCnt = 0;


	if(ChangeJob){ //�������� �÷��� ���;� ó���Ѵ� 
		for(i=0;i < 3 ; i++){
			ItemIndex1[i] = 0;
			ItemIndex2[i] = 0;
		}
		if(sinQuest_ChangeJob.CODE == SIN_QUEST_CODE_CHANGEJOB){ 
			SkillNpcFlag = 0;
			if(sinQuest_ChangeJob.State == SIN_QUEST_PROGRESS){ //����Ʈ �������ϰ��..
				if(sinChar->ChangeJob == 0){ //2�� �������� �ش�� 
					for(i=0;i < 100 ; i++){
						if(cInvenTory.InvenItem[i].Flag){
							if(cInvenTory.InvenItem[i].CODE ==  (sinQT1|sin01))ItemIndex1[0] = i+1;
							if(cInvenTory.InvenItem[i].CODE ==  (sinQT1|sin02))ItemIndex1[1] = i+1;
							if(cInvenTory.InvenItem[i].CODE ==  (sinQT1|sin03))ItemIndex1[2] = i+1;

						}
						if(cInvenTory.InvenItemTemp[i].Flag){
							if(cInvenTory.InvenItemTemp[i].CODE ==  (sinQT1|sin01))ItemIndex1[0] = i+1;
							if(cInvenTory.InvenItemTemp[i].CODE ==  (sinQT1|sin02))ItemIndex1[1] = i+1;
							if(cInvenTory.InvenItemTemp[i].CODE ==  (sinQT1|sin03))ItemIndex1[2] = i+1;
						}
					}
				}
			}
		}
		//2�� ���� �ڵ� (�۽�ũ��)
		if(ItemIndex1[0] && ItemIndex1[1] && ItemIndex1[2]){ //�ճ�ġ�� ����   
			SkillNpcFlag = 0;
			cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),QuestFilePath[1]);  
			cSkill.OpenFlag = 1;
			cInterFace.CheckAllBox(SIN_SKILL); //â�� �ϳ��� ���� 


			//sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob+1);
			//cMessageBox.ShowMessage3(MESSAGE_CHANGE_JOB,sinJobList->szName2);
		}
		//2�� ���� �ڵ� (����̿�)
		else if( (FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB2_NPC_M) ||FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB2_NPC_D)) && sinChar->ChangeJob < 1){
			cSinHelp.sinShowHelp(SIN_HELP_KIND_QUEST,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),QuestFilePath[9]);  
			cInterFace.CheckAllBox(SIN_SKILL); //â�� �ϳ��� ���� 
			sinQuest_ChangeJob2.State = 0;
			SkillNpcFlag = 0;
		}
		else if( sinQuest_ChangeJob4.CODE){
			SkillNpcFlag = 0;
			sinCheckChangeJobQuest4();
		}
		else{
			//���⼭ 3�� ��ų SkillNpcFlag = 0;
			if(!sinNpcChangeJob3())
			{
				SkillMasterFlag = 1; //��ų
			}
		}
	}
	else{
			SkillMasterFlag = 1; //��ų
	}
	cSkill.OpenFlag = 1;
	cInterFace.CheckAllBox(SIN_SKILL); //â�� �ϳ��� ���� 

	//////////////// ���� 3������ 
	/*
	if(sinChar->ChangeJob == 1 && FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB3)
	*/

	if(sinQuest_ChangeJob.State)//���⼭�� �ʱ�ȭ���ش� 
		sinChangeJobButtonShow = 0;
	if(sinQuest_ChangeJob2.State){//���⼭�� �ʱ�ȭ���ش�
		sinChangeJobButtonShow2 = 0;
	}
	if(sinQuest_ChangeJob3.State){//���⼭�� �ʱ�ȭ���ش�
		sinChangeJobButtonShow3 = 0;
	}
	if(sinQuest_ChangeJob4.State){//���⼭�� �ʱ�ȭ���ش�
		sinChangeJobButtonShow4 = 0;
	}


	//����Ʈ�� ��ġ�� �ʱ�ȭ���ش�
	if((FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB2_NPC_M) ||FindLastQuestCode(SIN_QUEST_CODE_CHANGEJOB2_NPC_D)) && sinChar->ChangeJob < 1){
		sinChangeJobButtonShow2 = 0;
	}

	//����Ʈ�� ��������� ������ ����Ƿ� �̰������� ó�����ش�
	switch(sinChar->ChangeJob){
	case 0:
		ChangeJobSkillPlus = 5;
	break;
	case 1:
		ChangeJobSkillPlus = 9;
	break;
	case 2:
		ChangeJobSkillPlus = 13;
	break;
	case 3:
		ChangeJobSkillPlus = 17;
	break;
	}


	

	SkillTreeCheckFlag = 1;
	for(int i=1; i  < ChangeJobSkillPlus ; i++){	
		if(!sinSkill.UseSkill[i].Point)
			SkillTreeCheckFlag = 0; 
	}
	CheckQuestButtonShow1 = 0;
	CheckQuestButtonShow2 = 0;
	CheckQuestButtonShow3 = 0;
	switch(ChangeJob){
		case 1:
			if(sinChar->ChangeJob == 0)
				if(SkillTreeCheckFlag)CheckQuestButtonShow1 = 1;
		break;
		case 2:
			if(sinChar->ChangeJob == 0)
				if(SkillTreeCheckFlag)CheckQuestButtonShow1 = 1;
			if(sinChar->ChangeJob == 1)
				if(SkillTreeCheckFlag)CheckQuestButtonShow2 = 1; 
//			if(smConfig.DebugMode)     
			if(sinChar->ChangeJob == 2)
					if(SkillTreeCheckFlag)CheckQuestButtonShow3 = 1;
		break;
		case 3:
			if(sinChar->ChangeJob == 0)
				if(SkillTreeCheckFlag)CheckQuestButtonShow1 = 1;
			if(sinChar->ChangeJob == 1)
				if(SkillTreeCheckFlag)CheckQuestButtonShow2 = 1;
			if(sinChar->ChangeJob == 2)
				if(SkillTreeCheckFlag)CheckQuestButtonShow3 = 1;
		break;

	}

	return TRUE;
}

//��ų ���⸦ ��ģ�� 
int sinSkillMasterClose()
{
	/*
	if(ChangeJobButtonclick){
		cInterFace.CheckAllBox(SIN_SKILL); //��ųâ�� ����ش� 

	}
	else{
	*/
		SkillMasterFlag = 0;	//��ų�� �Ҵ��Ҽ��ִ� ���� �ʱ�ȭ�Ѵ� 
		cSkill.OpenFlag = 0;	//��ų â�� �ݾ��ش� 
dwCurrentTime = dwPlayTime+5000;
	//}
	return TRUE;
}


static int LastPartyMode = -1;
int haSiegeBoardTempFlag = 0; //������ ����â�� ����� ����  
//��Ƽ â�� �����ش� 
int ShowParty()
{
	int ls;
	
	

	if ( InterfaceParty.PartyPosState==PARTY_START || InterfaceParty.PartyPosState==PARTY_END ) 
		return FALSE;

	InterfaceParty.qN_Pressf_CLEAR();

	ls = LastPartyMode;

	LastPartyMode = 1;

	if ( InterfaceParty.PartyPosState==PARTY_PROCESS && LastPartyMode!=ls ) {
		InterfaceParty.Main_menuSet(1);
		return TRUE;
	}

	InterfaceParty.InitMenuSet(1);

	switch(InterfaceParty.PartyPosState){
	case PARTY_NONE:
		InterfaceParty.PartyPosState =PARTY_START;
		//����Ʈ â�� ������ ���带 �ݾ��ش�.
		haSiegeBoardTempFlag = 1;	
		break;
	case PARTY_PROCESS:
		InterfaceParty.PartyPosState =PARTY_END;
		haSiegeBoardTempFlag = 0;
		break;
	/*case PARTY_END:
		InterfaceParty.PartyPosState =PARTY_START;
		break;*/

	}
	
	return TRUE;
}

//����Ʈ â�� �����ش� 
int ShowQuest()
{
	int ls;

	
	InterfaceParty.qN_Pressf_CLEAR();

	if ( InterfaceParty.PartyPosState==PARTY_START || InterfaceParty.PartyPosState==PARTY_END ) 
		return FALSE;

	ls = LastPartyMode;

	LastPartyMode = 0;

	if ( InterfaceParty.PartyPosState==PARTY_PROCESS && LastPartyMode!=ls ) {
		InterfaceParty.Main_menuSet(0);
		return TRUE;
	}

	InterfaceParty.InitMenuSet(0);
	
	switch(InterfaceParty.PartyPosState){
	case PARTY_NONE:
		InterfaceParty.PartyPosState =PARTY_START;
		//����Ʈ â�� ������ ���带 �ݾ��ش�.
		haSiegeBoardTempFlag = 1;	
		break;
	case PARTY_PROCESS:
		InterfaceParty.PartyPosState =PARTY_END;
		haSiegeBoardTempFlag = 0;		
		break;
	/*case PARTY_END:
		InterfaceParty.PartyPosState =PARTY_START;
		break;*/

	}
	
	return TRUE;
}

///////////////////////////////// ������ , ���� , ���׹̳� ���� �ڵ�ȭ 

//�������� �о�´� 
short sinGetLife()
{

		return (sinChar->Life[0])^CodeXorLife;
}

//������ ���´� 
short sinGetMana()
{

	return sinChar->Mana[0];
}

//���׹̳��� ���´� 
short sinGetStamina()
{

	return sinChar->Stamina[0];
}

//�������� �����Ѵ�				//kyle xtrapHeap
void sinSetLife(short Life)
{
#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func11_Protect( &(sinChar->Life), sizeof(sinChar->Life) );	//��ȣ���� ���Ἲ üũ
#endif

	//CheckCharForm(); //���� 
	sinChar->Life[0] = Life;
	if(sinChar->Life[0] < 0 ) sinChar->Life[0] = 0;
	if(sinChar->Life[0] >= sinChar->Life[1] )sinChar->Life[0] = sinChar->Life[1];
	sinChar->Life[0] ^= CodeXorLife;
	ReformCharForm(); //������ 

#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func12_Protect( &(sinChar->Life), sizeof(sinChar->Life) );	//��ȣ���� ��ȣ
	XTrap_CE1_Func13_Free( &(sinChar->Life), sizeof(sinChar->Life) );		//��ȣ���� ����
#endif
	
}

//������ �����Ѵ� 
void sinSetMana(short Mana)
{
#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func11_Protect( &(sinChar->Mana), sizeof(sinChar->Mana) );	//��ȣ���� ���Ἲ üũ
#endif

	//CheckCharForm(); //���� 
	sinChar->Mana[0] = Mana;
	if(sinChar->Mana[0] < 0 ) sinChar->Mana[0] = 0;
	if(sinChar->Mana[0] >= sinChar->Mana[1] )sinChar->Mana[0] = sinChar->Mana[1];
	ReformCharForm(); //������ 

#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func12_Protect( &(sinChar->Mana), sizeof(sinChar->Mana) );	//��ȣ���� ��ȣ
	XTrap_CE1_Func13_Free( &(sinChar->Mana), sizeof(sinChar->Mana) );		//��ȣ���� ����
#endif
}

//���׹̳��� �����Ѵ� 
void sinSetStamina(short Stamina)
{
#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func11_Protect( &(sinChar->Stamina), sizeof(sinChar->Stamina) );	//��ȣ���� ���Ἲ üũ
#endif

	//CheckCharForm(); //���� 
	sinChar->Stamina[0] = Stamina;
	if(sinChar->Stamina[0] < 0 ) sinChar->Stamina[0] = 0;
	if(sinChar->Stamina[0] >= sinChar->Stamina[1] )sinChar->Stamina[0] = sinChar->Stamina[1];
	ReformCharForm(); //������ 

#ifdef _XTRAP_GUARD_4_CLIENT //HEAP MEMORY TEST
	XTrap_CE1_Func12_Protect( &(sinChar->Stamina), sizeof(sinChar->Stamina) );	//��ȣ���� ��ȣ
	XTrap_CE1_Func13_Free( &(sinChar->Stamina), sizeof(sinChar->Stamina) );		//��ȣ���� ����
#endif

}

//	short   ItemAgingNum[2];	//0�� ����¡+�� 1�� ����¡�� �ǰ��ֳ� �ƴѰ�?
//	short   ItemAgingCount[2];	//0������ ���� ī��Ʈ 1����ġ �ִ� 

//////����¡ ������ üũ���� �÷��ش�(����¡��ġ)
/*
char *AgingMsgDamageMax = "�ִ���ݷ�";
char *AgingMsgDamageMin = "�ּҰ��ݷ�";
char *AgingMsgAttack_Rate = "���߷�";
char *AgingMsgDur = "������";
char *AgingMsgCritical = "ũ��Ƽ��";
char *AgingMsgShooting_Range = "�����Ÿ�";
char *AgingMsgMana = "���";
char *AgingMsgBlock = "����";
char *AgingMsgAbsorb = "�����";
char *AgingMsgDefense = "�����";
*/
//������
int sinSetDurabilityAging(sITEMINFO *pItem)
{
	int Temp = 0;
	Temp = pItem->Durability[1]/100;
	Temp += 1;
	pItem->Durability[1] -= Temp;
	if(pItem->Durability[1] < 0 ) pItem->Durability[1] = 0;
	if(pItem->Durability[1] < pItem->Durability[0] ) pItem->Durability[0] = pItem->Durability[1]; //���� ������ ���� 

	return TRUE;
}
//���ݷ�
int sinSetDamageAging(sITEMINFO *pItem)
{
	pItem->Damage[0]++;
	pItem->Damage[1]++;

	if(pItem->ItemAgingNum[0] >= 9){ //10�����ʹ� ������ +2
		pItem->Damage[0]++;
		pItem->Damage[1]++;
	}

	// ����� - ����¡ ���� ����(����� ������ ���ݷ� �߰�)
	if(pItem->ItemAgingNum[0] >= 19){ //20�����ʹ� ������ +3
		pItem->Damage[0]++;
		pItem->Damage[1]++;
	}

	return TRUE;
}
//����
int sinSetDefenseAging(sITEMINFO *pItem , int Percent)
{
	float Temp = 0;
	Temp = (float)pItem->Defence/(100.0f / (float)Percent);
	pItem->Defence += (int)Temp;
	return TRUE;
}

//���߷�
int sinSetAttackRateAging(sITEMINFO *pItem , int AddNum)
{
	pItem->Attack_Rating += AddNum;
	return TRUE;
}
//�����
int sinSetAbsorbAging(sITEMINFO *pItem , float Percent)
{
	pItem->fAbsorb += Percent;

	if(pItem->ItemAgingNum[0] >= 9){ //10�����ʹ� 2��
		pItem->fAbsorb += Percent;
	}

	// ����� - ����¡ ���� ����(����� ������ ���ݷ� �߰�)
	if(pItem->ItemAgingNum[0] >= 19){ //20�����ʹ� 3��
		pItem->fAbsorb += Percent;
	}
	return TRUE;
}

//ũ��Ƽ��
int sinSetCriticalAging(sITEMINFO *pItem)
{
	if(pItem->ItemAgingNum[0] && (pItem->ItemAgingNum[0]%2) == 1){
		pItem->Critical_Hit += 1;

	}
	return TRUE;
}

//����
int sinSetBlockRateAging(sITEMINFO *pItem)
{
	if(pItem->ItemAgingNum[0] && (pItem->ItemAgingNum[0]%2) == 1){
		pItem->fBlock_Rating += 1;

	}
	return TRUE;
}

int sinSetAgingItemIncreState(sITEMINFO *pItem)
{
	float TempDefense[2] = {0,0};
	//����¡ �ʱ�ȭ
	switch(pItem->CODE & sinITEM_MASK2){
		case sinWA1: //���� 
			//���ݷ� 
			sinSetDamageAging(pItem);
			//���߷�
			sinSetAttackRateAging(pItem , 10);
			//������
			sinSetDurabilityAging(pItem); 

		break;
		case sinWC1: //Ŭ��
			//���ݷ� 
			sinSetDamageAging(pItem);
			//���߷�
			sinSetAttackRateAging(pItem , 5);
			//ũ��Ƽ��  
			sinSetCriticalAging(pItem);

		break;

		case sinWH1: //�ظ� 
			//���ݷ� 
			sinSetDamageAging(pItem);

			//���߷�
			sinSetAttackRateAging(pItem , 10);
			//������
			sinSetDurabilityAging(pItem); 

		break;
		case sinWM1: //������ ������ 
			//���ݷ� 
			sinSetDamageAging(pItem);

			//���߷�
			sinSetAttackRateAging(pItem , 10);

			//���  
			pItem->fIncrease_Mana += 10;

		break;
		case sinWP1: //â 
			//���ݷ� 
			sinSetDamageAging(pItem);

			//���߷�
			sinSetAttackRateAging(pItem , 5);

			//ũ��Ƽ��  
			sinSetCriticalAging(pItem);

		break;
		case sinWS1: //Ȱ 
			//���ݷ� 
			sinSetDamageAging(pItem);

			//ũ��Ƽ��  
			sinSetCriticalAging(pItem);

			//������
			sinSetDurabilityAging(pItem); 
		break;

		case sinWS2: //Į  
			//���ݷ� 
			sinSetDamageAging(pItem);

			//���߷�
			sinSetAttackRateAging(pItem , 5);
			
			//ũ��Ƽ��  
			sinSetCriticalAging(pItem);

		break;

		case sinWT1: //�ں��� 
			//���ݷ� 
			sinSetDamageAging(pItem);

			//ũ��Ƽ��  
			sinSetCriticalAging(pItem);

			//������
			//sinSetDurabilityAging(pItem); 
		break;
			
		case sinDS1: //���� 
			//���� 
			sinSetBlockRateAging(pItem);
			//�����
			sinSetAbsorbAging(pItem , 0.2f);
			//������
			sinSetDurabilityAging(pItem); 
		break;

		case sinOM1: //������ ���� 
			//���� 
			sinSetDefenseAging(pItem,10);

			//�����
			sinSetAbsorbAging(pItem , 0.5f);

			//������
			sinSetDurabilityAging(pItem); 

		break;
		case sinDA1: case sinDA2://���� 
			//���� 
			sinSetDefenseAging(pItem,5);

			//�����
			sinSetAbsorbAging(pItem , 0.5f);

			//������
			sinSetDurabilityAging(pItem); 

		break; 
		case sinDB1: // ����� - ����¡ ������ �߰�(����)
			//���� 
			sinSetDefenseAging(pItem,10);
			//�����
			sinSetAbsorbAging(pItem , 0.5f);
			//������
			sinSetDurabilityAging(pItem); 
			break;
		case sinDG1: // ����� - ����¡ ������ �߰�(�尩)
			//���� 
			sinSetDefenseAging(pItem,10);
			//�����
			sinSetAbsorbAging(pItem , 0.5f);
			//������
			sinSetDurabilityAging(pItem); 
			break;
		case sinOA2: // ����� - ����¡ ������ �߰�(�ϸ�)
			//���� 
			sinSetDefenseAging(pItem,10);
			//���߷�
			sinSetAttackRateAging(pItem , 5);
			//������
			sinSetDurabilityAging(pItem); 
			break;
		}
		
		

		pItem->ItemAgingNum[0]++;	//�������� ������ �÷��ش� 
			
		pItem->ItemAgingNum[1] = 0; //�ʱ�ȭ 
		pItem->ItemAgingCount[0] = 0;
		pItem->ItemAgingCount[1] = 0;

		
		if( pItem->Level == 100 || pItem->Level == 102 || pItem->Level == 105 ||	// pluto 102���� ���� ���� ���� �ҷ��� // �庰 - 105�� ������ ����
			cAging.AginStoneKind == 3 || cAging.AginStoneKind == 4 || // ����� - ���� ���� ����, ���� ����¡ ���� ���ÿ��� ����¡ ������ ����ص� ������ ������ �ö��� �ʴ´�.
			cAging.AginStoneKind == 5  ) // �庰 - ���� ����¡ ���� 1.5
		{
			pItem->Level += 0;
		}
		else
		{
			if((pItem->ItemAgingNum[0] % 2 ) == 0)
			{
				pItem->Level++;
			}
		}
				
	ReformItem(pItem);  //������ ���� �ޱ� 
	return TRUE;
}

//�������� ������
int sinDownDurabilityAging(sITEMINFO *pItem)
{
	int Temp = 0;
	Temp = pItem->Durability[1]/100;
	Temp += 1;
	pItem->Durability[1] += Temp*DownNum;
	if(pItem->Durability[1] < 0 ) pItem->Durability[1] = 0;
	if(pItem->Durability[1] < pItem->Durability[0] ) pItem->Durability[0] = pItem->Durability[1]; //���� ������ ���� 

	return TRUE;
}	
																																																																																									
// ����¡ ���ݷ�
int sinDownDamageAging(sITEMINFO *pItem , int Level)
{
	/*
		DownNum =  Down + 1;		
	*/
	if( Level < 10 ){
		pItem->Damage[0] -= DownNum; 
		pItem->Damage[1] -= DownNum;
	}
	else{
		switch( DownNum ){
			case 1:
				pItem->Damage[0] -= 2; 
				pItem->Damage[1] -= 2;
			break;
			case 2:
				if( Level == 10 ){
					pItem->Damage[0] -= 2+1; 
					pItem->Damage[1] -= 2+1;				
				}
				else{
					pItem->Damage[0] -= 2*2; 
					pItem->Damage[1] -= 2*2;
				}
			break;
			case 3:
				if( Level == 10 ){
					pItem->Damage[0] -= 2+1+1; 
					pItem->Damage[1] -= 2+1+1;
				}
				else if( Level == 11 ){
					pItem->Damage[0] -= 2*2+1; 
					pItem->Damage[1] -= 2*2+1;
				}
				else{
					pItem->Damage[0] -= 2*3; 
					pItem->Damage[1] -= 2*3;
				}
			break;
		}		
	}
	

	return TRUE;
}
//����
int sinDownDefenseAging(sITEMINFO *pItem , int Percent)
{

	float Temp = 0;
	int   TempDefence =0;
	for(int i = 0 ; i< DownNum ;i++){
		Temp = (100.0f*(float)pItem->Defence)/(100.0f+(float)Percent);
		TempDefence = pItem->Defence - (int)Temp;
		pItem->Defence -= TempDefence;
	}
	return TRUE;
}

//���߷�
int sinDownAttackRateAging(sITEMINFO *pItem , int AddNum)
{

	pItem->Attack_Rating -= AddNum*DownNum;
	return TRUE;

}
//�����
int sinDownAbsorbAging(sITEMINFO *pItem , float Percent , int Level)
{

	pItem->fAbsorb -= Percent*(float)DownNum; 

	int Temp = Level-9; //���� 10�̻��� ������ ����ŭ�� �����ش�
	if(Temp >0 ){
		if(Temp == 1){
			pItem->fAbsorb -= Percent;
		}
		else{
			pItem->fAbsorb -= Percent*(float)DownNum;

		}

	}
	return TRUE;

}

//ũ��Ƽ��
int sinDownCriticalAging(sITEMINFO *pItem)
{
	pItem->Critical_Hit -=1; //�ϴ� �ϳ������ְ�
	if(DownNum > 2){
		if(pItem->ItemAgingNum[0] && (pItem->ItemAgingNum[0]%2) == 1){
			pItem->Critical_Hit -= 1;
		}
	}
	return TRUE;

}

//����
int sinDownBlockRateAging(sITEMINFO *pItem)
{
	pItem->fBlock_Rating -=1; //�ϴ� �ϳ������ְ�
	if(DownNum > 2){
		if(pItem->ItemAgingNum[0] && (pItem->ItemAgingNum[0]%2) == 1){
			pItem->fBlock_Rating -= 1;

		}
	}
	return TRUE;

}

//����¡ �����ʱ�ȭ
int sinAgingItemInit(sITEMINFO *pItem , int Down)
{

	float TempDefense[2] = {0,0}; 
	DownNum = Down + 1;	
	int AgingLevelTemp = 0;
	AgingLevelTemp = pItem->ItemAgingNum[0];

	pItem->ItemAgingNum[0] -= DownNum;	//���� ������ �����ش� (ũ��Ƽ�� ��깮��)
/*
	if((pItem->CODE & sinITEM_MASK2) == sinMA1){
	}
	else{
	
*/	
	//����¡ �����ٿ� --
	switch(pItem->CODE & sinITEM_MASK2){
		case sinWA1: //���� 
			//���ݷ� 
			sinDownDamageAging(pItem,AgingLevelTemp);
			//���߷�
			sinDownAttackRateAging(pItem , 10);
			//������
			sinDownDurabilityAging(pItem); 

		break;
		case sinWC1: //Ŭ��
			//���ݷ� 
			sinDownDamageAging(pItem,AgingLevelTemp);
			//���߷�
			sinDownAttackRateAging(pItem , 5);
			//ũ��Ƽ��  
			sinDownCriticalAging(pItem);

		break;

		case sinWH1: //�ظ� 
			//���ݷ� 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//���߷�
			sinDownAttackRateAging(pItem , 10);
			//������
			sinDownDurabilityAging(pItem); 

		break;
		case sinWM1: //������ ������ 
			//���ݷ� 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//���߷�
			sinDownAttackRateAging(pItem , 10);

			//���  
			pItem->fIncrease_Mana -= 10*DownNum;

		break;
		case sinWP1: //â 
			//���ݷ� 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//���߷�
			sinDownAttackRateAging(pItem , 5);

			//ũ��Ƽ��  
			sinDownCriticalAging(pItem);

		break;
		case sinWS1: //Ȱ 
			//���ݷ� 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//ũ��Ƽ��  
			sinDownCriticalAging(pItem);

			//������
			sinDownDurabilityAging(pItem); 
		break;

		case sinWS2: //Į  
			//���ݷ� 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//���߷�
			sinDownAttackRateAging(pItem , 5);
			
			//ũ��Ƽ��  
			sinDownCriticalAging(pItem);

		break;

		case sinWT1: //�ں��� 
			//���ݷ� 
			sinDownDamageAging(pItem,AgingLevelTemp);

			//ũ��Ƽ��  
			sinDownCriticalAging(pItem);

			//������
			//sinSetDurabilityAging(pItem); 
		break;
			
		case sinDS1: //���� 
			//���� 
			sinDownBlockRateAging(pItem);
			//�����
			sinDownAbsorbAging(pItem , 0.2f ,AgingLevelTemp );
			//������
			sinDownDurabilityAging(pItem); 
		break;

		case sinOM1: //������ ���� 
			//���� 
			sinDownDefenseAging(pItem,10);

			//�����
			sinDownAbsorbAging(pItem , 0.5f,AgingLevelTemp);

			//������
			sinDownDurabilityAging(pItem); 

		break;
		case sinDA1: case sinDA2://���� 
			//���� 
			sinDownDefenseAging(pItem,5);

			//�����
			sinDownAbsorbAging(pItem , 0.5f,AgingLevelTemp);

			//������
			sinDownDurabilityAging(pItem); 

		break; 
		case sinDB1: // ����� - ����¡ ������ �߰�(����)
			//���� 
			sinDownDefenseAging(pItem,10);
			//�����
			sinDownAbsorbAging(pItem , 0.5f, AgingLevelTemp);
			//������
			sinDownDurabilityAging(pItem); 
		break;
		case sinDG1: // ����� - ����¡ ������ �߰�(�尩)
			//���� 
			sinDownDefenseAging(pItem,10);
			//�����
			sinDownAbsorbAging(pItem , 0.5f, AgingLevelTemp);
			//������
			sinDownDurabilityAging(pItem); 
		break;
		case sinOA2: // ����� - ����¡ ������ �߰�(�ϸ�)
			//���� 
			sinDownDefenseAging(pItem,10);
			//���߷�
			sinDownAttackRateAging(pItem , 5);
			//������
			sinDownDurabilityAging(pItem); 
		break;
		}

		pItem->ItemAgingNum[1] = 0; //�ʱ�ȭ 
		pItem->ItemAgingCount[0] = 0;
		pItem->ItemAgingCount[1] = 0;
	
		// pluto ����¡
		if( pItem->Level >= 100 ){
			pItem->Level -= 0 ;
		}
		else

			pItem->Level -= 1; //������ ���ְ�
			if(DownNum > 2){
				if((pItem->ItemAgingNum[0] % 2 ) == 1){
					pItem->Level -=1; //����¡������ Ȧ���ϰ�� ������ �� �����ش� 
			}
	}


	ReformItem(pItem);  //������ ���� �ޱ� 
//	ReformMixItem( pItem ); //����¡ ������ 
	return TRUE;
}

int sinCheckAgingLevel(int Kind, bool bBillingUse)
{

	sITEM *pItem = 0;
	switch(Kind){
		case  SIN_AGING_ATTACK:
			if(!cInvenTory.InvenItem[sInven[0].ItemIndex-1].Flag ||
			!cInvenTory.InvenItem[sInven[0].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;
			
			if((cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWA1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWH1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWM1 ){
					pItem = &cInvenTory.InvenItem[sInven[0].ItemIndex-1];
				}

		break;
		/*
		case  SIN_AGING_DEFENSE:
			//////////// ���� , ����  (�ι� üũ�� �ؾ��ϱ⶧���� �̷����س��´�)
			if(cInvenTory.InvenItem[sInven[1].ItemIndex-1].Flag ||
				cInvenTory.InvenItem[sInven[1].ItemIndex-1].sItemInfo.ItemAgingNum[1]){
					if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinOM1){
						pItem = &cInvenTory.InvenItem[sInven[1].ItemIndex-1];
						if( pItem && pItem->sItemInfo.ItemKindCode == ITEM_KIND_AGING) {
							///////////ȣ��ɶ����� �÷��ش�  
							if(smConfig.DebugMode){
								pItem->sItemInfo.ItemAgingCount[0] +=200;
							}
							else{
								pItem->sItemInfo.ItemAgingCount[0]++;
							}
							if(pItem->sItemInfo.ItemAgingCount[1] &&  pItem->sItemInfo.ItemAgingCount[0] >= pItem->sItemInfo.ItemAgingCount[1] ){
								//������ �������� ������ 
								SendAgingUpgradeItemToServer(&pItem->sItemInfo); 
							}
						}
						pItem = 0;
					}
				}

			// ���� , �κ� 	
			if(cInvenTory.InvenItem[sInven[2].ItemIndex-1].Flag ||
				cInvenTory.InvenItem[sInven[2].ItemIndex-1].sItemInfo.ItemAgingNum[1]){
				if((cInvenTory.InvenItem[sInven[2].ItemIndex-1].CODE &sinITEM_MASK2) == sinDA1 ||
					(cInvenTory.InvenItem[sInven[2].ItemIndex-1].CODE &sinITEM_MASK2) == sinDA2 ){
					pItem = &cInvenTory.InvenItem[sInven[2].ItemIndex-1];
				}
			}
		
		break;
		*/
		case  SIN_AGING_CRITICAL:
			if(!cInvenTory.InvenItem[sInven[0].ItemIndex-1].Flag ||
			!cInvenTory.InvenItem[sInven[0].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;
			if((cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWC1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWS2 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWS1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWP1 ||
				(cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE &sinITEM_MASK2) == sinWT1){
					pItem = &cInvenTory.InvenItem[sInven[0].ItemIndex-1];

				}

		break;

		case  SIN_AGING_BLOCK:
			if(	!cInvenTory.InvenItem[sInven[1].ItemIndex-1].Flag ||
				!cInvenTory.InvenItem[sInven[1].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;

			if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinDS1)
				pItem = &cInvenTory.InvenItem[sInven[1].ItemIndex-1];

		break;

		// ����� - ����¡ ������ ������
		case SIN_AGING_DEFENSE_ARMOR: // ����, �κ�
			// ���� , �κ� 	
			if(cInvenTory.InvenItem[sInven[2].ItemIndex-1].Flag ||
				cInvenTory.InvenItem[sInven[2].ItemIndex-1].sItemInfo.ItemAgingNum[1]){
					if((cInvenTory.InvenItem[sInven[2].ItemIndex-1].CODE &sinITEM_MASK2) == sinDA1 ||
						(cInvenTory.InvenItem[sInven[2].ItemIndex-1].CODE &sinITEM_MASK2) == sinDA2 ){
							pItem = &cInvenTory.InvenItem[sInven[2].ItemIndex-1];
						}
				}
		break;
		
		// ����� - ����¡ ������ ������
		case SIN_AGING_DEFENSE_ORB: // ���� ����
			if(cInvenTory.InvenItem[sInven[1].ItemIndex-1].Flag ||
				cInvenTory.InvenItem[sInven[1].ItemIndex-1].sItemInfo.ItemAgingNum[1]){
					if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinOM1){
						pItem = &cInvenTory.InvenItem[sInven[1].ItemIndex-1];
					}
				}
		break;

		// ����� - ����¡ ������ �߰�(����)
		case SIN_AGING_DEFENSE_BOOTS: // ����
			if(	!cInvenTory.InvenItem[sInven[9].ItemIndex-1].Flag ||
				!cInvenTory.InvenItem[sInven[9].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;

			if((cInvenTory.InvenItem[sInven[9].ItemIndex-1].CODE &sinITEM_MASK2) == sinDB1)
				pItem = &cInvenTory.InvenItem[sInven[9].ItemIndex-1];
			break;

			// ����� - ����¡ ������ �߰�(�尩)
		case SIN_AGING_DEFENSE_GLOVES: // �尩
			if(	!cInvenTory.InvenItem[sInven[8].ItemIndex-1].Flag ||
				!cInvenTory.InvenItem[sInven[8].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;

			if((cInvenTory.InvenItem[sInven[8].ItemIndex-1].CODE &sinITEM_MASK2) == sinDG1)
				pItem = &cInvenTory.InvenItem[sInven[8].ItemIndex-1];
			break;

			// ����� - ����¡ ������ �߰�(�ϸ�)
		case SIN_AGING_DEFENSE_ARMLETS: // �ϸ�
			if(	!cInvenTory.InvenItem[sInven[7].ItemIndex-1].Flag ||
				!cInvenTory.InvenItem[sInven[7].ItemIndex-1].sItemInfo.ItemAgingNum[1])return FALSE;

			if((cInvenTory.InvenItem[sInven[7].ItemIndex-1].CODE &sinITEM_MASK2) == sinOA2)
				pItem = &cInvenTory.InvenItem[sInven[7].ItemIndex-1];
			break;
	}

	if(!pItem || pItem->sItemInfo.ItemKindCode != ITEM_KIND_AGING)return FALSE;

	///////////ȣ��ɶ����� �÷��ش�  
	if(smConfig.DebugMode)
	{
		if(bBillingUse == false)
		{
			pItem->sItemInfo.ItemAgingCount[0] +=200;
		}
		else if(bBillingUse == true)
			pItem->sItemInfo.ItemAgingCount[0] = pItem->sItemInfo.ItemAgingCount[1];
	}
	else
	{
		if(bBillingUse == false)
		{
			pItem->sItemInfo.ItemAgingCount[0]++;
		}
		else if(bBillingUse == true)
		{
			pItem->sItemInfo.ItemAgingCount[0] = pItem->sItemInfo.ItemAgingCount[1];
		}
	}

	if(pItem->sItemInfo.ItemAgingCount[1] && pItem->sItemInfo.ItemAgingCount[0] >= pItem->sItemInfo.ItemAgingCount[1] ){
		//������ �������� ������ 
		SendAgingUpgradeItemToServer(&pItem->sItemInfo);
	}



	/*

	//����¡ �������� �ƴϰų� ����¡�� ���� �������� �����Ѵ� 
	if(pItem->sItemInfo.ItemKindCode != ITEM_KIND_AGING ||
		(pItem->sItemInfo.ItemAgingCount[0] >= pItem->sItemInfo.ItemAgingCount[1]) )
		return FALSE;
		

	//////////////�� �밡�� �۾� 
	if(!CheckItemForm(&pItem->sItemInfo)){ //������ �����ޱ� 
		SendSetHackUser(12); //��ŷ�� �Ϸ����ߴ� ����!! ������ ��� TRUE ���� ���� 
		return FALSE;

	}

	//����¡ ������ ����Ȯ�� 
	if(!CheckMixItem( &pItem->sItemInfo )){
		SendSetHackUser(52); //��ŷ�� �Ϸ����ߴ� ���� ������ ��� TRUE ���� ���� 
		return FALSE;
	}
	*/

	return TRUE;	
}

//�ɷ�ġ�� ���� �������� �޴´� 
int sinRecvAgingIncreStateItem(sITEMINFO *pItem)
{
	for(int i = 0; i  < INVENTORY_MAXITEM ; i++){ 
		if(cInvenTory.InvenItem[i].sItemInfo.BackUpChkSum  == pItem->BackUpChkSum &&
			cInvenTory.InvenItem[i].sItemInfo.BackUpKey == pItem->BackUpKey &&
			cInvenTory.InvenItem[i].sItemInfo.CODE == pItem->CODE){
				memcpy(&cInvenTory.InvenItem[i].sItemInfo,pItem,sizeof(sITEMINFO));
				cInvenTory.ReFormInvenItem();   //�κ��丮 ������ ���� 
				cInvenTory.CheckRequireItemToSet(&cInvenTory.InvenItem[i]); 
				cInvenTory.SetItemToChar(); //�ɷ�ġ ������
				break;

		}
	}

	return TRUE;
}

//����¡ �����ۿ� ���� ��ŷ�� ���´� 
int sinCheckAgingItemHack(sITEM *pItem)
{ 
	if(	pItem->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT){
		return FALSE;

	}
	return TRUE;
}

///////////////��ŷ������ ���� ���� �Ѱ����� �����Ѵ� 
int sinMinusMoney(int Money,int Kind)
{
	//������ ������ ����޴´�.
	if(Kind){
		//--------------------------------------------------------------------------//
		#ifdef HASIEGE_MODE
		    //������ �������� ���̳ʽ� �Ӵ� ����
			
		    sinChar->Money -= Money+((Money*cSinSiege.GetTaxRate())/100);
			SetTotalSubMoney(Money+(Money*cSinSiege.GetTaxRate())/100);
		#else
			sinChar->Money -=Money;
		#endif
		//---------------------------------------------------------------------------//
	}
	else{
		sinChar->Money -=Money;
		SetTotalSubMoney(Money);
	
	}

	ReformCharForm();//������ 
	
	SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

	return TRUE	;		
}

int sinPlusMoney(int Money)
{
	sinChar->Money += Money;

	ReformCharForm();//������ 
	SetTotalAddMoney(Money);
	SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

	return TRUE;
}

///////////////��ŷ������ ���� ���� �Ѱ����� �����Ѵ� (â�� ���� ���� )
int sinMinusMoney2(int Money)
{
	sinChar->Money -= Money;

	ReformCharForm();//������ 
	SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

	return TRUE;
}

int sinPlusMoney2(int Money)
{
	sinChar->Money += Money;

	ReformCharForm();//������ 
	SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

	return TRUE;
}

//���ҼӼ��� ���Ѵ� 
int sinGetElement(smCHAR_INFO *pCharInfo)
{
/*
	sinSkillResistance[sITEMINFO_BIONIC] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
	sinSkillResistance[sITEMINFO_FIRE] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
	sinSkillResistance[sITEMINFO_ICE] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
	sinSkillResistance[sITEMINFO_LIGHTING] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
	sinSkillResistance[sITEMINFO_POISON] += Zenith_Element[sinSkill.UseSkill[i].Point-1]/TempDivide;
*/	
	
	
//	pCharInfo->Resistance
	
	return TRUE;
}
char  *PresentWeaponName[5][9] ={
	{"sp103" , 0,0,0,0,0,0,0,0 },
	{"fo104","fo105","fo106",0,0,0,0,0,0},
	{"fo101","fo102","fo103","os106",0,0,0,0,0},
	{"fo107","se103","se102","os107",0,0,0,0,0},
	{"se101","os108","fo108","gp111",0,0,0,0,0},
	
};


char  *PresentArmorName[5][9] ={
	{"pm104" , 0,0,0,0,0,0,0,0 },
	{"gp104","gp105","gp106","gp107",0,0,0,0,0},
	{"gp101","gp102","gp103","os106",0,0,0,0,0},
	{"gp108","gp109","gp110","os107",0,0,0,0,0},
	{"se101","os108","fo108","gp111",0,0,0,0,0},
	
};

int PresentPercentIndex[5] = {5900,2250,1450,250,150};

///���� ���� ������ ioi
//����

#ifdef _LANGUAGE_VEITNAM
char  *PresentWeaponName2[7][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//7
	{"wa117","wc117","wh118","wp118","ws119","ws220","wt118","wm118","os119"},
	{"os110",0,0,0,0,0,0,0,0},
	{"wa118","wc118","wh119","wp119","ws120","ws221","wt119","wm119","oa121"},
	{0,0,0,0,0,0,0,0,0},
	{"wa119","wc119","wh120","wp120","ws121","ws222","wt120","wm120",0},
	{"wa121","wc121","wh122","wp122","ws123","ws224","wt122","wm122",0},
};

char  *PresentArmorName2[7][9] ={
	{"pm104",0,0,0,0,0,0,0,0},
	{"da119","da219","ds117","om118","dg117","da217","db117","or120","oa120"},
	{0,0,0,0,0,0,0,0,0},
	{"da120","da220","ds118","om119","dg118","oa218","db118","os111","or121"},
	{"os112",0,0,0,0,0,0,0,0},
	{"da121","da221","ds119","om120","dg119","oa219","db119",0,0},
	{"da123","da223","ds121","om122","dg121","oa221","db121",0,0},
};

	int PresentPercentIndex2[7] = {4000,3000,1800,800,250,100,50};

#else
// ����� - �Ϻ� ���� �̺�Ʈ(���� ������ ���� ����Ʈ �� Ȯ��)
char  *PresentWeaponName2[8][9] ={
	{"bi123",0,0,0,0,0,0,0,0 }, // 2500
	{"pm104",0,0,0,0,0,0,0,0 }, // 2500
	{"os106",0,0,0,0,0,0,0,0 }, // 1590
	{"gp109",0,0,0,0,0,0,0,0 }, // 1500
	{"or228",0,0,0,0,0,0,0,0 }, // 700
	{"oa133",0,0,0,0,0,0,0,0 }, // 700
	{"gp121",0,0,0,0,0,0,0,0 }, // 500
	{"os109",0,0,0,0,0,0,0,0 }, // 10
};

//��
char  *PresentArmorName2[8][9] ={
	{"bi123",0,0,0,0,0,0,0,0 }, // 2500
	{"pm104",0,0,0,0,0,0,0,0 }, // 2500
	{"os106",0,0,0,0,0,0,0,0 }, // 1590
	{"gp109",0,0,0,0,0,0,0,0 }, // 1500
	{"or228",0,0,0,0,0,0,0,0 }, // 700
	{"oa133",0,0,0,0,0,0,0,0 }, // 700
	{"gp121",0,0,0,0,0,0,0,0 }, // 500
	{"os109",0,0,0,0,0,0,0,0 }, // 10
};


int PresentPercentIndex2[8] = {2500,2500,1590,1500,700,700,500,10};
#endif

//�ϰ����� ũ����Ż �̺�Ʈ (�ؿ� )
#ifdef _LANGUAGE_VEITNAM

	char  *PresentWeaponName3[5][9] ={
		{"pm104" , 0,0,0,0,0,0,0,0 },
		{"wa117","wc117","wh118","wp118","ws119","ws220","wt118","wm118","os109"},
		{"wa118","wc118","wh119","wp119","ws120","ws221","wt119","wm119","os111"},
		{"wa119","wc119","wh120","wp120","ws121","ws222","wt120","wm120","os112"},
		{"wa121","wc121","wh122","wp122","ws123","ws224","wt122","wm122",0},
		
	};

	char  *PresentArmorName3[5][9] ={
		{"pm104" , 0,0,0,0,0,0,0,0 },
		{"da119","da219","ds117","om118","dg117","oa217","db117","os110",0},
		{"da120","da220","ds118","om119","dg118","oa218","db118","or120","oa120"},
		{"da121","da221","ds119","om120","dg119","oa219","db119","or121","oa121"},
		{"da123","da223","ds121","om122","dg121","oa221","db121",0,0},
		
	};

	int PresentPercentIndex3[5] = {6500,2500,600,300,100};

#else
/*
	char  *PresentWeaponName3[5][9] ={
		{"pm104" , 0,0,0,0,0,0,0,0 },
		{"wa116","wc116","wh117","wm117","wp117","ws118","ws219","wt117",0},
		{"wa117","wc117","wh118","wm118","wp118","ws119","ws220","wt118",0},
		{"se101",0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		
	};

	char  *PresentArmorName3[5][9] ={
		{"pm104" , 0,0,0,0,0,0,0,0 },
		{"da118","da218","ds116","om117",0,0,0,0,0},
		{"da119","da219","ds117","om118",0,0,0,0,0},
		{"se101",0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		
	};

//	int PresentPercentIndex3[5] = {9300,300,200,200,0};
	int PresentPercentIndex3[5] = {2500,2500,2500,2500,0};
*/

/*
//����� - ���� �̺�Ʈ ���� ������ 
char  *PresentWeaponName3[12][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"bi123",0,0,0,0,0,0,0,0 },		//2
	{"gp109",0,0,0,0,0,0,0,0 },		//3
	{"gp121",0,0,0,0,0,0,0,0 },		//4
	{"or228",0,0,0,0,0,0,0,0},		//5
	{"oa133",0,0,0,0,0,0,0,0},		//6
	{"da121",0,0,0,0,0,0,0,0},		//7
	{"da221",0,0,0,0,0,0,0,0},		//8
	{"oa219",0,0,0,0,0,0,0,0},		//9
	{"dg119",0,0,0,0,0,0,0,0},		//10
	{"os112",0,0,0,0,0,0,0,0},		//11
	{"se101",0,0,0,0,0,0,0,0},		//12
};

char  *PresentArmorName3[12][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"bi123",0,0,0,0,0,0,0,0 },		//2
	{"gp109",0,0,0,0,0,0,0,0 },		//3
	{"gp121",0,0,0,0,0,0,0,0 },		//4
	{"or228",0,0,0,0,0,0,0,0},		//5
	{"oa133",0,0,0,0,0,0,0,0},		//6
	{"da121",0,0,0,0,0,0,0,0},		//7
	{"da221",0,0,0,0,0,0,0,0},		//8
	{"oa219",0,0,0,0,0,0,0,0},		//9
	{"dg119",0,0,0,0,0,0,0,0},		//10
	{"os112",0,0,0,0,0,0,0,0},		//11
	{"se101",0,0,0,0,0,0,0,0},		//12
};

int PresentPercentIndex3[12] = {3000,2000,1700,1600,650,650,50,50,100,100,50,50};
*/

/*
//����� - ȣ�� �̺�Ʈ ���� ������ 
char  *PresentWeaponName3[9][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"bi123",0,0,0,0,0,0,0,0 },		//2
	{"gp109",0,0,0,0,0,0,0,0 },		//3
	{"gp121",0,0,0,0,0,0,0,0},		//4
	{"or228",0,0,0,0,0,0,0,0},		//5
	{"oa133",0,0,0,0,0,0,0,0},		//6
	{"os106",0,0,0,0,0,0,0,0},		//7
	{"os107",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0 },		//9
};

char  *PresentArmorName3[9][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"bi123",0,0,0,0,0,0,0,0 },		//2
	{"gp109",0,0,0,0,0,0,0,0 },		//3
	{"gp121",0,0,0,0,0,0,0,0},		//4
	{"or228",0,0,0,0,0,0,0,0},		//5
	{"oa133",0,0,0,0,0,0,0,0},		//6
	{"os106",0,0,0,0,0,0,0,0},		//7
	{"os107",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0 },		//9
};
*/

//�庰 - �߷�Ÿ�� �̺�Ʈ ���� ������ 
char  *PresentChocoWeaponName3[17][9] ={
	{"sp115",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"GP120",0,0,0,0,0,0,0,0 },		//3
	{"GP109",0,0,0,0,0,0,0,0 },		//4
	{"bi106",0,0,0,0,0,0,0,0},		//5
	{"Bi107",0,0,0,0,0,0,0,0},		//6
	{"OS107",0,0,0,0,0,0,0,0},		//7
	{"OS108",0,0,0,0,0,0,0,0},		//8
	{"Da131",0,0,0,0,0,0,0,0},		//9
	{"Da231",0,0,0,0,0,0,0,0},		//10
	{"Da133",0,0,0,0,0,0,0,0},		//11
	{"Da233",0,0,0,0,0,0,0,0},		//12
	{"OR233",0,0,0,0,0,0,0,0},		//13
	{"FO107",0,0,0,0,0,0,0,0},		//14
	{"FO106",0,0,0,0,0,0,0,0},		//15
	{"bi108",0,0,0,0,0,0,0,0},		//16
	{"OS109",0,0,0,0,0,0,0,0},		//17
};

char  *PresentChocoArmorName3[17][9] ={
	{"sp115",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"GP120",0,0,0,0,0,0,0,0 },		//3
	{"GP109",0,0,0,0,0,0,0,0 },		//4
	{"bi106",0,0,0,0,0,0,0,0},		//5
	{"Bi107",0,0,0,0,0,0,0,0},		//6
	{"OS107",0,0,0,0,0,0,0,0},		//7
	{"OS108",0,0,0,0,0,0,0,0},		//8
	{"Da131",0,0,0,0,0,0,0,0},		//9
	{"Da231",0,0,0,0,0,0,0,0},		//10
	{"Da133",0,0,0,0,0,0,0,0},		//11
	{"Da233",0,0,0,0,0,0,0,0},		//12
	{"OR233",0,0,0,0,0,0,0,0},		//13
	{"FO107",0,0,0,0,0,0,0,0},		//14
	{"FO106",0,0,0,0,0,0,0,0},		//15
	{"bi108",0,0,0,0,0,0,0,0},		//16
	{"OS109",0,0,0,0,0,0,0,0},		//17
};

int PresentChocoPercentIndex3[17] = {1000,1000,100,200,1200,1200,489,10,600,600,600,600,400,900,900,200,1};


// �庰 - ĵ������ ���� ������
char  *PresentCandyWeaponName3[17][9] ={
	{"sp115",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"GP121",0,0,0,0,0,0,0,0 },		//3
	{"GP109",0,0,0,0,0,0,0,0 },		//4
	{"bi106",0,0,0,0,0,0,0,0},		//5
	{"Bi107",0,0,0,0,0,0,0,0},		//6
	{"OS107",0,0,0,0,0,0,0,0},		//7
	{"OS108",0,0,0,0,0,0,0,0},		//8
	{"Da139",0,0,0,0,0,0,0,0},		//9
	{"Da239",0,0,0,0,0,0,0,0},		//10
	{"Da141",0,0,0,0,0,0,0,0},		//11
	{"DA241",0,0,0,0,0,0,0,0},		//12
	{"OA137",0,0,0,0,0,0,0,0},		//13
	{"FO107",0,0,0,0,0,0,0,0},		//14
	{"FO106",0,0,0,0,0,0,0,0},		//15
	{"bi108",0,0,0,0,0,0,0,0},		//16
	{"OS109",0,0,0,0,0,0,0,0},		//17
};



char  *PresentCandyArmorName3[17][9] ={
	{"sp115",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"GP121",0,0,0,0,0,0,0,0 },		//3
	{"GP109",0,0,0,0,0,0,0,0 },		//4
	{"bi106",0,0,0,0,0,0,0,0},		//5
	{"Bi107",0,0,0,0,0,0,0,0},		//6
	{"OS107",0,0,0,0,0,0,0,0},		//7
	{"OS108",0,0,0,0,0,0,0,0},		//8
	{"Da139",0,0,0,0,0,0,0,0},		//9
	{"Da239",0,0,0,0,0,0,0,0},		//10
	{"Da141",0,0,0,0,0,0,0,0},		//11
	{"DA241",0,0,0,0,0,0,0,0},		//12
	{"OA137",0,0,0,0,0,0,0,0},		//13
	{"FO107",0,0,0,0,0,0,0,0},		//14
	{"FO106",0,0,0,0,0,0,0,0},		//15
	{"bi108",0,0,0,0,0,0,0,0},		//16
	{"OS109",0,0,0,0,0,0,0,0},		//17
};

int PresentCandyPercentIndex3[17] = {1000,1000,100,200,1200,1200,489,10,600,600,600,600,400,900,900,200,1};


// �庰 - �����ñ׸� ���޶��� ���� ������
char  *PresentMagicalGreenEmeraldWeaponName3[18][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },	//1
	{"bi119",0,0,0,0,0,0,0,0 },		//2
	{"gp118",0,0,0,0,0,0,0,0 },		//3
	{"gp120",0,0,0,0,0,0,0,0 },		//4
	{"gp119",0,0,0,0,0,0,0,0},		//5
	{"se101",0,0,0,0,0,0,0,0},		//6
	{"os107",0,0,0,0,0,0,0,0},		//7
	{"os108",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0},		//9
	{"os110",0,0,0,0,0,0,0,0},		//10
	{"os111",0,0,0,0,0,0,0,0},		//11
	{"bi112",0,0,0,0,0,0,0,0},		//12
	{"bi108",0,0,0,0,0,0,0,0},		//13
	{"bi107",0,0,0,0,0,0,0,0},		//14
	{"bi106",0,0,0,0,0,0,0,0},		//15
	{"fo107",0,0,0,0,0,0,0,0},		//16
	{"fo108",0,0,0,0,0,0,0,0},		//17
	{"fo109",0,0,0,0,0,0,0,0},		//18
};



char  *PresentMagicalGreenEmeraldArmorName3[18][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },	//1
	{"bi119",0,0,0,0,0,0,0,0 },		//2
	{"gp118",0,0,0,0,0,0,0,0 },		//3
	{"gp120",0,0,0,0,0,0,0,0 },		//4
	{"gp119",0,0,0,0,0,0,0,0},		//5
	{"se101",0,0,0,0,0,0,0,0},		//6
	{"os107",0,0,0,0,0,0,0,0},		//7
	{"os108",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0},		//9
	{"os110",0,0,0,0,0,0,0,0},		//10
	{"os111",0,0,0,0,0,0,0,0},		//11
	{"bi112",0,0,0,0,0,0,0,0},		//12
	{"bi108",0,0,0,0,0,0,0,0},		//13
	{"bi107",0,0,0,0,0,0,0,0},		//14
	{"bi106",0,0,0,0,0,0,0,0},		//15
	{"fo107",0,0,0,0,0,0,0,0},		//16
	{"fo108",0,0,0,0,0,0,0,0},		//17
	{"fo109",0,0,0,0,0,0,0,0},		//18
};

int PresentMagicalGreenEmeraldPercentIndex3[18] = {1400,300,400,200,100,1,300,200,1,1,1,100,1500,1500,1500,1200,800, 496};


// �庰 - �����ñ׸� ���� ���� ������
char  *PresentMagicalGreenJadeWeaponName3[15][9] ={
	{"pm103",0,0,0,0,0,0,0,0 },	//1
	{"Gp109",0,0,0,0,0,0,0,0 },	//2
	{"Gp121",0,0,0,0,0,0,0,0 },	//3
	{"Fo104",0,0,0,0,0,0,0,0 },		//4
	{"Fo105",0,0,0,0,0,0,0,0},		//5
	{"Fo106",0,0,0,0,0,0,0,0},		//6
	{"Os105",0,0,0,0,0,0,0,0},		//7
	{"Os106",0,0,0,0,0,0,0,0},		//8
	{"Da135",0,0,0,0,0,0,0,0},		//9
	{"Da137",0,0,0,0,0,0,0,0},		//10
	{"da235",0,0,0,0,0,0,0,0},		//11
	{"Da237",0,0,0,0,0,0,0,0},		//12
	{"Bi112",0,0,0,0,0,0,0,0},		//13
	{"Bi119",0,0,0,0,0,0,0,0},		//14
	{"bi121",0,0,0,0,0,0,0,0},		//15
};



char  *PresentMagicalGreedJadeArmorName3[15][9] ={
	{"pm103",0,0,0,0,0,0,0,0 },	//1
	{"Gp109",0,0,0,0,0,0,0,0 },	//2
	{"Gp121",0,0,0,0,0,0,0,0 },	//3
	{"Fo104",0,0,0,0,0,0,0,0 },		//4
	{"Fo105",0,0,0,0,0,0,0,0},		//5
	{"Fo106",0,0,0,0,0,0,0,0},		//6
	{"Os105",0,0,0,0,0,0,0,0},		//7
	{"Os106",0,0,0,0,0,0,0,0},		//8
	{"Da135",0,0,0,0,0,0,0,0},		//9
	{"Da137",0,0,0,0,0,0,0,0},		//10
	{"da235",0,0,0,0,0,0,0,0},		//11
	{"Da237",0,0,0,0,0,0,0,0},		//12
	{"Bi112",0,0,0,0,0,0,0,0},		//13
	{"Bi119",0,0,0,0,0,0,0,0},		//14
	{"bi121",0,0,0,0,0,0,0,0},		//15
};

int PresentMagicalGreedJadePercentIndex3[15] = {1500,1500,200,1500,1500,1000,300,200,500,500,500,500,100,100,100};


// �庰 - ī���� ���� ���� ������
char  *PresentTearOfKaraWeaponName3[18][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"bi119",0,0,0,0,0,0,0,0 },		//2
	{"gp109",0,0,0,0,0,0,0,0 },		//3
	{"gp121",0,0,0,0,0,0,0,0 },		//4
	{"gp119",0,0,0,0,0,0,0,0},		//5
	{"se101",0,0,0,0,0,0,0,0},		//6
	{"os107",0,0,0,0,0,0,0,0},		//7
	{"os108",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0},		//9
	{"os110",0,0,0,0,0,0,0,0},		//10
	{"os111",0,0,0,0,0,0,0,0},		//11
	{"bi112",0,0,0,0,0,0,0,0},		//12
	{"bi108",0,0,0,0,0,0,0,0},		//13
	{"bi107",0,0,0,0,0,0,0,0},		//14
	{"bi106",0,0,0,0,0,0,0,0},		//15
	{"fo107",0,0,0,0,0,0,0,0},		//16
	{"fo108",0,0,0,0,0,0,0,0},		//17
	{"fo109",0,0,0,0,0,0,0,0},		//18
};



char  *PresentTearOfKaraArmorName3[18][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"bi119",0,0,0,0,0,0,0,0 },		//2
	{"gp109",0,0,0,0,0,0,0,0 },		//3
	{"gp121",0,0,0,0,0,0,0,0 },		//4
	{"gp119",0,0,0,0,0,0,0,0},		//5
	{"se101",0,0,0,0,0,0,0,0},		//6
	{"os107",0,0,0,0,0,0,0,0},		//7
	{"os108",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0},		//9
	{"os110",0,0,0,0,0,0,0,0},		//10
	{"os111",0,0,0,0,0,0,0,0},		//11
	{"bi112",0,0,0,0,0,0,0,0},		//12
	{"bi108",0,0,0,0,0,0,0,0},		//13
	{"bi107",0,0,0,0,0,0,0,0},		//14
	{"bi106",0,0,0,0,0,0,0,0},		//15
	{"fo107",0,0,0,0,0,0,0,0},		//16
	{"fo108",0,0,0,0,0,0,0,0},		//17
	{"fo109",0,0,0,0,0,0,0,0},		//18
};

int PresentTearOfKaraPercentIndex3[18] = {1400,300,400,200,100,1,300,200,1,1,1,100,1500,1500,1500,1200,800,496};

// �庰 - ������� ã�ƶ� ���ξƹķ� ����
char  *PresentFindinvestigatorNineWeaponName3[1][9] = {"gf107",0,0,0,0,0,0,0,0 };
char  *PresentFindinvestigatorNineArmorName3[1][9] = {"gf107",0,0,0,0,0,0,0,0 };
int PresentFindinvestigatorNinePercentIndex3[1] = {10000};

// �庰 - ������� ã�ƶ� ���Ͼƹķ� ����
char  *PresentFindinvestigatorNineTaleWeaponName3[1][9] = {"gf108",0,0,0,0,0,0,0,0 };
char  *PresentFindinvestigatorNineTaleArmorName3[1][9] = {"gf108",0,0,0,0,0,0,0,0 };
int PresentFindinvestigatorNineTalePercentIndex3[1] = {10000};

// �庰 - ������� ã�ƶ� ���� ���� ������
char  *PresentFindinvestigatorWeaponName3[12][9] ={
	{"bi109",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"fo109",0,0,0,0,0,0,0,0 },		//3
	{"fo110",0,0,0,0,0,0,0,0 },		//4
	{"gp110",0,0,0,0,0,0,0,0},		//5
	{"gp119",0,0,0,0,0,0,0,0},		//6
	{"gp120",0,0,0,0,0,0,0,0},		//7
	{"bi178",0,0,0,0,0,0,0,0},		//8
	{"bi183",0,0,0,0,0,0,0,0},		//9
	{"bi177",0,0,0,0,0,0,0,0},		//10
	{"oa135",0,0,0,0,0,0,0,0},		//11
	{"or230",0,0,0,0,0,0,0,0},		//12
};



char  *PresentFindinvestigatorArmorName3[12][9] ={
	{"bi109",0,0,0,0,0,0,0,0 },		//1
	{"pm104",0,0,0,0,0,0,0,0 },		//2
	{"fo109",0,0,0,0,0,0,0,0 },		//3
	{"fo110",0,0,0,0,0,0,0,0 },		//4
	{"gp110",0,0,0,0,0,0,0,0},		//5
	{"gp119",0,0,0,0,0,0,0,0},		//6
	{"gp120",0,0,0,0,0,0,0,0},		//7
	{"bi178",0,0,0,0,0,0,0,0},		//8
	{"bi183",0,0,0,0,0,0,0,0},		//9
	{"bi177",0,0,0,0,0,0,0,0},		//10
	{"oa135",0,0,0,0,0,0,0,0},		//11
	{"or230",0,0,0,0,0,0,0,0},		//12
};

int PresentFindinvestigatorPercentIndex3[12] = {1500,1500,1300,1000,1500,1000,1500,300,200,100,50,50};


//����� - ���ϴ��� �ҿ� �̺�Ʈ (������ ��ƿ���)
char  *PresentWeaponName3[9][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"sp115",0,0,0,0,0,0,0,0 },		//2
	{"fo107",0,0,0,0,0,0,0,0 },		//3
	{"os107",0,0,0,0,0,0,0,0},		//4
	{"bi119",0,0,0,0,0,0,0,0},		//5
	{"bi107",0,0,0,0,0,0,0,0},		//6
	{"bi106",0,0,0,0,0,0,0,0},		//7
	{"gp120",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0 },		//9
};

char  *PresentArmorName3[9][9] ={
	{"pm104",0,0,0,0,0,0,0,0 },		//1
	{"sp115",0,0,0,0,0,0,0,0 },		//2
	{"fo107",0,0,0,0,0,0,0,0 },		//3
	{"os107",0,0,0,0,0,0,0,0},		//4
	{"bi119",0,0,0,0,0,0,0,0},		//5
	{"bi107",0,0,0,0,0,0,0,0},		//6
	{"bi106",0,0,0,0,0,0,0,0},		//7
	{"gp120",0,0,0,0,0,0,0,0},		//8
	{"os109",0,0,0,0,0,0,0,0 },		//9
};

int PresentPercentIndex3[9] = {2000,2000,1300,1200,1000,1000,1000,499,1};


// ����� - ���ĺ� ���� �̺�Ʈ
char  *PresentWeaponName5[11][9] ={
	{"pm104",0,0,0,0,0,0,0,0},													//1
	{"bi108",0,0,0,0,0,0,0,0},													//2
	{"bi106","bi107",0,0,0,0,0,0,0},											//3
	{"fo107",0,0,0,0,0,0,0,0},													//4
	{"or228","oa133",0,0,0,0,0,0,0},											//5
	{"os107","os108","fo109",0,0,0,0,0,0},										//6
	{"bi119",0,0,0,0,0,0,0,0},													//7
	{"gp119","gp120","gp121",0,0,0,0,0,0},										//8
	{"bi112",0,0,0,0,0,0,0,0},													//9
	{"bi213","bi214","bi215","bi216","bi217","bi218","bi219","bi220",0},		//10
	{"os109","se101",0,0,0,0,0,0,0},											//11
};

char  *PresentArmorName5[11][9] ={
	{"pm104",0,0,0,0,0,0,0,0},													//1
	{"bi108",0,0,0,0,0,0,0,0},													//2
	{"bi106","bi107",0,0,0,0,0,0,0},											//3
	{"fo107",0,0,0,0,0,0,0,0},													//4
	{"or228","oa133",0,0,0,0,0,0,0},											//5
	{"os107","os108","fo109",0,0,0,0,0,0},										//6
	{"bi119",0,0,0,0,0,0,0,0},													//7
	{"gp119","gp120","gp121",0,0,0,0,0,0},										//8
	{"bi112",0,0,0,0,0,0,0,0},													//9
	{"bi213","bi214","bi215","bi216","bi217","bi218","bi219","bi220",0},		//10
	{"os109","se101",0,0,0,0,0,0,0},											//11
};

int PresentPercentIndex5[11] = {1450,1300,2600,1198,1000,1000,300,100,50,1000,20};

#endif





DWORD sinForceOrbCode[] = {(sinFO1|sin01),(sinFO1|sin02),(sinFO1|sin03),(sinFO1|sin04),(sinFO1|sin05),(sinFO1|sin06),(sinFO1|sin07),(sinFO1|sin08),(sinFO1|sin09),(sinFO1|sin10),(sinFO1|sin11),(sinFO1|sin12),(sinFO1|sin13),(sinFO1|sin14)}; // ����� - ����, ������ ���� �߰�

// ����� - ���� ���� �߰�
DWORD sinMagincForceOrbCode[] = {(sinFO1|sin21),(sinFO1|sin22),(sinFO1|sin23),(sinFO1|sin24),(sinFO1|sin25),(sinFO1|sin26),(sinFO1|sin27),(sinFO1|sin28),(sinFO1|sin29),(sinFO1|sin30),(sinFO1|sin31),(sinFO1|sin32),(sinFO1|sin33),(sinFO1|sin34)};
// ����� - ���� ���� ���� �߰�
DWORD sinBillingMagicForceOrbCode[] = {(sinFO1|sin35),(sinFO1|sin36),(sinFO1|sin37)};

DWORD sinWingItemQuestServer(sCRAFTITEM_SERVER *pWingItem , smCHAR_INFO *pChar_Info)
{
	//3�� ��������Ʈ
	int i = 0, j = 0 , k = 0; 
	int CodeCount = 0;
	int ItemCount = 0;
	int ItemCountPuzzle = 0;
	int PresentPercent = 0;
	int PresentItemIndex = 0;
	int PresentPercentSum = 0;
	int PresentItemCount = 0;
	int PotionNum = 0;
	short ItemCount80_2[4] = {0,};

	int ItemIndex = 0; 
	char *((*pPresentWeaponName)[9]);
	char *((*pPresentArmorName)[9]);
	int  *pPresentPercentIndex;

	int RandomTemp = 0 ;

	int CristalCount=0;
	int CristalFlag =0;
	//�ϰ����� ũ����Ż�� �ٽ�üũ�Ѵ�.
	for( i = 0 ; i < 7 ; i++ ){
		for( j = 0 ; j < 12 ; j++ ){
			if( pWingItem->SheltomCode[i]==CristalEventCODE[j] ){
				CristalCount = i+1;
			}
		}
	}
	
	int WatermelonCount=0;
	int WatermelonFlag =0;
	//����� - ���� ��ƿ���(7���� ������ �ٽ�üũ�Ѵ�.)
	for(int n = 0 ; n < 7 ; n++ )
	{
		if( pWingItem->SheltomCode[n]==WatermelonEventCODE )
		{
			WatermelonCount = n+1;
		}
	}

	int ValentineCount=0;
	int ValentineFlag =0;
	//�庰 - �߷�Ÿ�� ���ݸ� ��ƿ���(7���� ���ݸ��� �ٽ�üũ�Ѵ�.)
	for(int n = 0 ; n < 7 ; n++ )
	{
		if( pWingItem->SheltomCode[n]==ValentineEventCODE )
		{
			ValentineCount = n+1;
		}
	}


	int CandydaysCount=0;
	int CandydaysFlag =0;
	// �庰 - ĵ������ (7���� ĵ�� �ٽ�üũ�Ѵ�.)
	for(int n = 0 ; n < 7 ; n++ )
	{
		if( pWingItem->SheltomCode[n]==CandydaysEventCODE )
		{
			CandydaysCount = n+1;
		}
	}


	int MagicalGreenEmeraldCount=0;
	int MagicalGreenEmeraldFlag =0;
	int MagicalGreenJadeCount=0;
	int MagicalGreenJadeFlag =0;
	// �庰 - �����ñ׸� (7���� ���޶��� �ٽ�üũ�Ѵ�.)
	for(int a = 0 ; a < 7 ; a++ )
	{
		if( pWingItem->SheltomCode[a]==MagicalGreenEventCODE[1] )
		{
			MagicalGreenEmeraldCount = a+1;
		}
	}

	// �庰 - �����ñ׸� (7���� ���� �ٽ�üũ�Ѵ�.)
	for(int b = 0 ; b < 7 ; b++ )
	{
		if( pWingItem->SheltomCode[b]==MagicalGreenEventCODE[0] )
		{
			MagicalGreenJadeCount = b+1;
		}
	}

	int TearOfKaraCount=0;
	int TearOfKaraFlag =0;
	// �庰 - ī���� ���� (7���� ���� �ٽ�üũ�Ѵ�.)
	for(int n = 0 ; n < 7 ; n++ )
	{
		if( pWingItem->SheltomCode[n]==TeatOfKaraEventCODE )
		{
			TearOfKaraCount = n+1;
		}
	}


	// �庰 - ������� ã�ƶ�
	int FindinvestigatorNineCount=0;
	int FindinvestigatorNineFlag =0;
	int FindinvestigatorTaleCount=0;
	int FindinvestigatorTaleFlag =0;
	
	for(int a = 0 ; a < 1 ; a++ )
	{
		if( pWingItem->SheltomCode[a]==FindinvestigatorEventCODE[0] )
		{
			FindinvestigatorNineCount = a+1;
		}
	}

	for(int b = 0 ; b < 1 ; b++ )
	{
		if( pWingItem->SheltomCode[b]==FindinvestigatorEventCODE[1] )
		{
			FindinvestigatorTaleCount = b+1;
		}
	}


	// ����� - ���ĺ� ���� �̺�Ʈ
	int PristonAlphabetCount = 0;
	int PristonAlphabetFlag = 0;
	for( int c = 0 ; c < 7 ; c++ )
	{
		for( int d = 0 ; d < 7 ; d++ )
		{
			if( pWingItem->SheltomCode[c]==PristonAlphabetEventCODE[d] )
			{
				PristonAlphabetCount = c+1;
			}
		}
	}

	//�ٺ������� 
	if( pWingItem->SheltomCode[0] == (sinGF1 | sin02) ){
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 5;
		pPresentWeaponName = PresentWeaponName; //������ ����
		pPresentArmorName = PresentArmorName;
		pPresentPercentIndex = PresentPercentIndex;
		PotionNum = 30;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;

		}
	}
	// ����ȣ������
	else if(pWingItem->SheltomCode[0] == (sinGF1 | sin03) &&  pWingItem->SheltomCode[1] == (sinGF1 | sin04)){
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 6;
		pPresentWeaponName = PresentWeaponName2;
		pPresentArmorName = PresentArmorName2;
		pPresentPercentIndex = PresentPercentIndex2;
		PotionNum = 20;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;

		}
	}

	// ������ ����
	else if(pWingItem->SheltomCode[0] == (sinGF1|sin05) ) { //&&  pWingItem->SheltomCode[1] == (sinGF1 | sin04)){
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 6;
		pPresentWeaponName = PresentWeaponName2;
		pPresentArmorName = PresentArmorName2;
		pPresentPercentIndex = PresentPercentIndex2;
		PotionNum = 20;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;

		}
	}
	//�ϰ����� ũ����Ż�� ���� ������ �ֱ�
	else if(CristalCount==7){
		CristalFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 5;
		//�ӽ÷�
		pPresentWeaponName   = PresentWeaponName3; //������ ����
		pPresentArmorName    = PresentArmorName3;
		pPresentPercentIndex = PresentPercentIndex3;
		PotionNum = 40;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}
	//����� - ���� ��ƿ���
	else if(WatermelonCount==7)
	{
		WatermelonFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 9; // ����� - ���� �̺�Ʈ 12���� �� �Ѱ��� �������� �����Ѵ�. // ȣ�� �̺�Ʈ 9���� �� �Ѱ����� �����Ѵ�. // ���ϴ��� �ҿ� �̺�Ʈ 9������ �Ѱ����� �����Ѵ�.
		//�ӽ÷�
		pPresentWeaponName   = PresentWeaponName3; //������ ����
		pPresentArmorName    = PresentArmorName3;
		pPresentPercentIndex = PresentPercentIndex3;
		PotionNum = 5; // ����� - ���� �̺�Ʈ ���� �������� ������ ��� ���� ������ �����Ѵ�. //����� - ���ϴ��� �ҿ� �̺�Ʈ(���� ���� 5��)
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}

	//�庰 - �߷�Ÿ�� ���ݸ� ��ƿ���
	else if(ValentineCount==7)
	{
		ValentineFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 17; // �庰 - �߷�Ÿ�� ���ݸ� �̺�Ʈ 17���� �� �Ѱ��� �������� �����Ѵ�. 
		//�ӽ÷�
		pPresentWeaponName   = PresentChocoWeaponName3; //������ ����
		pPresentArmorName    = PresentChocoArmorName3;
		pPresentPercentIndex = PresentChocoPercentIndex3;
		PotionNum = 5; // �庰 - �߷�Ÿ�� �̺�Ʈ ���� �������� ������ ��� ���� ������ �����Ѵ�.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}
	// ����� - ���ĺ� ���� �̺�Ʈ
	else if(PristonAlphabetCount==7){
		PristonAlphabetFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 11;
		//�ӽ÷�
		pPresentWeaponName   = PresentWeaponName5; //������ ����
		pPresentArmorName    = PresentArmorName5;
		pPresentPercentIndex = PresentPercentIndex5;
		PotionNum = 20;
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}


	// �庰 - ĵ������ ĵ�� ��ƿ���
	else if(CandydaysCount==7)
	{
		CandydaysFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 17; // �庰 - ĵ������ �̺�Ʈ 17���� �� �Ѱ��� �������� �����Ѵ�. 
		//�ӽ÷�
		pPresentWeaponName   = PresentCandyWeaponName3; //������ ����
		pPresentArmorName    = PresentCandyArmorName3;
		pPresentPercentIndex = PresentCandyPercentIndex3;
		PotionNum = 5; // �庰 - ĵ������ �̺�Ʈ ���� �������� ������ ��� ���� ������ �����Ѵ�.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}

	// �庰 - �����ñ׸� -  ���� ��ƿ���
	else if(MagicalGreenJadeCount==7)
	{
		MagicalGreenJadeFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 15; // �庰 - �����ñ׸� �̺�Ʈ 15���� �� �Ѱ��� �������� �����Ѵ�. 
		//�ӽ÷�
		pPresentWeaponName    = PresentMagicalGreenJadeWeaponName3; //������ ����
		pPresentArmorName	 = PresentMagicalGreedJadeArmorName3;
		pPresentPercentIndex = PresentMagicalGreedJadePercentIndex3;
		PotionNum = 10; // �庰 - �����ñ׸� �̺�Ʈ ���� �������� ������ ��� ���� ������ �����Ѵ�.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}


	// �庰 - �����ñ׸� -  ���޶��� ��ƿ���
	else if(MagicalGreenEmeraldCount==7)
	{
		MagicalGreenEmeraldFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 18; // �庰 - �����ñ׸� �̺�Ʈ 18���� �� �Ѱ��� �������� �����Ѵ�. 
		//�ӽ÷�
		pPresentWeaponName    = PresentMagicalGreenEmeraldWeaponName3; //������ ����
		pPresentArmorName	 = PresentMagicalGreenEmeraldArmorName3;
		pPresentPercentIndex = PresentMagicalGreenEmeraldPercentIndex3;
		PotionNum = 20; // �庰 - �����ñ׸� �̺�Ʈ ���� �������� ������ ��� ���� ������ �����Ѵ�.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}

	// �庰 - ī���� ����  ���� ��ƿ���
	else if(TearOfKaraCount==7)
	{
		TearOfKaraFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 18; // ī���� ���� �̺�Ʈ 18���� �� �Ѱ��� �������� �����Ѵ�. 
		//�ӽ÷�
		pPresentWeaponName   = PresentTearOfKaraWeaponName3; //������ ����
		pPresentArmorName    = PresentTearOfKaraArmorName3;
		pPresentPercentIndex = PresentTearOfKaraPercentIndex3;
		PotionNum = 10; // �庰 - ĵ������ �̺�Ʈ ���� �������� ������ ��� ���� ������ �����Ѵ�.
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}


	// �庰 - ������� ã�ƶ�( ���ξƹķ��� �����ϰ� ���� ������)
	else if( FindinvestigatorNineCount == 0 && FindinvestigatorTaleCount == 0 &&  pWingItem->DesCraftItem.CODE != 1000  )
	{
		for(i = 0 ; i < 5 ; i++)
		{
			

			if(sWingQuest[pWingItem->Index].ChangeItemCode)
			{
				if(sWingQuest[pWingItem->Index].NeedItemCode[i]  == pWingItem->SheltomCode[i]) break;
			}


			switch(pWingItem->SheltomCode[i])
			{
				case (sinQT1|sin09):
				case (sinQT1|sin10):
				case (sinQT1|sin11):
				case (sinQT1|sin06):
				case (sinQT1|sin12):
				break;
				default:
					FindinvestigatorNineFlag = 1;
					RandomTemp = GetRandomPos(0,100);
					ItemIndex = 1; // ������� ã�ƶ� ���ξƹķ� ����
					//�ӽ÷�
					pPresentWeaponName   = PresentFindinvestigatorNineWeaponName3; //������ ����
					pPresentArmorName    = PresentFindinvestigatorNineArmorName3;
					pPresentPercentIndex = PresentFindinvestigatorNinePercentIndex3;
					PotionNum = 10; 
					if(RandomTemp < 50){
						ItemCountPuzzle = 8;
					}
					else{
						ItemCountPuzzle = 16;
					}
					break;


			}

/*
		//	if( pWingItem->SheltomCode[i] == sinQT1|sin06 || pWingItem->SheltomCode[i] == sinQT1|sin09 || pWingItem->SheltomCode[i] == sinQT1|sin10 
		//			|| pWingItem->SheltomCode[i] == sinQT1|sin11 || pWingItem->SheltomCode[i] == sinQT1|sin12 ) break;
				

		//		if(sWingQuest[pWingItem->Index].NeedItemCode[i]  != pWingItem->SheltomCode[i])
		//		{
					FindinvestigatorNineFlag = 1;
					RandomTemp = GetRandomPos(0,100);
					ItemIndex = 1; // ������� ã�ƶ� ���ξƹķ� ����
					//�ӽ÷�
					pPresentWeaponName   = PresentFindinvestigatorNineWeaponName3; //������ ����
					pPresentArmorName    = PresentFindinvestigatorNineArmorName3;
					pPresentPercentIndex = PresentFindinvestigatorNinePercentIndex3;
					PotionNum = 10; 
					if(RandomTemp < 50){
						ItemCountPuzzle = 8;
					}
					else{
						ItemCountPuzzle = 16;
					}
					break;
	//			}
			*/
		}
	}

	// �庰 - ������� ã�ƶ�( ���ξƹķ��� �����ϰ� ������)
	else if( FindinvestigatorNineCount == 1 )
	{
		FindinvestigatorNineFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 1; // ������� ã�ƶ� ���Ͼƹķ� ����
		//�ӽ÷�
		pPresentWeaponName   = PresentFindinvestigatorNineTaleWeaponName3; //������ ����
		pPresentArmorName    = PresentFindinvestigatorNineTaleArmorName3;
		pPresentPercentIndex = PresentFindinvestigatorNineTalePercentIndex3;
		PotionNum = 10; 
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}

	// �庰 - ������� ã�ƶ�( ���Ͼƹķ��� �����ϰ� ������)
	else if( FindinvestigatorTaleCount == 1 ) 
	{
		FindinvestigatorTaleFlag = 1;
		RandomTemp = GetRandomPos(0,100);
		ItemIndex = 12; 
		//�ӽ÷�
		pPresentWeaponName   = PresentFindinvestigatorWeaponName3; //������ ����
		pPresentArmorName    = PresentFindinvestigatorArmorName3;
		pPresentPercentIndex = PresentFindinvestigatorPercentIndex3;
		PotionNum = 10; 
		if(RandomTemp < 50){
			ItemCountPuzzle = 8;
		}
		else{
			ItemCountPuzzle = 16;
		}
	}


	

	else{
		ItemIndex = 8; // ����� - �Ϻ� ���� �̺�Ʈ(8���� ������ �߿� �Ѱ����� �����Ѵ�)
		pPresentWeaponName = PresentWeaponName2;
		pPresentArmorName = PresentArmorName2;
		pPresentPercentIndex = PresentPercentIndex2;
		PotionNum = 10;

		//����
		for(i = 0 ; i < 8 ; i++){
			if((pWingItem->SheltomCode[i] & sinITEM_MASK2) == sinPZ1 ||
				(pWingItem->SheltomCode[i] & sinITEM_MASK2) == sinPZ2 ){
					for( j = 0 ; j < 8 ; j++){
						if(i == j)continue;
						if(pWingItem->SheltomCode[i] == pWingItem->SheltomCode[j]){
							pWingItem->Result = FALSE;
							return FALSE;

						}
					}
					if((pWingItem->SheltomCode[i] & sinITEM_MASK2) == sinPZ1)
						ItemCountPuzzle++;
					if((pWingItem->SheltomCode[i] & sinITEM_MASK2) == sinPZ2)
						ItemCountPuzzle +=2;

			}
		}

	}
	//���� ����
	if(ItemCountPuzzle == 8 || ItemCountPuzzle == 16){
		//��⼭ ������ �������� �����غ��� ��~
		PresentPercent = GetRandomPos(0,10000);
		for(i = 0 ; i < ItemIndex ; i++){
			if(PresentPercent >= PresentPercentSum && (pPresentPercentIndex[i]+PresentPercentSum) >=PresentPercent ){
				PresentItemCount = 0;
				//����
				if(ItemCountPuzzle == 8){
					for(k = 0 ; k < 9 ; k++){ // ����� - �Ϻ� ���� �̺�Ʈ(�� �����ۺ� Ȯ��)
						if(pPresentWeaponName[i][k]){
							PresentItemCount++;
						}
					}
					PresentItemIndex = GetRandomPos(0,PresentItemCount-1);
					while(1){
						if(pPresentWeaponName[i][PresentItemIndex]){
							for(j=0;j<MAX_ITEM;j++) {
								if ( lstrcmpi( pPresentWeaponName[i][PresentItemIndex] , sItem[j].LastCategory )==0 ) {
									pWingItem->DesCraftItem.sItemInfo.CODE = sItem[j].CODE;
									if((pWingItem->DesCraftItem.sItemInfo.CODE & sinITEM_MASK2) == sinPM1) 
										pWingItem->DesCraftItem.sItemInfo.PotionCount = PotionNum;
									if(CristalFlag)pWingItem->Result = SIN_WINGRESULT_CRISTAL;
									else pWingItem->Result = TRUE;
									return TRUE;
								}
							}
						}
						PresentItemIndex--;
						if(PresentItemIndex < 0 )break; //���ѷ��� ����
					}
				}
				//����
				PresentItemCount = 0;
				if(ItemCountPuzzle == 16){
					for(k = 0 ; k < 9 ; k++){
						if(pPresentArmorName[i][k]){
							PresentItemCount++;
						}
					}
					PresentItemIndex = GetRandomPos(0,PresentItemCount-1);
					while(1){
						if(pPresentArmorName[i][PresentItemIndex]){
							for(j=0;j<MAX_ITEM;j++) {
								if ( lstrcmpi( pPresentArmorName[i][PresentItemIndex] , sItem[j].LastCategory )==0 ) {
									pWingItem->DesCraftItem.sItemInfo.CODE = sItem[j].CODE;
									if((pWingItem->DesCraftItem.sItemInfo.CODE & sinITEM_MASK2) == sinPM1) 
										pWingItem->DesCraftItem.sItemInfo.PotionCount = PotionNum;
									if(CristalFlag)pWingItem->Result = SIN_WINGRESULT_CRISTAL;
									else pWingItem->Result = TRUE;
									return TRUE;
								}
							}
						}
						PresentItemIndex--;
						if(PresentItemIndex < 0 )break; //���ѷ��� ����
					}
				}
			}
			else PresentPercentSum += pPresentPercentIndex[i];
		}
	}
	
	//80_2 Lv����Ʈ
	for(i = 0 ; i < 3 ; i++){
		switch(pWingItem->SheltomCode[i]){
			case (sinQT1|sin09):
				ItemCount80_2[0] = 1;
			break;
			case (sinQT1|sin10):
				ItemCount80_2[1] = 1;
			break;
			case (sinQT1|sin11):
				ItemCount80_2[2] = 1;
			break;

		}
	}
	if(ItemCount80_2[0] && ItemCount80_2[1] && ItemCount80_2[2]){
		pWingItem->Result = TRUE;
		pWingItem->DesCraftItem.sItemInfo.CODE = (sinQT1|sin12);
		return TRUE;

	}
	for(i = 0 ; i < 3 ; i++){
		if(pWingItem->SheltomCode[i] == (sinQT1|sin06)){
			ItemCount++;
		}
	}
	if(ItemCount == 3 && pChar_Info){
		pWingItem->Result = TRUE;
		switch(pChar_Info->JOB_CODE){
			case 1:		//������
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWA1 | sin08);
			break;
			case 2:		//��ī�ϼ�
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWC1 | sin08);
			break;
			case 3:		//����
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWS1 | sin10);
			break;
			case 4:		//����ũ��
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWP1 | sin09);
			break;
			case 5:		//��Ż��Ÿ
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWT1 | sin09);
			break;
			case 6:		//����Ʈ
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWS2 | sin10);
			break;
			case 7:		//������
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWM1 | sin09);
			break;
			case 8:		//������Ƽ��
				pWingItem->DesCraftItem.sItemInfo.CODE = (sinWM1 | sin09);
			break;
		}
		//�������� �������ش�
		pWingItem->DesCraftItem.sItemInfo.ItemKindCode = ITEM_KIND_QUEST_WEAPON;
		//SetChangeJob3QuestItem(&pWingItem->DesCraftItem.sItemInfo); //������ �ʱ����
		return TRUE;

	}
	//���� ����
	if(pWingItem->DesCraftItem.CODE == 1000){
		for( i = 0 ; i < 12 ; i++){
			for(j = 0 ; j < 14 ; j++){ // ����� - ����, ������ ���� �߰�(12 -> 14))
				if((pWingItem->SheltomCode[i] & sinITEM_MASK3) == SheltomCode2[j]){
					pWingItem->Head[i] = 0;
					pWingItem->CheckSum[i] = 0;
					pWingItem->SheltomCode[i] = sinForceOrbCode[j];
					break;
				}
				// ����� - ���� ���� �߰�
				else if((pWingItem->SheltomCode[i] & sinITEM_MASK3) == MagicSheltomCode[j]){
					pWingItem->Head[i] = 0;
					pWingItem->CheckSum[i] = 0;
					pWingItem->SheltomCode[i] = sinMagincForceOrbCode[j];
					break;
				}
			}
		}
		pWingItem->Result = TRUE;
		return 0x100;
	}
	else{
		for(i = 0 ; i < 5 ; i++){
			if(sWingQuest[pWingItem->Index].ChangeItemCode){
				CodeCount++;
				if(sWingQuest[pWingItem->Index].NeedItemCode[i]  == pWingItem->SheltomCode[i]){
					ItemCount++;
				}
			}
		}
		if(CodeCount == ItemCount){
			pWingItem->DesCraftItem.sItemInfo.CODE = sWingQuest[pWingItem->Index].ChangeItemCode;
			pWingItem->DesCraftItem.sItemInfo.ItemKindCode = 0;
			pWingItem->Result = TRUE; 
			return TRUE;

		}
	}
	pWingItem->Result = FALSE;
	return FALSE;
}

//������ ���������� ������
int SendWingQuestItem() //�������� ����Ʈ 
{
	SendWingItemToServer( &sWingItem_Send );   
	return TRUE;
}

//�������� ���������� �޴´�
int RecvWingItem(sCRAFTITEM_SERVER *pWingItem)
{
 
	sITEM *pItem=0; 
	int   Index = 0; 
	int   i = 0; 
	sITEM sinTempItem;  //�����ۿ� ���õ� ������ ��� �־�δ� Temp

	//if(pWingItem->Result != TRUE)return FALSE;

	//�����ϰ��� �����ϳ� -0-

	for( i = 0; i < INVENTORY_MAXITEM*2 ; i++){
		if(i<100){
			if(!cInvenTory.InvenItem[i].Flag)continue;
			pItem = &cInvenTory.InvenItem[i];
		}
		else{
			if(!cInvenTory.InvenItemTemp[i-100].Flag)continue;
			pItem = &cInvenTory.InvenItemTemp[i-100];
		}

		//����� FALSE�ϰ�쿡�� �� �����۵� �����Ѵ�
		if(pWingItem->Result == FALSE){
			cInvenTory.DeleteInvenItemToServer(pWingItem->DesCraftItem.sItemInfo.CODE , pWingItem->DesCraftItem.sItemInfo.ItemHeader.Head ,
			 pWingItem->DesCraftItem.sItemInfo.ItemHeader.dwChkSum);

		}

		//�����ڵ尡 ���� ���ڵ� ����� üũ�����ʴ´� 
		if(pItem->ItemPosition == 2){  
			if(pItem->Class == ITEM_CLASS_WEAPON_TWO)
				continue; 
		}

		for(int j = 0 ; j < 8 ; j++){
			if(pItem->sItemInfo.CODE == pWingItem->SheltomCode[j] &&
				pItem->sItemInfo.ItemHeader.Head == pWingItem->Head[j] &&
				pItem->sItemInfo.ItemHeader.dwChkSum == pWingItem->CheckSum[j]){
					pItem->Flag = 0; //�������� �����ش�
					cInvenTory.ReFormInvenItem();
					cInvenTory.CheckWeight();
				}
				
		}
	}	
	
	//�����Ͽ�����쿡 �������� �������� �����Ѵ�
	if(pWingItem->Result == TRUE){
		sinMinusMoney(pWingItem->Money);//���� ����
		SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

		//�������� �κ��丮�� �����Ѵ�
		if(pWingItem->DesCraftItem.sItemInfo.CODE){
			if(LoadItemImage(&pWingItem->DesCraftItem.sItemInfo,&sinTempItem)){
				//����Ʈ �������� �ް� ������ ����
				if(sinTempItem.sItemInfo.ItemKindCode ==ITEM_KIND_QUEST_WEAPON){
					sinQuest_ChangeJob3.StartLevel = sinChar->Level;

				}
				sinSetSpecialItemCode(&sinTempItem.sItemInfo); //����Ⱦ������� �����Ѵ�
				cInvenTory.AutoSetInvenItem(&sinTempItem,1+pWingItem->DocIndex); //�ε����� 1��������� ��ǥ�� �����Ѵ�
				cInvenTory.ReFormInvenItem();
				ResetPotion();
				cInvenTory.ReFormPotionNum();	//���� ������ üũ�Ѵ�
			}
		}
	}

	ReformCharForm();//������ 
	cInvenTory.SetItemToChar();
	cInvenTory.CheckWeight();
	
	sinbaram_Stop = 0; //�̺�Ʈ�� �����۵��ǰ��Ѵ�
	cInvenTory.CheckPuzzle();

	return TRUE;	
}

//���� ����¡ �Ѵ�
int AgingRing()
{
	if(sInven[4].ItemIndex){
		if(cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[1] && cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.CODE == (sinOR2|sin01)){
			if(smConfig.DebugMode){
				cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[0] +=100;
			}
			else
				cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[0]++;
			if(cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[0] >= 
				cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo.ItemAgingCount[1]){
					SendAgingUpgradeItemToServer(&cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo);

				}
		}
	}
	if(sInven[5].ItemIndex){
		if( cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[1] &&cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.CODE == (sinOR2|sin01)){
			if(smConfig.DebugMode){
				cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[0] +=100;
			}
			else
				cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[0]++;
			if(cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[0] >= 
				cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo.ItemAgingCount[1]){
					SendAgingUpgradeItemToServer(&cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo);

				}

		}
	}

	return TRUE;
}

//���ڿ� �޸����� �ѱ��
int NumLineComa(int Num , char *pBuff)
{

	int i = 0;
	char strBuff[128];
	wsprintf(strBuff,"%d",Num);   
	int cnt = 0;
	int cnt2 = 0;
	int cnt3 = 0;

	int len = lstrlen(strBuff);
	while(1){
		if(strBuff[cnt]){ 
			pBuff[cnt+cnt2] = strBuff[cnt];
			cnt3 = (len-1)-cnt;
			if((cnt3 % 3) ==0 && cnt3 != 0){
				cnt2++;
				pBuff[cnt+cnt2] = ',';
			}
			cnt++;

		}
		else break;
	}

	return TRUE;
}

//������ ������ ��� ������ �����Ѵ� (����μ�)
int sinSaveGame(DWORD CODE , int Count)
{
	DWORD MASK_CODE = (CODE & sinITEM_MASK2);

	//����ϰ�� 
	if(MASK_CODE == sinPM1 && Count >= 4 ){
		SaveGameData();

	}
	if((  MASK_CODE == sinPL1 || MASK_CODE == sinPS1 ) && Count >= 10){
		SaveGameData();
	}
	return TRUE;
}

//��¥�� ������������ ������� �����ش�
int OverDay_Item_Flag = 0;
int OverDay_Item_Delete(sITEM *pItem)
{

	OverDay_Item_Flag = 1;
	lstrcpy(sLost_Item.szDoc,pItem->sItemInfo.ItemName);
	sLost_Item.LostItem[0].CODE = pItem->sItemInfo.CODE;
	sLost_Item.LostItem[1].CODE = 0;
	sLost_Item.Flag = 1;
	cInvenTory.CheckInvenEmpty(&sLost_Item.LostItem[0]); //�̹����ε� 
	cInvenTory.CheckInvenEmpty(&sLost_Item.LostItem[1]); //�̹����ε� 
	return TRUE;
}


//��� �������̽��� �ݴ´�
int sinCloseInterFace()
{
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
	if(cInvenTory.OpenFlag) cInvenTory.OpenFlag = SIN_CLOSE; //�κ��丮�� ���������� �����ش� 		 
	if(cCharShop.OpenFlag) cCharShop.OpenFlag = SIN_CLOSE; //������ ���������� �����ش� 		 
	if(cCharShop.OpenFlag){
		cCharShop.OpenFlag = 0; //������ �ݴ´�
		memset(cCharShop.CharShopItem,0,sizeof(sITEM)*30); //�������� �����ش�
	}
	cInvenTory.ChangeSpecialItem(2); //����Ʈ�������� �����Ѵ�
	return TRUE;
}

int sinCheck_ShowHelpMap()
{
	//if(cWareHouse.OpenFlag || MouseItem.Flag ||cShop.OpenFlag || ExitButtonClick ||){ //  pluto ���� , ����
	if(SkillNpcFlag || sinMessageBoxShowFlag || MouseItem.Flag || cWareHouse.OpenFlag || cTrade.OpenFlag || cCraftItem.OpenFlag || cAging.OpenFlag || cMyShop.OpenFlag||
		cCharShop.OpenFlag || ExitButtonClick || MyShopSendButton || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag || ReStartFlag ){	// pluto �׾����� ���� �� ���� �Ϸ��� ReStartFlag // ������ - �ͽ��� ���� â �߰�
		return FALSE;

	}
	if( g_IsDxProjectZoomIn <= 0 )
      SetFullZoomMap(1);
	return TRUE;
}

//���� �������� �޴´�
int sinRecvForceOrb(sITEMINFO *pItemInfo , int Count)
{

	int i = 0;
	sinMinusMoney(sCraftItem_Send.Money,1);//���� ����
	SendSaveMoney(); //�ݾ� ������ ���ϰ��ϱ����� ȣ���Ѵ� 

	SpecialItemShowFlag = 0; //������ �ݾ��ش�
	sITEM sinTempItem;  //�����ۿ� ���õ� ������ ��� �־�δ� Temp

	//�ϴ�����=================================================================

	//===========================================================================

	for(i = 0 ; i < Count ; i++){
		if(LoadItemImage(&pItemInfo[i],&sinTempItem)){ 
			if(!cInvenTory.AutoSetInvenItem(&sinTempItem)){     //������ ���������� 
				if(sinThrowItemToFeild(&sinTempItem)){          //�������� ������ 
					sinTempItem.Flag = 0;
				}
			}
			cMessageBox.ShowMessage(MESSAGE_FORCEORB_REFINING);
		}
	}
	memset(&sCraftItem,0,sizeof(sCRAFTITEM)); //�������� �� ���������� 
	memset(&sCraftItem_Recv,0,sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ  (�ϴ� �������ش�)
	memset(&sCraftItem_Send,0,sizeof(sCRAFTITEM_SERVER)); //���� �ͽ��� ����ü �ʱ�ȭ 
	
	cInvenTory.SetItemToChar(); //�������� ���õǸ� �ɷ�ġ�� �����Ѵ� 
	cInvenTory.ReFormInvenItem();
	cInvenTory.CheckWeight();   //���Ը� �����Ѵ� 
	ReformCharForm();//������ 
	sinbaram_Stop = 0; //�̺�Ʈ�� �����۵��ǰ��Ѵ�
	MixItemNoCopyFlag = 0; //������� �÷� �ʱ�ȭ
	cCraftItem.OpenFlag = 0 ; //â�� �ݴ´� 

	//Force Orb �ʱ�ȭ
	cCraftItem.ForceFlag = 0; 
	cCraftItem.ForceItemPrice = 0;
	ForceItemPrice2 = 0;

	return TRUE;
}

DWORD SummonSkillCode[] = {
	SKILL_METAL_GOLEM,
	SKILL_FIRE_ELEMENTAL,
	SKILL_RECALL_WOLVERIN,
	0,

};
int SummonSkillCodeIndex[10] = {0,};

//����Ŭ���̺�Ʈ
int sinDoubleClick()
{
	//��ȯ���� ���¸� �ٲ۴�
	int Cnt = 0;
	while(1){
		if(SummonSkillCode[Cnt] == 0)break; 
		if(SummonSkillCode[Cnt] == TempContinueSkill[ContinueSkillMouseIndex-1].CODE){
			SummonSkillCodeIndex[Cnt] ^= 1;
			SendUpdateSkillToServer(TempContinueSkill[ContinueSkillMouseIndex-1].CODE,SummonSkillCodeIndex[Cnt] ,
				0,0); 
			break;
		}
		Cnt++;
	}
	return TRUE;
}

