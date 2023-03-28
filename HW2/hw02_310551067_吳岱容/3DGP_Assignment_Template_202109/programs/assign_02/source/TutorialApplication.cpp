////////////////////////////////////////
//
// 3D Game Programming
// NYCU
// Instructor: SAI-KEUNG WONG
//
// Date: 2021/10/06
////////////////////////////////////////
// Student Name:
// Student ID:
// Student Email:
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
//using namespace std;

const float PI = 3.141592654;
double mAngular = 0;

BasicTutorial_00::BasicTutorial_00(void) {
mCurrentObject = 0;
moveFlg = false;

}

void BasicTutorial_00::chooseSceneManager()
{
	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	mSceneMgrArr[1] = mRoot
		->createSceneManager(ST_GENERIC, "secondary");
	winFlg = true;
	btmGroundVis = true;
	particleFlg = true;
	for(int i = 0; i<36 ;i++)	sphereFlg[i]=false;
    //
    // add your own stuff
    //
}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(120,300,600));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	// add your own stuff
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("SmallCam");
	mCamera->setPosition(Ogre::Vector3(0.01,1400,0));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);
}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	Ogre::Viewport *vp = mWindow ->addViewport(mCamera);
	vp ->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mViewportArr[0] = vp;
	//vp->setOverlaysEnabled(false);
	mCamera ->setAspectRatio(Ogre::Real(vp->getActualWidth())/Ogre::Real(vp->getActualHeight()));
}

void BasicTutorial_00::createViewport_01(void)
{
    // add your own stuff
	
	mCamera = mCameraArr[1];
	Ogre::Viewport *vp = mWindow ->addViewport(mCamera,1,0,0,0.25,0.25);
	vp ->setBackgroundColour(Ogre::ColourValue(1,1,0));
	mViewportArr[1] = vp;
	vp->setOverlaysEnabled(false);
	vp->setSkiesEnabled(false);
	mCamera ->setAspectRatio(Ogre::Real(4*vp->getActualWidth())/Ogre::Real(vp->getActualHeight()));
}

void BasicTutorial_00::createScene_00(void) 
{
	mousePressedFlg = false;
	PlaneBoundedVolumeList volList;
	mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(volList);

	mSceneMgr = mSceneMgrArr[0];
	mSceneMgr ->setAmbientLight(ColourValue(0.5,0.5,0.5));
	mSceneMgr ->setFog(FOG_LINEAR, ColourValue(1,1,1), 0,1400,1600);
	mSceneMgr ->setSkyBox(true,"Examples/SpaceSkyBox");
	mSceneMgr ->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

	mSelectionRect = new SelectionRectangle("selectionRect");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mSelectionRect);
	mSelectionRect->setLightMask(0);
	mSelectionRect->setCastShadows(false);

	Light *light = mSceneMgr ->createLight("Light1");
	light->setType(Light::LT_POINT);
	light->setPosition(Vector3(0,350,250));
	light->setDiffuseColour(1,1,1);
	light->setSpecularColour(1,1,1);
	mLight = light;

	//Plane creation===================================
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		800,800, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 		// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	ent->setMaterialName("Examples/Rocky");
	ground = ent;
	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 

	Plane plane2(Vector3::UNIT_Y, -30); 
	MeshManager::getSingleton().createPlane(
		"ground2", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane2, 
		800,800, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 		// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent2 = mSceneMgr->createEntity(
		"GroundEntity2", "ground2");
	ent2->setMaterialName("Examples/Rockwall");
	ground2 = ent2;
	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent2); 
	//plane creation=====================================================

	//robots==============================================================
	for (int i =0;i<18 ;i++){
		String name;
		genNameUsingIndex("robot",i,name);
		robotEnt[i] = mSceneMgr ->createEntity(name,"robot.mesh");
		robotNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		robotNode[i] ->setPosition(300*sin((PI)/180*i*20), 0, 300*cos((PI)/180*i*20));
		robotNode[i] ->lookAt(Vector3(0,0,0),Node::TransformSpace::TS_WORLD);
		robotNode[i] ->yaw(Degree(90));
		robotNode[i] ->attachObject(robotEnt[i]);
		
	}

	robotNode[0] ->scale(2,2,2);

	for (int i =18;i<36 ;i++){
		String name;
		genNameUsingIndex("robot",i,name);
		robotEnt[i] = mSceneMgr ->createEntity(name,"robot.mesh");
		robotNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		robotNode[i] ->setPosition(200*sin((PI)/180*i*20), 0, 200*cos((PI)/180*i*20));
		robotNode[i] ->lookAt(Vector3(0,0,0),Node::TransformSpace::TS_WORLD);
		robotNode[i] ->yaw(Degree(90));
		robotNode[i] ->attachObject(robotEnt[i]);
		mAnimationState[i] = robotEnt[i]->getAnimationState("Walk");
		mAnimationState[i]->setLoop(true);
		mAnimationState[i]->setEnabled(true);
	}
	//robot============================================

	//Sphere==========================================
	sphereEnt = mSceneMgr->createEntity("sphereEnt","sphere.mesh");
	sphereNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("sphereNode",Vector3(0,0,0));
	sphereNode ->attachObject(sphereEnt);
	sphereNode ->scale(0.7,0.7,0.7);

	//particle
	initParticleSystemForExplosion();
	setOffParticleSystem();

    // add your own stuff

    /*
    Radian angle(3.141952654/2.0);

    Vector3 axis(1.0, 0.0, 0.0);
    mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
            Vector3(0.0, 0.0, -750.0),
            Quaternion( angle, axis))
		->attachObject(ent);
    */
}

void BasicTutorial_00::createScene_01(void) 
{
    // add your own stuff
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
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
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
		SceneNode *node = ground2->getParentSceneNode();
		Vector3 pos = node ->getPosition();
		node ->setPosition(Vector3(pos.x,pos.y-2,pos.z));
        //98.14	450.69	964.20
        //-0.01	-0.30	-0.95
    }

    if (arg.key == OIS::KC_2 ) {
		SceneNode *node = ground2->getParentSceneNode();
		Vector3 pos = node ->getPosition();
		node ->setPosition(Vector3(pos.x,pos.y+2,pos.z));
        // add your own stuff
        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
    }

    if (arg.key == OIS::KC_3 ) {
		if (btmGroundVis){
			btmGroundVis = false;
			SceneNode *node = ground2->getParentSceneNode();
			node->detachAllObjects();
		}
		else{
			btmGroundVis = true;
			SceneNode *node = mSceneMgrArr[0] ->getRootSceneNode()->createChildSceneNode();
			node->attachObject(ground2);
		}
        // add your own stuff
        //-1356.16	634.32	-964.51
        //0.71	-0.44	0.55
    }

    if (arg.key == OIS::KC_7 ) {
		ColourValue intensity = mSceneMgrArr[0]->getAmbientLight();
		mSceneMgrArr[0] ->setAmbientLight(ColourValue(intensity.r+0.01,intensity.g+0.01,intensity.b+0.01));

         // add your own stuff
        //40.39	155.23	251.20
        //-0.02	-0.41	-0.91
    }

    if (arg.key == OIS::KC_8 ) {
		ColourValue intensity = mSceneMgrArr[0]->getAmbientLight();
		mSceneMgrArr[0] ->setAmbientLight(ColourValue(intensity.r-0.01,intensity.g-0.01,intensity.b-0.01));
        // add your own stuff
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }
	if (arg.key == OIS::KC_9 ) {
		ColourValue intensityDif = mLight->getDiffuseColour();
		ColourValue intensitySpec = mLight->getSpecularColour();
		mLight ->setDiffuseColour(ColourValue(intensityDif.r+0.01,intensityDif.g+0.01,intensityDif.b+0.01));
        // add your own stuff
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }
	if (arg.key == OIS::KC_0 ) {
		ColourValue intensityDif = mLight->getDiffuseColour();
		ColourValue intensitySpec = mLight->getSpecularColour();
		mLight ->setDiffuseColour(ColourValue(intensityDif.r-0.01,intensityDif.g-0.01,intensityDif.b-0.01));
        // add your own stuff
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }
	if (arg.key == OIS::KC_N){
		if (particleFlg){
			particleFlg = false;
			setOffParticleSystem();
		}
		else{
			particleFlg = true;
			setOffParticleSystem();
		}
	}

    if (arg.key == OIS::KC_M ) {
        
       Camera *c_ptr = mCameraArr[0];
       mWindow->removeViewport(mViewportArr[0]->getZOrder());
	Ogre::Viewport* vp = mWindow->addViewport(
        c_ptr,
        0,
        0.15,
        0.55,
        0.45,
        0.3
        );
	vp->setBackgroundColour(Ogre::ColourValue(0,0.5,0.0));
	c_ptr->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
   //delete mViewportArr[0];    // program crashes
    mViewportArr[0] = vp;       // make sure to save the new pointer
    
    }

	if (arg.key == OIS::KC_SLASH ) {
        // add your own stuff
		if (winFlg == true){
			winFlg = false;
			mWindow->removeViewport(1);
		}
		else{
			winFlg = true;
			createViewport_01();
		}
    }

	if (arg.key == OIS::KC_F1){
		ground->setMaterialName("Examples/Rocky");
	}
	if(arg.key==OIS::KC_F2){
		ground->setMaterialName("Examples/Tens");
	}
	if(arg.key==OIS::KC_F3){
		ground->setMaterialName("Examples/WaterStream");
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
	mSelectionRect->updateDT(evt.timeSinceLastFrame);
	mSelectionRect->updateMesh();
    //
    // add your own stuff
    //

	//light rotation========================
	Vector3 lightPos = mLight ->getPosition();
	mAngular+=1*evt.timeSinceLastFrame;
	mLight ->setPosition(Vector3(250*cos(mAngular),lightPos.y,250*sin(mAngular)));
	//=======================================

    return flg;
}

bool BasicTutorial_00::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	if (id == OIS::MB_Left){
		mousePressedFlg = true;
		Ray mRay =mTrayMgr->getCursorRay(mCamera);

		Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
		left = scn.x;  // (left, top) = (scn.x, scn.y)
		top = scn.y;

		right = scn.x;
		bottom = scn.y;

		mSelectionRect->setCorners(left, top, right, bottom);
		for (int i =0 ;i<36 ;i++)	if(robotNode[i]->getShowBoundingBox())	robotNode[i]->showBoundingBox(false);
	}
	
	return BaseApplication::mousePressed( arg, id );
}

bool BasicTutorial_00::mouseMoved(const OIS::MouseEvent &arg){
	Ray mRay =mTrayMgr->getCursorRay(mCamera);

	if (mousePressedFlg){
		Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
		Real mleft = scn.x;
		Real mtop = scn.y;
		if ((mtop>top)&&(mleft<left)) oppFlg = false;
		else if ((mtop<top)&&(mleft>left)) oppFlg = false;
		else oppFlg = true;
		left = mleft;
		top = mtop;
		if (oppFlg)	mSelectionRect->setCorners(left, top, right, bottom);
		else mSelectionRect->setCorners(right, top, left, bottom);
			
		//mSelectionRect->setCorners(right, top, left, bottom);
		mSelectionRect->setVisible(true);
	}
	if ( arg.state.Z.rel != 0) {
        Vector3 p = mCamera->getPosition( );
        float dy = arg.state.Z.rel/10; // set d to a proper value
        p.y -= dy;
        float dz = arg.state.Z.rel/10;
        p.z -= dz;
        mCamera->setPosition( p );
	}


	return BaseApplication::mouseMoved( arg);
}

bool BasicTutorial_00::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	if (id == OIS::MB_Left){
		mousePressedFlg = false;
		moveFlg = false;
	
		Real nleft = left+0.00001;
		Real nright = right;
		Real ntop = 1+top+0.00001;
		Real nbottom = 1+bottom;

		Ray topLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, ntop));
		Ray topRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, ntop));
		Ray bottomLeft = mTrayMgr->screenToScene(mCamera, Vector2(nleft, nbottom));
		Ray bottomRight = mTrayMgr->screenToScene(mCamera, Vector2(nright, nbottom));
		//End Using mTrayMgr=============
		mSelectionRect->setVisible(false);


		// The plane faces the counter clockwise position.
		PlaneBoundedVolume vol;
		int np = 100;
		vol.planes.push_back(Plane(topLeft.getPoint(3), topRight.getPoint(3), bottomRight.getPoint(3)));         // front plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), topLeft.getPoint(np), topRight.getPoint(np)));         // top plane
		vol.planes.push_back(Plane(topLeft.getOrigin(), bottomLeft.getPoint(np), 	topLeft.getPoint(np)));       // left plane
		vol.planes.push_back(Plane(bottomLeft.getOrigin(), bottomRight.getPoint(np), 	bottomLeft.getPoint(np)));   // bottom plane
		vol.planes.push_back(Plane(bottomRight.getOrigin(), topRight.getPoint(np), 	bottomRight.getPoint(np)));     // right plane */

		PlaneBoundedVolumeList volList;
		volList.push_back(vol);
		SelectVolList = volList;
		mVolQuery->setVolumes(volList);
	
		SceneQueryResult result = mVolQuery->execute();

		SceneQueryResultMovableList::iterator itr = result.movables.begin();


		// Get the results, set the camera height
		// We are interested in the first intersection. It is ok to traverse all the results.
		for (itr = result.movables.begin(); itr != result.movables.end(); ++itr)
		{
			if (*itr)
			{
				mCurrentObject = (*itr)->getParentSceneNode();
				if (mCurrentObject != sphereNode){
					bool flgShow = mCurrentObject->getShowBoundingBox();
					mCurrentObject->showBoundingBox(!flgShow);
				}
			}
		}
	}
	else if (id == OIS::MB_Right){
		Ray mRay =mTrayMgr->getCursorRay(mCamera);
		Plane p(Vector3(0, 1, 0), 0);
		std::pair<bool,Real> result = mRay.intersects(p);
		des = mRay.getPoint(result.second);
		moveFlg = true;
		for (int i =0; i<36;i++){
			sphereFlg[i]= false;
			v[i] = des-robotNode[i]->getPosition();
			v[i].normalise();
			v[i].y = 0;
		}
	}
	return BaseApplication::mouseReleased( arg, id );
}

bool BasicTutorial_00::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (moveFlg) {
		for (int i =0 ;i<36 ;i++){
			if (robotNode[i]->getShowBoundingBox()){
				Vector3 pos = robotNode[i]->getPosition();
				if (pos.x - des.x <0.1&&pos.x - des.x >-0.1 && pos.z-des.z<0.1 && pos.z-des.z>-0.1){
					robotNode[i]->showBoundingBox(false);
				}
				Real speedFactor = 20;
				Real t = evt.timeSinceLastFrame;
				robotNode[i]->lookAt(des,Node::TransformSpace::TS_WORLD);
				robotNode[i]->yaw(Degree(90));
				if (sphereFlg[i]){
					sphereFlg[i] = false;
					v[i] = des-pos;
					v[i].normalise();
					v[i].y=0;
				}
				Vector3 velocity = v[i];
				for (int k =0;k<36 ;k++){
					Vector3 PosDect = robotNode[k]->getPosition();
					//Ogre::String ss = Ogre::StringConverter::toString( pos -PosDect );
					//Ogre::LogManager::getSingletonPtr()->logMessage( ss );
					Vector3 test = PosDect - pos;
					if ((pos.x-PosDect.x)*(pos.x-PosDect.x)+(pos.z-PosDect.z)*(pos.z-PosDect.z)<400 && i!=k &&
						(test.x*v[i].x)+(test.z*v[i].z)>0){
						velocity = Vector3(0,0,0);
						break;
					}
					
				}
				Vector3 spherePos = sphereNode->getPosition();
				Vector3 vec = spherePos-pos;
				if ((pos.x-spherePos.x)*(pos.x-spherePos.x)+(pos.z-spherePos.z)*(pos.z-spherePos.z)<8100 && 
					(vec.x*v[i].x)+(vec.z*v[i].z)>0){
					sphereFlg[i] = true;
					Ogre::Quaternion rotation(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);
					velocity = rotation*velocity;
				}

				robotNode[i]->translate(t*speedFactor*velocity);
				mAnimationState[i] = robotEnt[i]->getAnimationState("Walk");
				mAnimationState[i]->setLoop(true);
				mAnimationState[i]->setEnabled(true);
				mAnimationState[i]->addTime(0.2*8*evt.timeSinceLastFrame);
			}
		}
	}
	return BaseApplication::frameRenderingQueued(evt);
}

void BasicTutorial_00::initParticleSystemForExplosion() {
	//mParticleNode = static_cast<SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());

	for (int i=0 ;i<36 ;i++){
		String name;
		genNameUsingIndex("explosion",i,name);
		ParticleSystem* p = mSceneMgr->createParticleSystem(name, "Examples/GreenyNimbus");
		p->setDefaultDimensions(10,10);
		p->setBounds(robotEnt[i]->getBoundingBox());
		ParticleEmitter *e = p->getEmitter(0);
		e->setEnabled(false);
		particle[i] = robotNode[i]->createChildSceneNode();
		particle[i]->attachObject(p);
		particle[i]->setVisible(false);
	}
	
}

void BasicTutorial_00::setOffParticleSystem() 
{
	/*robotNode[0] ->setVisible(true);
		MovableObject *s = robotNode[0]->getAttachedObject("explosion");
		ParticleSystem* p =static_cast<ParticleSystem*>(s);
		if (p == NULL) return;
		p->setVisible(true);
		ParticleEmitter *e = p->getEmitter(0);
		e->setEnabled(true);*/
	for(int i =0 ;i<36 ;i++){
		String name;
		genNameUsingIndex("explosion",i,name);
		particle[i] ->setVisible(true);
		MovableObject *s = particle[i]->getAttachedObject(name);
		ParticleSystem* p =static_cast<ParticleSystem*>(s);
		if (p == NULL) return;
		p->setVisible(true);
		ParticleEmitter *e = p->getEmitter(0);
		if(particleFlg)e->setEnabled(true);
		else e->setEnabled(false);
	}
	/*	fNode->setPosition(pos);
	fNode->setVisible(true);
	MovableObject *s = fNode->getAttachedObject(pname);
	ParticleSystem* p =static_cast<ParticleSystem*>(s);
	if (p == NULL) return;
	p->setVisible(true);
	ParticleEmitter *e = p->getEmitter(0);
	e->setEnabled(true);*/
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}

////////////////////////////////////////
// DO NOT DELETE THIS LINE: 2021/10/06. 3D Game Programming
////////////////////////////////////////