/*----------------------------------------------------------------------------*
*	���ϸ� :  sin3D.cpp	
*	�ϴ��� :  3D���� �۾��� �Ѵ�
*	�ۼ��� :  ����������Ʈ 2002�� 1��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"

#define  _ASSA_EFFECT_START_
/*----------------------------------------------------------------------------*
*								�������� 			
*-----------------------------------------------------------------------------*/	
cSIN3D cSin3D;

int sinStarParticleKey = 0;
int TestCount2 = 0;
/*----------------------------------------------------------------------------*
*							Ŭ���� �ʱ�, ���� 
*-----------------------------------------------------------------------------*/	
cSIN3D::cSIN3D()
{

}
cSIN3D::~cSIN3D()
{

}
/*----------------------------------------------------------------------------*
*							     �ʱ�ȭ 
*-----------------------------------------------------------------------------*/	
void cSIN3D::Init()
{
	char *PartBlue		= "PartBlue";
	char *PartGreen		= "PartGreen";
	char *PartRed		= "PartRed";
	char *Partyellow	= "Partyellow";
	char szPath[128];

	sinEffect2D_Line = CreateTextureMaterial( "Image\\SinImage\\Effect\\Return1.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );
	
	//��ƼŬ Blue Red Green �� �ε��Ѵ� 
	for(int i=1; i < 5 ; i++){
		wsprintf(szPath,"image\\Sinimage\\Effect\\Particle\\%s%d.tga",Partyellow,i);
		MatParticleGold[i-1] = CreateTextureMaterial( szPath , 0, 0, 0,0, SMMAT_BLEND_LAMP );

	}
	
	Rage_Of_SinBaRam = smASE_Read("Image\\SinImage\\Effect\\test.ase");
	//�а� ������ �� 
	MatWideCircle = CreateTextureMaterial( "image\\Sinimage\\Effect\\Effect2D\\WideCircle.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );

	//MatEffectLine = CreateTextureMaterial( "image\\Sinimage\\Effect\\Effect2D\\Line.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );
	MatEffectLine = CreateTextureMaterial( "image\\Sinimage\\Effect\\Effect2D\\YellowLine.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );

	MatParticle1 = CreateTextureMaterial( "image\\Sinimage\\Effect\\Particle\\Particle1.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );

	InitInterEffect();
	InitEffect2();
	sinInitSkillEffect();
	sinInitPublicEffect();
#ifdef   _ASSA_EFFECT_START_	//AssaEffect ����
	InitAssaEffect(); //�ƽ�����Ʈ �ʱ�ȭ 
#endif
	InitYameEffect();
	

}
/*----------------------------------------------------------------------------*
*						    Bmp������ �ε��Ѵ� 
*-----------------------------------------------------------------------------*/	
void cSIN3D::Load()
{

}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cSIN3D::Release()
{

}
/*----------------------------------------------------------------------------*
*							     �׸���  
*-----------------------------------------------------------------------------*/	
void cSIN3D::Draw(int x, int y,int z,int ax, int ay, int az)
{ // x,y,z ī�޶� ��ǥ  ax , ay , az ī�޶� ���� 

	//smRender.SetCameraPosi
	//smRender.GetDepthZ

	POINT3D Posi = {x,y,z}; 
	POINT3D Angle = { ax , ay , az};
	renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
#ifdef   _ASSA_EFFECT_START_	//AssaEffect ����
	DrawAssaEffect(&Posi,&Angle); 
#endif
	sinDrawEffect(x,y,z,ax,ay,az); 
	sinDrawEffect2(x,y,z,ax,ay,az);
	DrawYameEffect(&Posi,&Angle); 
	cHelpPet.Draw(&Posi , &Angle); //���� �׸���

	renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	
	
	////////TEST ���ؽ� ��ο� 
	//TestVertexDraw(cSin3D.MatWideCircle,(int)chrPlayer.pX,(int)chrPlayer.pY+10000,(int)chrPlayer.pZ,1000,128);

	
}
/*----------------------------------------------------------------------------*
*							     ����  
*-----------------------------------------------------------------------------*/	
void cSIN3D::Main()
{
	sinActiveEffect(); 
	sinActiveInterEffect();
	InterEffectAging2Process(); //����¡ ����Ʈ�� �����Ѵ� 

	sinActiveEffect2(); //����Ʈ�� �����δ� 
	
	
#ifdef   _ASSA_EFFECT_START_	//AssaEffect ����
	MainAssaEffect(); //�ƽ� ����Ʈ ���� 
#endif
	MainYameEffect();

}
/*----------------------------------------------------------------------------*
*							     ���� 
*-----------------------------------------------------------------------------*/	
void cSIN3D::Close()
{
	CloseYameEffect();
}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/	
void cSIN3D::LButtonDown(int x , int y)
{


}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cSIN3D::LButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/	
void cSIN3D::RButtonDown(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cSIN3D::RButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	

void cSIN3D::KeyDown()
{
	POINT3D Posi;
	Posi.x = (int)lpCurPlayer->pX;
	Posi.y = (int)lpCurPlayer->pY+13000;
	Posi.z = (int)lpCurPlayer->pZ;
		
	/////////////////// TEST EFFECTKEY

	if(smConfig.DebugMode){      
		ParticleTestKey(); //��ƼŬ �׽�Ʈ Key
		sinSkillTestKey(); //��ų ����Ʈ �׽�Ʈ Key
		sinPublicEffectTestKey(); //���� ����Ʈ �׽�Ʈ Key

		#ifdef   _ASSA_EFFECT_START_	//AssaEffect ����
			AssaEffectKeyDown(); //�ƽ� ����Ʈ �׽�Ʈ Key
		#endif
			YameEffectKeyDown(); //Yame ����Ʈ Keydown
		

//		if(VRKeyBuff['8']){
			//sinSetParticleTail(Posi);
			//setBossMonsterEffect2(Posi);
			//sinStartEffect(SIN_EFFECT_RAGE_OF_SINBARAM,(int)chrPlayer.pX,(int)chrPlayer.pY+10000,(int)chrPlayer.pZ);

//		}

		if(sinGetKeyClick('A')){   
			sinReleaseEffect(); 
			TestCount2 = 0;
			//SetInterEffectAging();
			//sinStartEffect(SIN_EFFECT_RAGE_OF_SINBARAM,(int)chrPlayer.pX,(int)chrPlayer.pY+10000,(int)chrPlayer.pZ);
			

		}
		//if(sinGetKeyClick('9')){   
		//	CheeseTestEffectImage();
			//Sword_Blast_Effect(lpCurPlayer,lpCurPlayer);
			//setBossMonsterEffect2(Posi); 
			//setBossMonsterEffect(lpCurPlayer,0);
			//sinSetEffect2(SIN_EFFECT_BOSSAURA , lpCurPlayer , SIN_EFFECT_NO_TIME);
			
			//sinSetParticleHit(Posi);

			//setBossMonsterEffect(&chrPlayer,0);  
			//setBossMonsterEffect2(&chrPlayer,0);  

			//sinSetEffect_IceVigo(&chrPlayer,SIN_EFFECT_NO_TIME);
			//sinSetEffect2(SIN_EFFECT_BOSSAURA , &chrPlayer , SIN_EFFECT_NO_TIME);
		//	setBossMonsterEffect(&chrPlayer,2);
/*		
			switch(TestCount2){
				case 0:
					sinSetEffect2(SIN_EFFECT_STUN,&chrPlayer,3*70);
				break;
				case 1:
					sinSetEffect_IceVigo(&chrPlayer,100*70);

				break;
				case 2:
					sinSetEffect2(SIN_EFFECT_BOSSAURA , &chrPlayer , SIN_EFFECT_NO_TIME);
					
				break;
			}
			TestCount2++;
*/			
		//}
	}
}

/*----------------------------------------------------------------------------*
*							  �̹��� �׸��� 
*-----------------------------------------------------------------------------*/	
int cSIN3D::sinDrawTexture(int x ,int y, int z , int AngleX, int AngleY ,int AngleZ , smFACE2D *lpFace,int sinAngle)
{
	//����Ʈ2���� ����� ��ǥ�� �޴´� 

	// Turn on Z-buffering
	//renderDevice.SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE ); (������ ����°� ���Ƽ� �����)
	renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	renderDevice.SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
	//renderDevice.SetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_USEW );//TRUE );


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

	smMaterialGroup->smMaterial[lpFace->MatNum].TwoSide = FALSE; //ī�޶� ȸ���� ������� ���̱� 
		
	smRender.SetMaterialGroup(smMaterialGroup);  //�о�帰 ��Ʈ������ ���� 
		

	//���� ���� Frame�� �����Ѵ�..
	smMaterialGroup->SetMaterialAnimFrame( &smMaterial[lpFace->MatNum], 2);
		
	//Camera���� �Է�.
	smRender.SetCameraPosi(x, y, z, AngleX, AngleY, AngleZ);
		
	if(sinAngle){
		if(smRender.AddFace2D(lpFace,sinAngle)){
			smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
			smRender.ClipRendFace();
			smRender.GeomVertex2D(FALSE);
			smRender.RenderD3D();

		}

	}
	else{
		if(smRender.AddFace2D(lpFace)){
			smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
			smRender.ClipRendFace();
			smRender.GeomVertex2D(FALSE);
			smRender.RenderD3D();

		}
	}

	return TRUE;
}

