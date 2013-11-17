/*
 * Oculus.h integrates the Oculus OVR library with the SpaceGame project
 * Author : Alex Flight
 * Version : 1.0
 * Date : 14/11/2013
 */

#ifndef OCULUS_H
#define OCULUS_H

#pragma once

#include "OVR.h"
#include "Ogre.h"

class Oculus
{
public:
	Oculus();
	~Oculus();

	bool setupOculus();
	bool setupOgre(Ogre::SceneManager *sceneManager, Ogre::RenderWindow *window, Ogre::SceneNode *parent);
	void shutDownOculus();
	void shutDownOgre();
	bool isOculusReady() const;
	bool isOgreReady() const;

	void updateCamera();
	void resetOrientation();

	Ogre::SceneNode *getCameraNode();
	Ogre::Quaternion getOrientation() const;
	Ogre::CompositorInstance *getCompositor(unsigned int i);
	Ogre::Camera *getCamera(unsigned int i);
	Ogre::ViewPoint *getViewport(unsigned int i);
	
	float getCentreOffset() const;

protected:
	OVR::DeviceManager *mDeviceManager;
	OVR::HMDDevice *mHmd;
	OVR::Util::Render::StereoConfig *mStereoConfig;
	OVR::SensorDevice *mSensor;
	OVR::SensorFusion *mSensorFusion;
	bool mOculusReady;		/// Has the oculus rift been fully initialised?
	bool mOgreReady;
	Ogre::SceneManager *mSceneManager;
	Ogre::RenderWindow *mWindow;
	Ogre::SceneNode *mCameraNode;
	Ogre::Quaternion mOrientation;
	float mCentreOffset;	/// Projection centre offset.
	Ogre::Camera *mCameras[2];
	Ogre::Viewport *mViewports[2];
	Ogre::CompositorInstance *mCompositors[2];
};

#endif