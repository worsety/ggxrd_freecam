#pragma once
#include <d3d9.h>
#include <mutex>
#include "CameraMoveInputs.h"
#include "Vector3.h"

using updateDarken_t = void(__thiscall*)(char* thisArg);
using updateCamera_t = void(__thiscall*)(char* thisArg, char** param1, char* param2);

class Camera
{
public:
	bool onDllMain();
	void updateCameraHook(char* thisArg, char** param1, char* param2);
	void updateCameraManually(CameraMoveInputs& inputs);
	bool isFreecamMode = false;
private:
	struct CameraValues {
		float x = 0.F;
		float y = 0.F;
		float z = 0.F;
		int pitch = 0;
		int yaw = 0;
		int roll = 0;
		float fov = 0.F;
	};
	class HookHelp {
		friend class Camera;
		void updateCameraHook(char** param1, char* param2);
	};
	updateCamera_t orig_updateCamera = nullptr;
	std::mutex orig_updateCameraMutex;
	void anglesToDirections(int pitch, int yaw, int roll, Vector3& forward, Vector3& up, Vector3& left, Vector3& yawAxis) const;
	void capAngle(int& angle);
};

extern Camera camera;
