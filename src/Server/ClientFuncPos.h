//Ŭ���̾�Ʈ ��� ������ �߰�
int	rsAddClientFuncPos( TRANS_CLIENT_FUNPOS	*lpClentFuncPos );
//Ŭ���̾�Ʈ ��� ������ �Լ� ( ���Ϸ� ���� )
int	rsRecvClientFuncPos( TRANS_CLIENT_FUNPOS	*lpClentFuncPos );
//Ŭ���̾�Ʈ ��� ������ ���
TRANS_CLIENT_FUNPOS *rsGetClientPos( int Version );
//Ŭ���̾�Ʈ ��� ������ ����
int	rsResetClientFuncPos();

//�޸� ýũ�� ����� ��ä�� Ŭ���̾�Ʈ�� ������
int rsSendFuncMemToClient( rsPLAYINFO *lpPlayInfo , DWORD dwFuncMem , DWORD	dwLen );
//üũ�� ��
int	rsCompClientFuncMem( rsPLAYINFO *lpPlayInfo , DWORD	dwFunc , DWORD dwChkSum );
//üũ�� ��2
int	rsCompClientFuncMem2( rsPLAYINFO *lpPlayInfo , DWORD dwFuncCode );
//Ŭ���̾�Ʈ �޸� ýũ ��� ����
int	rsCheckClientFuncMem( rsPLAYINFO *lpPlayInfo );
