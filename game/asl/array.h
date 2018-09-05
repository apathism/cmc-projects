#ifndef __ASL_ARRAY
#define __ASL_ARRAY

#include <cstdlib>
#include <new>

static const char *errBadAlloc = "Error: required memory can't be allocated";
static const char *errBadAddr = "Error: program attempts to access an element not included in the array";
static const char *errBadInterval = "Error: wrong interval given in arguments";

namespace asl
{
	template <class T>
	class Array
	{
	protected:
		T* data;
		int len, plen;
		static const int defplen = 16;

		// Empty constuctor
		Array(const bool);
	public:
		Array();
		Array(int);
		Array(int, const T&);
		Array(const Array&);
		virtual ~Array();

		T& operator[] (int);
		const T& operator[] (int) const;

		const Array<T>& operator= (const Array<T>&);
		Array<T> operator+ (const Array<T>&) const;

		bool operator== (const Array<T>&) const;
		bool operator!= (const Array<T>&) const;

		int Length() const;

		void Resize(int);
		void Resize(int, const T&);
		void Push(const T&);
		void Pop();

		Array<T> subArray(int, int) const;
		bool startsWith(const Array<T>&) const;
	};

	template <class T>
	Array<T>::Array(const bool x)
	{
	}

	template <class T>
	Array<T>::Array()
	{
		len = 0;
		plen = defplen;
		data = (T*)malloc(plen*sizeof(T));
		if (!data) throw errBadAlloc;
	}

	template <class T>
	Array<T>::Array (int m)
	{
		len = m;
		plen = m*2;
		data = (T*)malloc(plen*sizeof(T));
		if (!data) throw errBadAlloc;
		for (int j=0; j<m; ++j)
			new (data+j) T();
	}

	template <class T>
	Array<T>::Array (int m, const T &x)
	{
		len = m;
		plen = m*2;
		data = (T*)malloc(plen*sizeof(T));
		if (!data) throw errBadAlloc;
		for (int j=0; j<m; ++j)
			new (data+j) T(x);
	}

	template <class T>
	Array<T>::Array (const Array &x)
	{
		len = x.len;
		plen = x.plen;
		data = (T*)malloc(plen*sizeof(T));
		if (!data) throw errBadAlloc;
		for (int j=0; j<len; ++j)
			new (data+j) T(x.data[j]);
	}

	template <class T>
	Array<T>::~Array()
	{
		for (int j=0; j<len; ++j)
			data[j].~T();
		free(data);
	}

	template <class T>
	T& Array<T>::operator[] (int j)
	{
		if (len <= j) throw errBadAddr;
		return data[j];
	}

	template <class T>
	const T& Array<T>::operator[] (int j) const
	{
		if (len <= j) throw errBadAddr;
		return data[j];
	}

	template <class T>
	const Array<T>& Array<T>::operator=(const Array<T> &x)
	{
		for (int j=0; j<len; ++j)
			data[j].~T();
		len = x.len;
		if (plen < len)
		{
			plen = len*2;
			T* tdata = (T*)realloc(data, plen*sizeof(T));
			if (!tdata) throw errBadAlloc;
			data = tdata;
		}
		for (int j=0; j<len; ++j)
			new (data+j) T(x.data[j]);
		return *this;
	}

	template <class T>
	Array<T> Array<T>::operator+(const Array<T> &x) const
	{
		Array<T> s(true);
                s.len = len + x.len;
                s.plen = s.len * 2;
               	s.data = (T*)malloc(s.plen*sizeof(T));
                if (!data) throw errBadAlloc;
		for (int j=0; j<len; ++j)
			new (s.data+j) T(data[j]);
		for (int j=0; j<x.len; ++j)
			new (s.data+j+len) T(x.data[j]);
		return s;
	}

	template <class T>
	bool Array<T>::operator!= (const Array<T> &x) const
	{
		if (len != x.len) return true;
		for (int j=0; j<len; ++j)
			if (data[j] != x.data[j]) return true;
		return false;
	}

	template <class T>
	bool Array<T>::operator== (const Array<T> &x) const
	{
		return !operator!=(x);
	}

	template <class T>
	int Array<T>::Length() const
	{
		return len;
	}

	template <class T>
	void Array<T>::Resize(int m)
	{
		if (m > plen)
		{
			plen = m*2;
			T* tdata = (T*)realloc(data, plen*sizeof(T));
			if (!tdata) throw errBadAlloc;
			data = tdata;
		}
		for (int j=m; j<len; ++j)
			data[j].~T();
		for (int j=len; j<m; ++j)
			new (data+j) T();
		len = m;
	}

	template <class T>
	void Array<T>::Resize(int m, const T& x)
	{
		if (m > plen)
		{
			plen = m*2;
			T* tdata = (T*)realloc(data, plen*sizeof(T));
			if (!tdata) throw errBadAlloc;
			data = tdata;
		}
		for (int j=m; j<len; ++j)
			data[j].~T();
		for (int j=len; j<m; ++j)
			new (data+j) T(x);
		len = m;
	}

	template <class T>
	void Array<T>::Push(const T &x)
	{
		Resize(len+1, x);
	}

	template <class T>
	void Array<T>::Pop()
	{
		data[--len].~T();
	}

	template <class T>
	Array<T> Array<T>::subArray(int l, int r) const
	{
		if (l > r || l < 0 || r > len) throw errBadInterval;
		Array<T> s(true);
		s.len = r-l;
                s.plen = s.len * 2;
                data = (T*)malloc(s.plen*sizeof(T));
                if (!data) throw errBadAlloc;
		for (int j=l; j<r; ++j)
			new (s.data+(j-l)) T(data[j]);
		return s;
	}

	template <class T>
	bool Array<T>::startsWith(const Array<T> &x) const
	{
		if (len < x.len) return false;
		for (int j=0; j<x.len; ++j)
			if (data[j] != x.data[j]) return false;
		return true;
	}
};

#endif

