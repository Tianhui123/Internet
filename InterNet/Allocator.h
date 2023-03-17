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

		T* allocate(size_t size)	// ֻ�����ڴ濪��
		{
			return (T*)malloc(sizeof(T) * size);
		}

		void deallocate(T* ptr)	// ֻ�����ͷ��ڴ�
		{
			if (ptr)
				free(ptr);
			ptr = nullptr;
		}

		template<typename Ty>
		void constructor(Ty* p, Ty&& val)	// ֻ���������
		{
			new(p) Ty(std::forward<Ty>(val));
		}


		void destory(T* p)	// ֻ�����������
		{

			if (p)
				p->~T();
		}



	};

}