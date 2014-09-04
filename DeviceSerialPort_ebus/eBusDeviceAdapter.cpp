// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************


#include <PvDeviceGEV.h>
#include <PvDeviceU3V.h>

#include <list>
#include "ebusDeviceAdapter.h"//agb changed
#include <PvSampleUtils.h>

///
/// \brief Constructor.
///

EbusDeviceAdapter::EbusDeviceAdapter( PvDevice *aDevice )
    : mDevice( aDevice )
{
    mEventSinkList = new IPvDeviceEventSinkList;
    mDevice->RegisterEventSink( this );
}


///
/// \brief Destructor.
///

EbusDeviceAdapter::~EbusDeviceAdapter()
{
    mDevice->UnregisterEventSink( this );
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter to test for.
///
/// \return True if the parameter is present, false if not.
///

bool EbusDeviceAdapter::IsGenIntegerInNodeMap( const PvString &aParameterName )
{
    PvGenInteger *lParameter = mDevice->GetParameters()->GetInteger( aParameterName );
    cout << "IsGenIntegerInNodeMap" << endl;
    return ( lParameter != NULL );
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter to test for.
///
/// \return True if the parameter is present, false if not.
///

bool EbusDeviceAdapter::IsGenEnumInNodeMap( const PvString &aParameterName )
{
    cout << "IsGenEnumInNodeMap" << endl;
    PvGenEnum *lParameter = mDevice->GetParameters()->GetEnum( aParameterName );
    return ( lParameter != NULL );
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter to test for.
///
/// \return True if the parameter is present, false if not.
///

bool EbusDeviceAdapter::IsGenRegisterInNodeMap( const PvString &aParameterName )
{
   cout << "IsGenRegisterInNodeMap->"<<aParameterName.GetAscii()  << " " ;
    PvGenRegister *lParameter = mDevice->GetParameters()->GetRegister( aParameterName );
    cout << ( lParameter != NULL ) << endl;
    return ( lParameter != NULL );
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter to test for.
///
/// \return True if the parameter is present and readable, false if either condition fails.
///

bool EbusDeviceAdapter::IsGenReadable( const PvString &aParameterName )
{
    cout << "IsGenReadable" << endl;
    PvGenParameter *lParameter = mDevice->GetParameters()->Get( aParameterName );
    return ( lParameter != NULL ) && lParameter->IsReadable();
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter to test for.
///
/// \return True if the parameter is present and writable, false if either condition fails.
///

bool EbusDeviceAdapter::IsGenWritable( const PvString &aParameterName )
{
    cout << "IsGenWritable" << endl;
    PvGenParameter *lParameter = mDevice->GetParameters()->Get( aParameterName );
    return ( lParameter != NULL ) && lParameter->IsWritable();
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter to test for.
/// \param [in] aEnumEntry Name of the enum entry to test for.
///
/// \return True if the parameter is present and contains the specific enum entry, false if either condition fails.
///

bool EbusDeviceAdapter::IsGenEnumEntryAvailable( const PvString &aParameterName, const PvString &aEnumEntry )
{
    cout << "IsGenEnumEntryAvailable" << endl;
    PvGenEnum *lParameter = mDevice->GetParameters()->GetEnum( aParameterName );
    if ( lParameter != NULL )
    {
        const PvGenEnumEntry *lEE = NULL;
        if ( lParameter->GetEntryByName( aEnumEntry, &lEE ).IsOK() && ( lEE != NULL ) )
        {
            return lEE->IsAvailable();
        }
    }

    return false;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter.
/// \param [out] aValue Value of the parameter.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::GetGenIntegerValue( const PvString &aParameterName, int64_t &aValue )
{
    cout << "GetGenIntegerValue" << endl;
    PvResult aResult = mDevice->GetParameters()->GetIntegerValue( aParameterName, aValue );
    cout << "\t"<<aParameterName.GetAscii() <<"=" <<aValue <<endl;
    return aResult;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter.
/// \param [out] aList A list containing the string representation of all available enum entries.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::GetGenEnumEntriesAvailable( const PvString &aParameterName, PvStringList &aList )
{
    cout << "GetGenEnumEntriesAvailable" << endl;
    aList.Clear();

    PvGenEnum *lParameter = mDevice->GetParameters()->GetEnum( aParameterName );
    if ( lParameter != NULL )
    {
        int64_t lCount = 0;
        lParameter->GetEntriesCount( lCount );    

        for ( int64_t i = 0; i < lCount; i++ )
        {
            const PvGenEnumEntry *lEE = NULL;
            if ( lParameter->GetEntryByIndex( i, &lEE ).IsOK() && ( lEE != NULL ) )
            {
                if ( lEE->IsAvailable() )
                {
                    PvString lName;
                    lEE->GetName( lName );

                    aList.Add( lName );
                }
            }
        }

        return PvResult::Code::OK;
    }

    return PvResult::Code::NOT_FOUND;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter.
/// \param [out] aEnumEnty Value of the parameter.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::GetGenEnumValue( const PvString &aParameterName, PvString &aEnumEntry )
{
    cout << "GetGenEnumValue" << endl;
    return mDevice->GetParameters()->GetEnumValue( aParameterName, aEnumEntry );
}


///
/// \fn PvResult IEbusDeviceAdapter::GetGenStringValue ( const PvString &aParameterName, PvString &aValue )
/// \brief Returns the string value of string parameter identified by its name.
///
/// \param [in] aParameterName Name of the parameter.
/// \param [out] aValue Value of the parameter.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::GetGenStringValue( const PvString &aParameterName, PvString &aValue )
{
    cout << "GetGenStringValue" << endl;
    return mDevice->GetParameters()->GetStringValue( aParameterName, aValue );
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter.
/// \param [in] aEnumEntry New value for the enumeration.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::SetGenEnumValue( const PvString &aParameterName, const PvString &aEnumEntry )
{
    cout << "SetGenEnumValue" << endl;
    return mDevice->GetParameters()->SetEnumValue( aParameterName, aEnumEntry );
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter.
/// \param [in] aLength Length (in bytes) of the register.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::GetGenRegisterLength( const PvString &aParameterName, int64_t &aLength )
{
   cout << "GetGenRegisterLength->" <<aParameterName.GetAscii();
    PvGenRegister *lRegister = mDevice->GetParameters()->GetRegister( aParameterName );
    if ( lRegister != NULL )
    {

       PvResult aResult = lRegister->GetLength( aLength );
       cout << "\t" << dec <<aLength << endl;
       return aResult;
    }
    cout << "Not found" <<endl;
    return PvResult::Code::NOT_FOUND;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter.
/// \param [out] aDataBuffer Buffer where to copy the data.
/// \param [int] aByteCount How many bytes to copy to aDataBuffer.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::GetGenRegisterData( const PvString &aParameterName, uint8_t *aDataBuffer, int64_t aByteCount )
{
    cout << "GetGenRegisterData->"  << aParameterName.GetAscii() <<"=";
    PvGenRegister *lRegister = mDevice->GetParameters()->GetRegister( aParameterName );
    if ( lRegister != NULL )
    {
        PvResult aResult = lRegister->Get( aDataBuffer, aByteCount );
        for (int i=0; i<aByteCount; i++)
         {
            printf("%x ", aDataBuffer[i]);
         }
         cout <<endl;
         return aResult;
    }

    return PvResult::Code::NOT_FOUND;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter.
/// \param [out] aDataBuffer Buffer to copy the data from.
/// \param [int] aByteCount How many bytes to copy from aDataBuffer to the register.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::SetGenRegisterData( const PvString &aParameterName, const uint8_t *aDataBuffer, int64_t aByteCount )
{
   cout << "SetGenRegisterData->" << aParameterName.GetAscii() <<"=";
    PvGenRegister *lRegister = mDevice->GetParameters()->GetRegister( aParameterName );
    if ( lRegister != NULL )
    {
         for (int i=0; i<aByteCount; i++)
         {
            printf("%x ", aDataBuffer[i]);
         }
         cout <<endl;

        return lRegister->Set( aDataBuffer, aByteCount );
    }
    cout <<endl;
    return PvResult::Code::NOT_FOUND;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::RegisterGenInvalidator( const PvString &aParameterName )
{
    cout << "RegisterGenInvalidator" << endl;
    PvGenParameter *lParameter = mDevice->GetParameters()->Get( aParameterName );
    if ( lParameter != NULL )
    {
       cout <<"\t" << aParameterName.GetAscii() <<endl;
        return lParameter->RegisterEventSink( this );
    }

    return PvResult::Code::NOT_FOUND;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aParameterName Name of the parameter.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::UnregisterGenInvalidator( const PvString &aParameterName )
{
    cout << "UnregisterGenInvalidator" << endl;
    PvGenParameter *lParameter = mDevice->GetParameters()->Get( aParameterName );
    if ( lParameter != NULL )
    {
        return lParameter->UnregisterEventSink( this );
    }

    return PvResult::Code::NOT_FOUND;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \return True if the device is connected, false if not.
///

bool EbusDeviceAdapter::IsConnected()
{
    cout << "IsConnected" << endl;
    return ( mDevice != NULL ) && mDevice->IsConnected();
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aAddress Address of the register.
/// \param [in] aValue Value to write to the register.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::WriteRegister( int64_t aAddress, uint32_t aValue )
{
 //   cout << "WriteRegister" << endl;
    PvDeviceGEV *lDeviceGEV = dynamic_cast<PvDeviceGEV *>( mDevice );
    if ( lDeviceGEV != NULL )
    {
        return lDeviceGEV->WriteRegister( aAddress, aValue );
    }

    return PvResult::Code::NOT_SUPPORTED;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aAddress Address of the register.
/// \param [out] aValue Value read from the register.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::ReadRegister( int64_t aAddress, uint32_t &aValue )
{
    cout << "ReadRegister" << endl;
    PvDeviceGEV *lDeviceGEV = dynamic_cast<PvDeviceGEV *>( mDevice );
    if ( lDeviceGEV != NULL )
    {
        PvResult lResult = lDeviceGEV->ReadRegister( aAddress, aValue );
        cout << "\t" << std::hex << aAddress <<"=" << aValue <<endl;
        return lResult;
    }

    return PvResult::Code::NOT_SUPPORTED;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aBuffer Pointer to the data to write.
/// \param [in] aAddress Address where to write the data.
/// \param [aLength] aLength Length of the data to write, in bytes.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::WriteMemory( const uint8_t *aBuffer, int64_t aAddress, int64_t aLength )
{
    cout << "WriteMemory" << endl;
    return mDevice->WriteMemory( aAddress, aBuffer, aLength );
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] aBuffer Pointer of the buffer where the data will be written.
/// \param [in] aAddress Address where to read the daa.
/// \param [aLength] aLength Length of the data to read, in bytes.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::ReadMemory( uint8_t *aBuffer, int64_t aAddress, int64_t aLength )
{
    cout << "ReadMemory" << endl;
    PvResult aResult =  mDevice->ReadMemory( aAddress, aBuffer, aLength );
    cout << "ReadMemory=" << aResult.GetCode() << "  " << aAddress;
    return aResult;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \return True if the device runs on a Pleora design, false if not.
///

bool EbusDeviceAdapter::IsPleoraPowered()
{
    cout << "IsPleoraPowered" << endl;
    return IsConnected() && mDevice->IsPleoraPowered();
}


///
/// \brief PvDeviceEventSink callback from PvDevice
///
/// \param [in] aDevice Device that triggered the callback.
/// \param [in] aEventID Event ID.
/// \param [in] aChannel Channel of the event.
/// \param [in] aBlockID Block ID of the event.
/// \param [in] aTimestamp Timestamp of the event.
/// \param [in] aData Data pointer (can be NULL if EVENT_CMD instead of EVENTDATA_CMD)
/// \param [in] aDataLength Data Length (can be 0 if EVENT_CMD instead of EVENTDATA_CMD)
///

void EbusDeviceAdapter::OnEvent( PvDevice *aDevice, 
    uint16_t aEventID, uint16_t aChannel, uint64_t aBlockID, uint64_t aTimestamp, 
    const void *aData, uint32_t aDataLength )
{
//    PtUtilsLib::LockScope lLockScope( mEventSinkListMutex );
    /////////////////////////////////////////////////////


    IPvDeviceEventSinkList::iterator lIterator = mEventSinkList->begin();
    while ( lIterator != mEventSinkList->end() )
    {

         cout << "OnEvent->NotifyEvent" << hex << aEventID << endl;
        ( *( lIterator++ ) )->NotifyEvent( aEventID, aChannel, aBlockID, aTimestamp, aData, aDataLength );
    }
}


///
/// \brief PvGenParameter callback for GenICam parameters invalidators
///
/// \param [in] The invalidated parameter.
///

void EbusDeviceAdapter::OnParameterUpdate( PvGenParameter *aParameter )
{
    /////////////////////////////////////////////////////
   cout << "OnParameterUpdate->" << aParameter->GetName().GetAscii() <<endl;

    IPvDeviceEventSinkList::iterator lIterator = mEventSinkList->begin();
    while ( lIterator != mEventSinkList->end() )
    {
        PvString lName;
        aParameter->GetName( lName );
         cout << "\tNotifyInvalidatedGenParameter->" << lName.GetAscii() <<endl;
        ( *( lIterator++ ) )->NotifyInvalidatedGenParameter( lName );
    }
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \return True if using the GigE Vision protocol, false if not.
///

bool EbusDeviceAdapter::IsGigEVision()
{
    cout << "IsGigEVision" << endl;
    PvDeviceGEV *lDevice = dynamic_cast<PvDeviceGEV *>( mDevice );
    return lDevice != NULL;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \return True if using the USB3 Vision protocol, false if not.
///

bool EbusDeviceAdapter::IsUSB3Vision()
{
    cout << "IsUSB3Vision" << endl;
    PvDeviceU3V *lDevice = dynamic_cast<PvDeviceU3V *>( mDevice );
    return lDevice != NULL;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \return OK if the messaging channel is idle. TIMEOUT if the timeout elapsed before it becomes idle.
///

PvResult EbusDeviceAdapter::WaitForMessagingChannelIdle( uint32_t aTimeout )
{
    cout << "WaitForMessagingChannelIdle" << endl;
    return mDevice->WaitForMessagingChannelIdle( aTimeout );
}


///
/// \brief See IEbusDeviceAdapter for description.
/// 
/// \param [in] Pointer to an object implementing the IPvDeviceEventSink interface.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::RegisterEventSink( IPvDeviceEventSink *aEventSink )
{
   cout << "RegisterEventSink->" << endl;
    if ( aEventSink == NULL )
    {
        return PvResult::Code::INVALID_PARAMETER;
    }

    PvResult lResult = PvResult::Code::OK;

    // 1st check if the callback is already registered
    IPvDeviceEventSinkList::iterator lIterator = mEventSinkList->begin();
    while ( lIterator != mEventSinkList->end() )
    {
        if ( aEventSink == ( *lIterator++ ) )
        {
            // Already registered...
            return PvResult::Code::OK;
        }
    }

    if ( lResult.IsOK() )
    {
        // Add the event sink to our list of callbacks for this node
        mEventSinkList->push_back( aEventSink );
    }

    return PvResult::Code::OK;
}


///
/// \brief See IEbusDeviceAdapter for description.
///
/// \param [in] Pointer to an object implementing the IPvDeviceEventSink interface.
///
/// \return OK on success.
///

PvResult EbusDeviceAdapter::UnregisterEventSink( IPvDeviceEventSink *aEventSink )
{
    cout << "UnregisterEventSink" << endl;
    if ( aEventSink == NULL )
    {
        return PvResult::Code::INVALID_PARAMETER;
    }

 //   PtUtilsLib::LockScope lLockScope( mEventSinkListMutex );
    /////////////////////////////////////////////////////

    PvResult lResult = PvResult::Code::NOT_FOUND;

    // Try finding the event sink in our list of callbacks
    IPvDeviceEventSinkList::iterator lIterator = mEventSinkList->begin();
    while ( lIterator != mEventSinkList->end() )
    {
        if ( aEventSink == *lIterator )
        {
            // Remove the event sink from out list of callbacks
            mEventSinkList->erase( lIterator );
            lResult = PvResult::Code::OK;
            break;
        }

        lIterator++;
    }

    return lResult;
}

