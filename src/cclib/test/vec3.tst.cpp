
#define BOOST_TEST_MODULE example

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <iostream>

#include <math/vec3.h>

BOOST_AUTO_TEST_CASE(testVecConstruction)
{
    cclib::Vector3f a(1.2f, 3.4f, 4.4f);
    BOOST_CHECK(a.x() == 1.2f);
    BOOST_CHECK(a.y() == 3.4f);
    BOOST_CHECK(a.z() == 4.4f);
    
    cclib::Vector3f b;
    BOOST_CHECK(b.x() == 0.0f);
    BOOST_CHECK(b.y() == 0.0f);
    BOOST_CHECK(b.z() == 0.0f);

    cclib::Vector3f c(a);
    BOOST_CHECK(c.x() == a.x());
    BOOST_CHECK(c.y() == a.y());
    BOOST_CHECK(c.z() == a.z());
}

BOOST_AUTO_TEST_CASE(testVecSet)
{
    cclib::Vector3i a;
    a.set(1, 2, 3);
    BOOST_CHECK(a.x() == 1);
    BOOST_CHECK(a.y() == 2);
    BOOST_CHECK(a.z() == 3);

    cclib::Vector3i b;
    b.set(a);
    BOOST_CHECK(a.x() == b.x());
    BOOST_CHECK(a.y() == b.y());
    BOOST_CHECK(a.z() == b.z());
    
    cclib::Vector3i s(1,2,3);
    s.setZero();
    BOOST_CHECK(s.x() == 0);
    BOOST_CHECK(s.y() == 0);
    BOOST_CHECK(s.z() == 0);
}

BOOST_AUTO_TEST_CASE(testVecOperators)
{
    cclib::Vector3f a(1.2345, 2.3456, 3.4567);
    cclib::Vector3f b(1.2345, 2.3456, 3.4567);
    cclib::Vector3f c;

    BOOST_CHECK(a == b);
    BOOST_CHECK(b == a);
    BOOST_CHECK(!(a == c));
    BOOST_CHECK(!(b == c));

    BOOST_CHECK(c.equalsZero()); 
    BOOST_CHECK(!a.equalsZero()); 
    
    cclib::Vector3f l(3.0f, 0.0f, 0.0f); 
    BOOST_CHECK(l.length() == 3.0f); 
    BOOST_CHECK(l.lengthSquared() == 9.0f); 

    cclib::Vector3f d(1.0, 2.0, 3.0);
    BOOST_CHECK(d.dot(l) == 3.0); 

    // implement me!!!
    // cclib::Vector3f cross(2.0, -1.0, 3.0);
    // BOOST_CHECK(*(d.cross()) == cross); 

    cclib::Vector3d r;
    r.randomize();
    BOOST_CHECK(r.x() != 0.0f);
    BOOST_CHECK(r.y() != 0.0f);
    BOOST_CHECK(r.z() != 0.0f);
}

BOOST_AUTO_TEST_CASE(testDistance) 
{
    cclib::Vector3f v1(1.0f, 0.0f, 0.0f);
    cclib::Vector3f v2(-1.0f, 0.0f, 0.0f);
    BOOST_CHECK(v1.distance(v2) == 2.0f); 
    
    cclib::Vector3f v3(1.0f, 2.0f, 0.0f);
    cclib::Vector3f v4(-1.0f, 5.0f, 0.0f);
    BOOST_CHECK(v3.distance(v4) == v3.distance(v4.x(), v4.y(), v4.z())); 
}

BOOST_AUTO_TEST_CASE(testMoreOperators)
{
    cclib::Vector3f v1(1.0f, 2.0f, 0.0f);
    cclib::Vector3f v2(-1.0f, 5.0f, 1.0f);
     
    cclib::Vector3f v3 = v1 + v2;
    BOOST_CHECK(v3.x() == 0.0f);
    BOOST_CHECK(v3.y() == 7.0f);
    BOOST_CHECK(v3.z() == 1.0f);
    
    v3 = v1 - v2;
    BOOST_CHECK(v3.x() == 2.0f);
    BOOST_CHECK(v3.y() == -3.0f);
    BOOST_CHECK(v3.z() == -1.0f);
}

BOOST_AUTO_TEST_CASE(testSimplexNoiseGenerator)
{
    cclib::Vector3f v1 = cclib::Vector3f::simplexNoise(1.0f, 1.0f, 2.0f, 1.0f, 10.0f, 1.0f); 
    BOOST_CHECK(v1.x() == 0.760099828243255615234375f);
    BOOST_CHECK(v1.y() == 0.0f);
    BOOST_CHECK(v1.z() == 0.107877723872661590576171875f);
    
    // with 10 times the range
    cclib::Vector3f v2 = cclib::Vector3f::simplexNoise(1.0f, 1.0f, 2.0f, 1.0f, 10.0f, 1.0f, -10.f, 10.0f); 
    v1 = v1.scale(10.0);
    BOOST_CHECK(v1.x() == v2.x());
    BOOST_CHECK(v1.y() == v2.y());
    BOOST_CHECK(v1.z() == v2.z());
}
