export module BSpline;
import CSTDINT;
import Traits;
import Math;
export import Point;
import IEnumerable;

//ControlPoint
export namespace System {
	/// <summary>
	/// �X�v���C���Ȑ��̐���_��\���\����
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
	/// B-�X�v���C���Ȑ���̓_���v�Z����N���X
	/// </summary>
	class BSpline {
		ControlPoint* m_points = nullptr;	//����_�̔z��
		size_t m_pointCount = 0;	//m_points�̗v�f��
		uint32_t* m_knot = nullptr;	//�m�b�g�x�N�g���̔z��
		size_t m_knotCount = 0;	//�m�b�g�x�N�g���̐�
		uint16_t m_n = 0;	//����
		uint32_t m_unit = 0;	//��Ԃ̐��x�B�傫���ق�t�̕�����邱�Ƃ��ł��邪�A�����͏d���Ȃ�
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
		/// B-�X�v���C�����֐�bj,n(t)�B�ċA�I�ɌĂяo�����
		/// </summary>
		/// <param name="j">���̊֐��O����Ăяo���Ƃ��A�Ȑ���̐���_�ɂ����ĉ��ڂ����w�肷��</param>
		/// <param name="k">���̊֐��O����Ăяo���Ƃ��A�Ȑ��̎������w�肷��</param>
		/// <param name="t">�͈͂�[m_knot[m_n], m_knot[m_knotCount - m_n - 1]]</param>
		/// <returns>�w�肵����������v�Z����B-�X�v���C�����֐��̌���</returns>
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
		/// B-�X�v���C���Ȑ��̐���_�A�����A��Ԑ��x��ݒ肷��B
		/// ���̊֐��ȊO��public�֐��͂��̊֐����Ăяo���Ă��Ȃ��Ǝ��s����
		/// </summary>
		/// <param name="points">����_�̔z��</param>
		/// <param name="pointCount">�������̗v�f��</param>
		/// <param name="n">����</param>
		/// <param name="unit">��Ԑ��x�B�傫���ق�t�̕�����邱�Ƃ��ł��邪�A�����͏d���Ȃ�</param>
		/// <param name="closed">
		/// true�̂Ƃ��A�Ȑ��ɂ���B
		/// ���̂Ƃ��A����_�̔z��͍ŏ��̋Ȑ���̐���_���擪�ɂȂ�悤�ɕύX�����B
		/// �`��̎n�_�ɈႢ�������邪�A�`������͕ς�炸�A�`�挋�ʂɂ��e�����Ȃ�
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
		/// �m�b�g�x�N�g����ݒ肷��BSetFunction()�ȊO�̊֐��͂��̊֐����Ăяo���Ă��Ȃ��Ǝ��s����
		/// </summary>
		/// <param name="index">
		/// �g�p���鐧��_�̐擪�̃C���f�b�N�X(�[���C���f�b�N�X)�B
		/// SetFunction�֐��Őݒ肵������_�̔z��̃C���f�b�N�X�ƈ�v����
		/// </param>
		/// <returns>�g�p���鐧��_�̐�</returns>
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
		/// B-�X�v���C���֐���t�͈̔͂̐擪�̒l���擾����
		/// </summary>
		uint32_t Start() const noexcept { return m_knot && m_n < m_knotCount ? m_knot[m_n] : 0; }
		/// <summary>
		/// B-�X�v���C���֐���t�͈̔͂̍Ō�̒l���擾����
		/// </summary>
		uint32_t End() const noexcept { return m_knot && m_n + 1 <= m_knotCount ? m_knot[m_knotCount - m_n - 1] : 0; }
		/// <summary>
		/// B-�X�v���C���֐��ɂ���ċȐ���̓_���擾����
		/// </summary>
		/// <example>
		///		<code>
		///			CodePoint cp[] = { ... };
		///			uint16_t pointCount = sizeof(cp) / sizeof(cp[0]);
		///			uint32_t unit = ...;	//��Ԑ��x(bs.Start()��bs.End()�̍�)
		///			BSpline bs;
		///			bs.SetFunction(cp, pointCount, n, unit);
		///			uint32_t xMax = ..., yMax = ...;	//�Ȑ��̍ő�l
		///			//i�͋Ȑ��̎n�_�E�I�_�������͋Ȑ���ɂ��鐧��_�̃C���f�b�N�X������A
		///			//�I�_(i == pointCount - 2)�̂Ƃ����[�v�𔲂���
		///			for (size_t i = 0; i < pointCount - 1; ++i) {
		///				uint16_t controlCount = bs.SetKnot(i);	//���̋Ȑ���̐���_�܂ł̐���_��
		///				for (size_t t = bs.Start(), end = bs.End(); t < end; ++t) {
		///					uint32_t x, y;
		///					Spline(i, t, x, y, xMax, yMax);
		///					//(x, y)���g�p����
		///				}
		///				//for���ƍ��킹��(controlCount - 1)����i���������A
		///				//���̋Ȑ���ɂ��鐧��_���I�_���w���悤�ɂȂ�
		///				i += controlCount - 2;
		///			}
		///		</code>
		/// </example>
		/// <param name="index">
		/// �g�p���鐧��_�̐擪�̃C���f�b�N�X(�[���C���f�b�N�X)�B
		/// SetFunction�֐��Őݒ肵������_�̔z��̃C���f�b�N�X�ƈ�v����
		/// </param>
		/// <param name="t">�����B�͈͂�[BSpline::Start(), BSpline::End()]</param>
		/// <param name="x_ref">�Ȑ���̓_��X���W����������Q�ƕϐ�</param>
		/// <param name="y_ref">�Ȑ���̓_��Y���W����������Q�ƕϐ�</param>
		/// <param name="xMax">x_ref�̍ő�l�B�v�Z���ʂ�[0, xMax]�Ɏ��߂���</param>
		/// <param name="yMax">y_ref�̍ő�l�B�v�Z���ʂ�[0, yMax]�Ɏ��߂���</param>
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
		/// Splines()�֐��p�̃R���[�`���֐�
		/// </summary>
		/// <param name="xMax">x���W�̍ő�l</param>
		/// <param name="yMax">y���W�̍ő�l</param>
		IEnumerator<Point<int32_t>> GetSplineEnumerator(int32_t xMin, int32_t yMin, int32_t xMax, int32_t yMax) noexcept {
			//�O��̌��ʂ�ێ����A�����l��A���ŕԂ��Ȃ��悤�ɂ���
			Point<int32_t> prev = Point<int32_t>{ System::MAX_VALUE<int32_t>, System::MAX_VALUE<int32_t> };
			for (size_t i = 0; i < m_pointCount - 1; ++i) {
				//�Ȑ���̐���_���玟�̋Ȑ���̐���_�܂ł̐���_��
				//���̋Ȑ���̐���_�����݂��Ȃ��ꍇ�A�Ō�̐���_�܂ł̐���_��
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
					//�Ō�̐���_���Ȑ���̓_�̏ꍇ�A���O�̌v�Z�ōŌ�̐���_���擾�ł��Ă��邩�m�F����
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
		/// SetFunction()�֐��Őݒ肵��B-�X�v���C���Ȑ���̓_��񋓂���
		/// </summary>
		/// <param name="xMax">x���W�̍ő�l</param>
		/// <param name="yMax">y���W�̍ő�l</param>
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