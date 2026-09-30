#pragma once

#include <ZXingCpp.h>
#include <stb/stb_image_write.h>

#include <fstream>
#include <filesystem>
#include <map>
#include <iostream>

namespace fs = std::filesystem;


enum class FileFormat {
	png, jpg, svg, text
};

inline const std::map<std::string,FileFormat> fileFormat_map {
	{".png", FileFormat::png},
	{".jpg", FileFormat::jpg},
	{".jpeg", FileFormat::jpg},
	{".svg", FileFormat::svg},
	{".txt", FileFormat::text}
};

struct WriteCtx {
	bool textMode = false;
	bool fileMode = false;
	std::string ofile = {};
	ZXing::BarcodeFormat format;
	FileFormat fileFormat = FileFormat::png;
	int size;
	bool margin = true;
	bool terminal = false;
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


	if (ctx.terminal) {
		std::cout << ZXing::WriteBarcodeToUtf8(barcode);
		return;
	}


	auto image = ZXing::WriteBarcodeToImage(
		barcode,
		ZXing::WriterOptions()
			.scale(ctx.size)
			.addQuietZones(ctx.margin)
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
		case FileFormat::svg: {
			std::ofstream ofs(opath);
			if (!ofs) throw std::runtime_error("write(): svg: File open error");
			ofs << ZXing::WriteBarcodeToSVG(barcode);
		} break;
		case FileFormat::text: {
			std::ofstream ofs(opath);
			if (!ofs) throw std::runtime_error("write(): text: File open error");
			ofs << ZXing::WriteBarcodeToUtf8(barcode);
		} break;
	}

	std::cout << "Saved to " << opath.string() << "\n";

}
