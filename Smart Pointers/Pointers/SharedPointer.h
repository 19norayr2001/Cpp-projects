#pragma once
#include <iostream>
template <typename Type>
class SharedPointer
{
public:
	SharedPointer(Type* = nullptr);
	SharedPointer(const SharedPointer<Type>&);
	SharedPointer<Type>& operator=(const SharedPointer<Type>&);
	~SharedPointer();
public:
	Type& operator*();
	const Type& operator*() const;
	Type* operator->();
	const Type* operator->() const;
private:
	void Deallocate();
private:
	Type* m_data;
	int* m_count;
};

template <typename Type>
SharedPointer<Type>::SharedPointer(Type* ptr)
	:m_data(ptr)
	, m_count(new int(1))
{}

template <typename Type>
SharedPointer<Type>::SharedPointer(const SharedPointer<Type>& obj)
	:m_data(obj.m_data)
	, m_count(obj.m_count)
{
	(*m_count)++;
}

template <typename Type>
SharedPointer<Type>& SharedPointer<Type>::operator=(const SharedPointer<Type>& obj)
{
	if (this != &obj)
	{
		Deallocate();
		m_data = obj.m_data;
		m_count = obj.m_count;
		(*m_count)++;
	}
	return *this;
}

template <typename Type>
SharedPointer<Type>::~SharedPointer()
{
	Deallocate();
}

template <typename Type>
Type& SharedPointer<Type>::operator*()
{
	return *m_data;
}

template <typename Type>
const Type& SharedPointer<Type>::operator*() const
{
	return *m_data;
}

template <typename Type>
Type* SharedPointer<Type>::operator->()
{
	return m_data;
}

template <typename Type>
const Type* SharedPointer<Type>::operator->() const
{
	return m_data;
}

template <typename Type>
void SharedPointer<Type>::Deallocate()
{
	if (*m_count > 0){
		(*m_count)--;
	}
	if (m_count == 0)
	{
		delete m_count;
		delete m_data;
		m_count = nullptr;
		m_data = nullptr;
	}
}