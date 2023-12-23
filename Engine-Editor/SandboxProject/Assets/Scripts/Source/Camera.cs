using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Sandbox
{
    public class Camera : Entity
    {
        

        void OnUpdate(float ts)
        {
            float speed = 5f;
            Vector3 velocity = new Vector3(0);
            if (Input.IsKeyDown(KeyCode.Up))
            {
                velocity.Y = speed;
            }
            else if (Input.IsKeyDown(KeyCode.Down))
            {
                velocity.Y = -speed;
            }
            else
            {
                velocity.Y = 0f;
            }
            if(Input.IsKeyDown(KeyCode.Left))
            {
                velocity.X = -speed;
            }
            else if(Input.IsKeyDown(KeyCode.Right))
            {
                velocity.X = speed;
            }
            else
            {
                velocity.X = 0f;
            }

            Console.WriteLine("What");

            Vector3 translation = Translation;
            translation += velocity * ts;
            Translation = translation;
        }

    }
}
