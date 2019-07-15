#pragma once
#include <TEXFileOperation.h>

namespace ktexlib
{
	namespace NET
	{
		//native KTEX exception will not caught by .NET framework
		public ref class KTEXException : System::Exception
		{
		public:
			KTEXException(ktexlib::KTEXFileOperation::ktex_exception e);
			//message prop
			property System::String^ Message
			{
				System::String^ get() override;
			}
			property unsigned int KtexErrorID
			{
				unsigned int get();
			}
		private:
			ktexlib::KTEXFileOperation::ktex_exception& native;
		};

		public ref struct RgbaImage
		{

		};

		public enum class  Platform//platform
		{
			[System::ComponentModel::Description("OpenGL(PC)")]
			opengl = 12,
			[System::ComponentModel::Description("Xbox 360")]
			xb360 = 11,
			[System::ComponentModel::Description("PS3")]
			ps3 = 10,
			[System::ComponentModel::Description("Unkonwon Platform")]
			unk = 0
		};
		public enum class PixelFromat //pixel form
		{
			ARGB = 4,
			DXT1 = 0,
			DXT3 = 1,
			DXT5 = 2,
			unk = 7
		};
		public enum class TextureType //texture type
		{
			d1 = 1,//1d
			d2 = 2,//2d
			d3 = 3,//3d
			cube = 4//cubemap
		};
		public ref struct KtexInfo
		{
			PixelFromat PixelFormat = PixelFromat::DXT5;
			Platform PlatForm = Platform::opengl;
			TextureType TextureType = TextureType::d2;
			unsigned short MipMapCount = 1;
			unsigned char Flags = 0;

			virtual System::String^ ToString() override;
		};



		public ref class Ktex : System::Collections::Generic::IEnumerable<System::Windows::Media::Imaging::BitmapSource^>
		{
		public:
			Ktex();
			~Ktex();
			void Load(System::String^ KTEXpath);
			Ktex(System::String^ KTEXPath);
			void Append(System::Windows::Media::Imaging::BitmapImage^ Image);
			void AddAt(System::Windows::Media::Imaging::BitmapImage^ Image, System::UInt16 pitch);
			System::Windows::Media::Imaging::BitmapSource^ MainMipmap();
			void Convert();
			//props
			property System::Windows::Media::Imaging::BitmapSource^ indexer[unsigned short]
			{
				System::Windows::Media::Imaging::BitmapSource ^ get(unsigned short);
			};
			property KtexInfo^ Info
			{
				KtexInfo^ get();
				void set(KtexInfo^ value);
			}
			property System::String^ OutPut {System::String^ get(); void set(System::String^); };

			//Enumerateing
			ref class KtexEnumerator : System::Collections::Generic::IEnumerator<System::Windows::Media::Imaging::BitmapSource^>
			{
			public:
				KtexEnumerator(Ktex^ parent);
				~KtexEnumerator();
				virtual bool MoveNext();
				virtual void Reset();

				property virtual System::Windows::Media::Imaging::BitmapSource^ Current
				{
					virtual System::Windows::Media::Imaging::BitmapSource^ get();
				};
				property virtual System::Object^ cur_nongeneric
				{
					virtual System::Object^ get() = System::Collections::IEnumerator::Current::get;
				};
			private:
				Ktex^ parent;
				unsigned int i = 0;
			};
			virtual System::Collections::Generic::IEnumerator<System::Windows::Media::Imaging::BitmapSource^>^ GetEnumerator();
			virtual System::Collections::IEnumerator^ _begin() = System::Collections::IEnumerable::GetEnumerator;

			virtual System::String^ ToString() override;
		private:
			ktexlib::KTEXFileOperation::KTEX* native;
		};
	}
}