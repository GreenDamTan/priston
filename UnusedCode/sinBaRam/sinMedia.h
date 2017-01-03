/*----------------------------------------------------------------------------*
*	���ϸ� :  sinMedia.h
*	�ϴ��� :  ������ ���
*	�ۼ��� :  ����������Ʈ 2005�� 1��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#ifndef _SINMEDIA_HEADER_ 
#define _SINMEDIA_HEADER_ 


#include "amstream.h"
/*----------------------------------------------------------------------------*
*						Movie Ŭ���� 
*-----------------------------------------------------------------------------*/	
class SINMOVIE
{ 
	
	int StartMovie();

	IMultiMediaStream *pMMStream;              // ��Ʈ������
	IMediaStream *pPrimaryVidStream;     
	IDirectDrawMediaStream *pDDStream;     
	IDirectDrawSurface *pSurface;     
	IDirectDrawStreamSample *pSample;
	IDirectDrawSurface      *pMovieSurface;
	char FileName[MAX_PATH];
	RECT DRect,SRect;  // SRect : Clip Size(Prepare���� ����)    DRect : ���� ��ġ �� ũ��
	int MovieState; // 0:NULL 1:PLAYING 2 :END OR STOP ���� �˾ƺ��� ���� ����߽��ϴ�. ^^;;
	bool Open(IDirectDraw* pDD, char* name, IMultiMediaStream** ptMMStream);
	public :
	SINMOVIE();
	~SINMOVIE(); // call ShutDown();
	void SetBlitPos(int t,int b,int l,int r); 
	void SetBlitPos(RECT dest); 
	int GetMovieState() { return MovieState; }
	void ShutDown();
	bool PrepareMovie(IDirectDraw* pDD, IDirectDrawSurface *pDDSurface, char* FName);
	bool RenderStreamToSurface();
	void RenderStreamToSurfaceAll();

};
extern SINMOVIE sinMovie;
#endif