//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
/*!
\brief 3D Game Programming
\n
My Name: Dai Rong Wu
\n
My ID: 310551067
\n
My Email: azusa871227@gmail,com
\n Date: 2019/10/28

This is an assignment of 3D Game Programming

*/

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include "BaseApplication.h"

using namespace Ogre;

struct points{
	int gScore;
	int fScore;
	int hScore;
	int position[2];
};

struct relation{
	int parent[2];
	int self[2];
};

class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);
	virtual void createViewports(void);
	virtual void createScene(void);
	virtual void createCamera(void);
	virtual void chooseSceneManager(void);
    //
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    //
protected:
	/*!
	\brief Create a viewport

	Create a viewport for the entire screen.

	\return The sum of two integers.
	*/
	void createViewport_00(void);
	void createViewport_01(void);
	//
	void createCamera_00();
	void createCamera_01();

	void createScene_00();
	void createScene_01();
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
	//virtual bool mouseMoved( const OIS::MouseEvent &arg );
    void createSpace();

	void posCreate();
	bool aStar();
	void getCurPos();

protected:
    Ogre::Viewport* mViewportArr[8];
	Ogre::Camera* mCameraArr[8];
	Ogre::SceneManager* mSceneMgrArr[8];
	OgreBites::SdkCameraMan* mCameraManArr[8];

    //
    int mMoveDirection; // 1= right 2=left 3=up 4=down
	bool map[10][10];
	SceneNode *mapNode[10][10];
	Entity *mapEnt[10][10];

	int target[2];
	int currentPos[2];
	bool isPath;
	points finalPath[100];
	int pathLen;

	points openList[100];
	int openIdx;
	points closeList[100];
	int closeIdx;
	relation route[100];
	int routeIdx;

	SceneNode *botNode;
	Entity *botEnt;
	bool botState;
	int botPos[2];
	AnimationState *mAnimationState;


};

#endif // #ifndef __BasicTutorial_00_h_