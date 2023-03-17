#pragma once

namespace TianHui
{
	template<typename _T = int>
	class removeRef
	{
	public:
		using type = _T;
	};

	template<typename _T>
	class removeRef<_T&>
	{
	public:
		using type = _T;
	};

	template<typename _T>
	class removeRef<_T&&>
	{
	public:
		using type = _T;
	};


	class noCopy
	{
	protected:
		noCopy() = default;
		~noCopy() = default;
	public:
		noCopy(const noCopy&) = delete;
		noCopy& operator=(const noCopy&) = delete;

	};
}