#pragma once

enum TEXT_ALIGN { LEFT, RIGHT, CENTER, CENTER_XY };

class GUIElement {
protected:
    int m_x = 0;
    int m_y = 0;
    int m_width = 0.0f;
    int m_height = 0.0f;
public:
    inline GUIElement () {}
    inline virtual ~GUIElement () {}
    virtual bool update() = 0;
    // GETTERS
    inline const int& getX() const { return m_x; }
    inline const int& getY() const { return m_y; }
};
