#include <check.h>
#include "sum.h"

START_TEST(nums_sum_test)
{
    ck_assert_int_eq(sum(1, 3), 4);
    ck_assert_int_eq(sum(2, -2), 0);
    ck_assert_int_eq(sum(1, -3), -2);
}
END_TEST

Suite* sum_suite(void)
{
    Suite* s = suite_create("6502");
    TCase* tc = tcase_create("Sum");
    tcase_add_test(tc, nums_sum_test);
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