#include "console.h"

using namespace std;
using namespace GameEngine;

void Console::processCharacter(Character character)
{
    char characterCode = 0;

    switch (character)
    {
    case Character::letter_a:
        characterCode = 'a';
        break;

    case Character::backspace:
        if (!m_buffer.empty()) {
            m_buffer.pop_back();
        }
        break;

    case Character::enter:
        if (m_commandLineCallback != nullptr) {
            m_commandLineCallback(string(m_buffer.data(), 0, m_buffer.size()));
            m_buffer.clear();
        }
        break;

    default:
        break;
    }

    if (characterCode != 0) {
        m_buffer.push_back(characterCode);
    }
}
