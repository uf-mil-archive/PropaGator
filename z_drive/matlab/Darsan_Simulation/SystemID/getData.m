from = 1;
to = 270;%length(time);

time = rosbagTimestamp*10^-9;
time = time(from:to) - time(from);



f2 = current_starboard_thruster_force(from:to);
f1 = current_port_thruster_force(from:to);
theta1 = current_port_servo_angle(from:to) * 180/pi;
theta2 = current_starboard_servo_angle(from:to) * 180/pi;


X = x_current(from:to) - x_current(from);
Y = y_current(from:to) - y_current(from);
Yaw = yaw_current(from:to) * 180/pi;


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


