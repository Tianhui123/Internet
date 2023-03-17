#pragma once
#include"noCopy.h"
namespace TianHui
{
	using noCopy = TianHui::noCopy;
	template<typename T>
	class Allocator :noCopy
	{
	public:
		Allocator() = default;
		~Allocator() = default;

		T* allocate(size_t size)	// 只负责内存开辟
		{
			return (T*)malloc(sizeof(T) * size);
		}

		void deallocate(T* ptr)	// 只负责释放内存
		{
			if (ptr)
				free(ptr);
			ptr = nullptr;
		}

		template<typename Ty>
		void constructor(Ty* p, Ty&& val)	// 只负责对象构造
		{
			new(p) Ty(std::forward<Ty>(val));
		}


		void destory(T* p)	// 只负责对象析构
		{

			if (p)
				p->~T();
		}



	};

}