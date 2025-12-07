#include <iostream>
#include "IO/RandomAccessFile.h"
#include "IO/Buffer/BufferData.hpp"
#include "IO/Buffer/BufferedStreamWriter.h"

#define TOUCH_FILE "TestCases/1.txt"
#define WORKING_FILE "TestCases/BufferTest.txt"

int main(int argc, char** argv)
{
	std::cout << "Hello World!";

	{
		RandomAccessFile::deleteFile(WORKING_FILE);
		RandomAccessFile::touch(TOUCH_FILE);
		RandomAccessFile::rename(TOUCH_FILE, WORKING_FILE);
	}

	// Write byte data
	{
		RandomAccessFile pRafOut;
		bool bReturn = pRafOut.open(WORKING_FILE, std::ios_base::out);
		if (bReturn)
		{
			pRafOut.writeByte(77);
			pRafOut.writeShort(1977);
			pRafOut.writeInt(197777);
			pRafOut.writeLong(197777777777);
			pRafOut.writeLine("Hello World!");

			{
				std::string sStr = "return (IBinder) CLASS.getMethod(\"createDisplay\", String.class, boolean.class).invoke(null, name, secure);";
				BufferData buffer;
				buffer.set((int8_t*)sStr.c_str(), sStr.size());

				pRafOut.write((int8_t*)buffer.get(), buffer.size());
			}

			pRafOut.close();
		}
	}

	// Read byte data
	{
		RandomAccessFile pRafIn;
		bool bReturn = pRafIn.open(WORKING_FILE, std::ios_base::in);
		if (bReturn)
		{
			std::cout << pRafIn.readByte() << "\n";
			std::cout << pRafIn.readShort() << "\n";
			std::cout << pRafIn.readInt() << "\n";
			std::cout << pRafIn.readLong() << "\n";
			std::cout << pRafIn.readLine() << "\n";
			std::cout << pRafIn.readLine() << "\n";
			pRafIn.close();
		}
	}
	return EXIT_SUCCESS;
}