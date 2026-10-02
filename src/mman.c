/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
 * Copyright © 2026 Arn "Cor" Bronner <asbronner@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "mman.h"

/* Provide pseudo location for function?? https://www.reddit.com/r/AskProgramming/comments/1cn1bhj/how_to_use_inline_function_in_c/ */
extern CSPTR_PURE CSPTR_INLINE struct meta *get_meta(void *ptr);
extern CSPTR_INLINE size_t align(size_t s);

#undef smalloc

struct allocator smalloc_allocator = {malloc, free};

#ifdef _MSC_VER
# include <windows.h>
# include <malloc.h>
#endif

#ifndef _MSC_VER
static CSPTR_INLINE size_t atomic_add(volatile size_t *count, const size_t limit, const size_t val)
{
    size_t old_count, new_count;
    do {
      old_count = *count;
      if (old_count == limit)
          abort();
      new_count = old_count + val;
    } while (!__sync_bool_compare_and_swap(count, old_count, new_count));
    return new_count;
}
#endif

static CSPTR_INLINE size_t atomic_increment(volatile size_t *count)
{
#ifdef _MSC_VER
    return InterlockedIncrement64(count);
#else
    return atomic_add(count, SIZE_MAX, 1);
#endif
}

static CSPTR_INLINE size_t atomic_decrement(volatile size_t *count)
{
#ifdef _MSC_VER
    return InterlockedDecrement64(count);
#else
    return atomic_add(count, 0, -1);
#endif
}

void *sref(void *ptr)
{
  struct meta *meta = get_meta(ptr);
  assert(meta->ptr == ptr);
  assert(meta->kind & SHARED);
  if (!(meta->kind & SHARED))
    return NULL;
  atomic_increment(&((struct meta *)meta)->ref_count);
  return ptr;
}

void *smove_size(void *ptr, size_t size)
{
  struct meta *meta = get_meta(ptr);
  assert(meta->kind & UNIQUE);

  struct smalloc_args args;

  args = (struct smalloc_args) {
    .size = size,
    .kind = SHARED,
    .dtor = meta->dtor,
  };

  void *newptr = smalloc(&args);
  memcpy(newptr, ptr, meta->size);
  return newptr;
}

CSPTR_MALLOC_API CSPTR_INLINE static void *alloc_entry(size_t head, size_t size)
{
  const size_t totalsize = head + size;
#ifdef SMALLOC_FIXED_ALLOCATOR
  return malloc(totalsize);
#else /* !SMALLOC_FIXED_ALLOCATOR */
  return smalloc_allocator.alloc(totalsize);
#endif /* !SMALLOC_FIXED_ALLOCATOR */
}

CSPTR_MALLOC_API CSPTR_INLINE static void dealloc_entry(struct meta *meta, void *ptr)
{
  if (meta->dtor)
    meta->dtor(ptr);
#ifdef SMALLOC_FIXED_ALLOCATOR
  return free(meta);
#else /* !SMALLOC_FIXED_ALLOCATOR */
  return smalloc_allocator.dealloc(meta);
#endif /* !SMALLOC_FIXED_ALLOCATOR */
}

CSPTR_MALLOC_API void *smalloc(struct smalloc_args *args)
{
  if (!args->size)
    return NULL;

  size_t size = align(args->size);
  size_t head_size = sizeof(struct meta);
  struct meta *ptr = alloc_entry(head_size, size);
  if (ptr == NULL)
    return NULL;

  char *shifted = (char *)ptr + head_size;
  *(struct meta *)ptr = (struct meta) {
    .kind = args->kind,
    .dtor = args->dtor,
    .size = args->size,
#ifndef NDEBUG
    .ptr = shifted,
#endif /* !NDBEUG */
  };

  if (args->kind & SHARED)
    ptr->ref_count = 1;
  return shifted;
}

void sfree(void *ptr)
{
  if (!ptr)
    return ;

  assert((size_t)ptr == align((size_t)ptr));
  struct meta *meta = get_meta(ptr);
  assert(meta->ptr == ptr);

  if (meta->kind & SHARED && atomic_decrement(&((struct meta *)meta)->ref_count))
    return ;

  dealloc_entry(meta, ptr);
}
