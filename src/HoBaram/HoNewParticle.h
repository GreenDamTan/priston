
#ifndef _HO_NEW_PARTICLE_
#define _HO_NEW_PARTICEL_

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <list>
#include <typeinfo.h>


using namespace std;
#ifndef D3D_OVERLOADS
#define D3D_OVERLOADS
#endif

class HoNewParticleEmitterToken;
class HoNewParticle;
#define _HO_WIN_32_


#define EVENT_ID_NONE		    0x00000
#define EVENT_ID_ALPHA		    0x00001
#define EVENT_ID_COLOR		    0x00002
#define EVENT_ID_REDCOLOR	    0x00003
#define EVENT_ID_GREENCOLOR     0x00004
#define EVENT_ID_BLUECOLOR      0x00005
#define EVENT_ID_VELOCITY       0x00007
#define EVENT_ID_VELOCITYX      0x00008
#define EVENT_ID_VELOCITYY	    0x00009
#define EVENT_ID_VELOCITYZ      0x00010
#define EVENT_ID_SEQUENCE       0x00011
#define EVENT_ID_EVENTTIMER     0x00012
#define EVENT_ID_SIZE		    0x00013
#define EVENT_ID_PART_ANGLE     0x00014
#define EVENT_ID_PART_ANGLEX    0x00015
#define EVENT_ID_PART_ANGLEY    0x00016
#define EVENT_ID_PART_ANGLEZ    0x00017
#define EVENT_ID_SIZEEXT        0x00018
#define EVENT_ID_LOCAL_ANGLE    0x00019
#define EVENT_ID_LOCAL_ANGLEX   0x00020
#define EVENT_ID_LOCAL_ANGLEY   0x00021
#define EVENT_ID_LOCAL_ANGLEZ   0x00022

//Event ����..
class HoNewParticleEvent
{
public:
	HoNewParticleEvent()  
	{
		NextFadeEvent = NULL;
		SetID(EVENT_ID_NONE);
	}
    virtual ~HoNewParticleEvent()  {}
    HoNewParticleEvent *NextFadeEvent;
protected:
	HoMinMax<float> TimeRange;
	float ActualTime;
	bool Fade;

	int EventID;
public:
	virtual void InvalidateDeviceObjects() {} //���ҽ����� ��ȿȭ ��Ű�� �Լ�
	virtual void RestoreDeviceObjects() { } //���ҽ����� �ٽ� �д� �Լ�.

	virtual bool FadeAllowed() = 0;
	
	int  GetID() {return EventID;}
	void	SetID(int eventID) {EventID = eventID;}


	bool IsFade(void) const { return(Fade); }
	void SetFade(const bool data = true) { Fade = data; }

	HoMinMax<float> GetTimeRange(void) const { return(TimeRange); }
	void SetTimeRange(const HoMinMax<float> data) { TimeRange = data; }
	
	float GetActualTime(void) const { return(ActualTime); }
	void SetActualTime(const float data) { ActualTime = data; }

	virtual bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
		std::vector<HoNewParticleEmitterToken>::iterator &EndIter) = 0;

	virtual void DoItToIt(HoNewParticle &part) = 0;

	static void ProcessPropEqualsValue(HoMinMax<float> &prop,
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

	static void ProcessPropEqualsValue(HoMinMax<D3DVECTOR> &prop,
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

	static void ProcessPropEqualsValue(HoMinMax<D3DCOLORVALUE> &prop,
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);
};

class HoNewParticleEvent_CompareFunc : public std::binary_function<HoNewParticleEvent *, HoNewParticleEvent *, bool> 
{
public:
	bool operator() (const HoNewParticleEvent* lhs, const HoNewParticleEvent* rhs) const
	{
		return lhs->GetActualTime() < rhs->GetActualTime();
	}
};

class HoNewParticleEvent_Size : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_Size() { SetID(EVENT_ID_SIZE); }
	bool FadeAllowed() { return(true); }
	void DoItToIt(HoNewParticle &part);
	bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
							std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

	HoMinMax<float> GetSize(void) const { return(Size); }
	void SetSize(const HoMinMax<float> data) { Size = data; }

protected:
	HoMinMax<float> Size;
};

class HoNewParticleEvent_SizeExt : public HoNewParticleEvent
{
public:
    HoNewParticleEvent_SizeExt() { SetID(EVENT_ID_SIZEEXT); }
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    	bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
							std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

	HoMinMax<float> GetSize(void) const { return(SizeExt); }
	void SetSize(const HoMinMax<float> data) { SizeExt = data; }

protected:
	HoMinMax<float> SizeExt;
};


class HoNewParticleEvent_Color : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_Color() { SetID(EVENT_ID_COLOR); }
  bool FadeAllowed() { return(true); }
  void DoItToIt(HoNewParticle &part);
  bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  HoMinMax<D3DCOLORVALUE> GetColor(void) const { return(Color); }
  void SetColor(const HoMinMax<D3DCOLORVALUE> &data) { Color = data; }

protected:
  HoMinMax<D3DCOLORVALUE> Color;
};

class HoNewParticleEvent_RedColor : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_RedColor() { SetID(EVENT_ID_REDCOLOR); }	
  bool FadeAllowed() { return(true); }
  void DoItToIt(HoNewParticle &part);
  bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  HoMinMax<float> GetRedColor(void) const { return(RedColor); }
  void SetRedColor(const HoMinMax<float> data) { RedColor = data; }

protected:
  HoMinMax<float> RedColor;

};

class HoNewParticleEvent_GreenColor : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_GreenColor() { SetID(EVENT_ID_GREENCOLOR); }
  bool FadeAllowed() { return(true); }
  void DoItToIt(HoNewParticle &part);
  bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  HoMinMax<float> GetGreenColor(void) const { return(GreenColor); }
  void SetGreenColor(const HoMinMax<float> data) { GreenColor = data; }


protected:
  HoMinMax<float> GreenColor;
};

class HoNewParticleEvent_BlueColor : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_BlueColor() { SetID(EVENT_ID_BLUECOLOR); }
  bool FadeAllowed() { return(true); }
  void DoItToIt(HoNewParticle &part);
  bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  HoMinMax<float> GetBlueColor(void) const { return(BlueColor); }
  void SetBlueColor(const HoMinMax<float> data) { BlueColor = data; }

protected:
  HoMinMax<float> BlueColor;
};

class HoNewParticleEvent_Alpha : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_Alpha() { SetID(EVENT_ID_ALPHA); }
  bool FadeAllowed() { return(true); }
  void DoItToIt(HoNewParticle &part);
  bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  HoMinMax<float> GetAlpha(void) const { return(Alpha); }
  void SetAlpha(const HoMinMax<float> data) { Alpha = data; }


protected:
  HoMinMax<float> Alpha;
};

class HoNewParticleEvent_PartAngle : public HoNewParticleEvent
{
public:
    HoNewParticleEvent_PartAngle() { SetID(EVENT_ID_PART_ANGLE); }
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                            std::vector<HoNewParticleEmitterToken>::iterator &EndIter);


    HoMinMax<D3DVECTOR> GetPartAngle(void) const { return(PartAngle); }
    void SetPartAngle(const HoMinMax<D3DVECTOR> &data) { PartAngle = data; }
protected:
    HoMinMax<D3DVECTOR> PartAngle;
};

class HoNewParticleEvent_PartAngleX : public HoNewParticleEvent
{
public:
    HoNewParticleEvent_PartAngleX() { SetID(EVENT_ID_PART_ANGLEX); }
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                            std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

    HoMinMax<float> GetPartAngleX(void) const { return(PartAngleX); }
    void SetPartAngleX(const HoMinMax<float> &data) { PartAngleX = data; }
protected:
    HoMinMax<float> PartAngleX;
};

class HoNewParticleEvent_PartAngleY : public HoNewParticleEvent
{
public:
    HoNewParticleEvent_PartAngleY() { SetID(EVENT_ID_PART_ANGLEY); }
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                            std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

    HoMinMax<float> GetPartAngleY(void) const { return(PartAngleY); }
    void SetPartAngleY(const HoMinMax<float> &data) { PartAngleY = data; }
protected:
    HoMinMax<float> PartAngleY;
};

class HoNewParticleEvent_PartAngleZ : public HoNewParticleEvent
{
public:
    HoNewParticleEvent_PartAngleZ() { SetID(EVENT_ID_PART_ANGLEZ); }
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                            std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

    HoMinMax<float> GetPartAngleZ(void) const { return(PartAngleZ); }
    void SetPartAngleZ(const HoMinMax<float> &data) { PartAngleZ = data; }
protected:
    HoMinMax<float> PartAngleZ;
};

class HoNewParticleEvent_LocalAngle : public HoNewParticleEvent
{
public:
    HoNewParticleEvent_LocalAngle() { SetID(EVENT_ID_LOCAL_ANGLE); }
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                            std::vector<HoNewParticleEmitterToken>::iterator &EndIter);


    HoMinMax<D3DVECTOR> GetLocalAngle(void) const { return(LocalAngle); }
    void SetLocalAngle(const HoMinMax<D3DVECTOR> &data) { LocalAngle = data; }
protected:
    HoMinMax<D3DVECTOR> LocalAngle;
};

class HoNewParticleEvent_LocalAngleX : public HoNewParticleEvent
{
public:
    HoNewParticleEvent_LocalAngleX() { SetID(EVENT_ID_LOCAL_ANGLEX); }
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                            std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

    HoMinMax<float> GetLocalAngleX(void) const { return(LocalAngleX); }
    void SetLocalAngleX(const HoMinMax<float> &data) { LocalAngleX = data; }
protected:
    HoMinMax<float> LocalAngleX;
};

class HoNewParticleEvent_LocalAngleY : public HoNewParticleEvent
{
public:
    HoNewParticleEvent_LocalAngleY() { SetID(EVENT_ID_LOCAL_ANGLEY); }
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                            std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

    HoMinMax<float> GetLocalAngleY(void) const { return(LocalAngleY); }
    void SetLocalAngleY(const HoMinMax<float> &data) { LocalAngleY = data; }
protected:
    HoMinMax<float> LocalAngleY;
};

class HoNewParticleEvent_LocalAngleZ : public HoNewParticleEvent
{
public:
    HoNewParticleEvent_LocalAngleZ() { SetID(EVENT_ID_LOCAL_ANGLEZ); }
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                            std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

    HoMinMax<float> GetLocalAngleZ(void) const { return(LocalAngleZ); }
    void SetLocalAngleZ(const HoMinMax<float> &data) { LocalAngleZ = data; }
protected:
    HoMinMax<float> LocalAngleZ;
};


class HoNewParticleEvent_Velocity : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_Velocity() { SetID(EVENT_ID_VELOCITY);}
  bool FadeAllowed() { return(true); }
  void DoItToIt(HoNewParticle &part);
  bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                          std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  HoMinMax<D3DVECTOR> GetVelocity(void) const { return(Velocity); }
  void SetVelocity(const HoMinMax<D3DVECTOR> &data) { Velocity = data; }

protected:
  HoMinMax<D3DVECTOR> Velocity;

  int Flag;   // 0:None, 1:Out, 2:In

};

class HoNewParticleEvent_VelocityX : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_VelocityX() { SetID(EVENT_ID_VELOCITYX);}
    bool FadeAllowed() { return(true); }
    void DoItToIt(HoNewParticle &part);
    bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
                            std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

    HoMinMax<float> GetValue(void) const { return(VelocityX); }
    void SetValue(const HoMinMax<float> &data) { VelocityX = data; }

protected:
    HoMinMax<float> VelocityX;
};

class HoNewParticleEvent_VelocityY : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_VelocityY()
	{
		SetID(EVENT_ID_VELOCITYY); 
	}

	bool FadeAllowed() { return(true); }
	void DoItToIt(HoNewParticle &part);
	bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
							std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

	HoMinMax<float> GetValue(void) const { return(VelocityY); }
	void SetValue(const HoMinMax<float> &data) { VelocityY = data; }

protected:
	HoMinMax<float> VelocityY;
};

class HoNewParticleEvent_VelocityZ : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_VelocityZ() { SetID(EVENT_ID_VELOCITYZ); }
	bool FadeAllowed() { return(true); }
	void DoItToIt(HoNewParticle &part);
	bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
							std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

	HoMinMax<float> GetValue(void) const { return(VelocityZ); }
	void SetValue(const HoMinMax<float> &data) { VelocityZ = data; }

protected:
	HoMinMax<float> VelocityZ;
};

class HoNewParticleEvent_EventTimer : public HoNewParticleEvent
{
public:
	HoNewParticleEvent_EventTimer() { SetID(EVENT_ID_EVENTTIMER);}
  bool FadeAllowed() { return(false); }
  void DoItToIt(HoNewParticle &part);
  bool ProcessTokenStream(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
                          std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  HoMinMax<float> GetEventTimer(void) const { return(EventTimer); }
  void SetEventTimer(const HoMinMax<float> &data) { EventTimer = data; }

protected:
  HoMinMax<float> EventTimer;
};

enum ParticleType
{
    TYPE_ONE,       //ī�޶� �������θ� ���̴� ��ƼŬ.
    TYPE_TWO,       //���忡 ����� �ö󰡴� ��ƼŬ..
    TYPE_THREE,     //ī�޶� y���⸸ ���󰡴� ��ƼŬ..
    TYPE_FOUR,     //��ƼŬ ��� ��ƼŬ..(��ƼŬ ���ư��� ��θ� ���� ���� ��ƼŬ)
};


class HoNewParticle
{
public:
	HoNewParticle()
	{
		Weight    = 1.0f; Size = 1.0f; SizeStep = 0.0f;
		LifeTime  = 1.0f; Age = 0.0f; EventTimer = 0.0f;

        SizeExt = 0.0f;
        SizeExtStep = 0.0f;
        
		Color.r = 1.0f;
		Color.g = 1.0f;
		Color.b = 1.0f;
		Color.a = 1.0f;

		ColorStep.r = 0.0f;
		ColorStep.g = 0.0f;
		ColorStep.b = 0.0f;
		ColorStep.a = 0.0f;

		Dir.x = 0.0f;
		Dir.y = 0.0f;
		Dir.z = 0.0f;

		DirStep.x = 0.0f;
		DirStep.y = 0.0f;
		DirStep.z = 0.0f;

		WorldPos.x = 0.0f;
		WorldPos.y = 0.0f;
		WorldPos.z = 0.0f;

        LocalPos.x = 0.0f;
        LocalPos.y = 0.0f;
        LocalPos.z = 0.0f;

        
        PartAngle.x = 0.0f;
        PartAngle.y = 0.0f;
        PartAngle.z = 0.0f;

        PartAngleStep.x = 0.0f;
        PartAngleStep.y = 0.0f;
        PartAngleStep.z = 0.0f;

        LocalAngle.x = 0.0f;
        LocalAngle.y = 0.0f;
        LocalAngle.z = 0.0f;

        LocalAngleStep.x = 0.0f;
        LocalAngleStep.y = 0.0f;
        LocalAngleStep.z = 0.0f;

	}
	virtual ~HoNewParticle() { }

	std::vector<HoNewParticleEvent *>::iterator CurrentEvent; //���� ó���ؾ��� �̺�Ʈ�� ������...

	float Weight;		     //�߷°�
  
	float Size;			     //��ƼŬ ������
	float SizeStep;		     //��ƼŬ �������� ��ȭ��

    float SizeExt;
    float SizeExtStep;
    
	float LifeTime;		    //��� ���� �ð�.
	float Age;			    //��� �ִ� �ð� ī��Ʈ ��

	float EventTimer;		// different from age

	D3DCOLORVALUE Color;     //�÷���.
	D3DCOLORVALUE ColorStep; //�÷��� ��ȭ��


	D3DVECTOR WorldPos;			// ��ƼŬ�� ���忡���� ��ġ..
    D3DVECTOR LocalPos;         // ��ƼŬ��  Local������ ��ġ..

    D3DVECTOR PartAngle;        // ��ƼŬ�� ����..
    D3DVECTOR PartAngleStep;    // ��ƼŬ�� ������ ����

    D3DVECTOR LocalAngle;       // LocalAngle;
    D3DVECTOR LocalAngleStep;    // LocalAngelStep;
    
	D3DVECTOR Dir;			    // �ӵ� ����
	D3DVECTOR DirStep;		    // ���ӵ� ����

    list<D3DVECTOR> TraceList;

    void Init()
    {
        std::list<D3DVECTOR>::iterator i;
        i = TraceList.begin();
        for(i = TraceList.begin(); i != TraceList.end();)
        {
            i = TraceList.erase(i);
        }
    }

    inline bool Main(float timeDelta, int partType)
	{
		// ��� �ִ� �ð� ī��Ʈ�� ����.
		Age += timeDelta;
		EventTimer += timeDelta;

		// ��� ���� �ð��� ��������
		if (Age >=LifeTime)
		{
			return(false); // �׾���.
		}

        if(partType == TYPE_FOUR) //���� ��ƼŬ Ÿ���� ���븦 ����� ��ƼŬ�̸�
        {
            smMATRIX inRotXMatrix;
            smMATRIX inRotYMatrix;
            smMATRIX inRotZMatrix;

            smMATRIX outMatrix;
            smIdentityMatrix(outMatrix);
            smIdentityMatrix(inRotXMatrix);
            smIdentityMatrix(inRotYMatrix);
            smIdentityMatrix(inRotZMatrix);

            POINT3D angle;
            angle.x = (int)LocalAngle.x;
            angle.y = (int)LocalAngle.y;
            angle.z = (int)LocalAngle.z;

            smRotateXMatrix(inRotXMatrix, (int)(angle.x & ANGCLIP) );
            smRotateYMatrix(inRotYMatrix, (int)(angle.y & ANGCLIP) );
            smRotateZMatrix(inRotZMatrix, (int)(angle.z & ANGCLIP) );

            smMatrixMult(outMatrix, inRotXMatrix, inRotYMatrix);
            smMatrixMult(outMatrix, outMatrix, inRotZMatrix);

            POINT3D inlocalPos;
            inlocalPos.x = (int)(LocalPos.x*fONE);
            inlocalPos.y = (int)(LocalPos.y*fONE);
            inlocalPos.z = (int)(LocalPos.z*fONE);

            POINT3D outlocalPos;
            D3DVECTOR localPos;
            outlocalPos.x = inlocalPos.x * outMatrix._11+
                            inlocalPos.y * outMatrix._21+
                            inlocalPos.z * outMatrix._31;
            localPos.x = float(outlocalPos.x)/float(fONE*fONE);

            outlocalPos.y = inlocalPos.x * outMatrix._12+
                            inlocalPos.y * outMatrix._22+
                            inlocalPos.z * outMatrix._32;
            localPos.y = float(outlocalPos.y)/float(fONE*fONE);

            outlocalPos.z = inlocalPos.x * outMatrix._13+
                            inlocalPos.y * outMatrix._23+
                            inlocalPos.z * outMatrix._33;
            localPos.z = float(outlocalPos.z)/float(fONE*fONE);

            if(SizeExt > 0) //������ ���̰��� ������
            {
                //static int count = 0;
                //if(count > 2)
                //{
                    if(TraceList.size() < SizeExt)
                        TraceList.push_front(localPos);
                    else
                    {
                        if(!TraceList.empty())
                        {
                            std::list<D3DVECTOR>::iterator i;
                            i = TraceList.end();
                            --i;
                            TraceList.erase(i);
                            TraceList.push_front(localPos);
                        }
                    }
                    //count = 0;
                //}
                //count++;
            }
            else //������..
            {
                if(TraceList.size() == 1)
                {
                    TraceList.push_front(LocalPos);
                }
                else if(TraceList.size() == 2)
                {
                    std::list<D3DVECTOR>::iterator i;
                    i = TraceList.begin();
                    //--i;
                    TraceList.erase(i);
                    TraceList.push_front(LocalPos);
                }
            }
        }

		// ��ƼŬ�� �̵�.
		LocalPos.x += Dir.x * timeDelta;
		LocalPos.y += Dir.y * timeDelta;
		LocalPos.z += Dir.z * timeDelta;

        Color.r += ColorStep.r * timeDelta;
		Color.g += ColorStep.g * timeDelta;
		Color.b += ColorStep.b * timeDelta;
		Color.a += ColorStep.a * timeDelta;

		Dir.x   += DirStep.x * timeDelta;
		Dir.y   += DirStep.y * timeDelta;
		Dir.z   += DirStep.z * timeDelta;

		Size	+= SizeStep * timeDelta;

        PartAngle.x += PartAngleStep.x * timeDelta;
        PartAngle.y += PartAngleStep.y * timeDelta;
        PartAngle.z += PartAngleStep.z * timeDelta;

        LocalAngle.x += LocalAngleStep.x * timeDelta;
        LocalAngle.y += LocalAngleStep.y * timeDelta;
        LocalAngle.z += LocalAngleStep.z * timeDelta;

        SizeExt += SizeExtStep * timeDelta;

        return(true); // ��ƼŬ�� ������ ��� �ִ�.
	}
};


template <class TArrayElement> class HoRecyclingArrayDyn
{
public:
  HoRecyclingArrayDyn(int iNumElements)
  {
    m_iNumElements = iNumElements;

    m_bAlive = new bool[iNumElements];
    m_aElements = new TArrayElement[iNumElements];

    for(int q=0; q < iNumElements; q++) {
      m_bAlive[q] = false;
    }

    m_iUsedElements = 0;
    m_iLastNew = 0;
  }

  virtual ~HoRecyclingArrayDyn() {
    delete[] m_bAlive;
    delete[] m_aElements;
  }

  TArrayElement *New()
  {
    // assert that we have space for this one
    if (GetNumFreeElements() < 1)
    {
        OutputDebugString("CRecyclingArrayDyn::New: too many objects!");
        return NULL;
    }
        //throw(std::out_of_range("CRecyclingArrayDyn::New: too many objects!"));

    // find first element not in use.  as an optimization, we start at
    // the position that was allocated last, in the hopes that the next position
    // will be free.

    int i = m_iLastNew;
    for (int q=0; q < m_iNumElements; q++) {
      if (!m_bAlive[i]) {
        // we've found our free spot!  use it!
        break;
      }
      else {
        i++; if (i >= m_iNumElements) i = 0;
      }
    }

    if (m_bAlive[i]) {
      // huh? if we got here, there are no free elements in the list... yet 
      // GetNumFreeElements didn't tell us that in the beginning.  Logic error.
      OutputDebugString("TArrqyElement::New(): internal logic error.");
      return NULL;
//      throw(std::logic_error("TArrayElement::New(): internal logic error."));
    }

    // clear it
    m_aElements[i] = m_NewTemplate;

    // increment used count
    m_iUsedElements++;
    m_bAlive[i] = true;
    m_iLastNew = i;
    
    // return it
    return(&m_aElements[i]);
  }

  bool Delete(int index)
  {
    if (index < 0 || index >= m_iNumElements || !m_bAlive[index]) return false;
    // don't actually delete element at index;
    // just mark it free.
    m_bAlive[index] = false;
    m_iUsedElements--;
    return true;
  }

  bool Delete(TArrayElement *elem)
  {
    if (m_iUsedElements == 0) return(false);
    int iElem = reinterpret_cast<int>(elem);
    iElem -= reinterpret_cast<int>(&m_aElements[0]);
    int index = iElem / sizeof(TArrayElement);
    if (index < 0 || index >= m_iNumElements || !m_bAlive[index]) return(false);
    m_bAlive[index] = false;
    m_iUsedElements--;
    return(true);
  }

  int GetNumFreeElements(void) { return(m_iNumElements-GetNumUsedElements()); }
  int GetNumUsedElements(void) { return(m_iUsedElements); }

  int GetTotalElements(void) { return(m_iNumElements); }

  TArrayElement &GetAt(int index) { return(m_aElements[index]); }
  bool IsAlive(int index) { return(m_bAlive[index]); }

  void DeleteAll(void)
  {
    for (int q=0; q < m_iNumElements; q++) {
      m_bAlive[q] = false;
    }
    m_iUsedElements = 0;
    m_iLastNew = 0;
  }

protected:
  int m_iNumElements;
  int m_iUsedElements;
  TArrayElement *m_aElements; 
  bool *m_bAlive;
  int m_iLastNew;
  TArrayElement m_NewTemplate;
};


//����� define ���� ���ϴ� �����尡 ���...���⿡ �����Ѵ�.
#define FLOATS_EM				(FLOATNS+3)
#define SM_DIST_NEARZ			16
#define SMFLOAT_DIST_NEARZ		(SM_DIST_NEARZ<<FLOATNS)

//EventSequence ����...
class HoNewParticleEventSequence
{
public:

	HoNewParticleEventSequence()
	{
        Particles = NULL;
		Reset();
		SetRunning(true);
		reduceParticleNum = 0;
        DelayRunningTime = 0;
        TimeCount = 0;
		FastStopFlag = false;
		FastStopStateFlag = false;
		memset(&Angle, 0, sizeof(Angle));

		//Particles = new HoRecyclingArrayDyn<HoNewParticle>(NumParticles);
	}

	virtual ~HoNewParticleEventSequence()
	{
		Reset();
	}
    std::vector<HoNewParticleEvent *> Events;

private:
	int SumCount;
	smFACE2D Face2d;
	HoMinMax<float> Lifetime;
	HoMinMax<float> EmitRate;    // in particles/sec

	int Loops;
	HoMinMax<D3DVECTOR> SpawnDir;
	HoMinMax<D3DVECTOR> EmitRadius;
	HoMinMax<D3DVECTOR> Gravity;
    std::string StrTexFileName;
	
	D3DVECTOR		  Angle;	// �ϳ��� �������� ȸ������..

	
    float DelayRunningTime;		// ó�� ���۽� ������ �ð�..
    float TimeCount;			// �ð� ī��Ʈ


    int NumParticles;
	int DestBlendMode;
	int SrcBlendMode;
	
	long int TotalParticleLives;
    std::string StrName;
    float NumNewPartsExcess;

    HoRecyclingArrayDyn<HoNewParticle> *Particles;

	int		MaterialNum;
	bool	Running;  
	int		CellSize;				//���ư��� ������ ����� ��ƼŬ�� �������� ��ĭ�� ��.. (�⺻���� 0)
    int     ParticleType;               //
    
public:
	bool	IsRunning() { return Running; }
	void	SetRunning(bool running) { Running = running; }
    void    SetTexture(const char *strTexFileName);
    void    Reset();
    void    Draw(int x, int y, int z, int ax, int ay, int az);
    void    RestoreDeviceObjects();

	BOOL	GetCameraCoord(POINT3D &world);

	void SetAngle(D3DVECTOR angle)
	{
		Angle.x = angle.x;
		Angle.y = angle.y;
		Angle.z = angle.z;
	}

    void CreateNewParticle(D3DVECTOR partSysPos)
	{
		HoNewParticle *part = Particles->New();
        part->Init();
		part->LifeTime = Lifetime.GetRandomNumInRange();

		D3DVECTOR emitRadius = EmitRadius.GetRandomNumInRange();
        part->LocalPos.x += emitRadius.x;
        part->LocalPos.y += emitRadius.y;
        part->LocalPos.z += emitRadius.z;

        if(ParticleType == TYPE_FOUR)
            part->TraceList.push_back(part->LocalPos);

        part->WorldPos.x = partSysPos.x;
		part->WorldPos.y = partSysPos.y;
		part->WorldPos.z = partSysPos.z;

        std::vector<HoNewParticleEvent *>::iterator i;
        for (i = Events.begin(); i != Events.end() && !(*i)->GetActualTime(); i++)
		{
			(*i)->DoItToIt(*part);
		}

		part->CurrentEvent = i;
		TotalParticleLives++;
	}

    smRENDFACE *AddFace2DBillBoard( smFACE2D *face , D3DVECTOR inAngle);
    smRENDFACE *AddFace2DBillBoard( smFACE2D *face);

	smRENDFACE *AddFace2dPlane(smFACE2D *face);
	smRENDFACE * AddFace2dPlane(smFACE2D *face, D3DVECTOR inAngle);

	//smRENDFACE *AddFace2DWorld( smFACE2D *face, D3DVECTOR inAngle);

    bool        AddFaceTrace(HoNewParticle *part);
    bool        AddFaceThree(smFACE2D *face, D3DVECTOR inAngle);

	smSTAGE_VERTEX		Vertex[4];
	smSTAGE_FACE		FlatFace[2];
	smTEXLINK			TexLink[2];

	
	
	int reduceParticleNum;
	bool FastStopFlag;
	bool FastStopStateFlag;
	
	//�ð� ��ȭ��...
	void Main(float fElapsedTime, float fTimeDelta, D3DVECTOR m_vPartSysPos, bool fadeStopFlag, bool attachPosFlag)
	{

        if(TimeCount > DelayRunningTime)
        {
		    if (!Particles)
			    return;
			
		 	for (int q=0; q < Particles->GetTotalElements(); q++)
            {
                if (Particles->IsAlive(q))
                {
                    HoNewParticle &part = Particles->GetAt(q);
					
					//��ƼŬ�� ���� ����� �Ұ��..
					if(FastStopFlag == true)
					{
 		 			 	 part.Age = part.LifeTime-3;
						 part.EventTimer = 3;
						 DeleteLoopEvents(part);
					}
					if(attachPosFlag == TRUE)
					{
						part.WorldPos.x = m_vPartSysPos.x;
						part.WorldPos.y = m_vPartSysPos.y;
						part.WorldPos.z = m_vPartSysPos.z;
					}

					if (!part.Main(fTimeDelta, ParticleType))
                    {
                         Particles->Delete(&part);
					    //TotalParticleLives--;
                        reduceParticleNum++;
                    }
                    else
                    {
                        // ��ƼŬ�� �߷��� �����Ѵ�.
                        D3DVECTOR gravity = Gravity.GetRandomNumInRange();
                        gravity.x *= fTimeDelta;
                        gravity.y *= fTimeDelta;
                        gravity.z *= fTimeDelta;

                        part.Dir.x += gravity.x;
                        part.Dir.y += gravity.y;
                        part.Dir.z += gravity.z;
						
						/*
						if(attachPosFlag == TRUE)
						{
							part.WorldPos.x = m_vPartSysPos.x;
							part.WorldPos.y = m_vPartSysPos.y;
							part.WorldPos.z = m_vPartSysPos.z;
						}
						*/

                        RunEvents(part);
                    }
                } // is alive
            } // next particle

			if(FastStopFlag == true)
				FastStopFlag = false;

		    float emitRateThisFrame = EmitRate.GetRandomNumInRange();
		    int numNewParts = int(emitRateThisFrame * fTimeDelta);
		    NumNewPartsExcess += (float)(emitRateThisFrame * fTimeDelta)-numNewParts;
  
		    if (NumNewPartsExcess > 1.0f)
		    {
			    numNewParts += (int)NumNewPartsExcess;
			    NumNewPartsExcess -= (int)NumNewPartsExcess;
		    }

		    if (Loops > 0 && TotalParticleLives+numNewParts > Loops * NumParticles)
		    {
			    numNewParts = (Loops*NumParticles)-TotalParticleLives;
			    if (numNewParts <= 0)
			    {
				    numNewParts = 0;

				    if(TotalParticleLives-reduceParticleNum <= 0)
				    //if(TotalParticleLives <= 0)
					    SetRunning(false);
			    }
		    }
		
		    if(fadeStopFlag == false && FastStopFlag == false)
		    {
			    for (int q=0; q < numNewParts && Particles->GetNumFreeElements(); q++)
			    {
				    try
				    {
					    CreateNewParticle(m_vPartSysPos);
				    }
			    	catch(...)
				    {
					    q = numNewParts;
				    }
			    }
		    }
		    else
		    {
			    if(TotalParticleLives-reduceParticleNum <= 0)
			    //if(TotalParticleLives <= 0)
					    SetRunning(false);
		    }
        }
        TimeCount += fTimeDelta;
	}

	//Loop �̺�Ʈ�� ���ش�..
	void DeleteLoopEvents(HoNewParticle &part)
	{
		std::vector<HoNewParticleEvent *>::iterator i;
		
		// ����ƼŬ�� �ٸ� �̺�Ʈ�� ������ ������ �Ѵ�.
        for (i = part.CurrentEvent; i != Events.end(); i++)
        {
            int eventID = (*i)->GetID();
			if(eventID == EVENT_ID_EVENTTIMER)
			{
				
 				(*i)->RestoreDeviceObjects();
				(*i)->SetID(EVENT_ID_NONE);
				
			}

        }
	}

    void RunEvents(HoNewParticle &part)
	{
		// ����ƼŬ�� �ٸ� �̺�Ʈ�� ������ ������ �Ѵ�.
        std::vector<HoNewParticleEvent *>::iterator i;
		for (i = part.CurrentEvent; i != Events.end() && (*i)->GetActualTime() <= part.EventTimer; i++)
        {
			if( (*i)->GetID() != EVENT_ID_NONE)
			{

				float oldeventtimer = part.EventTimer;
				(*i)->DoItToIt(part);
				if (part.EventTimer != oldeventtimer)
				{
					// event timer has changed, we need to recalc m_CurEvent.
					std::vector<HoNewParticleEvent *>::iterator RecalcIter;
					for (RecalcIter = Events.begin();
					RecalcIter != Events.end() && (*RecalcIter)->GetActualTime() < part.EventTimer;
					RecalcIter++);
					// set our main iterator to the recalculated iterator
					// the -1 just compensates for the i++ in the main for loop
					i = RecalcIter-1;
				}
			}
        }
		part.CurrentEvent = i;
	}

	std::string GetName(void) const { return(StrName); }
	void SetName(const std::string &data) { StrName = data; }
	
	HoMinMax<float> GetEmitRate(void) const { return(EmitRate); }
	void SetEmitRate(const HoMinMax<float> data) { EmitRate = data; }

	HoMinMax<float> GetLifetime(void) const { return(Lifetime); }
	void SetLifetime(const HoMinMax<float> data) { Lifetime = data; }

	int GetNumParticles(void) const { return(NumParticles); }
	void SetNumParticles(const int data) { NumParticles = data; }
	
	int GetLoops(void) { return(Loops); }
	void SetLoops(const int data) { Loops = data; }
	
	HoMinMax<D3DVECTOR> GetSpawnDir(void) const { return(SpawnDir); }
	void SetSpawnDir(const HoMinMax<D3DVECTOR> &data) { SpawnDir = data; }
	
	HoMinMax<D3DVECTOR> GetEmitRadius(void) const { return(EmitRadius); }
	void SetEmitRadius(const HoMinMax<D3DVECTOR> &data) { EmitRadius = data; }

	HoMinMax<D3DVECTOR> GetGravity(void) const { return(Gravity); }
	void SetGravity(const HoMinMax<D3DVECTOR> &data) { Gravity = data; }
	
	int GetSrcBlendMode(void) const { return(SrcBlendMode); }
	void SetSrcBlendMode(const int data) { SrcBlendMode = data; }

	int GetDestBlendMode(void) const { return(DestBlendMode); }
	void SetDestBlendMode(const int data) { DestBlendMode = data; }

    int GetParticleType(void) const { return(ParticleType);}
    void SetParticleType(const int data) { ParticleType = data; }

    float GetDelay(void) const { return(DelayRunningTime); }
    void  SetDelay(const float data) { DelayRunningTime = data; }

	void DeleteAllParticles(void) 
	{
		Particles->DeleteAll(); 
		TotalParticleLives = 0; 
	}

    void NailDownRandomTimes(void)
	{
		for (std::vector<HoNewParticleEvent *>::iterator i = Events.begin(); i != Events.end(); i++) 
		{
			HoMinMax<float> timeRange = (*i)->GetTimeRange();

			float timeRandomRange = (*i)->GetTimeRange().GetRandomNumInRange();
			(*i)->SetActualTime((*i)->GetTimeRange().GetRandomNumInRange());
		}
	}

    void SortEvents(void)
	{
		// sort the events by the time they occur (initial = 1st, final = last)
		// a testament to the power of STL: one line to accomplish this.  Pretty sweet, huh?
		HoNewParticleEvent_CompareFunc cf;
		std::sort(Events.begin(), Events.end(), cf);

        //�� �κ� ������ ���� �ֱ׷�����...
        /*
		#ifdef _DEBUG
			// spit out the list of events, for debugging purposes.
			OutputDebugString("\n\nSorted Events:");
			for (std::vector<HoNewParticleEvent *>::iterator i = Events.begin(); i != Events.end(); i++)
			{
				std::vector<HoNewParticleEvent *>::iterator retVal;
				char buf[256];
				_snprintf(buf, sizeof(buf), "\nTime: %0.2f Type: %20s Fade: %c",
				(*i)->GetActualTime(), typeid(*(*i)).name(), (*i)->IsFade() ? 'Y' : 'N');
				OutputDebugString(buf);
			}

		#endif
		*/
	}
	

	void HoNewParticleEventSequence::CreateFadeLists()
	{
	    // for each event,
		for (std::vector<HoNewParticleEvent *>::iterator i = Events.begin(); i != Events.end(); i++)
		{
			// try to find the next fade event of the same type.
			for (std::vector<HoNewParticleEvent *>::iterator j = i; j != Events.end(); j++)
			{
				
				//if (j != i && typeid(**j) == typeid(**i) && (*j)->IsFade())
				//{
				
				if( j != i && (*j)->IsFade())
				{
					if( (*j)->GetID() == (*i)->GetID())
					{
						// we've found a fade event further in the future.  make a note that
						// this event needs to be linked to this future fade event (so that we
						// can calculate the deltas later).
						(*i)->NextFadeEvent = (*j);
						break;
					}
				}
			}
		}
	}

    //���ҽ����� ��ȿȭ ��Ų��.
	bool InvalidateDeviceObjects()
	{
		if(Particles)
		{
			delete (Particles);
			(Particles) = NULL;
		}
		

		for (std::vector<HoNewParticleEvent *>::iterator i = Events.begin(); i != Events.end(); i++)
		{
			(*i)->InvalidateDeviceObjects();
		}
		return true;
	}

	
};





typedef struct
{
  char name[25];
  int mode;
} BLENDINGMODE;

typedef struct
{
    char name[25];
    int mode;
} PARTICLETYPE;

enum TokenType
{
  RealNumber = 1, 
  Equals, Comma, OpenParen, CloseParen, OpenBrace, CloseBrace, Quote, 

  KeywordParticleSystem, KeywordEventSequence, KeywordEventSequenceDelay,
  KeywordRandom, KeywordXYZ, KeywordColor, KeywordTexture, AlphaBlendMode,
  KeywordInLength, KeywordOutLength,

  // system property tokens
  SysVectorProp, SeqAlphaBlendModeProp, SeqParticleProp, ParticleTypeMode,
  
  // event time tokens
  KeywordFade, KeywordSo, KeywordAt, KeywordInitial, KeywordFinal,
  
  // particle properties (event types)
  ParticleNumericProp, ParticleVectorProp, ParticleColorProp, ParticleAngleProp,

  // event sequence props
  SeqNumericProp, SeqVectorProp, 
  
  UnknownToken
};



class HoNewParticleEmitterToken
{
public:
	std::string StrValue;
	enum TokenType Type;

	bool IsEmitRate(void)      { return(StrValue.find("EMITRATE")        != std::string::npos); }
	bool IsPosition(void)      { return(StrValue.find("POSITION")        != std::string::npos); }
	bool IsSpawnDir(void)      { return(StrValue.find("SPAWNDIR")        != std::string::npos); }
	bool IsEmitRadius(void)    { return(StrValue.find("EMITRADIUS")      != std::string::npos); }
	bool IsLifeTime(void)      { return(StrValue.find("LIFETIME")        != std::string::npos); }
	bool IsNumParticles(void)  { return(StrValue.find("NUMPARTICLES")    != std::string::npos); }
	bool IsLoops(void)         { return(StrValue.find("LOOPS")           != std::string::npos); }
	bool IsGravity(void)       { return(StrValue.find("GRAVITY")         != std::string::npos); }
    bool IsDelay(void)         { return(StrValue.find("DELAY")           != std::string::npos); }
    /*
	bool IsSrcBlendMode(void)  { return(StrValue.find("SOURCEBLENDMODE") != std::string::npos); }
	bool IsDestBlendMode(void) { return(StrValue.find("DESTBLENDMODE")   != std::string::npos); }
    */
    bool IsBlendMode(void)     { return(StrValue.find("SOURCEBLENDMODE") != std::string::npos); }
	//bool IsCellSize(void)      { return(StrValue.find("CELLSIZE")        != std::string::npos); }
    bool IsParticleType(void)  { return(StrValue.find("PARTICLETYPE")    != std::string::npos); }

    std::string TypeAsString(void)
	{
		switch(Type)
		{
			case RealNumber: return("RealNumber");
			case Equals: return("Equals");
			case Comma: return("Comma");
			case OpenParen: return("OpenParen");
			case CloseParen: return("CloseParen");
			case OpenBrace: return("OpenBrace");
			case CloseBrace: return("CloseBrace");
			case Quote: return("Quote");

			case KeywordParticleSystem: return("KeywordParticleSystem");
			case KeywordEventSequence: return("KeywordEventSequence");
			case KeywordRandom: return("KeywordRandom");
			case KeywordXYZ: return("KeywordXYZ");
			case KeywordColor: return("KeywordColor");
			case KeywordTexture: return("KeywordTexture");

			case SysVectorProp: return("SysVectorProp");
			case SeqAlphaBlendModeProp: return("SeqAlphaBlendModeProp");
			case AlphaBlendMode: return("AlphaBlendMode");
            
			case KeywordFade: return("KeywordFade");
			case KeywordSo: return("KeywordSo");
			case KeywordAt: return("KeywordAt");
			case KeywordInitial: return("KeywordInitial");
			case KeywordFinal: return("KeywordFinal");

			case ParticleNumericProp: return("ParticleNumericProp");
			case ParticleVectorProp: return("ParticleVectorProp");
			case ParticleColorProp: return("ParticleColorProp");

			case SeqNumericProp: return("SeqNumericProp");
			case SeqVectorProp: return("SeqVectorProp");

		}
		return("**Unknown!**");
	}
};


#define BLEND_NONE              SMMAT_BLEND_NONE
#define BLEND_ALPHA             SMMAT_BLEND_ALPHA
#define BLEND_COLOR             SMMAT_BLEND_COLOR
#define BLEND_SHADOW            SMMAT_BLEND_SHADOW
#define BLEND_LAMP              SMMAT_BLEND_LAMP
#define BLEND_ADDCOLOR          SMMAT_BLEND_ADDCOLOR
#define BLEND_INVSHADOW         SMMAT_BLEND_INVSHADOW

class HoNewParticleEmitterTokenizer
{
public:
  static const int NUMBLENDINGMODES;
  static const BLENDINGMODE BlendingModes[6];
  
  static const int NUMPARTICLETYPES;
  static const PARTICLETYPE ParticleTypes[4];

  std::vector<HoNewParticleEmitterToken> TokenVector;
  
  void Tokenize(const char *str);

  static bool ProcessAlphaBlendMode(int &alphablendmode, 
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  static bool ProcessParticleTypeMode(int &particleTypeMode,
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  static bool ProcessColor(HoMinMax<D3DCOLORVALUE> &c,
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  static bool ProcessTime(HoMinMax<float> &TimeRange, bool &IsFade, 
    const float InitialTime, const float FinalTime,
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  static bool ProcessVector(HoMinMax<D3DVECTOR> &v, 
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  static bool ProcessNumber(HoMinMax<float> &number, 
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

  static void NextToken(std::vector<HoNewParticleEmitterToken>::iterator &TokenIter, 
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter) 
  {
		if (TokenIter == EndIter)
            OutputDebugString("Unexpected end-of-file.");
		TokenIter++;
  }

  

private:
  void AddToken(HoNewParticleEmitterToken token);
  void DetermineTokenType(HoNewParticleEmitterToken &token);

};

class HoNewParticleScriptRes;
class HoNewParticleEmitter
{
public:
	bool				Running;	  //��ƼŬ�� ��縦 �ؾ� �ϴ��� ���ƾ� �ϴ���.
	bool				AttachPosFlag;//��ƼŬ�� ��ġ�� ���� ������..
	bool                FadeStopFlag; //���� ���� �ִ� ��ƼŬ�� �� ��� �����͸� ����.
	bool				FastStopFlag; //���� ���� �ִ� ��ƼŬ�� ���� ����.
	bool				FastStopStateFlag;
	

private:
	std::string			StrLastError;

	std::string			StrName;
	
	HoMinMax<D3DVECTOR>	PosRange;	//��ƼŬ�� ����� ��ġ�� �ּ�, �ִ밪.			 
	D3DVECTOR			Pos;		// ��ƼŬ�� ����� ��ġ
	
	float				StartDelay; //���۽� ������..
	BOOL				Rendering;
	
	D3DVECTOR			Angle;		//��ƼŬ�������� ����..
public:
	HoNewParticleEmitter() {}
	virtual ~HoNewParticleEmitter() {}
	virtual void init();
	
	
	HoMinMax<D3DVECTOR> GetPosRange(void) const { return(PosRange); }
	void SetPosRange(const HoMinMax<D3DVECTOR> &data) { PosRange = data; }

	//����� ��ġ ����..
	D3DVECTOR GetPos(void) const { return(Pos); }
	void SetPos(const D3DVECTOR &data) 
	{
#ifndef _HO_WIN_32_
		Pos = data;
#endif

#ifdef _HO_WIN_32_
		Pos.x = float(data.x/fONE);
		Pos.y = float(data.y/fONE);
		Pos.z = float(data.z/fONE);
#endif
		AttachPosFlag = FALSE;
	}

	void SetAttachPos(const D3DVECTOR &data)
	{
#ifndef _HO_WIN_32_
		Pos = data;
#endif

#ifdef _HO_WIN_32_
		Pos.x = float(data.x/fONE);
		Pos.y = float(data.y/fONE);
		Pos.z = float(data.z/fONE);
#endif
		AttachPosFlag = TRUE;
	}
	
	virtual void FadeStop() { FadeStopFlag = true; }
	virtual void FastStop() { 
		FadeStopFlag = true;
		FastStopFlag = true; 
	}
	virtual void SetRendering(BOOL flag)
	{
		Rendering = flag;
	}

	virtual void Start(float startDelay = 0.f) 
	{ 
		StartDelay = startDelay;
		Running = true; 
	}
	virtual void Pause() { Running = false; }
	virtual void Stop()
	{ 
		Pause(); 
		DeleteAllParticles(); 
	}

	void SetAngle(D3DVECTOR angle)
	{
		Angle.x = angle.x;
		Angle.y = angle.y;
		Angle.z = angle.z;
	}
	
	virtual bool IsRunning()
	{
		return(Running);
	}
	virtual BOOL IsRendering()
	{
		return(Rendering);
	}

	//virtual bool Compile(const char *strScript);
	virtual void Main(float elapsedTime, float timeDelta);
    bool AddScript(HoNewParticleScriptRes *scrRes);

	void Draw(int x, int y, int z, int ax, int ay, int az)
	{
		if (IsRunning()) 
		{
			if(IsRendering())
			{
				for (std::vector<HoNewParticleEventSequence *>::iterator i = Sequences.begin(); i != Sequences.end(); i++) 
				{
					(*i)->Draw(x, y, z, ax, ay, az);
				}
			}
		}
	}

    /*
    bool AddToken(std::vector<HoNewParticleEmitterTokenizer> *token)
    {
       StrLastError = "Script Starting";


	    //HoNewParticleEmitterTokenizer *tokenizer;
        InvalidateDeviceObjects();
        init();

        std::vector<HoNewParticleEmitterToken>::iterator TokenIter = token->begin();
        std::vector<HoNewParticleEmitterToken>::iterator EndIter = token->end();

	    if (token->TokenVector.size() < 2)
	    	return false;


    	if (token->Type != KeywordParticleSystem)
		    return false;

	    HoNewParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
        if (token->Type != Quote)
		    return false;

	    StrName = RemoveQuotes(TokenIter->StrValue);
        HoNewParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	    if (token->Type != RealNumber)
		    return false;

	    HoNewParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
        if (token->Type != OpenBrace)
		    return false;

	    if(!ProcessParticleSystemBlock(TokenIter, EndIter))
		    return false;

	    Pos = PosRange.GetRandomNumInRange();
        RestoreDeviceObjects();

        return true;

    }
    */
    

    bool Compile(const char *strScript)
    {
	    StrLastError = "Script Starting";


	    HoNewParticleEmitterTokenizer tokenizer;
        InvalidateDeviceObjects();
        init();

        tokenizer.Tokenize(strScript);
        //tokenizer = g_NewParticleResMgr.AddScript(strScript);

	    std::vector<HoNewParticleEmitterToken>::iterator TokenIter = tokenizer.TokenVector.begin();
        std::vector<HoNewParticleEmitterToken>::iterator EndIter = tokenizer.TokenVector.end();

	    if (tokenizer.TokenVector.size() < 2)
	    	return false;


    	if (TokenIter->Type != KeywordParticleSystem)
		    return false;

	    HoNewParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
        if (TokenIter->Type != Quote)
		    return false;

	    StrName = RemoveQuotes(TokenIter->StrValue);
        HoNewParticleEmitterTokenizer::NextToken(TokenIter, EndIter);

	    if (TokenIter->Type != RealNumber)
		    return false;

	    HoNewParticleEmitterTokenizer::NextToken(TokenIter, EndIter);
        if (TokenIter->Type != OpenBrace)
		    return false;

	    if(!ProcessParticleSystemBlock(TokenIter, EndIter))
		    return false;

	    Pos = PosRange.GetRandomNumInRange();


        RestoreDeviceObjects();

        return true;
    }


private:
	virtual void InvalidateDeviceObjects(); //���ҵ��� ���� ��ȿȭ ��Ű�� �Լ�
	virtual void RestoreDeviceObjects();


	std::vector<HoNewParticleEventSequence *> Sequences;	//Emitter ������ ����.

	
	void DeleteAllParticles(void)
	{
		for (std::vector<HoNewParticleEventSequence *>::iterator i = Sequences.begin(); i != Sequences.end(); i++)
		{
			(*i)->DeleteAllParticles();
		}
	}

	bool ProcessParticleSystemBlock(
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

	bool ProcessEventSequenceBlock(HoNewParticleEventSequence &seq,
    std::vector<HoNewParticleEmitterToken>::iterator &TokenIter,
    std::vector<HoNewParticleEmitterToken>::iterator &EndIter);

	

	static HoNewParticleEvent *HoNewParticleEmitter::EventFactory(std::string EventName);
};


//extern HoNewParticleEmitter g_NewParticleEmitter;

#endif
