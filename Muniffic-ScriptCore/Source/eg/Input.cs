using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public class Input
    {
        /// <summary>
        /// Checks if given key is pressed.
        /// </summary>
        /// <param name="key">Code of the key.</param>
        /// <returns>true if the key is currently pressed and false if not.</returns>
        public static bool IsKeyDown(KeyCode key)
        {
            return InternalCalls.Input_IsKeyDown(key);
        }
        public static string GetClipboardContent()
        {
            return InternalCalls.Input_GetClipboardContent();
        }
        public static void SetClipboardContent(string content)
        {
            InternalCalls.Input_SetClipboardContent(content);
        }
    }
}
