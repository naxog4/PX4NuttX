/****************************************************************************
 * examples/nxflat/tests/pthread/pthread.c
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CHILD_ARG ((void*)0x12345678)
#define CHILD_RET ((void*)0x87654321)

/****************************************************************************
 * Private Types
 ****************************************************************************/

enum exit_values_e
{
  TESTRESULT_SUCCESS = 0,
  TESTRESULT_PTHREAD_ATTR_INIT_FAIL,
  TESTRESULT_PTHREAD_CREATE_FAIL,
  TESTRESULT_PTHREAD_JOIN_FAIL,
  TESTRESULT_CHILD_ARG_FAIL,
  TESTRESULT_CHILD_RETVAL_FAIL,
};

/****************************************************************************
 * External Functions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void *child_start_routine(void *arg)
{
  printf("CHILD: started with arg=%d\n", (int)arg);

  if (arg != CHILD_ARG)
    {
      printf("CHILD: expected arg=%d\n", (int)CHILD_ARG);
      return (void*)TESTRESULT_CHILD_ARG_FAIL;
    }
  sleep(2);

  printf("CHILD: returning %d\n", (int)CHILD_RET);
  pthread_exit(CHILD_RET);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(int argc, char **argv, char **envp)
{
  pthread_attr_t attr;
  pthread_t      thread;
  void          *retval;
  int            status;

  puts("PARENT: started\n");

  status = pthread_attr_init(&attr);
  if (status != 0)
    {
      printf("PARENT: pthread_attr_init() returned %d\n", status);
      exit(TESTRESULT_PTHREAD_ATTR_INIT_FAIL);
    }

  printf("PARENT: calling pthread_start with arg=%d\n", (int)CHILD_ARG);
  status = pthread_create(&thread, &attr, child_start_routine, CHILD_ARG);
  if (status != 0)
    {
      printf("PARENT: pthread_create() returned %d\n", status);
      exit(TESTRESULT_PTHREAD_CREATE_FAIL);
    }

  status = pthread_join(thread, &retval);
  if (status != 0)
    {
      printf("PARENT pthread_join() returned %d\n", status);

      exit(TESTRESULT_PTHREAD_JOIN_FAIL);
    }

  printf("PARENT child exitted with %d\n", (int)retval);
  if (retval != CHILD_RET)
    {
      printf("PARENT child thread did not exit with %d\n", (int)CHILD_RET);
      exit(TESTRESULT_CHILD_RETVAL_FAIL);
    }

  puts("PARENT returning success\n");
  return TESTRESULT_SUCCESS;
}
