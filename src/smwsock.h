#ifndef	_SMWSOCK_HEADER

#define	_SMWSOCK_HEADER

#include "nettype.h"


#define _PS_TALE

#ifdef _PS_TALE
//�������� ���� ��Ʈ��ũ
#define TCP_GAMEPORT		8180
//#define TCP_SERVPORT		9110
#include "Language\\language.h"

#ifdef	_LANGUAGE_KOREAN				//�ѱ���

#ifdef	_TEST_SERVER
//�׽�Ʈ ����
#define TCP_SERVPORT		10009
#else
#define TCP_SERVPORT		10003
#endif

#endif

#ifdef	_LANGUAGE_CHINESE				//�߱���
#define TCP_SERVPORT		10004
#endif

#ifdef	_LANGUAGE_TAIWAN				//��ȭ�α���
#define TCP_SERVPORT		10005
#endif

#ifdef	_LANGUAGE_JAPANESE				//�Ϻ���
#define TCP_SERVPORT		10006
#endif

#ifdef	_LANGUAGE_ENGLISH				//����

#ifdef	_LANGUAGE_PHILIPIN				//����
#define TCP_SERVPORT		10010
#else 
#define TCP_SERVPORT		10009
#endif
#endif

#ifdef _LANGUAGE_VEITNAM				//��Ʈ��
#define TCP_SERVPORT		10011
#endif

#ifdef _LANGUAGE_BRAZIL					//�����
#define TCP_SERVPORT		10012
#endif

#ifdef _LANGUAGE_ARGENTINA				//�Ƹ���Ƽ��
#define TCP_SERVPORT		10014
#endif

#ifdef	_LANGUAGE_THAI					//�±�
#define TCP_SERVPORT		10008
#endif


//�ִ� ���� ���ϼ� ( 100 �� - ������ )


#ifdef _W_SERVER

#ifdef _W_DATA_SERVER
#define CONNECTMAX			4096
#define	CONNECTMAX_MASK		4095
#else
//����
#define CONNECTMAX			1024
#define	CONNECTMAX_MASK		1023
#endif


#else
//Ŭ���̾�Ʈ
#define CONNECTMAX			64
#define	CONNECTMAX_MASK		63

#endif


#else
//�ͻ���Ʈ ���Ӽ���
//�ִ� ���� ���ϼ� ( 2000 �� - ������ )
#define CONNECTMAX			1024
#define	CONNECTMAX_MASK		1023

#endif


//�ִ� ��Ŷ ũ�� ( 8K Byte )
//#define smSOCKBUFF_SIZE	4096
#define smSOCKBUFF_SIZE	8192

extern int		smTransThreadMode;
extern int		smTransTurbRcvMode;

class smWINSOCK ;

struct smTHREADSOCK {
	smWINSOCK *smMySock;

	SOCKET Sock;

	int RecvLen;
	int RecvCode;

	int BuffCnt;

	int NewData;
	int Sucess;
	int Sleep;
	int SendReady;

	DWORD dwExitThread;
	DWORD dwAcessTime;

	char Buff[smSOCKBUFF_SIZE+256];
};


struct smTHREAD_ROUTE
{
	smTHREAD_ROUTE	*MyPoint;
	int Sleep;
	int TimeOut;
	smTHREADSOCK	*smThreadSock;
	DWORD dwSleepTime;

	HANDLE	hThread;
};

#define	ENC_PACKET_REC_MAX		16
#define	ENC_PACKET_REC_MASK		15

class smWINSOCK {
public:
	smWINSOCK *MyPoint;
	DWORD	MyNum;

	SOCKET sock;

	void *ExtData1;					//Ȯ�� ����Ÿ ������ 1
	void *ExtData2;					//Ȯ�� ����Ÿ ������ 2
	void *ExtData3;					//Ȯ�� ����Ÿ ������ 3
	void *ExtData4;					//Ȯ�� ����Ÿ ������ 4

	HANDLE hRecvThread;
	HANDLE hSendThread;

	smTHREADSOCK smRecvState;
	smTHREADSOCK smSendState;

	smTHREAD_ROUTE	*lpsmRecvRoute;		//�ܺ� ������ ���
	smTHREAD_ROUTE	*lpsmSendRoute;		//�ܺ� ������ ���

	int online;
	int status;
	char szBuff[128];
	char szIPAddr[16];
	DWORD	dwPort;

	DWORD	dwLastRecvTime;			//�ֱ� ���� �ð�
	DWORD	dwDeadLockTime;			//����� ýũ �ð�

	int	WaitReadMessage;			//Read �޼��� ��� ī����
	int	PostReadMessage;			//Read �޼��� �߰� ī����

	int	PacketSendQueCnt;			//������ ��Ŷť�� ����� ����Ÿ

	int	EncPacketSendCount;			//��ȣȭ ��Ŷ �۽� ī����
	int	EncPacketRecvCount;			//��ȣȭ ��Ŷ ���� ī����

	int	EncRecvPackets[ENC_PACKET_REC_MAX];			//�ֱ� ���� ��ȣ ��Ŷ 16���� ���
	int	EncRecvPacketCnt;							//�ֱ� ��ȣ ��Ŷ ��

	int ConnectDirection;			//���� ����	( 0<-����� 1-������ )

	SOCKADDR_IN local_sin;  /* Local socket - internet style */
	SOCKADDR_IN acc_sin;    /* Accept socket address - internet style */
	int acc_sin_len;        /* Accept socket address length */

	DWORD dwRecvSusCount;

	int	RecvPacketCount;		//���� ��Ŷ ī����
	int	SendPacketCount;		//���� ��Ŷ ī����
	int	RecvPopCount;			//������ �ޱ� �õ� ī��Ʈ
	int	RecvPopErrorCount;		//������ �ޱ� �õ� ���� ī��Ʈ
	int SendPacketErrorCount;	//���� ��Ŷ ���� ī����
	int RecvPacketErrorCount;	//���� ��Ŷ ���� ī����

	DWORD	dwEncPacketCode;	//��Ŷ ������ȣȭ �ڵ�
	BYTE	bEncXor;			//��Ŷ ������ȣȭ Xor
	BYTE	bDecXor;			//��Ŷ ������ȣȭ Xor


public:
	smWINSOCK();
	~smWINSOCK();

	int mesAccept( WPARAM wParam , LPARAM lParam );
	int mesReceve( LPARAM lParam );

	int Send( char *SendData , int len , int ImpMode=0 );
	int Send2( char *SendData , int len , int ImpMode=0 );		//��Ŷ�� ��ȣȭ �Ͽ� ����
	int Send3( char *SendData , int len , int ImpMode=0 );		//��Ŷ�� ��ȣȭ �Ͽ� ����

	int	CheckEncRecvPacket( int RecvCount );					//�ֱ� ��Ŷ�� ���������� Ȯ��
	int	PushEncRecvPacket( int RecvCount );						//�ֱ� ��Ŷ��ȣ ���

	//���� ����
	int CloseSocket();
	//������ �翬�� ��Ų��
	int ReconnectSock();
	//����
	int ConnectSock( char *szIP , WORD port );
	//�ְ� ���� ������ ����
	int CreateTransThread( int Mode );



};


extern smWINSOCK	smWSock[CONNECTMAX];

#define WSA_ACCEPT			(WM_USER + 0)
#define WSA_READ			(WM_USER + 1)
#define SWM_SENDSUCCESS     (WM_USER + 10)
#define SWM_RECVSUCCESS     (WM_USER + 11)
#define SWM_MCONNECT		(WM_USER + 12)
#define SWM_MDISCONNECT		(WM_USER + 13)


//��� ���� üũ ī����
extern	int	smTransRecvErrorCnt;
extern	int	smTransSendErrorCnt;
extern	int	smTransRecvErrorDisCnt;
extern	int	smTransSendErrorDisCnt;
extern	int smTransClearErrorCnt;
extern	int smRcvBuffOverCount;

//���� ���ε�
void InitBindSock( int port );
//���� ���� �ʱ�ȭ
void InitGameSocket( int ThreadMode=0 );
//���ε� ���� ����
void CloseBindSock();
//�ڽ��� IP�ּ� ����
char *smGetMyIp();
//�ڽ��� IP�ּ� ����
DWORD smGetMyIp2();
//dwIP�� string ���� ��ȯ
char *ConvStrIP( DWORD dwIP );

//WSA�޼��� ó��
int WSAMessage_Accept( DWORD wParam , DWORD lParam );
int WSAMessage_Read( DWORD wParam , DWORD lParam );

//������ ���� ��Ų��
smWINSOCK *smConnectSock( char *szIP , WORD port );
//������ ���� ��Ų�� ( ũ��Ƽ�� ���� )
smWINSOCK *smConnectSock3( char *szIP , WORD port );

//������ ���� ��Ų�� ( IP�� Ȯ���Ͽ� ������ ���� IP�� �����ϸ� ���� IP ���� �Ұ� )
smWINSOCK *smConnectSock2( char *szIP , WORD port );
//IP�� ���� ã��
smWINSOCK *smFindSocket( char *szIP );
//32��Ʈ IP�� ������ ã��
smWINSOCK *smFindSocket2( DWORD dwIP );
//������ ���� �ٽ� ���� ��Ų��
int smReconnectSock( smWINSOCK *lpsmSock );

//������ ��� ���ť�� ������ ��� ��Ų�� 
int PushTransSendSocket( smWINSOCK *lpsmsock );
//������ ��� ���ť���� ��� ������ �����´� 
smWINSOCK *PopTransSendSocket();
//�޴� ��� ���ť�� ������ ��� ��Ų�� 
int PushTransRecvSocket( smWINSOCK *lpsmsock );
//�޴� ��� ���ť���� ��� ������ �����´� 
smWINSOCK *PopTransRecvSocket();
//ť�� ������� ������ ã�� �����Ѵ�
int ClearTransQueSocket( smWINSOCK *lpsmsock );
//ť�� �ִ� ���� ���� Ȯ�� �Ͽ� ��� ���� ( ������ ������ ��� )
int ClearPacketSendQue( smWINSOCK *lpsmSock );

//�̻� �ִ� �����带 ã�� ó�� �Ѵ�
int CheckLostTransThread();
//������ �������� ��� ���� ����
int GetUseingSendThreadCount();
//�ޱ� �������� ��� ���� ����
int GetUseingRecvThreadCount();

//���Ͽ� ũ��ƼĮ ���� ����
void smEnterCSectionSocket();
//���Ͽ� ũ��ƼĮ ���� ����
void smLeaveCSectionSocket();

//�������� �޼����� �ִ��� Ȯ���Ͽ� ó���Ѵ�
int smCheckWaitMessage();
int smCheckWaitMessage( smWINSOCK *lpsmSock );

//��ȣȭ��Ŷ �� �����Ѵ�
DWORD DecodePacket( BYTE *lpPacket , BYTE *lpDecBuff );
//��Ŷ�� ��ȣȭ �Ѵ�
int	EncodePacket( DWORD dwKey , DWORD PacketCnt , BYTE *lpPacket , BYTE *lpEncBuff );

//��Ŷ�� ��ȣȭ �Ѵ�
int	EncodePacket2( DWORD dwKey , DWORD PacketCnt , BYTE *lpPacket , BYTE *lpEncBuff , BYTE bEncXor );
//��ȣȭ��Ŷ �� �����Ѵ�
DWORD DecodePacket2( BYTE *lpPacket , BYTE *lpDecBuff , BYTE bDecXor );



//��Ŷ ���� ��ȣȭ ����
typedef DWORD (*LPFN_EncodePacket)( DWORD dwKey , DWORD PacketCnt , BYTE *lpPacket , BYTE *lpEncBuff );
typedef DWORD (*LPFN_DecodePacket)( BYTE *lpPacket , BYTE *lpDecBuff );

extern LPFN_EncodePacket	fnEncodePacket;
extern LPFN_DecodePacket	fnDecodePacket;

#define	POS_ENC_START	0x0
#define	POS_ENC_PACCODE 0xB
#define POS_ENC_XOR		0x74

#define	POS_DEC_START	0xA0
#define	POS_DEC_XOR		0x11A

#define	ENC_PACKET_ASM_SIZE	0x150

extern BYTE smwsock_EncodePacket_AsmCode[ENC_PACKET_ASM_SIZE];


#endif