#include "TextMessage.h"

char *szAppName="�v���X�g���e�[��";


char *mgRequestTrade = "%s�l�Ɏ����\�����݂܂����B";
char *mgRequestTrade2 = "%s��͋������������߁A�����\�����߂܂���B";

char *mgDiconnect  = "�T�|�o�|�Ƃ̐ڑ����؂�܂����B";
char *mgDiconnect1 = "�T�|�o�|�Ƃ̐ڑ����؂�܂����B(1)";
char *mgDiconnect2 = "�T�|�o�|�Ƃ̐ڑ����؂�܂����B(2)";
char *mgDiconnect3 = "�T�|�o�|�Ƃ̐ڑ����؂�܂����B(3)";
char *mgDiconnect4 = "�T�|�o�|�Ƃ̐ڑ����؂�܂����B(4)";

char *mgCloseGame = "�Q�|�����I�����܂��B";
char *mgCloseWindow = "�g�p���̃E�B���h�E����Ă���I�����Ă��������B";
char *mgCloseBattle = "�퓬���͏I���ł��܂���B";
/*
char *mgQuestName[][4] = {
	{"�]�E�N�G�X�g","�]�E�N�G�X�g(����)","ChangeJob1a.Msg","ChangeJob1b.Msg"},
	{"�]�E�N�G�X�g[���[�����[���[]","�]�E�N�G�X�g(����)" , "ChangeJob2a.Msg" , "ChangeJob1b.Msg" },
	{"�]�E�N�G�X�g[���э�]","�]�E�N�G�X�g(����)" , "ChangeJob3a.Msg" , "ChangeJob1b.Msg" },
	
	{	"�]�E�N�G�X�g[����]"	,	"�]�E�N�G�X�g[����]" , "ChangeJob3-1.msg" , "ChangeJob3-2.msg" },
	{	"�]�E�N�G�X�g[����]"	,	"�]�E�N�G�X�g(����)" , "ChangeJob3-3.msg" , "ChangeJob3-4.msg" },

	//�t�@�C�^�[�@�p�C�N�}���@���J�j�V�����@�i�C�g
	{	"�]�E�N�G�X�g�Q��[1]"	,	"�]�E�N�G�X�g�Q��[2]"	, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-W1-2.msg" },
	{	"�]�E�N�G�X�g�Q��[3]"	,	"�]�E�N�G�X�g�Q��[4]"	, "Quest3\\ChangeJob3-W2-2.msg" , "Quest3\\ChangeJob3-W3-2.msg" },
	{	"�]�E�N�G�X�g�Q��[5]"	,	"�]�E�N�G�X�g�Q��[����]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//�A�[�`���[�@�A�^�����^
	{	"�]�E�N�G�X�g�Q��[1]"	,	"�]�E�N�G�X�g�Q��[2]"	, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-R1-2.msg" },
	{	"�]�E�N�G�X�g�Q��[3]"	,	"�]�E�N�G�X�g�Q��[4]"	, "Quest3\\ChangeJob3-R2-2.msg" , "Quest3\\ChangeJob3-R3-2.msg" },
	{	"�]�E�N�G�X�g�Q��[5]"	,	"�]�E�N�G�X�g�Q��[����]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },

	//�v���X�e�B�X�@�}�W�V����
	{	"�]�E�N�G�X�g�Q��[1]"	,	"�]�E�N�G�X�g�Q��[2]"	, "Quest3\\ChangeJob3-1.msg" , "Quest3\\ChangeJob3-M1-2.msg" },
	{	"�]�E�N�G�X�g�Q��[3]"	,	"�]�E�N�G�X�g�Q��[4]"	, "Quest3\\ChangeJob3-M2-2.msg" , "Quest3\\ChangeJob3-M3-2.msg" },
	{	"�]�E�N�G�X�g�Q��[5]"	,	"�]�E�N�G�X�g�Q��[����]"	, "Quest3\\ChangeJob3-3.msg" , "Quest3\\ChangeJob3-4.msg" },
	
	//������ ����Ʈ
	{	"�ޏ��B�̂��߂�"		,	"�ޏ��B�̂��߂�(����)"	, "LevelQuest\\Quest30.Msg" , "LevelQuest\\Quest30a.Msg" },	//14
	{	"The Cave"		,	"The Cave(����)" 		, "LevelQuest\\Quest55.Msg" , "LevelQuest\\Quest55a.Msg" },	//15
	{	"The Cave"		,	"The Cave(����)"		, "LevelQuest\\Quest55_2.Msg" , "LevelQuest\\Quest55_2a.Msg" }, //16
	{	"�~�P���Ƃ̗F��"		,	"�~�P���Ƃ̗F��(����)"	, "LevelQuest\\Quest70.Msg" , "LevelQuest\\Quest70a.Msg" },	//17
	{	"���󂳂ꂽ�t���[���["	,	"���󂳂ꂽ�t���[���[(����)"	, "LevelQuest\\Quest80.Msg" , "LevelQuest\\Quest80a.Msg" },	//18
	{	"�J���A�̗�"		,	"�J���A�̗�(����)" 		, "LevelQuest\\Quest85.Msg" , "LevelQuest\\Quest85a.Msg" },	//19
	{	"�����r���b�W"		,	"�����r���b�W(����)"	, "LevelQuest\\Quest90.Msg" , "LevelQuest\\Quest90a.Msg" },	//20
	
	//���ʽ� ���� ����Ʈ
	{	"�����̎���"	,	"�����̎����i�����j" ,	"LevelQuest\\Quest_7State_1.msg" , "LevelQuest\\Quest_7State_end.msg" },	//21
	{	"�����̎���"	,	"�����̎����i�����j" ,	"LevelQuest\\Quest_7State_2.msg" , "LevelQuest\\Quest_7State_end.msg" },	//22
	{	"�����̎���"	,	"�����̎����i�����j" ,	"LevelQuest\\Quest_7State_3.msg" , "LevelQuest\\Quest_7State_end.msg" },	//23

	{	"�������"	,	"��������i�����j" ,	"LevelQuest\\Quest_10State_1.msg" , "LevelQuest\\Quest_10State_end.msg" },	//24
	{	"�������"	,	"��������i�����j" ,	"LevelQuest\\Quest_10State_2_1.msg" , "LevelQuest\\Quest_10State_end.msg" },//25
	{	"�������"	,	"��������i�����j" ,	"LevelQuest\\Quest_10State_2_2.msg" , "LevelQuest\\Quest_10State_end.msg" },//26
	{	"�������"	,	"��������i�����j" ,	"LevelQuest\\Quest_10State_2_3.msg" , "LevelQuest\\Quest_10State_end.msg" },//27

	//3E��E�E����Ʈ
	{	"�]�E�e�X�g3��[1]"	,	"�]�E�e�X�g3���i�����j" ,	"Quest4\\ChangeJob4_1.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//28
	{	"�]�E�e�X�g3��[2]"	,	"�]�E�e�X�g3���i�����j" ,	"Quest4\\ChangeJob4_2.msg" ,	"Quest4\\ChangeJob4_5.msg" },	//29
	{	"�]�E�e�X�g3��[3]"	,	"�]�E�e�X�g3���i�����j" ,	"Quest4\\ChangeJob4_3_1.msg" ,	"Quest4\\ChangeJob4_3_2.msg" },	//30
	{	"�]�E�e�X�g3��[3]"	,	"�]�E�e�X�g3���i�����j" ,	"Quest4\\ChangeJob4_3_3.msg" ,	"Quest4\\ChangeJob4_3_4.msg" },	//31
	{	"�]�E�e�X�g3��[3]"	,	"�]�E�e�X�g3���i�����j" ,	"Quest4\\ChangeJob4_3_5.msg" ,	"Quest4\\ChangeJob4_3_6.msg" },	//32
	{	"�]�E�e�X�g3��[4]"	,	"�]�E�e�X�g3���i�����j" ,	"Quest4\\ChangeJob4_4.msg" ,	"Quest4\\ChangeJob4_5.msg" }	//33

};
*/
char *mgRefuseWhisper = "�v���C�x�[�g�`���b�g�����ۂ���܂���";
char *mgWeightOver = "�d�ʂ𒴉߂��܂���        ";

char *mgYahoo = "���b�z�`";
char *mgContinueChat = "%s�F�b���߂��Đオ�ł܂��Ă��܂����I����[-_-;";
char *mgRecvItem = "   �A�C�e��( %s ) ���󂯎��܂���  ";

char *mgItemTimeOut = "�A�C�e�� ( %s ) �g�p���Ԃ��߂��ď�������܂���";
char *mgSOD_Clear = " ���߂łƂ��������܂��B�܋� %dG���l�����܂��� ";

char *mgBlessCrystal_01 = "  �����ł�����𒴂��܂����B  ";
char *mgBlessCrystal_02 = "   ������1��݂̂ł��܂��B   ";
char *mgBlessCrystal_03 = "   �N���X�^�����g�p�ł��܂���B   ";

// Bellatra
char *g_lpBellatraTeamNameText[] = 
{
	"���̎���",
	"�y�̎���",
	"���̎���",
	"�΂̎���",
	"���z�̊�",
	"���̊�",
};

char *g_lpBellatraResult_Text1 = "%s ��";
char *g_lpBellatraResult_Text2 = "%s �� %s ��";
char *g_lpBellatraResult_Text3 = "�Q�������`�[����";
char *g_lpBellatraResult_Text4 = "��ʂ̓��֐i�݂܂���";
