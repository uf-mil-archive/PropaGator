#!/usr/bin/env python
import rospy
import roslib
roslib.load_manifest('server_interaction')
import os
import unicodedata
import requests
import json
import sys
import time
import math
import shutil
from datetime import datetime
from time import strftime
from collections import OrderedDict
from std_msgs.msg import String
from std_msgs.msg import Bool
from nav_msgs.msg import Odometry
from server_interaction.srv import image_info, image_infoRequest
from server_interaction.srv import buoy_color, buoy_colorRequest
from server_interaction.srv import start_end_run, start_end_runRequest
from server_interaction.srv import docking_bay, docking_bayRequest
from server_interaction.srv import gate_code, gate_codeRequest
from server_interaction.srv import server_images, server_imagesRequest
from server_interaction.srv import challenge, challengeRequest
from server_interaction.srv import url_and_course, url_and_courseRequest
import txros
from txros import util
from twisted.internet import reactor, defer
import signal
import traceback

class json_server(object):

	def __init__(self,node_handle):
		self._node_handle = node_handle

	def _init(self):
		self.dock_service = self._node_handle.get_service_client('docking_bay_information', docking_bay)
		self.gate_service = self._node_handle.get_service_client('gate_code_information',gate_code)
		self.get_images_service = self._node_handle.get_service_client('get_images',server_images)
		self.send_image_service = self._node_handle.get_service_client('send_image', image_info)
		self.send_buoy_service = self._node_handle.get_service_client('send_buoy_color', buoy_color)
		self.run_service = self._node_handle.get_service_client('start_end_run', start_end_run)
		self.challenge_service = self._node_handle.get_service_client('current_challenge', challenge)
		self.url_and_course_service = self._node_handle.get_service_client('url_and_course', url_and_course)
		return self

	@util.cancellableInlineCallbacks
	def interact(self, url_info, course_info):
		response = yield self.url_and_course_service(url_and_courseRequest(url_info,course_info))
		defer.returnValue(response)	

	@util.cancellableInlineCallbacks	
	def get_dock_info(self):
		response = yield self.dock_service(docking_bayRequest(True))
		defer.returnValue(response)
	
	@util.cancellableInlineCallbacks	
	def get_gate_info(self):
		response = yield self.gate_service(gate_codeRequest(True))
		defer.returnValue(response)

	@util.cancellableInlineCallbacks
	def get_server_images(self):
		response = yield self.get_images_service(server_imagesRequest(True))
		defer.returnValue(response)
	
	@util.cancellableInlineCallbacks
	def start_run(self):
		response = yield self.run_service(start_end_runRequest('start'))
		defer.returnValue(response)
		
	@util.cancellableInlineCallbacks
	def end_run(self):
		response = yield self.run_service(start_end_runRequest('end'))
		defer.returnValue(response)

	@util.cancellableInlineCallbacks
	def send_image_info(self,file_name, image_shape):
		response = yield self.send_image_service(image_infoRequest(file_name,image_shape))
		defer.returnValue(response)

	@util.cancellableInlineCallbacks
	def send_buoy_info(self, pinger_buoy_color):
		response = yield self.send_buoy_service(buoy_colorRequest(pinger_buoy_color))	
		defer.returnValue(response)		

	@util.cancellableInlineCallbacks
	def set_current_challenge(self, current_challenge_info):
		response = yield self.challenge_service(challengeRequest(current_challenge_info))
		defer.returnValue(response)

	
_server = None
@util.cancellableInlineCallbacks
def get_server(node_handle):
	global _server
	if _server is None:
		_server = yield json_server(node_handle)._init()
	defer.returnValue(_server)	