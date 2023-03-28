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

#ifndef __SelectionRectangle_h__
#define __SelectionRectangle_h__

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

using namespace Ogre;

class SelectionRectangle : public ManualObject
{
public:
    SelectionRectangle(const String &name)
        : ManualObject(name)
    {
		mDT = 0.01;
        setUseIdentityProjection(true);
        setUseIdentityView(true);
        setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
        setQueryFlags(0);
		
    }

/*!
	\brief Decide the scale when dragging.
*/
void updateDT(Real t)
{
	mDT += t;
}
/*!
	\brief  Update the selection rectangle.
*/
void updateMesh()
{
	setCorners(mLeft, mTop, mRight, mBottom);
}
	/*!
	\brief Set the corners of the rectangle.
	*/
	void setCorners(float left, float top, float right, float bottom)
	{
		mLeft = left;
		mTop = top;
		mRight = right;
		mBottom = bottom;
		//Mapping to Ogre3D Projection Plane Space
		//BEGIN Using CEGUI
		//left = left * 2 - 1;
		//right = right * 2 - 1;
		//top = 1 - top * 2;
		//bottom = 1 - bottom * 2;
		//End USING CEGUI

		//BEGIN Using TrayMgr
		left = left * 2 - 1;
		right = right * 2 - 1;
		top = -1 - top * 2;
		bottom = -1 -bottom * 2;
		//End USING TrayMgr
		//////////////////////////
		Real z = -1;
		clear();
		/*
		begin("Examples/Hilite/Yellow", RenderOperation::OT_LINE_STRIP);
		colour(1, 1, 1);
		position(left, top, z);
		position(right, top, z);
		position(right, bottom, z);
		position(left, bottom, z);
		position(left, top, z);
		*/
		
/////////////////////////////////////////
		//
		//  1----0
		//  | \  | 
		//  |  \ |
		//  3----2 
		
		begin("Examples/Hilite/Yellow", RenderOperation::OT_TRIANGLE_STRIP);
		//colour(1, 1, 1);
		//begin("", RenderOperation::OT_TRIANGLE_STRIP);
		
		
		position(right, top, z);
		textureCoord(5*mDT, 10);
		position(left, top, z);
		textureCoord(0, 10);
		position(right, bottom, z);
		textureCoord(5*mDT, 0);
		position(left, bottom, z);
		textureCoord(0, 0);
/////////////////////////////////////////
	
/////////////////////////////////////////
		
		end();

		AxisAlignedBox box;
		box.setInfinite();
		setBoundingBox(box);
	}
protected:
	Real mDT;
	Real mLeft, mTop, mRight, mBottom;
};


#endif