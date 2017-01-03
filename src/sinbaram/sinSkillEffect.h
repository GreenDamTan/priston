/*----------------------------------------------------------------------------*
*	���ϸ� :  sinSkillEffect.h	
*	�ϴ��� :  ����Ʈ ���� �۾� (������ ����Ʈ )
*	�ۼ��� :  ����������Ʈ 2002�� 6��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	

int sinSkillTestKey(); //��ų �׽�Ʈ Key

/*----------------------------------------------------------------------------*
*							�ý���
*-----------------------------------------------------------------------------*/	
void sinInitSkillEffect();
void sinSkillEffectMove(cSINEFFECT2 *pEffect);	//Move
void sinProcess7();
extern int Index2;
/*----------------------------------------------------------------------------*
*						����̿� ��ų 
*-----------------------------------------------------------------------------*/	
//////////// ����Ʈ
int sinEffect_Sword_Blast_Particle(POINT3D *pPosi);
int sinEffect_Sword_Blast(smCHAR *pChar , smCHAR *desChar);
int sinEffect_Holy_Body(smCHAR *pChar);
int sinEffect_Holy_Body_Cross(smCHAR *pChar );
int sinEffect_Holy_Body_Particle(POINT3D *pPosi);	


//////////// ��Ʋ��Ÿ 
int sinEffect_Farina(smCHAR *pChar , smCHAR *desChar);
int sinEffect_Farina_Particle(POINT3D *pPosi);
int sinEffect_VigorSpear(smCHAR *pChar , smCHAR *desChar);
int sinEffect_VigorSpear_Particle(POINT3D *pPosi);
int sinEffect_Shield_Strike(POINT3D *pPosi);
int sinEffect_Shield_Strike_Particle(POINT3D *pPosi);

//////////// ������Ʈ 
//int MultiSpark_Effect(smCHAR *pChar , smCHAR *desChar);
int sinEffect_MultiSpark_Particle(POINT3D *pPosi);
int sinEffect_MultiSpark_Particle2(POINT3D *pPosi);
int sinEffect_MultiSpark(smCHAR *pChar , smCHAR *desChar , int Num);
int sinEffect_HolyMind(smCHAR *pChar , int Time);
int sinEffect_HolyMind_Particle(POINT3D *pPosi);
int sinEffect_Healing(smCHAR *pChar);
int sinEffect_Healing_Particle(POINT3D *pPosi);
int sinEffect_Healing_Particle2(POINT3D *pPosi);
int sinEffect_HealParticle3(POINT3D *pPosi ,int Mat ,int Num , int Size , int Time , int Gravity );

//�׽�Ʈ 
int sinEffect_Healing2(smCHAR *pChar);

//////////// ������
int sinEffect_Agony(smCHAR *pChar);
int sinEffect_Agony_Particle(POINT3D *pPosi);
int sinEffect_Zenith(smCHAR *pChar,int Time);
int sinEffect_Zenith_Particle(POINT3D *pPosi , int R , int G , int B);
int sinEffect_FireBolt(smCHAR *pChar , smCHAR *desChar);
int sinEffect_FireBolt_Particle(POINT3D *pPosi);
int sinEffect_FireBolt_Particle2(POINT3D *pPosi); //�����ϴٺ��� ���ɉ�� ����!

//////////// ���� �ڵ�
int sinEffect_Bomb6(POINT3D *pPosi); //����
int sinEffect_Bomb6_Particle(POINT3D *pPosi);
int sinEffect_SkillHit(DWORD CODE , POINT3D *pPosi); //��ų Hit ����Ʈ 
int sinEffect_SetCharPosi(cSINEFFECT2 *pEffect); //��ǥ ����
int sinEfect_CheckCrash(cSINEFFECT2 *pEffect);   //��ǥ ����

int sinEffect_StartMagic(POINT3D *pPosi , int CharFlag,int Type = 0); //��������
//////////// extern
extern int MatFireBolt[3];
