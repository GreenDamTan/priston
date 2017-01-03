
extern int	GeoResult_X;
extern int	GeoResult_Y;
extern int	GeoResult_Z;


//���� �ﰢ�� �ȿ� ��� �ִ���(�ﰢ�� ���ο� ��ġ�� ���� ���� �ʴ°ɷ� ó��) ->ȣ�ٶ� ����
//bool    PointInTri(int a1, int b1, int a2, int b2, int a3, int b3, int pointX, int pointY);
//���� �ﰢ�� �ȿ� ��� �ִ���(�ﰢ�� ���ο� ��ġ�� ���� ���� �ʴ°ɷ� ó��) ->ȣ�ٶ� ����
bool PointInTri(float a1, float a2, float a3, float b1, float b2, float b3, float mouseX, float mouseY);

// �̵��� ���� ��ǥ���� ���ϴ� ���̱���! �����.
void GetMoveLocation(int x, int y, int z, int AngX, int AngY, int AngZ );
//  ����� ��ġ�� �ľ��Ͽ� �̵��ؾ� �� ������ �˷��ش�.
void GetTraceAngle( POINT3D *posi , POINT3D *angle , int tx , int ty , int tz );
int CheckLocateBox( int sx, int sz , int tx, int tz , int size );
int GetSubAngle( int a1, int a2 );
void GetRadian3D(int x1 , int y, int y1, int x2, int y_, int y2 );
int GetRadian2D(int x1 ,int y1, int x2, int y2 );
int GetDistance(int x1 , int y, int y1, int x2, int y_, int y2 );

// ī�޶� ���� ��Ʈ���� ����� ( sx,sy,sz -ī�޶�  tx,ty,tz - ������ )
int MakeTraceMatrix ( eCAMERA_TRACE *lpeTrace , int sx, int sy, int sz, int tx, int ty, int tz );
