using eg;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class GroundCheck
    {
        public static bool IsGrounded(BoxCollider2DComponent groundCheck)
        {
            List<Entity> entities = Entity.FindEntitiesByName("Wall");
            entities.AddRange(Entity.FindEntitiesByName("Parkour Wall"));

            foreach (Entity entity in entities)
            {
                if (groundCheck.CollidesWithTopEdge(entity))
                {
                    return true;
                }
            }

            return false;
        }
    }
}
