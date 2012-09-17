#ifndef __NON_COPY_ABLE_H__
#define __NON_COPY_ABLE_H__
/**
*	\file NonCopyable.h
*   \brief 使用类不可拷贝。
*   \remark 继承CNonCopyable的类均不可拷贝。
*           亦可在类定义中使用DECLARE_NON_COPY_ABLE(ClassName)宏;
*/
class CNonCopyable
{
protected:
	CNonCopyable(){}
	~CNonCopyable(){} //没有虚函数，故析构不需为虚

private:
	CNonCopyable(const CNonCopyable&);
	const CNonCopyable& operator = (const CNonCopyable&);
};

//使用注意事项：1、类必须提供自己的构造函数。2、在类定义的最尾部使用。
#define DECLARE_NON_COPY_ABLE(className) \
	private:\
		className(const className&);\
		const className& operator = (const className&);

#endif//__NON_COPY_ABLE_H__