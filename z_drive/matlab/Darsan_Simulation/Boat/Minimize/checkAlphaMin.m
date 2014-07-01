
close all
clear i uans

figure()

i = -6:.01:2;

h = cos(pi/3);

u1 = pi/3;

for j = 1:length(i)
    u2 = u1 + h*i(j);
%     uans(j) = costFunction( u2, u0, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C );
uans(j) = sin(u2);


end
h
plot(i,uans)


