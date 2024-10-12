#include "Markdown.h"

#include <sstream>
#include <vector>
#include <unordered_map>

ImFont* Markdown::regular_font = nullptr;
ImFont* Markdown::bold_font = nullptr;
ImFont* Markdown::italic_font = nullptr;
ImFont* Markdown::bold_italic_font = nullptr;
std::vector<Markdown::BlockType> Markdown::block_types;
int Markdown::heading_level = 0;
int Markdown::list_level = 0;
int Markdown::star_level = 0;
int Markdown::dash_level = 0;

void Markdown::init(ImFont* regular, ImFont* bold, ImFont* italic, ImFont* bold_italic)
{
	regular_font = regular;
	bold_font = bold;
	italic_font = italic;
	bold_italic_font = bold_italic;
}

void Markdown::text(const std::string& str, float indent)
{
	process_text(str.c_str(), indent);
}

void Markdown::text(const char* str, float indent)
{
	process_text(str, indent);
}

void Markdown::process_text(const std::string& str, float indend)
{
	ImGui::Indent(indend);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	std::stringstream ss(str);
	std::string line = "", block = "";

	while (std::getline(ss, line))
	{
		if (line.empty())
			continue;

		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == '#' && heading_level < 4)
				heading_level++;
			else if (line[i] == '*' && star_level < 3)
				star_level++;
			else if (line[i] == '-' && dash_level < 4)
				dash_level++;
			else
			{
				if(heading_level > 0)
				{ 
					if(heading_level == 1)
						render_heading(line, BlockType::Heading1);
					else if(heading_level == 2)
						render_heading(line, BlockType::Heading2);
					else if(heading_level == 3)
						render_heading(line, BlockType::Heading3);

					heading_level = 0;
					star_level = 0;
					dash_level = 0;
					block = "";
					break;
				}
				else
				{
					check_block_style(block);
				}

				heading_level = 0;
				star_level = 0;
				dash_level = 0;
			}

			block += line[i];

			if (i == line.length() - 1)
			{
				check_block_style(block, true);
			}
		}

		block = "";
		ImGui::NewLine();
	}

	ImGui::PopStyleVar();
	ImGui::Unindent(indend);
}

bool Markdown::check_if_mixed(BlockType block_type)
{
	switch (block_type)
	{
		case BlockType::Italic:
			return std::find(block_types.begin(), block_types.end(), BlockType::Bold) != block_types.end();
		break;
		case BlockType::Bold:
			return std::find(block_types.begin(), block_types.end(), BlockType::Italic) != block_types.end();
		break;
	}

	return false;
}

void Markdown::check_block_style(std::string& line, bool isLastCharacter)
{
	if(line.empty())
		return;

	if (star_level > 0)
	{
		if (star_level == 1)
			add_block_style(line, BlockType::Italic);
		else if (star_level == 2)
			add_block_style(line, BlockType::Bold);
	}
	else if (dash_level > 0)
	{
		if (dash_level == 1)
			add_block_style(line, BlockType::UnorderedListItem);
		else if (dash_level == 3)
			add_block_style(line, BlockType::HorizontalRule);
	}
	else if (isLastCharacter)
		render_line(line, BlockType::None);
}

void Markdown::trim_block_style(std::string& line, BlockType block_type)
{
	size_t start_pos = 0;
	std::string from = "";

	switch (block_type)
	{
		case BlockType::Heading1:
			from = "#";
		break;
		case BlockType::Heading2:
			from = "##";
		break;
		case BlockType::Heading3:
			from = "###";
		break;
		case BlockType::Italic:
			from = "*";
		break;
		case BlockType::Bold:
			from = "**";
		break;
	}

	if (from.empty())
		return;

	start_pos = line.find(from);
	if (start_pos != std::string::npos)
		line.erase(start_pos, from.length());
}

void Markdown::render_line(std::string& line, BlockType block_type)
{
	if(check_if_mixed(block_type))
		block_type = BlockType::Mixed;
	apply_block_style(line, block_type);
	ImGui::Text(line.c_str());
	ImGui::SameLine();
	clear_block_style(block_type);
	line = "";
}

void Markdown::add_block_style(std::string& line, BlockType block_type)
{
	trim_block_style(line, block_type);

	auto it = std::find(block_types.begin(), block_types.end(), block_type);

	if (it != block_types.end())
	{
		it = block_types.erase(it);
		if(!line.empty())
			render_line(line, block_type);
	}
	else
	{
		block_types.push_back(block_type);
		if(!line.empty())
			render_line(line, BlockType::None);
	}
}

void Markdown::render_heading(std::string& line, BlockType block_type)
{
	trim_block_style(line, block_type);

	apply_block_style(line, block_type);
	ImGui::Text(line.c_str());
	clear_block_style(block_type);
}

void Markdown::apply_block_style(std::string& line, BlockType block_type)
{
	switch (block_type)
	{
		case BlockType::Heading1:
			ImGui::PushFont(bold_font);
			ImGui::SetWindowFontScale(2.0f);
		break;
		case BlockType::Heading2:
			ImGui::PushFont(bold_font);
			ImGui::SetWindowFontScale(1.75f);
		break;
		case BlockType::Heading3:
			ImGui::PushFont(bold_font);
			ImGui::SetWindowFontScale(1.5f);
		break;
		case BlockType::Italic:
			ImGui::PushFont(italic_font);
		break;
		case BlockType::Bold:
			ImGui::PushFont(bold_font);
		break;
		case BlockType::Mixed:
			ImGui::PushFont(bold_italic_font);
		break;
		case BlockType::None:
			ImGui::PushFont(regular_font);
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
		case BlockType::Bold:
		case BlockType::Italic:
		case BlockType::Mixed:
		case BlockType::None:
			ImGui::PopFont();
			ImGui::SetWindowFontScale(1.0f);
			break;
	}
}