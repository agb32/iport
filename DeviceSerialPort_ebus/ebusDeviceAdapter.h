#include "PvDevice.h"
#include <IPvDeviceAdapter.h>
#include "PvDeviceSerialPort.h"
#include <list>

class IPvDeviceEventSinkList : public std::list<IPvDeviceEventSink *>
{
};

class EbusDeviceAdapter : public IPvDeviceAdapter, PvDeviceEventSink, PvGenEventSink
{
private:
   IPvDeviceEventSinkList * mEventSinkList;
   PvDevice * mDevice;

public:

    EbusDeviceAdapter(PvDevice *aDevice);
    virtual ~EbusDeviceAdapter();

    bool IsGenIntegerInNodeMap( const PvString &aParameterName );
    bool IsGenEnumInNodeMap( const PvString &aParameterName );
    bool IsGenRegisterInNodeMap( const PvString &aParameterName );
    bool IsGenReadable( const PvString &aParameterName );
    bool IsGenWritable( const PvString &aParameterName );
    bool IsGenEnumEntryAvailable( const PvString &aParameterName, const PvString &aEnumEntry );

    PvResult GetGenIntegerValue( const PvString &aParameterName, int64_t &aValue );
    PvResult GetGenEnumEntriesAvailable( const PvString &aParameterName, PvStringList &aList );
    PvResult GetGenEnumValue( const PvString &aParameterName, PvString &aEnumEntry );
    PvResult SetGenEnumValue( const PvString &aParameterName, const PvString &aEnumEntry );
    PvResult GetGenStringValue( const PvString &aParameterName, PvString &aValue );
    PvResult GetGenRegisterLength( const PvString &aParameterName, int64_t &aLength );
    PvResult GetGenRegisterData( const PvString &aParameterName, uint8_t *aDataBuffer, int64_t aByteCount );
    PvResult SetGenRegisterData( const PvString &aParameterName, const uint8_t *aDataBuffer, int64_t aByteCount );

    PvResult RegisterGenInvalidator( const PvString &aParameterName );
    PvResult UnregisterGenInvalidator( const PvString &aParameterName );

    PvResult WriteRegister( int64_t aAddress, uint32_t aValue );
    PvResult ReadRegister( int64_t aAddress, uint32_t &aValue );
	 PvResult WriteMemory( const uint8_t *aBuffer, int64_t aAddress, int64_t aLength );
	 PvResult ReadMemory( uint8_t *aBuffer, int64_t aAddress, int64_t aLength );

    PvResult WaitForMessagingChannelIdle( uint32_t aTimeout );

    bool IsConnected();
    bool IsPleoraPowered();
    bool IsGigEVision();
    bool IsUSB3Vision();

    PvResult RegisterEventSink( IPvDeviceEventSink *aEventSink );
    PvResult UnregisterEventSink( IPvDeviceEventSink *aEventSink );

    void OnParameterUpdate( PvGenParameter *aParameter );
    //    void EbusDeviceAdapter::OnEvent( PvDevice *aDevice, //agb changed
    void OnEvent( PvDevice *aDevice, 
       uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID, uint64_t aTimestamp, 
      const void *aData, uint32_t aDataLength );

};
