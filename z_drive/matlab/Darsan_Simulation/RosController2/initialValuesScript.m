

points = 20;

intom = 0.0254;                     %m/in
lbtokg = 0.453592;                  %lb/kg

ry1 = 11.5*intom;                   %m
ry2 =  -11.5*intom;                 %m
rx1 = -25*intom;              %m
rx2 = rx1;                          %m

portAngles = [linspace(-70*pi/180, -30*pi/180,points-3)'; -abs(atan(ry1/rx1)); pi/2-abs(atan(ry1/rx1)); 0];

starboardAngles = -portAngles;

strafAngle = atan(ry1/rx1);


thursters = [linspace(-23, 39,points-1)'; 0];

initialValues = [portAngles thursters starboardAngles thursters]