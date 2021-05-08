#pragma once
#include <utility>
#define TREAP_REVERSE

class ImplicitTreapNode {
public:
	ImplicitTreapNode(int v = 0, int p = 0, ImplicitTreapNode* lf = nullptr, ImplicitTreapNode* rg = nullptr)
		:mValue(v)
		, mPriority(p)
		, mLeft(lf)
		, mRight(rg)
		, mSize(1)
#ifdef TREAP_REVERSE
		, mReverse(false)
#endif // TREAP_REVERSE
	{ update(); }
public:
	void changeLeft(ImplicitTreapNode* node) { updateParams(); mLeft = node; update(); }
	void changeRight(ImplicitTreapNode* node) { updateParams(); mRight = node; update(); }
	ImplicitTreapNode* getLeft() { updateParams(); return mLeft; }
	const ImplicitTreapNode* getLeft() const { updateParams(); return mLeft; }
	ImplicitTreapNode* getRight() { updateParams(); return mRight; }
	const ImplicitTreapNode* getRight() const { updateParams(); return mRight; }
	size_t size() const { return mSize; }
	size_t leftSize() const { updateParams(); return (mLeft != nullptr ? mLeft->mSize : 0); }
	size_t rightSize() const { updateParams(); return (mRight != nullptr ? mRight->mSize : 0); }
	const int& getValue() const { updateParams(); return mValue; }
#ifdef TREAP_REVERSE
	void reverse() { mReverse ^= true; }
#endif // TREAP_REVERSE
public:
	static ImplicitTreapNode* merge(ImplicitTreapNode*, ImplicitTreapNode*);
	static std::pair<ImplicitTreapNode*, ImplicitTreapNode*> split(ImplicitTreapNode*, size_t);
private:
	void updateParams() const;
	void update() { mSize = leftSize() + rightSize() + 1; }
private:
	int mValue;
	int mPriority;
	mutable ImplicitTreapNode* mLeft;
	mutable ImplicitTreapNode* mRight;
	size_t mSize;
#ifdef TREAP_REVERSE
private:
	mutable bool mReverse;
#endif // TREAP_REVERSE
};


class ImplicitTreap {
public:
	ImplicitTreap();
	~ImplicitTreap();
	void pushBack(const int&);
	void insert(const int&, size_t);
	void remove(size_t);
#ifdef TREAP_REVERSE
	void reverse(size_t, size_t);
#endif // TREAP_REVERSE
	int keyOfOrder(size_t) const;
	int maxDepth() const;
	bool empty() const { return size() == 0; }
	size_t size() const { return mRoot != nullptr ? mRoot->size() : 0; }
private:
	void deallocate(ImplicitTreapNode*);
private:
	int keyOfOrder(size_t, ImplicitTreapNode*) const;
	int maxDepth(const ImplicitTreapNode*) const;
private:
	ImplicitTreapNode* mRoot;
};