//######################################################################################
//�� �� �� : 2004-04-20 ���� 10:53:30
//�� �� �� :
//��    �� :
//�� �� �� : �� �� ��
//######################################################################################//
#pragma once


//
void CreateFontImage(void);

// fRatio=1.f �̸� ���� ũ��.
int  DrawFontImage( char *lpStr, int xPos, int yPos, DWORD rgba, float fRatio=1.f );
int  DrawSpecialFontImage( int SpecialNum, int xPos, int yPos, DWORD rgba, float fRatio=1.f );

