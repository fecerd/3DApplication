export module VMD;
import Module;
import IO;
using namespace System;
using namespace System::IO;

export namespace System::Formats {
	struct VMDHeader;
	struct VMDMotion;
	struct VMDMorph;
	struct VMDCamera;
	struct VMDLight;
	struct VMDSelfShadow;
	struct VMDIKEnable;
	class VMD;
}

struct System::Formats::VMDHeader {
	char vmdHeader[30];
	char modelName[20];
};

struct System::Formats::VMDMotion {
	char boneName[15];
	uint32_t frameNo;
	Vector3 location;
	Quaternion rotation;
	uint8_t bezier[64];
};

struct System::Formats::VMDMorph {
	char name[15];
	//uint8_t padding[1];
	uint32_t frameNo;
	float weight;	//ウェイト([0.0f, 1.0f])
};

struct System::Formats::VMDCamera {
	uint32_t frameNo;
	float distance;	//距離
	Vector3 pos;	//座標
	Vector3 eulerAngle;	//オイラー角
	uint8_t Interpolation[24];	//補間係数
	uint32_t fov;	//視野角
	uint8_t persFlg;	//パースのOn/Off
	//uint8_t padding[3];
};

struct System::Formats::VMDLight {
	uint32_t frameNo;
	Vector3 rgb;	//光源色
	Vector3 vec;	//平行光線のベクトル
};

struct System::Formats::VMDSelfShadow {
	uint32_t frameNo;
	uint8_t mode;	//影モード(0: 影なし, 1: モード1, 2: モード2)
	//uint8_t padding[3];
	float distance;	//距離
};

struct System::Formats::VMDIKEnable {
	struct IKINFO {
		char name[20];	//対象のボーン名
		uint8_t enabled;	//このボーンのIKが有効か(0: false, 1:true)
	};
public:
	uint32_t frameNo;
	uint8_t visibleFlg;	//モデルの表示(0: false, 1: true)
	uint32_t infoCount;
	IKINFO* infos = nullptr;
public:
	~VMDIKEnable() noexcept {
		delete infos;
		infos = nullptr;
	}
};

class System::Formats::VMD {
public:
	const String filePathName;
	VMDHeader header;
	Vector<VMDMotion> motions;
	Vector<VMDMorph> morph;
	Vector<VMDCamera> cameras;
	Vector<VMDLight> lights;
	Vector<VMDSelfShadow> shadows;
	Vector<VMDIKEnable> ikEnabled;
public:
	VMD(const String& filePath) noexcept : filePathName(filePath) {
		System::IO::FileStream file(filePath, System::IO::OpenMode::IN_BINARY);
		if (!file) return;
		file.Read(&header, 50);
		uint32_t motionNum = 0;
		file.ReadLE(motionNum);
		motions.Reserve(motionNum);
		auto LoadMotion = [](System::IO::FileStream& file, VMDMotion* dst) {
			file.Read(dst, 15);
			file.Read(&dst->frameNo, 16);
			float rot[4];
			file.Read(rot, 16);
			dst->rotation.x = rot[0];
			dst->rotation.y = rot[1];
			dst->rotation.z = rot[2];
			dst->rotation.w = rot[3];
			file.Read(&dst->bezier, 64);
		};
		motions.AddRange(file, motionNum, LoadMotion);
		uint32_t morphCount = 0;
		file.ReadLE(morphCount);
		auto LoadMorph = [](System::IO::FileStream& f, VMDMorph* dst) {
			f.Read(dst, 15);
			f.Read(&dst->frameNo, 8);
		};
		morph.AddRange(file, morphCount, LoadMorph);
		uint32_t cameraCount = 0;
		file.ReadLE(cameraCount);
		auto LoadCamera = [](System::IO::FileStream& f, VMDCamera* dst) {
			f.Read(dst, 61);
		};
		cameras.AddRange(file, cameraCount, LoadCamera);
		uint32_t lightCount = 0;
		file.ReadLE(lightCount);
		auto LoadLight = [](System::IO::FileStream& f, VMDLight* dst) {
			f.Read(dst, 28);
		};
		lights.AddRange(file, lightCount, LoadLight);
		uint32_t shadowCount = 0;
		file.ReadLE(shadowCount);
		auto LoadShadow = [](System::IO::FileStream& f, VMDSelfShadow* dst) {
			f.Read(dst, 5);
			f.ReadLE(dst->distance);
		};
		shadows.AddRange(file, shadowCount, LoadShadow);
		uint32_t ikEnabledCount = 0;
		file.ReadLE(ikEnabledCount);
		auto LoadIKEnabled = [](System::IO::FileStream& f, VMDIKEnable* dst) {
			f.Read(dst, 5);
			f.ReadLE(dst->infoCount);
			delete dst->infos;
			dst->infos = new VMDIKEnable::IKINFO[dst->infoCount];
			for (uint32_t i = 0; i < dst->infoCount; ++i) f.Read(&dst->infos[i], 21);
		};
		ikEnabled.AddRange(file, ikEnabledCount, LoadIKEnabled);
	}
};