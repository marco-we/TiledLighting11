#include "GPAWrapper.h"
#include <stdio.h>

#define SAFE_CALL_RETURN(x) { if ( !m_bGPASupported ) return GPA_STATUS_ERROR_HARDWARE_NOT_SUPPORTED; return x; }
#define LOAD_FN_PTR(x) { pFn##x = (GPA_##x##PtrType)GetProcAddress( hDLL, "GPA_"#x ); if ( !pFn##x## ) return false; }

#include <Windows.h>

#include "..\\..\\..\\gpuperfapi\\Include\GPUPerfAPI.h"
#include "..\\..\\..\\gpuperfapi\\Include\GPUPerfAPIFunctionTypes.h"

namespace TiledLighting11
{
    GPAWrapper* GPAWrapper::m_pInstance = nullptr;

    struct GPAWrapper::GPAWrapperImpl
    {
        HMODULE                         hDLL = nullptr;
        GPA_InitializePtrType           pFnInitialize = nullptr;
        GPA_DestroyPtrType              pFnDestroy = nullptr;
        GPA_OpenContextPtrType          pFnOpenContext = nullptr;
        GPA_CloseContextPtrType         pFnCloseContext = nullptr;
        GPA_SelectContextPtrType        pFnSelectContext = nullptr;
        GPA_GetNumCountersPtrType       pFnGetNumCounters = nullptr;
        GPA_GetCounterNamePtrType       pFnGetCounterName = nullptr;
        GPA_GetCounterDescriptionPtrType   pFnGetCounterDescription = nullptr;
        GPA_GetCounterDataTypePtrType   pFnGetCounterDataType = nullptr;
        GPA_GetCounterUsageTypePtrType  pFnGetCounterUsageType = nullptr;
        GPA_GetDataTypeAsStrPtrType     pFnGetDataTypeAsStr = nullptr;
        GPA_GetUsageTypeAsStrPtrType    pFnGetUsageTypeAsStr = nullptr;
        GPA_EnableCounterPtrType        pFnEnableCounter = nullptr;
        GPA_DisableCounterPtrType       pFnDisableCounter = nullptr;
        GPA_GetEnabledCountPtrType      pFnGetEnabledCount = nullptr;
        GPA_GetEnabledIndexPtrType      pFnGetEnabledIndex = nullptr;
        GPA_IsCounterEnabledPtrType     pFnIsCounterEnabled = nullptr;        
        GPA_EnableCounterStrPtrType     pFnEnableCounterStr = nullptr;
        GPA_DisableCounterStrPtrType    pFnDisableCounterStr = nullptr;
        GPA_EnableAllCountersPtrType    pFnEnableAllCounters = nullptr;
        GPA_DisableAllCountersPtrType   pFnDisableAllCounters = nullptr;
        GPA_GetCounterIndexPtrType      pFnGetCounterIndex = nullptr;
        GPA_GetPassCountPtrType         pFnGetPassCount = nullptr;
        GPA_BeginSessionPtrType         pFnBeginSession = nullptr;
        GPA_EndSessionPtrType           pFnEndSession = nullptr;
        GPA_BeginPassPtrType            pFnBeginPass = nullptr;
        GPA_EndPassPtrType              pFnEndPass = nullptr;
        GPA_BeginSamplePtrType          pFnBeginSample = nullptr;
        GPA_EndSamplePtrType            pFnEndSample = nullptr;
        GPA_GetSampleCountPtrType       pFnGetSampleCount = nullptr;
        GPA_IsSampleReadyPtrType        pFnIsSampleReady = nullptr;
        GPA_IsSessionReadyPtrType       pFnIsSessionReady = nullptr;
        GPA_GetSampleUInt64PtrType      pFnGetSampleUInt64 = nullptr;
        GPA_GetSampleUInt32PtrType      pFnGetSampleUInt32 = nullptr;
        GPA_GetSampleFloat64PtrType     pFnGetSampleFloat64 = nullptr;
        GPA_GetSampleFloat32PtrType     pFnGetSampleFloat32 = nullptr;

        bool Initialize()
        {
            hDLL = LoadLibrary( L"GPUPerfAPIDX11-x64.dll" );
            if ( !hDLL )
                return false;

            // Initialize GPUPerfAPI
            LOAD_FN_PTR( Initialize );
            LOAD_FN_PTR( Destroy );
            LOAD_FN_PTR( OpenContext );
            LOAD_FN_PTR( CloseContext );
            LOAD_FN_PTR( SelectContext );
            LOAD_FN_PTR( GetNumCounters );
            LOAD_FN_PTR( GetCounterName );
            LOAD_FN_PTR( GetCounterDescription );
            LOAD_FN_PTR( GetCounterDataType );
            LOAD_FN_PTR( GetCounterUsageType );
            LOAD_FN_PTR( GetDataTypeAsStr );
            LOAD_FN_PTR( GetUsageTypeAsStr );
            LOAD_FN_PTR( EnableCounter );
            LOAD_FN_PTR( DisableCounter );
            LOAD_FN_PTR( GetEnabledCount );
            LOAD_FN_PTR( GetEnabledIndex );
            LOAD_FN_PTR( IsCounterEnabled );
            LOAD_FN_PTR( EnableCounterStr );
            LOAD_FN_PTR( DisableCounterStr );
            LOAD_FN_PTR( EnableAllCounters );
            LOAD_FN_PTR( DisableAllCounters );
            LOAD_FN_PTR( GetCounterIndex );
            LOAD_FN_PTR( GetPassCount );
            LOAD_FN_PTR( BeginSession );
            LOAD_FN_PTR( EndSession );
            LOAD_FN_PTR( BeginPass );
            LOAD_FN_PTR( EndPass );
            LOAD_FN_PTR( BeginSample );
            LOAD_FN_PTR( EndSample );
            LOAD_FN_PTR( GetSampleCount );
            LOAD_FN_PTR( IsSampleReady );
            LOAD_FN_PTR( IsSessionReady );
            LOAD_FN_PTR( GetSampleUInt64 );
            LOAD_FN_PTR( GetSampleUInt32 );
            LOAD_FN_PTR( GetSampleFloat64 );
            LOAD_FN_PTR( GetSampleFloat32 );

            return true;
        }

        void Shutdown()
        {
            if ( hDLL )
            {
                FreeLibrary( hDLL );
                hDLL = nullptr;
            }
        }

        };

        GPAWrapper& GPAWrapper::Instance()
        {
            if ( !m_pInstance )
            {
                m_pInstance = new GPAWrapper();
            }
            return *m_pInstance;
        }

        GPAWrapper::GPAWrapper()
        {
            m_pImpl = new GPAWrapper::GPAWrapperImpl();
            m_bGPASupported = m_pImpl->Initialize();
        }

        GPAWrapper::~GPAWrapper()
        {
            m_pImpl->Shutdown();
            delete m_pImpl;
        }

        bool GPAWrapper::IsGPASupported()
        {
            return m_bGPASupported;
        }

        int GPAWrapper::Initialize() 
        { 
            SAFE_CALL_RETURN( m_pImpl->pFnInitialize() );
        }

        int GPAWrapper::Destroy()
        {
            SAFE_CALL_RETURN( m_pImpl->pFnDestroy() );
        }

        int GPAWrapper::OpenContext( void* context )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnOpenContext( context ) );
        }

        int GPAWrapper::CloseContext()
        {
            SAFE_CALL_RETURN( m_pImpl->pFnCloseContext() );
        }

        int GPAWrapper::SelectContext( void* context )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnSelectContext( context ) );
        }

        int GPAWrapper::GetNumCounters( uint32_t* count )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetNumCounters( count ) );
        }

        int GPAWrapper::GetCounterName( uint32_t idx, const char** name )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetCounterName( idx, name ) );
        }

        int GPAWrapper::GetCounterDescription( uint32_t idx, const char** desc )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetCounterDescription( idx, desc ) );
        }

        int GPAWrapper::GetCounterDataType( uint32_t idx, GPAType* counterDataType )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetCounterDataType( idx, (GPA_Type*)counterDataType ) );
        }

        int GPAWrapper::GetCounterUsageType( uint32_t idx, GPAUsageType* counterUsageType )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetCounterUsageType( idx, (GPA_Usage_Type*)counterUsageType ) );
        }

        int GPAWrapper::GetDataTypeAsStr( GPAType counterUsageType, const char** typeStr )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetDataTypeAsStr( (GPA_Type)counterUsageType, typeStr ) );
        }

        int GPAWrapper::GetUsageTypeAsStr( GPAUsageType counterUsageType, const char** usageTypeStr )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetUsageTypeAsStr( (GPA_Usage_Type)counterUsageType, usageTypeStr ) );
        }

        int GPAWrapper::EnableCounter( uint32_t idx )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnEnableCounter( idx ) );
        }

        int GPAWrapper::GetEnabledCount( uint32_t* count )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetEnabledCount( count ) );
        }

        int GPAWrapper::GetEnabledIndex( uint32_t enabledNumber, uint32_t* enabledCounterIdx )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetEnabledIndex( enabledNumber, enabledCounterIdx ) );
        }

        int GPAWrapper::IsCounterEnabled( uint32_t idx )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnIsCounterEnabled( idx ) );
        }

        int GPAWrapper::EnableCounterStr( const char *str )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnEnableCounterStr( str ) );
        }

        int GPAWrapper::DisableCounterStr( const char *str )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnDisableCounterStr( str ) );
        }

        int GPAWrapper::EnableAllCounters()
        {
            SAFE_CALL_RETURN( m_pImpl->pFnEnableAllCounters() );
        }

        int GPAWrapper::DisableAllCounters()
        {
            SAFE_CALL_RETURN( m_pImpl->pFnDisableAllCounters() );
        }

        int GPAWrapper::GetCounterIndex( const char* counter, uint32_t* index )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetCounterIndex( counter, index ) );
        }

        int GPAWrapper::GetPassCount( uint32_t* numPasses )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetPassCount( numPasses ) );
        }

        int GPAWrapper::BeginSession( uint32_t* sessionId )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnBeginSession( sessionId ) );
        }

        int GPAWrapper::EndSession()
        {
            SAFE_CALL_RETURN( m_pImpl->pFnEndSession() );
        }

        int GPAWrapper::BeginPass()
        {
            SAFE_CALL_RETURN( m_pImpl->pFnBeginPass() );
        }

        int GPAWrapper::EndPass()
        {
            SAFE_CALL_RETURN( m_pImpl->pFnEndPass() );
        }

        int GPAWrapper::BeginSample( uint32_t sampleId )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnBeginSample( sampleId ) );
        }

        int GPAWrapper::EndSample()
        {
            SAFE_CALL_RETURN( m_pImpl->pFnEndSample() );
        }

        int GPAWrapper::GetSampleCount( uint32_t sessionId, uint32_t* samples )
        {
            //SAFE_CALL_RETURN( m_pImpl->pFnGetSampleCount( sessionId, samples ) );
            gpa_uint32 s;
            m_pImpl->pFnGetSampleCount( sessionId, &s );
            *samples = s;
            return GPA_STATUS_OK;
        }

        int GPAWrapper::IsSampleReady( bool* readyResult, uint32_t sessionId, uint32_t sampleId )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnIsSampleReady( readyResult, sessionId, sampleId ) );
        }

        int GPAWrapper::IsSessionReady( bool* readyResult, uint32_t sessionId )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnIsSessionReady( readyResult, sessionId ) );
        }

        int GPAWrapper::GetSampleUInt64( uint32_t sessionId, uint32_t sampleId, uint32_t counterId, uint64_t* result )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetSampleUInt64( sessionId, sampleId, counterId, result ) );
        }

        int GPAWrapper::GetSampleUInt32( uint32_t sessionId, uint32_t sampleId, uint32_t counterId, uint32_t* result )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetSampleUInt32( sessionId, sampleId, counterId, result ) );
        }

        int GPAWrapper::GetSampleFloat64( uint32_t sessionId, uint32_t sampleId, uint32_t counterId, double* result )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetSampleFloat64( sessionId, sampleId, counterId, result ) );
        }

        int GPAWrapper::GetSampleFloat32( uint32_t sessionId, uint32_t sampleId, uint32_t counterId, float* result )
        {
            SAFE_CALL_RETURN( m_pImpl->pFnGetSampleFloat32( sessionId, sampleId, counterId, result ) );
        }

        char GPAHelper::m_pSampleNames[MAX_SAMPLE_COUNT][MAX_SAMPLE_NAME_LENGTH];
        uint32_t GPAHelper::m_uSampleCount = 0;

        void GPAHelper::Reset()
        {
            m_uSampleCount = 0;
        }

        int GPAHelper::BeginSample( const char* name )
        {
            if ( m_uSampleCount == MAX_SAMPLE_COUNT )
                return -1;

            memset( m_pSampleNames[m_uSampleCount], 0, MAX_SAMPLE_NAME_LENGTH );
            strncpy_s( m_pSampleNames[m_uSampleCount], MAX_SAMPLE_NAME_LENGTH, name, strlen( name ) );
            
            return GPAWrapper::Instance().BeginSample( m_uSampleCount++ );
        }

        const char* GPAHelper::GetSampleName( uint32_t idx )
        {
            if ( idx < MAX_SAMPLE_COUNT )
                return m_pSampleNames[idx];
            else
                return "Unknown";
        }

#pragma warning( disable : 4996 )
        void WriteSession( uint32_t currentWaitSessionID, const char* filename, bool append )
        {
            GPAWrapper& gpa = GPAWrapper::Instance();

            uint32_t count;
            gpa.GetEnabledCount( &count );

            if ( !append )
            {
                const char* name;
                FILE* f = fopen( filename, "w" );
                assert( f );
                fprintf( f, "Identifier, " );
                for ( uint32_t counter = 0; counter < count; counter++ )
                {
                    uint32_t enabledCounterIndex;
                    gpa.GetEnabledIndex( counter, &enabledCounterIndex );
                    gpa.GetCounterName( enabledCounterIndex, &name );
                    fprintf( f, "%s, ", name );
                }
                fprintf( f, "\n" );
                fclose( f );
            }

            FILE* f = fopen( filename, "a+" );
            assert( f );
            uint32_t sampleCount;
            gpa.GetSampleCount( currentWaitSessionID, &sampleCount );
            for ( uint32_t sample = 0; sample < sampleCount; sample++ )
            {
                fprintf( f, "session: %d; sample: %d, ", currentWaitSessionID, sample );
                for ( gpa_uint32 counter = 0; counter < count; counter++ )
                {
                    uint32_t enabledCounterIndex;
                    gpa.GetEnabledIndex( counter, &enabledCounterIndex );
                    GPAType type;
                    gpa.GetCounterDataType( enabledCounterIndex, &type );
                    if ( type == GPA_TYPE_UINT32 )
                    {
                        uint32_t value;
                        gpa.GetSampleUInt32( currentWaitSessionID, sample, enabledCounterIndex, &value );
                        fprintf( f, "%u,", value );
                    }
                    else if ( type == GPA_TYPE_UINT64 )
                    {
                        uint64_t value;
                        gpa.GetSampleUInt64( currentWaitSessionID, sample, enabledCounterIndex, &value );
                        fprintf( f, "%I64u,", value );
                    }
                    else if ( type == GPA_TYPE_FLOAT32 )
                    {
                        float value;
                        gpa.GetSampleFloat32( currentWaitSessionID, sample, enabledCounterIndex, &value );
                        fprintf( f, "%f,", value );
                    }
                    else if ( type == GPA_TYPE_FLOAT64 )
                    {
                        double value;
                        gpa.GetSampleFloat64( currentWaitSessionID, sample, enabledCounterIndex, &value );
                        fprintf( f, "%f,", value );
                    }
                    else
                    {
                        assert( false );
                    }
                }
                fprintf( f, "\n" );
            }
            fclose( f );
        }
#pragma warning( default : 4996 )

    }
