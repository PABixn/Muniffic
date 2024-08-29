using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class BaseEnemy : DefaultBehaviour
    {
        public int enemyType = 0;
        public HealthComponent healthComponent;

        private EntityTypeComponent entityTypeComponent;

        private EnemyMeleeAttackComponent meleeAttackComponent;

        public int BasEnemyHealth { get; private set; } = 300;
        public int SquarEnemyHealth { get; private set; } = 800;
        public int CirclEnemyHealth { get; private set; } = 400;
        public int TrianglEnemyHealth { get; private set; } = 250;

        public float BasEnemySpeedMultiplier { get; private set; } = 1;
        public float SquarEnemySpeedMultiplier { get; private set; } = 0.5f;
        public float CirclEnemySpeedMultiplier { get; private set; } = 1;
        public float TrianglEnemySpeedMultiplier { get; private set; } = 1.5f;

        

        private Entity player;
        private Player playerScript;

        public void OnCreate()
        {
            player = Entity.FindEntityByName("Player");
            meleeAttackComponent = new EnemyMeleeAttackComponent(entity, new List<EntityType> { EntityType.PLAYER_CIRCLE, EntityType.PLAYER_BASE, EntityType.PLAYER_SQUARE, EntityType.PLAYER_TRIANGLE }, "PlayerWrapper");
        }

        public void OnUpdate(float ts)
        {
            if (playerScript == null) playerScript = player.As<Player>();
            if (healthComponent == null) healthComponent = entity.As<HealthComponent>();
            if(entityTypeComponent == null) entityTypeComponent = entity.As<EntityTypeComponent>();
            if (entityTypeComponent.entityType == EntityType.NONE)
            {
                entityTypeComponent.entityType = IntToEntityType(enemyType);
                SwitchEnemyType(IntToEnemyType(enemyType));
            }
            meleeAttackComponent.Update(ts);

            if (healthComponent.IsDead())
            {
                Die();
            }
        }

        private EnemyType IntToEnemyType(int type)
        {
            switch (type)
            {
                case 0:
                    return EnemyType.SQUARE;
                case 1:
                    return EnemyType.CIRCLE;
                case 2:
                    return EnemyType.TRIANGLE;
                default:
                    return EnemyType.SQUARE;
            }
        }

        private EntityType IntToEntityType(int type)
        {
            switch (type)
            {
                case 0:
                    return EntityType.ENEMY_SQUARE;
                case 1:
                    return EntityType.ENEMY_CIRCLE;
                case 2:
                    return EntityType.ENEMY_TRIANGLE;
                default:
                    return EntityType.ENEMY_SQUARE;
            }
        }

        private void SwitchEnemyType(EnemyType type)
        {
            switch (type)
            {
                case EnemyType.SQUARE:
                    InitSquareEnemy();
                    break;
                case EnemyType.CIRCLE:
                    InitCircleEnemy();
                    break;
                case EnemyType.TRIANGLE:
                    InitTriangleEnemy();
                    break;
                default:
                    InitSquareEnemy();
                    break;
            }
        }

        private void InitSquareEnemy()
        {
            enemyType = 0;
            healthComponent.maxHealth = SquarEnemyHealth;
            healthComponent.SetHealth(SquarEnemyHealth);
        }

        private void InitCircleEnemy()
        {
            enemyType = 1;
            healthComponent.maxHealth = CirclEnemyHealth;
            healthComponent.SetHealth(CirclEnemyHealth);
        }

        private void InitTriangleEnemy()
        {
            enemyType = 2;
            healthComponent.maxHealth = TrianglEnemyHealth;
            healthComponent.SetHealth(TrianglEnemyHealth);
        }

        private void Die()
        {
            Entity.Destroy(entity);
            playerScript.SwitchPlayerType(IntToEnemyType(enemyType));
        }

        public EnemyMeleeAttackComponent getMeleeAttackComponent()
        {
            return meleeAttackComponent;
        }
    };
}
