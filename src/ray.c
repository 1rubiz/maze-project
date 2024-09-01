#include "../dependencies/header.h"

ray_t rays[NUM_RAYS];

static bool foundHorzWallHit, foundVertWallHit;
static float horzWallHitX, horzWallHitY, vertWallHitX, vertWallHitY;
static int horzWallContent, vertWallContent;


/**
 * horzIntersection - Finds horizontal intersection with the wall
 * @rayAngle: current ray angle
 */
void horzIntersection(float rayAngle)
{
    // Initialize intersection flags and coordinates
    foundHorzWallHit = false;
    horzWallHitX = horzWallHitY = horzWallContent = 0;

    // Calculate the initial horizontal intercept
    yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
    if (isRayFacingDown(rayAngle)) {
        yintercept += TILE_SIZE;
    }
    xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

    // Calculate the step size for each iteration
    ystep = TILE_SIZE * (isRayFacingUp(rayAngle) ? -1 : 1);
    xstep = TILE_SIZE / tan(rayAngle);
    if ((isRayFacingLeft(rayAngle) && xstep > 0) || (isRayFacingRight(rayAngle) && xstep < 0)) {
        xstep = -xstep;
    }

    // Initialize the first point of intersection
    nextHorzTouchX = xintercept;
    nextHorzTouchY = yintercept;

    // Iterate until a wall is hit or the ray leaves the map boundaries
    while (isInsideMap(nextHorzTouchX, nextHorzTouchY))
    {
        float xToCheck = nextHorzTouchX;
        float yToCheck = nextHorzTouchY + (isRayFacingUp(rayAngle) ? -1 : 0);

        if (DetectCollision(xToCheck, yToCheck))
        {
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            horzWallContent = getMapValue((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
            foundHorzWallHit = true;
            break;
        }
        
        nextHorzTouchX += xstep;
        nextHorzTouchY += ystep;
    }
}


/**
 * vertIntersection - Finds vertical intersection with the wall
 * @rayAngle: current ray angle
 *
 */

void vertIntersection(float rayAngle)
{
	float nextVertTouchX, nextVertTouchY;
	float xintercept, yintercept, xstep, ystep;

	foundVertWallHit = false;
	vertWallHitX = 0;
	vertWallHitY = 0;
	vertWallContent = 0;

	xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight(rayAngle) ? TILE_SIZE : 0;
	yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft(rayAngle) ? -1 : 1;
	ystep = TILE_SIZE * tan(rayAngle);
	ystep *= (isRayFacingUp(rayAngle) && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown(rayAngle) && ystep < 0) ? -1 : 1;
	nextVertTouchX = xintercept;
	nextVertTouchY = yintercept;

	while (isInsideMap(nextVertTouchX, nextVertTouchY))
	{
		float xToCheck = nextVertTouchX + (isRayFacingLeft(rayAngle) ? -1 : 0);
		float yToCheck = nextVertTouchY;

		if (DetectCollision(xToCheck, yToCheck))
		{
			vertWallHitX = nextVertTouchX;
			vertWallHitY = nextVertTouchY;
			vertWallContent = getMapValue((int)floor(yToCheck / TILE_SIZE),
									   (int)floor(xToCheck / TILE_SIZE));
			foundVertWallHit = true;
			break;
		}
		nextVertTouchX += xstep;
		nextVertTouchY += ystep;
	}
}

/**
 * castRay - casting of each ray
 * @rayAngle: current ray angle
 * @stripId: ray strip identifier
 * FLT_MAX - the maximum value of a float.
 */
void castRay(float rayAngle, int stripId)
{
    // Normalize the ray angle to the range [0, TWO_PI]
    rayAngle = fmod(rayAngle + TWO_PI, TWO_PI);

    // Find horizontal and vertical intersections
    horzIntersection(rayAngle);
    vertIntersection(rayAngle);

    // Calculate distances to horizontal and vertical hits
    float horzHitDistance = foundHorzWallHit
        ? distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY)
        : FLT_MAX;
    float vertHitDistance = foundVertWallHit
        ? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
        : FLT_MAX;

    // Determine the closer intersection and update the ray data
    bool hitVertical = (vertHitDistance < horzHitDistance);
    rays[stripId].distance = hitVertical ? vertHitDistance : horzHitDistance;
    rays[stripId].wallHitX = hitVertical ? vertWallHitX : horzWallHitX;
    rays[stripId].wallHitY = hitVertical ? vertWallHitY : horzWallHitY;
    rays[stripId].wallHitContent = hitVertical ? vertWallContent : horzWallContent;
    rays[stripId].wasHitVertical = hitVertical;
    rays[stripId].rayAngle = rayAngle;
}


/**
 * castAllRays - cast of all rays
 *
 */

void castAllRays(void)
{
    float halfNumRays = NUM_RAYS / 2.0;
    
    for (int col = 0; col < NUM_RAYS; col++)
    {
        // Calculate the angle for the current ray
        float rayAngle = player.rotationAngle + atan((col - halfNumRays) / PROJ_PLANE);
        
        // Cast the ray and store the result
        castRay(rayAngle, col);
    }
}


/**
 * renderRays - draw all the rays
 *
 */

void renderRays(void)
{
	int i;

	for (i = 0; i < NUM_RAYS; i += 50)
	{
		drawLine(
			player.x * MINIMAP_SCALE_FACTOR,
			player.y * MINIMAP_SCALE_FACTOR,
			rays[i].wallHitX * MINIMAP_SCALE_FACTOR,
			rays[i].wallHitY * MINIMAP_SCALE_FACTOR,
			0xFF0000FF
		);
	}
}
