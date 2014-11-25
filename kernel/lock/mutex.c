/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * Author: shang <shang@andrew.cmu.edu>
 *         jian wang <jianw3@andrew.cmu.edu>
 * @date
 */

//#define DEBUG_MUTEX
#include <assert.h>
#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif
#define NULL 0
mutex_t gtMutex[OS_NUM_MUTEX];

void mutex_init()
{
    //initialize all mutex
    int index;
    for(index =0;index<OS_NUM_MUTEX;index++){
        gtMutex[index].bLock = FALSE;
        gtMutex[index].pHolding_Tcb = NULL;
        gtMutex[index].bAvailable = TRUE;
        gtMutex[index].pSleep_queue = NULL;

    }
}

int mutex_create_swi(void)
{
   // printf("mutex create");
    disable_interrupts();
    //find the first available mutex
    int index = 0;
    int retVal = 0;
    while((gtMutex[index].bAvailable != TRUE) && (index < OS_NUM_MUTEX)){
        index++;
    }
    if(index == OS_NUM_MUTEX){
        enable_interrupts();
        retVal = ENOMEM;
    }
    else{
        gtMutex[index].bAvailable = FALSE;
        retVal = index;
    }
    enable_interrupts();
  //  printf("mutex create retVal %d\n",retVal);
	return retVal; // fix this to return the correct value
}

int mutex_lock(int mutex  __attribute__((unused)))
{
 //   printf("mutex lock %d\n",mutex);
    tcb_t *cur_tcb=NULL;
    tcb_t *cur_holding_tcb=NULL;
    disable_interrupts();
    if((mutex >= OS_NUM_MUTEX) || (mutex < 0)){
     //   printf("wrong mutex\n");
        enable_interrupts();
        return EINVAL;
    }
    cur_tcb = get_cur_tcb();
    if(gtMutex[mutex].bLock == TRUE){
        cur_holding_tcb = gtMutex[mutex].pHolding_Tcb;
        if(cur_tcb == cur_holding_tcb){
      //  printf("mutex lock twice %d\n",mutex);
            //a task can not lock a mutex twice
            enable_interrupts();
            return EINVAL;
        }else{
            //need to put the task to the sleep queue
            tcb_t *temp_tcb;
            cur_tcb->mutex_sleep_queue = NULL;
            cur_tcb->pending = 1;
            temp_tcb = gtMutex[mutex].pSleep_queue;
            if(!temp_tcb){
                //null queue
        //        printf("cur holding tcb %d, no sleeo queue\n",cur_tcb);
                gtMutex[mutex].pSleep_queue = cur_tcb;
            }else{
                while(temp_tcb->mutex_sleep_queue){
                    temp_tcb = temp_tcb->mutex_sleep_queue;
                }
       //         printf("cur holding tcb %d, have sleep queue, add cur tcb %d\ni", temp_tcb, cur_tcb);
                temp_tcb->mutex_sleep_queue = cur_tcb;
            }
        }
        dispatch_sleep();
    }else{
         gtMutex[mutex].bLock = TRUE;

    }
    cur_tcb->holds_lock++;
    gtMutex[mutex].pHolding_Tcb = cur_tcb;
    enable_interrupts();
    return 0;
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
    //    printf("mutex unlock %d\n",mutex);
    tcb_t *cur_tcb;
    tcb_t *holding_tcb;
    tcb_t *first_tcb_in_queue;
    disable_interrupts();
    if((mutex >= OS_NUM_MUTEX) || (mutex < 0)){
        enable_interrupts();
        return EINVAL;
    }
    cur_tcb = get_cur_tcb();
    holding_tcb = gtMutex[mutex].pHolding_Tcb;
    //sanity check
    printf("cur_tcb %d, holding tcb %d\n",cur_tcb,holding_tcb);
  //  printf("cur_tcb %d, holding tcb %d\n",cur_tcb,holding_tcb);
    if(holding_tcb == NULL){
        enable_interrupts();
        return EINVAL;
    }
    //check whether the cur tcb is the holding tch of this mutex
    if(cur_tcb != holding_tcb){
       enable_interrupts();
       return -EPERM;
    }
    // move the task from the sleep queue
    first_tcb_in_queue = gtMutex[mutex].pSleep_queue;
    if(first_tcb_in_queue){
    //    printf("cur prio %d, next in queue %d\n",cur_tcb->native_prio,first_tcb_in_queue->native_prio);
        //move this task to the run queue
        gtMutex[mutex].pSleep_queue = first_tcb_in_queue->mutex_sleep_queue;
        first_tcb_in_queue->mutex_sleep_queue = NULL;
        first_tcb_in_queue->pending = 0;
        runqueue_add(first_tcb_in_queue,first_tcb_in_queue->native_prio);
    }else{
        //no task is waiting for this mutex
        gtMutex[mutex].bLock = FALSE;
    }
    dispatch_save();
    enable_interrupts();
	return 0; // fix this to return the correct value
}
