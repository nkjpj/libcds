/*
    This file is a part of libcds - Concurrent Data Structures library

    (C) Copyright Maxim Khizhinsky (libcds.dev@gmail.com) 2006-2017

    Source code repo: http://github.com/khizmax/libcds/
    Download: http://sourceforge.net/projects/libcds/files/

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "map_insdel_item_int.h"

namespace map {

    size_t Map_InsDel_item_int::s_nMapSize = 10000;      // map size
    size_t Map_InsDel_item_int::s_nInsertThreadCount = 4;  // count of insertion thread
    size_t Map_InsDel_item_int::s_nDeleteThreadCount = 4;  // count of deletion thread
    size_t Map_InsDel_item_int::s_nMaxLoadFactor = 8;      // maximum load factor

    size_t Map_InsDel_item_int::s_nAttemptCount = 2000;      // count of SUCCESS insert/delete for each thread
    size_t Map_InsDel_item_int::s_nGoalItem = 5000;

    size_t Map_InsDel_item_int::s_nCuckooInitialSize = 1024;// initial size for CuckooSet
    size_t Map_InsDel_item_int::s_nCuckooProbesetSize = 16; // CuckooSet probeset size (only for list-based probeset)
    size_t Map_InsDel_item_int::s_nCuckooProbesetThreshold = 0; // CUckooSet probeset threshold (0 - use default)

    size_t Map_InsDel_item_int::s_nFeldmanMap_HeadBits = 10;
    size_t Map_InsDel_item_int::s_nFeldmanMap_ArrayBits = 4;

    size_t Map_InsDel_item_int::s_nLoadFactor = 1;

    void Map_InsDel_item_int::SetUpTestCase()
    {
        cds_test::config const& cfg = get_config( "map_insdel_item_int" );

        s_nMapSize = cfg.get_size_t( "MapSize", s_nMapSize );
        if ( s_nMapSize < 1000 )
            s_nMapSize = 1000;

        s_nInsertThreadCount = cfg.get_size_t( "InsertThreadCount", s_nInsertThreadCount );
        if ( s_nInsertThreadCount == 0 )
            s_nInsertThreadCount = 2;

        s_nDeleteThreadCount = cfg.get_size_t( "DeleteThreadCount", s_nDeleteThreadCount );
        if ( s_nDeleteThreadCount == 0 )
            s_nDeleteThreadCount = 2;

        s_nGoalItem = cfg.get_size_t( "GoalItem", s_nGoalItem );
        if ( s_nGoalItem >= s_nMapSize )
            s_nGoalItem = s_nMapSize / 2;

        s_nAttemptCount = cfg.get_size_t( "AttemptCount", s_nAttemptCount );
        if ( s_nAttemptCount == 0 )
            s_nAttemptCount = 1000;

        s_nCuckooInitialSize = cfg.get_size_t( "CuckooInitialSize", s_nCuckooInitialSize );
        if ( s_nCuckooInitialSize < 256 )
            s_nCuckooInitialSize = 256;

        s_nCuckooProbesetSize = cfg.get_size_t( "CuckooProbesetSize", s_nCuckooProbesetSize );
        if ( s_nCuckooProbesetSize < 8 )
            s_nCuckooProbesetSize = 8;

        s_nCuckooProbesetThreshold = cfg.get_size_t( "CuckooProbesetThreshold", s_nCuckooProbesetThreshold );

        s_nFeldmanMap_HeadBits = cfg.get_size_t( "FeldmanMapHeadBits", s_nFeldmanMap_HeadBits );
        if ( s_nFeldmanMap_HeadBits == 0 )
            s_nFeldmanMap_HeadBits = 2;

        s_nFeldmanMap_ArrayBits = cfg.get_size_t( "FeldmanMapArrayBits", s_nFeldmanMap_ArrayBits );
        if ( s_nFeldmanMap_ArrayBits == 0 )
            s_nFeldmanMap_ArrayBits = 2;
    }

    std::vector<size_t> Map_InsDel_item_int_LF::get_load_factors()
    {
        cds_test::config const& cfg = get_config( "map_insdel_item_int" );

        s_nMaxLoadFactor = cfg.get_size_t( "MaxLoadFactor", s_nMaxLoadFactor );
        if ( s_nMaxLoadFactor == 0 )
            s_nMaxLoadFactor = 1;

        std::vector<size_t> lf;
        for ( size_t n = 1; n <= s_nMaxLoadFactor; n *= 2 )
            lf.push_back( n );

        return lf;
    }

#ifdef CDSTEST_GTEST_INSTANTIATE_TEST_CASE_P_HAS_4TH_ARG
    static std::string get_test_parameter_name( testing::TestParamInfo<size_t> const& p )
    {
        return std::to_string( p.param );
    }
    INSTANTIATE_TEST_CASE_P( a, Map_InsDel_item_int_LF, ::testing::ValuesIn( Map_InsDel_item_int_LF::get_load_factors() ), get_test_parameter_name );
#else
    INSTANTIATE_TEST_CASE_P( a, Map_InsDel_item_int_LF, ::testing::ValuesIn( Map_InsDel_item_int_LF::get_load_factors() ) );
#endif

} // namespace map
