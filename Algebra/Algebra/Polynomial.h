#pragma once
#include "DividedByZeroException.h"
#include <iostream>
#include <utility>
#include <vector>
template<typename T>
struct Node
{
	int pow;
	T coefficient;
	Node<T>* next;
	Node<T>(int p = 0, T c = T(0), Node<T>* ptr = nullptr)
		: pow(p)
		, coefficient(c)
		, next(ptr)
	{}
};
template<typename T>
class Polynomial
{
	template<typename U>
	friend std::ostream& operator<<(std::ostream&, const Polynomial<U>&);
public:
	Polynomial(T = T(0));
	Polynomial(const std::vector<std::pair<int, T>>&);
	Polynomial(const Polynomial<T>&);
	Polynomial<T>& operator=(const Polynomial<T>&);
	~Polynomial();
public:
	Polynomial<T>& operator+=(const Polynomial<T>&);
	Polynomial<T>& operator-=(const Polynomial<T>&);
	Polynomial<T>& operator*=(const Polynomial<T>&);
	Polynomial<T>& operator/=(const Polynomial<T>&);
	Polynomial<T>& operator%=(const Polynomial<T>&);
	Polynomial<T> operator-() const;
public:
	void normalize();
	void swap(Polynomial<T>&);
public:
	int degree() const { return m_tail == m_head ? -1 : m_tail->pow; }
	bool isZero() const { return m_tail == m_head; }
	template<typename U>
	auto calculate(U) ->decltype(std::declval<T>()* std::declval<U>()) const;
private:
	void deallocate();
	void allocate(const Polynomial<T>&);
	void restoreTail();
private:
	Node<T>* m_head;
	Node<T>* m_tail;
};
template<typename T>
Polynomial<T> operator+(Polynomial<T>, const Polynomial<T>&);
template<typename T>
Polynomial<T> operator-(Polynomial<T>, const Polynomial<T>&);
template<typename T>
Polynomial<T> operator*(Polynomial<T>, const Polynomial<T>&);
template<typename T>
Polynomial<T> operator/(Polynomial<T>, const Polynomial<T>&);
template<typename T>
Polynomial<T> operator%(Polynomial<T>, const Polynomial<T>&);
template<typename T>
Polynomial<T> gcd(const Polynomial<T>&, const Polynomial<T>&);

template<typename T>
Polynomial<T>::Polynomial(T elem)
	:m_head(new Node<T>)
	, m_tail(m_head) {
	if (elem != T(0)) {
		m_head->next = new Node<T>(0, elem);
		m_tail = m_head->next;
	}
}

template<typename T>
Polynomial<T>::Polynomial(const std::vector<std::pair<int, T>>& vec)
	: m_head(new Node<T>)
{
	Node<T>* ptr = m_head;
	for (size_t i = 0; i < vec.size(); ++i) {
		if (vec[i].second == T(0)) {
			continue;
		}
		ptr->next = new Node<T>(vec[i].first, vec[i].second);
		ptr = ptr->next;
	}
	m_tail = ptr;
}

template<typename T>
Polynomial<T>::Polynomial(const Polynomial<T>& obj)
	:m_head(nullptr)
{
	allocate(obj);
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T>& obj) {
	if (this != &obj) {
		deallocate();
		allocate(obj);
	}
	return *this;
}

template<typename T>
Polynomial<T>::~Polynomial() {
	deallocate();
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator+=(const Polynomial<T>& obj) {
	Node<T>* ptr = this->m_head;
	Node<T>* objptr = obj.m_head->next;
	while (objptr != nullptr) {
		while (ptr->next != nullptr && ptr->next->pow < objptr->pow) {
			ptr = ptr->next;
		}
		if (ptr->next == nullptr || ptr->next->pow > objptr->pow) {
			Node<T>* nx = ptr->next;
			ptr->next = new Node<T>(objptr->pow, objptr->coefficient, nx);
		}
		else {
			ptr->next->coefficient += objptr->coefficient;
			if (ptr->next->coefficient == 0) {
				Node<T>* nx = ptr->next->next;
				delete ptr->next;
				ptr->next = nx;
			}
		}
		objptr = objptr->next;
	}
	restoreTail();
	return *this;
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator-=(const Polynomial<T>& obj) {
	return *this += (-obj);
}

template<typename T>
Polynomial<T> Polynomial<T>::operator-() const {
	Polynomial pol = *this;
	Node<T>* ptr = pol.m_head;
	while (ptr->next != nullptr) {
		ptr->next->coefficient = -ptr->next->coefficient;
		ptr = ptr->next;
	}
	return pol;
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator*=(const Polynomial<T>& obj) {
	Node<T>* objptr = obj.m_head->next;
	Polynomial ret;
	while (objptr != nullptr) {
		Polynomial plus;
		Node<T>* ptr = this->m_head->next;
		Node<T>* plusptr = plus.m_head;
		while (ptr != nullptr) {
			plusptr->next = new Node<T>(ptr->pow + objptr->pow, ptr->coefficient * objptr->coefficient);
			ptr = ptr->next;
			plusptr = plusptr->next;
		}
		ret += plus;
		objptr = objptr->next;
	}
	return *this = ret;
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator/=(const Polynomial<T>& obj) {
	int deg1 = this->degree(), deg2 = obj.degree();
	if (obj.isZero()) {
		throw DividedByZeroException();
	}
	if (deg1 < deg2) return *this = Polynomial<T>(T(0));
	T majorValue1 = this->m_tail->coefficient;
	T majorValue2 = obj.m_tail->coefficient;
	Polynomial<T> result = Polynomial<T>({ {deg1 - deg2, majorValue1 / majorValue2} });
	Polynomial<T> pol = *this - result * obj;

	return *this = result + pol / obj;
}

template<typename T>
Polynomial<T>& Polynomial<T>::operator%=(const Polynomial<T>& obj) {
	return *this = *this - *this / obj * obj;
}

template<typename T>
void Polynomial<T>::normalize() {
	if (!isZero()) {
		const T& major_coeff = m_tail->coefficient;
		Node<T>* ptr = m_head->next;
		while (ptr != nullptr) {
			ptr->coefficient /= major_coeff;
			ptr = ptr->next;
		}
	}
}

template<typename T>
void Polynomial<T>::swap(Polynomial<T>& p) {
	std::swap(*this, p);
}

template<typename T>
template<typename U>
auto Polynomial<T>::calculate(U x0) ->decltype(std::declval<T>()* std::declval<U>()) const {
	decltype(std::declval<T>() * std::declval<U>()) res(0);
	U powx(1);
	int lastpow = 0;
	Node<T>* ptr = m_head;
	while (ptr->next != nullptr) {
		while (lastpow < ptr->next->pow) {
			powx *= x0;
			lastpow++;
		}
		res += powx * ptr->next->coefficient;
		ptr = ptr->next;
	}
	return res;
}

template<typename T>
void Polynomial<T>::deallocate() {
	Node<T>* ptr = m_head;
	while (ptr != nullptr) {
		Node<T>* del = ptr;
		ptr = ptr->next;
		delete del;
	}
	m_head = m_tail = nullptr;
}

template<typename T>
void Polynomial<T>::allocate(const Polynomial<T>& obj) {
	m_head = new Node<T>;
	Node<T>* ptr = m_head;
	Node<T>* objptr = obj.m_head->next;
	while (objptr != nullptr) {
		ptr->next = new Node<T>(objptr->pow, objptr->coefficient);
		ptr = ptr->next;
		objptr = objptr->next;
	}
	m_tail = ptr;
}

template<typename T>
void Polynomial<T>::restoreTail() {
	Node<T>* ptr = m_head;
	while (ptr->next != nullptr) {
		ptr = ptr->next;
	}
	m_tail = ptr;
}


template<typename T>
Polynomial<T> operator+(Polynomial<T> obj1, const Polynomial<T>& obj2) {
	return obj1 += obj2;
}

template<typename T>
Polynomial<T> operator-(Polynomial<T> obj1, const Polynomial<T>& obj2) {
	return obj1 -= obj2;
}
template<typename T>
Polynomial<T> operator*(Polynomial<T> obj1, const Polynomial<T>& obj2) {
	return obj1 *= obj2;
}

template<typename T>
Polynomial<T> operator/(Polynomial<T> obj1, const Polynomial<T>& obj2) {
	return obj1 /= obj2;
}

template<typename T>
Polynomial<T> operator%(Polynomial<T> obj1, const Polynomial<T>& obj2) {
	return obj1 %= obj2;
}

template<typename T>
Polynomial<T> gcd(const Polynomial<T>& a, const Polynomial<T>& b) {
	return (b.isZero() ? a : gcd(b, a % b));
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Polynomial<T>& obj) {
	Node<T>* ptr = obj.m_head->next;
	if (ptr == nullptr) {
		std::cout << "0";
		return out;
	}
	while (ptr != nullptr) {
		out << "(" << ptr->coefficient << ")x" << "^" << ptr->pow;
		ptr = ptr->next;
		out << (ptr != nullptr ? "+" : "");
	}
	return out;
}
