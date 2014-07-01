function output = sat(input, bound)

    if input < bound(1)
        output = bound(1);
    elseif input > bound(2)
        output = bound(2);
    else
        output = input;
    end

end
