#include "Markdown.h"

#include <sstream>
#include <vector>
#include <unordered_map>

void Markdown::init(ImFont* regular_font, ImFont* bold_font)
{
	this->regular_font = regular_font;
	this->bold_font = bold_font;
}

void Markdown::text(const std::string& str)
{
	process_text(str.c_str());
}

void Markdown::text(const char* str)
{
	process_text(str);
}

void Markdown::process_text(const std::string& str)
{
	int heading_level = 0, list_level = 0, star_level = 0, dash_level = 0;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	std::stringstream ss(str);
	std::string line = "";
	BlockType block_type = BlockType::None;
	std::unordered_map<size_t, BlockType> block_type_map;


	while (std::getline(ss, line))
	{
		if (line.empty())
			continue;

		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == '#')
				heading_level++;
			else if (line[i] == '*')
				star_level++;
			else if (line[i] == '-')
				dash_level++;
			else
			{
				if(heading_level > 0)
				{ 
					if(heading_level == 1)
						block_type = BlockType::Heading1;
					else if(heading_level == 2)
						block_type = BlockType::Heading2;
					else if(heading_level == 3)
						block_type = BlockType::Heading3;
				}
				else if (star_level > 0)
				{
					if(star_level == 1)
						block_type = BlockType::Italic;
					else if(star_level == 2)
						block_type = BlockType::Bold;
				}
				else if (dash_level > 0)
				{
					if(dash_level == 1)
						block_type = BlockType::UnorderedListItem;
					else if(dash_level == 3)
						block_type = BlockType::HorizontalRule;
				}

				heading_level = 0;
				star_level = 0;
				dash_level = 0;
			}
		}
	}

	ImGui::PopStyleVar();
}

void Markdown::apply_block_style(std::string& line, BlockType block_type)
{
	switch (block_type)
	{
		case BlockType::Heading1:
			ImGui::PushFont(bold_font);
			line = line.substr(1);
			break;
		case BlockType::Heading2:  
			ImGui::PushFont(bold_font);
			ImGui::SetWindowFontScale(0.75f);
			line = line.substr(2);
			break;
		case BlockType::Heading3:
			ImGui::PushFont(bold_font);
			ImGui::SetWindowFontScale(0.5f);
			line = line.substr(3);
			break;

	}
}

void Markdown::clear_block_style(BlockType block_type)
{
	switch (block_type)
	{
		case BlockType::Heading1:
		case BlockType::Heading2:
		case BlockType::Heading3:
			ImGui::PopFont();
			ImGui::SetWindowFontScale(1.0f);
			break;
	}
}