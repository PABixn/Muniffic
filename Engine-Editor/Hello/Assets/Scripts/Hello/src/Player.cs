using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using eg;

namespace Game
{
    internal class Player : DefaultBehaviour
    {
        public void OnStart()
        {
            Console.WriteLine("Player OnStart");
        }

        public void OnUpdate(float ts)
        {
            
        }

        public void Test()
        {
            Console.WriteLine("Player Test");
        }
    }
}
