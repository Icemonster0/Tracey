#define TRC_DEFINE_PARSER
#include "parser.hpp"

#include <fstream>
#include <sstream>

#include "math_util.hpp"

namespace trc {

// public

parser::Token::Token(std::string p_content, TokenType p_type, int p_line, int p_column)
    : content(p_content), type(p_type), line(p_line), column(p_column) {}

bool TraceyParser::load_file(std::string file_path, ShaderPack *shader_pack) {
    std::string buffer = read_file_content(file_path.c_str());
    if (buffer == "TRC_ERROR") {
        return false;
    }

    tokenize(buffer);

    std::string path = file_path.substr(0, file_path.find_last_of("\\/")+1);
    if(!parse(loaded_scene, shader_pack, path)) return false;

    return true;
}

Scene TraceyParser::get_loaded_scene() {
    return std::move(loaded_scene);
}

std::string TraceyParser::get_error_string() {
    return error_string;
}

// private

std::string TraceyParser::read_file_content(const char *path) {
    std::ifstream file {path};
    if (!file.is_open()) {
        error_string = std::string {"Unable to open file \""}.append(path).append("\"");
        return "TRC_ERROR";
    }
    std::stringstream s;
    s << file.rdbuf();
    return s.str();
}

void TraceyParser::tokenize(std::string buffer) {
    using namespace parser;

#   define PUSH_TOKEN(content, type) tokens.emplace_back(std::string {content}, type, line, col);
#   define IS_NUMBER(c) (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '.' || c == '+' || c == '-')

    tokens = std::list<Token>();
    int line = 1, col = 1;
    int i = 0;
    while (i < buffer.length()) {
        char c = buffer.at(i);

        // line break
        if (c == '\n') {
            ++i; col = 1; ++line;
        }
        // tab (assuming it is four spaces in width)
        else if (c == '\t') {
            ++i; col += 4;
        }
        // space
        else if (c == ' ') {
            ++i; ++col;
        }
        // opening brace
        else if (c == '{') {
             ++i; ++col; PUSH_TOKEN("{", BRACE_OPEN);
        }
        // closing brace
        else if (c == '}') {
             ++i; ++col; PUSH_TOKEN("}", BRACE_CLOSE);
        }
        // value
        else if (IS_NUMBER(c)) {
            std::string num = "";
            while (i < buffer.length()) {
                c = buffer.at(i);
                if (!IS_NUMBER(c)) break;
                num.push_back(c);
                ++i; ++col;
            }
            PUSH_TOKEN(num, VALUE);
        }
        // line comment
        else if (c == '/' && i < buffer.length()-1 && buffer.at(i+1) == '/') {
            while (i < buffer.length()) {
                c = buffer.at(i);
                if (c == '\n') break;
                ++i; ++col;
            }
        }
        // block comment
        else if (c == '/' && i < buffer.length()-2 && buffer.at(i+1) == '*') {
            ++i;
            while (i < buffer.length()) {
                c = buffer.at(i);
                if (c == '/' && buffer.at(i-1) == '*') break;
                if (c == '\n') {
                    col = 1; ++line;
                } else {
                    ++col;
                }
                ++i;
            }
            ++i; ++col;
        }
        // comma
        else if (c == ',') {
             ++i; ++col; PUSH_TOKEN(",", COMMA);
        }
        // string
        else if (c == '\"' && i < buffer.length()-1) {
            std::string str = "\"";
            ++i;
            while (i < buffer.length()) {
                c = buffer.at(i);
                if (c == '\"') break;
                if (c == '\n') {
                    col = 1; ++line;
                } else {
                    str.push_back(c);
                    ++col;
                }
                ++i;
            }
            str.push_back('\"');
            ++i; ++col;
            PUSH_TOKEN(str, STRING);
        }
        // word
        else {
            std::string word = "";
            while (i < buffer.length()) {
                c = buffer.at(i);
                if (c == '\n' || c == '\t' || c == ' ' || c == '{' || c == '}' || c == '.' || c == '/' || c == ',' || c == '\"') break;
                word.push_back(c);
                ++i; ++col;
            }
            PUSH_TOKEN(word, WORD);
        }
    }

#   undef PUSH_TOKEN
#   undef IS_NUMBER

}

bool TraceyParser::parse(Scene &scene, ShaderPack *shader_pack, std::string path) {
    using namespace parser;

    Material *material = scene.add_material(std::make_unique<Material>(glm::vec3 {1.f}));

    auto i = tokens.begin();
    while (i != tokens.end()) {
        if (i->type == WORD) {
            if (!parse_object(i, scene, shader_pack, material, path)) return false;
        }
        else {
            error_string = unexpected_token_str(*i);
            return false;
        }
    }

    return true;
}

/* advance iterator by one if condition is met, report unexpected_token error otherwise */
#define ADVANCE_IF(iterator, condition) if (!(condition)) { \
    error_string = unexpected_token_str(*iterator); return false; } \
    ++iterator;
/* report unexpected_eof error if iterator is at end */ \
#define CHECK_I(iterator) if (iterator == tokens.end()) { \
    error_string = unexpected_eof_str(); return false; }

bool TraceyParser::parse_object(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, Material *material, std::string path) {
    using namespace parser;

    while (i != tokens.end()) {
        Token token = *i;

        ADVANCE_IF(i, i->type == WORD); CHECK_I(i);

        if (token.content == "FLOOR") {
            if (!parse_floor(i, scene, shader_pack, material, path)) return false;
        } else if (token.content == "SPHERE") {
            if (!parse_sphere(i, scene, shader_pack, material, path)) return false;
        } else if (token.content == "POINT_LIGHT") {
            if (!parse_point_light(i, scene, shader_pack)) return false;
        } else if (token.content == "SUN_LIGHT") {
            if (!parse_sun_light(i, scene, shader_pack)) return false;
        } else if (token.content == "EXTERNAL") {
            if (!parse_external(i, scene, shader_pack, path)) return false;
        } else if (token.content == "ENVIRONMENT") {
            if (!parse_environment(i, scene, shader_pack, path)) return false;
        } else {
            error_string = unknown_keyword_str(token, "global scope");
            return false;
        }
    }

    return true;
}

bool TraceyParser::parse_floor(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, Material *material, std::string path) {
    using namespace parser;

    ADVANCE_IF(i, i->type == BRACE_OPEN); CHECK_I(i);

    float height = 0.f;

    while (i != tokens.end()) {
        Token token = *i;
        ADVANCE_IF(i, (i->type == WORD || i->type == BRACE_CLOSE));

        if (token.type == BRACE_CLOSE) break;

        CHECK_I(i);

        if (token.content == "HEIGHT") {
            if(!parse_float(i, height)) return false;
        }
        else if (token.content == "MATERIAL") {
            if(!parse_material(i, material, scene, path)) return false;
        }
        else {
            error_string = unknown_keyword_str(token, "FLOOR");
            return false;
        }
    }

    scene.add_object(std::unique_ptr<Shape>(new GroundPlane(height, shader_pack->shader_combined.get(), material)));

    return true;
}

bool TraceyParser::parse_sphere(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, Material *material, std::string path) {
    using namespace parser;

    ADVANCE_IF(i, i->type == BRACE_OPEN); CHECK_I(i);

    glm::vec3 position {0.f};
    float radius = 1.f;

    while (i != tokens.end()) {
        Token token = *i;
        ADVANCE_IF(i, (i->type == WORD || i->type == BRACE_CLOSE));

        if (token.type == BRACE_CLOSE) break;

        CHECK_I(i);

        if (token.content == "POSITION") {
            if(!parse_vec3(i, position)) return false;
        }
        else if (token.content == "RADIUS") {
            if(!parse_float(i, radius)) return false;
        }
        else if (token.content == "MATERIAL") {
            if(!parse_material(i, material, scene, path)) return false;
        }
        else {
            error_string = unknown_keyword_str(token, "SPHERE");
            return false;
        }
    }

    scene.add_object(std::unique_ptr<Shape>(new Sphere(position, radius, shader_pack->shader_combined.get(), material)));

    return true;
}

bool TraceyParser::parse_point_light(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack) {
    using namespace parser;

    ADVANCE_IF(i, i->type == BRACE_OPEN); CHECK_I(i);

    glm::vec3 position {0.f};
    float radius = 0.1f;
    glm::vec3 color {1.f};
    float intensity = 10.f;
    Material *mat;
    bool default_mat = true;

    while (i != tokens.end()) {
        Token token = *i;
        ADVANCE_IF(i, (i->type == WORD || i->type == BRACE_CLOSE));

        if (token.type == BRACE_CLOSE) break;

        CHECK_I(i);

        if (token.content == "POSITION") {
            if(!parse_vec3(i, position)) return false;
        }
        else if (token.content == "RADIUS") {
            if(!parse_float(i, radius)) return false;
        }
        else if (token.content == "COLOR") {
            if(!parse_vec3(i, color)) return false;
        }
        else if (token.content == "INTENSITY") {
            if(!parse_float(i, intensity)) return false;
        }
        else {
            error_string = unknown_keyword_str(token, "POINT_LIGHT");
            return false;
        }
    }

    scene.add_light(std::unique_ptr<Light>(new PointLight(position, color, intensity, radius)));

    return true;
}

bool TraceyParser::parse_sun_light(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack) {
    using namespace parser;

    ADVANCE_IF(i, i->type == BRACE_OPEN); CHECK_I(i);

    glm::vec3 direction {-1.f};
    float angle = 0.526f;
    glm::vec3 color {1.f};
    float intensity = 3.f;
    Material *mat;
    bool default_mat = true;

    while (i != tokens.end()) {
        Token token = *i;
        ADVANCE_IF(i, (i->type == WORD || i->type == BRACE_CLOSE));

        if (token.type == BRACE_CLOSE) break;

        CHECK_I(i);

        if (token.content == "DIRECTION") {
            if(!parse_vec3(i, direction)) return false;
        }
        else if (token.content == "ANGLE") {
            if(!parse_float(i, angle)) return false;
        }
        else if (token.content == "COLOR") {
            if(!parse_vec3(i, color)) return false;
        }
        else if (token.content == "INTENSITY") {
            if(!parse_float(i, intensity)) return false;
        }
        else {
            error_string = unknown_keyword_str(token, "SUN_LIGHT");
            return false;
        }
    }

    scene.add_light(std::unique_ptr<Light>(new SunLight(math::normalize(direction), color, intensity, angle)));

    return true;
}

bool TraceyParser::parse_external(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, std::string path) {
    using namespace parser;

    ADVANCE_IF(i, i->type == BRACE_OPEN); CHECK_I(i);

    std::string source = "";
    glm::vec3 position {0.f};
    glm::vec3 rotation {0.f};
    float scale = 1.f;

    Token token = *i;
    while (i != tokens.end()) {
        token = *i;
        ADVANCE_IF(i, (i->type == WORD || i->type == BRACE_CLOSE));

        if (token.type == BRACE_CLOSE) break;

        CHECK_I(i);

        if (token.content == "SOURCE") {
            if(!parse_string(i, source)) return false;
            path.append(source);
        }
        else if (token.content == "POSITION") {
            if(!parse_vec3(i, position)) return false;
        }
        else if (token.content == "ROTATION") {
            if(!parse_vec3(i, rotation)) return false;
        }
        else if (token.content == "SCALE") {
            if(!parse_float(i, scale)) return false;
        }
        else {
            error_string = unknown_keyword_str(token, "EXTERNAL");
            return false;
        }
    }

    if (source.length() == 0) {
        error_string = std::to_string(token.line)
            .append(":")
            .append(std::to_string(token.column))
            .append(" : SOURCE field must be specified for EXTERNAL object");
        return false;
    }

    glm::mat4 transform {1.f};
    transform = glm::translate(transform, position);
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));

    Scene new_scene;
    if (!load_non_native_file(path, new_scene, shader_pack, transform)) return false;

    scene.append(new_scene);

    return true;
}

bool TraceyParser::parse_environment(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, std::string path) {
    using namespace parser;

    ADVANCE_IF(i, i->type == BRACE_OPEN); CHECK_I(i);

    std::string source = "";
    glm::vec3 color {0.05f};
    float rotation = 0.f;

    Token token = *i;
    while (i != tokens.end()) {
        token = *i;
        ADVANCE_IF(i, (i->type == WORD || i->type == BRACE_CLOSE));

        if (token.type == BRACE_CLOSE) break;

        CHECK_I(i);

        if (token.content == "SOURCE") {
            if(!parse_string(i, source)) return false;
            path.append(source);
        }
        else if (token.content == "COLOR") {
            if(!parse_vec3(i, color)) return false;
        }
        else if (token.content == "ROTATION") {
            if(!parse_float(i, rotation)) return false;
        }
        else {
            error_string = unknown_keyword_str(token, "ENVIRONMENT");
            return false;
        }
    }

    EnvironmentTexture *env_tex = scene.set_environment(EnvironmentTexture {});

    if (source.length() == 0) {
        env_tex->set_color(color);
    } else {
        env_tex->load(path.c_str());
    }
    env_tex->set_rotation(rotation);

    return true;
}

bool TraceyParser::parse_material(std::list<parser::Token>::iterator &i, Material *&mat, Scene &scene, std::string path) {
    using namespace parser;

    ADVANCE_IF(i, i->type == BRACE_OPEN); CHECK_I(i);

    glm::vec3 albedo {1.f};
    float roughness = 0.5f;
    float metallic = 0.f;
    glm::vec3 emission {0.f};
    glm::vec3 normal {0.5f, 0.5f, 1.f};
    float transmissive = 0.f;
    float ior = 1.6f;

    std::shared_ptr<Attrib<glm::vec3>> albedo_attrib = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(albedo));
    std::shared_ptr<Attrib<float>> roughness_attrib = std::shared_ptr<Attrib<float>>(new AttribValue(roughness));
    std::shared_ptr<Attrib<float>> metallic_attrib = std::shared_ptr<Attrib<float>>(new AttribValue(metallic));
    std::shared_ptr<Attrib<glm::vec3>> emission_attrib = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(emission));
    std::shared_ptr<Attrib<glm::vec3>> normal_attrib = std::shared_ptr<Attrib<glm::vec3>>(new AttribValue(normal));
    std::shared_ptr<Attrib<float>> transmissive_attrib = std::shared_ptr<Attrib<float>>(new AttribValue(transmissive));
    std::shared_ptr<Attrib<float>> ior_attrib = std::shared_ptr<Attrib<float>>(new AttribValue(ior));

    AttribTexture<glm::vec3> *albedo_tex = new AttribTexture<glm::vec3>();
    AttribTexture<float> *roughness_tex = new AttribTexture<float>();
    AttribTexture<float> *metallic_tex = new AttribTexture<float>();
    AttribTexture<glm::vec3> *emission_tex = new AttribTexture<glm::vec3>();
    AttribTexture<glm::vec3> *normal_tex = new AttribTexture<glm::vec3>();
    AttribTexture<float> *transmissive_tex = new AttribTexture<float>();
    AttribTexture<float> *ior_tex = new AttribTexture<float>();

    while (i != tokens.end()) {
        Token token = *i;
        ADVANCE_IF(i, (i->type == WORD || i->type == BRACE_CLOSE));

        if (token.type == BRACE_CLOSE) break;

        CHECK_I(i);

        // printf(#param" "#type" "#full_type"\n");
#       define PARSE_PARAMETER(param, type, full_type) \
            if (!parse_##type##_tex(i, param, *param##_tex, path)) return false; \
            if (param##_tex->get_size() != glm::ivec2 {0, 0}) \
                param##_attrib = std::shared_ptr<Attrib<full_type>>(param##_tex); \
            else \
                param##_attrib = std::shared_ptr<Attrib<full_type>>(new AttribValue(param));

        if (token.content == "COLOR") {
            PARSE_PARAMETER(albedo, vec3, glm::vec3);
            for (int x = 0; x < albedo_tex->get_size().x; ++x) {
                for (int y = 0; y < albedo_tex->get_size().y; ++y) {
                    albedo_tex->set_pixel({x, y}, color::sRGB_to_flat(albedo_tex->get_pixel({x, y})));
                }
            }
        }
        else if (token.content == "ROUGHNESS") {
            PARSE_PARAMETER(roughness, float, float);
        }
        else if (token.content == "METALLIC") {
            PARSE_PARAMETER(metallic, float, float);
        }
        else if (token.content == "EMISSION") {
            PARSE_PARAMETER(emission, vec3, glm::vec3);
        }
        else if (token.content == "NORMAL") {
            PARSE_PARAMETER(normal, vec3, glm::vec3);
        }
        else if (token.content == "TRANSMISSION") {
            PARSE_PARAMETER(transmissive, float, float);
        }
        else if (token.content == "IOR") {
            PARSE_PARAMETER(ior, float, float);
        }
        else {
            error_string = unknown_keyword_str(token, "MATERIAL");
            return false;
        }

#       undef PARSE_PARAMETER

    }

    mat = scene.add_material(std::make_unique<Material>(
        albedo_attrib,
        roughness_attrib,
        metallic_attrib,
        emission_attrib,
        normal_attrib,
        transmissive_attrib,
        ior_attrib
    ));

    return true;
}

bool TraceyParser::parse_string(std::list<parser::Token>::iterator &i, std::string &str) {
    using namespace parser;

    ADVANCE_IF(i, i->type == BRACE_OPEN); CHECK_I(i);

    Token token = *i;
    ADVANCE_IF(i, i->type == STRING); CHECK_I(i);

    str = token.content.substr(1, token.content.length()-2);

    ADVANCE_IF(i, i->type == BRACE_CLOSE);

    return true;
}

bool TraceyParser::parse_float(std::list<parser::Token>::iterator &i, float &val) {
    std::vector<float> vec;
    AttribTexture<float> tmp;
    if(!parse_value(i, vec, tmp, 1, false, "")) return false;
    val = vec[0];
    return true;
}

bool TraceyParser::parse_float_tex(std::list<parser::Token>::iterator &i, float &val, AttribTexture<float> &tex, std::string path) {
    std::vector<float> vec;
    if(!parse_value(i, vec, tex, 1, true, path)) return false;
    if (vec.size() >= 1) val = vec[0];
    return true;
}

bool TraceyParser::parse_vec3(std::list<parser::Token>::iterator &i, glm::vec3 &val) {
    std::vector<float> vec;
    AttribTexture<glm::vec3> tmp;
    if(!parse_value(i, vec, tmp, 3, false, "")) return false;
    val = glm::vec3 {vec[0], vec[1], vec[2]};
    return true;
}

bool TraceyParser::parse_vec3_tex(std::list<parser::Token>::iterator &i, glm::vec3 &val, AttribTexture<glm::vec3> &tex, std::string path) {
    std::vector<float> vec;
    if(!parse_value(i, vec, tex, 3, true, path)) return false;
    if (vec.size() >= 3) val = glm::vec3 {vec[0], vec[1], vec[2]};
    return true;
}

#undef ADVANCE_IF
#undef CHECK_I

bool TraceyParser::load_non_native_file(std::string file_path, Scene &scene, ShaderPack *shader_pack, glm::mat4 transform) {
    printf("Loading file '%s'\n", file_path.c_str());

    Importer importer;
    if (!importer.load_file(file_path, shader_pack, transform)) {
        error_string = importer.get_error_string();
        return false;
    }
    scene = importer.get_loaded_scene();

    printf("\n");
    return true;
}

std::string TraceyParser::unexpected_token_str(parser::Token token) {
    return std::to_string(token.line)
           .append(":")
           .append(std::to_string(token.column))
           .append(" : Unexpected token '")
           .append(token.content)
           .append("'");
}

std::string TraceyParser::unexpected_eof_str() {
    return std::string {"Unexpected end of file"};
}

std::string TraceyParser::unknown_keyword_str(parser::Token token, std::string context) {
    return std::to_string(token.line)
           .append(":")
           .append(std::to_string(token.column))
           .append(" : Unknown keyword '")
           .append(token.content)
           .append("' in ")
           .append(context);
}

} /* trc */
