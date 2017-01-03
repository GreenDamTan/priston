/*----------------------------------------------------------------------------*
*	���ϸ� :  AssaParticle
*	�ϴ��� :  �ƽ�! ��ƼŬ
*	�ۼ��� :  ����������Ʈ 2002�� 12��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#ifndef _ASSAPARTICLE_HEADER_

#define _ASSAPARTICLE_HEADER_

#define ASSA_COLOR_CODE_YELLOW		0x00000001
#define ASSA_COLOR_CODE_BLUE		0x00000002
#define ASSA_COLOR_CODE_GREEN		0x00000003
#define ASSA_COLOR_CODE_VIOLET		0x00000004
#define ASSA_COLOR_CODE_RED			0x00000005


//���� ����Ʈ �ڵ� 
#define ASSA_PARTICLE_UNIONCORE		0x70000001
#define ASSA_EFFECT_RATOO3			0x70000002

/*----------------------------------------------------------------------------*
*								�Լ� 
*-----------------------------------------------------------------------------*/	
void sinAssaParticleProcess(cASSAEFFECT *pEffect); 
int  sinAssaParticleKeyDown();


/*----------------------------------------------------------------------------*
*							Particle	�Լ� 
*-----------------------------------------------------------------------------*/	

// ���� 
int AssaParticle_Stune(smCHAR *pChar , int Time);

// �� ����� 
int AssaMakeCircle(POINT3D *pPosi , char *FileName , int Radius);

// ��Ʈ �ø���
int AssaParticle_RisingBelt(POINT3D *pPosi ,char *FileName,int ContinueTime , int SpeedY );

//����� ��ƼŬ 
int AssaParticle_RisingParticle(POINT3D *pPosi , char *FileName ,int Size ,  int ContinueTime , int Range , int Count , int SpeedY);

//����� ����
int AssaParticle_RisingLine(POINT3D *pPosi , char *FileName , int Size, int ContinueTime , int Range , int Count , int SpeedY);

//��� ��� ���� 
int AssaParticle_RisingRotateLine(POINT3D *pPosi , char *FileName , int ContinueTime , int Range , int Count , int SpeedY);

//��� ��� ���� ���
int AssaParticle_RisingRotateLinePower(POINT3D *pPosi , char *FileName,int Range ,int Speed );

//������ ��ƼŬ 
int AssaParticle_Wave(POINT3D *pPosi , char *FileName ,int Size);

//Light
int AssaParticle_Light(POINT3D *pPosi , char *FileName ,int Size , int Height , int Time);

//������ ��ƼŬ 
int AssaParticle_Spread(POINT3D *pPosi , char *FileName );

//�׳� �о����� �̹��� ��ƼŬ 
int AssaParticle_WideFace(POINT3D *pPosi , char *FileName , int Size);

//���̴� ��ƼŬ
int AssaParticle_Gather(POINT3D *pPosi , char *FileName ,int Range , int Speed ,int Num);

//���鼭 ���̴� ��ƼŬ 
int AssaParticle_RotateGather(POINT3D *pPosi , char *FileName , int Num);

//����翡 ���� ������ 
int AssaParticle_BALLLIGHT_GATHER(POINT3D *pPosi , char *FileName );

//����翡 ���� ������ 
int AssaParticle_BALLLIGHT_ROTATE(POINT3D *pPosi , char *FileName ,int Range , int Speed);

//���������� ����¡ ��ƼŬ 
int AssaParticle_RisingParticleContinue(POINT3D *pPosi , char *FileName ,int Size ,int Range , int SpeedY);

//���鼭 ������ ��ƼŬ 
int AssaParticle_RotateWide(POINT3D *pPosi , char *FileName ,int Range , int SpeedY );

//������ ��ƼŬ 
int AssaParticle_Wide(POINT3D *pPosi , char *FileName ,int Num , int Time);

//�����ٰ� ���δ� 
int AssaParticle_BombGather(cASSAEFFECT *pEffect);

//�ֺ������� ��ƼŬ 
int AssaParticle_Area_Particle(POINT3D *Posi , char *FileName , int Range , int Time);

//����� Hit
int AssaParticle_SpecialHit(POINT3D *pPosi , int Time); 

//��ġ�� �ܻ� 
int AssaParticle_After_Image(POINT3D *Posi , char *FileName , int Range , int Time);

//�Ǿ�� ��ƼŬ
int AssaParticle_Piercing(cASSAEFFECT *pEffect);

//�Ǿ�� Į �ֺ� �ܻ� 
int AssaParticle_Piercing_Aura(cASSAEFFECT *pEffect);

//Ÿ�� ��ƼŬ (Brandish���� ���)
int AssaParticle_SpecialHit2(POINT3D *pPosi , int Time,int Kind=0);

//����� ��ƼŬ 
int AssaParticle_Windy2(smCHAR *pChar);

//Ȧ�� ���÷����� ��ƼŬ 
int AssaParticle_Holy_Reflection(cASSAEFFECT *pEffect);

//Ȧ�� ���ε� 
int AssaParticle_HolyMind(POINT3D *pPosi , int Flag=0);

//Light
int AssaParticle_Char_Light(smCHAR *pChar ,  DWORD ColorCODE , int Time ,int MaxAlpha, int  SizeW ,int SizeH=0 );

//Light2
int AssaParticle_Char_Light2(smCHAR *pChar ,  DWORD ColorCODE , int Time );

//UnionCore
int sinAssaUnionCoreEffect(smCHAR *pChar);

int sinEffect_Ratoo_Aura(smCHAR *pChar );

int sinEffect_Ratoo_Aura2(smCHAR *pChar );
#endif
