#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <list>

#include "../../lib/glm.hpp"
#include "../engine/scene.hpp"
#include "../engine/importer.hpp"
#include "../graphics/color_spaces.hpp"

namespace trc {

#ifdef TRC_DEFINE_PARSER
    /* advance iterator by one if condition is met, report unexpected_token error otherwise */
    #define ADVANCE_IF(iterator, condition) if (!(condition)) { \
        error_string = unexpected_token_str(*iterator); return false; } \
        ++iterator;
    /* report unexpected_eof error if iterator is at end */ \
    #define CHECK_I(iterator) if (iterator == tokens.end()) { \
        error_string = unexpected_eof_str(); return false; }
#endif

namespace parser {
    enum TokenType {
        WORD, BRACE_OPEN, BRACE_CLOSE, VALUE, COMMA, STRING
    };

    struct Token {
        Token(std::string p_content, TokenType p_type, int p_line, int p_column);

        std::string content;
        TokenType type;
        int line, column;
    };
} /* parser */

class TraceyParser {
public:
    bool load_file(std::string file_path, ShaderPack *shader_pack);

    Scene get_loaded_scene();
    std::string get_error_string();

private:
    Scene loaded_scene;
    std::string error_string = "Success";

    std::list<parser::Token> tokens;

    std::string read_file_content(const char *path);

    void tokenize(std::string buffer);

    bool parse(Scene &scene, ShaderPack *shader_pack, std::string path);
    bool parse_object(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, Material *material, std::string path);
    bool parse_floor(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, Material *materia, std::string pathl);
    bool parse_sphere(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, Material *material, std::string path);
    bool parse_point_light(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack);
    bool parse_sun_light(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack);
    bool parse_external(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, std::string path);
    bool parse_environment(std::list<parser::Token>::iterator &i, Scene &scene, ShaderPack *shader_pack, std::string path);
    bool parse_material(std::list<parser::Token>::iterator &i, Material *&mat, Scene &scene, std::string path);

    bool parse_string(std::list<parser::Token>::iterator &i, std::string &str);
    bool parse_float(std::list<parser::Token>::iterator &i, float &val);
    bool parse_float_tex(std::list<parser::Token>::iterator &i, float &val, AttribTexture<float> &tex, std::string path);
    bool parse_vec3(std::list<parser::Token>::iterator &i, glm::vec3 &val);
    bool parse_vec3_tex(std::list<parser::Token>::iterator &i, glm::vec3 &val, AttribTexture<glm::vec3> &tex, std::string path);

#   ifdef TRC_DEFINE_PARSER
    template <typename T>
    bool parse_value(std::list<parser::Token>::iterator &i, std::vector<float> &vec, AttribTexture<T> &texture, int desired_values, bool expect_texture, std::string path) {
        using namespace parser;

        ADVANCE_IF(i, i->type == BRACE_OPEN); CHECK_I(i);

        // load texture?
        Token token = *i;
        if (expect_texture && token.type == STRING) {
            ADVANCE_IF(i, i->type == STRING); CHECK_I(i);
            std::string relative_path = token.content.substr(1, token.content.length()-2);
            path.append(relative_path);

            if (image_rw::read_texture(path.c_str(), texture)) {
                error_string = std::string {"Unable to open file \""}.append(path).append("\"");
                return false;
            }

            ADVANCE_IF(i, i->type == BRACE_CLOSE);

            return true;
        }

        // parse value normally otherwise

        while (i != tokens.end()) {
            token = *i;
            ADVANCE_IF(i, (i->type == VALUE)); CHECK_I(i);

            vec.push_back(std::atof(token.content.c_str()));

            token = *i;
            if (token.type == BRACE_CLOSE && vec.size() != desired_values) {
                error_string = std::to_string(token.line).append(":").append(std::to_string(token.column))
                    .append(" : ").append(std::to_string(vec.size()))
                    .append(" value(s) provided where ")
                    .append(std::to_string(desired_values))
                    .append(" expected");
                return false;
            }

            token = *i;
            ADVANCE_IF(i, (i->type == COMMA || i->type == BRACE_CLOSE)); CHECK_I(i);

            if (token.type == BRACE_CLOSE) break;
        }

        return true;
    }
#   endif

    bool load_non_native_file(std::string file_path, Scene &scene, ShaderPack *shader_pack, glm::mat4 transform);

    std::string unexpected_token_str(parser::Token token);
    std::string unexpected_eof_str();
    std::string unknown_keyword_str(parser::Token token, std::string context);
};

} /* trc */

#endif /* end of include guard: PARSER_HPP */
