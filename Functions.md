# XMITMSGX Functions

The central function in the library is `xmmake()`.
Other functions are derivatives for convenience and standardization.

## Routines in the Native Library

The native interface is defined in C terms.

* int xmopen(,,)

Open the messages file, read it, get ready for service.
Returns: zero upon successful operation

`xmopen` takes the following arguments

filename: a pointer to a character string <br/>
options: a bitmask as an integer <br/>
msgstruct: a pointer to MSGSTRUCT or NULL (using NULL is not thread safe)

You should supply a MSGSTRUCT to represent your message repository.
If you supply NULL then xmopen() will attempt to use a global static
MSGSTRUCT, of which there is only one. (This is not thread safe.)

* int xmmake()

This is the central function: make a message.
All other print, string, and write functions are derivatives.
Returns: zero upon successful operation

`xmmake` takes the following argument

msgstruct: a pointer to MSGSTRUCT

While some functions will accept NULL for the MSGSTRUCT pointer,
xmmake() does not.

You must provide xmmake() with a buffer to hold the message, member
`msgbuf`, and its length, member `msglen`. The latter will be replaced
with the size of the resulting message.

* int xmprint(,,,,)

Print a message, stdout or stderr depending on level/letter.
Newline automatically appended. Optionally SYSLOG the message.
Returns: number of characters printed, negative indicates error
Return value does not reflect SYSLOG effects or errors.

`xmprint` takes the following arguments

message number <br/>
count of replacement tokens (plus one for the reserved token) <br/>
array of replacement tokens (token zero is reserved) <br/>
options: a bitmask as an integer <br/>
msgstruct: a pointer to MSGSTRUCT or NULL (NULL is not thread safe)

* int xmwrite(,,,,,)

Write a message to the indicated file descriptor.
Newline automatically appended. Optionally SYSLOG the message.
Returns: number of bytes written, negative indicates error
Return value does not reflect SYSLOG effects or errors.

`xmwrite` takes the following arguments

file descriptor <br/>
message number <br/>
count of replacement tokens (plus one for the reserved token) <br/>
array of replacement tokens (token zero is reserved) <br/>
options: a bitmask as an integer <br/>
msgstruct: a pointer to MSGSTRUCT or NULL (NULL is not thread safe)

* int xmstring(,,,,,)

Build the message and put it into a string buffer. No newline.
Returns: number of bytes in string, negative indicates error

`xmstring` takes the following arguments

pointer to buffer to hold resulting message string <br/>
size of output buffer <br/>
message number <br/>
count of replacement tokens (plus one for the reserved token) <br/>
array of replacement tokens (token zero is reserved) <br/>
msgstruct: a pointer to MSGSTRUCT or NULL (NULL is not thread safe)

* int xmclose()

Close (figuratively): free common storage and reset static variables.

Returns: zero upon successful operation

`xmclose` takes the following argument

msgstruct: a pointer to MSGSTRUCT or NULL (NULL is not thread safe)

## Routines in the Rexx Interface

All Rexx calls return a condition code and an optional result string.
This is similar to the style of Rexx/Sockets.

Functions in the Rexx interface are:

* `RxXmitmsgX("VERSION")`

* `RxXmitmsgX("INIT",library,options)`

* `RxXmitmsgX("PRINT",messageid,arg1,arg2,arg3,...)`

* `RxXmitmsgX("STRING",messageid,arg1,arg2,arg3,...)`

* `RxXmitmsgX("QUIT",libobj)`

Logically, there is only one Rexx function defined in the interface.
All Message Handler operations are driven as sub-functions of the one.

The `VERSION` subfunction can be called before the message library
is opened. (That is, before `INIT` is called.)

Replacement tokens begin at 1.
There is no reserved "token zero" like there is with the native interface.

## Methods in the Java Interface

All methods in the Java interface are defined as returning a string,
but for most the string is empty and should be ignored.

Methods in the Java interface are:

* method `MessageService.version()`

* method `MessageService.init(library,options)`

* method `MessageService.print(messageid,argarray)`

* method `MessageService.string(messageid,argarray)`

* method `MessageService.quit(libobj)`

The `version()` method can be called before the message library
is opened. (That is, before `init()` is called.)

Replacement tokens begin at 1.
There is no reserved "token zero" like there is with the native interface.

All replacement tokens are passed in a string array.


