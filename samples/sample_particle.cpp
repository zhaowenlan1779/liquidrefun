// SPDX-FileCopyrightText: 2022 Erin Catto
// SPDX-License-Identifier: MIT

#include "draw.h"
#include "human.h"
#include "random.h"
#include "sample.h"

#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "box2d/particle/b2ParticleSystem.h"
#include "box2d/particle/b2ParticleGroup.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <vector>


class DamBreak : public Sample
{
public:
	explicit DamBreak( SampleContext* context )
		: Sample( context )
	{
		if ( m_context->restart == false )
		{
			m_context->camera.m_center = { 0.0f, 0.0f };
			m_context->camera.m_zoom = 25.0f * 0.45f;
		}

		{
			b2BodyDef bodyDef = b2DefaultBodyDef();
			m_groundBodyId = b2CreateBody( m_worldId, &bodyDef );

			const b2Vec2 vertices[4] = {
				b2Vec2(-2, 0),
				b2Vec2(2, 0),
				b2Vec2(2, 4),
				b2Vec2(-2, 4)};
			b2ChainDef cd = b2DefaultChainDef();
			cd.count = 4;
			cd.points = vertices;
			cd.isLoop = true;
			b2CreateChain(m_groundBodyId, &cd);
		}

		b2ParticleSystemDef particleSystemDef; 
		b2ParticleSystem* m_particleSystem = b2CreateParticleSystem(m_worldId, &particleSystemDef);
		m_particleSystem->SetRadius(0.025f);
		m_particleSystem->SetDamping(0.2f);

		{
			b2Polygon box = b2MakeOffsetBox (0.8f, 1.0f, b2Vec2(-1.2f, 1.01f), b2Rot_identity);
			b2ShapeDef shapeDef = b2DefaultShapeDef();
			b2ShapeId polygonShape = b2CreatePolygonShape( m_groundBodyId, &shapeDef, &box );

			b2ParticleGroupDef pd;
			pd.flags = b2_waterParticle;
			pd.shape = polygonShape;
			b2ParticleGroup * const group = m_particleSystem->CreateParticleGroup(pd);

			b2DestroyShape(polygonShape, true);
		}
	}

	void UpdateGui() override
	{
		float height = 100.0f;
		ImGui::SetNextWindowPos( ImVec2( 10.0f, m_context->camera.m_height - height - 50.0f ), ImGuiCond_Once );
		ImGui::SetNextWindowSize( ImVec2( 240.0f, height ) );

		ImGui::Begin( "Dam Break", nullptr, ImGuiWindowFlags_NoResize );
		ImGui::End();
	}

	void Step() override
	{
		Sample::Step();
	}

	static Sample* Create( SampleContext* context )
	{
		return new DamBreak( context );
	}
};

static int sampleDamBreak = RegisterSample( "Particle", "Dam Break", DamBreak::Create );

class RigidParticles : public Sample
{
public:
	explicit RigidParticles( SampleContext* context )
		: Sample( context )
	{
		if ( m_context->restart == false )
		{
			m_context->camera.m_center = { 0.0f, 0.0f };
			m_context->camera.m_zoom = 25.0f * 0.45f;
		}

		{
			b2BodyDef bodyDef = b2DefaultBodyDef();
			m_groundBodyId = b2CreateBody( m_worldId, &bodyDef );

			{
				const b2Vec2 vertices[4] = {
					b2Vec2(-4, -1),
					b2Vec2(4, -1),
					b2Vec2(4, 0),
					b2Vec2(-4, 0)};
				b2Hull hull = b2ComputeHull(vertices, 4);
				b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
				b2ShapeDef def = b2DefaultShapeDef();
				b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			}

			{
				const b2Vec2 vertices[4] = {
					b2Vec2(-4, -0.1f),
					b2Vec2(-2, -0.1f),
					b2Vec2(-2, 2),
					b2Vec2(-4, 2)};
				b2Hull hull = b2ComputeHull(vertices, 4);
				b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
				b2ShapeDef def = b2DefaultShapeDef();
				b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			}

			{
				const b2Vec2 vertices[4] = {
					b2Vec2(2, -0.1f),
					b2Vec2(4, -0.1f),
					b2Vec2(4, 2),
					b2Vec2(2, 2)};
				b2Hull hull = b2ComputeHull(vertices, 4);
				b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
				b2ShapeDef def = b2DefaultShapeDef();
				b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			}
		}

		b2ParticleSystemDef particleSystemDef; 
		b2ParticleSystem* m_particleSystem = b2CreateParticleSystem(m_worldId, &particleSystemDef);
		m_particleSystem->SetRadius(0.035f);

		{
			b2Circle circle{{0, 3}, 0.5f};
			b2ShapeDef def = b2DefaultShapeDef();
			b2ShapeId shape = b2CreateCircleShape(m_groundBodyId, &def, &circle);
			b2ParticleGroupDef pd;
			// pd.flags = b2_waterParticle;
			pd.groupFlags = b2_rigidParticleGroup | b2_solidParticleGroup;
			pd.shape = shape;
			pd.color.Set(255, 0, 0, 255);
			m_particleSystem->CreateParticleGroup(pd);
			b2DestroyShape(shape, true);
		}

		{
			b2Circle circle{{-1, 3}, 0.5f};
			b2ShapeDef def = b2DefaultShapeDef();
			b2ShapeId shape = b2CreateCircleShape(m_groundBodyId, &def, &circle);
			b2ParticleGroupDef pd;
			pd.groupFlags = b2_rigidParticleGroup | b2_solidParticleGroup;
			pd.shape = shape;
			pd.color.Set(0, 255, 0, 255);
			m_particleSystem->CreateParticleGroup(pd);
			b2DestroyShape(shape, true);
		}

		{
			b2Polygon polygon = b2MakeBox(1, 0.5f);
			b2ShapeDef def = b2DefaultShapeDef();
			b2ShapeId shape = b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			b2ParticleGroupDef pd;
			pd.groupFlags = b2_rigidParticleGroup | b2_solidParticleGroup;
			pd.position = b2Vec2(1, 4);
			pd.angle = -0.5f;
			pd.angularVelocity = 2.0f;
			pd.shape = shape;
			pd.color.Set(0, 0, 255, 255);
			m_particleSystem->CreateParticleGroup(pd);
			b2DestroyShape(shape, true);
		}

		{
			b2BodyDef bd = b2DefaultBodyDef();
			bd.type = b2_dynamicBody;
			b2BodyId body = b2CreateBody(m_worldId, &bd);
			b2Circle circle{{0, 8}, 0.5f};
			b2ShapeDef def = b2DefaultShapeDef();
			b2CreateCircleShape(body, &def, &circle);
		}
	}

	void UpdateGui() override
	{
		float height = 100.0f;
		ImGui::SetNextWindowPos( ImVec2( 10.0f, m_context->camera.m_height - height - 50.0f ), ImGuiCond_Once );
		ImGui::SetNextWindowSize( ImVec2( 240.0f, height ) );

		ImGui::Begin( "Rigid Particles", nullptr, ImGuiWindowFlags_NoResize );
		ImGui::End();
	}

	void Step() override
	{
		Sample::Step();
	}

	static Sample* Create( SampleContext* context )
	{
		return new RigidParticles( context );
	}
};

static int sampleRigidParticles = RegisterSample( "Particle", "Rigid Particles", RigidParticles::Create );

class ElasticParticles : public Sample
{
public:
	explicit ElasticParticles( SampleContext* context )
		: Sample( context )
	{
		if ( m_context->restart == false )
		{
			m_context->camera.m_center = { 0.0f, 0.0f };
			m_context->camera.m_zoom = 25.0f * 0.45f;
		}

		{
			b2BodyDef bodyDef = b2DefaultBodyDef();
			m_groundBodyId = b2CreateBody( m_worldId, &bodyDef );

			{
				const b2Vec2 vertices[4] = {
					b2Vec2(-4, -1),
					b2Vec2(4, -1),
					b2Vec2(4, 0),
					b2Vec2(-4, 0)};
				b2Hull hull = b2ComputeHull(vertices, 4);
				b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
				b2ShapeDef def = b2DefaultShapeDef();
				b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			}

			{
				const b2Vec2 vertices[4] = {
					b2Vec2(-4, -0.1f),
					b2Vec2(-2, -0.1f),
					b2Vec2(-2, 2),
					b2Vec2(-4, 2)};
				b2Hull hull = b2ComputeHull(vertices, 4);
				b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
				b2ShapeDef def = b2DefaultShapeDef();
				b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			}

			{
				const b2Vec2 vertices[4] = {
					b2Vec2(2, -0.1f),
					b2Vec2(4, -0.1f),
					b2Vec2(4, 2),
					b2Vec2(2, 2)};
				b2Hull hull = b2ComputeHull(vertices, 4);
				b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
				b2ShapeDef def = b2DefaultShapeDef();
				b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			}
		}

		b2ParticleSystemDef particleSystemDef; 
		b2ParticleSystem* m_particleSystem = b2CreateParticleSystem(m_worldId, &particleSystemDef);
		m_particleSystem->SetRadius(0.035f);

		{
			b2Circle circle{{0, 3}, 0.5f};
			b2ShapeDef def = b2DefaultShapeDef();
			b2ShapeId shape = b2CreateCircleShape(m_groundBodyId, &def, &circle);
			b2ParticleGroupDef pd;
			// pd.flags = b2_waterParticle;
			pd.flags = b2_springParticle;
			pd.groupFlags = b2_solidParticleGroup;
			pd.shape = shape;
			pd.color.Set(255, 0, 0, 255);
			m_particleSystem->CreateParticleGroup(pd);
			b2DestroyShape(shape, true);
		}

		{
			b2Circle circle{{-1, 3}, 0.5f};
			b2ShapeDef def = b2DefaultShapeDef();
			b2ShapeId shape = b2CreateCircleShape(m_groundBodyId, &def, &circle);
			b2ParticleGroupDef pd;
			pd.flags = b2_elasticParticle;
			pd.groupFlags = b2_solidParticleGroup;
			pd.shape = shape;
			pd.color.Set(0, 255, 0, 255);
			m_particleSystem->CreateParticleGroup(pd);
			b2DestroyShape(shape, true);
		}

		{
			b2Polygon polygon = b2MakeBox(1, 0.5f);
			b2ShapeDef def = b2DefaultShapeDef();
			b2ShapeId shape = b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			b2ParticleGroupDef pd;
			pd.flags = b2_elasticParticle;
			pd.groupFlags = b2_solidParticleGroup;
			pd.position = b2Vec2(1, 4);
			pd.angle = -0.5f;
			pd.angularVelocity = 2.0f;
			pd.shape = shape;
			pd.color.Set(0, 0, 255, 255);
			m_particleSystem->CreateParticleGroup(pd);
			b2DestroyShape(shape, true);
		}

		{
			b2BodyDef bd = b2DefaultBodyDef();
			bd.type = b2_dynamicBody;
			b2BodyId body = b2CreateBody(m_worldId, &bd);
			b2Circle circle{{0, 8}, 0.5f};
			b2ShapeDef def = b2DefaultShapeDef();
			b2CreateCircleShape(body, &def, &circle);
		}
	}

	void UpdateGui() override
	{
		float height = 100.0f;
		ImGui::SetNextWindowPos( ImVec2( 10.0f, m_context->camera.m_height - height - 50.0f ), ImGuiCond_Once );
		ImGui::SetNextWindowSize( ImVec2( 240.0f, height ) );

		ImGui::Begin( "Elastic Particles", nullptr, ImGuiWindowFlags_NoResize );
		ImGui::End();
	}

	void Step() override
	{
		Sample::Step();
	}

	static Sample* Create( SampleContext* context )
	{
		return new ElasticParticles( context );
	}
};

static int sampleElasticParticles = RegisterSample( "Particle", "Elastic Particles", ElasticParticles::Create );

class ParticlesSurfaceTension : public Sample
{
public:
	explicit ParticlesSurfaceTension( SampleContext* context )
		: Sample( context )
	{
		if ( m_context->restart == false )
		{
			m_context->camera.m_center = { 0.0f, 0.0f };
			m_context->camera.m_zoom = 25.0f * 0.45f;
		}

		{
			b2BodyDef bodyDef = b2DefaultBodyDef();
			m_groundBodyId = b2CreateBody( m_worldId, &bodyDef );

			{
				const b2Vec2 vertices[4] = {
					b2Vec2(-4, -1),
					b2Vec2(4, -1),
					b2Vec2(4, 0),
					b2Vec2(-4, 0)};
				b2Hull hull = b2ComputeHull(vertices, 4);
				b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
				b2ShapeDef def = b2DefaultShapeDef();
				b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			}

			{
				const b2Vec2 vertices[4] = {
					b2Vec2(-4, -0.1f),
					b2Vec2(-2, -0.1f),
					b2Vec2(-2, 2),
					b2Vec2(-4, 2)};
				b2Hull hull = b2ComputeHull(vertices, 4);
				b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
				b2ShapeDef def = b2DefaultShapeDef();
				b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			}

			{
				const b2Vec2 vertices[4] = {
					b2Vec2(2, -0.1f),
					b2Vec2(4, -0.1f),
					b2Vec2(4, 2),
					b2Vec2(2, 2)};
				b2Hull hull = b2ComputeHull(vertices, 4);
				b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
				b2ShapeDef def = b2DefaultShapeDef();
				b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			}
		}

		b2ParticleSystemDef particleSystemDef; 
		b2ParticleSystem* m_particleSystem = b2CreateParticleSystem(m_worldId, &particleSystemDef);
		m_particleSystem->SetRadius(0.035f);

		{
			b2Circle circle{{0, 2}, 0.5f};
			b2ShapeDef def = b2DefaultShapeDef();
			b2ShapeId shape = b2CreateCircleShape(m_groundBodyId, &def, &circle);
			b2ParticleGroupDef pd;
			// pd.flags = b2_waterParticle;
			pd.flags = b2_tensileParticle | b2_colorMixingParticle;
			pd.shape = shape;
			pd.color.Set(255, 0, 0, 255);
			m_particleSystem->CreateParticleGroup(pd);
			b2DestroyShape(shape, true);
		}

		{
			b2Circle circle{{-1, 2}, 0.5f};
			b2ShapeDef def = b2DefaultShapeDef();
			b2ShapeId shape = b2CreateCircleShape(m_groundBodyId, &def, &circle);
			b2ParticleGroupDef pd;
			pd.flags = b2_tensileParticle | b2_colorMixingParticle;
			pd.shape = shape;
			pd.color.Set(0, 255, 0, 255);
			m_particleSystem->CreateParticleGroup(pd);
			b2DestroyShape(shape, true);
		}

		{
			b2Polygon polygon = b2MakeBox(1, 0.5f);
			b2ShapeDef def = b2DefaultShapeDef();
			b2ShapeId shape = b2CreatePolygonShape(m_groundBodyId, &def, &polygon);
			b2ParticleGroupDef pd;
			pd.flags = b2_tensileParticle | b2_colorMixingParticle;
			pd.position = b2Vec2(1, 4);
			pd.angle = -0.5f;
			pd.angularVelocity = 2.0f;
			pd.shape = shape;
			pd.color.Set(0, 0, 255, 255);
			m_particleSystem->CreateParticleGroup(pd);
			b2DestroyShape(shape, true);
		}

		{
			b2BodyDef bd = b2DefaultBodyDef();
			bd.type = b2_dynamicBody;
			b2BodyId body = b2CreateBody(m_worldId, &bd);
			b2Circle circle{{0, 8}, 0.5f};
			b2ShapeDef def = b2DefaultShapeDef();
			b2CreateCircleShape(body, &def, &circle);
		}
	}

	void UpdateGui() override
	{
		float height = 100.0f;
		ImGui::SetNextWindowPos( ImVec2( 10.0f, m_context->camera.m_height - height - 50.0f ), ImGuiCond_Once );
		ImGui::SetNextWindowSize( ImVec2( 240.0f, height ) );

		ImGui::Begin( "Elastic Particles", nullptr, ImGuiWindowFlags_NoResize );
		ImGui::End();
	}

	void Step() override
	{
		Sample::Step();
	}

	static Sample* Create( SampleContext* context )
	{
		return new ParticlesSurfaceTension( context );
	}
};

static int sampleParticlesSurfaceTension = RegisterSample( "Particle", "Surface Tension", ParticlesSurfaceTension::Create );
