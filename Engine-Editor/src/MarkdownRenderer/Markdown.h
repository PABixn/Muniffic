#pragma once
#include <string>
#include <vector>
#include "imgui.h"

class Markdown
{
	enum class BlockType
	{
		None = 0,
		Heading1 = 1 << 0,
		Heading2 = 1 << 1,
		Heading3 = 1 << 2,
		Italic = 1 << 3,
		Bold = 1 << 4,
		OrderedListItem = 1 << 5,
		UnorderedListItem = 1 << 6,
		HorizontalRule = 1 << 7,
		BoldAndItalic = 1 << 8
	};

public:
	Markdown() = default;

	static void init(ImFont* regular_font, ImFont* bold_font, ImFont* italic_font, ImFont* bold_italic_font, ImFont* heading_1, ImFont* heading_2, ImFont* heading_3);

	static void text(const std::string& str, float text_max_width = 0.0f, float indent = 0.0f);
	static void text(const char* str, float text_max_width = 0.0f, float indent = 0.0f);

private:
	static void process_text(const std::string& str, float text_max_width, float indent);

	static void set_block_width(float width);

	static void render_heading(std::string& line, BlockType block_type);
	static void render_line(std::string& line, BlockType block_type, bool isFirstLine);

	static void render_wrapped_text(std::string& text, bool isFirstLine);

	static void check_block_style(std::string& line, bool isFirstLine, bool isLastCharacter = false);

	static void trim_block_style(std::string& line, BlockType block_type);

	static void add_block_style(std::string& line, BlockType block_type, bool isFirstLine);

	static void apply_block_style(std::string& line, BlockType block_type);
	static void clear_block_style(BlockType block_type);

private:
	static ImFont* regular_font;
	static ImFont* bold_font;
	static ImFont* italic_font;
	static ImFont* bold_italic_font;
	static ImFont* heading_1_font;
	static ImFont* heading_2_font;
	static ImFont* heading_3_font;
	static std::vector<BlockType> block_types;
	static int heading_level, list_level, star_level, dash_level;
	static float block_width;
};