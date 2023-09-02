#include <iostream>
#include <vector>

class TempClassB
{
	public:
		TempClassB() = default;

		//TempClassB(int ii)
		//: i(ii)
		//{
		//	
		//}

		//TempClassB(const TempClassB& rhs) = default;

		//TempClassB(TempClassB&& rhs) noexcept
		//{
		//	i = rhs.i;
		//};
		//
		//TempClassB& operator=(TempClassB&& rhs) noexcept
		//{
		//	i = rhs.i;
		//	return *this;
		//}

		//TempClassB& operator=(const TempClassB& rhs) 
		//{
		//	i = rhs.i;
		//	return *this;
		//};

		~TempClassB() = default;

		int i;
	protected:
	private:
};

class TempClass
{
	public:
		TempClass() = default;

		TempClass(const TempClass& rhs) = default;

		//TempClass(const TempClass&&) {}
		//TempClass& operator=(TempClass&& rhs) noexcept
		//{
		//	return *this;
		//}

		~TempClass() = default;

	protected:
	private:
		std::vector<std::unique_ptr<TempClassB>> m_vList;
};

int main(int argc, char* argv[])
{
	int32_t iInt32 = 0;
	iInt32 |= (44 & 0xFF) << 24;
	iInt32 |= (33 & 0xFF) << 16;
	iInt32 |= (22 & 0xFF) << 8;
	iInt32 |= (11 & 0xFF);

	int8_t* i = (int8_t*)&iInt32;
	int8_t* j = (((int8_t*)&iInt32) + 1);
	int8_t* k = (((int8_t*)&iInt32) + 2);
	int8_t* l = (((int8_t*)&iInt32) + 3);

	std::cout << iInt32 << " " << *i << " " << *j << " " << *k << " " << *l << "";

	{
		TempClass* tc = new TempClass();
		std::unique_ptr<TempClass> tcUnique = std::make_unique<TempClass>();
	}

	exit(EXIT_SUCCESS);
}