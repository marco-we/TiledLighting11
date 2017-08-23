#pragma once

#include <cstdint>

namespace TiledLighting11
{
    enum GPAType
    {
        GPA_TYPE_FLOAT32,
        GPA_TYPE_FLOAT64,
        GPA_TYPE_UINT32,
        GPA_TYPE_UINT64,
        GPA_TYPE_INT32,
        GPA_TYPE_INT64
    };

    enum GPAUsageType
    {
        GPA_USAGE_TYPE_RATIO,
        GPA_USAGE_TYPE_PERCENTAGE,
        GPA_USAGE_TYPE_CYCLES,
        GPA_USAGE_TYPE_MILLISECONDS,
        GPA_USAGE_TYPE_BYTES,
        GPA_USAGE_TYPE_ITEMS,
        GPA_USAGE_TYPE_KILOBYTES,
    };

    enum GPAStatus
    {
        GPA_STATUS_OK = 0,
        GPA_STATUS_ERROR_NULL_POINTER,
        GPA_STATUS_ERROR_COUNTERS_NOT_OPEN,
        GPA_STATUS_ERROR_COUNTERS_ALREADY_OPEN,
        GPA_STATUS_ERROR_INDEX_OUT_OF_RANGE,
        GPA_STATUS_ERROR_NOT_FOUND,
        GPA_STATUS_ERROR_ALREADY_ENABLED,
        GPA_STATUS_ERROR_NO_COUNTERS_ENABLED,
        GPA_STATUS_ERROR_NOT_ENABLED,
        GPA_STATUS_ERROR_SAMPLING_NOT_STARTED,
        GPA_STATUS_ERROR_SAMPLING_ALREADY_STARTED,
        GPA_STATUS_ERROR_SAMPLING_NOT_ENDED,
        GPA_STATUS_ERROR_NOT_ENOUGH_PASSES,
        GPA_STATUS_ERROR_PASS_NOT_ENDED,
        GPA_STATUS_ERROR_PASS_NOT_STARTED,
        GPA_STATUS_ERROR_PASS_ALREADY_STARTED,
        GPA_STATUS_ERROR_SAMPLE_NOT_STARTED,
        GPA_STATUS_ERROR_SAMPLE_ALREADY_STARTED,
        GPA_STATUS_ERROR_SAMPLE_NOT_ENDED,
        GPA_STATUS_ERROR_CANNOT_CHANGE_COUNTERS_WHEN_SAMPLING,
        GPA_STATUS_ERROR_SESSION_NOT_FOUND,
        GPA_STATUS_ERROR_SAMPLE_NOT_FOUND,
        GPA_STATUS_ERROR_SAMPLE_NOT_FOUND_IN_ALL_PASSES,
        GPA_STATUS_ERROR_COUNTER_NOT_OF_SPECIFIED_TYPE,
        GPA_STATUS_ERROR_READING_COUNTER_RESULT,
        GPA_STATUS_ERROR_VARIABLE_NUMBER_OF_SAMPLES_IN_PASSES,
        GPA_STATUS_ERROR_FAILED,
        GPA_STATUS_ERROR_HARDWARE_NOT_SUPPORTED,
        GPA_STATUS_ERROR_CATALYST_VER_UNSUPPORTED_SI,
    };

    void WriteSession( uint32_t currentWaitSessionID, const char* filename, bool append );

    class GPAWrapper
    {
    public:

        static GPAWrapper& Instance();

        bool IsGPASupported();

        int Initialize();
        int Destroy();

        int OpenContext( void* context );
        int CloseContext();
        int SelectContext( void* context );

        int GetNumCounters( uint32_t* count );
        int GetCounterName( uint32_t idx, const char** name );
        int GetCounterDescription( uint32_t idx, const char** desc );
        int GetCounterDataType( uint32_t idx, GPAType* counterDataType );
        int GetCounterUsageType( uint32_t idx, GPAUsageType* counterUsageType );
        int GetDataTypeAsStr( GPAType counterUsageType, const char** typeStr );
        int GetUsageTypeAsStr( GPAUsageType counterUsageType, const char** usageTypeStr );

        int EnableCounter( uint32_t idx );        
        int GetEnabledCount( uint32_t* count );
        int GetEnabledIndex( uint32_t enabledNumber, uint32_t* enabledCounterIdx );
        int IsCounterEnabled( uint32_t idx );
        int EnableCounterStr( const char *str );
        int DisableCounterStr( const char *str );

        int EnableAllCounters();
        int DisableAllCounters();
        int GetCounterIndex( const char* counter, uint32_t* index );

        int GetPassCount( uint32_t* numPasses );

        int BeginSession( uint32_t* sessionId );
        int EndSession();
               
        int BeginPass();
        int EndPass();

        int BeginSample( uint32_t sampleId );
        int EndSample();

        int GetSampleCount( uint32_t sessionId, uint32_t* samples );
        int IsSampleReady( bool* readyResult, uint32_t sessionId, uint32_t sampleId );
        int IsSessionReady( bool* readyResult, uint32_t sessionId );

        int GetSampleUInt64( uint32_t sessionId, uint32_t sampleId, uint32_t counterId, uint64_t* result );
        int GetSampleUInt32( uint32_t sessionId, uint32_t sampleId, uint32_t counterId, uint32_t* result );
        int GetSampleFloat64( uint32_t sessionId, uint32_t sampleId, uint32_t counterId, double* result );
        int GetSampleFloat32( uint32_t sessionId, uint32_t sampleId, uint32_t counterId, float* result );
       
    private:

        GPAWrapper();
        ~GPAWrapper();

        static GPAWrapper* m_pInstance;

        bool m_bGPASupported;

        struct GPAWrapperImpl;
        GPAWrapperImpl* m_pImpl;               
    };
    
    class GPAHelper
    {
    public:

        static void Reset();

        static int BeginSample( const char* name );
        static const char* GetSampleName( uint32_t idx );

    private:

        const static uint32_t MAX_SAMPLE_COUNT = 32;
        const static uint32_t MAX_SAMPLE_NAME_LENGTH = 64;

        static char m_pSampleNames[MAX_SAMPLE_COUNT][MAX_SAMPLE_NAME_LENGTH];
        static uint32_t m_uSampleCount;
    };
}