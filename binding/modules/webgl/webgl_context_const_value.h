// ClearBufferMask
WEBGL_CONSTANT(DEPTH_BUFFER_BIT, 0x00000100);
WEBGL_CONSTANT(STENCIL_BUFFER_BIT, 0x00000400);
WEBGL_CONSTANT(COLOR_BUFFER_BIT, 0x00004000);

// BeginMode
WEBGL_CONSTANT(POINTS, 0x0000);
WEBGL_CONSTANT(LINES, 0x0001);
WEBGL_CONSTANT(LINE_LOOP, 0x0002);
WEBGL_CONSTANT(LINE_STRIP, 0x0003);
WEBGL_CONSTANT(TRIANGLES, 0x0004);
WEBGL_CONSTANT(TRIANGLE_STRIP, 0x0005);
WEBGL_CONSTANT(TRIANGLE_FAN, 0x0006);

// BlendingFactorDest
WEBGL_CONSTANT(ZERO, 0);
WEBGL_CONSTANT(ONE, 1);
WEBGL_CONSTANT(SRC_COLOR, 0x0300);
WEBGL_CONSTANT(ONE_MINUS_SRC_COLOR, 0x0301);
WEBGL_CONSTANT(SRC_ALPHA, 0x0302);
WEBGL_CONSTANT(ONE_MINUS_SRC_ALPHA, 0x0303);
WEBGL_CONSTANT(DST_ALPHA, 0x0304);
WEBGL_CONSTANT(ONE_MINUS_DST_ALPHA, 0x0305);

// BlendingFactorSrc
WEBGL_CONSTANT(DST_COLOR, 0x0306);
WEBGL_CONSTANT(ONE_MINUS_DST_COLOR, 0x0307);
WEBGL_CONSTANT(SRC_ALPHA_SATURATE, 0x0308);

// BlendEquationSeparate
WEBGL_CONSTANT(FUNC_ADD, 0x8006);
WEBGL_CONSTANT(BLEND_EQUATION, 0x8009);
WEBGL_CONSTANT(BLEND_EQUATION_RGB, 0x8009);   // same as BLEND_EQUATION
WEBGL_CONSTANT(BLEND_EQUATION_ALPHA, 0x883D);

// BlendSubtract
WEBGL_CONSTANT(FUNC_SUBTRACT, 0x800A);
WEBGL_CONSTANT(FUNC_REVERSE_SUBTRACT, 0x800B);

// Separate Blend Functions
WEBGL_CONSTANT(BLEND_DST_RGB, 0x80C8);
WEBGL_CONSTANT(BLEND_SRC_RGB, 0x80C9);
WEBGL_CONSTANT(BLEND_DST_ALPHA, 0x80CA);
WEBGL_CONSTANT(BLEND_SRC_ALPHA, 0x80CB);
WEBGL_CONSTANT(CONSTANT_COLOR, 0x8001);
WEBGL_CONSTANT(ONE_MINUS_CONSTANT_COLOR, 0x8002);
WEBGL_CONSTANT(CONSTANT_ALPHA, 0x8003);
WEBGL_CONSTANT(ONE_MINUS_CONSTANT_ALPHA, 0x8004);
WEBGL_CONSTANT(BLEND_COLOR, 0x8005);

// Buffer Objects
WEBGL_CONSTANT(ARRAY_BUFFER, 0x8892);
WEBGL_CONSTANT(ELEMENT_ARRAY_BUFFER, 0x8893);
WEBGL_CONSTANT(ARRAY_BUFFER_BINDING, 0x8894);
WEBGL_CONSTANT(ELEMENT_ARRAY_BUFFER_BINDING, 0x8895);

WEBGL_CONSTANT(STREAM_DRAW, 0x88E0);
WEBGL_CONSTANT(STATIC_DRAW, 0x88E4);
WEBGL_CONSTANT(DYNAMIC_DRAW, 0x88E8);

WEBGL_CONSTANT(BUFFER_SIZE, 0x8764);
WEBGL_CONSTANT(BUFFER_USAGE, 0x8765);

WEBGL_CONSTANT(CURRENT_VERTEX_ATTRIB, 0x8626);

// CullFaceMode
WEBGL_CONSTANT(FRONT, 0x0404);
WEBGL_CONSTANT(BACK, 0x0405);
WEBGL_CONSTANT(FRONT_AND_BACK, 0x0408);

// EnableCap
WEBGL_CONSTANT(TEXTURE_2D, 0x0DE1);
WEBGL_CONSTANT(CULL_FACE, 0x0B44);
WEBGL_CONSTANT(BLEND, 0x0BE2);
WEBGL_CONSTANT(DITHER, 0x0BD0);
WEBGL_CONSTANT(STENCIL_TEST, 0x0B90);
WEBGL_CONSTANT(DEPTH_TEST, 0x0B71);
WEBGL_CONSTANT(SCISSOR_TEST, 0x0C11);
WEBGL_CONSTANT(POLYGON_OFFSET_FILL, 0x8037);
WEBGL_CONSTANT(SAMPLE_ALPHA_TO_COVERAGE, 0x809E);
WEBGL_CONSTANT(SAMPLE_COVERAGE, 0x80A0);

// ErrorCode
WEBGL_CONSTANT(NO_ERROR, 0);
WEBGL_CONSTANT(INVALID_ENUM, 0x0500);
WEBGL_CONSTANT(INVALID_VALUE, 0x0501);
WEBGL_CONSTANT(INVALID_OPERATION, 0x0502);
WEBGL_CONSTANT(OUT_OF_MEMORY, 0x0505);

// FrontFaceDirection
WEBGL_CONSTANT(CW, 0x0900);
WEBGL_CONSTANT(CCW, 0x0901);

// GetPName
WEBGL_CONSTANT(LINE_WIDTH, 0x0B21);
WEBGL_CONSTANT(ALIASED_POINT_SIZE_RANGE, 0x846D);
WEBGL_CONSTANT(ALIASED_LINE_WIDTH_RANGE, 0x846E);
WEBGL_CONSTANT(CULL_FACE_MODE, 0x0B45);
WEBGL_CONSTANT(FRONT_FACE, 0x0B46);
WEBGL_CONSTANT(DEPTH_RANGE, 0x0B70);
WEBGL_CONSTANT(DEPTH_WRITEMASK, 0x0B72);
WEBGL_CONSTANT(DEPTH_CLEAR_VALUE, 0x0B73);
WEBGL_CONSTANT(DEPTH_FUNC, 0x0B74);
WEBGL_CONSTANT(STENCIL_CLEAR_VALUE, 0x0B91);
WEBGL_CONSTANT(STENCIL_FUNC, 0x0B92);
WEBGL_CONSTANT(STENCIL_FAIL, 0x0B94);
WEBGL_CONSTANT(STENCIL_PASS_DEPTH_FAIL, 0x0B95);
WEBGL_CONSTANT(STENCIL_PASS_DEPTH_PASS, 0x0B96);
WEBGL_CONSTANT(STENCIL_REF, 0x0B97);
WEBGL_CONSTANT(STENCIL_VALUE_MASK, 0x0B93);
WEBGL_CONSTANT(STENCIL_WRITEMASK, 0x0B98);
WEBGL_CONSTANT(STENCIL_BACK_FUNC, 0x8800);
WEBGL_CONSTANT(STENCIL_BACK_FAIL, 0x8801);
WEBGL_CONSTANT(STENCIL_BACK_PASS_DEPTH_FAIL, 0x8802);
WEBGL_CONSTANT(STENCIL_BACK_PASS_DEPTH_PASS, 0x8803);
WEBGL_CONSTANT(STENCIL_BACK_REF, 0x8CA3);
WEBGL_CONSTANT(STENCIL_BACK_VALUE_MASK, 0x8CA4);
WEBGL_CONSTANT(STENCIL_BACK_WRITEMASK, 0x8CA5);
WEBGL_CONSTANT(VIEWPORT, 0x0BA2);
WEBGL_CONSTANT(SCISSOR_BOX, 0x0C10);
WEBGL_CONSTANT(COLOR_CLEAR_VALUE, 0x0C22);
WEBGL_CONSTANT(COLOR_WRITEMASK, 0x0C23);
WEBGL_CONSTANT(UNPACK_ALIGNMENT, 0x0CF5);
WEBGL_CONSTANT(PACK_ALIGNMENT, 0x0D05);
WEBGL_CONSTANT(MAX_TEXTURE_SIZE, 0x0D33);
WEBGL_CONSTANT(MAX_VIEWPORT_DIMS, 0x0D3A);
WEBGL_CONSTANT(SUBPIXEL_BITS, 0x0D50);
WEBGL_CONSTANT(RED_BITS, 0x0D52);
WEBGL_CONSTANT(GREEN_BITS, 0x0D53);
WEBGL_CONSTANT(BLUE_BITS, 0x0D54);
WEBGL_CONSTANT(ALPHA_BITS, 0x0D55);
WEBGL_CONSTANT(DEPTH_BITS, 0x0D56);
WEBGL_CONSTANT(STENCIL_BITS, 0x0D57);
WEBGL_CONSTANT(POLYGON_OFFSET_UNITS, 0x2A00);
WEBGL_CONSTANT(POLYGON_OFFSET_FACTOR, 0x8038);
WEBGL_CONSTANT(TEXTURE_BINDING_2D, 0x8069);
WEBGL_CONSTANT(SAMPLE_BUFFERS, 0x80A8);
WEBGL_CONSTANT(SAMPLES, 0x80A9);
WEBGL_CONSTANT(SAMPLE_COVERAGE_VALUE, 0x80AA);
WEBGL_CONSTANT(SAMPLE_COVERAGE_INVERT, 0x80AB);

// GetTextureParameter
WEBGL_CONSTANT(NUM_COMPRESSED_TEXTURE_FORMATS, 0x86A2);
WEBGL_CONSTANT(COMPRESSED_TEXTURE_FORMATS, 0x86A3);

// HintMode
WEBGL_CONSTANT(DONT_CARE, 0x1100);
WEBGL_CONSTANT(FASTEST, 0x1101);
WEBGL_CONSTANT(NICEST, 0x1102);

// HintTarget
WEBGL_CONSTANT(GENERATE_MIPMAP_HINT, 0x8192);

// DataType
WEBGL_CONSTANT(BYTE, 0x1400);
WEBGL_CONSTANT(UNSIGNED_BYTE, 0x1401);
WEBGL_CONSTANT(SHORT, 0x1402);
WEBGL_CONSTANT(UNSIGNED_SHORT, 0x1403);
WEBGL_CONSTANT(INT, 0x1404);
WEBGL_CONSTANT(UNSIGNED_INT, 0x1405);
WEBGL_CONSTANT(FLOAT, 0x1406);

// PixelFormat
WEBGL_CONSTANT(DEPTH_COMPONENT, 0x1902);
WEBGL_CONSTANT(ALPHA, 0x1906);
WEBGL_CONSTANT(RGB, 0x1907);
WEBGL_CONSTANT(RGBA, 0x1908);
WEBGL_CONSTANT(LUMINANCE, 0x1909);
WEBGL_CONSTANT(LUMINANCE_ALPHA, 0x190A);

// PixelType
WEBGL_CONSTANT(UNSIGNED_SHORT_4_4_4_4, 0x8033);
WEBGL_CONSTANT(UNSIGNED_SHORT_5_5_5_1, 0x8034);
WEBGL_CONSTANT(UNSIGNED_SHORT_5_6_5, 0x8363);

// Shaders
WEBGL_CONSTANT(FRAGMENT_SHADER, 0x8B30);
WEBGL_CONSTANT(VERTEX_SHADER, 0x8B31);
WEBGL_CONSTANT(MAX_VERTEX_ATTRIBS, 0x8869);
WEBGL_CONSTANT(MAX_VERTEX_UNIFORM_VECTORS, 0x8DFB);
WEBGL_CONSTANT(MAX_VARYING_VECTORS, 0x8DFC);
WEBGL_CONSTANT(MAX_COMBINED_TEXTURE_IMAGE_UNITS, 0x8B4D);
WEBGL_CONSTANT(MAX_VERTEX_TEXTURE_IMAGE_UNITS, 0x8B4C);
WEBGL_CONSTANT(MAX_TEXTURE_IMAGE_UNITS, 0x8872);
WEBGL_CONSTANT(MAX_FRAGMENT_UNIFORM_VECTORS, 0x8DFD);
WEBGL_CONSTANT(SHADER_TYPE, 0x8B4F);
WEBGL_CONSTANT(DELETE_STATUS, 0x8B80);
WEBGL_CONSTANT(LINK_STATUS, 0x8B82);
WEBGL_CONSTANT(VALIDATE_STATUS, 0x8B83);
WEBGL_CONSTANT(ATTACHED_SHADERS, 0x8B85);
WEBGL_CONSTANT(ACTIVE_UNIFORMS, 0x8B86);
WEBGL_CONSTANT(ACTIVE_UNIFORM_MAX_LENGTH, 0x8B87);
WEBGL_CONSTANT(ACTIVE_ATTRIBUTES, 0x8B89);
WEBGL_CONSTANT(ACTIVE_ATTRIBUTE_MAX_LENGTH, 0x8B8A);
WEBGL_CONSTANT(SHADING_LANGUAGE_VERSION, 0x8B8C);
WEBGL_CONSTANT(CURRENT_PROGRAM, 0x8B8D);

// StencilFunction
WEBGL_CONSTANT(NEVER, 0x0200);
WEBGL_CONSTANT(LESS, 0x0201);
WEBGL_CONSTANT(EQUAL, 0x0202);
WEBGL_CONSTANT(LEQUAL, 0x0203);
WEBGL_CONSTANT(GREATER, 0x0204);
WEBGL_CONSTANT(NOTEQUAL, 0x0205);
WEBGL_CONSTANT(GEQUAL, 0x0206);
WEBGL_CONSTANT(ALWAYS, 0x0207);

// StencilOp
WEBGL_CONSTANT(KEEP, 0x1E00);
WEBGL_CONSTANT(REPLACE, 0x1E01);
WEBGL_CONSTANT(INCR, 0x1E02);
WEBGL_CONSTANT(DECR, 0x1E03);
WEBGL_CONSTANT(INVERT, 0x150A);
WEBGL_CONSTANT(INCR_WRAP, 0x8507);
WEBGL_CONSTANT(DECR_WRAP, 0x8508);

// StringName
WEBGL_CONSTANT(VENDOR, 0x1F00);
WEBGL_CONSTANT(RENDERER, 0x1F01);
WEBGL_CONSTANT(VERSION, 0x1F02);

// TextureMagFilter
WEBGL_CONSTANT(NEAREST, 0x2600);
WEBGL_CONSTANT(LINEAR, 0x2601);

// TextureMinFilter
WEBGL_CONSTANT(NEAREST_MIPMAP_NEAREST, 0x2700);
WEBGL_CONSTANT(LINEAR_MIPMAP_NEAREST, 0x2701);
WEBGL_CONSTANT(NEAREST_MIPMAP_LINEAR, 0x2702);
WEBGL_CONSTANT(LINEAR_MIPMAP_LINEAR, 0x2703);

// TextureParameterName
WEBGL_CONSTANT(TEXTURE_MAG_FILTER, 0x2800);
WEBGL_CONSTANT(TEXTURE_MIN_FILTER, 0x2801);
WEBGL_CONSTANT(TEXTURE_WRAP_S, 0x2802);
WEBGL_CONSTANT(TEXTURE_WRAP_T, 0x2803);

// TextureTarget
WEBGL_CONSTANT(TEXTURE, 0x1702);
WEBGL_CONSTANT(TEXTURE_CUBE_MAP, 0x8513);
WEBGL_CONSTANT(TEXTURE_BINDING_CUBE_MAP, 0x8514);
WEBGL_CONSTANT(TEXTURE_CUBE_MAP_POSITIVE_X, 0x8515);
WEBGL_CONSTANT(TEXTURE_CUBE_MAP_NEGATIVE_X, 0x8516);
WEBGL_CONSTANT(TEXTURE_CUBE_MAP_POSITIVE_Y, 0x8517);
WEBGL_CONSTANT(TEXTURE_CUBE_MAP_NEGATIVE_Y, 0x8518);
WEBGL_CONSTANT(TEXTURE_CUBE_MAP_POSITIVE_Z, 0x8519);
WEBGL_CONSTANT(TEXTURE_CUBE_MAP_NEGATIVE_Z, 0x851A);
WEBGL_CONSTANT(MAX_CUBE_MAP_TEXTURE_SIZE, 0x851C);

// TextureUnit
WEBGL_CONSTANT(TEXTURE0, 0x84C0);
WEBGL_CONSTANT(TEXTURE1, 0x84C1);
WEBGL_CONSTANT(TEXTURE2, 0x84C2);
WEBGL_CONSTANT(TEXTURE3, 0x84C3);
WEBGL_CONSTANT(TEXTURE4, 0x84C4);
WEBGL_CONSTANT(TEXTURE5, 0x84C5);
WEBGL_CONSTANT(TEXTURE6, 0x84C6);
WEBGL_CONSTANT(TEXTURE7, 0x84C7);
WEBGL_CONSTANT(TEXTURE8, 0x84C8);
WEBGL_CONSTANT(TEXTURE9, 0x84C9);
WEBGL_CONSTANT(TEXTURE10, 0x84CA);
WEBGL_CONSTANT(TEXTURE11, 0x84CB);
WEBGL_CONSTANT(TEXTURE12, 0x84CC);
WEBGL_CONSTANT(TEXTURE13, 0x84CD);
WEBGL_CONSTANT(TEXTURE14, 0x84CE);
WEBGL_CONSTANT(TEXTURE15, 0x84CF);
WEBGL_CONSTANT(TEXTURE16, 0x84D0);
WEBGL_CONSTANT(TEXTURE17, 0x84D1);
WEBGL_CONSTANT(TEXTURE18, 0x84D2);
WEBGL_CONSTANT(TEXTURE19, 0x84D3);
WEBGL_CONSTANT(TEXTURE20, 0x84D4);
WEBGL_CONSTANT(TEXTURE21, 0x84D5);
WEBGL_CONSTANT(TEXTURE22, 0x84D6);
WEBGL_CONSTANT(TEXTURE23, 0x84D7);
WEBGL_CONSTANT(TEXTURE24, 0x84D8);
WEBGL_CONSTANT(TEXTURE25, 0x84D9);
WEBGL_CONSTANT(TEXTURE26, 0x84DA);
WEBGL_CONSTANT(TEXTURE27, 0x84DB);
WEBGL_CONSTANT(TEXTURE28, 0x84DC);
WEBGL_CONSTANT(TEXTURE29, 0x84DD);
WEBGL_CONSTANT(TEXTURE30, 0x84DE);
WEBGL_CONSTANT(TEXTURE31, 0x84DF);
WEBGL_CONSTANT(ACTIVE_TEXTURE, 0x84E0);

// TextureWrapMode
WEBGL_CONSTANT(REPEAT, 0x2901);
WEBGL_CONSTANT(CLAMP_TO_EDGE, 0x812F);
WEBGL_CONSTANT(MIRRORED_REPEAT, 0x8370);

// Uniform Types
WEBGL_CONSTANT(FLOAT_VEC2, 0x8B50);
WEBGL_CONSTANT(FLOAT_VEC3, 0x8B51);
WEBGL_CONSTANT(FLOAT_VEC4, 0x8B52);
WEBGL_CONSTANT(INT_VEC2, 0x8B53);
WEBGL_CONSTANT(INT_VEC3, 0x8B54);
WEBGL_CONSTANT(INT_VEC4, 0x8B55);
WEBGL_CONSTANT(BOOL, 0x8B56);
WEBGL_CONSTANT(BOOL_VEC2, 0x8B57);
WEBGL_CONSTANT(BOOL_VEC3, 0x8B58);
WEBGL_CONSTANT(BOOL_VEC4, 0x8B59);
WEBGL_CONSTANT(FLOAT_MAT2, 0x8B5A);
WEBGL_CONSTANT(FLOAT_MAT3, 0x8B5B);
WEBGL_CONSTANT(FLOAT_MAT4, 0x8B5C);
WEBGL_CONSTANT(SAMPLER_2D, 0x8B5E);
WEBGL_CONSTANT(SAMPLER_CUBE, 0x8B60);

// Vertex Arrays
WEBGL_CONSTANT(VERTEX_ATTRIB_ARRAY_ENABLED, 0x8622);
WEBGL_CONSTANT(VERTEX_ATTRIB_ARRAY_SIZE, 0x8623);
WEBGL_CONSTANT(VERTEX_ATTRIB_ARRAY_STRIDE, 0x8624);
WEBGL_CONSTANT(VERTEX_ATTRIB_ARRAY_TYPE, 0x8625);
WEBGL_CONSTANT(VERTEX_ATTRIB_ARRAY_NORMALIZED, 0x886A);
WEBGL_CONSTANT(VERTEX_ATTRIB_ARRAY_POINTER, 0x8645);
WEBGL_CONSTANT(VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, 0x889F);

// Shader Source
WEBGL_CONSTANT(COMPILE_STATUS, 0x8B81);
WEBGL_CONSTANT(INFO_LOG_LENGTH, 0x8B84);
WEBGL_CONSTANT(SHADER_SOURCE_LENGTH, 0x8B88);

// Shader Precision-Specified Types
WEBGL_CONSTANT(LOW_FLOAT, 0x8DF0);
WEBGL_CONSTANT(MEDIUM_FLOAT, 0x8DF1);
WEBGL_CONSTANT(HIGH_FLOAT, 0x8DF2);
WEBGL_CONSTANT(LOW_INT, 0x8DF3);
WEBGL_CONSTANT(MEDIUM_INT, 0x8DF4);
WEBGL_CONSTANT(HIGH_INT, 0x8DF5);

// Framebuffer Object.
WEBGL_CONSTANT(FRAMEBUFFER, 0x8D40);
WEBGL_CONSTANT(RENDERBUFFER, 0x8D41);

WEBGL_CONSTANT(RGBA4, 0x8056);
WEBGL_CONSTANT(RGB5_A1, 0x8057);
WEBGL_CONSTANT(RGB565, 0x8D62);
WEBGL_CONSTANT(DEPTH_COMPONENT16, 0x81A5);
WEBGL_CONSTANT(STENCIL_INDEX, 0x1901);
WEBGL_CONSTANT(STENCIL_INDEX8, 0x8D48);
WEBGL_CONSTANT(DEPTH_STENCIL, 0x84F9);

WEBGL_CONSTANT(RENDERBUFFER_WIDTH, 0x8D42);
WEBGL_CONSTANT(RENDERBUFFER_HEIGHT, 0x8D43);
WEBGL_CONSTANT(RENDERBUFFER_INTERNAL_FORMAT, 0x8D44);
WEBGL_CONSTANT(RENDERBUFFER_RED_SIZE, 0x8D50);
WEBGL_CONSTANT(RENDERBUFFER_GREEN_SIZE, 0x8D51);
WEBGL_CONSTANT(RENDERBUFFER_BLUE_SIZE, 0x8D52);
WEBGL_CONSTANT(RENDERBUFFER_ALPHA_SIZE, 0x8D53);
WEBGL_CONSTANT(RENDERBUFFER_DEPTH_SIZE, 0x8D54);
WEBGL_CONSTANT(RENDERBUFFER_STENCIL_SIZE, 0x8D55);

WEBGL_CONSTANT(FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, 0x8CD0);
WEBGL_CONSTANT(FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, 0x8CD1);
WEBGL_CONSTANT(FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL, 0x8CD2);
WEBGL_CONSTANT(FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE, 0x8CD3);

WEBGL_CONSTANT(COLOR_ATTACHMENT0, 0x8CE0);
WEBGL_CONSTANT(DEPTH_ATTACHMENT, 0x8D00);
WEBGL_CONSTANT(STENCIL_ATTACHMENT, 0x8D20);
WEBGL_CONSTANT(DEPTH_STENCIL_ATTACHMENT, 0x821A);

WEBGL_CONSTANT(NONE, 0);

WEBGL_CONSTANT(FRAMEBUFFER_COMPLETE, 0x8CD5);
WEBGL_CONSTANT(FRAMEBUFFER_INCOMPLETE_ATTACHMENT, 0x8CD6);
WEBGL_CONSTANT(FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, 0x8CD7);
WEBGL_CONSTANT(FRAMEBUFFER_INCOMPLETE_DIMENSIONS, 0x8CD9);
WEBGL_CONSTANT(FRAMEBUFFER_UNSUPPORTED, 0x8CDD);

WEBGL_CONSTANT(FRAMEBUFFER_BINDING, 0x8CA6);
WEBGL_CONSTANT(RENDERBUFFER_BINDING, 0x8CA7);
WEBGL_CONSTANT(MAX_RENDERBUFFER_SIZE, 0x84E8);

WEBGL_CONSTANT(INVALID_FRAMEBUFFER_OPERATION, 0x0506);

// WebGL-specific enums
WEBGL_CONSTANT(UNPACK_FLIP_Y_WEBGL, 0x9240);
WEBGL_CONSTANT(UNPACK_PREMULTIPLY_ALPHA_WEBGL, 0x9241);
WEBGL_CONSTANT(CONTEXT_LOST_WEBGL, 0x9242);
WEBGL_CONSTANT(UNPACK_COLORSPACE_CONVERSION_WEBGL, 0x9243);
WEBGL_CONSTANT(BROWSER_DEFAULT_WEBGL, 0x9244);