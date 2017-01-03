/***************************************************************************
 *
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: directx.cpp
 *
 ***************************************************************************/
#include "dxWav.h"
#include <windowsx.h>
#include <mmsystem.h>
#include <conio.h>
//#include "..\cl_main\defines.h"
//#include "..\cl_main\initgd.h"

HRESULT     rval_g_lpDs;				// pluto ��Ÿ���� ä�� ������ ������ �������� ���� �����
HRESULT     dsrval_lpDirectSound;		// pluto ��Ÿ���� ä�� ������ ������ �������� ���� �����

int numSound = 0;

char dataDir[64];

//mouth����
int mouThRead;
BYTE *mouThdataOfs = NULL;

BOOL wav_opt = 1;           //wav option check(Dxwav.cpp)

LPDIRECTSOUND           g_lpDS = NULL;              // DirectSound object
BOOL                    g_bSoundPresent = FALSE;    // Do we have sound capabilites?
LPDIRECTSOUNDBUFFER     g_lpSounds[NUM_SOUNDS];     // Sound buffers
BOOL                    g_bSoundPaused = FALSE;     // Defined in WINMAIN.CPP


char sangJuWav[NUM_SOUNDS][64];
int  sangJuWav_F[NUM_SOUNDS] = {1,};		//�÷��׼���

/*
int chk_load_sangJuWav(char *wavName) 
{
	int i;
	for(i=0; i<NUM_SOUNDS; i++) {
		if(sangJuWav[i][0]==wavName[0]) {
			if ( _stricmp(sangJuWav[i], wavName) == 0 ) {
				if(sangJuWav_F[i] == 1) return 1;			//�̹� �ε��

				sangJuWav_F[i] = 1;		//�÷��� ����
				CreateBufferFromWaveFile(sangJuWav[i], i);
				return 1;
			}
		}
		
	}
	return -1;
}
*/


//�÷��׸� �̿��� �߰� �ε���.
int chk_load_sangJuWav(int wavN) 
{
	if(sangJuWav_F[wavN-1] == 1) return 1;			//�̹� �ε��
	sangJuWav_F[wavN-1] = 1;		//�÷��� ����
	CreateBufferFromWaveFile(sangJuWav[wavN-1], wavN);
	
	return 1;
}


char *sangJuWav2[NUM_SOUNDS]= {
"wav\\rain01.wav",
"wav\\thunder1.wav",
"wav\\Button.wav",
"wav\\New.wav",
"Wav\\CharacterDelete.wav",
"Wav\\ButtonBack.wav",
"Wav\\CharacterSelect.wav",

"Wav\\NewChar.wav",
"Wav\\Fire.wav",
"Wav\\Thunder.wav",
"Wav\\GameStart.wav",
"Wav\\Login.wav",
NULL
};

char *wnameBuf[NUM_SOUNDS]= {
"wav\\bgm0.wav",
"wav\\rain01.wav",
"wav\\thunder1.wav",
"wav\\step01.wav",
"Wav\\Effects\\weapon\\one hand swing - small 3.wav",
    NULL
};

int isPlaying(int i);

//=============== ������ ���� �Լ� ========================//
char *backMusicWav[] = {   //�ӽ÷� ��������.
"wav\\622s.wav",	//0
NULL,
NULL,
};

/*///////////////////////////////////////////////////////////////////////////
: �Լ�  �� :  int  BackMusic_Load( int stageNumber )
: �ϴ�  �� :  wav�� 0�����۷� ��wav�� �̸� �ε� ���ѵ�.
: ��    �� :  wavfile name�̳� ���۷κ��� wav����Ÿ�� �ε��ϴ� �Լ�
: ��    �� :  ���� : ������ wav�� 0�� ���۷� �ε��ϰԵ������� ����
: ��ȯ  �� :  InitDX_wav2�Լ��� �̿��� �̸� ������ ���ѵ��� ����� �ؾ���.(winmain.cpp)
: ��    �� :
///////////////////////////////////////////////////////////////////////////*/
int  BackMusic_Load( int stageNumber ) //1���������� ���ͽ����Կ� ����
{
	if(g_bSoundPresent == FALSE) return 0;

    char strFilename[128];
    strcpy( strFilename, dataDir);

    strcat( strFilename, backMusicWav[stageNumber] );

    if( !CreateBufferFromWaveFile(strFilename, 0) )
    {
        return FALSE;
    }
    return 1;
}

void BackMusicPlay()
{
    PlayWav(0, 1);
}

//�޸𸮷� �ε�� wav�� ���ش�.
void BackMusicClr()
{
	StopAllWav();
    sBufferClear(0);
}

void PlayWavEffect(int num)
{
    PlayWav(num);
}

void BackMusic_Stop()
{
	StopWav(0);
}



//----------------------------------------------------------------------
// 
// Function     : InitDirectX()
//
// Purpose      : Initialises DirectX (DirectDraw, Direct3D, Direct3DRM, DirectSound)
//
//----------------------------------------------------------------------
BOOL InitDX_Wav(HWND g_hwnd)  //�ʱ�ȭ
{
    if(!wav_opt) {
            g_bSoundPresent = FALSE;
            return 0;
    }

	int i;
    //FILE        *fp;
    //BYTE        pal[768];
    //HRESULT     rval;
    //DDSURFACEDESC ddsd;

    // Create DirectSound object
    //rval = DirectSoundCreate(NULL, &g_lpDS, NULL);	// pluto ��Ÿ���� ä�� ������ �Ű��

    // Determine whether sound is present
    if(rval_g_lpDs == DS_OK)
        g_bSoundPresent = TRUE;
    else
        g_bSoundPresent = FALSE;
    //g_bSoundPresent = rval == DS_OK ? TRUE : FALSE;

    if (g_bSoundPresent)
    {
        // Set the DirectSound cooperative level
        //TRY_DS(g_lpDS->SetCooperativeLevel(g_hWnd, DSSCL_NORMAL))

        g_lpDS->SetCooperativeLevel(g_hwnd, DSSCL_NORMAL);

        // Null out all the sound pointers
        for (i = 0; i < NUM_SOUNDS; i ++)
        {
            g_lpSounds[i] = NULL;
        }

        for(i=0; i<NUM_SOUNDS; i++)
        {
            if( wnameBuf[i] != NULL) {
                if (!CreateBufferFromWaveFile(wnameBuf[i], i))
                    //RegError("Couldn't load INTRO.WAV!");
                    return FALSE;
                numSound++;
            }
        }
    }
    // Yahoo!
    return TRUE;
}


//----------------------------------------------------------------------
// 
// Function     : TermDX_Wav();
//
// Purpose      : Destroys all the DirectX sound objects
//
//----------------------------------------------------------------------
void TermDX_Wav()
{
    if (!g_bSoundPresent) return;

    // Destroy sounds
    for (int i = 0; i < NUM_SOUNDS; i ++)
    {
        if (g_lpSounds[i])
        {
            g_lpSounds[i]->Release();
            g_lpSounds[i] = NULL;
        }
    }

    // Destroy DirectSound object
    if (g_lpDS)
    {
        g_lpDS->Release();
        g_lpDS = NULL;
    }
}


//----------------------------------------------------------------------
//
// Function     : CreateSoundBuffer()
//
// Purpose      : Creates a DirectSound buffer
//
//----------------------------------------------------------------------
BOOL CreateSoundBuffer(DWORD dwBuf, DWORD dwBufSize, DWORD dwFreq, DWORD dwBitsPerSample, DWORD dwBlkAlign, BOOL bStereo)
{
    PCMWAVEFORMAT pcmwf;
    DSBUFFERDESC dsbdesc;
    
    // Set up wave format structure.
    memset( &pcmwf, 0, sizeof(PCMWAVEFORMAT) );
    pcmwf.wf.wFormatTag         = WAVE_FORMAT_PCM;      
    pcmwf.wf.nChannels          = bStereo ? 2 : 1;
    pcmwf.wf.nSamplesPerSec     = dwFreq;
    pcmwf.wf.nBlockAlign        = (WORD)dwBlkAlign;
    pcmwf.wf.nAvgBytesPerSec    = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
    pcmwf.wBitsPerSample        = (WORD)dwBitsPerSample;

    // Set up DSBUFFERDESC structure.
    memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));  // Zero it out. 
    dsbdesc.dwSize              = sizeof(DSBUFFERDESC);
    //dsbdesc.dwFlags             = DSBCAPS_CTRLDEFAULT;  // Need default controls (pan, volume, frequency).
    dsbdesc.dwFlags             = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
		//DSBCAPS_CTRLDEFAULT;  // Need default controls (pan, volume, frequency).
    dsbdesc.dwBufferBytes       = dwBufSize; 
    dsbdesc.lpwfxFormat         = (LPWAVEFORMATEX)&pcmwf;

    g_lpDS->CreateSoundBuffer(&dsbdesc, &g_lpSounds[dwBuf], NULL);
    return TRUE;
}

//----------------------------------------------------------------------
// 
// Function     : ReadData()
//
// Purpose      : Reads in data from a wave file
//
//----------------------------------------------------------------------
BOOL ReadData(LPDIRECTSOUNDBUFFER lpDSB, FILE* pFile, DWORD dwSize, DWORD dwPos)
{
    // Seek to correct position in file (if necessary)
    if (dwPos != 0xffffffff) 
    {
        if (fseek(pFile, dwPos, SEEK_SET) != 0) 
        {
            return FALSE;
        }
    }

    // Lock data in buffer for writing
    LPVOID pData1;
    DWORD  dwData1Size;
    LPVOID pData2;
    DWORD  dwData2Size;
    HRESULT rval;
    rval = lpDSB->Lock(0, dwSize, &pData1, &dwData1Size, &pData2, &dwData2Size, DSBLOCK_FROMWRITECURSOR);
    if (rval != DS_OK)
    {
        return FALSE;
    }

    // Read in first chunk of data
    if (dwData1Size > 0) 
    {
        if (fread(pData1, dwData1Size, 1, pFile) != 1) 
        {               
            return FALSE;
        }
		//ktj :���콺���� �ӽ÷� ������
		if(mouThRead ==0) {
			mouThdataOfs = (BYTE*)pData1;
		}

    }
    // read in second chunk if necessary
    if (dwData2Size > 0) 
    {
        if (fread(pData2, dwData2Size, 1, pFile) != 1) 
        {
            return FALSE;
        }
    }

    // Unlock data in buffer
    rval = lpDSB->Unlock(pData1, dwData1Size, pData2, dwData2Size);
    if (rval != DS_OK)
    {
        return FALSE;
    }

    // Yahoo!
    return TRUE;
}

//----------------------------------------------------------------------
// 
// Function     : CreateSoundBufferFromWaveFile()
//
// Purpose      : Creates a DirectSound buffer from a wave file
//
//----------------------------------------------------------------------

BOOL CreateBufferFromWaveFile(char* FileName, DWORD dwBuf)
{
    DWORD dwSize;
    BOOL bStereo;
	// Open the wave file       
    FILE* pFile = fopen(FileName,"rb");
    if (pFile == NULL) return FALSE;

    // Read in the wave header
    WaveHeader1 wavHdr1;    //DATA[4]
    WaveHeader2 wavHdr2;    //DATA[16]

    WaveHeader0 wavHdr;	//�ϴ� DATA[18]�� �д´�.
    if (fread(&wavHdr, sizeof(wavHdr), 1, pFile) != 1) 
    {
        fclose(pFile);
        return NULL;
    }

	if(wavHdr.DATA[0]=='d' && wavHdr.DATA[1] == 'a') {

		fseek( pFile, 0, SEEK_SET);	//�ٽ� ó������ DATA[4]�� �д´�.
		fread(&wavHdr1, sizeof(wavHdr1), 1, pFile);
	    dwSize = wavHdr1.dwDSize;
	    // Is this a stereo or mono file?
		bStereo = wavHdr1.wChnls > 1 ? TRUE : FALSE;
		// Create the sound buffer for the wave file
		if (!CreateSoundBuffer(dwBuf, dwSize, wavHdr1.dwSRate, wavHdr1.BitsPerSample, wavHdr1.wBlkAlign, bStereo))
		{
			// Close the file
			fclose(pFile);
	        return FALSE;
		}
		if ( !g_lpSounds[dwBuf] ) {
			fclose(pFile);
			return FALSE;
		}
		//���콺�� �о���ϱⶫ�� �ӽ÷� ������
		mouThRead = dwBuf;
		// Read the data for the wave file into the sound buffer
		if (!ReadData(g_lpSounds[dwBuf], pFile, dwSize, sizeof(wavHdr1))) 
		{
	        fclose(pFile);
		    return FALSE;
		}
	}
	else if(wavHdr.DATA[0]=='f' && wavHdr.DATA[1] == 'a') {

		fseek( pFile, 0, SEEK_SET);	//�ٽ� ó������ DATA[16]�� �д´�.
		fread(&wavHdr2, sizeof(wavHdr2), 1, pFile);

	    dwSize = wavHdr2.dwDSize;
	    // Is this a stereo or mono file?
		bStereo = wavHdr2.wChnls > 1 ? TRUE : FALSE;

		// Create the sound buffer for the wave file
		if (!CreateSoundBuffer(dwBuf, dwSize, wavHdr2.dwSRate, wavHdr2.BitsPerSample, wavHdr2.wBlkAlign, bStereo))
		{
			// Close the file
			fclose(pFile);
	        return FALSE;
		}

		//���콺�� �о���ϱⶫ�� �ӽ÷� ������
		mouThRead = dwBuf;
		// Read the data for the wave file into the sound buffer
		if (!ReadData(g_lpSounds[dwBuf], pFile, dwSize, sizeof(wavHdr2))) 
		{
	        fclose(pFile);
		    return FALSE;
		}
	}
	else {

	    // Figure out the size of the data region
		dwSize = wavHdr.dwDSize;
		// Is this a stereo or mono file?
	    bStereo = wavHdr.wChnls > 1 ? TRUE : FALSE;

		// Create the sound buffer for the wave file
		if (!CreateSoundBuffer(dwBuf, dwSize, wavHdr.dwSRate, wavHdr.BitsPerSample, wavHdr.wBlkAlign, bStereo))
		{
			// Close the file
			fclose(pFile);
	        return FALSE;
		}

		//���콺�� �о���ϱⶫ�� �ӽ÷� ������
		mouThRead = dwBuf;
		// Read the data for the wave file into the sound buffer
		if (!ReadData(g_lpSounds[dwBuf], pFile, dwSize, sizeof(wavHdr))) 
		{
			fclose(pFile);
			return FALSE;
		}
	}

    // Close out the wave file
    fclose(pFile);
    // Yahoo!
    return TRUE;
}







//������ �ƴ� �޸𸮿��� ���̺긦 �е��� �Ѵ�.
//-----------------------------------------------------------------------------------//
BOOL ReadDataWavBuf(LPDIRECTSOUNDBUFFER lpDSB, BYTE *wavBUF, DWORD dwSize) 
{
	/*
    // Seek to correct position in file (if necessary)
    if (dwPos != 0xffffffff) 
    {
        if (fseek(pFile, dwPos, SEEK_SET) != 0) 
        {
            return FALSE;
        }
    }
	*/

	int cnt=0;
    // Lock data in buffer for writing
    LPVOID pData1;
    DWORD  dwData1Size;
    LPVOID pData2;
    DWORD  dwData2Size;
    HRESULT rval;

    rval = lpDSB->Lock(0, dwSize, &pData1, &dwData1Size, &pData2, &dwData2Size, DSBLOCK_FROMWRITECURSOR);
    if (rval != DS_OK)
    {
        return FALSE;
    }

    // Read in first chunk of data
    if (dwData1Size > 0) 
    {
        //if (fread(pData1, dwData1Size, 1, pFile) != 1) 
        //{               
        //    return FALSE;
        //}
		memcpy(pData1, &wavBUF[cnt], dwData1Size);
		cnt+=dwData1Size;
		//ktj :���콺���� �ӽ÷� ������
		if(mouThRead ==0) {
			mouThdataOfs = (BYTE*)pData1;
		}

    }

    // read in second chunk if necessary
    if (dwData2Size > 0) 
    {
        //if (fread(pData2, dwData2Size, 1, pFile) != 1) 
        //{
        //    return FALSE;
        //}
		memcpy(pData2, &wavBUF[cnt], dwData2Size);
    }

    // Unlock data in buffer
    rval = lpDSB->Unlock(pData1, dwData1Size, pData2, dwData2Size);
    if (rval != DS_OK)
    {
        return FALSE;
    }

    // Yahoo!
    return TRUE;
}

BOOL CreateBufferFromWavBUF(unsigned char* wavBUf, unsigned int dwBuf)
{
    DWORD dwSize;
    BOOL bStereo;
	BYTE cnt=0;

	// Open the wave file       
    //FILE* pFile = fopen(FileName,"rb");
    //if (pFile == NULL) return FALSE;

    // Read in the wave header
	WaveHeader1 wavHdr1;	//DATA[4]
	WaveHeader2 wavHdr2;	//DATA[16]

    WaveHeader0 wavHdr;	//�ϴ� DATA[18]�� �д´�.
    //if (fread(&wavHdr, sizeof(wavHdr), 1, pFile) != 1) 
    //{
    //    fclose(pFile);
    //    return NULL;
    //}

	memcpy(&wavHdr, &wavBUf[cnt], sizeof(wavHdr) );
	cnt+=sizeof(wavHdr);


	if(wavHdr.DATA[0]=='d' && wavHdr.DATA[1] == 'a') {
		//fseek( pFile, 0, SEEK_SET);	//�ٽ� ó������ DATA[4]�� �д´�.
		//fread(&wavHdr1, sizeof(wavHdr1), 1, pFile);
		cnt=0;
		memcpy(&wavHdr1, &wavBUf[cnt], sizeof(wavHdr1) );
		cnt+=sizeof(wavHdr1);


	    dwSize = wavHdr1.dwDSize;
	    // Is this a stereo or mono file?
		bStereo = wavHdr1.wChnls > 1 ? TRUE : FALSE;

		//fd2("0000  size %d freq %d %d %d %d  ",dwSize, wavHdr1.dwSRate, wavHdr1.BitsPerSample, wavHdr1.wBlkAlign, bStereo);
		// Create the sound buffer for the wave file
		if (!CreateSoundBuffer(dwBuf, dwSize, wavHdr1.dwSRate, wavHdr1.BitsPerSample, wavHdr1.wBlkAlign, bStereo))
		{
			// Close the file
			//fclose(pFile);
	        return FALSE;
		}


		//���콺�� �о���ϱⶫ�� �ӽ÷� ������
		mouThRead = dwBuf;
		// Read the data for the wave file into the sound buffer
		//if (!ReadDataWavBuf(g_lpSounds[dwBuf], pFile, dwSize, sizeof(wavHdr1))) 
		if( !ReadDataWavBuf(g_lpSounds[dwBuf], &wavBUf[cnt],dwSize ) )
		{
	        //fclose(pFile);
		    return FALSE;
		}
	}

	else if(wavHdr.DATA[0]=='f' && wavHdr.DATA[1] == 'a') {
		//fseek( pFile, 0, SEEK_SET);	//�ٽ� ó������ DATA[16]�� �д´�.
		//fread(&wavHdr2, sizeof(wavHdr2), 1, pFile);
		cnt=0;
		memcpy(&wavHdr2, &wavBUf[cnt], sizeof(wavHdr2) );
		cnt+=sizeof(wavHdr2);


	    dwSize = wavHdr2.dwDSize;
	    // Is this a stereo or mono file?
		bStereo = wavHdr2.wChnls > 1 ? TRUE : FALSE;

		
//fd2("111  size %d freq %d %d %d %d  ",dwSize, wavHdr2.dwSRate, wavHdr2.BitsPerSample, wavHdr2.wBlkAlign, bStereo);		
		// Create the sound buffer for the wave file
		if (!CreateSoundBuffer(dwBuf, dwSize, wavHdr2.dwSRate, wavHdr2.BitsPerSample, wavHdr2.wBlkAlign, bStereo))
		{
			// Close the file
			//fclose(pFile);
	        return FALSE;
		}

		//���콺�� �о���ϱⶫ�� �ӽ÷� ������
		mouThRead = dwBuf;
		// Read the data for the wave file into the sound buffer
		//if (!ReadData(g_lpSounds[dwBuf], pFile, dwSize, sizeof(wavHdr2))) 
		if( !ReadDataWavBuf(g_lpSounds[dwBuf], &wavBUf[cnt], dwSize ) )
		{
	        //fclose(pFile);
		    return FALSE;
		}
	}
	else {
	    // Figure out the size of the data region
		dwSize = wavHdr.dwDSize;
		// Is this a stereo or mono file?
	    bStereo = wavHdr.wChnls > 1 ? TRUE : FALSE;

//fd2("2222 size %d freq %d %d %d %d  ",dwSize, wavHdr.dwSRate, wavHdr.BitsPerSample, wavHdr.wBlkAlign, bStereo);

		// Create the sound buffer for the wave file
		if (!CreateSoundBuffer(dwBuf, dwSize, wavHdr.dwSRate, wavHdr.BitsPerSample, wavHdr.wBlkAlign, bStereo))
		{
			// Close the file
			//fclose(pFile);
	        return FALSE;
		}

		//���콺�� �о���ϱⶫ�� �ӽ÷� ������
		mouThRead = dwBuf;
		// Read the data for the wave file into the sound buffer
		//if (!ReadData(g_lpSounds[dwBuf], pFile, dwSize, sizeof(wavHdr))) 
		if( !ReadDataWavBuf(g_lpSounds[dwBuf], &wavBUf[cnt], dwSize ) )
		{
			//fclose(pFile);
			return FALSE;
		}
	}
    // Close out the wave file
    //fclose(pFile);

    // Yahoo!
    return TRUE;
}
//-----------------------------------------------------------------------------------//








//----------------------------------------------------------------------
// 
// Function     : StopAllSounds()
//
// Purpose      : Stops all sounds
//
//----------------------------------------------------------------------

BOOL StopAllWav()
{
	if (!g_bSoundPresent) return FALSE;

    // Make sure we have a valid sound buffer
    for (int i = 0; i < NUM_SOUNDS; i ++)
    {
        if (g_lpSounds[i])
        {
            DWORD dwStatus;
            //TRY_DS(g_lpSounds[i]->GetStatus(&dwStatus));
			g_lpSounds[i]->GetStatus(&dwStatus);

            if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
            {
                // Play the sound
                //TRY_DS(g_lpSounds[i]->Stop())
				g_lpSounds[i]->Stop();

            }
        }
    }

    // Yahoo!
    return TRUE;
}





#define VOL_SHIFT (-400)
#define VOL_MULT 10

#define PAN_SHIFT (-400)
#define PAN_MULT  10

#define FRQ_MULT  10
//----------------------------------------------------------------------
// 
// Function     : PlaySoundDS()
//                dwSound : wav�� ��ȣ,  dwFlags : 0-�ѹ����Ҹ�����,1-������ ���� ��ӼҸ�����
//				  dwVol (0~400) (�����δ� -400~0�̰� *10�Ѱ�)
//				  dwfreq : 441~4410(�����δ� 4410~44100HZ����)
// Purpose      : Plays a sound using direct sound
//
//----------------------------------------------------------------------
BOOL PlayWav(DWORD dwSound, DWORD dwFlags, DWORD dwVol, DWORD dwfreq)
{
    if (g_bSoundPaused) return TRUE;
    if (!g_bSoundPresent) return TRUE;

    // Make sure the sound is valid
    if (dwSound >= NUM_SOUNDS) return FALSE; 

	//�߰��ε�
	chk_load_sangJuWav( (int) dwSound) ;


    // Make sure we have a valid sound buffer
    if (g_lpSounds[dwSound])
    {
        DWORD dwStatus;
        g_lpSounds[dwSound]->GetStatus(&dwStatus);
        if ((dwStatus & DSBSTATUS_PLAYING) != DSBSTATUS_PLAYING)
        {
            // Play the sound
            g_lpSounds[dwSound]->SetVolume( (dwVol+VOL_SHIFT) * VOL_MULT);
			g_lpSounds[dwSound]->SetFrequency(dwfreq*FRQ_MULT);
			g_lpSounds[dwSound]->Play(0, 0, dwFlags);

        }
	
		//������� ó������
		if(dwSound==0)
			g_lpSounds[dwSound]->SetCurrentPosition(0); 

    }

    // Yahoo!
    return TRUE;
}


//�Ҹ��� ���� �ִ� �����̶� ��� �Ҹ���.
BOOL PlayWav2(DWORD dwSound, DWORD dwFlags, DWORD dwVol, DWORD dwfreq)
{
    if (g_bSoundPaused) return TRUE;
    if (!g_bSoundPresent) return TRUE;

    // Make sure the sound is valid
    if (dwSound >= NUM_SOUNDS) return FALSE;

	//�߰��ε�
	chk_load_sangJuWav( (int) dwSound) ;


    // Make sure we have a valid sound buffer
    if (g_lpSounds[dwSound])
    {
		
		DWORD dwStatus;
		g_lpSounds[dwSound]->GetStatus(&dwStatus);
		if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)	//�ϴ� ����
			//g_lpSounds[dwSound]->Stop();
			g_lpSounds[dwSound]->SetCurrentPosition(0);

		g_lpSounds[dwSound]->SetVolume( (dwVol+VOL_SHIFT) * VOL_MULT); 
		g_lpSounds[dwSound]->SetFrequency(dwfreq*FRQ_MULT);
		g_lpSounds[dwSound]->Play(0, 0, dwFlags);
    }

    return TRUE;
}

//�Ҹ��� ���������� Ȯ��
BOOL IsPlayWav( DWORD dwSound )
{
	DWORD dwStatus;

    if (g_bSoundPaused) return FALSE;
    if (!g_bSoundPresent) return FALSE;
    if (dwSound >= NUM_SOUNDS) return FALSE;
	if ( !g_lpSounds[dwSound] ) return FALSE;

	g_lpSounds[dwSound]->GetStatus(&dwStatus);

	if ((dwStatus & DSBSTATUS_PLAYING) != DSBSTATUS_PLAYING) return FALSE;

	return TRUE;
}


//�Ҹ��� ���� �ִ� �����̶� ��� �Ҹ���.
//dwfreq : 441~4410
//dwPan : 0~800(0:���� ����Ŀ, 400:�߰� 800:������ ����Ŀ������ �Ҹ��� ������.)
BOOL PlayWav3(DWORD dwSound, DWORD dwFlags, DWORD dwVol, DWORD dwfreq, DWORD dwPan)
{
    if (g_bSoundPaused) return TRUE;
    if (!g_bSoundPresent) return TRUE;

    // Make sure the sound is valid
    if (dwSound >= NUM_SOUNDS) return FALSE;

	//�߰��ε�
	chk_load_sangJuWav( (int) dwSound) ;

    // Make sure we have a valid sound buffer
    if (g_lpSounds[dwSound])
    {
		
		DWORD dwStatus;
		g_lpSounds[dwSound]->GetStatus(&dwStatus);
		if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)	//�ϴ� ����
			//g_lpSounds[dwSound]->Stop();
			g_lpSounds[dwSound]->SetCurrentPosition(0);

		//0~800(0:���� ����Ŀ, 400:�߰� 800:������ ����Ŀ)
		g_lpSounds[dwSound]->SetPan( (dwPan+PAN_SHIFT)*PAN_MULT);//4410);

		//441(*10) ~4410(*10)
		g_lpSounds[dwSound]->SetFrequency(dwfreq*FRQ_MULT);

		//0~400
		g_lpSounds[dwSound]->SetVolume( (dwVol+VOL_SHIFT) * VOL_MULT); 
		g_lpSounds[dwSound]->Play(0, 0, dwFlags);
    }

    return TRUE;
}



//�÷��̵��߿� volume change
BOOL PlayWav_VolChange(DWORD dwSound, DWORD dwVol)
{
    if (g_bSoundPaused) return TRUE;
    if (!g_bSoundPresent) return TRUE;

    // Make sure the sound is valid
    if (dwSound >= NUM_SOUNDS) return FALSE;

	//�߰��ε�
	chk_load_sangJuWav( (int) dwSound) ;

    // Make sure we have a valid sound buffer
    if (g_lpSounds[dwSound])
    {
		DWORD dwStatus;
		g_lpSounds[dwSound]->GetStatus(&dwStatus);
		if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING) {
			//0~400
			g_lpSounds[dwSound]->SetVolume( (dwVol+VOL_SHIFT) * VOL_MULT); 
		}

    }
    return TRUE;
}

//�÷��̵��߿� Frequency Change
BOOL PlayWav_FrequencyChange(DWORD dwSound, DWORD dwfreq)
{
    if (g_bSoundPaused) return TRUE;
    if (!g_bSoundPresent) return TRUE;

    // Make sure the sound is valid
    if (dwSound >= NUM_SOUNDS) return FALSE;

    // Make sure we have a valid sound buffer
    if (g_lpSounds[dwSound])
    {
		DWORD dwStatus;
		g_lpSounds[dwSound]->GetStatus(&dwStatus);
		if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
			//441(*10) ~4410(*10)
			g_lpSounds[dwSound]->SetFrequency(dwfreq*FRQ_MULT);
    }
    return TRUE;
}


//�÷��̵��߿� Panning Change
BOOL PlayWav_PanChange(DWORD dwSound, DWORD dwPan)
{
    if (g_bSoundPaused) return TRUE;
    if (!g_bSoundPresent) return TRUE;

    // Make sure the sound is valid
    if (dwSound >= NUM_SOUNDS) return FALSE;

    // Make sure we have a valid sound buffer
    if (g_lpSounds[dwSound])
    {
		DWORD dwStatus;
		g_lpSounds[dwSound]->GetStatus(&dwStatus);
		if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
			//0~800(0:���� ����Ŀ, 400:�߰� 800:������ ����Ŀ)
			g_lpSounds[dwSound]->SetPan( (dwPan+PAN_SHIFT)*PAN_MULT);//4410);
    }
    return TRUE;
}



BOOL StopWav(int i);

//ktj
void check_backMus(int flag)
{
	if(flag==0) {
		StopWav(0);
	}
	else {
		StopWav(0);
		PlayWav(0, 1);
	}
}

/*
//�Ʒ� ���̺�� g_lpSounds[..]�� 1�����͵��Ƿ� ��ȣ�� 1������ ���۵ʿ� ����
char *sangJuWav[NUM_SOUNDS] = {      //���� wav�� �̴´�.
"wav\\Ready.wav",              //���Ӹ�� ready
"wav\\go.wav",                 //���Ӹ�� gameing
"wav\\hurry.wav",              //Ư���ð��ȿ� �ȴ����� �ʴ��ѹ��� ���´�.
"wav\\��������.WAV",               //������ ���


"wav\\���ӳ�.wav",			//game��������.
"wav\\�ڸ�Ȯ��.WAV",               //������ ���
"wav\\��������.WAV",               //�������������
"wav\\��Ƣ���.WAV",               //��Ƣ�涧

"wav\\����Ʈ.WAV",               //������ ����϶�

NULL,
NULL,
};
*/
/*
char *sangJuWav[NUM_SOUNDS] = {     //���� wav�� �̴´�.
"wav\\start1.wav",					//���Ӹ�� ����
"wav\\clock.wav",					//Ư���ð��ȿ� �ȴ����� �ʴ��ѹ��� ���´�.
"wav\\bf.wav",					//������ ���

"wav\\bplace.wav",              //������ ���
"wav\\bd.wav",					//�������������
"wav\\wall_br.wav",             //���� �΋H�� ��Ƣ�涧


"wav\\win_hc.wav",             //������ ����϶�(�¸��ڼ�)
"wav\\win1.wav",

"wav\\loose1.wav",				//2�����Ϻ��ʹ� ������.
"wav\\loose2.wav",
"wav\\loose3.wav",
"wav\\loose4.wav",

"wav\\e_bu1.wav",						//���ǰ� �ö�.
"wav\\e_bu2.wav",

"wav\\bt_yes.wav",				//������߸��� ���� �������� ����.
"wav\\bt_wow.wav",


"wav\\ldown.wav",			//���ٳ�����.

"wav\\aim.wav",			//���ر�


"wav\\win_m.wav",			//�̰����� ����
"wav\\lose_m.wav",			//������ ����.


NULL,
NULL,
};

*/












//�����̺긦 ���� 0���� �׻�  ����ΰ� 1�� ���Ϻ��� ���ֿ� ���̺긦 �д´�.
BOOL InitDX_Wav2(HWND g_hwnd)  //�ʱ�ȭ
{
    char strFilename[128];
    if(!wav_opt) {
            g_bSoundPresent = FALSE;
            return 0;
    }

    int i;
    HRESULT     rval;
	


    // Create DirectSound object
    rval = DirectSoundCreate(NULL, &g_lpDS, NULL);

    // Determine whether sound is present
    if(rval == DS_OK)
            g_bSoundPresent = TRUE;
    else
            g_bSoundPresent = FALSE;
    
    /*
    //���� wav�� �ε��ϱ����� �ٲ۴�.
    if (g_bSoundPresent)
    {
        // Set the DirectSound cooperative level
        // Null out all the sound pointers
        for (i = 0; i < NUM_SOUNDS; i ++) {
            g_lpSounds[i] = NULL;
        }
    }
    */

    if (g_bSoundPresent)
    {
        // Set the DirectSound cooperative level
        g_lpDS->SetCooperativeLevel(g_hwnd, DSSCL_NORMAL);
        // Null out all the sound pointers
        for (i = 0; i < NUM_SOUNDS; i ++) {
            g_lpSounds[i] = NULL;
        }

		
        for(i=1; i<NUM_SOUNDS; i++)
        {
            if( sangJuWav[i-1] != NULL && sangJuWav[i-1][0] ) {
				if(  sangJuWav_F[i-1 ] == 0 ) 
					continue;

                strcpy ( strFilename , dataDir);
                strcat ( strFilename , sangJuWav[i-1] );

                if (!CreateBufferFromWaveFile(strFilename, i))
                {
					
					//fd2( " wav %s break ", strFilename );
                        //RegError("Couldn't load INTRO.WAV!");
                        //return FALSE;
                }
                //numSound++;
            }
        }
    }
    // Yahoo!






	Init_BGM(g_hwnd);		//���wav�̿��ϱ�
    return TRUE;
}





void TermDX_Wav2()
{
	if (!g_bSoundPresent) return;
    
    for (int i = 0; i < NUM_SOUNDS; i ++) {		// Destroy sounds
        if (g_lpSounds[i]) {
            PlayWav_VolChange(i, 0);
            g_lpSounds[i]->Release();	g_lpSounds[i] = NULL;
        }
    }
    if (g_lpDS) {								// Destroy DirectSound object
        g_lpDS->Release();
        g_lpDS = NULL;
    }

	Uninit_BGM();		//���wav�̿��ϱ�
}


BOOL CreateBufferFromWaveFile2(char* FileName, int sbufN)
{
	if (!g_bSoundPresent) {
		return FALSE;
	}
        StopWav(sbufN); //Ȥ�� �Ҹ��� ������ �����̸� ����.
	sBufferClear(sbufN);

	if (!CreateBufferFromWaveFile(FileName, sbufN))	{
                sBufferClear(sbufN);
                return FALSE;
        }
        return TRUE;
}


BOOL sBufferClear(int sbufN)
{
        if (!g_bSoundPresent) {
            return FALSE;
        }

	if (g_lpSounds[sbufN]) {
                g_lpSounds[sbufN]->Release();
		g_lpSounds[sbufN] = NULL;
		return TRUE;
        }
        return FALSE;
}


BOOL StopWav(int i)
{
        if (!g_bSoundPresent) {
            return FALSE;
        }

    // Make sure we have a valid sound buffer
     if (g_lpSounds[i])
	{
            DWORD dwStatus;
            //TRY_DS(g_lpSounds[i]->GetStatus(&dwStatus));
			g_lpSounds[i]->GetStatus(&dwStatus);

            if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
            {
                // Play the sound
                //TRY_DS(g_lpSounds[i]->Stop())
                g_lpSounds[i]->Stop();
            }
    }
    // Yahoo!
    return TRUE;
}


//ktj
void allWAVE_playORstop(int flag)
{
	int i;
	if(flag==0) {
		for(i=0; i<NUM_SOUNDS; i++) {		//stop wave
			if (g_lpSounds[i]) {
				StopWav(i);
			}
		}
	}
        else {

                if (g_lpSounds[0]) {    //back music���� ���̴� �͸� �Ҹ�����.
                                StopWav(0);
                                PlayWav(0);
                }
        }
}





//-------------------------- ktj
// �����÷������� ������, ����������ã�� �Լ�
			//read position, write position
//GetCurrentPosition(&position, NULL); 
//���� �����侾�� ���´�.
//HRESULT GetFrequency(  LPDWORD lpdwFrequency  );
//---------------------------------------
DWORD get_pos(DWORD dwSound)
{
	DWORD pos;
    if (g_bSoundPaused) return TRUE;
    if (!g_bSoundPresent) return TRUE;

    // Make sure the sound is valid
    if (dwSound >= NUM_SOUNDS) return FALSE;

    // Make sure we have a valid sound buffer
    if (g_lpSounds[dwSound])
    {
		DWORD dwStatus;
		g_lpSounds[dwSound]->GetStatus(&dwStatus);
		
		if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING) {
			//441(*10) ~4410(*10)
			g_lpSounds[dwSound]->GetCurrentPosition(&pos, NULL); 

			

			return pos;
		}
    }

    
	return 0;
}




//----------------------- 0���� �����ϴ� �����̹Ƿ� �װ��� ��ġ�� �˾Ƴ�
//���� �������� ���θ� �˾Ƴ��� ���� �Լ�
//�ϴ� ����Ÿ�� ���� ������������ ����������.
BYTE getmouTH()
{
	if(mouThdataOfs ==NULL) return 0;

	DWORD pos = get_pos(0);				//���콺����Ÿ�� 0����.
	BYTE b = mouThdataOfs[pos];
	return b;
}

int isPlaying(int i)
{
	if (!g_bSoundPresent) return FALSE;

    // Make sure we have a valid sound buffer
    if (g_lpSounds[i])   
	{
            DWORD dwStatus;
            //TRY_DS(g_lpSounds[i]->GetStatus(&dwStatus));
			g_lpSounds[i]->GetStatus(&dwStatus);

            if ((dwStatus & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING)
            {
                // Play the sound
                //TRY_DS(g_lpSounds[i]->Stop())
				return 1;
            }
			else  {
				return 0;
			}
    }
    // Yahoo!
    return 0;
}







































































//=======================================================================start
//sound ��� wav�� Ÿ�̸ӷ� �о ����
//=======================================================================start

#include "wave.h"
#include "shell.h"
#include "stream.h"


/*
BOOL Init_BGM(HWND hwnd);		//init
void Uninit_BGM(void);			//uninit
BOOL OpenBGM(LPSTR lpszFileName);	//bgm �
void PlayBGM(void);
void StopBGM(void);
*/

HRESULT SetupStreamBuffer(LPSTR lpszFilename);


LPDIRECTSOUND       lpDirectSound = NULL;

LPDIRECTSOUNDBUFFER lpDSBStreamBuffer = NULL;


HWND    m_hWnd;

LONG  m_lInTimer        = FALSE;
BOOL  m_bTimerInit      = FALSE;
BOOL  m_bPlaying        = FALSE;
BOOL  m_bIsCloseOnRun   = FALSE;
BOOL  m_bFileOpen       = FALSE;
UINT  m_uTimerID        = 0;
DWORD m_SecondBuffFocus = 0;

WAVEINFOCA  m_wiWave;



void LSStopBackground(DWORD dwState);
void LSResetWavePlayer(void);
void LSFillDataBuffer(void);
void CALLBACK LSTimeRunFuncForDSound(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);	


//=======================================================================
//       ��������� �����
//       Input.
//           DWORD dwState - �÷���
//-----------------------------------------------------------------------
void LSStopBackground(DWORD dwState)
{
    m_wiWave.bDonePlaying = TRUE;
    if(m_bTimerInit){
        timeKillEvent(m_uTimerID);
        timeEndPeriod(PLAYBACK_TIMER_PERIOD / PLAYBACK_OVERSAMPLE);
        // Busy wait for timer func to exit
        while(InterlockedExchange(&m_lInTimer, TRUE)) Sleep(100);
        m_bTimerInit = FALSE;
    }
    if(m_bPlaying){
        m_bPlaying = FALSE;
        lpDSBStreamBuffer->Stop();
    }
    // Short circuit to allow command-line forced shutdown
    if(!(dwState & DSSTREAM_STOPF_NOREOPEN) && !m_bIsCloseOnRun){
        LSResetWavePlayer();
        return;
    }else{
        if(m_bFileOpen){
             WaveCloseReadFile(&m_wiWave.hmmio, &m_wiWave.pwfx);
             if(lpDSBStreamBuffer){
                 lpDSBStreamBuffer->Release();
             }

             lpDSBStreamBuffer = NULL;

             m_bFileOpen = FALSE;
         }
    }
}

//=======================================================================
//       ���̺� ����� �ʱ�ȭ �Ѵ�.
//-----------------------------------------------------------------------
void LSResetWavePlayer(void)
{
    WaveStartDataRead(&m_wiWave.hmmio, &m_wiWave.mmck, &m_wiWave.mmckInRIFF);
    m_wiWave.bFoundEnd        = FALSE;
    m_wiWave.dwBytesRemaining = 0;

    LSFillDataBuffer();

    m_wiWave.bDonePlaying     = FALSE;
}

//=======================================================================
//       ���̷�Ʈ ���� ���۸� ä���.
//-----------------------------------------------------------------------
void LSFillDataBuffer(void)
{
    LPBYTE   lpWrite1, lpWrite2;
    DWORD    dwLen1, dwLen2;
    UINT     uActualBytesWritten;
    HRESULT  dsrval;
    dsrval = lpDSBStreamBuffer->Lock(0, m_wiWave.dwBufferSize, 
		//&((LPVOID)lpWrite1), 
		(LPVOID*) &lpWrite1, 
		&dwLen1,
        //&((LPVOID)lpWrite2), 
		(LPVOID *) &lpWrite2, 
		
		&dwLen2, 0);
    if(dsrval != DS_OK) return;
//    Assert( NULL == lpWrite2 );
//    Assert( 0 == dwLen2 );
    if(dwLen1){
//        Assert( NULL != lpWrite1 );
        WaveReadFile(m_wiWave.hmmio, (UINT)dwLen1, lpWrite1, &m_wiWave.mmck, &uActualBytesWritten);
        if(uActualBytesWritten < dwLen1){
            if(m_wiWave.bLoopFile){
                // If the file is shorter than the buffer and we're looping, we need to
                // read the file in again so that we don't get a block of silence before
                // the timer loops playback.
                LPBYTE lpTemp          = lpWrite1;
                DWORD  cbReadLoopTotal = dwLen1;
                do{
                    // Continue decrementing our count and moving our temp
                    // pointer forward until we've read the file enough times
                    // to fill the buffer.  NOTE: It's probably not efficient
                    // to bother with the overhead of streaming a file that's
                    // not at least as large as the buffer...
                    lpTemp          += uActualBytesWritten;
                    cbReadLoopTotal -= uActualBytesWritten;
                    WaveStartDataRead(&m_wiWave.hmmio, &m_wiWave.mmck, &m_wiWave.mmckInRIFF);
                    WaveReadFile(m_wiWave.hmmio, (UINT)cbReadLoopTotal, lpTemp, &m_wiWave.mmck, &uActualBytesWritten);
                }while(uActualBytesWritten < cbReadLoopTotal);
            }else{
                m_wiWave.bFoundEnd        = TRUE;
                m_wiWave.dwBytesRemaining = (DWORD)uActualBytesWritten;
                // Fill in silence
                FillMemory(lpWrite1+uActualBytesWritten, dwLen1-uActualBytesWritten,
                    (BYTE)(m_wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0));
            }
        }
    }
    dsrval = lpDSBStreamBuffer->Unlock((LPVOID)lpWrite1, dwLen1, (LPVOID)lpWrite2, 0);
    m_wiWave.dwNextWriteOffset = m_wiWave.dwProgress = 0;
}


//=======================================================================
//       Ÿ�̸� �̺�Ʈ �ڵ鷯
//-----------------------------------------------------------------------
//void fd2( char *fmt, ...);
void CALLBACK LSTimeRunFuncForDSound(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    LPBYTE  lpWrite1, lpWrite2, lpTemp;
    DWORD   dwLen1, dwLen2, dwPlay, dwWrite, dwPlayedLength, dwWriteLength;
    DWORD   dwLeftToRead, dwStatus;
    UINT    uChkErr;
    BOOL    fRefillLostBuffer = FALSE;
    HRESULT dsrval;

	//static cnt=0;
	//fd2( "timer : %d ", cnt++);

    if(InterlockedExchange(&m_lInTimer, TRUE)) return;

    // See if the buffer has been lost
    m_wiWave.lpDSBStreamBuffer->GetStatus(&dwStatus);
    if(DSBSTATUS_BUFFERLOST & dwStatus){
        // Restore the buffer and set some variables that will cause it
        // to be filled again and replayed
        dsrval = m_wiWave.lpDSBStreamBuffer->Restore();
        if(FAILED(dsrval)){
            InterlockedExchange(&m_lInTimer, FALSE);
            return;
        }
        m_wiWave.dwNextWriteOffset = 0;
        fRefillLostBuffer        = TRUE;
    }

    // Get and print the current position of the play cursor
    m_wiWave.lpDSBStreamBuffer->GetCurrentPosition(&dwPlay, &dwWrite);

    // If the play cursor is at the same spot as the last call, there are two
    // possibilities.  The first is that we were called extremely late and
    // happened to land on an integer number of complete buffer cycles.  This
    // is not very likely.  The other is that the play cursor didn't move.
    // Since we're oversampling, this is very likely.  In this case, we should
    // bail.
    if(dwPlay == m_wiWave.dwNextWriteOffset && !fRefillLostBuffer){
        InterlockedExchange(&m_lInTimer, FALSE);
        return;
    }
    // Have we found the end of the file and passed the buffer end?
    if(m_wiWave.bFoundEnd && !m_wiWave.dwBytesRemaining){
        if(!m_wiWave.bDonePlaying){
            m_wiWave.bDonePlaying = TRUE;
			LSStopBackground(0L);
        }
        InterlockedExchange(&m_lInTimer, FALSE);
        return;
    }

    if(dwPlay < m_wiWave.dwNextWriteOffset){
        // Calculate how many writeable bytes there are behind the play cursor
        dwPlayedLength = (dwPlay + m_wiWave.dwBufferSize - m_wiWave.dwNextWriteOffset);
    }else{
        // Calculate how many writeable bytes there are behind the play cursor
        dwPlayedLength = (dwPlay - m_wiWave.dwNextWriteOffset);
    }

    // If the buffer was lost, then we need to start filling data at the start of
    // the buffer, but we can decrease startup latency by only filling a segment
    // or two this time around.
    if(fRefillLostBuffer){
        dwWriteLength = 2 * m_wiWave.dwBufferSize / NUM_BUFFER_SEGMENTS;
    }else{
        dwWriteLength = dwPlayedLength;
    }

    m_wiWave.dwProgress += dwPlayedLength;

    // If m_wiWave.bFoundEnd == TRUE, then we've finished reading in the file,
    // but we need to wait until the buffer's play cursor passes the point we
    // were at when we found out we were done reading.
    if(m_wiWave.bFoundEnd && m_wiWave.dwBytesRemaining){
        // Decrement the count of how many bytes we have to wait for before
        // we can kill the timer procedure safely
        if(dwPlayedLength > m_wiWave.dwBytesRemaining){
            m_wiWave.dwBytesRemaining = 0;
        }else{
            m_wiWave.dwBytesRemaining -= dwPlayedLength;
        }
        if(m_wiWave.lpDSBStreamBuffer->Lock(m_wiWave.dwNextWriteOffset, dwWriteLength,
                                         //&((LPVOID)lpWrite1), 
										 (LPVOID *)&lpWrite1, 
										 &dwLen1,
                                         //&((LPVOID)lpWrite2), 
										 (LPVOID *) &lpWrite2, 
										 
										 &dwLen2, 0) != 0){
            OutputDebugString("LSTimeRunFuncForDSound() could not lock DirectSoundBuffer");
            InterlockedExchange(&m_lInTimer, FALSE);
            return;
        }

        // Silence out both parts of the locked buffer
        memset(lpWrite1, m_wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0, dwLen1);

        if(lpWrite2 && dwLen2){
            memset(lpWrite2, m_wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0, dwLen2);
        }
        m_wiWave.lpDSBStreamBuffer->Unlock((LPVOID)lpWrite1, dwLen1, (LPVOID)lpWrite2, dwLen2);
        // This code is stolen from the end of the routine -- we need to keep
        // zeroing out buffer segments while we're waiting for the play cursor to
        // catch up to the end of the WAVE data.
        m_wiWave.dwNextWriteOffset += dwWriteLength;
        if(m_wiWave.dwNextWriteOffset >= m_wiWave.dwBufferSize){
            m_wiWave.dwNextWriteOffset -= m_wiWave.dwBufferSize;
        }
        InterlockedExchange(&m_lInTimer, FALSE);
        return;
    }

    // Lock a segment of memory that is behind the play cursor
    if(m_wiWave.lpDSBStreamBuffer->Lock(m_wiWave.dwNextWriteOffset, dwWriteLength,
                                      //&((LPVOID)lpWrite1), 
									  (LPVOID *) &lpWrite1, 
									  &dwLen1,
                                      //&((LPVOID)lpWrite2), 
									  (LPVOID *) &lpWrite2, 
									  &dwLen2, 0) != 0){
        OutputDebugString("LSTimeRunFuncForDSound() could not lock DirectSoundBuffer");
        InterlockedExchange(&m_lInTimer, FALSE);
        return;
    }

    if(dwLen1 && !m_wiWave.bDonePlaying){
        WaveReadFile(m_wiWave.hmmio, (UINT)dwLen1, lpWrite1, &m_wiWave.mmck, &uChkErr);
        if(uChkErr < (UINT)dwLen1){
            if(!m_wiWave.bLoopFile){
                // Zero out the rest of this block
                if(m_wiWave.pwfx->wBitsPerSample == 8){
                    memset(lpWrite1+uChkErr, 128, ((UINT)dwLen1-uChkErr));
                }else if(m_wiWave.pwfx->wBitsPerSample == 16){
                    memset(lpWrite1+uChkErr, 0, ((UINT)dwLen1-uChkErr));
                }

                // Enable play completion detection code at the beginning of the next call
                m_wiWave.bFoundEnd = TRUE;
                if(dwPlay > m_wiWave.dwNextWriteOffset){
                    m_wiWave.dwBytesRemaining = (m_wiWave.dwNextWriteOffset + m_wiWave.dwBufferSize - dwPlay);
                }else{
                    m_wiWave.dwBytesRemaining = (m_wiWave.dwNextWriteOffset - dwPlay);
                }
            }else{
                lpTemp       = lpWrite1;
                dwLeftToRead = dwLen1;
                do{
                    // Continue decrementing our count and moving our temp
                    // pointer forward until we've read the file enough times
                    // to fill the buffer.  NOTE: It's probably not efficient
                    // to bother with the overhead of streaming a file that's
                    // not at least as large as the buffer...
                    lpTemp       += uChkErr;
                    dwLeftToRead -= uChkErr;
                    WaveStartDataRead(&m_wiWave.hmmio, &m_wiWave.mmck, &m_wiWave.mmckInRIFF);
                    WaveReadFile(m_wiWave.hmmio, (UINT)dwLeftToRead, lpTemp, &m_wiWave.mmck, &uChkErr);
                }while(uChkErr < dwLeftToRead);
            }
        }
    }
    // The bDonePlaying flag is set by the caller if the user stops playback
    // before the end of the WAVE file is encountered.  It tells us to cut this
    // racket out and play nothing in case it takes the caller a couple
    // interrupts to shut off the timer.
    else if(dwLen1 && m_wiWave.bDonePlaying){
        // Set the appropriate silence value
        _fmemset(lpWrite1, m_wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0, dwLen1);
    }

    if(dwLen2 && !m_wiWave.bDonePlaying){
        WaveReadFile(m_wiWave.hmmio, (UINT)dwLen2, lpWrite2, &m_wiWave.mmck, &uChkErr);
        if(uChkErr < (UINT)dwLen2){
            if(!m_wiWave.bLoopFile){
                // Zero out the rest of this block
                if(m_wiWave.pwfx->wBitsPerSample == 8){
                    memset(lpWrite2+uChkErr, 128, ((UINT)dwLen2-uChkErr));
                }else if(m_wiWave.pwfx->wBitsPerSample == 16){
                    memset(lpWrite2+uChkErr, 0, ((UINT)dwLen2-uChkErr));
                }
                // Enable play completion detection code at the beginning
                // of the next call
                m_wiWave.bFoundEnd = TRUE;
                if(dwPlay > m_wiWave.dwNextWriteOffset){
                    m_wiWave.dwBytesRemaining = (m_wiWave.dwNextWriteOffset + m_wiWave.dwBufferSize - dwPlay);
                }else{
                    m_wiWave.dwBytesRemaining = (m_wiWave.dwNextWriteOffset - dwPlay);
                }
            }else{
                lpTemp       = lpWrite2;
                dwLeftToRead = dwLen2;
                do{
                    // Continue decrementing our count and moving our temp
                    // pointer forward until we've read the file enough times
                    // to fill the buffer.  NOTE: It's probably not efficient
                    // to bother with the overhead of streaming a file that's
                    // not at least as large as the buffer...
                    lpTemp += uChkErr;
                    dwLeftToRead -= uChkErr;
                    WaveStartDataRead(&m_wiWave.hmmio, &m_wiWave.mmck, &m_wiWave.mmckInRIFF);
                    WaveReadFile(m_wiWave.hmmio, (UINT)dwLeftToRead, lpTemp, &m_wiWave.mmck, &uChkErr);
                }while(uChkErr < dwLeftToRead);
            }
        }
    }else if(lpWrite2 && dwLen2 && m_wiWave.bDonePlaying){
        // Set the appropriate silence value
        _fmemset(lpWrite2, m_wiWave.pwfx->wBitsPerSample == 8 ? 128 : 0, dwLen2);
    }
    m_wiWave.lpDSBStreamBuffer->Unlock((LPVOID)lpWrite1, dwLen1, (LPVOID)lpWrite2, dwLen2);
    m_wiWave.dwNextWriteOffset += dwWriteLength;
    if(m_wiWave.dwNextWriteOffset >= m_wiWave.dwBufferSize){
        m_wiWave.dwNextWriteOffset -= m_wiWave.dwBufferSize;
    }
    if(fRefillLostBuffer){
        m_wiWave.lpDSBStreamBuffer->Play(0, 0, DSBPLAY_LOOPING);
    }
    InterlockedExchange(&m_lInTimer, FALSE);

    return;
}









BOOL OpenBGM(LPSTR lpszFileName)
{
	if(LSEnable == TRUE){
        if(m_bFileOpen){
            m_bIsCloseOnRun = FALSE;
            // ������ �����ϱ� ���� �������̶�� ����� �Ѵ�.
            LSStopBackground(DSSTREAM_STOPF_NOREOPEN);
        }
        if((SetupStreamBuffer(lpszFileName)) != DS_OK){
            return FALSE;
        }else{
            m_bFileOpen = TRUE;
        }
    }
	return TRUE;
}

HRESULT SetupStreamBuffer(LPSTR lpszFilename)
{
    DSBUFFERDESC dsbd;
    HRESULT      dsRetVal;

    // This portion of the WAVE I/O is patterned after what's in DSTRWAVE, which
    // was in turn adopted from WAVE.C which is part of the DSSHOW sample.
    if(WaveOpenFile(lpszFilename, &m_wiWave.hmmio, &m_wiWave.pwfx, &m_wiWave.mmckInRIFF) != 0){
        return ERR_WAVE_OPEN_FAILED;
    }

    if(m_wiWave.pwfx->wFormatTag != WAVE_FORMAT_PCM){
        WaveCloseReadFile(&m_wiWave.hmmio, &m_wiWave.pwfx);
        return ERR_WAVE_INVALID_FORMAT;
    }
    // Seek to the data chunk
    if(WaveStartDataRead(&m_wiWave.hmmio, &m_wiWave.mmck, &m_wiWave.mmckInRIFF) != 0){
        WaveCloseReadFile(&m_wiWave.hmmio, &m_wiWave.pwfx);
        return ERR_WAVE_CORRUPTED_FILE;
    }
    // As a side note, mmck.ckSize will be the size of all the data in this file.
    // That's something which might be handy when calculating the length...

    // Calculate a buffer length, making sure it is an exact multiple of the
    // buffer segment size.
    m_wiWave.dwBufferSize = ((DWORD)m_wiWave.pwfx->nAvgBytesPerSec * (((NUM_BUFFER_SEGMENTS * PLAYBACK_TIMER_PERIOD) / 10)) / 100);

    // Create the secondary DirectSoundBuffer object to receive our sound data.
    memset(&dsbd, 0, sizeof(DSBUFFERDESC));
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2 | m_SecondBuffFocus;
    dsbd.dwBufferBytes = m_wiWave.dwBufferSize;
    dsbd.lpwfxFormat   = m_wiWave.pwfx;



	//ktj ��ħ
    dsRetVal = lpDirectSound->CreateSoundBuffer(&dsbd, &lpDSBStreamBuffer, NULL);

    if(dsRetVal != DS_OK){
        return ERR_CREATEDSB_FAILED;
    }

    m_wiWave.lpDSBStreamBuffer = lpDSBStreamBuffer;
    m_wiWave.bFoundEnd         = FALSE;
    m_wiWave.dwBytesRemaining  = 0;
    m_wiWave.bLoopFile         = TRUE;

    LSFillDataBuffer();

    m_wiWave.bDonePlaying      = FALSE;

    if(lpDSBStreamBuffer){
		lpDSBStreamBuffer->SetVolume((((VOL_MAX+VOL_SHIFT) * VOL_MULT)));
		lpDSBStreamBuffer->SetPan(((((PAN_MAX/2)+PAN_SHIFT) * PAN_MULT)));
		lpDSBStreamBuffer->SetFrequency(m_wiWave.pwfx->nSamplesPerSec);
    }
    return DS_OK;
}


void PlayBGM(void)
{
	if(LSEnable == TRUE){
        if(m_bPlaying){
            m_bIsCloseOnRun = FALSE;
            LSStopBackground(0L);
        }
        if(m_bFileOpen && lpDSBStreamBuffer){
            // ���� ������ġ�� ���� �� ó������...
            lpDSBStreamBuffer->SetCurrentPosition(0);
            lpDSBStreamBuffer->Play(0, 0, DSBPLAY_LOOPING);
        }else{
            m_bPlaying   = FALSE;
            m_bTimerInit = FALSE;
            return;
        }

        if(timeBeginPeriod(PLAYBACK_TIMER_PERIOD / PLAYBACK_OVERSAMPLE ) != 0){
            // Ÿ�̸Ӹ� ������ �ʴ´�.
            lpDSBStreamBuffer->Stop();
            m_bPlaying   = FALSE;
            m_bTimerInit = FALSE;
            return;
        }else{
            m_lInTimer = FALSE;
            if((m_uTimerID = timeSetEvent(PLAYBACK_TIMER_PERIOD / PLAYBACK_OVERSAMPLE,
                                        PLAYBACK_TIMER_ACCURACY, LSTimeRunFuncForDSound,
                                        (DWORD)0, TIME_PERIODIC )) != 0){
                m_bTimerInit = TRUE;
            }
        }
        m_bPlaying = TRUE;
    }
}

void StopBGM(void)
{
	if(LSEnable == TRUE && m_bPlaying)
		LSStopBackground(0);
}

void SetVolumeBGM( DWORD dwVol )
{
	if(LSEnable == TRUE && m_bPlaying) {
		lpDSBStreamBuffer->SetVolume( (dwVol+VOL_SHIFT) * VOL_MULT); 
	}
}




BOOL Init_BGM(HWND hwnd)
{
    m_hWnd          = hwnd;

	lpDirectSound = NULL;

	lpDSBStreamBuffer = NULL;

	m_lInTimer        = FALSE;
	m_bTimerInit      = FALSE;
	m_bPlaying        = FALSE;
	m_bIsCloseOnRun   = FALSE;
	m_bFileOpen       = FALSE;
	m_uTimerID        = 0;
	m_SecondBuffFocus = 0;

    HRESULT         dsrval;

	dsrval = DirectSoundCreate(NULL, &lpDirectSound, NULL);

    if(SUCCEEDED(dsrval) && (NULL != lpDirectSound)){
        //dsrval = lpDirectSound->Initialize(NULL);

        //if(SUCCEEDED(dsrval)){
            dsrval = lpDirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
		//}else{
		//	dsrval = FALSE;
		//}

		if(!SUCCEEDED(dsrval)){
        	dsrval        = FALSE;
            lpDirectSound->Release();
	        lpDirectSound = NULL;
	    }
    }
	
	else{
		//ktj: ����ī����»�������� �޽����ڽ�������.
		//MessageBox(hwnd, "Failed to create DirectSound COM object", "Lemon Sound Error!", MB_OK | MB_ICONSTOP);
    }
	

    if(SUCCEEDED(dsrval)){
        LSEnable = TRUE;
		return TRUE;
    }else{
		//CoUninitialize();
        LSEnable = FALSE;
		return FALSE;
    }
	



	LSEnable = TRUE;

	return TRUE;
}


void Uninit_BGM(void)
{
	if(LSEnable == TRUE){
		
        LSStopBackground(DSSTREAM_STOPF_NOREOPEN | DSSTREAM_STOPF_NOEXIT);


        if(m_bTimerInit){
            timeKillEvent(m_uTimerID);
            timeEndPeriod(PLAYBACK_TIMER_PERIOD / PLAYBACK_OVERSAMPLE);
            // Busy wait for timer func to exit
            while(InterlockedExchange(&m_lInTimer, TRUE)) Sleep(100);
            m_bTimerInit = FALSE;
        }
		
		
		if (lpDSBStreamBuffer) {
			lpDSBStreamBuffer->Stop();
            lpDSBStreamBuffer->Release();
			lpDSBStreamBuffer = NULL;
        }


        // DSOUND ������Ʈ ����
        if(lpDirectSound != NULL){
            lpDirectSound->Release();
            lpDirectSound = NULL;
            //CoUninitialize();
        }

		
    }
}

// pluto ��Ÿ���� ä�� ������ ������ �������� ���� �����
void InitDirectSound()
{
	lpDirectSound = NULL;

	rval_g_lpDs = DirectSoundCreate(NULL, &g_lpDS, NULL);

	dsrval_lpDirectSound = DirectSoundCreate(NULL, &lpDirectSound, NULL);
}

//=======================================================================end
//sound ��� wav�� Ÿ�̸ӷ� �о ����
//=======================================================================end