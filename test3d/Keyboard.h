#pragma once

#include <queue>
#include <bitset>

namespace Application {
    class Keyboard
    {
        friend class Window;
    public:
        class Event {
        public:
            enum class Type { Press, Release, Invalid };
        private:
            Type type;
            unsigned char code;
        public:
            inline Event() noexcept : type(Type::Invalid), code(0u) {}
            inline Event(Type type, unsigned char code) noexcept : type(type), code(code) {}
            inline bool IsPress() const noexcept { return type == Type::Press; }
            inline bool IsRelease() const noexcept { return type == Type::Release; }
            inline bool IsValid() const noexcept { return type != Type::Invalid; }
            inline unsigned char GetCode() const noexcept { return code; }
        };
    public:
        Keyboard() = default;
        Keyboard(const Keyboard&) = delete;
        Keyboard& operator=(const Keyboard&) = delete;
        bool KeyIsPressed(unsigned char keycode) const noexcept;
        Event ReadKey() noexcept;
        bool KeyIsEmpty() const noexcept;
        void FlushKey() noexcept;

        char ReadChar() noexcept;
        bool CharIsEmpty() const noexcept;
        void FlushChar() noexcept;

        void Flush() noexcept;

        void EnableAutorepeat() noexcept;
        void DisableAutorepeat() noexcept;
        bool AutorepeatIsEnabled() const noexcept;
    private:
        void OnKeyPressed(unsigned char keycode) noexcept;
        void OnKeyReleased(unsigned char keycode) noexcept;
        void OnChar(char character) noexcept;
        void ClearState() noexcept;

        template<typename T>
        static void TrimBuffer(std::queue<T>& buffer) noexcept;
    private:
        static constexpr unsigned int nKeys = 256u;
        static constexpr unsigned int bufferSize = 16u;
        bool autorepeatEnabled = false;
        std::bitset<nKeys> keyStates;
        std::queue<Event> keyBuffer;
        std::queue<char> charBuffer;
    };

    template<typename T>
    inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
    {
        while (buffer.size() > bufferSize)
        {
            buffer.pop();
        }
    }
}