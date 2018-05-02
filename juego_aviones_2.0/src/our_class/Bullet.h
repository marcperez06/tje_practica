#ifndef BULLET_H
#define BULLET_H

class Bullet {
	public:
		Vector3 position;
		Vector3 lastPosition;
		Vector3 velocity;
		float timeToLive;
		float power;
		int author; // uuid de la entidad que dispara.
		std::string type;

		Bullet();

};

#endif // !BULLET_H

