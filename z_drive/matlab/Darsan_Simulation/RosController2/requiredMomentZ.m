function output = requiredMomentZ(boat_angle_current, boat_angle_desired, boat_angular_velocity_current, boat_angular_velocity_desired)

global p_gain_theta_boat d_gain_theta_boat

temp_angle=boat_angle_desired-boat_angle_current;

output = p_gain_theta_boat*(temp_angle)+d_gain_theta_boat*(boat_angular_velocity_desired-boat_angular_velocity_current);

end



