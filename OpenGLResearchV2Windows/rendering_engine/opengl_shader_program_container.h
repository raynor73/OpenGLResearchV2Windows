#pragma once

#include <GL/glew.h>
#include <rendering_engine/opengl_error_detector.h>
#include <memory>

namespace GameEngine
{
class OpenGLShaderProgramContainer
{
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

    GLuint m_shaderProgram;

    /*GLint m_positionAttribute;
    GLint m_normalAttribute;
    GLint m_uvAttribute;
    GLint m_jointIndicesAttribute;
    GLint m_jointWeightsAttribute;*/

    GLint m_mvpMatrixUniform;
    /*GLint m_modelMatrixUniform;
    GLint m_mvMatrixUniform;
    GLint m_textureUniform;
    GLint m_diffuseColorUniform;
    GLint m_useDiffuseColorUniform;
    GLint m_topColorUniform;
    GLint m_bottomColorUniform;
    GLint m_topPointUniform;
    GLint m_bottomPointUniform;
    GLint m_isGradientUniform;

    GLint m_ambientColorUniform;
    GLint m_directionalLightColorUniform;
    GLint m_directionalLightDirectionUniform;

    GLint m_hasSkeletalAnimationUniform;
    GLint m_jointTransformsUniform;*/

public:
    OpenGLShaderProgramContainer(std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector, GLuint shaderProgram);

    GLuint shaderProgram() const { return m_shaderProgram; }

    /*GLint positionAttribute() const { return m_positionAttribute; }
    GLint normalAttribute() const { return m_normalAttribute; }
    GLint uvAttribute() const { return m_uvAttribute; }

    GLint jointIndicesAttribute() const { return m_jointIndicesAttribute; }
    GLint jointWeightsAttribute() const { return m_jointWeightsAttribute; }*/

    GLint mvpMatrixUniform() const { return m_mvpMatrixUniform; }
    /*GLint modelMatrixUniform() const { return m_modelMatrixUniform; }
    GLint mvMatrixUniform() const { return m_mvMatrixUniform; }
    GLint textureUniform() const { return m_textureUniform; }
    GLint diffuseColorUniform() const { return m_diffuseColorUniform; }

    GLint useDiffuseColorUniform() const { return m_useDiffuseColorUniform; }

    GLint topColorUniform() const { return m_topColorUniform; }
    GLint bottomColorUniform() const { return m_bottomColorUniform; }

    GLint topPointUniform() const { return m_topPointUniform; }
    GLint bottomPointUniform() const { return m_bottomPointUniform; }
    GLint isGradientUniform() const { return m_isGradientUniform; }

    GLint ambientColorUniform() const { return m_ambientColorUniform; }
    GLint directionalLightColorUniform() const { return m_directionalLightColorUniform; }
    GLint directionalLightDirectionUniform() const { return m_directionalLightDirectionUniform; }

    GLint hasSkeletalAnimationUniform() const { return m_hasSkeletalAnimationUniform; }
    GLint jointTransformsUniform() const { return m_jointTransformsUniform; }*/
};
}
