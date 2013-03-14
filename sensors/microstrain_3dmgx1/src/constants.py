
#most recently read values from IMU
ACCELGAIN = 8500
GYROGAIN = 10000
MAGGAIN = 2000


#command legend:

#0x01 = send raw data from sensors
sensors_raw = 0x01
sensors_raw_bytes = 23

#0x02 = send gyro-stabilized vectors
stabalized_vectors = 0x02
stabalized_vectors_bytes = 23

#0x03 = send instantaneous vectors
inst_vectors = 0x03
inst_vectors_bytes = 23

#0x04 = send instantaneous quaternion
inst_quarternion = 0x04
inst_quarternion_bytes = 13

#0x05 = send gyro-stabilized quaternion
stabalized_quarternion = 0x05
stabalized_quarternion_bytes = 13 

#0x06 = send capture gyro bias 
gyro_bias = 0x06
gyro_bias_bytes = 5

#0x07 = send temperature
temp = 0x07
temp_bytes = 7

#0x08 = read EEPROM value
read_EEPROM_cmd = 0x08
#0x09 = write EEPROM value
write_EEPROM_cmd = 0x09
	#-addr 230 = AccelGainScale
AccelGainScale = 230
	#-addr 130 = GyroGainScale
GyroGainScale = 130
	#-addr 232 = MagGainScale
MagGainScale = 232
	


#0x0F = tare coordinate system
tare_coord_sys = 0x0F
