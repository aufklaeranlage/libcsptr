#include <check.h>
#include "csptr/smart_ptr.h"
#include "utils.h"

START_TEST (test_shared_init) {
    void *ptr = shared_ptr(42);
    assert_valid_ptr(ptr);
    sfree(ptr);
} END_TEST

static int dtor_run = 0;

static void tfree(UNUSED void *ptr)
{
  dtor_run = 1;
}

START_TEST (test_shared_sref) {
    dtor_run = 0;

    f_destructor dtor = tfree;
    void *ptr = shared_ptr(42, dtor);
    assert_valid_ptr(ptr);

    {
        void *ptr2 = sref(ptr);
        ck_assert_msg(ptr == ptr2, "Expected reference to be the same pointer.");
        sfree(ptr2);
    }
    ck_assert_msg(dtor_run == 0, "Expected destructor NOT to have run.");
    sfree(ptr);
} END_TEST

TCase *make_shared_tests(void) {
    TCase *tc = tcase_create("shared");
    tcase_add_test(tc, test_shared_init);
    tcase_add_test(tc, test_shared_sref);
    return tc;
}
