/*
 * Copyright 2021 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "gm/gm.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkPaint.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkScalar.h"
#include "include/core/SkSize.h"
#include "include/core/SkString.h"
#include "include/core/SkTypeface.h"
#include "tools/Resources.h"
#include "tools/ToolUtils.h"

#include <string.h>
#include <initializer_list>

namespace skiagm {

namespace {
bool ColrV1VariationsEnabledForTest() { return true; }

const SkScalar kTextSizes[] = {12, 18, 30, 120};
const char testFontName[] = "fonts/test_glyphs-glyf_colr_1.ttf";
const SkScalar xWidth = 1200;
const SkScalar xTranslate = 200;
}

class ColrV1GM : public GM {
public:
    ColrV1GM(const char* testName,
             SkSpan<const uint32_t> codepoints,
             SkScalar skewX = 0.f,
             SkScalar rotateDeg = 0.f)
            : fTestName(testName)
            , fCodepoints(codepoints)
            , fSkewX(skewX)
            , fRotateDeg(rotateDeg) {
        fPreviousFlagFunc =
                SkGraphics::SetVariableColrV1EnabledFunc(ColrV1VariationsEnabledForTest);
    }

    ~ColrV1GM() override { SkGraphics::SetVariableColrV1EnabledFunc(fPreviousFlagFunc); }

protected:
    void onOnceBeforeDraw() override {
        fTypeface = MakeResourceAsTypeface(testFontName);
        fVariationSliders = ToolUtils::VariationSliders(fTypeface.get());
    }

    SkString onShortName() override {
        SkASSERT(!fTestName.isEmpty());
        SkString gm_name = SkStringPrintf("colrv1_%s", fTestName.c_str());

        if (fSkewX) {
            gm_name.append(SkStringPrintf("_skew_%.2f", fSkewX));
        }

        if (fRotateDeg) {
            gm_name.append(SkStringPrintf("_rotate_%.2f", fRotateDeg));
        }

        return gm_name;
    }

    bool onGetControls(SkMetaData* controls) override {
        return fVariationSliders.writeControls(controls);
    }

    void onSetControls(const SkMetaData& controls) override {
        return fVariationSliders.readControls(controls);
    }

    SkISize onISize() override {
        return SkISize::Make(xWidth, xWidth);
    }

    sk_sp<SkTypeface> makeVariedTypeface() {
        if (!fTypeface) {
            return nullptr;
        }
        SkSpan<const SkFontArguments::VariationPosition::Coordinate> coords =
                fVariationSliders.getCoordinates();
        SkFontArguments::VariationPosition varPos = {coords.data(),
                                                     static_cast<int>(coords.size())};
        SkFontArguments args;
        args.setVariationDesignPosition(varPos);
        return fTypeface->makeClone(args);
    }

    DrawResult onDraw(SkCanvas* canvas, SkString* errorMsg) override {
        canvas->drawColor(SK_ColorWHITE);
        SkPaint paint;

        canvas->translate(xTranslate, 20);

        if (!fTypeface) {
          *errorMsg = "Did not recognize COLR v1 font format.";
          return DrawResult::kSkip;
        }

        canvas->rotate(fRotateDeg);
        canvas->skew(fSkewX, 0);

        SkFont font(makeVariedTypeface());

        SkFontMetrics metrics;
        SkScalar y = 0;
        std::vector<SkColor> paint_colors = {
                SK_ColorBLACK, SK_ColorGREEN, SK_ColorRED, SK_ColorBLUE};
        auto paint_color_iterator = paint_colors.begin();
        for (SkScalar textSize : kTextSizes) {
            font.setSize(textSize);
            font.getMetrics(&metrics);
            SkScalar y_shift = -(metrics.fAscent + metrics.fDescent + metrics.fLeading) * 1.2;
            y += y_shift;
            paint.setColor(*paint_color_iterator);
            int x = 0;
            // Perform simple line breaking to fit more glyphs into the GM canvas.
            for (size_t i = 0; i < fCodepoints.size(); ++i) {
                canvas->drawSimpleText(&fCodepoints[i],
                                       sizeof(uint32_t),
                                       SkTextEncoding::kUTF32,
                                       x,
                                       y,
                                       font,
                                       paint);
                SkScalar glyphAdvance = font.measureText(
                        &fCodepoints[i], sizeof(uint32_t), SkTextEncoding::kUTF32, nullptr);
                if (x + glyphAdvance < xWidth - xTranslate) {
                    x += glyphAdvance + glyphAdvance * 0.05f;
                } else {
                    y += y_shift;
                    x = 0;
                }
            }
            paint_color_iterator++;
        }
        return DrawResult::kOk;
    }

private:
    using INHERITED = GM;

    SkString fTestName;
    sk_sp<SkTypeface> fTypeface;
    SkSpan<const uint32_t> fCodepoints;
    SkScalar fSkewX;
    SkScalar fRotateDeg;
    ToolUtils::VariationSliders fVariationSliders;
    SkGraphics::VariableColrV1EnabledFunc fPreviousFlagFunc;
};

// Generated using test glyphs generator script from https://github.com/googlefonts/color-fonts:
// $ python3 config/test_glyphs-glyf_colr_1.py -vvv  --generate-descriptions fonts/
// Regenerate descriptions and paste the generated arrays here when updating the test font.
namespace ColrV1TestDefinitions {
const uint32_t gradient_stops_repeat[] = {0xf0100, 0xf0101, 0xf0102, 0xf0103};
const uint32_t sweep_varsweep[] = {0xf0200, 0xf0201, 0xf0202, 0xf0203, 0xf0204, 0xf0205,
                                   0xf0206, 0xf0207, 0xf0208, 0xf0209, 0xf020a, 0xf020b,
                                   0xf020c, 0xf020d, 0xf020e, 0xf020f, 0xf0210, 0xf0211,
                                   0xf0212, 0xf0213, 0xf0214, 0xf0215, 0xf0216, 0xf0217};
const uint32_t paint_scale[] = {0xf0300, 0xf0301, 0xf0302, 0xf0303, 0xf0304, 0xf0305};
const uint32_t extend_mode[] = {0xf0500, 0xf0501, 0xf0502, 0xf0503, 0xf0504, 0xf0505};
const uint32_t paint_rotate[] = {0xf0600, 0xf0601, 0xf0602, 0xf0603};
const uint32_t paint_skew[] = {0xf0700, 0xf0701, 0xf0702, 0xf0703, 0xf0704, 0xf0705};
const uint32_t paint_transform[] = {0xf0800, 0xf0801, 0xf0802, 0xf0803};
const uint32_t paint_translate[] = {0xf0900, 0xf0901, 0xf0902, 0xf0903, 0xf0904, 0xf0905, 0xf0906};
const uint32_t composite_mode[] = {0xf0a00, 0xf0a01, 0xf0a02, 0xf0a03, 0xf0a04, 0xf0a05, 0xf0a06,
                                   0xf0a07, 0xf0a08, 0xf0a09, 0xf0a0a, 0xf0a0b, 0xf0a0c, 0xf0a0d,
                                   0xf0a0e, 0xf0a0f, 0xf0a10, 0xf0a11, 0xf0a12, 0xf0a13, 0xf0a14,
                                   0xf0a15, 0xf0a16, 0xf0a17, 0xf0a18, 0xf0a19, 0xf0a1a, 0xf0a1b};
const uint32_t foreground_color[] = {
        0xf0b00, 0xf0b01, 0xf0b02, 0xf0b03, 0xf0b04, 0xf0b05, 0xf0b06, 0xf0b07};
const uint32_t clipbox[] = {0xf0c00, 0xf0c01, 0xf0c02, 0xf0c03, 0xf0c04};
const uint32_t gradient_p2_skewed[] = {0xf0d00};
const uint32_t variable_alpha[] = {0xf1000};
};  // namespace ColrV1TestDefinitions

#define DEF_COLRV1_GM_SKEW_ROTATE(TEST_CATEGORY, SKEW, ROTATE) \
    DEF_GM(return new ColrV1GM(#TEST_CATEGORY, ColrV1TestDefinitions::TEST_CATEGORY, SKEW, ROTATE));

#define DEF_COLRV1_GM(TEST_CATEGORY) DEF_COLRV1_GM_SKEW_ROTATE(TEST_CATEGORY, 0.f, 0.f);


DEF_COLRV1_GM(clipbox);
DEF_COLRV1_GM(composite_mode);
DEF_COLRV1_GM_SKEW_ROTATE(composite_mode, -0.5f, 0.f);
DEF_COLRV1_GM_SKEW_ROTATE(composite_mode, -0.5f, 20.f);
DEF_COLRV1_GM_SKEW_ROTATE(composite_mode, 0.f, 20.f);
DEF_COLRV1_GM(extend_mode);
DEF_COLRV1_GM_SKEW_ROTATE(extend_mode, -0.5f, 0.f);
DEF_COLRV1_GM_SKEW_ROTATE(extend_mode, -0.5f, 20.f);
DEF_COLRV1_GM_SKEW_ROTATE(extend_mode, 0.f, 20.f);
DEF_COLRV1_GM(foreground_color);
DEF_COLRV1_GM(gradient_p2_skewed);
DEF_COLRV1_GM(gradient_stops_repeat);
DEF_COLRV1_GM_SKEW_ROTATE(gradient_stops_repeat, -0.5f, 0.f);
DEF_COLRV1_GM_SKEW_ROTATE(gradient_stops_repeat, -0.5f, 20.f);
DEF_COLRV1_GM_SKEW_ROTATE(gradient_stops_repeat, 0.f, 20.f);
DEF_COLRV1_GM(paint_rotate);
DEF_COLRV1_GM(paint_scale);
DEF_COLRV1_GM(paint_skew);
DEF_COLRV1_GM(paint_transform);
DEF_COLRV1_GM(paint_translate);
DEF_COLRV1_GM(sweep_varsweep);
DEF_COLRV1_GM_SKEW_ROTATE(sweep_varsweep, -0.5f, 0.f);
DEF_COLRV1_GM_SKEW_ROTATE(sweep_varsweep, -0.5f, 20.f);
DEF_COLRV1_GM_SKEW_ROTATE(sweep_varsweep, 0.f, 20.f);
DEF_COLRV1_GM(variable_alpha);

}  // namespace skiagm
