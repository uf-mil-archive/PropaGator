syms theta1 theta2 f1 f2 Fhatx phi;

intom = 0.0254;                 %m/in
rx1 = -12*intom;                %m
rx2 =  12*intom;                %m
ry1 = -69*intom/2 + 6*intom;    %m
ry2 = -69*intom/2 + 6*intom;    %m

syms rx1 rx2 ry1 ry2;

syms C1 C2 C3 C4 C5 C6 C7;

syms Fxref Fyref Mref pretheta1 pretheta2;

R(phi) = [cos(phi) sin(phi) 0;-sin(phi) cos(phi) 0; 0 0 1];

Fx(theta1, theta2, f1, f2) = -f1*sin(theta1) - f2*sin(theta2);
Fy(theta1, theta2, f1, f2) = f1*cos(theta1) + f2*cos(theta2);
M(theta1, theta2, f1, f2) = rx1*f1*cos(theta1) + ry1*f1*sin(theta1) ...
    + rx2*f2*cos(theta2) + ry2*f2*sin(theta2);

% FF = R*[Fx;Fy;M];


V = C1/2*(Fx-Fxref)^2 +...
C2/2*(Fy-Fyref)^2 +...
C3/2*(M-Mref)^2  +...
C4/2*f1^2 +...
C5/2*f2^2 +...
C6/2*(theta1 - pretheta1)^2 +...
C7/2*(theta2 - pretheta2)^2;


dVdtheta1 = diff(V,theta1)
dVdf1 = diff(V,f1)
dVdtheta2 = diff(V,theta2)
dVdf2 = diff(V,f2)

ddVddtheta1 = diff(V,theta1,2)
ddVddf1 = diff(V,f1,2)
ddVddtheta2 = diff(V,theta2,2)
ddVddf2 = diff(V,f2,2)

