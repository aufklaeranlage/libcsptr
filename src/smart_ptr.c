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

#include "smart_ptr.h"
#include "mman.h"

#undef smalloc

void *make_shared(void *ptr)
{
  struct meta *meta = get_meta(ptr);
  if (meta->kind & SHARED)
    return sref(ptr);

  struct smalloc_args args = (struct smalloc_args) {
    .size = meta->size,
    .kind = SHARED,
    .dtor = meta->dtor,
  };
  void *newptr = smalloc(&args);
  if (newptr == NULL)
    return NULL;
  memcpy(newptr, ptr, meta->size);
  return newptr;
}

bool ensure_shared(void *ptr)
{
  struct meta *meta = get_meta(ptr);
  return (meta->kind & SHARED);
}

void *make_unique(void *ptr) {
  struct meta *meta = get_meta(ptr);
  if (meta->kind & UNIQUE)
    return NULL;

  struct smalloc_args args = (struct smalloc_args) {
    .size = meta->size,
    .kind = UNIQUE,
    .dtor = meta->dtor,
  };
  void *newptr = smalloc(&args);
  if (newptr == NULL)
    return NULL;
  memcpy(newptr, ptr, meta->size);
  return newptr;
}

bool ensure_unique(void *ptr)
{
  struct meta *meta = get_meta(ptr);
  return (meta->kind & UNIQUE);
}
