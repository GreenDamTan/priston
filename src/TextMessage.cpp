#include "TextMessage.h"

#include "Language\\language.h"

#ifdef	_LANGUAGE_KOREAN
//////////////////// �ѱ��� - �⺻ ////////////////////////////
#include "Language\\Korean\\k_TextMessage.h"
#include "Language\\Korean\\k_QuestMsg.h"
#endif

#ifdef	_LANGUAGE_CHINESE
//�߱���
#include "Language\\Chinese\\C_TextMessage.h"
#include "Language\\Chinese\\C_QuestMsg.h"
#endif

#ifdef	_LANGUAGE_JAPANESE
//�Ϻ���
#include "Language\\Japanese\\J_TextMessage.h"
#include "Language\\Japanese\\J_QuestMsg.h"
#endif

#ifdef	_LANGUAGE_TAIWAN
//�����߱�
#include "Language\\Taiwan\\T_TextMessage.h"
#include "Language\\Taiwan\\T_QuestMsg.h"
#endif

#ifdef	_LANGUAGE_ENGLISH
//������
#include "Language\\English\\E_TextMessage.h"
#include "Language\\English\\E_QuestMsg.h"
#endif

#ifdef	_LANGUAGE_THAI
//�±���
#include "Language\\THAI\\Th_TextMessage.h"
#include "Language\\THAI\\Th_QuestMsg.h"
#endif

//��Ʈ����
#ifdef _LANGUAGE_VEITNAM
#include "Language\\VEITNAM\\V_TextMessage.h"
#include "Language\\VEITNAM\\V_QuestMsg.h"
#endif

//�������
#ifdef _LANGUAGE_BRAZIL
#include "Language\\BRAZIL\\B_TextMessage.h"
#include "Language\\BRAZIL\\B_QuestMsg.h"
#endif

//�Ƹ���Ƽ����
#ifdef _LANGUAGE_ARGENTINA
#include "Language\\ARGENTINA\\A_TextMessage.h"
#include "Language\\ARGENTINA\\A_QuestMsg.h"
#endif