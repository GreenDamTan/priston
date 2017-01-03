#ifndef _HO_ASSA_PARTICLE_EFFECT_
#define _HO_ASSA_PARTICLE_EFFECT_




//��� �ִ� ���۸� �����ϴ� �Լ�..(�Լ� �ȿ��� �������� ���� �ϸ� sin�� �Լ��� ���ľ� �ϹǷ�)
int		GetAssaEffect();
int		StopAssaCodeEffect(smCHAR *pChar, int code, int setTime = 0);
//BOOL	GetAssaCodeEffect(smCHAR *pChar, int code);
bool	AddAssaCodeEffectTime(smCHAR *pChar, int code, int time);

BOOL	GetAssaUseEffect(smCHAR *pChar, int code);
bool	GetAssaContinueSkill(int skillCode);			//���� ���� ��ų�� ���� �ǰ� �ִ��� �󿭾� �� �˻� �ϴ� �Լ�
//���̴� ���� ��ƼŬ...
extern int AssaParticle_LineIn(POINT3D *pPosi , char *texName = NULL, int continueTime = 15, int speed = 0);
extern int AssaParticle_LineOut(POINT3D *pPosi , char *texName = NULL, int continueTime = 10, int speed = 0);
extern int AssaParticle_Star(POINT3D *pPosi);

//Paladin HolyValor�� (Jang: ��Ƽ�� Member: ��Ƽ��)
extern int AssaParticle_HolyValor_Jang(smCHAR *pChar, int maxTime = 0);
extern int AssaParticle_HolyValor_Member(smCHAR *pChar, int maxTime = 0);

//Paladin Drastic Spirit�� (Jang: ��Ƽ�� Member: ��Ƽ��)
extern int AssaParticle_DrasticSpirit_Jang(smCHAR *pChar, int maxTime = 0);
extern int AssaParticle_DrasticSpirit_Member(smCHAR *pChar, int maxTime = 0);

//Valkyrie (Windy, TwistJavelin, FireJavelin)
extern int AssaParticle_Windy(smCHAR *pChar, int maxTime = 0);
extern int AssaParticle_TwistJavelinShot(POINT3D *pPosi, POINT3D *pDesti);
extern int AssaParticle_FireJavelin(POINT3D *pPosi, POINT3D *pDesti);
extern int AssaParticle_FireJavelin(smCHAR *pChar, POINT3D *pDesti);


//Saintess (DivineLighting)
//extern int AssaParticle_DivineLighting(POINT3D *pPosi, POINT3D *pDesti);
extern int AssaParticle_DivineLighting(smCHAR *pChar);
extern int AssaParticle_Sprak(POINT3D *pPosi);

//Wizard (ColumnOfWater, EnchantWeapon, DeadLay)
extern int AssaParticle_ColumnOfWater(POINT3D *pPosi, POINT3D *pDesti, int level = 1);
extern int AssaParticle_ColumnOfWaterHit(POINT3D *pPosi);

extern int AssaParticle_EnchantWeaponIceJang(smCHAR *pChar, int maxTime = 0);
extern int AssaParticle_EnchantWeaponFireJang(smCHAR *pChar, int maxTime = 0);
extern int AssaParticle_EnchantWeaponLightJang(smCHAR *pChar, int maxTime = 0);


extern int AssaParticle_EnchantWeaponJinMember(smCHAR *pChar);
extern int AssaParticle_EnchantWeaponIceMember(smCHAR *pChar, int maxTime = 0);
extern int AssaParticle_EnchantWeaponFireMember(smCHAR *pChar, int maxTime = 0);
extern int AssaParticle_EnchantWeaponLightMember(smCHAR *pChar, int maxTime = 0);

extern int AssaParticle_LightShot(POINT3D *pPosi, POINT3D *pDesti);
extern int AssaParticle_FireShot(POINT3D *pPosi, POINT3D *pDesti);
extern int AssaParticle_IceShot(POINT3D *pPosi, POINT3D *pDesti);

extern int AssaParticle_LightHit1(POINT3D *pPosi);
extern int AssaParticle_IceHit1(POINT3D *pPosi);
extern int AssaParticle_FireHit1(POINT3D *pPosi);
extern int AssaParticle_FireHit2(POINT3D *pPosi);

extern int AssaParticle_BigFireHit1(POINT3D *pPosi);

extern int AssaParticle_DeadLay(POINT3D *pPosi, POINT3D *pDesti);
extern int AssaParticle_HolyMind_Attack(smCHAR *pChar, int liveCount);

//int AssaParticle_AgingBody(smCHAR *pChar);

extern int StartEffect_Serqbus_Skill1(smCHAR *pChar, int liveCount);
extern int StartEffect_Serqbus_Skill2(smCHAR *pChar, int liveCount);
extern int StartEffect_Serqbus_Skill3(smCHAR *pChar, int liveCount);

//Fury ���� ����Ʈ
int AssaParticle_Fury_Eye(smCHAR *pChar);							//���� �ٴ� ����Ʈ
int AssaParticle_Fury_ShotFire(smCHAR *pChar, POINT3D *destPos);	//�ҽ��..

int AssaParticle_Fury_RespawnMonster(POINT3D *pPosi);						//���͵� ��ȯ..
int AssaParticle_Fury_RespawnBody(smCHAR *pChar);                  //���� ��ȯ�� ǻ���� �Կ��� ������ ��ƼŬ.
int AssaParticle_Fury_Skill1(smCHAR *pChar);						//���� ����..

int AssaParticle_Fury_Skill1(POINT3D *pPosi, POINT3D *pAngle);      //���� ����.. ���� ������ ���ؼ�..

//Evil Maple���� ����Ʈ..
int AssaParticle_EvilMapleShot(smCHAR *pChar);

//Evil Plant���� ����Ʈ..
int AssaParticle_EvilPlantShot(smCHAR *pChar);

//Thorn Crawler���� ����Ʈ..
int AssaParticle_ThornCrawlerShot(smCHAR *pChar, POINT3D *destPos);

//Stone Golem���� ����Ʈ..
int AssaParticle_StoneGolemShot(smCHAR *pChar);


//Heavy Goblin���� ����Ʈ..
int AssaParticle_HeavyGoblinHit1(smCHAR *pChar); //������ ����ġ�� ����
int AssaParticle_HeavyGoblinHit2(smCHAR *pChar); //����̵忡�� ġ�� ����

//Doom Gurad���� ����Ʈ..
int AssaParticle_DoomGuardHit1(smCHAR *pChar); //������ ����ġ�� ����
int AssaParticle_DoomGuardHit2(smCHAR *pChar); //����̵� ġ�� ����

//Snail ���� ����Ʈ..
int AssaParticle_SnailShot(smCHAR *pChar, POINT3D *destPos);

//Night Mare���� ����Ʈ..
int AssaParticle_NightMare(smCHAR *pChar);

//Ratoo ���彺
int AssaParticle_RatooHit1(smCHAR *pChar);

//Monster ġ��
int AssaParticle_Hit1(POINT3D *destPos);


//Spider �Ź� ���� ����
int AssaParticleSpiderSkill1(smCHAR *pChar);

//���� ����Ʈ
void AssaParticle_CastleRed(smCHAR *pChar);
void AssaParticle_CastleGreen(smCHAR *pChar);
void AssaParticle_CastleBlue(smCHAR *pChar);

//ClanMonster ����Ʈ
int AssaParticleClanMonster(smCHAR *pChar);
int AssaParticleClanMonsterHit1(smCHAR *pChar);
int AssaParticleClanMonsterHit2(smCHAR *pChar);
int AssaParticle_Sprak1(POINT3D *pPosi);


//3�� ��ų ������
void AssaParticle_Concentration(smCHAR *pChar,int liveCount);
void AssaParticle_SwiftAxe(smCHAR *pChar, int liveCount);
void AssaParticle_AvangingCrash(smCHAR* pChar);
void AssaParticle_BoneSmash1(smCHAR* pChar);
void AssaParticle_BoneSmash2(smCHAR* pChar);

//3�� ��ų ����ũ
void AssaParticle_VeonmSpear(smCHAR *pDest);
void AssaParticle_VeonmSpearPike(smCHAR *pChar);
void AssaParticle_Vanish(smCHAR *pChar);
void AssaParticle_ChainLance(POINT3D *pPosi);


//3�� ��ų ��ó
void AssaParticle_ElementalShotFire(POINT3D* pCur, POINT3D* pDest);
void AssaParticle_ElementalShotLighting(POINT3D* pCur, POINT3D* pDest);
void AssaParticle_GoldenFalcon(smCHAR *pChar, int liveCount);
void AssaParticle_BombHit1(POINT3D *pPosi);
void AssaParticle_BombShot(POINT3D* pCur, POINT3D* pDest);
void AssaParticle_Perforation(POINT3D* pCur, POINT3D* Dest);
void AssaParticle_PerforationHit(smCHAR* monster, smCHAR* character, float delay = 0.f);


//3�� ��ų ��ī�ϼ�
void AssaParticle_MetalArmor(smCHAR *pChar, int liveCount);
//void AssaParticle_GrandSmash(POINT3D *pPosi);
void AssaParticle_GrandSmash(smCHAR *pChar);
void AssaParticle_SparkShieldStart(smCHAR* pChar, int liveCount);
void AssaParticle_SparkShieldDefence(POINT3D *pPosi, POINT3D *pDesti);


//3�� ��ų ������
void AssaParticle_EnergyShield(smCHAR *pChar,int liveCount);
void AssaParticle_EnergyShieldDefence(smCHAR *pChar);
void AssaParticle_Diastrophism(smCHAR *pChar);
void AssaParticle_DiastrophismPart(smCHAR *pChar);
void AssaParticle_SpiritElemental(smCHAR *pChar, int liveCount);

void AssaParticle_DancingSwordFire(smCHAR *pChar,int liveCount, int level);
void AssaParticle_DancingSwordIce(smCHAR *pChar,int liveCount, int level);

//3�� ��ų ����Ʈ
void AssaParticle_FlameBrandish(smCHAR *pChar);
void AssaParticle_FlameBrandishHit1(smCHAR *pChar);
void AssaParticle_HolyIncantationShot(smCHAR* pDestChar);
void AssaParticle_HolyIncantationPart(smCHAR* pDestChar, int liveCount);
void AssaParticle_HolyIncantationHand(smCHAR* pDestChar);

void AssaParticle_GrandCrossHorz(POINT3D* pPosi, int angleY);
void AssaParticle_GrandCrossVert(POINT3D* pPosi, int angleY);
void AssaParticle_DivineInhalationStart(smCHAR* pChar, int liveCount);
void AssaParticle_DivineInhalationPart(smCHAR* pChar, int liveCount);
void AssaParticle_DivineInhalationDefence(POINT3D *pDest);


//3�� ��Ż��Ÿ
void AssaParticle_SplitJavelin(POINT3D* pCur, POINT3D* pDest, int count = 2);
void AssaParticle_TriumphOfValhalla(smCHAR *pDestChar, int liveCount);
void AssaParticle_TriumphOfValhallaMember(smCHAR *pCurChar, smCHAR *pDestChar, int liveCount);
void AssaParticle_LightingJavelin(POINT3D* pCur, POINT3D* pDest);
void AssaParticle_StormJavelin(POINT3D* pCur, POINT3D* pDest);
void AssaParticle_StormJavelinHit(smCHAR* monster, smCHAR* character, float delay = 0.f);


//3�� ��ų ������Ƽ��
void AssaParticle_VigorBall(smCHAR* pCurChar, smCHAR* pDestChar);
void AssaParticle_ResurrectionShot(smCHAR* pDestChar);
void AssaParticle_ResurrectionPart(smCHAR* pDestChar);
void AssaParticle_Extinction(smCHAR* pDestChar);
void AssaParticle_VirtualLife(smCHAR* pDestChar, int liveCount);



//Monster ��ƼŬ
void AssaParticle_MonsterPigonHand(smCHAR* pChar);

//ClassUp ��ƼŬ
void AssaParticle_ClassUpWeapon(smCHAR* pChar);
void AssaParticle_StopClassUpWeapon(smCHAR* pChar);
void AssaParticle_AgingBody(smCHAR* pChar, int num);

//Iron Monster ��ų
void AssaParticle_MonsterExplision(POINT3D *pPosi);
void AssaParticle_MonsterIronSadNessHit1(smCHAR *pChar);
void AssaParticle_MonsterIronSadNessSkill1(smCHAR *pChar);
void AssaParticle_MonsterIronHyperHit1(smCHAR *pChar, POINT3D *pDest);
void AssaParticle_MonsterIronHyperSkill1(smCHAR *pChar, POINT3D *pDest);

void AssaParticle_MonsterIronChainGolem(smCHAR *pChar);
void AssaParticle_MonsterIronRunicGuardian(smCHAR *pChar, smCHAR *pDestChar);
void AssaParticle_MonsterIronMountainHit1(smCHAR *pChar);
void AssaParticle_MonsterIronMountainSkill1(smCHAR *pChar);
void AssaParticle_MonsterIronTowerGolemHit1(smCHAR *pChar);
void AssaParticle_MonsterIronTowerGolemSkill1(smCHAR *pChar);
void AssaParticle_MonsterIronHit1(POINT3D *pPosi);

void AssaParticle_MonsterIronOmuHit1(smCHAR *pChar);
void AssaParticle_MonsterIronFist(smCHAR *pChar);
void AssaParticle_MonsterIronMorgonHit(smCHAR *pChar);
void AssaParticle_MonsterAvelArcher(smCHAR *pChar, smCHAR *pDestChar);
void AssaParticle_MonsterAvelQueen(smCHAR *pChar, smCHAR *pDestChar);
void AssaParticle_MonsterDeadZoneDead(smCHAR *pChar);
void AssaParticle_MonsterHyperDead(smCHAR *pChar);

void AssaParticle_MonsterRunicGuardianHand(smCHAR *pChar);
void AssaParticle_MonsterRunicGuardianShot(smCHAR *pChar, smCHAR *pDestChar);
void AssaParticle_MonsterRunicGuardianBossLightShot(smCHAR **pCharList, int charCount);

void AssaParticle_MonsterRunicGuardianBossShot(smCHAR *pChar, int blendType = SMMAT_BLEND_INVSHADOW);
void AssaParticle_MonsterRunicGrardianBossSkill(smCHAR *pChar);
void AssaParticle_MonsterRunicGuardianBossSkillHit(POINT3D *pDest);
void AssaParticle_MonsterRunicGuardianBossWeapon(smCHAR *pChar);
void AssaParticle_MonsterRunicGuardianBossHit(smCHAR *pCurChar, smCHAR *pDestChar, smCHAR **charList = 0, int charCount = 0);

void AssaParticle_ShelltomShot(POINT3D *pPosi, POINT3D *pDest, int type = 0);
void AssaParticle_BamShot(POINT3D *pPosi, POINT3D *pDest);

void AssaParticle_ShelltomWeapon(smCHAR *pChar, int time, int code);
void AssaParticle_ShelltomWeaponHit(POINT3D *pPosi);

//Terrain ȿ��
void AssaParticle_TerrainShowStart();
void AssaParticle_TerrainFire(POINT3D *pCur, POINT3D *pDest, int delay = 0);
void AssaParticle_TerrainFireHit(POINT3D *pCur);

void AssaMonsterHitTest(smCHAR *pChar);

//4�� ��ų
void SkillSaintGodlyShield(smCHAR *pChar, float time);
void SkillArchMageFlameWave(smCHAR *pChar);
void SkillCelestialChainLighting(smCHAR **pCharList, int charCount);
void SkillValhallaHallOfValhallaHandEffect(smCHAR *pChar, float time);
void SkillImpulsionLight(smCHAR *pChar, float time);
void SkillImpulsionLightHit(POINT3D *pos);
void SkillShadowMasterHit(POINT3D *pos);
void SkillPhoenixShotFlame(smCHAR *pChar, POINT3D *curPos);
void SkillGodBless(smCHAR *pChar);
void SkillParticle_FrostJavelin(smCHAR *pChar, float time);

void AssaMotionBlur(smCHAR *pChar, char *meshName1, char *meshName2, int timeCount);


// ���̽� �ʵ�

//ColdEye ������ ���..
int ParkAssaParticle_ColdEye(POINT3D *pPosi, POINT3D *pDesti);
//ColdEye ��ų����
int ParkParticle_ColdEyeSkill(smCHAR *pChar);
//�̽�ƽ ���ǳ� �����ϱ�
int ParkAssaParticle_MisticSpinel(smCHAR *pChar);
// ���ν�Ʈ ���̼�Ʈ ��� ����Ʈ
int ParkAssaParticle_AcientDia(smCHAR *pChar);
// ���ν�Ʈ ���̼�Ʈ �Ǿ��
int ParksinSkillEffect_AcientPiercing(smCHAR *pChar);
int ParksinSkillEffect_AcientPiercingAtt(smCHAR *pChar);

// ���̽� �� ����Ʈ
int ParkAssaParticle_IceGolemShot(smCHAR *pChar);
// ������ �̽�Ʈ	�⺻����
int ParkAssaParticle_MistIceBolt(smCHAR* pCurChar, smCHAR* pDestChar);
// ������ �̽�Ʈ	��ų����
int ParkAssaParticle_MistIceBall(smCHAR* pCurChar, smCHAR* pDestChar);

// ���̽� ���
int ParkAssaParticle_IceGoblinHit1(smCHAR *pChar);


int ParkAssaParticle_Valento1(smCHAR* pValento);	// ��������Ʈ �귣��
int ParkAssaParticle_Valento2_1(smCHAR* pValento);	// ��������Ʈ ����Ŀ (�Ǿ���� �� ������)
int ParkAssaParticle_Valento2_2(smCHAR* pValento);	// ����Ŀ �Ŀ� �⸦ ��ƽ��.
int ParkAssaParticle_Valento3_1(smCHAR* pValento);	// ��������Ʈ ����ġ��� (ParkAssaParticle_Valento1) ȣ�� ���Լ��� �÷�ġ��.
void ParkAssaParticle_MonsterIceValentoBody(smCHAR *pValento);	// ��������Ʈ ���뿡 �ٴ� ��ƼŬ
void ParkValentSwordShow(smCHAR* pValento, int nTime);	// ��������Ʈ �˱�

int ParkAssaParticle_ChaosKara1(smCHAR* pChar);	// ī���� ī�� �Ϲݰ���
int ParkAssaParticle_ChaosKara2(smCHAR* pChar);	// ī���� ī�� ���̽� ��Ƽ��
int ParkAssaParticle_ChaosKaraSkill_Monster(smCHAR* pMonster); // ī����ī�� �����̾ (���� �ڽ�)
int ParkAssaParticle_ChaosKaraSkill_User(smCHAR* pUser);	// ī����ī�� �����̾ (����)
void ParkAssaParticle_MonsterIceChaosKara(smCHAR *pChar);	// ī���� ī�� ������ ��ƼŬ


///////////////// ���ο� �ʵ� ////////////////
// ��öȣ //
void ParkAssaParticle_SpiderTrooperHit1(smCHAR *pChar);	// �����̴� Ʈ���� ������ �վ��

void ParkAssaParticle_StingRay(smCHAR *pChar);	//���÷��� ������ �վ��..
int ParkDivineLight_Effect( smCHAR *lpChar );		//����� ����Ʈ�� ��ų ���� ( ����Ʈ ���� )
int ParkAssaParticle_DivineLighting(smCHAR *pChar);	//����� ����Ʈ�� ��ų ���� ( ����Ʈ ���� )
int ParkAssaParticle_Sprak1(POINT3D *pPosi);
int ParkAssaParticle_Posion(smCHAR *pChar, int liveCount);		// ���ɸ��� ǥ��
void ParkAssaParticle_MummyShot(smCHAR *pChar, smCHAR *pDestChar);	// �ӹ̷ε� ������
#define STINGRAY_POSION_TIME	5

void ParkBlizzardGiantSkillToTarget(smCHAR *pTarget);	//���ڵ� ���̾�Ʈ ��ų ����Ʈ

void ParkDevilBirdAttackBlur(smCHAR* pDevilBird);	// ���� ���� �Ϲ� ���� ���۽� ��
void ParkDevilBirdAttackNormal(smCHAR *pTarget);	// ���� ���� ���ݵ��� 2�� Ÿ������Ʈ
void ParkDevilBirdSkillToTarget(smCHAR *pTarget);	// ���� ���� ������� Ÿ�� ����Ʈ

void ParkAssaParticle_MonsterTurtleCanonNormal(smCHAR *pTCanon, smCHAR* pTarget);	// ��Ʋĳ�� �Ϲ� ����

void ParkAssaParticle_FireCracker(smCHAR *pChar, int nFireCracker);	// ��������Ʈ

// ǫ�� ��Ʈ���� 20���� 4�����迭 �� 20 ^ 4 �� ���̺��� �����Ͽ� ���Ͻ����ش�.
// ��ȣ�� �Է��ϸ� �� ��ȣ�� �´� �迭�� �O�Ƽ� ���ڵ� �Ͽ� ���Ͻ����ش�.
DWORD GetTableNum(int n);

// 4���� ���� ���ڵ��Ѵ�.
DWORD Park_VarEncode(int a, int b, int c, int d);

// ���ڵ�� ���� �ٽ� ���ڵ� ���ش�.
void Park_VarDecode(DWORD dwData, int &nData1, int &nData2, int &nData3, int &nData4);

// �̺��� ����Ʈ
void ParkKelvezuHit(smCHAR *pChar);	// �̺��� ���ݽ� ĳ���� �ǰ�����Ʈ
void ParkKelvezuSkillHit(smCHAR *pChar);	// �̺��� ���ݽ� ĳ���� �ǰ�����Ʈ
void ParkKelvezuNormal1(smCHAR *pChar);
void ParkKelvezuNormal2(smCHAR *pKelvezu);
void ParkKelvezuSkill1(smCHAR *pKelvezu);
void ParkKelvezuSkill2(smCHAR *pKelvezu);
void ParkKelvezuJin(smCHAR *pKelvezu);	// �̺��� �������� �Կ��� ���⳪�°�

// �̺��� ���ݽ� ���鿡�� �˱�ȿ��
void ParkKelvezuFingerShow(smCHAR* pKelvezu, int nTime);

// �̺��� ���ݽ� �������� �˱�ȿ��
void ParkKelvezuWingShow(smCHAR* pKelvezu, int nTime);

// �̺��� ���ݽ� �������� �˱�ȿ��
void ParkKelvezuTaleShow(smCHAR* pKelvezu, int nTime);

// ������ ���� ����Ʈ
void ParkAssaParticle_WorldCupMonsterAttack(smCHAR* pMonster, smCHAR *pChar);
	// ������ ���Ͱ� ���ݽ� ĳ���� �ǰ�����Ʈ
void ParkWorldCupMonsterHit(smCHAR *pChar);



// Dark Phalanx ��� ���� ����Ʈ (â���� ��� â�� �İ��� ������ ����Ʈ - �Ǿ������ ó��)
void ParkAssaParticle_DarkPhalanx(smCHAR *pDarkPhalanx);

// ���̾� �� �Ϲ� ���� �� ��ų����
void ParkAssaParticle_TerrainFireHit(POINT3D *pCur, int nType);
void ParkAssaParticle_TerrainFire(POINT3D *pCur, POINT3D *pDest, int delay = 0, int nType = 0);
void ParkAssaParticle_FireWorm(smCHAR* pTarget, BOOL bSkill = FALSE);

// Ű�޶� �Ϲݰ��� ( ���� �� )
void ParkAssaParticle_ChimeraNormal(smCHAR *pChimera);

// Ű�޶� ��ų���� (��� - �Ӹ��� �ͽ�Ʈ���ǵ�)
void ParkAssaParticle_ChimeraSkill(smCHAR *pChar);

// Bloody Knight ��ų ���� (�ξ�)
void ParkAssaParticle_BloodySkill(smCHAR *pBloodKnight);

// Bloody Knight ��ų ���� �ǰ� (����)
void ParkAssaParticle_BloodySkillStun(smCHAR *pChar);

// �� �Ͽ�� �븻���� (�ι��� ��� ����� ��������)
void ParkAssaParticle_HellHoundNormal(smCHAR *pChar);

// �� �Ͽ�� ��ų���� (�Կ��� �ҳ���)
void ParkAssaParticle_HellHoundSkillAttack(smCHAR *pHellHound);

// �� �Ͽ�� ��ų������ �ǰݽ� (ȭ���� �۽��ε��� ����Ʈ)
void ParkAssaParticle_HellHoundSkillTaget(smCHAR *pChar);

// ��ũ ���� �⺻���� �ǰݽ�
void ParkAssaParticle_DarkGuardTaget(smCHAR *pChar);

// ��ũ ���� ��ų����
void ParkAssaParticle_DarkGuardSkill(smCHAR *pDarkGuard, smCHAR *pChar);

// ��ũ ������ �⺻����
void ParkAssaParticle_DarkMageNormal(smCHAR *pDarkMage, smCHAR *pChar);

// ��ũ ������ ��ų����
void ParkAssaParticle_DarkMageSkill(smCHAR *pChar);

// ��ũ ������ ������
void ParkAssaParticle_DarkMageDead(smCHAR *pDarkMage);






#endif