
uniform vec4 colorRed;
uniform vec4 colorGreen;
uniform float unknownInput;
vec4 main() {
    float _0_unknown = unknownInput;
    bool _1_ok = true;
    float _2_x = 34.0;
    _1_ok = _1_ok && _2_x == 34.0;
    _2_x = 30.0;
    _1_ok = _1_ok && _2_x == 30.0;
    _2_x = 64.0;
    _1_ok = _1_ok && _2_x == 64.0;
    _2_x = 16.0;
    _1_ok = _1_ok && _2_x == 16.0;
    _2_x = 19.0;
    _1_ok = _1_ok && _2_x == 19.0;
    _2_x = 1.0;
    _1_ok = _1_ok && _2_x == 1.0;
    _2_x = -2.0;
    _1_ok = _1_ok && _2_x == -2.0;
    _2_x = 3.0;
    _1_ok = _1_ok && _2_x == 3.0;
    _2_x = -4.0;
    _1_ok = _1_ok && _2_x == -4.0;
    _2_x = 5.0;
    _1_ok = _1_ok && _2_x == 5.0;
    _2_x = -6.0;
    _1_ok = _1_ok && _2_x == -6.0;
    _2_x = 7.0;
    _1_ok = _1_ok && _2_x == 7.0;
    _2_x = -8.0;
    _1_ok = _1_ok && _2_x == -8.0;
    _2_x = 9.0;
    _1_ok = _1_ok && _2_x == 9.0;
    _2_x = -10.0;
    _1_ok = _1_ok && _2_x == -10.0;
    _2_x = 11.0;
    _1_ok = _1_ok && _2_x == 11.0;
    _2_x = -12.0;
    _1_ok = _1_ok && _2_x == -12.0;
    _2_x = _0_unknown;
    _1_ok = _1_ok && _2_x == _0_unknown;
    _2_x = _0_unknown;
    _1_ok = _1_ok && _2_x == _0_unknown;
    _2_x = _0_unknown;
    _1_ok = _1_ok && _2_x == _0_unknown;
    _2_x = 0.0;
    _1_ok = _1_ok && _2_x == 0.0;
    _2_x = _0_unknown;
    _1_ok = _1_ok && _2_x == _0_unknown;
    _2_x = _0_unknown;
    _1_ok = _1_ok && _2_x == _0_unknown;
    _2_x = 0.0;
    _1_ok = _1_ok && _2_x == 0.0;
    _2_x = _0_unknown;
    _1_ok = _1_ok && _2_x == _0_unknown;
    _2_x = 0.0 / _0_unknown;
    _1_ok = _1_ok && _2_x == 0.0;
    _2_x += 1.0;
    _1_ok = _1_ok && _2_x == 1.0;
    _1_ok = _1_ok && _2_x == 1.0;
    _2_x -= 2.0;
    _1_ok = _1_ok && _2_x == -1.0;
    _1_ok = _1_ok && _2_x == -1.0;
    _1_ok = _1_ok && _2_x == -1.0;
    _2_x *= 2.0;
    _1_ok = _1_ok && _2_x == -2.0;
    _1_ok = _1_ok && _2_x == -2.0;
    _2_x /= 2.0;
    _1_ok = _1_ok && _2_x == -1.0;
    return _1_ok ? colorGreen : colorRed;
}
