module;
#include<chrono>
export module Time;
import CSTDINT;
import Traits;
import Objects;
import EventHandler;
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
		/// �^�C�}�[������������
		/// </summary>
		void Start() noexcept;
		/// <summary>
		/// �������X�V���A�o�^���ꂽ�C�x���g���Ăяo���B
		/// �C�x���g�̌Ăяo���񐔂̓X�P�[���̉e�����󂯂�
		/// </summary>
		void Update(bool callInvoke = true) noexcept;
	public:
		/// <summary>
		/// ���݂̎��Ԍo�߂̃X�P�[�����擾����
		/// </summary>
		float TimeScale() const noexcept { return m_scale; }
		/// <summary>
		/// ���Ԍo�߂̃X�P�[����ݒ肷��B
		/// DeltaTime�֐���Update�֐��ɉe������
		/// </summary>
		/// <param name="scale">�V���ɐݒ肷��X�P�[��</param>
		void TimeScale(float scale) noexcept { m_scale = scale; }
		/// <summary>
		/// ���݂�FixedUpdate�C�x���g�̌Ăяo���Ԋu���擾����
		/// </summary>
		/// <typeparam name="T">�P��</typeparam>
		template<Duration T = milliseconds>
		uint64_t FixedTimeStep() const noexcept {
			return static_cast<uint64_t>(duration_cast<T>(m_fixedTimeStep).count());
		}
		/// <summary>
		/// FixedUpdate�C�x���g�̌Ăяo���Ԋu��ݒ肷��
		/// </summary>
		/// <typeparam name="T">�P��</typeparam>
		/// <param name="n">�Ăяo���Ԋu</param>
		template<Duration T = milliseconds>
		void FixedTimeStep(uint64_t n) noexcept {
			m_fixedTimeStep = nanoseconds(T(n));
		}
		/// <summary>
		/// ���߂�FixedUpdate�Ăяo���񐔂��擾����
		/// </summary>
		uint64_t GetFixedUpdateCount() const noexcept {
			return m_fixedUpdateCount;
		}
	public:
		/// <summary>
		/// �^�C�}�[�J�n����̌o�ߎ��Ԃ��擾����
		/// </summary>
		/// <typeparam name="T">�P��</typeparam>
		template<Duration T = milliseconds>
		uint64_t RealTimeSinceStartup() const noexcept {
			return static_cast<uint64_t>(duration_cast<T>(m_unscaledCurrent - m_start).count());
		}
		/// <summary>
		/// ���ߓ��Update�֐��Ăяo���Ԃ̌o�ߎ��Ԃ��擾����B
		/// ���̒l�̓X�P�[���̉e�����󂯂�
		/// </summary>
		/// <typeparam name="T">�P��</typeparam>
		template<Duration T>
		uint64_t DeltaTime() const noexcept { return static_cast<uint64_t>(duration_cast<T>(m_scaledDelta).count()); }
		//float DeltaTime() const noexcept { return static_cast<float>(m_scaledDelta.count()) / nanoseconds(seconds(1)).count(); }

		/// <summary>
		/// ���ߓ��Update�֐��Ăяo���Ԃ̌o�ߎ��Ԃ��擾����
		/// </summary>
		/// <typeparam name="T">�P��</typeparam>
		template<Duration T>
		uint64_t UnscaledDeltaTime() const noexcept { return static_cast<uint64_t>(duration_cast<T>(m_unscaledDelta).count()); }
		/// <summary>
		/// �^�C�}�[�J�n�����Update�֐��Ăяo���񐔂��擾����
		/// </summary>
		uint64_t FrameCount() const noexcept { return m_frameCount; }
		/// <summary>
		/// ���ߓ��Update�֐��Ăяo���Ԃ̌o�ߎ��Ԃ𕶎���Ƃ��Ď擾����
		/// </summary>
		/// <typeparam name="T">
		/// �P�ʁB
		/// milliseconds��nanoseconds�ȊO�̏ꍇ�A�P�ʕ\�L���܂܂Ȃ�
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
		/// ���̊֐����Ăяo�����X���b�h���x�~����
		/// </summary>
		/// <param name="ms">�x�~���鎞��</param>
		static void Sleep(milliseconds ms) noexcept;
		static void Sleep(nanoseconds ns) noexcept;
		static void Sleep(seconds s) noexcept;
	};
}

//�N�����v�Z
export namespace System {
	struct YearMonthDay {
		uint32_t year;
		uint32_t month;
		uint32_t day;
	};

	/// <summary>
	/// �o�ߎ����Ɗ��������N�������Z�o����
	/// </summary>
	/// <param name="second">�o�ߎ���(�b)</param>
	/// <param name="year">��N</param>
	/// <param name="month">���</param>
	/// <param name="day">���</param>
	/// <returns>�N������\���\����</returns>
	YearMonthDay GetYearMonthDay(int64_t second, uint32_t year, uint32_t month, uint32_t day) noexcept {
		year_month_day date{ std::chrono::year(year), std::chrono::month(month), std::chrono::day(day) };
		date = std::chrono::time_point_cast<std::chrono::days>(static_cast<std::chrono::sys_days>(date) + std::chrono::seconds(second));
		return YearMonthDay{ static_cast<uint32_t>(static_cast<int>(date.year())), static_cast<unsigned>(date.month()), static_cast<unsigned>(date.day()) };
	}
}
