function output = Mintest(u0, phi, rx01, rx02, ry01, ry02, C0, inputBound, Fx, Fy, M)

rx1 = rx01; rx2 = rx02;
ry1 = ry01; ry2 = ry02;

C = C0;

[fx,fy,m] = forceOrientation(Fx,Fy,M, phi);

Fxref = fx; Fyref = fy; Mref  = m;

gridNum = 10;

grid = boundGrid(inputBound, gridNum, rx2,ry1);

u1 = getMinFromGrid(grid, u0, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C);

% u1 = u0;

    for k = 1:20
        h = getDirection(u1, u0, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C);
        alpha = getStepSize(u1, u0, h, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C);
        u1 = u1 + alpha*h;
        u1 = checkInputBound(u1,inputBound);
    end
    
    output = u1;

end