#pragma once
#ifndef Object_H
#define Object_H
import CSTDINT;

namespace System {
	class String;
	class Type;

	/// <summary>
	/// System���O��Ԃ�public�ȃN���X�̊��N���X
	/// </summary>
	class Object {
	public:
		constexpr Object() noexcept = default;
		constexpr virtual ~Object() noexcept = default;
	public:
		/// <summary>
		/// ���̃C���X�^���X�ƈ�����Derived::operator==(const Derived&amp;)�Ŕ�r����
		/// </summary>
		/// <param name="obj">��r����Object</param>
		/// <returns>Derived�^�Ƃ��ē����̏ꍇ�Atrue�B����ȊO�̏ꍇ�Afalse</returns>
		virtual bool Equals(const Object& obj) const noexcept { return *this == obj; }
		/// <summary>
		/// �n�b�V���l���擾����
		/// </summary>
		/// <returns>��������Ă���^�̒l�ɑ΂��ČŗL�̃n�b�V���l</returns>
		virtual size_t GetHashCode() const noexcept { return static_cast<size_t>(reinterpret_cast<uintptr_t>(this)); }
		/// <summary>
		/// Type�I�u�W�F�N�g���擾����
		/// </summary>
		/// <returns>���̃C���X�^���X�̌^���w��Type�I�u�W�F�N�g</returns>
		virtual Type GetType() const noexcept = 0;
		/// <summary>
		/// �����񉻂���
		/// </summary>
		/// <returns>���̃C���X�^���X�����l��\��String�I�u�W�F�N�g</returns>
		virtual String ToString() const noexcept = 0;
		/// <summary>
		/// �^�ŗL��ID���擾����
		/// </summary>
		/// <returns>�^�ŗL��4�o�C�g�����l</returns>
		virtual uint32_t GetTypeID() const noexcept = 0;
	public:
		bool operator==(const Object& obj) const noexcept { return this == &obj; }
	};
}

#endif