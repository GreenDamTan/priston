#include  "TextMessage.h" 
char *szAppName= "Priston Tale " ;
char *mgRequestTrade =  "�ʹ͡�ë��͢����ѧ %s " ;
char *mgRequestTrade2 =  "%s �������Թ��������ö�š����¹�� ";

char *mgDiconnect =  "�ѭ�ҳ�Ѵ��ͧ �������ö�Դ��͡Ѻ�����������" ;
char *mgDiconnect1 =  "�ѭ�ҳ�Ѵ��ͧ �������ö�Դ��͡Ѻ���������(1) �� ";
char *mgDiconnect2 =  "�ѭ�ҳ�Ѵ��ͧ �������ö�Դ��͡Ѻ���������(2) �� ";
char *mgDiconnect3 =  "�ѭ�ҳ�Ѵ��ͧ �������ö�Դ��͡Ѻ���������(3) �� ";
char *mgDiconnect4 =  "�ѭ�ҳ�Ѵ��ͧ �������ö�Դ��͡Ѻ���������(4) �� ";

char *mgCloseGame =  "�ͺ�س������ PT ��� " ;
char *mgCloseWindow =  "��سһԴ Window ����Դ�����͹ " ;
char *mgCloseBattle =  "�������ö�͡�ҡ���� ��е����� " ;
/*
char *mgQuestName[][4] = {
	{	 "Rank-up Exam " 	,	 "�������¹���� (����ó�)"  ,  "ChangeJob1a.Msg"  ,  "ChangeJob1b.Msg"  },
	{	 "Rank-up Exam [Royal Jelly] " 	,	 "�������¹���� (����ó�)"  ,  "ChangeJob2a.Msg"  ,  "ChangeJob1b.Msg"  },
	{	 "Rank-up Exam [Hair growth spray] " 	,	 "Rank-up Exam (Done)"  ,  "ChangeJob3a.Msg"  ,  "ChangeJob1b.Msg"  },
	{	"�������¹����  [���駷�� 2] "		,	"�������¹����  [���駷�� 2] " , "ChangeJob3-1.msg" , "ChangeJob3-2.msg" },
	{	"�������¹����  [���駷�� 2] "		,	"�������¹����  (����ó�) " , "ChangeJob3-3.msg" , "ChangeJob3-4.msg" },

	//Fighter Pikeman Mechanic Knight
	{	"�������¹���ʷ�� 2 [1] "		,	"�������¹���ʷ�� 2 [2] "		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-W1-2.msg" },
	{	"�������¹���ʷ�� 2 [3]  "		,	"�������¹���ʷ�� 2 [4] "		, "Quest3\\ChangeJob3-W2-2.msg" , "Quest3\\ChangeJob3-W3-2.msg" },
	{	"�������¹���ʷ�� 2 [5] "		,	"�������¹���ʷ�� 2 [����ó�] "	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//Archer Atlanta
	{	"�������¹���ʷ�� 2 [1] "		,	"�������¹���ʷ�� 2 [2] "		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-R1-2.msg" },
	{	"�������¹���ʤ��駷�� 2 [3] "		,	"�������¹���ʷ�� 2 [4] "		, "Quest3\\ChangeJob3-R2-2.msg" , "Quest3\\ChangeJob3-R3-2.msg" },
	{	"�������¹���ʷ�� 2 [5] "		,	"�������¹���ʷ�� 2 [����ó�]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//Pristess Magician
	{	"�������¹���ʷ�� 2 [1] "		,	"�������¹���ʷ�� 2 [2] "		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-M1-2.msg" },
	{	"�������¹���ʷ�� 2 [3] "		,	"�������¹���ʷ�� 2 [4] "		, "Quest3\\ChangeJob3-M2-2.msg" , "Quest3\\ChangeJob3-M3-2.msg" },
	{	"�������¹���ʷ�� 2 [5] "		,	"�������¹���ʷ�� 2 [����ó�] "	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },
		
	// Level Quest
	{	"����"		,	"���� (������)" 	,	"LevelQuest\\Quest30.Msg" , "LevelQuest\\Quest30a.Msg" },	//14
	{	"���ि "		,	"���ि(������)"	,	"LevelQuest\\Quest55.Msg" , "LevelQuest\\Quest55a.Msg" },	//15
	{	"���ि "		,	"���ि(������)"	,	"LevelQuest\\Quest55_2.Msg" , "LevelQuest\\Quest55_2a.Msg" }, //16
	{	"�Ե��Ҿ������ "	,	"�Ե��Ҿ������(������) "	,	"LevelQuest\\Quest70.Msg" , "LevelQuest\\Quest70a.Msg" },	//17
	{	"��֡ Fury"		,	"��֡ Fury(������)"	,	"LevelQuest\\Quest80.Msg" , "LevelQuest\\Quest80a.Msg" },	//18
	{	"��Ҵ���ҵҤ�������"	,	"��Ҵ���ҵҤ�������(������)",	"LevelQuest\\Quest85.Msg" , "LevelQuest\\Quest85a.Msg" },	//19
	{	"�����ҹ����� "	,	"�����ҹ�����(������)"	,	"LevelQuest\\Quest90.Msg" , "LevelQuest\\Quest90a.Msg" },	//20
	
	//���⺹���ʵ�
	{	"�����ͺ����Ҫ�ҳҨѡ�"	,	"�����ͺ����Ҫ�ҳҨѡ�(���º����)" ,	"LevelQuest\\Quest_7State_1.msg" , "LevelQuest\\Quest_7State_end.msg" },	//21
	{	"�����ͺ����Ҫ�ҳҨѡ�"	,	"�����ͺ����Ҫ�ҳҨѡ�(���º����)" ,	"LevelQuest\\Quest_7State_2.msg" , "LevelQuest\\Quest_7State_end.msg" },	//22
	{	"�����ͺ����Ҫ�ҳҨѡ�"	,	"�����ͺ����Ҫ�ҳҨѡ�(���º����)" ,	"LevelQuest\\Quest_7State_3.msg" , "LevelQuest\\Quest_7State_end.msg" },	//23

	{	"˹�ҧ��觤����ҡ��Һҡ"	,	"˹�ҧ��觤����ҡ��Һҡ(������)" ,	"LevelQuest\\Quest_10State_1.msg" , "LevelQuest\\Quest_10State_end.msg" },	//24
	{	"˹�ҧ��觤����ҡ��Һҡ"	,	"˹�ҧ��觤����ҡ��Һҡ(������)",	"LevelQuest\\Quest_10State_2_1.msg" , "LevelQuest\\Quest_10State_end.msg" },//25
	{	"˹�ҧ��觤����ҡ��Һҡ"	,	"˹�ҧ��觤����ҡ��Һҡ(������)"	"LevelQuest\\Quest_10State_2_2.msg" , "LevelQuest\\Quest_10State_end.msg" },//26
	{	"˹�ҧ��觤����ҡ��Һҡ"	,	"˹�ҧ��觤����ҡ��Һҡ(������)",	"LevelQuest\\Quest_10State_2_3.msg" , "LevelQuest\\Quest_10State_end.msg" },//27

	//3�������¹�Ҫվ�ͺ3
	{	"�������¹�Ҫվ�ͺ���3[1]"	,	"�������¹�Ҫվ�ͺ���3(������)" ,	"Quest4\\ChangeJob4_1.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//28
	{	"�������¹�Ҫվ�ͺ���3[2]"	,	"�������¹�Ҫվ�ͺ���3(������)",	"Quest4\\ChangeJob4_2.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//29
	{	"�������¹�Ҫվ�ͺ���3[3]"	,	"�������¹�Ҫվ�ͺ���3(������)",	"Quest4\\ChangeJob4_3_1.msg" ,	"Quest4\\ChangeJob4_3_2.msg" },	//30
	{	"�������¹�Ҫվ�ͺ���3[3]"	,	"�������¹�Ҫվ�ͺ���3(������)",	"Quest4\\ChangeJob4_3_3.msg" ,	"Quest4\\ChangeJob4_3_4.msg" },	//31
	{	"�������¹�Ҫվ�ͺ���3[3]"	,	"�������¹�Ҫվ�ͺ���3(������)" ,	"Quest4\\ChangeJob4_3_5.msg" ,	"Quest4\\ChangeJob4_3_6.msg" },	//32
	{	"�������¹�Ҫվ�ͺ���3[4]"	,	"�������¹�Ҫվ�ͺ���3(������)",	"Quest4\\ChangeJob4_4.msg" ,	"Quest4\\ChangeJob4_5.msg" }	//33

};

*/
char *mgRefuseWhisper =  "PM �ͧ��ҹ�١����ʸ " ;
char *mgWeightOver =  "���˹ѡ�Թ.. " ;

char *mgYahoo =  "Yes!" ;
char *mgContinueChat = "%s :  ����ҡ�˹���  �������� ������ -_-; ";
char *mgRecvItem = "   ���Ѻ����  ( %s ) ����";

char *mgItemTimeOut = " ����  ( %s ) �������ͧ�ҡ������ء���� ";
char *mgSOD_Clear = " ���ʴ������Թ�� �Ѻ�Թ�ҧ��ŷ���ҹ���Ѻ� ( %dGold) ";

char *mgBlessCrystal_01 = "�Թ�ӹǹ�������ö���¡��";
char *mgBlessCrystal_02 = "������¡����ö���¡����Ф�����ҹ�� ";
char *mgBlessCrystal_03 = "����ʵ�������";

// Bellatra
char *g_lpBellatraTeamNameText[] = 
{
	"Tribulation of Water",
	"Tribulation of Ground",
	"Tribulation of Wind",
	"Tribulation of Fire",
	"The Stage of Sun",
	"The Stage of Moon",
};

char *g_lpBellatraResult_Text1 = "%s �ͧ ";
char *g_lpBellatraResult_Text2 = "%s ��� %s �ͧ ";
char *g_lpBellatraResult_Text3 = "���������� ";
char *g_lpBellatraResult_Text4 = "�������鹺��ͧ��������.";
