#include "easy_cppunit_proxy.h"
#include <iostream>
//
// TestCase class
//
class TestTemplate : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestNull);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(TemplateSpecialization);
	CPPUNIT_TEST(explicit_specialization);
	CPPUNIT_TEST(partial_specialization);
	CPPUNIT_TEST(partial_order);
	CPPUNIT_TEST(param_int);
	CPPUNIT_TEST_SUITE_END();

protected:
	void TemplateSpecialization();
	void explicit_specialization();
	void partial_specialization();
	void partial_order();
	void param_int();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestTemplate);

//////////////////////////////////////////////////////////////////////////
/*
��ģ�����ƫ�ػ�������ʽ�ػ���������ģ��ֻ����ʾ�ػ���ȫ�ػ���
������ô���ô��ƫ�ػ�ֻ����������ػ����� T�ػ�ΪT*,const T*,�ȵȣ���ȫ�ػ�����嵽���ͣ���int,int*,const int*
*/
template<class T>	//	a
class X
{
private:
	int a_;
};

template<class T> //	b
void f(T)
{
	 
}

template<class T>	//	c
void f(int,T,double)
{

}

template<class T>	// 	��a�����ָ�����͵�ƫ�ػ�
class X <T*>
{

};

template<>			// 	��a�����int���͵�ƫ�ػ�
class X <int>
{

};

template<class T>	//	d	����b,c������ģ�岻��ƫ�ػ�
void f(T*) {} 

template<>			//	e  ����int����ʾ�ػ�
void f(int) {} 

void f(double)		//	f	
{

}

void TestTemplate::TemplateSpecialization()
{
	bool b = false;
	int i = 0;
	double d = 0.0f;
	f(b);		//	b
	f(i,42,d);	//	c
	f(&i);		//	d
	f(i);		//	e
	f(d);		//	f
}

#include <iostream>
template<typename _Key>
struct hash
{
	void operator()() { std::cout << "hash<T>" << std::endl;}
};

//	explicit specialization
template<>
struct hash<char>
{
	void operator()() { std::cout << "hash<char>" << std::endl;}
};

template<>
struct hash<unsigned char>
{
	void operator()() { std::cout << "hash<unsigned char>" << std::endl;}
};

void TestTemplate::explicit_specialization()
{
	hash<int> __h1;
	hash<char> __h2;
	hash<unsigned char> __h3;

	__h1();
	__h2();
	__h3();

}

template<typename I,typename O>
class test_class
{
public:
	test_class() { std::cout << "test_class I, O" << std::endl; }
};

template<typename T>
class test_class<T*,T*>
{
public:
	test_class() { std::cout << "test_class *T,*T" << std::endl; }
};

template<typename T>
class test_class<const T*,T*>
{
public:
	test_class() { std::cout << "test_class const *T,*T" << std::endl; }
};

void TestTemplate::partial_specialization()
{
	test_class<int,char>	__test1;
	test_class<int*,int*>	__test2;
	test_class<const int*,int*>	__test3;
}

template <class T> void f1(T) 
{
	std::cout << "Less specialized function called" << std::endl;
}

template <class T> void f1(T*) 
{
	std::cout << "More specialized function called" << std::endl;
}

template <class T> void f1(const T*) 
{
	std::cout << "Even more specialized function for const T*" << std::endl;
}

void TestTemplate::partial_order()
{
	int i = 0;
	const int j = 0;
	int *pi = &i;
	const int *cpi = &j;
	f1(i);
	f1(pi);
	f1(cpi);
}

template<typename T, int _Max>
void Param_Int(T _Type)
{
	std::cout << "Param_Int " << _Max << std::endl;
}

void TestTemplate::param_int()
{
	Param_Int<int,4>(3);
}
