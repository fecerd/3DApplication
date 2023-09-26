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
		bool Load(IO::FileStream& file, uint32_t offset, const MAXP& maxp);
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
