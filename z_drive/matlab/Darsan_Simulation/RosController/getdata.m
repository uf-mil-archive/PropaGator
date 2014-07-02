a = [428 4676 13296 15000];
b = [1766 9515 15399 17400];

for i = 1:length(a)
    data(i).required_force_y = required_force_y(a(i):b(i));
    data(i).required_force_x = required_force_x(a(i):b(i));
    data(i).required_moment_z = required_moment_z(a(i):b(i));
    
    data(i).x_current = x_current(a(i):b(i));
    data(i).y_current = y_current(a(i):b(i));
    data(i).z_current = z_current(a(i):b(i));
    data(i).yaw_current = yaw_current(a(i):b(i));
    
    data(i).x_desired = x_desired(a(i):b(i));
    data(i).y_desired = y_desired(a(i):b(i));
    data(i).yaw_desired = yaw_desired(a(i):b(i));
    
    data(i).x_velocity_current = x_velocity_current(a(i):b(i));
    data(i).y_velocity_current = y_velocity_current(a(i):b(i));
    data(i).z_velocity_current = z_velocity_current(a(i):b(i));
    data(i).yaw_velocity_current = yaw_velocity_current(a(i):b(i));
    
    data(i).x_velocity_desired = x_velocity_desired(a(i):b(i));
    data(i).y_velocity_desired = y_velocity_desired(a(i):b(i));
    data(i).yaw_velocity_desired = yaw_velocity_desired(a(i):b(i));
    
    data(i).current_port_servo_angle = current_port_servo_angle(a(i):b(i));
    data(i).current_port_thruster_force = current_port_thruster_force(a(i):b(i));
    data(i).current_starboard_servo_angle = current_starboard_servo_angle(a(i):b(i));
    data(i).current_starboard_thruster_force = current_starboard_thruster_force(a(i):b(i));
    
    data(i).estimated_port_servo_angle = estimated_port_servo_angle(a(i):b(i));
    data(i).estimated_port_thruster_force = estimated_port_thruster_force(a(i):b(i));
    data(i).estimated_starboard_servo_angle = estimated_starboard_servo_angle(a(i):b(i));
    data(i).estimated_starboard_thruster_force = estimated_starboard_thruster_force(a(i):b(i));
    
    data(i).force_bow_required = cos(data(i).yaw_current).*data(i).required_force_x + sin(data(i).yaw_current).*data(i).required_force_y;
	data(i).force_port_required = -sin(data(i).yaw_current).*data(i).required_force_x + cos(data(i).yaw_current).*data(i).required_force_y;
    data(i).moment_up_required = data(i).required_moment_z;
    
    data(i).bow_desired_from_boat = cos(data(i).yaw_current).*(data(i).x_desired - data(i).x_current) + sin(data(i).yaw_current).*(data(i).y_desired - data(i).y_current);
    data(i).port_desired_from_boat = -sin(data(i).yaw_current).*(data(i).x_desired - data(i).x_current) + cos(data(i).yaw_current).*(data(i).y_desired - data(i).y_current);
    data(i).yaw_desired_from_boat = data(i).yaw_desired - data(i).yaw_current;
    
    data(i).bow_velocity = cos(data(i).yaw_current).*(data(i).x_velocity_current) + sin(data(i).yaw_current).*(data(i).y_velocity_current);
    data(i).port_velocity = -sin(data(i).yaw_current).*(data(i).x_velocity_current) + cos(data(i).yaw_current).*(data(i).y_velocity_current);
    data(i).yaw_velocity = data(i).yaw_desired - data(i).yaw_current;
    
end
