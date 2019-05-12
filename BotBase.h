#pragma once
#define import using namespace
#define exUnused(ptr) (void*)ptr
#include <string>
import std;
namespace BambooBot {
	// 空值
	class null {
	public:
		bool isNull() const;
		void setNull(bool nullable);

	private:
		bool __is_null_val;
	};

	// 可设置值为 Null 的值
	template<typename T>
	class exNullableContent
	{
	public:
		exNullableContent() = default;
		~exNullableContent() = default;
		exNullableContent(const T &another);
		exNullableContent(const null &another);

		// 是否为 Null
		bool isNull() const { return __null_val.isNull(); }
		// 设置为 Null
		void setNull(const null &value) { this->__null_val = value; }
		T value() const { return __val; }

		bool operator==(const exNullableContent<T> &right);
		bool operator==(const null right);
		bool operator==(const T &right);
		bool operator!=(const exNullableContent<T> &right);
		bool operator!=(const null right);
		bool operator!=(const T &right);
		exNullableContent<T>& operator=(const exNullableContent<T> &right);
		exNullableContent<T>& operator=(const T &right);
		exNullableContent<T>& operator=(const null &right);

	private:
		T __val;
		null __null_val;
		enum class exNullableValueType : int {
			Null = 0x01,
			TemplateArgument = 0x0f
		} __value_type;
	};

	template<typename T>
	inline exNullableContent<T>::exNullableContent(const T & another)
	{
		this->__val = another;
	}
	template<typename T>
	inline exNullableContent<T>::exNullableContent(const null & another)
	{
		this->__null_val = another;
	}
	template<typename T>
	inline bool exNullableContent<T>::operator==(const exNullableContent<T>& right)
	{
		if (this->__value_type == right.__value_type)
		{
			switch (this->__value_type)
			{
			case exNullableValueType::Null:
				return (this->__null_val.isNull() == right.__null_val.isNull());
			case exNullableValueType::TemplateArgument:
				return (this->__val == right.value());
			}
		}
		return false;
	}
	template<typename T>
	inline bool exNullableContent<T>::operator==(const null right)
	{
		if (this->__value_type == right.__value_type)
		{
			if(this->__value_type == exNullableValueType::Null)
				return (this->__null_val.isNull() == right.__null_val.isNull());
			else return false;
		}
		return false;
	}
	template<typename T>
	inline bool exNullableContent<T>::operator==(const T & right)
	{
		if (this->__value_type == right.__value_type)
		{
			if (this->__value_type == exNullableValueType::TemplateArgument)
				return (this->__val == right.value());
			else return false;
		}
		return false;
	}
	template<typename T>
	inline bool exNullableContent<T>::operator!=(const exNullableContent<T>& right)
	{
		return !(*this == right);
	}
	template<typename T>
	inline bool exNullableContent<T>::operator!=(const null right)
	{
		return !(*this == right);
	}
	template<typename T>
	inline bool exNullableContent<T>::operator!=(const T & right)
	{
		return !(*this == right);
	}
	template<typename T>
	inline exNullableContent<T>& exNullableContent<T>::operator=(const exNullableContent<T>& right)
	{
		this->__value_type = right.__value_type;
		if (this->__value_type == exNullableValueType::TemplateArgument)
		{
			this->__val = right.value();
			this->__null_val.setNull(false);
		}
	}
	template<typename T>
	inline exNullableContent<T>& exNullableContent<T>::operator=(const T & right)
	{
		this->__value_type = exNullableValueType::TemplateArgument;
		this->__val = right;
	}
	template<typename T>
	inline exNullableContent<T>& exNullableContent<T>::operator=(const null & right)
	{
		this->__value_type = exNullableValueType::Null;
		this->__null_val.setNull(true);
	}

	// 机器人根对象，所有的对象必须派生自此基类
	class BotObject {
	public:
		// 禁止赋值和比较，以确保其唯一性
		bool operator==(const BotObject &) = delete;
		BotObject& operator=(const BotObject &) = delete;
	};
}