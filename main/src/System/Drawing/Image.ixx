export module Image;
import Objects;
import Vector;
import Memory;
import File;
export import Pixel;

export namespace System::Drawing {
	class Image : public Object {
	protected:
		uint32_t width = 0;
		uint32_t height = 0;
		Pixel* data = nullptr;
	public:
		Image() = default;
		Image(uint32_t width, uint32_t height) noexcept : width(width), height(height) {
			data = new Pixel[static_cast<size_t>(width) * height]{};
		}
		Image(uint32_t width, uint32_t height, Vector<Pixel>&& pixels) noexcept : width(width), height(height), data(pixels.Release()) {}
		Image(uint32_t width, uint32_t height, Pixel* data) noexcept : width(width), height(height) {
			size_t size = static_cast<size_t>(width) * height;
			this->data = new Pixel[size];
			for (size_t i = 0; i < size; ++i) this->data[i] = data[i];
		}
		Image(const Image& arg) noexcept : width(arg.width), height(arg.height) {
			size_t size = static_cast<size_t>(width) * height;
			data = new Pixel[size];
			for (size_t i = 0; i < size; ++i) data[i] = arg.data[i];
		}
		Image(Image&& arg) noexcept : width(arg.width), height(arg.height), data(arg.data) {
			arg.data = nullptr;
			arg.width = 0;
			arg.height = 0;
		}
		Image(const String& filePath) noexcept;
		~Image() noexcept {
			delete[] data;
			data = nullptr;
			width = 0;
			height = 0;
		}
	public:
		constexpr bool Enabled() const noexcept { return static_cast<bool>(*this); }
		Pixel* Data() noexcept { return data; }
		const Pixel* Data() const noexcept { return data; }
		uint32_t Width() const noexcept { return width; }
		uint32_t Height() const noexcept { return height; }
		size_t Size() const noexcept { return static_cast<size_t>(width) * height; }
	public:
		static Image CreateImage(uint32_t width, uint32_t height) noexcept {
			Image ret;
			ret.width = width;
			ret.height = height;
			ret.data = new Pixel[ret.Size()]{};
			return ret;
		}
		static Image CreateSingleColorImage(uint32_t width, uint32_t height, Pixel pixel) noexcept {
			Image ret;
			ret.width = width;
			ret.height = height;
			size_t size = ret.Size();
			ret.data = new Pixel[size];
			for (size_t i = 0; i < size; ++i) ret.data[i] = pixel;
			return ret;
		}
		static Image CreateToonImage() noexcept {
			Image ret;
			ret.width = 1;
			ret.height = 256;
			size_t size = ret.Size();
			ret.data = new Pixel[size];
			uint8_t i = 0;
			do {
				ret.data[255 - i] = Pixel(i, i, i, 255);
			} while (++i);
			return ret;
		}
		static Image FromFile(const String& filePath) noexcept;
	public:
		/// <summary>
		/// �摜���㉺���]����
		/// </summary>
		void Reverse() noexcept {
			uint32_t end = height / 2;
			for (uint32_t y = 0; y < end; ++y) {
				Pixel* p1 = &data[y * width];
				Pixel* p2 = &data[(height - 1 - y) * width];
				for (uint32_t x = 0; x < width; ++x, ++p1, ++p2) Memory::Swap(p1, p2);
			}
		}
		/// <summary>
		/// ���ׂẴs�N�Z����h��Ԃ�
		/// </summary>
		/// <param name="pixel">�h��Ԃ��F</param>
		void Fill(Pixel pixel) noexcept { for (size_t i = 0, size = Size(); i < size; ++i) data[i] = pixel; }
		/// <summary>
		/// (x, y)�̃s�N�Z���̐F�𔽓]����
		/// </summary>
		/// <param name="x">���]����s�N�Z����x���W</param>
		/// <param name="y">���]����s�N�Z����y���W</param>
		void InversionPixel(uint32_t x, uint32_t y) noexcept { if (x < width && y < height) data[x + width * y].Inversion(); }
		/// <summary>
		/// (x, y)����(width - 1, y)�܂ł̃s�N�Z���̐F�𔽓]����
		/// </summary>
		/// <param name="x">�n�_��x���W</param>
		/// <param name="y">���]����s��y���W</param>
		void InversionPixelHorizontal(uint32_t x, uint32_t y) noexcept {
			if (x < width && y < height) {
				Pixel* p = data + (width * y);
				for (; x < width; ++x) p[x].Inversion();
			}
		}
		/// <summary>
		/// ����̐F�����s�N�Z���̐F��ύX����
		/// </summary>
		/// <param name="from">�ύX����F</param>
		/// <param name="to">�ύX��̐F</param>
		void ChangePixelColor(Pixel from, Pixel to) noexcept {
			for (size_t i = 0, size = Size(); i < size; ++i) if (data[i] == from) data[i] = to;
		}
		/// <summary>
		/// ����̃A���t�@�l�����s�N�Z���̃A���t�@�l��ύX����
		/// </summary>
		/// <param name="from">�ύX����A���t�@�l</param>
		/// <param name="to">�ύX��̃A���t�@�l</param>
		void ChangePixelAlpha(uint8_t from, uint8_t to) noexcept {
			for (size_t i = 0, size = Size(); i < size; ++i) if (data[i].a = from) data[i].a = to;
		}
		/// <summary>
		/// (x, y)�̃s�N�Z���ɐF��ݒ肷��
		/// </summary>
		/// <param name="x">�ݒ肷��s�N�Z����x���W</param>
		/// <param name="y">�ݒ肷��s�N�Z����y���W</param>
		/// <param name="pixel">�ݒ肷��F</param>
		void SetPixel(uint32_t x, uint32_t y, Pixel pixel) noexcept {
			if (x < width && y < height) data[x + static_cast<size_t>(width) * y] = pixel;
		}
		/// <summary>
		/// (x, y)�̃s�N�Z�����擾����
		/// </summary>
		/// <param name="x">�擾����s�N�Z����x���W</param>
		/// <param name="y">�擾����s�N�Z����y���W</param>
		/// <returns>(x, y)�̃s�N�Z���l�B�͈͊O�̏ꍇ�APixel()��Ԃ�</returns>
		Pixel GetPixel(uint32_t x, uint32_t y) const noexcept {
			if (x < width && y < height) return data[x + static_cast<size_t>(width) * y];
			else return Pixel();
		}
		/// <summary>
		/// ����Image�I�u�W�F�N�g��\��t����B
		/// �w�肵���摜�����̉摜�͈̔͂Ɏ��܂�Ȃ��ꍇ�A�͈͓��̂ݓ\��t�������
		/// </summary>
		/// <param name="src">�\��t����摜</param>
		/// <param name="x">x���W�̃I�t�Z�b�g�B���̉摜���x���W���w�肷��</param>
		/// <param name="y">y���W�̃I�t�Z�b�g�B���̉摜���y���W���w�肷��</param>
		/// <param name="background">
		/// �摜�̔w�i�F�B
		/// �\��t����摜���ɂ��邱�̐F�̃s�N�Z���͓����ɂȂ�(�\��t������Ȃ�)
		/// </param>
		/// <returns>�I�t�Z�b�g�l�����̉摜�͈̔͊O�̏ꍇ�Afalse</returns>
		bool Paste(const Image& src, uint32_t x, uint32_t y, Pixel background = Pixels::White) noexcept {
			const uint32_t srcWidth = src.Width();
			const uint32_t srcHeight = src.Height();
			const uint32_t dstWidth = Width();
			const uint32_t dstHeight = Height();
			if (x >= dstWidth || y >= dstHeight) return false;
			uint32_t widthRange = x + srcWidth < dstWidth ? srcWidth : dstWidth - x - 1;
			uint32_t heightRange = y + srcHeight < dstHeight ? srcHeight : dstHeight - y - 1;
			Pixel* dp = Data();
			const Pixel* sp = src.Data();
			for (uint32_t ty = 0; ty < heightRange; ++ty) {
				Pixel* d0 = dp + (x + (ty + y) * dstWidth);
				const Pixel* s0 = sp + (ty * srcWidth);
				for (uint32_t tx = 0; tx < widthRange; ++tx) {
					if (!s0[tx].EqualColor(background)) d0[tx] = s0[tx];
				}
			}
			return true;
		}
	public:
		Type GetType() const noexcept { return Type::CreateType<Image>(); }
		String ToString() const noexcept {
			return String::Joint(u"Image Object { width: ", width, u", height: ", height, u" }");
		}
		uint32_t GetTypeID() const noexcept { return GetID<Image>(); }
	public:
		Pixel& operator[](size_t index) { return data[index]; }
		const Pixel& operator[](size_t index) const { return data[index]; }
		Image& operator=(const Image& rhs) noexcept {
			if (this == &rhs) return *this;
			delete[] data;
			width = rhs.width;
			height = rhs.height;
			size_t size = static_cast<size_t>(width) * height;
			data = new Pixel[size];
			for (size_t i = 0; i < size; ++i) data[i] = rhs.data[i];
			return *this;
		}
		Image& operator=(Image&& rhs) noexcept {
			if (this == &rhs) return *this;
			delete[] data;
			width = rhs.width;
			height = rhs.height;
			data = rhs.data;
			rhs.width = 0;
			rhs.height = 0;
			rhs.data = nullptr;
			return *this;
		}
		explicit constexpr operator bool() const noexcept {
			return data && width != 0 && height != 0;
		}
	};
}