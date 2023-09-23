export module Resource1;
import System;
import IO;
export import ResourcePath;
using namespace System;

export namespace ResourcePaths::Font {
	inline constexpr CString SourceHanSans = uR"(.\Font\SourceHanSans-Medium.otf)";
	inline constexpr CString Century = uR"(.\Font\century.ttf)";
}

export namespace ResourcePaths::Model {
	inline constexpr CString Miku = uR"(.\Model\初音ミク.pmd)";
	inline constexpr CString Rin = uR"(.\Model\鏡音リン.pmd)";
	inline constexpr CString ClassRoom = uR"(.\Model\Classroom\classroom.pmd)";
	inline constexpr CString Desk = uR"(.\Model\Classroom\desk.pmd)";
	inline constexpr CString UnityChan = uR"(.\Model\UnityChan\Models\unitychan.pmd)";
}

export namespace ResourcePaths::Motion {
	inline constexpr CString Dance = uR"(.\Motion\dance2.vmd)";
	inline constexpr CString Walk = uR"(.\Motion\FeminineWalk.vmd)";
}

export namespace ResourcePaths::Audio {
//	inline constexpr CString Field = uR"(.\Audio\MusMus-BGM-154.mp3)";
	inline constexpr CString Lesson = uR"(.\Audio\01 レッスン.mp3)";
	inline constexpr CString Corrent1 = uR"(.\Audio\correct_answer1.mp3)";
	inline constexpr CString Corrent2 = uR"(.\Audio\correct_answer2.mp3)";
	inline constexpr CString Corrent3 = uR"(.\Audio\correct_answer3.mp3)";
	inline constexpr CString BGM1 = u"./Audio/昼下がり気分.mp3";
	inline constexpr CString Walking = u"./Audio/Se_Walking_School.mp3";
}

export namespace ResourcePaths::Video {
	inline constexpr CString Waterfall = uR"(.\Video\waterfall.mp4)";
	inline constexpr CString Test2 = uR"(.\Video\test2.mp4)";
	inline constexpr CString Sea = uR"(.\Video\sea.mp4)";
	inline constexpr CString CountDown = u"./Video/count_down.mp4";
}
