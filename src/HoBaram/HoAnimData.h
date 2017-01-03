/*----------------------------------------------------------------------------*
*	���ϸ� :  HoAnimData.h
*	�ϴ��� :  ���ϸ��̼� ����Ÿ ����  
*	�ۼ��� :  ����������Ʈ 2000.1 
*	������ :  ȫȣ��.
*-----------------------------------------------------------------------------*/	

#ifndef _HO_ANIM_DATA_
#define _HO_ANIM_DATA_

//AnimFrame Data Flag
#define INFO_NONE				0x00000000
#define	INFO_IMAGE				0x00000001
#define	INFO_DELAY				0x00000002
#define INFO_ALPHA				0x00000004

//Ȯ��1 (����� ����)
#define	INFO_SIZEWIDTH			0x00000008

//Ȯ��2 (����� ����, ���� ���ϰ�, ȸ���� �ǰ� RGB���� ����)
#define INFO_ANGLE				0x00000010
#define	INFO_SIZEHEIGHT			0x00000020
#define INFO_RGB				0x00000040

#define INFO_DEFAULT			(INFO_IMAGE | INFO_DELAY | INFO_ALPHA)
#define INFO_ONESIZE			(INFO_IMAGE | INFO_DELAY | INFO_ALPHA | INFO_SIZEWIDTH)
#define INFO_ONESIZEANGLE		(INFO_IMAGE | INFO_DELAY | INFO_ALPHA | INFO_SIZEWIDTH | INFO_ANGLE)

struct hoAnimFrame
{
	//����Ʈ..
	unsigned short ImageNum;		//Frame�� �̹����� ��ȣ....
	unsigned short Delay;			//Frame�� Delay
	BYTE AlphaValue;				//Frame�� ���İ� ��ȭ��..
	
	//Ȯ��1...
	unsigned short SizeWidth;		//�⺻ ��jƮ ������ Width���� ����..(����, ���� ���̰� ����)
	
	//Ȯ��2...
	unsigned short SizeHeight;
	float Angle;					//Frame�� �ޱ� ��ȭ��.. (-360~360)����..
	BYTE ColorRValue;				//Frame�� �÷���..					
	BYTE ColorGValue;
	BYTE ColorBValue;
};

class HoAnimDataMgr;
//���ϸ��̼� ������ ����Ÿ ����...
class HoAnimSequenceData
{
public:
	HoAnimSequenceData();
	~HoAnimSequenceData();

	hoAnimFrame *AnimFrame;
	int			AnimFrameCount;
	int			StartBlendValue;
	int			BlendType;

public:
	int  DataFileIndex;				//�̹��� ����Ÿ ���� ����Ʈ�� �ε��� ��ȣ..
	char IniName[64];

private:
	BYTE		InfoFlag;					//���� Flag


public:
	int Load(char *iniName, HoAnimDataMgr *animationData = NULL);
	int Init();

	BYTE GetAnimInfo()
	{
		if(InfoFlag == INFO_DEFAULT)
		{
			return INFO_DEFAULT;
		}
		else if(InfoFlag == INFO_ONESIZE)
		{
			return INFO_ONESIZE;
		}
		else if(InfoFlag == INFO_ONESIZEANGLE)
		{
			return INFO_ONESIZEANGLE;
		}
		
		return INFO_NONE;
	}
};

//���ϸ��̼� �̹��� ����Ÿ ����...
class HoAnimImageData
{
private:
	int  MaterialNum;			//Material ��ȣ..(smMaterialGroup��)
	int	 AnimationCount;		//Animation�� ����..

public:
	char IniName[64];			//ini Name...

	HoAnimImageData();
	~HoAnimImageData();

	char *GetIniName() { return IniName;}
	int Load(char *iniName);
	int GetAnimationCount(){ return AnimationCount; }
	int GetMaterialNum(){ return MaterialNum; }
	int Init();

private:
	int Load(char *textureName, int count);

};


#define MAX_IMAGEDATA_BUFFER	100
#define MAX_SEQUENCEDATA_BUFFER	100
class HoAnimDataMgr
{
public:
	HoAnimDataMgr();
	~HoAnimDataMgr();
	
	int SequenceLoad(char *iniName);
	int	ImageLoad(char *iniName);
	
public:
	HoAnimSequenceData SequenceDataBuffer[MAX_SEQUENCEDATA_BUFFER];
	int				   SequenceDataBufferCount;

	HoAnimImageData    ImageDataBuffer[MAX_IMAGEDATA_BUFFER];
	int				   ImageDataBufferCount;
	
	
	int	GetSearchSequenceData(char *iniName);
	int	GetSearchAnimImageData(char *iniName);
		
	int	Init();

	//aniDataIndex ��ȣ�� ������ ���� Material�� ã�´�.
	int GetMaterialNum(int aniDataIndex)
	{
		if(aniDataIndex >= 0 && aniDataIndex < SequenceDataBufferCount)
			return ImageDataBuffer[SequenceDataBuffer[aniDataIndex].DataFileIndex].GetMaterialNum();
			
		return -1;
	}
};
extern HoAnimDataMgr AnimDataMgr;

#endif