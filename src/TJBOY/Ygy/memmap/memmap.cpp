#include <windows.h>
#include "memmap.h"
#define __MEMDEBUG



extern void fd( char *fmt, ...);
extern void fd2( char *fmt, ...);


CMemMap::CMemMap()
{
	m_cMemMapPtr = NULL;
	m_hMemMap = NULL;
	m_nMemMapSize = 0;
	m_bIsMemMap = FALSE;
	
}

CMemMap::~CMemMap()
{
}

VOID CMemMap::Clear()
{



	if((m_cMemMapPtr != NULL) &&m_bIsMemMap)
	{
		UnmapViewOfFile(m_cMemMapPtr);
	}
	if(m_hMemMap != NULL)
	{
		CloseHandle(m_hMemMap);
	}
}

VOID CMemMap::SetName(char* name)
{
	ZeroMemory(m_szMemMapName, sizeof(m_szMemMapName) );
	strncpy(m_szMemMapName,name,lstrlen(name));
}

VOID CMemMap::SetMemMapSize(int size)
{
	m_nMemMapSize = size;
}

/*=========================================================================================
*�޸� �� �ʱ�ȭ �Ѵ�.
==========================================================================================*/
BOOL CMemMap::InitMemoryMap()
{
	fd2( "�޸� �� InitMemoryMap() 00");

	if((m_nMemMapSize > 0) &&(m_szMemMapName[0] != 0))
	{
		m_hMemMap = CreateFileMapping(
			INVALID_HANDLE_VALUE,		//0xffffffff���� ����
			NULL,						//���ȼӼ� : ���� NULL
			PAGE_READWRITE,				//�б�, ����, �б�+����
			0,							//�Ҵ�޸��� �ִ�ũ���� ���� 32��Ʈ��(64��Ʈ������ ���������ϱ⶧��)
			m_nMemMapSize,				//�Ҵ�޸��� ���� 32��Ʈ
			m_szMemMapName);		//���� �̸�

		//���� ����
		if(m_hMemMap == NULL)
		{
			fd2( "�޸� �� InitMemoryMap() 11 ����");
			return FALSE;
		}	

		m_cMemMapPtr =(char *)MapViewOfFile(m_hMemMap, FILE_MAP_ALL_ACCESS,0,0, m_nMemMapSize);

		//�޸� �� �ּҸ� ������ ���µ� ����
		if(m_cMemMapPtr == NULL)
		{
			
			fd2( "�޸� �� InitMemoryMap() 44 FALSE");

			return FALSE;
		}
	}
	else
		return FALSE;


	fd2( "�޸� �� InitMemoryMap() 55 TRUE");
	m_bIsMemMap = TRUE;

	return TRUE;
}

/*=========================================================================================
�޸� �ʿ� ����Ÿ ����..
==========================================================================================*/
VOID CMemMap::WriteMemoryMap(char *data)
{
	if(m_nMemMapSize <=0) 
		return;
	ZeroMemory(m_cMemMapPtr, m_nMemMapSize);

#ifdef __MEMDEBUG
	char szTemp[1024];
	memcpy(szTemp,data,1024);

#endif
	memcpy(m_cMemMapPtr, data, m_nMemMapSize);
}

//ktj : size�־����
VOID CMemMap::WriteMemoryMap(char *data, int size)
{
	if(m_nMemMapSize <=0) 
		return;
	ZeroMemory(m_cMemMapPtr, m_nMemMapSize);

	memcpy(m_cMemMapPtr, data, size);
}





VOID CMemMap::ReadMemoryMap(char* data)
{
	ZeroMemory(data, m_nMemMapSize);
	memcpy(data,m_cMemMapPtr, m_nMemMapSize);
}