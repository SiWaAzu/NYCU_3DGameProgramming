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
My Email: azusa871227@gmail.com
\n Date: 2019/09/20

This is an assignment of 3D Game Programming
*/

////////////////////////////////////////
// You can delete or add some functions to do the assignment.
////////////////////////////////////////

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include "BaseApplication.h"
/**
  *I didn't modify any thing in this headfile.
  *This class define the function we use and the members.
  */
class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);
    virtual void chooseSceneManager(void);
	/*!
	\brief Call funcitons to create 2 cameras
	*/
    virtual void createCamera(void);
	/*!
	\brief call funcitons to create 2 viewports
	*/
	virtual void createViewports(void);
	/*!
	\brief call functions to create 2 scenes
	*/
	virtual void createScene(void);
	
	
    //
	/*!
	\brief Implement animation
	\n
	Implement animation when the frame refresh if the animation flag is flipped.
	\n
	*/
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    //
    // Add your own stuff.
    //
protected:
	/*!
	\brief Create a viewport
	\n
	Create a viewport for the entire screen.
	\n
	*/
	void createViewport_00(void);
	/*!
	\brief Create sub viewport
	\n
	Create Second viewport, and locate it on the left top, also set the z order.
	\n
	Also we set background color.
	\n
	*/
	void createViewport_01(void);
	/*!
	\brief Create main camera
	\n
	Create main camera, set the direction, position, and clip distance, and also give it manipulation.
	*/
	void createCamera_00();
	/*!
	\brief Create sub camera
	\n
	Create the camera for second viewport, also set its direction, position, and clip distance.
	*/
	void createCamera_01();
	/*!
	\brief Create main scene
	\n
	Create main scene, including a plane, two penguins and cubes, enabling lights and shadow.
	\n
	Also set the entities's scales, positions, and directions.
	\n
	*/
	void createScene_00();
	/*!
	\brief Create second scene
	\n
	Create second scene, including a plane and a cube.
	\n
	*/
	void createScene_01();
	/*!
	\brief Set key pressed events
	\n
	Set required key pressing event.
	\n
	key 1~5 for different camera position.
	\n
	key M / N for differet viewports showing on the window.
	\n
	key P for triggering animation.
	\n
	*/
    bool keyPressed( const OIS::KeyEvent &arg );
	/*!
	\brief Set key released events
	\n
	Didn't modify any thing.
	\n
	*/
    bool keyReleased( const OIS::KeyEvent &arg );
    //

    // Add your own stuff.
    //
protected:
    Ogre::Viewport* mViewportArr[8];
	Ogre::Camera* mCameraArr[8];
	Ogre::SceneManager* mSceneMgrArr[8];
	OgreBites::SdkCameraMan* mCameraManArr[8];
	//bool animationFlag;
	
    //
    // Add your own stuff.
    //
};


#endif // #ifndef __BasicTutorial_00_h_