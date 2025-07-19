#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "safe_free.h"
#include "flags6502.h"
#include "types6502.h"

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

START_TEST(flags_print_test)
{   
    Byte b1 = 0b00010111;
    Byte b2 = 0b11010101;
    Byte b3 = 0b11111111;
    Byte b4 = 0b00000000;

    flag_name(b1);
    flag_name(b2);
    flag_name(b3);
    flag_name(b4);
}
END_TEST

Suite* sum_suite(void)
{
    Suite* s = suite_create("6502");
    TCase* tc = tcase_create("safe_free");
    tcase_add_test(tc, flags_print_test);
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