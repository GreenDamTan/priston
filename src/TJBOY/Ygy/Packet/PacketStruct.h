/*===========================================================================================
============================================================================================*/
#ifndef __PACKETSTRUCT_H__
#define __PACKETSTRUCT_H__


#define STRING_MAX			(64+4)


//#define ID_MAX			64
#define CHANAME_MAX		64
#define CLANNAME_MAX    64
//#define CLANWON_MAX      90   //���� �ִ� Ŭ�������� 90��
//#define CLANWONLIST      3    //���� ���� 256������ �ȵ�



#define P_IDANDNAME				10001
#define P_OUT					10002
#define P_CLANINFO				10003


//���μ������� ��ȭ
#define INSIDE_IPPORT						20010 //���ӿ��� ������,��Ʈ�� ����
#define INSIDE_USERINFO						20001
#define INSIDE_USERINFOCHG					20030 //Ŭ���̾������ų� �ٲ��� db�� �ٲٱ�����.
#define INSIDE_GAME_OUT						20003
#define INSIDE_CLANCHAT                     20011 //Ŭ�� ä�ø޼���
#define INSIDE_VD		                    20012
#define GOPRIS_VUSERIDs         200030		//���̽����ѻ���� id��
#define GOPRIS_VCLIENTINFO      200031		//���̽�Ŭ���̾�Ʈ ����(���̽���������.���Ӿȵ�,���Ӳ�����,���������� ��..)
//priston->���̽�Ŭ���̾�Ʈ��..
#define INSIDE_MICONOFF         200050		//���׿��� ����ũ �¿������ش�.


#define INSIDE_GAMELOGIN					20000
#define INSIDE_GETWND                       20004
#define INSIDE_CLAWONLIST                   20005

#define INSIDE_DELCLANWON                20013 //Ŭ���� ����
#define INSIDE_DELCLAN                   20014 //Ŭ��   ����
#define INSIDE_MAKECLAN					 20015 //Ŭ��   �����


//Ŭ��->������ ������ �޽���
#define INSIDE_CLIENTINFO			250000   //��ó�� ����(������ ���Ӱ��ɿ��θ� �˼��ִ�)



//������ ���� �޼���
#define SERVER_CLANWONLS			30000
#define SERVER_CLANCHAT				30001
#define SERVER_CLWIN				30002
#define SERVER_CLWOUT				30003
#define SERVER_SENDVOC				30004






typedef struct _tagIndex
{
	DWORD		dwIndex;
}INDEX,*LPINDEX;


//ktj : OK
//���ӿ��� �����ִ� �����ǿ� ��Ʈ
//INSIDE_IPPORT						
typedef struct __tagIpPort
{
	INDEX           sIndex;
	char            ip[16];
	int				port;
	//HWND Chwnd;

}IPPORT,*LPIPPORT;

//INSIDE_USERINFO
//INSIDE_USERINFOCHG
typedef struct _tagUserInfo
{
	INDEX   sIndex;

	DWORD	dwClanNum;					//Ŭ����ȣ
	char	szClanName[STRING_MAX];		//Ŭ���̸�

	int     utype;						//Ŭ������ ����attr 
	char	szCharName[STRING_MAX];		//������ ĳ���� �̸�.
	char	szID      [STRING_MAX];		//���� �Ƶ�
	char	szConnServerName[16];		//������ �����̸��� ����ִ� ����
}USERINFO;


//INSIDE_GAME_OUT
typedef struct __tagPacketOut
{
	INDEX			sIndex;			//��Ÿ����Ÿ�� �ʿ����.
}P_Out;


//INSIDE_CLANCHAT Ŭ�� ä�� ��Ʈ��ó
typedef struct __tagClanChat
{
	INDEX		sIndex;
	char		szMsg[256];
}CLANCHAT;


//INSIDE_VD    ktj : voice�� ����Ÿũ�Ⱑ �������̶� �̷��� �Ѵ�.
typedef struct
{
	INDEX	sIndex;
	int		vSize;
	char	vData[4];
}VOICEDATA;


//������������ sendMessage����ذ��µ���Ÿ ======================= 
//GOPRIS_VUSERIDs    //���ϴ������� ����Ÿ
#define STREAMDATACNT 8 //���� : packetstruct.h�� ���ƾ���.
typedef struct
{
	INDEX	sIndex;
	DWORD	time;			//2�������� Ÿ�̸�ýũ������ ����Ÿ�� ��������ѿ뵵��.
	char	uID[STREAMDATACNT][32];		//����8��
}VOICTALKUSERDATA;
//VOICTALKUSERDATA vUserData;

//INSIDE_MICONOFF
typedef struct
{
	INDEX	sIndex;
	BOOL	bMicONOFF;
}VOICEONOFF;





//�Ʒ� ��Ʈ��ó�� ����
enum {
	VC_STATE_NOSERVER,		//���̽������� ����.
	VC_STATE_NOTCONNECT,	//���̽������� �����Ҽ� ����.
	VC_STATE_CONNECT,		//���̽������� connect
	VC_STATE_DISCONNECT,	//���̽������� disconnect
};
#define VC_STATE_NOSERVER_STR	"���̽������� ����."
#define VC_STATE_NOTCONNECT_STR	"���̽������� �����Ҽ� ����."
#define VC_STATE_CONNECT_STR	"���̽������� connect"
#define VC_STATE_DISCONNECT_STR	"���̽������� disconnect"
//GOPRIS_VCLIENTINFO
typedef struct
{
	INDEX	sIndex;
	int		vcl_state;
}VCLIENTINFO;










/*================================================================
�������� ��������Ÿ
=================================================================*/
typedef struct {
	int     utype;						//Ŭ������ ����attr 
	char	szCharName[STRING_MAX];		//������ ĳ���� �̸�.
	char	szID      [STRING_MAX];		//���� �Ƶ�
}SERVER_clanWon;
//SERVER_CLANWONLS
typedef struct __tagServer_ClanWonList
{
	INDEX               sIndex;
	int					clanwonSu;
	SERVER_clanWon		sclanwon[1];
}SERVER_CLANWONLIST;


//SERVER_CLANCHAT
typedef struct __tagServer_ClanChat
{
	INDEX               sIndex;
	SERVER_clanWon      clanwon;
	char				chat[256];
}SERVER_CLANCHATSTR;


//SERVER_CLWIN  SERVER_CLWOUT
typedef struct __tagServer_outClanWon
{
	INDEX               sIndex;
	SERVER_clanWon      clanwon;
}SERVER_inoutCLANWON;

//SERVER_SENDVOC    ktj : voice�� ����Ÿũ�Ⱑ �������̶� �̷��� �Ѵ�.
typedef struct
{
	INDEX				sIndex;
	SERVER_clanWon		clanwon;
	int					vSize;
	char				vData[4];
}SERVER_VOICEDATA;

















/*
//===============================================================
//���� �α��� �Ҷ�
//================================================================

typedef struct _tagGameLogin
{
	INDEX     index;
	char      szID[CHANAME_MAX+1];
}GAMELOGIN,*LPGAMELOGIN;



//=====================================================
// User�� ó�� �����Ҷ�
// User�� ���̵�� ĳ���� �̸��� ����
//=====================================================
typedef struct _tagIDandChaName
{
	char szID[ID_MAX+1];
	char szChaName[CHANAME_MAX+1];
}IDandCHANAME,LPIDandCHANAME;

typedef struct _tagPacketIDandChaName
{
	INDEX			sIndex;
	IDandCHANAME	sIDandChaName;

}P_IDandCHANANE;



//=====================================================
// User�� Ŭ�� ������ �����Ҷ�
// Ŭ���� ��ȣȭ Ŭ�� �̸��� ����
//=====================================================
typedef struct _tagClanInfo
{
	DWORD    dwClanNum;
	char     szClanName[CLANNAME_MAX+1];
}USER_CLANINFO;

typedef struct __tagClanInfo
{
	INDEX			     sIndex;
	USER_CLANINFO        ClanInfo;

}Packet_CLANINFO;







//====================================================================wav
//ktj : �Ϻ� ����
//#define WAV_SIZE  512 //256  //150
//#define WAV_SIZE  10000  //150
//#define WAV_SIZE  1024 //256  //150
#define WAV_SIZE  4096//2048 //256  //150

//ktj : ���̺� ����κ��� ����� ýũ�ϱ� ���� �־�а���.
typedef struct
{
	INDEX  sIndex;
	WORD  flag;						//������ ����Ÿ�ΰ�� 1��
	WORD  cnt;
	int  wavDataSize;
	char szChaName[CHANAME_MAX+1];
}WAVDATAHEADERchk;


typedef struct __tagWavData
{
	INDEX  sIndex;
	WORD  flag;						//������ ����Ÿ�ΰ�� 1��
	WORD  cnt;
	int  wavDataSize;
	char szChaName[CHANAME_MAX+1];
	char WavData[WAV_SIZE+1];
}WAVDATA,*LPWAVDATA;
//==============================================================//






typedef struct __tagWavChaName
{
	INDEX		sIndex;	
	char		szChaName[CHANAME_MAX+1];
}WAVCHANAME,*LPWAVCHANAME;

//Ŭ������ Ŭ������ ����
typedef struct __tagDeleteClanWon
{
	INDEX		sIndex;	
	char		szChaName[CHANAME_MAX+1];
	char		szClanName[CLANNAME_MAX+1];
	DWORD       dwClanNum;
}DELETECLANWON,*LPDELETECLANWON;

//Ŭ���� ����
typedef struct __tagDeleteClan
{
	INDEX		sIndex;	
	char		szClanName[CLANNAME_MAX+1];
	DWORD       dwClanNum;
}DELETECLAN,*LPDELETECLAN;


//Ŭ�� �����
typedef struct _TagMakeClan
{
	INDEX     index;
	DWORD     dwClanNum;
	char      szClanName[CLANNAME_MAX+1];
	char      szChaName[CHANAME_MAX+1];
	char      szID[CHANAME_MAX+1];
}MAKECLAN,*LPMAKECLAN;
*/



#endif //
