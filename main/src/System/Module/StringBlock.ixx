export module StringBlock;
export import CSTDINT;
export import Traits;
export import Allocator;
import Math;
export import VectorBase;
import CodePoint;
import <iostream>;	//overload operator<<

//CStringBlock
export namespace System {
	template<Traits::Concepts::CCharType str_t> class StringBlock;

	template<Traits::Concepts::CCharType str_t, size_t N>
	class CStringBlock {
		friend class StringBlock<str_t>;
	protected:
		str_t value[N];
	public:
		static constexpr size_t npos = MAX_VALUE<size_t>;
	public:
		constexpr CStringBlock() : value() {}
		constexpr CStringBlock(const CStringBlock<str_t, N>& arg) noexcept {
			for (size_t i = 0; i < N; ++i) value[i] = arg.value[i];
		}
		explicit constexpr CStringBlock(const str_t c) noexcept : value{ c, '\0'} {}
		constexpr CStringBlock(const str_t(&arg)[N]) noexcept {
			for (size_t i = 0; i < N; ++i) value[i] = arg[i];
			value[N - 1] = '\0';
		}
		constexpr CStringBlock(const str_t* arg, size_t n) noexcept {
			if (n == 0) n = N - 1;
			size_t i = 0;
			if (arg) for (; i < N - 1 && arg[i] != '\0' && i < n; ++i) value[i] = arg[i];
			for (; i <= N - 1; ++i) value[i] = '\0';			
		}
		~CStringBlock() = default;
	public:
		static constexpr auto Empty() noexcept {
			if constexpr (Traits::is_same_v<str_t, char>) return CStringBlock<str_t, 3>();
			else if constexpr (Traits::is_same_v<str_t, char8_t>) return CStringBlock<str_t, 5>();
			else if constexpr (Traits::is_same_v<str_t, char16_t>) return CStringBlock<str_t, 3>();
			else if constexpr (Traits::is_same_v<str_t, char32_t>) return CStringBlock<str_t, 2>();
			else if constexpr (Traits::is_same_v<str_t, wchar_t>) return CStringBlock<str_t, 3>();
		}
	public:
		constexpr CStringBlock<str_t, N> const& strcpy(const str_t *src, const size_t pos, const size_t n) noexcept {
			for (size_t i = 0; i < n && pos + i < N - 1; ++i) value[pos + i] = src[i];
			return *this;
		}
		constexpr CStringBlock<str_t, N> const& strcpy(const str_t src, const size_t pos) noexcept {
			if (pos < N) value[pos] = src;
			return *this;
		}
	public:
		constexpr const str_t* c_str() const noexcept { return value; }
		constexpr size_t Length() const noexcept { for (size_t i = 0; i < N; ++i) if (value[i] == '\0') return i; return 0; }
	public:
		template<size_t n>
		constexpr size_t find(const CStringBlock<str_t, n>& src) const noexcept {
			size_t len = Length(), srcLen = src.Length();
			if (len < srcLen) return npos;
			const str_t* srcData = src.c_str();
			for (size_t i = 0; i < len; ++i) {
				if (value[i] != srcData[0]) continue;
				if (len - i < srcLen) return npos;
				size_t j = 1;
				for (; j < srcLen; ++j) if (value[i + j] != srcData[j]) break;
				if (j == srcLen) return i;
			}
			return npos;
		}
		template<size_t n>
		constexpr size_t rfind(const CStringBlock<str_t, n>& src) const noexcept {
			size_t len = Length(), srcLen = src.Length();
			if (len < srcLen) return npos;
			const str_t* srcData = src.c_str();
			for (size_t i = len, srcEnd = srcLen - 1; i-- > 0;) {
				if (value[i] != srcData[srcEnd]) continue;
				if (i + 1 < srcLen) return npos;
				size_t j = 1;
				for (; j <= srcEnd; ++j) {
					if (value[i - j] != srcData[srcEnd - j]) break;
				}
				if (j > srcEnd) return i - j + 1;
			}
			return npos;
		}
		constexpr CStringBlock<str_t, N> substr(const size_t pos, const size_t n = npos) const noexcept {
			if (pos >= Length()) return CStringBlock<str_t, N>();
			else return CStringBlock<str_t, N>(value + pos, n);
		}
	public:
		constexpr auto operator[](const size_t i) const noexcept { return Length() <= i ? Empty() : CStringBlock(value[i]); }
		template<size_t n>
		constexpr auto operator+(const CStringBlock<str_t, n>& rhs) const noexcept {
			CStringBlock<str_t, N + n - 1> ret;
			const size_t len = Length();
			ret.strcpy(value, 0, len);
			const size_t rlen = rhs.Length();
			ret.strcpy(rhs.c_str(), len, rlen);
			ret.strcpy('\0', len + rlen);
			return ret;
		}
		template<size_t n>
		constexpr bool operator==(const CStringBlock<str_t, n>& rhs) const noexcept {
			size_t len = Length(), rhsLen = rhs.Length();
			if (len != rhsLen) return false;
			const str_t* rdata = rhs.c_str();
			for (size_t i = 0; i < len; ++i) if (value[i] != rdata[i]) return false;
			return true;
		}
		template<size_t n> constexpr bool operator!=(const CStringBlock<str_t, n>& rhs) const noexcept { return !(*this == rhs); }
		constexpr operator bool() const noexcept { return value[0] != '\0'; }
	};

	template<Traits::Concepts::CCharType str_t, Traits::Concepts::CIntegral T>
	constexpr CStringBlock<str_t, 21> GetCStringBlock(T n) noexcept {
		CStringBlock<str_t, 21> ret;
		size_t digit = Math::CountDigit(n);
		size_t i = 0;
		if (n < 0) ret.strcpy('-', i++);
		size_t abs = Math::Abs(n);
		while (digit > 0) {
			ret.strcpy('0' + (abs % 10), digit-- - 1 + i);
			abs /= 10;
		}
		return ret;
	}

	enum class NormalizeMode : uint8_t {
		Auto,			//自動選択
		Fixed,			//変換しない
		Normalize	//強制的に変換する
	};

	/// <summary>
	/// 浮動小数点数を文字列定数に変換する
	/// </summary>
	/// <typeparam name="str_t">変換先の文字型</typeparam>
	/// <typeparam name="digit">有効桁数</typeparam>
	/// <param name="d">浮動小数点数</param>
	/// <param name="mode">指数表記(x.xxe-10など)変換モード</param>
	/// <returns>
	/// 指定した有効桁数の文字列定数。
	/// 引数modeがAutoのとき、有効桁数がdの整数部の桁数よりも小さい、
	/// もしくは有効桁数がすべて0で埋まる(0.0001に対して有効桁数4以下)場合、指数表記となる
	/// </returns>
	template<Traits::Concepts::CCharType str_t, size_t digit = 10>
	constexpr auto GetCStringBlock(double d, NormalizeMode mode = NormalizeMode::Auto) noexcept {
		//許容する有効桁数の大きさ
		//1075桁 = 1桁(整数部) + 1074桁(小数部)
		//2^-1074(doubleで表せる一番小さい数)の最下位の桁は10^-1074の位であることに由来する
		constexpr size_t max_digit = 1075;
		if constexpr (digit == 0) return GetCStringBlock<str_t, 1>(d, mode);	//有効桁数0桁は1桁として扱う
		else if constexpr (digit > max_digit) return GetCStringBlock<str_t, max_digit>(d, mode);	//有効桁数はmax_digitまで
		else {
			CStringBlock<str_t, digit + 8> ret;	//有効桁数+8文字(小数点+e+指数符号+指数(最大3桁)+ヌル終端文字)
			if (Math::IsNan(d)) {
				ret.strcpy('n', 0);
				ret.strcpy('a', 1);
				ret.strcpy('n', 2);
			}
			else if (Math::IsInf(d)) {
				size_t pos = 0;
				if (d < 0) ret.strcpy('-', pos++);
				ret.strcpy('i', pos++);
				ret.strcpy('n', pos++);
				ret.strcpy('f', pos++);
			}
			else {
				//浮動小数点数の内部表現を解析
				//仮数部ビットを符号無し整数として文字列化
				auto frac_str = GetCStringBlock<str_t>(Math::GetFracValue(d));
				//ヌル終端文字を含むfrac_strの文字数
				const size_t frac_size = frac_str.Length() + 1;
				//符号無し53ビット整数は最大16桁のため、frac_sizeはヌル終端文字を含めて17文字に収まるはず
				//(18文字以上の場合、空文字列を返す)
				if (frac_size >= 18) return ret;
				//浮動小数点数の指数2^exp + 仮数部52ビット分のシフト
				int exp = Math::GetExpValue(d) - 52;
				//最大有効桁数+3(符号+小数点+ヌル終端文字)
				constexpr size_t BUF_SIZE = max_digit + 3;
				//計算用バッファ
				str_t r_buf[BUF_SIZE] = {};
				if (d < 0) r_buf[0] = '-';
				//正負記号を除いたバッファの先頭
				str_t* const buf = d >= 0 ? r_buf : r_buf + 1;
				//buf[length]は現在のヌル終端文字
				size_t length = 0;
				for (const str_t* ptr = frac_str.c_str(); length < frac_size; ++length) buf[length] = ptr[length];
				buf[length - 1] = '.';
				buf[length++] = '0';
				//高速化のため、2^n倍ごとに計算する(exp < nの場合、2倍ごとに計算する)
				constexpr int n = 16;
				//文字列bufの数値を(2^exp)倍する(exp < 0の場合、割り算になる)
				if (exp > 0) {
					for (int j = exp; j-- > 0;) {
						int mul = 1 << (j >= n ? n : 1);
						if (j >= n) j -= n - 1;
						int up = 0;	//繰り上がり
						for (size_t i = length; i-- > 0;) {
							if (buf[i] == '.') continue;
							int tmp = (buf[i] - '0') * mul + up;
							buf[i] = '0' + (tmp % 10);
							up = tmp / 10;
						}
						//繰り上がりが残っている場合、バッファ全体を右シフトして桁を増やす
						if (up) {
							size_t delta = Math::CountDigit(up);
							length = (buf + length + delta > r_buf + BUF_SIZE - 1) ? BUF_SIZE - 1 - (buf - r_buf) : length + delta;
							for (size_t i = length - delta; i-- > 0;) buf[i + delta] = buf[i];
							for (size_t i = delta; i-- > 0;) {
								buf[i] = '0' + (up % 10);
								up /= 10;
							}
						}
					}
				}
				else if (exp < 0) {
					for (int j = -exp; j-- > 0;) {
						int div = 1 << (j >= n ? n : 1);
						if (j >= n) j -= n - 1;
						int down = 0;	//繰り下がり
						for (size_t i = 0; i < length; ++i) {
							if (buf[i] == '.') continue;
							int tmp = (buf[i] - '0') + down;
							buf[i] = '0' + (tmp / div);
							down = (tmp % div) * 10;
						}
						//"0.dddd..."の形式でない時に、先頭が'0'で始まっている場合、'0'を消去する
						while (buf[0] == '0' && buf[1] != '.') {
							for (size_t i = 0; i < length; ++i) buf[i] = buf[i + 1];
							--length;
						}
						//繰り下がりが0になるか、バッファの終端に到達するまで割り算を続ける
						while (down && (buf + length < r_buf + BUF_SIZE - 1)) {
							buf[length++] = '0' + (down / div);
							down = (down % div) * 10;
						}
						buf[length] = '\0';
					}
				}
				//buf[dot]は現在の小数点
				size_t dot = 0;
				while (buf[dot] != '.') ++dot;
				//buf[first]は初めて出現する0以外の数字
				size_t first = buf[0] == '0' ? 2 : 0;
				while (buf[first] == '0') ++first;
				//buf[r]は有効桁数+1の位の数字
				size_t r = digit < dot ? digit : (digit + first + 1 < length ? digit + first + 1 : length);
				//有効桁数に合わせて丸め処理(偶数丸め)
				if (r < length) {
					//丸める桁の数字
					int tmp = buf[r] - '0';
					//繰り上がり(-1は偶数丸め)
					int up = tmp > 5 ? 1 : tmp < 5 ? 0 : -1;
					buf[r] = '0';
					for (size_t i = r; i-- > 0;) {
						if (i == dot) continue;
						tmp = buf[i] - '0';
						if (up == -1) up = tmp % 2 != 0 ? 1 : 0;
						//0 <= tmp + up < 20
						if (tmp + up == 10) {
							buf[i] = '0';
							up = 1;
						}
						else {
							buf[i] = '0' + tmp + up;
							up = 0;
						}
						if (!up) break;
					}
					//丸めの結果、桁の繰り上がりが起こった場合の処理
					if (up) {
						for (size_t i = r; i-- > 0;) buf[i + 1] = buf[i];
						buf[0] = '0' + 1;
						++dot;
						++r;
					}
				}
				int e = 0;	//N.xxx * 10^eの形に正規化したときの指数
				//正規化
				if ((mode == NormalizeMode::Auto && (digit < dot || digit < first)) || mode == NormalizeMode::Normalize) {
					//"x.xxxxxxx"の場合
					if (dot == 1) {
						if (first) {
							buf[0] = buf[first];
							for (size_t i = 1; first + i <= length; ++i) buf[1 + i] = buf[first + i];
							length -= first - 1;
							e = -static_cast<int>(first) + 1;
						}
					}
					//"xxxx.xxxx"の場合
					else {
						if (r < dot && r > 1) ++r;
						for (size_t i = dot; i-- > 1;) buf[i + 1] = buf[i];
						e = static_cast<int>(dot) - 1;
						dot = 1;
						buf[dot] = '.';
					}
				}
				//有効数字部分を文字列定数にコピー
				size_t pos = digit + 1 < length ? digit : length - 1;
				if (dot < digit) ++pos;
				if (d < 0) ++pos;
				ret.strcpy(r_buf, 0, pos);
				//指数表記を文字列定数に追加
				if (e != 0) {
					ret.strcpy('e', pos++);
					if (e > 0) ret.strcpy('+', pos++);
					else ret.strcpy('-', pos++);
					e = e > 0 ? e : -e;
					int tmp = static_cast<int>(Math::Pow(10, Math::CountDigit(e) - 1));
					if (tmp == 1) ret.strcpy('0', pos++);
					while (e != 0) {
						int n = e / tmp;
						ret.strcpy('0' + n, pos++);
						e -= n * tmp;
						tmp /= 10;
					}
				}
			}
			return ret;
		}
	}
}

//ToStringBlock
export namespace System::Encoding {
	/// <summary>
	/// CodePoint構造体の配列からutf8文字列を生成する
	/// </summary>
	/// <param name="codePoints">CodePoint構造体の配列へのポインタ</param>
	/// <param name="count">配列の長さ</param>
	/// <returns>char8_t型の文字列を持つStringBlockオブジェクト</returns>
	StringBlock<char8_t> ToU8StringBlock(const CodePoint* codePoints, size_t count) noexcept;

	/// <summary>
	/// CodePoint構造体の配列からutf16文字列を生成する
	/// </summary>
	/// <param name="codePoints">CodePoint構造体の配列へのポインタ</param>
	/// <param name="count">配列の長さ</param>
	/// <returns>char16_t型の文字列を持つStringBlockオブジェクト</returns>
	StringBlock<char16_t> ToU16StringBlock(const CodePoint* codePoints, size_t count) noexcept;

	/// <summary>
	/// CodePoint構造体の配列からutf32文字列を生成する
	/// </summary>
	/// <param name="codePoints">CodePoint構造体の配列へのポインタ</param>
	/// <param name="count">配列の長さ</param>
	/// <returns>char32_t型の文字列を持つStringBlockオブジェクト</returns>
	StringBlock<char32_t> ToU32StringBlock(const CodePoint* codePoints, size_t count) noexcept;

	/// <summary>
	/// CodePoint構造体の配列からワイド文字列を生成する
	/// </summary>
	/// <param name="codePoints">CodePoint構造体の配列へのポインタ</param>
	/// <param name="count">配列の長さ</param>
	/// <returns>wchar_t型の文字列を持つStringBlockオブジェクト</returns>
	StringBlock<wchar_t> ToWideStringBlock(const CodePoint* codePoints, size_t count) noexcept;

	/// <summary>
	/// CodePoint構造体の配列からマルチバイト文字列を生成する
	/// </summary>
	/// <param name="codePoints">CodePoint構造体の配列へのポインタ</param>
	/// <param name="count">配列の長さ</param>
	/// <returns>char型の文字列を持つStringBlockオブジェクト</returns>
	StringBlock<char> ToMultiByteStringBlock(const CodePoint* codePoints, size_t count) noexcept;
}

//StringBlock
export namespace System {
	template<Traits::Concepts::CCharType str_t>
	class StringBlock {
	private:
		using alloc_t = Allocator<str_t>;
		using traits = AllocatorTraits<alloc_t const>;
		static constexpr alloc_t alloc = alloc_t{};
	public: 
		str_t* value = nullptr;
		size_t size = 0;
	public:
		static constexpr size_t npos = MAX_VALUE<size_t>;
	public:
		StringBlock() noexcept : size(1) {
			value = alloc.allocate(size);
			traits::construct(alloc, value, str_t('\0'));
		}
		StringBlock(const StringBlock& arg) noexcept : size(arg.size) {
			value = alloc.allocate(size);
			for (size_t i = 0; i < size; ++i) traits::construct(alloc, value + i, arg.value[i]);
		}
		StringBlock(StringBlock&& arg) noexcept : size(arg.size), value(arg.value) {
			arg.size = 0;
			arg.value = nullptr;
		}
		StringBlock(str_t c) noexcept : size(2) {
			value = alloc.allocate(size);
			traits::construct(alloc, value, c);
			traits::construct(alloc, value + 1, str_t('\0'));
		}
		template<size_t N>
		StringBlock(const str_t(&arg)[N]) noexcept : size(N) {
			value = alloc.allocate(size);
			for (size_t i = 0; i < size; ++i) traits::construct(alloc, value + i, arg[i]);
			value[size - 1] = '\0';
		}
		StringBlock(const str_t* arg, size_t n = npos) noexcept {
			if (!arg) {
				size = 1;
				value = alloc.allocate(size);
				traits::construct(alloc, value, str_t('\0'));
			}
			else {
				if (n == 0) n = npos;
				size_t len = 0;
				while (arg[len] != '\0' && ++len < n);
				if (len == npos) {
					len = 0;
					size = 1;
				}
				else size = len + 1;
				value = alloc.allocate(size);
				for (size_t i = 0; i < len; ++i) traits::construct(alloc, value + i, arg[i]);
				for (size_t i = len; i < size; ++i) traits::construct(alloc, value + i, str_t('\0'));
			}
		}
		template<size_t N>
		StringBlock(const CStringBlock<str_t, N>& arg) noexcept {
			size_t len = arg.Length();
			size = len + 1;
			value = alloc.allocate(size);
			for (size_t i = 0; i < len; ++i) traits::construct(alloc, value + i, arg.value[i]);
			for (size_t i = len; i < size; ++i) traits::construct(alloc, value + i, str_t('\0'));			
		}
		~StringBlock() noexcept { InternalReset(); }
	public:
		/// <summary>
		/// 空文字列を取得する
		/// </summary>
		static StringBlock Empty() noexcept { return StringBlock(); }
		/// <summary>
		/// 指定した要素数の領域を持つ空文字列を取得する
		/// </summary>
		/// <param name="size">
		/// 要素数。
		/// n文字の文字列に使用する場合、ヌル終端文字のために(n+1)を
		/// 指定する必要がある
		/// </param>
		static StringBlock CreateStringBlock(size_t strSize) noexcept {
			StringBlock<str_t> ret;
			ret.InternalReset(strSize);
			return ret;
		}
	private:/* 内部使用関数 */
		/// <summary>
		/// 確保した領域を解放する。newSizeが0以外の時、新たな領域を確保する。
		/// 配列new/deleteにICEが発生しているため、メモリ確保/解放をこの関数で管理し、
		/// 一律で対処できるようにしたい。
		/// </summary>
		inline void InternalReset(size_t newSize = 0) noexcept {
			if (value) {
				for (size_t i = size; i-- > 0;) traits::destroy(alloc, value + i);
				alloc.deallocate(value, size);
				value = nullptr;
				size = 0;
			}
			if (newSize) {
				size = newSize;
				value = alloc.allocate(size);
				for (size_t i = 0; i < size; ++i) traits::construct(alloc, value + i, str_t('\0'));
			}
		}
	public:/* 文字配列取得 */
		/// <summary>
		/// 文字型配列を取得する(const版)
		/// </summary>
		const str_t* c_str() const noexcept { return value; }
		/// <summary>
		/// 文字型配列を取得する
		/// </summary>
		str_t* c_str() noexcept { return value; }
	public:/* 文字列操作(const) */
		/// <summary>
		/// 指定した文字列を先頭から検索する
		/// </summary>
		/// <param name="src">検索する文字列</param>
		/// <param name="pos">検索を始めるインデックス</param>
		/// <returns>
		/// 指定した文字列と最初に一致した位置(先頭のインデックス)。
		/// 一致する部分が存在しない場合、StringBlock::nposを返す
		/// </returns>
		size_t find(const StringBlock<str_t>& src, size_t pos = 0) const noexcept {
			const size_t selfLen = Length();
			const size_t srcLen = src.Length();
			if (selfLen < srcLen) return npos;
			const size_t searchEnd = selfLen - srcLen;
			const str_t* srcValue = src.c_str();
			for (size_t i = pos; i <= searchEnd; ++i) {
				if (value[i] != srcValue[0]) continue;
				size_t cur = 1;
				for (; cur < srcLen; ++cur) {
					if (value[i + cur] != srcValue[cur]) break;
				}
				if (cur == srcLen) return i;
			}
			return npos;
		}
		/// <summary>
		/// 指定した文字列を末尾から検索する
		/// </summary>
		/// <param name="src">検索する文字列</param>
		/// <param name="pos">検索を始めるインデックス。これは文字列の先頭となるインデックスである</param>
		/// <returns>
		/// 指定した文字列と最初に一致した位置(先頭のインデックス)。
		/// 一致する部分が存在しない場合、StringBlock::nposを返す
		/// </returns>
		size_t rfind(const StringBlock<str_t>& src, size_t pos = npos) const noexcept {
			const size_t selfLen = Length();
			const size_t srcLen = src.Length();
			if (selfLen < srcLen) return npos;
			const size_t searchBegin = selfLen - srcLen;
			const str_t* srcValue = src.c_str();
			for (size_t i = (searchBegin < pos ? searchBegin : pos) + 1; i-- > 0;) {
				if (value[i] != srcValue[0]) continue;
				size_t cur = 1;
				for (; cur < srcLen; ++cur) {
					if (value[i + cur] != srcValue[cur]) break;
				}
				if (cur == srcLen) return i;
			}
			return npos;
		}
		/// <summary>
		/// 部分文字列を取得する
		/// </summary>
		/// <param name="pos">
		/// 取得する部分文字列の開始位置(インデックス)。
		/// この文字列のヌル終端文字のインデックス以上の値を指定した場合、必ず空文字列を返す
		/// </param>
		/// <param name="n">
		/// 取得する文字列の要素数。
		/// 0を指定した場合、必ず空文字列を返す。
		/// この文字列の要素数より大きい値を指定した場合、ヌル終端文字まで取得する
		/// </param>
		StringBlock<str_t> substr(size_t pos, size_t n = npos) const noexcept {
			if (!n || pos >= size - 1) return StringBlock<str_t>();
			else return StringBlock<str_t>(value + pos, n);
		}
	public:/* 文字列操作(非const) */
		/// <summary>
		/// 指定した文字列を指定した位置に書き込む
		/// </summary>
		/// <param name="src">書き込む文字列の先頭を指すポインタ</param>
		/// <param name="n">書き込む文字数</param>
		/// <param name="pos">書き込みを始めるこの文字列のインデックス</param>
		StringBlock& strcpy(const str_t* src, size_t n, size_t pos) noexcept {
			//範囲外への書き込みは禁止
			if (pos >= size) return *this;
			//実際に書き込む文字数
			size_t count = size - pos - 1;
			if (n < count) count = n;
			//ヌル終端文字は書きこまない(文字列の長さは変わらない)
			for (size_t i = 0; i < count && src[i]; ++i) value[pos + i] = src[i];
			return *this;
		}
		/// <summary>
		/// 指定した文字を指定した位置に書き込む
		/// </summary>
		/// <param name="c">書き込む文字。一要素で表現できない文字は文字列版strcpyを使用する</param>
		/// <param name="pos">書き込み先のインデックス。ヌル終端文字は上書きできない</param>
		StringBlock& strcpy(const str_t c, size_t pos) noexcept {
			if (pos < size - 1) value[pos] = c;
			return *this;
		}
	public:/* 拡張文字列操作(const) */
		/// <summary>
		/// 文字列の長さ(ヌル終端文字を除く要素数)を取得する。
		/// 複数の要素で一文字を表す文字型の場合、文字数と一致しない可能性がある
		/// </summary>
		/// <returns>文字列がヌル終端のとき、戻り値はヌル終端文字のインデックスに等しい</returns>
		size_t Length() const noexcept {
			for (size_t i = 0; i < size; ++i) {
				if (value[i] == str_t('\0')) return i;
			}
			return size;
		}
		/// <summary>
		/// 内部に確保されている要素数を取得する
		/// </summary>
		size_t Capacity() const noexcept { return size; }
		/// <summary>
		/// この文字列が空文字列か調べる
		/// </summary>
		bool IsNullOrEmpty() const noexcept { return !(value && size && value[0]); }
		/// <summary>
		/// 指定した部分文字列をすべて置換した文字列を取得する
		/// </summary>
		/// <param name="src">検索する文字列</param>
		/// <param name="dst">置換後の文字列</param>
		/// <returns>この文字列の検索文字列と一致する部分をすべて置換した文字列</returns>
		StringBlock<str_t> Replace(const StringBlock<str_t>& src, const StringBlock<str_t>& dst) const noexcept {
			const size_t srcLen = src.Length();
			const size_t dstLen = dst.Length();
			//検索結果の配列
			VectorBase<size_t> vec;
			size_t pos = 0;
			while ((pos = find(src, pos)) != npos) {
				vec.Add(pos);
				pos += srcLen;
			}
			//一つも見つからない場合、置換しない
			if (!vec.Count()) return *this;
			//要素数は{元の文字列の要素数 - (置換回数 * 置換文字列の要素数の差) + ヌル終端文字 }
			StringBlock<str_t> ret = CreateStringBlock(Length() + vec.Count() * (dstLen - srcLen) + 1);
			size_t selfCur = 0;
			size_t retCur = 0;
			for (size_t i : vec) {
				if (retCur < i) {
					ret.strcpy(value + selfCur, i - selfCur, retCur);
					retCur += i - selfCur;
					selfCur = i;
				}
				ret.strcpy(dst.value, dstLen, retCur);
				retCur += dstLen;
				selfCur += srcLen;
			}
			if (value[selfCur]) ret.strcpy(value + selfCur, npos, retCur);
			return ret;
		}
	public:
		StringBlock<str_t> operator[](const size_t i) const noexcept { return Length() <= i ? Empty() : StringBlock(value[i]); }
		StringBlock<str_t>& operator=(const StringBlock<str_t>& rhs) noexcept {
			if (this == &rhs) return *this;
			InternalReset(rhs.size);
			for (size_t i = 0; i < size; ++i) value[i] = rhs.value[i];
			return *this;
		}
		StringBlock<str_t>& operator=(StringBlock<str_t>&& rhs) noexcept {
			if (this == &rhs) return *this;
			InternalReset();
			size = rhs.size;
			value = rhs.value;
			rhs.size = 0;
			rhs.value = nullptr;
			return *this;
		}
		StringBlock<str_t> operator+(const StringBlock<str_t>& rhs) const noexcept {
			const size_t len = Length();
			const size_t rlen = rhs.Length();
			StringBlock<str_t> ret = CreateStringBlock(len + rlen + 1);
			for (size_t i = 0; i < len; ++i) ret.value[i] = value[i];
			for (size_t i = 0; i < rlen; ++i) ret.value[len + i] = rhs.value[i];
			ret.value[len + rlen] = '\0';
			return ret;
		}
		StringBlock<str_t>& operator+=(const StringBlock<str_t>& rhs) noexcept { return *this = *this + rhs; }
		bool operator==(const StringBlock<str_t>& rhs) const noexcept {
			size_t len = Length(), rhsLen = rhs.Length();
			if (len != rhsLen) return false;
			const str_t* rdata = rhs.c_str();
			for (size_t i = 0; i < len; ++i) if (value[i] != rdata[i]) return false;
			return true;
		}
		friend inline bool operator==(const str_t lhs, const StringBlock<str_t>& rhs) noexcept {
			return StringBlock<str_t>(lhs) == rhs;
		}
		bool operator!=(const StringBlock<str_t>& rhs) const noexcept { return !(*this == rhs); }
	private:
		/// <summary>
		/// 文字コードの配列を取得する
		/// </summary>
		VectorBase<Encoding::CodePoint> ToCodePoints() const noexcept {
			using namespace Encoding;
			VectorBase<CodePoint> ret;
			CodePoint point;
			size_t pos = 0;
			do {
				point = GetCodePoint(value + pos);
				pos += point.count;
				ret.Add(point);
			} while (point.point != 0 && pos < size);
			return ret;
		}
	public:/* 文字型変換 */
		StringBlock<char8_t> ToU8StringBlock() const noexcept {
			if constexpr (Traits::is_same_v<char8_t, str_t>) return *this;
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<char8_t>(Encoding::ToU8StringBlock(tmp.Items(), tmp.Count()));
			}
		}
		StringBlock<char16_t> ToU16StringBlock() const noexcept {
			if constexpr (Traits::is_same_v<char16_t, str_t>) return *this;
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<char16_t>(Encoding::ToU16StringBlock(tmp.Items(), tmp.Count()));
			}
		}
		StringBlock<char32_t> ToU32StringBlock() const noexcept {
			if constexpr (Traits::is_same_v<char32_t, str_t>) return *this;
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<char32_t>(Encoding::ToU32StringBlock(tmp.Items(), tmp.Count()));
			}
		}
		StringBlock<wchar_t> ToWideStringBlock() const noexcept {
			if constexpr (Traits::is_same_v<wchar_t, str_t>) return *this;
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<wchar_t>(Encoding::ToWideStringBlock(tmp.Items(), tmp.Count()));
			}
		}
		StringBlock<char> ToMultiByteStringBlock() const noexcept {
			if constexpr (Traits::is_same_v<char, str_t>) return *this;
			else {
				VectorBase<Encoding::CodePoint> tmp = ToCodePoints();
				return StringBlock<char>(Encoding::ToMultiByteStringBlock(tmp.Items(), tmp.Count()));
			}
		}
	};

	//g++ではStringBlock<str_t>::allocがUndefined Referenceとなってしまうため、すべて実体化しておく。
	template class StringBlock<char>;
	template class StringBlock<char8_t>;
	template class StringBlock<char16_t>;
	template class StringBlock<char32_t>;
	template class StringBlock<wchar_t>;

	template<Traits::Concepts::CCharType str_t>
	std::ostream& operator<<(std::ostream& os, const StringBlock<str_t>& str) noexcept {
		return (os << str.ToMultiByteStringBlock().c_str());
	}
}
