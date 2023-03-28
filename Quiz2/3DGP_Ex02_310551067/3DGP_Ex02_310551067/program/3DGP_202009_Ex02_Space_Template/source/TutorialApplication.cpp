//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2019/10/28
//
#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

const float PI = 3.141592654;

#define mMoveDirection_NONE 0
#define mMoveDirection_DOWN (1 << 0)
#define mMoveDirection_UP   (1 << 1)
#define mMoveDirection_LEFT (1 << 2)
#define mMoveDirection_RIGHT (1<<3)


BasicTutorial_00::BasicTutorial_00(void)
    : 
mMoveDirection(mMoveDirection_NONE)
{}

void BasicTutorial_00::chooseSceneManager()
{
	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	mSceneMgrArr[1] = mRoot
		->createSceneManager(ST_GENERIC, "secondary");
}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(120,300,600));
	mCamera->lookAt(Ogre::Vector3(120,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,350,0.0));
	mCamera->lookAt(Ogre::Vector3(0.00001,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller

}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0.0,1.0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[0] = vp;
}

void BasicTutorial_00::createViewport_01(void)
{
}

void BasicTutorial_00::resolveCollision(
    SceneNode *nodeA, SceneNode *nodeB,
    float rA, float rB, float wA, float wB)
{
    Vector3 posA = nodeA->getPosition();
    Vector3 posB = nodeB->getPosition();
    float R = rA + rB;
    ///////////////////////
    // add your own stuff
    ///////////////////////
}

void BasicTutorial_00::resolveCollisionLargeSphere()
{
    float smallR = 15; // small sphere radius
    float largeR = 1.0/0.15*smallR; // large sphere radius
	Vector3 PosLarge = mLargeSphereSceneNode ->getPosition();

	for (int i =0; i<mNumSpheres; i++){
		push[i]=false;
		Vector3 pos = mSceneNode[i]->getPosition();
		Real disSqr = pos.squaredDistance(PosLarge);
		if (disSqr<=(smallR+largeR)*(smallR+largeR)+30){
			Vector3 StoL = (PosLarge-pos);
			StoL.normalise();
			Vector3 dir = curVec;
			dir.normalise();
			Vector3 v = -(StoL+dir);
			Real test = v.normalise();
			if (test==0) mSceneNode[i]->translate(dir*3);
			else mSceneNode[i]->translate(curVec+v*2);
			push[i]=true;
		}
	}
}

// perform collision handling for all pairs
void BasicTutorial_00::resolveCollisionSmallSpheres()
{
    float ri = 15; // sphere radius
    float rj = 15; // sphere radius
    for (int i = 0; i < mNumSpheres; ++i)
	{
		Vector3 posI = mSceneNode[i]->getPosition();
		for (int j = i+1; j < mNumSpheres; ++j) {
			Vector3 posJ = mSceneNode[j]->getPosition();

			Real disSqr = posI.distance(posJ);
			if (disSqr<=(ri+rj)-2 ){
				Vector3 v =posI-posJ;
				v.normalise();
				if (push[i] == true){
					if (posJ.x>600 || posJ.x<-600)	v.x=0;
					if (posJ.z>600 || posJ.z<-600) v.z=0;
					mSceneNode[j]->translate(-v*1.5);
					push[j] = true;
				}
				else{
					if (posI.x>600 || posI.x<-600)	v.x=0;
					if (posI.z>600 || posI.z<-600) v.z=0;
					mSceneNode[i]->translate(v*1.5);
					push[i] = true;
				}
			}
		}
		if(posI.x<-580)	mSceneNode[i]->translate(Vector3(1,0,0));
		if(posI.x>580)	mSceneNode[i]->translate(Vector3(-1,0,0));
		if(posI.z<-580)	mSceneNode[i]->translate(Vector3(0,0,1));
		if(posI.z>580)	mSceneNode[i]->translate(Vector3(0,0,-1));

            ///////////////////////
            // add your own stuff
            ///////////////////////
      }
}

void BasicTutorial_00::resolveCollision()
{
	resolveCollisionLargeSphere();
	resolveCollisionSmallSpheres();
    /*int num = mNumSpheres;
    for (int i = 0; i < num;++i) {
        resolveCollisionSmallSpheres();
        resolveCollisionLargeSphere();
    }*/
}

// reset positions of all small spheres
void BasicTutorial_00::reset()
{
    for (int i = 0; i < mNumSpheres; ++i ) {
        ///////////////////////
        // add your own stuff
        ///////////////////////
    }
}

void BasicTutorial_00::posCreate(){
	Vector3 largePos = mLargeSphereSceneNode ->getPosition();
	int i,j;
	//srand(time(0));
	for (i =1 ;i<=500 ;i++){
		do{
			posMem[i-1] =Vector3(-400+(rand()%30)*30,0,-400+(rand()%30)*30);
			for(j=1 ;j<i ;j++){
				if (posMem[i-1].x == posMem[j-1].x && posMem[i-1].z == posMem[j-1].z)
					break;
				if (posMem[i-1].x>largePos.x-105 && posMem[i-1].x<largePos.x+105 && posMem[i-1].z>largePos.z-105 && posMem[i-1].z<largePos.z+105)
					break;
			}
		}while(j!=i);
	}
}
// create all spheres
// "Examples/red"
// "Examples/green"
// "Examples/blue"
// "Examples/yellow"
void BasicTutorial_00::createSpace()
{
    //String name_en;
    String name_sn;
	mNumSpheres=500;
	posCreate();
    
    for (int i = 0; i < mNumSpheres; ++i ) {

//        genNameUsingIndex("R1", index, name_en);
//    genNameUsingIndex("S1", index, name_sn);
//    mEntity[index] = mSceneMgr
//		->createEntity( name_en, "sphere.mesh" );

         ///////////////////////
        // add your own stuff
        ///////////////////////
		vis[i]=true;
		mSceneNode[i]= mSceneMgr ->getRootSceneNode()->createChildSceneNode();
		genNameUsingIndex("S1", i, name_sn);
		mEntity[i] = mSceneMgr->createEntity(name_sn,"sphere.mesh");
		mSceneNode[i]->attachObject(mEntity[i]);
		mSceneNode[i]->setPosition(posMem[i]);
		mSceneNode[i]->scale(0.15, 0.15, 0.15);

    switch(rand()%3) {
    case 0:
        mEntity[i]->setMaterialName("Examples/red");
        break;
    case 1:
		mEntity[i]->setMaterialName("Examples/green");
        break;
    case 2:
		mEntity[i]->setMaterialName("Examples/blue");
        break;
    }

    //scale the small spheres
    //mSceneNode[index]->scale(0.15, 0.15, 0.15);
    }

    
}

void BasicTutorial_00::createScene_00(void) 
{
	mNumSpheres = 500;
	//changeFlag = false;
	mSceneMgr = mSceneMgrArr[0];
	//mSceneMgr->setAmbientLight( ColourValue( 0.25, 0.25, 0.25 ) ); 

	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	Light *light;
	light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(150, 250, 100)); 
	light->setDiffuseColour(0.3, 0.3, 0.3);		
	light->setSpecularColour(0.5, 0.5, 0.5);	

	light = mSceneMgr->createLight("Light2"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(-150, 300, 250)); 
	light->setDiffuseColour(0.25, 0.25, 0.25);		
	light->setSpecularColour(0.35, 0.35, 0.35);	

	//
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	//ent->setMaterialName("Examples/Rockwall");


	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 

    ent = mSceneMgr->createEntity(
		"Background", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	ent->setMaterialName("Examples/Rockwall");

    //Radian angle((45/180.0)*3.141592654);
    Radian angle(3.141952654/2.0);

    Vector3 axis(1.0, 0.0, 0.0);
    mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
            Vector3(0.0, 0.0, -750.0),
            Quaternion( angle, axis))
		->attachObject(ent);

	//float r_obs = 28.0;
	float d_gap = 30.0;
	int offset = 0;
	for (int i = 0;i <20; i++){
		int index = i+ offset;
		float x = -600+2*d_gap;
		float y = 10;
		float z =-600;
		String name_en;
		x+=2*d_gap*i;
		
		genNameUsingIndex("W1", index, name_en);
		wallEnt[index] = mSceneMgr->createEntity(name_en,"barrel.mesh");
		wallBarrel[index] = mSceneMgr ->getRootSceneNode()->createChildSceneNode();
		wallBarrel[index]->attachObject(wallEnt[index]);
		wallBarrel[index]->setPosition(x,y,z);
		wallBarrel[index]->scale(10,10,10);
	}
	offset+=20;
	for (int i = 0;i <20; i++){
		int index = i+ offset;
		float x = -600;
		float y = 10;
		float z =-600;
		String name_en;
		z+=2*d_gap*i;
		
		genNameUsingIndex("W1", index, name_en);
		wallEnt[index] = mSceneMgr->createEntity(name_en,"barrel.mesh");
		wallBarrel[index] = mSceneMgr ->getRootSceneNode()->createChildSceneNode();
		wallBarrel[index]->attachObject(wallEnt[index]);
		wallBarrel[index]->setPosition(x,y,z);
		wallBarrel[index]->scale(10,10,10);
	}
	offset+=20;
	for (int i = 0;i <20; i++){
		int index = i+ offset;
		float x = -600;
		float y = 10;
		float z =600;
		String name_en;
		x+=2*d_gap*i;
		
		genNameUsingIndex("W1", index, name_en);
		wallEnt[index] = mSceneMgr->createEntity(name_en,"barrel.mesh");
		wallBarrel[index] = mSceneMgr ->getRootSceneNode()->createChildSceneNode();
		wallBarrel[index]->attachObject(wallEnt[index]);
		wallBarrel[index]->setPosition(x,y,z);
		wallBarrel[index]->scale(10,10,10);
	}
	offset+=20;
	for (int i = 0;i <20; i++){
		int index = i+ offset;
		float x = 600;
		float y = 10;
		float z =-600+2*d_gap;
		String name_en;
		z+=2*d_gap*i;
		
		genNameUsingIndex("W1", index, name_en);
		wallEnt[index] = mSceneMgr->createEntity(name_en,"barrel.mesh");
		wallBarrel[index] = mSceneMgr ->getRootSceneNode()->createChildSceneNode();
		wallBarrel[index]->attachObject(wallEnt[index]);
		wallBarrel[index]->setPosition(x,y,z);
		wallBarrel[index]->scale(10,10,10);
	}

	mLargeSphereSceneNode = mSceneMgr ->getRootSceneNode() ->createChildSceneNode();
	mLargeSphereEntity = mSceneMgr ->createEntity("largeSphere","sphere.mesh");
	mLargeSphereEntity ->setMaterialName("Examples/yellow");
	mLargeSphereSceneNode ->attachObject(mLargeSphereEntity);
	mLargeSphereSceneNode ->setPosition(0,0,0);


    createSpace();

    ///////////////////////
        // add your own stuff
        ///////////////////////
    
    resolveCollision();
}

void BasicTutorial_00::createScene_01(void) 
{

}

void BasicTutorial_00::createViewports(void)
{
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
    //
    mCameraMan->getCamera()
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
//
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyPressed( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyPressed ***\n";
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    
    if (arg.key == OIS::KC_C ) {
        
        //How to clear ss?
        ss.str("");
        ss.clear();
        
        //stringstream ss; // Second way

        // Third way?
        //=============

        // How to get camerea position?
        //-----------------------------
        //This is incorrect.
        //Vector3 pos = mCamera->getPosition();
        //-----------------------------
        Vector3 pos = mCameraMan->getCamera()->getPosition(); //Correct
        ss << std::fixed << std::setprecision(2) 
            << "CameraPosition:" 
            << pos.x << "\t" 
            << pos.y << "\t" 
            << pos.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
        ss.str("");
        ss.clear();
        Vector3 dir = mCameraMan->getCamera()->getDirection();
        ss << std::fixed << std::setprecision(2) 
            << "CameraDirection:" 
            << dir.x << "\t" 
            << dir.y << "\t" 
            << dir.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
    }

    if (arg.key == OIS::KC_1 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(-77.89,	169.11,	1996.7));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.05,	-0.25,	-0.97));

    }

    if (arg.key == OIS::KC_2 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(-824.52,	468.58,	68.45));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.94,	-0.31,	-0.11));

        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
    }


    if (arg.key == OIS::KC_3 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(0.01,	1608.68,	0.01));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.001,	0.001,	-0.01));
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }

    ///////////////////////
    // add your own stuff
    ///////////////////////
	 if (arg.key == OIS::KC_7 ) {
		 mNumSpheres = 100;
		 for (int i=0; i<500; i++){
			 if (i>=400){
				 mSceneNode[i]->setVisible(false);
				vis[i] = false;
			 }
			 else{
				 mSceneNode[i]->setVisible(true);
				vis[i] = true;
			 }
		 }
    }
	 if (arg.key == OIS::KC_8 ) {
		 mNumSpheres = 200;
		 for (int i=0; i<500; i++){
			 if (i>=300){
				 mSceneNode[i]->setVisible(false);
				vis[i] = false;
			 }
			 else{
				 mSceneNode[i]->setVisible(true);
				vis[i] = true;
			 }
		 }
    }
	 if (arg.key == OIS::KC_9 ) {
		 mNumSpheres = 300;
		 for (int i=0; i<500; i++){
			 if (i>=200){
				 mSceneNode[i]->setVisible(false);
				vis[i] = false;
			 }
			 else{
				 mSceneNode[i]->setVisible(true);
				vis[i] = true;
			 }
		 }
    }
	 if (arg.key == OIS::KC_0 ) {
		 mNumSpheres = 500;
		 for (int i=0; i<500; i++){
				 mSceneNode[i]->setVisible(true);
				vis[i] = true;
		 }
    }

    if (arg.key == OIS::KC_B ) {
		posCreate();
		for (int i =0; i<500; i++){
			mSceneNode[i]->setPosition(posMem[i]);
			if (mNumSpheres == 500){
				mSceneNode[i]->setVisible(true);
				vis[i] = true;
			}
			//==================================
			else if (mNumSpheres == 300){
				if (i>=200){
				 mSceneNode[i]->setVisible(false);
				vis[i] = false;
			 }
			 else{
				 mSceneNode[i]->setVisible(true);
				vis[i] = true;
			 }
			}
			//==================================
			else if (mNumSpheres == 200){
				if (i>=300){
				 mSceneNode[i]->setVisible(false);
				vis[i] = false;
			 }
			 else{
				 mSceneNode[i]->setVisible(true);
				vis[i] = true;
			 }
			}
			//=====================================
			else if (mNumSpheres == 100){
				if (i>=400){
				 mSceneNode[i]->setVisible(false);
				vis[i] = false;
			 }
			 else{
				 mSceneNode[i]->setVisible(true);
				vis[i] = true;
			 }
			}
		}
    }

	if (arg.key == OIS::KC_U ) {
		mMoveDirection = mMoveDirection_UP;
    }
	if (arg.key == OIS::KC_J ) {
		mMoveDirection = mMoveDirection_DOWN;
    }
	if (arg.key == OIS::KC_H ) {
		mMoveDirection = mMoveDirection_LEFT;
    }
	if (arg.key == OIS::KC_K ) {
		mMoveDirection = mMoveDirection_RIGHT;
    }




    BaseApplication::keyPressed(arg);

    return flg;
}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
// 
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyReleased( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyReleased ***\n";

	mMoveDirection=0;
    
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    BaseApplication::keyReleased(arg);

    return flg;
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    //
    Vector3 mdir;
	
    if (mMoveDirection & mMoveDirection_UP ) {
		Vector3 currentPos = mLargeSphereSceneNode->getPosition();
       if (currentPos.z>-475) mdir += Vector3(0.0, 0.0, -1.5);
    }
	if (mMoveDirection & mMoveDirection_DOWN ) {
		Vector3 currentPos = mLargeSphereSceneNode->getPosition();
       if (currentPos.z<475) mdir += Vector3(0.0, 0.0, 1.5);
    }
	if (mMoveDirection & mMoveDirection_LEFT ) {
		Vector3 currentPos = mLargeSphereSceneNode->getPosition();
       if (currentPos.x>-475) mdir += Vector3(-1.5, 0.0, 0.0);
    }
	if (mMoveDirection & mMoveDirection_RIGHT ) {
		Vector3 currentPos = mLargeSphereSceneNode->getPosition();
       if (currentPos.x<475) mdir += Vector3(1.5, 0.0, 0.0);
    }
	curVec = mdir;
	mLargeSphereSceneNode->translate(mdir);
    ///////////////////////
    // add your own stuff
    ///////////////////////

    resolveCollision();
    //
    return flg;
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
