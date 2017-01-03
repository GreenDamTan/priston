
#define SIN_SKILL_USE_RIGHT		0x01000000		//�����ʿ��� ��밡�� 
#define SIN_SKILL_USE_LEFT		0x02000000		//���ʿ��� ��밡�� 
#define SIN_SKILL_USE_ALL		0x03000000		//������ ���� �ƹ����̳� ��밡�� 
#define SIN_SKILL_USE_NOT		0x04000000		//����ϴ� ��ų�� �ƴ�(�нú�)

#include "Language\\Language.h"

#ifdef	_LANGUAGE_KOREAN

/*----------------------------------------------------------------------------*
*							�۽�ũ��
*-----------------------------------------------------------------------------*/	

//��ī�ϼ� 
{"�ͽ�Ʈ�� ����",		"���и� �ִ��� ������ �� �ɷ��� ��� ��Ų��",
10,		34,2,		110,4,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Extreme_Shield,SKILL_EXTREME_SHIELD,SIN_SKILL_USE_RIGHT,E_Shield_UseMana},

{"��ī�� ��",			"���ȭ ��ź�� ���� �ش� ������ ������ Ÿ���� �ش�",
12,		28,2,		73,3,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Mechanic_Bomb,SKILL_MECHANIC_BOMB,SIN_SKILL_USE_RIGHT,Mechanic_Bomb_UseMana},

{"������ ��Ʈ����",		"���� ���� �������� Ű�� ���������� �Ӽ��� ��½�Ų��",
14,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Poison_Attribute,SKILL_POISON_ATTRIBUTE,SIN_SKILL_USE_NOT,0},

{"������ �ۼҼ�",		"�Ͻ������� ��ü�� ���ȭ�Ͽ� ���� �ð����� ������� ��½�Ų��",
17,		30,2,		110,5,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Physical_Absorption,SKILL_PHYSICAL_ABSORB,SIN_SKILL_USE_RIGHT,P_Absor_bUseMana},

{"�׷���Ʈ ���Ž�",		"���� ��� ���� �ϰ��� ���Ѵ�",
20,		38,2,		73,3,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,0,0,0},
F_Great_Smash,SKILL_GREAT_SMASH,SIN_SKILL_USE_ALL,G_Smash_UseMana},

{"�ƽø�����",			"���⿡ ���� ����ȭ�� ���� �ð����� �ִ� ���ݷ��� ��½�Ų��",
23,		41,2,		110,4,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,sinWS1,sinWT1,0},
F_Maximize,SKILL_MAXIMIZE,SIN_SKILL_USE_RIGHT,Maximize_UseMana},

{"������̼�",			"�߻��� ���⸦ �ڵ�ȭ �Ͽ� �����ð� ���� ������ �ӵ��� ���ݷ��� ��½�Ų��",
26,		42,2,		110,4,
{0,0,0},			{sinWS1,sinWT1,0,0,0,0,0,0},
F_Automation,SKILL_AUTOMATION,SIN_SKILL_USE_RIGHT,Automation_UseMana},

{"����ũ",			"���� ���� �������� �������� ������ ������ �����Ѵ�",
30,		40,2,		82,2,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Spark,SKILL_SPARK,SIN_SKILL_USE_RIGHT,Spark_UseMana},

{"��Ż �Ƹ�",		"���� �ð����� ��ī�ϼ� Ư���� ������ ö��ȭ ���� ������ ��� ��Ų��",
40,		48,2,		110,5,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Metal_Armor,SKILL_METAL_ARMOR,SIN_SKILL_USE_RIGHT,Metal_Armor_UseMana},

{"�׷��� ���Ž�",	"������ ��Ÿ�������� ���߷��� ��½��� ������ ��Ÿ�� ���Ѵ�", // �庰 - ��ų�뷱�� ����(10.08.10)
43,		44,2,		82,2,
{0,0,0},			{sinWA1,sinWC1,sinWP1,sinWS2,sinWH1,0,0,0},
F_Grand_Smash,SKILL_GRAND_SMASH,SIN_SKILL_USE_RIGHT,Grand_Smash_UseMana},

{"��ī�п��ݸ����͸�",	"��ī�ϼ� Ưȭ ������ �ɷ��� ����ȭ ��Ų��",
46,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_M_W_Mastery,SKILL_MECHANIC_WEAPON,SIN_SKILL_USE_NOT,0},

{"����ũ ����",		"������ ����ũ�� �����ϴ� ���и� �����Ѵ�.",
50,		52,2,		110,2,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Spark_Shield,SKILL_SPARK_SHIELD,SIN_SKILL_USE_RIGHT,Spark_Shield_UseMana},

//[��ī�ϼ�]4�� ��ų
{"���޼�",		"���� ������ �帣�� �ϰ����� �ֺ����� �����Ѵ�.",
60,     	55,2,		130,0,
{1,0,0},            {sinWC1,sinWH1,sinWP1,sinWS2,0,0,0,0},
F_Impulsion,SKILL_IMPULSION,SIN_SKILL_USE_RIGHT,Impulsion_UseMana},

{"���޼�",		"���� �������� �ֺ������� �ڽſ��� ���������.",
63,     	65,2,		440,0,
{1,0,0},            {0,0,0,0,0,0,0,0},
F_Compulsion,SKILL_COMPULSION,SIN_SKILL_USE_RIGHT,Compulsion_UseMana},

{"���׳�ƽ ���Ǿ�",	"�� �ֺ��� ���� �ڱⰡ �帣�� ��ü�� �����Ѵ�.",
66,     	71,2,		600,0,
{1,0,0},            {0,0,0,0,0,0,0,0},
F_Magnetic_Sphere,SKILL_MAGNETIC_SPHERE,SIN_SKILL_USE_RIGHT,Magnetic_Sphere_UseMana},

{"��Ż ��",		"��ī�� ���� ��ȯ�Ѵ�.",
70,     	71,2,		1000,0,
{1,0,0},            {0,0,0,0,0,0,0,0},
F_Metal_Golem,SKILL_METAL_GOLEM,SIN_SKILL_USE_RIGHT,Metal_Golem_UseMana},


//������ 
{"�᷹�� �����͸�",		"���� ���⿡ ���� �Ʒ����� ���������� ���ݴɷ��� ��� ��Ų��",
10,		0,0,		0,0,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWS2,0,0,0,0},
F_Melee_Mastery,SKILL_MELEE_MASTERY,SIN_SKILL_USE_NOT,0},

{"���̾� ��Ʈ����",		"�ҿ� ���� �������� Ű�� ���������� �Ӽ��� ��½�Ų��",
12,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Fire_Attribute,SKILL_FIRE_ATTRIBUTE,SIN_SKILL_USE_NOT,0},

{"���̺�",			"ª�� �ð� ���л��°� �Ǿ� ������ ������ �Ѵ�. �׷��� ���ݽ� ������� �����Ѵ�.",
14,		35,2,		93,3,
{0,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,0,0,0},
F_Raving,SKILL_RAVING,SIN_SKILL_USE_ALL,Raving_UseMana},

{"����Ʈ",			"������ ��Ÿ �������� ���߷��� ��½��� ������ ���� Ÿ���� ���Ѵ�",
17,		37,2,		86,4,
{0,0,0},			{sinWA1,sinWC1,sinWP1,sinWS2,0,0,0,0},
F_Impact,SKILL_IMPACT,SIN_SKILL_USE_ALL,Impact_UseMana},

{"Ʈ���� ����Ʈ",		"������ ��Ÿ�� ������ ������ �Ѵ�",
20,		39,2,		80,4,
{0,0,0},			{sinWA1,sinWC1,sinWP1,sinWS2,0,0,0,0},
F_Triple_Impact,SKILL_TRIPLE_IMPACT,SIN_SKILL_USE_ALL,T_Impact_UseMana},

{"�θ�Ʋ ����",			"������ �������� ������ ġ��Ÿ�� ���Ѵ�",
23,		41,2,		82,2,
{0,0,0},			{sinWA1,sinWS2,0,0,0,0,0,0},
F_Brutal_Swing,SKILL_BRUTAL_SWING,SIN_SKILL_USE_ALL,B_Swing_UseMana},

{"�ξ�",			"���� �⸦ �����Ͽ� �Ͻ������� ������ ȥ���� ���߸���",
26,		39,2,		112,4,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Roar,SKILL_ROAR,SIN_SKILL_USE_RIGHT,Roar_UseMana},

{"������ of ��ũ��",		"���� ��ũ���� ����� ����Ͽ� ������ ȭ���� Ÿ���� ���Ѵ�",
30,		45,2,		84,4,
{0,0,0},			{sinWA1,sinWH1,sinWS2,0,0,0,0,0},
F_RageOfZecram,SKILL_RAGE_OF_ZECRAM,SIN_SKILL_USE_RIGHT,R_Zecram_UseMana},

{"����Ʈ���̼�",	"���� ���߷��� �����Ͽ� �����ð����� ���߷��� ��½�Ų��",
40,		53,2,		110,3,
{0,0,0},			{sinWA1,sinWS2,0,0,0,0,0,0},
F_Concentration,SKILL_CONCENTRATION,SIN_SKILL_USE_RIGHT,Concentration_UseMana},

{"�¡ ũ����",	"������ ��Ÿ�� ������ ��Ÿ �������� ������ ���ظ� ������",
43,		48,2,		73,3,
{0,0,0},			{sinWA1,sinWP1,sinWS2,0,0,0,0,0},
F_Avanging_Crash,SKILL_AVANGING_CRASH,SIN_SKILL_USE_RIGHT,A_Crash_UseMana},

{"������Ʈ ����",	"������ ����� �ɼ��ϰ� �Ͽ� �����ð����� ���ݼӵ��� ����Ų��",
46,		53,2,		200,3,
{0,0,0},			{sinWA1,0,0,0,0,0,0,0},
F_Swift_Axe,SKILL_SWIFT_AXE,SIN_SKILL_USE_RIGHT,Swift_Axe_UseMana},

{"�� ũ����",		"����ε��� ����� ����ġ�� ����� ������ ȸ�� ��Ÿ�� ���Ѵ�",
50,		54,2,		73,3,
{0,0,0},			{sinWA1,sinWP1,sinWS2,0,0,0,0,0},
F_Bone_Crash,SKILL_BONE_CRASH,SIN_SKILL_USE_RIGHT,B_Crash_UseMana},

//[������]4�� ��ų
{"��Ʈ���̾�",	"���� �ĸ��Ű�� ������ ��Ÿ������ ���Ѵ�.",
60,		45,2,		150,0,
{1,0,0},			{sinWA1,sinWH1,0,0,0,0,0,0},
F_Destoryer,SKILL_DETORYER,SIN_SKILL_USE_ALL,Destoryer_UseMana},

{"����Ŀ",		"�����θ� ���� �Ҽ� ���� ������ ������ ���·� �����Ѵ�.������ ������� �ص��� �϶��Ѵ�",
63,		60,2,		700,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Berserker,SKILL_BERSERKER,SIN_SKILL_USE_RIGHT,Berserker_UseMana},

{"����Ŭ�� ��Ʈ����ũ",	"������ ȸ���������� �ټ��� ���� Ÿ���Ѵ�.",
66,		55,2,		135,0,
{1,0,0},			{sinWA1,sinWH1,sinWP1,sinWS2,0,0,0,0},
F_Cyclone_Strike,SKILL_CYCLONE_STRIKE,SIN_SKILL_USE_RIGHT,Cyclone_Strike_UseMana},

{"�ν�Ʈ �ｺ",	"������� ���������� ���� ��Ų��.",
70,		0,0,		0,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Boost_Health,SKILL_BOOST_HEALTH,SIN_SKILL_USE_NOT,},

//����ũ�� 
{"����ũ ����",			"â�� ���ϰ� ���� ���� �ٶ����� ���� �о�� �����Ѵ�",
10,		27,2,		80,4,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Pike_Wind,SKILL_PIKE_WIND,SIN_SKILL_USE_RIGHT,Pike_Wind_UseMana},

{"���̽� ��Ƽ����",		"���迡 ���� �������� Ű�� ���������� �Ӽ��� ��½�Ų��",
12,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Ice_Attribute,SKILL_ICE_ATTRIBUTE,SIN_SKILL_USE_NOT,0},

{"ũ��Ƽ�� ��",			"���� ġ������ �κ��� ��Ÿ�Ͽ� ũ��Ƽ�� ������ �� Ȯ���� ��½�Ų��",
14,		31,2,		84,3,
{0,0,0},			{sinWP1,sinWC1,sinWH1,sinWS2,0,0,0,0},
F_Critical_Hit,SKILL_CRITICAL_HIT,SIN_SKILL_USE_ALL,Critical_Hit_UseMana},

{"���� ũ����",			"���� ������ �پ� �ö� ������ ������ �ϰ��� ���Ѵ�",
17,		34,2,		76,4,
{0,0,0},			{sinWA1,sinWP1,sinWS2,0,0,0,0,0},
F_Jumping_Crash,SKILL_JUMPING_CRASH,SIN_SKILL_USE_ALL,Jumping_Crash_UseMana},

{"�׶��� ����ũ",		"���⸦ ���ϰ� ���� �ļ� ������ ������ ������ �Ѵ�",
20,		36,2,		84,4,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Ground_Pike,SKILL_GROUND_PIKE,SIN_SKILL_USE_RIGHT,G_Pike_UseMana},

{"����̵�",			"������ ���鿡�� ���� ȸ�� ������ ���Ѵ�",
23,		38,2,		83,4,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Tornado_Pike,SKILL_TORNADO,SIN_SKILL_USE_RIGHT,Tornado_UseMana},

{"���� ���潺 �����͸�",	"��� ����� ������ ���⸦ �̿��� ��� �ɷ��� ���������� ��½�Ų��.",
26,		0,0,		0,0,
{0,0,0},			{sinWA1,sinWH1,sinWP1,sinWS2,0,0,0,0},
F_Weapone_Defence_Mastery,SKILL_WEAPONE_DEFENCE_MASTERY,SIN_SKILL_USE_NOT,0},

{"�ͽ��Ҽ�",			"����� �⸦ �����Ͽ� �Ŵ�ȭ �� �� ���� ������ ���Ѵ�",
30,		42,2,		82,4,
{0,0,0},			{sinWA1,sinWH1,sinWP1,0,0,0,0,0},
F_Expansion,SKILL_EXPANSION,SIN_SKILL_USE_ALL,Expansion_UseMana},

{"���� ���Ǿ�",			"���� ���ŷ����� â�� �ٴڿ� ��� �ټ��� ���� ���� ���Ӽ��� ������ ���Ѵ�.",
40,		50,2,		110,2,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Venom_Spear,SKILL_VENOM_SPEAR,SIN_SKILL_USE_RIGHT,VenomSpear_UseMana},

{"��Ͻ�",			"�������� ����� �帮�� �Ͽ� ���� ���������� �������� ���ϰ� �Ѵ�.",
43,		53,2,		140,2,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Vanish,SKILL_VANISH,SIN_SKILL_USE_RIGHT,Vanish_UseMana},

{"ũ��Ƽ�� �����͸�",		"â�� ����Կ� �־� ũ��Ƽ�� ������ �� Ȯ���� ���������� ��� �����ش�",
46,		0,0,		0,0,
{0,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Critical_Mastery,SKILL_CRITICAL_MASTERY,SIN_SKILL_USE_NOT,0},

{"ä�� ����",	  		  "�����ϰ� ġ������ ȸ�� ���� ������ ���Ѵ�",
50,		51,2,		88,3,
{0,0,0},			{sinWC1,sinWP1,sinWS2,0,0,0,0,0},
F_Chain_Lance,SKILL_CHAIN_LANCE,SIN_SKILL_USE_RIGHT,Chain_Lance_UseMana},

//[����ũ��]4�� ��ų
{"��ؽ� ����",		"������ ������ �ľ��Ͽ�, ũ��Ƽ�� ������ �߻��� Ȯ���� �÷��ش�.",
60,		61,2,		520,0,
{1,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Assassin_Eye,SKILL_ASSASSIN_EYE,SIN_SKILL_USE_RIGHT,Assassin_Eye_UseMana},

{"��¡ ��Ʈ����ũ",	 "�⸦ ��� ġ������ ���Ӱ����� ���Ѵ�.",
63,		75,2,		150,0,
{1,0,0},			{sinWP1,sinWS2,0,0,0,0,0,0},
F_Charging_Strike,SKILL_CHARGING_STRIKE,SIN_SKILL_USE_RIGHT,Charging_Strike_UseMana},

{"���̱�",		"�ڽ��� ��ü�� �˾ƺ��� ����� �Ͽ� ȸ�Ƿ��� ��� ��Ų��.",
66,		71,2,		650,0,
{1,0,0},			{sinWP1,0,0,0,0,0,0,0},
F_Vague,SKILL_VAGUE,SIN_SKILL_USE_RIGHT,Vague_UseMana},

{"������ ������",	"�ſ� ���� �ӵ��� �������� ������ ���Ѵ�.",
70,		51,2,		160,0,
{1,0,0},			{sinWP1,sinWS2,0,0,0,0,0,0},
F_Shadow_Master,SKILL_SHADOW_MASTER,SIN_SKILL_USE_RIGHT,Shadow_Master_UseMana},



//���� 
{"��ī�� ȣũ",			"���� ���ŷ°� �ڿ��� ������ ������� �Ÿ� ��ȯ, �þ߸� Ȯ���� ���� �ð� ���߷��� ��½�Ų��",
10,		25,2,		115,5,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Scout_Hawk ,SKILL_SCOUT_HAWK,SIN_SKILL_USE_RIGHT,Scout_Hawk_UseMana },

{"���� �����͸�",		"Ȱ�迭 ���⿡ ���� �Ʒ����� ���ݷ��� ���������� ��½�Ų��",
12,		0,0,		0,0,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Shooting_Mastery,SKILL_SHOOTING_MASTERY,SIN_SKILL_USE_NOT,0},

{"���� �ַο�",			"�ٶ��� ���� ���� Ȱ�� �߻��Ͽ� ���ݷ��� ��½�Ų��",
14,		25,2,		82,3,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Wind_Arrow,SKILL_WIND_ARROW,SIN_SKILL_USE_ALL,Wind_Arrow_UseMana},

{"����Ʈ ����",			"������ �������� ���߷°� ���ݷ��� ��½��� ���� ��Ȯ�ϰ� Ÿ���Ѵ�",
17,		29,2,		81,3,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Perfect_Aim,SKILL_PERFECT_AIM,SIN_SKILL_USE_ALL,Perfect_Aim_UseMana},


{"��½� ����",			"�������� �ü� ���ó�� ������ ���߷��� �ش�ȭ ��Ų��",
20,		0,0,		0,0,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Dions_Eye,SKILL_DIONS_EYE,SIN_SKILL_USE_NOT,0},

{"���Ǹ� ����",			"������ ��� ������� ���ݼ����� �Ÿ� ��ȯ�Ѵ�",
23,		36,2,		115,4,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Falcon ,SKILL_FALCON,SIN_SKILL_USE_RIGHT,Falcon_UseMana },

{"�ַο� of ������",		"�ϴ÷� Ȱ�� �� �÷� ������ ��ġ�� ������� ������ ���Ѵ�",
26,		38,2,		82,4,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Arrow_of_Rage,SKILL_ARROW_OF_RAGE,SIN_SKILL_USE_RIGHT,A_Rage_UseMana},

{"�ֹ�����",			"������ ������ ���������� ȭ���� �߻��Ѵ�",
30,		41,2,		103,2,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Avalanche,SKILL_AVALANCHE,SIN_SKILL_USE_RIGHT,Avalanche_UseMana},

{"������Ż ��",		"���ҿ� ���յ� �����ϰ� ���� ȭ���� �߻��Ѵ�.",
40,		43,2,		82,2,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_E_Shot,SKILL_ELEMENTAL_SHOT,SIN_SKILL_USE_RIGHT,Elemental_Shot_UseMana},

{"�� ����",		"���ݰ� ġ���� �ɷ��� ���� ���� ��ȭ�� ������ ��ȯ�Ѵ�.",
43,		52,2,		120,4,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_G_Falcon,SKILL_GOLDEN_FALCON,SIN_SKILL_USE_RIGHT,Golden_Falcon_UseMana},

{"�� ��",		"���߼� ȭ���� �������� �߻��Ͽ� ���������� ���ظ� ������",
46,		46,2,		87,3,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_B_Shot,SKILL_BOMB_SHOT,SIN_SKILL_USE_RIGHT,BombShot_UseMana},

{"�������̼�",		"��Ȯ�ϰ� ������ �������� ���� �����ϴ� ġ������ ������ ���Ѵ�",
50,		48,2,		82,2,
{0,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Perforation,SKILL_PERFORATION,SIN_SKILL_USE_RIGHT,Perforation_UseMana},

//[����]4�� ��ų
{"���� �����",		"��ȯ�ڸ� ������ ������� ��ȯ�Ѵ�.",
60,		58,2,		900,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Recall_Wolverin,SKILL_RECALL_WOLVERIN,SIN_SKILL_USE_RIGHT,Recall_Wolverin_UseMana},

{"�̺��̼� �����͸�",	"���� ������ ȿ�������� ȸ���Ѵ�.",
63,		0,0,		0,0,
{1,0,0},			{0,0,0,0,0,0,0,0},
F_Evasion_Mastery,SKILL_EVASION_MASTERY,SIN_SKILL_USE_NOT,0},

{"�Ǵн�  ��",		"�Ǵн��� ����� ���� ������ ������ ���Ѵ�.",
66,		55,2,		100,0,
{1,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Phoenix_Shot,SKILL_PHOENIX_SHOT,SIN_SKILL_USE_RIGHT,Phoenix_Shot_UseMana},

{"���� ���� ����ó",	"�ڿ��� ������ ��ȭ�� ĳ������ �ɷ��� �ش�ȭ ��Ų��.",
70,		56,2,		900,0,
{1,0,0},			{sinWS1,0,0,0,0,0,0,0},
F_Force_Of_Nature,SKILL_FORCE_OF_NATURE,SIN_SKILL_USE_RIGHT,Force_Of_Nature_UseMana},


/*----------------------------------------------------------------------------*
*							����̿�
*-----------------------------------------------------------------------------*/	
/////// ����Ʈ
{"�ҵ� ����Ʈ",		"������ �˱⸦ ���� ���� �����Ѵ�.",
10,		22,2,		82,2,
{0,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Sword_Blast,SKILL_SWORD_BLAST,SIN_SKILL_USE_ALL,Sword_Blast_UseMana},

{"Ȧ�� �ٵ�",			"��ü�� �ż��ϰ� �Ͽ� �𵥵�迭�� ������ ����Ѵ�.",
12,		28,2,		110,3,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_HolyBody ,SKILL_HOLY_BODY,SIN_SKILL_USE_RIGHT,HolyBody_UseMana },

{"������ Ʈ���̴�",		"������ �Ʒ����� �ٷ��� ���������� ���� ��Ų��.",
14,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Physical_Traning,SKILL_PHYSICAL_TRANING,SIN_SKILL_USE_NOT,0},

{"���� ũ����",			"ġ�����̰� ������ ��Ÿ �������� ���� �����Ѵ�.",
17,		35,2,		82,2,
{0,0,0},			{sinWA1,sinWC1,sinWS2,0,0,0,0,0},
F_Double_Crash,SKILL_DOUBLE_CRASH,SIN_SKILL_USE_RIGHT,D_Crash_UseMana},

{"Ȧ�� ����",			"�ż��� ���� �ο��Ͽ�, ��Ƽ����� �Բ� �𵥵� ���� ���� ���ݷ��� ��ȭ�Ѵ�.",
20,		36,2,		100,3,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Holy_Valor,SKILL_HOLY_VALOR,SIN_SKILL_USE_RIGHT,Holy_Valor_Mana},

{"�귣��",			"�˻��� ��������� ���� ������ ���� ������ �ϰ��� ���Ѵ�.",
23,		38,2,		82,2,
{0,0,0},			{sinWP1,sinWS2,0,0,0,0,0,0},
F_Brandish,SKILL_BRANDISH,SIN_SKILL_USE_ALL,Brandish_Mana},

{"�Ǿ��",			"�������� ������ �˱�� �������� �����ϴ� ������ ���Ѵ�.",
26,		41,2,		82,2,
{0,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Piercing,SKILL_PIERCING,SIN_SKILL_USE_ALL,Piercing_Mana},

{"�巡��ƽ ���Ǹ�",		"�ȶ���� ������ ���ŷ����� ������ �ð����� ������ ��� ��Ų��.",
30,		44,2,		100,2,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Drastic_Spirit,SKILL_DRASTIC_SPIRIT,SIN_SKILL_USE_RIGHT,Drastic_Spirit_Mana},


{"�ҵ� �����͸�",	"�˿� ���� �Ʒ����� ���� ���ݴɷ��� ���������� ��� ��Ų��.",
40,		0,0,		0,0,
{0,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_S_Mastery,SKILL_SWORD_MASTERY,SIN_SKILL_USE_NOT,0},

{"����� ����",		"�ż� ���и� ����� ������ ����Ų �� �𵥵� ���� ������ ����Ͽ� �ڽ��� �������� ȯ����Ų��.",
43,		46,2,		130,1,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_D_Inhalation,SKILL_DIVINE_INHALATION,SIN_SKILL_USE_RIGHT,D_Inhalation_UseMana},

{"Ȧ�� �������̼�",	"�ż��� �ֹ��� �ܿ� �𵥵� ���͸� ��Ȥ�Ѵ�.",
46,		50,2,		120,1,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Holy_Incantation,SKILL_HOLY_INCANTATION,SIN_SKILL_USE_RIGHT,H_Incantation_UseMana},

{"�׷��� ũ�ν�",	"������� �̸����� ���� ����ϴ� ��Ÿ�� ���Ѵ�.",
50,		45,2,		82,2,
{0,0,0},			{sinWH1,sinWS2,0,0,0,0,0,0},
F_Grand_Cross,SKILL_GRAND_CROSS,SIN_SKILL_USE_RIGHT,Grand_Cross_UseMana},

//[����Ʈ]4�� ��ų
{"����� �Ǿ��",	"���� ���� �޾� ������ �˹��� ���� �����Ѵ�.",
70,		51,2,		160,0,
{1,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Divine_Piercing,SKILL_DIVINE_PIERCING,SIN_SKILL_USE_RIGHT,Divine_Piercing_UseMana},

{"���鸮 ����",	"�ſ��� ���ѹ��� �ż��� ���и� �����Ѵ�.",
63,		50,2,		400,0,
{1,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Godly_Shield,SKILL_GODLY_SHIELD,SIN_SKILL_USE_RIGHT,Godly_Shield_UseMana},

{"���� ����",	"���� �������� �������� ���� �޾� ����� �ְ��� ������ �����Ѵ�.",
66,		51,2,		330,0,
{1,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS2,sinWS1,sinWT1,0},
F_God_Bless,SKILL_GOD_BLESS,SIN_SKILL_USE_RIGHT,God_Bless_UseMana},

{"�ҵ� ���� ����Ƽ��",	"�������� �Ŵ��� �ϰ��� ���Ѵ�.",
60,		53,2,		250,0,
{1,0,0},			{sinWS2,0,0,0,0,0,0,0},
F_Sword_Of_Justice,SKILL_SWORD_OF_JUSTICE,SIN_SKILL_USE_RIGHT,Sword_Of_Justice_UseMana},
/////////// ��Ż��Ÿ
{"���� ��Ʈ����ũ",		"���и� ���� ���� �����ϴ� ������ ������ ���Ѵ�.",
10,		23,2,		82,2,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_S_Strike ,SKILL_SHIELD_STRIKE,SIN_SKILL_USE_RIGHT,S_Strike_UseMana },

{"�۸���",			"�ܻ��� ������ ����â�� ��ô�Ѵ�",
12,		26,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Farina,SKILL_FARINA,SIN_SKILL_USE_ALL,Farina_UseMana},

{"������ �����͸�",		"��â�迭 ���⿡ ���� �Ʒ����� ���ݷ��� ���������� ��½�Ų��.",
14,		0,0,		0,0,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_D_Mastery,SKILL_THROWING_MASTERY,SIN_SKILL_USE_NOT,0},

{"��� ���Ǿ�",			"��â�� �⸦ �ο��� �����Ѵ�.",
17,		32,2,		82,2,
{0,0,0},			{sinWP1,sinWT1,0,0,0,0,0,0},
F_V_Spear,SKILL_VIGOR_SPEAR,SIN_SKILL_USE_ALL,V_Spear_UseMana},

{"����",			"��â�� �ٶ�ó�� ������ ����� ��Ȯ�ϰ� �ָ� ������ �� �ֵ��� �Ѵ�.",
20,		34,2,		100,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Windy,SKILL_WINDY,SIN_SKILL_USE_RIGHT,Windy_Mana},

{"Ʈ����Ʈ ���",		"��â�� ���ϰ� ȸ������ ��Ȯ�ϰ� ������ ������ ���Ѵ�.",
23,		36,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Twist_Javelin,SKILL_TWIST_JAVELIN,SIN_SKILL_USE_RIGHT,Twist_Javelin_Mana},

{"��� ��Ŀ",			"���п� ������ ������ ���� ������� ����� ������.",
26,		42,2,		110,2,
{0,0,0},			{sinDS1,0,0,0,0,0,0,0},
F_Soul_Sucker,SKILL_SOUL_SUCKER,SIN_SKILL_USE_RIGHT,Soul_Sucker_Mana},

{"���̾� ���",		"���� �Ӽ��� �߰��� ��â���� ���� ���¿��.",
30,		40,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Fire_Javelin,SKILL_FIRE_JAVELIN,SIN_SKILL_USE_ALL,Fire_Javelin_Mana},

{"���ø� ���",	"�ſ� ���� �ӵ��� ������� ���� ������ ���Ѵ�.",
40,		43,2,		115,1,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Split_Javelin,SKILL_SPLIT_JAVELIN,SIN_SKILL_USE_RIGHT,Split_Javelin_UseMana},

{"Ʈ���̾��� ���� ���Ҷ�",	"������ �������� �ڽ��̳� �Ʊ��� ��⸦ �÷� ���ݷ��� ��� �����ش�.",
43,		50,2,		120,1,
{0,0,0},			{sinWT1,sinWS2,sinWS1,sinWP1,sinWH1,sinWA1,sinWC1,sinWM1},
F_T_Of_Valhalla,SKILL_TRIUMPH_OF_VALHALLA,SIN_SKILL_USE_RIGHT,T_Of_Valhalla_UseMana},

{"����Ʈ�� ���",	"������ �� �Ӽ��� ��â���� ���� �����Ѵ�.",
46,		46,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Lightning_Javelin,SKILL_LIGHTNING_JAVELIN,SIN_SKILL_USE_ALL,Lightning_Javelin_UseMana},

{"���� ���",	    "���� ��ǳ�� ����Ű�� â�� ���������� ��ô�Ѵ�.",
50,		48,2,		82,2,
{0,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Storm_Javelin,SKILL_STORM_JAVELIN,SIN_SKILL_USE_RIGHT,Storm_Javelin_UseMana},

//[��Ʋ��Ÿ]4�� ��ų
{"Ȧ ���� ���Ҷ�",	"���Ҷ��� ����� ȸ�Ǵɷ��� ���� ��Ų��.",
60,		52,2,		750,0,
{1,0,0},			{sinWA1,sinWC1,sinWH1,sinWP1,sinWS1,sinWT1,0,0},
F_Hall_Of_Valhalla,SKILL_HALL_OF_VALHALLA,SIN_SKILL_USE_RIGHT,Hall_Of_Valhalla_UseMana},

{"�ͽ�Ʈ�� ������",	"���� �پ� �ö� �г��� ����� ��� ������ ���� ������ ������ ���Ѵ�.",
63,		48,2,		210,0,
{1,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_X_Rage,SKILL_X_RAGE,SIN_SKILL_USE_RIGHT,X_Rage_UseMana},

{"���ν�Ʈ ���",	"��â ���� �����ð����� ���� �Ӽ� �ɷ��� �ο��Ѵ�.",
66,		50,2,		720,0,
{1,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Frost_Javelin,SKILL_FROST_JAVELIN,SIN_SKILL_USE_RIGHT,Frost_Javelin_UseMana},

{"������",	"������ ������ ���Ӱ����� ���Ѵ�.", // �庰 - ��ų�뷱�� ����(10.08.10) 55 -> 45
70,		45,2,		100,0,
{1,0,0},			{sinWT1,0,0,0,0,0,0,0},
F_Vengeance,SKILL_VENGEANCE,SIN_SKILL_USE_RIGHT,Vengeance_UseMana},

///////////������Ƽ��
{"����",			"�ż��� �������� ������� ȸ�� �����ش�.",
10,		15,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Healing,SKILL_HEALING,SIN_SKILL_USE_RIGHT,Healing_UseMana},

{"Ȧ�� ��Ʈ",			"�ż��� �� ����� �߻��Ѵ�. (�𵥵� ������ 100% ���ߵǸ�, ���ŷ¿� ����Ͽ� ���ݷ� ����)",
12,		11,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_HolyBolt,SKILL_HOLY_BOLT,SIN_SKILL_USE_ALL,HolyBolt_UseMana},

{"��Ƽ ����ũ",			"�������� ������� �������� ���� �����Ѵ�.",
14,		14,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_M_Spark,SKILL_MULTISPARK,SIN_SKILL_USE_RIGHT,M_Spark_UseMana},

{"Ȧ�� ���ε�",			"�ż��� �⵵�� ���� ���ݷ��� ��ȭ��Ų��.",
17,		19,1,		100,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_HolyMind,SKILL_HOLY_MIND,SIN_SKILL_USE_RIGHT,HolyMind_UseMana},

{"�޵����̼�",			"������ ���� ��� ȸ�� �ɷ��� ���������� ��� ��Ų��.",
20,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Meditation,SKILL_MEDITATION,SIN_SKILL_USE_NOT,0},

{"����� ����Ʈ��",		"�ż��� ������ ��ȯ�Ǿ� ������ �Ӹ����� �۷��Ѵ�. (���ŷ¿� ����Ͽ� ���ݷ� ����)",
23,		21,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Divine_Lightning,SKILL_DIVINE_LIGHTNING,SIN_SKILL_USE_RIGHT,Divine_Lightning_Mana},

{"Ȧ�� ���÷���",		"�ż��� ���� �����Ͽ�, �𵥵��� ������ �� �̿��Ѵ�.",
26,		23,1,		120,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Holy_Reflection,SKILL_HOLY_REFLECTION,SIN_SKILL_USE_RIGHT,Holy_Reflection_Mana},

{"�׷��� ����",			"���� �������� ��Ƽ������ ������� ȸ������ �ش�.",
30,		27,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Grand_Healing,SKILL_GRAND_HEALING,SIN_SKILL_USE_RIGHT,Grand_Healing_Mana},

{"��� ��",			"���� ���ŷ����� ���� ������ ����� ���������� �߻��Ѵ�.",
40,		24,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Vigor_Ball,SKILL_VIGOR_BALL,SIN_SKILL_USE_ALL,Vigor_Ball_UseMana},

{"��������",		"������ ������ ����� ���Ḧ �һ���Ų��.",
43,		31,1,		130,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Resurrection,SKILL_RESURRECTION,SIN_SKILL_USE_RIGHT,Resurrection_UseMana},

{"�ͽ��ü�",		"�������� �ֹ����� �𵥵� ������ ������ �Ϻθ� ������ �Ҹ��Ų��.",
46,		28,1,		120,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Extinction,SKILL_EXTINCTION,SIN_SKILL_USE_RIGHT,Extinction_UseMana},

{"����� ������",	"���� �������� ������� �Ͻ������� ��� �����ش�.",
50,		31,1,		134,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Virtual_Life,SKILL_VIRTUAL_LIFE,SIN_SKILL_USE_RIGHT,Virtual_Life_UseMana},

//[������Ƽ��]4�� ��ų
{"�۷��� ������ũ",	"�Ŵ��� ���� �������� ������ ���� ġ�ھ� �ö� Ŀ�ٶ� ���ظ� ���Ѵ�.",
60,		35,1,		120,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Glacial_Spike,SKILL_GLACIAL_SPIKE,SIN_SKILL_USE_RIGHT,Glacial_Spike_UseMana},

{"�����ʷ��̼� �ʵ�",	"�ż��� �⵵�� ����°� ����� ����ϸ�, �ֺ��� �ִ� �ڵ��� 50% ȸ������ �ش�.",
63,		41,1,		950,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Regeneration_Field,SKILL_REGENERATION_FIELD,SIN_SKILL_USE_RIGHT,Regeneration_Field_UseMana},

{"ü�� ����Ʈ��",	"������ �����Ͽ� �����ϴ� ������ ���� ������ ���Ѵ�.",
66,		41,1,		120,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Chain_Lightning,SKILL_CHAIN_LIGHTNING,SIN_SKILL_USE_RIGHT,Chain_Lightning_UseMana},

{"���� ������",		"�Ű��� ��ȣ�ڸ� ��ȯ�Ѵ�.",
70,		47,1,		800,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Summon_Muspell,SKILL_SUMMON_MUSPELL,SIN_SKILL_USE_RIGHT,Summon_Muspell_UseMana},


///////////������
{"�ְŴ�",			"�������� ��ü�� ����Ͽ� ����� ������Ų��.",
10,		12,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Agony,SKILL_AGONY,SIN_SKILL_USE_RIGHT,Agony_UseMana},

{"���̾� ��Ʈ",			"�������� ������ ���� �ҵ���� ���� �����Ѵ�.",
12,		11,1,		84,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_FireBolt,SKILL_FIRE_BOLT,SIN_SKILL_USE_ALL,FireBolt_UseMana},

{"���Ͼ�",			"������ ���ŷ����� ���� �ð����� ���ҼӼ��� ��½�Ų��.",
14,		15,1,		100,3,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Zenith,SKILL_ZENITH,SIN_SKILL_USE_RIGHT,Zenith_UseMana},

{"���̾� ��",			"������ ������ �ҵ���� �������� ������ �����Ѵ�. (���ŷ¿� ����Ͽ� ���ݷ� ����)",
17,		19,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_FireBall,SKILL_FIRE_BALL,SIN_SKILL_USE_RIGHT,FireBall_UseMana},

{"��Ż �����͸�",		"����ε��� ���� ���ŷ��� �̾�޴� ������ �������� ���������� ����� ��½�Ų��.",
20,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
F_Mental_Mastery,SKILL_MENTAL_MASTERY,SIN_SKILL_USE_NOT,0},

{"������̵�",			"������ ������ ������ ���� �ٶ��� ���� ���� ������� ������ ��Ÿ�Ѵ�. (���ŷ¿� ����Ͽ� ���ݷ� ����)",
23,		21,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Watornado,SKILL_WATORNADO,SIN_SKILL_USE_RIGHT,Watornado_Mana},

{"��þƮ ����",			"����ϰ� �ִ� ��� ������ ���� ���� ���� ������ ���� �����ϰ� �Ѵ�.",
26,		28,1,		104,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Enchant_Weapon,SKILL_ENCHANT_WEAPON,SIN_SKILL_USE_RIGHT,Enchant_Weapon_Mana},

{"���� ����",			"�չٴڿ��� ������ �θ��� ���� �߻� �Ѵ�. (���ŷ¿� ����Ͽ� ���ݷ� ����)",
30,		26,1,		82,2,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Dead_Ray,SKILL_DEAD_RAY,SIN_SKILL_USE_ALL,Dead_Ray_Mana},

{"������ ����",			"���� ���� �������� ���� ������ ����ϴ� ���� �����.",
40,		40,1,		150,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Energy_Shield,SKILL_ENERGY_SHIELD,SIN_SKILL_USE_RIGHT,Energy_Shield_UseMana},

{"��ƽ�Ʈ������",		"������ ������Ű�� ������ �������� ��ȿ �ݰ泻�� ���鿡�� ������ ������ ���Ѵ�",
43,		33,1,		106,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Diastrophism,SKILL_DIASTROPHISM,SIN_SKILL_USE_RIGHT,Diastrophism_UseMana},

{"���Ǹ� ������Ż",		"������ ������ ��ȯ�� ���� ���ݴɷ��� ������Ų��",
46,		35,1,		150,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Spirit_Elemental,SKILL_SPIRIT_ELEMENTAL,SIN_SKILL_USE_RIGHT,Spirit_Elemental_UseMana},

{"��� �ҵ�",			"������ ���� ���� Į�� ȯ���� ��ȯ�� ���� �����Ѵ�",
50,		30,1,		150,1,
{0,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_D_Sword,SKILL_DANCING_SWORD,SIN_SKILL_USE_RIGHT,Dancing_Sword_UseMana},

//[������]4�� ��ų
{"���̾� ������Ż",	"ȭ������ ������� ������ ������Ż�� ��ȯ�Ѵ�.",
60,		120,2,		900,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Fire_Elemental,SKILL_FIRE_ELEMENTAL,SIN_SKILL_USE_RIGHT,Fire_Elemental_UseMana},

{"�÷��� ���̺�",	"������ ȭ���� ����� ������ ������ ���� �Ѵ�.",
63,		32,1,		124,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Flame_Wave,SKILL_FLAME_WAVE,SIN_SKILL_USE_RIGHT,Flame_Wave_UseMana},

{"�����",		"������ �ְ���� ������ �ɷ��� ����߸���.",
66,		43,1,		460,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Distortion,SKILL_DISTORTION,SIN_SKILL_USE_RIGHT,Distortion_UseMana},

{"��Ƽ�����Ʈ",	"���ָ� ��Ƹ��� ���� ������ �Ŵ��� ����� ��ȯ�Ͽ� ������ ��Ÿ�Ѵ�.",
70,		39,1,		190,0,
{1,0,0},			{sinWM1,0,0,0,0,0,0,0},
F_Meteo,SKILL_M_METEO,SIN_SKILL_USE_RIGHT,Meteo_UseMana},

//���� ���� ���̺�
{"����",		"���꿡 ����� ������ ���ݷ��� ����Ų��",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SKILL_FORCE_ORB,0,0},

//����Ʈ Ÿ�� ���̺�
{"����Ʈ",		"���͸� ����ؿ��ÿ�!",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CHANGE_JOB3_QUEST,0,0},

//����Ʈ Ÿ�� ���̺�
{"����Ʈ",		"���͸� ����ؿ��ÿ�!",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CHANGE_ELEMENTARY_QUEST,0,0},

//������ Ŭ����ų ���̺�
{"�ۼҼ� ���� �۷θ�","�����ϵ��� ������� +20��Ų��.",	// ����� - ������ ���� Ŭ�� ��ų ���� ����
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CLANSKILL_ABSORB,0,0},

{"�Ŀ� ����","�����ϵ��� ���ݷ¸� 15%��� ��ŵ�ϴ�.",	// ����� - ������ ���� Ŭ�� ��ų ���� ����
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CLANSKILL_ATTACK,0,0},

{"�̺��̵� �Ʈ","�����ϵ��� Evade �� Ȯ���� 20%���� ��ŵ�ϴ�.",	// ����� - ������ ���� Ŭ�� ��ų ���� ����
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,CLANSKILL_EVASION,0,0},

//������ ������ ��� ��ų
{"��Ÿ���þ�","�����ð� �������°� �� 1/2 ���ݷ� ����",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_INVULNERABILITY,0,0},

{"���帮 ����","ũ��Ƽ�� Ȯ�� 5% ����",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_CRITICAL,0,0},

{"��� ���� �̺��̵�","�̺��̵� Ȯ�� 5%����",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_EVASION,0,0},

{"�����̾� ����","���̾� ũ����Ż Ÿ���� ������ 50% ����Ѵ�.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_R_FIRECRYTAL,0,0},

{"�����̾� ����","���̽� ũ����Ż Ÿ���� ������ 50% ����Ѵ�.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_R_ICECRYTAL,0,0},

{"�����̾� ����","����Ʈ�� ũ����Ż Ÿ���� ������ 50% ����Ѵ�.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_R_LINGHTINGCRYTAL,0,0},

{"����Ʈ ����","������ Ÿ�ݽ� �������ݷ��� 20% ���߽�Ų��.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_FIGHTER,0,0},

{"����Ʈ ����","��ī�ϼ� Ÿ�ݽ� �������ݷ��� 20% ���߽�Ų��.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_MECHANICIAN,0,0},

{"����Ʈ ����","����ũ�� Ÿ�ݽ� �������ݷ��� 20% ���߽�Ų��.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_PIKEMAN,0,0},

{"����Ʈ ����","���� Ÿ�ݽ� �������ݷ��� 20% ���߽�Ų��.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_ARCHER,0,0},

{"����Ʈ ����","����Ʈ Ÿ�ݽ� �������ݷ��� 20% ���߽�Ų��.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_KNIGHT,0,0},

{"����Ʈ ����","��Ʋ��Ÿ Ÿ�ݽ� �������ݷ��� 20% ���߽�Ų��.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_ATALANTA,0,0},

{"����Ʈ ����","������ Ÿ�ݽ� �������ݷ��� 20% ���߽�Ų��.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_MAGICIAN,0,0},

{"����Ʈ ����","������Ƽ�� Ÿ�ݽ� �������ݷ��� 20% ���߽�Ų��.",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,STONE_A_PRIESTESS,0,0},
//�����̾� �κ� ����ȭ ������
{"���ͳ� ������","�����ð� �������°� �� 1/2 ���ݷ� ����",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_P_INVULNERABILITY,0,0},

{"����Ʋ ����","ũ��Ƽ�� Ȯ�� 15% ����",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_P_CRITICAL,0,0},

{"���Ʈ ��ũ��","�̺��̵� Ȯ�� 15%����",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,SCROLL_P_EVASION,0,0},

// ����� - �ν��� ������(�����)
{"����� �ν���","������� 15% ��½�Ų��",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,BOOSTER_ITEM_LIFE,0,0},

// ����� - �ν��� ������(���)
{"��� �ν���","����� 15% ��½�Ų��",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,BOOSTER_ITEM_MANA,0,0},

// ����� - �ν��� ������(�ٷ�)
{"�ٷ� �ν���","�ٷ��� 15% ��½�Ų��",
0,		0,0,		0,0,
{0,0,0},			{0,0,0,0,0,0,0,0},
NULL,BOOSTER_ITEM_STAMINA,0,0},

#endif

#ifdef	_LANGUAGE_CHINESE
#include "Language\\chinese\\C_sinSkill_Info.h"
#endif

#ifdef	_LANGUAGE_JAPANESE
#include "Language\\Japanese\\J_sinSkill_Info.h"
#endif

#ifdef	_LANGUAGE_TAIWAN
#include "Language\\Taiwan\\T_sinSkill_Info.h"
#endif

#ifdef	_LANGUAGE_ENGLISH
#include "Language\\English\\E_sinSkill_Info.h"
#endif

#ifdef	_LANGUAGE_THAI
#include "Language\\THAI\\TH_sinSkill_Info.h"
#endif

#ifdef _LANGUAGE_VEITNAM
#include "Language\\VEITNAM\\V_sinSkill_Info.h"
#endif

#ifdef _LANGUAGE_BRAZIL
#include "Language\\BRAZIL\\B_sinSkill_Info.h"
#endif

#ifdef _LANGUAGE_ARGENTINA
#include "Language\\ARGENTINA\\A_sinSkill_Info.h"
#endif