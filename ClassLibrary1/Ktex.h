#pragma once
#pragma unmanaged
#include <TEXFileOperation.h>
#pragma managed
namespace ktexlib
{
	namespace NET
	{
		//native KTEX exception will not caught by .NET framework

		///<summary>异常类</summary>
		public ref class KTEXException : System::Exception
		{
		public:
			KTEXException(ktexlib::KTEXFileOperation::ktex_exception e);
			///<summary>异常信息</summary>
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
		///<summary>空类，保留</summary>
		public ref struct RgbaImage
		{

		};

		///<summary>材质对应平台</summary>
		public enum class  Platform//platform
		{
			///<summary>OpenGL, mainly PC</summary>
			opengl = 12,
			///<summary>Xbox 360</summary>
			xb360 = 11,
			///<summary>PlayStation 3</summary>
			ps3 = 10,
			///<summary>不应使用</summary>
			unk = 0
		};
		
		///<summary>格式</summary>
		public enum class PixelFromat //pixel form
		{
			///<summary>ARGB</summary>
			ARGB = 4,
			///<summary>bc1</summary>
			DXT1 = 0,
			///<summary>bc2</summary>
			DXT3 = 1,
			///<summary>bc3,default</summary>
			DXT5 = 2,
			///<summary>不应使用</summary>
			unk = 7
		};
		///<summary>材质类型</summary>
		public enum class TextureType
		{
			///<summary>1d</summary>
			d1 = 1,
			///<summary>2d，默认</summary>
			d2 = 2,
			///<summary>3d</summary>
			d3 = 3,
			///<summary>cube mapped</summary>
			cube = 4
		};

		///<summary>ktex信息</summary>
		public ref struct KtexInfo
		{
			PixelFromat PixelFormat;
			Platform PlatForm;
			TextureType TextureType;
			unsigned short MipMapCount;
			unsigned char Flags;

			KtexInfo();
			KtexInfo(int pixfmt, int platfrm, int _textyp, unsigned short mips, unsigned char f);

			///<summary>将该实例的信息转换为string</summary>
			virtual System::String^ ToString() override;
		};


		///<summary>ktex处理类</summary>
		public ref class Ktex : System::Collections::Generic::IEnumerable<System::Windows::Media::Imaging::BitmapSource^>
		{
		public:
			///<summary>默认构造函数，可用于生成ktex</summary>
			Ktex();

			///<summary>加载指定的ktex文件，用于读取</summary>
			Ktex(System::String^ KTEXPath);

			~Ktex();

			///<summary>加载ktex</summary>
			///<param name="KTEXpath">文件路径</param>
			void Load(System::String^ KTEXpath);

			///<summary>添加一张mipmap</summary>
			///<param name="Image">WPF图像</param>
			void Append(System::Windows::Media::Imaging::BitmapSource ^ Image);

			///<summary>添加一张指定了pitch值的mipmap</summary>
			///<param name="Image">WPF图像</param>
			///<param name="pitch">指定的pitch</param>
			void AddAt(System::Windows::Media::Imaging::BitmapSource ^ Image, System::UInt16 pitch);

			///<summary>获取主mipmap</summary>
			System::Windows::Media::Imaging::BitmapSource^ MainMipmap();

			///<summary>根据已有的rgba图像数据转换为ktex</summary>
			void Convert();

			///<summary>索引器</summary>
			property System::Windows::Media::Imaging::BitmapSource^ indexer[unsigned short]
			{
				System::Windows::Media::Imaging::BitmapSource ^ get(unsigned short);
			};

			///<summary>获取和设置ktex信息</summary>
			property KtexInfo^ Info
			{
				KtexInfo^ get();
				void set(KtexInfo^ value);
			}

			///<summary>获取和设置转换产生的ktex路径</summary>
			property System::String^ OutPut {System::String^ get(); void set(System::String^); };

			///<summary>遍历器</summary>
			ref class KtexEnumerator : System::Collections::Generic::IEnumerator<System::Windows::Media::Imaging::BitmapSource^>
			{
			public:
				///<summary>构造遍历器</summary>
				KtexEnumerator(Ktex^ parent);

				~KtexEnumerator();

				///<summary>将当前值移动到下一个值</summary>
				virtual bool MoveNext();

				///<summary>重置遍历器</summary>
				virtual void Reset();

				///<summary>当前值，不可通过赋值修改</summary>
				property virtual System::Windows::Media::Imaging::BitmapSource^ Current
				{
					virtual System::Windows::Media::Imaging::BitmapSource^ get();
				};

				///<summary>System.Collection.IEnumerable的Current属性</summary>
				property virtual System::Object^ cur_nongeneric
				{
					virtual System::Object^ get() = System::Collections::IEnumerator::Current::get;
				};
			private:
				Ktex^ parent;
				unsigned int i = 0;
			};

			///<summary>获取遍历器</summary>
			virtual System::Collections::Generic::IEnumerator<System::Windows::Media::Imaging::BitmapSource^>^ GetEnumerator();

			///<summary>获取非泛型版本的遍历器</summary>
			virtual System::Collections::IEnumerator^ _begin() = System::Collections::IEnumerable::GetEnumerator;

			///<summary>可获取ktex信息和当前输出路径</summary>
			virtual System::String^ ToString() override;
		private:
			ktexlib::KTEXFileOperation::KTEX* native;
		};
	}
}