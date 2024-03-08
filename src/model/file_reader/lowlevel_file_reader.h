#ifndef LOWLEVEL_FILE_READER_BASE_H_
#define LOWLEVEL_FILE_READER_BASE_H_

#include "file_reader_base.h"
#include "lowlevel_parser.h"
#include "processing.h"
#include "scene.h"

class LowlevelFileReader : public FileReaderBase {
 public:
  Scene ReadScene(std::string path) override {
    OWV_Mesh *m = owv_mesh_read_obj(path.c_str());

    Figure figure;

    figure.vertices_ =
        std::vector(m->positions, m->positions + m->vertex_count * 3);
    figure.face_vertex_counts_ = std::vector(
        m->face_vertex_counts, m->face_vertex_counts + m->face_count);
    figure.indices_ = std::vector(m->indices, m->indices + m->index_count);

    figure.stats_.faces_cnt = m->face_count;
    figure.stats_.vertices_cnt = m->vertex_count;
    figure.stats_.edges_cnt = m->index_count;

    /* Break the index array of faces into the index array of UNIQUE lines */
    size_t unique_edges_cnt;
    auto uliarr = owv_iarr_to_unique_lines(m, &unique_edges_cnt);
    figure.line_index_array_ = std::vector(uliarr, uliarr + unique_edges_cnt);

    /* Break the index array of faces into the array of index arrays by faces */
    //  owv_index_arr_to_2d_arr(m);
    /* Break the index array of faces into the index array of lines */
    //  this->index_array = owv_to_lines_index_arr(m);
    //  this->index_count = m->index_count * 2;

    /* Create a normalization matrix for the model */
    auto mb = owv_mesh_find_bounds(m);
    const float maxlen = mb.maxlen;
    auto scale_norm = TransformMatrixBuilder::CreateScaleMatrix(
        2 / maxlen, 2 / maxlen, 2 / maxlen);
    auto translation_norm = TransformMatrixBuilder::CreateTranslationMatrix(
        -mb.xcen, -mb.ycen, -mb.zcen);
    figure.normalization_matrix_ = scale_norm * translation_norm;

    // as we have only one figure in scene for now
    scene_.ReplaceFigure(figure);
    scene_.set_normalization_matrix(figure.normalization_matrix_);

    owv_mesh_destroy(m);
    return scene_;
  }

 private:
  Scene scene_;
};

#endif  // LOWLEVEL_FILE_READER_BASE_H_
