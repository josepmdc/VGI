#include <GLFW/glfw3.h>

class State {
  private:
    bool m_DisableCursor = false;
    bool m_DisableCursorCallback = false;
  public:
    State();
    void Modify(int key, int action);
    bool CursorEnabled() { return m_DisableCursor; }
    bool CursorCallbackEnabled() { return m_DisableCursorCallback; }
};
