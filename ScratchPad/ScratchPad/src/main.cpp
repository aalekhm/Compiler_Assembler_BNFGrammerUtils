#include "main.h"
#include <stdio.h>
#include "MyTuple.h"

int main_0(int argc, char* argv[])
{
	// Expr:	(7 - (2 * 3 + 5) * (8 - 4 / 2))
	// PostFix: 7 2 3 * 5 + 8 4 2 / - * -
	//								-
	//						7				*
	//								+				-
	//							*		5		8		/
	//						2		3				4		2
	std::vector<Tree*>	vExpression			= { new Tree("7"),
												new Tree("2"),
												new Tree("3"),
												new Tree("*"),
												new Tree("5"),
												new Tree("+"),
												new Tree("8"),
												new Tree("4"),
												new Tree("2"),
												new Tree("/"),
												new Tree("-"),
												new Tree("*"),
												new Tree("-")
											};
	convertPostFixExpressionToTree(vExpression);
	Tree* pRoot = vExpression[0];
	Traverse(pRoot);
	labelize_PostOrderTraversal(pRoot);
	std::cout << "Registers needed : " << pRoot->m_iRegCount << std::endl;
	vExpression.clear();

	// PostFix:		A 2 ^ 2 A * B * - B 2 ^ + A B - /
	// Pictorial:	https://i.stack.imgur.com/kmMp9.png
	vExpression								= { new Tree("A"),
												new Tree("2"),
												new Tree("^"),
												new Tree("2"),
												new Tree("A"),
												new Tree("*"),
												new Tree("B"),
												new Tree("*"),
												new Tree("-"),
												new Tree("B"),
												new Tree("2"),
												new Tree("^"),
												new Tree("+"),
												new Tree("A"),
												new Tree("B"),
												new Tree("-"),
												new Tree("/")
											};
	convertPostFixExpressionToTree(vExpression);
	pRoot = vExpression[0];
	Traverse(pRoot);
	labelize_PostOrderTraversal(pRoot);
	std::cout << "Registers needed : " << pRoot->m_iRegCount << std::endl;

	exit(EXIT_SUCCESS);
}

template <typename T>
struct is_pointer_type
{
	static const bool value = false;
};

template <typename T>
struct is_pointer_type<T*>
{
	static const bool value = true;
};

template <typename T>
bool is_pointer(const T&)
{
	return is_pointer_type<T>::value;
}

template <typename T>
size_t SizeOf()
{
	return 1;
}

#define STRUCT(__structName__, __contents__, __size__) \
struct __structName__ \
__contents__ \
template <> \
size_t SizeOf<__structName__>() \
{ \
return __size__; \
} \

// Suppress Compiler Padding in MSVC
//#pragma pack(push, 1)
STRUCT(Test,
{
	char	c;
	short	s;
	int*	p;
}; , 7)

STRUCT(Foo,
{
	char		c;
	short		s;
	unsigned	i;
	long		l;
	long long	ll;
	void*		vp;
	float		f;
	double		d;
	long long*	llp;
}; , 39);

//size_t SizeOf(Test) { return 7; }
//#pragma pack(pop)

//#define sizeof(__type__) ""

size_t SizeOf(void) { return 0; }
size_t SizeOf(int) { return sizeof(int); }
size_t SizeOf(float) { return sizeof(float); }
size_t SizeOf(double) { return sizeof(double); }
size_t SizeOf(long) { return sizeof(long); }
size_t SizeOf(long long) { return sizeof(long long); }
size_t SizeOf(void*) { return sizeof(void*); }

int main_1(int argc, char* argv[])
{
	int i = 0.0f;		// 4
	float f = 0.0f;		// 4
	double d = 0.0;		// 8
	long l = 0l;		// 4
	long long ll = 0;	// 8
	void* vp = NULL;	// 4/8

	std::cout << "SizeOf(Test) = " << SizeOf<Test>() << " | " << "sizeof(Test) = " << sizeof(Test) << std::endl;
	std::cout << "SizeOf(Foo) = " << SizeOf<Foo>() << " | " << "sizeof(Foo) = " << sizeof(Foo) << std::endl;

	exit(EXIT_SUCCESS);
}

int main_2(int argc, char* argv[])
{
	//std::cout << "declval<Test> = " << typeid(Test).name() << " " << decltype(test) << std::endl;

	MyTuple<int, float, std::string> data1(1, 2.0, "Hello World !");
	int data1_1 = data1.get<0>();
	float data1_2 = data1.get<1>();
	std::string data1_3 = data1.get<2>();

	int** ip = NULL;
	bool b = is_pointer(ip);

	bool bb = is_pointer_type<int*>::value;

	exit(EXIT_SUCCESS);
}

class SimpleAllocator
{
	public:
		SimpleAllocator() {}
		~SimpleAllocator() {}

		void* operator new(size_t iSize)
		{
			if (m_pAllocator == nullptr)
			{
				m_pAllocator = (SimpleAllocator*)malloc(sizeof(SimpleAllocator)); 
			}

			return m_pAllocator;
		}

		void* Alloc(size_t iSize)
		{
			void* pAddr = malloc(iSize);
			memset(pAddr, 0, iSize);

			return pAddr;
		}

		template <typename T>
		void Dealloc(T* pAddr)
		{
			if (pAddr != nullptr)
			{
				pAddr->~T();

				free(pAddr);
				pAddr = nullptr;
			}
		}

	private:
		static SimpleAllocator*	m_pAllocator;
};
SimpleAllocator* SimpleAllocator::m_pAllocator = nullptr;

#define EA_CA_NEW(Class, pAllocator) \
	new (pAllocator->Alloc(sizeof(Class))) Class \

#define EA_CA_DELETE(pAddress, pAllocator) \
	(pAllocator->Dealloc(pAddress)) \

class NewDeleteOperatorOverload
{
	public:
		NewDeleteOperatorOverload() {};
		~NewDeleteOperatorOverload() {};

		void* operator new(size_t iSize)
		{
			void* pAddr = malloc(sizeof(NewDeleteOperatorOverload) * iSize);
			memset(pAddr, 0, sizeof(NewDeleteOperatorOverload) * iSize);

			return pAddr;
		}

		void operator delete(void* pAddr)
		{
			if (pAddr != nullptr)
			{
				free((void*)pAddr);
				pAddr = nullptr;
			}
		}

		int32_t m_iDummyInt;
};

class SimpleClass
{
	public:
		SimpleClass(int32_t iDummyInt) {};
		~SimpleClass() {};
	private:
		int32_t m_iDummyInt;
};

int main(int argc, char* argv[])
{
	NewDeleteOperatorOverload* pNewDeleteOperatorOverload = new NewDeleteOperatorOverload();
	delete(pNewDeleteOperatorOverload);

	SimpleAllocator* pSimpleAllocator = new SimpleAllocator();

	SimpleClass* pSimpleClass_1 = new (pSimpleAllocator->Alloc(sizeof(SimpleClass))) SimpleClass (10);
	SimpleClass* pSimpleClass = EA_CA_NEW(SimpleClass, pSimpleAllocator)(10);

	EA_CA_DELETE(pSimpleClass, pSimpleAllocator);

	exit(EXIT_SUCCESS);
}

