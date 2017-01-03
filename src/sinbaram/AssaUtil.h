/*----------------------------------------------------------------------------*
*	���ϸ� :  AssaUtil
*	�ϴ��� :  �̰����� ����Ѱ� 
*	�ۼ��� :  ����������Ʈ 2002�� 12��
*	������ :  �ڻ� & ȣ����
*-----------------------------------------------------------------------------*/	
#ifndef _ASSAUTIL_HEADER_

#define _ASSAUTIL_HEADER_

char *FindFile(char *fileName); //ã�����ϴ� ������ ��θ� ���Ϲ޴´� 


void InitAssaRes();				//���ҽ��� ��ó�� �ѹ� �ʱ�ȭ �ϴ� �Լ�..
void CloseAssaRes();			//���ҽ��� �����ϴ� �Լ�.
void LoadAssaRes();				//���ҽ��� �д� �Լ�.

void *AssaSearchRes(char *resName, int blendType); //���ҽ��� ã�� �Լ�..(�ַ� ���� �Լ��� �̰�)


#include <list>
using namespace std;

//3D��θ� �����ϴ� Ŭ����..
class cASSATrace
{
public:
	list<POINT3D> TraceList;
private:
	unsigned int Length;		//����Ʈ�� ũ��.

public:
	cASSATrace()
	{
		Length = 50;	
	}

	~cASSATrace()
	{
			
	}
	
	//����Ʈ�� ũ�⸦ �����ִ� �Լ�.
	void SetLength(int length)
	{
		Length = length;	
	}

	//����Ʈ�� ����Ÿ�� �߰�..
	void AddData(POINT3D *pos)
	{
		//Length = 50;
		POINT3D curPos;
		curPos.x = pos->x;
		curPos.y = pos->y;
		curPos.z = pos->z;
		
		int t = TraceList.size();
		if(TraceList.size() < Length)
			TraceList.push_front(curPos);
		else
        {
			if(!TraceList.empty())
            {
				std::list<POINT3D>::iterator i;
				i = TraceList.end();
                --i;
                TraceList.erase(i);
				TraceList.push_front(curPos);
			}
		}
	}

};

bool	AssaAddFace2D ( smFACE2D *face , int ZPosiFlag = FALSE, int angle = 0);
bool	AssaAddFace2DLine(smFACE2D *face, POINT3D *localDestPos,BOOL centerFlag = FALSE);//destPos ��ġ���� �÷��� �׸��� ������..
bool	AssaAddFaceWorld(smFACE2D *face, POINT3D *inAngle = NULL);
bool	AssaAddFaceTrace(smFACE2D *face, cASSATrace *assaTrace);

bool    AssaSearchObjPos(smCHAR *character, smOBJ3D *obj, POINT3D *outObjPos);
bool	AssaSearchObjPos(smCHAR *character, smOBJ3D *obj, POINT3D *outObjPos, int length);
bool    AssaSearchWeaponPos(smCHAR *character, POINT3D *outWeaponPos, float u = 0.0f);

int AssaTestEffectImage();      //�׽�Ʈ�� ���� INI������ �о� �����Ѵ�
#endif