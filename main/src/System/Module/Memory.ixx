module;
#include <atomic>
export module Memory;
import CSTDINT;
import Traits;
import Math;
import <fstream>;
using namespace System::Traits;

//メモリコピー/スワップ
export namespace System::Memory {
	/// <summary>
	/// 単純なメモリコピーを行う
	/// </summary>
	/// <param name="src">コピー元</param>
	/// <param name="dst">コピー先</param>
	/// <param name="byteCount">
	/// コピーするバイト数。
	/// src、dstともに[0, byteCount - 1]が有効な範囲である必要がある。
	/// </param>
	/* dstとsrcの位置を入れ替えたので、呼び出しをすべて修正する(unusedによってコンパイルエラーを出す) */
	inline void Copy(void* dst, const void* src, size_t byteCount, bool unused) noexcept;
	/// <summary>
	/// CopyRange用の範囲を表す構造体
	/// </summary>
	struct BufferRange {
		size_t elementSize = 1;	//データの要素サイズ
		uint32_t width = 1;			//データ全体の幅
		uint32_t height = 1;		//データ全体の高さ
		uint32_t startX = 0;			//コピーを開始する要素のX座標
		uint32_t startY = 0;			//コピーを開始する要素のY座標
		uint32_t rangeW = 0;		//コピーに使用する範囲。0の場合、widthと同値
		uint32_t rangeH = 0;		//コピーに使用する範囲。0の場合、heightと同値
	};

	/// <summary>
	/// 指定した範囲間でメモリコピーを行う
	/// </summary>
	/// <param name="dst">コピー先</param>
	/// <param name="dstRange">コピー先の範囲</param>
	/// <param name="src">コピー元</param>
	/// <param name="srcRange">コピー元の範囲</param>
	/* dstとsrcの位置を入れ替えたので、呼び出しをすべて修正する(unusedによってコンパイルエラーを出す) */
	inline bool CopyRange(void* dst, const BufferRange& dstRange, const void* src, const BufferRange& srcRange, bool unused) noexcept {
		if (!src || !dst) return false;
		if (srcRange.startX >= srcRange.width) return false;
		if (dstRange.startX >= dstRange.width) return false;
		const size_t srcWidthByte = srcRange.elementSize * srcRange.width;
		const size_t dstWidthByte = dstRange.elementSize * dstRange.width;
		if (!srcWidthByte || !dstWidthByte || !srcRange.height || !dstRange.height) return false;
		const size_t srcStartOffset = srcRange.startX * srcRange.elementSize;
		const size_t dstStartOffset = dstRange.startX * dstRange.elementSize;

		const size_t srcCopyByteCount = srcRange.elementSize
			* Math::Min(
				srcRange.rangeW ? srcRange.rangeW : srcRange.width,
				srcRange.width - srcRange.startX
			);
		const size_t dstCopyByteCount = dstRange.elementSize
			* Math::Min(
				dstRange.rangeW ? dstRange.rangeW : dstRange.width,
				dstRange.width - dstRange.startX
			);
		const size_t copyByteCount = Math::Min(srcCopyByteCount, dstCopyByteCount);

		const uint32_t srcCopyHeight = Math::Min(
			srcRange.rangeH ? srcRange.rangeH : srcRange.height,
			srcRange.height - srcRange.startY
		);
		const uint32_t dstCopyHeight = Math::Min(
			dstRange.rangeH ? dstRange.rangeH : dstRange.height,
			dstRange.height - dstRange.startY
		);
		const uint32_t copyHeight = Math::Min(srcCopyHeight, dstCopyHeight);

		const uint8_t* src0 = static_cast<const uint8_t*>(src) + (srcRange.startY * srcWidthByte);
		uint8_t* dst0 = static_cast<uint8_t*>(dst) + (dstRange.startY * dstWidthByte);
		for (uint32_t y = 0; y < copyHeight; ++y) {
			Memory::Copy(
				dst0 + dstStartOffset,
				src0 + srcStartOffset,
				copyByteCount, true
			);
			src0 += srcWidthByte;
			dst0 += dstWidthByte;
		}
		return true;
	}

	/// <summary>
	/// 複数のコピー先に対して、指定した範囲間のメモリコピーを行う
	/// </summary>
	/// <param name="dst">コピー先の配列</param>
	/// <param name="dstArrayCount">コピー先の配列の要素数</param>
	/// <param name="dstRange">コピー先の範囲</param>
	/// <param name="src">コピー元</param>
	/// <param name="srcRange">コピー元の範囲</param>
	/* dstとsrcの位置を入れ替えたので、呼び出しをすべて修正する(unusedによってコンパイルエラーを出す) */
	inline bool CopyRanges(void* const* dstArray, uint32_t dstArrayCount, const BufferRange& dstRange, const void* src, const BufferRange& srcRange, bool unused) noexcept {
		if (!src || !dstArray || !dstArrayCount) return false;
		if (srcRange.startX >= srcRange.width) return false;
		if (dstRange.startX >= dstRange.width) return false;
		const size_t srcWidthByte = srcRange.elementSize * srcRange.width;
		const size_t dstWidthByte = dstRange.elementSize * dstRange.width;
		if (!srcWidthByte || !dstWidthByte || !srcRange.height || !dstRange.height) return false;
		const size_t srcStartOffset = srcRange.startX * srcRange.elementSize;
		const size_t dstStartOffset = dstRange.startX * dstRange.elementSize;

		const size_t srcCopyByteCount = srcRange.elementSize
			* Math::Min(
				srcRange.rangeW ? srcRange.rangeW : srcRange.width,
				srcRange.width - srcRange.startX
			);
		const size_t dstCopyByteCount = dstRange.elementSize
			* Math::Min(
				dstRange.rangeW ? dstRange.rangeW : dstRange.width,
				dstRange.width - dstRange.startX
			);
		const size_t copyByteCount = Math::Min(srcCopyByteCount, dstCopyByteCount);

		const uint32_t srcCopyHeight = Math::Min(
			srcRange.rangeH ? srcRange.rangeH : srcRange.height,
			srcRange.height - srcRange.startY
		);
		const uint32_t dstCopyHeight = Math::Min(
			dstRange.rangeH ? dstRange.rangeH : dstRange.height,
			dstRange.height - dstRange.startY
		);
		const uint32_t copyHeight = Math::Min(srcCopyHeight, dstCopyHeight);

		const uint8_t* src0 = static_cast<const uint8_t*>(src) + (srcRange.startY * srcWidthByte);
		size_t dstOffset = dstRange.startY * dstWidthByte + dstStartOffset;
		for (uint32_t y = 0; y < copyHeight; ++y) {
			const uint8_t* srcTmp = src0 + srcStartOffset;
			for (uint32_t i = 0; i < dstArrayCount; ++i) {
				Memory::Copy(
					static_cast<uint8_t*>(dstArray[i]) + dstOffset,
					srcTmp,
					copyByteCount, true
				);
			}
			src0 += srcWidthByte;
			dstOffset += dstWidthByte;
		}
		return true;
	}

	/// <summary>
	/// 引数のメモリを入れ替える
	/// </summary>
	/// <typeparam name="T">任意の型</typeparam>
	/// <param name="x">入れ替える変数への参照</param>
	/// <param name="y">入れ替える変数への参照</param>
	template <typename T>
	inline void Swap(T& x, T& y) noexcept {
		auto tx = reinterpret_cast<int_t<false, sizeof(T)>*>(&x);
		auto ty = reinterpret_cast<int_t<false, sizeof(T)>*>(&y);
		if (*tx == *ty) return;
		*tx = *tx ^ *ty;
		*ty = *tx ^ *ty;
		*tx = *tx ^ *ty;
	}
}

//メモリアラインメント
export namespace System::Memory {
	/// <summary>
	/// アラインメント調整したサイズを取得する
	/// </summary>
	/// <param name="size">調整するサイズ</param>
	/// <param name="alignment">アラインメント</param>
	template<Traits::Concepts::CUnsigned T>
	constexpr T GetAlignedSize(T size, T alignment) noexcept
	{
		if (!alignment) return size;
		if (!(alignment & (alignment - 1))) return static_cast<T>((size + (alignment - 1)) & ~(alignment - 1));
		else return size + alignment - (size % alignment);
	}
}

//ビット比較
export namespace System::Memory {
	/// <summary>
	/// 二つのメモリ領域のビット表現が一致するか調べる
	/// </summary>
	/// <param name="src1">第三引数で指定するバイト数を持つ領域へのポインタ</param>
	/// <param name="src2">第三引数で指定するバイト数を持つ領域へのポインタ</param>
	/// <param name="n">調べる領域のバイト数</param>
	/// <returns>完全一致したとき、true</returns>
	inline bool Compare(const void* src1, const void* src2, size_t n) noexcept {
		const char* ptr1 = reinterpret_cast<const char*>(src1);
		const char* ptr2 = reinterpret_cast<const char*>(src2);
		while (n-- > 0) {
			if (*ptr1 != *ptr2) return false;
			ptr1++;
			ptr2++;
		}
		return true;
	}
}

//エンディアン関係
export namespace System::Memory {
	/// <summary>
	/// 引数を反対のエンディアンで解釈した値を取得する
	/// </summary>
	/// <typeparam name="T">任意の型</typeparam>
	/// <param name="value">エンディアンを反転させる値</param>
	/// <returns>エンディアンが反転した値</returns>
	template <typename T>
	inline T EndianSwap(T value) noexcept {
		int_t<false, sizeof(T)>* tmp = reinterpret_cast<int_t<false, sizeof(T)>*>(&value);
		for (size_t i = 0, end = sizeof(T) / 2; i < end; ++i) Swap(*(reinterpret_cast<unsigned char*>(tmp) + i), *(reinterpret_cast<unsigned char*>(tmp) + sizeof(T) - i - 1));
		return *reinterpret_cast<T*>(tmp);
	}
	/// <summary>
	/// 環境がリトルエンディアンか調べる
	/// </summary>
	/// <returns>環境がリトルエンディアンの場合、true</returns>
	inline bool isLittleEndian() noexcept {
		int tmp = 1;
		return *reinterpret_cast<char*>(&tmp) == 1;
	}
	/// <summary>
	/// 第一引数のエンディアンを環境に合わせる
	/// </summary>
	/// <typeparam name="T">任意の型</typeparam>
	/// <param name="value">エンディアンを変換する値</param>
	/// <param name="isLittle">第一引数のエンディアン</param>
	/// <returns>環境のエンディアンに合わせた値。第二引数が環境と一致する場合、そのまま第一引数を返す</returns>
	template <typename T> inline T ByteOrder(const T& value, const bool isLittle = false) noexcept { return isLittle == isLittleEndian() ? value : EndianSwap(value); }
}

//ファイル読み込み用
export namespace System::Memory {
	template <typename T>
	inline void ReadBE(std::fstream& file, T& dst) noexcept {
		file.read(reinterpret_cast<char*>(&dst), sizeof(dst));
		dst = Memory::ByteOrder(dst);
	}
	template<typename T, size_t N>
	inline void ReadBE(std::fstream& file, T(&dst)[N]) noexcept {
		file.read(reinterpret_cast<char*>(dst), N * sizeof(T));
		if constexpr (sizeof(T) != 1) {
			for (size_t i = 0; i < N; ++i) dst[i] = Memory::ByteOrder(dst[i]);
		}
	}
	template<typename T>
	inline void ReadBE(std::fstream& file, T* dst, const size_t N) noexcept {
		file.read(reinterpret_cast<char*>(dst), N * sizeof(T));
		if constexpr (sizeof(T) != 1) {
			for (size_t i = 0; i < N; ++i) dst[i] = Memory::ByteOrder(dst[i]);
		}
	}
	template <typename T>
	inline void ReadLE(std::fstream& file, T& dst) noexcept {
		file.read(reinterpret_cast<char*>(&dst), sizeof(dst));
		dst = Memory::ByteOrder(dst, true);
	}
	template<typename T, size_t N>
	inline void ReadLE(std::fstream& file, T(&dst)[N]) noexcept {
		file.read(reinterpret_cast<char*>(dst), N * sizeof(T));
		if constexpr (sizeof(T) != 1) {
			for (size_t i = 0; i < N; ++i) dst[i] = Memory::ByteOrder(dst[i], true);
		}
	}
	template<typename T>
	inline void ReadLE(std::fstream& file, T* dst, const size_t N) noexcept {
		file.read(reinterpret_cast<char*>(dst), N * sizeof(T));
		if constexpr (sizeof(T) != 1) {
			for (size_t i = 0; i < N; ++i) dst[i] = Memory::ByteOrder(dst[i], true);
		}
	}
}

//ファイル書き込み用
export namespace System::Memory {
	template<typename T>
	inline void WriteBE(std::fstream& file, const T& src) noexcept {
		if (Memory::isLittleEndian()) {
			T tmp = Memory::EndianSwap(src);
			file.write(reinterpret_cast<const char*>(&tmp), sizeof(T));
		}
		else file.write(reinterpret_cast<const char*>(&src), sizeof(src));
	}
	template<typename T, size_t N>
	inline void WriteBE(std::fstream& file, const T(&src)[N]) noexcept {
		if constexpr (sizeof(T) != 1) {
			if (Memory::isLittleEndian()) {
				for (size_t i = 0; i < N; ++i) {
					T tmp = Memory::EndianSwap(src[i]);
					file.write(reinterpret_cast<const char*>(&tmp), sizeof(T));
				}
				return;
			}
		}
		file.write(reinterpret_cast<const char*>(src), N * sizeof(T));
	}
	template<typename T>
	inline void WriteBE(std::fstream& file, const T* src, const size_t N) noexcept {
		if constexpr (sizeof(T) != 1) {
			if (Memory::isLittleEndian()) {
				for (size_t i = 0; i < N; ++i) {
					T tmp = Memory::EndianSwap(src[i]);
					file.write(reinterpret_cast<const char*>(&tmp), sizeof(T));
				}
				return;
			}
		}
		file.write(reinterpret_cast<const char*>(src), N * sizeof(T));
	}
	template<typename T>
	inline void WriteLE(std::fstream& file, const T& src) noexcept {
		if (!Memory::isLittleEndian()) {
			T tmp = Memory::EndianSwap(src);
			file.write(reinterpret_cast<const char*>(&tmp), sizeof(T));
		}
		else file.write(reinterpret_cast<const char*>(&src), sizeof(T));
	}
	template<typename T, size_t N>
	inline void WriteLE(std::fstream& file, const T(&src)[N]) noexcept {
		if constexpr (sizeof(T) != 1) {
			if (!Memory::isLittleEndian()) {
				for (size_t i = 0; i < N; ++i) {
					T tmp = Memory::EndianSwap(src[i]);
					file.write(reinterpret_cast<const char*>(&tmp), sizeof(T));
				}
				return;
			}
		}
		file.write(reinterpret_cast<const char*>(src), N * sizeof(T));
	}
	template<typename T>
	inline void WriteLE(std::fstream& file, const T* src, const size_t N) noexcept {
		if constexpr (sizeof(T) != 1) {
			if (!Memory::isLittleEndian()) {
				for (size_t i = 0; i < N; ++i) {
					T tmp = Memory::EndianSwap(src[i]);
					file.write(reinterpret_cast<const char*>(&tmp), sizeof(T));
				}
				return;
			}
		}
		file.write(reinterpret_cast<const char*>(src), N * sizeof(T));
	}
}

//ファイル終了
export namespace System::Memory {
	/// <summary>
	/// ファイルストリームを閉じて、bool値を返す
	/// </summary>
	/// <param name="file">閉じるファイルストリームへの参照</param>
	/// <param name="returnValue">この関数が返すbool値</param>
	/// <returns>第二引数で指定したbool値</returns>
	inline bool Close(std::fstream& file, const bool returnValue) noexcept { file.close(); return returnValue; }
}

//アトミック関連
export namespace System {
	template<class T>
	using Atomic = std::atomic<T>;
	using memory_order = std::memory_order;
	inline constexpr std::memory_order memory_order_relaxed = std::memory_order::relaxed;
	inline constexpr std::memory_order memory_order_consume = std::memory_order::consume;
	inline constexpr std::memory_order memory_order_acquire = std::memory_order::acquire;
	inline constexpr std::memory_order memory_order_release = std::memory_order::release;
	inline constexpr std::memory_order memory_order_acq_rel = std::memory_order::acq_rel;
	inline constexpr std::memory_order memory_order_seq_cst = std::memory_order::seq_cst;
}
