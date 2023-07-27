export module TGA;
import CSTDINT;
import Objects;
import Image;
import File;
import Math;
import Vector;
import Exception;

export namespace System::Drawing {
	class TGA {
		//m_signature[18]�͂��̕�����(�k���I�[����)������
		//"TRUEVISION-TARGA\0"(+�k���I�[)�̏ꍇ������H
		static constexpr char Signature[18] = "TRUEVISION-XFILE.";
	private:
		//ImageID�t�B�[���h�̃o�C�g���B0�̂Ƃ��t�B�[���h�����݂��Ȃ�
		uint8_t m_idLength = 0;
		//�J���[�}�b�v�̗L��(0:�Ȃ��A1:����)
		uint8_t m_colorMapType = 0;
		//�摜�`��
		//0: �Ȃ�
		//1: �C���f�b�N�X�J���[(256�F)
		//2: �t���J���[
		//3: ����
		//4�r�b�g��(8)��1�̂Ƃ�(9����11)�ARLE���k����Ă���
		uint8_t m_imageType = 0;
		//�J���[�}�b�v��ǂݍ��ނƂ��̊J�n�C���f�b�N�X
		uint16_t m_firstEntryIndex = 0;
		//�J���[�}�b�v�̐�
		uint16_t m_colorMapLength = 0;
		//�G���g�����Ƃ̃r�b�g���B�ʏ�A15�A16�A24�A32�̂����ꂩ
		uint8_t m_colorMapEntrySize = 0;
		//�X�N���[����ł̉摜�̍�����X���W
		int16_t m_xOrigin = 0;
		//�X�N���[����ł̉摜�̍�����Y���W
		int16_t m_yOrigin = 0;
		//�摜�̕�
		uint16_t m_width = 0;
		//�摜�̍���
		uint16_t m_height = 0;
		//�s�N�Z�����Ƃ̃r�b�g���B�ʏ�A8�A16�A24�A32�̂����ꂩ
		uint8_t m_pixelDepth = 0;
		//���ʂ���A
		//0-3: ����(�A���t�@�`���l����I�[�o�[���C�@�\�Ɏg�p)
		//4: 0�̂Ƃ�������E�A1�̂Ƃ��E���獶�Ɋi�[
		//5: 0�̂Ƃ��������A1�̂Ƃ��ォ�牺�Ɋi�[
		//6,7: ���g�p�̈�(0�Œ�)
		uint8_t m_imageDescriptor = 0;
		//�摜���ʎq�B[m_idLength]
		uint8_t* m_imageID = nullptr;
		//�J���[�}�b�v�f�[�^�B
		//m_colorMapType��1�̂Ƃ��̂ݑ��݂���B
		//[RoundUp(m_colorMapLength * m_colorMapEntrySize / 8)]
		uint8_t* m_colorMap = nullptr;
		//�摜�f�[�^
		uint8_t* m_data = nullptr;
		//�t�@�C���擪����g���̈�ւ̃I�t�Z�b�g
		//0�̂Ƃ��A�g���̈悪���݂��Ȃ�
		int32_t m_extensionAreaOffset = 0;
		//�t�@�C���擪����J���҃f�B���N�g���ւ̃I�t�Z�b�g
		//0�̂Ƃ��A�J���҃f�B���N�g�������݂��Ȃ�
		int32_t m_developerDirectoryOffset = 0;
		//TGA�t�H�[�}�b�g��\���W��
		char m_signature[18] = {};
	public:
		TGA() noexcept = default;
		TGA(const String& filePath) noexcept {
			Load(filePath);
		}
		TGA(System::IO::FileStream&& file) noexcept {
			Load(file);
		}
		~TGA() noexcept {
			delete[] m_imageID;
			m_imageID = nullptr;
			delete[] m_colorMap;
			m_colorMap = nullptr;
			delete[] m_data;
			m_data = nullptr;
		}
	private:
		size_t GetColorMapByteSize() const noexcept {
			size_t colorMapBitCount = m_colorMapLength * m_colorMapEntrySize;
			return colorMapBitCount / 8 + (colorMapBitCount % 8 == 0 ? 0 : 1);
		}
		size_t GetPixelDataByteSize() const noexcept {
			size_t bit = static_cast<size_t>(m_width) * m_height * m_pixelDepth;
			return bit / 8 + (bit % 8 == 0 ? 0 : 1);
		}
		bool IsLeftToRight() const noexcept {
			return !(m_imageDescriptor & 0x10);
		}
		bool IsBottomToTop() const noexcept {
			return !(m_imageDescriptor & 0x20);
		}
	public:
		static bool IsTGA(System::IO::FileStream& file) noexcept {
			System::IO::StreamPos pos = file.TellPos();
			bool ret = true;
			file.Seek(-18, System::IO::SeekDir::End);
			char signature[18]{};
			file.Read(signature, 18);
			const char check[] = "TRUEVISION-";
			for (size_t i = 0; i < sizeof(check) - 1; ++i) {
				if (signature[i] != check[i]) {
					ret = false;
					break;
				}
			}
			if (signature[17] != '\0') ret = false;
			file.Clear();
			file.Seek(pos);
			return ret;
		}
	public:
		bool Load(const String& filePath) noexcept {
			System::IO::FileStream file(filePath, System::IO::OpenMode::IN_BINARY);
			return Load(file);
		}
		bool Load(System::IO::FileStream& file) noexcept {
			if (!file) return false;
			file.ReadLE(m_idLength);
			file.ReadLE(m_colorMapType);
			file.ReadLE(m_imageType);
			file.ReadLE(m_firstEntryIndex);
			file.ReadLE(m_colorMapLength);
			file.ReadLE(m_colorMapEntrySize);
			file.ReadLE(m_xOrigin);
			file.ReadLE(m_yOrigin);
			file.ReadLE(m_width);
			file.ReadLE(m_height);
			file.ReadLE(m_pixelDepth);
			file.ReadLE(m_imageDescriptor);
			if (m_idLength) {
				m_imageID = new uint8_t[m_idLength];
				file.ReadLE(m_imageID, m_idLength);
			}
			if (m_colorMapType) {
				size_t colorMapByteCount = GetColorMapByteSize();
				m_colorMap = new uint8_t[colorMapByteCount];
				file.Read(m_colorMap, colorMapByteCount);
			}
			size_t pixelByteCount = GetPixelDataByteSize();
			m_data = new uint8_t[pixelByteCount];
			file.Read(m_data, pixelByteCount);
			return static_cast<bool>(file);
		}
	private:
		void LoadTrueColor(Vector<Pixel>& pixels) const noexcept {
			size_t pixelCount = static_cast<size_t>(m_width) * m_height;
			pixels.Reserve(pixelCount);
			const uint8_t* read = m_data;
			if (m_pixelDepth == 24) {
				for (size_t i = 0; i < pixelCount; ++i) {
					pixels.Add(Pixel(read[2], read[1], read[0]));
					read += 3;
				}
			}
			else if (m_pixelDepth == 32) {
				for (size_t i = 0; i < pixelCount; ++i) {
					pixels.Add(Pixel(read[2], read[1], read[0], read[3]));
					read += 4;
				}
			}
		}
	public:
		Image ToImage() const {
			if (!m_data) return Image();
			Vector<Pixel> pixels;
			if (m_colorMap) {

			}
			else {
				if (m_imageType == 2) LoadTrueColor(pixels);
			}
			if (!pixels.Count()) throw LogicException(__FUNCSIG__, u"����TGA�`���̃T�|�[�g�͖������ł�", __FILE__, __LINE__);
			Image ret(m_width, m_height, static_cast<Vector<Pixel>&&>(pixels));
			if (IsBottomToTop()) ret.Reverse();
			return ret;
		}
	};
}