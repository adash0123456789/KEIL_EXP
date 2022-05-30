From LogStick Blog posting: Interrupts Interrupts and yet more Interrupts
A final hack to the Timer Interrupt example and I put together the code for the button 
interrupt example. **It doesn't work though!** 
 
If you run up the the example code you will see that all is running very nicely until 
you "Push my Button"; then all hell ensues... interrupts just keep on happening 
(the counter in the pressed routine shown in Watch pane #1 bottom middle,  
just increments endlessly) and the main program doesn't get a chance to run 
(the blinking pin doesn't blink anymore). Oh bother! So near and yet so utterly interrupted. 
I guess I will look at this when less jet-lagged.