function output = costFunction(input, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C)

    theta1  = input(1);
    f1      = input(2);
    theta2  = input(3);
    f2      = input(4);
    
    pretheta1   = preinput(1);
    pretheta2   = preinput(3);

    Fx = -f1*sind(theta1) - f2*sind(theta2);
    Fy = f1*cosd(theta1) + f2*cosd(theta2);
    M = rx1*f1*cosd(theta1) + ry1*f1*sind(theta1) + rx2*f2*cosd(theta2)...
        + ry2*f2*sind(theta2);

    output = C(1)*(Fx-Fxref)^2 +...
        C(2)*(Fy-Fyref)^2 +...
        C(3)*(M-Mref)^2  +...
        C(4)*f1^2 +...
        C(5)*f2^2 +...
        C(6)*(theta1 - pretheta1)^2 +...
        C(7)*(theta2 - pretheta2)^2;

end