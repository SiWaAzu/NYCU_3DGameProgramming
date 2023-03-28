#include "monsters.h"
#include "BasicTools.h"
#include "read_data.h"

MONSTER::MONSTER(SceneManager *a_SceneMgr) : GAME_OBJ(a_SceneMgr)
{
}

//
// Make the monster look at mTarget
//
void MONSTER::updateViewDirection()
{
	if (mTarget == 0) return;
	Vector3 pos = mTarget->getPosition();
	Vector3 target_pos = mSceneNode->getPosition();

	Vector3 dir = target_pos - pos;
	Vector3 src = mSceneNode->getOrientation() * Vector3::UNIT_Z;
	src.normalise();
	dir.normalise();
	src.y = 0;
	dir.y = 0;
	Ogre::Quaternion quat = src.getRotationTo(dir);
	if (DATA_READER::getMeshName() == "penguin.mesh")
		mSceneNode->yaw(Ogre::Degree(180));

	else
		mSceneNode->yaw(Ogre::Degree(90));
	
    mSceneNode->rotate(quat);
    //
    // Add your own stuff
    // 
}

//
// Update the position of the monster.
//
void MONSTER::update(const Ogre::FrameEvent& evt)
{
	Vector3 mv = mInitPosition - mTarget->getPosition();
    double dt = evt.timeSinceLastFrame;

    Vector3 new_pos= mSceneNode->getPosition();
	Vector3 dir = mTarget->getPosition() - mInitPosition;

	dir = dir.normalisedCopy();
	dir.y = 0;
	float dis = mv.length();

	if(dis >= 100) new_pos+=dir*100*dt;
	mInitPosition = new_pos;

	angle = angle + dt;
	if(angle >= 3.14159 * 2) angle = 0;
	Vector3 wave(0,0,0);
	wave.y = 0.1 * sin(angle);
	new_pos += wave;
	

	mSceneNode->setPosition(new_pos);

    // Add your own stuff
    // mSceneNode->setPosition(new_pos);
    //
updateViewDirection();
}