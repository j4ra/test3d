#pragma once

#include <queue>
#include <bitset>
#include <utility>

class Mouse
{
    friend class Window;
public:
    class Event {
    public:
        enum class Type { LPress, LRelease, RPress, RRelease, MPress, MRelease, WheelUp, WheelDown, Move, Invalid };
    private:
        Type type;
        bool leftIsPressed;
        bool rightIsPressed;
        bool middleIsPressed;
        int x, y;
    public:
        inline Event() noexcept 
            : type(Type::Invalid), leftIsPressed(false), rightIsPressed(false), 
            middleIsPressed(false), x(0), y(0){}
        inline Event(Type type, const Mouse& parent) noexcept 
            : type(type), leftIsPressed(parent.leftIsPressed), 
            rightIsPressed(parent.rightIsPressed), middleIsPressed(parent.middleIsPressed), 
            x(parent.x), y(parent.y) {}
        inline Type GetType() const noexcept { return type; }
        inline bool IsValid() const noexcept { return type != Type::Invalid; }
        inline std::pair<int, int> GetPos() const noexcept { return { x,y }; }
        inline int GetPosX() const noexcept { return x; }
        inline int GetPosY() const noexcept { return y; }
        inline bool LeftIsPressed() const noexcept { return leftIsPressed; }
        inline bool RightIsPressed() const noexcept { return rightIsPressed; }
        inline bool MiddleIsPressed() const noexcept { return middleIsPressed; }
    };
public:
    Mouse() = default;
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    std::pair<int, int> GetPos() const noexcept;
    int GetPosX() const noexcept;
    int GetPosY() const noexcept;
    bool RightIsPressed() const noexcept;
    bool LeftIsPressed() const noexcept;
    bool MiddleIsPressed() const noexcept;

    Event Read() noexcept;
    inline bool IsEmpty() const noexcept { return buffer.empty(); }
    void Flush() noexcept;
private:
    void OnMouseMove(int newx, int newy) noexcept;

    void OnLeftPressed(int x, int y) noexcept;
    void OnRightPressed(int x, int y) noexcept;
    void OnMiddlePressed(int x, int y) noexcept;

    void OnLeftReleased(int x, int y) noexcept;
    void OnRightReleased(int x, int y) noexcept;
    void OnMiddleReleased(int x, int y) noexcept;

    void OnWheelUp(int x, int y) noexcept;
    void OnWheelDown(int x, int y) noexcept;

    void TrimBuffer() noexcept;
private:
    static constexpr unsigned int bufferSize = 16u;
    int x;
    int y;
    bool leftIsPressed;
    bool rightIsPressed;
    bool middleIsPressed;
    std::queue<Event> buffer;
};