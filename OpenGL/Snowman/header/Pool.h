#ifndef POOL
#define POOL

#include <vector>
#include <../Snowman/header/GameObject/SnowBullet.h>
//Pooling을 위한 것

class Pool {
	std::vector<SnowBullet *> poolObjects;
	const int MAX_POOL_SIZE = 10;
public:
	Pool(Shader *shader, std::vector<GameObject*> & objects) 
	{
		poolObjects = std::vector<SnowBullet*>(MAX_POOL_SIZE);

		for (int i = 0; i < MAX_POOL_SIZE; i++)
		{
			SnowBullet* snowbullet = new SnowBullet(*shader, glm::vec3(0.5f, 0.5f, 0.5f));
			snowbullet->setIsActive(false);
			objects.push_back(snowbullet);
			poolObjects[i] = snowbullet;
		}
	}

	void createPoolObject()
	{
		for (int i = 0; i < MAX_POOL_SIZE; i++)
		{
			SnowBullet* snowBullet = poolObjects[i];
			if (!snowBullet->getIsActive())
			{
				snowBullet->setIsActive(true);
				return;
			}
		}
		//그냥 없는거

	}
};


#endif
#pragma once
