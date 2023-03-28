#include "MassSpringSystem.h"

MASS_SPRING_SYSTEM::MASS_SPRING_SYSTEM(int maxParticles,int maxSprings)
{
    init(maxParticles, maxSprings);
}

void MASS_SPRING_SYSTEM::init(int maxParticles,int maxSprings) 
{
    m_Gravity = Vector3(0.0, -9.8, 0.0);
    m_FlgCollisionConstraints = false;
	
    m_MaxParticles = maxParticles;
    m_CurParticles = 0;
    mParticles = new PARTICLE[m_MaxParticles];
    //
    m_MaxSprings = maxSprings;
    m_CurSprings = 0;
    mSprings = new SPRING[m_MaxSprings];
	
	
	Kd = 0.5;
	Ks = 0.25;

	epsilon = 0.05f;
}

int MASS_SPRING_SYSTEM::addParticle(SceneNode *n, Real radius, bool movable)
{
	if(m_CurParticles > m_MaxParticles)
		return -1;
	mParticles[m_CurParticles].snode = n;
	mParticles[m_CurParticles].radius = radius;
	mParticles[m_CurParticles].movable = movable;
	m_CurParticles ++;
    return m_CurParticles - 1;    
}

SPRING *MASS_SPRING_SYSTEM::addSpring(int n0, int n1)
{
	SPRING *s = 0;
	if(m_CurSprings > m_MaxSprings)
		return s;
    
	SPRING tmpString = SPRING();
	tmpString.n0 = n0;
	tmpString.n1 = n1;
	
	Vector3 p0 = mParticles[n0].snode->getPosition();
	Vector3 p1 = mParticles[n1].snode->getPosition();
	tmpString.L0 = (p1 - p0).length();

	Vector3 n = (p1 - p0) / (p1 - p0).length();
	tmpString.n = n;

	s = &tmpString;

	mSprings[m_CurSprings] = tmpString;

	m_CurSprings++;

    return s;
}

void MASS_SPRING_SYSTEM::setGravity(const Vector3 &g)
{
    m_Gravity = g;
}

//
// reset force
// compute damping force
// compute viscous damping force between particles
// compute spring force
// compute net force
void MASS_SPRING_SYSTEM::computeForce(float time_step)
{
	// reset force
    for (int i = 0; i < m_CurParticles; ++i ) 
	{
        mParticles[i].f = Vector3::ZERO;
    }

	// compute damping force
	for (int i = 0; i < m_CurParticles; ++i ) 
	{
		Vector3 dampingForce = -1.0 * Kd * mParticles[i].velocity;
		mParticles[i].f += dampingForce;
	}

	// compute viscous damping force between particles
	for(int i = 0; i < m_CurSprings; ++i)
	{
		int p0 = mSprings[i].n0;
		int p1 = mSprings[i].n1;

		Vector3 v0 = mParticles[p0].velocity;
		Vector3 v1 = mParticles[p1].velocity;

		Vector3 viscousDampingForce;
		
		viscousDampingForce = -1.0 * Ks * (v0 - v1);
		mParticles[p0].f += viscousDampingForce;

		viscousDampingForce = -1.0 * Ks * (v1 - v0);
		mParticles[p1].f += viscousDampingForce;
	}

	// compute spring force
	for(int i = 0; i < m_CurSprings; ++i)
	{
		int p0 = mSprings[i].n0;
		int p1 = mSprings[i].n1;
		
		Vector3 position0 = mParticles[p0].snode->getPosition();
		Vector3 position1 = mParticles[p1].snode->getPosition();
		
		// n(1,0) = (p1 - p0)/ || p1 - p0 ||
		Vector3 n = (position1 - position0) / (position1 - position0).length();
		
		// f(0,1)= k (|| p1 - p0 || - L0) n(1,0)
		Vector3 springForce = mSprings[i].k * ((position1 - position0).length() - mSprings[i].L0) * n;
		
		// Dealing with numeric error
		if(springForce.length() > epsilon)
		{
			if(springForce.length() > m_Gravity.length())
			{
				springForce /= springForce.length();
				springForce *= m_Gravity.length();
			}
			mParticles[p0].f += springForce;
		}
	}

	int numCircles = 15;
	// compute net force
	for (int i = 0; i < m_CurParticles; ++i ) 
	{
		if(i % numCircles == 0)
		{
			mParticles[i].f = 0;
			continue;
		}

		if(i > 15)
		{
			//mParticles[i].f = 0;
			//continue;
		}

		Vector3 gravityForce = mParticles[i].mass * m_Gravity;
		mParticles[i].f += gravityForce;
	}

}

void MASS_SPRING_SYSTEM::computeVelocity(float time_step)
{
	for (int i = 0; i < m_CurParticles; ++i ) 
	{
		Vector3 v = mParticles[i].velocity;
		mParticles[i].velocity = v + (mParticles[i].f / mParticles[i].mass) * time_step;
	}
}

void MASS_SPRING_SYSTEM::computePosition(float time_step)
{
	for (int i = 0; i < m_CurParticles; ++i ) 
	{
		Vector3 p = mParticles[i].snode->getPosition();
		p = p + mParticles[i].velocity * time_step;
		mParticles[i].snode->setPosition(p);
	}
}

void MASS_SPRING_SYSTEM::adjustVelocityDueToCollisionConstraint_Floor(float time_step)
{
    for(int i = 0; i < m_CurParticles; ++i)
	{
		Vector3 position0 = mParticles[i].snode->getPosition();
		
		if(position0.y <= epsilon)
		{
			mParticles[i].velocity.y = 0;
		}
	}
}

void MASS_SPRING_SYSTEM::adjustVelocityDueToCollisionConstraint_LargeSphere(float time_step)
{
	Vector3 LargeSpherePos = Vector3(.0, .0, .0);
    float large_radius = 100.0;
    float small_radius = 5.0;

	for(int i = 0; i < m_CurParticles; ++i)
	{
		Vector3 position0 = mParticles[i].snode->getPosition();
		Vector3 position1 = LargeSpherePos;
		
		if((position0 - position1).length() <= (epsilon + large_radius + small_radius))
		{
			Vector3 n = (position1 - position0) / (position1 - position0).length();
			if(n.dotProduct(mParticles[i].velocity) > 0)
			{
				mParticles[i].velocity = mParticles[i].velocity - n.dotProduct(mParticles[i].velocity) * n;
			}

			if(mParticles[i].velocity.length() < epsilon)
			{
				mParticles[i].velocity = 0;
			}
		}
	}

	// between small sphere
	for(int i = 0; i < m_CurSprings; ++i)
	{
		// i
		int p0 = mSprings[i].n0;
		// j
		int p1 = mSprings[i].n1;

		Vector3 position0 = mParticles[p0].snode->getPosition();
		Vector3 position1 = mParticles[p1].snode->getPosition();

		float distance = (position0 - position1).length();

		// n(1,0) = (p1 - p0)/ || p0 - p1 ||
		Vector3 n = (position1 - position0) / (position1 - position0).length();
		if(distance < (small_radius * 2 + epsilon)){
			if(n.dotProduct(mParticles[p1].velocity) > 0)
			{
				mParticles[p1].velocity = mParticles[p1].velocity - n.dotProduct(mParticles[p1].velocity) * n;
			}
		
			if(n.dotProduct(mParticles[p0].velocity) > 0)
			{
				mParticles[p0].velocity = mParticles[p0].velocity - n.dotProduct(mParticles[p0].velocity) * n;
			}
		}

		// Dealing Numeric Concussion
		if(mParticles[p1].velocity.length() < epsilon)
			mParticles[p1].velocity = 0;
		if(mParticles[p0].velocity.length() < epsilon)
			mParticles[p0].velocity = 0;
	}

}

void MASS_SPRING_SYSTEM::adjustVelocityUnRubust(float time_step)
{
	Vector3 LargeSpherePos = Vector3(.0, .0, .0);
		float large_radius = 100.0;
		float small_radius = 5.0;
    
		for(int i = 0; i < m_CurParticles; ++i)
		{
			Vector3 position0 = mParticles[i].snode->getPosition();
			Vector3 position1 = LargeSpherePos;
		
			if((position0 - position1).length() <= (large_radius + small_radius))
			{
				Vector3 n = (position1 - position0) / (position1 - position0).length();

				if(n.dotProduct(mParticles[i].velocity) > 0)
				{
					mParticles[i].velocity = mParticles[i].velocity - n.dotProduct(mParticles[i].velocity) * n * 2.5;
				}
			}
		}

		for(int i = 0; i < m_CurSprings; ++i)
		{
			int p0 = mSprings[i].n0;
			int p1 = mSprings[i].n1;

			Vector3 position0 = mParticles[p0].snode->getPosition();
			Vector3 position1 = mParticles[p1].snode->getPosition();

			float distance = (position0 - position1).length();

			Vector3 n = (position1 - position0) / (position1 - position0).length();
			if(distance < (small_radius * 2)){
				if(n.dotProduct(mParticles[p1].velocity) > 0)
				{
					mParticles[p1].velocity = mParticles[p1].velocity - n.dotProduct(mParticles[p1].velocity) * n;
				}
		
				if(n.dotProduct(mParticles[p0].velocity) > 0)
				{
					mParticles[p0].velocity = mParticles[p0].velocity - n.dotProduct(mParticles[p0].velocity) * n;
				}
			}
		}
}

void MASS_SPRING_SYSTEM::adjustVelocityDueToCollisionConstraint(float time_step)
{
    adjustVelocityDueToCollisionConstraint_Floor(time_step);
    adjustVelocityDueToCollisionConstraint_LargeSphere(time_step);
}

void MASS_SPRING_SYSTEM::reset()
{
    resetVelocity();
}

void MASS_SPRING_SYSTEM::resetVelocity()
{
    if (m_CurParticles < 0) return;
    for (int i = 0; i < m_CurParticles; ++i ) {
        PARTICLE *p = &mParticles[i];
        p->velocity = Vector3::ZERO;
    }
}

void MASS_SPRING_SYSTEM::setCollisionConstraint(bool enable)
{
    m_FlgCollisionConstraints = enable;
}

void MASS_SPRING_SYSTEM::update(float time_step) {
    computeForce(time_step);
    computeVelocity(time_step);
    
	if (m_FlgCollisionConstraints) 
		adjustVelocityDueToCollisionConstraint(time_step);
	else
		adjustVelocityUnRubust(time_step);
    computePosition(time_step);
}