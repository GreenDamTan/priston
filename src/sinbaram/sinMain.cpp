/*----------------------------------------------------------------------------*
*	���ϸ� :  sinMain.cpp 
*	�ϴ��� :  �Źٶ� ���� 
*	�ۼ��� :  ����������Ʈ 12�� 
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#define _SINBARAM_TEST   //����ÿ��� �׻� ���ش� 

#include "sinLinkHeader.h"

/*----------------------------------------------------------------------------*
*					         ���� ���� 
*-----------------------------------------------------------------------------*/

smCHAR_INFO *sinChar;		 //ĳ���� ���� ����ü 
smCHAR_INFO sinCharDisplay;  //ĳ���� ���� ����ü 
int sinbaram_Stop = 0;


/////////////////////////
/*----------------------------------------------------------------------------*
*						  ������ �ʱ�ȭ�Ѵ�	
*-----------------------------------------------------------------------------*/	
void sinInit()
{

	InitSub();					//������� �ʱ�ȭ 
	InitCursor();				//Ŀ�� �̹����� �ʱ�ȭ�Ѵ� 
	cItem.Init();				//������ �ʱ�ȭ
	cSkill.Init();				//��ųâ �ʱ�ȭ 
	cInvenTory.Init();			//�κ��丮 �ʱ�ȭ 
	cCharStatus.Init();			//ĳ���ͽ������ͽ� �ʱ�ȭ 
	cInterFace.Init();			//�������̽� �ʱ�ȭ 
	cShop.Init();				//���� �ʱ�ȭ 
	cTrade.Init();
	cMessageBox.Init();			//�޼��� �ڽ� �ʱ�ȭ 
	cSin3D.Init();				//�Źٶ� 3D
	cSinHelp.Init();			//�Źٶ� ����� �ʱ�ȭ
	cSinWarpGate.Init();
	g_CreatePetSystem();
	cHelpPet.Init();
	cSinSod2.Init();
	
	// PC�� �����  ��öȣ!!
	g_PCBang_CreatePetSystem(); 
	cPCBANGPet.Init();         


	//�ϱ�.. 
	chaSiege.init();
	chaQuest.init();
	chaPremiumitem.init();


	//ktj : ��db�� �ӽü����Լ�.  =============================== start
	void web_DB_start();		//������ �̹��ϱ� ������ ����.
	//web_DB_start();
	//ktj : �ӽ÷� ������.
	void clanMENU_INIT(int setN);
	clanMENU_INIT(1);				//2��° init
	//clanMENU_INIT(-1);			//�̰��� �ӽ�
	//=========================================================== end


}
/*----------------------------------------------------------------------------*
*							 ���� ����(70������)							  *
*-----------------------------------------------------------------------------*/	
DWORD dwSinTime = 0;
void sinMain()
{

	sinMouseMain();				//���콺 Ŭ���� �ѹ��� üũ�Ѵ� 
	sinKeyClickMain();			//Ű�Է��� �ѹ����� �����Ѵ� 

//	sinProc(SINKEYDOWN);		//Ű�Է��� ������ 	

	if(sinGetMouseButton(0) == TRUE)
			sinProc(SINLBUTTONDOWN);  //���콺 ���� ��ư  �ٿ������� �̺�Ʈ 
	if(sinGetMouseButton(1) == TRUE)	
			sinProc(SINRBUTTONDOWN);  //���콺 ������ ��ư �ٿ������� �̺�Ʈ 

	if(MouseButtonUpEvent == SINLBUTTONUP){
		sinProc(SINLBUTTONUP);
		MouseButtonUpEvent = 0; //��ư Up�̺�Ʈ �ʱ�ȭ 

	}

	if(MouseButtonUpEvent == SINRBUTTONUP){
		sinProc(SINRBUTTONUP);
		MouseButtonUpEvent = 0; //��ư Up�̺�Ʈ �ʱ�ȭ 

	}

	sinProc(SINMOUSEMOVE);      //���콺�� ���������� 

	
	MainSub();
	cItem.Main();
	cInterFace.Main();
	cSkill.Main();
	cInvenTory.Main();
	cCharStatus.Main();
	cShop.Main();
	cTrade.Main();
	cMessageBox.Main();
	//cSin3D.Main(); �쵿�� ȣ�� ioi
	cSinHelp.Main();
	cSinWarpGate.Main();
	cHelpPet.Main();
	cSinSod2.Main();

	//�ϱ�.. 
	chaSiege.Main();
	chaQuest.Main();
	chaPremiumitem.Main();

	//ktj : �ӽ÷� ������.
	void ktj_imsi_clanMENU_Main();
	ktj_imsi_clanMENU_Main();

	ShowInterFace(); //���� �������̽��� �����ش� 

	//��öȣ : 2005-11-21 ���� 8:59:05
	cPCBANGPet.Main();



	////// �̺�Ʈ�� �����س������� �ð��� ������ Ȱ��ȭ���ش�
	if(sinbaram_Stop){
		dwSinTime++;
		if(dwSinTime >= 3*70) sinbaram_Stop = 0;
	}
	else dwSinTime = 0;

}
/*----------------------------------------------------------------------------*
*							 �̹����� �׸��� 								  *
*-----------------------------------------------------------------------------*/
void sinDraw()
{
	
	renderDevice.SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	renderDevice.SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	renderDevice.SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);

	renderDevice.BeginScene();
	DrawSub();
	renderDevice.EndScene();
	dsDrawOffsetArray = dsARRAY_BOTTOM;

	///////////ĳ���� �������ͽ� 
	renderDevice.BeginScene();
	cCharStatus.Draw();
	renderDevice.EndScene();
	cCharStatus.DrawCharText();  


	dsDrawOffsetArray = dsARRAY_TOP;
	//�����̾� ���� ������
	renderDevice.BeginScene();
	chaPremiumitem.DrawUp();
	renderDevice.EndScene();


	dsDrawOffsetArray = dsARRAY_BOTTOM;

	///////////��ų 
	renderDevice.BeginScene();
	cSkill.Draw();			 
	renderDevice.EndScene();

	dsDrawOffsetArray = dsARRAY_TOP;
	renderDevice.BeginScene();
	cSkill.DrawUp();
	renderDevice.EndScene();
	

	dsDrawOffsetArray = dsARRAY_BOTTOM;
	///////////�κ��丮 
	renderDevice.BeginScene();
	cInvenTory.Draw();		
	renderDevice.EndScene();
	cInvenTory.DrawInvenText();   
	
	dsDrawOffsetArray = dsARRAY_TOP;

	///////////���� 
	
	renderDevice.BeginScene();
	cShop.Draw();			
	renderDevice.EndScene();
	cShop.DrawShopText(); 
	
	
	///////////Ʈ���̵� 
	renderDevice.BeginScene();
	cTrade.Draw();			
	renderDevice.EndScene();
	cTrade.DrawTradeText();


	dsDrawOffsetArray = dsARRAY_BOTTOM;	
	///////////�������̽�
	renderDevice.BeginScene();
	cInterFace.Draw();		
	renderDevice.EndScene();
	cInterFace.DrawInterText();
	
	///////////������ 
	renderDevice.BeginScene();
	cItem.Draw();			
	renderDevice.EndScene();
	cItem.DrawItemText();          


	dsDrawOffsetArray = dsARRAY_TOP;
	///////////�Źٶ� ����� 
	renderDevice.BeginScene();
	cSinHelp.Draw();
	renderDevice.EndScene();
	cSinHelp.DrawText();

	///////////�Źٶ� ��������Ʈ
	renderDevice.BeginScene();
	cSinWarpGate.Draw();
	renderDevice.EndScene();
	cSinWarpGate.DrawText();

	///////////�������̽� ����Ʈ 
	sinDrawInterEffect();

	//ktj :�ӽ�
	dsDrawOffsetArray = dsARRAY_TOP;
	void ktj_imsi_clanMENU_Draw();
	ktj_imsi_clanMENU_Draw();



	//DrawMenuFlame(); //�Լ� �ȿ��� BeginScene �� EndScene�� �Ѵ� 
	
	
	dsDrawOffsetArray = dsARRAY_BOTTOM;

	
	//�ؽ�Ʈ�� ���������� ����ش� 
	sinDrawText();

	dsDrawOffsetArray = dsARRAY_TOP;
	///////////�Źٶ� ����� ��
	renderDevice.BeginScene();
	cHelpPet.Draw(); 
	renderDevice.EndScene();
	cHelpPet.DrawText();


	//�Źٶ� Sod2
	renderDevice.BeginScene();
	cSinSod2.Draw(); 
	renderDevice.EndScene();
	cSinSod2.DrawText();

	//������  
	renderDevice.BeginScene();
	chaSiege.Draw();		
	renderDevice.EndScene();
	chaSiege.DrawText();

	//����Ʈ  
	renderDevice.BeginScene();
	chaQuest.Draw();
	renderDevice.EndScene();

	//�����̾� ���� ������
	renderDevice.BeginScene();
	chaPremiumitem.Draw();
	renderDevice.EndScene();
	chaPremiumitem.DrawText();


	///////////�޼��� �ڽ� 
	renderDevice.BeginScene();
	cMessageBox.Draw();		
	renderDevice.EndScene();
	cMessageBox.DrawMessageText();

	dsDrawOffsetArray = dsARRAY_TOP;

	if (renderDevice.IsDevice())
	{
		renderDevice.BeginScene();
		DrawCursor();
		renderDevice.EndScene();
	}	
}

//�ؽ�Ʈ�� ����ش�
void sinDrawText()
{

	cSkill.DrawSkillText();		   	
	sinTestDrawText();             //�׽�Ʈ �޼��� 
	EffectTestDrawText();		   //����Ʈ �׽�Ʈ DrawTest;
	EffectTestDrawText2();
/*
	HDC	hdc;
	lpDDSBack->GetDC( &hdc );
	SelectObject( hdc, sinFont);
    SetBkMode( hdc, TRANSPARENT );
	SetTextColor( hdc, RGB(255,255,255) );
	if(smConfig.DebugMode){
		dsTextLineOut(hdc,0,0,"i",1);
	}
	lpDDSBack->ReleaseDC( hdc );
*/

}
/*----------------------------------------------------------------------------*
*			���콺�ٿ�  , Ű�Է�  , ���콺 ������� �޼����� �޴´� 
*-----------------------------------------------------------------------------*/	
void sinProc(int Message)
{

	switch(Message)
	{ 
	case SINLBUTTONDOWN:
		if(sinbaram_Stop)return; 
		cInvenTory.CheckMousePotionNumForm();               //���콺 ������üũ�ϰ�
		cMessageBox.LButtonDown(pCursorPos.x,pCursorPos.y); //�޼��� �ڽ�
		cSinSod2.LButtonDown(pCursorPos.x,pCursorPos.y);    //�Źٶ� Sod2
		if(sinMessageBoxShowFlag && !MyShopExpBox && !nName )return;   //ȫ������ �Է¶����� ��ġ�� ���� ��.�� ũ�� // �庰 - �Է�â
		cInterFace.LButtonDown(pCursorPos.x,pCursorPos.y);  //�������̽� 
		cCharStatus.LButtonDown(pCursorPos.x,pCursorPos.y); //ĳ���� �������ͽ� 
		cInvenTory.LButtonDown(pCursorPos.x,pCursorPos.y);  //�κ��丮 
		cSkill.LButtonDown(pCursorPos.x,pCursorPos.y);      //��ų 
		cTrade.LButtonDown(pCursorPos.x,pCursorPos.y);      //Ʈ���̵� 
		cItem.LButtonDown(pCursorPos.x,pCursorPos.y);       //������ 
		cShop.LButtonDown(pCursorPos.x,pCursorPos.y);       //���� 
		cSin3D.LButtonDown(pCursorPos.x,pCursorPos.y);      //�Źٶ� 3D
		cSinHelp.LButtonDown(pCursorPos.x,pCursorPos.y);    //�Źٶ� �����
		cSinWarpGate.LButtonDown(pCursorPos.x,pCursorPos.y);//�Źٶ� ��������Ʈ
		cHelpPet.LButtonDown(pCursorPos.x,pCursorPos.y);    //�Źٶ� �� �����
		cInvenTory.ReFormMousePotionNum();                  //�������ش� 

		//�ϱ�.. 
		chaSiege.LButtonDown(pCursorPos.x,pCursorPos.y);
		chaQuest.LButtonDown(pCursorPos.x,pCursorPos.y);
		chaPremiumitem.LButtonDown(pCursorPos.x,pCursorPos.y);
	break;
	case SINLBUTTONUP:
		if(sinbaram_Stop)return;
		cMessageBox.LButtonUp(pCursorPos.x,pCursorPos.y);   //�޼��� �ڽ� 
		if(sinMessageBoxShowFlag)return;
		cInterFace.LButtonUp(pCursorPos.x,pCursorPos.y);    //�������̽� 
		cCharStatus.LButtonUp(pCursorPos.x,pCursorPos.y);   //ĳ���� �������ͽ� 
		cInvenTory.LButtonUp(pCursorPos.x,pCursorPos.y);    //�κ��丮 
		cShop.LButtonUp(pCursorPos.x,pCursorPos.y);		    //���� 
		cSkill.LButtonUp(pCursorPos.x,pCursorPos.y);	    //��ų 
		cTrade.LButtonUp(pCursorPos.x,pCursorPos.y);	    //Ʈ���̵� 
		cSin3D.LButtonUp(pCursorPos.x,pCursorPos.y);        //�Źٶ� 3D
		cSinHelp.LButtonUp(pCursorPos.x,pCursorPos.y);      //�Źٶ� �����
		cSinWarpGate.LButtonUp(pCursorPos.x,pCursorPos.y);  //�Źٶ� ��������Ʈ
		cHelpPet.LButtonUp(pCursorPos.x,pCursorPos.y);      //�Źٶ� �� �����
		cSinSod2.LButtonUp(pCursorPos.x,pCursorPos.y);      //�Źٶ� Sod2
		//������ �׽�Ʈ
		chaSiege.LButtonUp(pCursorPos.x,pCursorPos.y);
	break;

	case SINRBUTTONDOWN:
		if(MyShopSendButton)return;
		if(sinbaram_Stop)return;
		cMessageBox.RButtonDown(pCursorPos.x,pCursorPos.y); //�޼��� �ڽ� 
		if(sinMessageBoxShowFlag)return;
		cInterFace.RButtonDown(pCursorPos.x,pCursorPos.y);  //�������̽� 
		cCharStatus.RButtonDown(pCursorPos.x,pCursorPos.y); //ĳ���� �������ͽ� 
		cShop.RButtonDown(pCursorPos.x,pCursorPos.y);       //���� 
		cInvenTory.RButtonDown(pCursorPos.x,pCursorPos.y);  //�κ��丮 
		cSkill.RButtonDown(pCursorPos.x,pCursorPos.y);      //��ų 
		cTrade.RButtonDown(pCursorPos.x,pCursorPos.y);      //Ʈ���̵� 
		cItem.RButtonDown(pCursorPos.x,pCursorPos.y);       //������ 
		cSin3D.RButtonDown(pCursorPos.x,pCursorPos.y);      //�Źٶ� 3D
		cSinHelp.RButtonDown(pCursorPos.x,pCursorPos.y);    //�Źٶ� �����
		cSinWarpGate.RButtonDown(pCursorPos.x,pCursorPos.y);//�Źٶ� ��������Ʈ
		cHelpPet.RButtonDown(pCursorPos.x,pCursorPos.y);    //�Źٶ� �� �����
		cSinSod2.RButtonDown(pCursorPos.x,pCursorPos.y);    //�Źٶ� Sod2

	break;

	case SINRBUTTONUP:
		if(sinbaram_Stop)return;
		cMessageBox.RButtonUp(pCursorPos.x,pCursorPos.y);   //�޼��� �ڽ� 
		if(sinMessageBoxShowFlag)return;		
		cInterFace.RButtonUp(pCursorPos.x,pCursorPos.y);    //�������̽� 
		cCharStatus.RButtonUp(pCursorPos.x,pCursorPos.y);   //ĳ���� �������ͽ� 
		cInvenTory.RButtonUp(pCursorPos.x,pCursorPos.y);    //�κ��丮 
		cShop.RButtonUp(pCursorPos.x,pCursorPos.y);		    //���� 
		cSkill.RButtonUp(pCursorPos.x,pCursorPos.y);	    //��ų 
		cTrade.RButtonUp(pCursorPos.x,pCursorPos.y);	    //Ʈ���̵� 
		cMessageBox.RButtonUp(pCursorPos.x,pCursorPos.y);   //�޼��� �ڽ� 
		cSin3D.RButtonUp(pCursorPos.x,pCursorPos.y);        //�Źٶ� 3D
		cSinHelp.RButtonUp(pCursorPos.x,pCursorPos.y);      //�Źٶ� �����
		cSinWarpGate.RButtonUp(pCursorPos.x,pCursorPos.y);  //�Źٶ� ��������Ʈ
		cHelpPet.RButtonUp(pCursorPos.x,pCursorPos.y);		//�Źٶ� �� �����
		cSinSod2.RButtonUp(pCursorPos.x,pCursorPos.y);		//�Źٶ� Sod2
	break;

	case SINMOUSEMOVE:

	break;
	
	case SINKEYDOWN:
		if(sinbaram_Stop)return;
		cMessageBox.KeyDown();   //�޼��� �ڽ� 
		if(sinMessageBoxShowFlag)return;

		cInterFace.KeyDown();    //�������̽� 
		cCharStatus.KeyDown();   //ĳ���� �������ͽ� 
		cInvenTory.KeyDown();    //�κ��丮 
		cShop.KeyDown();		 //���� 
		cSkill.KeyDown();	     //��ų 
		cTrade.KeyDown();	     //Ʈ���̵� 
		cSin3D.KeyDown();		 //�Źٶ� 3D 
		cSinHelp.KeyDown();		 //�Źٶ� �����
		cSinWarpGate.KeyDown();  //�Źٶ� ��������Ʈ
		cHelpPet.KeyDown();      //�Źٶ� Ű�ٿ�
		cSinSod2.KeyDown();		 //�Źٶ� Sod2
		chaSiege.KeyDown();

		

		/////////////////////////////��� Ű 
		if(sinGetKeyClick('X')){
			cHelpPet.PetMessage("*System",3);
			if(!ExitButtonClick)ExitButtonClick =1;
			else ExitButtonClick = 0;
			cInterFace.CheckAllBox(SIN_SYSTEM); //â�� �ϳ��� ���� 
		}
		if(sinGetKeyClick('R')){
			cHelpPet.PetMessage("*RunAndWalk",3);
			if(cInterFace.sInterFlags.RunFlag)cInterFace.sInterFlags.RunFlag = 0;
			else cInterFace.sInterFlags.RunFlag =1;
		}
		if(sinGetKeyClick(VK_TAB)){
			cHelpPet.PetMessage("*Mini_Map",3);
			if(cInterFace.sInterFlags.MapOnFlag)cInterFace.sInterFlags.MapOnFlag = 0;
			else cInterFace.sInterFlags.MapOnFlag = 1;
		}
		if(sinGetKeyClick('Z')){
			cInterFace.sInterFlags.CameraAutoFlag++;
			if(cInterFace.sInterFlags.CameraAutoFlag > 2)cInterFace.sInterFlags.CameraAutoFlag = 0;

		}
		if(sinGetKeyClick('D')){
			cHelpPet.PetMessage("*Party",3);
			ShowParty();
		}
		if(sinGetKeyClick('Q')){
			cHelpPet.PetMessage("*Quest",3);
			ShowQuest();
		}

		if(sinGetKeyClick(VK_SPACE)){ //â�� �ݾ��ش� 
			cMessageBox.CloseMessage();
			if(!cCraftItem.OpenFlag && !cAging.OpenFlag && !SmeltingItem.OpenFlag && !ManufactureItem.m_OpenFlag || !cMixtureReset.OpenFlag) //����¡�� �ͽ��������� ������������ ESC�� �Դ´� // ������ - �ͽ��� ���� â �߰�
				cInterFace.CheckAllBox(SIN_ALL_CLOSE); //â�� �� �ݾ��ش� 

		}
		if(smConfig.DebugMode){ // ��ġ�� �������� �׽�Ʈ Ű    
			if(sinGetKeyClick('T') && sinGetKeyClick(VK_CONTROL) ){
				if(sinTestFlag)sinTestFlag = 0;
				else sinTestFlag = 1;
			}

			if(sinGetKeyClick('F') && sinGetKeyClick(VK_CONTROL) ){ //��¥ ����  
				if(cShop.OpenFlag == SIN_CLOSE)cShop.OpenFlag = SIN_OPEN; 
				else cShop.OpenFlag = SIN_CLOSE;
			
			}
			if(sinGetKeyClick('G') && sinGetKeyClick(VK_CONTROL) ){ //��¥ ���� 
				if(cCraftItem.OpenFlag== SIN_CLOSE)cCraftItem.OpenFlag = SIN_OPEN; 
				else cCraftItem.OpenFlag = SIN_CLOSE;
			}
			if(sinGetKeyClick('H') && sinGetKeyClick(VK_CONTROL) ){ //��¥ ����¡ 
				if(cCraftItem.ForceFlag== SIN_CLOSE)cCraftItem.ForceFlag = SIN_OPEN; 
				else cCraftItem.ForceFlag = SIN_CLOSE;
			}
			//if(sinGetKeyClick('J') && sinGetKeyClick(VK_CONTROL) ){ //��¥ â�� 
			//	if(cCraftItem.ForceFlag== SIN_CLOSE)cCraftItem.ForceFlag = SIN_OPEN; 
			//	else cCraftItem.ForceFlag = SIN_CLOSE;
			//}
			//if(sinGetKeyClick('K') && sinGetKeyClick(VK_CONTROL) ){ //��¥ ��ų  
			//	if(SkillMasterFlag== SIN_CLOSE)SkillMasterFlag = SIN_OPEN; 
			//	else SkillMasterFlag = SIN_CLOSE;
			//}

		}
#ifdef _SINBARAM_TEST
		sinTestKey(); //�׽�Ʈ Ű (����ÿ��� �������� �ʴ´�)
#endif


	break;

	}

}

/*----------------------------------------------------------------------------*
*							    ���� ���� 									  *
*-----------------------------------------------------------------------------*/	
void sinClose()
{
	//ktj : ��db�� �ӽü����Լ�.  =============================== start
	void clanMENU_close();
	clanMENU_close();
	//=========================================================== end

	CloseSub();			 //������� ���� 
	cInvenTory.Close();  //�κ��丮 ���� 
	cCharStatus.Close(); //ĳ���� �������ͽ� ���� 
	cInterFace.Close();  //�������̽� ���� 
	cItem.Close();       //������ ����
	cSkill.Close();		 //��ųâ ���� 
	cShop.Close();		 //���� ���� 
	cTrade.Close();
	cMessageBox.Close(); //�޼��� �ڽ� ���� 
	cSin3D.Close();
	cSinHelp.Close();    //�Źٶ� �����
	cSinWarpGate.Close();//�Źٶ� ��������Ʈ
	cHelpPet.Close();	 //�Źٶ� �굵���
	cSinSod2.Close();	 //�Źٶ� Sod2
	//��öȣ : 2005-11-21 ���� 9:00:31
	cPCBANGPet.Close();	
}

