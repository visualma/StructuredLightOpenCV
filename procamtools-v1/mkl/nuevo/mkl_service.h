/*******************************************************************************
!                             INTEL CONFIDENTIAL
!   Copyright(C) 1999-2010 Intel Corporation. All Rights Reserved.
!   The source code contained  or  described herein and all documents related to
!   the source code ("Material") are owned by Intel Corporation or its suppliers
!   or licensors.  Title to the  Material remains with  Intel Corporation or its
!   suppliers and licensors. The Material contains trade secrets and proprietary
!   and  confidential  information of  Intel or its suppliers and licensors. The
!   Material  is  protected  by  worldwide  copyright  and trade secret laws and
!   treaty  provisions. No part of the Material may be used, copied, reproduced,
!   modified, published, uploaded, posted, transmitted, distributed or disclosed
!   in any way without Intel's prior express written permission.
!   No license  under any  patent, copyright, trade secret or other intellectual
!   property right is granted to or conferred upon you by disclosure or delivery
!   of the Materials,  either expressly, by implication, inducement, estoppel or
!   otherwise.  Any  license  under  such  intellectual property  rights must be
!   express and approved by Intel in writing.
!
!*******************************************************************************
!  Content:
!      Intel(R) Math Kernel Library (MKL) interface for service routines
!******************************************************************************/

#ifndef _MKL_SERVICE_H_
#define _MKL_SERVICE_H_

#include <stdlib.h>
#include "mkl_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void    MKLGetVersion(MKLVersion *ver);   /* Obsolete */
void    MKL_Get_Version(MKLVersion *ver); /* Returns information about the active version of the Intel MKL software */
#define mkl_get_version MKL_Get_Version

void    MKLGetVersionString(char * buffer, int len);    /* Obsolete */
void    MKL_Get_Version_String(char * buffer, int len); /* Returns a string that contains the library version information */
#define mkl_get_version_string MKL_Get_Version_String

void    MKL_FreeBuffers(void);  /* Obsolete */
void    MKL_Free_Buffers(void); /* Free the memory allocated by Memory Manager */
#define mkl_free_buffers MKL_Free_Buffers

void    MKL_Thread_Free_Buffers(void); /* Free the memory allocated by Memory Manager in the current thread only */
#define mkl_thread_free_buffers MKL_Thread_Free_Buffers

MKL_INT64 MKL_MemStat(int* nbuffers);  /* Obsolete */
MKL_INT64 MKL_Mem_Stat(int* nbuffers); /* MKL Memory Manager statistical information. */
                                       /* Returns an amount of memory, allocated by Memory Manager */
                                       /* in <nbuffers> buffers. */
#define mkl_mem_stat MKL_Mem_Stat

/* Note: the next 2 functions are NOT the staff of MKL Memory Manager */
void* MKL_malloc(size_t size, int align); /* Allocate the aligned buffer */
void  MKL_free(void *ptr);                /* Free the memory allocated by MKL_malloc() */
#define mkl_malloc MKL_malloc
#define mkl_free   MKL_free

void    getcpuclocks(unsigned MKL_INT64 *);       /* Obsolete: gets CPU clocks */
void    MKL_Get_Cpu_Clocks(unsigned MKL_INT64 *); /* gets CPU clocks */
#define mkl_get_cpu_clocks MKL_Get_Cpu_Clocks

double  getcpufrequency(void);       /* Obsolete */
double  MKL_Get_Cpu_Frequency(void); /* get CPU frequency in GHz */
#define mkl_get_cpu_frequency MKL_Get_Cpu_Frequency

void    setcpufrequency(double*);       /* Obsolete */
void    MKL_Set_Cpu_Frequency(double*); /* Set "MKL internal" CPU frequency in GHz */
#define mkl_set_cpu_frequency MKL_Set_Cpu_Frequency

void MKL_Set_Num_Threads(int nth);
int  MKL_Get_Max_Threads(void);
int  MKL_Domain_Set_Num_Threads(int nth, int MKL_DOMAIN);
int  MKL_Domain_Get_Max_Threads(int MKL_DOMAIN);
void MKL_Set_Dynamic(int bool_MKL_DYNAMIC);
int  MKL_Get_Dynamic(void);

#define mkl_set_num_threads         MKL_Set_Num_Threads
#define mkl_get_max_threads         MKL_Get_Max_Threads
#define mkl_domain_set_num_threads  MKL_Domain_Set_Num_Threads
#define mkl_domain_get_max_threads  MKL_Domain_Get_Max_Threads
#define mkl_set_dynamic             MKL_Set_Dynamic
#define mkl_get_dynamic             MKL_Get_Dynamic

/* MKL Progress routine */
#ifndef _MKL_PROGRESS_H_
#define _MKL_PROGRESS_H_
int MKL_PROGRESS( int* thread, int* step, char* stage, int lstage );
int MKL_PROGRESS_( int* thread, int* step, char* stage, int lstage );
int mkl_progress( int* thread, int* step, char* stage, int lstage );
int mkl_progress_( int* thread, int* step, char* stage, int lstage );
#endif /* _MKL_PROGRESS_H_ */

int MKL_Enable_Instructions(int);
#define  mkl_enable_instructions MKL_Enable_Instructions
#define  MKL_AVX_ENABLE 1
#define  MKL_SINGLE_PATH_ENABLE 0x0600

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MKL_SERVICE_H_ */
