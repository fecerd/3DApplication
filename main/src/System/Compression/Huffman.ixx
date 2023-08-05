export module Huffman;
import Objects;
import Vector;
import BitStream;

namespace System::Compression {
	enum class HuffmanType : uint8_t {
		NoCompression = 0,
		FixedHuffmanCode = 1,
		DynamicHuffmanCode = 2,
		Reserved = 3
	};

	class Deflate {
	public:
		static constexpr uint8_t GetLengthExCount(uint16_t value) noexcept {
			if (value <= 264 || value == 285) return 0;
			else return (value - 265) / 4 + 1;
		}
		static constexpr uint16_t GetLength(uint16_t value) noexcept {
			if (value <= 256) return 0;
			else if (value <= 272) {
				if (value <= 264) return value - 257 + 3;
				else if (value <= 268) return (value - 265) * 2 + 11;
				else return (value - 269) * 4 + 19;
			}
			else if (value <= 284) {
				if (value <= 276) return (value - 273) * 8 + 35;
				else if (value <= 280) return (value - 277) * 16 + 67;
				else return (value - 281) * 32 + 131;
			}
			else return 258;
		}
		static constexpr uint16_t GetDistance(uint8_t distanceValue) noexcept {
			if (distanceValue <= 3) return distanceValue + 1;
			else {
				uint8_t half = distanceValue >> 1;
				return (distanceValue - (half << 1)) * (1 << (half - 1)) + (1 << half) + 1;
			}
		}
		static constexpr uint8_t GetDistanceExCount(uint8_t distanceValue) noexcept {
			if (distanceValue <= 3) return 0;
			else return (distanceValue - 4) / 2 + 1;
		}
	};

	struct HuffmanTree {
		HuffmanTree* zero = nullptr;
		HuffmanTree* one = nullptr;
		uint16_t data = 0;
	public:
		static void Clear(HuffmanTree* root) noexcept {
			if (root) {
				if (root->zero) Clear(root->zero);
				if (root->one) Clear(root->one);
				delete root;
			}
		}
	};
}

export namespace System::Compression {
	class HuffmanDecoder {
	private:
		static HuffmanTree* GetHuffmanTree(BitStream& bits, uint16_t count, HuffmanTree* tableTree) noexcept {
			if (count > 286) count = 286;
			uint8_t codeLength[286]{};	//codeLength[n] : 値nの符号長
			uint8_t codeLengthCount[16]{};	//符号長ごとの個数
			//ランレングス符号化された値/符号長表を解析
			for (uint16_t i = 0; i < count; ++i) {
				HuffmanTree* ptr = tableTree;
				while (ptr->zero) {
					if (bits.Get(true)) ptr = ptr->one;
					else ptr = ptr->zero;
				}
				uint8_t len = static_cast<uint8_t>(ptr->data);
				if (len >= 16) {
					uint8_t exBitCount = 0;
					if (len == 18) exBitCount = 7;
					else if (len == 17) exBitCount = 3;
					else exBitCount = 2;
					uint8_t exBit = bits.Get<uint8_t>(exBitCount, true, true);
					if (len == 17) i += exBit + 2;
					else if (len == 18) i += exBit + 10;
					else if (i) {
						uint8_t repeat = codeLength[i - 1];
						for (uint8_t j = 0, end = exBit + 3; j < end; ++j) codeLength[i + j] = repeat;
						codeLengthCount[repeat] += exBit + 3;
						i += exBit + 2;
					}
				}
				else {
					codeLength[i] = len;
					++codeLengthCount[len];
				}
			}
			codeLengthCount[0] = 0;	//符号長0の個数は無視する
			uint16_t nextCode[16]{};	//nextCode[n] : 符号長nの符号の値
			uint16_t code = 0;
			for (uint8_t i = 1; i < 16; ++i) {
				code = (code + codeLengthCount[i - 1]) << 1;
				nextCode[i] = code;
			}
			HuffmanTree* root = new HuffmanTree();
			for (uint16_t i = 0; i < count; ++i) {
				if (codeLength[i] > 0) {
					HuffmanTree* ptr = root;
					uint16_t mask = 1 << (codeLength[i] - 1);
					uint16_t code = nextCode[codeLength[i]]++;
					while (mask) {
						if (code & mask) {
							if (!ptr->one) ptr->one = new HuffmanTree();
							ptr = ptr->one;
						}
						else {
							if (!ptr->zero) ptr->zero = new HuffmanTree();
							ptr = ptr->zero;
						}
						mask >>= 1;
					}
					ptr->data = i;
				}
			}
			return root;
		}
		static HuffmanTree* GetTableTree(BitStream& bits, uint8_t hclen) noexcept {
			//符号表の符号表の符号長の並び順
			const uint8_t codeLengthOrder[19] = {
				16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
			};
			uint8_t codeLengthCode[19]{};	//codeLengthCode[n] : 値nの符号長
			for (uint8_t i = 0; i < hclen; ++i) codeLengthCode[codeLengthOrder[i]] = bits.Get<uint8_t>(3, true, true);
			const uint8_t maxLength = 7;	//符号長の最大値
			uint16_t codeLengthCount[8]{};	//符号長ごとの個数
			uint16_t nextCode[8]{};	//nextCode[n] : 符号長nの符号の値
			for (uint8_t i = 0; i < 19; ++i) ++codeLengthCount[codeLengthCode[i]];
			codeLengthCount[0] = 0;	//符号長0は無視する
			uint16_t code = 0;
			for (uint8_t i = 1; i <= maxLength; ++i) {
				code = (code + codeLengthCount[i - 1]) << 1;
				nextCode[i] = code;
			}
			uint16_t codeCode[19]{};	//codeCode[n] : 値nの符号
			for (uint8_t i = 0; i < 19; ++i) {
				uint8_t len = codeLengthCode[i];
				if (len > 0) codeCode[i] = nextCode[len]++;
			}
			HuffmanTree* root = new HuffmanTree();
			HuffmanTree* ptr = nullptr;
			for (uint8_t i = 0; i < 19; ++i) {
				if (codeLengthCode[i] > 0) {
					ptr = root;
					uint16_t mask = 1 << (codeLengthCode[i] - 1);
					while (mask) {
						if (codeCode[i] & mask) {
							if (!ptr->one) ptr->one = new HuffmanTree();
							ptr = ptr->one;
						}
						else {
							if (!ptr->zero) ptr->zero = new HuffmanTree();
							ptr = ptr->zero;
						}
						mask >>= 1;
					}
					ptr->data = i;
				}
			}
			return root;
		}
	private:
		inline static void DecodeFixedHuffman_Internal(BitStream& bits, uint16_t value, Vector<uint8_t>& ret) noexcept {
			uint8_t lengthExCount = Deflate::GetLengthExCount(value);
			uint8_t lengthEx = 0;
			if (lengthExCount) lengthEx = bits.Get<uint8_t>(lengthExCount, true, true);
			uint8_t distanceValue = bits.GetReverseBit<uint8_t>(5, true, true);
			uint8_t distanceExCount = Deflate::GetDistanceExCount(distanceValue);
			uint16_t distanceEx = 0;
			if (distanceExCount) distanceEx = bits.Get<uint16_t>(distanceExCount, true, true);
			uint16_t distance = Deflate::GetDistance(distanceValue) + distanceEx;
			const size_t count = ret.Count();
			if (distance != 0 && count >= distance) ret.CopyRange(count - distance, Deflate::GetLength(value) + lengthEx);
		}
	private:
		static void DecodeFixedHuffman(BitStream& bits, Vector<uint8_t>& ret, bool& isFinalBlock) noexcept {
			while (!bits.EndOfStream()) {
				uint16_t code = bits.GetReverseBit<uint16_t>(7, true, true);
				//7bit
				if (code <= 0b10111) {
					//256(ブロックの終端)
					if (code == 0) {
						if (isFinalBlock) return;
						isFinalBlock = bits.Get(true);
						HuffmanType type = static_cast<HuffmanType>(bits.Get<uint8_t>(2, true, true));
						if (type == HuffmanType::FixedHuffmanCode) DecodeFixedHuffman(bits, ret, isFinalBlock);
						else if (type == HuffmanType::DynamicHuffmanCode) DecodeDynamicHuffman(bits, ret, isFinalBlock);
						return;
					}
					uint16_t value = 256 + code;
					DecodeFixedHuffman_Internal(bits, value, ret);
				}
				else {
					//8bit
					code = (code << 1) | (bits.Get(true) ? 1 : 0);
					if (code <= 0b10111111) {
						ret.Add(static_cast<uint8_t>(code - 0b110000));
					}
					else if (code <= 0b11000111) {
						uint16_t value = 280 + code - 0b11000000;
						DecodeFixedHuffman_Internal(bits, value, ret);
					}
					else {
						//9bit
						code = (code << 1) | (bits.Get(true) ? 1 : 0);
						ret.Add(static_cast<uint8_t>(144 + code - 0b110010000));
					}
				}
			}
		}
		static void DecodeDynamicHuffman(BitStream& bits, Vector<uint8_t>& ret, bool& isFinalBlock) noexcept {
			uint16_t hlit = bits.Get<uint16_t>(5, true, true) + 257;
			uint8_t hdist = bits.Get<uint8_t>(5, true, true) + 1;
			uint8_t hclen = bits.Get<uint8_t>(4, true, true) + 4;
			HuffmanTree* tableTree = GetTableTree(bits, hclen);
			HuffmanTree* valueTree = GetHuffmanTree(bits, hlit, tableTree);
			HuffmanTree* distanceTree = GetHuffmanTree(bits, hdist, tableTree);
			HuffmanTree::Clear(tableTree);
			while (!bits.EndOfStream()) {
				HuffmanTree* valuePtr = valueTree;
				while (valuePtr->zero) {
					if (bits.Get(true)) valuePtr = valuePtr->one;
					else valuePtr = valuePtr->zero;
				}
				uint16_t value = valuePtr->data;
				if (value <= 256) {
					if (value <= 255) ret.Add(static_cast<uint8_t>(value));
					else {
						HuffmanTree::Clear(valueTree);
						HuffmanTree::Clear(distanceTree);
						if (isFinalBlock) return;
						isFinalBlock = bits.Get(true);
						HuffmanType type = static_cast<HuffmanType>(bits.Get<uint8_t>(2, true, true));
						if (type == HuffmanType::FixedHuffmanCode) DecodeFixedHuffman(bits, ret, isFinalBlock);
						else if (type == HuffmanType::DynamicHuffmanCode) DecodeDynamicHuffman(bits, ret, isFinalBlock);
						return;
					}
				}
				else {
					uint8_t lengthExCount = Deflate::GetLengthExCount(value);
					uint8_t lengthEx = 0;
					if (lengthExCount) lengthEx = bits.Get<uint8_t>(lengthExCount, true, true);
					HuffmanTree* distancePtr = distanceTree;
					while (distancePtr->zero) {
						if (bits.Get(true)) distancePtr = distancePtr->one;
						else distancePtr = distancePtr->zero;
					}
					uint8_t distanceExCount = Deflate::GetDistanceExCount(static_cast<uint8_t>(distancePtr->data));
					uint16_t distanceEx = 0;
					if (distanceExCount) distanceEx = bits.Get<uint16_t>(distanceExCount, true, true);
					uint16_t distance = Deflate::GetDistance(static_cast<uint8_t>(distancePtr->data)) + distanceEx;
					const size_t count = ret.Count();
					if (distance != 0 && count >= distance) ret.CopyRange(count - distance, Deflate::GetLength(value) + lengthEx);
				}
			}
		}
	public:
		static Vector<uint8_t> Decode(uint8_t* data, size_t byteCount, size_t reserved) noexcept {
			Vector<uint8_t> ret;
			ret.Reserve(reserved);
			BitStream bits(data, byteCount);
			bool isFinalBlock = bits.Get(true);
			HuffmanType type = static_cast<HuffmanType>(bits.Get<uint8_t>(2, true, true));
			if (type == HuffmanType::FixedHuffmanCode) DecodeFixedHuffman(bits, ret, isFinalBlock);
			else if (type == HuffmanType::DynamicHuffmanCode) DecodeDynamicHuffman(bits, ret, isFinalBlock);
			return ret;
		}
	};
}