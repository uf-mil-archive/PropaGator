function output = getDirection(input, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C)

hsd = SteepestDescent(input, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C);

output = zeros(4,1);

    for i = 1:length(hsd)
        output(i) = hsd(i);
    end
end