#include <zeno2/UI/Font.h>
#include <incbin.h>


INCBIN_EXTERN(zeno2_assets_regular_ttf);


namespace zeno2::UI {


Font::Font(const uint8_t *data, size_t size) {
    font = std::make_unique<FTTextureFont>((const unsigned char *)data, size);
    if (font->Error()) {
        throw ztd::error("Failed to load FTGL font!");
        abort();
    }
    font->CharMap(ft_encoding_unicode);

    layout = std::make_unique<FTSimpleLayout>();
    layout->SetFont(font.get());
}

Font &Font::set_font_size(float font_size) {
    font->FaceSize(font_size);
    return *this;
}

Font &Font::set_fixed_width(float width, FTGL::TextAlignment align) {
    layout->SetLineLength(width);
    layout->SetAlignment(align);
    return *this;
}

Font &Font::set_fixed_height(float height) {
    fixed_height = height;
    return *this;
}

AABB Font::calc_bounding_box(std::string const &str) {
    auto bbox = layout->BBox(str.data(), str.size());
    return AABB(bbox.Lower().X(), bbox.Lower().Y(),
                bbox.Upper().X() - bbox.Lower().X(),
                bbox.Upper().Y() - bbox.Lower().Y());
}

Font &Font::render(float x, float y, std::string const &str) {
    if (fixed_height > 0) {
        auto bbox = calc_bounding_box(str);
        y += fixed_height / 2 - bbox.ny / 2;
    }
    if (str.size()) {
        glPushMatrix();
        glTranslatef(x, y, 0.f);
        layout->Render(str.data(), str.size());
        glPopMatrix();
    }
    return *this;
}


Font get_default_font() {
    return Font(gzeno2_assets_regular_ttfData, gzeno2_assets_regular_ttfSize);
}


}
