% getdata.m
% parameters.m



for l = 1:length(data)
    u0 = [0 0 0 0];
    for i = 1:length(data(1).x_current)
    
    req = requiredForces(data(l).x_current(i),   data(l).x_desired(i),   data(l).x_velocity_current(i),   data(l).x_velocity_desired(i),...
                         data(l).y_current(i),   data(l).y_desired(i),   data(l).y_velocity_current(i),   data(l).y_velocity_desired(i),...
                         data(l).yaw_current(i), data(l).yaw_desired(i), data(l).yaw_velocity_current(i), data(l).yaw_velocity_desired(i), pdGain);
                        
    ForceX(i)  = req.ForceX;
    ForceY(i)  = req.ForceY;
    MomentZ(i) = req.MomentZ;
    
    errorX(i) = data(l).x_desired(i) - data(l).x_current(i);
    errorY(i) = data(l).y_desired(i) - data(l).y_current(i);
    errorYaw(i) = data(l).yaw_desired(i) - data(l).yaw_current(i);    
    
%     output = Minimizer(u0, rx1, rx2, ry1, ry2, gain, initialValues, ForceX(i), ForceY(i), MomentZ(i));
%     
%     theta1(i) = output(1);
%     f1(i) = output(2);
%     theta2(i) = output(3);
%     f2(i) = output(4);
    
%    u0 = output(i);
    
    end
    
%     estimated(l).port_servo_angle = theta1;
%     estimated(l).port_thruster_force = f1;
%     estimated(l).starboard_servo_angle = theta2;
%     estimated(l).starboard_thurster_force = f2;

    
    required(l).ForceX = ForceX;
    required(l).ForceY = ForceY;
    required(l).MomentZ = MomentZ;
    
    error(l).x = errorX;
    error(l).y = errorY;
    error(l).yaw = errorYaw;
    
    figure()
    plot(error(l).x);
    
    figure()
    plot(error(l).y);
    
    figure()
    plot(error(l).yaw);

    figure()
    plot(required(l).ForceX)
    title('X Force Required')
    hold on
    plot(data(l).required_force_x,'r')

    figure()
    plot(required(l).ForceY)
    title('Y Force Required')
    hold on
    plot(data(l).required_force_y,'r')

    figure()
    plot(required(l).MomentZ)
    title('Moment Required')
    hold on
    plot(data(l).required_moment_z,'r')

end
                        
                        
