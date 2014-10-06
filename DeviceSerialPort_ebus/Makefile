install:
	cp iportSerial.py /Canary/bin/

iportSerialWrite: iportSerialWrite.cpp eBusDeviceAdapter.cpp ebusDeviceAdapter.h
	g++ -D_UNIX_ iportSerialWrite.cpp eBusDeviceAdapter.cpp -o iportSerialWrite -I/opt/pleora/ebus_sdk/Ubuntu-12.04-x86_64/include -L/opt/pleora/ebus_sdk/Ubuntu-12.04-x86_64/lib -L/opt/pleora/ebus_sdk/RHEL-6-x86_64/lib -L/usr/lib64 -I/opt/pleora/ebus_sdk/RHEL-6-x86_64/include -lPvBase -lPtUtilsLib -lPvDevice -lPvStream -lPvGenICam -lPvSerial -lPvGUI -ludev



DeviceSerialPort: DeviceSerialPort.cpp eBusDeviceAdapter.cpp ebusDeviceAdapter.h
	g++ -D_UNIX_ DeviceSerialPort.cpp eBusDeviceAdapter.cpp -o DeviceSerialPort -I/opt/pleora/ebus_sdk/Ubuntu-12.04-x86_64/include -L/opt/pleora/ebus_sdk/Ubuntu-12.04-x86_64/lib -lPvBase -lPtUtilsLib -lPvDevice -lPvStream -lPvGenICam -lPvSerial -lPvGUI -Wall
