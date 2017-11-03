#include "palette.h"
#include "co.h"

Palette::Palette(Chess* chess)
    : chess_(chess)
{
    assert(chess_ != nullptr);

    palette_.assign(co::row_, vector<QLabel*>(co::col_, nullptr));
}

Palette::~Palette()
{
    for (QLabel*& label: palette_)
    {
        delete label;
        label = nullptr;
    }
}
