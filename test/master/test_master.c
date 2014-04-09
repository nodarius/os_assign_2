/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "test/master/test_master.check" instead.
 */

#include <check.h>

#line 1 "test/master/test_master.check"
START_TEST(calc_entered_test)
{
#line 2
ck_assert_msg(calc_entered("calc") == 1, "calc\n");
ck_assert_msg(calc_entered("calc\n") == 1, "calc\n");
ck_assert_msg(calc_entered("calc\0") == 1, "calc\n");
ck_assert_msg(calc_entered("calc\n123") == 1, "calc\n");
ck_assert_msg(calc_entered("CALC") == 0, "calc\n");
ck_assert_msg(calc_entered("asdjsd") == 0, "calc\n");

}
END_TEST

START_TEST(valid_input_test)
{
#line 10
ck_assert(valid_input("2+2") == 1);
ck_assert(valid_input("2-3") == 1);
ck_assert(valid_input("2*2") == 1);
ck_assert(valid_input("2/3") == 1);
ck_assert(valid_input("2:3") == 1);
ck_assert(valid_input("2//3") == 1);
ck_assert(valid_input("2::3") == 1);


}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, calc_entered_test);
    tcase_add_test(tc1_1, valid_input_test);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}