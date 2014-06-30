time = rosbagTimestamp*10^-9;
time = time - time(1);



f2 = currentstarboardthrusterforce;
f1 = currentportthrusterforce;
theta1 = currentportservoangle * 180/pi;
theta2 = currentstarboardservoangle * 180/pi;


X = xcurrent - xcurrent(1);
Y = ycurrent - ycurrent(1);
Yaw = yawcurrent * 180/pi;


for i = 1:length(Yaw)
    if Yaw(i)>0
        Yaw(i) = Yaw(i) - 360;
    end
end

rotate = [cosd(Yaw(1)) sind(Yaw(1));
            -sind(Yaw(1)) cosd(Yaw(1))];
vector = rotate*[X';Y'];

plot(vector(1,:),vector(2,:));
hold on
plot(X,Y,'k')

X = vector(1,:)';
Y = vector(2,:)';

Yaw = Yaw - Yaw(1);




% output.Yaw = output.Yaw - output.Yaw(1);
% output.X = output.X - output.X(1);
% output.Y = output.Y - output.Y(1);


% figure();
% plot (time);
% 
figure()
plot(Yaw)

figure()
plot(X)

figure()
plot(Y)
% 
% figure()
% plot(theta1)
% 
% figure()
% plot(theta2)

% clearvars -except f1 f2 theta1 theta2 X Y Yaw time


