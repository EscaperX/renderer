#pragma once

namespace cc
{
    struct ProfileStat
    {
        int vertex_stage_vertex_num;
        int vertex_stage_face_num;
        float vertex_stage_time;

        int frustum_culling_vertex_num;
        int frustum_culling_face_num;
        float frustum_culling_time;

        int early_z_face_num;
        float early_z_time;

        int raster_face_num;
        int raster_fragment_num;
        float raster_fragment_time;

        int shading_num;
        float shading_time;
    };
}