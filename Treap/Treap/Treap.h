#pragma once
#include <utility>

class TreapNode
{
public:
	TreapNode(int k = 0, int p = 0, TreapNode* lf = nullptr, TreapNode* rg = nullptr)
		: key(k)
		, mPriority(p)
		, mLeft(lf)
		, mRight(rg)
		, mSize(1)
	{ update(); }
public:
	void changeLeft(TreapNode* node) { update(); mLeft = node; }
	void changeRight(TreapNode* node) { update(); mRight = node; }
	TreapNode* getLeft() { return mLeft; }
	const TreapNode* getLeft() const { return mLeft; }
	TreapNode* getRight() { return mRight; }
	const TreapNode* getRight() const { return mRight; }
	size_t size() const { return mSize; }
	size_t leftSize() const { return (mLeft != nullptr ? mLeft->mSize : 0); }
	size_t rightSize() const { return (mRight != nullptr ? mRight->mSize : 0); }
public:
	static TreapNode* merge(TreapNode*, TreapNode*);
	static std::pair<TreapNode*, TreapNode*> split(TreapNode*, int);
private:
	void update() { mSize = leftSize() + rightSize() + 1; }
public:
	int key;
private:
	int mPriority;
	TreapNode* mLeft;
	TreapNode* mRight;
	size_t mSize;
};

class Treap
{
public:
	Treap();
	~Treap();
	void insert(const int&);
	void remove(const int&);
	bool find(const int&) const;
	int keyOfOrder(size_t) const;
	size_t orderOfKey(const int&) const;
	int maxDepth() const;
	bool empty() const { return size() == 0; }
	size_t size() const { return mRoot != nullptr ? mRoot->size() : 0; }
private:
	void deallocate(TreapNode*);
private:
	bool find(const int&, TreapNode*) const;
	int keyOfOrder(size_t, const TreapNode*) const;
	size_t orderOfKey(const int&, const TreapNode*) const;
	int maxDepth(const TreapNode*) const;
private:
	TreapNode* mRoot;
};