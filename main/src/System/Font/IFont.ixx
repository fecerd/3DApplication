module;
#include "../../Headers/EnableVirtual.hpp"
export module IFont;
import CSTDINT;
import Objects;
import Point;
import IEnumerable;
import Drawing;

export namespace System {
	/// <summary>
	/// Fontファイル情報を保持するクラスが継承するインターフェース
	/// </summary>
	class IFont {
	public:
		IFont() noexcept {}
		virtual ~IFont() noexcept {}
	public:
		/// <summary>
		/// ベースラインを0としたときの(スケーリングした)アセンダーラインの高さを取得する
		/// </summary>
		/// <param name="scalePermill">描画スケール(千分率)</param>
		virtual int16_t GetAscenderLine(uint32_t scalePermill) const noexcept = 0;
		/// <summary>
		/// 指定したスケールで文字列をビットマップに描画したときの幅を取得する
		/// </summary>
		/// <param name="str">文字列。改行記号'\n'が含まれる場合、一番幅が大きい行の幅を返す</param>
		/// <param name="scalePermill">描画スケール(千分率)</param>
		/// <remarks>
		/// [実装例]
		/// 文字列をコード位置に変換し、行ごとに(スケーリングした)前進幅の和を求め、最大値を返す。
		/// スケーリングによる切り捨てが生じるため、1加算するほうが良い
		/// </remarks>
		virtual uint32_t GetWidth(const String& str, uint32_t scalePermill) const noexcept = 0;
		/// <summary>
		/// 指定したスケールで指定した行数の文字列をビットマップに描画したときの高さを取得する
		/// </summary>
		/// <param name="scalePermill">描画スケール(千分率)</param>
		/// <param name="numLines">描画する行数</param>
		/// <remarks>
		/// [実装例]
		/// (スケーリングした)アセンダーラインからディセンダーラインまでの高さを一行の高さとし、
		/// (スケーリングした)各行の間(行数-1)の行間幅を足した値を返す。
		/// スケーリングによる切り捨てが生じるため、1加算するほうが良い
		/// </remarks>
		virtual uint32_t GetHeight(uint32_t scalePermill, uint32_t numLines) const noexcept = 0;
		/// <summary>
		/// コードポイントを指定してグリフの画像データへの参照を取得する
		/// </summary>
		/// <param name="codePoint">取得するグリフのコードポイント(Unicode)</param>
		/// <param name="scalePermill">描画スケール(千分率)</param>
		/// <remarks>
		/// [実装例]
		/// フォント情報からグリフを描画し、インスタンス内にキャッシュする。
		/// その後、キャッシュデータへの参照を返す
		/// </remarks>
		/// <returns>グリフ画像データへのconst参照</returns>
		virtual const Drawing::Image& GetImage(uint32_t codePoint, uint32_t scalePermill) const noexcept = 0;
	public:
		/// <summary>
		/// 呼び出すたびに描画先に文字列から一文字ずつ描画するコルーチンを取得する
		/// </summary>
		/// <param name="dst">描画先のImageオブジェクトへの参照</param>
		/// <param name="str">描画する文字列</param>
		/// <param name="origin">描画を始める原点。x座標は最初の文字の左端、y座標はベースラインと一致する</param>
		/// <param name="scalePermill">描画スケール(千分率)</param>
		/// <remarks>
		/// [実装例]
		/// IEnumeratorを返すコルーチン関数を作成し、IEnumerableを初期化して返す。
		/// [コルーチン関数実装注意点]
		/// 1. const参照は右辺値の場合コルーチン中にデストラクトされる可能性があるため、
		/// 引数はコピーによって初期化する(const String&ではなくStringにする)。
		/// 2. 描画先画像の範囲外には描画しない(範囲外アクセスに気をつける)
		/// </remarks>
		/// <returns>描画されていない文字が残っている場合、true。すべての文字を描画した場合、false</returns>
#if defined(NO_VIRTUAL)
#else
		virtual IEnumerable<bool> UpdateStringImage(Drawing::Image& dst, const String& str, const Point<int32_t>& origin, uint32_t scalePermill) const noexcept = 0;
#endif
	public:
		virtual void SetBaseLineColor(const Drawing::Color& color) noexcept = 0;
		virtual void SetContoursColor(const Drawing::Color& color) noexcept = 0;
	};
}
