#ifndef UTILS_H_
# define UTILS_H_

#include <check.h>
#include <inttypes.h>

inline int is_aligned(void *ptr) {
    uintptr_t off = (uintptr_t) ptr;
    return !(off % sizeof (void *));
}

inline void assert_valid_ptr(void *ptr) {
    ck_assert_msg(ptr != NULL,
            "Expected unique_ptr to return a non-null pointer.");

    ck_assert_msg(is_aligned(ptr),
            "Expected unique_ptr to return an aligned pointer.");
}

#define UNUSED __attribute__ ((unused))

#endif /* !UTILS_H_ */
