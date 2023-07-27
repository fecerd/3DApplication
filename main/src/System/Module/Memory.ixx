module;
#include <atomic>
export module Memory;
import CSTDINT;
import Traits;
import Math;
import <fstream>;
using namespace System::Traits;

//�������R�s�[/�X���b�v
export namespace System::Memory {
	/// <summary>
	/// �P���ȃ������R�s�[���s��
	/// </summary>
	/// <param name="src">�R�s�[��</param>
	/// <param name="dst">�R�s�[��</param>
	/// <param name="byteCount">
	/// �R�s�[����o�C�g���B
	/// src�Adst�Ƃ���[0, byteCount - 1]���L���Ȕ͈͂ł���K�v������B
	/// </param>
	/* dst��src�̈ʒu�����ւ����̂ŁA�Ăяo�������ׂďC������(unused�ɂ���ăR���p�C���G���[���o��) */
	inline void Copy(void* dst, const void* src, size_t byteCount, bool unused) noexcept;
	/// <summary>
	/// CopyRange�p�͈̔͂�\���\����
	/// </summary>
	struct BufferRange {
		size_t elementSize = 1;	//�f�[�^�̗v�f�T�C�Y
		uint32_t width = 1;			//�f�[�^�S�̂̕�
		uint32_t height = 1;		//�f�[�^�S�̂̍���
		uint32_t startX = 0;			//�R�s�[���J�n����v�f��X���W
		uint32_t startY = 0;			//�R�s�[���J�n����v�f��Y���W
		uint32_t rangeW = 0;		//�R�s�[�Ɏg�p����͈́B0�̏ꍇ�Awidth�Ɠ��l
		uint32_t rangeH = 0;		//�R�s�[�Ɏg�p����͈́B0�̏ꍇ�Aheight�Ɠ��l
	};

	/// <summary>
	/// �w�肵���͈͊ԂŃ������R�s�[���s��
	/// </summary>
	/// <param name="dst">�R�s�[��</param>
	/// <param name="dstRange">�R�s�[��͈̔�</param>
	/// <param name="src">�R�s�[��</param>
	/// <param name="srcRange">�R�s�[���͈̔�</param>
	/* dst��src�̈ʒu�����ւ����̂ŁA�Ăяo�������ׂďC������(unused�ɂ���ăR���p�C���G���[���o��) */
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
	/// �����̃R�s�[��ɑ΂��āA�w�肵���͈͊Ԃ̃������R�s�[���s��
	/// </summary>
	/// <param name="dst">�R�s�[��̔z��</param>
	/// <param name="dstArrayCount">�R�s�[��̔z��̗v�f��</param>
	/// <param name="dstRange">�R�s�[��͈̔�</param>
	/// <param name="src">�R�s�[��</param>
	/// <param name="srcRange">�R�s�[���͈̔�</param>
	/* dst��src�̈ʒu�����ւ����̂ŁA�Ăяo�������ׂďC������(unused�ɂ���ăR���p�C���G���[���o��) */
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
	/// �����̃����������ւ���
	/// </summary>
	/// <typeparam name="T">�C�ӂ̌^</typeparam>
	/// <param name="x">����ւ���ϐ��ւ̎Q��</param>
	/// <param name="y">����ւ���ϐ��ւ̎Q��</param>
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

//�������A���C�������g
export namespace System::Memory {
	/// <summary>
	/// �A���C�������g���������T�C�Y���擾����
	/// </summary>
	/// <param name="size">��������T�C�Y</param>
	/// <param name="alignment">�A���C�������g</param>
	template<Traits::Concepts::CUnsigned T>
	constexpr T GetAlignedSize(T size, T alignment) noexcept
	{
		if (!alignment) return size;
		if (!(alignment & (alignment - 1))) return static_cast<T>((size + (alignment - 1)) & ~(alignment - 1));
		else return size + alignment - (size % alignment);
	}
}

//�r�b�g��r
export namespace System::Memory {
	/// <summary>
	/// ��̃������̈�̃r�b�g�\������v���邩���ׂ�
	/// </summary>
	/// <param name="src1">��O�����Ŏw�肷��o�C�g�������̈�ւ̃|�C���^</param>
	/// <param name="src2">��O�����Ŏw�肷��o�C�g�������̈�ւ̃|�C���^</param>
	/// <param name="n">���ׂ�̈�̃o�C�g��</param>
	/// <returns>���S��v�����Ƃ��Atrue</returns>
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

//�G���f�B�A���֌W
export namespace System::Memory {
	/// <summary>
	/// �����𔽑΂̃G���f�B�A���ŉ��߂����l���擾����
	/// </summary>
	/// <typeparam name="T">�C�ӂ̌^</typeparam>
	/// <param name="value">�G���f�B�A���𔽓]������l</param>
	/// <returns>�G���f�B�A�������]�����l</returns>
	template <typename T>
	inline T EndianSwap(T value) noexcept {
		int_t<false, sizeof(T)>* tmp = reinterpret_cast<int_t<false, sizeof(T)>*>(&value);
		for (size_t i = 0, end = sizeof(T) / 2; i < end; ++i) Swap(*(reinterpret_cast<unsigned char*>(tmp) + i), *(reinterpret_cast<unsigned char*>(tmp) + sizeof(T) - i - 1));
		return *reinterpret_cast<T*>(tmp);
	}
	/// <summary>
	/// �������g���G���f�B�A�������ׂ�
	/// </summary>
	/// <returns>�������g���G���f�B�A���̏ꍇ�Atrue</returns>
	inline bool isLittleEndian() noexcept {
		int tmp = 1;
		return *reinterpret_cast<char*>(&tmp) == 1;
	}
	/// <summary>
	/// �������̃G���f�B�A�������ɍ��킹��
	/// </summary>
	/// <typeparam name="T">�C�ӂ̌^</typeparam>
	/// <param name="value">�G���f�B�A����ϊ�����l</param>
	/// <param name="isLittle">�������̃G���f�B�A��</param>
	/// <returns>���̃G���f�B�A���ɍ��킹���l�B�����������ƈ�v����ꍇ�A���̂܂ܑ�������Ԃ�</returns>
	template <typename T> inline T ByteOrder(const T& value, const bool isLittle = false) noexcept { return isLittle == isLittleEndian() ? value : EndianSwap(value); }
}

//�t�@�C���ǂݍ��ݗp
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

//�t�@�C���������ݗp
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

//�t�@�C���I��
export namespace System::Memory {
	/// <summary>
	/// �t�@�C���X�g���[������āAbool�l��Ԃ�
	/// </summary>
	/// <param name="file">����t�@�C���X�g���[���ւ̎Q��</param>
	/// <param name="returnValue">���̊֐����Ԃ�bool�l</param>
	/// <returns>�������Ŏw�肵��bool�l</returns>
	inline bool Close(std::fstream& file, const bool returnValue) noexcept { file.close(); return returnValue; }
}

//�A�g�~�b�N�֘A
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
