#pragma once

#include <cassert>
#pragma warning(disable:4100)
/**
	\brief 简单的锁，封装了CriticalSection
*/
class CSimpleLock
{
public:
	CSimpleLock()
	{
		::InitializeCriticalSection(&m_Lock);
	}

	~CSimpleLock()
	{
		::DeleteCriticalSection(&m_Lock);
	}
	void Lock()
	{
		::EnterCriticalSection(&m_Lock);
	}

	void UnLock()
	{
		::LeaveCriticalSection(&m_Lock);
	}

	BOOL TryLock()
	{
		return ::TryEnterCriticalSection(&m_Lock);
	}

private:
	CRITICAL_SECTION m_Lock;
};



class CSimpleAutoLock
{
	//这个类需要改进
	//1:兼容临界区
	//2:私有锁成员改为引用
public:
	CSimpleAutoLock(CSimpleLock * pSimpleLock)
	{
		m_pSimpleLock = pSimpleLock;
		pSimpleLock->Lock();
	}

	~CSimpleAutoLock()
	{
		m_pSimpleLock->UnLock();
	}

private:
	CSimpleLock * m_pSimpleLock;
};



//for common win32 HANDLE
template<typename T>
struct DefaultHandleCloser
{
	inline void operator()(T  t) const
    {
        ::CloseHandle(t);
    }
};

//for HDC 
template<typename T>
struct DCHandleCloser
{
	inline void operator()(T  t) const
    {
        ::DeleteDC(t);
    }
};

//for GDI Object
template<typename T>
struct GDIObjectCloser
{
	inline void operator()(T  t) const
	{
		::DeleteObject(t);
	}
};

template<typename T,  
			typename C = DefaultHandleCloser<HANDLE> ,
			T nullValue = NULL >
class ScopedHandleT
{
public:
	typedef T HandleType;

	explicit ScopedHandleT(T t) : handle(t) {}

	~ScopedHandleT()
	{
		c(handle);
	}

	bool IsVaild()
	{
		return handle != nullValue;
	}

	void Reset(T newHandle)
	{
		c(handle);
		handle = newHandle;
	}

	T Take()
	{
		T tmp = handle;
		handle = nullValue;
		return tmp;
	}

	operator T() { return handle; }
	
private:
	T handle;
	C c;

	ScopedHandleT(const ScopedHandleT&);
	void operator=(const ScopedHandleT&);
};

typedef ScopedHandleT<HANDLE>	ScopedHandle;
typedef ScopedHandleT<HANDLE,DefaultHandleCloser<HANDLE>, INVALID_HANDLE_VALUE>	ScopedFileHandle;
typedef ScopedHandleT<HDC,DCHandleCloser<HDC> > ScopedDC;
typedef ScopedHandleT<HFONT,GDIObjectCloser<HFONT> > ScopedFont;
typedef ScopedHandleT<HPEN,GDIObjectCloser<HPEN> > ScopedPen;
typedef ScopedHandleT<HBRUSH,GDIObjectCloser<HBRUSH> > ScopedBrush;
typedef ScopedHandleT<HPEN,GDIObjectCloser<HPEN> > ScopedPen;
typedef ScopedHandleT<HBITMAP,GDIObjectCloser<HBITMAP> > ScopedBitmap;


//for simple memory
template<typename T>
struct DefaultPtrDeleter
{
	inline void operator()(T  t) const
	{
		enum { type_must_be_complete = sizeof(t) };
		delete t;
	}
};

/**
	作用域内智能指针 code from chromium
*/
template <typename T, 
			typename D>
class ScopedPtrT{
public:

	typedef T element_type;

	// Constructor.  Defaults to intializing with NULL.
	// There is no way to create an uninitialized scoped_ptr.
	// The input parameter must be allocated with new.
	explicit ScopedPtrT(T* p = NULL) : ptr_(p) { }

	// Destructor.  If there is a C object, delete it.
	// We don't need to test ptr_ == NULL because C++ does that for us.
	~ScopedPtrT() {
		d(ptr_);
	}

	// Reset.  Deletes the current owned object, if any.
	// Then takes ownership of a new object, if given.
	// this->reset(this->get()) works.
	void Reset(T* p = NULL) {
		if (p != ptr_) {
			d(ptr_);
			ptr_ = p;
		}
	}

	// Accessors to get the owned object.
	// operator* and operator-> will assert() if there is no current object.
	T& operator*() const {
		assert(ptr_ != NULL);
		return *ptr_;
	}
	T* operator->() const  {
		assert(ptr_ != NULL);
		return ptr_;
	}
	T* Get() const { return ptr_; }

	// Comparison operators.
	// These return whether two scoped_ptr refer to the same object, not just to
	// two different but equal objects.
	bool operator==(T* p) const { return ptr_ == p; }
	bool operator!=(T* p) const { return ptr_ != p; }

	// Swap two scoped pointers.
	void Swap(ScopedPtrT& p2) {
		T* tmp = ptr_;
		ptr_ = p2.ptr_;
		p2.ptr_ = tmp;
	}

	// Release a pointer.
	// The return value is the current pointer held by this object.
	// If this object holds a NULL pointer, the return value is NULL.
	// After this operation, this object will hold a NULL pointer,
	// and will not own the object any more.
	T* Release()  {
		T* retVal = ptr_;
		ptr_ = NULL;
		return retVal;
	}

private:
	T* ptr_;
	D d;

	// Forbid comparison of scoped_ptr types.  If C2 != C, it totally doesn't
	// make sense, and if C2 == C, it still doesn't make sense because you should
	// never have the same object owned by two different scoped_ptrs.
	template <typename T2, typename D2> bool operator==(ScopedPtrT<T2, D2> const& p2) const;
	template <typename T2, typename D2> bool operator!=(ScopedPtrT<T2, D2> const& p2) const;

	// Disallow evil constructors
	ScopedPtrT(const ScopedPtrT&);
	void operator=(const ScopedPtrT&);
};

// Free functions
template <typename T,  typename D>
void Swap(ScopedPtrT<T, D>& p1, ScopedPtrT<T, D>& p2) {
	p1.Swap(p2);
}

template <typename T, typename D>
bool operator==(T* p1, const ScopedPtrT<T, D>& p2) {
	return p1 == p2.get();
}

template <typename T,typename D>
bool operator!=(T* p1, const ScopedPtrT<T, D>& p2) {
	return p1 != p2.get();
}


/**
	\brief VOID * 智能指针
*/
template< typename T>
class CVoidPSP
{
public:
	explicit CVoidPSP(void * p )
	{
		assert( p != NULL);
		m_pBuffer = p;
	}


	~CVoidPSP() 
	{
		
	}

	void * get()
	{
		return m_pBuffer;
	}

	void * operator& ()
	{
		assert( m_pBuffer != NULL);
		return m_pBuffer;
	}

	operator T*() const 
	{
		assert( m_pBuffer != NULL);

		return (T *)m_pBuffer;
	}

	T* operator->() const 
	{
		assert( m_pBuffer != NULL);

		return (T *)m_pBuffer;
	}
private:
	void * m_pBuffer;
};