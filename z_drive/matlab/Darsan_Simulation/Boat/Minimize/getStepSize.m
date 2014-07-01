function output = getStepSize(input, preinput, h, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C)

%     global Fxref Fyref Mref;

    alpha = .001;
    
    u0 = input;
    u1 = input + alpha*h;
    
    v0 = costFunction( u0, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C );
    v1 = costFunction( u1, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C );

    while v1 <= v0 && v1 ~= 0
        alpha = alpha + .001;
        u1 = input + alpha*h;
        v0 = v1;
        v1 = costFunction( u1, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C );
    end
    
    v0 = v1;
    alpha = .0015;

    while v1 <= v0  && v1 ~= 0
        alpha = alpha - 0.0001;
        u1 = input + alpha*h;
        v0 = v1;
        v1 = costFunction( u1, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C );
    end


    output = alpha;

end