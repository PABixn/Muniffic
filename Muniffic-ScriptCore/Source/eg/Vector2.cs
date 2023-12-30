﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace eg
{
    public struct Vector2
    {
        public float X, Y;

        public static Vector2 Zero = new Vector2(0f, 0f);

        public Vector2(Vector2 vec)
        {
            X = vec.X;
            Y = vec.Y;
        }

        public Vector2(float x)
        {
            X = x;
            Y = x;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }
        public float LengthSquared()
        {
            return X * X + Y * Y;
        }

        public float Length()
        {
            return (float)Math.Sqrt(LengthSquared());
        }

        static public Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        static public Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }

        static public Vector2 operator *(Vector2 a, float b)
        {
            return new Vector2(a.X * b, a.Y * b);
        }
    };
}