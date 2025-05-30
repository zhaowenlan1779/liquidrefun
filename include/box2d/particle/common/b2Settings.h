/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
* Copyright (c) 2013 Google, Inc.
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B2_SETTINGS_H
#define B2_SETTINGS_H

#include <stddef.h>
#include <assert.h>
#include <float.h>

#define B2_NOT_USED(x) ((void)(x))
#if DEBUG && !defined(NDEBUG)
#define b2Assert(A) assert(A)
#define B2_ASSERT_ENABLED 1
#else
#define b2Assert(A)
#define B2_ASSERT_ENABLED 0
#endif

// Statement which is compiled out when DEBUG isn't defined.
#if DEBUG
#define B2_DEBUG_STATEMENT(A) A
#else
#define B2_DEBUG_STATEMENT(A)
#endif  // DEBUG

// Calculate the size of a static array.
#define B2_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef signed char	int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;

#ifdef WIN32
typedef __int64   int64;
typedef unsigned __int64   uint64;
#else // !WIN32
typedef long long int64;
typedef unsigned long long uint64;
#endif

#define	b2_maxFloat		FLT_MAX
#define	b2_epsilon		FLT_EPSILON
#define b2_pi			3.14159265359f

#if !defined(b2Inline)
#if defined(__GNUC__)
#define b2Inline __attribute__((always_inline))
#else
#define b2Inline inline
#endif // defined(__GNUC__)
#endif // !defined(b2Inline)

// We expand the API so that other languages (e.g. Java) can call into
// our C++ more easily. Only set if when the flag is not externally defined.
#if !defined(LIQUIDFUN_EXTERNAL_LANGUAGE_API)
#if defined(SWIG) || defined(LIQUIDFUN_UNIT_TESTS)
#define LIQUIDFUN_EXTERNAL_LANGUAGE_API 1
#else
#define LIQUIDFUN_EXTERNAL_LANGUAGE_API 0
#endif
#endif

/// @file
/// Global tuning constants based on meters-kilograms-seconds (MKS) units.
///

// Particle

/// NEON SIMD requires 16-bit particle indices
#if !defined(B2_USE_16_BIT_PARTICLE_INDICES) && defined(LIQUIDFUN_SIMD_NEON)
#define B2_USE_16_BIT_PARTICLE_INDICES
#endif

/// A symbolic constant that stands for particle allocation error.
#define b2_invalidParticleIndex		(-1)

#ifdef B2_USE_16_BIT_PARTICLE_INDICES
#define b2_maxParticleIndex			0x7FFF
#else
#define b2_maxParticleIndex			0x7FFFFFFF
#endif

/// The default distance between particles, multiplied by the particle diameter.
#define b2_particleStride			0.75f

/// The minimum particle weight that produces pressure.
#define b2_minParticleWeight			1.0f

/// The upper limit for particle pressure.
#define b2_maxParticlePressure		0.25f

/// The upper limit for force between particles.
#define b2_maxParticleForce		0.5f

/// The maximum distance between particles in a triad, multiplied by the
/// particle diameter.
#define b2_maxTriadDistance			2
#define b2_maxTriadDistanceSquared		(b2_maxTriadDistance * b2_maxTriadDistance)

/// The initial size of particle data buffers.
#define b2_minParticleSystemBufferCapacity	256

/// The time into the future that collisions against barrier particles will be detected.
#define b2_barrierCollisionTime 2.5f

#endif
