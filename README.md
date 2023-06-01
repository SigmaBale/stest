# stest

## About
stest is small compact simple to understand and navigate
C library for running unit tests.

## Getting started
Clone the repository, and run 'make' to build from source.
If you want to 'install' the lib, use 'make install'.
Default installation path:
 - .so -> /usr/local/lib
 - .h  -> /usr/local/include

If you wish to change directory for header file and/or .so,
then either modify the LIB_DIR_INSTALL and HEADER_DIR_INSTALL
directories in Makefile or just copy paste the stest.h
and libstest.so anywhere you'd like.

After setting that up, just include the library in your source file,
and VERY IMPORTANT, make sure to add the instalation path for library
in path for linker to find it!
You need to set env variable LD_LIBRARY_PATH.

Set it in your shell config files:
- Fish: setenv LD_LIBRARY_PATH "/usr/local/lib"
- Bash: export LD_LIBRARY_PATH="/usr/local/lib"

You can use your own defined path also.
Just make sure the libstest.so is there in that directory.

Now for good measure do: 'sudo ldconfig'

## Example

```c
#include "include/stest.h"
#include <string.h>

void int_test(void);
void int_test_2(void);
void string_test(void);
void string_test_2(void);
void string_test_3(void);

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
```
