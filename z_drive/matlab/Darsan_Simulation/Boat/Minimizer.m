function output = Minimizer(u0, C0, inputBound)

% global rx1 rx2 ry1 ry2 C;
% global Fxref Fyref Mref;


C = C0;


u1 = u0;

    for k = 1:1000
        h = getDirection(u1, C);
        alpha = getStepSize(u1, h, C);
        u1 = u1 + alpha*h;
        u1 = checkInputBound(u1,inputBound);
    end
    
    output = u1;

end

function output = checkInputBound(input,inputBound)
k = [0;0;0;0];
    for i = 1:length(input)
        k(i) = sat(input(i),inputBound(i,:));
    end
    output = k;
end

function output = sat(input, bound)

    if input < bound(1)
        output = bound(1);
    elseif input > bound(2)
        output = bound(2);
    else
        output = input;
    end

end

function output = getStepSize(input, h, C)

%     global Fxref Fyref Mref;

    alpha = 1;
    
    u0 = input;
    u1 = input + alpha*h;
    
    v0 = costFunction( u0, C );
    v1 = costFunction( u1, C );

    while v1 < v0
        alpha = alpha + 1;
        u1 = input + alpha*h;
        v0 = v1;
        v1 = costFunction( u1, C );
    end

    while v1 > v0 && alpha > 0
        alpha = alpha - 0.01;
        u1 = input + alpha*h;
        v0 = v1;
        v1 = costFunction( u1, C );
    end

    output = alpha;

end

function output = costFunction(input, C)

% global rx1 rx2 ry1 ry2 C;
% global Fxref Fyref Mref;

    theta1  = input(1);
    f1      = input(2);
    theta2  = input(3);
    f2      = input(4);
    
    output = C(1)/2*theta1^2 + C(2)/2*f1^2 + C(3)/2*theta2^2 + C(4)/2*f2^2;

end

function output = getDirection(input, C)

hsd = SteepestDescent(input, C);
hn = Newtons(input, C);

output = zeros(4,1);

    for i = 1:length(hn)

        if sign(hn(i)) > 0
            output(i) = hsd(i);
        else
            output(i) = hsd(i);
        end
    end
end

function output = SteepestDescent(input, C)

% global rx1 rx2 ry1 ry2 C;
% global Fxref Fyref Mref;

theta1  = input(1);
f1      = input(2);
theta2  = input(3);
f2      = input(4);

dVdtheta1 = C(1)*theta1;

dVdf1 = C(2)*f1;

dVdtheta2 = C(3)*theta2;

dVdf2 = C(4)*f2;

output = -[dVdtheta1; dVdf1; dVdtheta2; dVdf2];

end

function output = Newtons(input, C)
% global rx1 rx2 ry1 ry2 C;
% global Fxref Fyref Mref;


ddVddtheta1 = C(1);

ddVddf1 = C(2);

ddVddtheta2 = C(3);

ddVddf2 = C(4);
 


output = [ddVddtheta1; ddVddf1; ddVddtheta2; ddVddf2];

end