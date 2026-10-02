#include <check.h>
#include "csptr/smart_ptr.h"
#include "mman.h"
#include "utils.h"

START_TEST (test_meta_kind_shared) {
  void *ptr = smalloc(SHARED, 1);
  struct meta *meta = get_meta(ptr);
  ck_assert_msg(meta->kind == SHARED, "Expected meta kind to be SHARED");
  sfree(ptr);
}

START_TEST (test_meta_kind_unique) {
  void *ptr = smalloc(UNIQUE, 1);
  struct meta *meta = get_meta(ptr);
  ck_assert_msg(meta->kind == UNIQUE, "Expected meta kind to be UNIQUE");
  sfree(ptr);
}

START_TEST (test_meta_size) {
  void *ptr = smalloc(UNIQUE, 456);
  struct meta *meta = get_meta(ptr);
  ck_assert_msg(meta->size == 456, "Expected meta size ot be provided size");
  sfree(ptr);
}

static int dtor_run = 0;

static void tfree(UNUSED void *ptr)
{
  dtor_run = 1;
}

START_TEST (test_meta_dtor) {
  void *ptr = smalloc(UNIQUE, 8, tfree);
  struct meta *meta = get_meta(ptr);
  ck_assert_msg(meta->dtor == tfree, "Expected meta dtor to be function address");
  sfree(ptr);
}

START_TEST (test_meta_dtor_not_defined) {
  void *ptr = smalloc(UNIQUE, 1);
  struct meta *meta = get_meta(ptr);
  ck_assert_msg(meta->dtor == NULL, "Expected meta dtor to be NULL if not initialized");
  sfree(ptr);
}

TCase *make_basic_tests(void) {
    TCase *tc = tcase_create("basic");
    tcase_add_test(tc, test_meta_kind_shared);
    tcase_add_test(tc, test_meta_kind_unique);
    tcase_add_test(tc, test_meta_size);
    tcase_add_test(tc, test_meta_dtor);
    tcase_add_test(tc, test_meta_dtor_not_defined);
    return tc;
}
