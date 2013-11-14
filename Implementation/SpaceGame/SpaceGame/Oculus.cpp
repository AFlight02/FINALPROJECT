#include "Oculus.h"

namespace
{
	const float g_defaultNearClip = 0.01f;
	const float g_defaultFarClip = 10000.0f;
	const float g_defaultIPD = 0.064f;
	const Ogre::ColourValue g_defaultViewportColour(97/255.0f, 97/255.0f, 200/255.0f);
	const float g_defaultProjectionCentreOffset = 0.14529906f;
	const float g_defaultDistortion[4] = {1.0f, 0.22f, 0.24f, 0};
}

Oculus::Oculus(void) : mSensorFusion(0),
					   mStereoConfig(0),
					   mHmd(0),
					   mDeviceManager(0),
					   mOculusReady(false),
					   mOgreReady(false),
					   mSensor(0),
					   mCentreOffset(g_defaultProjectionCentreOffset),
					   mWindow(0),
					   mSceneManager(0),
					   mCameraNode(0)
{
	for(int i=0;i<2;++i)
	{
		mCameras[i] = 0;
		mViewports[i] = 0;
		mCompositors[i] = 0;
	}
}


Oculus::~Oculus(void)
{
	shutDownOculus();
}

void Oculus::shutDownOculus()
{
	mOculusReady = false;

	delete mStereoConfig;
	mStereoConfig = 0;
	delete mSensorFusion;
	mSensorFusion = 0;

	if(mSensor)
	{
		mSensor->Release();
	}
	if(mHmd)
	{
		mHmd->Release();
		mHmd = 0;
	}
	if(mDeviceManager)
	{
		mDeviceManager->Release();
		mDeviceManager = 0;
	}

	OVR::System::Destroy();
}

void Oculus::shutDownOgre()
{
	mOgreReady = false;
	for(int i=0;i<2;++i)
	{
		if(mCompositors[i])
		{
			Ogre::CompositorManager::getSingleton().removeCompositor(mViewports[i], "Oculus");
			mCompositors[i] = 0;
		}
		if(mViewports[i])
		{
			mWindow->removeViewport(i);
			mViewports[i] = 0;
		}
		if(mCameras[i])
		{
			mCameras[i]->getParentSceneNode()->detachObject(mCameras[i]);
			mSceneManager->destroyCamera(mCameras[i]);
			mCameras[i] = 0;
		}
	}
	if(mCameraNode)
	{
		mCameraNode->getParentSceneNode()->removeChild(mCameraNode);
		mSceneManager->destroySceneNode(mCameraNode);
		mCameraNode = 0;
	}
	mWindow = 0;
	mSceneManager = 0;
}

bool Oculus::isOculusReady() const { return mOculusReady; }
bool Oculus::isOgreReady() const { return mOgreReady; }

bool Oculus::setupOculus()
{
	if(mOculusReady)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus - already initialised");
		return true;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus - initialising");
	OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));
	mDeviceManager = OVR::DeviceManager::Create();
	if(!mDeviceManager)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus - failed to initialise OVR Device Manager");
		return false;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus - initialised Device Manager");
	mStereoConfig = new OVR::Util::Render::StereoConfig();
	if(!mStereoConfig)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus - failed to initialise StereoConfig");
	}
	mCentreOffset = mStereoConfig->GetProjectionCenterOffset();
	Ogre::LogManager::getSingleton().logMessage("Oculus - created SteroConfig");
	mHmd = mDeviceManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
	if(!mHmd)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus - failed to initialise HMD");
		return false;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus - created HMD");
	OVR::HMDInfo deviceInfo;
	mHmd->GetDeviceInfo(&deviceInfo);
	mStereoConfig->SetHMDInfo(deviceInfo);
	mSensor = mHmd->GetSensor();
	if(!mSensor)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus - failed to initialise Sensor");
		return false;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus - created Sensor");
	mSensorFusion = new OVR::SensorFusion();
	if(!mSensorFusion)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus - failed to initialise SensorFusion");
		return false;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus - created SensorFusion");
	mSensorFusion->AttachToSensor(mSensor);
	mOculusReady = true;
	Ogre::LogManager::getSingleton().logMessage("Oculus - Oculus Setup Complete :: OCULUS READY");
	return true;
}

bool Oculus::setupOgre(Ogre::SceneManager *sceneManager, Ogre::RenderWindow *window, Ogre::SceneNode *parent)
{
	mWindow = window;
	mSceneManager = sceneManager;
	Ogre::LogManager::getSingleton().logMessage("Oculus - integrating with Ogre");
	if(parent)
		mCameraNode = parent->createChildSceneNode("OculusStereoCameraNode");
	else
		mCameraNode = sceneManager->getRootSceneNode()->createChildSceneNode("OculusStereoCameraNode");

	mCameras[0] = sceneManager->createCamera("OculusCamLeft");
	mCameras[1] = sceneManager->createCamera("OculusCamRight");

	Ogre::MaterialPtr matLeft = Ogre::MaterialManager::getSingleton().getByName("Ogre/Compositor/Oculus");
	Ogre::MaterialPtr matRight = matLeft->clone("Ogre/Compositor/Oculus/Right");
	Ogre::GpuProgramParametersSharedPtr pParamsLeft = matLeft->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	Ogre::GpuProgramParametersSharedPtr pParamsRight = matRight->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	Ogre::Vector4 warp;

	if(mStereoConfig)
	{
		warp = Ogre::Vector4(mStereoConfig->GetDistortionK(0), mStereoConfig->GetDistortionK(1), mStereoConfig->GetDistortionK(2), mStereoConfig->GetDistortionK(3));
	} else 
	{
		warp = Ogre::Vector4(g_defaultDistortion[0], g_defaultDistortion[1], g_defaultDistortion[2], g_defaultDistortion[3]);
	}
	pParamsLeft->setNamedConstant("HmdWarpParam", warp);
	pParamsRight->setNamedConstant("HmdWarpParam", warp);
	pParamsLeft->setNamedConstant("LensCentre", 0.5f+(mStereoConfig->GetProjectionCenterOffset()/2.0f));
	pParamsRight->setNamedConstant("LensCentre", 0.5f+(mStereoConfig->GetProjectionCenterOffset()/2.0f));

	Ogre::CompositorPtr compositor = Ogre::CompositorManager::getSingleton().getByName("OculusRight");
	compositor->getTechnique(0)->getOutputTargetPass()->getPass(0)->setMaterialName("Ogre/Compositor/Oculus/Right");

	for(int i=0; i<2; ++i)
	{
		mCameraNode->attachObject(mCameras[i]);
		if(mStereoConfig)
		{
			mCameras[i]->setNearClipDistance(mStereoConfig->GetEyeToScreenDistance());
			mCameras[i]->setFarClipDistance(g_defaultFarClip);
			mCameras[i]->setPosition((i*2-1) * mStereoConfig->GetIPD() * 0.5f, 0, 0);
			mCameras[i]->setAspectRatio(mStereoConfig->GetAspect());
			mCameras[i]->setFOVy(Ogre::Radian(mStereoConfig->GetYFOVRadians()));

			Ogre::Matrix4 proj = Ogre::Matrix4::IDENTITY;
			float temp = mStereoConfig->GetProjectionCenterOffset();
			proj.setTrans(Ogre::Vector3(-mStereoConfig->GetProjectionCenterOffset() * (2*i-1), 0, 0));
			mCameras[i]->setCustomProjectionMatrix(true, proj * mCameras[i]->getProjectionMatrix());
		} else
		{
			mCameras[i]->setNearClipDistance(g_defaultNearClip);
			mCameras[i]->setFarClipDistance(g_defaultFarClip);
			mCameras[i]->setPosition((i*2-1) * g_defaultIPD * 0.5f, 0, 0);
		}
		mViewports[i] = window->addViewport(mCameras[i], i, 0.5f*i, 0, 0.5f, 1.0f);
		mViewports[i]->setBackgroundColour(g_defaultViewportColour);
		mCompositors[i] = Ogre::CompositorManager::getSingleton().addCompositor(mViewports[i], i==0 ? "OculusLeft" : "OculusRight");
		mCompositors[i]->setEnabled(true);
	}
	mOgreReady = true;
	Ogre::LogManager::getSingleton().logMessage("Oculus - Oculus and Ogre set up!");
	return true;
}

void Oculus::updateCamera() 
{
	if(mOgreReady)
		mCameraNode->setOrientation(getOrientation());
}

Ogre::SceneNode *Oculus::getCameraNode()
{
	return mCameraNode;
}

Ogre::Quaternion Oculus::getOrientation() const
{
	if(mOculusReady)
	{
		OVR::Quatf q = mSensorFusion->GetOrientation();
		return Ogre::Quaternion(q.w, q.x, q.y, q.z);
	} else
	{
		return Ogre::Quaternion::IDENTITY;
	}
}

Ogre::CompositorInstance *Oculus::getCompositor(unsigned int i)
{
	return mCompositors[i];
}

float Oculus::getCentreOffset() const
{
	return mCentreOffset;
}

void Oculus::resetOrientation()
{
	if(mSensorFusion)
		mSensorFusion->Reset();
}