

points = 20;

portAngles = linspace(-70*pi/180, 10*pi/180,points)';
starboardAngles = -portAngles;

thursters = [linspace(-23, 39,points-1)'; 0];

initialValues = [portAngles thursters starboardAngles thursters];