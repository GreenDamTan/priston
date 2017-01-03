/*----------------------------------------------------------------------------*
*	���ϸ� :  AssaEffect
*	�ϴ��� :  ����Ʈ 
*	�ۼ��� :  ����������Ʈ 2002�� 11��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#ifndef _ASSAEFFECT_HEADER_

#define _ASSAEFFECT_HEADER_

#define cASSAEFFECT_MAX		5000

//////////////////// ���ÿɼ� 
#define ASSA_ROTATE				0x0000000F //ȸ��
#define ASSA_MOVE				0x000000F0 //�̵�
#define ASSA_ANGLE_ROTATE		0x00000F00 //����ȸ��
#define ASSA_CHAR_POSI			0x0000F000 //ĳ���� ��ǥ�̵��� ���� ������ 
#define ASSA_MOVE_ANGLE_SPEED	0x000F0000 //������ �ӵ��� �°� �̵� 
#define ASSA_DECRESIZE			0x00F00000 //����� �پ���
#define ASSA_INCRESIZE			0x0F000000 //����� �þ��
#define ASSA_MOVE_GATHER		0xF0000000 //������

//////////////////// ���� ���� (8���� �Ѿ�⶧��) 
#define ASSA_MOVE_SPREAD		0x70000000 //������
#define ASSA_MOVE_RANDOM_ROTATE 0x70000001 //Ball Rotate

#define ASSA_ANG_CHAR			0x00000001 //�ڽ��� ��
#define ASSA_ANG_DESCHAR		0x00000002 //���� �ڽ��� �� (OBJ)
#define ASSA_ANG_BONE			0x00000003 //���� �ڽ��� ���� �� 
////////////////////

#define ASSAFADEOUT			0		//���õ����ʴ� �⺻�ɼ�
#define ASSAFADEIN			1

////////////////////
#define ASSAFACE_BILLBOARD	0
#define ASSAFACE_WORLD		1
#define ASSAFACE_SCALE		2
#define ASSAFACE_TRACE		3


/////////////////// Type
#define ASSATYPE_CIRCLE		1


/////////////////// Particle Code
//#define ASSA_PARTICLE_WAVE			0x10000000


class cASSAEFFECT;
struct sASSACOLOR {int R; int G; int B; int A;};
struct sASSASIZE  {int SameSize; int w; int h;};

/*----------------------------------------------------------------------------*
*						  ASSAEFFECT ���� Ŭ����  
*-----------------------------------------------------------------------------*/	
class  cASSAEFFECT{
public:
	DWORD	MOVECODE;
	DWORD	MOVECODESUB; //�ڵ� �ߺ��ɶ� ��� ���
	DWORD   CODE;
	int		Time;		
	int		Max_Time;
	int		Bone;
	char    szFileName[128];
	int		Power;

	///////////// ����
	smFACE2D	Face;
	smPAT3D		*PatMesh;
	int			Mat;
	smOBJ3D     *Obj;		//Ư��������Ʈ
	///////////// ����
	int		Index;
	int		ZDepth;

	///////////// FACE ���Ḯ��Ʈ 
	cASSATrace *cAssaTrace; 

	///////////// ��ǥ ����
	POINT3D	Posi;			//��ǥ
	POINT3D	Angle;			//�ޱ�
	POINT3D	TempPosi;		//��꿡 ���Ǵ� ��ǥ 
	POINT3D	TempAngle;		//��꿡 ���Ǵ� �ޱ� 
	POINT3D StartPosi;		//�߽������κ����� ������ǥ
	int		AddHeight;		//������ ��ǥ
	POINT3D	DesPosi;		//������ ��ġ ��ǥ

	///////////// �ִϸ��̼�
	int		AniCount;
	int		AniDelayTime;
	int		AniMaxCount;
	int		AniMatIndex[10];//�ִϸ��̼��� �� ��Ʈ���� ��ȣ �ε��� 
	int		AniReverse;		//�ִϸ��̼� �����÷� ( 1�ٸ����� 2 ���� ) 

	///////////// ������
	POINT3D	MoveSpeed;
	POINT3D	MovePosi;
	int     GatherSpeed;
	int		Gravity;

	///////////// ��ǥ ȸ��
	POINT3D	RDistance;
	int		RAngle;
	int		RAngleSpeed;
	POINT3D RPosi; 
	POINT3D CenterPosi; //ȸ���߽���ǥ

	///////////// Angle ȸ��
	POINT3D	ARotate;
	POINT3D	ARotateSpeed;
	int     FACEY;

	///////////// ĳ����
	smCHAR	*pChar;			//�ڽ�
	smCHAR	*desChar;		//���õ� ĳ����

	///////////// ���� ����
	int		NotFadeIn;		//Fade In Fade Out �� �з��Ѵ� 
	int		NotFadeOut;		//Fade In Fade Out �� �з��Ѵ� 
	int     MaxAlphaAmount; //���� ���İ��� ���Ѵ� 
	float   AlphaAmount;	
	int		AlphaStartTime;
	
	sASSACOLOR Color;
	sASSACOLOR BackUpColor;

	/////////////FACE TYPE
	int FACE_TYPE;

	///////////// ������
	POINT3D ScalePosi;
	POINT3D ScaleSpeed;

	int CenterFlag;

	///////////// �浹 üũ
	int		CrashFlag;

	///////////// Size
	sASSASIZE Size;
	sASSASIZE ReSize;
	///////////// Type
	int     Type;		 //�� , �׸� , ���� , ����� Type 

	///////////// �׼� ����Ʈ
	int		ActionIndex[10]; //10���� �׼��� �������ִ� 
	int		ActionTime[10];  //10���� �׼��� �ð��� �������ִ� 

	///////////// �׼� �ڵ� 
	DWORD   MOVECODE_List[10]; //�ڵ带 �ٲ���
	int     MOVECODE_Time[10];


	int     ZposiFlag;       //ĳ���� ���ʺκп� ���̽��������� �÷��� ����� 
	///////////// Temp
	int		Temp[10];		 //��� ���̴� Ttemp
	float	fTemp[10];		 //��� ���̴� Ttemp


	///////////// �ڽ��� ����Ʈ���� �ƴ����� üũ�ϴ� �÷�
	int     MyPointerFlag;

public:
	cASSAEFFECT()
	{
		memset(this,0,sizeof(cASSAEFFECT));
		cASSATrace *pAssaTrace = new cASSATrace; //Face ���Ḯ��Ʈ 
		cAssaTrace = pAssaTrace;

	}
	virtual ~cASSAEFFECT()
	{
		if(cAssaTrace)delete cAssaTrace;
	}
	virtual	void Main(){}
	virtual void Draw(POINT3D *pPosi , POINT3D *pAngle){}
	
	//ĳ���� ���� ���¸� üũ�Ѵ� (�ٲ����δ� , �׾��ų� , ������ �������)
	int AssaReSetCharPointer();

};

/*----------------------------------------------------------------------------*
*						  ASSAEFFECT FACE Ŭ���� 
*-----------------------------------------------------------------------------*/	
class cASSAFACE : public cASSAEFFECT{
public:
	
	void Main()
	{
		//�⺻��
		Face.x = this->Posi.x;			//��ǥ ����
		Face.y = this->Posi.y; 
		Face.z = this->Posi.z; 
		Face.width = this->Size.w;		//ũ�� ���� 
		Face.height = this->Size.h;
		if(!Face.height){
			this->Size.h = this->Size.w;
			Face.height = Face.width; //ũ�⺸��
		}

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

	}
	void Draw(POINT3D *pPosi , POINT3D *pAngle)
	{
		
		if(!Face.MatNum)return;
		
		// Turn on Z-buffering
		//lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE ); (������ ����°� ���Ƽ� �����)
		//lpD3DDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
		renderDevice.SetRenderState( D3DRENDERSTATE_ZFUNC,   D3DCMP_LESSEQUAL );
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
			case ASSAFACE_BILLBOARD:
				if(!AssaAddFace2D(&Face,ZposiFlag,FACEY))return;
			break;
			case ASSAFACE_WORLD:
				if(!AssaAddFaceWorld(&Face,&Angle))return;
			break;
			case ASSAFACE_SCALE:
				if(!AssaAddFace2DLine(&Face,&ScalePosi,CenterFlag))return;
			break;
			case ASSAFACE_TRACE:
				if(!AssaAddFaceTrace(&Face,cAssaTrace))return;
			break;
		}

		smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
		smRender.ClipRendFace();
		smRender.GeomVertex2D(FALSE);
		smRender.RenderD3D();
	}
};

/*----------------------------------------------------------------------------*
*						  ASSAEFFECT MESH Ŭ���� 
*-----------------------------------------------------------------------------*/	
class cASSAMESH : public cASSAEFFECT{
public:

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

};

/*----------------------------------------------------------------------------*
*						  AssaEffect �⺻ �Լ�
*-----------------------------------------------------------------------------*/	
void InitAssaEffect();

void DrawAssaEffect(POINT3D *pPosi , POINT3D *pAngle);

void MainAssaEffect();

void MoveAssaEffect(cASSAEFFECT *pEffect);

void CloseAssaEffect();

void StopAssaEffect(cASSAEFFECT *pEffect);

void AssaEffectKeyDown();

/*----------------------------------------------------------------------------*
*							  Setting �Լ� 
*-----------------------------------------------------------------------------*/	
//AssaEffect����
int  SetAssaEffect(int MaxTime , char *FileName , smCHAR *pChar , POINT3D *pPosi , int AddHeight=0 , int BlendType=0); 

//������ Speed�� �°� �����δ� 
int AssaGetMoveLocation(cASSAEFFECT *pEffect);			

// Angle�� �����Ѵ�
int AssaSetAngle(cASSAEFFECT *pEffect , int AngleKind);	 

//���� Posi�� �����Ѵ� 
int AssaSetStartPosi(cASSAEFFECT *pEffect);
/*----------------------------------------------------------------------------*
*							  ��ǥ ��� �Լ� 
*-----------------------------------------------------------------------------*/	
//������ Speed�� �°� �����δ� 
int AssaGetMoveLocation(cASSAEFFECT *pEffect); 

//���� ���� ��ǥ�� ���Ѵ� 
int AssaGetCirclePosi(cASSAEFFECT *pEffect, int DistanceZ , int Angle);

//Stage���� �浹�� üũ�Ѵ� 
int AssaCheckStageCrash(cASSAEFFECT *pEffect);
/*----------------------------------------------------------------------------*
*							   ETC �Լ� 
*-----------------------------------------------------------------------------*/	
//�ִϸ��̼��� �����Ѵ� 
int AssaGetAniFileMat(cASSAEFFECT *pEffect);

//����Ʈ�� ī���Ѵ� 
int AssaCopyEffect(cASSAEFFECT *pEffect);

//ĳ���� �����Ͱ� �ٲ� �ٽ� �������ش� 
int AssaReSetCharPointer(cASSAEFFECT *pEffect);
/*----------------------------------------------------------------------------*
*							   extern 
*-----------------------------------------------------------------------------*/	
extern int Assa;  //�ε��� 
extern cASSAEFFECT *cAssaEffect[cASSAEFFECT_MAX]; //����Ʈ ���� 
#endif
