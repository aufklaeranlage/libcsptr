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

#ifndef CSPTR_MMAN_H
# define CSPTR_MMAN_H

# include "smalloc.h"

struct meta {
  enum pointer_kind kind;
  size_t size;
  f_destructor dtor;
#ifndef NDEBUG
  void *ptr;
#endif /* !NDEBUG */
  volatile size_t ref_count;
};

CSPTR_INLINE size_t align(size_t s)
{
  return (s + (sizeof(char *) - 1)) & ~(sizeof(char *) - 1);
}

CSPTR_PURE CSPTR_INLINE struct meta *get_meta(void *ptr)
{
  return (struct meta *)((char *)ptr - sizeof(struct meta));
}

#endif /* !CSPTR_MMAN_H */
