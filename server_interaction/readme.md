Server_Interaction
==========


# Usage

A FULL EXAMPLE OF USAGE IS FOUND IN THE test_main MISSION UNDER boat_launch. PLEASE refer to this example to see how everything works.

When writing a mission that requires interaction with the server, one must import server interaction by:

    from server_interaction import json_server_proxy

Additionaly an instance of json_server_proxy must be created. 

(We yield to make sure the instance is created)

    s =  yield json_server_proxy.get_server(nh)

Where nh is a node handle 

The .interact()method takes in the server URL/IP address and the course that is being attempted. Example:   

After creating the server instance, initialize interaction like so:

    url_was_set = (yield s.interact('http://url','course')).was_set

    if url_was_set:

        print "Url and course were set succesfully"

        challenge_was_set = (yield s.set_current_challenge('gates')).was_set

        if challenge_was_set:

            print "Challenge was set succesfully"

            run_started = (yield s.start_run()).success

            if run_started:

                print "Run started succesfully"

NOTICE the http:// in the server url. NOTICE the two parameters are strings. (url and course)
 
NOTICE THE SETTING OF THE FIRST CHALLENGE BEFORE STARTING THE RUN. Additionally, every time a new challenge is started the new current challenge needs to be  set as per competition regulations.

    yield s.set_current_challege(challenge)

where challenge is a string with the name of the challenge. As per regulations, the only challenges that matter and must be reported are:
"gates","obstacles","docking","pinger", "interop" or "return"

When doing the start gate challenge, the gate information can be retrieved like so:
(Assuming the server instance is called s)

Assign the previous call to a variable because you will need to get (yield) information from it. Example:

    start_gate_info = s.get_gate_info()

Once you decide you need the information received from the server, yield it:

    start_gate_info = yield start_gate_info

Get the information by creating variables to represent entrance and exit:

    gate_entrance = start_gate_info.entrance
    gate_exit = start_gate_info.exit

When doing the docking bay challenge, the dock information can be retrieved like so:
(Assuming the server instance is called s)

    docking_info = s.get_dock_info()

Assign the previous call to a variable because you will need to get the information from it. Example:
When you're ready to get the information:


    docking_info = yield docking_info

Get the information by creating variables to represent the first dock color, shape, and second dock color, shape:

    dock_one_color = docking_info.first_dock_color 
    dock_one_shape = docking_info.first_dock_symbol
    dock_two_color = docking_info.second_dock_color
    dock_two_shap = docking_info.second_dock_symbol

When doing the interop challenge retrieve the images from the server like so:
(Assuming the server instance is called s)

    images_info = s.get_server_images()

 Assign the previous call to a variable because you will need to get the information from it. Example:
 When you're ready to get the information:
 
    images_info = yield images_info

 Get the information by creating variables to represent the path that the images were saved at and the image count:

    images_path = images_info.file_path
    images_count = images_info.image_count

When reporting an image to the server, do it like so:
(Assuming the server instance is called s)

WARNING: Yield right away only if you are not doing anything else in the background. If the server is slow the mission will halt here until a server response is received

    send_image = yield s.send_image_info('1.JPG','ONE')

where you pass the file name and the identified shape. The previous call will return a boolean based on whether the server returned that the right image was identified. 

Find out whether the server returned that the right image was identified (This is a boolean):

    server_response = send_image.is_right_image

When reporting the pinger buoy, do it like so:
(Assuming the server instance is called s)

WARNING: Yield right away only if you are not doing anything else in the background. If the server is slow the mission will halt here until a server response is received

    send_buoy_color = yield s.send_buoy_info('blue')

Where the argument is the color of the identified buoy. The previous call returns a boolean based on whether the server returned that the right pinger buoy was identified.

Find out whether the server returned that the right pinger buoy was identified (This is a boolean):

    server_response = send_buoy_color.is_right_buoy

When all the missions have been completed, the run needs to be ended like so:

    yield s.end_run()    

# Testing

N/A (Depends on an active server)

# What's in here?

- A class that interacts with the competition server.

# Shutdown procedure

N/A

# TODO


# Issues

- Behavior when bad request is received.
