#include "GAtmosphere.h"

using namespace GMath;
using namespace GPhysics;

const Real GAtmosphere::ro_rate = castReal<float>(0.5f / 6668.0f);

// { return GPhysicConstants::ro0 - (altitude*ro_rate*GPhysicConstants::ro0); }
// altitute == 6668m => altitude*ro_rate == 0.5f
