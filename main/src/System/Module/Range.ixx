export module Range;
import Traits;
import Vector;	//using in ReverseEnumerator()
import IEnumerable;

export namespace System {
	template<Traits::Concepts::CForwardIterator I>
	class Range : public ICollection<Traits::iterator_value_t<I>> {
		using value_type = Traits::iterator_value_t<I>;
		I m_begin;
		I m_end;
	public:
		Range() noexcept requires(Traits::Concepts::CDefaultConstructible<I>) {}
		Range(const Range&) noexcept = default;
		Range(Range&&) noexcept = default;
		Range(I b, I e) noexcept : m_begin(b), m_end(e) {}
		Range(value_type* ptr, size_t count) noexcept : m_begin(ptr), m_end(ptr + count) {}
	public:
		I begin() const noexcept { return m_begin; }
		I end() const noexcept { return m_end; }
	public:
		IEnumerator<value_type> GetEnumerator() noexcept override {
			for (I ite = m_begin; ite != m_end; ++ite) co_yield *ite;
		}
		IEnumerator<value_type const> GetEnumerator() const noexcept override {
			for (I ite = m_begin; ite != m_end; ++ite) co_yield *ite;
		}
		IEnumerator<value_type> GetReverseEnumerator() noexcept override {
			if (m_begin == m_end) co_return;
			if constexpr (Traits::Concepts::CBidirectionalIterator<I>) {
				I ite = m_end;
				while (--ite != m_begin) co_yield *ite;
				co_yield *ite;
			}
			else {
				Vector<I> iterators;
				const auto diff = m_end - m_begin;
				iterators.Reserve(diff);
				for (I ite = m_begin; ite != m_end; ++ite) iterators.Add(ite);
				I* pIterator = iterators.Items();
				for (size_t i = iterators.Count(); i-- > 0;) co_yield *(pIterator[i]);
			}
		}
		IEnumerator<value_type const> GetReverseEnumerator() const noexcept override {
			if (m_begin == m_end) co_return;
			if constexpr (Traits::Concepts::CBidirectionalIterator<I>) {
				I ite = m_end;
				while (--ite != m_begin) co_yield *ite;
				co_yield *ite;
			}
			else {
				Vector<I> iterators;
				const auto diff = m_end - m_begin;
				iterators.Reserve(diff);
				for (I ite = m_begin; ite != m_end; ++ite) iterators.Add(ite);
				I* pIterator = iterators.Items();
				for (size_t i = iterators.Count(); i-- > 0;) co_yield *(pIterator[i]);
			}
		}
	public:
		Range& operator=(const Range&) noexcept = default;
		Range& operator=(Range&&) noexcept = default;
	};
	template<class T>
	Range(T* ptr, size_t count) -> Range<T*>;
}
