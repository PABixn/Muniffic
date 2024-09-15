using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class HealthBar : DefaultBehaviour
    {
        private TextComponent textComponent;
        private Entity player;
        private HealthComponent playersHealthComponent;

        public void OnCreate()
        {
            textComponent = entity.GetComponent<TextComponent>();
            textComponent.text = "alskdfj";
            player = Entity.FindEntityByName("Player");
        }

        public void OnUpdate(float ts)
        {
            if (playersHealthComponent == null) playersHealthComponent = player.As<HealthComponent>();
            textComponent.text = "Your health: " + playersHealthComponent.health.ToString();
        }
    }
}
