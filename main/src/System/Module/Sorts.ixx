export module Sorts;
import CSTDINT;
import Traits;
using namespace System;
using namespace System::Traits;

export namespace System {
	class Sorts {
	public:
		template<class T>
		static constexpr void Swap(T& x, T& y) noexcept {
			T tmp = static_cast<T&&>(x);
			x = static_cast<T&&>(y);
			y = static_cast<T&&>(tmp);
		}
		template<class T>
		static constexpr T& Middian(T& x, T& y, T& z) noexcept {
			return x < y ? (y < z ? y : (x < z ? z : x)) : (x < z ? x : (y < z ? z : y));
		}
		template<Concepts::CForwardIterator Ite>
		static constexpr Ite Increment(const Ite& ite, ptrdiff_t count, const Ite& beg) noexcept {
			if (count >= 0) {
				Ite ret = ite;
				for (ptrdiff_t i = 0; i < count; ++i) ++ret;
				return ret;
			}
			else {
				ptrdiff_t length = ite - beg;
				Ite ret = beg;
				for (ptrdiff_t i = 0, end = length + count; i < end; ++i) ++ret;
				return ret;
			}
		}
		template<Concepts::CBidirectionalIterator Ite>
		static constexpr Ite Increment(const Ite& ite, ptrdiff_t count, const Ite& beg) noexcept {
			Ite ret = ite;
			if (count >= 0) {
				for (ptrdiff_t i = 0; i != count; ++i) ++ret;
			}
			else {
				for (ptrdiff_t i = 0; i != count; --i) --ret;
			}
			return ret;
		}
		template<Concepts::CBidirectionalIterator Ite>
		static constexpr strong_ordering CompareConsistent(const Ite& lhs, const Ite& rhs, const Ite& end) noexcept {
			if (lhs == rhs) return strong_ordering::equal;
			if (rhs == end) return strong_ordering::less;
			Ite ite = lhs;
			while (ite != end) {
				if (ite == rhs) return strong_ordering::less;
				++ite;
			}
			return strong_ordering::greater;
		}
		template<Concepts::CRandomAccessIterator Ite>
		static constexpr strong_ordering CompareConsistent(const Ite& lhs, const Ite& rhs, const Ite& end) noexcept {
			return lhs <=> rhs;
		}
	public:
		template<Concepts::CForwardIterator Ite, Concepts::CCompareFunction<decltype(*declval<Ite>())> CompareFunc>
		static constexpr const Ite& Middian(const Ite& x, const Ite& y, const Ite& z, const CompareFunc& compare) noexcept {
			return compare(*x, *y) < 0 
				? (compare(*y, *z) < 0 ? y : (compare(*x, *z) < 0 ? z : x))
				: (compare(*x, *z) < 0 ? x : (compare(*y, *z) < 0 ? z : y));
		}
		template<Concepts::CForwardIterator Ite>
		static constexpr void InsertSort(Ite beg, Ite end, bool ascendingOrder) noexcept {
			using vtype = Traits::remove_cvref_t<decltype(*beg)>;
			if (ascendingOrder) {
				for (Ite i = Increment(beg, 1, beg); CompareConsistent(i, end, end) < 0; ++i) {
					vtype tmp = static_cast<vtype&&>(*i);
					Ite iprev = Increment(i, -1, beg);
					if (*iprev > tmp) {
						*i = static_cast<vtype&&>(*iprev);
						Ite j = iprev;
						for (; CompareConsistent(j, beg, end) > 0;) {
							Ite jprev = Increment(j, -1, beg);
							if (*jprev <= tmp) break;
							*j = static_cast<vtype&&>(*jprev);
							j = jprev;
						}
						*j = static_cast<vtype&&>(tmp);
					}
				}
			}
			else {
				for (Ite i = Increment(beg, 1, beg); CompareConsistent(i, end, end) < 0; ++i) {
					vtype tmp = static_cast<vtype&&>(*i);
					Ite iprev = Increment(i, -1, beg);
					if (*iprev < tmp) {
						*i = static_cast<vtype&&>(*iprev);
						Ite j = iprev;
						for (; CompareConsistent(j, beg, end) > 0;) {
							Ite jprev = Increment(j, -1, beg);
							if (*jprev >= tmp) break;
							*j = static_cast<vtype&&>(*jprev);
							j = jprev;
						}
						*j = static_cast<vtype&&>(tmp);
					}
				}
			}
		}
		template<Concepts::CForwardIterator Ite, Concepts::CCompareFunction<decltype(*declval<Ite>())> CompareFunc>
		static constexpr void InsertSort(Ite beg, Ite end, bool ascendingOrder, const CompareFunc& compare) noexcept {
			using vtype = Traits::remove_cvref_t<decltype(*beg)>;
			if (ascendingOrder) {
				for (Ite i = Increment(beg, 1, beg); CompareConsistent(i, end, end) < 0; ++i) {
					vtype tmp = static_cast<vtype&&>(*i);
					Ite iprev = Increment(i, -1, beg);
					if (compare(*iprev, tmp) > 0) {
						*i = static_cast<vtype&&>(*iprev);
						Ite j = iprev;
						for (; CompareConsistent(j, beg, end) > 0;) {
							Ite jprev = Increment(j, -1, beg);
							if (compare(*jprev, tmp) <= 0) break;
							*j = static_cast<vtype&&>(*jprev);
							j = jprev;
						}
						*j = static_cast<vtype&&>(tmp);
					}
				}
			}
			else {
				for (Ite i = Increment(beg, 1, beg); CompareConsistent(i, end, end) < 0; ++i) {
					vtype tmp = static_cast<vtype&&>(*i);
					Ite iprev = Increment(i, -1, beg);
					if (compare(*iprev, tmp) < 0) {
						*i = static_cast<vtype&&>(*iprev);
						Ite j = iprev;
						for (; CompareConsistent(j, beg, end) > 0;) {
							Ite jprev = Increment(j, -1, beg);
							if (compare(*jprev, tmp) >= 0) break;
							*j = static_cast<vtype&&>(*jprev);
							j = jprev;
						}
						*j = static_cast<vtype&&>(tmp);
					}
				}
			}
		}
		template<Concepts::CForwardIterator Ite>
		static constexpr void QuickSort(Ite beg, Ite end, bool ascendingOrder) noexcept {
			using vtype = Traits::remove_cvref_t<decltype(*beg)>;
			ptrdiff_t count = end - beg;
			if (count < 2) return;
			Ite last = Increment(end, -1, beg);
			if (count == 2) {
				if (ascendingOrder == (*beg > *last)) Swap<vtype>(*beg, *last);
				return;
			}
			vtype pivot = Middian<vtype>(*beg, *Increment(beg, count / 2, beg), *last);
			Ite i = beg;
			Ite j = last;
			if (ascendingOrder) {
				while (true) {
					for (; i != end && *i < pivot; ++i);
					for (; j != beg && *j >= pivot; j = Increment(j, -1, beg));
					if (CompareConsistent(i, j, end) < 0) Swap(*i, *j);
					else break;
					++i;
					j = Increment(j, -1, beg);
				}
				if (j == beg) {
					InsertSort(beg, end, ascendingOrder);
					return;
				}
			}
			else {
				while (true) {
					for (; i != end && *i >= pivot; ++i);
					for (; j != beg && *j < pivot; j = Increment(j, -1, beg));
					if (CompareConsistent(i, j, end) < 0) Swap(*i, *j);
					else break;
					++i;
					j = Increment(j, -1, beg);
				}
				if (i == beg) {
					InsertSort(beg, end, ascendingOrder);
					return;
				}
			}
			if (i == end) return;
			QuickSort(beg, Increment(i, 1, beg), ascendingOrder);
			QuickSort(i, end, ascendingOrder);
		}
		template<Concepts::CForwardIterator Ite,  Concepts::CCompareFunction<decltype(*declval<Ite>())> CompareFunc>
		static constexpr void QuickSort(Ite beg, Ite end, bool ascendingOrder, const CompareFunc& compare) noexcept {
			using vtype = Traits::remove_cvref_t<decltype(*beg)>;
			ptrdiff_t count = end - beg;
			if (count < 2) return;
			Ite last = Increment(end, -1, beg);
			if (count == 2) {
				if (ascendingOrder == compare(*beg, *last) > 0) Swap<vtype>(*beg, *last);
				return;
			}
			Ite pivot = Middian<Ite>(beg, Increment(beg, count / 2, beg), last, compare);
			Ite i = beg;
			Ite j = last;
			if (ascendingOrder) {
				while (true) {
					for (; i != end && compare(*i, *pivot) < 0; ++i);
					for (; j != beg && compare(*pivot, *j) < 0; j = Increment(j, -1, beg));
					if (CompareConsistent(i, j, end) < 0) Swap(*i, *j);
					else break;
					if (i == pivot) pivot = j;
					else if (j == pivot) pivot = i;
					++i;
					j = Increment(j, -1, beg);
				}
				if (j == beg) {
					InsertSort(beg, end, ascendingOrder, compare);
					return;
				}
			}
			else {
				while (true) {
					for (; i != end && compare(*pivot, *i) < 0; ++i);
					for (; j != beg && compare(*j, *pivot) < 0; j = Increment(j, -1, beg));
					if (CompareConsistent(i, j, end) < 0) Swap(*i, *j);
					else break;
					if (i == pivot) pivot = j;
					else if (j == pivot) pivot = i;
					++i;
					j = Increment(j, -1, beg);
				}
				if (i == beg) {
					InsertSort(beg, end, ascendingOrder,compare);
					return;
				}
			}
			if (i == end) return;
			QuickSort(beg, Increment(i, 1, beg), ascendingOrder, compare);
			QuickSort(i, end, ascendingOrder, compare);
		}
	};
}

//export namespace Helpers {
//	using namespace std;
//	class SortHelper {
//		template<class T>
//		inline static T& Midian(T& a, T& b, T& c) noexcept { return a < b ? (b < c ? b : (a < c ? c : a)) : (a < c ? a : (b < c ? c : b)); }
//		template<class T>
//		inline static const T& Midian(const T& a, const T& b, const T& c, strong_ordering(*compareFunction)(const T&, const T&)) noexcept {
//			return compareFunction(a, b) < 0 ? (compareFunction(b, c) < 0 ? b : (compareFunction(a, c) < 0 ? c : a)) : (compareFunction(a, c) < 0 ? a : (compareFunction(b, c) < 0 ? c : b));
//		}
//	private:
//		template<class T>
//		inline static void Swap(T& a, T& b) noexcept {
//			T tmp = static_cast<T&&>(a);
//			a = static_cast<T&&>(b);
//			b = static_cast<T&&>(tmp);
//		}
//		template<class T>
//		inline static void Merge(T* data, size_t count, size_t mid, bool ascendingOrder) noexcept {
//			T* tmp = new T[count];
//			size_t i = 0, j = mid, k = 0;
//			if (ascendingOrder) {
//				for (; i < mid && j < count; ++k) {
//					if (data[i] < data[j]) tmp[k] = static_cast<T&&>(data[i++]);
//					else tmp[k] = static_cast<T&&>(data[j++]);
//				}
//			}
//			else {
//				for (; i < mid && j < count; ++k) {
//					if (data[i] > data[j]) tmp[k] = static_cast<T&&>(data[i++]);
//					else tmp[k] = static_cast<T&&>(data[j++]);
//				}
//			}
//			if (i == mid) for (; j < count; ++j, ++k) tmp[k] = static_cast<T&&>(data[j]);
//			else for (; i < mid; ++i, ++k) tmp[k] = static_cast<T&&>(data[i]);
//			for (size_t n = 0; n < k; ++n) data[n] = static_cast<T&&>(tmp[n]);
//			delete[] tmp;
//		}
//		template<class T, class R>
//		inline static void Merge(T* data, size_t count, size_t mid, bool ascendingOrder, R T::* comparableMember) noexcept {
//			T* tmp = new T[count];
//			size_t i = 0, j = mid, k = 0;
//			if (ascendingOrder) {
//				for (; i < mid && j < count; ++k) {
//					if (data[i].*comparableMember < data[j].*comparableMember) tmp[k] = static_cast<T&&>(data[i++]);
//					else tmp[k] = static_cast<T&&>(data[j++]);
//				}
//			}
//			else {
//				for (; i < mid && j < count; ++k) {
//					if (data[i].*comparableMember > data[j].*comparableMember) tmp[k] = static_cast<T&&>(data[i++]);
//					else tmp[k] = static_cast<T&&>(data[j++]);
//				}
//			}
//			if (i == mid) for (; j < count; ++j, ++k) tmp[k] = static_cast<T&&>(data[j]);
//			else for (; i < mid; ++i, ++k) tmp[k] = static_cast<T&&>(data[i]);
//			for (size_t n = 0; n < k; ++n) data[n] = static_cast<T&&>(tmp[n]);
//			delete[] tmp;
//		}
//		template<class T>
//		inline static void Quick(T* data, size_t count, bool ascendingOrder) noexcept {
//			if (count < 2) return;
//			else if (count == 2) {
//				if (ascendingOrder && data[0] > data[1]) Swap(data[0], data[1]);
//				else if (!ascendingOrder && data[0] < data[1]) Swap(data[0], data[1]);
//				return;
//			}
//			T pivot = Midian(data[0], data[count / 2], data[count - 1]);
//			size_t i = 0;
//			size_t j = count - 1;
//			if (ascendingOrder) {
//				while (true) {
//					for (; i < count; ++i) if (data[i] >= pivot) break;
//					for (; j > 0; --j) if (data[j] < pivot) break;
//					if (i < j) Swap(data[i], data[j]);
//					else break;
//					++i; --j;
//				}
//				if (j == 0) {
//					InsertSort(data, count, ascendingOrder);
//					return;
//				}
//			}
//			else {
//				while (true) {
//					for (; i < count; ++i) if (data[i] < pivot) break;
//					for (; j > 0; --j) if (data[j] >= pivot) break;
//					if (i < j) Swap(data[i], data[j]);
//					else break;
//					++i; --j;
//				}
//				if (i == 0) {
//					InsertSort(data, count, ascendingOrder);
//					return;
//				}
//			}
//			if (i == count) return;
//			Quick(data, i, ascendingOrder);
//			Quick(&data[i], count - i, ascendingOrder);
//		}
//		template<class T, class R>
//		inline static void Quick(T* data, size_t count, bool ascendingOrder, R T::* comparableMember) noexcept {
//			if (count < 2) return;
//			else if (count == 2) {
//				if (ascendingOrder && data[0].*comparableMember > data[1].*comparableMember) Swap(data[0], data[1]);
//				else if (!ascendingOrder && data[0].*comparableMember < data[1].*comparableMember) Swap(data[0], data[1]);
//				return;
//			}
//			R pivot = Midian(data[0].*comparableMember, data[count / 2].*comparableMember, data[count - 1].*comparableMember);
//			size_t i = 0;
//			size_t j = count - 1;
//			if (ascendingOrder) {
//				while (true) {
//					for (; i < count; ++i) if (data[i].*comparableMember >= pivot) break;
//					for (; j > 0; --j) if (data[j].*comparableMember < pivot) break;
//					if (i < j) Swap(data[i], data[j]);
//					else break;
//					++i; --j;
//				}
//				if (j == 0) {
//					InsertSort(data, count, ascendingOrder, comparableMember);
//					return;
//				}
//			}
//			else {
//				while (true) {
//					for (; i < count; ++i) if (data[i].*comparableMember < pivot) break;
//					for (; j > 0; --j) if (data[j].*comparableMember >= pivot) break;
//					if (i < j) Swap(data[i], data[j]);
//					else break;
//					++i; --j;
//				}
//				if (i == count) {
//					InsertSort(data, count, ascendingOrder, comparableMember);
//					return;
//				}
//			}
//			if (i == count) return;
//			Quick(data, i, ascendingOrder, comparableMember);
//			Quick(&data[i], count - i, ascendingOrder, comparableMember);
//		}
//		template<class T>
//		inline static void Quick(T* data, size_t count, bool ascendingOrder, strong_ordering(*compareFunction)(const T&, const T&)) noexcept {
//			if (count < 2) return;
//			else if (count == 2) {
//				if (ascendingOrder && compareFunction(data[0], data[1]) > 0) Swap(data[0], data[1]);
//				else if (!ascendingOrder && compareFunction(data[0], data[1]) < 0) Swap(data[0], data[1]);
//				return;
//			}
//			T pivot = Midian(data[0], data[count / 2], data[count - 1], compareFunction);
//			size_t i = 0;
//			size_t j = count - 1;
//			if (ascendingOrder) {
//				while (true) {
//					for (; i < count; ++i) if (compareFunction(data[i], pivot) >= 0) break;
//					for (; j > 0; --j) if (compareFunction(data[j], pivot) < 0) break;
//					if (i < j) Swap(data[i], data[j]);
//					else break;
//					++i; --j;
//				}
//				if (j == 0) {
//					InsertSort(data, count, ascendingOrder, compareFunction);
//					return;
//				}
//			}
//			else {
//				while (true) {
//					for (; i < count; ++i) if (compareFunction(data[i], pivot) < 0) break;
//					for (; j > 0; --j) if (compareFunction(data[j], pivot) >= 0) break;
//					if (i < j) Swap(data[i], data[j]);
//					else break;
//					++i; --j;
//				}
//				if (i == count) {
//					InsertSort(data, count, ascendingOrder, compareFunction);
//					return;
//				}
//			}
//			if (i == count) return;
//			Quick(data, i, ascendingOrder, compareFunction);
//			Quick(&data[i], count - i, ascendingOrder, compareFunction);
//		}
//	public:
//		template<class T>
//		inline static void BubbleSort(T* data, size_t count, bool ascendingOrder) noexcept {
//			if (ascendingOrder) {
//				for (size_t end = count; end > 1; --end) {
//					for (size_t i = 1; i < end; ++i) if (data[i - 1] > data[i]) Swap(data[i - 1], data[i]);
//				}
//			}
//			else {
//				for (size_t end = count; end > 1; --end) {
//					for (size_t i = 1; i < end; ++i) if (data[i - 1] < data[i]) Swap(data[i - 1], data[i]);
//				}
//			}
//		}
//		template<class T, class R>
//		inline static void BubbleSort(T* data, size_t count, bool ascendingOrder, R T::* comparableMember) noexcept {
//			if (!comparableMember) return;
//			if (ascendingOrder) {
//				for (size_t end = count; end > 1; --end) {
//					for (size_t i = 1; i < end; ++i) if (data[i - 1].*comparableMember > data[i].*comparableMember) Swap(data[i - 1], data[i]);
//				}
//			}
//			else {
//				for (size_t end = count; end > 1; --end) {
//					for (size_t i = 1; i < end; ++i) if (data[i - 1].*comparableMember < data[i].*comparableMember) Swap(data[i - 1], data[i]);
//				}
//			}
//		}
//		template<class T>
//		inline static void InsertSort(T* data, size_t count, bool ascendingOrder) noexcept {
//			if (ascendingOrder) {
//				for (size_t i = 1; i < count; ++i) {
//					T tmp = static_cast<T&&>(data[i]);
//					if (data[i - 1] > tmp) {
//						data[i] = static_cast<T&&>(data[i - 1]);
//						size_t j = i - 1;
//						for (; j != 0 && data[j - 1] > tmp; --j) data[j] = static_cast<T&&>(data[j - 1]);
//						data[j] = static_cast<T&&>(tmp);
//					}
//				}
//			}
//			else {
//				for (size_t i = 1; i < count; ++i) {
//					T tmp = static_cast<T&&>(data[i]);
//					if (data[i - 1] < tmp) {
//						data[i] = static_cast<T&&>(data[i - 1]);
//						size_t j = i - 1;
//						for (; j != 0 && data[j - 1] < tmp; --j) data[j] = static_cast<T&&>(data[j - 1]);
//						data[j] = static_cast<T&&>(tmp);
//					}
//				}
//			}
//		}
//		template<class T, class R>
//		inline static void InsertSort(T* data, size_t count, bool ascendingOrder, R T::* comparableMember) noexcept {
//			if (!comparableMember) return;
//			if (ascendingOrder) {
//				for (size_t i = 1; i < count; ++i) {
//					T tmp = static_cast<T&&>(data[i]);
//					if (data[i - 1].*comparableMember > tmp.*comparableMember) {
//						data[i] = static_cast<T&&>(data[i - 1]);
//						size_t j = i - 1;
//						for (; j != 0 && data[j - 1].*comparableMember > tmp.*comparableMember; --j) data[j] = static_cast<T&&>(data[j - 1]);
//						data[j] = static_cast<T&&>(tmp);
//					}
//				}
//			}
//			else {
//				for (size_t i = 1; i < count; ++i) {
//					T tmp = static_cast<T&&>(data[i]);
//					if (data[i - 1].*comparableMember < tmp.*comparableMember) {
//						data[i] = static_cast<T&&>(data[i - 1]);
//						size_t j = i - 1;
//						for (; j != 0 && data[j - 1].*comparableMember < tmp.*comparableMember; --j) data[j] = static_cast<T&&>(data[j - 1]);
//						data[j] = static_cast<T&&>(tmp);
//					}
//				}
//			}
//		}
//		template<class T>
//		inline static void InsertSort(T* data, size_t count, bool ascendingOrder, strong_ordering(*compareFunction)(const T&, const T&)) noexcept {
//			if (!compareFunction) return;
//			if (ascendingOrder) {
//				for (size_t i = 1; i < count; ++i) {
//					T tmp = static_cast<T&&>(data[i]);
//					if (compareFunction(data[i - 1], tmp) > 0) {
//						data[i] = static_cast<T&&>(data[i - 1]);
//						size_t j = i - 1;
//						for (; j != 0 && compareFunction(data[j - 1], tmp) > 0; --j) data[j] = static_cast<T&&>(data[j - 1]);
//						data[j] = static_cast<T&&>(tmp);
//					}
//				}
//			}
//			else {
//				for (size_t i = 1; i < count; ++i) {
//					T tmp = static_cast<T&&>(data[i]);
//					if (compareFunction(data[i - 1], tmp) < 0) {
//						data[i] = static_cast<T&&>(data[i - 1]);
//						size_t j = i - 1;
//						for (; j != 0 && compareFunction(data[j - 1], tmp) < 0; --j) data[j] = static_cast<T&&>(data[j - 1]);
//						data[j] = static_cast<T&&>(tmp);
//					}
//				}
//			}
//		}
//		template<class T>
//		inline static void MergeSort(T* data, size_t count, bool ascendingOrder) noexcept {
//			if (count < 2) return;
//			size_t mid = count / 2;
//			MergeSort(data, mid, ascendingOrder);
//			MergeSort(&data[mid], count - mid, ascendingOrder);
//			Merge(data, count, mid, ascendingOrder);
//		}
//		template<class T, class R>
//		inline static void MergeSort(T* data, size_t count, bool ascendingOrder, R T::* comparableMember) noexcept {
//			if (!comparableMember) return;
//			if (count < 2) return;
//			size_t mid = count / 2;
//			MergeSort(data, mid, ascendingOrder, comparableMember);
//			MergeSort(&data[mid], count - mid, ascendingOrder, comparableMember);
//			Merge(data, count, mid, ascendingOrder, comparableMember);
//		}
//		template<class T>
//		inline static void QuickSort(T* data, size_t count, bool ascendingOrder) noexcept {
//			Quick(data, count, ascendingOrder);
//		}
//		template<class T, class R>
//		inline static void QuickSort(T* data, size_t count, bool ascendingOrder, R T::* comparableMember) noexcept {
//			if (!comparableMember) return;
//			Quick(data, count, ascendingOrder, comparableMember);
//		}
//		template<class T>
//		inline static void QuickSort(T* data, size_t count, bool ascendingOrder, strong_ordering(*compareFunction)(const T&, const T&)) noexcept {
//			if (!compareFunction) return;
//			Quick(data, count, ascendingOrder, compareFunction);
//		}
//	};
//}
