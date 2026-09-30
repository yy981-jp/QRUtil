#include <CLI/CLI.hpp>
#include <util/image.h>
#include <read.h>
#include <write.h>

#include <iostream>


std::string listNames(const std::map<std::string, ZXing::BarcodeFormat>& names) {
	std::string res;
	for (const auto& e: names)
		res += e.first + "\n";
	return res;
}

const std::map<std::string, ZXing::BarcodeFormat> formats = {
	{"aztec-code", ZXing::BarcodeFormat::AztecCode},
	{"aztec", ZXing::BarcodeFormat::Aztec},
	{"qr-model2", ZXing::BarcodeFormat::QRCodeModel2},
	{"qr", ZXing::BarcodeFormat::QRCode},
	{"pdf417", ZXing::BarcodeFormat::PDF417},
	{"itf", ZXing::BarcodeFormat::ITF},
	{"datamatrix", ZXing::BarcodeFormat::DataMatrix},
	{"codabar", ZXing::BarcodeFormat::Codabar},
	{"upc-e", ZXing::BarcodeFormat::UPCE},
	{"upc-a", ZXing::BarcodeFormat::UPCA},
	{"code128", ZXing::BarcodeFormat::Code128},
	{"ean-13", ZXing::BarcodeFormat::EAN13},
	{"code39-standard", ZXing::BarcodeFormat::Code39Std},
	{"code93", ZXing::BarcodeFormat::Code93},
	{"ean-8", ZXing::BarcodeFormat::EAN8},
	{"code39", ZXing::BarcodeFormat::Code39},
};


int main(int argc, char *argv[]) {
	CLI::App app{"QRCode util : Copyright (c) 2026 yy981"};
	app.require_subcommand(1);


	std::string target;
	auto addCommonOptions = [&](CLI::App* sub) {
		sub->add_option("target", target, "対象のファイルや文字列")->required();
	};


	WriteCtx writeCtx;
	bool no_margin = false;
	// subcmd: write
	auto subWrite = app.add_subcommand("w", "Write (Generate) mode");
	addCommonOptions(subWrite);

	subWrite->add_option("-o", writeCtx.ofile, "出力先")->default_val("code.png");
	subWrite->add_option("--size", writeCtx.size, "2次元コードのサイズ")->default_val(10);
	subWrite->add_flag("--no-margin", no_margin, "余白を生成しない")->default_val(false);
	subWrite->add_flag("--terminal,-m", writeCtx.terminal, "コンソール上に2次元コードを表示")->default_val(false);

	subWrite->add_option("--format", writeCtx.format, 
		"2次元コードの形式\n[利用可能な形式]:\n" + listNames(formats))
		->default_val(ZXing::BarcodeFormat::QRCode)
		->option_text("FORMAT")->transform(CLI::ignore_case)
		->transform(CLI::CheckedTransformer(formats, CLI::ignore_case));

	auto textOpt = subWrite->add_flag("-t,--text", writeCtx.textMode, "Text mode");
	auto fileOpt = subWrite->add_flag("-f,--file", writeCtx.fileMode, "File mode");
	textOpt->excludes(fileOpt);


	// subcmd: read
	auto subRead = app.add_subcommand("r", "Read (Parse) mode");
	addCommonOptions(subRead);
	bool showDetail = false;
	subRead->add_flag("-d,--detail", showDetail, "2次元コードの詳細も表示");



	// parse
	CLI11_PARSE(app, argc, argv);

	if (!writeCtx.textMode && !writeCtx.fileMode) writeCtx.textMode = true;
	writeCtx.margin = !no_margin;


	if (subWrite->parsed()) {
		write(writeCtx, target);
	} else if (subRead->parsed()) {
		const auto& code = parseImg(target);
	
		if (showDetail) {
			std::cout << std::format(
				"形式:\t{}\n"
				"内容:\t{}\n"
				"位置:\t{}\n"
				"回転:\t{}°\n"
				"鏡像:\t{}\n"
				"反転:\t{}\n"
				"ECI:\t{}\n"
				"識別子:\t{}\n",
				ZXing::ToString(code.format()),
				code.text(),
				ZXing::ToString(code.position()),
				code.orientation(),
				code.isMirrored(),
				code.isInverted(),
				code.hasECI(),
				code.symbologyIdentifier()
			);
		} else {
			std::cout << code.text();
			std::cerr << "\n";
		} 

	}
}
