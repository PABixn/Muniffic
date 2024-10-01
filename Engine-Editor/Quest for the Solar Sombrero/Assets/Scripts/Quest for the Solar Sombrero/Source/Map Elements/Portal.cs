using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class Portal : DefaultBehaviour
    {
        //private Entity player;
        //private Entity portal2;
        //private TransformComponent transformComponentPlayer;
        //private TransformComponent transformComponentPortal2;

        public void OnCreate()
        {
           
        }

        public void Update(float ts)
        {
        }

        public void OnCollision(Collision2D collision)
        {
            if (collision.otherEntity == null || collision.otherEntity.name != "Player")
            {
                return;
            }
            //player = Entity.FindEntityByName("Player");
            //portal2 = Entity.FindEntityByName("Portal2");
            //transformComponentPlayer = player.GetComponent<TransformComponent>();
            //transformComponentPortal2 = portal2.GetComponent<TransformComponent>();
            //transformComponentPlayer.translation = transformComponentPortal2.translation;
            Entity.Destroy(entity);
        }
    }
}
