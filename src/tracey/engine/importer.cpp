#include "importer.hpp"

#include <vector>
#include <memory>

#include "../util/math_util.hpp"

namespace trc {

bool Importer::load_file(std::string file_path, ShaderPack *shader_pack) {
    // setup assimp and load file
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file_path,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

    // check assimp errors
    if (scene == nullptr) {
        error_string = importer.GetErrorString();
        return false;
    }

    // construct a trc::Scene
    loaded_scene = assimp_to_trc(scene, shader_pack);

    return true;
}

Scene Importer::get_loaded_scene() {
    return std::move(loaded_scene);
}

std::string Importer::get_error_string() {
    return error_string;
}

Scene Importer::assimp_to_trc(const aiScene *scene, ShaderPack *shader_pack) {
    Scene trc_scene;

    // materials
    for (int i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial *mat = scene->mMaterials[i];
        import_material(mat, &trc_scene);
    }

    // meshes
    aiNode *root_node = scene->mRootNode;
    import_assimp_hierarchy(scene, root_node, glm::mat4 {1.f}, &trc_scene, shader_pack);

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
    std::vector<glm::vec2> tex_vec;

    pos_vec.reserve(mesh->mNumVertices);
    for (int i = 0; i < mesh->mNumVertices; ++i) {
        aiVector3D pos = mesh->mVertices[i];

        glm::vec4 p {pos[0], pos[1], pos[2], 1.f};
        p = p * transform;
        pos_vec.push_back(p.xyz());
    }

    glm::vec3 trash3;
    glm::vec4 trash4;
    glm::quat rotation;
    glm::decompose(transform, trash3, rotation, trash3, trash3, trash4);

    if (mesh->HasNormals()) {
        normal_vec.reserve(mesh->mNumVertices);
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            aiVector3D normal = mesh->mNormals[i];

            glm::vec4 n {normal[0], normal[1], normal[2], 1.f};
            n = n * rotation;
            normal_vec.push_back(n.xyz());
        }
    }

    if (mesh->GetNumUVChannels() > 0 && mesh->HasTextureCoords(0)) {
        tex_vec.reserve(mesh->mNumVertices);
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            aiVector3D tex = mesh->mTextureCoords[0][i];
            tex_vec.emplace_back(tex[0], tex[1]);
        }
    }

    Mesh *trc_mesh = trc_scene->add_mesh(std::make_unique<Mesh>(pos_vec, normal_vec, tex_vec));

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
        trc_scene->add_object(std::unique_ptr<Shape>(new Triangle(indices, indices, indices, trc_mesh, shader_pack->shader_combined.get(), trc_mat)));
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
            trc_scene->add_light(std::unique_ptr<Light>(new SunLight(trc_dir, trc_color / 500)));
            break;
        case aiLightSource_POINT:
            trc_scene->add_light(std::unique_ptr<Light>(new PointLight(trc_pos, trc_color / 500)));
            break;
        case aiLightSource_SPOT:
        case aiLightSource_UNDEFINED:
        default:
            trc_scene->add_light(std::unique_ptr<Light>(new Light(trc_pos, trc_color)));
            break;
    }
}

void Importer::import_material(const aiMaterial *mat, Scene *trc_scene) {
    // initialize deafult values
    aiColor3D albedo {1.f};
    float roughness = 0.5f;
    float metallic = 0.f;
    aiColor3D emission {0.f};
    aiColor3D normal {0.5f, 0.5f, 1.f};
    float transmissive = 0.f;
    float ior = 1.6f;

    // get values from assimp
    mat->Get(AI_MATKEY_BASE_COLOR, albedo);
    mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
    mat->Get(AI_MATKEY_METALLIC_FACTOR, metallic);

    float emission_intensity = 1.f;
    mat->Get(AI_MATKEY_COLOR_EMISSIVE, emission);
    mat->Get(AI_MATKEY_EMISSIVE_INTENSITY, emission_intensity);

    mat->Get(AI_MATKEY_TRANSMISSION_FACTOR, transmissive);
    mat->Get(AI_MATKEY_REFRACTI, ior);

    // translate to tracey (glm)
    glm::vec3 trc_albedo {albedo[0], albedo[1], albedo[2]};
    glm::vec3 trc_emission {emission[0], emission[1], emission[2]};
    trc_emission *= emission_intensity;
    glm::vec3 trc_normal {normal[0], normal[1], normal[2]};

    std::shared_ptr<Attrib<glm::vec3>> attrib_albedo = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(trc_albedo));
    std::shared_ptr<Attrib<float>> attrib_roughness = std::shared_ptr<Attrib<float>>(new AttribValue(roughness));
    std::shared_ptr<Attrib<float>> attrib_metallic = std::shared_ptr<Attrib<float>>(new AttribValue(metallic));
    std::shared_ptr<Attrib<glm::vec3>> attrib_emission = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(trc_emission));
    std::shared_ptr<Attrib<glm::vec3>> attrib_normal = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(trc_normal));
    std::shared_ptr<Attrib<float>> attrib_transmissive = std::shared_ptr<Attrib<float>>(new AttribValue(transmissive));
    std::shared_ptr<Attrib<float>> attrib_ior = std::shared_ptr<Attrib<float>>(new AttribValue(ior));

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

} /* trc */
