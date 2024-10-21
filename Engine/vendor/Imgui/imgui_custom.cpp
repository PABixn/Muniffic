
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui.h"
#ifndef IMGUI_DISABLE
#include "imgui_internal.h"
#include <iostream>
#include <sstream>
//tree nodes for hierarchy panel's entities
//drawing icons as images

//helper

static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.ColorEditCurrentID != 0);
    if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
        return;

    // When S == 0, H is undefined.
    // When H == 1 it wraps around to 0.
    if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
        *H = g.ColorEditSavedHue;

    // When V == 0, S is undefined.
    if (*V == 0.0f)
        *S = g.ColorEditSavedSat;
}


bool ImGui::CustomTreeNodeWithPicBehavior(ImTextureID textureID, ImGuiID id, ImGuiTreeNodeFlags flags, const char* labelArg, const char* label_endArg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    const char* label;
    char* mutable_str = nullptr;
    const char* label_end;
    float labelMaxWidth = GetWindowWidth() - 30 - (window->DC.TreeDepth * 20) - 70;
    //first call of editor rendering gives silly numbers
    if (labelMaxWidth > 100)
    {
        //if label is too long, we shorten it and give '...' at the end
        if (CalcTextSize(labelArg).x >= labelMaxWidth)
        {
            int n = IndexOfCharWhereTheTextShouldBeCutToFit(labelArg, label_endArg, labelMaxWidth);
            mutable_str = strdup(labelArg);

            if (mutable_str != nullptr && n >= 0 && n < strlen(mutable_str)) {
                mutable_str[n] = '\0';
                mutable_str[n - 1] = '.';
                mutable_str[n - 2] = '.';
                mutable_str[n - 3] = '.';
                //free(&mutable_str[n + 1]);
                label_end = &mutable_str[n];
                label = mutable_str;
            }
            else
            {
                label = labelArg;
                label_end = label_endArg;
            }

        }
        else
        {
            label = labelArg;
            label_end = label_endArg;
        }
    }
    else
    {
        label = labelArg;
        label_end = label_endArg;
    }

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    const ImVec2 padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));

    if (!label_end)
        label_end = FindRenderedTextEnd(label);
    const ImVec2 label_size = CalcTextSize(label, label_end, false);

    // We vertically grow up to current line height up the typical widget height.
    const float frame_height = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y * 2), label_size.y + padding.y * 2);
    ImRect frame_bb;
    frame_bb.Min.x = (flags & ImGuiTreeNodeFlags_SpanFullWidth) ? window->WorkRect.Min.x : window->DC.CursorPos.x;
    frame_bb.Min.y = window->DC.CursorPos.y;
    frame_bb.Max.x = window->WorkRect.Max.x;
    frame_bb.Max.y = window->DC.CursorPos.y + frame_height;
    if (display_frame)
    {
        // Framed header expand a little outside the default padding, to the edge of InnerClipRect
        // (FIXME: May remove this at some point and make InnerClipRect align with WindowPadding.x instead of WindowPadding.x*0.5f)
        frame_bb.Min.x -= IM_FLOOR(window->WindowPadding.x * 0.5f - 1.0f);
        frame_bb.Max.x += IM_FLOOR(window->WindowPadding.x * 0.5f);
    }

    const float text_offset_x = g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2);           // Collapser arrow width + Spacing
    const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
    const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
    ImVec2 text_pos;
    if (flags & ImGuiTreeNodeFlags_PropertiesComponent)
    {
        text_pos = ImVec2(window->DC.CursorPos.x + text_offset_x + 10, window->DC.CursorPos.y + text_offset_y);
    }
    else text_pos = ImVec2(window->DC.CursorPos.x + text_offset_x + 20, window->DC.CursorPos.y + text_offset_y);
    ItemSize(ImVec2(text_width, frame_height), padding.y);

    // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
    ImRect interact_bb = frame_bb;
    if (!display_frame && (flags & (ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth)) == 0)
        interact_bb.Max.x = frame_bb.Min.x + text_width + style.ItemSpacing.x * 2.0f;

    // Store a flag for the current depth to tell if we will allow closing this node when navigating one of its child.
    // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
    // This is currently only support 32 level deep and we are fine with (1 << Depth) overflowing into a zero.
    const bool is_leaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
    bool is_open = TreeNodeUpdateNextOpen(id, flags);

    // If a NavLeft request is happening and ImGuiTreeNodeFlags_NavLeftJumpsBackHere enabled:
    // Store data for the current depth to allow returning to this node from any child item.
    // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
    // It will become tempting to enable ImGuiTreeNodeFlags_NavLeftJumpsBackHere by default or move it to ImGuiStyle.
    bool store_tree_node_stack_data = false;
    if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
    {
        if ((flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) && is_open && !g.NavIdIsAlive)
            if (g.NavMoveDir == ImGuiDir_Left && g.NavWindow == window && NavMoveRequestButNoResultYet())
                store_tree_node_stack_data = true;
    }

    bool item_add = ItemAdd(interact_bb, id);
    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
    g.LastItemData.DisplayRect = frame_bb;

    if (!item_add)
    {
        if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
            TreePushOverrideID(id);
        IMGUI_TEST_ENGINE_ITEM_INFO(g.LastItemData.ID, label, g.LastItemData.StatusFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
        return is_open;
    }

    ImGuiButtonFlags button_flags = ImGuiTreeNodeFlags_None;
    if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
        button_flags |= ImGuiButtonFlags_AllowOverlap;
    if (!is_leaf)
        button_flags |= ImGuiButtonFlags_PressedOnDragDropHold;

    // We allow clicking on the arrow section with keyboard modifiers held, in order to easily
    // allow browsing a tree while preserving selection with code implementing multi-selection patterns.
    // When clicking on the rest of the tree node we always disallow keyboard modifiers.
    const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
    const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
    const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);
    if (window != g.HoveredWindow || !is_mouse_x_over_arrow)
        button_flags |= ImGuiButtonFlags_NoKeyModifiers;

    // Open behaviors can be altered with the _OpenOnArrow and _OnOnDoubleClick flags.
    // Some alteration have subtle effects (e.g. toggle on MouseUp vs MouseDown events) due to requirements for multi-selection and drag and drop support.
    // - Single-click on label = Toggle on MouseUp (default, when _OpenOnArrow=0)
    // - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=0)
    // - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=1)
    // - Double-click on label = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1)
    // - Double-click on arrow = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1 and _OpenOnArrow=0)
    // It is rather standard that arrow click react on Down rather than Up.
    // We set ImGuiButtonFlags_PressedOnClickRelease on OpenOnDoubleClick because we want the item to be active on the initial MouseDown in order for drag and drop to work.
    if (is_mouse_x_over_arrow)
        button_flags |= ImGuiButtonFlags_PressedOnClick;
    else if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
        button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    else
        button_flags |= ImGuiButtonFlags_PressedOnClickRelease;

    bool selected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
    const bool was_selected = selected;

    bool hovered, held;
    bool pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    bool toggled = false;
    if (!is_leaf)
    {
        if (pressed && g.DragDropHoldJustPressedId != id)
        {
            if ((flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) == 0 || (g.NavActivateId == id))
                toggled = true;
            if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
                toggled |= is_mouse_x_over_arrow && !g.NavDisableMouseHover; // Lightweight equivalent of IsMouseHoveringRect() since ButtonBehavior() already did the job
            if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseClickedCount[0] == 2)
                toggled = true;
        }
        else if (pressed && g.DragDropHoldJustPressedId == id)
        {
            IM_ASSERT(button_flags & ImGuiButtonFlags_PressedOnDragDropHold);
            if (!is_open) // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but never close it again.
                toggled = true;
        }

        if (g.NavId == id && g.NavMoveDir == ImGuiDir_Left && is_open)
        {
            toggled = true;
            NavMoveRequestCancel();
        }
        if (g.NavId == id && g.NavMoveDir == ImGuiDir_Right && !is_open) // If there's something upcoming on the line we may want to give it the priority?
        {
            toggled = true;
            NavMoveRequestCancel();
        }

        if (toggled)
        {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(id, is_open);
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
        }
    }
    if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
        SetItemAllowOverlap();
    // Render
    const ImU32 text_col = GetColorU32(ImGuiCol_Text);
    //ImGuiNavHighlightFlags nav_highlight_flags = ImGuiNavHighlightFlags_TypeThin;
    if (display_frame)
    {
        // Framed type
        const ImU32 bg_col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, true, style.FrameRounding);
        RenderNavHighlight(frame_bb, id);
    }
    if (hovered || selected)
    {
        const ImU32 bg_col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, false);
    }
    RenderNavHighlight(frame_bb, id);
    //drawing hierarchy lines
    if (flags & ImGuiTreeNodeFlags_Entity)
    {
        static int childnumber;
        static int lastTreeDepth;
        static int lastFrame;
        if (!lastFrame) {
            lastFrame = ImGui::GetFrameCount();
            lastTreeDepth = 0;
        }
        if (ImGui::GetFrameCount() != lastFrame) {
            childnumber = 0;
            lastFrame = ImGui::GetFrameCount();
            lastTreeDepth = 0;
        }
        if (window->DC.TreeDepth > 0) {
            if (lastTreeDepth > window->DC.TreeDepth) {
                window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - (19 * (lastTreeDepth + 1)) - (2 * (lastTreeDepth - 1)), text_pos.y + g.FontSize * 0.5f - 40));
                window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - (19 * (lastTreeDepth + 1)) - (2 * (lastTreeDepth - 1)), text_pos.y + g.FontSize * 0.5f));
                window->DrawList->PathStroke(text_col, false, 2.0f);
                lastTreeDepth = window->DC.TreeDepth;
                childnumber = 0;
            }
            if (lastTreeDepth != window->DC.TreeDepth) {
                lastTreeDepth = window->DC.TreeDepth;
                childnumber = 0;
            }
            else
            {
                childnumber += 1;
                window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - 17, text_pos.y + g.FontSize * 0.5f - 40));
                window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - 17, text_pos.y + g.FontSize * 0.5f - 10));
                window->DrawList->PathStroke(text_col, false, 2.0f);
            }
            window->DrawList->AddCircleFilled(ImVec2(text_pos.x - text_offset_x - 17, text_pos.y + g.FontSize * 0.5f), 3, text_col);
            window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - 17, text_pos.y + g.FontSize * 0.5f));
            window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - 11, text_pos.y + g.FontSize * 0.5f));
            window->DrawList->PathStroke(text_col, false, 2.0f);
            window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - 17, text_pos.y + g.FontSize * 0.5f - 10));
            window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - 17, text_pos.y + g.FontSize * 0.5f));
            window->DrawList->PathStroke(text_col, false, 2.0f);
            for (int i = 1; i < window->DC.TreeDepth; i++) {
                window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - (19 * (i + 1)) - (2 * (i - 1)), text_pos.y + g.FontSize * 0.5f - 40));
                window->DrawList->PathLineTo(ImVec2(text_pos.x - text_offset_x - (19 * (i + 1)) - (2 * (i - 1)), text_pos.y + g.FontSize * 0.5f));
                window->DrawList->PathStroke(text_col, false, 2.0f);
            }
        }
        else
        {
            lastTreeDepth = 0;
        }

    }
    //drawing icons
    ImRect bb;
    if (flags & ImGuiTreeNodeFlags_PropertiesComponent)
        bb = ImRect(window->DC.CursorPos + ImVec2(13, -25), window->DC.CursorPos + ImVec2(28, -10));
    else bb = ImRect(window->DC.CursorPos + ImVec2(15, -27), window->DC.CursorPos + ImVec2(35, -7));
    window->DrawList->AddImage(textureID, bb.Min, bb.Max);

    //TODO: drawing an eye

    if (g.LogEnabled)
        LogSetNextTextDecoration(">", NULL);
    RenderText(text_pos, label, label_end, false);
    if (mutable_str != nullptr)
    {
        free(mutable_str);
    }
    //drawing an arrow if the entity has children
    if (flags & ImGuiTreeNodeFlags_EntityWithChildren) {
        RenderArrow(window->DrawList, ImVec2(window->WorkRect.Max.x - 30, text_pos.y), text_col, is_open ? ImGuiDir_Down : ImGuiDir_Left, 1.0f);
    }
    //zna3
    //TODO: changing to a nice icon and adding a info when hovered over the image
    if (flags & ImGuiTreeNodeFlags_CopyingToChildren)
    {
        ImVec2 xyOfStart(frame_bb.Max - ImVec2(2 * frame_height, frame_height) + ImVec2(-7.f + (frame_height / 4), (frame_height / 8)));
        ImVec2 xyOfEnd(frame_bb.Max - ImVec2(frame_height, 0.f) + ImVec2(-7.f, -(frame_height / 8)));
        window->DrawList->AddImage(textureID, xyOfStart, ImVec2(xyOfEnd.x - (.25f * frame_height), xyOfEnd.y - (.25f * frame_height)));
        window->DrawList->AddImage(textureID, ImVec2(xyOfEnd.x - (.5f * frame_height), xyOfEnd.y - (.5f * frame_height)), xyOfEnd);
        if (hovered)
        {
            if (GetMousePos().x > xyOfStart.x && GetMousePos().x < xyOfEnd.x) {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(150.f);
                ImGui::TextWrapped("This component is inherited in children");
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }
        }

    }

    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        TreePushOverrideID(id);
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
    return is_open;
}

int ImGui::IndexOfCharWhereTheTextShouldBeCutToFit(const char* text, const char* textend, float size) {
    if (CalcTextSize(text, textend).x <= size) {
        return static_cast<int>(textend - text);
    }
    else {
        const char* start = text;
        const char* end = textend;

        while (start < end) {
            const char* mid = start + (end - start) / 2;
            if (CalcTextSize(text, mid).x <= size)
                start = mid + 1;
            else
                end = mid;
        }

        return static_cast<int>(start - text) - 1;
    }
}

void ImGui::HierarchyPanelTreePushOverrideID(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    Indent();
    window->DC.TreeDepth++;
    PushOverrideID(id);
}

bool ImGui::TextWithLineLimitV(const char* fmt, const int& lineLimit, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const char* text, * text_end;
    ImFormatStringToTempBufferV(&text, &text_end, fmt, args);
    return TextWithLineLimitEx(text, lineLimit, text_end, ImGuiTextFlags_NoWidthForLargeClippedText);
}

bool ImGui::TextWithLineLimitEx(const char* text, const int& lineLimit, const char* text_end_Arg, ImGuiTextFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    bool neededToBeShortened = false;
    ImGuiContext& g = *GImGui;
    char* text_end = (char*)text_end_Arg;
    // Accept null ranges
    if (text == text_end)
        text = text_end = "";

    // Calculate length
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = (char*)text + strlen(text); // FIXME-OPT

    ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;
    const bool wrap_enabled = (wrap_pos_x >= 0.0f);
    if (text_end - text <= 2000 || wrap_enabled)
    {
        // Common case
        const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
        ImVec2 text_size = CalcTextSize(text_begin, text_end, false, wrap_width);

        while (text_size.y > (lineLimit * g.FontSize))
        {
            neededToBeShortened = true;
            text_end[-1] = '\0';
            text_end[-2] = '-';
            text_end = &text_end[-1];
            text_size = CalcTextSize(text_begin, text_end, false, wrap_width);
        }
        //text_pos = ImVec2(text_pos.x+(ImGui::GetContentRegionAvail().x - text_size.x), text_pos.y);
        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size, 0.0f);
        if (!ItemAdd(bb, 0))
            return false;

        // Render (we don't hide text after ## in this end-user function)
        RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
        return neededToBeShortened;
    }
    else
    {
        // Long text!
        // Perform manual coarse clipping to optimize for long multi-line text
        // - From this point we will only compute the width of lines that are visible. Optimization only available when word-wrapping is disabled.
        // - We also don't vertically center the text within the line full height, which is unlikely to matter because we are likely the biggest and only item on the line.
        // - We use memchr(), pay attention that well optimized versions of those str/mem functions are much faster than a casually written loop.
        const char* line = text;
        const float line_height = GetTextLineHeight();
        ImVec2 text_size(0, 0);

        // Lines to skip (can't skip when logging text)
        ImVec2 pos = text_pos;
        if (!g.LogEnabled)
        {
            int lines_skippable = (int)((window->ClipRect.Min.y - text_pos.y) / line_height);
            if (lines_skippable > 0)
            {
                int lines_skipped = 0;
                while (line < text_end && lines_skipped < lines_skippable)
                {
                    const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                    if (!line_end)
                        line_end = text_end;
                    if ((flags & ImGuiTextFlags_NoWidthForLargeClippedText) == 0)
                        text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                    line = line_end + 1;
                    lines_skipped++;
                }
                pos.y += lines_skipped * line_height;
            }
            return false;
        }

        // Lines to render
        if (line < text_end)
        {
            ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
            while (line < text_end)
            {
                if (IsClippedEx(line_rect, 0))
                    break;

                const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                if (!line_end)
                    line_end = text_end;
                text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                RenderText(pos, line, line_end, false);
                line = line_end + 1;
                line_rect.Min.y += line_height;
                line_rect.Max.y += line_height;
                pos.y += line_height;
            }

            // Count remaining lines
            int lines_skipped = 0;
            while (line < text_end)
            {
                const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                if (!line_end)
                    line_end = text_end;
                if ((flags & ImGuiTextFlags_NoWidthForLargeClippedText) == 0)
                    text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                line = line_end + 1;
                lines_skipped++;
            }
            pos.y += lines_skipped * line_height;
        }
        text_size.y = (pos - text_pos).y;

        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size, 0.0f);
        ItemAdd(bb, 0);
        return false;
    }
}

bool ImGui::TextWithLineLimitAndToolTipV(const char* fmt, const int& lineLimit, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const char* text, * text_end;
    ImFormatStringToTempBufferV(&text, &text_end, fmt, args);
    if (TextWithLineLimitEx(text, lineLimit, text_end, ImGuiTextFlags_NoWidthForLargeClippedText))
    {
        if (IsItemHovered()) {
            BeginTooltip();
            TextWrappedV(text, args);
            EndTooltip();
        }
        return true;
    }
    return false;
}

bool ImGui::StylisedButton(const char* label, const ImVec2& size_arg, ImDrawFlags drawFlags)
{
    return StylisedButtonEx(label, size_arg, ImGuiButtonFlags_None, drawFlags);
}

bool ImGui::StylisedButtonEx(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags, ImDrawFlags drawFlags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavHighlight(bb, id);
    RenderFrameRounded(bb.Min, bb.Max, col, drawFlags, true, style.FrameRounding);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImGui::DragFloatRounded(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags, ImDrawFlags drawFlags)
{
    return StylisedDragScalar(label, ImGuiDataType_Float, v, v_speed, &v_min, &v_max, format, flags, drawFlags);
}

bool ImGui::CustomTreeNodeWithPicEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ImTextureID textureID, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = ImGui::CustomTreeNodeWithPicExV(ptr_id, flags, fmt, textureID, args);
    va_end(args);
    return is_open;
}

bool ImGui::CustomTreeNodeWithPicExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ImTextureID textureID, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const char* label, * label_end;
    ImFormatStringToTempBufferV(&label, &label_end, fmt, args);
    return CustomTreeNodeWithPicBehavior(textureID, window->GetID(ptr_id), flags, label, label_end);
}

bool ImGui::CustomTreeNodeEx(const void* ptr_id, ImTextureID id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    const char* label, * label_end;
    ImFormatStringToTempBufferV(&label, &label_end, fmt, nullptr);
    return CustomTreeNodeWithPicBehavior(id, window->GetID(ptr_id), flags, label, label_end);
}

void ImGui::RenderFrameRounded(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, ImDrawFlags flags, bool border, float rounding)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding, flags);
    const float border_size = g.Style.FrameBorderSize;
    if (border && border_size > 0.0f)
    {
        window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), rounding, flags, border_size);
        window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, flags, border_size);
    }
}

bool ImGui::TextWrappedWithLineLimit(const char* fmt, const int& line, ...)
{
    va_list args;
    va_start(args, fmt);
    bool res = TextWrappedWithLineLimitV(fmt, line, args);
    va_end(args);
    return res;
}

bool ImGui::TextWrappedWithLineLimitV(const char* fmt, const int& lineLimit, va_list args)
{
    ImGuiContext& g = *GImGui;
    const bool need_backup = (g.CurrentWindow->DC.TextWrapPos < 0.0f);  // Keep existing wrap position if one is already set
    if (need_backup)
        PushTextWrapPos(0.0f);
    bool res = TextWithLineLimitV(fmt, lineLimit, args);
    if (need_backup)
        PopTextWrapPos();
    return res;
};

bool ImGui::StylisedDragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, ImDrawFlags drawFlags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();

    bool drawArrows = !(flags & ImGuiSliderFlags_NoArrows);
    const float ArrowWidth = 13.f;

    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    if (ImGui::CalcTextSize(value_buf, value_buf_end).x > (w - 2 * (ArrowWidth)-6.f))
        drawArrows = false;

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    if (drawArrows)
        frame_bb = ImRect(frame_bb.Min + ImVec2(ArrowWidth, 0.f), frame_bb.Max - ImVec2(ArrowWidth, 0.f));

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
    bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
    if (!temp_input_is_active)
    {
        // Tabbing or CTRL-clicking on Drag turns it into an InputText
        const bool clicked = hovered && IsMouseClicked(0, ImGuiInputFlags_None, id);
        const bool double_clicked = (hovered && g.IO.MouseClickedCount[0] == 2 && TestKeyOwner(ImGuiKey_MouseLeft, id));
        const bool make_active = (clicked || double_clicked || g.NavActivateId == id);
        if (make_active && (clicked || double_clicked))
            SetKeyOwner(ImGuiKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if ((clicked && g.IO.KeyCtrl) || double_clicked || (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
                temp_input_is_active = true;

        // (Optional) simple click (without moving) turns Drag into an InputText
        if (g.IO.ConfigDragClickToInputText && temp_input_allowed && !temp_input_is_active)
            if (g.ActiveId == id && hovered && g.IO.MouseReleased[0] && !IsMouseDragPastThreshold(0, g.IO.MouseDragThreshold))
            {
                g.NavActivateId = id;
                g.NavActivateFlags = ImGuiActivateFlags_PreferInput;
                temp_input_is_active = true;
            }

        if (make_active && !temp_input_is_active)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask = (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    if (temp_input_is_active)
    {
        if (drawArrows)
        {
            frame_bb = ImRect(frame_bb.Min - ImVec2(ArrowWidth, 0.f), frame_bb.Max + ImVec2(ArrowWidth, 0.f));
        }
        // Only clamp CTRL+Click input when ImGuiSliderFlags_ClampOnInput is set (generally via ImGuiSliderFlags_AlwaysClamp)
        bool clamp_enabled = false;
        if ((flags & ImGuiSliderFlags_ClampOnInput) && (p_min != NULL || p_max != NULL))
        {
            const int clamp_range_dir = (p_min != NULL && p_max != NULL) ? DataTypeCompare(data_type, p_min, p_max) : 0; // -1 when *p_min < *p_max, == 0 when *p_min == *p_max
            if (p_min == NULL || p_max == NULL || clamp_range_dir < 0)
                clamp_enabled = true;
            else if (clamp_range_dir == 0)
                clamp_enabled = DataTypeIsZero(data_type, p_min) ? ((flags & ImGuiSliderFlags_ClampZeroRange) != 0) : true;
        }
        return TempInputScalar(frame_bb, id, label, data_type, p_data, format, clamp_enabled ? p_min : NULL, clamp_enabled ? p_max : NULL);
    }

    // Draw frame
    ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderNavHighlight(frame_bb, id);


    if (drawFlags) {
        if (drawArrows)
            RenderFrameRounded(frame_bb.Min, frame_bb.Max, frame_col, drawFlags, true, 0.f);
        else
            RenderFrameRounded(frame_bb.Min, frame_bb.Max, frame_col, drawFlags, true, style.FrameRounding);
    }
    else {
        if (drawArrows)
            RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, 0.f);
        else
            RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);
    }
    //increase and decrease arrows
    bool value_changed = false;
    if (drawArrows)
    {
        // left(decrease) arrow
        ImRect leftArrow_bb = ImRect(ImVec2(frame_bb.Min.x - ArrowWidth, frame_bb.Min.y), ImVec2(frame_bb.Min.x, frame_bb.Max.y));
        int ArrowID = id + 1;
        PushID(ArrowID);
        if (!ItemAdd(leftArrow_bb, ArrowID)) {
            PopID();
            return false;
        }
        bool hoveredArr, heldArr;
        bool pressedArr = ButtonBehavior(leftArrow_bb, ArrowID, &hoveredArr, &heldArr);
        frame_col = GetColorU32(g.ActiveId == ArrowID ? ImGuiCol_FrameBgActive : hoveredArr ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
        RenderFrameRounded(leftArrow_bb.Min, leftArrow_bb.Max, frame_col, ((drawFlags == ImDrawFlags_None) || !(drawFlags & ImDrawFlags_RoundCornersRight)) ? ImDrawFlags_RoundCornersLeft : ImDrawFlags_RoundCornersNone, false, g.Style.FrameRounding);
        if (hovered || hoveredArr) {
            const float ArrowHeight = leftArrow_bb.Max.y - leftArrow_bb.Min.y;
            window->DrawList->PathLineTo(ImVec2(leftArrow_bb.Min.x + (ArrowWidth * 5 / 8), leftArrow_bb.Min.y + (ArrowHeight / 4)));
            window->DrawList->PathLineTo(ImVec2(leftArrow_bb.Min.x + (ArrowWidth * 3 / 8), leftArrow_bb.Min.y + (ArrowHeight / 2)));
            window->DrawList->PathLineTo(ImVec2(leftArrow_bb.Min.x + (ArrowWidth * 5 / 8), leftArrow_bb.Min.y + (ArrowHeight * 3 / 4)));
            window->DrawList->PathStroke(GetColorU32(ImGuiCol_Text), false, 1.0f);
        }
        auto IncreaseDecreaseArrowBehavior = [&](const short plusMinus) {

            static int HeldArrowID = 0;
            static int CountFrameWhenHold = 0;
            auto checkAndDoStuff = [&]() {
                if (data_type == ImGuiDataType_Float) {
                    float* floatData = (float*)p_data;
                    float* floatMin = (float*)p_min;
                    float* floatMax = (float*)p_max;
                    if (*floatMax != *floatMin) {
                        if ((((*floatData + (plusMinus * v_speed) > *floatMin)) && (*floatData + (plusMinus * v_speed) < *floatMax)))
                        {
                            *floatData += v_speed * plusMinus;
                            ImGui::MarkItemEdited(ArrowID);
                            value_changed = true;
                        }
                        else if ((*floatData != *floatMin && *floatData != *floatMax))
                        {
                            if (plusMinus > 0)
                            {
                                *floatData = *floatMax;
                                ImGui::MarkItemEdited(ArrowID);
                                value_changed = true;
                            }
                            else {
                                *floatData = *floatMin;
                                ImGui::MarkItemEdited(ArrowID);
                                value_changed = true;

                            }
                        }
                    }
                    else {
                        *floatData += v_speed * plusMinus;
                        ImGui::MarkItemEdited(ArrowID);
                        value_changed = true;
                    }
                }
                else if (data_type == ImGuiDataType_S32) {
                    int* intData = (int*)p_data;
                    int* intMin = (int*)p_min;
                    int* intMax = (int*)p_max;
                    if (*intMax != *intMin) {
                        if ((((*intData + (plusMinus * v_speed) > *intMin)) && (*intData + (plusMinus * v_speed) < *intMax)))
                        {
                            *intData += v_speed * plusMinus;
                            ImGui::MarkItemEdited(ArrowID);
                            value_changed = true;
                        }
                        else if ((*intData != *intMin && *intData != *intMax))
                        {
                            if (plusMinus > 0)
                            {
                                *intData = *intMax;
                                ImGui::MarkItemEdited(ArrowID);
                                value_changed = true;
                            }
                            else {
                                *intData = *intMin;
                                ImGui::MarkItemEdited(ArrowID);
                                value_changed = true;

                            }
                        }
                    }
                    else {
                        *intData += v_speed * plusMinus;
                        ImGui::MarkItemEdited(ArrowID);
                        value_changed = true;
                    }
                }
                };
            if (pressedArr)
            {
                HeldArrowID = 0;
                CountFrameWhenHold = 0;
                checkAndDoStuff();
            }
            else if (heldArr)
            {
                if (HeldArrowID != ArrowID)
                {
                    HeldArrowID = ArrowID;
                }
                CountFrameWhenHold++;
                if (CountFrameWhenHold > 20)
                {
                    checkAndDoStuff();
                }

            }
            };
        IncreaseDecreaseArrowBehavior(-1);
        PopID();

        //right(increase) arrow
        ImRect rightArrow_bb = ImRect(ImVec2(frame_bb.Max.x, frame_bb.Min.y), ImVec2(frame_bb.Max.x + ArrowWidth, frame_bb.Max.y));
        ArrowID = id + 2;
        PushID(ArrowID);
        if (!ItemAdd(rightArrow_bb, ArrowID)) {
            PopID();
            return false;
        }
        pressedArr = ButtonBehavior(rightArrow_bb, ArrowID, &hoveredArr, &heldArr);
        frame_col = GetColorU32(g.ActiveId == ArrowID ? ImGuiCol_FrameBgActive : hoveredArr ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
        RenderFrameRounded(rightArrow_bb.Min, rightArrow_bb.Max, frame_col, (drawFlags == ImDrawFlags_None || !(drawFlags & ImDrawFlags_RoundCornersLeft)) ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersNone, false, g.Style.FrameRounding);
        if (hovered || hoveredArr) {
            const float ArrowHeight = rightArrow_bb.Max.y - rightArrow_bb.Min.y;
            window->DrawList->PathLineTo(ImVec2(rightArrow_bb.Min.x + (ArrowWidth * 3 / 8), rightArrow_bb.Min.y + (ArrowHeight / 4)));
            window->DrawList->PathLineTo(ImVec2(rightArrow_bb.Min.x + (ArrowWidth * 5 / 8), rightArrow_bb.Min.y + (ArrowHeight / 2)));
            window->DrawList->PathLineTo(ImVec2(rightArrow_bb.Min.x + (ArrowWidth * 3 / 8), rightArrow_bb.Min.y + (ArrowHeight * 3 / 4)));
            window->DrawList->PathStroke(GetColorU32(ImGuiCol_Text), false, 1.0f);
        }
        IncreaseDecreaseArrowBehavior(1);
        PopID();
    }

    //RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

    // Drag behavior
    value_changed = DragBehavior(id, data_type, p_data, v_speed, p_min, p_max, format, flags);
    if (value_changed)
        MarkItemEdited(id);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    //char value_buf[64];
    //const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    if (g.LogEnabled)
        LogSetNextTextDecoration("{", "}");
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
    return value_changed;
}

bool ImGui::StylisedDragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components, CalcItemWidth());
    size_t type_size = DataTypeGetInfo(data_type)->Size;

    for (int i = 0; i < components; i++)
    {
        PushID(i);
        if (i > 0)
            SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= DragScalar("", data_type, p_data, v_speed, p_min, p_max, format, flags);
        PopID();
        PopItemWidth();
        p_data = (void*)((char*)p_data + type_size);
    }
    PopID();

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }

    EndGroup();
    return value_changed;
}

bool ImGui::StylisedColorEdit(const char* label, float col[4], ImGuiColorEditFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float square_sz = GetFrameHeight();
    const char* label_display_end = FindRenderedTextEnd(label);
    float w_full = CalcItemWidth();
    g.NextItemData.ClearFlags();

    BeginGroup();
    PushID(label);
    const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
    if (set_current_color_edit_id)
        g.ColorEditCurrentID = window->IDStack.back();

    // If we're not showing any slider there's no point in doing any HSV conversions
    const ImGuiColorEditFlags flags_untouched = flags;
    if (flags & ImGuiColorEditFlags_NoInputs)
        flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

    // Context menu: display and modify options (before defaults are applied)
    if (!(flags & ImGuiColorEditFlags_NoOptions))
        ColorEditOptionsPopup(col, flags);

    // Read stored options
    if (!(flags & ImGuiColorEditFlags_DisplayMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
    if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
    if (!(flags & ImGuiColorEditFlags_PickerMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
    if (!(flags & ImGuiColorEditFlags_InputMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
    flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

    const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
    const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
    const int components = alpha ? 4 : 3;
    const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
    const float w_inputs = ImMax(w_full - w_button, 1.0f);
    w_full = w_inputs + w_button;

    // Convert to the formats we need
    float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
    if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
        ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
    else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
    {
        // Hue is lost when converting from grayscale rgb (saturation=0). Restore it.
        ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
        ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
    }
    int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

    bool value_changed = false;
    bool value_changed_as_float = false;

    const ImVec2 pos = window->DC.CursorPos;
    const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
    window->DC.CursorPos.x = pos.x + inputs_offset_x;

    if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
    {
        // RGB/HSV 0..255 Sliders
        const float w_items = w_inputs - style.ItemInnerSpacing.x * (components - 1);

        const bool hide_prefix = (IM_TRUNC(w_items / components) <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
        static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
        static const char* fmt_table_int[3][4] =
        {
            {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
            { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
            { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
        };
        static const char* fmt_table_float[3][4] =
        {
            {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
            { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
            { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
        };
        const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

        float prev_split = 0.0f;
        for (int n = 0; n < components; n++)
        {
            if (n > 0)
                SameLine(0, style.ItemInnerSpacing.x);
            float next_split = IM_TRUNC(w_items * (n + 1) / components);
            SetNextItemWidth(ImMax(next_split - prev_split, 1.0f));
            prev_split = next_split;

            // FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
            if (flags & ImGuiColorEditFlags_Float)
            {
                float h0 = 0.f;
                float h1 = 1.f;
                value_changed |= StylisedDragScalar(ids[n], ImGuiDataType_Float, &f[n], 1.0f / 255.0f, &h0, hdr ? &h0 : &h1, fmt_table_float[fmt_idx][n]);
                value_changed_as_float |= value_changed;
            }
            else
            {
                int h0 = 0;
                int h255 = 255;
                value_changed |= StylisedDragScalar(ids[n], ImGuiDataType_S32, &i[n], 1.0f, &h0, hdr ? &h0 : &h255, fmt_table_int[fmt_idx][n]);
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
        }
    }
    else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
    {
        // RGB Hexadecimal Input
        char buf[64];
        if (alpha)
            ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
        else
            ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
        SetNextItemWidth(w_inputs);
        if (InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsUppercase))
        {
            value_changed = true;
            char* p = buf;
            while (*p == '#' || ImCharIsBlankA(*p))
                p++;
            i[0] = i[1] = i[2] = 0;
            i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
            int r;
            if (alpha)
                r = sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
            else
                r = sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
            IM_UNUSED(r); // Fixes C6031: Return value ignored: 'sscanf'.
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
    }

    ImGuiWindow* picker_active_window = NULL;
    if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
    {
        const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

        const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
        if (ColorButton("##ColorButton", col_v4, flags))
        {
            if (!(flags & ImGuiColorEditFlags_NoPicker))
            {
                // Store current color and open a picker
                g.ColorPickerRef = col_v4;
                OpenPopup("picker");
                SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
            }
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

        if (BeginPopup("picker"))
        {
            if (g.CurrentWindow->BeginCount == 1)
            {
                picker_active_window = g.CurrentWindow;
                if (label != label_display_end)
                {
                    TextEx(label, label_display_end);
                    Spacing();
                }
                ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
                SetNextItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
                value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
            }
            EndPopup();
        }
    }

    if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
    {
        // Position not necessarily next to last submitted button (e.g. if style.ColorButtonPosition == ImGuiDir_Left),
        // but we need to use SameLine() to setup baseline correctly. Might want to refactor SameLine() to simplify this.
        SameLine(0.0f, style.ItemInnerSpacing.x);
        window->DC.CursorPos.x = pos.x + ((flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full + style.ItemInnerSpacing.x);
        TextEx(label, label_display_end);
    }

    // Convert back
    if (value_changed && picker_active_window == NULL)
    {
        if (!value_changed_as_float)
            for (int n = 0; n < 4; n++)
                f[n] = i[n] / 255.0f;
        if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
        {
            g.ColorEditSavedHue = f[0];
            g.ColorEditSavedSat = f[1];
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
            g.ColorEditSavedID = g.ColorEditCurrentID;
            g.ColorEditSavedColor = ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
        }
        if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

        col[0] = f[0];
        col[1] = f[1];
        col[2] = f[2];
        if (alpha)
            col[3] = f[3];
    }

    if (set_current_color_edit_id)
        g.ColorEditCurrentID = 0;
    PopID();
    EndGroup();

    // Drag and Drop Target
    // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
    if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(g.LastItemData.InFlags & ImGuiItemFlags_ReadOnly) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
    {
        bool accepted_drag_drop = false;
        if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512 //-V1086
            value_changed = accepted_drag_drop = true;
        }
        if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * components);
            value_changed = accepted_drag_drop = true;
        }

        // Drag-drop payloads are always RGB
        if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
        EndDragDropTarget();
    }

    // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
    if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
        g.LastItemData.ID = g.ActiveId;

    if (value_changed && g.LastItemData.ID != 0) // In case of ID collision, the second EndGroup() won't catch g.ActiveId
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

#endif // #ifndef IMGUI_DISABLE
