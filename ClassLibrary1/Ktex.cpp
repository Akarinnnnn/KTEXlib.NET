#include "stdafx.h"
#include "Ktex.h"

using namespace System;
using namespace KTEXLib_NET;


inline wchar_t* ms2wcs(String^ ms)
{
	auto wcs = new wchar_t[ms->Length];
	for (unsigned int i = 0; i < ms->Length; i++)
	{
		wcs[i] = ms[i];
	}
	return wcs;
}
inline ktexlib::KTEXFileOperation::RGBAv2 mimg2rgba(BitmapImage^ img)
{
	using namespace ktexlib::KTEXFileOperation;
	auto mdata = gcnew cli::array<Byte, 1>{};
	RGBAv2 ret =
	{
		img->PixelWidth,
		img->PixelHeight
	};
	__int64 size = img->StreamSource->Length;
	img->StreamSource->Write(mdata, 0, size);
	ret.data.resize(size);
	for (unsigned int i = 0; i < size; i++)
	{
		ret.data.data()[i] = mdata[i];
	}
	return ret;
}

inline BitmapImage^ rgba2mimg(ktexlib::KTEXFileOperation::RGBAv2 src)
{
	using namespace System::IO;
	using namespace System::Windows::Media;
	auto mdata = gcnew array<Byte>{};
	auto ms = gcnew MemoryStream(mdata,true);
	auto ret = gcnew BitmapImage();
	ret->BeginInit();
	ret->CacheOption = BitmapCacheOption::OnLoad;
	ret->StreamSource = ms;
	ret->EndInit();
	return ret;
}

KTEXLib_NET::KTEX::KTEX()
{
	native = new ktexlib::KTEXFileOperation::KTEX();
}

KTEXLib_NET::KTEX::~KTEX()
{
	delete native;
}

void KTEXLib_NET::KTEX::load(String^ KTEXpath)
{
	native->LoadKTEX(ms2wcs(KTEXpath));
}

KTEXLib_NET::KTEX::KTEX(String^ KTEXPath)
{
	native = new ktexlib::KTEXFileOperation::KTEX();
	native->LoadKTEX(ms2wcs(KTEXPath));
}

void KTEXLib_NET::KTEX::push_back(BitmapImage ^ Image)
{
	auto tgt = mimg2rgba(Image);
	native->PushRGBA(tgt);
}

void KTEXLib_NET::KTEX::push(BitmapImage ^ Image, UInt16 pitch)
{
	auto tgt = mimg2rgba(Image);
	native->PushRGBA(tgt, pitch);
}

BitmapImage^ KTEXLib_NET::KTEX::operator[](UInt16 i)
{
	return rgba2mimg(*(*native)[i]);
}

BitmapImage ^ KTEXLib_NET::KTEX::MainMipmap()
{
	return rgba2mimg(native->GetImageFromMipmap(0));
}

void KTEXLib_NET::KTEX::Convert()
{
	native->Convert();
}

