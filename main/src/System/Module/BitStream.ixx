export module BitStream;
import CSTDINT;
import Traits;
using namespace System::Traits;

export namespace System {
	class BitStream {
		uint8_t* m_data = nullptr;
		size_t m_byteCount = 0;
		uint8_t m_currentBit = 0;
		size_t m_currentByte = 0;
	public:
		bool IsZeroStart = true;
	public:
		BitStream() noexcept = delete;
		BitStream(const BitStream& arg) noexcept : m_data(arg.m_data), m_byteCount(arg.m_byteCount), m_currentBit(arg.m_currentBit), m_currentByte(arg.m_currentByte), IsZeroStart(arg.IsZeroStart) {}
		BitStream(uint8_t* data, size_t byteCount, bool isZeroStart = true) noexcept : m_data(data), m_byteCount(byteCount), IsZeroStart(isZeroStart), m_currentBit(byteCount == 0 ? 8 : isZeroStart ? 0 : 7) {}
		~BitStream() noexcept { m_data = nullptr; }
	public:
		inline bool StartBit() const noexcept { return IsZeroStart ? m_currentBit == 0 : m_currentBit == 7; }
		inline bool StartByte() const noexcept { return m_currentByte == 0; }
		inline bool EndBit() const noexcept { return IsZeroStart ? m_currentBit == 7 : m_currentBit == 0; }
		inline bool EndByte() const noexcept { return m_byteCount == 0 || (m_currentByte >= m_byteCount - 1); }
		inline bool EndOfStream() const noexcept { return m_currentBit == 8; }
	private:
		/// <summary>
		/// [lowBit, highBit]のビットマスクを取得する
		/// </summary>
		/// <param name="lowBit">ビットが1である最下位のビット番号。範囲は[0, sizeof(T) * 8 - 1]</param>
		/// <param name="highBit">ビットが1である最上位のビット番号。範囲は[0, sizeof(T) * 8 - 1]</param>
		/// <returns>T型のビットマスク値</returns>
		template<Concepts::CUnsigned T>
		static constexpr T GetBitMask(uint8_t lowBit, uint8_t highBit) noexcept {
			T ret = 0;
			for (uint8_t i = lowBit; i <= highBit; ++i) ret |= 1ull << i;
			return ret;
		}
		/// <summary>
		/// 並びを反転したビット列を取得する
		/// </summary>
		/// <param name="bits">並びを反転するビット列</param>
		/// <returns>並びが反転したビット列</returns>
		template<Concepts::CUnsigned T>
		static constexpr T ReverseBit(T bits) noexcept {
			if constexpr (sizeof(T) >= 8) bits = ((bits & 0x00000000ffffffffull) << 32) | ((bits >> 32) & 0x00000000ffffffffull);
			if constexpr (sizeof(T) >= 4) bits = ((bits & 0x0000ffff0000ffffull) << 16) | ((bits >> 16) & 0x0000ffff0000ffffull);
			if constexpr (sizeof(T) >= 2) bits = ((bits & 0x00ff00ff00ff00ffull) << 8) | ((bits >> 8) & 0x00ff00ff00ff00ffull);
			bits = ((bits & 0x0f0f0f0f0f0f0f0full) << 4) | ((bits >> 4) & 0x0f0f0f0f0f0f0f0full);
			bits = ((bits & 0x3333333333333333ull) << 2) | ((bits >> 2) & 0x3333333333333333ull);
			bits = ((bits & 0x5555555555555555ull) << 1) | ((bits >> 1) & 0x5555555555555555ull);
			return bits;
		}
	public:
		/// <summary>
		/// 現在指しているバイトにデータを書き込む。ストリームのビット位置は無視される
		/// </summary>
		/// <param name="byte">書き込む値</param>
		/// <param name="lowToHigh">第一引数を読み込む方向。trueのとき、最下位ビットから読み込む。書き込み方向はIsZeroStartに依存する</param>
		/// <param name="seekNext">trueのとき、次のバイトの先頭までストリームを読み進める</param>
		inline void SetByte(uint8_t byte, bool lowToHigh, bool seekNext = false) noexcept {
			if (EndOfStream()) return;
			m_data[m_currentByte] = lowToHigh == IsZeroStart ? byte : ReverseBit<uint8_t>(byte);
			if (seekNext) MoveNextByte();
		}
	public:
		/// <summary>
		/// 現在指しているビットにデータを書き込む
		/// </summary>
		/// <param name="bit">trueのときは1を、falseのときは0を書き込む</param>
		/// <param name="seekNext">trueのとき、1ビット読み進める</param>
		inline void Set(bool bit, bool seekNext = false) noexcept {
			if (EndOfStream()) return;
			if (bit) m_data[m_currentByte] |= GetBitMask<uint8_t>(m_currentBit, m_currentBit);
			else m_data[m_currentByte] &= ~GetBitMask<uint8_t>(m_currentBit, m_currentBit);
			if (seekNext) operator++();
		}
		/// <summary>
		/// 現在指しているビットを先頭としてビット列を書き込む。ビット列は第二引数で指定したビットから第三引数で指定したビットに向かって読み込む
		/// </summary>
		/// <param name="bits">書き込むビット列</param>
		/// <param name="startBit">第一引数を読み込む始点となるビット番号。範囲は[0, 63]</param>
		/// <param name="endBit">第一引数を読み込む終点となるビット番号。範囲は[0, 63]。</param>
		/// <param name="seekNext">trueのとき、書き込んだビット数だけストリームを読み進める</param>
		void Set(uint64_t bits, uint8_t startBit, uint8_t endBit, bool seekNext = false) noexcept {
			if (EndOfStream()) return;
			uint8_t currentBit = m_currentBit;
			size_t currentByte = m_currentByte;
			uint8_t low = startBit < endBit ? startBit : endBit;
			uint8_t high = startBit < endBit ? endBit : startBit;
			uint64_t tmp = bits & GetBitMask<uint64_t>(low, high);
			tmp >>= low;
			const uint8_t count = high - low + 1;
			if (low != startBit) tmp = ReverseBit(tmp) >> (sizeof(uint64_t) * 8 - count);
			for (uint8_t i = 0; i < count; ++i) {
				if (StartBit() && (i + 7 < count)) {
					SetByte(static_cast<uint8_t>(tmp & Traits::BITMASK_M<1, 8>), true, true);
					tmp >>= 8;
					i += 7;
				}
				else {
					Set(tmp & 0x01ull);
					tmp >>= 1;
					operator++();
				}
				if (EndOfStream()) break;
			}
			if (!seekNext) {
				m_currentBit = currentBit;
				m_currentByte = currentByte;
			}
		}
	public:
		/// <summary>
		/// 現在指しているバイトを取得する。ストリームのビット位置は無視される
		/// </summary>
		/// <param name="seekNext">trueのとき、次のバイトの先頭までストリームを読み進める</param>
		/// <returns>読み取った1バイトのデータ</returns>
		inline uint8_t GetByte(bool seekNext = false) noexcept {
			if (EndOfStream()) return 0;
			uint8_t ret = m_data[m_currentByte];
			if (seekNext) MoveNextByte();
			return ret;
		}
	public:
		/// <summary>
		/// 現在指しているビットを取得する
		/// </summary>
		/// <param name="seekNext">trueのとき、ストリームを1ビット読み進める</param>
		/// <returns>現在指しているビットが1のときはtrueを、0のときはfalseを返す</returns>
		template<bool CheckEnd = false>
		inline bool Get(bool seekNext = false) noexcept {
			if constexpr (CheckEnd) {
				if (EndOfStream()) return false;
			}
			bool ret = m_data[m_currentByte] & GetBitMask<uint8_t>(m_currentBit, m_currentBit);
			if (seekNext) operator++();
			return ret;
		}
		/// <summary>
		/// 現在指しているビットを先頭とするビット列を取得する
		/// </summary>
		/// <param name="bitCount">取得するビット数</param>
		/// <param name="lowToHigh">trueのとき、ビット列を最下位ビットから並べて取得する。falseのとき、ビット列を最上位ビットから並べて取得する</param>
		/// <param name="seekNext">trueのとき、取得したビット数だけストリームを進める</param>
		/// <returns>ビット列</returns>
		template<Concepts::CUnsigned T, bool CheckEnd = false>
		inline T Get(uint8_t bitCount, bool lowToHigh, bool seekNext = false) noexcept {
			if constexpr (CheckEnd) {
				if (EndOfStream()) return 0;
			}
			uint8_t currentBit = m_currentBit;
			size_t currentByte = m_currentByte;
			T ret = 0;
			int n = lowToHigh ? 0 : sizeof(T) * 8 - 1;
			if (bitCount > sizeof(T) * 8) bitCount = sizeof(T) * 8;
			for (uint8_t i = 0; i < bitCount; ++i) {
				if (StartBit() && (i + 7 < bitCount)) {
					uint8_t tmp = IsZeroStart == lowToHigh ? GetByte(true) : ReverseBit(GetByte(true));
					if (lowToHigh) {
						ret |= static_cast<T>(tmp) << n;
						n += 8;
					}
					else {
						n -= 7;
						ret |= static_cast<T>(tmp) << n;
						--n;
					}
					i += 7;
				}
				else {
					ret |= (Get(true) ? 1ull : 0ull) << n;
					if (lowToHigh) ++n;
					else --n;
				}
				if (EndOfStream()) break;
			}
			if (!seekNext) {
				m_currentBit = currentBit;
				m_currentByte = currentByte;
			}
			return ret;
		}
	public:
		/// <summary>
		/// 現在指しているビットを先頭とするビット列を逆順で取得する
		/// </summary>
		/// <param name="bitCount">取得するビット数</param>
		/// <param name="lowToHigh">trueのとき、ビット列を最下位ビットから並べて取得する。falseのとき、ビット列を最上位ビットから並べて取得する</param>
		/// <param name="seekNext">trueのとき、取得したビット数だけストリームを進める</param>
		/// <returns>逆順のビット列。第二引数がtrueのとき、最下位ビットは最後に取得したビットであり、第一引数から1引いたビット番号のビットが最初に取得したビットとなる</returns>
		template<Concepts::CUnsigned T>
		inline T GetReverseBit(uint8_t bitCount, bool lowToHigh, bool seekNext = false) noexcept {
			T ret = Get<T>(bitCount, !lowToHigh, seekNext);
			if (lowToHigh) return ret >> (sizeof(T) * 8 - bitCount);
			else return ret << (sizeof(T) * 8 - bitCount);
		}
	public:
		inline void SeekStartBit() noexcept { m_currentBit = IsZeroStart ? 0 : 7; }
		inline void SeekEndBit() noexcept { m_currentBit = IsZeroStart ? 7 : 0; }
	public:
		inline void SeekBit(size_t n, bool forward = true) noexcept {
			if (EndOfStream()) return;
			while (n > 0) {
				if (StartBit() && n >= 8) {
					if (forward) MoveNextByte();
					else MovePreviousByte();
					n -= 8;
				}
				else {
					if (forward) operator++();
					else operator--();
					--n;
				}
			}
		}
	public:
		inline void SeekStartByte() noexcept {
			if (m_byteCount != 0) {
				m_currentByte = 0;
				SeekStartBit();
			}
		}
		inline void SeekEndByte() noexcept {
			if (m_byteCount != 0) {
				m_currentByte = m_byteCount - 1;
				SeekStartBit();
			}
		}
		inline void MovePreviousByte() noexcept {
			if (EndOfStream()) return;
			if (m_currentByte != 0) --m_currentByte;
			SeekStartBit();
		}
		inline void MoveNextByte() noexcept {
			if (EndOfStream()) return;
			if (EndByte()) m_currentBit = 8;
			else {
				++m_currentByte;
				SeekStartBit();
			}
		}
	public:
		inline size_t GetCurrentByte() const noexcept { return m_currentByte; }
		inline uint8_t GetCurrentBit() const noexcept { return m_currentBit; }
		inline size_t GetByteCount() const noexcept { return m_byteCount; }
	public:
		inline BitStream& operator<<(size_t n) noexcept {
			while (n >= 8) {
				for (size_t i = 1; i < m_byteCount; ++i) m_data[i - 1] = m_data[i];
				m_data[m_byteCount - 1] = 0;
				n -= 8;
			}
			if (n > 0) {
				const uint8_t mask = GetBitMask<uint8_t>(0, 7 - static_cast<uint8_t>(n));
				const uint8_t mask2 = GetBitMask<uint8_t>(8 - static_cast<uint8_t>(n), 7);
				for (size_t i = 1; i < m_byteCount; ++i) {
					m_data[i - 1] = ((m_data[i - 1] & mask) << n) | ((m_data[i] & mask2 >> (8 - n)));
				}
				m_data[m_byteCount - 1] = (m_data[m_byteCount - 1] & mask) << n;
			}
			return *this;
		}
		inline BitStream& operator>>(size_t n) noexcept {
			while (n >= 8) {
				for (size_t i = m_byteCount - 1; i != 0; --i) m_data[i] = m_data[i - 1];
				m_data[0] = 0;
				n -= 8;
			}
			if (n > 0) {
				const uint8_t mask = GetBitMask<uint8_t>(static_cast<uint8_t>(n), 7);
				const uint8_t mask2 = GetBitMask<uint8_t>(0, static_cast<uint8_t>(n) - 1);
				for (size_t i = m_byteCount - 1; i != 0; --i) {
					m_data[i] = ((m_data[i] & mask) >> n) | ((m_data[i - 1] & mask2) << (8 - n));
				}
				m_data[0] = (m_data[0] & mask) >> n;
			}
			return *this;
		}
	public:
		inline BitStream& operator++() noexcept {
			if (m_currentBit == 8) return *this;
			if (IsZeroStart) {
				if (m_currentBit == 7) {
					if (m_currentByte >= m_byteCount - 1) m_currentBit = 8;
					else {
						++m_currentByte;
						m_currentBit = 0;
					}
				}
				else ++m_currentBit;
			}
			else {
				if (m_currentBit == 0) {
					if (m_currentByte >= m_byteCount - 1) m_currentBit = 8;
					else {
						++m_currentByte;
						m_currentBit = 7;
					}
				}
				else --m_currentBit;
			}
			return *this;
		}
		inline BitStream operator++(int) noexcept {
			BitStream ret = *this;
			operator++();
			return ret;
		}
		inline BitStream& operator--() noexcept {
			if (StartBit()) {
				MovePreviousByte();
				SeekEndBit();
			}
			else {
				if (IsZeroStart) --m_currentBit;
				else ++m_currentBit;
			}
			return *this;
		}
		inline BitStream operator--(int) noexcept {
			BitStream ret = *this;
			operator--();
			return ret;
		}
	};
}
