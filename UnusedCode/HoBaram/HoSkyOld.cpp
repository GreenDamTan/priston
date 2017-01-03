#include "..\\HoBaram\\HoLinkHeader.h"
#include "..\\field.h"

HoSky	SkyBackGround;



void HoSkyDrawInfo::LoadRes()
{
	
	WorkState = SKY_STATE_LOADING;

	/*
	for(unsigned index = 0; index < SkyResInfoList.size(); index++)
	{
		smPlayWindowMessage();
		if(SkyResInfoList[index].stageSky == NULL)
		{
			char buffer[256];
			strcpy(buffer, SkyResInfoList[index].resName.c_str());
			smPAT3D *stageSky = smASE_Read(buffer, 0 );
			if(stageSky)
			{
 				stageSky->ZeroNormals();
				for(int i = 0; i < stageSky->nObj3d; i++)
				{
  					stageSky->obj3d[i]->ClipStates = ( SMCLIP_NEARZ | SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
					stageSky->obj3d[i]->ColorEffect = FALSE;
				}
				stageSky->smMaterialGroup->ReadTextures();
				SkyResInfoList[index].stageSky = stageSky;
			}
		}
	}
	*/
}

void HoSkyDrawInfo::MainLoadRes()
{
  	 EnterCriticalSection( &cLoadSection );
	if(WorkState == SKY_STATE_IDLE)
	{
		return;
	}
	else if(WorkState == SKY_STATE_LOADING)
	{
		if(LoadingCount >= (int)SkyResInfoList.size())
		{
			LoadingCount = 0;
			WorkState = SKY_STATE_IDLE;
			LoadingSucess = TRUE;
		}
		
		//smPlayWindowMessage();
		if(SkyResInfoList[LoadingCount].stageSky == NULL && LoadingDelayCount >= 20)
		{
			char buffer[256];
			strcpy(buffer, SkyResInfoList[LoadingCount].resName.c_str());
			smPAT3D *stageSky = smASE_Read(buffer, 0 );
			if(stageSky)
			{
 				stageSky->ZeroNormals();
				for(int i = 0; i < stageSky->nObj3d; i++)
				{
  					stageSky->obj3d[i]->ClipStates = ( SMCLIP_NEARZ | SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
					stageSky->obj3d[i]->ColorEffect = FALSE;
				}
				stageSky->smMaterialGroup->ReadTextures();
				SkyResInfoList[LoadingCount].stageSky = stageSky;
			}
			LoadingDelayCount = 0;
			LoadingCount++;
		}
		
		LoadingDelayCount++;
	}
	else if(WorkState == SKY_STATE_RELEASE)
	{
		if(LoadingCount >= (int)SkyResInfoList.size())
		{
			LoadingCount = 0;
			WorkState = SKY_STATE_IDLE;
			LoadingSucess = FALSE;
		}
		
		//smPlayWindowMessage();
		if(SkyResInfoList[LoadingCount].stageSky != NULL && LoadingDelayCount >= 20)
		{
			delete SkyResInfoList[LoadingCount].stageSky;
			SkyResInfoList[LoadingCount].stageSky = NULL;
			LoadingDelayCount = 0;
			LoadingCount++;
		}

		
		LoadingDelayCount++;
	}
	LeaveCriticalSection( &cLoadSection );
}

void HoSkyDrawInfo::DestroyRes()
{
	if(LoadingSucess)
	WorkState = SKY_STATE_RELEASE;
	/*
	for(unsigned index = 0; index < SkyResInfoList.size(); index++)
	{
		smPlayWindowMessage();
		if(SkyResInfoList[index].stageSky != NULL)
		{
			delete SkyResInfoList[index].stageSky;
			SkyResInfoList[index].stageSky = NULL;
		}
	}
	*/
}

HoSky::HoSky()
{
	FirstSkyLoadFlag = FALSE;
	//TestModeFlag = TRUE;
	Init();
}

HoSky::~HoSky()
{
	Destroy();
}

int HoSky::Draw(int x, int y, int z, int ax, int ay, int az)
{
       //SetDxProjection( (g_PI/4.4f), 800, 600, 20.f, 4000.f );
   	if(DrawIndex != -1)
	 	Draw(x, y, z, ax, ay, az, DrawIndex, DrawAlphaCount, DrawNextAlphaCount);
	
	if(DrawOldIndex != -1)
		Draw(x, y, z, ax, ay, az, DrawOldIndex, DrawOldAlphaCount, DrawNextOldAlphaCount);
	//SetDxProjection( (g_PI/4.4f), 800, 600, 20.f, 4000.f );

 	if(smConfig.DebugMode)
	{
		static BOOL ColorInfoFlag = FALSE;

 		if(VRKeyBuff[VK_NUMPAD7])
		{
		 	if(ColorInfoFlag == FALSE)
				ColorInfoFlag = TRUE;
			VRKeyBuff[VK_NUMPAD0] = FALSE;
		}

		if(ColorInfoFlag)
		{
			HDC opHdc;
			lpDDSBack->GetDC( &opHdc );
			SetBkMode( opHdc, TRANSPARENT );
		 	
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));

			wsprintf(buffer, "ColorR:%d ColorG:%d ColorB:%d Dark:%d", BackColor_R, BackColor_G, BackColor_B, DarkLevel);
			HFONT oldFont;
			if(hFont != NULL)
				oldFont = (HFONT)SelectObject( opHdc , hFont );
		  	
 			SetTextColor( opHdc, RGB(0, 0, 0) );
 			TextOut(opHdc, 11, 101, buffer, strlen(buffer)); 

			SetTextColor( opHdc, RGB(255, 255, 255) );
			TextOut(opHdc, 10, 100, buffer, strlen(buffer)); 
			
			if(smRender.m_FogMode)
				wsprintf(buffer, "���׻��: FogColorR:%d FogColorG:%d FogColorB:%d", FogEndColorR, FogEndColorG, FogEndColorB, FogEndColorA);
			else
				wsprintf(buffer, "���׻�����");

			SetTextColor( opHdc, RGB(255, 255, 255) );

			SetTextColor( opHdc, RGB(0, 0, 0) );
 			TextOut(opHdc, 11, 116, buffer, strlen(buffer)); 
			
			SetTextColor( opHdc, RGB(255, 255, 255) );
			TextOut(opHdc, 10, 115, buffer, strlen(buffer)); 

			wsprintf(buffer, "���׽�����ġ:%d ���׳���ġ:%d", FogStartPos2, FogEndPos2);


			if(oldFont)
				SelectObject(opHdc, oldFont);


			lpDDSBack->ReleaseDC( opHdc );
		}
	}

	return TRUE;
}


int HoSky::Draw(int x, int y, int z, int ax, int ay, int az, unsigned int drawIndex, int alphaCount, int alphaNextCount)
{
	
	int drawR = 0;
	int drawG = 0;
	int drawB = 0;
	int drawA = 0;
	
	int oldR = 0;
	int oldG = 0;
	int oldB = 0;
	int oldA = 0;

	POINT3D camera;
	POINT3D	angle;

	memset(&camera, 0, sizeof(camera));
	memset(&angle, 0, sizeof(angle));

	if(SkyDrawInfoList.size() && drawIndex < (int)SkyDrawInfoList.size())
	{
		if(SkyDrawInfoList[drawIndex].SkyResInfoList.size())
		{
	 		int test = SkyDrawInfoList[drawIndex].SkyResInfoList.size();
 			for(unsigned int index = 0; index < SkyDrawInfoList[drawIndex].SkyResInfoList.size(); index++)
			{
				int alphaTestDepth = smRender.AlphaTestDepth;
  				smRender.AlphaTestDepth = 0;

				oldR = smRender.Color_R;
				oldG = smRender.Color_G;
				oldB = smRender.Color_B;
				oldA = smRender.Color_A;

				drawR = SkyDrawInfoList[drawIndex].SkyResInfoList[index].colorR;
 				drawG = SkyDrawInfoList[drawIndex].SkyResInfoList[index].colorG;
				drawB = SkyDrawInfoList[drawIndex].SkyResInfoList[index].colorB;
				drawA = SkyDrawInfoList[drawIndex].SkyResInfoList[index].colorA;
				
				
 				if(SkyDrawInfoList[drawIndex].SkyResInfoList[index].fadeSlowFlag ==	0)
				{
					if(alphaCount == 100)
					{
  						smRender.Color_R = drawR-255;
						smRender.Color_G = drawG-255;
 						smRender.Color_B = drawB-255;
 						smRender.Color_A = drawA-255;
					}
					else
					{
 						smRender.Color_R = drawR-255;
						smRender.Color_G = drawG-255;
 						smRender.Color_B = drawB-255;
  						smRender.Color_A = int((float)drawA*((float)alphaCount/100.f))-255;
					}
				}
				else 
				{
					//���� ���� �������� �÷� ���̴�...
					if(alphaNextCount == 100)
					{
  						smRender.Color_R = drawR-255;
						smRender.Color_G = drawG-255;
 						smRender.Color_B = drawB-255;
 						smRender.Color_A = drawA-255;
					}
					else
					{
						smRender.Color_R = drawR-255;
						smRender.Color_G = drawG-255;
 						smRender.Color_B = drawB-255;
  						smRender.Color_A = int((float)drawA*((float)alphaNextCount/100.f))-255;
					}
				}
				
				if ( y>230000 ) 
					y=230000;
 				camera.x = 0;
				camera.y = 0;
				camera.z = 0;

				angle.x = 0;
				angle.y = 0;
   				angle.z = 0;
				
				if(SkyDrawInfoList[drawIndex].SkyResInfoList[index].stageSky != NULL)
				{
					
   		 			lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
  			 		SkyDrawInfoList[drawIndex].SkyResInfoList[index].stageSky->TmAnimation( 0, 0,0,0 );
					SkyDrawInfoList[drawIndex].SkyResInfoList[index].stageSky->SetPosi(&camera, &angle);

					camera.x = 0;
					camera.y = y/8;
					camera.z = 0;

					angle.x = ax;
					angle.y = ay;
					angle.z = az;
					
					//smRender.Init();
					smRender.SetCameraPosi(x, y, z, ax, ay, az);					
					//SetDxProjection( (g_PI/4.4f), WinSizeX, WinSizeY, 20.f, 6000.f );
					SkyDrawInfoList[drawIndex].SkyResInfoList[index].stageSky->RenderD3D( &camera, &angle );
					//SetDxProjection( (g_PI/4.4f), WinSizeX, WinSizeY, 20.f, 4000.f );

					lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
				}
				
				smRender.Color_R = oldR;
				smRender.Color_G = oldG;
				smRender.Color_B = oldB;
				smRender.Color_A = oldA;
				
				smRender.AlphaTestDepth = alphaTestDepth;
			}
		}
	}
	
	return TRUE;
}



int HoSky::Main()
{
	
	//if(DrawIndex != -1)
	//{
	for(unsigned int index = 0; index < SkyDrawInfoList.size(); index++)
	{
		SkyDrawInfoList[index].MainLoadRes();
	}
	

    if(DrawAlphaCount < 100 && (TimeCount%6) == 0 )
		DrawAlphaCount++;

	if(DrawOldAlphaCount > 0 && (TimeCount%4) == 0)
		DrawOldAlphaCount--;
	
	if(DrawNextAlphaCount < 100)
		DrawNextAlphaCount++;
	
  	if(DrawNextAlphaCount >= 100 && DrawNextFlag == FALSE)
		DrawNextFlag = TRUE;
	
    if(DrawNextFlag)
  	{
		if(DrawOldAlphaCount < 20 && DrawNextOldAlphaCount > 0 && (TimeCount%2) == 0)
 			DrawNextOldAlphaCount--;
	}
	
	//�ϴ��� ���İ����� ������ ���ؼ� �ϴ��� ��ȭ�� ������ ������ ���..
	if(DrawOldAlphaCount == 0 && DrawNextOldAlphaCount == 0)
	{
		DestroyRes(DrawOldIndex);

		DrawOldAlphaCount = -1;
		DrawNextOldAlphaCount = -1;
		DrawOldIndex = -1;
	}
	
	if(FieldStartColorDelay != -1)
	{
		if(FieldStartColorDelay <= TimeCount)
		{ 
			BackColor_mR = FieldColorR;
			BackColor_mG = FieldColorG;
			BackColor_mB = FieldColorB;
			FieldStartColorDelay = -1;
		}
	}
	
	if( (FogStartColorR != FogEndColorR) ||
		(FogStartColorG != FogEndColorG) ||
		(FogStartColorB != FogEndColorB) ||
 		(FogStartColorA != FogEndColorA) ||
		(FogStartPos1 != FogStartPos2) ||
		(FogEndPos1 != FogEndPos2))
	{
		if(FogStartColorR < FogEndColorR)
			FogStartColorR++;
		if(FogStartColorR > FogEndColorR)
			FogStartColorR--;
		
		if(FogStartColorG < FogEndColorG)
			FogStartColorG++;
		if(FogStartColorG > FogEndColorG)
			FogStartColorG--;

		if(FogStartColorB < FogEndColorB)
			FogStartColorB++;
		if(FogStartColorB > FogEndColorB)
			FogStartColorB--;

		if(FogStartColorA < FogEndColorA)
			FogStartColorA++;
		if(FogStartColorA > FogEndColorA)
			FogStartColorA--;
		


		if(FogStartPos1 < FogStartPos2)
			FogStartPos1++;
		if(FogStartPos1 > FogStartPos2)
			FogStartPos1--;
		
		if(FogEndPos1 < FogEndPos2)
			FogEndPos1++;
		if(FogEndPos1 > FogEndPos2)
			FogEndPos1--;
		

  		smRender.m_dwFogColor = RGBA_MAKE(FogStartColorR, FogStartColorG, FogStartColorB, FogStartColorA);
		if( (smRender.m_dwFogColor%10) == 0)
		lpD3DDevice->SetRenderState( D3DRENDERSTATE_FOGCOLOR, smRender.m_dwFogColor);
		
		smRender.m_fFogStNum  = (float)FogStartPos1*4.f;
		if( (FogStartPos1 % 10) == 0)
		lpD3DDevice->SetRenderState( D3DRENDERSTATE_FOGTABLESTART, *((DWORD*)(&smRender.m_fFogStNum)) );
		
		if( (FogEndPos1 % 10) == 0)
		smRender.m_fFogEndNum = (float)FogEndPos1*4.f;
		lpD3DDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEEND, *((DWORD*)(&smRender.m_fFogEndNum)) );

		if( (FogStartColorR == FogEndColorR) &&
			(FogStartColorG == FogEndColorG) &&
			(FogStartColorB == FogEndColorB) &&
			(FogStartColorA == FogEndColorA) &&
			(FogStartPos1 == FogStartPos2) &&
		    (FogEndPos1 == FogEndPos2) )

		{
		 	smRender.m_dwFogColor = RGBA_MAKE(FogStartColorR, FogStartColorG, FogStartColorB, FogStartColorA);
			lpD3DDevice->SetRenderState( D3DRENDERSTATE_FOGCOLOR, smRender.m_dwFogColor);
		
			smRender.m_fFogStNum  = (float)FogStartPos1*4.f;
			lpD3DDevice->SetRenderState( D3DRENDERSTATE_FOGTABLESTART, *((DWORD*)(&smRender.m_fFogStNum)) );
		
			smRender.m_fFogEndNum = (float)FogEndPos1*4.f;
			lpD3DDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEEND, *((DWORD*)(&smRender.m_fFogEndNum)) );
			
			if(!SkyDrawInfoList[DrawIndex].FogFlag)
				if(smRender.m_FogMode)
					smRender.m_FogMode = 0;
		}
	}

	TimeCount++;
	return true;
}

void HoSky::LoadRes()
{
	for(unsigned int index = 0; index < SkyDrawInfoList.size(); index++)
	{
		SkyDrawInfoList[index].LoadRes();
	}
}

void HoSky::LoadRes(int index)
{

	if(index < 0 || index >= (int)SkyDrawInfoList.size())
		return;
	SkyDrawInfoList[index].LoadRes();
}

void HoSky::DestroyRes()
{
	for(unsigned int index = 0; index < SkyDrawInfoList.size(); index++)
	{
		SkyDrawInfoList[index].DestroyRes();
	}
}

void HoSky::DestroyRes(int index)
{
	if(index < 0 || index >= (int)SkyDrawInfoList.size())
		return;
	SkyDrawInfoList[index].DestroyRes();
}


extern int DarkLight;
void HoSky::Create()
{

	FILE *fp;
	if(smConfig.ScreenColorBit == 32)
 		fp = fopen("Effect\\Sky\\test32.txt", "rt");
	else
		fp = fopen("Effect\\Sky\\test16.txt", "rt");

	if( fp == NULL)
		return;

	HoSkyDrawInfo	tempSkyDrawInfo;
	skyResInfo      tempSkyResInfo;
	char	bufferLine[256];
	char	bufferResName[256];
	char	token[256];
	while( !feof(fp) )
	{
		fgets(bufferLine, 256, fp);
		sscanf(bufferLine, "%s", token);
		if(_stricmp(token, "TestMode:") == 0)
			sscanf(bufferLine, "%s%d", token, &TestModeFlag);
		if(_stricmp(token, "StartSky:") == 0)
			sscanf(bufferLine, "%s%d", token, &DrawIndex);
		sscanf(bufferLine, "%s%s", token, token);		//�ϴ� �̸��� ������ '{' �� ���´�.

		if(_stricmp(token, "{") == 0)
		{
 			tempSkyDrawInfo.FogFlag = false;
			while(_stricmp(token, "}") != 0)
			{
				if(_stricmp(token, "LightColor:") == 0)
					sscanf(bufferLine, "%s%d%d%d", token, &tempSkyDrawInfo.LightColorR, &tempSkyDrawInfo.LightColorG, &tempSkyDrawInfo.LightColorB);
				if(_stricmp(token, "LightDark:") == 0)
					sscanf(bufferLine, "%s%d", token, &tempSkyDrawInfo.LightDark);
				if(_stricmp(token, "FileName:") == 0)
				{
					sscanf(bufferLine, "%s%s%s%d%s%d%d%d%d",
					token, bufferResName, token, &tempSkyResInfo.fadeSlowFlag, token, 
					&tempSkyResInfo.colorR,
					&tempSkyResInfo.colorG,
					&tempSkyResInfo.colorB,
 					&tempSkyResInfo.colorA);
					
					tempSkyResInfo.resName = bufferResName;
					tempSkyDrawInfo.SkyResInfoList.push_back(tempSkyResInfo);
					tempSkyResInfo.fadeSlowFlag = 0;
				}

				//���� ���� ���� �о� ���̱�....
				if(_stricmp(token, "Fog:") ==  0) 
				{
					tempSkyDrawInfo.FogFlag = true;
					sscanf(bufferLine, "%s%s%d%s%d%s%d%d%d%d", token, token
						,&tempSkyDrawInfo.FogStartPos, token, &tempSkyDrawInfo.FogEndPos, token, &tempSkyDrawInfo.FogColorR,
						&tempSkyDrawInfo.FogColorG, &tempSkyDrawInfo.FogColorB, &tempSkyDrawInfo.FogColorA);

				}
				
				
				// �Ѷ��� ó���ϱ�
				fgets(bufferLine, 256, fp);
				sscanf(bufferLine, "%s", token);
				// �ּ� ó��
				if(strlen(token) >= 10)
				{
					if(token[0] == '/' && token[1] == '/')
					{
						fgets(bufferLine, 256, fp);
						sscanf(bufferLine, "%s", token);	//������ �ѱ��.
					}
				}
			}

 			if(tempSkyDrawInfo.SkyResInfoList.size())
			{
				SkyDrawInfoList.push_back(tempSkyDrawInfo);
				tempSkyDrawInfo.FogFlag = false;
				tempSkyDrawInfo.ChangeSkyNum = 0;
				tempSkyDrawInfo.ChangeSkyTime = 0;
				tempSkyDrawInfo.ChangeNextSkyEnd = false;
				tempSkyDrawInfo.SkyResInfoList.erase(tempSkyDrawInfo.SkyResInfoList.begin(), tempSkyDrawInfo.SkyResInfoList.end());
			}

		}
	}

	if(fp)
		fclose(fp);
	
	//LoadRes();
	AddLoaderSky(&SkyDrawInfoList[DrawIndex]);
	//�� �ε��� ���� ������ ����
	CreateLoaderThread();

	if(DrawIndex != -1 && TestModeFlag)
	{
		//�̹����� ó�� ���� ����..
 		if(SkyDrawInfoList[DrawIndex].FogFlag)
		{
 			smRender.m_FogMode = 1;
			
			FogEndColorR = SkyDrawInfoList[DrawIndex].FogColorR;
			FogEndColorG = SkyDrawInfoList[DrawIndex].FogColorG;
			FogEndColorB = SkyDrawInfoList[DrawIndex].FogColorB;
			FogEndColorA = SkyDrawInfoList[DrawIndex].FogColorA;

			FogStartPos2 = SkyDrawInfoList[DrawIndex].FogStartPos/4;
 			FogEndPos2   = SkyDrawInfoList[DrawIndex].FogEndPos/4;
			
			if(FogStartPos2 > 3000/4)
				FogStartPos2 = 3000/4;

			if(FogEndPos2 > 3000/4)
				FogEndPos2 = 3000/4;

		}
		else
		{
			//���� ����� ��� ���״� ���ش�.
			if(smRender.m_FogMode)
			{
				FogStartPos2 = FogStartPos1 + 3000/4;
				FogEndPos2 = FogEndPos1 + 3000/4;
			}
		}	
		
		SetFieldColor(SkyDrawInfoList[DrawIndex].LightColorR, SkyDrawInfoList[DrawIndex].LightColorG,
		SkyDrawInfoList[DrawIndex].LightColorB, SkyDrawInfoList[DrawIndex].LightDelay);
		
		DarkLight = SkyDrawInfoList[DrawIndex].LightDark;
		DarkLevel_Fix = 0;
		TimeCount = 0;
		//LoadRes(DrawIndex);
	}
	
	FirstSkyLoadFlag = TRUE;
}


void HoSky::Init()
{
    DrawIndex = -1;
	DrawOldIndex = -1;
	DrawAlphaCount = 100;
	TimeCount = 0;
	CodeID = -1;
	FieldStartColorDelay = -1;

	FieldColorR = 0;
	FieldColorG = 0;
	FieldColorB = 0;

	//FogStartColor = 0;
	//FogEndColor = 0;
	FogStartColorR = 0;
	FogStartColorG = 0;
	FogStartColorB = 0;
	FogStartColorA = 0;

	FogEndColorR = 0;
	FogEndColorG = 0;
	FogEndColorB = 0;
	FogEndColorA = 0;

	FogStartPos1 = 1000;
	FogStartPos2 = 1000;

	FogEndPos1 = 3000;
	FogEndPos2 = 3000;

	//SkyChangeState = 0;
	//SkyChangeTimeCount = 0;
	//SkyChangeFlag = FALSE;
}


//���� ���� �ʱ�ȭ..
void HoSky::Destroy()
{
	DestroyRes();
	SkyDrawInfoList.erase(SkyDrawInfoList.begin(), SkyDrawInfoList.end());
	DrawIndex = -1;
}



unsigned int HoSky::SetDrawIndex(unsigned drawIndex, int codeID)
{
	CodeID = codeID;
 	if(drawIndex >= SkyDrawInfoList.size() && drawIndex < -1)
		return -1;

	//�̹����� ó�� ���� ����..
 	if(DrawIndex == -1)
	{
		DrawOldIndex = -1;
		DrawIndex = drawIndex;

		DrawAlphaCount = 100;
	}
	else
	{
		DrawOldIndex = DrawIndex;
		DrawIndex = drawIndex;
		
		
		DrawAlphaCount = 50;
		DrawOldAlphaCount = 100;
		
		DrawNextAlphaCount = 0;
		DrawNextOldAlphaCount = 100;
		DrawNextFlag = FALSE;
	}
	
	if(DrawIndex != -1)
 	{
		if(SkyDrawInfoList[DrawIndex].FogFlag)
		{
 			smRender.m_FogMode = 1;
			
			FogEndColorR = SkyDrawInfoList[DrawIndex].FogColorR;
			FogEndColorG = SkyDrawInfoList[DrawIndex].FogColorG;
			FogEndColorB = SkyDrawInfoList[DrawIndex].FogColorB;
			FogEndColorA = SkyDrawInfoList[DrawIndex].FogColorA;

			FogStartPos2 = SkyDrawInfoList[DrawIndex].FogStartPos/4;
 			FogEndPos2   = SkyDrawInfoList[DrawIndex].FogEndPos/4;
			
			if(FogStartPos2 > 3000/4)
				FogStartPos2 = 3000/4;

			if(FogEndPos2 > 3000/4)
				FogEndPos2 = 3000/4;

		}
		else
		{
			//���� ����� ��� ���״� ���ش�.
			if(smRender.m_FogMode)
			{
				FogStartPos2 = FogStartPos1 + 3000/4;
				FogEndPos2 = FogEndPos1 + 3000/4;
			}
		}
		//LoadRes(DrawIndex);
		AddLoaderSky(&SkyDrawInfoList[DrawIndex]);
		//�� �ε��� ���� ������ ����
		CreateLoaderThread();

	}
	
	SetFieldColor(SkyDrawInfoList[DrawIndex].LightColorR, SkyDrawInfoList[DrawIndex].LightColorG,
 		SkyDrawInfoList[DrawIndex].LightColorB, SkyDrawInfoList[DrawIndex].LightDelay);
	
	DarkLight = SkyDrawInfoList[DrawIndex].LightDark;
	DarkLevel_Fix = 0;
	TimeCount = 0;
	

	return DrawIndex;
}


unsigned int HoSky::SetNextDrawIndex()
{
    DrawOldIndex = DrawIndex;
	DrawIndex++;
	if(DrawIndex >= (int)SkyDrawInfoList.size())
		DrawIndex = 0;
	if(DrawIndex != -1)
 	{
		//LoadRes(DrawIndex);
		AddLoaderSky(&SkyDrawInfoList[DrawIndex]);
		//�� �ε��� ���� ������ ����
		CreateLoaderThread();

 		if(SkyDrawInfoList[DrawIndex].FogFlag)
		{
 			smRender.m_FogMode = 1;
			
			FogEndColorR = SkyDrawInfoList[DrawIndex].FogColorR;
			FogEndColorG = SkyDrawInfoList[DrawIndex].FogColorG;
			FogEndColorB = SkyDrawInfoList[DrawIndex].FogColorB;
			FogEndColorA = SkyDrawInfoList[DrawIndex].FogColorA;

			FogStartPos2 = SkyDrawInfoList[DrawIndex].FogStartPos/4;
 			FogEndPos2   = SkyDrawInfoList[DrawIndex].FogEndPos/4;
			
			if(FogStartPos2 > 3000/4)
				FogStartPos2 = 3000/4;

			if(FogEndPos2 > 3000/4)
				FogEndPos2 = 3000/4;

		}
		else
		{
			//���� ����� ��� ���״� ���ش�.
			if(smRender.m_FogMode)
			{
				FogStartPos2 = FogStartPos1 + 3000/4;
				FogEndPos2 = FogEndPos1 + 3000/4;
			}
		}	
	}


	DrawAlphaCount = 50;
	DrawOldAlphaCount = 100;

	DrawNextAlphaCount = 0;
	DrawNextOldAlphaCount = 100;
	DrawNextFlag = FALSE;
	
	TimeCount = 0;
	
	int r = SkyDrawInfoList[DrawIndex].LightColorR;
	int g = SkyDrawInfoList[DrawIndex].LightColorG;
	int b = SkyDrawInfoList[DrawIndex].LightColorB;
	
 	SetFieldColor(SkyDrawInfoList[DrawIndex].LightColorR, SkyDrawInfoList[DrawIndex].LightColorG,
		SkyDrawInfoList[DrawIndex].LightColorB, SkyDrawInfoList[DrawIndex].LightDelay);
	
	DarkLight = SkyDrawInfoList[DrawIndex].LightDark;
	
	return DrawIndex;
}

int InitSky()
{
	SkyBackGround.Init();
	return TRUE;
}

int CreateSky()
{
	if(SkyBackGround.FirstSkyLoadFlag == FALSE)
	{
		SkyBackGround.Init();
		SkyBackGround.Destroy();
		SkyBackGround.Create();
	}
	
	return TRUE;
}

int ReCreateSky()
{
	SkyBackGround.Init();
	SkyBackGround.Destroy();
	SkyBackGround.Create();
	
	return TRUE;
}

int DrawSky(int x, int y, int z, int ax, int ay, int az) //eCAMERA_TRACE *cameraTrace)
{
	
	int OnStageField;
	if ( !lpCurPlayer ) 
		OnStageField=0;
	else 
		OnStageField = lpCurPlayer->OnStageField;

	if(StageField[OnStageField] != NULL)
	{
		if( StageField[OnStageField]->State != FIELD_STATE_DUNGEON && StageField[OnStageField]->State!=FIELD_STATE_ROOM)
		SkyBackGround.Draw(x, y, z, ax, ay, az);
	}
	return TRUE;
}

int DrawOpeningSky(int x, int y, int z, int ax, int ay, int az)
{
	SkyBackGround.Draw(x, y, z, ax, ay, az);
	return TRUE;
}


//static BOOL DungeonInFlag = FALSE;
//static int  DungeonDarkLightFieldCode = 0;

int MainSky()
{
  	SkyBackGround.Main();
	

	int OnStageField;
	if ( !lpCurPlayer ) 
		OnStageField=0;
	else 
		OnStageField = lpCurPlayer->OnStageField;

	//������ ��� �ʵ� ��� Į�� ���� �ٷ� �ٲ۴�.
	if ( StageField[OnStageField] && 
	   ( StageField[OnStageField]->State==FIELD_STATE_DUNGEON ||
 		 StageField[OnStageField]->State==FIELD_STATE_ROOM))
	{
		int codeID = StageField[OnStageField]->FieldCode+USER_SKY_ADD_CODE;
		if(SkyBackGround.GetCodeID() != codeID)
		{
			smRender.m_FogMode = 0;
			SkyBackGround.SetCodeID(codeID);
			
			
			DarkLevel_Fix = 0;
			BackColor_R = 0;
			BackColor_G = 0;
			BackColor_B = 0;

			BackColor_mR = 0;
			BackColor_mG = 0;
			BackColor_mB = 0;
				
			DarkLevel = 110;
			DarkLight = 110;
			
			switch( StageField[OnStageField]->FieldCode ) {
			case 24: //���� ����
  				BackColor_R = 0;
				BackColor_G = 10;
				BackColor_B = 40;

 		 		BackColor_mR = 0;
				BackColor_mG = 10;
				BackColor_mB = 40;
				
				DarkLevel = 60;
				DarkLight = 60;
				break;

			case 25: //���� ����
				
  	  			BackColor_R = 10;
				BackColor_G = 0;
				BackColor_B = -20;

				BackColor_mR = 10;
				BackColor_mG = 0;
				BackColor_mB = -20; 
				
				DarkLevel = 60;
				DarkLight = 60;

				break;

			case 26: //����� ����
				
 				BackColor_R = 0;
				BackColor_G = 10;
				BackColor_B = 40;

				BackColor_mR = 0;
				BackColor_mG = 10;
				BackColor_mB = 40; 

				DarkLevel = 40;
				DarkLight = 40;

				break;

			case 22: //����1
			case 23: //����2
				BackColor_R = 0;
				BackColor_G = 0;
				BackColor_B = 0;

				BackColor_mR = 0;
				BackColor_mG = 0;
				BackColor_mB = 0;
				
 				DarkLevel = 40;
				DarkLight = 40;
				break;
			}
		}
	}
	return TRUE;
}

int DestroySky()
{
	SkyBackGround.Destroy();
	return TRUE;
}

int ChangeSky(int codeID)
{
 	if(SkyBackGround.TestModeFlag == FALSE)
	{
		if(codeID == FIELD_BACKIMAGE_NIGHTIRON1 ||
	 	   codeID == FIELD_BACKIMAGE_NIGHTIRON2)
		{
			if(dwGameHour >= 1 && dwGameHour < 3)
				codeID = FIELD_BACKIMAGE_AURORA;
		}

		if(SkyBackGround.GetCodeID() != codeID) //�ʵ忡 ��ȭ�� ������ ��츸 ���ϰ� �Ѵ�.
		{
			//������ ���� ���ڱ� �ʵ尡 ���ϹǷ� ������ ������ �ٲ۴�..
			//���ӿ��� �Ѿ���� �ϴ� ���� ���� 
   	  		if(codeID == FIELD_BACKIMAGE_RUIN1)
				SkyBackGround.SetDrawIndex(0, codeID);   //0�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_RUIN2)
				SkyBackGround.SetDrawIndex(1, codeID);   //1�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_GLOWRUIN1)
				SkyBackGround.SetDrawIndex(2, codeID);   //2�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_GLOWRUIN2)
				SkyBackGround.SetDrawIndex(2, codeID);   //2�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_NIGHTRUIN1)
				SkyBackGround.SetDrawIndex(3, codeID);	 //3�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_NIGHTRUIN2)
				SkyBackGround.SetDrawIndex(3, codeID);   //3�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_DAY)
				SkyBackGround.SetDrawIndex(4, codeID);   //4�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_GLOWDAY)
				SkyBackGround.SetDrawIndex(5, codeID);   //5�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_NIGHT)
				SkyBackGround.SetDrawIndex(6, codeID);   //6�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_DESERT)
				SkyBackGround.SetDrawIndex(8, codeID);   //8�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_GLOWDESERT)
				SkyBackGround.SetDrawIndex(9, codeID);   //9�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_NIGHTDESERT)
				SkyBackGround.SetDrawIndex(10, codeID);   //10�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_DAYFALL)
				SkyBackGround.SetDrawIndex(11, codeID);   //11�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_GLOWFALL)
				SkyBackGround.SetDrawIndex(12, codeID);   //12�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_NIGHTFALL)
				SkyBackGround.SetDrawIndex(13, codeID);   //13�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_DAYIRON)
				SkyBackGround.SetDrawIndex(14, codeID);   //14�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_GLOWIRON)
				SkyBackGround.SetDrawIndex(15, codeID);   //15�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_NIGHTIRON1 || codeID == FIELD_BACKIMAGE_NIGHTIRON2)
				SkyBackGround.SetDrawIndex(16, codeID);   //16�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_AURORA)
				SkyBackGround.SetDrawIndex(17, codeID);   //17�� �ϴ�
			else if(codeID == FIELD_BACKIMAGE_RAIN)
			{
				int OnStageField;
				if ( !lpCurPlayer ) 
					OnStageField=0;
				else 
					OnStageField = lpCurPlayer->OnStageField;
				
				if(StageField[OnStageField] != NULL) 
				{
					//������ �ƴ� ��쿡�� �� ���� �Ѵ�.
					if( StageField[OnStageField]->State != FIELD_STATE_DUNGEON && StageField[OnStageField]->State!=FIELD_STATE_ROOM)
						SkyBackGround.SetDrawIndex(7, codeID);   //1�� ��ö�..
				}
				else//NULL�� ���� �ʵ带 ó�� ������ �̴�..
					SkyBackGround.SetDrawIndex(7, codeID);   //1�� ��ö�..
			}
		}
	}

	return TRUE;
}

int SetNextSky() 
{
    unsigned int drawIndex = SkyBackGround.SetNextDrawIndex();
	return TRUE;
}
