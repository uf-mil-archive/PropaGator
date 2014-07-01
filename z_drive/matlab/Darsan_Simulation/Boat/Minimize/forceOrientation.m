function [fx,fy,m] = forceOrientation(Fx,Fy,M, phi)
%#codegen

R = [cosd(phi) -sind(phi) 0;...
        sind(phi)  cosd(phi) 0;...
        0           0       1];
    
    FxyM = R'*[Fx;Fy;M];
    
    fx = FxyM(1);
    fy = FxyM(2);
    m = FxyM(3);
    
end