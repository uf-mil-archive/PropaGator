function output = resultantForceY (port_servo_angle,  port_thruster_force,  starboard_servo_angle, starboard_thruster_force)

output = port_thruster_force*sin(port_servo_angle) + starboard_thruster_force*sin(starboard_servo_angle);

end