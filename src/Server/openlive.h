
typedef struct _gamedate {
	char	UserID[30];		// ����� ID
	char	GPCode[16];		// GP�ڵ� 
	char	DisuseDay[30];		// ���ᳯ¥
	char    UsePeriod[30];		// �����ð�
	char	Guid[50];		// GUID
	char	PCRno[14];		// PCRNo
} gamedate;

typedef struct _PACKET {
  int size;
  int code;
  int OpenLiveCode;
  _gamedate jungdata;
} PACKET;

//Code �ڵ�� 0x38000000
//OpenLiveCode 1  : ��������� �������� ��ȯ

//* ���� ��
typedef struct _RPACKET {
  int size;
  int code;
  char UserID[30];		// ����� ID
} RPACKET;

//Code : 0 ���� , 1 ����
//UserID : ó���� ����� ID


int	RecvOpenLivePacket( smWINSOCK *lpsmSock , void *Packet );



