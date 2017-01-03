/*----------------------------------------------------------------------------*
*	File name: sinItem.h
*	That day: Manage
*	Date: Last updated December
*	Aptitude By: baksangyeol
*-----------------------------------------------------------------------------*/	
#ifndef	_SINITEM_HEADER_

#define _SINITEM_HEADER_

#define MAX_ITEM					1300	//071203 (700->720 ) pluto  
											//�ۼ��� : �����,  �ۼ��� : 08.04.07
											//��  �� : Ŭ��ġ�� �� 20�� �߰�(720->740)					
											// ����� - ��ǥ ���� �� 746 ���� ����
											// ����� - ���۾ϸ�(2), ���ǵ����(2), ��ų������(3), ����¡������(3)
											// �̵�����(1), ����ġ��������(3), �����ʱ� ����(5)   746 -> 765
											// ����� - �Ǵн� ��(30��) �߰� 765 -> 766
											// ����� - �̺�Ʈ ��, �̺�Ʈ �ƹ·� �߰� 766 -> 768
										 // ����� - ���� ���� ����, ���� ����¡ ����, ���̿��� �߰� 768 -> 771
										 // ����� 09.10.13 �ν��� ������(�����, ���, �ٷ�) 9�� �߰�(771->780)
										 // ����� 09.12.04 ���� ���� ���� �߰�(780->783)
										 // ����� 09.11.04 ���� ����(14��), ���� ����(14��), �Ϲ� ����(2�� ����, ������) �߰�(783->813) ���� ���� �߰�
										 // ����� 09.12.01 ���� ����� �� �߰�(813->817)
										 // ����� 09.12.08 ��Ÿ��, ��Ÿ�ƹķ� �߰�(817->819)
										 // ����� 09.12.17 �̺�Ʈ��(1�ð�, 1��), �̺�Ʈ�ƹķ�(1�ð�, 1��), ���ϴ��� ��(1��) �߰�(819->824)
										 // ����� 10.02.02 ���� ���� ��(ǻ����, �ٺ���) �߰�(824->826)
										 // �庰 10.02.01 �߷�Ÿ�� ���ݸ� �߰�(826->827)
										 // �庰 10.02.02 Heart Ring(7��) �߰�(827->828)
										 // ����� 10.02.12 ������ ����(os114) �߰� (828->829) 
									 	 // ����� 10.02.12 ���ĺ� �߰� (829->836) 
									   	 // �庰 10.02.16 ȭ��Ʈ�����Ʈ �̺�Ʈ ����, ��, ���ۼ�, ���� �����  �߰�(836->847)
										 // �庰 10.02.16 ���� ����� �� 1�� �߰�(4����)(847->851)
										 // �庰 10.03.01 ��Ʈ�ƹķ�(7��) �߰�, ĵ������ ĵ��(851->853)
										 // �庰 10.03.01 �Ǵн�(1�ð�), ����ġ 2�� ������(1�ð�), ���ǵ����1��, ���۾ϸ�1�� �߰� (853->857)
										 // �庰 10.03.01 ��������� 4���� �߰� // �庰 - ���� ����� �� �߰�(7��) (857->861)
										 // ����� 10.03.08 ����¡ ������(2��) ������ �߰� 3��(D,E,F) (864->865)
										 // ����� 10.03.26 ȣ���� ĸ�� �߰�(864->865)	
										 // �庰 10.03.29 ���������� ȥ ������(865->875)
										 // �庰 10.04.01 ���������� ȥ ���� �� ������(875->879)
									   	// �庰 10.04.13 �׶��Ƽ ��ũ��, �����ñ׸�(���޶���, ����), �޸𸮾� ���̵�, �Ⱓ�� 4��(879->890)
										// �庰 10.05.04 ���� �������� ���� 1.5, �Ⱓ�� 4��(890->895)
										// �庰 10.06.10 ���Ǹ�Ŀ����EX, ī���� ����(894->898) 
										// ����� 10.06.10 2010 ������ �̺�Ʈ (�౸�� ���� �߰�) (898->899)
										// ����� 10.07.13 �ͽ��� ���� ����, ������ ����, �ٷ� ���ེ ����(1��,7��), �ʵ� �ھ�(1��, 7��)������ �߰� (899 -> 909)
										// �庰 10.07.15  �űԾ����� 17��, ����(909->927) 
										// �庰 10.08.10 �ҿｺ�� 9��, ���� �ƹķ� 9�� �߰�( 927->945 )
										// �庰 10.09.03 ���� �̺�Ʈ ��, �ƹ� 3�� �߰�(945 -> 948 )
			
#define ITEMSIZE					22   //Value dividing the size of the item image

#define CHECK_COPY_ITEM				1
#define CHECK_GIVE_ITEM				2


/*---------------------------------------------*
*			The item is placed defines the position '
*----------------------------------------------*/
#define INVENTORY_POS_BOX			    0x00000001  
#define INVENTORY_POS_LHAND				0x00000002 
#define INVENTORY_POS_RHAND				0x00000004 
#define INVENTORY_POS_ARMOR				0x00000008 
#define INVENTORY_POS_BOOTS				0x00000010 
#define INVENTORY_POS_GLOVES			0x00000020 
#define INVENTORY_POS_LRING				0x00000040 
#define INVENTORY_POS_RRING				0x00000080 
#define INVENTORY_POS_SHELTOM			0x00000100 
#define INVENTORY_POS_AMULET			0x00000200 
#define INVENTORY_POS_ARMLET			0x00000800
#define INVENTORY_POS_TWO_HAND			0x00001000
#define INVENTORY_POS_POTION			0x00002000


/*---------------------------------------------*
*			defines the class of the item
*----------------------------------------------*/

#define ITEM_CLASS_WEAPON_ONE			INVENTORY_POS_RHAND 
#define ITEM_CLASS_WEAPON_TWO			(INVENTORY_POS_RHAND |INVENTORY_POS_LHAND) //Hand-to-everywhere there deuleogalsu
#define ITEM_CLASS_ARMOR				INVENTORY_POS_ARMOR 
#define ITEM_CLASS_BOOTS				INVENTORY_POS_BOOTS 
#define ITEM_CLASS_GLOVES				INVENTORY_POS_GLOVES 
#define ITEM_CLASS_SHIELDS				INVENTORY_POS_LHAND
#define ITEM_CLASS_RING					(INVENTORY_POS_LRING | INVENTORY_POS_RRING)
#define ITEM_CLASS_SHELTOM				INVENTORY_POS_SHELTOM 
#define ITEM_CLASS_AMULET				INVENTORY_POS_AMULET 
#define ITEM_CLASS_ARMLET				INVENTORY_POS_ARMLET 
#define ITEM_CLASS_MAGICIAL_STUFFS		INVENTORY_POS_LHAND
#define ITEM_CLASS_POTION				INVENTORY_POS_POTION
#define ITEM_CLASS_ECORE				INVENTORY_POS_BOX
#define ITEM_CLASS_QUEST				INVENTORY_POS_BOX
#define ITEM_CLASS_FORCEORB				INVENTORY_POS_BOX
#define ITEM_CLASS_SEEL					INVENTORY_POS_BOX

//Recognize that the close-launched type
#define ITEM_WEAPONCLASS_NOT_SHOOTING	1 //Melee item type
#define ITEM_WEAPONCLASS_SHOOTING		2 //Range item type 
#define ITEM_WEAPONCLASS_CASTING		3 //Casting item type

//Define the class of weapons
/*---------------------------------------------*
*			(Weapons) Primary Category			    
*----------------------------------------------*/
#define sinWA1			0x01010000	//Axes
#define sinWC1			0x01020000	//Claws
#define sinWH1			0x01030000	//Hammer & So On
#define sinWM1			0x01040000	//Magicial Stuffs
#define sinWP1			0x01050000	//Poles & Spears
#define sinWS1			0x01060000	//Shooters
#define sinWS2			0x01070000	//Swords
#define sinWT1			0x01080000	//Throwing Arms


/*---------------------------------------------*
*			(��)1�� ī�װ� 					    
*----------------------------------------------*/

#define sinDA1			0x02010000	//Armor
#define sinDB1			0x02020000	//Boots
#define sinDG1			0x02030000	//Gloves
#define sinDS1			0x02040000	//Shields
#define sinDA2			0x02050000	//Armor


/*---------------------------------------------*
*			(��ű���)1�� ī�װ� 					    
*----------------------------------------------*/

#define sinOA1			0x03010000	//Amulet
#define sinOA2			0x03020000	//Armlet
#define sinOM1			0x03030000	//Magicial Stuffs
#define sinOR1			0x03040000	//Ring
#define sinOR2			0x03050000	//Ring2
#define sinOS1			0x02350000	//Sheltom
#define sinFO1			0x03060000  //Force Orb
#define sinSE1			0x03070000  //Seel
#define sinPR1			0x03080000  // ���� pluto
#define sinPR2			0x03090000  // ���� pluto
#define sinPR3			0x03100000  // ��(A) pluto
#define sinPR4			0x03110000  // ��(B) pluto
/*---------------------------------------------*
*			(����)1�� ī�װ� 					    
*----------------------------------------------*/
#define sinPM1			0x04010000  //Mana
#define sinPL1			0x04020000  //Life
#define sinPS1			0x04030000  //Stamina

/*---------------------------------------------*
*			(��)1�� ī�װ� 					    
*----------------------------------------------*/
#define sinGG1			0x05010000 //Gold
#define sinBS1			0x05020000 //�����佺��

/*---------------------------------------------*
*			(��ȯ)1�� ī�װ� 					    
*----------------------------------------------*/
#define sinEC1			0x06010000

/*---------------------------------------------*
*			  ����Ʈ ������ 
*----------------------------------------------*/

#define sinQT1			0x07010000


/*---------------------------------------------*
*			  �̺�Ʈ ������ 
*----------------------------------------------*/
#define sinSP1			0x08010000 
#define sinGP1			0x08020000		//0x09010000 �����ϴ� �����̳� �׷�.. (& sinITEM_MASK2 �� �ȸ���)
#define sinQW1			0x08030000		//��������Ʈ ��
#define sinGF1			0x08040000		//����ǰ��
#define sinPZ1			0x08060000		//��ó ����
#define sinPZ2			0x08070000		//ŷȣ�� ����
#define sinCH1			0x08080000		//�߷�Ÿ�ε��� ���ݷ�
#define sinSD2			0x08090000		//Sod2 �߻� ������
#define sinBC1          0x080A0000      //������ �߻� ������
#define sinBI1          0x080B0000      //���ᱸ�� ������  
#define sinBI2          0x080C0000      //���ᱸ�� ������(��Ű�� ĳ�� ������ )  
#define sinGP2			0x080D0000		// �庰 - �ҿｺ��


/*---------------------------------------------*
*			  Make Item
*----------------------------------------------*/
#define sinMA1			0x09010000 
#define sinMA2			0x09020000


/*---------------------------------------------*
*			  ���� ��� ������
*----------------------------------------------*/
// pluto ����
#define sinWR1			0x0A010000 // ���� ������
#define sinDR1			0x0A020000 // ���� ������

#define EXTREME_RECIPE		1		// ������ ������
#define ANCIENT_RECIPE		2		// ����� ������
#define	MINOTAUR_RECIPE		3		// ������ ������
#define DOOM_RECIPE			4		// ����� ������
#define SALAMANDER_RECIPE	5		// ȭ���� ������
#define WYVERN_RECIPE		6		// �ٶ��� ������
#define PHOENIX_RECIPE		7		// �¾��� ������
#define FRENZY_RECIPE		8		// ������ ������
#define HEAVENS_RECIPE		9		// õ���� ������ 
#define SNOW_RECIPE			10		// ������ ������
#define MEMORY_RECIPE		11		// ����� ������ // �庰 - ���������� ȥ

/*---------------------------------------------*
*			������ 2�� ī�װ�(�����ۺ��� ����ȭ�� ���� ) 
*----------------------------------------------*/
#define sin00			0x00000000	
#define sin01			0x00000100	
#define sin02			0x00000200
#define sin03			0x00000300
#define sin04			0x00000400
#define sin05			0x00000500
#define sin06			0x00000600
#define sin07			0x00000700
#define sin08			0x00000800
#define sin09			0x00000900
#define sin10			0x00000A00
#define sin11			0x00000B00
#define sin12			0x00000C00
#define sin13			0x00000D00
#define sin14			0x00000E00
#define sin15			0x00000F00
#define sin16			0x00001000
#define sin17			0x00001100
#define sin18			0x00001200
#define sin19			0x00001300
#define sin20			0x00001400
#define sin21			0x00001500
#define sin22			0x00001600
#define sin23			0x00001700
#define sin24			0x00001800
#define sin25			0x00001900 //16
#define sin26			0x00002A00
#define sin27			0x00002B00
#define sin28			0x00002C00
#define sin29			0x00002D00
#define sin30			0x00002E00
#define sin31			0x00002F00
#define sin32			0x00003000
#define sin33			0x00003100
#define sin34			0x00003200
#define sin35			0x00003300
#define sin36			0x00003400
#define sin37			0x00003500
#define sin38			0x00003600
#define sin39			0x00003700
#define sin40			0x00003800
#define sin41			0x00003900
#define sin42			0x00003A00
#define sin43			0x00003B00
#define sin44			0x00003C00
#define sin45			0x00003D00
#define sin46			0x00003E00
#define sin47			0x00003F00
#define sin48			0x00004000	// pluto �߰�
#define sin49			0x00004100	// pluto �߰�
#define sin50			0x00004200	// pluto �߰�
#define sin51			0x00004300	// ����� - �Ǵн� ��(30��) �߰� 
#define sin52			0x00004400
#define sin53			0x00004500
#define sin54			0x00004600
#define sin55			0x00004700
#define sin56			0x00004800
#define sin57			0x00004900
#define sin58			0x00004A00
#define sin59			0x00004B00
#define sin60			0x00004C00  // ����� - ���� ���� ���� �߰�
#define sin61			0x00004D00  // ����� - ���� ����¡ ���� �߰�
#define sin62			0x00004E00
#define sin63			0x00004F00
#define sin64			0x00005000
#define sin65			0x00005100
#define sin66			0x00005200
#define sin67			0x00005300
#define sin68			0x00005400
#define sin69			0x00005500
#define sin70			0x00005600
#define sin71			0x00005700
#define sin72			0x00005800
#define sin73			0x00005900 
#define sin74			0x00005A00 
#define sin75			0x00005B00 
#define sin76			0x00005C00 
#define sin77			0x00005D00 
#define sin78			0x00005E00 
#define sin79			0x00005F00 
#define sin80			0x00006000 
#define sin81			0x00006100 // ����� - ����¡ ������(2��) ������ �߰�
#define sin82			0x00006200 // ����� - ����¡ ������(2��) ������ �߰�
#define sin83			0x00006300 // ����� - ����¡ ������(2��) ������ �߰�
#define sin84			0x00006400 // �庰 - �׸���Ƽ ��ũ��
#define sin85			0x00006500
#define sin86			0x00006600 
#define sin87			0x00006700 
#define sin88			0x00006800 
#define sin89			0x00006900 
#define sin90			0x00006A00 
#define sin91			0x00006B00 
#define sin92			0x00006C00 
#define sin93			0x00006D00 
#define sin94			0x00006E00 
#define sin95			0x00006F00 
#define sin96			0x00007000 
#define sin97			0x00007100 
#define sin98			0x00007200 


/*---------------------------------------------*
*			Items sound
*----------------------------------------------*/

#define		SIN_SOUND_AXES				1
#define		SIN_SOUND_CLAWS				2
#define		SIN_SOUND_HAMMER			3
#define		SIN_SOUND_MAGICIAL		 	4
#define		SIN_SOUND_POLES				5
#define		SIN_SOUND_SHOOTERS			6
#define		SIN_SOUND_SWORDS			7
#define		SIN_SOUND_THROWING			8
#define		SIN_SOUND_ARMOR				9
#define		SIN_SOUND_BOOTS				10
#define		SIN_SOUND_GLOVES			11
#define		SIN_SOUND_SHIELDS			12
#define		SIN_SOUND_AMULET			13
#define		SIN_SOUND_Armlet			14
#define		SIN_SOUND_RING				15
#define     SIN_SOUND_SHELTOM			16
#define     SIN_SOUND_POTION			17
#define     SIN_SOUND_COIN				18
#define     SIN_SOUND_STAFF				19
#define		SIN_SOUND_EAT_POTION		20
#define		SIN_SOUND_SHOW_INTER		21
#define		SIN_SOUND_REPAIR_ITEM		22
#define		SIN_SOUND_FAILD_MIXITEM		23
#define		SIN_SOUND_ARMOR2			24
#define		SIN_SOUND_EAT_POTION2		25


/** Elemental Resistance **/
#define   sITEMINFO_BIONIC			0	//��ü 
#define   sITEMINFO_EARTH			1	//���ڿ� 
#define   sITEMINFO_FIRE			2	//�� 
#define	  sITEMINFO_ICE				3	//�ñ� 
#define   sITEMINFO_LIGHTING		4	//���� 
#define   sITEMINFO_POISON			5	//�� 
#define   sITEMINFO_WATER			6	//�� 
#define   sITEMINFO_WIND			7	//�ٶ� 
#define	  sITEMINFO_NONE			15	//�Ӽ� ����

//////////* Item Type *////////
#define ITEM_KIND_NORMAL			0   //�⺻ ������ 
#define ITEM_KIND_CRAFT				1   //���� ������ 
#define ITEM_KIND_AGING				2   //����¡ ������ 
#define ITEM_KIND_QUEST				3	//����Ʈ�� ������ 
#define ITEM_KIND_EVENT				4	//�̺�Ʈ ������ 
#define ITEM_KIND_MAKE_MAIN			5	//�̺�Ʈ ������ 
#define ITEM_KIND_MAKE_ELEMENT		6	//�̺�Ʈ ������ 
#define ITEM_KIND_QUEST_WEAPON		7	//�̺�Ʈ ������ 
#define ITEM_KIND_SPECIAL			8	//SpecialItemFlag[1] �� �÷���Ͻ� ����Ⱦ��������� ����

/////////////////////////////////
#define   SIN_ADD_FIRE				0x00000001	//��
#define   SIN_ADD_ICE				0x00000002	//����
#define   SIN_ADD_LIGHTNING			0x00000004	//����
#define   SIN_ADD_POISON			0x00000008	//��
#define   SIN_ADD_BIO				0x00000010	//��
#define   SIN_ADD_CRITICAL			0x00000020	//ũ��Ƽ��
#define   SIN_ADD_ATTACK_RATE		0x00000040	//���߷�
#define   SIN_ADD_DAMAGE_MIN		0x00000080	//�ּҰ��ݷ�
#define   SIN_ADD_DAMAGE_MAX		0x00000100	//�ִ���ݷ�
#define   SIN_ADD_ATTACK_SPEED		0x00000200	//���ݼӵ�
#define   SIN_ADD_ABSORB			0x00000400	//�����
#define   SIN_ADD_DEFENCE			0x00000800	//����
#define   SIN_ADD_BLOCK_RATE		0x00001000	//����
#define   SIN_ADD_MOVE_SPEED		0x00002000	//�̵��ӵ�
#define   SIN_ADD_LIFE				0x00004000	//�ִ�����
#define   SIN_ADD_MANA				0x00008000	//�ִ���
#define   SIN_ADD_STAMINA			0x00010000	//�ִ�ٷ�
#define   SIN_ADD_LIFEREGEN			0x00020000  //��������
#define   SIN_ADD_MANAREGEN			0x00040000  //��·����
#define   SIN_ADD_STAMINAREGEN		0x00080000  //�ٷ·����


/*----------------------------------------------------------------------------*
*						 Items Property
*-----------------------------------------------------------------------------*/
struct	sITEM_CREATE {
	DWORD	Head;			//Header
	DWORD	dwVersion;		//Generated version
	DWORD	dwTime;			//Creation Time
	DWORD	dwChkSum;		//Items checksum
};

struct sITEM_SPECIAL {

	float	Add_fAbsorb;			//Add Absorb 
  	int     Add_Defence;			//Add Defence 
	float   Add_fSpeed;				//Add Speed 
	float   Add_fBlock_Rating;		//Add BlockRating
	int     Add_Attack_Speed;		//Add AttackSpeed
	int     Add_Critical_Hit;		//Add CriticalHit
	int     Add_Shooting_Range;		//Shooting Range 
	float   Add_fMagic_Mastery;     //Magic Mastery
	short	Add_Resistance[8];		//Resistance

	////////////////////////////////////////////////

	short	Lev_Attack_Resistance[8];	//���ҿ����� ���ݷ�
	int		Lev_Mana;				//����ȸ��(�ּ�)(�ִ�)
	int		Lev_Life;				//������ȸ��(�ּ�)(�ִ�) 
	int     Lev_Attack_Rating;		//���߷� 
	short	Lev_Damage[2];			//���ݷ�

	float	Per_Mana_Regen;			//���� ��� 6
	float	Per_Life_Regen;			//������ ��� 
	float	Per_Stamina_Regen;		//���׹̳� ���

	//////////////////////////////////////////////////

	DWORD	dwTemp[32];
};

struct sITEMINFO{

	DWORD	dwSize;						//������ ���� ũ��
	
	sITEM_CREATE	ItemHeader;			//������ ���� ����

	short	Durability[2];				//������

	//////// ���� ������ ����Ÿ������ ���ϴ� /////////

	/*-------------------------*
	*		 ������ ����	
	*--------------------------*/
	DWORD	CODE;				//������ �ڵ� 	
	char	ItemName[32];		//������ �̸� 	

	/*-------------------------*
	*		���� ���� 	
	*--------------------------*/
	int     Weight;				//���� 		
	int     Price;				//����

	/////////////////////////////////////////
	int		Index;				//�ε���
	int		PotionCount;		//����ī����
	////////////////////////////////////////

	/**����**/
	short	Resistance[8];		//���ҿ����� ���׷� 
	/********/

	int		Sight;				//�þ�

	DWORD	Temp0;

	/*-------------------------*
	*		 ���ݼ��� 
	*--------------------------*/
	short	Damage[2];			//���ݷ� 
	int     Shooting_Range;		//�����Ÿ� 
	int     Attack_Speed;		//���ݼӵ� 
	int     Attack_Rating;		//���߷� 
	int     Critical_Hit;		//1.5�� ������Ȯ��  

	/*-------------------------*
	*		 ���� 
	*--------------------------*/

	float	fAbsorb;			//����� 
  	int     Defence;			//���� 
	float   fBlock_Rating;		//(����)���� 

	/*-------------------------*
	*		 �̵����� 
	*--------------------------*/
	float     fSpeed;				//�̵� �ӵ� 

	/*-------------------------*
	*		 ���� ���� 
	*--------------------------*/

	int		Potion_Space;		//���� ���� �Ҵ� 
	
	/**Ư���ɷ�**/
	float   fMagic_Mastery;     //�������õ� 
	float   fMana_Regen;		//���� ��� 
	float   fLife_Regen;		//������ ��� 
	float   fStamina_Regen;	    //���׹̳� ��� 
	float   fIncrease_Life;     //������ �ִ�ġ��� 
	float   fIncrease_Mana;     //���� �ִ�ġ���
	float   fIncrease_Stamina;  //���׹̳� �ִ�ġ���

	/************/

	/*-------------------------*
	*		�䱸Ư�� 
	*--------------------------*/
	int     Level;				//���� 
	int     Strength;			//�� 	
	int     Spirit;				//���ŷ� 
	int     Talent;				//��� 
	int     Dexterity;			//��ø�� 
	int     Health;				//�ǰ� 

	/*-------------------------*
	*		 ȸ���� 
	*--------------------------*/
	short   Mana[2];            //����ȸ��(�ּ�)(�ִ�)
	short   Life[2];            //������ȸ��(�ּ�)(�ִ�) 
	short   Stamina[2];         //���׹̳�ȸ��(�ּ�)(�ִ�)
	
	/*-------------------------*
	*		 .....
	*--------------------------*/
	int		Money;				//������ ��
	int     NotUseFlag;         //���Ұ��� �÷�

	DWORD	BackUpKey;			//������
	DWORD	BackUpChkSum;		//��� ������ üũ��

	short   ScaleBlink[2];		//������ ���� 
	DWORD	UniqueItem;			//����ũ
	short	EffectBlink[2];		//����ǥ�� �ֱ� (0) ����Ʈ ýũ��(1)
	short	EffectColor[4];		//����ǥ�ø��

	DWORD	DispEffect;			//������ ǥ�� ����Ʈ (����)


	/*-------------------------*
	*		ĳ���� Ưȭ������    (���� �̼���)
	*--------------------------*/

	DWORD			JobCodeMask;		//Ưȭ ����	(������ ��Ʈ ����ũ)
	sITEM_SPECIAL	JobItem;			//Ưȭ ������ ����

	DWORD   ItemKindCode;		//������ ���� 0�̸� ��� ������ 
	DWORD   ItemKindMask; 

	short   ItemAgingNum[2];	//0�� ����¡+�� 1�� ����¡�� �ǰ��ֳ� �ƴѰ�?
	short   ItemAgingCount[2];	//0������ ���� ī��Ʈ 1����ġ �ִ� 
	short   ItemAgingProtect[2];//����¡ ��ȣ 

	short   SpecialItemFlag[2];   //0�� ���־����� �÷� 1 �� ����Ʈ���� ���� ���Ǵ� Ư��������

	DWORD	dwCreateTime;		//���� �ð�
	DWORD	dwTemp[9];			//����

};

/*----------------------------------------------------------------------------*
*						    struct  Item
*-----------------------------------------------------------------------------*/
struct sITEM
{
	DWORD	CODE;
	char	ItemNameIndex[32];	//Item Name	

	/*-------------------------*
	*	The set during initialization
	*--------------------------*/
	char	LastCategory[16];   //The final category	
	int		w;
	int     h;			        //Item size
	char	ItemFilePath[64];   //Items loaded when a file path
	DWORD	Class;		        //The type of item
	char    DorpItem[64];       //When the item drops
	DWORD   SetModelPosi;		//Position the item to be set
	int		SoundIndex;			//Sound item
	int     WeaponClass;		//Recognize that the close-launched type
	
	
	//--------------------------

	int Flag;			        //Flags items
	int x,y;			        //The item is drawn coordinates
	int SetX,SetY;				//Gives the item is set to be put in place

	DIRECTDRAWSURFACE	lpItem; //Pointer required to load the picture
	int		ItemPosition;       //This item gives a position on the presence of inventory (right hand, left hand, both hands, gapot ...)

	int     PotionCount;		//Only when the potion counts
	int     NotUseFlag;			//It gives the flag when the required value is not satisfied
	int     SellPrice;			//Items sold price

	int     OldX,OldY;          //Doetdeon coordinates the item is set in the past
	DIRECTDRAWSURFACE	lpTempItem; //Pointer required to load the picture
	//int		LimitTimeFlag;		    //Shelf Life

	sITEMINFO sItemInfo;

};

struct sITEMPRICE
{
	int PureSellPrice;
	int SellPrice;
	int RepairCost;
};

//Items requirement flag
struct sITEMREQUIRE
{
	int     rLevel;				//Level
	int     rStrength;			//Strength 	
	int     rSpirit;			//Spirit
	int     rTalent;			//Talent 
	int     rDexterity;			//Dexterity
	int     rHealth;			//Health 
};

/*----------------------------------------------------------------------------*
*						    class   ������
*-----------------------------------------------------------------------------*/
class cITEM
{	
public:
	char *DropItemPath;
	char *CharSetItemPath;

	int MatItemInfoBox;

	int MatItemInfoBox_TopLeft;
	int MatItemInfoBox_TopRight;
	int MatItemInfoBox_TopCenter;

	int MatItemInfoBox_BottomLeft;
	int MatItemInfoBox_BottomRight;
	int MatItemInfoBox_BottomCenter;

	int MatItemInfoBox_Left;
	int MatItemInfoBox_Right;
	int MatItemInfoBox_Center;


	DIRECTDRAWSURFACE	AgingBar;
	DIRECTDRAWSURFACE	AgingGage;
	DIRECTDRAWSURFACE	AgingBar2;

public:
	cITEM();
	~cITEM();

	void Init();
	void Load();
	void Release();
	void Close();
	void Draw();
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); 

	int ShowItemInfo(sITEM *pItem , int Flag,int Index=0); //Find the information store items 1 2 inventory items

	int ShowItemInfo2(sITEM *pItem ,int Flag,int Index=0); //Find the information store items 1 2 inventory items (shared by two)

	int DrawItemText(); //Item information text

	POINT SetTextXY(int x , int y, int len); //To correct the position the text is placed

	int SearchSpecialItemJob(DWORD SpecialJob_CODE); //Only items specific character
	
	int GetInfoBoxSize(sITEM *pItem ,int Col); //Calculate the size of the box

	//And preventing operation of the item table
	int CheckItemTable();

	//Item Shelf Life
	int GetItemLimitTime(sITEM *pItem);

	//It retrieves the coordinates of the item
	void GetMousePos(int *MouseX , int *MouseY);

	//Gauze test
	void SetItemInfo(char *Buff);
};

extern cITEM cItem;	 
extern sITEM sItem[MAX_ITEM];

extern sITEM TempItem;      //While items to be stored structure variable
extern sITEM MouseItem;	    //Items be transferred to the mouse

extern int sinShowItemInfoFlag;  //It shows the item information
extern DWORD sinItemTime;   //Items shelf life

extern int NotSell_Item_CODECnt;
extern int NotSell_Item_MASKCnt;
extern int NotSell_Item_KINDCnt;

extern int NotDrow_Item_CODECnt;
extern int NotDrow_Item_MASKCnt;
extern int NotDrow_Item_KINDCnt;

extern int NotSet_Item_CODECnt;
extern int NotSet_Item_MASKCnt;
extern int NotSet_Item_KINDCnt;

extern DWORD NotSell_Item_CODE[];
extern DWORD NotSell_Item_MASK[];
extern DWORD NotSell_Item_KIND[];

extern DWORD NotDrow_Item_CODE[];
extern DWORD NotDrow_Item_MASK[];
extern DWORD NotDrow_Item_KIND[];

extern DWORD NotSet_Item_CODE[];
extern DWORD NotSet_Item_MASK[];
extern DWORD NotSet_Item_KIND[];

//�����̾� ������ ���� 
//��Ƽ�� �߰� �Ŵ� �Ƹ� �ڵ�
//DWORD   dwPremiumItemArmorCODE[] ={
//	(sinDA1|sin31) , (sinDA1|sin32) , (sinDA1|sin33) , (sinDA1|sin34) , (sinDA1|sin35) ,
//};

#endif