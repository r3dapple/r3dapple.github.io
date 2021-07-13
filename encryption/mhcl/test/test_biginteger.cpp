#define BOOST_TEST_MODULE BigIntegerTests
#include <boost/test/unit_test.hpp>

#include "../src/biginteger/biginteger.h"

BOOST_AUTO_TEST_CASE(SMALL_MULTIPLICATION)
{
    BigInteger b(5);
    b.multiply("4");
    BigInteger r("20");
    BOOST_CHECK_EQUAL(r.getNumber(), b.getNumber());
}

BOOST_AUTO_TEST_CASE(BIG_MULTIPLICATION)
{
    BigInteger b("2345353532543245");
    b.multiply("46786578675475647");
    BigInteger r("109731067572139265403993971854515");
    BOOST_CHECK_EQUAL(r.getNumber(), b.getNumber());
}
