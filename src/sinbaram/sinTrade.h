/*----------------------------------------------------------------------------*
*	���ϸ� :  sinTrade.h	
*	�ϴ��� :  ������ ��ȯ 
*	�ۼ��� :  ����������Ʈ 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#define MAX_TRADE_ITEM			20
#define MAX_CRAFTITEM_INFO		300
#define MAX_AGINGITEM_INFO		100
#define MAX_MAKEITEM_INFO		100

#define T_SET_BOX				0
#define T_MONEY_POSI			1
#define T_MONEY_TEXT_POSI		2
#define T_WEIGHT_TEXT_POSI		3
#define T_CHECK_POSI			4
#define T_CLOSE_POSI			5
// ����¡�� ���� ������ //�����߰�
#define MAX_SHELTOM				14	//����¡�� �ʿ��� ������ �ִ�ġ // ����� - ����¡ ���� ����(19��, 20�� �߰�)
#define MAX_AGING				20	//����¡�� ������ ���� // ����� - ����¡ ���� ����(19��, 20�� �߰�)
#define MAX_RUNE				14	// pluto ����, ���� �� // �庰 - ���� �߰� // �庰 - ���������� ȥ
#define MAX_RECIPE_KIND			11	// pluto ������ ���� // �庰 - ���� // �庰 - ���������� ȥ( 10 -> 11 )
#define MAX_RECIPE_ITEM			60	// pluto ������ �ִ�ġ
#define MAX_HANDMADEITEM		176 // �庰 - ���������� ȥ (���ۼ� �߰��� +16)

#define MAX_SEEL_COUNT			4	// ����� - ���̿��� �� �߰�(����, ���̵�, ��Ǫ, ���̿���)

/*----------------------------------------------------------------------------*
*								Ʈ���̵�  ����ü 
*-----------------------------------------------------------------------------*/
struct sTRADE{
	sITEM TradeItem[MAX_TRADE_ITEM]; //Ʈ���̵�� ������ 
	int Money;			//Ʈ���̵�� �ݾ� 
	int Weight;			//������ ���� 
	int CheckFlag;		//Ȯ�� �÷� 
	int CheckMoney;
};

///////â�� ����ü 
struct sWAREHOUSE{
	sITEM WareHouseItem[100]; 
	int Money;
	short Weight[2];
	int BuyAreaCount; //â�� �󸶸�ŭ ��ԵǾ��� 
	int FakeMoney;


};

//////������ ���� ����ü 
struct sCRAFTITEM{
	sITEM CraftItem[15];	//���յ� ������ 
	int   SheltomIndex[12];

};

/////������ ������ ������ ����ü 
struct sCRAFTITEM_SERVER{
	int size, code;
	int DocIndex;
	int Result;
	sITEM DesCraftItem;
	DWORD SheltomCode[12];
	DWORD Head[12];
	DWORD CheckSum[12];
	int  Index;
	int  Money;

	DWORD A_StoneItemCode;
	DWORD A_StoneHead;
	DWORD A_StoneCheckSum;
};

///// ũ����Ʈ ������ 
struct sCRAFTITEM_INFO{
	DWORD MainItemCode[8];	//������ ������ �ڵ� 
	int AddItem[9];			//�߰��� ������ (����)
	int PlusElementCode[8];	//�߰��� ���� 
	float AddElement[8];	//�߰��� ��ġ 
	int ElementKind[8];		//��ġ�� ���� (+ % �Ҽ�)
	char Doc[256];			//���� 

};

// ������ - �ͽ��� ���� ������ ����ü
struct sMIXTURE_RESET_ITEM
{
	sITEM	MixtureResetItem[2];	// ���� ��ų ������(0�� ������, 1�� ����)
};

// ������ - �ͽ��� ���� ������ ����ü (������)
struct sMIXTURE_RESET_ITEM_SERVER
{
	int		size, code;
	int		DocIndex;
	int		Result;
	int		Index;

	sITEM	DesMixtureResetItem;	// ���� �� ������

	DWORD	dwMixtureResetStoneItemCode;	// �ͽ��� ���� �����
	DWORD	dwMixtureResetStoneHead;		// �ͽ��� ���� �����
	DWORD	dwMixtureResetStoneCheckSum;	// �ͽ��� ���� �����
};

///// ����¡ 
struct sAGINGITEM{
	sITEM AgingItem[15];	//���յ� ������ 
	int   SheltomIndex[12];	//���ҵ� ������ �ε��� 
	int   AgingOkFlag;	    //������ �ɼ��������� �÷� 

};

struct sAGINGITEM_INFO{
	DWORD MainItemCode;		//������ ������ �ڵ� 
	int PlusElementCode[8];	//�߰��� ���� 
	float AddElement[8];	//�߰��� ��ġ 
	int ElementKind[8];		//��ġ�� ���� (+ % �Ҽ�)
	char Doc[128];			//���� 

};

///// MakeItem
struct sMAKEITEM_INFO{
	DWORD CODE[15];			//���յǴ� �������� �ڵ�
	DWORD Result_CODE;		//���յ� ������ �ڵ� 

};

struct sPOSTBOX_ITEM{
	char	szName[64];
	DWORD	CODE;
};

//������ �籸�� ����ü
struct sRECON_ITEM{
	DWORD  dwCODE;    //..
	int	   iLevel[2]; //min~max
	int	   iCount;    //�ʿ� ����
	int    iPrice;    //����?
};

// pluto ����
struct SSmeltingItem
{
	sITEM SmeltingItem[5];	//���յ� ������ 
	int   SheltomIndex[12];
};

// pluto ����
struct sSMELTINGITEM_SERVER
{
	int size, code;
	int DocIndex;
	int Result;
	sITEM DesSmeltingItem;
	DWORD SmeltingCode[5];
	DWORD Head[5];
	DWORD CheckSum[5];
	int  Index;
	int  Money;

	DWORD A_StoneItemCode;
	DWORD A_StoneHead;
	DWORD A_StoneCheckSum;
};

// pluto ����
struct SManufactureItem
{
	sITEM	ManufactureItem[4];
	int		RuneIndex[12];
};

// pluto ����
struct SManufactureItem_Server
{
	int size, code;
	int DocIndex;
	int Result;
	sITEM DesManufactureItem;
	DWORD RuneCode[4];
	DWORD Head[4];
	DWORD CheckSum[4];
	int  Index;
	int  Money;

	DWORD A_StoneItemCode;
	DWORD A_StoneHead;
	DWORD A_StoneCheckSum;
};

// pluto ����
struct SManufacture_Rune
{
	int RecipeCode;
	int NeedRuneIndex[3];
};

// pluto ���� �� ���� ���� ������ �ش� ������ �ڵ� �߻�
struct SManufacture_CreateItemCode
{
	DWORD RecipeKind[2];
	DWORD CheckRuneCode[8][3];
	DWORD CreateItemCode[2][8];
};

// pluto ���� ������ ����, ���� ����
struct SManufacture_ResultItemInfo
{
	DWORD ItemCode;
	int	Weight;
	int Price;
	char ItemName[32];
};

// pluto ���� ������ ����, ���� ����  // �庰 - ���� 160(144) // �庰 - ���������� ȥ
struct SManufacture_ResultItemInfo_Server
{
	smTRANS_COMMAND		smTransCommand;
	DWORD ItemCode[MAX_HANDMADEITEM];
	int Weight[MAX_HANDMADEITEM];
	int Price[MAX_HANDMADEITEM];
	char ItemName[MAX_HANDMADEITEM][32];
};
/*----------------------------------------------------------------------------*
*							����¡  ������ Ŭ���� 
*-----------------------------------------------------------------------------*/
class cAGING{
public:

	int OpenFlag;
	int AgingItemCheckSum;
	DIRECTDRAWSURFACE	lpGraySheltom[MAX_SHELTOM];

public:
	//����¡ �� ���Ŵ� ����   //1.����¡���� 
	int  AginStoneKind;         //2.���� ����
								//����� - 3.���� ���� ����
								//����� - 4.���� ����¡ ����
                                //���� �߰���
	
public:

	int  AgingItem(int MakeItemFlag =0);		//�������� ����¡�Ѵ� 
	int  CancelAgingItem(int Flag=0);	//����¡ �������� ĵ���Ѵ� 
	int  CheckItem(sITEM *pItem);		//����¡�� �������� üũ�Ѵ� 
	int  CheckAgingItemOk(); //����¡�� �Ҽ��ִ����� üũ�Ѵ� 

	int  SetAgingItemAreaCheck(sITEM *pItem );	//����¡ �������� ���õ� ������ üũ�Ѵ� 
	
	///////////// ����¡ �������� ���ۿ��θ� ������ 
	int  CheckAgingData();

	/////////// �������� �������� ����¡�Ѵ�
	int sinSendAgingItem();
	int sinRecvAgingItem(sCRAFTITEM_SERVER *pCraftItem_Server);
	int sinRecvAgingItemResult(sCRAFTITEM_SERVER *pCraftItem_Server);

};

/*----------------------------------------------------------------------------*
*						������ - �ͽ��� ���� Ŭ���� 
*-----------------------------------------------------------------------------*/
class cMIXTURERESET
{
public:

	int	OpenFlag;					// �����ִ°�?
	int	MixtureResetItemCheckSum;	// �˻��
	int	HaveMixtureResetStone;		// �ͽ��� ���� ������ �ִ°�?

public:

	int	SetMixtureItemResetAreaCheck(sITEM *pItem);		// �ͽ��� ���µ� �������� ������ üũ�Ѵ�.
	int PickUpMixtureResetItem( int x, int y, int PickUpFlag = 0, int Kind = 0 );	// �ͽ��� ���� �������� �����Ѵ�.
	int LastSetMixtureResetItem( sITEM *pItem, int kind );	// �ͽ��� ���¿� �ʿ��� ������ ���´�
	int	CancelMixtureItemReset(int Flag=0);		// �ͽ��� ������ ����ϸ� â�� �ݴ´�.
	int	CheckMixtureResetItemForm();	// ������ üũ?
	int	ReformMixtureResetItem();		// ������ �ٽ� üũ?



	int	MixtureItemReset();	
	int	CheckItem();
	int	CheckMixtureItemResetOk();
	int	CheckMixtureItemData();	



	int	sinSendMixtureItemReset();			// ������ �ͽ��ĸ����� �������� ����
	int	sinRecvMixtureItemReset(sMIXTURE_RESET_ITEM_SERVER *pMixtureResetItem);		// �ͽ��� ���µ� �������� Recv�� ������ ����
	int	sinRecvMixtureItemResetResult(sMIXTURE_RESET_ITEM_SERVER *pMixtureResetItem_Server);
};

// pluto
/*----------------------------------------------------------------------------*
*								������ ���� Ŭ���� 
*-----------------------------------------------------------------------------*/
class CSmeltingItem
{
public:
	int OpenFlag;
	int SmeltingItemCheckSum;
	int m_SmeltingPrice;
public:
	int CancelSmeltingItem();
	int SetSmeltingItemAreaCheck( sITEM *pItem, int Kind = 0 );
	int LastSetSmeltingItem( sITEM *pItem, int Kind = 0);
	int CheckSmeltingItemForm();
	int ReFormSmeltingItem();
	int PickUpSmeltingItem( int x, int y, int PickUpFlag = 0, int Kind = 0 );
	int sinSendSmeltingItem(); // ������ ���� ������ ������
	int sinRecvSmeltingItem( sSMELTINGITEM_SERVER *pSmeltingItem_Server ); // ���� ������ �޴´�
	int sinRecvSmeltingItemResult( sSMELTINGITEM_SERVER *pSmeltingItem_Server );
	int SmeltingCheckEmptyArea( sITEM *pItem );
	int GetSmeltingCheckEmptyArea();
	int CheckSmeltingPrice();		// ���� ��� �˻�
	int GetSheltomIndex( DWORD SmeltingItemCode ); // ���� ȸ�� �̹��� ����
};

/*----------------------------------------------------------------------------*
* pluto							������ ���� Ŭ���� 
*-----------------------------------------------------------------------------*/

class CManufacture
{
public:
	int m_OpenFlag;
	int m_ManufactureItemCheckSum;
	int m_ManufacturePrice;
	DIRECTDRAWSURFACE m_lpGrayRune[MAX_SHELTOM];
public:
	void RuneIndexInit();	// �ʿ��� �� ȸ�� �̹���
	int CancelManufactureItem(); // ���� ���
	int SetManufactureItemAreaCheck( sITEM *pItem ); // ���� ������ �� �ڸ� �˻�
	int LastSetManufactureItem( sITEM *pItem );		// ���� ������ ���´�
	int CheckManufactureItem();
	int ReFormManufactureItem();
	int PickUpManufactureItem( int x, int y, int PickUpFlag = 0 );
	int sinSendManufactureItem();	// ������ ������
	int sinRecvManufactureItem( SManufactureItem_Server *pManufactureItem_Server );
	int RecvManufactureItemResult( SManufactureItem_Server *pManufactureItem_Server ); // �������� ���� ���
	int ManufactureCheckEmptyArea( sITEM *pItem );
	int GetRecipeIndex( DWORD Code ); // ������ �ε����� �����´�
	int GetManufactureCheckEmptyArea();
	SManufacture_ResultItemInfo ManufactureCheckItem( sITEM *pItemCode, smCHAR_INFO *charInfo );	// ���۵� ������ �ڵ�. ���� �˻� �ҷ��� // �庰 - ���������� ȥ
};

/*----------------------------------------------------------------------------*
*							ũ����Ʈ ������ Ŭ���� 
*-----------------------------------------------------------------------------*/
class cCRAFTITEM{
public:
	int OpenFlag;
	int CraftItemCheckSum;
	int ForceFlag;
	int ForceItemPrice;
	int iReconItemFlag;
public:

	int SetCraftItemAreaCheck(sITEM *pItem,int Kind=0); //�������� �����Ѵ� 
	int LastSetCraftItem(sITEM *pItem,int Kind=0);
	int PickUpCraftItem(int x , int y , int PickUpFlag=0,int Kind=0); //�������� ���ų� ������ �����ش� 

	//�������� �����Ѵ� 
	int MixingItem();

	//������ ������ üũ�Ѵ� 
	int CraftCheckEmptyArea(sITEM *pItem);
	int CancelMixItem(); //�ͽ��� �������� ����Ѵ� 

	int CheckHackCraftItem(); //ũ����Ʈ ������ ���� ����
	int CheckCraftItemForm(); //�����ϴ� ����� �ڽĵ��� �׿������� ơ!!
	int ReFormCraftItem();    //�����ϴ� ����� �ڽĵ��� �׿������� ơ!!

	int sinSendCraftItem();	  //������ �ͽ��� �������� ������ 	
	int sinRecvCraftItem(sCRAFTITEM_SERVER *pCraftItem_Server);   //�������� �ͽ��� �������� �޴´� 

	//�ͽ��� ������� �޴´� 
	int sinRecvCraftItemResult(sCRAFTITEM_SERVER *pCraftItem_Server);

	//Force Orb �������� ������ üũ�Ѵ�
	int CheckForceOrbPrice();
	int CheckForceOrbPrice(sITEM *pItem);

	//������ �籸�� �ý���
	int haCheckReconItem(); //������ �籸�� 
	int haSendReconItem();  //������ �籸�� �� ������ ������.
	/*----------------------------------------------------------------------------*
	* �ۼ��� : �ϴ��  			
	* Desc   : �߰��Ŵ� �Լ���  		
	*-----------------------------------------------------------------------------*/

	//�κ��丮 ����� üũ�Ѵ�.
	int GetCraftCheckEmptyArea();
	//sorting function
	int haCraftSortItem(sCRAFTITEM *pCraftItem);
};



/*----------------------------------------------------------------------------*
*								â�� Ŭ���� 
*-----------------------------------------------------------------------------*/

class cWAREHOUSE{
public:
	int OpenFlag;	//�Լ��� ����ȭ�� ���� ��¿�� ���� Ŭ���� ���� 
	int WareHouseCheckSum;

public:

	int SetWareHouseItemAreaCheck(sITEM *pItem); //������ üũ�Ѵ� 
	int CrashWareHouseItem(RECT &desRect, int PassItemIndex=0); //�������� ������������ ã�´� 
	int ChangeWareHouseItem(sITEM *pItem); //������ �������� �����Ѵ� 
	int LastSetWareHouseItem(sITEM *pItem); //�������� �����Ѵ� 
	int PickUpWareHouseItem(int x , int y , int PickUpFlag=0); //�������� ���ų� ������ �����ش� 

	//���Ը� ���Ѵ� 
	int GetWeight();

	//������ �̹����� �ε��Ѵ�
	int LoadWareHouseItemIamge();	
	
	//�κ��丮 �������� ��� ����Ѵ� 
	int BackUpInvenItem();
	//�������� �������ش� 
	int RestoreInvenItem(); 
	//â�� �ݴ´� 
	int CloseWareHouse();

	//////////////////���� ���� 
	int ReFormWareHouse();
	int CheckWareHouseForm();

	//////////////////���� �����ۿ� ���� ó�� (�������� ����������)
	int CopyItemNotPickUp(sITEM *pItem,int JumpIndex);
	int CheckCopyItem(); //����� �������� �ִ����� üũ�Ѵ� 
	int DeleteCopyItem(); //���� �������� �����ش� (���� �̻����� â�� ����������� ���� ������)

};

/*----------------------------------------------------------------------------*
*								��ų Ŭ���� 
*-----------------------------------------------------------------------------*/
class cTRADE{

public:

	int OpenFlag;
	int MatTrade[4];
	int MatWareHouse[3]; //â�� (�̹����ϳ��� �������� �����´�)
	int MatTradebuttonMain;
	int MatTradebuttonCancel;

	DIRECTDRAWSURFACE lpbuttonCancel;

	DIRECTDRAWSURFACE lpbuttonCancelInfo;

	DIRECTDRAWSURFACE lpbuttonOk;
	DIRECTDRAWSURFACE lpCheck;

	DIRECTDRAWSURFACE lpCheck_Glay;
	DIRECTDRAWSURFACE lpCheckInfo;
	DIRECTDRAWSURFACE lpDelayCheck;

	DIRECTDRAWSURFACE lpCenterBox;

	sITEM CheckItem[100];
	sITEM CheckRecvItem[MAX_TRADE_ITEM];

	////��� ���� 
	int   TradeRequestFlag;
	DWORD TradeCharCode;
	char  szTradeCharName[64];

	int   TradeItemCheckSum;



public:
	cTRADE();
	~cTRADE();

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

	void DrawTradeText(); //�ؽ��� ����ش� 

	///////////// Ʈ���̵� �Լ� 
	int SetTradeItemAreaCheck(sITEM *pItem); //������ üũ�Ѵ� 
	int CrashTradeItem(RECT &desRect, int PassItemIndex=0 , int Kind=0); //�������� ������������ ã�´� 
	int InitTradeColorRect(); //�÷� �ڽ� �ʱ�ȭ 
	int ChangeTradeItem(sITEM *pItem); //������ �������� �����Ѵ� 
	int LastSetTradeItem(sITEM *pItem); //�������� �����Ѵ� 
	int PickUpTradeItem(int x , int y , int PickUpFlag=0); //�������� ���ų� ������ �����ش� 

	//�������� ���ð������� �˾ƺ��� 
	int CheckTradeItemSet(); //���� üũ ����(���Ը� üũ�ϰ� ������ �Լ��� ȣ���Ѵ�)
	int TradeCheckEmptyArea(sITEM *pItem); //�������� ���õɼ��ִ��� üũ�Ѵ�
	int ChecketInvenItem(sITEM *pItem); //�������� �ӽ÷� �����Ѵ� 

	//�������� �κ��丮�� �����Ѵ� 
	int CheckOkTradeItem();			//Ʈ���̵� Ȯ�� 
	int CancelTradeItem();			//Ʈ���̵� ��� 
	int LoadTradeItemIamge();		//������ �̹����� �ε��Ѵ�

	//���� ���� 
	int CheckTradeButtonOk();		//������ ������ ��´� 

	//���Ը� ���Ѵ� 
	int GetWeight();

	////////////////////�������� �����ϴ� ����� �ڽĵ��� �׿������� ơ!!
	int CheckTradeItemForm();	//�˻� 
	int ReFormTradeItem();		//���� 


};
// pluto ���� ������ ����, ����
int ManufactureItemResultInfo( SManufacture_ResultItemInfo_Server *pResultItemInfo );
///////////// �Լ�
int CopyMixItemCheckDelete(sITEM *pItem);
int sinMakeItemCheck(); //MakeItem�� üũ�Ѵ�
int sinPosBoxNpc();     //PostBox Npc���׼� �޼����� �޴´� 
extern SManufacture_ResultItemInfo g_Manufacture_ItemInfo[MAX_HANDMADEITEM]; // �庰 - ���� // �庰 - ���������� ȥ
extern cTRADE cTrade;
extern sTRADE sTrade;
extern sTRADE sTradeRecv;

extern int RequestTradeButtonFlag; //�ŷ��䱸 ��ư Ŭ�� 
	   
extern cWAREHOUSE cWareHouse; //â�� Ŭ���� 
extern sWAREHOUSE sWareHouse; //â�� ����ü 
extern sITEM *BackUpInvenItem2; //�κ������� �ӽ� ��� 


extern cCRAFTITEM cCraftItem;	//������ ���� 

extern CSmeltingItem SmeltingItem; // pluto ����
extern SSmeltingItem S_smeltingItem; // pluto ����
extern sSMELTINGITEM_SERVER sSmeltingItem_Send; // pluto ���� ������ ����
extern sSMELTINGITEM_SERVER sSmeltingItem_Recv; // pluto ���� �������� ����

extern CManufacture ManufactureItem; // pluto ����
extern SManufactureItem g_sManufactureItem; // pluto ����
extern SManufactureItem_Server g_sManufactureItem_Send; // pluto ���� ������ ����
extern SManufactureItem_Server g_sManufactureItem_Recv; // pluto ���� �������� ����
extern SManufacture_Rune g_sManufacture_Rune[MAX_RECIPE_KIND];
//extern SManufacture_ResultItemInfo_Server g_Item_Recv;
extern sCRAFTITEM sCraftItem;
extern sCRAFTITEM_INFO sCraftItem_Info[MAX_CRAFTITEM_INFO]; //������ ���� ����

extern sCRAFTITEM_SERVER sCraftItem_Send; //������ ������ ������ 
extern sCRAFTITEM_SERVER sCraftItem_Recv; //�������� ���� ������

extern sCRAFTITEM_SERVER sAging_Send; //������ ������ ������ 
extern sCRAFTITEM_SERVER sAging_Recv; //�������� ���� ������ 


extern int ShowItemCraftMessageFlag; //���յ� ������ ���ڸ� �����ش� 
extern int ShowItemAgingMessageFlag; //�������� ����¡������ �޼����� �����ش� 
extern int ShowItemSmeltingMessageFlag; // pluto �������� ���� ������
extern int ShowItemManufactureMessageFlag; // pluto �������� ���� ������
extern int ShowItemOverSpace;	// pluto ��������

extern int TempShowMoney;	  //�������⸸ �ϴ� �� 
extern int ItemPickUpFlag;	  //�������� ���ؼ� �����̸� �ش� 	
extern int WareHouseSaveFlag;


extern cAGING	cAging;		  //����¡ 
extern sAGINGITEM sAgingItem;
extern sAGINGITEM_INFO sAgingItem_Info[MAX_AGINGITEM_INFO];

extern int AgingLevelAttack[21];
extern int AgingLevelCritical[21];

extern char szAgingItemBuff[128];
extern POINT CraftItemMessageSize;

extern int AgingCheckSheltomFlag; //����¡ ���� �÷� �ʱ�ȭ 
extern int AgingSheltomCnt2;
extern int MixItemNoCopyFlag;
extern int CheckMixItemCopyFlag;
extern sMAKEITEM_INFO sMakeItem_Info[MAX_MAKEITEM_INFO];

// �ͽ��� ���� ��ü �� ������ ����ȭ �ϱ�
extern cMIXTURERESET cMixtureReset;						// ������ - �ͽ��� ���� ��ü
extern sMIXTURE_RESET_ITEM	sMixtureResetItem;			// ������ - �ͽ��� ���� ������ ���� ����ü
extern sMIXTURE_RESET_ITEM sMixtureResetItemBackUp;		// ������ - �ͽ��� ���� ������ ���� ����ü ���
extern int	MixtureResetCheckFlag;						// ������ - �ͽ��� ������ �����Ѱ�?
extern sMIXTURE_RESET_ITEM_SERVER	sMixtureReset_Send;	// ������ - �ͽ��� ���� ������ ������ ����ü
extern sMIXTURE_RESET_ITEM_SERVER	sMixtureReset_Recv;	// ������ - �ͽ��� ���� �������� ���� ����ü

extern char *PostBoxDocFilePath[];

extern sPOSTBOX_ITEM sPostbox_Item[50];
extern int PostBoxIndexCnt;
extern DWORD ExpressItemCode;
extern char  szExpressItemName[64]; //���� ������ �̸� 

extern sCRAFTITEM_SERVER sWingItem_Send; //������ ������ ��������
extern sCRAFTITEM_SERVER sWingItem_Recv; //�������� ���� ������ (�� �ʿ��Ѱ� �ְڳ�)

extern int DownTradePosi[6][4];
extern DWORD SheltomCode2[];
extern DWORD MagicSheltomCode[]; // ����� - ���� ���� �߰�
extern DWORD BillingMagicSheltomCode[]; // ����� - ���� ���� ���� �߰�
extern int ForceItemPrice2;
extern sRECON_ITEM sReconItem[MAX_SEEL_COUNT]; // ����� - ���̿��� �� �߰�
extern int		MixCancelButtonDelayFlag; // pluto �ܺ� ������ �ٲ�� -_-