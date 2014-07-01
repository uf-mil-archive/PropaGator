function [box] = draw(data)

    box = getBox(data);
    
    drawAnimation(box);

end

function out = getBox(data)
    [x,y,theta] = getXYTheta(data);
    
    for i = 1:length(x)
        box(:,:,i) = translateRotate(x(i),y(i),theta(i));
    end
    
    out = box;
end

function drawAnimation(data)

    for k = 1:length(data)
        x = data(1,:,k);
        y = data(2,:,k);
        
        plot(x,y);
        hold on;
        axis([-5 95 -10 90])
       pause(.1)
    end
    
end

function [x,y,theta] = getXYTheta(data)
    x = data.data(:,1);
    y = data.data(:,2);
    theta = data.data(:,3);
end

%% loop to get every position info for a certain time point
function out = translateRotate(x,y,theta)
    ro = [cosd(theta) sind(theta);-sind(theta) cosd(theta)];
    
    a = [0 1 1 -1 -1 0];
    b = [2 1 -1 -1 1 2];

%     a = [-cosd(45) 0 cosd(45) cosd(45) 0 -cosd(45) -cosd(45) cosd(45)];
%     b = [-cosd(45) 0 -cosd(45) cosd(45) 0 cosd(45) -cosd(45) -cosd(45)];

    rotated = ro*[a;b];
    out = [rotated(1,:)+x;rotated(2,:)+y];
end

% drawnow command 

