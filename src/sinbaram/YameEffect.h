/*----------------------------------------------------------------------------*
*	���ϸ� :  YameEffect
*	�ϴ��� :  ���ο� ����Ʈ 
*	�ۼ��� :  ����������Ʈ 2003�� 6��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#ifndef _YAMEEFFECT_HEADER_

#define _YAMEEFFECT_HEADER_

#define YAMEEFFECT_MAX		5000

/*----------------------------------------------------------------------------*
*							FADE OPTION
*-----------------------------------------------------------------------------*/	
#define YAME_DEFAULT_FADE	0x00000000
#define YAME_SMOOTH_FADE	0x00000001
#define YAME_NOT_FADEIN		0x00000010
#define YAME_NOT_FADEOUT	0x00000100


/*----------------------------------------------------------------------------*
*							FACE KIND
*-----------------------------------------------------------------------------*/	
#define YAMEFACE_BILLBOARD	0
#define YAMEFACE_WORLD		1
#define YAMEFACE_SCALE		2
#define YAMEFACE_TRACE		3

/*----------------------------------------------------------------------------*
*							YAME STRUCT
*-----------------------------------------------------------------------------*/	
// �ִϸ��̼�
struct sYAME_ANI{
	int		Cnt;
	int		DelayTime;
	int		MaxCnt;
	int		MatIndex[10];   //�ִϸ��̼��� �� ��Ʈ���� ��ȣ �ε��� 
	int		Reverse;		//�ִϸ��̼� �����÷� ( 1�ٸ����� 2 ���� ) 

};

// ���� ����
struct sYAME_FADE{
	DWORD   FADE_CODE;
	int     MaxAlpha; //���� ���İ��� ���Ѵ� 
	int     NowAlpha; //���� ���İ��� ���Ѵ� 
	float   AlphaAmount;	
	int		AlphaStartTime;

};

// ȸ�� 
struct sYAME_ROTATE{
	int		RAngle;
	int		RAngleSpeed;
	POINT3D RPosi; 
	POINT3D CenterPosi; //ȸ���߽���ǥ
};

// Angle ȸ�� 
struct sYAME_ANGLE_ROTATE{
	POINT3D	ARotate;
	POINT3D	ARotateSpeed;
	int     FACEY;

};

// �� 
struct sYAME_COLOR{
	int r;
	int g;
	int b;
	int a;

};

// ������ 
struct sYAME_SCALE{
	POINT3D ScalePosi;
	POINT3D ScaleSpeed;
	int     CenterFlag;

};

struct sYAME_BILLBOARD{
	int FaceY;
	int ZposiFlag;

};




class cYAMEEFFECT;

/*----------------------------------------------------------------------------*
*						  YAMEEFFECT ���� Ŭ����  
*-----------------------------------------------------------------------------*/	
class  cYAMEEFFECT{
public:
	
	DWORD   CODE;				//����Ʈ ���� �ڵ� 
	DWORD	MOVECODE;			//�⺻ ������ 
	int		Time;				
	int		Max_Time;
	int		Bone;

	///////////// ����
	smFACE2D	Face;
	smPAT3D		*PatMesh;
	///////////// ����
	int		Index;
	int		ZDepth;

	///////////// ��ǥ ����
	POINT3D	Posi;			//��ǥ
	POINT3D	DesPosi;		//������ ��ġ ��ǥ
	POINT3D	Angle;			//�ޱ�

	int		AddHeight;		//������ ��ǥ
	
	///////////// ������
	POINT3D	MoveSpeed;

	///////////// ĳ����
	smCHAR	*pChar;			//�ڽ�
	smCHAR	*desChar;		//���õ� ĳ����
	
	/////////////FACE TYPE
	int FACE_TYPE;
	int FACE_POSI;
	///////////// Size
	int w;
	int h;
	
	///////////// �ִϸ��̼�
	sYAME_ANI	Ani;
	
	///////////// ���̵� 
	sYAME_FADE  Fade;

	///////////// ȸ��  
	sYAME_ROTATE Rotate;

	///////////// Angle ȸ�� 
	sYAME_ANGLE_ROTATE Angle_Rotate;

	///////////// �÷� 
	sYAME_COLOR	Color;

	///////////// ������ 
	sYAME_SCALE	Scale;

	///////////// ������ 
	sYAME_BILLBOARD BillBoard;

	///////////// FACE ���Ḯ��Ʈ 
	cASSATrace *cAssaTrace; 



public:
	cYAMEEFFECT()
	{
		memset(this,0,sizeof(cASSAEFFECT));
		cASSATrace *pAssaTrace = new cASSATrace; //Face ���Ḯ��Ʈ 
		cAssaTrace = pAssaTrace;

	}
	virtual ~cYAMEEFFECT()
	{
		if(cAssaTrace)delete cAssaTrace;
	}
	virtual	void Main(){}
	virtual void Draw(POINT3D *pPosi , POINT3D *pAngle){}
	
	//ĳ���� ���� ���¸� üũ�Ѵ� (�ٲ����δ� , �׾��ų� , ������ �������)
	//int AssaReSetCharPointer(); AssaEffect��������ߴ� 

};

/*----------------------------------------------------------------------------*
*						  YAMEEFFECT FACE Ŭ���� 
*-----------------------------------------------------------------------------*/	
class cYAMEFACE : public cYAMEEFFECT{
public:
	
	void Main()
	{
		Face.x = Posi.x;	//��ǥ ����
		Face.y = Posi.y; 
		Face.z = Posi.z; 
		Face.width  = w;	//ũ�� ���� 
		Face.height = h;
		if(!Face.height){
			h = w;
			Face.height = Face.width; //ũ�⺸��
		}
		if(!Fade.MaxAlpha)Fade.MaxAlpha = 255; 

		switch(Fade.FADE_CODE){
			case YAME_DEFAULT_FADE: 
				/*
				AlphaStartTime = (int)((float)Max_Time/100.0f*25.5f);
				AlphaAmount = 255.0f/(float)AlphaStartTime;
				if(Time <= AlphaStartTime ){
					Face.Transparency += (int)AlphaAmount; 
					if(Face.Transparency >255)
						Face.Transparency = 255;
				}
				*/

			break;
			case YAME_SMOOTH_FADE:  

			break;
			case YAME_NOT_FADEIN: 

			break;
			case YAME_NOT_FADEOUT: 

			break;
		}

		/*
		//////// FADE IN
		if(!MaxAlphaAmount){
			if(!NotFadeIn){
				if(Time == 1 )Face.Transparency = 0;
				AlphaStartTime = (int)((float)Max_Time/100.0f*25.5f);
				AlphaAmount = 255.0f/(float)AlphaStartTime;
				if(Time <= AlphaStartTime ){
					Face.Transparency += (int)AlphaAmount; 
					if(Face.Transparency >255)
						Face.Transparency = 255;
				}
			}
		}
		else{
			if(!NotFadeIn){
				if(Time == 1 )Face.Transparency = 0;
				AlphaStartTime = (int)((float)Max_Time/100.0f*25.5f);
				AlphaAmount = (float)MaxAlphaAmount/(float)AlphaStartTime;
				if(Time <= AlphaStartTime ){
					if(AlphaAmount < 0 ) AlphaAmount = 1;
					Face.Transparency += (int)AlphaAmount; 
					if(Face.Transparency >MaxAlphaAmount)
						Face.Transparency = MaxAlphaAmount;
				}
			}
			else{
				if(Time == 1 ) //�ѹ��� ���İ��� �������ش� 
					Face.Transparency = MaxAlphaAmount;
			}
		}
		
		//////// FADE OUT
		if(!NotFadeOut){
			if(!MaxAlphaAmount){
				AlphaStartTime = (int)((float)Max_Time/100.0f*25.5f);
				if((255.0f/(float)AlphaStartTime) >= 1){
					AlphaAmount = 255.0f/(float)AlphaStartTime;

				}
				if(Time > AlphaStartTime && AlphaAmount >= 1)
					Face.Transparency -= (int)AlphaAmount;
			}
			else{
				AlphaStartTime = (int)((float)Max_Time/100.0f*25.5f);
				if(((float)MaxAlphaAmount/(float)AlphaStartTime) >= 1){
					AlphaAmount = (float)MaxAlphaAmount/(float)AlphaStartTime;

				}
				if(Time > AlphaStartTime && AlphaAmount >= 1)
					Face.Transparency -= (int)AlphaAmount;

			}
		}
		*/
		/*
		//////// Animation
		if(AniReverse == 2){ //����
			if(AniDelayTime &&  (Time % AniDelayTime) == 0 ){
				AniCount--;
				if(AniCount < 1){
					if(	AniReverse == 2)AniReverse = 1;

				}
				Face.MatNum = AniMatIndex[AniCount];
			}

		}
		else{ //�ٸ� ����
			if(Time >0  && AniDelayTime &&  (Time % AniDelayTime) == 0 ){
				Face.MatNum = AniMatIndex[AniCount];
				AniCount++;
				if(AniCount > AniMaxCount-1){
					if(	AniReverse == 1)AniReverse = 2;
					else AniCount = 0;
				}
			}
		}
		//ĳ���� �������¸� üũ�ؼ� ����Ʈ�� ���ش� 
		AssaReSetCharPointer();
		*/

	}
	void Draw(POINT3D *pPosi , POINT3D *pAngle)
	{
		
		if(!Face.MatNum)
			return;
		
		// Turn on Z-buffering
		//lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE ); (������ ����°� ���Ƽ� �����)
		//lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
		renderDevice.SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
		//	lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_USEW );//TRUE );


		// null out the texture handle
		renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, 0);

		// turn on dithering
		renderDevice.SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE);

		renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_WRAP);
		renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE);

		// D3DFILTER_LINEAR
		renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_LINEAR);
		renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEAR);

		renderDevice.SetRenderState(D3DRENDERSTATE_SPECULARENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);

		renderDevice.SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE);

		renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);

		renderDevice.SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
		renderDevice.SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFN_LINEAR);


		smRender.Init(); //������ �ʱ�ȭ 

		smMaterialGroup->smMaterial[Face.MatNum].TwoSide = TRUE; //ī�޶� ȸ���� ������� ���̱� 
			
		smRender.SetMaterialGroup(smMaterialGroup);  //�о�帰 ��Ʈ������ ���� 
			

		//���� ���� Frame�� �����Ѵ�..
		smMaterialGroup->SetMaterialAnimFrame( &smMaterial[Face.MatNum], 2);
			
		//Camera���� �Է�.
		smRender.SetCameraPosi(pPosi->x, pPosi->y, pPosi->z,pAngle->x,pAngle->y,pAngle->z);

		switch(FACE_TYPE){
			case YAMEFACE_BILLBOARD:
				//BillBoard
				if(!AssaAddFace2D(&Face,BillBoard.ZposiFlag,BillBoard.FaceY))return;
			break;
			case YAMEFACE_WORLD:
				//if(!AssaAddFaceWorld(&Face,&Angle))return;
			break;
			case YAMEFACE_SCALE:
				//if(!AssaAddFace2DLine(&Face,&ScalePosi,CenterFlag))return;
			break;
			case YAMEFACE_TRACE:
				//if(!AssaAddFaceTrace(&Face,cAssaTrace))return;
			break;
		}

		smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
		smRender.ClipRendFace();
		smRender.GeomVertex2D(FALSE);
		smRender.RenderD3D();
	}
};

/*----------------------------------------------------------------------------*
*						  YAMEEFFECT MESH Ŭ���� 
*-----------------------------------------------------------------------------*/	
class cYAMEMESH : public cYAMEEFFECT{
public:
/*
	void Main()
	{

		//////// �����ӿ� �°� Max_Time�� ���Ѵ� 
		if(!Max_Time){
			Max_Time = AniMaxCount*AniDelayTime;
		}
		if(!AniCount)AniCount=1; //�������� �������ش� 
		if(!AniDelayTime)AniDelayTime=1; 

		////////Animation
		if(Time > 0 && AniDelayTime &&  (Time % AniDelayTime) == 0 ){
			AniCount++;
			if(AniCount >= AniMaxCount )
				AniCount = 0;
		}


		//////// FADE IN
		if(!NotFadeIn){
			if(!MaxAlphaAmount){
				if(Time == 1 ){
					Color.A = -255;Color.R = -255;Color.G = -255;Color.B = -255;
				}
				AlphaStartTime = (int)((float)Max_Time/100.0f*25.5f);
				if(AlphaStartTime > 255 )AlphaStartTime = 255; //�ִ�ð��� �������ش� 
				AlphaAmount = 255.0f/(float)AlphaStartTime;
				if(Time <= AlphaStartTime ){
					Color.A += (int)AlphaAmount;Color.R += (int)AlphaAmount;Color.G += (int)AlphaAmount;Color.B += (int)AlphaAmount;
					if(Color.A > 0 )Color.A = 0;
					if(Color.R > 0 )Color.R = 0;
					if(Color.G > 0 )Color.G = 0;
					if(Color.B > 0 )Color.B = 0;
				}
			}
			else{
				if(Time == 1 ){
					Color.A = -255;Color.R = -255;Color.G = -255;Color.B = -255;
				}
				AlphaStartTime = (int)((float)Max_Time/100.0f*25.5f);
				if(AlphaStartTime > MaxAlphaAmount )MaxAlphaAmount = 255; //�ִ�ð��� �������ش� 
				AlphaAmount = (float)MaxAlphaAmount/(float)AlphaStartTime;
				if(Time <= AlphaStartTime ){
					Color.A += (int)AlphaAmount;Color.R += (int)AlphaAmount;Color.G += (int)AlphaAmount;Color.B += (int)AlphaAmount;
					if(Color.A > 0 )Color.A = 0;
					if(Color.R > 0 )Color.R = 0;
					if(Color.G > 0 )Color.G = 0;
					if(Color.B > 0 )Color.B = 0;
				}
			}
		}		
		//////// FADE OUT
		if(!NotFadeOut){
			if(!MaxAlphaAmount){
				AlphaStartTime = (int)((float)Max_Time/100.0f*25.5f);
				if(AlphaStartTime > 255 )AlphaStartTime = 255; //�ִ�ð��� �������ش� 
				AlphaAmount = 255.0f/(float)AlphaStartTime;
				if(Time >= (Max_Time - AlphaStartTime)){
					Color.A -= (int)AlphaAmount;
					Color.R -= (int)AlphaAmount;
					Color.G -= (int)AlphaAmount;
					Color.B -= (int)AlphaAmount;
				}
			}
			else{
				AlphaStartTime = (int)((float)Max_Time/100.0f*25.5f);
				if(AlphaStartTime > MaxAlphaAmount )AlphaStartTime = MaxAlphaAmount; //�ִ�ð��� �������ش� 
				AlphaAmount = (float)MaxAlphaAmount/(float)AlphaStartTime;
				if(Time >= (Max_Time - AlphaStartTime)){
					Color.A -= (int)AlphaAmount;
					Color.R -= (int)AlphaAmount;
					Color.G -= (int)AlphaAmount;
					Color.B -= (int)AlphaAmount;
				}
			}
		}
		//ĳ���� �������¸� üũ�ؼ� ����Ʈ�� ���ش� 
		AssaReSetCharPointer();

	}
	void Draw(POINT3D *pPosi , POINT3D *pAngle)
	{

		BackUpColor.R = smRender.Color_R;	//���� ���������� Color�� ����Ѵ� 
		BackUpColor.G = smRender.Color_G;
		BackUpColor.B = smRender.Color_B; 
		BackUpColor.A = smRender.Color_A;

		smRender.Color_R = Color.R;			//Color ����
		smRender.Color_G = Color.G;
		smRender.Color_B = Color.B;
		smRender.Color_A = Color.A;
		if(PatMesh){
			PatMesh->Frame = AniCount*160;
			PatMesh->SetPosi(&Posi , &Angle);
			PatMesh->RenderD3D(pPosi,pAngle);
		}
		smRender.Color_R = BackUpColor.R;	//���� ���������� Color�� ���� 
		smRender.Color_G = BackUpColor.G;
		smRender.Color_B = BackUpColor.B; 
		smRender.Color_A = BackUpColor.A;

	}
*/
};

/*----------------------------------------------------------------------------*
*						  AssaEffect �⺻ �Լ�
*-----------------------------------------------------------------------------*/	
void InitYameEffect();

void DrawYameEffect(POINT3D *pPosi , POINT3D *pAngle);

void MainYameEffect();

void MoveYameEffect(cASSAEFFECT *pEffect);

void CloseYameEffect();

void StopYameEffect(cASSAEFFECT *pEffect);

void YameEffectKeyDown();

/*----------------------------------------------------------------------------*
*							  Setting �Լ� 
*-----------------------------------------------------------------------------*/	
//AssaEffect����
int  SetYameEffect(int MaxTime , char *FileName , smCHAR *pChar , POINT3D *pPosi , int AddHeight=0 , int BlendType=0); 

extern int Yame;  //�ε��� 
extern cYAMEEFFECT *cYameEffect[YAMEEFFECT_MAX]; //����Ʈ ���� 
#endif
