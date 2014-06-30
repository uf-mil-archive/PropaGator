syms port_servo_angle starboard_servo_angle port_thruster_force starboard_thruster_force Fhatx phi boat_mass boat_inertia;

% intom = 0.0254;                 %m/in
% port_servo_x_offset = -12*intom;                %m
% starboard_servo_x_offset =  12*intom;                %m
% port_servo_y_offset = -69*intom/2 + 6*intom;    %m
% starboard_servo_y_offset = -69*intom/2 + 6*intom;    %m

syms port_servo_x_offset starboard_servo_x_offset port_servo_y_offset starboard_servo_y_offset;

syms gain_error_force_x gain_error_force_y gain_error_moment_z gain_thrusters_force gain_deviation_equilibrum_servo_angle gain_deviation_changeof_servo_angle C7;

syms force_bow_required force_port_required moment_z_required current_port_servo_angle current_starboard_servo_angle;

% R(phi) = [cos(phi) sin(phi) 0;-sin(phi) cos(phi) 0; 0 0 1];

resultantForceX(port_servo_angle, starboard_servo_angle, port_thruster_force, starboard_thruster_force) = (port_thruster_force*cos(port_servo_angle) + starboard_thruster_force*cos(starboard_servo_angle));
resultantForceY(port_servo_angle, starboard_servo_angle, port_thruster_force, starboard_thruster_force) = (port_thruster_force*sin(port_servo_angle) + starboard_thruster_force*sin(starboard_servo_angle));
resultantMomentZ(port_servo_angle, starboard_servo_angle, port_thruster_force, starboard_thruster_force) = (-port_servo_x_offset*port_thruster_force*sin(port_servo_angle) + port_servo_y_offset*port_thruster_force*cos(port_servo_angle) - starboard_servo_x_offset*starboard_thruster_force*sin(starboard_servo_angle)...
        + starboard_servo_y_offset*starboard_thruster_force*cos(starboard_servo_angle));

% FF = R*[resultantForceX;resultantForceY;resultantMomentZ];


calcCost = gain_error_force_x/2*(resultantForceX-force_bow_required)^2 +...
gain_error_force_y/2*(resultantForceY-force_port_required)^2 +...
gain_error_moment_z/2*(resultantMomentZ-moment_z_required)^2  +...
gain_thrusters_force/2*(port_thruster_force^2 + starboard_thruster_force^2) +...
gain_deviation_equilibrum_servo_angle/2*(port_servo_angle^2 + starboard_servo_angle^2) +...
gain_deviation_changeof_servo_angle/2*((port_servo_angle - current_port_servo_angle)^2 + (starboard_servo_angle - current_starboard_servo_angle)^2)


dCost_dPortServoAngle = diff(calcCost,port_servo_angle)
dCost_dPortThrusterForce = diff(calcCost,port_thruster_force)
dCost_dStarboardServoAngle = diff(calcCost,starboard_servo_angle)
dCost_dStarboardThrusterForce = diff(calcCost,starboard_thruster_force)

% ddVddtheta1 = diff(calcCost,port_servo_angle,2)
% ddVddf1 = diff(calcCost,port_thruster_force,2)
% ddVddtheta2 = diff(calcCost,starboard_servo_angle,2)
% ddVddf2 = diff(calcCost,starboard_thruster_force,2)