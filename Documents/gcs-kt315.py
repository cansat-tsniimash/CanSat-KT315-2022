import sys
import argparse
import time, datetime
import struct
import datetime

from RF24 import RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
from RF24 import RF24_1MBPS, RF24_250KBPS, RF24_2MBPS
from RF24 import RF24_CRC_16, RF24_CRC_8, RF24_CRC_DISABLED
from RF24 import RF24 as RF24_CLASS


radio=RF24_CLASS(24, 1)


def generate_logfile_name(packet_type):
	now = datetime.datetime.utcnow().replace(microsecond=0)
	isostring = now.isoformat()  # string 2021-04-27T23:17:31
	isostring = isostring.replace('-', '')  # string 20210427T23:17:31
	isostring = isostring.replace(':', '')  # string 20210427T231731
	if packet_type == 0:
		packet_type_name = 'dosimeter.csv'
	elif packet_type == 1:
		packet_type_name = 'bmp.csv'
	elif packet_type == 2:
		packet_type_name = 'ds.csv'
	elif packet_type == 3:
		packet_type_name = 'gps.csv'
	elif packet_type == 4:
		packet_type_name = 'inertial.csv'
	elif packet_type == 5:
		packet_type_name = 'sebastian.csv'
	elif packet_type == -1:
		packet_type_name = 'all.bin'
	return f'kt315_telemetry_{isostring}_{packet_type_name}'


if __name__ == '__main__':
	static_payload_size = None


	radio.begin()

	radio.openReadingPipe(1, b'\xac\xac\xac\xac\xac')

	radio.setCRCLength(RF24_CRC_DISABLED)
	radio.setAddressWidth(5)
	radio.channel = 115
	radio.setDataRate(RF24_250KBPS)

	radio.setAutoAck(True)



	radio.enableAckPayload()
	radio.enableDynamicAck()

	if static_payload_size is not None:
		radio.disableDynamicPayloads()
		radio.payloadSize = static_payload_size
	else:
		radio.enableDynamicPayloads()

	radio.startListening()
	radio.printDetails()

	filename = generate_logfile_name(-1)
	filename_dosimeter = generate_logfile_name(0)
	filename_bmp = generate_logfile_name(1)
	filename_ds = generate_logfile_name(2)
	filename_gps = generate_logfile_name(3)
	filename_inertial = generate_logfile_name(4)
	filename_sebastian = generate_logfile_name(5)

	file = open(filename, 'wb')
	file_dosimeter = open(filename_dosimeter, 'wb')
	file_dosimeter.write(b'ticks per last second;ticks sum\n')
	file_dosimeter.flush()
	file_bmp = open(filename_bmp, 'wb')
	file_bmp.write(b'bmp_temperature;bmp_pressure\n')
	file_bmp.flush()
	file_ds = open(filename_ds, 'wb')
	file_ds.write(b'ds_temperature;status\n')
	file_ds.flush()
	file_gps = open(filename_gps, 'wb')
	file_gps.write(b'longtitude;latitude;altitude;time_sec;time_microsec;fix\n')
	file_gps.flush()
	file_inertial = open(filename_inertial, 'wb')
	file_inertial.write(b'lsm_acc_x;lsm_acc_y;lsm_acc_z;lsm_gyro_x;lsm_gyro_y;lsm_gyro_z;lis_mag_x;lis_mag_y;lis_mag_z;lux\n')
	file_inertial.flush()
	file_sebastian = open(filename_sebastian, 'wb')
	file_sebastian.write(b'quaternion 1;quaternion 2;quaternion 3;quaternion 4\n')
	file_sebastian.flush()


	COUNTER = 0
	LOSS = 0
	PREV_PACKET_NUMBER = None
	while True:
		has_payload, pipe_number = radio.available_pipe()
		if has_payload:
			payload_size = static_payload_size
			if payload_size is None:
				payload_size = radio.getDynamicPayloadSize()
				print(payload_size)

			package = radio.read(payload_size)
			print(f'got data {package}')
			packet_size = len(package)
			biter = struct.pack('>B', packet_size)
			record = biter + package
			
			package_service = package[:7]
			package_crc = package[len(package) - 2:]
			package_data = package[7:len(package) - 2]
			try:
				unpacked_service = struct.unpack('<BHL', package_service)
			except Exception as e:
				print(f'{e}\ndata received: {len(package)} bytes')
			else:
				try:
					unpacked_crc = struct.unpack('<H', package_crc)
				except Exception as e:
					print(f'{e}\ndata received: {len(package)} bytes')
				else:
					if unpacked_service[0] == 0:
						try:
							unpacked_data = struct.unpack('<LL', package)
						except Exception as e:
							print(f'{e}\ndata received: {len(package)} bytes')
						else:
							ticks_per_last_second = unpacked_data[0]
							ticks_sum = unpacked_data[1]
							file_dosimeter.write(bytes(f'{ticks_per_last_second};{ticks_sum}\n', 'utf-8'))
							file_dosimeter.flush()

					elif unpacked_service[0] == 1:
						try:
							unpacked_data = struct.unpack('<hL', package)
						except Exception as e:
							print(f'{e}\ndata received: {len(package)} bytes')
						else:
							bmp_temperature = unpacked_data[0]
							bmp_pressure = unpacked_data[1]
							file_bmp.write(bytes(f'{bmp_temperature};{bmp_pressure}\n', 'utf-8'))
							file_bmp.flush()

					elif unpacked_service[0] == 2:
						try:
							unpacked_data = struct.unpack('<fB', package)
						except Exception as e:
							print(f'{e}\ndata received: {len(package)} bytes')
						else:
							ds_temperature = unpacked_data[0]
							status = unpacked_data[1]
							file_ds.write(bytes(f'{ds_temperature};{status}\n', 'utf-8'))
							file_ds.flush()

					elif unpacked_service[0] == 3:
						try:
							unpacked_data = struct.unpack('<ffhLLB', package)
						except Exception as e:
							print(f'{e}\ndata received: {len(package)} bytes')
						else:
							longtitude = unpacked_data[0]
							latitude = unpacked_data[1]
							altitude = unpacked_data[2]
							time_sec = unpacked_data[3]
							time_microsec = unpacked_data[4]
							fix = unpacked_data[5]
							file_gps.write(bytes(f'{longtitude};{latitude};{altitude};{time_sec};{time_microsec};{fix}\n', 'utf-8'))
							file_gps.flush()

					elif unpacked_service[0] == 4:
						try:
							unpacked_data = struct.unpack('<hhhhhhhhhf', package)
						except Exception as e:
							print(f'{e}\ndata received: {len(package)} bytes')
						else:
							lsm_acc = unpacked_data[:3]
							lsm_gyro = unpacked_data[3:6]
							lis_mag = unpacked_data[6:9]
							lux = unpacked_data[9]
							file_inertial.write(bytes(f'{lsm_acc[0]};{lsm_acc[1]};{lsm_acc[2]};{lsm_gyro[0]};{lsm_gyro[1]};{lsm_gyro[2]};{lis_mag[0]};{lis_mag[1]};{lis_mag[2]};{lux}\n', 'utf-8'))
							file_inertial.flush()

					elif unpacked_service[0] == 5:
						try:
							unpacked_data = struct.unpack('<ffff', package)
						except Exception as e:
							print(f'{e}\ndata received: {len(package)} bytes')
						else:
							quaternion = unpacked_data
							file_sebastian.write(bytes(f'{quaternion[0]};{quaternion[1]};{quaternion[2]};{quaternion[3]}\n', 'utf-8'))
							file_sebastian.flush()

					if PREV_PACKET_NUMBER is not None:
						if (PREV_PACKET_NUMBER + 1) & 0xFFFF != num:
							LOSS += 1
						PREV_PACKET_NUMBER = num
					COUNTER += 1


				print(f'LOSS = {LOSS}, COUNTER = {COUNTER}, LOSS PERCENT = {LOSS / COUNTER * 100}%')

			file.write(record)
			file.flush()
		else:
			pass

		time.sleep(0.005)