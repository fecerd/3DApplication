module Image;
import BMP;
import PNG;
import TGA;

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