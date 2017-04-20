#define BOOST_TEST_MODULE "test_from_toml"
#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif
#include <toml/toml.hpp>
#include <map>
#include <unordered_map>
#include <list>
#include <deque>

BOOST_AUTO_TEST_CASE(test_from_toml_exact)
{
    toml::Boolean  b(true);
    toml::Integer  i(42);
    toml::Float    f(3.14);
    toml::String   s("hoge");
    toml::Datetime d(std::chrono::system_clock::now());
    toml::Array    a;
    a.emplace_back(2);
    a.emplace_back(7);
    a.emplace_back(1);
    a.emplace_back(8);
    a.emplace_back(2);
    toml::Table    t;
    t.emplace("val1", true);
    t.emplace("val2", 42);
    t.emplace("val3", 3.14);
    t.emplace("val4", "piyo");

    toml::value v1(b);
    toml::value v2(i);
    toml::value v3(f);
    toml::value v4(s);
    toml::value v5(d);
    toml::value v6(a);
    toml::value v7(t);

    toml::Boolean  u1;
    toml::Integer  u2;
    toml::Float    u3;
    toml::String   u4;
    toml::Datetime u5;
    toml::Array    u6;
    toml::Table    u7;

    toml::from_toml(u1, v1);
    toml::from_toml(u2, v2);
    toml::from_toml(u3, v3);
    toml::from_toml(u4, v4);
    toml::from_toml(u5, v5);
    toml::from_toml(u6, v6);
    toml::from_toml(u7, v7);

    BOOST_CHECK_EQUAL(u1, b);
    BOOST_CHECK_EQUAL(u2, i);
    BOOST_CHECK_EQUAL(u3, f);
    BOOST_CHECK_EQUAL(u4, s);
    BOOST_CHECK_EQUAL(u6.at(0).cast<toml::value_t::Integer>(), a.at(0).cast<toml::value_t::Integer>());
    BOOST_CHECK_EQUAL(u6.at(1).cast<toml::value_t::Integer>(), a.at(1).cast<toml::value_t::Integer>());
    BOOST_CHECK_EQUAL(u6.at(2).cast<toml::value_t::Integer>(), a.at(2).cast<toml::value_t::Integer>());
    BOOST_CHECK_EQUAL(u6.at(3).cast<toml::value_t::Integer>(), a.at(3).cast<toml::value_t::Integer>());
    BOOST_CHECK_EQUAL(u6.at(4).cast<toml::value_t::Integer>(), a.at(4).cast<toml::value_t::Integer>());
}

BOOST_AUTO_TEST_CASE(test_from_toml_cast)
{
    toml::Integer  i(42);
    toml::Float    f(3.14);
    toml::Array    a;
    a.emplace_back(2);
    a.emplace_back(7);
    a.emplace_back(1);
    a.emplace_back(8);
    a.emplace_back(2);
    toml::Table    t;
    t.emplace("val1", true);
    t.emplace("val2", 42);
    t.emplace("val3", 3.14);
    t.emplace("val4", "piyo");

    toml::value vi(i);
    toml::value vf(f);
    toml::value va(a);
    toml::value vt(t);

    int                     u1;
    std::size_t             u2;
    float                   u3;
    std::list<int>          u4;
    std::deque<std::size_t> u5;
    std::map<std::string, toml::value> u6;

    std::list<int> expect_list;
    expect_list.push_back(2);
    expect_list.push_back(7);
    expect_list.push_back(1);
    expect_list.push_back(8);
    expect_list.push_back(2);

    toml::from_toml(u1, vi);
    toml::from_toml(u2, vi);
    toml::from_toml(u3, vf);
    toml::from_toml(u4, va);
    toml::from_toml(u5, va);
    toml::from_toml(u6, vt);

    BOOST_CHECK_EQUAL(u1, 42);
    BOOST_CHECK_EQUAL(u2, 42ul);
    BOOST_CHECK_CLOSE_FRACTION(u3, 3.14, 1e-3);

    bool same_list = (u4 == expect_list);
    BOOST_CHECK(same_list);

    BOOST_CHECK_EQUAL(u5.at(0), 2);
    BOOST_CHECK_EQUAL(u5.at(1), 7);
    BOOST_CHECK_EQUAL(u5.at(2), 1);
    BOOST_CHECK_EQUAL(u5.at(3), 8);
    BOOST_CHECK_EQUAL(u5.at(4), 2);

    BOOST_CHECK_EQUAL(u6["val1"].cast<toml::value_t::Boolean>(), true);
    BOOST_CHECK_EQUAL(u6["val2"].cast<toml::value_t::Integer>(), 42);
    BOOST_CHECK_CLOSE_FRACTION(u6["val3"].cast<toml::value_t::Float>(), 3.14, 1e-3);
    BOOST_CHECK_EQUAL(u6["val4"].cast<toml::value_t::String >(), "piyo");

}
