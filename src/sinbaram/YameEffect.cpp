/*----------------------------------------------------------------------------*
*	���ϸ� :  AssaEffect.cpp	
*	�ϴ��� :  ����Ʈ 
*	�ۼ��� :  ����������Ʈ 2002�� 11��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"

/*----------------------------------------------------------------------------*
*							���� ���� 
*-----------------------------------------------------------------------------*/	

cYAMEEFFECT *cYameEffect[YAMEEFFECT_MAX];  //����Ʈ ���� 
sinEFFECT_ZDEPTH sinEffect_Zdepth4[YAMEEFFECT_MAX];//������ ���� �ε��� ����ü 
int Yame;

/*----------------------------------------------------------------------------*
*							Key Down
*-----------------------------------------------------------------------------*/	
void YameEffectKeyDown()
{

	POINT3D Posi = {lpCurPlayer->pX,lpCurPlayer->pY+10000,lpCurPlayer->pZ};  
	
	if(sinGetKeyClick('9')){  
		//SetYameEffect(500,"star01M_04.bmp",lpCurPlayer,0);
		//SetAssaEffect(100,"spark01_01.bmp",0,&Posi,0);

	}
	//lpCharSelPlayer	//�ѹ� ������ ����� ������ 
	//lpCharMsTrace		//��� ������ ����� ������
	


}
/*----------------------------------------------------------------------------*
*								Init
*-----------------------------------------------------------------------------*/	
void InitYameEffect()
{
	/////////// ���ҽ� �ʱ�ȭ 
	//InitAssaRes(); 

	/////////// ���ҽ��� �д� �Լ�.
	//LoadAssaRes();

	/////////// �߸� ����Ʈ �ʱ�ȭ 
	memset(cYameEffect,0,sizeof(cYAMEEFFECT));

}
/*----------------------------------------------------------------------------*
*								Draw
*-----------------------------------------------------------------------------*/	
void DrawYameEffect(POINT3D *pPosi , POINT3D *pAngle)
{

	int i=0;
	int IndexCount = 0;

	///////////////////  ZDepth�� ���Ѵ�
	for(i = 0 ; i < YAMEEFFECT_MAX ; i++){          
		if(cYameEffect[i] && !cYameEffect[i]->Bone && cYameEffect[i]->Time >0){
			smRender.SetCameraPosi(pPosi->x,pPosi->y,pPosi->z,pAngle->x,pAngle->y,pAngle->z);
			for(int j = 0 ; j < YAMEEFFECT_MAX ;j++){   
				if(!sinEffect_Zdepth4[j].Index){
					sinEffect_Zdepth4[j].ZDepth = smRender.GetDepthZ(cYameEffect[i]->Posi.x   , cYameEffect[i]->Posi.y , cYameEffect[i]->Posi.z);
					sinEffect_Zdepth4[j].Index = i+1;
					IndexCount++;
					break;
				}
			}
		}
	}
	//////////////////// ����   
	sinBubbleSort(sinEffect_Zdepth4,IndexCount-1);
	for(i = 0 ; i < IndexCount ; i++)
	{   
		if(sinEffect_Zdepth4[i].Index)
		{ 
			renderDevice.BeginScene();
			if(cYameEffect[sinEffect_Zdepth4[i].Index-1]->pChar)
			{
				if(cYameEffect[sinEffect_Zdepth4[i].Index-1]->pChar->FlagShow !=0){ //ĳ���͸� �׸����������� ����Ʈ�� �׸����ʴ´�
					cYameEffect[sinEffect_Zdepth4[i].Index-1]->Draw(pPosi,pAngle);	
				}
			}
			else
				cYameEffect[sinEffect_Zdepth4[i].Index-1]->Draw(pPosi,pAngle);	
			renderDevice.EndScene();
		}
	}	
	memset(sinEffect_Zdepth4,0,sizeof(sinEFFECT_ZDEPTH)*YAMEEFFECT_MAX);
}
/*----------------------------------------------------------------------------*
*								Main
*-----------------------------------------------------------------------------*/	
void MainYameEffect()
{

	for(int  i = 0 ; i < YAMEEFFECT_MAX ; i++){ 
		if(cYameEffect[i]){
			cYameEffect[i]->Time++;
			cYameEffect[i]->Main();

			if(cYameEffect[i]->Time > 0 ) // -Time ����
				//MoveAssaEffect(cYameEffect[i]);

			if(cYameEffect[i]->Time >= cYameEffect[i]->Max_Time){
				delete cYameEffect[i];
				cYameEffect[i] = NULL;

			}
		}
	}
}
/*----------------------------------------------------------------------------*
*								Close
*-----------------------------------------------------------------------------*/	
void CloseYameEffect()
{


}
/*----------------------------------------------------------------------------*
*							  Move
*-----------------------------------------------------------------------------*/	
void MoveYameEffect(cASSAEFFECT *pEffect)
{

}
/*----------------------------------------------------------------------------*
*							  Stop
*-----------------------------------------------------------------------------*/	
void StopYameEffect(cASSAEFFECT *pEffect)
{
	//�ð����� �������� 
	//pEffect->Time = pEffect->Max_Time; 
}

/******************************************************************************************
*								Setting �Լ� 
*******************************************************************************************/	
/*----------------------------------------------------------------------------*
*							Set Assa Effect
*-----------------------------------------------------------------------------*/	
int  SetYameEffect(int MaxTime , char *FileName , smCHAR *pChar , POINT3D *pPosi , int AddHeight ,int BlendTpye)
{
	char TempFileName[64];
	char FileFormat[32];

	WORD wBlendType = 0;
	switch(BlendTpye){
		case 0:
			wBlendType = SMMAT_BLEND_LAMP; //���� �⺻������ ���� Tpye�� LAMP�� �����Ǿ��ִ�
		break;
		case 1:
			wBlendType = SMMAT_BLEND_ALPHA;
		break;
		case 2:
			wBlendType = SMMAT_BLEND_COLOR;
		break;
		case 3:
			wBlendType = SMMAT_BLEND_SHADOW;
		break;
		case 4:
			wBlendType = SMMAT_BLEND_ADDCOLOR;
		break;
		case 5:
			wBlendType = SMMAT_BLEND_INVSHADOW;
		break;
	}

	FileFormat[0] = 0; //Ȥ�ø� ������ ���� ������츦 ����� �ʱ�ȭ���ش�

	lstrcpy(TempFileName , FileName);
	int len=lstrlen(TempFileName);
	for(int i = 0; i < len ; i++){
		if(TempFileName[i] == '.'){
			strcpy(FileFormat,&TempFileName[i+1]);  //Ȯ���� 
			break;
		}
	}
	
	for(int  i = 0 ; i < YAMEEFFECT_MAX ; i++){
		if(!cYameEffect[i]){ 
			//////// BONE ����
			if( stricmp(FileName, "Bone") == 0 ){
				cYAMEFACE *pFace = new cYAMEFACE; 
				cYameEffect[i] = pFace; //�⺻ Ŭ������ �Ҵ� 
				cYameEffect[i]->Bone = 1;


			}
			if( stricmp(FileFormat, "bmp") == 0 || stricmp(FileFormat, "tga") == 0){
				cYAMEFACE *pFace = new cYAMEFACE; 
				cYameEffect[i] = pFace; //�⺻ Ŭ������ �Ҵ� 
				cYameEffect[i]->Face.r = 255;
				cYameEffect[i]->Face.g = 255;
				cYameEffect[i]->Face.b = 255;
				cYameEffect[i]->Face.Transparency = 255;
				cYameEffect[i]->Face.TexRect = sinTexRect;
				cYameEffect[i]->Face.MatNum = *(int *)AssaSearchRes(FileName,wBlendType);
				
			}
			if(stricmp(FileFormat, "ase") == 0 ){
				cYAMEMESH *pMesh = new cYAMEMESH;
				cYameEffect[i] = pMesh; //�⺻ Ŭ������ �Ҵ� 
				cYameEffect[i]->PatMesh = (smPAT3D *)AssaSearchRes(FileName,wBlendType);

			}
			if(pChar){
				cYameEffect[i]->Posi.x = pChar->pX;
				cYameEffect[i]->Posi.y = pChar->pY;
				cYameEffect[i]->Posi.z = pChar->pZ;
				cYameEffect[i]->pChar = pChar;
//				if(cYameEffect[i]->pChar == lpCurPlayer)cYameEffect[i]->MyPointerFlag = 1;

			}
			if(pPosi){
				cYameEffect[i]->Posi.x = pPosi->x;
				cYameEffect[i]->Posi.y = pPosi->y;
				cYameEffect[i]->Posi.z = pPosi->z;
			}
			//lstrcpy(cYameEffect[i]->szFileName,FileName); //�����̸��� ī���Ѵ�
			cYameEffect[i]->AddHeight = AddHeight;
			cYameEffect[i]->Posi.y += AddHeight;
			cYameEffect[i]->Max_Time = MaxTime;
//			cYameEffect[i]->CenterPosi = cYameEffect[i]->Posi;
			if(pChar){ //�⺻������ Angle�� �ڽ��� �������� �Ѵ� 
				cYameEffect[i]->Angle.y = (-(pChar->Angle.y&ANGCLIP)+ANGLE_180)&ANGCLIP;
			}
			return i;
		}
	}
	return FALSE;
	
}
