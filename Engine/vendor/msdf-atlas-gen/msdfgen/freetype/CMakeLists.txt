# freetype/CMakeLists.txt

project(freetype LANGUAGES C)

file(GLOB_RECURSE header_freetype
	"include/ft2build.h"
	"include/freetype/*.h"
	"include/freetype/config/*.h"
	"include/freetype/internal/*.h"
)
# Add freetype as a static library
add_library(freetype STATIC 
		${header_freetype}
		
		"src/autofit/autofit.c"
		"src/base/ftbase.c"
		"src/base/ftbbox.c"
		"src/base/ftbdf.c"
		"src/base/ftbitmap.c"
		"src/base/ftcid.c"
		"src/base/ftdebug.c"
		"src/base/ftfstype.c"
		"src/base/ftgasp.c"
		"src/base/ftglyph.c"
		"src/base/ftgxval.c"
		"src/base/ftinit.c"
		"src/base/ftmm.c"
		"src/base/ftotval.c"
		"src/base/ftpatent.c"
		"src/base/ftpfr.c"
		"src/base/ftstroke.c"
		"src/base/ftsynth.c"
		"src/base/ftsystem.c"
		"src/base/fttype1.c"
		"src/base/ftwinfnt.c"
		"src/bdf/bdf.c"
		"src/bzip2/ftbzip2.c"
		"src/cache/ftcache.c"
		"src/cff/cff.c"
		"src/cid/type1cid.c"
		"src/gzip/ftgzip.c"
		"src/lzw/ftlzw.c"
		"src/pcf/pcf.c"
		"src/pfr/pfr.c"
		"src/psaux/psaux.c"
		"src/pshinter/pshinter.c"
		"src/psnames/psnames.c"
		"src/raster/raster.c"
		"src/sdf/sdf.c"
		"src/sfnt/sfnt.c"
		"src/smooth/smooth.c"
		"src/truetype/truetype.c"
		"src/type1/type1.c"
		"src/type42/type42.c"
		"src/winfonts/winfnt.c"
)

# Include directories
target_include_directories(freetype PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Define build flags
target_compile_definitions(freetype PUBLIC
    FT2_BUILD_LIBRARY
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_WARNINGS
)

# Platform-specific configurations for Windows
if(WIN32)
    target_compile_definitions(freetype PRIVATE "_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_SYSTEM_VERSION "latest")
endif()

# Set build configurations
set_target_properties(freetype PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/freetype"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/freetype"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/freetype"
)

# Configurations for Debug, Release, and Dist builds
target_compile_options(freetype PRIVATE
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:Dist>:-O3>
)
