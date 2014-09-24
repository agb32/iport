// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

//
// Shows how to use the PvDeviceSerialPort class to communicate with a Pleora GigE Vision or USB3 Vision device.
//

#include <PvSampleUtils.h>
#include <PvDeviceFinderWnd.h>
#include <PvDevice.h>
#include <PvDeviceSerialPort.h>
//#include <PvDeviceAdapter.h>
#include "ebusDeviceAdapter.h"


PV_INIT_SIGNAL_HANDLER();

//added by agb.
#ifdef _UNIX_
#include <ctime>
void Sleep(int a){
  struct timespec t;
  t.tv_sec=a/1000;
  t.tv_sec=(a%1000)*1000000;
  nanosleep(&t,NULL);
}
#endif

//#define UART0
#define BULK0

#define SPEED ( "Baud115200" )
#define STOPBITS ( "One" )
#define PARITY ( "None" )

#define TEST_COUNT ( 16 )

//agb added these...
#define PV_GUI_NOT_AVAILABLE
//
// Shows how to use a PvDeviceSerialPort to write data to an IP engine and continuously read it back
// (with the IP engine serial port set in loop back mode).
//

bool TestSerialCommunications(char *cmd)
{
    PvResult lResult = PvResult::Code::INVALID_PARAMETER;
    const PvDeviceInfo *lDeviceInfo = NULL;

#ifdef PV_GUI_NOT_AVAILABLE
	PvSystem lSystem;
    lDeviceInfo = PvSelectDevice( lSystem );
    if( lDeviceInfo == NULL )
    {
        cout << "No device selected." << endl;
        return false;
    }
#else
	PvDeviceFinderWnd lDeviceFinderWnd;
    lResult = lDeviceFinderWnd.ShowModal();
    if( !lResult.IsOK() )
    {
        cout << "No device selected." << endl;
        return false;
    }
    lDeviceInfo = lDeviceFinderWnd.GetSelected();
#endif // PV_GUI_NOT_AVAILABLE

    // If no device is selected, abort
    if( lDeviceInfo == NULL )
    {
        cout << "No device selected." << endl;
        return false;
	}

    // Connect to the GEV or U3V Device
    cout << "Connecting to " << lDeviceInfo->GetDisplayID().GetAscii() << endl;
	PvDevice* lDevice = PvDevice::CreateAndConnect( lDeviceInfo, &lResult );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to connect to " << lDeviceInfo->GetDisplayID().GetAscii() << endl;
	cout << lResult.GetCodeString().GetAscii() << endl;
	cout << lResult.GetDescription().GetAscii() << endl;
        return false;
    }

	// Create PvDevice adapter
	EbusDeviceAdapter *lDeviceAdapter = new EbusDeviceAdapter( lDevice );

    // Get device parameters need to control streaming
    PvGenParameterArray *lParams = lDevice->GetParameters();

	// Configure serial port - this is done directly on the device GenICam interface, not 
	// on the serial port object! 
#ifdef UART0
	lParams->SetEnumValue( "Uart0BaudRate", SPEED );
	lParams->SetEnumValue( "Uart0NumOfStopBits", STOPBITS );
	lParams->SetEnumValue( "Uart0Parity", PARITY );

	// For this test to work without attached serial hardware we enable the port loop back
	lParams->SetBooleanValue( "Uart0Loopback", true );
#endif
#ifdef BULK0
	lParams->SetEnumValue( "BulkSelector", "Bulk0" );
	lParams->SetEnumValue( "BulkMode", "UART" );
	lParams->SetEnumValue( "BulkBaudRate", SPEED );
	lParams->SetEnumValue( "BulkNumOfStopBits", STOPBITS );
	lParams->SetEnumValue( "BulkParity", PARITY );

	// For this test to work without attached serial hardware we enable the port loop back
	lParams->SetBooleanValue( "BulkLoopback", false );
#endif // BULK0

	// Open serial port
	PvDeviceSerialPort lPort;
#ifdef UART0
	lResult = lPort.Open( lDeviceAdapter, PvDeviceSerial0 );
#endif // UART0
#ifdef BULK0
	lResult = lPort.Open( lDeviceAdapter, PvDeviceSerialBulk0 );
#endif // BULK0
	if ( !lResult.IsOK() )
	{
		cout << "Unable to open serial port on device: " << lResult.GetCodeString().GetAscii() << " " <<  lResult.GetDescription().GetAscii() << endl;
		return false;
	}
	cout << "Serial port opened" << endl;

	// Make sure the PvDeviceSerialPort receive queue is big enough
	lPort.SetRxBufferSize( 32 );

    uint8_t *lInBuffer = NULL;
    uint8_t *lOutBuffer = NULL;

    uint32_t lSize = strlen(cmd)+2;
    // Allocate test buffers
    lInBuffer = new uint8_t[ lSize ];
    lOutBuffer = new uint8_t[ lSize ];
    
    // Fill input buffer
    for ( uint32_t i = 0; i < lSize-2; i++ )
      {
	lInBuffer[ i ] = cmd[i];//static_cast<uint8_t>( rand() % 256 );
      }
    lInBuffer[lSize-2]='\r';
    lInBuffer[lSize-1]='\n';
    
    // Send the buffer content on the serial port
    uint32_t lBytesWritten = 0;
    lResult = lPort.Write( lInBuffer, lSize, lBytesWritten );
    if ( !lResult.IsOK() )
      {
	// Unable to send data over serial port!
	cout << "Error sending data over the serial port: " << lResult.GetCodeString().GetAscii() << " " <<  lResult.GetDescription().GetAscii() << endl;
	return -1;
      }
    
    cout << "Sent " << lBytesWritten << " bytes through the serial port" << endl;
    
    Sleep(100);
    //
    // Wait until we have all the bytes or we timeout. The Read method only times out
    // if no data is available when the function call occurs, otherwise it returns
    // all the currently available data. It is possible we have to call Read multiple
    // times to retrieve all the data if all the expected data hasn't been received yet.
    //
    // Your own code driving a serial protocol should check for a message being complete,
    // whether it is on some sort of EOF or length. You should pump out data until you
    // have what you are waiting for or reach some timeout.
    //
    cout << "Reading bytes" <<endl;
    uint32_t lTotalBytesRead = 0;
    while ( lTotalBytesRead < lBytesWritten )
      {
	uint32_t lBytesRead = 0;
	lResult = lPort.Read( lOutBuffer + lTotalBytesRead, lSize - lTotalBytesRead, lBytesRead, 100 );
	if ( lResult.GetCode() == PvResult::Code::TIMEOUT )
	  {
	    cout << "Timeout" << endl;
	    break;
	  }
	
	// Increments read head
	lTotalBytesRead += lBytesRead;
	Sleep(100);
      }
    
    cout <<"Sent:" << lBytesWritten << " Received:" <<lTotalBytesRead <<endl;
    // Validate answer
    if ( lTotalBytesRead != lBytesWritten )
      {
	// Did not receive all expected bytes
	cout << "Only received " << lTotalBytesRead << " out of " << lBytesWritten << " bytes" << endl;
      }
    else
      {
	// Compare input and output buffers
	uint32_t lErrorCount = 0;
	for ( uint32_t i = 0; i < lBytesWritten; i++ )
	  {
	    if ( lInBuffer[ i ] != lOutBuffer[ i ] )
	      {
		lErrorCount++;
	      }
	  }
	
	// Display error count
	cout << "Error count: " << lErrorCount << endl;
      }
    
    delete []lInBuffer; 
    lInBuffer = NULL;
    
    delete []lOutBuffer; 
    lOutBuffer = NULL;
    
    
    cout << endl;
    
    
    if ( lInBuffer != NULL )
      {
        delete []lInBuffer;
        lInBuffer = NULL;
      }
    
    if ( lOutBuffer != NULL )
      {
        delete []lOutBuffer;
        lOutBuffer = NULL;
      }
    
    // Close serial port
    lPort.Close();
    cout << "Serial port closed" << endl;
    
    // Delete device adapter (before freeing PvDevice!)
    delete lDeviceAdapter;
    lDeviceAdapter = NULL;
    
    // Finally release the device. Use PvDevice::Free as the device was allocated using PvDevice::CreateAndConnect.
    cout << "Disconnecting and freeing device" << endl;
    PvDevice::Free( lDevice );
    
    return true;
}


//
// Main function
//

int main(int argc, char **argv)
{
  //PV_SAMPLE_INIT();//agb removed
  char *cmd;
    // PvDeviceSerialPort used to perform serial communication through a Pleora GigE Vision or USB3 Vision device
  if(argc>1){
    cmd=argv[1];
    
    cout << "iportSerialWrite:"<<cmd << endl << endl;
    TestSerialCommunications(cmd);
    
    //cout << endl << "<press a key to exit>" << endl;
    //PvWaitForKeyPress();
  }else{
    cout << "No command provided"<<endl;
  }
  //PV_SAMPLE_TERMINATE();//agb removed

    return 0;
}

