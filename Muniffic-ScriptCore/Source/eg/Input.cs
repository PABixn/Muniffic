using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public class Input
    {
        public static string GetClipboardContent()
        {
            return InternalCalls.Input_GetClipboardContent();
        }
        public static void SetClipboardContent(string content)
        {
            InternalCalls.Input_SetClipboardContent(content);
        }
        public static string GetKeyName(KeyCode key)
        {
            return InternalCalls.Input_GetKeyName(key);
        }
        /// <summary>
        /// Checks if given key is pressed.
        /// </summary>
        /// <param name="key">Code of the key.</param>
        /// <returns>true if the key is currently pressed and false if not.</returns>
        public static bool IsKeyPressed(KeyCode key)
        {
            return InternalCalls.Input_IsKeyPressed(key);
        }
        public static bool IsKeyReleased(KeyCode key)
        {
            return InternalCalls.Input_IsKeyReleased(key);
        }
        public static bool IsMouseButtonPressed(MouseCode button)
        {
            return InternalCalls.Input_IsMouseButtonPressed(button);
        }
        public static bool IsMouseButtonReleased(MouseCode button)
        {
            return InternalCalls.Input_IsMouseButtonReleased(button);
        }
        public static bool IsCursorOnWindow()
        {
            return InternalCalls.Input_IsCursorOnWindow();
        }
        public static float GetCursorPositonX()
        {
            return InternalCalls.Input_GetCursorPositonX();
        }
        public static float GetCursorPositonY()
        {
            return InternalCalls.Input_GetCursorPositonY();
        }
        public static void SetCursorMode(int mode)
        {
            InternalCalls.Input_SetCursorMode(mode);
        }
        public static void SetStickyKeysEnabled(bool enable)
        {
            InternalCalls.Input_SetStickyKeysEnabled(enable);
        }
        public static void SetStickyMouseButtonsEnabled(bool enable)
        {
            InternalCalls.Input_SetStickyMouseButtonsEnabled(enable);
        }
    }
}
