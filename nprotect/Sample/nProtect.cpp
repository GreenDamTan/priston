//#include "nProtect.h"

#include <windows.h>
#include <process.h>
#include "psapi.h"
#include "nprotect.h"

class nP nPro;



/*	ȭ���̸�	��ȣȭ�Ⱥ���	��ȣȭǬ����
findhack.exe	securetext[0]	pass_char[0]
NPCHK.DLL		securetext[1]	pass_char[1]
CheckCRC		securetext[2]	pass_char[2]
npmon.exe		securetext[3]	pass_char[3]
NPPSK.DLL		securetext[4]	pass_char[4]
KillTask		securetext[5]	pass_char[5]
*/


char securetext[6][20]={	//��ȣȭ�Ⱥ���
	{-103,-106,-111,-101,-105,-98,-100,-108,-47,-102,-121,-102,-1,-1,-1,-1,-6,-1,-1,-1},
	{-79,-81,-68,-73,-76,-47,-69,-77,-77,-1,120,101,0,0,0,0,5,0,0,0},
	{-68,-105,-102,-100,-108,-68,-83,-68,-1,0,-121,-102,-1,-1,-1,-1,-6,-1,-1,-1},
	{-111,-113,-110,-112,-111,-47,-102,-121,-102,-1,120,101,0,0,0,0,5,0,0,0},
	{-79,-81,-81,-84,-76,-47,-69,-77,-77,-1,-121,-102,-1,-1,-1,-1,-6,-1,-1,-1},
	{-76,-106,-109,-109,-85,-98,-116,-108,-1,0,120,101,0,0,0,0,5,0,0,0}};

char pass_char[6][20];	//��ȣȭǬ����

//BOOL CheckCRC(char * FilePath);
//typedef BOOL(MYCHECKCRC)(char *);
//MYCHECKCRC *pCheckCRC;

nP::nP()
{
	
}

nP::~nP()
{

}




//////////////////// ��ȣȭ �Ⱥκ��� ���������� �����ִ� �Լ�///////////////////////
void nP::Init()
{	
	for(int j=0;j<6;j++) for(int i=0;i<20;i++) pass_char[j][i] = securetext[j][i] ^ 0xff;
}



/*
Return ���� ���� �����

0	:	������ ó��
1	:	Can't Load Dll
2	:	Can't Load Funcation
3	:	Alert!! Modified File
4	:	Can't Free Dll
5	:	Error Kill
*/




////////////////////////////// nProtect �� ȭ�� �������� üũ���ִ� �Լ�//////////////////
int nP::NProtectCheck(void)
{
	nPro.Init();		// ��ȣȭ �κ� �ʱ�ȭ

	char sz_SystemFolder[256], sz_buf[256];
	
	GetSystemDirectory(sz_SystemFolder, 256);	//�ý��� ������ ���´�
	strcpy(sz_buf,sz_SystemFolder);
	strcat(sz_buf,"\\");
	strcat(sz_buf,pass_char[3]);

	// �ý��� ������ npmon.exe�� ���������� Ȯ���Ѵ�.
	// ���ϰ��� 1�̸�(TRUE) ����, 0�̸�(FALSE) �������� �˷��ش�.
	if(!CheckCRC(sz_buf))
	{
		// ������ �Ǿ���
		return 3;
	}
	else
		return 0;
		

	// FindHack.exe�� ���������� Ȯ���Ѵ�.
    // ���ϰ��� 1�̸�(TRUE) ����, 0�̸�(FALSE) �������� �˷��ش�.	
	if(CheckCRC(pass_char[0]))
	{
		// ������ �Ǿ���
		return 3;
	}
	else
		return 0;

	//CheckCRC("c:\windows\system32\npmon.exe");
	//CheckCRC("findhack.exe");
	if(m_hInstDll == NULL)
	{
		m_hInstDll = LoadLibrary(pass_char[1]);
		if(m_hInstDll == NULL)	//����ó��
		{
			// dll�� �ε尡 �ȉ�����
			return 1;
		}
		else
		{
 			pFunc_npchk = (bool(*)(char*))GetProcAddress(m_hInstDll,pass_char[2]);

			if(pFunc_npchk == NULL)
			{
				//MessageBox(NULL,"Can't Load Funcation","Can't Load",MB_OK);
				return  2;
			}
			//UpdateData(true);		<< �̺κ��� nProtect �翡 �������Դϴ�
			if(pass_char[0] !="")		// finehack.exe üũ
			{
				if(pFunc_npchk(pass_char[0])){
					//üũ�� �ؼ� ������ �������� ó��
					//MessageBox(NULL,"Safe File","Safe",MB_OK);
				}
				else {
					// üũ�� ������ ȭ�� ������ ��������� ó��
					//MessageBox(NULL,"Alert!! Modified File","Alert!!",MB_OK);
					return 3;
				}
			}
		}
	}

	if(m_hInstDll != NULL)
	{
		if(FreeLibrary(m_hInstDll)){
			m_hInstDll = NULL;
			// dll�� �޸𸮻󿡼� �����ִ� �κ�
			//MessageBox(NULL,"Free Dll","Free",MB_OK);
		}
		else {
			// dll�� �޸𸮻󿡼� ������ �� �������� ó��
			//MessageBox(NULL,"Can't Free Dll","Can't Free",MB_OK);
			return 4;
		}
	}
	return 0;
}

/*
int nP::NProtectCheckMON(void)
{
	nPro.Init();		// ��ȣȭ �κ� �ʱ�ȭ

	char *test="C:\WINDOWS\system32\NPMON.exe";
	if(m_hInstDll == NULL)
	{
		m_hInstDll = LoadLibrary(pass_char[1]);
		if(m_hInstDll == NULL)	//����ó��
		{
			// dll�� �ε尡 �ȉ�����
			return 1;
		}
		else
		{
 			pFunc_npchk = (bool(*)(char*))GetProcAddress(m_hInstDll,pass_char[2]);

			if(pFunc_npchk == NULL)
			{
				//MessageBox(NULL,"Can't Load Funcation","Can't Load",MB_OK);
				return  2;
			}
			//UpdateData(true);		<< �̺κ��� nProtect �翡 �������Դϴ�
			if(test !="")		// npmon.exe üũ
			{
				if(pFunc_npchk(test)){
					//üũ�� �ؼ� ������ �������� ó��
					//MessageBox(NULL,"Safe File","Safe",MB_OK);
				}
				else {
					// üũ�� ������ ȭ�� ������ ��������� ó��
					//MessageBox(NULL,"Alert!! Modified File","Alert!!",MB_OK);
					return 3;
				}
			}
		}
	}

	if(m_hInstDll != NULL)
	{
		if(FreeLibrary(m_hInstDll)){
			m_hInstDll = NULL;
			// dll�� �޸𸮻󿡼� �����ִ� �κ�
			//MessageBox(NULL,"Free Dll","Free",MB_OK);
		}
		else {
			// dll�� �޸𸮻󿡼� ������ �� �������� ó��
			//MessageBox(NULL,"Can't Free Dll","Can't Free",MB_OK);
			return 4;
		}
	}
	return 0;

 }
*/






////////////////////////   nProtect�� ���� �Լ�  /////////////////////////////
/*
nProtect �ε�� ���ϰ��� �����
1024 : ���ܰ�� �޸𸮻� ��ŷ���� �������� �������
1025 : ���ܰ�� �޸𸮻� ��ŷ���� �����ϳ� ���������� ġ���������
1026 : ���ܰ�� �޸𸮻��� ��ŷ���� ���������� ����ڰ� ġ�Ḧ �������� �ʰų� ���α׷����� ġ�Ḧ ���������� ���� ���������
1027 : ��ŷ�� ���� ���α׷��� ���������� �ٿ�ε� ���� �ʾ������. URL�� �߸��ǰų� ������ ���������� �������� ���� ���
1028 : NPX.DLL��� ���� �� nProtect ������ �ʿ��� ������ �������
1029 : ���α׷������� ���ܻ����� �߻����� ���
1030 : ����ڰ� "����" ��ư�� Ŭ������ ����� ó����
*/

int nP::NProtectRun(void)		// nProtect�� ����
{
	nPro.Init();		// ��ȣȭ �κ� �ʱ�ȭ
	// findhack.exe�� �����Ű�� �κ�
	result = (int)spawnl(P_WAIT,pass_char[0],pass_char[0],NULL);
	return result;

}





/////////////////////////////////// nProtect�� �޸𸮿����� �����ִ� �Լ� ////////////////////

int nP::NProtectFree(void)		
{
	nPro.Init();		// ��ȣȭ �κ� �ʱ�ȭ
	
	if(m_hInstDll == NULL)
	{
		m_hInstDll = LoadLibrary(pass_char[4]);
		if(m_hInstDll == NULL)
		{
			// dll�ε尡 �ȉ����� ����ó��
			//MessageBox(NULL,"Can't Load Dll","Error",MB_OK);
			return 1;
		}
		else
		{
			// dll�ε尡 ���������� �̷�� ������
 			//MessageBox(NULL,"Success Load Dll","Success",MB_OK);
			
			pFunc_npkill = (int(*)(char*))GetProcAddress(m_hInstDll,pass_char[5]);

			if(pFunc_npkill == NULL)
			{
				//MessageBox(NULL,"Can't Load Funcation","Can't Load",MB_OK);
				return 2;
			}
			//UpdateData(true);
			if(pass_char[3] !="")		// �޸𸮻� npmon.exe�� �پ����ִ��� üũ
			{
				temp = pFunc_npkill(pass_char[3]);

				if(temp !=0) {
					// �޸𸮻��� npmon.exe�� ���������� Remove��������
					//MessageBox(NULL,"Sucess Kill","Sucess",MB_OK);
				}
				else {
					// �޸𸮻��� npmon.exe�� Remove�� �����Ͽ�����
					//MessageBox(NULL,"Error Kill","Error",MB_OK);
					return 5;
				}
			}
		}
	}

	if(m_hInstDll != NULL)
	{
		if(FreeLibrary(m_hInstDll)){
			m_hInstDll = NULL;
			// dll�� �޸𸮻󿡼� �����ִ� �κ�
			//MessageBox(NULL,"Free Dll","Free",MB_OK);
		}
		else {
			// dll�� �޸𸮻󿡼� ������ �� �������� ó��
			return 4;
		}
	}
	return 0;
}







////////////////////////   nProtect�� ������ NPMON.EXEȭ���� �˻��ϴ� �Լ�  /////////////////////////////
/*
nProtect����� NPMON.EXE üũ�� �ش��ϴ� ���ϰ��� �����
0	:	�޸𸮿� �ִ°�� (�ε�Ȱ��)
1	:	�޸𸮿� ���°�� (�� �ε�Ȱ��)
*/

int nP::findNPMON()
{
	char findnpmon_secure[2][20]={	//��ȣȭ�Ⱥ���
	{-88,-88,-88,-47,-79,-81,-83,-80,-85,-70,-68,-85,-47,-68,-80,-78,-1,-1,-1,-1},
	{-85,-103,-115,-110,-79,-81,-78,-80,-79,-1,67,84,46,67,79,77,0,0,0,0}
	};
	char findnpmon_read[2][20];	//��ȣȭǬ����

	for(int j=0;j<2;j++) for(int i=0;i<20;i++) findnpmon_read[j][i] = findnpmon_secure[j][i] ^ 0xff;

	HWND hwnd;

    hwnd = FindWindow(findnpmon_read[1], findnpmon_read[0]);
    if (hwnd != 0) {
		 //MessageBox(NULL,"������","Ȯ��",MB_OK);
		 return 0;
	
    } else {
		//MessageBox(NULL,"�� ������","Ȯ��",MB_OK);
		return 1;
   }

	return 0;
}

BOOL nP::CheckCRC(char * FilePath)
{
	BOOL dOutput;
	HINSTANCE m_hDll;

	if ((m_hDll = LoadLibrary("npchk.dll")) == NULL)
		MessageBox(NULL,"npchk.dll ������ ã���� �����ϴ�.","error",MB_OK);

	pCheckCRC = (MYCHECKCRC *)GetProcAddress(m_hDll, "CheckCRC");
	dOutput = (*pCheckCRC)((LPTSTR)(LPCTSTR)FilePath);

	if(dOutput)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
