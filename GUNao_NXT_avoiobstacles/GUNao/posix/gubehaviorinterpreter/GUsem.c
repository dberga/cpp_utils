/*
 * Copyright (C) 2004, 2011 Rene Hexel. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified other than the possible
 *    addition of one or more copyright notices.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice(s), this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. The name of the copyright holder may not be used to endorse or
 *    promote products derived from this software without specific
 *    prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*	$Id: sem.c,v 1.1.1.1 2004/01/15 02:44:21 rh Exp $	*/

#include <sys/types.h>

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include "GUsemaphore.h"

#define	ERROR(e)	do { errno = (e) ; return -1; }			\
				/* NOTREACHED */ while (/* CONSTCOND */ 0)

struct _pthread_gu_sem_st {
	int		psem_value;	/* current semaphore value */
	pthread_cond_t	psem_notify;	/* notification to other threads */
	pthread_mutex_t	psem_lock;	/* mutex for exclusive access */
};

int
gu_sem_init(gu_sem_t *psem, int pshared, unsigned value)
{
	gu_sem_t sem;
	int err;

	if (pshared) 			/* not implemented */
		ERROR(EPERM);

	if (value > SEM_VALUE_MAX)
		ERROR(EINVAL);

	if ((sem = malloc(sizeof(struct _pthread_gu_sem_st))) == NULL)
		ERROR(ENOSPC);

	sem->psem_value = value;

	if ((err = pthread_cond_init(&sem->psem_notify, NULL)) != 0) {
		free(sem);
		ERROR(err);
	}

	if ((err = pthread_mutex_init(&sem->psem_lock, NULL)) != 0) {
		pthread_cond_destroy(&sem->psem_notify);
		free(sem);
		ERROR(err);
	}

	*psem = sem;

	return 0;
}


int
gu_sem_destroy(gu_sem_t *psem)
{
	gu_sem_t sem = *psem;

	pthread_mutex_destroy(&sem->psem_lock);
	pthread_cond_destroy(&sem->psem_notify);
	free(sem);

	return 0;
}


static void
gu_sem_unlock(void *p)
{
	gu_sem_t sem = p;
	pthread_mutex_unlock(&sem->psem_lock);
}


int
gu_sem_wait(gu_sem_t *psem)
{
	gu_sem_t sem = *psem;
	int err;

	if ((err = pthread_mutex_lock(&sem->psem_lock)) != 0)
		ERROR(err);

	pthread_cleanup_push(gu_sem_unlock, sem);
	while (sem->psem_value <= 0)
		pthread_cond_wait(&sem->psem_notify, &sem->psem_lock);
	pthread_cleanup_pop(0);

	sem->psem_value--;

	gu_sem_unlock(sem);

	return 0;
}


int
gu_sem_trywait(gu_sem_t *psem)
{
	gu_sem_t sem = *psem;
	int err;

	if ((err = pthread_mutex_lock(&sem->psem_lock)) != 0)
		ERROR(err);

	if (sem->psem_value <= 0) {
		gu_sem_unlock(sem);
		ERROR(EAGAIN);
	}

	sem->psem_value--;

	gu_sem_unlock(sem);

	return 0;
}


int
gu_sem_post(gu_sem_t *psem)
{
	gu_sem_t sem = *psem;
	int err;

	if ((err = pthread_mutex_lock(&sem->psem_lock)) != 0)
		ERROR(err);

	if (++sem->psem_value > 0)
		pthread_cond_signal(&sem->psem_notify);

	gu_sem_unlock(sem);

	return 0;
}


int
gu_sem_getvalue(gu_sem_t *psem, int *sval)
{
	gu_sem_t sem = *psem;
	int err;

	if ((err = pthread_mutex_lock(&sem->psem_lock)) != 0)
		ERROR(err);

	*sval = sem->psem_value;

	gu_sem_unlock(sem);

	return 0;
}


