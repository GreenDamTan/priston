
struct	sITEM_CREATE {
	DWORD	Head;			//���
	DWORD	dwVersion;		//���� ����
	DWORD	dwTime;			//�����ð�
	DWORD	dwChkSum;		//������ üũ��
};

struct sITEM_SPECIAL {

	//�׳� �߰��Ǵ� ��
	int		Add_Absorb;				//����� 
  	int     Add_Defence;			//���� 
	float   Add_fSpeed;				//�̵� �ӵ� 
	float   Add_fBlock_Rating;		//(����)���� 
	int     Add_Attack_Speed;		//���ݼӵ�
	int     Add_Critical_Hit;		//1.5�� ������Ȯ��
	int     Add_Shooting_Range;		//�����Ÿ� 
	float   Add_fMagic_Mastery;     //�������õ� 
	short	Add_Resistance[8];		//���ҿ����� ���׷� 

	////////////////////////////////////////////////

	short	Lev_Attack_Resistance[8];	//���ҿ����� ���ݷ�
	int		Lev_Mana;				//����ȸ��(�ּ�)(�ִ�)
	int		Lev_Life;				//������ȸ��(�ּ�)(�ִ�) 
	int     Lev_Attack_Rating;		//���߷� 
	short	Lev_Damage[2];			//���ݷ�

	int		Per_Mana_Regen;			//���� ��� 
	int		Per_Life_Regen;			//������ ��� 
	int		Per_Stamina_Regen;		//���׹̳� ���

	//////////////////////////////////////////////////

	DWORD	dwTemp[32];
};


struct sITEMINFO{

	DWORD	dwSize;						//������ ���� ũ��
	
	sITEM_CREATE	ItemHeader;			//������ ���� ����

	short	Durability[2];				//������

	//////// ���� ������ ����Ÿ������ ���ϴ� /////////

	/*-------------------------*
	*		 ������ ����	
	*--------------------------*/
	DWORD	CODE;				//������ �ڵ� 	
	char	ItemName[32];		//������ �̸� 	

	/*-------------------------*
	*		���� ���� 	
	*--------------------------*/
	int     Weight;				//���� 		
	int     Price;				//����

	/////////////////////////////////////////
	int		Index;				//�ε���
	int		Counter;			//ī����
	////////////////////////////////////////

	/**����**/
	short	Resistance[8];		//���ҿ����� ���׷� 
	/********/

	int		Sight;				//�þ�

	DWORD	Temp0;

	/*-------------------------*
	*		 ���ݼ��� 
	*--------------------------*/
	short	Damage[2];			//���ݷ� 
	int     Shooting_Range;		//�����Ÿ� 
	int     Attack_Speed;		//���ݼӵ� 
	int     Attack_Rating;		//���߷� 
	int     Critical_Hit;		//1.5�� ������Ȯ��  

	/*-------------------------*
	*		 ���� 
	*--------------------------*/

	int		Absorb;				//����� 
  	int     Defence;			//���� 
	float   fBlock_Rating;		//(����)���� 

	/*-------------------------*
	*		 �̵����� 
	*--------------------------*/
	float     fSpeed;				//�̵� �ӵ� 

	/*-------------------------*
	*		 ���� ���� 
	*--------------------------*/

	int		Potion_Space;		//���� ���� �Ҵ� 
	
	/**Ư���ɷ�**/
	float   fMagic_Mastery;     //�������õ� 
	float   fMana_Regen;		//���� ��� 
	float   fLife_Regen;		//������ ��� 
	float   fStamina_Regen;	    //���׹̳� ��� 
	float   fIncrease_Life;     //������ �ִ�ġ��� 
	float   fIncrease_Mana;     //���� �ִ�ġ���
	float   fIncrease_Stamina;  //���׹̳� �ִ�ġ���

	/************/

	/*-------------------------*
	*		�䱸Ư�� 
	*--------------------------*/
	int     Level;				//���� 
	int     Strength;			//�� 	
	int     Spirit;				//���ŷ� 
	int     Talent;				//��� 
	int     Dexterity;			//��ø�� 
	int     Health;				//�ǰ� 

	/*-------------------------*
	*		 ȸ���� 
	*--------------------------*/
	short   Mana[2];            //����ȸ��(�ּ�)(�ִ�)
	short   Life[2];            //������ȸ��(�ּ�)(�ִ�) 
	short   Stamina[2];         //���׹̳�ȸ��(�ּ�)(�ִ�)
	

	/*-------------------------*
	*		 .....
	*--------------------------*/
	int		Money;				//������ ��


	DWORD	dwTemp1[8];			//����

	DWORD	DispEffect;			//������ ǥ�� ����Ʈ (����)

	/*-------------------------*
	*		ĳ���� Ưȭ������    (���� �̼���)
	*--------------------------*/

	DWORD			JobCodeMask;		//Ưȭ ����	(������ ��Ʈ ����ũ)
	sITEM_SPECIAL	JobItem;			//Ưȭ ������ ����

	DWORD	dwTemp[16];			//����
};


