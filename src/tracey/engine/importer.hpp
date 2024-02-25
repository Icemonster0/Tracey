#ifndef IMPORTER_HPP
#define IMPORTER_HPP

#include <string>
#include <cstring>
#include <cstdint>

#include "../../lib/assimp.hpp"
#include "../../lib/glm.hpp"

#include "scene.hpp"
#include "../graphics/shader_pack.hpp"
#include "../graphics/material.hpp"
#include "../graphics/image_read_write.hpp"

namespace trc {

class Importer {
public:
    bool load_file(std::string file_path, ShaderPack *shader_pack);

    Scene get_loaded_scene();
    std::string get_error_string();

private:
    Scene loaded_scene;
    std::string error_string = "Success";

    Scene assimp_to_trc(const aiScene *scene, ShaderPack *shader_pack, std::string path);
    void import_assimp_hierarchy(const aiScene *scene, const aiNode *root_node, const glm::mat4 transform, Scene *trc_scene, ShaderPack *shader_pack);
    void import_mesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 transform, Scene *trc_scene, ShaderPack *shader_pack);
    void import_light(const aiLight *light, const aiScene *scene, const aiNode *root_node, Scene *trc_scene);
    void import_material(const aiMaterial *mat, const aiScene *scene, Scene *trc_scene, std::string path);

    void import_material_attrib(
        std::shared_ptr<Attrib<glm::vec3>> &attrib,
        aiColor3D default_value,
        const char *pKey, unsigned int type, unsigned int idx,
        aiTextureType tex_type,
        const aiMaterial *mat,
        const aiScene *scene,
        std::string path);
    void import_material_attrib(
        std::shared_ptr<Attrib<float>> &attrib,
        float default_value,
        const char *pKey, unsigned int type, unsigned int idx,
        aiTextureType tex_type,
        const aiMaterial *mat,
        const aiScene *scene,
        std::string path);

    template <typename T>
    AttribTexture<T> import_texture(const aiMaterial *mat, aiTextureType tex_type, const aiScene *scene, std::string path) {
        aiString texture_file;
        mat->Get(AI_MATKEY_TEXTURE(tex_type, 0), texture_file);

        // get full relative path
        path.append(texture_file.C_Str());

        if (strlen(texture_file.C_Str()) == 0) {
            // texture doesn't exist
            return AttribTexture<T>();
        }

        else if (const aiTexture *texture = scene->GetEmbeddedTexture(texture_file.C_Str())) {
            // texture is embedded
            AttribTexture<T> trc_texture;
            printf("Loading embedded texture: %s", texture_file.C_Str());
            if (texture->mHeight == 0) {
                // texture is compressed
                if (image_rw::read_texture_from_memory((uint8_t*)texture->pcData, (int)texture->mWidth, trc_texture) != 0) {
                    printf("    Failed\n");
                    make_texture_invalid(trc_texture);
                } else printf("    Success\n");
            }
            else {
                // texture data is uncompressed
                aiTexture_to_AttribTexture(texture, trc_texture);
                printf("    Success\n");
            }
            return std::move(trc_texture);
        }

        else {
            // texture is a regular file
            printf("Loading texture file: %s", path.c_str());
            AttribTexture<T> trc_texture;
            if (image_rw::read_texture(path.c_str(), trc_texture) != 0) {
                printf("    Failed\n");
                make_texture_invalid(trc_texture);
            } else printf("    Success\n");
            return std::move(trc_texture);
        }
    }

    void aiTexture_to_AttribTexture(const aiTexture *texture, AttribTexture<glm::vec3> &trc_texture);
    void aiTexture_to_AttribTexture(const aiTexture *texture, AttribTexture<float> &trc_texture);

    void make_texture_invalid(AttribTexture<glm::vec3> &trc_texture);
    void make_texture_invalid(AttribTexture<float> &trc_texture);
};

} /* trc */

#endif /* end of include guard: IMPORTER_HPP */
