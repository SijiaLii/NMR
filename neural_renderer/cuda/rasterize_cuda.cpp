#include <torch/torch.h>

#include <vector>

// CUDA forward declarations

std::vector<at::Tensor> forward_face_index_map_cuda(
        at::Tensor faces,
        at::Tensor face_index_map,
        at::Tensor weight_map,
        at::Tensor depth_map,
        at::Tensor face_inv_map,
        at::Tensor faces_inv,
        at::Tensor face_visibility,
        at::Tensor face_list,
        int image_size,
        int block_size,
        float near,
        float far,
        int return_rgb,
        int return_alpha,
        int return_depth,
        int return_visibility);

std::vector<at::Tensor> forward_texture_sampling_cuda(
        at::Tensor faces,
        at::Tensor textures,
        at::Tensor face_index_map,
        at::Tensor weight_map,
        at::Tensor depth_map,
        at::Tensor rgb_map,
        at::Tensor sampling_index_map,
        at::Tensor sampling_weight_map,
        int image_size,
        float eps);

at::Tensor backward_pixel_map_cuda(
        at::Tensor faces,
        at::Tensor face_index_map,
        at::Tensor rgb_map,
        at::Tensor alpha_map, at::Tensor grad_rgb_map,
        at::Tensor grad_alpha_map,
        at::Tensor grad_faces,
        int image_size,
        float eps,
        int return_rgb,
        int return_alpha);

at::Tensor backward_textures_cuda(
        at::Tensor face_index_map,
        at::Tensor sampling_weight_map,
        at::Tensor sampling_index_map,
        at::Tensor grad_rgb_map,
        at::Tensor grad_textures,
        int num_faces);

at::Tensor backward_depth_map_cuda(
        at::Tensor faces,
        at::Tensor depth_map,
        at::Tensor face_index_map,
        at::Tensor face_inv_map,
        at::Tensor weight_map,
        at::Tensor grad_depth_map,
        at::Tensor grad_faces,
        int image_size);

// C++ interface

// #define CHECK_CUDA(x) AT_CHECK(x.type().is_cuda(), #x " must be a CUDA tensor")
// #define CHECK_CONTIGUOUS(x) AT_CHECK(x.is_contiguous(), #x " must be contiguous")
// #define CHECK_CUDA(x) TORCH_CHECK(x.type().is_cuda(), #x " must be a CUDA tensor")
#define CHECK_CUDA(x) TORCH_CHECK(x.is_cuda(),      #x " must be a CUDA tensor")
#define CHECK_CONTIGUOUS(x) TORCH_CHECK(x.is_contiguous(), #x " must be contiguous")
#define CHECK_INPUT(x) CHECK_CUDA(x); CHECK_CONTIGUOUS(x)

std::vector<at::Tensor> forward_face_index_map(
        at::Tensor faces,
        at::Tensor face_index_map,
        at::Tensor weight_map,
        at::Tensor depth_map,
        at::Tensor face_inv_map,
        at::Tensor faces_inv,
        at::Tensor face_visibility,
        at::Tensor face_list,
        int image_size,
        int block_size,
        float near,
        float far,
        int return_rgb,
        int return_alpha,
        int return_depth,
        int return_visibility) {

    CHECK_INPUT(faces);
    CHECK_INPUT(face_index_map);
    CHECK_INPUT(weight_map);
    CHECK_INPUT(depth_map);
    CHECK_INPUT(face_inv_map);
    CHECK_INPUT(faces_inv);

    return forward_face_index_map_cuda(faces, face_index_map, weight_map,
                                       depth_map, face_inv_map, faces_inv,
                                       face_visibility, face_list,
                                       image_size, block_size, near, far,
                                       return_rgb, return_alpha, return_depth, return_visibility);
}

std::vector<at::Tensor> forward_texture_sampling(
        at::Tensor faces,
        at::Tensor textures,
        at::Tensor face_index_map,
        at::Tensor weight_map,
        at::Tensor depth_map,
        at::Tensor rgb_map,
        at::Tensor sampling_index_map,
        at::Tensor sampling_weight_map,
        int image_size,
        float eps) {

    CHECK_INPUT(faces);
    CHECK_INPUT(textures);
    CHECK_INPUT(face_index_map);
    CHECK_INPUT(weight_map);
    CHECK_INPUT(depth_map);
    CHECK_INPUT(rgb_map);
    CHECK_INPUT(sampling_index_map);
    CHECK_INPUT(sampling_weight_map);

    return forward_texture_sampling_cuda(faces, textures, face_index_map,
                                    weight_map, depth_map, rgb_map,
                                    sampling_index_map, sampling_weight_map,
                                    image_size, eps);
}

at::Tensor backward_pixel_map(
        at::Tensor faces,
        at::Tensor face_index_map,
        at::Tensor rgb_map,
        at::Tensor alpha_map,
        at::Tensor grad_rgb_map,
        at::Tensor grad_alpha_map,
        at::Tensor grad_faces,
        int image_size,
        float eps,
        int return_rgb,
        int return_alpha) {

    CHECK_INPUT(faces);
    CHECK_INPUT(face_index_map);
    CHECK_INPUT(rgb_map);
    CHECK_INPUT(alpha_map);
    CHECK_INPUT(grad_rgb_map);
    CHECK_INPUT(grad_alpha_map);
    CHECK_INPUT(grad_faces);

    return backward_pixel_map_cuda(faces, face_index_map, rgb_map, alpha_map,
                                   grad_rgb_map, grad_alpha_map, grad_faces,
                                   image_size, eps, return_rgb, return_alpha);
}

at::Tensor backward_textures(
        at::Tensor face_index_map,
        at::Tensor sampling_weight_map,
        at::Tensor sampling_index_map,
        at::Tensor grad_rgb_map,
        at::Tensor grad_textures,
        int num_faces) {

    CHECK_INPUT(face_index_map);
    CHECK_INPUT(sampling_weight_map);
    CHECK_INPUT(sampling_index_map);
    CHECK_INPUT(grad_rgb_map);
    CHECK_INPUT(grad_textures);

    return backward_textures_cuda(face_index_map, sampling_weight_map,
                                  sampling_index_map, grad_rgb_map,
                                  grad_textures, num_faces);
}

at::Tensor backward_depth_map(
        at::Tensor faces,
        at::Tensor depth_map,
        at::Tensor face_index_map,
        at::Tensor face_inv_map,
        at::Tensor weight_map,
        at::Tensor grad_depth_map,
        at::Tensor grad_faces,
        int image_size) {

    CHECK_INPUT(faces);
    CHECK_INPUT(depth_map);
    CHECK_INPUT(face_index_map);
    CHECK_INPUT(face_inv_map);
    CHECK_INPUT(weight_map);
    CHECK_INPUT(grad_depth_map);
    CHECK_INPUT(grad_faces);

    return backward_depth_map_cuda(faces, depth_map, face_index_map,
                                   face_inv_map, weight_map,
                                   grad_depth_map, grad_faces,
                                   image_size);
}

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
    m.def("forward_face_index_map", &forward_face_index_map, "FORWARD_FACE_INDEX_MAP (CUDA)");
    m.def("forward_texture_sampling", &forward_texture_sampling, "FORWARD_TEXTURE_SAMPLING (CUDA)");
    m.def("backward_pixel_map", &backward_pixel_map, "BACKWARD_PIXEL_MAP (CUDA)");
    m.def("backward_textures", &backward_textures, "BACKWARD_TEXTURES (CUDA)");
    m.def("backward_depth_map", &backward_depth_map, "BACKWARD_DEPTH_MAP (CUDA)");
}
