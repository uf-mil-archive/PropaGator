function [data] = requiredForces(x_current,   x_desired,   x_velocity_current,   x_velocity_desired,...
                                 y_current,   y_desired,   y_velocity_current,   y_velocity_desired,...
                                 yaw_current, yaw_desired, yaw_velocity_current, yaw_velocity_desired, pdGain)
                             
data.ForceX = pdGain.xp * (x_desired - x_current) + pdGain.xd * (x_velocity_desired - x_velocity_current);
data.ForceY = pdGain.yp * (y_desired - y_current) + pdGain.yd * (y_velocity_desired - y_velocity_current);
data.MomentZ = pdGain.yawd * (yaw_desired - yaw_current) + pdGain.yawd * (yaw_velocity_desired - yaw_velocity_current);

end