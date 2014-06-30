function output = resultantMomentZ( port_servo_angle, port_thruster_force, starboard_servo_angle, starboard_thruster_force)

global port_servo_y_offset port_servo_x_offset starboard_servo_y_offset starboard_servo_x_offset

output =  (port_servo_y_offset*port_thruster_force*cos(port_servo_angle) + starboard_servo_y_offset*starboard_thruster_force*cos(starboard_servo_angle) - port_servo_x_offset*port_thruster_force*sin(port_servo_angle) - starboard_servo_x_offset*starboard_thruster_force*sin(starboard_servo_angle));

end