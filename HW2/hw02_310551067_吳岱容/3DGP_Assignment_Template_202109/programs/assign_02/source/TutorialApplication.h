//
// 3D Game Programming
// NYCU
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
\n Date: 2021/10/31
\n
This is an assignment of 3D Game Programming.
\n
Application class.
*/

////////////////////////////////////////
// You can delete or add some functions to do the assignment.
////////////////////////////////////////

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include "BaseApplication.h"
#include "selection_rectangle.h"

class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);
	/*!
	\brief Define SceneMgrs.
	*/
    virtual void chooseSceneManager(void);
	/*!
	\brief Call funcitons to create cameras.
	*/
    virtual void createCamera(void);
	/*!
	\brief Call funcitons to create viewports.
	*/
	virtual void createViewports(void);
	/*!
	\brief Call functions to create scenes.
	*/
	virtual void createScene(void);
	/*!
	\brief Deal with the task when mouse is moved.
	*/
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	/*!
	\brief Deal with the task when mouse is pressed.
	*/
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	/*!
	\brief Deal with the task when mouse is moved.
	*/
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	/*!
	\brief Deal with the movement of the robots and the animation.
	\n
	It is a Queue store frame need to be rendered.
	*/
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	
	
    /*!
	\brief Update animation.
	\n Update the selection rectangle and light rotation.
	*/
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    //
    // Add your own stuff.
    //
protected:
	/*!
	\brief Create a viewport
	Create a viewport for the entire screen.
	*/
	void createViewport_00(void);
	/*!
	\brief Create a viewport
	Create a viewport for the smallmap.
	*/
	void createViewport_01(void);
	/*!
	\brief Create the main camera
	\n
	Create the main camera, set the direction, position, and clip distance, and also give it manipulation.
	*/
	void createCamera_00();
	/*!
	\brief Create the small camera
	\n
	Create the camera for the small map, set the direction, position, clip distance, and ratio.
	*/
	void createCamera_01();
	/*!
	\brief Create the main scene
	\n
	Create the main scene including all the entity required and attach them to the scene node.
	\n
	Also set the entities's scales, positions, and directions.
	\n
	*/
	void createScene_00();
	/*!
	\brief Do nothing.
	*/
	void createScene_01();
	/*!
	\brief Deal with the key pressed events.
	\n Perform the task as the spec requires.
	*/
    bool keyPressed( const OIS::KeyEvent &arg );
	/*!
	\brief Deal with the key released events.
	*/
    bool keyReleased( const OIS::KeyEvent &arg );
	/*!
	\brief Initialization of particle systems of each robot.
	\n Let the scene node of particle systems become the child of robots' scene node.
	*/
	void initParticleSystemForExplosion();
	/*!
	\brief Turn on or turn off the effcts.
	\n Decide the effect should be turned on or off.
	*/
	void setOffParticleSystem();
    //
    // Add your own stuff.
    //
protected:
	/*!
	\brief Viewport array.
	*/
    Ogre::Viewport* mViewportArr[8];
	/*!
	\brief Camera array.
	*/
	Ogre::Camera* mCameraArr[8];
	/*!
	\brief Scene manager array.
	*/
	Ogre::SceneManager* mSceneMgrArr[8];
	/*!
	\brief Camera manager array.
	*/
	OgreBites::SdkCameraMan* mCameraManArr[8];
	/*!
	\brief Light in the scene.
	*/
	Ogre::Light *mLight;
	/*!
	\brief Robot entities.
	*/
	Ogre::Entity *robotEnt[100];
	/*!
	\brief Scene nodes of the robots.
	*/
	Ogre::SceneNode *robotNode[100];
	/*!
	\brief Scene node of the particle systems.
	*/
	Ogre::SceneNode *particle[36];
	/*!
	\brief Sphere entity.
	*/
	Ogre::Entity *sphereEnt;
	/*!
	\brief Top ground.
	*/
	Ogre::Entity *ground;
	/*!
	\brief Lower ground.
	*/
	Ogre::Entity *ground2; 
	/*!
	\brief Sphere scene node.
	*/
	Ogre::SceneNode *sphereNode;
	/*!
	\brief Coordination for the selection rectangle.
	*/
	Real left, top, right, bottom;
	/*!
	\brief The selection rectangle object.
	*/
	SelectionRectangle *mSelectionRect;
	/*!
	\brief Query for selected robots.
	*/
	PlaneBoundedVolumeListSceneQuery *mVolQuery;
	/*!
	\brief Tmp object.
	*/
	SceneNode *mCurrentObject;
	/*!
	\brief Tmp object. 
	*/
	SceneNode *mParticleNode;
	/*!
	\brief List for selected robots.
	*/
	PlaneBoundedVolumeList SelectVolList;
	/*!
	\brief Velocity of the robots. 
	*/
	Vector3 v[36];
	/*!
	\brief Robots' animation state. 
	*/
	AnimationState *mAnimationState[36];
	/*!
	\brief Flag to detect collision between the shpere and the robots. 
	*/
	bool sphereFlg[36];

	/*!
	\brief Flag to detect the mouse button.
	*/
	bool mousePressedFlg;
	/*!
	\brief Flag to decide the points of the selection rectangle. 
	*/
	bool oppFlg;
	/*!
	\brief Flag of the small map.
	*/
	bool winFlg;
	/*!
	\brief If the lower ground is visable or not. 
	*/
	bool btmGroundVis;
	/*!
	\brief Flag of visibility of the particle system. 
	*/
	bool particleFlg;
	/*!
	\brief Flag of the robots' movement. 
	*/
	bool moveFlg;

	/*!
	\brief Destination. 
	*/
	Vector3 des;

    //
    // Add your own stuff.
    //
};


#endif // #ifndef __BasicTutorial_00_h_