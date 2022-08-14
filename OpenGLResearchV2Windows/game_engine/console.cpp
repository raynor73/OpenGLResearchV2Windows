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

    case Character::letter_e:
        characterCode = 'e';
        break;

    case Character::letter_i:
        characterCode = 'i';
        break;

    case Character::letter_l:
        characterCode = 'l';
        break;

    case Character::letter_n:
        characterCode = 'n';
        break;

    case Character::letter_p:
        characterCode = 'p';
        break;

    case Character::letter_q:
        characterCode = 'q';
        break;

    case Character::letter_r:
        characterCode = 'r';
        break;

    case Character::letter_s:
        characterCode = 's';
        break;

    case Character::letter_t:
        characterCode = 't';
        break;

    case Character::letter_u:
        characterCode = 'u';
        break;

    case Character::letter_x:
        characterCode = 'x';
        break;

    case Character::period:
        characterCode = '.';
        break;

    case Character::left_parenthesis:
        characterCode = '(';
        break;

    case Character::right_parenthesis:
        characterCode = ')';
        break;

    case Character::semicolon:
        characterCode = ';';
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
