/*----------------------------------------------------------------------------*
*	���ϸ� :  sinAssaSkillEffect
*	�ϴ��� :  Assa ��ų ����Ʈ
*	�ۼ��� :  ����������Ʈ 2002�� 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

int sinAssaSkillEffectKeyDown(); //��ų����Ʈ �׽�Ʈ 

int sinAssaSkillEffectProcess(cASSAEFFECT *pEffect);

/*----------------------------------------------------------------------------*
*						  Assa ��ų ����Ʈ 
*-----------------------------------------------------------------------------*/	
////////����Ʈ 
int sinSkillEffect_Brandish(smCHAR *pChar);  
int sinSkillEffect_Piercing(smCHAR *pChar);
int sinSkillEffect_Piercing_Attack(smCHAR *pChar);
int sinSkillEffect_DrasticSpirit(smCHAR *pChar);

////////������
int sinSkillEffect_Energy_Shelter(smCHAR *pChar);

////////������Ƽ��
int sinSkillEffect_Great_Healing(smCHAR *pChar);
int sinSkillEffect_Great_Healing_Party(smCHAR *pChar);
int sinSkillEffect_Holy_Reflection(smCHAR *pChar , int Time);
int sinSkillEffect_Holy_Reflection_Defense(smCHAR *pChar);
// 1�� ��ų ����
int sinSkillEffect_HolyMind(smCHAR *pChar,int Time);

////////��Ż��Ÿ
int sinSkillEffect_Soul_Sucker(smCHAR *pChar , smCHAR *DesChar);
//int sinSkillEffect_Twist_Javelin(smCHAR *pChar , smCHAR *DesChar);
int sinSkillEffect_Windy(smCHAR *pChar);
int sinSkillEffect_Windy_Gather(smCHAR *pChar);

/*----------------------------------------------------------------------------*
*						        Extern 
*-----------------------------------------------------------------------------*/	

