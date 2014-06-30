function output = requiredForceX(x_current, x_desired, x_velocity_current, x_velocity_desired)

global p_gain_x d_gain_x

output = p_gain_x*(x_desired-x_current)+d_gain_x*(x_velocity_desired-x_velocity_current);

end