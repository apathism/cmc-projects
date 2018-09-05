#include <cstdlib>
#include <climits>
#include "string.h"
#include "array.h"

using namespace asl;

String::String(const bool b): Array<char>(b)
{
}

String::String(int n): Array<char>(n)
{
}

String::String(): Array<char>()
{
}

String::String(const char* s): Array<char>((int)strlen(s))
{
	for (int j=0; j<len; ++j)
		data[j] = s[j];
}

String::String(const String &s): Array<char>(s)
{
}

String::String(const Array<char> &s): Array<char>(s)
{
}

String::~String()
{
}

String String::fromInt(int v)
{
	String t;
	int tv = v;
	if (v < 0) v = -v;
	while (v)
	{
		t.Push('0' + (v % 10));
		v /= 10;
	}
	if (tv < 0) t.Push('-');
	if (!tv) t.Push('0');
	for (int j=0; j<t.Length()/2; j++)
	{
		char z = t[j];
		t[j] = t[t.Length()-j-1];
		t[t.Length()-j-1] = z;
	}
	return t;
}

int String::strlen(const char *s)
{
	int r = 0;
	while (*(s++) != '\0') ++r;
	return r;
}

const String& String::operator=(const char *s)
{
	Resize(strlen(s));
	for (int j=0; j<len; ++j)
		data[j] = s[j];
	return *this;
}

const String& String::operator=(const String &s)
{
	Resize(s.len);
	for (int j=0; j<len; ++j)
		data[j] = s.data[j];
	return *this;
}

String String::operator+(const char *s) const
{
	int l = strlen(s);
	String t(len + l);
       	for (int j=0; j<len; ++j)
               	t.data[j] = data[j];
       	for (int j=0; j<l; ++j)
               	t.data[len+j] = s[j];
	return t;
}

String String::operator+(const String &s) const
{
	String t(len + s.len);
	for (int j=0; j<len; ++j)
		t.data[j] = data[j];
	for (int j=0; j<s.len; ++j)
		t.data[len+j] = s.data[j];
	return t;
}

namespace asl
{
	String operator+(const char* s, const String &d)
	{
		int l = d.strlen(s);
		String t(d.len + l);
		for (int j=0; j<l; ++j)
			t.data[j] = s[j];
		for (int j=0; j<d.len; ++j)
			t.data[l+j] = d.data[j];
		return t;
	}
};

bool String::operator!= (const char *s) const
{
	if ((int)strlen(s) != len) return true;
	for (int j=0; j<len; ++j)
		if (s[j] != data[j]) return true;
	return false;
}

bool String::operator!= (const String &s) const
{
	return (*this).Array<char>::operator!=(s);
}

bool String::operator== (const char *s) const
{
	return !operator!=(s);
}

bool String::operator== (const String &s) const
{
	return (*this).Array<char>::operator==(s);
}

char* String::cstring() const
{
	char* ns = new char[len+1];
	for (int j=0; j<len; ++j)
		ns[j] = data[j];
	ns[len] = '\0';
	return ns;
}

bool String::startsWith(const char *s) const
{
	int t = strlen(s);
	if (t > len) return false;
	for (int j=0; j<t; ++j)
		if (data[j] != s[j]) return false;
	return true;
}

bool String::startsWith(const String &s) const
{
	if (s.len > len) return false;
	for (int j=0; j<s.len; ++j)
		if (data[j] != s.data[j]) return false;
	return true;
}

String String::subString(int l, int r) const
{
	if (l > r || l < 0 || r > len) throw errBadInterval;
	String s(r-l);
	for (int j=l; j<r; j++)
		s[j-l] = data[j];
	return s;
}

Array<String> String::Split(char c) const
{
	int j = 0, sc = 0;
	char ll = c;
	for (; j<len; ++j)
	{
		if (ll == c && data[j] != c) ++sc;
		ll = data[j];
	}

	Array<String> r(sc);
	j = 0;
	for (int k=0; k<sc; k++)
	{
		while (data[j] == c) ++j;
		int z = j;
		while (z < len && data[z] != c) ++z;
		r[k] = subString(j, z);
		j = z;
	}
	return r;
}

bool String::toInt(int &i) const
{
	long long v = 0;
	if (!len) return false;
	for (int j = (data[0] == '-')? 1 : 0; j<len; ++j)
	{
		if (data[j]<'0' || data[j]>'9') return false;
		v = v*10 + (data[j] - '0');
		if (-v<INT_MIN) return false;
	}
	if (data[0] == '-') v=-v;
	if (v<INT_MIN || v>INT_MAX) return false;
	i = v;
	return true;
}

int String::toIntForce() const
{
	int temp;
	if (toInt(temp))
		return temp;
	else
		return 0;
}
