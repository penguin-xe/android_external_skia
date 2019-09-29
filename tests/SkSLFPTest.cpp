/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/sksl/SkSLCompiler.h"
#include "src/sksl/SkSLStringStream.h"

#include "tests/Test.h"

static void test(skiatest::Reporter* r, const char* src, const GrShaderCaps& caps,
                 std::vector<const char*> expectedH, std::vector<const char*> expectedCPP) {
    SkSL::Program::Settings settings;
    settings.fCaps = &caps;
    SkSL::Compiler compiler;
    SkSL::StringStream output;
    std::unique_ptr<SkSL::Program> program = compiler.convertProgram(
                                                             SkSL::Program::kFragmentProcessor_Kind,
                                                             SkSL::String(src),
                                                             settings);
    if (!program) {
        SkDebugf("Unexpected error compiling %s\n%s", src, compiler.errorText().c_str());
        return;
    }
    REPORTER_ASSERT(r, program);
    bool success = compiler.toH(*program, "Test", output);
    if (!success) {
        SkDebugf("Unexpected error compiling %s\n%s", src, compiler.errorText().c_str());
    }
    REPORTER_ASSERT(r, success);
    if (success) {
        for (const char* expected : expectedH) {
            bool found = strstr(output.str().c_str(), expected);
            if (!found) {
                SkDebugf("HEADER MISMATCH:\nsource:\n%s\n\nexpected:\n'%s'\n\nreceived:\n'%s'", src,
                         expected, output.str().c_str());
            }
            REPORTER_ASSERT(r, found);
        }
    }
    output.reset();
    success = compiler.toCPP(*program, "Test", output);
    if (!success) {
        SkDebugf("Unexpected error compiling %s\n%s", src, compiler.errorText().c_str());
    }
    REPORTER_ASSERT(r, success);
    if (success) {
        for (const char* expected : expectedCPP) {
            bool found = strstr(output.str().c_str(), expected);
            if (!found) {
                SkDebugf("CPP MISMATCH:\nsource:\n%s\n\nexpected:\n'%s'\n\nreceived:\n'%s'", src,
                         expected, output.str().c_str());
            }
            REPORTER_ASSERT(r, found);
        }
    }
}

static void test_failure(skiatest::Reporter* r, const char* src, const char* error) {
    SkSL::Compiler compiler;
    SkSL::Program::Settings settings;
    sk_sp<GrShaderCaps> caps = SkSL::ShaderCapsFactory::Default();
    settings.fCaps = caps.get();
    std::unique_ptr<SkSL::Program> program = compiler.convertProgram(
                                                             SkSL::Program::kFragmentProcessor_Kind,
                                                             SkSL::String(src),
                                                             settings);
    if (!compiler.errorCount()) {
        compiler.optimize(*program);
    }
    SkSL::String skError(error);
    if (compiler.errorText() != skError) {
        SkDebugf("SKSL ERROR:\n    source: %s\n    expected: %s    received: %s", src, error,
                 compiler.errorText().c_str());
    }
    REPORTER_ASSERT(r, compiler.errorText() == skError);
}

DEF_TEST(SkSLFPHelloWorld, r) {
    test(r,
         "/* HEADER */"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "/* HEADER */\n"
             "\n"
             "/**************************************************************************************************\n"
             " *** This file was autogenerated from GrTest.fp; do not modify.\n"
             " **************************************************************************************************/\n"
             "#ifndef GrTest_DEFINED\n"
             "#define GrTest_DEFINED\n"
             "#include \"include/core/SkTypes.h\"\n\n"
             "#include \"src/gpu/GrCoordTransform.h\"\n"
             "#include \"src/gpu/GrFragmentProcessor.h\"\n"
             "class GrTest : public GrFragmentProcessor {\n"
             "public:\n"
             "    static std::unique_ptr<GrFragmentProcessor> Make() {\n"
             "        return std::unique_ptr<GrFragmentProcessor>(new GrTest());\n"
             "    }\n"
             "    GrTest(const GrTest& src);\n"
             "    std::unique_ptr<GrFragmentProcessor> clone() const override;\n"
             "    const char* name() const override { return \"Test\"; }\n"
             "private:\n"
             "    GrTest()\n"
             "    : INHERITED(kGrTest_ClassID, kNone_OptimizationFlags) {\n"
             "    }\n"
             "    GrGLSLFragmentProcessor* onCreateGLSLInstance() const override;\n"
             "    void onGetGLSLProcessorKey(const GrShaderCaps&,GrProcessorKeyBuilder*) const "
                 "override;\n"
             "    bool onIsEqual(const GrFragmentProcessor&) const override;\n"
             "    GR_DECLARE_FRAGMENT_PROCESSOR_TEST\n"
             "    typedef GrFragmentProcessor INHERITED;\n"
             "};\n"
             "#endif\n"
         },
         {
             "/**************************************************************************************************\n"
             " *** This file was autogenerated from GrTest.fp; do not modify.\n"
             " **************************************************************************************************/\n"
             "#include \"GrTest.h\"\n\n"
             "#include \"include/gpu/GrTexture.h\"\n"
             "#include \"src/gpu/glsl/GrGLSLFragmentProcessor.h\"\n"
             "#include \"src/gpu/glsl/GrGLSLFragmentShaderBuilder.h\"\n"
             "#include \"src/gpu/glsl/GrGLSLProgramBuilder.h\"\n"
             "#include \"src/sksl/SkSLCPP.h\"\n"
             "#include \"src/sksl/SkSLUtil.h\"\n"
             "class GrGLSLTest : public GrGLSLFragmentProcessor {\n"
             "public:\n"
             "    GrGLSLTest() {}\n"
             "    void emitCode(EmitArgs& args) override {\n"
             "        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;\n"
             "        const GrTest& _outer = args.fFp.cast<GrTest>();\n"
             "        (void) _outer;\n"
             "        fragBuilder->codeAppendf(\"%s = half4(1.0);\\n\", args.fOutputColor);\n"
             "    }\n"
             "private:\n"
             "    void onSetData(const GrGLSLProgramDataManager& pdman, "
                                "const GrFragmentProcessor& _proc) override {\n"
             "    }\n"
             "};\n"
             "GrGLSLFragmentProcessor* GrTest::onCreateGLSLInstance() const {\n"
             "    return new GrGLSLTest();\n"
             "}\n"
             "void GrTest::onGetGLSLProcessorKey(const GrShaderCaps& caps, "
                                                "GrProcessorKeyBuilder* b) const {\n"
             "}\n"
             "bool GrTest::onIsEqual(const GrFragmentProcessor& other) const {\n"
             "    const GrTest& that = other.cast<GrTest>();\n"
             "    (void) that;\n"
             "    return true;\n"
             "}\n"
             "GrTest::GrTest(const GrTest& src)\n"
             ": INHERITED(kGrTest_ClassID, src.optimizationFlags()) {\n"
             "}\n"
             "std::unique_ptr<GrFragmentProcessor> GrTest::clone() const {\n"
             "    return std::unique_ptr<GrFragmentProcessor>(new GrTest(*this));\n"
             "}\n"
         });
}

DEF_TEST(SkSLFPInput, r) {
    test(r,
         "layout(key) in half2 point;"
         "void main() {"
         "sk_OutColor = half4(point, point);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "static std::unique_ptr<GrFragmentProcessor> Make(SkPoint point) {",
             "return std::unique_ptr<GrFragmentProcessor>(new GrTest(point));",
             "GrTest(SkPoint point)",
             ", point(point)"
         },
         {
             "fragBuilder->codeAppendf(\"%s = half4(half2(%f, %f), half2(%f, %f));\\n\", "
                                      "args.fOutputColor, _outer.point.fX, _outer.point.fY, "
                                      "_outer.point.fX, _outer.point.fY);",
             "if (point != that.point) return false;"
         });
}

DEF_TEST(SkSLFPUniform, r) {
    test(r,
         "uniform half4 color;"
         "void main() {"
         "sk_OutColor = color;"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "static std::unique_ptr<GrFragmentProcessor> Make()"
         },
         {
            "colorVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf4_GrSLType, "
                                                        "\"color\");",
         });
}

// SkSLFPInUniform tests the simplest plumbing case, default type, no tracking
// with a setUniform template that supports inlining the value call with no
// local variable.
DEF_TEST(SkSLFPInUniform, r) {
    test(r,
         "in uniform half4 color;"
         "void main() {"
         "sk_OutColor = color;"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "static std::unique_ptr<GrFragmentProcessor> Make(SkRect color) {",
         },
         {
            "colorVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf4_GrSLType, "
                                                        "\"color\");",
            "pdman.set4fv(colorVar, 1, reinterpret_cast<const float*>(&(_outer.color)));"
         });
}

// As above, but tests in uniform's ability to override the default ctype.
DEF_TEST(SkSLFPInUniformCType, r) {
    test(r,
         "layout(ctype=SkPMColor4f) in uniform half4 color;"
         "void main() {"
         "sk_OutColor = color;"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "static std::unique_ptr<GrFragmentProcessor> Make(SkPMColor4f color) {",
         },
         {
            "colorVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf4_GrSLType, "
                                                        "\"color\");",
            "pdman.set4fv(colorVar, 1, (_outer.color).vec());"
         });
}

// Add state tracking to the default typed SkRect <-> half4 uniform. But since
// it now has to track state, the value inlining previously done for the
// setUniform call is removed in favor of a local variable.
DEF_TEST(SkSLFPTrackedInUniform, r) {
    test(r,
         "layout(tracked) in uniform half4 color;"
         "void main() {"
         "sk_OutColor = color;"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "static std::unique_ptr<GrFragmentProcessor> Make(SkRect color) {",
         },
         {
            "SkRect colorPrev = SkRect::MakeEmpty();",
            "colorVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf4_GrSLType, "
                                                        "\"color\");",
            "const SkRect& colorValue = _outer.color;",
            "if (colorPrev.isEmpty() || colorPrev != colorValue) {",
            "colorPrev = colorValue;",
            "pdman.set4fv(colorVar, 1, reinterpret_cast<const float*>(&colorValue));"
         });
}

// Test the case where the template does not support variable inlining in
// setUniform (i.e. it references the value multiple times).
DEF_TEST(SkSLFPNonInlinedInUniform, r) {
    test(r,
         "in uniform half2 point;"
         "void main() {"
         "sk_OutColor = half4(point, point);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "static std::unique_ptr<GrFragmentProcessor> Make(SkPoint point) {",
         },
         {
            "pointVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf2_GrSLType, "
                                                        "\"point\");",
            "const SkPoint& pointValue = _outer.point;",
            "pdman.set2f(pointVar, pointValue.fX, pointValue.fY);"
         });
}

// Test handling conditional uniforms (that use when= in layout), combined with
// state tracking and custom ctypes to really put the code generation through its paces.
DEF_TEST(SkSLFPConditionalInUniform, r) {
    test(r,
         "layout(key) in bool test;"
         "layout(ctype=SkPMColor4f, tracked, when=test) in uniform half4 color;"
         "void main() {"
         "  if (test) {"
         "    sk_OutColor = color;"
         "  } else {"
         "    sk_OutColor = half4(1);"
         "  }"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "static std::unique_ptr<GrFragmentProcessor> Make(bool test, SkPMColor4f color) {",
         },
         {
            "SkPMColor4f colorPrev = {SK_FloatNaN, SK_FloatNaN, SK_FloatNaN, SK_FloatNaN}",
            "auto test = _outer.test;",
            "if (test) {",
            "colorVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kHalf4_GrSLType, "
                                                        "\"color\");",
            "if (colorVar.isValid()) {",
            "const SkPMColor4f& colorValue = _outer.color;",
            "if (colorPrev != colorValue) {",
            "colorPrev = colorValue;",
            "pdman.set4fv(colorVar, 1, colorValue.vec());"
         });
}

DEF_TEST(SkSLFPSections, r) {
    test(r,
         "@header { header section }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "header section"
         },
         {});
    test(r,
         "@class { class section }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "class GrTest : public GrFragmentProcessor {\n"
             "public:\n"
             " class section"
         },
         {});
    test(r,
         "@cpp { cpp section }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {},
         {"cpp section"});
    test(r,
         "@constructorParams { int x, float y, std::vector<float> z }"
         "in float w;"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "Make(float w,  int x, float y, std::vector<float> z )",
             "return std::unique_ptr<GrFragmentProcessor>(new GrTest(w, x, y, z));",
             "GrTest(float w,  int x, float y, std::vector<float> z )",
             ", w(w) {"
         },
         {});
    test(r,
         "@constructor { constructor section }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             "private:\n constructor section"
         },
         {});
    test(r,
         "@initializers { initializers section }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
             ": INHERITED(kGrTest_ClassID, kNone_OptimizationFlags)\n    ,  initializers section"
         },
         {});
    test(r,
         "half x = 10;"
         "@emitCode { fragBuilder->codeAppendf(\"half y = %d\\n\", x * 2); }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {},
         {
            "x = 10.0;\n"
            " fragBuilder->codeAppendf(\"half y = %d\\n\", x * 2);"
         });
    test(r,
         "@fields { fields section }"
         "@clone { }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
            "const char* name() const override { return \"Test\"; }\n"
            " fields section private:"
         },
         {});
    test(r,
         "@make { make section }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
            "public:\n"
            " make section"
         },
         {});
    test(r,
         "uniform half calculated;"
         "layout(key) in half provided;"
         "@setData(varName) { varName.set1f(calculated, provided * 2); }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {},
         {
             "void onSetData(const GrGLSLProgramDataManager& varName, "
                            "const GrFragmentProcessor& _proc) override {\n",
             "UniformHandle& calculated = calculatedVar;",
             "auto provided = _outer.provided;",
             "varName.set1f(calculated, provided * 2);"
         });
    test(r,
         "@test(testDataName) { testDataName section }"
         "void main() {"
         "sk_OutColor = half4(1);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {},
         {
             "#if GR_TEST_UTILS\n"
             "std::unique_ptr<GrFragmentProcessor> GrTest::TestCreate(GrProcessorTestData* testDataName) {\n"
             " testDataName section }\n"
             "#endif"
         });
}

DEF_TEST(SkSLFPTransformedCoords, r) {
    test(r,
         "void main() {"
         "sk_OutColor = half4(sk_TransformedCoords2D[0], sk_TransformedCoords2D[0]);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {},
         {
            "SkString sk_TransformedCoords2D_0 = "
                           "fragBuilder->ensureCoords2D(args.fTransformedCoords[0].fVaryingPoint);",
            "fragBuilder->codeAppendf(\"%s = half4(%s, %s);\\n\", args.fOutputColor, "
                    "_outer.computeLocalCoordsInVertexShader() ? sk_TransformedCoords2D_0.c_str() :"
                    " \"_coords\", _outer.computeLocalCoordsInVertexShader() ? "
                    "sk_TransformedCoords2D_0.c_str() : \"_coords\");"
         });

}

DEF_TEST(SkSLFPLayoutWhen, r) {
    test(r,
         "layout(when=someExpression(someOtherExpression())) uniform half sometimes;"
         "void main() {"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {},
         {
            "if (someExpression(someOtherExpression())) {\n"
            "            sometimesVar = args.fUniformHandler->addUniform"
         });

}

DEF_TEST(SkSLFPChildProcessors, r) {
    test(r,
         "in fragmentProcessor child1;"
         "in fragmentProcessor child2;"
         "void main() {"
         "    sk_OutColor = sample(child1) * sample(child2);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
            "this->registerChildProcessor(std::move(child1));",
            "this->registerChildProcessor(std::move(child2));"
         },
         {
            "SkString _sample93(\"_sample93\");\n",
            "this->invokeChild(_outer.child1_index, &_sample93, args);\n",
            "SkString _sample110(\"_sample110\");\n",
            "this->invokeChild(_outer.child2_index, &_sample110, args);\n",
            "fragBuilder->codeAppendf(\"%s = %s * %s;\\n\", args.fOutputColor, _sample93.c_str(), "
                                     "_sample110.c_str());\n",
            "this->registerChildProcessor(src.childProcessor(child1_index).clone());",
            "this->registerChildProcessor(src.childProcessor(child2_index).clone());"
         });
}

DEF_TEST(SkSLFPChildProcessorsWithInput, r) {
    test(r,
         "in fragmentProcessor child1;"
         "in fragmentProcessor child2;"
         "void main() {"
         "    half4 childIn = sk_InColor;"
         "    half4 childOut1 = sample(child1, childIn);"
         "    half4 childOut2 = sample(child2, childOut1);"
         "    sk_OutColor = childOut2;"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
            "this->registerChildProcessor(std::move(child1));",
            "this->registerChildProcessor(std::move(child2));"
         },
         {
            "SkString _input128(\"childIn\");",
            "SkString _sample128(\"_sample128\");",
            "this->invokeChild(_outer.child1_index, _input128.c_str(), &_sample128, args);",
            "fragBuilder->codeAppendf(\"\\nhalf4 childOut1 = %s;\", _sample128.c_str());",
            "SkString _input174(\"childOut1\");",
            "SkString _sample174(\"_sample174\");",
            "this->invokeChild(_outer.child2_index, _input174.c_str(), &_sample174, args);",
            "this->registerChildProcessor(src.childProcessor(child1_index).clone());",
            "this->registerChildProcessor(src.childProcessor(child2_index).clone());"
         });
}

DEF_TEST(SkSLFPChildProcessorWithInputExpression, r) {
    test(r,
         "in fragmentProcessor child;"
         "void main() {"
         "    sk_OutColor = sample(child, sk_InColor * half4(0.5));"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
            "this->registerChildProcessor(std::move(child));",
         },
         {
            "SkString _input64 = SkStringPrintf(\"%s * half4(0.5)\", args.fInputColor);",
            "SkString _sample64(\"_sample64\");",
            "this->invokeChild(_outer.child_index, _input64.c_str(), &_sample64, args);",
            "fragBuilder->codeAppendf(\"%s = %s;\\n\", args.fOutputColor, _sample64.c_str());",
            "this->registerChildProcessor(src.childProcessor(child_index).clone());",
         });
}

DEF_TEST(SkSLFPNestedChildProcessors, r) {
    test(r,
         "in fragmentProcessor child1;"
         "in fragmentProcessor child2;"
         "void main() {"
         "    sk_OutColor = sample(child2, sk_InColor * sample(child1, sk_InColor * half4(0.5)));"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
            "this->registerChildProcessor(std::move(child1));",
            "this->registerChildProcessor(std::move(child2));"
         },
         {
            "SkString _input121 = SkStringPrintf(\"%s * half4(0.5)\", args.fInputColor);",
            "SkString _sample121(\"_sample121\");",
            "this->invokeChild(_outer.child1_index, _input121.c_str(), &_sample121, args);",
            "SkString _input93 = SkStringPrintf(\"%s * %s\", args.fInputColor, _sample121.c_str());",
            "SkString _sample93(\"_sample93\");",
            "this->invokeChild(_outer.child2_index, _input93.c_str(), &_sample93, args);",
            "fragBuilder->codeAppendf(\"%s = %s;\\n\", args.fOutputColor, _sample93.c_str());",
            "this->registerChildProcessor(src.childProcessor(child1_index).clone());",
            "this->registerChildProcessor(src.childProcessor(child2_index).clone());"
         });
}

DEF_TEST(SkSLFPChildFPAndGlobal, r) {
    test(r,
         "in fragmentProcessor child;"
         "bool hasCap = sk_Caps.externalTextureSupport;"
         "void main() {"
         "    if (hasCap) {"
         "        sk_OutColor = sample(child, sk_InColor);"
         "    } else {"
         "        sk_OutColor = half4(1);"
         "    }"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
            "this->registerChildProcessor(std::move(child));"
         },
         {
            "hasCap = sk_Caps.externalTextureSupport;",
            "fragBuilder->codeAppendf(\"bool hasCap = %s;\\nif (hasCap) {\", (hasCap ? \"true\" : "
                                     "\"false\"));",
            "SkString _input130 = SkStringPrintf(\"%s\", args.fInputColor);",
            "SkString _sample130(\"_sample130\");",
            "this->invokeChild(_outer.child_index, _input130.c_str(), &_sample130, args);",
            "fragBuilder->codeAppendf(\"\\n    %s = %s;\\n} else {\\n    %s = half4(1.0);\\n}\\n\","
                                     " args.fOutputColor, _sample130.c_str(), args.fOutputColor);",
            "this->registerChildProcessor(src.childProcessor(child_index).clone());"
         });
}

DEF_TEST(SkSLFPChildProcessorInlineFieldAccess, r) {
    test(r,
         "in fragmentProcessor child;"
         "void main() {"
         "    if (child.preservesOpaqueInput) {"
         "        sk_OutColor = sample(child, sk_InColor);"
         "    } else {"
         "        sk_OutColor = half4(1);"
         "    }"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
            "this->registerChildProcessor(std::move(child));"
         },
         {
            "fragBuilder->codeAppendf(\"if (%s) {\", "
                    "(_outer.childProcessor(_outer.child_index).preservesOpaqueInput() ? ",
            "SkString _input105 = SkStringPrintf(\"%s\", args.fInputColor);",
            "SkString _sample105(\"_sample105\");",
            "this->invokeChild(_outer.child_index, _input105.c_str(), &_sample105, args);",
            "fragBuilder->codeAppendf(\"\\n    %s = %s;\\n} else {\\n    %s = half4(1.0);\\n}\\n\","
                                     " args.fOutputColor, _sample105.c_str(), args.fOutputColor);",
            "this->registerChildProcessor(src.childProcessor(child_index).clone());"
         });
}

DEF_TEST(SkSLFPChildProcessorFieldAccess, r) {
    test(r,
         "in fragmentProcessor child;"
         "bool opaque = child.preservesOpaqueInput;"
         "void main() {"
         "    if (opaque) {"
         "        sk_OutColor = sample(child);"
         "    } else {"
         "        sk_OutColor = half4(0.5);"
         "    }"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {
            "this->registerChildProcessor(std::move(child));"
         },
         {
            "opaque = _outer.childProcessor(_outer.child_index).preservesOpaqueInput();",
            "fragBuilder->codeAppendf(\"bool opaque = %s;\\nif (opaque) {\", (opaque ? \"true\" : "
                                     "\"false\"));",
            "SkString _sample126(\"_sample126\");",
            "this->invokeChild(_outer.child_index, &_sample126, args);",
            "fragBuilder->codeAppendf(\"\\n    %s = %s;\\n} else {\\n    %s = half4(0.5);\\n}\\n\","
                                     " args.fOutputColor, _sample126.c_str(), args.fOutputColor);",
            "this->registerChildProcessor(src.childProcessor(child_index).clone());"
         });
}

DEF_TEST(SkSLFPNullableChildProcessor, r) {
    test(r,
         "in fragmentProcessor? child;"
         "void main() {"
         "    if (child != null) {"
         "        sk_OutColor = sample(child);"
         "    } else {"
         "        sk_OutColor = half4(0.5);"
         "    }"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {},
         {
            "fragBuilder->codeAppendf(\"if (%s) {\", _outer.child_index >= 0 ? \"true\" : "
                                     "\"false\");",
            "SkString _sample93(\"_sample93\");",
            "if (_outer.child_index >= 0) {",
            "this->invokeChild(_outer.child_index, &_sample93, args);",
            "}",
            "fragBuilder->codeAppendf(\"\\n    %s = %s;\\n} else {\\n    %s = half4(0.5);\\n}\\n\","
                                     " args.fOutputColor, _sample93.c_str(), args.fOutputColor);"

         });
}

DEF_TEST(SkSLFPBadIn, r) {
    test_failure(r,
         "in half4 c;"
         "void main() {"
         "    sk_OutColor = c;"
         "}",
         "error: 1: 'in' variable must be either 'uniform' or 'layout(key)', or there must be a "
         "custom @setData function\n1 error\n");
}

DEF_TEST(SkSLFPSampleCoords, r) {
    test(r,
         "in fragmentProcessor child;"
         "@coordTransform { SkMatrix() }"
         "void main() {"
         "    sk_OutColor = sample(child) + sample(child, sk_TransformedCoords2D[0] / 2);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {},
         {
            "SkString _sample94(\"_sample94\");\n",
            "this->invokeChild(_outer.child_index, &_sample94, args);\n",
            "SkString _sample110(\"_sample110\");\n",
            "SkString sk_TransformedCoords2D_0 = fragBuilder->ensureCoords2D("
                                                     "args.fTransformedCoords[0].fVaryingPoint);\n",
            "SkString _coords110 = SkStringPrintf(\"%s / 2.0\", "
                    "_outer.computeLocalCoordsInVertexShader() ? sk_TransformedCoords2D_0.c_str() :"
                    " \"_coords\");\n",
            "this->invokeChild(_outer.child_index, &_sample110, args, _coords110.c_str());\n",
            "fragBuilder->codeAppendf(\"%s = %s + %s;\\n\", args.fOutputColor, _sample94.c_str(), "
                                     "_sample110.c_str());\n"
         });
}

DEF_TEST(SkSLFPFunction, r) {
    test(r,
         "in fragmentProcessor? child;"
         "half4 flip(half4 c) { return c.abgr; }"
         "void main() {"
         "    sk_OutColor = flip(sk_InColor);"
         "}",
         *SkSL::ShaderCapsFactory::Default(),
         {},
         {
            "SkString flip_name;",
            "const GrShaderVar flip_args[] = { GrShaderVar(\"c\", kHalf4_GrSLType)};",
            "fragBuilder->emitFunction(kHalf4_GrSLType, \"flip\", 1, flip_args, "
                                      "\"return c.wzyx;\\n\", &flip_name);",
            "fragBuilder->codeAppendf(\"%s = %s(%s);\\n\", args.fOutputColor, flip_name.c_str(), "
                                      "args.fInputColor);"
         });
}
