function [y,uu] = boatDynamics(input)
    global m I rx1 rx2 ry1 ry2;
    %   %%%%%%%%%%%%%%%%%%%%%%%%%
    % %       Boat Geometry       %
    % % %%%%%%%%%%%%%%%%%%%%%%%%% %
    %
    % % %%%%%%%%%%%%%%%%%%%%%%%%%-----
    % % %                       %   A
    % % %                       %   |
    % % %                       %   |
    % % %                       %   |
    % % %                       %   |
    % % %           O           %  69"
    % % %           %           %   |
    % % %   O       %<-12"->O---%   |
    % % %           %       A   %   |
    % % %           %     6"|   %   |
    % % %           %       V   %   V
    % % %%%%%%%%%%%%%%%%%%%%%%%%%-----
    % % |<---------34"--------->|
    % % |                       |
    
    intom = 0.0254;                     %m/in
    lbtokg = 0.453592;                  %lb/kg

    rx1 = -12*intom;                    %m
    rx2 =  12*intom;                    %m
    ry1 = -69*intom/2 + 6*intom;        %m
    ry2 = ry1;                          %m

    m = 80*lbtokg;                      %kg
    I = 1/12*m*((2*rx1)^2 + (2*ry1)^2); %m^2/kg

    ts = 0.1;
    
    y(:,i+1) = systemStep1( y(:,i), u, ts );
    
end

function v1 = systemStep1(v0,u0,ts)

    % %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %
    %    v = [x...          %    u = [theta1...    %
    %        x_dot...       %        f1...         %
    %        y...           %        theta2...     %
    %        y_dot...       %        f2]           %
    %        theta...       %                      %
    %        theta_dot];    %                      %
    % %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %
    
    global m I rx1 rx2 ry1 ry2;

    v1(1) = v0(1) + ts * ( v0(2) );
    v1(2) = v0(2) + ts * ( -u0(2)/m*sind(u0(1)+v0(5)) - u0(4)/m*sind(u0(3)+v0(5)) - 5*v0(2) );
    v1(3) = v0(3) + ts * ( v0(4) );
    v1(4) = v0(4) + ts * ( u0(2)/m*cosd(u0(1)+v0(5)) + u0(4)/m*cosd(u0(3)+v0(5)) - 2*v0(4));
    v1(5) = v0(5) + ts * ( v0(6) );
    v1(6) = v0(6) + ts * ( rx1/I*u0(2)*cosd(u0(1)) + ry1/I*u0(2)*sind(u0(1)) + rx2/I*u0(4)*cosd(u0(3)) + ry2/I*u0(4)*sind(u0(3)) - 9*v0(6) );

end

