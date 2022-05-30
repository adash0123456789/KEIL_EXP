From http://www.logstick.blogspot.com/ entry entitle "We interrupt this program..."

I have decided that polling a button to see if it's been pressed really is uncool. 
A much more realistic setup is to attach a button to an interrupt -- press the button 
and your program enters an interrupt service routine where you can service the button press. 
This is much more what embedded software is all about. 

However, the one problem for me is that I really hate writing interrupt code. I have found
setting up an interrupt controller and enabling interrupts one of the most hit and miss bits
of writing embedded software. There's always something that I forget or that isn't clear
in the manual. There I sit  carefully single stepping my interrupt setup code right up 
to the "Enable Interrupts" statement. One more step and Kachow! the processor is off in 
the weeds without any real clue of how it got there. Repeat until out of coffee.

So I'm approaching writing code for the Vectored Interrupt Controller (VIC) on the LPC2148 
somewhat with trepidation.

Looking in the examples in the RealView Microcontroller Development Kit, 
I see there is one example aptly named "interrupt" that sets up a couple of SWI's 
and a FIQ but sadly doesn't really setup the VIC. I'm sure I'll need more than a couple 
of interrupts for this project, so setting them up on the VIC seems to be the way to go.

Poking around the discussion forum I came across an article here 
http://www.keil.com/support/docs/2888.htm that dealt with setting up an entry in 
the VIC vector table to handle a timer interrupt. I took this code and built it with 
the RealView Microcontroller Development Kit. I then loaded the code up in the simulator 
and single stepped it. To my delight, I found that I could see the internals of both Timer0 
and the VIC being set up, since they are both simulated peripherals on the LPC2148. You can try 
this at home too by building this project and single stepping it.