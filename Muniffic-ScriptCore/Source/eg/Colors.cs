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
        public float red, green, blue, alpha;

        #region Constructors

        /// <summary>
        /// Creates new Color with RGB values and sets alpha to 1.
        /// </summary>
        public Color(float red, float green, float blue)
        {
            this.red = red;
            this.green = green;
            this.blue = blue;
            alpha = 1f;
        }

        /// <summary>
        /// Creates new Color with RGB values and specified alpha.
        /// </summary>
        public Color(float red, float green, float blue, float alpha)
        {
            this.red = red;
            this.green = green;
            this.blue = blue;
            this.alpha = alpha;
        }

        /// <summary>
        /// Creates new Color based on Vector4.
        /// </summary>
        /// <param name="color">Vector3 with corresponding RGBA values.</param>
        public Color(Vector4 color)
        {
            red = color.X;
            green = color.Y;
            blue = color.Z;
            alpha = color.W;
        }

        /// <summary>
        /// Creates new Color based on Vector3 and sets alpha to 1.
        /// </summary>
        /// <param name="color">Vector3 with corresponding RGB values.</param>
        public Color(Vector3 color)
        {
            red = color.X;
            green = color.Y;
            blue = color.Z;
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
                    red = int.Parse(hex.Substring(0, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    green = int.Parse(hex.Substring(2, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    blue = int.Parse(hex.Substring(4, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    alpha = 1f;
                }
                else if (hex.Length == 8)
                {
                    red = int.Parse(hex.Substring(0, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    green = int.Parse(hex.Substring(2, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    blue = int.Parse(hex.Substring(4, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                    alpha = int.Parse(hex.Substring(6, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                }
                else
                {
                    red = 0.0f;
                    green = 0.0f;
                    blue = 0.0f;
                    alpha = 0.0f; //error
                }
            }
            catch(System.FormatException)
            {
                red = 0.0f;
                green = 0.0f;
                blue = 0.0f;
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
            return string.Format("#{0:X2}{1:X2}{2:X2}{3:X2}", (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255), (int)(color.alpha * 255));
        }

        /// <summary>
        /// Converts Color to Vector4.
        /// </summary>
        /// <param name="color">Color in default RGBA color format.</param>
        /// <returns>Vector4 with corresponding RGBA values.</returns>
        public static Vector4 ToVector4(Color color)
        {
            return new Vector4(color.red, color.green, color.blue, color.alpha);
        }

        /// <summary>
        /// Converts Color to Vector3.
        /// </summary>
        /// <param name="color">Color in default RGBA color format.</param>
        /// <returns>Vector4 with corresponding RGB values.</returns>
        public static Vector3 ToVector3(Color color)
        {
            return new Vector3(color.red, color.green, color.blue);
        }
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

            float max = new[] { color.red, color.green, color.blue }.Max();
            float min = new[] { color.red, color.green, color.blue }.Min();

            float delta = max - min;

            hsv[1] = max != 0 ? delta / max : 0;
            hsv[2] = max;

            if (hsv[1] == 0)
            {
                return new HSVColor(hsv[0] / 100f, (float)Math.Round(hsv[1], 2), (float)Math.Round(hsv[2], 2));
            }
            if (color.red == max)
            {
                hsv[0] = ((color.green - color.blue) / delta);
            }
            else if (color.green == max)
            {
                hsv[0] = ((color.blue - color.red) / delta) + 2.0f;
            }
            else if (color.blue == max)
            {
                hsv[0] = ((color.red - color.green) / delta) + 4.0f;
            }

            hsv[0] *= 60.0f;

            if (hsv[0] < 0)
            {
                hsv[0] += 360.0f;
            }

            return new HSVColor(hsv[0] / 100f, (float)Math.Round(hsv[1], 2), (float)Math.Round(hsv[2], 2));
        }
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

            float max = new[] { color.red, color.green, color.blue }.Max();
            float min = new[] { color.red, color.green, color.blue }.Min();

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

                if (max == color.red)
                {
                    hsl[0] = (color.green - color.blue) / delta + (color.green < color.blue ? 6 : 0);
                }
                else if (max == color.green)
                {
                    hsl[0] = (color.blue - color.red) / delta + 2;
                }
                else if (max == color.blue)
                {
                    hsl[0] = (color.red - color.green) / delta + 4;
                }

                hsl[0] *= 60;
            }

            return new HSLColor(hsl[0] / 100f, (float)Math.Round(hsl[1], 2), (float)Math.Round(hsl[2], 2));
        }
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
        
        private static CMYKColor RGBAToCMYK(Color color)
        {
            float c = 1 - color.red;
            float m = 1 - color.green;
            float y = 1 - color.blue;
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
    }
}
