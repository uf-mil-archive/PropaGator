Server_Interaction
==========


# Usage

When writing a mission that requires interaction with the server, one must import server interaction by:

    from server_interaction import json_server

Additionaly an instance of server_interaction must be created. The constructor takes in the server URL/IP address and the course that is being attempted. Example:

    s = json_server.server_interaction('http://server-address','courseA')

NOTICE THE HTTP:// in the previous implementation

If you are doing this from the main mission, a run must be started as per competition regulations, do this lik so:
(Assuming the server interaction instance is called s)

    s.set_current_challenge('gates') 

    s.start_run()

The previous call to .start_run() will return a boolean in case it wants to be used. The boolean is based on the server response i.e. true if the run is authorized, false otherwise.    

NOTICE THE SETTING OF THE FIRST CHALLENGE BEFORE STARTING THE RUN. Additionally, every time a new challenge is started the new current challenge needs to be  set as per competition regulations.

    s.set_current_challege(challenge)

where challenge is a string with the name of the challenge. As per regulations, the only challenges that matter and must be reported are:
"gates","obstacles","docking","pinger", "interop" or "return"

When doing the start gate challenge, the gate information can be retrieved like so:
(Assuming the server instance is called s)

    s.get_gate_info()

Assign the previous call to a variable because you will need to get the information from it. Example:

    start_gate_info = s.get_gate_info()

Get the information by creating variables to represent entrance and exit:

    gate_entrance = start_gate_info.entrance
    gate_exit = start_gate_info.exit

When doing the docking bay challenge, the dock information can be retreived like so:
(Assuming the server instance is called s)

    s.get_dock_info()

Assign the previous call to a variable because you will need to get the information from it. Example:

    docking_info = s.get_dock_info()

Get the information by creating variables to represent the first dock color, shape, and second dock color, shape:

    dock_one_color = docking_info.first_dock_color 
    dock_one_shape = docking_info.first_dock_symbol
    dock_two_color = docking_info.second_dock_color
    dock_two_shap = docking_info.second_dock_symbol

When doing the interop challenge retreive the images from the server like so:
(Assuming the server instance is called s)

    s.get_server_images()

 Assign the previous call to a variable because you will need to get the information from it. Example:
 
    images_info = s.get_server_images()

 Get the information by creating variables to represent the path that the images where saved at and the image count:

    images_path = images_info.file_path
    images_count = images_info.image_count

When reporting and image to the service, do it like so:
(Assuming the server instance is called s)

    s.send_image_info('1.JPG','ONE')

where you pass the file name and the identified shape. The previous called will return a boolean based on whether the server returned that the right image was identified. 

When reporting the pinger buoy, do it like so:
(Assuming the server instance is called s)

    s.send_buoy_info('blue')

Where the argument is the color of the identified buoy. The previous called returns a boolean based on whether the server returned that the right pinger buoy was identified.

When all the missions have been completed, the run needs to be ended like so:

    s.end_run()    

# Testing

N/A (Depends on an active server)

# What's in here?

- A class that interacts with the competition server.

# Shutdown procedure

N/A

# TODO


# Issues

- Behavior when bad request is received.
