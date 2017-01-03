/*----------------------------------------------------------------------------*
*	���ϸ� :  sinShop.h	
*	�ϴ��� :  ������ �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#define     MAX_SHOP_CLASS_ITEM	30  //�� Ŭ������ 30��

#define     SHOP_ROW			9  //�ӽ� ���� ������ ���� 
#define     SHOP_COL			9
	
#define 	SINALLREPAIR		1 
#define 	SINREPAIR			2    
#define 	SINSELL				3      
#define 	SINBUY				4       
#define 	SINRIGHTARROW		5
#define 	SINLEFTARROW		6  
#define     SINSHOPEXIT			7

#define 	SIN_WEAPON			1 
#define 	SIN_DEFENSE			2 
#define 	SIN_ETC				3 


struct	sMYSHOP_ITEM{
	sITEMINFO	sItem;
	int			Price;
	short       Posi[2];
	DWORD		dwTemp[3];
};

struct sMYSHOP{
	DWORD			CHAR_CODE;
	DWORD			dwTemp[6];
	sMYSHOP_ITEM	sMyShopItem[30]; //������ �޴� ������
};

struct sMYSHOP_ITEM_SERVER{ //���λ������� ������ ������ 
	DWORD CODE;
	DWORD CkSum;
	DWORD Head;
	DWORD sTime;
	DWORD Price;
	DWORD SendFlag;
	DWORD Buyer;

	DWORD Temp[4];

};

class cSHOP{


public:

	int OpenFlag;	//���� �κ��丮�� �����ֳ� �ƴѰ��� ���� �÷� 
	int UseItemFlag; // ����� - �̵� ���� ������

	int PosX;
	int PosY;

	int MatShop[10];

	int SelectShop; // 1���� ,2 �� ,3 �׿� �͵� 

	int ShopIconPosi;

	
	sITEM ShowShopItem[30];	//�ѹ��� ������ ������ �ִ� 30
	sITEM ShopItem[60]; //����� ������ 
	

	int RevWeaponCnt;
	int RevDefenseCnt;
	int RevEtcCnt;
	int MatMyShop_Button;
	
	DIRECTDRAWSURFACE lpGoldEdit;

	DIRECTDRAWSURFACE lpAllRepair;
	DIRECTDRAWSURFACE lpRepair;

	DIRECTDRAWSURFACE lpSell;
	DIRECTDRAWSURFACE lpBuy;

	DIRECTDRAWSURFACE lpAllRepair_D;
	DIRECTDRAWSURFACE lpRepair_D;

	DIRECTDRAWSURFACE lpSell_D;
	DIRECTDRAWSURFACE lpBuy_D;

	DIRECTDRAWSURFACE lpRightArrow;
	DIRECTDRAWSURFACE lpLeftArrow;

	
	DIRECTDRAWSURFACE lpTabAttack;
	DIRECTDRAWSURFACE lpTabDefense;
	DIRECTDRAWSURFACE lpTabEtc;

	DIRECTDRAWSURFACE lpTabAttack_D;
	DIRECTDRAWSURFACE lpTabDefense_D;
	DIRECTDRAWSURFACE lpTabEtc_D;

	DIRECTDRAWSURFACE lpExit;
	DIRECTDRAWSURFACE lpGrid;

	DIRECTDRAWSURFACE lpRepairAllInfo;	
	DIRECTDRAWSURFACE lpRepairInfo;
	DIRECTDRAWSURFACE lpDefaultRepairAll;	
	DIRECTDRAWSURFACE lpDefaultRepair;

	DIRECTDRAWSURFACE lpSellInfo;
	DIRECTDRAWSURFACE lpBuyInfo;


	DIRECTDRAWSURFACE lpTitle_Attack;
	DIRECTDRAWSURFACE lpTitle_Etc;
	DIRECTDRAWSURFACE lpTitle_Trade;
	DIRECTDRAWSURFACE lpTitle_WareHouse;

	DIRECTDRAWSURFACE lpWeightEdit;

	DIRECTDRAWSURFACE lpNoUseBox;

	DIRECTDRAWSURFACE lpCraftItemMain;
	DIRECTDRAWSURFACE lpCraftItemButton;
	DIRECTDRAWSURFACE lpCraftItemButtonInfo;
	DIRECTDRAWSURFACE lpTitle_CraftItem;

	// pluto ����
	DIRECTDRAWSURFACE lpSmeltingItemMain;
	DIRECTDRAWSURFACE lpTitle_SmeltingItem;
	DIRECTDRAWSURFACE lpSmeltingItemButtonInfo;

	// pluto ����
	DIRECTDRAWSURFACE lpManufactureItemMain;
	DIRECTDRAWSURFACE lpTitle_ManufactureItem;
	DIRECTDRAWSURFACE lpManufactureItemButtonInfo;

	// ������ - �ͽ��� ���¿� �̹��� �ε� �߰�
	DIRECTDRAWSURFACE lpMResetTitle;
	DIRECTDRAWSURFACE lpMResetMain;
	DIRECTDRAWSURFACE lpMResetButton;
	DIRECTDRAWSURFACE lpMResetButtonInfo;
	
	DIRECTDRAWSURFACE lpTitle_Aging;
	DIRECTDRAWSURFACE lpAging_Info;
	

	DIRECTDRAWSURFACE	lpShopMain;
	DIRECTDRAWSURFACE	lpShopMain2;


	//���� ����
	DIRECTDRAWSURFACE	lpMyShopExp;
	DIRECTDRAWSURFACE	lpMyShopExp_;
	DIRECTDRAWSURFACE	lpMyShopExp_T;
	DIRECTDRAWSURFACE	lpMyShopNoSale;
	DIRECTDRAWSURFACE	lpMyShopNoSale_;
	DIRECTDRAWSURFACE	lpMyShopNoSale_T;
	DIRECTDRAWSURFACE	lpMyShopSale;
	DIRECTDRAWSURFACE	lpMyShopSale_;
	DIRECTDRAWSURFACE	lpMyShopSale_T;
	DIRECTDRAWSURFACE    lpTitle_MyShop;
	DIRECTDRAWSURFACE    lpImage_MyShop;
	DIRECTDRAWSURFACE    lpMyShop_T;
	

	
public:
	cSHOP();
	~cSHOP();

	void Init(); //Ŭ���� �ʱ�ȭ
	void Load();
	void Release();
	void Draw();
	void Close();//Ŭ���� ���� 
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); 

	void CheckShopNpcState();  //NPC�� ������� üũ�Ѵ� 

	//void CopyShopItemToShow(int Index);//�������� �޾ƿ� �������� ������ ���������� �����Ѵ� 
	void CopyShopItemToShow(int Index,int Kind=0);

	int GetShopItemXY(sITEM *pItem); //�������� ��ǥ�� ���Ѵ� 

	void DrawShopText(); //���� �ؽ�Ʈ�� ǥ���Ѵ� 

	////////////////////�ƾ��� ��� �ȱ� 
	int RecvBuyItemToServer(sITEM *pItem ,int ItemCount=1); //�������� ��� 
	int SellItemToShop(sITEM *pItem,int ItemCount=1); //�ƾ����� �Ǵ� 

	int SendBuyItemToServer(sITEM *pItem , int ItemCount=1 ); //���������������� ��´� 

	void DeleteShopItem(); //������ ������ ������������ �ʱ�ȭ�Ѵ�

	int CheckHighRankItem(sITEM *pItem); //���������� üũ

	//���� �������� ����ִ� �� üũ�Ѵ�.
	int haBuyMoneyCheck(int BuyMoney);
	//���� ������ ��������� ������ ǥ�����ش�.
	int haShopItemPrice(int Money);
};

/*----------------------------------------------------------------------------*
*								���λ��� ��� Ŭ���� 
*-----------------------------------------------------------------------------*/
class cMYSHOP{
public:
	int OpenFlag;	
	sITEM MyShopItem[30];

	char szDoc[128];     //ȫ������
	char szSendDoc[128]; //������ ���� ȫ������

public:
	
	//�������� ���õɰ����� ã�´�
	int SetMyShopItem(sITEM *pItem);

	//�����Ѵ�
	int LastSetMyShopItem(sITEM *pItem);

	//�κ��������� ����Ѵ�
	int SetShopItemToInven(sITEM *pItem);

	//������ ����� �������� ã�´�
	int SearchShopItemToInven(sITEM *pItem);

	//�������� ������ ������ 
	int SendMyShopItem(); 

	//�������� ���ſ�û�� ���� �������� �޴´�
	int RecvMyShopItem( DWORD dwCharCode , sMYSHOP_ITEM_SERVER *lpShopItem );

	//�ڽ��� ������ �ִ� �ݾ��� �����Ѵ�
	int GetLimitMoney(int Money);

	//������ ���ȸ��� ������ �ݴ´�
	int AutoCloseShop();

	//���λ����� ���� ���ؿ´�
	int GetTotalMyShopItemMoney();

};
/*----------------------------------------------------------------------------*
*								���λ��� Ŭ���� 
*-----------------------------------------------------------------------------*/
class cCHARSHOP{
public:
	int OpenFlag;	

public:
	sITEM CharShopItem[30];
	
	//�������� �޴´�
	int RecvShopItem(sMYSHOP *sMyShop);

	//���� �������� ã�´�
	int SearchMyShopItem(DWORD CODE ,DWORD Head , DWORD CheckSum);

	//���λ��������� �������� �κ��丮�� �����Ѵ�
	int SetCharShopItemToInven(sITEMINFO *pItem_Info);

	//���λ������� �������� ���
	int BuyItem(int Index);

};

extern cSHOP cShop;

extern int sinShopKind;  //���� ������ ����  1  ���� 2 ��� 3 ���� (defalt �� ����)

extern int TalkNpcState; //���� ���� NPC�� �Բ� ������� 

extern int ShopGoldEdit[2][4];

extern int ShopArrowPosi; //���� ȭ��ǥ ���� 

extern int BuyItemServerFlag; //�������� ������ �춧 �޼����� ���������� ������� 

extern cMYSHOP cMyShop;
extern cCHARSHOP cCharShop;

extern sMYSHOP     sMyShop;       //������ MyShop ����ü
extern sMYSHOP     sRecvMyShop;   //�޴� MyShop ����ü
extern int MyShopSendButton;

extern int MyShopExpBox;
extern int MyShopItemSellMoney2;
extern sMYSHOP_ITEM_SERVER sMyShop_Server;
extern sITEMINFO MyShopPotion;
extern int MyShopItemCancelIndex[2];
