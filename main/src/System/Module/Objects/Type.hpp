#pragma once
#ifndef Type_H
#define Type_H
#include "Object.hpp"
import TypeName;

namespace System {
	class Type : public Object {
		String* m_typeName = nullptr;
		String* m_templateArgs = nullptr;
		size_t m_templateCount = 0;
	public:
		Type() noexcept = default;
		Type(const Type&) noexcept;
		Type(Type&& arg) noexcept
			: m_typeName(arg.m_typeName), m_templateArgs(arg.m_templateArgs), m_templateCount(arg.m_templateCount)
		{
			arg.m_typeName = nullptr;
			arg.m_templateArgs = nullptr;
			arg.m_templateCount = 0;
		}
		Type(const char16_t*) noexcept;
		~Type() noexcept;
	public:
		bool Equals(const Object& obj) const noexcept override { return obj.GetTypeID() == GetTypeID() ? *this == static_cast<const Type&>(obj) : false; }
		Type GetType() const noexcept override { return Type(TypeName<Type>::GetFullName().c_str()); }
		String ToString() const noexcept override;
		uint32_t GetTypeID() const noexcept override;
	public:
		bool operator==(const Type& rhs) const noexcept;
	public:
		template<class T>
		static Type CreateType() { return Type(TypeName<T>::GetFullName().c_str()); }
	};
}

#endif