#pragma once

#include <ZXingCpp.h>
#include <stb/stb_image_write.h>

#include <fstream>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;


enum class FileFormat {
	png, jpg
};

inline const std::map<std::string,FileFormat> fileFormat_map {
	{".png", FileFormat::png},
	{".jpg", FileFormat::jpg},
	{".jpeg", FileFormat::jpg},
};

struct WriteCtx {
	bool textMode = false;
	bool fileMode = false;
	std::string ofile = {};
	ZXing::BarcodeFormat format;
	FileFormat fileFormat = FileFormat::png;
	int size;
	bool quietZone = true;
};

struct WriteFormat {
	std::string name;
	ZXing::BarcodeFormat format;
};


void write(const WriteCtx& ctx, const std::string& target) {
	std::string text;
	if (ctx.textMode)
		text = std::move(target);
	else {
		std::ifstream ifs(target);
		if (!ifs) throw std::runtime_error("File open error");

		text.assign((std::istreambuf_iterator<char>(ifs)),
					std::istreambuf_iterator<char>());
	}

	auto barcode = ZXing::CreateBarcodeFromText(
		text,
		ZXing::BarcodeFormat::QRCode
	);

	auto image = ZXing::WriteBarcodeToImage(
		barcode,
		ZXing::WriterOptions()
			.scale(ctx.size)
			.addQuietZones(ctx.quietZone)
	);


	// 出力
	fs::path opath(ctx.ofile);
	if (!opath.has_extension()) opath += ".png";

	const std::string ext = opath.extension().string();
	if (!fileFormat_map.contains(ext)) throw std::runtime_error("This file extension isn't supported.");
	switch (fileFormat_map.at(ext)) {
		case FileFormat::png: {
			stbi_write_png(
				opath.string().c_str(),
				image.width(),
				image.height(),
				1,
				image.data(),
				image.rowStride()
			);
		} break;
		case FileFormat::jpg: {
			stbi_write_jpg(
				opath.string().c_str(),
				image.width(),
				image.height(),
				1,
				image.data(),
				image.rowStride()
			);
		} break;
	}

}
