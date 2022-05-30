Reading An Analog Input in Software
===================================
In my travels around the net looking for information regarding the A/D converter, 
I came across several examples. I have now distilled them into one simple example 
of software driven A/D conversion, which can be found here. The program 
initializes channel 1 on AD Converter 0 for software driven A/D with 10 bits of 
accuracy. The function ADC_Read powers on A/D Converter 0 and starts the conversion. 
When the conversion is "Done" A/D Converter 0 is powered down again (gotta save those 
batteries). 

I have hacked the script that changes the voltage on an input pin to make that 
pin AD01. You can start the script running using the Analog ON button in the toolbox 
when you are in the uVision debugger. If you run this example, you can see the value 
of the Analog Input AD01 changing, and the result of the A/D conversion shows up in 
the RESULT portion of the A/D Global Status Register. The RESULT also appears as a 
10 bit number in the result variable in the debugger watch window. Just for fun I 
added the AD01 input and the result variable to the Logic Analyzer and you see can see 
these values change in step. This sort of thing would be useful if there were long 
delays between reading the A/D in which case result graph would become more a series 
of steps than a straight line and it would be easy to determine what data was being 
lost between sampling of the analog input.