#include "../dependencies/header.h"


/**
 * movePlayer - set the next position of the player
 * @DeltaTime: time elapsed since the last frame
*/
void movePlayer(float DeltaTime)
{
    // Update the player's rotation angle based on input
    player.rotationAngle += player.turnDirection * player.turnSpeed * DeltaTime;

    // Calculate movement step based on the player's direction and speed
    float moveStep = player.walkDirection * player.walkSpeed * DeltaTime;

    // Calculate the player's new potential position
    float newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
    float newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

    // Move the player if no collision is detected at the new position
    if (!DetectCollision(newPlayerX, newPlayerY))
    {
        player.x = newPlayerX;
        player.y = newPlayerY;
    }
}


/**
 * renderPlayer - render the player
 *
*/

void renderPlayer(void)
{
	drawRect(
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.width * MINIMAP_SCALE_FACTOR,
		player.height * MINIMAP_SCALE_FACTOR,
		0xFFFFFFFF
	);
}
