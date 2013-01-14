
#define BOOST_TEST_MODULE example

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <math/vec2.h>

BOOST_AUTO_TEST_CASE(testVecConstruction)
{
    cclib::Vector2f a(1.2f, 3.4f);
    BOOST_CHECK(a.x() == 1.2f);
    BOOST_CHECK(a.y() == 3.4f);
    
    cclib::Vector2f b;
    BOOST_CHECK(b.x() == 0.0f);
    BOOST_CHECK(b.y() == 0.0f);

    cclib::Vector2f c(a);
    BOOST_CHECK(c.x() == a.x());
    BOOST_CHECK(c.y() == a.y());
}

BOOST_AUTO_TEST_CASE(testVecSet)
{
    cclib::Vector2i a;
    a.set(1, 2);
    BOOST_CHECK(a.x() == 1);
    BOOST_CHECK(a.y() == 2);

    cclib::Vector2i b;
    b.set(a);
    BOOST_CHECK(a.x() == b.x());
    BOOST_CHECK(a.y() == b.y());
    
    cclib::Vector2i s;
    s.setPolar(10, 3);
    BOOST_CHECK(s.x() == -9);
    BOOST_CHECK(s.y() == 1);

    s.setZero();
    BOOST_CHECK(s.x() == 0);
    BOOST_CHECK(s.y() == 0);
}

BOOST_AUTO_TEST_CASE(testVecOperators)
{
    cclib::Vector2f a(1.2345, 2.3456);
    cclib::Vector2f b(1.2345, 2.3456);
    cclib::Vector2f c;

    BOOST_CHECK(a == b);
    BOOST_CHECK(b == a);
    BOOST_CHECK(!(a == c));
    BOOST_CHECK(!(b == c));

    BOOST_CHECK(c.equalsZero()); 
    BOOST_CHECK(!a.equalsZero()); 
    
    cclib::Vector2f l(3.0f, 0.0f); 
    BOOST_CHECK(l.length() == 3.0f); 
    BOOST_CHECK(l.lengthSquared() == 9.0f); 

    cclib::Vector2f d(1.0, 2.0);
    BOOST_CHECK(d.dot(l) == 3.0); 

    cclib::Vector2f cross(2.0, -1.0);
    BOOST_CHECK(*(d.cross()) == cross); 

    cclib::Vector2d r;
    r.randomize();
    BOOST_CHECK(r.x() != 0.0f);
    BOOST_CHECK(r.y() != 0.0f);
}

BOOST_AUTO_TEST_CASE(testDistance) 
{
    cclib::Vector2f v1(1.0f, 0.0f);
    cclib::Vector2f v2(-1.0f, 0.0f);
    BOOST_CHECK(v1.distance(v2) == 2.0f); 
    
    cclib::Vector2f v3(1.0f, 2.0f);
    cclib::Vector2f v4(-1.0f, 5.0f);
    BOOST_CHECK(v3.distance(v4) == v3.distance(v4.x(), v4.y())); 
}

BOOST_AUTO_TEST_CASE(testMoreOperators)
{
    cclib::Vector2f v1(1.0f, 2.0f);
    cclib::Vector2f v2(-1.0f, 5.0f);
     
    cclib::Vector2f::Ptr v3 = v1 + v2;
    BOOST_CHECK(v3->x() == 0.0f);
    BOOST_CHECK(v3->y() == 7.0f);
    
    v3 = v1 - v2;
    BOOST_CHECK(v3->x() == 2.0f);
    BOOST_CHECK(v3->y() == -3.0f);
}

