#ifndef __REFERENCE_COUNTER_H__
#define __REFERENCE_COUNTER_H__
#ifdef _WIN32
/**
*	\file RefCounter.h
*	\brief  提供引用计数操作，需要使用引用计数的类直接继承本类，即可具备引用计数操作 
*			亦可聚合本类的对象达到引用计数的目的，推荐使用继承
*			使用CSmartPtr可以达到自动对引用计数进行管理的效果
*			使用方法:1、新定义的类（CNew）继承至CRefCounter，这里新类拥有引用计数功能。
*			2、定义智能类：typedef CSmartPtr<CNew> CNewPtr; 
*			3、到此：就可随意使用CNew的指针了，如：CNewPtr newPtr = new CNew;
*	\remark 引用计数使用注意：
*			引用计数使用时，可能出现循环引用的情况（如： A引用B， B又引用A）。
*			出现这种情况时双方都释放不了，出现了死锁的状况，故使用引用计数的对象系统
*			应当是设计良好的树形结构或线性结构，而不应该出现闭环的状况。这是设计的职责。
*/
#include <Windows.h>

namespace Utility
{
	class CRefCounter
	{
	public:
		CRefCounter();
		/**
		*	\brief 拷贝构造函数，不拷贝引用计数，引用计数从零开始
		*/
		CRefCounter(const CRefCounter&);
		virtual ~CRefCounter()
		{
		}
		/**
		*	\brief 赋值操作不改变引用计数
		*/
		CRefCounter& operator= (const CRefCounter&)
		{
			return *this;
		}

		/**
		*	\brief 增加引用
		*/
		virtual void addRef();
		/**
		*	\brief 减少引用
		*/
		virtual void release();
		/**
		*	\brief 获得当前的引用数
		*/
		virtual LONG getRefCnt() const ;
		/**
		*	\brief 设置当引用计数减少到0时的操作方式：
		*		false表示delete掉自己，true表示不delete
		*		对象构造时默认为false,即计数减少到0时delete自己
		*/
		virtual void setNoDelete(bool);

	protected:
		volatile LONG m_lRef;
		bool m_bNoDelete;

	};

	/**
	*	\brief 任何继承至CRefCounter的类，可以使用该类达到智能指针的效果
	*/
	template<typename T>
	class CSmartPtr
	{
	public:
		CSmartPtr(T* p = NULL)
		{
			m_p = p;
			if ( m_p )
			{
				m_p->addRef();	
			}
		}
		CSmartPtr(const CSmartPtr<T>& r)
		{
			m_p = r.m_p;
			if ( m_p )
			{
				m_p->addRef();
			}
		}
		~CSmartPtr()
		{
			if ( m_p )
			{
				m_p->release();
			}
		}
		CSmartPtr& operator = (const CSmartPtr<T> &r)
		{
			if ( r.m_p != m_p )
			{
				if ( m_p )
				{
					m_p->release();
				}

				m_p = r.m_p;
				if ( m_p )
				{
					m_p->addRef();
				}
				
			}
			return *this;
		}
		CSmartPtr& operator = (T* p)
		{
			if ( m_p != p )
			{
				if ( m_p )
				{
					m_p->release();
				}

				if ( p )
				{
					p->addRef();
				}
				m_p = p;
			}
			return *this;
		}
		
		T* get() const
		{
			return m_p;
		}

		/**
		*	\brief 注意：该函数可能抛异常。当指针为空时抛异常
		*			暂时找不到在此处不抛异常的方案。
		*/
		T* operator ->() const
		{
			assert( m_p != NULL );
			if ( !m_p )
			{
				throwNullException();
			}
			return m_p;
			
		}

		/**
		*	\brief 注意：该函数可能抛异常。当指针为空时抛异常
		*			暂时找不到在此处不抛异常的方案。
		*/
		T& operator *() const
		{
			assert( m_p != NULL );

			if ( !m_p )
			{
				throwNullException();
			}
			return *(m_p);
		}
		/**
		*	\brief 达到使用if(x)判断指针是否为空的效果（x为CSmarPtr对象）
		*/
		operator bool() const
		{
			return m_p ? true : false;
		}
		
		/**
		*	\brief 实现动态类型转换
		*/
		template<typename Y>
		static CSmartPtr dynamicCast( const CSmartPtr<Y> &r)
		{
			return CSmartPtr<T>(dynamic_cast<T*>(r.m_p));
		}
		
		/**
		*	\brief 实现将同一继承体系的指针类型转换成该智能指针对象
		*/
		template<typename Y>
		static CSmartPtr dynamicCast( Y* p )
		{
			return CSmartPtr<T>(dynamic_cast<T*>(p));
		}

	protected:
		void throwNullException() const 
		{
			std::string strErrMsg =  "SmartPtr using null pointer: ";
			strErrMsg += __FILE__;
			throw  strErrMsg;
		}
	public:
		T* m_p;
	};
}

#endif //_WIN32

#endif//!__REFERENCE_COUNTER_H__