//7A7A7A584023252521217E6667766D65696C3200580000000000020001000000060575B61879000030750000197900009A01000000C07640094CC8010080FB4D18C8CC017176785A5E24242421217E7A766D657473724200
#pragma once

#ifndef BOOL
typedef int BOOL;
#endif

#define ZSOPT_USESYSGUARD	0x01
#define ZSOPT_SHOWSPLASH	0x02
#define ZSOPT_SHOWTRAY		0x04

#ifndef ZWAVE_ERROR_CODE
#define ZWAVE_STATUS_INVALID_HANDLE				0xE0190001
#define ZWAVE_STATUS_CODE_NOT_FOUND				0xE0190002
#define ZWAVE_STATUS_NOT_INITIALIZED			0xE0190003
#define ZWAVE_STATUS_UNICODE_CONVERSION_FAIL	0xE0190004
#define ZWAVE_STATUS_INVALID_PARAMETER			0xE0190005
#define ZWAVE_STATUS_INVALID_DATABASE			0xE0190006
#define ZWAVE_STATUS_INSUFFICIENT_RESOURCE		0xE0190007
#define ZWAVE_STATUS_UNHANDLED_EXCEPTION		0xE0190008
#define ZWAVE_STATUS_STATE_MISMATCH				0xE0190009
#define ZWAVE_STATUS_INVALID_DLL				0xE0190010
#define ZWAVE_STATUS_INVALID_OPERATION			0xE0190011
#define ZWAVE_STATUS_INVALID_PACKET				0xE0190012
#endif

typedef char *PACKETDATA;
typedef const char *CPACKETDATA;

BOOL ZCWAVE_Init();
BOOL ZCWAVE_Cleanup();
BOOL ZCWAVE_Probe(CPACKETDATA szPack, PACKETDATA szReturn, SIZE_T nSize);
HANDLE ZCWAVE_CreateCodeBox();
BOOL ZCWAVE_CloseCodeBox(HANDLE h);
BOOL ZCWAVE_ProbeCodeBox(HANDLE h, CPACKETDATA data, PACKETDATA buf, SIZE_T size);
BOOL ZCWAVE_SysEnter(LPCTSTR path, DWORD opt);
BOOL ZCWAVE_SysExit();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                