function plotCostFunction(grid, preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C)

[ii, iii] = size(grid);
v0 = costFunction(grid(1,:), preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C);

output = [grid(1,1);grid(1,2);grid(1,3);grid(1,4)];

figure();

for i = 1:ii
    for j = 1:ii
        for k = 1:ii
            for l = 1:ii
                input = [grid(i,1);grid(j,2);grid(k,3);grid(l,4)];
                v1(l) = costFunction(input,preinput, Fxref, Fyref, Mref, rx1, rx2, ry1, ry2, C);                
            end
            plot(grid(:,4),v1);
            pause(.05);
        end
    end
end

end