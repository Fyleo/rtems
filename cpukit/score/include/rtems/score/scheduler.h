/**
 *  @file  rtems/score/scheduler.h
 *
 *  @brief Constants and Structures Associated with the Scheduler
 *
 *  This include file contains all the constants and structures associated
 *  with the scheduler.
 */

/*
 *  Copyright (C) 2010 Gedare Bloom.
 *  Copyright (C) 2011 On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_SCHEDULER_H
#define _RTEMS_SCORE_SCHEDULER_H

#include <rtems/score/percpu.h>
#include <rtems/score/priority.h>
#include <rtems/score/thread.h>
#if defined(__RTEMS_HAVE_SYS_CPUSET_H__) && defined(RTEMS_SMP)
  #include <sys/cpuset.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @defgroup ScoreScheduler Scheduler Handler
 *
 *  @ingroup Score
 *
 *  This handler encapsulates functionality related to managing sets of threads
 *  that are ready for execution.
 */
/**@{*/

typedef struct Scheduler_Control Scheduler_Control;

typedef struct Scheduler_Node Scheduler_Node;

/**
 * @brief The scheduler operations.
 */
typedef struct {
  /** @see _Scheduler_Handler_initialization() */
  void ( *initialize )( const Scheduler_Control * );

  /** @see _Scheduler_Schedule() */
  void ( *schedule )( const Scheduler_Control *, Thread_Control *);

  /** @see _Scheduler_Yield() */
  void ( *yield )( const Scheduler_Control *, Thread_Control *);

  /** @see _Scheduler_Block() */
  void ( *block )( const Scheduler_Control *, Thread_Control * );

  /** @see _Scheduler_Unblock() */
  void ( *unblock )( const Scheduler_Control *, Thread_Control * );

  /** @see _Scheduler_Change_priority() */
  void ( *change_priority )(
    const Scheduler_Control *,
    Thread_Control *,
    Priority_Control,
    bool
  );

  /** @see _Scheduler_Allocate() */
  bool ( *allocate )( const Scheduler_Control *, Thread_Control * );

  /** @see _Scheduler_Free() */
  void ( *free )( const Scheduler_Control *, Thread_Control * );

  /** @see _Scheduler_Update() */
  void ( *update )( const Scheduler_Control *, Thread_Control * );

  /** @see _Scheduler_Priority_compare() */
  int ( *priority_compare )(
    Priority_Control,
    Priority_Control
  );

  /** @see _Scheduler_Release_job() */
  void ( *release_job ) (
    const Scheduler_Control *,
    Thread_Control *,
    uint32_t
  );

  /** @see _Scheduler_Tick() */
  void ( *tick )( const Scheduler_Control *, Thread_Control * );

  /** @see _Scheduler_Start_idle() */
  void ( *start_idle )(
    const Scheduler_Control *,
    Thread_Control *,
    Per_CPU_Control *
  );

#if defined(__RTEMS_HAVE_SYS_CPUSET_H__) && defined(RTEMS_SMP)
  /** @see _Scheduler_Get_affinity() */
  bool ( *get_affinity )(
    const Scheduler_Control *,
    Thread_Control *,
    size_t,
    cpu_set_t *
  );
  
  /** @see _Scheduler_Set_affinity() */
  bool ( *set_affinity )(
    const Scheduler_Control *,
    Thread_Control *,
    size_t,
    const cpu_set_t *
  );
#endif
} Scheduler_Operations;

/**
 * @brief Scheduler context.
 *
 * The scheduler context of a particular scheduler implementation must place
 * this structure at the begin of its context structure.
 */
typedef struct Scheduler_Context {
#if defined(RTEMS_SMP)
  /**
   * @brief Count of processors owned by this scheduler instance.
   */
  uint32_t processor_count;
#endif
} Scheduler_Context;

/**
 * @brief Scheduler control.
 */
struct Scheduler_Control {
  /**
   * @brief Reference to a statically allocated scheduler context.
   */
  Scheduler_Context *context;

  /**
   * @brief The scheduler operations.
   */
  Scheduler_Operations Operations;

  /**
   * @brief The scheduler name.
   */
  uint32_t name;
};

/**
 * @brief Scheduler node for per-thread data.
 */
struct Scheduler_Node {
  /* No fields yet */
};

/**
 * @brief Registered schedulers.
 *
 * Application provided via <rtems/confdefs.h>.
 *
 * @see _Scheduler_Count.
 */
extern const Scheduler_Control _Scheduler_Table[];

/**
 * @brief Count of registered schedulers.
 *
 * Application provided via <rtems/confdefs.h> on SMP configurations.
 *
 * It is very important that this is a compile-time constant on uni-processor
 * configurations (in this case RTEMS_SMP is not defined) so that the compiler
 * can optimize the some loops away
 *
 * @see _Scheduler_Table.
 */
#if defined(RTEMS_SMP)
  extern const size_t _Scheduler_Count;
#else
  #define _Scheduler_Count ( (size_t) 1 )
#endif

#if defined(RTEMS_SMP)
  /**
   * @brief The scheduler assignment default attributes.
   */
  #define SCHEDULER_ASSIGN_DEFAULT UINT32_C(0x0)

  /**
   * @brief The presence of this processor is optional.
   */
  #define SCHEDULER_ASSIGN_PROCESSOR_OPTIONAL SCHEDULER_ASSIGN_DEFAULT

  /**
   * @brief The presence of this processor is mandatory.
   */
  #define SCHEDULER_ASSIGN_PROCESSOR_MANDATORY UINT32_C(0x1)

  /**
   * @brief Scheduler assignment.
   */
  typedef struct {
    /**
     * @brief The scheduler for this processor.
     */
    const Scheduler_Control *scheduler;

    /**
     * @brief The scheduler assignment attributes.
     *
     * Use @ref SCHEDULER_ASSIGN_DEFAULT to select default attributes.
     *
     * The presence of a processor can be
     * - @ref SCHEDULER_ASSIGN_PROCESSOR_OPTIONAL, or
     * - @ref SCHEDULER_ASSIGN_PROCESSOR_MANDATORY.
     */
    uint32_t attributes;
  } Scheduler_Assignment;

  /**
   * @brief The scheduler assignments.
   *
   * The length of this array must be equal to the maximum processors.
   *
   * Application provided via <rtems/confdefs.h>.
   *
   * @see _Scheduler_Table and rtems_configuration_get_maximum_processors().
   */
  extern const Scheduler_Assignment _Scheduler_Assignments[];
#endif

/**
 * @brief Does nothing.
 *
 * @param[in] scheduler Unused.
 * @param[in] the_thread Unused.
 */
void _Scheduler_default_Schedule(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 * @brief Returns true.
 *
 * @param[in] scheduler Unused.
 * @param[in] the_thread Unused.
 *
 * @retval true Always.
 */
bool _Scheduler_default_Allocate(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 * @brief Does nothing.
 *
 * @param[in] scheduler Unused.
 * @param[in] the_thread Unused.
 */
void _Scheduler_default_Free(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 * @brief Does nothing.
 *
 * @param[in] scheduler Unused.
 * @param[in] the_thread Unused.
 */
void _Scheduler_default_Update(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 * @brief Does nothing.
 *
 * @param[in] scheduler Unused.
 * @param[in] the_thread Unused.
 * @param[in] deadline Unused.
 */
void _Scheduler_default_Release_job(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  uint32_t                 deadline
);

/**
 * @brief Performs tick operations depending on the CPU budget algorithm for
 * each executing thread.
 *
 * This routine is invoked as part of processing each clock tick.
 *
 * @param[in] scheduler The scheduler.
 * @param[in] execution An executing thread.
 */
void _Scheduler_default_Tick(
  const Scheduler_Control *scheduler,
  Thread_Control          *executing
);

/**
 * @brief Starts an idle thread.
 *
 * @param[in] scheduler The scheduler.
 * @param[in] the_thread An idle thread.
 * @param[in] cpu This parameter is unused.
 */
void _Scheduler_default_Start_idle(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Per_CPU_Control         *cpu
);

#if defined(__RTEMS_HAVE_SYS_CPUSET_H__) && defined(RTEMS_SMP)
  /**
   * @brief Get affinity for the default scheduler.
   *
   * @param[in] thread The associated thread.
   * @param[in] cpusetsize The size of the cpuset.
   * @param[out] cpuset Affinity set containing all CPUs.
   *
   * @retval 0 Successfully got cpuset
   * @retval -1 The cpusetsize is invalid for the system
   */
  bool _Scheduler_default_Get_affinity(
    const Scheduler_Control *scheduler,
    Thread_Control          *thread,
    size_t                   cpusetsize,
    cpu_set_t               *cpuset
  );

  /** 
   * @brief Set affinity for the default scheduler.
   *
   * @param[in] thread The associated thread.
   * @param[in] cpusetsize The size of the cpuset.
   * @param[in] cpuset Affinity new affinity set.
   *
   * @retval 0 Successful
   *
   *  This method always returns successful and does not save
   *  the cpuset.
   */
  bool _Scheduler_default_Set_affinity(
    const Scheduler_Control *scheduler,
    Thread_Control          *thread,
    size_t                   cpusetsize,
    const cpu_set_t         *cpuset
  );
#endif

/**
 * @brief Indicates if thread priority queues are broken with the configured
 * scheduler or not.
 *
 * See also PR2174: Memory corruption with EDF scheduler and thread priority
 * queues.
 */
extern const bool _Scheduler_FIXME_thread_priority_queues_are_broken;

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
