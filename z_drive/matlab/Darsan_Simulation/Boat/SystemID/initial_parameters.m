intom = 0.0254;                     %m/in
lbtokg = 0.453592;                  %lb/kg

ry1 = -12*intom;                    %m
ry2 =  12*intom;                    %m
rx1 = -69*intom/2 + 6*intom;        %m
rx2 = rx1;                          %m

M = 80*lbtokg;                      %kg
I = 1/12*M*((2*ry1)^2 + (2*rx1)^2); %m^2/kg

friX = 3;
friY = 8;
friZ = 4;