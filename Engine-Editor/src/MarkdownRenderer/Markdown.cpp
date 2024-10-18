#include "Markdown.h"

#include <sstream>
#include <vector>
#include <unordered_map>

ImFont* Markdown::regular_font = nullptr;
ImFont* Markdown::bold_font = nullptr;
ImFont* Markdown::italic_font = nullptr;
ImFont* Markdown::bold_italic_font = nullptr;
ImFont* Markdown::heading_1_font = nullptr;
ImFont* Markdown::heading_2_font = nullptr;
ImFont* Markdown::heading_3_font = nullptr;
std::vector<Markdown::BlockType> Markdown::block_types;
int Markdown::heading_level = 0;
int Markdown::list_level = 0;
int Markdown::star_level = 0;
int Markdown::dash_level = 0;
float Markdown::block_width = 0.0f;

void Markdown::init(ImFont* regular, ImFont* bold, ImFont* italic, ImFont* bold_italic, ImFont* heading_1, ImFont* heading_2, ImFont* heading_3)
{
	regular_font = regular;
	bold_font = bold;
	italic_font = italic;
	bold_italic_font = bold_italic;
	heading_1_font = heading_1;
	heading_2_font = heading_2;
	heading_3_font = heading_3;
}

void Markdown::text(const std::string& str, float text_max_width, float indent)
{
	process_text(str.c_str(), text_max_width, indent);
}

void Markdown::text(const char* str, float text_max_width, float indent)
{
	process_text(str, text_max_width, indent);
}

void Markdown::process_text(const std::string& str, float text_max_width, float indend)
{
	set_block_width(text_max_width);

	if(indend != 0.0f)
		ImGui::Indent(indend);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	std::stringstream ss(str);
	std::string line = "", block = "", next_line = "";
	bool isFirstLine = true, isLastLine = false;

	while (std::getline(ss, line))
	{
		if (line.empty())
			continue;

		if(ss.peek() == EOF)
			isLastLine = true;

		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == '#' && heading_level < 4 && i == heading_level)
				heading_level++;
			else if (line[i] == '*' && star_level < 4)
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
					check_block_style(block, isFirstLine);
				}

				heading_level = 0;
				star_level = 0;
				dash_level = 0;
			}

			block += line[i];

			if (i == line.length() - 1)
			{
				check_block_style(block, isFirstLine, true);
				heading_level = 0;
				star_level = 0;
				dash_level = 0;
			}
		}

		block = "";
		ImGui::NewLine();
		if(!isLastLine)
			ImGui::NewLine();
		isFirstLine = false;
	}

	ImGui::PopStyleVar();

	if(indend != 0.0f)
		ImGui::Unindent(indend);
}

void Markdown::set_block_width(float width)
{
	if (width != 0.0f)
		block_width = width;
	else
		block_width = ImGui::GetContentRegionAvail().x;
}

void Markdown::check_block_style(std::string& line, bool isFirstLine, bool isLastCharacter)
{
	if(line.empty())
		return;

	if (star_level > 0)
	{
		if (star_level == 1)
			add_block_style(line, BlockType::Italic, isFirstLine);
		else if (star_level == 2)
			add_block_style(line, BlockType::Bold, isFirstLine);
		else if (star_level == 3)
			add_block_style(line, BlockType::BoldAndItalic, isFirstLine);
	}
	else if (dash_level > 0)
	{
		if (dash_level == 1)
			add_block_style(line, BlockType::UnorderedListItem, isFirstLine);
		else if (dash_level == 3)
			add_block_style(line, BlockType::HorizontalRule, isFirstLine);
	}
	else if (isLastCharacter)
		render_line(line, BlockType::None, isFirstLine);
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
		case BlockType::BoldAndItalic:
			from = "***";
		break;
	}

	if (from.empty())
		return;

	start_pos = line.find(from);
	if (start_pos != std::string::npos)
		line.erase(start_pos, from.length());
}

void Markdown::render_line(std::string& line, BlockType block_type, bool isFirstLine)
{
	if(line.empty())
		return;

	apply_block_style(line, block_type);
	render_wrapped_text(line, isFirstLine);
	ImGui::SameLine();
	clear_block_style(block_type);
	line = "";
}

void Markdown::render_wrapped_text(std::string& line, bool isFirstLine)
{
	std::stringstream ss(line);

	std::string word = "";

	while (ss >> word)
	{
		if (ImGui::CalcTextSize(word.c_str()).x + ImGui::GetCursorPosX() >= block_width)
		{
			if(!isFirstLine)
				ImGui::NewLine();
			else
				isFirstLine = false;
		}

		ImGui::Text((word + " ").c_str());
		ImGui::SameLine();
		isFirstLine = false;
	}
}

void Markdown::add_block_style(std::string& line, BlockType block_type, bool isFirstLine)
{
	trim_block_style(line, block_type);

	auto it = std::find(block_types.begin(), block_types.end(), block_type);

	if (it != block_types.end())
	{
		it = block_types.erase(it);
		if(!line.empty())
			render_line(line, block_type, isFirstLine);
	}
	else
	{
		block_types.push_back(block_type);
		if(!line.empty())
			render_line(line, BlockType::None, isFirstLine);
	}
}

void Markdown::render_heading(std::string& line, BlockType block_type)
{
	trim_block_style(line, block_type);

	apply_block_style(line, block_type);
	render_wrapped_text(line, true);
	clear_block_style(block_type);
}

void Markdown::apply_block_style(std::string& line, BlockType block_type)
{
	switch (block_type)
	{
		case BlockType::Heading1:
			ImGui::PushFont(heading_1_font);
		break;
		case BlockType::Heading2:
			ImGui::PushFont(heading_2_font);
		break;
		case BlockType::Heading3:
			ImGui::PushFont(heading_3_font);
		break;
		case BlockType::Italic:
			ImGui::PushFont(italic_font);
		break;
		case BlockType::Bold:
			ImGui::PushFont(bold_font);
		break;
		case BlockType::BoldAndItalic:
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
		case BlockType::BoldAndItalic:
		case BlockType::None:
			ImGui::PopFont();
			ImGui::SetWindowFontScale(1.0f);
			break;
	}
}