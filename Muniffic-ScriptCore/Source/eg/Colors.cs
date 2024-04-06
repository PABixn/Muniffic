using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    /// <summary>
    /// Color in default RGBA Color format.
    /// </summary>
    public class Color
    {
        public float r, g, b, alpha;

        #region Constructors

        /// <summary>
        /// Creates new Color with RGB values and sets alpha to 1.
        /// </summary>
        public Color(float red, float green, float blue)
        {
            red = red > 1 ? 1 : red < 0 ? 0 : red;
            green = green > 1 ? 1 : green < 0 ? 0 : green;
            blue = blue > 1 ? 1 : blue < 0 ? 0 : blue;

            this.r = red;
            this.g = green;
            this.b = blue;
            alpha = 1f;
        }

        /// <summary>
        /// Creates new Color with RGB values and specified alpha.
        /// </summary>
        public Color(float red, float green, float blue, float alpha)
        {
            red = red > 1 ? 1 : red < 0 ? 0 : red;
            green = green > 1 ? 1 : green < 0 ? 0 : green;
            blue = blue > 1 ? 1 : blue < 0 ? 0 : blue;
            alpha = alpha > 1 ? 1 : alpha < 0 ? 0 : alpha;

            this.r = red;
            this.g = green;
            this.b = blue;
            this.alpha = alpha;
        }

        /// <summary>
        /// Creates new Color based on Vector4.
        /// </summary>
        /// <param name="color">Vector3 with corresponding RGBA values.</param>
        public Color(Vector4 color)
        {
            r = color.X;
            g = color.Y;
            b = color.Z;
            alpha = color.W;
        }

        /// <summary>
        /// Creates new Color based on Vector3 and sets alpha to 1.
        /// </summary>
        /// <param name="color">Vector3 with corresponding RGB values.</param>
        public Color(Vector3 color)
        {
            r = color.X;
            g = color.Y;
            b = color.Z;
            alpha = 1f;
        }
        
        /// <summary>
        /// Creates new Color from hex string.
        /// </summary>
        /// <param name="hex">String with color in hex format.</param>
        /// <remarks>Giving 6 digit hex will result in alpha being set to 1.</remarks>
        public Color(string hex)
        {
            try
            {
                if (hex.StartsWith("#"))
                {
                    hex = hex.Substring(1);
                }

                if (hex.Length == 6)
                {
                    r = int.Parse(hex.Substring(0, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    g = int.Parse(hex.Substring(2, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    b = int.Parse(hex.Substring(4, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    alpha = 1f;
                }
                else if (hex.Length == 8)
                {
                    r = int.Parse(hex.Substring(0, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    g = int.Parse(hex.Substring(2, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    b = int.Parse(hex.Substring(4, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    alpha = int.Parse(hex.Substring(6, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                }
                else
                {
                    r = 0.0f;
                    g = 0.0f;
                    b = 0.0f;
                    alpha = 0.0f; //error
                }
            }
            catch(System.FormatException)
            {
                r = 0.0f;
                g = 0.0f;
                b = 0.0f;
                alpha = 0.0f; //error
            }
        }

        #endregion
        
        /// <summary>
        /// Creates new Color with all values set to 0.
        /// </summary>
        public static Color Zero()
        {
            return new Color(0, 0, 0, 0);
        }

        /// <summary>
        /// Converts Color to hex string.
        /// </summary>
        /// <param name="color">Color in default RGBA color format.</param>
        /// <returns>String with color in hex format.</returns>
        public static string ToHex(Color color)
        {
            return string.Format("#{0:X2}{1:X2}{2:X2}{3:X2}", (int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255), (int)(color.alpha * 255));
        }

        /// <summary>
        /// Converts Color to Vector4.
        /// </summary>
        /// <param name="color">Color in default RGBA color format.</param>
        /// <returns>Vector4 with corresponding RGBA values.</returns>
        public static Vector4 ToVector4(Color color)
        {
            return new Vector4(color.r, color.g, color.b, color.alpha);
        }

        /// <summary>
        /// Converts Color to Vector3.
        /// </summary>
        /// <param name="color">Color in default RGBA color format.</param>
        /// <returns>Vector4 with corresponding RGB values.</returns>
        public static Vector3 ToVector3(Color color)
        {
            return new Vector3(color.r, color.g, color.b);
        }

        #region Operators

        public static Color operator +(Color a, Color b)
        {
            return new Color(a.r + b.r, a.g + b.g, a.b + b.b, a.alpha + b.alpha);
        }

        public static Color operator -(Color a, Color b)
        {
            return new Color(a.r - b.r, a.g - b.g, a.b - b.b, a.alpha - b.alpha);
        }

        public static Color operator *(Color a, Color b)
        {
            return new Color(a.r * b.r, a.g * b.g, a.b * b.b, a.alpha * b.alpha);
        }

        public static Color operator /(Color a, Color b)
        {
            return new Color(a.r / b.r, a.g / b.g, a.b / b.b, a.alpha / b.alpha);
        }

        public static bool operator ==(Color a, Color b)
        {
            return a.r == b.r && a.g == b.g && a.b == b.b && a.alpha == b.alpha;
        }

        public static bool operator !=(Color a, Color b)
        {
            return a.r != b.r || a.g != b.g || a.b != b.b || a.alpha != b.alpha;
        }

        public override string ToString()
        {
            return string.Format("Red: {0}, Green: {1}, Blue: {2}, Alpha: {3}", red, green, blue, alpha);
        
        }

        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }

            Color color = obj as Color;
            if ((object)color == null)
            {
                return false;
            }

            return r == color.r && g == color.g && b == color.b && alpha == color.alpha;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        #endregion

        #region Named Colors

        public static Color white { get => new Color(1f, 1f, 1f); }
        public static Color black { get => new Color(0f, 0f, 0f); }
        public static Color red { get => new Color(1f, 0f, 0f); }
        public static Color green { get => new Color(0f, 1f, 0f); }
        public static Color blue { get => new Color(0f, 0f, 1f); }
        public static Color yellow { get => new Color(1f, 1f, 0f); }
        public static Color cyan { get => new Color(0f, 1f, 1f); }
        public static Color magenta { get => new Color(1f, 0f, 1f); }
        public static Color gray { get => new Color(0.5f, 0.5f, 0.5f); }
        public static Color silver { get => new Color(0.75f, 0.75f, 0.75f); }
        public static Color maroon { get => new Color(0.5f, 0f, 0f); }
        public static Color olive { get => new Color(0.5f, 0.5f, 0f); }
        public static Color lime { get => new Color(0f, 1f, 0f); }
        public static Color aqua { get => new Color(0f, 1f, 1f); }
        public static Color teal { get => new Color(0f, 0.5f, 0.5f); }
        public static Color navy { get => new Color(0f, 0f, 0.5f); }
        public static Color fuchsia { get => new Color(1f, 0f, 1f); }
        public static Color purple { get => new Color(0.5f, 0f, 0.5f); }
        public static Color orange { get => new Color(1f, 0.647f, 0f); }
        public static Color brown { get => new Color(0.647f, 0.165f, 0.165f); }
        public static Color darkGray { get => new Color(0.3f, 0.3f, 0.3f); }
        public static Color mediumGray { get => new Color(0.6f, 0.6f, 0.6f); }
        public static Color lightGray { get => new Color(0.8f, 0.8f, 0.8f); }
        public static Color violet { get => new Color(0.933f, 0.509f, 0.933f); }
        public static Color indigo { get => new Color(0.294f, 0.0f, 0.51f); }
        public static Color blueViolet { get => new Color(0.541f, 0.169f, 0.886f); }
        public static Color royalBlue { get => new Color(0.255f, 0.412f, 0.882f); }
        public static Color cornflowerBlue { get => new Color(0.392f, 0.584f, 0.929f); }
        public static Color dodgerBlue { get => new Color(0.118f, 0.565f, 1.0f); }
        public static Color steelBlue { get => new Color(0.275f, 0.51f, 0.706f); }
        public static Color skyBlue { get => new Color(0.529f, 0.808f, 0.922f); }
        public static Color deepSkyBlue { get => new Color(0.0f, 0.749f, 1.0f); }
        public static Color lightSkyBlue { get => new Color(0.529f, 0.808f, 0.98f); }
        public static Color pink { get => new Color(1.0f, 0.753f, 0.796f); }
        public static Color peach { get => new Color(1.0f, 0.855f, 0.725f); }
        public static Color lavender { get => new Color(0.902f, 0.902f, 0.98f); }
        public static Color mint { get => new Color(0.686f, 0.933f, 0.686f); }
        public static Color coral { get => new Color(1.0f, 0.498f, 0.314f); }
        public static Color turquoise { get => new Color(0.251f, 0.878f, 0.816f); }
        public static Color gold { get => new Color(1.0f, 0.843f, 0.0f); }
        public static Color silverGray { get => new Color(0.749f, 0.749f, 0.749f); }
        public static Color darkSlateGray { get => new Color(0.184f, 0.31f, 0.31f); }
        public static Color salmon { get => new Color(0.98f, 0.502f, 0.447f); }
        public static Color pastelPink { get => new Color(1.0f, 0.769f, 0.796f); }
        public static Color pastelBlue { get => new Color(0.68f, 0.78f, 0.937f); }
        public static Color pastelGreen { get => new Color(0.596f, 0.984f, 0.596f); }
        public static Color pastelYellow { get => new Color(0.961f, 0.961f, 0.863f); }
        public static Color pastelPurple { get => new Color(0.7f, 0.507f, 0.714f); }
        public static Color sienna { get => new Color(0.627f, 0.322f, 0.176f); }
        public static Color khaki { get => new Color(0.941f, 0.902f, 0.549f); }
        public static Color oliveDrab { get => new Color(0.419f, 0.556f, 0.137f); }
        public static Color sandyBrown { get => new Color(0.957f, 0.643f, 0.376f); }
        public static Color rosyBrown { get => new Color(0.737f, 0.561f, 0.561f); }
        #endregion
    }

    /// <summary>
    /// Color in HSV format.
    /// </summary>
    public class HSVColor
    {
        public float hue, saturation, value, alpha;

        #region Constructors

        /// <summary>
        /// Creates new HSVColor with HSV values and sets alpha to 1.
        /// </summary>
        public HSVColor(float hue, float saturation, float value)
        {
            hue = hue > 1 ? 1 : hue < 0 ? 0 : hue;
            saturation = saturation > 1 ? 1 : saturation < 0 ? 0 : saturation;
            value = value > 1 ? 1 : value < 0 ? 0 : value;

            this.hue = hue;
            this.saturation = saturation;
            this.value = value;
            alpha = 1f;
        }

        /// <summary>
        /// Creates new HSVColor with HSV values and specified alpha.
        /// </summary>
        public HSVColor(float hue, float saturation, float value, float alpha)
        {
            hue = hue > 1 ? 1 : hue < 0 ? 0 : hue;
            saturation = saturation > 1 ? 1 : saturation < 0 ? 0 : saturation;
            value = value > 1 ? 1 : value < 0 ? 0 : value;
            alpha = alpha > 1 ? 1 : alpha < 0 ? 0 : alpha;

            this.hue = hue;
            this.saturation = saturation;
            this.value = value;
            this.alpha = alpha;
        }

        /// <summary>
        /// Creates new HSVColor from default RGBA color format.
        /// </summary>
        /// <param name="color">Color in default RGBA color format.</param>
        public HSVColor(Color color)
        {
            HSVColor hsv = RGBAToHSV(color);
            hue = hsv.hue;
            saturation = hsv.saturation;
            value = hsv.value;
            alpha = hsv.alpha;
        }

        #endregion

        /// <summary>
        /// Creates new HSVColor with all values set to 0.
        /// </summary>
        public static HSVColor Zero()
        {
            return new HSVColor(0, 0, 0, 0);
        }

        /// <summary>
        /// Converts HSVColor to default RGBA color format.
        /// </summary>
        /// <param name="color">Color in HSVColor format.</param>
        /// <returns>Color in default RGBA Color format.</returns>
        public static Color ToColor(HSVColor color)
        {
            return HSVToRGBA(color);
        }

        #region Operators

        public static HSVColor operator +(HSVColor a, HSVColor b)
        {
            return new HSVColor(a.hue + b.hue, a.saturation + b.saturation, a.value + b.value, a.alpha + b.alpha);
        }

        public static HSVColor operator -(HSVColor a, HSVColor b)
        {
            return new HSVColor(a.hue - b.hue, a.saturation - b.saturation, a.value - b.value, a.alpha - b.alpha);
        }

        public static HSVColor operator *(HSVColor a, HSVColor b)
        {
            return new HSVColor(a.hue * b.hue, a.saturation * b.saturation, a.value * b.value, a.alpha * b.alpha);
        }

        public static HSVColor operator /(HSVColor a, HSVColor b)
        {
            return new HSVColor(a.hue / b.hue, a.saturation / b.saturation, a.value / b.value, a.alpha / b.alpha);
        }

        public static bool operator ==(HSVColor a, HSVColor b)
        {
            return a.hue == b.hue && a.saturation == b.saturation && a.value == b.value && a.alpha == b.alpha;
        }

        public static bool operator !=(HSVColor a, HSVColor b)
        {
            return a.hue != b.hue || a.saturation != b.saturation || a.value != b.value || a.alpha != b.alpha;
        }

        public override string ToString()
        {
            return string.Format("Hue: {0}, Saturation: {1}, Value: {2}, Alpha: {3}", hue, saturation, value, alpha);
        }

        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }

            HSVColor color = obj as HSVColor;
            if ((object)color == null)
            {
                return false;
            }

            return hue == color.hue && saturation == color.saturation && value == color.value && alpha == color.alpha;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        #endregion

        #region Private

        public static Color HSVToRGBA(HSVColor hsv)
        {
            float c = hsv.value / 100.0f * hsv.saturation / 100.0f;
            float x = c * (1 - Math.Abs((hsv.hue / 60.0f) % 2 - 1));
            float m = hsv.value / 100.0f - c;

            float r, g, b;
            if (hsv.hue >= 0 && hsv.hue < 60)
            {
                r = c;
                g = x;
                b = 0;
            }
            else if (hsv.hue >= 60 && hsv.hue < 120)
            {
                r = x;
                g = c;
                b = 0;
            }
            else if (hsv.hue >= 120 && hsv.hue < 180)
            {
                r = 0;
                g = c;
                b = x;
            }
            else if (hsv.hue >= 180 && hsv.hue < 240)
            {
                r = 0;
                g = x;
                b = c;
            }
            else if (hsv.hue >= 240 && hsv.hue < 300)
            {
                r = x;
                g = 0;
                b = c;
            }
            else
            {
                r = c;
                g = 0;
                b = x;
            }

            return new Color(
                (float)Math.Round((byte)Math.Round((r + m) * 255f) / 255f, 2),
                (float)Math.Round((byte)Math.Round((g + m) * 255f) / 255f, 2),
                (float)Math.Round((byte)Math.Round((b + m) * 255f) / 255f, 2)
            );
        }

        public static HSVColor RGBAToHSV(Color color)
        {
            float[] hsv = new float[3];

            float max = new[] { color.r, color.g, color.b }.Max();
            float min = new[] { color.r, color.g, color.b }.Min();

            float delta = max - min;

            hsv[1] = max != 0 ? delta / max : 0;
            hsv[2] = max;

            if (hsv[1] == 0)
            {
                return new HSVColor(hsv[0] / 100f, (float)Math.Round(hsv[1], 2), (float)Math.Round(hsv[2], 2));
            }
            if (color.r == max)
            {
                hsv[0] = ((color.g - color.b) / delta);
            }
            else if (color.g == max)
            {
                hsv[0] = ((color.b - color.r) / delta) + 2.0f;
            }
            else if (color.b == max)
            {
                hsv[0] = ((color.r - color.g) / delta) + 4.0f;
            }

            hsv[0] *= 60.0f;

            if (hsv[0] < 0)
            {
                hsv[0] += 360.0f;
            }

            return new HSVColor(hsv[0] / 100f, (float)Math.Round(hsv[1], 2), (float)Math.Round(hsv[2], 2));
        }

        #endregion
    }

    /// <summary>
    /// Color in HSL format.
    /// </summary>
    public class HSLColor
    {
        public float hue, saturation, lightness, alpha;

        #region Constructors

        /// <summary>
        /// Creates new HSLColor with HSL values and sets alpha to 1.
        /// </summary>
        public HSLColor(float hue, float saturation, float lightness)
        {
            hue = hue > 1 ? 1 : hue < 0 ? 0 : hue;
            saturation = saturation > 1 ? 1 : saturation < 0 ? 0 : saturation;
            lightness = lightness > 1 ? 1 : lightness < 0 ? 0 : lightness;

            this.hue = hue;
            this.saturation = saturation;
            this.lightness = lightness;
            alpha = 1f;
        }

        /// <summary>
        /// Creates new HSLColor with HSL values and specified alpha.
        /// </summary>
        public HSLColor(float hue, float saturation, float lightness, float alpha)
        {
            hue = hue > 1 ? 1 : hue < 0 ? 0 : hue;
            saturation = saturation > 1 ? 1 : saturation < 0 ? 0 : saturation;
            lightness = lightness > 1 ? 1 : lightness < 0 ? 0 : lightness;
            alpha = alpha > 1 ? 1 : alpha < 0 ? 0 : alpha;

            this.hue = hue;
            this.saturation = saturation;
            this.lightness = lightness;
            this.alpha = alpha;
        }

        /// <summary>
        /// Creates new HSLColor from default RGBA color format.
        /// </summary>
        /// <param name="color">Color in default RGBA color format.</param>
        public HSLColor(Color color)
        {
            HSLColor hsl = RGBAToHSL(color);
            hue = hsl.hue;
            saturation = hsl.saturation;
            lightness = hsl.lightness;
            alpha = hsl.alpha;
        }

        #endregion

        /// <summary>
        /// Creates new HSLColor with all values set to 0.
        /// </summary>
        public static HSLColor Zero()
        {
            return new HSLColor(0, 0, 0, 0);
        }

        /// <summary>
        /// Converts HSLColor to default RGBA color format.
        /// </summary>
        /// <param name="color">Color in HSL color format.</param>
        /// <returns>Color in default RGBA color format.</returns>
        public static Color ToColor(HSLColor color)
        {
            return HSLToRGBA(color);
        }

        #region Operators

        public static HSLColor operator +(HSLColor a, HSLColor b)
        {
            return new HSLColor(a.hue + b.hue, a.saturation + b.saturation, a.lightness + b.lightness, a.alpha + b.alpha);
        }

        public static HSLColor operator -(HSLColor a, HSLColor b)
        {
            return new HSLColor(a.hue - b.hue, a.saturation - b.saturation, a.lightness - b.lightness, a.alpha - b.alpha);
        }

        public static HSLColor operator *(HSLColor a, HSLColor b)
        {
            return new HSLColor(a.hue * b.hue, a.saturation * b.saturation, a.lightness * b.lightness, a.alpha * b.alpha);
        }

        public static HSLColor operator /(HSLColor a, HSLColor b)
        {
            return new HSLColor(a.hue / b.hue, a.saturation / b.saturation, a.lightness / b.lightness, a.alpha / b.alpha);
        }

        public static bool operator ==(HSLColor a, HSLColor b)
        {
            return a.hue == b.hue && a.saturation == b.saturation && a.lightness == b.lightness && a.alpha == b.alpha;
        }

        public static bool operator !=(HSLColor a, HSLColor b)
        {
            return a.hue != b.hue || a.saturation != b.saturation || a.lightness != b.lightness || a.alpha != b.alpha;
        }

        public override string ToString()
        {
            return string.Format("Hue: {0}, Saturation: {1}, Lightness: {2}, Alpha: {3}", hue, saturation, lightness, alpha);
        }

        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }

            HSLColor color = obj as HSLColor;
            if ((object)color == null)
            {
                return false;
            }

            return hue == color.hue && saturation == color.saturation && lightness == color.lightness && alpha == color.alpha;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        #endregion

        #region Private

        public static Color HSLToRGBA(HSLColor hsl)
        {
            float c = (1 - Math.Abs(2 * hsl.lightness / 100.0f - 1)) * hsl.saturation / 100.0f;
            float x = c * (1 - Math.Abs((hsl.hue / 60.0f) % 2 - 1));
            float m = hsl.lightness / 100.0f - c / 2.0f;

            float r, g, b;
            if (hsl.hue >= 0 && hsl.hue < 60)
            {
                r = c;
                g = x;
                b = 0;
            }
            else if (hsl.hue >= 60 && hsl.hue < 120)
            {
                r = x;
                g = c;
                b = 0;
            }
            else if (hsl.hue >= 120 && hsl.hue < 180)
            {
                r = 0;
                g = c;
                b = x;
            }
            else if (hsl.hue >= 180 && hsl.hue < 240)
            {
                r = 0;
                g = x;
                b = c;
            }
            else if (hsl.hue >= 240 && hsl.hue < 300)
            {
                r = x;
                g = 0;
                b = c;
            }
            else
            {
                r = c;
                g = 0;
                b = x;
            }

            return new Color(
                (float)Math.Round((byte)Math.Round((r + m) * 255f) / 255f, 2),
                (float)Math.Round((byte)Math.Round((g + m) * 255f) / 255f, 2),
                (float)Math.Round((byte)Math.Round((b + m) * 255f) / 255f, 2)
            );
        }


        public static HSLColor RGBAToHSL(Color color)
        {
            float[] hsl = new float[3];

            float max = new[] { color.r, color.g, color.b }.Max();
            float min = new[] { color.r, color.g, color.b }.Min();

            float delta = max - min;

            hsl[2] = (max + min) / 2;

            if (delta == 0)
            {
                hsl[0] = 0;
                hsl[1] = 0;
            }
            else
            {
                hsl[1] = hsl[2] < 0.5 ? delta / (max + min) : delta / (2 - max - min);

                if (max == color.r)
                {
                    hsl[0] = (color.g - color.b) / delta + (color.g < color.b ? 6 : 0);
                }
                else if (max == color.g)
                {
                    hsl[0] = (color.b - color.r) / delta + 2;
                }
                else if (max == color.b)
                {
                    hsl[0] = (color.r - color.g) / delta + 4;
                }

                hsl[0] *= 60;
            }

            return new HSLColor(hsl[0] / 100f, (float)Math.Round(hsl[1], 2), (float)Math.Round(hsl[2], 2));
        }

        #endregion
    }

    /// <summary>
    /// Color in CMYK format.
    /// </summary>
    public class CMYKColor
    {
        public float cyan, magenta, yellow, black, alpha;

        #region Constructors

        /// <summary>
        /// Creates new CMYKColor with CMYK values and sets alpha to 1.
        /// </summary>
        public CMYKColor(float cyan, float magenta, float yellow, float black)
        {
            cyan = cyan > 1 ? 1 : cyan < 0 ? 0 : cyan;
            magenta = magenta > 1 ? 1 : magenta < 0 ? 0 : magenta;
            yellow = yellow > 1 ? 1 : yellow < 0 ? 0 : yellow;
            black = black > 1 ? 1 : black < 0 ? 0 : black;

            this.cyan = cyan;
            this.magenta = magenta;
            this.yellow = yellow;
            this.black = black;
            alpha = 1f;
        }

        /// <summary>
        /// Creates new CMYKColor with CMYK values and specified alpha.
        /// </summary>
        public CMYKColor(float cyan, float magenta, float yellow, float black, float alpha)
        {
            cyan = cyan > 1 ? 1 : cyan < 0 ? 0 : cyan;
            magenta = magenta > 1 ? 1 : magenta < 0 ? 0 : magenta;
            yellow = yellow > 1 ? 1 : yellow < 0 ? 0 : yellow;
            black = black > 1 ? 1 : black < 0 ? 0 : black;
            alpha = alpha > 1 ? 1 : alpha < 0 ? 0 : alpha;

            this.cyan = cyan;
            this.magenta = magenta;
            this.yellow = yellow;
            this.black = black;
            this.alpha = alpha;
        }

        /// <summary>
        /// Creates new CMYKColor from default RGBA color format.
        /// </summary>
        /// <param name="color">Color in default RGBA color format.</param>
        public CMYKColor(Color color)
        {
            CMYKColor cmyk = RGBAToCMYK(color);
            cyan = cmyk.cyan;
            magenta = cmyk.magenta;
            yellow = cmyk.yellow;
            black = cmyk.black;
            alpha = cmyk.alpha;
        }

        #endregion

        /// <summary>
        /// Creates new CMYKColor with all values set to 0.
        /// </summary>
        public static CMYKColor Zero()
        {
            return new CMYKColor(0, 0, 0, 0, 0);
        }

        /// <summary>
        /// Converts CMYKColor to default RGBA color format.
        /// </summary>
        /// <param name="color">Color in CMYK color format.</param>
        /// <returns>Color in default RGBA color format.</returns>
        public static Color ToColor(CMYKColor color)
        {
            return CMYKToRGBA(color);
        }

        #region Operators

        public static CMYKColor operator +(CMYKColor a, CMYKColor b)
        {
            return new CMYKColor(a.cyan + b.cyan, a.magenta + b.magenta, a.yellow + b.yellow, a.black + b.black, a.alpha + b.alpha);
        }

        public static CMYKColor operator -(CMYKColor a, CMYKColor b)
        {
            return new CMYKColor(a.cyan - b.cyan, a.magenta - b.magenta, a.yellow - b.yellow, a.black - b.black, a.alpha - b.alpha);
        }

        public static CMYKColor operator *(CMYKColor a, CMYKColor b)
        {
            return new CMYKColor(a.cyan * b.cyan, a.magenta * b.magenta, a.yellow * b.yellow, a.black * b.black, a.alpha * b.alpha);
        }

        public static CMYKColor operator /(CMYKColor a, CMYKColor b)
        {
            return new CMYKColor(a.cyan / b.cyan, a.magenta / b.magenta, a.yellow / b.yellow, a.black / b.black, a.alpha / b.alpha);
        }

        public static bool operator ==(CMYKColor a, CMYKColor b)
        {
            return a.cyan == b.cyan && a.magenta == b.magenta && a.yellow == b.yellow && a.black == b.black && a.alpha == b.alpha;
        }

        public static bool operator !=(CMYKColor a, CMYKColor b)
        {
            return a.cyan != b.cyan || a.magenta != b.magenta || a.yellow != b.yellow || a.black != b.black || a.alpha != b.alpha;
        }

        public override string ToString()
        {
            return string.Format("Cyan: {0}, Magenta: {1}, Yellow: {2}, Black: {3}, Alpha: {4}", cyan, magenta, yellow, black, alpha);
        }

        public override bool Equals(object obj)
        {
            if (obj == null)
            {
                return false;
            }

            CMYKColor color = obj as CMYKColor;
            if ((object)color == null)
            {
                return false;
            }

            return cyan == color.cyan && magenta == color.magenta && yellow == color.yellow && black == color.black && alpha == color.alpha;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        #endregion

        #region Private

        private static CMYKColor RGBAToCMYK(Color color)
        {
            float c = 1 - color.r;
            float m = 1 - color.g;
            float y = 1 - color.b;
            float k = Math.Min(c, Math.Min(m, y));

            if (k == 1)
            {
                return new CMYKColor(0, 0, 0, 1);
            }

            return new CMYKColor(
                    (c - k) / (1 - k),
                    (m - k) / (1 - k),
                    (y - k) / (1 - k),
                    k);
        }


        private static Color CMYKToRGBA(CMYKColor cmyk)
        {
            return new Color((1 - cmyk.cyan) * (1 - cmyk.black),
                             (1 - cmyk.magenta) * (1 - cmyk.black),
                             (1 - cmyk.yellow) * (1 - cmyk.black),
                             cmyk.alpha);
        }

        #endregion
    }
}