#include <check.h>
#include "csptr/smart_ptr.h"
#include "utils.h"

START_TEST (test_unique_init) {
    void *ptr = unique_ptr(42);
    assert_valid_ptr(ptr);
    sfree(ptr);
} END_TEST

// static int dtor_run = 0;
//
// static void tfree(UNUSED void *ptr)
// {
//   dtor_run = 1;
// }
//
// START_TEST (test_unique_sref) {
//     dtor_run = 0;
//
//     f_destructor dtor = tfree;
//     void *ptr = unique_ptr(42, dtor);
//     assert_valid_ptr(ptr);
//
//     {
//         void *ptr2 = sref(ptr);
//         ck_assert_msg(ptr2 == NULL, "Expected reference to UNIQUE pointer ot be NULL.");
//         sfree(ptr2);
//     }
//     ck_assert_msg(dtor_run == 0, "Expected destructor NOT to have run.");
//     sfree(ptr);
// } END_TEST

TCase *make_unique_tests(void) {
    TCase *tc = tcase_create("unique");
    tcase_add_test(tc, test_unique_init);
    // tcase_add_test(tc, test_unique_sref);
    return tc;
}
