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
from server_interaction.srv import image_info
from server_interaction.srv import buoy_color
from server_interaction.srv import start_end_run
from server_interaction.srv import docking_bay
from server_interaction.srv import gate_code
from server_interaction.srv import server_images

class server_interaction:
	url = None
	course = None
	challenge = None
	running = None
	x = None
	y = None
	z = None

	def __init__(self, url, course):
		self.url = url
		self.course = course
		rospy.init_node('server_interaction')
		self.dock_service = rospy.Service('docking_bay_information',docking_bay,self.dock_callback)
		self.gate_service = rospy.Service('gate_code_information',gate_code,self.gate_callback)
		self.get_images_service = rospy.Service('get_images',server_images,self.images_callback)
		self.send_image_service = rospy.Service('send_image', image_info ,self.send_image_callback)
		self.send_buoy_service = rospy.Service('send_buoy_color', buoy_color, self.send_buoy_callback)
		self.run_service = rospy.Service('start_end_run', start_end_run, self.run_callback)
		self.gps_subscriber = rospy.Subscriber('absodom', Odometry, self.gps_callback)
		self.callback_timer = rospy.Timer(rospy.Duration(1),self.heartbeat)

	def get_docking_bay(self):
		link = '/automatedDocking/%s/UF' %self.course
		url = self.url+link
		r = requests.get(url, verify = False)
		json_info = r.json()['dockingBaySequence']
		firstDockInfo = json_info[0]
		secondDockInfo = json_info[1]
		firstDockSymbol = firstDockInfo['symbol']				
		firstDockColor = firstDockInfo['color']				
		secondDockSymbol = secondDockInfo['symbol']							
		secondDockColor =  secondDockInfo['color']
		######## docking bay information ##########
		return (firstDockSymbol,firstDockColor, secondDockSymbol, secondDockColor)
	def dock_callback(self,request_info):
		return self.get_docking_bay()

	def get_gates(self):
		link = '/obstacleAvoidance/%s/UF' %self.course
		url = self.url+link
		r = requests.get(url, verify = False)
		json_info = r.json()['gateCode']
		gate_code = json_info.split(",")
		entrance = gate_code[0].replace("(","")
		exit = gate_code[1].replace(")","")
		######### gate code information #######
		return (entrance, exit)
	def gate_callback(self,request_info):
		return self.get_gates()

	def get_images(self):
		def my_range(start, end, step):
		    while start < end:
		        yield start
		        start += step

		link = '/interop/images/%s/UF' %self.course	
		url = self.url+link
		headers = {'Content-Type':'text/html'}
		r = requests.get(url, headers = headers, verify = False)
		html_info = r.text
		links = html_info.split('"')
		links2 = []
		imageNames = []
		counter = -1
		for index in my_range(0, len(links), 1):
		    if index % 2 != 0:
		    	links2.append(links[index])
		for sublink in links2:
			imageNames.append(sublink.split("/")[5])
		global imgCount
		global path
		imgCount = 0					
		for sublink in links2:				
			requestLink = self.url + sublink
			counter = counter + 1
			imageName = imageNames[counter]
			print "Dowloading: %s" %imageName
			#after having parsed the html that the server returned
			#this generates as many requests as links the server provides
			#and saves the images to ~/output/ServerImages/					
			time.sleep(1)
			r = requests.get(requestLink, stream = True)
			path = os.path.join(os.path.expanduser('~'), 'output', 'ServerImages/')
			if os.path.isdir(path):
				print("Saving image on %s") %str(path)
			else:
				os.makedirs(path)
			time.sleep(1)						
			with open(path+imageName,'wb') as out_file:					
				shutil.copyfileobj(r.raw, out_file)					
			del r
			imgCount = imgCount + 1
		path = str(path)		
		count = int(imgCount/2)
		return path, count
	def images_callback(self,request_info):
		return self.get_images()

	def send_image_callback(self,request_info):
		imageName = request_info.file_name
		shape = request_info.image_shape
		postImageLink = '/interop/image/%s/UF' %self.course
		sendImageInfoLink = '/interop/report/%s/UF' %self.course
		#creating urls
		putImageOnServerUrl = self.url +  postImageLink
		sendImageInfoUrl = self.url +  sendImageInfoLink
		###################################### put image on server #############################################
		global path
		#find image on path where it is stored
		path = os.path.join(os.path.expanduser('~'), 'output', 'ServerImages', imageName)
		files = {'file': (imageName, open(path, 'rb'), 'multipart/mixed', {'Expires': '0'})}
		#Send image to server and get an image ID.
		#create request #1 (POST), put image file on server
		request1 = requests.post(putImageOnServerUrl, files=files, verify=False)
		print (" ")
		print "Information returned by server: "
		print request1.text
		print (" ")
		#getting image ID from server. Will be a json structure like:
		#{"imaeId":"a4aa8224-07f2-4b57-a03a-c8887c2505c7"}
		# wait one second... just for the heck of it
		time.sleep(1)
		imageID = request1.json()['imageId']
		######################################## send image information #########################################				
		#ready payload to send to server..
		imageID = unicodedata.normalize('NFKD', imageID).encode('ascii','ignore')
		headers = {'Content-Type':'application/json'}
		print "Payload being sent to server: "
		print json.dumps(OrderedDict([("course",self.course),("team","UF"),("shape",shape),("imageId",imageID)]))
		print (" ")
		payload = OrderedDict([("course",self.course),("team","UF"),("shape",shape),("imageId",imageID)])
		#create request #2, post image info json structure
		time.sleep(1)
		try:
			request2 = requests.post(sendImageInfoUrl, headers = headers, data = json.dumps(payload), verify = False)
			time.sleep(1)
			print "Request status code: "
			if request2.status_code == 200:
				print "\033[0;32m%s\033[0m" %(request2.status_code)
				print (" ")
			else:
				print "\033[0;31m%s\033[0m" %(request2.status_code)
				print (" ")	
			print "Information returned by sever: "
			print request2.text
			if request2.status_code == 200:
				#decode json response from server
				status = request2.json()['success']
				print (" ")
				if status == True:
					print "Is this the right image?"
					print "\033[0;32m%s\033[0m" %status
					return True
				else:
					print "Is this the right image?"
					print "\033[0;31m%s\033[0m" %status
					return False
			else:
				raise rospy.ServiceException		
		except rospy.ServiceException:			
			print "Server call failed"	
	def send_buoy_callback(self, request_info):
		color = request_info.buoy_color
		reportColorLink = '/pinger/%s/UF' %self.course
		fullLink = self.url + reportColorLink
		payload = OrderedDict([("course",self.course),("team","UF"),("buoyColor",color)])
		print"Payload being sent to server: "
		print json.dumps(payload)
		print (" ")
		headers = {'content-type': 'application/json'}
		try:
			#create request objet: post link is "fullLink", headers, data is the json string.	
			r = requests.post(fullLink, headers=headers, data=json.dumps(payload), verify=False)
			print "Information received from server: "
			print r.text
			print (" ")
			if(r.status_code == 200): 
				success = r.json()
				#if the right buoy color was sent, server return a json structure: {"success":"true"}.. else false
				if(success['success'] == True):			
				# return status from server to tell whether the right buoy was identified or not
					print "Was the right buoy identified?"
					print "\033[0;32m%s\033[0m" %success['success']
					return True			
				else:
					print "Was the right buoy identified?"
					print "\033[0;31m%s\033[0m" %success['success']				
					return False		
			else:			
				raise rospy.ServiceException	
		except rospy.ServiceException:			
				print "Server call failed"

	def run_callback(self,request_info):
		start_or_end = request_info.status
		startLink = '/run/start/%s/UF' %self.course
		endLink = '/run/end/%s/UF' %self.course
		headers = {'Content-Type':'application/json'}
		if start_or_end == "start":
			print "Starting run..."
			print (" ")
			fullLink = self.url + startLink
			print "URl that request is being sent to: "
			print fullLink
			print (" ")
			r = requests.post(fullLink, headers = headers, verify = False)
			print "Request status code:"
			if r.status_code == 200:
				self.running =  True
				print "\033[0;32m%s\033[0m" %r.status_code
				print(" ")
			else:
				print "\033[0;31m%s\033[0m" %r.status_code
				print(" ")
			print "Information returned by sever: "		
			print r.text
			print (" ")
			print("Did the server authorize the run to start?")
			if r.json()["success"] == True:
				print "\033[0;32m%s\033[0m" %r.json()["success"]
				return True
			else:
				print "\033[0;31m%s\033[0m" %r.json()["success"]
				return False	
		if start_or_end== "end":
			print "Ending run..."
			print (" ")
			print "URl that request is being sent to: "
			fullLink = self.url + endLink
			print fullLink
			print (" ")
			r = requests.post(fullLink, headers = headers, verify = False)
			print "Request status code:"
			if r.status_code == 200:
				self.running = False
				print "\033[0;32m%s\033[0m" %r.status_code
				print(" ")
			else:
				print "\033[0;31m%s\033[0m" %r.status_code
				print(" ")
			print "Information retuned by server: "	
			print r.text
			print (" ")
			print("Did the server authorize the run to end?")
			if r.json()["success"] == True:
				print "\033[0;32m%s\033[0m" %r.json()["success"]
				return True	
			else:
				print "\033[0;31m%s\033[0m" %r.json()["success"]
				return False	
	def set_current_challenge(self, current_challenge_info):
		self.challenge = current_challenge_info
		print "Challenge was set to: %s" %self.challenge
		return True	

	def get_dock_info(self):
		rospy.wait_for_service('docking_bay_information')
		get_dock = rospy.ServiceProxy('docking_bay_information', docking_bay)
		docking_info = get_dock(True)
		return docking_info

	def get_gate_info(self):
		rospy.wait_for_service('gate_code_information')	
		get_gate = rospy.ServiceProxy('gate_code_information', gate_code)
		gate_code_info = get_gate(True)
		return gate_code_info

	def get_server_images(self):
		rospy.wait_for_service('get_images')
		get_images = rospy.ServiceProxy('get_images', server_images)
		images_info = get_images(True)
		return images_info

	def start_run(self):
		rospy.wait_for_service('start_end_run')
		start_run = rospy.ServiceProxy('start_end_run', start_end_run)
		run_info = start_run('start')
		return run_info.success	

	def end_run(self):
		rospy.wait_for_service('start_end_run')
		end_run = rospy.ServiceProxy('start_end_run', start_end_run)
		run_info = end_run('end')
		return run_info.success

	def send_image_info(self,file_name, image_shape):
		rospy.wait_for_service('send_image')
		send_image = rospy.ServiceProxy('send_image', image_info)
		is_right_image = send_image(file_name,image_shape)
		return is_right_image.is_right_image

	def send_buoy_info(self, pinger_buoy_color):
		rospy.wait_for_service('send_buoy_color')
		send_buoy = rospy.ServiceProxy('send_buoy_color', buoy_color)
		is_right_buoy = send_buoy(pinger_buoy_color)
		return is_right_buoy.is_right_buoy

	def gps_callback(self, gps_pos):

		self.x = gps_pos.pose.pose.position.x
		self.y = gps_pos.pose.pose.position.y
		self.z = gps_pos.pose.pose.position.z

	def heartbeat(self,event):	
		def ecef_to_lla(ecef):
			#earths's radius in meters
			a = 6378137 
			#eccentricity
			e = 8.1819190842622e-2  
			asq = math.pow(a,2)
			esq = math.pow(e,2)
			x = ecef[0]
			y = ecef[1]
			z = ecef[2]
			b = math.sqrt( asq * (1-esq) )
			bsq = math.pow(b,2)
			ep = math.sqrt( (asq - bsq)/bsq)
			p = math.sqrt( math.pow(x,2) + math.pow(y,2) )
			th = math.atan2(a*z, b*p)
			lon = math.atan2(y,x)
			lat = math.atan2( (z + math.pow(ep,2)*b*math.pow(math.sin(th),3) ), (p - esq*a*math.pow(math.cos(th),3)) )
			N = a/( math.sqrt(1-esq*math.pow(math.sin(lat),2)) )
			alt = p / math.cos(lat) - N
			#mod lat to keep it between 0 and 2 pi
			lon = lon % (2*math.pi)
			#changing radians to degrees
			lat = math.degrees(lat)
			lon = math.degrees(lon)
			#normalizing angle
			lat = normalizeAngle(lat)
			lon = normalizeAngle(lon)
			ret = (lat, lon)
			return ret;
		def normalizeAngle(angle):
			#normalize angle to keep it between -179 and 180 degrees
			new_angle = angle	
			while new_angle <= -180: 
				new_angle += 360	
			while new_angle > 180: 
				new_angle -= 360	
			return new_angle;
		if self.running:
			ecef = (self.x,self.y,self.z)
			gps_data = ecef_to_lla(ecef)
			timeStamp = datetime.utcnow()
			timeStamp = timeStamp.strftime('%Y%m%d%H%M%S')
			latitude = str(gps_data[0])
			longitude = str(gps_data[1])
			latitude = latitude[0:9]
			longitude = longitude[0:10]	
			sublinkMain = '/heartbeat/%s/UF' %self.course
			url = self.url +  sublinkMain
			#payload to send to server
			position = OrderedDict([("datum","WGS84"),("latitude",latitude),("longitude",longitude)])
			payload = OrderedDict([("timestamp",timeStamp),("challenge",self.challenge),("position",position)])
			heartbeat_pub = rospy.Publisher('gps_heartbeat', String, queue_size=10)
			headers = {'content-type': 'application/json'}
			print "Heartbeat payload sent to server: "
			print json.dumps(payload)					
			r = requests.post(url, headers = headers, data = json.dumps(payload), verify = False)
			print "Server return:"
			print r.text
