function output = Minimizer(u0, rx01, rx02, ry01, ry02, gain, grid, Fxref, Fyref, Mref)

rx1 = rx01; rx2 = rx02;
ry1 = ry01; ry2 = ry02;



%Fxref = Fx; Fyref = Fy; Mref  = M;

gridNum = 10;

% grid = boundGrid(inputBound, gridNum, rx1,ry1);

u1 = getMinFromGrid(grid, u0, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain);

% u1 = u0;

% %     for k = 1:20
% %         h = getDirection(u1, u0, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C);
% %         alpha = getStepSize(u1, u0, h, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C);
% %         u1 = u1 + alpha*h;
% %         u1 = checkInputBound(u1,inputBound);
% %     end

    
    output = u1;

end

function output = getMinFromGrid(grid, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain)

[ii, iii] = size(grid);
v0 = costFunction(grid(1,:), preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain);

output = [grid(1,1);grid(1,2);grid(1,3);grid(1,4)];

for i = 1:ii
    for j = 1:ii
        for k = 1:ii
            for l = 1:ii
                input = [grid(i,1);grid(j,2);grid(k,3);grid(l,4)];
                v1 = costFunction(input,preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain);
                if v0(1) > v1(1)
                    v0(1) = v1(1);
                    output = input;
                else
                end
            end
        end
    end
end

end

function output = checkInputBound(input,inputBound)
k = [0;0;0;0];
    for i = 1:length(input)
        k(i) = sat(input(i),inputBound(i,:));
    end
    output = k;
end

function output = boundGrid(inputBound, gridNum, rx1,ry1)

p = gridNum;

current_port_servo_angle =    [0.2664;0.1837;0.1010;0.0184;-0.0643;-0.1470;-0.2296;-0.3123;-0.3950;-0.4777;...
-0.5603;-0.6430;-0.7257;-0.8084;-0.8910;-0.9737;-1.0564;-1.1054;-1.1391;-1.2217];                      %linspace(inputBound(1,1),inputBound(1,2),p)';
current_port_thruster_force =        [39.25;30;20;10;5;0;-5;-10;-15;-23.65;zeros(10,1)];                                                             %linspace(inputBound(2,1),inputBound(2,2),p)';
current_starboard_servo_angle =    -1*[0.2664;0.1837;0.1010;0.0184;-0.0643;-0.1470;-0.2296;-0.3123;-0.3950;-0.4777;...
-0.5603;-0.6430;-0.7257;-0.8084;-0.8910;-0.9737;-1.0564;-1.1054;-1.1391;-1.2217];                      %linspace(inputBound(3,1),inputBound(3,2),p)';
current_starboard_thruster_force =        [39.25;30;20;10;5;0;-5;-10;-15;-23.65;zeros(10,1)];                                                            %linspace(inputBound(4,1),inputBound(4,2),p)';

%i = atan(rx1/ry1)*180/pi;

output = [current_port_servo_angle, current_port_thruster_force, current_starboard_servo_angle, current_starboard_thruster_force];%;...
%           inputBound(1,1),  0,      inputBound(3,1),  0;...
%              0,    100,    0,    100;...%];
%              0,   -100,    0,   -100]; %...
%             i,    0,    i,    0];

end

function output = sat(input, bound)

    if input < bound(1)
        output = bound(1);
    elseif input > bound(2)
        output = bound(2);
    else
        output = input;
    end

end

% function output = getStepSize(input, preinput, h, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain, m, I)
% 
%     alpha = .001;
%     
%     u0 = input;
%     u1 = input + alpha*h;
%     
%     v0 = costFunction( u0, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain, m, I );
%     v1 = costFunction( u1, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain, m, I);
% 
%     while v1 <= v0 && v1 ~= 0
%         alpha = alpha + .001;
%         u1 = input + alpha*h;
%         v0 = v1;
%         v1 = costFunction( u1, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain, m, I );
%     end
%     
%     v0 = v1;
%     alpha = .0015;
% 
%     while v1 <= v0  && v1 ~= 0
%         alpha = alpha - 0.0001;
%         u1 = input + alpha*h;
%         v0 = v1;
%         v1 = costFunction( u1, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain, m, I );
%     end
% 
% 
%     output = alpha;
% 
% end

function output = costFunction(input, preinput, Fxref, Fyref, Mref, port_servo_x_offset, starboard_servo_x_offset, port_servo_y_offset, starboard_servo_y_offset, gain)

    port_servo_angle  = input(1);
    port_thruster_force      = input(2);
    starboard_servo_angle  = input(3);
    starboard_thruster_force      = input(4);
    
    current_port_servo_angle   = preinput(1);
    current_starboard_servo_angle   = preinput(3);

    Fx = (port_thruster_force*cos(port_servo_angle) + starboard_thruster_force*cos(starboard_servo_angle));
    Fy = (port_thruster_force*sin(port_servo_angle) + starboard_thruster_force*sin(starboard_servo_angle));
    M = (-port_servo_x_offset*port_thruster_force*sin(port_servo_angle) + port_servo_y_offset*port_thruster_force*cos(port_servo_angle)... 
        - starboard_servo_x_offset*starboard_thruster_force*sin(starboard_servo_angle) + starboard_servo_y_offset*starboard_thruster_force*cos(starboard_servo_angle));

    output = gain.error_force_x/2               * (Fx-Fxref)^2 +...
        gain.error_force_y/2                    * (Fy-Fyref)^2 +...
        gain.error_moment_z/2                   * (M-Mref)^2  +...
        gain.thruster_force/2                   * port_thruster_force^2 +...
        gain.thruster_force/2                   * starboard_thruster_force^2 +...
        gain.deviation_equilibrum_servo_angle/2 * port_servo_angle^2 +...
        gain.deviation_equilibrum_servo_angle/2 * starboard_servo_angle^2 +...
        gain.deviation_changeof_servo_angle/2   * (port_servo_angle - current_port_servo_angle)^2 +...
        gain.deviation_changeof_servo_angle/2   * (starboard_servo_angle - current_starboard_servo_angle)^2;

end

% function output = getDirection(input, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain, m, I)
% 
% hsd = SteepestDescent(input, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain, m, I);
% 
% output = zeros(4,1);
% 
%     for i = 1:length(hsd)
%         output(i) = hsd(i);
%     end
%     
% end

% function output = SteepestDescent(input, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, gain, m, I)
% 
% theta1  = input(1);
% f1      = input(2);
% theta2  = input(3);
% f2      = input(4);
% 
% pretheta1   = preinput(1);
% pretheta2   = preinput(3);
% 
% dVdtheta1 = C(3)*(f1*ry1*cos(theta1)...
%     - f1*rx1*sin(theta1))*(f1*rx1*cos(theta1)...
%     - Mref + f2*rx2*cos(theta2) + f1*ry1*sin(theta1)...
%     + f2*ry2*sin(theta2)) - C(6)*(pretheta1 - theta1)...
%     - C(2)*f1*sin(theta1)*(f1*cos(theta1) - Fyref + f2*cos(theta2))...
%     + C(1)*f1*cos(theta1)*(Fxref + f1*sin(theta1) + f2*sin(theta2));
% 
% dVdf1 = C(4)*f1 + C(1)*sin(theta1)*(Fxref + f1*sin(theta1)...
%     + f2*sin(theta2)) + C(2)*cos(theta1)*(f1*cos(theta1) - Fyref...
%     + f2*cos(theta2)) + C(3)*(rx1*cos(theta1)...
%     + ry1*sin(theta1))*(f1*rx1*cos(theta1) - Mref + f2*rx2*cos(theta2)...
%     + f1*ry1*sin(theta1) + f2*ry2*sin(theta2));
% 
% dVdtheta2 = C(3)*(f2*ry2*cos(theta2)...
%     - f2*rx2*sin(theta2))*(f1*rx1*cos(theta1)...
%     - Mref + f2*rx2*cos(theta2) + f1*ry1*sin(theta1)...
%     + f2*ry2*sin(theta2)) - C(7)*(pretheta2 - theta2)...
%     - C(2)*f2*sin(theta2)*(f1*cos(theta1) - Fyref + f2*cos(theta2))...
%     + C(1)*f2*cos(theta2)*(Fxref + f1*sin(theta1) + f2*sin(theta2));
% 
% dVdf2 = C(5)*f2 + C(1)*sin(theta2)*(Fxref + f1*sin(theta1)...
%     + f2*sin(theta2)) + C(2)*cos(theta2)*(f1*cos(theta1) - Fyref...
%     + f2*cos(theta2)) + C(3)*(rx2*cos(theta2)...
%     + ry2*sin(theta2))*(f1*rx1*cos(theta1) - Mref + f2*rx2*cos(theta2)...
%     + f1*ry1*sin(theta1) + f2*ry2*sin(theta2));
%  
% output = -[dVdtheta1; dVdf1; dVdtheta2; dVdf2];
% 
% end

