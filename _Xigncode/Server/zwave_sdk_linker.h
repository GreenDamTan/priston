//7A7A7A584023252521217E6667766D65696C320058000000000002000000000022A6D20410A40000409C000011A40000880100000040A7E3A36BC90100006E5C758ACA017176785A5E24242421217E7A766D657473724200
#ifndef ZWAVE_SDK_LINKER
#define ZWAVE_SDK_LINKER

#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
#pragma warning(disable:4995)

#define ZWAVE_STATE_STOP	0x0001
#define ZWAVE_STATE_RUN		0x0002
#define ZWAVE_STATE_PENDING	0x0004

enum Z_SCAN_TYPE_LIST
{
	// ġƮ���� Ž�� ��ȸ�� �����ڵ�
	Z_SCAN_TYPE_ANTI_CHEAT_ENGINE=1,
	// ��Ŭ���̾�Ʈ Ž�� ��ȸ�� �����ڵ�
	Z_SCAN_TYPE_ANTI_NONECLIENTBOT=2,
	// Ŭ���̾�Ʈ ��Ƽ�׸�Ƽ ���� �ڵ�
	Z_SCAN_TYPE_TEST_CLIENT_INTEGRITY=4,
};


// �����
enum Z_RETURN
{
	// �߸��� ��Ŷ
	Z_RTN_ERROR = -1,
	// �������� Ŭ���̾�Ʈ
	Z_RTN_NONE, 
	// ��Ŭ���̾�Ʈ �� Ŭ���̾�Ʈ
	Z_RTN_NONCLIENT,
	// ġƮ���� Ž��
	Z_RTN_BLACK_CODE,
	// ġƮ���� ���� Ž��
	Z_RTN_SUSPICIOUS,
	// ��ũ�� Ž��
	Z_RTN_USERDEFINED,
	// ����� ����
	Z_RTN_RESEND
	// ��Ŷ ������
};

typedef struct _ZWAVE_DEBUG_INFOA
{
	int		make_BlockSize;			// ��ü ������ ��Ŷ ������
	int		code_BlockSize;			// ��ü ��Ŷ�� �ڵ� �� ������
	int		code_FileSize;				// ������ ��ü ũ��
	int		code_HashCount;			// ��ü ���ϻ����� / �ڵ�������� = �ڵ� ������ �� ����
	FILE*	z_fp;						// File open Handle
	char	szCodeFile[MAX_PATH];		// �ڵ� ���ϸ�
	char	szCodeResult[MAX_PATH];	// ���ϰ�
	char	szFunction[MAX_PATH];		// �Լ���
	int		code_Index_Table;			// ����ϴ� ���̺�
	int		code_Database_Index;		// ����ϴ� ���ڵ� ��ȣ
} ZWAVE_DEBUG_INFOA, *PZWAVE_DEBUG_INFOA;

typedef struct _ZWAVE_DEBUG_INFOW
{
	int		make_BlockSize;			// ��ü ������ ��Ŷ ������
	int		code_BlockSize;			// ��ü ��Ŷ�� �ڵ� �� ������
	int		code_FileSize;				// ������ ��ü ũ��
	int		code_HashCount;			// ��ü ���ϻ����� / �ڵ�������� = �ڵ� ������ �� ����
	FILE*	z_fp;						// File open Handle
	WCHAR	szCodeFile[MAX_PATH];		// �ڵ� ���ϸ�
	WCHAR	szCodeResult[MAX_PATH];	// ���ϰ�
	WCHAR	szFunction[MAX_PATH];		// �Լ���
	int		code_Index_Table;			// ����ϴ� ���̺�
	int		code_Database_Index;		// ����ϴ� ���ڵ� ��ȣ
} ZWAVE_DEBUG_INFOW, *PZWAVE_DEBUG_INFOW;

typedef char *PACKETDATA;
typedef const char *CPACKETDATA;

#ifndef LPCBYTE
typedef const BYTE *LPCBYTE;
#endif

#ifdef _UNICODE
#define ZWAVE_DEBUG_INFO			ZWAVE_DEBUG_INFOW
#define ZSWAVE_Initialize		ZSWAVE_InitializeW
#define ZSWAVE_GetLibraryPath	ZSWAVE_GetLibraryPathW
#define ZSWAVE_VerifyEx			ZSWAVE_VerifyExW
#define ZSWAVE_SetVerifyValue	ZSWAVE_SetVerifyValueW
#define ZSWAVE_DebugInfo			ZSWAVE_DebugInfoW
#define ZSWAVE_Link				ZSWAVE_LinkW
#else
#define ZWAVE_DEBUG_INFO		ZWAVE_DEBUG_INFOA
#define ZSWAVE_Initialize		ZSWAVE_InitializeA
#define ZSWAVE_GetLibraryPath	ZSWAVE_GetLibraryPathA
#define ZSWAVE_VerifyEx			ZSWAVE_VerifyExA
#define ZSWAVE_SetVerifyValue	ZSWAVE_SetVerifyValueA
#define ZSWAVE_DebugInfo		ZSWAVE_DebugInfoA
#define ZSWAVE_Link				ZSWAVE_LinkA
#endif


BOOL ZSWAVE_LinkA(LPCSTR pszlibdllpath);
BOOL ZSWAVE_LinkW(LPCWSTR pszlibdllpath);
BOOL ZSWAVE_UnLink();

BOOL	WINAPI ZSWAVE_InitializeW(LPCWSTR path);
BOOL	WINAPI ZSWAVE_InitializeA(LPCSTR path);
BOOL	WINAPI ZSWAVE_Uninitialize();
DWORD	WINAPI ZSWAVE_GetDBConnectionCount();
BOOL	WINAPI ZSWAVE_GetLibraryPathW(LPWSTR path, SIZE_T size);
BOOL	WINAPI ZSWAVE_GetLibraryPathA(LPSTR path, SIZE_T size);
int		WINAPI ZSWAVE_GetGroupCount();
int		WINAPI ZSWAVE_GetGroupDataCount(int index);
HANDLE	WINAPI ZSWAVE_OpenAt(int gi, int di, SIZE_T bs);
HANDLE	WINAPI ZSWAVE_Open(SIZE_T bs, DWORD type);
DWORD	WINAPI ZSWAVE_WaitForStop(DWORD t);
DWORD	WINAPI ZSWAVE_WaitForState(LONG state, DWORD t);
BOOL	WINAPI ZSWAVE_Close(HANDLE h);
BOOL	WINAPI ZSWAVE_GetDataAt(HANDLE h, int no, PACKETDATA data);
BOOL	WINAPI ZSWAVE_Verify(HANDLE h, LPCBYTE data, SIZE_T size);
int		WINAPI ZSWAVE_VerifyExA(HANDLE h, CPACKETDATA data, SIZE_T size, LPSTR msg, SIZE_T msgSize);
int		WINAPI ZSWAVE_VerifyExW(HANDLE h, CPACKETDATA data, SIZE_T size, LPWSTR msg, SIZE_T msgSize);
BOOL	WINAPI ZSWAVE_SetVerifyValueA(HANDLE h, LPCSTR v);
BOOL	WINAPI ZSWAVE_SetVerifyValueW(HANDLE h, LPCWSTR v);
BOOL	WINAPI ZSWAVE_DebugInfoA(HANDLE h, PZWAVE_DEBUG_INFOA info);
BOOL	WINAPI ZSWAVE_DebugInfoW(HANDLE h, PZWAVE_DEBUG_INFOW info);
LONG	WINAPI ZSWAVE_GetState();
BOOL	WINAPI ZSWAVE_SetState(LONG s);
int		WINAPI ZSWAVE_GetDataCount(HANDLE s);

#ifndef ZWAVE_ERROR_CODE
#define ZWAVE_ERROR_CODE
#define ZWAVE_STATUS_BASE						0xE0190000
#define ZWAVE_STATUS_INVALID_HANDLE				(ZWAVE_STATUS_BASE | 0x0001)
#define ZWAVE_STATUS_CODE_NOT_FOUND				(ZWAVE_STATUS_BASE | 0x0002)
#define ZWAVE_STATUS_NOT_INITIALIZED			(ZWAVE_STATUS_BASE | 0x0003)
#define ZWAVE_STATUS_UNICODE_CONVERSION_FAIL	(ZWAVE_STATUS_BASE | 0x0004)
#define ZWAVE_STATUS_INVALID_PARAMETER			(ZWAVE_STATUS_BASE | 0x0005)
#define ZWAVE_STATUS_INVALID_DATABASE			(ZWAVE_STATUS_BASE | 0x0006)
#define ZWAVE_STATUS_INSUFFICIENT_RESOURCE		(ZWAVE_STATUS_BASE | 0x0007)
#define ZWAVE_STATUS_UNHANDLED_EXCEPTION		(ZWAVE_STATUS_BASE | 0x0008)
#define ZWAVE_STATUS_STATE_MISMATCH				(ZWAVE_STATUS_BASE | 0x0009)
#define ZWAVE_STATUS_INVALID_DLL				(ZWAVE_STATUS_BASE | 0x0010)
#define ZWAVE_STATUS_INVALID_OPERATION			(ZWAVE_STATUS_BASE | 0x0011)
#define ZWAVE_STATUS_INVALID_PACKET				(ZWAVE_STATUS_BASE | 0x0012)
#define ZWAVE_STATUS_PACKET_VERSION_MISMATCH	(ZWAVE_STATUS_BASE | 0x0013)
#define ZWAVE_STATUS_PACKET_HANDLE_MISMATCH		(ZWAVE_STATUS_BASE | 0x0014)
#define ZWAVE_STATUS_PACKET_STATE_MISMATCH		(ZWAVE_STATUS_BASE | 0x0015)
#define ZWAVE_STATUS_PACKET_SEQNUM1_MISMATCH	(ZWAVE_STATUS_BASE | 0x0016)
#define ZWAVE_STATUS_PACKET_SEQNUM2_MISMATCH	(ZWAVE_STATUS_BASE | 0x0017)
#define ZWAVE_STATUS_PACKET_NOT_COMS			(ZWAVE_STATUS_BASE | 0x0018)
#define ZWAVE_STATUS_PACKET_CRC_ERROR			(ZWAVE_STATUS_BASE | 0x0019)
#define ZWAVE_STATUS_DB_CRC_ERROR				(ZWAVE_STATUS_BASE | 0x0020)
#define ZWAVE_STATUS_NCB_NOT_EXISTS				(ZWAVE_STATUS_BASE | 0x0021)
#define ZWAVE_STATUS_INT_NOT_EXISTS				(ZWAVE_STATUS_BASE | 0x0022)
#define ZWAVE_STATUS_XCE_NOT_EXISTS				(ZWAVE_STATUS_BASE | 0x0023)
#define ZWAVE_STATUS_VERSION_MISMATCH			(ZWAVE_STATUS_BASE | 0x0024)
#endif

#endif