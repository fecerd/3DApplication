export module FontFactory;
export import Module;
export import IFont;

export import OpenType;
import Encoding;

namespace System::Internal {
	class FontMap : public HashMap<String, IFont*> {
	public:
		using HashMap<String, IFont*>::HashMap;
		~FontMap() noexcept {
			for (IFont* x : Values()) delete x;
		}
	};
}

export namespace System {
	/// <summary>
	/// フォントを管理するファクトリクラス
	/// </summary>
	class FontFactory {
		HashMap<String, IFont*> fonts = 3;
	public:
		~FontFactory() noexcept {
			for (IFont* x : fonts.Values()) {
				delete x;
			}
		}
	public:
		bool SetFont(const String& filePath, const String& fontName) noexcept {
			OpenType* font = new OpenType();
			if (!font->Load(filePath)) {
				IFont* tmp = font;
				font = nullptr;
				delete tmp;
				return false;
			}
			return fonts.Insert(fontName, font);
		}
		IFont* GetFont(const String& fontName) noexcept {
			return fonts.At(fontName);
		}
	public:
		static ManagedObject<FontFactory> GetFontFactory() noexcept {
			static ManagedObject<FontFactory> ret;
			if (!ret) {
				ret = ManagedObject<FontFactory>(new FontFactory());
			}
			return ret;
		}
	};
}
