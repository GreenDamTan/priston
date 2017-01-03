/*===========================================================================================
============================================================================================*/
#pragma once
#ifndef __PACKET_H__
#define __PACKET_H__

#include "../Memmap/memmap.h"


//#define MEMORYMAP_MAX  4096

class CPacket
{
public:
	CPacket();
	~CPacket();
public:
	VOID Init();
	VOID Clear();	
	VOID Parsing();
	
	char* GetData(){return memMapReadData;}						//���� ����Ÿ�� ����
	VOID SetSelectCha(char *id,char *chaname, char *clanname,DWORD clannum); //ĳ���� ����

	//�̰� ���߿�...
	VOID SetClanChat(char *msg);


public:
	

private:
	INDEX			*m_Index;
	USERINFO		m_userInfo;

	CLANCHAT            m_sClanChat;

	//�޸𸮸ʿ��� ���� ����Ÿ�� ����(�޸𸮸ʰ� ũ�Ⱑ ���ƾ��Կ� ����)
	char				memMapReadData[MEMORYMAP_MAX];	
};
#endif