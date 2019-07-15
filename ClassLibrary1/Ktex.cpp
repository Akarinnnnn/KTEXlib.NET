#include "stdafx.h"
#include "Ktex.h"
using namespace System;
using namespace ktexlib;
using namespace ktexlib::NET;
using namespace System::Windows::Media::Imaging;
using ktex_iterator = System::Collections::Generic::IEnumerator<BitmapSource^>^;
using System::Collections::IEnumerator;

inline wchar_t* ms2wcs(String^ ms)
{
	auto wcs = new wchar_t[ms->Length + 1];
	for (int i = 0; i < ms->Length; i++)
	{
		wcs[i] = ms[i];
	}
	wcs[ms->Length] = 0;
	return wcs;
}

inline ktexlib::KTEXFileOperation::RGBAv2 wpf2rgba(BitmapSource ^ img)
{
	using namespace ktexlib::KTEXFileOperation;
	using namespace System::Windows::Media;
	auto mdata = gcnew cli::array<Byte> { 0 };
	RGBAv2 ret =
	{
		img->PixelWidth,
		img->PixelHeight
	};
	int buffsize = img->PixelHeight * img->PixelWidth * (img->Format.BitsPerPixel / 8);
	int rgba_size = img->PixelHeight * img->PixelWidth * 4;
	ret.data.resize(rgba_size);
	//分配缓冲区，复制
	unsigned char* buffier = new unsigned char[buffsize]{ 0 };
	img->CopyPixels(Windows::Int32Rect::Empty, IntPtr(buffier), buffsize, img->PixelWidth * 4);

	if (img->Format == PixelFormats::Bgr32 || img->Format == PixelFormats::Bgra32)
	{
		for (int i = 0; i < buffsize; i += 4)
		{
			std::swap(*(buffier + i), *(buffier + i + 3));
		}
		ret.data.assign(buffier, buffier + buffsize - 1);
		delete[] buffier;
		return ret;
	}
	
	if (img->Format == PixelFormats::Bgr24)
	{
		auto rgba = ret.data.data();

		if (buffsize <= 3)
			throw gcnew Exception("buffsize<3, is " + buffsize.ToString());
		for (int i = 0, j = 0; (i < buffsize) && (j < rgba_size); i += 3, j += 4)
		{
			rgba[j]		= buffier[i + 2];
			rgba[j + 1] = buffier[i + 1];
			rgba[j + 2] = buffier[i];
		}
		delete[] buffier;
		return ret;

	}

	if (img->Format == PixelFormats::Rgb24)
	{
		auto rgba = ret.data.data();

		if (buffsize <= 3)
			throw gcnew Exception("buffsize<3, is " + buffsize.ToString());
		for (int i = 0, j = 0; (i < buffsize) && (j < rgba_size); i += 3, j += 4)
		{
			rgba[j] = buffier[i];
			rgba[j + 1] = buffier[i + 1];
			rgba[j + 2] = buffier[i + 2];
		}
		delete[] buffier;
		return ret;
	}

	{
		delete[] buffier;
		throw gcnew NotSupportedException("Not Supported Format:" + img->Format.ToString());
	}
}

inline BitmapSource^ rgba2wpf(ktexlib::KTEXFileOperation::RGBAv2 src)
{
	using namespace System::IO;
	using namespace System::Windows::Media;
	using namespace System::Drawing;
	auto hbitmap = (gcnew System::Drawing::Bitmap(
		src.width,
		src.height,
		src.width * src.height * 4,
		System::Drawing::Imaging::PixelFormat::Canonical,
		IntPtr(
			src.data.data()
		)
	))->GetHbitmap();
	return System::Windows::Interop::Imaging::CreateBitmapSourceFromHBitmap(hbitmap, IntPtr::Zero, Windows::Int32Rect::Empty,
		BitmapSizeOptions::FromEmptyOptions());
}

ktexlib::NET::Ktex::Ktex()
{
	native = new ktexlib::KTEXFileOperation::KTEX();
}

ktexlib::NET::Ktex::~Ktex()
{
	delete native;
}

void ktexlib::NET::Ktex::Load(String^ KTEXpath)
{
	try
	{
		auto path = ms2wcs(KTEXpath);
		native->LoadKTEX(path);
		delete[] path;
	}
	catch (const KTEXFileOperation::ktex_exception& e)
	{
		throw gcnew KTEXException(std::move(e));
	}
}

ktexlib::NET::Ktex::Ktex(String^ KTEXPath)
{
	native = new ktexlib::KTEXFileOperation::KTEX();

	try
	{
		auto path = ms2wcs(KTEXPath);
		native->LoadKTEX(path);
		delete[] path;
	}
	catch (const KTEXFileOperation::ktex_exception& e)
	{
		throw gcnew KTEXException(e);
	}
}

void ktexlib::NET::Ktex::Append(BitmapImage ^ Image)
{
	auto tgt = wpf2rgba(Image);
	native->PushRGBA(tgt);
}

void ktexlib::NET::Ktex::AddAt(BitmapImage^ Image, System::UInt16 pitch)
{
	auto tgt = wpf2rgba(Image);
	native->PushRGBA(tgt, pitch);
}

BitmapSource^ ktexlib::NET::Ktex::indexer::get(unsigned short i)
{
	return rgba2wpf(*(*native)[i]);
}

BitmapSource ^ ktexlib::NET::Ktex::MainMipmap()
{
	return rgba2wpf(native->GetImageFromMipmap(0));
}

void ktexlib::NET::Ktex::Convert()
{
	try
	{
		native->Convert();
	}
	catch (const std::out_of_range& e)
	{
		throw gcnew System::IndexOutOfRangeException(gcnew String(e.what()) + native->Info.mipscount);
	}
	catch (const std::invalid_argument& e)
	{
		throw gcnew System::ArgumentException(gcnew String(e.what()));
	}
	catch (const KTEXFileOperation::ktex_exception& e)
	{
		throw gcnew KTEXException(e);
	}
}

ktex_iterator ktexlib::NET::Ktex::GetEnumerator()
{
	return gcnew KtexEnumerator(this);
}

System::Collections::IEnumerator^ ktexlib::NET::Ktex::_begin()
{
	return gcnew KtexEnumerator(this);
}

System::String^ ktexlib::NET::Ktex::ToString()
{
	return Info->ToString();
}

ktexlib::NET::KTEXException::KTEXException(KTEXFileOperation::ktex_exception e) : native(e)
{

}

String^ ktexlib::NET::KTEXException::Message::get()
{
	return gcnew String(native.what());
}

unsigned int KTEXException::KtexErrorID::get()
{
	return native.code();
}

ktexlib::NET::Ktex::KtexEnumerator::KtexEnumerator(Ktex^ parent)
{
	this->parent = parent;
}

ktexlib::NET::Ktex::KtexEnumerator::~KtexEnumerator()
{
	
}

bool ktexlib::NET::Ktex::KtexEnumerator::MoveNext()
{
	if (i <= parent->native->Info.mipscount)
	{
		i++;
		return true;
	}
	else
	{
		return false;
	}
}

void ktexlib::NET::Ktex::KtexEnumerator::Reset()
{
	this->i = 0;
}


BitmapSource^ ktexlib::NET::Ktex::KtexEnumerator::Current::get()
{
	if (i == 0)
	{
		throw gcnew InvalidOperationException(gcnew String(L"枚举未开始，请先调用MoveNext"));
	}
	auto rgba = parent->native->GetImageFromMipmap(i);
	return rgba2wpf(rgba);
}

Object^ ktexlib::NET::Ktex::KtexEnumerator::cur_nongeneric::get()
{
	return Current;
}

System::String^ ktexlib::NET::KtexInfo::ToString()
{
	return String::Format(
		L"Mipmaps Count:{0}\n" +
		L"Pixel Format:{1}\n" +
		L"Platform:{2}\n" +
		L"Texture Type:{3}\n" +
		L"Flags:{4}", MipMapCount, PixelFormat, PlatForm, TextureType, Convert::ToString(this->Flags, 2));
}

String^ Ktex::OutPut::get()
{
	return gcnew String(native->output.c_str());
}

void Ktex::OutPut::set(String^ path)
{
	auto opt = ms2wcs(path);
	native->output = opt;
	delete opt;
}

KtexInfo^ Ktex::Info::get()
{
	auto ret = gcnew KtexInfo();
	ret->PixelFormat	= PixelFromat(native->Info.pixelformat);
	ret->PlatForm		= Platform(native->Info.platform);
	ret->TextureType	= TextureType(native->Info.texturetype);
	ret->Flags			= native->Info.flags;
	ret->MipMapCount	= native->Info.mipscount;
	return ret;
}
void Ktex::Info::set(KtexInfo^ v)
{
	using namespace KTEXFileOperation;
	auto& n = native->Info;
	n.flags			= v->Flags;
	n.mipscount		= v->MipMapCount;
	n.platform		= platfrm(v->PlatForm);
	n.pixelformat	= pixfrm(v->PixelFormat);
	n.texturetype	= textyp(v->TextureType);
}
