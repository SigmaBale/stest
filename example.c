#include "include/stest.h"
#include <string.h>

// expands to 'void int_test(void);'
TEST(int_test);
TEST(int_test_2);
TEST(string_test);
TEST(string_test_2);
TEST(string_test_3);

int
main(void)
{
    ssuite* suite_str = ssuite_new("string_tests");
    ssuite* suite_int = ssuite_new("int_tests");

    ssuite_add_test(suite_str, string_test);
    ssuite_add_test(suite_str, string_test_2);
    ssuite_add_test(suite_str, string_test_3);

    ssuite_add_test(suite_int, int_test);
    ssuite_add_test(suite_int, int_test_2);

    srunner* runner = srunner_new();
    srunner_add_suite(runner, suite_str);
    srunner_add_suite(runner, suite_int);

    srunner_run(runner);
    srunner_free(runner);
}

TEST(int_test)
{
    int x = 5;
    int y = 6;

    ASSERT_EQ(sizeof(x), sizeof(y));

    ASSERT(x == y);
}

TEST(int_test_2)
{
    int x = 5;
    int y = 6;

    ASSERT_NEQ(x, y);
    ASSERT(6 == y);
}

TEST(string_test)
{
    const char* first  = "We are equal";
    const char* second = "We are equal";

    ASSERT_STR_EQ(first, second);
}

TEST(string_test_2)
{
    const char* first  = "We are equal";
    const char* second = "We are notequal";

    ASSERT_STR_NEQ(first, second);
    ASSERT_STR_EQ(first, "We are equal");
}

TEST(string_test_3)
{
    const char* first  = "We are equal";
    const char* second = "We are notequal";

    ASSERT_STR_EQ(first, second);
}
