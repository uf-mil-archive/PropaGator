function output = force_port_required(x_current, x_desired, x_velocity_current, x_velocity_desired, y_current, y_desired, y_velocity_current, y_velocity_desired, yaw_current)

output = -sin(yaw_current)*requiredForceX(x_current, x_desired, x_velocity_current, x_velocity_desired) +...
    cos(yaw_current)*requiredForceY(y_current, y_desired, y_velocity_current, y_velocity_desired);

end