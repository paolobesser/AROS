/*
    $Id$
    $Log$
    Revision 1.1  1996/07/28 16:37:24  digulla
    Initial revision

    Desc:
    Lang: english
*/
#include <exec/ports.h>
#include <aros/libcall.h>

/*****************************************************************************

    NAME */
	#include <clib/exec_protos.h>

	__AROS_LH1(struct Message *, GetMsg,

/*  SYNOPSIS */
	__AROS_LA(struct MsgPort *, port, A0),

/*  LOCATION */
	struct ExecBase *, SysBase, 62, Exec)

/*  FUNCTION
	Get a message from a given messageport. This function doesn't wait
	and returns NULL if the messageport is empty. Therefore it's
	generally a good idea to WaitPort() or Wait() on the given port first.

    INPUTS
	port - Pointer to messageport

    RESULT
	Pointer to message removed from the port.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
	WaitPort(), PutMsg()

    INTERNALS

    HISTORY

******************************************************************************/
{
    __AROS_FUNC_INIT

    struct Message *msg;

    /* Protect the message list. */
    Disable();

    /* Get first node. */
    msg=(struct Message *)RemHead(&port->mp_MsgList);

    /* All done. */
    Enable();
    return msg;
    __AROS_FUNC_EXIT
}

