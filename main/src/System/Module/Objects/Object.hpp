#pragma once
#ifndef Object_H
#define Object_H
import CSTDINT;

namespace System {
	class String;
	class Type;

	/// <summary>
	/// System名前空間のpublicなクラスの基底クラス
	/// </summary>
	class Object {
	public:
		constexpr Object() noexcept = default;
		constexpr virtual ~Object() noexcept = default;
	public:
		/// <summary>
		/// このインスタンスと引数をDerived::operator==(const Derived&amp;)で比較する
		/// </summary>
		/// <param name="obj">比較するObject</param>
		/// <returns>Derived型として等価の場合、true。それ以外の場合、false</returns>
		virtual bool Equals(const Object& obj) const noexcept { return *this == obj; }
		/// <summary>
		/// ハッシュ値を取得する
		/// </summary>
		/// <returns>実装されている型の値に対して固有のハッシュ値</returns>
		virtual size_t GetHashCode() const noexcept { return static_cast<size_t>(reinterpret_cast<uintptr_t>(this)); }
		/// <summary>
		/// Typeオブジェクトを取得する
		/// </summary>
		/// <returns>このインスタンスの型を指すTypeオブジェクト</returns>
		virtual Type GetType() const noexcept = 0;
		/// <summary>
		/// 文字列化する
		/// </summary>
		/// <returns>このインスタンスが持つ値を表すStringオブジェクト</returns>
		virtual String ToString() const noexcept = 0;
		/// <summary>
		/// 型固有のIDを取得する
		/// </summary>
		/// <returns>型固有の4バイト整数値</returns>
		virtual uint32_t GetTypeID() const noexcept = 0;
	public:
		bool operator==(const Object& obj) const noexcept { return this == &obj; }
	};
}

#endif