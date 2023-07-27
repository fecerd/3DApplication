export module Type2Charstring;
import CSTDINT;

//HintData
export namespace System {
	//Charstring�`���̓ǂݎ��Ɏg�p����f�[�^�\����
	struct HintData {
		int16_t currentX = 0;
		int16_t currentY = 0;
		int16_t beginningX = 0;
		int16_t beginningY = 0;
		float scale = 1.0f;
		int16_t* hStem = nullptr;
		int16_t* vStem = nullptr;
		uint8_t hStemCountX2 = 0;
		uint8_t vStemCountX2 = 0;
		uint8_t currentHintmask[12] = {};
		uint8_t currentCntrmask[12] = {};
		uint16_t gid = 0;
	public:
		HintData() noexcept = default;
		~HintData() {
			delete[] hStem;
			delete[] vStem;
			hStem = nullptr;
			vStem = nullptr;
		}
	public:
		//�o�^����Ă���X�e��������q���g�}�X�N�̃o�C�g�����v�Z���ĕԂ�
		uint8_t GetMaskSize() const noexcept {
			uint8_t stemCount = (hStemCountX2 / 2) + (vStemCountX2 / 2);
			return stemCount % 8 == 0 ? stemCount / 8 : (stemCount / 8) + 1;
		}
	public:
		//arg[argCount]��Hstem�Ƃ���HintData�ɓo�^���AargCount��0�ɂ���
		void SetHstem(int16_t* const arg, uint8_t& argCount) noexcept {
			hStemCountX2 = argCount;
			argCount = 0;
			hStem = new int16_t[hStemCountX2];
			for (uint8_t j = 0; j < hStemCountX2; ++j) hStem[j] = arg[j];
		}
		//arg[argCount]��Vstem�Ƃ���HintData�ɓo�^���AargCount��0�ɂ���
		void SetVstem(int16_t* const arg, uint8_t& argCount) noexcept {
			vStemCountX2 = argCount;
			argCount = 0;
			vStem = new int16_t[vStemCountX2];
			for (uint8_t j = 0; j < vStemCountX2; ++j) vStem[j] = arg[j];
		}
	};
}
