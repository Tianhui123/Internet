#pragma once
#include<iostream>
#include"m_ptr.h"

namespace TianHui
{

	class m_string final
	{
		template<typename T>
		using mPtr = m_ptr<T>;

	public:
		m_string(const char* str = "\0");

		explicit m_string(char* ptr, size_t size);

		explicit m_string(size_t size, char p);

		~m_string();

		m_string(const m_string&);

		m_string operator+(const char*);

		void operator+=(const char*);

		m_string(m_string&&)noexcept;

		struct Iterator
		{
			Iterator(char* p) :
				_past(p)
			{
				p = nullptr;
			}
			inline~Iterator()
			{
				_past = nullptr;
			}

			template<typename Ty>
			Iterator(Ty&& p)noexcept
			{
				_past = p._past;
				p._past = nullptr;
			}

			Iterator operator++();
			Iterator operator++(int);
			Iterator operator--();
			Iterator operator--(int);
			const char& operator*();

			bool operator!=(const Iterator& p)const
			{
				return _past != p._past;
			}

		private:
			char* _past;
		};


	public:

		inline Iterator begin()const
		{
			return Iterator(_string.getPtr());
		}

		inline Iterator end()const
		{
			return Iterator(_string.getPtr() + _size);
		}

		inline bool empty()const
		{
			return _size == 0;
		}

		inline size_t size()const
		{
			return _size;
		}

		m_string& operator=(const m_string& p);		

		void operator=(m_string&& p)noexcept;
		
		char& operator[](size_t);

		char* c_str()const;

		friend m_string operator+(const m_string& , const m_string& );

		friend m_string operator+(const char*, const m_string&);

		friend std::ostream& operator<<(std::ostream& , const m_string& );

	private:
		

	private:
		//char* _string ;
		mPtr<char>_string;
		size_t _size;
	};



}


using mString = TianHui::m_string;