char *szAppName="�������� ����";


char *mgRequestTrade = "%s�Բ� �ŷ��� ��û�߽��ϴ�";
char *mgRequestTrade2 = "%s���� �Ÿ��� �־ �ŷ� ��û�� �Ҽ� �����ϴ�";

char *mgDiconnect = "�������� ������ ���� �����ϴ�";
char *mgDiconnect1 = "�������� ������ ���� �����ϴ� (1)";
char *mgDiconnect2 = "�������� ������ ���� �����ϴ� (2)";
char *mgDiconnect3 = "�������� ������ ���� �����ϴ� (3)";
char *mgDiconnect4 = "�������� ������ ���� �����ϴ� (4)";

char *mgCloseGame = "������ ���� �մϴ�";
char *mgCloseWindow = "������� â�� �ݰ� ���� �Ͻʽÿ�";
char *mgCloseBattle = "�����߿��� �����Ҽ� �����ϴ�";
/*
char *mgQuestName[][4] = {
	{	"��������"	,	"�������� (�Ϸ�)" , "ChangeJob1a.Msg" , "ChangeJob1b.Msg" },
	{	"�������� [�ξ�����]"	,	"�������� (�Ϸ�)" , "ChangeJob2a.Msg" , "ChangeJob1b.Msg" },
	{	"�������� [�߸���]"		,	"�������� (�Ϸ�)" , "ChangeJob3a.Msg" , "ChangeJob1b.Msg" },

	{	"�������� [�ι�°]"		,	"�������� [�ι�°]" , "ChangeJob3-1.msg" , "ChangeJob3-2.msg" },
	{	"�������� [�ι�°]"		,	"�������� (�Ϸ�)" , "ChangeJob3-3.msg" , "ChangeJob3-4.msg" },

	//������ ����ũ�� ��ī�ϼ� ����Ʈ
	{	"�������� 2��[1]"		,	"�������� 2��[2]"		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-W1-2.msg" },
	{	"�������� 2��[3]"		,	"�������� 2��[4]"		, "Quest3\\ChangeJob3-W2-2.msg" , "Quest3\\ChangeJob3-W3-2.msg" },
	{	"�������� 2��[5]"		,	"�������� 2��[�Ϸ�]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//���� ��Ż��Ÿ
	{	"�������� 2��[1]"		,	"�������� 2��[2]"		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-R1-2.msg" },
	{	"�������� 2��[3]"		,	"�������� 2��[4]"		, "Quest3\\ChangeJob3-R2-2.msg" , "Quest3\\ChangeJob3-R3-2.msg" },
	{	"�������� 2��[5]"		,	"�������� 2��[�Ϸ�]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//������Ƽ�� ������
	{	"�������� 2��[1]"		,	"�������� 2��[2]"		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-M1-2.msg" },
	{	"�������� 2��[3]"		,	"�������� 2��[4]"		, "Quest3\\ChangeJob3-M2-2.msg" , "Quest3\\ChangeJob3-M3-2.msg" },
	{	"�������� 2��[5]"		,	"�������� 2��[�Ϸ�]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//������ ����Ʈ
	{	"�׳���� ���Ͽ�"	,	"�׳���� ���Ͽ�(�Ϸ�)" ,	"LevelQuest\\Quest30.Msg" , "LevelQuest\\Quest30a.Msg" },	//14
	{	"The Cave"	,	"The Cave(�Ϸ�)" ,					"LevelQuest\\Quest55.Msg" , "LevelQuest\\Quest55a.Msg" },	//15
	{	"The Cave"	,	"The Cave(�Ϸ�)" ,					"LevelQuest\\Quest55_2.Msg" , "LevelQuest\\Quest55_2a.Msg" }, //16
	{	"���̷����� ����"	,	"���̷����� ����(�Ϸ�)" ,	"LevelQuest\\Quest70.Msg" , "LevelQuest\\Quest70a.Msg" },	//17
	{	"���ε� ǻ��",	"���ε� ǻ��(�Ϸ�)" ,				"LevelQuest\\Quest80.Msg" , "LevelQuest\\Quest80a.Msg" },	//18
	{	"Į������ ����","Į������ ����(�Ϸ�)" ,				"LevelQuest\\Quest85.Msg" , "LevelQuest\\Quest85a.Msg" },	//19
	{	"���� ������","���� ������(�Ϸ�)" ,					"LevelQuest\\Quest90.Msg" , "LevelQuest\\Quest90a.Msg" },	//20

	//���ʽ� ���� ����Ʈ
	{	"�ձ��ǽ���"	,	"�ձ��ǽ���(�Ϸ�)" ,	"LevelQuest\\Quest_7State_1.msg" , "LevelQuest\\Quest_7State_end.msg" },	//21
	{	"�ձ��ǽ���"	,	"�ձ��ǽ���(�Ϸ�)" ,	"LevelQuest\\Quest_7State_2.msg" , "LevelQuest\\Quest_7State_end.msg" },	//22
	{	"�ձ��ǽ���"	,	"�ձ��ǽ���(�Ϸ�)" ,	"LevelQuest\\Quest_7State_3.msg" , "LevelQuest\\Quest_7State_end.msg" },	//23

	{	"�����÷�"	,	"�����÷�(�Ϸ�)" ,	"LevelQuest\\Quest_10State_1.msg" , "LevelQuest\\Quest_10State_end.msg" },	//24
	{	"�����÷�"	,	"�����÷�(�Ϸ�)" ,	"LevelQuest\\Quest_10State_2_1.msg" , "LevelQuest\\Quest_10State_end.msg" },//25
	{	"�����÷�"	,	"�����÷�(�Ϸ�)" ,	"LevelQuest\\Quest_10State_2_2.msg" , "LevelQuest\\Quest_10State_end.msg" },//26
	{	"�����÷�"	,	"�����÷�(�Ϸ�)" ,	"LevelQuest\\Quest_10State_2_3.msg" , "LevelQuest\\Quest_10State_end.msg" },//27

	//3�� ���� ����Ʈ
	{	"�������� 3��[1]"	,	"�������� 3��(�Ϸ�)" ,	"Quest4\\ChangeJob4_1.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//28
	{	"�������� 3��[2]"	,	"�������� 3��(�Ϸ�)" ,	"Quest4\\ChangeJob4_2.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//29
	{	"�������� 3��[3]"	,	"�������� 3��(�Ϸ�)" ,	"Quest4\\ChangeJob4_3_1.msg" ,	"Quest4\\ChangeJob4_3_2.msg" },	//30
	{	"�������� 3��[3]"	,	"�������� 3��(�Ϸ�)" ,	"Quest4\\ChangeJob4_3_3.msg" ,	"Quest4\\ChangeJob4_3_4.msg" },	//31
	{	"�������� 3��[3]"	,	"�������� 3��(�Ϸ�)" ,	"Quest4\\ChangeJob4_3_5.msg" ,	"Quest4\\ChangeJob4_3_6.msg" },	//32
	{	"�������� 3��[4]"	,	"�������� 3��(�Ϸ�)" ,	"Quest4\\ChangeJob4_4.msg" ,	"Quest4\\ChangeJob4_5.msg" }	//33

};
*/
char *mgRefuseWhisper = "�Ӹ��� �źεǾ����ϴ�";
char *mgWeightOver = "���踦 �ʰ��߽��ϴ�        ";

char *mgYahoo = "��ȣ";
char *mgContinueChat = "%s : �ʹ� ���� ���� �ߴ��� ���� ���� ���ȴ�! ��� -_-;";
char *mgRecvItem = "   ������ ( %s ) �� �޾ҽ��ϴ�   ";

char *mgItemTimeOut = "������ ( %s ) ���Ⱓ�� ���� �Ǿ� ���ŵǾ����ϴ�";
char *mgSOD_Clear = " �����մϴ�. ��� %d���� ȹ���߽��ϴ� ";


char *mgBlessCrystal_01 = "  ��ȯ�Ҽ� �ִ� ������ �ʰ��߽��ϴ�  ";
char *mgBlessCrystal_02 = "   ��ȯ�� �� ������ �Ҽ� �ֽ��ϴ�   ";
char *mgBlessCrystal_03 = "   ũ����Ż�� ����Ҽ� �����ϴ�   ";

char *mgGetQuestItem = "> (%s) �������� ȹ���߽��ϴ�";

// Bellatra
char *g_lpBellatraTeamNameText[] = 
{
	"���� �÷�",
	"���� �÷�",
	"�ٶ��� �÷�",
	"���� �÷�",
	"�¾��� ��",
	"���� ��",
};

char *g_lpBellatraResult_Text1 = "%s ��";
char *g_lpBellatraResult_Text2 = "%s �� %s ��";
char *g_lpBellatraResult_Text3 = "������ ����";
char *g_lpBellatraResult_Text4 = "���� ž���� �����Ͽ����ϴ�.";
