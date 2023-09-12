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

