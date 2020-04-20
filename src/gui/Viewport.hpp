#include <glm/glm.hpp>



/*
 * Parent widgets define the coordinate space that their children live in.
 * the widget itself is thus defined in the coordinate space that its parent has 
 * set for its children. The size and position, are also in this same coordinate
 * space, thus positions, and sizes can be meaningfully compared with sibling 
 * widgets.
 */

class Viewport
{
public:

    glm::vec2 mapToChildSpace(glm::vec2 point);
    glm::vec2 mapFromChildSpace(glm::vec2 point);

    glm::vec2 mapToParentSpace(glm::vec2 point);
    glm::vec2 mapFromParentSpace(glm::vec2 point);

    void setSize(glm::vec2 size);
    void setPosition(glm::vec2 position);

private:

    // Both position and size are measured in parent's coordinate space.
    glm::vec2 m_position;
    glm::vec2 m_size;

    double m_childrenScale = 1.0;
    glm::vec2 m_childrenOffset = glm::vec2(0, 0);

};

