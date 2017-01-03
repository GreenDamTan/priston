
#define		MAX_PATTERN		256
#define		MAX_STAGEOBJ		1024


struct smSTAGE_OBJ3D {

	smPAT3D *BipPattern;
	smPAT3D *Pattern;
	POINT3D Posi;
	POINT3D Angle;

	smSTAGE_OBJ3D *LinkObj;
	int sum;
	int LastDrawTime;

};


class smSTAGE_OBJECT {
public:
	smSTAGE_OBJ3D *ObjectMap[MAP_SIZE][MAP_SIZE];

	smSTAGE_OBJ3D mObj[ MAX_STAGEOBJ ];
	int nObj;
	POINT3D Camera;
	POINT3D CameraAngle;

	int SumCnt;

	smSTAGE_OBJECT();
	~smSTAGE_OBJECT();

	int AddObject( smPAT3D *Pat, int x, int y, int z , int angX, int angY, int angZ );
	//������Ʈ ���� �ڵ���ǥ ( �ʿ� ���Ե� ������Ʈ )
	int AddObject( smPAT3D *Pat );
	//���Ϸ� ������Ʈ ���� �ڵ���ǥ ( �ʿ� ���Ե� ������Ʈ )
	int AddObjectFile( char *szFile , char *szBipFile=0 );

	int Draw( smSTAGE_OBJ3D *mobj );
	int Draw( int x, int y, int z, int ax, int ay, int az );
	int Draw2( int x, int y, int z, int ax, int ay, int az );

	//######################################################################################
	//�� �� �� : �� �� ��
	int DrawOpening( smSTAGE_OBJ3D *mobj, int FrameStep );
	int DrawOpening( int x, int y, int z, int ax, int ay, int az, int FrameStep );
	//######################################################################################

};
