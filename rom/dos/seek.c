/*
    $Id$
    $Log$
    Revision 1.1  1996/07/28 16:37:23  digulla
    Initial revision

    Desc:
    Lang: english
*/
#include <clib/exec_protos.h>
#include <dos/dosextens.h>
#include <dos/filesystem.h>

/*****************************************************************************

    NAME */
	#include <clib/dos_protos.h>

	__AROS_LH3(LONG, Seek,

/*  SYNOPSIS */
	__AROS_LA(BPTR, file,     D1),
	__AROS_LA(LONG, position, D2),
	__AROS_LA(LONG, mode,     D3),

/*  LOCATION */
	struct DosLibrary *, DOSBase, 11, Dos)

/*  FUNCTION
	Changes the actual read/write position in a file and/or
	reads the actual position, e.g to get the actual position
	do a Seek(file,0,OFFSET_CURRENT).

	This function may fail (obviously) on certain devices such
	as pipes or console handlers.

    INPUTS
	file     - filehandle
	position - relative offset in bytes (positive, negative or 0).
	mode     - Where to count from. Either OFFSET_BEGINNING,
		   OFFSET_CURRENT or OFFSET_END.

    RESULT
	Absolute position in bytes before the Seek(), -1 if an error
	happened. IoErr() will give additional information in that case.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

    HISTORY
	29-10-95    digulla automatically created from
			    dos_lib.fd and clib/dos_protos.h

*****************************************************************************/
{
    __AROS_FUNC_INIT
    __AROS_BASE_EXT_DECL(struct DosLibrary *,DOSBase)

    /* Get pointer to filehandle */
    struct FileHandle *fh=(struct FileHandle *)BADDR(file);

    /* Get pointer to process structure */
    struct Process *me=(struct Process *)FindTask(NULL);

    /* Get pointer to I/O request. Use stackspace for now. */
    struct IOFileSys io,*iofs=&io;

    /* Prepare I/O request. */
    iofs->IOFS.io_Message.mn_Node.ln_Type=NT_REPLYMSG;
    iofs->IOFS.io_Message.mn_ReplyPort   =&me->pr_MsgPort;
    iofs->IOFS.io_Message.mn_Length      =sizeof(struct IOFileSys);
    iofs->IOFS.io_Device =fh->fh_Device;
    iofs->IOFS.io_Unit   =fh->fh_Unit;
    iofs->IOFS.io_Command=FSA_SEEK;
    iofs->IOFS.io_Flags  =0;
    iofs->io_Args[0]=position<0?-1:0;
    iofs->io_Args[1]=position;
    iofs->io_Args[2]=mode;

    /* Send the request. */
    DoIO(&iofs->IOFS);

    /* return */    
    if((me->pr_Result2=iofs->io_DosError))
	return -1;
    else
	return iofs->io_Args[1];
    __AROS_FUNC_EXIT
} /* Seek */
