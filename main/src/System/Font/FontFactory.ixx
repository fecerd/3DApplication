export module FontFactory;
export import IFont;
import Objects;
import HashMap;
import OpenType;

export namespace System {
	/// <summary>
	/// �t�H���g���Ǘ�����t�@�N�g���N���X
	/// </summary>
	class FontFactory {
		inline static HashMap<String, IFont*> fonts = 3;
	public:
		static bool SetFont(const String& filePath, const String& fontName) noexcept {
			OpenType* font = new OpenType();
			if (!font->Load(filePath)) {
				delete font;
				return false;
			}
			if (!fonts.IsInitializedDestructor()) {
				fonts.SetDestructor(
					[](HashMap<String, IFont*>& map) {
						for (IFont* x : map.Values()) delete x;
					}
				);
			}
			return fonts.Insert(fontName, font);
		}
		static IFont* GetFont(const String& fontName) noexcept {
			return fonts.At(fontName);
		}
	};
}