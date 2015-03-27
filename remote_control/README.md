To see the functionality of this node, Follow the below steps:
Run roscore 
Set up rqt
Run the xboxdrv
Run the joy node with js1 specified from cmd line
AXES [L STICK X, L STICK Y, R STICK X, R STICK Y, __, ___, Cursor Keys, Cursor Keys]
BUTTONS [GREEN, RED, BLUE, YELLOW, BACK, START, __ , __ ]
**********************************************************************************************
Upon complete correct set up, you can monitor the values of the pwm messages on the stm board,
L STICK :: /pwm1
R STICK :: /pwm2
Full Forward :: 0.02(seconds), 
Full Reverse :: 0.009(seconds),
Default :: 0.015(seconds)
**********************************************************************************************
