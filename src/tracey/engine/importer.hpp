#ifndef IMPORTER_HPP
#define IMPORTER_HPP

#include <string>

#include "../../lib/assimp.hpp"
#include "../../lib/glm.hpp"

#include "scene.hpp"
#include "../graphics/shader_pack.hpp"
#include "../graphics/material.hpp"

namespace trc {

class Importer {
public:
    bool load_file(std::string file_path, ShaderPack *shader_pack);

    Scene get_loaded_scene();
    std::string get_error_string();

private:
    Scene loaded_scene;
    std::string error_string = "Success";

    Scene assimp_to_trc(const aiScene *scene, ShaderPack *shader_pack);
    void import_assimp_hierarchy(const aiScene *scene, const aiNode *root_node, const glm::mat4 transform, Scene *trc_scene, ShaderPack *shader_pack);
    void import_mesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 transform, Scene *trc_scene, ShaderPack *shader_pack);
    void import_light(const aiLight *light, const aiScene *scene, const aiNode *root_node, Scene *trc_scene);
    void import_material(const aiMaterial *mat, Scene *trc_scene);
};

} /* trc */

#endif /* end of include guard: IMPORTER_HPP */
