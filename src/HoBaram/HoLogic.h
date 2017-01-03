#ifndef _HO_LOGIC_H_
#define _HO_LOGIC_H_

/*
//���峭 ���� ġ��...
class HoWeaponFix : public HoEffectObject
{
public:
	HoWeaponFix();
	~HoWeaponFix();

private:	
	int	TimeCount;    //�ð� ī��Ʈ
	
	int CurrentBlendValue;
	int BlendStep;
	
	int VertexCount;
	int WeaponFixImage;

	int DestX;
	int DestY;
	int DestWidth;
	int DestHeight;


	int CurrentWidth, CurrentHeight;
	int WidthStep, HeightStep;
public:
	int Start(int x, int y, int sizeX, int sizeY);
	int Draw();
	int Init();

	int Main();
};
*/

/*
//Potion�� ���� Particle
class HoPotionParticle : public HoEffectParticle
{
public:
	HoPotionParticle();
	~HoPotionParticle();
private:
	int		LocalAngle;

public:
	int Main();
	void Init();

	int Draw(int x, int y, int z, int ax, int ay, int az);
	int Start(int x, int y, int z, float xv, float yv, float zv, char *iniName);
};
*/

#define PAT_PROCESS		2000
#define PAT_END			3000


#define MAX_BLUR_LIST		15
#define	MAX_BLUR_VERTEX		300
#define	MAX_BLUR_FACE		200
#define	MAX_BLUR_TEXLINK	200


//�������� ���ư��� ������Ʈ..
class HoEffectPatDest : public HoEffectPat
{
private:
	int Level;
	int DestX, DestY, DestZ;
	int DestAngleX, DestAngleY, DestAngleZ;
	

	float VelocityY;
	int   Step;

	HoPrimitiveBillboard PrimitiveBillBoard; //Event�� Effect Object�� ������� ����Ʈ..
	
	int SkillType;
	int WorkState;
	
	
	//�ܻ��...
	BOOL BlurStartFlag;
	POINT3D	PosList[MAX_BLUR_LIST];
	int		PosListCount;
	
	smSTAGE_VERTEX		Vertex[MAX_BLUR_VERTEX];
	int					VertexCount;
	
	smSTAGE_FACE		Face[MAX_BLUR_FACE];
	int					FaceCount;
	
	smTEXLINK			TexLink[MAX_BLUR_TEXLINK];
	int					TexLinkCount;
	int					SumCount;
	
	int					CreatePathFace();
	
	int					AniDataIndex;
	int					BlurBlendValue;//CurrentBlendValue;
	int					BlurTimeCount;
	int					BlurCurrentFrame;
	int					SizeWidth;


public:
	HoEffectPatDest();
	~HoEffectPatDest();
	
public:
	int Draw(int x, int y, int z, int ax, int ay, int az);
	int Main();

	//���󰡴� ������Ʈ..
	int StartDest(POINT3D currentPos, POINT3D destPos, smPAT3D *pat, int SkillType, int level = 0);
	int StartAni(int x, int y, int z, int angleX, int angleY, int angleZ, int destX, int destY, int destZ, smPAT3D *pat, char *addEffectName = NULL);
	int EventEnd();
	

	//�ܻ��.
	int StartBlur(char *iniName);
	int MainBlur();
	int	DrawBlur(int x, int y, int z, int ax, int ay, int az);
};




//��� ����.
#define		POS_LIST_MAX	15

#define		MAX_SHIELD_VERTEX	300
#define		MAX_SHIELD_FACE		200
#define		MAX_SHIELD_TEXLINK	200

class HoEffectShield : public HoEffectObject
{
public:
	HoEffectShield();
	~HoEffectShield();
	
	HoPrimitiveBillboard	BackBillBoard;		//Shile1���� ���󰡴� ���׶� ������..
	HoEffectPat				UnderPat;
	
	int AniDataIndex;
	int	CurrentFrame;
	int TimeCount;
	int BlendStep;
	int CurrentBlendValue;
	
	smFACE2D			Face2d;
	
	int SizeWidth;
	int SizeHeight;
	
	POINT3D DirectionAngle;	
	POINT3D	PosList[POS_LIST_MAX];
	int		PosListCount;
	
	smSTAGE_VERTEX		Vertex[MAX_SHIELD_VERTEX];
	int					VertexCount;
	
	smSTAGE_FACE		Face[MAX_SHIELD_FACE];
	int					FaceCount;
	
	smTEXLINK			TexLink[MAX_SHIELD_TEXLINK];
	int					TexLinkCount;
	
	int					SumCount;
	
	smCHAR				*Character;
	
	int					EndCount; //�� �ð��� �ʰ� ���� ��� �����°�.

	int					ShieldType;

	

protected:
	//BOOL AddFace2D(smFACE2D *face);
	int	CreatePathFace();
public:
	void Init();
	int Draw(int x, int y, int z, int ax, int ay, int az);
	int Main();
	int Start(int x, int y, int z, int shieldType, int angle, int circleRadius, smCHAR *character); 
	
	void SetUnderPat(smPAT3D *pat, int aniEnd);
};


//�տ� �⸦ ������ ���..
class HoEffectPower : public HoEffectObject
{
public:
	HoEffectPower();
	~HoEffectPower();

	int  Skill_n;			//ktj : Power����Ʈ�� �������� ����̶� ����ġ���ʿ��ؼ� �Ϻη�����.

private:
	HoPrimitiveBillboard	BackBillBoard;			//�ڿ��� ���� ���� �ϴ� ����Ʈ
	
	HoPrimitiveBillboard	ParticleBillBoard[10];

	smCHAR					*Character;
	int						AniDataIndex;


public:
	int Draw(int x, int y, int z, int ax, int ay, int az);
	int Main();					//sw=1->power1.ini 2->power2.ini
	int Start(int x, int y, int z, int sw, int aniType = ANI_ONE, smCHAR *character = NULL);
};


//struct h
//���ο� Ÿ��...(������ ������ ȭ�鿡 �� ���� ���鼭 ������ ������ ���)(Tornado)
class HoEffectHitLine : public HoEffectObject
{
public:
	HoEffectHitLine();
	~HoEffectHitLine();



	//ktj : ���γ���. ==========================start
	int  Skill_n;			//ktj : ����̵��� �������� ����̶� ����ġ���ʿ��ؼ� �Ϻη�����.
	int  cntM, cnt0;
	int delayCount ;		//Ŭ�󽺺������̵���Ŵ.

	
	POINT3D DestPos;		//���ư� ������...
	float Vx;				//�ӵ� ����..
	float Vy;
	float Vz;
	//ktj : ���γ���. ==========================end


private:

	POINT3D DirectionAngle;	
	POINT3D	PosList[POS_LIST_MAX];
	int		PosListCount;
	
	smSTAGE_VERTEX		Vertex[MAX_SHIELD_VERTEX];
	int					VertexCount;
	
	smSTAGE_FACE		Face[MAX_SHIELD_FACE];
	int					FaceCount;
	
	smTEXLINK			TexLink[MAX_SHIELD_TEXLINK];
	int					TexLinkCount;
	int					SumCount;
	
	int					CreatePathFace();
	
	int					AniDataIndex;
	int					CurrentBlendValue;
	int					TimeCount;
	int					CurrentFrame;
	
	int					SizeHeight;

public:
	int Draw(int x, int y, int z, int ax, int ay, int az);
	int Main();
	//ktj : ����
	//int Start(int x, int y, int z, int destX, int destY, int destZ);
	int Start(int x, int y, int z, int destX, int destY, int destZ, int sw);
};


//��� ����.
#define		STUN_POS_LIST_MAX	10

//���� ���Ͽ�.. (?ǥ ������)
class HoEffectStun: public HoEffectObject
{
public:
	HoEffectStun();
	~HoEffectStun();


private:
	/*
	POINT3D					RotateAngle;
	HoPrimitiveBillboard	BackBillBoard;

	smCHAR					*Character;
	*/
	POINT3D DirectionAngle;	
	POINT3D	PosList[POS_LIST_MAX];
	int		PosListCount;
	
	smSTAGE_VERTEX		Vertex[MAX_SHIELD_VERTEX];
	int					VertexCount;
	
	smSTAGE_FACE		Face[MAX_SHIELD_FACE];
	int					FaceCount;
	
	smTEXLINK			TexLink[MAX_SHIELD_TEXLINK];
	int					TexLinkCount;
	int					SumCount;
	
	int					CreatePathFace();
	
	int					AniDataIndex;
	int					CurrentBlendValue;
	int					TimeCount;
	int					CurrentFrame;
	
	int					SizeHeight;

	smCHAR				*Character;

public:
	int Draw(int x, int y, int z, int ax, int ay, int az);
	int Main();
	int Start(int worldX, int worldY, int worldZ, smCHAR *character);
};



#endif