#pragma once

// https://riptutorial.com/cplusplus/example/19276/variadic-template-data-structures

// A0. ==> Incomplete Declaration
template <typename... T>
struct MyTuple;

// A1. - Base Case
template <>
struct MyTuple<> {};

// B0 ==> Incomplete Declaration
template <size_t idx, typename T>
struct GetHelper;

// A2. - MyTuple: Recursive Template Definition
template <typename Tfirst, typename... Trest>
struct MyTuple<Tfirst, Trest...>
{
	MyTuple(const Tfirst& first, const Trest&... rest)
	: m_first(first)
	, m_rest(rest...)
	{}

	template <size_t idx>
	auto get()
	{
		return GetHelper<idx, MyTuple<Tfirst, Trest...>>::get(*this);
	}

	Tfirst				m_first;
	MyTuple<Trest...>	m_rest;
};

// B1. - Base Case
template<typename Tfirst, typename... Trest>
struct GetHelper<0, MyTuple<Tfirst, Trest... >>
{
	static Tfirst get(MyTuple<Tfirst, Trest...>& data)
	{
		return data.m_first;
	}
};

// B2. - recursive Template Definition
template<size_t idx, typename Tfirst, typename... Trest>
struct GetHelper<idx, MyTuple<Tfirst, Trest... >>
{
	static auto get(MyTuple<Tfirst, Trest...>& data)
	{
		return GetHelper<idx - 1, MyTuple<Trest...>>::get(data.m_rest);
	}
};