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
		HorizontalRule = 1 << 7
	};

public:
	Markdown() = default;

	void init(ImFont* regular_font, ImFont* bold_font);

	void text(const std::string& str);
	void text(const char* str);

private:
	void process_text(const std::string& str);

	void render_heading(std::string& line, BlockType block_type);
	void render_line(std::string& line, BlockType block_type);

	void check_block_style(std::string& line, bool isLastCharacter = false);

	void trim_block_style(std::string& line, BlockType block_type);

	void add_block_style(std::string& line, BlockType block_type);

	void apply_block_style(std::string& line, BlockType block_type);
	void clear_block_style(BlockType block_type);

private:
	ImFont* regular_font = nullptr;
	ImFont* bold_font = nullptr;
	std::vector<BlockType> block_types;
	int heading_level = 0, list_level = 0, star_level = 0, dash_level = 0;
};
