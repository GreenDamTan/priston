//-----------------------------------------------------------------------
//
//  isao�� ������ ssl�� üũ.
//
//-----------------------------------------------------------------------
#define __ISAOCHECK_CPP__
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <Winsock.h>
#include <wininet.h>
#include <time.h>

#include "../clanmenu/tjclanDEF.h"
#include "Language\\language.h"

#ifdef USE_PROSTONTALE		//=========== �������ϼҽ��ΰ��
	#include "sinbaram\\SinLinkHeader.h"
	#include "HoBaram\\HoLinkHeader.h"
	#include "HoBaram\\HoTextFile.h"
	#include "..\\clanmenu\\cE_CViewClanInfo.h"
#endif


#include "..\\clanmenu\\BMPsmooth.h"
#include "..\\clanmenu\\tjclan.h"
#include "..\\clanmenu\\cE_CViewClanInfo.h"
//#include "..\\clanmenu\\Zip.h"
#include <wininet.h>
#pragma comment(lib,"wininet.lib")


//ktj : ���� ������� ����ؾ� ���� ��ŷ�� �ȵʿ� �����Ұ�.
#include "isaocheck.h"

extern void fd( char *fmt, ...);
extern void fd2( char *fmt, ...);

#define BUF_LEN 4096

extern char szConnServerName[16];
extern int  nClanTicket; //ticket��ȣ (���߿� �ӽ� Ƽ�Ϲ�ȣ 9999)

extern BOOL	g_bDebugOption;
void debug_fd2( char *fmt, ...)
{
	if(!g_bDebugOption) return;

	char buff[4096];
	
    va_list args;

    va_start( args, fmt );
    vsprintf( buff, fmt, args );
    va_end( args );

    strcat( buff, "\r\n" );


	int errorMSGdr=0;
	if(strlen(buff)>2048 ) {
		buff[1024] = 0;
		errorMSGdr= 1;




		HANDLE hFile;
		DWORD dwAcess =0;
		hFile = CreateFile( "DDD2.txt", GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
							NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

		if( hFile != INVALID_HANDLE_VALUE ) {
			SetFilePointer( hFile , 0 , NULL , FILE_END );

			//����Ÿ ����     ����    ��ũ��    ������ ������ ����Ʈ��
			int chk = WriteFile( hFile , buff , lstrlen(buff) , &dwAcess , NULL );


			//����Ÿ ����     ����    ��ũ��    ������ ������ ����Ʈ��
			if(errorMSGdr== 1) {
				char msg[256] = "..\n�޽����� �ʹ��� �߶���";
				WriteFile( hFile , msg, lstrlen(msg) , &dwAcess , NULL );
			}
			

			//���� �ݱ�
			CloseHandle( hFile );
		}




		return;
		
	}

	HANDLE hFile;
    DWORD dwAcess =0;
    hFile = CreateFile( "DDD2.txt", GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile != INVALID_HANDLE_VALUE ) {
        SetFilePointer( hFile , 0 , NULL , FILE_END );
        //����Ÿ ����     ����    ��ũ��    ������ ������ ����Ʈ��
        int chk = WriteFile( hFile , buff , lstrlen(buff) , &dwAcess , NULL );


		if(errorMSGdr== 1) {
			char msg[256] = "\n�޽����� �ʹ��� �߶���";
			WriteFile( hFile , msg, lstrlen(msg) , &dwAcess , NULL );
		}
		

        //���� �ݱ�
        CloseHandle( hFile );
    }
}










BOOL GetHttpRequest(LPSTR httpname,LPSTR Request,DWORD MaxBuffer,int *len)
{
	char szSizeBuffer[32];
    DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
    DWORD dwFileSize = 0;
    DWORD dwBytesRead = 0;
    BOOL bSuccessful = FALSE;
    //int i;
 
    HINTERNET m_Session = NULL;
    m_Session = InternetOpen("AppName",  //���ø����̼��� �̸�
							INTERNET_OPEN_TYPE_PRECONFIG,  
							// Use registry settings.
                            NULL,
							// Proxy name. NULL indicates use default.
                            NULL,  
							// List of local servers. NULL indicates default.
                            0) ;
 	HINTERNET hHttpFile = NULL;
    hHttpFile = InternetOpenUrl(m_Session, (const char *) httpname, NULL,
								0, 0, 0);
    if (hHttpFile)
    {
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer,
					    &dwLengthSizeBuffer, NULL) ;
	    //BOOL bQuery = InternetQueryDataAvailable(hHttpFile,&dwLengthSizeBuffer,0,0);
        if(bQuery==TRUE)
        {
			dwFileSize=atol(szSizeBuffer);                                                          
            dwFileSize > MaxBuffer ? dwFileSize = MaxBuffer :
			dwFileSize = dwFileSize;
 
            BOOL bRead = InternetReadFile(hHttpFile, Request,dwFileSize, &dwBytesRead);
            if (bRead)
            {
				bSuccessful = TRUE;
				*len = dwBytesRead;
		
				//i = lstrlen(Request);
            }
            InternetCloseHandle(hHttpFile);
		}
	}
    if(hHttpFile) InternetCloseHandle(hHttpFile);
    if(m_Session) InternetCloseHandle(m_Session);
    return bSuccessful;
 }            




/*///////////////////////////////////////////////////////////////////////////
: Ŭ�� ��: class ISAOCHECK
: �� �� �� : ��db�� �̿� ����Ÿ�� �����´�.
: ��    �� :
: ��    �� :
: �� ȯ �� :
: ��    �� : ��db�� ����� ����Ÿ�� �̾Ƴ��� �Ұ�� �� Ŭ�󽺸� �̿��Ѵ�.
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
ISAOCHECK::ISAOCHECK()
{

	HSsu = 0;						//��罺Ʈ���� ����
	for(int i=0; i<30; i++)
		fileDATA_HS[i]= NULL;		//�� 2048binary�� ����Ÿ�� 4096 hexa string���� �ٲ���.
}

ISAOCHECK::~ISAOCHECK()
{
}

void ISAOCHECK::init()
{
	iCheckFlag = 0;
	hostPORT = 0;
}

void ISAOCHECK::init_HostName_Port(char *hostname, int port )
{
	if(hostname==NULL) return;

	hostPORT = port;
	ZeroMemory(hostName, sizeof(char)*64 );
//#ifdef __CLANSERVER__
	//strcpy(hostName, "211.44.231.10");
//#else
	strcpy(hostName, hostname);
///#endif
}

void ISAOCHECK::init_HostName_Port1(char *hostname, int port )
{
	if(hostname==NULL) return;

	hostPORT1 = port;
	ZeroMemory(hostName1, sizeof(char)*64 );
	strcpy(hostName1, hostname);
}

void ISAOCHECK::init(int iCheckON, int iTimeOUT )
{
	iCheckFlag = iCheckON;	//config.isaocheck���� ýũ��
	iTimeOut = iTimeOUT;
}

void ISAOCHECK::close()
{
}


void ISAOCHECK::main()
{

}



/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int connect_WEBdb( char *szWriteBuffer, char *szReadBuffer , long StartTime )
: �� �� �� :  ��db�� ������ ����Ÿ�� ���ϴ� �Լ�
: ��    �� :
: ��    �� :
: �� ȯ �� :
: ��    �� :  �� Ŭ���� �ٽ� �Լ���.
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/

int ISAOCHECK::connect_WEBdb( char *szWriteBuffer, char *szReadBuffer)
{
	char szSizeBuffer[32];
    DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
    DWORD dwFileSize = 0;
    DWORD dwBytesRead = 0;
    
    int iRet= ISAO_ERR_WINSOCK_NOERR;

    ZeroMemory(szReadBuffer,BUF_LEN);
    //int i;
 
    HINTERNET m_Session = NULL;
    
    m_Session = InternetOpen("AppName",  //���ø����̼��� �̸�
							INTERNET_OPEN_TYPE_PRECONFIG,  
							// Use registry settings.
                            NULL,
							// Proxy name. NULL indicates use default.
                            NULL,  
							// List of local servers. NULL indicates default.
                            0) ;
    if(!m_Session) 
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
 	HINTERNET hHttpFile = NULL;
    hHttpFile = InternetOpenUrl(m_Session, (const char *) szWriteBuffer, NULL,
								0, 0, 0);
    if (hHttpFile)
    {
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer,
					    &dwLengthSizeBuffer, NULL) ;
	    //BOOL bQuery = InternetQueryDataAvailable(hHttpFile,&dwLengthSizeBuffer,0,0);
        if(bQuery==TRUE)
        {
			dwFileSize=atol(szSizeBuffer);                                                          
            dwFileSize > BUF_LEN ? dwFileSize = BUF_LEN :
			dwFileSize = dwFileSize;
 
            BOOL bRead = InternetReadFile(hHttpFile, szReadBuffer,dwFileSize, &dwBytesRead);
            if (bRead)
            {
				
				if((bRead == TRUE)&&(dwFileSize !=dwBytesRead))
				{			
					iRet = ISAO_ERR_RECONNECT;
				}
		
				
            }
		   else iRet = ISAO_ERR_RECONNECT;
            InternetCloseHandle(hHttpFile);
		}
	}
    else
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
    }
    if(hHttpFile) InternetCloseHandle(hHttpFile);
    if(m_Session) InternetCloseHandle(m_Session);
    return iRet;
}

int ISAOCHECK::connect_WEBdb_1( char *szWriteBuffer, char *szReadBuffer)
{
	char szSizeBuffer[32];
    DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
    DWORD dwFileSize = 0;
    DWORD dwBytesRead = 0;    
    int iRet= ISAO_ERR_WINSOCK_NOERR;

    ZeroMemory(szReadBuffer,BUF_LEN);
    //int i;
 
    HINTERNET m_Session = NULL;
    m_Session = InternetOpen("AppName",  //���ø����̼��� �̸�
							INTERNET_OPEN_TYPE_PRECONFIG,  
							// Use registry settings.
                            NULL,
							// Proxy name. NULL indicates use default.
                            NULL,  
							// List of local servers. NULL indicates default.
                            0) ;
    if(!m_Session) 
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
 	HINTERNET hHttpFile = NULL;
    hHttpFile = InternetOpenUrl(m_Session, (const char *) szWriteBuffer, NULL,
								0, 0, 0);
    if (hHttpFile)
    {
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer,
					    &dwLengthSizeBuffer, NULL) ;
	    //BOOL bQuery = InternetQueryDataAvailable(hHttpFile,&dwLengthSizeBuffer,0,0);
        if(bQuery==TRUE)
        {
			dwFileSize=atol(szSizeBuffer);                                                          
            dwFileSize > BUF_LEN ? dwFileSize = BUF_LEN :
			dwFileSize = dwFileSize;
 
            BOOL bRead = InternetReadFile(hHttpFile, szReadBuffer,dwFileSize, &dwBytesRead);
            if (bRead)
            {
				if((bRead == TRUE)&&(dwFileSize !=dwBytesRead))
				{			
					iRet = ISAO_ERR_RECONNECT;
				}		
				
            }
		   else iRet = ISAO_ERR_RECONNECT;
            InternetCloseHandle(hHttpFile);
		}
	}
    else
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
    if(hHttpFile) InternetCloseHandle(hHttpFile);
    if(m_Session) InternetCloseHandle(m_Session);
    return iRet;
}
/*
int ISAOCHECK::connect_WEBdb( char *szWriteBuffer, char *szReadBuffer)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;

	//-----------------------------------------------
	// ���� ���Ǵ� TCP ���� �ۼ�
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;

	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	

	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(hostPORT );					//Ŭ�󽺳����� ���� 
	server.sin_addr.s_addr = inet_addr(hostName);		//hostPORT hostName

	//ygy : ���� �κ�
	//struct hostent* ClanHostName = gethostbyname(hostName);
	//memcpy(&server.sin_addr.s_addr , ClanHostName->h_addr_list[0],ClanHostName->h_length);
	//end ygy
	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}


   	
	//ktj : �ӽ�
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// ������ ���ð���ŭ�� �亯�� ��ٸ�
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd�� �÷��� ����
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// �亯�� �޴µ��� ��� �����.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// �����ߴ�

	}else{												// �����ߴ�
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}




	ZeroMemory(szReadBuffer,sizeof(szReadBuffer));
	long startTime, endTime ;
	time( &startTime  );

	int rcnt=0;
	while(1) {
		iLen = recv(Fd,szReadBuffer,BUF_LEN-1,0);		//���� �д´�.
		//iLen = recv(Fd,szReadBuffer,20,0);		//���� �д´�.
		
		if(iLen==0) {
			Sleep(100);
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else {					//����Ÿ�޾����Ƿ� ������.
			break;
		}

		time( &endTime);
		if( (endTime-startTime) > 2) {		//2��������ٷ��� ���̾ȿ��� �������ϵ�����.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}


	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// ���� �� �����٣������̴٣�

		
		goto IsaoCheck_Free;
	}
	szReadBuffer[iLen] = 0;
	
	//����׿����� ǥ����.
	//fd2( "\nFrom server: ---from--\n%s--end--\n ���� ���̴� %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n ��db ���� ����." );

	return iRet;
}


//ygy
int ISAOCHECK::connect_WEBdb_1( char *szWriteBuffer, char *szReadBuffer)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;

	//-----------------------------------------------
	// ���� ���Ǵ� TCP ���� �ۼ�
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;

	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	

	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(hostPORT1 );
	server.sin_addr.s_addr = inet_addr(hostName1);		//hostPORT hostName//Ŭ�󽺳����� ���� 	

	//ygy : ���� �κ�
	//struct hostent* ClanHostName = gethostbyname(hostName1);
	//memcpy(&server.sin_addr.s_addr , ClanHostName->h_addr_list[0],ClanHostName->h_length);
	//ygy

	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}


   	
	//ktj : �ӽ�
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// ������ ���ð���ŭ�� �亯�� ��ٸ�
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd�� �÷��� ����
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// �亯�� �޴µ��� ��� �����.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// �����ߴ�

	}else{												// �����ߴ�
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}


	


	long startTime, endTime ;
	time( &startTime  );

	int rcnt=0;
	while(1) {
		iLen = recv(Fd,szReadBuffer,BUF_LEN,0);		//���� �д´�.
		//iLen = recv(Fd,szReadBuffer,20,0);		//���� �д´�.
		
		if(iLen==0) {
			Sleep(100);
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else {					//����Ÿ�޾����Ƿ� ������.
			break;
		}

		time( &endTime);
		if( (endTime-startTime) > 2) {		//2��������ٷ��� ���̾ȿ��� �������ϵ�����.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}


	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// ���� �� �����٣������̴٣�

		
		goto IsaoCheck_Free;
	}
	szReadBuffer[iLen] = 0;
	
	//����׿����� ǥ����.
	//fd2( "\nFrom server: ---from--\n%s--end--\n ���� ���̴� %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n ��db ���� ����." );

	return iRet;
}
*/


//binary����Ÿ�� �д°�쿡 �����.(���ϵ�..)
int ISAOCHECK::connect_WEBdb_BINARY( char *szWriteBuffer, char *szReadBuffer, int *dataSize)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;
		
	//-----------------------------------------------
	// ���� ���Ǵ� TCP ���� �ۼ�
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;
    
	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	

	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(hostPORT1 );					//Ŭ�󽺳����� ���� 
	//server.sin_addr.s_addr = inet_addr(hostName);		//hostPORT hostName
	//server.sin_addr.s_addr = inet_addr(hostName1);		//hostPORT hostName
	//ygy
	struct hostent* ClanHostName = gethostbyname(hostName1);
	memcpy(&server.sin_addr.s_addr , ClanHostName->h_addr_list[0],ClanHostName->h_length);
	//ygy
	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}
   	
	//ktj : �ӽ�
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// ������ ���ð���ŭ�� �亯�� ��ٸ�
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd�� �÷��� ����
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// �亯�� �޴µ��� ��� �����.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// �����ߴ�

	}else{												// �����ߴ�
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}

	//ktj : ��ħ

	
	/*
	int rcnt=0;
	while(1) {
		iLen = recv(Fd,&szReadBuffer[rcnt],BUF_LEN,0);		//���� �д´�.
		rcnt += iLen;
		if(iLen==0) break;
	}
	*/

	long startTime, endTime ;
	time( ( time_t* )&startTime );

	
	int rcnt=0;

	/*
	//ktj : ����
	while(1) {
		iLen = recv(Fd,szReadBuffer,BUF_LEN,0);		//���� �д´�.

		rcnt+=iLen;
		//iLen = recv(Fd,szReadBuffer,20,0);		//���� �д´�.
		
		if(iLen==0) {
			Sleep(100);
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else {					//����Ÿ�޾����Ƿ� ������.
			if(rcnt >= 3128) break;
		}

		
		time( &endTime);
		if( (endTime-startTime) > 2) {		//2��������ٷ��� ���̾ȿ��� �������ϵ�����.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}

	*/

	
	while(1) {
		iLen = recv(Fd,&szReadBuffer[rcnt],100,0);		//���� �д´�.
		rcnt+=iLen;
		//iLen = recv(Fd,szReadBuffer,20,0);		//���� �д´�.
		if(iLen==0) {
			//Sleep(100);
			if(rcnt >= 3126)		//����Ÿ�޾����Ƿ� ������.
				break;
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else 
		{					

		}

		time( ( time_t* )&endTime );
		if( (endTime-startTime) > 10) {		//10��������ٷ��� ���̾ȿ��� �������ϵ�����.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}







	*dataSize = rcnt ;
	

	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// ���� �� �����٣������̴٣�
		
		goto IsaoCheck_Free;
	}
	//szReadBuffer[iLen] = 0;
	
	//����׿����� ǥ����.
	//fd2( "\nFrom server: ---from--\n%s--end--\n ���� ���̴� %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n ��db ���� ����." );

	return iRet;
}



/*
if( (i = send(iSock, (char*)data, l, 0)) == SOCKET_ERROR)
    //if( (i = send(iSock, (char*)cmd.data, cmd.dataLen, 0)) == SOCKET_ERROR)
    {
        error = WSAGetLastError();
        if(error==10054) {
			//ktj
            //fd(" sW Error %d: cmd.dataLen %d ret %d  closeCLIENT", error, cmd.dataLen);
            closeCLIENT();   //�ϴ� ����
        }
        if(error == 10035) {
            fdWrite = 0;            //���� �����Ѵ�.
        }
        return 0;
    }



//binary����Ÿ�� �д°�쿡 �����.(���ϵ�..)
int ISAOCHECK::connect_WEBdb_BINARY( char *szWriteBuffer, char *szReadBuffer, int *dataSize)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;
		
	//-----------------------------------------------
	// ���� ���Ǵ� TCP ���� �ۼ�
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;
    
	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	

	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(hostPORT );					//Ŭ�󽺳����� ���� 
	server.sin_addr.s_addr = inet_addr(hostName);		//hostPORT hostName
	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}
   	
	//ktj : �ӽ�
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// ������ ���ð���ŭ�� �亯�� ��ٸ�
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd�� �÷��� ����
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// �亯�� �޴µ��� ��� �����.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// �����ߴ�

	}else{												// �����ߴ�
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}

	//ktj : ��ħ
	
	int rcnt=0;
	while(1) {
		iLen = recv(Fd,&szReadBuffer[rcnt],BUF_LEN,0);		//���� �д´�.
		rcnt += iLen;
		if(iLen==0) break;
	}
	*dataSize = rcnt ;

	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// ���� �� �����٣������̴٣�
		
		goto IsaoCheck_Free;
	}
	//szReadBuffer[iLen] = 0;
	
	//����׿����� ǥ����.
	//fd2( "\nFrom server: ---from--\n%s--end--\n ���� ���̴� %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n ��db ���� ����." );

	return iRet;
}
*/




//��db���� �ǵ��ƿ½�Ʈ������ int�� ����Ÿ�� �̾Ƴ�.
int ISAOCHECK::chk_readbuffer(char *cmd, char *readbuf, int *iRejectC )
{
	int iRejectCode = 111111;			//1�� ����� ���°���.
	char *pBuf;

	//pBuf = strstr(readbuf,"account=");
	pBuf = strstr(readbuf,cmd);
    if( pBuf == NULL)	//�߰��� �� ������.
	{
		return ISAO_ERR_NONE_REEJCT;
	}

	//pBuf = pBuf + strlen( "account:" );
	pBuf = pBuf + strlen( cmd );
	while(1) {			//�����̽��� ���ش�.
		if(*pBuf != ' ') break;
		pBuf ++;
	}
	sscanf( pBuf , "%d", &iRejectCode );

	return iRejectCode;
}


//�� db���� �� ��Ʈ������ ���ϴ� ��Ʈ���� �̾Ƴ�.
int ISAOCHECK::chk_readStrBuf(char *cmd, char *readbuf, char *retBuf)
{
	int iRejectCode = 111111;			//1�� ����� ���°���.
	char *pBuf;

	//pBuf = strstr(readbuf,"account=");
	pBuf = strstr(readbuf,cmd);
    if( pBuf == NULL)	//�߰��� �� ������.
	{
		return ISAO_ERR_NONE_REEJCT;
	}

	//pBuf = pBuf + strlen( "account:" );
	pBuf = pBuf + strlen( cmd );
	while(1) {			//�����̽��� ���ش�.
		if(*pBuf != ' ') break;
		pBuf ++;
	}
	sscanf( pBuf , "%s", retBuf );

	return iRejectCode;
}









/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int IsaoCheck()
: �� �� �� :  id�� pwd�� ��ȿ���θ� �˾Ƴ��� �Լ���
: ��    �� :  ������ �׼����� �������ӽ� id,pwd���ִ� ��db������ �� �˾Ƴ���.
: ��    �� :  
: �� ȯ �� :
: ��    �� :  ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::IsaoCheck( char *id, char *pass, char *ipaddr, long StartTime )
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	
	sprintf( szWriteBuffer,
		//"POST http://%s/login.php?UserID=%s&PassWord=%s\r\n\r\n\r\n", hostName, id,pass );
		"http://%s/login.php?UserID=%s&PassWord=%s", hostName, id,pass );
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
	
	//ktj : �ӽ�
	fd2(" �� db���� ������ %s ", szReadBuffer);

	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	
	ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 0: //���̵�, �н����� ��Ȯ	
		//ret_chk = LoginCheck(id,StartTime);			//���� ���� 13�̸� Login ���̺� ��� 14 �̸� ��� ����
		sprintf(szWriteBuffer,
			//"POST http://%s/LoginCheck.php?UserID=%s&StartTime=%d\r\n\r\n\r\n",hostName, id, StartTime );
			"http://%s/LoginCheck.php?UserID=%s&StartTime=%d", hostName, id, StartTime );
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 13:
			goto IsaoCheck_Free;
		case 14:
			goto IsaoCheck_Free;
		}
		goto IsaoCheck_Free;
	case 1: //�н����� �ҷ�		
		goto IsaoCheck_Free;
	case 2: //���̵� ��ϵ��� �ʾ���
		 //ret_chk = InsertUser(id,pass,"aaaa");
		 sprintf(szWriteBuffer,
			//"POST http://%s/InsertUser.php?UserID=%s&PassWord=%s&UserName=%s\r\n\r\n\r\n",hostName, id,pass,"aaaa");
			"http://%s/InsertUser.php?UserID=%s&PassWord=%s&UserName=%s",hostName, id,pass,"aaaa");
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );

		switch(ret_chk)
		{
		case 3: //��� ��Ȯ 
			//ret_chk = LoginCheck(id,12);		//���� ���� 5�̸� Login ���̺� ��� 6 �̸� ��� ����
			sprintf(szWriteBuffer,
				//"POST http://%s/LoginCheck.php?UserID=%s&StartTime=%d\r\n\r\n\r\n",hostName, id, StartTime );
			"http://%s/LoginCheck.php?UserID=%s&StartTime=%d",hostName, id, StartTime );
			ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
			ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		    switch(ret_chk)
			{
		    case 5:
			    goto IsaoCheck_Free;
		    case 6:
			    goto IsaoCheck_Free;
			}
			goto IsaoCheck_Free;
		case 4: //��Ͻ���
			goto IsaoCheck_Free;
		 }	
		goto IsaoCheck_Free;


	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}








/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int IsaoCheck2()
: �� �� �� :  ������ ������ ������ ����
: ��    �� :  
: ��    �� :
: �� ȯ �� :
: ��    �� :
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
//���ݼ����� id, ip, �����ϰ� �ִ� �ʸ� �Ѱ��ش�.
int ISAOCHECK::IsaoCheck_GameExit(char *id, char *pass, char *ipaddr, long usertime)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	
	
	sprintf( szWriteBuffer, 
	//"POST http://%s/Login_End.php?UserID=%s&IP=%s&EndTime=%d&UseTime=%d\r\n\r\n\r\n",hostName, id,ipaddr,iTimeOut,usertime);
	"http://%s/Login_End.php?UserID=%s&IP=%s&EndTime=%d&UseTime=%d",hostName, id,ipaddr,iTimeOut,usertime);
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);

	//ktj : �ӽ�
	fd2(" �� db���� ������ %s ", szReadBuffer);
	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 7: //���� ���� 7�̸� Log���̺�(���� ���̺�)�� ��� �����ϸ鼭 Login ���̺� �ִ� �����ʹ� ����
		goto IsaoCheck_Free;

	case 8: //���� ���� 8�̸� Log���̺�(���� ���̺�)�� ��� ����
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:

	return ret_chk ;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}

















//ygy
/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int isCheckClanJang()
: �� �� �� :  ������ Ŭ�������� ��ϵǾ� �ִ��� üũ(100:Ŭ�����, 101:Ŭ����  102:Ŭ������ �ƴϴ�)
: ��    �� :  
: ��    �� :  id, ���Ӽ���
: �� ȯ �� :
: ��    �� :  ���� ����Լ� 
: ��    �� :  100,101�� Ŭ���̸��� ������.
: PROJECT  :  tjboyh/isClanMember.php�� ������.
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::isCheckClanJang(char *ClanZang_id, char* gserver)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	//�Ϻ�Ŭ��
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isCheckClanJang.asp?userid=%s&gserver=%s&ticket=%d", hostName,ClanZang_id,gserver,nClanTicket);	
	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}
	
	//ktj : �ӽ�
	
	debug_fd2( "isCheckClanJang() ������ ���� : %d(ret_chk %d) ���� : %s ", strlen(szWriteBuffer), ret_chk, szWriteBuffer);
	debug_fd2( "isCheckClanJang() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	



	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //Ŭ���� ��ϵǾ� ���� ����
			break;
		case 1: //Ŭ���� ���
			break;
		case 5:
			break;
		
		/*
		case 100:	//Ŭ����
		case 101:	//Ŭ����(�Ἲ�ɻ�Ϸ���)
		case 102:	//Ŭ�������� ���Ե����� ����
		case 103:	//�Ἲ�ɻ����̸� 10���� �������
		case 104:	//�Ἲ�ɻ����̸� 10���� ���������
		case 105:	//���ӿ���
		*/			
			break;
			

		case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
			fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
			goto IsaoCheck_Free;
		}



		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}

/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int isCheckClanName()
: �� �� �� :  Ŭ�� �̸��� �ߺ��Ǿ����� üũ
: ��    �� :  
: ��    �� :  id, ���Ӽ���
: �� ȯ �� :
: ��    �� :  ���� ����Լ� 
: ��    �� :  
: PROJECT  :  
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::isCheckClanName(char *ClanName,char *gserver)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];

/*	
//�Ϻ�Ŭ���̸��˻�
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	MBCSToWeb(ClanName,				UTF8_ClanName,932);

	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isCheckClanName.asp?ClName=%s&gserver=%s&ticket=%d", hostName, 
	UTF8_ClanName,
	gserver,nClanTicket);

#else
*/	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isCheckClanName.asp?ClName=%s&gserver=%s&ticket=%d", hostName, ClanName,gserver,nClanTicket);
//#endif
	

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//ktj : �ӽ�
	

	debug_fd2( "isCheckClanName() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "isCheckClanName() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	

	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //Ŭ���� ��ϵǾ� ���� ����
			break;
		case 1: //Ŭ���� ���
			break;
		
		/*
		case 100:	//Ŭ����
		case 101:	//Ŭ����(�Ἲ�ɻ�Ϸ���)
		case 102:	//Ŭ�������� ���Ե����� ����
		case 103:	//�Ἲ�ɻ����̸� 10���� �������
		case 104:	//�Ἲ�ɻ����̸� 10���� ���������
		case 105:	//���ӿ���
		*/				

		case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
			fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
			goto IsaoCheck_Free;
		}



		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}


/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int ReadClanInfo(char *clanNum)
: �� �� �� :  Ŭ����ũ ��ȣ�� Ŭ�� ���� �о����
: ��    �� :  
: ��    �� :  clannum
: �� ȯ �� :
: ��    �� :  ���� ����Լ� 
: ��    �� :  
: PROJECT  :  
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::ReadClanInfo(char *clanNum,char* ret_Buf)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	//�Ϻ�Ŭ����ȣ
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isCheckClanNum.asp?num=%s&ticket=%d", hostName,clanNum,nClanTicket);
	
	

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//ktj : �ӽ�
	
	debug_fd2( "ReadClanInfo() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "ReadClanInfo() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	


	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //Ŭ���� ��ϵǾ� ���� ����
			break;
		case 1: //Ŭ�� ���� �о���� ����
			strcpy( ret_Buf, szReadBuffer);
			break;			

		case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
			fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
			goto IsaoCheck_Free;
		}



		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}



//ȣ��Clan�߰�
int ISAOCHECK::isCheckClanMember(char *pszServerName, char *pszUserName)
{
	int		iRejectCode = 111111;
	char	szWriteBuffer[BUF_LEN];
	char	szReadBuffer[BUF_LEN];
	int		receiveCode;

	ZeroMemory(szReadBuffer, sizeof(szReadBuffer));
/*
//�Ϻ�Ŭ������˻�
#ifdef _LANGUAGE_JAPANESE

	char UTF8_pszUserName[1024];		memset(UTF8_pszUserName,0,sizeof(UTF8_pszUserName));
	MBCSToWeb(pszUserName,		UTF8_pszUserName,932);

	
	sprintf(szWriteBuffer,
	"http://%s/ServerMain/isCheckClanwon.asp?gserver=%s&clwon=%s",hostName,
	pszServerName,UTF8_pszUserName);

#else
*/
	sprintf(szWriteBuffer,
	"http://%s/ServerMain/isCheckClanwon.asp?gserver=%s&clwon=%s",hostName,
	pszServerName,pszUserName);
//#endif
	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		receiveCode = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(receiveCode != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}
	
	if(strlen(szReadBuffer) > 2048) {
		szReadBuffer[2048] = 0;
		debug_fd2( "isClanMember() �ʹ��� �߶���.");
	}

	switch(receiveCode)
	{
		case ISAO_ERR_WINSOCK_NOERR:
		{
			receiveCode = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
			if(receiveCode == ISAO_ERR_NONE_REEJCT)
			{
				fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
			}
		}

		case ISAO_ERR_WINSOCK_START:		//���� Start ���� �߻�
		{
			fd2("WSAStartup()����");
			break;
		}
		case ISAO_ERR_WINSOCK_VERSION:		//���� ������ Ʋ��
		{
			fd2("������ ���� ����");
			break;
		}

		case ISAO_ERR_TCP_CONNECT:			//���� ����
		{
			fd2( "connect() ���� ����" );
			break;
		}
		case ISAO_ERR_SSL_READ:				//Read ����
		{
			fd2(" recv()  �б� ����" );
			break;
		}
		case ISAO_ERR_SSL_TIMEOUT:			//TimeOut
		{
			fd2("---------------- select() Ÿ�Ӿƿ� ����" );
			break;
		}
	}
	
	return receiveCode;
}

/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int isClanMember()
: �� �� �� :  Ŭ�������������(100:Ŭ�����, 101:Ŭ����  102:Ŭ������ �ƴϴ�)
: ��    �� :  
: ��    �� :  id, Ŭ���̸����� ����
: �� ȯ �� :
: ��    �� :  ���� ����Լ� 
: ��    �� :  100,101�� Ŭ���̸��� ������.
: PROJECT  :  tjboyh/isClanMember.php�� ������.
///////////////////////////////////////////////////////////////////////////*/

//ktj : isClanMember.php: �� �����־����.
int ISAOCHECK::isClanMember(char *ClanZang_id, char* gserver,char* ClanZang_ChaName,char* ret_Buf)
{

	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	ZeroMemory(szReadBuffer , sizeof(szReadBuffer ));
/*
//�Ϻ�Ŭ������˻�	
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);

	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isClanMember.asp?userid=%s&gserver=%s&chname=%s&ticket=%d", 
	hostName,ClanZang_id,gserver,
	UTF8_ClanZang_ChaName,
	nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/isClanMember.asp?userid=%s&gserver=%s&chname=%s&ticket=%d", hostName,ClanZang_id,gserver,ClanZang_ChaName,nClanTicket);
/*		"http://pristonclan.hangame.com/ServerMain/isClanMember.asp?userid=������&gserver=������&chname=��������&ticket=411");*/ // ����Ŭ��
//#endif
	

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//for(cnt=0; cnt<BUF_LEN;cnt++) {
	//	szReadBuffer[cnt] = 'a';
	//}
	if(strlen(szReadBuffer) > 2048) {
		szReadBuffer[2048] = 0;
		debug_fd2( "isClanMember() �ʹ��� �߶���.");
	}
	//ktj : �ӽ�
	
	debug_fd2( "isClanMember() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "isClanMember() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	

	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //Ŭ���� ��ϵǾ� ���� ����
			strcpy( ret_Buf, szReadBuffer);
			break;
		case 1: //Ŭ���� ���
//			break;
		case 5:
		case 4:
		case 3:
		case 2: //Ŭ���� ���
			strcpy( ret_Buf, szReadBuffer);
			break;
		/*
		case 100:	//Ŭ����
		case 101:	//Ŭ����(�Ἲ�ɻ�Ϸ���)
		case 102:	//Ŭ�������� ���Ե����� ����
		case 103:	//�Ἲ�ɻ����̸� 10���� �������
		case 104:	//�Ἲ�ɻ����̸� 10���� ���������
		case 105:	//���ӿ���
		*/

			strcpy( ret_Buf, szReadBuffer);
			break;
			

		case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
			fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
			goto IsaoCheck_Free;
		}



		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}




/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int joinClanMember()
: �� �� �� :  Ŭ���� ����� ���
: ��    �� :  
: ��    �� :  Ŭ���� id, Ŭ������ ĳ���� id
: �� ȯ �� :
: ��    �� :  ���� ����Լ� 
: ��    �� :  
: PROJECT  : 
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::joinClanMember(char *ClanZang_ID, char *gserver, char *ClanZang_ChaName,char* ClanName,char* ClanWon,char* ClanWon_ID,int level,DWORD chtype,int joinlevel,char *chipflag)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//�Ϻ�Ŭ������
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	char UTF8_ClanWon[1024];			memset(UTF8_ClanWon,0,sizeof(UTF8_ClanWon));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);
	MBCSToWeb(ClanName,				UTF8_ClanName,932);
	MBCSToWeb(ClanWon,				UTF8_ClanWon,932);

	
	sprintf( szWriteBuffer,
			"http://%s/ServerMain/clanInsertClanWon.asp?userid=%s&gserver=%s&chname=%s&clName=%s&clwon=%s&clwonUserid=%s&lv=%d&ticket=%d&chtype=%d&chlv=%d&chipflag=%s", 
			hostName,
			ClanZang_ID,
			gserver,
				UTF8_ClanZang_ChaName,
				UTF8_ClanName,
				UTF8_ClanWon,
			ClanWon_ID,level,nClanTicket,chtype,joinlevel,chipflag);

#else
*/
	sprintf( szWriteBuffer,
			"http://%s/ServerMain/clanInsertClanWon.asp?userid=%s&gserver=%s&chname=%s&clName=%s&clwon=%s&clwonUserid=%s&lv=%d&ticket=%d&chtype=%d&chlv=%d&chipflag=%s", 
			hostName,ClanZang_ID,gserver,ClanZang_ChaName,ClanName,ClanWon,ClanWon_ID,level,nClanTicket,chtype,joinlevel,chipflag);
//#endif



	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//ktj : �ӽ�
	
	debug_fd2( "joinClanMember() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "joinClanMember() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	


	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			

		ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
		switch(ret_chk)
		{
		case 0: //Ʈ����� ����
			break;
		case 1: //Ŭ���� ���� ����
			break;
		case 2: //���� ���Խ�Ű���� Ŭ������ �̹� �ٸ� Ŭ���� ���������� ���(�ڱ� Ŭ������)
			break;
		case 3: //Ŭ���� �������� �ʰų� Ŭ�� ���� �ƴѰ��
			break;
		case 4: //Ŭ�� ����� 32 �̻��� ���(Max : 32)
			break;
		case 100:	//����
		case 101:	//Ŭ���̸� �������.
		case 102:	//Ŭ�����߰�����
		case 103:	//
			break;

		case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
			fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
			goto IsaoCheck_Free;
		}


		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}




/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int makeClan()
: �� �� �� :  Ŭ���� ���θ������� �Լ�
: ��    �� :  
: ��    �� :  Ŭ���̸�, ����, Ŭ������ id�� �ʿ���.
: �� ȯ �� :
: ��    �� :  ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::makeClan(char *ClanZang_ID, char* gserver,char* ClanZang_ChaName,char* ClanName,char *explanation,DWORD chtype,int level,char* data)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];

	int chkCnt=0;
	

//�Ϻ�Ŭ������	
 #ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	char UTF8_explanation[1024];			memset(UTF8_explanation,0,sizeof(UTF8_explanation));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);
	MBCSToWeb(ClanName,				UTF8_ClanName,932);
	MBCSToWeb(explanation,			UTF8_explanation,932);
	
	sprintf	//�̺κи� ���ܳ���
	(
	szWriteBuffer,
	"http://%s/Clan/clanInsert.aspx?userid=%s&gserver=%s&chname=%s&clName=%s&expl=%s&ticket=%d&chtype=%d&lv=%d",
	hostName1,ClanZang_ID, gserver,
		UTF8_ClanZang_ChaName,
		UTF8_ClanName,
		UTF8_explanation,
		nClanTicket,chtype,level 
	);

 #else  

	sprintf
	(
	szWriteBuffer,
	"http://%s/Clan/clanInsert.aspx?userid=%s&gserver=%s&chname=%s&clName=%s&expl=%s&ticket=%d&chtype=%d&lv=%d",
	hostName1,ClanZang_ID, gserver,ClanZang_ChaName,ClanName,explanation,nClanTicket,chtype,level 
	);

#endif

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb_1( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//ktj : �ӽ�
	
	debug_fd2( "makeClan() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "makeClan() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	



	
	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy
	case 0: //Ʈ����� ����
		break;
	case 1: //Ŭ���� ���Լ���
		strcpy( data, szReadBuffer);
		break;
	case 2: //���� ���Խ�Ű���� Ŭ������ �̹� �ٸ� Ŭ���� ���������� ���(�ڱ� Ŭ������)
		break;
	case 3: //Ŭ���� �������� �ʰų� Ŭ�� ���� �ƴѰ��	
		break;
	case 4: //Ŭ�� ����� 32 �̻��� ���(Max : 32)
		break;
	case 100:
		break;
		/*
	case 0: 
		goto IsaoCheck_Free;
	case 1: //�н����� �ҷ�		
		goto IsaoCheck_Free;
	case 2: //���̵� ��ϵ��� �ʾ���
		goto IsaoCheck_Free;*/

	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}




/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int ISAOCHECK::read_ClanName(int readClumns)
: �� �� �� :
: ��    �� : �÷��� ������ �ش� ����Ÿ ����
: ��    �� :
: �� ȯ �� :
: ��    �� :  ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::read_ClanName(int readClumns)
{
	char *clumns[] = {
		"name",
		"explanation",
		"date",
		"Image",
		"clanZang",
		"clanWon1",
		"clanWon2",
		"clanWon3",
		"clanWon4",
		"clanWon5",
		"clanWon6",
	};

	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	sprintf( szWriteBuffer,
		//"POST http://%s/tjboyh/clan_readClanName.php?columnName=%s&ticket=%d\r\n\r\n\r\n", hostName, clumns[readClumns] ,nClanTicket);
		"http://%s/tjboyh/clan_readClanName.php?columnName=%s&ticket=%d", hostName, clumns[readClumns] ,nClanTicket);
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
	
	//ktj : �ӽ�
	fd2(" �� db���� ������ %s ", szReadBuffer);


	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	ret_chk = chk_readbuffer("account:", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 0: 
		goto IsaoCheck_Free;
	case 1: //�н����� �ҷ�		
		goto IsaoCheck_Free;
	case 2: //���̵� ��ϵ��� �ʾ���
		goto IsaoCheck_Free;

	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}






/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int ISAOCHECK::read_ClanName(int readClumns)
: �� �� �� :
: ��    �� : Ŭ���̾�Ʈ �̸��� �־ �� Į�� ���
: ��    �� :
: �� ȯ �� :
: ��    �� :  ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::read_ClanData(char *ClanZang_ID,char* gserver,char* ClanZang_ChaName,char* ret_Buf)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//�Ϻ�Ŭ�� Ŭ�������κ�����
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);

	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/GetAllMyClanMember.asp?userid=%s&gserver=%s&chname=%s&ticket=%d", 
		hostName,ClanZang_ID,gserver,
		UTF8_ClanZang_ChaName,
		nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/GetAllMyClanMember.asp?userid=%s&gserver=%s&chname=%s&ticket=%d", hostName,ClanZang_ID,gserver,ClanZang_ChaName,nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	
	debug_fd2( "read_ClanData() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "read_ClanData() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	

	//fd2(" �� db���� ������ %s ", szReadBuffer);
	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy
	case 0: //������ Ŭ���� ����
		break;
	case 1://���� ���
		strcpy( ret_Buf, szReadBuffer);
		break;
	case 100:					//ok
		strcpy( ret_Buf, szReadBuffer);
	case 101:					//������.
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}



/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int ISAOCHECK::BreakUPClan()
: �� �� �� :
: ��    �� : Ŭ���� ������.
: ��    �� :
: �� ȯ �� :
: ��    �� : ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::BreakUPClan(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName, char *ClanName)
{
	//return -1;		//��������.

	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//�Ϻ�Ŭ������
#ifdef _LANGUAGE_JAPANESE
	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	char UTF8_explanation[1024];			memset(UTF8_explanation,0,sizeof(UTF8_explanation));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);
	MBCSToWeb(ClanName,				UTF8_ClanName,932);
	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/clanRemove.asp?userid=%s&gserver=%s&chname=%s&clName=%s&ticket=%d",
	hostName,ClanZang_ID,gserver,
		UTF8_ClanZang_ChaName, 
		UTF8_ClanName,
	nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/clanRemove.asp?userid=%s&gserver=%s&chname=%s&clName=%s&ticket=%d",
	hostName,ClanZang_ID,gserver,ClanZang_ChaName, ClanName,nClanTicket);
//#endif

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	fd2(" �� db���� ������ %s ", szReadBuffer);
	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 0: //Ʈ����� ����
		break;
	case 1: //���� ����
		break;
	case 2: //���� ĳ������ ���� Ŭ���� ���� �ƴϰų� �ش�Ŭ���� ����
		break;
	/*case 100:					//����
	case 101:					//clanName ���̺� Ŭ������ �ִµ� clanWon���̺� ����.
	case 102:					//clanWon���̺��� Ŭ�������� ����
	case 103:					//clanName�ȿ� Ŭ���̸� �������.
	case 104:					//clanName���̺��� Ŭ������ ����
		goto IsaoCheck_Free;
		*/
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}


/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int ISAOCHECK::ReadNotice()
: �� �� �� :
: ��    �� : Ŭ������ �߹��Ѵ�.
: ��    �� :
: �� ȯ �� :
: ��    �� : ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/

extern char szNoticeUrl[255];


/*///////////////////////////////////////////////////////////////////////////
//notice����
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::connect_WEBdb_NOTICE(char *szWriteBuffer, char *szReadBuffer)
{
		char szSizeBuffer[32];
    DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
    DWORD dwFileSize = 0;
    DWORD dwBytesRead = 0;    
    int iRet= ISAO_ERR_WINSOCK_NOERR;

    ZeroMemory(szReadBuffer,BUF_LEN);
    //int i;
 
    HINTERNET m_Session = NULL;
    m_Session = InternetOpen("AppName",  //���ø����̼��� �̸�
							INTERNET_OPEN_TYPE_PRECONFIG,  
							// Use registry settings.
                            NULL,
							// Proxy name. NULL indicates use default.
                            NULL,  
							// List of local servers. NULL indicates default.
                            0) ;
    if(!m_Session) 
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
 	HINTERNET hHttpFile = NULL;
    hHttpFile = InternetOpenUrl(m_Session, (const char *) szWriteBuffer, NULL,
								0, 0, 0);
    if (hHttpFile)
    {
		BOOL bQuery = HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer,
					    &dwLengthSizeBuffer, NULL) ;
	    //BOOL bQuery = InternetQueryDataAvailable(hHttpFile,&dwLengthSizeBuffer,0,0);
        if(bQuery==TRUE)
        {
			dwFileSize=atol(szSizeBuffer);                                                          
            dwFileSize > BUF_LEN ? dwFileSize = BUF_LEN :
			dwFileSize = dwFileSize;
 
            BOOL bRead = InternetReadFile(hHttpFile, szReadBuffer,dwFileSize, &dwBytesRead);
            if (bRead)
            {
				if((bRead == TRUE)&&(dwFileSize !=dwBytesRead))
				{			
					iRet = ISAO_ERR_RECONNECT;
				}
		
				
            }
		   else iRet = ISAO_ERR_RECONNECT;
		}
	}
    else
    {
	    iRet = ISAO_ERR_TCP_CONNECT;
	    return iRet;
    }
    if(hHttpFile) InternetCloseHandle(hHttpFile);
    if(m_Session) InternetCloseHandle(m_Session);
    return iRet;
}
/*
int ISAOCHECK::connect_WEBdb_NOTICE(char *szServerDomain, int port, char *szWriteBuffer, char *szReadBuffer)
{
	struct timeval tmv;    //timeval
	struct sockaddr_in server;
	char Value = '\0';
	char Msg[1024] = "";
	SOCKET Fd;
	
	int iLen;
	int iRet= ISAO_ERR_WINSOCK_NOERR;
	int iSelectRet;
	
    fd_set fdset;

	//-----------------------------------------------
	// ���� ���Ǵ� TCP ���� �ۼ�
	//-----------------------------------------------
	WORD         wSocketVersion;
	WSADATA      wsaData;
    int          iError;

	wSocketVersion = MAKEWORD(1,1);
	iError = WSAStartup(wSocketVersion,&wsaData);
	if(iError != 0)
	{
		return ISAO_ERR_WINSOCK_START;
	}
	if(HIBYTE(wsaData.wVersion) != 1 || LOBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return ISAO_ERR_WINSOCK_VERSION;
	}	



	
	PHOSTENT phe;
	char szIP[16];
	in_addr  ia;
	int cnt,len;
	// �������� IP�ּ����� Ȯ��
	len = lstrlen(szServerDomain);
	for(cnt=0;cnt<len;cnt++) {
		if ( szServerDomain[cnt]!='.' && (szServerDomain[cnt]<'0' || szServerDomain[cnt]>'9') ) break;
	}
	if ( cnt<len ) {
		//DNS ���ϱ�
		phe = gethostbyname( szServerDomain );
		if ( phe ) {
			ia.S_un.S_addr = ((unsigned long *)phe->h_addr)[0];
			lstrcpy( szIP , inet_ntoa (ia) );
		}
		else
			return FALSE;
	}
	else {
		//�������� IP�� Ȯ��
		lstrcpy( szIP , szServerDomain );
	}



	Fd = socket (AF_INET,SOCK_STREAM,0);
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons( port);					//Ŭ�󽺳����� ���� 
	//server.sin_addr.s_addr = inet_addr(severIP);		//hostPORT hostName
	server.sin_addr.S_un.S_addr = inet_addr ( szIP );

	//connect
	if (connect( Fd, (struct sockaddr *)&server,sizeof(server)) == -1){
		return ISAO_ERR_TCP_CONNECT;
	}


   	
	//ktj : �ӽ�
	//fd2( "send( %s )", szWriteBuffer  );
	send(Fd,szWriteBuffer,strlen(szWriteBuffer)+1,0);

	tmv.tv_sec = iTimeOut;		// ������ ���ð���ŭ�� �亯�� ��ٸ�
	tmv.tv_usec = 0;
	FD_ZERO( &fdset );			// read select
    FD_SET( Fd, &fdset );		// fd�� �÷��� ����
	iSelectRet = select( Fd + 1, &fdset, (fd_set*)NULL,(fd_set*)NULL,&tmv);		// �亯�� �޴µ��� ��� �����.

	if( iSelectRet > 0 && FD_ISSET( Fd, &fdset ) ){		// �����ߴ�

	}else{												// �����ߴ�
		iRet = ISAO_ERR_SSL_TIMEOUT;
		goto IsaoCheck_Free;
	}




	ZeroMemory(szReadBuffer,sizeof(szReadBuffer));
	long startTime, endTime ;
	time( &startTime  );

	int rcnt=0;
	while(1) {
		iLen = recv(Fd,szReadBuffer,BUF_LEN-1,0);		//���� �д´�.
		//iLen = recv(Fd,szReadBuffer,20,0);		//���� �д´�.
		
		if(iLen==0) {
			Sleep(100);
		}
		else if(iLen == SOCKET_ERROR)				// error
		{
	        int error = WSAGetLastError();
			if(error==10054)    {
				iRet = ISAO_ERR_SSL_TIMEOUT;
				goto IsaoCheck_Free;
			}
		}
		else {					//����Ÿ�޾����Ƿ� ������.
			break;
		}

		time( &endTime);
		if( (endTime-startTime) > 2) {		//2��������ٷ��� ���̾ȿ��� �������ϵ�����.
			iRet = ISAO_ERR_RECONNECT;
			goto IsaoCheck_Free;
		}
	}


	if( iLen < 0 ){
		iRet = ISAO_ERR_SSL_READ;				// ���� �� �����٣������̴٣�

		
		goto IsaoCheck_Free;
	}
	szReadBuffer[iLen] = 0;
	
	//����׿����� ǥ����.
	//fd2( "\nFrom server: ---from--\n%s--end--\n ���� ���̴� %d ",szReadBuffer, iLen );
	
IsaoCheck_Free:

	closesocket( Fd );
	fd2("\n ��db ���� ����." );

	return iRet;
}
*/

int ConvertURL( char *szURL , char *szServer , char *szPath )
{
	int  cnt,len;
	char *lpServer=0;
	char *lpPath=0;
	char szbuff[256];

	lstrcpy( szbuff, szURL );

	len = lstrlen( szbuff );

	for(cnt=0;cnt<len;cnt++) {
		//(http://)
		if( !lpServer && szbuff[cnt]=='/' && szbuff[cnt+1]=='/' ) {
			lpServer = &szbuff[cnt+2];
			cnt+=2;
		}

		if ( !lpPath && szbuff[cnt]=='/' && szbuff[cnt+1]!='/' ) {
			lpPath = &szbuff[cnt];
		}
	}

	if ( lpPath ) {
		if ( szPath ) 
			lstrcpy( szPath , lpPath );
		lpPath[0] = 0;
	}
	else
		if ( szPath ) szPath[0]=0;

	if ( szServer ) {
		if ( lpServer ) {
			lstrcpy( szServer , lpServer );
		}
		else {
			lstrcpy( szServer , szbuff );
		}
	}

	return TRUE;
}


int ISAOCHECK::ReadNotice(char *userid,char *chaname,char *gserver,char *buf)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];


	char szServer[256], szPath[256];
	ConvertURL( szNoticeUrl, szServer , szPath );

	sprintf( szWriteBuffer,
		"%s?userid=%s&gserver=%s&chname=%s&ticket=%d\r\n\r\n\r\n", szNoticeUrl,userid,gserver,chaname,nClanTicket);

	//sprintf( szWriteBuffer,
	//	"POST %s?userid=%s&gserver=%s&chname=%s\r\n\r\n\r\n", szNoticeUrl, "yuri", "�׽�Ʈ����", "����");

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		//ret_chk = connect_WEBdb_NOTICE( szServer, 80, szWriteBuffer, szReadBuffer);
		ret_chk = connect_WEBdb_NOTICE(szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;		
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	
	debug_fd2( "ReadNotice() ������ ���� : %d(ret_chk %d) ���� : %s ", strlen(szWriteBuffer), ret_chk, szWriteBuffer);
	debug_fd2( "ReadNotice() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	



	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	
	//for(int index = 0; index < 10; index++)
		//strcat(szReadBuffer, "skjdflsdklfjlksdjf\n");

	switch(ret_chk)
	{
		//ygy
	case 0: //Ʈ����� ����
		break;
	case 1:
		strcpy( buf, szReadBuffer);
		break;
	case 2: 
		break;
	case 3: 
		break;
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� �߹� ���� ������Ʈ ����
	case 103:					//clanWon�� ���� ����
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}
/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int ISAOCHECK::Banishiment()
: �� �� �� :
: ��    �� : Ŭ������ �߹��Ѵ�.
: ��    �� :
: �� ȯ �� :
: ��    �� : ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::Banishiment(char *ClanZang_ID,char *gserver,char *ClanZang_ChaName,char *ClanName, char *ClanWon)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//�Ϻ�Ŭ�����߹�
#ifdef _LANGUAGE_JAPANESE

	char UTF8_ClanZang_ChaName[1024];		memset(UTF8_ClanZang_ChaName,0,sizeof(UTF8_ClanZang_ChaName));
	char UTF8_ClanName[1024];				memset(UTF8_ClanName,0,sizeof(UTF8_ClanName));
	char UTF8_ClanWon[1024];			memset(UTF8_ClanWon,0,sizeof(UTF8_ClanWon));
	
	MBCSToWeb(ClanZang_ChaName,		UTF8_ClanZang_ChaName,932);
	MBCSToWeb(ClanName,				UTF8_ClanName,932);
	MBCSToWeb(ClanWon,				UTF8_ClanWon,932);
	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/clanWonRelease.asp?userid=%s&gserver=%s&chname=%s&clName=%s&%s&ticket=%d",
	hostName,ClanZang_ID,gserver,
	UTF8_ClanZang_ChaName,
	UTF8_ClanName,
	UTF8_ClanWon,
	nClanTicket);
#else
*/
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/clanWonRelease.asp?userid=%s&gserver=%s&chname=%s&clName=%s&%s&ticket=%d", hostName,ClanZang_ID,gserver,ClanZang_ChaName,ClanName,ClanWon,nClanTicket);
//#endif
		

	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	
	debug_fd2( "Banishiment() ������ ���� : %d(ret_chk %d) ���� : %s ", strlen(szWriteBuffer), ret_chk, szWriteBuffer);
	debug_fd2( "Banishiment() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	



	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy
	case 0: //Ʈ����� ����
		break;
	case 1: //Ŭ���� Ż�� ����
		break;
	case 2: //Ŭ���� �������� �ʰų� Ŭ�� ���� �ƴѰ�� �Ǵ� Ŭ������ �ƴѰ��
		break;
	case 3: //Ż��� ���������� Ŭ��ȸ������ 6�����Ϸ� �������� ���(LimitDate ����)
		break;
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� �߹� ���� ������Ʈ ����
	case 103:					//clanWon�� ���� ����
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}





//��Ŭ���� �Ӹ�==================================================================
int ISAOCHECK::AppointSubChip(char *gserver,char* clanwon)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];

/*
//�Ϻ�Ŭ����Ŭ�����Ӹ�
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clanwon[1024];		memset(UTF8_clanwon,0,sizeof(UTF8_clanwon));
	MBCSToWeb(clanwon,		UTF8_clanwon,932);

	
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/ClanSubChipUpdate.asp?chname=%s&gserver=%s&ticket=%d", hostName,
	UTF8_clanwon,
	gserver,9999);//nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
	"http://%s/ServerMain/ClanSubChipUpdate.asp?chname=%s&gserver=%s&ticket=%d", hostName,clanwon,gserver,9999);//nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	debug_fd2( "AppointSubChip() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "AppointSubChip() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);


	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	}
	//winsock ���� ���� ýũ��.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy	
	case 0:  //Ŭ������ �ƴ�
		break;
	case 2:	//Ŭ��Ĩ�϶�
		break;
	case 1:	//��Ŭ��Ĩ �Ӹ� ����
		break;	
	case 3: //��Ŭ��Ĩ�� ������
		break;
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� �߹� ���� ������Ʈ ����
	case 103:					//clanWon�� ���� ����
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}

//��Ŭ���� ����==================================================================
int ISAOCHECK::ReleaseSubChip(char *gserver,char* clanwon)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];

/*
//�Ϻ�Ŭ����Ŭ��������
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clanwon[1024];		memset(UTF8_clanwon,0,sizeof(UTF8_clanwon));
	MBCSToWeb(clanwon,		UTF8_clanwon,932);

	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/ClanSubChipRelease.asp?chname=%s&gserver=%s&ticket=%d", 
		hostName,
		UTF8_clanwon,
		gserver,9999);//nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/ClanSubChipRelease.asp?chname=%s&gserver=%s&ticket=%d", hostName,clanwon,gserver,9999);//nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	debug_fd2( "ReleaseSubChip() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "ReleaseSubChip() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);


	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	}
	//winsock ���� ���� ýũ��.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy	
	case 0: //Ŭ������ �ƴ�
		break;
	case 2: //Ŭ��Ĩ�̰ų� ��Ŭ��
		break;
	case 1: //���Ӽ���
		break;
	
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� �߹� ���� ������Ʈ ����
	case 103:					//clanWon�� ���� ����
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}

//����==============================================================
int ISAOCHECK::LeaveClan(char *clanname,char *chaname,char *gserver)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//�Ϻ�Ŭ��Ŭ��Ż��
#ifdef _LANGUAGE_JAPANESE
	char UTF8_chaname[1024];				memset(UTF8_chaname,0,sizeof(UTF8_chaname));
	char UTF8_clanname[1024];			memset(UTF8_clanname,0,sizeof(UTF8_clanname));
	

	MBCSToWeb(chaname,				UTF8_chaname,932);
	MBCSToWeb(clanname,			UTF8_clanname,932);
	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/clanChipChange.asp?chname=%s&gserver=%s&clName=%s&ticket=%d", 
		hostName,
		UTF8_chaname,
		gserver,
		UTF8_clanname,
		nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/clanChipChange.asp?chname=%s&gserver=%s&clName=%s&ticket=%d", hostName,chaname,gserver,clanname,nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	debug_fd2( "LeaveClan() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "LeaveClan() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);


	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	}
	//winsock ���� ���� ýũ��.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy	
	case 1: //���� ����
		break;
	case 2: //Ŭ������ �ƴ�
		break;
	case 3: //���������ʴ� Ŭ��
		break;
	case 4://����50�̸�
		break;
	case 5://���� �ڰݸ� ����
		break;
	case 6: //����ĳ�����̤� ���� �ִ��ο��� ���� ���� �ο����� ŭ
		break;
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� �߹� ���� ������Ʈ ����
	case 103:					//clanWon�� ���� ����
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}





/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int ISAOCHECK::Secession()
: �� �� �� :
: ��    �� : Ŭ������ Ż���Ѵ�.
: ��    �� :
: �� ȯ �� :
: ��    �� : ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::Secession(char* clanzang,char* gserver,char* SecessionID,char* clanname)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//�Ϻ�Ŭ��Ż��
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clanzang[1024];			memset(UTF8_clanzang,0,sizeof(UTF8_clanzang));
	char UTF8_SecessionID[1024];		memset(UTF8_SecessionID,0,sizeof(UTF8_SecessionID));
	char UTF8_clanname[1024];			memset(UTF8_clanname,0,sizeof(UTF8_clanname));
	
	MBCSToWeb(clanzang,			UTF8_clanzang,932);
	MBCSToWeb(SecessionID,		UTF8_SecessionID,932);
	MBCSToWeb(clanname,			UTF8_clanname,932);


	sprintf( szWriteBuffer,
		"http://%s/ServerMain/clanWonSelfLeave.asp?userid=%s&gserver=%s&chname=%s&clName=%s&ticket=%d",
		hostName,
		UTF8_clanzang,
		gserver,
		UTF8_SecessionID,
		UTF8_clanname,
		nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/clanWonSelfLeave.asp?userid=%s&gserver=%s&chname=%s&clName=%s&ticket=%d",
		hostName,clanzang,gserver,SecessionID,clanname,nClanTicket);
//#endif

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	fd2("Secession() �� db���� ������ %s ", szReadBuffer);
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	}
	//winsock ���� ���� ýũ��.  ================================== end

	//ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	//ygy
	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
		//ygy
	case 0: //Ʈ����� ����
		break;
	case 1: //Ŭ���� Ż�� ����
		break;
	case 2: //Ŭ���� �������� �ʰų� Ŭ�� ���� �ƴѰ�� �Ǵ� Ŭ������ �ƴѰ��
		break;
	case 3: //Ż��� ���������� Ŭ��ȸ������ 6�����Ϸ� �������� ���(LimitDate ����)
		break;
	case 4://Ż���Ϸ��� �ڰ� ���� Ŭ���� Ŭ������ ���(����)
		break;
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� �߹� ���� ������Ʈ ����
	case 103:					//clanWon�� ���� ����
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}


/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int 
: �� �� �� :
: ��    �� : 
: ��    �� :
: �� ȯ �� :
: ��    �� : ���� ���� �÷��� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::isKPFlag(char *userid,char *clanname, char *chaname,char *gserver,char *pflag, char *gunbun)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//�Ϻ�Ŭ��KPFlag
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clannamee[1024];			memset(UTF8_clannamee,0,sizeof(UTF8_clannamee));
	char UTF8_chaname[1024];			memset(UTF8_chaname,0,sizeof(UTF8_chaname));

	MBCSToWeb(clanname,			UTF8_clannamee,932);
	MBCSToWeb(chaname,			UTF8_chaname,932);
	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/isKflag.asp?userid=%s&clName=%s&chName=%s&gserver=%s&KFlag=%s&Gubun=%s&ticket=%d",
		 hostName,userid,
		 UTF8_clannamee, 
		 UTF8_chaname,
		 gserver,pflag, gunbun,nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/isKflag.asp?userid=%s&clName=%s&chName=%s&gserver=%s&KFlag=%s&Gubun=%s&ticket=%d",
		 hostName,userid,clanname, chaname,gserver,pflag, gunbun,nClanTicket);
//#endif
	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	
	debug_fd2( "isPFlag() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "isPFlag() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	

	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	}
	//winsock ���� ���� ýũ��.  ================================== end

	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� ���� ������Ʈ ����
		goto IsaoCheck_Free;
	case ISAO_ERR_TCP_CONNECT:		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}

/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int 
: �� �� �� :
: ��    �� : 
: ��    �� :
: �� ȯ �� :
: ��    �� : �ʱ�ȭ�� ���� �÷��� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::isPFlag(char *userid,char *clanname, char *chaname,char *gserver,char *pflag, char *gunbun)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
/*
//�Ϻ�Ŭ���ʱ�ȭ�÷���
#ifdef _LANGUAGE_JAPANESE

	char UTF8_clanname[1024];			memset(UTF8_clanname,0,sizeof(UTF8_clanname));
	char UTF8_chaname[1024];			memset(UTF8_chaname,0,sizeof(UTF8_chaname));
	
	MBCSToWeb(clanname,				UTF8_clanname,932);
	MBCSToWeb(chaname,				UTF8_chaname,932);

	
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/isPFlag.asp?userid=%s&clName=%s&chName=%s&gserver=%s&PFlag=%s&Gubun=%s&ticket=%d",
		 hostName,userid,
		 UTF8_clanname, 
		 UTF8_chaname,
		 gserver,pflag, gunbun,nClanTicket);

#else
*/
	sprintf( szWriteBuffer,
		"http://%s/ServerMain/isPFlag.asp?userid=%s&clName=%s&chName=%s&gserver=%s&PFlag=%s&Gubun=%s&ticket=%d",
		 hostName,userid,clanname, chaname,gserver,pflag, gunbun,nClanTicket);
//#endif
	

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	
	debug_fd2( "isPFlag() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "isPFlag() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	

	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	}
	//winsock ���� ���� ýũ��.  ================================== end

	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� ���� ������Ʈ ����
		goto IsaoCheck_Free;
	case ISAO_ERR_TCP_CONNECT:		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}





/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int 
: �� �� �� :
: ��    �� : 
: ��    �� :
: �� ȯ �� :
: ��    �� : ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::WarhouseRight(char *clname, char *right_str)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	sprintf( szWriteBuffer,
		//"POST http://%s/tjboyh/clan_WarehouseRight.php?clName=%s&whRight=%s&ticket=%d\r\n\r\n\r\n", hostName, clname,right_str ,nClanTicket);
		"http://%s/tjboyh/clan_WarehouseRight.php?clName=%s&whRight=%s&ticket=%d", hostName, clname,right_str ,nClanTicket);
	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	fd2("WarhouseRight() �� db���� ������ %s ", szReadBuffer);
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	}
	//winsock ���� ���� ýũ��.  ================================== end

	ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� ���� ������Ʈ ����
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}


/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :int 
: �� �� �� :
: ��    �� : 
: ��    �� :
: �� ȯ �� :
: ��    �� : ���� ����Լ� 
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
int ISAOCHECK::updateFlag(char *clname)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	sprintf( szWriteBuffer,
		//"POST http://%s/tjboyh/updateFlag.php?clName=%s&ticket=%d\r\n\r\n\r\n", hostName, clname,nClanTicket);
		"http://%s/tjboyh/updateFlag.php?clName=%s&ticket=%d", hostName, clname,nClanTicket);
	
	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	
	//ktj : �ӽ�
	fd2("updateFlag() �� db���� ������ %s ", szReadBuffer);
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

	ret_chk = chk_readbuffer("account=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//������Ʈ ����
		goto IsaoCheck_Free;
		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}












void clanDATA_save(char *fname, char *buff, int dataLen)
{
    HANDLE hFile;
	char allFname[64];
	sprintf( allFname,
		"%s\\%s\\%s",SAVE_DIR,szConnServerName, fname);

    DWORD dwAcess =0;
    hFile = CreateFile( allFname, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile != INVALID_HANDLE_VALUE ) {
        //����Ÿ ����     ����    ��ũ��    ������ ������ ����Ʈ��
        int chk = WriteFile( hFile , buff , dataLen , &dwAcess , NULL );
        //���� �ݱ�
        CloseHandle( hFile );
    }
}

void clanDATA_save2(char *fname, char *buff, int dataLen)
{
    HANDLE hFile;
	char allFname[64];
	sprintf( allFname,
		"%s\\%s",CLANINFO_DIR,  fname);

    DWORD dwAcess =0;
    hFile = CreateFile( allFname, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile != INVALID_HANDLE_VALUE ) {
        //����Ÿ ����     ����    ��ũ��    ������ ������ ����Ʈ��
        int chk = WriteFile( hFile , buff , dataLen , &dwAcess , NULL );
        //���� �ݱ�
        CloseHandle( hFile );
    }
}

















/*///////////////////////////////////////////////////////////////////////
NPC Wav���� ��������
///////////////////////////////////////////////////////////////////////*/

//http://211.39.144.158/NPCSound/darksanc_amb.yul
extern char szWavUrl[255];
int	ISAOCHECK::GetWavFile(char* FileName)
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [BUF_LEN];
	
	strcpy(szNoticeUrl,"211.39.144.158/NPCSound");
	sprintf(szWriteBuffer,"http://%s/%s.yul",szNoticeUrl,FileName);

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//g_zip.DeComOpen("darksanc_amb.yul");

	
	//ktj : �ӽ�
	
	debug_fd2( "GetWavFile() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "GetWavFile() ������ ���� : %d ���� : %s ", strlen(szReadBuffer), szReadBuffer);
	

	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	}
	//winsock ���� ���� ýũ��.  ================================== end

	ret_chk = chk_readbuffer("Code=", szReadBuffer, &iRejectCode );
	switch(ret_chk)
	{
	case 100:					//����
	case 101:					//�׷�Ŭ���̸�����.
	case 102:					//clanName�� ���� ������Ʈ ����
		goto IsaoCheck_Free;
	case ISAO_ERR_TCP_CONNECT:		
	case ISAO_ERR_NONE_REEJCT:					//�����ȣ�� ����.
		fd2("ISAO_ERR_NONE_REEJCT  : �����ȣ�� ����" );
		goto IsaoCheck_Free;
	}
	

IsaoCheck_Free:
	return ret_chk;


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}


/*
//ktj : �������ִ� ������ �ٿ�޾� ���Ϸ� �����ϴ� �Լ���.
*/
int ISAOCHECK::read_ClanImage(char *clanname,char *filename)
{
	int dataSize=0;
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [65536];	//�ӽ�
	char szBMPName[255];
	//char GSever;
	//char UniqueClanNum[64];

	ZeroMemory(szBMPName,sizeof(char)*255);




/*

	//============���� start

	//ktj : �̷��� �ص� ������ �������µ� �������. "GET http://%s/tjboyh/a.bmp\n\r\n\r\n", hostName);
	sprintf( szWriteBuffer,
		  "GET http://%s/ClanContent/%s.bmp\r\n\r\n\r\n\r\n", hostName1, filename);

	int cnt=0;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		//ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		ret_chk = connect_WEBdb_BINARY( szWriteBuffer, szReadBuffer, &dataSize);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}
	//============���� end
*/
	
	//�ֱ� ��ģ��
	sprintf( szWriteBuffer,
		//"http://211.39.144.158/ClanContent/%s.bmp",filename);
		"http://%s/ClanContent/%s.bmp", hostName1, filename);
		  //"GET http://211.39.144.158/ClanContent/713000102.bmp\r\n\r\n\r\n\r\n");


	int cnt=0;
	BOOL bret_chk = FALSE;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		bret_chk = GetHttpRequest(szWriteBuffer,szReadBuffer,65536,&dataSize);
		if(bret_chk) break;
		else cnt++;
		if(cnt>5) break;
		
	}
	




	//ktj : �ӽ�
	
	debug_fd2( "read_ClanImage() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "read_ClanImage() ������ ���� : %d ���� : bmp���Ͼ���2�������� %c%c. ", strlen(szReadBuffer), szReadBuffer[0],szReadBuffer[1]);
	




	
	
	

	//�ӽ�
	if((szReadBuffer[0] == 'B')&&(szReadBuffer[1] == 'M'))
	{
		char server[4];
		char uniqueclannum[64];
		
		//server = filename[0];
		//strcpy(uniqueclannum,&filename[3]);

		int nClanMarklen =0;
		
		ZeroMemory(server,sizeof(server));
		nClanMarklen = lstrlen(filename);
		if(nClanMarklen == 9) {
			server[0] = filename[0];
			strcpy(uniqueclannum,&filename[3]);
		}
		else{
			server[0] = filename[0];
			server[1] = filename[1];
			strcpy(uniqueclannum,&filename[4]);
		}

		//wsprintf(szBMPName,"%s.bmp",filename);
		wsprintf(szBMPName,"%s_%s.%s",server ,uniqueclannum,SAVE_EXE);
		clanDATA_save(szBMPName, szReadBuffer, dataSize);

		char path[128];
		char path2[128];
		int bmpSAVE(char *loadfname, char *savefname);
		wsprintf(path,"%s\\%s\\%s",SAVE_DIR,szConnServerName,szBMPName);
		wsprintf(path2,"%s\\%s\\%s_%s_16.%s",SAVE_DIR,szConnServerName,server ,uniqueclannum,SAVE_EXE);

		bmpSAVE(path, path2);

		ret_chk = 1;
	}
	else
	{
		ret_chk = 2;
	}

	/*char *Convert_HS2bin(char *hs_buf, int len);
	char *banaryData = Convert_HS2bin(szReadBuffer, dataSize/2);


	char saveFname[64];
	sprintf( saveFname, "CLAN_%s", filename);		//���������� CLAN_�̶�� �̸��� �Ǿտ� ����.
	clanDATA_save(saveFname, banaryData , dataSize/2);
	delete banaryData ;
	*/



	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		//ktj : ����Ÿ ������� ����Ʈ����
		fd2( "������ ���� ���� dataSize : %d ", dataSize);
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}

int ISAOCHECK::read_ClanImage2(char *clanname,char *filename,CLANINFO *ci)
{
	int dataSize=0;
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [65536];	//�ӽ�
	char szBMPName[255];

	ZeroMemory(szBMPName,sizeof(char)*255);





	/*
	//============���� s

	//ktj : �̷��� �ص� ������ �������µ� �������. "GET http://%s/tjboyh/a.bmp\n\r\n\r\n", hostName);
	sprintf( szWriteBuffer,
		//"POST http://%s/tjboyh/fileDOWN.php?filename=%s\r\n\r\n\r\n", hostName, filename);
		//"GET http://%s/tjboyh/clanimage/%s\n\r\n\r\n", hostName, filename);	//ktj.:OK -> �̷��� �ϸ� ���ϸ� �´�.
		  "GET http://%s/ClanContent/%s.bmp\r\n\r\n\r\n\r\n", hostName1,filename);
	
	int cnt=0;
	
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		//ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		ret_chk = connect_WEBdb_BINARY( szWriteBuffer, szReadBuffer, &dataSize);
		if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		cnt++;
	}

	//============���� e
	*/





	
	//�ֱ� ��ģ��

	sprintf( szWriteBuffer,
		"http://%s/ClanContent/%s.bmp", hostName1,filename);
		  //"GET http://211.39.144.158/ClanContent/713000102.bmp\r\n\r\n\r\n\r\n");

	int cnt=0;
	BOOL bret_chk = FALSE;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		bret_chk = GetHttpRequest(szWriteBuffer,szReadBuffer,65536,&dataSize);
		if(bret_chk) break;
		else cnt++;
		//ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
		//ret_chk = connect_WEBdb_BINARY( szWriteBuffer, szReadBuffer, &dataSize);
		//if(ret_chk != ISAO_ERR_RECONNECT) break;
		if(cnt>5) break;
		//cnt++;
	}
	



	//ktj : �ӽ�
	
	debug_fd2( "read_ClanImage2() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	debug_fd2( "read_ClanImage2() ������ ���� : %d ���� : bmp���Ͼ���2�������� %c%c. ", strlen(szReadBuffer), szReadBuffer[0],szReadBuffer[1]);
	





	
	char path[128];
	char path1[128];
	//�ӽ�
	if((szReadBuffer[0] == 'B')&&(szReadBuffer[1] == 'M'))
	{

		//wsprintf(szFile,"%s\\%c_%s.bmp",CLANINFO_DIR,ClanInfo[ClanInfo_nNum].ClanInfoHeader.Gserver,ClanInfo[ClanInfo_nNum].ClanInfoHeader.UniqueClaClanInfo_nNum);
		//wsprintf(szBMPName,"%s.bmp",filename);	
		

		wsprintf(szBMPName,"%s_%s.%s",ci->ClanInfoHeader.Gserver ,
								ci->ClanInfoHeader.UniqueClaClanInfo_nNum,
								SAVE_EXE);
		clanDATA_save(szBMPName, szReadBuffer, dataSize);

		
		int bmpSAVE(char *loadfname, char *savefname);
		wsprintf(path,"%s\\%s\\%s",SAVE_DIR,szConnServerName,szBMPName);
		wsprintf(path1,"%s\\%s\\%s_%s_16.%s",SAVE_DIR,szConnServerName,ci->ClanInfoHeader.Gserver ,ci->ClanInfoHeader.UniqueClaClanInfo_nNum,SAVE_EXE);

		bmpSAVE(path, path1);

		ret_chk = 1;
	}
	else
	{
		ret_chk = 2;
	}

	/*char *Convert_HS2bin(char *hs_buf, int len);
	char *banaryData = Convert_HS2bin(szReadBuffer, dataSize/2);


	char saveFname[64];
	sprintf( saveFname, "CLAN_%s", filename);		//���������� CLAN_�̶�� �̸��� �Ǿտ� ����.
	clanDATA_save(saveFname, banaryData , dataSize/2);
	delete banaryData ;
	*/



	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		//ktj : ����Ÿ ������� ����Ʈ����
		fd2( "������ ���� ���� dataSize : %d ", dataSize);
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}











//���̳ʸ� sorce�� dest�� hexa��Ʈ������ �ٲ�������.
void ISAOCHECK::Convert_bin2HS(char *sorce, int len)
{
	int i, j, Len;
	for(i=0; i<30; i++) {			//����Ÿ�� ������ �����.
		if(fileDATA_HS[i])	delete fileDATA_HS[i];
	}

	HSsu = len / 2048;
	if(len % 2048) HSsu++;
	char byte0, byte1;


	for(j=0; j<HSsu; j++)
	{
		if(j== (HSsu-1) ) {
			fileDATA_HS[j] = new char [ (len%2048)*2+1];			//hexa ��Ʈ���� 2�谡 ��.
			Len = len%2048;
		}
		else {
			fileDATA_HS[j] = new char [   (2048*2)+1];
			Len = 2048;
		}

		for(i=0; i<Len; i++) {
			byte0 = *sorce & 0x0f;
			byte1 = (*sorce & 0xf0)>>4;
			sorce++;

			if(byte0<10) {
				fileDATA_HS[j][i*2 + 0] = byte0+'0';
			}
			else {
				fileDATA_HS[j][i*2 + 0] = (byte0-10)+'A';
			}
	
			if(byte1<10) {
				fileDATA_HS[j][i*2 + 1] = byte1+'0';
			}
			else {
				fileDATA_HS[j][i*2 + 1] = (byte1-10)+'A';
			}
		}
		fileDATA_HS[j][i*2] = 0;		//��Ʈ���̶� �������� 0�� �ٿ�����.
	}
}


//hexa ��Ʈ���� ���̳ʸ��� �ٲ�����.
char *Convert_HS2bin(char *hs_buf, int len)
{
	char *dest;
	char byte0, byte1;
	dest = new char [len];			

	for(int i=0; i<len; i++) {
		byte0 = hs_buf[i*2 + 0];		//hexa��Ʈ���� ������ �ι��..
		byte1 = hs_buf[i*2 + 1];
			
		if( byte0 <='9') {
			 byte0 -= '0';
		}
		else {
			 byte0 -= 'A';
			 byte0 += 10;
		}

		if( byte1 <='9') {
			 byte1 -= '0';
		}
		else {
			 byte1 -= 'A';
			 byte1 += 10;
		}
		dest[i] = (byte1<<4) | byte0;
	}
	return dest;
}



char *fileREAD(char *fileName, char *buf, int *len)
{
    HANDLE hFile;
    DWORD dwAcess =0;
	char *newBuf;
    hFile = CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL );

    if ( hFile == INVALID_HANDLE_VALUE ) return 0;

    //SetFilePointer( hFile , 0, NULL , FILE_BEGIN );
	*len = GetFileSize( hFile , NULL );
	if(*len==0) goto closeJMP;

	if(buf == NULL) {
		newBuf= new char[*len];
	}
	else {
		newBuf= buf ;
	}
	ReadFile( hFile , newBuf , *len , &dwAcess , NULL );

    

closeJMP:
    CloseHandle( hFile );

    return newBuf;
}






//�����̹����� �о hexa��Ʈ������ �ٲ㼭 ������ �־����.
int ISAOCHECK::write_ClanImageFile(char *fname)
{
	int i;
	int dataSize=0;
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN*5];
	char szReadBuffer [BUF_LEN*5];	//�ӽ�
	

	//char fileName[] = "image\\clanDATA\\a222.txt";
	char fileName[64];
	char OrigFName[] = "a.bmp";
	char *fileBuf=NULL;
	int len;

	sprintf( fileName,"%s\\%s\\%s",SAVE_DIR,szConnServerName, fname	);


	fileBuf = fileREAD(fileName, NULL, &len);

	Convert_bin2HS(fileBuf, len);		//binary->hexa string���� ��ȯ
	if(fileBuf) 
		delete fileBuf;


	
	//�̰� ���߿� ������ �о���� �Լ��� ������.
	//Convert_HS2bin(hs_fileBuf , len);

	/*
	//file_DATA�κ��� ��� �������� too large ������ ����......
	sprintf( szWriteBuffer,
		"POST http://%s/tjboyh/clan_insertClanImage.php?file_name=%s&file_DATA=%s\r\n\r\n\r\n", hostName,OrigFName, fileDATA_HS[0]);
	//"GET http://%s/tjboyh/a.bmp\n\r\n\r\n", hostName);
	//"GET /tjboyh/ktj_login.php http/1.0\n\r\n\r\n");	//ktj.:OK
	//"GET http://%s/tjboyh/abc.txt\n\r\n\r\n", hostName);	//ktj.:OK -> �̷��� �ϸ� ���ϸ� �´�.
	//"GET /tjboyh/abc.txt http/1.0\n\r\n\r\n");	//ktj.:OK -> �̷��� �ϸ� ���������� �������Բ� �����̿´�.
	
	while(1) {
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
	if(szReadBuffer[0]!=0) 
		break;
	}

	//ktj : �ӽ�
	fd2(" �� db���� ������ %s ", szReadBuffer);
	*/







	for(i=0; i<HSsu; i++) 
	{
		sprintf( szWriteBuffer,	//file_name=�����̸�&file_HSsu=2048���̳ʸ�(4096��罺Ʈ��)�������&file_cnt=���ī��Ʈ&file_DATA=������ ����Ÿ.
			"POST http://%s/tjboyh/clan_insertClanImage.php?file_name=%s&file_HSsu=%d&file_cnt=%d&file_DATA=%s\r\n\r\n\r\n", hostName,OrigFName,    HSsu,i+1,fileDATA_HS[i]);

		while(1) {
			ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
			if(szReadBuffer[0]!=0) 
				break;
		}
	}

	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���
	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		//fd2( "From server: ---from--\n%s--end--\n ",szReadBuffer);			
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end


IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}























/*///////////////////////////////////////////////////////////////////////////
: �� �� �� :  int Exam()
: �� �� �� :  ������ �����ϱ����� example��
: ��    �� :  ���Լ����� ������ ���ĺ���.
: ��    �� :
: �� ȯ �� :
: ��    �� :
: ��    �� :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/

int ISAOCHECK::WEBdb_Exam()
{
	int ret_chk;
	int iRejectCode = 111111;
	char szWriteBuffer[1024];
	char szReadBuffer [1024]={0,};
	
	
	sprintf( szWriteBuffer,
		"POST http://%s/tjboyh/ktj_login.php\r\n\r\n\r\n" ,hostName);
	ret_chk = connect_WEBdb( szWriteBuffer, szReadBuffer);
	
	//ktj : �ӽ�
	fd2(" �� db���� ������ %s ", szReadBuffer);
	
	return ret_chk ;
}


void WEBdb_Exam()
{
	iCheck.init_HostName_Port("211.192.180.113", 80);
	iCheck.init(1, 20);
	iCheck.WEBdb_Exam();
}


/*
//ktj : �������ִ� ������ �ٿ�޾� ���Ϸ� �����ϴ� �Լ���.
*/
int ISAOCHECK::read_sod2(char *usid, char *charname,char *gserver, int index , char *szSod2Buff)
{
	int dataSize=0;
	int ret_chk = 0; 
	int iRejectCode = 111111;
	char szWriteBuffer[BUF_LEN];
	char szReadBuffer [65536];	//�ӽ�
	memset(szSod2Buff,0,sizeof(szSod2Buff));
	memset(szWriteBuffer,0,sizeof(szWriteBuffer[BUF_LEN]));
	memset(szReadBuffer,0,sizeof(szReadBuffer));

/*
//�Ϻ�Ŭ���̾ƴϰ�SOD
#ifdef _LANGUAGE_JAPANESE

	char UTF8_charname[1024];				memset(UTF8_charname,0,sizeof(UTF8_charname));

	MBCSToWeb(charname,				UTF8_charname,932);
	
	sprintf( szWriteBuffer,
	"http://%s/servermain/sod2info.asp?userid=%s&chname=%s&gserver=%s&index=%d", hostName, usid, 
	UTF8_charname, 
	gserver, index);

#else
*/
	sprintf( szWriteBuffer,
	"http://%s/servermain/sod2info.asp?userid=%s&chname=%s&gserver=%s&index=%d", hostName, usid, charname, gserver, index);
//#endif

	int cnt=0;
	BOOL bret_chk = FALSE;
	while(1) {		//����Ʈ�ǵ� ���� �ȿ��°�츦 ���� 5������ �����Ѵ�.
		bret_chk = GetHttpRequest(szWriteBuffer,szSod2Buff,65536,&dataSize);
		if(bret_chk) break;
		else cnt++;
		if(cnt>5) break;
		 
	}

	//ktj : �ӽ�
	//debug_fd2( "read_ClanImage() ������ ���� : %d ���� : %s ", strlen(szWriteBuffer), szWriteBuffer);
	//debug_fd2( "read_ClanImage() ������ ���� : %d ���� : bmp���Ͼ���2�������� %c%c. ", strlen(szReadBuffer), szReadBuffer[0],szReadBuffer[1]);
	
	//ktj : �Ʒ� ýũ�� ��� �ӽ������� ���

	//winsock ���� ���� ýũ��.  ================================== start
	switch(ret_chk) {
	case ISAO_ERR_WINSOCK_NOERR:		//���� ����.
		//ktj : �ӽ÷� �������� �� ����Ÿ�� �ѷ���.
		//ktj : ����Ÿ ������� ����Ʈ����
		fd2( "������ ���� ���� dataSize : %d ", dataSize);
		break;
	case ISAO_ERR_WINSOCK_START:		//���� START�� �����߻�				//window only
		fd2("WSAStartup()����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_WINSOCK_VERSION:		//���ӹ����� Ʋ�� ������������.			//window only
		fd2("������ ���� ����");
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_TCP_CONNECT:			// ���� ���Ǵ� �ԣã��ڳ����信��
		fd2( "connect() ���� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_READ:				// �ӣӣ� ���� �� �� �����٣�
		fd2(" recv()  �б� ����" );
		goto IsaoCheck_winsockERR;
	case ISAO_ERR_SSL_TIMEOUT:			// Ÿ�� �ƿ��ߴ٣�
		fd2("---------------- select() Ÿ�Ӿƿ� ����" );
		goto IsaoCheck_winsockERR;
	};
	//winsock ���� ���� ýũ��.  ================================== end

IsaoCheck_winsockERR:			//winsock���� ����ÿ�����
	return ret_chk ;
}



bool UTF8ToWeb(char *szUTF8,char *szWeb)
{
	int iCount = 0;
	char chHigh;
	char chLow;
	for(int i=0;szUTF8[i]!=0;i++)
	{
		chHigh = chLow = 0;
		

		// %����
		szWeb[iCount] = '%';
		iCount++;

		//���� ��Ʈ
		chHigh = (szUTF8[i] & 0xf0) >> 4;

		if( (0<= chHigh) &&	(chHigh <= 9))
		{
			szWeb[iCount] = '0' + chHigh;
			iCount++;
		}
		else
		{
			szWeb[iCount] = 'a' + chHigh - 0x0a;
			iCount++;
		}

		//���� ��Ʈ
		chLow = szUTF8[i] & 0x0f;

		if( (0<= chLow) && (chLow <= 9) )
		{
			szWeb[iCount] = '0' +  chLow;
			iCount++;
		}
		else
		{
			szWeb[iCount] = 'a' + chLow - 0x0a;
			iCount++;
		}

	}
	szWeb[iCount] = 0;
	
	return true;	
}

bool MBCSToUTF8(char *szMBCS,char *szUTF8,int iMBCSCodePage)
{
	wchar_t wUniCode[1024];	memset(wUniCode,0,sizeof(wUniCode));

	//�����ڵ�� ��ȯ => ���� wUnicode��
	MultiByteToWideCharByCodePage(szMBCS,wUniCode,iMBCSCodePage);

	//UTF8�� ��ȯ => ���� szUTF8��
	WideCharToMultiByteByCodePage(wUniCode,szUTF8,CP_UTF8);

	return true;
}

bool	MultiByteToWideCharByCodePage(char *szMBCS,wchar_t *szUnicode,int iCodePage)
{
	int iLen = MultiByteToWideChar(iCodePage, 0, szMBCS, lstrlen(szMBCS), NULL, NULL);
	MultiByteToWideChar(iCodePage, 0, szMBCS, lstrlen(szMBCS), szUnicode, iLen);

	return true;
}



bool	WideCharToMultiByteByCodePage(wchar_t *szUnicode,char *szMBCS,int iCodePage)
{//wcslen
	int iLen = WideCharToMultiByte(iCodePage, 0, szUnicode, wcslen(szUnicode), NULL, NULL,NULL,NULL);
	WideCharToMultiByte(iCodePage, 0, szUnicode, wcslen(szUnicode), szMBCS, iLen,NULL,NULL);

	return true;
}

bool MBCSToWeb(char *szMBCS,char *szWeb,int iMBCSCodePage)
{
	char szUTF8[1024];	memset(szUTF8,0,sizeof(szUTF8));
	MBCSToUTF8(szMBCS,szUTF8,iMBCSCodePage);
	UTF8ToWeb(szUTF8,szWeb);
	return true;
}

