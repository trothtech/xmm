# SYSLOG Correlation

CMS 'XMITMSG' and in turn this "xmitmsgx" project (henceforth "xmm")
uses severity levels. These align nicely with SYSLOG priorities.

## IBM versus Unix

IBM has always done a rigorous job of tying system conditions and events
with their printed documentation. (Consider the sometime popular monicker
"I Bring Manuals".) It is ironic that their better known systems (e.g.,
MVS, now known as z/OS) do not have an 'XMITMSG' but still demonstrate
disciplined message references.

Messages consist of an alphanumeric tag, then the unique message number,
*then the severity level*, and finally the human readable message.
Severity levels are sufficiently coarse grained that IT staff can
quickly know what class of response or action to engage, while the
message number and combined prefix/tag provide direct reference to
detailed documentation.

Here is a correlation between IBM severities and Unix SYSLOG levels.

    Code    Type            mnemonic        priority
    -       -               LOG_DEBUG       7 = debugging
    I       Information     LOG_INFO        6 = general info
    R       Response        LOG_NOTICE      5 = requires special handling (response needed)
    W       Warning         LOG_WARNING     4 = a warning
    E       Error           LOG_ERR         3 = an error
    S       Sever           LOG_CRIT        2 = a critical condition
    T       Terminal        LOG_ALERT       1 = requires immediate action, a component or job has stopped
    -       -               LOG_EMERG       0 = panic (or "emergency", catastrophic failure)

## SYSLOG

The `xmm` project library can optionally drive `syslog()`.
When calling `syslog()`, the library determines the SYSLOG priority
from the severity level of the message.

SYSLOG priorities are listed in the table above.
SYSLOG facilities do not necessarily align and can be specified
when initializing the `xmm` library.

## References

/usr/include/sys/syslog.h


