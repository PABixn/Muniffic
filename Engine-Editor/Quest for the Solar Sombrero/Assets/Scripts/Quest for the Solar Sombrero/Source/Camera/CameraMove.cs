using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class CameraMove : DefaultBehaviour
    {
        TransformComponent transformComponent;
        Entity player;

        public void OnUpdate(float ts)
        {
            transformComponent.translation = player.GetComponent<TransformComponent>().translation;
            //DebugConsole.Log(player.GetComponent<TransformComponent>().translation.XY.ToString(), logType:DebugConsole.LogType.Info);
        }

        public void OnCreate()
        {
            player = Entity.FindEntityByName("Player");
            transformComponent = entity.GetComponent<TransformComponent>();
            Console.WriteLine("player lkasjdflkjasdlkfjlkasjdlfk");
        }
    }
}
