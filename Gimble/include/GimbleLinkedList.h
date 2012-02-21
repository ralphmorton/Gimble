
#ifndef _GIMBLE_LINKEDLIST_
#define _GIMBLE_LINKEDLIST_

#include "GimbleReqs.h"

namespace Gimble
{
	template <class T> class LinkedListNode
	{
	private:
		template <class T> friend class LinkedList;

	private:
		T mItem;
		LinkedListNode<T> *mPrev;
		LinkedListNode<T> *mNext;

	private:
		LinkedListNode(T item) : mItem(item), mPrev(0), mNext(0)
		{

		}

		~LinkedListNode()
		{
			if (mNext)
				delete mNext;
		}

	public:
		inline T item() const { return mItem; }
		inline LinkedListNode<T> *prev() { return mPrev; }
		inline LinkedListNode<T> *next() { return mNext; }
	};

	template <class T> class LinkedList
	{
	private:
		template <class T> friend class LinkedListNode;

	private:
		LinkedListNode<T> *mFirst;
		LinkedListNode<T> *mLast;

	public:
		LinkedList() : mFirst(0), mLast(0) { }
		~LinkedList() { delete mFirst; }

		inline void insertLast(T t)
		{
			if (!mFirst)
			{
				mFirst = mLast = new LinkedListNode<T>(t);
				return;
			}

			insertAfter(t, mLast);
		}

		inline void insertFirst(T t)
		{
			if (!mFirst)
			{
				mFirst = mLast = new LinkedListNode<T>(t);
				return;
			}

			insertBefore(t, mFirst);
		}

		inline void insertBefore(T t, LinkedListNode<T> *n)
		{
			LinkedListNode<T> *nnew = new LinkedListNode<T>(t);

			LinkedListNode<T> *temp = n->mPrev; 
			if (temp)
				temp->mNext = nnew;

			nnew->mPrev = temp;
			nnew->mNext = n;
			n->mPrev = nnew;

			if (!temp) mFirst = nnew;
		}

		inline void insertAfter(T t, LinkedListNode<T> *n)
		{ 
			LinkedListNode<T> *nnew = new LinkedListNode<T>(t);

			LinkedListNode<T> *temp = n->mNext; 
			if (temp)
				temp->mPrev = n;

			nnew->mPrev = n;
			nnew->mNext = temp;
			n->mNext = nnew;

			if (!temp) mLast = nnew;
		}

		inline void remove(LinkedListNode<T> *n)
		{
			LinkedListNode<T> *last = n->mPrev;
			LinkedListNode<T> *next = n->mNext;

			if (last) last->mNext = next;
			if (next) next->mPrev = last;

			if (n == mFirst) mFirst = next;
			if (n == mLast) mLast = last;

			n->mPrev = n->mNext = 0;
			delete n;
		}

		inline LinkedListNode<T> *first() const { return mFirst; }
		inline LinkedListNode<T> *last() const { return mLast; }
	};
}

#endif