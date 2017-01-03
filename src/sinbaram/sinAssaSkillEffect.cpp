/*----------------------------------------------------------------------------*
*	���ϸ� :  sinAssaSkillEffect.cpp	
*	�ϴ��� :  Assa ��ų ����Ʈ 
*	�ۼ��� :  ����������Ʈ 2002�� 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#include "sinLinkHeader.h"
#include "..\\HoBaram\\HoNewParticleLinkHeader.h" //���ο� ȣ�ٶ�

/*----------------------------------------------------------------------------*
*							���� ���� 
*-----------------------------------------------------------------------------*/	
/*
#define ASSA_ROTATE				0x0000000F //ȸ��
#define ASSA_MOVE				0x000000F0 //�̵�
#define ASSA_ANGLE_ROTATE		0x00000F00 //����ȸ��
#define ASSA_CHAR_POSI			0x0000F000 //ĳ���� ��ǥ�̵��� ���� ������ 
#define ASSA_MOVE_ANGLE_SPEED	0x000F0000 //������ �ӵ��� �°� �̵� 
#define ASSA_ANG_CHAR			0x00000001 //�ڽ��� ��
#define ASSA_ANG_DESCHAR		0x00000002 //���� �ڽ��� �� (OBJ)
#define ASSA_ANG_BONE			0x00000003 //���� �ڽ��� ���� �� 
*/

/*----------------------------------------------------------------------------*
*							Key Down
*-----------------------------------------------------------------------------*/	
int TestNum = 0;
int sinAssaSkillEffectKeyDown() //��ų����Ʈ �׽�Ʈ 
{
	//lpCharSelPlayer	//�ѹ� ������ ����� ������ 
	//lpCharMsTrace		//��� ������ ����� ������
	
	POINT3D Posi = {lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ};   
 
 	if(sinGetKeyClick('0')){        
		//AssaParticle_Char_Light2(lpCurPlayer , ASSA_COLOR_CODE_YELLOW , 400);
		//AssaParticle_Char_Light(lpCurPlayer , ASSA_COLOR_CODE_YELLOW ,30000, 300);
		sinEffect_StartMagic(&Posi,2,1);
		//sinSkillEffect_Windy(lpCurPlayer);
		//sinSkillEffect_DrasticSpirit(lpCurPlayer);
		//sinSkillEffect_Brandish(lpCurPlayer);
		//sinSkillEffect_Piercing(lpCurPlayer);  
		//sinSkillEffect_Energy_Shelter(lpCurPlayer);  
		//sinSkillEffect_Holy_Reflection(lpCurPlayer);
		//sinSkillEffect_Holy_Reflection_Defense(lpCurPlayer);
		//sinSkillEffect_Soul_Sucker(lpCurPlayer,lpCharSelPlayer); 
		//sinSkillEffect_Great_Healing(lpCurPlayer);
		//sinSkillEffect_Great_Healing_Party(lpCurPlayer);
		//TestNum++;
		//sinSkillEffect_HolyMind(lpCurPlayer,10); �մ�� ... �����~~~

	}
	/*
	if(sinGetKeyClick('9')){  
		//TestNum--;

	}
	if(sinGetKeyClick('8')){  
		switch(TestNum){
			case 0:
				sinSkillEffect_Energy_Shelter(lpCurPlayer);  

			break;
			case 1:
				sinSkillEffect_Piercing(lpCurPlayer); 

			break;
			case 2:
				sinSkillEffect_DrasticSpirit(lpCurPlayer);

			break;
			case 3:
				sinSkillEffect_Brandish(lpCurPlayer);

			break;
			case 4:
				sinSkillEffect_Great_Healing(lpCurPlayer);

			break;
			case 5:
				sinSkillEffect_Soul_Sucker(lpCurPlayer,lpCharSelPlayer); 

			break;
			case 6:
				sinSkillEffect_Holy_Reflection(lpCurPlayer);
			break;

		}
	}
	*/
		//sinSkillEffect_Energy_Shelter(lpCurPlayer);  
		//sinSkillEffect_Piercing(lpCurPlayer);  
		//sinSkillEffect_DrasticSpirit(lpCurPlayer); 
		//sinSkillEffect_Brandish(lpCurPlayer); 
		//sinSkillEffect_Great_Healing(lpCurPlayer); 
		//sinSkillEffect_Soul_Sucker(lpCurPlayer,lpCharSelPlayer); 
		//sinSkillEffect_Holy_Reflection(lpCurPlayer); 
		//sinSkillEffect_Twist_Javelin(lpCurPlayer ,lpCharSelPlayer ); 

	return TRUE;
}

/*----------------------------------------------------------------------------*
*							Process
*-----------------------------------------------------------------------------*/	
int sinAssaSkillEffectProcess(cASSAEFFECT *pEffect)
{

	switch(pEffect->CODE){      
		case SKILL_PIERCING:    
			if(pEffect->Time == 1){ //�ѹ��� üũ�Ѵ�       
				pEffect->Obj = pEffect->pChar->AnimPattern->GetObjectFromName("Bip weapon01"); //������Ʈ�� �д´� 
			}
			AssaSearchObjPos(pEffect->pChar,pEffect->Obj, &pEffect->CenterPosi);
			//�⸦ ������ 
			if(pEffect->ActionIndex[3] == 1){
				if(pEffect->GatherSpeed>1){
					pEffect->GatherSpeed -= pEffect->Time/4;
					if(pEffect->GatherSpeed <1 )pEffect->GatherSpeed = 1;

				}

				pEffect->TempPosi.x = pEffect->CenterPosi.x - pEffect->Posi.x;
				pEffect->TempPosi.y = pEffect->CenterPosi.y - pEffect->Posi.y;
				pEffect->TempPosi.z = pEffect->CenterPosi.z - pEffect->Posi.z;

				pEffect->Posi.x += pEffect->TempPosi.x/pEffect->GatherSpeed;
				pEffect->Posi.y += pEffect->TempPosi.y/pEffect->GatherSpeed;
				pEffect->Posi.z += pEffect->TempPosi.z/pEffect->GatherSpeed;
			}

			if(pEffect->Bone){   
				if((pEffect->Time ==1)){//������ �ѹ��� �����Ѵ� 
					//AssaParticle_LineIn(&pEffect->CenterPosi,"line.bmp",3,100);
					//AssaParticle_Piercing_Aura(pEffect);
				}
				if((pEffect->Time %3) == 0 ){
					AssaParticle_RisingParticleContinue(&pEffect->CenterPosi,"star05B_02.bmp",500,128,GetRandomPos(20,80));
					AssaParticle_After_Image(&pEffect->CenterPosi,"star01B_04.bmp",16,0);

				}
				if(pEffect->Time == 40){
				//�޽� �߻����� �Լ��λ���  (�׽�Ʈ�Ҷ��� ���� )
					//sinSkillEffect_Piercing_Attack(pEffect->pChar);
					/*
					Assa = SetAssaEffect(40,"Piercing.ASE",pEffect->pChar,0,0); 
					cAssaEffect[Assa]->NotFadeIn = 1;
					AssaParticle_Piercing(cAssaEffect[Assa]);
					*/
		
				}
				
			}
		break;
		case SKILL_WINDY:
			/*
			if(pEffect->Time == 1){ //�ѹ��� üũ�Ѵ�      
				pEffect->Obj = pEffect->pChar->AnimPattern->GetObjectFromName("Bip weapon01"); //������Ʈ�� �д´� 
			}
			*/
			AssaSearchWeaponPos(pEffect->pChar,&pEffect->CenterPosi,0);                       
			//AssaSearchObjPos(pEffect->pChar,pEffect->Obj, &pEffect->CenterPosi);
			//�⸦ ������ 
			if(pEffect->ActionIndex[3] == 1){      
				if(pEffect->GatherSpeed>1){
					pEffect->GatherSpeed -= pEffect->Time/4;
					if(pEffect->GatherSpeed <1 )pEffect->GatherSpeed = 1;
				}
				pEffect->TempPosi.x = pEffect->CenterPosi.x - pEffect->Posi.x;
				pEffect->TempPosi.y = pEffect->CenterPosi.y - pEffect->Posi.y;
				pEffect->TempPosi.z = pEffect->CenterPosi.z - pEffect->Posi.z;

				pEffect->Posi.x += pEffect->TempPosi.x/pEffect->GatherSpeed;
				pEffect->Posi.y += pEffect->TempPosi.y/pEffect->GatherSpeed;
				pEffect->Posi.z += pEffect->TempPosi.z/pEffect->GatherSpeed;
			}
			if(pEffect->Bone){    
				if(pEffect->Time == 10) //�����ġ�� ���߱����ؼ� �̷��� ���ش� 
					sinSkillEffect_Windy_Gather(pEffect->pChar);
				if(pEffect->Time == 37 ) 
					AssaParticle_Windy2(pEffect->pChar);
			}


		break;

		/*
		case SKILL_ENERGY_SHELTER:       
			if(pEffect->Time == 1){ //�ѹ��� üũ�Ѵ�     
				pEffect->Obj = pEffect->pChar->AnimPattern->GetObjectFromName("Bip weapon01"); //������Ʈ�� �д´� 
			}
			AssaSearchObjPos(pEffect->pChar,pEffect->Obj, &pEffect->CenterPosi);

			//�⸦ ������ 
			if(pEffect->ActionIndex[3] == 1){
				if(pEffect->GatherSpeed>1){
					pEffect->GatherSpeed -= pEffect->Time/4;
					if(pEffect->GatherSpeed <1 )pEffect->GatherSpeed = 1;

				}

				pEffect->TempPosi.x = pEffect->CenterPosi.x - pEffect->Posi.x;
				pEffect->TempPosi.y = pEffect->CenterPosi.y - pEffect->Posi.y;
				pEffect->TempPosi.z = pEffect->CenterPosi.z - pEffect->Posi.z;

				pEffect->Posi.x += pEffect->TempPosi.x/pEffect->GatherSpeed;
				pEffect->Posi.y += pEffect->TempPosi.y/pEffect->GatherSpeed;
				pEffect->Posi.z += pEffect->TempPosi.z/pEffect->GatherSpeed;
			}

			if(pEffect->Bone){
				if((pEffect->Time %3) == 0 ){
					AssaParticle_RisingParticleContinue(&pEffect->CenterPosi,"star05G_02.bmp",500,128,GetRandomPos(20,80));
					AssaParticle_After_Image(&pEffect->CenterPosi,"star01G_04.bmp",16,0);

				}
				
			}
		
		break;
		*/
		case SKILL_HOLY_REFLECTION:
			if(pEffect->Bone){ //���� ���� ��ġ����ƼŬ�� �ѷ��ش� 
				AssaParticle_Holy_Reflection(pEffect); 

			}
			if(pEffect->ActionIndex[0] == 1){  
				pEffect->RDistance.z += pEffect->Time/4;
				pEffect->Gravity -=2;
				pEffect->Posi.y += pEffect->Gravity;
				pEffect->Angle.y += 10; pEffect->Angle.y &= ANGCLIP;
				pEffect->Angle.x += 10; pEffect->Angle.x &= ANGCLIP;
				pEffect->Angle.z += 10; pEffect->Angle.z &= ANGCLIP;

			}
			else if(pEffect->ActionIndex[1] == 1){  
				if(pEffect->Time < 10)
					pEffect->Size.w += 400;
				else pEffect->Size.w -= 400;
			}	
			else if(pEffect->ActionIndex[1] == 2){  
				
			}
			else{
				pEffect->Gravity -=2;
				pEffect->Posi.y += pEffect->Gravity;
			}

		break;
		case SKILL_GRAND_HEALING:
			pEffect->Angle.y = -((pEffect->RAngle)+ANGLE_270)&ANGCLIP; 

		break;
		case SKILL_SOUL_SUCKER:        
			if(pEffect->GatherSpeed>1){
				pEffect->GatherSpeed -= pEffect->Time/4;
				if(pEffect->GatherSpeed <1 )pEffect->GatherSpeed = 1;
			}
			pEffect->TempPosi.x = pEffect->CenterPosi.x - pEffect->Posi.x;
			pEffect->TempPosi.y = pEffect->CenterPosi.y - pEffect->Posi.y;
			pEffect->TempPosi.z = pEffect->CenterPosi.z - pEffect->Posi.z;

			pEffect->Posi.x += pEffect->TempPosi.x/pEffect->GatherSpeed;
			pEffect->Posi.y += pEffect->TempPosi.y/pEffect->GatherSpeed;
			pEffect->Posi.z += pEffect->TempPosi.z/pEffect->GatherSpeed;

		break;
		case SKILL_HOLY_MIND:
			//if(pEffect->Time+30 <= pEffect->Max_Time)  
			if((pEffect->Time % 3) ==0)
				AssaParticle_HolyMind(&pEffect->Posi);

		break;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*
*						(Knight) Brandish
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_Brandish(smCHAR *pChar)
{
	
	for(int i = 0; i <2 ; i++){    
		Assa = SetAssaEffect(0,"Brabdish.ASE",pChar,0,0);                              
		cAssaEffect[Assa]->AniMaxCount = 20; 
		cAssaEffect[Assa]->AniDelayTime = 2;
		cAssaEffect[Assa]->StartPosi.x = 256*8*6;
		cAssaEffect[Assa]->Angle.y =  ((pChar->Angle.y&ANGCLIP)+ANGLE_270)&ANGCLIP;
		AssaSetStartPosi(cAssaEffect[Assa]);
		cAssaEffect[Assa]->Angle.y =  -((pChar->Angle.y&ANGCLIP))&ANGCLIP;
		AssaParticle_SpecialHit2(&cAssaEffect[Assa]->Posi,0);
	}
	SetDynLight( cAssaEffect[Assa]->Posi.x , cAssaEffect[Assa]->Posi.y , cAssaEffect[Assa]->Posi.z ,255, 120, 0, 255, 200 ,1);
	return TRUE;
}

/*----------------------------------------------------------------------------*
*						(Knight) Piercing
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_Piercing_Attack(smCHAR *pChar) //�Ǿ�� ���� 
{
	Assa = SetAssaEffect(30,"Piercing.ASE",pChar,0,0);    
	cAssaEffect[Assa]->NotFadeIn = 1;
	AssaParticle_Piercing(cAssaEffect[Assa]); //�ΰ�����ش�

	return TRUE;
}
int sinSkillEffect_Piercing(smCHAR *pChar)
{
	//Bone
	POINT3D TempPosi = {pChar->pX,pChar->pY,pChar->pZ}; 	        	  
	int TempIndex = 0 ,Temp4 = 0;
	
	//����Ʈ 
	AssaParticle_Char_Light2(pChar,ASSA_COLOR_CODE_BLUE,70);
	/*
	for(int i = 0 ;i < 1 ; i++){  
		AssaParticle_Char_Light(pChar,ASSA_COLOR_CODE_BLUE , 100 , 20000,30000);
	}
	*/

	SetDynLight( pChar->pX , pChar->pY , pChar->pZ ,0, 150, 255, 255, 130 ,1);
	//Į�ֺ��� ����Ʈ  
	Assa = SetAssaEffect(60,"Bone",pChar,0,7000);                          
	cAssaEffect[Assa]->CODE = SKILL_PIERCING;    //���� ���󰡴� ��ƼŬ�� �̰����� ����� 
	/*
	//�տ� �Ⱑ���δ� ��������
	for(int i = 0 ; i < 3 ; i++){                                                      
		Assa = SetAssaEffect(80,"fire11_08.bmp",pChar,0,pChar->PatHeight+10000);                    
		cAssaEffect[Assa]->Size.w = 1500;  
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_CHAR_POSI;
		//cAssaEffect[Assa]->MoveSpeed.y = 500; 
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE; 
		cAssaEffect[Assa]->cAssaTrace->SetLength(10); //������̸� ��´�
		cAssaEffect[Assa]->CenterPosi.x = pChar->pX;
		cAssaEffect[Assa]->CenterPosi.y = pChar->pY+cAssaEffect[Assa]->AddHeight+10000;
		cAssaEffect[Assa]->CenterPosi.z = pChar->pZ; 
		cAssaEffect[Assa]->GatherSpeed = 80;
		cAssaEffect[Assa]->CODE = SKILL_PIERCING;
		cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->ActionIndex[3] = 1;
		AssaGetCirclePosi(cAssaEffect[Assa],256*64,(i+1)*(4096/3));
		TempIndex = Assa;

		Assa = SetAssaEffect(80,"star05B_03.bmp",pChar,0,cAssaEffect[Assa]->AddHeight);                   
		cAssaEffect[Assa]->Size.w = 4000;  
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_CHAR_POSI;
		cAssaEffect[Assa]->MoveSpeed.x = cAssaEffect[TempIndex]->MoveSpeed.x;
		cAssaEffect[Assa]->MoveSpeed.y = cAssaEffect[TempIndex]->MoveSpeed.y;
		cAssaEffect[Assa]->MoveSpeed.z = cAssaEffect[TempIndex]->MoveSpeed.z;
		cAssaEffect[Assa]->CenterPosi.x = cAssaEffect[TempIndex]->CenterPosi.x;
		cAssaEffect[Assa]->CenterPosi.y = cAssaEffect[TempIndex]->CenterPosi.y;
		cAssaEffect[Assa]->CenterPosi.z = cAssaEffect[TempIndex]->CenterPosi.z;
		cAssaEffect[Assa]->GatherSpeed = cAssaEffect[TempIndex]->GatherSpeed;
		AssaGetCirclePosi(cAssaEffect[Assa],cAssaEffect[TempIndex]->RDistance.z,cAssaEffect[TempIndex]->RAngle);
		cAssaEffect[Assa]->ActionIndex[3] = 1;
		cAssaEffect[Assa]->CODE = SKILL_PIERCING;//cAssaEffect[TempIndex]->CODE;
		
		for(int j = 0 ; j < 10 ; j++)
			AssaParticle_RisingParticleContinue(&cAssaEffect[Assa]->Posi,"star05B_02.bmp",500,128,GetRandomPos(20,80));

	}
	*/

	//�տ� �ִ� �ع�ġ 
	for(int i = 0 ; i < 6 ; i++){                                                                                                           
		Assa = SetAssaEffect(200,"star05B_03.bmp",pChar,0,pChar->PatHeight-2500);                     
		cAssaEffect[Assa]->Size.w = 4000;  
		//cAssaEffect[Assa]->ZposiFlag =1;
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE_RANDOM_ROTATE;
		//cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE; 
		//cAssaEffect[Assa]->cAssaTrace->SetLength(30); //������̸� ��´�
		cAssaEffect[Assa]->RAngleSpeed = 100;
		cAssaEffect[Assa]->MoveSpeed.z = 256*2;
		cAssaEffect[Assa]->Angle.x = GetRandomPos(0,4096);
		cAssaEffect[Assa]->Angle.y = GetRandomPos(0,4096);
		cAssaEffect[Assa]->CODE = SKILL_PIERCING;
		//cAssaEffect[Assa]->Time = -30;

		//cAssaEffect[Assa]->NotFadeIn = 1;
		
	}

	/* ���̻��� ��ȿ!
	//���� ���θ� �ö� �θ���  
	for(int i = 0 ; i < 20 ; i++){                                                                                                                    
		Assa = SetAssaEffect(100,"star06B_04.bmp",pChar,0,-1000);                               
		cAssaEffect[Assa]->Size.w = 200;  
		cAssaEffect[Assa]->MoveSpeed.y = 100;
		AssaGetCirclePosi(cAssaEffect[Assa],256*7,i*256);
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE |ASSA_CHAR_POSI;
		if(Temp4 > 0 )Temp4 = -1;
		else Temp4 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 25*Temp4;
		cAssaEffect[Assa]->Time = GetRandomPos(i*-5,i*-10);
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_SCALE;
		cAssaEffect[Assa]->ScalePosi.y = 10000;
		//cAssaEffect[Assa]->NotFadeIn = 1;

	}
	*/

	//������ ��ƼŬ
	/*
	AssaParticle_RisingParticle(&TempPosi,"star04B_01.bmp",1000,1,1500,3,300);     
	//AssaParticle_RisingBelt(&TempPosi,"belt-02.ASE",3,100);
	AssaParticle_RisingLine(&TempPosi,"star04B_01.bmp",300,1,1000,3,300);  
	AssaParticle_RisingRotateLine(&TempPosi,"star04B_01.bmp",1,500,1,200);  
	

	//Mesh 
	Assa = SetAssaEffect(0,"NT23-Piecing.ASE",pChar,0,3000);            
	cAssaEffect[Assa]->AniMaxCount = 20;
	cAssaEffect[Assa]->AniDelayTime = 2;
	cAssaEffect[Assa]->Time = -70;
	TempIndex = Assa;
	
	//�ܻ���ƼŬ  
	Assa = SetAssaEffect(30,"star03B_07.bmp",pChar,0,10000);                       
	cAssaEffect[Assa]->MOVECODE = ASSA_MOVE_ANGLE_SPEED;
	cAssaEffect[Assa]->Size.w = 3000;

	cAssaEffect[Assa]->MoveSpeed.z = 256*6;
	//cAssaEffect[Assa]->Angle.y = cAssaEffect[TempIndex]->Angle.y;
	cAssaEffect[Assa]->Angle.y = -((cAssaEffect[TempIndex]->Angle.y)+ANGLE_180)&ANGCLIP;
	cAssaEffect[Assa]->Time = cAssaEffect[TempIndex]->Time;
	cAssaEffect[Assa]->CODE = SKILL_PIERCING; 
	cAssaEffect[Assa]->StartPosi.z = 256*32*4;
	AssaSetStartPosi(cAssaEffect[Assa]);
	*/

	return TRUE;
}

/*----------------------------------------------------------------------------*
*						(Knight) Drastic Spirit
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_DrasticSpirit(smCHAR *pChar)
{
	
	POINT3D TempPosi = {pChar->pX,pChar->pY-2000,pChar->pZ};	              	  
	SetDynLight( pChar->pX , pChar->pY , pChar->pZ ,50, 50, 255, 255, 200 ,1);          


	//�޽�
	Assa = SetAssaEffect(0,"DrasticSpirit.ASE",pChar,0,0);                 
	cAssaEffect[Assa]->AniMaxCount = 30;   
	cAssaEffect[Assa]->AniDelayTime = 2;


	
	//������ 
	//AssaParticle_RisingRotateLinePower(&TempPosi , "star04B_04.bmp",256+126,500);           
	AssaParticle_RotateWide(&TempPosi , "star06B_04.bmp",256,400);             
	AssaParticle_RisingParticle(&TempPosi,"star04B_01.bmp",1000,1,1000,3,200);      

	//�޽�
	Assa = SetAssaEffect(0,"DrasticSpirit.ASE",pChar,0,0);                
	cAssaEffect[Assa]->AniMaxCount = 30;   
	cAssaEffect[Assa]->AniDelayTime = 2;


	//��ƼŬ
	Assa = SetAssaEffect(100,"helmet01.bmp",0,&TempPosi,15000);                   
	cAssaEffect[Assa]->Size.w = 6000;
	cAssaEffect[Assa]->ReSize.w = 100;
	cAssaEffect[Assa]->ReSize.h = cAssaEffect[Assa]->ReSize.w;
	cAssaEffect[Assa]->MOVECODE = ASSA_INCRESIZE;
	cAssaEffect[Assa]->Time = -70;
	cAssaEffect[Assa]->MOVECODE = ASSA_MOVE|ASSA_INCRESIZE;


	TempPosi.y += 15000;
	//AssaParticle_Wide(&TempPosi , "star06B_04.bmp" ,30 , -70);  ��� ���´�
	//cAssaEffect[Assa]->MoveSpeed.y = 50;

	
	//sinEffect_Light5(&pEffect->Posi,15000); 
	//cAssaEffect[Assa]->AniDelayTime = 1;
	//cAssaEffect[Assa]->Time = -70;
	


	return TRUE;
}

/*----------------------------------------------------------------------------*
*						(Wizard) Energy Shelter
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_Energy_Shelter(smCHAR *pChar)
{
/*
	int Temp4 = 1 ,  TempIndex = 0;
	
	//���� ���� 
	Assa = SetAssaEffect(200,"Bone",pChar,0,7000);                        
	cAssaEffect[Assa]->CODE = SKILL_ENERGY_SHELTER;    //���� ���󰡴� ��ƼŬ�� �̰����� ����� 

	//�տ� �Ⱑ���δ� ��������
	for(int i = 0 ; i < 3 ; i++){                                                      
		Assa = SetAssaEffect(80,"fire11_02.bmp",pChar,0,pChar->PatHeight-2500);                    
		cAssaEffect[Assa]->Size.w = 1500;  
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_CHAR_POSI;
		cAssaEffect[Assa]->MoveSpeed.y = 500; 
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE; 
		cAssaEffect[Assa]->cAssaTrace->SetLength(10); //������̸� ��´�
		cAssaEffect[Assa]->CenterPosi.x = pChar->pX;
		cAssaEffect[Assa]->CenterPosi.y = pChar->pY+cAssaEffect[Assa]->AddHeight-2500;
		cAssaEffect[Assa]->CenterPosi.z = pChar->pZ; 
		cAssaEffect[Assa]->GatherSpeed = 80;
		cAssaEffect[Assa]->CODE = SKILL_ENERGY_SHELTER;
		cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->ActionIndex[3] = 1;
		AssaGetCirclePosi(cAssaEffect[Assa],256*64,(i+1)*(4096/3));
		TempIndex = Assa;

		Assa = SetAssaEffect(80,"star05G_03.bmp",pChar,0,cAssaEffect[Assa]->AddHeight);                   
		cAssaEffect[Assa]->Size.w = 4000;  
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_CHAR_POSI;
		cAssaEffect[Assa]->MoveSpeed.x = cAssaEffect[TempIndex]->MoveSpeed.x;
		cAssaEffect[Assa]->MoveSpeed.y = cAssaEffect[TempIndex]->MoveSpeed.y;
		cAssaEffect[Assa]->MoveSpeed.z = cAssaEffect[TempIndex]->MoveSpeed.z;
		cAssaEffect[Assa]->CenterPosi.x = cAssaEffect[TempIndex]->CenterPosi.x;
		cAssaEffect[Assa]->CenterPosi.y = cAssaEffect[TempIndex]->CenterPosi.y;
		cAssaEffect[Assa]->CenterPosi.z = cAssaEffect[TempIndex]->CenterPosi.z;
		cAssaEffect[Assa]->GatherSpeed = cAssaEffect[TempIndex]->GatherSpeed;
		AssaGetCirclePosi(cAssaEffect[Assa],cAssaEffect[TempIndex]->RDistance.z,cAssaEffect[TempIndex]->RAngle);
		cAssaEffect[Assa]->ActionIndex[3] = 1;
		cAssaEffect[Assa]->CODE = SKILL_ENERGY_SHELTER;//cAssaEffect[TempIndex]->CODE;
		
		for(int j = 0 ; j < 10 ; j++)
			AssaParticle_RisingParticleContinue(&cAssaEffect[Assa]->Posi,"star05G_02.bmp",500,128,GetRandomPos(20,80));

	}

	//�տ� �ִ� �ع�ġ 
	for(int i = 0 ; i < 6 ; i++){                                                                                                          
		Assa = SetAssaEffect(300,"star05G_03.bmp",pChar,0,pChar->PatHeight-2500);                     
		cAssaEffect[Assa]->Size.w = 4000;  
		//cAssaEffect[Assa]->ZposiFlag =1;
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE_RANDOM_ROTATE;
		//cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE; 
		//cAssaEffect[Assa]->cAssaTrace->SetLength(30); //������̸� ��´�
		cAssaEffect[Assa]->RAngleSpeed = 100;
		cAssaEffect[Assa]->MoveSpeed.z = 256;
		cAssaEffect[Assa]->Angle.x = GetRandomPos(0,4096);
		cAssaEffect[Assa]->Angle.y = GetRandomPos(0,4096);
		cAssaEffect[Assa]->CODE = SKILL_ENERGY_SHELTER;
		//cAssaEffect[Assa]->NotFadeIn = 1;
		
	}

	//���� ���θ� �ö� �θ���  
	for(int i = 0 ; i < 20 ; i++){                                                                                                                   
		Assa = SetAssaEffect(200,"star06G_04.bmp",pChar,0,-1000);                               
		cAssaEffect[Assa]->Size.w = 600;  
		cAssaEffect[Assa]->MoveSpeed.y = 100;
		AssaGetCirclePosi(cAssaEffect[Assa],256*7,i*256);
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE |ASSA_CHAR_POSI;
		if(Temp4 > 0 )Temp4 = -1;
		else Temp4 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 25*Temp4;
		cAssaEffect[Assa]->Time = GetRandomPos(i*-10,i*-20);
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_SCALE;
		cAssaEffect[Assa]->ScalePosi.y = 10000;
		//cAssaEffect[Assa]->NotFadeIn = 1;

	}

	//���� ��� ��� 
//	Assa = SetAssaEffect(500,"energyshelter.ASE",pChar,0,7000);                      
//	cAssaEffect[Assa]->MOVECODE = ASSA_CHAR_POSI;
//	cAssaEffect[Assa]->Time = -100;

*/
	return TRUE;

	
}
/*----------------------------------------------------------------------------*
*						(Saintess) Great Healing
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_Great_Healing(smCHAR *pChar)
{
	int Temp = 1 ,Temp2 =1 , Temp4 = 1;  
	SetDynLight( pChar->pX, pChar->pY , pChar->pZ ,255, 150, 100, 150, 180 ,1);  
	//���� 
	for(int i = 0 ; i < 2 ; i++){                       
		Assa = SetAssaEffect(0,"GH.ASE",pChar,0,0);                                       
		cAssaEffect[Assa]->AniMaxCount  = 30;   
		cAssaEffect[Assa]->AniDelayTime = 4;
		//cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->MOVECODE = ASSA_CHAR_POSI;
		cAssaEffect[Assa]->Time = i*-30;

	}
	
	for(int i = 0 ; i < 20 ; i++){                  
		Assa = SetAssaEffect(200,"star04Y_01.bmp",pChar,0,7000);    //�⺻������ Char�ϰ��� AngleY�� ����ֱ⶧���� �ʱ�ȭ���ش�
		//cAssaEffect[Assa]->Angle.y = GetRandomPos(0,2046); //Angle �� �ʱ�ȭ���ش� 
		cAssaEffect[Assa]->Angle.y = 0;
		cAssaEffect[Assa]->Size.w = 1000;  
		AssaGetAniFileMat(cAssaEffect[Assa]);
		cAssaEffect[Assa]->AniCount = GetRandomPos(0,cAssaEffect[Assa]->AniMaxCount);
		cAssaEffect[Assa]->AniDelayTime = GetRandomPos(30,45);
		cAssaEffect[Assa]->AniReverse = 1;
		cAssaEffect[Assa]->StartPosi.z = GetRandomPos(256*16,256*32);
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE;
		if(Temp > 0 )Temp = -1;
		else Temp = 1;
		cAssaEffect[Assa]->MoveSpeed.y = 10*Temp;
		AssaSetStartPosi(cAssaEffect[Assa]);
	}
		//ȸ�� 
		/*
		Assa = SetAssaEffect(120,"star06Y_04.bmp",pChar,0,0);                              
		cAssaEffect[Assa]->Size.w = 800;  
		cAssaEffect[Assa]->MoveSpeed.y = 300;
		AssaGetCirclePosi(cAssaEffect[Assa],256*7,GetRandomPos(0,4096));
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE |ASSA_CHAR_POSI;
		if(Temp4 > 0 )Temp4 = -1;
		else Temp4 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 50*Temp4;
		cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_SCALE;
		cAssaEffect[Assa]->ScalePosi.y = 15000;
		cAssaEffect[Assa]->NotFadeIn = 1;
		*/

	for(int i = 0 ; i < 10 ; i++){                 		
		//ȸ�� ����
		Assa = SetAssaEffect(120,"star06Y_04.bmp",pChar,0,0);             
		cAssaEffect[Assa]->Size.w = 1000; 
		cAssaEffect[Assa]->MoveSpeed.y = 300;
		AssaGetCirclePosi(cAssaEffect[Assa],256*8,GetRandomPos(0,4096));
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE |ASSA_CHAR_POSI;
		if(Temp4 > 0 )Temp4 = -1;
		else Temp4 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 50*Temp4;
		cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE;
		cAssaEffect[Assa]->cAssaTrace->SetLength(30); //������̸� ��´�
		cAssaEffect[Assa]->NotFadeIn = 1;
	}	

		/////// ��ƼŬ  
		/*
		Assa = SetAssaEffect(120,"star01Y_01.bmp",pChar,0,0);         
		cAssaEffect[Assa]->Size.w = 1000;  
		cAssaEffect[Assa]->MoveSpeed.y = 300;
		AssaGetCirclePosi(cAssaEffect[Assa],256*10,GetRandomPos(0,4096));
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE | ASSA_CHAR_POSI;
		if(Temp2 > 0 )Temp2 = -1;
		else Temp2 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 50*Temp2;
		cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
		cAssaEffect[Assa]->NotFadeIn = 1;
		AssaGetAniFileMat(cAssaEffect[Assa]);
		cAssaEffect[Assa]->AniDelayTime = GetRandomPos(10,15);
		cAssaEffect[Assa]->AniCount = GetRandomPos(0,cAssaEffect[Assa]->AniMaxCount);
		*/
	

	return TRUE;
}

int sinSkillEffect_Great_Healing_Party(smCHAR *pChar)
{
	POINT3D TempPosi = {pChar->pX,pChar->pY,pChar->pZ};	                            	  
	int Temp =1 , Temp2 =1 ,  Temp3 = 1 , Temp4 = 1;;

	SetDynLight( pChar->pX, pChar->pY , pChar->pZ ,255, 150, 100, 150, 100 ,1); 
	for(int i = 0 ; i < 4 ; i++){ 	    
		///////����  
		Assa = SetAssaEffect(120,"fire01_02.bmp",pChar,0,0);          
		cAssaEffect[Assa]->Size.w = 600; 
		cAssaEffect[Assa]->MoveSpeed.y = 300;
		AssaGetCirclePosi(cAssaEffect[Assa],256*6,GetRandomPos(0,4096));
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE |ASSA_CHAR_POSI;
		if(Temp > 0 )Temp = -1;
		else Temp = 1;
		cAssaEffect[Assa]->RAngleSpeed = 100*Temp;
		cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE;
		cAssaEffect[Assa]->cAssaTrace->SetLength(30); //������̸� ��´�
		cAssaEffect[Assa]->NotFadeIn = 1;

		/////// ��ƼŬ  
		Assa = SetAssaEffect(120,"star01Y_01.bmp",pChar,0,0);         
		cAssaEffect[Assa]->Size.w = 2000;  
		cAssaEffect[Assa]->MoveSpeed.y = 200;
		AssaGetCirclePosi(cAssaEffect[Assa],256*8,GetRandomPos(0,4096));
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE | ASSA_CHAR_POSI;
		if(Temp2 > 0 )Temp2 = -1;
		else Temp2 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 50*Temp2;
		cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
		cAssaEffect[Assa]->cAssaTrace->SetLength(30); //������̸� ��´�
		cAssaEffect[Assa]->NotFadeIn = 1;
		AssaGetAniFileMat(cAssaEffect[Assa]);
		cAssaEffect[Assa]->AniDelayTime = GetRandomPos(10,15);
		cAssaEffect[Assa]->AniCount = GetRandomPos(0,cAssaEffect[Assa]->AniMaxCount);

		//�޽� 
		Assa = SetAssaEffect(70,"GreatHealing.ASE",pChar,0,4000);         
		cAssaEffect[Assa]->AniMaxCount = 32; 
		cAssaEffect[Assa]->AniDelayTime = 4;
		AssaGetCirclePosi(cAssaEffect[Assa],256*8,GetRandomPos(0,4096));
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE|ASSA_CHAR_POSI;
		cAssaEffect[Assa]->MoveSpeed.y = 200;
		if(Temp3 > 0 )Temp3 = -1;
		else Temp3 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 50 *Temp3;
		cAssaEffect[Assa]->CODE = SKILL_GRAND_HEALING;
		cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);

		//ȸ�� 
		Assa = SetAssaEffect(120,"star06Y_04.bmp",pChar,0,0);                              
		cAssaEffect[Assa]->Size.w = 1000;  
		//cAssaEffect[Assa]->MoveSpeed.y = 300;
		AssaGetCirclePosi(cAssaEffect[Assa],256*7,GetRandomPos(0,4096));
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE |ASSA_CHAR_POSI;
		if(Temp4 > 0 )Temp4 = -1;
		else Temp4 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 50*Temp4;
		cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_SCALE;
		cAssaEffect[Assa]->ScalePosi.y = 15000;
		cAssaEffect[Assa]->NotFadeIn = 1;

		//���� �� 
		if(i < 2){        
			Assa = SetAssaEffect(140,"ground_ring.bmp",pChar,0,1200-((i+1)*300));                         
			cAssaEffect[Assa]->Size.w = 3000*(i+2);    
			cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_WORLD;
			cAssaEffect[Assa]->MOVECODE = ASSA_ANGLE_ROTATE|ASSA_CHAR_POSI;
			cAssaEffect[Assa]->ARotateSpeed.y = 30;
		}
	}
	return TRUE;
}

/*----------------------------------------------------------------------------*
*						(Valkyrie) Soul Sucker
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_Soul_Sucker(smCHAR *pChar , smCHAR *DesChar)
{
	
	int Temp = 1 , TempIndex = 0 , Temp2 = 1,Temp3 = 1, Temp4 = 1;       
	POINT3D Posi2;
	int TempTime = -10;
	if(!DesChar)return FALSE;    
	SetDynLight( DesChar->pX, DesChar->pY , DesChar->pZ ,255, 180, 100, 150, 130 ,1);  
	//��ȥ�� ��������
	    
	Assa = SetAssaEffect(150,"etc01_01.bmp",DesChar,0,DesChar->PatHeight+1000);                                        
	cAssaEffect[Assa]->Size.w = 5000;    
	cAssaEffect[Assa]->Size.h = 10000;   
	cAssaEffect[Assa]->AniDelayTime = 15; 
	cAssaEffect[Assa]->MOVECODE = ASSA_MOVE;
	cAssaEffect[Assa]->MoveSpeed.y = 100;
	AssaGetAniFileMat(cAssaEffect[Assa]);
	cAssaEffect[Assa]->Time = TempTime;


	Posi2 = cAssaEffect[Assa]->Posi;
	Posi2.y -= 4000;

	//AssaTest_Shot(&cAssaEffect[Assa]->Posi);
	AssaParticle_SpecialHit(&Posi2,0);
	for(int i = 0 ; i < 3 ; i++){                                            
		Assa = SetAssaEffect(80,"fire01_02.bmp",DesChar,0,DesChar->PatHeight-2500);                    
		cAssaEffect[Assa]->Size.w = 2000;  
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_CHAR_POSI;
		if(i==0){
			cAssaEffect[Assa]->MoveSpeed.y = 500; 
		}
		if(i == 1){
			cAssaEffect[Assa]->MoveSpeed.x = 500;
			cAssaEffect[Assa]->MoveSpeed.z = 500;
		}
		if(i == 2){
			cAssaEffect[Assa]->MoveSpeed.x = -500;
			cAssaEffect[Assa]->MoveSpeed.z = -500;
		}
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE;  
		cAssaEffect[Assa]->cAssaTrace->SetLength(10); //������̸� ��´�
		cAssaEffect[Assa]->CenterPosi.x = pChar->pX;
		cAssaEffect[Assa]->CenterPosi.y = pChar->pY+pChar->PatHeight-2500;
		cAssaEffect[Assa]->CenterPosi.z = pChar->pZ; 
		cAssaEffect[Assa]->GatherSpeed = 80;
		cAssaEffect[Assa]->CODE = SKILL_SOUL_SUCKER;
		cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->Time = TempTime;
		TempIndex = Assa;

		//����... 
		Assa = SetAssaEffect(80,"star02Y_03.bmp",DesChar,0,cAssaEffect[Assa]->AddHeight);                   
		cAssaEffect[Assa]->Size.w = 10000;  
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_CHAR_POSI;
		cAssaEffect[Assa]->MoveSpeed.x = cAssaEffect[TempIndex]->MoveSpeed.x;
		cAssaEffect[Assa]->MoveSpeed.y = cAssaEffect[TempIndex]->MoveSpeed.y;
		cAssaEffect[Assa]->MoveSpeed.z = cAssaEffect[TempIndex]->MoveSpeed.z;
		cAssaEffect[Assa]->CenterPosi.x = cAssaEffect[TempIndex]->CenterPosi.x;
		cAssaEffect[Assa]->CenterPosi.y = cAssaEffect[TempIndex]->CenterPosi.y;
		cAssaEffect[Assa]->CenterPosi.z = cAssaEffect[TempIndex]->CenterPosi.z;
		cAssaEffect[Assa]->GatherSpeed = cAssaEffect[TempIndex]->GatherSpeed;
		cAssaEffect[Assa]->Time = TempTime;
		cAssaEffect[Assa]->CODE = SKILL_SOUL_SUCKER;//cAssaEffect[TempIndex]->CODE;
		//cAssaEffect[Assa]->NotFadeIn = 1;

		//����  ���Ѵ� 
		Assa = SetAssaEffect(60,"fire01_02.bmp",pChar,0,pChar->PatHeight-2500);         
		cAssaEffect[Assa]->Size.w = 1000;  
		cAssaEffect[Assa]->MoveSpeed.y = -300;
		AssaGetCirclePosi(cAssaEffect[Assa],256*8,(i+1)*1200);
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE | ASSA_CHAR_POSI;
		if(Temp3 > 0 )Temp3 = -1;
		else Temp3 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 50*Temp3;
		cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
		cAssaEffect[Assa]->cAssaTrace->SetLength(30); //������̸� ��´�
		cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE; 
		cAssaEffect[Assa]->Time = -30+TempTime;
 
		//AssaParticle_Light(&cAssaEffect[Assa]->CenterPosi ,"light01-03.bmp", 5000,0,-30);
		//AssaParticle_Area_Particle(&cAssaEffect[Assa]->CenterPosi ,"star01Y_01.bmp", 16,-30); ������
		///////////��ġ�� ��ƼŬ 
		for(int j = 0 ; j < 3 ; j++){          
			Assa = SetAssaEffect(30,"star01Y_01.bmp",pChar,0,pChar->PatHeight-2500);         
			cAssaEffect[Assa]->Size.w = 2000;  
			cAssaEffect[Assa]->MoveSpeed.y = -200;
			AssaGetCirclePosi(cAssaEffect[Assa],256*8,GetRandomPos(0,4098));
			cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE | ASSA_CHAR_POSI;
			if(Temp2 > 0 )Temp2 = -1;
			else Temp2 = 1;
			cAssaEffect[Assa]->RAngleSpeed = 50*Temp2;
			cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
			cAssaEffect[Assa]->NotFadeIn = 1;
			AssaGetAniFileMat(cAssaEffect[Assa]);
			cAssaEffect[Assa]->AniDelayTime = GetRandomPos(10,15);
			cAssaEffect[Assa]->AniCount = GetRandomPos(0,cAssaEffect[Assa]->AniMaxCount);
			cAssaEffect[Assa]->Time = -30+TempTime;

			////////�������� ���Ѵ� 
			//ȸ�� 
			Assa = SetAssaEffect(50,"star06Y_04.bmp",pChar,0,3000);                            
			cAssaEffect[Assa]->Size.w = 800;  
			cAssaEffect[Assa]->MoveSpeed.y = -200;
			AssaGetCirclePosi(cAssaEffect[Assa],256*7,GetRandomPos(0,4096));
			cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE |ASSA_CHAR_POSI;
			if(Temp4 > 0 )Temp4 = -1;
			else Temp4 = 1;
			cAssaEffect[Assa]->RAngleSpeed = 50*Temp4;
			cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
			cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_SCALE;
			cAssaEffect[Assa]->ScalePosi.y = 10000;
			cAssaEffect[Assa]->NotFadeIn = 1;
			cAssaEffect[Assa]->Time = -30+TempTime;

		}
	}
	return TRUE;
}

/*----------------------------------------------------------------------------*
*						(Saintess) Holy Reflection
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_Holy_Reflection_Defense(smCHAR *pChar)
{
	int Temp =  1 , Temp2 =1; 
	SetDynLight( pChar->pX, pChar->pY , pChar->pZ ,100, 50, 100, 150, 200 ,1);          
	Assa = SetAssaEffect(40,"HolyReflection-gu.ASE",pChar,0,0);                                                
	cAssaEffect[Assa]->MOVECODE = ASSA_CHAR_POSI;
	//cAssaEffect[Assa]->Time = -10; //���⸦ ���߱����� ��� �����̸� �ش� 

	/*
	for(int i = 0 ; i < 3 ; i++){     
		Assa = SetAssaEffect(120,"HolyReplection3.tga",pChar,0,3000);           
		cAssaEffect[Assa]->Size.w = 600; 
		cAssaEffect[Assa]->MoveSpeed.y = 200;
		AssaGetCirclePosi(cAssaEffect[Assa],256*12,GetRandomPos(0,4096));
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE |ASSA_CHAR_POSI;
		if(Temp > 0 )Temp = -1;
		else Temp = 1;
		cAssaEffect[Assa]->RAngleSpeed = 50*Temp;
		//cAssaEffect[Assa]->Time = GetRandomPos(-5*i,-i*4);
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE;
		cAssaEffect[Assa]->cAssaTrace->SetLength(30); //������̸� ��´�
		cAssaEffect[Assa]->NotFadeIn = 1;
	}
	*/

	/*
	for(int i = 0 ; i < 30 ; i++){                          
		Assa = SetAssaEffect(100,"star01M_04.bmp",pChar,0,GetRandomPos(5000,10000));
		cAssaEffect[Assa]->Size.w = 500;  
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE_SPREAD;
		cAssaEffect[Assa]->RAngle = rand()%4096;
		cAssaEffect[Assa]->RDistance.z = 64;
		cAssaEffect[Assa]->Gravity = 10;
		cAssaEffect[Assa]->MoveSpeed.y = 300;
		cAssaEffect[Assa]->NotFadeIn = 1;
	}
	*/
	
	for(int i = 0 ; i < 7 ; i++){                           
		Assa = SetAssaEffect(100,"star01M_04.bmp",pChar,0,7000);    //�⺻������ Char�ϰ��� AngleY�� ����ֱ⶧���� �ʱ�ȭ���ش�
		//cAssaEffect[Assa]->Angle.y = GetRandomPos(0,2046); //Angle �� �ʱ�ȭ���ش� 
		cAssaEffect[Assa]->Angle.y = 0;
		cAssaEffect[Assa]->Size.w = GetRandomPos(700,1000);  
		cAssaEffect[Assa]->StartPosi.z = GetRandomPos(256*15,256*31);
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE;
		if(Temp > 0 )Temp = -1;
		else Temp = 1;
		cAssaEffect[Assa]->MoveSpeed.y = 10*Temp;
		AssaSetStartPosi(cAssaEffect[Assa]);
		//cAssaEffect[Assa]->Time = -10; //���⸦ ���߱����� ��� �����̸� �ش� 

	}
	

	return TRUE;
}

int sinSkillEffect_Holy_Reflection(smCHAR *pChar , int Time)
{
	SetDynLight( pChar->pX, pChar->pY , pChar->pZ ,100, 50, 100, 150, 200 ,1);     
	POINT3D TempPosi = {pChar->pX,pChar->pY,pChar->pZ};          
	int Temp = 1 ,  Temp2 = 1;

	//���� ���� �ո����ƼŬ�� ���ؼ� 
	Assa = SetAssaEffect(100,"Bone",pChar,0,14000);       
	cAssaEffect[Assa]->CODE = SKILL_HOLY_REFLECTION;

	//õ��  
	for(int i = 0 ; i < 2 ; i++){          
		Assa = SetAssaEffect(0,"2HolyReflection.ASE",pChar,0,14000);                                            
		cAssaEffect[Assa]->AniMaxCount = 30; 
		cAssaEffect[Assa]->AniDelayTime = 4;
		cAssaEffect[Assa]->StartPosi.x = -256*10;
		cAssaEffect[Assa]->Angle.y =  ((pChar->Angle.y&ANGCLIP)+ANGLE_270)&ANGCLIP; //��ġ�� ������� Angle����
		AssaSetStartPosi(cAssaEffect[Assa]);
		cAssaEffect[Assa]->Angle.y = (-(pChar->Angle.y&ANGCLIP)+ANGLE_180)&ANGCLIP; //ĳ���� Angle�� �ٽ� ����
		cAssaEffect[Assa]->ActionIndex[1] = 2; //������ ������쿡�� ��Ƴ������ִ� üũ�� �ϱ����ؼ�
		cAssaEffect[Assa]->CODE   = SKILL_HOLY_REFLECTION;

		//cAssaEffect[Assa]->MOVECODE = ASSA_MOVE; �������� ��ô�� 
		//cAssaEffect[Assa]->MoveSpeed.y = -100;
	}
	//���� ����Ʈ 
	for(int i = 0 ; i < 3 ; i++){             
		Assa = SetAssaEffect(70,"flare.tga",pChar,0,12500);      
		cAssaEffect[Assa]->Size.w = GetRandomPos(3000,5000); 
		cAssaEffect[Assa]->Size.h = GetRandomPos(200,500); 
		cAssaEffect[Assa]->FACEY  = (i+1)*(4096/6);
		cAssaEffect[Assa]->NotFadeIn = 1;
		//cAssaEffect[Assa]->ZposiFlag = 1;
		cAssaEffect[Assa]->ActionIndex[1] = 1;
		cAssaEffect[Assa]->Face.r = 255;
		cAssaEffect[Assa]->Face.g = 150;
		cAssaEffect[Assa]->Face.b = 255;
		cAssaEffect[Assa]->CODE   = SKILL_HOLY_REFLECTION;
		cAssaEffect[Assa]->Time = -90;
	}
	//���� ��ƼŬ 
	for(int i = 0 ; i < 20 ; i++){   
		Assa = SetAssaEffect(100,"star05M_02.bmp",pChar,0,12500);                   
		cAssaEffect[Assa]->Size.w = GetRandomPos(300,500);
		//cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE_ANGLE_SPEED;
		cAssaEffect[Assa]->MoveSpeed.z = GetRandomPos(20,40);
		cAssaEffect[Assa]->Angle.y = 0; //Angle�� �ʱ�ȭ���ְ� Random�� ���� ��´�
		cAssaEffect[Assa]->Time = -90;
		cAssaEffect[Assa]->Gravity = 50;
		cAssaEffect[Assa]->CODE   = SKILL_HOLY_REFLECTION;
		//cAssaEffect[Assa]->ZposiFlag = 1;

	}
	//���� ��ƼŬ 
	for(int i = 0 ; i < 50 ; i++){                               
		Assa = SetAssaEffect(120,"flw.ase",pChar,0,12000);                       
		cAssaEffect[Assa]->Size.w = GetRandomPos(700,1500);
		AssaGetCirclePosi(cAssaEffect[Assa],GetRandomPos(256*4,256*5),GetRandomPos(0,4096));
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE | ASSA_CHAR_POSI;
		if(Temp2 > 0 )Temp2 = -1;
		else Temp2 = 1;
		cAssaEffect[Assa]->RAngleSpeed = GetRandomPos(20,30)*Temp2;
		cAssaEffect[Assa]->Time = GetRandomPos(-50,0)-90;
		//cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->Gravity = GetRandomPos(30,50);
		cAssaEffect[Assa]->ActionIndex[0] = 1;
		cAssaEffect[Assa]->CODE   = SKILL_HOLY_REFLECTION;
		cAssaEffect[Assa]->Angle.x = GetRandomPos(0,4096);
		cAssaEffect[Assa]->Angle.y = GetRandomPos(0,4096);
		cAssaEffect[Assa]->Angle.z = GetRandomPos(0,4096);
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_WORLD;
		//cAssaEffect[Assa]->ZposiFlag = 1;

	}
	
	// ��ȣ�� 
	for(int i = 0 ; i < 2 ; i++){   
		Assa = SetAssaEffect(700,"mn.ASE",pChar,0,7000);                                                
		cAssaEffect[Assa]->MOVECODE = ASSA_ANGLE_ROTATE | ASSA_CHAR_POSI; 
		cAssaEffect[Assa]->ARotateSpeed.y = 10;
		cAssaEffect[Assa]->Time = -90;
		cAssaEffect[Assa]->Max_Time = Time*70;
		cAssaEffect[Assa]->ActionIndex[1] = 2; //������ ������쿡�� ��Ƴ������ִ� üũ�� �ϱ����ؼ�
		cAssaEffect[Assa]->CODE   = SKILL_HOLY_REFLECTION;

	}

	return TRUE;
}
/*----------------------------------------------------------------------------*
*						(Valkyrie) sinSkillEffect_Twist_Javelin
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_Twist_Javelin(smCHAR *pChar , smCHAR *DesChar)
{
	Assa = SetAssaEffect(0,"TwistJavelin.ASE",pChar,0,7000);                  
	cAssaEffect[Assa]->AniMaxCount = 60; 
	cAssaEffect[Assa]->AniDelayTime = 1;

	return TRUE;
}

/*----------------------------------------------------------------------------*
*						(Valkyrie) Windy
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_Windy(smCHAR *pChar)
{
	SetDynLight( pChar->pX, pChar->pY , pChar->pZ ,255, 180, 100, 150, 180 ,1);   
	Assa = SetAssaEffect(75,"Bone",pChar,0,7000);                          
	cAssaEffect[Assa]->CODE = SKILL_WINDY;  
	AssaParticle_Char_Light2(pChar,ASSA_COLOR_CODE_YELLOW,200);
	return TRUE;
}
int sinSkillEffect_Windy_Gather(smCHAR *pChar)
{
	int TempIndex ,Temp4=1;   
	//�տ� �Ⱑ���δ� �������� 
	for(int i = 0 ; i < 3 ; i++){                                                                       
		Assa = SetAssaEffect(100,"fire11_02.bmp",pChar,0,pChar->PatHeight-2500);                    
		cAssaEffect[Assa]->Size.w = 1500;  
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_CHAR_POSI;
		//cAssaEffect[Assa]->MoveSpeed.y = 500; 
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_TRACE; 
		cAssaEffect[Assa]->cAssaTrace->SetLength(10); //������̸� ��´�

		cAssaEffect[Assa]->StartPosi.x = 256*8*4;
		cAssaEffect[Assa]->Angle.y =  ((pChar->Angle.y&ANGCLIP)+ANGLE_270)&ANGCLIP;
		AssaSetStartPosi(cAssaEffect[Assa]);
		cAssaEffect[Assa]->Angle.y = 0;
		//cAssaEffect[Assa]->Angle.y =  -((pChar->Angle.y&ANGCLIP))&ANGCLIP;

		//AssaSearchWeaponPos(cAssaEffect[Assa]->pChar,&cAssaEffect[Assa]->CenterPosi,0);
		cAssaEffect[Assa]->CenterPosi.x = cAssaEffect[Assa]->Posi.x; 
		cAssaEffect[Assa]->CenterPosi.y = cAssaEffect[Assa]->Posi.y+cAssaEffect[Assa]->pChar->PatHeight;
		cAssaEffect[Assa]->CenterPosi.z = cAssaEffect[Assa]->Posi.z; 
		cAssaEffect[Assa]->GatherSpeed = 80;
		cAssaEffect[Assa]->CODE = SKILL_WINDY;
		cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->ActionIndex[3] = 1;

		AssaGetCirclePosi(cAssaEffect[Assa],256*64,(i+1)*(4096/3));
		TempIndex = Assa;

		//Assa = SetAssaEffect(80,"star05B_03.bmp",pChar,0,cAssaEffect[Assa]->AddHeight);                   
		Assa = SetAssaEffect(100,"star01Y_04.bmp",pChar,0,cAssaEffect[Assa]->AddHeight);                   
		cAssaEffect[Assa]->Size.w = 4000;  
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_CHAR_POSI;
		cAssaEffect[Assa]->MoveSpeed.x = cAssaEffect[TempIndex]->MoveSpeed.x;
		cAssaEffect[Assa]->MoveSpeed.y = cAssaEffect[TempIndex]->MoveSpeed.y;
		cAssaEffect[Assa]->MoveSpeed.z = cAssaEffect[TempIndex]->MoveSpeed.z;
		cAssaEffect[Assa]->CenterPosi.x = cAssaEffect[TempIndex]->CenterPosi.x;
		cAssaEffect[Assa]->CenterPosi.y = cAssaEffect[TempIndex]->CenterPosi.y;
		cAssaEffect[Assa]->CenterPosi.z = cAssaEffect[TempIndex]->CenterPosi.z;
		cAssaEffect[Assa]->GatherSpeed = cAssaEffect[TempIndex]->GatherSpeed;
		AssaGetCirclePosi(cAssaEffect[Assa],cAssaEffect[TempIndex]->RDistance.z,cAssaEffect[TempIndex]->RAngle);
		cAssaEffect[Assa]->ActionIndex[3] = 1;
		cAssaEffect[Assa]->CODE = SKILL_WINDY;//cAssaEffect[TempIndex]->CODE;
		cAssaEffect[Assa]->Posi.x = cAssaEffect[TempIndex]->Posi.x;
		cAssaEffect[Assa]->Posi.y = cAssaEffect[TempIndex]->Posi.y;
		cAssaEffect[Assa]->Posi.z = cAssaEffect[TempIndex]->Posi.z;

		
		for(int j = 0 ; j < 10 ; j++)
			AssaParticle_RisingParticleContinue(&cAssaEffect[Assa]->Posi,"star05G_02.bmp",700,200,GetRandomPos(40,100));

	}
	/*
	for(int i = 0 ; i < 30 ; i++){                                                                                                                         
		Assa = SetAssaEffect(170,"star06G_04.bmp",pChar,0,-10000);                               
		cAssaEffect[Assa]->Size.w = 600;  
		cAssaEffect[Assa]->MoveSpeed.y = 300;
		AssaGetCirclePosi(cAssaEffect[Assa],256*7,i*256);
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE | ASSA_ROTATE |ASSA_CHAR_POSI;
		if(Temp4 > 0 )Temp4 = -1;
		else Temp4 = 1;
		cAssaEffect[Assa]->RAngleSpeed = 25*Temp4;
		cAssaEffect[Assa]->Time = GetRandomPos(0,100);
		cAssaEffect[Assa]->FACE_TYPE = ASSAFACE_SCALE;
		cAssaEffect[Assa]->ScalePosi.y = 10000*2;
		//cAssaEffect[Assa]->NotFadeIn = 1;

	}
	*/

	return TRUE;
}
/*----------------------------------------------------------------------------*
*						 Holy Mind
*-----------------------------------------------------------------------------*/	
int sinSkillEffect_HolyMind(smCHAR *pChar ,int Time)
{
	Assa = SetAssaEffect(Time*70,"bone",pChar,0,pChar->PatHeight+2000);   
	cAssaEffect[Assa]->CODE = SKILL_HOLY_MIND; 
	cAssaEffect[Assa]->MOVECODE = ASSA_CHAR_POSI;

	Assa = SetAssaEffect(Time*70,"Holy_m2.bmp",pChar,0,pChar->PatHeight+2000);                    
	cAssaEffect[Assa]->Size.w = 5000;
	cAssaEffect[Assa]->MaxAlphaAmount = 100;
	cAssaEffect[Assa]->NotFadeIn = 1;
	cAssaEffect[Assa]->MOVECODE = ASSA_CHAR_POSI;
	//cAssaEffect[Assa]->NotFadeOut = 1;

	/*
	for(int i = 0; i < 8 ; i++){         
		Assa = SetAssaEffect(500,"HM.ASE",pChar,0,pChar->PatHeight+1000,1);//BlendType�� Alpha�� �ش�                           
		cAssaEffect[Assa]->AniDelayTime = 3;
		cAssaEffect[Assa]->AniMaxCount  = 30;
		cAssaEffect[Assa]->NotFadeIn = 1;
		cAssaEffect[Assa]->MOVECODE = ASSA_MOVE_RANDOM_ROTATE; 
		cAssaEffect[Assa]->MOVECODESUB = ASSA_CHAR_POSI; //&������ �ȵǴ� �ڵ带 ����Ҷ� SUB�� ���� (���� �����̴� -_-)
		cAssaEffect[Assa]->Angle.x = GetRandomPos(0,4096);
		cAssaEffect[Assa]->Angle.y = GetRandomPos(0,4096);
		cAssaEffect[Assa]->MoveSpeed.z = 256*8;
		cAssaEffect[Assa]->RAngleSpeed = 30;
		cAssaEffect[Assa]->CODE = SKILL_HOLY_MIND;

	}
	*/


	return TRUE;
}
