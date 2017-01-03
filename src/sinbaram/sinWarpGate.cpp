/*----------------------------------------------------------------------------*
*	���ϸ� :  sinWarpGate.cpp	
*	�ϴ��� :  �Źٶ� ���� ����Ʈ
*	�ۼ��� :  ����������Ʈ 2003�� 6��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"
#include "..\\field.h"
#include "Server\\onserver.h"
 
/*----------------------------------------------------------------------------*
*								�������� 			
*-----------------------------------------------------------------------------*/	
cSINWARPGATE cSinWarpGate;
sUNIONCORE_EFFECT sWarpGateEffect[10];
//x-162  y-73
sSINWARPGATE sSinWarpGate={{ {96,199},{64,21},{88,84},{67,177},{78,116},{150,165},{158,137},{155,65}, {140,225} ,{0,0} }};		//�ʵ弱�� ��ǥ
int WarpGateUseCost[10] = { 100,300,500,1000,2000,4000};																	//������ ���Ǵ� ��
int sinWarpGateCODE[10] = {3,21,18,1,6,9,12,29,37};																			//�̵��� �ʵ�	

//���� ĳ��(�̺�Ʈ��) ��������Ʈ------------------------------------------------------------------------/

sSINWARPGATE sHaWarpGate={{{69,207},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}};
int haWarpGateCODE[10] = {33};
//----------------------------------------------------------------------------------------------/


/*----------------------------------------------------------------------------*
*							Ŭ���� �ʱ�, ���� 
*-----------------------------------------------------------------------------*/	
cSINWARPGATE::cSINWARPGATE()
{

}
cSINWARPGATE::~cSINWARPGATE()
{

}
/*----------------------------------------------------------------------------*
*							     �ʱ�ȭ 
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::Init()
{
	//�⺻���� ���� ��ġ�� �����Ѵ�
	sSinWarpGate.MapPosi.x = 272+200;
	sSinWarpGate.MapPosi.y = 120;
	
//	sSinWarpGate.CancelPosi.x = sSinWarpGate.MapPosi.x+200 - 40;
//	sSinWarpGate.CancelPosi.y = sSinWarpGate.MapPosi.y+256 - 20;

	MatWarpMap = CreateTextureMaterial( "Image\\SinImage\\Help\\WG_map.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatIcon    = CreateTextureMaterial( "Image\\SinImage\\Help\\WG_icon.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatSelect  = CreateTextureMaterial( "Image\\SinImage\\Help\\WG_slt.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	Load();
}
/*----------------------------------------------------------------------------*
*						    Bmp������ �ε��Ѵ� 
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::Load()
{
	lpCancelBox			= LoadDibSurfaceOffscreen("image\\clanImage\\chipbutton\\msBOX.bmp");
	lpCancelButton		= LoadDibSurfaceOffscreen("image\\clanImage\\MessageBox\\c1.bmp");
	lpCancelButton_G	= LoadDibSurfaceOffscreen("image\\clanImage\\MessageBox\\c.bmp");
}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::Release()
{

}
/*----------------------------------------------------------------------------*
*							     �׸���  
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::Draw()
{
	int i = 0;
	if(sSinWarpGate.ShowFlag){   
		//���� 
		dsDrawTexImage(MatWarpMap,sSinWarpGate.MapPosi.x,sSinWarpGate.MapPosi.y,256,256,255);

		//ǥ��
		for(i = 0 ; i < sSinWarpGate.GateUseIndex ; i++){
			dsDrawTexImage(MatIcon,sSinWarpGate.MapPosi.x+sSinWarpGate.GatePosi[i].x,
				sSinWarpGate.MapPosi.y+sSinWarpGate.GatePosi[i].y,16,16,255);
		}
		//--------------------���� ĳ��(�̺�Ʈ)------------------------------------
		for(i = 0 ; i < sHaWarpGate.GateUseIndex ; i++){
			dsDrawTexImage(MatIcon,sSinWarpGate.MapPosi.x+sHaWarpGate.GatePosi[i].x,
				sSinWarpGate.MapPosi.y+sHaWarpGate.GatePosi[i].y,16,16,255);

		}
		//---------------------------------------------------------------------------
		DrawSprite(sSinWarpGate.CancelPosi.x , sSinWarpGate.CancelPosi.y ,lpCancelBox,0,0,48,23);
		if(sSinWarpGate.CancelFlag){
			DrawSprite(sSinWarpGate.CancelPosi.x+8 , sSinWarpGate.CancelPosi.y+4 ,lpCancelButton,0,0,32,16);
		}
		else{
			DrawSprite(sSinWarpGate.CancelPosi.x+8 , sSinWarpGate.CancelPosi.y+4 ,lpCancelButton_G,0,0,32,16);
		}
	}

	for( i = 0 ; i < 10 ; i++){
		if(sWarpGateEffect[i].Flag){
			dsDrawTexImage(sWarpGateEffect[i].MatNum,sWarpGateEffect[i].x,sWarpGateEffect[i].y,sWarpGateEffect[i].w,sWarpGateEffect[i].h,
				sWarpGateEffect[i].Alpha);
		}
	}


	//				DrawSprite(sSinHelp[i].ButtonPosi.x+9,sSinHelp[i].ButtonPosi.y+4,cMessageBox.lpButtonOk[1],0,0,32,16); //Ȯ�α��� 
			 
}
/*----------------------------------------------------------------------------*
*							     ����  
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::Main()
{
	int  i = 0;	

	sSinWarpGate.CancelPosi.x = sSinWarpGate.MapPosi.x+224 - 40;     
	sSinWarpGate.CancelPosi.y = sSinWarpGate.MapPosi.y+238 - 20;

	sSinWarpGate.GateSelect = -1; 
	sHaWarpGate.GateSelect  = -1;
	sSinWarpGate.CancelFlag = 0;

	
	if(sSinWarpGate.ShowFlag){
	

		for(i = 0 ; i < sSinWarpGate.GateUseIndex ; i++){  
			if(sSinWarpGate.MapPosi.x + sSinWarpGate.GatePosi[i].x-5 < pCursorPos.x && sSinWarpGate.MapPosi.x + sSinWarpGate.GatePosi[i].x +21 > pCursorPos.x &&
				sSinWarpGate.MapPosi.y + sSinWarpGate.GatePosi[i].y-5 < pCursorPos.y && sSinWarpGate.MapPosi.y + sSinWarpGate.GatePosi[i].y +21 > pCursorPos.y ){
					sSinWarpGate.GateSelect = i;
					SetWarGateSelectEffect(sSinWarpGate.MapPosi.x + sSinWarpGate.GatePosi[i].x , sSinWarpGate.MapPosi.y + sSinWarpGate.GatePosi[i].y);
					cSinHelp.sinShowHelp(SIN_HELP_KIND_WARPGATE,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),WarpGateDoc[i+1]);  
				}

		}

		//-------------------���� ĳ��(�̺�Ʈ��)-------------------------------------------------------------------------/
		for(i = 0 ; i < sHaWarpGate.GateUseIndex ; i++){  
			if(sSinWarpGate.MapPosi.x + sHaWarpGate.GatePosi[i].x-5 < pCursorPos.x && sSinWarpGate.MapPosi.x + sHaWarpGate.GatePosi[i].x +21 > pCursorPos.x &&
				sSinWarpGate.MapPosi.y + sHaWarpGate.GatePosi[i].y-5 < pCursorPos.y && sSinWarpGate.MapPosi.y + sHaWarpGate.GatePosi[i].y +21 > pCursorPos.y ){
					sHaWarpGate.GateSelect = i;
					SetWarGateSelectEffect(sSinWarpGate.MapPosi.x + sHaWarpGate.GatePosi[i].x , sSinWarpGate.MapPosi.y + sHaWarpGate.GatePosi[i].y);
					// �庰 - ���� ĳ��
					/*
					if ( sBiInfo && (sBiInfo->EventPlay[0]&BIMASK_ADULT_USER)!=0 ) {
						cSinHelp.sinShowHelp(SIN_HELP_KIND_WARPGATE,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),HaWarpGateDoc[i]);  
					}
					else{
						cSinHelp.sinShowHelp(SIN_HELP_KIND_WARPGATE,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),haC_Not_TeleportFilePath);  
					}
					*/
				}
		}
		//-------------------------------------------------------------------------------------------------------------------/
	
	
		if(sSinWarpGate.GateSelect < 0 && sHaWarpGate.GateSelect < 0){ //��� �ߺ�ȣ��Ǵ°��� ���´�

			cSinHelp.sinShowHelp(SIN_HELP_KIND_WARPGATE,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),WarpGateDoc[0]);  
		}
		if(sSinWarpGate.CancelPosi.x < pCursorPos.x && sSinWarpGate.CancelPosi.x + 32 > pCursorPos.x &&
			sSinWarpGate.CancelPosi.y < pCursorPos.y && sSinWarpGate.CancelPosi.y+21 > pCursorPos.y ){
				sSinWarpGate.CancelFlag = 1;

			}
	}
	//����Ʈ
	for( i = 0 ; i < 10 ; i++){  
		if(sWarpGateEffect[i].Flag){
			sWarpGateEffect[i].x--;
			sWarpGateEffect[i].y--;
			sWarpGateEffect[i].w+=2;
			sWarpGateEffect[i].h+=2;
			sWarpGateEffect[i].Alpha -=5;
			sWarpGateEffect[i].Time++;
			if(sWarpGateEffect[i].Max_Time < sWarpGateEffect[i].Time){
				memset(&sWarpGateEffect[i],0,sizeof(sUNIONCORE_EFFECT));

			}
		}
	}

	if(sSinWarpGate.EffectFlag){
		sSinWarpGate.EffectTime++;
		if(sSinWarpGate.EffectTime > 40){
			sSinWarpGate.EffectTime = 0;
			sSinWarpGate.EffectFlag = 0;
		}
	}


	
	//cInvenTory.UnionCoreParticle2(UnionCorePosi.x , UnionCorePosi.y); //����Ʈ 
}
/*----------------------------------------------------------------------------*
*							     ���� 
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::Close()
{

}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::LButtonDown(int x , int y)
{
	sHaWarpGate.MoveFieldNum=0;
	sSinWarpGate.MoveFieldNum=0;
	if(sSinWarpGate.ShowFlag){     
		if(sSinWarpGate.CancelFlag){
			sSinWarpGate.ShowFlag = 0;
			WingWarpGate_Field(-1);

		}
		
		if(sSinWarpGate.GateSelect > -1){  
			if(FieldLimitLevel_Table[sinWarpGateCODE[sSinWarpGate.GateSelect]] > sinChar->Level){
				cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_LEVEL);
				return;

			}
			if(sSinWarpGate.GateUseIndex < 4 || sSinWarpGate.GateSelect <2){ //��ī���ٰ� �ʶ��̿��� �������� ��쿡�� ��¥��
				sSinWarpGate.ShowFlag = 0;
				sSinWarpGate.MoveFieldNum = sSinWarpGate.GateSelect;
				cMessageBox.ShowMessage3(MESSAGE_WARP,WarpGateName[sSinWarpGate.GateSelect+1]);

			}
			else{
				if(lpCurPlayer->OnStageField >=0 && StageField[lpCurPlayer->OnStageField]->FieldCode ==
					sinWarpGateCODE[sSinWarpGate.GateSelect]){
					sSinWarpGate.ShowFlag = 0;
					sSinWarpGate.MoveFieldNum = sSinWarpGate.GateSelect;
					sSinWarpGate.SameAreaFlag = 1;
					cMessageBox.ShowMessage3(MESSAGE_WARP,WarpGateName[sSinWarpGate.GateSelect+1]);

				}
				else{
					sSinWarpGate.SameAreaFlag = 0;
					if(cShop.haBuyMoneyCheck(WarpGateUseCost[sSinWarpGate.GateUseIndex-4])){
						sSinWarpGate.ShowFlag = 0;
						sSinWarpGate.MoveFieldNum = sSinWarpGate.GateSelect;
						cMessageBox.ShowMessage3(MESSAGE_WARP,WarpGateName[sSinWarpGate.GateSelect+1]);
					}
					else{
						cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
					}

				}
			}
		}
		if(sHaWarpGate.GateSelect > -1){
			//���� �������� ���� Ŭ������ �޼���
			//if(FieldLimitLevel_Table[sinWarpGateCODE[sSinWarpGate.GateSelect]] > sinChar->Level){
			//	cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_LEVEL);
			//	return;
			//}
			if(lpCurPlayer->OnStageField >=0 && StageField[lpCurPlayer->OnStageField]->FieldCode ==
					haWarpGateCODE[sHaWarpGate.GateSelect]){
					sSinWarpGate.ShowFlag = 0;
					sHaWarpGate.MoveFieldNum = sHaWarpGate.GateSelect+1;
					sSinWarpGate.SameAreaFlag = 0;
					cMessageBox.ShowMessage3(MESSAGE_WARP,HaWarpGateName[sHaWarpGate.GateSelect]);

			}
			//�����ʵ� �̵� ����
		//	else if(sBiInfo && (sBiInfo->EventPlay[0]&BIMASK_ADULT_USER)==0 ){
				//sSinWarpGate.SameAreaFlag = 0;
		//		sSinWarpGate.ShowFlag=0;
		//		cMessageBox.ShowMessage(MESSAGE_NOT_CASTLE_TELEPORT);
		//		WingWarpGate_Field(-1);
				//cSinHelp.sinShowHelp(SIN_HELP_KIND_C_TELEPORT,QuestMessageBoxPosi2.x,QuestMessageBoxPosi2.y,QuestMessageBoxSize2.x,QuestMessageBoxSize2.y, RGBA_MAKE(0,15,128,125),haC_Not_TeleportFilePath);  
				//sHaWarpGate.MoveFieldNum = sHaWarpGate.GateSelect;
				
		//	}
			else{
				sSinWarpGate.SameAreaFlag = 0;
				if(cShop.haBuyMoneyCheck(WarpGateUseCost[sSinWarpGate.GateUseIndex-4])){
					sSinWarpGate.ShowFlag = 0;
					sHaWarpGate.MoveFieldNum = sHaWarpGate.GateSelect+1;
					cMessageBox.ShowMessage3(MESSAGE_WARP,HaWarpGateName[sHaWarpGate.GateSelect]);
				}
				else{
					cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_MONEY);
				}

			}

		}
	}

}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::LButtonUp(int x , int y)
{
}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::RButtonDown(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::RButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::KeyDown()
{
	//�׽�Ʈ�� Ű
	if(smConfig.DebugMode){                  
		if(sinGetKeyClick('P')){   
			//SerchUseWarpGate();
		}
	}
}
/*----------------------------------------------------------------------------*
*							TEXT �� ����ش�
*-----------------------------------------------------------------------------*/	
void cSINWARPGATE::DrawText()
{
/*
	HDC	hdc;
	char strBuff[128]; 
	lpDDSBack->GetDC( &hdc );

	SelectObject( hdc, sinFont);
    SetBkMode( hdc, TRANSPARENT ); 
	SetTextColor( hdc, RGB(255,255,255) ); 

	lpDDSBack->ReleaseDC( hdc );
	*/
}
/*----------------------------------------------------------------------------*
*						Ȱ��ȭ�� ��������Ʈ�� ã�´�
*-----------------------------------------------------------------------------*/	
int cSINWARPGATE::SerchUseWarpGate()
{
	sSinWarpGate.ShowFlag = 1;
	sITEM *pItem=0; 
	sSinWarpGate.GateUseIndex = 0;  
	sHaWarpGate.GateUseIndex  = 0;
	int   i = 0;
	for( i = 0; i < INVENTORY_MAXITEM*2 ; i++){
		if(i<100){
			if(!cInvenTory.InvenItem[i].Flag)continue;
			pItem = &cInvenTory.InvenItem[i];
		}
		else{
			if(!cInvenTory.InvenItemTemp[i-100].Flag)continue;
			pItem = &cInvenTory.InvenItemTemp[i-100];
		}
		switch(pItem->sItemInfo.CODE){
			case (sinQW1 | sin01): //��Ż��
				sSinWarpGate.GateUseIndex =  4;
			break;
			case (sinQW1 | sin02): //�ǹ���
				sSinWarpGate.GateUseIndex =  5;
			break;
			case (sinQW1 | sin03): //�����
				sSinWarpGate.GateUseIndex =  6;
			break;
			case (sinQW1 | sin04): //���̾���
				sSinWarpGate.GateUseIndex =  7;
			break;
			case (sinQW1 | sin05): //���̾ƽ���
				sSinWarpGate.GateUseIndex =  8;
			break;
			case (sinQW1 | sin06): //�ͽ�Ʈ����		�ν�Ʈ ���Ϸ��� (�����߰�)
				sSinWarpGate.GateUseIndex =  9;
			break;
			
		}
	}
	//���� ���� Ŭ�� �� ��������Ʈ Ȱ��ȭ
	if(rsBlessCastle.dwMasterClan == GetClanCode(lpCurPlayer->smCharInfo.ClassClan)&&GetClanCode(lpCurPlayer->smCharInfo.ClassClan)!=0) {
			sHaWarpGate.GateUseIndex = 1;
	}
	if(!sSinWarpGate.GateUseIndex)
		sSinWarpGate.GateUseIndex =  2;//�⺻������ ��ī���ٰ� �ʶ��̴� �����ִ� 
	return TRUE;
}

/*----------------------------------------------------------------------------*
*						���õ� ��������Ʈ�� ����Ʈ 
*-----------------------------------------------------------------------------*/	
int cSINWARPGATE::SetWarGateSelectEffect(int x , int y)
{ 
	
	if(sSinWarpGate.EffectFlag)return FALSE; 
	int  i = 0;
	for( i = 0 ; i < 3 ; i++){  
		if(!sWarpGateEffect[i].Flag){
			sWarpGateEffect[i].Flag = 1;
			sWarpGateEffect[i].MatNum = MatSelect;
			sWarpGateEffect[i].Max_Time  = 20+(i*2);
			sWarpGateEffect[i].x = x;
			sWarpGateEffect[i].y = y;
			sWarpGateEffect[i].w = 16;
			sWarpGateEffect[i].h = 16;
			sWarpGateEffect[i].Alpha = 155+(i*20);

		}
	}
	sSinWarpGate.EffectFlag = 1;
	return TRUE;
}