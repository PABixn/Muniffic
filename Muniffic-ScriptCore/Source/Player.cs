using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Sandbox
{
    public class Player : Entity
    {
       
        void OnCreate()
        {
            Console.WriteLine("Player created! - " + ID);
        }

        void OnUpdate(float ts)
        {
            Console.WriteLine("Player updated!");
            float speed = 1f;
            Vector3 velocity = new Vector3(0);
            if (Input.IsKeyDown(KeyCode.W))
            {
                velocity.Y = speed;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                velocity.Y = -speed;
            }
            else
            {
                velocity.Y = 0f;
            }
            if(Input.IsKeyDown(KeyCode.A))
            {
                velocity.X = -speed;
            }
            else if(Input.IsKeyDown(KeyCode.D))
            {
                velocity.X = speed;
            }
            else
            {
                velocity.X = 0f;
            }

            Vector3 translation = Translation;
            translation += velocity * ts;
            Translation = translation;
        }

    }
}
