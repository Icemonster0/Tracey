#include "importer.hpp"

#include <vector>
#include <memory>

#include "../util/math_util.hpp"
#include "../graphics/color_spaces.hpp"

namespace trc {

// public

bool Importer::load_file(std::string file_path, ShaderPack *shader_pack, glm::mat4 transform) {
    // setup assimp and load file
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file_path,
        aiProcess_CalcTangentSpace         |
        aiProcess_Triangulate              |
        aiProcess_JoinIdenticalVertices    |
        aiProcess_SortByPType              |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_ValidateDataStructure    |
        aiProcess_FixInfacingNormals);

    // check assimp errors
    if (scene == nullptr) {
        error_string = importer.GetErrorString();
        return false;
    }

    // construct a trc::Scene
    std::string path = file_path.substr(0, file_path.find_last_of("\\/")+1);
    loaded_scene = assimp_to_trc(scene, shader_pack, transform, path);

    return true;
}

Scene Importer::get_loaded_scene() {
    return std::move(loaded_scene);
}

std::string Importer::get_error_string() {
    return error_string;
}

// private

Scene Importer::assimp_to_trc(const aiScene *scene, ShaderPack *shader_pack, glm::mat4 transform, std::string path) {
    Scene trc_scene;

    // materials
    for (int i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial *mat = scene->mMaterials[i];
        import_material(mat, scene, &trc_scene, path);
    }

    // meshes
    aiNode *root_node = scene->mRootNode;
    import_assimp_hierarchy(scene, root_node, glm::transpose(transform), &trc_scene, shader_pack);

    // lights
    for (int i = 0; i < scene->mNumLights; ++i) {
        aiLight *light = scene->mLights[i];
        import_light(light, scene, root_node, &trc_scene);
    }

    return std::move(trc_scene);
}

void Importer::import_assimp_hierarchy(const aiScene *scene, const aiNode *root_node, const glm::mat4 transform, Scene *trc_scene, ShaderPack *shader_pack) {
    glm::mat4 new_transform = glm::make_mat4(root_node->mTransformation[0]) * transform;

    for (int i = 0; i < root_node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[root_node->mMeshes[i]];
        import_mesh(mesh, scene, new_transform, trc_scene, shader_pack);
    }

    for (int i = 0; i < root_node->mNumChildren; ++i) {
        aiNode *node = root_node->mChildren[i];
        import_assimp_hierarchy(scene, node, new_transform, trc_scene, shader_pack);
    }
}

void Importer::import_mesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 transform, Scene *trc_scene, ShaderPack *shader_pack) {
    std::vector<glm::vec3> pos_vec;
    std::vector<glm::vec3> normal_vec;
    std::vector<glm::vec3> tan_vec;
    std::vector<glm::vec3> bitan_vec;
    std::vector<glm::vec2> tex_vec;

    pos_vec.reserve(mesh->mNumVertices);
    for (int i = 0; i < mesh->mNumVertices; ++i) {
        aiVector3D pos = mesh->mVertices[i];

        glm::vec4 p {pos[0], pos[1], pos[2], 1.f};
        p = p * transform;
        pos_vec.push_back(p.xyz());
    }

    if (mesh->HasNormals()) {
        glm::mat4 normal_transform = glm::transpose(glm::inverse(transform));
        normal_vec.reserve(mesh->mNumVertices);
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            aiVector3D normal = mesh->mNormals[i];

            glm::vec4 n {normal[0], normal[1], normal[2], 1.f};
            n = n * normal_transform;
            normal_vec.push_back(math::normalize(n.xyz()));
        }
    }

    if (mesh->HasTangentsAndBitangents()) {
        tan_vec.reserve(mesh->mNumVertices);
        bitan_vec.reserve(mesh->mNumVertices);
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            aiVector3D tangent = mesh->mTangents[i];
            aiVector3D bitangent = mesh->mBitangents[i];

            glm::vec4 t {tangent[0], tangent[1], tangent[2], 1.f};
            glm::vec4 bt {bitangent[0], bitangent[1], bitangent[2], 1.f};
            t = t * transform;
            bt = bt * transform;
            tan_vec.push_back(math::normalize(t.xyz()));
            bitan_vec.push_back(math::normalize(bt.xyz()));
        }
    }

    if (mesh->GetNumUVChannels() > 0 && mesh->HasTextureCoords(0)) {
        tex_vec.reserve(mesh->mNumVertices);
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            aiVector3D tex = mesh->mTextureCoords[0][i];
            tex_vec.emplace_back(tex[0], tex[1]);
        }
    }

    Mesh *trc_mesh = trc_scene->add_mesh(std::make_unique<Mesh>(pos_vec, normal_vec, tan_vec, bitan_vec, tex_vec));

    auto mat_it = trc_scene->get_material_list()->begin();
    std::advance(mat_it, mesh->mMaterialIndex);
    Material *trc_mat = (*mat_it).get();

    for (int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        glm::ivec3 indices {
            face.mIndices[0],
            face.mIndices[1],
            face.mIndices[2]
        };
        trc_scene->add_object(std::unique_ptr<Shape>(new Triangle(indices, trc_mesh, shader_pack->shader_combined.get(), trc_mat)));
    }
}

void Importer::import_light(const aiLight *light, const aiScene *scene, const aiNode *root_node, Scene *trc_scene) {
    aiVector3D pos = light->mPosition;
    aiVector3D dir = light->mDirection;
    aiColor3D color = light->mColorDiffuse;
    aiVector2D size = light->mSize;

    glm::vec3 trc_pos {pos[0], pos[1], pos[2]};
    glm::vec3 trc_dir {dir[0], dir[1], dir[2]};
    glm::vec3 trc_color {color[0], color[1], color[2]};
    glm::vec2 trc_size {size[0], size[1]};

    const aiNode *light_node = root_node->FindNode(light->mName);
    glm::mat4 transform {1.f};
    const aiNode *i_node = light_node;
    while (true) {
        transform = transform * glm::make_mat4(i_node->mTransformation[0]);
        if (i_node == root_node) break;
        i_node = i_node->mParent;
    }

    glm::vec3 trash3;
    glm::vec4 trash4;
    glm::quat rotation;
    glm::decompose(transform, trash3, rotation, trash3, trash3, trash4);

    trc_pos = glm::vec3 {glm::vec4 {trc_pos, 1.f} * transform};
    trc_dir = math::normalize(glm::vec3 {glm::vec4 {trc_dir, 1.f} * rotation});

    switch (light->mType) {
        case aiLightSource_DIRECTIONAL:
            trc_scene->add_light(std::unique_ptr<Light>(new SunLight(trc_dir, trc_color / 700)));
            break;
        case aiLightSource_POINT:
            trc_scene->add_light(std::unique_ptr<Light>(new PointLight(trc_pos, trc_color / 700)));
            break;
        case aiLightSource_SPOT:
        case aiLightSource_UNDEFINED:
        default:
            trc_scene->add_light(std::unique_ptr<Light>(new Light(trc_pos, trc_color)));
            break;
    }
}

void Importer::import_material(const aiMaterial *mat, const aiScene *scene, Scene *trc_scene, std::string path) {
    // initialize deafult values
    aiColor3D albedo {1.f};
    float roughness = 0.5f;
    float metallic = 0.f;
    aiColor3D emission {0.f};
    float emission_intensity = 1.f;
    aiColor3D normal {0.5f, 0.5f, 1.f};
    float transmissive = 0.f;
    float ior = 1.6f;

    // albedo
    mat->Get(AI_MATKEY_BASE_COLOR, albedo);
    glm::vec3 trc_albedo {albedo[0], albedo[1], albedo[2]};
    std::shared_ptr<Attrib<glm::vec3>> attrib_albedo;
    AttribTexture<glm::vec3> albedo_tex = import_texture<glm::vec3>(mat, aiTextureType_BASE_COLOR, scene, path);
    if (albedo_tex.get_size() != glm::ivec2 {0}) {
        for (int x = 0; x < albedo_tex.get_size().x; ++x) {
            for (int y = 0; y < albedo_tex.get_size().y; ++y) {
                albedo_tex.set_pixel({x, y}, color::sRGB_to_flat(albedo_tex.get_pixel({x, y})));
            }
        }
        attrib_albedo = std::shared_ptr<Attrib<glm::vec3>>(new AttribTexture(albedo_tex));
    } else {
        attrib_albedo = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(trc_albedo));
    }

    // roughness
    std::shared_ptr<Attrib<float>> attrib_roughness;
    import_material_attrib(attrib_roughness, roughness, AI_MATKEY_ROUGHNESS_FACTOR, aiTextureType_DIFFUSE_ROUGHNESS, mat, scene, path);

    // metallic
    std::shared_ptr<Attrib<float>> attrib_metallic;
    import_material_attrib(attrib_metallic, metallic, AI_MATKEY_METALLIC_FACTOR, aiTextureType_METALNESS, mat, scene, path);

    // emission
    mat->Get(AI_MATKEY_COLOR_EMISSIVE, emission);
    mat->Get(AI_MATKEY_EMISSIVE_INTENSITY, emission_intensity);
    glm::vec3 trc_emission {emission[0], emission[1], emission[2]};
    trc_emission *= emission_intensity;
    std::shared_ptr<Attrib<glm::vec3>> attrib_emission;
    AttribTexture<glm::vec3> emission_tex = import_texture<glm::vec3>(mat, aiTextureType_EMISSION_COLOR, scene, path);
    if (emission_tex.get_size() != glm::ivec2 {0}) {
        for (int x = 0; x < emission_tex.get_size().x; ++x) {
            for (int y = 0; y < emission_tex.get_size().y; ++y) {
                emission_tex.set_pixel({x, y}, emission_tex.get_pixel({x, y}) * emission_intensity);
            }
        }
        attrib_emission = std::shared_ptr<Attrib<glm::vec3>>(new AttribTexture(emission_tex));
    } else {
        attrib_emission = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(trc_emission));
    }

    // normal
    glm::vec3 trc_normal {normal[0], normal[1], normal[2]};
    std::shared_ptr<Attrib<glm::vec3>> attrib_normal;
    AttribTexture<glm::vec3> normal_tex = import_texture<glm::vec3>(mat, aiTextureType_NORMALS, scene, path);
    if (normal_tex.get_size() != glm::ivec2 {0}) {
        attrib_normal = std::shared_ptr<Attrib<glm::vec3>>(new AttribTexture(normal_tex));
    } else {
        attrib_normal = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(trc_normal));
    }

    // transmissive
    std::shared_ptr<Attrib<float>> attrib_transmissive;
    import_material_attrib(attrib_transmissive, transmissive, AI_MATKEY_TRANSMISSION_FACTOR, aiTextureType_TRANSMISSION, mat, scene, path);

    // ior
    mat->Get(AI_MATKEY_REFRACTI, ior);
    std::shared_ptr<Attrib<float>> attrib_ior = std::shared_ptr<Attrib<float>>(new AttribValue(ior));

    // create the new material
    trc_scene->add_material(std::make_unique<Material>(
        attrib_albedo,
        attrib_roughness,
        attrib_metallic,
        attrib_emission,
        attrib_normal,
        attrib_transmissive,
        attrib_ior
    ));
}

// instead of supplying (pKey, type, idx), use the macros AI_MATKEY_XXX which will provide the three values
void Importer::import_material_attrib(
        std::shared_ptr<Attrib<glm::vec3>> &attrib,
        aiColor3D default_value,
        const char *pKey, unsigned int type, unsigned int idx,
        aiTextureType tex_type,
        const aiMaterial *mat,
        const aiScene *scene,
        std::string path) {

    mat->Get(pKey, type, idx, default_value);
    glm::vec3 value {default_value[0], default_value[1], default_value[2]};

    // check for a texture and import it if found
    AttribTexture<glm::vec3> texture = import_texture<glm::vec3>(mat, tex_type, scene, path);
    if (texture.get_size() != glm::ivec2 {0}) {
        attrib = std::shared_ptr<Attrib<glm::vec3>>(new AttribTexture(texture));
    } else {
        attrib = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(value));
    }
}

// instead of supplying (pKey, type, idx), use the macros AI_MATKEY_XXX which will provide the three values
void Importer::import_material_attrib(
        std::shared_ptr<Attrib<float>> &attrib,
        float default_value,
        const char *pKey, unsigned int type, unsigned int idx,
        aiTextureType tex_type,
        const aiMaterial *mat,
        const aiScene *scene,
        std::string path) {

    mat->Get(pKey, type, idx, default_value);

    // check for a texture and import it if found
    AttribTexture<float> texture = import_texture<float>(mat, tex_type, scene, path);
    if (texture.get_size() != glm::ivec2 {0}) {
        attrib = std::shared_ptr<Attrib<float>>(new AttribTexture(texture));
    } else {
        attrib = std::shared_ptr<Attrib<float>>(new AttribValue(default_value));
    }
}

void Importer::aiTexture_to_AttribTexture(const aiTexture *texture, AttribTexture<glm::vec3> &trc_texture) {
    glm::ivec2 size {texture->mWidth, texture->mHeight};
    trc_texture.set_size(size);
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            int offset = y * size.x + x;
            float r = float(texture->pcData[offset].r) / 255.f;
            float g = float(texture->pcData[offset].g) / 255.f;
            float b = float(texture->pcData[offset].b) / 255.f;

            trc_texture.set_pixel({x, y}, glm::saturate(glm::vec3 {r, g, b}));
        }
    }
}

void Importer::aiTexture_to_AttribTexture(const aiTexture *texture, AttribTexture<float> &trc_texture) {
    glm::ivec2 size {texture->mWidth, texture->mHeight};
    trc_texture.set_size(size);
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            int offset = y * size.x + x;
            float r = float(texture->pcData[offset].r) / 255.f;
            float g = float(texture->pcData[offset].g) / 255.f;
            float b = float(texture->pcData[offset].b) / 255.f;

            trc_texture.set_pixel({x, y}, std::clamp((r+g+b) / 3.f, 0.f, 1.f));
        }
    }
}

void Importer::make_texture_invalid(AttribTexture<glm::vec3> &trc_texture) {
    trc_texture.set_size(glm::ivec2 {1, 1});
    trc_texture.fill(glm::vec3 {1.f, 0.f, 1.f});
}

void Importer::make_texture_invalid(AttribTexture<float> &trc_texture) {
    trc_texture.set_size(glm::ivec2 {1, 1});
    trc_texture.fill(0.f);
}

} /* trc */
