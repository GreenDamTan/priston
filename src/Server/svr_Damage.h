//���� ���� ����
int rsRecvAttackData_Old( rsPLAYINFO *lpPlayInfo , TRANS_ATTACKDATA *lpTransAttackData , smCHAR *lpTargetChar , int Type );
//���� ������ ����
int rsRecvRangeAttackData_Old( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA *lpTransSkilAttackData , int Type );
//���� ������ ���� ����
int rsRecvRangeAttackUserData_Old( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA *lpTransSkilAttackData );


//���� ���� ����
int rsRecvAttackData( rsPLAYINFO *lpPlayInfo , TRANS_ATTACKDATA2 *lpTransAttackData );
//�������� ���� ����
int rsRecvRangeAttackData( rsPLAYINFO *lpPlayInfo , TRANS_SKIL_ATTACKDATA2 *lpTransRangeAttackData );;

//���� ���� ýũ��
DWORD	dm_GetRangeDamgeChkSum( TRANS_SKIL_ATTACKDATA2 *lpTransSkilAttackData2 );
//���� ���� ýũ��
DWORD	dm_GetDamgeChkSum( TRANS_ATTACKDATA2 *lpTransAttackData2 );
//�ڸǵ� ýũ��
DWORD	dm_GetCommandChkSum( smTRANS_COMMAND *lpTransCommand , DWORD dwObjectSerial );

//������ �������� ��ų �ڵ����� �˻�
int rsCheckSkillLevel( rsPLAYINFO *lpPlayInfo , DWORD dwSkillCode );

//������ ��ȣ Ǯ�� �Լ�
int	rsDecodeDamagePacket( rsPLAYINFO *lpPlayInfo , void *lpPacket );
//������ ��ȣ ���� �Լ�
int	rsEncodeDamagePacket( rsPLAYINFO *lpPlayInfo , void *lpPacket );
//������ ��ȣȭ ����� ��ä�� Ŭ���̾�Ʈ�� ������
int rsSendDamageFuncToClient( rsPLAYINFO *lpPlayInfo );
//������ ��ȣȭ ����� ��ä�� Ŭ���̾�Ʈ�� ������
int rsSendDamageFuncToClient2( rsPLAYINFO *lpPlayInfo );
//������ ��ȣȭ Ű����
int rsSetDamagePacketKey( rsPLAYINFO *lpPlayInfo );
//��ȣ ��� ����
int	rsProcessSafePacket( rsPLAYINFO *lpPlayInfo );
//��ȣ ������� Ȯ���Ͽ� ����
int	rsCompareSafePacket( rsPLAYINFO *lpPlayInfo , DWORD dwRcvPacketCode );

//������ ����Ÿ ������ ������ �α׿� ���
int rsRecordDamageError( rsPLAYINFO *lpPlayInfo , smTRANS_COMMAND *lpTransCommand );
