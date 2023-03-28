////////////////////////////////////////
//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2019/09/20
////////////////////////////////////////
// Student Name:Dai Rong Wu
// Student ID:310551067
// Student Email:azusa871227@gmail.com
//
////////////////////////////////////////
// You can delete or add some functions to do the assignment.
////////////////////////////////////////

#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

const float PI = 3.141592654;
bool aniFlag = false;
bool touchFlag = false;
double R = 200;
double mAngularAcc = 0.05;
double mAngularSpeed = 0;
double maxSpeed = 5;
double mAngular = 0;

BasicTutorial_00::BasicTutorial_00(void) {}

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
	mCamera->lookAt(Ogre::Vector3(120,0,0)); //ori:0,0,0
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	// add your own stuff
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr ->createCamera("secondaryCam");
	mCamera ->setPosition(Ogre::Vector3(0,350,0.001));
	mCamera ->lookAt(Ogre::Vector3(0,0,0));
	mCamera ->setNearClipDistance(5);
}


void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	 // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	mViewportArr[0] = vp;
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void BasicTutorial_00::createViewport_01(void)
{
    // add your own stuff
	mCamera =mCameraArr[1];
	Ogre::Viewport* vp = mWindow ->addViewport(mCamera,1,0.75,0,0.25,0.25);
	vp ->setBackgroundColour(Ogre::ColourValue(0,0,1));
	mViewportArr[1] = vp;
	mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setOverlaysEnabled(false);
}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];
	mSceneMgr -> setAmbientLight(ColourValue(0,0,0));
	mSceneMgr -> setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	//enable shadow and ambient light
	Plane plane (Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane(
		"ground", 										
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
	);
	// create Plane object
	mSceneMgr -> getRootSceneNode() ->createChildSceneNode("groundSceneNode",Vector3(0,0,0)) ->attachObject(mSceneMgr->createEntity("GroundEntity","ground"));
	//attach ground to sceneNode (groundSceneNode)
	SceneNode *bigPenguinNode = mSceneMgr ->getRootSceneNode() ->createChildSceneNode("bigPenguinNode",Vector3(0,50,0));
	Entity *bigPenguin= mSceneMgr-> createEntity("bigPenguin","penguin.mesh");
	bigPenguinNode ->attachObject(bigPenguin);
	bigPenguinNode ->scale(2,3,2);
	bigPenguin ->setCastShadows(true);

	//create big penguin
	SceneNode *smallPenguinNode =  mSceneMgr ->getRootSceneNode() ->createChildSceneNode("smallPenguinNode",Vector3(200,50,0));
	Entity *smallPenguin = mSceneMgr ->createEntity("smallPenguin","penguin.mesh");
	smallPenguinNode ->attachObject(smallPenguin);
	smallPenguinNode ->lookAt(bigPenguinNode->getPosition(),Node::TransformSpace::TS_WORLD);
	smallPenguinNode ->yaw(Degree(-180));
	smallPenguinNode ->scale(1.5,2,1.5);
	smallPenguin ->setCastShadows(true);

	int cubeNum = 72;
	int L = 255;
	for (int i=0; i<cubeNum; i++){
		String name;
		genNameUsingIndex("c",i,name);
		Entity *cubes = mSceneMgr ->createEntity(name,"cube.mesh");
		cubes ->setMaterialName("Examples/SphereMappedRustySteel");
		cubes ->setCastShadows(true);
		AxisAlignedBox bb = cubes ->getBoundingBox();
		int cubeSize = bb.getMaximum().x - bb.getMinimum().x;
		SceneNode *sNode = mSceneMgr ->getRootSceneNode() ->createChildSceneNode();
		sNode ->attachObject(cubes);
		double fx = i/(double)(cubeNum-1);
		double h = (1+sin(fx*PI*4))*50;
		double circleRads=100;
		double x1 = circleRads*cos(fx*PI*2);
		double z1 = circleRads*sin(fx*PI*2);
		double unitF = 1.0/cubeSize/cubeNum*L*0.8;
		sNode ->scale(unitF, h/cubeSize, unitF);
		sNode ->setPosition(x1,50,z1);
	}
	//circle cubes
	for (int i=0; i<cubeNum; i++){
		String name;
		genNameUsingIndex("b",i,name);
		
		Entity *cubes = mSceneMgr ->createEntity(name,"cube.mesh");
		cubes ->setMaterialName("Examples/Chrome");
		cubes ->setCastShadows(true);
		AxisAlignedBox bb = cubes ->getBoundingBox();
		int cubeSize = bb.getMaximum().x - bb.getMinimum().x;
		SceneNode *sNode = mSceneMgr ->getRootSceneNode() ->createChildSceneNode();
		sNode ->attachObject(cubes);
		double fx = 2*i/(double)(cubeNum-1);
		double x = fx*L -L/2.0;
		double h = (1+cos(fx*3.1415*2.0))*20;
		double unitF = 1.0/cubeSize/cubeNum*L*0.8;
		sNode ->scale(unitF, h/cubeSize, unitF);
		sNode ->setPosition(x,20,125);
	}
	//row cubes
	Light *L1 = mSceneMgr ->createLight("L1");
	L1 ->setType(Light::LT_POINT);
	L1 ->setPosition(Vector3(150,250,100));
	L1 ->setDiffuseColour(1.0,1.0,1.0);
	L1 ->setSpecularColour(1.0,1.0,1.0);

	Light *L2 = mSceneMgr ->createLight("L2");
	L2 ->setType(Light::LT_POINT);
	L2 ->setPosition(Vector3(-150,300,100));
	L2 ->setDiffuseColour(0,1.0,0);
	L2 ->setSpecularColour(0,1.0,0);

    // add your own stuff

}

void BasicTutorial_00::createScene_01(void) 
{
    // add your own stuff
	
	mSceneMgr = mSceneMgrArr[1];
	mSceneMgr -> setAmbientLight(ColourValue(0,0,0));
	mSceneMgr -> setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	//ambient color & shadow
	Plane plane (Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane(
		"ground2", 										
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
	);
	mSceneMgr -> getRootSceneNode() ->createChildSceneNode("groundSceneNode2",Vector3(0,0,0)) ->attachObject(mSceneMgr->createEntity("GroundEntity2","ground2"));
	//plane creation
	Light *subL =mSceneMgr ->createLight("subL");
	subL ->setType(Light::LT_POINT);
	subL ->setPosition(Vector3(100,150,250));
	subL ->setSpecularColour(0,0,1);
	subL ->setDiffuseColour(0,0,1);
	//Light setting
	Entity *ent = mSceneMgr ->createEntity("subEnt","cube.mesh");
	ent ->setMaterialName("Examples/green");
	SceneNode *sNode = mSceneMgr ->getRootSceneNode() ->createChildSceneNode("subEntNode",Vector3(50,50,0));
	sNode ->attachObject(ent);
	sNode ->scale(0.2,0.25,0.15);
	ent ->setCastShadows(true);
	sNode ->yaw(Degree(-45));
	sNode ->pitch(Degree(-30));
	//cube creation in sub viewport

}

void BasicTutorial_00::createViewports(void)
{
    //Do not modify
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
    //Do not modify
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
    //Do not modify
	createScene_00();
	createScene_01();

	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    mCamera = mCameraArr[0];

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
            ->setPosition(Vector3(98.14,	450.69,	964.20));
        mCameraMan->getCamera()
            ->setDirection(Vector3(-0.01,	-0.30,	-0.95));

        //98.14	450.69	964.20
        //-0.01	-0.30	-0.95
    }

    if (arg.key == OIS::KC_2 ) {
        // add your own stuff
        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
		mCameraMan ->getCamera() ->setPosition(Vector3(-1463.00, 606.45, -513.24));
		mCameraMan->getCamera() ->setDirection(Vector3(0.88, -0.47, 0.10));
    }

    if (arg.key == OIS::KC_3 ) {
        // add your own stuff
        //-1356.16	634.32	-964.51
        //0.71	-0.44	0.55
		mCameraMan ->getCamera() ->setPosition(Vector3(-1356.16, 634.32, -964.51));
		mCameraMan->getCamera() ->setDirection(Vector3(0.71, -0.44, 0.55));
    }

    if (arg.key == OIS::KC_4 ) {
         // add your own stuff
        //40.39	155.23	251.20
        //-0.02	-0.41	-0.91
		mCameraMan ->getCamera() ->setPosition(Vector3(40.39, 155.23, 251.20));
		mCameraMan->getCamera() ->setDirection(Vector3(-0.02, -0.41, -0.91));
    }

    if (arg.key == OIS::KC_5 ) {
        // add your own stuff
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
		mCameraMan ->getCamera() ->setPosition(Vector3(19.94, 822.63, 30.79));
		mCameraMan->getCamera() ->setDirection(Vector3(0.00, -0.99, -0.11));
    }

    if (arg.key == OIS::KC_M ) {
        
       Camera *c_ptr = mCameraArr[0];
	   Camera *c_ptr_2 = mCameraArr[1];
       mWindow->removeViewport(mViewportArr[0]->getZOrder());
	   mWindow ->removeViewport(mViewportArr[1]->getZOrder());
	Ogre::Viewport* vp = mWindow->addViewport(
        c_ptr,
        1,
        0,
        0,
        0.5,
        0.25
        );
	Ogre::Viewport* vp2 = mWindow->addViewport(c_ptr_2);
	vp->setBackgroundColour(Ogre::ColourValue(0,0.5,0.0));
	c_ptr->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	c_ptr_2->setAspectRatio(
		Ogre::Real(vp2->getActualWidth())/Ogre::Real(vp2->getActualHeight()));
   //delete mViewportArr[0];    // program crashes
    mViewportArr[0] = vp;       // make sure to save the new pointer
	mViewportArr[1] = vp2;
	vp2 ->setOverlaysEnabled(false);
    }

    if (arg.key == OIS::KC_N ) {
        // add your own stuff
		Camera *c_ptr = mCameraArr[0];
	    Camera *c_ptr_2 = mCameraArr[1];
        mWindow->removeViewport(mViewportArr[0]->getZOrder());
	    mWindow ->removeViewport(mViewportArr[1]->getZOrder());
		Ogre::Viewport* vp = mWindow->addViewport(c_ptr);
		Ogre::Viewport* vp2 = mWindow->addViewport(
			c_ptr_2,
			1,
			0,
			0.35,
			0.5,
			0.3
			);
		vp ->setBackgroundColour(Ogre::ColourValue(0,0.5,0));
		c_ptr->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
		c_ptr_2->setAspectRatio(
		Ogre::Real(vp2->getActualWidth())/Ogre::Real(vp2->getActualHeight()));
		mViewportArr[0] = vp;       // make sure to save the new pointer
		mViewportArr[1] = vp2;
		vp2 ->setOverlaysEnabled(false);
    }

	if (arg.key == OIS::KC_P){
		if (aniFlag == false) aniFlag = true;
		else aniFlag = false;
	}

    // Do not delete this line
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
    
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    BaseApplication::keyReleased(arg);

    return flg;
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    //
    // add your own stuff
    //
	Vector3 orin = Vector3(200,50,0);
	mSceneMgr = mSceneMgrArr[0];
	SceneNode* bigPenguinNode = mSceneMgr->getSceneNode("bigPenguinNode");
	SceneNode* smallPenguinNode = mSceneMgr->getSceneNode("smallPenguinNode");
	Vector3 smallPos = smallPenguinNode->getPosition();
	if (aniFlag == true){
		bigPenguinNode ->lookAt(smallPos,Node::TransformSpace::TS_WORLD);
		bigPenguinNode ->yaw(Degree(-180));
		smallPenguinNode ->lookAt(Vector3(0,50,0),Node::TransformSpace::TS_WORLD);
		smallPenguinNode ->yaw(Degree(180));
		mAngularSpeed+=mAngularAcc*evt.timeSinceLastFrame;
		if (mAngularSpeed >=5 )	mAngularSpeed = 5;
		else if (mAngularSpeed<=(-5)) mAngularSpeed = -5;
		mAngular +=mAngularSpeed*evt.timeSinceLastFrame;
		double x = R*cos(mAngular);
		double z = R*sin(mAngular);
		smallPos.x = x;
		smallPos.y = smallPos.y;
		smallPos.z = z;
		smallPenguinNode->setPosition(smallPos);

		if (x>=199 && x<200 && z>=(-1) && z<0 && touchFlag == false){
			touchFlag = true;
			mAngularAcc = mAngularAcc*(-1);
			mAngularSpeed = 0;
		}
		else if (x>=200 || x<199 || z<(-1) ||z>=0)	touchFlag = false; 
	}
	else if (aniFlag == false){
		//stop
		mAngularSpeed = 0;
	}
	//animation block
    return flg;
}
int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}

////////////////////////////////////////
// DO NOT DELETE THIS LINE: 2018/09/20. 3D Game Programming
////////////////////////////////////////