#pragma once
#include"Allocator.h"

namespace TianHui
{

	template<typename T, typename Alloc = Allocator<T>>
	class m_vector
	{
		#define initSize 10
		using type = typename removeRef<T>::type;


	public:

		class m_iterator
		{
		public:

			explicit m_iterator() = default;
			explicit m_iterator(type* ptr) :
				ptr_(ptr) {}

			~m_iterator()
			{
				ptr_ = nullptr;
			}

			type operator*()
			{
				if (ptr_)
					return *ptr_;
				else
					throw std::runtime_error("无效指针");
			}

			m_iterator(const m_iterator& p)
			{
				ptr_ = p.ptr_;
			}

			m_iterator& operator=(const m_iterator& p)
			{
				if (this == &p)
					return*this;
				ptr_ = p.ptr_;
				p.ptr_ = nullptr;
				return *this;
			}

			m_iterator operator++()
			{
				return m_iterator(++ptr_);
			}

			m_iterator operator++(int)
			{
				return m_iterator(ptr_++);
			}

			m_iterator operator--()
			{
				return m_iterator(--ptr_);
			}

			m_iterator operator--(int)
			{
				return m_iterator(ptr_--);
			}

			bool operator!=(const m_iterator&p)
			{
				return ptr_ != p.ptr_;
			}

		private:
			type* ptr_;

		};


	public:

		explicit m_vector():
			first_{  },
			move_{},
			end_{},
			alloc_{}
		{
			//把内存开辟和对象构造分开
			first_ = alloc_.allocate(initSize);
			move_ = first_;
			end_ = move_ + initSize;
		}

		explicit m_vector(const size_t size) :
			first_{ },
			move_{},
			end_{},
			alloc_{}
		{
			//把内存开辟和对象构造分开
			first_ = alloc_.allocate(size);
			move_ = first_;
			end_ = move_ + size;
		}



		~m_vector()
		{
			//析构容器有效的元素，然后释放整个底层的内存空间
			if (first_&&move_!=first_)
				for (type* i = first_; i != move_; ++i)
				{
					alloc_.destory(i);	// 把first指向的数组里的有效值析构掉
				}

			alloc_.deallocate(first_);	// 释放堆上的数组内存
			first_ = move_ = end_ = nullptr;

		}

		m_vector(const m_vector<type>& rhs) :
			first_{},
			move_{},
			end_{},
			alloc_{}
		{

			int size = { static_cast<int>(rhs.end_ - rhs.first_) };

			first_ = alloc_.allocate(size);
			move_ = first_;
			end_ = first_ + size;

			for (type* i = rhs.first_; i != rhs.move_; ++i, ++move_)
				alloc_.constructor(move_, *i);


		}

		m_vector<type>& operator=(const m_vector<type>& rhs)
		{

			if (this == &rhs)
				return *this;

			if (first_) {
				for (type* i = first_; i != move_; ++i)
				{
					alloc_.destory(i);
				}

				alloc_.deallocate(first_);
			}

			int size = { static_cast<int>(rhs.end_ - rhs.first_) };

			first_ = alloc_.allocate(size);
			move_ = first_;
			end_ = first_ + size;

			for (type* i = rhs.first_; i != rhs.move_; ++i, ++move_)
			{

				alloc_.constructor(move_, *i);
			}


			return *this;

		}


		const type* operator->()const
		{
			return first_;
		}

		void push(type&& a)
		{
			if (move_ == end_)
				expend();

			alloc_.constructor(move_++, std::forward<type>(a));

		}

		void push(const type& a)
		{
			if (move_ == end_)
				expend();

			//alloc_.constructor(move_++, a);
			new(move_++) type(a);
		}

		void pop()
		{
			//把对象析构和释放内存分开
			if (move_ == end_)
				return;

			--move_;
			alloc_.destory(move_);
		}

		void clear()
		{
			for (size_t i = 0; i < size(); ++i)
				pop();
		}

		bool inline empty()const 
		{ 
			return move_ == end_;
		}

		bool inline full()const 
		{
			return move_ == end_;
		}

		size_t inline size()const
		{ 
			return move_ - first_; 
		}

		const type& operator[](const size_t& a)
		{
			size_t siz = size();

			if (a < siz)
				return *(first_+a);
			else
			{
				std::cerr << "数组下标越界" << std::endl;
				throw std::runtime_error("数组越界");
			}
		}

		m_iterator begin()
		{
			return m_iterator(first_);
		}

		m_iterator end()
		{
			return m_iterator(end_);
		}


	private:
		type* first_;
		type* move_;
		type* end_;
		Alloc alloc_;
		//空间调整
		void expend()
		{

			size_t size = { static_cast<size_t>(end_ - first_) };

			type* temp = alloc_.allocate(2 * size);
			for (type* i = first_, *j = temp; i != move_; ++i, ++j)
			{
				alloc_.constructor(j, std::move(*i));
			}


			if (first_)
			{
				for (type* i = first_; i != move_; ++i)
				{
					alloc_.destory(i);
				}

				alloc_.deallocate(first_);
				first_ = nullptr;
			}

			first_ = temp;
			move_ = first_ + size;
			end_ = first_ + 2 * size;

			temp = nullptr;

		}

	};


}
template<typename T>
using mVector = TianHui::m_vector<T>;