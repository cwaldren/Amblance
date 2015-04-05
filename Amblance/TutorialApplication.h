/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    virtual void createScene(void);


	virtual void createCamera(void);
	virtual void createViewport(void);

	virtual void createFrameListener(void);



	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	
	virtual bool keyPressed(const OIS::KeyEvent& evt);
	virtual bool keyReleased(const OIS::KeyEvent& evt);

	virtual bool mouseMoved(const OIS::MouseEvent& evt);
	virtual bool mousePressed (const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::Vector3 mDirection;
	Ogre::Vector3 mCameraDirection;

private:
	bool processUnbufferedInput(const Ogre::FrameEvent& evt);

};

#endif // #ifndef __TutorialApplication_h_
