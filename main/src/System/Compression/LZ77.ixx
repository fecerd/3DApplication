export module LZ77;
import CSTDINT;
import Traits;
import Sorts;
import Vector;
using namespace System::Traits;

export namespace System::Compression {
	struct Node {
		size_t count;
		uint16_t value;
		uint8_t length;
	public:
		static void CalculateLength(Vector<Node>& nodes) noexcept;
	};

	struct Package {
		Vector<Node*> data;
		size_t count;
	public:
		Package(Node& node) noexcept {
			data.Add(&node);
			count = node.count;
		}
	public:
		Package& operator+=(const Package& rhs) noexcept {
			data.AddRange(rhs.data.Items(), rhs.data.Count());
			count += rhs.count;
			for (size_t i = 0, c = data.Count(); i < c; ++i) ++(data[i]->length);
			return *this;
		}
	};

	inline void Node::CalculateLength(Vector<Node>& nodes) noexcept {
		nodes.Sort(true, [](const Node& x, const Node& y) { return x.count <=> y.count; });
		const size_t nodeCount = nodes.Count();
		if (nodeCount < 3) return;
		Vector<Package*> packs;
		packs.Add(new Package(nodes[0]));
		packs.Add(new Package(nodes[1]));
		size_t next = 2;
		while (true) {
			if (packs.Count() < 2) {
				for (size_t i = packs.Count(); next < nodeCount && i < 2; ++i) packs.Add(new Package(nodes[next++]));
			}
			if (packs.Count() <= 2 && next >= nodeCount) break;
			Package* tmp = packs[0];
			*tmp += *packs[1];
			delete packs[1];
			packs.Remove(packs.GetIterator(1));
			packs.Remove(packs.GetIterator(0));
			for (size_t i = 0, c = packs.Count(); i < c; ++i) {
				if (tmp->count <= packs[i]->count) {
					packs.Insert(packs.GetIterator(i), tmp);
					tmp = nullptr;
					break;
				}
			}
			if (!tmp) continue;
			while (next < nodeCount && tmp->count > nodes[next].count) packs.Add(new Package(nodes[next++]));
			packs.Add(tmp);
		}
		for (size_t i = 0, c = packs.Count(); i < c; ++i) delete packs[i];
	}
}

export namespace System::Compression {
	struct LZ77Triple {
		size_t length = 0;
		size_t distance = 0;
		uint8_t c = 0;
	};

	struct LZ77ForDeflate {
		uint16_t data = 0;
		uint8_t lengthEx = 0;
		uint8_t lengthExCount = 0;
		uint8_t distance = 0;
		uint16_t distanceEx = 0;
		uint8_t distanceExCount = 0;
	public:
		static constexpr uint8_t GetLengthExCount(uint16_t value) noexcept {
			if (value <= 256) return 0;
			else if (value <= 264) return 0;
			else if (value <= 268) return 1;
			else if (value <= 272) return 2;
			else if (value <= 276) return 3;
			else if (value <= 280) return 4;
			else if (value <= 284) return 5;
			else return 0;
		}
		static constexpr uint8_t GetDistanceExCount(uint8_t distance) noexcept {
			if (distance <= 3) return 0;
			else return 1 + (distance - 4) / 2;
		}
	private:
		static constexpr uint16_t GetValueFromLength(uint16_t length, uint8_t& exBitCount, uint8_t& exBit) noexcept {
			if (length < 3) {
				exBitCount = 0;
				exBit = 0;
				return 256;
			}
			else if (length <= 10) {
				exBitCount = 0;
				exBit = 0;
				return 257 + length - 3;
			}
			else if (length <= 18) {
				exBitCount = 1;
				exBit = (length - 11) % 2;
				return 265 + (length - 11) / 2;
			}
			else if (length <= 34) {
				exBitCount = 2;
				exBit = (length - 19) % 4;
				return 269 + (length - 19) / 4;
			}
			else if (length <= 66) {
				exBitCount = 3;
				exBit = (length - 35) % 8;
				return 273 + (length - 35) / 8;
			}
			else if (length <= 130) {
				exBitCount = 4;
				exBit = (length - 67) % 16;
				return 277 + (length - 67) / 16;
			}
			else if (length <= 257) {
				exBitCount = 5;
				exBit = (length - 131) % 32;
				return 281 + (length - 131) / 32;
			}
			else {
				exBitCount = 0;
				exBit = 0;
				return 285;
			}
		}
		static constexpr uint8_t GetDistanceValueFromDistance(uint16_t distance, uint8_t& exBitCount, uint16_t& exBit) noexcept {
			if (distance <= 4) {
				exBitCount = 0;
				exBit = 0;
				return distance - 1;
			}
			else if (distance <= 8) {
				exBitCount = 1;
				exBit = (distance - 5) % 2;
				return 4 + (distance - 5) / 2;
			}
			else if (distance <= 128) {
				if (distance <= 16) {
					exBitCount = 2;
					exBit = (distance - 9) % 4;
					return 6 + (distance - 9) / 4;
				}
				else if (distance <= 32) {
					exBitCount = 3;
					exBit = (distance - 17) % 8;
					return 8 + (distance - 17) / 8;
				}
				else if (distance <= 64) {
					exBitCount = 4;
					exBit = (distance - 33) % 16;
					return 10 + (distance - 33) / 16;
				}
				else {
					exBitCount = 5;
					exBit = (distance - 65) % 32;
					return 12 + (distance - 65) / 32;
				}
			}
			else if (distance <= 2048) {
				if (distance <= 256) {
					exBitCount = 6;
					exBit = (distance - 129) % 64;
					return 14 + (distance - 129) / 64;
				}
				else if (distance <= 512) {
					exBitCount = 7;
					exBit = (distance - 257) % 128;
					return 16 + (distance - 257) / 128;
				}
				else if (distance <= 1024) {
					exBitCount = 8;
					exBit = (distance - 513) % 256;
					return 18 + (distance - 513) / 256;
				}
				else {
					exBitCount = 9;
					exBit = (distance - 1025) % 512;
					return 20 + (distance - 1025) / 512;
				}
			}
			else {
				if (distance <= 4096) {
					exBitCount = 10;
					exBit = (distance - 2049) % 1024;
					return 22 + (distance - 2049) / 1024;
				}
				else if (distance <= 8192) {
					exBitCount = 11;
					exBit = (distance - 4097) % 2048;
					return 24 + (distance - 4097) / 2048;
				}
				else if (distance <= 16384) {
					exBitCount = 12;
					exBit = (distance - 8193) % 4096;
					return 26 + (distance - 8193) / 4096;
				}
				else {
					exBitCount = 13;
					exBit = (distance - 16385) % 8192;
					return 28 + (distance - 16385) / 8192;
				}
			}
		}
	public:
		static constexpr uint16_t GetLengthFromValue(uint16_t data) noexcept {
			if (data <= 256) return 0;
			else if (data <= 264) return data - 257 + 3;
			else if (data <= 268) return (data - 265) * 2 + 11;
			else if (data <= 272) return (data - 269) * 4 + 19;
			else if (data <= 276) return (data - 273) * 8 + 35;
			else if (data <= 280) return (data - 277) * 16 + 67;
			else if (data <= 284) return (data - 281) * 32 + 131;
			else return 258;
		}
		static constexpr uint16_t GetDistanceFromDistanceValue(uint8_t distanceValue) noexcept {
			if (distanceValue <= 3) return distanceValue + 1;
			else {
				uint8_t half = distanceValue >> 1;
				return (distanceValue - (half << 1)) * (1 << (half - 1)) + (1 << half) + 1;
			}
		}
	private:
		static constexpr void SetValue(LZ77ForDeflate& x, uint16_t length, uint16_t distance) noexcept {
			x.data = GetValueFromLength(length, x.lengthExCount, x.lengthEx);
			x.distance = GetDistanceValueFromDistance(distance, x.distanceExCount, x.distanceEx);
		}
	public:
		static Vector<LZ77ForDeflate> ToLZ77ForDeflate(const uint8_t* rawData, size_t byteCount) noexcept {
			Vector<LZ77ForDeflate> ret;
			size_t slidingWindowBegin = 0;
			size_t slidingWindowEnd = 0;
			const size_t slidingWindowMax = 32768;
			for (size_t i = 0; i < byteCount; ++i) {
				uint16_t tLength = 0;
				uint16_t tDistance = 0;
				for (size_t j = slidingWindowBegin; j < slidingWindowEnd; ++j) {
					if (rawData[i] == rawData[j]) {
						uint16_t k = 1;
						for (; j + k < slidingWindowEnd && i + k < byteCount; ++k) if (rawData[i + k] != rawData[j + k]) break;
						if (k > 2 && tLength <= k) {
							tLength = k;
							tDistance = static_cast<uint16_t>(i - j);
						}
					}
				}
				LZ77ForDeflate tmp;
				if (tLength == 0) {
					tmp.data = rawData[i];
					++slidingWindowEnd;
				}
				else {
					SetValue(tmp, tLength, tDistance);
					i += tLength - 1;
					slidingWindowEnd += tLength;
				}
				ret.Add(tmp);
				if (slidingWindowEnd - slidingWindowBegin > slidingWindowMax) slidingWindowBegin = slidingWindowEnd - slidingWindowMax;
			}
			if (ret.Count() != 0 && ret[ret.Count() - 1].data != 256) ret.Add(LZ77ForDeflate{ 256, 0, 0, 0, 0, 0 });
			return ret;
		}
		static void Decode(LZ77ForDeflate* data, size_t dataCount, Vector<uint8_t>& out) noexcept {
			out.Reserve(dataCount);
			for (size_t i = 0; i < dataCount; ++i) {
				LZ77ForDeflate& lz = data[i];
				if (lz.data < 256) out.Add(static_cast<uint8_t>(lz.data));
				else if (lz.data == 256) return;
				else {
					uint16_t length = GetLengthFromValue(lz.data) + lz.lengthEx;
					uint16_t distance = GetDistanceFromDistanceValue(lz.distance) + lz.distanceEx;
					size_t count = out.Count();
					out.Reserve(length);
					if (count >= distance) {
						size_t offset = count - distance;
						for (size_t i = 0; i < length; ++i) out.Add(out[offset + i]);
					}
				}
			}
		}
	public:
		static Vector<Node> GetHuffmanCodeLength(const LZ77ForDeflate* data, size_t count) noexcept {
			size_t counts[286] = {};
			for (size_t i = 0; i < count; ++i) ++counts[data[i].data];
			Vector<Node> nodes;
			for (uint16_t i = 0; i < 286; ++i) if (counts[i] != 0) nodes.Add(Node(counts[i], i, 1));
			Node::CalculateLength(nodes);
			return nodes;
		}
		static Vector<Node> GetHuffmanCodeLengthFromDistance(const LZ77ForDeflate* data, size_t count) noexcept {
			size_t counts[30] = {};
			for (size_t i = 0; i < count; ++i) if (data[i].data > 256) ++counts[data[i].distance];
			Vector<Node> nodes;
			for (uint16_t i = 0; i < 30; ++i) if (counts[i] != 0) nodes.Add(Node(counts[i], i, 1));
			Node::CalculateLength(nodes);
			return nodes;
		}
	public:
		bool operator==(const LZ77ForDeflate&) const noexcept = default;
	};

	class LZ77 {
	public:
		static Vector<LZ77Triple> ToLZ77Triples(const uint8_t* data, size_t byteCount) noexcept {
			Vector<LZ77Triple> ret;
			const size_t maxLength = 5;
			size_t slidingWindowBegin = 0;
			size_t slidingWindowEnd = 0;
			size_t slidingWindowMax = 0xff;
			size_t byteEnd = byteCount - 1;
			for (size_t i = 0; i < byteCount; ++i) {
				LZ77Triple tmp;
				for (size_t j = slidingWindowBegin; j < slidingWindowEnd; ++j) {
					if (data[i] == data[j]) {
						size_t k = 1;
						for (; j + k < slidingWindowEnd && i + k < byteEnd; ++k) {
							if (data[i + k] != data[j + k]) break;
						}
						if (tmp.length <= k) {
							tmp.length = k;
							tmp.distance = i - j;
						}
					}
				}
				if (tmp.length == 0) {
					tmp.c = data[i];
					++slidingWindowEnd;
				}
				else {
					i += tmp.length;
					slidingWindowEnd += tmp.length + 1;
					tmp.c = data[i];
				}
				ret.Add(tmp);
				if (slidingWindowEnd - slidingWindowBegin > slidingWindowMax) slidingWindowBegin = slidingWindowEnd - slidingWindowMax;
			}
			return ret;
		}
	public:
		static Vector<uint8_t> DecodeFromLZ77Triple(const LZ77Triple* data, size_t count) noexcept {
			Vector<uint8_t> ret;
			for (size_t i = 0; i < count; ++i) {
				LZ77Triple tmp = data[i];
				if (tmp.length == 0) ret.Add(tmp.c);
				else {
					const uint8_t* ptr = ret.Items();
					const size_t size = ret.Count();
					Vector<uint8_t> t;
					t.AddRange(ptr + size - tmp.distance, tmp.length);
					for (size_t j = 0, end = t.Count(); j < end; ++j) ret.Add(t[j]);
					ret.Add(tmp.c);
				}
			}
			return ret;
		}
	};
}