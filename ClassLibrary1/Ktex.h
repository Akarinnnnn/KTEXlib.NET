#pragma once
#include <TEXFileOperation.h>
using namespace System;
using namespace System::Windows::Media::Imaging;
namespace KTEXLib_NET 
{
	//KTEX exception will not caught by .NET framework
	public enum class  platfrm//platform
	{
		opengl = 12,
		xb360 = 11,
		ps3 = 10,
		unk = 0
	};
	public enum class pixfrm //pixel form
	{
		ARGB = 4,
		DXT1 = 0,
		DXT3 = 1,
		DXT5 = 2,
		unk = 7
	};
	public enum class textyp //texture type
	{
		d1 = 1,//1d
		d2 = 2,//2d
		d3 = 3,//3d
		cube = 4//cubemap
	};
	public ref struct KTEXinfo
	{
		pixfrm PixelForm = pixfrm::DXT5;
		platfrm PlatForm = platfrm::opengl;
		textyp TextureType = textyp::d1;
		UInt16 MipMapCount = 1;
		Byte Flags = 0;
	};
	public ref class KTEX
	{
	public:
		KTEX();
		~KTEX();
		void load(String^ KTEXpath);
		KTEX(System::String^ KTEXPath);
		void push_back(BitmapImage^ Image);
		void push(BitmapImage^ Image, UInt16 pitch);
		BitmapImage^ operator[](UInt16 i);
		BitmapImage^ MainMipmap();
		void Convert();
	private:
		ktexlib::KTEXFileOperation::KTEX* native;
	};
	using KTEXList = System::Collections::Generic::List<KTEX^>;
}
