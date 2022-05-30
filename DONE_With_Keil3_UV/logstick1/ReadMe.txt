From LogStick Blog postings: Pushing a button and Pushing my button
The ability to waggle a pin low and high via a mouse click in the debugger
and read its state in a program may seem somewhat unexciting, but it's a
start for me. Looking at some of the other examples in the RealView
Microcontroller Development Kit, I note that there are several that use buttons
=hat appear in a window called the "Toolbox". A button that I can push to waggle
the pin is exactly what I am looking for.

The Traffic example contains a button that allows me to simulate the 
"Push to walk" button at a crosswalk. Whenever I start the debugger and
simulation for this example some commands and C-like code get loaded into the
debugger at startup. These are shown scrolling past on the debugger's output
window. But where do these commands come from?  A quick grep of the Traffic 
example's directory for one of the strings that scrolled past and I find that
the Traffic example has a .ini file that contains the debugger commands. 
Interesting...

Digging a little further, I find that the project options allow me to specify
that a .ini file is executed when the debugger session commences. This is neat
as it enables me to consistently set up my debug world with breakpoints, 
watchpoints, etc. whenever I start debugging. Moreover, it enables me to 
define buttons!

The .ini file for Traffic looks like this: 

PORT0 |= 0x4000;          /* set P0.14 low: Key Input     */ // Hmmm... I think they mean high!

/* define a debug function for the pedestrian push button */ 
signal void push_key (void)  {
  PORT0 &= ~0x004000;       /* set P0.14 low  */
  swatch (0.05);            /* wait 50 msec   */
  PORT0 |= 0x004000;        /* set P0.14 high */
}

/* define a toolbar button to call push_key */ 
define button "Push for Walk", "push_key ()"
...

It seems I can waggle the pins using the PORT0 variable in much the same
way as I can waggle the GPIO under program control. Looking in the manual, 
the keyword "signal" hooks the function that succeeds it into the simulator
so that it runs alongside the simulation. The manual says there's a bunch 
of stuff I can do with this to simulate signals coming in with specific 
intervals. The swatch and twatch functions can pause a running signal 
function for seconds or CPU states respectively. 

So the signal function above sets pin P0.14 low, lets the simulation run
on for a simulated 50 milliseconds and then takes PO.14 high. 

A quick hack of this code and I get a button for my code:

PORT0 |= 0x00000001;		/* set P0:1 low: Key Input     */

/* define a debug function for the push button */ 
signal void push_button (void)  {
  PORT0 &= ~0x00000001;		/* set P0:1 low  */
  swatch (0.05);			/* wait 50 msec   */
  PORT0 |= 0x00000001;		/* set P0:1 high */
}

/* define a toolbar button to call push_button */ 
define button "Push my button", "push_button ()"
