/*----------------------------------------------------------------------------*
*	���ϸ� :  sinHelpPet.cpp
*	�ϴ��� :  ����� ���� �ͱٴ� 
*	�ۼ��� :  ����������Ʈ 3�� 
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#include "sinLinkHeader.h"

//#include "..\\tjboy\\clanmenu\\tjclan.h"
//#include "..\\tjboy\\clanmenu\\Help.h"
#include "..\\field.h"

cPCBANGPET cPCBANGPet;
int MatPcBangPetImage[4] = {0,};
//int PetMessageOkButtonFlag = 0;
/*----------------------------------------------------------------------------*
*								Init 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::Init()
{
	//PetShow = 1; //�ϴ��� �������°ɷ� �Ѵ�
	Load();
}
/*----------------------------------------------------------------------------*
*								Load 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::Load()
{
	//MatPetImage[0] =  CreateTextureMaterial( "Image\\SinImage\\PetData\\PR_Box.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );
	MatPcBangPetImage[0] =  CreateTextureMaterial( "Effect\\PcBangPet\\Pet_Phoenimin\\Pet_terry01.TGA" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatPcBangPetImage[1] =  CreateTextureMaterial( "Effect\\PcBangPet\\Pet_Phoenimin\\Pet_terry01.TGA" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatPcBangPetImage[2] =  CreateTextureMaterial( "Effect\\PcBangPet\\Pet_Phoenimin\\Pet_terry01.TGA" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	MatPcBangPetImage[3] =  CreateTextureMaterial( "Effect\\PcBangPet\\Pet_Phoenimin\\Pet_terry01.TGA" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

}
/*----------------------------------------------------------------------------*
*								Release 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::Release()
{

}
/*----------------------------------------------------------------------------*
*								Draw 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::Draw() //2D
{
	/*
	if(PetKind && PetShow){     
		if(HelpMsgFlag){   
			g_Help.DrawBox(3,3,32,4);//nBoxHeight_Count);     
			dsDrawTexImage( MatPetImage[PetKind-1] , 10, 20, 64, 64 , 255 );  //�������̽� ����   
			DrawSprite(522,94, lpbltr_Button_OK_G,0,0,47,12);    
			if(PetMessageOkButtonFlag){
				DrawSprite(522,94 , lpbltr_Button_OK,0,0,47,12);    
			}
			//dsDrawTexImage( MatPetImage[2] , 10, 20, 64, 64 , 255 );  //�������̽� ����   
		}
		DrawPetMessage();
	}
	*/
}
void cPCBANGPET::Draw(POINT3D *pPosi , POINT3D *pAngle) //3D
{

}
/*----------------------------------------------------------------------------*
*								DrawText 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::DrawText()
{
	/*
	if(!HelpMsgFlag)return;
	HDC	hdc;
	lpDDSBack->GetDC( &hdc );     
	SelectObject( hdc, sinFont); 
    SetBkMode( hdc, TRANSPARENT );
	SetTextColor( hdc, RGB(255,255,255) ); 

	for(int  i = 0 ; i < (int)PetMsg_Map[HelpMsgId].size(); i++){  
		dsTextLineOut(hdc,75,22+(i*15), PetMsg_Map[HelpMsgId][i]->c_str(), PetMsg_Map[HelpMsgId][i]->length());

	}

	lpDDSBack->ReleaseDC( hdc );
	*/
	
}
/*----------------------------------------------------------------------------*
*								Close 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::Close()
{

}
/*----------------------------------------------------------------------------*
*								Main 			
*-----------------------------------------------------------------------------*/	
DWORD sinPcBangPetLoadingTime = 0;
void cPCBANGPET::Main()
{
	//�ѹ��� ����ȴ� (ȣ�������������� ������������ -_- )
	if(LoadingPet){
		sinPcBangPetLoadingTime++;
		if(sinPcBangPetLoadingTime >= 70*3){
			CheckPetData();
			LoadingPet = 0;
			sinPcBangPetLoadingTime =0;

		}
	}
/*
	if(PetKind && PetShow){
		PetRandomNormalMessage();//��� ���� �޼���
		PetRandomBattleMessage();//��Ʋ ���� �޼���
	}

	if(PetKind && PetShow){
		if(MsgFlag){
			MsgTime++;
			if(MsgTime > (70*4)){ //4�ʰ� �޼����� �����ش�
				MsgFlag = 0;
				MsgTime = 0;
				szTempId[0] = 0;
				MessageIndex = -1;
			}
		}
	}

	string TempString;
	if(PetMessageOkButtonFlag !=2)
		PetMessageOkButtonFlag = 0;
	if(HelpMsgFlag){ 
		HelpMsgTime++;
		if (  522< pCursorPos.x  && 522+50 > pCursorPos.x  && 
			94 < pCursorPos.y  && 94+25 > pCursorPos.y  ){
				if(PetMessageOkButtonFlag != 2){
					PetMessageOkButtonFlag = 1;
				}
		}
		if(HelpMsgTime > (70*15) || PetMessageOkButtonFlag == 2){ //(Ȯ��) �������� ���¿��� 10�ʰ� �޼����� �����ش�
			PetMessageOkButtonFlag = 0;
			HelpMsgTime = 0;
			HelpMsgFlag = 0;
			//���⼭ Pet�� �����ش� (������ �޼����� ������)
			if( lstrcmp(HelpMsgId,"*Level_Message10-2") ==0 ){
				if(sinChar->Level >= 10){
					ClosePet();

				}
			}

			//���� �޼����� �����ش�
			if(!Message_Keep.empty()){
				HelpMsgFlag = 1;
				TempString = Message_Keep.front();
				lstrcpy(HelpMsgId , TempString.c_str());
				Message_Keep.pop_front(); //ù��° ���� ���� 
			}
		}
	}
	*/
}
/*----------------------------------------------------------------------------*
*								LButtonDown 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::LButtonDown(int x , int y)
{
	/*
	if(PetMessageOkButtonFlag){
		PetMessageOkButtonFlag = 2;

	}
	*/
}
/*----------------------------------------------------------------------------*
*								LButtonUp 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::LButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*								RButtonDown 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::RButtonDown(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*								RButtonUp 			
*-----------------------------------------------------------------------------*/	
void cPCBANGPET::RButtonUp(int x, int y)
{

}

/*----------------------------------------------------------------------------*
*							���� ������ �о�´�	
*-----------------------------------------------------------------------------*/	
/*
char *szPetMessageCommand[] = {
	"*PetKind",
	"*PetName",
	"*Welcom_ALL",
	"*Welcom_Tems",
	"*Welcom_Mora",
	"*ChatBox",
	"*ChatTab_All",
	"*ChatTab_Clan",
	"*ChatTab_Party",
	"*ChatTab_Trade",
	"*ChatTab_Whisper",
	"*Bar_Stamina",
	"*Bar_Life",
	"*LeftSkill",
	"*RightSkill",

	"*Bar_Mana",
	"*DayandNight",
	"*Bar_Exp",
	"*Exp_Num",
	"*Potion_ShortKey",

	"*RunAndWalk",
	"*Camera_Hand",
	"*Camera_Auto",
	"*Camera_Fix",
	"*Mini_Map",
	"*CarStatus",
	"*InvenTory",
	"*Skill",
	"*Party",
	"*Quest",
	"*System",
	"*InvenTory_Item",
	"*Drop_Item",
	"*Monster",
	"*Message",


	"*Battle_Message",
	"*Normal_Message",

	"*Level_Message2-1",
	"*Level_Message2-2",
	"*Level_Message3",
	"*Level_Message4",
	"*Level_Message5",
	"*Level_Message6-1",
	"*Level_Message6-2",
	"*Level_Message7",
	"*Level_Message8",
	"*Level_Message9-1",
	"*Level_Message9-2",
	"*Level_Message10-1",
	"*Level_Message10-2",
	
	"*Special_Message8_Mecha",
	"*Special_Message8_Fighter",
	"*Special_Message8_Pikeman",
	"*Special_Message8_Archer",
	"*Special_Message8_Knight",
	"*Special_Message8_Atalanta",
	"*Special_Message8_Priestess",
	"*Special_Message8_Magician",
	"*Special_Message9",
	"", //�̰� �Ⱦ��� ���ȴ�

};
*/

/*----------------------------------------------------------------------------*
*								KeyDown 			
*-----------------------------------------------------------------------------*/	
//int sinTestMessageIndex = 0;
void cPCBANGPET::KeyDown()
{
	/*
	//�޼��� �׽�Ʈ Ű 
	int len = 0; 
	if(smConfig.DebugMode){                                                                
		if(sinGetKeyClick('B')){		
			len = lstrlen(szPetMessageCommand[38+sinTestMessageIndex]);
			if(len){
				PetMessageHelp(szPetMessageCommand[38+sinTestMessageIndex]);
				sinTestMessageIndex++;
			}
			else{
				sinTestMessageIndex = 0;

			}
		}
	}
	*/
}
/*----------------------------------------------------------------------------*
*							���� ������ �о�´�	
*-----------------------------------------------------------------------------*/	
int cPCBANGPET::GetFileData()
{
	/*

	char *szFilePath = "Image\\SinImage\\PetData\\PetMessage.txt";

	char decode[512];
	char decode2[512];
	char line[512];

	FILE	*fp;
	char *p;
	int CmdNum;

	fp = fopen( szFilePath , "rb" );
	if ( fp==NULL ) return FALSE;

	CmdNum = 0;
	while( !feof( fp ) )//  feof: file end���� �о�� 
	{
		if( fgets( line, 255, fp ) == NULL)	break; 
		p = GetWord( decode , line);

		if ( lstrcmp( decode, szPetMessageCommand[CmdNum] ) == 0 ) {
			while(1){
				if( fgets( line, 255, fp ) == NULL)	break; 
 				GetString( decode , line);
				GetWord(decode2 , line); //end �� �����ϱ����ؼ� ���
				if(lstrcmp(decode2, "end" ) ==0 )break;
				PetMsg_Map[szPetMessageCommand[CmdNum]].push_back(new string(decode));
				
			}
			CmdNum++;
		}
	}
	*/
	return TRUE;
}
/*----------------------------------------------------------------------------*
*						�޼����� �����ش�								
*-----------------------------------------------------------------------------*/	
int cPCBANGPET::PetMessage(char *Id , int Index) //ä��â �޼���
{
	/*
	if(!PetKind || !PetShow)return FALSE; 
 
	if(0 == Index && lstrcmp( Id, szTempId )==0 ){ 
		MsgTime = 1;  //Ÿ���� ���ܼ� �������� ���ش�
		return FALSE; //���� �̺�Ʈ�� ���Ͻ�Ų��
	}
	MessageIndex = Index;
	lstrcpy(szTempId,Id);
	if( Index <= (int)PetMsg_Map[Id].size()-1){
		wsprintf(szPetBuff,"%s : %s",PetMsg_Map["*PetName"][PetKind-1]->c_str(),PetMsg_Map[Id][Index]->c_str()); //�ӽ� �׽�Ʈ   
	}
	MsgFlag = 1;
	MsgTime = 0;
	*/
	return TRUE;
}

int cPCBANGPET::DrawPetMessage()
{
	/*
	if(MsgFlag){  
		PetRendPoint = HoPetRenderPoint;
		DrawCharacterMessage(PetRendPoint.x,PetRendPoint.y,szPetBuff,30);
		
	}
	*/
	return TRUE;
}

int cPCBANGPET::PetMessageHelp(char *Id) //��� ���� �޼���
{
	/*
	//�޼����� ��Ƶд�
	if(HelpMsgFlag)
		Message_Keep.push_back(Id);
	else{
		HelpMsgFlag = 1;
		lstrcpy(HelpMsgId,Id);
	}
	*/
	return TRUE;
}

//Pet ��뿩�ο� �ε����� üũ�Ѵ�
int cPCBANGPET::CheckPetData() 
{

	//3�ʰ� �����̸� ���� �ε��Ѵ� ȣ���������� ������ ��Ź -0-
	if(LoadingPet == 0 ){
		LoadingPet = 1;
		return FALSE;

	}
	if( PetShow == 1 && PetKind == 0){
		PetKind = 1;
	}

	//if(!smConfig.DebugMode)return FALSE; //���� ���߿� ������Ѵ�
	//if(sinChar->Level >=10 )return FALSE;
	/*
	char szTempBuff[128];
	                                                                
	//Pet������ �о�� �ε��Ѵ�
	if(!GetFileData())return FALSE; //������ �д´� 
	if(PetKind == 0){
		PetKind = GetRandomPos(1,4);
		//�̸��� ��¦ ������ 
		wsprintf(szTempBuff,PetMsg_Map["*Welcom_ALL"][0]->c_str(),PetMsg_Map["*PetKind"][PetKind-1]->c_str());
		PetMsg_Map["*Welcom_ALL"][0]->assign(szTempBuff);
		
		wsprintf(szTempBuff,PetMsg_Map["*Welcom_ALL"][1]->c_str(),PetMsg_Map["*PetName"][PetKind-1]->c_str());
		PetMsg_Map["*Welcom_ALL"][1]->assign(szTempBuff);
		PetMessageHelp("*Welcom_ALL");

		//������ �޼����� �����ش�
		if(sinChar->JOB_CODE < 5 ){ //�۽�ũ�� 
			PetMessageHelp("*Welcom_Tems");

		}
		else{ //����̿�
			PetMessageHelp("*Welcom_Mora");

		}
	}

	//��⼭ ���� �޽��� ����Ʈ�� �ͱٴ�
	int i = 0; 
	for(i = 0; i < (int)PetMsg_Map["*Normal_Message"].size();  i++){
		NormalMsg_Index.push_back(i);
	}
	for(i = 0; i < (int)PetMsg_Map["*Battle_Message"].size();  i++){
		BattleMsg_Index.push_back(i);
	}
	*/
	ShowPet();//Pet�� �����ش�
	PetShow = 1; //�ϴ��� �������°ɷ� �Ѵ�

	return TRUE;
}

//���� �޼���
//DWORD dwRandomMsgTime = 0;
int cPCBANGPET::PetRandomNormalMessage()
{

	if(!PetKind || !PetShow)return FALSE;	

	/*
	//�ϴ� �Ϲݸ޼����� üũ�Ѵ�
	int RandomNum = 0;
	int i = 0; 
	dwRandomMsgTime++;
	if(lpCurPlayer->OnStageField>=0 && StageField[ lpCurPlayer->OnStageField ]->State == FIELD_STATE_VILLAGE){
		if(dwRandomMsgTime >= 70*30){
		//if(dwRandomMsgTime >= 70){ //�׽�Ʈ�� ���� 1�ʸ�
			if(NormalMsg_Index.size() != 0 ){
				RandomNum = GetRandomPos((int)NormalMsg_Index.front(),(int)NormalMsg_Index.back());
				NormalMsg_Index.remove(RandomNum); 
				PetMessage("*Normal_Message",RandomNum);
				dwRandomMsgTime = 0;
			}
			else{ //�޼����� �ٻ��Ǹ� �ٽ� �������ش�
				for(i = 0; i < (int)PetMsg_Map["*Normal_Message"].size();  i++){
					NormalMsg_Index.push_back(i);
				}
			}
		}
	}
	*/
	return TRUE;
}
//int PetExpMessage[] = {5,10,20,30,35,40,50,60,70,80,90,100};
//int PetExpIndex = 0;
//int ExpMessageFlag = 0;
int cPCBANGPET::PetRandomBattleMessage()
{
	int RandomNum = 0;
	int i = 0;
	/*
	//���� 1~7 ���̿� ����ġȹ�濡 ���� �����޼����� ������
	if(LevelFlag){
		PetExpIndex = 0; //�ε��� �ʱ�ȭ
		LevelFlag = 0;

	}
	if(lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_ATTACK){
		if(sinChar->Level < 8){
			if(cPCBANGPET.PetExp > PetExpMessage[PetExpIndex]){//��� 8���� �����޼���
				if(BattleMsg_Index.size() != 0 ){
					RandomNum = GetRandomPos((int)BattleMsg_Index.front(),(int)BattleMsg_Index.back());
					BattleMsg_Index.remove(RandomNum); 
					PetMessage("*Battle_Message",RandomNum);
					PetExpIndex++;
				}
				else{
					for(i = 0; i < (int)PetMsg_Map["*Battle_Message"].size();  i++){
						BattleMsg_Index.push_back(i);
					}
				}
			}
		}
		else{
			if(sinChar->Level == 8 && cPCBANGPET.PetExp >= 90 && !ExpMessageFlag){
				ExpMessageFlag = 1;
				switch(sinChar->JOB_CODE){
					case 1:		//������
						PetMessageHelp("*Special_Message8_Fighter");
					break;
					case 2:		//��ī�ϼ�
						PetMessageHelp("*Special_Message8_Mecha");					
					break;
					case 3:		//����
						PetMessageHelp("*Special_Message8_Archer");					
					break;
					case 4:		//����ũ��
						PetMessageHelp("*Special_Message8_Pikeman");					
					break;
					case 5:		//��Ż��Ÿ
						PetMessageHelp("*Special_Message8_Atalanta");
					break;
					case 6:		//����Ʈ
						PetMessageHelp("*Special_Message8_Knight");
					break;
					case 7:		//������
						PetMessageHelp("*Special_Message8_Magician");
					break;
					case 8:		//������Ƽ��
						PetMessageHelp("*Special_Message8_Priestess");
					break;
				}

			}
			if(sinChar->Level == 9 && cPCBANGPET.PetExp >= 90 && ExpMessageFlag != 2){
				ExpMessageFlag =2;
				PetMessageHelp("*Special_Message9");

			}
		}
	}
	*/
	return TRUE;
}

//Pet�� ȣ���Ѵ�
int cPCBANGPET::ShowPet()
{
	if(!PetKind) return FALSE;
	//Pet�� ȣ���Ѵ�
	switch(PetKind){
		case 1:
			g_HoPCBangAddPet(lpCurPlayer, HoPCBangPet::Type::BurstP);
		break;
		case 2:
			g_HoPCBangAddPet(lpCurPlayer, HoPCBangPet::Type::BurstS);
		break;
		case 3:
			g_HoPCBangAddPet(lpCurPlayer, HoPCBangPet::Type::BurstL);
		break;
		case 4:
			g_HoPCBangAddPet(lpCurPlayer, HoPCBangPet::Type::BurstD);
		break;
	}

	return TRUE;
}
int cPCBANGPET::ClosePet()
{
	
	PetShow = 0;
	g_HoPCBangDeletePet(lpCurPlayer); // PC ���� �ƴϰų� �ð��� �ٵǾ����� �������.
	
	return TRUE;
}
int cPCBANGPET::PetOnOff(char *szBuff)
{
	/*
	//if(sinChar->Level >= 10)return FALSE;
	if(lstrcmp ( szBuff , "/�ǽù�") == 0){
		PetShow ^= 1;
		switch(PetShow){
			case 0:
				ClosePet();

			break;
			case 1:
				ShowPet();
			break;
		}
	}
	*/
	return TRUE;
}