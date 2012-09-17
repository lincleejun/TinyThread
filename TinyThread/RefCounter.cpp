#include <assert.h>
#include "RefCounter.h"

using namespace Utility;
CRefCounter::CRefCounter()
{
	m_lRef = 0;
	m_bNoDelete = false;
}

CRefCounter::CRefCounter(const CRefCounter&)
{
	m_lRef = 0;
	m_bNoDelete = false;
}

void CRefCounter::addRef()
{
	assert(::InterlockedExchangeAdd(static_cast<volatile LONG *>(&m_lRef), 0) >= 0);
	::InterlockedIncrement(static_cast<volatile LONG *>(&m_lRef));
}

void CRefCounter::release()
{
	assert(::InterlockedExchangeAdd((volatile LONG *)(&m_lRef), 0) > 0);
	if(::InterlockedDecrement(static_cast<volatile LONG *>(&m_lRef)) == 0 && !m_bNoDelete)
	{
		m_bNoDelete = true;
		delete this;
	}
}

LONG CRefCounter::getRefCnt() const 
{
	return InterlockedExchangeAdd(const_cast<volatile LONG*>(&m_lRef), 0);
}

void CRefCounter::setNoDelete(bool bNoDelete)
{
	m_bNoDelete = bNoDelete;
}