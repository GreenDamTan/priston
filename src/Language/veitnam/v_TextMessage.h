#include "TextMessage.h"

char *szAppName="Priston Tale";


char *mgRequestTrade = "G��i y�u c��u trao ���i t��i %s";
char *mgRequestTrade2 = "%s �a� �i qua� xa ��� co� th�� trao ���i";

char *mgDiconnect = "K��t n��i v��i Server bi� ng��t";
char *mgDiconnect1 = "K��t n��i v��i Server bi� ng��t (1)";
char *mgDiconnect2 = "K��t n��i v��i Server bi� ng��t (2)";
char *mgDiconnect3 = "K��t n��i v��i Server bi� ng��t (3)";
char *mgDiconnect4 = "K��t n��i v��i Server bi� ng��t (4)";

char *mgCloseGame = "�ang thoa�t kho�i tro� ch�i";
char *mgCloseWindow = "Vui lo�ng �o�ng c��a s�� na�y la�i  tr���c khi thoa�t.";
char *mgCloseBattle = "Kh�ng th�� thoa�t kho�i tro� ch�i   khi �ang chi��n ���u";
/*
char *mgQuestName[][4] = {
	{	"Rank up quest"	,	"Rank up quest(Completed)" , "ChangeJob1a.Msg" , "ChangeJob1b.Msg" },
	{	"Rank up quest"	,	"Rank-up Exam (Done)" , "ChangeJob2a.Msg" , "ChangeJob1b.Msg" },
	{	"Rank up quest"	,	"Rank up quest (Completed)" , "ChangeJob3a.Msg" , "ChangeJob1b.Msg" },

	{	"Rank up quest [Second]"		,	"Rank up quest [Second]" , "ChangeJob3-1.msg" , "ChangeJob3-2.msg" },
	{	"Rank up quest [Second]"		,	"Rank up quest (Completed)" , "ChangeJob3-3.msg" , "ChangeJob3-4.msg" },

	//Fighter Pikeman Mecanicion Knight
	{	"Rank up quest second[1]"		,	"Rank up quest second[2]"		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-W1-2.msg" },
	{	"Rank up quest second[3]"		,	"Rank up quest second[4]"		, "Quest3\\ChangeJob3-W2-2.msg" , "Quest3\\ChangeJob3-W3-2.msg" },
	{	"Rank up quest second[5]"		,	"Rank up quest second[Completed]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//Archer Atalanta
	{	"Rank up quest second[1]"		,	"Rank up quest second[2]"		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-R1-2.msg" },
	{	"Rank up quest second[3]"		,	"Rank up quest second[4]"		, "Quest3\\ChangeJob3-R2-2.msg" , "Quest3\\ChangeJob3-R3-2.msg" },
	{	"Rank up quest second[5]"		,	"Rank up quest second[Completed]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//Priestess Magicion
	{	"Rank up quest second[1]"		,	"Rank up quest second[2]"		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-M1-2.msg" },
	{	"Rank up quest second[3]"		,	"Rank up quest second[4]"		, "Quest3\\ChangeJob3-M2-2.msg" , "Quest3\\ChangeJob3-M3-2.msg" },
	{	"Rank up quest second[5]"		,	"Rank up quest second[Completed]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },
	
	//������ �u̽�Ʈ
	{	"For her"	,	"For her(completed)" ,	"LevelQuest\\Quest30.Msg" , "LevelQuest\\Quest30a.Msg" },	//14
	{	"The Cave"	,	"The Cave(completed)" ,					"LevelQuest\\Quest55.Msg" , "LevelQuest\\Quest55a.Msg" },	//15
	{	"The Cave"	,	"The Cave(completed)" ,					"LevelQuest\\Quest55_2.Msg" , "LevelQuest\\Quest55_2a.Msg" }, //16
	{	"Friendship with Michelle"	,	"Friendship with Michelle(completed)" ,	"LevelQuest\\Quest70.Msg" , "LevelQuest\\Quest70a.Msg" },	//17
	{	"Enclosed Fury",	"Enclosed Fury(completed)" ,				"LevelQuest\\Quest80.Msg" , "LevelQuest\\Quest80a.Msg" },	//18
	{	"Tears of Kaliha","Tears of Kaliha(completed)" ,				"LevelQuest\\Quest85.Msg" , "LevelQuest\\Quest85a.Msg" },	//19
	{	"Village of eura","Village of eura(completed)" ,					"LevelQuest\\Quest90.Msg" , "LevelQuest\\Quest90a.Msg" },	//20

	//���ʽ� ���� ����Ʈ
	{	"Test of Kingdom",	"Test of Kingdom(Complete)" ,	"LevelQuest\\Quest_7State_1.msg" , "LevelQuest\\Quest_7State_end.msg" },	//21
	{	"Test of Kingdom",	"Test of Kingdom(Complete)",	"LevelQuest\\Quest_7State_2.msg" , "LevelQuest\\Quest_7State_end.msg" },	//22
	{	"Test of Kingdom",	"Test of Kingdom(Complete)",	"LevelQuest\\Quest_7State_3.msg" , "LevelQuest\\Quest_7State_end.msg" },	//23

	{	"Bitter ordeal"	,	"Bitter ordeal(Complete)" ,	"LevelQuest\\Quest_10State_1.msg" , "LevelQuest\\Quest_10State_end.msg" },	//24
	{	"Bitter ordeal"	,	"Bitter ordeal(Complete)" ,	"LevelQuest\\Quest_10State_2_1.msg" , "LevelQuest\\Quest_10State_end.msg" },//25
	{	"Bitter ordeal"	,	"Bitter ordeal(Complete)" ,	"LevelQuest\\Quest_10State_2_2.msg" , "LevelQuest\\Quest_10State_end.msg" },//26
	{	"Bitter ordeal"	,	"Bitter ordeal(Complete)" ,	"LevelQuest\\Quest_10State_2_3.msg" , "LevelQuest\\Quest_10State_end.msg" },//27

	//3�� ���� ����Ʈ
	{	"Job Quest 3th[1]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_1.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//28
	{	"Job Quest 3th[2]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_2.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//29
	{	"Job Quest 3th[3]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_3_1.msg" ,	"Quest4\\ChangeJob4_3_2.msg" },	//30
	{	"Job Quest 3th[3]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_3_3.msg" ,	"Quest4\\ChangeJob4_3_4.msg" },	//31
	{	"Job Quest 3th[3]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_3_5.msg" ,	"Quest4\\ChangeJob4_3_6.msg" },	//32
	{	"Job Quest 3th[4]"	,	"Job Quest 3th(Complete)" ,	"Quest4\\ChangeJob4_4.msg" ,	"Quest4\\ChangeJob4_5.msg" }	//33

};
*/
char *mgRefuseWhisper = "Tin nh��n ri�ng �a� bi� t�� ch��i";
char *mgWeightOver = "Kh�ng th�� mang th�m ����c n��a ";

char *mgYahoo = "Yes!";
char *mgContinueChat = "%s : T�i no�i nhi��u ���n m��c c��ng ca� l���i la�i r��i! WOW q^_^p";
char *mgRecvItem = "  Ba�n �a� nh��n ����c mo�n ��� ( %s )  ";

char *mgItemTimeOut = "Mo�n ��� �a� bi� xoa� ( %s ) v���t qua� th��i ha�n s�� du�ng cho phe�p ";
char *mgSOD_Clear = " Chu�c m��ng! Ba�n �a� nh��n ����c gia�i th���ng( %d Va�ng) ";

char *mgBlessCrystal_01 = "  Gi��i ha�n cho vi��c tri��u t��p cu�a ba�n �a� h��t  ";
char *mgBlessCrystal_02 = "   Ba�n chi� co� th�� tri��u t��p m��i l��n m��t qua�i v��t   ";
char *mgBlessCrystal_03 = "   Ba�n kh�ng th�� s�� du�ng vi�n ngo�c na�y  ";


// Bellatra
char *g_lpBellatraTeamNameText[] = 
{
	"Th�� tha�ch cu�a N���c",
	"Th�� tha�ch cu�a ���t",
	"Th�� tha�ch cu�a Gio� ",
	"Th�� tha�ch cu�a L��a",
	"T��ng Nh��t Quan",
	"T��ng Nguy��t Quang",
};

char *g_lpBellatraResult_Text1 = "%s of";
char *g_lpBellatraResult_Text2 = "%s and %s of";
char *g_lpBellatraResult_Text3 = "Join team";
char *g_lpBellatraResult_Text4 = "Chuy��n l�n t��ng cao h�n.";
