using System;
using System.Collections.Generic;
using System.Diagnostics.PerformanceData;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public class Camera : DefaultBehaviour
    {
        Entity player;
        public float zoom = 1f;
        public float zoomSpeed = 5f;
        TransformComponent transform;
        TransformComponent playerTransform;

        void OnCreate()
        {
            CameraComponent.ProjectionType type = CameraComponent.ProjectionType.Perspective;
            if (HasComponent<CameraComponent>())
                GetComponent<CameraComponent>().type = type;

            player = Entity.FindEntityByName("Player");

            if (player != null)
            {
                DebugConsole.Log("Player found!", DebugConsole.LogType.Info);
            }
            else
            {
                DebugConsole.Log("Player not found", DebugConsole.LogType.Error);
            }

            transform = GetComponent<TransformComponent>();
            playerTransform = player.GetComponent<TransformComponent>();

            transform.translation = new Vector3(playerTransform.translation.X, playerTransform.translation.Y, 10 / zoom);
        }

        void OnUpdate(float ts)
        {
            if (Input.IsKeyPressed(KeyCode.Q))
                zoom -= 1 / zoomSpeed * ts;
            else if (Input.IsKeyPressed(KeyCode.E))
                zoom += 1 / zoomSpeed * ts;

            transform.translation = new Vector3(playerTransform.translation.X, playerTransform.translation.Y, 10 / zoom);
        }
    }
}