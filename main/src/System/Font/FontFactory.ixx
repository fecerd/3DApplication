module;
#include "../../Headers/EnableVirtual.hpp"
export module FontFactory;
export import IFont;
import Objects;
import Function;
import HashMap;
import OpenType;
import SmartPtrs;

#if defined(NO_VIRTUAL)

export namespace System {
	class FontFactory {
	public:
		static bool SetFont(const String& filePath, const String& fontName) noexcept {
			return false;
		}
		static IFont* GetFont(const String& fontName) noexcept {
			return nullptr;
		}
	};
}

#else

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
		inline static Internal::FontMap fonts = Internal::FontMap(3);
	public:
		static bool SetFont(const String& filePath, const String& fontName) noexcept {
			OpenType* font = new OpenType();
			if (!font->Load(filePath)) {
				delete font;
				return false;
			}
			return fonts.Insert(fontName, font);
		}
		static IFont* GetFont(const String& fontName) noexcept {
			return fonts.At(fontName);
		}
	};
}

#endif
