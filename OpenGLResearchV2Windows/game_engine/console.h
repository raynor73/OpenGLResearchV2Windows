#pragma once

#include <game_engine/without_generated_methods.h>
#include <functional>
#include <string>

namespace GameEngine
{
class Console : public WithoutGeneratedMethods
{
public:
    enum class Character {
        letter_a, letter_b, letter_c, letter_d, letter_e, letter_f, letter_g, letter_h, letter_i,
        letter_j, letter_k, letter_l, letter_m, letter_n, letter_o, letter_p, letter_q, letter_r,
        letter_s, letter_t, letter_u, letter_v, letter_w, letter_x, letter_y, letter_z,
        number_0, number_1, number_2, number_3, number_4, number_5, number_6, number_7, number_8, number_9,
        exclamation_mark, tilde, at, dollar, percent, caret, ampersand, asterisc, left_parenthesis, right_parenthesis,
        minus, underscore, plus, left_brace, right_brace, left_bracket, right_bracket, left_chevron,
        colon, semicolon, double_quote, quote, backslash, slash, vertical_bar, question_mark,
        number_sign, equals_sign, space, backspace, enter
    };

    void setCommandLineCallback(std::function<void(std::string)> callback) { m_commandLineCallback = callback; }
    void processCharacter(Character);

private:
    std::vector<char> m_buffer;

    std::function<void(std::string)> m_commandLineCallback = nullptr;
};
}

