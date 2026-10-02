#include <check.h>
#include "csptr/smart_ptr.h"
#include "utils.h"

START_TEST (test_pointer_valid) {
    int *a = unique_ptr(42);
    assert_valid_ptr(a);
    sfree(a);
} END_TEST

static int dtor_run = 0;

static void f_dtor_run(UNUSED void *ptr)
{
  dtor_run = 1;
}

START_TEST (test_dtor_run) {
    dtor_run = 0;

    f_destructor dtor = f_dtor_run;
    int *a = unique_ptr(42, dtor);
    assert_valid_ptr(a);
    sfree(a);
    ck_assert_msg(dtor_run, "Expected destructor to run");
} END_TEST

TCase *make_scalar_tests(void) {
    TCase *tc = tcase_create("scalar");
    tcase_add_test(tc, test_pointer_valid);
    tcase_add_test(tc, test_dtor_run);
    return tc;
}
