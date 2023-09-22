export module Time;
import CSTDINT;
import Traits;
import Objects;
import EventHandler;
import <compare>;
import <chrono>;	//std::chrono
using namespace std::chrono;

export namespace System {
	template<class T>
	concept Duration = Traits::is_any_of_v<T, years, months, weeks, days, hours, minutes, seconds, milliseconds, nanoseconds>;
	using seconds = std::chrono::seconds;
	using milliseconds = std::chrono::milliseconds;
	using nanoseconds = std::chrono::nanoseconds;
	template<class T>
	concept Seconds = System::Traits::is_any_of_v<T, seconds, milliseconds, nanoseconds>;
}

export namespace System {
	class Timer {
		std::chrono::steady_clock::time_point m_start;
		std::chrono::steady_clock::time_point m_scaledCurrent;
		std::chrono::steady_clock::time_point m_unscaledCurrent;
		nanoseconds m_scaledDelta;
		nanoseconds m_unscaledDelta;
		uint64_t m_frameCount = 0;
		nanoseconds m_fixedDeltaMod = nanoseconds(0);
		uint64_t m_fixedUpdateCount = 0;
	private:
		float m_scale = 1.f;
		nanoseconds m_fixedTimeStep = nanoseconds(milliseconds(20));
	public:
		System::EventHandler<void> Invoke;
		System::EventHandler<void> FixedInvoke;
	public:
		Timer() noexcept = default;
	public:
		/// <summary>
		/// タイマーを初期化する
		/// </summary>
		void Start() noexcept;
		/// <summary>
		/// 時刻を更新し、登録されたイベントを呼び出す。
		/// イベントの呼び出し回数はスケールの影響を受ける
		/// </summary>
		void Update(bool callInvoke = true) noexcept;
	public:
		/// <summary>
		/// 現在の時間経過のスケールを取得する
		/// </summary>
		float TimeScale() const noexcept { return m_scale; }
		/// <summary>
		/// 時間経過のスケールを設定する。
		/// DeltaTime関数とUpdate関数に影響する
		/// </summary>
		/// <param name="scale">新たに設定するスケール</param>
		void TimeScale(float scale) noexcept { m_scale = scale; }
		/// <summary>
		/// 現在のFixedUpdateイベントの呼び出し間隔を取得する
		/// </summary>
		/// <typeparam name="T">単位</typeparam>
		template<Duration T = milliseconds>
		uint64_t FixedTimeStep() const noexcept {
			return static_cast<uint64_t>(duration_cast<T>(m_fixedTimeStep).count());
		}
		/// <summary>
		/// FixedUpdateイベントの呼び出し間隔を設定する
		/// </summary>
		/// <typeparam name="T">単位</typeparam>
		/// <param name="n">呼び出し間隔</param>
		template<Duration T = milliseconds>
		void FixedTimeStep(uint64_t n) noexcept {
			m_fixedTimeStep = nanoseconds(T(n));
		}
		/// <summary>
		/// 直近のFixedUpdate呼び出し回数を取得する
		/// </summary>
		uint64_t GetFixedUpdateCount() const noexcept {
			return m_fixedUpdateCount;
		}
	public:
		/// <summary>
		/// タイマー開始からの経過時間を取得する
		/// </summary>
		/// <typeparam name="T">単位</typeparam>
		template<Duration T = milliseconds>
		uint64_t RealTimeSinceStartup() const noexcept {
			return static_cast<uint64_t>(duration_cast<T>(m_unscaledCurrent - m_start).count());
		}
		/// <summary>
		/// 直近二つのUpdate関数呼び出し間の経過時間を取得する。
		/// この値はスケールの影響を受ける
		/// </summary>
		/// <typeparam name="T">単位</typeparam>
		template<Duration T>
		uint64_t DeltaTime() const noexcept { return static_cast<uint64_t>(duration_cast<T>(m_scaledDelta).count()); }
		//float DeltaTime() const noexcept { return static_cast<float>(m_scaledDelta.count()) / nanoseconds(seconds(1)).count(); }

		/// <summary>
		/// 直近二つのUpdate関数呼び出し間の経過時間を取得する
		/// </summary>
		/// <typeparam name="T">単位</typeparam>
		template<Duration T>
		uint64_t UnscaledDeltaTime() const noexcept { return static_cast<uint64_t>(duration_cast<T>(m_unscaledDelta).count()); }
		/// <summary>
		/// タイマー開始からのUpdate関数呼び出し回数を取得する
		/// </summary>
		uint64_t FrameCount() const noexcept { return m_frameCount; }
		/// <summary>
		/// 直近二つのUpdate関数呼び出し間の経過時間を文字列として取得する
		/// </summary>
		/// <typeparam name="T">
		/// 単位。
		/// millisecondsかnanoseconds以外の場合、単位表記を含まない
		/// </typeparam>
		template<Duration T>
		String DeltaTimeString() const noexcept {
			static String s0 = u"Delta Time: ";
			static String ms = u"ms";
			static String ns = u"ns";
			return String::Joint(s0, DeltaTime<T>(), System::Traits::is_same_v<T, milliseconds> ? ms : System::Traits::is_same_v<T, nanoseconds> ? ns : String(u""));
		}
	public:
		/// <summary>
		/// この関数を呼び出したスレッドを休止する
		/// </summary>
		/// <param name="ms">休止する時間</param>
		static void Sleep(milliseconds ms) noexcept;
		static void Sleep(nanoseconds ns) noexcept;
		static void Sleep(seconds s) noexcept;
	};
}

//年月日計算
export namespace System {
	struct YearMonthDay {
		uint32_t year;
		uint32_t month;
		uint32_t day;
	};

	/// <summary>
	/// 経過時刻と基準時刻から年月日を算出する
	/// </summary>
	/// <param name="second">経過時刻(秒)</param>
	/// <param name="year">基準年</param>
	/// <param name="month">基準月</param>
	/// <param name="day">基準日</param>
	/// <returns>年月日を表す構造体</returns>
	YearMonthDay GetYearMonthDay(int64_t second, uint32_t year, uint32_t month, uint32_t day) noexcept {
		year_month_day date{ std::chrono::year(year), std::chrono::month(month), std::chrono::day(day) };
		date = std::chrono::time_point_cast<std::chrono::days>(static_cast<std::chrono::sys_days>(date) + std::chrono::seconds(second));
		return YearMonthDay{ static_cast<uint32_t>(static_cast<int>(date.year())), static_cast<unsigned>(date.month()), static_cast<unsigned>(date.day()) };
	}
}
