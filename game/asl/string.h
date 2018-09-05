#ifndef __ASL_STRING
#define __ASL_STRING

#include "array.h"

namespace asl
{
	class String: public Array<char>
	{
		// empty constructor
		String(const bool);
		String(int);

		static int strlen(const char*);
	public:
		String();
		String(const char*);
		String(const Array<char>&);
		String(const String&);
		~String();

		static String fromInt(int);

		const String& operator=(const char*);
		const String& operator=(const String&);

		String operator+(const char*) const;
		String operator+(const String&) const;
		friend String operator+(const char*, const String&);

		bool operator==(const char*) const;
		bool operator==(const String&) const;
		bool operator!=(const char*) const;
		bool operator!=(const String&) const;

		char* cstring() const;

		bool startsWith(const char* s) const;
		bool startsWith(const String &s) const;

		String subString(int, int) const;
		Array<String> Split(char) const;

		bool toInt(int&) const;
		int toIntForce() const;
	};
};

#endif
