function output = resultantForceX(port_servo_angle, port_thruster_force, starboard_servo_angle, starboard_thruster_force)

output = port_thruster_force*cos(port_servo_angle) + starboard_thruster_force*cos(starboard_servo_angle);

end