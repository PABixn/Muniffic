using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public class Color
    {
        public float red, green, blue, alpha;

        #region Constructors

        public Color(float red, float green, float blue)
        {
            this.red = red;
            this.green = green;
            this.blue = blue;
            alpha = 1f;
        }

        public Color(float red, float green, float blue, float alpha)
        {
            this.red = red;
            this.green = green;
            this.blue = blue;
            this.alpha = alpha;
        }

        public Color(Vector4 color)
        {
            red = color.X;
            green = color.Y;
            blue = color.Z;
            alpha = color.W;
        }

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

        public static Color Zero()
        {
            return new Color(0, 0, 0, 0);
        }

        public static string ToHex(Color color)
        {
            return string.Format("#{0:X2}{1:X2}{2:X2}{3:X2}", (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255), (int)(color.alpha * 255));
        }

        public static Vector4 ToVector4(Color color)
        {
            return new Vector4(color.red, color.green, color.blue, color.alpha);
        }
    }

    public class HSVColor
    {
        public float hue, saturation, value, alpha;

        #region Constructors

        public HSVColor(float hue, float saturation, float value)
        {
            this.hue = hue;
            this.saturation = saturation;
            this.value = value;
            alpha = 1f;
        }

        public HSVColor(float hue, float saturation, float value, float alpha)
        {
            this.hue = hue;
            this.saturation = saturation;
            this.value = value;
            this.alpha = alpha;
        }

        public HSVColor(Color color)
        {
            HSVColor hsv = RGBAToHSV(color);
            hue = hsv.hue;
            saturation = hsv.saturation;
            value = hsv.value;
            alpha = hsv.alpha;
        }

        #endregion

        public static HSVColor Zero()
        {
            return new HSVColor(0, 0, 0, 0);
        }

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
                return new HSVColor(hsv[0], (float)Math.Round(hsv[1] * 100f, 2), (float)Math.Round(hsv[2] * 100f, 2));
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

            return new HSVColor(hsv[0], (float)Math.Round(hsv[1] * 100f, 2), (float)Math.Round(hsv[2] * 100f, 2));
        }
    }
}
