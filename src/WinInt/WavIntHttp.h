//######################################################################################
//�� �� �� : 2003-10-10 ���� 4:45:50
//�� �� �� :
//��    �� :
//�� �� �� : �� �� ��
//######################################################################################
#pragma once


//
struct SWavDownUrl
{
	// �� �߰��ؾ� �ϴ� ����Ÿ ������ ����.  ��_��a
	char szRemainderUrl[128];
};


//
extern int  CreateWavIntHttp( char *lpWavUrlBasis );
extern void DestroyWavIntHttp(void);

extern int WavIntHttpPush( char *lpRemainderUrl );
//extern int WavIntHttpPop( SWavDownUrl *lpWavDownInfo );

extern void MainWavIntHttp(void);