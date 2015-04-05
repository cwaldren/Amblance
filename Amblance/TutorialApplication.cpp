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
	mSceneMgr->setShadowTechnique(Ogre::SHADOWDETAILTYPE_TEXTURE);

	
	Ogre::Entity* entAmbulance = mSceneMgr->createEntity("Ambulance", "amblance.mesh");
	entAmbulance->setCastShadows(true);
	entAmbulance->setMaterialName("Amblance/Amblance");
	
	Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("AmbulanceNode");
	node->attachObject(entAmbulance);

	//-----------
	Ogre::SceneNode* node2 = node->createChildSceneNode("CameraNode", Ogre::Vector3(0,-10,-25));
	node2->attachObject(mCamera);
	//------------


	node->scale(Ogre::Vector3(5,4,5));
	node->setPosition(0,25, 100);

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

	
	Ogre::Light* sun = mSceneMgr->createLight("sun");
	sun->setType(Ogre::Light::LT_DIRECTIONAL);

	
	sun->setDiffuseColour(Ogre::ColourValue::White);
	sun->setSpecularColour(Ogre::ColourValue::White);
	sun->setDirection(.3, -.8, -1);
	//---------------------------------------------------------------------------------------------------
	//Cars in op direction
	Ogre::Entity* car = mSceneMgr->createEntity("car", "amblance.mesh");
	car->setCastShadows(true);
	car->setMaterialName("Amblance/Amblance");

	Ogre::SceneNode* carNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CarNode");
	carNode->attachObject(car);

	carNode->scale(Ogre::Vector3(5,4,-5));
	carNode->setPosition(25,25, 1500);
	//-------------------------------------------------------------------
	Ogre::Entity* car2 = mSceneMgr->createEntity("car2", "amblance.mesh");
	car2->setCastShadows(true);
	car2->setMaterialName("Amblance/Amblance");

	Ogre::SceneNode* carNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("CarNode2");
	carNode2->attachObject(car2);

	carNode2->scale(Ogre::Vector3(5,4,5));
	carNode2->setPosition(-35,25, 1500);
	
}


void TutorialApplication::createCamera()
{
	// Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,150,-70));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,60,100));
    mCamera->setNearClipDistance(5);

	mCameraMan = 0;
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void TutorialApplication::createViewport(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}



bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (mWindow->isClosed()) return false;
	if (mShutDown) return false;
	mKeyboard->capture();
	mMouse->capture();
	mTrayMgr->frameRenderingQueued(evt);
	//Boundry Checks
	if(mSceneMgr->getSceneNode("AmbulanceNode")->getPosition().x > leftBound && mDirection.x == mMove) {
		mDirection.x = 0;
	} else if( mSceneMgr->getSceneNode("AmbulanceNode")->getPosition().x < rightBound && mDirection.x == -mMove) {
		mDirection.x = 0;
	}
	mSceneMgr->getSceneNode("AmbulanceNode")->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	//op car
	if(mSceneMgr->getSceneNode("CarNode")->getPosition().z < mSceneMgr->getSceneNode("AmbulanceNode")->getPosition().z - 100) {
		mSceneMgr->getSceneNode("CarNode")->setPosition(25,25,mSceneMgr->getSceneNode("AmbulanceNode")->getPosition().z + respawnDist);
	} else {
		mSceneMgr->getSceneNode("CarNode")->translate(mCarMoveT * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}
	//op car2
	if(mSceneMgr->getSceneNode("CarNode2")->getPosition().z < mSceneMgr->getSceneNode("AmbulanceNode")->getPosition().z - 100) {
		mSceneMgr->getSceneNode("CarNode2")->setPosition(-35,25,mSceneMgr->getSceneNode("AmbulanceNode")->getPosition().z + respawnDist);
	} else {
		mSceneMgr->getSceneNode("CarNode2")->translate(mCarMoveA * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}
	mSceneMgr->getSceneNode("AmbulanceNode")->getChild("CameraNode")->translate(mCameraDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	mCamera->lookAt(mSceneMgr->getSceneNode("AmbulanceNode")->getPosition()+Ogre::Vector3(0,0,100));

	
	return true;
}

void TutorialApplication::createFrameListener(void) 
{
	BaseApplication::createFrameListener();
	mRotate = .13;
	mMove = 250;
	leftBound = 100;
	rightBound = -107;
	mDirection = Ogre::Vector3(0,0,500);
	mCameraDirection = Ogre::Vector3::ZERO;
	//op cars
	mCarMoveT = Ogre::Vector3(0,0,-250);
	mCarMoveA = Ogre::Vector3(0,0,250);
	respawnDist = 1500;
}

bool TutorialApplication::keyPressed( const OIS::KeyEvent& evt )
{
	switch(evt.key) 
	{
	case OIS::KC_ESCAPE:
		mShutDown = true;
		break;
	case OIS::KC_W:
		mDirection.z = mMove;
		break;
	case OIS::KC_S:
		mDirection.z = -mMove;
		break;
	case OIS::KC_A:
		mDirection.x = mMove;
		break;
	case OIS::KC_D:
		mDirection.x = -mMove;
		break;
	case OIS::KC_PGDOWN:
		mCameraDirection.y = 10;
		break;
	case OIS::KC_PGUP:
		mCameraDirection.y = -10;
		break;

	default:
		break;
	}
	return true;
}
bool TutorialApplication::keyReleased( const OIS::KeyEvent& evt )
{
	switch (evt.key)
	{
	case OIS::KC_W:
		mDirection.z = 0;
		break;
 
	case OIS::KC_S:
		mDirection.z = 0;
		break;
 
	case OIS::KC_A:
		mDirection.x = 0;
		break;
 
	case OIS::KC_D:
		mDirection.x = 0;
		break;

	case OIS::KC_PGDOWN:
		mCameraDirection.y = 0;
		break;
	case OIS::KC_PGUP:
		mCameraDirection.y = 0;
		break;
	default:
		break;
	}
	return true;
}
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
