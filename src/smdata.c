
typedef enum { ASSERT, ASSERT_EQ, ASSERT_STREQ, ASSERT_NEQ } assert_t;

typedef struct {
    assert_t    assertion_type;
    const char* exp;
    const char* src;
    const char* dst;
    const char* file;
    const char* line;
    const char* fn_name;
} Metadata;
