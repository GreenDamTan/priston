/*----------------------------------------------------------------------------*
*	���ϸ� :  Morayion.cpp	
*	�ϴ��� :  ����̿��� ��ų
*	�ۼ��� :  ����������Ʈ 9��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

#include "..\\sinLinkHeader.h"

/*----------------------------------------------------------------------------*
*						�⺻ ���� �Լ� 	
*-----------------------------------------------------------------------------*/	
//��ų�� ����Ҽ��ִ����� üũ�Ѵ� 
DWORD sinNotToggleSkill_CODE[][5] = {
	{SKILL_HOLY_BODY,SKILL_HOLY_VALOR,SKILL_DRASTIC_SPIRIT,0},
	{SKILL_ZENITH,0},
	{SKILL_WINDY,0},
	{SKILL_HOLY_REFLECTION,0},
	{SKILL_SPIRIT_ELEMENTAL,0},
	{SKILL_DANCING_SWORD,0},
	{SKILL_CONCENTRATION,0},
	{SKILL_SWIFT_AXE,0},
	{SKILL_FALCON,0},
	{SKILL_GOLDEN_FALCON,0},
	{SKILL_SCOUT_HAWK,0},
	{SKILL_DIVINE_INHALATION,0},
	{SKILL_METAL_ARMOR,0},
	{SKILL_SPARK_SHIELD,0},
	{SKILL_VANISH,0},
	{SKILL_GODLY_SHIELD,0},
	{SKILL_GOD_BLESS,0},
	{SKILL_FROST_JAVELIN,0},
	{SKILL_REGENERATION_FIELD,0},
	{SKILL_SUMMON_MUSPELL,0},
	{SKILL_FIRE_ELEMENTAL,0},
	{SKILL_DISTORTION,0},
//	{SKILL_COMPULSION,0}, // �庰 - ��ų�뷱�� ����(10.08.10)
	{SKILL_MAGNETIC_SPHERE,0},
	{SKILL_METAL_GOLEM,0},
	{SKILL_BERSERKER,0},
	{SKILL_ASSASSIN_EYE,0},
	{SKILL_VAGUE,0},
	{SKILL_RECALL_WOLVERIN,0},
	//������ ������ ��ų
	{SCROLL_INVULNERABILITY,0},
	{SCROLL_CRITICAL,0},
	{SCROLL_EVASION,0},
	{STONE_R_FIRECRYTAL,0},
	{STONE_R_ICECRYTAL,0},
	{STONE_R_LINGHTINGCRYTAL,0},
	{STONE_A_FIGHTER,0},
	{STONE_A_MECHANICIAN,0},
	{STONE_A_PIKEMAN,0},
	{STONE_A_ARCHER,0},
	{STONE_A_KNIGHT,0},
	{STONE_A_ATALANTA,0},
	{STONE_A_MAGICIAN,0},
	{STONE_A_PRIESTESS,0},
	//{SKILL_TRIUMPH_OF_VALHALLA,SKILL_HALL_OF_VALHALLA,0},
	0,    //0�� ������������ ���ѷ����� �����ִ�
};

int sinCheckSkillUseOk(sSKILL *lpSkill)
{ 

	ContinueSkillCancelFlag = 0; //�̰����� �ʱ�ȭ���ش� 
	int i = 0;

	//SkillInfo ����Ÿ�� �˻��Ѵ� 
	if(!cSkill.CheckCharSkillInfo())return FALSE;  

	#define  CHAR_SKILL_MASK 0xFF000000
	//�ڽ��� ��ų�� �´����� üũ�Ѵ� 
	//DRZ_EDIT (add new class handling here)
	int   CharCode[8] = { JOBCODE_MECHANICIAN, JOBCODE_FIGHTER, JOBCODE_ARCHER, JOBCODE_PIKEMAN, JOBCODE_KNIGHT, JOBCODE_ATALANTA, JOBCODE_PRIESTESS, JOBCODE_MAGICIAN };
	DWORD CharSkillCode[8] = {GROUP_MECHANICIAN,GROUP_FIGHTER ,  GROUP_ARCHER,GROUP_PIKEMAN,GROUP_KNIGHT,GROUP_ATALANTA,GROUP_PRIESTESS,GROUP_MAGICIAN};

	for(int j= 0 ; j < 8 ; j++){ 
		if(CharCode[j] == sinChar->JOB_CODE){
			if((lpSkill->CODE & CHAR_SKILL_MASK) !=CharSkillCode[j] ){
				return FALSE;
			}
		}
	}


	/*
	//������ ���׹̳��� �ѹ��� üũ�Ѵ� // �庰-���� ���ེ ���� ���
	if(lpSkill->Skill_Info.UseMana)
	{ 
		if( chaPremiumitem.m_ManaReducePotiontime > 0 )
		{
			if(sinGetMana() >= lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] - (lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) && 
				sinGetStamina() >= lpSkill->Skill_Info.UseStamina[0] + (lpSkill->Skill_Info.UseStamina[1]*(lpSkill->Point-1)))
			{
				return TRUE;
			}
			else
				return FALSE;
		}
		else
		{
			if((sinGetMana() - lpSkill->Skill_Info.UseMana[lpSkill->Point - 1]) < 0)
			return FALSE;
		}
	}

	if(lpSkill->Skill_Info.UseStamina[0]){
		if((sinGetStamina() - (lpSkill->Skill_Info.UseStamina[0] + (lpSkill->Skill_Info.UseStamina[1]*(lpSkill->Point-1)))) < 0 )
			return FALSE;

	}
	*/


	/*
	 //������ ���׹̳��� �ѹ��� üũ�Ѵ� // �庰-���� ���ེ ���� ���
	if(lpSkill->Skill_Info.UseMana)
	{ 
		if(chaPremiumitem.m_ManaReducePotiontime > 0) // ����� - ���� ���ེ ������� ��� ��ų ��밡���� ������ �Բ� ���δ�.
		{
			if(sinGetMana() - ( lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] - (lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) ) < 0)
				return FALSE;
		}
		else
		{
			if((sinGetMana() - lpSkill->Skill_Info.UseMana[lpSkill->Point - 1]) < 0)
				return FALSE;	    
		}  
	}
	*/

	// �庰 - �׸� �Ӽ�
	if(chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1) // ����� - ���� ���ེ ������� ��� ��ų ��밡���� ������ �Բ� ���δ�.
	{
		if(sinGetMana() - ( lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] - ((lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) + 
			(lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] * 20 / 100))) < 0)
			return FALSE;
	}

	else if( chaPremiumitem.m_ManaReducePotiontime > 0 )
	{
		if(sinGetMana() - ( lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] - (lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) ) < 0)
			return FALSE;
	}

	else if( chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		if(sinGetMana() - ( lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] - (lpSkill->Skill_Info.UseMana[lpSkill->Point - 1] * 20 / 100) ) < 0)
			return FALSE;
	}

	else
	{
		if((sinGetMana() - lpSkill->Skill_Info.UseMana[lpSkill->Point - 1]) < 0)
			return FALSE;	    
	}  

	//��۵Ǹ� �ȵǴ� ��ų�� üũ���ش� 
	int CntSkillNum = 0;  //��ų�� ī��Ʈ ���ش�
	int k = 0;
	
	int Index3 = 0; //�����������ó 
	if(Index3 = cSkill.SearchContiueSkillIndex(SKILL_FORCE_OF_NATURE)){
		if(ContinueSkill[Index3-1].Flag == 1 && lpSkill->CODE == SKILL_FORCE_OF_NATURE)
			return FALSE;
	}

	if(Index3 = cSkill.SearchContiueSkillIndex(SKILL_HALL_OF_VALHALLA)){
		if(ContinueSkill[Index3-1].Flag == 1 && lpSkill->CODE == SKILL_HALL_OF_VALHALLA)
			return FALSE;
	}

	while(1){
		if(!sinNotToggleSkill_CODE[CntSkillNum][0])break;    //������ ��ų�� �ƴ϶�� while���� �������´�.
		for(i = 0 ; 0 != sinNotToggleSkill_CODE[CntSkillNum][i] ; i++){
            if(sinNotToggleSkill_CODE[CntSkillNum][i] == lpSkill->CODE ){ //���� ���� ��������ų�� ã�´�.
				//���� ����ϸ� �ȵŴ� ��ų�� üũ���ش�.
				for(k = 0 ; 0 != sinNotToggleSkill_CODE[CntSkillNum][k] ; k++){ 
					for( int j = 0 ; j < MAX_CONTINUE_SKILL ; j++){   
						if(ContinueSkill[j].Flag && ContinueSkill[j].CODE == sinNotToggleSkill_CODE[CntSkillNum][k]){
							return FALSE;
						}
					}
				}
			}
		}
		CntSkillNum++;  
	}


	int UseFlag  = 0; 
	int UseFlag2 = 0;

	if(lpSkill->Skill_Info.UseWeaponCode[0]){
		UseFlag = 1;
		for(int i=0;i<8;i++){
			if(lpSkill->Skill_Info.UseWeaponCode[i]){
				if(lpSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE&sinITEM_MASK2)){ //
					UseFlag2 = 1;
					break;
				}
			}
		}
	}

	//��Ż�Ƹ� ��Ư�� ����Ҽ�����
	if(lpSkill->CODE == SKILL_METAL_ARMOR){
		if(sInven[2].ItemIndex){
			if(cInvenTory.InvenItem[sInven[2].ItemIndex-1].sItemInfo.JobCodeMask == sinChar->JobBitMask){
				if(lpSkill->UseSkillFlag){
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	if(UseFlag){
		if(UseFlag2 && lpSkill->UseSkillFlag)
			return TRUE;
	}
	else{
		if(lpSkill->UseSkillFlag)
			return TRUE;
	}

	//////////// ��Ʋ��Ÿ ���д����� , ���� ��Ʈ����ũ
	if(lpSkill->CODE == SKILL_SHIELD_STRIKE || lpSkill->CODE == SKILL_EXTREME_SHIELD || lpSkill->CODE == SKILL_DIVINE_INHALATION || lpSkill->CODE == SKILL_SPARK_SHIELD || lpSkill->CODE == SKILL_GODLY_SHIELD){
		if(lpSkill->UseSkillFlag){
			if(lpSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE&sinITEM_MASK2)){ //
				return TRUE;
			}
		}
	}

	/////////// �ҿＮĿ (�ͽ����� ���п� ����¡�� ���и�����Ҽ��ִ�)
	if(lpSkill->CODE == SKILL_SOUL_SUCKER){ //�ͽ�ó�� ����¡ �������� �ƴѰ��� ����Ҽ����� 
		if(cInvenTory.InvenItem[sInven[1].ItemIndex-1].sItemInfo.ItemKindCode == ITEM_KIND_CRAFT ||
			cInvenTory.InvenItem[sInven[1].ItemIndex-1].sItemInfo.ItemKindCode == ITEM_KIND_AGING ){
				if(lpSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE&sinITEM_MASK2)){ //
					if(lpSkill->UseSkillFlag){	//<============== ����
						return TRUE;
					}
				}
			}
	
	}
	return FALSE;
}

//����� ��ų�� ��°� ������ �Ҹ��Ѵ�
int sinUseManaStamina(sSKILL *lpSkill)
{
	/*
	if( chaPremiumitem.m_ManaReducePotiontime <= 0 )	// pluto ���� ���ེ ����
	{
		if(lpSkill->Skill_Info.UseMana){
			sinSetMana(sinGetMana() - lpSkill->Skill_Info.UseMana[lpSkill->Point - 1]);
		}
	}
	else
	{
		if( lpSkill->Skill_Info.UseMana )
		{
			sinSetMana( sinGetMana() - ( lpSkill->Skill_Info.UseMana[ lpSkill->Point - 1] - (lpSkill->Skill_Info.UseMana[ lpSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) )  );
		}
	}
	*/

	// �庰 - �׸� �Ӽ�
	if( chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0  && sinChar->GravityScroolCheck[1] == 1)
	{
		sinSetMana( sinGetMana() - ( lpSkill->Skill_Info.UseMana[ lpSkill->Point - 1] - 
			((lpSkill->Skill_Info.UseMana[ lpSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) +
			(lpSkill->Skill_Info.UseMana[ lpSkill->Point - 1] * 20 / 100)) )  );
	}

	else if( chaPremiumitem.m_ManaReducePotiontime > 0 )
	{
		sinSetMana( sinGetMana() - ( lpSkill->Skill_Info.UseMana[ lpSkill->Point - 1] - (lpSkill->Skill_Info.UseMana[ lpSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) )  );
	}

	else if( chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1 )
	{
		sinSetMana( sinGetMana() - ( lpSkill->Skill_Info.UseMana[ lpSkill->Point - 1] - (lpSkill->Skill_Info.UseMana[ lpSkill->Point - 1] * 20 / 100) )  );
	}

	else
	{
		if(lpSkill->Skill_Info.UseMana)
			sinSetMana(sinGetMana() - lpSkill->Skill_Info.UseMana[lpSkill->Point - 1]);
	}


	// ����� - �ٷ� ���ེ ����
	if(lpSkill->Skill_Info.UseStamina[0])
	{
		if( chaPremiumitem.m_StaminaReducePotiontime <= 0 )
		{
			sinSetStamina(sinGetStamina() - (lpSkill->Skill_Info.UseStamina[0] + (lpSkill->Skill_Info.UseStamina[1]*(lpSkill->Point-1))));
		}
		else
		{
			sinSetStamina(sinGetStamina() - ( (lpSkill->Skill_Info.UseStamina[0] + (lpSkill->Skill_Info.UseStamina[1]*(lpSkill->Point-1)))
				- ( (lpSkill->Skill_Info.UseStamina[0] + (lpSkill->Skill_Info.UseStamina[1]*(lpSkill->Point-1))) * chaPremiumitem.m_StaminaReducePotionValue / 100)) );
		}
	}

	cSkill.CheckSkillMasteryForm(lpSkill); // ��ų �����͸��� üũ�Ѵ�
	lpSkill->Mastery =0; //����� �����̰� �ʱ�ȭ 
	cSkill.ReformSkillMasteryForm(lpSkill);

	
	lpSkill->UseSKillIncreCount++;
	if(lpSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[lpSkill->Skill_Info.SkillNum]+((lpSkill->Point-1)/3)){
		lpSkill->UseSKillIncreCount = 0;
		lpSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

	}

	return TRUE;
}

//���������� ���� ��ų�� �����Ѵ� 
int sinContinueSkillSet(sSKILL *lpSkill)
{
	int i = 0;
	//������ ���常 ����ó��(�ؿ�)
	if(lpSkill->CODE == SKILL_ENERGY_SHIELD|| lpSkill->CODE == SKILL_COMPULSION) // �庰 - ��ų�뷱�� ����(10.08.10)
	{
		for( i = 0 ; i < MAX_CONTINUE_SKILL ; i++)
		{
			if(ContinueSkill[i].Flag )
			{
				if(ContinueSkill[i].CODE == SKILL_ENERGY_SHIELD || ContinueSkill[i].CODE == SKILL_COMPULSION) // �庰 - ��ų�뷱�� ����(10.08.10)
				{
					memset(&ContinueSkill[i],0,sizeof(sSKILL));
					break;
				}
			}
		}
	}

	for(i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(!ContinueSkill[i].Flag){
			memcpy(&ContinueSkill[i],lpSkill,sizeof(sSKILL));
			ContinueSkill[i].Flag = 1;
			cSkill.SearchSkillDoc(&ContinueSkill[i]);
			if((lpSkill->CODE != SKILL_FORCE_ORB && lpSkill->CODE != CHANGE_JOB3_QUEST &&lpSkill->CODE != CHANGE_ELEMENTARY_QUEST &&
				lpSkill->CODE != BOOSTER_ITEM_LIFE && lpSkill->CODE != BOOSTER_ITEM_MANA && lpSkill->CODE != BOOSTER_ITEM_STAMINA))  // ����� - �ν��� ������
				ContinueSkill[i].MatIcon = 0;//�ʱ�ȭ�ѹ������ش�
			break;

		}
	}

	return TRUE;
}
//�ð��� ������ ���ӻ�뽺ų������ �ʱ�ȭ�Ѵ�
int sinCheckContinueSkill()
{
	//�۽�ũ�� cpp���� üũ�ϰ�����
	/*
	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(ContinueSkill[i].Flag){
			ContinueSkill[i].Time[0]++;
			if(ContinueSkill[i].Time[0] >= ContinueSkill[i].Time[1]*70){
				memset(&ContinueSkill[i],0,sizeof(sSKILL));
				cInvenTory.SetItemToChar(); //������ �����Ѵ� 
			}
		}
	}
	*/
	return TRUE;
}
/*----------------------------------------------------------------------------*
*						��ų�� ����Ѵ� 
*-----------------------------------------------------------------------------*/	
int sinSwitchSkill(sSKILL *pSkill)
{
	/* ��Ƽ�� ��ų������ ��ų�� ����ġ�����ʴ´� 
	int i = 0;
	switch(pSkill->CODE){
		case SKILL_HOLY_VALOR:
			for(i = 0; i < 10 ; i++){
				if(ContinueSkill[i].CODE == SKILL_HOLY_BODY || ContinueSkill[i].CODE == SKILL_DRASTIC_SPIRIT){
					memset(&ContinueSkill[i],0,sizeof(sSKILL));
					cInvenTory.SetItemToChar();

				}
			}
		break;
		case SKILL_HOLY_BODY:
			for(i = 0; i < 10 ; i++){
				if(ContinueSkill[i].CODE == SKILL_HOLY_VALOR || ContinueSkill[i].CODE == SKILL_DRASTIC_SPIRIT){
					memset(&ContinueSkill[i],0,sizeof(sSKILL));
					cInvenTory.SetItemToChar();
				}
			}
		break;
		case SKILL_DRASTIC_SPIRIT:
			for(i = 0; i < 10 ; i++){
				if(ContinueSkill[i].CODE == SKILL_HOLY_VALOR || ContinueSkill[i].CODE == SKILL_DRASTIC_SPIRIT){
					memset(&ContinueSkill[i],0,sizeof(sSKILL));
					cInvenTory.SetItemToChar();
				}
			}
		break;

	}
	*/

	return TRUE;
}

/*----------------------------------------------------------------------------*
*						����̿� ��ų�Լ� 
*-----------------------------------------------------------------------------*/	

//////////////// ����Ʈ
int F_Sword_Blast()
{
	//���Ͼ���	
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_HolyBody()	
{
	pUseSkill->UseTime = HolyBody_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	sinSwitchSkill(pUseSkill);
	return TRUE;
}
int F_Physical_Traning()
{
	//���Ͼ���
	return TRUE;
}
int F_Double_Crash()
{
	//���Ͼ���
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}

/////////// ��Ʋ��Ÿ
int F_S_Strike()
{
	//���Ͼ���
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}
int F_Farina()
{
	//���� ����
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}
int F_D_Mastery()
{
	//����~~ ���� ���Ͼ���
	
	return TRUE;
}
int F_V_Spear()
{
	//���ƾƾ� ���� ������ ������ ��.�� 
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}

/////////// ������Ƽ��
int F_Healing()
{
	//���� ���� ���Ͼ�����...
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}
int F_HolyBolt()
{
	//���� �� ���϶�
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}
int F_M_Spark()
{
	//���� ���� �����࿩...
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}
int F_HolyMind()
{
	//���� ���� ������ ������...
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}

/////////// ������
int F_Agony()
{
	short TempLife,TempLife2,TempMana;
	sinUseManaStamina(pUseSkill);	
	TempLife = (short)(((float)sinChar->Life[1]/100.0f)*(float)Agony_ConvLife[pUseSkill->Point-1]);
	TempMana = (short)(((float)sinChar->Mana[1]/100.0f)*(float)Agony_ConvLife[pUseSkill->Point-1]);
	if((sinGetLife()-TempLife) > 0){
		TempLife2 = sinGetLife()-TempLife;
		sinSetLife(TempLife2);
		TempMana = sinGetMana()+TempMana;
		sinSetMana(TempMana);
	
	};
	
	//
	//sinSetLife

	return TRUE;
}
int F_FireBolt()
{
	//�޷�
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}
int F_Zenith()
{
	//pUseSkill->Time[1] = Zenith_Time[pUseSkill->Point-1];
	pUseSkill->UseTime = Zenith_Time[pUseSkill->Point-1]; 
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);	
	cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	return TRUE;
}
int F_ZenithFromServer(int Time) //��Ƽ���� �����Ű�����ؼ� �������� �޾ƿ´� 
{
	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(!ContinueSkill[i].Flag){
			ContinueSkill[i].Flag = 1;
			ContinueSkill[i].CODE = SKILL_ZENITH;
			ContinueSkill[i].Time[1] = Time;
			ContinueSkill[i].PartyFlag = 1;
			break;

		}
	}
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}

int F_FireBall()
{
	//����̿� ��!
	sinUseManaStamina(pUseSkill);	
	return TRUE;
}
/*----------------------------------------------------------------------------*
*						����̿� 2����ų�Լ� 
*-----------------------------------------------------------------------------*/	
//����Ʈ
int F_Holy_Valor()
{

	pUseSkill->UseTime = Holy_Valor_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	sinSwitchSkill(pUseSkill);
	return TRUE;
}
/* ��ô�� 
int F_Holy_Valor_Party() //��Ƽ�� ���� 
{
	pUseSkill->UseTime = Holy_Valor_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	sinSwitchSkill(pUseSkill);
	return TRUE;
}
*/
int F_Brandish()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Piercing()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

int F_Drastic_Spirit()
{
	pUseSkill->UseTime = Drastic_Spirit_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	sinSwitchSkill(pUseSkill);
	cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	return TRUE;
}

int F_Windy()
{
	
	pUseSkill->UseTime = Windy_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill); 
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	return TRUE;
}

int F_Twist_Javelin()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

int F_Soul_Sucker()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

int F_Fire_Javelin()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}


int F_Meditation()
{
	return TRUE;
}

int F_Divine_Lightning()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

int F_Holy_Reflection()
{
	pUseSkill->UseTime = Holy_Reflection_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

int F_Grand_Healing()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

int F_Mental_Mastery()
{
	return TRUE;
}

int F_Watornado()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

int F_Enchant_Weapon()
{

	//pUseSkill->UseTime = Enchant_Weapon_Time[pUseSkill->Point-1];
	//sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

int F_Dead_Ray()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

/*----------------------------------------------------------------------------*
*						����̿� 3����ų�Լ� 
*-----------------------------------------------------------------------------*/	
//����Ʈ
int F_S_Mastery()
{
	
	return TRUE;
}

int F_D_Inhalation()
{
	pUseSkill->UseTime = D_Inhalation_Time[pUseSkill->Point-1]; 
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	SwitchSkill(pUseSkill);
	cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	return TRUE; 
}
int F_Holy_Incantation()
{
	sinUseManaStamina(pUseSkill); 
	return TRUE;
}
int F_Grand_Cross()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;

}

//��Ż��Ÿ
int F_Split_Javelin()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_T_Of_Valhalla()
{
	//pUseSkill->UseTime = T_Of_Valhalla_Time[pUseSkill->Point-1];
	//sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	//sinSwitchSkill(pUseSkill); //���� ������ ����ġ�Ұ� ���� 
	//cInvenTory.SetItemToChar(); //������ �����Ѵ� 

	if(!pUseSkill->SkillTaget_CODE)
		SetT_Of_ValhallaFlag2 = 1;


	return TRUE;
}
int F_Lightning_Javelin()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Storm_Javelin()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

//������Ƽ��
int F_Vigor_Ball()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Resurrection()
{
	sinUseManaStamina(pUseSkill); //
	return TRUE;
}
int F_Extinction()
{
	sinUseManaStamina(pUseSkill); //
	return TRUE;
}
int F_Virtual_Life() //����� �����ϼ�
{
	//pUseSkill->UseTime = Virtual_Life_Time[pUseSkill->Point-1];
	//sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill); //Set�Լ����� ���ش�
	//sinSwitchSkill(pUseSkill); 
	//cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	return TRUE;
}

//������
int F_Energy_Shield()
{
	pUseSkill->UseTime = Energy_Shield_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Diastrophism()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Spirit_Elemental()
{
	pUseSkill->UseTime = Spirit_Elemental_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	return TRUE;
}
int F_D_Sword()
{
	pUseSkill->UseTime = Dancing_Sword_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	return TRUE;
}
/*----------------------------------------------------------------------------*
*						����̿� 4����ų�Լ� 
*-----------------------------------------------------------------------------*/
//����Ʈ
int F_Divine_Piercing()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Godly_Shield()
{
	pUseSkill->UseTime = Godly_Shield_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	SwitchSkill(pUseSkill);
	return TRUE;
}
int F_God_Bless()
{
	pUseSkill->UseTime = God_Bless_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	return TRUE;
}
int F_Sword_Of_Justice()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

//��Ʋ��Ÿ
int F_Hall_Of_Valhalla()
{
	//��ô��
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_X_Rage()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Frost_Javelin()
{
	pUseSkill->UseTime = Frost_Javelin_Time[pUseSkill->Point-1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	return TRUE;
}
int F_Vengeance()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

//������Ƽ��
int F_Glacial_Spike()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Regeneration_Field()
{
	//pUseSkill->UseTime = Regeneration_Field_Time[pUseSkill->Point-1];
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Chain_Lightning()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Summon_Muspell()
{
	pUseSkill->UseTime = Summon_Muspell_Time[pUseSkill->Point-1];
	sinUseManaStamina(pUseSkill); //��ȯ���� �����ؾ����� ���
	sinContinueSkillSet(pUseSkill);
	return TRUE;
}

//������
int F_Fire_Elemental() //���� �����غ�����
{
	pUseSkill->UseTime = 6*60;;
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	SummonSkillCodeIndex[1] = 0;
	return TRUE;
}
int F_Flame_Wave()
{
	sinUseManaStamina(pUseSkill); //��ȯ���� �����ؾ����� ���
	return TRUE;
}
int F_Distortion()
{
	pUseSkill->UseTime = Distortion_Time[pUseSkill->Point-1];
	//sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	//cInvenTory.SetItemToChar(); //������ �����Ѵ� 
	
	return TRUE;
}
int F_Meteo()
{
	sinUseManaStamina(pUseSkill); //��ȯ���� �����ؾ����� ���
	return TRUE;
}

