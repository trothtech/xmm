# xmm

XMITMSG for POSIX systems

## xmitmsgx

`xmitmsgx` (or simply `xmm`) is a library and `xmitmsg` is program
for presenting enumerated messages. The command works similarly to
that of IBM VM/CMS 'XMITMSG' but with Unix syntax (dashed options
before positional arguments) rather than CMS syntax (positional args,
open paren, then options). The message file source format is the same
as that of CMS except it does not have to be pre-processed.

## Rationale

Application messages in context can be referenced by number.
The text of the message can be made available in any local language.
Tokens, for lack of a better term, can be inserted into the message
with correct placement.

This is suitable for ...

* error messages
* non-error status messages
* logging, reporting
* decorations: button labels, title bars, field identifiers

 ... anything textual intended for human consumption.

## Example

For an example from actual traffic in the FUZIX project,
consider both English and Spanish for a failing `make` target:

    fallo en las instrucciones para el objetivo 'stty.rel'

    recipe for target 'stty.rel' failed

Here, "stty.rel" varies so is represented by a token.
It also moves within the message to accommodate the language.
So message 1234 would be coded in the message file as:

    1234    E fallo en las instrucciones para el objetivo '&1'

    1234    E recipe for target '&1' failed

"E" because this is an error condition.
Using an application ID of "MAK" and a caller tag representing
the recipe processor "RCP", the resulting message might look like:

    MAKRCP1234E fallo en las instrucciones para el objetivo 'stty.rel'

    MAKRCP1234E recipe for target 'stty.rel' failed

The message code MAKRCP1234E can be used for reference
even if the local language is neither English nor Spanish.


