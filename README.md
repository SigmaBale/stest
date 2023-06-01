# stest

## About
stest is small compact simple to understand and navigate
C library for running unit tests.

## Getting started
Clone the repository, and run 'make' to build from source.
Then do 'make install' to install the library.
Default installation path:
 - .so -> /usr/local/bin
 - .h  -> /usr/local/include

If you wish to change directory for header file and/or .so,
then either modify the BIN_DIR_INSTALL and HEADER_DIR_INSTALL
directories in Makefile or just copy paste the stest.h
and libstest.so anywhere you'd like.

After setting that up, just include the library in your source file:
`#include<stest.h>`
and you are all set.

## Example

```c
#include <stest.h>
#include <ccol.h>

int main(void) {
    // Use any name you'd like
    ssuite* suite = ssuite_new("vec_tests");

    // Insert tests into suite
    ssuite_add_test(suite, vec_create_test);
    ssuite_add_test(suite, vec_insert_test);

    // Create test runner
    srunner* runner = srunner_new();
    
    // Add suite into the runner
    srunner_add_suite(runner, suite);

    // Run all suites of tests
    srunner_run(runner);

    return 0;
}

TEST(vec_create_test) {
    cvec* vec = cvec_new();

    ASSERT(sizeof(vec) == sizeof(cvec));

    ASSERT_NEQ(vec != NULL);
}

TEST(vec_insert_test) {
    cvec* vec = cvec_new(sizeof(int));

    ASSERT(vec != NULL);

    cvec_push(vec, &(int){5});

    ASSERT_EQ(cvec_len(vec), 1);
}
```
