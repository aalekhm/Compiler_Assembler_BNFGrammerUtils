#pragma once

#include <string>
#include <map>
#include <memory>
#include <regex>
#include "RandomAccessFile.h"

#define NOT !
#define WRITE_STRING(__filePointer__, __string__) __filePointer__->write(__string__);
#define WRITE_LINE(__filePointer__, __singleLine__) if(NOT __singleLine__) __filePointer__->writeLine(""); else __filePointer__->writeByte(' ');
#define WRITE_TABS(__filePointer__, __COUNT__, __singleLine__) \
{ \
	if(NOT __singleLine__) \
	{ \
		std::string sTabs = ""; \
		for(int i = 0; i < __COUNT__; i++) \
		{ \
			sTabs += "\t"; \
		} \
\
		WRITE_STRING(__filePointer__, sTabs.c_str()); \
	} \
} \

static int TAB_COUNT = -1;
static bool IS_ROOT_OBJ = true;
static std::string BuildPhaseTypes[] = {	"PBXFrameworksBuildPhase",
											"PBXResourcesBuildPhase",
											"PBXSourcesBuildPhase",
											"PBXShellScriptBuildPhase",
											"PBXCopyFilesBuildPhase"
									};
									
enum class EPBXVALUE_TYPE
{
	INVALID = -1,
	NUMBER = 0,
	STRING,
	IDENTIFIER
};

enum class EPBXTYPE_PARSE_STATE
{
	START = 0,
	ISA_TYPE,
	END_OF_TYPE,
};
