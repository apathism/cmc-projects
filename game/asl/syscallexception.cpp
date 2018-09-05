#include <cstdio>
#include <cstring>
#include <cerrno>
#include "syscallexception.h"

using namespace asl;

SyscallException::SyscallException()
{
	err = errno;
	syscall = 0;
}

SyscallException::SyscallException(const String &callstr)
{
	err = errno;
	this->syscall = callstr;
}

String SyscallException::getSyscall() const
{
	return syscall;
}

String SyscallException::getDescription() const
{
	return String(strerror(errno));
}

String SyscallException::getMessage() const
{
	return getSyscall() + ": " + getDescription();
}
