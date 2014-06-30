function output = requiredForceY(y_current, y_desired, y_velocity_current, y_velocity_desired)

global p_gain_y d_gain_y

output =  p_gain_y*(y_desired-y_current)+d_gain_y*(y_velocity_desired-y_velocity_current);

end