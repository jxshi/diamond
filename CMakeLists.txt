cmake_minimum_required (VERSION 2.6)
project (DIAMOND)

option(BUILD_STATIC "BUILD_STATIC" OFF)
option(EXTRA "EXTRA" OFF)
option(STATIC_LIBGCC "STATIC_LIBGCC" OFF)
option(STATIC_LIBSTDC++ "STATIC_LIBSTDC++" OFF)

IF(STATIC_LIBSTDC++)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
endif()

IF(STATIC_LIBGCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc")
endif()

if(BUILD_STATIC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBRARIES OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

set(CMAKE_CXX_STANDARD 11)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif()

if(CMAKE_BUILD_MARCH)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=${CMAKE_BUILD_MARCH}")
endif()

find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_ITERATOR_DEBUG_LEVEL=0)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-uninitialized -Wno-ignored-attributes -Wno-unused-variable")
endif()

include_directories(
  "${CMAKE_SOURCE_DIR}/src"
  "${ZLIB_INCLUDE_DIR}"
  "${CMAKE_SOURCE_DIR}/src/lib")

set(DISPATCH_OBJECTS
"src/dp/swipe/banded_3frame_swipe.cpp"
"src/dp/swipe/swipe.cpp"
"src/dp/swipe/banded_swipe.cpp"
"src/search/collision.cpp"
"src/search/stage1.cpp"
"src/search/stage2.cpp"
"src/tools/benchmark.cpp"
"src/dp/swipe/swipe_wrapper.cpp"
"src/util/tantan.cpp"
)

add_library(arch_generic OBJECT ${DISPATCH_OBJECTS})
target_compile_options(arch_generic PUBLIC -DDISPATCH_ARCH=ARCH_GENERIC)
add_library(arch_sse4_1 OBJECT ${DISPATCH_OBJECTS})
target_compile_options(arch_sse4_1 PUBLIC -DDISPATCH_ARCH=ARCH_SSE4_1 -mssse3 -mpopcnt -msse4.1)

add_executable(diamond $<TARGET_OBJECTS:arch_generic>
  $<TARGET_OBJECTS:arch_sse4_1>
  src/run/main.cpp
  src/basic/config.cpp
  src/basic/score_matrix.cpp
  src/blast/blast_filter.cpp
  src/blast/blast_seg.cpp
  src/blast/sm_blosum45.c
  src/blast/sm_blosum50.c
  src/blast/sm_blosum62.c
  src/blast/sm_blosum80.c
  src/blast/sm_blosum90.c
  src/blast/sm_pam30.c
  src/blast/sm_pam70.c
  src/blast/sm_pam250.c
  src/data/queries.cpp
  src/data/reference.cpp
  src/data/seed_histogram.cpp
  src/output/daa_record.cpp
  src/util/command_line_parser.cpp
  src/util/seq_file_format.cpp
  src/util/util.cpp 
  src/util/Timer.cpp
  src/basic/basic.cpp
  src/basic/hssp.cpp
  src/dp/ungapped_align.cpp
  src/run/tools.cpp
  src/dp/greedy_align.cpp
  src/output/output_format.cpp
  src/output/join_blocks.cpp
  src/data/frequent_seeds.cpp
  src/align/legacy/query_mapper.cpp
  src/output/blast_tab_format.cpp
  src/dp/padded_banded_sw.cpp
  src/dp/needleman_wunsch.cpp
  src/output/blast_pairwise_format.cpp
  src/dp/comp_based_stats.cpp
  src/run/double_indexed.cpp
  src/output/sam_format.cpp
  src/align/align.cpp
  src/search/setup.cpp
  src/dp/diag_scores.cpp
  src/data/taxonomy.cpp
  src/basic/masking.cpp
  src/dp/banded_sw.cpp
  src/data/seed_set.cpp
  src/util/simd.cpp
  src/output/taxon_format.cpp
  src/output/view.cpp
  src/output/output_sink.cpp
  src/output/target_culling.cpp
  src/align/legacy/greedy_pipeline.cpp
  src/align/legacy/swipe_pipeline.cpp
  src/align/legacy/banded_swipe_pipeline.cpp
  src/data/ref_dictionary.cpp
  src/util/io/compressed_stream.cpp
  src/util/io/deserializer.cpp
  src/util/io/file_sink.cpp
  src/util/io/file_source.cpp
  src/util/io/input_file.cpp
  src/util/io/input_stream_buffer.cpp
  src/util/io/output_file.cpp
  src/util/io/output_stream_buffer.cpp
  src/util/io/serializer.cpp
  src/util/io/temp_file.cpp
  src/util/io/text_input_file.cpp
  src/data/taxon_list.cpp
  src/data/taxonomy_nodes.cpp
  src/util/algo/MurmurHash3.cpp
  src/search/stage0.cpp
  src/data/seed_array.cpp
  src/output/paf_format.cpp
  src/util/system/system.cpp
  src/run/cluster.cpp
  src/util/algo/greedy_vortex_cover.cpp
  src/util/sequence/sequence.cpp
  src/tools/tsv_record.cpp
  src/tools/tools.cpp
  src/util/system/getRSS.cpp
  src/util/math/sparse_matrix.cpp
  src/lib/tantan/LambdaCalculator.cc
  src/data/taxonomy_filter.cpp
  src/util/algo/upgma.cpp
  src/util/algo/upgma_mc.cpp
  src/util/algo/edge_vec.cpp
  src/util/string/string.cpp
  src/align/extend.cpp
  src/align/target.cpp
  src/test/simulate.cpp
  src/test/test.cpp
  src/align/ranking.cpp
  src/align/ungapped.cpp
  src/align/gapped.cpp
  src/align/culling.cpp
  src/cluster/medoid.cpp
  src/align/output.cpp
)

if(EXTRA)
  target_sources(diamond
    PUBLIC
      src/extra/benchmark.cpp
	  src/extra/test.cpp
	  src/dp/sw_3frame.cpp
	  src/extra/roc.cpp
	  src/extra/model_sim.cpp
	  src/extra/opt.cpp
  )
  add_definitions(-DEXTRA)
endif()

target_link_libraries(diamond ${ZLIB_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS diamond DESTINATION bin)
