/* 10
 * auth.cpp
 * ISAO ��� ������ �ϴ� ������   
 *
 */
#define __AUTH_CPP__

#include <windowsx.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "sinbaram\\sinLinkHeader.h"

#pragma comment(lib,"ws2_32.lib")



#include "../clanmenu/tjclanDEF.h"

#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��

	#include "sinbaram\\SinLinkHeader.h"
	#include "HoBaram\\HoLinkHeader.h"
	#include "Language\\language.h"
	#include "HoBaram\\HoTextFile.h"
	#include "..\\clanmenu\\cE_CViewClanInfo.h"


#endif
#include "..\\clanmenu\\tjclanDEF.h"
#include "..\\clanmenu\\tjclan.h"


#include "auth.h"
#include "isaocheck.h"

extern char szConnServerName[16];

void fd( char *fmt, ...);
void fd2( char *fmt, ...);


BOOL workfound;		//���Ұ� ������ 0��.


int AUTH_QUE_nums=0;			//ť ��������.
CRITICAL_SECTION g_cs;

AUTH_QUE *AUTH_que;
AUTHLINKTBL aAuthLinkTbl[AUTHLINKTBL_MAX];

DWORD WINAPI AUTH_MainLoop(void* pVoid);

static BOOL AUTH_proc( int mode, int quenum, int threadindex);


//ktj : Ŭ�� ������� �˾Ƴ���.  : AUTH_QUE_CMD_CL_MAKE
int AUTH_proc_isClanMember( int quenum, int threadindex);
int AUTH_proc_isClanMember_Result( int quenum, int threadindex);

//ktj : Ŭ�� ����� : AUTH_QUE_CMD_CL_MAKE
int AUTH_proc_ClanMake( int quenum, int threadindex);
int AUTH_proc_ClanMake_Result( int quenum, int threadindex);
//ktj : Ŭ�� ��� ����� : AUTH_QUE_CMD_CL_MEMBER_MAKE
int AUTH_proc_Mmake( int quenum, int threadindex);
int AUTH_proc_Mmake_Result( int quenum, int threadindex);

//ktj : Ŭ������Ÿ�� �д´�. : AUTH_QUE_CMD_CL_MEMBER_MAKE
int AUTH_proc_ReadClan( int quenum, int threadindex);
int AUTH_proc_ReadClan_Result( int quenum, int threadindex);


//ktj : Ŭ���� ��ü�Ѵ�.    AUTH_QUE_CMD_BreakUP
int AUTH_proc_BreakUP( int quenum, int threadindex);
int AUTH_proc_BreakUP_Result( int quenum, int threadindex);


//ktj : Ư���Ƶ� �߹�.    AUTH_QUE_CMD_BreakUP
int AUTH_proc_Banishiment( int quenum, int threadindex);
int AUTH_proc_Banishiment_Result( int quenum, int threadindex);

//ktj : â�����.    AUTH_QUE_CMD_WarhouseR
int AUTH_proc_WarhouseR( int quenum, int threadindex);
int AUTH_proc_WarhouseR_Result( int quenum, int threadindex);


//ktj :Ŭ���Ἲ �Ϸ�� AUTH_QUE_CMD_updateFlag
int AUTH_proc_updateFlag( int quenum, int threadindex);
int AUTH_proc_updateFlag_Result( int quenum, int threadindex);



//ktj :Ŭ��Ż�� AUTH_QUE_CMD_Secession
int AUTH_proc_Secession( int quenum, int threadindex);
int AUTH_proc_Secession_Result( int quenum, int threadindex);



//ktj :Ŭ����ũ�б� AUTH_QUE_CMD_MarkREAD
int AUTH_proc_MarkREAD( int quenum, int threadindex);
int AUTH_proc_MarkREAD_Result( int quenum, int threadindex);

//ygy : �ٸ� Ŭ����ũ�б�

int AUTH_proc_MarkREAD2( int quenum, int threadindex);
int AUTH_proc_MarkREAD2_Result( int quenum, int threadindex);

//ygy : ������ Ŭ�������� ��ϵǾ� �ִ��� üũ
int AUTH_proc_isCheckClanJang( int quenum, int threadindex);
int AUTH_proc_isCheckClanJang_Result( int quenum, int threadindex);

//ygy : Ŭ���̸��� �ߺ��Ǿ����� üũ
int AUTH_proc_isCheckClanName( int quenum, int threadindex);
int AUTH_proc_isCheckClanName_Result( int quenum, int threadindex);

//ygy : �ʱ� ȭ�� ���� �÷���
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

//Wav���� ��������
int AUTH_proc_GetWavFile( int quenum, int threadindex);
int AUTH_proc_GetWavFile_Result( int quenum, int threadindex);





//ȣ��Clan�߰�
int AUTH_proc_sodindex( int quenum, int threadindex);
int AUTH_proc_sodindex_Result( int quenum, int threadindex);

//ȣ��Clan�߰�
int AUTH_proc_isCheckClanMember(int quenum, int threadindex);
int AUTH_proc_isCheckClanMember_Result(int quenum, int threadindex);

// function jump table
typedef int (*FUNC)( int, int);
FUNC AUTH_proc_jmptbl[][AUTH_QUE_CMD_ALLNUM] = //3
{	
	{
		NULL,						// AUTH_QUE_COMMAND_NONE,
		AUTH_proc_isClanMember,
		AUTH_proc_ClanMake,			//Ŭ�� �����
		AUTH_proc_Mmake,			//Ŭ�� ��������
		AUTH_proc_ReadClan,			//Ŭ������Ÿ�� �д´�.
		AUTH_proc_BreakUP,			//Ŭ���� ��ü�Ѵ�.
		AUTH_proc_Banishiment,		//Ŭ�����߹�
		AUTH_proc_WarhouseR,
		AUTH_proc_updateFlag,		//Ŭ���� �����Ἲ�Ȱ����� ������Ʈ��.

		AUTH_proc_Secession,		//Ŭ��Ż��
		

		AUTH_proc_MarkREAD,
		AUTH_proc_isCheckClanJang,		//Ŭ��¯ ��ϵǾ� �ִ��� üũ
		AUTH_proc_isCheckClanName,      //Ŭ���̸� �ߺ����� üũ
		AUTH_proc_isPFlag,		     //�ʱ�ȭ�� �÷���
		AUTH_proc_ReadClanInfo,
		AUTH_proc_MarkREAD2,
		AUTH_proc_Updatcldata,
		AUTH_proc_ReadNotice,
		AUTH_proc_LeaveClan,
		AUTH_proc_isKPFlag, //���Ӱ��� �÷���
		AUTH_proc_AppointSubChip,//��Ŭ���� �Ӹ�
		AUTH_proc_ReleaseSubChip,//��Ŭ���� ����
		AUTH_proc_GetWavFile,//wav���� ��������
		AUTH_proc_sodindex,
		AUTH_proc_isCheckClanMember, //ȣ��Clan�߰�
		
	},
	{
		NULL,
		AUTH_proc_isClanMember_Result,
		AUTH_proc_ClanMake_Result,		//���� result
		AUTH_proc_Mmake_Result,		//��   result
		AUTH_proc_ReadClan_Result,			//Ŭ������Ÿ�� �д´�.
		AUTH_proc_BreakUP_Result,
		AUTH_proc_Banishiment_Result,
		AUTH_proc_WarhouseR_Result,
		AUTH_proc_updateFlag_Result,

		AUTH_proc_Secession_Result,	//Ŭ��Ż��
		
		AUTH_proc_MarkREAD_Result,
		AUTH_proc_isCheckClanJang_Result,
		AUTH_proc_isCheckClanName_Result,      //Ŭ���̸� �ߺ����� üũ
		AUTH_proc_isPFlag_Result,
		AUTH_proc_ReadClanInfo_Result,
		AUTH_proc_MarkREAD2_Result,
		AUTH_proc_Updatcldata_Result,
		AUTH_proc_ReadNotice_Result,
		AUTH_proc_LeaveClan_Result,
		AUTH_proc_isKPFlag_Result,
		AUTH_proc_AppointSubChip_Result, //��Ŭ���� �Ӹ�
		AUTH_proc_ReleaseSubChip_Result,//��Ŭ���� ����
		AUTH_proc_GetWavFile_Result,//Wav���� ��������
		AUTH_proc_sodindex_Result,
        AUTH_proc_isCheckClanMember_Result, //ȣ��Clan�߰�
	}
};

//ktj : AUTH_proc_jmptbl[mode][command]�� ���Ǹ�
//		���� 0:����, 1�� ������ ���ϰ��� ���� �Լ��� ������ ����.
//		��)���� ��		AUTH_proc_jmptbl[0][AUTH_QUE_COMMAND_AUTH]���
//			���������� 	AUTH_proc_jmptbl[1][AUTH_QUE_COMMAND_AUTH]���



//AUTH_QUE ���� ����Ÿ ó���Լ� ======================================
int AUTH_QUE_AppendTail( AUTH_QUE *pNow, int type );
AUTH_QUE*  AUTH_QUE_getTop( int type );
int AUTH_QUE_Remove( AUTH_QUE *pNow );
int AUTH_QUE_Count( int type );
int AUTH_addQue( AUTH_QUE *que);


// AUTH DB ť ��ũ ���̺��� �ǵڿ� �Ѱ� �߰���
int AUTH_QUE_AppendTail( AUTH_QUE *pNow, int type )
{	
	AUTH_QUE *pOldTail;
	// ���� ������ �� �� �����̰� �ڿ� �߰��Ѵ٣�
	pOldTail = aAuthLinkTbl[type].QueTail.pPrev;
	// �� �����̿��ٳ� �� ������ �ڽ�
	pOldTail->pNext = pNow;

	// ������ �� ������ �ڽſ��� �Ѵ٣�
	aAuthLinkTbl[type].QueTail.pPrev = pNow;

	// �ڽ��� �� ������ �� �����̿��ٳ� �����ͣ�
	pNow->pPrev = pOldTail;
	// �ڽ��� ������ ���� ����
	pNow->pNext = &aAuthLinkTbl[type].QueTail;

	return TRUE;
}


// DB ť ��ũ ���̺��� ���θ� ���� �ش٣�
AUTH_QUE*  AUTH_QUE_getTop( int type )
{
	// MUTEX LOCK

	// �鿡 ����ǰ�� ���� ������ �����Ѵ٣�
	// �װ��� ���� �����̿��٣����� �뱸 �ƹ��͵� ���٣�
	if( aAuthLinkTbl[type].QueTop.pNext == &aAuthLinkTbl[type].QueTail ){
		return NULL;
	}

	// �̰��̴٣�
	return aAuthLinkTbl[type].QueTop.pNext;
}

// ��ũ������ �� ������ ���� ��ң�
int AUTH_QUE_Remove( AUTH_QUE *pNow )
{
	AUTH_QUE *pOldNext, *pOldPrev;

	// �ڽ��� ���ĸ� ����Ѵ٣�
	pOldNext = pNow->pNext;
	pOldPrev = pNow->pPrev;
	// ���ĸ� ���� ���ش٣�
	pNow->pNext = NULL;
	pNow->pPrev = NULL;
	// �������� ��´٣�
	pOldNext->pPrev = pOldPrev;
	pOldPrev->pNext = pOldNext;

	return TRUE;
}

// ��ũ�� ����.
int AUTH_QUE_Count( int type )
{
	int iCount = 0;
	AUTH_QUE *pNow;
	// ������ ��ġ
	pNow = aAuthLinkTbl[type].QueTop.pNext;
	// ���� ������ ���٣�
	for( ; pNow->pNext; pNow=pNow->pNext ){
		iCount ++;
	}

	return iCount;
}


//-------------------------------------------------------------------------
// ť�� ó���� �߰��Ѵ�.
// �μ�
// AUTH_QUE *que   ����ϴ� ť
// ���ƿ��� ��:
// ���� :0 �̻󣮼�Ʈ�� que�� ��ȣ
// ���� :-1
//-------------------------------------------------------------------------

int AUTH_addQue( AUTH_QUE *que)
{    
	AUTH_QUE *pNew;
	EnterCriticalSection(&g_cs);			//ktj : window
	//pthread_mutex_lock( &AUTH_que_mutex);	//ktj : linux

	// freeList�� ����
	pNew = AUTH_QUE_getTop( AUTHLINKTBL_FREE );  //0
	if( pNew == NULL ){
		//eprint( "err:cannot add auth que\n");
		LeaveCriticalSection(&g_cs);				//ktj : window
		//pthread_mutex_unlock( &AUTH_que_mutex);	//ktj : linux
		return -1;
	}

	// ������ ī��
	pNew->command = que->command;
	pNew->fdid = que->fdid;
	pNew->result = AUTH_ERROR_OTHER;
	//ktj : �ϴ� ����
	//pNew->query_time = NowTime.tv_sec;
	pNew->query_time = 10;
	strcpy( pNew->cdkey, que->cdkey);
	strcpy( pNew->isaokey, que->isaokey);
	strcpy( pNew->password, que->password);

	//ktj : ���� ����. --s
	strcpy( pNew->ip_address, que->ip_address);
	strcpy( pNew->id, que->id);
	strcpy( pNew->expl, que->expl);
	strcpy( pNew->clanName, que->clanName);
	strcpy( pNew->charId, que->charId);
	//ygy
	strcpy( pNew->ClanZang_ID,que->ClanZang_ID);
	strcpy( pNew->ClanZang_ChaName, que->ClanZang_ChaName);
	strcpy(pNew->gserver, que->gserver);
	strcpy(pNew->ClanWon_ID,que->ClanWon_ID);
	strcpy(pNew->ClanWon_ChaName,que->ClanWon_ChaName);
	strcpy(pNew->Mark,que->Mark);
	strcpy(pNew->Gubun,que->Gubun);
	strcpy(pNew->PFlag,que->PFlag);
	
	pNew->howmanyTime = que->howmanyTime;
	pNew->nNum = que->nNum;
	pNew->ci = que->ci;
	pNew->job_code = que->job_code;
	pNew->level = que->level;
	pNew->joinlevel = que->joinlevel;
	pNew->joinjobcode = que->joinjobcode;
	strcpy(pNew->chipflag , que->chipflag);
	strcpy(pNew->szWavFileName,que->szWavFileName);


	/*char gserver[64]; //���� ������
	char ClanWon[64];   //���� ĳ���͸�
	char ClanZang[64];  //Ŭ�� ¯
*/

	//ktj : ���� ����. --e

	pNew->status = AUTH_QUE_STATUS_EXIST;  //1
	pNew->sod2index = que->sod2index;



	// ��ũ������ �� ������ ���� ��ң�
	AUTH_QUE_Remove( pNew );
	// ���� ��ٸ��� ��(��)�� �ڿ� �߰��Ѵ٣�
	AUTH_QUE_AppendTail( pNew, AUTHLINKTBL_START_WAIT );

	LeaveCriticalSection(&g_cs);				//ktj : window

	// �迭�� ��ȣ
	return pNew->index;
}















/**************************************************************************
 *
 * DWORD WINAPI AUTH_MainLoop(void* pVoid)
 * AUTH ������ ���η���(�����忡 ���� �Ҹ�����)
 *
 *
 **************************************************************************/
DWORD WINAPI AUTH_MainLoop(void* pVoid)
{
	THREADstruct *thr = (THREADstruct *)pVoid;

	int cnt = 0;
	//BOOL found;
	static struct timeval prev = {0,0};	
	unsigned long thread_index = -1;
	int ret = 0;
	
	//fd2("��������η���while ���� ���� ");


	while(1){
		Sleep(10);

		long myTime;
		time( (time_t*)&myTime );
		//fd2("��������η���cnt %d  myTime : %d ", cnt++, myTime);

		switch(thr->flag) {
        case FLAG_WAIT:
			time( ( time_t* )&myTime );
			//fd2("��������η��� FLAG_WAIT ����. %d  ", myTime );


			thr->state=FLAG_WAIT;
			//EnterCriticalSection(&g_cs);
            SuspendThread( (void *)thr->thrHandle );    //WAIT���� ������ �ڵ�������Ŵ.
														//�簳�� ������ �ۿ��� ����.
			//LeaveCriticalSection(&g_cs);

			time( ( time_t* )&myTime );
			fd2("��������η��� FLAG_WAIT ��. %d ", myTime);
            break;

        case FLAG_EXIT:
            goto _EXIT;

		case FLAG_EXEC:
			thr->state=FLAG_EXEC;
			//fd2("��������η��� FLAG_EXEC ��. ");

			/*
			EnterCriticalSection(&g_cs);
			
			LeaveCriticalSection(&g_cs);
			*/



			workfound = FALSE;
			EnterCriticalSection(&g_cs);			//ktj : windows //ktj:432�ٿ��� ��ü�Կ�����

			// ������� ��Ʈ��ó�� �Ǿ��� ������(AUTHLINKTBL_START_WAIT : 1)
			AUTH_QUE *que;
			que = AUTH_QUE_getTop(AUTHLINKTBL_START_WAIT );
			
			if( que != NULL ){			// ������� ����Ÿ�� �־� index�� ������.
				workfound = TRUE;			
				cnt = que->index;		
			}else{
				workfound = FALSE;			
			}
  
			// �߰ߵ� ����Ÿ�� ������ �ణ �޽��� continue�Ѵ�.
			if( !workfound ) {
				LeaveCriticalSection(&g_cs);				//window only
				continue;
			}

			// ������ ��
			// ť ���� ���森�ٸ� �����忡 ����ϴ٣�
			AUTH_que[cnt].status = AUTH_QUE_STATUS_BEGINAUTHPROC; //2
			//�۾��� �߰��Ѵ٣�
			AUTH_QUE_Remove( &AUTH_que[cnt] );
			AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_WORKING );  //2

			LeaveCriticalSection(&g_cs);				//window only

			// AUTH ó��
			//ktj : FAUTH_proc_jmptbl[][AUTH_QUE_COMMAND_ALLNUM] �� ���õ� �Լ��� ��������ش�
			//      ������Ʈ���� asp,php������ �����ϴ� �Լ��� �����.
			if( !AUTH_proc( 0, cnt, thread_index)) {		
				fd2( "auth_proc error [que:%d command:%d]\n", cnt, AUTH_que[cnt].command);
				break;
			}

			// ť ���� ����
			EnterCriticalSection(&g_cs);			//window only
		
			// ���� ������ ť�� ������.
			AUTH_QUE_Remove( &AUTH_que[cnt] );
			AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_FINISH ); //3

			AUTH_que[cnt].status = AUTH_QUE_STATUS_ENDOFAUTHPROC; //3
			LeaveCriticalSection(&g_cs);


			break;
        }
	}
_EXIT:
    thr->flag = FLAG_DEAD;

    ExitThread( TRUE );
    return TRUE;

	

	/*

	//���� ������ ���� ����
	//ktj : ����κ��� �ܺ��Լ����� �����ϵ��� �Ǿ�����
    while(1){
		
		found = FALSE;
		EnterCriticalSection(&g_cs);			//ktj : windows //ktj:432�ٿ��� ��ü�Կ�����

		// ������� ��Ʈ��ó�� �Ǿ��� ������(AUTHLINKTBL_START_WAIT : 1)
		AUTH_QUE *que;
	    que = AUTH_QUE_getTop(AUTHLINKTBL_START_WAIT );
			
		if( que != NULL ){			// ������� ����Ÿ�� �־� index�� ������.
			found = TRUE;			
			cnt = que->index;		
		}else{
			found = FALSE;			
		}
  
		// �߰ߵ� ����Ÿ�� ������ �ణ �޽��� continue�Ѵ�.
		if( !found) {
			LeaveCriticalSection(&g_cs);				//window only
			continue;
		}

		// ������ ��
		// ť ���� ���森�ٸ� �����忡 ����ϴ٣�
		AUTH_que[cnt].status = AUTH_QUE_STATUS_BEGINAUTHPROC; //2
		//�۾��� �߰��Ѵ٣�
		AUTH_QUE_Remove( &AUTH_que[cnt] );
		AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_WORKING );  //2

        LeaveCriticalSection(&g_cs);				//window only

		// AUTH ó��
		//ktj : FAUTH_proc_jmptbl[][AUTH_QUE_COMMAND_ALLNUM] �� ���õ� �Լ��� ��������ش�
		//      ������Ʈ���� asp,php������ �����ϴ� �Լ��� �����.
		if( !AUTH_proc( 0, cnt, thread_index)) {		
			fd2( "auth_proc error [que:%d command:%d]\n", cnt, AUTH_que[cnt].command);
			break;
		}

		
		// ť ���� ����
		EnterCriticalSection(&g_cs);			//window only
		
		// ���� ������ ť�� ������.
		AUTH_QUE_Remove( &AUTH_que[cnt] );
		AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_FINISH ); //3

		AUTH_que[cnt].status = AUTH_QUE_STATUS_ENDOFAUTHPROC; //3
		LeaveCriticalSection(&g_cs);

	}
	
	fd2("��������η��� ExitThread  ");
	ExitThread( TRUE );

	return 0;
	*/
}
	

//-------------------------------------------------------------------------
// AUTH �Լ� ���� ��Ŵ
//AUTH_MainLoop(void* pVoid),AUTH_getResultLoop( void) �� �ΰ� �Լ����� �Ҹ�����
//�Լ������͸� �ҷ� ���������.
//-------------------------------------------------------------------------
BOOL AUTH_proc( int mode, int quenum, int threadindex)
{	

	if(mode==0)
		mode = 0;

	if(mode==1)
		mode = 1;

    FUNC	func;
	BOOL	ret = FALSE;

	// ���� üũ
	if( quenum < 0 || quenum > AUTH_QUE_nums ) {
		//eprint( "err: quenum \n");
		fd2( "err: quenum \n");
		return FALSE;
	} 

	if( AUTH_que[quenum].command <= AUTH_QUE_CMD_NONE || AUTH_que[quenum].command >= AUTH_QUE_CMD_ALLNUM )
	{
		fd2( "err: command \n");
		return FALSE;
	}
	
	func = AUTH_proc_jmptbl[mode][AUTH_que[quenum].command];
	if( func) {
		//isaocheck���� ���� �����ϴ� �Լ����� �Լ������Ϳ� ���� ����Ǿ���.
		//===============================================================
		ret = func( quenum, threadindex);	
		//===============================================================
	}
	else {
		fd2( "err: no such function\n");
	}
	if( !ret)
		fd2("err func:\n");
	return ret;
	
}










































WebDB_Thread::WebDB_Thread()
{
	
}
WebDB_Thread::~WebDB_Thread()
{

	//######################################################################################
	//�� �� �� : �� �� ��
	AUTH_close();
	//######################################################################################
}
void WebDB_Thread::init()
{
}

void WebDB_Thread::main()
{
	AUTH_getResultLoop();
}


void WebDB_Thread::end()
{
	
}





/*///////////////////////////////////////////////////////////////////////////
: Ŭ�� ��: AUTH_init
: �� �� �� : ������� ����Ÿ ����
: �� ȯ �� :
///////////////////////////////////////////////////////////////////////////*/
int WebDB_Thread::AUTH_init(int Num)
{
	fd2( "AUTH_init()�Լ� start" );
	
	//ktj ��ü
	AUTH_QUE_nums = Num;//config.authquenum;

	int i;

	// �޽��� ť�� Ȯ��
	//AUTH_que = allocateMemory( sizeof( AUTH_QUE) * config.authquenum);	//����
	AUTH_que = new AUTH_QUE[AUTH_QUE_nums ];								//ktj : setup.cf���� �о�Ȯ����.
	
	if( AUTH_que == NULL ) {
		//eprint( "can not allocate memory %d\n", sizeof( AUTH_QUE)*config.authquenum);
		fd2("can not allocatte memory %d\n","new memory");
		return FALSE;
	}


	// �޽��� ť�� �ʱ�ȭ
	//for( i = 0; i < (int)config.authquenum; i ++ ) {
	for( i = 0; i < (int)AUTH_QUE_nums; i ++ ) {
		AUTH_que[i].status = AUTH_QUE_STATUS_NONE;
		AUTH_que[i].command = AUTH_QUE_CMD_NONE;
		AUTH_que[i].index = i;							//���۹�ȣ
		AUTH_que[i].fdid = -1;
		AUTH_que[i].cdkey[0] = '\0';
		AUTH_que[i].isaokey[0] = '\0';
		AUTH_que[i].password[0] = '\0';
		
		AUTH_que[i].result = AUTH_ERROR_OTHER;
	}


	//ktj: ũ��Ƽ�� ������ �ʱ�ȭ
	InitializeCriticalSection(&g_cs);
//	pthread_mutex_init( &AUTH_que_mutex, NULL);
//	pthread_mutex_init( &AUTH_que_wait_mutex, NULL);
    
	// AUTH ť ��ũ ���̺��� �ʱ�ȭ	(AUTHLINKTBL_MAX==���� 4��)
	for( i = 0; i < AUTHLINKTBL_MAX; i ++ ){
		aAuthLinkTbl[i].QueTop.pNext = &aAuthLinkTbl[i].QueTail;	// ������ ������ ����
		aAuthLinkTbl[i].QueTop.pPrev = NULL;						// ������ ��(��)�� �̷��

		aAuthLinkTbl[i].QueTail.pPrev = &aAuthLinkTbl[i].QueTop;	// ������ ��(��)�� ����
		aAuthLinkTbl[i].QueTail.pNext = NULL;						// ������ ������ �̷��
	}

	// ���θ� �ϴ� ��ũ ����Ʈ�� �����Ѵ٣�
	for( i = 0; i < (int)AUTH_QUE_nums; i ++ ) {
		AUTH_QUE_AppendTail( &AUTH_que[i], AUTHLINKTBL_FREE );
	}
	
	
	//ktj : ������� ������� ���� ����
	//AUTH �����带 ����.(AUTH_MainLoop()�Լ��� ��� ���ư�)
	DWORD dwSendMsgId;
	ThreadS.thrHandle  = 0;
    ThreadS.flag =0;
	
    ThreadS.cnt = 0;
	DWORD thrHandle = (unsigned long)CreateThread(NULL,0,AUTH_MainLoop, (void*)&ThreadS,
													0,&dwSendMsgId);
	ThreadS.thrHandle = thrHandle;

	if(ThreadS.thrHandle ==0) 
		return 0;


	fd2( "AUTH_init()�Լ� end" );

	return TRUE;
}
/*///////////////////////////////////////////////////////////////////////////
: �Լ�  �� : AUTH_close()
: �ϴ�  �� : ���� ó��
: ��    �� :
: ��ȯ  �� :
///////////////////////////////////////////////////////////////////////////*/
int WebDB_Thread::AUTH_close( void)
{
	//AUTH �����带 ������.(TerminateThread():�ܺο��� �����带 �������Լ�)
    TerminateThread( (void*)ThreadS.thrHandle , 0 );

	delete AUTH_que;
	AUTH_que = NULL;

	DeleteCriticalSection(&g_cs);

	fd2( "AUTH_close()�Լ� " );
	return TRUE;
}


/*///////////////////////////////////////////////////////////////////////////
: �Լ�  �� :
: �ϴ�  �� : �������� ��������, �簳�Լ�
: ��    �� :
: ��ȯ  �� :
///////////////////////////////////////////////////////////////////////////*/
void WebDB_Thread::Suspend()     //���� : ktj : ó���� ����Ÿ�� ���°��� Ȯ������ �����Ұ�
{
	//�հ� ������ �׳� ����.
	//if(	workfound != FALSE ||   AUTH_QUE_getTop( AUTHLINKTBL_FINISH ) != NULL)
	//	return;

	fd2("�����Լ� Suspend() 00 ");
	ThreadS.flag = FLAG_WAIT;

	fd2("�����Լ� Suspend() 11 ");
}

void WebDB_Thread::Resume()      //�簳
{
	fd2("�簳�Լ� Resume() �� ");

	if(ThreadS.flag == FLAG_EXEC) return; //flag�� ��������̸� �׳� ����.

		
	fd2("�簳�Լ� Resume() while ���� ");
    while(1) {
		fd2("�簳�Լ� Resume() while �� ");
		
		if(ThreadS.flag == FLAG_WAIT) {

			fd2("�簳�Լ� Resume() while �� break; ");


			break;  //flag�� �������϶��� ������.
		}
    }

	ThreadS.flag = FLAG_EXEC;				//Ȱ���簳(����:�÷��׸� ResumeThread()�տ����� ���� �ʹ������� �����尡 ��ų��������.)
    ResumeThread( (void *)ThreadS.thrHandle  );

	fd2("�簳�Լ� Resume()  ��");
}




/**************************************************************************
 * ���⿡���� ���� �����尡 ó���ϴ� �κ�.
 * AUTH_MainLoop�� ���� ó���� �� ����� ó���ϴ� �Լ�
 * ���� �޸𸮸� ������ ���� ���� ������
 *  severNT.cpp�� ���ο��� ��ӺҸ����� ó���ʿ� ����
 **************************************************************************/
int WebDB_Thread::AUTH_getResultLoop( void)
{
	int i;
	static	int cnt = 0;

	AUTH_QUE *que;
	cnt = -1;

	EnterCriticalSection(&g_cs);				//window only
	
	for( i = 0; i < AUTH_QUE_nums; i ++ )
	{
		que = AUTH_QUE_getTop( AUTHLINKTBL_FINISH ); //ó���� ���� ť�� �����´�.
		if( que != NULL ){
			cnt = que->index;	// �迭 ��ȣ

			AUTH_QUE_Remove( que );
			// Result ó���Ѵ٣�
			if( !AUTH_proc( 1,cnt, -1)) {
				//eprint( "auth_proc error [%d]\n", cnt);
			}
			AUTH_que[cnt].status = AUTH_QUE_STATUS_NONE;
			// �ϴ� ��ũ��
			AUTH_QUE_AppendTail( &AUTH_que[cnt], AUTHLINKTBL_FREE );
		}else{
			break;
		}
	}
	LeaveCriticalSection(&g_cs);					//window only
	return  TRUE;
}











/*/////////////////////////////////////////////////////////////////////////// START
: �� �� �� :  ��db�� �����ϴ� ���ó�� �Լ� 
			  �Լ������Ͷ����� C���Լ��� �����ؼ� �Ҹ������� ����
///////////////////////////////////////////////////////////////////////////*/


/*///////////////////////////////////////
: �� �� �� :  
: �� �� �� :  ��� �о����
: ��    �� :
: ��    �� :
/////////////////////////////////////////*/

int WebDB_Thread::ReadNotice(char *userid,char *chaname,char *gserver)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp, sizeof(AuthQueTmp));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_ReadNotice;          // Ŭ������� ����

	//ygy	
	strncpy( AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );	


	//ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}
int AUTH_proc_ReadNotice( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadNotice( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ReadNotice( int quenum, int threadindex)
{
	char *chaname,*gserver,*userid;
	int knret = 1, iBadMsg = 0;	

	userid = AUTH_que[quenum].ClanZang_ID;
	chaname = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;


	fd2("AUTH_proc_ReadClanInfo %s ", AUTH_que[quenum].Mark);

	//int nMarkNum;
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.ReadNotice(userid,chaname,gserver,AUTH_que[quenum].ret_Buf);
	
	
	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
int AUTH_proc_ReadNotice_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadNotice_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_ReadNotice_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�

	//iResultCode = 1;
	//strcpy(AUTH_que[quenum].ret_Buf,"�ȳ��ϼ���. ����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�. �ٽ� �����Ͽ��ֽʽÿ������ Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.����� Ŭ���� �����ڷν� �̹� ������ ������ �� �����ϴ�.");
	//AUTH_que[quenum].ret_Buf[0] = 0;

	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);		
		break;	
	case 100:				//�Ϲ� ���,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;
	default:
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}


/*///////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ŭ�� ��ũ ��ȣ�� Ŭ�� ���� �������
: ��    �� :
: ��    �� :
/////////////////////////////////////////*/
//ygy : Ŭ����ũ��ȣ�� Ŭ�� ���� �������
int AUTH_proc_ReadClanInfo( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadClanInfo( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_ReadClanInfo( int quenum, int threadindex)
{
	char *szMarkNum;
	CLANINFO* claninfo;
	//int SaveNum;
	int knret = 1, iBadMsg = 0;
	char* clnaName;

	//id = AUTH_que[quenum].id;
	szMarkNum = AUTH_que[quenum].Mark;
	claninfo = AUTH_que[quenum].ci;
	//SaveNum = AUTH_que[quenum].nNum;



	fd2("AUTH_proc_ReadClanInfo %s ", AUTH_que[quenum].Mark);

	//int nMarkNum;
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.ReadClanInfo(szMarkNum,AUTH_que[quenum].ret_Buf);
	if(knret == 1){
		char* clanINFO_result(char *marknum,char *szInfo,CLANINFO* ci);
		clnaName = clanINFO_result(AUTH_que[quenum].Mark,AUTH_que[quenum].ret_Buf,claninfo);
		knret = iCheck.read_ClanImage2(clnaName,AUTH_que[quenum].Mark,claninfo);
		if(knret ==1)
		{
			claninfo->nStatus = 2;
		}
		else
		{
			claninfo->nStatus = 3;
		}
		//??????????????????????????????????????????????????????????????????????????
		//nCheckDownLoad = 2;
		//??????????????????????????????????????????????????????????????????????????
	}
	else		//�ٿ�ε� ������.
	{
		claninfo->nStatus = 3;
	}
	
	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
int AUTH_proc_ReadClanInfo_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadClanInfo_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ReadClanInfo_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	int clanINFO_result2(int ret_code, char *marknum,CLANINFO *ci);
	//clanINFO_result2(ANSdata.ret_val, AUTH_que[quenum].Mark);


	clanINFO_result2(iResultCode, AUTH_que[quenum].Mark,AUTH_que[quenum].ci);


	//ktj : �ӽ�
	//fd2("AUTH_proc_ReadClanInfo_Result :  ret_val %d  AUTH_que[quenum].Mark : %s ",   ANSdata.ret_val, AUTH_que[quenum].Mark);
	
	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}


int WebDB_Thread::ReadClanInfo(char *clanNum,CLANINFO *ci)
{
	//ktj : �ӽ�
	static cnt=0;
	fd2("ReadClanInfo:  Ŭ����ũ��ȣ %s  (cnt %d )",   clanNum,  cnt);
	cnt++;


	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_ReadClanInfo;          // �����ϴ�
	
	
	strncpy( AuthQueTmp.Mark, clanNum, sizeof( AuthQueTmp.Mark) );
	//AuthQueTmp.nNum =  num;
	AuthQueTmp.ci = ci;

		

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}

//ȣ��Clan�߰�
//�ش� ���̵� Ŭ�� Ż�� �� �簡�� �Ⱓ(4��)�� ������� �ʾ��� ���� Ŭ�� ���Խ��� �޼���
//1�̸� 4���� ������ ����̰� 0�̸� 4���� ���� ���
int WebDB_Thread::isCheckClanMember(char *pszServerName, char *pszUserName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isCheckClanMember;          

	//strncpy( AuthQueTmp.ClanZang_ID,      pszUserID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, pszUserName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver,          pszServerName, sizeof( AuthQueTmp.gserver) );

	//ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;

}

int AUTH_proc_isCheckClanMember_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanMember_Result(quenum, threadindex);	
}

int AUTH_proc_isCheckClanMember(int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanMember(quenum, threadindex);
}



int WebDB_Thread::AUTH_proc_isCheckClanMember(int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	ClanZang_ID      = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver          = AUTH_que[quenum].gserver;
	
 	knret = iCheck.isCheckClanMember(gserver,ClanZang_ChaName);
	 //knret = iCheck.isCheckClanMember(ClanZang_ID,gserver,ClanZang_ChaName);
	
	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	
	return TRUE;
}

int WebDB_Thread::AUTH_proc_isCheckClanMember_Result(int quenum, int threadindex)
{
	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����


	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.		
		break;	
	
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}
	return TRUE;
}


/*///////////////////////////////////////
: �� �� �� :  

: �� �� �� :  Ŭ����������� �˾Ƴ����Լ�
: ��    �� :
: ��    �� :
/////////////////////////////////////////*/
int AUTH_proc_isClanMember( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isClanMember( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isClanMember( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.isClanMember(ClanZang_ID,gserver,ClanZang_ChaName, AUTH_que[quenum].ret_Buf);


	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
   

int AUTH_proc_isClanMember_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isClanMember_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isClanMember_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 5:
	case 4:
	case 3:
	case 2:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 100:				//�Ϲ� ���,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;
	default:
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}


/*///////////////////////////////////////
: �� �� �� :  
: �� �� �� :  �ʱ� ȭ�� ���� �÷���
: ��    �� :
: ��    �� :
/////////////////////////////////////////*/

/*///////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ŭ����������� �˾Ƴ����Լ�
: ��    �� :
: ��    �� :
/////////////////////////////////////////*/
int AUTH_proc_isPFlag( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isPFlag( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isPFlag( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*PFlag,*Gubun,*ClanName;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	PFlag = AUTH_que[quenum].PFlag;
	Gubun = AUTH_que[quenum].Gubun;
	ClanName = AUTH_que[quenum].clanName;
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.isPFlag(ClanZang_ID,ClanName,ClanZang_ChaName,gserver, PFlag,Gubun);


	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_isPFlag_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isPFlag_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isPFlag_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.		
		break;	
	case 100:				//�Ϲ� ���,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

int WebDB_Thread::isPFlag(char *userid, char *clName, char *chName,char *gserver, char *PFlag, char *Gubun)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isPFlag;          // �����ϴ�
	
	strncpy( AuthQueTmp.Gubun, Gubun, sizeof( AuthQueTmp.Gubun ) );
	strncpy(AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ));
	strncpy( AuthQueTmp.clanName, clName, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.ClanZang_ChaName, chName, sizeof( AuthQueTmp.ClanZang_ChaName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.PFlag, PFlag, sizeof( AuthQueTmp.PFlag ) );
	

	

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}

/*///////////////////////////////////////
: �� �� �� :  
: �� �� �� :  ���õ� ������ Ŭ�������� ��ϵǾ� �ִ��� üũ
: ��    �� :
: ��    �� :
/////////////////////////////////////////*/
int WebDB_Thread::isCheckClanJang(char *ClanZang_ID, char *gserver)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isCheckClanJang;          // �����ϴ�
		
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}
int AUTH_proc_isCheckClanJang( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanJang( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isCheckClanJang( int quenum, int threadindex)
{
	char* ClanZang_ID,*gserver;
	int knret = 1, iBadMsg = 0;

	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;	
	gserver= AUTH_que[quenum].gserver;
	
	
	knret = iCheck.isCheckClanJang(ClanZang_ID,gserver);


	//ktj : �����̹Ƿ� ���߿� ����ÿ�.
	//Į���� ���� ����Ÿ�б�
	//knret = iCheck.read_ClanName(0);
	//�̹����б� : OK
	//knret = iCheck.write_ClanImageFile("a.bmp");
	///knret = iCheck.read_ClanImage("a.bmp");


	
	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_isCheckClanJang_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanJang_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isCheckClanJang_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	
	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}
	
	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
		case 0://Ʈ����� ����
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 1://���� ����
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		/*case 2://ĳ���Ͱ� �̹� �ٸ� Ŭ���� �Ҽ�
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 3://Ŭ���̸� �ߺ�
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;*/
		case 100:				//���ڰ� ����
		iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 1004:
		iResultCode = ANSdata.ret_val;
			break;
	case 101:				//Ŭ������� �ȵ�.
	case 102:				//Ŭ�������� ����� �ȵ�.
	case 103:				//Ŭ�������� ����� �ȵ�.
		break;
//ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val );

	


	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}





/*///////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ŭ���̸��� �ߺ����� üũ
: ��    �� :
: ��    �� :
/////////////////////////////////////////*/
int WebDB_Thread::isCheckClanName(char *ClanName,char *gserver)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isCheckClanName;          // �����ϴ�
		
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}
int AUTH_proc_isCheckClanName( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanName( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isCheckClanName( int quenum, int threadindex)
{
	char* ClanName,*gserver;
	int knret = 1, iBadMsg = 0;

	ClanName = AUTH_que[quenum].clanName;	
	gserver= AUTH_que[quenum].gserver;
	
	
	knret = iCheck.isCheckClanName(ClanName,gserver);


	//ktj : �����̹Ƿ� ���߿� ����ÿ�.
	//Į���� ���� ����Ÿ�б�
	//knret = iCheck.read_ClanName(0);
	//�̹����б� : OK
	//knret = iCheck.write_ClanImageFile("a.bmp");
	///knret = iCheck.read_ClanImage("a.bmp");


	
	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_isCheckClanName_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isCheckClanName_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_isCheckClanName_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	
	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}
	
	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
		case 0://Ŭ���̸��� �ߺ����� ����
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 1://Ŭ���̸��� �ߺ�
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		/*case 2://ĳ���Ͱ� �̹� �ٸ� Ŭ���� �Ҽ�
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 3://Ŭ���̸� �ߺ�
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;*/
		case 100:				//���ڰ� ����
		iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 1004:
		iResultCode = ANSdata.ret_val;
			break;
	case 101:				//Ŭ������� �ȵ�.
	case 102:				//Ŭ�������� ����� �ȵ�.
	case 103:				//Ŭ�������� ����� �ȵ�.
		break;
//ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val );

	


	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}



/*///////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ŭ���� ������Լ�
: ��    �� :
: ��    �� :
/////////////////////////////////////////*/
int AUTH_proc_ClanMake( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ClanMake( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ClanMake( int quenum, int threadindex)
{
	//char *userId, *clanName, *expl, *userCid;
	char* ClanZang_ID,*ClanZang_ChaName,*gserver,*ClanName,*expl;
	int knret = 1, iBadMsg = 0;
	DWORD chtype;
	int level;

	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName= AUTH_que[quenum].ClanZang_ChaName;
	ClanName = AUTH_que[quenum].clanName;
	expl = AUTH_que[quenum].expl;
	gserver= AUTH_que[quenum].gserver;
	chtype = AUTH_que[quenum].job_code;
	level = AUTH_que[quenum].level;
	
	
	knret = iCheck.makeClan(ClanZang_ID,gserver,ClanZang_ChaName,ClanName,expl,chtype,level,AUTH_que[quenum].ret_Buf);


	//ktj : �����̹Ƿ� ���߿� ����ÿ�.
	//Į���� ���� ����Ÿ�б�
	//knret = iCheck.read_ClanName(0);
	//�̹����б� : OK
	//knret = iCheck.write_ClanImageFile("a.bmp");
	///knret = iCheck.read_ClanImage("a.bmp");


	
	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
   

int AUTH_proc_ClanMake_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ClanMake_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ClanMake_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	
	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}
	
	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
		case 0://Ʈ����� ����
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 1://���� ����
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf); //Ŭ�� ����� ������ �´�.
			break;
		case 2://ĳ���Ͱ� �̹� �ٸ� Ŭ���� �Ҽ�
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 3://Ŭ���̸� �ߺ�
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 100:				//���ڰ� ����
		iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 1004:
		iResultCode = ANSdata.ret_val;
			break;
	case 101:				//Ŭ������� �ȵ�.
	case 102:				//Ŭ�������� ����� �ȵ�.
	case 103:				//Ŭ�������� ����� �ȵ�.
		break;
//ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val );

	


	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}









/*/////////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ŭ�����Խ� ����� ������Լ�
: ��    �� :
: ��    �� :
//////////////////////////////////////////*/
int AUTH_proc_Mmake( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Mmake(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Mmake( int quenum, int threadindex)
{
	char *clanName, *memId, *memCharId;
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*ClanWon_ID,*ClanWon_ChaName,*chipflag;
	int knret = 1, iBadMsg = 0;
	int level =0;
	int joinlevel;
	DWORD job_code;


	clanName = AUTH_que[quenum].clanName;
	memId     = AUTH_que[quenum].id;				//�ӽ÷� �ƹ����ۿ��� �־��.
	memCharId = AUTH_que[quenum].charId;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	ClanWon_ID = AUTH_que[quenum].ClanWon_ID;
	ClanWon_ChaName = AUTH_que[quenum].ClanWon_ChaName;
	level = AUTH_que[quenum].level;
	job_code = AUTH_que[quenum].joinjobcode;
	joinlevel = AUTH_que[quenum].joinlevel;
	chipflag = AUTH_que[quenum].chipflag;
	

	//ygy
	//knret = iCheck.joinClanMember(clanName, memId, memCharId);
	//char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID
	knret = iCheck.joinClanMember(ClanZang_ID,gserver,ClanZang_ChaName,clanName,ClanWon_ChaName,ClanWon_ID,level,job_code,joinlevel,chipflag);
    
	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;

	return TRUE;
}


int AUTH_proc_Mmake_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Mmake_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Mmake_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
		case 0://Ʈ����� ����
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 1://Ŭ���� ���� ����
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 2://���� ���Խ�Ű���� Ŭ������ �̹� �ٸ� Ŭ���� ���������� ���(�ڱ� Ŭ������)
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 3://Ŭ���� �������� �ʰų� Ŭ�� ���� �ƴѰ��
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 4://Ŭ�� ����� 32 �̻��� ���(Max : 32)
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 100:
			iResultCode = AUTH_que[quenum].result;
			break;
		case 1004:
			iResultCode = AUTH_que[quenum].result;
			break;
	case 101:
	case 102:
	case 103:
		break;
//ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}

	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return 1;
}





/*/////////////////////////////////////////
: �� �� �� :  
: �� �� �� :  ������� Ŭ�� ����Ÿ�� �д´�.
: ��    �� :
: ��    �� :
//////////////////////////////////////////*/

int AUTH_proc_ReadClan( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadClan(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ReadClan( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver;
	int knret = 1, iBadMsg = 0;

	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.read_ClanData(ClanZang_ID,gserver,ClanZang_ChaName, AUTH_que[quenum].ret_Buf);


	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_ReadClan_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReadClan_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_ReadClan_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
		//ygy
		case 0: //������ Ŭ���� ����
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 1://���� ���
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
			break;
		case 100:				//�Ϲ� ���,
			ANSdata.ret_val = iResultCode;
			break;
		case	1004:
			ANSdata.ret_val = iResultCode;
			break;
		case 101:				//Ŭ�����ϰ�� ���� Ŭ���̸��� �˾Ƴ�.
		case 102:				//����ƴ�.
			strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
			break;

			//ygy
		//case AUTH_OK:
		//	break;
		default :
			iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
			break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}



/*/////////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ŭ���� ��ü�Ѵ�.
: ��    �� :
: ��    �� :
//////////////////////////////////////////*/

int AUTH_proc_BreakUP( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_BreakUP(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_BreakUP( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*ClanName;
	int knret = 1, iBadMsg = 0;

	ClanName = AUTH_que[quenum].clanName;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver= AUTH_que[quenum].gserver;
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.BreakUPClan(ClanZang_ID,gserver,ClanZang_ChaName,ClanName);

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_BreakUP_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_BreakUP_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_BreakUP_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
		//ygy
		//case AUTH_OK:
		//break;
		case 0:
			iResultCode = AUTH_que[quenum].result;
			break;
		case 1: //���� ����
			iResultCode = AUTH_que[quenum].result;
			break;
		case 2: //���� ĳ������ ���� Ŭ���� ���� �ƴϰų� �ش�Ŭ���� ����
			iResultCode = AUTH_que[quenum].result;
			break;
			//end ygy
	case 100:				//����⼺��
		iResultCode = AUTH_que[quenum].result;
		break;
	case 1004:
		iResultCode = AUTH_que[quenum].result;
		break;
	case 101:				//clanWon���̺� Ŭ�����ִµ� clanWon���̺� ���°��(ġ����)
	case 102:				//clanWon���̺��� Ŭ������������>
	case 103:				//clanName�ȿ� Ŭ�� ������ ���������������
	case 104:				//clanName���̺��� Ŭ������ ����
		break;

	
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ygy
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}






/*/////////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ư��id�� Ŭ������ Ż��
: ��    �� :
: ��    �� :
//////////////////////////////////////////*/

int AUTH_proc_Secession( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Secession(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Secession( int quenum, int threadindex)
{
	char *clanName, *secession_id,*gserver,*clanzang;
	int knret = 1, iBadMsg = 0;

	clanName = AUTH_que[quenum].clanName;
	//ygy
	//banish_id = AUTH_que[quenum].expl;
	clanzang = AUTH_que[quenum].ClanWon_ChaName;
	gserver = AUTH_que[quenum].gserver;
	secession_id = AUTH_que[quenum].ClanWon_ID;

	/*
	strncpy( AuthQueTmp.clanName, claname, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanWon, SecessionID, sizeof( AuthQueTmp.ClanWon ) );
	strncpy( AuthQueTmp.ClanZang, clanzang, sizeof( AuthQueTmp.ClanZang ) );
	*/

	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.Secession(secession_id,gserver,clanzang ,clanName);
	//int ISAOCHECK::Secession(char* clanzang,char* gserver,char* SecessionID,char* claname)

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_Secession_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Secession_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Secession_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	//ygy
	case 0: //Ʈ����� ����
			iResultCode = AUTH_que[quenum].result;
			break;
	case 1: //Ŭ���� Ż�� ����
			iResultCode = AUTH_que[quenum].result;
			break;
	case 2://Ŭ���� �������� �ʰų� Ŭ�� ���� �ƴѰ�� �Ǵ� Ŭ������ �ƴѰ��
			iResultCode = AUTH_que[quenum].result;
			break;
	case 3://Ż��� ���������� Ŭ��ȸ������ 6�����Ϸ� �������� ���(LimitDate ����)
			iResultCode = AUTH_que[quenum].result;
			break;
	case 100:	
			iResultCode = AUTH_que[quenum].result;
			break;			//�߹� ����
	case ISAO_ERR_TCP_CONNECT: //���� ���ӿ���
			iResultCode = AUTH_que[quenum].result;
			break;
	case 101:				//�߹�Ұ�
		break;
     //ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);
	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}





/*/////////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ư��id�� Ŭ������ �߹��Ѵ�.
: ��    �� :
: ��    �� :
//////////////////////////////////////////*/

int AUTH_proc_Banishiment( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Banishiment(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Banishiment( int quenum, int threadindex)
{
	char *clanName, *ClanWon,*gserver,*ClanZang_ID,*ClanZang_ChaName;
	int knret = 1, iBadMsg = 0;

	clanName = AUTH_que[quenum].clanName;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	ClanWon = AUTH_que[quenum].ClanWon_ChaName;

	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.Banishiment(ClanZang_ID,gserver,ClanZang_ChaName,clanName,ClanWon);

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_Banishiment_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Banishiment_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_Banishiment_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
		//ygy
		case 0: //Ʈ����� ����
			iResultCode = AUTH_que[quenum].result;
			break;
		case 1: //Ŭ���� Ż�� ����
			iResultCode = AUTH_que[quenum].result;
			break;
		case 2://Ŭ���� �������� �ʰų� Ŭ�� ���� �ƴѰ�� �Ǵ� Ŭ������ �ƴѰ��
			iResultCode = AUTH_que[quenum].result;
			break;
		case 3://Ż��� ���������� Ŭ��ȸ������ 6�����Ϸ� �������� ���(LimitDate ����)
			iResultCode = AUTH_que[quenum].result;
			break;
	case 100:				//�߹�Ϸ�
		iResultCode = AUTH_que[quenum].result;
		break;
	case 1004:
		iResultCode = AUTH_que[quenum].result;
		break;
	case 101:				//�߹�Ұ�
		break;
     //ygy
	//case AUTH_OK:
	//	break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);
	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}





/*/////////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ư��id�� Ŭ������ �߹��Ѵ�.
: ��    �� :
: ��    �� :
//////////////////////////////////////////*/

int AUTH_proc_WarhouseR( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_WarhouseR(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_WarhouseR( int quenum, int threadindex)
{
	char *clanName, *banish_id;
	int knret = 1, iBadMsg = 0;

	clanName = AUTH_que[quenum].clanName;
	banish_id = AUTH_que[quenum].expl;
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.WarhouseRight(clanName, banish_id);

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_WarhouseR_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_WarhouseR_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_WarhouseR_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 100:				//�Ϸ�
	case 101:				//
	case 102:				//
		break;

	case AUTH_OK:
		break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("â����� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);
	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}




/*/////////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ŭ���Ἲ�� �÷��� ����
: ��    �� :
: ��    �� :
//////////////////////////////////////////*/

int AUTH_proc_updateFlag( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_updateFlag(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_updateFlag( int quenum, int threadindex)
{
	char *clanName, *banish_id;
	int knret = 1, iBadMsg = 0;

	clanName = AUTH_que[quenum].clanName;
	banish_id = AUTH_que[quenum].expl;
	
	knret = iCheck.updateFlag(clanName);

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_updateFlag_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_updateFlag_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_updateFlag_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 100:				//�Ϸ�
	case 101:				//Ŭ������.
	case 102:				//
		break;

	case AUTH_OK:
		iResultCode = AUTH_que[quenum].result;
		break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("â����� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);
	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}








/*/////////////////////////////////////////
: �� �� �� :  
: �� �� �� :  Ŭ����ũ�б�
: ��    �� :
: ��    �� :
//////////////////////////////////////////*/

int AUTH_proc_MarkREAD( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_MarkREAD(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_MarkREAD( int quenum, int threadindex)
{
	//char *clanName, *banish_id;
	char *Mark,*clanname;
	int knret = 1, iBadMsg = 0;

	Mark = AUTH_que[quenum].Mark;
	clanname = AUTH_que[quenum].clanName;
	//banish_id = AUTH_que[quenum].expl;
	
	knret = iCheck.read_ClanImage(clanname,Mark);

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_MarkREAD_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_MarkREAD_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_MarkREAD_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;	

	//ktj : �ӽ� ����
	//AUTH_que[quenum].result = 2;

	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 1: //Ŭ�� ��ũ �б� ����
			
		iResultCode =AUTH_que[quenum].result ;	// ����Ʈ�� ��Ÿ�� ����
		break;
	case 2://Ŭ�� ��ũ �б� ����
		iResultCode = AUTH_que[quenum].result ;	// ����Ʈ�� ��Ÿ�� ����
		break;

	case 100:				//�Ϸ�
		iResultCode = AUTH_que[quenum].result;
		break;
	case 1004:
		iResultCode = AUTH_que[quenum].result;
		break;
	case 101:				//Ŭ������.
	case 102:				//
		break;
	default:
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;

//	case AUTH_OK:
//		break;
	//default :
	//	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	//	AUTH_que[quenum].result = iResultCode;	// ����Ʈ�� ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("â����� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);
	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

/*/////////////////////////////////////////
: �� �� �� :  
: �� �� �� :  �ٸ� Ŭ����ũ�б�
: ��    �� :
: ��    �� :
//////////////////////////////////////////*/

int AUTH_proc_MarkREAD2( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_MarkREAD2(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_MarkREAD2( int quenum, int threadindex)
{
	//char *clanName, *banish_id;
	char *Mark,*clanname;
	int knret = 1, iBadMsg = 0;

	Mark = AUTH_que[quenum].Mark;
	clanname = AUTH_que[quenum].clanName;
	//banish_id = AUTH_que[quenum].expl;
	
	//knret = iCheck.read_ClanImage2(clanname,Mark);





	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_MarkREAD2_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_MarkREAD2_Result( quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_MarkREAD2_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;	
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
		case 1: //Ŭ�� ��ũ �б� ����
			
			iResultCode =AUTH_que[quenum].result ;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 2://Ŭ�� ��ũ �б� ����
			iResultCode = AUTH_que[quenum].result ;	// ����Ʈ�� ��Ÿ�� ����
			break;
	case 100:				//�Ϸ�
		iResultCode = AUTH_que[quenum].result;
		break;
	case 1004:
		iResultCode = AUTH_que[quenum].result;
		break;
	case 101:				//Ŭ������.
	case 102:				//
		break;
	default:
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;

//	case AUTH_OK:
//		break;
	//default :
	//	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	//	AUTH_que[quenum].result = iResultCode;	// ����Ʈ�� ��Ÿ�� ����
	//	break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("â����� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);
	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}



int AUTH_proc_Updatcldata( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Updatcldata(quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_Updatcldata(int quenum, int threadindex)
{
	char *id,*chaname,*gserver;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	id = AUTH_que[quenum].ClanZang_ID;
	chaname = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.isClanMember(id,gserver,chaname, AUTH_que[quenum].ret_Buf);
	if((knret == 0)||(knret == 2)||(knret == 1)||(knret == 3)||(knret == 4)||(knret == 5)){
		void ParsingIsClanMember(char* data,int myposition);
		ParsingIsClanMember(AUTH_que[quenum].ret_Buf,knret);
		if((knret == 2)||(knret == 1)||(knret == 5)){
			knret=iCheck.read_ClanData(id,gserver,chaname, AUTH_que[quenum].ret_Buf);
			if(knret == 1){
				void ParsigClanWon(char* data);
				ParsigClanWon(AUTH_que[quenum].ret_Buf);
				knret = iCheck.read_ClanImage(chaname,cldata.ClanMark);
			}
		}
	}


	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;

	return 1;
}

int AUTH_proc_Updatcldata_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_Updatcldata_Result(quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_Updatcldata_Result( int quenum, int threadindex)
{
	char szData[128];
	char GServer;
	char szNum[64];
	char szFile[256];
	char szFile2[256];
	ZeroMemory(szNum,sizeof(szNum));
	ZeroMemory(szFile,sizeof(szFile));
	ZeroMemory(szFile2,sizeof(szFile2));

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
		//ygy
		case 0: //������ Ŭ���� ����
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
			break;
		case 1://���� ���
			iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����			
			GServer = cldata.ClanMark[0];
			strcpy(szNum , &cldata.ClanMark[3]);
			wsprintf(szFile,"%s\\%s\\%c_%s.%s",SAVE_DIR,szConnServerName,GServer,szNum,SAVE_EXE );
			wsprintf(szFile2,"%s\\%s\\%c_%s_16.%s",SAVE_DIR,szConnServerName,GServer,szNum,SAVE_EXE);
#ifdef  USE_PROSTONTALE						
			if(cldata.hClanMark16) {
				cldata.hClanMark16->Release();
				cldata.hClanMark16 = NULL;
			}

			if(cldata.hClanMark){
				cldata.hClanMark->Release();
				cldata.hClanMark = NULL;
			}

			cldata.hClanMark16= LoadDibSurfaceOffscreen(szFile2);
			cldata.hClanMark= LoadDibSurfaceOffscreen(szFile);
#endif

			break;
		case 100:				//�Ϲ� ���,
			ANSdata.ret_val = iResultCode;
			break;
		case	1004:
			ANSdata.ret_val = iResultCode;
			break;
		case 101:				//Ŭ�����ϰ�� ���� Ŭ���̸��� �˾Ƴ�.
		case 102:				//����ƴ�.			
			break;

			//ygy
		//case AUTH_OK:
		//	break;
		default :
			iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
			break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	


	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}


	//cE_Updatecldata.cpp�� ����
	void g_UpdateCldata_Close(); //������ ����
	g_UpdateCldata_Close(); //������ ����

	return TRUE;
}

int WebDB_Thread::Updatcldata(char *id, char *gserver, char *chaname)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));


	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_Updatcldata;          // Ŭ����������� �˷��޶�..	
	
	//ygy
	//strncpy( AuthQueTmp.id, id, sizeof( AuthQueTmp.id ) );
	strncpy( AuthQueTmp.ClanZang_ID, id, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver) );
		
	

	//ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}

/*/////////////////////////////////////////////////////////////////////////// END
: �� �� �� :  ��db�� �����ϴ� ���ó�� �Լ� 
///////////////////////////////////////////////////////////////////////////*/















/*/////////////////////////////////////////////////////////////////////////// START
: �� �� �� :  WebDB�� �����ؼ� ������ �����ϴ� �����Լ���.
///////////////////////////////////////////////////////////////////////////*/

//�ش� id�� Ŭ���������� ���θ� �˾Ƴ�.
int WebDB_Thread::isClanMember(char *ClanZang_id, char* gserver,char* ClanZang_ChaName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isClanMember;          // Ŭ����������� �˷��޶�..	
	
	//ygy
	//strncpy( AuthQueTmp.id, id, sizeof( AuthQueTmp.id ) );
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_id, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver) );

	//ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}




//Ŭ������� ���Խ�Ų��.
int WebDB_Thread::joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID,int level,DWORD chtype,int joinlevel,char *chipflag)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp, sizeof(AuthQueTmp));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_CL_MEMBER_MAKE;          // Ŭ������� ����

	//ygy	
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.ClanWon_ChaName, ClanWon, sizeof( AuthQueTmp.ClanWon_ChaName ) );
	strncpy( AuthQueTmp.ClanWon_ID, ClanWon_ID, sizeof( AuthQueTmp.ClanWon_ID ) );
	strncpy( AuthQueTmp.chipflag, chipflag, sizeof( AuthQueTmp.chipflag ) );
	
	AuthQueTmp.level = level;
	AuthQueTmp.joinlevel = joinlevel;
	AuthQueTmp.joinjobcode = (int)chtype;



	//ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}





//Ŭ���� �����ʹٸ� �����.
int WebDB_Thread::make_Clan(char *ClanZang_ID, char* gserver,char* ClanZang_ChaName,char* ClanName,char *explanation,DWORD chtype,int level)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_CL_MAKE;          // �����ϴ�
		
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.expl, explanation, sizeof( AuthQueTmp.expl ) );
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );

	AuthQueTmp.level = level;
	AuthQueTmp.job_code = chtype;

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}





//Ŭ���� �д´�.
int WebDB_Thread::ReadClan(char *ClanZang_ID,char* gserver,char* ClanZang_ChaName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_ReadClan;          // �����ϴ�
	
	//ygy	
	strncpy(AuthQueTmp.ClanZang_ID,ClanZang_ID,sizeof(AuthQueTmp.ClanZang_ID));
	strncpy(AuthQueTmp.gserver,gserver,sizeof(AuthQueTmp.gserver));
	strncpy(AuthQueTmp.ClanZang_ChaName,ClanZang_ChaName,sizeof(AuthQueTmp.ClanZang_ChaName));


	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}


//Ŭ���� ��ü�Ѵ�.
int WebDB_Thread::BreakUP_Clan(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName, char *ClanName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_BreakUP;          // �����ϴ�
	
	//ygy	
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName) );
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("BreakUP_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}



//Ư�� �Ƶ� �߹��Ѵ�.
int WebDB_Thread::Banishiment(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName,char *ClanName, char *ClanWon)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_Banishiment;          // �����ϴ�	

	//ygy
	strncpy( AuthQueTmp.clanName, ClanName, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.ClanZang_ID, ClanZang_ID, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, ClanZang_ChaName, sizeof( AuthQueTmp.ClanZang_ChaName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver) );
	strncpy( AuthQueTmp.ClanWon_ChaName, ClanWon, sizeof( AuthQueTmp.ClanWon_ChaName) );
	//strncpy( AuthQueTmp.expl, id_str, strlen( id_str ) );			//�ӽ÷� .expl���� ����.

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("BreakUP_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}



//�ʱ�ȭ�� �÷���

//Ż���Ѵ�.
int WebDB_Thread::Secession(char* clanzang,char* gserver,char* SecessionID,char* claname)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp, sizeof( AUTH_QUE  ) );

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_Secession;          // �����ϴ�

	strncpy( AuthQueTmp.clanName, claname, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanWon_ChaName, SecessionID, sizeof( AuthQueTmp.ClanWon_ChaName ) );
	strncpy( AuthQueTmp.ClanWon_ID, clanzang, sizeof( AuthQueTmp.ClanZang_ID ) );

	//ygy
	//strncpy( AuthQueTmp.expl, id_str, strlen( id_str ) );			//�ӽ÷� .expl���� ����.

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("BreakUP_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}




//â����� �ٲ�
int WebDB_Thread::Warhouse_right(char *clanName, char *right_str)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_WarhouseR			;          // �����ϴ�
	

	strncpy( AuthQueTmp.clanName, clanName, sizeof( AuthQueTmp.clanName ) );
	strncpy( AuthQueTmp.expl, right_str, strlen( right_str ) );			//�ӽ÷� .expl���� ����.

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("Warhouse_right() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}


//����================================================================
int AUTH_proc_LeaveClan( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_LeaveClan( quenum, threadindex);
}
int AUTH_proc_LeaveClan_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_LeaveClan_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_LeaveClan( int quenum, int threadindex)
{
	
	char *clanname, *chaname,*gserver;
	int knret = 1, iBadMsg = 0;

	chaname = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	clanname = AUTH_que[quenum].clanName;
	
	knret = iCheck.LeaveClan(clanname,chaname,gserver);

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return 1;
}
int WebDB_Thread::AUTH_proc_LeaveClan_Result( int quenum, int threadindex)
{
	char szData[128];

	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����
	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 1:				//�Ϸ�
	case 2:				//Ŭ������ �ƴ�
	case 3:				//�������� �ʴ� Ŭ��
	case 4:				//���� 50�̸�
	case 5:				//���� �ڰݸ� ���� ???????????????????????
	case 6:				//����ĳ������ �����ִ��ο��� ���� ���� �ο����� ŭ
	case 100:			//���ڰ� �����������ϰ�
		break;
	case 1004:				//�������پȵȰ��
		break;
	default :
		iResultCode = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("â����� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);
	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
	
}
int WebDB_Thread::LeaveClan(char *clanname,char *chaname,char *gserver)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp, sizeof(AuthQueTmp));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_LeaveClan;          // Ŭ������� ����

	//ygy	
	strncpy( AuthQueTmp.clanName, clanname, sizeof( AuthQueTmp.ClanZang_ID ) );
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );	


	//ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;	
}


//Ŭ���Ἲ�ϷḦ ���� �÷��׼���
int WebDB_Thread::updateFlag(char *clanName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_updateFlag;		//Ŭ���Ἲ�ϷḦ ���� �÷��׼���	
	
	strncpy( AuthQueTmp.clanName, clanName, sizeof( AuthQueTmp.clanName ) );

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("Warhouse_right() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}



//Ŭ����ũ�б�
int WebDB_Thread::MarkREAD(char *clanname,char *mark)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_MarkREAD;		//Ŭ���Ἲ�ϷḦ ���� �÷��׼���	
	
	strncpy( AuthQueTmp.Mark, mark, sizeof( AuthQueTmp.Mark ) );
	strncpy(AuthQueTmp.clanName,clanname,sizeof(AuthQueTmp.clanName));

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("Warhouse_right() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}

//�ٸ� Ŭ����ũ�б�
int WebDB_Thread::MarkREAD2(char *clanname,char *mark)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_MarkREAD2;		//Ŭ���Ἲ�ϷḦ ���� �÷��׼���	
	
	strncpy( AuthQueTmp.Mark, mark, sizeof( AuthQueTmp.Mark ) );
	strncpy(AuthQueTmp.clanName,clanname,sizeof(AuthQueTmp.clanName));

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("Warhouse_right() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}




//ktj : ���õ� ȣ��Ʈ�̸��� NULL�̸� Ŭ���޴��� '������'���� �پ��ش�.
extern BOOL bCheckHostNameandPort ;			//tjclan.cpp
void WebDB_Thread::CheckHostNamePort(char *hostname, int port)
{
	if(hostname == NULL) {
		bCheckHostNameandPort = FALSE;
		return;
	}
	if(port == 0){
		bCheckHostNameandPort = FALSE;
		return;
	}

	bCheckHostNameandPort = TRUE;
}
















int WebDB_Thread::start_GOLDmoney(int fd, char *cdkey, char *accountid, char *passwd, char *ip_address)
{
	return 0;
		//ktj : ������ ����ó������ ���� ����� ����.
		AUTH_QUE AuthQueTmp;
		ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

		/*
		//ktj : ������.
		if(Connect[fd].howmanyTime==0) return 0;		//����� �α��� �ȵǼ� ������ ����ó�� ����.

		// ť�� ������ �ִ´٣�
		AuthQueTmp.fdid = Connect[fd].fdid;
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH;
		//ktj : ���γ���.
		AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH_KTJEND;
		
		strncpy( AuthQueTmp.cdkey, Connect[fd].cdkey, sizeof( AuthQueTmp.cdkey ) );
		strncpy( AuthQueTmp.isaokey, Connect[fd].accountid, sizeof( AuthQueTmp.isaokey ) );
		strncpy( AuthQueTmp.password, Connect[fd].passwd, sizeof( AuthQueTmp.password ) );
		strncpy( AuthQueTmp.ip_address, Connect[fd].ip_address, sizeof( AuthQueTmp.ip_address ) );
		AuthQueTmp.howmanyTime = Connect[fd].howmanyTime;
		*/

		AuthQueTmp.fdid = 100;
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH;
		//ktj : ���γ���.
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH_KTJEND;
		AuthQueTmp.command = AUTH_QUE_CMD_CL_MAKE;          // �����ϴ�
		
		strncpy( AuthQueTmp.cdkey, cdkey, sizeof( AuthQueTmp.cdkey ) );
		strncpy( AuthQueTmp.isaokey, accountid, sizeof( AuthQueTmp.isaokey ) );
		strncpy( AuthQueTmp.password, passwd, sizeof( AuthQueTmp.password ) );
		strncpy( AuthQueTmp.ip_address, ip_address, sizeof( AuthQueTmp.ip_address ) );

		long myTime;
		time( ( time_t* )&myTime );
		AuthQueTmp.howmanyTime = myTime;//Connect[fd].howmanyTime = myTime;
	
		// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "start_GOLDmoney����\n" );
		fd2("������ ���� ���� ť�� ��Ͼȵ�. %s \n", accountid );
		return 0;
	}
	return 1;
}

//ktj ���� ����.: �� ����ó�� 
int WebDB_Thread::end_GOLDmoney(int fd, char *cdkey, char *accountid, char *passwd, char *ip_address)
{
		//ktj : ������ ����ó������ ���� ����� ����.
		AUTH_QUE AuthQueTmp;


		/*
		//ktj : ������.
		if(Connect[fd].howmanyTime==0) return 0;		//����� �α��� �ȵǼ� ������ ����ó�� ����.

		// ť�� ������ �ִ´٣�
		AuthQueTmp.fdid = Connect[fd].fdid;
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH;
		//ktj : ���γ���.
		AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH_KTJEND;
		
		strncpy( AuthQueTmp.cdkey, Connect[fd].cdkey, sizeof( AuthQueTmp.cdkey ) );
		strncpy( AuthQueTmp.isaokey, Connect[fd].accountid, sizeof( AuthQueTmp.isaokey ) );
		strncpy( AuthQueTmp.password, Connect[fd].passwd, sizeof( AuthQueTmp.password ) );
		strncpy( AuthQueTmp.ip_address, Connect[fd].ip_address, sizeof( AuthQueTmp.ip_address ) );
		AuthQueTmp.howmanyTime = Connect[fd].howmanyTime;
		*/

		AuthQueTmp.fdid = 100;
		//AuthQueTmp.command = AUTH_QUE_COMMAND_AUTH;
		//ktj : ���γ���.
		AuthQueTmp.command = AUTH_QUE_CMD_CL_MEMBER_MAKE;
		
		strncpy( AuthQueTmp.cdkey, cdkey, sizeof( AuthQueTmp.cdkey ) );
		strncpy( AuthQueTmp.isaokey, accountid, sizeof( AuthQueTmp.isaokey ) );
		strncpy( AuthQueTmp.password, passwd, sizeof( AuthQueTmp.password ) );
		strncpy( AuthQueTmp.ip_address, ip_address, sizeof( AuthQueTmp.ip_address ) );
		AuthQueTmp.howmanyTime = 100;//Connect[fd].howmanyTime;
	
		// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );


		//ktj : ���׶����� �ϴ� ������.
		//nrproto_ClientLogin_send( fd, 0, "end_GOLDmoney ����\n" );

		fd2("������ ���� ���� ť�� ��Ͼȵ�. %s \n", accountid );
		return 0;
	}
	return 1;
}
/*/////////////////////////////////////////////////////////////////////////// END
: �� �� �� :  WebDB�� �����ؼ� ������ �����ϴ� �����Լ���.
///////////////////////////////////////////////////////////////////////////*/

char web_DBip0[64] = {0,};
int  web_DBport0 = 0;
char web_DBip1[64] = {0,};
int  web_DBport1 = 0;
int  nClanTicket = 0; //ticket��ȣ



void web_DB_ip_port_init(char *ip0, int port0, char *ip1, int port1)
{
	strcpy(web_DBip0, ip0);
	strcpy(web_DBip1, ip1);

	web_DBport0 = port0;
	web_DBport1 = port1;

}

void web_DB_ip_port_init(char *ip0, int port0, char *ip1, int port1,int ticket)
{
	strcpy(web_DBip0, ip0);
	strcpy(web_DBip1, ip1);

	web_DBport0 = port0;
	web_DBport1 = port1;
#ifdef __CLANSERVER__
	nClanTicket = 9999;
#else
	nClanTicket = ticket;
#endif
}

//winmain.cpp���� ���õ�.
void web_DB_start(char *hostN0, int port0, char *hostN1, int port1);
void web_DB_start()
{
	//dns
	//char hostN0[] ="clanserver.pristontale.com" ; //"211.39.144.157"
	//char hostN1[] = "image.pristontale.com";//"211.39.144.158";

	//ip
	char hostN0[] ="211.39.144.157";
	char hostN1[] ="211.39.144.158";



	//char hostN0[] = "211.39.144.155";
	//char hostN1[] = "211.39.144.154";

	int port0 = 80;
	int port1 = 80;

	//ktj : ���� : ������ web_DBip0�� ������ �مf��(������ ���ص� �ǵ���)
	//if(web_DBip0[0]==0) {		//��������ȵ������� �׳� ��Į ���������.
	//	 web_DB_start(hostN0, port0, hostN1, port1 );
	//}
	//else {
		web_DB_start(web_DBip0, web_DBport0 , web_DBip1, web_DBport1 );
	//}
	
	//ktj : �ӽ�
	//web_DB_start(hostN0, port0, hostN1, port1 );

	//web_DB_start(NULL, port0, NULL, port1  );
}


BOOL bip_port_error = FALSE;
//KTJ : �ӽ� �Լ���
////ktj : winmain.cpp�� int SetGameMode( int mode ) �� ���õǾ��� �ִ°���
void web_DB_start(char *hostN0, int port0, char *hostN1, int port1)
{		
	if((hostN0 == NULL)||(port0 == 0) || (hostN1 == NULL)|| (port1 == 0)) {
		bip_port_error = TRUE;


		//�޽��� ��������� ������ �����Ǿ����.
		WebDB.AUTH_init(10);
		WebDB.ThreadS.flag = FLAG_WAIT;
		WebDB.Suspend();     //�ϴ� ����

		while(1) {
			if(WebDB.ThreadS.flag == FLAG_WAIT)
				break;
		}

		return;
	}
	static cnt=0;
	if(cnt==1) return;
	cnt=1;
	/*
	char hostN[] =  "211.39.144.157";
	char hostN1[] = "211.39.144.158";
	int port = 80;
	int port1 = 80;
	*/



	//===================================================================================
	//���� : ���� �ؾ���.================================================================
	//===================================================================================

	//iCheck.init(1, 30);		//��db���ø��� ����
	iCheck.init(1, 5);		//��db���ø��� ����

	//Ŭ������ ȣ��Ʈ �����ǰ� 0, �׸��� ��Ʈ�� 0 �� ��� Ȯ���ϴ� �Լ�
	WebDB.CheckHostNamePort(hostN0,port0); 

	iCheck.init_HostName_Port (hostN0, port0);
	iCheck.init_HostName_Port1(hostN1 ,port1);		//�����Ұ�.

	//===================================================================================
	//���� : ���� �ؾ���.================================================================
	//===================================================================================

	WebDB.AUTH_init(10);
	WebDB.ThreadS.flag = FLAG_WAIT;
	WebDB.Suspend();     //�ϴ� ����

	while(1) {
		if(WebDB.ThreadS.flag == FLAG_WAIT)
			break;
	}
}


int web_DB_state_FLAG_WAIT()
{
	if(WebDB.ThreadS.flag == FLAG_WAIT)
		return 1;

	return 0;
}


void web_DB_main()
{
	WebDB.main();
}

void web_DB_end()
{
	WebDB.end();
}



//���Ӱ��� �÷���================================================================




int AUTH_proc_isKPFlag( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isKPFlag( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isKPFlag( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*PFlag,*Gubun,*ClanName;
	int knret = 1, iBadMsg = 0;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;
	PFlag = AUTH_que[quenum].PFlag;
	Gubun = AUTH_que[quenum].Gubun;
	ClanName = AUTH_que[quenum].clanName;
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.isKPFlag(ClanZang_ID,ClanName,ClanZang_ChaName,gserver, PFlag,Gubun);


	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_isKPFlag_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_isKPFlag_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_isKPFlag_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.		
		break;	
	case 100:				//�Ϲ� ���,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

int WebDB_Thread::isKPFlag(char *userid, char *clName, char *chName,char *gserver, char *PFlag, char *Gubun)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_isKPFlag;          // �����ϴ�
	
	strncpy( AuthQueTmp.Gubun, Gubun, sizeof( AuthQueTmp.Gubun ) );
	strncpy(AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ));
	strncpy( AuthQueTmp.clanName, clName, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.ClanZang_ChaName, chName, sizeof( AuthQueTmp.ClanZang_ChaName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy( AuthQueTmp.PFlag, PFlag, sizeof( AuthQueTmp.PFlag ) );
	

	

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}

int WebDB_Thread::AppointSubChip(char* userid,char* chaname,char* clanname,char *gserver,char* clanwon)//��Ŭ���� �Ӹ�
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_AppointSubChip;          // �����ϴ�
		
	strncpy(AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ));
	strncpy( AuthQueTmp.clanName, clanname, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy(AuthQueTmp.ClanWon_ChaName,clanwon,sizeof(AuthQueTmp.ClanWon_ChaName));;
	
	

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}



int AUTH_proc_AppointSubChip( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_AppointSubChip( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_AppointSubChip( int quenum, int threadindex)
{
	char *ClanZang_ID,*ClanZang_ChaName,*gserver,*ClanName;
	int knret = 1, iBadMsg = 0;
	//char *gserver;
	char *clanwon;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;	
	clanwon = AUTH_que[quenum].ClanWon_ChaName;
	ClanName = AUTH_que[quenum].clanName;
	
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.AppointSubChip(gserver,clanwon);

	//��Ŭ��Ĩ �Ӹ��� �����Ǹ� �÷��װ� ����(�÷��װ� : 300)
	//if(knret == 1)
	//{
	//	knret = iCheck.isKPFlag(ClanZang_ID,ClanZang_ChaName,ClanName,gserver,"300","0");
	//}

	

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}

int AUTH_proc_AppointSubChip_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_AppointSubChip_Result( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_AppointSubChip_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 0://Ŭ������ �ƴ�
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		break;
	case 3://��Ŭ��Ĩ�� ������
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.		
		break;
	case 2: //Ŭ��Ĩ�̰ų� 
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.		
		break;	
	case 1: //���漺��
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.		
		break;

	case 100:				//�Ϲ� ���,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

	
int WebDB_Thread::ReleaseSubChip(char* userid,char* chaname,char* clanname,char *gserver,char* clanwon) //�� Ŭ���� ����
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_ReleaseSubChip;          // �����ϴ�
		
	strncpy(AuthQueTmp.ClanZang_ID, userid, sizeof( AuthQueTmp.ClanZang_ID ));
	strncpy( AuthQueTmp.clanName, clanname, sizeof( AuthQueTmp.clanName ) );	
	strncpy( AuthQueTmp.ClanZang_ChaName, chaname, sizeof( AuthQueTmp.ClanZang_ChaName ) );	
	strncpy( AuthQueTmp.gserver, gserver, sizeof( AuthQueTmp.gserver ) );
	strncpy(AuthQueTmp.ClanWon_ChaName,clanwon,sizeof(AuthQueTmp.ClanWon_ChaName));;
	
	

	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}

//��Ŭ���� ����
int AUTH_proc_ReleaseSubChip( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReleaseSubChip(quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_ReleaseSubChip( int quenum, int threadindex)
{
	int knret = 1, iBadMsg = 0;
	char *gserver;
	char *clanwon;
	char *ClanZang_ID,*ClanZang_ChaName,*ClanName;

	//id = AUTH_que[quenum].id;
	ClanZang_ID = AUTH_que[quenum].ClanZang_ID;
	ClanZang_ChaName = AUTH_que[quenum].ClanZang_ChaName;
	gserver = AUTH_que[quenum].gserver;	
	clanwon = AUTH_que[quenum].ClanWon_ChaName;
	ClanName = AUTH_que[quenum].clanName;
	
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.ReleaseSubChip(gserver,clanwon);
	//if(knret == 1) //��Ŭ��Ĩ ���Ӱ��� �÷��� ����(301)
	//{
	//	knret = iCheck.isKPFlag(ClanZang_ID,ClanZang_ChaName,ClanName,gserver,"301","0");
	//}

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}


int AUTH_proc_ReleaseSubChip_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_ReleaseSubChip_Result(quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_ReleaseSubChip_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 0: //Ŭ������ �ƴҰ��
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		break;
	case 1://���� ����
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		break;
	case 2: //Ŭ�����̰ų� ��Ŭ���� ���
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.		
		break;	
	case 100:				//�Ϲ� ���,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	return TRUE;
}

/*////////////////////////////////////////////////////////////////////////////////
WAV ���� ��������
////////////////////////////////////////////////////////////////////////////////*/
int	WebDB_Thread::GetWavFile(char* FileName)
{
	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_GetWavFile;          // �����ϴ�
		
	strncpy(AuthQueTmp.szWavFileName, FileName, sizeof( AuthQueTmp.szWavFileName ));
	
	long myTime;
	time( ( time_t* )&myTime );
	AuthQueTmp.howmanyTime = myTime;
	
	// ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}
int AUTH_proc_GetWavFile( int quenum, int threadindex)
{
	return  WebDB.AUTH_proc_GetWavFile(quenum,threadindex);
}

int WebDB_Thread::AUTH_proc_GetWavFile( int quenum, int threadindex)
{
	int knret = 1, iBadMsg = 0;
	char *szFileName;	

	szFileName = AUTH_que[quenum].szWavFileName;
	
	
	//id�� �ش��ϴ� �ڰ� Ŭ��������, 100(Ŭ�����), 101(Ŭ����:Ŭ������������), �ƴϸ� 102(Ŭ���� �����������)
	knret = iCheck.GetWavFile(szFileName);
	//if(knret == 1) //��Ŭ��Ĩ ���Ӱ��� �÷��� ����(301)
	//{
	//	knret = iCheck.isKPFlag(ClanZang_ID,ClanZang_ChaName,ClanName,gserver,"301","0");
	//}

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
int AUTH_proc_GetWavFile_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_GetWavFile_Result(quenum,threadindex);
}

int WebDB_Thread::AUTH_proc_GetWavFile_Result( int quenum, int threadindex)
{
	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 0: //Ŭ������ �ƴҰ��
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		break;
	case 1://���� ����
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		break;
	case 2: //Ŭ�����̰ų� ��Ŭ���� ���
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.		
		break;	
	case 100:				//�Ϲ� ���,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;		
	default :
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	//ktj : �ϴ� ����
	// ����� �����ϰ� ���� �ش٣�
	//ktj : �����ִºκ��� �־����.
	/*
	nrproto_ClientLogin_send( 
			fd, 
			iResultCode,
			szData);
	//nrproto_ClientLogin_send( fd, 1, "OK" );
	*/
	//ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}

	return TRUE;
}




//=================================================================



//sod ����
int sod2INFOindex(char *UserId, char* CharName,char* GServer , int Index)
{
	WebDB.sod2INFOindex(UserId, CharName,GServer , Index);
	return TRUE;
}


//sod ����
int WebDB_Thread::sod2INFOindex(char *UserId, char* CharName,char* GServer , int Index)
{
	Resume();      //�簳


	AUTH_QUE AuthQueTmp;
	ZeroMemory(&AuthQueTmp,sizeof(AUTH_QUE));

	AuthQueTmp.fdid = 100;
	AuthQueTmp.command = AUTH_QUE_CMD_SOD2;          // Ŭ����������� �˷��޶�..	
	

	strncpy( AuthQueTmp.id, UserId, sizeof( AuthQueTmp.id ) );
	strncpy( AuthQueTmp.charId, CharName, sizeof( AuthQueTmp.charId ) );
	strncpy( AuthQueTmp.gserver, GServer, sizeof( AuthQueTmp.gserver) );
	AuthQueTmp.sod2index = Index;
	
	//ť�� ���
	if( AUTH_addQue( &AuthQueTmp ) < 0 ){
		// �����ϸ� ������ ���� �ش٣���Ÿ�� ������
		//nrproto_CharLogin_send( fd, AUTH_ERROR_OTHER, "authorization required\n" );

		//ktj : ���׶����� �ϴ� ����
		//nrproto_ClientLogin_send( fd, 0, "make_Clan() ����\n" );
		fd2("make_Clan() ����   : ť�� ��Ͼȵ�");
		return 0;
	}
	return 1;
}





/*///////////////////////////////////////
: �� �� �� :  

: �� �� �� :  Ŭ����������� �˾Ƴ����Լ�
: ��    �� :
: ��    �� :
/////////////////////////////////////////*/
int AUTH_proc_sodindex( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_sodindex( quenum, threadindex);
}

int WebDB_Thread::AUTH_proc_sodindex( int quenum, int threadindex)
{

	int knret = 1, iBadMsg = 0;
	knret = iCheck.read_sod2(AUTH_que[quenum].id, AUTH_que[quenum].charId,AUTH_que[quenum].gserver, AUTH_que[quenum].sod2index  , AUTH_que[quenum].szSod2Buff);

	// ����� �ݳ�
	AUTH_que[quenum].result = knret;
	// ����� �ݳ�
	AUTH_que[quenum].badmsg = iBadMsg;
	return TRUE;
}
   

int AUTH_proc_sodindex_Result( int quenum, int threadindex)
{
	return WebDB.AUTH_proc_sodindex_Result(quenum, threadindex);
}
int WebDB_Thread::AUTH_proc_sodindex_Result( int quenum, int threadindex)
{

	cSinSod2.RecvWebData(AUTH_que[quenum].sod2index , AUTH_que[quenum].szSod2Buff);

	/*

	char *aszAuthErrStr[] = {
	"OK",
	"Err CDKEY", 
	"Err ISAO_ID", 
	"Err PASSWORD", 
	"Err EXPIRE", 
	"Err LOCOUT", 
	"Err ISAO_DB", 
	"Err OTHER", 
	""
	};

	char szData[128];
	
	if( AUTH_que[quenum].result == 0 ){
		szData[0] = '\0';
	}else{
		sprintf( szData, "faild" );
	}

	char szBuffer[256];

	// ���� �޽��� ���̺��� ��������� ���ڷ� ǥ��
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
		sprintf( szBuffer, "%s", aszAuthErrStr[AUTH_que[quenum].result] );
	}else{
		sprintf( szBuffer, "ExtraCode=%d", AUTH_que[quenum].result );
	}

	int iResultCode;
	iResultCode = AUTH_que[quenum].result;	// ����Ʈ�� ��Ÿ�� ����

	
	// ���������� Ŭ���̾�Ʈ���� ���� �ִ� �ڵ�
	switch( iResultCode ){
	case 0:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 1:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 5:
	case 4:
	case 3:
	case 2:
		ANSdata.ret_val = iResultCode;				//tjclan.cpp�� ���ǵ� ������ �亯�� �ִ´�.
		strcpy(ANSdata.ret_Buf, AUTH_que[quenum].ret_Buf);
		break;
	case 100:				//�Ϲ� ���,
		ANSdata.ret_val = iResultCode;
		break;
	case 1004:
		ANSdata.ret_val = iResultCode;
		break;
	default:
		ANSdata.ret_val = AUTH_ERROR_OTHER;	// ��Ÿ�� ����
		break;
	}


	fd2("Ŭ����������� �亯�� ����. :  ret_val %d  ", ANSdata.ret_val);

	

	// �������� ���� �̸��� OK.
	if( 0 <= AUTH_que[quenum].result && AUTH_que[quenum].result < AUTH_ERROR_END ){
	}else{
		AUTH_que[quenum].result = AUTH_ERROR_OTHER;
	}
	*/

	return TRUE;
}
