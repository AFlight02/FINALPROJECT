// ==========================================================================
// MinimalOculus - Minimal Oculus Application
// Show hardware parameters and head tracker values.
// ==========================================================================
// Revision History, Last Change First:
// YYYY/MM/DD Author               Description
// ========== ==================== ==========================================
// 2013/09/08 AFlight			   Implemented to test
// 2013/04/12 Geekmaster           Add Acceleration Data.
// 2013/02/22 Cybereality          First release version.

#include "OVR.h"
#include <iostream>
#include <conio.h>

//#define STD_GRAV 9.81 // What SHOULD work with Rift, but off by 1000
#define STD_GRAV 9.81  // This gives nice 1.00G on Z with Rift face down !!!

using namespace OVR;
using namespace std;

Ptr<DeviceManager>	pManager;
Ptr<HMDDevice>		pHMD;
Ptr<SensorDevice>	pSensor;
SensorFusion		FusionResult;
HMDInfo			Info;
bool			InfoLoaded;

void Init()
{
	System::Init();

	pManager = *DeviceManager::Create();

	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if (pHMD)
        {
           InfoLoaded = pHMD->GetDeviceInfo(&Info);

	   pSensor = *pHMD->GetSensor();
	}
	else
	{
	   pSensor = *pManager->EnumerateDevices<SensorDevice>().CreateDevice();
	}

	if (pSensor)
	{
	   FusionResult.AttachToSensor(pSensor);
	}
}

void Clear()
{
	pSensor.Clear();
        pHMD.Clear();
	pManager.Clear();

	System::Destroy();
}

void Output()
{
	cout << "----- Oculus Console -----" << endl;

	if (pHMD)
	{
		cout << " [x] HMD Found" << endl;
	}
	else
	{
		cout << " [ ] HMD Not Found" << endl;
	}

	if (pSensor)
	{
		cout << " [x] Sensor Found" << endl;
	}
	else
	{
		cout << " [ ] Sensor Not Found" << endl;
	}

	cout << "--------------------------" << endl;

	if (InfoLoaded)
        {
		cout << " DisplayDeviceName: " << Info.DisplayDeviceName << endl;
		cout << " ProductName: " << Info.ProductName << endl;
		cout << " Manufacturer: " << Info.Manufacturer << endl;
		cout << " Version: " << Info.Version << endl;
		cout << " HResolution: " << Info.HResolution<< endl;
		cout << " VResolution: " << Info.VResolution<< endl;
		cout << " HScreenSize: " << Info.HScreenSize<< endl;
		cout << " VScreenSize: " << Info.VScreenSize<< endl;
		cout << " VScreenCenter: " << Info.VScreenCenter<< endl;
		cout << " EyeToScreenDistance: " << Info.EyeToScreenDistance << endl;
		cout << " LensSeparationDistance: " << Info.LensSeparationDistance << endl;
		cout << " InterpupillaryDistance: " << Info.InterpupillaryDistance << endl;
		cout << " DistortionK[0]: " << Info.DistortionK[0] << endl;
		cout << " DistortionK[1]: " << Info.DistortionK[1] << endl;
		cout << " DistortionK[2]: " << Info.DistortionK[2] << endl;
		cout << "--------------------------" << endl;
        }

	cout << endl << " Press ENTER to continue" << endl;

	cin.get();

	while(pSensor)
	{
		Vector3f acc = FusionResult.GetAcceleration();
		Quatf quaternion = FusionResult.GetOrientation();

		float yaw, pitch, roll;
		quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);

		cout << "Yaw=" << RadToDegree(yaw) << 
			" Pitch=" << RadToDegree(pitch) << 
			" Roll=" << RadToDegree(roll) <<
			" X=" << acc.x / STD_GRAV << 
			" Y=" << acc.y / STD_GRAV << 
			" Z=" << acc.z / STD_GRAV << endl;

		Sleep(50);

		if (_kbhit()) exit(0);
	}
}

int main()
{
	Init();
	Output();
	Clear();
}