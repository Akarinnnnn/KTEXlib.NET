#pragma once
#include "ktexcommon.h"
namespace ktexlib
{
	namespace Atlas
	{
		class __API atlas
		{
		public:
			atlas(std::wstring output, std::vector<ktexlib::KTEXFileOperation::RGBAv2>& imgs);
			atlas(std::wstring output, std::vector<ktexlib::KTEXFileOperation::mipmapv2>& m);
			void xmlgen();
		private:
			std::vector<b_box> bboxes;
			std::filesystem::path path;
		};
	}
}