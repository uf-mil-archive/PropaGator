 
i = 1;

figure()
subplot(4,1,1), plot(data(i).estimated_port_servo_angle)
title('estimated configuration actuator')
subplot(4,1,2), plot(data(i).estimated_port_thruster_force)
subplot(4,1,3), plot(data(i).estimated_starboard_servo_angle)
subplot(4,1,4), plot(data(i).estimated_starboard_thruster_force)

figure()
subplot(3,1,1), plot(data(i).force_bow_required)
title('force from boat')
subplot(3,1,2), plot(data(i).force_port_required)
subplot(3,1,3), plot(data(i).required_moment_z)

figure()
subplot(3,1,1), plot(data(i).x_current) 
hold on 
plot(data(i).x_desired, 'r')
title('globle position')
subplot(3,1,2), plot(data(i).y_current) 
hold on 
plot(data(i).y_desired, 'r')
subplot(3,1,3), plot(data(i).yaw_current) 
hold on 
plot(data(i).yaw_desired, 'r')

figure()
subplot(3,1,1), plot(data(i).bow_desired_from_boat) 
hold on 
plot(zeros(size(data(i).bow_desired_from_boat)), 'r')
title('desired position from boat')
subplot(3,1,2), plot(data(i).port_desired_from_boat) 
hold on 
plot(zeros(size(data(i).bow_desired_from_boat)), 'r')
subplot(3,1,3), plot(data(i).yaw_desired_from_boat)
hold on 
plot(zeros(size(data(i).bow_desired_from_boat)), 'r')
