#include <assert.h>

#include <chrono>
#include <filesystem>
namespace fs = std::filesystem;
#include <iostream>
#include <set>
#include <utility>

/* Reference parser */
#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"
/* Custom parser */
#include "owviewer.h"
#include "owviewer/details.h"

using namespace std::chrono;
struct Timer {
  high_resolution_clock::time_point start, stop;
  static size_t last_time;

  Timer() { start = high_resolution_clock::now(); }

  ~Timer() {
    stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    last_time = duration.count();

    std::cout << duration.count() << "ms" << std::endl;
  }
};

#define dir "/Users/mark/Downloads/3Dmodels/"
/* models */
#define lion "Lion.obj"                     // 2.5m + 5m
#define cube "cube_not_centered.obj"        // 3m + 6m
#define romanesco "Romanesco Top.obj"       // 3.5m
#define bethoven "Beethoven_Denkmal_C.obj"  // 567k + 1.1m
#define car911 "911_scene.obj"              // 150k + 120k
#define scull "SKULLlow.obj"                // 150k + 120k

size_t Timer::last_time = 0;
std::string path = dir;

int main() {
  path += romanesco;
  std::cout << "file: " << fs::path(path.c_str()).filename() << '\n';

  std::cout << "fast_obj parsing: ";
  fastObjMesh *m = nullptr;
  {
    Timer t;
    m = fast_obj_read(path.c_str());
  }
  assert(m);
  fast_obj_destroy(m);

  size_t time_total = 0;
  std::cout << "custom parser: ";
  OWV_Mesh *m2 = nullptr;
  {
    Timer t;
    m2 = owv_mesh_read_obj(path.c_str());
  }
  time_total += Timer::last_time;
  assert(m2);
  std::cout << "model statistics: ";
  std::cout << "vertices:" << m2->vertex_count << " | ";
  std::cout << "faces:" << m2->face_count << " | ";
  std::cout << "polygon edges:" << m2->index_count << std::endl;

  std::cout << "bounding box search: ";
  OWV_MeshBounds mb;
  {
    Timer t;
    mb = owv_mesh_find_bounds(m2);
    (void)mb;
  }
  time_total += Timer::last_time;

  std::cout << "building of the lines index array: ";
  unsigned int *iarr = nullptr;
  {
    Timer t;
    iarr = owv_iarr_to_lines(m2);
  }
  assert(iarr);
  time_total += Timer::last_time;

  size_t time_duplicates = 0;
  std::cout << "removing line duplicates {\n";
  std::cout << "\tflipping lines in the index array: ";
  {
    Timer t;
    owv_iarr_lines_flip(iarr, m2->index_count * 2);
  }
  time_duplicates += Timer::last_time;

  std::cout << "\tsorting of the lines index array: ";
  {
    Timer t;
    /* set */
    //    std::set<std::pair<unsigned int, unsigned int> > s;
    //    for (int i = 0; i < m2->index_count * 2; i += 2) {
    //      s.insert(std::make_pair(iarr[i], iarr[i + 1]));
    //    }
    //    std::cout << s.size() << " ";
    /* unordered multimap */
    //    std::unordered_multimap<unsigned int, unsigned int> umap;
    //    for (int i = 0; i < m2->index_count * 2; i += 2) {
    //      umap.insert(std::make_pair(iarr[i], iarr[i + 1]));
    //    }
    //    std::cout << umap.size() << " ";
    /* qsort */
    owv_iarr_lines_sort(iarr, m2->index_count * 2);
  }
  time_duplicates += Timer::last_time;

  std::cout << "\tdeleting index duplicates in the index array: ";
  size_t newlen = 0;
  {
    Timer t;
    iarr = owv_iarr_lines_clean(iarr, m2->index_count * 2, &newlen);
  }
  time_duplicates += Timer::last_time;
  std::cout << "\tunique edges count: " << newlen / 2 << std::endl;
  owv_mesh_destroy(m2);

  std::cout << "} " << time_duplicates << "ms\n";
  time_total += time_duplicates;
  std::cout << "total: " << time_total << "ms" << std::endl;
}
