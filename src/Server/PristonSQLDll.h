#ifdef PRISTONSQLDLL_EXPORTS
#define PRISTONSQLDLL_API __declspec(dllexport)
#else
#define PRISTONSQLDLL_API __declspec(dllimport)
#endif


//1) �α��ν� 
PRISTONSQLDLL_API int SQLLoginProcess( char * lpUserID );

//2) ĳ���� ������
PRISTONSQLDLL_API int SQLAddCharProcess( char * lpUserID, char * lpCharID );

//3) ĳ���� ������
PRISTONSQLDLL_API int SQLDelCharProcess( char * lpUserID, char * lpCharID );

//4) �α׾ƿ���, Client ���� ����������
PRISTONSQLDLL_API int SQLLogoutProcess( char * lpUserID, char * lpCharID, char * lpRace, char * lpOccupation, unsigned long nExpPoint, int nLevel );
