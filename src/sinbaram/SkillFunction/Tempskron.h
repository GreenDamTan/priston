/*----------------------------------------------------------------------------*
*	���ϸ� :  Tempskron.h	
*	�ϴ��� :  �۽�ũ���� ��ų ��� 
*	�ۼ��� :  ����������Ʈ 1��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#define SIN2_ATTACK_RATE		0
#define SIN2_DAMAGE				1
#define SIN2_DEFENSE			2
#define SIN2_ABSORB				3
#define SIN2_SPEED				4
#define SIN2_LIFE				5
#define SIN2_MANA				6 // ����� - �ν��� ������(���)
#define SIN2_STAMINA			7 // ����� - �ν��� ������(�ٷ�)

#define USE_SKILL_MASTERY_COUNT	100

//////////////// ���� 

int RequireStateCheck(int Mana);	//�䱸 ������ �˻��Ѵ� 	
int ContinueSkillUse(sSKILL *pSkill,int Time); //���������� ���Ǵ� ��ų 
void CheckContinueSkill();	//���������� ������� ��ų�� üũ�Ͽ� �ð��� ������ �ʱ�ȭ ���ش� 
void SwitchSkill(sSKILL *pSkill);   //��ų�� ����ġ�Ѵ� 

//////////////// ��ī�ϼ� 
int F_Extreme_Shield();		//�ͽ�Ʈ�� ���� 
int F_Mechanic_Bomb();			//��ī�� �� 
int F_Physical_Absorption();	//������ ����
int F_Poison_Attribute();	//������ ��Ʈ����Ʈ 

int F_Great_Smash();		//�׷���Ʈ ���Ž� 
int F_Maximize();			//�ƽø�����
int F_Automation();			//������̼� 
int F_Spark();				//����ũ 

int F_Metal_Armor();		//��Ż�Ƹ�
int F_Grand_Smash();		//�׷��� ���޽�
int F_M_W_Mastery();		//��ī�п��������͸�
int F_Spark_Shield();		//����ũ ����

//4�� ��ų
int F_Impulsion();          //���÷μ�
int F_Compulsion();         //���÷μ�
int F_Magnetic_Sphere();    //�ų�ƽ �Ǿ�
int F_Metal_Golem();        //��Ż ��

//////////////// ������
int F_Melee_Mastery();		//�и� �����͸� 
int F_Fire_Attribute();		//���̾� ��Ʈ����Ʈ 
int F_Raving();				//���̺� 
int F_Impact();				//����Ʈ 

int F_Triple_Impact();		//Ʈ���� ����Ʈ 
int F_Brutal_Swing();		//�η�Ż ����
int F_Roar();				//�ξ� 
int F_RageOfZecram();		//������ ���� ��ũ��

int F_Concentration();		//����Ʈ���̼�
int F_Avanging_Crash();		//�¡ ũ����
int F_Swift_Axe();			//����ǪƮ����
int F_Bone_Crash();			//�� ũ����

//4�� ��ų
int F_Destoryer();          //���丮��
int F_Berserker();          //����Ŀ
int F_Cyclone_Strike();     //����Ŭ�� ��Ʈ����ũ
int F_Boost_Health();       //�ν�Ʈ �ｺ

//////////////// ����ũ�� 
int F_Pike_Wind();			//����ũ���� 
int F_Ice_Attribute();		//���̽� ��Ʈ����Ʈ 
int F_Critical_Hit();		//ũ��Ƽ�� ��
int F_Jumping_Crash();		//��� ���Ǿ� 

int F_Ground_Pike();		//�׶��� ������ũ 
int F_Tornado_Pike();		//����̵�  
int F_Weapone_Defence_Mastery();	//���� ���潺 �����͸�
int F_Expansion();			//�ͽ���� 

int F_Venom_Spear();		//�����Ǿ�
int F_Vanish();				//��Ͻ�
int F_Critical_Mastery();	//���� �����͸�
int F_Chain_Lance();		//ü�η���
//4�� ��ų
int F_Assassin_Eye();       //��ؽ� ����
int F_Charging_Strike();    //��¡ ��Ʈ����ũ
int F_Vague();              //���̱�
int F_Shadow_Master();      //���Ϳ� ������

//////////////// ����
int F_Scout_Hawk();			//��ī��Ʈ ȣũ 
int F_Shooting_Mastery();	//���� �����͸� 
int F_Wind_Arrow();			//���� ���ο� 
int F_Perfect_Aim();		//����Ʈ ����  

int F_Dions_Eye();			//��½� ���� 
int F_Falcon();				//���� 
int F_Arrow_of_Rage();		//���ο� ���� ������ 
int F_Avalanche();			//���� ���� 

int F_E_Shot();				//������Ż��
int F_G_Falcon();			//�������
int F_B_Shot();				//����
int F_Perforation();		//�������̼�

//4�� ��ų
int F_Recall_Wolverin();    //���� �����
int F_Evasion_Mastery();    //�̺��̼� �����͸�
int F_Phoenix_Shot();       //�Ǵн� ��
int F_Force_Of_Nature();    //���� ���� ����




////////////////////////////��ī�ϼ� ��ų �������� 
//�ͽ�Ʈ�� ���� 
extern int E_Shield_BlockRate[10];
extern int E_Shield_UseTime[10];
extern int E_Shield_UseMana[10];

//��ī�� ��
extern int Mechanic_Bomb_Damage[10][2];
extern int Mechanic_Bomb_Attack_Range[10];
extern int Mechanic_Bomb_UseMana[10];

//��Ű�� ���� 
extern int P_Absorb[10][2];
extern int P_Absorb_UseTime[10];
extern int P_Absor_bUseMana[10];

//������ ��Ʈ����Ʈ 
extern int PlusPoison[10];

//�׷���Ʈ ���Ž� 
extern int G_Smash_Attack_Rate[10];
extern int G_Smash_Attack_Rage[10];
extern int G_Smash_UseMana[10];

//�ƽø����� 
extern int Maximize_Damage[10];
extern int Maximize_Weapon_Speed[10];
extern int Maximize_Time[10]; 
extern int Maximize_UseMana[10];

//������̼�  
extern int Automation_Damage[10];
extern int Automation_Speed[10];
extern int Automation_Time[10];
extern int Automation_UseMana[10];

//����ũ 
extern int Spark_Damage[10][2];
extern int Spark_Area[10];
extern int Spark_UseMana[10];

//��Ż �Ƹ�
extern int Metal_Armor_Defense[10];
extern int Metal_Armor_Time[10];
extern int Metal_Armor_UseMana[10];

//�׷��� ���޽�
extern int Grand_Smash_AttackRate[10];
extern int Grand_Smash_Damage[10];
extern int Grand_Smash_UseMana[10];

//��ī�� ���� �����͸�
extern int M_Weapon_Mastey[10];

//����ũ ���� 
extern int Spark_Shield_Damage[10];
extern int Spark_Shield_Defense[10];
extern int Spark_Shield_Time[10];
extern int Spark_Shield_UseMana[10];

//4�� ��ų
//���ø���
extern int Impulsion_LightingDamage[10][2];   //����Ʈ�� �߰� ���ݷ�
extern int Impulsion_Range[10];               //�����Ÿ�
extern int Impulsion_LightingNum[10];         //��Ʈ ����
extern int Impulsion_UseMana[10];

//���ø���
extern int Compulsion_AddAbsorb[10];		 //�߰� �����
extern int Compulsion_Area[10];              //�ֺ� ����
extern int Compulsion_Time[10];              //���� �ð�
extern int Compulsion_UseMana[10];

//�޳�ƽ �Ǿ�
extern int Magnetic_Sphere_AddDamage[10][2]; //�ڱ� ���ݷ� ����������
extern int Magnetic_Sphere_Area[10];		 //��ȿ�ݰ�
extern int Magnetic_Sphere_AttackDelay[10];  //���� ������
extern int Magnetic_Sphere_Time[10];         //���� Ÿ��
extern int Magnetic_Sphere_UseMana[10];

//��Ż ��
extern int Metal_Golem_Damage[10][2];      //���ݷ�
extern int Metal_Golem_Hit[10];			   //���߷�
extern int Metal_Golem_Defense[10];		   //����
extern int Metal_Golem_Life[10];           //�����
extern int Metal_Golem_UseMana[10];

////////////////////////////������ ��ų ��������
//�и� �����͸� 
extern int Melee_Mastery_DamagePercent[10];

//���̾� ��Ʈ����Ʈ 
extern int PlusFire[10];

//���̺� 
extern int Raving_Damage[10];
extern int Ravind_Speed[10];
extern float Raving_UseLife[10];
extern int Raving_UseMana[10];

//����Ʈ 
extern int Impact_Attack_Rating[10];
extern int Impact_Damage[10];
extern int Impact_UseMana[10];

//Ʈ���� ����Ʈ
extern int T_Impact_Damage[10];
extern int T_Impact_Hit[10];
extern int T_Impact_UseMana[10];

//�θ�Ż ���� 
extern int B_Swing_Damage[10];
extern int B_Swing_Critical[10];
extern int B_Swing_UseMana[10];

//�ξ�
extern int Roar_Range[10];
extern int Roar_Time[10];
extern int Roar_UseMana[10];

//������ ���� ��ũ�� 
extern int R_Zecram_Damage[10];
extern int R_Zecram_UseMana[10];

//����Ʈ���̼�
extern int Concentration_AttackRate[10];
extern int Concentration_Time[10];
extern int Concentration_UseMana[10];

//�¡ ũ����
extern int A_Crash_Damage[10];
extern int A_Crash_AttackRate[10];
extern int A_Crash_UseMana[10];

//������Ʈ ����
extern int Swift_Axe_Speed[10];
extern int Swift_Axe_Time[10];
extern int Swift_Axe_UseMana[10];

//�� ũ����
extern int B_Crash_Damage[10];
extern int B_Crash_DemonDamage[10];
extern int B_Crash_UseMana[10];


//**4�� ��ų
//���丮��
extern int Destoryer_DamagePercent[10];   //���ݷ� ����
extern int Destoryer_AddCritical[10];	   //ũ��Ƽ�� �߰�
extern int Destoryer_UseMana[10];

//����Ŀ
extern int Berserker_AddAttack[10];        //�߰� ���ݷ�
extern int Berserker_SubAbsorb[10];        //����� ����ġ
extern int Berserker_Time[10];             //�����ð�
extern int Berserker_UseMana[10];

//����Ŭ�� ��Ʈ����ũ
extern int Cyclone_Strike_DamagePercent[10];      //���ݷ� ����
extern int Cyclone_Strike_AreaDamage[10][2];      //���� ���ݷ�
extern int Cyclone_Strike_AttackNum[10];		  //�ִ� Ÿ�ݼ�
extern int Cyclone_Strike_Area[10];               //���� ��
extern int Cyclone_Strike_UseMana[10];

//��Ʈ �ｺ
extern int Boost_Health_Life[10];//����� ������ �߰�


///////////////////////////����ũ�� ��ų ��������
//����ũ ���� 
extern int Pike_Wind_Damage[10][2];
extern int Pike_Wind_Push_Lenght[10];
extern int Pike_Wind_Throw_Height[10];
extern int Pike_Wind_UseMana[10];

//���̽� ��Ʈ����Ʈ 
extern int PlusIce[10];

//ũ��Ƽ�� �� 
extern int Critical_Hit_Critical[10];
extern int Critical_Hit_UseMana[10];

//���� ũ���� 
extern int Jumping_Crash_UseMana[10];
extern int Jumping_Crash_Attack_Rating[10];
extern int Jumping_Crash_Damage[10];

//�׶��� ������ũ 
extern int G_Pike_Damage[10][2];
extern int G_Pike_Range[10];
extern int G_Pike_UseMana[10];
extern int G_Pike_Time[10];

//����̵� 
extern int Tornado_Damage[10];
extern int Tornado_Stun[10];
extern int Tornado_Range[10];
extern int Tornado_UseMana[10];

//���� ���潺 �����͸� 
extern int W_D_Mastery_Block[10];

//�� 
extern int Expansion_Size[10];
extern int Expansion_UseMana[10];
extern int Expansion_Damage[10];

//���� ���Ǿ�
extern int VenomSpear_Damage[10][2];
extern int VenomSpear_Num[10];
extern int VenomSpear_Time[10];
extern int VenomSpear_Range[10];
extern int VenomSpear_UseMana[10];

extern float Vanish_Speed[10];
extern int Vanish_Damage[10];
extern int Vanish_Range[10];
extern int Vanish_Time[10];
extern int Vanish_UseMana[10];

//���� �����͸�
extern int Critical_Mastery_Critical[10];

//ü�� ����
extern int Chain_Lance_Damage[10];
//extern int Chain_Lance_AttackRate[10];
extern int Chain_Lance_UseMana[10];

//**4����ų
//��ؽ� ����
extern int Assassin_Eye_AddCritical[10];       //ũ��Ƽ�� ���ݷ� ����
extern int Assassin_Eye_M_SubCritical[10];     //�� ���� ũ��Ƽ�� �ɷ� ��ȭ
extern int Assassin_Eye_Time[10];              //����Ÿ��
extern int Assassin_Eye_UseMana[10];

//��¡ ��Ʈ����ũ
extern int Charging_Strike_DamagePercent[10];        //���ݷ� ����
extern int Charging_Strike_CharingDamagePercent[10]; //��¡�� ���ݷ� ����
extern int Charging_Strike_Time[10];               //��¡ �ð�
extern int Charging_Strike_UseMana[10];

//���̱�
extern int Vague_EvasionPercent[10];  //�߰� ȸ��Ȯ�� �߰�
extern int Vague_Time[10];            //�����ð�
extern int Vague_UseMana[10];

//���Ϳ� ������
extern int Shadow_Master_DamagePercent[10];       //���ݷ� ����
extern int Shadow_Master_AddHit[10];              //���߷� ����
extern int Shadow_Master_ShadowNum[10];			  //�ܻ� �޺��� ����
extern int Shadow_Master_UseMana[10];


///////////////////////////���� ��ų ��������
//��ī��Ʈ ȣũ 
extern int Scout_Hawk_Attack_Rate[10];
extern int Scout_Hawk_UseTime[10];
extern int Scout_Hawk_UseMana[10];

//���� �����͸� 
extern int S_Mastery_DamagePercent[10];

//���� ���ο� 
extern int Wind_Arrow_Speed[10];
extern int Wind_Arrow_Damage[10];
extern int Wind_Arrow_UseMana[10];

//����Ʈ ���� 

extern int Perfect_Aim_Attack_Rate[10];
extern int Perfect_Aim_Damage[10];
extern int Perfect_Aim_Damage_LV[10];
extern int Perfect_Aim_UseMana[10];

//��½� ���� 
extern int D_Eye_Attack_Rate[10];

//���� 
extern int Falcon_Damage[10][2];
extern int Falcon_Time[10];
extern int Falcon_UseMana[10];

//���ο� ���� ������ 
extern int A_Rage_ArrowNum[10];
extern int A_Rage_Damage[10];
extern int A_Rage_Area[10];
extern int A_Rage_UseMana[10];

//������ü 
extern int Avalanche_Speed[10];
extern int Avalanche_Damage[10];
extern int Avalanche_ArrowNum[10];
extern int Avalanche_UseMana[10];

//������Ż ��
extern int Elemental_Shot_Ice[10][2];
extern int Elemental_Shot_Fire[10][2];
extern int Elemental_Shot_Lightning[10][2];
extern int Elemental_Shot_UseMana[10];

//��� ����
extern int   Golden_Falcon_Damage[10];
extern float Golden_Falcon_LifeRegen[10];
extern int   Golden_Falcon_Time[10];
extern int   Golden_Falcon_UseMana[10];

//�� ��
extern int BombShot_Damage[10];
extern int BombShot_DemonDamage[10];
extern int BombShot_AddDamage[10][2];
extern int BombShot_Area[10];
extern int BombShot_UseMana[10];

//�������̼�
extern int Perforation_Damage[10];
extern int Perforation_Critical[10];
extern int Perforation_UseMana[10];
extern int Perforation_Attack_Range[10];

//**4�� ��ų
//���� �����
extern int Recall_Wolverin_Damage[10][2];//���ݷ�
extern int Recall_Wolverin_Hit[10];      //���߷�
extern int Recall_Wolverin_Defense[10];  //����
extern int Recall_Wolverin_Life[10];     //�����
extern int Recall_Wolverin_UseMana[10];

//�̺��̼� �����͸�
extern int Evasion_Mastery_AddPercent[10];   //�߰� ȸ��  �ɷ� ����

//�Ǵн� ��
extern int   Phoenix_Shot_DamagePercent[10];      //���ݷ� ����
extern int   Phoenix_Shot_Time[10];               //��¡ �ð�
extern int   Phoenix_Shot_UseMana[10];

//���� ���� ����
extern int Force_Of_Nature_AddDamage[10];		  //�߰� ���ݷ�
extern int Force_Of_Nature_AddHit[10];			  //�߰� ���߷�
extern int Force_Of_Nature_FalconAddDamage[10];	  //������ �߰� ���ݷ�
extern int Force_Of_Nature_Time[10];              //�����ð�
extern int Force_Of_Nature_UseMana[10];



//��ų ����� ���Ǵ� �� 
extern int sinMasterSkill_Money[16];
extern int sinMasterSkill_PlusMoney[16];


// ��ų ����� �÷� ���� �ٲ۴� 
extern int SkillTextColor[10];

//�䱸ġ�� ������������쿡�� ȿ���� �ش� 
extern int NotUseSkillElement[3];	//�䱸ġ ���� �÷� 
extern int sinMasteryIncreaIndex[16];


//������ Ŭ����ų 
extern int C_Skill_HpPoint[10];      //hp ���
extern int C_Skill_AttPercent[10];   //���ݷ� ���
extern int C_Skill_EvaPercent[10];      //ȸ���� ���