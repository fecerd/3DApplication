import System;
import Resource1;
using namespace System;

bool font_test() noexcept {
	ManagedObject<FontFactory> factory = FontFactory::GetFontFactory();
	bool success = factory->SetFont(ResourcePaths::GetPathName(ResourcePaths::Font::SourceHanSans), u"Century");
	System::IFont* century = factory->GetFont(u"Century");
	if (!century) return false;
	century->SetBaseLineColor(Drawing::Colors::White);
	century->SetContoursColor(Drawing::Colors::Black);
	Drawing::Image image(1920, 1080);
	uint32_t permilli = 50;
	int32_t originy = century->GetAscenderLine(permilli);
	if (originy < 0) originy = 0;
	for (bool b : century->UpdateStringImage(image, u"Hello World!", Point<int32_t>(0, originy), permilli)) {}
	return Drawing::BMP::FromImage(image).Save(ResourcePaths::ExePath.CreatePath(u"./Hello_World.bmp").PathName());
}

int main(int argc, char** argv) {
	//リソースパスのルートを設定
	ResourcePaths::RootPath = String(u"C:/source/vscode/3DApplication/main/Resources");
	ResourcePaths::ExePath = String(argv[0]);

	font_test();

	return 0;
}