#include <check.h>
#include "csptr/smart_ptr.h"
#include "mman.h"
#include "utils.h"

START_TEST (test_ensure_shared_from_unique) {
  void *ptr = smalloc(UNIQUE, 1);
  assert_valid_ptr(ptr);
  ck_assert_msg(!ensure_shared(ptr), "Expected ensure_shared to return false on UNIQUE pointer"); sfree(ptr);
}

START_TEST (test_ensure_shared_from_shared) {
  void *ptr = smalloc(SHARED, 1);
  assert_valid_ptr(ptr);
  ck_assert_msg(ensure_shared(ptr), "Expected ensure_shared to return true on SHARED pointer");
  sfree(ptr);
}

START_TEST (test_make_shared_from_unique) {
  void *ptr = smalloc(UNIQUE, 1);
  assert_valid_ptr(ptr);
  void *ptr2 = make_shared(ptr);
  assert_valid_ptr(ptr2);
  ck_assert_msg(ptr != ptr2, "Expected pointer returned by make_shared from UNIQUE pointer to be new");
  sfree(ptr);
  sfree(ptr2);
}

START_TEST (test_make_shared_from_shared) {
  void *ptr = smalloc(SHARED, 1);
  assert_valid_ptr(ptr);
  void *ptr2 = make_shared(ptr);
  assert_valid_ptr(ptr);
  ck_assert_msg(ptr2 == ptr, "Expected pointer returned by make_shared from SHARED pointer to be the same");
  sfree(ptr);
  sfree(ptr2);
}

START_TEST (test_ensure_unique_from_shared) {
  void *ptr = smalloc(SHARED, 1);
  assert_valid_ptr(ptr);
  ck_assert_msg(!ensure_unique(ptr), "Expected ensure_unqiue to return false on SHARED pointer");
  sfree(ptr);
}

START_TEST (test_ensure_unique_from_unique) {
  void *ptr = smalloc(UNIQUE, 1);
  assert_valid_ptr(ptr);
  ck_assert_msg(ensure_unique(ptr), "Expected ensure_unique to return true on UNIQUE pointer");
  sfree(ptr);
}

START_TEST (test_make_unique_from_shared) {
  void *ptr = smalloc(SHARED, 1);
  assert_valid_ptr(ptr);
  void *ptr2 = make_unique(ptr);
  assert_valid_ptr(ptr2);
  ck_assert_msg(ptr != ptr2, "Expected pointer returned by make_unique from SHARED pointer to be new");
  sfree(ptr);
  sfree(ptr2);
}

START_TEST (test_make_unique_from_unique) {
  void *ptr = smalloc(UNIQUE, 1);
  assert_valid_ptr(ptr);
  void *ptr2 = make_unique(ptr);
  ck_assert_msg(ptr2 == NULL, "Expected pointer returned by make_unique from UNIQUE pointer to be NULL");
  sfree(ptr);
}

TCase *make_conv_tests(void) {
    TCase *tc = tcase_create("conv");
    tcase_add_test(tc, test_ensure_shared_from_shared);
    tcase_add_test(tc, test_ensure_shared_from_unique);
    tcase_add_test(tc, test_make_shared_from_shared);
    tcase_add_test(tc, test_make_shared_from_unique);
    tcase_add_test(tc, test_ensure_unique_from_unique);
    tcase_add_test(tc, test_ensure_unique_from_shared);
    tcase_add_test(tc, test_make_unique_from_unique);
    tcase_add_test(tc, test_make_unique_from_shared);
    return tc;
}
