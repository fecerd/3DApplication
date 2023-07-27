export module BMP;
import Objects;
import Image;
using Image = System::Drawing::Image;
import File;
import Math;
import Vector;
import Exception;

namespace System::Drawing {
	/// <summary>
	/// BMP�t�@�C���̃t�@�C���w�b�_��\���\����
	/// </summary>
	struct BitmapFileHeader {
		//�t�@�C���^�C�v�̕W���B{ 'B', 'M' }�Œ�
		uint8_t bfType[2];
		//�t�@�C���̃o�C�g���B�f�R�[�_�͎g�p���ׂ��łȂ�
		uint32_t bfSize;
		//�\��̈�B0�Œ�
		uint16_t bfReserved1;
		//�\��̈�B0�Œ�
		uint16_t bfReserved2;
		//�t�@�C���̐擪����摜�f�[�^�܂ł̃I�t�Z�b�g(�o�C�g)
		//0�ȊO�̏ꍇ�A�f�R�[�_�͎g�p���Ă��悢
		uint32_t bfOffBits;
	public:
		/// <summary>
		/// �t�@�C���w�b�_��ǂݎ��
		/// </summary>
		/// <param name="file">���݈ʒu��BitmapFileHeader�̐擪���w���Ă���File�I�u�W�F�N�g</param>
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(bfType);
			if (bfType[0] != 'B' || (bfType[1] != 'M')) return false;
			file.ReadLE(bfSize);
			file.ReadLE(bfReserved1);
			file.ReadLE(bfReserved2);
			file.ReadLE(bfOffBits);
			return static_cast<bool>(file);
		}
		/// <summary>
		/// File�I�u�W�F�N�g�̌��݈ʒu�Ƀt�@�C���w�b�_����������
		/// </summary>
		/// <param name="file">�������ݐ��File�I�u�W�F�N�g</param>
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(bfType);
			file.WriteLE(bfSize);
			file.WriteLE(bfReserved1);
			file.WriteLE(bfReserved2);
			file.WriteLE(bfOffBits);
		}
	public:
		/// <summary>
		/// �t�@�C���T�C�Y�Ɖ摜�f�[�^�܂ł̃I�t�Z�b�g���w�肵�ď���������
		/// </summary>
		/// <param name="fileSize">�t�@�C���S�̂̃o�C�g���B0�ł��悢</param>
		/// <param name="pixelDataOffset">�t�@�C���̐擪����摜�f�[�^�܂ł̃I�t�Z�b�g(�o�C�g)�B0�ł��悢</param>
		void Init(uint32_t fileSize, uint32_t pixelDataOffset) noexcept {
			bfType[0] = 'B';
			bfType[1] = 'M';
			bfSize = fileSize;
			bfReserved1 = 0;
			bfReserved2 = 0;
			bfOffBits = pixelDataOffset;
		}
	};

	//OS/2�Ŏg�p����Ă���CORE�^�C�v�̏��w�b�_��\���\����
	struct BitmapCoreInfoHeader {
		//�摜�f�[�^�̕��B1�ȏ�̐��l������
		uint16_t bcWidth;
		//�摜�f�[�^�̍����B1�ȏ�̐��l������
		uint16_t bcHeight;
		//�`�����l�����B1�Œ�
		uint16_t bcPlanes;
		//�s�N�Z�����Ƃ̃r�b�g���B1, 4, 8, 24���g�p����
		uint16_t bcBitCount;
	public:
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(bcWidth);
			file.ReadLE(bcHeight);
			file.ReadLE(bcPlanes);
			file.ReadLE(bcBitCount);
			return static_cast<bool>(file);
		}
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(bcWidth);
			file.WriteLE(bcHeight);
			file.WriteLE(bcPlanes);
			file.WriteLE(bcBitCount);
		}
	public:
		void Init(uint16_t width, uint16_t height, uint16_t bitCount) noexcept {
			bcWidth = width;
			bcHeight = height;
			bcPlanes = 1;
			bcBitCount = bitCount;
		}
	public:
		uint32_t GetLineSize() const noexcept {
			return static_cast<uint32_t>((static_cast<size_t>(bcWidth) * bcBitCount + 31) / 32ull * 4);
		}
	};

	//�W���I�ȏ��w�b�_��\���\����
	struct BitmapInfoInfoHeader {
		//�摜�̕�(px)�B0�ȉ��ɂ͂Ȃ�Ȃ�
		int32_t biWidth;
		//�摜�̍���(px)�B0�ȊO�̐��l������B
		//�����̏ꍇ�A�摜�f�[�^�͉��̍s����i�[����(�{�g���A�b�v)�A
		//�����̏ꍇ�A�摜�f�[�^�͏�̍s����i�[�����(�g�b�v�_�E��)
		int32_t biHeight;
		//�`�����l�����B1�Œ�
		uint16_t biPlanes;
		//�s�N�Z�����Ƃ̃r�b�g���B
		//0, 1, 4, 8, 16, 24, 32�̂����ꂩ���g�p����
		uint16_t biBitCount;
		//�摜�f�[�^�̌`����\���B
		//0: �񈳏k�`���BbiBitCount��0�ȊO���g�p����B
		//1: 8�r�b�gRLE�`���BbiBitCount��8�Œ�B
		//2: 4�r�b�gRLE�`���BbiBitCount��4�Œ�B
		//3: �r�b�g�t�B�[���h�����񈳏k�`���BbiBitCount��16��32���g�p����B
		//4: JPEG�`���BbiBitCount��0�Œ�B
		//5: PNG�`���BbiBitCount��0�Œ�B
		uint32_t biCompression;
		//�摜�f�[�^�̃o�C�g��
		uint32_t biSizeImage;
		//��������1���[�g�����Ƃ̃s�N�Z�����B
		//�s�v�ȏꍇ�A0���i�[����
		int32_t biXPixPerMeter;
		//�c������1���[�g�����Ƃ̃s�N�Z�����B
		//�s�v�ȏꍇ�A0���i�[����
		int32_t biYPixPerMeter;
		//�J���[�p���b�g�Ɋi�[�����F���B
		//���̒l��0���AbiBitCount��8�ȉ��̏ꍇ�A
		//�J���[�p���b�g�̐F����(2^biBitCount)�ɂȂ�
		uint32_t biClrUsed;
		//�J���[�p���b�g�̐F�̂����A���m�ɕ\�����ׂ��F�̐��B
		//�J���[�p���b�g�̐擪����A���̐��l�̕��̐F���d�v�F�ł���B
		//0�̏ꍇ�A���ׂĂ̐F���d�v�F�ƂȂ�
		uint32_t biClrImportant;
	public:
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(biWidth);
			file.ReadLE(biHeight);
			file.ReadLE(biPlanes);
			file.ReadLE(biBitCount);
			file.ReadLE(biCompression);
			file.ReadLE(biSizeImage);
			file.ReadLE(biXPixPerMeter);
			file.ReadLE(biYPixPerMeter);
			file.ReadLE(biClrUsed);
			file.ReadLE(biClrImportant);
			return static_cast<bool>(file);
		}
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(biWidth);
			file.WriteLE(biHeight);
			file.WriteLE(biPlanes);
			file.WriteLE(biBitCount);
			file.WriteLE(biCompression);
			file.WriteLE(biSizeImage);
			file.WriteLE(biXPixPerMeter);
			file.WriteLE(biYPixPerMeter);
			file.WriteLE(biClrUsed);
			file.WriteLE(biClrImportant);
		}
	public:
		void Init(uint32_t width, int32_t height, uint16_t bitCount) noexcept {
			biWidth = static_cast<int32_t>(width);
			biHeight = height;
			biPlanes = 1;
			biBitCount = bitCount;
			biCompression = 0;
			uint32_t lineSize = GetLineSize();
			biSizeImage = static_cast<uint32_t>(lineSize * System::Math::Abs(biHeight));
			biXPixPerMeter = 0;
			biYPixPerMeter = 0;
			biClrUsed = 0;
			biClrImportant = 0;
		}
	public:
		uint32_t GetLineSize() const noexcept {
			return static_cast<uint32_t>((static_cast<size_t>(biWidth) * biBitCount + 31) / 32ull * 4);
		}
	};

	//V4�^�C�v�̏��w�b�_��\���\����
	struct BitmapV4InfoHeader : public BitmapInfoInfoHeader {
		//�Ԑ����̃J���[�}�X�N�l�B�r�b�g�t�B�[���h�����񈳏k�`���̏ꍇ�A�L��
		uint32_t bv4RedMask;
		//�ΐ����̃J���[�}�X�N�l�B�r�b�g�t�B�[���h�����񈳏k�`���̏ꍇ�A�L��
		uint32_t bv4GreenMask;
		//�����̃J���[�}�X�N�l�B�r�b�g�t�B�[���h�����񈳏k�`���̏ꍇ�A�L��
		uint32_t bv4BlueMask;
		//�A���t�@�����̃J���[�}�X�N�l�B�r�b�g�t�B�[���h�����񈳏k�`���̏ꍇ�A�L��
		uint32_t bv4AlphaMask;
		//�F��ԃ^�C�v�BV4�^�C�v�̏��w�b�_�ł�0�Œ�B
		//"BGRs"(0x73524742): sRGB��Ԃ�����
		//" niW"(0x57696e20): Windows�W���̐F��Ԃ�����
		//"KNIL"(0x4c494e4b): BMP�t�@�C�����ɐF��ԃv���t�@�C���t�@�C���ւ̃p�X���܂܂��
		//"DEBM"(0x4d424544): BMP�t�@�C�����ɐF��ԃv���t�@�C�������ߍ��܂�Ă���
		uint32_t bv4CSType;
		//CIEXYZ�J���[���f���̐Ԑ����̍��W(x, y, z)�Bbv4CSType��0�̏ꍇ�A�L��
		int32_t bv4RedEndPoint[3];
		//CIEXYZ�J���[���f���̗ΐ����̍��W(x, y, z)�Bbv4CSType��0�̏ꍇ�A�L��
		int32_t bv4GreenEndPoint[3];
		//CIEXYZ�J���[���f���̐����̍��W(x, y, z)�Bbv4CSType��0�̏ꍇ�A�L��
		int32_t bv4BlueEndPoint[3];
		//CIE��xyY�F�x��Ԃɂ�����Y�̐Ԑ�����\���B16.16�̌Œ菬���_��
		uint32_t bv4GammaRed;
		//CIE��xyY�F�x��Ԃɂ�����Y�̗ΐ�����\���B16.16�̌Œ菬���_��
		uint32_t bv4GammaGreen;
		//CIE��xyY�F�x��Ԃɂ�����Y�̐�����\���B16.16�̌Œ菬���_��
		uint32_t bv4GammaBlue;
	public:
		bool Load(IO::FileStream& file, uint32_t biSize) noexcept {
			if (!BitmapInfoInfoHeader::Load(file)) return false;
			if (biSize <= 40) return static_cast<bool>(file);
			file.ReadLE(bv4RedMask);
			file.ReadLE(bv4GreenMask);
			file.ReadLE(bv4BlueMask);
			if (biSize <= 52) return static_cast<bool>(file);
			file.ReadLE(bv4AlphaMask);
			if (biSize <= 56) return static_cast<bool>(file);
			file.ReadLE(bv4CSType);
			if (biSize <= 60) return static_cast<bool>(file);
			file.ReadLE(bv4RedEndPoint);
			file.ReadLE(bv4GreenEndPoint);
			file.ReadLE(bv4BlueEndPoint);
			if (biSize <= 96) return static_cast<bool>(file);
			file.ReadLE(bv4GammaRed);
			file.ReadLE(bv4GammaGreen);
			file.ReadLE(bv4GammaBlue);
			return static_cast<bool>(file);
		}
		void Write(IO::FileStream& file, uint32_t biSize) const noexcept {
			BitmapInfoInfoHeader::Write(file);
			if (biSize <= 40) return;
			file.WriteLE(bv4RedMask);
			file.WriteLE(bv4GreenMask);
			file.WriteLE(bv4BlueMask);
			if (biSize <= 52) return;
			file.WriteLE(bv4AlphaMask);
			if (biSize <= 56) return;
			file.WriteLE(bv4CSType);
			if (biSize <= 60) return;
			file.WriteLE(bv4RedEndPoint);
			file.WriteLE(bv4GreenEndPoint);
			file.WriteLE(bv4BlueEndPoint);
			if (biSize <= 96) return;
			file.WriteLE(bv4GammaRed);
			file.WriteLE(bv4GammaGreen);
			file.WriteLE(bv4GammaBlue);
		}
	public:
		void Init(uint32_t width, int32_t height, uint16_t bitCount, uint32_t biSize) noexcept {
			BitmapInfoInfoHeader::Init(width, height, bitCount);
			if (biSize <= 40) return;
			bv4RedMask = bitCount == 16 ? 0x00007c00 : 0x00ff0000;
			bv4GreenMask = bitCount == 16 ? 0x000003e0 : 0x0000ff00;
			bv4BlueMask = bitCount == 16 ? 0x0000001f : 0x000000ff;
			if (biSize <= 52) return;
			bv4AlphaMask = bitCount == 16 ? 0 : 0xff000000;
			if (biSize <= 56) return;
			bv4CSType = 0;
			if (biSize <= 60) return;
			bv4RedEndPoint[0] = 0;
			bv4RedEndPoint[1] = 0;
			bv4RedEndPoint[2] = 0;
			bv4GreenEndPoint[0] = 0;
			bv4GreenEndPoint[1] = 0;
			bv4GreenEndPoint[2] = 0;
			bv4BlueEndPoint[0] = 0;
			bv4BlueEndPoint[1] = 0;
			bv4BlueEndPoint[2] = 0;
			if (biSize <= 96) return;
			bv4GammaRed = 0;
			bv4GammaGreen = 0;
			bv4GammaBlue = 0;
		}
	};

	//V5�^�C�v�̏��w�b�_��\���\����
	struct BitmapV5InfoHeader : public BitmapV4InfoHeader {
		//sRGB�F��ԃ^�C�v�B
		//1: Saturation�B�}��O���t�Ɏg�p�B
		//2: Relative Colorimetric�B���S��f�U�C����Ɏg�p�B
		//4: Perceptual�B�ʐ^�╗�i��Ɏg�p�B
		//8: Absolute Colorimetric�B�v���r���[�Ɏg�p�B
		uint32_t bv5Intent;
		//���w�b�_�̐擪����F��ԃv���t�@�C���f�[�^�ւ̃I�t�Z�b�g(�o�C�g)�B
		//BitmapV4InfoHeader::bv4CStype���A
		//	"KNIL"(link)�̏ꍇ�A�v���t�@�C���f�[�^�̓t�@�C���p�X��\���k���I�[�̕�����B
		//	"DEBM"(embedded)�̏ꍇ�A�v���t�@�C���f�[�^��bv5ProfileSize(�o�C�g)�̖��ߍ��݃f�[�^�B
		uint32_t bv5ProfileData;
		//�v���t�@�C���f�[�^�̃o�C�g��
		uint32_t bv5ProfileSize;
		//�\��̈�B0�Œ�
		uint32_t bv5Reserved;
	public:
		bool Load(IO::FileStream& file, uint32_t biSize) noexcept {
			if (!BitmapV4InfoHeader::Load(file, biSize)) return false;
			if (biSize <= 108) return static_cast<bool>(file);
			file.ReadLE(bv5Intent);
			if (biSize <= 112) return static_cast<bool>(file);
			file.ReadLE(bv5ProfileData);
			file.ReadLE(bv5ProfileSize);
			if (biSize <= 120) return static_cast<bool>(file);
			file.ReadLE(bv5Reserved);
			return static_cast<bool>(file);
		}
		void Write(IO::FileStream& file, uint32_t biSize) const noexcept {
			BitmapV4InfoHeader::Write(file, biSize);
			if (biSize <= 108) return;
			file.WriteLE(bv5Intent);
			if (biSize <= 112) return;
			file.WriteLE(bv5ProfileData);
			file.WriteLE(bv5ProfileSize);
			if (biSize <= 120) return;
			file.WriteLE(bv5Reserved);
		}
	public:
		void Init(uint32_t width, int32_t height, uint16_t bitCount, uint32_t biSize) noexcept {
			BitmapV4InfoHeader::Init(width, height, bitCount, biSize);
			if (biSize <= 108) return;
			bv5Intent = 1;
			if (biSize <= 112) return;
			bv5ProfileData = 0;
			bv5ProfileSize = 0;
			if (biSize <= 120) return;
			bv5Reserved = 0;
		}
	};

	/// <summary>
	/// BMP�t�@�C���̏��w�b�_��\���\����
	/// </summary>
	struct BitmapInfoHeader {
		//���̃w�b�_�̃o�C�g��
		uint32_t biSize = 0;
		union {
			BitmapCoreInfoHeader core;
			BitmapInfoInfoHeader info;
			BitmapV4InfoHeader v4;
			BitmapV5InfoHeader v5;
		};
	public:
		/// <summary>
		/// ���w�b�_��ǂݎ��
		/// </summary>
		/// <param name="file">���݈ʒu��BitmapInfoHeader�̐擪���w���Ă���File�I�u�W�F�N�g</param>
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(biSize);
			if (biSize == 12) return core.Load(file);
			else if (biSize >= 40) return v5.Load(file, biSize);
			else return false;
		}
		/// <summary>
		/// File�I�u�W�F�N�g�̌��݈ʒu�ɏ��w�b�_����������
		/// </summary>
		/// <param name="file">�������ݐ��File�I�u�W�F�N�g</param>
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(biSize);
			if (biSize == 12) core.Write(file);
			else if (biSize >= 40) v5.Write(file, biSize);
		}
	public:
		/// <summary>
		/// ���w�b�_�T�C�Y�Ɖ摜�f�[�^�̕��A�����A�s�N�Z�����Ƃ̃r�b�g�����w�肵�ď���������
		/// </summary>
		/// <param name="infoHeaderSize">
		/// ���w�b�_�̃T�C�Y�B
		/// 12, 40, 52, 56, 60, 96, 108, 112, 120, 124�̂����ꂩ���w�肷��B
		/// </param>
		/// <param name="width">�摜�f�[�^�̕�(px)�B0�ȊO�̒l���w�肷��</param>
 		/// <param name="height">�摜�f�[�^�̍���(px)�B0�ȊO�̒l���w�肷��</param>
		/// <param name="bitCount">
		/// �s�N�Z�����Ƃ̃r�b�g���B
		/// 0, 1, 4, 8, 16, 24, 32�̂����ꂩ���w�肷��
		/// </param>
		void Init(uint32_t infoHeaderSize, uint32_t width, int32_t height, uint16_t bitCount) noexcept {
			biSize = infoHeaderSize;
			if (biSize == 12) core.Init(static_cast<uint16_t>(width), static_cast<uint16_t>(height), bitCount);
			else if (biSize >= 40) v5.Init(width, height, bitCount, biSize);
		}
	public:
		/// <summary>
		/// ���w�b�_����ABMP�t�@�C���Ƀr�b�g�t�B�[���h���܂܂�邩���ׂ�
		/// </summary>
		bool ExistsBitField() const noexcept {
			return biSize == 40
				&& (info.biBitCount == 16 || info.biBitCount == 32)
				&& info.biCompression == 3;
		}
		/// <summary>
		/// ���w�b�_����ABMP�t�@�C���ɃJ���[�p���b�g���܂܂�邩���ׂ�
		/// </summary>
		bool ExistsPalette() const noexcept {
			if (biSize != 12 && biSize < 40) return false;
			switch (biSize == 12 ? core.bcBitCount : info.biBitCount) {
			case 1:
			case 4:
			case 8:
				return true;
			default:
				return biSize == 12 ? false : info.biClrUsed;
			}
		}
		/// <summary>
		/// BMP�t�@�C���Ɋ܂܂��J���[�p���b�g�����擾����
		/// </summary>
		uint32_t GetPaletteCount() const noexcept {
			if (!ExistsPalette()) return 0;
			if (biSize >= 40 && info.biClrUsed) return info.biClrUsed;
			return 1ul << (biSize == 12 ? core.bcBitCount : info.biBitCount);
		}
		/// <summary>
		/// �s�N�Z�����Ƃ̃r�b�g�����擾����
		/// </summary>
		uint16_t GetBitCount() const noexcept {
			return IsCoreHeader() ? core.bcBitCount : info.biBitCount;
		}
		/// <summary>
		/// ���̏��w�b�_��CORE�^�C�v�����ׂ�
		/// </summary>
		bool IsCoreHeader() const noexcept { return biSize == 12; }
		/// <summary>
		/// �摜�f�[�^���g�b�v�_�E��(��̍s->���̍s�̏��ɕ���ł���)�����ׂ�
		/// </summary>
		bool HasTopDownImageData() const noexcept {
			return IsCoreHeader() ? false : info.biHeight < 0;
		}
		/// <summary>
		/// �摜�f�[�^��1�s���Ƃ̃o�C�g��(�p�f�B���O�܂�)���擾����
		/// </summary>
		uint32_t GetLineSize() const noexcept {
			if (biSize != 12 && biSize < 40) return 0;
			return biSize == 12 ? core.GetLineSize() : info.GetLineSize();
		}
		/// <summary>
		/// �摜�f�[�^�̕�(px)���擾����
		/// </summary>
		uint32_t GetWidth() const noexcept {
			if (biSize != 12 && biSize < 40) return 0;
			return static_cast<uint32_t>(biSize == 12 ? core.bcWidth : info.biWidth);
		}
		/// <summary>
		/// �摜�f�[�^�̍���(px)���擾����
		/// </summary>
		uint32_t GetHeight() const noexcept {
			if (biSize != 12 && biSize < 40) return 0;
			return static_cast<uint32_t>(biSize == 12 ? core.bcHeight : System::Math::Abs(info.biHeight));
		}
		/// <summary>
		/// �摜�f�[�^�̃o�C�g��(�p�f�B���O�܂�)���擾����
		/// </summary>
		size_t GetImageDataSize() const noexcept {
			uint32_t lineSize = GetLineSize();
			return static_cast<size_t>(lineSize) * GetHeight();
		}
	};

	/// <summary>
	/// BMP�t�@�C���̃r�b�g�t�B�[���h��\���\����
	/// </summary>
	struct BitmapBitField {
		//�Ԑ����̃J���[�}�X�N�l
		uint32_t bfRedMask;
		//�ΐ����̃J���[�}�X�N�l
		uint32_t bfGreenMask;
		//�����̃J���[�}�X�N�l
		uint32_t bfBlueMask;
	public:
		/// <summary>
		/// �r�b�g�t�B�[���h��ǂݎ��
		/// </summary>
		/// <param name="file">���݈ʒu��BitmapBitField�̐擪���w���Ă���File�I�u�W�F�N�g</param>
		bool Load(IO::FileStream& file) noexcept {
			file.ReadLE(bfRedMask);
			file.ReadLE(bfGreenMask);
			file.ReadLE(bfBlueMask);
			return static_cast<bool>(file);
		}
		/// <summary>
		/// File�I�u�W�F�N�g�̌��݈ʒu�Ƀr�b�g�t�B�[���h����������
		/// </summary>
		/// <param name="file">�������ݐ��File�I�u�W�F�N�g</param>
		void Write(IO::FileStream& file) const noexcept {
			file.WriteLE(bfRedMask);
			file.WriteLE(bfGreenMask);
			file.WriteLE(bfBlueMask);
		}
	public:
		/// <summary>
		/// �ԁA�΁A�̃J���[�}�X�N���w�肵�ď���������
		/// </summary>
		/// <param name="red">�Ԑ����̃J���[�}�X�N</param>
		/// <param name="green">�ΐ����̃J���[�}�X�N</param>
		/// <param name="blue">�����̃J���[�}�X�N</param>
		void Init(uint32_t red, uint32_t green, uint32_t blue) noexcept {
			bfRedMask = red;
			bfGreenMask = green;
			bfBlueMask = blue;
		}
	};

	struct RGBTRIPLE {
		uint8_t rgbBlue;
		uint8_t rgbGreen;
		uint8_t rgbRed;
	public:
		static bool Load(IO::FileStream& file, RGBTRIPLE* triple, uint32_t paletteCount) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) file.ReadLE(&triple[i].rgbBlue, 3);
			return static_cast<bool>(file);
		}
		static void Write(IO::FileStream& file, const RGBTRIPLE* triple, uint32_t paletteCount) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) file.WriteLE(&triple[i].rgbBlue, 3);
		}
	public:
		static uint32_t GetPaletteIndex(const RGBTRIPLE* palette, uint32_t paletteCount, uint8_t red, uint8_t green, uint8_t blue) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) {
				if (palette[i].rgbBlue == blue && palette[i].rgbGreen == green && palette[i].rgbRed == red) {
					return i;
				}
			}
			return paletteCount;
		}
	public:
		constexpr operator Pixel() const noexcept { return Pixel(rgbRed, rgbGreen, rgbBlue); }
	};

	struct RGBQUAD {
		uint8_t rgbBlue;
		uint8_t rgbGreen;
		uint8_t rgbRed;
		uint8_t rgbReserved;
	public:
		static bool Load(IO::FileStream& file, RGBQUAD* quad, uint32_t paletteCount) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) file.ReadLE(&quad[i].rgbBlue, 4);
			return static_cast<bool>(file);
		}
		static void Write(IO::FileStream& file, const RGBQUAD* quad, uint32_t paletteCount) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) file.WriteLE(&quad[i].rgbBlue, 4);
		}
	public:
		static uint32_t GetPaletteIndex(const RGBQUAD* palette, uint32_t paletteCount, uint8_t red, uint8_t green, uint8_t blue) noexcept {
			for (uint32_t i = 0; i < paletteCount; ++i) {
				if (palette[i].rgbBlue == blue && palette[i].rgbGreen == green && palette[i].rgbRed == red) {
					return i;
				}
			}
			return paletteCount;
		}
	public:
		constexpr operator Pixel() const noexcept { return Pixel(rgbRed, rgbGreen, rgbBlue, rgbReserved); }
	};
}

export namespace System::Drawing {
	class BMP : public Object {
		BitmapFileHeader header;
		BitmapInfoHeader infoHeader;
		//infoHeader.biSize��40���A
		//infoHeader.info.biBitCount��16�܂���32�ŁA
		//����infoHeader.info.biCompression��3�̂Ƃ��̂ݑ��݂���
		BitmapBitField* bitField = nullptr;
		//infoHeader.(core.bcBitCount/info.biBitCount)��1, 4, 8�܂��́A
		//infoHeader.biClrUsed��1�ȏ�̂Ƃ��̂ݑ��݂���
		union {
			//infoHeader.biSize��12(CORE�^�C�v)�̂Ƃ��g�p����
			RGBTRIPLE* triple = nullptr;
			//triple�łȂ��ꍇ�A��������g�p����
			RGBQUAD* quad;
		public:
			Pixel GetPixel(bool useTriple, uint32_t paletteIndex) const noexcept {
				if (useTriple) return static_cast<Pixel>(triple[paletteIndex]);
				else return static_cast<Pixel>(quad[paletteIndex]);
			}
		} palette;
		uint8_t* data = nullptr;
	public:
		BMP() noexcept = default;
		BMP(BMP&& arg) noexcept : header(arg.header), infoHeader(arg.infoHeader), bitField(arg.bitField), data(arg.data) {
			palette.triple = arg.palette.triple;
			arg.bitField = nullptr;
			arg.palette.triple = nullptr;
			arg.data = nullptr;
		}
		explicit BMP(const String& filename) noexcept { Load(filename); }
		explicit BMP(System::IO::FileStream&& file) noexcept { Load(file); }
		~BMP() {
			delete bitField;
			bitField = nullptr;
			if (palette.triple) {
				if (infoHeader.IsCoreHeader()) delete[] palette.triple;
				else delete[] palette.quad;
			}
			palette.triple = nullptr;
			delete[] data;
			data = nullptr;
		}
	public:
		/// <summary>
		/// ���̃t�@�C����BMP�t�@�C�������ׂ�
		/// </summary>
		/// <param name="file">�o�C�i�����͂ŊJ���ꂽ�t�@�C���I�u�W�F�N�g</param>
		static bool IsBMP(System::IO::FileStream& file) noexcept {
			System::IO::StreamPos pos = file.TellPos();
			file.Seek(0, System::IO::SeekDir::Begin);
			uint8_t type[2]{};
			file.ReadLE(type);
			file.Clear();
			file.Seek(pos);
			return type[0] == 'B' && type[1] == 'M';
		}
	public:
		bool Load(const String& filePath) noexcept {
			System::IO::FileStream file{ filePath, System::IO::OpenMode::IN_BINARY };
			if (!file) return false;
			return Load(file);
		}
		bool Load(System::IO::FileStream& file) noexcept {
			Clear();
			if (!header.Load(file)) return Clear(false);
			if (!infoHeader.Load(file)) return Clear(false);
			if (infoHeader.ExistsBitField()) {
				bitField = new BitmapBitField;
				if (!bitField->Load(file)) return Clear(false);
			}
			uint32_t paletteCount = infoHeader.GetPaletteCount();
			if (paletteCount) {
				if (infoHeader.biSize == 12) {
					palette.triple = new RGBTRIPLE[paletteCount];
					if (!RGBTRIPLE::Load(file, palette.triple, paletteCount)) return Clear(false);
				}
				else {
					palette.quad = new RGBQUAD[paletteCount];
					if (!RGBQUAD::Load(file, palette.quad, paletteCount)) return Clear(false);
				}
			}
			size_t dataSize = infoHeader.GetImageDataSize();
			if (dataSize) {
				data = new uint8_t[dataSize];
				file.ReadLE(data, dataSize);
				if (!file) return Clear(false);
			}
			else return Clear(false);
			return static_cast<bool>(file);
		}
		bool Save(const String& filePath) noexcept {
			if (!data || infoHeader.GetWidth() <= 0 || infoHeader.GetHeight() == 0) return false;
			try {
				IO::FileStream file(filePath, IO::OpenMode::OUT_BINARY);
				if (!file) return false;
				header.Write(file);
				infoHeader.Write(file);
				//�r�b�g�t�B�[���h��������
				if (infoHeader.ExistsBitField() && bitField) bitField->Write(file);
				//�J���[�p���b�g��������
				uint32_t paletteCount = infoHeader.GetPaletteCount();
				if (paletteCount && palette.triple) {
					if (infoHeader.IsCoreHeader()) RGBTRIPLE::Write(file, palette.triple, paletteCount);
					else RGBQUAD::Write(file, palette.quad, paletteCount);
				}
				size_t dataSize = infoHeader.GetImageDataSize();
				file.WriteLE(data, dataSize);
			}
			catch (std::exception&) {
				return false;
			}
			return true;
		}
		bool Clear(bool result = false) {
			delete[] data;
			data = nullptr;
			if (palette.triple) {
				if (infoHeader.biSize == 12) delete[] palette.triple;
				else delete[] palette.quad;
				palette.triple = nullptr;
			}
			delete bitField;
			bitField = nullptr;
			infoHeader.Init(40, 0, 0, 0);
			header.Init(0, 0);
			return result;
		}
	private:
		void Load1bitIndexed(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			size_t widthLen = (w >> 3) + (w % 8 == 0 ? 0 : 1);
			widthLen = widthLen + (widthLen % 4 == 0 ? 0 : 4 - (widthLen % 4));
			bool useTriple = infoHeader.IsCoreHeader();
			Pixel p0 = palette.GetPixel(useTriple, 0);
			Pixel p1 = palette.GetPixel(useTriple, 1);
			for (uint32_t y = h; y-- > 0;) {
				uint32_t x = 0;
				size_t cur = y * widthLen;
				while (x < w) {
					uint8_t paletteNum = data[cur++];
					if (x + 7 < w) {
						if (paletteNum & 0x80) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x40) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x20) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x10) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x08) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x04) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x02) pixels.Add(p1);
						else pixels.Add(p0);
						if (paletteNum & 0x01) pixels.Add(p1);
						else pixels.Add(p0);
						x += 8;
					}
					else {
						do {
							if (paletteNum & 0x80) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x40) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x20) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x10) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x08) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x04) pixels.Add(p1);
							else pixels.Add(p0);
							if (++x >= w) break;
							if (paletteNum & 0x02) pixels.Add(p1);
							else pixels.Add(p0);
							++x;
						} while (false);
					}
				}
			}
		}
		void Load4bitIndexed(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			size_t widthLen = (w >> 1) + (w % 2 == 0 ? 0 : 1);
			widthLen = widthLen + (widthLen % 4 == 0 ? 0 : 4 - (widthLen % 4));
			bool useTriple = infoHeader.IsCoreHeader();
			const uint32_t end = (w >> 1) << 1;
			for (uint32_t y = h; y-- > 0;) {
				size_t cur = y * widthLen;
				for (uint32_t x = 0; x < end; x += 2) {
					uint8_t paletteNum = data[cur++];
					pixels.Add(palette.GetPixel(useTriple, (paletteNum & 0xf0u) >> 4));
					pixels.Add(palette.GetPixel(useTriple, paletteNum & 0x0fu));
				}
				if (end != w) pixels.Add(palette.GetPixel(useTriple, (data[cur] & 0xf0u) >> 4));
			}
		}
		void Load8bitIndexed(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			const size_t widthLen = w + (w % 4 == 0 ? 0 : 4 - (w % 4));
			bool useTriple = infoHeader.IsCoreHeader();
			for (uint32_t y = h; y-- > 0;) {
				size_t cur = y * widthLen;
				for (uint32_t x = 0; x < w; ++x) {
					uint8_t paletteNum = data[cur++];
					pixels.Add(palette.GetPixel(useTriple, paletteNum));
				}
			}
		}
		void Load24bitTrueColor(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			size_t widthLen = w * 3;
			widthLen = widthLen + (widthLen % 4 == 0 ? 0 : 4 - (widthLen % 4));
			for (uint32_t y = h; y-- > 0;) {
				size_t cur = y * widthLen;
				for (uint32_t x = 0; x < w; ++x) {
					pixels.Add(Pixel(data[cur + 2], data[cur + 1], data[cur]));
					cur += 3;
				}
			}
		}
		void Load32bitTrueColor(Vector<Pixel>& pixels, uint32_t w, uint32_t h) const noexcept {
			for (uint32_t y = h; y-- > 0;) {
				size_t cur = static_cast<size_t>(y) * w * 4;
				for (uint32_t x = 0; x < w; ++x) {
					pixels.Add(Pixel(data[cur + 2], data[cur + 1], data[cur], data[cur + 3]));
					cur += 4;
				}
			}
		}
	public:
		/// <summary>
		/// Image�^�I�u�W�F�N�g�𐶐�����
		/// </summary>
		Image ToImage() const noexcept {
			Vector<Pixel> pixels;
			uint32_t w = infoHeader.GetWidth();
			uint32_t h = infoHeader.GetHeight();
			size_t pixelCount = static_cast<size_t>(w) * h;
			pixels.Reserve(pixelCount);
			switch (infoHeader.GetBitCount()) {
			case 1: {
				Load1bitIndexed(pixels, w, h);
				break;
			}
			case 4: {
				Load4bitIndexed(pixels, w, h);
				break;
			}
			case 8: {
				Load8bitIndexed(pixels, w, h);
				break;
			}
			case 24: {
				Load24bitTrueColor(pixels, w, h);
				break;
			}
			case 32: {
				Load32bitTrueColor(pixels, w, h);
				break;
			}
			}
			Image ret(w, h, static_cast<Vector<Pixel>&&>(pixels));
			if (!infoHeader.HasTopDownImageData()) ret.Reverse();
			return ret;
		}
		/// <summary>
		/// Image�I�u�W�F�N�g����BMP�^�I�u�W�F�N�g�𐶐�����
		/// </summary>
		/// <param name="image">�\�[�X�ƂȂ�Image�^�ϐ��ւ�const�Q��</param>
		static BMP FromImage(const Image& image) noexcept {
			BMP ret;
			const Pixel* pixels = image.Data();
			const uint32_t width = image.Width();
			const uint32_t height = image.Height();
			ret.infoHeader.Init(40, width, height, 24);
			//�t�@�C���w�b�_14Byte�Ə��w�b�_�̃o�C�g���̍��v
			uint32_t headerSize = 14 + ret.infoHeader.biSize;
			//�摜�f�[�^�̃o�C�g��
			size_t dataSize = ret.infoHeader.GetImageDataSize();
			ret.header.Init(static_cast<uint32_t>(headerSize + dataSize), headerSize);
			ret.data = new uint8_t[dataSize];
			//1�s���Ƃ̃p�f�B���O�T�C�Y(�o�C�g)
			const uint32_t paddingSize = ret.infoHeader.GetLineSize() - (width * 3);
			size_t i = 0;	//���݂̃f�[�^�ʒu
			for (uint32_t y = height; y-- > 0;) {
				const Pixel* p = pixels + (y * static_cast<size_t>(width));
				for (uint32_t x = 0; x < width; ++x, i += 3) {
					ret.data[i] = p->b;
					ret.data[i + 1] = p->g;
					ret.data[i + 2] = p->r;
					++p;
				}
				//�p�f�B���O�T�C�Y�����f�[�^�ʒu��i�߂�
				for (uint32_t n = 0; n < paddingSize; ++n, ++i);
			}
			return ret;
		}
	public://Object�N���X�I�[�o�[���C�h
		bool Equals(const Object& obj) const noexcept override { return GetTypeID() == obj.GetTypeID() ? *this == static_cast<const BMP&>(obj) : false; }
		Type GetType() const noexcept override { return Type(u"System::Drawing::BMP"); }
		String ToString() const noexcept override { return String(u"Bitmap Image"); }
		uint32_t GetTypeID() const noexcept override { return GetID<BMP>(); }
	};
}