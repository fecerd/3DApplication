module OpenType_Internal;
import CSTDINT;
import VectorBase;
import BSpline;
import Math;
import OpenType_Internal0;
import Drawing;

namespace System {
	CFF1::~CFF1() noexcept {
		delete[] privateDict;
		privateDict = nullptr;
		for (Type2CharstringParser* x : m_parsers.Values()) delete x;
	}

	bool CFF1::Load(IO::FileStream& file, uint32_t offset, const MAXP& maxp) {
		if (!header.Load(file, offset)) return false;
		if (!name.Load(file, 0, 0)) return false;
		name.PrintNameIndex();
		if (!topDict.Load(file, 0, 0)) return false;
		topDict.PrintDICTIndex();
		if (!string.Load(file, 0, 0)) return false;
		string.PrintStringIndex();
		DICTValue<uint8_t> charstringTypeValue = topDict.GetData(0).GetDICTValue<uint8_t>(DICTOperator::CharstringType);
		uint8_t charstringType = charstringTypeValue.count ? charstringTypeValue.data[0] : 2;
		if (!globalSubrsIndex.Load(file, 0, 0, charstringType)) return false;
		//Encoding
		DICTValue<uint32_t> value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::Encoding);
		if (value.data && !encoding.Load(file, offset, value.data[0])) return false;
		//FDArray
		value = topDict.GetData(0).GetDICTValue<uint32_t>(DICTOperator::FDArray);
		if (value.data && !fdArray.Load(file, offset, value.data[0])) return false;
		fdArray.PrintFDArrayIndex();
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


	Drawing::Image CFF1::GetGlyph_Internal(const Type2CharstringParser& parser, float scale, const Drawing::Pixel& contoursPixel) noexcept {
		const VectorBase<BSpline*>& bsplines = parser.GetBSplines();
		if (!bsplines.Count()) return Drawing::Image();
		Drawing::Image image{
			static_cast<uint32_t>(parser.GetGlyphWidth() * scale + 1),
			static_cast<uint32_t>(parser.GetGlyphHeight() * scale + 1)
		};
		Drawing::Pixel* pixels = image.Data();
		const uint32_t imageXMax = image.Width() - 1;
		const uint32_t imageYMax = image.Height() - 1;
		const int16_t xMin = parser.GetXMin();
		const int16_t yMin = parser.GetYMin();
		for (BSpline* bspline : bsplines) {
			bool first = true;	//輪郭の最初の点の処理時のみtrue
			Point<int32_t> prev;	//直前の点座標
			int8_t yFirstSign = 0;	//輪郭内で初めてy座標が変化した方向(1 or -1)
			int8_t yPrevSign = 0;	//直前のy座標が変化した方向(1 or -1)
			for (Point<int32_t> p : bspline->Splines(xMin, yMin, parser.GetXMax(), parser.GetYMax())) {
				uint32_t x = static_cast<uint32_t>((p.x - xMin) * scale);
				//グリフ座標は左下原点、Image座標は左上原点なので上下反転
				uint32_t y = imageYMax - static_cast<uint32_t>((p.y - yMin) * scale);
				//座標(0, y)のピクセルへのポインタ
				Drawing::Pixel* line0 = pixels + (y * (imageXMax + 1));
				image.SetPixel(x, y, contoursPixel);
				//現在の点の水平右方向のピクセルのアルファ値を反転する(0x00 <-> 0xff)
				if (first) {
					first = false;
					PixelInverse(line0, x, imageXMax);
				}
				else {
					int8_t ySign = static_cast<int8_t>(Math::Sign(static_cast<int64_t>(y) - prev.y));
					if (ySign) {
						PixelInverse(line0, x, imageXMax);
						//y座標の変位方向が反転したとき、直前の点でのアルファ値反転を取り消す
						if (yPrevSign && ySign != yPrevSign) {
							line0 += static_cast<int64_t>(yPrevSign) * (imageXMax + 1);
							PixelInverse(line0, prev.x, imageXMax);
						}
						yPrevSign = ySign;
						if (!yFirstSign) yFirstSign = ySign;
					}
				}
				prev.x = x;
				prev.y = y;
			}
			//輪郭の最後の点(最初の点)が上底もしくは下底のいずれでもない場合、
			//最後のアルファ値反転を取り消す
			if (yFirstSign && yFirstSign == yPrevSign) {
				Drawing::Pixel* line0 = pixels + (prev.y * (imageXMax + 1));
				PixelInverse(line0, prev.x, imageXMax);
			}
		}
		//アルファ値0x00のピクセルを黒色(0x000000ff)に塗りつぶす
		for (uint32_t y = 0; y <= imageYMax; ++y) {
			Drawing::Pixel* line0 = pixels + (imageXMax + 1ull) * y;
			for (uint32_t x = 0; x <= imageXMax; ++x) {
				if (line0[x].a == 0x00u) {
					line0[x].r = 0x00u;
					line0[x].g = 0x00u;
					line0[x].b = 0x00u;
					line0[x].a = 0xffu;
				}
				else if (line0[x].a == 0xaau) line0[x].a = 0xffu;
			}
		}
		return image;
	}
}
