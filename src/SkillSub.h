
//��ų�� �����Ѵ�
int OpenPlaySkill( sSKILL *lpSkill );
//��ų ������ �õ�
int PlaySkillAttack( sSKILL *lpSkill , smCHAR *lpChar );
//��ų ���� �Ÿ��� ���Ѵ�
int GetSkillDistRange( sSKILL *lpSkill , int AttackRange , int AttackMode );
//��ų ������ �õ�
int PlaySkillLoop( int SkillCode , smCHAR *lpChar );

//�ܻ�� ��ų���� ����
int SetSkillMotionBlurColor( int dwSkillCode );
//���� ��ų Ȯ��
int GetSkillPower( smCHAR *lpChar , smCHAR *lpChar2 , int dx, int dy, int dz );
//��ų ���� ����Ʈ
int	LearnSkillEffect( smCHAR *lpChar=0 );
//��ų����
int CheckPlaySkill();
