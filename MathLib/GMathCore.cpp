
#include "GMathCore.h"
#include <limits>

using namespace GMath;

const Real Constants::pi      = castReal<float>(3.14159265358979323846f);
const Real Constants::pi_half = Constants::pi/2.0f;
const Real Constants::pi_double= Constants::pi*2.0f;
const Real Constants::pi_quarter = Constants::pi/4.0f;
const Real Constants::euler   = castReal<float>(2.71828182845904523536f);
const Real Constants::epsilon = castReal<float>( std::numeric_limits<float>::min()*10.0f);
const Real Constants::approssimation = castReal<float>(0.00000000001f);

const Real Constants::millsConstant = castReal<float>( 1.30637788386308069046f );

const double Constants::doubleMin = std::numeric_limits<double>::min();
const Real Constants::realMin= castReal<float>( std::numeric_limits<float>::min() );
const Int64 Constants::int64Min= std::numeric_limits<Int64>::min();
const Int32 Constants::int32Min= std::numeric_limits<Int32>::min();
const Int16 Constants::int16Min= std::numeric_limits<Int16>::min();
const Int8  Constants::int8Min= std::numeric_limits<Int8>::min();

const double Constants::doubleMax = std::numeric_limits<double>::max();
const Real Constants::realMax = castReal<float>( std::numeric_limits<float>::max() );
const Int64 Constants::int64Max = std::numeric_limits<Int64>::max();
const Int32 Constants::int32Max = std::numeric_limits<Int32>::max();
const Int16 Constants::int16Max = std::numeric_limits<Int16>::max();
const Int8  Constants::int8Max = std::numeric_limits<Int8>::max();
const uint64 Constants::uint64Max= std::numeric_limits<uint64>::max();
const uint32 Constants::uint32Max= std::numeric_limits<uint32>::max();
const uint16 Constants::uint16Max= std::numeric_limits<uint16>::max();
const uint8 Constants::uint8Max= std::numeric_limits<uint8>::max();

