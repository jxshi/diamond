gcc -c -O3 -DNDEBUG src/blast/sm_blosum45.c src/blast/sm_blosum50.c src/blast/sm_blosum62.c src/blast/sm_blosum80.c src/blast/sm_blosum90.c src/blast/sm_pam30.c src/blast/sm_pam70.c src/blast/sm_pam250.c
g++ -DNDEBUG -O3 -Wno-deprecated-declarations $1 $2 $3 \
  sm*.o \
  src/run/main.cpp \
  src/basic/config.cpp \
  src/util/tinythread.cpp \
  src/util/compressed_stream.cpp \
  src/basic/score_matrix.cpp \
  src/blast/blast_filter.cpp \
  src/blast/blast_seg.cpp \
  src/data/queries.cpp \
  src/data/reference.cpp \
  src/data/seed_histogram.cpp \
  src/output/daa_record.cpp \
  src/search/search.cpp \
  src/util/command_line_parser.cpp \
  src/util/seq_file_format.cpp \
  src/util/util.cpp  \
  src/util/Timer.cpp \
  src/basic/basic.cpp \
  src/basic/hssp.cpp \
  src/dp/ungapped_align.cpp \
  src/run/tools.cpp \
  src/dp/greedy_align.cpp \
  src/run/benchmark.cpp \
  src/search/stage2.cpp \
  src/output/output_format.cpp \
  src/output/join_blocks.cpp \
  src/data/frequent_seeds.cpp \
  src/align/query_mapper.cpp \
  src/align/align_target.cpp \
  src/output/blast_tab_format.cpp \
  src/dp/padded_banded_sw.cpp \
  src/dp/needleman_wunsch.cpp \
  src/output/blast_pairwise_format.cpp \
  src/extra/roc.cpp \
  src/dp/comp_based_stats.cpp \
  src/extra/model_sim.cpp \
  src/run/double_indexed.cpp \
  src/search/collision.cpp \
  src/output/sam_format.cpp \
  src/align/align.cpp \
  src/search/setup.cpp \
  src/extra/opt.cpp \
  src/dp/diag_scores.cpp \
  src/data/taxonomy.cpp \
  src/lib/tantan/tantan.cc \
  src/basic/masking.cpp \
  src/dp/swipe.cpp \
  src/dp/banded_sw.cpp \
  src/data/sorted_list.cpp \
  src/data/seed_set.cpp \
  src/util/binary_file.cpp \
  src/util/simd.cpp \
  src/output/taxon_format.cpp \
  src/output/view.cpp \
  src/output/output_sink.cpp \
  src/output/target_culling.cpp \
-lz -lpthread -o diamond
