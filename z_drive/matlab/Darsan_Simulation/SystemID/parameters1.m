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

kr = 2;
fw = 4;
C = [1 0 0 0 0 0;0 0 1 0 0 0;0 0 0 0 1 0];

K = [1000 1000 1000 10 10 1 1];


    ts = 0.1;
    
% for i = 1:length(minInput)
%     uSat(i) = saturation([minInput(2),maxInput(2)]);
% end


intom = 0.0254;                     %m/in
lbtokg = 0.453592;                  %lb/kg

ry1 = 12*intom;                    %m
ry2 =  -12*intom;                    %m
rx1 = -.7; %-69*intom/2 + 6*intom;        %m
rx2 = rx1;                          %m

minInput = [atan(rx1/ry1)*180/pi-10; -30; atan(rx2/ry2)*180/pi-100; -30];
maxInput = [atan(rx1/ry1)*180/pi+100; 30; atan(rx2/ry2)*180/pi+10; 30];

inputBound = [minInput maxInput];

m = 46.27; %80*lbtokg;                      %kg
I = 15;%1/12*m*((2*ry1)^2 + (2*rx1)^2); %m^2/kg

%%% PID Control Gaines
k1x = 100;
k2x = 60;%(4*k1x*m)^.5;

k1y = 100;
k2y = 60;%(4*k1y*m)^.5;

k1m = 50;
k2m = 25;%(4*k1m*I)^.5;

friX1 = 4;
friX2 = 0.76462;
friY  = 8;
friZ  = 4;