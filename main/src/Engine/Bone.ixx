export module Bone;
import System;
using namespace System;

export namespace Engine {
	struct Bone;
	struct IK;
}

struct Engine::Bone {
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	size_t id = System::MAX_VALUE<size_t>;
	size_t parent = 0;
	String name;
	ControlPoint rotationPoint[2] = { { true, 0, 0 }, { true, 127, 127 } };
	ControlPoint xTranslatePoint[2] = { { true, 0, 0 }, { true, 127, 127 } };
	ControlPoint yTranslatePoint[2] = { { true, 0, 0 }, { true, 127, 127 } };
	ControlPoint zTranslatePoint[2] = { { true, 0, 0 }, { true, 127, 127 } };
	bool isIKBone = false;
public:
	Bone() noexcept = default;
	Bone(const Bone&) noexcept = default;
	Bone(Bone&& arg) noexcept = default;
public:
	Bone& operator=(const Bone&) noexcept = default;
	Bone& operator=(Bone&& rhs) noexcept = default;
};

struct Engine::IK {
	size_t id = System::MAX_VALUE<size_t>;
	size_t targetID = System::MAX_VALUE<size_t>;
	Vector<size_t> chainID;
	size_t interations = 1;
	float limit = 180;
};