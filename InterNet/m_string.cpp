#include"m_string.h"

namespace TianHui
{
	using m_string = TianHui::m_string;
	m_string::m_string(const char* str) :
		_string{mPtr<char>( new char[strlen(str)+1],2)},
		_size(0)
	{
		_size = strlen(str);
		strcpy_s(_string.getPtr(), _size + 1, str);
	}

	m_string::m_string(size_t size, char p) :
		_string{ mPtr<char>(new char[size + 1]{},2) },
		_size(size)
	{
		char* i = _string.getPtr();
		for (; i != _string.getPtr() + _size; ++i)
			*i = p;
		*i = '\0';
		i = nullptr;

	}

	m_string::~m_string()
	{


	}

	m_string::m_string(const m_string& p) :
		_string{ mPtr<char>(new char[p._size + 1]{},2) },
		_size(p._size)
	{
		//_string = new char[_size + 1]{};
		strcpy_s(_string.getPtr(), _size + 1, p._string.getPtr());

	}

	m_string m_string::operator+(const char*p)
	{
		if (!p)
			return *this;
		size_t s1 = strlen(p);
		if (s1 == 0)
			return *this;

		size_t size = s1 + _size;

		char* ptr = new char[size + 1]{};

		for (char* i = _string.getPtr(), *temp = ptr; i != _string.getPtr() + _size;)
			*temp++ = *i++;

		char* temp = ptr + _size;
		for (const char* i = p; i != p+ s1;)
			*temp++ = *i++;

		*temp = '\0';

		return m_string(ptr,size);
	}

	void m_string::operator+=(const char* p)
	{
		if (!p)
			return;
		size_t s1 = strlen(p);
		if (s1 == 0)
			return;

		size_t size = s1 + _size;

		char* ptr = new char[size + 1]{};

		for (char* i = _string.getPtr(), *temp = ptr; i != _string.getPtr() + _size;)
			*temp++ = *i++;

		char* temp = ptr + _size;
		for (const char* i = p; i != p + s1;)
			*temp++ = *i++;

		*temp = '\0';


		_string = mPtr<char>{ ptr };
		_size = size;

		ptr = nullptr;

		return;
	}

	m_string::m_string(m_string&& p)noexcept :
		_string(p._string),
		_size(p._size)
	{

	}

	m_string::Iterator m_string::Iterator::operator++()
	{

		return Iterator(++_past);
	}

	m_string::Iterator m_string::Iterator::operator++(int)
	{
		return Iterator(_past++);
	}

	m_string::Iterator m_string::Iterator::operator--()
	{
		return Iterator(--_past);
	}

	m_string::Iterator m_string::Iterator::operator--(int)
	{
		return Iterator(_past--);
	}

	const char& m_string::Iterator::operator*()
	{
		if (_past)
			return *_past;
		else
		{
			throw std::runtime_error("无效指针");
		}
	}


	m_string& m_string:: operator=(const m_string& p)
	{
		if (this == &p)
			return *this;

		_size = p._size;

		/*if (_string) {
			delete[]_string;
		}*/


		_string = mPtr<char>{ new char[_size + 1]{} };

		strcpy_s(_string.getPtr(), _size + 1, p._string.getPtr());
		return *this;
	}


	void m_string::operator=(m_string&& p)noexcept
	{
		if (this == &p)
			return;

		_size = p._size;
		_string = p._string;
		//p._string = nullptr;
		return ;
	}




	m_string::m_string(char* ptr, size_t size) :
		_string{ mPtr<char>{ptr,2} },
		_size(size)
	{
		ptr = nullptr;
	}


	m_string operator+(const m_string& a, const m_string& b)
	{

		size_t size = a._size + b._size;
		char* ptr = new char[size + 1]{};
		strcpy_s(ptr, a._size + 1, a._string.getPtr());

		char* i = ptr + a._size;
		strcpy_s(i, b._size + 1, b._string.getPtr());

		return m_string(ptr, size);

	}

	m_string operator+(const char* ptr, const m_string& str)
	{
		if (!ptr)
			return str;
		size_t ss = strlen(ptr);
		if (ss == 0)
			return str;

		size_t size = str._size + ss;

		char* temp = new char[size + 1]{};

		char* mv = temp;
		for (const char* i = ptr; i != ptr + ss;)
			*mv++ = *i++;

		mv = temp + ss;
		for (char* i = str._string.getPtr(); i != str._string.getPtr() + str._size;)
			*mv++ = *i++;
		*mv = '\0';

		return m_string(temp, size);
	}


	char& m_string::operator[](size_t index_)
	{
		if (index_ < _size)
			return _string[index_];
		else
			throw std::runtime_error("数组越界");
	}

	char* m_string::c_str()const
	{
		return _string.getPtr();
	}

	std::ostream& operator<<(std::ostream& out, const m_string& a)
	{
		out << a._string.getPtr();
		return out;
	}


}