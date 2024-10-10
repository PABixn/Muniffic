#pragma once
#include <string>
#include <vector>
#include "imgui.h"

class Markdown
{
	enum class BlockType
	{
		None,
		Heading1,
		Heading2,
		Heading3,
		Italic,
		Bold,
		OrderedListItem,
		UnorderedListItem,
		HorizontalRule
	};

public:
	Markdown() = default;

	void init(ImFont* regular_font, ImFont* bold_font);

	void text(const std::string& str);
	void text(const char* str);

private:
	void process_text(const std::string& str);

	void apply_block_style(std::string& line, BlockType block_type);
	void clear_block_style(BlockType block_type);

private:
	ImFont* regular_font = nullptr;
	ImFont* bold_font = nullptr;
};
