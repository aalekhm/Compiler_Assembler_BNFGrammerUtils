
// https://rosettacode.org/wiki/Compiler/
// http://dotnet.jku.at/applications/visualizer/
// https://stackoverflow.com/questions/1669/learning-to-write-a-compiler
// http://kenyatips.com/weird-programming-languages/
// https://www.memorymanagement.org/mmref/alloc.html
// https://www.tenouk.com/Bufferoverflowc/Bufferoverflow1c.html
// Programming Ion (Bitwise : Bitwise is an educational project where we create the software/hardware stack for a computer from scratch.):
//		https://www.youtube.com/playlist?list=PLU94OURih-CiP4WxKSMt3UcwMSDM3aTtX
//		https://github.com/pervognsen/bitwise/
// Programming an x64 compiler from scratch:
//		https://www.youtube.com/watch?v=N9B2KeAWXgE&t
//		https://www.youtube.com/watch?v=Mx29YQ4zAuM
//	ManyButFinite.com
//		https://manybutfinite.com/archives/
// 			- Anatomy of a Program in Memory
//				https://manybutfinite.com/post/anatomy-of-a-program-in-memory/
// 			- How The Kernel Manages Your Memory
//				https://manybutfinite.com/post/how-the-kernel-manages-your-memory/
// 			- Page Cache, the Affair Between Memory and Files
//				https://manybutfinite.com/post/page-cache-the-affair-between-memory-and-files/
// 			- Journey to the Stack, Part I
//				https://manybutfinite.com/post/journey-to-the-stack/
// 			- Epilogues, Canaries, and Buffer Overflows
//				https://manybutfinite.com/post/epilogues-canaries-buffer-overflows/
// 			- Closures, Objects, and the Fauna of the Heap
//				https://manybutfinite.com/post/closures-objects-heap/
// 			- When Does Your OS Run?
//				https://manybutfinite.com/post/when-does-your-os-run/
// 			- What does an idle CPU do?
//				https://manybutfinite.com/post/what-does-an-idle-cpu-do/
// https://www.cs.bgu.ac.il/~spl121/Inheritance
// https://feepingcreature.github.io/oop.html
// https://www.installsetupconfig.com/win32programming/processtoolhelpapis12.html
// http://www.abhisheksur.com/2011/09/internals-of-net-objects-and-use-of-sos.html
// Music
// 		http://musicforprogramming.net/?one
// 		https://www.youtube.com/watch?v=PNhQakLdI9o
// 		https://www.youtube.com/watch?v=4ZW6Eme9eVY
// 		https://www.youtube.com/watch?v=qRLCwKosN_c&t=548s&list=PLkX4vguQdLMhph17ZMyFo66RqmvQnwlVx&index=3
// https://ruslanspivak.com/lsbasi-part1/

// ToDo:
//		static methods in an 'interface'.
//		'abstract' class...a class that does not implement all the methods of an 'interface'.
//		one cannot create an object of an 'abstract' class.

// RE:
//		http://blog.chmodxx.net/reading-list/

// Scripting:
//		http://www.drdobbs.com/cpp/extracting-function-parameter-and-return/240000586#disqus_thread
//		https://web.archive.org/web/20151230105916/https://code.google.com/p/dconsole/
//		https://alejandrohitti.com/2015/06/21/template-metaprogramming-in-cpp/
//		https://www.randygaul.net/2014/12/18/small-c-reflection-demo/
//		https://www.randygaul.net/2014/01/01/automated-lua-binding/
//		https://www.randygaul.net/2013/12/30/c-function-binding-2/
//		https://www.randygaul.net/category/templates/

// EH:
//		https://github.com/Hack-with-Github/Awesome-Hacking

// ToDo:
//		- [DONE] 'float' as primitive type along with 'int8_t', 'int16_t' & 'int32_t'.

SYSFUNC void 	$_glLoadIdentity();
SYSFUNC void 	$_glClearColor(int32_t iRed, int32_t iGreen, int32_t iBlue, int32_t iAlpha);
SYSFUNC void 	$_glColor3f(float fRed, float fGreen, float fBlue);
SYSFUNC int32_t $_retSysFunc(int32_t iValue);
SYSFUNC float 	$_retFloatFunc(float fValue);
SYSFUNC float 	$_glColor3fMul(float fRed, float fGreen, float fBlue);

float retFunFloat()
{
	return 91.9;
}

void systemCallTest()
{
	print("In systemCallTest");
	putc(10);

	float fSub = -1.9 - 10;
	print("fSub = ", fSub);
	putc(10);
	
	float fFloat = 11.1 * 2.3;
	print("fFloat = ", fFloat);
	putc(10);
	
	int32_t iFloatToInt = retFunFloat();
	print("iFloatToInt = ", iFloatToInt);
	putc(10);
	
	// "$_" before a function denotes a 'System Function' call.
	//		- A 'System Function' is a C++ function that can be called from the script.
	$_glLoadIdentity();
	$_glClearColor(1, 0, 0, 0);
	
	int32_t iRetValue = $_retSysFunc(11);
	print("iRetValue = ", iRetValue);
	putc(10);

	float fSysRetSysFunc = $_retSysFunc(11);
	print("fSysRetSysFunc = ", fSysRetSysFunc);
	putc(10);

	float fSysRetFloatFunc = $_retFloatFunc(29.3);
	print("fSysRetFloatFunc = ", fSysRetFloatFunc);
	putc(10);
	
	int32_t i32SysRetFloatFunc = 12 * $_retFloatFunc(29.3) + $_glColor3fMul(1.9, 3.7, 9.3);
	print("i32SysRetFloatFunc = ", i32SysRetFloatFunc);
	putc(10);

	int8_t i8SysRetFloatFunc = 12 * $_retFloatFunc(29.3) + $_glColor3fMul(1.9, 3.7, 9.3);
	print("i8SysRetFloatFunc = ", i8SysRetFloatFunc);
	putc(10);

	$_glColor3f( (2 * 3) + (-0.25 + 1 - fSysRetFloatFunc), 21.9 * 7.3, fFloat - fSub);
}

interface IntfA
{
	inline void walk();
};

interface IntfB
{
	inline void talk();
};

interface IntfC : IntfA, IntfB
{
	inline void sleep();
};

interface IntfD
{
	inline void fuck();
};

interface IntfE
{
	inline void fuck();
};

struct AAA implements IntfC, IntfD, IntfE
{
	int32_t 	m_iAAAInt1 = 101;
	int8_t 		m_aAAANumberList[8] = { 1, 2 * 1, 3, 4, 5, 6, 7, 8 };
			
	inline virtual void printMembers()
	{
		print("<--------------------------------------------------------->");
		putc(10);
		
		print("m_iAAAInt1 = ", this->m_iAAAInt1);
		putc(10);
		
		this->m_aAAANumberList[0] = 100;
		
		print("m_aAAANumberList = ");
		for(int8_t i = 0; i < 8; i++)
		{
			print(" , ", this->m_aAAANumberList[i]);
		}
		putc(10);

		print("</--------------------------------------------------------->");
		putc(10);
	}
	
	// Note: Default Constructor() & #Destructor() need to be declared at bottom of the 'struct' after all the function decls.
	inline void AAA()
	{
		print("In AAA");
		putc(10);
	}
	
	inline virtual void walk()
	{
		print("Walking...");
		putc(10);
	}
	
	inline virtual void talk()
	{
		print("Talking...");
		putc(10);
	}
		
	inline virtual void sleep()
	{
		print("Sleeping...");
		putc(10);
	}

	inline virtual void fuck()
	{
		print("Fucking...");
		putc(10);
	}
	
	inline static void staticFunc()
	{
		print("In StaticFunc...");
		putc(10);
	}

	inline static int32_t staticFuncReturn()
	{
		print("In staticFuncReturn...");
		putc(10);
		
		return 1977;
	}
};

struct BBB : AAA
{
	int32_t 	m_iBBBInt1;
	int8_t 		m_aBBBNumberList[8] = { 11, 12, 13, 14, 15, 16, 17, 18 };

	"@Override"
	inline virtual void printMembers()
	{
		print("<***********************************************************>");
		putc(10);

		print("m_iBBBInt1 = ", this->m_iBBBInt1);
		putc(10);
		
		print("m_aAAANumberList = ");
		putc(10);
		for(int8_t i = 0; i < 8; i++)
		{
			this->m_aAAANumberList[i] = 100 + i;
			print(this->m_aAAANumberList[i], " ");
		}
		putc(10);
		
		print("m_aBBBNumberList = ");
		for(int8_t i = 0; i < 8; i++)
		{
			print(this->m_aBBBNumberList[i], " ");
		}
		putc(10);

		print("</***********************************************************>");
		putc(10);
		
		// Call to parent::func();
		AAA::printMembers();
	}
};

struct CCC : BBB
{
	int32_t 	m_iCCCInt1;
	int8_t 		m_aCCCNumberList[8] = { 21, 22, 23, 24, 25, 26, 27, 28 };
	
	"@Override"
	inline virtual void printMembers()
	{
		print("<############################################################>");
		putc(10);

		print("m_iCCCInt1 = ", this->m_iCCCInt1);
		putc(10);
		
		this->m_aAAANumberList[0] = 1001;
		print("m_aAAANumberList = ");
		putc(10);
		for(int8_t i = 0; i < 8; i++)
		{
			print(this->m_aAAANumberList[i], " ");
		}
		putc(10);

		print("m_aBBBNumberList = ");
		for(int8_t i = 0; i < 8; i++)
		{
			this->m_aBBBNumberList[i] = 2001 + i;
			print(this->m_aBBBNumberList[i], " ");
		}
		putc(10);

		print("m_aCCCNumberList = ");
		for(int8_t i = 0; i < 8; i++)
		{
			print(this->m_aCCCNumberList[i], " ");
		}
		putc(10);

		print("</############################################################>");
		putc(10);
	
		// Call to Parent::func();
		BBB::printMembers();

		// Call to grandParent::func();
		AAA::printMembers();
	}
	
	inline void #CCC()
	{
		print("<#CCC>");
		putc(10);
	}
};

struct Part
{
	int32_t 	m_iUniquePartID;
	inline virtual void getManufacturer() 
	{ 
		print("In Part::getManufacturer");
		putc(10);
	}	

	inline virtual void getDealer() 
	{ 
		print("In Part::getDealer");
		putc(10);
	}	
};

struct MachinePart : Part
{
	int32_t 	m_iOEMID;
	int32_t 	m_iManufacturerID;
	int32_t 	m_iPartNumber;
	int32_t 	m_iDescription;
	
	inline virtual void getPartNumber() 
	{ 
		print("In MachinePart::getPartNumber");
		putc(10);
	}

	inline virtual void getDescription() 
	{ 
		print("In MachinePart::getDescription = ", m_iDescription);
		putc(10);
	}
};

struct Tyre : MachinePart
{
	int8_t 	m_iSpeed = 0;
	int8_t 	m_iRating = 0;
	
	"@Override"
	inline virtual void getPartNumber() 
	{
		this->getDescription();
		
		this->m_iPartNumber = 999;
		this->m_iUniquePartID = 555;
		print("In Tyre::getPartNumber this->m_iPartNumber = ", this->m_iPartNumber, ", this->m_iUniquePartID = ", this->m_iUniquePartID);
		putc(10);
	}

	inline void getTyreDetails() 
	{
		print("In Tyre::getTyreDetails ==> this->m_iPartNumber = ", this->m_iPartNumber, ", this->m_iUniquePartID = ", this->m_iUniquePartID);
		putc(10);
		
		this->m_iUniquePartID = 143165576;
	}

	"@Override"
	inline virtual void getDescription() 
	{ 
		print("In Tyre::getDescription");
		putc(10);
	}
};

struct Shape
{
	int32_t 		m_iWidth;
	int32_t 		m_iHeight;
	int32_t 		m_iOutline;
	int32_t 		m_iFill;
	
	inline int32_t area()
	{
		int32_t iArea = m_iWidth * m_iHeight;
		return iArea;
	}
	
	inline int32_t perimeter()
	{
		int32_t iPerimeter = (2 * m_iWidth) + (2 * m_iHeight);
		return iPerimeter;
	}
};

struct Example1
{
	// * Member Variables should be declared before the functions.
	int8_t	m_aNumberList[8];
	
	// Note: "inline" keyword is mandatory before any function definition as of now.
	inline void example1Func1()
	{
		print("In example1Func1");
		putc(10);
	}
	
	// For this struct, Default Constructor & Destructor will be created by the compiler.
	// The Constructor will generate code equivalent to:
	//		m_aNumberList = malloc(8*sizeof(int8_t)); // Allocate memory on the heap of the specified size & store the address in 'm_aNumberList';
	// The Destructor will be populated with the code equivalent:
	//		free(m_aNumberList);
};

struct Person
{
	// * Member Variables should be declared before the functions.
	int8_t	m_iTest = 18;
	int8_t 	m_sName[16] = {'A', 'a', 'r', 'i', 'v' };
	int8_t 	m_sAddress[32];
	int8_t 	m_sEmail[32] = {'a', 'a', 'l', 'e', 'k', 'h', 'm', '@', 'g', 'm', 'a', 'i', 'l', '.', 'c', 'o', 'm' };
	int8_t*	m_pDesignation;
	int8_t	m_iAge = 20;
	
	inline void Person(int8_t iAge)
	{
		print("In Person() with 1 argument");
		putc(10);
		
		print("m_iAge = ", m_iAge, ", m_iTest = ", m_iTest);
		putc(10);
        
		m_iAge = iAge;
		print("m_iAge after assignment = ", m_iAge);
		putc(10);
	}
	
	inline void func1()
	{
		print("Entering func1");
		putc(10);
				
		print("Before m_iAge = ", m_iAge);
		putc(10);
		
		m_iAge = 1;

		print("After m_iAge = ", m_iAge);
		putc(10);

		// Currently this call won't work as while parsing top-down to 'func1()', AST for 'getAge()' is not created, yet.
		//int32_t iAge = getAge();
		
		print("Exiting func1");
		putc(10);
	}
	
	inline int32_t getAge()
	{
		print("Entering getAge");
		putc(10);
		
		func1();		// This call would work as func1() AST is already created.
		
		return m_iAge;
	}
	
	inline void pointerArgTest(CCC* pCCC)
	{
		print("<________________________________________________________________________>");
		putc(10);
		
		pCCC->printMembers();
		
		print("</________________________________________________________________________>");
		putc(10);
	}
	
	// Note: The destructor function has a peculiar prefix of '#' unlike the usual '~'.
	//		 This is done just to make parsing easier as '~' is also bitwisenot.
	inline void #Person()
	{
		print("In #Person()");
		putc(10);
	}
};

static int32_t* pStaticPtr_1;
static int32_t* pStaticPtr_2;
static int32_t* pStaticPtr_3;

static int8_t* pStaticPtr_0_Start;

void pointerArgTest(AAA* pAAA, BBB* pBBB, CCC* pCCC)
{
	print("<@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@>");
	putc(10);
	
	pAAA->printMembers();
	pBBB->printMembers();
	pCCC->printMembers();
	
	print("</@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@>");
	putc(10);	
}

void structTest()
{
	AAA* pAAA = new AAA();
	pAAA->printMembers();
	AAA::staticFunc();
	
	int32_t iRet = (2 * AAA::staticFuncReturn()) * AAA::staticFuncReturn();
	print("iRet = ", iRet);
	putc(10);

	BBB* pBBB = new BBB();
	pBBB->printMembers();

	CCC* pCCC = new CCC();
	pCCC->printMembers();
	
	////////////////////////////////////////////////////////////////////////////////
	print("pAAA->m_aAAANumberList = ");
	for(int8_t i = 0; i < 8; i++)
	{
		print(pAAA->m_aAAANumberList[i], " ");
	}
	putc(10);
	pAAA->walk();
	pAAA->talk();
	pAAA->sleep();
	pAAA->fuck();

	print("pBBB->m_aBBBNumberList = ");
	for(int8_t i = 0; i < 8; i++)
	{
		print(pBBB->m_aBBBNumberList[i], " ");
	}
	putc(10);
	
	print("pCCC->m_aCCCNumberList = ");
	for(int8_t i = 0; i < 8; i++)
	{
		print(pCCC->m_aCCCNumberList[i], " ");
	}
	putc(10);
	////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////
	print("pCCC->m_aAAANumberList = ");
	for(int8_t i = 0; i < 8; i++)
	{
		print(pCCC->m_aAAANumberList[i], " ");
	}
	putc(10);

	print("pCCC->m_aBBBNumberList = ");
	for(int8_t i = 0; i < 8; i++)
	{
		print(pCCC->m_aBBBNumberList[i], " ");
	}
	putc(10);
	////////////////////////////////////////////////////////////////////////////////
	
	pointerArgTest(pAAA, pBBB, pCCC);

	free(pAAA);
	free(pBBB);
	free(pCCC);
}

int32_t retNewFunc()
{
	print("In retNewFunc.....");
	putc(10);
	
	Example1 pExampleObj0;					// OR (a) Person* pPersonObj0;
	Person pPersonObj1 = new Person();		// OR (b) Person* pPersonObj1();
	Person pPersonObj2 = new Person(10);	// OR (c) Person* pPersonObj2(10);
	//Person pPersonObjs[4];				// OR (d) Person* pPersonObjs[4];
	
	CCC* pCCC = new CCC();
	pPersonObj2->pointerArgTest(pCCC);

	Tyre pTyre = new Tyre();
	pTyre->m_iSpeed = 11;
	print("Tyre pTyre->m_iSpeed = ", pTyre->m_iSpeed);
	putc(10);
	pTyre->m_iDescription = 9876;
	print("Tyre MachinePart::m_iDescription = ", pTyre->m_iDescription);
	putc(10);
	pTyre->m_iOEMID = 4321;
	print("Tyre Part::m_iOEMID = ", pTyre->m_iOEMID);
	putc(10);
	pTyre->getPartNumber();
	print("Tyre pTyre->m_iPartNumber = ", pTyre->m_iPartNumber, ", pTyre->m_iUniquePartID = ", pTyre->m_iUniquePartID);
	putc(10);
	pTyre->m_iUniquePartID = 5467;
	pTyre->getTyreDetails();
	print("Tyre Part::m_iUniquePartID = ", pTyre->m_iUniquePartID);
	putc(10);
	
	MachinePart pMachinePart;
	pMachinePart->m_iOEMID = 1234;
	print("MachinePart pMachinePart->m_iOEMID = ", pMachinePart->m_iOEMID);
	putc(10);
	pMachinePart->m_iDescription = 5678;
	print("MachinePart pMachinePart->m_iDescription = ", pMachinePart->m_iDescription);
	putc(10);
	pMachinePart->getDescription();
	
	pExampleObj0->example1Func1();
	pPersonObj2->func1();
	
	pExampleObj0->example1Func1();
	
	int32_t iAge = 10 + pPersonObj2->getAge();
	print("1. iAge = ", iAge);
	putc(10);
	
	pExampleObj0->example1Func1();
	
	iAge = 10 + pPersonObj2->m_iTest;
	print("2. iAge = ", iAge);
	putc(10);
	
	pExampleObj0->example1Func1();
	
	pPersonObj2->m_iAge = 90 + pPersonObj2->m_iTest;
	
	pExampleObj0->example1Func1();
	
	iAge = 10 + pPersonObj2->getAge();
	print("3. iAge = ", iAge);
	putc(10);
	
	int32_t iSizeOfExample1 = sizeOf("Example1");
	int32_t iSizeOfPerson = sizeOf("Person");
	int32_t iSizeOfTyre = sizeOf("Tyre");
	int32_t iSizeOfCCC = sizeOf("CCC");
	int32_t iTotal = ( sizeOf("Example1") + sizeOf("Person") + sizeOf("Tyre") + sizeOf("CCC") );
	print("iTotal = ", iTotal);
	putc(10);
	print("sizeOf(Example1) = ", iSizeOfExample1, ", sizeOf(Person) = ", iSizeOfPerson, ", sizeOf(Tyre) = ", iSizeOfTyre, ", sizeOf(CCC) = ", iSizeOfCCC, ", iTotal = ", iTotal);
	putc(10);
	
	// ToDo: functionCall() doesn't work in 'print'
	//print("sizeOf(Example1) = ", sizeOf("Example1"), ", sizeOf(Person) = ", sizeOf("Person"), ", sizeOf(Tyre) = ", sizeOf("Tyre"), ", sizeOf(CCC) = ", sizeOf("CCC"));
		
	int8_t arr1[10] = {111, 222, 333, 444, 555};	//int32_t arr1[10];
													//int32_t arr[] = {10, 20, 30, 40};
													//int32_t arr[6] = {10, 20, 30, 40};
	for(int32_t ii = 0; ii < 10; ii++)
	{
		print("arr1[", ii, "] = ", arr1[ii]);
		putc(10);
	}
	
	memSet(arr1, 0, 10);
	print("After memSet");
	putc(10);
	for(int32_t ii = 0; ii < 10; ii++)
	{
		print("arr1[", ii, "] = ", arr1[ii]);
		putc(10);
	}

	int8_t arr2[10] = {11, 22, 33, 44, 55};			//int32_t arr1[10];
													//int32_t arr[] = {10, 20, 30, 40};
													//int32_t arr[6] = {10, 20, 30, 40};
													
	for(int32_t ii = 0; ii < 10; ii++)
	{
		print("arr2[", ii, "] = ", arr2[ii]);
		putc(10);
	}

	memCpy(arr1, arr2, 10);
	print("After memCpy");
	putc(10);
	for(int32_t ii = 0; ii < 10; ii++)
	{
		print("arr1[", ii, "] = ", arr1[ii]);
		putc(10);
	}
	
	int8_t pCmpStatus = memCmp(arr2, arr1, 10);
	print("(arr2 == arr1) == ", pCmpStatus);
	putc(10);
	
	int8_t* pArr4 = memChr(arr2, 33, 10);
	print("33 found @ pos ", (pArr4 - arr2));
	putc(10);
	
	int8_t ch = 'A';
	ch = 'B';
	print("int8_t ch = ", ch);
	putc(10);
	
	int16_t iShort = 131071;
	print("int16_t iShort = ", iShort);
	putc(10);

	int32_t j = 1;
	int32_t* pNew = malloc(j * 9);
	print("pNew Allocated @ ", pNew);
	putc(10);
	
	pNew[0] = 1977;
	print("Value pNew[0] = ", pNew[0]);
	putc(10);
	
	////////////////////////////////////////////////
	int8_t* pPtr_0 = malloc(j * 8);
	print("pPtr_0 Allocated @ ", pPtr_0, ", pPtr_0[0] = ", pPtr_0[0]);
	putc(10);
	
	pStaticPtr_0_Start = pPtr_0;
	
	pPtr_0[0] = 1982;
	pPtr_0++;
	pPtr_0[0] = 1983;
	pPtr_0--;
	print("Value of pPtr_0[0] = ", pPtr_0[0], ", pPtr_0[1] = ", pPtr_0[1]);
	putc(10);
	pPtr_0++;
	print("Value of pPtr_0[1] = ", pPtr_0[0]);
	putc(10);
	
	pPtr_0 = pStaticPtr_0_Start;
	pPtr_0[2] = 1984;
	pPtr_0[3] = 1985;
	pPtr_0[4] = 1986;
	print("Value of pPtr_0[0] = ", pPtr_0[0], ", pPtr_0[1] = ", pPtr_0[1], ", pPtr_0[2] = ", pPtr_0[2], ", pPtr_0[3] = ", pPtr_0[3], ", pPtr_0[4] = ", pPtr_0[4]);
	putc(10);
	
	print("Value of pNew = ", pNew[0]);
	putc(10);

	pPtr_0[0] = pNew[0];
	print("Assigned pPtr_0 = pNew ==> ", pPtr_0[0]);
	putc(10);
	
	for(int32_t ii = 0; ii < 8; ii++)
	{
		// LValue Nested ArrayIndexing
		pPtr_0[ii + 2 - 2 + pNew[0] - 1977] = 255 - ii;
	}
	
	for(int32_t ii = 0; ii < 8; ii++)
	{
		print("pPtr_0[", ii, "] = ", pPtr_0[ii + 2 - 2]);
		putc(10);
	}
	
	// RValue Nested ArrayIndexing
	int32_t iVar = pNew[0] + pPtr_0[2 * 3 - 4 + pPtr_0[1] - 256];	// 1977 + 255
	print("iVar = ", iVar, "... pNew[0] = ", pNew[0], ", pPtr_0[1] = ", pPtr_0[1]);
	putc(10);
	////////////////////////////////////////////////
	
	///////////////////////////////////////////////
	pNew[j*0] = 2016 * 2;
	print("pNew[0] = ", pNew[0]);
	putc(10);
	///////////////////////////////////////////////
	
	int32_t iCount = pStaticPtr_3[0];
	iCount++;
	pStaticPtr_3[0] = iCount;
	print("pStaticPtr_3 Count ==> ", pStaticPtr_3[0]);
	putc(10);
	
	print("In retNewFunc() start. (65535 & 255) = ", (65535 & 255), ", (170 | 85) = ", (170 | 85), ", (170 ^ 85) = ", (170 ^ 85), ", (~170) = ", (~170));
	putc(10);
	
	print("Bitwise: (15 << 4) = ", (15 << 4), ", (240 >> 4) = ", (240 >> 4));
	putc(10);	

	int32_t mmi = 11;
	switch(mmi)
	{
		case 10:
			print("In case 10");
			putc(10);			
		break;
		case 11:
			print("In case 11");
			putc(10);			
		case 12:
			print("In case 12");
			putc(10);					
		break;
		default:
			print("In default case");
			putc(10);						
		break;
	}
	
	if(mmi == 11)
	{
		Person pPersonObj_InsideIf;
		
		// Arrays will be deleted automatically at the END_OF_BLOCK
		int8_t arr2[8] = {111, 222, 333, 444, 555};		//int32_t arr2[8];
														//int32_t arr2[] = {10, 20, 30, 40};
														//int32_t arr2[6] = {10, 20, 30, 40};
		
		print("arr2 inside if == ", arr2[0]);
		
		// Memory allocated by 'arr2' & 'pPersonObj_InsideIf' will be freed when the scope of 'if' ends.
	}
	
	int32_t j = 0;
	++j;
	
	int32_t m = 0;
	m = ++j + ++j + ++j;
	
	print("m = ", m);
	putc(10);
	
	for(int32_t i = 0, j = 1, int32_t k = 1; i < 10; ++i, ++k)
	{
		print("ForLoop i = ", i, " ,val of j = ", j, " ,k = ", k);
		putc(10);
	}
	
	free(pNew);
	free(pPtr_0);
	
	// Objects created by the notation in (a), (b), (c) & (d) are created on the HEAP & need to be deleted MANUALLY.
	// Although, objects created by (a) & (d) will to be deleted AUTOMATICALLY just where the scope ends.
	free(pTyre);
	free(pPersonObj1);
	free(pPersonObj2);
	free(pCCC);

	int32_t iReturnValue = 1977;
	
	return iReturnValue;
}

int32_t retFunc()
{
	pStaticPtr_1 = malloc(14);
	
	print("In retFunc.....");
	putc(10);
	
	free(pStaticPtr_1);
	
	return retNewFunc();
}

void tempFunc(int32_t arg0, int32_t arg1)
{
	pStaticPtr_2 = malloc(19);
	
	print("In tempFunc()");
	putc(10);
		
	int32_t iRet = 10;
	iRet = (10 + retFunc()) + 10;
	print("iRet = ", iRet);
	putc(10);
	
	if(((retFunc() + 10) - 1900) == 87)
	{
		print("retFunc returned 1977")
		putc(10);
	}
	
	int32_t iRet1 = 101 + retFunc();
	print("iRet1 = ", iRet1);
	putc(10);
	
	int32_t j = 111;
	j = j + arg0 + retFunc();
	print("tempFunc() args = ", j, ", ", arg1);
	putc(10);
	
	int32_t iCount = 1970;
	while(iCount <= retFunc())
	{
		iCount = iCount + 1;
		print("In While = ", iCount);
		putc(10);
	}
	
	if(j < 10)
	{
		j = 10;
		int32_t k = 2;
		k = 3;
	}
	else
	{
		int32_t k = 11;
		while(j == 11)
		{
			int32_t j = 1111;
			j = j + 1;
			
			arg0 = arg0 - 1;
		}
		
		k = j * 2;
	}
	
	free(pStaticPtr_2);
	
	print("End of tempFunc()");
	putc(10);
}

void testPrinting()
{
	/*** test printing, embedded \n and comments with lots of '*' ***/
	print(42);
	print("\nHello World\nGood Bye\nok\n");
	print("Print a slash n - \\n.\n");
}

void simpleCount()
{
	int32_t count = 1;
	while (count < 10) {
		print("count is: ", count, "\n");
		putc(10);
		count = count + 1;
	}
}

void _100Doors()
{
	/* 100 Doors */
	int32_t i = 1;
	while (i * i <= 100) {
		print("door ", i * i, " is open\n");
		putc(10);
		i = i + 1;
	}
}

void negativeTest()
{
	print("negativeTest : ");
	putc(10);
	
	int32_t a = (-1 * ((1 * (5 * 15)) / 10));
	print(a);
	putc(10);
	
	int32_t b = -a + (3 * 4);
	print("b = ", b);
	putc(10);
	
	print("-b = ", -b);
	putc(10);
	
	print("-(1) = ", -(1));
	putc(10);
	
	int32_t i = a;
	print("i = a = ", a);
	putc(10);

	i = ++a + ++a + ++a;
	print("i = ", i, " , a = ", a);
	putc(10);
	
	i = --a + --a + --a;
	print("i = ", i, " , a = ", a);
	putc(10);

	i = a++ + a++ + a++;
	print("i = ", i, " , a = ", a);
	putc(10);
    
	i = a-- + a-- + a--;
	print("i = ", i, " , a = ", a);
	putc(10);
}

void deep()
{
	print("In deep");
	putc(10);
	
//	print(---------------------------------+++5, "\n");
	print("Deep enough = ", ((((((((3 + 2) * ((((((2))))))))))))));
	putc(10);
 
	if (1) 
	{ 
		if (1) 
		{ 
			if (1) 
			{ 
				if (1) 
				{ 
					if (1) 
					{ 
						print(15); 
						putc(10);
					} 
				} 
			} 
		} 
	}
	
	print("End deep");
	putc(10);
}

void gcd()
{
	/* Compute the gcd of 1071, 1029:  21 */
 
	int32_t a = 1071;
	int32_t b = 1029;
	print("gcd of ", a, " & ", b, " = ");
	
	while (b != 0) {
		int32_t new_a = b;
		b     = a % b;
		a     = new_a;
	}
	
	print(a);
	putc(10);
}

void factorial()
{
	/* 12 factorial is 479001600 */
 
	int32_t n = 12;
	int32_t result = 1;
	int32_t i = 1;
	while (i <= n) {
		result = result * i;
		i = i + 1;
	}
	
	print("Factorial of ", n, " = ", result);
	putc(10);
}

void fibonacci()
{
	/* fibonacci of 44 is 701408733 */
 
	int32_t n = 44;
	int32_t i = 1;
	int32_t a = 0;
	int32_t b = 1;
	int32_t w = 0;
	while (i < n) {
		int32_t w = a + b;
		a = b;
		b = w;
		i = i + 1;
	}
	print(w, "\n");
	putc(10);
}

void fizzBuzz()
{
	/* FizzBuzz */
	int32_t i = 1;
	while (i <= 100) {
		if (!(i % 15))
		{
			print("FizzBuzz");
			putc(10);
		}
		else 
		{
			if (!(i % 3))
			{
				print("Fizz");
				putc(10);
			}
			else 
			{
				if (!(i % 5)) 
				{
					print("Buzz");
					putc(10);
				}
				else 
				{
					print(i);
					putc(10);
				}
			}
		}
	
		print("\n");
		putc(10);
		i = i + 1;
	}
}

void _99BottlesOfBeer()
{
	/* 99 bottles */
	int32_t bottles = 99;
	while (bottles > 0) {
		print(bottles, " bottles of beer on the wall\n");
		putc(10);
		print(bottles, " bottles of beer\n");
		putc(10);
		print("Take one down, pass it around\n");
		putc(10);
		bottles = bottles - 1;
		print(bottles, " bottles of beer on the wall\n\n");
		putc(10);
	}
}

void primes()
{
	/*
	Simple prime number generator
	*/
	int32_t count = 1;
	int32_t n = 1;
	int32_t limit = 100;
	while (n < limit) {
		int32_t k=3;
		int32_t p=1;
		n=n+2;
		while ((k*k<=n) && (p)) {
			p=n/k*k!=n;
			k=k+2;
		}
		if (p) {
			print(n, " is prime\n");
			putc(10);
			count = count + 1;
		}
	}
	print("Total primes found: ", count, "\n");
	putc(10);
}

void AsciiMandlebrot()
{
/*
 This is an integer ascii Mandelbrot generator
 */
    int32_t left_edge   = -420;
    int32_t right_edge  =  300;
    int32_t top_edge    =  300;
    int32_t bottom_edge = -300;
    int32_t x_step      =    7;
    int32_t y_step      =   15;
 
    int32_t max_iter    =  200;
 
    int32_t y0 = top_edge;
    while (y0 > bottom_edge) {
        int32_t x0 = left_edge;
        while (x0 < right_edge) {
            int32_t y = 0;
            int32_t x = 0;
            int32_t the_char = ' ';
            int32_t i = 0;
            while (i < max_iter) {
                int32_t x_x = (x * x) / 200;
                int32_t y_y = (y * y) / 200;
                if (x_x + y_y > 800 ) {
                    the_char = '0' + i;
                    if (i > 9) {
                        the_char = '@';
                    }
                    i = max_iter;
                }
                y = x * y / 100 + y0;
                x = x_x - y_y + x0;
                i = i + 1;
            }
            putc(the_char);
            x0 = x0 + x_step;
        }
        putc(10);
        y0 = y0 - y_step;
    }
}

void callMe()
{
	int32_t ii = 0;
	while(ii < 3)
	{
		print("printing AsciiMandlebrot Before", ii);
		putc(10);
		
		// TODO:: Problem with the stack when we call a function inside a nested block.
		AsciiMandlebrot();
		
		ii = ii + 1;
		print("printing AsciiMandlebrot After", ii);
		putc(10);
		
		tempFunc(555, 1977);
		putc(10);
	}
}

void main()
{
	pStaticPtr_3 = malloc(4);
	pStaticPtr_3[0] = 0;
	
	print("In main()");
	putc(10);
		
	float a = (-3.7 * ((7.9 * (5 * 15.9)) / 5.7));
	print("a = ", a); // -407.682
	putc(10);
	
	systemCallTest();
	structTest();
	
	tempFunc(55, 101);
	testPrinting();
	simpleCount();
	_100Doors();
	negativeTest();
	deep();
	gcd();
	factorial();
	fibonacci();
	fizzBuzz();
	_99BottlesOfBeer();
	primes();
	AsciiMandlebrot();
	
	callMe();
	
	free(pStaticPtr_3);
	
	print("End of main().....");
	putc(10);
}