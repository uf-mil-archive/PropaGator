intom = 0.0254;                     %m/in
lbtokg = 0.453592;                  %lb/kg

ry1 = 12*intom;                    %m
ry2 =  -12*intom;                    %m
rx1 = -1.2474; %-69*intom/2 + 6*intom;        %m
rx2 = rx1;                          %m

m = 46.27;%80*lbtokg;                      %kg
I = 1/12*m*((2*ry1)^2 + (2*rx1)^2); %m^2/kg

friX1 = 4;
friX2 = 0.76462;
friY  = 8;
friZ  = 4;