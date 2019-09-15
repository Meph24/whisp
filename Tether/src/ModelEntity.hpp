#ifndef ENTITYDIAMOND_HPP
#     define ENTITYDIAMOND_HPP

#include "Entity.h"

#include "Spacevec.h"

#include "Model.hpp"
#include "Timestamp.h"
#include "CumulativeMat.hpp"

#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;


class ModelEntity : public Entity
{
	Model model;

	CumulativeMat cummat;
public:
	ModelEntity(const Model& model);
	~ModelEntity();

	void move(spacevec d);
	void rotate(vec3 rot);
	void scale(vec3 scale);

	spacevec getPos() const;

	vector<vec4> shape(const vec3& pos = vec3(0.0f)) const;

	//temporary solution to find extent in a certain direction
	// here the direction is hardcoded to below
	virtual float groundedDistance() const;

	virtual void draw(	Timestamp ts, 
						Frustum* viewFrustum, 
						ChunkManager* cm, 
						DrawServiceProvider* dsp
					 );

	virtual void tick(	Timestamp t,
						TickServiceProvider* tsp
					 );
};

#endif /* ENTITYDIAMOND_HPP */
