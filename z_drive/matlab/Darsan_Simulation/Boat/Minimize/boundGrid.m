function output = boundGrid(inputBound, gridNum,rx2,ry1)

p = gridNum;

theta1 =    linspace(inputBound(1,1),inputBound(1,2),p)';
f1 =        linspace(inputBound(2,1),inputBound(2,2),p)';
theta2 =    linspace(inputBound(3,1),inputBound(3,2),p)';
f2 =        linspace(inputBound(4,1),inputBound(4,2),p)';

i = atan(rx2/-ry1)*180/pi;

output = [theta1, f1, theta2, f2;...
            0,    1,    0,    1;...
           -i,   -1,   -i,   -1;...
            i,    0,    i,    0];

end