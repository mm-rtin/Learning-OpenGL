#ifndef ANIMATION_H
#define ANIMATION_H

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                     Animation
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/
namespace TWEEN {
	enum TWEEN_TYPES {
		LINEAR,
		IN_LINEAR,
		OUT_LINEAR,
		IN_OUT_LINEAR,
		IN_QUAD,
		OUT_QUAD,
		IN_OUT_QUAD,
		IN_CUBIC,
		OUT_CUBIC,
		IN_OUT_CUBIC,
		IN_QUART,
		OUT_QUART,
		IN_OUT_QUART,
		IN_QUINT,
		OUT_QUINT,
		IN_OUT_QUINT,
		IN_EXPO,
		OUT_EXPO,
		IN_OUT_EXPO,
		IN_CIRC,
		OUT_CIRC,
		IN_OUT_CIRC,
		IN_SINE,
		OUT_SINE,
		IN_BACK,
		OUT_BACK,
		IN_OUT_BACK,
		OUT_BOUNCE,
	};
}
namespace ANIMATION {
	enum ANIMATION_TYPES {
		TRANSLATE,
		SCALE,
		ROTATE
	};
}

class Animation {

private:

public:

	/// methods
	Animation();
	~Animation();

	/// LINEAR
	static float linear (float, float, float, float);
	static float inLinear (float, float, float, float);
	static float outLinear (float, float, float, float);
	static float inOutLinear (float, float, float, float);

	/// QUAD
	static float inQuad (float, float, float, float);
	static float outQuad (float, float, float, float);
	static float inOutQuad (float, float, float, float);

	/// CUBIC
	static float inCubic (float, float, float, float);
	static float outCubic (float, float, float, float);
	static float inOutCubic (float, float, float, float);

	/// QUART
	static float inQuart (float, float, float, float);
	static float outQuart (float, float, float, float);
	static float inOutQuart (float, float, float, float);
	
	/// QUINT
	static float inQuint (float, float, float, float);
	static float outQuint (float, float, float, float);
	static float inOutQuint (float, float, float, float);

	/// EXPO
	static float inExpo (float, float, float, float);
	static float outExpo (float, float, float, float);
	static float inOutExpo (float, float, float, float);

	/// CIRC
	static float inCirc (float, float, float, float);
	static float outCirc (float, float, float, float);
	static float inOutCirc (float, float, float, float);

	/// SINE
	static float inSine (float, float, float, float);
	static float outSine (float, float, float, float);
	static float inOutSine (float, float, float, float);

	/// BACK
	static float inBack (float, float, float, float);
	static float outBack (float, float, float, float);
	static float inOutBack (float, float, float, float);

	/// BOUNCE
	static float outBounce (float, float, float, float);
};
#endif