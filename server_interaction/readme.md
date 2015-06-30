Server_Interaction
==========

Takes in the server URL and the course that we are attempting and 
handles requests to the necessary links.

# Notes

The url scanner MUST be ran prior to the launch file with the nodes.

# Usage

    rosrun server_interaction url_scanner.py

This will prompt you to input the url.
"http://some-url/some-link" or "some-url/some-link"
This also scans the course that is being attempted. This will be either "courseA", "courseB" or "openTest" without quotes.
    
    roslaunch server_interaction json_services.launch
    roslaunch server_interaction server_interaction.launch

The previous launch files launch all the files pertaining server interaction, including heartbeat and the services for reporting information.

Information on how to use the services is commented out in the very client scripts called pinger_buoy_color_client.py and image_info_client.py

In essence all that needs to be done is to copy the code into your script and pass the paramenters onto the service handler.

# Testing

N/A (Depends on an active server)

# What's in here?

- Nodes that interact with the competition server.
- Two services that are used to send the pinger buoy information and
  information pertaining the received images.

# Shutdown procedure

N/A (Ctrl+c) will kill the nodes.

# TODO

- Implement the structure in a better manner.

# Issues

- Behavior when bad request is received is unknow.
