export module BSpline;
import CSTDINT;
import Traits;
import Math;
export import Point;
import IEnumerable;

//ControlPoint
export namespace System {
	/// <summary>
	/// スプライン曲線の制御点を表す構造体
	/// </summary>
	struct ControlPoint {
		bool OnCurve = true;
		int16_t x = 0;
		int16_t y = 0;
	public:
		constexpr ControlPoint() noexcept = default;
		constexpr ControlPoint(bool newOnCurve, int16_t newX, int16_t newY) noexcept : OnCurve(newOnCurve), x(newX), y(newY) {};
	};
}

//BSpline
export namespace System {
	/// <summary>
	/// B-スプライン曲線上の点を計算するクラス
	/// </summary>
	class BSpline {
		ControlPoint* m_points = nullptr;	//制御点の配列
		size_t m_pointCount = 0;	//m_pointsの要素数
		uint32_t* m_knot = nullptr;	//ノットベクトルの配列
		size_t m_knotCount = 0;	//ノットベクトルの数
		uint16_t m_n = 0;	//次数
		uint32_t m_unit = 0;	//補間の精度。大きいほどtの幅を取ることができるが、処理は重くなる
	public:
		BSpline() noexcept = default;
		BSpline(BSpline&& arg) noexcept
			: m_points(arg.m_points), m_pointCount(arg.m_pointCount), m_knot(arg.m_knot)
			, m_knotCount(arg.m_knotCount), m_n(arg.m_n), m_unit(arg.m_unit)
		{
			arg.m_points = nullptr;
			arg.m_knot = nullptr;
		}
		~BSpline() noexcept {
			delete m_points;
			delete m_knot;
			m_points = nullptr;
			m_knot = nullptr;
		}
	private:
		/// <summary>
		/// B-スプライン基底関数bj,n(t)。再帰的に呼び出される
		/// </summary>
		/// <param name="j">この関数外から呼び出すとき、曲線上の制御点において何個目かを指定する</param>
		/// <param name="k">この関数外から呼び出すとき、曲線の次数を指定する</param>
		/// <param name="t">範囲は[m_knot[m_n], m_knot[m_knotCount - m_n - 1]]</param>
		/// <returns>指定した引数から計算したB-スプライン基底関数の結果</returns>
		float Basis(size_t j, uint16_t k, uint32_t t) const noexcept {
			if (k == 0) return (m_knot[j] <= t && t < m_knot[j + 1]) ? 1.0f : 0.0f;
			else if (m_knot[j + k] == m_knot[j] && m_knot[j + k + 1] == m_knot[j + 1]) return 0.0f;
			else if (m_knot[j + k] == m_knot[j]) return	(m_knot[j + k + 1] - t) * Basis(j + 1, k - 1, t) / (m_knot[j + k + 1] - m_knot[j + 1]);
			else if (m_knot[j + k + 1] == m_knot[j + 1]) return (t - m_knot[j]) * Basis(j, k - 1, t) / (m_knot[j + k] - m_knot[j]);
			else return ((t - m_knot[j]) * Basis(j, k - 1, t) / (m_knot[j + k] - m_knot[j])) +
				((m_knot[j + k + 1] - t) * Basis(j + 1, k - 1, t) / (m_knot[j + k + 1] - m_knot[j + 1]));
		}
	public:
		/// <summary>
		/// B-スプライン曲線の制御点、次数、補間精度を設定する。
		/// この関数以外のpublic関数はこの関数を呼び出していないと失敗する
		/// </summary>
		/// <param name="points">制御点の配列</param>
		/// <param name="pointCount">第一引数の要素数</param>
		/// <param name="n">次数</param>
		/// <param name="unit">補間精度。大きいほどtの幅を取ることができるが、処理は重くなる</param>
		/// <param name="closed">
		/// trueのとき、閉曲線にする。
		/// このとき、制御点の配列は最初の曲線上の制御点が先頭になるように変更される。
		/// 描画の始点に違いが生じるが、描画方向は変わらず、描画結果にも影響しない
		/// </param>
		void SetFunction(const ControlPoint* points, size_t pointCount, uint16_t n, uint32_t unit, bool closed = false) {
			m_pointCount = closed ? pointCount + 1 : pointCount;
			delete m_points;
			m_points = new ControlPoint[m_pointCount];
			if (closed) {
				size_t tmp = 0;
				for (; tmp < pointCount; ++tmp) if (points[tmp].OnCurve) break;
				size_t first = tmp == pointCount ? 0 : tmp;
				for (size_t i = first; i < pointCount; ++i) m_points[i - first] = points[i];
				if (tmp == pointCount) m_points[0].OnCurve = true;
				for (size_t i = 0; i < first; ++i) m_points[pointCount - first + i] = points[i];
				m_points[pointCount] = m_points[0];
			}
			else {
				for (size_t i = 0; i < m_pointCount; ++i) m_points[i] = points[i];
			}
			m_n = n;
			m_unit = unit;
			delete m_knot;
			m_knot = nullptr;
			m_knotCount = 0;
		}
		/// <summary>
		/// ノットベクトルを設定する。SetFunction()以外の関数はこの関数を呼び出していないと失敗する
		/// </summary>
		/// <param name="index">
		/// 使用する制御点の先頭のインデックス(ゼロインデックス)。
		/// SetFunction関数で設定した制御点の配列のインデックスと一致する
		/// </param>
		/// <returns>使用する制御点の数</returns>
		size_t SetKnot(size_t index) {
			if (!m_points || m_pointCount < 2 || index + 1 >= m_pointCount) return 0;
			size_t controlCount = 1;
			while (index + controlCount < m_pointCount) {
				++controlCount;
				if (m_points[index + controlCount - 1].OnCurve) break;
			}
			delete m_knot;
			m_knotCount = controlCount + m_n + 1;
			m_knot = new uint32_t[m_knotCount];
			m_knot[0] = 0;
			uint32_t val = m_unit;
			if (m_points[index].OnCurve) {
				for (size_t i = 1; i < m_n + 1; ++i) m_knot[i] = 0;
				for (size_t i = m_n + 1, end = m_knotCount - (m_n + 1); i < end; ++i, val += m_unit) m_knot[i] = val;
			}
			else {
				for (size_t i = 1, end = m_knotCount - (m_n + 1); i < end; ++i, val += m_unit) m_knot[i] = val;
			}
			if (m_points[index + controlCount - 1].OnCurve) {
				for (size_t i = m_knotCount - (m_n + 1); i < m_knotCount; ++i) m_knot[i] = val;
			}
			else {
				for (size_t i = m_knotCount - (m_n + 1); i < m_knotCount; ++i, val += m_unit) m_knot[i] = val;
			}
			return controlCount;
		}
		/// <summary>
		/// B-スプライン関数のtの範囲の先頭の値を取得する
		/// </summary>
		uint32_t Start() const noexcept { return m_knot && m_n < m_knotCount ? m_knot[m_n] : 0; }
		/// <summary>
		/// B-スプライン関数のtの範囲の最後の値を取得する
		/// </summary>
		uint32_t End() const noexcept { return m_knot && m_n + 1 <= m_knotCount ? m_knot[m_knotCount - m_n - 1] : 0; }
		/// <summary>
		/// B-スプライン関数によって曲線上の点を取得する
		/// </summary>
		/// <example>
		///		<code>
		///			CodePoint cp[] = { ... };
		///			uint16_t pointCount = sizeof(cp) / sizeof(cp[0]);
		///			uint32_t unit = ...;	//補間精度(bs.Start()とbs.End()の差)
		///			BSpline bs;
		///			bs.SetFunction(cp, pointCount, n, unit);
		///			uint32_t xMax = ..., yMax = ...;	//曲線の最大値
		///			//iは曲線の始点・終点もしくは曲線上にある制御点のインデックスが入り、
		///			//終点(i == pointCount - 2)のときループを抜ける
		///			for (size_t i = 0; i < pointCount - 1; ++i) {
		///				uint16_t controlCount = bs.SetKnot(i);	//次の曲線上の制御点までの制御点数
		///				for (size_t t = bs.Start(), end = bs.End(); t < end; ++t) {
		///					uint32_t x, y;
		///					Spline(i, t, x, y, xMax, yMax);
		///					//(x, y)を使用する
		///				}
		///				//for文と合わせて(controlCount - 1)だけiが増加し、
		///				//次の曲線上にある制御点か終点を指すようになる
		///				i += controlCount - 2;
		///			}
		///		</code>
		/// </example>
		/// <param name="index">
		/// 使用する制御点の先頭のインデックス(ゼロインデックス)。
		/// SetFunction関数で設定した制御点の配列のインデックスと一致する
		/// </param>
		/// <param name="t">引数。範囲は[BSpline::Start(), BSpline::End()]</param>
		/// <param name="x_ref">曲線上の点のX座標が代入される参照変数</param>
		/// <param name="y_ref">曲線上の点のY座標が代入される参照変数</param>
		/// <param name="xMax">x_refの最大値。計算結果は[0, xMax]に収められる</param>
		/// <param name="yMax">y_refの最大値。計算結果は[0, yMax]に収められる</param>
		void Spline(size_t index, uint32_t t, int32_t& x_ref, int32_t& y_ref, int32_t xMin, int32_t yMin, int32_t xMax, int32_t yMax) noexcept {
			if (!m_points || !m_knot) {
				x_ref = 0;
				y_ref = 0;
				return;
			}
			float x = 0;
			float y = 0;
			for (size_t i = 0, end = m_knotCount - m_n - 1; i < end; ++i) {
				float base = Basis(i, m_n, t);
				x += base * m_points[index + i].x;
				y += base * m_points[index + i].y;
			}
			x_ref = System::Math::Clamp<int32_t>(Math::Round(x), xMin, xMax);
			y_ref = System::Math::Clamp<int32_t>(Math::Round(y), yMin, yMax);
		}
	private:
		/// <summary>
		/// Splines()関数用のコルーチン関数
		/// </summary>
		/// <param name="xMax">x座標の最大値</param>
		/// <param name="yMax">y座標の最大値</param>
		IEnumerator<Point<int32_t>> GetSplineEnumerator(int32_t xMin, int32_t yMin, int32_t xMax, int32_t yMax) noexcept {
			//前回の結果を保持し、同じ値を連続で返さないようにする
			Point<int32_t> prev = Point<int32_t>{ System::MAX_VALUE<int32_t>, System::MAX_VALUE<int32_t> };
			for (size_t i = 0; i < m_pointCount - 1; ++i) {
				//曲線上の制御点から次の曲線上の制御点までの制御点数
				//次の曲線上の制御点が存在しない場合、最後の制御点までの制御点数
				size_t usedPointCount = SetKnot(i);
				if (usedPointCount != 2) {
					for (uint32_t t = Start(), end = End(); t < end; ++t) {
						Point<int32_t> ret;
						Spline(i, t, ret.x, ret.y, xMin, yMin, xMax, yMax);
						if (prev != ret) {
							co_yield ret;
							prev = ret;
						}
					}
					//最後の制御点が曲線上の点の場合、直前の計算で最後の制御点が取得できているか確認する
					ControlPoint& last = m_points[i + usedPointCount - 1];
					if (last.OnCurve) {
						Point<int32_t> ret{ static_cast<int32_t>(last.x), static_cast<int32_t>(last.y) };
						if (prev != ret) {
							co_yield ret;
							prev = ret;
						}
					}
					i += usedPointCount - 2;
				}
				else {
					int16_t x0 = m_points[i].x;
					int16_t y0 = m_points[i].y;
					int16_t x1 = m_points[i + 1].x;
					int16_t y1 = m_points[i + 1].y;
					bool steep = System::Math::Abs(y1 - y0) > System::Math::Abs(x1 - x0);
					if (steep) {
						System::Math::Swap(x0, y0);
						System::Math::Swap(x1, y1);
					}
					int64_t deltax = static_cast<int64_t>(System::Math::Abs(static_cast<int32_t>(x1) - x0));
					int64_t deltay = static_cast<int64_t>(System::Math::Abs(static_cast<int32_t>(y1) - y0));
					int64_t error = deltax / 2;
					int16_t y = y0;
					int16_t inc = x0 < x1 ? 1 : -1;
					int16_t ystep = y0 < y1 ? 1 : -1;
					for (int16_t x = x0; x != x1; x += inc) {
						Point<int32_t> ret{ static_cast<int32_t>(x), static_cast<int32_t>(y) };
						if (steep) System::Math::Swap(ret.x, ret.y);
						if (prev != ret) {
							co_yield ret;
							prev = ret;
						}
						error -= deltay;
						if (error < 0) {
							y += ystep;
							error += deltax;
						}
					}
					Point<int32_t> ret{ static_cast<int32_t>(x1), static_cast<int32_t>(y1) };
					if (steep) System::Math::Swap(ret.x, ret.y);
					if (prev != ret) {
						co_yield ret;
						prev = ret;
					}
				}
			}
		}
	public:
		/// <summary>
		/// SetFunction()関数で設定したB-スプライン曲線上の点を列挙する
		/// </summary>
		/// <param name="xMax">x座標の最大値</param>
		/// <param name="yMax">y座標の最大値</param>
		IEnumerable<Point<int32_t>> Splines(int32_t xMin, int32_t yMin, int32_t xMax, int32_t yMax) noexcept {
			return IEnumerable<Point<int32_t>>(
				new IEnumerator<Point<int32_t>>(
					[this, xMin, yMin, xMax, yMax](bool) { return this->GetSplineEnumerator(xMin, yMin, xMax, yMax); },
					false
					)
				);
		}
	};
}