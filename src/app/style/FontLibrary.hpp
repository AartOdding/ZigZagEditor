#pragma once

#include <vector>

#include <imgui.h>



class FontLibrary
{
public:

	FontLibrary() = default;
	~FontLibrary();

	FontLibrary(const FontLibrary&) = delete;
	FontLibrary(FontLibrary&&) = delete;

	// Important do not call during an ImGui frame, always call in between frames.
	void updateFonts();

	ImFont* getUiFont() const;
	ImFont* getCodeFont() const;

	int getFontSize() const;
	void setFontSize(int fontSize);

	int getCodeSize() const;
	void setCodeSize(int codeSize);

	// The framebuffer scale argument should be used when the scale between the window size and
	// the framebuffer size is not 1. This is for instance the case on mac retina displays.
	void setScaling(float desiredScale, float frameBufferScale = 1.0f);
	float getScaling() const;

private:

	void loadFonts();

	ImFont* m_uiFont = nullptr;
	ImFont* m_codeFont = nullptr;

	bool m_fontsLoaded = false;
	bool m_fontsOutdated = true;

	int m_fontSize = 18;
	int m_codeSize = 22;
	float m_desiredScaling = 1.0f;
	float m_frameBufferScaling = 1.0f;
	float m_imguiScaling = 1.0f;

	std::vector<unsigned char> m_openSans;
	std::vector<unsigned char> m_sourceCodePro;

};
