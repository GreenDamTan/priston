/*----------------------------------------------------------------------------*
*	���ϸ� :  sinClassInven.h
*	�ϴ��� :  �κ��丮�� ��� ������ �����Ѵ� 
*	�ۼ��� :  ����������Ʈ 12�� 
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#define INVENTORY_MAXITEM		100 //�κ��丮�� ������ �ִ� �ִ������ 
#define INVENTORY_BOX_SIZE		22  //������ �ڽ��� ũ�� 
#define INVENTORY_BOX_COL		6   //������ �ڽ��� ���� (����)
#define INVENTORY_BOX_ROW		12  //������ �ڽ��� ���� (����)

#define INVENTORY_MAX_POS		13    //â�� ������ 10���� �����ۼ��� ������ ���� 3�� �ӽ��̴� RECT�� ����Ѵ� 
#define MAX_WINGQUEST			10  //�� ������ ���� 10�� �Ѱھ� --;


#define		NO_BOX_COLOR				0
#define		SET_ITEM_CHECK_COLOR		1
#define		OVERLAP_ITEM_COLOR			2
#define		NOT_SETTING_COLOR			3
#define		OVERLAP_BOX_COLOR           4
#define		SELECT_ITEM_COLOR			5
#define     NOT_USE_COLOR				6
#define		NOT_AGING_ITEM				7
#define     NOT_ENOUGH_MONEY			8	//������ 


#define     MAX_USESKILL                17 

struct sINVENTORY{
	DWORD  Position;    //���õ� ��ġ 
	RECT   Rect;        //��ġ ��ǥ 
	int    ItemIndex;   //�κ� �����ǿ� ����Ǿ��ִ� ������ �ε��� ItemIndex�� ������ ����� ������ �̻�� 
	int    WeaponClass; // ���� ��빫�Ⱑ �������� �߻������� 

};

#define		MAX_INVEN_PARTICLE		200

#define		SIN_EFFECT_SHOW_INVEN		1
#define		SIN_EFFECT_SHOW_WAREHOUSE	2
#define		SIN_EFFECT_SHOW_TRADE		3
#define		SIN_EFFECT_SHOW_GYUNGFUM	10

/*
#define   sITEMINFO_BIONIC			0	//��ü 
#define   sITEMINFO_EARTH			1	//���ڿ� 
#define   sITEMINFO_FIRE			2	//�� 
#define	  sITEMINFO_ICE				3	//�ñ� 
#define   sITEMINFO_LIGHTING		4	//���� 
#define   sITEMINFO_POISON			5	//�� 
#define   sITEMINFO_WATER			6	//�� 
#define   sITEMINFO_WIND			7	//�ٶ� 
#define	  sITEMINFO_NONE			15	//�Ӽ� ����
*/


////////�Ӽ� ������ ����ü
struct sELEMENT_ATTACK{
	int Bio[2];
	int Fire[2];
	int Ice[2];
	int Lighting[2];
	int Posion[2];
};

////////�κ��丮 ����Ʈ(��ƼŬ) ����ü
struct sINVENTORY_EFFECT{
	smFACE2D InvenFace;
	int Time;
	int Max_Time;
	POINT Speed;
	int Direction;
	int AniCount;
	int FadeType;
	int Kind;		//��𿡼� ���� ����Ʈ�ΰ� 
	int Flag;
	POINT DesPosi;
	POINT Posi;

};

//���Ͽ� �ھ� ��� ����Ʈ ����ü 
struct sUNIONCORE_EFFECT{
	int Flag;
	int Time;
	int Max_Time;
	int MatNum;
	int x;
	int y;
	int Alpha;
	int w;
	int h;

};

struct sWINGQUEST{
	DWORD NeedItemCode[6];
	int   Money;
	DWORD ChangeItemCode;

};

class cINVENTORY{

public:

	int OpenFlag;       //������ �� �����ϱ� ������ ��ġ�� ����� �ٲ�� �ȵȴ� 

	sITEM InvenItem[INVENTORY_MAXITEM];

	sITEM InvenItemTemp[INVENTORY_MAXITEM];        //�ڽ��� �ִ� �������� ��� �����Ѵ� (0-79 Box 80-83 Posi) 
	sITEM BackUpInvenItemTemp[INVENTORY_MAXITEM];  //�ڽ��� �ִ� �������� ��� �����Ѵ� (0-79 Box 80-83 Posi) 


	char JobName[32];

	DIRECTDRAWSURFACE	lpInvenArrow[2];
	DIRECTDRAWSURFACE	lpCoverSmall;
	DIRECTDRAWSURFACE	lpCoverMiddle;
	DIRECTDRAWSURFACE	lpCoverLarge;
	DIRECTDRAWSURFACE	lpMyShopIcon;
	DIRECTDRAWSURFACE	lpMyShopIcon_T;



	DIRECTDRAWSURFACE	lpPotionBack;
	DIRECTDRAWSURFACE	lpChangeABInfo[2];
	DIRECTDRAWSURFACE	lpExitInfo;
	DIRECTDRAWSURFACE	lpWeightInfo;
	DIRECTDRAWSURFACE	lpItemABInfo[2];

	DIRECTDRAWSURFACE	lpShowWeaponClass[4];

	DIRECTDRAWSURFACE	lpIndiShopIcon;


	DIRECTDRAWSURFACE   lpPuzzleInven;     //���� ĭ
	DIRECTDRAWSURFACE   lpPuzzlewidth;     //�������� ������ ���콺�̹��� 
	DIRECTDRAWSURFACE   lpPuzzleHeight;    //�������� ������ ���콺�̹��� 




	int MatInven[4];   
	int MatInvenParticle[5];

	DWORD InvenDataCheckSum;	//����Ÿ�� ���� üũ�� 

	/////////////// �������� ������ �÷��ִ� ����
	int ViewDamagePercent;
	int ViewDamageNum;

	//
	int iEventBuyItemKind;
	
public:
	cINVENTORY();
	~cINVENTORY();

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
	void KeyDown(); //Ű üũ 

	///////////////////////////
	int LoadItemInfo(); //������ ������ �ε����� �����Ѵ�  
	int AutoSetInvenItem(sITEM *pItem , int PostBox=0); //�������� �ڵ� �����Ѵ� 

	int InvenEmptyAearCheck(sITEM *pItem);//���⿵���� �ƴ� �ڽ� ������ �����Ѵ� 

	int SetItemToChar(int CheckHackFlag=0); //�������� �ɷ�ġ�� ĳ���Ϳ� �����Ѵ� 
	int LastSetInvenItem(sITEM *pItem, int AutoSet=0); //�������� �κ��丮�� �����Ѵ� 

	int PickUpInvenItem(int x , int y, int PickUpFlag=0); //�������� ���´� 

	int ChangeInvenItem(sITEM *pItem); //�������� �ٲ۴� 

	int CrashInvenItem(RECT &desRect ,int PassItemIndex=0 ); //�����۳����� �浹������ üũ�Ѵ�
														   //PassItemIndex�� �ΰ��̻� �������� �����ִ����� üũ�Ѵ�

	////////////////////////// ������ Load & Release
	void InvenItemLoad();    //�������� Load �Ѵ� 
	void InvenItemRelease(); //�������� Release �Ѵ�  


	////////////////////////// üũ 
	int CheckRequireItem();  //�������� ��밡���������θ� üũ�Ѵ� 
	void CheckWeight();      //���Ը� üũ�Ѵ� 
	int CheckRequireItemToSet(sITEM *pItem); //�����Ҽ� �ִ����� üũ�Ѵ� 

	int InvenNotOpenCheck(sITEM *pItem); //�κ��丮�� ������ ���࿩�� 


	//////////////////////////TwoHand ���� ���� 
	int SetTwoHandItem(sITEM *pItem);     //���   ���⸦ �����Ѵ� 
	int PickUpTwoHandItem(sITEM *pItem);  //���ڵ� ���⸦ ���´� 
	int OverlapTwoHandItem(sITEM *pItem); //���ڵ� ����� ��ġ�� �������� üũ�Ѵ� 
	int OverlapTwoHandSwitch(sITEM *pItem);//�����տ� ������ �޼��� üũ �޼��� ������ üũ  
	int ClearTwoHandPosiAndItem(sITEM *pItem); //���ڵ� �������� �����ش� 

	
	//////////////////////////Potion  ���� 
	int AutoSetPotion(sITEM *pItem);      //���� �ڵ����� 
	int MouseSetPotion(sITEM *pItem);     //���콺�� ������ �����Ѵ� 
	int AutoSetPotionToBox(sITEM *pItem); //������ â�� �����Ѵ� 
	int cINVENTORY::ReSettingPotion();    //���ǰ����� �����Ұ�쿡�� ������ ������ â�� �����Ѵ� 
	int UsePotion(int x , int y);         //������ ����Ѵ� 
	int CheckOverlapItem(sITEM *pItem, int PassItemIndex);   //������ �������� ����� ã�� �����Ѵ�

	/////////////////////////ĳ���� ���� 
	int SearchJobAndSetting(); //������ ������ ã�Ƽ� �����Ѵ� 
	void LoadCharState();      //ĳ������ ���¸� �ε��Ѵ� 

	////////////////////////AB ������ ���� 
	int ChangeABItem(int Posi);


	int DrawInvenText(); //�κ��丮 Text�� ǥ���Ѵ� 

	int SetInvenItemAreaCheck(sITEM *pItem); //�������� �� ������ üũ�Ѵ�
	int InitColorRect(); //�÷� �ڽ� RECT �ʱ�ȭ 
	int CheckInvenPos(int x , int y , int lx ,int ly , RECT &rect); //�������� ���� ���� �浹üũ 


	/////////////////////////
	int ThrowInvenItemToField(sITEM *pItem); //�������� ������ 
	int NoSettingThrowItem(sITEM *pItem ,int Flag= 0);    //���ý��н� �������� ������ 
	
	/////////////////////////������ �������� 
	sITEMPRICE GetInvenItemPrice(sITEM *pItem);//�������� ���������� ���´� 
	int RepairInvenItem(sITEM *pItem , int RepairCost);//�������� �����Ѵ� 
	
	int CheckSetOk(sITEM *pItem,int AutoFlag=0); //�����Ҽ��ִ����� üũ�Ѵ� 

	//////////////////////// ������ Cheat ���� 
	int CopyItemClear(sITEM *pItem);//����� �������� ���ش�  
	POINT CheckImageSize(DIRECTDRAWSURFACE lpdd); //�׸��� ����� ���ؼ� �����Ѵ� 
	int ImsiCheckItemImageSize();  //�ӽ÷� �������� ����� üũ�Ѵ� 

	/////////////////////// �߸��� ��ǥ�� �������ش� 
	int ReSetPotionXY(sITEM *pItem,int kind=0);

	////////////////////// ��ȯ �ƾ��� 
	int UseEcore(sITEM *pItem);

	////////////////////// ���콺 ������ ����
	int ResetMouseItem();

	//////////////////////������ ������ �����ϱ����� ������ ��ȭ�Ѵ�
	int CheckInvenItemForm();   //�������� ����Ÿ�� �´��� üũ 
	int ReFormInvenItem();		//������ ����Ÿ�� �ٽ� ���� 	

	////////////////////// ���� ���� ���� ����
	int CheckMoneyLimit(int Money);
	
	////////////////////// ���� ���� ���۹���
	int CheckPotionNumForm();
	int ReFormPotionNum();

	////////////////////// ������ ���� �׿��� Ư���� �������� üũ�Ѵ� 
	int CheckSpecialItem(sITEM *pItem);

	////////////////////// InvenTory ����Ʈ 
	int ShowInvenEffectParticle();
	int SetInvenParticle(int x , int y , int Kind );
	int sinDrawInvenTexImage(smFACE2D *Face);
	int MoveInvenParticle(); //��ƼŬ�� �����δ� 


	////////////////////// �κ��丮�� �������� �����Ҽ��ִ� ������ �ֳ��� üũ�Ѵ� 
	int CheckSetEmptyArea(sITEM *pItem);

	////////////////////// ������ ���ο� ����
	int ResetInvenItemToServer(DWORD CODE ,DWORD Head , DWORD CheckSum , DWORD Time );  //����
	int DeleteInvenItemToServer(DWORD CODE ,DWORD Head , DWORD CheckSum);				//���� 


	///////////// �̺�Ʈ (������ �Դ´� )
	int EatSongPyeun(sITEM *pItem);


	//////////// ������ �������� �����Ѵ� 
	int CharOnlySetItem(sITEM *pItem);

	//////////// ĳ���� ���� ������ �˻����� ������ ������ 
	int HackCharStateCheck(smCHAR_INFO *pChar_Info , smCHAR_INFO *desChar_Info);

	//// �߸���ġ�� �������� ���ġ�Ѵ�
	int CheckResetInvenItem();

	//���� �������� �ֳ����� ã�ƺ��� 
	int SearchItemCode(DWORD CODE , int KindFlag=0);
	
	//�ִ뵥������ ũ��Ƽ���� üũ�ؼ� �����κ����ش�  
	int CheckDamage();

	//����Ʈ�������� üũ�ؼ� Clear�Ѵ�
	int ClearQuestItem();

	//��ũ�ھ �������� �޴´�
	int RecvLinkCore(sITEMINFO *pItem);

	//���콺 ������ üũ�Ѵ�
	int CheckMousePotionNumForm();
	
	//���콺�� ������ �����Ѵ� 
	int ReFormMousePotionNum();

	//���Ͽ� �ھ� ����Ʈ�� �����Ѵ� 
	int SetUnionCoreEffect(int x , int y);
	int UnionCoreParticleProcess();
	int UnionCoreParticle(int x , int y);
	int UnionCoreParticle2(int x , int y , int Flag = 0); //������ ����Ʈ 

	//�κ��丮�� ���õɼ��ִ��� üũ�Ѵ�
	int CheckInvenEmpty(sITEM *pItem);

	//sItemInfo�� �������� �κ��丮�� �����Ѵ�
	int SetInvenToItemInfo(sITEMINFO *pItemInfo);

	//���� ����ġ�� �̹����� �ε��Ѵ�
	int LoadMoneyExpImage(sITEM *pItem);

	//�������� ����Ʈ 
	int sinWingQuestNpc(); 

	int SendWingQuestItem(int Index); //�������� ����Ʈ 

	//�ѹ��� ��밡���� ������������ üũ�Ѵ�
	int CheckRequireItemToSet2(sITEM *pItem);

	//Ư���� �������� ����Ʈ �κ��丮�� �����Ѵ�
	int SetSpecialItem(sITEM *pItem);

	//Special �������� �����ش�
	int ChangeSpecialItem(int Flag = 0);

	//Special ������ ����
	int CheckSpecialItemPosi();

	//�ڵ峪 ������ ������ �κ��丮 ������ �ε����� �����Ѵ�
	int SearchItemIndex(DWORD CODE , int KindFlag);
	
	//���ָӴϸ� ����� �ٲ۴�
	int ChangePuzzle(sITEM *pItem);

	//������ üũ�Ѵ�
	int CheckPuzzle();

	//������ ������
	int CheckSendPuzzle();

	//���� NPC
	int SendPuzzleNpc();

	//�������� �������� �����Ѵ� 
	int IncreViewDamage(int Persent , int Num=0);

	//�ٺ��� ���� üũ�ؼ� ������ ������
	int SendEventItem();

	//�ٺ��� ���� üũ�Ѵ�
	int CheckEventItem();

	//ForceOrb�� ����Ʈ�� �׸���
	int DrawForceOrbEffect(int x , int y);

	//ForceOrb�� ����Ѵ�
	int UseForceOrb(sITEM *pItem , int Flag = 0);

	// ����� - ���� ���� �߰�(���� ������ ����Ѵ�)
	int UseMagicForceOrb(sITEM *pItem , int Flag = 0);

	// ����� - ���� ���� ���� �߰�(���� ���� ������ ����Ѵ�)
	int UseBillingMagicForceOrb(sITEM *pItem , int Flag = 0);

	//ForceOrb�� �����Ѵ�(��������� ������)
	int SetForceOrb(DWORD ItemCODE , int Time);

	// ����� - ���� ���� �߰� (���� ������ �����Ѵ�.)
	int SetMagicForceOrb(DWORD ItemCODE , int Time);

	// ����� - ���� ���� ���� �߰� (���� ���� ������ �����Ѵ�.)
	int SetBillingMagicForceOrb(DWORD ItemCODE , int Time);
	
	//����� - ĸ�� �������� ����Ѵ�.(ȣ���� ĸ�� ���)
	int UseCapsuleItem(sITEM *pItem);



	/*----------------------------------------------------------------------------*
	* �ۼ��� : �ϴ��  			
	* Desc   : �߰��Ŵ� �Լ���  		
	*-----------------------------------------------------------------------------*/
	//Fox�������� üũ�Ѵ�
	int CheckFoxEventItem();
	//�������� �ƹ����� ������
	int SendFoxEventItem();
	//��¦��¦ ���� üũ�Ѵ�
	int CheckChristMasEventItem();
	//��¦��¦ ���縦 ������
	int SendChristmasEventItem();
	//����Ʈ �������� ��ǥ�� �ٲ۴�
	int ReSetQuestItemPosi();
	//�ϰ����� ũ����Ż�� ���� üũ�Ѵ�
	int CheckCristalEventItem();
	//�ϰ����� ũ����Ż�� ������ ������.
	int SendCristalEventItem();

	//����� - ���� ��ƿ��� ���� ������ ������ üũ�Ѵ�.
	int CheckWatermelonEventItem();
	//����� - ���� ��ƿ��� ���� �������� ������ ������
	int SendWatermelonEventItem();
	// ����� - ���� �������� ������ �����Ѵ�.(�κ����� ���� ���� �����ϱ�)
	int CheckItemCount(DWORD CODE);

	//�庰 - �߷�Ÿ�� ���ݷ� ��ƿ��� ���� ������ ������ üũ�Ѵ�.
	int CheckValentineEventItem();
	//�庰 - �߷�Ÿ�� �̺�Ʈ // �߷�Ÿ�� ���ݷ� �������� ������ ������
	int SendValentineEventItem();
	// �庰 - ���� �������� ������ �����Ѵ�.(�κ����� �߷�Ÿ�� ���ݷ� ���� �����ϱ�)
	int CheckValentineItemCount(DWORD CODE);

	// ����� - ���ĺ� ���� �̺�Ʈ(���ĺ��� ������ üũ�Ѵ�.)
	int CheckPristonAlphabetEventItem();
	// ����� - ���ĺ� ���� �̺�Ʈ(���ĺ� �������� ������ ������)
	int SendPristonAlphabetEventItem();

	// �庰 - ĵ������ ĵ�� ��ƿ��� ĵ�� ������ ������ üũ�Ѵ�.
	int CheckCandydaysEventItem();
	// �庰 - ĵ������ �̺�Ʈ // ĵ������ ĵ�� �������� ������ ������
	int SendCandydaysEventItem();
	// �庰 - ���� �������� ������ �����Ѵ�.(�κ����� ĵ�� ���� �����ϱ�)
	int CheckCandyItemCount(DWORD CODE);

	// �庰 - �����ñ׸� - ������ ������ üũ�Ѵ�.
	int CheckMagicalGreenEventItem();
	// �庰 - �����ñ׸� - �������� ������ ������
	int SendMagicalGreenEventItem();
	int SendMagicalGreenEmeraldEventItem();
	// �庰 - �����ñ׸� - ���� �������� ������ �����Ѵ�.
	int CheckMagicalGreenItemCount(DWORD CODE);
	int CheckMagicalGreenItemEmeraldCount(DWORD CODE);

	// �庰 - ī���� ���� ������ üũ�Ѵ�.
	int CheckTearOfKaraEventItem();
	// �庰 - ī���� ������ ������ ������
	int SendTearOfKaraEventItem();
	// �庰 - ī���� ���� ���� �����ϱ�
	int CheckTearOfKaraItemCount(DWORD CODE);

	// ����� - 2010 ������ �̺�Ʈ - �౸�� ���� ����ϱ�
	int UseSoccerBallPotionItem(sITEM *pItem);

	// �庰 - ������� ã�ƶ�
	int CheckFindinvestigatorEventItem();
	int CheckFindinvestigatorKamiuEventItem();
	int CheckFindinvestigatorEtoEventItem();
	int SendFindinvestigatorNineEventItem();
	int SendFindinvestigatorTaleEventItem();
	int SendFindinvestigatorTaleEtoEventItem();
	int CheckFindinvestigatorNineItemCount(DWORD CODE);
	int CheckFindinvestigatorTaleItemCount(DWORD CODE);




	int DeleteCoupleRing();		// pluto Ŀ�ø�

	// �庰 - �Է�â
	char szDoc[128];     // �Է¹��� ��
	char szSendDoc[128]; //������ ���� ��


};

extern cINVENTORY	cInvenTory;

extern sINVENTORY sInven[INVENTORY_MAX_POS];

extern DWORD sinInvenColor[7];

extern sITEM *pUsePotion; //����� ������ ������ 

extern int sinLoadOK; //�ε� �Ϸ�� ���� ���� 

extern int LbuttonUpCheckFlag;

extern int sinChangeSetFlag; //���� ���� ü���� �÷� 

extern int SelectInvenItemIndex;

extern int CrashItemIndex[2];

extern int 	AutoSetItemIndex;

extern int SelectPotionIndex;

extern int ArrowState[2];

extern int InvenArrowPosi;

extern int sinButtonFlag;

extern sINVENTORY_EFFECT	sInven_Effect[MAX_INVEN_PARTICLE];	//�κ� ��ƼŬ 

extern int ArrowState[2];

extern int InvenArrowPosi;

extern int sinUndeadAbsorb;

extern float   sinAdd_fMagic_Mastery;
extern int	   sinInterHeight2;

extern sELEMENT_ATTACK	sElement_Attack;
extern int sinTempDamage2[2];
extern int sinTempAbsorb;
extern int sinTempBlock;
///////////�ӽ� Extern ���߿� �����
extern int SendCheckItemToServer(sITEMINFO *lpItem);

extern sITEMINFO TempLinkCoreItem;

extern POINT UnionCorePosi;

extern DIRECTDRAWSURFACE   lpGold7;
extern DIRECTDRAWSURFACE   lpExp7;

extern sWINGQUEST sWingQuest[MAX_WINGQUEST];
extern int   sinOverWeightCnt;

extern int SpecialItemShowFlag;
extern sITEM MyShopMouseItem;
extern int sinMyShopItemPass;
extern int NotChangeSetItemFlag;
extern int MatInterEffectAging[5];
extern int MatInterEffectAging2[4];
extern int MatSuccessEffectAging;
extern int ForceOrbUseTime[];
extern int ForceOrbDamage[];
extern int MagicForceOrbDamage[]; // ����� - ���� ���� �߰� ���ݷ� 
extern int BillingMagicForceOrbDamage[];  // ����� - ���� ���� ���� �߰� ���ݷ� 
extern int MagicForceOrbUseTime[]; // ����� - ���� ���� �߰�
extern int BillingMagicForceOrbUseTime[]; // ����� - ���� ���� ���� �߰�
extern int ForceOrbUseLevel[][2];
extern int MagicForceOrbUseLevel[][2]; // ����� - ���� ����(��� ����)
extern int BillingMagicForceOrbUseLevel[][2]; // ����� - ���� ���� ���� �߰�

extern int BoosterItem_UseTime[]; // ����� - �ν��� ������(�����, ���, �ٷ�) 
extern int BoosterItem_DataPercent[]; // ����� - �ν��� ������(�����, ���, �ٷ�) ���� �ۼ�Ʈ

extern int nSkillDelay_UseTime[]; // �庰 - ��ų ������

// �庰 - �Է�â
extern int nName;

//�ϰ����� ����
extern DWORD CristalEventCODE[12];

//����� - ���� ��ƿ���
extern DWORD WatermelonEventCODE;

//�庰 - �߷�Ÿ�� ���ݸ� ��ƿ���
extern DWORD ValentineEventCODE;

// ����� - ���ĺ� ���� �̺�Ʈ
extern DWORD PristonAlphabetEventCODE[7];

// �庰 - ĵ������ ĵ�� ��ƿ���
extern DWORD CandydaysEventCODE;

// �庰 - �����ñ׸�
extern DWORD MagicalGreenEventCODE[2];

// �庰 - ī���� ���� ��ƿ���
extern DWORD TeatOfKaraEventCODE;

// �庰 - ������� ã�ƶ� 
extern DWORD FindinvestigatorEventCODE[2];

//������ ������ 
extern DWORD dwCastlItemCODE[];
extern DWORD dwPremiumItemCode[];

// ����� - �ν��� ������
extern DWORD dwBoosterItemCode[];

// �庰 - ��ų ������
extern DWORD dwSkillDelayItemCode[];
