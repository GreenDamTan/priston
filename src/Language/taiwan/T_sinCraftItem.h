//// ���s�զX�D��������

#define   SIN_ADD_FIRE			0x00000001	//��
#define   SIN_ADD_ICE			0x00000002	//�B
#define   SIN_ADD_LIGHTNING		0x00000004	//�p
#define   SIN_ADD_POISON			0x00000008	//�r
#define   SIN_ADD_BIO			0x00000010	//��
#define   SIN_ADD_CRITICAL			0x00000020	//�@�[��
#define   SIN_ADD_ATTACK_RATE		0x00000040	//�R���v
#define   SIN_ADD_DAMAGE_MIN		0x00000080	//�̤p�����O
#define   SIN_ADD_DAMAGE_MAX		0x00000100	//�̤j�����O
#define   SIN_ADD_ATTACK_SPEED		0x00000200	//�����t��
#define   SIN_ADD_ABSORB			0x00000400	//�B�~���m�O
#define   SIN_ADD_DEFENCE			0x00000800	//���m�O
#define   SIN_ADD_BLOCK_RATE		0x00001000	//��ײv
#define   SIN_ADD_MOVE_SPEED		0x00002000	//���ʳt��
#define   SIN_ADD_LIFE			0x00004000	//�ͩR��
#define   SIN_ADD_MANA			0x00008000	//�]�k��
#define   SIN_ADD_STAMINA			0x00010000	//��O��
#define   SIN_ADD_LIFEREGEN		0x00020000 	 //�ͩR�ȦA��
#define   SIN_ADD_MANAREGEN		0x00040000 	 //�]�k�ȦA��
#define   SIN_ADD_STAMINAREGEN		0x00080000	  //��O�ȦA��

#define   SIN_ADD_NUM				1
#define   SIN_ADD_PERCENT			2


//���~�� 
//�Z��		(�@8��)
//{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
//����		(�@9��)
//{3,2,3,0,0,0,0,0,0},
//�l�[�n��	(�{�b�R���v,�̤j�����O) (�@8��)
//{SIN_ADD_ATTACK_RATE ,SIN_ADD_DAMAGE_MAX ,0,0,0,0,0,0},
//�l�[�ƭ�
//{30,20,0,0,0,0,0,0},
//�ƭȼз�
//{SIN_ADD_NUM,SIN_ADD_PERCENT,0,0,0,0,0,0},
//"�@�[�� +1 �̤j/�̤p �����O�A�U +1"}},

//1
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{5,0,0,0,0,0,0,0,0},
{SIN_ADD_BIO,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���ݩ� +5\r"},

//2
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,5,0,0,0,0,0,0,0},
{SIN_ADD_POISON,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�r�ݩ� +5\r"},

//3
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,5,0,0,0,0,0,0},
{SIN_ADD_ICE,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B�ݩ� +5\r"},

//4
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,5,0,0,0,0,0},
{SIN_ADD_FIRE,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���ݩ� +5\r"},

//5
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,0,5,0,0,0,0},
{SIN_ADD_LIGHTNING,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�p�ݩ� +5\r"},

//6
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,0,0,1,0,0,0},
{SIN_ADD_CRITICAL,0,0,0,0,0,0,0},
{1,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�@�[�� +1%\r"},

//7
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,0,0,0,1,0,0},
{SIN_ADD_ATTACK_RATE,0,0,0,0,0,0,0},
{50,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�R���v +40\r"},

//8
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,0,0,0,0,1,0},
{SIN_ADD_STAMINA,0,0,0,0,0,0,0},
{20,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"��O�� +20\r"},

//9
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,0,0,0,0,0,1},
{SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0,0},
{2,2,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�̤p�����O +2\r�̤j�����O +2\r"},

//10
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{1,1,0,0,0,0,0,0,0},
{SIN_ADD_ATTACK_RATE,0,0,0,0,0,0,0},
{20,0,0,0,0,0,0,0},
{SIN_ADD_PERCENT,0,0,0,0,0,0,0},
"�R���v +20%\r"},

//11
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{3,3,0,0,0,0,0,0,0},
{SIN_ADD_ATTACK_RATE,0,0,0,0,0,0,0},
{40,0,0,0,0,0,0,0},
{SIN_ADD_PERCENT,0,0,0,0,0,0,0},
"�R���v +40%\r"},

//12
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{1,0,2,0,0,0,0,0,0},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MIN,0,0,0,0,0,0},
{10,1,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�R���v +10\r�̤p�����O +1\r"},

//13
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{1,0,0,2,0,0,0,0,0},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MAX,0,0,0,0,0,0},
{10,1,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�R���v +10\r�̤j�����O +1\r"},

//14
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{2,0,2,2,0,0,0,0,0},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0},
{20,1,1,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"�R���v +20\r�̤p�����O +1\r�̤j�����O +1\r"},

//15
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{1,0,0,1,1,0,0,0,0},
{SIN_ADD_ATTACK_RATE,0,0,0,0,0,0,0},
{100,0,0,0,0,0,0,0},
{SIN_ADD_PERCENT,0,0,0,0,0,0,0},
"�R���v +100%\r"},

//16
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,1,2,0,0,0,0,0,0},
{SIN_ADD_DAMAGE_MIN,0,0,0,0,0,0,0},
{1,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�̤p�����O +1\r"},

//17
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,1,0,2,0,0,0,0,0},
{SIN_ADD_DAMAGE_MAX,0,0,0,0,0,0,0},
{2,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�̤j�����O +2\r"},


//18
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,1,2,2,0,1,0,0,0},
{SIN_ADD_CRITICAL,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0},
{2,1,1,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"�@�[�� +2%\r�̤p�����O +1\r�̤j�����O +1\r"},

//19
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,1,0,4,4,0,0,0,0},
{SIN_ADD_DAMAGE_MAX,0,0,0,0,0,0,0},
{4,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�̤j�����O +4\r"},

//20
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{1,1,0,4,4,0,0,0,0},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MAX,0,0,0,0,0,0},
{10,4,0,0,0,0,0,0},
{SIN_ADD_PERCENT,SIN_ADD_NUM,0,0,0,0,0,0},
"�R���v +10%\r�̤j�����O +4\r"},

//21
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,1,2,4,1,0,0,0,0},
{SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0,0},
{1,3,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�̤p�����O +1\r�̤j�����O +3\r"},

//22
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,1,4,2,1,0,0,0,0},
{SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0,0},
{2,2,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�̤p�����O +2\r�̤j�����O +2\r"},

//23
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,2,2,1,4,0,0,0},
{SIN_ADD_CRITICAL,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0},
{4,2,2,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"�@�[�� +4%\r�̤p�����O +2\r�̤j�����O +2\r"},

//24
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{1,1,2,4,1,0,0,0,0},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0},
{10,1,4,0,0,0,0,0},
{SIN_ADD_PERCENT,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"�R���v +10%\r�̤p�����O +1\r�̤j�����O +4\r"},

//25
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,1,3,3,0,0,1,0,0},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0},
{40,3,3,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"�R���v+40\r�̤p�����O +3\r�̤j�����O +3\r"},

//26
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,0,5,5,2,0,0},
{SIN_ADD_CRITICAL,SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MAX,0,0,0,0,0},
{4,60,5,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_PERCENT,SIN_ADD_NUM,0,0,0,0,0},
"�@�[�� +4%\r�R���v +60%\r�̤j�����O +5\r"},

//27
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,3,3,2,2,2,0,0},
{SIN_ADD_ATTACK_RATE,SIN_ADD_CRITICAL,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0},
{80,2,3,3,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0},
"�R���v +80\r�@�[�� +2%\r�̤p�����O +3\r�̤j�����O +3\r"},

//28
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,4,4,0,2,0,2,0},
{SIN_ADD_CRITICAL,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,SIN_ADD_MANA,0,0,0,0},
{4,4,4,20,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0},
"�@�[�� +4%\r�̤p�����O +4\r�̤j�����O +4\r�]�k�� +20\r"},

//29
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,2,2,0,0,1,4,0},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,SIN_ADD_LIFE,0,0,0,0},
{60,4,4,20,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0},
"�R���v +60\r�̤p�����O +4\r�̤j�����O +4\r�ͩR +20\r"},

//30
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,2,1,4,0,0,2},
{SIN_ADD_CRITICAL,SIN_ADD_DAMAGE_MAX,0,0,0,0,0,0},
{4,15,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_PERCENT,SIN_ADD_PERCENT,0,0,0,0,0},
"�@�[�� +4%\r�̤j�����O +15%\r"},

//31
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,1,1,3,0,0,0,2},
{SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0,0},
{6,6,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�̤p�����O +6\r�̤j�����O +6\r"},

//32
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,1,1,4,0,1,0,4},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,0,0,0,0,0},
{160,4,4,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"�R���v +160\r�̤p�����O +4\r�̤j�����O +4\r"},

//33
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,0,0,3,0,5,2},
{SIN_ADD_CRITICAL,SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,SIN_ADD_LIFE,0,0,0,0},
{3,9,9,25,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_PERCENT,SIN_ADD_PERCENT,SIN_ADD_NUM,0,0,0,0},
"�@�[�� +3%\r�̤p�����O +9%\r�̤j�����O +9%\r�ͩR +25\r"},

//34
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,0,0,0,4,4,4},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MAX,SIN_ADD_STAMINA,0,0,0,0,0},
{80,12,40,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"�R���v +80\r�̤j�����O +12\r�@�O +40\r"},

//35
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{0,0,0,0,5,0,0,2,2},
{SIN_ADD_DAMAGE_MIN,SIN_ADD_DAMAGE_MAX,SIN_ADD_MANA,0,0,0,0,0},
{7,7,10,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"�̤p�����O +7\r�̤j�����O +7\r�]�k��+10\r"},

//36
{{sinWA1,sinWC1,sinWH1,sinWM1,sinWP1,sinWS1,sinWS2,sinWT1},
{1,1,1,1,1,1,1,1,1},
{SIN_ADD_ATTACK_RATE,SIN_ADD_DAMAGE_MAX,SIN_ADD_MANA,0,0,0,0,0},
{50,5,10,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0},
"�R���v +50\r�̤j�����O +5\r�]�k�� +10\r"},

// ���� ----------------------------------------------------------------------------------------------------

//1
{{sinDA1,sinDA2,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0,0},
{SIN_ADD_BIO,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�]���m +5\r"},

//2
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,5,0,0,0,0,0,0,0},
{SIN_ADD_POISON,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�r���m +5\r"},

//3
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,5,0,0,0,0,0,0},
{SIN_ADD_ICE,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B���m +5\r"},

//4
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,5,0,0,0,0,0},
{SIN_ADD_FIRE,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�����m +5\r"},

//5
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,5,0,0,0,0},
{SIN_ADD_LIGHTNING,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�p���m +5\r"},

//6
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,0,1,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{20,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +20\r"},

//7
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,0},
{SIN_ADD_ABSORB,0,0,0,0,0,0,0},
{0.6f,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B�� +0.6\r"},

//8
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,0},
{SIN_ADD_STAMINA,0,0,0,0,0,0,0},
{10,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�@�O +10\r"},

//9
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{40,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +40\r"},

//10
{{sinDA1,sinDA2,0,0,0,0,0,0},
{2,2,0,0,0,0,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{10,0,0,0,0,0,0,0},
{SIN_ADD_PERCENT,0,0,0,0,0,0,0},
"���m +10%\r"},

//11
{{sinDA1,sinDA2,0,0,0,0,0,0},
{1,0,2,0,0,0,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{20,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +20\r"},

//12
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,2,1,1,0,0,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{15,0,0,0,0,0,0,0},
{SIN_ADD_PERCENT,0,0,0,0,0,0,0},
"���m +15%\r"},

//13
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,3,0,3,0,0,0,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{30,0.2f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +30\r�B�� +0.2\r"},

//14
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,1,0,2,1,1,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{40,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +40\r"},

//15
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,1,0,0,2,2,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{45,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +45\r"},

//16
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,1,0,4,0,0,1,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{10,0.6f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +10\r�B�� +0.6\r"},

//17
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,1,0,0,0,0,2,0,0},
{SIN_ADD_ABSORB,0,0,0,0,0,0,0},
{1.2f,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B�� +1.2\r"},

//18
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,1,0,3,0,0},
{SIN_ADD_ABSORB,0,0,0,0,0,0,0},
{1.8f,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B�� +1.8\r"},

//19
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,1,0,0,1,1,4,0,0},
{SIN_ADD_ABSORB,0,0,0,0,0,0,0},
{2.4f,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B�� +2.4\r"},

//20
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,3,0,3,0,3,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{30,1.8f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +30\r�B�� +1.8\r"},

//21
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,0,3,3,0,0},
{SIN_ADD_ABSORB,0,0,0,0,0,0,0},
{30,0,0,0,0,0,0,0},
{SIN_ADD_PERCENT,0,0,0,0,0,0,0},
"�B�� +30%\r"},

//22
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,4,0,2,2,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{40,1.2f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +40\r�B�� +1.2\r"},

//23
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,1,0,0,4,0},
{SIN_ADD_LIFE,0,0,0,0,0,0,0},
{40,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�ͩR +40\r"},

//24
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,1,1,2,0,3,0},
{SIN_ADD_DEFENCE,SIN_ADD_MANA,0,0,0,0,0,0},
{40,30,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +40\r�]�k +30\r"},

//25
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,1,0,4,2,0},
{SIN_ADD_ABSORB,SIN_ADD_LIFE,0,0,0,0,0,0},
{2.4f,20,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�B�� +2.4\r�ͩR +20\r"},

//26
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,1,0,0,4,1},
{SIN_ADD_STAMINA,SIN_ADD_MANA,SIN_ADD_LIFE,0,0,0,0,0},
{40,20,10,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"��O +40\r�]�k +20\r�ͩR +10\r"},

//27
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,4,2,0,0,1},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{80,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +80\r"},

//28
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,2,0,2,0,1},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,SIN_ADD_STAMINA,0,0,0,0,0},
{40,1.2f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"���m +40\r�B�� +1.2\r��O +20\r"},

//29
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,0,3,0,2,1},
{SIN_ADD_DEFENCE,SIN_ADD_MANA,0,0,0,0,0,0},
{60,20,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +60\r�]�k +20\r"},

//30
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,0,0,5,0,1},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{50,3.0f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +50\r�B�� +3.0\r"},

//31
{{sinDA1,sinDA2,0,0,0,0,0,0},
{0,0,0,0,0,1,0,4,2},
{SIN_ADD_DEFENCE,SIN_ADD_LIFE,0,0,0,0,0,0},
{100,20,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +100\r�ͩR +20\r"},

//32
{{sinDA1,sinDA2,0,0,0,0,0,0},
{2,2,2,2,2,2,0,0,0},
{SIN_ADD_BIO,SIN_ADD_FIRE,SIN_ADD_ICE,SIN_ADD_LIGHTNING,SIN_ADD_POISON,0,0,0},
{5,5,5,5,5,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0},
"�]���m +5\r�r���m +5\r�B���m +5\r�����m +5\r�p���m +5\r"},

//33
{{sinDA1,sinDA2,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{10,1.0f,0,0,0,0,0,0},
{SIN_ADD_PERCENT,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +10%\r�B�� 1.0\r"},


// ���� ----------------------------------------------------------------------------------------------------

//1
{{sinDS1,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0,0},
{SIN_ADD_BIO,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�]���m +5\r"},

//2
{{sinDS1,0,0,0,0,0,0,0},
{0,5,0,0,0,0,0,0,0},
{SIN_ADD_POISON,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�r���m +5\r"},

//3
{{sinDS1,0,0,0,0,0,0,0},
{0,0,5,0,0,0,0,0,0},
{SIN_ADD_ICE,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B���m +5\r"},

//4
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,5,0,0,0,0,0},
{SIN_ADD_FIRE,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�����m +5\r"},

//5
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,5,0,0,0,0},
{SIN_ADD_LIGHTNING,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�p���m +5\r"},

//6
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,0,1,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{10,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +10\r"},

//7
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,0},
{SIN_ADD_BLOCK_RATE,0,0,0,0,0,0,0},
{1,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"��ײv +1\r"},

//8
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,0},
{SIN_ADD_LIFE,0,0,0,0,0,0,0},
{10,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�ͩR +10\r"},

//9
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{20,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +20\r"},

//10
{{sinDS1,0,0,0,0,0,0,0},
{2,0,1,0,0,0,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{10,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +10\r"},

//11
{{sinDS1,0,0,0,0,0,0,0},
{0,2,2,1,0,0,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{10,0,0,0,0,0,0,0},
{SIN_ADD_PERCENT,0,0,0,0,0,0,0},
"���m +10%\r"},

//12
{{sinDS1,0,0,0,0,0,0,0},
{0,3,1,0,1,0,0,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{15,0.3f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +15\r�B�� +0.3\r"},

//13
{{sinDS1,0,0,0,0,0,0,0},
{0,0,2,0,2,0,0,0,0},
{SIN_ADD_ABSORB,0,0,0,0,0,0,0},
{0.3f,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B�� +0.3\r"},

//14
{{sinDS1,0,0,0,0,0,0,0},
{0,1,0,0,1,2,0,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{10,0.6f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +10\r�B�� +0.6\r"},

//15
{{sinDS1,0,0,0,0,0,0,0},
{0,2,2,2,1,1,0,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{20,0.3f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +20\r�B�� +0.3\r"},

//16
{{sinDS1,0,0,0,0,0,0,0},
{0,0,1,0,2,0,1,0,0},
{SIN_ADD_ABSORB,SIN_ADD_BLOCK_RATE,0,0,0,0,0,0},
{0.6f,2,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�B�� +0.6\r��ײv 2%\r"},

//17
{{sinDS1,0,0,0,0,0,0,0},
{0,0,3,0,0,3,1,0,0},
{SIN_ADD_BLOCK_RATE,0,0,0,0,0,0,0},
{3,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"��ײv +3%\r"},

//18
{{sinDS1,0,0,0,0,0,0,0},
{0,4,4,0,0,0,1,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_BLOCK_RATE,0,0,0,0,0,0},
{40,1,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +40\r��ײv +1%\r"},

//19
{{sinDS1,0,0,0,0,0,0,0},
{0,2,1,1,0,1,2,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_BLOCK_RATE,0,0,0,0,0,0},
{20,2,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +20\r��ײv +2%\r"},

//20
{{sinDS1,0,0,0,0,0,0,0},
{4,1,0,0,0,1,0,2,0},
{SIN_ADD_DEFENCE,SIN_ADD_MANA,0,0,0,0,0,0},
{40,10,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +40\r�]�k +10\r"},

//21
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,0,2,1,2,0},
{SIN_ADD_BLOCK_RATE,SIN_ADD_LIFE,0,0,0,0,0,0},
{2,20,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"��ײv +2%\r�ͩR +20\r"},

//22
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,5,0,0,2,0},
{SIN_ADD_ABSORB,SIN_ADD_STAMINA,0,0,0,0,0,0},
{1.0f,50,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�B�� +1.0\r��O +50\r"},

//23
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,3,1,0,2,0},
{SIN_ADD_ABSORB,SIN_ADD_LIFE,0,0,0,0,0,0},
{0.6f,40,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�B�� +0.6\r�ͩR +40\r"},

//24
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,0,5,0,0,1},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{50,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m +50\r"},

//25
{{sinDS1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,2,2},
{SIN_ADD_DEFENCE,SIN_ADD_BLOCK_RATE,SIN_ADD_LIFE,0,0,0,0,0},
{40,2,20,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"���m +40\r��ײv +2%\r�ͩR +20\r"},

//26
{{sinDS1,0,0,0,0,0,0,0},
{2,2,2,2,2,2,0,0,0},
{SIN_ADD_BIO,SIN_ADD_POISON,SIN_ADD_LIGHTNING,SIN_ADD_FIRE,SIN_ADD_ICE,0,0,0},
{5,5,5,5,5,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0},
"�]���m +5\r�r���m +5\r�B���m +5\r�����m +5\r�p���m +5\r"},

//27
{{sinDS1,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{20,0.5f,0,0,0,0,0,0},
{SIN_ADD_PERCENT,SIN_ADD_NUM,0,0,0,0,0,0},
"���m +20%\r�B�� +0.5\r"},

// �]�� -----------------------------------------------------------------------------------------------

{{sinOM1,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0,0},
{SIN_ADD_BIO,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���ݩ� +5\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,5,0,0,0,0,0,0,0},
{SIN_ADD_POISON,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�r�ݩ� +5\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,5,0,0,0,0,0,0},
{SIN_ADD_ICE,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B�ݩ� +5\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,5,0,0,0,0,0},
{SIN_ADD_FIRE,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���ݩ� +5\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,0,5,0,0,0,0},
{SIN_ADD_LIGHTNING,0,0,0,0,0,0,0},
{5,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�p�ݩ� +5\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,0,0,1,0,0,0},
{SIN_ADD_MANAREGEN,0,0,0,0,0,0,0},
{0.3f,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�]�k�ȦA�� +0.3\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,1,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{10,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m�O +10\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,0},
{SIN_ADD_MANA,0,0,0,0,0,0,0},
{20,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�]�k�� +20\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1},
{SIN_ADD_ABSORB,0,0,0,0,0,0,0},
{1.0f,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B�~���m�O +1.0\r"},

{{sinOM1,0,0,0,0,0,0,0},
{2,0,1,0,0,0,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{10,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m�O +10\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,2,2,1,0,0,0,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{10,0,0,0,0,0,0,0},
{SIN_ADD_PERCENT,0,0,0,0,0,0,0},
"���m�O +10%\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,3,1,0,2,0,0,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,0,0,0,0,0,0},
{15,0.4f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m�O +15\r�B�~���m�O +0.4\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,3,0,3,0,0,0,0},
{SIN_ADD_ABSORB,0,0,0,0,0,0,0},
{0.6f,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"�B�~���m�O +0.6\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,1,0,0,2,2,0,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_MANAREGEN,0,0,0,0,0,0},
{10,0.4f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m�O +10\r�]�k�ȦA�� +0.4\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,2,2,2,2,1,0,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_MANAREGEN,0,0,0,0,0,0},
{20,0.2f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m�O +20\r�]�k�ȦA�� +0.2\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,1,0,4,0,1,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{40,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m�O +40\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,3,0,0,3,1,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_MANAREGEN,0,0,0,0,0,0},
{10,0.6f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m�O +10\r�]�k�ȦA�� +0.6\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,4,4,0,0,0,1,0,0},
{SIN_ADD_DEFENCE,0,0,0,0,0,0,0},
{50,0,0,0,0,0,0,0},
{SIN_ADD_NUM,0,0,0,0,0,0,0},
"���m�O +50\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,2,1,1,0,1,2,0,0},
{SIN_ADD_DEFENCE,SIN_ADD_MANAREGEN,0,0,0,0,0,0},
{40,0.4f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"���m�O +40\r�]�k�ȦA�� +0.4\r"},

{{sinOM1,0,0,0,0,0,0,0},
{4,1,0,0,0,2,0,2,0},
{SIN_ADD_MANAREGEN,SIN_ADD_MANA,0,0,0,0,0,0},
{0.4f,40,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�]�k�ȦA�� +0.4\r�]�k�� +40\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,0,0,2,1,2,0},
{SIN_ADD_DEFENCE,SIN_ADD_MANAREGEN,SIN_ADD_STAMINA,0,0,0,0,0},
{10,0.4f,20,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"���m�O +10\r�]�k�ȦA�� +0.4\r��O�� +20\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,2,5,0,0,2,0},
{SIN_ADD_MANA,SIN_ADD_STAMINA,0,0,0,0,0,0},
{50,50,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�]�k�� +50\r��O�� +50\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,0,3,2,0,2,0},
{SIN_ADD_ABSORB,SIN_ADD_MANAREGEN,SIN_ADD_LIFE,0,0,0,0,0},
{0.6f,0.6f,20,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"�B�~���m�O +0.6\r�]�k�ȦA�� +0.6\r�ͩR�� +20\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,0,0,5,0,0,1},
{SIN_ADD_ABSORB,SIN_ADD_MANAREGEN,0,0,0,0,0,0},
{1.0f,1.0f,0,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0,0},
"�B�~���m�O +1.0\r�]�k�ȦA�� +1.0\r"},

{{sinOM1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,2,2,2},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,SIN_ADD_MANA,0,0,0,0,0},
{20,1.0f,40,0,0,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"���m�O +20\r�B�~���m�O +1.0\r�]�k�� +40\r"},

{{sinOM1,0,0,0,0,0,0,0},
{2,2,2,2,2,2,0,0,0},
{SIN_ADD_BIO,SIN_ADD_POISON,SIN_ADD_ICE,SIN_ADD_FIRE,SIN_ADD_LIGHTNING,0,0,0},
{5,5,5,5,5,0,0,0},
{SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0},
"���ݩ� +5\r�r�ݩ� +5\r�B�ݩ� +5\r���ݩ� +5\r�p�ݩ� +5\r"},

{{sinOM1,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1},
{SIN_ADD_DEFENCE,SIN_ADD_ABSORB,SIN_ADD_MANAREGEN,0,0,0,0,0},
{20,1.0f,0.5f,0,0,0,0,0},
{SIN_ADD_PERCENT,SIN_ADD_NUM,SIN_ADD_NUM,0,0,0,0,0},
"���m�O +20%\r�B�~���m�O +1.0\r�]�k�ȦA�� +0.5\r"},
