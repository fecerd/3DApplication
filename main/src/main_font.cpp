import System;
import Resource1;
using namespace System;

bool font_test(const String& str, const String& fontpath, const String& savePath) noexcept {
	ManagedObject<FontFactory> factory = FontFactory::GetFontFactory();
	bool success = factory->SetFont(fontpath, fontpath);
	System::IFont* pfont = factory->GetFont(fontpath);
	if (!pfont) return false;
	pfont->SetBaseLineColor(Drawing::Colors::White);
	pfont->SetContoursColor(Drawing::Colors::Black);
	Drawing::Image image(1920, 1080);
	uint32_t width = pfont->GetWidth(String(str.c_str(), str.find('\n')), 1000);
	uint32_t permilli = static_cast<uint32_t>(image.Width() / static_cast<float>(width) * 1000);

	int32_t originy = pfont->GetAscenderLine(permilli);
	if (originy < 0) originy = 0;
	for (bool b : pfont->UpdateStringImage(image, str, Point<int32_t>(0, originy), permilli)) {}
	return Drawing::BMP::FromImage(image).Save(savePath);
}

int main(int argc, char** argv) {
	//リソースパスのルートを設定
	ResourcePaths::RootPath = String(u"C:/source/vscode/3DApplication/main/Resources");
	ResourcePaths::ExePath = String(argv[0]);


	String str = u"こんにちは世界！\nHello World!";

	font_test(
		str,
		ResourcePaths::GetPathName(ResourcePaths::Font::SourceHanSans),
		ResourcePaths::ExePath.CreatePath(u"./Hello_World1.bmp").PathName()
	);
	font_test(
		str,
		ResourcePaths::GetPathName(ResourcePaths::Font::Century),
		ResourcePaths::ExePath.CreatePath(u"./Hello_World2.bmp").PathName()
	);

	return 0;
}