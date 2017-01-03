#include "..\\particle.h"
#include "..\\HoBaram\\HoLinkHeader.h"


//#define GRAVITY D3DVECTOR(0.0f, -9.8f, 0.0f)
int EngineFps = 70;
HoParticle::HoParticle()
{
	Age  = -1.0f;
	//Size = 20.0f;
	Color.r = 255.f;
	Color.g = 0.0f;
	Color.b = 0.0f;
	Color.a = 255.f/2;
	TimeCount = 0;
}

HoParticle::~HoParticle()
{


}

//EngineFps�� 70fps�̸� Main�� 1�ʿ� 70�� ȣ���Ѵ�. TimeCount >= 1�̸� 1/fps�� ���������̴�.
bool HoParticle::Main()
{
	TimeCount++;
	if(TimeCount >= 1)
	{
		if(Age+TimeCount/EngineFps >= LifeTime)
		{
			Age = -1.0f;
			return false;
		}
		else
		{
			Age += TimeCount/EngineFps;
			PrevLocation = Location;
			Location.x += Velocity.x*(TimeCount);
			Location.y += Velocity.y*(TimeCount);
			Location.z += Velocity.z*(TimeCount);
			
			D3DVECTOR	initGravity;
			initGravity.x =   0.0f;
			initGravity.y =  -9.8f;
			initGravity.z =   0.0f;
			
			Velocity.x += ( initGravity.x * Gravity * TimeCount);
			Velocity.y += ( initGravity.y * Gravity * TimeCount);
			Velocity.z += ( initGravity.z * Gravity * TimeCount);

			Color.r += ColorDelta.r * (TimeCount);
			Color.g += ColorDelta.g * (TimeCount);
			Color.b += ColorDelta.b * (TimeCount);

			Alpha   += AlphaDelta * (TimeCount);
			Size    += SizeDelta * (TimeCount);
			Gravity += GravityDelta * (TimeCount);
			
			/*
			//���ư��� �������� ȸ���� �ϴ� ���...
			if ( Parent->GetVelocityRotateState() )
			{
				D3DVECTOR velocityLocation;
				D3DVECTOR localLocation;
				Parent->GetLocation(velocityLocation);
				
				localLocation.x = Location.x-velocityLocation.x;
				localLocation.y = Location.y-velocityLocation.y;
				localLocation.z = Location.z-velocityLocation.z;
				
				double x;
				double y;
				
				
				int localAngle = ANGLE_360/60;
				float hoSin = float(GetSin[localAngle]);
				float hoCos = float(GetCos[localAngle]);
				
				hoSin = hoSin/65536.f;
				hoCos = hoCos/65536.f;
				
				x = (localLocation.x*hoCos - localLocation.y*hoSin);
				y = (localLocation.x*hoSin + localLocation.y*hoCos);

				PrevLocation = Location;
				Location.x = float(x+velocityLocation.x);
				Location.y = float(y+velocityLocation.y);
			}
			
			if( Parent->GetAttractiveState() )
			{
				D3DVECTOR attractLocation;
				Parent->GetLocation(attractLocation);
				attractLocation.x = attractLocation.x - Location.x;
				attractLocation.y = attractLocation.y - Location.y;
				attractLocation.z = attractLocation.z - Location.z;

				D3DVECTOR attractDir = attractLocation; 
				
				float length;
				length = (float)sqrt(attractDir.x*attractDir.x+attractDir.y*attractDir.y+attractDir.z*attractDir.z);
				
				//length = length*0.5;
				float speed = float(rand()%10+5);
				
				if(speed < 4)
				{
					speed *= 0.9f;
				}				
				Velocity.x += attractDir.x/length * speed * TimeCount;
				Velocity.y += attractDir.y/length * speed * TimeCount;
				Velocity.z += attractDir.z/length * speed * TimeCount;


				if(length < 5000)
				{
					Velocity.x *= 0.9f;
					Velocity.y *= 0.9f;
					Velocity.z *= 0.9f;
				}
			}
			*/
			
		}
		TimeCount = 0;
	}
	return true;
}


HoParticleSystem::HoParticleSystem() : HoEffectObject()
{
	make_pSu = MAX_PARTICLES;
	//��ŰŬ ���� �÷��׵�...
	AutoDieFlag = TRUE;
	//---- Particle System Settings ---- 
	ColorStart.r	= 0.0f;
	ColorStart.g	= 0.0f;
	ColorStart.b	= 0.0f;
	
//	ColorVar.r	= 0.0f;
	//ColorVar.g	= 0.0f;
	//ColorVar.b	= 0.0f;

	ColorEnd.r	= 0.0f;
	ColorEnd.g	= 0.0f;
	ColorEnd.b	= 0.0f;

	AlphaStart	=  0.0f;
//	AlphaVar	=   0.0f;
	AlphaEnd	=  0.0f;

	SizeStart	= 0.0f;
//	SizeVar		= 1.0f;
	SizeEnd		= 0.0f;

	Speed		= 0.0f;
//	SpeedVar	= 50.0f;

	Theta		= 180.0f;
	
	Life		= 1.0f;
//	LifeVar	    = 0.0f;
	
	GravityStart	= 0.0f;
//	GravityVar		= 0.0f;
	GravityEnd		= 0.0f;

	ParticlesPerSec	= EngineFps+1;
	
	Age = 0.0f;
	EmissionResidue = 0.0f;
//	TimeLastUpdate = 0.0f;
//	EmissionResidue = 0.0f;

	Direction.x = 0.0f;
	Direction.y = 0.0f;
	Direction.z = 0.0f;
	
	Location.x = 0.0f;
	Location.y = 0.0f;
	Location.z = 0.0f;

	PrevLocation.x = 0.0f;
	PrevLocation.y = 0.0f;
	PrevLocation.z = 0.0f;
	
	TimeCount = 0.0f;
	
	ParticlesPerSec = 0;

	//Test
	Location.x = 0;
	Location.y = 0;
	Location.z = 0;//-115*fONE;
	//Location.z = 0;

	PrevLocation.x = Location.x;
	PrevLocation.y = Location.y;
	PrevLocation.z = Location.z;
}

HoParticleSystem::~HoParticleSystem()
{

}

void HoParticleSystem::TranslateWorld(int x, int y, int z)
{
	PrevLocation = Location;
	Location.x = (float)x;
	Location.y = (float)y;
	Location.z = (float)z;
}


int HoParticleSystem::Start(int x, int y, int z, int matNum, int particleCode)
{
	WorldX = (float)x;
	WorldY = (float)y;
	WorldZ = (float)z;

	LocalX = 0;
	LocalY = 0;
	LocalZ = 0;
	
	make_pSu = MAX_PARTICLES;
	ParticleCode = particleCode;

	
	switch(particleCode)
	{
		case MONSTER_FORZENMIST_SHOT2:
		case SKILL_METEO:
		case MONSTER_PIGON_SHOT1:
		{
			StartFlag = TRUE;
			MaterialNum = matNum;
			AutoDieFlag = FALSE;
			
			ColorStart.r	= 255.0f;
			ColorStart.g    = 100;
			ColorStart.b	= 50.0f;
			
			ColorEnd.r	= 255.0f;
			ColorEnd.g	= 50.0f;
			ColorEnd.b	= 50.0f;

			SizeStart	= 12.0f;
			SizeEnd		= 1.0f;
			
			if(particleCode == MONSTER_FORZENMIST_SHOT2)
			{
				ColorStart.r	= 50.0f;
				ColorStart.g    = 70.0f;
				ColorStart.b	= 255.0f;
				
				ColorEnd.r	= 50.0f;
				ColorEnd.g	= 50.0f;
				ColorEnd.b	= 255.0f;

				SizeStart	= 40.0f;
				SizeEnd		= 25.0f;
			}
			AlphaStart	=  200.0f;
			AlphaEnd	=    0.0f;

			

			Speed		= 200.0f;
			Theta		= 180.0f;
			

			Life		= 0.10f;
			GravityStart	= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps*10;
			
			Age = 0.9f;
			
			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			TimeCount = 0.0f;
			
			Location.x = (float)x;
			Location.y = (float)y;
			Location.z = (float)z;
			
			PrevLocation.x = (float)Location.x;
			PrevLocation.y = (float)Location.y;
			PrevLocation.z = (float)Location.z;
		}
		break;
		
		case MONSTER_MEPHIT_SHOT1:
		case MONSTER_MEPHIT_SHOT2:		//ktj : ����
		case MONSTER_IMP_SHOT1:
		{
			StartFlag = TRUE;
			MaterialNum = matNum;
			AutoDieFlag = FALSE;
			
			ColorStart.r	= 230.0f;
			ColorStart.g    = 250.f;
			ColorStart.b	= 250.0f;
			
//			ColorVar.r	= 0.0f;
//			ColorVar.g	= 0.0f;
//			ColorVar.b	= 0.0f;

			ColorEnd.r	= 0.0f;
			ColorEnd.g	= 0.0f;
			ColorEnd.b	= 0.0f;
			

			AlphaStart	=  200.0f;
//			AlphaVar	=    0.0f;
			AlphaEnd	=    0.0f;

			SizeStart	= 4.0f;
			if(MONSTER_MEPHIT_SHOT2==particleCode)	SizeStart	= 6.0f;

//			SizeVar		= 0.0f;
			SizeEnd		= 0.5f;

			Speed		= 100.0f;
//			SpeedVar	= 0.0f;

			Theta		= 180.0f;
			

			Life		= 0.005f;
//			LifeVar	    = 0.0f;
			
			GravityStart	= 0.0f;
//			GravityVar		= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps*10;
			
			Age = 0.9f;
			//	TimeLastUpdate = 0.0f;
//			EmissionResidue = 0.0f;

			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			TimeCount = 0.0f;
			
			Location.x = (float)x;
			Location.y = (float)y;
			Location.z = (float)z;
			
			PrevLocation.x = (float)Location.x;
			PrevLocation.y = (float)Location.y;
			PrevLocation.z = (float)Location.z;	
		}
		break;

		case MONSTER_IMP_SHOT2:		//ktj : ���� ���Ե�
		case EFFECT_FIRE_CRACKER:	// ��öȣ : ����
		{
			StartFlag = TRUE;
			MaterialNum = matNum;
			AutoDieFlag = FALSE;
			
			ColorStart.r	= 230.0f;
			ColorStart.g    = 250.f;
			ColorStart.b	= 250.0f;
			
//			ColorVar.r	= 0.0f;
//			ColorVar.g	= 0.0f;
//			ColorVar.b	= 0.0f;

			ColorEnd.r	= 0.0f;
			ColorEnd.g	= 0.0f;
			ColorEnd.b	= 0.0f;
			

			AlphaStart	=  200.0f;
//			AlphaVar	=    0.0f;
			AlphaEnd	=    0.0f;

			//SizeStart	= 4.0f;		//ktj : ����
			if(particleCode == MONSTER_IMP_SHOT2)
			{
				SizeStart	= 6.0f;
	//			SizeVar		= 0.0f;
				//SizeEnd		= 0.5f;	//ktj : ����
				SizeEnd		= 2.5f;
			}
			else if(EFFECT_FIRE_CRACKER==particleCode)
			{
				SizeStart	= 3.0f;
				SizeEnd		= 1.5f;
			}

			Speed		= 100.0f;
			//if(EFFECT_FIRE_CRACKER==particleCode)	Speed	= 50.0f;
//			SpeedVar	= 0.0f;

			Theta		= 180.0f;
			

			Life		= 0.005f;
//			LifeVar	    = 0.0f;
			
			GravityStart	= 0.0f;
//			GravityVar		= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps*10;
			
			Age = 0.9f;
			//	TimeLastUpdate = 0.0f;
//			EmissionResidue = 0.0f;

			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			TimeCount = 0.0f;
			
			Location.x = (float)x;
			Location.y = (float)y;
			Location.z = (float)z;
			
			PrevLocation.x = (float)Location.x;
			PrevLocation.y = (float)Location.y;
			PrevLocation.z = (float)Location.z;	
		}
		break;

		case MONSTER_IMP_SHOT3:		//ktj : ���� ���Ե�
		{
			make_pSu = 2;			//ktj : shadow�� ��ġ�� �˰Ժ������� ���� ������ �ȵ�.

			StartFlag = TRUE;
			MaterialNum = matNum;
			AutoDieFlag = FALSE;
			

			//ktj : shadow�� ��°�� ColorStart�� ColorEnd���� 255
			//AlphaStart AlphaEnd�� 255������.
			ColorStart.r	= 255.0f;
			ColorStart.g    = 255.0f;
			ColorStart.b	= 255.0f;
			
//			ColorVar.r	= 0.0f;
//			ColorVar.g	= 0.0f;
//			ColorVar.b	= 0.0f;

			ColorEnd.r	= 255.0f;
			ColorEnd.g	= 255.0f;
			ColorEnd.b	= 255.0f;
			

			//AlphaStart	=  200.0f;	//ktj : ����
			AlphaStart	=  255.0f;	
//			AlphaVar	=    0.0f;
			AlphaEnd	=    0.0f;		//����
			AlphaEnd	=    255.0f;

			//SizeStart	= 4.0f;		//ktj : ����
			SizeStart	= 30.0f;//15.0f;
			
//			SizeVar		= 0.0f;
			//SizeEnd		= 0.5f;	//ktj : ����
			//SizeEnd		= 2.5f;
			SizeEnd		= 30.0f;//15.0f;

			Speed		= 100.0f;
//			SpeedVar	= 0.0f;

			Theta		= 180.0f;
			

			Life		= 0.005f;
//			LifeVar	    = 0.0f;
			
			GravityStart	= 0.0f;
//			GravityVar		= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps*10;
			
			Age = 0.9f;
			//	TimeLastUpdate = 0.0f;
//			EmissionResidue = 0.0f;

			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			TimeCount = 0.0f;
			
			Location.x = (float)x;
			Location.y = (float)y;
			Location.z = (float)z;
			
			PrevLocation.x = (float)Location.x;
			PrevLocation.y = (float)Location.y;
			PrevLocation.z = (float)Location.z;	
		}
		break;

		//��öȣ
		case MONSTER_FORZENMIST_SHOT1:	// ���̽���Ʈ
		{
			//make_pSu = 2;			//ktj : shadow�� ��ġ�� �˰Ժ������� ���� ������ �ȵ�.

			StartFlag = TRUE;
			MaterialNum = matNum;
			AutoDieFlag = FALSE;
			

			//ktj : shadow�� ��°�� ColorStart�� ColorEnd���� 255
			//AlphaStart AlphaEnd�� 255������.
			ColorStart.r	= 50.0f;
			ColorStart.g    = 70.0f;
			ColorStart.b	= 255.0f;
			

			ColorEnd.r	= 0.0f;
			ColorEnd.g	= 0.0f;
			ColorEnd.b	= 0.0f;

			//ColorEnd.r	= 50.0f;
			//ColorEnd.g	= 50.0f;
			//ColorEnd.b	= 255.0f;
			

			AlphaStart	=  200.0f;	
			AlphaEnd	=    0.0f;		//����
			//AlphaEnd	=    255.0f;


			SizeStart	= 15.0f;//15.0f;
			
			SizeEnd		= 15.0f;//15.0f;

			Speed		= 100.0f;


			Theta		= 180.0f;
			

			Life		= 0.1f;
			
			GravityStart	= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps*10;
			
			Age = 0.9f;

			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			TimeCount = 0.0f;
			
			Location.x = (float)x;
			Location.y = (float)y;
			Location.z = (float)z;
			
			PrevLocation.x = (float)Location.x;
			PrevLocation.y = (float)Location.y;
			PrevLocation.z = (float)Location.z;	
		}
		break;


		/*
		case MONSTER_FORZENMIST_SHOT2:	// ���̽���Ʈ
		{
			StartFlag = TRUE;
			MaterialNum = matNum;
			AutoDieFlag = FALSE;
			
			ColorStart.r	= 255.0f;
			ColorStart.g    = 100;
			ColorStart.b	= 50.0f;
			
			ColorEnd.r	= 255.0f;
			ColorEnd.g	= 50.0f;
			ColorEnd.b	= 50.0f;
			
			AlphaStart	=  200.0f;
			AlphaEnd	=    0.0f;

			SizeStart	= 12.0f;
			SizeEnd		= 1.0f;

			Speed		= 200.0f;
			Theta		= 180.0f;
			

			Life		= 0.10f;
			GravityStart	= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps*10;
			
			Age = 0.9f;
			
			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			TimeCount = 0.0f;
			
			Location.x = (float)x;
			Location.y = (float)y;
			Location.z = (float)z;
			
			PrevLocation.x = (float)Location.x;
			PrevLocation.y = (float)Location.y;
			PrevLocation.z = (float)Location.z;
			*/

	}
	return true;
}


int HoParticleSystem::Main()
{
	if(StartFlag == FALSE)
		return FALSE;
	//30Frame �����̸� TimeCount�� 30���� ���´�.
	TimeCount++;
	if(TimeCount >= 1)
	{
		ParticlesAlive = 0;
		//�̹� ��� �ִ� ��ƼŬ ������Ʈ �ϰ�...
		//for( int i = 0; i<MAX_PARTICLES; i++)
		for( int i = 0; i<make_pSu ; i++)
		
		{
			if(Particles[i].Age >= 0.0f)
				if(Particles[i].Main())
					ParticlesAlive++;
		}
		
		//��ƼŬ �ý����� ������ ���ϸ� �Ҹ�ǰ� �Ѵ�.
		if(AutoDieFlag)
			Age -= (TimeCount/EngineFps);
		
		if(Age < 0)
		{
			if(ParticlesAlive <= 0)
				StartFlag = FALSE;
			Age = -1.0f;
			return FALSE;
		}
		
		//��ƼŬ�� �ʿ��� ������ ���Ѵ�.(70Frame�� �������� �Ѵ�)
		//ParticlesPerSec: �ʴ� �Ѹ� ��ƼŬ�� ����.
		//EmissionResidue: ���Ѹ� ��ƼŬ�� ����.
		float particleNeeded = ParticlesPerSec*(TimeCount/EngineFps)+EmissionResidue;
		
		//int�� Cast
		unsigned int particlesCreated = (unsigned int)particleNeeded;
		//�ʿ��� ��ƼŬ�� ���� ������ ���� ��ŭ ��ƼŬ�� �ٽ� �����Ѵ�.
		EmissionResidue = particleNeeded - particlesCreated;

		if( particleNeeded > 0)
		{
			//for( i = 0; i < MAX_PARTICLES; i++)
			for ( int i = 0; i < make_pSu; i++ )
			{
				if(!particlesCreated)
					break;

				if( Particles[i].Age < 0.0f)
				{
					//��ƼŬ�� ����
					Particles[i].Age = 0.0f;					//��ƼŬ ���� �ʱ�ȭ(�׾��� �� �� -1.0f)
					Particles[i].LifeTime = Life;				//��ƼŬ�� ��� �־�� �� �ð� �ʱ�ȭ
					Clamp( Particles[i].LifeTime, MIN_LIFETIME, MAX_LIFETIME);
					
					//��ƼŬ�� ����...
					Particles[i].Color.r = ColorStart.r;
					Particles[i].Color.g = ColorStart.g;
					Particles[i].Color.b = ColorStart.b;
					Particles[i].Color.a = ColorStart.a;

					Clamp( Particles[i].Color.r, 0.0f, 255.0f);
					Clamp( Particles[i].Color.g, 0.0f, 255.0f);
					Clamp( Particles[i].Color.b, 0.0f, 255.0f);
					
					//��ƼŬ�� ���� ��ȭ �� ���..
					Particles[i].ColorDelta.r = (ColorEnd.r - Particles[i].Color.r) / (Particles[i].LifeTime*EngineFps);
					Particles[i].ColorDelta.g = (ColorEnd.g - Particles[i].Color.g) / (Particles[i].LifeTime*EngineFps);
					Particles[i].ColorDelta.b = (ColorEnd.b - Particles[i].Color.b) / (Particles[i].LifeTime*EngineFps);

					//��ƼŬ�� ���� ��ȭ��...
					Particles[i].Alpha = AlphaStart;
					Clamp( Particles[i].Alpha, MIN_ALPHA, MAX_ALPHA );
					Particles[i].AlphaDelta = (AlphaEnd - Particles[i].Alpha) / (Particles[i].LifeTime*EngineFps);
					
					//��ƼŬ�� ũ�� ��ȭ��...
					Particles[i].Size = SizeStart;
					Clamp( Particles[i].Size, MIN_SIZE, MAX_SIZE );
					Particles[i].SizeDelta = (SizeEnd - Particles[i].Size) / (Particles[i].LifeTime*EngineFps);


					//��ƼŬ�� �߷� ��ȭ��...
					Particles[i].Gravity = GravityStart*9.8f;
					
					Clamp( Particles[i].Gravity, MIN_GRAVITY*9.8f, MAX_GRAVITY*9.8f);
					Particles[i].GravityDelta = ( GravityEnd*9.8f - Particles[i].Gravity ) / (Particles[i].LifeTime*EngineFps);
					
					//��ƼŬ�� �ӵ� ����....(��ƼŬ�� �̵��Ұ�츦 ����ؼ�...
					D3DVECTOR tempVelocity;
					tempVelocity.x = (Location.x - PrevLocation.x)/(TimeCount/EngineFps);
					tempVelocity.y = (Location.y - PrevLocation.y)/(TimeCount/EngineFps);
					tempVelocity.z = (Location.z - PrevLocation.z)/(TimeCount/EngineFps);
					
					//��ƼŬ�� ó�� ��ġ�� ���Ѵ�.
					
					//ktj : ����
					Particles[i].Location.x   = PrevLocation.x + tempVelocity.x * RANDOM_NUM * (TimeCount/EngineFps);
					Particles[i].Location.y   = PrevLocation.y + tempVelocity.y * RANDOM_NUM * (TimeCount/EngineFps);
					Particles[i].Location.z	  = PrevLocation.z + tempVelocity.z * RANDOM_NUM * (TimeCount/EngineFps);
					
				
					Particles[i].PrevLocation = Location;
					float RandomYaw = RANDOM_NUM   * 3.14159f * 2.0f;
					float RandomPitch = RANDOM_NUM * Theta * 3.14159f / 180.0f ;

					//��������� ��ƼŬ�� ���ư��� ���͸� �����Ѵ�.
					Particles[i].Velocity.y = cosf( RandomPitch );
					Particles[i].Velocity.x = sinf( RandomPitch) * cosf(RandomYaw);
					Particles[i].Velocity.z = sinf( RandomPitch) * sinf(RandomYaw);
					
					float newSpeed = Speed;
					
					Clamp( newSpeed, MIN_SPEED, MAX_SPEED );

					//��öȣ ������ �ӵ�
					if(ParticleCode == EFFECT_FIRE_CRACKER)
					{
						Particles[i].Velocity.x *= newSpeed;
						Particles[i].Velocity.y *= newSpeed;
						Particles[i].Velocity.z *= newSpeed;
					}
					else {
						//ktj : ����
						Particles[i].Velocity.x *= newSpeed;
						Particles[i].Velocity.y *= newSpeed;
						Particles[i].Velocity.z *= newSpeed;
					}
				
					

					//���� �������� ��ƼŬ ����...
					particlesCreated--;
					Particles[i].SetParent(this);
				}
			}
		}

		TimeCount = 0;
	}
	return true;
}

void HoParticleSystem::GetLocation(D3DVECTOR& vLocation)
{
	vLocation = Location;
}


int HoParticleSystem::Draw(int x, int y, int z, int ax, int ay, int az)
{
	if(StartFlag == FALSE)
		return FALSE;
	
	if(make_pSu <= 0 )
		return FALSE;

	smRender.Init();
	smRender.SetMaterialGroup(smMaterialGroup);
	smMaterialGroup->smMaterial[MaterialNum].TwoSide = TRUE;


	//Camera���� �Է�.
	smRender.SetCameraPosi(x, y, z, ax, ay, az);
	
	
	//for(int i = 0; i < MAX_PARTICLES; i++)
	for(int i = 0; i < make_pSu ; i++)
	{
		if(Particles[i].Age >= 0.0f)
		{
			Face2d.x = int(Particles[i].Location.x);
			Face2d.y = int(Particles[i].Location.y);
			Face2d.z = int(Particles[i].Location.z);
			
			Face2d.r = int(Particles[i].Color.r);
			Face2d.g = int(Particles[i].Color.g);
			Face2d.b = int(Particles[i].Color.b);

			Face2d.Transparency = int(Particles[i].Alpha);
			
			Face2d.TexRect.left   = 0;
			Face2d.TexRect.top    = 0;
			Face2d.TexRect.right  = 1;
			Face2d.TexRect.bottom = 1;
			
			
			Face2d.width  = int(Particles[i].Size) << FLOATNS;
			Face2d.height = int(Particles[i].Size) << FLOATNS;
			Face2d.MatNum = MaterialNum;
			
			smRender.AddFace2D(&Face2d);
		}
	}
	
	renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
	//lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZFUNC,   D3DCMP_LESSEQUAL);
	//lpD3DDevice->SetRenderState( D3DRENDERSTATE_LASTPIXEL, TRUE );

	smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
	smRender.ClipRendFace();
	smRender.GeomVertex2D(FALSE);
	smRender.RenderD3D();

	//if(AttractiveFlag)
	//SetDynLight(Location.x, Location.y ,Location.z, LightR, LightG, LightB, 255, 100, 1);

	renderDevice.SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	//lpD3DDevice->SetRenderState( D3DRENDERSTATE_LASTPIXEL, FALSE );

	//if(AttractiveFlag)
	

	return TRUE;
}

void HoParticleSystem::Init()
{
	//MaterialNum = CreateTextureMaterial( "bubble.tga" ,0 , 0 , 0 ,0 , SMMAT_BLEND_LAMP );
	StartFlag = FALSE;
	
}

HoParticleBow::HoParticleBow()
{
	//��ŰŬ ���� �÷��׵�...
	StartFlag = FALSE;
}

HoParticleBow::~HoParticleBow()
{

}

int HoParticleBow::Main()
{
	if(StartFlag == FALSE)
		return FALSE;
	//30Frame �����̸� TimeCount�� 30���� ���´�.
	TimeCount++;
	if(TimeCount >= 1)
	{
		ParticlesAlive = 0;
		//�̹� ��� �ִ� ��ƼŬ ������Ʈ �ϰ�...
		for( int i = 0; i<MAX_PARTICLES; i++)
		{
			if(Particles[i].Age >= 0.0f)
				if(Particles[i].Main())
					ParticlesAlive++;
		}
		
		
		Age -= (TimeCount/EngineFps);
		if(Age < 0)
		{
			if(ParticlesAlive <= 0)
				StartFlag = FALSE;
			Age = -1.0f;
			return FALSE;
		}
		

		//��ƼŬ�� �ʿ��� ������ ���Ѵ�.(70Frame�� �������� �Ѵ�)
		//ParticlesPerSec: �ʴ� �Ѹ� ��ƼŬ�� ����.
		//EmissionResidue: ���Ѹ� ��ƼŬ�� ����.
		float particleNeeded = ParticlesPerSec*(TimeCount/EngineFps)+EmissionResidue;
		
		//int�� Cast
		unsigned int particlesCreated = (unsigned int)particleNeeded;
		//�ʿ��� ��ƼŬ�� ���� ������ ���� ��ŭ ��ƼŬ�� �ٽ� �����Ѵ�.


		EmissionResidue = particleNeeded - particlesCreated;

		if( particleNeeded > 0)
		{
			for ( int i = 0; i < MAX_PARTICLES; i++ )
			{
				if(!particlesCreated)
					break;

				if( Particles[i].Age < 0.0f)
				{
					//��ƼŬ�� ����
					Particles[i].Age = 0.0f;						 //��ƼŬ ���� �ʱ�ȭ(�׾��� �� �� -1.0f)
					Particles[i].LifeTime = Life; //��ƼŬ�� ��� �־�� �� �ð� �ʱ�ȭ
					Clamp( Particles[i].LifeTime, MIN_LIFETIME, MAX_LIFETIME);
					
					//��ƼŬ�� ����...
					Particles[i].Color.r = (ColorStart.r);
					Particles[i].Color.g = (ColorStart.g);
					Particles[i].Color.b = (ColorStart.b);
					Particles[i].Color.a = (ColorStart.a);

					Clamp( Particles[i].Color.r, 0.0f, 255.0f);
					Clamp( Particles[i].Color.g, 0.0f, 255.0f);
					Clamp( Particles[i].Color.b, 0.0f, 255.0f);
					
					//��ƼŬ�� ���� ��ȭ �� ���..
					Particles[i].ColorDelta.r = (ColorEnd.r - Particles[i].Color.r) / (Particles[i].LifeTime*EngineFps);
					Particles[i].ColorDelta.g = (ColorEnd.g - Particles[i].Color.g) / (Particles[i].LifeTime*EngineFps);
					Particles[i].ColorDelta.b = (ColorEnd.b - Particles[i].Color.b) / (Particles[i].LifeTime*EngineFps);

					//��ƼŬ�� ���� ��ȭ��...
					Particles[i].Alpha = AlphaStart;
					Clamp( Particles[i].Alpha, MIN_ALPHA, MAX_ALPHA );
					Particles[i].AlphaDelta = (AlphaEnd - Particles[i].Alpha) / (Particles[i].LifeTime*EngineFps);
					
					
					//��ƼŬ�� ũ�� ��ȭ��...
					Particles[i].Size = SizeStart;
					Clamp( Particles[i].Size, MIN_SIZE, MAX_SIZE );
					Particles[i].SizeDelta = (SizeEnd - Particles[i].Size) / (Particles[i].LifeTime*EngineFps);
				
					//��ƼŬ�� �߷� ��ȭ��...
					Particles[i].Gravity = GravityStart*9.8f;
					
					Clamp( Particles[i].Gravity, MIN_GRAVITY*9.8f, MAX_GRAVITY*9.8f);
					Particles[i].GravityDelta = ( GravityEnd*9.8f - Particles[i].Gravity ) / (Particles[i].LifeTime*EngineFps);
					
				
					//��ƼŬ�� �ӵ� ����....(��ƼŬ�� �̵��Ұ�츦 ����ؼ�...
					D3DVECTOR tempVelocity;
					tempVelocity.x = (Location.x - PrevLocation.x)/(TimeCount/EngineFps);
					tempVelocity.y = (Location.y - PrevLocation.y)/(TimeCount/EngineFps);
					tempVelocity.z = (Location.z - PrevLocation.z)/(TimeCount/EngineFps);
					
					//��ƼŬ�� ó�� ��ġ�� ���Ѵ�.
					Particles[i].Location.x   = PrevLocation.x + tempVelocity.x * RANDOM_NUM * (TimeCount/EngineFps);
					Particles[i].Location.y   = PrevLocation.y + tempVelocity.y * RANDOM_NUM * (TimeCount/EngineFps);
					Particles[i].Location.z	  = PrevLocation.z + tempVelocity.z * RANDOM_NUM * (TimeCount/EngineFps);

					Particles[i].PrevLocation = Location;
					float RandomYaw = RANDOM_NUM   * 3.14159f * 2.0f;
					float RandomPitch = RANDOM_NUM * Theta * 3.14159f / 180.0f ;
					
					//��������� ��ƼŬ�� ���ư��� ���͸� �����Ѵ�.
					Particles[i].Velocity.y = cosf( RandomPitch );
					Particles[i].Velocity.x = sinf( RandomPitch) * cosf(RandomYaw);
					Particles[i].Velocity.z = sinf( RandomPitch) * sinf(RandomYaw);
					
					float newSpeed = Speed;
					
					Clamp( newSpeed, MIN_SPEED, MAX_SPEED );
					Particles[i].Velocity.x *= newSpeed;
					Particles[i].Velocity.y *= newSpeed;
					Particles[i].Velocity.z *= newSpeed;
					
					//���� �������� ��ƼŬ ����...
					particlesCreated--;
					Particles[i].SetParent(this);
				}
			}
		}
		TimeCount = 0;
	}
	return TRUE;
}

/*
int HoParticleBow::Start(int x, int y, int z, int angleX, int angleY, int angleZ, int r, int g, int b, int a, int matNum)
{
	if(StartFlag)
		return FALSE;
	VectorInFlag = FALSE;
	AttractiveFlag = FALSE;
	VelocityRotateFlag = TRUE;
	StartFlag = TRUE;

	MaterialNum = matNum;
	
	ColorStart.r	= (float)r;
	ColorStart.g	= (float)g;
	ColorStart.b	= (float)b;
	
	ColorVar.r	= 0;
	ColorVar.g	= 0;
	ColorVar.b	= 0;
	
	ColorEnd.r	= float(r/2);
	ColorEnd.g	= float(g/2);
	ColorEnd.b	= float(b/2);

	AlphaStart	=   (float)a;
	AlphaVar	=   0.0f;
	AlphaEnd	=   0.0f;

	SizeStart	= 2.0f;
	SizeVar		= 0.0f;
	SizeEnd		= 1.0f;

	Speed		= 150.0f;
	SpeedVar	= 0.0f;

	Theta		= 180.0f;
	
	Life		= 0.8f;
	LifeVar	    = 0.0f;
	
	GravityStart	= 0.0f;
	GravityVar		= 0.0f;
	GravityEnd		= 0.0f;
	
	ParticlesPerSec	= EngineFps;
	
	Age = 0.2f;
	EmissionResidue = 0.0f;

	Direction.x = 0.0f;
	Direction.y = 0.0f;
	Direction.z = 0.0f;
	
	TimeCount = 0.0f;
	
	//Test
	Location.x = (float)x;
	Location.y = (float)y;
	Location.z = (float)z;
	//Location.z = 0;

	PrevLocation.x = (float)x;
	PrevLocation.y = (float)y;
	PrevLocation.z = (float)z;
	
	AngleX = angleX;
	AngleY = angleY;
	AngleZ = angleZ;

	return TRUE;
}
*/

int HoParticleBow::Start(int x, int y, int z, int r, int g, int b, int a, int matNum)
{
	if(StartFlag)
		return FALSE;
	StartFlag = TRUE;
	
	//make_pSu = MAX_PARTICLES;
	MaterialNum = matNum;
	
	ColorStart.r	= (float)r;
	ColorStart.g	= (float)g;
	ColorStart.b	= (float)b;
	
	ColorEnd.r	= float(r/2);
	ColorEnd.g	= float(g/2);
	ColorEnd.b	= float(b/2);

	AlphaStart	=   (float)a;
//	AlphaVar	=   0.0f;
	AlphaEnd	=   0.0f;

	SizeStart	= 1.0f;
//	SizeVar		= 0.0f;
	SizeEnd		= 1.0f;

	Speed		= 150.0f;
//	SpeedVar	= 0.0f;

	Theta		= 180.0f;
	
	Life		= 0.7f;
//	LifeVar	    = 0.0f;
	
	GravityStart	= 0.0f;
//	GravityVar		= 0.0f;
	GravityEnd		= 0.0f;
	
	ParticlesPerSec	= EngineFps/4;
	
	Age = 0.2f;
	EmissionResidue = 0.0f;

	Direction.x = 0.0f;
	Direction.y = 0.0f;
	Direction.z = 0.0f;
	
	TimeCount = 0.0f;
	
	//Test
	Location.x = (float)x;
	Location.y = (float)y;
	Location.z = (float)z;
	//Location.z = 0;

	PrevLocation.x = (float)x;
	PrevLocation.y = (float)y;
	PrevLocation.z = (float)z;
	
	
	return TRUE;
}


int HoParticleBow::StartFire(int x, int y, int z, int r, int g, int b, int a, int matNum)
{
	if(StartFlag)
		return FALSE;
	StartFlag = TRUE;
	MaterialNum = matNum;
	
	//make_pSu = MAX_PARTICLES;

	ColorStart.r	= (float)r;
	ColorStart.g	= (float)g;
	ColorStart.b	= (float)b;
	
//	ColorVar.r	= 0;
//	ColorVar.g	= 0;
//	ColorVar.b	= 0;
	
	ColorEnd.r	= 0.f;
	ColorEnd.g	= 0.f;
	ColorEnd.b	= 0.f;

	AlphaStart	=   (float)a;
//	AlphaVar	=   0.0f;
	AlphaEnd	=   0.0f;

	SizeStart	= 6.0f;
//	SizeVar		= 0.0f;
	SizeEnd		= 4.0f;

	Speed		= 100.0f;
//	SpeedVar	= 0.0f;

	Theta		= 180.0f;
	
	Life		= 0.5f;
//	LifeVar	    = 0.0f;
	
	GravityStart	= 0.0f;
//	GravityVar		= 0.0f;
	GravityEnd		= 0.0f;
	
	ParticlesPerSec	= EngineFps*2;
	
	Age = 0.04f;
	EmissionResidue = 0.0f;

	Direction.x = 0.0f;
	Direction.y = 0.0f;
	Direction.z = 0.0f;
	
	TimeCount = 0.0f;
	
	//Test
	Location.x = (float)x;
	Location.y = (float)y;
	Location.z = (float)z;
	//Location.z = 0;

	PrevLocation.x = (float)x;
	PrevLocation.y = (float)y;
	PrevLocation.z = (float)z;
	
	
	return TRUE;
}

HoParticleTornado::HoParticleTornado()
{

}

HoParticleTornado::~HoParticleTornado()
{

}

int HoParticleTornado::Main()
{
	if(StartFlag)
	{
		HoParticleSystem::Main();
		PrevLocation = Location;
		
		LocalAngle += ANGLE_360/60;
		float hoSin = float(GetSin[LocalAngle&ANGCLIP]);
		float hoCos = float(GetCos[LocalAngle&ANGCLIP]);
		double x;
		double z;
		
		hoSin = hoSin/65536.f;
		hoCos = hoCos/65536.f;
		
		x = (LocalX*hoCos - LocalZ*hoSin);
		z = (LocalX*hoSin + LocalZ*hoCos);
		

		Location.x = float(WorldX+x);
		Location.z = float(WorldZ+z);
		Location.y += 100;


		//Vector �������� ���� ���ư��� �Ѵ�.
		// calculate the vector between the particle and the attractor
		D3DVECTOR	attractLocation;
		attractLocation.x = (LocalX+WorldX)-WorldX;
		attractLocation.y = (LocalY+WorldY)-WorldY;
		attractLocation.z = (LocalZ+WorldZ)-WorldZ;
		

		D3DVECTOR attractDir = attractLocation; 
		float length;
		length = (float)sqrt(attractDir.x*attractDir.x+attractDir.y*attractDir.y+attractDir.z*attractDir.z);
		
		//PrevLocation = Location;
		LocalX += attractLocation.x/length*100; 
		//Location.y += attractDir.y/length * 300.0f;
		LocalZ += attractLocation.z/length*100; 
	}

	return TRUE;	
}

int HoParticleTornado::Start(int worldX, int worldY, int worldZ, int startX, int startY, int startZ, int matNum)
{
	LocalAngle = 20;
	WorldX = (float)worldX;
	WorldY = (float)worldY;
	WorldZ = (float)worldZ;
	
	//���� ��ġ..
	LocalX = (float)startX;
	LocalY = (float)startY;
	LocalZ = (float)startZ;

	StartFlag = TRUE;
	MaterialNum = matNum;
	

	ColorStart.r	= 255.0f;
	ColorStart.g	= 200.0f;
	ColorStart.b	= 125.0f;
	
//	ColorVar.r	= 0.0f;
	//ColorVar.g	= 0.0f;
	//ColorVar.b	= 0.0f;

	ColorEnd.r	= 255.0f/2;
	ColorEnd.g	= 200.0f/2;
	ColorEnd.b	= 100.0f/2;
	

	AlphaStart	=  200.0f;
//	AlphaVar	=    0.0f;
	AlphaEnd	=    0.0f;

	SizeStart	= 8.0f;
//	SizeVar		= 1.0f;
	SizeEnd		= 3.0f;

	Speed		= 50.0f;
//	SpeedVar	= 0.0f;

	Theta		= 180.0f;
	
	Life		= 1.0f;
//	LifeVar	    = 0.0f;
	
	GravityStart	= 0.0f;
//	GravityVar		= 0.0f;
	GravityEnd		= 0.0f;
	
	ParticlesPerSec	= EngineFps;
	
	Age = 2.0f;
	//	TimeLastUpdate = 0.0f;
	EmissionResidue = 0.0f;

	Direction.x = 0.0f;
	Direction.y = 0.0f;
	Direction.z = 0.0f;
	
	TimeCount = 0.0f;
	
	Location.x = 0;
	Location.y = WorldY;
	Location.z = 0;
	
	PrevLocation.x = Location.x;
	PrevLocation.y = Location.y;
	PrevLocation.z = Location.z;
	
	return TRUE;
}


HoParticleDest::HoParticleDest() : HoParticleSystem()
{
	StartFlag = false;
	SetClassID(CLASS_ID_PARTICLE_DEST);
}

HoParticleDest::~HoParticleDest()
{
	StartFlag = false;
}

int HoParticleDest::Main()
{
	if(StartFlag)
	{
		HoParticleSystem::Main();
		PrevLocation = Location;
		
		float x = 0.f;
		float z = 0.f;
		
		if(EndWaveFlag)
		{
			LocalAngle -= ANGLE_360/(180+80);
			float hoSin = float(GetSin[LocalAngle&ANGCLIP]);
			float hoCos = float(GetCos[LocalAngle&ANGCLIP]);
			hoSin = hoSin/65536.f;
			hoCos = hoCos/65536.f;
			
			x = (LocalX*hoCos - LocalZ*hoSin);
			z = (LocalX*hoSin + LocalZ*hoCos);
			

			Location.x = float(WorldX+x);
			Location.z = float(WorldZ+z);
		}
		else
		{
			Location.x = WorldX+LocalX;
			Location.y = WorldY+LocalY;
			Location.z = WorldZ+LocalZ;
		}
		
		//�̵��� ��ġ�� ���� ������ ����ȭ�� ���ؼ�.....
		D3DVECTOR	attractLocation;
		attractLocation.x = DestX - (WorldX+LocalX);
		attractLocation.y = DestY - (WorldY+LocalY);
		attractLocation.z = DestZ - (WorldZ+LocalZ);
		
		D3DVECTOR attractDir = attractLocation;
		//������ ���̸� ���Ѵ�.
		float length;
		length = (float)sqrt(attractDir.x*attractDir.x+attractDir.y*attractDir.y+attractDir.z*attractDir.z);
		
		if(length == 0.0f)
			length = 1.0f;
		//���� ����ȭ...
		PrevLocation = Location;
		LocalX += attractDir.x/length * 850.0f;
		LocalZ += attractDir.z/length * 850.0f;
	}

	
	return TRUE;
}


int HoParticleDest::Start(int worldX, int worldY, int worldZ, int destX, int destY, int destZ, int matNum, int type)
{
	
	SetClassID(CLASS_ID_PARTICLE_DEST);
	switch(type)
	{
		case 0:
		{
			EndWaveFlag = FALSE;
			
			StartFlag = TRUE;
			MaterialNum = matNum;
			
			ColorStart.r	= 255.0f;
			ColorStart.g	= 255.0f;
			ColorStart.b	= 255.0f;
			
//			ColorVar.r	= 0.0f;
//			ColorVar.g	= 0.0f;
//			ColorVar.b	= 0.0f;

			ColorEnd.r	= 255.0f;
			ColorEnd.g	= 255.0f;
			ColorEnd.b	= 255.0f;
			

			AlphaStart	=  100.0f;
//			AlphaVar	=   0.0f;
			AlphaEnd	=   0.0f;

			SizeStart	= 10.0f;
//			SizeVar		= 1.0f;
			SizeEnd		= 3.0f;

			Speed		= 150.0f;
//			SpeedVar	= 0.0f;

			Theta		= 180.0f;
			
			Life		= 0.4f;
//			LifeVar	    = 0.0f;
			
			GravityStart	= 0.0f;
//			GravityVar		= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps;
			
			Age = 0.6f;
			//	TimeLastUpdate = 0.0f;
			EmissionResidue = 0.0f;

			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			TimeCount = 0.0f;
			
			DestX = destX;
			DestY = destY;
			DestZ = destZ;
			
			LocalX = 0;
			LocalY = 0;
			LocalZ = 0;

			WorldX = (float)worldX;
			WorldY = (float)worldY;
			WorldZ = (float)worldZ;
			
			PrevLocation.x = Location.x;
			PrevLocation.y = Location.y;
			PrevLocation.z = Location.z;

			Location.x = worldX+LocalX;
			Location.y = worldY+LocalY;
			Location.z = worldZ+LocalZ;
		break;
		}

		case 1:
		{
			EndWaveFlag = TRUE;
			LocalAngle = 0;
			StartFlag = TRUE;
			MaterialNum = matNum;
			
			ColorStart.r	= 60.0f;
			ColorStart.g	= 60.0f;
			ColorStart.b	= 60.0f;
			

			ColorEnd.r	= 0.0f;
			ColorEnd.g	= 0.0f;
			ColorEnd.b	= 0.0f;
			

			AlphaStart	=  150.0f;
			AlphaEnd	=   10.0f;


			SizeStart	= 12.0f;
//			SizeVar		= 0.0f;
			SizeEnd		= 5.0f;

			Speed		= 150.0f;
//			SpeedVar	= 0.0f;

			Theta		= 180.0f;
			
			Life		= 0.6f;
//			LifeVar	    = 0.0f;
			
			GravityStart	=  0.0f;
//			GravityVar		=  0.0f;
			GravityEnd		=  0.0f;
			
			ParticlesPerSec	= EngineFps/2;
			
			Age = 0.4f;
			//	TimeLastUpdate = 0.0f;
			EmissionResidue = 0.0f;


			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			TimeCount = 0.0f;
			
			DestX = destX;
			DestY = destY;
			DestZ = destZ;
			
			LocalX = 0;
			LocalY = 0;
			LocalZ = 0;

			WorldX = (float)worldX;
			WorldY = (float)worldY;
			WorldZ = (float)worldZ;
			
			PrevLocation.x = Location.x;
			PrevLocation.y = Location.y;
			PrevLocation.z = Location.z;

			Location.x = worldX+LocalX;
			Location.y = worldY+LocalY;
			Location.z = worldZ+LocalZ;

			break;
		}

		case 2:
		{
			EndWaveFlag = FALSE;
			
			StartFlag = TRUE;
			MaterialNum = matNum;
			
			ColorStart.r	= 255.0f;
			ColorStart.g	= 255.0f;
			ColorStart.b	= 255.0f;
			
//			ColorVar.r	= 0.0f;
//			ColorVar.g	= 0.0f;
//			ColorVar.b	= 0.0f;

			ColorEnd.r	= 255.0f;
			ColorEnd.g	= 255.0f;
			ColorEnd.b	= 255.0f;
			

			AlphaStart	=  100.0f;
//			AlphaVar	=   0.0f;
			AlphaEnd	=   0.0f;

			SizeStart	= 8.0f;
//			SizeVar		= 1.0f;
			SizeEnd		= 3.0f;

			Speed		= 50.0f;
//			SpeedVar	= 0.0f;

			Theta		= 180.0f;
			
			Life		= 0.6f;
//			LifeVar	    = 0.0f;
			
			GravityStart	= 0.0f;
//			GravityVar		= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps*2;
			
			Age = 0.6f;
			//	TimeLastUpdate = 0.0f;
			EmissionResidue = 0.0f;

			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			TimeCount = 0.0f;
			
			DestX = destX;
			DestY = destY;
			DestZ = destZ;
			
			LocalX = 0;
			LocalY = 0;
			LocalZ = 0;

			WorldX = (float)worldX;
			WorldY = (float)worldY;
			WorldZ = (float)worldZ;
			
			PrevLocation.x = Location.x;
			PrevLocation.y = Location.y;
			PrevLocation.z = Location.z;

			Location.x = worldX+LocalX;
			Location.y = worldY+LocalY;
			Location.z = worldZ+LocalZ;
			break;
		}
	}

	return TRUE;
}

/*
int HoParticleDest::Start(int WorldX, int WorldY, int WorldZ, int destX, int destY, int destZ, int startLength, int matNum)
{
	//Ground Pike ��...
	StartLength = startLength;

	DestX = destX;
	DestY = destY;
	DestZ = destZ;


	StartFlag = TRUE;
	MaterialNum = matNum;
	AttractiveFlag = FALSE;

	ColorStart.r	= 100.0f;
	ColorStart.g	= 100.0f;
	ColorStart.b	= 100.0f;
	
	ColorVar.r	= 0.0f;
	ColorVar.g	= 0.0f;
	ColorVar.b	= 0.0f;

	ColorEnd.r	= 0.0f;
	ColorEnd.g	= 0.0f;
	ColorEnd.b	= 0.0f;
	

	AlphaStart	=  200.0f;
	AlphaVar	=   0.0f;
	AlphaEnd	=   0.0f;

	SizeStart	= 15.0f;
	SizeVar		= 1.0f;
	SizeEnd		= 5.0f;

	Speed		= 150.0f;
	SpeedVar	= 0.0f;

	Theta		= 180.0f;
	
	Life		= 0.5f;
	LifeVar	    = 0.0f;
	
	GravityStart	= 0.0f;
	GravityVar		= 0.0f;
	GravityEnd		= 0.0f;
	
	ParticlesPerSec	= EngineFps/2;
	
	Age = 0.8f;
	//	TimeLastUpdate = 0.0f;
	EmissionResidue = 0.0f;

	Direction.x = 0.0f;
	Direction.y = 0.0f;
	Direction.z = 0.0f;
	
	TimeCount = 0.0f;
	
	Location.x = WorldX;
	Location.y = WorldY;
	Location.z = WorldZ;
	
	PrevLocation.x = Location.x;
	PrevLocation.y = Location.y;
	PrevLocation.z = Location.z;
}
*/

/*
//�Ұ��� Ŭ����...
HoParticleFire::HoParticleFire()
{
	ColorStart.r	= 255.0f;
	ColorStart.g	= 200.0f;
	ColorStart.b	= 100.0f;
	
	ColorVar.r	= 0.0f;
	ColorVar.g	= 0.0f;
	ColorVar.b	= 0.0f;

	ColorEnd.r	= 255.0f;
	ColorEnd.g	= 0.0f;
	ColorEnd.b	= 0.0f;

	AlphaStart	=  200.0f;
	AlphaVar	=   0.0f;
	AlphaEnd	=  0.0f;

	SizeStart	= 8.0f;
	SizeVar		= 0.0f;
	SizeEnd		= 4.0f;

	Speed		= 20.0f;
	SpeedVar	= 0.0f;

	Theta		= 180.0f;
	
	Life		= 0.5f;
	LifeVar	    = 0.5f;
	
	
	GravityStart	= -0.15f;
	GravityVar		= 0.0f;
	GravityEnd		= -0.01f;
	


	ParticlesPerSec	= EngineFps;
	
	Age = 0.0f;

	EmissionResidue = 0.0f;

	Direction.x = 0.0f;
	Direction.y = 1.0f;
	Direction.z = 0.0f;
	
	Location.x = 0.0f;
	Location.y = 0.0f;
	Location.z = 0.0f;

	PrevLocation.x = 0.0f;
	PrevLocation.y = 0.0f;
	PrevLocation.z = 0.0f;
	
	TimeCount = 0.0f;
	
	//Test
	Location.x = 10000;
	Location.y = 10000;
	Location.z = -115*fONE;
	//Location.z = 0;

	PrevLocation.x = Location.x;
	PrevLocation.y = Location.y;
	PrevLocation.z = Location.z;	
}

HoParticleFire::~HoParticleFire()
{

}

int HoParticleFire::Main()
{
	HoParticleSystem::Main();
	
	return true;
}

int HoParticleFire::Draw(int x, int y, int z, int ax, int ay, int az)
{
	HoParticleSystem::Draw(x, y, z, ax, ay, az);
	return TRUE;
}
*/

void HoParticleSpark::Init()
{
	MaterialNum = CreateTextureMaterial( "spark.tga" ,0 , 0 , 0 ,0 , SMMAT_BLEND_LAMP );
}

void HoParticleSpark::Start(int x, int y, int z, int matNum, int type)
{
	switch(type)
	{
		case 0:
		{
			ColorStart.r	= 255.0f;
			ColorStart.g	= 255.0f;
			ColorStart.b	= 255.0f;
			
//			ColorVar.r	= 0.0f;
//			ColorVar.g	= 0.0f;
//			ColorVar.b	= 0.0f;

			ColorEnd.r	= 0.0f;
			ColorEnd.g	= 0.0f;
			ColorEnd.b	= 0.0f;
			

			AlphaStart	=  255.0f;
//			AlphaVar	=    0.0f;
			AlphaEnd	=    0.0f;

			SizeStart	= 4.0f;
//			SizeVar		= 1.0f;
			SizeEnd		= 6.0f;

			Speed		= 1000.0f;
//			SpeedVar	= 0.0f;

			Theta		= 180.0f;
			
			Life		= 0.5f;
//			LifeVar	    = 0.0f;
			
			GravityStart	= 0.05f;
//			GravityVar		= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps*2;
			
			Age = 0.15f;
			//	TimeLastUpdate = 0.0f;
			EmissionResidue = 0.0f;

			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			Location.x = 0.0f;
			Location.y = 0.0f;
			Location.z = 0.0f;

			PrevLocation.x = 0.0f;
			PrevLocation.y = 0.0f;
			PrevLocation.z = 0.0f;
			
			TimeCount = 0.0f;
			//Test
			Location.x = 0;
			Location.y = 10000;
			Location.z = -115*fONE;
			Location.z = 0;

			PrevLocation.x = Location.x;
			PrevLocation.y = Location.y;
			PrevLocation.z = Location.z;	

			break;			
		}

		case 1:
		{
			ColorStart.r	= 255.0f;
			ColorStart.g	= 255.0f;
			ColorStart.b	= 255.0f;
			
//			ColorVar.r	= 0.0f;
//			ColorVar.g	= 0.0f;
//			ColorVar.b	= 0.0f;

			ColorEnd.r	= 0.0f;
			ColorEnd.g	= 0.0f;
			ColorEnd.b	= 0.0f;
			

			AlphaStart	=  255.0f;
//			AlphaVar	=    0.0f;
			AlphaEnd	=  0.0f;

			SizeStart	= 4.0f;
//			SizeVar		= 1.0f;
			SizeEnd		= 6.0f;

			Speed		= 600.0f;
//			SpeedVar	= 0.0f;

			Theta		= 180.0f;
			
			Life		= 0.3f;
//			LifeVar	    = 0.0f;
			
			GravityStart	= 0.05f;
//			GravityVar		= 0.0f;
			GravityEnd		= 0.0f;
			
			ParticlesPerSec	= EngineFps*4;
			
			Age = 0.15f;
			//	TimeLastUpdate = 0.0f;
			EmissionResidue = 0.0f;

			Direction.x = 0.0f;
			Direction.y = 0.0f;
			Direction.z = 0.0f;
			
			Location.x = 0.0f;
			Location.y = 0.0f;
			Location.z = 0.0f;

			PrevLocation.x = 0.0f;
			PrevLocation.y = 0.0f;
			PrevLocation.z = 0.0f;
			
			TimeCount = 0.0f;
			//Test
			Location.x = 0;
			Location.y = 10000;
			Location.z = -115*fONE;
			Location.z = 0;

			PrevLocation.x = Location.x;
			PrevLocation.y = Location.y;
			PrevLocation.z = Location.z;
			
			break;
		}
	}
	
	MaterialNum = matNum;
	StartFlag = TRUE;
	
	WorldX = (float)x;
	WorldY = (float)y;
	WorldZ = (float)z;

	LocalX = 0;
	LocalY = 0;
	LocalZ = 0;


	Location.x = (float)x;
	Location.y = (float)y;
	Location.z = (float)z;
	

	PrevLocation.x = Location.x;
	PrevLocation.y = Location.y;
	PrevLocation.z = Location.z;
}

//Spark���� Ŭ����
HoParticleSpark::HoParticleSpark()
{
	ColorStart.r	= 255.0f;
	ColorStart.g	= 255.0f;
	ColorStart.b	= 255.0f;
	
//	ColorVar.r	= 0.0f;
//	ColorVar.g	= 0.0f;
//	ColorVar.b	= 0.0f;

	ColorEnd.r	= 0.0f;
	ColorEnd.g	= 0.0f;
	ColorEnd.b	= 0.0f;
	

	AlphaStart	=  255.0f;
//	AlphaVar	=    0.0f;
	AlphaEnd	=    0.0f;

	SizeStart	= 4.0f;
//	SizeVar		= 1.0f;
	SizeEnd		= 6.0f;

	Speed		= 1000.0f;
//	SpeedVar	= 0.0f;

	Theta		= 180.0f;
	
	Life		= 0.5f;
//	LifeVar	    = 0.0f;
	
	GravityStart	= 0.05f;
//	GravityVar		= 0.0f;
	GravityEnd		= 0.0f;
	
	ParticlesPerSec	= EngineFps*2;
	
	Age = 0.15f;
	//	TimeLastUpdate = 0.0f;
	EmissionResidue = 0.0f;

	Direction.x = 0.0f;
	Direction.y = 0.0f;
	Direction.z = 0.0f;
	
	Location.x = 0.0f;
	Location.y = 0.0f;
	Location.z = 0.0f;

	PrevLocation.x = 0.0f;
	PrevLocation.y = 0.0f;
	PrevLocation.z = 0.0f;
	
	TimeCount = 0.0f;
	//Test
	Location.x = 0;
	Location.y = 10000;
	Location.z = -115*fONE;
	Location.z = 0;

	PrevLocation.x = Location.x;
	PrevLocation.y = Location.y;
	PrevLocation.z = Location.z;	
}

HoParticleSpark::~HoParticleSpark()
{

}


int HoParticleSpark::Draw(int x, int y, int z, int ax, int ay, int az)
{
	if(StartFlag == FALSE)
		return FALSE;

	smRender.Init();
	smRender.SetMaterialGroup(smMaterialGroup);
	smMaterialGroup->smMaterial[MaterialNum].TwoSide = TRUE;

	//Camera���� �Է�.
	smRender.SetCameraPosi(x, y, z, ax, ay, az);
	
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		if(Particles[i].Age >= 0.0f)
		{
			Face2d.x = int(Particles[i].Location.x);
			Face2d.y = int(Particles[i].Location.y);
			Face2d.z = int(Particles[i].Location.z);
			
			Face2d.r = int(Particles[i].Color.r);
			Face2d.g = int(Particles[i].Color.g);
			Face2d.b = int(Particles[i].Color.b);
			Face2d.Transparency = int(Particles[i].Alpha);
			
			Face2d.TexRect.left   = 0;
			Face2d.TexRect.top    = 0;
			Face2d.TexRect.right  = 1;
			Face2d.TexRect.bottom = 1;
			
			
			Face2d.width  = int(Particles[i].Size) << FLOATNS;
			Face2d.height = int(Particles[i].Size) << FLOATNS;

			Face2d.MatNum = MaterialNum;
			
			AddFace2D(&Face2d, i);
		}
	}
	
	//lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
	//lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZFUNC,   D3DCMP_LESSEQUAL);
	//lpD3DDevice->SetRenderState( D3DRENDERSTATE_LASTPIXEL, TRUE );

	smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
	smRender.ClipRendFace();
	smRender.GeomVertex2D(FALSE);
	smRender.RenderD3D();

	//lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
	//lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE );

	//lpD3DDevice->SetRenderState( D3DRENDERSTATE_LASTPIXEL, FALSE );
	return TRUE;
}

//����� define ���� ���ϴ� �����尡 ���...���⿡ �����Ѵ�.
#define FLOATS_EM				(FLOATNS+3)
#define SM_DIST_NEARZ			16
#define SMFLOAT_DIST_NEARZ		(SM_DIST_NEARZ<<FLOATNS)

//������ ����..
bool HoParticleSpark::AddFace2D(smFACE2D *face, int particleIndex)
{
	if(Particles[particleIndex].Age < 0.0f)
		return false;

	D3DVECTOR begProj=Particles[particleIndex].PrevLocation;
	//D3DVECTOR endProj=Particles[particleIndex].Location;
	D3DVECTOR endProj=Location;
	
	

	int x,y,z;
	int rx , ry, rz;
	int width , height;
	
	x = (int)begProj.x;
	y = (int)begProj.y;
	z = (int)begProj.z;

	//�߽� ��ǥ�� ī�޶� ��ǥ�� ��ȯ
	x -= smRender.CameraPosiX;
	y -= smRender.CameraPosiY;
	z -= smRender.CameraPosiZ;

	rx = x * smRender.mCamera._11 +
		 y * smRender.mCamera._21 +
		 z * smRender.mCamera._31 ;

	ry = x * smRender.mCamera._12 +
		 y * smRender.mCamera._22 +
		 z * smRender.mCamera._32 ;

	rz = x * smRender.mCamera._13 +
		 y * smRender.mCamera._23 +
		 z * smRender.mCamera._33 ;

	begProj.x = float(rx>>FLOATS_EM);
	begProj.y = float(ry>>FLOATS_EM);
	begProj.z = float(rz>>FLOATS_EM);

//	z -= 12*fONE;
	begProj.z -= 24*fONE;
	if ( begProj.z<SMFLOAT_DIST_NEARZ ) return false;


	//begProj.x = x;
	//begProj.y = y;
	//begProj.z = z;
	
	x = (int)endProj.x;
	y = (int)endProj.y;
	z = (int)endProj.z;
	//�߽� ��ǥ�� ī�޶� ��ǥ�� ��ȯ
	x -= smRender.CameraPosiX;
	y -= smRender.CameraPosiY;
	z -= smRender.CameraPosiZ;

	rx = x * smRender.mCamera._11 +
		 y * smRender.mCamera._21 +
		 z * smRender.mCamera._31 ;

	ry = x * smRender.mCamera._12 +
		 y * smRender.mCamera._22 +
		 z * smRender.mCamera._32 ;

	rz = x * smRender.mCamera._13 +
		 y * smRender.mCamera._23 +
		 z * smRender.mCamera._33 ;

	endProj.x = float(rx>>FLOATS_EM);
	endProj.y = float(ry>>FLOATS_EM);
	endProj.z = float(rz>>FLOATS_EM);

//	z -= 12*fONE;
	endProj.z -= 24*fONE;
	if ( endProj.z<SMFLOAT_DIST_NEARZ ) return false;
	//endProj.x = x;
	//endProj.y = y;
	//endProj.z = z;

	
	
	/*
	int x,y,z;
	int rx , ry, rz;
	int width , height;
	*/

	smRENDVERTEX *v[4];
	smRENDFACE	 *rf;
	short		sColor[4];

	sColor[ SMC_A ] = face->Transparency;
	sColor[ SMC_R ] = face->r;
	sColor[ SMC_G ] = face->g;
	sColor[ SMC_B ] = face->b;

	x = face->x;
	y = face->y;
	z = face->z;

	//�߽� ��ǥ�� ī�޶� ��ǥ�� ��ȯ
	x -= smRender.CameraPosiX;
	y -= smRender.CameraPosiY;
	z -= smRender.CameraPosiZ;

	rx = x * smRender.mCamera._11 +
		 y * smRender.mCamera._21 +
		 z * smRender.mCamera._31 ;

	ry = x * smRender.mCamera._12 +
		 y * smRender.mCamera._22 +
		 z * smRender.mCamera._32 ;

	rz = x * smRender.mCamera._13 +
		 y * smRender.mCamera._23 +
		 z * smRender.mCamera._33 ;

	x = rx>>FLOATS_EM;
	y = ry>>FLOATS_EM;
	z = rz>>FLOATS_EM;

//	z -= 12*fONE;
	z -= 24*fONE;


	if ( z<SMFLOAT_DIST_NEARZ ) return false;

	width  = face->width >> 1;
	height = face->height >> 1;

	//������ ���ؽ� �߰� ( ��ǥ ���� �Է� )
	/*
	v[0] = smRender.AddRendVertex ( x-width , y-height , z, sColor );
	v[1] = smRender.AddRendVertex ( x+width , y-height , z, sColor );
	v[2] = smRender.AddRendVertex ( x-width , y+height , z, sColor );
	v[3] = smRender.AddRendVertex ( x+width , y+height , z, sColor );
	*/
	
	float dx    = endProj.x - begProj.x;
	float dy    = endProj.y - begProj.y;
	float oolen = (float)sqrt(dx*dx+dy*dy);

	dx = dx/oolen*150;
	dy = dy/oolen*150;
	
	//dx = dx*5;
	//dy = dy*5;

	//if(oolen > 100)
		//oolen = oolen*0.000005;

	//float oolen = *0.000005;
	
	D3DVECTOR persp;
	persp.x = -dy;
	persp.y = +dx;
	persp.z = 0;
	
	if (abs((int)dx) > 1.0f) begProj.x -= (int)dx;
	if (abs((int)dy) > 1.0f) begProj.y -= (int)dy;
	
	//Stretch
	v[0] = smRender.AddRendVertex ( int(endProj.x-persp.x),  int(endProj.y-persp.y), int(endProj.z), sColor);
	v[1] = smRender.AddRendVertex ( int(endProj.x+persp.x),  int(endProj.y+persp.y), int(endProj.z), sColor);
	v[3] = smRender.AddRendVertex ( int(begProj.x+persp.x),  int(begProj.y+persp.y), int(begProj.z), sColor);
	v[2] = smRender.AddRendVertex ( int(begProj.x-persp.x),  int(begProj.y-persp.y), int(begProj.z), sColor);
	
	//ù��° ������ ( 1 - 2 - 3 );
	rf = &smRender.RendFace[ smRender.nRendFace++ ];
	rf->lpRendVertex[0] = v[0];
	rf->lpRendVertex[1] = v[1];
	rf->lpRendVertex[2] = v[2];
	rf->Matrial    = face->MatNum ;		//��Ʈ���� ����
	rf->ClipStatus = v[0]->ClipStatus|v[1]->ClipStatus|v[2]->ClipStatus;
	rf->lpTexLink  = 0;

	
	//�ؽ��� ����
	smRender.AddRendTempTexLink( rf , 0,	face->TexRect.left , face->TexRect.bottom,
								face->TexRect.right , face->TexRect.bottom,
								face->TexRect.left , face->TexRect.top );

	
	//�ι�° ������ ( 2 - 4 - 3 );
	rf = &smRender.RendFace[ smRender.nRendFace++ ];
	rf->lpRendVertex[0] = v[1];
	rf->lpRendVertex[1] = v[3];
	rf->lpRendVertex[2] = v[2];

	/*
	rf->lpRendVertex[0] = v[1];
	rf->lpRendVertex[1] = v[3];
	rf->lpRendVertex[2] = v[2];
	*/

	rf->Matrial    = face->MatNum ;		//��Ʈ���� ����
	rf->ClipStatus = v[1]->ClipStatus|v[2]->ClipStatus|v[3]->ClipStatus;
	rf->lpTexLink  = 0;

	//�ؽ��� ����
	smRender.AddRendTempTexLink( rf , 0,	face->TexRect.right , face->TexRect.bottom,
								face->TexRect.right , face->TexRect.top,
									face->TexRect.left , face->TexRect.top );
	return true;
}




