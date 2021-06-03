#pragma once
#include "Polynomial.h"
#include <vector>
#include <utility>

template<typename T>
class SmithNormalizer {
private:
	SmithNormalizer();
public:
	SmithNormalizer(const std::vector<std::vector<Polynomial<T>>>& matrix);
public:
	const std::vector<std::vector<Polynomial<T>>>& getMatrix() const { return m_matrix; }
private:
	void normalize();
	void swap_rows(size_t, size_t);
	void swap_columns(size_t, size_t);
	void add_row(size_t, size_t, const Polynomial<T>&);
	void add_column(size_t, size_t, const Polynomial<T>&);
	std::pair<size_t, size_t> get_minimum_index() const;
	void move_element(size_t, size_t, size_t, size_t);
	void normalize_edges();
	void normalize_row_edge();
	void normalize_column_edge();
	void normalize_row_edge_element(size_t);
	void normalize_column_edge_element(size_t);
private:
	std::vector<std::vector<Polynomial<T>>> m_matrix;
};

template<typename T>
SmithNormalizer<T>::SmithNormalizer(const std::vector<std::vector<Polynomial<T>>>& matrix)
	:m_matrix(matrix) {
	normalize();
}

template<typename T>
void SmithNormalizer<T>::normalize() {
	std::pair<size_t, size_t> p = get_minimum_index();
	move_element(0, 0, p.first, p.second);

	size_t n = m_matrix.size();
	size_t m = m_matrix[0].size();
	normalize_edges();

	if (n == 1) {
		return;
	}

	std::vector<std::vector<Polynomial<T>>> matrix(n - 1, std::vector<Polynomial<T>>(m - 1));
	for (size_t i = 1; i < n; ++i) {
		for (size_t j = 1; j < m; ++j) {
			matrix[i - 1][j - 1] = m_matrix[i][j];
		}
	}

	SmithNormalizer sm(matrix);
	for (size_t i = 1; i < n; ++i) {
		for (size_t j = 1; j < m; ++j) {
			m_matrix[i][j] = sm.m_matrix[i - 1][j - 1];
		}
	}
}

template<typename T>
void SmithNormalizer<T>::swap_rows(size_t i, size_t j) {
	if (i == j) return;
	std::swap(m_matrix[i], m_matrix[j]);
}

template<typename T>
void SmithNormalizer<T>::swap_columns(size_t i, size_t j) {
	if (i == j) return;
	for (size_t t = 0; t < m_matrix.size(); ++t) {
		m_matrix[t][i].swap(m_matrix[t][j]);
	}
}

template<typename T>
void SmithNormalizer<T>::add_row(size_t i, size_t j, const Polynomial<T>& coeff) {
	for (size_t t = 0; t < m_matrix.size(); ++t) {
		m_matrix[i][t] += coeff * m_matrix[j][t];
	}
}

template<typename T>
void SmithNormalizer<T>::add_column(size_t i, size_t j, const Polynomial<T>& coeff) {
	for (size_t t = 0; t < m_matrix.size(); ++t) {
		m_matrix[t][i] += coeff * m_matrix[t][j];
	}
}

template<typename T>
std::pair<size_t, size_t> SmithNormalizer<T>::get_minimum_index() const {
	size_t min_i = 0;
	size_t min_j = 0;
	int min_deg = m_matrix[min_i][min_j].degree();
	size_t n = m_matrix.size();
	size_t m = m_matrix[0].size();
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < m; ++j) {
			if (m_matrix[i][j].degree() < min_deg && !m_matrix[i][j].isZero()) {
				min_deg = m_matrix[i][j].degree();
				min_i = i;
				min_j = j;
			}
		}
	}
	return { min_i, min_j };
}

template<typename T>
void SmithNormalizer<T>::move_element(size_t i, size_t j, size_t n, size_t m) {
	swap_rows(i, n);
	swap_columns(j, m);
}

template<typename T>
void SmithNormalizer<T>::normalize_edges() {
	normalize_column_edge();
	normalize_row_edge();
	normalize_column_edge();
	size_t m = m_matrix[0].size();
	for (size_t i = 1; i < m; ++i) {
		add_column(0, i, T(1));
		normalize_column_edge();
	}
	normalize_row_edge();
	normalize_column_edge();

	m_matrix[0][0].normalize();
}

template<typename T>
void SmithNormalizer<T>::normalize_row_edge() {
	size_t m = m_matrix[0].size();
	bool is_existing_nonzero = false;
	for (size_t i = 1; i < m; ++i) {
		if (!m_matrix[0][i].isZero()) {
			is_existing_nonzero = true;
			normalize_row_edge_element(i);
		}
	}
}

template<typename T>
void SmithNormalizer<T>::normalize_column_edge() {
	size_t n = m_matrix.size();
	bool is_existing_nonzero = false;
	for (size_t i = 1; i < n; ++i) {
		if (!m_matrix[i][0].isZero()) {
			is_existing_nonzero = true;
			normalize_column_edge_element(i);
		}
	}
}

template<typename T>
void SmithNormalizer<T>::normalize_row_edge_element(size_t i) {
	if (m_matrix[0][i].isZero()) {
		return;
	}

	do {
		swap_columns(0, i);
		Polynomial<T> res = m_matrix[0][0] / m_matrix[0][i];
		add_column(0, i, -res);

	} while (!m_matrix[0][0].isZero());
	swap_columns(0, i);
}

template<typename T>
void SmithNormalizer<T>::normalize_column_edge_element(size_t i) {
	if (m_matrix[i][0].isZero()) {
		return;
	}

	do {
		swap_rows(0, i);
		Polynomial<T> res = m_matrix[0][0] / m_matrix[i][0];
		add_row(0, i, -res);

	} while (!m_matrix[0][0].isZero());
	swap_rows(0, i);
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const SmithNormalizer<T>& sm) {
	auto& matrix = sm.getMatrix();
	
	for (auto& v : matrix) {
		for (auto& p : v) {
			out << p << ' ';
		}
		out << std::endl;
	}

	return out;
}