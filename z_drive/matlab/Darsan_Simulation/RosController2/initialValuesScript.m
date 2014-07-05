

points = 20;

intom = 0.0254;                     %m/in
lbtokg = 0.453592;                  %lb/kg

port_servo_y_offset         =   11.5*intom;                   %m
starboard_servo_y_offset    =  -11.5*intom;                 %m
port_servo_x_offset         =  -(24-8)*intom;                %m
starboard_servo_x_offset    =    rx1;                          %m

strafAngle = atan(port_servo_y_offset/port_servo_x_offset);

portAngles = [linspace(-70*pi/180, -30*pi/180, points-3)'; -abs(strafAngle); pi/2-abs(strafAngle); 0];

starboardAngles = -portAngles;




thursters = [linspace(-50, 50,points-1)'; 0];

initialValues = [portAngles thursters starboardAngles thursters]