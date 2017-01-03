#ifndef __AUTH_H__
#define __AUTH_H__
#ifndef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
#include "..\\clanmenu\\cE_CViewClanInfo.h"
#endif


//DEFINE_YCY
#define CDKEYLEN 32
#define CONFIG   10

#define AUTH_ERROR_OTHER 1009
#define INET_ADDRSTRLEN 16
#define AUTH_ERROR_END  20
#define AUTH_OK 1
//END_YCY

// DB�� ť�� Ŀ�ǵ�
enum {
	/*
	AUTH_QUE_COMMAND_NONE,
	AUTH_QUE_COMMAND_AUTH,          // �����ϴ�
	AUTH_QUE_COMMAND_AUTH_KTJEND,   // ktj : ���γ��� ������ ��������
	AUTH_QUE_COMMAND_ALLNUM,
	*/

	AUTH_QUE_CMD_NONE,

	AUTH_QUE_CMD_isClanMember,		//Ŭ����������� �˾Ƴ�
	AUTH_QUE_CMD_CL_MAKE,			//Ŭ������(Ŭ��Ĩ�� Ŭ�����鶧 ��� : Ŭ����Ŭ��������Ÿ����)
	AUTH_QUE_CMD_CL_MEMBER_MAKE,    //Ŭ��������(Ŭ���� �����Ѱ�� ��� : Ŭ������Ÿ�߰�,Ŭ��������Ÿ����)
	AUTH_QUE_CMD_ReadClan,			//Ŭ������Ÿ�� �д´�.
	AUTH_QUE_CMD_BreakUP,			//Ŭ���� ��ü�Ѵ�.
	AUTH_QUE_CMD_Banishiment,		//Ŭ���� �߹��Ѵ�.
	AUTH_QUE_CMD_WarhouseR,			//���ѹٲ�.
	AUTH_QUE_CMD_updateFlag,		//Ŭ���Ἲ�ϷḦ ���� �÷��׼���
	AUTH_QUE_CMD_Secession,		//Ŭ���� ������ Ż���Ѵ�.

	AUTH_QUE_CMD_MarkREAD,		//Ŭ����ũ�� �д´�.
	//ygy
	AUTH_QUE_CMD_isCheckClanJang,		//Ŭ������ �ߺ����� üũ
	AUTH_QUE_CMD_isCheckClanName,		//Ŭ���̸��� �ߺ����� üũ
	AUTH_QUE_CMD_isPFlag,
	AUTH_QUE_CMD_ReadClanInfo,
	AUTH_QUE_CMD_MarkREAD2,		//Ŭ����ũ�� �д´�.
	AUTH_QUE_CMD_Updatcldata,
	AUTH_QUE_CMD_ReadNotice,
	AUTH_QUE_CMD_LeaveClan,
	AUTH_QUE_CMD_isKPFlag,
	AUTH_QUE_CMD_AppointSubChip,
	AUTH_QUE_CMD_ReleaseSubChip,
	AUTH_QUE_CMD_GetWavFile,

	AUTH_QUE_CMD_SOD2,
	AUTH_QUE_CMD_isCheckClanMember,	//ȣ��Clan�߰� (Ŭ�� Ż�� �� �簡�� �Ⱓ(4��)�� ����Ǿ����� �ƴ��� �˻�
	AUTH_QUE_CMD_ALLNUM,
};

// ť�� 1���� �������� ���磮
// ���� ������� ó���� �ϱ� ������ �����Ѵ٣�
enum {
	AUTH_QUE_STATUS_NONE,
	AUTH_QUE_STATUS_EXIST,          // �����Ͱ� �����Ѵ٣�
	AUTH_QUE_STATUS_BEGINAUTHPROC,        // (�ٸ� �����尡)�����͸� �а� ó���ϰ� �ִ٣�
	AUTH_QUE_STATUS_ENDOFAUTHPROC,  // AUTH ó�� ����
};

// AUTH �޽��� ť ���� ü
typedef struct _STRUCT_AUTH_QUE{
	
	int status;                  // �� �������� ����
	int command;                 // � �䱸�� ��
	int fdid;                    // �÷��̾��� fdid
	int result;                  // DB ó���� ���
	int badmsg;                  // �÷��̾�� �޽����� ���
	int index;					// �迭�� ��ȣ
	unsigned int query_time;              // ť�� �Ƿڸ� �´� �ð�
	char cdkey[CDKEYLEN];
	char isaokey[128];
	char password[128];



	//------------------  ktj : ���� ����.	start
	char ip_address[128];
	long howmanyTime;		//���ӽð�ýũ��:������ýũ�� ������� ���̺��صξ����.	
	
	char id[64];			//�ѱ� 8��,
	char charId[64];		//ĳ����id
	char expl[256];			//200����Ʈ������ȿ��
	char clanName[64];		//�ѱ� 8��,
	char ret_Buf[2048];		//web db���� ���ƿ� ��Ʈ��
	char gserver[64]; //���� ������
	char ClanWon_ID[64];   //���� ĳ���͸�
	char ClanWon_ChaName[64];
	char ClanZang_ID[64];  //Ŭ�� ¯���̵�
	char ClanZang_ChaName[64]; //Ŭ��¯ ĳ���� �̸�
	char Gubun[1]; //Ŭ��¯���� Ŭ�������� ����
    char Mark[64];
    int  nNum;
    CLANINFO* ci;
    int ClanInfo_Count;
	int level; //����

	int joinlevel;
	int joinjobcode;
	char chipflag[2]; //Ŭ��Ĩ���� ��Ŭ��Ĩ���� ����
	DWORD job_code;//ĳ���� Ÿ��
	char  szWavFileName[64];
	char  szSod2Buff[65536];


	//------------------  ktj : ���� ����.	end
    //====================ygy
    char PFlag[3]; //ùȭ�� ���� �÷���
 


	struct _STRUCT_AUTH_QUE *pNext;	// ������ ��ũ ������
	struct _STRUCT_AUTH_QUE *pPrev;	// ��(��)�� ��ũ ������
//	pthread_mutex_t mutex;       // �� ����ü�� mutex


	int sod2index;
}AUTH_QUE;


int AUTH_addQue( AUTH_QUE *que);
int AUTH_getResultLoop( void);



DWORD WINAPI AUTH_MainLoop(void* pVoid);
int end_GOLDmoney(char* cdkey, char* isaokey, char* passwd, char* ip_address);
int end_GOLDmoney2(char* cdkey, char* isaokey, char* passwd, char* ip_address);
int start_GOLDmoney(int fd, char *cdkey, char *accountid, char *passwd, char *ip_address);
int IsaoCheck2( char *id, char *pass, int iCheckOn, int iTimeOut , char *ipaddr, long usertime);
// ���գԣ� ť�� ��ũ ���̺�
enum{
	AUTHLINKTBL_FREE,
	AUTHLINKTBL_START_WAIT,
	AUTHLINKTBL_WORKING,
	AUTHLINKTBL_FINISH,
	AUTHLINKTBL_MAX
};
typedef struct {
	AUTH_QUE QueTop, QueTail;
}AUTHLINKTBL;

//AUTHLINKTBL aAuthLinkTbl[AUTHLINKTBL_MAX];


int end_GOLDmoney(int fd, char *cdkey, char *accountid, char *passwd, char *ip_address);

#endif





#define FLAG_DEAD  0
#define FLAG_EXEC  1
#define FLAG_WAIT  2
#define FLAG_EXIT  3

typedef struct
{
    DWORD thrHandle;
    int flag;
	int state;
    int cnt;
} THREADstruct;

class WebDB_Thread
{
public:
	

	THREADstruct ThreadS;

    WebDB_Thread();
	~WebDB_Thread();
	
	void init();							//�� ó������ ����
	void init(int iCheckON);				//��db���ø��� ����
	void init_HostName_Port(char *hostname, int port);
	void main();
	void end();

	//ktj : Ŭ�����ӿ� ȣ��Ʈ�� �������ִ��� ����
	void CheckHostNamePort(char *hostname, int port);

	int AUTH_init(int Num);
	int AUTH_close( void);

	void Suspend();    //����
	void Resume();      //�簳

	int AUTH_getResultLoop( void);




	//��db�� �����ϴ� ���ó�� �Լ� : ����ɸ��� ó���Լ��� ����Լ��� ������־����.

	//Ŭ����������� �˾Ƴ�.
	int AUTH_proc_isClanMember( int quenum, int threadindex);
	int AUTH_proc_isClanMember_Result( int quenum, int threadindex);

	//ȣ��Clan�߰�
	int AUTH_proc_isCheckClanMember(int quenum, int threadindex);
	int AUTH_proc_isCheckClanMember_Result(int quenum, int threadindex);
    int isCheckClanMember(char *pszServerName, char *pszUserName);


	//Ŭ���� ����� �Լ�
	int AUTH_proc_ClanMake( int quenum, int threadindex);
	int AUTH_proc_ClanMake_Result( int quenum, int threadindex);


	//Ŭ���⺸�� ����� �Լ�
	int AUTH_proc_Mmake( int quenum, int threadindex);
	int AUTH_proc_Mmake_Result( int quenum, int threadindex);
	
	//Ŭ���� �д� �Լ�
	int AUTH_proc_ReadClan( int quenum, int threadindex);
	int AUTH_proc_ReadClan_Result( int quenum, int threadindex);

	
	//Ŭ���� �����ϴ��Լ�.
	int AUTH_proc_BreakUP( int quenum, int threadindex);
	int AUTH_proc_BreakUP_Result( int quenum, int threadindex);

	//Ư�� id�߹�
	int AUTH_proc_Banishiment( int quenum, int threadindex);
	int AUTH_proc_Banishiment_Result( int quenum, int threadindex);

	//���ѹٲ�.
	int AUTH_proc_WarhouseR( int quenum, int threadindex);
	int AUTH_proc_WarhouseR_Result( int quenum, int threadindex);

	//Ŭ���Ἲ����.
	int AUTH_proc_updateFlag( int quenum, int threadindex);
	int AUTH_proc_updateFlag_Result( int quenum, int threadindex);

	
	//Ŭ��Ż��.
	int AUTH_proc_Secession( int quenum, int threadindex);
	int AUTH_proc_Secession_Result( int quenum, int threadindex);

	//Ŭ����ũ�б�

	int AUTH_proc_MarkREAD( int quenum, int threadindex);
	int AUTH_proc_MarkREAD_Result( int quenum, int threadindex);

	//�ٸ� Ŭ����ũ�б�

	int AUTH_proc_MarkREAD2( int quenum, int threadindex);
	int AUTH_proc_MarkREAD2_Result( int quenum, int threadindex);

	//isCheckClanJang ���õ� ������ Ŭ�������� ��ϵǾ� �ִ��� üũ
	int AUTH_proc_isCheckClanJang( int quenum, int threadindex);
	int AUTH_proc_isCheckClanJang_Result( int quenum, int threadindex);
	
	//ygy : Ŭ���̸��� �ߺ��Ǿ����� üũ
	int AUTH_proc_isCheckClanName( int quenum, int threadindex);
	int AUTH_proc_isCheckClanName_Result( int quenum, int threadindex);

	//ygy :�ʱ� ȭ�� ���� �÷���
	int AUTH_proc_isPFlag( int quenum, int threadindex);
	int AUTH_proc_isPFlag_Result( int quenum, int threadindex);

	//ygy :���Ӱ��� �÷���
	int AUTH_proc_isKPFlag( int quenum, int threadindex);
	int AUTH_proc_isKPFlag_Result( int quenum, int threadindex);

	//ygy : Ŭ����ũ��ȣ�� Ŭ�� ���� �������
	int AUTH_proc_ReadClanInfo( int quenum, int threadindex);
	int AUTH_proc_ReadClanInfo_Result( int quenum, int threadindex);

	//ygy : cldata ������Ʈ
	int AUTH_proc_Updatcldata( int quenum, int threadindex);
	int AUTH_proc_Updatcldata_Result( int quenum, int threadindex);

	//ygy : notice
	int AUTH_proc_ReadNotice( int quenum, int threadindex);
	int AUTH_proc_ReadNotice_Result( int quenum, int threadindex);

	//����
	int AUTH_proc_LeaveClan( int quenum, int threadindex);
	int AUTH_proc_LeaveClan_Result( int quenum, int threadindex);

	//��Ŭ���� �Ӹ�
	int AUTH_proc_AppointSubChip( int quenum, int threadindex);
	int AUTH_proc_AppointSubChip_Result( int quenum, int threadindex);

	//��Ŭ���� ����
	int AUTH_proc_ReleaseSubChip( int quenum, int threadindex);
	int AUTH_proc_ReleaseSubChip_Result( int quenum, int threadindex);

	//wav���� ��������
	int AUTH_proc_GetWavFile( int quenum, int threadindex);
	int AUTH_proc_GetWavFile_Result( int quenum, int threadindex);



	//WebDB�� �����ؼ� ������ �����ϴ� �����Լ���.(�����鿡���� �����Ǿ���)
	int isClanMember(char *ClanZang_id, char* gserver,char* ClanZang_ChaName);
	//Ŭ������� ���Խ�Ų��.
	//int joinClanMember(char *clanName, char *memid, char *memCharid);
	//int joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID);
	int joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID,int level,DWORD chtype,int joinlevel,char *chipflag);

	int make_Clan(char *ClanZang_ID, char* gserver,char* ClanZang_ChaName,char* ClanName,char *explanation,DWORD chtype,int level);
	int ReadClan(char *ClanZang_ID,char* gserver,char* ClanZang_ChaName);			//Ŭ������Ÿ�� �д´�.
	//int BreakUP_Clan(char *clanName);		//Ŭ���� ��ü�Ѵ�.
	int BreakUP_Clan(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName, char *ClanName);		//Ŭ���� ��ü�Ѵ�.
	int Banishiment(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName,char *ClanName, char *ClanWon);
	int Warhouse_right(char *clanName, char *right_str);
	int updateFlag(char *clanName);

	//Ŭ����ũ�б�
	int MarkREAD(char *clanname,char *mark);
	//�ٸ� Ŭ�� ��ũ �о����
	int MarkREAD2(char *clanname,char *mark);


	int Secession(char* clanzang,char* gserver,char* SecessionID,char* clname);
	//ygy :�ʱ� ȭ�� ���� �÷���
	int isPFlag(char *userid, char *clName, char *chName,char *gserver, char *PFlag, char *Gubun);

	//ygy :���� ���� �÷���
	int isKPFlag(char *userid, char *clName, char *chName,char *gserver, char *PFlag, char *Gubun);

	//ygy
	//isCheckClanJang ���õ� ������ Ŭ�������� ��ϵǾ� �ִ��� üũ
	int isCheckClanJang(char *ClanZang_ID, char *gserver);

	//ygy : Ŭ���̸��� �ߺ��� �Ǿ����� üũ
	int isCheckClanName(char *ClanName,char *gserver);
	//ygy : Ŭ����ũ��ȣ�� Ŭ�� ���� �������
	int ReadClanInfo(char *clanNum,CLANINFO *ci);

	//ygy : ������Ʈ cldata
	int Updatcldata(char *id, char *gserver, char *chaname);
	//Ư�� ĳ���Ϳ� ���� ���
	int ReadNotice(char *userid,char *chaname,char *gserver);

	//����
	int LeaveClan(char *clanname,char *chaname,char *gserver);

	//�������� ���̺����� ��������
	int	GetWavFile(char* FileName);

	int AppointSubChip(char* userid,char* chaname,char* clanname,char *gserver,char* clanwon);//��Ŭ���� �Ӹ�
	int ReleaseSubChip(char* userid,char* chaname,char* clanname,char *gserver,char* clanwon); //�� Ŭ���� ����



//sod ����
int sod2INFOindex(char *UserId, char* CharName,char* GServer, int Index);

int AUTH_proc_sodindex( int quenum, int threadindex);
int AUTH_proc_sodindex_Result( int quenum, int threadindex);







	int start_GOLDmoney(int fd, char *cdkey, char *accountid, char *passwd, char *ip_address);
	int end_GOLDmoney(int fd, char *cdkey, char *accountid, char *passwd, char *ip_address);
};



extern int sod2INFOindex(char *UserId, char* CharName,char* GServer , int Index);
extern BOOL bip_port_error;

#undef EXTERN
#ifdef __AUTH_CPP__
	#define EXTERN
#else
	#define EXTERN extern
#endif

EXTERN WebDB_Thread WebDB;






void web_DB_start();		//���� ���ۿ��� ����
void web_DB_main();			//web db�� ����ϴ� Ŭ�� �޴������� �Ҹ�����.
void web_DB_end();			//���� ������ ����
