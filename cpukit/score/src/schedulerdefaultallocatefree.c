/**
 * @file
 *
 * @brief Scheduler Default Allocate and Release Operation
 *
 * @ingroup ScoreScheduler
 */

/*
 *  COPYRIGHT (c) 2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/scheduler.h>

bool _Scheduler_default_Allocate(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
)
{
  ( void ) scheduler;
  ( void ) the_thread;

  return true;
}

void _Scheduler_default_Free(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
)
{
  ( void ) scheduler;
  ( void ) the_thread;
}
