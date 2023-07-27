export module OpenType;
import :Internal;
import HashMap;
export import IFont;
import Encoding;

//TabelRecordIndex
namespace System {
	/// <summary>
	/// �e�e�[�u����OpenType::m_tableRecords�̂ǂ̃C���f�b�N�X�ɑΉ����Ă��邩�\���\����
	/// </summary>
	struct TableRecordIndex {
		static constexpr uint16_t NoImpl = 0xffffu;	//���݂��Ȃ��e�[�u��
		uint16_t cmap = NoImpl;
		uint16_t head = NoImpl;
		uint16_t hhea = NoImpl;
		uint16_t hmtx = NoImpl;
		uint16_t maxp = NoImpl;
		uint16_t name = NoImpl;
		uint16_t os_2 = NoImpl;
		uint16_t post = NoImpl;
		uint16_t loca = NoImpl;
		uint16_t glyf = NoImpl;
		uint16_t cff1 = NoImpl;
		uint16_t cff2 = NoImpl;
	private:
		static constexpr bool equal(const char lhs[4], const char rhs[4]) noexcept {
			if (lhs[0] != rhs[0]) return false;
			if (lhs[1] != rhs[1]) return false;
			if (lhs[2] != rhs[2]) return false;
			if (lhs[3] != rhs[3]) return false;
			return true;
		}
	public:
		/// <summary>
		/// TableRecord�̓��I�z�񂩂�e�e�[�u���ւ̃I�t�Z�b�g�l��ǂݍ��݁A�ۑ�����
		/// </summary>
		/// <param name="tableRecords">TableRecord�^�̓��I�z��</param>
		constexpr void Set(const Vector<TableRecord>& tableRecords) noexcept {
			const ptrdiff_t count = tableRecords.end() - tableRecords.begin();
			for (uint16_t i = 0; i < count; ++i) {
				const char* tag = tableRecords[i].tag;
				if (equal(tag, "cmap")) cmap = i;
				else if (equal(tag, "head")) head = i;
				else if (equal(tag, "hhea")) hhea = i;
				else if (equal(tag, "hmtx")) hmtx = i;
				else if (equal(tag, "maxp")) maxp = i;
				else if (equal(tag, "name")) name = i;
				else if (equal(tag, "OS/2")) os_2 = i;
				else if (equal(tag, "post")) post = i;
				else if (equal(tag, "loca")) loca = i;
				else if (equal(tag, "glyf")) glyf = i;
				else if (equal(tag, "CFF ")) cff1 = i;
				else if (equal(tag, "CFF2")) cff2 = i;
			}
		}
		/// <summary>
		/// OpenType�t�H���g�ɕK�{�ȃe�[�u�������ׂđ��݂��邩���ׂ�
		/// </summary>
		constexpr bool HasRequiredTables() const noexcept {
			return cmap != NoImpl && head != NoImpl && hhea != NoImpl
				&& hmtx != NoImpl && maxp != NoImpl && name != NoImpl
				&& os_2 != NoImpl && post != NoImpl;
		}
	};
}

//OpenType�N���X�������e�[�u��
namespace System {
/*SVG�A�E�g���C���p�e�[�u��*/
	//SVG m_svg;
/*�r�b�g�}�b�v�O���t�p�e�[�u��*/
	//EBDT m_ebdt;
	//EBLC m_eblc;
	//EBSC m_ebsc;
	//CBDT m_cbdt;
	//CBLC m_cblc;
	//SBIX m_sbix;
/*�g���@�\�e�[�u��*/
	//BASE m_base;
	//GDEF m_gdef;
	//GPOS m_gpos;
	//GSUB m_gsub;
	//JSTF m_jstf;
	//MATH m_math;
/*OpenType�t�H���g�o���G�[�V����(HDMX, VDMX�s�v)*/
	//AVAR m_avar;
	//CVAR m_cvar;
	//FVAR m_fvar;
	//GVAR m_gvar;
	//HVAR m_hvar;
	//MVAR m_mvar;
	//STAT m_stat;
	//VVAR m_vvar;
/*�J���[�t�H���g(CBDT, CBLC, SBIX, SVG�K�{)*/
	//COLR m_colr;
	//CPAL m_cpal;
/*���̑�*/
	//DSIG m_dsig;
	//HDMX m_hdmx;
	//KERN m_kern;
	//LISH m_lish;
	//MERG m_merg;
	//META m_meta;
	//PCLT m_pclt;
	//VDMX m_vdmx;
	//VHEA m_vhea;
	//VMTX m_vmtx;
}

//OpenType
export namespace System {
	class OpenType : public IFont {
		OffsetTable m_offsetTable;
		Vector<TableRecord> m_tableRecords;
		TableRecordIndex m_tableIndex;
	public://�K�{�e�[�u��
		CMAP m_cmap;
		HEAD m_head;
		HHEA m_hhea;
		MAXP m_maxp;
		HMTX m_hmtx;
		//NAME m_name;
		//OS_2 m_os2;
		//POST m_post;
	public:/*TrueType�A�E�g���C���p�e�[�u��*/
		LOCA m_loca;
		GLYF m_glyf;
		//CVT m_cvt;
		//FPGM m_fpgm;
		//PREP m_prep;
		//GASP m_gasp;
	public:/*CFF�A�E�g���C���p�e�[�u��*/
		CFF1 m_cff1;
		//CFF2 m_cff2;
		//VORG m_vorg;
	private:
		System::Drawing::Pixel m_baseLineColor = System::Drawing::Colors::Red;
		System::Drawing::Pixel m_contoursColor = System::Drawing::Colors::Green;
	private:
		//�G���[���Ɏg�p���镝0����0�̃O���t�摜
		const System::Drawing::Image m_notdef = System::Drawing::Image();
		//��x�`�悵���O���t�摜���L���b�V������n�b�V���}�b�v�B
		//[Key: �X�P�[��(�番��), Value: [Key: �O���tID, Value: �O���t�摜]]
		mutable System::HashMap<uint32_t, System::HashMap<uint16_t, System::Drawing::Image>> m_caches = 3;
	public:
		bool Load(const String& filename) noexcept {
			System::IO::FileStream file{ filename, System::IO::OpenMode::IN_BINARY };
			if (!file) return false;
			if (!m_offsetTable.Load(file)) return false;
			m_tableRecords.Reserve(m_offsetTable.numTables);
			for (size_t i = 0; i < m_offsetTable.numTables; ++i) {
				TableRecord tmp;
				if (!tmp.Load(file)) return false;
				m_tableRecords.Add(tmp);
			}
			m_tableIndex.Set(m_tableRecords);
			//�K�{�e�[�u��
			if (!m_tableIndex.HasRequiredTables()) return false;
			if (!m_cmap.QuickLoad(file, m_tableRecords[m_tableIndex.cmap].offset)) return false;
			if (!m_head.Load(file, m_tableRecords[m_tableIndex.head].offset)) return false;
			if (!m_hhea.Load(file, m_tableRecords[m_tableIndex.hhea].offset)) return false;
			if (!m_maxp.Load(file, m_tableRecords[m_tableIndex.maxp].offset)) return false;
			if (!m_hmtx.Load(file, m_tableRecords[m_tableIndex.hmtx].offset, m_hhea, m_maxp)) return false;
			//TrueType
			if (m_tableIndex.loca != TableRecordIndex::NoImpl) {
				if (!m_loca.Load(file, m_tableRecords[m_tableIndex.loca].offset, m_head, m_maxp)) return false;
			}
			if (m_tableIndex.glyf != TableRecordIndex::NoImpl) {
				if (!m_glyf.Load(file, m_tableRecords[m_tableIndex.glyf].offset, m_maxp, m_hmtx, m_loca)) return false;
			}
			//CFF�A�E�g���C��
			if (m_offsetTable.HasCFF()) {
				if (m_tableIndex.cff1 != TableRecordIndex::NoImpl) {
					if (!m_cff1.Load(file, m_tableRecords[m_tableIndex.cff1].offset, m_maxp)) return false;
				}
			}
			return true;
		}
	public:
		uint16_t GetGID(uint32_t unicodePoint, uint32_t uvs = 0) const noexcept {
			return m_cmap.GetGID(unicodePoint, uvs);
		}
		const System::Drawing::Image& GetGlyphImage(uint16_t gid, uint32_t scalePermill) const noexcept {
			System::HashMap<uint16_t, System::Drawing::Image>* caches = m_caches.AtPtr(scalePermill);
			if (!caches) {
				m_caches.Insert(scalePermill, System::HashMap<uint16_t, System::Drawing::Image>(3));
				caches = m_caches.AtPtr(scalePermill);
				if (!caches) return m_notdef;
			}
			System::Drawing::Image* ret = caches->AtPtr(gid);
			if (ret) return *ret;
			const float scale = scalePermill / 1000.0f;
			if (!m_offsetTable.HasCFF()) {	//TrueType
				System::Drawing::Image result = m_glyf.GetGlyphImageByGID(gid, scale, m_contoursColor);
				caches->Insert(gid, static_cast<System::Drawing::Image&&>(result));
			}
			else {	//CFF
				System::Drawing::Image result = m_cff1.GetGlyphImageByGID(gid, scale, m_hmtx.GetAdvanceWidth(gid), m_contoursColor);
				caches->Insert(gid, static_cast<System::Drawing::Image&&>(result));
			}
			ret = caches->AtPtr(gid);
			if (ret) return *ret;
			else return m_notdef;
		}
		int16_t GetXMin(uint16_t gid) const noexcept {
			if (!m_offsetTable.HasCFF()) return m_glyf.GetXMin(gid);
			else return m_cff1.GetXMin(gid);
		}
		int16_t GetYMax(uint16_t gid) const noexcept {
			if (!m_offsetTable.HasCFF()) return m_glyf.GetYMax(gid);
			else return m_cff1.GetYMax(gid);
		}
		uint16_t GetAdvanceWidth(uint16_t gid) const noexcept {
			if (!m_offsetTable.HasCFF()) return m_hmtx.GetAdvanceWidth(gid);
			else return m_cff1.GetAdvanceWidth(gid);
		}
		int16_t GetLeftSideBearing(uint16_t gid) const noexcept {
			return m_hmtx.GetLeftSideBearing(gid);
		}
	private:
		uint16_t GetAdvanceWidth(uint16_t gid, uint16_t advanceWidth) const noexcept {
			if (!m_offsetTable.HasCFF()) return advanceWidth;
			else return m_cff1.GetAdvanceWidth(gid, advanceWidth);
		}
	private:
		void DrawBaseLine(System::Drawing::Pixel* dstPixels, uint32_t dstWidth, uint32_t dstHeight, const Point<int32_t>& origin) const noexcept {
			if (0 <= origin.y && origin.y < static_cast<int64_t>(dstHeight)) {
				System::Drawing::Pixel* line0 = dstPixels + dstWidth * origin.y;
				for (uint32_t x = 0; x < dstWidth; ++x) line0[x] = m_baseLineColor;
			}
		}
		static Vector<Encoding::CodePoint> GetCodePoints(const String& str) noexcept {
			const char16_t* data = str.c_str();
			Vector<Encoding::CodePoint> ret;
			ret.Reserve(str.Length());
			do {
				Encoding::CodePoint cp = Encoding::GetCodePoint(data);
				ret.Add(cp);
				data += cp.count;
			} while (*data != u'\0');
			return ret;
		}
		IEnumerator<bool> UpdateStringImageEnumerator(System::Drawing::Image& dst, const String& str, Point<int32_t> origin, uint32_t scalePermill) const noexcept {
			bool ret = true;
			const int16_t ascender = m_hhea.GetAscender();
			const int16_t descender = m_hhea.GetDescender();
			const int16_t lineGap = m_hhea.GetLineGap();
			const float scale = scalePermill / 1000.f;
			System::Drawing::Pixel* dstPixels = dst.Data();
			const uint32_t dstWidth = dst.Width();
			const uint32_t dstHeight = dst.Height();
			Vector<Encoding::CodePoint> cps = GetCodePoints(str);
			const int32_t xLeft = origin.x;
			DrawBaseLine(dstPixels, dstWidth, dstHeight, origin);
			for (size_t i = 0, count = cps.Count(); i < count; ++i) {
				if (cps[i].point == U'\n') {
					origin.y += static_cast<uint32_t>((ascender - descender + lineGap) * scale);
					origin.x = xLeft;
					DrawBaseLine(dstPixels, dstWidth, dstHeight, origin);
					continue;
				}
				uint16_t gid = GetGID(cps[i].point, i + 1 < count && cps[i + 1].IsUVS() ? cps[i + 1].point : 0);
				const System::Drawing::Image& image = GetGlyphImage(gid, scalePermill);
				const System::Drawing::Pixel* srcPixels = image.Data();
				const uint32_t srcWidth = image.Width();
				const uint32_t srcHeight = image.Height();
				int16_t leftSideBearing = static_cast<int16_t>(GetLeftSideBearing(gid) * scale);
				origin.x += leftSideBearing;
				uint16_t advanceWidth = static_cast<uint16_t>(GetAdvanceWidth(gid) * scale);
				if (image.Enabled()) {
					if (origin.x + srcWidth >= dstWidth) {
						origin.y += static_cast<uint32_t>((ascender - descender + lineGap) * scale);
						origin.x = xLeft;
						DrawBaseLine(dstPixels, dstWidth, dstHeight, origin);
					}
					const int16_t yMax = static_cast<int16_t>(GetYMax(gid) * scale);
					for (int16_t y = 0; y < yMax; ++y) {
						uint32_t srcY = static_cast<uint32_t>(y);
						int32_t tDstY = origin.y - yMax + y;
						if (tDstY < 0) continue;
						uint32_t dstY = static_cast<uint32_t>(tDstY);
						if (srcY >= srcHeight || dstY >= dstHeight) continue;
						const System::Drawing::Pixel* srcLine0 = srcPixels + srcWidth * srcY;
						System::Drawing::Pixel* dstLine0 = dstPixels + dstWidth * dstY;
						for (uint32_t srcX = 0; srcX < srcWidth; ++srcX) {
							int32_t tDstX = origin.x + srcX;
							if (tDstX < 0) continue;
							uint32_t dstX = static_cast<uint32_t>(tDstX);
							if (dstX < dstWidth && !srcLine0[srcX].EqualColor(System::Drawing::Pixels::White)) {
								dstLine0[dstX] = srcLine0[srcX];
							}
						}
					}
					for (int64_t y = yMax, yEnd = static_cast<int64_t>(srcHeight); y < yEnd; ++y) {
						if (y < 0) continue;
						uint32_t srcY = static_cast<uint32_t>(y);
						int32_t tDstY = static_cast<int32_t>(static_cast<int32_t>(origin.y) - yMax + y);
						if (tDstY < 0) continue;
						uint32_t dstY = static_cast<uint32_t>(tDstY);
						if (srcY >= srcHeight || dstY >= dstHeight) continue;
						const System::Drawing::Pixel* srcLine0 = srcPixels + srcWidth * srcY;
						System::Drawing::Pixel* dstLine0 = dstPixels + dstWidth * dstY;
						for (uint32_t srcX = 0; srcX < srcWidth; ++srcX) {
							int32_t tDstX = origin.x + srcX;
							if (tDstX < 0) continue;
							uint32_t dstX = static_cast<uint32_t>(tDstX);
							if (dstX < dstWidth && !srcLine0[srcX].EqualColor(System::Drawing::Pixels::White)) {
								dstLine0[dstX] = srcLine0[srcX];
							}
						}
					}
				}
				co_yield ret;
				origin.x -= leftSideBearing;
				origin.x += static_cast<uint32_t>(advanceWidth);
			}
			ret = false;
			co_yield ret;
		}
	public:/* IFont�I�[�o�[���C�h */
		int16_t GetAscenderLine(uint32_t scalePermill) const noexcept override {
			return static_cast<int16_t>(m_hhea.ascender * (scalePermill / 1000.f));
		}
		uint32_t GetWidth(const String& str, uint32_t scalePermill) const noexcept override {
			const float scale = scalePermill / 1000.f;
			Vector<Encoding::CodePoint> cps = GetCodePoints(str);
			uint32_t ret = 0;
			uint32_t current = 0;
			for (size_t i = 0, count = cps.Count(); i < count; ++i) {
				Encoding::CodePoint& cp = cps[i];
				if (cp.point == U'\n') {
					if (current > ret) ret = current;
					current = 0;
					continue;
				}
				const uint16_t gid = GetGID(cps[i].point, i + 1 < count && cps[i + 1].IsUVS() ? cps[i + 1].point : 0);
				current += static_cast<uint32_t>(GetAdvanceWidth(gid, m_hmtx.GetAdvanceWidth(gid)) * scale);
			}
			return (ret < current ? current : ret) + 1;
		}
		uint32_t GetHeight(uint32_t scalePermill, uint32_t numLines) const noexcept override {
			const int16_t ascender = m_hhea.GetAscender();
			const int16_t descender = m_hhea.GetDescender();
			const uint32_t glyphHeight = static_cast<uint32_t>(ascender - descender);
			const float scale = scalePermill / 1000.f;
			const int16_t lineGap = m_hhea.GetLineGap();
			return static_cast<uint32_t>((glyphHeight * numLines + lineGap * (numLines - 1)) * scale) + 1;
		}
		const System::Drawing::Image& GetImage(uint32_t codePoint, uint32_t scalePermill) const noexcept override {
			uint16_t gid = GetGID(codePoint);
			return GetGlyphImage(gid, scalePermill);
		}
		IEnumerable<bool> UpdateStringImage(System::Drawing::Image& dst, const String& str, const Point<int32_t>& origin, uint32_t scalePermill) const noexcept override {
			return IEnumerable<bool>(
				new IEnumerator<bool>(
					[this, &dst, str, origin, scalePermill](bool) { return this->UpdateStringImageEnumerator(dst, str, origin, scalePermill); },
					false
					)
				);
		}
	public:
		void SetBaseLineColor(const System::Drawing::Color& color) noexcept override { m_baseLineColor = color; }
		void SetContoursColor(const System::Drawing::Color& color) noexcept override { m_contoursColor = color; m_contoursColor.a = 0xaa; }
	};
}