
// ���� �Ҽ��� MATRIX
void smRotateZMatrix( smMATRIX &m , int rads );
void smRotateYMatrix( smMATRIX &m , int rads );
void smRotateXMatrix( smMATRIX &m , int rads );
void smMatrixMult( smMATRIX &m , smMATRIX &a, smMATRIX &b );
void smMatrixMult2( smMATRIX &m , smMATRIX &a, smMATRIX &b );
void smIdentityMatrix( smMATRIX &m );
void smZeroMatrix( smMATRIX &m );
void smTranslateMatrix( smMATRIX &m , int x , int y, int z );

int smMatrixInvert( smMATRIX &a , smMATRIX &q );

//��Ʈ���� ��ȯ
void smMatrixShift( smMATRIX &m , int shift );


void smMatrixFromQuaternion( smMATRIX& mat, float x, float y, float z, float w );
void smQuaternionMult( int& Qx, int& Qy, int& Qz, int & Qw,
                       int Ax, int Ay, int Az, int Aw,
                       int Bx, int By, int Bz, int Bw );
void smQuaternionFromAxis( float &x, float &y, float &z, float &w );
void smDQuaternionFromAxis( double &x, double &y, double &z, double &w );


// �ε� �Ҽ��� double Ÿ�� MATRIX
void smMatrixFromDMatrix( smMATRIX &m , smDMATRIX &d );
void smDZeroMatrix( smDMATRIX &m );
void smDIdentityMatrix( smDMATRIX &m );
// Rotate matrix about X axis
void smDRotateXMatrix( smDMATRIX &m , int rads );
// Rotate matrix about Y axis
void smDRotateYMatrix( smDMATRIX &m , int rads );
// Rotate matrix about Z axis
void smDRotateZMatrix( smDMATRIX &m , int rads );
// Translate matrix 
void smDTranslateMatrix( smDMATRIX &m , float x , float y, float z );
void smDMatrixMult( smDMATRIX &m , smDMATRIX &a, smDMATRIX &b );
void smDMatrixFromQuaternion( smDMATRIX& mat, double x, double y, double z, double w );

void smDQuaternionFromMatrix( FLOAT& x, FLOAT& y, FLOAT& z, FLOAT& w, smDMATRIX& mat );

extern	VOID D3DMath_QuaternionSlerp( FLOAT& Qx, FLOAT& Qy, FLOAT& Qz, FLOAT& Qw,
                              FLOAT Ax, FLOAT Ay, FLOAT Az, FLOAT Aw,
                              FLOAT Bx, FLOAT By, FLOAT Bz, FLOAT Bw,
                              FLOAT fAlpha );



// ���� �Ҽ��� MATRIX Ȯ�� ���� ���� 15��Ʈ �Ҽ��� 1/32768
void smERotateZMatrix( smEMATRIX &m , int rads );
void smERotateYMatrix( smEMATRIX &m , int rads );
void smERotateXMatrix( smEMATRIX &m , int rads );
void smEMatrixMult( smEMATRIX &m , smEMATRIX &a, smEMATRIX &b );
void smEMatrixMult2( smEMATRIX &m , smEMATRIX &a, smEMATRIX &b );
void smEIdentityMatrix( smEMATRIX &m );
void smEZeroMatrix( smEMATRIX &m );
void smETranslateMatrix( smEMATRIX &m , int x , int y, int z );
void smEMatrixShift( smEMATRIX &m , int shift );



// �ε� �Ҽ��� float Ÿ�� MATRIX
void smFZeroMatrix( smFMATRIX &m );
void smFIdentityMatrix( smFMATRIX &m );

//�����
HRESULT smFMatrixInvert( smFMATRIX& q, smFMATRIX& a );

// Rotate matrix about X axis
void smFRotateXMatrix( smFMATRIX &m , int rads );
// Rotate matrix about Y axis
void smFRotateYMatrix( smFMATRIX &m , int rads );
// Rotate matrix about Z axis
void smFRotateZMatrix( smFMATRIX &m , int rads );
// Translate matrix 
void smFTranslateMatrix( smFMATRIX &m , float x , float y, float z );
void smFMatrixMult( smFMATRIX &m , smFMATRIX &a, smFMATRIX &b );
void smFMatrixFromQuaternion( smFMATRIX& mat, float x, float y, float z, float w );

void smFQuaternionFromMatrix( FLOAT& x, FLOAT& y, FLOAT& z, FLOAT& w, smFMATRIX& mat );


void smFMatrixFromMatrix( smFMATRIX &m , smMATRIX &d );
void smMatrixFromFMatrix( smMATRIX &m , smFMATRIX &d );

//######################################################################################
//�� �� �� : �� �� ��
void smFMatrixFromDMatrix( smFMATRIX &m, smDMATRIX &d );
//######################################################################################


//######################################################################################
//�� �� �� : �� �� ��
const FLOAT g_PI       =  3.14159265358979323846f; // Pi

extern HRESULT D3D_SetProjectionMatrix( D3DMATRIX &mat, float fFOV, float fAspect, float fNearPlane, float fFarPlane );
//######################################################################################

//######################################################################################
//�� �� �� : �� �� ��
int smEMatrixInvert( smEMATRIX &q, smEMATRIX &a );
//######################################################################################

