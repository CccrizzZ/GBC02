#include "CollisionManager.h"
#include "Util.h"
#include <algorithm>



int CollisionManager::squaredDistance(glm::vec2 P1, glm::vec2 P2)
{
	int diffOfXs = P2.x - P1.x;
	int diffOfYs = P2.y - P1.y;
	int result = diffOfXs * diffOfXs + diffOfYs * diffOfYs;

	return result;
}

bool CollisionManager::squaredRadiusCheck(GameObject * object1, GameObject * object2)
{
	glm::vec2 P1 = object1->getPosition();
	glm::vec2 P2 = object2->getPosition();
	int halfHeights = (object1->getHeight() + object2->getHeight()) * 0.5f;

	//if (glm::distance(P1, P2) < halfHeights) {

	if (CollisionManager::squaredDistance(P1, P2) < (halfHeights * halfHeights)) {
		if (!object2->getIsColliding()) {
			
			object2->setIsColliding(true);

			switch (object2->getType()) {
			case PLANET:
				std::cout << "Collision with Planet!" << std::endl;
				TheSoundManager::Instance()->playSound("yay", 0);
				break;
			case MINE:
				std::cout << "Collision with Mine!" << std::endl;
				TheSoundManager::Instance()->playSound("thunder", 0);
				break;
			default:
				//std::cout << "Collision with unknown type!" << std::endl;
					break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->setIsColliding(false);
		return false;
	}
}

// bool CollisionManager::AABBCheck(GameObject * object1, GameObject * object2)
// {
// 	// prepare relevant variables
// 	glm::vec2 P1 = object1->getPosition();
// 	glm::vec2 P2 = object2->getPosition();
// 	float P1width = object1->getWidth();
// 	float P1height = object1->getHeight();
// 	float P2width = object2->getWidth();
// 	float P2height = object2->getHeight();

// 	if(
// 		P1.x < P2.x + P2width &&
// 		P1.x + P1width > P2.x &&
// 		P1.y < P2.y + P2height &&
// 		P1.y + P1height > P2.y
// 		)
// 	{
// 		if (!object2->getIsColliding()) {

// 			object2->setIsColliding(true);

// 			switch (object2->getType()) {
// 			case PLATFORM:

// 				break;
// 			default:
// 				//std::cout << "Collision with unknown type!" << std::endl;
// 				break;
// 			}

// 			return true;
// 		}
// 		return false;
// 	}
// 	else
// 	{
// 		object2->setIsColliding(false);
// 		return false;
// 	}
	
// 	return false;
// }
bool CollisionManager::AABB(GameObject* obj1, GameObject* obj2)
{
	int tempW1 = obj1->getWidth() / 2;
	int tempW2 = obj2->getWidth() / 2;
	int tempH1 = obj1->getHeight() / 2;
	int tempH2 = obj2->getHeight() / 2;
	float obj1_X = obj1->getPosition().x;
	float obj2_X = obj2->getPosition().x;
	float obj1_Y = obj1->getPosition().y;
	float obj2_Y = obj2->getPosition().y;


	if
	((
		obj1_X + tempW1 >= obj2_X - tempW2 &&
		obj1_X - tempW1 <= obj2_X + tempW2) &&
		(obj1_Y + tempH1 >= obj2_Y - tempH2 &&
		obj1_Y - tempH1 <= obj2_Y + tempH2
	))
	{
		return true;
		
	}
	return false;
}

bool CollisionManager::lineLineCheck(glm::vec2 line1Start, glm::vec2 line1End, glm::vec2 line2Start, glm::vec2 line2End)
{
	float x1 = line1Start.x;
	float x2 = line1End.x;
	float x3 = line2Start.x;
	float x4 = line2End.x;
	float y1 = line1Start.y;
	float y2 = line1End.y;
	float y3 = line2Start.y;
	float y4 = line2End.y;

	// calculate the distance to intersection point
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) 
	{
		return true;
	}
	
	return false;
}

bool CollisionManager::lineRectCheck(glm::vec2 line1Start, glm::vec2 line1End, glm::vec2 recStart, float recWidth, float recHeight)
{
	float x1 = line1Start.x;
	float x2 = line1End.x;
	float y1 = line1Start.y;
	float y2 = line1End.y;
	float rx = recStart.x;
	float ry = recStart.y;
	float rw = recWidth;
	float rh = recHeight;

	//TODO FIX THIS

	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	bool left = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx, ry + rh));
	bool right = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx + rw, ry), glm::vec2(rx + rw, ry + rh));
	bool top = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx + rw, ry));
	bool bottom = lineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry + rh), glm::vec2(rx + rw, ry + rh));

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
		return true;
	}

	return false;
}

int CollisionManager::minSquaredDistanceLineLine(glm::vec2 line1Start, glm::vec2 line1End, glm::vec2 line2Start, glm::vec2 line2End)
{
	glm::vec2 u = line1End - line1Start;
	glm::vec2 v = line2End - line2Start;
	glm::vec2 w = line1Start - line2Start;
	float a = Util::dot(u, u);         // always >= 0
	float b = Util::dot(u, v);
	float c = Util::dot(v, v);         // always >= 0
	float d = Util::dot(u, w);
	float e = Util::dot(v, w);
	float D = a * c - b * b;					// always >= 0
	float sc, tc;

	// compute the line parameters of the two closest points
	if (D < Util::EPSILON) {					// the lines are almost parallel
		sc = 0.0;
		tc = (b > c ? d / b : e / c);			// use the largest denominator
	}
	else {
		sc = (b*e - c * d) / D;
		tc = (a*e - b * d) / D;
	}

	// get the difference of the two closest points
	glm::vec2 dP = w + (sc * u) - (tc * v);  // =  L1(sc) - L2(tc)

	float norm = Util::dot(dP, dP);
	return norm;
}

int CollisionManager::circleAABBsquaredDistance(glm::vec2 circleCentre, int circleRadius, glm::vec2 boxStart, int boxWidth, int boxHeight)
{
	float dx = std::max(boxStart.x - circleCentre.x, 0.0f);
	dx = std::max(dx, circleCentre.x - (boxStart.x + boxWidth));
	float dy = std::max(boxStart.y - circleCentre.y, 0.0f);
	dy = std::max(dy, circleCentre.y - (boxStart.y + boxHeight));

	return (dx * dx) + (dy * dy);
}

bool CollisionManager::circleAABBCheck(GameObject * object1, GameObject * object2)
{
	// circle
	glm::vec2 circleCentre = object1->getPosition();
	int circleRadius = std::max(object1->getWidth() * 0.5f, object1->getHeight() * 0.5f);
	// aabb
	int boxWidth = object2->getWidth();
	int halfBoxWidth = boxWidth * 0.5f;
	int boxHeight = object2->getHeight();
	int halfBoxHeight = boxHeight * 0.5f;
	
	glm::vec2 boxStart = object2->getPosition() - glm::vec2(boxWidth * 0.5f, boxHeight * 0.5f);

	if(circleAABBsquaredDistance(circleCentre, circleRadius, boxStart, boxWidth, boxHeight) <= (circleRadius * circleRadius))
	{
		if (!object2->getIsColliding()) {

			object2->setIsColliding(true);

			glm::vec2 attackVector = object1->getPosition() - object2->getPosition();
			glm::vec2  normal = glm::vec2(0.0f, -1.0f);


			/*std::cout << "=====================================" << std::endl;
			std::cout << "AttackX: " << attackVector.x << std::endl;
			std::cout << "AttackY: " << attackVector.y << std::endl;*/
			float dot = Util::dot(attackVector, normal);
			/*std::cout << "dot: " << dot << std::endl;*/
			float angle = acos(dot / Util::magnitude(attackVector)) * Util::Rad2Deg;
			//std::cout << "Angle: " << angle << std::endl;
			
			switch (object2->getType()) {
			case PLANET:
				std::cout << "Collision with Planet!" << std::endl;
				TheSoundManager::Instance()->playSound("yay", 0);
				break;
			case MINE:
				std::cout << "Collision with Mine!" << std::endl;
				TheSoundManager::Instance()->playSound("thunder", 0);
				break;
			case SHIP:
				//std::cout << "Collision with Ship!" << std::endl;
				TheSoundManager::Instance()->playSound("thunder", 0);

				if((attackVector.x > 0 && attackVector.y < 0) || (attackVector.x < 0 && attackVector.y < 0)) 
					// top right or top left
				{
					if(angle <= 45)
					{
						object1->setVelocity(glm::vec2(object1->getVelocity().x, -object1->getVelocity().y));
					}
					else
					{
						object1->setVelocity(glm::vec2(-object1->getVelocity().x, object1->getVelocity().y));
					}
				}

				if((attackVector.x > 0 && attackVector.y > 0)  || (attackVector.x < 0 && attackVector.y > 0))
					// bottom right or bottom left
				{
					if(angle <= 135)
					{
						object1->setVelocity(glm::vec2(-object1->getVelocity().x, object1->getVelocity().y));
					}
					else
					{
						object1->setVelocity(glm::vec2(object1->getVelocity().x, -object1->getVelocity().y));
					}
				}
				
				break;
			default:
				//std::cout << "Collision with unknown type!" << std::endl;
				break;
			}

			return true;
		}
		return false;
	}
	else
	{
		object2->setIsColliding(false);
		return false;
	}
	
	return false;
}

bool CollisionManager::capsuleCapsuleCheck(GameObject * object1, GameObject * object2)
{
	glm::vec2 P1 = object1->getPosition();
	glm::vec2 P2 = object2->getPosition();
	int P1Width = object1->getWidth();
	int P1Height = object1->getHeight();
	int P2Width = object2->getWidth();
	int P2Height = object2->getHeight();
	
	float radius1 = glm::max(P1Width, P1Height);
	float radius2 = glm::max(P2Width, P2Height);

	float distSq = minSquaredDistanceLineLine(
		glm::vec2(P1.x, P1.y - P1Height * 0.5),
		glm::vec2(P1.x, P1.y + P1Height * 0.5),
		glm::vec2(P2.x - P2Width * 0.5f, P2.y), 
		glm::vec2(P2.x + P2Width * 0.5f, P2.y));
	
	float sumRadii = radius1 + radius2;
	
	if(distSq <= (sumRadii * sumRadii))
	{
		if (!object2->getIsColliding()) {

			object2->setIsColliding(true);

			switch (object2->getType()) {
				case PLATFORM:
					std::cout << "Collision with Platform!" << std::endl;
					TheSoundManager::Instance()->playSound("yay", 0);
					break;
				default:
					//std::cout << "Collision with unknown type!" << std::endl;
					break;
			}
			return true;
		}
	}
	else
	{
		object2->setIsColliding(false);
		return false;
	}
	
	return false;
}

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}
