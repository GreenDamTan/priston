/*==========================================================================
 *
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: directx.h
 *
 ***************************************************************************/

#ifndef __DXWAV_H_
#define __DXWAV_H_

// Includes....
//#include "d3drmwin.h"
#include <dsound.h>
#include <stdio.h>

// Defines.... : ������ ������ �� 15��( ref���� 0�� �������� ����ϰ� 1~8�� �޸𸮿� ���ֵ� ���)
#define NUM_SOUNDS      1700

// Structures....
#pragma pack(1)
//wav�� ������ �ٸ������ 2������ �д´�.
//DATA[..]�� ũ�Ⱑ 4�� 18�� 2���� �Ǿ��־�,
//�켱 18�� �о� DATA[0],DATA[1]�� 0�ΰ�쿡�� �״�� �а� ���� 'd','a'�̸� 
//�ٽ� ó������ DATA[4]¥���� �о ����Ѵ�.
struct WaveHeader0	
{
    BYTE        RIFF[4];          // "RIFF"
    DWORD       dwSize;           // Size of data to follow
    BYTE        WAVE[4];          // "WAVE"
    BYTE        fmt_[4];          // "fmt "
    DWORD       dw16;             // 16
    WORD        wOne_0;           // 1
    WORD        wChnls;           // Number of Channels
    DWORD       dwSRate;          // Sample Rate
    DWORD       BytesPerSec;      // Sample Rate
    WORD        wBlkAlign;        // 1
    WORD        BitsPerSample;    // Sample size
    BYTE        DATA[18];          // "DATA"=>00face.....data�� �Ǿ� �ִ�.
    DWORD       dwDSize;          // Number of Samples
};


struct WaveHeader1
{
    BYTE        RIFF[4];          // "RIFFFF"�� ��� DATA�� 18�ӿ� ����
    DWORD       dwSize;           // Size of data to follow
    BYTE        WAVE[4];          // "WAVE"
    BYTE        fmt_[4];          // "fmt "
    DWORD       dw16;             // 16
    WORD        wOne_0;           // 1
    WORD        wChnls;           // Number of Channels
    DWORD       dwSRate;          // Sample Rate
    DWORD       BytesPerSec;      // Sample Rate
    WORD        wBlkAlign;        // 1
    WORD        BitsPerSample;    // Sample size
    BYTE        DATA[4];          // "DATA"
    DWORD       dwDSize;          // Number of Samples
};


struct WaveHeader2	//DATA[16] : 'fact'�� ������.
{
    BYTE        RIFF[4];          // "RIFFFF"�� ��� DATA�� 18�ӿ� ����
    DWORD       dwSize;           // Size of data to follow
    BYTE        WAVE[4];          // "WAVE"
    BYTE        fmt_[4];          // "fmt "
    DWORD       dw16;             // 16
    WORD        wOne_0;           // 1
    WORD        wChnls;           // Number of Channels
    DWORD       dwSRate;          // Sample Rate
    DWORD       BytesPerSec;      // Sample Rate
    WORD        wBlkAlign;        // 1
    WORD        BitsPerSample;    // Sample size
    BYTE        DATA[16];          // "fact...data"
    DWORD       dwDSize;          // Number of Samples
};



#pragma pack()



// Functions ...

BOOL CreateSoundBuffer(DWORD dwBuf, DWORD dwBufSize, DWORD dwFreq, DWORD dwBitsPerSample, DWORD dwBlkAlign, BOOL bStereo, BOOL bStaticBuf);
BOOL ReadData(LPDIRECTSOUNDBUFFER lpDSB, FILE* pFile, DWORD dwSize, DWORD dwPos);
BOOL CreateBufferFromWaveFile(char* FileName, DWORD dwBuf);


// ======================= ���� ����Ͻ� �Լ����� ==============================
BOOL InitDX_Wav(HWND g_hwnd); //�ʱ�ȭ 
void TermDX_Wav();            //����ȭ
BOOL StopAllWav();            //���� �Ҹ�����




//---------- ���� : ���⿡ ���� wav ������ freq 2205*10HZ�� ���ַ� �Ǿ��־�
//           �ٸ� ������ ��� ����� �ȵ� ���ɼ��� ����
//----------------------------------------------------------------------
// 
// Function     : PlaySoundDS()
//                dwSound : wav�� ��ȣ,  dwFlags : 0-�ѹ����Ҹ�����,1-������ ���� ��ӼҸ�����
//				  dwVol  : 0~400     (�����δ� -400~0�̰� *10�Ѱ�)
//				  dwfreq : 441~4410  (�����δ� 4410~44100HZ����)
BOOL PlayWav(DWORD dwSound, DWORD dwFlags=0, DWORD dwVol=400, DWORD dwfreq=2205);

BOOL PlayWav2(DWORD dwSound, DWORD dwFlags=0, DWORD dwVol=400, DWORD dwfreq=2205);

//dwfreq : 441~4410(���ļ� 4410~44100HZ���� �����ϹǷ� 44100hz�� wav�� ���ļ��� ���̻�
			//�ö��� ����������:�����⸦ �̿��� 22050hz���� �ٿ���Ѿ���)
//dwPan : 0~800(0:���� ����Ŀ, 400:�߰� 800:������ ����Ŀ������ �Ҹ��� ������.)
BOOL PlayWav3(DWORD dwSound, DWORD dwFlags=0, DWORD dwVol=400, DWORD dwfreq=2205, DWORD dwPan=400);


//�÷��̵��߿� volume change
BOOL PlayWav_VolChange(DWORD dwSound, DWORD dwVol=400);
//�÷��̵��߿� Frequency Change
BOOL PlayWav_FrequencyChange(DWORD dwSound, DWORD dwfreq=2205);
//�÷��̵��߿� Panning Change
BOOL PlayWav_PanChange(DWORD dwSound, DWORD dwPan=400);









// ======================= ���� ����Ͻ� �Լ����� ==============================


//---------------- menu ���ÿ����� ����ϴ� �ʱ�ȭ�� ------------------//
//BOOL InitDX_Wav_MENU(HWND g_hwnd);  //�ʱ�ȭ


BOOL InitDX_Wav2(HWND g_hwnd);  //�ʱ�ȭ
void TermDX_Wav2();
BOOL CreateBufferFromWaveFile2(char* FileName, int sbufN);
BOOL CreateBufferFromWavBUF(unsigned char* wavBUf, unsigned int dwBuf);//�޸𸮷κ����б�
BOOL sBufferClear(int sbufN);

void allWAVE_playORstop(int flag);	//ȭ���� �ջ�� ���(alttab��)�� ��� �� ���

BYTE getmouTH();		//���� �������� �˱����Ѱ�


BOOL StopWav(int i);




//========== dxwav.cpp�� �� �������� �Լ���. ====================
int  BackMusic_Load( int stageNumber );     //load 1~??����
void BackMusicPlay();           //play
void BackMusicClr();            //�޸𸮷� �ε�� wav�� ���ش�.
void BackMusic_Stop();


#endif


/*
char *sangJuWav[NUM_SOUNDS] = {      //���� wav�� �̴´�.
"wav\\Ready.wav",              //���Ӹ�� ready
"wav\\go.wav",                 //���Ӹ�� gameing
"wav\\hurry.wav",              //Ư���ð��ȿ� �ȴ����� �ʴ��ѹ��� ���´�.
"wav\\se1.WAV",               //������ ���
"wav\\gameover.wav",			//game��������.
"wav\\se2.WAV",               //������ ���
"wav\\se6.WAV",               //�������������
};
*/

#define WAV_READY()     PlayWav(1)		//�� ����
#define WAV_CLOCK()     PlayWav(2)		//�ð��б�
#define WAV_SEND()      PlayWav(3)		//��

#define WAV_CLEAVE()    PlayWav(4)		//������
#define WAV_DRAP()      PlayWav(5)		//��������
#define WAV_BANSA()     PlayWav(6)		//�������鼭 ���� �΋H�� �ݻ�ɽ�..



#define WAV_VICTOR1()	PlayWav(7)	//"pbdou\\�¸�_�ڼ�.wav",	//������ ����϶�
#define WAV_VICTOR2()	PlayWav(8)	//"pbdou\\�¸�1.wav.wav",	

#define WAV_LOSS1()	PlayWav(9)	//"pbdou\\�й�1.wav",					//game��������.
#define WAV_LOSS2()	PlayWav(10)	//"pbdou\\�й�2.wav",
#define WAV_LOSS3()	PlayWav(11)	//"pbdou\\�й�3.wav",
#define WAV_LOSS4()	PlayWav(12)	//"pbdou\\�й�4.wav",


#define WAV_ENBALL1()	PlayWav(13)		//"������1.wav",//���ǰ��� ���� �ö󰥶�..
#define WAV_ENBALL2()	PlayWav(14)		//"������2.wav",

#define WAV_BALLDOWN1() PlayWav(15)		//"������߸�1����!.wav",//������߸��� ���� �������� ����.
#define WAV_BALLDOWN2() PlayWav(16)		//"������߸�2�Ϳ�1.wav",

#define WAV_LINEDOWN()	PlayWav(17)		//"����down.wav",
#define WAV_JOJUN()		PlayWav(18)		//"���ر�.wav",


#define WAV_WIN_M()		PlayWav(19)		//"wav\\win_m.wav",		//�̰����� ����
#define WAV_LOSE_M()		PlayWav(20)		//"wav\\lose_m.wav",	//������ ����.









BOOL Init_BGM(HWND hwnd);		//init
void Uninit_BGM(void);			//uninit
BOOL OpenBGM(LPSTR lpszFileName);	//bgm �
void PlayBGM(void);
void StopBGM(void);
void SetVolumeBGM( DWORD dwVol );		//���� ����

//�Ҹ��� ���������� Ȯ��
BOOL IsPlayWav( DWORD dwSound );

void InitDirectSound();	// pluto ��Ÿ���� ä�� ������ ������ �������� ���� �����


/*

class KSound
{
public:
    HWND    m_hWnd;

	LONG  m_lInTimer        ;
	BOOL  m_bTimerInit      ;
	BOOL  m_bPlaying        ;
	BOOL  m_bIsCloseOnRun   ;
	BOOL  m_bFileOpen       ;
	UINT  m_uTimerID        ;
	DWORD m_SecondBuffFocus ;

	//WAVEINFOCA  m_wiWave;



    //HRESULT CreateDirectSoundBuffer(LSFILEINFO*);
    //void    ReleaseDirectSoundBuffer(LSFILEINFO*);
    BOOL    IsPCMWave(LPSTR);

    HRESULT SetupStreamBuffer(LPSTR);


    BOOL  Init(HWND);
    void  Uninit(void);
    BOOL  OpenBGM(LPSTR);
    //LSFILEINFO *OpenFX(LPSTR);
    void  PlayBGM(void);
    void  StopBGM(void);
    //BOOL  PlayFX(LSFILEINFO*);
    //BOOL  StopFX(LSFILEINFO*);
    //BOOL  Volume(LSFILEINFO*);
    //BOOL  Pan(LSFILEINFO*);
    //BOOL  Freq(LSFILEINFO *pFileInfo);
};

*/