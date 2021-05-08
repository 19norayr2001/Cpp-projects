#pragma once
#include <vector>

template<typename T, typename Binary_Functor>
class SegmentTree {
	typedef T value_type;
public:
	template<typename It>
	SegmentTree(It, It, Binary_Functor);
	value_type query(int, int) const;
	void update(size_t, value_type);
	size_t findIndex(int) const;
private:
	void build(int, int, const std::vector<T>&, size_t = 1);
	value_type query(int, int, int, int, size_t = 1) const;
	size_t findIndex(int, int, int, size_t = 1) const;
private:
	std::vector<value_type> t;
	size_t n;
	Binary_Functor mFunction;
};

template <typename T, typename Binary_Functor>
template <typename It>
SegmentTree<T, Binary_Functor>::SegmentTree(It begin, It end, Binary_Functor f)
	:mFunction(f) {
	std::vector<T> arr(begin, end);
	n = arr.size();
	build(0, n - 1, arr);
}


template <typename T, typename Binary_Functor>
void SegmentTree<T, Binary_Functor>::build(int tleft, int tright, const std::vector<T>& arr, size_t kod) {
	if (tleft == tright) {
		while (kod >= t.size()) {
			t.emplace_back();
		}
		t[kod] = arr[tleft];
		return;
	}
	int m = (tleft + tright) / 2;
	build(tleft, m, arr, 2 * kod);
	build(m + 1, tright, arr, 2 * kod + 1);
	t[kod] = mFunction(t[2 * kod], t[2 * kod + 1]);
}

template <typename T, typename Binary_Functor>
T SegmentTree<T, Binary_Functor>::query(int left, int right) const {
	return query(left, right, 0, n - 1);
}

template <typename T, typename Binary_Functor>
T SegmentTree<T, Binary_Functor>::query(int left, int right, int tleft, int tright, size_t kod) const {
	if (tleft == left && tright == right)
		return t[kod];
	int m = (tleft + tright) / 2;
	if (right <= m)
		return query(left, right, tleft, m, 2 * kod);
	if (left > m)
		return query(left, right, m + 1, tright, 2 * kod + 1);
	return mFunction(query(left, m, tleft, m, 2 * kod), query(m + 1, right, m + 1, tright, 2 * kod + 1));
}

template <typename T, typename Binary_Functor>
void SegmentTree<T, Binary_Functor>::update(size_t i, value_type x) {
	size_t j = findIndex(i);
	value_type m = x - t[j];
	while (j != 0) {
		t[j] += m;
		j /= 2;
	}
}

template <typename T, typename Binary_Functor>
size_t SegmentTree<T, Binary_Functor>::findIndex(int i) const {
	return findIndex(i, 0, n - 1);
}

template <typename T, typename Binary_Functor>
size_t SegmentTree<T, Binary_Functor>::findIndex(int i, int tleft, int tright, size_t kod) const {
	if (tleft == tright)
		return kod;
	int m = (tleft + tright) / 2;
	if (i <= m)
		return findIndex(i, tleft, m, 2 * kod);
	return findIndex(i, m + 1, tright, 2 * kod + 1);
}
