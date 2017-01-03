/*----------------------------------------------------------------------------*
*	���ϸ� :  haPremiumItemItem.h	
*	�ϴ��� :  �����̾� ������ ���񽺰���  
*	�ۼ��� :  ����������Ʈ 2005�� 9��
*	������ :  �ϴ�� 
*-----------------------------------------------------------------------------*/
#pragma once

/*----------------------------------------------------------------------------*
* Desc: ������ �Ҹ� �������� ���   		
*-----------------------------------------------------------------------------*/
namespace nsPremiumItem{
	enum _Item{
		THIRD_EYES     = 1,	 //�����۵������ 
		EXPUP_POTION   = 2,	 //����ġ �� ����
		VAMPIRIC_CUS   = 3,	 //���Ǹ� Ŀ����
		MANA_RECHAR_P  = 4,	 //���� ����¡ ����
		MIGHT_OF_AWELL = 5,	 // pluto ����Ʈ ���� ����
		MANA_REDUCE_P  = 6,	 // pluto ���� ���ེ
		PHENIX_PET	   = 7,  // pluto ��(�ؿ�)
		HELP_PET_TERRY = 8,  // ����� - ���� ����� �� �߰�
		HELP_PET_NEPSYS= 9,  // ����� - ���� ����� �� �߰�
		HELP_PET_IO	   = 10,  // ����� - ���� ����� �� �߰�
		HELP_PET_MUTE  = 11, // ����� - ���� ����� �� �߰�
		VAMPIRIC_CUS_EX   = 12,	 // �庰 - ���Ǹ� Ŀ���� EX
		STAMINA_REDUCE_P  = 13, // ����� - �ٷ� ���ེ ����
		//�����߰�

	};
}
using namespace nsPremiumItem;	

										// pluto ��(�ؿ�)
#define		SINUPKEEPITEM_MAX       13	// pluto ���� ���ེ ���� ����Ʈ ���� ���� �߰� // ����� - ���� ����� �� �߰� 8 -> 11 // �庰 - ���Ǹ� Ŀ���� EX 11 -> 12 
                                        // ����� - �ٷ� ���ེ ���� 12 -> 13
/*----------------------------------------------------------------------------*
* Desc: CONST  �� �ð�Ÿ�� DWORD ���¸� ���.		
*-----------------------------------------------------------------------------*/
const DWORD SINITEM_TIME_5MINUTE    = (15*60*1);	   //15��
const DWORD SINITEM_TIME_10MINUTE   = (30*60*1);	   //30��
const DWORD SINITEM_TIME_3HOURS		= (3*60*60*1); 	   //3�ð�
const DWORD SINITEM_TIME_ONEDAY     = (24*60*60*1);    //1��
const DWORD SINITEM_TIME_SEVENDAY   = (7*24*60*60*1);  //7��
const DWORD SINITEM_TIME_30DAY		= (30*24*60*60*1); //30��
// �庰 - ����ġ���� ����(100% 1�ð�)
const DWORD SINITEM_TIME_1HOURS   = (1*60*60*1);	   //1�ð�




/*----------------------------------------------------------------------------*
* Desc: �ڷ���Ʈ �ھ�		
*-----------------------------------------------------------------------------*/
struct sTelePortCore{
	short   FieldIndex[2];
	bool    PageButtonFlag[2]; 
	short   PageIndex;
	short   ItemIndex;
	char    FieldName[64]; 
};

/*----------------------------------------------------------------------------*
* Desc: ���� ���� �Ŵ� ������		
*-----------------------------------------------------------------------------*/ 
struct sUpKeepItem{
	int     TGAImageNumber;
	bool    Visible;          //������ ������ 
	bool    Infoflag;         //������ ������ ������ �����ش�. 
	char    Id[32];		
	int		PosiX;
	int		PosiY;
	DWORD	IconTime[2];
	short   Alpha;
	short   Alphakind;
	int		Per;		//������Ȯ�� ��ġ�� ��Ÿ���� ����.
};

/*----------------------------------------------------------------------------*
* class cHaPremiumItem		
*-----------------------------------------------------------------------------*/
class cHaPremiumItem
{
public:
	DIRECTDRAWSURFACE  lpUpKeepItemBackImage[SINUPKEEPITEM_MAX];    //���Ŵ� �̹�
	sTelePortCore		  TelePortCore;						           //�ڷ���Ʈ �ھ�
	sUpKeepItem           UpKeepItem[SINUPKEEPITEM_MAX];               //����ġ ��� ������	
	bool                  UseClickPremiumItemFlag;                     //�����̾� ������ ���� ����� ���߾���ϴ°�üũ  
	int                   Using_ItemKind;
	int                   UpKeepItemTGAImage[SINUPKEEPITEM_MAX];       //���Ŵ� �̹���
	bool                  haItemUseDelayFlag;
	int					  Using_SelectInvenItemIndex;				  // ����� - �̵� ���� ������(�κ����� ���õ� ������ �ε����� ������.)

	//�����Ŵ� �ð�
	DWORD                 m_ThirdEyesTime;
	DWORD                 m_ExpUpPotionTime;
	DWORD                 m_VampiricCuspidTime;
	DWORD                 m_ManaRechargingPTime;
	DWORD				  m_ManaReducePotiontime;		// pluto ���� ���ེ ����
	int					  m_ManaReducePotionValue;		// pluto ���� ���ེ ���� ���� %��
	DWORD				  m_MightofAwellTime;			// pluto ����Ʈ ���� ����
	int					  m_MightofAwellWeight;			// pluto ����Ʈ ���� ���� ������
	DWORD				  m_PhenixPetTime;				// pluto ��(�ؿ�)
	DWORD				  m_HelpPetTime;				// ����� - ���� ����� �� �߰�
	DWORD				  m_VampiricCuspidEXTime;		// �庰 - ���Ǹ� Ŀ���� EX
	DWORD				  m_StaminaReducePotiontime;    // ����� - �ٷ� ���ེ ����
	int					  m_StaminaReducePotionValue;   // ����� - �ٷ� ���ེ ���� ���� %��

	// �庰 - �׸� �Ӽ�
	DWORD				  m_TerryTime;
  
public:
	cHaPremiumItem();
	~cHaPremiumItem();

	//msg
	void init();
	void Main();
	void Draw();
	void DrawUp();
	void KeyDown(); //Ű üũ
	void LButtonDown(int x , int y);
	void LButtonUp(int x,int y);
	int  DrawText();
	//�����̾� �κ� ����ȭ ������ vvvvv
	int  UsePremiumItem(int kind=0);					     //���
	int  SetPremiumItemSkill(int kind=0);					 //��ɼ� ������ ���
	int  UseTeleportScrollItem(short ItemIndex);             //�ڷ���Ʈ�ھ� ������ ���
	int  CheckWindowOpen();									 //�������� ���Ŵ� ������ â�� ������ �������� ���Ұ�
	int  CheckUpKeepItem();					                 //���� �����Ŵ� ������ üũ 
	void InfoBox(int PoisX,int PosiY,int Width,int Height);  //���� ���� ���� �޾� �ڽ��� �����. 
	int  SetUpKeepItem(int kind,DWORD dwTime,bool visible,char* id,int Percent = 0,short ItemType=0);
	int  cHaPremiumItem::CheckTime(DWORD CurrentTime);		

	//�������� ���� ����  �ð��� �޾� �´�.
	void SetThirdEyesTime(DWORD Time){ 	m_ThirdEyesTime   = Time;};
	void SetExpUpPotionTime(DWORD Time){ m_ExpUpPotionTime = Time;};
	void SetVampiricCuspidTime(DWORD Time){ m_VampiricCuspidTime = Time;};
	void SetVampiricCuspidEXTime(DWORD Time){ m_VampiricCuspidEXTime = Time;}; // �庰 - ���Ǹ� Ŀ���� EX
	void SetManaRechargingPTime(DWORD Time){ m_ManaRechargingPTime = Time;};
	void SetManaReducePotionTime(DWORD Time){ m_ManaReducePotiontime = Time; };		// pluto ���� ���ེ ����
	void SetManaReducePotionValue(int Value){ m_ManaReducePotionValue = Value; };	// pluto ���� ���ེ ���� ���� %��
	void SetMightOfAwellTime(DWORD Time){ m_MightofAwellTime = Time; };				// pluto ����Ʈ ���� ����
	void SetMightOfAwellWeight(int Weight){ m_MightofAwellWeight = Weight; };		// pluto ����Ʈ ���� ���� ������
	void SetPhenixPetTime(DWORD Time){ m_PhenixPetTime = Time; };					// pluto ��(�ؿ�)
	void SetStaminaReducePotionTime(DWORD Time){ m_StaminaReducePotiontime = Time; };		// ����� - �ٷ� ���ེ ����
	void SetStaminaReducePotionValue(int Value){ m_StaminaReducePotionValue = Value; };		// ����� - �ٷ� ���ེ ���� ���� %��

	// �庰 - �׸� �Ӽ�
	void SetHelpPetTimeTime(DWORD Time){ m_TerryTime = m_HelpPetTime = Time; };
  	
	//���ƾƮ����
	int  UseHairtintPotion(int ItemKind);//ItemKind 1-A�� 2-B�� 3-C�� 4-D�� 5-E , 6-F, 7-G, 8-H, 

	// ����� - ����¡ ������ ������
	int UseAgingMaster(int ItemKind);

	// ����� - ��ų ������ ������
	int UseSkillMaster(int ItemKind);

	// ����� - �̵� ���� ������
	int UseMoveShop();


};
extern cHaPremiumItem  chaPremiumitem;