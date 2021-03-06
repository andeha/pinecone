//
//  MathTests.cpp
//  Pinecone Unit Tests
//

#include <Pinecone.h>
#include <Additions/Helpers.h>
#include <Additions/Performance.h>

INNER_DATA double epsilon = 0.001;

UNITTEST(Relative_1)
{
    double y = Relative<double>::arithmeticBetween(1.0, 2.0);
    ENSURE(Similar(y, 1.5, epsilon), "arithmeticBetween failed");
    Relative<double> l = Relative<double>::leftChange(1.0, 2.0);
    ENSURE(Similar(l.value, -.5, epsilon), "leftChange failed");
    Relative<double> r = Relative<double>::rightChange(1.0, 2.0);
    ENSURE(Similar(r.value, 1.0, epsilon), "rightChange failed");
    
    //static T Relative<double>::diff(T x₁, T x₂) { return x₁ - x₂; }
    //static T dist(T x₁, T x₂, T (^N)(T x, T y)) { return N(x₁, x₂); }
    // y = Relative<double>::dist(l.value, r.value, ^(double x, double y) { return sqrt(x*x + y*y); });
    
    y = Relative<double>::max(1.0, 2.0);
    ENSURE(Similar(y, 2.0, epsilon), "max 1 failed");
    y = Relative<double>::max(2.0, 1.0);
    ENSURE(Similar(y, 2.0, epsilon), "max 2 failed");
    
    y = Relative<double>::min(1.0, 2.0);
    ENSURE(Similar(y, 1.0, epsilon), "min 1 failed");
    y = Relative<double>::min(2.0, 1.0);
    ENSURE(Similar(y, 1.0, epsilon), "min 2 failed");
    
    bool b = Relative<int>::eqrel(1, 2);
    ENSURE(!b, "eqrel 1 failed");
    b = Relative<int>::eqrel(2, 2);
    ENSURE(b, "eqrel 2 failed");
    
    b = Relative<int>::eqeql(1, 2);
    ENSURE(!b, "eqeql 1 failed");
    b = Relative<int>::eqeql(2, 2);
    ENSURE(b, "eqeql 2 failed");
}

UNITTEST(Abs_1)
{
    double y = abs64d(1.0);
    ENSURE(Similar(y, 1.0, epsilon), "abs64d 1 failed");
    y = abs64d(-y);
    ENSURE(Similar(y, 1.0, epsilon), "abs64d 2 failed");
}

UNITTEST(Abs_2)
{
    float y = abs32s(1.0);
    ENSURE(Similar(y, 1.0, epsilon), "abs32s 1 failed");
    y = abs32s(-y);
    ENSURE(Similar(y, 1.0, epsilon), "abs32s 2 failed");
}

UNITTEST(Abs_3)
{
    float y = abs(1.0);
    ENSURE(Similar(y, 1.0, epsilon), "abs 1 failed");
    y = abs(-y);
    ENSURE(Similar(y, 1.0, epsilon), "abs 2 failed");
}

UNITTEST(Inf_1)
{
    bool b = isinf(1.0);
    ENSURE(!b, "isinf failed");
    b = isnan(1.0);
    ENSURE(!b, "isnan failed");
    b = isnegone(1.0);
    ENSURE(!b, "isnegone failed");
}

UNITTEST(Nan_1)
{
    bool b = isinf(1.0/0.0);
    ENSURE(b, "isinf failed");
}

UNITTEST(Neg_1)
{
    bool b = isinf(1.0);
    b = isnan(1.0);
    b = isnegone(1.0);
}

UNITTEST(Modf_1)
{
    double fract;
    double integer = modf(3.14, &fract);
    ENSURE(Similar(integer, 3, epsilon), "Error in Modf_1 1");
    ENSURE(Similar(fract, 0.14, epsilon), "Error in Modf_1 1");
}

UNITTEST(Exp_1)
{
    double y = exp(0.0);
    ENSURE(Similar(y, 1.0, epsilon), "Error in Exp_1 1");
    y = exp(1.0);
    ENSURE(Similar(y, 2.71828, epsilon), "Error in Exp_1 2");
    y = exp(3.14);
    ENSURE(Similar(y, 23.1039, epsilon), "Error in Exp_1 3");
}

UNITTEST(Ln_1)
{
    double y = ln(1.0);
    ENSURE(Similar(y, 0.0, epsilon), "Error in Ln_1 1");
    y = ln(3.14);
    ENSURE(Similar(y, 1.14422, epsilon), "Error in Ln_1 2");
    // double y = ln(0.0);
    // ENSURE(Similar(y, 1.0, epsilon), "Error in Ln_1 3");
}

UNITTEST(Pow_1)
{
    double y = pow(1.0, 0.0);
    ENSURE(Similar(y, 1.0, epsilon), "Error in Pow_1 1");
    y = pow(1.0, 1.0);
    ENSURE(Similar(y, 1.0, epsilon), "Error in Pow_1 2");
    y = pow(1.0, 2.0);
    ENSURE(Similar(y, 1.0, epsilon), "Error in Pow_1 3");
    y = pow(2.0, 1.0);
    ENSURE(Similar(y, 2.0, epsilon), "Error in Pow_1 3");
    y = pow(2.0, 2.0);
    ENSURE(Similar(y, 4.0, epsilon), "Error in Pow_1 3");
}

UNITTEST(Sqrt_1)
{
    double y = sqrt(0.0);
    ENSURE(Similar(y, 0.0, epsilon), "Error in Sqrt_1 1");
    y = sqrt(1.0);
    ENSURE(Similar(y, 1.0, epsilon), "Error in Sqrt_1 2");
    y = sqrt(2.0);
    ENSURE(Similar(y, 1.41421, epsilon), "Error in Sqrt_1 3");
    y = sqrt(3.0);
    ENSURE(Similar(y, 1.73205, epsilon), "Error in Sqrt_1 4");
}

MACRO
INNER_FUNCTION
float rsqrt_alt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int*)&x;            // store floating-point bits in integer
    i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
    x = *(float*)&i;              // convert new bits into float
    x = x*(1.5f - xhalf*x*x);     // One round of Newton's method
    return x;
}

UNITTEST(RSqrt_1)
{
    MEASURE_START(SSE_RSQRT)
    float x = rsqrt(10);
    MEASURE_END(SSE_RSQRT)
    
    MEASURE_START(SEQUENTIAL_RSQRT)
    float y = rsqrt_alt(10);
    MEASURE_END(SEQUENTIAL_RSQRT)
    
    ENSURE(Similar(x - y, 0.0, epsilon), "Error in RSqrt_1");
}

// #include <fenv.h>

MACRO
INNER_FUNCTION
__builtin_int_t SSE_CastNFloor(double d)
{
    // SSE rounding mode defaults to "round to nearest"
    uint32_t csr = _mm_getcsr();
    _mm_setcsr((csr & ~(_MM_ROUND_MASK)) | _MM_ROUND_DOWN);
    __m128d x = { d, d };
    int64_t result = _mm_cvtsd_si64(x);
    _mm_setcsr(csr);
    return result;
}

INNER_FUNCTION
__attribute__((target("avx2")))
__attribute__((target("avx512f")))
__builtin_int_t AVX512_CastNFloor(double d)
{
    __m128d x = { d, d };
    return _mm_cvt_roundsd_i64(x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
}

MACRO
INNER_FUNCTION
__builtin_int_t Language_RoundToNearest(double d)
{
    return d < 0 ? (__builtin_int_t)(d - 0.5) : (__builtin_int_t)(d + 0.5);
}

MACRO
INNER_FUNCTION
__builtin_int_t Language_TruncateTowardsZero(double d)
{
    return (__builtin_int_t)d; // truncate toward zero
}

UNITTEST(FloorNCast_1) // Should cast towards negative infinity.
{
    MEASURE_START(SSE_CASTNFLOOR)
    __builtin_int_t x1 = SSE_CastNFloor(Naturals::π);
    __builtin_int_t x2 = SSE_CastNFloor(Naturals::e);
    __builtin_int_t x3 = SSE_CastNFloor(-Naturals::e);
    __builtin_int_t x4 = SSE_CastNFloor(-Naturals::π);
    MEASURE_END(SSE_CASTNFLOOR)
    
    ENSURE(Similar(x1, 3.0, epsilon), "Error in  π");
    ENSURE(Similar(x2, 2.0, epsilon), "Error in  e");
    ENSURE(Similar(x3, -3.0, epsilon), "Error in -e");
    ENSURE(Similar(x4, -4.0, epsilon), "Error in -π");
    
    MEASURE_START(LANGUAGE_ROUND_TO_NEARSET)
    __builtin_int_t y1 = Language_RoundToNearest(Naturals::π);
    __builtin_int_t y2 = Language_RoundToNearest(Naturals::e);
    __builtin_int_t y3 = Language_RoundToNearest(-Naturals::e);
    __builtin_int_t y4 = Language_RoundToNearest(-Naturals::π);
    MEASURE_END(LANGUAGE_ROUND_TO_NEARSET)
    
    ENSURE(Similar(y1, 3.0, epsilon), "Error in  π");
    ENSURE(Similar(y2, 3.0, epsilon), "Error in  e");
    ENSURE(Similar(y3, -3.0, epsilon), "Error in -e");
    ENSURE(Similar(y4, -3.0, epsilon), "Error in -π");
}

UNITTEST(Arctan_1)
{
    double y = arctan(0.0);
    ENSURE(Similar(y, 0.0, epsilon), "Error in Arctan_1 1");
    y = arctan(1.0);
    ENSURE(Similar(y, 0.785398, epsilon), "Error in Arctan_1 2");
    y = arctan(-1.0);
    ENSURE(Similar(y, -0.785398, epsilon), "Error in Arctan_1 3");
}

UNITTEST(Sincos_1)
{
    double s, c;
    sincos(0.0, &s, &c);
    ENSURE(Similar(s, 0.0, epsilon), "Error in Sincos_1 1");
    ENSURE(Similar(c, 1.0, epsilon), "Error in Sincos_1 2");
    sincos(Naturals::π, &s, &c);
    ENSURE(Similar(s, 0.0, epsilon), "Error in Sincos_1 3");
    ENSURE(Similar(c, -1.0, epsilon), "Error in Sincos_1 4");
    sincos(Naturals::π/2, &s, &c);
    ENSURE(Similar(s, 1.0, epsilon), "Error in Sincos_1 5");
    ENSURE(Similar(c, 0.0, epsilon), "Error in Sincos_1 6");
    sincos(Naturals::π/4, &s, &c);
    ENSURE(Similar(s, 0.707107, epsilon), "Error in Sincos_1 7");
    ENSURE(Similar(c, 0.707107, epsilon), "Error in Sincos_1 8");
    sincos(Naturals::π/6, &s, &c);
    ENSURE(Similar(s, 0.5, epsilon), "Error in Sincos_1 9");
    ENSURE(Similar(c, 0.866025, epsilon), "Error in Sincos_1 10");
}

UNITTEST(Floor_1)
{
    double y = pinecone_floor(1.5);
    ENSURE(Similar(y, 1.0, epsilon), "Error when pinecone_floor 1");
}

