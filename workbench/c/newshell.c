#include <exec/memory.h>
#include <clib/exec_protos.h>
#include <dos/dosextens.h>
#include <dos/dostags.h>
#include <clib/dos_protos.h>
#include <utility/tagitem.h>

CALLENTRY /* Before the first symbol */

struct ExecBase *SysBase;
struct DosLibrary *DOSBase;

static LONG tinymain(void);

LONG entry(struct ExecBase *sysbase)
{
    LONG error=RETURN_FAIL;
    SysBase=sysbase;
    DOSBase=(struct DosLibrary *)OpenLibrary("dos.library",39);
    if(DOSBase!=NULL)
    {
	error=tinymain();
	CloseLibrary((struct Library *)DOSBase);
    }
    return error;
}

static LONG tinymain(void)
{
    STRPTR args[2]={ "CON:", "S:Shell-Startup" };
    struct RDArgs *rda;
    BPTR lock, in, out, shell;
    STRPTR s1, s2, s3, buf;
    struct Process *process;
    LONG error=RETURN_ERROR;
    
    rda=ReadArgs("WINDOW,FROM",(ULONG *)args,NULL);
    if(rda!=NULL)
    {
	s1=s2=(STRPTR)args[1];
	while(*s2++)
	    ;
	buf=(STRPTR)AllocVec(6+2*(s2-s1),MEMF_ANY);
	if(buf!=NULL)
	{
	    CopyMem("FROM ",buf,5);
	    s3=buf+5;
	    s2=s1;
	    *s3++='\"';
	    while(*s1)
	    {
		if(*s1=='*'||*s1=='\"'||*s1=='\n')
		    *s3++='*';
		if(*s1=='\n')
		    *s3++='n';
		else
		    *s3++=*s1;
		s1++;
	    }
	    *s3++='\"';
	    *s3=0;
	
	    shell=LoadSeg("c:shell");
	    if(shell)
	    {
		out=Open(args[0],MODE_READWRITE);
		if(out)
		{
		    /* Clone output filehandle */
		    lock=DupLockFromFH(out);
		    if(lock)
		    {
		        in=OpenFromLock(lock);
		        if(!in)
		            UnLock(lock);
		    }else
		        in=0;
		    if(in)
		    {
			struct TagItem tags[]=
			{
			    { NP_Arguments, 0 },
			    { NP_Input, 0 },
			    { NP_Output, 0 },
			    { NP_Error, 0 },
			    { NP_Seglist, 0 },
			    { NP_Cli, 1 },
			    { TAG_END, 0 }
			};
			tags[0].ti_Data=(LONG)buf;
			tags[1].ti_Data=in;
			tags[2].ti_Data=out;
			tags[4].ti_Data=shell;
			process=CreateNewProc(tags);
			if(process!=NULL)
			{
			    out=in=shell=0;
			    error=0;
			}
			Close(in);
		    }
		    Close(out);
		}
		UnLoadSeg(shell);
	    }
	    FreeVec(buf);
	}
	FreeArgs(rda);
    }else
	error=RETURN_FAIL;
    if(error)
	PrintFault(IoErr(),"NewShell");
    return error;
}
