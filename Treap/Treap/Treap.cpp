#include "Treap.h"
#include <algorithm>
#include <chrono>
#include <random>

TreapNode* TreapNode::merge(TreapNode* node1, TreapNode* node2)
{
	if (node1 == nullptr)
		return node2;
	if (node2 == nullptr)
		return node1;
	if (node1->key > node2->key)
		std::swap(node1, node2);
	TreapNode* ret{};
	if (node1->mPriority > node2->mPriority)
	{
		ret = new TreapNode(node1->key, node1->mPriority, node1->getLeft());
		ret->changeRight(merge(node1->getRight(), node2));
		delete node1;
	}
	else
	{
		ret = new TreapNode(node2->key, node2->mPriority, nullptr, node2->getRight());
		ret->changeLeft(merge(node1, node2->getLeft()));
		delete node2;
	}
	return ret;
}

std::pair<TreapNode*, TreapNode*> TreapNode::split(TreapNode* node, int x)
{
	if (node == nullptr)
		return std::make_pair(nullptr, nullptr);
	TreapNode* retleft{}, * retright{};
	if (node->key < x)
	{
		std::pair<TreapNode*, TreapNode*> pright = TreapNode::split(node->getRight(), x);
		retleft = node;
		node->changeRight(pright.first);
		retright = pright.second;
	}
	else
	{
		std::pair<TreapNode*, TreapNode*> pleft = TreapNode::split(node->getLeft(), x);
		retleft = pleft.first;
		retright = node;
		retright->changeLeft(pleft.second);
	}
	return std::make_pair(retleft, retright);
}

Treap::Treap()
	: mRoot(nullptr)
{}

Treap::~Treap()
{
	deallocate(mRoot);
}

void Treap::insert(const int& value)
{
	if (find(value))
		return;
	static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	TreapNode* my_node = new TreapNode(value, rng() % (1ll << 31));
	if (mRoot == nullptr)
	{
		mRoot = my_node;
		return;
	}
	std::pair<TreapNode*, TreapNode*> p = TreapNode::split(mRoot, value);
	mRoot = TreapNode::merge(p.second, TreapNode::merge(p.first, my_node));
}

void Treap::remove(const int& value)
{
	std::pair<TreapNode*, TreapNode*> first_split_pair = TreapNode::split(mRoot, value);
	std::pair<TreapNode*, TreapNode*> second_split_pair = TreapNode::split(first_split_pair.second, value + 1);
	if (second_split_pair.first == nullptr)
		throw std::exception("Element not found");
	delete second_split_pair.first;
	if (empty())
	{
		mRoot = nullptr;
		return;
	}
	mRoot = TreapNode::merge(first_split_pair.first, second_split_pair.second);
}

bool Treap::find(const int& value) const
{
	return find(value, mRoot);
}

bool Treap::find(const int& value, TreapNode* root) const
{
	if (root == nullptr)
		return false;
	if (value == root->key)
		return true;
	if (value < root->key)
		return find(value, root->getLeft());
	return find(value, root->getRight());
}

int Treap::keyOfOrder(size_t ind) const
{
	if (ind >= size())
		throw std::exception("Index out of bounds");
	ind++;
	return keyOfOrder(ind, mRoot);
}

int Treap::keyOfOrder(size_t ind, const TreapNode* root) const
{
	size_t leftcount = root->leftSize();
	if (leftcount >= ind)
		return keyOfOrder(ind, root->getLeft());
	if (ind == leftcount + 1)
		return root->key;
	return keyOfOrder(ind - leftcount - 1, root->getRight());
}

size_t Treap::orderOfKey(const int& key) const
{
	return orderOfKey(key, mRoot) - 1;
}

size_t Treap::orderOfKey(const int& key, const TreapNode* root) const
{
	if (root == nullptr)
		throw std::exception("Element not found");
	if (key == root->key)
		return 1 + root->leftSize();
	if (key < root->key)
		return orderOfKey(key, root->getLeft());
	return 1 + root->leftSize() + orderOfKey(key, root->getRight());
}

int Treap::maxDepth() const
{
	return maxDepth(mRoot);
}

int Treap::maxDepth(const TreapNode* node) const
{
	if (node == nullptr)
		return 0;
	return std::max(maxDepth(node->getRight()), maxDepth(node->getLeft())) + 1;
}

void Treap::deallocate(TreapNode* node)
{
	if (node == nullptr)
		return;
	if (node->getLeft() != nullptr)
		deallocate(node->getLeft());
	if (node->getRight() != nullptr)
		deallocate(node->getRight());
	delete node;
}