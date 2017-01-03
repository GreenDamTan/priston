#include <windows.h>
#include "common.h"
#include "smreg.h"
#include <stdio.h>
#include "Language\\language.h"


//������Ʈ�� ���� �����κ�(�ӽ÷� �ּ�ó����)
#ifdef _LANGUAGE_VEITNAM
#else
#ifdef _LANGUAGE_ARGENTINA
#else
	#define	FILE_REG_NAME		"ptReg.rgx"
#endif
#endif



// ������Ʈ������ ���������� ����Ÿ�� ���Ѵ�
BYTE *RegDataBuff = new BYTE[128];

#ifdef FILE_REG_NAME

static char *szRegFileName = FILE_REG_NAME;


struct smREG_STRING {
	char	szRegPath[128];
	char	szRegString[128];
};

struct smREG_BUFF {
	int	nCount;
	smREG_STRING	RegString[100];
};

static int RegFileMode = 0;

static smREG_BUFF smRegBuff;

static char *GetWord(char *q , char *p)
{

	//	while ( *p != '\n' ) {
	while ( (*p == 32) || (*p == 9) ) // SPACE or TAB or ':'�� ���� ��Ŵ
	{
		p++;
	}

	while ( (*p != 32) && (*p != 9) ) // SPACE or TAB
	{
		if ( *p == '\n' || *p=='\r' ) break;
		*q++ = *p++;
	}
	*q++ = 0;	// end of one string, put Null character
	//	}
	//	*q = '\n';

	return p;
}

static char *GetString(char *q , char *p)
{

	while(*p!=34) p++;

	p++;

	while( (*p!=34) ) {
		if ( *p==0 || *p==0x0D || *p==0x0A ) break;
		*q++ = *p++;
	}

	p++;

	*q++ = 0;	// end of one string, put Null character

	return p;
}

static int ReadRegFile()
{
	char string[512];
	char line[512];
	char *p;

	smRegBuff.nCount = 0;

	FILE *fp;
	fp = fopen(	szRegFileName , "rb" );
	if (!fp) return FALSE;

	while( !feof( fp ) )//  feof: file end���� �о�� 
	{
		if( fgets( line, 512, fp ) == NULL)	
			break;
		p = GetString( string , line);
		lstrcpy( smRegBuff.RegString[ smRegBuff.nCount ].szRegPath , string );
		p = GetString( string , p);
		lstrcpy( smRegBuff.RegString[ smRegBuff.nCount ].szRegString , string );

		smRegBuff.nCount++;
	}

	fclose(fp);

	return TRUE;
}

static int SaveRegFile()
{
	char line[512];


	FILE *fp;
	fp = fopen(	szRegFileName , "wb" );
	if (!fp) return FALSE;

	for (int cnt=0;cnt<smRegBuff.nCount;cnt++) {

		wsprintf( line , "\"%s\" \"%s\"\r\n" , smRegBuff.RegString[cnt].szRegPath , smRegBuff.RegString[cnt].szRegString );
		fwrite( line , lstrlen(line) , 1, fp );

	}
	fclose(fp);

	return TRUE;
}

static smREG_STRING *GetRegString_File( char *szRegName )
{
	if ( !RegFileMode ) {
		ReadRegFile();
		RegFileMode++;
	}

	for(int cnt=0;cnt<smRegBuff.nCount;cnt++) {

		if ( lstrcmpi( smRegBuff.RegString[cnt].szRegPath , szRegName )==0 ) {
			return &smRegBuff.RegString[cnt];
		}
	}

	return NULL;
}

static int SetRegString_File( char *szRegName , char *szString )
{

	smREG_STRING *RegString = GetRegString_File( szRegName );

	if ( RegString ) {
		//������ ����
		lstrcpy( RegString->szRegString , szString );
	}
	else {
		//�ű� �߰�
		lstrcpy( smRegBuff.RegString[ smRegBuff.nCount ].szRegPath , szRegName );
		lstrcpy( smRegBuff.RegString[ smRegBuff.nCount ].szRegString , szString );
		smRegBuff.nCount++;
	}

	SaveRegFile();		//���Ϸ� ����

	return TRUE;
}

char *GetRegString( HKEY hkeyRoot , char *RegPath , char *RegName )
{
	DWORD	strLength;
	DWORD	lpType;
	HKEY	hkey;
	int		result;


	smREG_STRING *RegString = GetRegString_File( RegName );			//���Ͽ��� ã�Ƽ� �о��
	if ( RegString ) {
		lstrcpy( (char *)RegDataBuff , RegString->szRegString );
		return (char *)RegDataBuff;
	}

	strLength = 128;
	lpType = REG_SZ;

	RegDataBuff[0] = 0;

	result = RegOpenKey( hkeyRoot , RegPath , &hkey  );

	if ( result==ERROR_SUCCESS ) {
		result = RegQueryValueEx( hkey , RegName , NULL , &lpType , RegDataBuff , &strLength );
		RegCloseKey( hkey );
	}

	if ( result!=ERROR_SUCCESS ) return NULL;

	SetRegString_File( RegName , (char *)RegDataBuff );			//���Ϸ� ���� ����

	return (char *)RegDataBuff;
}

// ������Ʈ������ ���������� ����Ÿ�� �����Ѵ�.
int SetRegString( HKEY hkeyRoot , char *RegPath , char *RegName ,char *data )
{

	return SetRegString_File( RegName , data );			//���Ϸ� ���� ����
}

#else

char *GetRegString( HKEY hkeyRoot , char *RegPath , char *RegName )
{
	DWORD	strLength;
	DWORD	lpType;
	HKEY	hkey;
	int		result;
	
	strLength = 128;
	lpType = REG_SZ;

	RegDataBuff[0] = 0;

	result = RegOpenKey( hkeyRoot , RegPath , &hkey  );

	if ( result==ERROR_SUCCESS ) {
		result = RegQueryValueEx( hkey , RegName , NULL , &lpType , RegDataBuff , &strLength );
		RegCloseKey( hkey );
	}

	if ( result!=ERROR_SUCCESS ) return NULL;

	return (char *)RegDataBuff;
}

// ������Ʈ������ ���������� ����Ÿ�� �����Ѵ�.
int SetRegString( HKEY hkeyRoot , char *RegPath , char *RegName ,char *data )
{
	DWORD	strLength;
	HKEY	hkey;
	int		result;
	DWORD	dwDisposition;

	strLength = 128;


//	result = RegOpenKey( hkeyRoot , RegPath , &hkey  );
	result = RegCreateKeyEx( hkeyRoot, RegPath, 0,"",
				REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
				&hkey , &dwDisposition );

	if ( result==ERROR_SUCCESS ) {
		result = RegSetValueEx( hkey, RegName, NULL,REG_SZ, (CONST BYTE *)data , lstrlen((char *)data) );
		RegCloseKey( hkey );
	}

	if ( result!=ERROR_SUCCESS ) return FALSE;

	return TRUE;
}

#endif

// ���� ��Ʈ���� ���� �����
int DeleteRegValue( HKEY hkeyRoot , char *RegPath , char *RegName  )
{
	DWORD	strLength;
	HKEY	hkey;
	int		result;

	strLength = 128;

	result = RegOpenKey( hkeyRoot , RegPath , &hkey  );
	if ( result==ERROR_SUCCESS ) {
		result = RegDeleteValue( hkey, RegName );
		RegCloseKey( hkey );
	}

	if ( result!=ERROR_SUCCESS ) return FALSE;

	return TRUE;
}
