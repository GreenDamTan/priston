/*----------------------------------------------------------------------------*
*	���ϸ� :  sinUtil.h
*	�ϴ��� :  �ϴ��� ����
*	�ۼ��� :  ����������Ʈ 2004�� 11��
*	������ :  �ڻ� 
*-----------------------------------------------------------------------------*/	
#ifndef _SINUTIL_HEADER_ 
#define _SINUTIL_HEADER_ 
/*----------------------------------------------------------------------------*
*						���ڿ� Ŭ���� 
*-----------------------------------------------------------------------------*/	
class sinSTRING{
private:

char			*m_aString;

public:
sinSTRING(void);
sinSTRING(sinSTRING &rString);
sinSTRING(char *pString);

~sinSTRING(void);

//Operators
sinSTRING & operator = (sinSTRING &rString);
sinSTRING & operator = (char *pString);
bool operator == (sinSTRING &rString);
bool operator == (char *pString);
bool operator != (sinSTRING &rString);
bool operator != (char *pString);

//Functions
char *GetString(void);
int  GetLength(void);
};

//
int halpRelease(DIRECTDRAWSURFACE lpTemp);
int haSearchString(char *strBuffer,char KeyBuffer);
int haStringCenter(int PosiX,char *strbuff);


#endif