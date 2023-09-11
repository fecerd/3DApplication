#if defined(__GNUC__) && !defined(__clang__)
import Image;
#else
module Image;
#endif
import Objects;
import File;
import PNG;
import TGA;
import BMP;

namespace System::Drawing {
	Image::Image(const System::String& filePath) noexcept {
		System::IO::FileStream file(filePath, System::IO::OpenMode::IN_BINARY);
		if (System::Drawing::BMP::IsBMP(file)) *this = System::Drawing::BMP(static_cast<System::IO::FileStream&&>(file)).ToImage();
		else if (System::Drawing::PNG::IsPNG(file)) *this = System::Drawing::PNG(static_cast<System::IO::FileStream&&>(file)).ToImage();
		else if (System::Drawing::TGA::IsTGA(file)) *this = System::Drawing::TGA(static_cast<System::IO::FileStream&&>(file)).ToImage();
	}

	Image Image::FromFile(const String& filePath) noexcept {
		System::IO::FileStream file(filePath, System::IO::OpenMode::IN_BINARY);
		if (System::Drawing::BMP::IsBMP(file)) return System::Drawing::BMP(static_cast<System::IO::FileStream&&>(file)).ToImage();
		else if (System::Drawing::PNG::IsPNG(file)) return System::Drawing::PNG(static_cast<System::IO::FileStream&&>(file)).ToImage();
		else if (System::Drawing::TGA::IsTGA(file)) return System::Drawing::TGA(static_cast<System::IO::FileStream&&>(file)).ToImage();
		else return Image();
	}
}
