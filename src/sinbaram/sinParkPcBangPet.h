/*----------------------------------------------------------------------------*
*	���ϸ� :  sinParkPcBangPet.h
*	�ϴ��� :  PCBang ���� �ͱٴ� 
*	�ۼ��� :  ����������Ʈ 3�� 
*	������ :  �ڻ� // ��öȣ ����
*-----------------------------------------------------------------------------*/	
#ifndef _SINPCBANGPET_HEADER_

#define _SINPCBANGPET_HEADER_

#include <string>
#include <map>
#include <vector>

//pet message list
typedef vector<string*> PetMessage;
typedef map< string , PetMessage> PetmsgMap;
typedef vector<int> MessageList;
typedef list<string> MessageKeep;
typedef list<int> MessageIndex;

/*----------------------------------------------------------------------------*
*							cHELPPET 	class		
*-----------------------------------------------------------------------------*/	
class cPCBANGPET{

public:
	PetmsgMap	PetMsg_Map;
	MessageList BattleM_List;
	MessageKeep Message_Keep;
	MessageIndex BattleMsg_Index;
	MessageIndex NormalMsg_Index;

	///////////// �� 
	smPAT3D		*PatPetMesh_Bone; //Pet�� Bone
	smPAT3D		*PatPetMesh; //Pet��

	///////////// ��ǥ , Angle 
	POINT3D		PetPosi;	   //Pet��ǥ
	POINT3D		PetAngle;	   //Pet�ޱ�
	POINT3D     PetRendPoint;  //ȭ�� Point��ǥ
	RECT		PetRendRect2D; //ȭ�� Rect;	

	POINT3D		CameraPetAngle;	//ī�޶� Anlge
	POINT3D     CameraPetPosi;  //ī�޶� Posi

	///////////// �ִϸ��̼�
	int		AniCount;
	int		AniMaxCount;
	int     AniTime;

	//////////// �÷�
	sASSACOLOR Color;

	//////////// �޽�������
	char szPetBuff[256];
	int MsgFlag;  //�޼��� flag�� ������� ȣ�������ʴ´�
	int MsgTime;  //�ð��� ��Ÿ����
	int MessageIndex; //0 �޼���ID 1 �޼��� �ε���
	char szTempId[32];

	int HelpMsgFlag;  //�޼��� flag�� ������� ȣ�������ʴ´�
	int HelpMsgTime;  //�ð��� ��Ÿ����
	char HelpMsgId[32];

	/////////// Pet����
	int PetKind; //1 ȭ��(�׸�) 2 ����(�ܽý�) 3ǳ��(�̿�) 4����(��Ʈ)
	int PetShow;

	/////////// ���� �޼������� ����ġ
	int PetExp;
	int LevelFlag;

	/////////// �ε� ����
	int LoadingPet;
	
public:
	cPCBANGPET(){};
	~cPCBANGPET(){};

	void Init(); //Ŭ���� �ʱ�ȭ
	void Load();
	void Release();
	void Draw();
	void Draw(POINT3D *pPosi , POINT3D *pAngle);
	void DrawText();
	void Close();//Ŭ���� ���� 
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); //Ű üũ 

	//�޼��� �Ľ�
	int GetFileData();

	//Pet�޼����� �����Ѵ�
	int PetMessage(char *Id , int Index);

	//Pet�޼��� ������ �����ش� (���)
	int PetMessageHelp(char *Id); //��� ���� �޼���

	//Pet 2D��ǥ�� ���Ѵ�
	int GetPetMessagePosi();

	//Pet �޼����� �׷��ش�
	int DrawPetMessage();

	//Pet �� ������ üũ�Ѵ�
	int CheckPetData();

	//Random �޼���
	int PetRandomBattleMessage();
	int PetRandomNormalMessage();

	//Pet�� ȣ���Ѵ�
	int ShowPet();
	int ClosePet();

	int PetOnOff(char *szBuff);

};

extern cPCBANGPET cPCBANGPet;
//extern int PetMessageOkButtonFlag;




//class cSINPET  cSINBARAM_UI


#endif
