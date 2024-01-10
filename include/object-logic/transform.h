#ifndef HiGUI_TRANSFORM_H
#define HiGUI_TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace hi
{

class Transform 
{
public:
    Transform() noexcept
        : position(0.0f), rotation(glm::quat()), scale(1.0f) {}

    glm::mat4 GetTransformationMatrix() const noexcept {
        glm::mat4 pos_matrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);
        glm::mat4 rot_matrix = glm::toMat4(rotation);

        return pos_matrix * rot_matrix * scale_matrix;
    }

    // set/get position
    void setPosition(const glm::vec3& pos) noexcept { position = pos; }
    const glm::vec3& getPosition() const noexcept { return position; }

    // set/get rotation (using quaternions)
    void setRotation(const glm::quat& rot) noexcept { rotation = rot; }
    const glm::quat& getRotation() const noexcept { return rotation; }

    // set/get scale
    void setScale(const glm::vec3& scl) noexcept { scale = scl; }
    const glm::vec3& getScale() const noexcept { return scale; }

private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};


} // namespace 'hi'

#endif HiGUI_TRANSFORM_H