#pragma once

#include <genesis.h>
#include "player.h"
#include "mobmanager.h"
#include "bullet.h"
#include "interactions.h"

#define X_MARGIN 32 // Margen en píxeles para la verificación inicial en X, ajusta según sea necesario

bool checkAABBIntersection(const struct pBody *body1, struct pBody *body2)
{
    // Verificar si alguno de los punteros es NULL
    if (body1 == NULL || body2 == NULL)
    {
        return FALSE;
    }

    if (body1->dead || body2->dead)
    {
        return FALSE;
    }

    // Calcular la distancia absoluta en el eje X entre las posiciones globales
    s16 xDistance = abs(body1->globalPosition.x - body2->globalPosition.x);

    // asumiendo que body1 es el player 1 y body 2 es un mob
    // si la distancia es mayor que una pantalla, desactivar y esconder mob
    if (xDistance > 300)
    {
        body2->active = FALSE;
        SPR_setPosition(body2->sprite, 0, 0);
        return FALSE;
    }
    else
    {
        body2->active = TRUE;
    }

    // Si la distancia en X es mayor que tamaño del hitbox, no hay necesidad de verificar AABB
    if (xDistance > body1->aabb.max.x)
    {
        return FALSE;
    }

    // Si pasamos el chequeo de distancia en X, procedemos con la verificación AABB completa
    // tal vez sea mejor calcularla aca nuevamente para claridad
    AABB body1Bounds = body1->globalAABB;
    AABB body2Bounds = body2->globalAABB;

    // Comprobar intersección en el eje X
    bool intersectX = body1Bounds.min.x <= body2Bounds.max.x &&
                      body1Bounds.max.x >= body2Bounds.min.x;

    // Comprobar intersección en el eje Y
    bool intersectY = body1Bounds.min.y <= body2Bounds.max.y &&
                      body1Bounds.max.y >= body2Bounds.min.y;

    // Hay intersección si se cumple en ambos ejes
    return intersectX && intersectY;
}

void checkHitMobs()
{
    Vect2D_s16 bulletGlobalPosition;

    // todo chequear solo active bullets y no el max_bullets posible
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            bulletGlobalPosition.x = bullets[i].position.x + cameraPosition.x+4; // +4 center th bulet hitbox
            bulletGlobalPosition.y = bullets[i].position.y + cameraPosition.y+4;
            for (int j = 0; j < MAX_NUM_MOBS; j++)
            {
                debug(mobs[j]->dead, "mobs[j]->dead", j);
                if (mobs[j]->active && !mobs[j]->dead)
                {
                    // Verificación inicial rápida en X
                    //    if (abs(bulletGlobalPosition.x - mobs[j]->globalPosition.x) <= X_MARGIN)
                    //  {
                    // Verificación completa de colisión

                    if (bulletGlobalPosition.x >= mobs[j]->globalAABB.min.x &&
                        bulletGlobalPosition.x <= mobs[j]->globalAABB.max.x && 
                        bulletGlobalPosition.y >= mobs[j]->globalAABB.min.y &&
                        bulletGlobalPosition.y <= mobs[j]->globalAABB.max.y)
                    {
                     /*   debug(j, "j", 2);
                        debug(mobs[j]->hp, "mobs[j]->hp", 3);
                        debug(bullets[i].active, "bullets[i].active", 4);*/
                        
                        // Colisión detectada

                        bullets[i].active = FALSE; // Desactivar la bala
                        hitMob(mobs[j]);
                    }

                    //  }
                }
            }
        }
    }
}