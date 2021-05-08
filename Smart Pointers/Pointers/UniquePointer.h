#pragma once
template <typename Type>
class UniquePointer
{
public:
	UniquePointer(Type* = nullptr);
	UniquePointer(UniquePointer<Type>&);
	UniquePointer<Type>& operator=(UniquePointer<Type>&);
	~UniquePointer();
public:
	Type& operator*();
	const Type& operator*() const;
	Type* operator->();
	const Type* operator->() const;
private:
	Type* m_data;
};

template <typename Type>
UniquePointer<Type>::UniquePointer(Type* ptr)
	:m_data(ptr)
{}

template <typename Type>
UniquePointer<Type>::UniquePointer(UniquePointer<Type>& obj)
	: m_data(obj.m_data)
{
	obj.m_data = nullptr;
}

template <typename Type>
UniquePointer<Type>& UniquePointer<Type>::operator=(UniquePointer<Type>& obj)
{
	if (this != &obj)
	{
		delete m_data;
		m_data = obj.m_data;
		obj.m_data = nullptr;
	}
	return *this;
}

template <typename Type>
UniquePointer<Type>::~UniquePointer()
{
	delete m_data;
}

template <typename Type>
Type& UniquePointer<Type>::operator*()
{
	return *m_data;
}

template <typename Type>
const Type& UniquePointer<Type>::operator*() const
{
	return *m_data;
}

template <typename Type>
Type* UniquePointer<Type>::operator->()
{
	return m_data;
}

template <typename Type>
const Type* UniquePointer<Type>::operator->() const
{
	return m_data;
}