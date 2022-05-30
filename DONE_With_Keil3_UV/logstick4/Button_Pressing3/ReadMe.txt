This is the fully working button interrupt example.

Pin 1 of GPIO 0 is used to trigger external interrupt 0.
The external interrupt is set to invoke the interrupt service
routine in slot0 of the Vectored Interrupt Controller.
This routine increments a counter showing how many times
it has been invoked.

The interrupt is triggered on edge to enure that one 
interrupt is taken for one button press.

The foreground program waggles pin 1 of GPIO 1 up an down
to provide visual representation that the program is running
and all is well.

When run using the simulation environment in the RealView
Microcontroller Development Kit a button is added to the 
toolbox which can be used to take Pin 1 low and trigger
the interrupt.