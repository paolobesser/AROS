/*
    $Id$
    $Log$
    Revision 1.1  1996/07/28 16:37:24  digulla
    Initial revision

    Desc:
    Lang: english
*/
#include "exec_intern.h"
#include "semaphores.h"

/*****************************************************************************

    NAME */
	#include <exec/semaphores.h>
	#include <clib/exec_protos.h>

	__AROS_LH1(void, ObtainSemaphoreList,

/*  SYNOPSIS */
	__AROS_LA(struct List *, sigSem, A0),

/*  LOCATION */
	struct ExecBase *, SysBase, 97, Exec)

/*  FUNCTION
	This function obtains all semaphores in the list at once.
	Note that this doesn't include arbitration for the list as
	a whole - you will have to arbitrate for the whole list yourself.

    INPUTS
	sigSem - pointer to list full of semaphores

    RESULT

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

    HISTORY

*****************************************************************************/
{
    __AROS_FUNC_INIT
    struct Node *n;

    /*
	There's no arbitration needed - the first semaphore in the list
	list arbitrates for the full list.
	Get first element in the list.
    */
    n=sigSem->lh_Head;

    /* And follow it. */
    while(n->ln_Succ!=NULL)
    {
	/* Free the semaphore */
	ReleaseSemaphore((struct SignalSemaphore *)n);

	/* Get next element */
	n=n->ln_Succ;
    }
    __AROS_FUNC_EXIT
} /* ReleaseSemaphoreList */
