/*	$Id: semaphore.h,v 1.1.1.1 2004/01/15 02:44:21 rh Exp $	*/

/*-
 * Copyright (c) 2011 Rene Hexel
 * Copyright (c) 2003, 2004 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Christos Zoulas and Rene Hexel.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PTHREAD_GU_SEMAPHORE_H_
#define _PTHREAD_GU_SEMAPHORE_H_

/* Simplified POSIX 1003.1b semaphores */

struct _pthread_gu_sem_st;
typedef	struct _pthread_gu_sem_st *gu_sem_t;

#ifndef SEM_VALUE_MAX
#define	SEM_FAILED	((gu_sem_t *)0)
#define	SEM_VALUE_MAX	(~0U)
#endif

#include <sys/cdefs.h>

__BEGIN_DECLS
int	 gu_sem_destroy(gu_sem_t *);
int	 gu_sem_getvalue(gu_sem_t * __restrict, int * __restrict);
int	 gu_sem_init(gu_sem_t *, int, unsigned int);
int	 gu_sem_post(gu_sem_t *);
int	 gu_sem_trywait(gu_sem_t *);
int	 gu_sem_wait(gu_sem_t *);

/** Not yet implemented
gu_sem_t	*gu_sem_open(const char *, int, ...);
int	 gu_sem_close(gu_sem_t *);
int	 gu_sem_unlink(const char *);
**/

#ifndef GU_SEM_NO_POSIX
#define sem_t           gu_sem_t
#define sem_destroy(s)  gu_sem_destroy(s)
#define sem_getvalue(s, p) gu_sem_getvalue((s), (p))
#define sem_init(s,p,v) gu_sem_init((s), (p), (v))
#define sem_post(s)     gu_sem_post(s)
#define sem_wait(s)     gu_sem_wait(s)
#define sem_trywait(s)  gu_sem_trywait(s)
#endif

__END_DECLS

#endif /* !_PTHREAD_GU_SEMAPHORE_H_ */
