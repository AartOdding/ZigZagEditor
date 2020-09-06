#include <app/style/FontLibrary.hpp>
#include <app/Directories.hpp>

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include <imgui_impl_opengl3.h>
#include <imgui_freetype.h>


// https://stackoverflow.com/questions/24716250/c-store-read-binary-file-into-buffer
static std::vector<unsigned char> read_binary_file(const std::string filename)
{
    // binary mode is only for switching off newline translation
    std::ifstream file(filename, std::ios::binary);
    file.unsetf(std::ios::skipws);

    std::streampos file_size;
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> vec(file_size);
    vec.insert(vec.begin(),
        std::istream_iterator<unsigned char>(file),
        std::istream_iterator<unsigned char>());
    return (vec);
}


FontLibrary::~FontLibrary()
{

}

void FontLibrary::updateFonts()
{
    if (!m_fontsLoaded)
    {
        loadFonts();
        m_fontsLoaded = true;
    }
    if (m_fontsOutdated)
    {
        m_imguiScaling = m_desiredScaling / m_frameBufferScaling;
        ImGui::GetIO().Fonts->Clear();
        ImGui_ImplOpenGL3_DestroyFontsTexture();
        ImFontConfig fontConfig;
        fontConfig.FontDataOwnedByAtlas = false;
        m_uiFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(m_openSans.data(), m_openSans.size(), m_fontSize * m_frameBufferScaling * m_imguiScaling, &fontConfig);
        m_codeFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(m_sourceCodePro.data(), m_sourceCodePro.size(), m_codeSize * m_frameBufferScaling * m_imguiScaling, &fontConfig);
        ImGui::GetIO().FontGlobalScale = 1.0f / m_frameBufferScaling;
        ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts);
        ImGui_ImplOpenGL3_CreateFontsTexture();
        ImGui::GetStyle() = ImGuiStyle();
        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(m_imguiScaling);

        m_fontsOutdated = false;
    }
}

ImFont* FontLibrary::getUiFont() const
{
    return m_uiFont;
}

ImFont* FontLibrary::getCodeFont() const
{
    return m_codeFont;
}

int FontLibrary::getFontSize() const
{
    return m_fontSize;
}

int FontLibrary::getCodeSize() const
{
    return m_codeSize;
}

float FontLibrary::getScaling() const
{
    return m_imguiScaling;
}

void FontLibrary::setFontSize(int fontSize)
{
    int clampedFontSize = std::clamp<int>(fontSize, 6, 52);
    m_fontsOutdated |= (m_fontSize != clampedFontSize);
    m_fontSize = clampedFontSize;
}

void FontLibrary::setCodeSize(int codeSize)
{
    int clampedCodeSize = std::clamp<int>(codeSize, 6, 52);
    m_fontsOutdated |= (m_codeSize != clampedCodeSize);
    m_codeSize = clampedCodeSize;
}

void FontLibrary::setScaling(float desiredScale, float frameBufferScale)
{
    float clampedDesiredScaling = std::clamp<float>(desiredScale, 0.25f, 4.0f);
    float clampedFrameBufferScaling = std::clamp<float>(frameBufferScale, 0.25f, 4.0f);
    m_fontsOutdated |= (m_desiredScaling != clampedDesiredScaling || m_frameBufferScaling != clampedFrameBufferScaling);
    m_desiredScaling = clampedDesiredScaling;
    m_frameBufferScaling = clampedFrameBufferScaling;
}

void FontLibrary::loadFonts()
{
    m_openSans = read_binary_file((Directories::resourcesDir() / "Open_Sans" / "OpenSans-Regular.ttf").string());
    m_sourceCodePro = read_binary_file((Directories::resourcesDir() / "Source_Code_Pro" / "SourceCodePro-Regular.ttf").string());
}
