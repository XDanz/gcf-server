//
// Created by danter on 2017-01-14.
//


#include <sys/poll.h>
#include "Pratical.h"

int		daemon_proc;		/* set nonzero by daemon_init() */

static void	err_doit(int, int, const char *, va_list);

void
Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
        err_sys("bind error");
}

/* include Socket */
int
Socket(int family, int type, int protocol)
{
    int		n;

    if ( (n = socket(family, type, protocol)) < 0)
        err_sys("socket error");
    return(n);
}
/* end Socket */

/* include Listen */
void
Listen(int fd, int backlog)
{
    char	*ptr;

    /*4can override 2nd argument with environment variable */
    if ( (ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);

    if (listen(fd, backlog) < 0)
        err_sys("listen error");
}
/* end Listen */

int
Poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
    int		n;

    if ( (n = poll(fdarray, nfds, timeout)) < 0)
        err_sys("poll error");

    return(n);
}

void
Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close error");
}

void
Write(int fd, void *ptr, size_t nbytes)
{
    if (write(fd, ptr, nbytes) != nbytes)
        err_sys("write error");
}

//* Fatal error related to system call
/* Print message and terminate */

void
err_sys(const char *fmt, ...)
{
    va_list		ap;

    va_start(ap, fmt);
    err_doit(1, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

/* Print message and terminate */

void
err_quit(const char *fmt, ...)
{
    va_list		ap;

    va_start(ap, fmt);
    err_doit(0, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}



/* Print message and return to caller
 * Caller specifies "errnoflag" and "level" */

static void
err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
    int		errno_save, n;
    char	buf[MAXLINE + 1];

    errno_save = errno;		/* value caller might want printed */
#ifdef	HAVE_VSNPRINTF
    vsnprintf(buf, MAXLINE, fmt, ap);	/* safe */
#else
    vsprintf(buf, fmt, ap);					/* not safe */
#endif
    n = strlen(buf);
    if (errnoflag)
        snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
    strcat(buf, "\n");

    if (daemon_proc) {
        syslog(level, buf);
    } else {
        fflush(stdout);		/* in case stdout and stderr are the same */
        fputs(buf, stderr);
        fflush(stderr);
    }
    return;
}
int Accept(int fd, struct sockaddr* sa, socklen_t* salenptr)
{
    int		n;

    again:
    if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef	EPROTO
        if (errno == EPROTO || errno == ECONNABORTED)
#else
            if (errno == ECONNABORTED)
#endif
            goto again;
        else
            err_sys("accept error");
    }
    return(n);
}
