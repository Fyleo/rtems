/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <tmacros.h>
#include "test_support.h"
#include <rtems/cpuuse.h>

const char rtems_test_name[] = "SMP 9";

volatile int killtime;

rtems_task Test_task(
  rtems_task_argument argument
)
{
  while (1)
    ;
  /* XXX something goes here */
}

rtems_task Init(
  rtems_task_argument argument
)
{
  uint32_t           i;
  char               ch;
  uint32_t           cpu_num;
  rtems_id           id;
  rtems_status_code  status;

  TEST_BEGIN();

  locked_print_initialize();

  for ( killtime=0; killtime<1000000; killtime++ )
    ;
  
  for ( i=0; i<rtems_get_processor_count() -1; i++ ) {
    ch = '1' + i;

    status = rtems_task_create(
      rtems_build_name( 'T', 'A', ch, ' ' ),
      1,
      RTEMS_MINIMUM_STACK_SIZE,
      RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES,
      &id
    );
    directive_failed( status, "task create" );

    cpu_num = rtems_get_current_processor();
    locked_printf(" CPU %" PRIu32 " start task TA%c\n", cpu_num, ch);

    status = rtems_task_start( id, Test_task, i+1 );
    directive_failed( status, "task start" );
  }

  locked_printf(" kill 10 clock ticks\n" );
  while ( rtems_clock_get_ticks_since_boot() < 10 )
    ;

  rtems_cpu_usage_report();

  TEST_END();
  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_SMP_APPLICATION
#define CONFIGURE_SMP_MAXIMUM_PROCESSORS   4

#define CONFIGURE_MAXIMUM_TASKS            \
    (1 + CONFIGURE_SMP_MAXIMUM_PROCESSORS)
#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_SEMAPHORES 1

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
/* end of file */
