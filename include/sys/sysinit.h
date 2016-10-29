#ifndef _SYS_SYSINIT_H_
#define _SYS_SYSINIT_H_

#include <sys/linkerset.h>
#include <stdbool.h>
#include <stdint.h>

/* Initialization modules. A given initialization step can be in exactly
 * one module and can depend on any other modules (assuming no cyclic
 * dependencies arise).
 *
 * Order is not important here, as long as SYSINIT_EARLY is at index 0
 * and SYSINIT_LATE is at index SYSINIT_NUM_MODULES - 1.
 * */
#define SYSINIT_EARLY   (1 << 0)
#define SYSINIT_LATE    (1 << 1)
#define SYSINIT_NUM_MODULES 2

/* A single initialization step.
 *
 * Initialization steps are added to the SYSINIT_LINKERSET, and will
 * be executed in sequence once sys_init() is called.
 *
 * The order of the steps will be determined by the dependencies of
 * each step. Any given step declares its dependencies in the
 * depends_mask, and its module in the module_mask.
 *
 * If a step depends on a given module, then it cannot be executed
 * before all steps in that module are complete.
 *
 * There is no order guarantee for modules in the same step.
 *
 * Every step is considered to implicitly depend on the
 * SYSINIT_EARLY step, and so that will be executed first. The
 * SYSINIT_LATE step is considered to implicitly depend on all steps, so
 * will be executed last.  */
typedef struct init_step {
        uint64_t module_mask; // Which module the step is a part of
        uint64_t depends_mask; // Which module the step requires.
        const char *name; // A name to print out for the step
        int (*step)(void); // The actual step to execute
        bool warn_on_fail; // Whether the step can fail without a panic
        struct init_step *_nextptr; // Internal
} init_step_t;

/* Internal only. */
#define _SYSINIT_STEP(name, step, module, depends_mask, warn_on_fail) \
        init_step_t _sysinit_step_##step = {\
                (module), \
                (depends_mask), \
                (name), \
                (step), \
                (warn_on_fail), \
                NULL, \
        }; \
        LINKERSET_ENTRY(SYSINIT_LINKERSET, _sysinit_step_##step)

/* Add a new sysinit step with the given properties. The step cannot
 * fail and the system will panic() if it does. */
#define SYSINIT_STEP(name, step, module, depends_mask) \
        _SYSINIT_STEP(name, step, module, depends_mask, false)

/* Add a new sysinit step with the given properties. The step is allowed
 * to fail and the system will display a warning if it does. */
#define SYSINIT_STEP_CANFAIL(name, step, module, depends_mask) \
        _SYSINIT_STEP(name, step, module, depends_mask, true)

/* Run the startup steps. Returns 1 if some of the init steps failed
 * (but were designed as `warn_on_fail'), and 0 otherwise. */
int sys_init(void);

#endif
