module;
#include "../../Headers/EnableVirtual.hpp"
export module OpenType_Internal;
export import CSTDINT;
export import Traits;
import HashMap;
export import IO;
export import Drawing;
export import OpenType_Internal0;

//CFF1
export namespace System {
	struct CFF1 {
		CFF1Header header;
		CFF1Index name;
		CFF1Index topDict;
		StringIndex string;
		CFF1SubrsIndex globalSubrsIndex;
		CFF1Encoding encoding;
		CFF1Index charString;
		CFF1Charset charset;
		FDSelect fdSelect;
		CFF1Index fdArray;
		//PrivateDICTの配列。
		//要素数はtopDictにPrivateが存在する場合は[1]、それ以外の場合は[fdArray.count]
		PrivateDICT* privateDict = nullptr;
	private:
		//charStringに含まれるグリフ数を保持する。
		//charString.countが0の場合、MAXP::numGlyphsを保持する
		uint16_t m_numGlyphs = 0;
		//privateDictの要素数を保持する
		uint16_t m_privateDictCount = 0;
	private:
		mutable HashMap<uint16_t, Type2CharstringParser*> m_parsers = 3;
	public:
		CFF1() noexcept {}
		~CFF1() noexcept;
	public:
		bool Load(IO::FileStream& file, uint32_t offset, const MAXP& maxp) {
			if (!header.Load(file, offset)) return false;
			if (!name.Load(file, 0, 0)) return false;
			if (!topDict.Load(file, 0, 0)) return false;
			if (!string.Load(file, 0, 0)) return false;
			DICTValue<uint8_t> charstringTypeValue = topDict.GetData(0).GetDICTValue<uint8_t>(DICTOperator::CharstringType);
			uint8_t charstringType = charstringTypeValue.count ? charstringTypeValue.data[0] : 2;
			if (!globalSubrsIndex.Load(file, 0, 0, charstringType)) return false;

			//Encoding
			DICTValue<uint32_t> value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::Encoding);
			if (value.data && !encoding.Load(file, offset, value.data[0])) return false;
			//FDArray
			value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::FDArray);
			if (value.data && !fdArray.Load(file, offset, value.data[0])) return false;
			//PrivateDICT
			value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::Private);
			if (value.data && value.count == 2) {
				m_privateDictCount = 1;
				privateDict = new PrivateDICT[m_privateDictCount];
				if (!privateDict[0].Load(file, value.data[0], offset, value.data[1], charstringType)) return false;
			}
			else {
				m_privateDictCount = fdArray.count;
				privateDict = new PrivateDICT[m_privateDictCount];
				for (uint16_t i = 0; i < m_privateDictCount; ++i) {
					DICTValue<uint32_t> fdValue = fdArray.GetData(i).GetDICTValue<uint32_t>(DICTOperator::Private);
					if (fdValue.data && fdValue.count == 2 && !privateDict[i].Load(file, fdValue.data[0], offset, fdValue.data[1], charstringType)) {
						m_privateDictCount = i;
						return false;
					}
				}
			}
			//Charstring
			value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::CharStrings);
			if (value.data && !charString.Load(file, offset, value.data[0])) return false;
			m_numGlyphs = charString.count ? charString.count : maxp.numGlyphs;
			//Charset
			if (m_numGlyphs) {
				value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::charset);
				if (value.data && !charset.Load(file, offset, value.data[0], m_numGlyphs)) return false;
			}
			//FDSelect
			if (m_numGlyphs) {
				value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::FDSelect);
				if (value.data && !fdSelect.Load(file, offset, value.data[0], m_numGlyphs)) return false;
			}
			return static_cast<bool>(file);
		}
	private:
		const PrivateDICT& GetPrivateDICT(uint16_t gid) const noexcept {
			return privateDict[m_privateDictCount == 1 ? 0 : fdSelect.GetFDIndex(gid)];
		}
		const CFF1SubrsIndex& GetLocalSubrsIndex(uint16_t gid) const noexcept {
			return GetPrivateDICT(gid).subrs;
		}
		uint16_t GetDefaultWidth(uint16_t gid) const noexcept {
			DICTValue<uint16_t> value = GetPrivateDICT(gid).GetData().GetDICTValue<uint16_t>(DICTOperator::defaultWidthX);
			return value.data ? value.data[0] : 0;
		}
		uint16_t GetNormalWidth(uint16_t gid) const noexcept {
			DICTValue<uint16_t> value = GetPrivateDICT(gid).GetData().GetDICTValue<uint16_t>(DICTOperator::nominalWidthX);
			return value.data ? value.data[0] : 0;
		}
	private:
		Type2CharstringParser* GetParser(uint16_t gid) const noexcept {
			return m_parsers.At(gid);
		}
		Type2CharstringParser* GetParser(uint16_t gid, uint16_t advanceWidth) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			if (!parser) {
				const CFF1IndexData type2 = charString.GetData(gid);
				parser = new Type2CharstringParser{ type2.data, type2.size, GetLocalSubrsIndex(gid), globalSubrsIndex };
				m_parsers.Insert(gid, parser);
				const uint16_t defaultWidth = GetDefaultWidth(gid);
				if (advanceWidth != defaultWidth) parser->SetNormalWidth(GetNormalWidth(gid));
				else {
					parser->SetNormalWidth(defaultWidth);
					parser->SetWidthDelta(0);
				}
				parser->Parse();
			}
			return parser;
		}
	private:
		static Drawing::Image GetGlyph_Internal(const Type2CharstringParser& parser, float scale, const Drawing::Pixel& contoursPixel) noexcept;
	public:
		int16_t GetXMin(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetXMin() : 0;
		}
		int16_t GetXMax(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetXMax() : 0;
		}
		int16_t GetYMin(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetYMin() : 0;
		}
		int16_t GetYMax(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetYMax() : 0;
		}
		uint16_t GetAdvanceWidth(uint16_t gid) const noexcept {
			Type2CharstringParser* parser = m_parsers.At(gid);
			return parser ? parser->GetAdvanceWidth() : 0;
		}
		uint16_t GetAdvanceWidth(uint16_t gid, uint16_t advanceWidth) const noexcept {
			Type2CharstringParser* parser = GetParser(gid, advanceWidth);
			return parser ? parser->GetAdvanceWidth() : 0;
		}
		/// <summary>
		/// 指定したグリフ画像を取得する
		/// </summary>
		/// <param name="gid">グリフID</param>
		/// <param name="scale">描画スケール</param>
		/// <param name="advancedWidth">hmtxテーブルから取得するグリフIDに対応する前進幅</param>
		Drawing::Image GetGlyphImageByGID(uint16_t gid, float scale, uint16_t advanceWidth, const Drawing::Pixel& contoursPixel) const noexcept {
			if (gid < m_numGlyphs) {
				Type2CharstringParser* parser = GetParser(gid, advanceWidth);
				if (parser) return GetGlyph_Internal(*parser, scale, contoursPixel);
			}
			return Drawing::Image();
		}
	};
}
