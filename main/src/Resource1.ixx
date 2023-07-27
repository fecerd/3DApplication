export module Resource1;
import System;
import IO;
export import ResourcePath;
using namespace System;
using namespace System::IO;

export namespace ResourcePaths::Font {
	constexpr CString SourceHanSans = uR"(.\Font\SourceHanSans-Medium.otf)";
	constexpr CString Century = uR"(.\Font\century.ttf)";
}

export namespace ResourcePaths::Model {
	constexpr CString Miku = uR"(.\Model\初音ミク.pmd)";
	constexpr CString Rin = uR"(.\Model\鏡音リン.pmd)";
	constexpr CString ClassRoom = uR"(.\Model\Classroom\classroom.pmd)";
	constexpr CString Desk = uR"(.\Model\Classroom\desk.pmd)";
	constexpr CString UnityChan = uR"(.\Model\UnityChan\Models\unitychan.pmd)";
}

export namespace ResourcePaths::Motion {
	constexpr CString Dance = uR"(.\Motion\dance2.vmd)";
	constexpr CString Walk = uR"(.\Motion\FeminineWalk.vmd)";
}

export namespace ResourcePaths::Audio {
	constexpr CString Field = uR"(.\Audio\MusMus-BGM-154.mp3)";
	constexpr CString Lesson = uR"(.\Audio\01 レッスン.mp3)";
	constexpr CString Corrent1 = uR"(.\Audio\correct_answer1.mp3)";
	constexpr CString Corrent2 = uR"(.\Audio\correct_answer2.mp3)";
	constexpr CString Corrent3 = uR"(.\Audio\correct_answer3.mp3)";
}

export namespace ResourcePaths::Video {
	constexpr CString Waterfall = uR"(.\Video\waterfall.mp4)";
	constexpr CString Test2 = uR"(.\Video\test2.mp4)";
	constexpr CString Sea = uR"(.\Video\sea.mp4)";
}