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
	mCamera->setPosition(Ogre::Vector3(0.01,1200,0));
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
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0.0,1.0));
	//vp->setOverlaysEnabled(false);
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[0] = vp;
}
void BasicTutorial_00::createViewport_01(void)
{
	mCamera = mCameraArr[1];
	Ogre::Viewport *vp = mWindow ->addViewport(mCamera,1,0,0,1,1);
	vp ->setBackgroundColour(Ogre::ColourValue(1,1,0));
	mViewportArr[1] = vp;
	//vp->setOverlaysEnabled(false);
	//vp->setSkiesEnabled(false);
	mCamera ->setAspectRatio(Ogre::Real(vp->getActualWidth())/Ogre::Real(vp->getActualHeight()));
}


void BasicTutorial_00::posCreate(){
	for (int i = 0; i<10 ;i++){
		for (int j = 0; j<10 ;j++){
			map[i][j]=true;
		}
	}
	for (int i =0; i<10 ;i++){
		map[i][0]=false;
		map[i][9]=false;
		map[0][i]=false;
		map[9][i]=false;
	}
	map[0][4] = true;
	map[1][4]=false;
	map[9][5]=true;
	map[8][5]=false;
	for (int i=2; i<8;i++){
		map[i][2]=false;
		map[i][7]=false;
	}
	map[4][4]=false;
	map[5][5]=false;
}
// create all spheres
// "Examples/red"
// "Examples/green"
// "Examples/blue"
// "Examples/yellow"

void BasicTutorial_00::createScene_00(void) 
{

	mSceneMgr = mSceneMgrArr[0];
	//mSceneMgr->setAmbientLight( ColourValue( 0.25, 0.25, 0.25 ) ); 

	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	posCreate();

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

	botEnt = mSceneMgr->createEntity("robot","robot.mesh");
	botNode = mSceneMgr ->getRootSceneNode()->createChildSceneNode("robot",Vector3(-600+60+120*3,0,-600+60+120*4));
	botNode->attachObject(botEnt);
	botState = false;

	String name_en;
	String name_sn;
	int index=0;

	for (int i =0; i<10; i++){
		for (int j=0 ;j<10; j++){
			genNameUsingIndex("BR",index, name_en);
			genNameUsingIndex("SR",index, name_sn);
			index++;
			if (map[i][j]){
				mapEnt[i][j] = mSceneMgr->createEntity(name_en,"sphere.mesh");
				mapEnt[i][j]->setMaterialName("Examples/blue");
				float posX = -600+120*i+60;
				float posZ = -600+120*j+60;
				mapNode[i][j] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn,Vector3(posX,0,posZ));
				mapNode[i][j]->attachObject(mapEnt[i][j]);
				mapNode[i][j]->scale(0.2,0.2,0.2);
				mapNode[i][j]->setVisible(false);
			}
			else{
				mapEnt[i][j] = mSceneMgr->createEntity(name_en,"barrel.mesh");
				float posX = -600+120*i+60;
				float posZ = -600+120*j+60;
				mapNode[i][j] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn,Vector3(posX,0,posZ));
				mapNode[i][j]->attachObject(mapEnt[i][j]);
				mapNode[i][j]->scale(10,10,10);
			}
		}
	}

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
	mCameraMan = mCameraManArr[1];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[1];
    //mCamera = mCameraArr[1];
    //
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

void BasicTutorial_00::getCurPos(){
	Vector3 pos = botNode->getPosition();
	currentPos[0]=int((pos.x+600)/120);
	currentPos[1]=int((pos.z+600)/120);
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
	if (botState){
		mAnimationState = botEnt->getAnimationState("Walk");
		mAnimationState->setLoop(true);
		mAnimationState->setEnabled(true);
		mAnimationState->addTime(evt.timeSinceLastFrame * 1.5f);
	}
	else{
		mAnimationState = botEnt->getAnimationState("Idle");
		mAnimationState->setLoop(true);
		mAnimationState->setEnabled(true);
		mAnimationState->addTime(evt.timeSinceLastFrame * 1.5f);
	}

	if (botState == true && pathLen>0){

		int goal[2];
		goal[0] = finalPath[0].position[0];
		goal[1] = finalPath[0].position[1];
	//	std::cout <<"("<<goal[0]<<","<<goal[1]<<")\n";
		float coordinateX = -600+120*goal[0]+60;
		float coordinateZ = -600+120*goal[1]+60;
		int dx = coordinateX-botNode->getPosition().x;
		int dz = coordinateZ-botNode->getPosition().z;
		float dt = evt.timeSinceLastFrame;
		Vector3 dir = Vector3(dx,0,dz);
		dir.normalise();
		botNode->translate(dir*120*dt);
		float disX = coordinateX - botNode->getPosition().x;
		float disZ = coordinateZ - botNode->getPosition().z;
		float dis = sqrt(disX*disX + disZ*disZ);
		botNode->lookAt(Vector3(coordinateX,0,coordinateZ),Node::TS_WORLD, Vector3::UNIT_X);
		if (dis<=1){
			//std::cout <<"("<<goal[0]<<","<<goal[1]<<")\n";
			botNode->setPosition(Vector3(coordinateX,0,coordinateZ));
			mapNode[goal[0]][goal[1]] ->setVisible(false);
			for (int i =0; i<pathLen; i++){
				if (i+1<pathLen)
					finalPath[i] = finalPath[i+1];
				else finalPath[i] = points();
			}
			pathLen--;
			if (pathLen<=0)	botState = false;
		}
	}

    //
   
    ///////////////////////
    // add your own stuff
    ///////////////////////

    //
    return flg;
}

bool BasicTutorial_00::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){

	if (id == OIS::MB_Left){
		for(int i = 0; i<10; i++){
			for(int j = 0; j<10; j++){
				if(map[i][j]) mapNode[i][j]->setVisible(false);
			
			}
		}

		isPath = false;

		Ray mRay =mTrayMgr->getCursorRay(mCamera);
		Plane p(Vector3(0, 1, 0), 0);
		std::pair<bool,Real> result = mRay.intersects(p);
		Vector3 des = mRay.getPoint(result.second);
		int PosX = (des.x+600)/120;
		int PosZ = (des.z+600)/120;
		if (map[PosX][PosZ]){	//if it is not wall;
			//botState = true;
			mapNode[PosX][PosZ]->setVisible(true);
			target[0]=PosX;
			target[1]=PosZ;
			getCurPos();
			isPath = aStar();
			std::cout<<isPath;

			int pathWalk[2];
			pathWalk[0] = target[0];
			pathWalk[1]=target[1];
			points tmpPath[100];
			pathLen = 0;
			tmpPath[pathLen].position[0] = target[0];
			tmpPath[pathLen].position[1] = target[1];
			pathLen++;
			while(!(pathWalk[0] == currentPos[0] && pathWalk[1] == currentPos[1])){
				for (int i=0 ;i<routeIdx; i++){
				//	std :: cout<<"pair: ("<< route[i].parent[0] << "," <<route[i].parent[1] <<")\n";
					if (route[i].self[0] == pathWalk[0] && route[i].self[1] == pathWalk[1]){
						pathWalk[0] = route[i].parent[0];
						pathWalk[1] = route[i].parent[1];
						if (!(pathWalk[0] == currentPos[0] && pathWalk[1] == currentPos[1])&& !(pathWalk[0] == target[0] && pathWalk[1] == target[1])){
							tmpPath[pathLen].position[0] = pathWalk[0];
							tmpPath[pathLen].position[1] = pathWalk[1];
							pathLen++;
						}
						break;
					}
				}
			}
			int j =0;
		//	if (botState)	j=1;
			for (int i = pathLen-1 ;i>=0 ;i--){
				finalPath[j] = tmpPath[i];
				j++;
			}
			//if (botState) pathLen++;
			for (int i=0 ;i<pathLen ;i++){
				std :: cout<<"\npair: ("<< finalPath[i].position[0] << "," <<finalPath[i].position[1] <<")"<<i<<"\n";
				mapNode[finalPath[i].position[0]][finalPath[i].position[1]]->setVisible(true);
			}
			botState = true;

		}
		
	}


	return BaseApplication::mousePressed( arg, id );
}

bool BasicTutorial_00::aStar(){
	openIdx = 0;
	closeIdx = 0;
	routeIdx = 0;
	openList[openIdx].position[0]=currentPos[0];
	openList[openIdx].position[1]=currentPos[1];

	openList[openIdx].gScore=0;
	openList[openIdx].hScore = abs(target[0]-openList[openIdx].position[0])+abs(target[1]-openList[openIdx].position[1]);
	openList[openIdx].fScore = openList[openIdx].gScore+openList[openIdx].hScore;
	openIdx++;
	////add start point into open
	while (openIdx>0){
		//find smallest f score
		int lowestScore = 100000;
		int lowestIdx;
		for (int i=0; i<openIdx; i++){
			if(openList[i].fScore<lowestScore){
				lowestIdx = i;
				lowestScore = openList[i].fScore;
			}
		}
		//return path
		points cur = openList[lowestIdx];
		if (cur.position[0] == target [0] && cur.position[1]==target[1]){
			std::cout<<"reach goal!";
			return true;
		}
		//add current to close
		closeList[closeIdx]=cur;
		closeIdx++;
		for (int i = lowestIdx; i<openIdx; i++){
			if (i+1<openIdx)	openList[i]=openList[i+1];
			else openList[i] = points();
		}
		openIdx--;
		//test neighbor
		for (int i =0; i<4; i++){
			int neighbor[2];
			switch(i){
			case 0:
				neighbor[0] = cur.position[0]+1;
				neighbor[1] = cur.position[1];
				break;
			case 1:
				neighbor[0] = cur.position[0]-1;
				neighbor[1] = cur.position[1];
				break;
			case 2:
				neighbor[0] = cur.position[0];
				neighbor[1] = cur.position[1]+1;
				break;
			case 3:
				neighbor[0] = cur.position[0];
				neighbor[1] = cur.position[1]-1;
				break;
			}

			if (neighbor[0]<0 || neighbor[0]>9 || neighbor[1]<0 || neighbor[1]>9 || map[neighbor[0]][neighbor[1]]==false){
				continue;
			}

			bool inClose = false;
			for (int j=0 ;j<closeIdx ;j++){
				if (closeList[j].position[0] == neighbor[0] && closeList[j].position[1] == neighbor[1]){
					inClose = true;
					break;
				}
			}
			if (inClose)	continue;

			bool inOpen = false;
			bool tmpBetter = false;
			int tmpG = cur.fScore+1;
			int realG = abs(currentPos[0] - cur.position[0])+abs(currentPos[1] - cur.position[1]);
			for (int j=0 ;j<openIdx ;j++){
				if (neighbor[0]==openList[j].position[0] && neighbor[1]==openList[j].position[1]){
					inOpen = true;
					break;
				}
			}
			if(!inOpen){
				tmpBetter = true;
			}
			else if (tmpG<realG){
				tmpBetter = true;
			}
			if (tmpBetter){
				route[routeIdx].parent[0] = cur.position[0];
				route[routeIdx].parent[1] = cur.position[1];
				route[routeIdx].self[0] = neighbor[0];
				route[routeIdx].self[1] = neighbor[1];
				routeIdx++;

				openList[openIdx].position[0] = neighbor[0];
				openList[openIdx].position[1] = neighbor[1];
				openList[openIdx].gScore = tmpG;
				openList[openIdx].hScore = abs(target[0]-neighbor[0])+abs(target[1]-neighbor[1]);
				openList[openIdx].fScore=openList[openIdx].gScore+openList[openIdx].hScore;
				openIdx++;
			}
		}
	}
	return false;

}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
