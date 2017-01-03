/*******************************************************************************************
*������ : ������
*ó������ ��¥ : 2003.9.27
*���� ���� ��¥ : 2003.9.29
*******************************************************************************************/

#ifndef __ZIP_H__
#define __ZIP_H__

#define YULVERSION		00001
#define DATASIZE		1024*1024*10

#define NPCWAV_DIR "image\\clanImage\\NPCWav"

typedef struct
{
	char szZipVersion[64];			//zlib����
	char  cUnCompFilName[64];		//������ Ǯ������ ���� �̸�
	DWORD dwVersion;				//ZipInfo����
	DWORD dwFileSize;				//
	DWORD ulCompLen;				//����ũ��
	DWORD dwDecopLen;
	DWORD dwFlag;
	
	
	char  cData[DATASIZE];					//������ ����Ÿ�� ����
}ZIPINFO;

class CZip
{
public:
	CZip();	
	~CZip();

public:
	VOID Init();
	BOOL CompOpen(char* FileName);
	BOOL DeComOpen(char* FileName);
	BOOL Write(char* FileName,LPCVOID data,int len);
	VOID DeleteData();
	BOOL ZipCompression(char* data,char* FileName,DWORD len);
	BOOL ZipDecompression(char* data);
	VOID ZipCheckError(int err);
	int CheckFile(char* FileName,int nMatch,char* cMath);
	VOID GetFileName(char* FileName,int nMatch);
	BOOL WriteYUL(char* FileName,char* data);

private:
	HANDLE m_hFile;
	char* m_cData;
	//char* m_cCompressData;
	//char* m_cUnCompreeData;
	unsigned long	m_ulCompressLen;
	unsigned long	m_ulComprLen;
	unsigned long	m_ulUnComprLen;
	DWORD			m_dwFileSize;

	ZIPINFO			m_sZi;
	char			m_szFileName[256];

	
};
#endif

#ifdef ZIP_CPP

class CZip g_zip;
#else
extern class CZip g_zip;
#endif