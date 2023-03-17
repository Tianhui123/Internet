#pragma once
#include<iostream>
#include"noCopy.h"

namespace TianHui
{
	/// <summary>
	/// 没有考虑线程安全和异常处理问题
	/// </summary>
	/// <typeparam name="Ty"></typeparam>
	template<typename Ty>
	class m_ptr
	{
	public:
		explicit m_ptr<Ty>() :
			ptr_{ nullptr },
			count_{ nullptr },
			boolArry_(false)
		{
			//std::cout << "cons()" << std::endl;

		}

		explicit m_ptr<Ty>(size_t n) :
			ptr_{ new Ty[n]{} },
			count_{ new size_t(1) },
			boolArry_(true)
		{
			//std::cout << "cons(int)" << std::endl;

		}

		//默认只能new一块内存
		explicit m_ptr<Ty>(Ty* p) :
			ptr_(p),
			count_{ new size_t(1) },
			boolArry_(false)
		{

			//std::cout << "cons(Ty*p)" << std::endl;
		}


		//传入new的内存大小
		explicit m_ptr<Ty>(Ty* p, size_t size) :
			ptr_(p),
			count_{ new size_t(1) },
			boolArry_(false)
		{
			if (size > 1)
				boolArry_ = true;

			//std::cout << "cons(Ty*p , size_t size)" << std::endl;
		}



		m_ptr<Ty>(const m_ptr<Ty>& p)
		{
			//std::cout << "cons(const m_ptr<Ty>& p)" << std::endl;


			ptr_ = p.ptr_;
			count_ = p.count_;
			boolArry_ = p.boolArry_;

			++* count_;

		}

		m_ptr<Ty>& operator=(const m_ptr<Ty>& p)
		{
			if (this == &p)
				return *this;

			isFree();

			ptr_ = p.ptr_;
			count_ = p.count_;
			boolArry_ = p.boolArry_;

			++* count_;

			return *this;
		}

		m_ptr<Ty>& operator=(m_ptr<Ty>&& p) noexcept
		{
			if (this == &p)
				return*this;

			isFree();

			ptr_ = p.ptr_;
			p.ptr_ = nullptr;

			count_ = p.count_;
			p.count_ = nullptr;

			boolArry_ = p.boolArry_;

			return *this;
		}

		m_ptr<Ty>(m_ptr<Ty>&& p) noexcept :
			ptr_{ p.ptr_ },
			count_{ p.count_ }
		{
			boolArry_ = p.boolArry_;
			p.ptr_ = nullptr;
			p.count_ = nullptr;
		}

		~m_ptr<Ty>()
		{
			//std::cout << "~~cons()";
			isFree();

		}

		size_t getCount()const
		{
			return *count_;
		}

		Ty* getPtr()const
		{
			return ptr_;
		}

		Ty& operator[](size_t index_)
		{
			return ptr_[index_];
		}

		Ty& operator*()const
		{
			if (ptr_)
				return *ptr_;
			else
				throw std::runtime_error("无效指针");
		}

		Ty* operator->()const
		{
			return ptr_;
		}

		m_ptr<Ty> operator+(int n)
		{
			return m_ptr<Ty>(ptr_ + n);
		}

		m_ptr<Ty> operator++()
		{
			return m_ptr<Ty>(++ptr_);
		}

		m_ptr<Ty> operator++(int)
		{
			return m_ptr<Ty>(ptr_++);
		}

		m_ptr<Ty> operator--()
		{
			return m_ptr<Ty>(--ptr_);
		}

		m_ptr<Ty> operator--(int)
		{
			return m_ptr<Ty>(ptr_--);
		}

		bool operator!=(const m_ptr<Ty>& p)
		{
			return ptr_ != p.ptr_;
		}

		bool operator!=(const Ty* p)
		{
			return ptr_ != p;
		}

		bool operator==(const m_ptr<Ty>& p)
		{
			return ptr_ == p.ptr_;
		}

		bool operator==(const Ty* p)
		{
			return ptr_ == p;
		}

	private:
		Ty* ptr_;
		size_t* count_;
		bool boolArry_;

	private:

		bool isArry()
		{
			return boolArry_;
		}

		// 判断是否释放掉内存
		void isFree()
		{

			if (this == nullptr||count_ == nullptr)
				return;

			if ((-- * count_) == 0)
			{


				if (!isArry())
				{
					if (this->ptr_)
						delete this->ptr_;
					ptr_ = nullptr;

					//std::cout << "noArry" << std::endl;
				}
				else
				{
					//std::cout << "Arry" << std::endl;
					if (this->ptr_)
						delete[]this->ptr_;
					ptr_ = nullptr;
				}

				delete this->count_;
				count_ = nullptr;

			}

		}



	};
}
template<typename Ty>
using mPtr = TianHui::m_ptr<Ty>;
