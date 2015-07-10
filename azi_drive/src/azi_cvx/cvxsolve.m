% Produced by CVXGEN, 2015-07-07 00:46:46 -0400.
% CVXGEN is Copyright (C) 2006-2012 Jacob Mattingley, jem@cvxgen.com.
% The code in this file is Copyright (C) 2006-2012 Jacob Mattingley.
% CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial
% applications without prior written permission from Jacob Mattingley.

% Filename: cvxsolve.m.
% Description: Solution file, via cvx, for use with sample.m.
function [vars, status] = cvxsolve(params, settings)
Ba = params.Ba;
Ohm = params.Ohm;
Q = params.Q;
a_0 = params.a_0;
a_max = params.a_max;
a_min = params.a_min;
da_max = params.da_max;
jBa_u = params.jBa_u;
tau = params.tau;
u_0 = params.u_0;
u_max = params.u_max;
u_min = params.u_min;
cvx_begin
  % Caution: automatically generated by cvxgen. May be incorrect.
  variable da(2, 1);
  variable du(2, 1);

  minimize(quad_form(da, Ohm) + quad_form(tau - Ba*u_0 - Ba*du - jBa_u*da, Q));
  subject to
    u_min - u_0 <= du;
    du <= u_max - u_0;
    a_min - a_0 <= da;
    da <= a_max - a_0;
    -da_max <= da;
    da <= da_max;
cvx_end
vars.da = da;
vars.du = du;
status.cvx_status = cvx_status;
% Provide a drop-in replacement for csolve.
status.optval = cvx_optval;
status.converged = strcmp(cvx_status, 'Solved');