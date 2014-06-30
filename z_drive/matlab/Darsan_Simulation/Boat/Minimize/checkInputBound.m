function output = checkInputBound(input,inputBound)
k = [0;0;0;0];
    for i = 1:length(input)
        k(i) = sat(input(i),inputBound(i,:));
    end
    output = k;
end