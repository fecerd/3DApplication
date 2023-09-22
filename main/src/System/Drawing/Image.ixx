export module Image;
import Allocator;
import Objects;
import VectorBase;
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
			Allocator<Pixel> alloc;
			const size_t size = static_cast<size_t>(width) * height;
			data = alloc.allocate(size);
			for (size_t i = 0; i < size; ++i) AllocatorTraits<Allocator<Pixel>>::ConstructDefault(alloc, data + i);
			//data = new Pixel[static_cast<size_t>(width) * height]{};
		}
		Image(uint32_t width, uint32_t height, VectorBase<Pixel>&& pixels) noexcept : width(width), height(height), data(pixels.Release()) {}
		Image(uint32_t width, uint32_t height, Pixel* pixels) noexcept : width(width), height(height) {
			Allocator<Pixel> alloc;
			const size_t size = static_cast<size_t>(width) * height;
			data = alloc.allocate(size);
			for (size_t i = 0; i < size; ++i) AllocatorTraits<Allocator<Pixel>>::Construct(alloc, data + i, pixels[i]);
			//data = new Pixel[size];
			//for (size_t i = 0; i < size; ++i) this->data[i] = data[i];
		}
		Image(const Image& arg) noexcept : Image(arg.width, arg.height, arg.data) {}
		Image(Image&& arg) noexcept : width(arg.width), height(arg.height), data(arg.data) {
			arg.data = nullptr;
			arg.width = 0;
			arg.height = 0;
		}
		Image(const String& filePath) noexcept;
		~Image() noexcept {
			Reset();
		}
	private:
		void Reset() noexcept {
			if (data) {
				Allocator<Pixel> alloc;
				const size_t size = static_cast<size_t>(width) * height;
				for (size_t i = size; i-- > 0;) AllocatorTraits<Allocator<Pixel>>::Destroy(alloc, data + i);
				alloc.deallocate(data, size);
			}
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
			return Image(width, height);
		}
		static Image CreateSingleColorImage(uint32_t width, uint32_t height, Pixel pixel) noexcept {
			Image ret;
			ret.width = width;
			ret.height = height;

			Allocator<Pixel> alloc;
			const size_t size = ret.Size();
			ret.data = alloc.allocate(size);
			for (size_t i = 0; i < size; ++i) AllocatorTraits<Allocator<Pixel>>::Construct(alloc, ret.data + i, pixel);

			//size_t size = ret.Size();
			//ret.data = new Pixel[size];
			//for (size_t i = 0; i < size; ++i) ret.data[i] = pixel;
			return ret;
		}
		static Image CreateToonImage() noexcept {
			Image ret;
			ret.width = 1;
			ret.height = 256;

			Allocator<Pixel> alloc;
			const size_t size = ret.Size();
			ret.data = alloc.allocate(size);
			for (uint32_t i = 0; i < ret.height; ++i) {
				AllocatorTraits<Allocator<Pixel>>::Construct(alloc, ret.data + (255 - i), Pixel(i, i, i, 255));
			}

			// size_t size = ret.Size();
			// ret.data = new Pixel[size];
			// uint8_t i = 0;
			// do {
			// 	ret.data[255 - i] = Pixel(i, i, i, 255);
			// } while (++i);
			return ret;
		}
		static Image FromFile(const String& filePath) noexcept;
	public:
		/// <summary>
		/// 画像を上下反転する
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
		/// すべてのピクセルを塗りつぶす
		/// </summary>
		/// <param name="pixel">塗りつぶす色</param>
		void Fill(Pixel pixel) noexcept { for (size_t i = 0, size = Size(); i < size; ++i) data[i] = pixel; }
		/// <summary>
		/// (x, y)のピクセルの色を反転する
		/// </summary>
		/// <param name="x">反転するピクセルのx座標</param>
		/// <param name="y">反転するピクセルのy座標</param>
		void InversionPixel(uint32_t x, uint32_t y) noexcept { if (x < width && y < height) data[x + width * y].Inversion(); }
		/// <summary>
		/// (x, y)から(width - 1, y)までのピクセルの色を反転する
		/// </summary>
		/// <param name="x">始点のx座標</param>
		/// <param name="y">反転する行のy座標</param>
		void InversionPixelHorizontal(uint32_t x, uint32_t y) noexcept {
			if (x < width && y < height) {
				Pixel* p = data + (width * y);
				for (; x < width; ++x) p[x].Inversion();
			}
		}
		/// <summary>
		/// 特定の色を持つピクセルの色を変更する
		/// </summary>
		/// <param name="from">変更する色</param>
		/// <param name="to">変更後の色</param>
		void ChangePixelColor(Pixel from, Pixel to) noexcept {
			for (size_t i = 0, size = Size(); i < size; ++i) if (data[i] == from) data[i] = to;
		}
		/// <summary>
		/// 特定のアルファ値を持つピクセルのアルファ値を変更する
		/// </summary>
		/// <param name="from">変更するアルファ値</param>
		/// <param name="to">変更後のアルファ値</param>
		void ChangePixelAlpha(uint8_t from, uint8_t to) noexcept {
			for (size_t i = 0, size = Size(); i < size; ++i) if (data[i].a == from) data[i].a = to;
		}
		/// <summary>
		/// (x, y)のピクセルに色を設定する
		/// </summary>
		/// <param name="x">設定するピクセルのx座標</param>
		/// <param name="y">設定するピクセルのy座標</param>
		/// <param name="pixel">設定する色</param>
		void SetPixel(uint32_t x, uint32_t y, Pixel pixel) noexcept {
			if (x < width && y < height) data[x + static_cast<size_t>(width) * y] = pixel;
		}
		/// <summary>
		/// (x, y)のピクセルを取得する
		/// </summary>
		/// <param name="x">取得するピクセルのx座標</param>
		/// <param name="y">取得するピクセルのy座標</param>
		/// <returns>(x, y)のピクセル値。範囲外の場合、Pixel()を返す</returns>
		Pixel GetPixel(uint32_t x, uint32_t y) const noexcept {
			if (x < width && y < height) return data[x + static_cast<size_t>(width) * y];
			else return Pixel();
		}
		/// <summary>
		/// 他のImageオブジェクトを貼り付ける。
		/// 指定した画像がこの画像の範囲に収まらない場合、範囲内のみ貼り付けされる
		/// </summary>
		/// <param name="src">貼り付ける画像</param>
		/// <param name="x">x座標のオフセット。この画像上のx座標を指定する</param>
		/// <param name="y">y座標のオフセット。この画像上のy座標を指定する</param>
		/// <param name="background">
		/// 画像の背景色。
		/// 貼り付ける画像内にあるこの色のピクセルは透明になる(貼り付けされない)
		/// </param>
		/// <returns>オフセット値がこの画像の範囲外の場合、false</returns>
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
			*this = Image(rhs);
			//delete[] data;
			// width = rhs.width;
			// height = rhs.height;
			// const size_t size = Size();
			// data = new Pixel[size];
			// for (size_t i = 0; i < size; ++i) data[i] = rhs.data[i];
			return *this;
		}
		Image& operator=(Image&& rhs) noexcept {
			if (this == &rhs) return *this;
			Reset();
			//delete[] data;
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
