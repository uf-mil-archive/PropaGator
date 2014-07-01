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


intom = 0.0254;                     %m/in
lbtokg = 0.453592;                  %lb/kg

rx1 = -12*intom;                    %m
rx2 =  12*intom;                    %m
ry1 = -69*intom/2 + 6*intom;        %m
ry2 = ry1;                          %m

M = 80*lbtokg;                      %kg
I = 1/12*M*((2*rx1)^2 + (2*ry1)^2); %m^2/kg

%%% PID Control Gaines
k1x = 50;
k2x = (4*k1x*M)^.5;

k1y = 50;
k2y = (4*k1y*M)^.5;

k1m = 70;
k2m = (4*k1m*I)^.5;


    ts = 0.1;
    
minInput = [-50;-100;-100;-100];
maxInput = [100;100;50;100];

inputBound = [minInput maxInput];

for i = 1:length(minInput)
    uSat(i) = saturation([minInput(2),maxInput(2)]);
end
