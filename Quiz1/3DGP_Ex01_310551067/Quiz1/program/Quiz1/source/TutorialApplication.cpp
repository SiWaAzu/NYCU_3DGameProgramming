//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
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
const float speedPitch = 0.05;

BasicTutorial_00::BasicTutorial_00(void)
    : 
mFlg_Root(false), mFlg_Root_Dir(true)

, mFlg_Arm(false), mFlg_Arm_Dir(true)
, mPitchAngle(0)      
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
	mCamera = mCameraArr[1];

	Ogre::Viewport* vp = mWindow->addViewport(
		mCamera,
		1,
		0.75,
		0.0,
		0.25,
		0.25
		);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setOverlaysEnabled(false);
    //
    mViewportArr[1] = vp;
}

void BasicTutorial_00::flagInitialize(){
	bool mFlg_Penguin_Jump = true;
    bool mFlg_Penguin_Dir = true;
    //
    bool mFlg_Root = false;
    bool mFlg_Root_Dir = false;
    bool mFlg_Arm = false;
    bool mFlg_Arm_Dir = false;
}


void BasicTutorial_00::createHierarchyStructure()
{
    String name_en;
    String name_sn;
	
	flagInitialize();

    int index = 0;

    genNameUsingIndex("R1", index, name_en);
    genNameUsingIndex("S1", index, name_sn);
    mEntity[index] = mSceneMgr
		->createEntity( name_en, "sphere.mesh" ); ;
    mSceneNode[index] = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		name_sn, Vector3( 0, 0, 0 ) ); 

    mSceneNode[index]->attachObject(mEntity[index]);
    //
    mSphere = mSceneNode[index];
    int root_index = index;
    //
    ++index;	//now index ==1 
	int colStartIdx = index;
    int num = 12;
    double angle = 30;
    for (int i =0; i < num; ++i, ++index) {
        genNameUsingIndex("R1", index, name_en);
        genNameUsingIndex("S1", index, name_sn);
        mEntity[index] = mSceneMgr
            ->createEntity( name_en, "column.mesh" ); ;
        mSceneNode[index] = mSceneNode[root_index]
        ->createChildSceneNode( 
            name_sn, Vector3( 0, 100, 0 ) ); 
        mSceneNode[index]->scale(0.25, 1.0, 0.25);
        mSceneNode[index]->translate(Vector3(0.0, 0.0, 0.0),
            Node::TS_PARENT);
        mSceneNode[index]->attachObject(mEntity[index]);
        mSceneNode[index]->pitch(Degree(30), Node::TS_PARENT);
        mSceneNode[index]->yaw(Degree(i*angle), Node::TS_PARENT);
    }

   // ++index;
    genNameUsingIndex("R1", index, name_en);
    genNameUsingIndex("S1", index, name_sn);
    mEntity[index] = mSceneMgr
		->createEntity( name_en, "cube.mesh" ); ;
    mSceneNode[index] = mSceneNode[root_index]
		->createChildSceneNode( 
		name_sn, Vector3( 0, 350, 0 ) ); 
    mSceneNode[index]->scale(1.3/0.35, 0.1, 1.3/0.35);
    mSceneNode[index]->attachObject(mEntity[index]);
    //
    mPlatform = mSceneNode[index];
    //
    ++index;

	//int colBlackIdx = index;
	mEntity[index] = mSceneMgr ->createEntity("colBlack","column.mesh");
	mEntity[index]->setMaterialName("Examples/BeachStones");
	mSceneNode[index] = mSceneNode[root_index] ->createChildSceneNode("colBlackNode",Vector3(0,355,0));
	mSceneNode[index] ->scale(1,0.1,1);
	mSceneNode[index] ->attachObject(mEntity[index]);
	mColumn = mSceneNode[index];
    
	index++;
	mEntity[index] = mSceneMgr ->createEntity("penguin","penguin.mesh");
	mSceneNode[index] = mSceneNode[index-1] ->createChildSceneNode("penguinNode",Vector3(0,600,0));
	mSceneNode[index] ->scale(1,10,1);
	mSceneNode[index] ->attachObject(mEntity[index]);
	mPenguin = mSceneNode[index];
    //mPenguin = mSceneNode[index];
}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];
	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	Light *light;
	light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(150, 250, 100)); 
	light->setDiffuseColour(0.0, 1.0, 0.0);		
	light->setSpecularColour(0.0, 1.0, 0.0);	

	light = mSceneMgr->createLight("Light2"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(-150, 300, 250)); 
	light->setDiffuseColour(0.5, 0.5, 0.5);		
	light->setSpecularColour(0.5, 0.5, 0.5);	

    light = mSceneMgr->createLight("Light3"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(-150, 400, 250)); 
	light->setDiffuseColour(0.5, 0.5, 0.5);		
	light->setSpecularColour(0.0, 0.0, 0.5);	

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

    createHierarchyStructure();
}

void BasicTutorial_00::createScene_01(void) 
{
	mSceneMgr = mSceneMgrArr[1];
	//mSceneMgr->setAmbientLight( ColourValue( 1.0,1.0, 1.0 ) ); 
	mSceneMgr->setAmbientLight( ColourValue( 0.0,0.0, 0.0 ) );  
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE); 
//mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);

	Light *light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(100, 150, 250)); 
	light->setDiffuseColour(0.0, 0.0, 1.0);		
	light->setSpecularColour(0.0, 0.0, 1.0);	

	Entity *ent3 
		= mSceneMgr
		->createEntity( "Robot3", "cube.mesh" ); 
	//ent2->setCastShadows(true);
	SceneNode *node3 
		= mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode( 
		"RobotNode3", Vector3( 50, 0, 0 ) ); 
	node3->attachObject( ent3 );
	node3->translate(0, 30, 0);
	node3->scale(0.4, 0.2, 0.2);
	ent3->setMaterialName("Examples/green");
	//
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 		// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	//ent->setMaterialName("Examples/RustySteel");
	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 


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
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

    }

    if (arg.key == OIS::KC_2 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(-824.52,	468.58,	68.45));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.94,	-0.31,	-0.11));

        //-824.52,	468.58,	68.45
        //0.94,	-0.31,	-0.11
    }


    if (arg.key == OIS::KC_3 ) {
		mCameraMan->getCamera()
            ->setPosition(Vector3(-19.94,	822.63,	30.79));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.00,	-0.99,	-0.11));
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }

	if (arg.key == OIS::KC_7){
		if (mFlg_Arm & !(mFlg_Arm_Dir))	mFlg_Arm = false;
		else mFlg_Arm = true;
		mFlg_Arm_Dir = false;
	}

	if (arg.key == OIS::KC_8){
		if (mFlg_Arm & mFlg_Arm_Dir)	mFlg_Arm = false;
		else mFlg_Arm = true;
		mFlg_Arm_Dir = true;
	}
	if (arg.key == OIS::KC_9){
		if (mFlg_Root & !(mFlg_Root_Dir))	mFlg_Root = false;
		else mFlg_Root = true;
		mFlg_Root_Dir = false;
	}
	if(arg.key == OIS::KC_0){
		if (mFlg_Root & mFlg_Root_Dir)	mFlg_Root = false;
		else mFlg_Root = true;
		mFlg_Root_Dir = true;
	}

    //
    // Add your own stuff
    //
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

void BasicTutorial_00::HandleKeyEventAndUpdateObjects(const Ogre::FrameEvent& evt)
{
    //
    // Add your own stuff
    //
    /*
    float a = 20.0*evt.timeSinceLastFrame;
    if ( mFlg_Arm_Dir ) {
        mPitchAngle += a;
        if ( mPitchAngle > 30) {
             mPitchAngle -= a;
             mFlg_Arm_Dir = !mFlg_Arm_Dir;
             a = -a;
        }
        mColumn->pitch(Degree(a));
    ......
    */
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    //
    HandleKeyEventAndUpdateObjects(evt);

	//floatingPenguin
	Vector3 PenguinPos = mPenguin ->getPosition();
	if (PenguinPos.y<=600)	mFlg_Penguin_Jump = true;
	else if (PenguinPos.y>=650) mFlg_Penguin_Jump = false;
	if (mFlg_Penguin_Jump)	mPenguin ->setPosition(PenguinPos.x, PenguinPos.y+0.1, PenguinPos.z); 
	else	mPenguin ->setPosition(PenguinPos.x, PenguinPos.y-0.1, PenguinPos.z); 
	//floatingPenguin

	//pitch
	if (mFlg_Arm){
		Quaternion quat = mColumn ->getOrientation();
		Radian currentRad = quat.getPitch();
		if (currentRad <=Degree(-30)){
			mFlg_Arm_Dir = true;
		}
		else if (currentRad >=Degree(30) ){
			mFlg_Arm_Dir = false;
		}
		if (mFlg_Arm_Dir){
			mColumn -> pitch(Degree(speedPitch));
		}
		if (!mFlg_Arm_Dir){
			mColumn->pitch(Degree(-speedPitch));
		}
	}
	

	//rotation
	if (mFlg_Root){
		if (mFlg_Root_Dir == false){
			mSphere ->yaw(Degree(0.05));
		}
		else if (mFlg_Root_Dir == true){
			mSphere ->yaw(Degree(-0.05));
		}
	}

	//mColumn ->pitch(Degree(speedPitch));
    //
    return flg;
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
