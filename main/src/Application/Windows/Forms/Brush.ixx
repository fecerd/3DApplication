export module Brush;
import Drawing;
import WinAPI;
using namespace System::Drawing;
using namespace WinAPI;

//Brush
export namespace System::Application::Windows::Forms {
	class Brush {
		friend class Control;
	protected:
		HBRUSH m_hBrush = nullptr;
	protected:
		Brush() noexcept = default;
	public:
		Brush(const Brush&) noexcept = delete;
		Brush(Brush&& arg) noexcept : m_hBrush(arg.m_hBrush) {
			arg.m_hBrush = nullptr;
		}
		virtual ~Brush() noexcept {
			DeleteObject(m_hBrush);
			m_hBrush = nullptr;
		}
	public:
		HBRUSH GetHBrush() const noexcept { return m_hBrush; }
	public:
		Brush& operator=(const Brush&) noexcept = delete;
		Brush& operator=(Brush&& rhs) noexcept {
			if (this == &rhs) return *this;
			DeleteObject(m_hBrush);
			m_hBrush = rhs.m_hBrush;
			rhs.m_hBrush = nullptr;
			return *this;
		}
	};

	class SolidBrush : public Brush {
	public:
		SolidBrush() noexcept = delete;
		SolidBrush(const Color& color) noexcept {
			m_hBrush = CreateSolidBrush(color);
		}
	};
}
