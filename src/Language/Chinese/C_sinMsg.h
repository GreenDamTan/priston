#include "sinLinkHeader.h"

sMESSAGEBOX	sMessageBox[100] = {
	{"��Ʒ����û���㹻�Ŀռ�"},
	{"���ı����Ѿ�������������"},   
	{"��Ʒ����ʹ��"},	   
	{"��Ǯ����"},
	{"��Ʒ���ܱ��ϳ�"},
	{"�Ѿ����������Ʒ������������"},
	{"����ȡ��"},
	{"ҩ�����ܱ����"},
	{"��Ʒ�ϳ�ʧ����"},
	{"��л�������ľ���"},
	{"�ֿ�����Ʒ��������"},
	{"��Ʒ���ܱ�����"},
	{"��Ʒ�Ѿ����ı�.���ٴμ��"},
	{"ӵ�еĽ�Ǯ���������������"}, 
	{"��Ʒ����ʧ��"},
	{"�Ѿ�������"},
	{"��ƷͼƬδ��׼��"},
	{"��ֻ������һ��"},
	{"ֻ�й����Ա����ʹ�������Ʒ"},
	{"��������һ�������в���ʹ�������Ʒ"},
	{"���ڵȼ����������ܱ�����"},
	{"�����ȼ��Ѿ�������-1"},
	{"�����ȼ��Ѿ�������-2"},
	{"�����ȼ��Ѿ�������+1"},
	{"�����ȼ��Ѿ�������+2"},
	{"��������Ʒ�Ĺ����ٶ�"},
	{"   ����������Ʒ    "},
	{"   ��������ֵ+15"},
	{"   �����Ѿ������    "},
	{"   �𰸴�����    "},
	{"   û�и�����Գ��۵���Ʒ��  "},
	{"��Ʒ�Ѿ�������"},
	{"���ҵĸ����̵��Ѿ��ر�"},
	{"   �Ҳ�������   "},
	{"    ��Ʒ����������     "},
	{"    ���Խ�ɫ����ʹ��    "},
	{"    Ů�Խ�ɫ����ʹ��    "},
	{" Ů�Խ�ɫ���ܻ�ø���Ʒ "},
	{" ���Խ�ɫ���ܻ�ø���Ʒ "},
	{"    ��Ʒ�۸��Ѿ��ı�  "},
	{"    �ǹ��ά����Ǯ    "},
	{"  ���ĵȼ�������ʹ���������֮ʯ  "},
	{"     ����֮ʯ����ʹ�ù�����    "},
	{"     ����ǿ��ģʽ���ڱ�����    "},
	{"    ����֮ʯ�ĸ����Ѿ����    "},
	{"    �������Ե�����5��   "},
	{"    ���ܵ�����1��    "},
	{"���Ե�ͼ��ܵ�ֱ�����5���1��"},
	{"���Ե�ͼ��ܵ�ֱ�����5���2��"},
	{"   ��������ֵ�ͷ�����1%  "},
	{"   ����ֵ����40��   " },
	{"�����ڽ�������" },
	{"����ȼ�����80ʱ��������ÿ��������õ�7���������Ե�." },
	{"����ȼ�����90ʱ��������ÿ��������õ�10���������Ե�." },
	{"�����²��Ե�����." },
	{"���ڳ�ʱ����ʧ��." },
	{"Ӷ����������."},
	{"����ʹ���еļ�����Ʒ."},
	{"���ܽ������͵�ף���Ǳ���������GMѯ������"},	
	{"����Ʒ�����ڸ����̵���г���."},
	{"û���㹻������˰�տ��Ա�ȡ��."},
	{"���Ѿ��ش���˸�����."},
	{"��л���ش��˸�����."}, 
	{"���ĵȼ�����ʹ�ø÷�ӡ��Ʒ."},
	{"��Ʒ�Ѿ�������."},
	{"����Ը���Ů���ս������ʤ����."},
	{"����Ը���Ů���ս������ʧ����."},
	{"�Ѿ���õ��⳥��Ʒ."},
	{"ͬһ���Ե���Ʒ����һ��ʹ��."},
	{"�Ժ����ʹ��"},
	{"��ϱ�ʯ�Ĺ����в���ȡ��."},
	{"��ϱ�ʯ�Ĺ����в����ֳ�����."},
	{"����ʹ���뵱ǰ����һ����Ⱦ����."},
	{"��Ʒұ��ʧ��."}, // pluto ұ��ʧ����Ϣ
	{"��Ʒ����ʧ��."}, // pluto ����ʧ����Ϣ
	{"��ϱ�ʯ��״̬�²��ܳ��п�ʯ/��ʯ."}, // pluto
	{"�������ֵ����״̬�²��ܳ����ؼ�."}, // pluto
	{"����ͬ���ʯ/ˮ��"}, // pluto
	{"���칫ʽ������˳�����"}, // pluto MESSAGE_NOT_NEED_RUNE
};

sMESSAGEBOX2 sMessageBox2[20] = {
	{"������Ʒ","���빺��?"},
	{"������Ʒ","�������?"},
	{"������Ʒ","�����׳�?"},
	{"���ٽ�Ǯ  ","�����׳�?"},
	{"",""}, 
	{"���ٽ�Ǯ "," ���뽻��?"},
	{"���ٽ�Ǯ  ","�����ƶ�?"},
	{"���ٽ�Ǯ  ","�����û�?"},
	{"�������� ","���빺��?"},
	{"���ٽ�Ǯ  ","�������?"}, 
	{"����ע��","������Ʒ��Ǯô? "}, 
	{"������Ʒ","���빺��?"},  //�����̵� ҩˮ
	{"���ٽ�Ǯ  ","����ȡ��?"}, 
	{"  ���� ","���������Ʒ��?"}, 
	{"  ���� ","���������Ʒ��?"}, //ֻ�е�14���������
	{"���ٽ�Ǯ ","����ȡ��?"},//ȡ������ս�Ľ�Ǯ
	{"����Ӷ��","�����Ӷ?"} //Ӷ����Ӷ
	/*
	{"������","����װ��?"},
	{"��Ҫ��","����װ��?"},
	{"���붪","����װ��?"},
	{"���붪","����Ǯ?"},
	{"",""}, 
	{"���뽻��"," ����Ǯ?"},
	{"�����ƶ�","����Ǯ?"},
	{"�����ջ�","����Ǯ?"},
	{"  ������  ","װ����?"}, 
	{"  ������  ","װ����?"}, 
	{"  ������  ","װ����?"}, 
	{"  ������  ","װ����?"}, 
	{"  ������  ","װ����?"}, 
	{"  ������  ","װ����?"}, 
	{"  ������  ","װ����?"},  //ֻ�е�14���������
	*/
};

sMESSAGEBOX3 sMessageBox3[MESSAGE3_MAX] = {
	{"����","ѧϰ�ü���ô?"},
	{"����", "����ô?"}, //̹����
	{"����", "ִ���������ô?"},
	{"����","���·�����?"},
	{"����SOD"," ��Ҫ����"},
	{"����","���·�����?"},			
	{"����", "ִ���������ô?"}, // ħ��
	{"����", "���������Ʒô?"}, //PostBox
	{"����", "���������?"}, //LinkCore
	{"����", "���������Ʒ?"}, //RecvOthers
	{"����", "���͵����� ?"}, //RecvOthers
	{"","����������"}, //RecvOthers
	{"����","������Ʒô?"}, //RecvOthers
	{"����", "������Ʒô?"}, //��Ʒ����
	{"����", "��������ô?"}, //����������
	{" ����", "֧���þ���ô?"}, //��Ʒ����
	{" �����̵� ", "fffffffffff"}, //��Ʒ����
	{"����", "���׸���Ʒô? "}, //������Ʒ����
	{"����", "�������Ʒô?"}, //�����̵���Ʒ����
	{"���� ", "ȡ��ע��ô?"}, //ȡ�������̵���Ʒע��
	{"����", "���׸���Ʒô? "}, //�ͱ����Ľ���Ʒ����
	{"����", "���׸���Ʒô? "}, //��β����Ʒ����
	{"����", "����������?"}, //Level30
	{"����", "����������?"}, //Level55
	{"����", "����������?"}, //Level55_2
	{"����", "����������?"}, //Level70
	{"����", "����������?"}, //Level80
	{"����", "����������?"}, //Level85
	{"����", "����������?"}, //Level90
	{"����", "���д�����?"}, //����
	{"����", "����������?"}, //Level80
	{"����", "����������?"}, //Level90_2
	{"����", "����������?"}, //��4��תְ
	{"����", "������?"}, //��4��תְ
	{"����", "���׸���Ʒ��? "},//����7��ˮ�� ��Ʒ
	{"����", "���д�����?"},    //Teleport
	{"���빺��","һ��ˮ������?"},      //ˮ��������.
	{"��ϣ��", "���ܸ�������?"}, //A
	{"��ϣ��", "���ܸ�������?"}, //B
	{"��ϣ��", "���ܸ�������?"}, //C
	{"��ϣ��", "���ܸ�������?"}, //D
	{"��ϣ��", "���ܸ�������?"}, //E
	{"��ϣ��", "���ܸ�������?"}, //F
	{"��ϣ��", "���ܸ�������?"}, //G
	{"��ϣ��", "ʹ�ø���Ʒ��?"},       //ʹ�ù�����Ʒ
	{"��ϣ�� ", "���ܸ�������?"},//�Ƿ����100LV����
	{"��ϣ�� ", "���ܸ���Ʒ��?"},   //100LV��Ʒ
	{"��ϣ��","�ƶ�����������?"},   //�Ƿ��ƶ���100LV���������
	{"����", "���д�����?"},    					//Teleport 												//pluto
	{"����", "���׸���Ʒô? "}, //Change item(Shining dust) 			//pluto
	{"����", "���д�����?"},    		//Twisted forest 									//pluto
	{"����", "������Ʒ��?"}, 	// Manufacturing message 					//pluto
	{"����", "���д�����?"},    		// The newest map(Endless tower) 	//pluto
};


char *SkillMaster[4] = {
	"     ѧϰ����",
	"        תְ",
	"     �뿪��Ϸ"

};
char *SkillMaster2[4] = {
	"",
	"",
	""

};

char *ReStartMsg[4] = {
	"        �ص�ԭ��     ",
	"        �ص�����     ",
	"        �뿪��Ϸ     ",
	"       ����ֵ����    "

};
char *ReStartMsg2[4] = {
	"   ��ʧ1%��Ǯ/����ֵ ",
	"",
	"",
	"      ���ܻص�ԭ��   ",
};


char *SkillUseInfoText[10]= { "       ��������      ","     ѧϰ�¼��ܺ���߼��ܵȼ�    ","      ��ѡ��Ҫ�����ļ���      ",
						   	  "   ����Ŀǰ��û�м��ܵ�   ","          �޷�ѧϰ����      " 
	};

char *sinMsgSkillPoint = "         ���ܵ� : %d";
char *sinMsgEliteSkillPoint = "��Ӣ���ܵ� : %d " ;

char *sinStateName[5]={
	"����ֵ : %d/%d  ",
	"����ֵ : %d/%d  ",
	"��  �� : %d/%d  ",
	"����ֵ : %d/%d  ",
	"����ֵ : %dM/%dM  "};


char *TownName[3] = {
	"�ص�����\r",
	"�ص���ά˹��\r",
	"�ص��ƶ���\r"};

char *sinAbilityName[50] = {
	"����: \r","�����ٶ�: \r","���: \r","��ɱ��: \r","���: \r","������: \r",
	"����: \r","�ֵ���: \r","�ٶ�: \r","�;ö�: \r","�ָ�����: \r","�ָ�����:\r",
	"�ָ�����: \r","ħ����: \r","��Ȼ����: \r","�����: \r","������: \r","�׷���: \r","������: \r",
	"ˮ����: \r","�����: \r","��������: \r","��������: \r","��������: \r","�������: \r",
	"�������: \r","�������: \r","ҩˮ�������: \r","�ȼ�Ҫ��: \r","����Ҫ��: \r","����Ҫ��: \r",
	"����Ҫ��: \r","����Ҫ��: \r","����Ҫ��: \r"};


char *sinSpecialName[50] = {
	"�����ٶ�: \r","��ɱ��: \r","����: \r","����: \r",
	"�ֵ���: \r","ħ��������: \r","�ٶ�: \r","ħ����: \r","��Ȼ����: \r",
	"�����: \r","������: \r","�׷���: \r","������: \r","ˮ����: \r","�����: \r",
	"������: \r","����: \r","���: \r",
	"ħ����: \r","��Ȼ����: \r","�𹥻�: \r","������: \r",
	"�׹���: \r","������: \r","ˮ����: \r","�繥��: \r",
	"�������ֵ����: \r","�������ֵ����: \r",
	"��������: \r","��������: \r","��������: \r",
	"�����۸�: \r","����۸�: \r"};


char *SpecialName3 = "%s ������Ч��\r";
char *MixResultTitle = "��Ʒ��ϵĽ��";
char *NowLevelText = "Ŀǰ�ȼ�\r";
char *NextLevelText = "��һ���ȼ�\r";

char *NormalAttckName ="��������";
char *RequirLevel = "%s (�ȼ�Ҫ��:%d)\r";
char *UseItemGroupName = "��Ӧ��װ��\r";

char *PoisonName = "������:\r";
char *FireName = "�����:\r";
char *AttackAreaName = "������Χ:\r";
char *Attack_RateName = "����:\r";
char *Max_DamageIncre = "�������ֵ����:\r";
char *DamageAddName = "��������ǿ:\r";
char *Attack_SpeedAdd = "�����ٶ�����:\r";
char *IncreBlock = "�ֵ�������:\r";
char *CountinueTime = "ά�ֵ�ʱ��:\r";
char *UseManaName = "�ķ�����:\r";
char *DamageName = "������:\r";
char *ShootingRangeName = "���:\r";
char *IncreAsorb = "��������:\r";
char *SecName = "��";
char *PlusDamageName = "��������:\r";
char *HitNumName = "������������:\r";
char *DecreLifeName = "��������:\r";
char *Attck_RateAdd = "��������:\r";
char *Add_CriticalName = "��߱�ɱ��:\r";
char *Push_AreaName = "�˺�ľ���:\r";
char *FireAttackDamageAdd ="��߻�ϵ������:\r";
char *IceName = "������:\r";
char *IceAttackDamageAdd = "��ϵ������:\r";
char *StuneRate = "�ε��ĸ���:\r";
char *DefenseRateIncre = "������������:\r";
char *WeaponSizeName ="�����Ĵ�С:\r";
char *HwakRotationNum = "ӥ��������:\r";
char *WeaponSpeedAddName  = "�������ٶ�����:\r";
char *IncreAttack_RateName = "��������:\r";
char *ShootingNumName = "��������:\r";
char *SeriesShootingCount = "��������ļ���:\r";
char *UseStaminaName = "�ķ�����:\r";
char *MasterSkillMoneyName = "ѧϰ���ܵķ���:\r";

char *ItemAgingResult = "��Ʒ��ϵĽ��";

char *sinGold ="%d��";
char *sinGiveItem5 = "������Ʒ\r";
char *sinCopyItem5 = "������Ʒ\r";
char *sinNum7 = "��";
char *sinSkillPointName = "���ܵ�";
char *sinG_Pike_Time3 = "����ʱ�� :\r"; 
char *sinItemLimitTimeOverMsg = "������������\r";
char *sinDeadSongPyeunEat = "��ѽ�����±��Ѿ�������!";
char *SmeltingResultTitle = "��Ʒұ�����";		// pluto ұ��
char *ManufactureResultTitle = "��Ʒ������";	// pluto ����
char *ManufacturingTitle = "���";	//random

/////////////////// ħ��׷�Ӽ��� 
char *sinMaxDamageAdd = "��󹥻�������+\r";
char *sinDamagePiercing = "(��͸����)\r";
char *sinAddStamina = "����׷����:\r";
char *sinAddLife = "����׷��ֵ:\r";
//char *sinPartyHealDoc = " ����ӵ�����»ظ�������/3 :\r ��Ӧ�ľ���ֵ����:\r";
char *sinDamageUndead50 = "�Բ���ϵ���﹥����׷��50% :\r";
char *sinNumCount2 = "����:\r";
char *sinNumCount3 = "����\r";
char *sinDecreDamage = "��������Сֵ:\r";
char *sinConvert4 = "�任��:\r";
char *sinIncreElement = "Ԫ����������ֵ:\r";
char *sinPartyIncreElement = "��Ա������ֵ����50% ����\r";
char *sinFireBallDamage2 = "(�����еĹ����� 100% �˺�)\r";
char *sinFireBallDamage3 = "(�����뾶 60  (1/3 �˺�))\r";
char *sinAddDamage7 = "׷�ӹ�����:\r";
char *PlusDamageWeapon = "����";
char *SparkDamage = "ÿ�η����Ĺ�����:\r";
char *SparkNum7 = "��෢����:\r";
char *StunRage ="���ξ���:\r";
char *RecvPotion7 = "..*..";

//ħ��ڶ��μ���
char *PartyArea7 ="ս��ʱ��ȫ���Ķ�Ա��ǿ����:\r";
char *Area17 ="�뾶:\r";
char *AddAttack_Rate9 ="����������:\r";
char *PiercingRange ="��ͨ����:\r";
char *IncreDefense = "����������:\r";
char *AddShootingRange = "�����������:\r";
char *LifeAbsorb = "������������:\r";
char *FireDamage = "׷�ӻ����Թ�����:\r";
char *IceDamage = "׷�ӱ����Թ�����:\r";
char *LightningDamage = "׷�������Թ�����:\r";
char *AddManaRegen = "׷����������:\r";
char *LightNum = "�׵�����:\r";
char *ReturnDamage = "���ص��˺�:\r";
char *Area18 = "��Ч�뾶:\r";
char *IncreMana5 = "��������:\r";
char *Area19 = "��Ч����:\r";
char *HelpTitle8 = "����";
char *QuestTitle8 = "����";
char *ResearchTitle8 = "����";
char *TeleportTitle8 = "���;���";

char *IncreWeaponAttack_RateName = "��������������:\r";
char *AddMaxDamage = "����˺��̶�����:\r";
char *LinkCoreName = "���͵������Ա����\r";
char *DesLinkCore = "�Է�:";
char *Itemul = "��ô�õ�װ��";
char *ItemRecvOk = "���������";
char *Money5 = "Ԫ";
char *Exp5 = "����ֵ";
char *WingItemName[6] = {"����֮��","��ɫ֮��","�ƽ�֮��","���֮��","����֮��","��ʧ֮��"};
char *WarpGateName[10] = {"����֮��","����","�ƶ���","�ƻ�����","��֮��","�����ׯ","��ά˹��","���֮��","������½","��ʧ֮��"};
char *sinWarningName5 = "�Ϲ�����";
char *ClanCristalName = "�����Աר��\r";
char *NowMyShopSell = "������\r";
char *SecretNumName = "������֤���";
char *MyShopExpDoc7 = "������";
char *NotAgingItem2 = "�޷������װ��";
char *ExpPercent2 = "����ֵ : %d.%d%s";
char *Mutant7   =   "ͻ������";
char *Mechanic7 = "ħ�޹���";
char *Demon7    = "��ħϵ";
char *Nomal7    = "һ�����";
char *Undead7    = "��������";
char *MonsterAddDamage2 = "�����˺�)\r";
char *MonsterAddDamageFire = "���ӻ����)\r";
char *HaWarpGateName[] = {"ף���Ǳ�",0};
char *FallGameName = "����֮��"; // Twisted forest
char *EndlessGameName = "��ʧ֮��"; // Endless tower
char *LookHelp	= ">>��ϸ��Ϣ..."; //>>more..

//������Ʒ
char *HoldStarNumDoc = "������������";
char *StarItemBuy7   = "��������";
char *ChangeMoney7   = "(һ�����ǵļ۸�: 100000)"; 



//�����μ���
char *LightningDamage2 = "�����˺�:\r";
char *SheildDefense = "���ӷ���:\r";

char *DemonDamage4  = "�Զ�ħϵ������:\r";
char *PoisonDamage3  = "���˺�:\r";
char *PikeNum4       = "ì������:\r";
char *poisoningTime  = "�ж�ʱ��:\r";
char *PlusCriticalName = "��������һ����%:\r";
char *SpiritFalconDamage2 = "ӥ���˺�:\r";
char *LifeGegenPlus = "��������ֵ:\r";
char *PlusDamage4 = "�����˺�:\r";
char *PiercingPercent = "���̰ٷֱ�:\r";
char *DamageToLife = "����ֵ�˺�:\r";
char *MyMonster7 = "�ҵĳ���:\r";
char *LifeIncre4 = "��������:\r";
char *UndeadDamage3 = "����ϵ�˺�:\r";

char *AttackNum3     = "��������:\r"; 
char *AttackRateMinus = "���͹���Ƶ��:\r"; 
char *MaxDamagePlus2 = "��󸽼��˺�s:\r"; 
char *LightningAddDamage3 = "���總���˺�:\r"; 
char *ReLifePercent4    = "��������:\r"; 
char *GetExp3    = "��þ���ֵ:\r"; 
char *ResurrectionChar4    = "����ȼ�"; 
char *ExtinctionPercent2    = "��ʧ�ٷֱ�:\r"; 
char *ExtinctionAmount2    = "��ʧ����:\r"; 
char *IncreLifePercent2   = "��������ֵ�ٷֱ�:\r"; 

char *ReduceDamage3 = "�����˺�:\r"; 
char *IncreMagicDamage = "����ħ���˺�:\r"; 
char *AttackDelay3   = "�����ӳ�:\r"; 

char *FireDamage2 = "�����˺�:\r";
char *IceDamage2 = "��ϵ�˺�:\r";
char *AddDefense8 = "���ӷ���:\r";
char *SkillDamage5 = "�����˺�:\r";

char *FireDamage3 = "�����˺�:\r";
char *AddSpeed7 = "�����ٶ�:\r";
char *AfterDamage7 = "֮���⵽�˺�:\r";
char *MonsterSight7 = "������Ұ:\r";

//���Ĵμ���
//vamp
char *LinghtingAddDamage4="��������Ĺ�������:\r";
char *MaxBoltNum4="����ѹ:\r";
char *AddAbsorb4="�����˺�������:\r";
char *IncreArea4="�������:\r";
char *IncreAttack4="���๥��ǿ��:\r";
char *GolemLife4="����+����:\r";


char *SubAbsorb4="���������˺�:\r";
char *AreaDamage4="����ڵĹ���Ƶ��:\r";
char *MaxAttackNum4="��������:\r";
char *LifeUp4="��������ֵ:\r";

char *CriticalAddDamage4="���ӵ�����һ����:\r";
char *MonstervsSubCritical4="�������������һ����:\r";
char *ChargingAddPercentDamage4="���ӱ����Ĺ���Ƶ��:\r";

char *IncreEvasionPercent4="�����������:\r";
char *AddShadowNum4="�����Ӿ����:\r";

char *WolverinLife4="����ֵ:\r";
char *WolverinRate4="׼ȷ��:\r";
char *WolverinDefense4="��������:\r";
char *AddEvasion4="���Ӷ������:\r";
char *AddDamage4="����(����)����Ƶ��:\r";
char *FalconAddDamage4="������ӥ�Ĺ���Ƶ��:\r";

//ħ��
char *IncreAttackAbsorb4="���ӵ�����:\r";

char *RectAngleArea4="�ھ�ȷ�ķ�Χ�Ƕ��ڹ���:\r"; 

char *MonsterSubSpeed4="�ܵ����﹥�������ٶ�:\r";


char *AddLifeRegen4="������������:\r";
char *AddManaRegen4="������������:\r";
char *MagicArea4="ħ����Ч���:\r";
char *ChainNum4="��Ӱ��Ĺ�������:\r";
char *ChainRange4="��Ӱ������ŭ��ֵ:\r";
char *UndeadDamageAbsorb4="����ϵ��������:\r";
char *UserBlockPercent4="��ɫ����:\r";

char *SecondIncreMana4="����ÿ��ħ���ָ�:\r";
char *FireDamage4="�𹥻�����:\r";
char *DecreSpeed4="����:\r";
char *DecreAttack4="���͹����ٶ�:\r";
char *AddPercentDamage4="���ӹ���Ƶ��:\r";


//�����˺�
char *ChainDamage3    = "��Ҫ�˺�)\r";
char *BrandishDamage3 = "�����˺�";
char *PhysicalAbsorb3 = "��������";
char *SparkDamage3    = "�ŵ��˺�";
char *BrutalSwingCritical3   = "����һ��";
char *Attck_RateAdd5 = "���ӹ���Ƶ��:\r";
char *Triumph4="ʤ��";
char *Use4="��";
char *DivineShield4="ʥ�ܵĵֵ���";

//�����
char *QuestMonsterName[] = {"������","����սʿ","ħ��ʿ","��װЫ��","��������","̩̹����" ,
                         "����֮�صĹ���","���޵Ĺ���","�Ŵ�����һ��Ĺ���"};


//��� ������ +_+
char *VampOption[10] ={"��������%: \r","��Ѫ��: \r","����˺�: \r","��������: \r","����: \r"};


char *Quest3ItemName[]={"��ħ����","�Ŵ�������","����","����","ˮ��","�ƽ�","���"};

char *Quset3ItemDoc15 = "������Ʒ";
char *Quset3ItemDoc16 = " ���� ";

char *SparkDamage10 = "�����˺�:\r";

char *RequirLevel3 = "(��Ҫ�ȼ�:%d)\r";

char *RecvItemTT = "�һ������Ʒ";
char *PuzzleEvent5 = "�������";
char *BabelHorn = "�ͱ���֮��";
char *ChristMas				="ʥ���ڵ�����";
char *NineTailFoxItem = "��β�����";
char *MyShopItemSell5 = "�۸�: \r";
char *CristalItem     ="��ˮ��";//��ˮ��
char *StarDust				="ʥ���ڵ�����";


char *sinDeadCandyEat = "ζ�����ѳ�!";

char *PotionCntDoc2 = "ҩ��";
char *ItemPrice7 = "��Ʒ�۸�";

char *BuyMyShopItem7 = " %s �Ѿ������� %d ������ %s �������� %d��";
char *BuyMyShopItem8 = "�Ѿ��� %s �������� %d ������ %s �������� %d��";
char *ItemName87 = "��Ʒ";

char *sinEvent87 = "Σ���ղؼҵ�����";
char *sinAddDamageItem = "���ӹ�����: \r";
char *sinContinueTimeItem = "����ʱ��: \r";
char *BuyPotionMoney = "��Ǯ:";
char *BuyPotionKind = "����:";
char *SheltomName2[] = {"ʥ��","ƽ��","���","��","��Ϯ","���","�ڰ�","��ʥ","����", "����", "��ħ", "����"};

char *AddPercentDamage3= "���ӹ�����%: \r";

char *sinLuckyBox = "���˰�";
char *SodMessage_Etc[]= {
			"�����������Ϊ",
			"ֻ�й���᳤���Եõ���.",
			"���ڵ�˰��Ϊ %d%s.",
			};
			
char *SodMessage_Clan[] = {
			"�ܵ��������� ",
			"ս�������ṩ�Ľ�ƷΪ.",
			};
			
char *SodMessage_Master[] = {
			"�ܵ���������",
			"ս�������ṩ�Ľ�ƷΪ.",
			"����Ҫ�һ���",
			};
			
char *OtherClanMaster[] = {
			"������Ȼ�õ���Ʒ",
			"����һ��Ľ��.",
			};
/*----------------------------------------------------------------------------*
*			����			����սĿ¼��Ϣ
*-----------------------------------------------------------------------------*/	
char *SiegeMessage_Taxrates[] = {
	"˰�ʵ���Ϊ%d%s.",
	"�ܵ�˰���Ѿ����ۼ�",
	"��.",
	"���볷��˰����?.",
	"���ڵ�˰��Ϊ%d%s.",
};
char *SiegeMessage_MercenrayA[] = {
	"�������",
	"����������",
	"�������ۣ�����ս��������.",
	"Ӷ����������Ϊ:20 / %d",
};
char *SiegeMessage_MercenrayB[] = {
	"���Ǿ���",
	"��ΪӶ����˵�۸���������ǱȽϾ����",
	"��ӵ���еȳ̶ȵ�ս����.",
	"Ӷ����������Ϊ:20 / %d",
};
char *SiegeMessage_MercenrayC[] = {
	"��ף���ĻʼҾ���",
	"��ߵȼ���Ӷ���Ŷ�",
	"��ӵ����ǿ���ս����",
	"Ӷ����������Ϊ:20 / %d",
};
char *SiegeMessage_TowerIce[] = {
	"��ϵˮ����",
	"�����н����ƶ��ٶȵ�Ч���Լ�",
	"�����н��͹����ٶȵ�����.",
	"��ˮ���������Ѿ����",
};
char *SiegeMessage_TowerFire[] = {
	"��ϵˮ����",
	"����м�ǿ�Ĺ���������",
	"û��������������.",
	"��ˮ���������Ѿ����",
};
char *SiegeMessage_TowerLighting[] = {
	"��ϵˮ����",
	"�׾��н����ƶ��ٶ��Լ�",
	"�����н��͹����ٶȵ�����.",
	"��ˮ���������Ѿ����",
};
char *SiegeMessage_MerMoney   = "Ӷ������: ";
char *SiegeMessage_TowerMoney = "����������: ";

char *sinClanMaster7 = "�ھ�����";
char *sinPrize7 = "��Ʒ : ";

char *sinLevelQuestMonster[]={"������","�ɱ���","�۷侫","̨��","³ͼ��","��������","��ȭ����"};
char *sinLevelQusetDoc="�Ѿ���ɱ��� %d �� %s ^^";

char *sinTeleportDoc[] = {"Ģ����Ѩ","�䷿��Ѩ","����֮��","����֮��"};
char *sinLevelQuest90_2Mon[] = {"����ʮ��","������","�޴�ħ"};
char *sinLevelQusetDoc2="����Ҫȥ���� %d �� %s";
char *sinLevelQusetDoc3="<Quest> Get the item from %s";
char *sinLimitTiem2 = "ʱ������: \r";
char *sinMinute2 = "����";
char *sinDay4    = "��";
char *sinHour4   = "Сʱ";
char *sinLevelQusetDoc4="���Ѿ��ɹ������� %d �� %s";

sinSTRING sinTestMsg7("����, ������, ������!");

char *sinMonCodeName[]={
	0,
	"������",
	"����սʿ",
	"ħ��ʿ",
	"�ؼ�Ы��",
	"���������",     //5
	"̩̹����",
	"�ɱ���",
	"�۷侫",
	"̨��",
	"³ͼ��",                //10
	"��������",
	"��ȭ����",
	"����ʮ��",
	"������",
	"�޴�ħ",              //15
	"���þ�ʿ",
	"������ţ",
	"����������",
	"ľ����",
	"�ڰ���ʿ",             //20
	"������",            
	"��˫ʯ��",
	"��˫ʯħ",
	"���׿�ħ",
	"ħ����",       //25
	"ħǹ����",
	"����",
	"ħս����",
	"����",
	"Ů��",                //30
	"��װħ�ޱ�",
	"����ħ��",
	"����",
	"Ӣ���˹",
	"��������",           //35
	"��Ӱħ��",
	"����",
	"ħ������",
	"���",
	"��е����",            //40
	0,
};

char *ChargingTime4="���ת���ٶ�:\r";
char *AddIceDamage4="���ӱ�˪Ԫ���˺�:\r";
char *IceTime4="����ʱ��:\r";
char *sinAttackType = "������ʽ:\r";
char *sinAttackType2[2] ={"�ֶ�\r","�Զ�\r"};

char *Won="��Ǯ";

//����ս��Ʒ˵��
char *CastlItemInfo[] ={
	{"��ʱ�����޵�\r"},  
	{"����5%��\r"},
	{"���ٷֱ����Ӷ��\r"},
	{"�ָ���������ҵ�\r"},
	{"ԭ�ظ���.\r"},
	{"ˮ�����Ĺ���\r"},     //
	{"��ʹ�ö���Ĺ���\r"},
	0,
};
char *CastlItemInfo2[] ={
	{"���������� 1/2.\r"},
	{"����һ��\r"},
	{"\r"},
	{"������������ħ��.\r"},
	{"\r"},
	{"����50%\r"},
	{"����20%\r"},
	0,
};


char *CharClassTarget[8]={
	"ʹ�ö���(��ʿ)\r",
	"ʹ�ö���(��е��)\r",
	"ʹ�ö���(ǹ��)\r",
	"ʹ�ö���(������)\r",
	"ʹ�ö���(����)\r",
	"ʹ�ö���(ħǹ��)\r",
	"ʹ�ö���(ħ��ʦ)\r",
	"ʹ�ö���(��˾)\r",
};
char *AttributeTower[4]={
	"����(��)\r",
	"����(��)\r",
	"����(��)\r",
	"����ʱ��:%d��\r",
};

char *SiegeMessage_MerComplete ="Ӷ�������Ѿ����";

//��ʼ���� 
char *haQuestMonsterName[]={
	"������",
	"������",
	"ħ�ޱ�",
	"��ħ��",
	"����",
	"������",
	"����ħ��",
	"������",
	0,
};
char *ha100LVQuestItemName[]={
	"����֮��",
	"����֮צ",
	"����֮��",
	"����֮ǹ",
	"����֮��",
	"����֮��",
	"����֮��",
	"����֮��",
	0,
};
//100LV���� (����Ů��Ļ�Ӱ)
char *ha100LVQuestMonterName[]={
	"��������Ĺ���",
	"������ɽ�ȵĹ���",
	0,
};
char *ha100LVQuestName = "��������";

//�����շ���Ʒ
char *PremiumItemDoc[][2]={
	{"�������Ե�\r"     ,""},     //���Գ�ʼ����Ʒ
	{"���ü��ܵ�\r"     ,""},    //���ܳ�ʼ����Ʒ
	{"��������/���ܵ�\r",""},     //���Գ�ʼ����Ʒ
	{"ʹ���������\r"    ,"�Զ�����.\r"},        //������Ʒ
	{"��˲����޵�״̬.\r","���������ή��1/2\r"},  //��������
	{"����15%��\r"    ,"����һ��.\r"},		//��ŭ����
	{"����15%��\r"         ,"��ܻ���.\r"},		//���ܾ���
};
char *UpKeepItemDoc[] = {
	"ʣ��ʱ��: %d��",
	"�����Ʒ %d%s����",
	"��þ���ֵ %d%s����",
	"������%d%s����",
	"��ս����ҩ��",
	"��ս����ҩ��",
	"����%d����",	// pluto mite of awell
	"�������ļ��� %d%s",		// pluto Mana reduce potion 
	0,
};
char *UpKeepItemName[] = {
	"����װ�������ҩˮ",
	"���Ӿ���ֵ��ҩˮ",
	"�����ظ�ҩ��",
	"�����ظ�ҩ��",
	"����ǿ��ҩˮ",		// pluto mite of awell
	"��Ӣ����ҩˮ",		// pluto Mana reduce potion
	"���",			// phoenix
	0,
};