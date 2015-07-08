% csolve  Solves a custom quadratic program very rapidly.
%
% [vars, status] = csolve(params, settings)
%
% solves the convex optimization problem
%
%   minimize(quad_form(da, Ohm) + quad_form(tau - Ba*u_0 - Ba*du - jBa_u*da, Q))
%   subject to
%     u_min - u_0 <= du
%     du <= u_max - u_0
%     a_min - a_0 <= da
%     da <= a_max - a_0
%     -da_max <= da
%     da <= da_max
%
% with variables
%       da   2 x 1
%       du   2 x 1
%
% and parameters
%       Ba   3 x 2
%      Ohm   2 x 2    PSD
%        Q   3 x 3    PSD
%      a_0   2 x 1
%    a_max   2 x 1
%    a_min   2 x 1
%   da_max   2 x 1
%    jBa_u   3 x 2
%      tau   3 x 1
%      u_0   2 x 1
%    u_max   2 x 1
%    u_min   2 x 1
%
% Note:
%   - Check status.converged, which will be 1 if optimization succeeded.
%   - You don't have to specify settings if you don't want to.
%   - To hide output, use settings.verbose = 0.
%   - To change iterations, use settings.max_iters = 20.
%   - You may wish to compare with cvxsolve to check the solver is correct.
%
% Specify params.Ba, ..., params.u_min, then run
%   [vars, status] = csolve(params, settings)
% Produced by CVXGEN, 2015-07-07 00:46:46 -0400.
% CVXGEN is Copyright (C) 2006-2012 Jacob Mattingley, jem@cvxgen.com.
% The code in this file is Copyright (C) 2006-2012 Jacob Mattingley.
% CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial
% applications without prior written permission from Jacob Mattingley.

% Filename: csolve.m.
% Description: Help file for the Matlab solver interface.
