/*----------------------------------------------------------------------------*
*	���ϸ� :  AssaParticle.cpp	
*	�ϴ��� :  �ƽ�! ��ƼŬ 
*	�ۼ��� :  ����������Ʈ 2002�� 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"

/*----------------------------------------------------------------------------*
*							 CODE
*-----------------------------------------------------------------------------*/	
#define  PARTICLE7_HIT			0x00700001

/*----------------------------------------------------------------------------*
*							 KeyDown
*-----------------------------------------------------------------------------*/	
int sinAssaParticle7KeyDown() //��ƼŬ �׽�Ʈ 
{
	//lpCharSelPlayer	//�ѹ� ������ ����� ������ 
	//lpCharMsTrace		//��� ������ ����� ������

	POINT3D Posi = {lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ};   

	if(sinGetKeyClick('G')){
		//AssaParticle7_Hit(&Posi);
		//AssaTest_Shot(&Posi);
	}

	return TRUE;
}

/*----------------------------------------------------------------------------*
*							  Process 
*-----------------------------------------------------------------------------*/	
void sinAssaParticle7Process(cASSAEFFECT *pEffect)
{

	//������ �ڵ� 
	switch(pEffect->CODE){       
		case PARTICLE7_HIT:
			switch(pEffect->ActionIndex[0]){
				case 1:
					if(pEffect->Time < pEffect->Max_Time-30)
						pEffect->Size.w += 5000;
					else pEffect->Size.w -= 5000;

				break;
			}
		break;
	}

}
/*----------------------------------------------------------------------------*
*							 Hit
*-----------------------------------------------------------------------------*/	
int AssaParticle7_Hit(POINT3D *pPosi)
{
	//��  
	for(int i = 0 ; i < 1 ; i++){                                                      
		Assa = SetAssaEffect(90,"flare.tga",0,pPosi,5000);      
		cAssaEffect[Assa]->Size.w = GetRandomPos(7000,10000); 
		cAssaEffect[Assa]->Size.h = GetRandomPos(500,1200); 
		//cAssaEffect[Assa]->FACEY  = GetRandomPos(0,4096);
		cAssaEffect[Assa]->CODE   = PARTICLE7_HIT;
		cAssaEffect[Assa]->NotFadeIn = 1;
		//cAssaEffect[Assa]->ZposiFlag = 1;
		cAssaEffect[Assa]->ActionIndex[0] = 1;
		//cAssaEffect[Assa]->Face.r = 255;
		//cAssaEffect[Assa]->Face.g = 150;
		//cAssaEffect[Assa]->Face.b = 0;
		//cAssaEffect[Assa]->ActionIndex[3] = 4; 

	}	
	/*
	//�� 2
	for(int i = 0 ; i < 4 ; i++){                  
		Assa = SetAssaEffect(90,"flare.tga",0,pPosi,0);      
		cAssaEffect[Assa]->Size.w = GetRandomPos(10000,15000);
		cAssaEffect[Assa]->Size.h = GetRandomPos(500,1200); 
		cAssaEffect[Assa]->FACEY  = GetRandomPos(0,4096);
		cAssaEffect[Assa]->CODE   = ASSA_PARTICLE_SPECIAL_HIT;
		cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->ActionIndex[0] = 1;
		cAssaEffect[Assa]->Face.r = 255;
		cAssaEffect[Assa]->Face.g = 200;
		cAssaEffect[Assa]->Face.b = 50;
		cAssaEffect[Assa]->Time = -5*i+Time;
		cAssaEffect[Assa]->ZposiFlag = 1;

		
	}	

	//�� 
	for(int i = 0 ; i < 2 ; i++){                
		Assa = SetAssaEffect(70,"water2.tga",0,pPosi,0);      
		cAssaEffect[Assa]->Size.w = GetRandomPos(3000,3001);
		cAssaEffect[Assa]->FACEY  = GetRandomPos(0,4096);
		cAssaEffect[Assa]->ActionIndex[1] = 1;
		cAssaEffect[Assa]->CODE   = ASSA_PARTICLE_SPECIAL_HIT;
		cAssaEffect[Assa]->Face.r = 255;
		cAssaEffect[Assa]->Face.g = 150;
		cAssaEffect[Assa]->Face.b = 0;
		cAssaEffect[Assa]->Time = (i*-10)+Time;
		cAssaEffect[Assa]->ZposiFlag = 1;

	}	
	
	//�� 2
	for(int i = 0 ; i < 1 ; i++){                      
		Assa = SetAssaEffect(60,"Light01.tga",0,pPosi,0);       
		cAssaEffect[Assa]->Size.w = GetRandomPos(10000,10001);
		cAssaEffect[Assa]->FACEY  = GetRandomPos(0,4096);
		cAssaEffect[Assa]->ActionIndex[3] = 1;
		cAssaEffect[Assa]->CODE   = ASSA_PARTICLE_SPECIAL_HIT;
		cAssaEffect[Assa]->Face.r = 255;
		cAssaEffect[Assa]->Face.g = 200;
		cAssaEffect[Assa]->Face.b = 50;
		cAssaEffect[Assa]->ZposiFlag = 1;
		cAssaEffect[Assa]->Time = Time;

	}	

	//��ƼŬ  
	for(int i = 0 ; i < 15 ; i++){        
		Assa = SetAssaEffect(100,"flare.tga",0,pPosi,0);       
		cAssaEffect[Assa]->Size.w = GetRandomPos(500,1300);
		cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->Face.r = 255;
		cAssaEffect[Assa]->Face.g = 150;
		cAssaEffect[Assa]->Face.b = 0;
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE_ANGLE_SPEED;
		cAssaEffect[Assa]->MoveSpeed.z = GetRandomPos(128,256);
		cAssaEffect[Assa]->ActionIndex[2] =1;
		cAssaEffect[Assa]->Time = GetRandomPos(-20,0)+Time;
		cAssaEffect[Assa]->Gravity = 50;
		cAssaEffect[Assa]->CODE   = ASSA_PARTICLE_SPECIAL_HIT;
		cAssaEffect[Assa]->ZposiFlag = 1;
	}	

	for(int i = 0 ; i < 15 ; i++){         
		Assa = SetAssaEffect(100,"flare.tga",0,pPosi,0);       
		cAssaEffect[Assa]->Size.w = GetRandomPos(500,1300);
		cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->Face.r = 255;
		cAssaEffect[Assa]->Face.g = 200;
		cAssaEffect[Assa]->Face.b = 50;
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE_ANGLE_SPEED;
		cAssaEffect[Assa]->MoveSpeed.z = GetRandomPos(128,256);
		cAssaEffect[Assa]->ActionIndex[2] =1;
		cAssaEffect[Assa]->Time = GetRandomPos(-20,0)+Time;
		cAssaEffect[Assa]->Gravity = 50;
		cAssaEffect[Assa]->CODE   = ASSA_PARTICLE_SPECIAL_HIT;
		cAssaEffect[Assa]->ZposiFlag = 1;
	}	

*/
	return TRUE;
}

