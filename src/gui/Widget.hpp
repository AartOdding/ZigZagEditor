#include <glm/glm.hpp>

#include <ZigZag/ParentChildRelationship.hpp>



class Widget : public ZigZag::ZigZagChild<Widget, Widget>,
               public ZigZag::ZigZagParent<Widget, Widget>
{
public:

    Widget(Widget* parent);

    Widget* getParentWidget() const;
    void setParentWidget(Widget*) const;

    const std::vector<Widget*>& getChildWidgets() const;


    glm::vec2 mapToChildren(glm::vec2 point) const;
    glm::vec2 mapFromChildren(glm::vec2 point) const;

    glm::vec2 mapToParent(glm::vec2 point) const;
    glm::vec2 mapFromParent(glm::vec2 point) const;


    void setSize(glm::vec2 size);
    void setPosition(glm::vec2 position);

    virtual bool contains(glm::vec2 point);

protected:

    virtual void mouseFocusGained() { }
    virtual void mouseFocusLost() { }

    virtual void keyboardFocusGained() { }
    virtual void keyboardFocusLost() { }

    virtual void keyPressed() { }
    virtual void keyReleased() { }

    virtual void mousePressed() { }
    virtual void mouseReleased() { }

private:

    // Both position and size are measured in the coordinate space that the parent widget defines for its
    // children, this means that all sibling widgets live in the same coordinate space as this.
    glm::vec2 m_position;
    glm::vec2 m_size;

    double m_childrenScale = 1.0;
    glm::vec2 m_childrenOffset = glm::vec2(0, 0);

    bool m_wantsMouseFocus = true;
    bool m_wantsKeyboardFocus = false;

    static Widget* mouseFocusWidget;
    static Widget* keyboardFocusWidget;

};
