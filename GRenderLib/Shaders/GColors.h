#pragma once
#ifndef GCOLORS_H
#define GCOLORS_H

#include <GMath.h>
using GMath::areEqual;

//struct float[4] = red, green, blue, alpha
struct GColor
{
	float red, green, blue, alpha;
	GColor()										: red(.0f),green(.0f),blue(.0f),alpha(1.0f)	{}
	GColor(float r, float g, float b, float a=1.0f)	: red(r),green(g),blue(b),alpha(a)			{}
	void setValues(float r, float g, float b, float a=1.0f)		{red =r; green=g; blue=b; alpha=a;}

	bool operator == (const GColor& other) const
	{
		return (areEqual(red, other.red) && areEqual(green, other.green) && areEqual(blue, other.blue) && areEqual(alpha, other.alpha) );
	}
};
namespace GColors
{
	const GColor AliceBlue			  (0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f);
    const GColor AntiqueWhite		  (0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f);
    const GColor Aqua				  (0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f);
    const GColor Aquamarine           (0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f);
    const GColor Azure                (0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f);
    const GColor Beige                (0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f);
    const GColor Bisque               (1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f);
    const GColor Black                (0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f);
    const GColor BlanchedAlmond       (1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f);
    const GColor Blue                 (0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f);
    const GColor BlueViolet           (0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f);
    const GColor Brown                (0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f);
    const GColor BurlyWood            (0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f);
    const GColor CadetBlue            (0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f);
    const GColor Chartreuse           (0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f);
    const GColor Chocolate            (0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f);
    const GColor Coral                (1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f);
    const GColor CornflowerBlue       (0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f);
    const GColor Cornsilk             (1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f);
    const GColor Crimson              (0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f);
    const GColor Cyan                 (0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f);
    const GColor DarkBlue             (0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f);
    const GColor DarkCyan             (0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f);
    const GColor DarkGoldenrod        (0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f);
    const GColor DarkGray             (0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f);
    const GColor DarkGreen            (0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f);
    const GColor DarkKhaki            (0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f);
    const GColor DarkMagenta          (0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f);
    const GColor DarkOliveGreen       (0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f);
    const GColor DarkOrange           (1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f);
    const GColor DarkOrchid           (0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f);
    const GColor DarkRed              (0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f);
    const GColor DarkSalmon           (0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f);
    const GColor DarkSeaGreen         (0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f);
    const GColor DarkSlateBlue        (0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f);
    const GColor DarkSlateGray        (0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f);
    const GColor DarkTurquoise        (0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f);
    const GColor DarkViolet           (0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f);
    const GColor DeepPink             (1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f);
    const GColor DeepSkyBlue          (0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f);
    const GColor DimGray              (0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f);
    const GColor DodgerBlue           (0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f);
    const GColor Firebrick            (0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f);
    const GColor FloralWhite          (1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f);
    const GColor ForestGreen          (0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f);
    const GColor Fuchsia              (1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f);
    const GColor Gainsboro            (0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f);
    const GColor GhostWhite           (0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f);
    const GColor Gold                 (1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f);
    const GColor Goldenrod            (0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f);
    const GColor Gray                 (0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f);
    const GColor Green                (0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f);
    const GColor GreenYellow          (0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f);
    const GColor Honeydew             (0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f);
    const GColor HotPink              (1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f);
    const GColor IndianRed            (0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f);
    const GColor Indigo               (0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f);
    const GColor Ivory                (1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f);
    const GColor Khaki                (0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f);
    const GColor Lavender             (0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f);
    const GColor LavenderBlush        (1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f);
    const GColor LawnGreen            (0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f);
    const GColor LemonChiffon         (1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f);
    const GColor LightBlue            (0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f);
    const GColor LightCoral           (0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f);
    const GColor LightCyan            (0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f);
    const GColor LightGoldenrodYellow (0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f);
    const GColor LightGreen           (0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f);
    const GColor LightGray            (0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f);
    const GColor LightPink            (1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f);
    const GColor LightSalmon          (1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f);
    const GColor LightSeaGreen        (0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f);
    const GColor LightSkyBlue         (0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f);
    const GColor LightSlateGray       (0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f);
    const GColor LightSteelBlue       (0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f);
    const GColor LightYellow          (1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f);
    const GColor Lime                 (0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f);
    const GColor LimeGreen            (0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f);
    const GColor Linen                (0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f);
    const GColor Magenta              (1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f);
    const GColor Maroon               (0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f);
    const GColor MediumAquamarine     (0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f);
    const GColor MediumBlue           (0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f);
    const GColor MediumOrchid         (0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f);
    const GColor MediumPurple         (0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f);
    const GColor MediumSeaGreen       (0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f);
    const GColor MediumSlateBlue      (0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f);
    const GColor MediumSpringGreen    (0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f);
    const GColor MediumTurquoise      (0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f);
    const GColor MediumVioletRed      (0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f);
    const GColor MidnightBlue         (0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f);
    const GColor MintCream            (0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f);
    const GColor MistyRose            (1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f);
    const GColor Moccasin             (1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f);
    const GColor NavajoWhite          (1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f);
    const GColor Navy                 (0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f);
    const GColor OldLace              (0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f);
    const GColor Olive                (0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f);
    const GColor OliveDrab            (0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f);
    const GColor Orange               (1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f);
    const GColor OrangeRed            (1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f);
    const GColor Orchid               (0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f);
    const GColor PaleGoldenrod        (0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f);
    const GColor PaleGreen            (0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f);
    const GColor PaleTurquoise        (0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f);
    const GColor PaleVioletRed        (0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f);
    const GColor PapayaWhip           (1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f);
    const GColor PeachPuff            (1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f);
    const GColor Peru                 (0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f);
    const GColor Pink                 (1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f);
    const GColor Plum                 (0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f);
    const GColor PowderBlue           (0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f);
    const GColor Purple               (0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f);
    const GColor Red                  (1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f);
    const GColor RosyBrown            (0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f);
    const GColor RoyalBlue            (0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f);
    const GColor SaddleBrown          (0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f);
    const GColor Salmon               (0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f);
    const GColor SandyBrown           (0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f);
    const GColor SeaGreen             (0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f);
    const GColor SeaShell             (1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f);
    const GColor Sienna               (0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f);
    const GColor Silver               (0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f);
    const GColor SkyBlue              (0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f);
    const GColor SlateBlue            (0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f);
    const GColor SlateGray            (0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f);
    const GColor Snow                 (1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f);
    const GColor SpringGreen          (0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f);
    const GColor SteelBlue            (0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f);
    const GColor Tan                  (0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f);
    const GColor Teal                 (0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f);
    const GColor Thistle              (0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f);
    const GColor Tomato               (1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f);
    const GColor Transparent          (0.000000000f, 0.000000000f, 0.000000000f, 0.000000000f);
    const GColor Turquoise            (0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f);
    const GColor Violet               (0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f);
    const GColor Wheat                (0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f);
    const GColor White                (1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f);
    const GColor WhiteSmoke           (0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f);
    const GColor Yellow               (1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f);
    const GColor YellowGreen          (0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f);

}	//EO namespace GColors

#endif	//GCOLORS_H