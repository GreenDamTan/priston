///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//���� : ĳ���� ������ Ŭ����ü�� Ż��� ó��
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __CE_DELETECHA_HPP__
#define __CE_DELETECHA_HPP__

#include "clan_Enti.h"

//ĳ���� ���� ���� Ŭ�� ó��=======================================================start

#define CLAN_DELETECHA_START					800  //ĳ���� ������ isClanmemberȣ��
#define CLAN_DELETECHA_RELEASECLAN_BEFORE        801  //Ŭ������ ��� Ŭ�� ��ü ó��
#define CLAN_DELETECHA_RELEASECLAN              802
#define CLAN_DELETECHA_SECESSION_BEFORE			803  //Ŭ������ ��� Ŭ�� Ż�� ó��
#define CLAN_DELETECHA_SECESSION				804
#define CLAN_DIRECT_DELETE					805

//ĳ���� ���� ���� Ŭ�� ó��=======================================================end

class CLANEntity_deletecha: public CLANEntity
{
public:
	CLANEntity_deletecha();
	~CLANEntity_deletecha();

	int Init(int Life);
	int Clear();

	void Main();	
	void Draw();
	void menuInit(int num);
	void SetDeleteChaInfo(char *id, char *gserver, char *chaname);

	BOOL bFlag;
public:
	inline int GetHowDeleteChaFlag(){return nHowDeleteChaFlag;}
	inline void SetHowDeleteChaFlag() {nHowDeleteChaFlag=0;}
public:
	int nHowDeleteChaFlag;
	
	
	
};


#endif 



#ifdef CE_DELETECHA_CPP

class CLANEntity_deletecha cClanDeleteCha;



#else

extern class CLANEntity_deletecha cClanDeleteCha;



#endif
