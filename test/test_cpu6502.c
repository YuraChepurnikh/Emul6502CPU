#include <check.h>
#include <stdlib.h>
#include "safe_free.h"

START_TEST(safe_free_test)
{
    int* ptr = (int*) malloc(sizeof(int));

    /*
     * Although free(NULL) is a no-op, we want 
     * to check that malloc returned something
     * other than null.
     */

    if (ptr)
    {
        safe_free(ptr);
        ck_assert_ptr_null(ptr);
    }
}
END_TEST

Suite* sum_suite(void)
{
    Suite* s = suite_create("6502");
    TCase* tc = tcase_create("safe_free");
    tcase_add_test(tc, safe_free_test);
    suite_add_tcase(s, tc);
    return s;
}

int main(void)
{
    SRunner* sr = srunner_create(sum_suite());
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return failed;
}