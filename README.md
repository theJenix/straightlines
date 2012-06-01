This project contains workarounds for various Parallels 7 curiousities that I
encountered while trying to run Linux on my MacBook Pro 15" (late 2011).

Curiousity: 

(copied from the Paralles support thread:
http://forum.parallels.com/showthread.php?p=626072#post626072)

"
The title says it all. Occasionally in Coherence mode, my guests will start
receiving keyboard input that I'm typing into applications running under OS X. A
temporary fix seems to be exiting re-entering Coherence mode, but the problem
will just happen again later. I tried searching for this issue, but all I could
find were other people having this same problem, with no solution (or even
official response) on the issue. Anybody else having this problem? It's driving
me nuts. I'm using Parallels Desktop 7 under Lion.
I'm having this same issue...it's very random, very hard to reproduce. I did a
bit of investigation and have come up with the following diagnosis: It appears
that the problem stems from Parallels not properly releasing the keyboard and
mouse. You can test this theory on your machine by pressing your control release
keystroke the next time this happens...for me, that stopped Parallels from
capturing keyboard input until it was focused again. Unfortunately, after the
next time it was focus, the problem would reappear until I exited and re-entered
Coherence mode. All in all, a total pain in the butt.
"

Workaround: 

This application watches the active window and sends
the control release command when it senses Parallels has lost focus. A couple
caveats:
- Right now, it assumes Ctrl+Alt is the control release command. If yours is
  different, you'll have to change that in the code.
- It sends the keystrokes to all windows (i.e. as if it was typed on the
  keyboard). With the default command, this should not be that big a deal as
most programs require a letter/number/ASCII key to perform action.

