/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.6-dev */

#ifndef PB_SINKMODEL_PB_H_INCLUDED
#define PB_SINKMODEL_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _PfylTaskState_EnumPfylTaskState { 
    PfylTaskState_EnumPfylTaskState_ACTIVATED = 0, 
    PfylTaskState_EnumPfylTaskState_STOPPED = 1 
} PfylTaskState_EnumPfylTaskState;

/* Struct definitions */
typedef struct _PfylTaskCreated { 
    uint64_t tick; 
    char taskName[32]; 
} PfylTaskCreated;

typedef struct _PfylTaskDeleted { 
    uint64_t tick; 
    char taskName[32]; 
} PfylTaskDeleted;

typedef struct _PfylTaskState { 
    uint64_t tick; 
    char taskName[32]; 
} PfylTaskState;


/* Helper constants for enums */
#define _PfylTaskState_EnumPfylTaskState_MIN PfylTaskState_EnumPfylTaskState_ACTIVATED
#define _PfylTaskState_EnumPfylTaskState_MAX PfylTaskState_EnumPfylTaskState_STOPPED
#define _PfylTaskState_EnumPfylTaskState_ARRAYSIZE ((PfylTaskState_EnumPfylTaskState)(PfylTaskState_EnumPfylTaskState_STOPPED+1))


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define PfylTaskCreated_init_default             {0, ""}
#define PfylTaskDeleted_init_default             {0, ""}
#define PfylTaskState_init_default               {0, ""}
#define PfylTaskCreated_init_zero                {0, ""}
#define PfylTaskDeleted_init_zero                {0, ""}
#define PfylTaskState_init_zero                  {0, ""}

/* Field tags (for use in manual encoding/decoding) */
#define PfylTaskCreated_tick_tag                 1
#define PfylTaskCreated_taskName_tag             2
#define PfylTaskDeleted_tick_tag                 1
#define PfylTaskDeleted_taskName_tag             2
#define PfylTaskState_tick_tag                   1
#define PfylTaskState_taskName_tag               2

/* Struct field encoding specification for nanopb */
#define PfylTaskCreated_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   tick,              1) \
X(a, STATIC,   SINGULAR, STRING,   taskName,          2)
#define PfylTaskCreated_CALLBACK NULL
#define PfylTaskCreated_DEFAULT NULL

#define PfylTaskDeleted_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   tick,              1) \
X(a, STATIC,   SINGULAR, STRING,   taskName,          2)
#define PfylTaskDeleted_CALLBACK NULL
#define PfylTaskDeleted_DEFAULT NULL

#define PfylTaskState_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   tick,              1) \
X(a, STATIC,   SINGULAR, STRING,   taskName,          2)
#define PfylTaskState_CALLBACK NULL
#define PfylTaskState_DEFAULT NULL

extern const pb_msgdesc_t PfylTaskCreated_msg;
extern const pb_msgdesc_t PfylTaskDeleted_msg;
extern const pb_msgdesc_t PfylTaskState_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define PfylTaskCreated_fields &PfylTaskCreated_msg
#define PfylTaskDeleted_fields &PfylTaskDeleted_msg
#define PfylTaskState_fields &PfylTaskState_msg

/* Maximum encoded size of messages (where known) */
#define PfylTaskCreated_size                     44
#define PfylTaskDeleted_size                     44
#define PfylTaskState_size                       44

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
