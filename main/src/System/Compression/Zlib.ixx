export module Zlib;
import Objects;
import Traits;
import Math;

export namespace System::Compression {
	/// <summary>
	/// Adler-32を計算する関数
	/// </summary>
	class Adler32 {
		static constexpr uint16_t BASE = 65521;
	public:
		/// <summary>
		/// Adler-32を計算する
		/// </summary>
		/// <param name="data">計算するデータ</param>
		/// <param name="byteCount">第一引数のバイト長</param>
		/// <param name="s2">計算結果の上位2バイト</param>
		/// <param name="s1">計算結果の下位2バイト</param>
		static void Calculate(const uint8_t* data, size_t byteCount, uint16_t& s2, uint16_t& s1) {
			s1 = 1;
			s2 = 0;
			for (size_t i = 0; i < byteCount; ++i) {
				s1 = (s1 + data[i]) % BASE;
				s2 = (s2 + s1) % BASE;
			}
		}
		static uint32_t Rolling(uint32_t adler, uint32_t length, uint8_t delValue, uint8_t addValue) noexcept {
			uint32_t a = adler & 0xffff;
			uint32_t b = (adler >> 16) & 0xffff;
			b = (b + addValue - delValue * (length + 1) + a - 1 + BASE) % BASE;
			a = (a + addValue - delValue + BASE) % BASE;
			return (b << 16) | a;
		}
	};

	//struct Zlib {
	//	uint8_t CMF = 0;	//圧縮方式とフラグ(Compression Method and Flags)
	//	uint8_t FLG = 0;	//フラグ
	//	uint32_t DICTID = 0;	//プリセット辞書ID(HasDictionary関数がtrueを返すときのみ存在)
	//	HuffmanBlock data;	//圧縮データ
	//	uint16_t adler32[2] = { 0, 0 };	//Adler-32
	//public:
	//	Zlib() noexcept = default;
	//	Zlib(const uint8_t* binary, size_t byteCount) noexcept {
	//		if (byteCount < 7) return;
	//		size_t pos = 0;
	//		CMF = binary[pos++];
	//		FLG = binary[pos++];
	//		if (HasDictionary()) {
	//			DICTID = static_cast<uint32_t>(binary[pos++] << 24);
	//			DICTID |= static_cast<uint32_t>(binary[pos++] << 16);
	//			DICTID |= static_cast<uint32_t>(binary[pos++] << 8);
	//			DICTID |= static_cast<uint32_t>(binary[pos++]);
	//		}
	//		if (pos < byteCount - 4) data = static_cast<HuffmanBlock&&>(HuffmanBlock(binary + pos, byteCount - 4 - pos));
	//		adler32[0] = static_cast<uint16_t>(binary[byteCount - 4] << 8);
	//		adler32[0] |= static_cast<uint16_t>(binary[byteCount - 3]);
	//		adler32[1] = static_cast<uint16_t>(binary[byteCount - 2] << 8);
	//		adler32[1] |= static_cast<uint16_t>(binary[byteCount - 1]);
	//	}
	//	~Zlib() noexcept = default;
	//public:
	//	uint8_t GetCompressionMethod() const noexcept { return CMF & Traits::BITMASK_M<1, 4>; }
	//	uint8_t GetCompressionInfo() const noexcept { return (CMF & Traits::BITMASK_M<5, 8>) >> 4; }
	//	uint32_t GetSlidingWindowSize() const noexcept {
	//		uint32_t cinfo = GetCompressionInfo();
	//		cinfo += 8;
	//		return static_cast<uint32_t>(Math::Pow(2, cinfo));
	//	}
	//	uint8_t GetCompressionLevel() const noexcept { return (FLG & Traits::BITMASK_M<7, 8>) >> 6; }
	//	bool HasDictionary() const noexcept { return (FLG & Traits::BITMASK_M<6, 6>); }
	//	bool IsBroken() const noexcept {
	//		uint16_t tmp = static_cast<uint16_t>(CMF << 8) | static_cast<uint16_t>(FLG);
	//		return tmp % 31 == 0;
	//	}
	//	void CalculateCheckFlag() noexcept {
	//		uint16_t tmp = static_cast<uint16_t>(CMF << 8) | static_cast<uint16_t>(FLG & Traits::BITMASK_M<6, 8>);
	//		uint8_t flg = static_cast<uint8_t>(0x1fu - (tmp % 0x1fu));
	//		FLG = static_cast<uint8_t>((FLG & Traits::BITMASK_M<6, 8>) | (flg & Traits::BITMASK_M<1, 5>));
	//	}
	//};

	class ZlibView {
		uint8_t* data;
		const size_t byteCount;
	public:
		ZlibView(uint8_t* dataPtr, size_t byteCount) noexcept : data(dataPtr), byteCount(byteCount) {}
	public:
		uint8_t GetCMF() const noexcept { return data ? data[0] : 0; }
		uint8_t GetFLG() const noexcept { return data && byteCount >= 2 ? data[1] : 0; }
		uint32_t GetDICTID() const noexcept { return HasDictionary() && byteCount >= 6 ? static_cast<uint32_t>((data[2] << 24) | (data[3] << 16) | (data[4] << 8) | (data[5])) : 0; }
		uint32_t GetAdler32() const noexcept {
			if (data && (HasDictionary() ? byteCount >= 10 : byteCount >= 6)) {
				uint32_t ret = static_cast<uint32_t>(data[byteCount - 4] << 24);
				ret |= static_cast<uint32_t>(data[byteCount - 3] << 16);
				ret |= static_cast<uint32_t>(data[byteCount - 2] << 8);
				ret |= static_cast<uint32_t>(data[byteCount - 1]);
				return ret;
			}
			else return 0;
		}
	public:
		uint8_t GetCompressionMethod() const noexcept { return GetCMF() & Traits::BITMASK_M<1, 4>; }
		uint8_t GetCompressionInfo() const noexcept { return (GetCMF() & Traits::BITMASK_M<5, 8>) >> 4; }
		uint32_t GetSlidingWindowSize() const noexcept {
			uint32_t cinfo = GetCompressionInfo();
			cinfo += 8;
			return static_cast<uint32_t>(Math::Pow(2, cinfo));
		}
		uint8_t GetCompressionLevel() const noexcept { return (GetFLG() & Traits::BITMASK_M<7, 8>) >> 6; }
		bool HasDictionary() const noexcept { return (GetFLG() & Traits::BITMASK_M<6, 6>); }
		bool IsBroken() const noexcept { return (static_cast<uint16_t>(GetCMF() << 8) | static_cast<uint16_t>(GetFLG())) % 31 == 0; }
	public:
		uint8_t* GetCompressedData() const noexcept {
			return data ? (HasDictionary() ? (byteCount > 10 ? data + 6 : nullptr) : (byteCount > 6 ? data + 2 : nullptr)) : nullptr;
		}
		size_t GetCompressedDataSize() const noexcept {
			return HasDictionary() ? (byteCount > 10 ? byteCount - 10 : 0) : (byteCount > 6 ? byteCount - 6 : 0);
		}
	};
}