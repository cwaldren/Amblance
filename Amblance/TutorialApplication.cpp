/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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
#include "TutorialApplication.h"

//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.15, 0.15, 0.15));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	
	Ogre::Entity* entAmbulance = mSceneMgr->createEntity("Ambulance", "amblance.mesh");
	entAmbulance->setCastShadows(true);
	entAmbulance->setMaterialName("Amblance/Amblance");
	
	Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("AmbulanceNode");
	node->attachObject(entAmbulance);
	node->scale(Ogre::Vector3(5,4,5));
	node->setPosition(0,20, 100);
	
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::Plane road(Ogre::Vector3::UNIT_Y, 4);

	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 2048*40, 2048*40, 20, 20, true, 1, 400, 400, Ogre::Vector3::UNIT_Z);
	
	Ogre::MeshManager::getSingleton().createPlane("road", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		road, 256,2048 * 40, 12, 12, true, 1, 1, 400, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	Ogre::Entity* entRoad   = mSceneMgr->createEntity("RoadEntity", "road");

	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entRoad);

	entGround->setMaterialName("Amblance/Grass");
	entGround->setCastShadows(false);

	entRoad->setMaterialName("Amblance/Road");
	entRoad->setCastShadows(false);


	Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(200,200,0));

	
	pointLight->setDiffuseColour(Ogre::ColourValue::White);
	pointLight->setSpecularColour(Ogre::ColourValue::White);
	//pointLight->setAttenuation(10000,1,1,1);
	

}

bool TutorialApplication::processUnbufferedInput(const Ogre::FrameEvent& evt)
{ 
	static bool mMouseDown = false;     // If a mouse button is depressed
    static Ogre::Real mToggle = 0.0;    // The time left until next toggle
    static Ogre::Real mRotate = 0.13;   // The rotate constant
    static Ogre::Real mMove = 250; 

	bool currMouse = mMouse->getMouseState().buttonDown(OIS::MB_Left);
	if (currMouse && !mMouseDown)
	{
		Ogre::Light* light = mSceneMgr->getLight("pointLight");
		light->setVisible(! light->isVisible());
	}
	mMouseDown = currMouse;

	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	if (mKeyboard->isKeyDown(OIS::KC_I)) {
		transVector.z += mMove;
	}
	if (mKeyboard->isKeyDown(OIS::KC_K)) {
		transVector.z -= mMove;
	}
	if (mKeyboard->isKeyDown(OIS::KC_J)) {
		if(mKeyboard->isKeyDown( OIS::KC_LSHIFT )) {
			mSceneMgr->getSceneNode("AmbulanceNode")->yaw(Ogre::Degree(mRotate * 5));
		} else {
			transVector.x -= mMove; // Strafe left
		}
	}
	if (mKeyboard->isKeyDown(OIS::KC_L))  {
		if(mKeyboard->isKeyDown( OIS::KC_LSHIFT )) {
			mSceneMgr->getSceneNode("AmbulanceNode")->yaw(Ogre::Degree(-mRotate * 5));
		} else {
			transVector.x += mMove; // Strafe right
		}
	}
	if (mKeyboard->isKeyDown(OIS::KC_U)) // Up
	{
		transVector.y += mMove;
	}
	if (mKeyboard->isKeyDown(OIS::KC_O)) // Down
	{
		transVector.y -= mMove;
	}
	mSceneMgr->getSceneNode("AmbulanceNode")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	
    return true;
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	return true;
}

void TutorialApplication::createFrameListener(void) 
{
	BaseApplication::createFrameListener();
}

bool TutorialApplication::keyPressed( const OIS::KeyEvent& evt ){return true;}
bool TutorialApplication::keyReleased( const OIS::KeyEvent& evt ){return true;}
// OIS::MouseListener
bool TutorialApplication::mouseMoved( const OIS::MouseEvent& evt ){return true;}
bool TutorialApplication::mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){return true;}
bool TutorialApplication::mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){return true;}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
