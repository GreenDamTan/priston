#include "TextMessage.h"

char *szAppName=" Mundo Priston ";


char *mgRequestTrade = " Se envi� un pedido de intercambio a %s ";
char *mgRequestTrade2 = " %s est� demasiado lejos para pedir un intercambio ";

char *mgDiconnect = " Se perdi� la conexi�n con el Servidor ";
char *mgDiconnect1 = " Se perdi� la conexi�n con el Servidor (1) ";
char *mgDiconnect2 = " Se perdi� la conexi�n con el Servidor (2) ";
char *mgDiconnect3 = " Se perdi� la conexi�n con el Servidor (3) ";
char *mgDiconnect4 = " Se perdi� la conexi�n con el Servidor (4) ";

char *mgCloseGame = " Abandonando el Juego... ";
char *mgCloseWindow = " Por favor, cierra la ventana actual antes de salir del juego ";
char *mgCloseBattle = " No se puede abandonar mientras se combate ";
/*
char *mgQuestName[][4] = {
	{	" Misi�n de cambio de Rango "	,	" Misi�n de Rank up(Completada) " , "ChangeJob1a.Msg" , "ChangeJob1b.Msg" },
	{	" Misi�n de cambio de Rango ",	" Examen de Rank-up(Hecho) " , "ChangeJob2a.Msg" , "ChangeJob1b.Msg" },
	{	" Misi�n de cambio de Rango "	,	" Misi�n de Rank up(Completada) " , "ChangeJob3a.Msg" , "ChangeJob1b.Msg" },

	{	" Misi�n de cambio de Rango [Segunda] "		,	" Misi�n de cambio de Rango[Segunda] " , "ChangeJob3-1.msg" , "ChangeJob3-2.msg" },
	{	" Misi�n de cambio de Rango [Segunda] "		,	" Misi�n de cambio de Rango(Completada) " , "ChangeJob3-3.msg" , "ChangeJob3-4.msg" },

	//Luchador Piquero Mec�nico Caballero
	{	"Misi�n de cambio de Rango segunda[1] "		,	"Misi�n de cambio de Rango segunda[2] "		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-W1-2.msg" },
	{	"Misi�n de cambio de Rango segunda[3] "		,	"Misi�n de cambio de Rango segunda[4] "		, "Quest3\\ChangeJob3-W2-2.msg" , "Quest3\\ChangeJob3-W3-2.msg" },
	{	"Misi�n de cambio de Rango segunda[5] "		,	"Misi�n de cambio de Rango segunda[Completada] "	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//Arquero Atalanta
	{	"Misi�n de cambio de Rango segunda[1] "		,	"Misi�n de cambio de Rango segunda[2] "		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-R1-2.msg" },
	{	"Misi�n de cambio de Rango segunda[3] "		,	"Misi�n de cambio de Rango segunda[4] "		, "Quest3\\ChangeJob3-R2-2.msg" , "Quest3\\ChangeJob3-R3-2.msg" },
	{	"Misi�n de cambio de Rango segunda[5] "		,	"Misi�n de cambio de Rango segunda[Completada] "	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//Sacerdotisa Mago
	{	"Misi�n de cambio de Rango segunda[1] "		,	"Misi�n de cambio de Rango segunda[2] "		, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-M1-2.msg" },
	{	"Misi�n de cambio de Rango segunda[3] "		,	"Misi�n de cambio de Rango segunda[4] "		, "Quest3\\ChangeJob3-M2-2.msg" , "Quest3\\ChangeJob3-M3-2.msg" },
	{	"Misi�n de cambio de Rango segunda[5] "		,	"Misi�n de cambio de Rango segunda[Completada] "	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//������ ����Ʈ
	{	" Para ella "	,	" Para ella(completada) " ,	"LevelQuest\\Quest30.Msg" , "LevelQuest\\Quest30a.Msg" },	//14
	{	" La Cueva "	,	" La Cueva(completada) " ,					"LevelQuest\\Quest55.Msg" , "LevelQuest\\Quest55a.Msg" },	//15
	{	" La Cueva "	,	" La Cueva(completada) " ,					"LevelQuest\\Quest55_2.Msg" , "LevelQuest\\Quest55_2a.Msg" }, //16
	{	" Ayudando a Michel"	,	" Ayudando a Michel(completada) " ,	"LevelQuest\\Quest70.Msg" , "LevelQuest\\Quest70a.Msg" },	//17
	{	" Furia Encerrada ",	" Furia Encerrada(completada) " ,				"LevelQuest\\Quest80.Msg" , "LevelQuest\\Quest80a.Msg" },	//18
	{	" L�grimas de Chalia "," L�grimas de Chalia(completada) " ,				"LevelQuest\\Quest85.Msg" , "LevelQuest\\Quest85a.Msg" },	//19
	{	" Villa de Eura "," Villa de Eura(completada) " ,					"LevelQuest\\Quest90.Msg" , "LevelQuest\\Quest90a.Msg" },	//20

	//���ʽ� ���� ����Ʈ
	{	" Prueba del Reino ",	" Prueba del Reino(Completada) " ,	"LevelQuest\\Quest_7State_1.msg" , "LevelQuest\\Quest_7State_end.msg" },	//21
	{	" Prueba del Reino ",	" Prueba del Reino(Completada) ",	"LevelQuest\\Quest_7State_2.msg" , "LevelQuest\\Quest_7State_end.msg" },	//22
	{	" Prueba del Reino ",	" Prueba del Reino(Completada) ",	"LevelQuest\\Quest_7State_3.msg" , "LevelQuest\\Quest_7State_end.msg" },	//23

	{	" Amargo Sufrimiento "	,	" Amargo Sufrimiento(Completada) " ,	"LevelQuest\\Quest_10State_1.msg" , "LevelQuest\\Quest_10State_end.msg" },	//24
	{	" Amargo Sufrimiento "	,	" Amargo Sufrimiento(Completada) " ,	"LevelQuest\\Quest_10State_2_1.msg" , "LevelQuest\\Quest_10State_end.msg" },//25
	{	" Amargo Sufrimiento "	,	" Amargo Sufrimiento(Completada) " ,	"LevelQuest\\Quest_10State_2_2.msg" , "LevelQuest\\Quest_10State_end.msg" },//26
	{	" Amargo Sufrimiento "	,	" Amargo Sufrimiento(Completada) " ,	"LevelQuest\\Quest_10State_2_3.msg" , "LevelQuest\\Quest_10State_end.msg" },//27

	//3�� ���� ����Ʈ
	{	" Misi�n de Trabajo 3[1] "	,	" Misi�n de Trabajo 3(Completada) " ,	"Quest4\\ChangeJob4_1.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//28
	{	" Misi�n de Trabajo 3[2] "	,	" Misi�n de Trabajo 3(Completada) " ,	"Quest4\\ChangeJob4_2.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//29
	{	" Misi�n de Trabajo 3[3] "	,	" Misi�n de Trabajo 3(Completada) " ,	"Quest4\\ChangeJob4_3_1.msg" ,	"Quest4\\ChangeJob4_3_2.msg" },	//30
	{	" Misi�n de Trabajo 3[3] "	,	" Misi�n de Trabajo 3(Completada) " ,	"Quest4\\ChangeJob4_3_3.msg" ,	"Quest4\\ChangeJob4_3_4.msg" },	//31
	{	" Misi�n de Trabajo 3[3] "	,	" Misi�n de Trabajo 3(Completada) " ,	"Quest4\\ChangeJob4_3_5.msg" ,	"Quest4\\ChangeJob4_3_6.msg" },	//32
	{	" Misi�n de Trabajo 3[4] "	,	" Misi�n de Trabajo 3(Completada) " ,	"Quest4\\ChangeJob4_4.msg" ,	"Quest4\\ChangeJob4_5.msg" }	//33

};
*/
char *mgRefuseWhisper = " El Mensaje Privado fue rehusado ";
char *mgWeightOver = " Sobrepasado ";

char *mgYahoo = " !S�! ";
char *mgContinueChat = " %s : hablo demasiado, !mi lengua se congel�! WOW -_-; ";
char *mgRecvItem = "  Has recibido el objeto( %s )  ";

char *mgItemTimeOut = " El objeto fue borrado por su tiempo de validez ";
char *mgSOD_Clear = " !Felicitaciones!!! Ganaste el premio de ( %dGold) ";


char *mgBlessCrystal_01 = "  Se ha excedido la cantidad de invocaciones  ";
char *mgBlessCrystal_02 = "   Puedes invocar s�lo una vez   ";
char *mgBlessCrystal_03 = "   No puedes usar este cristal   ";


// Bellatra
char *g_lpBellatraTeamNameText[] =
{
	" Tribulaci�n de Agua ",
	" Tribulaci�n de Suelo ",
	" Tribulaci�n de Viento ",
	" Tribulaci�n de Fuego ",
	" Escenario del Sol ",
	" Escenario de la Luna ",
};

char *g_lpBellatraResult_Text1 = " %s de ";
char *g_lpBellatraResult_Text2 = " %s y %s de ";
char *g_lpBellatraResult_Text3 = " Une equipo ";
char *g_lpBellatraResult_Text4 = " Cambio de Rango para la Torre Alta. ";
