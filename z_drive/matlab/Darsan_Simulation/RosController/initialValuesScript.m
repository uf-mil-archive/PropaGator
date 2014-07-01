portAngles = linspace(-70*pi/180, 10*pi/180,20)';
starboardAngles = -portAngles;

thursters = [linspace(-23, 39,19)'; 0];

initialValues = [portAngles thursters starboardAngles thursters];