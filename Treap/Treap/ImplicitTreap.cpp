#include "ImplicitTreap.h"
#include <algorithm>
#include <chrono>
#include <random>

ImplicitTreapNode* ImplicitTreapNode::merge(ImplicitTreapNode* node1, ImplicitTreapNode* node2) {
	if (node1 == nullptr) {
		return node2;
	}
	if (node2 == nullptr) {
		return node1;
	}
	ImplicitTreapNode* ret{};
	if (node1->mPriority > node2->mPriority) {
		ret = new ImplicitTreapNode(node1->getValue(), node1->mPriority, node1->getLeft());
		ret->changeRight(merge(node1->getRight(), node2));
		delete node1;
	}
	else {
		ret = new ImplicitTreapNode(node2->getValue(), node2->mPriority, nullptr, node2->getRight());
		ret->changeLeft(merge(node1, node2->getLeft()));
		delete node2;
	}
	return ret;
}

std::pair<ImplicitTreapNode*, ImplicitTreapNode*> ImplicitTreapNode::split(ImplicitTreapNode* node, size_t x) {
	if (node == nullptr || x <= 0) {
		return std::make_pair(nullptr, node);
	}
	if (x >= node->size()) {
		return std::make_pair(node, nullptr);
	}
	ImplicitTreapNode* retleft{}, * retright{};
	if (node->leftSize() < x) {
		std::pair<ImplicitTreapNode*, ImplicitTreapNode*> pright = ImplicitTreapNode::split(node->getRight(), x - node->leftSize() - 1);
		retleft = node;
		node->changeRight(pright.first);
		retright = pright.second;
	}
	else {
		std::pair<ImplicitTreapNode*, ImplicitTreapNode*> pleft = ImplicitTreapNode::split(node->getLeft(), x);
		retleft = pleft.first;
		retright = node;
		retright->changeLeft(pleft.second);
	}
	return std::make_pair(retleft, retright);
}

void ImplicitTreapNode::updateParams() const {
#ifdef TREAP_REVERSE
	if (mReverse) {
		std::swap(mLeft, mRight);
		if (mLeft != nullptr)
			mLeft->reverse();
		if (mRight != nullptr)
			mRight->reverse();
		mReverse = false;
	}
#endif // TREAP_REVERSE
}

ImplicitTreap::ImplicitTreap()
	: mRoot(nullptr)
{}

ImplicitTreap::~ImplicitTreap() {
	deallocate(mRoot);
}

void ImplicitTreap::pushBack(const int& value) {
	insert(value, size());
}

void ImplicitTreap::insert(const int& value, size_t ind) {
	static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	ImplicitTreapNode* my_node = new ImplicitTreapNode(value, rng() % (1ll << 31));
	if (mRoot == nullptr) {
		mRoot = my_node;
		return;
	}
	std::pair<ImplicitTreapNode*, ImplicitTreapNode*> p = ImplicitTreapNode::split(mRoot, ind);
	mRoot = ImplicitTreapNode::merge(ImplicitTreapNode::merge(p.first, my_node), p.second);
}

void ImplicitTreap::remove(size_t ind) {
	std::pair<ImplicitTreapNode*, ImplicitTreapNode*> first_split_pair = ImplicitTreapNode::split(mRoot, ind);
	std::pair<ImplicitTreapNode*, ImplicitTreapNode*> second_split_pair = ImplicitTreapNode::split(first_split_pair.second, 1);
	if (second_split_pair.first == nullptr) {
		throw std::exception("Element not found");
	}
	delete second_split_pair.first;
	if (empty()) {
		mRoot = nullptr;
		return;
	}
	mRoot = ImplicitTreapNode::merge(first_split_pair.first, second_split_pair.second);
}

#ifdef TREAP_REVERSE
void ImplicitTreap::reverse(size_t left, size_t right) {
	if (left < 0) {
		left = 0;
	}
	if (right > size()) {
		right = size();
	}
	if (right <= left) {
		return;
	}
	std::pair<ImplicitTreapNode*, ImplicitTreapNode*> p1 = ImplicitTreapNode::split(mRoot, left);
	std::pair<ImplicitTreapNode*, ImplicitTreapNode*> p2 = ImplicitTreapNode::split(p1.second, right - left);
	p2.first->reverse();
	mRoot = ImplicitTreapNode::merge(p1.first, ImplicitTreapNode::merge(p2.first, p2.second));
}
#endif // TREAP_REVERSE

int ImplicitTreap::keyOfOrder(size_t ind) const {
	if (ind >= size()) {
		throw std::exception("Index out of bounds");
	}
	ind++;
	return keyOfOrder(ind, mRoot);
}

int ImplicitTreap::keyOfOrder(size_t ind, ImplicitTreapNode* root) const {
	size_t leftcount = root->leftSize();
	if (leftcount >= ind) {
		return keyOfOrder(ind, root->getLeft());
	}
	if (ind == leftcount + 1) {
		return root->getValue();
	}
	return keyOfOrder(ind - leftcount - 1, root->getRight());
}

int ImplicitTreap::maxDepth() const {
	return maxDepth(mRoot);
}

int ImplicitTreap::maxDepth(const ImplicitTreapNode* node) const {
	if (node == nullptr) {
		return 0;
	}
	return std::max(maxDepth(node->getRight()), maxDepth(node->getLeft())) + 1;
}

void ImplicitTreap::deallocate(ImplicitTreapNode* node) {
	if (node == nullptr) {
		return;
	}
	if (node->getLeft() != nullptr) {
		deallocate(node->getLeft());
	}
	if (node->getRight() != nullptr) {
		deallocate(node->getRight());
	}
	delete node;
}