#ifndef SHADER_SYNTAX_HPP
#define SHADER_SYNTAX_HPP

#define TRC_DECLARE_SHADER(name) \
struct name : public Shader { \
    glm::vec4 evaluate(ShaderData shader_data) const; \
};

#define TRC_DEFINE_SHADER(name) \
glm::vec4 name::evaluate(ShaderData shader_data) const

#endif /* end of include guard: SHADER_SYNTAX_HPP */
