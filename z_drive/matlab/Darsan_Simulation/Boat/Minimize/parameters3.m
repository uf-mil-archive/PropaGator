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

K = [100 100 100 1 1 0.1 0.1];


intom = 0.0254;                     %m/in
lbtokg = 0.453592;                  %lb/kg

rx1 = -12*intom;                    %m
rx2 =  12*intom;                    %m
ry1 = -69*intom/2 + 6*intom;        %m
ry2 = ry1;                          %m

M = 80*lbtokg;                      %kg
I = 1/12*M*((2*rx1)^2 + (2*ry1)^2); %m^2/kg

    ts = 0.1;
    minInput = [-50;-100;-100;-100];
maxInput = [100;100;50;100];
% minInput = [-30;-70;-45;-70];
% maxInput = [45;100;30;100];

inputBound = [minInput maxInput];

for i = 1:length(minInput)
    uSat(i) = saturation([minInput(i),maxInput(i)]);
end
