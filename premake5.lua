-- OUT_DIR and INT_DIR must be defined in the top-most premake file before including this file

project "PDFGen"
    language "C"
    kind "StaticLib"
    warnings "Off"

    debugdir (OUT_DIR)
    targetdir (OUT_DIR)
    objdir (INT_DIR)

    files
    {
        "pdfgen.h",
        "pdfgen.c"
    }

    includedirs
    {
        "."
    }

    filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        systemversion "latest"

    filter "system:macosx"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "Full"

    filter "configurations:Release"
        runtime "Release"
        symbols "Off"
        optimize "Full"
