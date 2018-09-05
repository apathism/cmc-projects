#include "messageexception.h"

using namespace asl;

MessageException::MessageException(const String &msg)
{
	this->msg = msg;
}

String MessageException::getMessage() const
{
	return msg;
}
