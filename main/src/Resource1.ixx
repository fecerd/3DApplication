export module Resource1;
import System;
import IO;
export import ResourcePath;
using namespace System;

export namespace ResourcePaths::Font {
	inline constexpr CString SourceHanSans = u"./Font/SourceHanSans-Medium.otf";
	inline constexpr CString Century = u"./Font/century.ttf";
}

export namespace ResourcePaths::Model {
	inline constexpr CString Miku = u"./Model/初音ミク.pmd";
	inline constexpr CString Rin = u"./Model/鏡音リン.pmd";
	inline constexpr CString Classroom = u"./Model/Classroom/classroom.pmd";
	inline constexpr CString Desk = u"./Model/Classroom/desk.pmd";
	inline constexpr CString UnityChan = u"./Model/UnityChan/Models/unitychan.pmd";
	inline constexpr CString Field = u"./Model/Field1/道_木なし.pmd";
}

export namespace ResourcePaths::Motion {
	inline constexpr CString Dance = u"./Motion/dance2.vmd";
	inline constexpr CString Walk = u"./Motion/FeminineWalk.vmd";
	namespace Moves {
		inline constexpr CString StopToWalk = u"./Motion/Moves/stop_to_walk.vmd";
		inline constexpr CString Walking = u"./Motion/Moves/walking.vmd";
		inline constexpr CString WalkToStop = u"./Motion/Moves/walk_to_stop.vmd";
		inline constexpr CString StopToDash = u"./Motion/Moves/stop_to_dash.vmd";
		inline constexpr CString Dash = u"./Motion/Moves/dash.vmd";
		inline constexpr CString DashToStop = u"./Motion/Moves/dash_to_stop.vmd";
		inline constexpr CString DashToWalk = u"./Motion/Moves/dash_to_walk.vmd";
		inline constexpr CString WalkToDash = u"./Motion/Moves/walk_to_dash.vmd";
		inline constexpr CString Stopping = u"./Motion/Moves/stopping.vmd";
		inline constexpr CString WalkToJump = u"./Motion/Moves/walk_to_jump.vmd";
		inline constexpr CString Jumping = u"./Motion/Moves/jumping.vmd";
		inline constexpr CString JumpToFall = u"./Motion/Moves/jump_to_fall.vmd";
		inline constexpr CString Falling = u"./Motion/Moves/falling.vmd";
		inline constexpr CString FallToWalk = u"./Motion/Moves/fall_to_walk.vmd";
	}
}

export namespace ResourcePaths::Audio {
	inline constexpr CString BGM1 = u"./Audio/昼下がり気分.mp3";
}

export namespace ResourcePaths::SE {
	inline constexpr CString Walking = u"./SE/walking_school.mp3";
	inline constexpr CString Correct1 = u"./Audio/correct_answer1.mp3";
	inline constexpr CString Correct2 = u"./Audio/correct_answer2.mp3";
	inline constexpr CString Correct3 = u"./Audio/correct_answer3.mp3";
}

export namespace ResourcePaths::Video {
	inline constexpr CString CountDown = u"./Video/count_down.mp4";
}
