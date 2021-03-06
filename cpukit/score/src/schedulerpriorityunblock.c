/**
 *  @file
 *
 *  @brief Scheduler Priority Unblock
 *  @ingroup ScoreScheduler
 */

/*
 *  Scheduler Handler
 *
 *  Copyright (C) 2010 Gedare Bloom.
 *  Copyright (C) 2011 On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/schedulerpriorityimpl.h>

void _Scheduler_priority_Unblock (
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
)
{
  Scheduler_priority_Context *context =
    _Scheduler_priority_Get_context( scheduler );
  Scheduler_priority_Node *node = _Scheduler_priority_Node_get( the_thread );

  _Scheduler_priority_Ready_queue_enqueue(
    the_thread,
    &node->Ready_queue,
    &context->Bit_map
  );

  /* TODO: flash critical section? */

  /*
   *  If the thread that was unblocked is more important than the heir,
   *  then we have a new heir.  This may or may not result in a
   *  context switch.
   *
   *  Normal case:
   *    If the current thread is preemptible, then we need to do
   *    a context switch.
   *  Pseudo-ISR case:
   *    Even if the thread isn't preemptible, if the new heir is
   *    a pseudo-ISR system task, we need to do a context switch.
   */
  if ( the_thread->current_priority < _Thread_Heir->current_priority ) {
    _Thread_Heir = the_thread;
    if ( _Thread_Executing->is_preemptible ||
        the_thread->current_priority == 0 )
      _Thread_Dispatch_necessary = true;
  }
}
