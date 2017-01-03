#ifndef __ISAOCHECK_H__
#define __ISAOCHECK_H__
#ifndef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
#include "..\\clanmenu\\cE_CViewClanInfo.h"
#endif

#define SAVE_DIR	"savedata\\clanDATA"
#define SAVE_EXE	"cl"

// �̰��� �ɣӣ����� ���� �ڵ��̹Ƿ�
// �����ϰ� �� ����ϰ�

enum{
	ISAO_REJECT_OK = 0,			// ����������������
	ISAO_REJECT_BADPASS = 1,	// �н� ���� �ҷ���
	ISAO_REJECT_BADNAME = 2,	// ������ ��ϣ�
	ISAO_REJECT_LOCKOUT = 3,	// ���� �Ⱓ���� ���� �н� ���� ���� ������ ��ũ�ƿ�
	ISAO_REJECT_TEST_OVER = 4,	// �� ����� ���� �ʾұ� ������ ���ӽ� �н� ������ ��ȿ ������ ��������
	ISAO_REJECT_UNFAIR = 5,		// ���� ���� ������ ���źΣ�
	ISAO_REJECT_STOPPING = 6,	// �Ѷ� ����
	ISAO_REJECT_EXPIRE = 7,		// ���� ����
	ISAO_REJECT_NOT_BOUGHT = 8, // ��ǰ�� ����
	ISAO_REJECT_DB_MAINTE = 9,	// �ģ� �����ͳͽ� ��(��)
	ISAO_REJECT_OTHER = 10,		// ��Ÿ�� ����

	//���� ===============================================================================
	//ktj : 100~999�� ���̴� �������� php���� �����ڵ�� ����ϹǷ� ���⼭ ���� ������������.
	//���� ===============================================================================


	// �� �Ʒ��� ������ �� �ִ٣�
	ISAO_ERR_TOP = 1000,
	ISAO_ERR_WINSOCK_NOERR,		//ó�����ýû�� 
	ISAO_ERR_WINSOCK_START,		//���� START�� �����߻�					//window only
	ISAO_ERR_WINSOCK_VERSION,	//���ӹ����� Ʋ�� ������������.			//window only
	ISAO_ERR_TCP_CONNECT,		// ���� ���Ǵ� �ԣã��ڳ����信��
	ISAO_ERR_SSL_READ,			// �ӣӣ� ���� �� �� �����٣�
	ISAO_ERR_SSL_TIMEOUT,		// Ÿ�� �ƿ��ߴ٣�
	ISAO_ERR_NONE_REEJCT,		// ���� ��ȣ�� ���ڿ��� ���� ���� �ʴ�.
	ISAO_ERR_UNKNOWN_REJECT,	// ���� ��ȣ�� �ν��� �� ����.
	ISAO_ERR_RECONNECT,			// ������Ʈ�ϵ����Ѵ�.
	ISAO_ERR_END
};

int IsaoCheck( char *id, char *pass, int iCheckOn, int iTimeOut , char *ipaddr, long StartTime );
int IsaoCheck2( char *id, char *pass, int iCheckOn, int iTimeOut , char *ipaddr, long usertime);

int UserLogin(char* UserID, char* PassWord);
int InsertUser(char* UserID,char* PassWord,char* UserName);
int DeleteUser(char* UserID);
int LoginCheck(char* UserID,char* start_time);
int Login_List(char* UserID,char* end_time,char* use_time);
#endif


//�ؿ�
bool MultiByteToWideCharByCodePage(char *szMBCS,wchar_t *szUnicode,int iCodePage);
bool WideCharToMultiByteByCodePage(wchar_t *szUnicode,char *szMBCS,int iCodePage);
bool MBCSToUTF8(char *szMBCS,char *szUTF8,int iMBCSCodePage);
bool UTF8ToWeb(char *szUTF8,char *szWeb);
bool MBCSToWeb(char *szMBCS,char *szWeb,int iMBCSCodePage);





class ISAOCHECK
{
public:
	int iCheckFlag;
	int iTimeOut;
	int hostPORT;
	char hostName[64];
	//ygy
	int hostPORT1;
	char hostName1[64];
	//end ygy


	int HSsu;					//��罺Ʈ���� ����
	char *fileDATA_HS[30];		//�� 2048binary�� ����Ÿ�� 4096 hexa string���� �ٲ���.


    ISAOCHECK();
	~ISAOCHECK();

	void init();								//�� ó������ ����
	void init(int iCheckON, int iTimeOUT );		//��db���ø��� ����
	void init_HostName_Port(char *hostname, int port);
	void init_HostName_Port1(char *hostname, int port);
	void close();
	void main();
	
	
	//�ַ� ��Ʈ���� �޴� �Լ�
	int connect_WEBdb( char *szWriteBuffer, char *szReadBuffer);
	//ygy
	int connect_WEBdb_1( char *szWriteBuffer, char *szReadBuffer);
	//���ϵ��� ���̳����� �д� �Լ�.
	int connect_WEBdb_BINARY( char *szWriteBuffer, char *szReadBuffer, int *dataSize);

	int chk_readbuffer(char *cmd, char *readbuf, int *iRejectC );			//���� ����Ÿ���� �亯�� int����Ÿ�� �̾Ƴ���.
	int chk_readStrBuf(char *cmd, char *readbuf, char *retBuf);	//���� ����Ÿ���� �亯�� ��Ʈ������Ÿ�� �̾Ƴ���.

	int IsaoCheck( char *id, char *pass, char *ipaddr, long StartTime);
	int IsaoCheck_GameExit(char *id, char *pass, char *ipaddr, long usertime);



	//Ŭ�����ΰ��� ����(Ŭ�����̸� Ŭ���̸�������.)
	//int isClanMember(char *id, char *ret_Buf);
	int isClanMember(char *ClanZang_id, char* gserver,char* ClanZang_ChaName,char* ret_Buf);

	//ȣ��Clan�߰�
	int isCheckClanMember(char *pszServerName, char *pszUserName);

	//��� �߰�
	//int joinClanMember(char *clanName, char *memId, char *memCharId);
	//int joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID);
	int joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID,int level,DWORD chtype,int joinlevel,char *chipflag);
	
	

	//YGY:isCheckClanJang //���õ� ������ Ŭ�������� ��ϵǾ� �ִ��� üũ
	int isCheckClanJang(char *ClanZang_ID, char *gserver);
	//YGY: Ŭ���̸� �ߺ�üũ
	int isCheckClanName(char *ClanName,char *gserver);
	//makeClan
	int makeClan(char *ClanZang_ID, char* gserver,char* ClanZang_ChaName,char* ClanName,char *explanation,DWORD chtype,int level,char* data);
	//clanName���ۿ��� ���ϴ� Į�� ����Ÿ�� �о��.
	int read_ClanName(int readClumns);
	//int read_ClanData(char *clname, char *ret_Buf);		//clanName�� ����Ÿ�� ������.
	int read_ClanData(char *ClanZang_ID,char* gserver,char* ClanZang_ChaName,char* ret_Buf);		//clanName�� ����Ÿ�� ������.
	//int BreakUPClan(char *clname);			//Ŭ���� ������.
	int BreakUPClan(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName, char *ClanName);			//Ŭ���� ������.
	//int Banishiment(char *clname, char *banisID);	//Ư��id�� �߹���.
	int Banishiment(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName,char *ClanName, char *ClanWon);
	int WarhouseRight(char *clname, char *right_str);	//â����� �ٲ�.
	int updateFlag(char *clname);
	
	int Secession(char* clanzang,char* gserver,char* SecessionID,char* clanname);	//Ż��
	//ygy : �ʱ� â ���� �÷���
	int isPFlag(char *userid,char *clanname, char *chaname,char *gserver,char *pflag, char *gunbun);
	//ygy : Ŭ����ũ��ȣ�� Ŭ�� ���� �������
	int ReadClanInfo(char *clanNum,char* ret_Buf);


	//������ Ư�� ���丮���� Ŭ���� �׸��� �о���δ�.
	int read_ClanImage(char *clanname,char *filename);
	//�ٸ� Ŭ����ũ �̹��� �о� ����
	int read_ClanImage2(char *clanname,char *filename,CLANINFO *ci);	
	//Ư�����丮���� Ŭ���׸��� ���̺��� : ��� hexa ��Ʈ������ ������ �����.
	int write_ClanImageFile(char *filename);		

	//Ư�� ĳ���Ϳ� ���� ���
	//int connect_WEBdb_NOTICE1(char *severIP, int port, char *szWriteBuffer, char *szReadBuffer);
	int connect_WEBdb_NOTICE(char *szWriteBuffer, char *szReadBuffer);
	int ReadNotice(char *userid,char *chaname,char *gserver,char *buf);	
	//����
	int LeaveClan(char *clanname,char *chaname,char *gserver);

	//���Ӱ��� �÷�
	int isKPFlag(char *userid,char *clanname, char *chaname,char *gserver,char *pflag, char *gunbun);

	int AppointSubChip(char *gserver,char* clanwon);//��Ŭ���� �Ӹ�
	int ReleaseSubChip(char *gserver,char* clanwon); //�� Ŭ���� ����

	//�������� ���̺����� ��������
	int	GetWavFile(char* FileName);


	void Convert_bin2HS(char *sorce, int len);
	int WEBdb_Exam();
	






	int read_sod2(char *usid, char *charname,char *gserver, int index , char *szSod2Buff);

};




#undef EXTERN
#ifdef __ISAOCHECK_CPP__
	#define EXTERN
#else
	#define EXTERN extern
#endif

EXTERN ISAOCHECK iCheck;




